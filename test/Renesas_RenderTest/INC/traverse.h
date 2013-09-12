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
#ifndef INCLUDED_TRAVERSE_H
#define INCLUDED_TRAVERSE_H

#include <pehdr.h>
#include <romldr.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef BOOL (*SIGNING_CALLBACK)(DWORD data, DWORD size);

BOOL SigProcessImage(const ROMHDR *promhdr, DWORD _rom_offset, SIGNING_CALLBACK _call_back);

#ifdef __cplusplus
}
#endif

#endif
