// GfxAnimationTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GfxAnimationTest.h"
#include "GfxAnimationTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGfxAnimationTestDlg dialog




CGfxAnimationTestDlg::CGfxAnimationTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGfxAnimationTestDlg::IDD, pParent)
	, m_lineEffect(FALSE)
	,m_rotateEffect(false)
	,m_shakeEffect(false)
	,m_AlphaEffect(false)
	,m_fadeOurEffect(false)
	,m_pAnimation(NULL)
	,m_pAnimation2(NULL)
	,m_pPresenter(NULL)
	,m_bOpen(false)
	,m_hwnd(NULL)
	,m_pDiag(NULL)
	,m_pStream(NULL)
	,m_srcWidth(704)
	,m_srcHeight(576)
	,m_canvasWidth(1920)
	,m_canvasHeight(1080)
	,m_bInit(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CGfxAnimationTestDlg::~CGfxAnimationTestDlg()
{
	DestroyPresenter();
}

void CGfxAnimationTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, m_lineEffect);
}

BEGIN_MESSAGE_MAP(CGfxAnimationTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CGfxAnimationTestDlg::OnBnClickedInit)
	ON_BN_CLICKED(IDC_BUTTON1, &CGfxAnimationTestDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_BUTTON2, &CGfxAnimationTestDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_CHECK1, &CGfxAnimationTestDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CGfxAnimationTestDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK3, &CGfxAnimationTestDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK4, &CGfxAnimationTestDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK5, &CGfxAnimationTestDlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// CGfxAnimationTestDlg message handlers

BOOL CGfxAnimationTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGfxAnimationTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGfxAnimationTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGfxAnimationTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGfxAnimationTestDlg::OnBnClickedInit()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

DWORD WINAPI CGfxAnimationTestDlg::ThreadProc( LPVOID lpParameter)
{
    	CGfxAnimationTestDlg * pThis = (CGfxAnimationTestDlg*)lpParameter;
	FILE* hFile = fopen(_T("e:\\stream\\mediset794_7_704X576.YUV"), _T("rb"));
	FILE* hFile2 = fopen(_T("e:\\stream\\533.000__CTV_1_720X480.YUV"), _T("rb"));
	fseek(hFile ,0,SEEK_END);
	int FileLength = ftell(hFile);
	fseek(hFile , 0 , SEEK_SET);

	fseek(hFile2 ,0,SEEK_END);
	int FileLength2 = ftell(hFile2);
	fseek(hFile2 , 0 , SEEK_SET);

	LPBYTE pbuf = (LPBYTE	)malloc(pThis->m_srcWidth*pThis->m_srcHeight*3/2);
	if(!hFile  || !pbuf)		return -1;

	LPBYTE pBufArr[3];

	DWORD strides[3];
	while(pThis->m_threadRuning)
	{
		pBufArr[0] = pbuf;
		pBufArr[1] = pbuf + pThis->m_srcWidth* pThis->m_srcHeight;
		pBufArr[2] = pbuf+ pThis->m_srcWidth* pThis->m_srcHeight*5/4;
		strides[0] =  pThis->m_srcWidth;
		strides[1] = strides[2] = strides[0]/2;

		if(!((FileLength - ftell(hFile )) >= pThis->m_srcWidth*pThis->m_srcHeight*3/2)&&(!feof(hFile )))
			fseek(hFile , 0 , SEEK_SET);			

		if(!((FileLength2 - ftell(hFile2 )) >= 720 *480*3/2)&&(!feof(hFile2 )))
			fseek(hFile2 , 0 , SEEK_SET);			

		fread(pbuf ,sizeof(BYTE),pThis->m_srcWidth*pThis->m_srcHeight*3/2,hFile);
		pThis->m_pStream->Present(pBufArr, strides, 0);

		bool enable = true;
		pThis->m_pStream2->Get(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0,0);
		if(enable)
		{
			pBufArr[0] = pbuf;
			pBufArr[1] = pbuf + 720* 480;
			pBufArr[2] = pbuf+ 720* 480*5/4;
			strides[0] =  720;
			strides[1] = strides[2] = strides[0]/2;

			fread(pbuf ,sizeof(BYTE),720*480*3/2,hFile2);
			pThis->m_pStream2->Present(pBufArr, strides, 0);
		}
		pThis->m_pPresenter->Render(0);
	}
	delete pbuf;
	fclose(hFile);
	return 0;
}


HRESULT CGfxAnimationTestDlg::InitVideoPresenterV2()
{
	m_pDiag = new CDialog(IDD_FORMVIEW,NULL);
	m_pDiag->Create(IDD_FORMVIEW, NULL);
	m_pDiag->ShowWindow(SW_SHOWNORMAL);
	m_pDiag->MoveWindow(0, 0, 640, 480, 1);

	m_hwnd  = m_pDiag->GetSafeHwnd();

	HRESULT hr = S_OK;
	VP_OpenOptionsAdv params;
	memset(&params, 0,sizeof(params));
	params.canvasWidth = m_canvasWidth;
	params.canvasHeight= m_canvasHeight;
	params.hWnd = m_hwnd ;
	hr = m_pPresenter->Open(&params, sizeof(params));
	if(FAILED(hr))	return hr;

	VP_OPEN_STREAM_PARAMS streamParam;
	memset(&streamParam, 0,sizeof(streamParam));
	streamParam.dwWidth = m_srcWidth;
	streamParam.dwHeight = m_srcHeight;
	streamParam.fAlpha = 1.0f;
	streamParam.dwFourCC = MAKE_FOURCC('Y', 'V', '1', '2');
	hr = m_pPresenter->CreateStream(&streamParam, sizeof(streamParam), &m_pStream);
	if(FAILED(hr))	return hr;
	hr = m_pStream->Open();
	if(FAILED(hr))	return hr;

	bool enable = true;
	m_pStream->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);

	RECT rDst;
	SetRect(&rDst , m_canvasWidth/4,m_canvasHeight /4,m_canvasWidth/4*3,m_canvasHeight/4*3 );
	m_pStream->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&rDst,0);

	streamParam.dwWidth = 720;
	streamParam.dwHeight = 480;
	hr = m_pPresenter->CreateStream(&streamParam, sizeof(streamParam), &m_pStream2);
	if(FAILED(hr))	return hr;
	hr = m_pStream2->Open();
	if(FAILED(hr))	return hr;

	m_threadRuning = true;
	m_hThread = ::CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
	if(!m_hThread )	return E_FAIL;

	hr = m_pAnimation->Open(&m_pStream);
	if(FAILED(hr))	return hr;

	hr = m_pAnimation2->Open(&m_pStream2);
	if(FAILED(hr))	return hr;
	return S_OK;
}


