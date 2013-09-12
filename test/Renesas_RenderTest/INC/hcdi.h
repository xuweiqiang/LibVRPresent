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
#ifndef     __HCDI_H__
#define     __HCDI_H__

#ifndef     __USBTYPES_H__
#include    "usbtypes.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct  _HCD_FUNCS;
typedef struct _HCD_FUNCS HCD_FUNCS, * PHCD_FUNCS, * LPHCD_FUNCS;
typedef struct _HCD_FUNCS const * PCHCD_FUNCS;
typedef struct _HCD_FUNCS const * LPCHCD_FUNCS;

/******************************************************************************/
// Entry Points for USBD

BOOL HcdAttach(LPVOID lpvHcd, LPCHCD_FUNCS lpHcdFuncs, LPLPVOID lppvContext);
BOOL HcdDetach(LPVOID lpvContext);

BOOL HcdDeviceAttached(LPVOID lpvContext, UINT iDevice, UINT iEndpointZero,
                       LPCUSB_DEVICE lpDeviceInfo, LPLPVOID lppvDeviceDetach);
BOOL HcdDeviceDetached(LPVOID lpvDeviceDetach);



/******************************************************************************/
// HCD Entry Points
// NOTE: the USBD does not statically call these.  It calls them through
// a table of pointers that gets passed to it from the HCD (either OHIC or UHCI)


//USB Subsystem Commands
typedef BOOL (* LPHCD_GET_FRAME_NUMBER)(LPVOID, LPDWORD);
typedef BOOL (* LPHCD_GET_FRAME_LENGTH)(LPVOID, LPUSHORT);
typedef BOOL (* LPHCD_SET_FRAME_LENGTH)(LPVOID, HANDLE, USHORT);
typedef BOOL (* LPHCD_STOP_ADJUSTING_FRAME)(LPVOID);


//Controls pipe handling
typedef BOOL (* LPHCD_OPEN_PIPE)(LPVOID, UINT, LPCUSB_ENDPOINT_DESCRIPTOR,
                                 LPUINT);
typedef BOOL (* LPHCD_CLOSE_PIPE)(LPVOID, UINT, UINT);
typedef BOOL (* LPHCD_RESET_PIPE)(LPVOID, UINT, UINT);
typedef BOOL (* LPHCD_IS_PIPE_HALTED)(LPVOID, UINT, UINT, LPBOOL);


//Transfer commands
typedef BOOL (* LPHCD_ISSUE_TRANSFER)(LPVOID, UINT, UINT,
                                      LPTRANSFER_NOTIFY_ROUTINE, LPVOID, DWORD,
                                      LPCVOID, DWORD, DWORD, LPCDWORD, DWORD,
                                      LPVOID, ULONG, LPCVOID, LPDWORD, LPDWORD,
                                      LPBOOL, LPDWORD, LPDWORD);

typedef BOOL (* LPHCD_ABORT_TRANSFER)(LPVOID, UINT, UINT,
                                      LPTRANSFER_NOTIFY_ROUTINE, LPVOID,
                                      LPCVOID);

typedef BOOL (* LPHCD_DISABLE_DEVICE)(LPVOID lpvHcd, UINT iDevice, BOOL fReset);
BOOL HcdDisableDevice(LPVOID lpvHcd, UINT iDevice, BOOL fReset);

typedef BOOL (* LPHCD_SUSPEND_RESUME)(LPVOID lpvHcd, UINT iDevice, BOOL fSuspend);
BOOL HcdSuspendResume(LPVOID lpvHcd, UINT iDevice, BOOL fSuspend);

struct _HCD_FUNCS {
    DWORD                       dwCount;

    LPHCD_GET_FRAME_NUMBER      lpGetFrameNumber;
    LPHCD_GET_FRAME_LENGTH      lpGetFrameLength;
    LPHCD_SET_FRAME_LENGTH      lpSetFrameLength;
    LPHCD_STOP_ADJUSTING_FRAME  lpStopAdjustingFrame;
    LPHCD_OPEN_PIPE             lpOpenPipe;
    LPHCD_CLOSE_PIPE            lpClosePipe;
    LPHCD_RESET_PIPE            lpResetPipe;
    LPHCD_IS_PIPE_HALTED        lpIsPipeHalted;
    LPHCD_ISSUE_TRANSFER        lpIssueTransfer;
    LPHCD_ABORT_TRANSFER        lpAbortTransfer;
    LPHCD_DISABLE_DEVICE        lpDisableDevice;
    LPHCD_SUSPEND_RESUME        lpSuspendResume;
};

#ifdef __cplusplus
}
#endif

#endif

