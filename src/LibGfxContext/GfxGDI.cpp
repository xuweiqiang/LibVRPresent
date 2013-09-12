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
// 1) Check multimon support (multiple monitors)
// 2) Implement interlacing support

#include "../../inc/GfxBuildDef.h"

#ifdef GFX_GDI
#include <stdio.h>
#include <windows.h>
#include <ddraw.h>
#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"

#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxguid.lib")

#if ((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501))
	typedef HRESULT (* LPDIRECTDRAWENUMERATEEX)(LPDDENUMCALLBACKEX lpCallback, LPVOID lpContext, DWORD dwFlags);
#endif

#define SURFACELOST_RESTORE_TIMEOUT 5000

struct GfxMonitorInfo
{
	GUID guid;
	HMONITOR hm;
	MONITORINFO info;
};

class CGfxGDI: public IMcGfxOverlay, public CMcUnknown
{
#define MONITORS_AVAIL_NUM	9
#define DISPMODE_NUM	36
public:
	DECLARE_IUNKNOWN

	CGfxGDI();
	virtual ~CGfxGDI();
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
	STDMETHODIMP GetDDCaps(LONG*pCaps);
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
	STDMETHODIMP CreateSurfaces(DWORD dwBuffers);
	STDMETHODIMP ReleaseSurfaces();
	STDMETHODIMP Restore();
	STDMETHODIMP Update();
	STDMETHODIMP Blit(LONG num, DWORD dwFlags);
	HBITMAP GetSurface(LONG num);
	LPBYTE GetSurfaceBits(LONG num);
	STDMETHODIMP EnumerateMonitors();
	GfxMonitorInfo *GetBestMonitor(POINT *pt);
	static BOOL WINAPI EnumMonitorCallbackEx(GUID FAR* lpGUID, 
											 LPTSTR lpDriverDescription, LPTSTR lpDriverName, 
											LPVOID lpContext, HMONITOR hm);
	static BOOL WINAPI EnumMonitorCallback(GUID FAR* lpGUID, 
											 LPTSTR lpDriverDescription, LPTSTR lpDriverName, 
											LPVOID lpContext);

	STDMETHODIMP EnumerateDispModes();

	RECT				m_rectSrc, m_rectDst, m_rectScn;
	RECT				m_rectBltSrc, m_rectBltDst;
	RECT				*m_prectSurfScale;
	DWORD				*m_pdwSurfScaleFlags;
	DWORD				m_dwWidth, m_dwHeight, m_dwBpp, m_dwFourCC;
	DWORD				m_dwLastFlipTime;
	const DWORD			*m_pdwFourCCList;
	DWORD				m_dwBackBuffers;
	BOOL				m_bShow, m_bLastShow, m_bDstCkey, m_bSrcCkey, m_bScnClip, m_bDstTrueCkeyValid, m_bAlpha;
	DWORD				m_dwDstCkey, m_dwSrcCkey, m_dwDstTrueCkey;
	HBITMAP				*m_pBackBuffer;
	LPBYTE				*m_pBackBufferBits;
	HWND				m_hwnd;
	DWORD				m_dwNumMonitors;
	GfxMonitorInfo		m_aMonitors[MONITORS_AVAIL_NUM];
	GfxMonitorInfo		*m_pMonitorInfo;
};

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('R','5','6','5'),
	0
};

CGfxGDI::CGfxGDI() : CMcUnknown(0,0)
{
	m_pBackBuffer = 0;
	m_pBackBufferBits = 0;
	m_dwWidth = m_dwHeight = 0;
	m_dwBpp = 0;
	m_dwBackBuffers = 0;
	m_bDstCkey = m_bSrcCkey = m_bScnClip = m_bDstTrueCkeyValid = m_bAlpha = FALSE;
	m_bShow = m_bLastShow = TRUE;
	m_dwDstCkey = m_dwSrcCkey = 0;
	m_pdwSurfScaleFlags = 0;
	m_prectSurfScale = 0;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectScn,sizeof(m_rectScn));
	ZeroMemory(&m_rectBltSrc,sizeof(m_rectBltSrc));
	ZeroMemory(&m_rectBltDst,sizeof(m_rectBltDst));
	ZeroMemory(&m_aMonitors,sizeof(m_aMonitors));
	m_hwnd = 0;
	m_dwNumMonitors = 0;
	m_pMonitorInfo = 0;
}

CGfxGDI::~CGfxGDI()
{
}

