//=============================================================================
// CIDANA CONFIDENTIAL INFORMATION
//
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO CIDANA, INC.
//	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//	COMPANY.
//
//	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//	PURPOSE.
//
//	Copyright (c) 2008  Cidana, Inc. All Rights Reserved.
//
//-----------------------------------------------------------------------------

// Things to do:(achung)
// 1) Check directdraw surface lost through resolution change - done
// 2) Check multimon support (multiple monitors)- done
// 3) Check overlay failure (creation)- done
// 4) Implement interlacing support
#include "../../inc/GfxBuildDef.h"


#ifdef PRIMAII_SAVECOPY
#include <stdio.h>
#include <windows.h>
#include <ddraw.h>
#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"

#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"dxguid.lib")

#if ((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501))
typedef HRESULT (*LPDIRECTDRAWENUMERATEEX)(LPDDENUMCALLBACKEX lpCallback, LPVOID lpContext, DWORD dwFlags);
#endif
#ifdef _WIN32_WCE
#ifdef DIRECTDRAW_VERSION
#define IDirectDraw7 IDirectDraw4
#define IDirectDrawSurface7 IDirectDrawSurface4
#define IID_IDirectDraw7 IID_IDirectDraw4
#define LPDIRECTDRAWSURFACE7 LPDIRECTDRAWSURFACE4
#define DDFLIP_WAITVSYNC DDFLIP_WAIT
#define DDBLT_WAITVSYNC DDBLT_WAIT
#define DDDEVICEIDENTIFIER2 DDDEVICEIDENTIFIER
#else
#define IDirectDraw7 IDirectDraw
#define IDirectDrawSurface7 IDirectDrawSurface
#define IID_IDirectDraw7 IID_IDirectDraw
#define LPDIRECTDRAWSURFACE7 LPDIRECTDRAWSURFACE
#define LPDDSURFACEDESC2 LPDDSURFACEDESC
#define DDSURFACEDESC2 DDSURFACEDESC
#define DDDEVICEIDENTIFIER2 DDDEVICEIDENTIFIER
#endif
#endif

#define SURFACELOST_RESTORE_TIMEOUT 5000

struct GfxMonitorInfo
{
	GUID guid;
	HMONITOR hm;
	MONITORINFO info;
};


typedef struct
{
	IDirectDraw* pDDraw;
	IDirectDrawSurface* pPrimary;
	IDirectDrawSurface* pOverlay;
	IDirectDrawSurface* pBackbuffer;
	RECT rSrc;
}SSHARED_DATA;


#define DDBLT_DEINTERLACE DDBLT_KEYSRCOVERRIDE

 typedef struct _DDDEINTERLACEPARAMS
{
	DWORD dwField;
	DWORD dwReserved;
}DDDEINTERLACEPARAMS,* LPDDDEINTERLACEPARAMS;


typedef struct _DDDEINTERLACEBLTFX
{
	DWORD dwSize;// size of structure
	DWORD dwROP;// Win32 raster operations
	DWORD dwFillColor;// color in RGB or Palettized (Brush value for Win32 ROPs)
	DDCOLORKEY ddckDestColorkey;// DestColorkey override
	DDDEINTERLACEPARAMS dddipDeinterlaceParams;// deinterlace params
} DDDEINTERLACEBLTFX,* LPDDDEINTERLACEBLTFX;

class CGfxDDraw : public IMcGfxOverlay, public CMcUnknown
{
#define MONITORS_AVAIL_NUM	9
#define DISPMODE_NUM	36
public:
	DECLARE_IUNKNOWN

	CGfxDDraw(DWORD surface_type);
	virtual ~CGfxDDraw();
	STDMETHODIMP Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers,
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst = NULL);		
	STDMETHODIMP Close();
	STDMETHODIMP Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount);
	STDMETHODIMP Unlock(LONG num);
	STDMETHODIMP Flip(LONG num, DWORD dwFlags);
	STDMETHODIMP Show(BOOL bShow);
	STDMETHODIMP GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags);
	STDMETHODIMP SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags);
	STDMETHODIMP SetSrcRect(const RECT *pRect);
	STDMETHODIMP SetDstRect(const RECT *pRect);
	STDMETHODIMP SetScnRect(const RECT *pRect);
	STDMETHODIMP GetScnRect(RECT *pRect);

	STDMETHODIMP SetWindowHandle(HWND hwnd);
	STDMETHODIMP GetWindowHandle(HWND& hwnd);
	STDMETHODIMP SetAlpha(DWORD dwAlpha);
	STDMETHODIMP SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey);
	STDMETHODIMP SetDstColorKey(BOOL bEnable, DWORD dwDstCkey);
	STDMETHODIMP GetColorControl(LONG num, DDCOLORCONTROL *pCC);
	STDMETHODIMP GetGammaControl(LONG num, DDGAMMARAMP *pGR);
	STDMETHODIMP SetColorControl(LONG num, DDCOLORCONTROL *pCC);
	STDMETHODIMP SetGammaControl(LONG num, DDGAMMARAMP *pGR);
	STDMETHODIMP GetDDCaps(LONG *pCaps);
	STDMETHODIMP GetDDDevice(LPVOID *pDevice, DWORD *pdwSize);
	STDMETHODIMP GetSurfaceInfo(GfxOverlayInfo *pInfo);
	STDMETHODIMP IsOpen();
	STDMETHODIMP GetVRHandle(LPVOID pHandle)
	{
		return E_FAIL;
	}
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
  STDMETHODIMP SetHWRenderSurf(LPVOID pSurf,int mode)
	{
	#ifdef PRIMAII_SAVECOPY
	//	IDirectDrawSurface* overlay =(IDirectDrawSurface*)pSurf;

		if(m_surfaceFromCodec.pDDraw == 0)
			SetRect(&m_rectDst,0,0,0,0);
		
		memcpy(&m_surfaceFromCodec, pSurf, sizeof(m_surfaceFromCodec));

		static LONG i =0;
		IDirectDrawSurface7 * pBackbuff = GetSurface(i);
		i++;
		if(i>=m_dwBackBuffers)
			i=0;


		DDDEINTERLACEBLTFX sBltFX ={0};
		sBltFX.dwSize = sizeof(sBltFX);
		sBltFX.dddipDeinterlaceParams.dwField ='EVEN';// ¡®EVEN¡¯ or ¡® ODD¡¯,

		if(pBackbuff)
		{
			if(m_deinterlaced)
			  pBackbuff->Blt(&m_rectDst, m_surfaceFromCodec.pBackbuffer,&m_surfaceFromCodec.rSrc,
								  DDBLT_DEINTERLACE,(LPDDBLTFX)&sBltFX);
					 else
			  pBackbuff->Blt(&m_rectDst, m_surfaceFromCodec.pBackbuffer,&m_surfaceFromCodec.rSrc,
								  0, 0);
		}

		return S_OK;
	#else
		return E_NOTIMPL;
	#endif
	}
	STDMETHODIMP SetDeinterlaceMode(DWORD dwMode)
	{
		m_deinterlaced = dwMode;
		return S_OK;
	}

	STDMETHODIMP GetDeinterlaceMode(DWORD* pdwMode)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;}
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;} 
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}


