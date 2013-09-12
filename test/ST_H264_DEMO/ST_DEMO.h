// ST_DEMO.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CST_DEMOApp:
// See ST_DEMO.cpp for the implementation of this class
//

class CST_DEMOApp : public CWinApp
{
public:
	CST_DEMOApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CST_DEMOApp theApp;
