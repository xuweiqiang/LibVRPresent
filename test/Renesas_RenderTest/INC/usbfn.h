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

        USBFN.H

Abstract:

        Shared header for USB Function MDD and PDD drivers.

--*/

#ifndef _USBFN_H_
#define _USBFN_H_

#include <usbfntypes.h>
#include <usbfnioctl.h>
#include <bldver.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifndef dim
#define dim(x) (sizeof(x)/sizeof((x)[0]))
#endif

#define GARBAGE_DWORD 0xCCCCCCCC


// Transfer structure passed to the PDD from the MDD in IssueTransfer.
typedef struct _STransfer {
    DWORD               dwCallerPermissions;
    DWORD               dwFlags;
    PVOID               pvBuffer;
    DWORD               dwBufferPhysicalAddress;
    DWORD               cbBuffer;
    DWORD               cbTransferred;
    DWORD               dwUsbError; // Possible values are in usbfntypes.h

    PVOID               pvPddData; // PDD can do whatever it likes with this
    PVOID               pvPddTransferInfo; // Specific to PDD from client
} STransfer, *PSTransfer;

// Check dwFlags.
#define TRANSFER_IS_IN(pTransfer)   (pTransfer->dwFlags & USB_REQUEST_DEVICE_TO_HOST)
#define TRANSFER_IS_OUT(pTransfer)  (!TRANSFER_IS_IN(pTransfer))


// Called by MDD's UFN_Deinit routine.
typedef DWORD (WINAPI *PFN_UFN_PDD_DEINIT) (
    PVOID pvPddContext
    );

// Return something other than ERROR_SUCCESS if there is no way this 
// configuration could be supported.
// If ERROR_SUCCESS is returned, the MDD will proceed to call 
// IsEndpointSupportable for each endpoint in the configuration and each speed.
typedef DWORD (WINAPI *PFN_UFN_PDD_IS_CONFIGURATION_SUPPORTABLE) (
    PVOID                       pvPddContext,
    UFN_BUS_SPEED               Speed,
    PUFN_CONFIGURATION          pConfiguration
    );

// Called to handshake the supportable endpoint values. The MDD calls
// this routine for endpoint descriptor in order to find a "physical"
// endpoint that it will work with. If the PDD returns failure, the MDD 
// will try again with the next "physical" endpoint until it finds a 
// match or runs out of "physical" endpoints.
// 
// The PDD may change the wMaxPacketSize and bEndpointAddress in the 
// endpoint descriptor in order to make this work.
typedef DWORD (WINAPI *PFN_UFN_PDD_IS_ENDPOINT_SUPPORTABLE) (
    PVOID                       pvPddContext,
    DWORD                       dwEndpoint,
    UFN_BUS_SPEED               Speed,
    PUSB_ENDPOINT_DESCRIPTOR    pEndpointDesc,
    BYTE                        bConfigurationValue,
    BYTE                        bInterfaceNumber,
    BYTE                        bAlternateSetting
    );

// The PDD should enable the endpoint for send/receive.
typedef DWORD (WINAPI *PFN_UFN_PDD_INIT_ENDPOINT) (
    PVOID                       pvPddContext,
    DWORD                       dwEndpoint,
    UFN_BUS_SPEED               Speed,
    PUSB_ENDPOINT_DESCRIPTOR    pEndpointDesc,
    PVOID                       pvReserved,
    BYTE                        bConfigurationValue,
    BYTE                        bInterfaceNumber,
    BYTE                        bAlternateSetting
    );

// Defines for backwards compatibility.
#define LOCK_PDD_ENDPOINT(pv)
#define UNLOCK_PDD_ENDPOINT(pv)

// Called to give the PDD the full set of descriptors. The purpose here is 
// for the PDD to load the descriptors into the function controller if
// that is required.
typedef DWORD (WINAPI *PFN_UFN_PDD_REGISTER_DEVICE) (
    PVOID                           pvPddContext,
    PCUSB_DEVICE_DESCRIPTOR         pHighSpeedDeviceDesc,
    PCUFN_CONFIGURATION             pHighSpeedConfig,
    PCUSB_CONFIGURATION_DESCRIPTOR  pHighSpeedConfigDesc,
    PCUSB_DEVICE_DESCRIPTOR         pFullSpeedDeviceDesc,
    PCUFN_CONFIGURATION             pFullSpeedConfig,
    PCUSB_CONFIGURATION_DESCRIPTOR  pFullSpeedConfigDesc,
    PCUFN_STRING_SET                pStringSets,
    DWORD                           cStringSets
    );

