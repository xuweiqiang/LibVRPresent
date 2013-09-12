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
// This file contains internal definitions for the Power Manager implementation.
//

#pragma once

#include <windows.h>
#include <pmpriv.h>
#include <celog.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define ZONE_ERROR              DEBUGZONE(0)
#define ZONE_WARN               DEBUGZONE(1)
#define ZONE_INIT               DEBUGZONE(2)
#define ZONE_RESUME             DEBUGZONE(3)
#define ZONE_DEVICE             DEBUGZONE(4)
#define ZONE_IOCTL              DEBUGZONE(5)
#define ZONE_REFCNT             DEBUGZONE(6)
#define ZONE_REQUIRE            DEBUGZONE(7)
#define ZONE_NOTIFY             DEBUGZONE(8)
#define ZONE_REGISTRY           DEBUGZONE(9)
#define ZONE_PLATFORM           DEBUGZONE(10)
#define ZONE_API                DEBUGZONE(11)
#define ZONE_ALLOC              DEBUGZONE(12)
#define ZONE_TIMERS             DEBUGZONE(13)

#if defined(SHIP_BUILD) && !defined(_PM_NOCELOG)
#define _PM_NOCELOG     // disable CELOG if SHIP_BUILD
#endif  // defined(SHIP_BUILD) && !defined(_PM_NOCELOG)

#if defined(DEBUG) || !defined(_PM_NOCELOGMSG)
#define SETFNAME(name)          LPCTSTR pszFname = (name)
#else   // defined(DEBUG) || !defined(_PM_NOCELOGMSG)
#define SETFNAME(name)          ((void) 0)
#endif  // defined(DEBUG) || !defined(_PM_NOCELOGMSG)

#if defined(_PM_NOCELOG)
#ifdef DEBUG
#define PMLOGMSG(cond, printf_exp)     DEBUGMSG(cond, printf_exp)
#else   // DEBUG
#define PMLOGMSG(cond, printf_exp)     ((void) 0)
#endif  // DEBUG
#else   // defined(_PM_NOCELOG)
#ifdef DEBUG
#define PMLOGMSG(cond, printf_exp)     ((void) ((cond) ? (NKDbgPrintfW printf_exp), (CeLogMsg printf_exp), 1 : 0))
#else   // DEBUG
#define PMLOGMSG(cond, printf_exp)      RETAILCELOGMSG(cond, printf_exp)
#endif  // DEBUG
#endif  // defined(_PM_NOCELOG)

// convenience macros
#define UnusedParameter(x)      x = x
#define dim(x)                  (sizeof(x) / sizeof(x[0]))

#define PMLOCK()                PmLock()
#define PMUNLOCK()              PmUnlock()
#define PMENTERUPDATE()         PmEnterUpdate()
#define PMLEAVEUPDATE()         PmLeaveUpdate()

// device notification queue parameters
#define PNP_QUEUE_ENTRIES       3
#define PNP_MAX_NAMELEN         128
#define PNP_QUEUE_SIZE          (PNP_QUEUE_ENTRIES * (sizeof(DEVDETAIL) + (PNP_MAX_NAMELEN * sizeof(TCHAR))))

// activity timer parameters
#define MAXTIMERINTERVAL        (0xFFFFFFFF / 1000)     // in seconds

// Default priorities for the various PM threads.  These can be overridden
// using the registry.  Note that the priority for the resume thread must
// be higher that of any thread calling SetSystemPowerState() or accessing
// any resoure that may be locked during SetSystemPowerState().  Since this
// might include the filesystem lock, the resume thread needs to be higher
// priority than any driver thread that touches the file system or accesses
// the registry while handling power IOCTLs.
#define DEF_PNP_THREAD_PRIORITY     249                 // THREAD_PRIORITY_ABOVE_NORMAL
#define DEF_RESUME_THREAD_PRIORITY  99                  // must be high priority (see above comment)
#define DEF_SUSPEND_THREAD_PRIORITY 0                   // resume thread priority + 1, or 0 to disable adjustment
#define DEF_SYSTEM_THREAD_PRIORITY  249                 // THREAD_PRIORITY_ABOVE_NORMAL
#define DEF_ACTIVITY_TIMER_THREAD_PRIORITY  249         // THREAD_PRIORITY_ABOVE_NORMAL
#define	DEF_PRESUSPEND_THREAD_PRIORITY	249				// THREAD_PRIORITY_ABOVE_NORMAL

typedef const GUID *LPCGUID;            // LPGUID defined in objbase.h, but no LPCGUID

// This structure is used to identify a device in a specific 
// class of devices.
typedef struct _DeviceId_tag {
    const GUID *pGuid;
    LPCTSTR pszName;
} DEVICEID, *PDEVICEID;

