// SH4_t1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "SH4_t1.h"
#include "SH4_t1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSH4_t1Dlg dialog
static DWORD process_start = 0;
static DWORD process_end = 0;
static DWORD render_start = 0;
static DWORD sdio_start = 0;
static DWORD idle_start = 0;
static DWORD idle_end = 0;
static DWORD frameCount = 0;

#define PresentTimerID 10
#define PresentTimer_Interval 10

TCHAR tc_path[MAX_PATH] = {0};

CSH4_t1Dlg::CSH4_t1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSH4_t1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSH4_t1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSH4_t1Dlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSH4_t1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CSH4_t1Dlg message handlers

BOOL CSH4_t1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	OnBnClickedButton1();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CSH4_t1Dlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_SH4_T1_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_SH4_T1_DIALOG));
	}
}
#endif


void CSH4_t1Dlg::OnBnClickedButton1()
{
	HMODULE h_dll;	
	if(FAILED(LoadLib(h_dll)))
		goto error;

	m_vSourceWidth = 320;
	m_vSourceHeight = 240;
	m_vDestWidth = 800;
	m_vDestHeight = 480;
	if(FAILED(Init_CreatePresenter(h_dll)))
	{
		MessageBox(_T("Can't open Video Presenter \n"), _T("Error"), MB_OK);
		goto error;
	}
	
	OpenVideo();

#ifdef _WIN32_WCE
	process_start = GetTickCount();
	idle_start = GetIdleTime();
#endif
	while(OnTimer(NULL));
	return;

error:
	//ClosePresenterAndFreeLib(m_pPresent, h_dll);
	return;
}

#include "tchar.h"

HRESULT CSH4_t1Dlg::LoadLib(HMODULE& h_dll)
{
#if 0
	LPCTSTR  szFilter( 
		TEXT("All Support(*.yuv;*.cif;*.m2v;*.264)|*.yuv;*.cif;*.m2v;*.264|")
		TEXT("All Files(*.*)|*.*|")
		TEXT("|")
		);
	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_EXPLORER, szFilter);
	if (FileDlg.DoModal() != IDOK )
	{
      		return E_FAIL;
    }
	else if (FileDlg.GetFileExt() != TEXT("yuv") 
		&& FileDlg.GetFileExt() != TEXT("YUV" )
		&& FileDlg.GetFileExt() != TEXT("cif") 
		&& FileDlg.GetFileExt() != TEXT("CIF")
		&& FileDlg.GetFileExt() != TEXT("m2v")  
		&& FileDlg.GetFileExt() != TEXT("M2V")
		&&FileDlg.GetFileExt() != TEXT("264"))
	{
		CString string = FileDlg.GetFileExt();
		MessageBox(_T("Invalid file type! Only yuv,cif,264,m2v is supported. \n"), _T("Error"), MB_OK);
		return E_FAIL;
	}
#endif
//	UpdateData(TRUE);
	
	//if(m_pPresent != NULL)
	{
	//	CloseVideo();
	}
	//Open file

	m_file = _tfopen(_T("\\Storage Card\\cmmbvideo_qvga_ref_320x240.yuv"), _T("rb"));
	if(m_file == NULL)
	{
		MessageBox(_T("Can't open input file!\n"), _T("Error"), MB_OK);
		return E_FAIL;
	}
	fseek(m_file,0,SEEK_END);
	m_FileLength = ftell(m_file);
	fseek(m_file, 0 , SEEK_SET);
	TCHAR DirPath[MAX_PATH] = {0};
	TCHAR FilePath[MAX_PATH] = {0};
	// Get Tuners folder path
	GetModuleFileName(NULL, DirPath, MAX_PATH);
	TCHAR*	p = _tcsrchr(DirPath, '\\');
	p++;
	p[0] = 0;
	_tcscpy(FilePath, DirPath);
	_tcscat(FilePath, _T("LibVRPresent.dll"));
	h_dll = LoadLibrary(FilePath); 
	if(h_dll == NULL)
	{
		MessageBox(_T("Failed to load library \n"), _T("Error"), MB_OK);
		return E_FAIL;
    }

	return S_OK;
}

