// LibVideoMixer_TestDlg.cpp : implementation file
//


#include "stdafx.h"
#include "LibVideoMixer_Test.h"
#include "LibVideoMixer_TestDlg.h"
#include "DisplayDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <afxtempl.h>

static CArray <ICivmVideoContext*,ICivmVideoContext*> m_Array;
static CArray <CDisplayDlg*,CDisplayDlg*> m_ArrayDlg;

// CLibVideoMixer_TestDlg dialog

TCHAR file_path[MAX_PATH] = {0};
#define m_vSourceWidth 		1920
#define m_vSourceHeight 		1080

static CDisplayDlg *g_wndMixer = NULL;
static CDisplayDlg *g_CanvasDialog  = NULL;
static bool g_StreamThread_Running = true;
FILE* g_hFile= 0 ;

CLibVideoMixer_TestDlg::CLibVideoMixer_TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLibVideoMixer_TestDlg::IDD, pParent),
	m_pPresent(NULL),
	m_pVideoMixer(NULL),
	m_pCurrVideoContext(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hVideoPresenter = NULL;
	m_bRuning = false;
}

void CLibVideoMixer_TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLibVideoMixer_TestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CREATE_MIXER, &CLibVideoMixer_TestDlg::OnBnClickedCreateMixer)
	ON_BN_CLICKED(IDC_BUTTON_INITVR, &CLibVideoMixer_TestDlg::OnBnClickedInitVR)
	//ON_BN_CLICKED(IDC_BUTTON_CREATE_STREAM, &CLibVideoMixer_TestDlg::OnBnClickedCreateStream)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_CONTEXT, &CLibVideoMixer_TestDlg::OnBnClickedCreateMixerContext)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CLibVideoMixer_TestDlg::OnBnClickedConnectContext)
	ON_BN_CLICKED(IDC_BUTTON_UNINIT_VR, &CLibVideoMixer_TestDlg::OnBnClickedUninitVR)
	ON_BN_CLICKED(IDC_BUTTON_DESTROY_VM, &CLibVideoMixer_TestDlg::OnBnClickedDestroyVM)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CLibVideoMixer_TestDlg::OnBnClickedButtonDisconnect)
	//ON_BN_CLICKED(IDC_BUTTON_DESTROYSTREAM, &CLibVideoMixer_TestDlg::OnBnClickedButtonDestroystream)
	ON_BN_CLICKED(IDC_BUTTON_DESTROY_CONTEXT, &CLibVideoMixer_TestDlg::OnBnClickedButtonDestroyContext)
	ON_BN_CLICKED(IDC_BUTTON_CreateAnimation, &CLibVideoMixer_TestDlg::OnBnClickedButtonCreateanimation)
	ON_BN_CLICKED(IDC_BUTTON2, &CLibVideoMixer_TestDlg::OnBnClickedButtonStartAnimation)
	ON_BN_CLICKED(IDC_BUTTON3, &CLibVideoMixer_TestDlg::OnBnClickedButtonStopAnimation)
	ON_BN_CLICKED(IDC_BUTTON4, &CLibVideoMixer_TestDlg::OnBnClickedButtonDestroyAnimation)
END_MESSAGE_MAP()


// CLibVideoMixer_TestDlg message handlers

