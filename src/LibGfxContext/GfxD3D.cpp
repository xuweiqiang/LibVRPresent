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

#ifdef GFX_D3D
#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "TCHAR.h"

#include "GfxD3D_IDTPixelFormat.h"
#include "GfxD3D_IDTSurface.h"

#pragma comment(lib, "d3d9.lib")

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


#if ((_WIN32_WCE == 0x600)||(_WIN32_WCE == 0x502)||(_WIN32_WCE == 0x501))
	typedef HRESULT (* LPDIRECTDRAWENUMERATEEX)(LPDDENUMCALLBACKEX lpCallback, LPVOID lpContext, DWORD dwFlags);
#endif

#define SURFACELOST_RESTORE_TIMEOUT 5000
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct GfxMonitorInfo
{
	GUID guid;
	HMONITOR hm;
	MONITORINFO info;
};

static WNDCLASSEX wc_d3d = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
                  GetModuleHandle(NULL),NULL, LoadCursor(NULL, IDC_ARROW), NULL, NULL,
                  _T("D3D_Cidana"), NULL };

class CGfxD3D: public IMcGfxOverlay, public CMcUnknown
{
#define MONITORS_AVAIL_NUM	9
#define DISPMODE_NUM	36
public:
	DECLARE_IUNKNOWN

	CGfxD3D();
	virtual ~CGfxD3D();
	STDMETHODIMP Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst = NULL);	
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
		return E_FAIL;
	}
	STDMETHODIMP SetDeinterlaceMode(DWORD dwMode)
	{
		m_deInterlaceMode = dwMode;
		Update();
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
	STDMETHODIMP Update();
	STDMETHODIMP Blit(LONG num, DWORD dwFlags);
	STDMETHODIMP InitD3D(HWND hWnd);
	STDMETHODIMP UnInitD3D();
	STDMETHODIMP Restore();
	STDMETHODIMP DestroyInternalWnd();
	
	static DWORD WINAPI ThreadProc( LPVOID lpParameter);

	RECT				m_rectSrc, m_rectDst, m_rectScn,m_rectabsDst;
	RECT				m_rectBltSrc, m_rectBltDst;
	RECT				*m_prectSurfScale;
	DWORD				*m_pdwSurfScaleFlags;
	DWORD				m_dwWidth, m_dwHeight, m_dwBpp, m_dwFourCC;
	const DWORD			*m_pdwFourCCList;
	DWORD				m_dwBackBuffers;
	BOOL				m_bShow, m_bDstCkey, m_bSrcCkey, m_bScnClip, m_bDstTrueCkeyValid, m_bAlpha;
	DWORD				m_dwDstCkey, m_dwSrcCkey, m_dwDstTrueCkey;
	HWND				m_hwnd;
	GfxD3D_IDTSurface m_OffScreenSurf; // Surface to hold DX9logo Image
	D3DDISPLAYMODE d3ddm;
	LPDIRECT3D9             m_pD3D      ;	// Used to create the D3DDevice
	LPDIRECT3DDEVICE9       m_pD3DDevice ;	// Our rendering device
  //IDirect3D9Ex* m_pD3D;	
//IDirect3DDevice9Ex* m_pD3DDevice;
	BOOL	m_bOpen;
	BOOL	m_bInternalWnd;
	BOOL	m_bRestoreSucc;
	BOOL	m_bYV12OffPlaneSupp;
	DWORD m_deInterlaceMode;
	GfxD3D_IDTSurface m_DeinterlacedSurf; 
	RECT				m_rDeinterlacedSrc;
	DWORD m_flipFlag;
	DWORD 	m_preScale;

	HANDLE m_hThread;
    DWORD m_maxinum_fps;
    DWORD m_curr_fps;
    DWORD m_start_tick;
    DWORD m_render_frames;
};