HRESULT CSH4_t1Dlg::Init_CreatePresenter(HMODULE& h_dll)
{
	fpCreateVideoPresenter fpProcCreate;
	VP_OpenOptions sInit;

	fpProcCreate = (fpCreateVideoPresenter)GetProcAddress(h_dll, _T("CreateVideoPresenter"));
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

	
	ZeroMemory(&sInit, sizeof(sInit));
	sInit.dwWidth = m_vSourceWidth;
	sInit.dwHeight= m_vSourceHeight;
	sInit.dwSurfType = 0;
	sInit.dwFourCC = MAKE_FOURCC('Y','V','1','2');
	if(m_pPresent->Open(&sInit,sizeof(sInit)))
		return E_FAIL;

	memset(&m_rect_last, 0 ,sizeof(m_rect_last));
	return S_OK;
}


void CSH4_t1Dlg::OpenVideo()
{
	m_lRotationMode = VP_ROTATION_0;
	if(m_pPresent)
	{
		m_pPresent->Set(IID_IMcVideoPresenter, MC_PROPID_VP_ROTATION,0,0,&m_lRotationMode,0);
	}

	//Back Buffer Mode, then Create a new thread
	ZeroMemory(&Srcrect, sizeof(Srcrect));
	Srcrect.left = 0;
	Srcrect.right= m_vSourceWidth;
	Srcrect.top = 0;
	Srcrect.bottom = m_vSourceHeight;
	
	ZeroMemory(&Dstrect, sizeof(Dstrect));
	Dstrect.left = 0;
	Dstrect.right= m_vDestWidth;
	Dstrect.top = 0;
	Dstrect.bottom = m_vDestHeight;

	m_dwStrides[0] = m_vSourceWidth;
	m_dwStrides[1] = m_vSourceWidth>>1;
	m_dwStrides[2] = m_vSourceWidth>>1;
	
	m_pYbuf = (LPBYTE)malloc(Srcrect.right*Srcrect.bottom);
	m_pTMPbuf = (LPBYTE)malloc(Srcrect.right*Srcrect.bottom);
	m_pUbuf = (LPBYTE)malloc(Srcrect.right*Srcrect.bottom>>2);
	m_pVbuf = (LPBYTE)malloc(Srcrect.right*Srcrect.bottom>>2);
	if((!m_pYbuf )||(!m_pUbuf)||(!m_pVbuf))
	{
	    MessageBox(_T("Memory malloc failed\n"), _T("Error"), MB_OK);
	    return;
	}

	//set a timer to display
	SetTimer(PresentTimerID,PresentTimer_Interval,NULL);	

	status = ePlay;

	m_eVideoMode = VP_ZOOM_MODE_STRETCH;
	if(m_pPresent)
		m_pPresent->Set(IID_IMcVideoPresenter, MC_PROPID_VP_ZOOM,0,0,&m_eVideoMode,0);


#ifdef _WIN32_WCE
	frameCount = 0;
#endif
#ifdef _WIN32_WCE
	//m_idle.Open();
#endif

}


void CSH4_t1Dlg::CloseVideo()
{
#ifdef _WIN32_WCE
	//m_idle.Close();
#endif
	status = eStop;


	if(m_pPresent)
	{
		m_pPresent->Close();
		m_pPresent->Release();
		m_pPresent = NULL;
	}

	if(m_file)
	{
		fclose(m_file);
		m_file = NULL;
	}

	//Destroy all
	free(m_pYbuf );
	free(m_pUbuf);
	free(m_pVbuf);

	KillTimer(PresentTimerID);
}