BOOL CLibVideoMixer_TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	GetDlgItem(IDC_BUTTON_CREATE_MIXER)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DESTROY_VM)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_INITVR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_UNINIT_VR)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CREATE_CONTEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_INITVR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UNINIT_VR)->EnableWindow(FALSE);

	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DESTROY_CONTEXT)->EnableWindow(FALSE);

	MoveWindow(800,640,1024,768);
	if(!g_wndMixer)
	{
		g_wndMixer = new CDisplayDlg(0,0);
		g_wndMixer ->Create(IDD_DIALOG1,this);
		g_wndMixer ->SetWindowText(_T("Play2"));
		g_wndMixer ->ShowWindow(SW_SHOWNOACTIVATE);
//		g_wndMixer ->ShowWindow(SW_HIDE);
		g_wndMixer ->MoveWindow(800,0,640, 480, 1);

	}

	if(FAILED(LoadLib()))
	{
		MessageBox(_T("FAILED to load librarys!\n"), _T("Error"), MB_OK);
		return -1;
	}

	if(FAILED(InitVR()))
		return -1;

	if(FAILED(InitVM()))
		return -1;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLibVideoMixer_TestDlg::OnPaint()
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
HCURSOR CLibVideoMixer_TestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CLibVideoMixer_TestDlg::LoadLib()
{
/*
	if(m_pPresent)
	{
		m_pPresent->Close();
		m_pPresent->Release();
		m_pPresent = NULL;
	}
	

	if(m_hVideoPresenter)
	{
		FreeLibrary(m_hVideoPresenter);
		m_hVideoPresenter = NULL;
	}
*/		


	TCHAR DirPath[MAX_PATH] = {0};
	TCHAR FilePath[MAX_PATH] = {0};
	// Get Tuners folder path
	GetModuleFileName(NULL, DirPath, MAX_PATH);
	TCHAR*	p = _tcsrchr(DirPath, '\\');
	p++;
	p[0] = 0;

	_stprintf(FilePath,_T("LibVRPresent.dll"));
	m_hVideoPresenter = LoadLibrary(FilePath); 
	if(m_hVideoPresenter == NULL)
	{
		MessageBox(_T("Failed to load LibVRPresent.dll \n"), _T("Error"), MB_OK);
		return E_FAIL;
    }
	
	return S_OK;
}

void CLibVideoMixer_TestDlg::OnBnClickedOpenFile()
{

	LPCTSTR  szFilter( 
		TEXT("All Support(*.yuv;*.cif;*.m2v;*.264)|*.yuv;*.cif;*.m2v;*.264|")
		TEXT("All Files(*.*)|*.*|")
		TEXT("|")
		);
	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_EXPLORER, szFilter);
	if (FileDlg.DoModal() != IDOK )
	{
      		return;
    }

	//Open file
	_tcscpy(file_path, FileDlg.GetPathName());

	if(g_hFile)
	{
		fclose(g_hFile);
		g_hFile = NULL;
	}
	
	g_hFile = _tfopen(file_path, _T("rb"));
	if(g_hFile == NULL)
	{
		MessageBox(_T("Can't open input file!\n"), _T("Error"), MB_OK);
		return;
	}
	fseek(g_hFile, 0 , SEEK_SET);

}


void CLibVideoMixer_TestDlg::OnBnClickedCreateMixer()
{
	HWND handle = g_wndMixer->GetSafeHwnd();
	RECT rClient;
	::GetClientRect(handle ,&rClient);

	HRESULT hr = m_pVideoMixer->Open(handle ,0,0,&rClient);
	if(FAILED(hr))
	{
		MessageBox(_T("Can't open Video Mixer!\n"), _T("Error"), MB_OK);
		return;
	}

	GetDlgItem(IDC_BUTTON_CREATE_CONTEXT)->EnableWindow(TRUE);

	GetDlgItem(IDC_BUTTON_CREATE_MIXER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DESTROY_VM)->EnableWindow(TRUE);

}

HRESULT CLibVideoMixer_TestDlg::InitVM()
{
	fpCreateVideoMixer fpProcCreate;
	fpProcCreate = (fpCreateVideoMixer)GetProcAddress(m_hVideoPresenter, ("CreateVideoMixer"));
	if(fpProcCreate==0)
	{
	    MessageBox(_T("Entry function is not found \n"), _T("Error"), MB_OK);
		return E_FAIL;
	}

	if(!m_pVideoMixer)
	{
		m_pVideoMixer = fpProcCreate(eMIXERTYPE_OPENGL);
	     if(m_pVideoMixer==0)
	     {
	 	    MessageBox(_T("Can't create Video Mixer!\n"), _T("Error"), MB_OK);
			return E_FAIL;
	     }
	}

	return S_OK;
}

