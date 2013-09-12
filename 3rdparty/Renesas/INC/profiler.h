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
//------------------------------------------------------------------------------
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//  
//------------------------------------------------------------------------------
//
//  This module contains the function prototypes and constant, type and
//  structure definitions for the Windows CE profiling functionality.
//
//------------------------------------------------------------------------------
#ifndef _PROFILER_H_
#define _PROFILER_H_

#ifdef __cplusplus
extern "C" {
#endif

#pragma warning(disable:4200) // nonstandard extensions warning


//------------------------------------------------------------------------------
//
// Control APIs exposed to applications
//
//------------------------------------------------------------------------------

// ProfileStartEx uses this structure
typedef struct ProfilerControl {
    DWORD dwVersion;                // Version of this struct, set to 1
    DWORD dwOptions;                // PROFILE_* flags
    DWORD dwReserved;
    union {
        // Used to control the kernel profiler, if PROFILE_OEMDEFINED is not set
        struct {
            DWORD dwUSecInterval;   // Sampling interval
        } Kernel;
        
        // Used to control an OEM-defined profiler, if PROFILE_OEMDEFINED is set
        struct {
            DWORD dwProcessorType;  // Type of processor expected
            DWORD dwControlSize;    // Size of the OEM-defined data
            BYTE  bHardwareSpecificSettings[0]; // Followed by OEM-defined data
        } OEM;
    };
} ProfilerControl;


//
// dwOptions to ProfileStart
//
#define PROFILE_OBJCALL          0x00000001  // record object calls
#define PROFILE_KCALL            0x00000002  // record kcalls
#define PROFILE_BUFFER           0x00000004  // record hits to buffer (cannot be used with PROFILE_CELOG)
#define PROFILE_PAUSE            0x00000008  // pause recording
#define PROFILE_CONTINUE         0x00000010  // resume recording
#define PROFILE_STARTPAUSED      0x00000020  // start in paused state
#define PROFILE_CELOG            0x00000040  // record hits via CeLog
#define PROFILE_OEMDEFINED       0x00000080  // control OEM-defined profiler
#define PROFILE_START            0x00000100  // start recording
#define PROFILE_STOP             0x00000200  // stop recording
#define PROFILE_OEM_QUERY        0x00000400  // query current OEM counters
// Record callstack with every hit (requires PROFILE_CELOG).
#define PROFILE_CALLSTACK        0x00000800
// When the INPROC flag is used with PROFILE_CALLSTACK, the profiler will record
// only the (bottom) part of the callstack that's within the thread's owner
// process, without crossing any PSL boundaries.  If the INPROC flag is not set,
// PROFILE_CALLSTACK will record the thread's entire callstack.
#define PROFILE_CALLSTACK_INPROC 0x00001000


DWORD ProfileStart(DWORD dwUSecInterval, DWORD dwOptions);
DWORD ProfileStartEx(ProfilerControl *pControl);
VOID ProfileCaptureStatus();
void ProfileStop(void);


// IOCTL_HAL_OEM_PROFILER uses this struct to return hardware-specific profiling
// data on a query.  The OAL ISR uses it to pass data to ProfilerHitEx.
typedef struct OEMProfilerData {
    DWORD ra;           // Return address of profiling interrupt
    DWORD dwBufSize;    // Size of the following data buffer, does not include this struct
    BYTE  buf[0];       // Followed by the buffer of data
} OEMProfilerData;


//------------------------------------------------------------------------------
//
// OEM-implemented profiler functions.
//
//------------------------------------------------------------------------------
void OEMProfileTimerDisable(void);
void OEMProfileTimerEnable(DWORD dwUSecInterval);


//------------------------------------------------------------------------------
//
// Kernel functions used by the OAL to send data to the kernel profiler
//
//------------------------------------------------------------------------------
// BUGBUG can't expose because it's declared in different places with different types!
//void ProfilerHit(unsigned int ra);
void ProfilerHitEx(OEMProfilerData *pData);  // BUGBUG NKProfilerHitEx?



//------------------------------------------------------------------------------
//
// CeLog profiler events
//
//------------------------------------------------------------------------------

// CELID_PROFILER_START
typedef ProfilerControl _CEL_PROFILER_START, CEL_PROFILER_START, *PCEL_PROFILER_START;

// CELID_PROFILER_STOP does not use a struct

// CELID_MONTECARLO_HIT
typedef struct _CEL_MONTECARLO_HIT {
    DWORD   dwReturnAddr;
} CEL_MONTECARLO_HIT, *PCEL_MONTECARLO_HIT;

// CELID_OEMPROFILER_HIT
typedef struct _CEL_OEMPROFILER_HIT {
    DWORD   dwReturnAddr;   // Return address of profiling interrupt
    DWORD   dwBufSize;      // Size of the following data buffer, does not include this struct
    BYTE    bData[0];       // Followed by the buffer of data
} CEL_OEMPROFILER_HIT, *PCEL_OEMPROFILER_HIT;


// The following functions are available only to code that is linked into the
// kernel.
#ifdef IN_KERNEL

//------------------------------------------------------------------------------
//
// Returns a pointer to the name of the module which started the given thread. 
// If the module cannot be found, the return value is NULL.
//
//------------------------------------------------------------------------------
LPCSTR GetModName(
   PTHREAD pth,
   unsigned int pc
   ); 


//------------------------------------------------------------------------------
//
// Returns a pointer to the name of the function the given thread was started 
// with.  If the function cannot be found, the return value is NULL.
//
//------------------------------------------------------------------------------
LPCSTR GetFunctionName(
   PTHREAD pth,
   unsigned int pc
   );


//------------------------------------------------------------------------------
//
// The current module handle and function name will be copied into the given args. 
// If the values cannot be found, the handle will be INVALID_HANDLE_VALUE and 
// the string will be empty. 
//
//------------------------------------------------------------------------------
void GetThreadName(
    PTHREAD pth,
    HANDLE* hModule,
    WCHAR*  szThreadFunctionName
    );


#endif // IN_KERNEL


#pragma warning(default:4200) // nonstandard extensions warning

#ifdef __cplusplus
}
#endif


#endif // _PROFILER_H_
