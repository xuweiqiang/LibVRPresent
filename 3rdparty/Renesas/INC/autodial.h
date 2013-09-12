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

autodial.h

Abstract:

Autodial function declarations.

Notes: 

--*/

#ifndef _AUTODIAL_H_
#define _AUTODIAL_H_
#include <windows.h>
#include <ras.h>

DWORD AutoDialInitializeModule(void);
DWORD AutoDialCleanupModule(void);
DWORD AutoDialStartConnection(TCHAR *szUseDialEntry);
DWORD AutoDialEndConnection(void);
DWORD AutoDialGetConnectionStatus(RASCONNSTATUS *lpRasConn, TCHAR *szRasName);
#endif   /* _AUTODIAL_H_ */
