// vncviewer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CVncViewerApp:
// See vncviewer.cpp for the implementation of this class
//

class CVncViewerApp : public CWinApp
{
public:
	CVncViewerApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CVncViewerApp theApp;