STDMETHODIMP CGfxGDI::EnumerateMonitors()
{
#ifndef _WIN32_WCE
	HRESULT hr;
	HINSTANCE hlib;
	LPDIRECTDRAWENUMERATEEX pDirectDrawEnumerateEx;

	hlib = LoadLibrary(TEXT("ddraw.dll"));
	if(hlib==0)
		return E_FAIL;

	m_dwNumMonitors = 0;
    pDirectDrawEnumerateEx = (LPDIRECTDRAWENUMERATEEX )GetProcAddress(hlib,"DirectDrawEnumerateExA");
    if(pDirectDrawEnumerateEx)
    {
        hr = pDirectDrawEnumerateEx(EnumMonitorCallbackEx, this,
                                DDENUM_ATTACHEDSECONDARYDEVICES |
                                DDENUM_DETACHEDSECONDARYDEVICES |
                                DDENUM_NONDISPLAYDEVICES);
	}
    else
		hr = DirectDrawEnumerate(EnumMonitorCallback,this);
	FreeLibrary(hlib);
	return SUCCEEDED(hr) ? S_OK : E_FAIL;
#else
	return S_OK;
#endif
}

GfxMonitorInfo *CGfxGDI::GetBestMonitor(POINT *pt)
{
	int i;

	for(i=0;i<(signed)m_dwNumMonitors;i++)
	{
		if(PtInRect(&m_aMonitors[i].info.rcMonitor,*pt))
			return &m_aMonitors[i];
	}
	return 0;
}

STDMETHODIMP CGfxGDI::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{

	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));

	if(pdwFourCC)
		m_pdwFourCCList = pdwFourCC;
	else
		m_pdwFourCCList = const_dwFourCC;
	DP("[GFXGDI]Open w:%d h:%d \n",m_dwWidth, m_dwHeight);

#ifndef _WIN32_WCE
	POINT pt;
	EnumerateMonitors();
	pt.x = m_rectDst.left;
	pt.y = m_rectDst.top;
	m_pMonitorInfo = GetBestMonitor(&pt);
#endif

	m_bScnClip = TRUE;
	if(m_pMonitorInfo==0)
		SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	else
		m_rectScn = m_pMonitorInfo->info.rcMonitor;
	return CreateSurfaces(dwBuffers);
}

STDMETHODIMP CGfxGDI::Close()
{
	ReleaseSurfaces();
	m_dwNumMonitors = 0;
	return S_OK;
}

STDMETHODIMP CGfxGDI::CreateSurfaces(DWORD dwBuffers)
{
	int i;
	BYTE bTemp[sizeof(BITMAPINFOHEADER) + 3 * sizeof(RGBQUAD)];
	BITMAPINFO *pBMI = (BITMAPINFO *)bTemp;
	BITMAPINFOHEADER *pBMIHeader = &pBMI->bmiHeader;
	DWORD *pBMIColors = (DWORD *)&pBMI->bmiColors;

	if(m_dwWidth==0 || m_dwHeight==0 || m_pdwFourCCList==0 || m_pBackBuffer)
		return E_FAIL;
	ZeroMemory(bTemp,sizeof(bTemp));

	i=0;

	bool bfound = false;
	while(!bfound)
	{
		m_dwFourCC=m_pdwFourCCList[i++];
		if(m_dwFourCC == 0)
			break;
		switch(m_dwFourCC)
		{
		case MAKEFOURCC('R','5','6','5'):
			pBMIColors[0] = 0x1F << 11;
			pBMIColors[1] = 0x3F << 5;
			pBMIColors[2] = 0x1F;
			m_dwBpp = 16;
			bfound = true;
			break;
#if 0
			// needs to be implemented
		case MAKEFOURCC('R','2','4',' '):
			pBMIColors[0] = 0x00FF0000;
			pBMIColors[1] = 0x0000FF00;
			pBMIColors[2] = 0x000000FF;
			m_dwBpp = 24;
			goto found;
#endif
		default:
			break;
		}
	}
	if(m_dwFourCC==0)
		return E_FAIL;
	m_pBackBuffer = new HBITMAP[dwBuffers];
	if(m_pBackBuffer==0)
		return E_FAIL;
	m_pBackBufferBits = new LPBYTE[dwBuffers];
	if(m_pBackBufferBits==0)
	{
		delete[] m_pBackBuffer;
		return E_FAIL;
	}
	pBMIHeader->biSize = sizeof(BITMAPINFOHEADER);
	pBMIHeader->biWidth = m_dwWidth;
	pBMIHeader->biHeight = -(signed)m_dwHeight;
	pBMIHeader->biPlanes = 1;
	pBMIHeader->biBitCount = (WORD)m_dwBpp;
	pBMIHeader->biCompression = BI_BITFIELDS;
	pBMIHeader->biSizeImage = m_dwWidth*m_dwHeight*m_dwBpp >> 3;
	for(i=0;i<(signed)dwBuffers;i++)
	{
		if((m_pBackBuffer[i]=CreateDIBSection(0, pBMI, DIB_RGB_COLORS, (void**)&m_pBackBufferBits[i], 0, 0))==0)
			break;
	}
	m_dwBackBuffers = i;
	if(i==0)
	{
		ReleaseSurfaces();
		return E_FAIL;
	}
	m_pdwSurfScaleFlags = new DWORD[m_dwBackBuffers];
	m_prectSurfScale = new RECT[m_dwBackBuffers];
	return S_OK;
}

