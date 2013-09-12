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

termctrl.h

Abstract:  

Functions:

Notes:


--*/
#ifndef _TERMCTRL_H_
#define _TERMCTRL_H_

//====== TERMINAL EMULATOR CONTROL ===========================================


#ifdef __cplusplus
extern "C" {
#endif

#define TERMCTRL_CLASSNAME   TEXT("TermCtrl")

typedef enum {
   TW_SUCCESS = 0,         // User exitted normally (OK button)
   TW_CANCEL,              // User clicked the close 'X' (but may have typed okay)
   TW_FAILURE              // There was a failure creating the window
} TW_RET_TYPE;

TW_RET_TYPE
TerminalWindow(
   HANDLE  hCommPort,
   LPCTSTR szWindowTitle,
   HWND*   phWnd
   );

typedef TW_RET_TYPE (* PFNTERMINALWINDOW) (
   HANDLE  hCommPort,
   LPCTSTR szWindowTitle,
   HWND*   phWnd
   );

#ifdef __cplusplus
}
#endif

#endif  // _TERMCTRL_H_


