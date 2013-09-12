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

#include "../../inc/GfxBuildDef.h"

#ifdef TITAN_II
#include <windows.h>
#include "GfxOverlay.h"
#include "..\..\3rdparty\TitanII\inc\RendererSDK.h"
#include "..\..\3rdparty\TitanII\inc\BHSurfMgr.h"
#include "../../inc/McCom.h"

//#pragma comment(lib, "..\\..\\3rdparty\\TitanII\\lib\\BHVidRenderer.lib")
//#pragma comment(lib, "..\\..\\3rdparty\\TitanII\\lib\\BHSurfMgr.lib")

typedef VR_RESULT	(*CVR_CreateVideoRender_fxn)(VR_HANDLE *pHandle,  VR_HWND hWnd);
typedef VR_RESULT	(*CVR_DeleteVideoRender_fxn)(VR_HANDLE handle);
typedef VR_RESULT	(*CVR_SetVideoFormat_fxn)(VR_HANDLE handle, VR_SURF_FORMATID format_id, int width,	int height,   VR_SRCRECT* pSrcRect,	 int* LumPitch,   int* ChromPitch);
typedef VR_RESULT	(*CVR_SetDisplayMode_fxn)(VR_HANDLE handle,  VR_DISP_MODE_DESC mode);
typedef VR_RESULT	(*CVR_SetDstRect_fxn)(VR_HANDLE handle, VR_DSTRECT  rect);
typedef VR_RESULT	(*CVR_Render_fxn)(VR_HANDLE handle,   VR_FRAME_SURFACE* pSurf,  VR_REN_SYNC_MODE WaitMode);
typedef VR_RESULT	(*CVR_WaitComplete_fxn)(VR_HANDLE handle);
typedef VR_RESULT	(*CVR_CreateSurface_fxn)(VR_HANDLE handle,VR_FRAME_SURFACE** ppSurf,	int bPhysical);
typedef VR_RESULT	(*CVR_DestroySurface_fxn)(VR_HANDLE handle,	VR_FRAME_SURFACE* pSurf);
typedef VR_RESULT	(*CVR_LockSurface_fxn)(VR_HANDLE handle, VR_FRAME_SURFACE* pSurf);
typedef VR_RESULT	(*CVR_UnlockSurface_fxn)(VR_HANDLE handle,VR_FRAME_SURFACE* pSurf);

typedef VR_RESULT (*CVR_SetDeinterlaceMode_fxn)(VR_HANDLE handle,  VR_DEINTERLACE_MODE mode);	
typedef VR_RESULT (*CVR_SetRenderLayer_fxn)(VR_HANDLE handle, VR_RENDER_LAYER layer,  double fAlpha);

typedef VR_RESULT (*CVR_SetOSDRegionNum_fxn)(VR_HANDLE handle, int num );
typedef VR_RESULT (*CVR_SetOSDInfo_fxn)(VR_HANDLE handle,	 int region_num, PVR_OSD_INFO inf );

typedef SX_UINT32 (*BSM_Init_fxn)(VR_HANDLE handle);
typedef void (*BSM_DeInit_fxn)();
typedef void (*BSM_OutputSurf_fxn)(SXMemSurfDesc* surf, SX_REN_SYNC_MODE WaitMode);
typedef VR_FRAME_SURFACE* (*CvtToRendererSurf_fxn)(SXMemSurfDesc* surf);

typedef SX_UINT32 (*BSM_LockSurf_fxn)(SXMemSurfDesc* surf);
typedef SX_UINT32 (*BSM_UnlockSurf_fxn)(SXMemSurfDesc* surf);
typedef VR_FRAME_SURFACE *LPVR_FRAME_SURFACE;

