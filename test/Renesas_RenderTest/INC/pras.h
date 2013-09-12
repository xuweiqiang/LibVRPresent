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

ppras.h

Abstract:

Contains the private portions of ras.h

Notes: 


--*/
    
#ifndef _PRIV_RAS_H_
#define _PRIV_RAS_H_

#ifdef __cplusplus
extern "C" {
#endif



#ifdef UNDER_CE
#define RNA_GETINFO		3

// Our main dialog has DWL_USER set to the following value.
#define RNAAPP_MAGIC_NUM 0x006A6D6D

#endif // UNDER_CE



#ifdef __cplusplus
}
#endif


#endif // _PRIV_RAS_H_
