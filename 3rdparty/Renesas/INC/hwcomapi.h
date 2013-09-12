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

hwcomapi.h

Abstract:

Holds pdd level prototypes of win32 comm apis.

Notes: 


--*/

#ifdef __cplusplus
extern "C" {
#endif


ULONG
HWGetStatus(
    PVOID	pHead,
    LPCOMSTAT	lpStat
    );

VOID
HWReset(
    PVOID   pHead
    );

VOID
HWGetModemStatus(
    PVOID   pHead,
    PULONG  pModemStatus
    );

VOID
HWGetCommProperties(
    PVOID	pHead,
    LPCOMMPROP	pCommProp
    );

VOID
HWPurgeComm(
    PVOID pHead,
    DWORD   fdwAction
    );

#ifdef __cplusplus
}
#endif

    
