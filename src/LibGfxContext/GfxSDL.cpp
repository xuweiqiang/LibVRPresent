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

#ifdef GFX_SDL
#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#ifdef __linux__
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <X11/Xlib.h>
#else
#include <SDL.h>
#include <SDL_syswm.h>
#endif
#include "../../inc/dp.h"

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")

 
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

class CGfxSDL: public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxSDL();
	virtual ~CGfxSDL();
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

#ifdef __linux__
	STDMETHODIMP SetWindowHandle(Window hwnd)
	{
		if(m_hwnd!=hwnd)
		{
			m_hwnd = hwnd;
#if 0	//Don't set wnd here. It will never takes effect.
			char input[256];
			memset(input, 0 ,sizeof(input));
			sprintf(input, "SDL_WINDOWID=%ld",m_hwnd);
			SDL_putenv(input);
#endif			
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
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
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
	STDMETHODIMP UIFlip(LPVOID pHandle)
	{
		return S_OK;
	}

	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}

protected:
	STDMETHODIMP CreateSurfaces(DWORD dwBuffers);
	STDMETHODIMP ReleaseSurfaces();
	STDMETHODIMP Restore();
	STDMETHODIMP Update();
	SDL_Surface* GetRGBSurface(LONG num);
	SDL_Overlay* GetYUVSurface(LONG num);
	SDL_Surface* SDL_ScaleSurface(SDL_Surface* Surface, Uint16 Width, Uint16 Height);
	Uint32 ReadPixel(SDL_Surface *surface, int x, int y);
	void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

	RECT				m_rectSrc, m_rectDst, m_rectScn;
	RECT				m_rectBltSrc;
	DWORD				m_dwWidth, m_dwHeight, m_dwBpp, m_dwFourCC;
	DWORD				m_dwLastFlipTime;
	const DWORD			*m_pdwFourCCList;
	DWORD				m_dwBackBuffers;
	BOOL				m_bShow, m_bLastShow, m_bDstCkey, m_bSrcCkey, m_bScnClip, m_bDstTrueCkeyValid, m_bAlpha;
	DWORD				m_dwDstCkey, m_dwSrcCkey, m_dwDstTrueCkey;
	SDL_Surface			**m_pBackBuffer;
	SDL_Overlay			**m_pOverlaySurf;
	#ifdef __linux__
	Window 				m_hwnd;
	#else
	HWND				m_hwnd;
	#endif
	DWORD				m_dwNumMonitors;
	SDL_VideoInfo m_videoInfo;
	SDL_Surface* m_screen;
	DWORD m_surface_type;
	SDL_SysWMinfo m_wminfo;

};

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
	MAKEFOURCC('R','5','6','5'),
	0
};

CGfxSDL::CGfxSDL() : CMcUnknown(0,0)
{
	m_pBackBuffer = 0;
	m_dwWidth = m_dwHeight = 0;
	m_dwBpp = 0;
	m_dwBackBuffers = 0;
	m_bShow = m_bLastShow = m_bDstCkey = m_bSrcCkey = m_bScnClip = m_bDstTrueCkeyValid = m_bAlpha = FALSE;
	m_dwDstCkey = m_dwSrcCkey = 0;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectScn,sizeof(m_rectScn));
	ZeroMemory(&m_rectBltSrc,sizeof(m_rectBltSrc));
	ZeroMemory(&m_videoInfo,sizeof(m_videoInfo));
	ZeroMemory(&m_wminfo,sizeof(m_wminfo));

	m_hwnd = 0;
	m_dwNumMonitors = 0;
	m_surface_type = 0;
}

CGfxSDL::~CGfxSDL()
{

}