//-----------------------------------------------------------------------------
// The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{		
	switch (msg)
	{
		case WM_TIMER:
		case WM_DESTROY:
			DestroyWindow(hWnd);
			PostQuitMessage(0);
			return 0;

		case WM_PAINT:
			ValidateRect(hWnd, NULL);
			return 0;

		case WM_SIZE:
			InvalidateRect(hWnd, NULL, TRUE);
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
	}

}


static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('R','5','6','5'),
	MAKEFOURCC('R','8','8','8'),
	0
};

DWORD WINAPI CGfxD3D ::ThreadProc( LPVOID lpParameter)
{
    CGfxD3D * pThis = (CGfxD3D*)lpParameter;

	pThis->m_hwnd = CreateWindow(_T("D3D_Cidana"), _T("D3D in Cidana!"), 
		WS_OVERLAPPEDWINDOW, 0, 0, pThis->m_dwWidth, pThis->m_dwHeight,
		NULL, NULL, wc_d3d.hInstance, 0);
	if(pThis->m_hwnd == NULL)
		return 0x80000001;

	MSG                         msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}

	return 0;
}	

CGfxD3D::CGfxD3D() : 
CMcUnknown(0,0)
{
	m_dwWidth = m_dwHeight = 0;
	m_dwBpp = 0;
	m_dwBackBuffers = 0;
	m_bShow = m_bDstCkey = m_bSrcCkey = m_bScnClip = m_bDstTrueCkeyValid = m_bAlpha = FALSE;
	m_dwDstCkey = m_dwSrcCkey = 0;
	m_pdwSurfScaleFlags = 0;
	m_prectSurfScale = 0;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectScn,sizeof(m_rectScn));
	ZeroMemory(&m_rectBltSrc,sizeof(m_rectBltSrc));
	ZeroMemory(&m_rectBltDst,sizeof(m_rectBltDst));
	ZeroMemory(&m_rDeinterlacedSrc,sizeof(m_rDeinterlacedSrc));
	ZeroMemory(&m_rectabsDst,sizeof(m_rectabsDst));


	m_hwnd = 0;
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_bOpen = FALSE;
	m_bInternalWnd = FALSE;
	ZeroMemory(&d3ddm,sizeof(d3ddm));
	// Register the window class
	RegisterClassEx(&wc_d3d);
	m_bRestoreSucc = TRUE;
	m_bYV12OffPlaneSupp = FALSE;
	m_flipFlag = GFXOVERLAY_FLIP_ODD;
	m_preScale = 1;
	m_hThread = 0;
}

CGfxD3D::~CGfxD3D()
{
}

STDMETHODIMP CGfxD3D::InitD3D(HWND hWnd)
{
	DWORD i=0;
	D3DFORMAT dpFormat = D3DFMT_UNKNOWN;
	// Create the D3D object, which is needed to create the D3DDevice.
//	if ((FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION,&m_pD3D)))) return E_FAIL;
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	ZeroMemory(&d3ddm,sizeof(d3ddm));
	// Get the current desktop display mode
	if (FAILED( m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm))) return E_FAIL;

	m_bYV12OffPlaneSupp = FALSE;

	if(SUCCEEDED(m_pD3D->CheckDeviceFormatConversion(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,(D3DFORMAT)MAKEFOURCC('Y','V','1','2'),D3DFMT_R5G6B5)))
		m_bYV12OffPlaneSupp = TRUE;

	D3DCAPS9 caps;
	memset(&caps, 0,sizeof(caps));
	HRESULT hr = m_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,&caps);
	if(SUCCEEDED(hr))
	{
		if(caps.MaxPrimitiveCount < m_dwWidth*m_dwHeight)
		{
			m_bYV12OffPlaneSupp = false;
		}
	}
	//Performance comes down on INTEL's Integrated Graphic Card. So use RGB offplain by fore.
	m_bYV12OffPlaneSupp = FALSE;

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