typedef struct _DeviceInterface_tag {
    BOOL (WINAPI * pfnInitInterface) (VOID);
    HANDLE (WINAPI *pfnOpenDevice) (struct _DeviceState_tag *);
    BOOL (WINAPI * pfnCloseDevice) (HANDLE);
    BOOL (WINAPI * pfnRequestDevice) (HANDLE, DWORD, LPVOID, DWORD, LPVOID, DWORD, LPDWORD);
} DEVICE_INTERFACE, *PDEVICE_INTERFACE;

// this structure describes a power-manageable device
typedef struct _DeviceState_tag {
    LPCTSTR     pszName;                // device's name
    CEDEVICE_POWER_STATE    curDx;      // current official power state (not necessarily supported by the device)
    CEDEVICE_POWER_STATE    floorDx;    // minimum device power state, or PwrDeviceUnspecified
    CEDEVICE_POWER_STATE    ceilingDx;  // maximum device power state, or PwrDeviceUnspecified
    CEDEVICE_POWER_STATE    setDx;      // power state if explicitly set, or PwrDeviceUnspecified
    CEDEVICE_POWER_STATE    lastReqDx;  // last state requested by the device
    CEDEVICE_POWER_STATE    actualDx;   // current actual device power state
    CEDEVICE_POWER_STATE    pendingDx;  // Pending DX for updating 
    DWORD                   dwNumPending; // Number of Pending for updating.
    struct _DeviceState_tag *pParent;   // parent device, or NULL
    POWER_CAPABILITIES      caps;       // as reported by the device
    DWORD       dwRefCount;             // structure can be deallocated when this is 0
    HANDLE      hDevice;                // handle to the device from OpenDevice(), or NULL
    PDEVICE_INTERFACE       pInterface; // interface to the device class power management routines
    struct _DeviceList_tag  *pListHead; // pointer to the containing list
    struct _DeviceState_tag *pNext;     // linked list pointers
    struct _DeviceState_tag *pPrev;
} DEVICE_STATE, *PDEVICE_STATE;

// this structure describes a set of power manageable devices
typedef struct _DeviceList_tag {
    LPCGUID     pGuid;                  // class of device
    PDEVICE_STATE pList;                // pointer to devices
    HANDLE      hMsgQ;                  // device notification queue
    HANDLE      hnClass;                // handle from RequestDeviceNotifications()
    PDEVICE_INTERFACE      pInterface; // interface to the device class power management routines
    struct _DeviceList_tag *pNext;      // singly linked list pointer
} DEVICE_LIST, *PDEVICE_LIST;

// this structure describes a restriction on device power states
// for a particular device or class of devices.  The restriction 
// may be a floor or a ceiling, depending on the context in which it
// is used.
typedef struct _DevicePowerRestriction_tag {
    PDEVICEID   pDeviceId;              // specifies affected device(s)
    HANDLE      hOwner;                 // process that created the restriction, or 0
    CEDEVICE_POWER_STATE    devDx;      // device power state
    LPCTSTR     pszSystemState;         // system state for the restriction, or NULL
    DWORD       dwFlags;                // set POWER_FORCE to keep restriction during suspend
    struct _DevicePowerRestriction_tag *pNext;  // linked list pointers
    struct _DevicePowerRestriction_tag *pPrev;
} DEVICE_POWER_RESTRICTION, *PDEVICE_POWER_RESTRICTION, **PPDEVICE_POWER_RESTRICTION;

// this structure describes a system power state
typedef struct _SystemPowerState_tag {
    LPCTSTR pszName;                    // power state name
    CEDEVICE_POWER_STATE defaultCeilingDx;  // default max device power state
    DWORD   dwFlags;                    // power state flags
} SYSTEM_POWER_STATE, *PSYSTEM_POWER_STATE, **PPSYSTEM_POWER_STATE;

// this structure describes a power notification request
typedef struct _PowerNotification_tag {
    HANDLE hMsgQ;                       // message queue to talk to the requestor
    DWORD dwFlags;                      // flag bits (see pm.h)
    HANDLE hOwner;                      // process that owns this structure
    struct _PowerNotification_tag *pNext;   // linked list pointers
    struct _PowerNotification_tag *pPrev;
} POWER_NOTIFICATION, *PPOWER_NOTIFICATION, **PPPOWER_NOTIFICATION;

// this structure statically allocates enough memory for a power broadcast message
typedef struct _PowerBroadcastBuffer_tag {
    // these members must match POWER_BROADCAST!
    DWORD   Message;
    DWORD   Flags;
    DWORD   Length;

    // this is the data part of the broadcast message
    WCHAR   SystemPowerState[MAX_PATH + 1];
} POWER_BROADCAST_BUFFER, *PPOWER_BROADCAST_BUFFER;

