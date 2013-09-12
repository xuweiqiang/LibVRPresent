//===================================== ========================================
//  CIDANA CONFIDENTIAL INFORMATION
//
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO CIDANA, INC.
// 	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//	COMPANY.
//
// 	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// 	PURPOSE.
//
// 	Copyright (c) 2008  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------


#include "../../inc/GfxBuildDef.h"
#ifndef GFXPRESENT_OMX

#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0502		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.  
#define _WIN32_WINNT 0x0502	// Change this to the appropriate value to target other versions of Windows.
#endif			

#include <stdio.h>
#include <windows.h>
#include <ddraw.h>
#include "../../inc/GfxPresent.h"
#include "../../inc/GfxContext.h"
#include "../LibGfxContext/GfxOverlay.h"
#include "../../inc/McCom.h"
#include "../../inc/CProtect.h"

#ifdef ANDROID
#include "VideomixerInterface.h"
#else
#include "../../src/LibVideoMixer/VideomixerInterface.h"
#endif
#include "../../inc/dp.h"
#include "../../inc/GfxPresentPrepare.h"
#include "../../inc/GfxVideoWindow.h"

#ifndef WINCE
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "tchar.h"

#include "../../3rdparty/WIN32/D3DMixer/D3DMixer.h"
#pragma comment(lib, "../../3rdparty/WIN32/D3DMixer/D3DMixer.lib")
#endif

#include "../../inc/DMCP.h"
#include "../../inc/VDecRnd.h"

#include "GfxPresentDef.h"
#ifdef ST_SAVECOPY
#include "../../3rdparty/ST/demo_NOCOPY/stglib.h"
#include "../../3rdparty/ST/demo_NOCOPY/STSGA_lib.h"
#endif

#ifdef ST_LINUX
#include "../../3rdparty/ST_Linux/noDataCopy/stglib.h"
#include "../../3rdparty/ST_Linux/noDataCopy/STSGA_lib.h"
#elif defined(RENESAS)
#include "../../3rdparty/Renesas/include/libvo.h"
#endif

//#define DUMP_FPS
#ifdef DUMP_FPS
static DWORD g_startTick = 0;
static DWORD g_frames = 0;
#endif
#define PRESENT_BACKBUFFER_NUM 2
 
#ifdef  USE_IPP_ROTATION
#include "ipp.h"

inline int WidthRect(const RECT *r)
{
	return r->right - r->left;
}

inline int HeightRect(const RECT *r)
{
	return r->bottom - r->top;
}

#endif


#define MAX_FOURCC_NUMBER	30

#ifdef GFX_DDRAW
CDDRAWPREINIT::CDDRAWPREINIT()
{
	DWORD tick = GetTickCount();
#ifdef _WIN32_WCE
	DirectDrawCreate(0,(LPDIRECTDRAW*)&m_pDDraw,0);
#else
	DirectDrawCreateEx( 0,(void**)&m_pDDraw,IID_IDirectDraw7,0);
#endif
	DP("DirectDrawCreate cost:%d \n",GetTickCount() - tick);
}

CDDRAWPREINIT::~CDDRAWPREINIT()
{
	if(m_pDDraw)
	{
		m_pDDraw->Release();
		m_pDDraw = 0;
 	}
}

STDMETHODIMP CDDRAWPREINIT::GetConfig(void* pData) 
{
	if(pData)
		*reinterpret_cast<IDirectDraw**>(pData) = m_pDDraw;
	return S_OK;
}
#endif
#if 0 //ndef WINCE
class CVRPresenterStreamOvl : 
	public IMcVideoPresenterStream,
	public CMcUnknown
{
public:
	CVRPresenterStreamOvl(IMcVideoMixer * pFather);
	virtual ~CVRPresenterStreamOvl ();

	//IUNKNOWN
	DECLARE_IUNKNOWN

	STDMETHODIMP Open(IN VP_OPEN_STREAM_PARAMS *pOpenParams,
					 IN DWORD dwOpenSize);
	STDMETHODIMP Close();
	STDMETHODIMP Render(DWORD dwRenderFlags);
	STDMETHODIMP Get(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned);
	STDMETHODIMP Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData);
	STDMETHODIMP LockSurface(DWORD dwIndex, LPBYTE *ppData, DWORD *pdwStride);
	STDMETHODIMP UnlockSurface(DWORD dwIndex);

private:
	IMcVideoMixer *m_pD3DMixer;
	IMcVideoStream *m_pD3DStream;
};
#endif
const DWORD CVRPresentOvl::const_SurfFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
	MAKEFOURCC('Y','U','Y','2'),
	MAKEFOURCC('R','5','6','5'),
	0
};

#define DEFAULT_INPUT_FOURCC	MAKEFOURCC('Y','V','1','2')
const DWORD CVRPresentOvl::const_InputFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
	MAKEFOURCC('Y','V','1','6'),
	MAKEFOURCC('N','V','1','2'),
	MAKEFOURCC('P','B','S','1'),
	MAKEFOURCC('R','5','6','5'),
	MAKEFOURCC('Y','U','Y','2'),
	MAKEFOURCC('X','R','G','B'),
	0
};

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
	MAKEFOURCC('Y','U','Y','V'),
	MAKEFOURCC('Y','U','Y','2'),
	MAKEFOURCC('R','5','6','5'),
	MAKEFOURCC('R','8','8','8'),
	MAKEFOURCC('X','R','G','B'),
	MAKEFOURCC('N','V','1','2'),
	//MAKEFOURCC('A','R','3','2'),
	//MAKEFOURCC('I','M','C','1'),
	0
};

#define VIDEO_PRESENTER_NAME TEXT("Video Presenter")
#define HARDWARE_VIDEO_EFFECT_INIT_VALUE	128
#define SOFTWARE_VIDEO_EFFECT_INIT_VALUE	128

CVRPresentOvl::CVRPresentOvl() :
	m_lZoomMode(VE_MODE_STRETCH),
	m_lInterpolationMode(0),
	m_lRotationMode(VP_ROTATION_0),
	m_lDeblockMode(0),
	m_lResampleMode(0),
	m_lRenderEnabled(false),
	m_bZoomEnable(TRUE),
	m_dwStatus(0),
	m_dwPresentIndex(0),
	m_pOverlay(0),
	m_overlayType(0),
	m_pvideoMixer(0),
	CMcUnknown(VIDEO_PRESENTER_NAME,0),
	m_pVideoWindow(0),
	m_bOpen(FALSE),
	m_bGoMixer(FALSE),
	pVideoMemoryInfo(0),
	m_bSoftScale(0),
	m_bDMCPEnable(true),
	m_pOSDbuf(0)
{
	ZeroMemory(&m_rSrcSize,sizeof(m_rSrcSize));
	ZeroMemory(&m_rDst,sizeof(m_rDst));
	ZeroMemory(&m_rScreen,sizeof(m_rScreen));
	ZeroMemory(&m_alCC,sizeof(m_alCC));
	ZeroMemory(&m_VBI,sizeof(m_VBI));
	ZeroMemory(&m_OpenOptions,sizeof(m_OpenOptions));
	ZeroMemory(&m_ginfo,sizeof(m_ginfo));
	ZeroMemory(&m_videoEffect, sizeof(m_videoEffect));

	ZeroMemory(&m_surfHandle, sizeof(m_surfHandle));
	ZeroMemory(&m_bufferMemoryInfo, sizeof(m_bufferMemoryInfo));
	ZeroMemory(&m_sColorCaps, sizeof(m_sColorCaps));
	ZeroMemory(&m_SoftOpenOptions, sizeof(m_SoftOpenOptions));
	ZeroMemory(&rVideoWnd, sizeof(rVideoWnd));
	ZeroMemory(&m_absoluteRectBak, sizeof(m_absoluteRectBak));
	InitializeCriticalSection(&m_csDisplay);

	m_pDMCP = 0;
	m_DMCP_dsp_enable = TRUE;
	m_lastpresent_tick = 0;
	m_DMCP_ACP_flag = 0;
	m_DMCP_CGMS_flag = 0;
	m_DMCP_HDCP_flag = 0;
#ifndef WINCE
	m_pVideoMixer_D3D = 0;
#endif
	m_FPS = 0;
	m_vsyncMode = VP_VSYNC_MODE_ASYNC;
	m_StreamCount = 0;
	m_pConfig = 0;
	#ifdef GFX_DDRAW
	m_pConfig = new CDDRAWPREINIT();
	#endif
}

CVRPresentOvl::~CVRPresentOvl()
{
	Close();

	
	if(m_pvideoMixer)
	{
		m_pvideoMixer->Release();
		m_pvideoMixer = 0;	
	}

	if(m_pVideoWindow)
	{
		m_pVideoWindow->Release();
		m_pVideoWindow = 0;
	}
	if(m_pOverlay)
	{
		m_pOverlay->Release();
		m_pOverlay = 0;
	}

	if(pVideoMemoryInfo)
	{
		delete pVideoMemoryInfo;
		pVideoMemoryInfo = 0;
	}

	if(m_pConfig)
	{
		m_pConfig->Release();
		m_pConfig = 0;
	}

	DeleteCriticalSection(&m_csDisplay);
}

