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
#ifndef __INCLUDE_SIPDEV__
#define __INCLUDE_SIPDEV__

#ifdef __cplusplus
extern "C" {
#endif


//	Device io control commands for the sip.
//	Also used as dwCode for WM_SIPTHREADCALL.
#define SIPDEV_SHOW_IM			1
#define SIPDEV_GET_INFO 		2
#define SIPDEV_SET_INFO 		3
#define SIPDEV_GET_CURRENT_IM	4
#define SIPDEV_SET_CURRENT_IM	5
#define SIPDEV_REGISTER_UI		6
#define	SIPDEV_DEFAULT_RECT		7
#define	SIPDEV_ENUMERATE		8
#define SIPDEV_IMM_SET_ACTIVE_CONTEXT	9


struct SIP_IMM_SET_ACTIVE_CONTEXT
	{
	HWND	hwnd;
	BOOL	bOpen;
	DWORD	dwConversion;
	DWORD	dwSentence;
	HKL		hkl;
	};



#ifdef __cplusplus
};
#endif

#endif//__INCLUDE_SIPDEV__
