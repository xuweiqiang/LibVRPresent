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

#ifdef GFX_FB
#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "McCom.h"
#ifdef __linux__
	#if defined(ST) || defined(PXA_LINUX) || defined (__QNX__)
	#else
	#include <X11/Xlib.h>
	#endif
#endif
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "../../inc/dp.h"

#ifdef PXA_LINUX
#include "gcu.h"
#define DST_ALIGN 16

#define SCN_WIDTH 480
#define SCN_HEIGHT 848
#endif

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
 ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif

enum
{
	RGB_SURFACE = 1,
	YUV_SURFACE,
};



class CGfxFB: public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxFB();
	virtual ~CGfxFB();
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
	HRESULT SetVRHandle(void* pInput) {return E_FAIL;};
#if defined (__linux__) && !defined(__QNX__)
	STDMETHODIMP SetWindowHandle(Window hwnd)
	{
		if(m_hwnd!=hwnd)
		{
			m_hwnd = hwnd;
		}

		return S_OK;
	}

	STDMETHODIMP GetWindowHandle(Window& hwnd)
	{
		return E_NOTIMPL;
	}
	
#else
	STDMETHODIMP SetWindowHandle(HWND hwnd)
	{
		if(m_hwnd!=hwnd)
		{
			m_hwnd = hwnd;
		}
		return S_OK;
	}

	STDMETHODIMP GetWindowHandle(HWND& hwnd)
	{
		return E_NOTIMPL;
	}
#endif
	STDMETHODIMP GetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetHWRenderSurf(LPVOID pSurf)
	{
		return E_NOTIMPL;
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
	STDMETHODIMP UIFlip(LPVOID pHandle) {	return E_NOTIMPL;}
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}

protected:
	STDMETHODIMP CreateSurfaces(DWORD dwBuffers);
	STDMETHODIMP ReleaseSurfaces();
	STDMETHODIMP Restore();
	STDMETHODIMP Update();

	RECT				m_rectSrc, m_rectDst, m_rectScn;
	DWORD				m_dwWidth, m_dwHeight, m_dwBpp, m_dwFourCC;
	DWORD				m_dwLastFlipTime;
	const DWORD			*m_pdwFourCCList;
	DWORD				m_dwBackBuffers;
	BOOL				m_bShow, m_bLastShow, m_bDstCkey, m_bSrcCkey, m_bScnClip, m_bDstTrueCkeyValid, m_bAlpha;
	DWORD				m_dwDstCkey, m_dwSrcCkey, m_dwDstTrueCkey;
	#if defined (__linux__) && !defined(__QNX__)
	Window 				m_hwnd;
	#else
	HWND				m_hwnd;
	#endif
	DWORD				m_dwNumMonitors;
	DWORD m_surface_type;

	int m_iHndFB;
	struct fb_var_screeninfo m_fbvinfo;
	struct fb_fix_screeninfo m_fbfinfo;
	int m_iBufSize;
	unsigned char * m_pBuff;
	BOOL m_bOpen;
};

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('R','5','6','5'),
	MAKEFOURCC('R','8','8','8'),
	0
};

CGfxFB::CGfxFB() : CMcUnknown(0,0)
{
	m_dwWidth = m_dwHeight = 0;
	m_dwBpp = 0;
	m_dwBackBuffers = 0;
	m_bShow = m_bLastShow = m_bDstCkey = m_bSrcCkey = m_bScnClip = m_bDstTrueCkeyValid = m_bAlpha = FALSE;
	m_dwDstCkey = m_dwSrcCkey = 0;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectScn,sizeof(m_rectScn));
	ZeroMemory(&m_fbvinfo,sizeof(m_fbvinfo));
	ZeroMemory(&m_fbfinfo,sizeof(m_fbfinfo));

	m_hwnd = 0;
	m_dwNumMonitors = 0;
	m_surface_type = 0;
	m_iHndFB = NULL;
	m_iBufSize = 0;
	m_pBuff = NULL;
}

CGfxFB::~CGfxFB()
{

}


STDMETHODIMP CGfxFB::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	POINT pt;

	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;


	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	if(pdwFourCC)
		m_pdwFourCCList = pdwFourCC;
	else
		m_pdwFourCCList = const_dwFourCC;
	DP("[GFXFB]Open w:%d h:%d \n",m_dwWidth, m_dwHeight);
	pt.x = m_rectDst.left;
	pt.y = m_rectDst.top;
	m_bScnClip = TRUE;

	return CreateSurfaces(dwBuffers);
}

STDMETHODIMP CGfxFB::Close()
{
	ReleaseSurfaces();
	m_dwNumMonitors = 0;
	return S_OK;
}

