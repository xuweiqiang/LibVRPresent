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

defui.h

Abstract:  

Notes: 


--*/
#ifndef __DEFUI_H__
#define __DEFUI_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

BOOL GetDefUI3(
            PTSTR pszUser, 	     // [OUT] ptr to buffer at least UNLEN+1 long
            PTSTR pszDomain,     // [OUT] ptr to buffer at least DNLEN+1 long
            PCTSTR pszPassType,  // [IN] "LMPW" for LanMan password 
            PBYTE pbPass,        // [OUT] ptr to buffer long enough for specified credential type
            PDWORD pcbPass);     // [IN,OUT] size of credentials
BOOL GetDefUI(PTSTR pszUser, PTSTR pszDomain);
BOOL SetDefUI(PTSTR pszUser, PTSTR pszDomain, PTSTR pszPass);

#ifdef __cplusplus
}
#endif

#endif	// __DEFUI_H__