HRESULT CLibVideoMixer_TestDlg::InitVR()
{
	fpCreateVideoPresenter fpProcCreate;
	fpProcCreate = (fpCreateVideoPresenter)GetProcAddress(m_hVideoPresenter, ("CreateVideoPresenter"));
	if(fpProcCreate==0)
	{
	    MessageBox(_T("Entry function is not found \n"), _T("Error"), MB_OK);
		return E_FAIL;
	}

    m_pPresent = fpProcCreate();
     if(m_pPresent==0)
     {
 	    MessageBox(_T("Can't create Video Presenter!\n"), _T("Error"), MB_OK);
		return E_FAIL;
     }

	VP_OpenOptions params;
	ZeroMemory(&params, sizeof(params));
	params.dwWidth = m_vSourceWidth;
	params.dwHeight = m_vSourceHeight;
	
	//params.hWnd = g_wndMixer->GetSafeHwnd();
	HRESULT hr = m_pPresent->Open(&params, sizeof(params));
	if(FAILED(hr))
		return E_FAIL;

	RECT Dstrect;
	SetRect(&Dstrect, 0,0,640,480);
	m_pPresent->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&Dstrect,0);

	return S_OK;
}

DWORD WINAPI CLibVideoMixer_TestDlg::ThreadProc( LPVOID lpParameter)
{

    CLibVideoMixer_TestDlg * pThis = (CLibVideoMixer_TestDlg*)lpParameter;
    while(pThis->m_bRuning)
  	{
		RECT rCanvas;
		if(pThis->m_pVideoMixer)
		{
			if(SUCCEEDED(pThis->m_pVideoMixer->GetViewPort(&rCanvas)))
			{
				if(g_CanvasDialog )
					g_CanvasDialog ->MoveWindow(0,0,rCanvas.right - rCanvas.left
					, rCanvas.bottom - rCanvas.top, 1);

			}
		}
			

/*

		HWND handle = g_wndMixer->GetSafeHwnd();
		RECT rClient;
		::GetClientRect(handle ,&rClient);

		POINT p={0,0};
		::ClientToScreen(handle , &p);
		OffsetRect(&rClient,p.x,p.y);

		if(pThis->m_pVideoMixer)
			pThis->m_pVideoMixer->SetViewPort(&rClient);
		
		if(pThis->m_pPresent)
		{
			HRESULT hr = pThis->m_pPresent->Render(0); 
			if(hr == VR_FAIL_CANVASCHANGED)
			{
				VP_CANVASINFO_PARAMS params = {0};
				pThis->m_pPresent->Get(IID_IMcVideoPresenter, MC_PROPID_VP_CANVAS_INFO,0,0,&params,0,0);
				if(g_CanvasDialog)
					g_CanvasDialog->MoveWindow(0,0,params.dwWidth,params.dwHeight);
			}


		}
*/		
		Sleep(40);
	}

	return 0;
}
void CLibVideoMixer_TestDlg::OnBnClickedInitVR()
{
	if(!g_CanvasDialog )
	{
		g_CanvasDialog  = new CDisplayDlg(0,m_pVideoMixer);
		g_CanvasDialog ->Create(IDD_DIALOG1,this);
	  	TCHAR pBuf[MAX_PATH];
		_stprintf(pBuf,_T("Canvas"));
		g_CanvasDialog ->SetWindowText(pBuf);
		g_CanvasDialog ->ShowWindow(SW_SHOWNORMAL);
		g_CanvasDialog ->MoveWindow(100,0,320, 240, 1);
	}

	if(m_bRuning == false)
	{
		m_bRuning = true;
		::CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
		g_StreamThread_Running =true;
		::CreateThread(NULL, 0, ThreadProcStream, m_pPresent, 0, NULL);

	}

	GetDlgItem(IDC_BUTTON_INITVR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UNINIT_VR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CREATE_MIXER)->EnableWindow(TRUE);
}

