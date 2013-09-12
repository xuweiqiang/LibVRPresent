//=============================================================================
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

// Things to do: (achung)
// 1) Check directdraw surface lost through resolution change - done
// 2) Check multimon support (multiple monitors) - done
// 3) Check overlay failure (creation) - done
// 4) Implement interlacing support
#include "../../inc/GfxBuildDef.h"

#ifdef GFX_DDRAW_OSD
#include <stdio.h>
#include <windows.h>
#include <ddraw.h>
#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"

#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxguid.lib")

#ifdef HUAYANG_A6
#include <WinIoCtl.h>
#define IOCTL_ENTER_SELECT_OVERLAY    CTL_CODE( FILE_DEVICE_VIDEO, 20, METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#endif	


#if  ((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501))
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


class CGfxDDrawOSD : public IMcGfxOverlay, public CMcUnknown
{
#define MONITORS_AVAIL_NUM	9
#define DISPMODE_NUM	36
public:
	DECLARE_IUNKNOWN

	CGfxDDrawOSD();
	virtual ~CGfxDDrawOSD();
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
    STDMETHODIMP SetHWRenderSurf(LPVOID pSurf,int mode )
	{
		return E_FAIL;
	}
	STDMETHODIMP SetDeinterlaceMode(DWORD dwMode)
	{
		return E_NOTIMPL;
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
	BOOL				m_bBlit;
	DWORD				m_dwDstCkey, m_dwSrcCkey, m_dwDstTrueCkey;
	DWORD				m_dwNumFourCCCodes;
	DWORD				*m_pdwFourCCCodes;
#ifdef PRIMAII
	static IDirectDraw7		*m_pDDraw;
#else
	IDirectDraw7		*m_pDDraw;
#endif
	IDirectDrawSurface7 *m_pPrimary, *m_pOverlay, **m_pBackBuffer;
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
	BOOL m_bOpen; 	
};


#ifdef PRIMAII
IDirectDraw7	* CGfxDDrawOSD::m_pDDraw = 0;
#endif

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('R','5','6','5'),
	0
};

CGfxDDrawOSD::CGfxDDrawOSD() : CMcUnknown(0,0)
{
	m_pDDraw = 0;
	m_pPrimary = 0;
	m_pOverlay = 0;
	m_pBackBuffer = 0;
	m_dwWidth = m_dwHeight = 0;
	m_dwBackBuffers = 0;
	m_bLastShow = m_bDstCkey = m_bSrcCkey = m_bScnClip = m_bDstTrueCkeyValid = m_bAlpha = m_bBlit = FALSE;
	m_bShow = false;
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
	m_bOpen = FALSE;
}

CGfxDDrawOSD::~CGfxDDrawOSD()
{
}

HRESULT WINAPI CGfxDDrawOSD::EnumModesCallback(
  LPDDSURFACEDESC2 lpDDSurfaceDesc, 
  LPVOID lpContext
)
{

	CGfxDDrawOSD *pMe;
	DDSURFACEDESC2 *pDisp;

	pMe = reinterpret_cast<CGfxDDrawOSD *>(lpContext);
	pDisp = &pMe->m_aDisps[pMe->m_dwNumDisps++];
	*pDisp = *lpDDSurfaceDesc;
	return ((pMe->m_dwNumDisps<DISPMODE_NUM)?S_OK:E_FAIL);
}

STDMETHODIMP CGfxDDrawOSD::EnumerateDispModes()
{
	if(m_pDDraw ==NULL)
		return E_FAIL;

	return m_pDDraw->EnumDisplayModes(NULL,NULL,this,EnumModesCallback);
}


STDMETHODIMP CGfxDDrawOSD::EnumerateMonitors()
{
	HRESULT hr = E_FAIL;
	HINSTANCE hlib;
	LPDIRECTDRAWENUMERATEEX pDirectDrawEnumerateEx;

	hlib = LoadLibrary(TEXT("ddraw.dll"));
	if(hlib==0)
		return E_FAIL;

	m_dwNumMonitors = 0;
#ifdef _WIN32_WCE
	#if  ((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501))
	    pDirectDrawEnumerateEx = (LPDIRECTDRAWENUMERATEEX )GetProcAddress(hlib,L"DirectDrawEnumerateEx");
	#else
	    pDirectDrawEnumerateEx = (LPDIRECTDRAWENUMERATEEX )GetProcAddress(hlib,L"DirectDrawEnumerateExW");
	#endif
#else
    pDirectDrawEnumerateEx = (LPDIRECTDRAWENUMERATEEX )GetProcAddress(hlib,"DirectDrawEnumerateExA");
#endif
    if(pDirectDrawEnumerateEx)
    {
        hr = pDirectDrawEnumerateEx(EnumMonitorCallbackEx, this,
                                DDENUM_ATTACHEDSECONDARYDEVICES |
                                DDENUM_DETACHEDSECONDARYDEVICES
		#if  ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))
                                |DDENUM_NONDISPLAYDEVICES
		#endif
		);
    }
#if  ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))
	else
		hr = DirectDrawEnumerate(EnumMonitorCallback,this);
#endif
	FreeLibrary(hlib);
	return SUCCEEDED(hr) ? S_OK : E_FAIL;
}