// this structure keeps track of activity timers
typedef struct _ActivityTimer_tag {
    LPTSTR      pszName;            // timer name
    DWORD       dwTimeout;          // in milliseconds
    DWORD       dwTimeLeft;         // in milliseconds
    HANDLE      hevReset;           // public event handle, resets the timer
    HANDLE      hevActive;          // private handle, device still active
    HANDLE      hevInactive;        // private handle, device inactive
    LPDWORD     pdwWakeSources;     // wake sources that fire this timer
    DWORD       dwResetCount;       // number of times timer has been reset
    DWORD       dwExpiredCount;     // number of times timer has expired
} ACTIVITY_TIMER, *PACTIVITY_TIMER, **PPACTIVITY_TIMER;

// this structure is used for transmitting notification messages to the PM's policy handler
typedef struct PowerPolicyMessage_tag {
    DWORD       dwMessage;          // dwNotification from PowerPolicyNotify()
    DWORD       dwData;             // dwNotifyData from PowerPolicyNotify()
    HANDLE      hOwnerProcess;      // informational, filled in by caller
} POWERPOLICYMESSAGE, *PPOWERPOLICYMESSAGE;

#define PMPOLICY_NOTIFICATION_QUEUE         _T("PowerManager/NotificationQueue")

// function prototypes -- MDD functions
PVOID PmAlloc(DWORD dwSize);
BOOL PmFree(LPVOID lpvMemory);
VOID PmLock(VOID);
VOID PmUnlock(VOID);
VOID PmEnterUpdate(VOID);
VOID PmLeaveUpdate(VOID);
VOID DeviceStateAddRef(PDEVICE_STATE pds);
VOID DeviceStateDecRef(PDEVICE_STATE pds);
DWORD WINAPI PnpThreadProc(LPVOID lpvParam);
DWORD WINAPI ResumeThreadProc(LPVOID lpvParam);
DWORD WINAPI SystemThreadProc(LPVOID lpvParam);
DWORD WINAPI ActivityTimersThreadProc(LPVOID lpvParam);
VOID AddDevice(LPCGUID guidDevClass, LPCTSTR pszName, PDEVICE_STATE pParent, 
    PPOWER_CAPABILITIES pCaps);
VOID RemoveDevice(LPCGUID guidDevClass, LPCTSTR pszName);
DWORD RegQueryTypedValue(HKEY hk, LPCWSTR pszValue, PVOID pvData, 
    LPDWORD pdwSize, DWORD dwType);
BOOL GetPMThreadPriority(LPCTSTR pszValueName, LPINT piThreadPriority);
BOOL ConvertStringToGuid (LPCTSTR pszGuid, GUID *pGuid);
DWORD DeviceIdSize(PDEVICEID pId);
PDEVICEID DeviceIdCloneIntoBuffer(PDEVICEID pIdSrc, PDEVICEID pIdDst, DWORD cbDst);
PDEVICEID DeviceIdClone(PDEVICEID pId);
PDEVICEID DeviceIdParseNameString(LPCTSTR pszName, DWORD dwFlags);
VOID DeviceIdDestroy(PDEVICEID pId);
PDEVICE_STATE DeviceStateCreate(LPCTSTR pszName);
BOOL DeviceStateDestroy(PDEVICE_STATE pds);
BOOL DeviceStateAddList(PDEVICE_LIST pList, PDEVICE_STATE pdsDevice);
BOOL DeviceStateRemList(PDEVICE_STATE pds);
PDEVICE_STATE DeviceStateFindList(PDEVICE_LIST pdl, LPCTSTR pszName);
PDEVICE_LIST GetDeviceListFromClass(LPCGUID guidDevClass);
BOOL CheckDevicePointer(PDEVICE_STATE pds);
BOOL UpdateDeviceState(PDEVICE_STATE pds);
VOID UpdateClassDeviceStates(PDEVICE_LIST pdl);
VOID UpdateAllDeviceStates(VOID);
PDEVICE_POWER_RESTRICTION PowerRestrictionCreate(PDEVICEID pDeviceId, 
    HANDLE hOwner, CEDEVICE_POWER_STATE Dx, LPCTSTR pszSystemState, DWORD dwFlags);
BOOL PowerRestrictionDestroy(PDEVICE_POWER_RESTRICTION pdpr);
BOOL PowerRestrictionAddList(PPDEVICE_POWER_RESTRICTION ppdsList, 
    PDEVICE_POWER_RESTRICTION pdpr);
BOOL PowerRestrictionRemList(PPDEVICE_POWER_RESTRICTION ppListHead, 
    PDEVICE_POWER_RESTRICTION pdpr);
PDEVICE_POWER_RESTRICTION PowerRestrictionFindList(PDEVICE_POWER_RESTRICTION pList, 
    PDEVICEID pDeviceId, LPCTSTR pszSystemState);
BOOL PowerRestrictionCheckList(PDEVICE_POWER_RESTRICTION pList, 
    PDEVICE_POWER_RESTRICTION pdpr);
