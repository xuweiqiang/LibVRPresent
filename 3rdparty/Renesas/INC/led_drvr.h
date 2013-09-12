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

led_drvr.h

--*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
#define SETFNAME(name)          LPCTSTR pszFname = name
#else   // DEBUG
#define SETFNAME(name)
#endif  // DEBUG

// debug zones
#define ZONE_ERROR          DEBUGZONE(0)
#define ZONE_WARN           DEBUGZONE(1)
#define ZONE_INIT           DEBUGZONE(2)
#define ZONE_IOCTL          DEBUGZONE(3)
#define ZONE_HARDWARE       DEBUGZONE(4)
#define ZONE_PDD            DEBUGZONE(5)
#define ZONE_FUNCTION       DEBUGZONE(6)

#define LOCKNLED()          EnterCriticalSection(&gcsNLed)
#define UNLOCKNLED()        LeaveCriticalSection(&gcsNLed)

// This type of function implments the optional PDD ioctl interface.  It
// returns a Win32 error code, NOT a BOOL.
typedef DWORD (*PFN_NLED_PDD_IOCONTROL)(
              DWORD  dwContext,
              DWORD  Ioctl,
              PUCHAR pInBuf,
              DWORD  InBufLen, 
              PUCHAR pOutBuf,
              DWORD  OutBufLen,
              PDWORD pdwBytesTransferred);


EXTERN_C
VOID
WINAPI
NLedDriverPowerDown(
	BOOL power_down
	);
typedef VOID (WINAPI *PFN_NLED_DRIVER_POWER_DOWN)(BOOL);




EXTERN_C
BOOL
WINAPI
NLedDriverGetDeviceInfo(
	INT		nId,
	PVOID	pOutput
	);
typedef BOOL (WINAPI *PFN_NLED_DRIVER_GET_DEVICE_INFO)(INT, PVOID);



EXTERN_C
BOOL
WINAPI
NLedDriverSetDevice(
	INT		nId,
	PVOID	pInput
	);
typedef BOOL (WINAPI *PFN_NLED_DRIVER_SET_DEVICE)(INT, PVOID);



EXTERN_C
BOOL
WINAPI
NLedDriverInitialize(
	void
	);
typedef BOOL (WINAPI *PFN_NLED_DRIVER_INITIALIZE)(void);


EXTERN_C
BOOL
WINAPI
NLedDriverDeInitialize(
	void
	);
typedef BOOL (WINAPI *PFN_NLED_DRIVER_DEINITIALIZE)(void);

#ifndef NLED_DATA_DEF
extern CRITICAL_SECTION gcsNLed;
extern PFN_NLED_PDD_IOCONTROL gpfnNLedPddIOControl;
#endif  // NLED_DATA_DEF

#ifdef __cplusplus
}
#endif


