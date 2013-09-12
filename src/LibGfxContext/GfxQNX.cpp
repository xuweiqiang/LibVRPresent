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

#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"

#include <screen.h>

//#pragma comment(lib, "SDL.lib")
//#pragma comment(lib, "SDLmain.lib")
 
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

class CGfxQNX: public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxQNX();
	virtual ~CGfxQNX();
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

	STDMETHODIMP GetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetHWRenderSurf(LPVOID pSurf,int mode)
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
	STDMETHODIMP UIFlip(LPVOID pHandle)
	{
		return S_OK;
	}

	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}

protected:

	RECT				m_rectSrc, m_rectDst, m_rectScn;
	RECT				m_rectBltSrc;
	DWORD				m_dwWidth, m_dwHeight, m_dwBpp;
	DWORD				m_dwLastFlipTime;
	const DWORD			*m_pdwFourCCList;
	DWORD				m_dwBackBuffers;
	BOOL				m_bShow, m_bLastShow, m_bDstCkey, m_bSrcCkey, m_bScnClip, m_bDstTrueCkeyValid, m_bAlpha;
	DWORD				m_dwDstCkey, m_dwSrcCkey, m_dwDstTrueCkey;
	HWND				m_hwnd;
	bool m_bopen;
	DWORD				m_dwNumMonitors;

	screen_context_t screen_ctx;
	screen_display_t screen_disp;
	screen_window_t screen_win;
	screen_pixmap_t screen_pix;
	screen_buffer_t screen_pbuf;
	screen_buffer_t screen_wbuf;
 	int m_format;
	int m_stride;
	void *m_pointer;
};

CGfxQNX::CGfxQNX() : CMcUnknown(0,0)
{
	m_dwWidth = m_dwHeight = 0;
	m_dwBpp = 0;
	m_dwBackBuffers = 0;
	m_bShow = m_bLastShow = m_bDstCkey = m_bSrcCkey = m_bScnClip = m_bDstTrueCkeyValid = m_bAlpha = FALSE;
	m_dwDstCkey = m_dwSrcCkey = 0;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectScn,sizeof(m_rectScn));
	ZeroMemory(&m_rectBltSrc,sizeof(m_rectBltSrc));

	m_hwnd = 0;
	m_dwNumMonitors = 0;
	m_bopen = 0;
}

CGfxQNX::~CGfxQNX()
{

}

STDMETHODIMP CGfxQNX::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	if(m_bopen)
		return E_UNEXPECTED;

	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;

	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	
	int rc;
	rc = screen_create_context(&screen_ctx, SCREEN_WINDOW_MANAGER_CONTEXT);
	rc = screen_create_window(&screen_win, screen_ctx);

	int buf_size[2];
	buf_size[0]=m_dwWidth;
	buf_size[1]=m_dwHeight;

	int val;
	val = SCREEN_USAGE_NATIVE | SCREEN_USAGE_ROTATION;
	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_USAGE, &val);
	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, buf_size);
	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SOURCE_SIZE, buf_size);
	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SIZE, buf_size);

	int pos[2] = { 0, 0 };
	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_POSITION, pos);
	rc = screen_create_window_buffers(screen_win, 1);


	rc = screen_create_pixmap(&screen_pix, screen_ctx);
	val = SCREEN_USAGE_WRITE | SCREEN_USAGE_NATIVE | SCREEN_USAGE_ROTATION;
	rc = screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_USAGE, &val);

	rc = screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_FORMAT, &m_format);
	rc = screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_FORMAT, &m_format);


	rc = screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_BUFFER_SIZE, buf_size);
	rc = screen_create_pixmap_buffer(screen_pix);
	rc = screen_get_pixmap_property_pv(screen_pix, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_pbuf);
	rc = screen_get_buffer_property_pv(screen_pbuf, SCREEN_PROPERTY_POINTER, &m_pointer);
	rc = screen_get_buffer_property_iv(screen_pbuf, SCREEN_PROPERTY_STRIDE, &m_stride);


	m_bopen = true;
	return S_OK;
}

