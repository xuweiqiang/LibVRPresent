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


Module Name:

    guidgen.h

Abstract:

    GUID generator for Windows CE

--*/
#if ! defined (__guidgen_H__)
#define __guidgen_H__		1

#if defined (__cplusplus)
extern "C" {
#endif

int CeGenerateGUID (GUID *);
int CeGenerateLUID (LUID *);

#if defined (__cplusplus)
};
#endif
#endif