GfxMonitorInfo *CGfxDDrawOSD::GetBestMonitor(POINT *pt)
{
	int i;

	for(i=0;i<(signed)m_dwNumMonitors;i++)
	{
		if(PtInRect(&m_aMonitors[i].info.rcMonitor,*pt))
			return &m_aMonitors[i];
	}
	return 0;
}

STDMETHODIMP CGfxDDrawOSD::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	POINT pt;

	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;
	if(m_bOpen)
	{
		DP("[GFXDDRAW_OSD]Invalid call \n");
		return E_FAIL;
	}

	m_bSrcCkey = false;
	m_hwnd = 0;
	
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	
	if(pdwFourCC)
		m_pdwFourCCList = pdwFourCC;
	else
		m_pdwFourCCList = const_dwFourCC;

	#if defined(MT3351_DBG) || defined(MT3353) || defined(PRIMAII)
	m_pdwFourCCList = const_dwFourCC;
	#endif

	DP("[GFXDDRAW_OSD]Open w:%d h:%d \n",m_dwWidth, m_dwHeight);
	SetRect(&m_rectDst,0,0,m_dwWidth,m_dwHeight);

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

	if(m_pDDraw == 0)
	{
#ifdef _WIN32_WCE
	if(FAILED(DirectDrawCreate(0,(LPDIRECTDRAW*)&m_pDDraw,0)))
		return E_FAIL;
#else
	if(FAILED(DirectDrawCreateEx(m_pMonitorInfo?&m_pMonitorInfo->guid:0,(void**)&m_pDDraw,IID_IDirectDraw7,0)))
		return E_FAIL;
#endif
	}
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
			DP("[GFXDDRAW_OSD] Not support size(%d , %d )\n",m_dwWidth, m_dwHeight );
			m_pDDraw->Release();
			m_pDDraw = 0;
			return E_FAIL;
		}
	}
#endif

#if  (!((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501)))
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
#ifndef MT3353
	m_pDDraw->GetDeviceIdentifier(&m_Device,0);
#endif
#endif
	dwBuffers = 1;

#ifdef HUAYANG_A6
	WORD CBNIN[1];
	CBNIN[0]=2; 
	HANDLE HCBN = CreateFile(_T("CBN1:"),GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL );
	DeviceIoControl(
	HCBN,
	IOCTL_ENTER_SELECT_OVERLAY,		(LPVOID)CBNIN, sizeof(BYTE),
	NULL,		NULL,		NULL,		NULL);
#endif	

	if(SUCCEEDED(CreateSurfaces(dwBuffers)))
	{

		LPBYTE pBuf;
		LONG lStride;

		if(SUCCEEDED(Lock(0,(LPVOID*)&pBuf,&lStride,0)))
		{
			if(m_dwFourCC == MAKEFOURCC('R','5','6','5'))
				memset(pBuf, 0 , lStride*m_dwHeight);
			Unlock(0);
			Flip(0,0);
		}
	
		SetRect(&m_rectSrc, 0,0,m_dwWidth, m_dwHeight);
		m_bOpen = TRUE;
		return S_OK;
	}


	ReleaseSurfaces();
	m_pDDraw->Release();
	m_pDDraw = 0;
	return E_FAIL;
}

STDMETHODIMP CGfxDDrawOSD::Close()
{
	if(!m_bOpen)
	{
		DP("[GFXDDRAW_OSD]Invalid call \n");
		return E_FAIL;
	}

	m_bShow = FALSE;
	Update();
	ReleaseSurfaces();

#ifndef PRIMAII
	if(m_pDDraw)
	{
		m_pDDraw->Release();
		m_pDDraw = 0;
	}
#endif
	if(m_pdwFourCCCodes)
	{
		delete[] m_pdwFourCCCodes;
		m_pdwFourCCCodes = 0;
	}
	m_dwNumMonitors = 0;
	m_dwNumDisps = 0;

	m_bOpen = FALSE;
	DP("[GFXDDRAW_OSD]Close \n");
	return S_OK;
}

