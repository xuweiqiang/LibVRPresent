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

    MSPYIME.H

Abstract:

    Microsoft Pinyin IME (Simplified Chinese) specific defineds and APIs

Revision History:

    Version 1.0

--*/

#ifndef __MSPYIME_H__
#define __MSPYIME_H__

//
// MSPY3 pinyin types used by SetPinyinType() and GetPinyinType()
//

#define MSPY3_PINYIN_TYPE_FULL          1
#define MSPY3_PINYIN_TYPE_DOUBLE        2

//
// MSPY3 sets one of following error codes when ImmRegisterWord() or ImmUnregisterWord() fails
//

// Phrases longer than 9 characters are not supported.
#define ERROR_MSPY_EUDP_TOO_LONG                0x20000001

// The EUDP term contains surrogate, ANSI, symbol, or extended characters
#define ERROR_MSPY_EUDP_UNSUPPORT_CHAR          0x20000002

// A duplicate term exists in the built-in dictionary.
#define ERROR_MSPY_EUDP_BUILT_IN                0x20000005

// A duplicate term exists in the EUDP file.
#define ERROR_MSPY_EUDP_DUPLICATE               0x20000006

// Single characters are not supported.
#define ERROR_MSPY_EUDP_SINGLE_CHAR             0x20000009

// The EUDP has an invalid reading.
#define ERROR_MSPY_EUDP_INVALID_READING         0x2000000A

// The EUDP file is full.
#define ERROR_MSPY_EUDP_FULL                    0x2000000B

// Fatal error, including out of memory.
#define ERROR_MSPY_EUDP_FATAL_ERROR             0x2000000C

#ifdef __cplusplus
extern "C" {
#endif

//
// This function sets the pinyin type of the current MSPY3.
//    Parameter
//        dwType 
//            Specifies that Pinyin type for the current MSPY3. This can be one of the following values: 
//                MSPY3_PINYIN_TYPE_FULL 
//                MSPY3_PINYIN_TYPE_DOUBLE 
//    Return value
//        If the function succeeds, the return value is TRUE. Otherwise, the return value is FALSE.
//

BOOL WINAPI SetPinyinType(DWORD dwType);

//
// This function retrieves the pinyin type of the current MSPY3.
//    Parameter
//        pdwType 
//            Pointer to a DWORD buffer to receive the Pinyin type of the current MSPY3.
//    Return value
//        If the function succeeds, the return value is TRUE, 
//        and current MSPY3's pinyin type will be filled into buffer pointed by pdwType. 
//        Otherwise, the return value is FALSE.

BOOL WINAPI GetPinyinType(DWORD* pdwType);

//
// This function delete all the content that is stored in the EUDP file.
//    Parameter
//        None
//    Return value
//        No return value
//

void WINAPI ClearEudpContent(void);

#ifdef __cplusplus
} /* end of 'extern "C" {' */
#endif

#endif //__MSPYIME_H__