protected:
	STDMETHODIMP UpdateOverlay_ZeroBackbuff();
	STDMETHODIMP SetPixelFormat(DDPIXELFORMAT *pPF, DWORD dwFourCC);
	STDMETHODIMP CreateSurfaces(DWORD dwBuffers);
	STDMETHODIMP ReleaseSurfaces();
	STDMETHODIMP Restore();
	STDMETHODIMP Update();
	STDMETHODIMP FindTrueDstCkey();
	STDMETHODIMP Blit(LONG num, DWORD dwFlags);
	LPDIRECTDRAWSURFACE7 GetSurface(LONG num);
	STDMETHODIMP EnumerateMonitors();
	GfxMonitorInfo *GetBestMonitor(POINT *pt);
	static HRESULT WINAPI EnumSurfaceCallback(LPDIRECTDRAWSURFACE7 pSurf, LPDDSURFACEDESC2 pDesc, LPVOID lpContext);
	static BOOL WINAPI EnumMonitorCallbackEx(GUID FAR* lpGUID,
											 LPTSTR lpDriverDescription, LPTSTR lpDriverName,
											LPVOID lpContext, HMONITOR hm);
	static BOOL WINAPI EnumMonitorCallback(GUID FAR* lpGUID,
											 LPTSTR lpDriverDescription, LPTSTR lpDriverName,
											LPVOID lpContext);

	STDMETHODIMP EnumerateDispModes();

	static HRESULT WINAPI EnumModesCallback(LPDDSURFACEDESC2 lpDDSurfaceDesc,
	 LPVOID lpContext);
	char *GetErrorMsg(HRESULT DDRVAL);

	
	RECT				m_rectSrc, m_rectDst, m_rectScn;
	RECT				m_rectBltSrc, m_rectBltDst;
	DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
	DWORD				m_dwLastFlipTime;
	const DWORD			*m_pdwFourCCList;
	DWORD				m_dwBackBuffers;
	BOOL				m_bShow, m_bLastShow, m_bDstCkey, m_bSrcCkey, m_bScnClip, m_bDstTrueCkeyValid, m_bAlpha;
	DWORD				m_dwDstCkey, m_dwSrcCkey, m_dwDstTrueCkey;
	DWORD				m_dwNumFourCCCodes;
	DWORD				*m_pdwFourCCCodes;
	IDirectDraw7		*m_pDDraw;
	IDirectDrawSurface7 *m_pPrimary,*m_pOverlay,**m_pBackBuffer;
	DDOVERLAYFX			m_OFX;
	DDBLTFX				m_BFX;
	DDCAPS				m_Caps;
	DDSURFACEDESC2		m_PrimaryDesc;
	DDDEVICEIDENTIFIER2	m_Device;
	HWND				m_hwnd;
	IDirectDrawClipper	*m_pClipper;
	DWORD				m_dwNumMonitors;
	GfxMonitorInfo		m_aMonitors[MONITORS_AVAIL_NUM];
	GfxMonitorInfo		*m_pMonitorInfo;

	DWORD				m_dwNumDisps;
	DDSURFACEDESC2 m_aDisps[DISPMODE_NUM];
	DWORD m_surface_type;
	BOOL m_bOpen;	
	long long m_frameCount;

	VP_STRUCT_ST_PARMS m_stparam;
	SSHARED_DATA m_surfaceFromCodec;
	DWORD m_deinterlaced;

};

static const DWORD const_dwFourCC[]=
{
	MAKEFOURCC('N','V','1','2'),
	MAKEFOURCC('Y','V','1','2'),
	MAKEFOURCC('Y','U','Y','2'),
	MAKEFOURCC('Y','U','Y','V'),
	MAKEFOURCC('R','5','6','5'),
	//MAKEFOURCC('A','R','3','2'),
	//MAKEFOURCC('I','M','C','1'),
	0
};

static const DWORD const_MT3353_FourCC_HW[]={
MAKEFOURCC('M','T','K','1'),
0
};

static const DWORD const_MT3353_FourCC_SW[]={
MAKEFOURCC('N','V','1','2'),
0
};

static const DWORD const_PRIMAII_FourCC[]={
MAKEFOURCC('N','V','1','2'),
0
};

CGfxDDraw::CGfxDDraw(DWORD surface_type): CMcUnknown(0,0)
{
	m_pDDraw = 0;
	m_pPrimary = 0;
	m_pOverlay = 0;
	m_pBackBuffer = 0;
	m_dwWidth = m_dwHeight = 0;
	m_dwBackBuffers = 0;
	m_bShow = m_bLastShow = m_bDstCkey = m_bSrcCkey = m_bScnClip = m_bDstTrueCkeyValid = m_bAlpha = FALSE;
	m_dwDstCkey = m_dwSrcCkey = 0;
	ZeroMemory(&m_OFX,sizeof(m_OFX));
	m_OFX.dwSize = sizeof(m_OFX);
	ZeroMemory(&m_BFX,sizeof(m_BFX));
	m_BFX.dwSize = sizeof(m_BFX);
	ZeroMemory(&m_Caps,sizeof(m_Caps));
	m_Caps.dwSize = sizeof(m_Caps);
	ZeroMemory(&m_PrimaryDesc,sizeof(m_PrimaryDesc));
	m_PrimaryDesc.dwSize = sizeof(m_PrimaryDesc);
	ZeroMemory(&m_Device,sizeof(m_Device));
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectScn,sizeof(m_rectScn));
	ZeroMemory(&m_rectBltSrc,sizeof(m_rectBltSrc));
	ZeroMemory(&m_rectBltDst,sizeof(m_rectBltDst));
	ZeroMemory(&m_aMonitors,sizeof(m_aMonitors));
	ZeroMemory(&m_aDisps,sizeof(m_aDisps));
	m_hwnd = 0;
	m_pClipper = 0;
	m_dwNumFourCCCodes = 0;
	m_pdwFourCCCodes = 0;
	m_dwNumMonitors = 0;
	m_pMonitorInfo = 0;
	m_dwNumDisps = 0;
	m_surface_type = surface_type;
	m_bOpen = FALSE;
	memset(&m_stparam, 0 ,sizeof(m_stparam));
	memset(&m_surfaceFromCodec, 0 ,sizeof(m_surfaceFromCodec));
}

CGfxDDraw::~CGfxDDraw()
{
}

HRESULT WINAPI CGfxDDraw::EnumModesCallback(
 LPDDSURFACEDESC2 lpDDSurfaceDesc,
 LPVOID lpContext
)
{

	CGfxDDraw *pMe;
	DDSURFACEDESC2 *pDisp;

	pMe = reinterpret_cast<CGfxDDraw *>(lpContext);
	pDisp =&pMe->m_aDisps[pMe->m_dwNumDisps++];
	*pDisp =*lpDDSurfaceDesc;
	return ((pMe->m_dwNumDisps<DISPMODE_NUM)?S_OK:E_FAIL);
}

STDMETHODIMP CGfxDDraw::EnumerateDispModes()
{
	if(m_pDDraw ==NULL)
		return E_FAIL;

	return m_pDDraw->EnumDisplayModes(NULL,NULL,this,EnumModesCallback);
}


STDMETHODIMP CGfxDDraw::EnumerateMonitors()
{
	HRESULT hr;
	HINSTANCE hlib;
	LPDIRECTDRAWENUMERATEEX pDirectDrawEnumerateEx;

	hlib = LoadLibrary(TEXT("ddraw.dll"));
	if(hlib==0)
		return E_FAIL;

	m_dwNumMonitors = 0;
#ifdef _WIN32_WCE
	#if ((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501))
	  pDirectDrawEnumerateEx =(LPDIRECTDRAWENUMERATEEX )GetProcAddress(hlib,L"DirectDrawEnumerateEx");
	#else
	  pDirectDrawEnumerateEx =(LPDIRECTDRAWENUMERATEEX )GetProcAddress(hlib,L"DirectDrawEnumerateExW");
	#endif
#else
  pDirectDrawEnumerateEx =(LPDIRECTDRAWENUMERATEEX )GetProcAddress(hlib,"DirectDrawEnumerateExA");
#endif
  if(pDirectDrawEnumerateEx)
	{
	  hr = pDirectDrawEnumerateEx(EnumMonitorCallbackEx, this,
							  DDENUM_ATTACHEDSECONDARYDEVICES |
							  DDENUM_DETACHEDSECONDARYDEVICES
		#if ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))
								|DDENUM_NONDISPLAYDEVICES
		#endif
		);
	}
#if ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))
	else
		hr = DirectDrawEnumerate(EnumMonitorCallback,this);
#endif
	FreeLibrary(hlib);
	return SUCCEEDED(hr)? S_OK : E_FAIL;
}

GfxMonitorInfo *CGfxDDraw::GetBestMonitor(POINT *pt)
{
	int i;

	for(i=0;i<(signed)m_dwNumMonitors;i++)
	{
		if(PtInRect(&m_aMonitors[i].info.rcMonitor,*pt))
			return &m_aMonitors[i];
	}
	return 0;
}

STDMETHODIMP CGfxDDraw::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers,
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	POINT pt;

	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;
	if(m_bOpen)
	{
		DP("[GFXDDRAW]Invalid call \n");
		return E_FAIL;
	}

	m_deinterlaced = 0;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));

	memset(&m_stparam, 0 ,sizeof(m_stparam));


#ifdef MTK3353_SW
	m_pdwFourCCList = const_MT3353_FourCC_SW;
#elif defined(MTK3353_HW)
	m_pdwFourCCList = const_MT3353_FourCC_HW;
#else
	if(pdwFourCC)
		m_pdwFourCCList = pdwFourCC;
	else
		m_pdwFourCCList = const_dwFourCC;