STDMETHODIMP CGfxDDrawOSD::CreateSurfaces(DWORD dwBuffers)
{
	int i,cc;
	IDirectDrawSurface7 *pSurf;
	DDSURFACEDESC2 ddsd;

	if(m_dwWidth==0 || m_dwHeight==0 || m_pdwFourCCList==0 || m_pOverlay)
		return E_FAIL;
	if(FAILED(m_pDDraw->SetCooperativeLevel(0,DDSCL_NORMAL)))
		return E_FAIL;
	/* create primary */
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	if(FAILED(m_pDDraw->CreateSurface(&ddsd,&m_pPrimary,0)))
	{
		DP("GFXDDRAW_OSD ERROR !!! \n", __LINE__);
		return E_FAIL;
	}

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
	ddsd.ddsCaps.dwCaps |= DDSCAPS_FLIP  ;
	#ifdef DDSCAPS_COMPLEX
	ddsd.ddsCaps.dwCaps |= DDSCAPS_COMPLEX;
	#endif

	if(dwBuffers)
		ddsd.dwFlags |= DDSD_BACKBUFFERCOUNT;

	ddsd.dwWidth = m_dwWidth;
	ddsd.dwHeight = m_dwHeight;

	//Create overlay surface
	{
#if  ((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501))
		if (m_Caps.dwOverlayCaps != 0)
#else
		if(m_Caps.dwCaps&DDCAPS_OVERLAY)
#endif
		{	// attempt to create overlay surface
			for(cc=0;m_pdwFourCCList[cc];cc++)
			{
				HRESULT hr = E_PENDING;
				if(FAILED(SetPixelFormat(&ddsd.ddpfPixelFormat,m_pdwFourCCList[cc])))
					continue;
				m_dwFourCC = m_pdwFourCCList[cc];
				for(i=dwBuffers;i>=0;i--)
				{
					ddsd.dwBackBufferCount = i;
					hr = m_pDDraw->CreateSurface(&ddsd,&m_pOverlay,0);
					if(SUCCEEDED(hr))
						break;
				}
				if(i<0)
				{
					DP("[FOUCC]: 0x%x 0x%x \n", m_pdwFourCCList[cc],hr);
					DP("GFXDDRAW_OSD ERROR !!! \n", __LINE__);
					continue;
				}
				DP("[GFXDDRAW_OSD]Create [0x%x]surface success \n",m_pdwFourCCList[cc]);

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
				DP("[GFXDDRAW_OSD]Overlay_Backbuf surface is created sucessfully \n");
				return S_OK;
			}
		}
		m_pOverlay = 0;
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

				pPF = &m_PrimaryDesc.ddpfPixelFormat;
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

STDMETHODIMP CGfxDDrawOSD::SetSrcRect(const RECT *pRect)
{
	return S_OK;
}

STDMETHODIMP CGfxDDrawOSD::SetDstRect(const RECT *pRect)
{
	POINT pt;
	GfxMonitorInfo *pinfo;


	if(pRect == 0)	return E_INVALIDARG;


	((RECT *)pRect)->left &= ~(2-1);
	((RECT *)pRect) ->top &= ~(2-1);
	((RECT *)pRect) ->right &= ~(2-1);
	((RECT *)pRect) ->bottom &= ~(2-1);

	DP("[GFXDDRAW_OSD]Set destRect (%d,%d, %d, %d) \n", pRect->left, pRect->top, pRect->right, pRect->bottom);
	if(EqualRect(pRect,&m_rectDst))
		return S_OK;

	if((pRect->right - pRect->left == 0) && (pRect->bottom  - pRect->top  == 0))
		return S_OK;

#if  (!defined(MT3353))
#if  (!defined(HUAYANG_A6))
	if((pRect->right - pRect->left != m_rectDst.right - m_rectDst.left) || (pRect->bottom  - pRect->top != m_rectDst.bottom - m_rectDst.top))
		return E_UNEXPECTED;
#endif
#endif
	m_rectDst = *pRect;
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

STDMETHODIMP CGfxDDrawOSD::SetScnRect(const RECT *pRect)
{
	return S_OK;	
}

STDMETHODIMP CGfxDDrawOSD::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn ;
	return S_OK;
}

STDMETHODIMP CGfxDDrawOSD::SetWindowHandle(HWND hwnd)
{
	if(m_hwnd!=hwnd)
	{
		m_hwnd = hwnd;
		if(m_pClipper)
			m_pClipper->SetHWnd(0,m_hwnd);
	}
	return S_OK;
}

STDMETHODIMP CGfxDDrawOSD::GetWindowHandle(HWND& hwnd)
{
	hwnd = m_hwnd;
	return S_OK;
}

STDMETHODIMP CGfxDDrawOSD::SetAlpha(DWORD dwAlpha)
{
	  if(m_Caps.dwOverlayCaps &DDOVERLAYCAPS_ALPHACONSTANT)
	  {
		m_bAlpha = TRUE;
		m_OFX.dwAlphaConst = dwAlpha*0xff/MAX_ALPHA_INTERNAL;
		m_OFX.dwAlphaConstBitDepth= 8;
		return Update();
	  }
	return S_OK;
}

STDMETHODIMP CGfxDDrawOSD::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	m_bSrcCkey = bEnable;

	if(m_bSrcCkey)
		m_dwSrcCkey = dwSrcCkey;

	return S_OK;
}

STDMETHODIMP CGfxDDrawOSD::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
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
#if  (_WIN32_WCE != 0x600)	//ColorControl and Gamma Control isn't included in ddraw.lib. 
	return m_pOverlay ? S_OK : E_FAIL;
#else
	return m_pOverlay && m_Caps.dwOverlayCaps &DDOVERLAYCAPS_CKEYDEST ? S_OK : E_FAIL;
#endif
}

