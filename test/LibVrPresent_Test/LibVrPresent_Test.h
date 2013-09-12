// LibVrPresent_Test.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CLibVrPresent_TestApp:
// See LibVrPresent_Test.cpp for the implementation of this class
//

class CLibVrPresent_TestApp : public CWinApp
{
public:
	CLibVrPresent_TestApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CLibVrPresent_TestApp theApp;
