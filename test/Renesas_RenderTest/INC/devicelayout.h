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
#ifndef _DEVICE_LAYOUT_H_
#define _DEVICE_LAYOUT_H_

#include <windows.h>

typedef struct {
    UINT32  uPrefixMask;
    UINT32  uMin;
    UINT32  uMax;
    UINT8  *puScanCodeToVKey;
} ScanCodeToVKeyData;


typedef struct _KEYBD_EVENT {
    UINT32 uiVk;
    UINT32 uiSc;
    KEY_STATE_FLAGS KeyStateFlags;
} KEYBD_EVENT, *PKEYBD_EVENT;


// Remapping function typedef
// Returns the number of remapped events placed in pRmpKbdEvents
// Call with pRmpKbdEvents == NULL anc cMaxRmpKbdEvents == 0 to get the
// maximum size necessary for pRmpKbdEvents buffer.
typedef UINT (*PFN_KEYBD_REMAP)(
    const KEYBD_EVENT *pKbdEvents,      // List of events to remap
    UINT               cKbdEvents,      // Count of events in pKbdEvents
    KEYBD_EVENT       *pRmpKbdEvents,   // Buffer where remapped events will be placed
    UINT               cMaxRmpKbdEvents // Maximum number of remapped events
    );

typedef struct tagDEVICE_LAYOUT {
    DWORD dwSize;
    
    WORD wPddMask; // Matches the device layout with its PDD

    // Scan code to virtual key
    ScanCodeToVKeyData **rgpscvk;
    UINT cpscvk;

    // Remapping functions
    PFN_KEYBD_REMAP     pfnRemapKey;
} DEVICE_LAYOUT, *PDEVICE_LAYOUT;


typedef BOOL (*PFN_DEVICE_LAYOUT_ENTRY)(PDEVICE_LAYOUT pDeviceLayout);


#endif // _DEVICE_LAYOUT_H_