STDMETHODIMP CGfxSDL::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	POINT pt;

	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;

	if(m_hwnd)
	{
		char input[256];
		memset(input, 0 ,sizeof(input));
		sprintf(input, "SDL_WINDOWID=%ld",m_hwnd);
		SDL_putenv(input);
		DP(input);
	}
	else
	{
		char input[256];
		memset(input, 0 ,sizeof(input));
		sprintf(input, "SDL_VIDEO_WINDOW_POS=0,0");
		SDL_putenv(input);
		DP(input);
	}

	if(SDL_WasInit(SDL_INIT_VIDEO)==0)
	{
		if(SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			DP("[GfxSDL]Error: %s \n",SDL_GetError());
			return E_FAIL;
		}
	}

	const SDL_VideoInfo* pInfo  = SDL_GetVideoInfo();
	if(pInfo == NULL)
	{
		DP("[GfxSDL]Error: %s \n",SDL_GetError());
		return E_FAIL;
	}
	memcpy(&m_videoInfo, pInfo, sizeof(m_videoInfo));
	
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	if(pdwFourCC)
		m_pdwFourCCList = pdwFourCC;
	else
		m_pdwFourCCList = const_dwFourCC;

	
	DP("[GFXSDL]Open w:%d h:%d dwBuffers:%d \n",m_dwWidth, m_dwHeight, dwBuffers);
	pt.x = m_rectDst.left;
	pt.y = m_rectDst.top;
	m_bScnClip = TRUE;
	SetRect(&m_rectScn,0,0,m_videoInfo.current_w, m_videoInfo.current_h);
	SetRect(&m_rectSrc,0,0,m_dwWidth,m_dwHeight);
	return CreateSurfaces(dwBuffers);
}

STDMETHODIMP CGfxSDL::Close()
{
	DP("[GFXSDL]Close \n");

	ReleaseSurfaces();
	m_dwNumMonitors = 0;
	SDL_Quit();
	Sleep(100);
	return S_OK;
}

STDMETHODIMP CGfxSDL::CreateSurfaces(DWORD dwBuffers)
{
	int i;
	DWORD rmask, gmask, bmask, amask;

	if(m_dwWidth==0 || m_dwHeight==0 || m_pdwFourCCList==0 || m_pBackBuffer)
		return E_FAIL;
	for(i=0;m_dwFourCC=m_pdwFourCCList[i];i++)
	{
		switch(m_dwFourCC)
		{
		case MAKEFOURCC('R','5','6','5'):
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xf100;
			gmask = 0x07e0;
			bmask = 0x001f;
			amask = 0;
#else
			rmask = 0x001f;
			gmask = 0x07e0;
			bmask = 0x00f1;
			amask = 0;
#endif

			m_dwBpp = 16;
			m_surface_type = RGB_SURFACE;
			goto found;
#if 0
			// needs to be implemented
		case MAKEFOURCC('R','2','4',' '):
			pBMIColors[0] = 0x00FF0000;
			pBMIColors[1] = 0x0000FF00;
			pBMIColors[2] = 0x000000FF;
			m_dwBpp = 24;
			goto found;
#endif
		break;
		case MAKEFOURCC('Y','V','1','2'):
			m_surface_type = YUV_SURFACE;
			goto found;
		default:
			break;

		}
	}
found:
	if(m_dwFourCC==0)
		return E_FAIL;

	if(m_surface_type == YUV_SURFACE)
	{
		m_pOverlaySurf = new SDL_Overlay*[dwBuffers];
		if(m_pOverlaySurf == 0)
			return E_FAIL;
		memset(m_pOverlaySurf,0, sizeof(SDL_Overlay*[dwBuffers]));
	}

	if(m_surface_type == RGB_SURFACE)
	{
		m_pBackBuffer = new SDL_Surface*[dwBuffers];
		if(m_pBackBuffer==0)
			return E_FAIL;
		memset(m_pBackBuffer ,0, sizeof(SDL_Overlay*[dwBuffers]));
	}

	DWORD flags = SDL_HWSURFACE|SDL_DOUBLEBUF ;
	#ifdef __linux__
 	flags |= SDL_NOFRAME;
	#else
 	flags |= SDL_RESIZABLE;
	#endif
	if(!SDL_VideoModeOK(m_dwWidth, m_dwHeight,16, flags))
		DP("[GfxSDL] %s \n",SDL_GetError());
		
        if((m_screen = SDL_SetVideoMode(m_dwWidth, m_dwHeight, 16, flags)) == NULL)
		DP("[GfxSDL] %s \n",SDL_GetError());


	SDL_VERSION( &m_wminfo.version );
	SDL_GetWMInfo ( &m_wminfo);

	SetDstRect(&m_rectSrc);
	
	for(i=0;i<(signed)dwBuffers;i++)
	{
		if(m_surface_type == RGB_SURFACE)
		{
			if((m_pBackBuffer[i]=SDL_AllocSurface(SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_SRCCOLORKEY|SDL_SRCALPHA, m_dwWidth, m_dwHeight, m_screen->format->BitsPerPixel,
				m_screen->format->Rmask, m_screen->format->Gmask, m_screen->format->Bmask,m_screen->format->Amask)) == NULL)
			{
				DP("[GfxSDL]Error: %s \n",SDL_GetError());
				break;
			}
		}
		else if(m_surface_type == YUV_SURFACE)
		{
			if((m_pOverlaySurf[i]=SDL_CreateYUVOverlay( m_dwWidth, m_dwHeight, m_dwFourCC, m_screen)) == NULL)
			{
				DP("[GfxSDL]SDL_CreateYUVOverlay Error: %s \n",SDL_GetError());
				break;
			}
			DP("buf[%d]:%d \n", i, m_pOverlaySurf[i]);


			SDL_Rect rDst;
			rDst.x = rDst.y =0;
			rDst.w = m_dwWidth;
			rDst.h = m_dwHeight;
			SDL_DisplayYUVOverlay(m_pOverlaySurf[i], &rDst);
		}

	}
	m_dwBackBuffers = i;
	DP("%d buffers is created \n",m_dwBackBuffers );
	if(i==0)
	{
		ReleaseSurfaces();
		return E_FAIL;
	}
	if(m_surface_type == RGB_SURFACE)
		DP("m_surface_type = RGB_SURFACE \n");
	else
		DP("m_surface_type = YUV_SURFACE \n");

	RECT rDst;
	SetRect(&rDst,0,0,m_dwWidth, m_dwHeight);
	SetDstRect(&rDst);
	DP("Open success \n");
	return S_OK;
}

