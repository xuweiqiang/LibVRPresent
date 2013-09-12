// LibVrPresent_TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LineQuality_Test.h"
#include "LineQuality_TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PRESENT_OVERLAY

// CLibVrPresent_TestDlg dialog


#define VIDEO_SURFACE	VP_SURF_LAYERBOTTOM
#define TTX_SURFACE		VP_SURF_LAYER3
#define SUBTITLE_SURFACE	VP_SURF_LAYER2

CLibVrPresent_TestDlg::CLibVrPresent_TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLibVrPresent_TestDlg::IDD, pParent)
	, m_vSourceWidth(480)
	, m_vSourceHeight(272)
	, m_vClipStartX(0)
	, m_vClipStartY(0)
	, m_vClipWidth(480)
	, m_vClipHeight(272)
	, m_vDestWdith(480)
	, m_vDestHeight(272)
	, m_vBlackLineWidth(1)
	, m_vWhiteLineWidth(1)
	, bFullScreen(FALSE)
	, procCreate(NULL)
	, etype(eOverlay)

{
	m_pPresent = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hDll = 0;
}

CLibVrPresent_TestDlg::~CLibVrPresent_TestDlg()
{
	ReleasePresent();
	if(m_hDll)
	{
		FreeLibrary(m_hDll);
		m_hDll = 0;
	}
}

void CLibVrPresent_TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SourceWidth, m_vSourceWidth);
	DDV_MinMaxUInt(pDX, m_vSourceWidth, 0, 2000);
	DDX_Text(pDX, IDC_EDIT_SourceHeight, m_vSourceHeight);
	DDV_MinMaxUInt(pDX, m_vSourceHeight, 0, 2000);
	DDX_Text(pDX, IDC_EDIT_ClipStartx, m_vClipStartX);
	DDV_MinMaxUInt(pDX, m_vClipStartX, 0, 2000);
	DDX_Text(pDX, IDC_EDIT_ClipStartY, m_vClipStartY);
	DDV_MinMaxUInt(pDX, m_vClipStartY, 0, 2000);
	DDX_Text(pDX, IDC_EDIT_Clip_Width, m_vClipWidth);
	DDV_MinMaxUInt(pDX, m_vClipWidth, 0, 2000);
	DDX_Text(pDX, IDC_EDIT_ClipHeight, m_vClipHeight);
	DDV_MinMaxUInt(pDX, m_vClipHeight, 0, 2000);
	DDX_Text(pDX, IDC_EDIT_DestWidth, m_vDestWdith);
	DDV_MinMaxUInt(pDX, m_vDestWdith, 0, 2000);
	DDX_Text(pDX, IDC_EDIT_DestHeight, m_vDestHeight);
	DDV_MinMaxUInt(pDX, m_vDestHeight, 0, 2000);
	DDX_Text(pDX, IDC_EDIT_BlackLine, m_vBlackLineWidth);
	DDV_MinMaxUInt(pDX, m_vBlackLineWidth, 1, 1000);
	DDX_Text(pDX, IDC_EDITWhiteLine, m_vWhiteLineWidth);
	DDV_MinMaxUInt(pDX, m_vWhiteLineWidth, 1, 1000);
}

BEGIN_MESSAGE_MAP(CLibVrPresent_TestDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_HorizontalLine, &CLibVrPresent_TestDlg::OnBnClickedButtonHorizontalline)
	ON_BN_CLICKED(IDC_BUTTON_diagonal, &CLibVrPresent_TestDlg::OnBnClickedButtondiagonal)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_VerticalLine, &CLibVrPresent_TestDlg::OnBnClickedButtonVerticalline)
END_MESSAGE_MAP()

void CLibVrPresent_TestDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleasePresent();
}
// CLibVrPresent_TestDlg message handlers

BOOL CLibVrPresent_TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CLibVrPresent_TestDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_LIBVRPRESENT_TEST_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_LIBVRPRESENT_TEST_DIALOG));
	}
}
#endif

