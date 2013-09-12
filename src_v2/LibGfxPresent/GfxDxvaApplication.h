//////////////////////////////////////////////////////////////////////
//
// CDXVAApplication.h
// 
// Contains most of the CDXVAApplication logic. 
// 
// WinMain creates an instance of this class. This class runs the
// message loop for the CDXVAApplication window.
// 
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) CIDANA Corporation. All rights reserved.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "../../inc/GfxPresent.h"
#include "../../src_v2/LibGfxPresent/GfxDxvahd_utils.h"

struct ProcAmpInfo
{
    BOOL                        bSupported;
    DXVAHD_FILTER_RANGE_DATA    Range;
    INT                         CurrentValue;
    INT                         Step;
};

class CDXVAApplication
{
	#define MAX_STREAM_NUM	15
    HWND                m_Hwnd;     // CDXVAApplication window
    DwmHelper           m_DWM;      // DWM functions
    D3DHelper           m_D3D;      // Manages the Direct3D device
    Timer               m_timer;    // Timer for running the render loop.


    DXVAHD_DEVICE_USAGE m_usage;    // DXVA-HD device usage (command-line parameter)

    // DXVA-HD interfaces
    IDXVAHD_Device          *m_pDXVAHD;
    IDXVAHD_VideoProcessor  *m_pDXVAVP;
    IDirect3DSurface9       *m_pMainStream;
    IDirect3DSurface9*      m_ppSubStream[MAX_STREAM_NUM];

    // Current settings
    WORD    m_PlanarAlphaValue;
    BYTE    m_PixelAlphaValue;

    RECT    m_rcMainVideoSourceRect;    // Source rectangle for the main video image.
    RECT    m_rcMainVideoDestRect;      // Destination rectangle for the main video image.
  //  RECT    m_rcVideoSubRect;           // Source rectangle for the substream image.
    
    // Note: The Substream destination rectangle is calculated per frame, so we do not
    // store it in a member variable.

    // Values for calculating the target rectangle, as a percentage of the 
    // destination surface.
    INT     m_TargetWidthPercent;       
    INT     m_TargetHeightPercent;      

    // Color information.
    INT     m_iExtendedColor;       // Index into the EX_COLOR_INFO array.
    INT     m_iBackgroundColor;     // Index into the BACKGROUND_COLORS array.

    // Proc amp filtering information


    ProcAmpInfo m_Filters[ NUM_FILTERS ];

	static D3DFORMAT VIDEO_MAIN_FORMAT          ;
	static D3DFORMAT VIDEO_SUB_FORMAT           ; // AYUV

	// Primary stream
	//static UINT VIDEO_MAIN_WIDTH ;
	//static UINT VIDEO_MAIN_HEIGHT;
	static RECT VIDEO_MAIN_RECT  ;

	static UINT DEST_BORDER ;
	static RECT VIDEO_MAIN_DEST_RECT ; 

	// Substreams
	static UINT SUB_STREAM_COUNT  ;

	static UINT VIDEO_SUB_WIDTH   ;
	static UINT VIDEO_SUB_HEIGHT  ;

	static UINT VIDEO_SUB_SURF_WIDTH ;
	static UINT VIDEO_SUB_SURF_HEIGHT;
	static RECT VIDEO_SUB_RECT ;
    DXVAHD_VPDEVCAPS m_caps;
public:
	typedef struct 
	{
		bool valid;
		VP_OPEN_STREAM_PARAMS open_params;
		IDirect3DSurface9*      m_pStream;
		DXVAHD_FRAME_FORMAT format;
		RECT m_rectSrc, m_rectDst, m_rectScn;
		bool m_bEnable;
		float fAlpha;
	}SSTREAM_INFO;
	SSTREAM_INFO m_streamInfo[MAX_STREAM_NUM];
	SSTREAM_INFO m_canvasInfo;
	bool m_bCanvasInit;
	HHOOK  m_hHook;
public:
    CDXVAApplication(HWND wnd);

    ~CDXVAApplication();

	HRESULT   Initialize();
	HRESULT UnInitialize();
    INT     MessageLoop();
    LRESULT HandleMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	int CreateStream(IN VP_OPEN_STREAM_PARAMS *pOpenParams, IN DWORD dwOpenSize , IDirect3DSurface9*&      pStream, int& id);
	int DestroyStream(int id);
    BOOL    ProcessVideoFrame();
	HRESULT DXVAAppSetSrc(int id, RECT* pSrc);
    HRESULT DXVAAppSetDest(int id, RECT* pDst);
	HRESULT DXVAAppSetEnable(int id, bool* pEnable);
    HRESULT AdjustAlphaSetting(int id, float* pAlpha);
	HRESULT DXVAAppSetDeInterlaceMode(int id, DXVAHD_FRAME_FORMAT format);
	HRESULT DXVAAppSetProcEnable(bool* pEnable);
	HRESULT DXVAAppSetMainDestRect(RECT* pDst);
	HRESULT DXVAAppSetMainSrcRect(RECT* pDst);
	HRESULT DXVAGetFilter(DXVAHD_FILTER filter, ProcAmpInfo* pInfo);
	HRESULT DXVASetFilter(DXVAHD_FILTER filter, INT CurrentValue);
	HRESULT DXVACreateCanvas(DWORD width, DWORD height);
	HRESULT DXVADestroyCanvas();
	void    OnSize();
	

private:
    BOOL    InitializeDXVAHD();
    void    DestroyDXVAHD();

    BOOL    ResetDevice(BOOL bChangeWindowMode = FALSE);

    BOOL    PreTranslateMessage(const MSG& msg);


    HRESULT UpdateVideoSubRect();
    HRESULT ApplySettings();
    HRESULT ResetSettings();
    HRESULT AdjustExtendedColor(int dy);
    HRESULT AdjustBackgroundColor(eBACK_COLOR color);
    HRESULT AdjustFilter(DXVAHD_FILTER filter);
    HRESULT AdjustTargetRect(int dx, int dy);
        
    void    CalculateSubstreamRect(INT frame, RECT *prect);
//	int UpdateStreamInfo(); 
	HRESULT  CreateSubStream();



	bool m_bEnable;

};
