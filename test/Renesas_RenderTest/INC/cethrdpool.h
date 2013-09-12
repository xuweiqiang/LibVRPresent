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


// CeThrdPool.h defines a subset of the desktop Windows Thread Pool and Event Handler
// API's.  Its primary use is for porting desktop code that uses these constructs to
// Windows CE.  Applications wishing to use this functionality should statically
// link to ceosutil.lib.  

// IMPORTANT NOTES: 
// (1) Unlike the desktop, applications must presently call 
//     CEDeInitializeThreadPool() upon shutdown.
//
// (2) These functions have not been independantly tested.  Instead they are tested
//     indirectly through the internal applications that make use of them.  Therefore
//     %100 compliance with the desktop is not guaranteed.


#ifndef __CE_THRDPOOL_H
#define __CE_THRDPOOL_H

// The following flags are the only ones that are supported on Windows CE for the functions below.
#define WT_EXECUTEDEFAULT       0x00000000
#define WT_EXECUTEINWAITTHREAD  0x00000004
#define WT_EXECUTEONLYONCE      0x00000008
#define WT_EXECUTELONGFUNCTION  0x00000010
#define WT_EXECUTEINPERSISTENTTHREAD 0x00000080

typedef WAITORTIMERCALLBACKFUNC WAITORTIMERCALLBACK;

#if defined (__cplusplus)
extern "C" {
#endif

BOOL RegisterWaitForSingleObject(PHANDLE phNewWaitObject, HANDLE hObject, WAITORTIMERCALLBACK Callback, PVOID Context, ULONG dwMilliseconds, ULONG dwFlags);
BOOL UnregisterWaitEx(HANDLE WaitHandle, HANDLE CompletionEvent);

HANDLE WINAPI CreateTimerQueue(void);
BOOL WINAPI DeleteTimerQueueEx(HANDLE TimerQueue, HANDLE CompletionEvent);

BOOL WINAPI CreateTimerQueueTimer(PHANDLE phNewTimer, HANDLE TimerQueue, WAITORTIMERCALLBACK Callback,
                                  PVOID Parameter, DWORD DueTime, DWORD Period, ULONG Flags);

BOOL WINAPI DeleteTimerQueueTimer(HANDLE TimerQueue, HANDLE Timer, HANDLE CompletionEvent);
BOOL WINAPI QueueUserWorkItem(LPTHREAD_START_ROUTINE Function, PVOID Context, ULONG Flags);

// This MUST be called to initialize the thread pool on Windows CE.  It is safe to call this from DllMain().
BOOL CEInitializeThreadPool(void);
// This MUST be called to deinitialize the thread pool on Windows CE.  It is NOT safe to call this from DllMain().
void CEDeInitializeThreadPool(void);

#if defined (__cplusplus)
};
#endif

#endif // __CE_THRDPOOL_H

