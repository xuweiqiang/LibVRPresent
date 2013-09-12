//////////////////////////////////////////////////////////////////////
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) CIDANA Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////

#include "GfxDxvaDXVAHD_Sample.h"
#include "GfxDxvaApplication.h"
//#include "resource.h"
#include <new>

//HWND InitializeWindow(LPVOID lpParam);
extern RECT ScaleRectangle(const RECT& input, const RECT& src, const RECT& dst);

void DisplayError(HWND hwnd, WCHAR *pszMessage)
{
   // MessageBox(hwnd, pszMessage, L"Error", MB_OK | MB_ICONERROR);
}

static CDXVAApplication* g_pDXVAApp = NULL;
LRESULT CALLBACK HookFunc(int code, WPARAM wParam, LPARAM lParam)
{
	LRESULT hr = 0;
	CWPSTRUCT msg = *( CWPSTRUCT *)lParam;
	if(g_pDXVAApp)
	{
		g_pDXVAApp->HandleMessage(msg.hwnd, LOWORD(msg.message), msg.wParam, msg.lParam);
		 if(code < 0)
			 hr = CallNextHookEx(g_pDXVAApp->m_hHook, code,wParam, lParam);
	}
	return hr;
}

//-------------------------------------------------------------------
// Initialize
//
// Called once at the start of the CDXVAApplication.
//-------------------------------------------------------------------
D3DFORMAT CDXVAApplication::VIDEO_MAIN_FORMAT          = D3DFMT_YUY2;
D3DFORMAT CDXVAApplication::VIDEO_SUB_FORMAT           = D3DFORMAT('VUYA'); // AYUV

// Primary stream
UINT CDXVAApplication::DEST_BORDER = 0;
//RECT CDXVAApplication::VIDEO_MAIN_DEST_RECT = { DEST_BORDER, DEST_BORDER, VIDEO_MAIN_WIDTH - DEST_BORDER, VIDEO_MAIN_HEIGHT - DEST_BORDER }; 

// Substreams
UINT CDXVAApplication::SUB_STREAM_COUNT  = 0;

UINT CDXVAApplication::VIDEO_SUB_WIDTH   = 128;
UINT CDXVAApplication::VIDEO_SUB_HEIGHT  = 128;

UINT CDXVAApplication::VIDEO_SUB_SURF_WIDTH = VIDEO_SUB_WIDTH * 3;
UINT CDXVAApplication::VIDEO_SUB_SURF_HEIGHT = VIDEO_SUB_HEIGHT * 3;
RECT CDXVAApplication::VIDEO_SUB_RECT = { VIDEO_SUB_WIDTH, VIDEO_SUB_HEIGHT };

CDXVAApplication::CDXVAApplication(HWND wnd) :
    m_pDXVAHD(NULL),
    m_pDXVAVP(NULL),
    m_pMainStream(NULL),
    m_PlanarAlphaValue(DEFAULT_PLANAR_ALPHA_VALUE),
    m_PixelAlphaValue(DEFAULT_PIXEL_ALPHA_VALUE),
    m_usage(DXVAHD_DEVICE_USAGE_PLAYBACK_NORMAL),
    m_Hwnd(wnd),
    m_bEnable(TRUE),
    m_bCanvasInit(false)
{
	ZeroMemory(m_ppSubStream, sizeof( m_ppSubStream) );

	ZeroMemory(&m_Filters, sizeof(m_Filters));
	ZeroMemory(&m_streamInfo, sizeof(m_streamInfo));
	ZeroMemory(&m_canvasInfo, sizeof(m_canvasInfo));
	ResetSettings();
	g_pDXVAApp = this;
}

CDXVAApplication::~CDXVAApplication()
{
	g_pDXVAApp = NULL;
	if(m_hHook)
		UnhookWindowsHook(WH_CALLWNDPROC, HookFunc);
    DestroyDXVAHD();
}