#ifdef DELL_NETBOOK
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
#endif
	d3dpp.BackBufferWidth = m_dwWidth*m_preScale;
	d3dpp.BackBufferHeight = m_dwHeight*m_preScale;
	d3dpp.BackBufferCount = m_dwBackBuffers;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; // We want to be able to directly write to backbuffer memory
	for( i=0;m_dwFourCC=m_pdwFourCCList[i];i++)
	{
		D3DDISPLAYMODE dpMode;

		if(m_bYV12OffPlaneSupp&& m_dwFourCC == MAKEFOURCC('Y','V','1','2'))
			dpFormat = D3DFMT_R5G6B5;
		else
		{
			switch(m_dwFourCC)
			{
				case MAKEFOURCC('R','5','6','5'):
					dpFormat = D3DFMT_R5G6B5;
					break;
				case MAKEFOURCC('R','8','8','8'):
					dpFormat = D3DFMT_X8R8G8B8;
					break;
				case MAKEFOURCC('Y','U','Y','2'):
					dpFormat = D3DFMT_YUY2;
					break;
				default:
					break;
			}
		}
		HRESULT hr = m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,dpFormat,0,&dpMode);
		if(SUCCEEDED(hr))
		{
			d3dpp.BackBufferFormat = dpFormat;
		      m_maxinum_fps = dpMode.RefreshRate;
			break;
		}
	}

	if(d3dpp.BackBufferFormat == NULL)	//No one is init successfully
	{
		if(m_pD3D)
		{
			m_pD3D ->Release();
			m_pD3D = NULL;
		}
		DP("[GFXD3D]Format are not supported!");
		return E_FAIL;
	}

	// Create D3D Device
	BOOL bSucc = FALSE;
	while(d3dpp.BackBufferCount&&(bSucc == FALSE))
	{
//	  D3DDISPLAYMODEEX playmode;
 //   memset(&playmode, 0 ,sizeof(playmode));
  //  playmode.
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
//						&d3dpp, NULL,&m_pD3DDevice)))
		{
			d3dpp.BackBufferCount--;
		}
		else
			bSucc = TRUE;
	}
	
	if (bSucc == FALSE)
	{
		DP("[GFXD3D]Could not create D3D device!");
		if(m_pD3D)
		{
			m_pD3D ->Release();
			m_pD3D=NULL;
		}
		return E_FAIL;
	}

	switch(dpFormat)
	{
		case D3DFMT_R5G6B5:
			DP("[GFXD3D]Internal D3D R565 MainSurf ");
			break;
		case D3DFMT_R8G8B8:
			DP("[GFXD3D]Internal D3D  R888 MainSurf  ");
			break;
		case D3DFMT_YUY2:
			DP("[GFXD3D]Internal D3D YUY2  MainSurf ");
			break;
		default:
			DP("[GFXD3D]Internal D3D UNKNOWN MainSurf  ");
			break;
	}

	m_dwBackBuffers = d3dpp.BackBufferCount;

	// Load image to surface
	int ret  = m_OffScreenSurf.Initialize(m_pD3DDevice);
	if (ret != DT_OK)
		DP("[GFXD3D]Could not initial offscreen surface!");

	ret  = m_DeinterlacedSurf.Initialize(m_pD3DDevice);
	if (ret != DT_OK)
		DP("[GFXD3D]Could not initial offscreen surface!");

	if(m_bYV12OffPlaneSupp&& m_dwFourCC == MAKEFOURCC('Y','V','1','2'))
		dpFormat = (D3DFORMAT)m_dwFourCC;

	ret = m_OffScreenSurf.CreateSurface(m_dwWidth,m_dwHeight ,dpFormat);
	if (ret != DT_OK)
	{
		DP("[GFXD3D] Could not create offscreen surface!");
		return E_FAIL;
	}

	ret = m_DeinterlacedSurf.CreateSurface(m_dwWidth*2,m_dwHeight/2,dpFormat);
	if (ret != DT_OK)
	{
		DP("[GFXD3D] Could not create offscreen surface!");
		return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP CGfxD3D::UnInitD3D()
{
	HRESULT Res = 0;

	Res = m_DeinterlacedSurf.DestroySurface();
	if(FAILED(Res))
		DP("[GFXD3D]UnInitD3D() failed off_destoy!");

	Res = m_DeinterlacedSurf.UnInitiallize();
	if(FAILED(Res))
		DP("[GFXD3D]UnInitD3D() failed off_uninit!");

		
	Res = m_OffScreenSurf.DestroySurface();
	if(FAILED(Res))
		DP("[GFXD3D]UnInitD3D() failed off_destoy!");
		
	Res = m_OffScreenSurf.UnInitiallize();
	if(FAILED(Res))
		DP("[GFXD3D]UnInitD3D() failed off_uninit!");
	
	if(m_pD3DDevice)
	{
		Res = (int)m_pD3DDevice->Release();
		if(FAILED(Res))
			DP("m_pD3DDevice->Release return %u\n", Res);
		m_pD3DDevice = NULL;
	}
	if(m_pD3D)
	{
		Res = (int)m_pD3D->Release();
		if(FAILED(Res))
			DP("m_pD3D->Release return %u\n", Res);
		m_pD3D =NULL;
	}
	return S_OK;
}

STDMETHODIMP CGfxD3D::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
	const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)
{
	if(m_bOpen)
		return E_FAIL;

	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectabsDst,sizeof(m_rectabsDst));

	m_deInterlaceMode = GFXOVERLAY_DEINTERLACE_WEAVE;
	m_flipFlag = GFXOVERLAY_FLIP_ODD;
	
	if(pdwFourCC)
		m_pdwFourCCList = pdwFourCC;
	else
		m_pdwFourCCList = const_dwFourCC;
	DP("[GFXD3D]Open w:%d h:%d \n",m_dwWidth, m_dwHeight);
	m_bScnClip = TRUE;
	SetRect(&m_rectScn,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	m_dwBackBuffers = dwBuffers;
	
	// Create the application's window
	if(m_hwnd == NULL)
	{
		m_hThread = ::CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
		while(!m_hwnd){Sleep(10);}

		m_bInternalWnd = TRUE;
	}
	else
		m_bInternalWnd = FALSE;

	if(m_dwWidth <= 320 || m_dwHeight <= 240)
		m_preScale = 5;
	else
		m_preScale = 1;
		
	// Initialize Direct3D
	if (SUCCEEDED(InitD3D(m_hwnd)))
	{

		if(m_bInternalWnd)
		{
			// Show the window
			ShowWindow(m_hwnd, SW_HIDE);
			UpdateWindow(m_hwnd);
		}

	}
	else
	{
		DestroyInternalWnd();
		return E_FAIL;
	}

  m_start_tick = 0;
	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CGfxD3D::Close()
{
	if(m_bOpen == FALSE)
		return S_OK;
	DP("[GFXD3D] Close\n");
	HRESULT Res = UnInitD3D();
	m_bOpen = FALSE;
	DestroyInternalWnd();
	return Res;
}

STDMETHODIMP CGfxD3D::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;
	DP("[GFXD3D]Set SourceRect (%d,%d, %d, %d) \n", m_rectSrc.left, m_rectSrc.top, m_rectSrc.right, m_rectSrc.bottom);
	return Update();
}

