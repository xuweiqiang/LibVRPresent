// LibVrPresent_TestDlg.cpp : implementation file
//
#include "stdafx.h"
#include "LibVrPresent_Test.h"
#include "LibVrPresent_TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef PresentTimerID
#define PresentTimerID 10
#define PresentTimer_Interval 10
#endif

// CLibVrPresent_TestDlg dialog
//#define STRIDE_TEST
//#define OMX_TEST
//#define NO_FEED_DATA

#ifdef _WIN32_WCE
#define _tfopen		_wfopen
#else	/* not _UNICODE */
#define _tfopen		fopen
#endif

#ifdef _WIN32_WCE
static DWORD process_start = 0;
static DWORD process_end = 0;
static DWORD render_start = 0;
static DWORD sdio_start = 0;
static DWORD idle_start = 0;
static DWORD idle_end = 0;
static DWORD frameCount = 0;
#endif

#define VIDEO_SURFACE	VP_SURF_LAYERBOTTOM
#define TTX_SURFACE		VP_SURF_LAYER3
#define SUBTITLE_SURFACE	VP_SURF_LAYER2

#define FPS_CT 25
//#define DEMO_PIP

#ifndef _WIN32_WCE
//#define USE_NEW_INTERFACE
#endif

#ifdef OSD_TEST
#define OSD_SRC_WIDTH	320
#define OSD_SRC_HEIGHT  40
#define OSD_DST_X  0
#define OSD_DST_Y  200
#define OSD_DST_W 480
#define OSD_DST_H  70
#endif


RECT CLibVrPresent_TestDlg::m_rect_last = {0,0,0,0};
TCHAR tc_path[MAX_PATH] = {0};

CLibVrPresent_TestDlg::CLibVrPresent_TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLibVrPresent_TestDlg::IDD, pParent)
	, m_vSourceWidth(720)
	, m_vSourceHeight(480)
	, m_vClipStartX(0)
	, m_vClipStartY(0)
	, m_vClipWidth(m_vSourceWidth)
	, m_vClipHeight(m_vSourceHeight)
	, m_vDestStartX(0)
	, m_vDestStartY(0)
	, m_vDestWidth(320)
	, m_vDestHeight(240)
	, etype(eAuto)
	, status(eStop)
	, m_pPresent(NULL)
	, m_PresentObjectBuffer(NULL)
	, m_FreePresentObject(NULL)
	, m_PresentObject(NULL)
	, m_hBBPresentSema(NULL)
	, m_dwBuffers(0)
	, m_hBBPresentThread(NULL)
	, m_fStopPresentThread(FALSE)
	, m_vBackBufferNumber(12)
	, m_vBackBufferEnable(0)
	, m_file(NULL)
	, m_bReplay(true)
	, m_isFullScr(FALSE)
	, m_FileLength(0)
	, setPage(NULL)
	,m_lRotationMode(VP_ROTATION_0)
	, m_eVideoMode(VP_ZOOM_MODE_STRETCH)
	, bOsd1Enabled(FALSE)
	, bOsd2Enabled(FALSE)
	,m_pPresentAdv(NULL)
	,m_pMainStream(NULL)
	,m_pSubStream1(NULL)
	,m_pSubStream2(NULL)
	,m_pSubStream3(NULL)
	,m_pSubStream4(NULL)
	,m_pSubStream5(NULL)
	,m_pSubStream6(NULL)
	,m_pAnimation(NULL)
{
	m_bCKey = FALSE;
	m_dwCKey = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_wnd = 0;
#ifdef OSD_TEST
    m_pPresent_OSD = NULL;
#endif

#ifdef STRIDE_TEST
	m_vSourceWidth = 320;
	m_vSourceHeight = 180;
#endif
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
	DDX_Text(pDX, IDC_EDIT_DestWidth, m_vDestWidth);
	DDV_MinMaxUInt(pDX, m_vDestWidth, 0, 2000);
	DDX_Text(pDX, IDC_EDIT_DestHeight, m_vDestHeight);
	DDV_MinMaxUInt(pDX, m_vDestHeight, 0, 2000);
	DDX_Text(pDX, IDC_EDIT_BB_NUMBER, m_vBackBufferNumber);
	DDV_MinMaxUInt(pDX, m_vBackBufferNumber, 0, 20);
	DDX_Check(pDX, IDC_CHECK_BB_ENABLE, m_vBackBufferEnable);
	DDX_Text(pDX, IDC_EDIT_Dest_StartX, m_vDestStartX);
	//DDV_MinMaxUInt(pDX, m_vDestStartX, 0, 2000);
	DDX_Text(pDX, IDC_EDIT_Dest_StartY, m_vDestStartY);
	//DDV_MinMaxUInt(pDX, m_vDestStartY, 0, 2000);
	DDX_Control(pDX, IDC_EDIT_SourceWidth, m_SrcW);
	DDX_Control(pDX, IDC_EDIT_SourceHeight, m_SrcH);
	DDX_Control(pDX, IDC_EDIT_Dest_StartX, m_DstX);
	DDX_Control(pDX, IDC_EDIT_Dest_StartY, m_DstY);
	DDX_Control(pDX, IDC_EDIT_DestWidth, m_DstW);
	DDX_Control(pDX, IDC_EDIT_DestHeight, m_DstH);
	DDX_Check(pDX, IDC_CHECK_REPLAY, m_bReplay);
	DDX_Control(pDX, IDC_BUTTON_PAUSE, m_PausePlay);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_Stop);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}

BEGIN_MESSAGE_MAP(CLibVrPresent_TestDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CLibVrPresent_TestDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_CHECK_BB_ENABLE, &CLibVrPresent_TestDlg::OnBnClickedCheckBbEnable)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CLibVrPresent_TestDlg::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CLibVrPresent_TestDlg::OnBnClickedButtonStop)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CLibVrPresent_TestDlg::OnTcnSelchangeTab1)
	ON_WM_MOVE()
	// ON_BN_CLICKED
	ON_BN_CLICKED(IDC_RADIO_LETTER_BOX, &CLibVrPresent_TestDlg::OnBnClickedRadioLetterBox)
	ON_BN_CLICKED(IDC_RADIO_CROPPING,	&CLibVrPresent_TestDlg::OnBnClickedRadioCropping)
	ON_BN_CLICKED(IDC_RADIO_STRETCH,	&CLibVrPresent_TestDlg::OnBnClickedRadioStretch)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE, &CLibVrPresent_TestDlg::OnBnClickedButtonRotate)
	ON_BN_CLICKED(IDC_BUTTON_OSD1, &CLibVrPresent_TestDlg::OnBnClickedButtonOsd1)
	ON_BN_CLICKED(IDC_BUTTON_OSD2, &CLibVrPresent_TestDlg::OnBnClickedButtonOsd2)
END_MESSAGE_MAP()


// CLibVrPresent_TestDlg message handlers
void CLibVrPresent_TestDlg::SetPlayWnd()
{
	RECT rect;
	GetWindowRect(&rect);
	// Set Play Window Postion
#ifdef CREATE_WINDOW_BY_APP
#ifdef ST_DEMO
	ddlg->MoveWindow(0, 0, 800, 480, 1);
#else
#ifdef _WIN32_WCE
	ddlg->MoveWindow(0, 0, 320, 240, 1);
#else
	ddlg->MoveWindow(0,0,m_vDestWidth, m_vDestHeight, 1);
#endif
#endif
//	m_vDestWidth = m_vSourceWidth;
	//m_vDestHeight = m_vSourceHeight;
#endif
	UpdateData(FALSE);
}

BOOL CLibVrPresent_TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_PausePlay.EnableWindow(FALSE);
	m_Stop.EnableWindow(FALSE);
#ifdef CREATE_WINDOW_BY_APP
	m_DstX.EnableWindow(FALSE);
	m_DstY.EnableWindow(FALSE);
	m_DstW.EnableWindow(FALSE);
	m_DstH.EnableWindow(FALSE);
#endif

	GetDlgItem(IDC_RADIO_CROPPING)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_LETTER_BOX)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_STRETCH)->EnableWindow(FALSE);

	//TODO: Add extra initialization here
	// Set Main Window Postion
	//MoveWindow(425, 380, 275, 250, 1);

#ifdef CREATE_WINDOW_BY_APP
	//Generate Play Window
	ddlg = new CDisplayDlg();
	ddlg->Create(IDD_DIALOG_DISPLAY,this);
	ddlg->SetWindowText(_T("Play"));
	SetPlayWnd();
	m_wnd = ddlg->GetSafeHwnd();
#endif

	//Generate VideoEffect Tab window
	m_Tab.InsertItem(0, _T("Info"));
	m_Tab.InsertItem(1, _T("VideoEffect"));