void CLibVideoMixer_TestDlg::OnBnClickedCreateMixerContext()
{
	m_pCurrVideoContext = NULL;
	m_pVideoMixer->CreateVideoContext(&m_pCurrVideoContext, 0,0);
	if(!m_pCurrVideoContext)
		MessageBox(_T("FAILED to CreateVideoContext!\n"), _T("Error"), MB_OK);

	m_pCurrVideoContext->Show(true);

	CDisplayDlg *ddlg = new CDisplayDlg(m_pCurrVideoContext,0);
	ddlg ->Create(IDD_DIALOG1,this);
	TCHAR pBuf[MAX_PATH];
	_stprintf(pBuf,_T("Context%d"),m_Array.GetCount());
	ddlg->SetWindowText(pBuf);
	ddlg->ShowWindow(SW_SHOWNORMAL);
	ddlg->MoveWindow(0,0,320, 240, 1);

	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
	m_Array.Add(m_pCurrVideoContext);
	m_ArrayDlg.Add(ddlg);
	if(m_Array.GetCount()>=1 )
	{
		GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_BUTTON_DESTROY_CONTEXT)->EnableWindow(TRUE);

}

void CLibVideoMixer_TestDlg::OnBnClickedConnectMixer()
{
	if(FAILED(m_pVideoMixer->Connect(IID_IMcVideoPresenter, m_pPresent)))
		MessageBox(_T("FAILED to Connect IID_IMcVideoPresenterAdv!\n"), _T("Error"), MB_OK);

}

void CLibVideoMixer_TestDlg::OnBnClickedConnectContext()
{
	if(FAILED(m_pCurrVideoContext ->Connect(IID_IMcVideoPresenter, m_pPresent)))
		MessageBox(_T("FAILED to Connect IID_IMcVideoPresenterStream!\n"), _T("Error"), MB_OK);

	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(TRUE);

}

void CLibVideoMixer_TestDlg::OnBnClickedUninitVR()
{
	m_bRuning = false;

	if(m_pPresent)
	{
		m_pPresent->Close();
	}
	GetDlgItem(IDC_BUTTON_INITVR)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UNINIT_VR)->EnableWindow(FALSE);
	if(g_CanvasDialog )
	{
		delete g_CanvasDialog ;
		g_CanvasDialog  = NULL;
	}
	g_StreamThread_Running = false;
}

void CLibVideoMixer_TestDlg::OnBnClickedDestroyVM()
{
	OnBnClickedButtonDestroyContext();
	if(m_pVideoMixer)
	{
		m_pVideoMixer->Close();
	}
	GetDlgItem(IDC_BUTTON_CREATE_MIXER)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DESTROY_VM)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CREATE_CONTEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);

	return;
}

DWORD WINAPI CLibVideoMixer_TestDlg::ThreadProcStream( LPVOID lpParameter)
{
	IMcVideoPresenter* pPresent = (IMcVideoPresenter*)lpParameter;

	static LPBYTE pbuf = NULL;
	if(!pbuf)
		pbuf = (LPBYTE)malloc(sizeof(BYTE)*m_vSourceWidth*m_vSourceHeight*3/2);
	memset(pbuf, 0x00, m_vSourceWidth*m_vSourceHeight*3/2);
	LPBYTE bufArrary[3];
	bufArrary[0]= pbuf;
	bufArrary[1]= pbuf+ m_vSourceWidth*m_vSourceHeight;
	bufArrary[2]= pbuf + m_vSourceWidth*m_vSourceHeight*5/4;

	DWORD dwStrides[3];
	dwStrides[0] = m_vSourceWidth;
	dwStrides[1] = dwStrides[2] = m_vSourceWidth>>1;

	while(g_StreamThread_Running )
	{
		if(g_hFile)
		{
			if(feof(g_hFile))
				fseek(g_hFile, 0 , SEEK_SET);
			fread((void*)pbuf,sizeof(BYTE),m_vSourceWidth*m_vSourceHeight*3/2,g_hFile);
		}

		//memset(pbuf, rand(), m_vSourceWidth*m_vSourceHeight*3/2);
		DWORD tick = GetTickCount();
		char buff[256];
		if(pPresent)
		{
			pPresent->Present(bufArrary, dwStrides, 0);
		}
		sprintf(buff, "present cost:%d \n",GetTickCount() - tick);
		//OutputDebugStringA(buff);
		Sleep(40);
	}
	
	return S_OK;
}

void CLibVideoMixer_TestDlg::OnBnClickedButtonDisconnect()
{
	if(FAILED(m_pCurrVideoContext ->DisConnect(IID_IMcVideoPresenter, 0)))
		MessageBox(_T("FAILED to Connect IID_IMcVideoPresenterStream!\n"), _T("Error"), MB_OK);

	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
}