STDMETHODIMP CGfxQNX::Close()
{
	if(!m_bopen)
		return E_UNEXPECTED;

 	screen_destroy_pixmap(screen_pix);
	screen_destroy_window(screen_win);
	screen_destroy_context(screen_ctx);

	m_bopen = false;
	return S_OK;
}


STDMETHODIMP CGfxQNX::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;
	DP("[GFXSDL]Set SourceRect (%d,%d, %d, %d) \n", m_rectSrc.left, m_rectSrc.top, m_rectSrc.right, m_rectSrc.bottom);
	return S_OK;
}

STDMETHODIMP CGfxQNX::SetDstRect(const RECT *pRect)
{
	if(pRect->left == 0 && pRect->top == 0 && pRect->right == 0 &&pRect->bottom == 0 )
		return S_OK;
	
	if(EqualRect(pRect,&m_rectDst))
		return S_OK;

	DP("CGfxQNX::SetDstRect %d,%d,%d,%d\n",pRect->left, pRect->top, pRect->right , pRect->bottom);
	m_rectDst = *pRect;

	return S_OK;
}

STDMETHODIMP CGfxQNX::SetScnRect(const RECT *pRect)
{
	return S_OK;
}

STDMETHODIMP CGfxQNX::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn ;
	return S_OK;
}

STDMETHODIMP CGfxQNX::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}

STDMETHODIMP CGfxQNX::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxQNX::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxQNX::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxQNX::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxQNX::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxQNX::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}


STDMETHODIMP CGfxQNX::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(ppBuf)
 	  *ppBuf = m_pointer;
  	
	if(pStride)
			*pStride = m_dwWidth;

 	return S_OK;
}

STDMETHODIMP CGfxQNX::Unlock(LONG num)
{
	return S_OK;
}

STDMETHODIMP CGfxQNX::Flip(LONG num, DWORD dwFlags)
{
	int rc;

	struct {
	int src_x[2];
	int src_y[2];
	int src_width[2];
	int src_height[2];
	int dst_x[2];
	int dst_y[2];
	int dst_width[2];
	int dst_height[2];
	int none;
	} screen_blit_attr;
	memset(&screen_blit_attr,0,sizeof(screen_blit_attr));
	
	screen_blit_attr.src_x = { SCREEN_BLIT_SOURCE_X, 0 };
	screen_blit_attr.src_y = { SCREEN_BLIT_SOURCE_Y, 0 };
	screen_blit_attr.src_width = { SCREEN_BLIT_SOURCE_WIDTH, m_dwWidth};
	screen_blit_attr.src_height = { SCREEN_BLIT_SOURCE_HEIGHT, m_dwHeight };
	screen_blit_attr.dst_x = { SCREEN_BLIT_DESTINATION_X, 0 };
	screen_blit_attr.dst_y = { SCREEN_BLIT_DESTINATION_Y, 0 };

	screen_blit_attr.dst_width = { SCREEN_BLIT_DESTINATION_WIDTH, m_dwWidth };
	screen_blit_attr.dst_height = { SCREEN_BLIT_DESTINATION_HEIGHT, m_dwHeight};
	screen_blit_attr.none = SCREEN_BLIT_END;

	rc = screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_wbuf);
	rc = screen_blit(screen_ctx, screen_wbuf, screen_pbuf, (const int *)&screen_blit_attr);

	int rects[4]={0,0,m_dwWidth,m_dwHeight};
	rc = screen_post_window(screen_win, screen_wbuf, 1, rects, 0);

	return S_OK;	
}

STDMETHODIMP CGfxQNX::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;
	m_bShow = bShow;
	return S_OK;	
}

STDMETHODIMP CGfxQNX::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxQNX::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CGfxQNX::GetDDCaps(LONG*pCaps)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxQNX::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxQNX::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(pInfo==0)
		return E_FAIL;
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
	pInfo->dwFourCC = MAKEFOURCC('R','5','6','5');
	pInfo->dwCaps = 0;
	return S_OK;
}

STDMETHODIMP CGfxQNX::IsOpen()
{
	return m_bopen;
}

IMcGfxOverlay *CreateOverlayQNX()
{
	return new CGfxQNX;
}