#ifdef SET_PAGE
	setPage = new CSetPage();
	if(setPage)
		setPage->Create(IDD_DIALOG_SETTINGS, (CWnd*)GetDlgItem(IDC_TAB1));
#endif		
	CheckDlgButton(IDC_RADIO_STRETCH,BST_CHECKED);

	m_pYbuf = m_pUbuf = m_pVbuf = 0;
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

int CLibVrPresent_TestDlg::SetClipRect(IMcVideoPresenter* pPresent )
{

/***************************Please call SETVBI() while source size change ********************************************/
#if 0
	//FOR YV12
	memset(&vbi,0,sizeof(vbi));
	vbi.dwWidth[0] = m_vSourceWidth;
	vbi.dwHeight[0] = m_vSourceHeight;
	vbi.dwStride[0] =m_vSourceWidth;

	vbi.dwWidth[1] = (m_vSourceWidth)>>1;
	vbi.dwHeight[1] = (m_vSourceHeight)>>1;
	vbi.dwStride[1] = (m_vSourceWidth)>>1;

	vbi.dwWidth[2] = (m_vSourceWidth)>>1;
	vbi.dwHeight[2] = (m_vSourceHeight)>>1;
	vbi.dwStride[2] = (m_vSourceWidth)>>1;

	ret = pPresent->SetVBI(&vbi);
	if(FAILED(ret))
		return ret;
#endif
	#ifdef STRIDE_TEST
	m_dwStrides[0] = 384;
	m_dwStrides[1] = m_dwStrides[2] = 176;
	#else
	m_dwStrides[0] = m_vSourceWidth;
	m_dwStrides[1] = m_dwStrides[2] = m_vSourceWidth>>1;
	#endif
/***************************Please call SETVBI() while source size change end********************************************/

//ZOOM MODE is enabled by default. 
//1. Zoom mode enabled. RECTSRC isn't working. You can Set(ZOOM_MODE) to switch between STRECTCH/LETTERBOX/CROPPING
//2. Zoom mode disabled. RECTSRC works. You can clip a rect in source rect.
//It's not a must to set source rectangle if there's no source size change.
#if 0
	//set Cliprect
	ZeroMemory(&Cliprect, sizeof(Cliprect));
	Cliprect.left = m_vClipStartX;
	Cliprect.right= m_vClipStartX+m_vClipWidth;
	Cliprect.top = m_vClipStartY;
	Cliprect.bottom = (m_vClipStartY + m_vClipHeight);	
	pPresent->SetSurface(VIDEO_SURFACE,IID_IMcVideoPresenter,MC_PROPID_VP_RECTSRC,0,0,&Cliprect,0);
#endif

	return 0;
}

//Set Destination Size. It should be properly set after size change.
#ifndef TEST_V2_SURF
int CLibVrPresent_TestDlg::SetDestRect(IMcVideoPresenter* pPresent)
#else
int CLibVrPresent_TestDlg::SetDestRect(IMcVideoPresenterAdv* pPresent)
#endif
{
	if(pPresent == NULL)
		return -1;

	ZeroMemory(&Dstrect, sizeof(Dstrect));

	if(m_wnd)
	{
		POINT p={0,0};

		Dstrect.left = 0;
		Dstrect.right=m_vDestWidth;
		Dstrect.top = 0;
		Dstrect.bottom = + m_vDestHeight;
		
	}
	else
	{
		Dstrect.left = m_vDestStartX;
		Dstrect.right= Dstrect.left + m_vDestWidth;
		Dstrect.top = m_vDestStartY;
		Dstrect.bottom = Dstrect.top + m_vDestHeight;
	}

	if(memcmp(&m_rect_last , &Dstrect, sizeof(Dstrect)) != 0)
	{
		memcpy(&m_rect_last , &Dstrect, sizeof(Dstrect));

#ifdef USE_NEW_INTERFACE

		pPresent->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&Dstrect,0);
		RECT videosurf_rect;
		memset(&videosurf_rect, 0 ,sizeof(videosurf_rect));
		videosurf_rect.right =  m_vSourceWidth;
		videosurf_rect.bottom =  m_vSourceHeight;
		pPresent->SetSurface(VIDEO_SURFACE,IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&videosurf_rect,0);

	#ifdef DEMO_PIP
			videosurf_rect.left =  m_vSourceWidth/2;
			videosurf_rect.bottom =  m_vSourceHeight/2;
			m_pPresent->SetSurface(SUBTITLE_SURFACE,IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&videosurf_rect,0);
			videosurf_rect.left =  m_vSourceWidth/3;
			videosurf_rect.right =  m_vSourceWidth/3+m_vSourceWidth/2;
			videosurf_rect.top=  m_vSourceHeight/3;
			videosurf_rect.bottom =  m_vSourceHeight/3+ m_vSourceHeight/2;
			m_pPresent->SetSurface(TTX_SURFACE,IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&videosurf_rect,0);
	#endif
#else
		bool enable = true;
		pPresent->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);

		pPresent->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&Dstrect,0);

#ifdef OSD_TEST
		RECT dstOSD;
		SetRect(&dstOSD, OSD_DST_X, OSD_DST_Y, OSD_DST_X+OSD_DST_W, OSD_DST_Y + OSD_DST_H);
		if(m_pPresent_OSD->Set(IID_IMcVideoPresenter, MC_PROPID_VP_RECTDST,0,0,&dstOSD,0))
			return E_FAIL;
#endif	

#endif
	}

	return 0;
}

//It's ony necessary to set dest color key in DDRAW mode. If not, other windows will not be able to overwrite it.
//After Destination color key is set. Please draw a pure color as the background of video. If not, video will be invisable.
int CLibVrPresent_TestDlg::SetDestColorKey(IMcVideoPresenter* pPresent)
{
	HRESULT hr = S_OK;

	if(pPresent == NULL)
		return E_FAIL;

	m_dwCKey = RGB(0,0,1);
	hr = pPresent->Set(IID_IMcVideoPresenter,MC_PROPID_VP_COLORKEYDST,0,0,&m_dwCKey,0);
	m_bCKey = SUCCEEDED(hr);
	return hr;
}

void PushData(LPBYTE psrc, DWORD dwwStride, DWORD dwHStride,FILE* m_file)
{
	if(m_file==NULL)
		return;
	LPBYTE pdst = psrc +dwwStride;

	for(DWORD i =0;i< dwHStride;i++) 
	{
		fread(psrc,sizeof(BYTE),dwwStride,m_file);
		memcpy(pdst, psrc,dwwStride);
		psrc += 2*dwwStride;
		pdst = psrc +dwwStride;
	}
	return;
}

void CLibVrPresent_TestDlg::InitPresentObject(DWORD Number)
{
    InitializeCriticalSection(&m_csObjects);
    m_hBBPresentSema = CreateSemaphore(NULL, 0, Number, NULL);

    PresentBufferInfo_T* Objects = new PresentBufferInfo_T[Number];
    m_PresentObjectBuffer = m_FreePresentObject = Objects;
    int i = 0;
    while(1)
    {
        memset(Objects, 0, sizeof(PresentBufferInfo_T));
        Objects->FrameNumber = i;
        if(++i == Number)
        {
            Objects->Next = NULL;
            break;
        }
        Objects->Next = Objects+1;
        Objects ++;
    }
}

PresentBufferInfo_T* CLibVrPresent_TestDlg::GetPresentObject()
{
    if(!m_PresentObject)
        return NULL;
    EnterCriticalSection(&m_csObjects);
    PresentBufferInfo_T *ret = NULL;
    if(m_PresentObject)
    {
        ret = m_PresentObject;
        m_PresentObject = m_PresentObject->Next;
    }
    LeaveCriticalSection(&m_csObjects);
    return ret;
}
void CLibVrPresent_TestDlg::PutPresentObject(PresentBufferInfo_T* Object)
{
    EnterCriticalSection(&m_csObjects);
    Object->Next = NULL;

    if(!m_PresentObject)
    {
        m_PresentObject = Object;
    }
    else
    {
        PresentBufferInfo_T *Tail = m_PresentObject; 
        while(Tail->Next)
        {
            Tail = Tail->Next;
        }
        Tail->Next = Object;
    }
    LeaveCriticalSection(&m_csObjects);
}

PresentBufferInfo_T* CLibVrPresent_TestDlg::GetFreePresentObject()
{    
    if(!m_FreePresentObject)
        return NULL;
    EnterCriticalSection(&m_csObjects);
    PresentBufferInfo_T *ret = NULL;
    if(m_FreePresentObject)
    {
        ret = m_FreePresentObject;
        m_FreePresentObject = m_FreePresentObject->Next;
    }
    LeaveCriticalSection(&m_csObjects);
    return ret;
}