HRESULT CGfxAnimationTestDlg::InitPresenter()
{
	if(m_bInit)	return S_OK;

	fpCreateVideoPresenterAdv pEntry = NULL;
	HMODULE h_dll  = LoadLibrary(LIB_NAME_V1);
	if(!h_dll )		return E_FAIL;
	pEntry  = (fpCreateVideoPresenterAdv)GetProcAddress(h_dll, ENTRY_FUNC_NAME_V2);
	if(!pEntry)		return E_FAIL;
	m_pPresenter = pEntry();
	if(!m_pPresenter)		return E_FAIL;

	fpCreateVideoAnimation pEntryAnimation = NULL;
	pEntryAnimation  = (fpCreateVideoAnimation)GetProcAddress(h_dll, ENTRY_ANIMATION_FUNC_NAME);
	if(!pEntryAnimation )		return E_FAIL;
	m_pAnimation = pEntryAnimation();
	if(!m_pAnimation)		return E_FAIL;

	m_pAnimation2 = pEntryAnimation();
	if(!m_pAnimation2)		return E_FAIL;

	HRESULT hr = InitVideoPresenterV2();
	if(FAILED(hr))	return hr;

	m_bInit = true;
	return S_OK;
}


HRESULT CGfxAnimationTestDlg::DestroyPresenter()
{
	if(!m_bInit)	return S_OK;

	m_threadRuning = false;
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	m_hThread = NULL;

	m_pAnimation->Stop();
	m_pAnimation->Close();
	m_pAnimation->Release();
	m_pAnimation = NULL;

	m_pAnimation2->Stop();
	m_pAnimation2->Close();
	m_pAnimation2->Release();
	m_pAnimation2 = NULL;

	m_pPresenter->Close();
	m_pPresenter->Release();
	m_pPresenter = NULL;

	delete m_pDiag;
	m_pDiag = NULL;
	m_bInit = false;
	return S_OK;

}
void CGfxAnimationTestDlg::OnBnClickedStart()
{
	HRESULT hr = S_OK;
	hr = InitPresenter();
	if(FAILED(hr))	return;


	ANIMATION_PARAMS params;
	memset(&params, 0 ,sizeof(params));

	if(m_fadeOurEffect)
	{
		static int times = 0;
		if(times %2 == 0)
		{
			params.type |= MC_ANIMATION_LINE;
			SetRect(&params.sLine.rSrc,0, 0,m_canvasWidth ,m_canvasHeight);
			SetRect(&params.sLine.rDst,m_canvasWidth  ,0,m_canvasWidth *2 ,m_canvasHeight);
			params.sLine.repeatTimes = 1;
			params.sLine.duration = 400;
			m_pAnimation->Start(&params,NULL);

			bool enable = true;
			m_pStream2->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);

			SetRect(&params.sLine.rSrc,-m_canvasWidth, 0,0,m_canvasHeight);
			SetRect(&params.sLine.rDst,0,0,m_canvasWidth ,m_canvasHeight);
			m_pAnimation2->Start(&params,NULL);

		}
		else
		{
			params.type |= MC_ANIMATION_LINE;
			SetRect(&params.sLine.rSrc,0, 0,m_canvasWidth ,m_canvasHeight);
			SetRect(&params.sLine.rDst,m_canvasWidth  ,0,m_canvasWidth *2 ,m_canvasHeight);
			params.sLine.repeatTimes = 1;
			params.sLine.duration = 400;
			m_pAnimation2->Start(&params,NULL);

			bool enable = true;
			m_pStream2->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);

			SetRect(&params.sLine.rSrc,-m_canvasWidth, 0,0,m_canvasHeight);
			SetRect(&params.sLine.rDst,0,0,m_canvasWidth ,m_canvasHeight);
			m_pAnimation->Start(&params,NULL);

		}
		times++;

		return ;
	}

	
	if(m_lineEffect)
	{
		params.type |= MC_ANIMATION_LINE;
		SetRect(&params.sLine.rSrc,0,0,4,4);
		SetRect(&params.sLine.rDst,0, 0,m_canvasWidth ,m_canvasHeight);
//		SetRect(&params.sLine.rSrc,-m_canvasWidth/2, m_canvasHeight/4 - 100,0,m_canvasHeight/4*3 - 100);
//		SetRect(&params.sLine.rDst,m_canvasWidth, m_canvasHeight/4 + 100 ,m_canvasWidth + m_canvasWidth/2  + 200,m_canvasHeight/4*3 + 100 + 200);
		params.sLine.repeatTimes = 1;
		params.sLine.duration = 1000;
	}

	if(m_shakeEffect)
	{
		params.type |= MC_ANIMATION_SHAKE;
		params.sShake.duration = 1000;
		params.sShake.Gap = 1000;
		params.sShake.repeatTimes = 10;
	}

	if(m_AlphaEffect)
	{
		params.type |= MC_ANIMATION_ALPHA;
		params.sAlpha.srcAlpha = MIN_ALPHA_VAL ;
		params.sAlpha.dstAlpha = MAX_ALPHA_VAL ;
		params.sAlpha.duration = 400;
		params.sAlpha.repeatTimes = 1;
	}

	m_pAnimation->Start(&params,NULL);