//Set Destination Size. It should be properly set after size change.
int CSH4_t1Dlg::SetDestRect(IMcVideoPresenter* pPresent)
{
	if(pPresent == NULL)
		return -1;

	ZeroMemory(&Dstrect, sizeof(Dstrect));

	{
		Dstrect.left =0;// m_vDestStartX;
		Dstrect.right= m_vDestWidth;
		Dstrect.top = 0;//m_vDestStartY;
		Dstrect.bottom = m_vDestHeight;
	}

	if(memcmp(&m_rect_last , &Dstrect, sizeof(Dstrect)) != 0)
	{
		memcpy(&m_rect_last , &Dstrect, sizeof(Dstrect));
		pPresent->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&Dstrect,0);

	}

	return 0;
}


bool CSH4_t1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if(status == ePause) 
		return true;
	else if(status == eStop)
	{
		CloseVideo();
		DWORD cpu_val = 100;//m_idle.Measure();
		process_end = GetTickCount();
		idle_end = GetIdleTime();
		TCHAR logBuf[256];	
		TCHAR logBuf2[256];	
		_stprintf(logBuf, _T("Player duration %dms, idle %dms pure render:%f \n"), process_end-process_start, idle_end-idle_start, (float)((float)((float)idle_end-idle_start/1000)*100/(float)(((float)process_end-(float)process_start)/1000)));
		_stprintf(logBuf2, _T("CPU usage %d%% frm_rate :%f sdio: %f \n"), 	cpu_val, (float)((float)frameCount/(float)((float)((float)process_end-(float)process_start)/1000)), 
			(float)((float)((float)sdio_start/1000)*100/(float)(((float)process_end-(float)process_start)/1000)));
		_tcscat(logBuf,logBuf2);
		MessageBox(logBuf, _T("INFO"), MB_OK);
		sdio_start = render_start = 0;
		return false;
	}

	static BOOL bFirst = TRUE;
	if(bFirst  == TRUE && SetDestRect(m_pPresent))
	{
		return false;
	}
	bFirst = FALSE;
	//
	//YV12 MODE START
	//	

	//Main Loop 


	DWORD FrameSize = (Srcrect.right*Srcrect.bottom * 3)>>1;
	LPBYTE curr[3];
	static DWORD fileLeft = m_FileLength;
//	if(((fileLeft ) >= FrameSize))
	if(((m_FileLength - ftell(m_file)) >= FrameSize)&&(!feof(m_file)))
	{
		DWORD tmpTick2 = GetTickCount();

		DWORD y_stride = Srcrect.right;
		DWORD u_stride = Srcrect.right>>1;
		DWORD v_stride = Srcrect.right>>1;;
		//memcpy(m_pYbuf , m_pTMPbuf,y_stride*Srcrect.bottom );
		//memcpy(m_pUbuf , m_pVbuf,u_stride*Srcrect.bottom>>1 );
		//memcpy(m_pVbuf , m_pYbuf,v_stride*Srcrect.bottom>>1 );
		
//		fileLeft  = fileLeft - FrameSize;
		fread(m_pYbuf ,sizeof(BYTE),y_stride*Srcrect.bottom,m_file);
		fread(m_pUbuf,sizeof(BYTE),u_stride*Srcrect.bottom>>1,m_file);
		fread(m_pVbuf,sizeof(BYTE),v_stride*Srcrect.bottom>>1,m_file);

#ifdef _WIN32_WCE
		sdio_start+= (GetTickCount() - tmpTick2);
#endif
		curr[0] = &m_pYbuf [0];
		curr[1] = &m_pUbuf[0];
		curr[2] = &m_pVbuf[0];
		DWORD tmpTick = GetTickCount();
		
		
		{
			DWORD tick = GetTickCount();
			if(m_pPresent)
				m_pPresent->Present(curr, m_dwStrides, 0);
		}
#ifdef _WIN32_WCE
		render_start += (GetTickCount() - tmpTick);
		frameCount++;
#endif
	}
	// to the end of the stream
	else
	{
		status = eStop;
	}
	return true;
}

