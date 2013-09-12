#include "SUBSync.h"

#include <stdio.h>
#include <math.h>
#include <assert.h>

#define PCR_VALID_BLUR_TIME 200  //(ms)This value is invalid, come from my mind.
#define INFINITE_SCHEDULE 0xffffffff

#ifdef DEBUG_INFO
//#define _DP_FILE
static void __cdecl DP(LPCSTR szFormat, ...)
{
	char szBuffer[256]; 
	va_list vl; 
	va_start(vl, szFormat);

#ifdef __linux
	vsnprintf(szBuffer,255,szFormat,vl);
#else
	_vsnprintf(szBuffer,255,szFormat,vl);
#endif
#ifdef _DP_FILE
	FILE* f_dp = fopen("dp.txt", "at");
	fprintf(f_dp, szBuffer);
	fclose(f_dp);
#else
	szBuffer[255]=0;
#ifdef _WIN32_WCE
	{
		WCHAR szBufferW[256];

		MultiByteToWideChar(CP_ACP,0,szBuffer,-1,szBufferW,255);
		szBufferW[255]=0;
		OutputDebugString(szBufferW);
	}
#else

#ifdef __linux__
	fprintf(stderr,"%s",szBuffer); fflush(stderr);
#else
	OutputDebugStringA(szBuffer);
#endif
#endif
#endif
	va_end(vl);
}
#else
#define DP(x,...) x
#define DPW(x,...) x
#endif

