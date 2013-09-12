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
#ifndef __NCLIENTVIEW_HPP__
#define __NCLIENTVIEW_HPP__
/*


Component Name:

nclientview.hpp

Abstract:

This header file defines the interface to the GWE component "nclientview".
This component implements the customizable nonclient user interface. 

The "nclientview" component works in conjunction with the "nclient" component.

*/

// New hit test flag for the OK button in the caption.
#define HTOKBTN             22

// Internal DrawFrameControl flag
#define DFCS_FOREGROUND		0x10000

// CalcRect codes 
enum WidgetId
	{
	widInvalid = -1,
	widClose,
	widMaxRestore,
	widMin,
	widOk,
	widHelp
	};

typedef int NcHitTestCode;

class NonClientView_t
{
private:
    NonClientView_t();									//	NOT IMPLEMENTED default constructor
	NonClientView_t(const NonClientView_t&);			//	NOT IMPLEMENTED default copy constructor
	NonClientView_t& operator=(const NonClientView_t&);	//	NOT IMPLEMENTED default assignment constructor
	~NonClientView_t();									//	NOT IMPLEMENTED default destructor

public:
	
	static
	NcHitTestCode
	DoHitTest(
		HWND    hwnd,				// [in]
		POINT   pt,					// [in]
		UINT32	WindowState,		// [in]
		bool	bTopLevelWindow		// [in]               
		);

	static
	BOOL
	DoPaint(
		HWND	hwnd,				// [in]
		HRGN	hrgn,				// [in]
		UINT32	WindowState,		// [in]
		bool	bTopLevelWindow,	// [in]
		bool	bRestorable,		// [in]
		bool	bForeground			// [in]
		);

	static
	BOOL
	ExposedDrawFrameControl(
		HDC hdc,					// [in]
		RECT* prc,					// [in]
		UINT wType,					// [in]
		UINT wState					// [in]
		);

	static
	BOOL
	DrawFrameControl(
		HDC hdc,					// [in]
		RECT* lprc,					// [in]
		UINT wType, 				// [in]
		UINT wState					// [in]
		);

	static
	RECT
	WidgetRect(
		HWND	hwnd,				// [in]
		enum 	WidgetId	wid,	// [in]
		bool	bTopLevelWindow		// [in]
		);
	
	static
	RECT
	BoundingRect(
		HWND	hwnd			// [in]
		);						

	static
	void
	BoundPoint(
		RECT*	prcBound,		// [in]
		POINT*	ppt				// [in/out]
		);

	static
	void
	CalcClientRectWithNoScrollBars(
		HWND 	hwnd,			// [in]
		RECT*	lprc,			// [in/out]
		DWORD	dwStyle,		// [in]
		DWORD	dwExStyle		// [in]
		);

	static
	void
	CalcClientRectWithHorzScrollBars(
		RECT*	lprc			// [in/out]
		);
		
	static
	void
	CalcClientRectWithVertScrollBars(
		RECT*	lprc,			// [in/out]
		DWORD	dwExStyle		// [in]
		);

	static
	void
	NonClientView_t::
	CalcClientRectWithoutHorzScrollBars(
		RECT*	lprc
		);
	
	static
	bool
	NonClientView_t::
	DoesHorzScrollBarFit(
		RECT*	lprc			// [in]
		);

	static
	bool
	NonClientView_t::
	DoesVertScrollBarFit(
		RECT*	lprc			// [in]
		);

	static
	BOOL
	DrawBorder(
		HWND	hwnd,			// [in]
		HDC		hdc,			// [in]
		LPCRECT	lprc			// [in]
		);      
	
	static
	BOOL
	CalcCaptionRect(
		HWND hwnd,				// [in]
		RECT* lprc              // [out]
		);      
};

#endif /* __NCLIENTVIEW_HPP__ */

