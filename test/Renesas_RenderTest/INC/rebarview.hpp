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
#ifndef __REBARVIEW_HPP__
#define __REBARVIEW_HPP__
/*


Abstract:

This header file defines the interface to the RebarView_t class
which implements the customizable rebar gripper.

*/

#include <windows.h>

class RebarView_t
{
private:
    RebarView_t();									//	NOT IMPLEMENTED default constructor
	RebarView_t(const RebarView_t&);				//	NOT IMPLEMENTED default copy constructor
	RebarView_t& operator=(const RebarView_t&);		//	NOT IMPLEMENTED default assignment constructor
	~RebarView_t();									//	NOT IMPLEMENTED default destructor

public:
	static
	BOOL 
	DrawGripper(
		HDC		hdc,				// [in]
		LPRECT	lprc				// [in]
		);      
	
	static 
	void
	InitSpacingValues(
		int*	pstartspace,		// [out]
		int*	pgripperwidth,	// [out]
		int*	pinterspace		// [out]
		);

	/**********************
	Additional skinning functions
	/**********************/

	static
	void
	SetEdgeBorderType(
		UINT* puEdgeType,		// [out]
		UINT* puBorderType		// [out]
		);
};

#endif /* __REBARVIEW_HPP__ */