#endif

	DP("[GFXDDRAW]Open w:%d h:%d \n",m_dwWidth, m_dwHeight);
	EnumerateMonitors();
	pt.x = m_rectDst.left;
	pt.y = m_rectDst.top;
	m_pMonitorInfo = GetBestMonitor(&pt);
	#ifdef PXA310
	m_bScnClip = FALSE;
	#else
	m_bScnClip = TRUE;
	#endif
	if(m_pMonitorInfo==0)
		SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	else
		m_rectScn = m_pMonitorInfo->info.rcMonitor;

	m_pDDraw = m_surfaceFromCodec.pDDraw;
	m_pPrimary = m_surfaceFromCodec.pPrimary;
	
	if((m_pDDraw == 0)||(m_pPrimary ==0))	goto success;
	
	if(FAILED(m_pDDraw->GetCaps(&m_Caps,0)))
	{
		m_pDDraw->Release();
		m_pDDraw = 0;
		return E_FAIL;
	}
#if ((_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501))
	if(!(m_Caps.dwCaps2&DDCAPS2_WIDESURFACES))
	{	//check whether supported
		if((m_dwWidth> static_cast<DWORD>(m_rectScn.right))||(m_dwHeight > static_cast<DWORD>(m_rectScn.bottom)))
		{
			DP("[GFXDDRAW] Not support size(%d ,%d )\n",m_dwWidth, m_dwHeight );
			return E_FAIL;
		}
	}
#endif

#if (!((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501)))
	if(m_Caps.dwCaps == DDCAPS_NOHARDWARE)
		return E_ACCESSDENIED;
#endif
	EnumerateDispModes();
	if(m_pdwFourCCCodes)
	{
		delete[] m_pdwFourCCCodes;
		m_pdwFourCCCodes = 0;
	}
	if(m_Caps.dwNumFourCCCodes)
	{
		m_dwNumFourCCCodes = m_Caps.dwNumFourCCCodes;
		m_pdwFourCCCodes = new DWORD[m_Caps.dwNumFourCCCodes];
		m_pDDraw->GetFourCCCodes(&m_dwNumFourCCCodes,m_pdwFourCCCodes);
		//OutputDebugStringA((char *)m_pdwFourCCCodes);
	}
#ifndef MT3351_DBG
	m_pDDraw->GetDeviceIdentifier(&m_Device,0);
#endif

	if(SUCCEEDED(CreateSurfaces(dwBuffers)))
	{

#if 0//def FREESCALE_IMX51
		HDC hDisplay = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
		InterlacedVideoData interlaceData;
		interlaceData.bIsInterlaced = TRUE;
		interlaceData.topField = TopFieldSelect_Even;
		if (ExtEscape(hDisplay, DISPLAY_IS_VIDEO_INTERLACED, sizeof(InterlacedVideoData),(LPSTR)&interlaceData,
		NULL, NULL)< 0)
		{
			return E_FAIL;
		}

#endif


#if defined(MT3351_RGB)|| defined(PRIMAII_SAVECOPY)
		SetRect(&m_rectDst,0,0,m_dwWidth,m_dwHeight);
#endif
success:
		SetRect(&m_rectSrc, 0,0,m_dwWidth, m_dwHeight);

		m_frameCount = 0;
		m_bOpen = TRUE;
		DP("[GFXDDRAW]OPEN SUCCESS\n");

		return S_OK;
	}
	Close();
	return E_FAIL;
}

STDMETHODIMP CGfxDDraw::Close()
{
	if(!m_bOpen)
		return S_OK;
		
	ReleaseSurfaces();

	if(m_pClipper)
	{
		m_pClipper->Release();
		m_pClipper = NULL;
	}

	if(m_pdwFourCCCodes)
	{
		delete[] m_pdwFourCCCodes;
		m_pdwFourCCCodes = 0;
	}
	m_dwNumMonitors = 0;
	m_dwNumDisps = 0;

	m_bOpen = FALSE;
	DP("[GFXDDRAW]Close SUCCESS\n");
	return S_OK;
}

STDMETHODIMP CGfxDDraw::CreateSurfaces(DWORD dwBuffers)
{
	int i,cc;
	IDirectDrawSurface7 *pSurf;
	DDSURFACEDESC2 ddsd;

	if(m_dwWidth==0 || m_dwHeight==0 || m_pdwFourCCList==0 || m_pOverlay)
		return E_FAIL;
	if(FAILED(m_pDDraw->SetCooperativeLevel(0,DDSCL_NORMAL)))
		return E_FAIL;



#ifndef _WIN32_WCE
	if(FAILED(m_pPrimary->GetSurfaceDesc(&m_PrimaryDesc)))
	{
		m_pPrimary->Release();
		m_pPrimary = 0;
		return E_FAIL;
	}
#endif

	//Init parameters
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OVERLAY;
	ddsd.dwFlags |= DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps |= DDSCAPS_FLIP ;
	#ifdef DDSCAPS_COMPLEX
	ddsd.ddsCaps.dwCaps |= DDSCAPS_COMPLEX;
	#endif

	if(dwBuffers)
		ddsd.dwFlags |= DDSD_BACKBUFFERCOUNT;

#ifdef PRIMAII_SPD
	#define DDSD_CKSRCBLT  	  0x00010000
	ddsd.dwFlags |= DDSD_CKSRCBLT ;
	ddsd.ddckCKSrcBlt.dwColorSpaceHighValue ='ALLO';
	ddsd.ddckCKSrcBlt.dwColorSpaceLowValue = 0 ;	 
#endif

	ddsd.dwWidth = m_dwWidth;
	ddsd.dwHeight = m_dwHeight;

#ifdef PRIMAII_SAVECOPY
	return S_OK;
#endif
	//Create overlay surface
	if((m_surface_type == VP_OPEN_FLAG_DDRAW_AUTO)||(m_surface_type&VP_OPEN_FLAG_DDRAW_OVERLAY))
	{
#if ((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501))
		if (m_Caps.dwOverlayCaps != 0)
#else
		if(m_Caps.dwCaps&DDCAPS_OVERLAY)
#endif
		{	// attempt to create overlay surface
			for(cc=0;m_pdwFourCCList[cc];cc++)
			{
				HRESULT hr;
				if(FAILED(SetPixelFormat(&ddsd.ddpfPixelFormat,m_pdwFourCCList[cc])))
					continue;
				m_dwFourCC = m_pdwFourCCList[cc];
				for(i=dwBuffers;i>=1;i--)
				{
					ddsd.dwBackBufferCount = i;
					hr = m_pDDraw->CreateSurface(&ddsd,&m_pOverlay,0);
					if(SUCCEEDED(hr))
					{
						DP("backbuffer :%d \n", ddsd.dwBackBufferCount);
						break;
					}
				}
				if(i<=0)
				{
					DP("[FOUCC]: 0x%x 0x%x \n", m_pdwFourCCList[cc],hr);
					DP("GFXDDRAW ERROR !!!\n", __LINE__);
					continue;
				}
				DP("[GFXDDRAW_OVERLAY]Create [0x%x]surface success \n",m_pdwFourCCList[cc]);

				if(i==0)
				{
					m_dwBackBuffers = 0;
				}
				else
				{
					m_pBackBuffer = new LPDIRECTDRAWSURFACE7[i];
					m_dwBackBuffers = 0;
					/* get back buffers */
					for(pSurf=m_pOverlay;m_dwBackBuffers<(unsigned)i;)
					{
						if(FAILED(pSurf->EnumAttachedSurfaces(this,EnumSurfaceCallback)))
							break;
						pSurf = m_pBackBuffer[m_dwBackBuffers-1];
					}
				}
				
				if(m_pdwFourCCList[cc]== MAKEFOURCC('Y','V','1','2'))
				{
					LPBYTE pDst = NULL;
					LONG dstStride = 0;
					LONG rgbCount= 0;
					//Lock(GFXOVERLAY_SURF_PRIMARY,(LPVOID*)&pDst,&dstStride,&rgbCount);
					//memset(pDst,0,m_rectScn.right*m_rectScn.bottom*rgbCount/8);
					//Unlock(GFXOVERLAY_SURF_PRIMARY);
					for(int i=0;i<(int)m_dwBackBuffers;i++)
					{
						hr = Lock(i,(LPVOID*)&pDst,&dstStride,0 );
						if(SUCCEEDED(hr))
						{
							for(int j=0;j<(int)m_dwHeight;j++)
							{
								memset(pDst + dstStride*i, 0x00, m_dwWidth);
							}

							for(int j=0;j<(int)m_dwHeight/2;j++)
							{
								memset(pDst +m_dwWidth*m_dwHeight+ dstStride/2*i, 0x80,m_dwWidth/2);
							}

							for(int j=0;j<(int)m_dwHeight/2;j++)
							{
								memset(pDst +m_dwWidth*m_dwHeight*5/4+ dstStride/2*i, 0x80,m_dwWidth/2);
							}
							
							Unlock(i);
						}

					}
				}
				DP("[GFXDDRAW_OVERLAY]Overlay_Backbuf surface is created sucessfully \n");
				return S_OK;
			}
		}
		m_pOverlay = 0;
	}


	//Create blit surface
	if((m_surface_type == VP_OPEN_FLAG_DDRAW_AUTO)||(m_surface_type&VP_OPEN_FLAG_DDRAW_BLIT))
	{
#if ((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501))
		if(m_Caps.dwBltCaps)
#else
		if((m_Caps.dwCaps&DDCAPS_BLTFOURCC || m_Caps.dwCaps&DDCAPS_BLT)&& m_Caps.dwCaps&DDCAPS_BLTSTRETCH)
#endif
		{
			ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
			ddsd.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY;
			m_pBackBuffer = new LPDIRECTDRAWSURFACE7[dwBuffers];
			for(cc=0;m_pdwFourCCList[cc];cc++)
			{
				if(FAILED(SetPixelFormat(&ddsd.ddpfPixelFormat,m_pdwFourCCList[cc])))
					continue;
				m_dwFourCC = m_pdwFourCCList[cc];
				ZeroMemory(m_pBackBuffer,sizeof(m_pBackBuffer[0])*dwBuffers);
				for(i=0;i<(signed)dwBuffers;i++)
				{
					if(FAILED(m_pDDraw->CreateSurface(&ddsd,&m_pBackBuffer[i],0))|| m_pBackBuffer[i]==0)
						break;
				}
				if(i==0)
					continue;
/*
				if(m_hwnd)
				{
					m_pClipper = 0;
					HRESULT hr = m_pDDraw->CreateClipper(0,&m_pClipper,0);
					if(SUCCEEDED(hr)&& m_pClipper)	//[Golan]Clipper isn't enabled currently. If you want to open it. Please remember to create a cliplist and attach to Clipper.
					{
						hr = m_pClipper->SetHWnd(0,m_hwnd);
						if(SUCCEEDED(hr))
						{
							hr = m_pPrimary->SetClipper(m_pClipper);
							if(FAILED(hr))
								m_pClipper->Release();
						}
						else
							m_pClipper->Release();
					}
				}
*/
				m_dwBackBuffers = i;
				DP("[GFXDDRAW_NONOVERLAY]Primary_backbuf surface[0x%x] is created sucessfully \n",m_pdwFourCCList[cc]);
				return S_OK;
			}
		}

	}



	//Create failed . Release all.
	if(m_pBackBuffer)
	{
		delete[] m_pBackBuffer;
		m_pBackBuffer = 0;
		m_dwBackBuffers = 0;
	}
	/* check if the surface allocations allow primary surface locking */
	for(cc=0;m_pdwFourCCList[cc];cc++)
	{
		m_dwFourCC = m_pdwFourCCList[cc];
		switch(m_dwFourCC)
		{
		case MAKEFOURCC('P','5','6','5'):	// primary 565
			{
				DDPIXELFORMAT *pPF;

				pPF =&m_PrimaryDesc.ddpfPixelFormat;
				if(pPF->dwFlags&DDPF_RGB &&
					pPF->dwRGBBitCount==16 &&
					pPF->dwRBitMask==0xf800 &&
					pPF->dwGBitMask==0x07e0 &&
					pPF->dwBBitMask==0x001f)
					return S_OK;
			}
		}
	}
	ReleaseSurfaces();
	return E_FAIL;
}

