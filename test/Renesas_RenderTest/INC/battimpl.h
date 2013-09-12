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

#include <windows.h>
#include <battery.h>

#pragma once

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#ifdef DEBUG
#define SETFNAME(name)          LPCTSTR pszFname = name
#else   // DEBUG
#define SETFNAME(name)
#endif  // DEBUG

// debug zones
#define ZONE_ERROR          DEBUGZONE(0)
#define ZONE_WARN           DEBUGZONE(1)
#define ZONE_INIT           DEBUGZONE(2)
#define ZONE_BATTERY        DEBUGZONE(3)
#define ZONE_IOCTL          DEBUGZONE(4)
#define ZONE_HARDWARE       DEBUGZONE(5)
#define ZONE_RESUME         DEBUGZONE(6)
#define ZONE_FUNCTION       DEBUGZONE(7)
#define ZONE_PDD            DEBUGZONE(8)

#define LOCKBATTERY()       EnterCriticalSection(&gcsBattery)
#define UNLOCKBATTERY()     LeaveCriticalSection(&gcsBattery)

// This type of function implments the optional PDD ioctl interface.  It
// returns a Win32 error code, NOT a BOOL.
typedef DWORD (*PFN_BATTERY_PDD_IOCONTROL)(
              DWORD  dwContext,
              DWORD  Ioctl,
              PUCHAR pInBuf,
              DWORD  InBufLen, 
              PUCHAR pOutBuf,
              DWORD  OutBufLen,
              PDWORD pdwBytesTransferred);

// api function prototypes -- these live in the MDD
void BatteryAPIStopwatch (BOOL fStart, BOOL fReset);
DWORD WINAPI BatteryAPIGetSystemPowerStatusEx2(PSYSTEM_POWER_STATUS_EX2 pstatus,
                       DWORD dwLen, BOOL fUpdate);
BOOL WINAPI BatteryAPIGetSystemPowerStatusEx(PSYSTEM_POWER_STATUS_EX pstatus,
                       BOOL fUpdate);
void WINAPI BatteryAPIGetLifeTimeInfo (LPSYSTEMTIME pstLastChange, DWORD * pcmsCpuUsage,
                       DWORD * pcmsPreviousCpuUsage);
void WINAPI BatteryAPINotifyOfTimeChange (BOOL fForward, FILETIME * pftDelta);

// PDD function prototypes
BOOL WINAPI BatteryPDDInitialize(LPCTSTR pszRegistryContext);
void WINAPI BatteryPDDDeinitialize(void);
void WINAPI BatteryPDDResume(void);
void WINAPI BatteryPDDPowerHandler(BOOL bOff);
BOOL WINAPI BatteryPDDGetStatus(PSYSTEM_POWER_STATUS_EX2 pstatus,
                       PBOOL pfBatteriesChangedSinceLastCall);
LONG BatteryPDDGetLevels(void);
BOOL BatteryPDDSupportsChangeNotification(void);

#ifndef BATTERY_DATA_DEF
extern CRITICAL_SECTION gcsBattery;
extern PFN_BATTERY_PDD_IOCONTROL gpfnBatteryPddIOControl;
#endif  // BATTERY_DATA_DEF

#ifdef __cplusplus
}
#endif  // __cplusplus