class CGfxTitanII_OSD : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxTitanII_OSD();
	virtual ~CGfxTitanII_OSD();
	STDMETHODIMP Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst = NULL);		
	STDMETHODIMP Close();
	STDMETHODIMP Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount);
	STDMETHODIMP Unlock(LONG num);
	STDMETHODIMP Flip(LONG num, DWORD dwFlags);
	STDMETHODIMP Show(BOOL bShow);
	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  

    STDMETHODIMP SetHWRenderSurf(LPVOID pSurf,int mode)
	{
		return E_FAIL;
	}
	STDMETHODIMP GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags)
	{
		return E_FAIL;
	}
	STDMETHODIMP SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
	{
		return E_FAIL;
	}
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
		if(pHandle == NULL)
			return E_FAIL;
		*reinterpret_cast<VR_HANDLE*>(pHandle) = m_pVr;  
		return S_OK;
	}
	STDMETHODIMP SetVRHandle(LPVOID pHandle);
	STDMETHODIMP SetDeinterlaceMode(DWORD dwMode)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP GetDeinterlaceMode(DWORD* pdwMode)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}


protected:

	STDMETHODIMP CreateSurfaces(DWORD buffers);
	STDMETHODIMP ReleaseSurfaces();
	STDMETHODIMP IsSurfaceValid(LONG num);
	bool LoadRendererDll();
	void UnloadRendererDll();


	#define DECLEAR_RENDERER_FXN(fn) fn##_fxn m_##fn;
	DECLEAR_RENDERER_FXN(CVR_CreateVideoRender)
	DECLEAR_RENDERER_FXN(CVR_DeleteVideoRender)
	DECLEAR_RENDERER_FXN(CVR_SetVideoFormat)
	DECLEAR_RENDERER_FXN(CVR_SetDisplayMode)
	DECLEAR_RENDERER_FXN(CVR_SetDstRect)
	DECLEAR_RENDERER_FXN(CVR_Render)
	DECLEAR_RENDERER_FXN(CVR_WaitComplete)
	DECLEAR_RENDERER_FXN(CVR_CreateSurface)
	DECLEAR_RENDERER_FXN(CVR_DestroySurface)
	DECLEAR_RENDERER_FXN(CVR_LockSurface)
	DECLEAR_RENDERER_FXN(CVR_UnlockSurface)
	DECLEAR_RENDERER_FXN(CVR_SetDeinterlaceMode)
	DECLEAR_RENDERER_FXN(CVR_SetRenderLayer)
	DECLEAR_RENDERER_FXN(CVR_SetOSDRegionNum)
	DECLEAR_RENDERER_FXN(CVR_SetOSDInfo)
	DECLEAR_RENDERER_FXN(BSM_Init)
	DECLEAR_RENDERER_FXN(BSM_DeInit)
	DECLEAR_RENDERER_FXN(BSM_OutputSurf)
	DECLEAR_RENDERER_FXN(CvtToRendererSurf)
	DECLEAR_RENDERER_FXN(BSM_LockSurf)
	DECLEAR_RENDERER_FXN(BSM_UnlockSurf)

	RECT				m_rectSrc, m_rectDst;
	DWORD				m_dwWidth, m_dwHeight;
	DWORD				m_dwOsdLayer_num;
	BOOL				m_bShow, m_bLastShow, m_bDstCkey, m_bSrcCkey, m_bAlpha;
	DWORD				m_dwDstCkey, m_dwSrcCkey, m_dwDstTrueCkey;
	DWORD				m_dwAlpha;
	VR_HANDLE			m_pVr;
	void	**m_pBackBuffer;
	DWORD				m_dwOldWidth,m_dwOldHeight;
	VR_OSD_INFO m_osdInfo;
	const DWORD			*m_pdwFourCCList;
	bool	m_internal_hnd;
	HMODULE			m_hRendererDll;
	HMODULE			m_hSurfMgrDll;					

};

CGfxTitanII_OSD::CGfxTitanII_OSD()
{
	m_pVr = 0;
	m_pBackBuffer = 0;
	m_dwWidth = m_dwHeight = 0;
	m_dwOsdLayer_num = 0;
	m_bShow = m_bLastShow = m_bDstCkey = m_bSrcCkey =  m_bAlpha = FALSE;
	m_dwDstCkey = m_dwSrcCkey = 0;
	m_dwAlpha = 100;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_osdInfo,sizeof(m_osdInfo));
	m_dwOldWidth = m_dwOldHeight = 0;
	m_pdwFourCCList = NULL;
	m_internal_hnd = false;
}