#if 0
	bool bRun = true;
	ANIMATION_PARAMS sParam;
	memset(&sParam, 0,sizeof(sParam));
	while(bRun )
	{
		m_pAnimation->IsRunning(&bRun, &sParam);
		if(sParam.sLine.state == MC_ANIMATION_STATE_FINISH)
			break;
	}
	RECT rDst;
	SetRect(&rDst , m_canvasWidth/4,m_canvasHeight /4,m_canvasWidth/4*3,m_canvasHeight/4*3 );
	m_pStream->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&rDst,0);
#endif
}

void CGfxAnimationTestDlg::OnBnClickedStop()
{
	DestroyPresenter();


}

void CGfxAnimationTestDlg::OnBnClickedCheck1()
{
	CButton *pBtn = (CButton*)GetDlgItem(IDC_CHECK1);
	if(BST_CHECKED == (pBtn->GetCheck()))
		m_lineEffect = true;
	else
		m_lineEffect = false;


	pBtn = (CButton*)GetDlgItem(IDC_CHECK2);
	if(BST_CHECKED == (pBtn->GetCheck()))
		m_rotateEffect = true;
	else
		m_rotateEffect = false;


	pBtn = (CButton*)GetDlgItem(IDC_CHECK3);
	if(BST_CHECKED == (pBtn->GetCheck()))
		m_shakeEffect= true;
	else
		m_shakeEffect= false;

	pBtn = (CButton*)GetDlgItem(IDC_CHECK4);
	if(BST_CHECKED == (pBtn->GetCheck()))
		m_AlphaEffect = true;
	else
		m_AlphaEffect = false;

	pBtn = (CButton*)GetDlgItem(IDC_CHECK5);
	if(BST_CHECKED == (pBtn->GetCheck()))
		m_fadeOurEffect= true;
	else
		m_fadeOurEffect= false;

	//OnBnClickedStart();	
	
	// TODO: Add your control notification handler code here
}