STDMETHODIMP CGfxD3D::SetDstRect(const RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;

	if(m_hwnd && m_bInternalWnd)
	{
		if(memcmp(&m_rectabsDst, pRect,sizeof(m_rectabsDst)) ==0)
			return S_OK;
		m_rectabsDst = *pRect;
		SetWindowPos(m_hwnd,0,m_rectabsDst.left, m_rectabsDst.top, m_rectabsDst.right - m_rectabsDst.left, m_rectabsDst.bottom - m_rectabsDst.top ,0);

		RECT r;
		::GetClientRect(m_hwnd, &r);
		SetRect(&m_rectDst, 0,0,r.right - r.left, r.bottom- r.top);

	}
	else
	{
		m_rectDst = m_rectabsDst = *pRect;
		
		POINT p={0,0};
		::ClientToScreen(m_hwnd, &p);
		OffsetRect(&m_rectDst,-p.x,-p.y);

		RECT r;
		::GetClientRect(m_hwnd, &r);
		int dstWndWidth = r.right - r.left;
		int dstWndHeight = r.bottom - r.top;
		if(dstWndWidth && dstWndHeight)
		{
		       m_rectDst.left =  m_rectabsDst.left *m_dwWidth/dstWndWidth;
		       m_rectDst.right =  m_rectabsDst.right*m_dwWidth/dstWndWidth ;
		       m_rectDst.top =  m_rectabsDst.top*m_dwHeight/dstWndHeight ;
		       m_rectDst.bottom =  m_rectabsDst.bottom*m_dwHeight/dstWndHeight ;
		}
	}
	Update();
	if(m_bShow)
		Flip(0, 0);
	return S_OK;

#if 0
	RECT r;
	RECT inputRect;
	DWORD dwSrcWidth,dwSrcHeight;
	ZeroMemory(&r, sizeof(r));
	ZeroMemory(&inputRect, sizeof(inputRect));

	memcpy(&inputRect, pRect, sizeof(inputRect));


	::GetClientRect(m_hwnd, &r);
	dwSrcWidth = r.right - r.left;
	dwSrcHeight = r.bottom- r.top;

#if 0
	//Notice: To do check whether ClientToScreen() is screen coordinate system is correct.
	//Put absolute Coordinate system  to reference Coordinate system 
	//if(m_bInternalWnd)
	POINT p={0,0};
	::ClientToScreen(m_hwnd, &p);
	OffsetRect(&inputRect,-p.x,-p.y);
#endif

		if(dwSrcWidth)
		{
			m_rectDst.left = ((LONG)(inputRect.left*m_dwWidth)/(LONG)dwSrcWidth);
			m_rectDst.right = ((LONG)(inputRect.right*m_dwWidth)/(LONG)dwSrcWidth);
		}

		if(dwSrcHeight)
		{
			m_rectDst.top = ((LONG)(inputRect.top*m_dwHeight)/(LONG)dwSrcHeight);
			m_rectDst.bottom = ((LONG)(inputRect.bottom*m_dwHeight)/(LONG)dwSrcHeight);
		}

	DP("[GFXD3D]Set DestRect (%d,%d, %d, %d) \n", m_rectDst.left, m_rectDst.top, m_rectDst.right, m_rectDst.bottom,0);




#endif
}