STDMETHODIMP CGfxSDL::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;
	DP("[GFXSDL]Set SourceRect (%d,%d, %d, %d) \n", m_rectSrc.left, m_rectSrc.top, m_rectSrc.right, m_rectSrc.bottom);
	return Update();
}

STDMETHODIMP CGfxSDL::SetDstRect(const RECT *pRect)
{
	if(pRect->left == 0 && pRect->top == 0 && pRect->right == 0 &&pRect->bottom == 0 )
		return S_OK;
	
	if(EqualRect(pRect,&m_rectDst))
		return S_OK;

	DP("CGfxSDL::SetDstRect %d,%d,%d,%d\n",pRect->left, pRect->top, pRect->right , pRect->bottom);
	m_rectDst = *pRect;

	DWORD flags = SDL_HWSURFACE|SDL_DOUBLEBUF;
	#ifdef __linux__
 	flags |= SDL_NOFRAME;
	#else
 	flags |= SDL_RESIZABLE;
	#endif

	if(SDL_VideoModeOK(m_rectDst.right - m_rectDst.left, m_rectDst.bottom - m_rectDst.top,16, flags))
	{
		m_screen = SDL_SetVideoMode(m_rectDst.right - m_rectDst.left, m_rectDst.bottom - m_rectDst.top, 16, flags);
	}

	#ifdef __linux__
/*
	RECT zeroRect = {0,0,0,0};
	if(memcmp(&zeroRect, &m_rectDst,sizeof(RECT)) != 0)
	{
		Window root;
		Window parent;
		Window *children;
		unsigned int children_count;

		if (m_wminfo.subsystem == SDL_SYSWM_X11) 
		{
			XQueryTree(	m_wminfo.info.x11.display,
					m_wminfo.info.x11.window,
					&root,
					&parent,
					&children,
					&children_count);

			m_wminfo.info.x11.lock_func();
			XMoveResizeWindow(m_wminfo.info.x11.display, parent, m_rectDst.left, m_rectDst.top, m_rectDst.right - m_rectDst.left, m_rectDst.bottom - m_rectDst.top);
			m_wminfo.info.x11.unlock_func();

			if( children ) XFree(children);
		}

	}
	else
		return E_FAIL;
*/
	#else
		if(m_hwnd == NULL)
		{
		  HWND hwnd = m_wminfo.window;
		  SetWindowPos( hwnd, HWND_TOP, m_rectDst.left, m_rectDst.top , m_rectDst.right - m_rectDst.left, m_rectDst.bottom - m_rectDst.top,0 );
		}
	#endif
	
	Update();

	DP("CGfxSDL::SetDstRect Quit\n");

	return S_OK;
}

STDMETHODIMP CGfxSDL::SetScnRect(const RECT *pRect)
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
	DP("[GFXSDL]SetScnRect (%d,%d,%d,%d) \n", m_rectScn.left, m_rectScn.top, m_rectScn.right, m_rectScn.bottom);
	return Update();
}