STDMETHODIMP CGfxDDraw::SetSrcRect(const RECT *pRect)
{
	if(!m_bOpen)
		return E_FAIL;

	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;

	#ifdef MT3351_RGB
		return S_OK;
	#endif
	
	m_rectSrc =*pRect;

	#if defined(SIRF_A4_NEWOS)&&!defined(SIRF_A6)

		if(m_rectSrc.left > 36)
		{
			DWORD width = m_rectSrc.right - m_rectSrc.left;
			m_rectSrc.left = 36;
			m_rectSrc.right = m_rectSrc.left + width;
		}
	#endif

	DP("[GFXDDRAW]Set SourceRect (%d,%d,%d,%d)\n", m_rectSrc.left, m_rectSrc.top, m_rectSrc.right, m_rectSrc.bottom);
	return Update();
}

STDMETHODIMP CGfxDDraw::SetDstRect(const RECT *pRect)
{
	if(!m_bOpen)
		return E_FAIL;

	POINT pt;
	GfxMonitorInfo *pinfo;

#ifdef FREESCALE_IMX51
	if(pRect)
#endif
	{
		if(EqualRect(pRect,&m_rectDst))
			return S_OK;

		if((pRect->right - pRect->left == 0)&&(pRect->bottom - pRect->top == 0))
			return S_OK;

#if defined(MT3351_RGB)|| defined(PRIMAII_SAVECOPY)
		if((pRect->right - pRect->left != m_rectDst.right - m_rectDst.left)||(pRect->bottom - pRect->top != m_rectDst.bottom - m_rectDst.top))
			return E_UNEXPECTED;
#endif
		DP("[GFXDDRAW]Set destRect (%d,%d,%d,%d)\n", pRect->left, pRect->top, pRect->right, pRect->bottom);
		m_rectDst =*pRect;
	}
#ifdef HOLEX_A4
	float height =(float)((float)(m_rectDst.bottom - m_rectDst.top)*(float)1.09);
	float gap =(height -(float)(m_rectDst.bottom - m_rectDst.top))/2;
	m_rectDst.bottom +=gap;
	m_rectDst.top -= gap;
#endif

	if(m_pMonitorInfo)
	{	// the size and offset may have changed... by simply restoring surfaces
		GetMonitorInfo(m_pMonitorInfo->hm,&m_pMonitorInfo->info);
		m_rectScn = m_pMonitorInfo->info.rcMonitor;
	}
	else
		SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	Update();
	pt.x = m_rectDst.left;	// check to see if we have gone to a different monitor
	pt.y = m_rectDst.top;
	pinfo = GetBestMonitor(&pt);
	if(pinfo && m_pMonitorInfo!=pinfo)
		return S_FALSE;
	return S_OK;
}

STDMETHODIMP CGfxDDraw::SetScnRect(const RECT *pRect)
{
	return S_OK;	// ignored

	RECT rZero ={0,0,0,0};
	if(pRect==0)
	{
		m_bScnClip = FALSE;
		return S_OK;
	}
	if((EqualRect(pRect,&rZero))||(EqualRect(pRect,&m_rectScn)&& m_bScnClip))
		return S_OK;
	m_rectScn =*pRect;
	m_bScnClip = TRUE;
	DP("[GFXDDRAW]SetScnRect (%d,%d,%d,%d)\n", m_rectScn.left, m_rectScn.top, m_rectScn.right, m_rectScn.bottom);
	return Update();
}

STDMETHODIMP CGfxDDraw::GetScnRect(RECT *pRect)
{
	if(!m_bOpen)
		return E_FAIL;

	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn ;
	return S_OK;
}

STDMETHODIMP CGfxDDraw::SetWindowHandle(HWND hwnd)
{
	if(!m_bOpen)
		return E_FAIL;

	if(m_hwnd!=hwnd)
	{
		m_hwnd = hwnd;
		if(m_pClipper)
			m_pClipper->SetHWnd(0,m_hwnd);
	}
	return S_OK;
}

STDMETHODIMP CGfxDDraw::GetWindowHandle(HWND& hwnd)
{
	if(!m_bOpen)
		return E_FAIL;

	hwnd = m_hwnd;
	return S_OK;
}

STDMETHODIMP CGfxDDraw::SetAlpha(DWORD dwAlpha)
{
	if(!m_bOpen)
		return E_FAIL;

#if defined(DDOVERLAYCAPS_ALPHACONSTANT)
 if(m_Caps.dwOverlayCaps &DDOVERLAYCAPS_ALPHACONSTANT)
 {
		m_bAlpha = TRUE;
		m_OFX.dwAlphaConst = dwAlpha*0xff/MAX_ALPHA_INTERNAL;
		m_OFX.dwAlphaConstBitDepth= 8;
		return Update();
 }
 else
#endif
  return E_NOTIMPL;

/*
#if defined(ALPHADEST)&& defined(DDOVER_ALPHADSTCONSTOVERRIDE)
	if(!m_bAlpha || dwAlpha!=m_OFX.dwAlphaDestConst)
	{
		m_bAlpha = TRUE;
		m_OFX.dwAlphaDestConst = dwAlpha;
		m_OFX.dwAlphaDestConstBitDepth = 8;
		return Update();
	}
#elif defined(DDOVER_ALPHASRCCONSTOVERRIDE)
	if(!m_bAlpha || dwAlpha!=m_OFX.dwAlphaSrcConst)
	{
		m_bAlpha = TRUE;
		m_OFX.dwAlphaSrcConst = dwAlpha;
		m_OFX.dwAlphaSrcConstBitDepth = 8;
		return Update();
	}
#endif
*/
	return S_OK;
}