void CLibVrPresent_TestDlg::PutFreePresentObject(PresentBufferInfo_T* Object)
{    
    EnterCriticalSection(&m_csObjects);
    Object->Next = m_FreePresentObject;
    m_FreePresentObject = Object;
    LeaveCriticalSection(&m_csObjects);
}

BOOL CLibVrPresent_TestDlg::WaitPresentObjectEmpty(DWORD ms)
{
    //ms, wait time in millisecond 
    while(m_PresentObject && ms)
    {
        (ms > 10) ? (ms -= 10) : (ms = 0);
        Sleep(10);
    }
    return m_PresentObject ? FALSE : TRUE;
}
void CLibVrPresent_TestDlg::DeinitPresentObject(void)
{
    DeleteCriticalSection(&m_csObjects);
    CloseHandle(m_hBBPresentSema);
    m_hBBPresentSema = NULL;
    m_FreePresentObject = NULL;
    m_PresentObject = NULL;
    delete [] m_PresentObjectBuffer;
    m_PresentObjectBuffer = NULL;
}

DWORD WINAPI CLibVrPresent_TestDlg::BBPresentProc( LPVOID lpParameter)
{
    CLibVrPresent_TestDlg* pThis = (CLibVrPresent_TestDlg*)lpParameter;
	DWORD lasttick = GetTickCount();
	DWORD tmpTick = 0;
    while(1)
    {
        if(WaitForSingleObject(pThis->m_hBBPresentSema, 100) != WAIT_OBJECT_0)
        {
            if(pThis->m_fStopPresentThread)
            {
                break;
            }
            else
            {
                continue;
            }
        }
        PresentBufferInfo_T* PresentObject = pThis->GetPresentObject();

        if(PresentObject->Vaild)
        {
#ifdef USE_NEW_INTERFACE
            pThis->m_pPresent->PresentBuffer(VIDEO_SURFACE	,PresentObject->FrameNumber, 0);
#else
            pThis->m_pPresent->PresentBuffer(PresentObject->FrameNumber, 0);
#endif
        }
	pThis->PutFreePresentObject(PresentObject);

	//Keep framerate to be 25fps
	tmpTick = GetTickCount() - lasttick;
	if(tmpTick < 40)
		Sleep(40 - tmpTick);
	lasttick = GetTickCount();

	}
    return 0;
}

HRESULT CLibVrPresent_TestDlg::LoadLib(HMODULE& h_dll)
{
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

	UpdateData(TRUE);
	
	if(m_pPresent != NULL)
	{
		CloseVideo();
	}
	//Open file
	_tcscpy(tc_path, FileDlg.GetPathName());
	
	m_file = _tfopen(tc_path, _T("rb"));
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

HRESULT CLibVrPresent_TestDlg::Init_CreatePresenterProbe(HMODULE& h_dll, eRUNTYPE surface_mode )
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


	fpCreateVideoAnimation fpCreateAni;
	fpCreateAni = (fpCreateVideoAnimation)GetProcAddress(h_dll, _T("CreateVideoAnimation"));
	if(fpCreateAni)
	{
		m_pAnimation = fpCreateAni();
	}
	
		
	HRESULT hr = S_OK;
	hr = TestV2(h_dll);
	if(SUCCEEDED(hr))	return hr;
	
#ifdef OSD_TEST
	m_pPresent_OSD = fpProcCreate();
     if(m_pPresent_OSD==0)
     {
 	    MessageBox(_T("Can't create osd Video Presenter!\n"), _T("Error"), MB_OK);
 		return E_FAIL;
     }
#endif
	
	ZeroMemory(&sInit, sizeof(sInit));
	sInit.dwBuffers = m_vBackBufferEnable ? m_vBackBufferNumber : 1;
	sInit.dwWidth = m_vSourceWidth;
	sInit.dwHeight= m_vSourceHeight;
	if(surface_mode==eAuto)
	{
		sInit.dwSurfType = 0;
	}
	else if(surface_mode==eGDI)
	{
		sInit.dwSurfType |= VP_OPEN_FLAG_GDI;
	}
	else if(surface_mode==eDDRAW)
	{
		sInit.dwSurfType|= VP_OPEN_FLAG_DDRAW;
	}
	else if(surface_mode==eD3D)
	{
		sInit.dwSurfType |= VP_OPEN_FLAG_D3D;
	}
	else if(surface_mode==eSDL)
	{
		sInit.dwSurfType |= VP_OPEN_FLAG_SDL;
	}
	else if(surface_mode == eSAMSUNG)
	{
		sInit.dwSurfType |= VP_OPEN_FLAG_SAMSUNG;
	}
	
#ifdef CREATE_WINDOW_BY_APP
	sInit.hWnd = m_wnd;
	#endif

	sInit.dwFourCC = MAKE_FOURCC('Y','V','1','2');
#ifdef OMX_TEST
	sInit.dwFourCC = MAKE_FOURCC('M','P','G','2');
#endif

	if(m_pPresent->Open(&sInit,sizeof(sInit)))
		return E_FAIL;

	bool enable = true;
	m_pPresent->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);

	VP_SURFACE_OpenOptions opt;
	memset(&opt, 0 ,sizeof(opt));
	opt.dwWidth = sInit.dwWidth;
	opt.dwHeight= sInit.dwHeight;
	opt.dwFourCC = MAKE_FOURCC('Y','V','1','2');

#ifdef USE_NEW_INTERFACE
		m_pPresent->CreateSurface(VIDEO_SURFACE,&opt);
	#ifdef DEMO_PIP
		m_pPresent->CreateSurface(TTX_SURFACE,&opt);
		m_pPresent->CreateSurface(SUBTITLE_SURFACE,&opt);
	#endif
#endif

#ifdef OSD_TEST
	sInit.dwWidth = OSD_SRC_WIDTH;
	sInit.dwHeight = OSD_SRC_HEIGHT;
	sInit.dwFourCC = MAKE_FOURCC('R','5','6','5');
	sInit.dwBuffers = 1;
	sInit.dwSurfType = VP_OPEN_FLAG_DDRAW_OSD;
	if(m_pPresent_OSD->Open(&sInit,sizeof(sInit)))
		return E_FAIL;

	m_pPresent_OSD->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);
	
#endif

	memset(&m_rect_last, 0 ,sizeof(m_rect_last));
	return S_OK;
}

HRESULT CLibVrPresent_TestDlg::Init_CreatePresenter(HMODULE& h_dll)
{
	return Init_CreatePresenterProbe(h_dll, etype);
}

void CLibVrPresent_TestDlg::OnBnClickedButtonOpen()
{
	HMODULE h_dll;	
	if(FAILED(LoadLib(h_dll)))
		goto error;
	
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

	return;

error:
	ClosePresenterAndFreeLib(m_pPresent, h_dll);

}