CGfxTitanII_OSD::~CGfxTitanII_OSD()
{
	Close();


}

STDMETHODIMP CGfxTitanII_OSD::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	if(dwWidth==0 || dwHeight==0)
		return E_FAIL;

	LoadRendererDll();
	if(!m_pVr)
	{
		if(m_CVR_CreateVideoRender(&m_pVr, NULL) != VR_OK)
			return E_FAIL;
		m_internal_hnd = true;
	}
	else
		m_internal_hnd = false;

	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_pdwFourCCList = pdwFourCC;
	return CreateSurfaces(1);
}

STDMETHODIMP CGfxTitanII_OSD::Close()
{
	ReleaseSurfaces();
	if(m_internal_hnd  && m_pVr)
	{
		m_CVR_DeleteVideoRender(m_pVr);
		m_pVr = NULL;
	}
	UnloadRendererDll();
	return S_OK;
}

STDMETHODIMP CGfxTitanII_OSD::SetVRHandle(LPVOID pHandle)
{
	m_pVr = reinterpret_cast<VR_HANDLE*>(pHandle);
	if(!m_pVr)
		return E_FAIL;
	m_CVR_SetOSDRegionNum(m_pVr, m_dwOsdLayer_num);
	return S_OK;
}

STDMETHODIMP CGfxTitanII_OSD::CreateSurfaces(DWORD buffers)
{
	int i;

	if(m_dwWidth==0 || m_dwHeight==0)
		return E_FAIL;
	if(m_pVr && m_dwWidth==m_dwOldWidth && m_dwHeight==m_dwOldHeight)
		return S_OK;

	ReleaseSurfaces();

	m_dwOldWidth = m_dwWidth;
	m_dwOldHeight = m_dwHeight;

	m_rectSrc.left = 0;
	m_rectSrc.right = m_dwWidth;
	m_rectSrc.top = 0;
	m_rectSrc.bottom = m_dwHeight;
	m_osdInfo.src_width = m_dwWidth;
	m_osdInfo.src_height = m_dwHeight;

	m_pBackBuffer = new void*[buffers];
	ZeroMemory(m_pBackBuffer,sizeof(m_pBackBuffer[0])*buffers);

	for(i=0;i<(signed)buffers;i++)
	{
		void* pBuf = NULL;
		pBuf = malloc(4*m_dwWidth* m_dwHeight);
		if(pBuf == NULL)
			break;
		ZeroMemory(pBuf, (4*m_dwWidth* m_dwHeight));
		m_pBackBuffer[i] = pBuf;
	}
	
	if(i==0)
	{
	    delete[] m_pBackBuffer;
	    m_pBackBuffer = 0;
	    return E_FAIL;
	}

	m_dwOsdLayer_num = i;

	if(m_pVr)
		m_CVR_SetOSDRegionNum(m_pVr, m_dwOsdLayer_num);


	return S_OK;
}

STDMETHODIMP CGfxTitanII_OSD::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	if(pRect->left != 0 || pRect->top !=0)
		return E_FAIL;

	m_dwWidth = pRect->right;
	m_dwHeight = pRect->bottom;
	return (CreateSurfaces(m_dwOsdLayer_num));

}

STDMETHODIMP CGfxTitanII_OSD::SetDstRect(const RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	m_osdInfo.dest.left = pRect->left;
	m_osdInfo.dest.right = pRect->right;
	m_osdInfo.dest.top = pRect->top;
	m_osdInfo.dest.bottom = pRect->bottom;
	for(DWORD i=0;i<m_dwOsdLayer_num;i++)
		Flip(i,0);
	return S_OK;
}

