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

// Contains the data structures and function prototypes for a keyboard PDD.

#ifndef _KEYBDPDD_H_
#define _KEYBDPDD_H_


#include <windows.h>

typedef void (*PFN_KEYBD_PDD_POWER_HANDLER)(UINT uiPddId, BOOL fTurnOff);
typedef void (*PFN_KEYBD_PDD_TOGGLE_LIGHTS)(UINT uiPddId, 
    KEY_STATE_FLAGS KeyStateFlags);

typedef struct tagKEYBD_PDD {
    WORD wPddMask; // Matches the keyboard layout with its PDD
    LPCTSTR pszName; // Used to identify PDD to user
    PFN_KEYBD_PDD_POWER_HANDLER pfnPowerHandler;
    PFN_KEYBD_PDD_TOGGLE_LIGHTS pfnToggleLights;
} KEYBD_PDD, *PKEYBD_PDD;

typedef void (*PFN_KEYBD_EVENT)(UINT uiPddId, UINT32 uiScanCode, 
    BOOL fKeyDown);

typedef BOOL (*PFN_KEYBD_PDD_ENTRY)(UINT uiPddId, 
    PFN_KEYBD_EVENT pfnKeybdEvent, PKEYBD_PDD *ppKeybdPdd);


#endif // _KEYBDPDD_H_
