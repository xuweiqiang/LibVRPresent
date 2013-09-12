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

#ifdef GFX_ANDROID_SF
#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"
#include "../../3rdparty/yuvrgb/y420rgbAPI.h"

#if defined(ANDROID20) || defined(ANDROID21)
#include <ui/ISurface.h>
#include <binder/MemoryHeapBase.h>
#else
#include <ISurface.h>
#include <MemoryHeapBase.h>
#endif

#include <MemoryBase.h>
#include <MemoryHeapBase.h>
#include <MemoryHeapPmem.h>

using namespace android;

#define ANDROID_ISURFACE_BACKBUFF_NUM 4
//#define LOG_FILENAME "/sdcard/logvrp"

//#define LOG2FILE
#ifdef LOG2FILE
//#undef DP
//#define DP(x...) \
//    do { fprintf(m_logfile,x); fflush(m_logfile);  } while(0)
#endif

/*
#undef DP
#define DP(x...) 
*/
#ifndef MAKEFOURCC
    #define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)
/*
#ifdef PLATOFRM_ANDROID_G2
#define SIM_SCN_WIDTH 480
#define SIM_SCN_HEIGHT 320
#define SURF_SCN_WIDTH SIM_SCN_WIDTH
#define SURF_SCN_HEIGHT SIM_SCN_HEIGHT
#elif defined(PLATOFRM_ANDROID_ACERNETBOOK)
#define SIM_SCN_WIDTH 1024
#define SIM_SCN_HEIGHT 600
#define SURF_SCN_WIDTH SIM_SCN_WIDTH
#define SURF_SCN_HEIGHT SIM_SCN_HEIGHT
#elif defined(PLATOFRM_DELL_MINI9)
#define SIM_SCN_WIDTH 800
#define SIM_SCN_HEIGHT 600
#define SURF_SCN_WIDTH SIM_SCN_WIDTH
#define SURF_SCN_HEIGHT SIM_SCN_HEIGHT
#elif defined(PLATOFRM_ANDROID_X2)
#define SIM_SCN_WIDTH 320
#define SIM_SCN_HEIGHT 400
#define SURF_SCN_WIDTH SIM_SCN_WIDTH
#define SURF_SCN_HEIGHT SIM_SCN_HEIGHT
#else
#define SIM_SCN_WIDTH 320
#define SIM_SCN_HEIGHT 400
#define SURF_SCN_WIDTH SIM_SCN_WIDTH
#define SURF_SCN_HEIGHT SIM_SCN_HEIGHT
#endif
*/
class CGfxAndroidSF : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxAndroidSF();
	virtual ~CGfxAndroidSF();
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
	STDMETHODIMP GetSurfaceInfo(GfxOverlayInfo *pInfo);
	STDMETHODIMP UIFlip(void*)
	{
		return S_OK;
	}

	STDMETHODIMP GetDDCaps(LPVOID *pCaps)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP IsOpen();
	STDMETHODIMP GetVRHandle(LPVOID pHandle)
	{
		return E_FAIL;
	}
	STDMETHODIMP SetWindowHandle(Window hwnd)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetWindowHandle(Window& hwnd)
	{
		return E_NOTIMPL;
	}

    STDMETHODIMP SetHWRenderSurf(LPVOID pSurf,int mode)
	{
		if(pSurf == NULL)
			return E_FAIL;
		
		if(m_bOpen == false)
			return E_FAIL;

		VR_ANDROID_ISUR_PARAM params;
		params = (*(VR_ANDROID_ISUR_PARAM *)pSurf);
		DP("SetHWRenderSurf: pSurf:%p\n",pSurf);


		EnterCriticalSection(&m_csDisplay);

		if (m_isurface) 
		{
		//	DP("unregisterBuffers 0x%x\n",m_isurface);

			m_isurface->unregisterBuffers();
			m_isurface = NULL;
		}
		
		m_isurface = (ISurface*)(params.pAndroidSFBuf);
    DP("m_isurface : %p \n",m_isurface );
		if(m_isurface == NULL)
		{
			LeaveCriticalSection(&m_csDisplay);
			return E_FAIL;
		}


      sp<MemoryHeapBase> heap;
      heap = new MemoryHeapBase("/dev/pmem_adsp", m_dwWidth*m_dwHeight*m_bytesPerPixel*ANDROID_ISURFACE_BACKBUFF_NUM);
      if (heap->heapID() < 0) {
          LOGI("Creating physical memory heap failed, reverting to regular heap.");
          heap = new MemoryHeapBase(m_dwWidth*m_dwHeight*m_bytesPerPixel*ANDROID_ISURFACE_BACKBUFF_NUM);
      } else {
      DP("Create phyical memory heap success \n");
          sp<MemoryHeapPmem> pmemHeap = new MemoryHeapPmem(heap);
          pmemHeap->slap();
          heap = pmemHeap;
      }
		if(heap == NULL){ DP("CGfxAndroidSF::Set m_FrameHeap == NULL\n"); }

		status_t status;
		m_buffers = ISurface::BufferHeap (m_dwWidth, m_dwHeight, m_dwWidth, 
			m_dwHeight, m_dpFormat, heap);

		DP("registerBuffers m_dwWidth:%d ,m_dwHeight:%d m_dpFormat: 0x%x float:%f\n",
          m_dwWidth,m_dwHeight,m_dpFormat,m_bytesPerPixel);
		status = m_isurface->registerBuffers(m_buffers);
		if(status)
		{
			m_isurface = NULL;
			DP("CGfxAndroidSF::Set isurface->registerBuffers error status %x\n", status);
			LeaveCriticalSection(&m_csDisplay);
			return E_FAIL;
		}
		DP("CGfxAndroidSF::SetHWRenderSurf  end\n");
		LeaveCriticalSection(&m_csDisplay);
		return S_OK;
	}
	STDMETHODIMP SetDeinterlaceMode(DWORD dwMode)
	{
		return E_NOTIMPL;
	}

	STDMETHODIMP GetDeinterlaceMode(DWORD* pdwMode)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP GetDDCaps(LONG* pCaps)
	{
		return E_NOTIMPL;
	}
	STDMETHODIMP SetVSYNC(DWORD dwMode){	return E_NOTIMPL;} 
	STDMETHODIMP GetVSYNC(DWORD* pdwMode){	return E_NOTIMPL;}  
	STDMETHODIMP SetDisplayMode(int disp_mode){return E_NOTIMPL;}

