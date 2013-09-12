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
#ifndef _PRIV_IMM_
#define _PRIV_IMM_

#ifdef __cplusplus
extern "C"  {
#endif

#define IGSC_DEFIMCFALLBACK		0x0002
HIMC
WINAPI
ImmGetContextFromWindowGwe(
	HWND	hWnd
	);

DWORD
WINAPI
ImmAssociateValueWithGwesMessageQueue(
	DWORD dwValue,
	UINT Flags
	);

BOOL
WINAPI
ImmAssociateContextWithWindowGwe(
	HWND	hwnd,
	HIMC	himc,
	DWORD	dwFlags,
	HIMC	*phimcPrev,
	HWND	*phwndFocus,
	HIMC	*phimcFocusPrev,
	HIMC	*phimcFocusNew
	);



void
WINAPI
ImmSetImeWndIMC(
	HIMC	hImc,
	HWND	hImeWnd
	);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif
