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
    iplcommon.h

Abstract:
    IPL common header file. This file contains all the type and
    function definitions used by boot loader.

Functions:


Notes:

--*/
#ifndef _IPLCOMMON_H_
#define _IPLCOMMON_H_

// Message code enumeration.
//
typedef enum
{
    IPL_ERROR_BAD_IPLTOC,
    IPL_ERROR_BPINIT_FAILED,
    IPL_ERROR_OPENPARTITION_FAILED,
    IPL_ERROR_GETSTORAGETYPE_FAILED,
    IPL_ERROR_GETIMAGEINFO_FAILED,
    IPL_ERROR_TRANSLATEADDR_FAILED,
    IPL_ERROR_READDATA_FAILED,
    IPL_ERROR_JUMP_FAILED,
    IPL_INFO_LOADING_IMAGE,
    IPL_INFO_JUMPING_IMAGE
} IPL_MESSAGE_CODE;


// Messaging handler callback pointer.
//
typedef void (*PFN_MessageHandler) (IPL_ERROR_CODE, LPWSTR);


// IPLcommon Function prototypes.
//
void IPLmain(void);
BOOLEAN OEMGetUpdateMode(void);
BOOLEAN OEMTranslateBaseAddress(UINT32 ulPartType, UINT32 ulAddr, UINT32 *pulTransAddr);
BOOLEAN OEMInitDebugMessages(void);
void OEMLaunchImage(UINT32 ulLaunchAddr);

#endif  // _IPLCOMMON_H_.