STDMETHODIMP CGfxGDI::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;
	DP("[GFXGDI]Set SourceRect (%d,%d, %d, %d) \n", m_rectSrc.left, m_rectSrc.top, m_rectSrc.right, m_rectSrc.bottom);
	return Update();
}

STDMETHODIMP CGfxGDI::SetDstRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectDst))
		return S_OK;
	m_rectDst = *pRect;

//	if((m_rectDst.right - m_rectDst.left != m_dwWidth) || (m_rectDst.bottom - m_rectDst.top != m_dwHeight))	
//		return E_UNEXPECTED;

	if(m_pMonitorInfo)
	{	// the size and offset may have changed... by simply restoring surfaces
		GetMonitorInfo(m_pMonitorInfo->hm,&m_pMonitorInfo->info);
		m_rectScn = m_pMonitorInfo->info.rcMonitor;
	}
	else
		SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	Update();

#ifndef _WIN32_WCE
	GfxMonitorInfo *pinfo;
	POINT pt;
	pt.x = m_rectDst.left;	// check to see if we have gone to a different monitor
	pt.y = m_rectDst.top;
	pinfo = GetBestMonitor(&pt);
	if(pinfo && m_pMonitorInfo!=pinfo)
		return S_FALSE;
#endif
	return S_OK;
}

STDMETHODIMP CGfxGDI::SetScnRect(const RECT *pRect)
{
	return S_OK;	// ignored
}

STDMETHODIMP CGfxGDI::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn ;
	return S_OK;
}

STDMETHODIMP CGfxGDI::SetWindowHandle(HWND hwnd)
{
	if(m_hwnd!=hwnd)
	{
		m_hwnd = hwnd;
	}
	return S_OK;
}

STDMETHODIMP CGfxGDI::GetWindowHandle(HWND& hwnd)
{
	hwnd = m_hwnd;
	return S_OK;
}

STDMETHODIMP CGfxGDI::SetAlpha(DWORD dwAlpha)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGDI::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
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

STDMETHODIMP CGfxGDI::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return E_FAIL;
}

STDMETHODIMP CGfxGDI::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
#if 0	//Because it's not supportted on many pc. I disabled it& for further development //By Golan 
	 VIDEOPARAMETERS disp;
	 LONG ret=0;
	 if(pCC == NULL)
	 	return E_FAIL;
	 ZeroMemory(&disp, sizeof(disp));
	disp.dwCommand = VP_COMMAND_GET;
	GUID guidTmp = {0x02c62061,0x1097,0x11d1,{0x92,0x0f,0x00,0xa0,0x24,0xdf,0x15,0x6e  } };
	disp.Guid = guidTmp;

	ret = ChangeDisplaySettingsEx(NULL,NULL, NULL,CDS_VIDEOPARAMETERS,&disp);
	if(ret == DISP_CHANGE_SUCCESSFUL)
	{
		if(disp.dwFlags & VP_FLAGS_BRIGHTNESS)
		{
			pCC->dwFlags = DDCOLOR_BRIGHTNESS;
			pCC->lBrightness = disp.dwBrightness;
		}
		if(disp.dwFlags & VP_FLAGS_CONTRAST)
		{
			pCC->dwFlags = DDCOLOR_CONTRAST;
			pCC->lContrast = disp.dwContrast;
		}
		return S_OK;
	}
#endif
	return E_FAIL;
}

STDMETHODIMP CGfxGDI::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGDI::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
#if 0
	 VIDEOPARAMETERS disp;
	 LONG ret=0;
	 if(pCC == NULL)
	 	return E_FAIL;
	 ZeroMemory(&disp, sizeof(disp));
	disp.dwCommand = VP_COMMAND_SET;
	disp.dwFlags = VP_FLAGS_BRIGHTNESS|VP_FLAGS_CONTRAST;
	disp.dwBrightness = pCC->lBrightness;
	disp.dwContrast = pCC->lContrast;

	ret = ChangeDisplaySettingsEx(NULL,NULL, NULL,CDS_VIDEOPARAMETERS,&disp);
	if(ret == DISP_CHANGE_SUCCESSFUL)
	{
		return S_OK;
	}
