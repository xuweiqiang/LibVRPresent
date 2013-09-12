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

battery.h

Abstract:  

Interface to the Battery Driver library

Notes: 


--*/
/* 
    @doc DRIVERS

    @module Battery Device Driver | Interface to the Battery Driver library
   
    @topic Battery Driver | The Battery Driver is an object file (battery.obj) 
    that contains the functions and variables declared in Battery.h, 
    which constitute the interface by which GWE gets information about the 
    main and backup batteries and AC power.
*/   
   
#ifndef _BATTERY_H_
#define _BATTERY_H_

#ifdef __cplusplus
extern "C" {
#endif


/* 
    @func BOOL | BatteryDrvrGetStatus | Return the status of the main and backup
	batteries
    
    @parm PSYSTEM_POWER_STATUS_EX2 | pSystemPowerStatus | Buffer to be filled 
    in with the battery status information.  See the Win32 Documentation for a
	description of the SYSTEM_POWER_STATUS structure.  

*/

BOOL WINAPI BatteryDrvrGetStatus (PSYSTEM_POWER_STATUS_EX2 pPowerStatus,
                                  PBOOL pfBatteriesChangedSinceLastCall);


/* 
    @func LONG | BatteryDrvrGetLevels | Return the number of levels the battery
    driver is capable of returning in the BatteryFlag and BackupBatteryFlag
	fields of the BATTERY_STATUS structure.
    
	@rdesc The LOWORD of the return value is the number of main battery levels
	which BatteryDrvrGetStatus is capable of returning in the BatteryFlag
	field of the BATTERY_STATUS structure.  For example, if only 
	BATTERY_FLAG_HIGH and BATTERY_FLAG_LOW can be returned, the LOWORD of the 
	return value would be 2.  If BATTERY_FLAG_CRITICAL  can also be returned,
	the LOWORD of the return value would be 3.  The values BATTERY_FLAG_CHARGING, 
	BATTERY_FLAG_NO_BATTERY, or BATTERY_FLAG_UNKNOWN are to be ignored for the 
	purposes of this count.

	The HIWORD of the return value is the number of backup battery levels which
 	BatteryDrvrGetStatus is capable of returning.  The same rules apply as to
	the main battery levels.
*/    
LONG WINAPI BatteryDrvrGetLevels (void);

// this structure packages up the GetBatteryLifetimeInfo() parameters into a form that can
// be passed easily via IOCTL.
typedef struct _BatteryLifetimeInfo_tag {
    SYSTEMTIME stLastChange;
    DWORD cmsCpuUsage;
    DWORD cmsPreviousCpuUsage;
} BATTERYLIFETIMEINFO, *PBATTERYLIFETIMEINFO;


/* 
    @func BOOL | BatteryDrvrSupportsChangeNotification | Returns whether the 
	hardware can detect when the batteries have been changed.  If FALSE,
	BATTERY_STATUS.fBatteriesChangedSinceLastCall will always be FALSE.
*/
BOOL WINAPI BatteryDrvrSupportsChangeNotification (void);

// Battery IOCTL codes
#define IOCTL_BATTERY_GETSYSTEMPOWERSTATUSEX             \
    CTL_CODE(FILE_DEVICE_BATTERY, 0x100, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BATTERY_GETSYSTEMPOWERSTATUSEX2             \
    CTL_CODE(FILE_DEVICE_BATTERY, 0x101, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BATTERY_GETLIFETIMEINFO             \
    CTL_CODE(FILE_DEVICE_BATTERY, 0x102, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BATTERY_GETLEVELS             \
    CTL_CODE(FILE_DEVICE_BATTERY, 0x103, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BATTERY_SUPPORTSCHANGENOTIFICATION             \
    CTL_CODE(FILE_DEVICE_BATTERY, 0x104, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_BATTERY_NOTIFYOFTIMECHANGE             \
    CTL_CODE(FILE_DEVICE_BATTERY, 0x105, METHOD_BUFFERED, FILE_ANY_ACCESS)

// Battery driver interface GUID (used with AdvertiseInterface())
#define BATTERY_DRIVER_CLASS        _T("{DD176277-CD34-4980-91EE-67DBEF3D8913}")

// API event name for the battery interface.  If calling OpenEvent() on this name returns a valid
// handle, the battery APIs exist on the platform.  The handle will be signaled when the APIs become
// ready.
#define BATTERY_API_EVENT_NAME  _T("SYSTEM/BatteryAPIsReady")

#ifdef __cplusplus
}
#endif

#endif
