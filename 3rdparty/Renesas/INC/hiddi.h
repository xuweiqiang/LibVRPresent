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

#ifndef _HIDDI_H_
#define _HIDDI_H_

#include <windows.h>
#include "hidusage.h"
#include "hidpddi.h"
#include "hidpi.h"
#include "hidtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef LPVOID HID_HANDLE;
typedef struct _HID_FUNCS const * PCHID_FUNCS;


// Structure that describes how a HID client was loaded.
typedef struct _HID_DRIVER_SETTINGS {
    // Vendor specific (also Device Specific)
    DWORD dwVendorId;
    DWORD dwProductId;
    DWORD dwReleaseNumber;

    // Interface specific
    DWORD dwInterfaceNumber;
    DWORD dwCollection;

    // Report specific per top level collection
    DWORD dwUsagePage;
    DWORD dwUsage;
} HID_DRIVER_SETTINGS, *PHID_DRIVER_SETTINGS;

#define HID_NO_INFO 0xFFFFFFFF // Default value. Each byte must be the same.


/******************************************************************************/
//These are entry points which all Client Drivers MUST have

BOOL WINAPI HIDDeviceAttach(
    HID_HANDLE                 hDevice, 
    PCHID_FUNCS                pHidFuncs,
    const HID_DRIVER_SETTINGS *pDriverSettings,
    PHIDP_PREPARSED_DATA       phidpPreparsedData,
    PVOID                     *ppvNotifyParameter,
    DWORD                      dwUnused
    );

BOOL WINAPI HIDDeviceNotifications(
    DWORD  dwMsg,            // Notification code
    WPARAM wParam,           // Message parameter
    PVOID  pvNotifyParameter // Client-supplied value
    );


// Client function typedefs and strings.
#define SZ_HID_CLIENT_ATTACH _T("HIDDeviceAttach")
typedef BOOL (WINAPI *LPHID_CLIENT_ATTACH) (
    HID_HANDLE                 hDevice, 
    PCHID_FUNCS                pHidFuncs, 
    const HID_DRIVER_SETTINGS *pDriverSettings, 
    PHIDP_PREPARSED_DATA       phidpPreparsedData,
    PVOID                     *ppvNotifyParameter,
    DWORD                      dwUnused
    );

#define SZ_HID_CLIENT_NOTIFICATIONS _T("HIDDeviceNotifications")
typedef BOOL (WINAPI *LPHID_CLIENT_NOTIFICATIONS) (
    DWORD  dwMsg,             // Notification code
    WPARAM wParam,            // Message parameter
    PVOID  pvNotifyParameter  // Client-supplied value
    );

// Notification messages
#define HID_CLOSE_DEVICE 1 // Does not use wParam


typedef DWORD (WINAPI *LPGET_REPORT) (
    HID_HANDLE       hDevice,
    HIDP_REPORT_TYPE type,
    PCHAR            pbBuffer,
    DWORD            cbBuffer,
    PDWORD           pcbTransferred,
    DWORD            dwTimeout
    );

typedef DWORD (WINAPI *LPSET_REPORT) (
    HID_HANDLE       hDevice,
    HIDP_REPORT_TYPE type,
    PCHAR            pbBuffer,
    DWORD            cbBuffer,
    DWORD            dwTimeout
    );

typedef DWORD (WINAPI *LPGET_INTERRUPT_REPORT) (
    HID_HANDLE hDevice,
    PCHAR      pbBuffer,
    DWORD      cbBuffer, 
    PDWORD     pcbTransferred,
    HANDLE     hCancel,
    DWORD      dwTimeout
    );

typedef DWORD (WINAPI *LPGET_STRING) (
    HID_HANDLE      hDevice,
    HID_STRING_TYPE stringType,
    DWORD           dwIdx,     // Only used with stringType == HID_STRING_INDEXED 
    LPWSTR          pszBuffer, // Set to NULL to get character count
    DWORD           cchBuffer, // Count of chars that will fit into pszBuffer
                               // including the NULL terminator.
    PDWORD          pcchActual // Count of chars in the string NOT including 
                               // the NULL terminator
    );

typedef DWORD (WINAPI *LPGET_QUEUE_SIZE) (
    HID_HANDLE hDevice,
    PDWORD     pdwSize
    );

typedef DWORD (WINAPI *LPSET_QUEUE_SIZE) (
    HID_HANDLE hDevice,
    DWORD      dwSize
    );



// Function Table
struct _HID_FUNCS {
    DWORD                               dwCount; // Size of structure

    // The following are HID driver functions. See client.cpp for descriptions.
    LPGET_REPORT                        lpGetReport;
    LPSET_REPORT                        lpSetReport;
    LPGET_INTERRUPT_REPORT              lpGetInterruptReport;
    LPGET_STRING                        lpGetString;
    LPGET_QUEUE_SIZE                    lpGetQueueSize;
    LPSET_QUEUE_SIZE                    lpSetQueueSize;
};

typedef struct _HID_FUNCS HID_FUNCS, * PHID_FUNCS, * LPHID_FUNCS;
typedef struct _HID_FUNCS const * PCHID_FUNCS;
typedef struct _HID_FUNCS const * LPCHID_FUNCS;


#ifdef __cplusplus
}
#endif

#endif // _HIDDI_H_