#endif
	return E_FAIL;
}

STDMETHODIMP CGfxGDI::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

HBITMAP CGfxGDI::GetSurface(LONG num)
{	
	if(num>=0 && num<(signed)m_dwBackBuffers)
		return m_pBackBuffer[num];
	return 0;
}

LPBYTE CGfxGDI::GetSurfaceBits(LONG num)
{	
	if(num>=0 && num<(signed)m_dwBackBuffers)
		return m_pBackBufferBits[num];
	return 0;
}

STDMETHODIMP CGfxGDI::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	LPBYTE pSurf;

	pSurf = GetSurfaceBits(num);
	if(pSurf==0)
		return E_FAIL;
	if(ppBuf)
		*ppBuf = pSurf;
	if(pStride)
		*pStride = m_dwWidth*m_dwBpp >> 3;
	if(pPixCount)
		*pPixCount = m_dwBpp;
	m_pdwSurfScaleFlags[num] = 0;
	return S_OK;
}

STDMETHODIMP CGfxGDI::Unlock(LONG num)
{
	return S_OK;
}

STDMETHODIMP CGfxGDI::Blit(LONG num, DWORD dwFlags)
{
	HBITMAP pSurf;
	HDC hSrcDC, hDstDC;
	BOOL bRet;
	int scalenum;

	pSurf = GetSurface(num);
	if(pSurf==0)
		return E_FAIL;
	hDstDC = ::GetDC(m_hwnd);
	hSrcDC = ::CreateCompatibleDC(hDstDC);
	HBITMAP pSurf_bak = (HBITMAP )::SelectObject(hSrcDC, pSurf);
	if(num<0)
		scalenum = 0;
	else
		scalenum = num;

	DWORD dwBlitFlag ;
	if(m_bShow)	
		dwBlitFlag  = SRCCOPY;
	else
		dwBlitFlag  = BLACKNESS;

	if(m_pdwSurfScaleFlags[scalenum])
	{

		bRet = ::StretchBlt(hDstDC,
					m_rectBltDst.left , m_rectBltDst.top,
					(m_rectBltDst.right - m_rectBltDst.left),	m_rectBltDst.bottom - m_rectBltDst.top,	   
					hSrcDC,
					m_prectSurfScale[scalenum].left,
					m_prectSurfScale[scalenum].top,
					m_prectSurfScale[scalenum].right -m_prectSurfScale[scalenum].left,
					m_prectSurfScale[scalenum].bottom -m_prectSurfScale[scalenum].top,
					dwBlitFlag);
	}
	else
		bRet = ::StretchBlt(hDstDC,
					m_rectBltDst.left, m_rectBltDst.top,
					m_rectBltDst.right - m_rectBltDst.left,	m_rectBltDst.bottom - m_rectBltDst.top,	   
					hSrcDC,
					m_rectBltSrc.left, m_rectBltSrc.top,
					m_rectBltSrc.right - m_rectBltSrc.left, m_rectBltSrc.bottom - m_rectBltSrc.top,		   
					dwBlitFlag);
#if 0
	bRet = ::BitBlt(hDstDC,
				m_rectBltDst.left, m_rectBltDst.top,
				m_rectBltDst.right - m_rectBltDst.left,	m_rectBltDst.bottom - m_rectBltDst.top,	   
				hSrcDC,
				m_rectBltSrc.left, m_rectBltSrc.top,
				SRCCOPY);
#endif
   ::SelectObject(hSrcDC, pSurf_bak);
   ::DeleteDC(hSrcDC);
   ::ReleaseDC(m_hwnd, hDstDC);
   return bRet ? S_OK : E_FAIL;
}

STDMETHODIMP CGfxGDI::Flip(LONG num, DWORD dwFlags)
{
	return Blit(num,dwFlags);
}

STDMETHODIMP CGfxGDI::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;
	m_bShow = bShow;
	return Update();
}

