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
#ifndef __COMBOBOXVIEW_HPP_INCLUDED__
#define __COMBOBOXVIEW_HPP_INCLUDED__
/*


Abstract:

This header file defines the interface to the GWE component "cmbctlview".
This component implements the customizable portions of the combobox user interface. 

The "cmbctlview" component works in conjunction with the "cmbctl" component.

*/

class ComboBoxView_t
{
public:
	static
	void
	WindowStyleCustomization(
		HWND			hwnd,		// [in]
		DWORD        	Style,		// [in]
		DWORD      	 	ExStyle,	// [in]
		DWORD        	*pStyle,	// [out]
		DWORD      	 	*pExStyle  	// [out]
		);      
	
	static
	void
	DrawFocus(
		HDC		hdc,	// [in]
		RECT*	prc		// [in]
		);
	
	static
	int
	TopCoordinate(
		HDC		hdc,   	// [in]
		RECT*	lprc   	// [in]
		);
};

#endif /* __COMBOBOXVIEW_HPP_INCLUDED__ */