STDMETHODIMP CGfxFB::CreateSurfaces(DWORD dwBuffers)
{
	char cDevNode[MAX_PATH];
#ifdef PXA_LINUX
	sprintf(cDevNode,"/dev/fb2");
#else
	sprintf(cDevNode,"/dev/fb0");
#endif
	m_iHndFB = open(cDevNode, O_RDWR);
	
	if(m_iHndFB < 0)
	{
		DP("framebuffer open error\n");
		return E_FAIL;
	}
	
	int ret = -1;
	ret = ioctl(m_iHndFB, FBIOGET_VSCREENINFO, &m_fbvinfo);
	if(ret)
	{
		DP("ioctl(..., FBIOGET_VSCREENINFO, ...) on framebuffer error %x\n", ret);
		goto FAIL;
	}

	DP("m_fbvinfo.bits_per_pixel %d\n", m_fbvinfo.bits_per_pixel);

#ifdef PXA_LINUX
	//Set SCN Size
	SetRect(&m_rectScn,0,0,SCN_WIDTH,SCN_HEIGHT);
	m_fbvinfo.xres = m_dwWidth;
	m_fbvinfo.yres = m_dwHeight;
	m_fbvinfo.bits_per_pixel = 19;
	m_fbvinfo.nonstd = (FORMAT_PLANAR_420<< 20) | (0<< 10) | 0;
	ret = ioctl(m_iHndFB, FBIOPUT_VSCREENINFO, &m_fbvinfo);
	printf("Open()::FBIOPUT_VSCREENINFO :%d \n", ret);
#endif

	ret = ioctl(m_iHndFB, FBIOGET_FSCREENINFO, &m_fbfinfo);
	if(ret)
	{
		DP("ioctl(..., FBIOGET_FSCREENINFO, ...) on framebuffer error %x\n", ret);
		goto FAIL;
	}

	#ifdef PXA_LINUX
	m_iBufSize = m_fbfinfo.smem_len;
	DP("m_fbfinfo.smem_len :%d \n", m_fbfinfo.smem_len);
	#else
	m_iBufSize = m_fbvinfo.xres * m_fbvinfo.bits_per_pixel * m_fbvinfo.yres / 8;
	#endif
	
	m_pBuff = (unsigned char *)mmap(0, m_iBufSize, PROT_WRITE, MAP_SHARED, m_iHndFB, 0);
	if((void*)-1 == m_pBuff)
	{
		DP("mmap framebuffer error\n");
		goto FAIL;
	}

#ifdef PXA_LINUX
	m_dwFourCC = MAKEFOURCC('Y','V','1','2');
#else	
	if(m_fbvinfo.bits_per_pixel == 16)
		m_dwFourCC = MAKEFOURCC('R','5','6','5');
	else if(m_fbvinfo.bits_per_pixel == 24)
		m_dwFourCC = MAKEFOURCC('R','8','8','8');
	else
		goto FAIL;
#endif

	m_bOpen = TRUE;
	return S_OK;

FAIL:
	ReleaseSurfaces();
	return E_FAIL;
}

STDMETHODIMP CGfxFB::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;
	DP("[GFXFB]Set SourceRect (%d,%d, %d, %d) \n", m_rectSrc.left, m_rectSrc.top, m_rectSrc.right, m_rectSrc.bottom);
	return Update();
}

STDMETHODIMP CGfxFB::SetDstRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectDst))
		return S_OK;

	m_rectDst = *pRect;
	IntersectRect(&m_rectDst,&m_rectScn,&m_rectDst);

#ifdef PXA_LINUX
	DP("FB Setdest(%d,%d,%d,%d) \n",m_rectDst.left, m_rectDst.top, m_rectDst.right, m_rectDst.bottom);
	return E_UNEXPECTED;

	m_fbvinfo.xres  = (m_rectDst.right - m_rectDst.left)&(~(DST_ALIGN-1));
	m_fbvinfo.yres  = (m_rectDst.bottom - m_rectDst.top)&(~(DST_ALIGN -1));

	m_fbvinfo.bits_per_pixel = 19;
	m_fbvinfo.nonstd = (FORMAT_PLANAR_420<< 20) | (m_rectDst.top << 10) | m_rectDst.left;

	if (m_pBuff != NULL)
	{
		munmap(m_pBuff, m_iBufSize);
		m_pBuff = NULL;
	}

	DP("Update info: left = %d, top = %d, xres = %d, yres = %d\n", m_fbvinfo.nonstd&0x3ff, (m_fbvinfo.nonstd>>10)&0x3ff, m_fbvinfo.xres, m_fbvinfo.yres);


	int ret = ioctl(m_iHndFB, FBIOPUT_VSCREENINFO, &m_fbvinfo);
	DP("Update()::FBIOPUT_VSCREENINFO :%d \n", ret);
	
	ioctl(m_iHndFB, FBIOGET_FSCREENINFO, &m_fbfinfo);
	m_iBufSize = m_fbfinfo.smem_len;
	DP("Update()::m_iBufSize  :%d \n", m_iBufSize );
	m_pBuff = (unsigned char *)mmap(0, m_iBufSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_iHndFB, 0);
	if(m_pBuff ==NULL)
	{
		DP("Update()::set dest failed \n");
		return E_FAIL;
	}