void CLibVrPresent_TestDlg::OnTimer(UINT_PTR nIDEvent)
{
	UpdateData(TRUE);
	if(status == ePause) 
		return;
	else if(status == eStop)
	{
		CloseVideo();
#ifdef _WIN32_WCE
#ifndef SHx
		DWORD cpu_val = m_idle.Measure();
		process_end = GetTickCount();
		idle_end = GetIdleTime();
		TCHAR logBuf[256];	
		TCHAR logBuf2[256];	
		_stprintf(logBuf, _T("Player duration %dms, idle %dms pure render:%f \n"), process_end-process_start, idle_end-idle_start, (float)((float)((float)render_start/1000)*100/(float)(((float)process_end-(float)process_start)/1000)));
		_stprintf(logBuf2, _T("CPU usage %d%% frm_rate :%f sdio: %f \n"), 	cpu_val, (float)((float)frameCount/(float)((float)((float)process_end-(float)process_start)/1000)), 
			(float)((float)((float)sdio_start/1000)*100/(float)(((float)process_end-(float)process_start)/1000)));
		_tcscat(logBuf,logBuf2);
		MessageBox(logBuf, _T("INFO"), MB_OK);
		sdio_start = render_start = 0;
#endif
#endif
		return;
	}
	static BOOL bFirst = TRUE;
#ifndef TEST_V2_SURF
	if(bFirst  == TRUE && SetDestRect(m_pPresent))
#else
	if(bFirst  == TRUE && SetDestRect(m_pPresentAdv))
#endif
	{
		bFirst = FALSE;
		return;
	}
	bFirst = FALSE;
	//
	//YV12 MODE START
	//	

	//Main Loop 


	DWORD FrameSize = (Srcrect.right*Srcrect.bottom * 3)>>1;
	LPBYTE curr[3];
	if(((m_FileLength - ftell(m_file)) >= FrameSize)&&(!feof(m_file)))
	{
		DWORD tmpTick2 = GetTickCount();

		DWORD y_stride = Srcrect.right;
		DWORD u_stride = Srcrect.right>>1;
		DWORD v_stride = Srcrect.right>>1;;
#ifdef STRIDE_TEST
		y_stride = 384;
		u_stride = v_stride = 176;
#endif
#ifndef NO_FEED_DATA
		fread(m_pYbuf ,sizeof(BYTE),y_stride*Srcrect.bottom,m_file);
#ifndef OMX_TEST
		fread(m_pUbuf,sizeof(BYTE),u_stride*Srcrect.bottom>>1,m_file);
		fread(m_pVbuf,sizeof(BYTE),v_stride*Srcrect.bottom>>1,m_file);
#else

		m_dwStrides[0] = y_stride*Srcrect.bottom;
#endif
#endif

#ifdef _WIN32_WCE
		sdio_start+= (GetTickCount() - tmpTick2);
#endif
		curr[0] = &m_pYbuf [0];
#ifndef OMX_TEST
		curr[1] = &m_pUbuf[0];
		curr[2] = &m_pVbuf[0];
#endif
		DWORD tmpTick = GetTickCount();
		
		if(m_vBackBufferEnable && m_dwBuffers > 1)//back buffer mode
		{
			PresentBufferInfo_T* PresentObject = NULL;
			while(PresentObject == NULL)
			{
				PresentObject = GetFreePresentObject();
				Sleep(10);
			}

#ifdef USE_NEW_INTERFACE
			if(FAILED(m_pPresent->PrepareBuffer(VIDEO_SURFACE,PresentObject->FrameNumber, curr, m_dwStrides, 0)))
#else
			if(FAILED(m_pPresent->PrepareBuffer(PresentObject->FrameNumber, curr, m_dwStrides, 0)))
#endif
			{
				PresentObject->Vaild = FALSE;
				//RETAILMSG(1, (TEXT("Prepare Buffer %d Failed\r\n"), PresentObject->FrameNumber));
			}
			else
			{
				//RETAILMSG(1, (TEXT("Prepare Buffer %d OK\r\n"), PresentObject->FrameNumber));
				PresentObject->Vaild = TRUE;
			}
			PutPresentObject(PresentObject);
			ReleaseSemaphore(m_hBBPresentSema, 1, NULL);
		}
		else// non back buffer mode
		{
#ifdef USE_NEW_INTERFACE
#ifndef NO_FEED_DATA
			m_pPresent->Present(VIDEO_SURFACE,curr, m_dwStrides, 0);
	#ifdef DEMO_PIP
			m_pPresent->Present(TTX_SURFACE,curr, m_dwStrides, 0);
			m_pPresent->Present(SUBTITLE_SURFACE,curr, m_dwStrides, 0);
	#endif
#endif
#else
			DWORD tick = GetTickCount();
			if(m_pPresentAdv)
			{
				static DWORD startTick = GetTickCount();
				m_pMainStream->Present(curr, m_dwStrides, 0);
	//			Sleep(40);
				//if(GetTickCount() - startTick > 2000)
				{
					static FILE* fp = NULL;
					if(!fp)
					{
						fp = _tfopen(tc_path, _T("rb"));
						fseek(fp, 0 , SEEK_SET);
						if(fp == NULL)
						{
							MessageBox(_T("Can't open input file!\n"), _T("Error"), MB_OK);
						}
						bool enable = true;
						if(m_pSubStream3)
							m_pSubStream3->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);

					}

					if(!(((m_FileLength - ftell(fp)) >= FrameSize)&&(!feof(fp))))
						fseek(fp, 0 , SEEK_SET);
					
					//fread(m_pYbuf ,sizeof(BYTE),y_stride*Srcrect.bottom,fp);
					//fread(m_pUbuf,sizeof(BYTE),u_stride*Srcrect.bottom>>1,fp);
					//fread(m_pVbuf,sizeof(BYTE),v_stride*Srcrect.bottom>>1,fp);
#ifndef NO_FEED_DATA
					if(m_pSubStream3)
						m_pSubStream3->Present(curr, m_dwStrides, 0);
#endif

#if 0
					static DWORD rendertimer = 0;
					rendertimer += 4;
					if(rendertimer >255)
						rendertimer = 255;
					if(m_pSubStream3)
					{
						VP_SCAN_MODE scanMode = VP_SCAN_INTERLACED_TOPFIELED_FIRST;
						m_pSubStream3->Set(IID_IMcVideoPresenter, MC_PROPID_VP_SCAN_MODE,0,0,&scanMode,0);
						m_pSubStream3->Set(IID_IMcVideoPresenter, MC_PROPID_VP_ALPHA,0,0,&rendertimer,0);
					}

					RECT dst2;
					SetRect(&dst2, m_vSourceWidth/2 * ((float)(255 - rendertimer)/255),0, m_vSourceWidth/2+m_vSourceWidth/2* ((float)(255 - rendertimer)/255),m_vSourceHeight/2);
					dst2.left *= (float)rendertimer/255;
					dst2.top *= (float)rendertimer/255;
					dst2.right *= (float)rendertimer/255;
					dst2.bottom *= (float)rendertimer/255;
					if(m_pSubStream3)
						m_pSubStream3->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&dst2,0);
//Animation Test
					static bool bOpen = false;
					if(!bOpen)
					{
						bOpen = true;
						if(m_pAnimation)
						{

							m_pAnimation->Open(&m_pSubStream3);
							ANIMATION_PARAMS param;
							memset(&param, 0 ,sizeof(param));
							param.type = MC_ANIMATION_LINE ;
							SetRect(&param.sLine.rSrc, m_vSourceWidth/2, 0, m_vSourceWidth, m_vSourceHeight/2 );
							SetRect(&param.sLine.rDst, 0,0, m_vSourceWidth/2, m_vSourceHeight/2);
							param.sLine.repeatTimes = 0xff;
							param.sLine.duration  = 1000;
							m_pAnimation->Start(&param,NULL);
					
						}

					}
#endif
				}


#if 0
				if(GetTickCount() - startTick > 4000 && m_pSubStream4)
				{
					static FILE* fp2 = NULL;
					if(!fp2)
					{
						fp2 = _tfopen(tc_path, _T("rb"));
						fseek(fp2, 0 , SEEK_SET);
						if(fp2 == NULL)
						{
							MessageBox(_T("Can't open input file!\n"), _T("Error"), MB_OK);
						}
						bool enable = true;
						m_pSubStream4->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);

						DWORD rendertimer = 60;
						m_pSubStream4->Set(IID_IMcVideoPresenter, MC_PROPID_VP_ALPHA,0,0,&rendertimer,0);

					}

					if(!(((m_FileLength - ftell(fp2)) >= FrameSize)&&(!feof(fp2))))
						fseek(fp2, 0 , SEEK_SET);
					
					fread(m_pYbuf ,sizeof(BYTE),y_stride*Srcrect.bottom,fp2);
					fread(m_pUbuf,sizeof(BYTE),u_stride*Srcrect.bottom>>1,fp2);
					fread(m_pVbuf,sizeof(BYTE),v_stride*Srcrect.bottom>>1,fp2);
					m_pSubStream4->Present(curr, m_dwStrides, 0);

				}


				if(GetTickCount() - startTick > 6000 && m_pSubStream5)
				{
					static FILE* fp3 = NULL;
					if(!fp3)
					{
						fp3 = _tfopen(tc_path, _T("rb"));
						fseek(fp3, 0 , SEEK_SET);
						if(fp3 == NULL)
						{
							MessageBox(_T("Can't open input file!\n"), _T("Error"), MB_OK);
						}
						bool enable = true;
						m_pSubStream5->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);

						//DWORD rendertimer = 180;
						//m_pSubStream5->Set(IID_IMcVideoPresenter, MC_PROPID_VP_ALPHA,0,0,&rendertimer,0);

					}

					if(!(((m_FileLength - ftell(fp3)) >= FrameSize)&&(!feof(fp3))))
						fseek(fp3, 0 , SEEK_SET);
					
					fread(m_pYbuf ,sizeof(BYTE),y_stride*Srcrect.bottom,fp3);
					fread(m_pUbuf,sizeof(BYTE),u_stride*Srcrect.bottom>>1,fp3);
					fread(m_pVbuf,sizeof(BYTE),v_stride*Srcrect.bottom>>1,fp3);
					m_pSubStream5->Present(curr, m_dwStrides, 0);

				}
