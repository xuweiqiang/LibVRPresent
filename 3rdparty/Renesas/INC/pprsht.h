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
/*****************************************************************************\
*                                                                             *
* pprsht.h - - Interface for the Windows Property Sheet Pages                  *
*                                                                             *
* Version 1.0                                                                 *
*                                                                             *
*                                                                             *
\*****************************************************************************/

#ifndef _PRSHTP_H_
#define _PRSHTP_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef UNDER_CE // Moved from prsht.h. The following are not supported on WinCE
//++++++++++++++++++++++++++ NOT SUPPORTED ON WINCE ++++++++++++++++++++ 
#define PSN_GETOBJECT           (PSN_FIRST-10)
//++++++++++++++++++++++++++ NOT SUPPORTED ON WINCE ++++++++++++++++++++ 
#endif // UNDER_CE


#define PSCB_NOPRECREATE      1000 /// tells prsht that you don't want
                                    // the pre-create notify, which is a perf win

#define PSM_RESETFORSIP         (WM_USER + 0x300)

#define PSP_ANSI                0x0100  // Used in Unicode version only
#define PSP_SHPAGE              0x0200  // Used in Unicode version only
#define PSP_ALL                 0x07FF
#define PSP_IS16                0x8000
#define PSH_ALL                 0x80003FFF
#ifdef _WIN32
WINCOMMCTRLAPI HPROPSHEETPAGE WINAPI CreateProxyPage32Ex(HPROPSHEETPAGE hpage16, HINSTANCE hinst16);
WINCOMMCTRLAPI HPROPSHEETPAGE WINAPI CreateProxyPage(HPROPSHEETPAGE hpage16, HINSTANCE hinst16);
#endif
// these need to match shell.h's ranges
#define PSN_HASHELP             (PSN_FIRST-4)
#define PropSheet_SetWizButtonsNow(hDlg, dwFlags) PropSheet_SetWizButtons(hDlg, dwFlags)
#ifdef CC_INTERNAL
#include <shellsdk.h> // needed for SHACTIVATEINFO

//
//  Flag values for dwInternalFlags
//

#define PSPI_RESERVED           1

typedef struct _PSP {
#ifdef UNICODE
    DWORD   dwInternalFlags;
    LPVOID  lpANSIPage;
#endif
    PROPSHEETPAGE psp;
    // NOTE: the above member can be variable size so don't add any
    // structure members here
} PSP, FAR *HPROPSHEETPAGE;

typedef struct
{
    HWND hDlg;          // the dialog for this instance data
    PROPSHEETHEADER psh;

    HWND hwndCurPage;   // current page hwnd
    HWND hwndTabs;      // tab control window
    int nCurItem;       // index of current item in tab control
    int idDefaultFallback; // the default id to set as DEFID if page doesn't have one

    int nReturn;
    UINT nRestart;

    int xSubDlg, ySubDlg;       // dimensions of sub dialog
    int cxSubDlg, cySubDlg;

    BOOL fFlags;
    DWORD dwVer;         //Version of COMMCTRL client was built with
    HFONT hFontBold;

    SHACTIVATEINFO sai;
    
    HWND hwndLink;      // Link window
    int  iLinkHeight;   // Height of the lines
} PROPDATA, FAR *LPPROPDATA;
// defines for fFlags
#define PD_NOERASE       0x0001
#define PD_CANCELTOCLOSE 0x0002
#define PD_DESTROY       0x0004

#ifndef _WIN32
//
typedef struct
{
        DWORD style;
        BYTE  cdit;
        WORD  x;
        WORD  y;
        WORD  cx;
        WORD  cy;
} DLGTEMPLATE, FAR *LPDLGTEMPLATE;
typedef const DLGTEMPLATE FAR *LPCDLGTEMPLATE;
#endif

#endif // CC_INTERNAL

#ifdef __cplusplus
}
#endif

#endif // _PRSHTP_H_     //
