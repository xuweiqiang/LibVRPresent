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

Component Name:

gweadd.h

Abstract:  

This header file declares the function Initialize_AdditionalComponents().
If an additional components (beyond those that come with Windows CE) is added
to GWE via the cesysgen.bat file, and if that component needs to do some
initialization at boot time, then that components can implement this function.
GWE will call this function at boot time.
++*/

#ifdef __cplusplus
extern "C"
#endif
void Initialize_AdditionalComponents (void);