STDMETHODIMP CGfxDDrawOSD::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
#if  ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))	//ColorControl and Gamma Control isn't included in ddraw.lib. 
	HRESULT hr;
	IDirectDrawSurface7 *pSurf;
	IDirectDrawColorControl *pColor;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	if(FAILED(pSurf->QueryInterface(IID_IDirectDrawColorControl, (void**)&pColor)))
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

STDMETHODIMP CGfxDDrawOSD::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
#if ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))	//ColorControl and Gamma Control isn't included in ddraw.lib. 	HRESULT hr;
	HRESULT hr;
	IDirectDrawSurface7 *pSurf;
	IDirectDrawGammaControl *pGamma;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	if(FAILED(pSurf->QueryInterface(IID_IDirectDrawGammaControl, (void**)&pGamma)))
		return E_FAIL;
	hr = pGamma->GetGammaRamp(0,pGR);
	pGamma->Release();
	return hr;
#else
	return E_FAIL;
#endif
}

STDMETHODIMP CGfxDDrawOSD::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
#if  ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))	//ColorControl and Gamma Control isn't included in ddraw.lib. 	HRESULT hr;
	HRESULT hr;
	IDirectDrawSurface7 *pSurf;
	IDirectDrawColorControl *pColor;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	if(FAILED(pSurf->QueryInterface(IID_IDirectDrawColorControl, (void**)&pColor)))
		return E_FAIL;
	hr = pColor->SetColorControls(pCC);
	pColor->Release();
	return hr;
#else
	return E_FAIL;
#endif
}

STDMETHODIMP CGfxDDrawOSD::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
#if ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))	//ColorControl and Gamma Control isn't included in ddraw.lib. 	HRESULT hr;
	HRESULT hr;
	IDirectDrawSurface7 *pSurf;
	IDirectDrawGammaControl *pGamma;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	if(FAILED(pSurf->QueryInterface(IID_IDirectDrawGammaControl, (void**)&pGamma)))
		return E_FAIL;
	hr = pGamma->SetGammaRamp(0,pGR);
	pGamma->Release();
	return S_OK;
#else
	return E_FAIL;
#endif
}

IDirectDrawSurface7 *CGfxDDrawOSD::GetSurface(LONG num)
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

STDMETHODIMP CGfxDDrawOSD::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
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
	hr = pSurf->Lock(0, &ddsd, dwFlags, 0);
	if(hr==DDERR_SURFACELOST) /* Access to the surface is refused because the surface memory is gone. 
	Call the IDirectDrawSurface::Restore method on this surface to restore the memory associated with it. */
	{
	//	DWORD dwTick = GetTickCount();
		while(hr != S_OK)
		{
			if(FAILED(Restore()))
				return E_FAIL;

			return E_FAIL;
			#if 0	//Fix the issue that surface could be unavailable even if it can be locked successfully. by Golan
			hr = pSurf->Lock(0, &ddsd, dwFlags, 0);
			Sleep(50);
			if(( GetTickCount() - dwTick)>=SURFACELOST_RESTORE_TIMEOUT)
			{
				printf("Error!!! %s \n",GetErrorMsg(hr));
				break;
			}
			#endif
		}
	}
	if(FAILED(hr))
		return E_FAIL;
	if(ppBuf)
		*ppBuf = ddsd.lpSurface;
	if(pStride)
		*pStride = ddsd.lPitch;
	if(pPixCount)
		*pPixCount = ddsd.ddpfPixelFormat.dwRGBBitCount;

	#ifdef PRIMAII

	if(m_bSrcCkey)
	{
		unsigned short* pBuff = (unsigned short* )ddsd.lpSurface + ddsd.lPitch/2 - 32;
		for(unsigned int i =0;i<m_dwHeight;i++)
		{
			for(int j =0;j<32;j++)
			pBuff[i*(ddsd.lPitch/2) + j] = (unsigned short)m_dwSrcCkey;
		}
	}

	#endif
	return S_OK;
}

STDMETHODIMP CGfxDDrawOSD::Unlock(LONG num)
{
	IDirectDrawSurface7 *pSurf;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	return pSurf->Unlock(0);
}


STDMETHODIMP CGfxDDrawOSD::Blit(LONG num, DWORD dwFlags)
{
	IDirectDrawSurface7 *pSurf;
	HRESULT hr;
	DWORD dwFlags2;

	if(!m_bBlit)
		return E_FAIL;
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

STDMETHODIMP CGfxDDrawOSD::Flip(LONG num, DWORD dwFlags)
{
	int i;
	IDirectDrawSurface7 *pSurf;
	HRESULT hr= E_PENDING;
	DWORD dwDDFlags = 0;

	if(m_pOverlay==0)
		return Blit(num,dwFlags);

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;

#ifdef _WIN32_WCE
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
	return hr;
}

STDMETHODIMP CGfxDDrawOSD::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;

	m_bShow = bShow;
	return Update();
}

