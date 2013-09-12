// RenderTest.h : main header file for the RenderTest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"

// CRenderTestApp:
// See RenderTest.cpp for the implementation of this class
//

class CRenderTestApp : public CWinApp
{
public:
	CRenderTestApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
public:
	afx_msg void OnAppAbout();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnStartTest();
};

extern CRenderTestApp theApp;
