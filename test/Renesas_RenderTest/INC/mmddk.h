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
// -----------------------------------------------------------------------------
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//  ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//  PARTICULAR PURPOSE.
//  
// -----------------------------------------------------------------------------
//
//  MMDDK.H - Include file for Multimedia Device Development Kit
//
//  Note: You must include the WINDOWS.H header file before including this file.
//  
// -----------------------------------------------------------------------------
#ifndef _INC_MMDDK
#define _INC_MMDDK

#include "pshpack1.h"   // Assume byte packing throughout

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*    If defined, the following flags inhibit inclusion
 *    of the indicated items:
 *
 *        MMNOWAVEDEV         - Waveform support
 */
#ifdef MMNOWAVE
  #define MMNOWAVEDEV
#endif


#ifndef MMNOWAVEDEV
/****************************************************************************

		       Waveform device driver support

****************************************************************************/

// maximum number of wave device drivers loaded
#define MAXWAVEDRIVERS 10


//------------------------------------------------------------------------------
//
//  @doc    WDEV_EXT
//
//  @struct WAVEOPENDESC | 
//          The WAVEOPENDESC structure contains information needed by 
//          waveform input and output drivers. The 
//          structure is created by waveapi.dll and passed to the 
//          driver with a <m WODM_OPEN> or <m WIDM_OPEN> message. 
//  
//  @field  HWAVE | hWave |
//          Specifies the client’s handle to the device, as assigned 
//          by waveapi.dll.
//  
//  @field  LPWAVEFORMATEX | lpFormat |
//          Points to a WAVEFORMATEX structure, indicating the 
//          waveform data format requested by the client. (The 
//          WAVEFORMATEX structure is described in the Win32 SDK.) 
//  
//  @field  DWORD | dwCallback |
//          Specifies the pointer to the callback function inside waveapi.dll.
//  
//  @field  DWORD | dwInstance |
//          Contains the dwCallbackInstance argument that the client 
//          specified when calling the <f waveInOpen> or <f waveOutOpen>
//          function. 
//  
//  @field  UINT | uMappedDeviceID | 
//          For wave mapper, contains device identifier of mapped device. 
//
//------------------------------------------------------------------------------
typedef struct waveopendesc_tag {
    HWAVE          hWave;             // handle
    LPWAVEFORMATEX lpFormat;          // format of wave data
    DWORD          dwCallback;        // callback
    DWORD          dwInstance;        // app's private instance information
    UINT           uMappedDeviceID;   // device to map to if WAVE_MAPPED set
} WAVEOPENDESC;
typedef WAVEOPENDESC FAR *LPWAVEOPENDESC;


// Extended Device Caps - new in Windows CE 4.0
typedef struct tag_WAVEOUTEXTCAPS {
    DWORD   dwMaxHwStreams;             // max number of hw-mixed streams
    DWORD   dwFreeHwStreams;            // available HW streams
    DWORD   dwSwMixerSampleRate;        // preferred sample rate for software mixer (0 indicates no preference)
    DWORD   dwSwMixerBufferSize;        // preferred buffer size for software mixer (0 indicates no preference)
    DWORD   dwSwMixerBufferCount;       // preferred number of buffers for software mixer (0 indicates no preference)
    DWORD   dwMinSampleRate;            // minimum sample rate for a hw-mixed stream
    DWORD   dwMaxSampleRate;            // maximum sample rate for a hw-mixed stream
} WAVEOUTEXTCAPS;
typedef WAVEOUTEXTCAPS * PWAVEOUTEXTCAPS;

// Get/Set Property structure - new in Windows CE 5.0
typedef struct tag_WAVEPROPINFO {
    LPGUID  pPropSetId;         // identifier of the property set
    ULONG   ulPropId;           // identifier of the property within the property set
    LPVOID  pvPropParams;       // ptr to instance parameters for the property
    ULONG   cbPropParams;       // # bytes in the structure pointed to by pvPropParams
    LPVOID  pvPropData;         // ptr to buffer in which to store the property value
    ULONG   cbPropData;         // # bytes in the structure pointed to by pvPropData
    PULONG  pcbReturn;          // # bytes returned in the structure pointed to by pvPropData (Get only)
} WAVEPROPINFO, * PWAVEPROPINFO;