// Called to notify the PDD that the descriptors are changing.
typedef DWORD (WINAPI *PFN_UFN_PDD_DEREGISTER_DEVICE) (
    PVOID                           pvPddContext
    );


// Called to tell the PDD to start its IST, connect to the bus, etc.
typedef DWORD (WINAPI *PFN_UFN_PDD_START) (
    PVOID        pvPddContext
    );

// Tells the PDD to stop its IST, disconnect from the bus, etc.
typedef DWORD (WINAPI *PFN_UFN_PDD_STOP) (
    PVOID        pvPddContext
    );

// Issue a transfer on the given physical endpoint. The PDD owns the
// transfer until it calls pfnNotify with UFN_MSG_TRANSFER_COMPLETE.
typedef DWORD (WINAPI *PFN_UFN_PDD_ISSUE_TRANSFER) (
    PVOID  pvPddContext,
    DWORD  dwEndpoint,
    PSTransfer pTransfer
    );

// Abort the given transfer on the physical endpoint.
// The MDD will guarantee that this transfer is actually valid
// on that endpoint.
typedef DWORD (WINAPI *PFN_UFN_PDD_ABORT_TRANSFER) (
    PVOID           pvPddContext,
    DWORD           dwEndpoint,
    PSTransfer      pTransfer
    );

// Turn off an endpoint.
typedef DWORD (WINAPI *PFN_UFN_DEINIT_ENDPOINT) (
    PVOID           pvPddContext,
    DWORD           dwEndpoint
    );

// Stall an endpoint.
typedef DWORD (WINAPI *PFN_UFN_STALL_ENDPOINT) (
    PVOID           pvPddContext,
    DWORD           dwEndpoint
    );

// Clear an endpoint stall.
typedef DWORD (WINAPI *PFN_UFN_CLEAR_ENDPOINT_STALL) (
    PVOID           pvPddContext,
    DWORD           dwEndpoint
    );

// Signal that the data phase of a control transfer is complete.
// Some PDDs may ignore this if the function controllers handle
// this automatically.
typedef DWORD (WINAPI *PFN_UFN_SEND_CONTROL_STATUS_HANDSHAKE) (
    PVOID           pvPddContext,
    DWORD           dwEndpoint
    );

// Called by the MDD to set the addres on the bus.
// Some PDDs may ignore this if the function controllers handle
// this automatically.
typedef DWORD (WINAPI *PFN_UFN_SET_ADDRESS) (
    PVOID pvPddContext,
    BYTE  bAddress
    );

// Called to get the halted status of an endpoint.
typedef DWORD (WINAPI *PFN_UFN_IS_ENDPOINT_HALTED) (
    PVOID pvPddContext,
    DWORD dwEndpoint,
    PBOOL pfHalted
    );

// Called to initiate a remote wakeup to the host.
typedef DWORD (WINAPI *PFN_UFN_INITIATE_REMOTE_WAKEUP) (
    PVOID pvPddContext
    );

// Called in response to UFN_PowerDown.
typedef void (WINAPI *PFN_UFN_POWER_DOWN) (
    PVOID pvPddContext
    );

// Called in response to UFN_PowerUp.
typedef void (WINAPI *PFN_UFN_POWER_UP) (
    PVOID pvPddContext
    );

// Called in response to UFN_IOControl and also for MDD-specific IOCTLs.
// The source of the IOCTL is given in the source parameter.
typedef enum _IOCTL_SOURCE {
    BUS_IOCTL = 0,
    EXTERN_IOCTL,
    MDD_IOCTL,
} IOCTL_SOURCE;

