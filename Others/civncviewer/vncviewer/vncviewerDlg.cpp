// vncviewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "vncviewer.h"
#include "vncviewerDlg.h"
#include "dispdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CVncViewerDlg dialog

CVncViewerDlg::CVncViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVncViewerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVncViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SERVER, m_editServerIP);
}

BEGIN_MESSAGE_MAP(CVncViewerDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CVncViewerDlg::OnBnClickedButtonConnect)
END_MESSAGE_MAP()


// CVncViewerDlg message handlers

BOOL CVncViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_editServerIP.SetWindowText(_T("192.168.0.100"));
	m_editServerIP.EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CVncViewerDlg::OnSize(UINT nType, int cx, int cy)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_VNCVIEWER_DIALOG_WIDE) : 
		MAKEINTRESOURCE(IDD_VNCVIEWER_DIALOG));
	}
}
#endif


void CVncViewerDlg::OnBnClickedButtonConnect()
{
	CDispDlg dlg;
	CString serverAddr;
	m_editServerIP.GetWindowText(serverAddr);
	//dlg.SetVncServer(serverAddr);
	dlg.DoModal();
}
