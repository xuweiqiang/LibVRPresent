// RenderTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RenderTest.h"
#include "MainFrm.h"

#include "RenderTestDoc.h"
#include "RenderTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRenderTestApp

BEGIN_MESSAGE_MAP(CRenderTestApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CRenderTestApp::OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, &CRenderTestApp::OnStartTest)
END_MESSAGE_MAP()



// CRenderTestApp construction
CRenderTestApp::CRenderTestApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CRenderTestApp object
CRenderTestApp theApp;

// CRenderTestApp initialization

BOOL CRenderTestApp::InitInstance()
{

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CRenderTestDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CRenderTestView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}


// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;	// return TRUE unless you set the focus to a control
			// EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// App command to run the dialog
void CRenderTestApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
static int STARTX =	0;
static int STARTY = 0;

static int SRC_WIDTH =	320;
static int SRC_HEIGHT =	240;

static int DST_WIDTH	 = 320;
static int DST_HEIGHT	= 240;

static int SCREEN_STRIDE	= 800;

#include "libvo.h"

void CRenderTestApp::OnStartTest()
{
	VO_init(NULL);

	int align = 4;
	STARTX &= ~(align -1 );
	STARTY &= ~(align -1 );
	SRC_WIDTH &= ~(align -1 );
	SRC_HEIGHT &= ~(align -1 );
	DST_WIDTH &= ~(align -1 );
	DST_HEIGHT &= ~(align -1 );

	VO_set_video_startpos(STARTX , STARTY);
	VO_set_video_parameter(SRC_WIDTH , SRC_HEIGHT , SRC_WIDTH, DST_WIDTH , DST_HEIGHT, SCREEN_STRIDE, 1);

	LPBYTE yBuf, cBuf;
	yBuf = (LPBYTE)malloc(SRC_WIDTH*SRC_HEIGHT);
	cBuf = (LPBYTE)malloc(SRC_WIDTH*SRC_HEIGHT/2);
	
	FILE* m_file = fopen("\\Storage Card\\cmmbvideo_qvga_ref.yuv", "rb");
	if(!m_file)	return;
	fseek(m_file, 0 , SEEK_SET);
	while((!feof(m_file)))
	{
		fread(yBuf ,sizeof(BYTE),SRC_WIDTH*SRC_HEIGHT,m_file);
		fread(cBuf ,sizeof(BYTE),SRC_WIDTH*SRC_HEIGHT/2,m_file);
		VO_write(yBuf, cBuf, NULL);
		VO_read();
	}
	free(yBuf);
	free(cBuf);
	VO_deinit();
}
