#ifndef _SUB_SYNC_H_
#define _SUB_SYNC_H_
#include <windows.h>

class CSubSync
{
public:
	CSubSync();
	~CSubSync();
	BOOL Synchronize(DWORD pts, int timeout_value); //unit: ms
	void SetTimeReference(DWORD time_ref);
	DWORD GetTimeReference();
	void DisableSync(BOOL bSync);
	void BreakSync();
	void SetAlarmClock(DWORD trigger_time);
	BOOL TryToTriggerAlarm(DWORD ref_time);
	//DWORD GetTimeValue();
private:
	enum {ALARM_CLOCK_EVENT_NUMBER = 2};
	DWORD m_dwPCR;  //unit: ms
	BOOL m_bSync;
	//HANDLE m_hSyncBreak;
	BOOL m_bUseAlarmClock;
	DWORD m_dwScheduleTime;
	DWORD m_dwTriggerPoint;
	HANDLE m_hAlarmClock[ALARM_CLOCK_EVENT_NUMBER];	//0: Triggered Alarm(in range/out of range).
													//1: Break Alarm (not triggered before quit).
};



#endif
