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
#ifndef __GWEBYPASSCOREDLLTHUNK_HPP__
#define __GWEBYPASSCOREDLLTHUNK_HPP__

#ifndef __WINDOWS__
#include <windows.h>
#endif // __WINDOWS__

// This header allows you to call the internal version of the APIs
// thus bypassing the coredll thunks.  

struct ScrollBarInfoInternal;

class GweBypassCoredllThunk_t
{
public:

	static  
	LRESULT
	SendMessageW_I(
		HWND	hWnd,
		UINT	uMsg,
		WPARAM	wParam,
		LPARAM	lParam
		);

	static  
	HWND
	GetForegroundWindow_I(
		void
	);

	static  
	BOOL 
	SetWindowPos_I (
		HWND hwndThis,
		HWND hwndInsertAfter,
		int             x,
		int             y,
		int             cx,
		int             cy,
		UINT            fuFlags
		);

	static 
	BOOL 
	GetWindowRect_I (
		HWND            hwndThis,
		PRECT           prc
		);

	static 
	BOOL 
	GetClientRect_I (
		HWND            hwndThis,
		LPRECT          prc
		);

	static 
	BOOL 
	InvalidateRect_I (
		HWND            hwndThis,
        LPCRECT         prc,
        BOOL            fErase
        );

	static 
	BOOL
	InvalidateRgn_I (
		HWND            hwndThis,
        HRGN            hRgn,
        BOOL            fErase
        );

	static
	BOOL
	ValidateRgn_I  (
		HWND            hwndThis,
        HRGN            hRgn
        );
	
	static 
	HWND 
	GetWindow_I (
        HWND            hwndThis,
        UINT32          relation
        );

	static 
	HWND 
	ChildWindowFromPoint_I (
        HWND            hwndThis,
        POINT           pt
        );

	static 
	BOOL 
	ClientToScreen_I (
        HWND            hwndThis,
        POINT *         pPoint
        );


	static 
	BOOL 
	ScreenToClient_I (
        HWND            hwndThis,
        POINT *         pPoint
        );

	static 
	BOOL 
	SetWindowTextW_I (
        HWND            hwndThis,
        LPCTSTR         psz
        );

	static 
	INT 
	GetWindowTextW_I (
        HWND            hwndThis,
        LPTSTR          psz,
        int             cchMax
        );

	static 
	LONG 
	SetWindowLongW_I (
        HWND            hwndThis,
        int             nIndex,
        LONG            lNewLong
        );

    static 
	LONG 
	GetWindowLongW_I (
        HWND            hwndThis,
        int             nIndex
        );

	static 
	HDC 
	BeginPaint_I (
        HWND            hwndThis,
        PPAINTSTRUCT    pps
        );

	static 
	BOOL 
	EndPaint_I (
        HWND            hwndThis,
        PPAINTSTRUCT    pps
        );

	static 
	HDC 
	GetDC_I (
        HWND    hwndThis
        );

	static 
	int 
	ReleaseDC_I (
        HWND            hwndThis,
        HDC             hdc
        );

	static 
	LRESULT 
	DefWindowProcW_I (
        HWND            hwndThis,
        UINT            msg,
        WPARAM          wParam,
        LPARAM          lParam
        );

	static 
	DWORD 
	GetClassLongW_I (
        HWND            hwndThis,
        INT             nIndex
        );

	static 
	DWORD 
	SetClassLongW_I (
        HWND            hwndThis,
        INT             nIndex,
        LONG            lNewValue
        );

	static 
	BOOL 
	DestroyWindow_I (
        HWND            hwndThis
        );

	static 
	BOOL 
	ShowWindow_I (
        HWND            hwndThis,
        INT             nCmdShow
        );

	static 
	BOOL 
	UpdateWindow_I (
        HWND    hwndThis
        );

	static
	BOOL
	RedrawWindow_I(
	HWND 	hwndThis, 
	CONST RECT* lprcUpdate, 
	HRGN	hrgnUpdate, 
	UINT	flags);
	
	static 
	HWND 
	SetParent_I (
        HWND            hwndThis,
        HWND            hwndNewParent
        );

	static 
	HWND 
	GetParent_I (
        HWND    hwndThis
        );    

	static 
	HDC 
	GetWindowDC_I (
        HWND    hwndThis
        ) ;   

	static 
	BOOL 
	IsWindow_I (
        HWND  hwndThis
        ) ;   

	static 
	BOOL 
	MoveWindow_I (
        HWND            hwndThis,
        int             X,
        int             Y,
        int             nWidth,
        int             nHeight,
        BOOL            bRepaint
        );  

	static 
	int 
	GetUpdateRgn_I (
        HWND            hwndThis,
        HRGN            hRgn,
        BOOL            bErase
        );

	static 
	BOOL 
	GetUpdateRect_I (
        HWND            hwndThis,
        LPRECT          lpRect,
        BOOL            bErase
        );  

	static 
	BOOL 
	BringWindowToTop_I (
        HWND  hwndThis
        );  

