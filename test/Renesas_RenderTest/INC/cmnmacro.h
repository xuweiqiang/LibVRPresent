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

cmnmacro.h

Abstract:

Some common macros

Notes: 


--*/
#ifndef _CMNMACRO_H_
#define _CMNMACRO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define LOINT16(l)           ((INT16)(l))
#define HIINT16(l)           ((INT16)(UINT16)(((UINT32)(l) >> 16) & 0xFFFF))

#define LOUINT16(l)          ((UINT16)(l))
#define HIUINT16(l)          ((UINT16)(((UINT32)(l) >> 16) & 0xFFFF))

#define CR_BLACK  RGB(0x00,0x00,0x00)
#define CR_DKGRAY RGB(0x80,0x80,0x80)
#define CR_LTGRAY RGB(0xc0,0xc0,0xc0)
#define CR_WHITE  RGB(0xff,0xff,0xff)

#ifdef __cplusplus
}
#endif

#endif /*_CMNMACRO_H_*/

