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
#ifndef __BUTTONCOMMON_HPP_INCLUDED__
#define __BUTTONCOMMON_HPP_INCLUDED__

const int GWL_BTNSTATE =	0;
const int GWL_BTNFONT  =	4;

// Buttons
const unsigned int BS_HORZMASK =    0x00000300;
const unsigned int BS_TYPEMASK =    0x000F;		// bStyle
const unsigned int HOR_ALIGN_MASK = 0x00000300;
const unsigned int VER_ALIGN_MASK = 0x00000C00;

const unsigned short BST_CHECKMASK   =   0x0003;

/* DrawBtnText codes */
const unsigned int DBT_TEXT =  0x0001;
const unsigned int DBT_FOCUS = 0x0002;
const unsigned int PBF_DEFAULT =     0x0002;
const unsigned int PBF_PUSHABLE =    0x0001;


unsigned 
int
TestWindowFlag(
	HWND			hwnd,
	unsigned int	flag
	);


#endif /* __BUTTONCOMMON_HPP_INCLUDED__ */