CSubSync::CSubSync()
{
	m_dwPCR = 0xffffffff;	//Init value
	m_bSync = TRUE;  //born to be Sync!
	//m_hSyncBreak = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_bUseAlarmClock = FALSE;
	m_dwScheduleTime = INFINITE_SCHEDULE;
	for (int i=0;i<ALARM_CLOCK_EVENT_NUMBER;i++)
	{
		m_hAlarmClock[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
}

CSubSync::~CSubSync()
{

}

//Once PCR is over PTS for at most PCR_VALID_BLUR_TIME, show it.
BOOL CSubSync::Synchronize(DWORD pts, int timeout_value)
{
	if (!m_bSync)	//If dump, Synchronize will always return TRUE immediately.
		return TRUE;
#if 0
	/*Another way to sync: Calculate the interval time and sleep it*/
	UINT64 PCR_value = m_dwPCR;  //unit: ms
	UINT64 interval_ms = (PCR_value >= pts) ? (PCR_value - pts) : 0;  //unit: ms
	if (interval_ms)
	{
		if (interval_ms < PCR_VALID_BLUR_TIME)
		{
			DP("[Subtitle]Sync successful!\n");
			return TRUE;
		}
		else
		{
			DP("[Subtitle]PTS is too old, give up!\n");
			return FALSE;
		}
	}
	else
	{
		if (pts - PCR_value > timeout_value)
		{
			DP("[Subtitle]PCR falls behind unusually, please check it!\n");
			return FALSE;
		}
		//DP("[Subtitle]Waiting for PCR...\n");
		
		while(WaitForSingleObject(m_hSyncBreak, pts - PCR_value) == WAIT_TIMEOUT)
		{
			PCR_value = m_dwPCR;
			if (PCR_value >= pts)
			{
				UINT64 dwPass = PCR_value - pts;
				//DP("[Subtitle]Over pass = %d\n", dwPass);
				if (dwPass < PCR_VALID_BLUR_TIME)
				{
					return TRUE;
				}
				if (PCR_value - pts >= PCR_VALID_BLUR_TIME)	//In case of m_dwPCR is beyond boarder.
				{
					//DP("[Subtitle]PCR updates over_quickly, may need to enlarge the range.\n");
					return FALSE;
				}
			}
			//else
				//DP("[Subtitle]After sleep PCR does not catch up.\n");	
		}
		return FALSE;
	}
#endif
	
#if 0
	int start_time = GetTickCount();
	DP("[Subtitle]Begin sync with PTS: %d!\n", pts);
	while (GetTickCount() - start_time < timeout_value)
	{
		DWORD PCR_value = m_dwPCR;  //unit: ms

		DWORD interval_ms = (PCR_value >= pts) ? (PCR_value - pts) : 0;  //unit: ms
		if (interval_ms)
		{
			if (interval_ms < PCR_VALID_BLUR_TIME)
			{
				DP("[Subtitle]Sync successful!\n");
				return TRUE;
			}
			else
			{
				DP("[Subtitle]PTS is too old, give up!\n");
				return FALSE;
			}
		}
		else
		{
			DP("[Subtitle]Waiting for PCR!\n");
			Sleep(PCR_VALID_BLUR_TIME/2);
		}
	}
	DP("[Subtitle]PTS is too new, cannot reach!\n");
#endif

	BOOL ret = FALSE;
	SetAlarmClock(pts);
	m_bUseAlarmClock = TRUE;
	DWORD dwWaitReturn = WaitForMultipleObjects(ALARM_CLOCK_EVENT_NUMBER, m_hAlarmClock, FALSE, timeout_value);
	if (dwWaitReturn != WAIT_TIMEOUT)
	{
		if (WaitForSingleObject(m_hAlarmClock[1], 0) == WAIT_OBJECT_0)		//1: Break Alarm (not triggered before quit).
		{
			ResetEvent(m_hAlarmClock[1]);
			ret = FALSE;
			goto endding;
		}
		if (WaitForSingleObject(m_hAlarmClock[0], 0) == WAIT_OBJECT_0)		//0: Triggered Alarm(in range/out of range).
		{
			ResetEvent(m_hAlarmClock[0]);
			assert(m_dwTriggerPoint > m_dwScheduleTime);
			if (m_dwTriggerPoint - m_dwScheduleTime < PCR_VALID_BLUR_TIME)
			{
				ret = TRUE;
			}
			else 
			{
				DP("[Subtitle]PCR updates over_quickly, may need to enlarge the range.\n");
				ret = FALSE;
			}
			goto endding;
		}
	}

endding:
	m_bUseAlarmClock = FALSE;
	m_dwScheduleTime = INFINITE_SCHEDULE;	//Reset schedule.
	return ret;
}

void CSubSync::SetTimeReference(DWORD time_ref)  //unit: ms
{
#if 0
	static DWORD start_tick = GetTickCount();
	static DWORD start_pcr = time_ref;
	DWORD present_tick = GetTickCount();
	DWORD present_pcr = time_ref;
	DP("tick = %d, pcr = %d, tick-pcr = %d.\n", present_tick-start_tick, present_pcr-start_pcr, (present_tick-start_tick)-(present_pcr-start_pcr));
#endif
	
	m_dwPCR = time_ref;
	if (m_bUseAlarmClock)
	{
		TryToTriggerAlarm(time_ref);
	}
}

void CSubSync::SetAlarmClock(DWORD trigger_time)
{
	assert(m_dwScheduleTime == INFINITE_SCHEDULE);
	m_dwScheduleTime = trigger_time;
}

BOOL CSubSync::TryToTriggerAlarm(DWORD ref_time)
{
	BOOL bTriggerSuccess = FALSE;
	if (ref_time > m_dwScheduleTime)
	{
		DP("[Subtitle]Trigger point pass Schedule time by %d.\n", ref_time - m_dwScheduleTime);
		m_dwTriggerPoint = ref_time;
		m_bUseAlarmClock = FALSE;		//Close alarm.
		SetEvent(m_hAlarmClock[0]);		//0: Triggered Alarm(in range/out of range).
		bTriggerSuccess = TRUE;
	}
	return bTriggerSuccess;
}

void CSubSync::DisableSync(BOOL bSync)
{
	m_bSync = !bSync;
}

DWORD CSubSync::GetTimeReference()	//unit: ms
{
	return m_dwPCR;
}

void CSubSync::BreakSync()
{
	//SetEvent(m_hSyncBreak);
	SetEvent(m_hAlarmClock[1]);				//1: Break Alarm (not triggered before quit).
}
