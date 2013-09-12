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

Module Name:   e_to_k.h

Abstract:  
    This contains declarations of the corresponding EDBG routines for 
    KITL in order to support building both EDGG and KITL.
Functions:


Notes: 

--*/
#ifndef _ETOK_H_
#define _ETOK_H_

#define KITLOutputDebugString    EdbgOutputDebugString
#define OEMKitlGetSecs           OEMEthGetSecs
#define KITLDebugZone            EdbgDebugZone
#define pKITLRegisterDfltClient  pEdbgRegisterDfltClient
#define pKITLSend                pEdbgSend
#define pKITLRecv                pEdbgRecv
#define pKITLInitializeInterrupt pEdbgInitializeInterrupt

#define KITLRegisterDfltClient   EdbgRegisterDfltClient
#define KITLRegisterClient       EdbgRegisterClient
#define KITLDeregisterClient     EdbgDeregisterClient
#define KITLSend                 EdbgSend
#define KITLRecv                 EdbgRecv
#define KITLSetDebug             EdbgSetDebug

#endif _ETOK_H_