HRESULT CDXVAApplication::Initialize()
{
	if (!m_DWM.Initialize())
	{
		DisplayError(NULL, L"Unable to initialize DWM.");
		return E_FAIL;
	}

	if (!m_Hwnd)
	{
		DisplayError(NULL, L"Unable to create CDXVAApplication window.");
		return E_FAIL;
	}

	m_hHook = SetWindowsHook(WH_CALLWNDPROC, HookFunc);
	if(!m_hHook)
	{
		int ret = GetLastError();
		if(ret !=0)	return E_FAIL;
	}

	if (!m_D3D.InitializeD3D9(m_Hwnd))
	{
		DisplayError(NULL, L"Unable to create Direct3D device.");
		return E_FAIL;
	}

	if (!InitializeDXVAHD())
	{
		DisplayError(NULL, L"Unable to create DXVA-HD device.");
		return E_FAIL;
	}

	if (!m_timer.InitializeTimer(VIDEO_MSPF))
	{
		DisplayError(NULL, L"Unable to initialize timer.");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CDXVAApplication::UnInitialize()
{
	DestroyDXVAHD();
	return S_OK;
}

//-------------------------------------------------------------------
// ProcessVideoFrame
//
// Process and render the next video frame.
//-------------------------------------------------------------------

BOOL CDXVAApplication::ProcessVideoFrame()
{
	HRESULT hr = S_OK;

	if (!m_D3D.m_pD3D || !m_pDXVAVP)
		return FALSE;

	if(!m_bEnable)	return S_OK;

	RECT client;
	GetClientRect(m_Hwnd, &client);

	if (IsRectEmpty(&client))
		return TRUE;

	// Check the current status of D3D9 device.  
	hr = m_D3D.TestCooperativeLevel();

	switch (hr)
	{
		case D3D_OK :
			break;

		case D3DERR_DEVICELOST :
			return TRUE;

		case D3DERR_DEVICENOTRESET :
			return FALSE;
			break;

		default :
			return FALSE;
	}

	IDirect3DSurface9 *pRT = NULL;  // Render target

	DWORD frame = m_timer.GetFrameNumber();

	RECT dest = { 0 };
	RECT ssdest = { 0 };
	DXVAHD_STREAM_DATA stream_data[MAX_STREAM_NUM];
	ZeroMemory(&stream_data, sizeof(stream_data));

	// Get the render-target surface.

	hr = m_D3D.m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pRT);

	if (FAILED(hr)) { goto done; }

	// Initialize the stream data structures for the primary video stream 
	// and the substream.

	UINT streamCount= 0;
	if(m_pMainStream)
	{
		stream_data[streamCount].Enable = TRUE;
		stream_data[streamCount].OutputIndex = 0;
		stream_data[streamCount].InputFrameOrField = frame;
		stream_data[streamCount].pInputSurface = m_pMainStream;
		streamCount++;
	}

	for(int index =0;index<sizeof(m_streamInfo)/sizeof(SSTREAM_INFO);index++)
	{
		if(m_streamInfo[index].valid && (m_streamInfo[index].m_bEnable))
		{
			stream_data[streamCount].Enable = TRUE;
			stream_data[streamCount].OutputIndex = 0;
			stream_data[streamCount].InputFrameOrField = frame;
			stream_data[streamCount].pInputSurface = m_streamInfo[index].m_pStream;
			streamCount++;
		}
	}

	AdjustTargetRect(0, 0);

	UpdateVideoSubRect();

	// Scale the destination rectangle to the window client area.
	dest = ScaleRectangle(m_rcMainVideoDestRect, m_rcMainVideoSourceRect, client);

	UINT pStreamId = 0 ;
	hr = DXVAHD_SetDestinationRect(
	m_pDXVAVP,
	pStreamId++,
	TRUE,
	dest
	);
	if (FAILED(hr)) { goto done; }

	DWORD back_frame = frame;
	for(int index =0;index<sizeof(m_streamInfo)/sizeof(SSTREAM_INFO);index++)
	{
		if((m_streamInfo[index].valid))
		{
			// Scale to the window client area.
			ssdest = ScaleRectangle(m_streamInfo[index].m_rectDst, m_rcMainVideoSourceRect, client); 
			hr = DXVAHD_SetDestinationRect(
				m_pDXVAVP,
				pStreamId++,
				TRUE,
				ssdest
				);
			if (FAILED(hr)) { goto done; }
		}
	}

	// Color-fill the render target if the target rectangle is less
	// than the entire render target.

	hr = m_D3D.m_pDevice->ColorFill(pRT, NULL, D3DCOLOR_XRGB(0, 0, 0));
	if (FAILED(hr)) { goto done; }

	// Perform the blit.
	hr = m_pDXVAVP->VideoProcessBltHD(pRT,frame,streamCount,stream_data);
	if (FAILED(hr)) goto done; 

	// Enable DWM queuing.
	hr = m_DWM.EnableDwmQueuing(m_Hwnd);
	if (FAILED(hr)) { goto done; }


	// Present the frame.
	hr = m_D3D.m_pDevice->Present(NULL, NULL, NULL, NULL);

	done:
	SafeRelease(&pRT);

	return SUCCEEDED(hr);
}



//-------------------------------------------------------------------
// ResetDevice
//
// Resets the Direct3D device.
//-------------------------------------------------------------------

BOOL CDXVAApplication::ResetDevice(BOOL bChangeWindowMode)
{
	// Destroy the DXVA-HD device, because it may be holding D3D9 resources.
	DestroyDXVAHD();

	// Reset the Direct3D device and re-create the DXVA-HD device.
	if (m_D3D.ResetDevice(bChangeWindowMode) && InitializeDXVAHD())
	{
	return TRUE;
	}

	// Either resetting the Direct3D device failed, or initializing
	// the DXVA-HD device failed.

	// Try to recover by recreating the devices from the scratch.
	DestroyDXVAHD();
	if (m_D3D.InitializeD3D9(m_Hwnd) && InitializeDXVAHD())
	{
	return TRUE;
	}


	// Still failed. 
	// If we failed to initialize in fullscreen mode, try falling
	// back to windowed mode.
	DestroyDXVAHD();
	if (!m_D3D.m_bWindowed)
	{
		if (m_D3D.SetWindowedMode(TRUE) && m_D3D.InitializeD3D9(m_Hwnd) && InitializeDXVAHD())
		{
			return TRUE;
		}
	}

	// Still failed. Give up.
	return FALSE;
};


//-------------------------------------------------------------------
// InitializeDXVAHD
//
// Create the DXVA-HD video processor device, and initialize
// DXVA-HD resources.
//-------------------------------------------------------------------

BOOL CDXVAApplication::InitializeDXVAHD()
{
	HRESULT hr = S_OK;
	DWORD index = 0;

	D3DFORMAT *pFormats = NULL;
	DXVAHD_VPCAPS *pVPCaps = NULL;

	DXVAHD_CONTENT_DESC desc;

	desc.InputFrameFormat = m_canvasInfo.format;
	desc.InputFrameRate.Numerator = VIDEO_FPS;
	desc.InputFrameRate.Denominator = 1;
	desc.InputWidth = m_canvasInfo.open_params.dwWidth;
	desc.InputHeight = m_canvasInfo.open_params.dwHeight;
	desc.OutputFrameRate.Numerator = VIDEO_FPS;
	desc.OutputFrameRate.Denominator = 1;
	desc.OutputWidth = m_canvasInfo.open_params.dwWidth;
	desc.OutputHeight = m_canvasInfo.open_params.dwHeight;

	PDXVAHDSW_Plugin pSWPlugin = NULL;

	if (m_D3D.m_bDXVA_SW)
	{
		// Load the software DXVA-HD device.

		HMODULE hSWPlugin = LoadLibrary(L"dxvahdsw.dll");
		if (hSWPlugin == NULL)
		{
			//DBGMSG(L"Could not load dxvahdsw.dll\n");
			return FALSE;
		}

		pSWPlugin = (PDXVAHDSW_Plugin)GetProcAddress(hSWPlugin, "DXVAHDSW_Plugin");
		if (pSWPlugin == NULL)
		{
			//DBGMSG(L"Could not get DXVAHDSW_Plugin proc address.\n");
			return FALSE;
		}
	}

	// Create the DXVA-HD device.
	hr = DXVAHD_CreateDevice(
	m_D3D.m_pDevice,
	&desc,
	m_usage,
	pSWPlugin,
	&m_pDXVAHD
	);
	if (FAILED(hr)) { goto done; }

	// Get the DXVA-HD device caps.
	ZeroMemory(&m_caps, sizeof(m_caps));
	hr = m_pDXVAHD->GetVideoProcessorDeviceCaps(&m_caps);
	if (FAILED(hr)) { goto done; }

	if (m_caps.MaxInputStreams < 1 + SUB_STREAM_COUNT)
	{
		//DBGMSG(L"Device only supports %d input streams.\n", m_caps.MaxInputStreams);
		hr = E_FAIL;
		goto done;
	}

	// Check the output format.
	pFormats = new (std::nothrow) D3DFORMAT[ m_caps.OutputFormatCount ];
	if (pFormats == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto done;
	}

	hr = m_pDXVAHD->GetVideoProcessorOutputFormats(m_caps.OutputFormatCount, pFormats);
	if (FAILED(hr)) { goto done; }
	for (index = 0; index < m_caps.OutputFormatCount; index++)
	{
		if (pFormats[index] == VIDEO_RENDER_TARGET_FORMAT)
		{
			break;
		}
	}
	if (index == m_caps.OutputFormatCount)
	{
	hr = E_FAIL;
	goto done;
	}
	delete [] pFormats;
	pFormats = NULL;


	// Check the input formats.
	pFormats = new (std::nothrow) D3DFORMAT[ m_caps.InputFormatCount ];
	if (pFormats == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto done;
	}

	hr = m_pDXVAHD->GetVideoProcessorInputFormats(m_caps.InputFormatCount, pFormats);
	if (FAILED(hr)) { goto done; }

	for(int i=0;i<sizeof(m_streamInfo)/sizeof(SSTREAM_INFO);i++)
	{
		if(!m_streamInfo[i].valid)
			continue;
		for (index = 0; index < m_caps.InputFormatCount; index++)
		{
			if (pFormats[index] == m_streamInfo[i].open_params.dwFourCC)
			{
			    break;
			}
		}
		if (index == m_caps.InputFormatCount)
		{
			ZeroMemory(&m_streamInfo[i], sizeof(SSTREAM_INFO));
			m_streamInfo[i].valid = false;
			hr = E_FAIL;
			goto done;
		}
	}

	delete [] pFormats;
	pFormats = NULL;

	// Create the VP device.
	pVPCaps = new (std::nothrow) DXVAHD_VPCAPS[ m_caps.VideoProcessorCount ];
	if (pVPCaps == NULL)
	{
		hr = E_OUTOFMEMORY;
		goto done;
	}

	hr = m_pDXVAHD->GetVideoProcessorCaps(m_caps.VideoProcessorCount, pVPCaps);
	if (FAILED(hr)) { goto done; }

	hr = m_pDXVAHD->CreateVideoProcessor(&pVPCaps[0].VPGuid, &m_pDXVAVP);
	if (FAILED(hr)) { goto done; }

	// Create the video surface for the primary video stream.
	hr = m_pDXVAHD->CreateVideoSurface(
		m_canvasInfo.open_params.dwWidth,
		m_canvasInfo.open_params.dwHeight,
		(D3DFORMAT)m_canvasInfo.open_params.dwFourCC,
		m_caps.InputPool,
		0,	//MainStream id
		DXVAHD_SURFACE_TYPE_VIDEO_INPUT,
		1,
		&m_pMainStream,
		NULL
		);
	if (FAILED(hr)) { goto done; }

	//Save main info
	m_canvasInfo.m_pStream = m_pMainStream;

	// Set the initial stream states for the primary stream.
	hr = DXVAHD_SetStreamFormat(m_pDXVAVP, 0, (D3DFORMAT)m_canvasInfo.open_params.dwFourCC);
	if (FAILED(hr)) { goto done; }

	hr = DXVAHD_SetFrameFormat(m_pDXVAVP, 0, m_canvasInfo.format);
	if (FAILED(hr)) { goto done; }

	hr = CreateSubStream();
	if (FAILED(hr)) { goto done; }

	// Load the bitmap onto the substream surface.

	//hr = LoadBitmapResourceToAYUVSurface(
	//    m_ppSubStream[0],
	//    VIDEO_SUB_SURF_WIDTH,
	//    VIDEO_SUB_SURF_HEIGHT,
	//    IDB_BITMAP1,
	//    m_PixelAlphaValue
	//    );

	//if (FAILED(hr)) { goto done; }

	// Get the image filtering capabilities.

	for (DWORD i = 0; i < NUM_FILTERS; i++)
	{
		if (m_caps.FilterCaps & (1 << i))
		{
			m_Filters[i].bSupported = TRUE;
			m_pDXVAHD->GetVideoProcessorFilterRange(PROCAMP_FILTERS[i], &m_Filters[i].Range);
			m_Filters[i].CurrentValue = m_Filters[i].Range.Default;
			INT range = m_Filters[i].Range.Maximum - m_Filters[i].Range.Minimum;
			m_Filters[i].Step = range > 32 ? range / 32 : 1;
		}
		else
		{
			m_Filters[i].bSupported = FALSE;
		}
	}

	// Apply the current settings.
	hr = ApplySettings();
	if (FAILED(hr)) { goto done; }

	hr = UpdateVideoSubRect();

	done:
	delete [] pFormats;
	delete [] pVPCaps;
	return (SUCCEEDED(hr));
}

//-------------------------------------------------------------------
// DestroyDXVAHD
//
// Releases DXVA-HD resources.
//-------------------------------------------------------------------

void CDXVAApplication::DestroyDXVAHD()
{
	SafeRelease(&m_pMainStream);

	for (DWORD i = 0; i < SUB_STREAM_COUNT; i++)
	{
		SafeRelease(&m_ppSubStream[i]);
	}

	SafeRelease(&m_pMainStream);

	SafeRelease(&m_pDXVAVP);
	SafeRelease(&m_pDXVAHD);
}


//-------------------------------------------------------------------
// MessageLoop
//
// Runs the message loop for the CDXVAApplication window.
//
// This method also waits on the timer the controls the frame
// rate for the video.
//-------------------------------------------------------------------

INT CDXVAApplication::MessageLoop()
{
	MSG msg = {0};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (PreTranslateMessage(msg))
			{
				continue;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		// Wait until the timer expires or any message is posted.

		if (WAIT_OBJECT_0 == MsgWaitForMultipleObjects(
			1,
			&m_timer.Handle(),
			FALSE,
			INFINITE,
			QS_ALLINPUT
			))
		{
			// Draw the next video frame.
			if (!ProcessVideoFrame())
			{
				DestroyWindow(m_Hwnd);
			}
		}
	}

    return INT(msg.wParam);
}


//-------------------------------------------------------------------
// HandleMessage
//
// Handles window messages.
//-------------------------------------------------------------------

LRESULT CDXVAApplication::HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(hwnd != m_Hwnd)
		return S_OK;

	switch (uMsg)
	{
		case WM_SIZE:
		OnSize();
		return 0L;
	}        
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


//-------------------------------------------------------------------
// OnSize
//
// Handles WM_SIZE messages.
//-------------------------------------------------------------------

void CDXVAApplication::OnSize()
{
	if (m_D3D.m_pDevice == NULL)
	{
		return;
	}

	RECT rect;
	GetClientRect(m_Hwnd, &rect);

	if (IsRectEmpty(&rect))
	{
		return;
	}

	// Do not reset the device while the mode change is in progress.
	if (m_D3D.m_bInModeChange)
	{
		return;
	}

	if (!m_D3D.ResetDevice(FALSE))
	{
		DestroyWindow(m_Hwnd);
		return;
	}

	InvalidateRect(m_Hwnd, NULL, FALSE);
}


//-------------------------------------------------------------------
// PreTranslateMessage
//
// Handles window messages before they are sent to TranslateMessage. 
//-------------------------------------------------------------------

BOOL CDXVAApplication::PreTranslateMessage(const MSG& msg)
{
	// Only interested in Alt + Enter.
	if (msg.message != WM_SYSKEYDOWN || msg.wParam != VK_RETURN)
	{
		return FALSE;
	}

	if (!m_D3D.m_pDevice)
	{
		return TRUE;
	}

	RECT rect;
	GetClientRect(msg.hwnd, &rect);

	if (IsRectEmpty(&rect))
	{
		return TRUE;
	}

	// Toggle the window mode (windowed/full screen)
	if (ResetDevice(TRUE))
	{
		return TRUE;
	}

	DestroyWindow(msg.hwnd);
	return TRUE;
}


//-------------------------------------------------------------------
// UpdateVideoSubRect
//
// The CDXVAApplication displays a sub-rectangle from the substream
// surface. The substream rectangle is calculated from the current
// mode. (The user can switch modes using the function keys.)
//
// As a result, each mode displays a different portion of the 
// substream bitmap.
//-------------------------------------------------------------------

HRESULT CDXVAApplication::UpdateVideoSubRect()
{
    LONG dx = 0, dy = 0;
    HRESULT hr = S_OK;

    if (m_pDXVAVP)
    	{
		UINT iStreamCount = 1;
		for(int i=0;i<sizeof(m_streamInfo)/sizeof(SSTREAM_INFO);i++)
		{
			if(m_streamInfo[i].valid)
			{
			        hr = DXVAHD_SetSourceRect(
			            m_pDXVAVP,
			            iStreamCount,
			            TRUE,
			            m_streamInfo[i].m_rectSrc
			            );
				if(FAILED(hr))	return hr;
				iStreamCount++;
			}
		}
    	}
    return hr;

}


HRESULT CDXVAApplication::ApplySettings()
{
	HRESULT hr = S_OK;

	if (m_pDXVAVP)
	{
		// Planar alpha
		hr = DXVAHD_SetPlanarAlpha(m_pDXVAVP, 0, TRUE, float(m_PlanarAlphaValue) / 0xFF);
		if (FAILED(hr)) { goto done; }


		if(1)
		{
			// Main video source rectangle.
			hr = DXVAHD_SetSourceRect(
				m_pDXVAVP,
				0,
				TRUE,
				m_rcMainVideoSourceRect
				);
			if (FAILED(hr)) { goto done; }

			// Main video destination rectangle.
			hr = DXVAHD_SetDestinationRect(
				m_pDXVAVP,
				0,
				TRUE,
				m_rcMainVideoDestRect
				);
			if (FAILED(hr)) { goto done; }

			// Target rectangle.
			hr = AdjustTargetRect(0, 0);
			if (FAILED(hr)) { goto done; }
		}

		// Extended color info.
		hr = AdjustExtendedColor(0);
		if (FAILED(hr)) { goto done; }

		// Image filters

		for (DWORD i = 0; i < NUM_FILTERS; i++)
		{
			hr = AdjustFilter((DXVAHD_FILTER)i);
			if (FAILED(hr)) { goto done; }
		}

		// Background color.
		hr = AdjustBackgroundColor(eCOLOR_BLACK);

	}

done:
    return hr;

}


//-------------------------------------------------------------------
// ResetSettings
//
// Restores all user-controlled settings to their original state.
//-------------------------------------------------------------------

HRESULT CDXVAApplication::ResetSettings()
{
	HRESULT hr = S_OK;

	ZeroMemory(&m_streamInfo, sizeof(m_streamInfo));
	int index = 0;
	for(index =0;index<sizeof(m_streamInfo)/sizeof(SSTREAM_INFO);index++)
	{
		m_streamInfo[index].format = DXVAHD_FRAME_FORMAT_PROGRESSIVE;
	}
	ZeroMemory(&m_caps, sizeof(m_caps));
	ZeroMemory(&m_canvasInfo, sizeof(m_canvasInfo));

	m_PixelAlphaValue = DEFAULT_PIXEL_ALPHA_VALUE;
	m_PlanarAlphaValue = DEFAULT_PLANAR_ALPHA_VALUE;
	m_iExtendedColor = 0;
	m_iBackgroundColor = 0;

	m_TargetWidthPercent = 100;
	m_TargetHeightPercent = 100;


	for (DWORD i = 0; i < NUM_FILTERS; i++)
	{
		m_Filters[ i ].CurrentValue = m_Filters[i].Range.Default;
	}

	// Substream pixel alpha
	//if (m_ppSubStream[0])
	//{
	//    hr = SetAYUVSurfacePixelAlpha(
	//        m_ppSubStream[0],
	//        VIDEO_SUB_SURF_WIDTH,
	//        VIDEO_SUB_SURF_HEIGHT,
	//        m_PixelAlphaValue
	//        );

	//    if (FAILED(hr)) { goto done; }
	//}
#if 0
	hr = ApplySettings();

	if (FAILED(hr)) { goto done; }

	if (m_pDXVAVP)
	{
	// Disable target rectangles

	RECT rc = { 0 };

	hr = DXVAHD_SetTargetRect(m_pDXVAVP, FALSE, rc);
	if (FAILED(hr)) { goto done; }
	}
#endif
	done:
	return hr;
}


//-------------------------------------------------------------------
// AdjustAlphaSetting
//
// Adjusts the planar alpha for the primary video stream, or the
// per-pixel alpha for the substream.
//-------------------------------------------------------------------

HRESULT CDXVAApplication::AdjustAlphaSetting(int id, float* pAlpha)
{
	HRESULT hr = S_OK;
	if((id>= MAX_STREAM_NUM -1)||(!m_streamInfo[id].valid) || !pAlpha)	return hr;

	m_streamInfo[id].fAlpha = *pAlpha;
        hr = DXVAHD_SetPlanarAlpha(m_pDXVAVP, id+1, TRUE, m_streamInfo[id].fAlpha );
	return hr;


#if 0
    if (dy != 0)
    {
        // Adjust planar alpha
        if (dy > 0)
        {
            m_PlanarAlphaValue = min(m_PlanarAlphaValue + 8, 0xFF);
        }
        else
        {
            m_PlanarAlphaValue = m_PlanarAlphaValue < 8 ? 0 : m_PlanarAlphaValue - 8;
        }
        hr = DXVAHD_SetPlanarAlpha(m_pDXVAVP, 0, TRUE, float(m_PlanarAlphaValue) / 0xFF);

        if (FAILED(hr)) { goto done; }

    }

    if (dx != 0)
    {
        // Adjust per-pixel alpha. This is done by writing new values to 
        // the video surface.

        if (dx > 0)
        {
            m_PixelAlphaValue = min(m_PixelAlphaValue + 8, 0xFF);
        }
        else 
        {
            m_PixelAlphaValue = m_PixelAlphaValue < 8 ? 0 : m_PixelAlphaValue - 8;      
        }
        
        //hr = SetAYUVSurfacePixelAlpha(
        //    m_ppSubStream[0],
        //    VIDEO_SUB_SURF_WIDTH,
        //    VIDEO_SUB_SURF_HEIGHT,
        //    m_PixelAlphaValue
        //    );
    }
done:
    return hr;
#endif
}


//-------------------------------------------------------------------
// AdjustExtendedColor
//
// Changes the extended color information.
//-------------------------------------------------------------------

HRESULT CDXVAApplication::AdjustExtendedColor(int dy)
{
	// Cycle through extended color settings

	HRESULT hr = S_OK;
	if (dy > 0)
	{
		if (++m_iExtendedColor > NUM_EX_COLORS - 1)
		{
			m_iExtendedColor = 0;
		}
	}
	else if (dy < 0)
	{
		if (--m_iExtendedColor < 0)
		{
			m_iExtendedColor = NUM_EX_COLORS - 1;
		}
	}

	// Output color space.
	hr = DXVAHD_SetOutputColorSpace(
		m_pDXVAVP,
		TRUE,       // Playback
		EX_COLOR_INFO[m_iExtendedColor].bRgbRange16_235,
		EX_COLOR_INFO[m_iExtendedColor].bBT709,
		0
		);
	if (FAILED(hr)) { goto done; }

	// Input color space
	for (DWORD i = 0; i < 1 + SUB_STREAM_COUNT; i++)
	{
		hr = DXVAHD_SetInputColorSpace(
			m_pDXVAVP,
			i,
			TRUE,       // Playback
			EX_COLOR_INFO[m_iExtendedColor].bRgbRange16_235,
			EX_COLOR_INFO[m_iExtendedColor].bBT709,
			0
			);
		if (FAILED(hr)) 
			break; 
	}

	done:
	return hr;
}


//-------------------------------------------------------------------
// AdjustBackgroundColor
//
// Changes the background color.
//-------------------------------------------------------------------

HRESULT CDXVAApplication::AdjustBackgroundColor(eBACK_COLOR color)
{
	HRESULT hr = S_OK;

	DXVAHD_COLOR clr;

	clr.RGB = BACKGROUND_COLORS[ color ];

	hr = DXVAHD_SetBackgroundColor(
		m_pDXVAVP,
		FALSE,       // YCbCr?
		clr
		);

	return hr;
}


//-------------------------------------------------------------------
// AdjustFilter
//
// Changes one of the ProcAmp filter values.
//-------------------------------------------------------------------

HRESULT CDXVAApplication::AdjustFilter(DXVAHD_FILTER filter)
{
	HRESULT hr = S_OK;

	if (filter > NUM_FILTERS)
	{
		return E_UNEXPECTED;
	}

	if (!m_Filters[ filter ].bSupported)
	{
		return S_OK; // Unsupported filter. Ignore.
	}

	const INT step = m_Filters[ filter ].Step;
	const INT minimum = m_Filters[ filter ].Range.Minimum;
	const INT maximum = m_Filters[ filter ].Range.Maximum;
	INT val = m_Filters[ filter ].CurrentValue;
	if (val >= minimum && val <= maximum)
	{
		hr = DXVAHD_SetFilterValue(
			m_pDXVAVP,
			0, 
			filter,
			TRUE,
			val
			);

		if (FAILED(hr))
		{
			// Try the default.
			val = m_Filters[ filter ].Range.Default;
			hr = DXVAHD_SetFilterValue(
				m_pDXVAVP,
				0, 
				filter,
				TRUE,
				val
				);
		}

		if (SUCCEEDED(hr))
		{
			m_Filters[ filter ].CurrentValue = val;
		}

	}

	return hr;
}


//-------------------------------------------------------------------
// AdjustTargetRect
//
// Changes the target rectangle. 
//-------------------------------------------------------------------

HRESULT CDXVAApplication::AdjustTargetRect(int dx, int dy)
{
    HRESULT hr = S_OK;

    if (dy > 0)
    {
        m_TargetHeightPercent = min(100, m_TargetHeightPercent + 4);
    }
    else if (dy < 0)
    {
        m_TargetHeightPercent = max(0, m_TargetHeightPercent - 4);
    }

    if (dx > 0)
    {
        m_TargetWidthPercent = min(100, m_TargetWidthPercent + 4);
    }
    else if (dx < 0)
    {
        m_TargetWidthPercent = max(0, m_TargetWidthPercent - 4);
    }

    RECT client;
    RECT target;

    GetClientRect(m_Hwnd, &client);

    target.left   = client.left;//   + (client.right  - client.left) / 2 * (100 - m_TargetWidthPercent)  / 100;
    target.right  = client.right;//  - (client.right  - client.left) / 2 * (100 - m_TargetWidthPercent)  / 100;
    target.top    = client.top  ;//  + (client.bottom - client.top)  / 2 * (100 - m_TargetHeightPercent) / 100;
    target.bottom = client.bottom ;//- (client.bottom - client.top)  / 2 * (100 - m_TargetHeightPercent) / 100;

    hr = DXVAHD_SetTargetRect(m_pDXVAVP, TRUE, target);

    return hr;
}


//-------------------------------------------------------------------
// CalculateSubstreamRect
//
// Calculates the destination rectangle for the substream, based
// on the current frame number.
//-------------------------------------------------------------------

void CDXVAApplication::CalculateSubstreamRect(INT frame, RECT *prect)
{
    INT x, y, wx, wy;
//	HRESULT hr;
    x = frame * VIDEO_SUB_VX;
    wx = m_canvasInfo.open_params.dwWidth- 100;
    x = (x / wx) & 0x1 ? wx - (x % wx) : x % wx;

    y = frame * VIDEO_SUB_VY;
    wy = m_canvasInfo.open_params.dwHeight - 100;
    y = (y / wy) & 0x1 ? wy - (y % wy) : y % wy;

    SetRect(prect, x, y, x + VIDEO_SUB_WIDTH, y + VIDEO_SUB_HEIGHT);
}

int CDXVAApplication::CreateStream(IN VP_OPEN_STREAM_PARAMS *pOpenParams, IN DWORD dwOpenSize, IDirect3DSurface9*&  pStream, int& id)
{
	int index = 0;
	int ret = true;

	if(!pOpenParams)  
		return false;

	//Ensure main stream is created by first
	HRESULT hr = S_OK;

	for(index =0;index<sizeof(m_streamInfo)/sizeof(SSTREAM_INFO);index++)
	{
		if(!m_streamInfo[index].valid)	break;
	}
	if(index>=sizeof(m_streamInfo)/sizeof(SSTREAM_INFO))	return E_UNEXPECTED;

	memset(&m_streamInfo[index], 0,sizeof(SSTREAM_INFO));
	m_streamInfo[index].valid = true;
	memcpy(&m_streamInfo[index].open_params,pOpenParams, min(sizeof(VP_OPEN_STREAM_PARAMS),dwOpenSize)  );
	SetRect(&m_streamInfo[index].m_rectSrc, 0,0,m_streamInfo[index].open_params.dwWidth, m_streamInfo[index].open_params.dwHeight);

	//UpdateStreamInfo();

	if(!ResetDevice())
		return false;
	//If format aren't supported
	if(!m_streamInfo[index].m_pStream)	return false;

	pStream = m_streamInfo[index].m_pStream;

	id = index;
	return true;
}

int CDXVAApplication::DestroyStream(int id)
{
	if(!m_streamInfo[id].valid || id >= MAX_STREAM_NUM -1)	return false;
	memset(&m_streamInfo[id], 0 ,sizeof(SSTREAM_INFO));

	return true;
}

HRESULT CDXVAApplication::CreateSubStream()
{
	HRESULT hr = S_OK;

	UINT iStreamCount = 1;
	for(int i=0;i<sizeof(m_streamInfo)/sizeof(SSTREAM_INFO);i++)
	{
		if(m_streamInfo[i].valid)
		{

			hr = m_pDXVAHD->CreateVideoSurface(
				m_streamInfo[i].open_params.dwWidth,
				m_streamInfo[i].open_params.dwHeight,
				(D3DFORMAT)m_streamInfo[i].open_params.dwFourCC,
				m_caps.InputPool,
				0,
				DXVAHD_SURFACE_TYPE_VIDEO_INPUT,
				1,
				&m_streamInfo[i].m_pStream,
				NULL
				);

			if (FAILED(hr)) { goto done; }


			// Set the initial stream states for the substream.

			// Video format
			hr = DXVAHD_SetStreamFormat(m_pDXVAVP, iStreamCount , (D3DFORMAT)m_streamInfo[i].open_params.dwFourCC);

			if (FAILED(hr)) { goto done; }

			// Frame format (progressive)
			hr = DXVAHD_SetFrameFormat(m_pDXVAVP, iStreamCount , m_streamInfo[i].format );

			if (FAILED(hr)) { goto done; }

			// Luma key
			hr = DXVAHD_SetLumaKey(m_pDXVAVP, iStreamCount , false, 0.9f, 1.0f);

			if (FAILED(hr)) { goto done; }
		
			iStreamCount++;
		}

	}

done:
	return hr;
}

HRESULT CDXVAApplication::DXVAAppSetSrc(int id, RECT* pSrc)
{
	HRESULT hr = S_OK;
	if((id>= MAX_STREAM_NUM -1)||(!m_streamInfo[id].valid) || !pSrc)	return hr;
	m_streamInfo[id].m_rectSrc= *pSrc;
	return hr;
}

HRESULT CDXVAApplication::DXVAAppSetDest(int id, RECT* pDst)
{
	HRESULT hr = S_OK;
	if((id>= MAX_STREAM_NUM -1)||(!m_streamInfo[id].valid) || !pDst)	return hr;
	m_streamInfo[id].m_rectDst = *pDst;
	return hr;
}

HRESULT CDXVAApplication::DXVAAppSetEnable(int id, bool* pEnable)
{
	HRESULT hr = S_OK;
	if((id>= MAX_STREAM_NUM-1)||(!m_streamInfo[id].valid) || !pEnable)	return hr;

	m_streamInfo[id].m_bEnable = *pEnable;
	return hr;
}

HRESULT CDXVAApplication::DXVAAppSetDeInterlaceMode(int id, DXVAHD_FRAME_FORMAT format)
{
	HRESULT hr = S_OK;
	if((id>= MAX_STREAM_NUM-1)||(!m_streamInfo[id].valid))	return hr;

	if(m_streamInfo[id].format  != format)
	{
		m_streamInfo[id].format =  format;
		hr = DXVAHD_SetFrameFormat(m_pDXVAVP, id+1, m_streamInfo[id].format );
	}
	return hr;

}

HRESULT CDXVAApplication::DXVAAppSetProcEnable(bool* pEnable)
{
	HRESULT hr = S_OK;
	if(!pEnable)	return E_FAIL;

	m_bEnable = *pEnable;
	if(!m_bEnable)
    	{
		IDirect3DSurface9 *pRT = NULL;  // Render target
		hr = m_D3D.m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pRT);
		if(FAILED(hr))	return hr;
        	hr = m_D3D.m_pDevice->ColorFill(pRT, NULL, D3DCOLOR_XRGB(0, 0, 0));
		if(FAILED(hr))	return hr;

		hr = m_D3D.m_pDevice->Present(NULL, NULL, NULL, NULL);
		if(FAILED(hr))	return hr;
    	}

	return S_OK;
}