STDMETHODIMP CGfxTitanII_OSD::SetScnRect(const RECT *pRect)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII_OSD::GetScnRect(RECT *pRect)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII_OSD::SetWindowHandle(HWND hwnd)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII_OSD::GetWindowHandle(HWND& hwnd)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII_OSD::SetAlpha(DWORD dwAlpha)	//Maxinum value is 100.
{
	m_dwAlpha = dwAlpha;
	if(m_bShow)
		m_osdInfo.alpha = (dwAlpha>100? 1:((float)dwAlpha/100));
	return S_OK;
}

STDMETHODIMP CGfxTitanII_OSD::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	if(bEnable)
		m_osdInfo.src_color_key = dwSrcCkey;
	else
		m_osdInfo.src_color_key = 0;
	return S_OK;
}

STDMETHODIMP CGfxTitanII_OSD::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII_OSD::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII_OSD::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII_OSD::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII_OSD::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII_OSD::IsSurfaceValid(LONG num)
{	
	if(num>=0 && num<(signed)m_dwOsdLayer_num)
	{
	    return S_OK;	//Valid buffer count
	}
	return E_FAIL;
}

STDMETHODIMP CGfxTitanII_OSD::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(FAILED(IsSurfaceValid(num)))
		return E_FAIL;

	if(ppBuf)
	    *ppBuf = m_pBackBuffer[num];
	if(pStride)
	    *pStride = m_osdInfo.src_width;

	return S_OK;
}

STDMETHODIMP CGfxTitanII_OSD::Unlock(LONG num)
{
	HRESULT hr = VR_OK;

	if(FAILED(IsSurfaceValid(num)))
		return E_FAIL;

	return S_OK;
}



STDMETHODIMP CGfxTitanII_OSD::Flip(LONG num, DWORD dwFlags)
{
	HRESULT hr;

	if(FAILED(IsSurfaceValid(num)))
		return E_FAIL;
	if(!m_pVr)
		return E_FAIL;

	m_osdInfo.iRegNum = 1+num;	//RegNum starts from 1
	m_osdInfo.bImgChanged = 1;
	m_osdInfo.pBitmap = (unsigned char*)m_pBackBuffer[num];
	hr = m_CVR_SetOSDInfo(m_pVr, m_osdInfo.iRegNum, &m_osdInfo);

	return hr==VR_OK ? S_OK : E_FAIL;
}

STDMETHODIMP CGfxTitanII_OSD::Show(BOOL bShow)
{
	HRESULT hr=VR_OK;
	if(m_bShow==bShow)
		return S_OK;
	m_bShow = bShow;

	if(m_bShow==FALSE)
	{
		m_osdInfo.alpha = 0;
	}
	else
	{
		m_osdInfo.alpha = (m_dwAlpha>100? 1:((float)m_dwAlpha/100));
	}
	
	m_osdInfo.bImgChanged = 0;

	if(!m_pVr)
		return E_FAIL;

	hr = m_CVR_SetOSDInfo(m_pVr, m_osdInfo.iRegNum, &m_osdInfo);
	return hr==VR_OK ? S_OK : E_FAIL;
}

STDMETHODIMP CGfxTitanII_OSD::ReleaseSurfaces()
{
	if(m_pBackBuffer)
	{
		int i;

		for(i=0;i<(signed)m_dwOsdLayer_num;i++)
		{
			if(m_pBackBuffer[i])
				delete m_pBackBuffer[i];
		}
		delete [] m_pBackBuffer;
		m_pBackBuffer = 0;
	}

    m_dwOldWidth = 0;
    m_dwOldHeight = 0;
	return S_OK;
}


STDMETHODIMP CGfxTitanII_OSD::GetDDCaps(LONG*pCaps)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII_OSD::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxTitanII_OSD::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if((m_pBackBuffer==0 ) || pInfo==0)
		return E_FAIL;
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;
	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwOsdLayer_num;
	pInfo->dwFourCC = MAKEFOURCC('X','R','G','B');
	pInfo->dwCaps = 0;
	return S_OK;
}

STDMETHODIMP CGfxTitanII_OSD::IsOpen()
{
	return m_pBackBuffer ? S_OK : S_FALSE;
}