STDMETHODIMP CGfxD3D::SetScnRect(const RECT *pRect)
{
	return S_OK;	// ignored
}

STDMETHODIMP CGfxD3D::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn ;
	return S_OK;
}
STDMETHODIMP CGfxD3D::SetWindowHandle(HWND hwnd)
{
	if(hwnd == NULL)	
		return S_OK;
	
	if(m_hwnd!=hwnd)
	{
		DestroyInternalWnd();
		m_bInternalWnd = FALSE;
		m_hwnd = hwnd;
	}
	return S_OK;
}

STDMETHODIMP CGfxD3D::GetWindowHandle(HWND& hwnd)
{
	hwnd  = m_hwnd;
	return S_OK;
}

STDMETHODIMP CGfxD3D::SetAlpha(DWORD dwAlpha)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxD3D::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
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

STDMETHODIMP CGfxD3D::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return E_FAIL;
}

STDMETHODIMP CGfxD3D::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_FAIL;
}

STDMETHODIMP CGfxD3D::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxD3D::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_FAIL;
}

STDMETHODIMP CGfxD3D::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}


STDMETHODIMP CGfxD3D::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(m_deInterlaceMode == GFXOVERLAY_DEINTERLACE_WEAVE)
	{
		if(m_OffScreenSurf.Lock(ppBuf, pStride) == DT_OK)
			return S_OK;
	}
	else
	{
		if(m_DeinterlacedSurf.Lock(ppBuf, pStride) == DT_OK)
			return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CGfxD3D::Unlock(LONG num)
{
	if(m_deInterlaceMode == GFXOVERLAY_DEINTERLACE_WEAVE)
	{
		if(m_OffScreenSurf.UnLock() == DT_OK)
			return S_OK;
	}
	else
	{
		if(m_DeinterlacedSurf.UnLock() == DT_OK)
			return S_OK;

	}
	return E_FAIL;
}

STDMETHODIMP CGfxD3D::Blit(LONG num, DWORD dwFlags)
{

	if(m_bRestoreSucc == FALSE)
	{
		if(FAILED(Restore()))
			return E_FAIL;
	}
	// Must be initialized
	if (m_pD3DDevice == NULL) return E_FAIL;

	// Clear the backbuffer to a black
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

	// No need to do beginscene and end scene since there's no 3D rendering going on
	if (m_bShow)
	{
		// Get backbuffer and render to it
		LPDIRECT3DSURFACE9 surf;
		if (SUCCEEDED(m_pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surf)))
		{

			RECT rect; rect.left = 0; rect.top=0; rect.right=m_dwWidth; rect.bottom=m_dwHeight;
		//BlitFast do 1.Lock the buffer. 2. draw buffer 3. unlock buffer by ourselves.  Do not use it.
		//	int ret = m_OffScreenSurf.BltFast(surf, 0, 0, m_rectBltSrc);
		int ret =  0;
		if(m_deInterlaceMode !=  GFXOVERLAY_DEINTERLACE_WEAVE)
		{
			m_flipFlag = dwFlags;
			Update();
			ret = m_DeinterlacedSurf.BltStretch(surf, m_rDeinterlacedSrc, m_rectBltDst);
		}
		else
			ret = m_OffScreenSurf.BltStretch(surf, m_rectBltSrc, m_rectBltDst);

			if (ret != DT_OK)
			{
				if (ret == DT_INVALID_FORMAT)
				{
					DP("Invalid pixelformat on source or destination surface! Rendering stopped.");
				}
				else if (ret == D3DERR_INVALIDCALL)
				{
					DP("Blit failed ::D3DERR_INVALIDCALL ");
				}
			}

			surf->Release();
		}
		else
		{
			DP("Could not get the backbuffer! Rendering stopped.");
			m_bShow = FALSE;
		}
	}

	// Present the backbuffer contents to the display
	DWORD ret = m_pD3DDevice->Present(NULL, NULL, m_hwnd, NULL); 
//	DWORD ret = m_pD3DDevice->PresentEx(NULL, NULL, m_hwnd, NULL, D3DPRESENT_DONOTWAIT); 

  if(FAILED(ret))
	{	 
		DP("D3D Device Present failed \n");
		if(ret == D3DERR_DEVICELOST)
		{
			Sleep(1000);		
			return E_ACCESSDENIED;
		}

//		return Restore();
	}

	return  S_OK;
}

