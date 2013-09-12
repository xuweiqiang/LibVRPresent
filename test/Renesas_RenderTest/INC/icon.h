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
/*


*/
#ifndef _ICON_H_
#define _ICON_H_

#include <windows.h>

HICON
WINAPI
LoadIconW_I(
	HINSTANCE	hinst,
	PCTSTR		pszIcon
    );

BOOL 
WINAPI 
DrawIconEx_I(
	HDC		hdc, 
	int		X, 
	int		Y,
	HICON	hicon, 
	int		cx, 
	int		cy,
	UINT	istepIfAniCur, 
	HBRUSH	hbrFlickerFreeDraw, 
	UINT	diFlags
	);


BOOL
WINAPI
DestroyIcon_I(
	HICON	hicon
    );


HICON
WINAPI
CreateIconIndirect_I(
	PICONINFO	pii
	);

BOOL 
GetIconInfo_I(
	HICON hIcon,
	PICONINFO piconinfo
	);



#endif /* _ICON_H_ */