#define LOAD_RENDERER_FXN(dll, fn) \
do {\
	m_##fn = (fn##_fxn)GetProcAddress(dll, L###fn);\
	if (!m_##fn)\
	{\
		RETAILMSG(1, (L"LoadRendererDll: function %s missed\r\n", L###fn));\
		return false;\
	}\
} while (0);


bool CGfxTitanII_OSD::LoadRendererDll()
{
	if (!m_hRendererDll)
	{
		m_hRendererDll = LoadLibrary(L"BHVidRenderer.dll");
		if (!m_hRendererDll)
		{
			RETAILMSG(1, (L"Load BHVidRenderer.dll Failed"));
			return false;
		}
		m_hSurfMgrDll = LoadLibrary(L"BHSurfMgr.dll");
		if (!m_hSurfMgrDll)
		{
			RETAILMSG(1, (L"Load BHSurfMgr.dll Failed"));
			FreeLibrary(m_hRendererDll);
			m_hRendererDll = NULL;
			return false;

		}
	}

	LOAD_RENDERER_FXN(m_hRendererDll, CVR_CreateVideoRender)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_DeleteVideoRender)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_SetVideoFormat)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_SetDisplayMode)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_SetDstRect)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_Render)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_WaitComplete)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_CreateSurface)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_DestroySurface)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_LockSurface)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_UnlockSurface)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_SetDeinterlaceMode)
	LOAD_RENDERER_FXN(m_hRendererDll, CVR_SetRenderLayer)
	LOAD_RENDERER_FXN(m_hSurfMgrDll, BSM_Init)
	LOAD_RENDERER_FXN(m_hSurfMgrDll, BSM_DeInit)
	LOAD_RENDERER_FXN(m_hSurfMgrDll, BSM_OutputSurf)
	LOAD_RENDERER_FXN(m_hSurfMgrDll, CvtToRendererSurf)
	LOAD_RENDERER_FXN(m_hSurfMgrDll, BSM_LockSurf)
	LOAD_RENDERER_FXN(m_hSurfMgrDll, BSM_UnlockSurf)

	return true;
	

}

#define UNLOAD_RENDERER_FXN(fn)  m_##fn = NULL;

void CGfxTitanII_OSD::UnloadRendererDll()
{
	UNLOAD_RENDERER_FXN(CVR_CreateVideoRender)
	UNLOAD_RENDERER_FXN(CVR_DeleteVideoRender)
	UNLOAD_RENDERER_FXN(CVR_SetVideoFormat)
	UNLOAD_RENDERER_FXN(CVR_SetDisplayMode)
	UNLOAD_RENDERER_FXN(CVR_SetDstRect)
	UNLOAD_RENDERER_FXN(CVR_Render)
	UNLOAD_RENDERER_FXN(CVR_WaitComplete)
	UNLOAD_RENDERER_FXN(CVR_CreateSurface)
	UNLOAD_RENDERER_FXN(CVR_DestroySurface)
	UNLOAD_RENDERER_FXN(CVR_LockSurface)
	UNLOAD_RENDERER_FXN(CVR_UnlockSurface)
	UNLOAD_RENDERER_FXN(CVR_SetDeinterlaceMode)
	UNLOAD_RENDERER_FXN(CVR_SetRenderLayer)
	UNLOAD_RENDERER_FXN(BSM_Init)
	UNLOAD_RENDERER_FXN(BSM_DeInit)
	UNLOAD_RENDERER_FXN(BSM_OutputSurf)
	UNLOAD_RENDERER_FXN(CvtToRendererSurf)
	UNLOAD_RENDERER_FXN(BSM_LockSurf)
	UNLOAD_RENDERER_FXN(BSM_UnlockSurf)
	if (m_hRendererDll)
	{
		FreeLibrary(m_hRendererDll);
		m_hRendererDll = NULL;
		FreeLibrary(m_hSurfMgrDll);
		m_hSurfMgrDll = NULL;
	}

	
}

IMcGfxOverlay *CreateOverlayTitanII_OSD()
{
	return new CGfxTitanII_OSD;
}

#endif
