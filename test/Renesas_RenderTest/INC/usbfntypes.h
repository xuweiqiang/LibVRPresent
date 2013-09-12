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

        USBFNTYPES.H

Abstract:

        USB Function Types/Definitions; Used By Every USB Function PDD and client.
        
--*/

#ifndef _USBFNTYPES_H_
#define _USBFNTYPES_H_

#include <usb200.h>
#include <ceddk.h>

#ifdef __cplusplus
extern "C" {
#endif


// The prefix convention in this header is as follows:
// 
// USB_xxx is for structures/values defined in the USB specification
// UFN_xxx is for the function-specific interfac


typedef USB_ENDPOINT_DESCRIPTOR const * PCUSB_ENDPOINT_DESCRIPTOR;
typedef USB_INTERFACE_DESCRIPTOR const * PCUSB_INTERFACE_DESCRIPTOR;
typedef USB_CONFIGURATION_DESCRIPTOR const * PCUSB_CONFIGURATION_DESCRIPTOR;
typedef USB_DEVICE_DESCRIPTOR const * PCUSB_DEVICE_DESCRIPTOR;
typedef USB_STRING_DESCRIPTOR const * PCUSB_STRING_DESCRIPTOR;


typedef struct _UFN_ENDPOINT {
    DWORD                           dwCount;

    USB_ENDPOINT_DESCRIPTOR         Descriptor;
    PVOID                           pvExtended;
    DWORD                           cbExtended;
} UFN_ENDPOINT, *PUFN_ENDPOINT;
typedef UFN_ENDPOINT const * PCUFN_ENDPOINT;


typedef struct _UFN_INTERFACE {
    DWORD                           dwCount;

    USB_INTERFACE_DESCRIPTOR        Descriptor;
    PVOID                           pvExtended;
    DWORD                           cbExtended;
    PUFN_ENDPOINT                   pEndpoints;
} UFN_INTERFACE, *PUFN_INTERFACE;
typedef UFN_INTERFACE const * PCUFN_INTERFACE;


typedef struct _UFN_CONFIGURATION {
    DWORD                           dwCount;

    USB_CONFIGURATION_DESCRIPTOR    Descriptor;
    PVOID                           pvExtended;
    DWORD                           cbExtended;
    PUFN_INTERFACE                  pInterfaces;
} UFN_CONFIGURATION, *PUFN_CONFIGURATION;
typedef UFN_CONFIGURATION const * PCUFN_CONFIGURATION;


typedef struct _UFN_STRING_SET {
  WORD      wLangId;
  LPCWSTR  *ppszStrings;
  DWORD     cStrings;
} UFN_STRING_SET, *PUFN_STRING_SET;
typedef UFN_STRING_SET const * PCUFN_STRING_SET;

#define MAX_STRING_DESC_WCHARS ( (255 - 2) / sizeof(WCHAR) )


#include <pshpack1.h>
typedef struct _USB_DEVICE_REQUEST {
    UCHAR   bmRequestType;
    UCHAR   bRequest;
    USHORT  wValue;
    USHORT  wIndex;
    USHORT  wLength;
} USB_DEVICE_REQUEST, *PUSB_DEVICE_REQUEST;
typedef USB_DEVICE_REQUEST const * PCUSB_DEVICE_REQUEST;
#include <poppack.h>


// USB_DEVICE_REQUEST.bmRequestType bits for control Pipes
#define     USB_REQUEST_DEVICE_TO_HOST      0x80
#define     USB_REQUEST_HOST_TO_DEVICE      0x00
#define     USB_REQUEST_STANDARD            0x00
#define     USB_REQUEST_CLASS               0x20
#define     USB_REQUEST_VENDOR              0x40
#define     USB_REQUEST_RESERVED            0x60
#define     USB_REQUEST_FOR_DEVICE          0x00
#define     USB_REQUEST_FOR_INTERFACE       0x01
#define     USB_REQUEST_FOR_ENDPOINT        0x02
#define     USB_REQUEST_FOR_OTHER           0x03

// UFN Transfer Errors
#define     UFN_NO_ERROR                        0x00000000
#define     UFN_DEVICE_NOT_RESPONDING_ERROR     0x00000005
#define     UFN_CANCELED_ERROR                  0x00000101
#define     UFN_NOT_COMPLETE_ERROR              0x00000103
#define     UFN_CLIENT_BUFFER_ERROR             0x00000104


// UFN Events 
typedef enum _UFN_BUS_EVENT {
    UFN_DETACH = 0,
    UFN_ATTACH,
    UFN_RESET,
    UFN_SUSPEND,
    UFN_RESUME,
} UFN_BUS_EVENT, *PUFN_BUS_EVENT;


typedef enum _UFN_BUS_SPEED {
    BS_UNKNOWN_SPEED = 0,
    BS_FULL_SPEED = (1 << 0),
    BS_HIGH_SPEED = (1 << 1),
} UFN_BUS_SPEED, *PUFN_BUS_SPEED;

#define IS_VALID_SPEED(Speed) ( ((Speed) == BS_FULL_SPEED) || \
    ((Speed) == BS_HIGH_SPEED) )