protected:
	
	RECT				m_rectSrc, m_rectDst;
	DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
	const DWORD			*m_pdwFourCCList;
	DWORD				m_dwBackBuffers;
	BOOL				m_bShow, m_bLastShow;
	HWND				m_hwnd;
	BOOL m_bOpen; 	

	FILE *m_logfile;
	ISurface* m_isurface;
	ISurface::BufferHeap m_buffers;
	int m_dpFormat;
	CRITICAL_SECTION m_csDisplay; 
	float m_bytesPerPixel;
};

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
	MAKEFOURCC('N','V','1','2'),
#ifndef SAMSUNG_ANDROID
	MAKEFOURCC('R','5','6','5'),
#endif
	0
};

CGfxAndroidSF::CGfxAndroidSF() : CMcUnknown(0,0)
{
	m_dwWidth = m_dwHeight = 0;
	m_dwBackBuffers = ANDROID_ISURFACE_BACKBUFF_NUM;
	m_bShow = m_bLastShow = TRUE;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	m_hwnd = 0;
	m_bOpen = FALSE;
	m_dwFourCC = 0;
	m_logfile = NULL;
	InitializeCriticalSection(&m_csDisplay);

	m_dpFormat = 0;
}

CGfxAndroidSF::~CGfxAndroidSF()
{
	Close();
 	DeleteCriticalSection(&m_csDisplay);
}