HRESULT CDXVAApplication::DXVAAppSetMainDestRect(RECT* pDst)
{
	HRESULT hr = S_OK;
	if(!pDst)	return E_FAIL;

	if(memcmp(&m_rcMainVideoDestRect , pDst,sizeof(m_rcMainVideoDestRect)))
	{
		m_rcMainVideoDestRect = *pDst;
		ApplySettings();
	}
	return hr;
}

HRESULT CDXVAApplication::DXVAAppSetMainSrcRect(RECT* pDst)
{
	HRESULT hr = S_OK;
	if(!pDst)	return E_FAIL;

	if(memcmp(&m_rcMainVideoSourceRect , pDst,sizeof(m_rcMainVideoDestRect)))
	{
		m_rcMainVideoSourceRect = *pDst;
		hr = ApplySettings();
	}
	return hr;
}

HRESULT CDXVAApplication::DXVAGetFilter(DXVAHD_FILTER filter, ProcAmpInfo* pInfo)
{
	if(!pInfo)
		return E_FAIL;
	*pInfo = m_Filters[ filter];
	return S_OK;
}

HRESULT CDXVAApplication::DXVASetFilter(DXVAHD_FILTER filter, INT CurrentValue)
{
	if(!m_Filters[filter].bSupported)
		return E_FAIL;
	if(CurrentValue > m_Filters[filter].Range.Maximum)
		CurrentValue = m_Filters[filter].Range.Maximum;

	if(CurrentValue < m_Filters[filter].Range.Minimum)
		CurrentValue = m_Filters[filter].Range.Minimum;
	
	m_Filters[filter].CurrentValue = CurrentValue;
	AdjustFilter(filter);
	return S_OK;
}

HRESULT CDXVAApplication::DXVACreateCanvas(DWORD width, DWORD height)
{
	HRESULT hr = S_OK;
	if(m_bCanvasInit)	return E_FAIL;
	memset(&m_canvasInfo, 0 ,sizeof(m_canvasInfo));
	m_canvasInfo.open_params.dwWidth = width;
	m_canvasInfo.open_params.dwHeight = height;
	m_canvasInfo.open_params.dwFourCC = D3DFMT_X8R8G8B8;//MAKE_FOURCC('Y','V','1','2');
	m_canvasInfo.format	= DXVAHD_FRAME_FORMAT_PROGRESSIVE;

	SetRect(&m_rcMainVideoSourceRect, 0,0, width, height);
	SetRect(&m_rcMainVideoDestRect, 0,0, width, height);

	hr = Initialize();
	if(FAILED(hr))	return hr;

	
	m_bCanvasInit = true;
	return S_OK;
}

HRESULT CDXVAApplication::DXVADestroyCanvas()
{
	if(!m_bCanvasInit)	return E_FAIL;

	if(!ResetDevice())
		return E_FAIL;

	memset(&m_canvasInfo, 0 ,sizeof(m_canvasInfo));
	m_bCanvasInit = false;
	return S_OK;
}