#define USB_ENDPOINT_ADDRESS_IN     0x80
#define USB_ENDPOINT_ADDRESS_OUT    0x00


#define USB_FULL_HIGH_SPEED_CONTROL_MAX_PACKET_SIZE 0x040 // 64 bytes

#define USB_FULL_SPEED_CONTROL_MAX_PACKET_SIZE      0x040 // 64 bytes
#define USB_FULL_SPEED_BULK_MAX_PACKET_SIZE         0x040 // 64 bytes
#define USB_FULL_SPEED_INTERRUPT_MAX_PACKET_SIZE    0x040 // 64 bytes
#define USB_FULL_SPEED_ISOCHRONOUS_MAX_PACKET_SIZE  0x3FF // 1023 bytes

#define USB_HIGH_SPEED_CONTROL_MAX_PACKET_SIZE      0x040 // 64 bytes
#define USB_HIGH_SPEED_BULK_MAX_PACKET_SIZE         0x200 // 512 bytes
#define USB_HIGH_SPEED_INTERRUPT_MAX_PACKET_SIZE    0x400 // 1024 bytes
#define USB_HIGH_SPEED_ISOCHRONOUS_MAX_PACKET_SIZE  0x400 // 1024 bytes


// Callback parameter for the MDD. dwMsg parameters are listed below.
typedef BOOL (WINAPI *LPUFN_NOTIFY) (
    PVOID   pvNotifyParameter,
    DWORD   dwMsg,
    DWORD   dwParam
    );

// Messages sent to the notify routine.

// dwParam = UFN_BUS_EVENT
#define UFN_MSG_BUS_EVENTS                  1

// dwParam = wValue from the device request
#define UFN_MSG_CONFIGURED                  2

// dwParam = UFN_BUS_SPEED
#define UFN_MSG_BUS_SPEED                   3

// dwParam = PUSB_DEVICE_REQUEST
// This message is for setup packets that are not processed by the MDD.
// The client is responsible for sending the control status handshake.
#define UFN_MSG_SETUP_PACKET                4

// dwParam = PUSB_DEVICE_REQUEST
// This message is for setup packets that are processed by the MDD.
// The client must not send the control status handshake.
#define UFN_MSG_PREPROCESSED_SETUP_PACKET   5

// dwParam = the USB bus address
#define UFN_MSG_SET_ADDRESS                 6


// Handle that identifies the client to the MDD.
typedef PVOID UFN_HANDLE;

// Handle that identifies a pipe to the MDD.
typedef PVOID UFN_PIPE, *PUFN_PIPE;