STDMETHODIMP CGfxDDraw::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	if(!m_bOpen)
		return E_FAIL;

	if(m_bSrcCkey!=bEnable)
	{
		m_bSrcCkey = bEnable;
		if(bEnable)
			m_dwSrcCkey = dwSrcCkey;
		return Update();
	}
	if(bEnable && m_dwSrcCkey!=dwSrcCkey)
	{
		m_dwSrcCkey = dwSrcCkey;
		return Update();
	}
	return S_OK;
}

STDMETHODIMP CGfxDDraw::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	if(!m_bOpen)
		return E_FAIL;

	HRESULT hr;

	if(m_bDstCkey!=bEnable)
	{
		m_bDstCkey = bEnable;
		if(bEnable)
			m_dwDstCkey = dwDstCkey;
		hr = Update();
	}
	if(bEnable && m_dwDstCkey!=dwDstCkey)
	{
		m_dwDstCkey = dwDstCkey;
		m_bDstTrueCkeyValid = FALSE;
		hr = Update();
	}
#if (_WIN32_WCE != 0x600)	//ColorControl and Gamma Control isn't included in ddraw.lib.
	return m_pOverlay ? S_OK : E_FAIL;
#else
	return m_pOverlay && m_Caps.dwOverlayCaps &DDOVERLAYCAPS_CKEYDEST ? S_OK : E_FAIL;
#endif
}

STDMETHODIMP CGfxDDraw::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	if(!m_bOpen)
		return E_FAIL;

#if ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))	//ColorControl and Gamma Control isn't included in ddraw.lib.
	HRESULT hr;
	IDirectDrawSurface7 *pSurf;
	IDirectDrawColorControl *pColor;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	if(FAILED(pSurf->QueryInterface(IID_IDirectDrawColorControl,(void**)&pColor)))
		return E_FAIL;
	if(pCC == NULL)
		return E_FAIL;
	ZeroMemory(pCC, sizeof(DDCOLORCONTROL));
	pCC->dwSize = sizeof(DDCOLORCONTROL);
	hr = pColor->GetColorControls(pCC);
	pColor->Release();
	return hr;
#else
	return E_FAIL;
#endif
}

STDMETHODIMP CGfxDDraw::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	if(!m_bOpen)
		return E_FAIL;

#if ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))	//ColorControl and Gamma Control isn't included in ddraw.lib.	HRESULT hr;
	HRESULT hr;
	IDirectDrawSurface7 *pSurf;
	IDirectDrawGammaControl *pGamma;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	if(FAILED(pSurf->QueryInterface(IID_IDirectDrawGammaControl,(void**)&pGamma)))
		return E_FAIL;
	hr = pGamma->GetGammaRamp(0,pGR);
	pGamma->Release();
	return hr;
#else
	return E_FAIL;
#endif
}

STDMETHODIMP CGfxDDraw::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	if(!m_bOpen)
		return E_FAIL;

#if ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))	//ColorControl and Gamma Control isn't included in ddraw.lib.	HRESULT hr;
	HRESULT hr;
	IDirectDrawSurface7 *pSurf;
	IDirectDrawColorControl *pColor;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	if(FAILED(pSurf->QueryInterface(IID_IDirectDrawColorControl,(void**)&pColor)))
		return E_FAIL;
	hr = pColor->SetColorControls(pCC);
	pColor->Release();
	return hr;
#else
	return E_FAIL;
#endif
}

STDMETHODIMP CGfxDDraw::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	if(!m_bOpen)
		return E_FAIL;

#if ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))	//ColorControl and Gamma Control isn't included in ddraw.lib.	HRESULT hr;
	HRESULT hr;
	IDirectDrawSurface7 *pSurf;
	IDirectDrawGammaControl *pGamma;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	if(FAILED(pSurf->QueryInterface(IID_IDirectDrawGammaControl,(void**)&pGamma)))
		return E_FAIL;
	hr = pGamma->SetGammaRamp(0,pGR);
	pGamma->Release();
	return S_OK;
#else
	return E_FAIL;
#endif
}

IDirectDrawSurface7 *CGfxDDraw::GetSurface(LONG num)
{		
	if(num==GFXOVERLAY_SURF_PRIMARY ||
		num==0 && m_pOverlay==0 && m_dwBackBuffers==0)
		return m_pPrimary;
	if((num==GFXOVERLAY_SURF_OVERLAY)||(m_dwBackBuffers == 0))
		return m_pOverlay;
	if(num<(signed)m_dwBackBuffers)
		return m_pBackBuffer[num];
	return 0;
}

STDMETHODIMP CGfxDDraw::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(!m_bOpen)
		return E_FAIL;

	HRESULT hr;
	IDirectDrawSurface7 *pSurf;
	DDSURFACEDESC2 ddsd;
	DWORD dwFlags = 0;

	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
#ifdef DDLOCK_WAIT
	dwFlags |= DDLOCK_WAIT;
#endif

#ifdef TI3630
 dwFlags |= DDLOCK_WAITNOTBUSY;
#endif


again:
	hr = pSurf->Lock(0,&ddsd, dwFlags, 0);
if(hr == DDERR_WASSTILLDRAWING)
goto again;
if(hr==DDERR_SURFACELOST)/* Access to the surface is refused because the surface memory is gone.
	Call the IDirectDrawSurface::Restore method on this surface to restore the memory associated with it.*/
	{
		DWORD dwTick = GetTickCount();
		while(hr != S_OK)
		{
			if(FAILED(Restore()))
				return E_FAIL;

			return E_FAIL;
			#if 0	//Fix the issue that surface could be unavailable even if it can be locked successfully. by Golan
			hr = pSurf->Lock(0,&ddsd, dwFlags, 0);
			Sleep(50);
			if(( GetTickCount()- dwTick)>=SURFACELOST_RESTORE_TIMEOUT)
			{
				printf("Error!!!%s \n",GetErrorMsg(hr));
				break;
			}
			#endif
		}
	}
	if(FAILED(hr))
		return E_FAIL;
	if(ppBuf)
		*ppBuf = ddsd.lpSurface;
#if defined(SAMSUNG6443)|| defined(MTK3353_SW)|| defined(VNCVIEWER)
	if(pStride)
		*pStride = m_dwWidth;
#else
	if(pStride)
		*pStride = ddsd.lPitch;
#endif

	if(pPixCount)
		*pPixCount = ddsd.ddpfPixelFormat.dwRGBBitCount;

 
	return S_OK;
}

STDMETHODIMP CGfxDDraw::Unlock(LONG num)
{
	if(!m_bOpen)
		return E_FAIL;

	IDirectDrawSurface7 *pSurf;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	return pSurf->Unlock(0);
}


STDMETHODIMP CGfxDDraw::Blit(LONG num, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;

	IDirectDrawSurface7 *pSurf;
	HRESULT hr;
	DWORD dwFlags2;

	if(!m_bShow)	return S_OK;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
#ifdef _WIN32_WCE
	dwFlags2 = DDBLT_WAITVSYNC;
#else
	dwFlags2 = DDBLT_WAIT;
#endif
	hr = m_pPrimary->Blt(&m_rectBltDst,pSurf,&m_rectBltSrc,dwFlags2,&m_BFX);
	if(FAILED(hr))
		DP("hr:0x%x \n",hr);
	return hr;
}

STDMETHODIMP CGfxDDraw::Flip(LONG num, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;

	int i;
	IDirectDrawSurface7 *pSurf;
	HRESULT hr;
	DWORD dwDDFlags = 0;

	if(m_pOverlay == 0)
		return Blit(num,dwFlags);

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;

	#if 0
	if(pSurf)
		return pSurf->UpdateOverlay(&m_rectBltSrc, m_pPrimary,&m_rectBltDst, DDOVER_SHOW,&m_OFX);
	#endif
#ifdef _WIN32_WCE
//#ifndef SIRF_A4
//	dwDDFlags |= DDFLIP_WAITVSYNC;
//#endif
#else
	dwDDFlags |= DDFLIP_WAIT;
#endif
	if(dwFlags&GFXOVERLAY_FLIP_EVEN)
		dwDDFlags |= DDFLIP_EVEN;
	if(dwFlags&GFXOVERLAY_FLIP_ODD)
		dwDDFlags |= DDFLIP_ODD;

//#define DO_DELAY
#ifdef DO_DELAY
	int diff;
	DWORD dwTime;
#ifdef _WIN32_WCE
	dwTime = GetTickCount();
#else
	dwTime = timeGetTime();
#endif
	const int max = 8;
	if(m_dwLastFlipTime && max)
	{
		diff = dwTime-m_dwLastFlipTime;
		if(diff>=0 && diff<=max-3)
		{
			//OutputDebugString(TEXT("Sleep!\n"));
			Sleep(max-diff);
		}
	}
	m_dwLastFlipTime = dwTime;
#endif


	for(i=0;i<5;i++)
	{
		hr = m_pOverlay->Flip(pSurf,dwDDFlags);
		if(hr==DDERR_WASSTILLDRAWING)
		{
			Sleep(5);
			continue;
		}
		break;
	}
	if(DDERR_OUTOFCAPS == hr)
	{
		//OutputDebugString(TEXT("Flip DDERR_OUTOFCAPS \n"));
		if(FAILED(Restore()))
			return E_FAIL;
	}
	else if(hr != S_OK)
	{
		DP("GfxDDRaw Flip failed. Flip (%d,%d) ret:0x%x \n", pSurf, dwDDFlags,hr);
	}

	if(hr == S_OK)
		m_frameCount++;
	return hr;
}

