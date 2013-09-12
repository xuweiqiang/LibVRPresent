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

#ifndef _KEYBDIST_H_
#define _KEYBDIST_H_

typedef UINT (*PFN_KEYBD_PDD_GET_KEYBD_EVENT)(UINT uiPddId, 
    UINT32 rguiScanCode[16], BOOL rgfKeyUp[16]);

typedef struct tagKEYBD_IST {
    HANDLE hevInterrupt;
    DWORD dwSysIntr_Keybd;
    UINT uiPddId;
    PFN_KEYBD_PDD_GET_KEYBD_EVENT pfnGetKeybdEvent;
    PFN_KEYBD_EVENT pfnKeybdEvent;
} KEYBD_IST, *PKEYBD_IST;

BOOL
KeybdIstLoop(
	PKEYBD_IST pKeybdIst
	);

#endif
