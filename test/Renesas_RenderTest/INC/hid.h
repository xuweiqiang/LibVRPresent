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
    hid.h

Abstract:
    Shared header for Human Interface Device (HID) MDD and PDD drivers.

Functions:

Notes:

--*/

#ifndef _HID_H_
#define _HID_H_

#include <windows.h>
#include <hidpi.h>
#include <hidtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IS_HID_REPORT_TYPE_VALID(x) \
    ( ((x) == HidP_Input) || ((x) == HidP_Output) || ((x) == HidP_Feature) )


//
// MDD functions that the PDD calls
//

typedef LPVOID HID_PDD_HANDLE;


// Called with that report descriptor and ID structure of a newly-connected
// device.
BOOL
WINAPI
HidMdd_Attach(
    HID_PDD_HANDLE           hPddDevice,         // PDD function parameter
    PHIDP_REPORT_DESCRIPTOR  pbReportDescriptor, // Raw report descriptor
    DWORD                    cbReportDescriptor, // Bytes in report descriptor
    DWORD                    dwVendorID,         // Device's Vendor ID
    DWORD                    dwProductID,        // Device's Product ID
    DWORD                    dwReleaseNumber,    // Device's Release Number
    DWORD                    dwInterface,        // Device's Interface (0 if NA)
    PVOID                   *ppvNotifyParameter, // MDD function parameter
    PDWORD                   pcbMaxInputReport   // Receives max input report length
    );


// Notifies the MDD of device changes.
BOOL
WINAPI
HidMdd_Notifications(
    DWORD  dwMsg,             // Notification message
    WPARAM wParam,            // Message parameter
    PVOID  pvNotifyParameter  // MDD function parameter
    );

// Notification messages

// Specifies that a device has been detached.
#define HID_MDD_CLOSE_DEVICE    1 // Does not use wParam


// Called by the PDD when a new report arrives. The report is supplied
// in its raw format (ie. the report ID of 0 is not prepended).
DWORD
WINAPI
HidMdd_ProcessInterruptReport(
    PBYTE pData,              // Raw report
    DWORD cbData,             // Bytes in report
    PVOID pvNotifyParameter   // MDD function parameter
    );


// Called by the PDD during XXX_Init. The PDD is responsible for 
// making sure the return value is stored and passed into
// HidMdd_Deinit.
DWORD
WINAPI
HidMdd_Init (
    LPCWSTR lpszDevKey
    );


// Called by the PDD during XXX_Deinit. The PDD is responsible for 
// making sure dwContext is the same value returned from
// HidMdd_Init.
BOOL
WINAPI
HidMdd_Deinit(
    DWORD dwContext
    );


// Called by the PDD during XXX_Open. The PDD is responsible for 
// making sure the return value is stored and passed into
// HidMdd_Close.
DWORD
WINAPI
HidMdd_Open(
    DWORD dwContext,
    DWORD dwAccMode,
    DWORD dwShrMode
    );


// Called by the PDD during XXX_Close. The PDD is responsible for 
// making sure dwContext is the same value returned from
// HidMdd_Open.
BOOL
WINAPI
HidMdd_Close(
    DWORD dwContext
    );


// Called by the PDD during XXX_IOControl. The PDD is responsible for 
// making sure dwContext is the same value returned from
// HidMdd_Open.
//
// IOCTLs with a Function value between 0x300 and 0x3FF are reserved 
// for the MDD.
BOOL
WINAPI
HidMdd_IOControl(
    DWORD          dwContext, 
    DWORD          dwIoControlCode, 
    LPVOID         lpInBuffer, 
    DWORD          nInBufferSize, 
    LPVOID         lpOutBuffer, 
    DWORD          nOutBufferSize, 
    LPDWORD        lpBytesReturned
    );


// Called by the PDD from its DLL entry function. This allows the MDD
// to initialize any MDD global variables.
BOOL 
WINAPI
HidMdd_DllEntry(
    DWORD dwReason
    );


//
// PDD functions that the MDD calls
//
// The MDD guarantees that buffers are not NULL (except where acceptable),
// but the buffers are not completely validated. The PDD should encapsulate 
// buffer accesses in __try - __except blocks.
//


