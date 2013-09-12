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

mshellap.h

Abstract:  

Notes: 


--*/
#ifndef _INC_MSHELLAPI
#define _INC_MSHELLAPI

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WINCEMACRO
#error WINCEMACRO not defined __FILE__
#endif


#define Shell_NotifyIcon IMPLICIT_DECL(BOOL, SH_SHELL, 6, \
					  (DWORD,PNOTIFYICONDATA))
#define SHAddToRecentDocs IMPLICIT_DECL(void, SH_SHELL, 7, \
							(DWORD, LPCVOID))
#define SHCreateExplorerInstance IMPLICIT_DECL(BOOL, SH_SHELL, 9, \
					  (LPCTSTR, UINT))

#define NotSystemParametersInfo_Trap PRIV_IMPLICIT_DECL(BOOL, SH_SHELL, 27, \
        (DWORD, DWORD, LPVOID, DWORD))
#define SHGetAppKeyAssoc_Trap \
    PRIV_IMPLICIT_DECL(BYTE, SH_SHELL, 28, (LPCTSTR))
#define SHSetAppKeyWndAssoc_Trap \
    PRIV_IMPLICIT_DECL(BOOL, SH_SHELL, 29, (BYTE, HWND))
#define SHCloseApps_Trap \
    PRIV_IMPLICIT_DECL(BOOL, SH_SHELL, 37, (DWORD))
#define SHSipPreference_Trap \
    PRIV_IMPLICIT_DECL(BOOL, SH_SHELL, 38, (HWND, SIPSTATE))
#define SHSetNavBarText_Trap \
    PRIV_IMPLICIT_DECL(BOOL, SH_SHELL, 41, (HWND, LPCTSTR))
#define SHDoneButton_Trap \
    PRIV_IMPLICIT_DECL(BOOL, SH_SHELL, 42, (HWND, DWORD))
#define SHChangeNotifyRegister_Trap \
    PRIV_IMPLICIT_DECL(BOOL, SH_SHELL, 50, (HWND, LPCTSTR, SHCHANGENOTIFYENTRY *, HPROCESS, LPCTSTR, BOOL *))
#define SHFileNotifyRemove_Trap \
    PRIV_IMPLICIT_DECL(BOOL, SH_SHELL, 34, (HWND))
#define SHFileNotifyFree_Trap \
    PRIV_IMPLICIT_DECL(void, SH_SHELL, 35, (FILECHANGENOTIFY *, HPROCESS))
#define SHNotificationAdd_Trap \
    PRIV_IMPLICIT_DECL(LRESULT, SH_SHELL, 55, (SHNOTIFICATIONDATA *, LPTSTR, LPTSTR))
#define SHNotificationUpdate_Trap \
    PRIV_IMPLICIT_DECL(LRESULT, SH_SHELL, 56, (DWORD, SHNOTIFICATIONDATA *, LPTSTR, LPTSTR))
#define SHNotificationRemove_Trap \
    PRIV_IMPLICIT_DECL(LRESULT, SH_SHELL, 57, (const CLSID *, DWORD))
#define SHNotificationGetData_Trap \
    PRIV_IMPLICIT_DECL(LRESULT, SH_SHELL, 58, (const CLSID *, DWORD, \
    SHNOTIFICATIONDATA *, LPTSTR, LPTSTR, DWORD *))
#define SendChangeNotificationToWindow_Trap \
    PRIV_IMPLICIT_DECL(BOOL, SH_SHELL, 73, (HPROCESS))


#ifdef __cplusplus
}
#endif

#endif  /* _INC_MSHELLAPI */
