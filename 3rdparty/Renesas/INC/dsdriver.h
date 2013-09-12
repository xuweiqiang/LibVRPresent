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
/*==========================================================================;
 *
 *
 *  File:       dsdriver.h
 *  Content:    DirectSound driver include file
 *
 ***************************************************************************/

#ifndef __DSDRIVER_INCLUDED__
#define __DSDRIVER_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//////////////////////////////////////////////////////////////////////////////////////
//
// DeviceIoControl Interface to Unified Audio Driver Model
//
//////////////////////////////////////////////////////////////////////////////////////

#define IOCTL_DSHAL_MESSAGE                CTL_CODE(FILE_DEVICE_SOUND,    5, METHOD_BUFFERED, FILE_ANY_ACCESS)

// The DirectSound features in a Winddows CE UADM (Unified Audio Driver Model) Driver,
// the interface methods use a DeviceIoControl transport.
// In the DeviceIoControl call:
//      * the control code is IOCTL_DSHAL_MESSAGE
//      * the input buffer is one of the DSDMP_xxx structures defined below
//        but in every case, the first DWORD of the input buffer is the
//        subfunction code that indentifies the interface method being invoked
//      * the input buffer size is at least big enough for the corresponding
//        DSDMP_xxx structure
//      * the output buffer is the HRESULT returned by the interface method
//      * the output buffer size is sizeof(HRESULT)
//
// Note that there is no QueryInterface or reference-counting over the DeviceIoControl transport
// The DeviceIoControl caller is responsible for reference counting, and will send a
// DSDM_CLOSE message when a driver buffer is finally released.
// The driver must support the DSDM_SETNOTIFICATION message.
// If the driver does not support property sets, it must return E_NOTIMPL in response to 
// the DSDM_QUERYPROP message.


//
#define DSDM_BASE                   100

#define DSDM_CREATEBUFFER           (DSDM_BASE +  0)
#define DSDM_DELETEBUFFER           (DSDM_BASE +  1)
#define DSDM_DUPLICATEBUFFER        (DSDM_BASE +  2)
#define DSDM_LOCK                   (DSDM_BASE +  3)
#define DSDM_UNLOCK                 (DSDM_BASE +  4)
#define DSDM_SETFORMAT              (DSDM_BASE +  5)
#define DSDM_SETFREQUENCY           (DSDM_BASE +  6)
#define DSDM_SETVOLUMEPAN           (DSDM_BASE +  7)
#define DSDM_SETPOSITION            (DSDM_BASE +  8)
#define DSDM_GETPOSITION            (DSDM_BASE +  9)
#define DSDM_PLAY                   (DSDM_BASE + 10)
#define DSDM_STOP                   (DSDM_BASE + 11)
#define DSDM_GETSTATUS              (DSDM_BASE + 12)
#define DSDM_SETNOTIFICATIONS       (DSDM_BASE + 13)
#define DSDM_GETPROP                (DSDM_BASE + 14)
#define DSDM_SETPROP                (DSDM_BASE + 15)
#define DSDM_QUERYPROP              (DSDM_BASE + 16)

typedef struct _DSDMP_MSG
{
    DWORD       dwMsg;            // DSDM_xxxx
    DWORD       dwInstance;       // Pointer to driver buffer instance data
} DSDMP_MSG, * PDSDMP_MSG;

typedef struct _DSDMP_CREATEBUFFER
{
    DWORD       dwMsg;            // DSDM_CREATEBUFFER
    PDWORD      pdwInstance;      // Pointer to driver buffer instance data for new buffer
    DWORD       dwDeviceIndex;    // Index of device on which which to create the buffer
    LPCDSBUFFERDESC pDesc;
} DSDMP_CREATEBUFFER, * PDSDMP_CREATEBUFFER;

typedef struct _DSDMP_DELETEBUFFER
{
    DWORD       dwMsg;              // DSDM_DELETEBUFFER
    DWORD       dwInstance;         // Driver buffer instance data
} DSDMP_DELETEBUFFER, *PDSDM_DELETEBUFFER;


typedef struct _DSDMP_DUPLICATEBUFFER 
{
    DWORD       dwMsg;              // DSDM_DUPLICATEBUFFER
    DWORD       dwInstance;         // Driver buffer instance data (existing buffer)
    PDWORD      pdwInstance;        // Pointer to driver buffer instance data for new buffer
} DSDMP_DUPLICATEBUFFER, *PDSDMP_DUPLICATEBUFFER;


typedef struct _DSDMP_LOCK 
{
    DWORD       dwMsg;              // DSDM_LOCK
    DWORD       dwInstance;         // Driver buffer instance data
    DWORD       dwOffset;           
    DWORD       dwCount;
    LPVOID *    ppBuffer1;
    LPDWORD     pdwCount1;
    LPVOID *    ppBuffer2;
    LPDWORD     pdwCount2;
    DWORD       dwFlags;
} DSDMP_LOCK, * PDSDMP_LOCK;


typedef struct _DSDMP_UNLOCK 
{
    DWORD       dwMsg;              // DSDM_UNLOCK
    DWORD       dwInstance;         // Driver buffer instance data
    LPVOID      pBuffer1;
    DWORD       dwCount1;
    LPVOID      pBuffer2;
    DWORD       dwCount2;
} DSDMP_UNLOCK, * PDSDMP_UNLOCK;


