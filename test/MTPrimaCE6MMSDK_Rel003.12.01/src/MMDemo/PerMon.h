/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. Platform Software               *
 *                                                                         *
 *    Copyright (c) 2008 by SiRF Technology, Inc.  All rights reserved.    *
 *                                                                         *
 *    This Software is protected by United States copyright laws and       *
 *    international treaties.  You may not reverse engineer, decompile     *
 *    or disassemble this Software.                                        *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains SiRF Technology, Inc.'s confidential and      *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
 *    Software without SiRF Technology, Inc.'s  express written            *
 *    permission.   Use of any portion of the contents of this Software    *
 *    is subject to and restricted by your written agreement with          *
 *    SiRF Technology, Inc.                                                *
 *                                                                         *
 ***************************************************************************/




#include <vector>
#include <assert.h>
#include <stdio.h>
using namespace std;


#ifndef DIM
#define DIM(_array) ((sizeof(_array)/sizeof(_array[0])))
#endif 


#define COUNTER_TO_MS(counter_) ((double)(counter_)*1000/(double)frequency)
#define COUNTER_TO_FPS(counter_) ((double)nFrame*frequency/(counter_))

class PerMon
{
public:
	PerMon(WCHAR* fn)
	{
		item[ID_READ_FILE] = L"Read";
		item[ID_DECODE] = L"Decode";
		item[ID_RENDER] = L"Render";

		Clear();
	
		LARGE_INTEGER t1;
		QueryPerformanceFrequency(&t1);
		frequency = t1.QuadPart;
		assert(frequency);
		
		if (fn)
		{
			m_fp = _wfopen(fn, L"wb");
			if (!m_fp)
			{
				RETAILMSG(1, (L"Can't Open %s\r\n", fn));
			}
		}
	}

	~PerMon()
	{
		if (m_fp)
		{
			FlushToFile();
			fclose(m_fp);
		}
	}
	void StartReadFile()
	{
		StartCount(ID_READ_FILE);

	}
	void EndReadFile()
	{
		EndCount(ID_READ_FILE);

	}

	void StartDecode()
	{
		StartCount(ID_DECODE);
	}

	void EndDecode()
	{
		EndCount(ID_DECODE);
	}
	

	void StartRender()
	{
		StartCount(ID_RENDER);
	}
	
	void EndRender()
	{
		EndCount(ID_RENDER);
		nFrame++;
		
		int i;
		for (i = 0; i < ID_LAST; i++)
		{
			total[i] += counter[i];
		}
		if (m_fp)
		{
			for (int i = 0; i < ID_LAST; i++)
			{
				perfs[i].push_back(counter[i]);
			}
		}

		//clear
		for (i = 0; i < ID_LAST; i++)
		{
			counter[i] = 0;
		}
		
		
	}

	void FlushToFile()
	{
		if (m_fp)
		{
			
			for (size_t i = 0 ; i < perfs[0].size(); i++)
			{
				LONGLONG all = 0;
				for (int j = 0; j < ID_LAST; j++)
				{
					LONGLONG counter = perfs[j][i];
					fwprintf(m_fp, L"%s = %4.3f ms, ", item[j], COUNTER_TO_MS(counter));
					all += counter;
				}
				fwprintf(m_fp, L"%s = %4.3f ms\r\n", L"total", COUNTER_TO_MS(all));
			}

			for (int j = 0; j < ID_LAST; j++)
			{
				perfs[j].clear();
			}
		}
	}

	void GetMemoryStatus(WCHAR* info)
	{
		MEMORYSTATUS stat;
		stat.dwLength = sizeof(MEMORYSTATUS);
		GlobalMemoryStatus(&stat);
		swprintf(info, L"MEMORYSTATUS: dwMemoryLoad = %d,  dwTotalPhys = %d, dwAvailPhys = %d, dwTotalVirtual = %d, dwAvailVirtual = %d\r\n", 
			stat.dwMemoryLoad, stat.dwTotalPhys, stat.dwAvailPhys, stat.dwTotalVirtual, stat.dwAvailVirtual);
	}

	void PrintStatInfo()
	{
		//RETAILMSG(1, (L"nFrame = %d\r\n", nFrame));
		if (nFrame)
		{
			
			WCHAR tmp[MAX_PATH*2];
			GetStatInfo(tmp);
			wprintf(L"%s", tmp);
			
		}
		WCHAR tmp[MAX_PATH*2];
		GetMemoryStatus(tmp);
		wprintf(L"%s", tmp);
	}

	
	void LogStatInfo()
	{
		if (m_fp && nFrame)
		{
			WCHAR tmp[MAX_PATH*2];
			GetStatInfo(tmp);
			fwprintf(m_fp, L"%s", tmp);
			GetMemoryStatus(tmp);
			fwprintf(m_fp, L"%s", tmp);			
		}

	}
	void Clear()
	{
		for (int i = 0; i < ID_LAST; i++)
		{
			perfs[i].clear();

		}		
		memset(total, 0, sizeof(total));
		memset(start, 0, sizeof(start));
		memset(counter,0, sizeof(counter));
		nFrame = 0;
		
	}
	void Log(WCHAR* log)
	{
		if (m_fp)
		{
			fwprintf(m_fp, L"%s\r\n", log);

		}
	}



private:
	void GetStatInfo(WCHAR* info)
	{
		LONGLONG  all = total[ID_READ_FILE] + total[ID_DECODE] + total[ID_RENDER];
		swprintf(info, L"total frame = %d,  %s = %4.3fms, %s = %4.3fms(%4.3f FPS),  %s = %4.3fms, total %4.3fms(%4.3f FPS)\r\n",
			nFrame,	
			item[ID_READ_FILE], COUNTER_TO_MS(total[ID_READ_FILE]),
			item[ID_DECODE], COUNTER_TO_MS(total[ID_DECODE]), COUNTER_TO_FPS(total[ID_DECODE]),
			item[ID_RENDER], COUNTER_TO_MS(total[ID_RENDER]),
			COUNTER_TO_MS(all), 	COUNTER_TO_FPS(all));

			
	}

//#define _PRINT_ITEM_
	void StartCount(int id)
	{
#ifdef _PRINT_ITEM_
		RETAILMSG(1, (L"+%s\r\n", item[id]));
#endif

		LARGE_INTEGER t1;
		QueryPerformanceCounter(&t1);
		start[id] =  t1.QuadPart;

		
	}
	void EndCount(int id)
	{
		LARGE_INTEGER t2;
		QueryPerformanceCounter(&t2);
		counter[id] =  t2.QuadPart - start[id];
#ifdef _PRINT_ITEM_
		RETAILMSG(1, (L"-%s\r\n", item[id]));
#endif
	}


	enum {ID_READ_FILE, ID_DECODE, ID_RENDER, ID_LAST};

	vector<LONGLONG>	perfs[ID_LAST];
	LONGLONG			total[ID_LAST];							//total cost
	LONGLONG			start[ID_LAST];							//start time
	LONGLONG			counter[ID_LAST];						//current counter;
	WCHAR*				item[ID_LAST];							//item name

	DWORD				nFrame;

	LONGLONG            frequency;
	FILE*				m_fp;




	

	

};