// SH4_t1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CSH4_t1App:
// See SH4_t1.cpp for the implementation of this class
//

class CSH4_t1App : public CWinApp
{
public:
	CSH4_t1App();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CSH4_t1App theApp;