STDMETHODIMP CGfxGDI::Update()
{
	RECT rs, rd;

	SetRect(&rs,0,0,m_dwWidth,m_dwHeight);
	IntersectRect(&rs,&rs,&m_rectSrc);
	rd = m_rectDst;
	if(m_hwnd)
	{
		POINT p;

		ZeroMemory(&p,sizeof(p));
		ClientToScreen(m_hwnd,&p);
		OffsetRect(&rd,-p.x,-p.y);
	}
#if 0
	OffsetRect(&rd,-m_rectScn.left,-m_rectScn.top);
	if(m_Caps.dwAlignSizeSrc>1)
	{
		rs.right &= ~(m_Caps.dwAlignSizeSrc-1);
		rs.left &= ~(m_Caps.dwAlignSizeSrc-1);
	}
#endif
	m_rectBltSrc = rs;
	m_rectBltDst = rd;
	return S_OK;
}

STDMETHODIMP CGfxGDI::ReleaseSurfaces()
{
	int i;

	m_bDstTrueCkeyValid = FALSE;
	if(m_pBackBuffer)
	{
		for(i=0;i<(signed)m_dwBackBuffers;i++)
		{
			if(m_pBackBuffer[i])
			{
				DeleteObject(m_pBackBuffer[i]);
				m_pBackBuffer[i]=0;
			}
		}
		delete[] m_pBackBuffer;
		m_pBackBuffer = 0;
	}
	if(m_pBackBufferBits)
	{
		delete[] m_pBackBufferBits;
		m_pBackBufferBits = 0;
	}
	if(m_pdwSurfScaleFlags)
	{
		delete[] m_pdwSurfScaleFlags;
		m_pdwSurfScaleFlags = 0;
	}
	if(m_prectSurfScale)
	{
		delete[] m_prectSurfScale;
		m_prectSurfScale = 0;
	}
	m_dwBackBuffers = 0;
	return S_OK;
}

STDMETHODIMP CGfxGDI::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	DWORD dwFlags;
	RECT r;

	if(pSrc==0 || pDst==0 || pdwFlags==0 || (m_rectDst.right-m_rectDst.left  == 0) || (m_rectDst.bottom-m_rectDst.top  == 0))
		return E_FAIL;
	SetRect(&r, 0, 0, 0, 0);

	dwFlags = 0;
	if((m_rectSrc.right-m_rectSrc.left > m_rectDst.right-m_rectDst.left)||(m_rectSrc.bottom-m_rectSrc.top > m_rectDst.bottom-m_rectDst.top))
	{	// downscale issue (horizontal)
		dwFlags |= GFXOVERLAY_SCALE_X|GFXOVERLAY_SCALE_Y;
		r.right = m_rectDst.right-m_rectDst.left;
		r.bottom = m_rectDst.bottom-m_rectDst.top;

		*pSrc = m_rectSrc;
		*pDst = r;
	}
	*pdwFlags = dwFlags;
	return S_OK;
}

STDMETHODIMP CGfxGDI::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	m_prectSurfScale[num] = *pScale;
	m_pdwSurfScaleFlags[num] = dwFlags;
	return S_OK;
}

STDMETHODIMP CGfxGDI::Restore()
{
	m_bLastShow = TRUE;
	m_bShow = TRUE;
	//m_hwnd = 0;
	return S_OK;
}

STDMETHODIMP CGfxGDI::GetDDCaps(LONG*pCaps)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGDI::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxGDI::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(m_pBackBuffer==0 || pInfo==0)
		return E_FAIL;
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
	pInfo->dwFourCC = m_dwFourCC;
	pInfo->dwCaps = 0;
	return S_OK;
}

STDMETHODIMP CGfxGDI::IsOpen()
{
	return m_pBackBuffer ? S_OK : S_FALSE;
}

BOOL WINAPI CGfxGDI::EnumMonitorCallbackEx(GUID FAR* lpGUID, 
											 LPTSTR lpDriverDescription, LPTSTR lpDriverName, 
											LPVOID lpContext, HMONITOR hm)
{
	CGfxGDI *pMe;
	GfxMonitorInfo *pMon;

	pMe = reinterpret_cast<CGfxGDI *>(lpContext);

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


BOOL WINAPI CGfxGDI::EnumMonitorCallback(GUID FAR* lpGUID, 
											 LPTSTR lpDriverDescription, LPTSTR lpDriverName, 
											LPVOID lpContext)
{
	CGfxGDI *pMe;
	GfxMonitorInfo *pMon;

	pMe = reinterpret_cast<CGfxGDI *>(lpContext);
	pMon = &pMe->m_aMonitors[pMe->m_dwNumMonitors++];
	pMon->guid = *lpGUID;
	pMon->hm = 0;
	return (pMe->m_dwNumMonitors<MONITORS_AVAIL_NUM);
}

IMcGfxOverlay *CreateOverlayGDI()
{
	return new CGfxGDI;
}

#endif