STDMETHODIMP CGfxSDL::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn ;
	return S_OK;
}

STDMETHODIMP CGfxSDL::SetAlpha(DWORD dwAlpha)
{
	SDL_Surface* pSurf = NULL;

	for(DWORD i = 0;i<m_dwBackBuffers;i++)
	{
		pSurf = GetRGBSurface(i);
		if(pSurf == NULL)
			return E_FAIL;
		if(dwAlpha == 0)	//Disable alpha
			SDL_SetAlpha(pSurf, 0,0);
		else
			SDL_SetAlpha(pSurf, SDL_SRCALPHA,(Uint8)dwAlpha);
	}

	return S_OK;
}

STDMETHODIMP CGfxSDL::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	SDL_Surface* pSurf = NULL;
	if(m_surface_type != RGB_SURFACE)
		return E_FAIL;
	
	if(m_bSrcCkey!=bEnable)
	{
		m_bSrcCkey = bEnable;
		if(bEnable)
		{
			m_dwSrcCkey = dwSrcCkey;
		}

		for(DWORD i = 0;i<m_dwBackBuffers;i++)
		{
			pSurf = GetRGBSurface(i);
			if(pSurf == NULL)
				return E_FAIL;
			if(bEnable)
				SDL_SetColorKey(pSurf, SDL_SRCCOLORKEY, m_dwSrcCkey);
			else
				SDL_SetColorKey(pSurf, 0, m_dwSrcCkey);
		}
	
	}
	else if(bEnable && m_dwSrcCkey!=dwSrcCkey)
	{
		m_dwSrcCkey = dwSrcCkey;
		for(DWORD i = 0;i<m_dwBackBuffers;i++)
		{
			pSurf = GetRGBSurface(i);
			if(pSurf == NULL)
				return E_FAIL;
			if(bEnable)
				SDL_SetColorKey(pSurf, SDL_SRCCOLORKEY, m_dwSrcCkey);
		}
	}
	return S_OK;
}

STDMETHODIMP CGfxSDL::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSDL::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSDL::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSDL::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSDL::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

SDL_Overlay*  CGfxSDL::GetYUVSurface(LONG num)
{	
	if(num>=0 && num<(signed)m_dwBackBuffers)
		return m_pOverlaySurf[num];
	return 0;
}

SDL_Surface*  CGfxSDL::GetRGBSurface(LONG num)
{	
	if(num>=0 && num<(signed)m_dwBackBuffers)
		return m_pBackBuffer[num];
	return 0;
}

STDMETHODIMP CGfxSDL::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(m_surface_type == RGB_SURFACE)
	{
		SDL_Surface* pSurf = NULL;
		pSurf = GetRGBSurface(num);
		if(pSurf == NULL)
			return E_FAIL;

		if(SDL_MUSTLOCK(pSurf))
		{
			if(SDL_LockSurface(pSurf)!=0)
			{
				DP("[GfxSDL]Error: %s \n",SDL_GetError());
				return E_FAIL;
			}
		}
		if(ppBuf)
			*ppBuf = pSurf->pixels;
		if(pStride)
			*pStride = pSurf->pitch;//m_dwWidth*m_dwBpp >> 3;
	}

	if(m_surface_type == YUV_SURFACE)
	{
		SDL_Overlay* pSurf = NULL;
		pSurf = GetYUVSurface(num);
		if(pSurf == NULL)
		{
			return E_FAIL;
		}
		if(SDL_LockYUVOverlay(pSurf)!=0)
		{
			DP("[GfxSDL]Error: %s \n",SDL_GetError());
			return E_FAIL;
		}
		if(ppBuf)
			*ppBuf = *(pSurf->pixels);
		if(pStride)
			*pStride = *(pSurf->pitches);//m_dwWidth*m_dwBpp >> 3;
	}

	
	if(pPixCount)
		*pPixCount = m_dwBpp;

 	return S_OK;
}

STDMETHODIMP CGfxSDL::Unlock(LONG num)
{
	if(m_surface_type == RGB_SURFACE)
	{
		SDL_Surface* pSurf = NULL;

		pSurf = GetRGBSurface(num);
		if(pSurf == NULL)
			return E_FAIL;

		if(SDL_MUSTLOCK(pSurf))
			SDL_UnlockSurface(pSurf);
	}

	if(m_surface_type == YUV_SURFACE)
	{
		SDL_Overlay* pSurf = NULL;
		pSurf = GetYUVSurface(num);
		if(pSurf == NULL)
		{
			return E_FAIL;
		}
		SDL_UnlockYUVOverlay(pSurf);
	}
	return S_OK;
}