STDMETHODIMP CVRPresentOvl::Open(VP_OpenOptions *pOptions, DWORD dwOptionSize)
{
	int state;
	HRESULT res = S_OK;
	DWORD i =0;

	CProtect proc(&m_csDisplay);

	if((pOptions==0)||(sizeof(m_OpenOptions)!=dwOptionSize))
		return E_INVALIDARG;

	if(m_bOpen)
	{
		DP("[GFXPRESENTOVL] Invalid call. Video Presenter is opened already \n");
		return E_FAIL;
	}


	//pOptions->dwWidth = 480;
	if(pOptions->dwWidth == 0 || pOptions->dwHeight == 0)
		return E_INVALIDARG;

	m_unsupportSizeFlag = false;
#if  defined(SIRF_A4_NEWOS) && !defined(SIRF_A6)
	if((pOptions->dwWidth >= 768))
	{
		memcpy(&m_unsupportOptions, pOptions, sizeof(m_OpenOptions));
		pOptions->dwWidth  = 768;
		m_unsupportSizeFlag = true;
	}
#elif defined(FREESCALE_IMX31)
	if((pOptions->dwWidth >= 720)||(pOptions->dwHeight >= 576))
		return E_INVALIDARG;
#endif


	if(pOptions->dwHeight%2)
	{
	//#ifdef TITAN_II
		pOptions->dwHeight -= 1;
	//#else
	//	return E_INVALIDARG;
	//#endif		
	}

	m_StreamCount = 0;
	m_bPresented = false;
	m_dwAlpha = MAX_ALPHA_INTERNAL;
	ZeroMemory(&m_rDst,sizeof(m_rDst));
	ZeroMemory(&m_VBI,sizeof(m_VBI));
	ZeroMemory(&rVideoWnd, sizeof(rVideoWnd));
	ZeroMemory(&m_absoluteRectBak, sizeof(m_absoluteRectBak));
	m_bSoftScale = FALSE;

	m_Deinterlaced_Mode = VP_DEINTERLACE_None;
	m_FPS = 0;
	m_vsyncMode = VP_VSYNC_MODE_ASYNC;
	
	m_dwCkeySrc = 0x00108080;
	m_bCkeySrc = false;
	m_lFlipVideoMode  = VP_FLIPVH_NO;

	ZeroMemory(&m_SoftOpenOptions, sizeof(m_SoftOpenOptions));

	if(pOptions != &m_OpenOptions)
		memcpy(&m_OpenOptions, pOptions, sizeof(m_OpenOptions));


	#ifdef SAMSUNG_2450_LEADER_INTL
	m_OpenOptions.dwWidth &= ~(8-1);

	#elif defined(SIRF_A4_NEWOS)
	m_OpenOptions.dwWidth &= ~(4-1);
	m_OpenOptions.dwHeight &= ~(4-1);

	#elif defined(ST_LINUX)
	m_OpenOptions.dwWidth &= ~(4-1);
	m_OpenOptions.dwHeight &= ~(4-1);
    if(m_OpenOptions.dwWidth == 0 || m_OpenOptions.dwHeight == 0)
    {
      return E_FAIL;
    }

  #elif defined(RENESAS_ANDROID)
  if(m_OpenOptions.dwWidth%8)
    	m_OpenOptions.dwWidth &= ~(8-1);

  if(m_OpenOptions.dwHeight%4)
    	m_OpenOptions.dwHeight &= ~(4-1);

  #elif defined(MTK3353_SW)
	m_OpenOptions.dwHeight &= ~(8-1);
	#endif

/*********************WorkAround for specfic platform of WCE**************************************/
	#if defined(PXA310) || defined(SAMSUNG_2416)|| defined(SAMSUNG6443) || defined(VNCVIEWER) 
		m_OpenOptions.dwBuffers = 1;
	#endif
/*********************WorkAround for specfic platform of WCE End**************************************/

	if(m_OpenOptions.dwBuffers == 0)
		m_OpenOptions.dwBuffers = PRESENT_BACKBUFFER_NUM;


/*********************FourCC Check**************************************/
//Check whether input fourCC is supportted 
	if(m_OpenOptions.dwFourCC == 0)
		m_OpenOptions.dwFourCC = DEFAULT_INPUT_FOURCC;
	
	for(i=0;const_InputFourCC[i] != 0;i++)
	{
		if(const_InputFourCC[i] == m_OpenOptions.dwFourCC)
			break;
	}

	//Invalid input fourCC
	if(const_InputFourCC[i] == 0)
	{
		return E_FAIL;
	}


#ifdef DUMMY
	m_pOverlay = 0;
	m_bOpen = TRUE;
	return S_OK;
#endif

	surfaceFourCC = const_InputFourCC[i];

/*********************FourCC Check End**************************************/

	for(state=0;;state++)
	{

		if(m_pOverlay)
		{

			if(m_overlayType & m_OpenOptions.dwSurfType)
			{
				state = -1;
				goto try_init;
			}
			else
			{
				m_pOverlay->Release();
				m_pOverlay = 0;
			}
		}

		switch(state)
		{
		case 0:	 // special hardware accelerated overlays
#if defined(TITAN)
#pragma message("TITAN defined, CreateOverlayTitan")
				m_pOverlay = CreateOverlayTitan();
#elif defined(TITAN_II)
			if((m_OpenOptions.dwSurfType)==0 ||
				(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_TITIAN_II))
			{
				if(m_OpenOptions.dwFlags & VP_OPEN_FLAG_OSD)
				{
#pragma message("TITANII defined, CreateOverlayTitanII OSD")
#ifndef __linux__
					m_pOverlay = CreateOverlayTitanII_OSD();
#endif
					m_overlayType = VP_OPEN_FLAG_TITIAN_II_OSD;
				}
				else
				{
#pragma message("TITANII defined, CreateOverlayTitanII")
#ifndef __linux__
					m_pOverlay = CreateOverlayTitanII();
#endif
					m_overlayType = VP_OPEN_FLAG_TITIAN_II;
				}
			}

#elif defined(GFX_EGL)
			if((m_OpenOptions.dwSurfType)==0 ||
				(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_EGL))
			{
				m_pOverlay = CreateEGL();
				m_overlayType = VP_OPEN_FLAG_EGL;
			}

#elif defined(RMI)
			if((m_OpenOptions.dwSurfType)==0 ||
				(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_RMI))
			{
#ifndef __linux__
				m_pOverlay = CreateOverlayRMI();
#endif
				m_overlayType = VP_OPEN_FLAG_RMI;
			}

#elif defined(RMI_CE6)
        {
			m_pOverlay = CreateOverlayRMI_CE6();
			m_overlayType = VP_OPEN_FLAG_RMI;
        }

#elif defined(GFX_QNX)
        {
			m_pOverlay = CreateOverlayQNX();
			m_overlayType = VP_OPEN_FLAG_QNX;
        }
#elif defined(GFX_ANDROID_ANativeWindow)
        {
			m_pOverlay = CreateOverlayAndroidANativeWindow();
			m_overlayType = VP_OPEN_FLAG_ANATIVEWINDOW;
        }

#elif defined(GFX_MSTAR)
			if((m_OpenOptions.dwSurfType)==0 ||
				(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_MSTAR))
			{
				m_pOverlay = CreateOverlayMSTAR();
				m_overlayType = VP_OPEN_FLAG_MSTAR;
			}
#elif defined(SAMSUNG)
			if((m_OpenOptions.dwSurfType)==0 ||
				(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_SAMSUNG))
			{
				m_pOverlay = CreateOverlaySAMSUNG();
				m_overlayType = VP_OPEN_FLAG_SAMSUNG;
			}
#elif defined(PXA)
			if((m_OpenOptions.dwSurfType)==0 ||
				(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_PXA))
			{
				m_pOverlay = CreateOverlayPXA();
				m_overlayType = VP_OPEN_FLAG_PXA;
			}
#elif defined(ST)
			if((m_OpenOptions.dwSurfType)==0 ||
				(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_ST))
			{
				m_pOverlay = CreateOverlayST();
				m_overlayType = VP_OPEN_FLAG_ST;
			}

#elif defined(GFX_OPENGL)
			if((m_OpenOptions.dwSurfType)==0 ||
				m_OpenOptions.dwSurfType&VP_OPEN_FLAG_OPENGL)
			{
				m_pOverlay = CreateOverlayOPENGL();
				m_overlayType = VP_OPEN_FLAG_OPENGL;
			}

#elif defined(SAMSUNG6410)
			if((m_OpenOptions.dwSurfType)==0 ||
				(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_SAMSUNG6410))
			{
				m_pOverlay = CreateOverlaySAMSUNG6410();
				m_overlayType = VP_OPEN_FLAG_SAMSUNG6410;
			}

#elif defined(JIASHI_SAMSUNG6410)
			if((m_OpenOptions.dwSurfType)==0 ||
				(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_JIASHI_SAMSUNG6410))
			{
				m_pOverlay = CreateJIASHI_SAMSUNG6410();
				m_overlayType = VP_OPEN_FLAG_JIASHI_SAMSUNG6410;
			}
#elif defined(RENESAS)
			if((m_OpenOptions.dwSurfType==0) ||
				(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_RENESAS))
			{
				m_pOverlay = CreateOverlayRenesas();
				m_overlayType = VP_OPEN_FLAG_RENESAS;
			}
		    else if(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_RENESASHW)
			{
				m_pOverlay = CreateOverlayRenesasHW();
				m_overlayType = VP_OPEN_FLAG_RENESASHW;
			}
#elif defined(ST_LINUX)
			if((m_OpenOptions.dwSurfType)==0 ||
			(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_ST_LINUX))
			{
				m_pOverlay = CreateOverlaySTLnx(false);
				m_overlayType = VP_OPEN_FLAG_ST_LINUX;
			}
#elif defined(GFX_OPENGLES)
			if((m_OpenOptions.dwSurfType)==0 ||
				(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_OPENGLES))
			{
				m_pOverlay = CreateOverlayOpenGLES();
				m_overlayType = VP_OPEN_FLAG_OPENGLES;
			}

#elif defined(GFX_ANDROID_OPENGL_ES_SHADER)
		        {
				m_pOverlay = CreateOverlayAndroidOGL_ES_SHADER();
				m_overlayType = VP_OPEN_FLAG_OPENGLES;
		        }
#elif defined(GFX_ANDROID_STAGEFRIGHT)
			{
				m_pOverlay = CreateAndroidStageFright();
				m_overlayType = VP_OPEN_FLAG_AUTO;
			}

#elif defined(GFX_ANDROID_ISURFACE_OVERLAY)
			{
				m_pOverlay = CreateAndroidISurfaceOverlay();
				m_overlayType = VP_OPEN_FLAG_ANDROID_ISURFACE_OVERLAY;
			}
#endif
			break;
		case 1:	// d3d
#if defined(ST_LINUX)
			if((m_OpenOptions.dwSurfType&VP_OPEN_FLAG_ST_EXTERNALBUFFER))
			{
				m_pOverlay = CreateOverlaySTLnx(true);
				m_overlayType = VP_OPEN_FLAG_ST_EXTERNALBUFFER;
			}
#endif

#ifdef GFX_D3D
#ifndef _WIN32_WCE
			if((m_OpenOptions.dwSurfType)==0 ||
				m_OpenOptions.dwSurfType&VP_OPEN_FLAG_D3D)
				{
#ifndef __linux__
					m_pOverlay = CreateOverlayD3D();
#endif
					m_overlayType = VP_OPEN_FLAG_D3D;
				}
#endif
#endif
			break;
		case 2:	// regular directdraw overlays
#ifdef GFX_DDRAW
			if((m_OpenOptions.dwSurfType)==0 ||
				m_OpenOptions.dwSurfType&VP_OPEN_FLAG_DDRAW|| 
				m_OpenOptions.dwSurfType&VP_OPEN_FLAG_ST|| 
				m_OpenOptions.dwSurfType&VP_OPEN_FLAG_ST_EXTERNALBUFFER
				)
				{
#ifndef __linux__

#if defined(SIRF_A4_PVEGPS ) || defined(SAMSUNG_PC100)
	m_OpenOptions.dwFlags = VP_OPEN_FLAG_DDRAW_BLIT;
#endif

					m_pOverlay = CreateOverlayDDraw(m_OpenOptions.dwFlags);
#endif
					m_overlayType = VP_OPEN_FLAG_DDRAW;
				}
#endif
			break;
		case 3:	// gdi
#ifdef GFX_GDI
			if((m_OpenOptions.dwSurfType)==0 ||
				m_OpenOptions.dwSurfType&VP_OPEN_FLAG_GDI)
				{
#ifndef __linux__
					m_pOverlay = CreateOverlayGDI();
#endif
					m_overlayType = VP_OPEN_FLAG_GDI;
				}
#endif
			break;
		case 4: // gx mode
#ifdef GFX_GX
		#if ((_WIN32_WCE == 0x502) || (_WIN32_WCE == 0x501))
			if((m_OpenOptions.dwSurfType)==0 ||
				m_OpenOptions.dwSurfType&VP_OPEN_FLAG_GX)
				{
#ifndef __linux__
					m_pOverlay = CreateOverlayGx();
#endif
					m_overlayType = VP_OPEN_FLAG_GX;
				}
		#endif
#endif
			break;
		case 5:
#ifdef GFX_SDL
#ifndef _WIN32_WCE
#if defined(__linux__) || defined(SDL_SUPPORT)
		if((m_OpenOptions.dwSurfType)==0 ||
				m_OpenOptions.dwSurfType&VP_OPEN_FLAG_SDL)
				{
					m_pOverlay = CreateOverlaySDL();
					m_overlayType = VP_OPEN_FLAG_SDL;
				}
#endif
#endif
#endif
			break;
		case 6:
#ifdef GFX_ANDROID_OMX
				{
					m_pOverlay = CreateOverlayAndroid();
					m_overlayType = VP_OPEN_FLAG_ANDROID;
				}
#endif			
#ifdef GFX_ANDROID_SF
				{
					m_pOverlay = CreateAndroidSF();
					m_overlayType = VP_OPEN_FLAG_ANDROID_SF;
				}
#endif				

#ifdef GFX_ANDROID_OPENGL_ES
				{
					m_pOverlay = CreateOverlayAndroidOGL();
					m_overlayType = VP_OPEN_FLAG_ANDROID_OGL;
				}
#endif

#ifdef GFX_ANDROID_OCE
				{
					m_pOverlay = CreateOverlayAndroidOCE();
					m_overlayType = VP_OPEN_FLAG_ANDROID_OPENCORE;
				}
#endif

#ifdef GFX_ANDROID_CANVAS
				{
					m_pOverlay = CreateOverlayAndroidCanvas();
					m_overlayType = VP_OPEN_FLAG_ANDROID_CANVAS;
				}
#endif
#ifdef GFX_ANDROID_SURFACE
				{
					m_pOverlay = CreateOverlayAndroidSurface();
					m_overlayType = VP_OPEN_FLAG_ANDROID_SURFACE;
				}
#endif
#ifdef GFX_ANDROID_SOFTWARE
				{
					m_pOverlay = CreateOverlayAndroidSoftware();
					m_overlayType = VP_OPEN_FLAG_ANDROID_SURFACE;
				}
#endif


#ifdef GFX_ANDROID_OPENCORE
				{
					m_pOverlay = CreateOverlayAndroidOCE();
					m_overlayType = VP_OPEN_FLAG_ANDROID_OPENCORE;
				}
#endif
	break;
		case 7:
#ifdef GFX_FB
		if((m_OpenOptions.dwSurfType)==0 ||
				m_OpenOptions.dwSurfType&VP_OPEN_FLAG_FB)
				{
					m_pOverlay = CreateOverlayFB();
					m_overlayType = VP_OPEN_FLAG_FB;
				}
#endif			
			break;
		case 8:	
#ifdef GFX_DDRAW_OSD
			if((m_OpenOptions.dwSurfType)==0 ||
				m_OpenOptions.dwSurfType&VP_OPEN_FLAG_DDRAW_OSD)
				{
#ifndef __linux__
					m_pOverlay = CreateDDrawOSD();
#endif
					m_overlayType = VP_OPEN_FLAG_DDRAW_OSD;
				}
#endif
			break;

		default:
			m_overlayType = 0;
			return E_FAIL;
		}
		
try_init:
		if(m_pOverlay && m_OpenOptions.hWnd)
		{
#if defined(__linux__) && !defined(__QNX__) 
			m_pOverlay->SetWindowHandle((Window)m_OpenOptions.hWnd);
#else
			m_pOverlay->SetWindowHandle((HWND)m_OpenOptions.hWnd);
#endif
		}

#if defined(NO_ROT_SCL) 
		if(m_overlayType == VP_OPEN_FLAG_DDRAW)
		{
			DWORD FourCC[] =
			{
				MAKEFOURCC('R','5','6','5'),
				0
			};
			if(m_pOverlay && 
				SUCCEEDED(m_pOverlay->Open(m_OpenOptions.dwWidth, 
				m_OpenOptions.dwHeight, m_OpenOptions.dwBuffers,(const DWORD *)&FourCC[0]),m_pConfig))
				break;
			else
				m_OpenOptions.dwBuffers = PRESENT_BACKBUFFER_NUM;

		}
#elif defined(MT3351_DBG) && !defined(MT3351_RGB)
			if(m_overlayType != VP_OPEN_FLAG_DDRAW_OSD)
			{
				DWORD backBuff_bak = m_OpenOptions.dwBuffers ;
				m_OpenOptions.dwBuffers = 2;
				DWORD FourCC[] =
				{
					MAKEFOURCC('Y','U','Y','V'),
					0
				};
				if(m_pOverlay && 
					SUCCEEDED(m_pOverlay->Open(m_OpenOptions.dwWidth, 
						m_OpenOptions.dwHeight, m_OpenOptions.dwBuffers,(const DWORD *)&FourCC[0]),m_pConfig))
					break;
				m_OpenOptions.dwBuffers = backBuff_bak;
			}

#elif defined(ST_SAVECOPY)
			if(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_ST_EXTERNALBUFFER)
			{
				DWORD FourCC[] =
				{
					MAKEFOURCC('P','B','S','1'),
					0
				};
				if(m_pOverlay && 
					SUCCEEDED(m_pOverlay->Open(m_OpenOptions.dwWidth, 
						m_OpenOptions.dwHeight, m_OpenOptions.dwBuffers,(const DWORD *)&FourCC[0],m_pConfig)))
					break;
			}

#elif defined(MT3351_RGB)
			if(m_overlayType != VP_OPEN_FLAG_DDRAW_OSD)
			{
				DWORD backBuff_bak = m_OpenOptions.dwBuffers ;
				m_OpenOptions.dwBuffers = 2;
				DWORD FourCC[] =
				{
					MAKEFOURCC('R','5','6','5'),
					0
				};
				if(m_pOverlay && 	SUCCEEDED(m_pOverlay->Open(m_OpenOptions.dwWidth, 
						m_OpenOptions.dwHeight, m_OpenOptions.dwBuffers,
						(const DWORD *)&FourCC[0]),m_pConfig))
					break;
				m_OpenOptions.dwBuffers = backBuff_bak;
			}
			else
			{
				DP("MT3351 RGB VIDEO Layer Mode doesn't support OSD layer!!!!!!!!!!\n");
				return E_FAIL;
			}
#endif

		//Try to init fourcc with input foucc mode
		DWORD fourCC[] =	{0,0};
		fourCC[0] = surfaceFourCC;
		if(m_pOverlay && 
			SUCCEEDED(m_pOverlay->Open(m_OpenOptions.dwWidth, 
				m_OpenOptions.dwHeight, m_OpenOptions.dwBuffers,(const DWORD *)&fourCC[0],m_pConfig)))
			{
					break;
			}

		if(m_overlayType == VP_OPEN_FLAG_D3D && surfaceFourCC  == MAKEFOURCC('Y','V','1','6'))
		{
			DWORD FourCC[] =
			{
				MAKEFOURCC('R','5','6','5'),
				0
			};
			if(m_pOverlay && 
				SUCCEEDED(m_pOverlay->Open(m_OpenOptions.dwWidth, m_OpenOptions.dwHeight, 
					m_OpenOptions.dwBuffers,(const DWORD *)&FourCC[0],m_pConfig)))
				break;
		}

		//Try to init by default fourcc mode
		if(m_pOverlay && 
			SUCCEEDED(m_pOverlay->Open(m_OpenOptions.dwWidth, m_OpenOptions.dwHeight, 
				m_OpenOptions.dwBuffers,(const DWORD *)&const_dwFourCC[0],m_pConfig)))
				{
					break;
				}

		if(m_pOverlay)
		{
			m_pOverlay->Close();
			m_pOverlay->Release();
			m_pOverlay = 0;
		}


	}
	
	switch(m_overlayType)
	{
	case VP_OPEN_FLAG_HARDWARE	:
			DP("VP_OPEN_FLAG_HARDWARE Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_DDRAW:
			DP("VP_OPEN_FLAG_DDRAW Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_GX:
			DP("VP_OPEN_FLAG_GX Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_GDI:
			DP("VP_OPEN_FLAG_GDI Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_D3D:
			DP("VP_OPEN_FLAG_D3D Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_TITIAN_II:
			DP("VP_OPEN_FLAG_TITIAN_II Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_TITIAN_II_OSD:
			DP("VP_OPEN_FLAG_TITIAN_II_OSD Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_RMI:
			DP("VP_OPEN_FLAG_RMI Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_SDL:
			DP("VP_OPEN_FLAG_SDL Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_SAMSUNG:
			DP("VP_OPEN_FLAT_SAMSUNG Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_PXA:
			DP("VP_OPEN_FLAT_PXA Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_ANDROID:
			DP("VP_OPEN_FLAG_ANDROID Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_FB:
			DP("VP_OPEN_FLAG_FB Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_ANDROID_OPENCORE:
			DP("VP_OPEN_FLAG_ANDROID_OPENCORE Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_OPENGL:
			DP("VP_OPEN_FLAG_OPENGL Surface is created !!!\n");
			break;
	case VP_OPEN_FLAG_DDRAW_OSD:
			DP("VP_OPEN_FLAG_DDRAW OSD Surface is created !!!\n");
			break;
    case VP_OPEN_FLAG_ST_EXTERNALBUFFER:
			DP("VP_OPEN_FLAG_ST_EXTERNALBUFFER is created !!!\n");
			break;
    case VP_OPEN_FLAG_ST_LINUX:
			DP("VP_OPEN_FLAG_ST_LINUX is created !!!\n");
			break;
	default:
			DP("UNKNOWN Surface is created !!!\n");
			break;

	}
	
	//Get HardwareSurface Info
	memset(&m_ginfo, 0 ,sizeof(m_ginfo));
	m_ginfo.dwSize = sizeof(m_ginfo);
	m_pOverlay->GetSurfaceInfo(&m_ginfo);

	DP("[GFXPRESENT] Open :%d,%d \n",m_OpenOptions.dwWidth,m_OpenOptions.dwHeight);



	   if(m_OpenOptions.dwFourCC == MAKEFOURCC('R','5','6','5'))
		DP("RGB565   ");
	   else  if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','V','1','2'))
		DP("YV12   ");
	   else  if(m_OpenOptions.dwFourCC == MAKEFOURCC('N','V','1','2'))
		DP("NV12   ");

		DP("->   ");

	   if(m_ginfo.dwFourCC == MAKEFOURCC('R','5','6','5'))
		DP("RGB565   ");
	   else  if(m_ginfo.dwFourCC == MAKEFOURCC('Y','V','1','2'))
		DP("YV12   ");
	   else  if(m_ginfo.dwFourCC == MAKEFOURCC('N','V','1','2'))
		DP("NV12   ");

	m_dwPresentIndex = 0;

	if(m_overlayType == VP_OPEN_FLAG_DDRAW_OSD)
	{
		m_pOSDbuf = (unsigned char*)malloc( m_OpenOptions.dwWidth*m_OpenOptions.dwHeight*4);

		if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','V','1','2'))
		{
			memset(m_pOSDbuf, 0x10, m_OpenOptions.dwWidth*m_OpenOptions.dwHeight);
			memset(m_pOSDbuf + m_OpenOptions.dwWidth*m_OpenOptions.dwHeight, 0x80, m_OpenOptions.dwWidth*m_OpenOptions.dwHeight/2);
		}
		else if(m_OpenOptions.dwFourCC == MAKEFOURCC('R','5','6','5'))
		{
			memset(m_pOSDbuf, 0x00, m_OpenOptions.dwWidth*m_OpenOptions.dwHeight*2);
		}
		
	}

	SetRect(&m_rSrcSize, 0, 0, m_OpenOptions.dwWidth, m_OpenOptions.dwHeight);

	m_pOverlay->GetScnRect(&m_rScreen);
	
	UpdateDisplayParameters();

/*********************Init VideoEffect**************************************/
	m_videoEffect.s32Brightness = SOFTWARE_VIDEO_EFFECT_INIT_VALUE;
	m_videoEffect.s32Contrast = SOFTWARE_VIDEO_EFFECT_INIT_VALUE;
	m_videoEffect.s32Saturation = SOFTWARE_VIDEO_EFFECT_INIT_VALUE;

	res = InitColorControl();
	if(FAILED(res))
	{
		return E_FAIL;
	}
/*********************Init VideoEffect End**************************************/


/*********************Init VideoWindow**************************************/
	if(!m_pVideoWindow)
		m_pVideoWindow = CreateVideoWindow();
	if(m_pVideoWindow == 0)
	{
		return E_FAIL;
	}

	VE_Options tVEOpen;
	ZeroMemory(&tVEOpen, sizeof(tVEOpen));

	tVEOpen.mode = (VE_MODE)m_lZoomMode;
	tVEOpen.rSrcClip.right = tVEOpen.rDst.right = tVEOpen.rSrc.right = m_OpenOptions.dwWidth;
	tVEOpen.rSrcClip.bottom = tVEOpen.rDst.bottom =  tVEOpen.rSrc.bottom = m_OpenOptions.dwHeight;
	tVEOpen.aspect.dwAspectX = m_OpenOptions.dwAspectX;
	tVEOpen.aspect.dwAspectY = m_OpenOptions.dwAspectY;
	
#ifdef FREESCALE_IMX51
	SetRect(&tVEOpen.rDst, 0,0 ,4, 4);
#endif
	res = m_pVideoWindow->Open(&tVEOpen,sizeof(tVEOpen));
	if(FAILED(res))
	{

		return E_FAIL;
	}
/*********************Init VideoWindow**************************************/

/*********************Init VideoMixer**************************************/

#ifndef MT3351_DBG
	if(!m_pvideoMixer)
		m_pvideoMixer = CreateFrameMixer();
	if(m_pvideoMixer == 0)
	{
		return E_FAIL;
	}

	struct VM_OpenOptions mixer_options;
	memset(&mixer_options, 0,sizeof(mixer_options));
	mixer_options.dwFourCC = m_OpenOptions.dwFourCC;
	mixer_options.dwWidth = m_OpenOptions.dwWidth;
	mixer_options.dwHeight = m_OpenOptions.dwHeight;
	if(FAILED(m_pvideoMixer->Open(&mixer_options, sizeof(mixer_options))))
	{
		m_pvideoMixer->Close();
		m_pvideoMixer->Release();
		m_pvideoMixer = 0;
	}
	else
	{
		if(FAILED(m_pvideoMixer->SetInterface(0,IID_IMcVideoPresenter,static_cast<IMcVideoPresenter*>(this))))
		{
			m_pvideoMixer->Close();
			m_pvideoMixer->Release();
			m_pvideoMixer = 0;
		}	
	}
#endif		
/*********************Init VideoMixer end**************************************/

	m_pDMCP = FindDmcpDisplay();
	if(m_pDMCP != 0)
	{
		DMCP_DisplayOpenParams dmcp_params;
		ZeroMemory(&dmcp_params,sizeof(dmcp_params));
		dmcp_params.pInfoFunc = 0;
		dmcp_params.pInfoFuncCtxt = 0;
		res = m_pDMCP->Open(&dmcp_params,sizeof(dmcp_params));
		if(FAILED(res))
		{
			DP("[GFXPRESENT]DMCP Open failed \n");
			m_pDMCP->Release();
			m_pDMCP = 0;
		}
		else
		{
			DMCP_DisplayState WrStatus;
			ZeroMemory(&WrStatus,sizeof(WrStatus));
			WrStatus.dwSize = sizeof(WrStatus);
			WrStatus.dwCPFlag = DMCP_CPFLAG_ACP | DMCP_CPFLAG_CGMSA | DMCP_CPFLAG_HDCP;
			WrStatus.dwACP = m_DMCP_ACP_flag;
			WrStatus.dwCGMSA= m_DMCP_CGMS_flag;
			WrStatus.dwHDCP= m_DMCP_HDCP_flag;
			
			res = m_pDMCP->SetDisplayState(&WrStatus);
			if(FAILED(res))
				DP("[GFXPRESENT]DMCP Set failed : res:%d __ %d \n",res, WrStatus.dwErrorCPFlag);

		}

	}
	else
		DP("[GFXPRESENT]Warning!  DMCP isn't found \n");



	m_lastpresent_tick = 0;
	m_bOpen = TRUE;
	m_bGoMixer = FALSE;

	#ifdef DUMP_FPS
	g_frames = 0;
	#endif


	//Clear in enabled state.
	m_lRenderEnabled = true;
#ifdef WINCE
	if(m_overlayType != VP_OPEN_FLAG_DDRAW_OSD)
		Set(IID_IMcVideoPresenter, MC_PROPID_VP_CLEAR,0,0,0,0);
#endif

#if defined(MT3351_DBG) || defined(MT3353)  || defined(SAMSUNG6410)	
	//|| defined(TITAN_II) 
	m_lRenderEnabled = true;
#elif defined(__linux__)
	m_lRenderEnabled = true;
#else
	m_lRenderEnabled = false;
#endif

#ifndef SIRF_A6
	if(m_overlayType == VP_OPEN_FLAG_DDRAW_OSD)
		m_lRenderEnabled = true;
#endif

	m_pOverlay->Show(m_lRenderEnabled);


	DP("[GFXPRESENT] open success");
	return S_OK;
}

STDMETHODIMP CVRPresentOvl::Close()
{
	if(!m_bOpen)
		return S_OK;

	CProtect proc(&m_csDisplay);

	if(m_pOSDbuf)
	{
		free(m_pOSDbuf);
		m_pOSDbuf = 0;
	}
	if(m_pDMCP)
	{
		m_pDMCP->Close();
		m_pDMCP->Release();
		m_pDMCP = 0;
	}

	for(int i=VP_SURF_LAYERTOP;i<=VP_SURF_LAYERBOTTOM;i++)
		DestroySurface((VR_SURFACE_TYPE)i);

	if(m_pvideoMixer)
	{
		m_pvideoMixer->Close();
	}

	if(m_pVideoWindow)
	{
		m_pVideoWindow->Close();
	}
	if(m_pOverlay)
	{
		m_pOverlay->Close();
	}
	m_bOpen = FALSE;
	
	return S_OK;
}

STDMETHODIMP  CVRPresentOvl::CreateSurface(VR_SURFACE_TYPE type, VP_SURFACE_OpenOptions* pparam)
{
	if(!m_bOpen)
		return E_FAIL;

	if((pparam == 0))
		return E_INVALIDARG;

	if(!m_pvideoMixer)
		return S_OK;
	VM_SURFACE_LAYER_ID id;
	ConvertVRType2VMType(type, &id);
	
	if(m_surfHandle[type].bOpen == FALSE)
	{
		struct VM_SurfaceOpenOptions opt;
		memset(&opt, 0 ,sizeof(opt));
		opt.dwWidth = pparam->dwWidth;
		opt.dwHeight = pparam->dwHeight;
		opt.dwInputFourCC = pparam->dwFourCC;
		opt.dwTranColor = pparam->dwTranColor;
		if(FAILED(m_pvideoMixer->CreateSurface(id,&opt)))
			return E_FAIL;
		m_surfHandle[type].bOpen = TRUE;

#if !defined(MT3353) && !defined(TITAN_II)
		m_bGoMixer = TRUE;
#endif

		return S_OK;
	}
	
	return E_FAIL;

}

STDMETHODIMP  CVRPresentOvl::DestroySurface(VR_SURFACE_TYPE type)
{
	if((!m_bOpen)||(!m_pvideoMixer))
		return S_OK;

	VM_SURFACE_LAYER_ID id;
	ConvertVRType2VMType(type, &id);
	if(m_surfHandle[type].bOpen == TRUE)
	{
		if(FAILED(m_pvideoMixer->DestroySurface(id)))
			return E_FAIL;
			
		m_surfHandle[type].bOpen = FALSE;
		return S_OK;
	}
	return S_OK;
}

STDMETHODIMP CVRPresentOvl::Reset()
{
	HRESULT res = S_OK;
	res = Close();
	if(FAILED(res))
		return E_FAIL;
	
	res = Open(&m_OpenOptions,sizeof(m_OpenOptions));
	if(FAILED(res))
		return E_FAIL;

	return S_OK;
}

HRESULT CVRPresentOvl::InitColorControl()
{
	memset(&m_alCC[0],0,sizeof(m_alCC));
	m_alCC[MC_PROPID_VP_BRIGHTNESS - MC_PROPID_VP_BRIGHTNESS] = HARDWARE_VIDEO_EFFECT_INIT_VALUE;
	m_alCC[MC_PROPID_VP_CONTRAST - MC_PROPID_VP_BRIGHTNESS] = HARDWARE_VIDEO_EFFECT_INIT_VALUE;
	m_alCC[MC_PROPID_VP_SATURATION - MC_PROPID_VP_BRIGHTNESS] = HARDWARE_VIDEO_EFFECT_INIT_VALUE;

	ZeroMemory(&m_sColorCaps, sizeof(m_sColorCaps));
	m_sColorCaps.dwSize = sizeof(m_sColorCaps);
	HRESULT hr = S_OK;
	if(m_pOverlay)
		hr = m_pOverlay->GetColorControl(GFXOVERLAY_SURF_OVERLAY,&m_sColorCaps);
	if(FAILED(hr))
	{
		m_sColorCaps.dwFlags |=DDCOLOR_BRIGHTNESS;
		m_sColorCaps.dwFlags |=DDCOLOR_CONTRAST;
		m_sColorCaps.dwFlags |=DDCOLOR_SATURATION;
		
		return S_OK;
	}
	if(m_sColorCaps.dwFlags&DDCOLOR_BRIGHTNESS)
	{
		ConvertPrecision(m_sColorCaps.lBrightness, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, MAX_BRIGHTNESS_INTERNAL, MIN_BRIGHTNESS_INTERNAL );
		m_alCC[MC_PROPID_VP_BRIGHTNESS - MC_PROPID_VP_BRIGHTNESS] = m_sColorCaps.lBrightness;
	}
	if(m_sColorCaps.dwFlags&DDCOLOR_CONTRAST)
	{
		ConvertPrecision(m_sColorCaps.lContrast, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, MAX_CONTRAST_INTERNAL, MIN_CONTRAST_INTERNAL );
		m_alCC[MC_PROPID_VP_CONTRAST- MC_PROPID_VP_BRIGHTNESS] = m_sColorCaps.lContrast;
	}
	if(m_sColorCaps.dwFlags&DDCOLOR_SATURATION)
	{
		ConvertPrecision(m_sColorCaps.lSaturation, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, MAX_SATURATION_INTERNAL, MIN_SATURATION_INTERNAL );
		m_alCC[MC_PROPID_VP_SATURATION- MC_PROPID_VP_BRIGHTNESS] = m_sColorCaps.lSaturation;
	}
	if(m_sColorCaps.dwFlags&DDCOLOR_HUE)
	{
		ConvertPrecision(m_sColorCaps.lHue, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, MAX_HUE_INTERNAL, MIN_HUE_INTERNAL );
		m_alCC[MC_PROPID_VP_HUE - MC_PROPID_VP_BRIGHTNESS] = m_sColorCaps.lHue;
	}
	if(m_sColorCaps.dwFlags&DDCOLOR_GAMMA)
	{
		ConvertPrecision(m_sColorCaps.lGamma, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, MAX_GAMMA_INTERNAL, MIN_GAMMA_INTERNAL );
		m_alCC[MC_PROPID_VP_GAMMA- MC_PROPID_VP_BRIGHTNESS] = m_sColorCaps.lGamma;
	}
	if(m_sColorCaps.dwFlags&DDCOLOR_COLORENABLE)
	{
		ConvertPrecision(m_sColorCaps.lColorEnable, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, MAX_COLOREN_INTERNAL, MIN_COLOREN_INTERNAL );
		m_alCC[MC_PROPID_VP_COLORENABLE- MC_PROPID_VP_BRIGHTNESS] = m_sColorCaps.lColorEnable;
	}
	if(m_sColorCaps.dwFlags&DDCOLOR_SHARPNESS)
	{
		ConvertPrecision(m_sColorCaps.lSharpness, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL, MAX_SHARPNESS_INTERNAL, MIN_SHARPNESS_INTERNAL );
		m_alCC[MC_PROPID_VP_SHARPNESS- MC_PROPID_VP_BRIGHTNESS] = m_sColorCaps.lSharpness;
	}

	if(!(m_sColorCaps.dwFlags&DDCOLOR_BRIGHTNESS))
	{
		m_sColorCaps.dwFlags |=DDCOLOR_BRIGHTNESS;
	}
		
	if(!(m_sColorCaps.dwFlags&DDCOLOR_CONTRAST))
	{
		m_sColorCaps.dwFlags |=DDCOLOR_CONTRAST;
	}
	
	if(!(m_sColorCaps.dwFlags&DDCOLOR_SATURATION))
	{
		m_sColorCaps.dwFlags |=DDCOLOR_SATURATION;
	}
	
	return S_OK;
}

HRESULT CVRPresentOvl::RestoreColorControl()
{
	LONG value;

	for(int i= MC_PROPID_VP_BRIGHTNESS;i<=MC_PROPID_VP_SHARPNESS ;i++)
	{
		if(SUCCEEDED(Get(IID_IMcVideoPresenter,i,0,0,(LPVOID)&value,0,0)))
		{
			Set(IID_IMcVideoPresenter, i,0,0,(LPVOID)&value,0);
		}
	}

	return S_OK;
}

STDMETHODIMP CVRPresentOvl::SetInterface(DWORD dwRoute, REFIID riid, LPUNKNOWN pUnk)
{
	return E_FAIL;
}

STDMETHODIMP CVRPresentOvl::Get(REFGUID guidPropSet, DWORD dwPropID, 
  LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, 
  DWORD cbPropData, DWORD *pcbReturned)
{

	switch (dwPropID)
	{
	case MC_PROPID_VP_VSYNC:
		return ReturnComLong(m_vsyncMode ,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_ZOOM_ENABLE:
		return ReturnComLong(m_bZoomEnable,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_ZOOM:
		return ReturnComLong(m_lZoomMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_ROTATION:
		return ReturnComLong(m_lRotationMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_INTERPOLATION:
		return ReturnComLong(m_lInterpolationMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_DEBLOCK:
		return ReturnComLong(m_lDeblockMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_RESAMPLE:
		return ReturnComLong(m_lResampleMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_ENABLE:
		return ReturnComLong(m_lRenderEnabled,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_TYPE:
		return ReturnComLong(m_overlayType,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_DMCP_ENABLE:
		return ReturnComLong(m_bDMCPEnable,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_FLIPVIDEO:
		return ReturnComLong(m_lFlipVideoMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_DEINTERLACE_MODE:
		{
			if(m_pOverlay)
				return (m_pOverlay->GetDeinterlaceMode((DWORD*)pPropData));
			else
				return S_OK;
		}
	case MC_PROPID_VP_RECTDST_DEVICE:
	case MC_PROPID_VP_RECTDST:
		{
			if(pPropData==0)
				return E_POINTER;
			*reinterpret_cast<RECT *>(pPropData) = m_rDst;
			if(pcbReturned)
				*pcbReturned = sizeof(RECT);
			return S_OK;
		}
 	case MC_PROPID_VP_RECTSRC:
		{
			if(pPropData==0)
				return E_POINTER;
			*reinterpret_cast<RECT *>(pPropData) = m_rSrcSize;
			if(pcbReturned)
				*pcbReturned = sizeof(RECT);
			return S_OK;
		}
	case MC_PROPID_VP_RECT_SCREEN:
		{
			if(pPropData==0)
				return E_POINTER;

#ifdef GFX_ANDROID_SURFACE
	 		HRESULT hr = E_FAIL;
			if(m_pOverlay)
				m_pOverlay->GetScnRect(&m_rScreen);
			if(FAILED(hr))
				DP("%s :%d failed \n",__FUNCTION__, __LINE__);
#endif
			*reinterpret_cast<RECT *>(pPropData) = m_rScreen;
			if(pcbReturned)
				*pcbReturned = sizeof(RECT);
			return S_OK;
		}
	case MC_PROPID_VP_BRIGHTNESS:
	case MC_PROPID_VP_CONTRAST:
	case MC_PROPID_VP_HUE:
	case MC_PROPID_VP_SATURATION:
	case MC_PROPID_VP_GAMMA:
	case MC_PROPID_VP_COLORENABLE:
	case MC_PROPID_VP_SHARPNESS:
		{
			if(pPropData==0)
				return E_POINTER;

			int index = dwPropID - MC_PROPID_VP_BRIGHTNESS;

			*reinterpret_cast<LONG *>(pPropData) = m_alCC[index];
			if(pcbReturned)
				*pcbReturned = sizeof(LONG);
			return S_OK;

/*
			//Software video effect
			if((dwPropID == MC_PROPID_VP_BRIGHTNESS))
			{
				*reinterpret_cast<LONG *>(pPropData) = m_videoEffect.s32Brightness;
				if(pcbReturned)
					*pcbReturned = sizeof(LONG);
				return S_OK;
			}
			if((dwPropID == MC_PROPID_VP_CONTRAST))
			{
				*reinterpret_cast<LONG *>(pPropData) = m_videoEffect.s32Contrast;
				if(pcbReturned)
					*pcbReturned = sizeof(LONG);
				return S_OK;
			}
			if((dwPropID == MC_PROPID_VP_SATURATION))
			{
				*reinterpret_cast<LONG *>(pPropData) = m_videoEffect.s32Saturation;
				if(pcbReturned)
					*pcbReturned = sizeof(LONG);
				return S_OK;
			}
*/
		}
	case MC_PROPID_VP_COLORSYS_CAP:
		{
			if(pPropData==0)
				return E_POINTER;
			
			*reinterpret_cast<DDCOLORCONTROL *>(pPropData) = m_sColorCaps;
			if(pcbReturned)
				*pcbReturned = sizeof(DDCOLORCONTROL);
			return S_OK;
		}
	case MC_PROPID_VP_WND:
		{
			HRESULT ret = E_FAIL;
		#if defined(__linux__) && !defined(__QNX__) 
			Window wnd;
		#else
			HWND wnd;
		#endif
			if(pPropData==0)
				return E_POINTER;
			if(m_pOverlay == 0)
				return E_FAIL;
			ZeroMemory(&wnd,sizeof(wnd));

			CProtect proc(&m_csDisplay);
			ret = m_pOverlay->GetWindowHandle(wnd);
			if(FAILED(ret))
				return ret;
		#if defined(__linux__) && !defined(__QNX__) 
			*reinterpret_cast<Window *>(pPropData) = wnd;
		#else			
			*reinterpret_cast<HWND *>(pPropData) = wnd;
		#endif
			if(pcbReturned)
				*pcbReturned = sizeof(HWND);
			return S_OK;
		}
	#ifdef MX37_NONMEMCPY
	case MC_PROPID_VP_VIDEO_MEMORY:
		{
			DWORD* pNum = (DWORD*)pInstanceData;
			
			VP_VIDEO_MEMORY_INFO* pInfo = (VP_VIDEO_MEMORY_INFO*)pPropData;
			if((m_pOverlay == 0)||(pNum == 0 )||(*pNum >= m_OpenOptions.dwBuffers)||(pInfo == 0))
				return E_FAIL;

			if(pVideoMemoryInfo == 0)
			{
				CProtect proc(&m_csDisplay);

				pVideoMemoryInfo = new VP_VIDEO_MEMORY_INFO[m_OpenOptions.dwBuffers];
				if(!pVideoMemoryInfo)
					return E_FAIL;
				
				if(m_pOverlay)
				{
					m_pOverlay->Lock(GFXOVERLAY_SURF_OVERLAY,(LPVOID *)&m_bufferMemoryInfo.pBuf,&m_bufferMemoryInfo.lStride,0);
					m_pOverlay->Unlock(GFXOVERLAY_SURF_OVERLAY);
					for(int i=0;i<m_OpenOptions.dwBuffers;i++)
					{
						if(FAILED(m_pOverlay->Lock(i,(LPVOID *)&pVideoMemoryInfo[i].pBuf,&pVideoMemoryInfo[i].lStride,0)))
						{
							delete pVideoMemoryInfo;
							pVideoMemoryInfo = 0;
							return E_FAIL;
						}
						m_pOverlay->Unlock(i);
					}
				}
			}

			pInfo->pBuf = pVideoMemoryInfo[*pNum].pBuf;
			pInfo->lStride = pVideoMemoryInfo[*pNum].lStride;

			return S_OK;
		}
	#endif
#if defined(TITAN_II)
	case MC_PROPID_VP_TITIANII_HND:
		{
			if(m_pOverlay)
				return (m_pOverlay->GetVRHandle(pPropData));
			else
				return S_OK;
		}
#endif
	case MC_PROPID_VP_DMCP_ACP:
	case MC_PROPID_VP_DMCP_CGMS:
	case MC_PROPID_VP_DMCP_HDCP:
		{
			if(pPropData == 0)
				return E_INVALIDARG;

			if(m_pDMCP == 0)
				return E_ACCESSDENIED;

			DWORD dmcp_flag = 0;
			DMCP_DisplayState RdStatus;
			ZeroMemory(&RdStatus,sizeof(RdStatus));
			RdStatus.dwSize = sizeof(RdStatus);
			m_pDMCP->GetDisplayState(&RdStatus);
			if(dwPropID == MC_PROPID_VP_DMCP_ACP)
			{
				if(RdStatus.dwCPFlag & DMCP_CPFLAG_ACP)
				{
					switch(RdStatus.dwACP)
					{
						case DMCP_ACP_Level0:
							dmcp_flag = VP_DMCP_ACP_Level0;
							break;
						case DMCP_ACP_Level1:
							dmcp_flag = VP_DMCP_ACP_Level1;
							break;
						case DMCP_ACP_Level2:
							dmcp_flag = VP_DMCP_ACP_Level2;
							break;
						case DMCP_ACP_Level3:
							dmcp_flag = VP_DMCP_ACP_Level3;
							break;
						default:
							return E_ACCESSDENIED;
					}

					*reinterpret_cast<DWORD*>(pPropData) = dmcp_flag;
					return S_OK;
				}
				else
					return E_ACCESSDENIED;
			}
			else if(dwPropID == MC_PROPID_VP_DMCP_CGMS)
			{
				if(RdStatus.dwCPFlag & DMCP_CPFLAG_CGMSA)
				{
					switch(RdStatus.dwCGMSA)
					{
						case DMCP_CGMSA_Disabled :
							dmcp_flag = VP_DMCP_CGMSA_Disabled ;
							break;
						case DMCP_CGMSA_CopyFreely:
							dmcp_flag = VP_DMCP_CGMSA_CopyFreely;
							break;
						case DMCP_CGMSA_CopyNoMore:
							dmcp_flag = VP_DMCP_CGMSA_CopyNoMore;
							break;
						case DMCP_CGMSA_CopyOneGeneration:
							dmcp_flag = VP_DMCP_CGMSA_CopyOneGeneration;
						case DMCP_CGMSA_CopyNever:
							dmcp_flag = VP_DMCP_CGMSA_CopyNever;
							break;
						default:
							return E_ACCESSDENIED;
					}
					*reinterpret_cast<DWORD*>(pPropData) = dmcp_flag;
					return S_OK;
				}
				else
					return E_ACCESSDENIED;
			}
			else if(dwPropID == MC_PROPID_VP_DMCP_HDCP)
			{
				if(RdStatus.dwCPFlag & DMCP_CPFLAG_HDCP)
				{
					switch(RdStatus.dwHDCP)
					{
						case DMCP_HDCP_Level0:
							dmcp_flag = VP_DMCP_HDCP_Level0;
							break;
						case DMCP_HDCP_Level1:
							dmcp_flag = VP_DMCP_HDCP_Level1;
							break;
						default:
							return E_ACCESSDENIED;
					}
					*reinterpret_cast<DWORD*>(pPropData) = dmcp_flag;
					return S_OK;
				}
				else
					return E_ACCESSDENIED;
			}
			else
				return E_FAIL;
			break;
		}
		case MC_PROPID_VP_INTRNAL_SURFINFO:
		{
			if(m_pOverlay)
				return (m_pOverlay->GetSurfaceInfo((GfxOverlayInfo *)pPropData));
			else
				return S_OK;
		}
		
		

	}
	return E_PROP_ID_UNSUPPORTED;
}

STDMETHODIMP CVRPresentOvl::Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
{

	if(!m_bOpen)
		return E_FAIL;

	switch (dwPropID)
	{
	case MC_PROPID_VP_DLL_DIRECTORY:
	{
#ifdef ANDROID_NDK
		if(pPropData)
			return (SetDllDirectory((LPCSTR)pPropData));
		return E_INVALIDARG;
#endif
	}
   case MC_PROPID_VP_FLIPDELAY:
   {
   #ifdef ST_LINUX
			LONG delay = *(LONG *)pPropData;
			//DP("set delay %d \n",delay);
			if(m_pOverlay)
				return m_pOverlay->SetFLipDelay(delay);
			else
				return S_OK;
  #else
			return E_NOTIMPL;
  #endif
   }
	case MC_PROPID_VP_ZOOM_ENABLE:
		{
			LONG lZoomEnable = *(BOOL *)pPropData;
			m_bZoomEnable = lZoomEnable ;
			return S_OK;
		}
	case MC_PROPID_VP_VSYNC:
		{
			m_vsyncMode = *(VP_VSYNC_MODE*)pPropData;
			if(m_pOverlay)
				return (m_pOverlay->SetVSYNC(m_vsyncMode));
			else
				return S_OK;
		}
		
	case MC_PROPID_VP_ZOOM:
		{
#ifndef ANDROID
			LONG lZoomMode = *(LONG *)pPropData;
#else
			short lZoomMode = *(short *)pPropData;
#endif

			if(m_lZoomMode!=lZoomMode)
			{
#ifdef  SIRF_A4_PVEGPS
				Set(IID_IMcVideoPresenter, MC_PROPID_VP_CLEAR,0,0,0,0);
#endif
				HRESULT ret;
				m_lZoomMode = lZoomMode;
				LONG caps = 0;
				if(m_pOverlay && SUCCEEDED(m_pOverlay->GetDDCaps(&caps)))
				{
					if(caps &GFXOVERLAY_CAPS_ZOOM_SUPPORT )
					{
						ret = m_pOverlay->SetDisplayMode(m_lZoomMode);
						if(FAILED(ret))	DP("HW GFXOVERLAY_CAPS_ZOOM_SUPPORT:%d \n", ret);
						return S_OK;
					}
				}


				if(m_pVideoWindow)
				{
					ret = m_pVideoWindow->Set(IID_IMcVideoPresenter, VE_COMMAND_MODE,0,0,&m_lZoomMode, sizeof(m_lZoomMode));
					if(SUCCEEDED(ret) )
					{
						#ifndef ST_SAVECOPY
						if(m_lRenderEnabled)
						#endif
							SetVideoWndDst();
					}
				}
			}
			return S_OK;
		}
	case MC_PROPID_VP_FLIPVIDEO:
		{
			LONG lMode = *(LONG *)pPropData;
			CProtect proc(&m_csDisplay);

			if((lMode != VP_FLIPH) && (lMode != VP_FLIPV) && (lMode != VP_FLIPVH_NO) )
				return E_INVALIDARG;
				
			m_lFlipVideoMode = lMode;

			return S_OK;
		}
	case MC_PROPID_VP_ROTATION:
		{
			LONG lRotationMode = *(LONG *)pPropData;

			CProtect proc(&m_csDisplay);

			if(m_lRotationMode!=lRotationMode)
			{
				m_lRotationMode = lRotationMode;
				UpdateDisplayParameters();
			}
			return S_OK;
		}
	case MC_PROPID_VP_RECTDST:
		{
			#ifndef ANDROID
			#ifndef __linux__
			if(m_OpenOptions.hWnd)
			{
				UpdateDestPos();
				return S_OK;
			}
			#endif
			#endif
			RECT r;
			r = *reinterpret_cast<RECT *>(pPropData);
			if(r.left == 0 && r.top==0 && r.right==0 && r.bottom == 0)	return E_INVALIDARG;

			if((m_OpenOptions.hWnd) && (m_overlayType != VP_OPEN_FLAG_D3D))
			{
				#ifndef ANDROID
				#ifndef __linux__
				POINT p={0,0};

				CProtect proc(&m_csDisplay);

				::ClientToScreen((HWND)m_OpenOptions.hWnd, &p);
				OffsetRect(&r,p.x,p.y);
				#endif
				#endif
			}

//				DP("[GFXPRESENTOVL] Set DestRecatangle (%d,%d,%d,%d)\n", r.left, r.top,r.right, r.bottom);
			if((rVideoWnd .left == r.left) &&(rVideoWnd .top == r.top) &&
				(rVideoWnd .right == r.right) && (rVideoWnd .bottom == r.bottom)
				)
				return S_OK;
			if(m_pVideoWindow)
			{
				HRESULT ret;
				ret = m_pVideoWindow->Set(IID_IMcVideoPresenter, VE_COMMAND_DSTRECT,0,0,&r, sizeof(r));
				if(FAILED(ret))
					return ret;
				//if(m_lRenderEnabled)
				SetVideoWndDst();
			}
			rVideoWnd  = r;
			return S_OK;
		}
#ifndef NO_ROT_SCL
	case MC_PROPID_VP_RECTSRC:
		{
			RECT *pr = reinterpret_cast<RECT *>(pPropData);
			//EnterCriticalSection(&m_csDisplay);
			if(!EqualRect(&m_rSrcSize,pr))
			{
				m_rSrcSize = *pr;
				UpdateDisplayParameters();
			}
			//LeaveCriticalSection(&m_csDisplay);
			return S_OK;
		}
#endif
	case MC_PROPID_VP_ENABLE:
		{
			bool lEnable = *(bool*)pPropData;
			HRESULT	 ret = S_OK;

			CProtect proc(&m_csDisplay);
			if(m_lRenderEnabled!=lEnable)
			{
				LONG caps = 0;
				if(m_pOverlay && SUCCEEDED(m_pOverlay->GetDDCaps(&caps)))
				{
					if(caps&GFXOVERLAY_CAPS_HIDE_NOT_SUPPORT)
					{
						if(lEnable)
							ret = Restore(FALSE);
						else
							ret = m_pOverlay->Close();

						if(SUCCEEDED(ret))
							m_lRenderEnabled = lEnable;
						return ret;

					}

				}


				m_lRenderEnabled = lEnable;

#if defined(SIRF_A4_NEWOS) || defined(TI3630) 
#ifndef SIRF_A6
				if(m_lRenderEnabled)
				{
					Set(IID_IMcVideoPresenter, MC_PROPID_VP_CLEAR,0,0,0,0);
				}
#endif
#endif

#ifndef RMI_CE6
				if(m_lRenderEnabled)
					SetVideoWndDst();
#endif
				if(m_pOverlay)
					m_pOverlay->Show(lEnable);
				UpdateDisplayParameters();


				

			}
			return ret;
		}
	case MC_PROPID_VP_BRIGHTNESS:
	case MC_PROPID_VP_CONTRAST:
	case MC_PROPID_VP_HUE:
	case MC_PROPID_VP_SATURATION:
	case MC_PROPID_VP_GAMMA:
	case MC_PROPID_VP_COLORENABLE:
	case MC_PROPID_VP_SHARPNESS:
		{
			LONG lValue= *(LONG *)pPropData;
			int index = dwPropID - MC_PROPID_VP_BRIGHTNESS;

			if(lValue > MAX_COLORCONTROL_VAL)
				lValue = MAX_COLORCONTROL_VAL;
			if(lValue < MIN_COLORCONTROL_VAL)
				lValue = MIN_COLORCONTROL_VAL;

			if((dwPropID != MC_PROPID_VP_BRIGHTNESS)	&&(dwPropID != MC_PROPID_VP_CONTRAST)
				&&(dwPropID != MC_PROPID_VP_SATURATION))
			return E_NOTIMPL;


			//Hardware video effcet
			//EnterCriticalSection(&m_csDisplay);
				
			//Save value
			{
				m_alCC[index] = lValue;
				if(m_pOverlay)
				{
					DDCOLORCONTROL sColor;
					HRESULT hr;
					ZeroMemory(&sColor, sizeof(sColor));
					sColor.dwSize = sizeof(sColor);
					switch(dwPropID)
					{
						case MC_PROPID_VP_BRIGHTNESS:
							if(!(m_sColorCaps.dwFlags&DDCOLOR_BRIGHTNESS))
								goto quit;
							ConvertPrecision(lValue, MAX_BRIGHTNESS_INTERNAL, MIN_BRIGHTNESS_INTERNAL, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL );
							sColor.dwFlags = DDCOLOR_BRIGHTNESS;
							sColor.lBrightness = lValue;  //Luminance intensity, in IRE units times 100. The valid range is 0 to 10,000
							break;
						case MC_PROPID_VP_CONTRAST:
							if(!(m_sColorCaps.dwFlags&DDCOLOR_CONTRAST))
								goto quit;
							ConvertPrecision(lValue, MAX_CONTRAST_INTERNAL, MIN_CONTRAST_INTERNAL, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL );
							sColor.dwFlags = DDCOLOR_CONTRAST ;
							sColor.lContrast  = lValue;  
							break;
						case MC_PROPID_VP_HUE:
							if(!(m_sColorCaps.dwFlags&DDCOLOR_HUE))
								goto quit;
							ConvertPrecision(lValue, MAX_HUE_INTERNAL, MIN_HUE_INTERNAL, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL );
							sColor.dwFlags = DDCOLOR_HUE ;
							sColor.lHue  = lValue;  
							break;
						case MC_PROPID_VP_SATURATION:
							if(!(m_sColorCaps.dwFlags&DDCOLOR_SATURATION))
								goto quit;
							ConvertPrecision(lValue, MAX_SATURATION_INTERNAL, MIN_SATURATION_INTERNAL, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL );
							sColor.dwFlags = DDCOLOR_SATURATION;
							sColor.lSaturation  = lValue;  
							break;
						case MC_PROPID_VP_GAMMA:
							if(!(m_sColorCaps.dwFlags&DDCOLOR_GAMMA))
								goto quit;
							ConvertPrecision(lValue, MAX_GAMMA_INTERNAL, MIN_GAMMA_INTERNAL, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL );
							sColor.dwFlags = DDCOLOR_GAMMA;
							sColor.lGamma  = lValue;  
							break;
						case MC_PROPID_VP_COLORENABLE:
							if(!(m_sColorCaps.dwFlags&DDCOLOR_COLORENABLE))
								goto quit;
							ConvertPrecision(lValue, MAX_COLOREN_INTERNAL, MIN_COLOREN_INTERNAL, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL );
							sColor.dwFlags = DDCOLOR_COLORENABLE;
							sColor.lColorEnable  = lValue;  
							break;
						case MC_PROPID_VP_SHARPNESS:
							if(!(m_sColorCaps.dwFlags&DDCOLOR_SHARPNESS))
								goto quit;
							ConvertPrecision(lValue, MAX_SHARPNESS_INTERNAL, MIN_SHARPNESS_INTERNAL, MAX_COLORCONTROL_VAL,MIN_COLORCONTROL_VAL );
							sColor.dwFlags = DDCOLOR_SHARPNESS;
							sColor.lSharpness  = lValue;  
							break;
						default:
							goto quit;
							break;
					}
					hr = m_pOverlay->SetColorControl(GFXOVERLAY_SURF_OVERLAY,&sColor);
					if(FAILED(hr))
					{
						DP("[GFXPRESENTOVL]SetColorControl: [%d]->%d failed ==%d\n",dwPropID, lValue, hr);
					quit:
						DP("[GFXPRESENTOVL]SetColorControl[%d] isn't supportted by HW\n",dwPropID);

						//It's hw not supported. use SW mode
						{
							if(dwPropID == MC_PROPID_VP_BRIGHTNESS)	//Do software conversion. Notice: the performance comes down.
							{
								m_videoEffect.s32Brightness = *(LONG *)pPropData;
							}
							else if(dwPropID == MC_PROPID_VP_CONTRAST)
							{
								m_videoEffect.s32Contrast = *(LONG *)pPropData;
							}
							else if(dwPropID == MC_PROPID_VP_SATURATION)
							{
								m_videoEffect.s32Saturation = *(LONG *)pPropData;
							}
							else
							{
								return E_NOTIMPL;
							}
							hr = m_PrepareData.Set(MC_PROPID_GFXPREPARE_VIDEOEFFECT,&m_videoEffect,sizeof(m_videoEffect));
							return hr;			

						}

					}


				}
			}
			//LeaveCriticalSection(&m_csDisplay);
			return S_OK;
		}

	case MC_PROPID_VP_RESAMPLE:
		{
			LONG lResampleMode = *(LONG *)pPropData;

			CProtect proc(&m_csDisplay);
			if(m_lResampleMode!=lResampleMode)
			{
				m_lResampleMode = lResampleMode;
			}
			return S_OK;
		}
	case MC_PROPID_VP_INTERPOLATION:
		{
			LONG lInterpolationMode = *(LONG *)pPropData;

			m_lInterpolationMode = lInterpolationMode;
			return S_OK;
		}
	case MC_PROPID_VP_DEBLOCK:
		{
			LONG lDeblockMode = *(LONG *)pPropData;

			m_lDeblockMode = lDeblockMode;
			return S_OK;
		}
	case MC_PROPID_VP_COLORKEYDST:
		{
			DWORD dwCkeyDst = *(DWORD *)pPropData;
			DWORD dwfinalKey;
			if(m_pOverlay)
			{
				if(FAILED(TransferColorKey(dwCkeyDst,&dwfinalKey)))
					return E_FAIL;
				
				CProtect proc(&m_csDisplay);
				HRESULT hr = m_pOverlay->SetDstColorKey(TRUE,dwfinalKey);
				return hr;
			}
			else
				return S_OK;
		}
	case MC_PROPID_VP_COLORKEYSRC:
		{
			m_bCkeySrc = true;
			m_dwCkeySrc  = *(DWORD *)pPropData;
			DWORD dwfinalKey;
			if(m_pOverlay)
			{
				if(FAILED(TransferColorKey(m_dwCkeySrc,&dwfinalKey)))
					return E_FAIL;

				CProtect proc(&m_csDisplay);

				HRESULT hr = m_pOverlay->SetSrcColorKey(TRUE,dwfinalKey);

				 if(m_bPresented == false)
				 {
					if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','V','1','2'))
					{
						char y,u,v;
						y = (char)((m_dwCkeySrc>>16)&0xff);
						u = (char)((m_dwCkeySrc>>8)&0xff);
						v = (char)((m_dwCkeySrc>>0)&0xff);
						memset(m_pOSDbuf, y, m_OpenOptions.dwWidth*m_OpenOptions.dwHeight);
						memset(m_pOSDbuf+m_OpenOptions.dwWidth*m_OpenOptions.dwHeight,
							u, m_OpenOptions.dwWidth*m_OpenOptions.dwHeight/4);
						memset(m_pOSDbuf +m_OpenOptions.dwWidth*m_OpenOptions.dwHeight*5/4
							, v, m_OpenOptions.dwWidth*m_OpenOptions.dwHeight/4);
					}
				 }
				return hr;
			}
			else
				return S_OK;
		}
	case MC_PROPID_VP_WND:
		{
			HRESULT res = S_OK;
			#if defined(__linux__) && !defined(__QNX__) 
           		Window* d = (Window*)pPropData;
			#else
           		HWND* d = (HWND*)pPropData;
			#endif
			if(d)
			{
				CProtect proc(&m_csDisplay);

				m_OpenOptions.hWnd = (LPVOID)*d;
				UpdateDestPos();
				res = UpdateDisplayParameters();
#ifdef MTK3353_HW
				PresentBuffer(m_dwPresentIndex,0);
#endif
			}
			return res;
		}
	case MC_PROPID_VP_ALPHA:
		{
			LONG lAlpha = *(LONG *)pPropData;
			ConvertPrecision(lAlpha, MAX_ALPHA_INTERNAL, MIN_ALPHA_INTERNAL, MAX_ALPHA_VAL,MIN_ALPHA_VAL);
			m_dwAlpha = lAlpha;

			CProtect proc(&m_csDisplay);
			HRESULT hr  = E_FAIL;
			if(m_pOverlay)
				hr = m_pOverlay->SetAlpha(lAlpha);
			return (hr);
		}
	case MC_PROPID_VP_DEINTERLACE_MODE:
		{
#if defined(SIRF_A4_NEWOS) || defined(MT3353) || defined(MT3351_DBG)
			return	S_OK;
#endif
			VP_DEINTERLACE_MODE mode = *(VP_DEINTERLACE_MODE *)pPropData; 
			if(mode  == m_Deinterlaced_Mode )
				return S_OK;

			m_Deinterlaced_Mode = mode;
#if defined(PRIMAII_SAVECOPY)
			EnterCriticalSection(&m_csDisplay);
			HRESULT hr = m_pOverlay->SetDeinterlaceMode(m_Deinterlaced_Mode);
			LeaveCriticalSection(&m_csDisplay);
#endif
			/*
			DWORD dwInter = GFXOVERLAY_DEINTERLACE_WEAVE;
			switch(m_Deinterlaced_Mode)
			{
			case VP_DEINTERLACE_WEAVE:
				dwInter = GFXOVERLAY_DEINTERLACE_WEAVE;
				break;
			case VP_DEINTERLACE_BOB_SINGLE_FIELD:
				dwInter = GFXOVERLAY_DEINTERLACE_BOB_SINGLE_FIELD;
				break;
			case VP_DEINTERLACE_BOB_DOUBLE_FIELD:
				dwInter = GFXOVERLAY_DEINTERLACE_BOB_DOUBLE_FIELD;
				break;
			default:
				break;
			}
			EnterCriticalSection(&m_csDisplay);
			HRESULT hr = m_pOverlay->SetDeinterlaceMode(dwInter);
			LeaveCriticalSection(&m_csDisplay);
			*/
			return S_OK;
		}

	case MC_PROPID_VP_DMCP_ACP:
	case MC_PROPID_VP_DMCP_CGMS:
	case MC_PROPID_VP_DMCP_HDCP:
		{
			if(pPropData == 0)
				return E_INVALIDARG;

			if(m_pDMCP == 0)
				return E_ACCESSDENIED;

			DWORD dmcp_flag = *(DWORD*)pPropData;
			DMCP_DisplayState WrStatus;
			ZeroMemory(&WrStatus,sizeof(WrStatus));
			WrStatus.dwSize = sizeof(WrStatus);

			if(dwPropID == MC_PROPID_VP_DMCP_ACP)
			{
				WrStatus.dwCPFlag = DMCP_CPFLAG_ACP;
				switch(dmcp_flag)
				{
					case VP_DMCP_ACP_Level0:
						dmcp_flag = DMCP_ACP_Level0;
						break;
					case VP_DMCP_ACP_Level1:
						dmcp_flag = DMCP_ACP_Level1;
						break;
					case VP_DMCP_ACP_Level2:
						dmcp_flag = DMCP_ACP_Level2;
						break;
					case VP_DMCP_ACP_Level3:
						dmcp_flag = DMCP_ACP_Level3;
						break;
					default:
						return E_INVALIDARG;
				}
				WrStatus.dwACP = dmcp_flag;
				m_DMCP_ACP_flag = dmcp_flag;
			}
			else if(dwPropID == MC_PROPID_VP_DMCP_CGMS)
			{
				WrStatus.dwCPFlag = DMCP_CPFLAG_CGMSA;
				switch(dmcp_flag)
				{
					case VP_DMCP_CGMSA_Disabled:
						dmcp_flag = DMCP_CGMSA_Disabled;
						break;
					case VP_DMCP_CGMSA_CopyFreely:
						dmcp_flag = DMCP_CGMSA_CopyFreely;
						break;
					case VP_DMCP_CGMSA_CopyNoMore:
						dmcp_flag = DMCP_CGMSA_CopyNoMore;
						break;
					case VP_DMCP_CGMSA_CopyOneGeneration:
						dmcp_flag = DMCP_CGMSA_CopyOneGeneration;
						break;
					case VP_DMCP_CGMSA_CopyNever:
						dmcp_flag = DMCP_CGMSA_CopyNever;
						break;
					default:
						return E_INVALIDARG;
				}
				WrStatus.dwCGMSA= dmcp_flag;
				m_DMCP_CGMS_flag = dmcp_flag;
			}
			else if(dwPropID == MC_PROPID_VP_DMCP_HDCP)
			{
				WrStatus.dwCPFlag = DMCP_CPFLAG_HDCP;
				switch(dmcp_flag)
				{
					case VP_DMCP_HDCP_Level0 :
						dmcp_flag = DMCP_HDCP_Level0 ;
						break;
					case VP_DMCP_HDCP_Level1:
						dmcp_flag = DMCP_HDCP_Level1;
						break;
					default:
						return E_INVALIDARG;
				}
				WrStatus.dwHDCP= dmcp_flag;
				m_DMCP_HDCP_flag = dmcp_flag;
			}
			else 
				return E_FAIL;

			CProtect proc(&m_csDisplay);
			HRESULT hr = m_pDMCP->SetDisplayState(&WrStatus);
			return hr;
		}
	case MC_PROPID_VP_ANDROID_ISURFACE:
	case MC_PROPID_VP_VP_HND:			
			{
				//DP("CVRPresentOvl::Set MC_PROPID_VP_ISURFACE\n");
				if(m_pOverlay)
					return m_pOverlay->SetHWRenderSurf(pPropData);
				else
					return S_OK;
			}
	case MC_PROPID_VP_ANDROID_SURFACE:
          {
            if(m_overlayType != VP_OPEN_FLAG_ANDROID_SURFACE)
              return E_NOTIMPL;
			if(m_pOverlay)
			    	return m_pOverlay->SetHWRenderSurf(pPropData,1);
			else
				return S_OK;
          }
     case MC_PROPID_VP_ANDROID_ISURFACE_SURFACE:
			{
				if(m_pOverlay)
					return m_pOverlay->SetHWRenderSurf(pPropData,HW_RENDER_ANDROID_SURFACE);
				else
					return S_OK;
			}	
     case MC_PROPID_VP_ANDROID_ISURFACE_ANATIVEWINDOW:
			{
				if(m_pOverlay)
					return m_pOverlay->SetHWRenderSurf(pPropData,HW_RENDER_ANDROID_ANATIVEWINDOW);
				else
					return S_OK;
			}	

#if defined(TITAN_II)
	case MC_PROPID_VP_TITIANII_HND:
		{
			if(m_pOverlay)
				return (m_pOverlay->SetVRHandle(pPropData));
			else
				return S_OK;
		}
#endif
	case MC_PROPID_VP_CLEAR_BUFFER:
	case MC_PROPID_VP_CLEAR:
		{
#ifdef MT3353
			if(m_overlayType == VP_OPEN_FLAG_DDRAW)
				return S_OK;

#elif defined(PRIMAII_SAVECOPY) || defined(ST_SAVECOPY_LINUX) 
			return S_OK;
#elif defined(ST_SAVECOPY)
			if(m_OpenOptions.dwSurfType&VP_OPEN_FLAG_ST_EXTERNALBUFFER)
				return S_OK;
#endif
			HRESULT hr = S_OK;
			DWORD clearWidth=0;
			DWORD clearHeight=0;

			if(m_unsupportSizeFlag)
			{
				clearWidth = m_unsupportOptions.dwWidth;
				clearHeight = m_unsupportOptions.dwHeight;
			}
			else
			{
				clearWidth = m_OpenOptions.dwWidth;
				clearHeight = m_OpenOptions.dwHeight;
			}

#ifdef PRIMAII
		if(m_overlayType == VP_OPEN_FLAG_DDRAW)
		{
			if(m_OpenOptions.dwWidth&(64-1))
				clearWidth = (m_OpenOptions.dwWidth&~(64-1)) + 64;
			else
				clearWidth = m_OpenOptions.dwWidth;

			if(m_OpenOptions.dwHeight&(64-1))
				clearHeight = (m_OpenOptions.dwHeight&~(64-1)) + 64;
			else
				clearHeight = m_OpenOptions.dwHeight;
		}
#endif

			float bytePerPixcel  = 0.0f;
			if((m_OpenOptions.dwFourCC == MAKEFOURCC('Y','V','1','2'))
				|| (m_OpenOptions.dwFourCC == MAKEFOURCC('N','V','1','2')))
				bytePerPixcel  = 1.5f;
			else if(m_OpenOptions.dwFourCC == MAKEFOURCC('R','5','6','5'))
				bytePerPixcel  = 2.0;
			else
				return E_NOTIMPL;
			
			LPBYTE pbuf = 0;
			pbuf = (LPBYTE)malloc((DWORD)(clearWidth*clearHeight*bytePerPixcel));
			if(!pbuf)
				return E_FAIL;
			if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','V','1','2'))
			{
				unsigned char y = (unsigned char)(m_dwCkeySrc>>16 & 0x000000ff);
				unsigned char u = (unsigned char)(m_dwCkeySrc>>8 & 0x000000ff);
				unsigned char v = (unsigned char)(m_dwCkeySrc & 0x000000ff);
				memset(pbuf, y ,clearWidth*clearHeight);
				memset(pbuf+clearWidth*clearHeight, u ,clearWidth*clearHeight/4);
				memset(pbuf+clearWidth*clearHeight*5/4, v ,clearWidth*clearHeight/4);


				DWORD strides[3];
				strides[0]= clearWidth; 
				strides[1] = strides[2] = clearWidth/2;
				
				LPBYTE bufStrides[3];
				bufStrides[0] = pbuf; 
				bufStrides[1] = pbuf+clearWidth*clearHeight;
				bufStrides[2] = pbuf+clearWidth*clearHeight*5/4;
				if(dwPropID == MC_PROPID_VP_CLEAR)
					hr = Present(bufStrides, strides,0);
				else
				{
					if(m_overlayType == VP_OPEN_FLAG_DDRAW)
						hr = PrepareBuffer(GFXOVERLAY_SURF_OVERLAY,bufStrides, strides,0);
					else
						hr = PrepareBuffer(0,bufStrides, strides,0);
				}
				
			}	
			else if(m_OpenOptions.dwFourCC == MAKEFOURCC('N','V','1','2'))
			{
				unsigned char y = (unsigned char)(m_dwCkeySrc>>16 & 0x000000ff);
				unsigned char u = (unsigned char)(m_dwCkeySrc>>8 & 0x000000ff);
				unsigned char v = (unsigned char)(m_dwCkeySrc & 0x000000ff);
				memset(pbuf, y ,clearWidth*clearHeight);

				for(int i=0;i<clearWidth*clearHeight/2;i++)
				{
					if(i%2==0)
						pbuf[clearWidth*clearHeight+i] = u;
					else
						pbuf[clearWidth*clearHeight+i] = v;
				}

				DWORD strides[3];
				strides[0]= clearWidth; 
				strides[1] = strides[2] = clearWidth/2;
				
				LPBYTE bufStrides[3];
				bufStrides[0] = pbuf; 
				bufStrides[1] = pbuf+clearWidth*clearHeight;
				bufStrides[2] = pbuf+clearWidth*clearHeight*5/4;
				if(dwPropID == MC_PROPID_VP_CLEAR)
					hr = Present(bufStrides, strides,0);
				else
				{
					if(m_overlayType == VP_OPEN_FLAG_DDRAW)
						hr = PrepareBuffer(GFXOVERLAY_SURF_OVERLAY,bufStrides, strides,0);
					else
						hr = PrepareBuffer(0,bufStrides, strides,0);
				}
			}
			else if(m_OpenOptions.dwFourCC == MAKEFOURCC('R','5','6','5'))
			{
				BYTE rgb = 0x00;
				memset(pbuf, rgb,(DWORD)(clearWidth*clearHeight*bytePerPixcel));

				DWORD strides[3];
				strides[0] = strides[1] = strides[2] = clearWidth*2; 
				LPBYTE bufArray[3];
				bufArray[0] = bufArray[1] = bufArray[2] = pbuf; 
				if(dwPropID == MC_PROPID_VP_CLEAR)
				{
						hr = PrepareBuffer(0,bufArray, strides,0);
						hr = PresentBuffer(0,0);
				}
				else
				{
					if(m_overlayType == VP_OPEN_FLAG_DDRAW)
						hr = PrepareBuffer(GFXOVERLAY_SURF_OVERLAY,bufArray, strides,0);
					else
						hr = PrepareBuffer(0,bufArray, strides,0);
				}
				UpdateOSDBuffer(bufArray,strides);
		
			}

			free(pbuf);
			return hr;
		
		}
	case MC_PROPID_VP_DMCP_ENABLE:
		{
			if(!pPropData)	return E_INVALIDARG;
			m_bDMCPEnable = *(bool*)pPropData;
			return S_OK;
		}
	case MC_PROPID_VP_STREAM_COUNT:
		{
			if(!pPropData)	return E_INVALIDARG;
			m_StreamCount= *(int*)pPropData;
			return S_OK;
		}
	
#ifdef SAMSUNG6410
	case MC_PROPID_VP_SAMSUNG6410_OPENSYS:
		{
			if(m_pOverlay)
				return m_pOverlay->OpenSys();
			else
				return 	S_OK;
		}
	case MC_PROPID_VP_SAMSUNG6410_OPENMEDIA:
		{
			if(m_pOverlay)
				return m_pOverlay->OpenMedia();
			else
				return S_OK;
		}
#endif
	}
	return E_PROP_ID_UNSUPPORTED;
}

STDMETHODIMP CVRPresentOvl::GetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, 
	DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
{
	return S_OK;
}

STDMETHODIMP CVRPresentOvl::SetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, 
	DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
{
	if(!m_pvideoMixer)
		return E_FAIL;

	VM_SURFACE_LAYER_ID	 id;
	ConvertVRType2VMType(type, &id);


	if(dwPropID == MC_PROPID_VP_ALPHA)
	{
		ConvertPrecision(*(LONG *)pPropData, MAX_ALPHA_INTERNAL, MIN_ALPHA_INTERNAL, MAX_ALPHA_VAL,MIN_ALPHA_VAL);
	}

	return m_pvideoMixer->Set(id, guidPropSet, dwPropID, pInstanceData, cbInstanceData, pPropData, cbPropData);
}

HRESULT CVRPresentOvl::SetSrcSize(DWORD dwWidth, DWORD dwHeight)
{
	RECT r;
	HRESULT ret;

	SetRect(&r, 0, 0, dwWidth, dwHeight);
	if(EqualRect(&r,&m_rSrcSize))
		return S_OK;
	m_rSrcSize = r;

	if(m_pVideoWindow)
	{
		ret = m_pVideoWindow->Set(IID_IMcVideoPresenter, VE_COMMAND_SRCRECT,0,0,&r, sizeof(r));
		if(FAILED(ret))
			return ret;
		ret = SetVideoWndDst();
		if(FAILED(ret))
			return ret;
	}
	return S_OK;
}


HRESULT CVRPresentOvl::Restore()
{
	return Restore(TRUE);
}


HRESULT CVRPresentOvl::Restore(BOOL bResDMCP)
{
	if(!m_bOpen)
		return E_FAIL;

	if(m_pOverlay == NULL)
		return S_OK;
	
#ifdef PXA310
	return S_OK;
#endif

	HRESULT res =E_PENDING;

	if(m_OpenOptions.dwWidth ==0 || m_OpenOptions.dwHeight ==0)
	{
		DP("[GFXPRESENTOVL]Restore: Invalid parameter == %d, %d ,%d \n",m_rSrcSize.right, m_rSrcSize.bottom);
		return E_FAIL;
	}

	CProtect proc(&m_csDisplay);
	if(m_pOverlay)
	{
		m_pOverlay->Close();

		#if defined(NO_ROT_SCL)

			if(m_overlayType == VP_OPEN_FLAG_DDRAW)
			{
				DWORD A4_ddrawFourCC[] =
				{
					MAKEFOURCC('R','5','6','5'),
					0
				};
				res = m_pOverlay->Open(m_rDst.right - m_rDst.left, m_rDst.bottom -  m_rDst.top,
					m_OpenOptions.dwBuffers,(const DWORD *)&A4_ddrawFourCC[0],m_pConfig);
			}
			else
				res = m_pOverlay->Open(m_rDst.right - m_rDst.left, m_rDst.bottom -  m_rDst.top ,
				m_OpenOptions.dwBuffers,(const DWORD *)&const_dwFourCC[0],m_pConfig);

		#elif defined(MT3351_RGB)
			DWORD FourCC[] =
			{
				MAKEFOURCC('R','5','6','5'),
				0
			};
		res = m_pOverlay->Open(m_OpenOptions.dwWidth, m_OpenOptions.dwHeight, 
			m_OpenOptions.dwBuffers,(const DWORD *)&FourCC[0],m_pConfig);

		#else
		DWORD FourCC[2]	;
		memset(FourCC, 0 ,sizeof(FourCC));
		FourCC[0] = m_ginfo.dwFourCC;

		res = m_pOverlay->Open(m_OpenOptions.dwWidth, m_OpenOptions.dwHeight, 
			m_OpenOptions.dwBuffers,(const DWORD *)&FourCC[0],m_pConfig);
		#endif
		if(SUCCEEDED(res))
		{
			m_pOverlay->GetSurfaceInfo(&m_ginfo);
			m_dwPresentIndex = 0;
			RestoreColorControl();
			SetSrcSize(m_OpenOptions.dwWidth, m_OpenOptions.dwHeight);
		}
		else
			DP("[GFXPRESENTOVL]Restore() Overlay open failed == %d\n",res);
	}
	m_dwStatus &= ~1;
	
	UpdateDisplayParameters();

	if(bResDMCP)
	{
		if(m_pDMCP)
			m_pDMCP->Restore();
	}

	return res;
}

HRESULT CVRPresentOvl::UpdateDisplayParameters()
{
	HRESULT hr = S_OK;
	if(m_pOverlay == NULL)
		return S_OK;

	m_pOverlay->SetSrcRect(&m_rSrcSize);
	hr = m_pOverlay->SetDstRect(&m_rDst);
	if(hr == E_UNEXPECTED)
	{		//Hardware doesn't support scalling
		#ifdef PXA
		DWORD alignx = 16;
		DWORD width = m_rDst.right - m_rDst.left;
		DWORD wpad = ~(width-1) & (alignx-1);
		width = width + wpad;
		m_rDst.right = m_rDst.left + width;
		#endif

		CProtect proc(&m_csDisplay);
		m_bSoftScale = TRUE;
		m_SoftOpenOptions = m_OpenOptions;
		m_SoftOpenOptions.dwWidth = m_rDst.right - m_rDst.left;
		m_SoftOpenOptions.dwHeight = m_rDst.bottom - m_rDst.top;

		m_pOverlay->Close();
		DWORD dwFourCC[2] = {0,0};
		dwFourCC[0] = m_ginfo.dwFourCC; 
		hr = m_pOverlay->Open(m_SoftOpenOptions.dwWidth, m_SoftOpenOptions.dwHeight,
			m_SoftOpenOptions.dwBuffers,(const DWORD *)&dwFourCC[0],m_pConfig, &m_rDst);

		if(FAILED(hr))
		{
			DP("[GFXPRESENTOVL] SW destination Set failed \n");
			return hr;
		}

		SetRect(&m_rSrcSize, 0, 0, m_SoftOpenOptions.dwWidth , m_SoftOpenOptions.dwHeight);
		m_pOverlay->SetSrcRect(&m_rSrcSize);
		hr = m_pOverlay->SetDstRect(&m_rDst);
		if(FAILED(hr))
		{
			DP("[GFXPRESENTOVL] SW destination Set2 failed \n");
			return hr;
		}
		if(m_bCkeySrc)
			Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_COLORKEYSRC,0,0,&m_dwCkeySrc ,sizeof(m_dwCkeySrc ));


		m_pOverlay->GetSurfaceInfo(&m_ginfo);
		m_dwPresentIndex = 0;

		if(m_pOSDbuf&&m_lRenderEnabled)
		{
			LPBYTE planes[3];
			DWORD strides[3];
			if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','V','1','2'))
			{
				planes[0]= m_pOSDbuf;
				planes[1] = m_pOSDbuf+m_OpenOptions.dwWidth*m_OpenOptions.dwHeight;
				planes[2] = m_pOSDbuf + m_OpenOptions.dwWidth*m_OpenOptions.dwHeight*5/4;
				
				strides[0] = m_OpenOptions.dwWidth;
				strides[1] = strides[2] = m_OpenOptions.dwWidth/2;					
				Present(planes, strides,0);
			}
			else if(m_OpenOptions.dwFourCC == MAKEFOURCC('R','5','6','5'))
			{
				planes[0]= m_pOSDbuf;
				planes[1] = planes[2] =0;
				strides[0] = m_OpenOptions.dwWidth*2;
				strides[1] = strides[2] = 0;
				Present(planes, strides,0);
			}

		}
	}
	else if(hr==S_FALSE)
		m_dwStatus |= 1;
	else
		m_dwStatus &= ~1;
	m_pOverlay->SetScnRect(&m_rScreen);
#if defined(__linux__) && !defined(__QNX__) 
	m_pOverlay->SetWindowHandle((Window)m_OpenOptions.hWnd);
#else
	m_pOverlay->SetWindowHandle((HWND)m_OpenOptions.hWnd);
#endif

	return hr;
}

STDMETHODIMP CVRPresentOvl::ConvertVRType2VMType(VR_SURFACE_TYPE type, VM_SURFACE_LAYER_ID* pId)
{
	if(pId == 0)
		return E_FAIL;

	switch(type)
	{
		case VP_SURF_LAYERTOP:
			*pId = SURFACELAYER_TOP;
			break;
		case VP_SURF_LAYER1:
			*pId = SURFACELAYER_LAYER1;
			break;
		case VP_SURF_LAYER2:
			*pId = SURFACELAYER_LAYER2;
			break;
		case VP_SURF_LAYER3:
			*pId = SURFACELAYER_LAYER3;
			break;
		case VP_SURF_LAYERBOTTOM:
			*pId = SURFACELAYER_BOTTOM;
			break;
		default:
			return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CVRPresentOvl::PrepareBuffer(VR_SURFACE_TYPE type, LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;

	if(!m_pvideoMixer|| !m_bGoMixer)
		return PrepareBuffer(lNum, ppbPlanes, pdwStrides,dwFlags);

	VM_SURFACE_LAYER_ID id;	
	HRESULT res = ConvertVRType2VMType(type,&id);
	if(FAILED(res))
		return res;
	
	return m_pvideoMixer->Prepare(id, ppbPlanes, pdwStrides, dwFlags);
}

STDMETHODIMP CVRPresentOvl::PresentBuffer(VR_SURFACE_TYPE type, LONG lNum, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;

	if(!m_pvideoMixer||!m_bGoMixer)
		return PresentBuffer(lNum, dwFlags);

	VM_SURFACE_LAYER_ID id;	
	HRESULT res = ConvertVRType2VMType(type,&id);
	if(FAILED(res))
		return res;
		
	return m_pvideoMixer->PresentBuffer(id, dwFlags);
}

STDMETHODIMP CVRPresentOvl::Present(VR_SURFACE_TYPE type, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;

	HRESULT hr = S_OK;
	if((!m_pvideoMixer||!m_bGoMixer))
	{
		hr = Present(ppbPlanes, pdwStrides, dwFlags);
		return hr;
	}


	VM_SURFACE_LAYER_ID id;	
	HRESULT res = ConvertVRType2VMType(type,&id);
	if(FAILED(res))
		return res;
	
	res = m_pvideoMixer->Prepare(id, ppbPlanes, pdwStrides, dwFlags);
	if(FAILED(res))
		return res;

	res = m_pvideoMixer->PresentBuffer(id, dwFlags);
	if(FAILED(res))
		return res;

	return S_OK;
}

STDMETHODIMP CVRPresentOvl::Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	HRESULT hr = E_PENDING;
	#ifdef DUMP_FPS
	DWORD presentTick = GetTickCount();
	#endif

    DWORD tick[10];
    tick[0] = GetTickCount();

	if(!m_bOpen)
		return E_FAIL;

	if(!m_lRenderEnabled)	
		return S_FALSE;

	if(m_pOverlay == NULL)
		return S_OK;

	#if defined(__linux__) && !defined(__QNX__) 
	if(dwFlags == eANDROID_UI_FLIP && m_pOverlay)
		return m_pOverlay->UIFlip(0);
	#endif

	#ifdef PXA310
		m_dwPresentIndex = GFXOVERLAY_SURF_OVERLAY;
	#endif

	if((!(PRESENT_FLAG_NO_COPY_DATA & dwFlags))&&((ppbPlanes == 0)||(pdwStrides==0)))
	{
		return E_FAIL;
	}

	#ifdef DUMP_FILE
	{
	        FILE* fp = fopen("1.yuv", "wb");
		for(int i =m_OpenOptions.dwHeight - 2;i<m_OpenOptions.dwHeight;i++)
		{
		    fwrite(ppbPlanes[0] + pdwStrides[0]*i, 1, m_OpenOptions.dwWidth, fp);
		}

		for(int i =(m_OpenOptions.dwHeight>>1) - 1;i<m_OpenOptions.dwHeight>>1;i++)
		{
		    fwrite(ppbPlanes[1] + pdwStrides[1]*i, 1, m_OpenOptions.dwWidth>>1, fp);
		}
		
		for(int i =(m_OpenOptions.dwHeight>>1 )- 1;i<m_OpenOptions.dwHeight>>1;i++)
		{
		    fwrite(ppbPlanes[2] + pdwStrides[2]*i, 1, m_OpenOptions.dwWidth>>1, fp);
		}

		fclose(fp);

	}


	#endif

	
	#ifndef NO_ROT_SCL
	LONG caps = 0;
	if(m_pOverlay && SUCCEEDED(m_pOverlay->GetDDCaps(&caps)))
	{
		if((caps&GFXOVERLAY_CAPS_CROPPING_SUPPORT)&&(pdwStrides[0] > m_OpenOptions.dwWidth))
		{
			DP("Testing GFXOVERLAY_CAPS_CROPPING_SUPPORT--start---------------%d,%d--------\n",m_OpenOptions.dwWidth,pdwStrides[0]  );

			RECT cropRect;
			CProtect proc(&m_csDisplay);

			SetRect(&cropRect, 0 ,0,m_OpenOptions.dwWidth ,m_OpenOptions.dwHeight);
			m_OpenOptions.dwWidth = pdwStrides[0];

			m_pOverlay->Close();
			m_pOverlay->Open(m_OpenOptions.dwWidth, m_OpenOptions.dwHeight, 
				m_OpenOptions.dwBuffers,(const DWORD *)&const_dwFourCC[0],m_pConfig);
			m_rSrcSize = cropRect;
			UpdateDisplayParameters();

		//	Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_RECTSRC,0,0,&cropRect,0);
			DP("Testing GFXOVERLAY_CAPS_CROPPING_SUPPORT--End-----------------------\n");
		}
	}
	#endif

	UpdateDestPos();

#ifdef MX37_NONMEMCPY
	BOOL bFound = FALSE;
	//Change to correct id when hardware decoding
	if(pVideoMemoryInfo)
	{
		for(int i=0;i<m_OpenOptions.dwBuffers;i++)
		{
			if(pVideoMemoryInfo[i].pBuf == ppbPlanes[0])
			{
				LPBYTE tmp = pVideoMemoryInfo[i].pBuf;
				pVideoMemoryInfo[i].pBuf = m_bufferMemoryInfo.pBuf;
				m_bufferMemoryInfo.pBuf = tmp;
				m_dwPresentIndex = i;
				bFound = TRUE;
				break;
			}
		}
	}

	if(!bFound)
#endif
#if !defined(RMI) && !defined(PRIMAII_SAVECOPY)
    if(m_bPresented == false)
   {
      DP("m_overlayType :%p pdwStrides[0]:%d,%d,%d \n",m_overlayType ,pdwStrides[0],pdwStrides[1],pdwStrides[2]);
      DP("ppbPlanes[0]:%d,%d,%d 1-0: %d  2-0:%d \n",ppbPlanes[0],ppbPlanes[1],ppbPlanes[2],ppbPlanes[1] - ppbPlanes[0],
      ppbPlanes[2] - ppbPlanes[0]);
    }

    if((m_overlayType == VP_OPEN_FLAG_ANDROID_ISURFACE_OVERLAY  || m_overlayType == VP_OPEN_FLAG_ANDROID_OPENCORE)
          && ((DWORD)pdwStrides[0] == m_OpenOptions.dwWidth)
          && (((DWORD)(ppbPlanes[1] - ppbPlanes[0]) == m_OpenOptions.dwWidth*m_OpenOptions.dwHeight)
                ||((DWORD)(ppbPlanes[2] - ppbPlanes[0]) == m_OpenOptions.dwWidth*m_OpenOptions.dwHeight))
          &&(m_OpenOptions.dwWidth*m_OpenOptions.dwHeight > 2*1024*1024/1.5/2/*backbuffer*/)
                )
    {
      if(m_bPresented == false)
          DP("bypass mode \n");

	if(m_pOverlay)
	{
		m_pOverlay->SetVRHandle(ppbPlanes[0]);
		hr = m_pOverlay->Flip(0,GFXOVERLAY_FLIP_FIXED);
	}
      return hr;
    }
  else
  {
#ifdef GFX_ANDROID_ISURFACE_OVERLAY
      if(m_OpenOptions.dwWidth*m_OpenOptions.dwHeight > 6*1024*1024/1.5)
      {
        DP("Unsupport!!! too big resolution \n ");
        return E_FAIL;
      }
#endif
      if(m_bPresented == false)
          DP("copy mode \n");
  }

    tick[1] = GetTickCount();

	m_bPresented  = true;
	if(!(PRESENT_FLAG_NO_COPY_DATA & dwFlags))
	{
		hr = PrepareBuffer(m_dwPresentIndex,ppbPlanes,pdwStrides,dwFlags);
		if(FAILED(hr))
		{
		//	DP("[GFXPRESENTOVL]Present: preparebuf failed :%d \n",m_lRenderEnabled );
			return hr;
		}
		UpdateOSDBuffer(ppbPlanes,pdwStrides);
	}
#endif


	#if defined(ST_SAVECOPY)

	STSGA_MEM_S* pStBuff[3] = {0};

	pStBuff[0] = (STSGA_MEM_S*)ppbPlanes[0];
	pStBuff[1] = (STSGA_MEM_S*)ppbPlanes[1];
	pStBuff[2] = (STSGA_MEM_S*)ppbPlanes[2];

	if(pStBuff[0] && pStBuff[1] && pStBuff[2] && pStBuff[0]->m_hdc)
	{
		#ifdef ST_SAVECOPY
		STSetFlipAddress(pStBuff[0]->m_hdc, (DWORD)pStBuff[0]->pPhyAdr,
			(DWORD)((LONG)pStBuff[2]->pPhyAdr - (LONG)pStBuff[0]->pPhyAdr),
			(DWORD)((LONG)pStBuff[1]->pPhyAdr - (LONG)pStBuff[0]->pPhyAdr),pdwStrides[0],
			pdwStrides[1],pdwStrides[2]);
		#endif

	}
	else
		DP("Invalid st handle \n");

		#elif defined(RENESAS)
		if(m_overlayType == VP_OPEN_FLAG_RENESASHW)
		{
        GfxRENESAS_HND hnd;
        hnd.pYBuff = ppbPlanes[0];
        hnd.pUVBuff = ppbPlanes[1];
		if(m_pOverlay)
			return (m_pOverlay->SetHWRenderSurf(&hnd));
		else
			return S_OK;
		}

	#elif defined(PRIMAII_SAVECOPY)
		if(m_pOverlay)
		{
			HRESULT hr = m_pOverlay->SetHWRenderSurf(ppbPlanes[2]);
			if(SUCCEEDED(hr))
			{
				UpdateDisplayParameters();
			}
			return hr;
		}
		return S_OK;
	#endif

    
#ifdef ST_LINUX
    if(m_overlayType == VP_OPEN_FLAG_ST_EXTERNALBUFFER)
    {
      	STSGA_MEM_S* pStBuff[3] = {0};

      	pStBuff[0] = (STSGA_MEM_S*)ppbPlanes[0];
      	pStBuff[1] = (STSGA_MEM_S*)ppbPlanes[1];
      	pStBuff[2] = (STSGA_MEM_S*)ppbPlanes[2];

      	if(pStBuff[0] && pStBuff[1] && pStBuff[2] && pStBuff[0]->m_hdc)
      	{
      			GfxST_HND hnd;
      			hnd.hdc = pStBuff[0]->m_hdc;
      			hnd.yBuff = (DWORD)(pStBuff[0]->pPhyAdr);
      			hnd.uOffset = (DWORD)((LONG)pStBuff[1]->pVirAdr - (LONG)pStBuff[0]->pVirAdr);
      			hnd.vOffset = (DWORD)((LONG)pStBuff[2]->pVirAdr - (LONG)pStBuff[0]->pVirAdr);
      			hnd.yStride = pdwStrides[0];
    			hnd.uStride = pdwStrides[1];
    			hnd.vStride = pdwStrides[2];
			hnd.m_bitmap = pStBuff[0]->m_bitmap;
			if(m_pOverlay)
				m_pOverlay->SetHWRenderSurf(&hnd);
      	}
      	else
      		DP("Invalid st handle \n");
if(0)
{
	        FILE* fp = fopen("/mnt/dump1.yuv", "a");
		for(int i =0;i<m_OpenOptions.dwHeight;i++)
		{
		    fwrite(pStBuff[0]->pVirAdr + pdwStrides[0]*i, 1, m_OpenOptions.dwWidth, fp);
		}

		for(int i =0;i<m_OpenOptions.dwHeight>>1;i++)
		{
		    fwrite(pStBuff[1]->pVirAdr+ pdwStrides[1]*i, 1, m_OpenOptions.dwWidth>>1, fp);
		}
		
		for(int i =0;i<m_OpenOptions.dwHeight>>1;i++)
		{
		    fwrite(pStBuff[2]->pVirAdr + pdwStrides[2]*i, 1, m_OpenOptions.dwWidth>>1, fp);
		}

		fclose(fp);

	}
    }
#endif

    tick[2] = GetTickCount();

	hr = PresentBuffer(m_dwPresentIndex,dwFlags);
	if(++m_dwPresentIndex>=m_ginfo.dwBackBuffers)
		m_dwPresentIndex = 0;

    tick[3] = GetTickCount();

	if(FAILED(hr))
	{
		DP("[GFXPRESENTOVL]Present: PresentBuffer failed ==%d\n",hr);
	}

	if(GetTickCount() -m_lastpresent_tick > 1000)
	{
		if(m_pDMCP)
		{
			if(m_OpenOptions.hWnd)
			{
				#ifndef ANDROID
				#ifndef __linux__
				static RECT rect_bak = {0,0,0,0};
				RECT dst_rect = {0,0,0,0}; 

				CProtect proc(&m_csDisplay);
				::GetClientRect((HWND)m_OpenOptions.hWnd, &dst_rect );

				POINT p={0,0};
				::ClientToScreen((HWND)m_OpenOptions.hWnd, &p);
				OffsetRect(&dst_rect ,p.x,p.y);

				if(memcmp(&dst_rect, &rect_bak, sizeof(rect_bak))!=0)
				{
					HRESULT hr = S_OK;
					hr = m_pDMCP->SetTargetRect(&dst_rect);
					if(FAILED(hr))
						DP("[GFXPRESENT] SetTargetRect failed :0x%x \n",hr);

					rect_bak = dst_rect; 
				}
				#endif
				#endif
			}

			m_DMCP_dsp_enable = (m_pDMCP->CanDisplay(FALSE)!=0);
			if(m_DMCP_dsp_enable == FALSE)
			{
				if(m_bDMCPEnable == false)
					m_DMCP_dsp_enable = true;	
				else
					DP("DMCP->CanDisplay() = FALSE \n");
			}

		}


		m_lastpresent_tick = GetTickCount();
	}

	if(!m_DMCP_dsp_enable)
		return E_ACCESSDENIED;

	#ifdef DUMP_FPS
	if(g_frames == 0)
		g_startTick = GetTickCount();

	g_frames++;
	if(g_frames%50 == 0)
	DP("FPS:%d Present() :%d msec \n",g_frames*1000/(GetTickCount() -g_startTick ),
		GetTickCount() - presentTick);
	#endif

    tick[9] = GetTickCount();
#ifdef DUMP_FPS
    if(tick[9]-tick[0] > 30)
    {
      DP("Present() too slow:%d msec \n",tick[9] - tick[0]);
      DP("    prepare cost:%d msec \n",tick[1] - tick[0]);
      DP("    prepareBuff cost:%d msec \n",tick[2] - tick[1]);
      DP("    presentBuff cost:%d msec \n",tick[3] - tick[2]);
    }
#endif

#ifdef FREESCALE_IMX51
	static DWORD lastPresentTime = GetTickCount();
	if(GetTickCount() > lastPresentTime +1000)
	{
		if(m_pOverlay)
			m_pOverlay->SetDstRect(0);		
		lastPresentTime = GetTickCount();
	}
#endif

	return hr;
}

HRESULT CVRPresentOvl::PresentRect(LPBYTE *ppbPlanes, DWORD *pdwStrides,DWORD dwFlags ,RECT* pSrcRgn, RECT* pDstRgn) 
{
	HRESULT hr;
	if(!m_bOpen)
		return E_FAIL;

	if((ppbPlanes == 0)||(pdwStrides==0) || (pSrcRgn== 0) || (pDstRgn == 0))
		return E_FAIL;

	LPBYTE pBuf;
	LONG lStride;

	if(ppbPlanes[0]==0 || ppbPlanes[1]==0)
		return E_FAIL;

	if(m_pOverlay == NULL)
		return S_OK;

	if(FAILED(m_pOverlay->Lock(m_dwPresentIndex,(LPVOID *)&pBuf,&lStride,0)))
	{
		return E_FAIL;
	}

	if(m_OpenOptions.dwFourCC == MAKEFOURCC('X','R','G','B'))
	{
		switch(m_ginfo.dwFourCC)
		{
		case MAKEFOURCC('X','R','G','B'):
			{
				unsigned int uSrcWidth = pSrcRgn->right  - pSrcRgn->left;
				pSrcRgn->top = m_rSrcSize.bottom - pSrcRgn->top;
				pSrcRgn->bottom = m_rSrcSize.bottom - pSrcRgn->bottom;
				for (int y = pSrcRgn->top; y > pSrcRgn->bottom; y --)
				{
					unsigned char *pDstRGBA = pBuf + ((lStride * (y - 1) + pSrcRgn->left) << 2);
					unsigned char *pSrcRGBA = ppbPlanes[0] + ((lStride * (y - 1) + pSrcRgn->left) << 2);//((uSrcWidth * (pSrcRgn->top - y)) << 2);
					memcpy(pDstRGBA, pSrcRGBA, uSrcWidth << 2);
				}
			}
			break;
		default:
			break;
		}

		hr = PresentBuffer(m_dwPresentIndex,dwFlags);
		if(++m_dwPresentIndex>=m_ginfo.dwBackBuffers)
			m_dwPresentIndex = 0;

		return hr;
	}
	return E_FAIL;
}

HRESULT CVRPresentOvl::ConvertPrecision(LONG& lSrcVal, const LONG lDstMax, const LONG lDstMin, const LONG lSrcMax,const LONG lSrcMin)
{
	LONG lSrcNum = lSrcMax - lSrcMin;
	LONG lDstNum = lDstMax - lDstMin;

	if(lSrcVal>lSrcMax)
		lSrcVal = lDstMax;
	else if(lSrcVal < lSrcMin)
		lSrcVal = lDstMin;
	else		
		lSrcVal = lDstMin + (lSrcVal-lSrcMin)*lDstNum/lSrcNum;
	return S_OK;
}

HRESULT CVRPresentOvl::UpdateOSDBuffer(LPBYTE *ppbPlanes, DWORD *pdwStrides)
{
	if(m_overlayType == VP_OPEN_FLAG_DDRAW_OSD)
	{
		if(m_pOSDbuf!=ppbPlanes[0])
		{
			if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','V','1','2'))
			{
				if(((DWORD)pdwStrides[0] == m_OpenOptions.dwWidth) && 
					(((DWORD)(ppbPlanes[1] - ppbPlanes[0])) == m_OpenOptions.dwWidth*m_OpenOptions.dwHeight) &&
					(((DWORD)(ppbPlanes[2] - ppbPlanes[0])) == m_OpenOptions.dwWidth*m_OpenOptions.dwHeight*5/4))
						memcpy(m_pOSDbuf, ppbPlanes[0], m_OpenOptions.dwWidth*m_OpenOptions.dwHeight*3/2);
				else if(pdwStrides[0] == m_OpenOptions.dwWidth)
				{
					memcpy(m_pOSDbuf, ppbPlanes[0], m_OpenOptions.dwWidth*m_OpenOptions.dwHeight);
					memcpy(m_pOSDbuf + m_OpenOptions.dwWidth*m_OpenOptions.dwHeight,
						ppbPlanes[1], m_OpenOptions.dwWidth*m_OpenOptions.dwHeight/4);
					memcpy(m_pOSDbuf + m_OpenOptions.dwWidth*m_OpenOptions.dwHeight*5/4,
						ppbPlanes[2], m_OpenOptions.dwWidth*m_OpenOptions.dwHeight/4);

				}
			}
			else if(m_OpenOptions.dwFourCC == MAKEFOURCC('R','5','6','5'))
			{
				if(pdwStrides[0] == m_OpenOptions.dwWidth*2)
					memcpy(m_pOSDbuf, ppbPlanes[0], m_OpenOptions.dwWidth*m_OpenOptions.dwHeight*2);
				else
				{
					for(int i=0;i<m_OpenOptions.dwHeight;i++)
						memcpy(m_pOSDbuf + m_OpenOptions.dwWidth*2*i, 
							ppbPlanes[0] + pdwStrides[0]*i, m_OpenOptions.dwWidth*2);
				}
			}

		}

	}
	return S_OK;
}

#ifdef USE_IPP_ROTATION
STDMETHODIMP CVRPresentOvl::PrepareBufferIPP_YV12(LPBYTE frameBuffer, 	LONG lStride , LPBYTE *ppbPlanes,  DWORD *pdwStrides, DWORD* pdwBufStrides)
{

	IppCameraRotation rotation;
	IppCameraInterpolation interpolation;
	int rcpRatioX, rcpRatioY;
	const Ipp8u* pS[3];
	Ipp8u* pD[3];
	IppiSize srcSize, dstSize;
	RECT* pSrcSize =	&m_rSrcSize;
	pD[0] = frameBuffer;

#if  1//(_WIN32_WCE != 0x600)
	pD[2] = frameBuffer + pdwBufStrides[0];
	pD[1] = frameBuffer + pdwBufStrides[0]+ pdwBufStrides[1];
#else
	pD[1] = frameBuffer + pdwBufStrides[0];
	pD[2] = frameBuffer + pdwBufStrides[0]+ pdwBufStrides[1];
#endif	

	srcSize.width = WidthRect(pSrcSize);
	srcSize.height = HeightRect(pSrcSize);

	interpolation = ippCameraInterpBilinear;
	
	int srcStride[3]; 
	srcStride[0] = (int)pdwStrides[0];
	srcStride[1] = (int)pdwStrides[1];
	srcStride[2] = (int)pdwStrides[2];

	int dstStride[3]; 
	dstStride[0] = (int)lStride;
	dstStride[1] = (int)lStride>>1;
	dstStride[2] = (int)lStride>>1;

	
	pS[0] = ppbPlanes[0];
	pS[1] = ppbPlanes[1];
	pS[2] = ppbPlanes[2];

	if(m_lRotationMode==VP_ROTATION_0)
	{	// PORTRAIT
		rotation = ippCameraRotateDisable;
		dstSize.width = WidthRect(pSrcSize);
		dstSize.height = HeightRect(pSrcSize);
		rcpRatioX =	(int)(((double)((WidthRect(pSrcSize)-1)<<16))/(WidthRect(pSrcSize)-1));
		rcpRatioY =	(int)(((double)((HeightRect(pSrcSize)-1)<<16))/(HeightRect(pSrcSize)-1));
	}
	else if (m_lRotationMode==VP_ROTATION_90)
	{	// LANDSCAPE
		rotation = ippCameraRotate90L;
		dstSize.height = WidthRect(pSrcSize);
		dstSize.width = HeightRect(pSrcSize);
		rcpRatioX =	(int)(((double)((WidthRect(pSrcSize)-1)<<16))/(HeightRect(pSrcSize)-1));
		rcpRatioY =	(int)(((double)((HeightRect(pSrcSize)-1)<<16))/(WidthRect(pSrcSize)-1));
	}
	else if (m_lRotationMode==VP_ROTATION_180)
	{	// UPSIDE DOWN
		rotation = ippCameraRotate180;
		dstSize.width = WidthRect(pSrcSize);
		dstSize.height = HeightRect(pSrcSize);
		rcpRatioX =	(int)(((double)((WidthRect(pSrcSize)-1)<<16))/(WidthRect(pSrcSize)-1));
		rcpRatioY =	(int)(((double)((HeightRect(pSrcSize)-1)<<16))/(HeightRect(pSrcSize)-1));
	}
	else if (m_lRotationMode==VP_ROTATION_270)
	{	// LANDSCAPE, FLIP
		rotation = ippCameraRotate90R;
		dstSize.height = WidthRect(pSrcSize);
		dstSize.width = HeightRect(pSrcSize);
		rcpRatioX =	(int)(((double)((WidthRect(pSrcSize)-1)<<16))/(HeightRect(pSrcSize)-1));
		rcpRatioY =	(int)(((double)((HeightRect(pSrcSize)-1)<<16))/(WidthRect(pSrcSize)-1));
	}
	//DP("[VideoRender] Before IPP resize&rotate : srcWidth = %d, srcHeight = %d, srcStep = %d, dstWidth = %d, dstHeight = %d, dstStep = %d.\n", srcSize.width, srcSize.height, srcStep[0], dstSize.width, dstSize.height, dstStep[0]);
	ippiYCbCr420RszRot_8u_P3R(pS, srcStride, srcSize, pD,
			dstStride, dstSize, interpolation, rotation,
			rcpRatioX, rcpRatioY);
	return S_OK;
}
#endif

STDMETHODIMP CVRPresentOvl::PrepareBuffer(LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	LPBYTE pBuf;
	LONG lStride;
	HRESULT hr = E_FAIL;
	RECT rScaleSrc, rScaleDst;
	DWORD dwScaleFlags = 0;

	if(!m_bOpen)
		return E_FAIL;

	if(m_pOverlay == NULL) 
		return S_OK;

	if(m_overlayType == VP_OPEN_FLAG_ST_EXTERNALBUFFER)
		return S_OK;
    
#if defined(ST_SAVECOPY)
	return S_OK;
#endif
	
#ifdef TITAN_II
    if(surfaceFourCC == MAKEFOURCC('P','B','S','1'))    //TITIAN II HW render mode
	{
		if(m_pOverlay)
			return(m_pOverlay->SetHWRenderSurf(ppbPlanes[0]));
		else
			return S_OK;
	}
#endif

#if defined(GFX_ANDROID_ISURFACE_OVERLAY)
LPBYTE tmpPlane = ppbPlanes[1];
ppbPlanes[1] = ppbPlanes[2];
ppbPlanes[2] = tmpPlane;
#endif
    if(ppbPlanes[0]==0)
	{
		DP("[GFXPRESENTOVL]PresentBuffer: Invalid parameter == %d, %d ,%d \n",ppbPlanes[0], ppbPlanes[1], ppbPlanes[2]);
		return E_FAIL;
	}

	ZeroMemory(&rScaleDst, sizeof(rScaleDst));

	//if(surfaceFourCC != DEFAULT_INPUT_FOURCC)
	//	return E_FAIL;
	#ifdef DMCP_TEST
	if(!m_DMCP_dsp_enable)
	{
		if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','V','1','2'))
		{
			LPBYTE srcY = 0;
			LPBYTE srcU = 0;
			LPBYTE srcV = 0;
			for(DWORD i=0;i<m_OpenOptions.dwHeight;i++)
			{
				srcY = ppbPlanes[0] + pdwStrides[0]*i;
				memset(srcY, 0 ,m_OpenOptions.dwWidth);
				srcU = ppbPlanes[1] + pdwStrides[1]*i;
				memset(srcU, 0 ,m_OpenOptions.dwWidth>>1);
				srcV = ppbPlanes[2] + pdwStrides[2]*i;
				memset(srcV, 0 ,m_OpenOptions.dwWidth>>1);
			}
		}
		else if(m_OpenOptions.dwFourCC == MAKEFOURCC('X','R','G','B'))
		{
			memset(ppbPlanes[0],m_OpenOptions.dwWidth*m_OpenOptions.dwHeight*4 );
		}

	}
	#endif

	CProtect proc(&m_csDisplay);

	if(FAILED(m_pOverlay->Lock(lNum,(LPVOID *)&pBuf,&lStride,0)))
	{
		DP("[GFXPRESENTOVL]PresentBuffer:Lock failed \n");
		
		hr = Restore(false);
		if(hr == E_ACCESSDENIED)
		{//HARDWARE ACCELERATION DISABLED

			//if(m_OpenOptions.dwSurfType& VP_OPEN_FLAG_DDRAW) 
			//	m_OpenOptions.dwSurfType = (~VP_OPEN_FLAG_DDRAW)&m_OpenOptions.dwSurfType;
			//if(m_OpenOptions.dwSurfType& VP_OPEN_FLAG_D3D) 
			//	m_OpenOptions.dwSurfType = (~VP_OPEN_FLAG_D3D)&m_OpenOptions.dwSurfType;

			if(m_OpenOptions.dwSurfType != VP_OPEN_FLAG_AUTO)
				m_OpenOptions.dwSurfType |= VP_OPEN_FLAG_GDI;

			RECT dstBak = rVideoWnd;
			RECT srcBak = m_rSrcSize;
			LONG vpEnable = m_lRenderEnabled;

			hr = Close();
			if(SUCCEEDED(hr))
			{
				hr = Open(&m_OpenOptions,  sizeof(m_OpenOptions));
				if(SUCCEEDED(hr))
				{
					Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_RECTDST,0,0,&dstBak,0);
					Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_RECTSRC,0,0,&srcBak,0);
					Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_ENABLE,0,0,&vpEnable,0);
				}

			}
			return hr;
		}
		return E_FAIL;
	}
/*

	DP("lock[%d]pbuf:%p, stride:%d \n", lNum,pBuf,lStride);
	DP("size:%d,%d \n", m_OpenOptions.dwWidth , m_OpenOptions.dwHeight);

	memset(pBuf, 0 , m_OpenOptions.dwWidth * m_OpenOptions.dwHeight*3/2);
	DP("memset success \n");
*/	
	if(m_bSoftScale)
	{
		ZeroMemory(&rScaleSrc, sizeof(rScaleSrc));
		ZeroMemory(&rScaleDst, sizeof(rScaleDst));
		rScaleSrc.right = m_OpenOptions.dwWidth;
		rScaleSrc.bottom = m_OpenOptions.dwHeight;
		rScaleDst.right = m_SoftOpenOptions.dwWidth;
		rScaleDst.bottom = m_SoftOpenOptions.dwHeight;
		dwScaleFlags = m_bSoftScale;		
	}
	else
	{
		if(FAILED(m_pOverlay->GetSurfScaleRects(lNum, &rScaleSrc, &rScaleDst, &dwScaleFlags)))
			dwScaleFlags = 0;
	}




	DWORD srcWidth[3];
	DWORD srcHeight[3];
	memset(srcWidth, 0 ,sizeof(srcWidth));
	memset(srcHeight, 0 ,sizeof(srcHeight));
	srcWidth[0] = m_OpenOptions.dwWidth;
	srcWidth[1] = srcWidth[2] = (m_OpenOptions.dwWidth>>1);
	srcHeight[0] = m_OpenOptions.dwHeight;
	srcHeight[1] = srcHeight[2] = (m_OpenOptions.dwHeight>>1);
	

	if(m_unsupportSizeFlag)
	{
		dwScaleFlags = GFXOVERLAY_SCALE_X | GFXOVERLAY_SCALE_Y;
		SetRect(&rScaleSrc, 0,0, m_unsupportOptions.dwWidth,m_unsupportOptions.dwHeight);
		SetRect(&rScaleDst, 0,0, m_OpenOptions.dwWidth,m_OpenOptions.dwHeight);
		srcWidth[0] = m_unsupportOptions.dwWidth;
		srcWidth[1] = srcWidth[2] = (m_unsupportOptions.dwWidth>>1);
		srcHeight[0] = m_unsupportOptions.dwHeight;
		srcHeight[1] = srcHeight[2] = (m_unsupportOptions.dwHeight>>1);
	}

	if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','V','1','2'))
	{
		switch(m_ginfo.dwFourCC)
		{
		case MAKEFOURCC('Y','U','Y','2'):
		case MAKEFOURCC('Y','U','Y','V'):
			hr = m_PrepareData.PrepareBufferYUY2(m_OpenOptions.dwFourCC,pBuf, lStride, 
				ppbPlanes, pdwStrides, srcWidth, srcHeight, dwFlags, m_lRotationMode,m_lFlipVideoMode);
			if(FAILED(hr))
				DP("!!!!!!!!Preapre failed err:0x%x \n",hr);
			break;
		case MAKEFOURCC('U','Y','V','Y'):
			hr = m_PrepareData.PrepareBufferUYVY(m_OpenOptions.dwFourCC,pBuf, lStride, 
				ppbPlanes, pdwStrides, srcWidth, srcHeight, dwFlags, m_lRotationMode,m_lFlipVideoMode);
			if(FAILED(hr))
				DP("!!!!!!!!Preapre failed err:0x%x \n",hr);
			break;
		case MAKEFOURCC('Y','V','1','2'):
			{
				DWORD pdwBufStridesOrdered[3];
				pdwBufStridesOrdered[0] = lStride*srcHeight[0]; 
				pdwBufStridesOrdered[1] = (lStride>>1)*srcHeight[1]; 
				pdwBufStridesOrdered[2] = (lStride>>1)*srcHeight[2]; 
				#ifdef PXA310
				pdwBufStridesOrdered[0] = m_OpenOptions.dwWidth*m_OpenOptions.dwHeight; 
				pdwBufStridesOrdered[1] = (m_OpenOptions.dwWidth*m_OpenOptions.dwHeight)>>2; 
				pdwBufStridesOrdered[2] = (m_OpenOptions.dwWidth*m_OpenOptions.dwHeight)>>2;
				#endif
				
				//Swap U,V planes.
				if((m_overlayType == VP_OPEN_FLAG_SAMSUNG)||(m_overlayType == VP_OPEN_FLAG_ST)
				    ||(m_overlayType == VP_OPEN_FLAG_ANDROID_OPENCORE)||(m_overlayType == VP_OPEN_FLAG_MSTAR)
					|| (m_overlayType == VP_OPEN_FLAG_SAMSUNG6410) || (m_overlayType == VP_OPEN_FLAG_ANDROID))
				{
					LPBYTE buffTmp = ppbPlanes[1];
					ppbPlanes[1] = ppbPlanes[2];
					ppbPlanes[2] = buffTmp;
				}

			#ifdef USE_IPP_ROTATION
				hr = PrepareBufferIPP_YV12(pBuf, lStride, ppbPlanes,pdwStrides, pdwBufStridesOrdered);
			#else

				hr = m_PrepareData.PrepareBufferYV12(pBuf, lStride, ppbPlanes, 
					pdwStrides, srcWidth, srcHeight, dwFlags, m_lRotationMode,
					dwScaleFlags,&rScaleDst,m_Deinterlaced_Mode,m_lFlipVideoMode);
			#endif
			if(FAILED(hr))
				DP("!!!!!!!!Preapre failed err:0x%x \n",hr);

			}
			break;
		case MAKEFOURCC('Y','U','1','2'):
			{
				LPBYTE ppbPlanesOrdered[3];
				DWORD pdwStridesOrdered[3];
				DWORD pdwWidthOrdered[3];
				DWORD pdwHeightOrdered[3];
				DWORD pdwBufStridesOrdered[3];

				ppbPlanesOrdered[0] = ppbPlanes[0];
				ppbPlanesOrdered[2] = ppbPlanes[1];
				ppbPlanesOrdered[1] = ppbPlanes[2];
				pdwStridesOrdered[0] = pdwStrides[0];
				pdwStridesOrdered[2] = pdwStrides[1];
				pdwStridesOrdered[1] = pdwStrides[2];
				pdwWidthOrdered[0] = srcWidth[0];
				pdwWidthOrdered[2] = srcWidth[1];
				pdwWidthOrdered[1] = srcWidth[2];
				pdwHeightOrdered[0] = srcHeight[0];
				pdwHeightOrdered[2] = srcHeight[1];
				pdwHeightOrdered[1] = srcHeight[2];
				pdwBufStridesOrdered[0] = lStride*pdwHeightOrdered[0]; 
				pdwBufStridesOrdered[1] = (lStride>>1)*pdwHeightOrdered[1]; 
				pdwBufStridesOrdered[2] = (lStride>>1)*pdwHeightOrdered[2]; 
				hr =m_PrepareData.PrepareBufferYV12(pBuf, lStride, ppbPlanesOrdered, 
					pdwStridesOrdered, pdwWidthOrdered, pdwHeightOrdered, dwFlags, 
					m_lRotationMode, dwScaleFlags,&rScaleDst,m_Deinterlaced_Mode,m_lFlipVideoMode);
				if(FAILED(hr))
					DP("!!!!!!!!Preapre failed err:0x%x \n",hr);

			}
			break;
		case MAKEFOURCC('N','V','1','2'):
			{
#if  !defined(RENESAS) && !defined(MTK3353_SW)
				LPBYTE buffTmp = ppbPlanes[1];
				ppbPlanes[1] = ppbPlanes[2];
				ppbPlanes[2] = buffTmp;
#endif
				hr = m_PrepareData.PrepareBufferYV12ToNV12(pBuf, lStride, ppbPlanes, 
					pdwStrides, srcWidth, srcHeight, dwFlags, m_lRotationMode,m_lFlipVideoMode);
				if(FAILED(hr))
					DP("!!!!!!!!Preapre failed err:0x%x \n",hr);

			}
			break;
		case MAKEFOURCC('Y','U','2','4'):
			hr = m_PrepareData.PrepareBufferYU24(pBuf, lStride, ppbPlanes, pdwStrides, 
				srcWidth, srcHeight, dwFlags,m_lFlipVideoMode);
			if(FAILED(hr))
				DP("!!!!!!!!Preapre failed err:0x%x \n",hr);
			break;
		case MAKEFOURCC('P','5','6','5'):
		case MAKEFOURCC('R','5','6','5'):
#ifdef ANDROID_IPP
			if(!dwScaleFlags)
			{
				SetRect(&rScaleSrc, 0,0,srcWidth[0],srcHeight[0]);
				SetRect(&rScaleDst, 0,0,srcWidth[0],srcHeight[0]);
			}
			hr = m_PrepareData.PrepareBufferRGB565Scaled_IPP(pBuf, lStride, ppbPlanes, 
					pdwStrides, &rScaleSrc, &rScaleDst, dwFlags, m_lRotationMode,m_lFlipVideoMode);
			if(FAILED(hr))
				DP("!!!!!!!!Preapre failed err:0x%x \n",hr);
#else
			if(dwScaleFlags)
			{
				hr = m_PrepareData.PrepareBufferRGB565Scaled(pBuf, lStride,
					ppbPlanes, pdwStrides, &rScaleSrc, &rScaleDst, dwFlags, 
					m_lRotationMode,m_Deinterlaced_Mode,m_lFlipVideoMode);
				if(FAILED(hr))
					DP("!!!!!!!!Preapre failed err:0x%x \n",hr);
				if(m_pOverlay)
					m_pOverlay->SetSurfScale(lNum,&rScaleDst,dwScaleFlags);
			}
			else
			{

				/*
				if((m_Deinterlaced_Mode != VP_DEINTERLACE_WEAVE) && (m_overlayType == VP_OPEN_FLAG_D3D))
				{
					DWORD deinterSrcStride[3] = {pdwStrides[0] *2,pdwStrides[1] *2,pdwStrides[2] *2};
					srcHeight[0] /=2;
					srcHeight[1] /=2;
					srcHeight[2] /=2;
					hr = m_PrepareData.PrepareBufferRGB565(pBuf,lStride, ppbPlanes, deinterSrcStride, srcWidth, srcHeight, dwFlags, m_lRotationMode);

					ppbPlanes[0] += pdwStrides[0]*2;
					pBuf += lStride/2;
					hr |= m_PrepareData.PrepareBufferRGB565(pBuf,lStride, ppbPlanes, deinterSrcStride, srcWidth, srcHeight, dwFlags, m_lRotationMode);
				}
				else
				*/
				{
					hr = m_PrepareData.PrepareBufferRGB565(pBuf,lStride, ppbPlanes, 
						pdwStrides, srcWidth, srcHeight, dwFlags, m_lRotationMode,
						m_Deinterlaced_Mode,m_lFlipVideoMode);
				}
				if(FAILED(hr))
					DP("!!!!!!!!Preapre failed err:0x%x \n",hr);
			}
#endif
	        break;
	    case MAKEFOURCC('P','B','S','1'):
	        // The Priam back surface mode, just copy the handle of back surfcae
	        memcpy(pBuf, ppbPlanes, sizeof(void*));
	        break;
		case  MAKEFOURCC('X','R','G','B'):
			{
                RECT rBuf;
				SetRect(&rBuf, 0,0,srcWidth[0],srcHeight[0]);
				hr = m_PrepareData.PrepareBufferYV12ToARGB(pBuf, lStride, ppbPlanes, 
				pdwStrides, &rBuf, &rBuf, dwFlags, m_lRotationMode,m_lFlipVideoMode);
//				if(surfaceFourCC != MAKEFOURCC('R','5','6','5'))
//				{
//					DP("PrepareBuffer Invalid input forcc format :0x%x\n", surfaceFourCC);
//					break;
//				}
//
//				hr = m_PrepareData.PrepareBufferXRGB(pBuf,lStride, ppbPlanes, pdwStrides, srcWidth, srcHeight, dwFlags, m_lRotationMode);
//				if(FAILED(hr))
//					DP("!!!!!!!!Preapre failed err:0x%x \n",hr);

				break;
			}
		case MAKEFOURCC('R','8','8','8'):
			if(m_overlayType == VP_OPEN_FLAG_EGL)
				{
					LPBYTE buffTmp = ppbPlanes[1];
					ppbPlanes[1] = ppbPlanes[2];
					ppbPlanes[2] = buffTmp;
				}

			if(dwScaleFlags)
				hr = m_PrepareData.PrepareBufferRGB888(pBuf, lStride, ppbPlanes, 
				pdwStrides, &rScaleSrc, &rScaleDst, dwFlags, m_lRotationMode,m_lFlipVideoMode);
			else
			{
				RECT rBuf;
				SetRect(&rBuf, 0,0,srcWidth[0],srcHeight[0]);
				hr = m_PrepareData.PrepareBufferRGB888(pBuf, lStride, ppbPlanes, 
				pdwStrides, &rBuf, &rBuf, dwFlags, m_lRotationMode,m_lFlipVideoMode);
			
			}

			if(FAILED(hr))
				DP("!!!!!!!!Preapre failed err:0x%x \n",hr);
	        	break;
		case MAKEFOURCC('M','T','K','1'):
			memcpy(pBuf, ppbPlanes[0], pdwStrides[0]);
			break;
		//case MAKEFOURCC('U','Y','V','Y'):
		//		hr = m_PrepareData.PrepareBufferYV12ToUYVY(pBuf, lStride, ppbPlanes, pdwStrides, &rScaleSrc, &rScaleDst, dwFlags, m_lRotationMode);
		//	break;
		default:
			DP("Unknow format input:%p output:%p \n",m_OpenOptions.dwFourCC, m_ginfo.dwFourCC);
			break;
		}
	}
	else if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','U','Y','2'))
	{
		switch(m_ginfo.dwFourCC)
		{
		case MAKEFOURCC('Y','U','Y','2'):
		case MAKEFOURCC('Y','U','Y','V'):
			hr = m_PrepareData.PrepareBufferYUY2(m_OpenOptions.dwFourCC,pBuf, lStride, 
			ppbPlanes, pdwStrides, srcWidth, srcHeight, dwFlags,m_lRotationMode,m_lFlipVideoMode);
			if(FAILED(hr))
				DP("!!!!!!!!Preapre failed err:0x%x \n",hr);
			break;
		default:
			DP("Unknow format input:%p output:%p \n",m_OpenOptions.dwFourCC, m_ginfo.dwFourCC);
			break;
		}
	}
	else if(m_OpenOptions.dwFourCC == MAKEFOURCC('X','R','G','B'))
	{
	  switch(m_ginfo.dwFourCC)
	  {
        case MAKEFOURCC('X','R','G','B'):
      		memcpy(pBuf,ppbPlanes[0],srcWidth[0]*srcHeight[0]*4 );
          break;
        case MAKEFOURCC('Y','V','1','2'):
			hr = m_PrepareData.PrepareBufferARGB2YV12(pBuf, lStride, ppbPlanes, pdwStrides,
				srcWidth, srcHeight, dwFlags,m_lRotationMode,m_Deinterlaced_Mode,m_lFlipVideoMode);
		if(FAILED(hr))
			DP("!!!!!!!!Preapre failed err:0x%x \n",hr);

		break;
        default:
  			DP("Unknow format input:%p output:%p \n",m_OpenOptions.dwFourCC, m_ginfo.dwFourCC);
	  }
	}
	else if(m_OpenOptions.dwFourCC == MAKEFOURCC('R','5','6','5'))
	{

	  switch(m_ginfo.dwFourCC)
	  {
        case MAKEFOURCC('R','5','6','5'):
    		if(dwScaleFlags)
    		{
    			hr = m_PrepareData.PrepareBufferR565ToR565Scaled(pBuf, lStride, ppbPlanes, 
    			pdwStrides, &rScaleSrc, &rScaleDst, dwFlags, m_lRotationMode,m_lFlipVideoMode);
    			if(FAILED(hr))
    				DP("!!!!!!!!Preapre failed err:0x%x \n",hr);
    		}
    		else
			{
				for(int i=0;i<srcHeight[0];i++)
				memcpy(pBuf+lStride*i ,ppbPlanes[0] +pdwStrides[0]*i,srcWidth[0]*2);
				hr = S_OK;
			}
          break;
        case MAKEFOURCC('Y','V','1','2'):
		hr = m_PrepareData.PrepareBufferRGB5652YV12(pBuf, lStride, ppbPlanes, pdwStrides,
				srcWidth, srcHeight, dwFlags,m_lRotationMode,m_Deinterlaced_Mode,m_lFlipVideoMode);
		if(FAILED(hr))
			DP("!!!!!!!!Preapre failed err:0x%x \n",hr);

		break;
        default:
  			DP("Unknow format input:%p output:%p \n",m_OpenOptions.dwFourCC, m_ginfo.dwFourCC);
	  }

	}
	else if(m_OpenOptions.dwFourCC == MAKEFOURCC('Y','V','1','6'))
	{
		//if(m_ginfo.dwFourCC == MAKEFOURCC('R','5','6','5'))
		//{
		//	RECT rSrc;
		//	SetRect(&rSrc, 0, 0, m_OpenOptions.dwWidth, m_OpenOptions.dwHeight);
		//	hr = m_PrepareData.PrepareBufferYV16ToR565(pBuf,lStride, ppbPlanes,srcWidth,  &rSrc, &rSrc,0,0);
		//}
		//else
			DP("YV16 to 0x%x doesn't support. \n", surfaceFourCC);
	}

	else if(m_OpenOptions.dwFourCC == MAKEFOURCC('N','V','1','2'))
	{

	  switch(m_ginfo.dwFourCC)
	  {
        case MAKEFOURCC('N','V','1','2'):
          if(m_overlayType == VP_OPEN_FLAG_RENESASHW)
            break;

			if((DWORD)lStride == srcWidth[0] && (((DWORD)pdwStrides[0]) == srcWidth[0]))
			{
				memcpy(pBuf, ppbPlanes[0], srcWidth[0]*srcHeight[0]);

#ifdef PRIMAII
				DWORD alignheight = ((srcHeight[0]-1) & (~(64-1))) + 64;
				memcpy(pBuf + (srcWidth[0]* alignheight), 	ppbPlanes[1], srcWidth[0]*srcHeight[0]/2);
#else
				memcpy(pBuf + srcWidth[0]*srcHeight[0], 	ppbPlanes[1], srcWidth[0]*srcHeight[0]/2);
#endif
			}
			else
			{
#ifdef PRIMAII
				DWORD alignheight = ((srcHeight[0]-1) & (~(64-1))) + 64;
#endif

				for(unsigned int i=0;i<srcHeight[0];i++)
					memcpy(pBuf + i*lStride,	ppbPlanes[0] + i*pdwStrides[0], srcWidth[0]);

				for(unsigned int i=0;i<srcHeight[0]/2;i++)
				{
#ifdef PRIMAII
					memcpy(pBuf + i*lStride + (lStride* alignheight),
						ppbPlanes[1] + i*pdwStrides[0], srcWidth[0]);
#else
					memcpy(pBuf + i*lStride/2 + lStride*srcHeight[0],	
						ppbPlanes[1] + i*pdwStrides[0]/2, srcWidth[0]/2);
#endif
				}

			}
			hr = S_OK;

          break;

        default:
  			DP("Unknow format input:%p output:%p \n",m_OpenOptions.dwFourCC, m_ginfo.dwFourCC);
      }
	}

	if(m_pOverlay)
		m_pOverlay->Unlock(lNum);
	return hr;
}

STDMETHODIMP CVRPresentOvl::PresentBuffer(LONG lNum, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;

	if(m_pOverlay == NULL)
		return S_OK;
 
	if(m_Deinterlaced_Mode == VP_DEINTERLACE_BOB_SINGLE_FIELD)
		dwFlags = VP_PRESENT_FIELD_TOP_ONLY;

	HRESULT res= S_OK;

	CProtect proc(&m_csDisplay);

	if(dwFlags&VP_PRESENT_FIELD_TOPFIRST)
	{
		m_pOverlay->Show(m_lRenderEnabled & m_DMCP_dsp_enable );
		res = m_pOverlay->Flip(lNum,GFXOVERLAY_FLIP_ODD);
		if(FAILED(res))
			goto ERROR_PROCESS;
		if(m_FPS)
			Sleep(1000/(m_FPS*2));
		res = m_pOverlay->Flip(GFXOVERLAY_SURF_OVERLAY,GFXOVERLAY_FLIP_EVEN);
		if(FAILED(res))
			goto ERROR_PROCESS;
	}
	else if(dwFlags&VP_PRESENT_FIELD_BOTTOMFIRST)
	{
		m_pOverlay->Show(m_lRenderEnabled & m_DMCP_dsp_enable );
		res = m_pOverlay->Flip(lNum,GFXOVERLAY_FLIP_EVEN);
		if(FAILED(res))
			goto ERROR_PROCESS;
		if(m_FPS)
			Sleep(1000/(m_FPS*2));
		res = m_pOverlay->Flip(GFXOVERLAY_SURF_OVERLAY,GFXOVERLAY_FLIP_ODD);
		if(FAILED(res))
			goto ERROR_PROCESS;

	}
	else if(dwFlags&VP_PRESENT_FIELD_TOP_ONLY)
	{
		m_pOverlay->Show(m_lRenderEnabled & m_DMCP_dsp_enable );
		res = m_pOverlay->Flip(lNum,GFXOVERLAY_FLIP_ODD);
		if(FAILED(res))
			goto ERROR_PROCESS;
	}
	else if(dwFlags&VP_PRESENT_FIELD_BOTTOM_ONLY)
	{
		m_pOverlay->Show(m_lRenderEnabled & m_DMCP_dsp_enable );
		res = m_pOverlay->Flip(lNum,GFXOVERLAY_FLIP_EVEN);
		if(FAILED(res))
			goto ERROR_PROCESS;
	}
	else
	{
	#if defined(SIRF_A4_NEWOS)// || defined(MT3351_DBG)
		res = m_pOverlay->Flip(lNum,0);
		if(FAILED(res))
		{
			DP("!!!!!!!!Flip failed L:%d \n", __LINE__);
			goto ERROR_PROCESS;
		}
		m_pOverlay->Show(m_lRenderEnabled & m_DMCP_dsp_enable );
	#else
		m_pOverlay->Show(m_lRenderEnabled & m_DMCP_dsp_enable );
		res = m_pOverlay->Flip(lNum,0);
		if(FAILED(res))
		{
			DP("!!!!!!!!Flip failed L:%d \n", __LINE__);
			goto ERROR_PROCESS;
		}
	#endif
	}

ERROR_PROCESS:
	if(res == E_ACCESSDENIED)
	{//HARDWARE ACCELERATION DISABLED
		//if(m_OpenOptions.dwSurfType& VP_OPEN_FLAG_DDRAW) 
		//	m_OpenOptions.dwSurfType = (~VP_OPEN_FLAG_DDRAW)&m_OpenOptions.dwSurfType;
		//if(m_OpenOptions.dwSurfType& VP_OPEN_FLAG_D3D) 
		//	m_OpenOptions.dwSurfType = (~VP_OPEN_FLAG_D3D)&m_OpenOptions.dwSurfType;

		if(m_OpenOptions.dwSurfType != VP_OPEN_FLAG_AUTO)
				m_OpenOptions.dwSurfType |= VP_OPEN_FLAG_GDI;

		RECT dstBak = m_rDst;
		RECT srcBak = m_rSrcSize;
		LONG vpEnable = m_lRenderEnabled;
		Close();
		Open(&m_OpenOptions,  sizeof(m_OpenOptions));
		
		//Reinit dest rectangle. when hwnd is available.
		if(m_OpenOptions.hWnd)
		{
			#ifndef ANDROID
			#ifndef __linux__
			::GetClientRect((HWND)m_OpenOptions.hWnd, &dstBak);

			POINT p={0,0};
			::ClientToScreen((HWND)m_OpenOptions.hWnd, &p);

			OffsetRect(&dstBak,p.x,p.y);
			#endif
			#endif
		}
		Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_RECTDST,0,0,&dstBak,0);
		Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_RECTSRC,0,0,&srcBak,0);
		Set(EMC_PROPSET_GFXCTXT,MC_PROPID_VP_ENABLE,0,0,&vpEnable,0);

	}
	return S_OK;
}

STDMETHODIMP CVRPresentOvl::SetVBI(VP_VIDEOBUFFER_INFO *pVBI)
{
	if(!m_bOpen)
		return E_FAIL;

	HRESULT hr = S_OK;

	memcpy(&m_VBI, pVBI, sizeof(m_VBI));

	//Do not Recreate the surface while size doesn't change
	GfxOverlayInfo sInfo;
	ZeroMemory(&sInfo,sizeof(sInfo));
	sInfo.dwSize = sizeof(sInfo);
	if(m_pOverlay)
		hr = m_pOverlay->GetSurfaceInfo(&sInfo);

	if(SUCCEEDED(hr))
	{
		if((sInfo.dwWidth == m_VBI.dwWidth[0])&&(sInfo.dwHeight == m_VBI.dwHeight[0]))
			return S_OK;
	}
	else
		return E_FAIL;

	#ifdef PXA310
	UpdateDisplayParameters();
	#endif

	m_OpenOptions.dwWidth = m_VBI.dwWidth[0];
	m_OpenOptions.dwHeight = m_VBI.dwHeight[0];
	m_OpenOptions.dwAspectX = m_VBI.dwAspectX;
	m_OpenOptions.dwAspectY = m_VBI.dwAspectY;
	
	return Restore(FALSE);
}

STDMETHODIMP CVRPresentOvl::SetFPS(DWORD dwFPS)
{
	m_FPS = dwFPS;
	return S_OK;
}

STDMETHODIMP CVRPresentOvl::GetBackBuffers(DWORD *pdwBackBuffers)
{
	if(!m_bOpen)
		return E_FAIL;

	if( pdwBackBuffers==0)
		return E_FAIL;

	if(m_pOverlay==NULL)
		return S_OK;
	
	GfxOverlayInfo info;
	ZeroMemory(&info,sizeof(info));
	info.dwSize = sizeof(info);
	if(FAILED(m_pOverlay->GetSurfaceInfo(&info)))
		return E_FAIL;
	*pdwBackBuffers = info.dwBackBuffers;
	return S_OK;
}

STDMETHODIMP CVRPresentOvl::GetStatus(DWORD *pdwStatus)
{
	if(pdwStatus==0)
		return E_POINTER;
	*pdwStatus = m_dwStatus;
	return S_OK;
}

STDMETHODIMP CVRPresentOvl::GetCaps(VP_CAPS *pCaps)
{
	if(!m_bOpen)
		return E_FAIL;

	if(pCaps==0)
		return E_POINTER;

	pCaps->dwCaps= 0;
	switch(m_ginfo.dwFourCC)
	{
	case MAKEFOURCC('Y','V','1','2'):
	case MAKEFOURCC('Y','U','1','2'):
		pCaps->dwCaps |= VP_CAP_PREPARE_BOB;
		break;
	}
	pCaps->dwCaps |=(m_ginfo.dwCaps&GFXOVERLAY_CAP_FLIP_BOB ? VP_CAP_PRESENT_BOB : 0)
		| (m_ginfo.dwCaps&GFXOVERLAY_CAP_FLIP_DEINTERLACE ? VP_CAP_PRESENT_DEINTERLACE : 0)
		;

	if((m_overlayType == VP_OPEN_FLAG_D3D)|| (m_overlayType == VP_OPEN_FLAG_GDI))
		pCaps->dwCaps |= VP_CAP_CROPPING ;	
	return S_OK;
}

//
//STDMETHODIMP CVRPresentOvl::OpenAdv(IN VP_OpenOptionsAdv *pOpenParams, IN DWORD dwOpenSize)
//{
//#ifndef WINCE
//	VideoMixer_OpenParams param;
//	memset(&param, 0 ,sizeof(param));
//	param.dwQuality = pOpenParams->dwQuality;
//	param.dwRenderMode = pOpenParams->dwRenderMode;
//	param.fRenderFrequency = pOpenParams->fRenderFrequency;
//	param.hWnd = pOpenParams->hWnd;
//	return (m_pVideoMixer_D3D->Open(&param, sizeof(param)));
//#else
//	return E_NOTIMPL;
//#endif
//}
//
//STDMETHODIMP CVRPresentOvl::CloseAdv()
//{
//#ifndef WINCE
//	return (m_pVideoMixer_D3D->Close());
//#else
//	return E_NOTIMPL;
//#endif
//}
//
//STDMETHODIMP CVRPresentOvl::Render(DWORD dwRenderFlags)
//{
//#ifndef WINCE
//	if(m_pVideoMixer_D3D)
//		return (m_pVideoMixer_D3D->Render(dwRenderFlags));
//	else
//		return E_FAIL;
//#else
//	return E_NOTIMPL;
//#endif
//}
//
//STDMETHODIMP CVRPresentOvl::GetAdv(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
//	IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned)
//{
//#ifndef WINCE
//	if(m_pVideoMixer_D3D)
//		return (m_pVideoMixer_D3D->Get(guidPropSet, dwPropID, pInstanceData, 
//			cbInstanceData, pPropData, cbPropData, pcbReturned));
//	else
//		return E_FAIL;
//#else
//	return E_NOTIMPL;
//#endif
//}
//
//STDMETHODIMP CVRPresentOvl::SetAdv(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, 
//	IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData)
//{
//#ifndef WINCE
//	if(m_pVideoMixer_D3D)
//		return (m_pVideoMixer_D3D->Set(guidPropSet, dwPropID, pInstanceData, 
//			cbInstanceData, pPropData, cbPropData));
//	else
//		return E_FAIL;
//#else
//	return E_NOTIMPL;
//#endif
//}


HRESULT CVRPresentOvl::SetVideoWndDst()
{
	HRESULT ret = S_OK;
	RECT rSrc, rDst;
	if(m_pVideoWindow == 0)
		return E_FAIL;

	ret = m_pVideoWindow->Get(IID_IMcVideoPresenter, VE_COMMAND_SRCRECT,0,0,&rSrc, sizeof(rSrc),0);
	if(FAILED(ret))
		return ret;

	ret = m_pVideoWindow->Get(IID_IMcVideoPresenter, VE_COMMAND_DSTRECT,0,0,&rDst, sizeof(rDst),0);
	if(FAILED(ret))
		return ret;

#ifndef NO_ROT_SCL
	//Do not enable it. It causes PROC_SW zoom mode incorrect.
	if(m_StreamCount <=1)
	{
		ret = Set(EMC_PROPSET_GFXCTXT, MC_PROPID_VP_RECTSRC,0,0,&rSrc,sizeof(rSrc));
		if(FAILED(ret))
			return ret;
	}
#endif

	ret = SetRectDstInt(&rDst);
	if(FAILED(ret))
		return ret;

	if(m_dwStatus)
		Restore();

	return S_OK;
}

HRESULT CVRPresentOvl::SetRectDstInt(RECT* pDst)
{
	HRESULT ret = S_OK;
	RECT r;
	CProtect proc(&m_csDisplay);

	r = *pDst;

#ifdef SAMSUNG_2416
	DWORD dwAlign = 4;
	r.left = (r.left|(dwAlign -1)) + 1 - dwAlign ;
	r.top = (r.top|(dwAlign -1)) + 1 - dwAlign ;
	r.right = (r.right|(dwAlign -1)) + 1 - dwAlign ;
	r.bottom = (r.bottom|(dwAlign -1)) + 1 - dwAlign ;
#endif

	if(!EqualRect(&m_rDst,&r))
	{
		m_rDst = r;

#ifdef ZHONGHONG_MX31
		if((m_rDst.right >720)||(m_rDst.bottom >480))\
		{
			m_rDst.left = 40;
			m_rDst.right = 760;
			m_rDst.top = 16*2;
			m_rDst.bottom = 480-16*2;
		}
#elif defined(NO_ROT_SCL) 
		if(m_overlayType == VP_OPEN_FLAG_DDRAW)
		{
			DWORD A4_ddrawFourCC[] =
			{
				MAKEFOURCC('R','5','6','5'),
				0
			};
			if(m_pOverlay)
				m_pOverlay->Close();
#ifdef NO_ROT_SCL
		#define WIDTH_ALIGNED 4
		#define HEIGHT_ALIGNED 4
			m_rDst.left &= ~(WIDTH_ALIGNED -1);
			m_rDst.right &= ~(WIDTH_ALIGNED -1);
			m_rDst.top &= ~(HEIGHT_ALIGNED -1); 
			m_rDst.bottom &= ~(HEIGHT_ALIGNED -1); 
#endif
			IntersectRect(&m_rDst,&m_rDst,&m_rScreen);
			if(m_pOverlay)
				ret = m_pOverlay->Open(m_rDst.right - m_rDst.left, 
					m_rDst.bottom -  m_rDst.top, m_OpenOptions.dwBuffers,
					(const DWORD *)&A4_ddrawFourCC[0],m_pConfig);
			else
				ret = S_OK;
			if(FAILED(ret))
			{
				return ret;
			}
		}
#endif
		UpdateDisplayParameters();

		if(m_pDMCP)
		{
			ret = m_pDMCP->SetTargetRect(&m_rDst);
			if(FAILED(ret))
			{
				DP("[GFXPRESENT] SetTargetRect failed :0x%x \n",ret);
				return ret;
			}

			m_DMCP_dsp_enable = (m_pDMCP->CanDisplay(FALSE)!=0);
			if(m_DMCP_dsp_enable == FALSE)
			{
				if(m_bDMCPEnable == false)
					m_DMCP_dsp_enable = true;	
				else
					DP("DMCP->CanDisplay() = FALSE \n");
			}
		}

	}

	return S_OK;
}

HRESULT CVRPresentOvl::UpdateDestPos()
{
	if(m_OpenOptions.hWnd == 0)
		return S_OK;
	
#ifndef __linux__
	RECT relRect = {0,0,0,0};
	RECT abRect = {0,0,0,0};
	POINT p={0,0};
	CProtect proc(&m_csDisplay);

	::GetClientRect((HWND)m_OpenOptions.hWnd, &relRect);
	::ClientToScreen((HWND)m_OpenOptions.hWnd, &p);

	abRect = relRect;
	OffsetRect(&abRect,p.x,p.y);
	if(memcmp(&abRect, &m_absoluteRectBak ,sizeof(RECT)) != 0)
	{
		if(m_overlayType != VP_OPEN_FLAG_D3D)
		{
			#ifndef ANDROID
			OffsetRect(&relRect,p.x,p.y);
			#endif
		}

		if(m_pVideoWindow)
		{
			HRESULT ret;
			ret = m_pVideoWindow->Set(IID_IMcVideoPresenter, VE_COMMAND_DSTRECT,0,0,&relRect, sizeof(relRect));
			if(FAILED(ret))
				return ret;
			SetVideoWndDst();
		}
		rVideoWnd  = relRect;

		m_absoluteRectBak  = abRect;
	}
#endif

	return S_OK;
}


STDMETHODIMP CVRPresentOvl::LockSurface( LPBYTE *ppData, DWORD *pdwStride) 
{
	if(!m_pOverlay)	return E_UNEXPECTED;
	EnterCriticalSection(&m_csDisplay);
	if(FAILED(m_pOverlay->Lock(m_dwPresentIndex,(LPVOID *)ppData,(LONG*)pdwStride,0)))
	{
		LeaveCriticalSection(&m_csDisplay);
		return E_FAIL;
	}
	return S_OK;
}

STDMETHODIMP CVRPresentOvl::UnlockSurface() 
{
	if(!m_pOverlay)	return E_UNEXPECTED;
	if(FAILED(m_pOverlay->Unlock(m_dwPresentIndex)))
		return E_FAIL;
	
	LeaveCriticalSection(&m_csDisplay);
	return S_OK;
}

STDMETHODIMP CVRPresentOvl::FlipSurface(){
	if(!m_pOverlay)	return E_UNEXPECTED;
	
	m_pOverlay->Flip(m_dwPresentIndex, 0);
	if(++m_dwPresentIndex>=m_ginfo.dwBackBuffers)
		m_dwPresentIndex = 0;

	#ifdef DUMP_FPS
	g_frames++;

	if(g_frames%50 == 0)
	DP("FPS:%d FlipSurface()\n",g_frames*1000/(GetTickCount() -g_startTick ));
	#endif

	return S_OK;
}

extern CI_RESULT CI_TranslateColorKey(IN CI_U32 u32Mode, 	\
	IN CI_U32 u32InKey, OUT CI_U32 *pu32OutKey);

HRESULT CVRPresentOvl::TransferColorKey(DWORD in, DWORD* pout)
{
	if(m_OpenOptions.dwFourCC == m_ginfo.dwFourCC)
	{
		*pout = in;
		return S_OK;
	}

	switch(m_OpenOptions.dwFourCC)
	{
		case MAKEFOURCC('Y','V','1','2'):
			{
				switch(m_ginfo.dwFourCC)
				{
					case MAKEFOURCC('R','5','6','5'):
						{
							CI_U32 out;
							CI_RESULT hr = CI_TranslateColorKey(CI_IMAGEPROC_CSC_YCBCR420_TO_RGB565,
								in, &out);
							if(hr != CI_SOK)
								return E_FAIL;
							*pout = out ;
						}
						break;
					default:
						return E_NOTIMPL;
				}

			}
			break;
		default:
			return E_NOTIMPL;
	}

	return S_OK;
}

static STDMETHODIMP LoadVDecRnd(IMcVideoPresenter ** ppPresenter)
{
#ifdef __linux__
	return E_FAIL;
#else
	HMODULE h_dll;
	h_dll = LoadLibrary(_T("LibVDecRnd.dll")); 
	if(h_dll == 0)
	{
		DP("[Info]Do not have VDecRnd library\n");
		return E_FAIL;
      }

	fpCreateVDecRnd fpProcCreate = 0;
#ifdef _WIN32_WCE
	fpProcCreate = (fpCreateVDecRnd)GetProcAddress(h_dll, _T("CreateVDecRnd"));
#else
	fpProcCreate = (fpCreateVDecRnd)GetProcAddress(h_dll, "CreateVDecRnd");
#endif
	if(fpProcCreate == 0)
		return E_FAIL;

      IMcVDecRnd_Decoder* pDec = fpProcCreate();
	if(pDec == 0)	
		return E_FAIL;

	if(FAILED(pDec->QueryInterface(IID_IMcVideoPresenter, (void **)ppPresenter )) || (*ppPresenter == 0) )
		return E_FAIL;

	return S_OK;
#endif
}

IMcVideoPresenter *CreateVideoPresenter()
{
	IMcVideoPresenter *pPresenter = 0;
	if(SUCCEEDED(LoadVDecRnd(&pPresenter)))
		return pPresenter;

	#ifdef VM_SUPPORT
   	DP("%s: %s new CVRPresent\n",__FILE__, __FUNCTION__);
	return new CVRPresent();
	#else
   	DP("%s: %s new CVRPresentOvl\n",__FILE__, __FUNCTION__);
	return new CVRPresentOvl();
	#endif
}
#ifndef __linux__
IMcVideoPresenterAdv *CreateVideoPresenterAdv()
{
	HMODULE h_dll;
	h_dll = LoadLibrary(VIDEOPRESENTER_V2_FILENAME); 
	if(h_dll == 0)
		return 0;
	
	fpCreateVideoPresenterAdv	fpProcCreate;
#ifndef	WINCE
	fpProcCreate = (fpCreateVideoPresenterAdv)GetProcAddress(h_dll, "CreateVideoPresenterAdv");
#else
	fpProcCreate = (fpCreateVideoPresenterAdv)GetProcAddress(h_dll, _T("CreateVideoPresenterAdv"));
#endif
	if(fpProcCreate == 0)
		return 0;

	return fpProcCreate();

}
#endif

IMcVideoPresenterAnimation*	CreateVideoAnimation()
{
	HMODULE h_dll;
	h_dll = LoadLibrary(VIDEOPRESENTER_V2_FILENAME); 
	if(h_dll == 0)
		return 0;

	fpCreateVideoAnimation fpProcCreate;
#ifndef	WINCE
	fpProcCreate = (fpCreateVideoAnimation)GetProcAddress(h_dll, "CreateVideoAnimation");
#else
	fpProcCreate = (fpCreateVideoAnimation)GetProcAddress(h_dll, _T("CreateVideoAnimation"));
#endif
	if(fpProcCreate == 0)
		return 0;

	return fpProcCreate();

}
#ifndef __linux__
extern ICivmVideoMixer *CreateVideoMixerPRI(E_MIXERTYPE type);

ICivmVideoMixer *CreateVideoMixer(E_MIXERTYPE type)
{
	return CreateVideoMixerPRI(type);

}
#endif
#endif

#ifdef _MANAGED
#pragma managed(push, off)
#endif

static TCHAR g_szModulePath[MAX_PATH+1];  // present only for WinCE

#ifndef ARRAYSIZE
#define ARRAYSIZE(a)	 sizeof(a)/sizeof(a[0]) 
#endif
static BOOL UpdateDllDirectory(HMODULE hLib)
{
	TCHAR *tp;
#ifndef __linux__
	if(GetModuleFileName(hLib,g_szModulePath,ARRAYSIZE(g_szModulePath))==0)
		return FALSE;
#endif	
	if((tp = _tcsrchr(g_szModulePath,_T('\\')))==0 &&
		(tp = _tcsrchr(g_szModulePath,_T('/')))==0)
	{
		g_szModulePath[0] = 0;
		return FALSE;
	}
	tp[1] = 0;
#if defined(WINCE) || defined(__linux__)
	return TRUE;
#else
	return SetDllDirectory(g_szModulePath);
#endif
}

#ifndef __linux__
#if defined(WINCE)
BOOL APIENTRY DllMain( HANDLE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
#else
BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
#endif
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		UpdateDllDirectory((HMODULE)hModule); // adds current dll directory to dll search path
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#endif
#ifdef _MANAGED
#pragma managed(pop)
#endif

