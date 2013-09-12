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
#ifndef __HEADERVIEW_HPP__
#define __HEADERVIEW_HPP__
/*


Abstract:

This header file defines the interface to the HeaderView_t class
which implements the customizable header control border.

*/

#include <windows.h>

class HeaderView_t
{
private:
    HeaderView_t();									//	NOT IMPLEMENTED default constructor
	HeaderView_t(const HeaderView_t&);				//	NOT IMPLEMENTED default copy constructor
	HeaderView_t& operator=(const HeaderView_t&);	//	NOT IMPLEMENTED default assignment constructor
	~HeaderView_t();								//	NOT IMPLEMENTED default destructor

public:
	static
	void 
	DrawButtonBorder(
		HDC 	hdc, 		// [in]
		DWORD	style,		// [in]
		LPRECT 	prc, 		// [in]
		BOOL 	fItemSunken	// [in]
		);
	
	// used to determine where to draw the header bitmap/text
	static
	void
	CalcOffset(
		int		cxBorder,	// [in]
		int		cyBorder,	// [in]
		int*	pxOffset, 	// [out]
		int* 	pyOffset	// [out]
		);      
	
	static
	int
	Height(
		int	cyChar,		// [in]
		int cyEdge		// [in]
		);      
};

#endif /* __HEADERVIEW_HPP__ */