STDMETHODIMP CGfxDDrawOSD::FindTrueDstCkey()
{
	HDC hdc;
	DWORD dwPixel= 0;
	DWORD dwCkeyTest = 0;
	DWORD dwCkeyOld = 0;
	DWORD dwPixCount, dwReturnPixel;
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
		dwCkeyTest = *(DWORD *)pBuf;
		dwCkeyTest &= dwPixCount<32 ? (1<<dwPixCount)-1 : -1;
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



STDMETHODIMP CGfxDDrawOSD::Update()
{
	DWORD dwFlags;
	RECT rs, rd;

	if((m_rectDst.left == 0)&&(m_rectDst.right == 0)&&(m_rectDst.top == 0)&&(m_rectDst.bottom == 0))
		dwFlags = DDOVER_HIDE;
	else
		dwFlags = m_bShow ? DDOVER_SHOW : DDOVER_HIDE;

	if(m_pPrimary==0)
		return E_FAIL;
	
	if(m_bAlpha)
	{
		dwFlags |= DDOVER_ALPHACONSTOVERRIDE;
	}
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
			 rs.bottom = rs.top + (dst_height<<1);
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
		rs.left &= ~(m_Caps.dwAlignBoundarySrc-1);
	}

	if(m_Caps.dwAlignSizeSrc>1)
	{
		DWORD dwSrcWid = rs.right - rs.left;
		dwSrcWid &= ~(m_Caps.dwAlignSizeSrc-1);
		rs.right =rs.left + dwSrcWid;
	}

	if(m_Caps.dwAlignBoundaryDest>1)
	{
		rd.left &= ~(m_Caps.dwAlignBoundaryDest-1);
	}

	if(m_Caps.dwAlignSizeDest>1)
	{
		DWORD dwDstWid = rd.right - rd.left;
		dwDstWid &= ~(m_Caps.dwAlignSizeDest-1);
		rd.right = rd.left + dwDstWid;
	}

	m_rectBltSrc = rs;
	m_rectBltDst = rd;
	if(m_bLastShow==FALSE && m_bShow==FALSE)
		return S_OK;
	if(m_pOverlay)
	{
		DP("[GFXDDRAW_OSD] UpdateOverlay [%d,%d,%d,%d]->[%d,%d,%d,%d]\n", rs.left, rs.right, rs.top, rs.bottom, rd.left, rd.right, rd.top, rd.bottom);

#ifdef _WIN32_WCE 
#if(_WIN32_WCE != 0x600)
	    if (m_Caps.dwCKeyCaps & DDCKEYCAPS_SRCOVERLAY)
#endif
		{
#ifndef SIRF_A6
			if(m_bSrcCkey)
			{
				DDCOLORKEY ddck;
				memset(&ddck, 0 ,sizeof(ddck));
					
				ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue = m_dwSrcCkey;//0x00008410;//m_dwSrcCkey;
				HRESULT hr =m_pOverlay->SetColorKey(DDCKEY_SRCOVERLAY,&ddck);  
				if(FAILED(hr))
					DP("Source Color Key FAILED!\n");
				else
					dwFlags |= DDOVER_KEYSRC;
			}

#else
		    if ((m_Caps.dwOverlayCaps & DDOVERLAYCAPS_CKEYSRC) && m_bSrcCkey)
		    {
		        dwFlags |= DDOVER_KEYSRCOVERRIDE;

		        // Create an overlay FX structure so we can specify a source color key.
		        // This information is ignored if the DDOVER_SRCKEYOVERRIDE flag 
		        // isn't set.
		        m_OFX.dckSrcColorkey.dwColorSpaceLowValue=m_dwSrcCkey; 
		        m_OFX.dckSrcColorkey.dwColorSpaceHighValue=m_dwSrcCkey;
		    }

#endif
	    }

#endif

		if(dwFlags&DDOVER_HIDE)
		{
			DP("[GFXDDRAW_OSD] UpdateOverlay--- Hide Overlay\n");
		#if defined(MT3351_DBG) || defined(MT3353) || defined(PRIMAII)
			if(FAILED(m_pOverlay->UpdateOverlay(&rs, m_pPrimary, &rd, DDOVER_HIDE, NULL)))
				return E_FAIL;
		#else
			if(FAILED(m_pOverlay->UpdateOverlay(&rs, m_pPrimary, NULL, dwFlags, &m_OFX)))
				return E_FAIL;
		#endif
		}
		else
		{
			DP("[GFXDDRAW_OSD] UpdateOverlay--- Show Overlay\n");

			#ifdef KOVAN_MX31
			if(m_Caps.dwAlignSizeDest>1)
			{
				rd.right &= ~(m_Caps.dwAlignSizeDest-1);
				rd.left &= ~(m_Caps.dwAlignSizeDest-1);
			}
		
			if((rd.right - rd.left > 720)||(rd.bottom - rd.top >421))
			{
				rd.left = (800 - 720)/2;
				rd.top = (480 - 421)/2;
				rd.right = rd.left + 720;
				rd.bottom = rd.top + 421;
			}
			#endif

#if defined(NO_ROT_SCL)
			rs.left = rs.top = 0;
			rs.right = m_dwWidth;
			rs.bottom = m_dwHeight;
#endif
			HRESULT hr = m_pOverlay->UpdateOverlay(&rs, m_pPrimary, &rd, dwFlags, &m_OFX);
			if(FAILED(hr))
			{
				DP("DDRAW_OSD ERRORCODE :0x%x \n",hr);			
				return E_FAIL;
			}
		}
	}
	m_bLastShow = m_bShow;
	return S_OK;
}

