// TITIANII_OSD_TEST.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CTITIANII_OSD_TESTApp:
// See TITIANII_OSD_TEST.cpp for the implementation of this class
//

class CTITIANII_OSD_TESTApp : public CWinApp
{
public:
	CTITIANII_OSD_TESTApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTITIANII_OSD_TESTApp theApp;