#if 0
				if(GetTickCount() - startTick > 12000 && m_pSubStream6)
				{
					static FILE* fp4 = NULL;
					if(!fp4)
					{
						fp4 = _tfopen(tc_path, _T("rb"));
						fseek(fp4, 0 , SEEK_SET);
						if(fp4 == NULL)
						{
							MessageBox(_T("Can't open input file!\n"), _T("Error"), MB_OK);
						}
						bool enable = true;
						m_pSubStream6->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);
						DWORD rendertimer = 220;
						m_pSubStream6->Set(IID_IMcVideoPresenter, MC_PROPID_VP_ALPHA,0,0,&rendertimer,0);

					}

					if(!(((m_FileLength - ftell(fp4)) >= FrameSize)&&(!feof(fp4))))
						fseek(fp4, 0 , SEEK_SET);
					
					fread(m_pYbuf ,sizeof(BYTE),y_stride*Srcrect.bottom,fp4);
					fread(m_pUbuf,sizeof(BYTE),u_stride*Srcrect.bottom>>1,fp4);
					fread(m_pVbuf,sizeof(BYTE),v_stride*Srcrect.bottom>>1,fp4);
					m_pSubStream6->Present(curr, m_dwStrides, 0);

				}
#endif
#endif
				
#ifndef NO_FEED_DATA

				HRESULT hr = m_pPresentAdv->Render(0);
				if(hr == VR_FAIL_CANVASCHANGED)
				{
					VP_CANVASINFO_PARAMS canvas;
					m_pPresentAdv->Get(IID_IMcVideoPresenter, MC_PROPID_VP_CANVAS_INFO,0,0,&canvas,0,0);

					RECT dst;
					SetRect(&dst, 0,0, canvas.dwWidth,canvas.dwHeight);
					m_pMainStream->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&dst,0);

								
					SetRect(&dst, 0,0, canvas.dwWidth/2,canvas.dwHeight/2);
					m_pSubStream3->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&dst,0);
				}
#endif
				//if(GetTickCount() - tick <40)
			//		Sleep(40 - (GetTickCount() - tick));
			}

#ifndef NO_FEED_DATA
			if(m_pPresent)
				m_pPresent->Present(curr, m_dwStrides, 0);
#endif
#endif
		}
#ifdef _WIN32_WCE
		render_start += (GetTickCount() - tmpTick);
		frameCount++;
#endif
	}
	// to the end of the stream
	else
	{
		if(FALSE == m_bReplay)
		{
			status = eStop;
		}
		else
			fseek(m_file, 0 , SEEK_SET);
	}
{
	static DWORD fmtCnt = 0;
	fmtCnt ++;
	static DWORD SetTick = GetTickCount();
	if(GetTickCount() -SetTick > 3000 )
	{
		TCHAR buf[256];

		_stprintf(buf,_T("fps:%.1f \n"),(float)fmtCnt/ ((GetTickCount() -SetTick)/1000));
#if 1//ndef _WIN32_WCE
		OutputDebugString(buf);
#endif
		SetTick = GetTickCount();
		fmtCnt = 0;
	}
}
#ifdef OSD_TEST
	LPBYTE pOSDbuf  = (LPBYTE)malloc(OSD_SRC_WIDTH*OSD_SRC_HEIGHT*2);
	LPBYTE curr_osd[3];
	if(pOSDbuf == NULL)
		MessageBox(_T("OSD Memory malloc failed\n"), _T("Error"), MB_OK);
	else
	{
		static DWORD i=0;
		i+=0x7;
		for(DWORD j=0;j<OSD_SRC_WIDTH*OSD_SRC_HEIGHT*2;j++)
		{
//			pOSDbuf[j] =  (i++);
			pOSDbuf[j] =  rand();
		}
		curr_osd[0] = curr_osd[1] = curr_osd[2] = &pOSDbuf[0];

#ifndef NO_FEED_DATA
		m_pPresent_OSD->Present(curr_osd, m_dwStrides , 0);
#endif
		free(pOSDbuf);
	}
#endif			
    //
    //YV12 MODE END
    //

	//static DWORD lasttick = GetTickCount();
	//DWORD tmpTick = 0;

	//tmpTick = GetTickCount() - lasttick;
	//if(tmpTick < (1000/FPS_CT))
	//	Sleep((1000/FPS_CT) - tmpTick);
	//lasttick = GetTickCount();

}

void CLibVrPresent_TestDlg::OnBnClickedCheckBbEnable()
{
    UpdateData(TRUE);

    CWnd* hWnd = GetDlgItem(IDC_EDIT_BB_NUMBER);
    if(hWnd)
    {
        hWnd->EnableWindow(m_vBackBufferEnable);
    }
}
void CLibVrPresent_TestDlg::OnBnClickedButtonPause()
{
	// TODO: Add your control notification handler code here
	if(ePlay == status)
	{
		status = ePause;
		m_PausePlay.SetWindowText(_T("PLAY"));
	}
	else if(ePause == status)
	{
		status = ePlay;
		m_PausePlay.SetWindowText(_T("PAUSE"));
	}
	else
	{
		MessageBox(_T("Play status error\n"), _T("Error"), MB_OK);
		return;
	}
}

void CLibVrPresent_TestDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	if(eStop != status)
	{

#ifdef _WIN32_WCE
#ifndef SHx
		DWORD cpu_val = m_idle.Measure();
		process_end = GetTickCount();
		idle_end = GetIdleTime();
		TCHAR logBuf[256];	
		TCHAR logBuf2[256];	
		_stprintf(logBuf, _T("Player duration %dms, idle %dms pure render:%f \n"), process_end-process_start, idle_end-idle_start, (float)((float)((float)render_start/1000)*100/(float)(((float)process_end-(float)process_start)/1000)));
		_stprintf(logBuf2, _T("CPU usage %d%% frm_rate :%f sdio: %f \n"), 	cpu_val, (float)((float)frameCount/(float)((float)((float)process_end-(float)process_start)/1000)), 
			(float)((float)((float)sdio_start/1000)*100/(float)(((float)process_end-(float)process_start)/1000)));
		_tcscat(logBuf,logBuf2);
		MessageBox(logBuf, _T("INFO"), MB_OK);
		sdio_start = render_start = 0;
#endif
#endif
		CloseVideo();
		m_PausePlay.SetWindowText(_T("PLAY"));
	}
	else
	{
		MessageBox(_T("Play status error\n"), _T("Error"), MB_OK);
		return;
	}
}

void CLibVrPresent_TestDlg::MoveDest(CPoint point)
{
	m_vDestStartX = point.x;
	m_vDestStartY = point.y;
	UpdateData(FALSE);//put modify to window
}

void CLibVrPresent_TestDlg::OpenVideo()
{
#ifdef TEST_V2_SURF
	if(setPage)
		setPage->SetPresent(m_pPresentAdv);
#else
	if(setPage)
		setPage->SetPresent(m_pPresent);
#endif

	SetPlayWnd();
#ifdef CREATE_WINDOW_BY_APP
	ddlg->ShowWindow(SW_SHOWNORMAL);
#endif
	EnableSettings(FALSE);
	m_PausePlay.EnableWindow(TRUE);
	m_Stop.EnableWindow(TRUE);
	m_PausePlay.SetWindowText(_T("PAUSE"));

	GetDlgItem(IDC_RADIO_CROPPING)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_LETTER_BOX)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_STRETCH)->EnableWindow(TRUE);
    if(SetClipRect(m_pPresent))
    {
	    MessageBox(_T("Invalid source size \n"), _T("Error"), MB_OK);
		return;
	}

#ifdef CREATE_WINDOW_BY_APP
#ifndef _WIN32_WCE
	SetDestColorKey(m_pPresent);
	ddlg->SetCKey(m_bCKey,m_dwCKey);
#endif
#endif

	m_lRotationMode = VP_ROTATION_0;
	if(m_pPresent)
	{
		m_pPresent->Set(IID_IMcVideoPresenter, MC_PROPID_VP_ROTATION,0,0,&m_lRotationMode,0);
		if(0 != (m_pPresent->GetBackBuffers(&m_dwBuffers)))
		{
			MessageBox(_T("Can not get BackBuffer \n"), _T("Error"), MB_OK);
			return;
		}

	}

	//Back Buffer Mode, then Create a new thread
	if(m_vBackBufferEnable && m_dwBuffers > 1)
	{
		InitPresentObject(m_dwBuffers);
		m_hBBPresentThread = ::CreateThread(NULL, 0, BBPresentProc, this, 0, NULL);
		SetThreadPriority(m_hBBPresentThread, THREAD_PRIORITY_ABOVE_NORMAL);
		m_fStopPresentThread = FALSE;
	}

	ZeroMemory(&Srcrect, sizeof(Srcrect));
	Srcrect.left = 0;
	Srcrect.right= m_vSourceWidth;
	Srcrect.top = 0;
	Srcrect.bottom = m_vSourceHeight;
	
	ZeroMemory(&Dstrect, sizeof(Dstrect));
	Dstrect.left = m_vDestStartX;
	Dstrect.right= Dstrect.left + m_vDestWidth;
	Dstrect.top = m_vDestStartY;
	Dstrect.bottom = Dstrect.top + m_vDestHeight;

