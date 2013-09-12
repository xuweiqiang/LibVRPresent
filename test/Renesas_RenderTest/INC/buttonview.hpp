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
#ifndef __BUTTONVIEW_HPP_INCLUDED__
#define __BUTTONVIEW_HPP_INCLUDED__
/*


Abstract:

This header file defines the interface to the GWE component "btnctlview".
This component implements the customizable button control user interface. 

The "btnctlview" component works in conjunction with the "btnctl" component.

*/

// CalcRect codes 
enum ButtonControlRectangleStyle
	{
	CBR_CLIENTRECT,
	CBR_CHECKBOX,
	CBR_CHECKTEXT,
	CBR_GROUPTEXT,
	CBR_GROUPFRAME,
	CBR_PUSHBUTTON
	};

class ButtonView_t 
{

private:
    ButtonView_t();								//	NOT IMPLEMENTED default constructor
	ButtonView_t(const ButtonView_t&);				//	NOT IMPLEMENTED default copy constructor
	ButtonView_t& operator=(const ButtonView_t&);	//	NOT IMPLEMENTED default assignment constructor
	~ButtonView_t();								//	NOT IMPLEMENTED default destructor
  
public:

	static
	void
	DrawText(         
		HWND    hwnd,								// [in]
		HDC     hdc, 								// [in]
		BOOL    dbt,								// [in]
		BOOL    fDepress,							// [in]
		UINT    pbfPush,							// [in]
		enum    ButtonControlRectangleStyle bcrs,	// [in]
		WORD    alignment							// [in]
		);
	
	static
	void
	DrawCheckBoxandRadioStyle(
		HWND    hwnd,		// [in]
		HDC     hdc,    	// [in]
		HBRUSH  hbr,    	// [in]
		WORD    alignment 	// [in]   
		);
	
	static
	void
	DrawPushStyle(
	  HWND		hwnd,	  	// [in] 
	  HDC		hdc,	  	// [in] 
	  HBRUSH	hbr, 	  	// [in]
	  UINT		pbfPush,  	// [in]
	  UINT		state	  	// [in]
	  );
	
	static
    HBRUSH 
    InitDC(
        HWND	hwnd,  	// [in]
        HDC		hdc  	// [in]
        );
	
	static
	void 
	ButtonView_t::
	DrawPushable(
		HDC 	hdc,	// [in]
		HBRUSH 	hbr,	// [in]
		RECT* 	lprc,	// [in] 
		UINT 	state,	// [in] 
		UINT 	flags	// [in]
		);

	static
	void
	DrawGroupBoxText(
		HWND	hwnd,		// [in]
		HBRUSH	hbr,		// [in]
		HDC		hdc,		// [in]
		WORD	alignment	// [in]
		);

	static
	void
	PaintNonPushStyle(
		HWND	hwnd,	// [in]
		HDC		hdc,	// [in]
		HBRUSH	hbr		// [in]
		);
	
	static
	void
	PaintGroupBox(
		HWND	hwnd,		// [in]
		HDC		hdc,		// [in]
		HBRUSH	hbr,	   	// [in]
		WORD	alignment	// [in]
		);
	
	static
	bool
	HitDetect(
		HWND	hwnd,	// [in]
		POINT	pt		// [in]
		);      
};

#endif /* __BUTTONVIEW_HPP_INCLUDED__ */
