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
#ifndef __TOOLBARVIEW_HPP__
#define __TOOLBARVIEW_HPP__
/*


Abstract:

This header file defines the interface to the ToolBarView_t class
which implements the customizable toolbar control border.

Modify viewsm.bmp, stdsm.bmp, viewsm.2bp, and stdsm.2bp to customize
the system-defined button bitmaps IDB_STD_SMALL_COLOR and IDB_VIEW_SMALL_COLOR.

*/

#include <windows.h>

enum BitmapType
{
	btStdSmallColor,
	btStdSmallMono,
	btViewSmallColor,
	btViewSmallMono,
	btClose,
	btOk
};

class ToolBarView_t
{
private:
    ToolBarView_t();								//	NOT IMPLEMENTED default constructor
	ToolBarView_t(const ToolBarView_t&);			//	NOT IMPLEMENTED default copy constructor
	ToolBarView_t& operator=(const ToolBarView_t&);	//	NOT IMPLEMENTED default assignment constructor
	~ToolBarView_t();								//	NOT IMPLEMENTED default destructor

public:
	static
	void
	DrawButtonBorder(
		HDC		hdc, 		// [in]
		RECT*	prc,        // [in]
		bool	bPushed,    // [in]
		DWORD	style		// [in]
	);

	static
	void
	CalcOffset(
		int*	pxOffset, 		// [out]
		int*	pyOffset		// [out]
		);
	
	static
	void
	FaceRect(
		RECT*	prcButton,		// [in/out]
		int 	cxEdge,			// [in]
		int 	cyEdge,			// [in]
		int 	cxBorder,			// [in]
		int 	cyBorder			// [in]
		);
	
	static
	void
	InitializeButtonSlopValue(
		int* pxSlop,		// [out]
		int* pySlop			// [out]
		);

	static
	void
	DrawDropdownArrow(
		HDC	hdc,				// [in] 
		int	x, 					// [in]
		int	y, 					// [in]
		int	dx,					// [in]
		int	dy,					// [in]
		int	cxEdge,				// [in]
		int	cyEdge,				// [in]
		int	cxBorder,			// [in]
		int	cyBorder,			// [in]
		COLORREF clrBtnFace,	// [in]
		int	cxDropdownArrow,	// [in]
		int	yOffset,   			// [in]
		DWORD	style,   			// [in]
		DWORD	customDrawRet,   			// [in]
		BOOL	isPressedDropdown	// [in]
		);      

	static
	HBITMAP
	LoadSystemBitmap(
		enum BitmapType bt		// [in]
		);
};

#endif /* __TOOLBARVIEW_HPP__ */

