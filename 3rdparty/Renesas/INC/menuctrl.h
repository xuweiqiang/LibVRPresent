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

menuctrl.h

Abstract:  

Notes: 


--*/
#ifndef _MENUCTRL_H_
#define _MENUCTRL_H_

#ifdef __cplusplus
extern "C" {
#endif


/* When creating a window of class "Menu", pass a PMENUCONTROLINFO as the 
   lpParam parameter to CreateWindow.
 */

#define MENUCONTROLCLASS TEXT("Menu")
 
typedef struct
{
    HINSTANCE hinst;        /* instance containing the menu resource */
    LPCTSTR   lpszMenuName; /* menu resource name or ID */
    HWND      hwndNotify;   /* the window that will receive menu notifications*/
} MENUCONTROLINFO, *PMENUCONTROLINFO, *LPMENUCONTROLINFO;

#ifdef __cplusplus
}
#endif

#endif /* _MENUCTRL_H_ */
