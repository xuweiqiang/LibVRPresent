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
#ifndef _LAYMGR_H_
#define _LAYMGR_H_


#include <keybddbg.h>

// wPddMask bit masks
#define PS2_AT_PDD  0x0002
#define PS2_NOP_PDD 0x0004

// 0x0100, 0x0200, 0x0400, and 0x0800 are reserved for OEM use.


#ifndef dim
#define dim(x) (sizeof(x)/sizeof(x[0]))
#endif


#endif // _LAYMGR_H_