#ifdef STRIDE_TEST
	m_pYbuf = (LPBYTE)malloc(384*180);
	m_pUbuf = (LPBYTE)malloc(384*180>>2);
	m_pVbuf = (LPBYTE)malloc(384*180>>2);

#else
	m_pYbuf = (LPBYTE)malloc(Srcrect.right*Srcrect.bottom);
	m_pUbuf = (LPBYTE)malloc(Srcrect.right*Srcrect.bottom>>2);
	m_pVbuf = (LPBYTE)malloc(Srcrect.right*Srcrect.bottom>>2);
#endif
	if((!m_pYbuf )||(!m_pUbuf)||(!m_pVbuf))
	{
	    MessageBox(_T("Memory malloc failed\n"), _T("Error"), MB_OK);
	    return;
	}

	//set a timer to display
	SetTimer(PresentTimerID,PresentTimer_Interval,NULL);	

	status = ePlay;

	SetDisplayMode(m_eVideoMode);

#ifdef _WIN32_WCE
	frameCount = 0;
#endif
#ifdef _WIN32_WCE
#ifndef SHx
	m_idle.Open();
#endif
#endif

}


void CLibVrPresent_TestDlg::CloseVideo()
{
#ifdef _WIN32_WCE
#ifndef SHx
	m_idle.Close();
#endif
#endif
	status = eStop;

	if(m_vBackBufferEnable && m_dwBuffers > 1)
	{
		WaitPresentObjectEmpty(2000);
		m_fStopPresentThread = TRUE;
		WaitForSingleObject(m_hBBPresentThread, INFINITE);
		CloseHandle(m_hBBPresentThread);
		DeinitPresentObject();
		m_dwBuffers = 0;
	}

#ifdef CREATE_WINDOW_BY_APP
	ddlg->ShowWindow(SW_HIDE);
#endif
	EnableSettings(TRUE);
	m_PausePlay.EnableWindow(FALSE);
	m_Stop.EnableWindow(FALSE);

	if(setPage)
		setPage->SetPresent(NULL);

	if(m_pPresentAdv)
	{
		if(m_pMainStream)
		{
			m_pMainStream->Close();
			m_pPresentAdv->DestroyStream(m_pMainStream);
			m_pMainStream = NULL;
		}
		if(m_pSubStream1)
		{
			m_pSubStream1->Close();
			m_pPresentAdv->DestroyStream(m_pSubStream1);
			m_pSubStream1= NULL;
		}
		if(m_pSubStream2)
		{
			m_pSubStream2->Close();
			m_pPresentAdv->DestroyStream(m_pSubStream2);
			m_pSubStream2 = NULL;
		}
		m_pPresentAdv->Close();
		m_pPresentAdv->Release();
		m_pPresentAdv = NULL;
		
	}


	if(m_pPresent)
	{
#ifdef USE_NEW_INTERFACE
		m_pPresent->DestroySurface(VIDEO_SURFACE);
#endif
		m_pPresent->Close();
		m_pPresent->Release();
		m_pPresent = NULL;
	}

#ifdef OSD_TEST
	if(m_pPresent_OSD)
	{
		m_pPresent_OSD->Close();
		m_pPresent_OSD->Release();
		m_pPresent_OSD = NULL;
	}
#endif
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


void CLibVrPresent_TestDlg::ReSize(CSize size)
{
	TCHAR sz[10];
	_stprintf(sz, _T("%d"), size.cx);
	SetDlgItemText(IDC_EDIT_DestWidth, sz);
	_stprintf(sz, _T("%d"), size.cy);
	SetDlgItemText(IDC_EDIT_DestHeight, sz);
}

void CLibVrPresent_TestDlg::EnableSettings(bool able)
{	
	GetDlgItem(IDC_EDIT_SourceWidth)->EnableWindow(able);
	GetDlgItem(IDC_EDIT_SourceHeight)->EnableWindow(able);
	GetDlgItem(IDC_EDIT_ClipStartx)->EnableWindow(able);
	GetDlgItem(IDC_EDIT_ClipStartY)->EnableWindow(able);
	GetDlgItem(IDC_EDIT_Clip_Width)->EnableWindow(able);
	GetDlgItem(IDC_EDIT_ClipHeight)->EnableWindow(able);
	GetDlgItem(IDC_CHECK_BB_ENABLE)->EnableWindow(able);
	GetDlgItem(IDC_EDIT_BB_NUMBER)->EnableWindow(able);
}

void CLibVrPresent_TestDlg::FullScrVideo()
{
	RECT rect;
	if(FALSE == m_isFullScr)
	{
		m_isFullScr = TRUE;

		m_BeforeFullScrRect.left = m_vDestStartX;
		m_BeforeFullScrRect.top = m_vDestStartY;
		m_BeforeFullScrRect.right = m_vDestStartX + m_vDestWidth;
		m_BeforeFullScrRect.bottom = m_vDestStartY + m_vDestHeight;

		LONG type = GetWindowLong(m_hWnd, GWL_STYLE);
		type  &= ~WS_BORDER;
		SetWindowLong(m_hWnd, GWL_STYLE, type);
		rect.top = rect.left = 0;
		rect.right = GetSystemMetrics(SM_CXSCREEN);
		rect.bottom = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		LONG type = GetWindowLong(m_hWnd, GWL_STYLE);
		type  |= WS_BORDER;
		SetWindowLong(m_hWnd, GWL_STYLE, type);

		m_isFullScr = FALSE;
		rect = m_BeforeFullScrRect;
	}
	
#ifdef CREATE_WINDOW_BY_APP
	ddlg->MoveWindow(rect.left-3, rect.top-29, rect.right-rect.left+6, rect.bottom-rect.top+32, 1);
#endif
	m_vDestStartX = rect.left;
	m_vDestStartY = rect.top;
	m_vDestWidth = rect.right - rect.left;
	m_vDestHeight = rect.bottom - rect.top;
	UpdateData(FALSE);
}

void CLibVrPresent_TestDlg::OnBnClickedRadioLetterBox()
{
	// TODO: Add your control notification handler code here
	m_eVideoMode = VP_ZOOM_MODE_LETTERBOX;
	SetDisplayMode(m_eVideoMode);
}

void CLibVrPresent_TestDlg::OnBnClickedRadioCropping()
{
	// TODO: Add your control notification handler code here
	m_eVideoMode = VP_ZOOM_MODE_CROPPING;
	SetDisplayMode(m_eVideoMode);
}

void CLibVrPresent_TestDlg::OnBnClickedRadioStretch()
{
	// TODO: Add your control notification handler code here
	m_eVideoMode = VP_ZOOM_MODE_STRETCH;
	SetDisplayMode(m_eVideoMode);
}

void CLibVrPresent_TestDlg::SetDisplayMode(VP_ZOOM_MODE mode)
{
	if(m_pPresentAdv)
		m_pPresentAdv->Set(IID_IMcVideoPresenter, MC_PROPID_VP_ZOOM,0,0,&m_eVideoMode,0);
	if(m_pPresent)
		m_pPresent->Set(IID_IMcVideoPresenter, MC_PROPID_VP_ZOOM,0,0,&m_eVideoMode,0);
}

BOOL CLibVrPresent_TestDlg::IsValidDll(TCHAR *dllFile)
{
	int TotalNum = 0, ValidNum = 0;
	TCHAR FilePath[MAX_PATH] = {0};

	_tcscpy(FilePath, dllFile);
	HMODULE h_dll;
	h_dll = LoadLibrary(FilePath); 
	if(h_dll == NULL)
		goto error;

	if(FAILED(Init_CreatePresenter(h_dll)))
		goto error;

	ClosePresenterAndFreeLib(m_pPresent, h_dll);
	return TRUE;

error:
	ClosePresenterAndFreeLib(m_pPresent, h_dll);
	return FALSE;
}

void CLibVrPresent_TestDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	if(m_Tab.GetCurSel()==1)
	{
		CRect recttab;
		m_Tab.GetWindowRect(&recttab);
		int cy = GetSystemMetrics(SM_CYMENU);
		recttab.top += cy;
		if(setPage)
		{
			setPage->MoveWindow(&recttab);
			setPage->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if(setPage)
			setPage->ShowWindow(SW_HIDE);
	}
	*pResult = 0;
}

// Put SetDlg with MainWindow
void CLibVrPresent_TestDlg::OnMove(int x,int y)
{
	CDialog::OnMove(x,y);

	// TODO: Add your message handler code here
	CRect recttab;
	m_Tab.GetWindowRect(&recttab);
	int cy = GetSystemMetrics(SM_CYMENU);
	recttab.top += cy;
	if(setPage)
		setPage->MoveWindow(&recttab);
}

void CLibVrPresent_TestDlg::ClosePresenterAndFreeLib(IMcVideoPresenter*& pPresent, HMODULE& h_dll)
{
	if(pPresent != NULL)
	{
		pPresent->Close();
		pPresent->Release();
		pPresent = NULL;
	}
#ifdef OSD_TEST
	if(m_pPresent_OSD)
	{
		m_pPresent_OSD->Close();
		m_pPresent_OSD->Release();
		m_pPresent_OSD = NULL;
	}
#endif

	FreeLibrary(h_dll);
	return;
}


void CLibVrPresent_TestDlg::OnBnClickedButtonRotate()
{
	switch(m_lRotationMode)
	{
		case VP_ROTATION_0:
			m_lRotationMode = VP_ROTATION_90;
			break;
		case VP_ROTATION_90:
			m_lRotationMode = VP_ROTATION_180;
			break;
		case VP_ROTATION_180:
			m_lRotationMode = VP_ROTATION_270;
			break;
		case VP_ROTATION_270:
			m_lRotationMode = VP_ROTATION_0;
			break;
	}
	m_pPresent->Set(IID_IMcVideoPresenter, MC_PROPID_VP_ROTATION,0,0,&m_lRotationMode,0);
	
}

void CLibVrPresent_TestDlg::OnBnClickedButtonOsd1()
{
	#define SUBTITLE_WIDTH		720
	#define SUBTITLE_HEIGHT		36

#if defined(USE_NEW_INTERFACE) || defined(TEST_V2_SURF)
	bool bV2Test = m_pPresentAdv?true:false;
	HRESULT hr = S_OK;
	if(bOsd1Enabled == FALSE)
	{
		TCHAR DirPath[MAX_PATH] = {0};
		TCHAR FilePath[MAX_PATH] = {0};
		// Get Tuners folder path
		GetModuleFileName(NULL, DirPath, MAX_PATH);
		TCHAR*	p = _tcsrchr(DirPath, '\\');
		p++;
		p[0] = 0;
		_tcscpy(FilePath, DirPath);
		_tcscat(FilePath, _T("SUB_720x36.YUV"));

		FILE* fp = _tfopen(FilePath, _T("rb"));
		if(fp)
		{

			if(bV2Test)
			{
				if(!m_pSubStream1)
				{
					VP_OPEN_STREAM_PARAMS params;
					params.type = eSUBSTREAMS;
					params.dwWidth = SUBTITLE_WIDTH;
					params.dwHeight = SUBTITLE_HEIGHT;
					params.dwFourCC = MAKE_FOURCC('Y','V','1','2');
					hr = m_pPresentAdv->CreateStream(&params, sizeof(params),&m_pSubStream1);
					if(FAILED(hr))	goto RET;
					hr = m_pSubStream1->Open();
					if(FAILED(hr))	goto RET;
				}

				RECT dst;
				SetRect(&dst, 0,0, SUBTITLE_WIDTH,SUBTITLE_HEIGHT);
				hr = m_pSubStream1->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&dst,0);
				if(FAILED(hr))	goto RET;

				bool enable = true;
				hr = m_pSubStream1->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);
				if(FAILED(hr))	goto RET;
			}
			else
			{
				RECT src = {0,0,SUBTITLE_WIDTH,SUBTITLE_HEIGHT};
				VP_SURFACE_OpenOptions opt;
				memset(&opt, 0 ,sizeof(opt));
				opt.dwWidth = src.right - src.left;
				opt.dwHeight= src.bottom - src.top;
				opt.dwFourCC = MAKE_FOURCC('Y','V','1','2');
				HRESULT res = m_pPresent->CreateSurface(TTX_SURFACE,&opt);
				if(FAILED(res))
					goto RET;
				
				RECT ttxsurf_rect;
				memset(&ttxsurf_rect, 0 ,sizeof(ttxsurf_rect));
				ttxsurf_rect.left = 0;
				ttxsurf_rect.top = m_vSourceHeight - SUBTITLE_HEIGHT;
				ttxsurf_rect.right =  m_vSourceWidth;
				ttxsurf_rect.bottom =  m_vSourceHeight;
				m_pPresent->SetSurface(TTX_SURFACE,IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&ttxsurf_rect,0);

				DWORD dwTransColor = (0x39<<16)|(0x0<<8)|(0x0);
				m_pPresent->SetSurface(TTX_SURFACE,IID_IMcVideoPresenter,MC_PROPID_VP_COLORKEYSRC,0,0,&dwTransColor,0);

				DWORD dwAlpha = MAX_ALPHA_VAL/2;
				m_pPresent->SetSurface(TTX_SURFACE,IID_IMcVideoPresenter,MC_PROPID_VP_ALPHA,0,0,&dwAlpha,0);

			}


			BYTE buf[SUBTITLE_WIDTH*SUBTITLE_HEIGHT*3/2];
			fread(buf,sizeof(BYTE),SUBTITLE_WIDTH*SUBTITLE_HEIGHT*3/2, fp);

			LPBYTE curr[3];
			curr[0]= buf;
			curr[1]= buf + SUBTITLE_WIDTH*SUBTITLE_HEIGHT;
			curr[2]= buf + SUBTITLE_WIDTH*SUBTITLE_HEIGHT*5/4;

			DWORD dwStrides[3];
			dwStrides[0] = SUBTITLE_WIDTH;
			dwStrides[1] = dwStrides[2] = SUBTITLE_WIDTH>>1;
			
			if(bV2Test)
			{
				if(m_pSubStream1)
				{
#ifndef NO_FEED_DATA
					hr = m_pSubStream1->Present(curr, dwStrides, 0);
					if(FAILED(hr))	goto RET;
#endif
					bool enable = true;
					m_pPresentAdv->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);
					hr = m_pSubStream1->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);
					if(FAILED(hr))	goto RET;
#ifndef NO_FEED_DATA
					hr = m_pPresentAdv->Render(0);
					if(FAILED(hr))	goto RET;
#endif
				
				}
			}