VOID UpdateRequirementDevices(PDEVICEID pDeviceId);
VOID DeleteProcessRequirements(HANDLE hProcess);
PPOWER_NOTIFICATION PowerNotificationCreate(HANDLE hMsgQ, HANDLE hProcess);
BOOL PowerNotificationDestroy(PPOWER_NOTIFICATION ppn);
BOOL PowerNotificationAddList(PPPOWER_NOTIFICATION ppListHead, PPOWER_NOTIFICATION ppn);
BOOL PowerNotificationRemList(PPPOWER_NOTIFICATION ppListHead, PPOWER_NOTIFICATION ppn);
PACTIVITY_TIMER ActivityTimerCreate(LPTSTR pszName, DWORD dwTimeout, LPTSTR pszSources);
BOOL ActivityTimerDestroy(PACTIVITY_TIMER pat);
PACTIVITY_TIMER ActivityTimerFindByName(LPCTSTR pszName);
PACTIVITY_TIMER ActivityTimerFindByWakeSource(DWORD dwWakeSource);
PDEVICE_LIST DeviceListCreate(LPCGUID pClass);
VOID DeviceListDestroy(PDEVICE_LIST pdl);
VOID DeleteProcessNotifications(HANDLE hProcess);
DWORD RegReadSystemPowerState(LPCTSTR pszName, PPSYSTEM_POWER_STATE ppsps, 
    PPDEVICE_POWER_RESTRICTION ppdpr);
PSYSTEM_POWER_STATE SystemPowerStateCreate(LPCTSTR pszName);
BOOL SystemPowerStateDestroy(PSYSTEM_POWER_STATE psps);
VOID SendNotification(PPOWER_NOTIFICATION ppn, PPOWER_BROADCAST ppb, DWORD dwLen);
VOID GenerateNotifications(PPOWER_BROADCAST ppb) ;
DWORD PmSetSystemPowerState_I(LPCWSTR pwsState, DWORD StateFlags,
    DWORD Options, BOOL fInternal);
BOOL PmUpdateSystemPowerStatesIfChanged ();

// function prototypes -- PDD functions called by the MDD
VOID WINAPI PlatformResumeSystem(VOID);
DWORD WINAPI PlatformValidatePMRegistry(VOID);
VOID WINAPI PlatformManageSystemPower(HANDLE);
DWORD WINAPI PlatformMapPowerStateHint(DWORD, LPTSTR, DWORD); 
DWORD WINAPI PlatformSetSystemPowerState(LPCTSTR pwsState, BOOL fForce, BOOL fInternal);
VOID WINAPI PlatformSendInitialNotifications(PPOWER_NOTIFICATION ppn, DWORD dwFlags);
BOOL WINAPI PlatformDeviceListInit(PDEVICE_LIST);

// function prototypes -- common PDD functions
HANDLE PmPolicyCreateNotificationQueue(void);
void PmPolicyCloseNotificationQueue(HANDLE hq);
DWORD PmPolicyReadNotificationQueue(HANDLE hqNotify, LPVOID pvBuffer, DWORD dwSize);
BOOL PmInitPowerStatus(HMODULE hmCoreDll);
BOOL PmUpdatePowerStatus(void);
DWORD GetNotificationRequestCount(DWORD dwMask) ;
BOOL SuspendButtonPressed(BOOL bReleaseSupported) ;
BOOL  SuspendButtonReleased(BOOL bReleaseSupported) ;
    
// global variables
#ifndef _DEFINE_PM_VARS
extern CRITICAL_SECTION gcsPowerManager;
extern CRITICAL_SECTION gcsDeviceUpdateAPIs;
extern const GUID idGenericPMDeviceClass;
extern const GUID idPMDisplayDeviceClass;
extern HANDLE ghevPmShutdown;
extern HANDLE ghevResume;
extern HANDLE ghevTimerResume;
extern HANDLE ghPmHeap;
extern HANDLE ghtResume;
extern HANDLE ghtSystem;
extern HANDLE ghtActivityTimers;
extern PDEVICE_LIST gpDeviceLists;
extern PDEVICE_POWER_RESTRICTION gpFloorDx;
extern PDEVICE_POWER_RESTRICTION gpCeilingDx;
extern PPOWER_NOTIFICATION gpPowerNotifications;
extern PSYSTEM_POWER_STATE gpSystemPowerState;
extern PPACTIVITY_TIMER gppActivityTimers;

#if defined(DEBUG) || !defined(_PM_NOCELOGMSG)
// we can use zones in retail builds as well now
extern  DBGPARAM    dpCurSettings;
#endif  // defined(DEBUG) || !defined(_PM_NOCELOGMSG)

#endif  // _DEFINE_PM_VARS

#ifdef __cplusplus
};
#endif  // __cplusplus