STDMETHODIMP CGfxAndroidSF::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	UINT32 CamIFState = 0;
	int res = 0;

	if(dwWidth==0 || dwHeight==0)
	{
		DP("[CGfxAndroidSF::Open] dwWidth==0 || dwHeight==0\n");
		return E_FAIL;
	}
	if(m_bOpen)
	{
		DP("[CGfxAndroidSF::Open] Invalid call \n");
		return E_FAIL;
	}

	m_dwFourCC = 0;
	m_dpFormat = 0;
	m_bytesPerPixel = 0.0f;

	m_isurface = NULL;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));

	SetRect(&m_rectSrc, 0, 0 ,m_dwWidth, m_dwHeight);

	if(pDst && (pDst->right - pDst->left != 0) && (pDst->bottom - pDst->top != 0))
	{
		m_rectDst = *pDst;
	}
	
	if(pdwFourCC)
		m_pdwFourCCList = pdwFourCC;
	else
		m_pdwFourCCList = const_dwFourCC;

	for(DWORD i=0;m_dwFourCC=m_pdwFourCCList[i];i++)
	{
		//D3DDISPLAYMODE dpMode;
		switch(m_dwFourCC)
		{
/*
			case MAKEFOURCC('Y','V','1','2'):
				m_dpFormat = HAL_PIXEL_FORMAT_YCbCr_420_P;
				m_bytesPerPixel = 1.5f;
				break;
			case MAKEFOURCC('N','V','1','2'):
				m_dpFormat = HAL_PIXEL_FORMAT_YCbCr_420_SP;
				m_bytesPerPixel = 1.5f;
				break;
*/
			case MAKEFOURCC('R','5','6','5'):
				m_dpFormat = PIXEL_FORMAT_RGB_565;
				m_bytesPerPixel = 2.0f;
				break;
  
			default:
				break;
		}
		if(m_dpFormat != 0)
			break;
	}
	DP("Open :%x ,w,h:%d,%d\n", m_dwFourCC, m_dwWidth, m_dwHeight);
	if(m_dpFormat  == 0)
	{
		DP("[CGfxAndroidSF::Open] m_dpFormat == 0 \n");
		return E_FAIL;
	}

	
	DP("[<<<<<<<<<<<<<<CGfxAndroidSF]Open w:%d h:%d \n",m_dwWidth, m_dwHeight);

	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CGfxAndroidSF::Close()
{
	DP("close \n");
	EnterCriticalSection(&m_csDisplay);
	if (m_isurface != NULL) 
	{
		//DP("unregisterBuffers 0x%x\n",m_isurface);
		m_isurface->unregisterBuffers();
		m_isurface = NULL;
	}
	LeaveCriticalSection(&m_csDisplay);

	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxAndroidSF::SetSrcRect(const RECT *pRect)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidSF::SetDstRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectDst) ){
		return S_OK;
	}
	
	DWORD width = pRect->right - pRect->left;
	DWORD height = pRect->bottom - pRect->top;

	if((width == 0) || (height ==0 )){
		DP("[<<<<<<<<<<<<<<CGfxAndroidSF]Set destRect (width == 0) || (height ==0 )\n");
		return E_FAIL;
	}
	
	m_rectDst = *pRect;
	DP("[<<<<<<<<<<<<<<CGfxAndroidSF]Set destRect (%d,%d, %d, %d) \n", m_rectDst.left, m_rectDst.top, m_rectDst.right, m_rectDst.bottom);
	
	return S_OK;
}

STDMETHODIMP CGfxAndroidSF::SetScnRect(const RECT *pRect)
{
	return S_OK;	
}

STDMETHODIMP CGfxAndroidSF::GetScnRect(RECT *pRect)
{
	return E_FAIL;
}

STDMETHODIMP CGfxAndroidSF::SetWindowHandle(HWND hwnd)
{
	m_hwnd = hwnd;
	return S_OK;
}

STDMETHODIMP CGfxAndroidSF::GetWindowHandle(HWND& hwnd)
{
	hwnd = m_hwnd;
	return S_OK;
}

STDMETHODIMP CGfxAndroidSF::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidSF::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidSF::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidSF::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidSF::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidSF::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidSF::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidSF::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if (m_isurface == NULL) return E_FAIL;
  if(num >= ANDROID_ISURFACE_BACKBUFF_NUM)  return E_INVALIDARG;
	EnterCriticalSection(&m_csDisplay);
	if(ppBuf)
		*ppBuf = (static_cast<BYTE*>(m_buffers.heap->base() )+ (int)(num*m_dwWidth*m_dwHeight*m_bytesPerPixel));
	
	if(pStride){
#ifndef ANDROID23
		if(m_dpFormat == HAL_PIXEL_FORMAT_YCbCr_420_P || 
		    m_dpFormat == HAL_PIXEL_FORMAT_YCbCr_420_SP)
			*pStride = m_dwWidth;
      else
 #endif
			*pStride = m_dwWidth*m_bytesPerPixel;
	}

	return S_OK;
}

STDMETHODIMP CGfxAndroidSF::Unlock(LONG num)
{
	LeaveCriticalSection(&m_csDisplay);
	return S_OK;
}

STDMETHODIMP CGfxAndroidSF::Flip(LONG num, DWORD dwFlags)
{
	if (m_isurface == NULL) return E_FAIL;
  if(num >= ANDROID_ISURFACE_BACKBUFF_NUM)  return E_INVALIDARG;
	EnterCriticalSection(&m_csDisplay);
	m_isurface->postBuffer(num*m_dwWidth*m_dwHeight*m_bytesPerPixel);
	LeaveCriticalSection(&m_csDisplay);
  	return S_OK;
}

STDMETHODIMP CGfxAndroidSF::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;

	m_bShow = bShow;
	return S_OK;
}

STDMETHODIMP CGfxAndroidSF::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidSF::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidSF::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if((pInfo==0) ||(pInfo->dwSize!=sizeof(*pInfo)))
		return E_FAIL;

	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
	pInfo->dwFourCC = m_dwFourCC;
	pInfo->dwCaps = 0; 

	return S_OK;
}

STDMETHODIMP CGfxAndroidSF::IsOpen()
{
	return m_bOpen;
}

IMcGfxOverlay *CreateAndroidSF()
{
	DP("[CGfxAndroidSF] CreateAndroidSF   \n");
	return new CGfxAndroidSF();
}
#endif

