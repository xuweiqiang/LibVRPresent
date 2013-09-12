// LibVideoMixer_Test.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CLibVideoMixer_TestApp:
// See LibVideoMixer_Test.cpp for the implementation of this class
//

class CLibVideoMixer_TestApp : public CWinApp
{
public:
	CLibVideoMixer_TestApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CLibVideoMixer_TestApp theApp;