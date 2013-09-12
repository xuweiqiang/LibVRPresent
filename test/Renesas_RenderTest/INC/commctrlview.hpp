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
#ifndef __COMMCTRLVIEW_HPP_INCLUDED__
#define __COMMCTRLVIEW_HPP_INCLUDED__
/*


Abstract:

This header file defines the interface to the CommCtrlView_t class
which implements the common control OEM-customizable initialization function.  
This function is called every time commctrl.dll is loaded.

*/

#include <windows.h>

class CommCtrlView_t
{
private:
    CommCtrlView_t();									//	NOT IMPLEMENTED default constructor
	CommCtrlView_t(const CommCtrlView_t&);				//	NOT IMPLEMENTED default copy constructor
	CommCtrlView_t& operator=(const CommCtrlView_t&);	//	NOT IMPLEMENTED default assignment constructor
	~CommCtrlView_t();									//	NOT IMPLEMENTED default destructor

public:
	static
	void
	Init(
		HANDLE hinstDLL
		);
};


#endif /* __COMMCTRLVIEW_HPP_INCLUDED__ */