typedef DWORD (WINAPI *PFN_UFN_IOCONTROL) (
    PVOID           pvPddContext,
    IOCTL_SOURCE    source,
    DWORD           dwCode,
    PBYTE           pbInBuf,
    DWORD           cbInBuf,
    PBYTE           pbOutBuf,
    DWORD           cbOutBuf,
    PDWORD          pcbActualOutBuf
    );


// Notification routine for the device. See usbfntypes.h for most 
// notification values.
typedef BOOL (WINAPI *PFN_UFN_MDD_NOTIFY) (
    PVOID pvMddContext,
    DWORD dwMsg,
    DWORD dwParam
    );


// Structure that the MDD passes to UfnPdd_Init.
typedef struct _UFN_MDD_INTERFACE_INFO {
    DWORD               dwVersion;

    PFN_UFN_MDD_NOTIFY  pfnNotify;
} UFN_MDD_INTERFACE_INFO, *PUFN_MDD_INTERFACE_INFO;


// Structure that the PDD must fill out in UfnPdd_Init.
typedef struct _UFN_PDD_INTERFACE_INFO {
    DWORD           dwVersion;

    DWORD           dwCapabilities;
    DWORD           dwEndpointCount;
    PVOID           pvPddContext;

    PFN_UFN_PDD_DEINIT pfnDeinit;
    PFN_UFN_PDD_IS_CONFIGURATION_SUPPORTABLE pfnIsConfigurationSupportable;
    PFN_UFN_PDD_IS_ENDPOINT_SUPPORTABLE pfnIsEndpointSupportable;
    PFN_UFN_PDD_INIT_ENDPOINT pfnInitEndpoint;
    PFN_UFN_PDD_REGISTER_DEVICE pfnRegisterDevice;
    PFN_UFN_PDD_DEREGISTER_DEVICE pfnDeregisterDevice;
    PFN_UFN_PDD_START pfnStart;
    PFN_UFN_PDD_STOP pfnStop;
    PFN_UFN_PDD_ISSUE_TRANSFER pfnIssueTransfer;
    PFN_UFN_PDD_ABORT_TRANSFER pfnAbortTransfer;
    PFN_UFN_DEINIT_ENDPOINT pfnDeinitEndpoint;
    PFN_UFN_STALL_ENDPOINT pfnStallEndpoint;
    PFN_UFN_CLEAR_ENDPOINT_STALL pfnClearEndpointStall;
    PFN_UFN_SEND_CONTROL_STATUS_HANDSHAKE pfnSendControlStatusHandshake;
    PFN_UFN_SET_ADDRESS pfnSetAddress;
    PFN_UFN_IS_ENDPOINT_HALTED pfnIsEndpointHalted;
    PFN_UFN_INITIATE_REMOTE_WAKEUP pfnInitiateRemoteWakeup;
    PFN_UFN_POWER_DOWN pfnPowerDown;
    PFN_UFN_POWER_UP pfnPowerUp;
    PFN_UFN_IOCONTROL pfnIOControl;
} UFN_PDD_INTERFACE_INFO, *PUFN_PDD_INTERFACE_INFO;

#define UFN_PDD_CAPS_SUPPORTS_FULL_SPEED    BS_FULL_SPEED
#define UFN_PDD_CAPS_SUPPORTS_HIGH_SPEED    BS_HIGH_SPEED

// Version to use in the UFN_PDD_INTERFACE_INFO structure.
#define UFN_PDD_INTERFACE_VERSION_MAJOR   CE_MAJOR_VER
#define UFN_PDD_INTERFACE_VERSION_MINOR   CE_MINOR_VER
#define UFN_PDD_INTERFACE_VERSION \
    MAKELONG(UFN_PDD_INTERFACE_VERSION_MINOR, \
        UFN_PDD_INTERFACE_VERSION_MAJOR)


// Called by MDD's UFN_Init routine.
// PDD calls back to the MDD with the pvMddContext value.
// The PDD is responsible for filling in pRegistrationInfo.
DWORD
WINAPI
UfnPdd_Init(
    LPCTSTR                     pszActiveKey,
    PVOID                       pvMddContext,
    PUFN_MDD_INTERFACE_INFO     pMddInterfaceInfo,
    PUFN_PDD_INTERFACE_INFO     pPddInterfaceInfo
    );


