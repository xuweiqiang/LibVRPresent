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
//			WARNING			WARNING				WARNING
// This header file contains function prototypes that are invoked from device.exe.
// Do not modify these routines or their prototypes unless you are also able to
// modify device.exe, the Windows CE device manager.
//			WARNING			WARNING				WARNING
//

#pragma once

#include <pnp.h>
#include <pm.h>

EXTERN_C DWORD WINAPI PmDevicePowerNotify(PVOID pvDevice, 
    CEDEVICE_POWER_STATE DeviceState, DWORD Flags);
EXTERN_C DWORD WINAPI PmGetSystemPowerState(LPWSTR pBuffer, DWORD Length, 
    PDWORD pFlags);
EXTERN_C VOID WINAPI PmNotify(DWORD Flags, HPROCESS dwPid, HTHREAD dwTid);
EXTERN_C DWORD WINAPI PmReleasePowerRequirement(HANDLE h, DWORD Flags);
EXTERN_C HANDLE WINAPI PmSetPowerRequirement(PVOID pvDevice, 
    CEDEVICE_POWER_STATE DeviceState, ULONG DeviceFlags, 
    PVOID pvSystemState, ULONG StateFlags);
EXTERN_C DWORD WINAPI PmSetSystemPowerState(LPCWSTR pwsState, DWORD StateFlags,
    DWORD Options);
EXTERN_C HANDLE WINAPI PmRequestPowerNotifications(HANDLE hMsgQ, DWORD Flags); 
EXTERN_C DWORD WINAPI PmStopPowerNotifications(HANDLE h);
EXTERN_C HANDLE WINAPI PmRegisterPowerRelationship(PVOID pvParent, 
    PVOID pvChild, PPOWER_CAPABILITIES pCaps, DWORD Flags);
EXTERN_C DWORD WINAPI PmReleasePowerRelationship(HANDLE h); 
EXTERN_C DWORD WINAPI PmSetDevicePower(PVOID pvDevice, DWORD dwDeviceFlags, 
    CEDEVICE_POWER_STATE dwState);
EXTERN_C DWORD WINAPI PmGetDevicePower(PVOID pvDevice, DWORD dwDeviceFlags, 
    PCEDEVICE_POWER_STATE pdwState); 
EXTERN_C BOOL WINAPI PmInit(VOID);
EXTERN_C VOID WINAPI PmPowerHandler(BOOL bOff);
