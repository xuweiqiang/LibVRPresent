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

Module:

	Microsoft Pocket IME (pIME) Ver 2.0

File Name:

	stdinc.h

Abstract:

	Standard include file for pIME.

Notes:


--*/

#ifndef STDINC_H_
#define STDINC_H_

/*
 *	header files
 */
#include <imewarn.h>

#ifndef UNDER_CE
#define _IMM_
#endif // UNDER_CE

#include <windows.h>
#include <windowsx.h>

#ifndef UNDER_CE
#undef _IMM_
#endif // UNDER_CE

#pragma warning(disable:4001)	// single line comment
#include <winnls.h>

#include "strsafe.h"


//IMEDEF.H/C_EQU.H
#define CP_JAPAN (932)
#define	C_MAX_DIC_INPUT		60		/* 辞書見出し最大バイト数（EOS除く） */
#define	C_MAX_DIC_OUTPUT	60		/* 辞書漢字最大バイト数（EOS除く） */

/////////////////////////////////////////////////////
/////     MEMORY FUNCTIONS
/////////////////////////////////////////////////////
#ifdef  UNDER_CE

#define MemAlloc(a)             LocalAllocPtr(LMEM_FIXED, a)
#define LocalAllocPtr(a,b)      (LocalLock(LocalAlloc(a,b)))
#define MemReAlloc(a,b)         LocalReAllocPtr(a, b, LMEM_MOVEABLE)
#define LocalReAllocPtr(a,b,c)  (LocalLock(LocalReAlloc(a,b,c)))
#define MemFree(a)              LocalFreePtr(a)

#define WndStyleEX              WS_EX_TOPMOST | WS_EX_NOACTIVATE
#define CompWndStyleEX          WS_EX_TOPMOST | WS_EX_NOACTIVATE | WS_EX_DLGMODALFRAME

#define BarWndStyle             WS_POPUP | WS_BORDER
#define GuideWndStyle           WS_POPUP | WS_DLGFRAME
#define CandWndStyle            WS_POPUP | WS_CLIPCHILDREN
#define CompWndStyle            WS_POPUP
#define DefCompWndStyle         WS_POPUP

#define SWP_NOSENDCHANGING      0

#ifdef KEY_ALL_ACCESS
#undef KEY_ALL_ACCESS
#endif
#define KEY_ALL_ACCESS          0

#define DictProcName            TEXT("FDictionaryProp")
#define LearnProcName           TEXT("LearningProp")
#define SetActiveUIToolName     TEXT("SetFlagActiveUITool")
#define IsActiveUIToolName      TEXT("IsActiveUITool")
#define IFEDicInstanceName      TEXT("CreateIFEDictionaryInstance")
#define IImeIPointInstanceName  TEXT("CreateIImeIPointInstance")

#else   // UNDER_CE

#define MemAlloc(a)             GlobalAllocPtr(GMEM_MOVEABLE, a)
#define MemReAlloc(a,b)         GlobalReAllocPtr(a, b, GMEM_MOVEABLE)
#define MemFree(a)              GlobalFreePtr(a)

#define WndStyleEX              0
#define CompWndStyleEX          0

#define BarWndStyle             WS_POPUP | WS_BORDER | WS_DISABLED
#define GuideWndStyle           WS_POPUP | WS_BORDER | WS_DISABLED
#define CandWndStyle            WS_COMPDEFAULT | WS_DLGFRAME | WS_CLIPCHILDREN
#define CompWndStyle            WS_COMPNODEFAULT
#define DefCompWndStyle         WS_COMPDEFAULT | WS_DLGFRAME

#define VK_DBE_ALPHANUMERIC              0x0f0
#define VK_DBE_KATAKANA                  0x0f1
#define VK_DBE_HIRAGANA                  0x0f2
#define VK_DBE_SBCSCHAR                  0x0f3
#define VK_DBE_DBCSCHAR                  0x0f4
#define VK_DBE_ROMAN                     0x0f5
#define VK_DBE_NOROMAN                   0x0f6

#define DictProcName            "FDictionaryProp"
#define LearnProcName           "LearningProp"
#define SetActiveUIToolName     "SetFlagActiveUITool"
#define IsActiveUIToolName      "IsActiveUITool"
#define IFEDicInstanceName      "CreateIFEDictionaryInstance"
#define IImeIPointInstanceName      "CreateIImeIPointInstance"

#endif  // UNDER_CE


#ifndef UNDER_CE
#define DEBUGMSG(cond,printf_exp) ((void)0)
#endif // UNDER_CE



#endif //!STDINC_H_