// Called by MDD's DllEntry.
// The MDD has already registered the debug zones and 
// DisableThreadLibraryCalls by the time this is called.
// The PDD should initialize any global variables, etc in this routine.
BOOL
WINAPI
UfnPdd_DllEntry(
    HANDLE hDllHandle,
    DWORD  dwReason, 
    LPVOID lpReserved
    );


// The PDD should use this dwMsg value to signal that a transfer is complete.
// The STransfer* should be placed in dwParam.
// As soon as the PDD makes this call, it no longer has ownership of the 
// transfer and should no longer access it.
#define UFN_MSG_TRANSFER_COMPLETE     100


#ifdef DEBUG

#define FUNCTION_ENTER_MSG() DEBUGMSG(ZONE_FUNCTION, (_T("%s ++\r\n"), pszFname))
#define FUNCTION_LEAVE_MSG() DEBUGMSG(ZONE_FUNCTION, (_T("%s --\r\n"), pszFname))

extern DBGPARAM dpCurSettings;

// Debug zone defs
#define ZONE_ERROR              DEBUGZONE(0)
#define ZONE_WARNING            DEBUGZONE(1)
#define ZONE_INIT               DEBUGZONE(2)
#define ZONE_TRANSFER           DEBUGZONE(3)
#define ZONE_PIPE               DEBUGZONE(4)
#define ZONE_SEND               DEBUGZONE(5)
#define ZONE_RECEIVE            DEBUGZONE(6)
#define ZONE_USB_EVENTS         DEBUGZONE(7)

// Zones 8-11 are reserved for the PDD

#define ZONE_FUNCTION           DEBUGZONE(12)
#define ZONE_COMMENT            DEBUGZONE(13)

// "Hidden" debug zone which sends all debug output to celog instead of
// the debugger. This is sometimes useful when the PDD is very 
// timing-dependent.
#define ZONE_CELOG              DEBUGZONE(16)


#define DBG_ERROR               (1 << 0)
#define DBG_WARNING             (1 << 1)
#define DBG_INIT                (1 << 2)
#define DBG_TRANSFER            (1 << 3)
#define DBG_PIPE                (1 << 4)
#define DBG_SEND                (1 << 5)
#define DBG_RECEIVE             (1 << 6)
#define DBG_USB_EVENTS          (1 << 7)

// Zones 8-11 are reserved for the PDD

#define DBG_FUNCTION            (1 << 12)
#define DBG_COMMENT             (1 << 13)
#define DBG_CELOG               (1 << 16)


// The PDD should use this macro to set up the debug zones.
#define UFN_GENERATE_DPCURSETTINGS(szName, szZone8, szZone9, szZone10, szZone11, ulMask) \
    extern "C" DBGPARAM dpCurSettings = { \
        szName, \
        { \
            _T("Error"), _T("Warning"), _T("Init"), _T("Transfer"), \
            _T("Pipe"), _T("Send"), _T("Receive"), _T("USB Events"), \
            szZone8, szZone9, szZone10, szZone11, \
            _T("Function"), _T("Comments"), _T(""), _T("") \
        }, \
        ulMask \
    };

#define UFN_DEFAULT_DPCURSETTINGS_NAME  _T("USBFN")

#include <celog.h>

// If ZONE_CELOG is set, the debug messages are sent to celog instead of the
// standard debug output.
// This is useful if the debug messages cause timing problems.
#undef DEBUGMSG
#define DEBUGMSG(cond, printf_exp) \
    ( (void)(((cond) ? \
        ( (ZONE_CELOG) ? (CeLogMsg printf_exp), 1 : (NKDbgPrintfW printf_exp), 1) : 0) ) )

#else

#define UFN_GENERATE_DPCURSETTINGS(szName, szZone8, szZone9, szZone10, szZone11, ulMask)

#define FUNCTION_ENTER_MSG()
#define FUNCTION_LEAVE_MSG()

#endif // DEBUG


#ifdef __cplusplus
}
#endif

#endif //_USBFN_H_