#ifndef NO_FEED_DATA
			else
				m_pPresent->Present(TTX_SURFACE,curr, dwStrides, 0);
#endif
			bOsd1Enabled = TRUE;
RET:
			fclose(fp);
		}

	}
	else
	{
		if(bV2Test)
		{
			if(m_pSubStream1)
			{
				bool enable = false;
				m_pPresentAdv->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);
//				m_pSubStream1->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);
			}
		}
		else
		{
			if(m_pPresent)
				m_pPresent->DestroySurface(TTX_SURFACE);
		}
		bOsd1Enabled = FALSE;
	}
#endif

}

void CLibVrPresent_TestDlg::OnBnClickedButtonOsd2()
{
	#define TTX_WIDTH		480
	#define TTX_HEIGHT		520

#if defined(USE_NEW_INTERFACE) || defined(TEST_V2_SURF)
	bool bV2Test = m_pPresentAdv?true:false;
	HRESULT hr = S_OK;
	if(bOsd2Enabled == FALSE)
	{
		
		TCHAR DirPath[MAX_PATH] = {0};
		TCHAR FilePath[MAX_PATH] = {0};
		// Get Tuners folder path
		GetModuleFileName(NULL, DirPath, MAX_PATH);
		TCHAR*	p = _tcsrchr(DirPath, '\\');
		p++;
		p[0] = 0;
		_tcscpy(FilePath, DirPath);
		_tcscat(FilePath, _T("TTX_480x520.YUV"));

		FILE* fp = _tfopen(FilePath, _T("rb"));
		if(fp)
		{
			if(bV2Test)
			{
				if(!m_pSubStream2)
				{
					VP_OPEN_STREAM_PARAMS params;
					params.type = eSUBSTREAMS;
					params.dwWidth = TTX_WIDTH;
					params.dwHeight = TTX_HEIGHT;
					params.dwFourCC = MAKE_FOURCC('Y','V','1','2');
					hr = m_pPresentAdv->CreateStream(&params, sizeof(params),&m_pSubStream2);
					if(FAILED(hr))	goto RET;
					hr = m_pSubStream2->Open();
					if(FAILED(hr))	goto RET;
				}
				RECT dst;
				SetRect(&dst, 0,0, TTX_WIDTH,TTX_HEIGHT);
				hr = m_pSubStream2->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&dst,0);
				if(FAILED(hr))	goto RET;

				bool enable = true;
				hr = m_pSubStream2->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);
				if(FAILED(hr))	goto RET;

			}
			else
			{
				RECT src = {0,0,TTX_WIDTH,TTX_HEIGHT};
				VP_SURFACE_OpenOptions opt;
				memset(&opt, 0 ,sizeof(opt));
				opt.dwWidth = src.right - src.left;
				opt.dwHeight= src.bottom - src.top;
				opt.dwFourCC = MAKE_FOURCC('Y','V','1','2');
				opt.dwTranColor = RGB(57,0,0);
				HRESULT res = m_pPresent->CreateSurface(SUBTITLE_SURFACE,&opt);

				if(FAILED(res))
					goto RET;

				RECT ttxsurf_rect;
				memset(&ttxsurf_rect, 0 ,sizeof(ttxsurf_rect));
				ttxsurf_rect.left = 0;
				ttxsurf_rect.top = 0;
				ttxsurf_rect.right =  m_vSourceWidth;
				ttxsurf_rect.bottom =  m_vSourceHeight;
				m_pPresent->SetSurface(SUBTITLE_SURFACE,IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&ttxsurf_rect,0);

				DWORD dwAlpha = MAX_ALPHA_VAL/2;
				m_pPresent->SetSurface(SUBTITLE_SURFACE,IID_IMcVideoPresenter,MC_PROPID_VP_ALPHA,0,0,&dwAlpha,0);
			}


			BYTE buf[TTX_WIDTH*TTX_HEIGHT*3/2];
			fread(buf,sizeof(BYTE),sizeof(buf), fp);

			LPBYTE curr[3];
			curr[0]= buf;
			curr[1]= buf + TTX_WIDTH*TTX_HEIGHT;
			curr[2]= buf + TTX_WIDTH*TTX_HEIGHT*5/4;

			DWORD dwStrides[3];
			dwStrides[0] = TTX_WIDTH;
			dwStrides[1] = dwStrides[2] = TTX_WIDTH>>1;

			if(bV2Test)
			{
				if(m_pSubStream2)
				{
					hr = m_pSubStream2->Present(curr, dwStrides, 0);
					if(FAILED(hr))	goto RET;
					bool enable = true;
					hr = m_pSubStream2->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);
					if(FAILED(hr))	goto RET;
#ifndef NO_FEED_DATA
					hr = m_pPresentAdv->Render(0);
					if(FAILED(hr))	goto RET;
#endif
				}
			}
			else
			{
				if(m_pPresent)
					m_pPresent->Present(SUBTITLE_SURFACE,curr, dwStrides, 0);
			}
			bOsd2Enabled = TRUE;
