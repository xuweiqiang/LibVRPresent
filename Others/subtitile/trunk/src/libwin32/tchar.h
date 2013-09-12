//=============================================================================
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO INTERVIDEO, INC.
// 	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//	COMPANY.
//
// 	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// 	PURPOSE.
//
// 	Copyright (c) 2004 - 2005  InterVideo Corporation.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#ifndef _TCHAR_H
#define _TCHAR_H 1

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#if defined(TCHAR)
#undef TCHAR
#endif

#ifndef MAX_PATH
#define MAX_PATH	260
#endif

#ifndef _TCHAR_DEFINED
#if defined(UNICODE) || defined(_UNICODE)
typedef wchar_t TCHAR, *PTCHAR;
typedef unsigned short TBYTE , *PTBYTE ;
#else
typedef char TCHAR, *PTCHAR;
typedef unsigned char TBYTE , *PTBYTE ;
#endif
#define _TCHAR_DEFINED
#endif /* !_TCHAR_DEFINED */

#ifndef MAX_STR_LEN		
#define MAX_STR_LEN			4096
#endif

static inline int _swprintf(wchar_t * _String, const wchar_t * _Format, ...)
{
	va_list ap;
	va_start(ap, _Format);
	return vswprintf(_String, MAX_STR_LEN, _Format, ap);
}

static inline FILE * _wfopen(const wchar_t * _Filename, const wchar_t * _Mode)
{
	char szFileName[MAX_PATH];
	char szMode[10] = {0};
	wcstombs(szFileName, _Filename, MAX_PATH);
	wcstombs(szMode, _Mode, 10);
	FILE *pf = fopen(szFileName, szMode);
	return pf;
}

static inline int _wtoi(const wchar_t *_Str)
{
	wchar_t *pEnd = 0;
	return wcstol(_Str, &pEnd, 10);
}

static inline char * itoa(int _Val, char * _DstBuf, int _Radix)
{
	if (_Radix == 10)
		sprintf(_DstBuf,"%d",_Val);
	else if (_Radix == 16)
		sprintf(_DstBuf,"%x",_Val);
	
	return _DstBuf;
}

#if defined(UNICODE) || defined(_UNICODE)

#if defined ANDROID
	#ifdef __cplusplus
	extern "C" {
	#endif // __cplusplus
	wchar_t *wcsdup(const wchar_t *s);
	#ifdef __cplusplus
		}
	#endif // __cplusplus
	#define _wcsicmp					wcscmp
	#define _wcsnicmp					wcsncmp
#else
	#define _wcsicmp					wcscasecmp
	#define _wcsnicmp					wcsncasecmp
#endif
	#define _tcsdup						wcsdup
	#define _tcslen						wcslen
	#define _tcscpy						wcscpy
	#define _tcscpy_s(a,b,c)			wcscpy(a,c)
	#define _tcsncpy					wcsncpy
	#define _tcsncpy_s(a,b,c,d)			wcsncpy(a,c,d)
	#define _tcscat						wcscat
	#define _tcsncat					wcsncat
	#define _tcscat_s(a,b,c)			wcscat(a,c)
	#define _tcsncicmp					_wcsnicmp
	#define _tcsnicmp					_wcsnicmp
	#define _tcsicmp					_wcsicmp
	#define _tcscmp						wcscmp
	#define _tcsncmp					wcsncmp
	#define _tcschr						wcschr
	#define _tcsrchr					wcsrchr
	#define _tcsstr						wcsstr
	#define _stprintf					_swprintf
	#define _sntprintf					swprintf
	#define _stprintf_s					swprintf
	#define _tprintf					wprintf
	#define _topen						_wopen
	#define _tfopen						_wfopen
	#define _ftprintf					fwprintf
	#define _fgetts						fgetws
	#define _fputts						fputws
	#define _stscanf					swscanf
	#define _vsntprintf					vswprintf
	#define _vsntprintf_s(a,b,c,d,e)	vswprintf(a,b,d,e)

	#define _ttoi						_wtoi
	#define _tstoi						_wtoi
	#define _ttol						_wtoi
	#define _itot						_itow
	#define _ltot						_ltow
	#define _tcstol						wcstol
	#define _tcstoul					wcstoul

	#define _T(x)						L##x
	#define _TEXT						_T

#else

	#define stricmp						strcasecmp
	#define strnicmp					strncasecmp
	#define _tcsdup						_strdup
	#define _tcslen						strlen
	#define _tcscpy						strcpy
	#define _tcscpy_s(a,b,c)			strcpy(a,c)
	#define _tcsncpy					strncpy
	#define _tcsncpy_s(a,b,c,d)			strncpy(a,c,d)
	#define _tcscat						strcat
	#define _tcsncat					strncat
	#define _tcscat_s(a,b,c)			strcat(a,c)
	#define _tcsncicmp					strnicmp
	#define _tcsnicmp					strnicmp
	#define _tcsicmp					stricmp
	#define _tcscmp						strcmp
	#define _tcsncmp					strncmp
	#define _tcschr						strchr
	#define _tcsrchr					strrchr
	#define _tcsstr						strstr
	#define _stprintf					sprintf
	#define _sntprintf					snprintf
	#define _stprintf_s					snprintf
	#define _tprintf					printf
	#define _topen						open
	#define _tfopen						fopen
	#define _ftprintf					fprintf
	#define _fgetts						fgets
	#define _fputts						fputs
	#define _stscanf					sscanf
	#define _vsntprintf					vsnprintf
	#define _vsntprintf_s(a,b,c,d,e)	vsnprintf(a,b,d,e)

	#define _ttoi						atoi
	#define _tstoi						atoi
	#define _ttol						atol
	#define _itot						itoa
	#define _ltot						itoa
	#define _tcstol						strtol
	#define _tcstoul					strtoul

	#define _T(x)						x
	#define _TEXT						_T

	// shall we remove the following definition
	#define wsprintf					sprintf
	#define wcsncpy_s(a,b,c,d)			wcsncpy(a,c,d)
	#define _write						write

#endif





#endif /* _TCHAR_H */
