// SH4_t1.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SH4_t1.h"
#include "SH4_t1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSH4_t1App

BEGIN_MESSAGE_MAP(CSH4_t1App, CWinApp)
END_MESSAGE_MAP()


// CSH4_t1App construction
CSH4_t1App::CSH4_t1App()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CSH4_t1App object
CSH4_t1App theApp;

// CSH4_t1App initialization

BOOL CSH4_t1App::InitInstance()
{

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CSH4_t1Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
