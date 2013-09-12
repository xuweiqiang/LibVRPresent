//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  

outmsg.h

Abstract:

Simple set of functions to allow debug/console output in utility programs.

Notes: 

This is a simple function that can be used in place of NKDbgPrintfW()
and _tprintf() to do the correct thing when used on a system with a
console or without.  By default if the system will support a console
(i.e. has wprintf in coredll) then it will use it.  Otherwise it will
use OutputDebugString() to send the result to the debugger.  You can
override the default behavior by setting v_fDebugOut to TRUE which
will always force the output to the debugger.  It's recommended that
the command line option "-d" be used to switch to Debugger output.

This include file should be in all .c (or .cpp) files that need to use
the function.  In one of the .c files you need to #define DECLARE_OUTMSG
which actually instantiates the code (do this above the #include of outmsg.h).

For example:
	Main .c file:
	#include "windows.h"
	#include "tchar.h"
	#define DECLARE_OUTMSG
	#include "outmsg.h"


	Other .c files:
	#include "windows.h"
	#include "tchar.h"
	#include "outmsg.h"


Function usage.  Use the function like _tprintf().  It will do the correct thing
for NT or NTANSI builds as well as CE (unicode) builds.  If you want to support
NTANSI builds then be aware of the paramters for ANSI strings (i.e. use %hs for
an ansi string).

--*/
#ifndef _OUTMSG_H_
#define _OUTMSG_H_

#ifdef __cplusplus
extern "C" {
#endif


	
extern DWORD	v_fDebugOut;

extern int OutputMessage (TCHAR *pFormat, ...);



#ifdef DECLARE_OUTMSG
#ifdef UNDER_CE
typedef int (__cdecl *PFN_wprintf)(const wchar_t *, ...);
PFN_wprintf	v_pfn_wprintf;
HMODULE v_hCoreDLL;
#endif

DWORD v_fDebugOut;

int
OutputMessage (TCHAR *pFormat, ...)
{
	va_list ArgList;
	TCHAR	Buffer[256];
	int		RetVal;

	va_start (ArgList, pFormat);

	RetVal = _vsntprintf(Buffer, 255, pFormat, ArgList);
	Buffer[255] = TEXT('\0'); // Force NULL-termination

	if (!v_fDebugOut) {
#ifdef UNDER_CE
		if (v_pfn_wprintf == NULL) {
			// Since not all configs contain the wprintf function we'll
			// try to find it.  If it's not there we'll default to using
			// OutputDebugString.
			v_hCoreDLL = LoadLibrary(TEXT("coredll.dll"));
			if (v_hCoreDLL) {
				v_pfn_wprintf = (PFN_wprintf)GetProcAddress(v_hCoreDLL, TEXT("wprintf"));
			}
			
		}
		if (v_pfn_wprintf != NULL) {
			(v_pfn_wprintf) (TEXT("%s"), Buffer);
		} else {
			// Couldn't find the entry point, revert to OutputDebugString()
			v_fDebugOut = TRUE;
		}
#else
		_tprintf (TEXT("%s"), Buffer);
#endif
	}
	
	if (v_fDebugOut) {
		OutputDebugString (Buffer);
	}

	return RetVal;
}

#endif

#ifdef __cplusplus
}
#endif

#endif // _OUTMSG_H_