// messages sent to wodMessage() entry-point function
#define WODM_GETNUMDEVS       3
#define WODM_GETDEVCAPS       4
#define WODM_OPEN             5
#define WODM_CLOSE            6
#define WODM_PREPARE          7
#define WODM_UNPREPARE        8
#define WODM_WRITE            9
#define WODM_PAUSE            10
#define WODM_RESTART          11
#define WODM_RESET            12
#define WODM_GETPOS           13
#define WODM_GETPITCH         14
#define WODM_SETPITCH         15
#define WODM_GETVOLUME        16
#define WODM_SETVOLUME        17
#define WODM_GETPLAYBACKRATE  18
#define WODM_SETPLAYBACKRATE  19
#define WODM_BREAKLOOP        20
#define WODM_BUSY             21
#define WODM_GETEXTDEVCAPS    22    // new in Windows CE 4.0
#define WODM_GETPROP          23    // new in Windows CE 5.0
#define WODM_SETPROP          24    // new in Windows CE 5.0
#define WODM_MAPPER_STATUS              (DRVM_MAPPER_STATUS + 0)
#define WAVEOUT_MAPPER_STATUS_DEVICE    0
#define WAVEOUT_MAPPER_STATUS_MAPPED    1
#define WAVEOUT_MAPPER_STATUS_FORMAT    2

// messages sent to widMessage() entry-point function
#define WIDM_GETNUMDEVS  50
#define WIDM_GETDEVCAPS  51
#define WIDM_OPEN        52
#define WIDM_CLOSE       53
#define WIDM_PREPARE     54
#define WIDM_UNPREPARE   55
#define WIDM_ADDBUFFER   56
#define WIDM_START       57
#define WIDM_STOP        58
#define WIDM_RESET       59
#define WIDM_GETPOS      60
#define WIDM_GETPROP     61         // new in Windows CE 5.0
#define WIDM_SETPROP     62         // new in Windows CE 5.0
#define WIDM_MAPPER_STATUS              (DRVM_MAPPER_STATUS + 0)
#define WAVEIN_MAPPER_STATUS_DEVICE     0
#define WAVEIN_MAPPER_STATUS_MAPPED     1
#define WAVEIN_MAPPER_STATUS_FORMAT     2

#endif // ifndef MMNOWAVEDEV

#ifdef MMNOMIX
  #define MMNOMIXDEV
#endif


#ifndef MMNOMIXDEV
/****************************************************************************

               Audio Mixer device driver support

****************************************************************************/
//
//  maximum number of mixer drivers that can be loaded by MSMIXMGR.DLL
//
#define MAXMIXERDRIVERS     10


//
//  mixer device open information structure
//
//
typedef struct tMIXEROPENDESC
{
    HMIXER          hmx;            // handle that will be used
    LPVOID          pReserved0;     // reserved--driver should ignore
    DWORD           dwCallback;     // callback
    DWORD           dwInstance;     // app's private instance information

} MIXEROPENDESC, *PMIXEROPENDESC, FAR *LPMIXEROPENDESC;


//
//
//
//
#define MXDM_INIT                   100
#define MXDM_USER                   (MXDM_INIT + 1)

#define MXDM_BASE                   (1)
#define MXDM_GETNUMDEVS             (MXDM_BASE + 0)
#define MXDM_GETDEVCAPS             (MXDM_BASE + 1)
#define MXDM_OPEN                   (MXDM_BASE + 2)
#define MXDM_CLOSE                  (MXDM_BASE + 3)
#define MXDM_GETLINEINFO            (MXDM_BASE + 4)
#define MXDM_GETLINECONTROLS        (MXDM_BASE + 5)
#define MXDM_GETCONTROLDETAILS      (MXDM_BASE + 6)
#define MXDM_SETCONTROLDETAILS      (MXDM_BASE + 7)

#define MIXER_PATH	TEXT("Drivers\\Mixers")

/*
	IOCTL_MIX_OUT_MUTE format:
	
	LPVOID lpInBuffer				// unsigned int* or NULL
	DWORD nInBufferSize			// sizeof(unsigned int)	or 0
	LPVOID lpOutBuffer,			// unsigned int* or NULL
	DWORD nOutBufferSize			// sizeof(unsigned int)	or 0
	LPDWORD lpBytesReturned		// unsigned long int* or NULL
	LPOVERLAPPED lpOverlapped	// NULL - must be for WinCE
	
	Sets/Gets the MIX driver MUTE enable state, 1 = mute, 0 = unmute
*/

#define MIX_DEVICE_TYPE ((ULONG)0x8000)

#define IOCTL_MIX_MESSAGE \
    CTL_CODE(MIX_DEVICE_TYPE, (0x0040), METHOD_BUFFERED, FILE_ANY_ACCESS)

#endif /* MMNOMIXDEV */


#define MMDDKINC

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#include "poppack.h"        /* Revert to default packing */

#endif /* _INC_MMDDK */