typedef struct _DSDMP_SETFORMAT 
{
    DWORD       dwMsg;              // DSDM_SETFORMAT
    DWORD       dwInstance;         // Driver buffer instance data
    LPWAVEFORMATEX   pWFX;
} DSDMP_SETFORMAT, * PDSDMP_SETFORMAT;


typedef struct _DSDMP_SETFREQUENCY 
{
    DWORD       dwMsg;              // DSDM_SETFREQUENCY
    DWORD       dwInstance;         // Driver buffer instance data
    DWORD       dwFreq;
} DSDMP_SETFREQUENCY, * PDSDMP_SETFREQUENCY;


typedef struct _DSDMP_SETVOLUMEPAN 
{
    DWORD       dwMsg;              // DSDM_SETVOLUMEPAN
    DWORD       dwInstance;         // Driver buffer instance data
    LONG        lVolume;
    LONG        lPan;
} DSDMP_SETVOLUMEPAN, * PDSDMP_SETVOLUMEPAN;


typedef struct _DSDMP_SETPOSITION 
{
    DWORD       dwMsg;              // DSDM_SETPOSITION
    DWORD       dwInstance;         // Driver buffer instance data
    DWORD       dwPosition;
} DSDMP_SETPOSITION, * PDSDMP_SETPOSITION;


typedef struct _DSDMP_GETPOSITION
{
    DWORD       dwMsg;              // DSDM_GETPOSITION
    DWORD       dwInstance;         // Driver buffer instance data
    PDWORD      pdwPlay;
    PDWORD      pdwWrite;
} DSDMP_GETPOSITION, * PDSDMP_GETPOSITION;


typedef struct _DSDMP_PLAY 
{
    DWORD       dwMsg;              // DSDM_PLAY
    DWORD       dwInstance;         // Driver buffer instance data
    DWORD       dwFlags;
} DSDMP_PLAY, * PDSDMP_PLAY;


typedef struct _DSDMP_STOP 
{
    DWORD       dwMsg;              // DSDM_STOP
    DWORD       dwInstance;         // Driver buffer instance data
} DSDMP_STOP, * PDSDMP_STOP;


typedef struct _DSDMP_GETSTATUS 
{
    DWORD       dwMsg;              // DSDM_GETSTATUS
    DWORD       dwInstance;         // Driver buffer instance data
    PDWORD      pdwStatus;
} DSDMP_GETSTATUS, * PDSDMP_GETSTATUS;


typedef struct _DSDMP_SETNOTIFICATIONS 
{
    DWORD       dwMsg;              // DSDM_SETNOTIFICATIONS
    DWORD       dwInstance;         // Driver buffer instance data
    DWORD       dwCount;            // 
    LPCDSBPOSITIONNOTIFY  pDSNP;
} DSDMP_SETNOTIFICATIONS, * PDSDMP_SETNOTIFICATIONS;


typedef struct _DSDMP_QUERYPROP 
{
    DWORD       dwMsg;              // DSDM_QUERYPROP
    DWORD       dwInstance;         // Driver buffer instance data
    LPGUID      pGUID;
    ULONG       ulPropId;
    PULONG      pulSupport;
} DSDMP_QUERYPROP, * PDSDMP_QUERYPROP;


typedef struct _DSDMP_GETPROP 
{
    DWORD       dwMsg;              // DSDM_GETPROP
    DWORD       dwInstance;         // Driver buffer instance data
    LPGUID      pGUID;
    ULONG       ulPropId;   
    PVOID       pParams;
    ULONG       cbParams;
    PVOID       pData;   
    ULONG       cbData;  
    PULONG      pcbReturnedData;  
} DSDMP_GETPROP, * PDSDMP_GETPROP;

typedef struct _DSDMP_SETPROP 
{
    DWORD       dwMsg;              // DSDM_SETPROP
    DWORD       dwInstance;         // Driver buffer instance data
    LPGUID      pGUID;
    ULONG       ulPropId;
    PVOID       pParams;
    ULONG       cbParams;
    PVOID       pData; 
    ULONG       cbData;    
} DSDMP_SETPROP, * PDSDMP_SETPROP;

typedef union
{
    DSDMP_MSG               msg;
    DSDMP_CREATEBUFFER      crb;
    DSDMP_DUPLICATEBUFFER   dpb;
    DSDMP_DELETEBUFFER      dlt;
    DSDMP_LOCK              lck;
    DSDMP_UNLOCK            ulk;
    DSDMP_SETFORMAT         sfo;
    DSDMP_SETFREQUENCY      sfr;
    DSDMP_SETVOLUMEPAN      svp;
    DSDMP_SETPOSITION       sps;
    DSDMP_GETPOSITION       gps;
    DSDMP_PLAY              ply;
    DSDMP_STOP              stp;
    DSDMP_GETSTATUS         gst;
    DSDMP_SETNOTIFICATIONS  snp;
    DSDMP_QUERYPROP         qpr;
    DSDMP_GETPROP           gpr;
    DSDMP_SETPROP           spr;
} DSDVR_MESSAGE_PARAMS, * PDSDVR_MESSAGE_PARAMS;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DSDRIVER_INCLUDED__