void CGfxSDL::DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

Uint32 CGfxSDL::ReadPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

SDL_Surface* CGfxSDL::SDL_ScaleSurface(SDL_Surface* Surface, Uint16 Width, Uint16 Height)
{
	if(!Surface || !Width || !Height)
	    return 0;

	SDL_Surface * pRet = SDL_CreateRGBSurface(Surface->flags, Width, Height, Surface->format->BitsPerPixel,
        Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);

	double  _stretch_factor_x = (static_cast<double>(Width)  / static_cast<double>(Surface->w)),
        _stretch_factor_y = (static_cast<double>(Height) / static_cast<double>(Surface->h));

	for(Sint32 y = 0; y < Surface->h; y++) //Run across all Y pixels.
	        for(Sint32 x = 0; x < Surface->w; x++) //Run across all X pixels.
	            for(Sint32 o_y = 0; o_y < _stretch_factor_y; ++o_y) //Draw _stretch_factor_y pixels for each Y pixel.
	                for(Sint32 o_x = 0; o_x < _stretch_factor_x; ++o_x) //Draw _stretch_factor_x pixels for each X pixel.
	                    DrawPixel(pRet, static_cast<Sint32>(_stretch_factor_x * x) + o_x, 
	                        static_cast<Sint32>(_stretch_factor_y * y) + o_y, ReadPixel(Surface, x, y));

	return pRet;
}

STDMETHODIMP CGfxSDL::Flip(LONG num, DWORD dwFlags)
{
	SDL_Rect src,dst;
	memset(&src,0,sizeof(src));
	memset(&dst,0,sizeof(dst));
	src.w =dst.w = (Uint16)m_dwWidth;
	src.h = dst.h = (Uint16)m_dwHeight;

	dst.x = m_rectDst.left;
	dst.y = m_rectDst.top;
	dst.w = (Uint16)(m_rectDst.right - m_rectDst.left);
	dst.h = (Uint16)(m_rectDst.bottom - m_rectDst.top);

	if(m_surface_type == RGB_SURFACE)
	{
		SDL_Surface* pSurf = NULL;
		pSurf = GetRGBSurface(num);
		if(pSurf == NULL)
			return E_FAIL;
		if((dst.w >src.w)||(dst.h>src.h))	//Upscaling
		{
			pSurf = SDL_ScaleSurface(pSurf, dst.w, dst.h);
			if(pSurf == NULL)
				return E_FAIL;
			//SetRGBSurface(num,pSurf);
			if(SDL_BlitSurface(pSurf,&dst,m_screen, &dst) != 0)
			{
				DP("[GfxSDL] %s \n",SDL_GetError());
				return E_FAIL;		
			}

			SDL_FreeSurface(pSurf);


		}
		else
		{
			if(SDL_BlitSurface(pSurf,&src,m_screen, &dst) != 0)
			{
				DP("[GfxSDL] %s \n",SDL_GetError());
				return E_FAIL;		
			}
		}

		if(SDL_Flip(m_screen) != 0)
		{
			DP("[GfxSDL] %s \n",SDL_GetError());
			return E_FAIL;		
		}

	}

	if(m_surface_type == YUV_SURFACE)
	{
		SDL_Overlay* pSurf = NULL;
		pSurf = GetYUVSurface(num);
		if(pSurf == NULL)
		{
			return E_FAIL;
		}
		if(SDL_DisplayYUVOverlay(pSurf, &dst) != 0)
		{
			DP("[GfxSDL] %s \n",SDL_GetError());
			return E_FAIL;		
		}
	}

	Sleep(10);
	return S_OK;	
}

STDMETHODIMP CGfxSDL::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;
	m_bShow = bShow;
	return Update();
}