STDMETHODIMP CGfxD3D::Restore()
{
	HRESULT ret=0;

	m_bOpen = FALSE;
	ret = UnInitD3D();
	if(FAILED(ret))
	{
		m_bRestoreSucc = FALSE;
		return ret;
	}

	ret = Open(m_dwWidth, m_dwHeight, m_dwBackBuffers, m_pdwFourCCList,0);
	if(FAILED(ret))
	{
		m_bRestoreSucc = FALSE;
		return ret;
	}
	m_bRestoreSucc = TRUE;
	return S_OK;
}

STDMETHODIMP CGfxD3D::Flip(LONG num, DWORD dwFlags)
{
 #ifndef DISABLE_SKIPPINGFRAME
  if(m_start_tick == 0 || GetTickCount() - m_start_tick > 5000)
  {
    m_start_tick  = GetTickCount();
    m_curr_fps = 0;
    m_render_frames = 0;
  }
  else
  {
    DWORD duration = (GetTickCount() - m_start_tick);
	if(duration )
	{
		m_curr_fps  = m_render_frames*1000/duration ; 

		if(m_curr_fps >= m_maxinum_fps - 2)
		{
			//DP(" Warning SkipFrame!! m_curr_fps %d,m_maxinum_fps %d \n",m_curr_fps ,m_maxinum_fps);
			return S_FALSE;
		}
		m_render_frames++;
	}
  }
  #endif
  return Blit(num,dwFlags);
}