STDMETHODIMP CGfxDDraw::Show(BOOL bShow)
{
	if(!m_bOpen)
		return E_FAIL;

	if(m_bShow==bShow)
		return S_OK;

#ifdef MTK3353_HW
	if(m_frameCount == 0 && bShow)
		return S_OK;
#endif

	m_bShow = bShow;

	HRESULT hr = Update();
	if(FAILED(hr))
		return hr;

#ifdef MT3353
	if(m_bShow)
		hr = Flip(0,0);
#endif
	return hr;
}

STDMETHODIMP CGfxDDraw::FindTrueDstCkey()
{
	HDC hdc;
	DWORD dwPixel,dwPixCount, dwReturnPixel, dwCkeyTest, dwCkeyOld;
	int i;
	LPBYTE pBuf;

	if(m_pPrimary==0)
		return E_FAIL;
	for(i=0;i<4;i++)
	{	// must read the colorkey a couple of times since the window can be busy
		if(FAILED(m_pPrimary->GetDC(&hdc)))
			return E_FAIL;
		dwPixel = GetPixel(hdc,0,0);
		dwReturnPixel = SetPixel(hdc,0,0,m_dwDstCkey);
		m_pPrimary->ReleaseDC(hdc);
		if(dwReturnPixel==1)	// failure value
			return E_FAIL;
		if(FAILED(Lock(GFXOVERLAY_SURF_PRIMARY,(LPVOID *)&pBuf,0,&dwPixCount)))
			return E_FAIL;
		dwCkeyTest =*(DWORD *)pBuf;
		dwCkeyTest &= dwPixCount<32 ?(1<<dwPixCount)-1 :-1;
		Unlock(GFXOVERLAY_SURF_PRIMARY);
		if(i==0)
		{
			dwCkeyOld = dwCkeyTest;
			continue;
		}
		if(dwCkeyOld==dwCkeyTest)
			break;
	}
	if(i==4)
		return E_FAIL;
	if(FAILED(m_pPrimary->GetDC(&hdc)))
		return E_FAIL;
	SetPixel(hdc,0,0,dwPixel);
	m_pPrimary->ReleaseDC(hdc);
	m_dwDstTrueCkey = dwCkeyTest;
	m_bDstTrueCkeyValid = TRUE;
	return S_OK;
}

STDMETHODIMP CGfxDDraw::Update()
{
	DWORD dwFlags;
	DDCOLORKEY ddck;
	RECT rs, rd;

	if((m_rectDst.left == 0)&&(m_rectDst.right == 0)&&(m_rectDst.top == 0)&&(m_rectDst.bottom == 0))
		dwFlags = DDOVER_HIDE;
	else
		dwFlags = m_bShow ? DDOVER_SHOW : DDOVER_HIDE;

	if(m_pPrimary==0)
		return E_FAIL;
	if(m_bDstCkey)
	{
	#if ((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501))
		if(m_pOverlay &&(m_Caps.dwOverlayCaps&DDOVERLAYCAPS_CKEYDEST))	//Overlay surface & support dest color key
	#endif
		{
			if(!m_bDstTrueCkeyValid)
				FindTrueDstCkey();
			ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue = m_dwDstTrueCkey;
			if(FAILED(m_pPrimary->SetColorKey(DDCKEY_DESTOVERLAY,&ddck)))
				OutputDebugStringW(L"Destination Color Key FAILED!\n");
			dwFlags |= DDOVER_KEYDEST;
		}
	}
	if(m_bSrcCkey)
	{
		ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue = m_dwSrcCkey;
		HRESULT hr =(m_pOverlay ? m_pOverlay : m_pPrimary)->SetColorKey(DDCKEY_SRCOVERLAY,&ddck); 
		if(FAILED(hr))
			OutputDebugStringW(L"Source Color Key FAILED!\n");
		else
			dwFlags |= DDOVER_KEYSRC;
	}
#ifdef WINCE
	if(m_bAlpha)
	{
		dwFlags |= DDOVER_ALPHACONSTOVERRIDE;
/*
#if defined(ALPHADEST)&& defined(DDOVER_ALPHADSTCONSTOVERRIDE)
		dwFlags |= DDOVER_ALPHASRCCONSTOVERRIDE;
#elif defined(DDOVER_ALPHASRCCONSTOVERRIDE)
		dwFlags |= DDOVER_ALPHASRCCONSTOVERRIDE;
#endif
*/
	}
#endif
#ifndef _WIN32_WCE
	//dwFlags |= DDOVER_BOB | DDOVER_INTERLEAVED;
#endif
	SetRect(&rs,0,0,m_dwWidth,m_dwHeight);
	IntersectRect(&rs,&rs,&m_rectSrc);
	rd = m_rectDst;
#ifdef FREESCALE_IMX31
	//Adjust the height to be 1:1 or 2:1 if only 12% off.

	int src_width, src_height, dst_width, dst_height;
	src_width = rs.right - rs.left;
	src_height = rs.bottom - rs.top;
	dst_width = rd.right - rd.left;
	dst_height = rd.bottom - rd.top;
	if(src_height>dst_height && src_height<=dst_height+(dst_height>>3))	
		 rs.bottom = rs.top + dst_height;
	else
	{
		src_height>>=1;
		if(src_height>dst_height && src_height<=dst_height+(dst_height>>3))	
			 rs.bottom = rs.top +(dst_height<<1);
	}
#endif
	if(m_bScnClip)
	{
		IntersectRect(&rd,&rd,&m_rectScn);
		if(!EqualRect(&rd,&m_rectDst))
		{				// translate the clipped destination rectangle to clipping for source rectangle
			RECT rn;	// normalization 
			int wd,ws,hd,hs;

			rn = rd;
			OffsetRect(&rn,-m_rectDst.left,-m_rectDst.top);
			wd = m_rectDst.right-m_rectDst.left;
			hd = m_rectDst.bottom-m_rectDst.top;
			ws = rs.right-rs.left;
			hs = rs.bottom-rs.top;
			if(wd<=0 || hd<=0)
				SetRect(&rs,0,0,0,0);
			else
			{
				rn.left = rn.left*ws/wd;
				rn.right = rn.right*ws/wd;
				rn.top = rn.top*hs/hd;
				rn.bottom = rn.bottom*hs/hd;
				OffsetRect(&rn,rs.left,rs.top);
				IntersectRect(&rs,&rs,&rn);
			}
		}
	}
	OffsetRect(&rd,-m_rectScn.left,-m_rectScn.top);

	// handle alignment
	if(m_Caps.dwAlignBoundarySrc>1)
	{
		rs.left &=~(m_Caps.dwAlignBoundarySrc-1);
	}

	if(m_Caps.dwAlignSizeSrc>1)
	{
		DWORD dwSrcWid = rs.right - rs.left;
		dwSrcWid &=~(m_Caps.dwAlignSizeSrc-1);
		rs.right =rs.left + dwSrcWid;
	}

	rd.left &=~(16-1);
	rd.top &=~(16-1);
	rd.right &=~(16-1);
	rd.bottom &=~(16-1);




	if(m_Caps.dwAlignBoundaryDest>1)
	{
		rd.left &=~(m_Caps.dwAlignBoundaryDest-1);
	}

	if(m_Caps.dwAlignSizeDest>1)
	{
		DWORD dwDstWid = rd.right - rd.left;
		dwDstWid &=~(m_Caps.dwAlignSizeDest-1);
		rd.right = rd.left + dwDstWid;
	}

	m_rectBltSrc = rs;
	m_rectBltDst = rd;
	if(m_bLastShow==FALSE && m_bShow==FALSE)
		return S_OK;
	if(m_pOverlay)
	{
		DP("[GFXDDRAW] UpdateOverlay [%d,%d,%d,%d]->[%d,%d,%d,%d]\n", rs.left, rs.right, rs.top, rs.bottom, rd.left, rd.right, rd.top, rd.bottom);

#if 0// (_WIN32_WCE == 0x600)
	  if (m_Caps.dwOverlayCaps & DDOVERLAYCAPS_CKEYSRC)
		{
		  dwFlags |= DDOVER_KEYSRCOVERRIDE;

			// Create an overlay FX structure so we can specify a source color key.
			// This information is ignored if the DDOVER_SRCKEYOVERRIDE flag 
			// isn't set.
		  m_OFX.dckSrcColorkey.dwColorSpaceLowValue=0;// black as the color key
		  m_OFX.dckSrcColorkey.dwColorSpaceHighValue=0;
		}
#endif

#ifdef FREESCALE_IMX31_FLICKER
	if((dwFlags&DDOVER_HIDE)&&((m_dwWidth < 640)&&(m_dwHeight< 480)))
	{
		SetRect(&rd, 0 ,0 ,64, 1);
		dwFlags &=~DDOVER_HIDE;
		dwFlags |= DDOVER_SHOW;
		HRESULT hr = m_pOverlay->UpdateOverlay(&rs, m_pPrimary,&rd, dwFlags,&m_OFX);
		if(SUCCEEDED(hr))
			return hr;
		DP("Freescale doesn't support scale to 64x1\n");
		dwFlags &=~DDOVER_SHOW;
		dwFlags |= DDOVER_HIDE;
	}
#endif

		if(dwFlags&DDOVER_HIDE)
		{
			DP("[GFXDDRAW] UpdateOverlay--- Hide Overlay\n");
			if(FAILED(m_pOverlay->UpdateOverlay(&rs, m_pPrimary, NULL, dwFlags,&m_OFX)))
				return E_FAIL;
		}
		else
		{
			DP("[GFXDDRAW] UpdateOverlay--- Show Overlay\n");

			#ifdef KOVAN_MX31
			if(m_Caps.dwAlignSizeDest>1)
			{
				rd.right &=~(m_Caps.dwAlignSizeDest-1);
				rd.left &=~(m_Caps.dwAlignSizeDest-1);
			}
		
			if((rd.right - rd.left > 720)||(rd.bottom - rd.top >421))
			{
				rd.left =(800 - 720)/2;
				rd.top =(480 - 421)/2;
				rd.right = rd.left + 720;
				rd.bottom = rd.top + 421;
			}
			#endif

#if defined(NO_ROT_SCL)
			rs.left = rs.top = 0;
			rs.right = m_dwWidth;
			rs.bottom = m_dwHeight;
#endif


			HRESULT hr = m_pOverlay->UpdateOverlay(&rs, m_pPrimary,&rd, dwFlags,&m_OFX);
			if(FAILED(hr))
			{
				DP("DDRAW ERRORCODE :0x%x \n",hr);			
				return E_FAIL;
			}
		}
	}
	m_bLastShow = m_bShow;
	return S_OK;
}