STDMETHODIMP CGfxDDrawOSD::ReleaseSurfaces()
{
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
	if(m_pPrimary)
	{
		m_pPrimary->Release();
		m_pPrimary = 0;
	}
	if(m_pBackBuffer)
	{
		if(m_bBlit)
		{
			int i;

			for(i=0;i<(signed)m_dwBackBuffers;i++)
				m_pBackBuffer[i]->Release();
			m_bBlit = FALSE;
		}
		delete[] m_pBackBuffer;
		m_pBackBuffer = 0;
		m_dwBackBuffers = 0;
	}
	m_bLastShow = FALSE;
	m_bShow = FALSE;
	m_bDstTrueCkeyValid = FALSE;
	return S_OK;
}


STDMETHODIMP CGfxDDrawOSD::Restore()
{
	HRESULT hr = E_FAIL;

	//Try to call RestoreAllSurfaces() which supported by IDirectDraw4 . 
	if(m_pDDraw)
	{
		hr = m_pDDraw->RestoreAllSurfaces();
	}
	else
		hr =  E_FAIL;

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
		if(m_pBackBuffer && m_bBlit)
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

STDMETHODIMP CGfxDDrawOSD::SetPixelFormat(DDPIXELFORMAT *pPF, DWORD dwFourCC)
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

STDMETHODIMP CGfxDDrawOSD::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
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

STDMETHODIMP CGfxDDrawOSD::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	if(m_dwFourCC==MAKEFOURCC('P','5','6','5'))
		return S_OK;
	return E_FAIL;
}
STDMETHODIMP CGfxDDrawOSD::GetDDCaps(LONG *pCaps)
{
	if(pCaps==0)
		return E_POINTER;
	return S_OK;
}

STDMETHODIMP CGfxDDrawOSD::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	if(pDevice==0)
		return E_POINTER;
	*(DDDEVICEIDENTIFIER2 *)pDevice = m_Device;
	if(pdwSize)
		*pdwSize = sizeof(m_Device);
	return S_OK;
}

STDMETHODIMP CGfxDDrawOSD::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(m_pPrimary==0 || pInfo==0)
		return E_FAIL;
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
	pInfo->dwFourCC = m_dwFourCC;
	pInfo->dwCaps = 0 
	#if  ((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501))
		| (GFXOVERLAY_CAP_FLIP_BOB)
	#else
		| (m_Caps.dwCaps2&DDCAPS2_CANBOBINTERLEAVED && 
			m_Caps.dwCaps2&DDCAPS2_CANFLIPODDEVEN ? GFXOVERLAY_CAP_FLIP_BOB : 0)
	#endif
#ifdef DDFXCAPS_OVERLAYDEINTERLACE
		| (m_Caps.dwFXCaps&DDFXCAPS_OVERLAYDEINTERLACE ? GFXOVERLAY_CAP_FLIP_DEINTERLACE : 0)
#endif
		;
	return S_OK;
}

STDMETHODIMP CGfxDDrawOSD::IsOpen()
{
	return m_pOverlay ? S_OK : S_FALSE;
}

HRESULT WINAPI CGfxDDrawOSD::EnumSurfaceCallback(LPDIRECTDRAWSURFACE7 pSurf, LPDDSURFACEDESC2 pDesc, LPVOID lpContext)
{
	CGfxDDrawOSD *pMe;

	pMe = reinterpret_cast<CGfxDDrawOSD *>(lpContext);
	pMe->m_pBackBuffer[pMe->m_dwBackBuffers++] = pSurf;
	return DDENUMRET_OK;
}