STDMETHODIMP CGfxD3D::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;
	m_bShow = bShow;

	if(m_bInternalWnd)
	{
		if(m_bShow)
			ShowWindow(m_hwnd, SW_SHOW);
		else
			ShowWindow(m_hwnd, SW_HIDE);

		UpdateWindow(m_hwnd);
	}
	return Update();
}

STDMETHODIMP CGfxD3D::Update()
{
	RECT rs, rd;

	SetRect(&rs,0,0,m_dwWidth,m_dwHeight);
	IntersectRect(&rs,&rs,&m_rectSrc);
	rd = m_rectDst;
  
	RECT blitRect = {0,0,m_dwWidth*m_preScale,m_dwHeight*m_preScale};
	IntersectRect(&rd ,&rd ,&blitRect);

	m_rectBltSrc = rs;
	m_rectBltDst = rd;

	m_rectBltDst.right *=m_preScale;
	m_rectBltDst.left *=m_preScale ;
	m_rectBltDst.top *=m_preScale ;
	m_rectBltDst.bottom *=m_preScale;

	if(m_deInterlaceMode !=  GFXOVERLAY_DEINTERLACE_WEAVE)
	{
		DWORD widthOffset = 0;
		if(m_flipFlag == GFXOVERLAY_FLIP_ODD)
			widthOffset = 0;
		else
			widthOffset  = m_rectBltSrc.right -m_rectBltSrc.left;
		
		SetRect(&m_rDeinterlacedSrc,widthOffset  + m_rectBltSrc.left, m_rectBltSrc.top/2, widthOffset  + m_rectBltSrc.right, m_rectBltSrc.bottom/2);
			
	}

	m_rectBltDst.left &= 0xfffe;
	m_rectBltDst.right &= 0xfffe;
	m_rectBltDst.top &= 0xfffe;
	m_rectBltDst.bottom &= 0xfffe;

	m_rDeinterlacedSrc.top &= 0xfffe;
	m_rDeinterlacedSrc.bottom &= 0xfffe;

#if 0	
	if(m_rectBltDst.left)
	{
		DWORD width = m_rectBltDst.right - m_rectBltDst.left;
		width = (m_dwWidth-width)/2;
		m_rectBltDst.right -= m_rectBltDst.left;
		m_rectBltDst.left = 0;
		m_rectBltDst.right += width;
		m_rectBltDst.left = width;
		
	}

	if(m_rectBltDst.top)
	{
		DWORD height = m_rectBltDst.bottom - m_rectBltDst.top;
		height = (m_dwHeight-height)/2;
		m_rectBltDst.bottom -= m_rectBltDst.top;
		m_rectBltDst.top = 0;
		m_rectBltDst.bottom += height;
		m_rectBltDst.top = height;
	}
#endif
	return S_OK;
}


STDMETHODIMP CGfxD3D::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxD3D::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return E_FAIL;
}


STDMETHODIMP CGfxD3D::GetDDCaps(LONG *pCaps)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxD3D::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxD3D::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
	pInfo->dwFourCC = m_dwFourCC;
	pInfo->dwCaps = 0;
	return S_OK;
}

STDMETHODIMP CGfxD3D::IsOpen()
{
	return m_bOpen ? S_OK : S_FALSE;
}

STDMETHODIMP CGfxD3D::DestroyInternalWnd()
{
	if(m_hwnd && m_bInternalWnd)
	{
		::SendMessage(m_hwnd, WM_DESTROY, NULL, NULL);
		m_hwnd= NULL;
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
		m_hThread = NULL;

	}

	return S_OK;
}


IMcGfxOverlay *CreateOverlayD3D()
{
	return new CGfxD3D;
}
#endif
