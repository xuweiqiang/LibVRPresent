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
/*********************************************************************
 File:              gxinfo.h
 Description:   GameX device info
 *********************************************************************
 *********************************************************************/

#ifndef __GXINFO_H__
#define __GXINFO_H__

struct GXDeviceInfo {
	long idVersion;
	void * pvFrameBuffer;
	unsigned long cbStride;
	unsigned long cxWidth;
	unsigned long cyHeight;
	unsigned long cBPP;
	unsigned long ffFormat;
	short vkButtonUpPortrait;
	short vkButtonUpLandscape;
	POINT ptButtonUp;
	short vkButtonDownPortrait;
	short vkButtonDownLandscape;
	POINT ptButtonDown;
	short vkButtonLeftPortrait;
	short vkButtonLeftLandscape;
	POINT ptButtonLeft;
	short vkButtonRightPortrait;
	short vkButtonRightLandscape;
	POINT ptButtonRight;
	short vkButtonAPortrait;
	short vkButtonALandscape;
	POINT ptButtonA;
	short vkButtonBPortrait;
	short vkButtonBLandscape;
	POINT ptButtonB;
	short vkButtonCPortrait;
	short vkButtonCLandscape;
	POINT ptButtonC;
	short vkButtonStartPortrait;
	short vkButtonStartLandscape;
	POINT ptButtonStart;
	void * pvReserved1;
	void * pvReserved2;
};

struct GXScreenRect {
	DWORD dwTop;
	DWORD dwLeft;
	DWORD dwWidth;
	DWORD dwHeight;
};

#define kidVersion100 100
#define kfLandscape	0x8			// Screen is rotated 270 degrees
#define kfPalette	0x10
#define kfDirect	0x20
#define kfDirect555	0x40
#define kfDirect565	0x80
#define kfDirect888	0x100
#define kfDirect444	0x200

#define GETGXINFO       0x00020000

#endif
