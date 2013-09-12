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
#ifndef __UPDOWNVIEW_HPP__
#define __UPDOWNVIEW_HPP__
/*


Abstract:

This header file defines the interface to the UpdownView_t class
which implements the customizable Updown control.

*/

#include <windows.h>

class UpdownView_t
{
private:
    UpdownView_t();									//	NOT IMPLEMENTED default constructor
	UpdownView_t(const UpdownView_t&);				//	NOT IMPLEMENTED default copy constructor
	UpdownView_t& operator=(const UpdownView_t&);	//	NOT IMPLEMENTED default assignment constructor
	~UpdownView_t();								//	NOT IMPLEMENTED default destructor

public:
	static
	void 
	HorzDrawSettings(
	       DWORD dwStyle,
	       int iVersion,
	       LPRECT prcBtn,
	       BOOL bEnabled,
	       BOOL fHasBorder,
	       unsigned fUp,
	       unsigned fDown,
	       int nPos,
	       int nUpper,
	       int nLower,
	       UINT uHot,
	       UINT* puLeftFlags, 
	       LPRECT prcLeft,
	       UINT* puRightFlags,
	       LPRECT prcRight
		);      

	static
	void 
	VertDrawSettings(
	       DWORD dwStyle,
	       int iVersion,
	       LPRECT prcBtn,
	       BOOL bEnabled,
	       BOOL fHasBorder,
	       unsigned fUp,
	       unsigned fDown,
	       int nPos,
	       int nUpper,
	       int nLower,
	       UINT uHot,
	       UINT* puUpFlags, 
	       LPRECT prcUp,
	       UINT* puDownFlags,
	       LPRECT prcDown
		);      

	static
	void
	SetSunkenBorder(
	       DWORD dwStyle,
	       UINT* puBorderType
	);

};

#endif /* __UpdownVIEW_HPP__ */


