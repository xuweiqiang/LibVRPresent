#ifndef _VRPRESENT_DP_H_
#define _VRPRESENT_DP_H_

#ifdef __linux__
#include "stdarg.h"
#endif

#ifdef MT3351_DBG
#undef _DEBUG
#undef _DEBUG_CONSOLE
#endif

static void __cdecl DP(LPCSTR szFormat, ...)
{
#ifndef ANDROID
#ifdef __linux__
	TCHAR szBuffer[256];
	va_list vl;
	va_start(vl, szFormat);
	_vsntprintf(szBuffer,255,szFormat, vl);
	szBuffer[255]=0;
	fprintf(stderr,"%s",szBuffer);
	va_end(vl);
#else	
	char buffer[256] = {0};
	char bufferTime[256] = {0};
	SYSTEMTIME time;	
	va_list ap;
	va_start(ap, szFormat);
	_vsnprintf(buffer, 255, (char*)szFormat, ap);
	GetSystemTime(&time);
	sprintf(bufferTime, "%d/%d/%d, %d:%d:%d ", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

#ifdef _DEBUG_CONSOLE
	TCHAR tBufferTime[MAX_PATH];
	TCHAR tBuffer[MAX_PATH];

	MultiByteToWideChar(CP_UTF8, 0, bufferTime, -1, tBufferTime, sizeof(tBufferTime));
	MultiByteToWideChar(CP_UTF8, 0, buffer, -1, tBuffer, sizeof(tBuffer));

	OutputDebugString(tBufferTime);
	OutputDebugString(tBuffer);
#endif
#ifdef _DEBUG
	FILE* f_txt = fopen(".\\vrpresent_log.txt", "a+");
	fprintf(f_txt, bufferTime);
	fprintf(f_txt, buffer);
	fclose(f_txt);
#endif

	va_end(ap);
#endif
#endif
	return;
}

#endif