BOOL WINAPI CGfxDDrawOSD::EnumMonitorCallbackEx(GUID FAR* lpGUID, 
											 LPTSTR lpDriverDescription, LPTSTR lpDriverName, 
											LPVOID lpContext, HMONITOR hm)
{
	CGfxDDrawOSD *pMe;
	GfxMonitorInfo *pMon;

	pMe = reinterpret_cast<CGfxDDrawOSD *>(lpContext);

	if(pMe->m_dwNumMonitors >=MONITORS_AVAIL_NUM)
		return FALSE;
	pMon = &pMe->m_aMonitors[pMe->m_dwNumMonitors++];
	if(lpGUID)
		pMon->guid = *lpGUID;
	else
		ZeroMemory(&pMon->guid,sizeof(pMon->guid));
	pMon->hm = hm;
	ZeroMemory(&pMon->info,sizeof(pMon->info));
	pMon->info.cbSize = sizeof(pMon->info);
	GetMonitorInfo(hm,&pMon->info);
	return (pMe->m_dwNumMonitors<MONITORS_AVAIL_NUM);
}


BOOL WINAPI CGfxDDrawOSD::EnumMonitorCallback(GUID FAR* lpGUID, 
											 LPTSTR lpDriverDescription, LPTSTR lpDriverName, 
											LPVOID lpContext)
{
	CGfxDDrawOSD *pMe;
	GfxMonitorInfo *pMon;

	pMe = reinterpret_cast<CGfxDDrawOSD *>(lpContext);
	pMon = &pMe->m_aMonitors[pMe->m_dwNumMonitors++];
	if(lpGUID)
		pMon->guid = *lpGUID;
	pMon->hm = 0;
	return (pMe->m_dwNumMonitors<MONITORS_AVAIL_NUM);
}

char *CGfxDDrawOSD::GetErrorMsg(HRESULT DDRVAL)
{
    struct DDErrorMessage
    {
        HRESULT Code;
        char *EngMsg;
    };
    static DDErrorMessage LookupTable[] = 
    {
		{ DDERR_CURRENTLYNOTAVAIL,"CurrentlyNotAvail"},
        { DDERR_GENERIC, "Generic"},
        { DDERR_HEIGHTALIGN, "HeightAlign"},
        { DDERR_INCOMPATIBLEPRIMARY, "IncompatiblePrimary"},
        { DDERR_INVALIDCAPS, "InvalidCaps"},
        { DDERR_INVALIDCLIPLIST, "InvalidClipList"},
        { DDERR_INVALIDMODE, "InvalidMode"},
        { DDERR_INVALIDOBJECT, "InvalidObject"},
        { DDERR_INVALIDPARAMS, "InvalidParams"},
        { DDERR_INVALIDPIXELFORMAT, "InvalidPixelFormat"},
        { DDERR_INVALIDRECT, "InvalidRect"},
        { DDERR_LOCKEDSURFACES, "LockedSurfaces"},
        { DDERR_NOALPHAHW, "NoAlphaHardware"},
    	  { DDERR_NOCLIPLIST, "NoClipList"},
        { DDERR_NOCOLORCONVHW, "NoColorConversion"},
        { DDERR_NOCOOPERATIVELEVELSET, "NoCooperativeLevelSet"},
        { DDERR_NOCOLORKEYHW, "NoColorkeyHardware"},
        { DDERR_NOFLIPHW, "NoFlipHardware"},
        { DDERR_NOTFOUND, "ItemNotFind"},
        { DDERR_NOOVERLAYHW, "NoOverlayHardware"},
        { DDERR_OVERLAPPINGRECTS, "OverlappingRects"},
        { DDERR_NORASTEROPHW, "NoRasterOPHardware"},
        { DDERR_NOSTRETCHHW, "NoStretchHardware"},
        { DDERR_NOVSYNCHW, "NoVsyncHardware"},
        { DDERR_NOZOVERLAYHW, "NoZOverlayHardware"},
        { DDERR_OUTOFCAPS, "OutofCaps"},
        { DDERR_OUTOFMEMORY, "OutofMemory"},
        { DDERR_OUTOFVIDEOMEMORY, "OutofVideoMemory"},
        { DDERR_PALETTEBUSY, "PaletteBusy"},
        { DDERR_COLORKEYNOTSET, "ColorkeyNotSet"},
        { DDERR_SURFACEBUSY, "SurfaceBusy"},
        { DDERR_NOBLTHW, "NoBltHardware"},
        { DDERR_SURFACELOST, "SurfaceLost"},
        { DDERR_UNSUPPORTED, "UnSupported"},
        { DDERR_CANTLOCKSURFACE, "CantLockSurface"},
        { DDERR_WASSTILLDRAWING, "WasStillDrawing"},
        { DD_OK, "OK"},
        { 0, 0},

	#if  ((_WIN32_WCE != 0x600)&&(_WIN32_WCE != 0x502)&&(_WIN32_WCE != 0x501))
	{ DDERR_NO3D, "No3D" },
	{ DDERR_NOCOLORKEY, "NoColorkey"},
	{ DDERR_NODIRECTDRAWSUPPORT, "NoDirectDrawSupport"},
	{ DDERR_NOEXCLUSIVEMODE, "NoExclusiveMode"},
	{ DDERR_NOGDI, "NoGDI"},
	{ DDERR_NOMIRRORHW, "NoMirrorHardware"},
	{ DDERR_NOROTATIONHW, "NoRotationHardware"},
	{ DDERR_NOT4BITCOLOR, "No4BitColor"},
	{ DDERR_NOT4BITCOLORINDEX, "No4BitColorIndex"},
	{ DDERR_NOT8BITCOLOR, "No8BitColor"},
	{ DDERR_NOTEXTUREHW, "NoTextureMappingHardware"},
	{ DDERR_NOZBUFFERHW, "NoZBufferHardware"},
	{ DDERR_OVERLAYCANTCLIP, "OverlayCantClip"},
	{ DDERR_OVERLAYCOLORKEYONLYONEACTIVE, "OverlayColorkeyOnlyOneActive"},
	{ DDERR_SURFACEALREADYATTACHED, "SurfaceAlreadyAttached"},
	{ DDERR_SURFACEALREADYDEPENDENT,"SurfaceAlreadyDependent"},
	{ DDERR_ALREADYINITIALIZED, "AlreadyInitialized"},
	{ DDERR_CANNOTATTACHSURFACE,"CannotAttachSurface"},
	{ DDERR_CANNOTDETACHSURFACE,"CannotDetachSurface"},
	{ DDERR_EXCEPTION, "Exception"},
	#endif
	};
    char *msg = "NotFind";

    for (int i=0;LookupTable[i].EngMsg!=NULL;i++)
        if (LookupTable[i].Code == DDRVAL)
            return LookupTable[i].EngMsg;
    return msg;
}

