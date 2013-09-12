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

mcommdlg.h

Abstract:  

Notes: 


--*/
#ifndef _INC_MCOMMDLG
#define _INC_MCOMMDLG

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WINCEMACRO
#error WINCEMACRO not defined __FILE__
#endif

// exported for commdlg (prototyped in commdlg.h)
#define GetOpenFileNameW IMPLICIT_DECL(BOOL, SH_SHELL, 3, \
		(LPOPENFILENAMEW))
#define GetSaveFileNameW IMPLICIT_DECL(BOOL, SH_SHELL, 4, \
		(LPOPENFILENAMEW))

#ifdef __cplusplus
}
#endif

#endif  /* _INC_MCOMMDLG */
