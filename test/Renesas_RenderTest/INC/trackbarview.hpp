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
#include <windows.h>
/*


Abstract:

This header file defines the interface to the TrackBarView_t class
which implements the customizable trackbar tics, thumb, and channel.

*/

class TrackBarView_t
{
private:
    TrackBarView_t();									//	NOT IMPLEMENTED default constructor
	TrackBarView_t(const TrackBarView_t&);				//	NOT IMPLEMENTED default copy constructor
	TrackBarView_t& operator=(const TrackBarView_t&);	//	NOT IMPLEMENTED default assignment constructor
	~TrackBarView_t();									//	NOT IMPLEMENTED default destructor

public:

	static
	void
	PASCAL 
	DrawFocus(
		HWND	hwnd,		// [in]
		HDC		hdc			// [in]
		);

	static
	void
	PASCAL
	DrawThumb(
		HWND	hwnd,			// [in]
		HDC		hdc,			// [in]
		DWORD	style,			// [in]
		int		iThumbHeight,	// [in]
		int		iThumbWidth,	// [in]
		LPRECT	lprc,			// [in]
		BOOL	fSelected		// [in]
		);

	static
	void
	PASCAL
	DrawTics(
		HDC		hdc,						// [in]
		DWORD	style,						// [in]
		RECT*	prc,						// [in]
		PDWORD	pTics,						// [in]
		int		nTics,						// [in]
		int		ticFreq,					// [in]
		LONG	lSelStart,					// [in]
		LONG	lSelEnd,   					// [in]
		LONG	lLogMin,					// [in]
		LONG	lLogMax,					// [in]
		LONG	iSizePhys,      	   		// [in]
		bool 	bDrawSelectionTriangles		// [in]
		);

	static
	void 
	PASCAL
	DrawChannel(
		HDC		hdc,						// [in]
		DWORD	style,						// [in]
		BOOL	bSelection,					// [in]
		LONG	lSelStart,					// [in]
		LONG	lSelEnd,   					// [in]
		LONG	lLogMin,					// [in]
		LONG	lLogMax,					// [in]
		LONG	iSizePhys,      			// [in]
		int		left,						// [in]
		LPRECT 	lprc						// [in]
		); 
};