STDMETHODIMP CGfxSDL::Update()
{
	RECT rs, rd;

	if(m_surface_type == YUV_SURFACE)
	{
		for(int i=0;i<m_dwBackBuffers;i++)
		{
			SDL_Rect rDst;
			rDst.x = m_rectDst.left;
			rDst.y = m_rectDst.top;
			rDst.w = (Uint16)(m_rectDst.right - m_rectDst.left);
			rDst.h = (Uint16)(m_rectDst.bottom - m_rectDst.top);
			SDL_DisplayYUVOverlay(m_pOverlaySurf[i], &rDst);
		}
	}


	SetRect(&rs,0,0,m_dwWidth,m_dwHeight);
	IntersectRect(&rs,&rs,&m_rectSrc);

	rd = m_rectDst;

	if(m_hwnd)
	{
		POINT p;

		ZeroMemory(&p,sizeof(p));
#ifdef __linux__
		XWindowAttributes xProp;
		XGetWindowAttributes(m_wminfo.info.x11.display, m_hwnd, &xProp);
		p.x = xProp.x;
		p.y = xProp.y;
#else
		ClientToScreen(m_hwnd,&p);
#endif
		OffsetRect(&rd,-p.x,-p.y);
	}

	if(((DWORD)(m_rectSrc.right - m_rectSrc.left) != m_dwWidth) || ((DWORD)(m_rectSrc.bottom - m_rectSrc.top) != m_dwHeight))
	{
		SDL_Rect clip;
		memset(&clip, 0,sizeof(clip));
		clip.x = (Uint16)m_rectSrc.left;
		clip.y = (Uint16)m_rectSrc.top;
		clip.w = (Uint16)(m_rectSrc.right - m_rectSrc.left);
		clip.h = (Uint16)(m_rectSrc.bottom - m_rectSrc.top);
		if(SDL_SetClipRect(m_screen, &clip) != SDL_TRUE)
			DP("[GfxSDL]Error: %s \n",SDL_GetError());

	}
	else
		SDL_SetClipRect(m_screen, NULL);



	m_rectBltSrc = rs;

	return S_OK;
}

STDMETHODIMP CGfxSDL::ReleaseSurfaces()
{
	int i;

	m_bLastShow = FALSE;
	m_bShow = FALSE;
	m_bDstTrueCkeyValid = FALSE;
	if(m_pBackBuffer)
	{
		for(i=0;i<(signed)m_dwBackBuffers;i++)
		{
			if(m_pBackBuffer[i])
			{
				SDL_FreeSurface(m_pBackBuffer[i]);
				m_pBackBuffer[i]=0;
			}
		}
		delete[] m_pBackBuffer;
		m_pBackBuffer = 0;
	}


	if(m_pOverlaySurf)
	{
		for(i=0;i<(signed)m_dwBackBuffers;i++)
		{
			if(m_pOverlaySurf[i])
			{
				SDL_FreeYUVOverlay(m_pOverlaySurf[i]);
				m_pOverlaySurf[i]=0;
			}
		}
		delete[] m_pOverlaySurf;
		m_pOverlaySurf = 0;
		
	}
	m_dwBackBuffers = 0;
	return S_OK;
}

STDMETHODIMP CGfxSDL::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	DWORD dwFlags;
	RECT r;
	if(pSrc==0 || pDst==0 || pdwFlags==0)
		return E_FAIL;
	r = m_rectSrc;
	dwFlags = 0;
	if(m_rectSrc.right-m_rectSrc.left > m_rectDst.right-m_rectDst.left)
	{	// downscale issue (horizontal)
		dwFlags |= GFXOVERLAY_SCALE_X;
		r.right = m_rectSrc.left + m_rectDst.right-m_rectDst.left;
	}
	if(m_rectSrc.bottom-m_rectSrc.top > m_rectDst.bottom-m_rectDst.top)
	{	//downscale issue (vertical)
		dwFlags |= GFXOVERLAY_SCALE_Y;
		r.bottom = m_rectSrc.top + m_rectDst.bottom-m_rectDst.top;
	}
	*pSrc = m_rectSrc;
	*pDst = r;
	*pdwFlags = dwFlags;
	return S_OK;
}

STDMETHODIMP CGfxSDL::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return S_OK;
}

STDMETHODIMP CGfxSDL::Restore()
{
	m_bLastShow = FALSE;
	m_bShow = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxSDL::GetDDCaps(LONG*pCaps)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSDL::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxSDL::GetSurfaceInfo(GfxOverlayInfo *pInfo)
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

STDMETHODIMP CGfxSDL::IsOpen()
{
	return m_pBackBuffer ? S_OK : S_FALSE;
}

IMcGfxOverlay *CreateOverlaySDL()
{
	return new CGfxSDL;
}

#endif
