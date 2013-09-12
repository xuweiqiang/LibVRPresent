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
#ifndef __LISTBOXVIEW_HPP_INCLUDED__
#define __LISTBOXVIEW_HPP_INCLUDED__
/*


Abstract:

This header file defines the interface to the GWE component "lbctlview".
This component implements the customizable portions of the listbox user interface. 

The "lbctlview" component works in conjunction with the "lbctl" component.

*/

class ListBoxView_t
{
public:
	static
	int
	ItemHeight(
		HWND	hwnd,	// [in]
		HDC		hdc		// [in]
		);

	static
	void
	DrawFocus(
		HDC		hdc,	// [in]
		RECT*	prc,	// [in]
		bool	bOn		// [in]
		);
	
	static
	int
	TopCoordinate(
		HDC		hdc,		// [in]
		RECT*	lprc,		// [in]
		int		ascent		// [in]
		);
};


#endif /* __LISTBOXVIEW_HPP_INCLUDED__ */