	static 
	int
	GetWindowTextLengthW_I (
        HWND hwndThis
        );   

	static 
	BOOL 
	IsChild_I (
        HWND            hwndThis,
        HWND            hWnd
        );      

    static 
	BOOL 
	IsWindowVisible_I (
        HWND            hwndThis
        ) ; 

	static 
	BOOL 
	ValidateRect_I (
        HWND            hwndThis,
        CONST RECT      *lpRect
        );   
	
	static 
	int 
	GetClassNameW_I (
        HWND            hwndThis,
        PTSTR           szClassName,
        int             cchClassName
        );

    static 
	BOOL 
	EnableWindow_I (
        HWND hwndThis,
        BOOL bEnable
        );

    static 
	BOOL 
	IsWindowEnabled_I(
		HWND hwndThis
		);

    static 
	int 
	ScrollWindowEx_I(
        HWND            hwndThis,
        int             dx,
        int             dy,
        CONST RECT *    prcScroll,
        CONST RECT *    prcClip ,
        HRGN            hrgnUpdate,
        LPRECT          prcUpdate,
        UINT            flags
        );

	static 
	BOOL 
	RectangleAnimation_I (
		HWND                hwndThis,
		LPCRECT             prc,
		BOOL                    fMaximize
        ) ;

	static 
	DWORD 
	GetWindowThreadProcessId_I(
		HWND                hwndThis,
		LPDWORD             lpdwProcessId
        );

	static 
	void 
	SetAssociatedMenu_I (
        HWND  hwndThis,
    	HMENU hmenu
    	);   

	static 
	HMENU 
	GetAssociatedMenu_I (
    	HWND    hwndThis
    	);   

	static 
	HDC 
	GetDCEx_I(
		HWND    hwndThis,
		HRGN	hrgnClip,
		DWORD	flags
		);         

	static 
	int 
	SetWindowRgn_I  (
		HWND hwndThis,
		HRGN hrgn,
		BOOL bRedraw
        );   

	static 
	int 
	GetWindowRgn_I  (
		HWND hwndThis,
		HRGN hrgn
        );
	
	static
	ScrollBarInfoInternal *
	GetSBInfoInternal(
		HWND	hwnd,
		BOOL	fVert
		);      

	static
	BOOL
	SetProp_I(
		HWND hWnd,       
		LPCTSTR lpString,
		HANDLE hData     
		);
	
	static
	HANDLE 
	GetProp_I(
		HWND hWnd,       
		LPCTSTR lpString 
		);
	
	static
	HANDLE 
	RemoveProp_I(
		HWND hWnd,       
		LPCTSTR lpString 
		);
	
	static
	int 
	EnumPropsEx_I( 
		HWND hWnd,       
		PROPENUMPROCEX lpEnumFunc,
		LPARAM lParam,
		HPROCESS hProcCallingContext
		);
};
								   
class StockObjectHandles
{
public:
    static
    HGDIOBJ
    GetStockObject_I(
        int IdxObject
    );
};      


//BOOL
//WINAPI
//SetForegroundWindow_I(
//	HWND	hwnd
//	);


//UINT
//WINAPI
//SetTimer_I(
//	HWND		hwnd,
//	UINT		idTimer,
//	UINT		uTimeout,
//	TIMERPROC	tmprc
//	);

//BOOL
//WINAPI
//KillTimer_I(
//	HWND		hwnd,
//	UINT		uIDEvent
//	);

//HICON
//WINAPI
//LoadIconW_I(
//	HINSTANCE	hinst,
//	PCTSTR		pszIcon
//    );

//BOOL 
//WINAPI 
//DrawIconEx_I(
//	HDC		hdc, 
//	int		X, 
//	int		Y,
//	HICON	hicon, 
//	int		cx, 
//	int		cy,
//	UINT	istepIfAniCur, 
//	HBRUSH	hbrFlickerFreeDraw, 
//	UINT	diFlags
//	);


//BOOL
//WINAPI
//DestroyIcon_I(
//	HICON	hicon
//    );


//HICON
//WINAPI
//CreateIconIndirect_I(
//	PICONINFO	pii
//	);

// BUGBUG: We only support disabling a scroll bar in its entirity.
#define WF_NC_SBH_PRESENT	0x00000020	//	Horizontal scroll bar present
#define	WF_NC_SBV_PRESENT	0x00000010	//	Vertical scroll bar present
#define	WF_NC_SBV_SCROLLING	0x00000080	//	Vertical non-client scrollbar
#define WF_SBV_DISABLED		0x00000200	//	Draw vertical scroll bar disabled
#define WF_SBH_DISABLED		0x00000400 	//	Draw horz scroll bar disabled
#define WF_NC_DISABLE_CLOSE	0x00000100	//	Draw caption close box disabled


#define	WF_WIN_IS_SB_CTL         0x00000040      // Scroll bar control window





#endif // __GWEBYPASSCOREDLLTHUNK_HPP__