RET:
			fclose(fp);

		}

	}
	else
	{
		if(bV2Test)
		{
			if(m_pSubStream2)
			{
				hr = m_pSubStream2->Close();
				if(FAILED(hr))	return;

				hr = m_pPresentAdv->DestroyStream(m_pSubStream2);
				if(FAILED(hr))	return;
				m_pSubStream2 = NULL;
			}
		}
		else
		{
			if(m_pPresent)
				m_pPresent->DestroySurface(SUBTITLE_SURFACE);
		}
		bOsd2Enabled = FALSE;
	}
#endif
}


HRESULT CLibVrPresent_TestDlg::TestV2(HMODULE& h_dll)
{
#ifndef TEST_V2_SURF
	return E_FAIL;
#endif
	HRESULT hr= m_pPresent->QueryInterface(IID_IMcVideoPresenterAdv, (void**)&m_pPresentAdv);
	if(FAILED(hr) || (m_pPresentAdv == NULL))
			printf("Do not support V2 interface!");
	 	    //MessageBox(_T("Do not support V2 interface!\n"), _T("MSG"), MB_OK);

	if(m_pPresentAdv)
	{
		VP_OpenOptionsAdv params;
		ZeroMemory(&params, sizeof(params));
		params.hWnd = m_wnd;
		params.canvasWidth = m_vSourceWidth;
		params.canvasHeight = m_vSourceHeight;
			
		hr = m_pPresentAdv->Open(&params, sizeof(params));
		if(FAILED(hr))
			return E_FAIL;

		VP_OPEN_STREAM_PARAMS sMainparams;
		memset(&sMainparams,0,sizeof(sMainparams));
		sMainparams.type = eMAINSTREAM;
		sMainparams.dwWidth = m_vSourceWidth;
		sMainparams.dwHeight = m_vSourceHeight;
		sMainparams.dwFourCC = MAKE_FOURCC('Y','V','1','2');
		sMainparams.fAlpha = 1.0f;
		hr = m_pPresentAdv->CreateStream(&sMainparams, sizeof(sMainparams),&m_pMainStream);
		if(FAILED(hr))
			return E_FAIL;

		hr = m_pMainStream->Open();
		if(FAILED(hr))	return E_FAIL;

		VP_DEINTERLACE_MODE  mode = VP_DEINTERLACE_BOB_SINGLE_FIELD;
		m_pPresentAdv->Set(IID_IMcVideoPresenter,MC_PROPID_VP_DEINTERLACE_MODE,0,0,&mode,0);
		
		RECT dst;
		SetRect(&dst, 0,0, m_vSourceWidth,m_vSourceHeight);
		m_pMainStream->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&dst,0);

		bool enable = true;
		m_pMainStream->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);

		VP_SCAN_MODE scanMode = VP_SCAN_INTERLACED_TOPFIELED_FIRST;
		m_pMainStream->Set(IID_IMcVideoPresenter, MC_PROPID_VP_SCAN_MODE,0,0,&scanMode,0);

/*	
		VP_OPEN_STREAM_PARAMS sSubparams;
		sSubparams.type = eSUBSTREAMS;
		sSubparams.dwWidth = m_vSourceWidth;
		sSubparams.dwHeight = m_vSourceHeight;
		sSubparams.dwFourCC = MAKE_FOURCC('N','V','1','2');
		sSubparams.fAlpha = 0.3f;
		hr = m_pPresentAdv->CreateStream(&sSubparams, sizeof(sSubparams),&m_pSubStream3);
		if(FAILED(hr))	return hr;

		hr = m_pSubStream3->Open();
		if(FAILED(hr))	return E_FAIL;
*/
		#ifdef TITAN_II
	typedef PVOID		VR_HANDLE;

		VR_HANDLE titian_hnd;
		hr = m_pMainStream->Get(IID_IMcVideoPresenter, MC_PROPID_VP_TITIANII_HND,0,0,&titian_hnd,0,0);
		if(FAILED(hr))	return hr;

		hr = m_pSubStream3->Set(IID_IMcVideoPresenter, MC_PROPID_VP_TITIANII_HND,0,0,titian_hnd,0);
		if(FAILED(hr))	return hr;
#endif
		SetRect(&dst, 0,0, m_vSourceWidth/2,m_vSourceHeight/2);
	//	m_pSubStream3->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&dst,0);

#if 0

		hr = m_pPresentAdv->CreateStream(&sSubparams, sizeof(sSubparams),&m_pSubStream4);
		if(FAILED(hr))	return hr;

		hr = m_pSubStream4->Open();
		if(FAILED(hr))	return E_FAIL;

		SetRect(&dst, m_vSourceWidth/2,0, m_vSourceWidth,m_vSourceHeight/2);
		m_pSubStream4->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&dst,0);

		m_pSubStream4->Set(IID_IMcVideoPresenter, MC_PROPID_VP_SCAN_MODE,0,0,&scanMode,0);

		hr = m_pPresentAdv->CreateStream(&sSubparams, sizeof(sSubparams),&m_pSubStream5);
		if(FAILED(hr))	return hr;


		hr = m_pSubStream5->Open();
		if(FAILED(hr))	return E_FAIL;

		SetRect(&dst, 0,m_vSourceHeight/2, m_vSourceWidth/2,m_vSourceHeight);
		m_pSubStream5->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&dst,0);

		hr = m_pPresentAdv->CreateStream(&sSubparams, sizeof(sSubparams),&m_pSubStream6);
		if(FAILED(hr))	return hr;

		hr = m_pSubStream6->Open();
		if(FAILED(hr))	return E_FAIL;

		SetRect(&dst, m_vSourceWidth/2,m_vSourceHeight/2, m_vSourceWidth,m_vSourceHeight);
		m_pSubStream6->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&dst,0);
#endif

		m_pPresent= NULL;
		return S_OK;
	}
		
	return E_FAIL;
}