#endif

	Update();
	return S_OK;
}

STDMETHODIMP CGfxFB::SetScnRect(const RECT *pRect)
{
	RECT rZero = {0,0,0,0};
	if(pRect==0)
	{
		m_bScnClip = FALSE;
		return S_OK;
	}
	if((EqualRect(pRect,&rZero))||(EqualRect(pRect,&m_rectScn) && m_bScnClip))
		return S_OK;
	m_rectScn = *pRect;
	m_bScnClip = TRUE;
	DP("[GFXFB]SetScnRect (%d,%d,%d,%d) \n", m_rectScn.left, m_rectScn.top, m_rectScn.right, m_rectScn.bottom);
	return Update();
}

STDMETHODIMP CGfxFB::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn ;
	return S_OK;
}

STDMETHODIMP CGfxFB::SetAlpha(DWORD dwAlpha)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxFB::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxFB::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxFB::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxFB::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxFB::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxFB::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}


STDMETHODIMP CGfxFB::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
#ifdef PXA_LINUX
	if(ppBuf)
		*ppBuf = m_pBuff;

	if(pStride)
		*pStride = m_fbvinfo.xres;

	if(pPixCount)
		*pPixCount = 1;
#else
	if(ppBuf)
		*ppBuf = m_pBuff+m_fbfinfo.line_length*(m_rectDst.top>0?m_rectDst.top-1:0)+m_rectDst.left*2;

	if(pStride)
		*pStride = m_fbvinfo.xres * m_fbvinfo.bits_per_pixel / 8;

	if(pPixCount)
		*pPixCount = m_fbvinfo.bits_per_pixel / 8;

#endif

	return S_OK;
}

STDMETHODIMP CGfxFB::Unlock(LONG num)
{
	return S_OK;
}

STDMETHODIMP CGfxFB::Flip(LONG num, DWORD dwFlags)
{
	return S_OK;	
}

STDMETHODIMP CGfxFB::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;
	m_bShow = bShow;
	return Update();
}

STDMETHODIMP CGfxFB::Update()
{
	return S_OK;
}

STDMETHODIMP CGfxFB::ReleaseSurfaces()
{
	int i;

	if (m_iHndFB && m_pBuff)
	{
		int ret = -1;
		if(-1 == munmap(m_pBuff, m_iBufSize))
		{
			DP("munmap error\n");
		}
		m_pBuff = NULL;
		m_iBufSize = 0;
	}
	
	if (m_iHndFB)
	{
		close(m_iHndFB);
		m_iHndFB = 0;
	}

	m_bOpen = FALSE;
	m_bLastShow = FALSE;
	m_bShow = FALSE;
	m_bDstTrueCkeyValid = FALSE;
	m_dwBackBuffers = 0;
	return S_OK;
}

STDMETHODIMP CGfxFB::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	DWORD dwFlags = 0;
	RECT r;
	if(pSrc==0 || pDst==0 || pdwFlags==0)
		return E_FAIL;

	if(m_rectSrc.right-m_rectSrc.left != m_rectDst.right-m_rectDst.left)
	{	// downscale issue (horizontal)
		dwFlags |= GFXOVERLAY_SCALE_X;
	}
	if(m_rectSrc.bottom-m_rectSrc.top != m_rectDst.bottom-m_rectDst.top)
	{	//downscale issue (vertical)
		dwFlags |= GFXOVERLAY_SCALE_Y;
	}
	*pSrc = m_rectSrc;
	*pDst = m_rectDst;
	*pdwFlags = dwFlags;


	return S_OK;
}

STDMETHODIMP CGfxFB::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CGfxFB::Restore()
{
	m_bLastShow = FALSE;
	m_bShow = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxFB::GetDDCaps(LONG*pCaps)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxFB::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxFB::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(pInfo==0)
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

STDMETHODIMP CGfxFB::IsOpen()
{
	return m_bOpen== TRUE? S_OK : S_FALSE;
}

IMcGfxOverlay *CreateOverlayFB()
{
	return new CGfxFB;
}

#endif
