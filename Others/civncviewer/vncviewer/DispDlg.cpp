// DispDlg.cpp : implementation file
//

#include "stdafx.h"
#include "vncviewer.h"
#include "DispDlg.h"


// CDispDlg dialog

IMPLEMENT_DYNAMIC(CDispDlg, CDialog)

CDispDlg::CDispDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDispDlg::IDD, pParent), m_rfbClient(NULL)
{
	m_pPresenterAdv  = 0;
	m_pStream1 = 0;
}

CDispDlg::~CDispDlg()
{
}

void CDispDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDispDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDispDlg message handlers
void CDispDlg::SetVncServer(const char * serverAddr)
{
	m_serverAddr = serverAddr;
}

BOOL CDispDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if (Connect())
	{
		MoveWindow(0, 0, m_rfbClient->width, m_rfbClient->height);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

bool CDispDlg::Connect()
{
	if (m_rfbClient != NULL)
	{
		if (m_rfbClient->frameBuffer != NULL)
		{
			free(m_rfbClient->frameBuffer);
			m_rfbClient->frameBuffer = NULL;
		}
		rfbClientCleanup(m_rfbClient);
		m_rfbClient = NULL;
	}

	const int bitsPerSample = 8;
	const int samplesPerPixel = 3;
	const int bytesPerPixel = 4;

	m_rfbClient = rfbGetClient(bitsPerSample, samplesPerPixel, bytesPerPixel);
	if (m_rfbClient == NULL)
	{
		MessageBox(_T("Failed to initialize vnc client"));
	}

	m_rfbClient->format.redShift     = 16;  // strict to local RGB order. For Windows is BGRA
	m_rfbClient->format.greenShift   = 8;
	m_rfbClient->format.blueShift    = 0;

	m_rfbClient->format.redMax   = 255;
	m_rfbClient->format.greenMax = 255;
	m_rfbClient->format.blueMax  = 255;

	m_rfbClient->MallocFrameBuffer = FrameBufferResizeCb;
	m_rfbClient->canHandleNewFBSize = TRUE;
	m_rfbClient->GotFrameBufferUpdate = FrameBufferUpdateCb;
	//m_rfbClient->GotXCutText = got_cut_text;
	//m_rfbClient->HandleKeyboardLedState = kbd_leds;
	//m_rfbClient->HandleTextChat = text_chat;
	//m_rfbClient->GetPassword = get_password;

	rfbClientSetClientData(m_rfbClient, (void *)tagMain, this);

	char * argv[] = { "CidanaVncViewer", "-encodings", "tight", "-compress", "8", "192.168.0.81" };
	int argc = sizeof(argv) / sizeof(char *);
	rfbBool ret = rfbInitClient(m_rfbClient, &argc, argv);
	if (!ret)
	{
		if (m_rfbClient->frameBuffer != NULL)
		{
			free(m_rfbClient->frameBuffer);
			m_rfbClient->frameBuffer = NULL;
		}
		rfbClientCleanup(m_rfbClient);
		m_rfbClient = NULL;

		MessageBox(_T("Failed to connect to vnc server"));
		return false;
	}

	OutputDebugString(_T("Connect Okay\n"));
	SetTimer(1, 100, NULL);
	return true;
}

rfbBool CDispDlg::OnRfbResize(rfbClient *client)
{
	int frameBufferSize = client->width * client->height * client->format.bitsPerPixel/8;
	m_rfbClient->frameBuffer = (uint8_t *)malloc(frameBufferSize);
	memset(m_rfbClient->frameBuffer, 0, frameBufferSize);

	CString msg;
	msg.Format(_T("OnRfbResize: %d, %d\n"), client->width, client->height);
	OutputDebugString(msg);


	if(m_pStream1)
	{
		m_pStream1->Close();
		m_pStream1->Release();
		m_pStream1 = 0;
	}
	if(m_pPresenterAdv)
	{
		m_pPresenterAdv->Release();
		m_pPresenterAdv = 0;
	}


	TCHAR DirPath[MAX_PATH] = {0};
	TCHAR FilePath[MAX_PATH] = {0};
	// Get Tuners folder path
	GetModuleFileName(NULL, DirPath, MAX_PATH);
	TCHAR*	p = _tcsrchr(DirPath, '\\');
	p++;
	p[0] = 0;
	_tcscpy(FilePath, DirPath);
	_tcscat(FilePath, _T("LibVRPresent.dll"));
	HMODULE h_dll = LoadLibrary(FilePath); 
	fpCreateVideoPresenterAdv fpCreate = (fpCreateVideoPresenterAdv)GetProcAddress(h_dll, ENTRY_FUNC_NAME_V2);
	if(fpCreate == 0)
		return false;

	HRESULT hr = S_OK;
	m_pPresenterAdv = fpCreate();
	if(m_pPresenterAdv == 0)
		return false;

	VP_OpenOptionsAdv option;
	memset(&option, 0, sizeof(option));
	hr = m_pPresenterAdv->Open(&option, sizeof(option));
	if(FAILED(hr)) return -1;
	hr = m_pPresenterAdv->Get(IID_IMcVideoPresenter, MC_PROPID_VP_CANVAS_INFO,
	             0, 0, &m_canvasInfo, sizeof(m_canvasInfo),0);
	if(FAILED(hr)) return -1;

	bool bEnable = true;
	VP_ZOOM_MODE mode = VP_ZOOM_MODE_LETTERBOX;

	VP_OPEN_STREAM_PARAMS stream_parms;
	memset(&stream_parms, 0, sizeof(stream_parms));
	stream_parms.type = eMAINSTREAM;
	stream_parms.dwWidth = client->width;
	stream_parms.dwHeight = client->height;
	stream_parms.dwFourCC = 'X'|'R'<<8|'G'<<16|'B'<<24;
	stream_parms.fAlpha = 1.0f;
	hr = m_pPresenterAdv->CreateStream(&stream_parms, sizeof(stream_parms), &m_pStream1);
	if(FAILED(hr)) return -1;

	hr = m_pStream1->Open();
	if(FAILED(hr)) return -1;

	RECT dstRect1;
	SetRect(&dstRect1, 0, 0, m_canvasInfo.dwWidth-10, m_canvasInfo.dwHeight-10);
	m_pStream1->Set(IID_IMcVideoPresenter, MC_PROPID_VP_RECTDST, 0, 0, &dstRect1, sizeof(dstRect1));

	m_pStream1->Set(IID_IMcVideoPresenter, MC_PROPID_VP_ENABLE, 0, 0, &bEnable, sizeof(bEnable));
	m_pStream1->Set(IID_IMcVideoPresenter, MC_PROPID_VP_ZOOM, NULL, NULL,&mode,sizeof(mode));

	return TRUE;
}

void CDispDlg::OnRfbUpdate(rfbClient *client, int x, int y, int w, int h)
{
//	CString msg;
//	msg.Format(_T("OnRfbUpdate: %d, %d, %d, %d\n"), x, y, w, h);
//	OutputDebugString(msg);

#if 0
	FILE * file = fopen("rfbdump.bin", "wb");
	if (file != NULL)
	{
		fwrite(client->frameBuffer, 1, m_rfbClient->width * m_rfbClient->height * (m_rfbClient->format.bitsPerPixel / 8), file);
		fclose(file);
	}
#else

long startTick = GetTickCount();
	
    unsigned char* pBuffArr[3];
    pBuffArr[0] = m_rfbClient->frameBuffer;
    DWORD strides[3] = {m_rfbClient->width* (m_rfbClient->format.bitsPerPixel/8),
		m_rfbClient->width* (m_rfbClient->format.bitsPerPixel/8),
		m_rfbClient->width* (m_rfbClient->format.bitsPerPixel/8)};

    if (m_pStream1) {
        HRESULT hr = m_pStream1->Present(pBuffArr, strides, 0);
        if(FAILED(hr))
            printf("error :%d \n",__LINE__);
    }
	TRACE(_T("cost:%d \n"), GetTickCount() - startTick);

	/*
	CBitmap bitmap;
	bitmap.CreateBitmap(m_rfbClient->width, m_rfbClient->height, 1, m_rfbClient->format.bitsPerPixel, m_rfbClient->frameBuffer);
	CDC * dispDc = GetDC();
	CDC memDc;
	memDc.CreateCompatibleDC(dispDc);
	CBitmap * oldBitmap = memDc.SelectObject(&bitmap);
	CRect dispRect;
	GetClientRect(dispRect);
	dispDc->BitBlt(x, y, w, h, &memDc, x, y, SRCCOPY);
	memDc.SelectObject(oldBitmap);

*/	
#endif
}

BOOL CDispDlg::DestroyWindow()
{

	if(m_pStream1)
	{
		m_pStream1->Close();
		m_pStream1->Release();
		m_pStream1 = 0;
	}
	if(m_pPresenterAdv)
	{
		m_pPresenterAdv->Release();
		m_pPresenterAdv = 0;
	}

	if (m_rfbClient != NULL)
	{
		if (m_rfbClient->frameBuffer != NULL)
		{
			free(m_rfbClient->frameBuffer);
			m_rfbClient->frameBuffer = NULL;
		}
		rfbClientCleanup(m_rfbClient);
		m_rfbClient = NULL;
	}
	return CDialog::DestroyWindow();
}

void CDispDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		KillTimer(nIDEvent);
		while (1)
		{
			int ret = WaitForMessage(m_rfbClient, 10);
			if (ret < 0)
			{
				OutputDebugString(_T("WaitForMessage ret negitive value!\n"));
				break;
			}
			else if (ret > 0)
			{
				if (!HandleRFBServerMessage(m_rfbClient))
				{
					OutputDebugString(_T("HandleRFBServerMessage failure!\n"));
					break;
				}
			}
			else  // timeout
			{
				break;
			}
		}
		SetTimer(1, 100, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}

void CDispDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_rfbClient != NULL)
	{
		SendPointerEvent(m_rfbClient, point.x, point.y, 1);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDispDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	OnOK();
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CDispDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
}