// Register the device's descriptors with the MDD.
// The wMaxPacket and bEndpointAddress parameters may be changed
// by the MDD/PDD as required by the function controller.
typedef DWORD (WINAPI *LPUFN_REGISTER_DEVICE) (
    UFN_HANDLE              hDevice,
    PUSB_DEVICE_DESCRIPTOR  pHighSpeedDeviceDesc,
    PUFN_CONFIGURATION      pHighSpeedConfig,
    PUSB_DEVICE_DESCRIPTOR  pFullSpeedDeviceDesc,
    PUFN_CONFIGURATION      pFullSpeedConfig,
    PCUFN_STRING_SET        pStringSets,
    DWORD                   cStringSets
    );

// Connect the device to the bus.
typedef DWORD (WINAPI *LPUFN_START) (
    UFN_HANDLE      hDevice,
    LPUFN_NOTIFY    lpDeviceNotify,
    PVOID           pvDeviceNotifyParameter,
    PUFN_PIPE       phDefaultPipe
    );

// Remove the device from the bus.
typedef DWORD (WINAPI *LPUFN_STOP) (
    UFN_HANDLE hDevice
    );

// Deregister the descriptor set.
typedef DWORD (WINAPI *LPUFN_DEREGISTER_DEVICE) (
    UFN_HANDLE hDevice
    );

// Open a pipe.
typedef DWORD (WINAPI *LPUFN_OPEN_PIPE) (
    UFN_HANDLE   hDevice,
    DWORD        dwEndpointAddress,
    PUFN_PIPE    phPipe
    );

// Close a pipe.
typedef DWORD (WINAPI *LPUFN_CLOSE_PIPE) (
    UFN_HANDLE hDevice,
    UFN_PIPE   hPipe
    );


// Transfer completion callback typedef.
typedef DWORD (WINAPI *LPTRANSFER_NOTIFY_ROUTINE)(PVOID pvNotifyParameter);

// Handle that identifies a transfer to the MDD.
typedef PVOID UFN_TRANSFER, *PUFN_TRANSFER;

// Issue a transfer.
typedef DWORD (WINAPI *LPUFN_ISSUE_TRANSFER) (
    UFN_HANDLE                  hDevice,
    UFN_PIPE                    hPipe,
    LPTRANSFER_NOTIFY_ROUTINE   lpNotifyRoutine,
    PVOID                       pvNotifyContext,
    DWORD                       dwFlags,
    DWORD                       cbBuffer, 
    PVOID                       pvBuffer,
    DWORD                       dwBufferPhysicalAddress,
    PVOID                       pvPddTransferInfo,
    PUFN_TRANSFER               phTransfer
    );

// Transfer flags.
#define USB_IN_TRANSFER  0x80
#define USB_OUT_TRANSFER 0x00

// Get the status of a transfer.
typedef DWORD (WINAPI *LPUFN_GET_TRANSFER_STATUS) (
    UFN_HANDLE   hDevice,
    UFN_TRANSFER hTransfer,
    PDWORD       pcbTransferred,
    PDWORD       pdwUsbError
    );

// Abort a transfer.
typedef DWORD (WINAPI *LPUFN_ABORT_TRANSFER) (
    UFN_HANDLE   hDevice,
    UFN_TRANSFER hTransfer
    );

// Close a transfer.
typedef DWORD (WINAPI *LPUFN_CLOSE_TRANSFER) (
    UFN_HANDLE   hDevice,
    UFN_TRANSFER hTransfer
    );

// Stall a pipe.
typedef DWORD (WINAPI *LPUFN_STALL_PIPE) (
    UFN_HANDLE hDevice,
    UFN_PIPE   hPipe
    );

// Clear the stalled state from a pipe.
typedef DWORD (WINAPI *LPUFN_CLEAR_PIPE_STALL) (
    UFN_HANDLE hDevice,
    UFN_PIPE   hPipe
    );