int CLibVrPresent_TestDlg::SetScreenRect(RECT* pSrc, IMcVideoPresenter* pPresent)
{
	int ret= 0;
	if((pSrc == NULL)||(pPresent == NULL))
		return -1;

	memset(&vbi,0,sizeof(vbi));
	vbi.dwWidth[0] = pSrc->right;
	vbi.dwHeight[0] = pSrc->bottom;
	vbi.dwStride[0] =pSrc->right;

	vbi.dwWidth[1] = (pSrc->right)>>1;
	vbi.dwHeight[1] = (pSrc->bottom)>>1;
	vbi.dwStride[1] = (pSrc->right)>>1;

	vbi.dwWidth[2] = (pSrc->right)>>1;
	vbi.dwHeight[2] = (pSrc->bottom)>>1;
	vbi.dwStride[2] = (pSrc->right)>>1;

	ret = pPresent->SetVBI(&vbi);
	if(FAILED(ret))
		return ret;

	ret = pPresent->SetSurface(VIDEO_SURFACE,IID_IMcVideoPresenter,MC_PROPID_VP_RECTSRC,0,0,pSrc,0);
	if(FAILED(ret))
		return ret;

	return 0;
}

int CLibVrPresent_TestDlg::SetDestRect(RECT* pDst,IMcVideoPresenter* pPresent)
{
	int ret= 0;
	if(pDst == NULL)
		return -1;
	pPresent->SetSurface(VP_SURF_LAYERTOP, IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,pDst,0);
	return 0;
}

int CLibVrPresent_TestDlg::ReleasePresent()
{
	if(m_pPresent)
	{
		int ret;

		ret = m_pPresent->Close();
		m_pPresent->Release();
		m_pPresent = 0;
		if(FAILED(ret))
		{
			MessageBox(_T("Failed to Destroy  Video Presenter\n"), _T("Error"), MB_OK);
			return -1;
		}
	}
	return 0;
}

int CLibVrPresent_TestDlg::ShowSurface(int type)
{
	RECT Srcrect, Cliprect, Dstrect;
	LPBYTE pYbuf, pUbuf, pVbuf;

	if(LoadDLL()<0)
		return -1;
	UpdateData(TRUE);
	ZeroMemory(&Cliprect, sizeof(Cliprect));
	Cliprect.left = m_vClipStartX;
	Cliprect.right= m_vClipStartX+m_vClipWidth;
	Cliprect.top = m_vClipStartY;
	Cliprect.bottom = m_vClipStartY + m_vClipHeight;
	if(SetScreenRect(&Cliprect, m_pPresent))
	{
		MessageBox(_T("Invalid source size \n"), _T("Error"), MB_OK);
		return -1;
	}
	ZeroMemory(&Dstrect, sizeof(Dstrect));
	Dstrect.left = 0;
	Dstrect.right= m_vDestWdith;
	Dstrect.top = 0;
	Dstrect.bottom = m_vDestHeight;
	if(SetDestRect(&Dstrect, m_pPresent))
	{
		MessageBox(_T("Invalid dst size \n"), _T("Error"), MB_OK);
		return -1;
	}
	ZeroMemory(&Srcrect, sizeof(Srcrect));
	Srcrect.left = 0;
	Srcrect.right= m_vSourceWidth;
	Srcrect.top = 0;
	Srcrect.bottom = m_vSourceHeight;
	pYbuf = (LPBYTE)malloc(Srcrect.right*Srcrect.bottom);
	pUbuf = (LPBYTE)malloc(Srcrect.right*Srcrect.bottom>>2);
	pVbuf = (LPBYTE)malloc(Srcrect.right*Srcrect.bottom>>2);
	if(pYbuf==0 || pUbuf==0 || pVbuf==0)
	{
		MessageBox(_T("Memory malloc failed\n"), _T("Error"), MB_OK);
		if(pYbuf)
			free(pYbuf);
		if(pUbuf)
			free(pUbuf);
		return -1;
	}
	int i;
	int matrix_sum = m_vBlackLineWidth+ m_vWhiteLineWidth;
	if(type==0)
	{	// vertical
		for(i=0; i<(Srcrect.right*Srcrect.bottom); i++)
		{
			if((i%m_vSourceWidth%matrix_sum)<m_vBlackLineWidth)
				pYbuf[i] = 0; 		//Draw black
			else
				pYbuf[i] = 255;		//Draw white 
		}
	}
	else if(type==1)
	{	// diagonal
		for(int i =0;i<(Srcrect.right*Srcrect.bottom);i++)
		{
			if(((i+(i/m_vSourceWidth))%m_vSourceWidth%matrix_sum)<m_vBlackLineWidth)
				pYbuf[i] = 0; 		//Draw black
			else
				pYbuf[i] = 255;		//Draw white 
		}
	}
	else if(type==2)
	{	// horizontal
		for(int i =0;i<(Srcrect.right*Srcrect.bottom);i++)
		{
			if((i/m_vSourceWidth%matrix_sum)<m_vBlackLineWidth)
				pYbuf[i] = 0; 		//Draw black
			else
				pYbuf[i] = 255;		//Draw white 
		}
	}
	memset((pUbuf),128,(sizeof(BYTE far)*(Srcrect.right*Srcrect.bottom>>2)));
	memset((pVbuf),128,(sizeof(BYTE far)*(Srcrect.right*Srcrect.bottom>>2)));

	int frame_count = 0;
	DWORD dCounterStart = GetTickCount();
	DWORD dNoIOCounter = 0;
	DWORD dNoIOCounter_tmp = 0;
	 {

		LPBYTE curr[3];
		curr[0] = &pYbuf[0];
		curr[1] = &pUbuf[0];
		curr[2] = &pVbuf[0];

		dNoIOCounter_tmp = GetTickCount();
		m_pPresent->Present(VP_SURF_LAYERTOP, curr,vbi.dwStride,0);
		dNoIOCounter += (GetTickCount() - dNoIOCounter_tmp);
		frame_count++;
	}
	free(pYbuf);
	free(pUbuf);
	free(pVbuf);
	bFullScreen = TRUE;
	return 0;
}