void CLibVideoMixer_TestDlg::OnBnClickedButtonDestroyContext()
{
	
	int contextNumber = 0; 

	contextNumber = m_ArrayDlg.GetCount ();
	for(int i=0;i<contextNumber;i++)
	{
		CDisplayDlg* obj = (m_ArrayDlg.GetAt (i));
		if(obj)
		{
			delete obj;
		}
	}

	m_ArrayDlg.RemoveAll();
	
	contextNumber = m_Array.GetCount ();
	for(int i=0;i<contextNumber;i++)
	{
		ICivmVideoContext* obj = (m_Array.GetAt (i));
		m_pVideoMixer->DestroyVideoContext(obj);

		if(obj)
			obj->Release();
	}

	m_Array.RemoveAll();

	GetDlgItem(IDC_BUTTON_DESTROY_CONTEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	if(m_Array.GetCount()<1)
	{
		GetDlgItem(IDC_BUTTON_DISCONNECT)->EnableWindow(FALSE);
	}

}

void CLibVideoMixer_TestDlg::OnCancel()
{
	OnBnClickedDestroyVM();
	OnBnClickedUninitVR();
	if(g_wndMixer)
		delete g_wndMixer;
	CDialog::OnCancel();
}
	

void CLibVideoMixer_TestDlg::OnBnClickedButtonCreateanimation()
{
	
}

void CLibVideoMixer_TestDlg::OnBnClickedButtonStartAnimation()
{
	OnBnClickedOpenFile();

	VP_DEINTERLACE_MODE mode = VP_DEINTERLACE_BOB_SINGLE_FIELD;
	m_pPresent->Set(IID_IMcVideoPresenter,MC_PROPID_VP_DEINTERLACE_MODE,0,0,&mode,0);
	
	/*
	static VP_ZOOM_MODE mode = VP_ZOOM_MODE_STRETCH;
	if(mode == VP_ZOOM_MODE_STRETCH)
		mode = VP_ZOOM_MODE_LETTERBOX;
	else if(mode == VP_ZOOM_MODE_LETTERBOX)
		mode = VP_ZOOM_MODE_CROPPING;
	else if(mode == VP_ZOOM_MODE_CROPPING)
		mode = VP_ZOOM_MODE_STRETCH;

	if(m_pCurrVideoContext)
		m_pCurrVideoContext ->Set(MC_PROPID_VP_ZOOM,&mode,sizeof(mode));

*/		
/*
	memset(&m_animation_parm, 0,sizeof(m_animation_parm));

	VR_ANIMATION_ROTATE_PARAMS params;
	params.start_angle = 0.0f;
	params.stop_angle = 360.0f;
	params.angle_step = 1.0f;
	params.xFactor= 1.0f;
	params.yFactor= 0.0f;
	params.zFactor= 1.0f;
	m_animation_parm.pParms = &params;
	m_animation_parm.times = 5;
	m_animation_parm.type = eVR_ANIMATION_ROTATE;
*/
	/*if(m_pCurrStream)
	{
		m_pCurrStream->StartAnimation(&m_animation_parm);
		
	}*/


}

void CLibVideoMixer_TestDlg::OnBnClickedButtonStopAnimation()
{
	static VP_ZOOM_MODE mode = VP_ZOOM_MODE_STRETCH;
	if(mode == VP_ZOOM_MODE_STRETCH)
		mode = VP_ZOOM_MODE_LETTERBOX;
	else if(mode == VP_ZOOM_MODE_LETTERBOX)
		mode = VP_ZOOM_MODE_CROPPING;
	else if(mode == VP_ZOOM_MODE_CROPPING)
		mode = VP_ZOOM_MODE_STRETCH;

	if(m_pCurrVideoContext)
		m_pCurrVideoContext ->Set(MC_PROPID_VP_ZOOM,&mode,sizeof(mode));


	/*if(m_pCurrStream)
	{
		m_pCurrStream->StopAnimation();
	}*/
}

void CLibVideoMixer_TestDlg::OnBnClickedButtonDestroyAnimation()
{
	// TODO: Add your control notification handler code here
}