// Initiate a remote wakeup of the host.
typedef DWORD (WINAPI *LPUFN_INITIATE_REMOTE_WAKEUP) (
    UFN_HANDLE hDevice
    );

// Signal completion of the data phase of a control transfer.
typedef DWORD (WINAPI *LPUFN_SEND_CONTROL_STATUS_HANDSHAKE) (
    UFN_HANDLE hDevice
    );


// MDD function table.
typedef struct _UFN_FUNCTIONS {
    LPUFN_REGISTER_DEVICE                   lpRegisterDevice;
    LPUFN_START                             lpStart;
    LPUFN_STOP                              lpStop;
    LPUFN_DEREGISTER_DEVICE                 lpDeregisterDevice;
    LPUFN_OPEN_PIPE                         lpOpenPipe;
    LPUFN_CLOSE_PIPE                        lpClosePipe;
    LPUFN_ISSUE_TRANSFER                    lpIssueTransfer;
    LPUFN_GET_TRANSFER_STATUS               lpGetTransferStatus;
    LPUFN_ABORT_TRANSFER                    lpAbortTransfer;
    LPUFN_CLOSE_TRANSFER                    lpCloseTransfer;
    LPUFN_STALL_PIPE                        lpStallPipe;
    LPUFN_CLEAR_PIPE_STALL                  lpClearPipeStall;
    LPUFN_INITIATE_REMOTE_WAKEUP            lpInitiateRemoteWakeup;
    LPUFN_SEND_CONTROL_STATUS_HANDSHAKE     lpSendControlStatusHandshake;
} UFN_FUNCTIONS, *PUFN_FUNCTIONS;
typedef UFN_FUNCTIONS const * PCUFN_FUNCTIONS;


// Called to initialize the MDD-Client interface. Retrieves the device
// handle and function table.
DWORD
UfnInitializeInterface(
    LPCTSTR         pszActiveKey,
    UFN_HANDLE     *phDevice,
    PUFN_FUNCTIONS  pFunctions,
    PVOID          *ppvInterfaceInfo
    );

// Called to deinitialize the MDD-Client interface. This should only be
// called when you are certain that there are no more client threads in
// any callback function.
DWORD
UfnDeinitializeInterface(
    PVOID       pvInterfaceInfo
    );


// String descriptors are not NULL-terminated, but we will
// NULL-terminate them in this structure.
typedef struct _UFN_CLIENT_REG_INFO {
    DWORD   dwSize; // Set to the size of this structure
    
    DWORD   idVendor;
    WCHAR   szVendor[MAX_STRING_DESC_WCHARS + 1]; // +1 for NULL

    DWORD   idProduct;
    WCHAR   szProduct[MAX_STRING_DESC_WCHARS + 1]; // +1 for NULL

    DWORD   bcdDevice;
} UFN_CLIENT_REG_INFO, *PUFN_CLIENT_REG_INFO;

#define UFN_IDVENDOR_VALNAME        L"idVendor"
#define UFN_IDVENDOR_VALTYPE        REG_DWORD
#define UFN_MANUFACTURER_VALNAME    L"Manufacturer"
#define UFN_MANUFACTURER_VALTYPE    REG_SZ
#define UFN_IDPRODUCT_VALNAME       L"idProduct"
#define UFN_IDPRODUCT_VALTYPE       REG_DWORD
#define UFN_PRODUCT_VALNAME         L"Product"
#define UFN_PRODUCT_VALTYPE         REG_SZ
#define UFN_BCDDEVICE_VALNAME       L"bcdDevice"
#define UFN_BCDDEVICE_VALTYPE       REG_DWORD

// Called to read device information from the registry.
DWORD
UfnGetRegistryInfo(
    LPCTSTR                 pszActiveKey,
    PUFN_CLIENT_REG_INFO    pRegInfo
    );


#ifdef __cplusplus
}
#endif

#endif // _USBFNTYPES_H_