STDMETHODIMP CGfxDDraw::ReleaseSurfaces()
{
	if(m_pBackBuffer)
	{
		int i;
		for(i=0;i<(signed)m_dwBackBuffers;i++)
		{
			m_pBackBuffer[i]->Release();
		}

		delete[] m_pBackBuffer;
		m_pBackBuffer = 0;
		m_dwBackBuffers = 0;
	}

	if(m_pOverlay)
	{
		m_pOverlay->Release();
		m_pOverlay = 0;
	}
	if(m_pClipper)
	{
		if(m_pPrimary)
			m_pPrimary->SetClipper(0);
		m_pClipper->Release();
		m_pClipper = 0;
	}


	m_bLastShow = FALSE;
	m_bShow = FALSE;
	m_bDstTrueCkeyValid = FALSE;
	return S_OK;
}


STDMETHODIMP CGfxDDraw::Restore()
{
	HRESULT hr = E_FAIL;

	//Try to call RestoreAllSurfaces() which supported by IDirectDraw4 .
	if(m_pDDraw)
	{
		hr = m_pDDraw->RestoreAllSurfaces();
	}
	else
		hr = E_FAIL;

	if(FAILED(hr))	//Do the restore() by ourselves
	{
		//Restore surface one by one
		if(m_pOverlay)
		{
			hr = m_pOverlay->Restore();
			if(FAILED(hr))
				return E_FAIL;
		}
		if(m_pPrimary)
		{
			hr = m_pPrimary->Restore();
			if(FAILED(hr))
				return E_FAIL;
		}
		if(m_pBackBuffer)
		{	// only implicit surfaces need restoration
			int i;

			for(i=0;i<(signed)m_dwBackBuffers;i++)
			{
				hr = m_pBackBuffer[i]->Restore();
				if(FAILED(hr))
					return E_FAIL;
			}
		}
		if(m_pClipper)
		{
			if(m_pPrimary)
				hr = m_pPrimary->SetClipper(m_pClipper);
		}
	}
	m_bLastShow = FALSE;
	m_bShow = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxDDraw::SetPixelFormat(DDPIXELFORMAT *pPF, DWORD dwFourCC)
{
	int i;

	ZeroMemory(pPF,sizeof(*pPF));
	pPF->dwSize = sizeof(*pPF);
	switch(dwFourCC)
	{
	case MAKEFOURCC('A','R','3','2'):
		pPF->dwFlags = DDPF_RGB | DDPF_ALPHAPIXELS;
		pPF->dwRGBBitCount = 32;
		pPF->dwRBitMask = 0xff0000;
		pPF->dwGBitMask = 0x00ff00;
		pPF->dwBBitMask = 0x0000ff;
		pPF->dwRGBAlphaBitMask = 0xff000000;
		return S_OK;
	case MAKEFOURCC('X','R','3','2'):
		pPF->dwFlags = DDPF_RGB;
		pPF->dwRGBBitCount = 32;
		pPF->dwRBitMask = 0xff0000;
		pPF->dwGBitMask = 0x00ff00;
		pPF->dwBBitMask = 0x0000ff;
		return S_OK;
	case MAKEFOURCC('R','2','4',' '):
		pPF->dwFlags = DDPF_RGB;
		pPF->dwRGBBitCount = 24;
		pPF->dwRBitMask = 0xff0000;
		pPF->dwGBitMask = 0x00ff00;
		pPF->dwBBitMask = 0x0000ff;
		return S_OK;
	case MAKEFOURCC('R','5','6','5'):
		pPF->dwFlags = DDPF_RGB;
		pPF->dwRGBBitCount = 16;
		pPF->dwRBitMask = 0xf800;
		pPF->dwGBitMask = 0x07e0;
		pPF->dwBBitMask = 0x001f;
		return S_OK;
	case MAKEFOURCC('Y','U','Y','2'):
	case MAKEFOURCC('Y','U','Y','V'):
	case MAKEFOURCC('Y','V','1','2'):
	case MAKEFOURCC('M','T','K','1'):
		pPF->dwFlags = DDPF_FOURCC;
		pPF->dwFourCC = dwFourCC;
		return S_OK;

	default:
		for(i=0;i<(signed)m_dwNumFourCCCodes;i++)
		{
			if(m_pdwFourCCCodes[i]==dwFourCC)
			{
				pPF->dwFlags = DDPF_FOURCC;
				pPF->dwFourCC = dwFourCC;
				return S_OK;
			}
		}
	}
	return E_FAIL;
}

STDMETHODIMP CGfxDDraw::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	if(pSrc==0 || pDst==0 || pdwFlags==0)
		return E_FAIL;
#ifndef NO_ROT_SCL
	if(m_dwFourCC==MAKEFOURCC('P','5','6','5'))
#endif	
	{
		*pdwFlags = GFXOVERLAY_SCALE_X | GFXOVERLAY_SCALE_Y;
		*pSrc = m_rectSrc;
#if defined(NO_ROT_SCL)
		RECT dst;
		dst.right = m_rectDst.right - m_rectDst.left;
		dst.left = 0;
		dst.bottom = m_rectDst.bottom - m_rectDst.top;
		dst.top= 0;
		*pDst = dst;
#else
		*pDst = m_rectDst;
#endif
		return S_OK;
	}


	return E_FAIL;
}

STDMETHODIMP CGfxDDraw::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	if(m_dwFourCC==MAKEFOURCC('P','5','6','5'))
		return S_OK;
	return E_FAIL;
}
STDMETHODIMP CGfxDDraw::GetDDCaps(LONG *pCaps)
{
	if(pCaps==0)
		return E_POINTER;
	//*pCaps = GFXOVERLAY_CAPS_HIDE_NOT_SUPPORT;
	return S_OK;
}

