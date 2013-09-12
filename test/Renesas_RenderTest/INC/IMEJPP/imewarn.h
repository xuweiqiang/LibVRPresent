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

Module:

	Microsoft Pocket IME (pIME) Ver 2.0

File Name:

	imewarn.h

Abstract:

	For control warnings.

Notes: 


--*/

/* for warning level 4 */

	/* "'%$L' : '%$T' differs in indirection to slightly different base types from '%$T'" */
#pragma warning(disable:4057)

	/* "'%$S' : named type definition in parentheses" */
#pragma warning(disable:4115)

	/* "nonstandard extension used : zero-sized array in struct/union" */
#pragma warning(disable:4200)

	/* "nonstandard extension used : nameless struct/union" */
#pragma warning(disable:4201)

	/* "nonstandard extension used : benign typedef redefinition" */
#pragma warning(disable:4209)

	/* "nonstandard extension used : bit field types other than int" */
#pragma warning(disable:4214)

	/* "nonstandard extension used : must specify at least a storage class or a type" */
#pragma warning(disable:4218)

	/* "cast truncates constant value" */
#pragma warning(disable:4310)

	/* "'%$S' : unreferenced inline function has been removed" */
#pragma warning(disable:4514)

	/* "Note: %Fs" */
#pragma warning(disable:4699)

	/* "'%$D' : function not expanded" */
#pragma warning(disable:4710)