class CGfxOverlayDummy : public IMcGfxOverlay, public CMcUnknown
{
#define MONITORS_AVAIL_NUM	9
#define DISPMODE_NUM	36
public:
	DECLARE_IUNKNOWN

	CGfxOverlayDummy(){};
	virtual ~CGfxOverlayDummy(){};
	STDMETHODIMP Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst = NULL)
	{
		return S_OK;
	}

	STDMETHODIMP Close()
	{
		return S_OK;
	}
	STDMETHODIMP Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
	{
		return S_OK;
	}
	STDMETHODIMP Unlock(LONG num)
	{
		return S_OK;
	}
	STDMETHODIMP Flip(LONG num, DWORD dwFlags)
	{
		return S_OK;
	}
	STDMETHODIMP Show(BOOL bShow)
	{
		return S_OK;
	}
	STDMETHODIMP GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
	{
		return S_OK;
	}
	STDMETHODIMP SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
	{
		return S_OK;
	}
	STDMETHODIMP SetSrcRect(const RECT *pRect)
	{
		return S_OK;
	}
	STDMETHODIMP SetDstRect(const RECT *pRect)
	{
		return S_OK;
	}
	STDMETHODIMP SetScnRect(const RECT *pRect)
	{
		return S_OK;
	}
	STDMETHODIMP GetScnRect(RECT *pRect)
	{
		return S_OK;
	}

	STDMETHODIMP SetWindowHandle(HWND hwnd)
	{
		return S_OK;
	}
	STDMETHODIMP GetWindowHandle(HWND& hwnd)
	{
		return S_OK;
	}
	STDMETHODIMP SetAlpha(DWORD dwAlpha)
	{
		return S_OK;
	}
	STDMETHODIMP SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
	{
		return S_OK;
	}
	STDMETHODIMP SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
	{
		return S_OK;
	}
	STDMETHODIMP GetColorControl(LONG num, DDCOLORCONTROL *pCC)
	{
		return S_OK;
	}
	STDMETHODIMP GetGammaControl(LONG num, DDGAMMARAMP *pGR)
	{
		return S_OK;
	}
	STDMETHODIMP SetColorControl(LONG num, DDCOLORCONTROL *pCC)
	{
		return S_OK;
	}
	STDMETHODIMP SetGammaControl(LONG num, DDGAMMARAMP *pGR)
	{
		return S_OK;
	}
	STDMETHODIMP GetDDCaps(LONG *pCaps)
	{
		return S_OK;
	}
	STDMETHODIMP GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
	{
		return S_OK;
	}
	STDMETHODIMP GetSurfaceInfo(GfxOverlayInfo *pInfo)
	{
		return S_OK;
	}
	STDMETHODIMP IsOpen()
	{
		return S_OK;
	}
	STDMETHODIMP GetVRHandle(LPVOID pHandle)
	{
		return E_FAIL;
	}
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
    STDMETHODIMP SetHWRenderSurf(LPVOID pSurf,int mode )
	{
		return E_FAIL;
	}
	STDMETHODIMP SetDeinterlaceMode(DWORD dwMode)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP GetDeinterlaceMode(DWORD* pdwMode)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}
};

IMcGfxOverlay *CreateDDrawOSD()
{
	return new CGfxDDrawOSD();
//	return new CGfxOverlayDummy();
}
#endif