void CLibVrPresent_TestDlg::OnBnClickedButtonHorizontalline()
{
	ShowSurface(2);
}

void CLibVrPresent_TestDlg::OnBnClickedButtondiagonal()
{
	ShowSurface(1);
}

void CLibVrPresent_TestDlg::OnBnClickedButtonVerticalline()
{
	ShowSurface(0);
}

int CLibVrPresent_TestDlg::LoadDLL()
{
	if(m_pPresent)
		return 0;
	if(m_hDll==0)
	{
		TCHAR DirPath[MAX_PATH] = {0};
		TCHAR FilePath[MAX_PATH] = {0};
		TCHAR*	p;
	
		GetModuleFileName(NULL, DirPath, MAX_PATH);
		p = _tcsrchr(DirPath, '\\');
		p[1] = 0;	// terminate
		_tcscpy(FilePath, DirPath);
		_tcscat(FilePath, _T("LibVRPresent.dll"));
		m_hDll = LoadLibrary(FilePath); 
		if(m_hDll==0)
		{
			MessageBox(_T("Failed to load library \n"), _T("Error"), MB_OK);
			return - 1;
		}
	}
	if(etype==eOverlay)
		procCreate = (fpCreateVideoPresenter)GetProcAddress(m_hDll, _T("CreateVideoPresenterOvl"));
	else if(etype== eRGB)
	 	procCreate = (fpCreateVideoPresenter)GetProcAddress(m_hDll, _T("CreateVideoPresenterRGB"));
	else
		procCreate = (fpCreateVideoPresenter)GetProcAddress(m_hDll, _T("CreateVideoPresenter"));
	m_pPresent = procCreate();
	if(m_pPresent==0)
	{
		MessageBox(_T("Can't create Video Presenter!\n"), _T("Error"), MB_OK);
		return -1;
	}
	
	VP_OpenOptions sInit;
	ZeroMemory(&sInit, sizeof(sInit));
	sInit.dwWidth = m_vDestWdith;
	sInit.dwHeight= m_vDestHeight;
		
	if(m_pPresent->Open(&sInit,sizeof(sInit)))
	{
		m_pPresent->Close();
		m_pPresent->Release();
		m_pPresent = 0;
		MessageBox(_T("Can't open Video Presenter \n"), _T("Error"), MB_OK);
		return -1;
	}
	return 0;
}