// Get an Input, Output, or Feature report from the device. This is not to be
// be used for recurring interrupt reports. The PDD calls into the MDD with 
// HidMdd_ProcessReport for that.
DWORD
WINAPI
HidPdd_GetReport(
    HID_PDD_HANDLE   hPddDevice,     // PDD function parameter
    HIDP_REPORT_TYPE type,           // Type of report to get
    PCHAR            pbBuffer,       // Buffer for report
    DWORD            cbBuffer,       // Size of the buffer
    PDWORD           pcbTransferred, // Receives the count of bytes transferred
    DWORD            dwTimeout,      // Timeout value in ms
    BYTE             bReportID       // ID of report to get
    );


// Set an Input, Output, or Feature report on the device.
DWORD
WINAPI
HidPdd_SetReport(
    HID_PDD_HANDLE   hPddDevice,     // PDD function parameter
    HIDP_REPORT_TYPE type,           // Type of report to set
    PCHAR            pbBuffer,       // Buffer containing report
    DWORD            cbBuffer,       // Count of bytes in report
    DWORD            dwTimeout,      // Timeout value in ms
    BYTE             bReportID       // ID of report to set
    );


// Get a device string. For predefined types see hiddi.h.
// Call with pszBuffer == NULL to get the character count required 
// (then add 1 for the NULL terminator).
DWORD
WINAPI
HidPdd_GetString(
    HID_PDD_HANDLE  hPddDevice,// PDD function parameter
    HID_STRING_TYPE stringType,// Type of string to get
    DWORD           dwIdx,     // Used with stringType == HID_STRING_INDEXED
    LPWSTR          pszBuffer, // Set to NULL to get character count
    DWORD           cchBuffer, // Count of chars that will fit into pszBuffer
                               // including the NULL terminator.
    PDWORD          pcchActual // Count of chars in the string NOT including
                               // the NULL terminator
    );


// Get the idle rate for a specific report.
DWORD
WINAPI
HidPdd_GetIdle(
    HID_PDD_HANDLE hPddDevice, // PDD function parameter
    PDWORD         pdwIdle,    // Receives the idle rate
    DWORD          dwReportID  // Report ID
    );


// Set the idle rate for a specific report. The idle rate is expressed in
// 4 ms increments, so to set idle rate of 20 ms, bDuration should be 5.
// bDuration of 0 means infinite. bReportID of 0 means to apply to all
// reports.
DWORD
WINAPI
HidPdd_SetIdle(
    HID_PDD_HANDLE hPddDevice, // PDD function parameter
    DWORD          dwDuration, // Time between duplicate reports
    DWORD          dwReportID  // Report ID
    );


// Commands the PDD to initiate some activity or configuration.
DWORD
WINAPI
HidPdd_IssueCommand(
    HID_PDD_HANDLE hPddDevice, // PDD function parameter
    DWORD          dwMsg,      // Notification message
    WPARAM         wParam      // Message parameter
    );

// Command messages

// wParam == 0 to disable, non-zero to enable
#define HID_PDD_REMOTE_WAKEUP   1



#ifdef DEBUG

#define SETFNAME(name)          const TCHAR * const pszFname = (name)

// Debug zone defs
#define ZONE_ERROR              DEBUGZONE(0)
#define ZONE_WARNING            DEBUGZONE(1)
#define ZONE_INIT               DEBUGZONE(2)
#define ZONE_FUNCTION           DEBUGZONE(3)

#define ZONE_HID_DATA           DEBUGZONE(4)
#define ZONE_COMMENT            DEBUGZONE(5) 

#define ZONE_PARSE              DEBUGZONE(8)

// Zones 10-15 are reserved for the PDD


//
// ***** Debug Utility Functions (provided by the MDD) *****
// 

HLOCAL
HidAlloc(
    SIZE_T cb
    );

HLOCAL
HidFree(
    LPVOID ptr
    );

BOOL
DebugIsLocked(
    INT cInCS
    );

void
DebugLock(
    LPCRITICAL_SECTION pCS,
    INT *pcInCS
    );

void
DebugUnlock(
    LPCRITICAL_SECTION pCS,
    INT *pcInCS
    );

#else

#define SETFNAME(name)

#define HidAlloc(cb) LocalAlloc(LMEM_FIXED, cb)
#define HidFree(ptr) LocalFree(ptr)
#define DebugIsLocked(ptr)
#define DebugLock(ptr1, ptr2)
#define DebugUnlock(ptr1, ptr2)

#endif // DEBUG


#ifdef __cplusplus
}
#endif

#endif // _HID_H_

