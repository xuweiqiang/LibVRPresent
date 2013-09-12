// TITIANII_OSD_TESTDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TITIANII_OSD_TEST.h"
#include "TITIANII_OSD_TESTDlg.h"

#include "RendererSDK.h"
#include "BHSurfMgr.h"

#pragma comment(lib, "BHVidRenderer.lib")
#pragma comment(lib, "BHSurfMgr.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTITIANII_OSD_TESTDlg dialog

CTITIANII_OSD_TESTDlg::CTITIANII_OSD_TESTDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTITIANII_OSD_TESTDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTITIANII_OSD_TESTDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTITIANII_OSD_TESTDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTITIANII_OSD_TESTDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTITIANII_OSD_TESTDlg message handlers
VR_HANDLE			m_pVr = NULL;
void	*m_pBackBuffer = NULL;
DWORD m_dwWidth = 320;
DWORD m_dwHeight = 240;

BOOL CTITIANII_OSD_TESTDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    CVR_CreateVideoRender(&m_pVr, NULL);

	m_pBackBuffer = malloc(4*m_dwWidth* m_dwHeight);
	if(m_pBackBuffer == NULL)
		return FALSE;

	while(1)
	{
		OnBnClickedButton1();
		Sleep(100);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CTITIANII_OSD_TESTDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_TITIANII_OSD_TEST_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_TITIANII_OSD_TEST_DIALOG));
	}
}
#endif

void CTITIANII_OSD_TESTDlg::OnBnClickedButton1()
{
	HRESULT hr;
	CVR_SetOSDRegionNum(m_pVr, 1);

	VR_OSD_INFO m_osdInfo;
	ZeroMemory(&m_osdInfo,sizeof(m_osdInfo));
	m_osdInfo.src_width = m_dwWidth;
	m_osdInfo.src_height = m_dwHeight;

	m_osdInfo.dest.left  = m_osdInfo.dest.top = 0;
	m_osdInfo.dest.right = m_dwWidth;
	m_osdInfo.dest.bottom = m_dwHeight;

	m_osdInfo.alpha = 0.5;
	m_osdInfo.src_color_key  = RGB(0x5f,0x5f,0x5f);
	m_osdInfo.iRegNum = 1;
	m_osdInfo.bImgChanged = 1;

	static DWORD color = 0;
	color += 10;
	memset(m_pBackBuffer, color ,4*m_dwWidth* m_dwHeight );
	m_osdInfo.pBitmap = (unsigned char*)m_pBackBuffer;
	hr = CVR_SetOSDInfo(m_pVr, m_osdInfo.iRegNum, &m_osdInfo);
#if 0
	free(m_pBackBuffer);
	CVR_DeleteVideoRender(m_pVr);
#endif
	// TODO: Add your control notification handler code here
}