STDMETHODIMP CGfxDDraw::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	if(pDevice==0)
		return E_POINTER;
	*(DDDEVICEIDENTIFIER2 *)pDevice = m_Device;
	if(pdwSize)
		*pdwSize = sizeof(m_Device);
	return S_OK;
}

STDMETHODIMP CGfxDDraw::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(!m_bOpen)
		return E_FAIL;

	if(m_pPrimary==0 || pInfo==0)
		return E_FAIL;
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
	pInfo->dwFourCC = m_dwFourCC;
	pInfo->dwCaps = 0 
	#if ((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501))
		|(GFXOVERLAY_CAP_FLIP_BOB)
	#else
		|(m_Caps.dwCaps2&DDCAPS2_CANBOBINTERLEAVED &&
			m_Caps.dwCaps2&DDCAPS2_CANFLIPODDEVEN ? GFXOVERLAY_CAP_FLIP_BOB : 0)
	#endif
#ifdef DDFXCAPS_OVERLAYDEINTERLACE
		|(m_Caps.dwFXCaps&DDFXCAPS_OVERLAYDEINTERLACE ? GFXOVERLAY_CAP_FLIP_DEINTERLACE : 0)
#endif
		;
	return S_OK;
}

STDMETHODIMP CGfxDDraw::IsOpen()
{
	return m_pOverlay ? S_OK : S_FALSE;
}

HRESULT WINAPI CGfxDDraw::EnumSurfaceCallback(LPDIRECTDRAWSURFACE7 pSurf, LPDDSURFACEDESC2 pDesc, LPVOID lpContext)
{
	CGfxDDraw *pMe;

	pMe = reinterpret_cast<CGfxDDraw *>(lpContext);
	pMe->m_pBackBuffer[pMe->m_dwBackBuffers++]= pSurf;
	return DDENUMRET_OK;
}


BOOL WINAPI CGfxDDraw::EnumMonitorCallbackEx(GUID FAR* lpGUID,
											 LPTSTR lpDriverDescription, LPTSTR lpDriverName,
											LPVOID lpContext, HMONITOR hm)
{
	CGfxDDraw *pMe;
	GfxMonitorInfo *pMon;

	pMe = reinterpret_cast<CGfxDDraw *>(lpContext);

	if(pMe->m_dwNumMonitors >=MONITORS_AVAIL_NUM)
		return FALSE;
	pMon =&pMe->m_aMonitors[pMe->m_dwNumMonitors++];
	if(lpGUID)
		pMon->guid =*lpGUID;
	else
		ZeroMemory(&pMon->guid,sizeof(pMon->guid));
	pMon->hm = hm;
	ZeroMemory(&pMon->info,sizeof(pMon->info));
	pMon->info.cbSize = sizeof(pMon->info);
	GetMonitorInfo(hm,&pMon->info);
	return (pMe->m_dwNumMonitors<MONITORS_AVAIL_NUM);
}


BOOL WINAPI CGfxDDraw::EnumMonitorCallback(GUID FAR* lpGUID,
											 LPTSTR lpDriverDescription, LPTSTR lpDriverName,
											LPVOID lpContext)
{
	CGfxDDraw *pMe;
	GfxMonitorInfo *pMon;

	pMe = reinterpret_cast<CGfxDDraw *>(lpContext);
	pMon =&pMe->m_aMonitors[pMe->m_dwNumMonitors++];
	if(lpGUID)
		pMon->guid =*lpGUID;
	pMon->hm = 0;
	return (pMe->m_dwNumMonitors<MONITORS_AVAIL_NUM);
}

char *CGfxDDraw::GetErrorMsg(HRESULT DDRVAL)
{
  struct DDErrorMessage
	{
	  HRESULT Code;
	  char *EngMsg;
	};
  static DDErrorMessage LookupTable[]=
	{
		{ DDERR_CURRENTLYNOTAVAIL,"CurrentlyNotAvail"},
		{ DDERR_GENERIC,"Generic"},
		{ DDERR_HEIGHTALIGN,"HeightAlign"},
		{ DDERR_INCOMPATIBLEPRIMARY,"IncompatiblePrimary"},
		{ DDERR_INVALIDCAPS,"InvalidCaps"},
		{ DDERR_INVALIDCLIPLIST,"InvalidClipList"},
		{ DDERR_INVALIDMODE,"InvalidMode"},
		{ DDERR_INVALIDOBJECT,"InvalidObject"},
		{ DDERR_INVALIDPARAMS,"InvalidParams"},
		{ DDERR_INVALIDPIXELFORMAT,"InvalidPixelFormat"},
		{ DDERR_INVALIDRECT,"InvalidRect"},
		{ DDERR_LOCKEDSURFACES,"LockedSurfaces"},
		{ DDERR_NOALPHAHW,"NoAlphaHardware"},
		 { DDERR_NOCLIPLIST,"NoClipList"},
		{ DDERR_NOCOLORCONVHW,"NoColorConversion"},
		{ DDERR_NOCOOPERATIVELEVELSET,"NoCooperativeLevelSet"},
		{ DDERR_NOCOLORKEYHW,"NoColorkeyHardware"},
		{ DDERR_NOFLIPHW,"NoFlipHardware"},
		{ DDERR_NOTFOUND,"ItemNotFind"},
		{ DDERR_NOOVERLAYHW,"NoOverlayHardware"},
		{ DDERR_OVERLAPPINGRECTS,"OverlappingRects"},
		{ DDERR_NORASTEROPHW,"NoRasterOPHardware"},
		{ DDERR_NOSTRETCHHW,"NoStretchHardware"},
		{ DDERR_NOVSYNCHW,"NoVsyncHardware"},
		{ DDERR_NOZOVERLAYHW,"NoZOverlayHardware"},
		{ DDERR_OUTOFCAPS,"OutofCaps"},
		{ DDERR_OUTOFMEMORY,"OutofMemory"},
		{ DDERR_OUTOFVIDEOMEMORY,"OutofVideoMemory"},
		{ DDERR_PALETTEBUSY,"PaletteBusy"},
		{ DDERR_COLORKEYNOTSET,"ColorkeyNotSet"},
		{ DDERR_SURFACEBUSY,"SurfaceBusy"},
		{ DDERR_NOBLTHW,"NoBltHardware"},
		{ DDERR_SURFACELOST,"SurfaceLost"},
		{ DDERR_UNSUPPORTED,"UnSupported"},
		{ DDERR_CANTLOCKSURFACE,"CantLockSurface"},
		{ DDERR_WASSTILLDRAWING,"WasStillDrawing"},
		{ DD_OK,"OK"},
		{ 0, 0},

	#if ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))
	{ DDERR_NO3D,"No3D"},
	{ DDERR_NOCOLORKEY,"NoColorkey"},
	{ DDERR_NODIRECTDRAWSUPPORT,"NoDirectDrawSupport"},
	{ DDERR_NOEXCLUSIVEMODE,"NoExclusiveMode"},
	{ DDERR_NOGDI,"NoGDI"},
	{ DDERR_NOMIRRORHW,"NoMirrorHardware"},
	{ DDERR_NOROTATIONHW,"NoRotationHardware"},
	{ DDERR_NOT4BITCOLOR,"No4BitColor"},
	{ DDERR_NOT4BITCOLORINDEX,"No4BitColorIndex"},
	{ DDERR_NOT8BITCOLOR,"No8BitColor"},
	{ DDERR_NOTEXTUREHW,"NoTextureMappingHardware"},
	{ DDERR_NOZBUFFERHW,"NoZBufferHardware"},
	{ DDERR_OVERLAYCANTCLIP,"OverlayCantClip"},
	{ DDERR_OVERLAYCOLORKEYONLYONEACTIVE,"OverlayColorkeyOnlyOneActive"},
	{ DDERR_SURFACEALREADYATTACHED,"SurfaceAlreadyAttached"},
	{ DDERR_SURFACEALREADYDEPENDENT,"SurfaceAlreadyDependent"},
	{ DDERR_ALREADYINITIALIZED,"AlreadyInitialized"},
	{ DDERR_CANNOTATTACHSURFACE,"CannotAttachSurface"},
	{ DDERR_CANNOTDETACHSURFACE,"CannotDetachSurface"},
	{ DDERR_EXCEPTION,"Exception"},
	#endif
	};
  char *msg ="NotFind";

  for (int i=0;LookupTable[i].EngMsg!=NULL;i++)
	  if (LookupTable[i].Code == DDRVAL)
		  return LookupTable[i].EngMsg;
  return msg;
}

IMcGfxOverlay *CreateOverlayDDraw(DWORD surface_type)
{
	return new CGfxDDraw(surface_type);
}
#endif
