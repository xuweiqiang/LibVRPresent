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

#ifdef GFX_ANDROID_ISURFACE_OVERLAY
#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"

#include <ISurface.h>
#include <hardware.h>

#if ANDROID_VER==23
#include "../../3rdparty/RENESAS_ANDROID/overlay23.h"
#else
#include "../../3rdparty/RENESAS_ANDROID/overlay.h"
#endif
#include <ui/Overlay.h>

#include <sys/ioctl.h>
#include <binder/MemoryHeapBase.h>
#include <binder/MemoryHeapPmem.h>
#include <binder/MemoryBase.h>
#include <linux/android_pmem.h>

using namespace android;
/*
struct overlay_buffer_emxx {
	int index;
	struct timeval time;
	void *vaddr;
	void *paddr;
	int w_src;
	int h_src;
};
*/

//#define INFOMAX
#ifndef MAKEFOURCC
    #define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)


#define IOC_SIZ_MAGIC           ('N')
#define EMXX_CIDANA_PHYSTART \
		_IOW(IOC_SIZ_MAGIC, 14, unsigned long)
#define EMXX_CIDANA_PHYSIZE \
		_IOW(IOC_SIZ_MAGIC, 15, unsigned long)
		
class CGfxAndroidISurfOverlay : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

#ifndef RENESAS_ANDROID
  #define ISURFACEOVL_BACKBUFF_NUM 1// 2
#else
  #define ISURFACEOVL_BACKBUFF_NUM 2
#endif

	CGfxAndroidISurfOverlay();
	virtual ~CGfxAndroidISurfOverlay();
	STDMETHODIMP Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst = NULL);		
	STDMETHODIMP Close();
	STDMETHODIMP Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount);
	STDMETHODIMP Unlock(LONG num);
	STDMETHODIMP Flip(LONG num, DWORD dwFlags);
	STDMETHODIMP Show(BOOL bShow);
	STDMETHODIMP GetSurfScaleRects(LONG num, RECT *pRSrcRect, RECT *pDstRect, DWORD *pdwFlags);
	STDMETHODIMP SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags){	return E_NOTIMPL;}
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
		DP("SetHWRenderSurf: pParam:%p\n",pSurf);

		EnterCriticalSection(&m_csDisplay);
		m_isurface = (ISurface*)(params.pAndroidSFBuf);
      DP("m_isurface : %p \n",m_isurface);
		if(m_isurface == NULL)
		{
			LeaveCriticalSection(&m_csDisplay);
			return E_FAIL;
		}


	if (m_pOverlay != NULL) {
		m_pOverlay->destroy();
//m_pOverlay.clear();
	}

		sp<OverlayRef> ref = m_isurface->createOverlay(m_dwWidth, m_dwHeight, m_dpFormat, 0);
		m_pOverlay = new Overlay(ref);


		overlay_buffer_t buf;
		for (int i = 0; i < ISURFACEOVL_BACKBUFF_NUM; i++) {
			buf = (overlay_buffer_t)&mOverlayBuffer[i];

			if (m_pOverlay->dequeueBuffer(&buf)) {
        			DP("SetHWRenderSurf: dequeueBuffer FAILED \n");
				LeaveCriticalSection(&m_csDisplay);
				return E_FAIL;
			}
			DP("mOverlayBuffer[%d].%p,%p \n",i,mOverlayBuffer[i].paddr, mOverlayBuffer[i].vaddr);
			mOverlayBuffer[i].w_src = m_dwWidth;
			mOverlayBuffer[i].h_src = m_dwHeight;

		}

        if(mOverlayBuffer[0].paddr == mOverlayBuffer[1].paddr)
        DP("!!!!!!!!!!! mOverlayBuffer[0].paddr == mOverlayBuffer[1].paddr = %p\n",mOverlayBuffer[0].paddr );

        if(mOverlayBuffer[0].vaddr == mOverlayBuffer[1].vaddr)
        DP("!!!!!!!!!!! mOverlayBuffer[0].vaddr== mOverlayBuffer[1].vaddr= %p\n",
          mOverlayBuffer[0].vaddr);

        
		LeaveCriticalSection(&m_csDisplay);
   		DP("SetHWRenderSurf: successfully \n");

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
	    m_pBuffStraight = (unsigned char*)pHandle;
		return S_OK;
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
	int m_dpFormat;
	CRITICAL_SECTION m_csDisplay; 
	float m_bytesPerPixel;
	unsigned char* m_pBuff[ISURFACEOVL_BACKBUFF_NUM];
 	sp<Overlay> 	m_pOverlay; 
	struct overlay_buffer_emxx mOverlayBuffer[ISURFACEOVL_BACKBUFF_NUM];
	unsigned char* m_pBuffStraight;
    MemoryHeapBase*         m_heap;
    MemoryBase* m_PBuffer;

#ifdef RENESAS_ANDROID
   unsigned long m_renesas_memory_size;
	unsigned char *m_renesas_memory_addr;
   unsigned long m_renesas_phymemory_addr;
	int m_renesas_fd;
#endif

};

static const DWORD const_dwFourCC[] =
{
//	MAKEFOURCC('N','V','1','2'),
	MAKEFOURCC('Y','V','1','2'),
	0
};

CGfxAndroidISurfOverlay::CGfxAndroidISurfOverlay() : CMcUnknown(0,0)
{
	m_dwWidth = m_dwHeight = 0;
	m_dwBackBuffers = ISURFACEOVL_BACKBUFF_NUM;
	m_bShow = m_bLastShow = TRUE;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	m_hwnd = 0;
	m_bOpen = FALSE;
	m_dwFourCC = 0;
	m_logfile = NULL;
	InitializeCriticalSection(&m_csDisplay);

	m_dpFormat = 0;
	m_heap = 0;
  m_PBuffer = 0;
}

CGfxAndroidISurfOverlay::~CGfxAndroidISurfOverlay()
{
 	Close();
 	DeleteCriticalSection(&m_csDisplay);
}

STDMETHODIMP CGfxAndroidISurfOverlay::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
	UINT32 CamIFState = 0;
	int res = 0;

	if(dwWidth==0 || dwHeight==0)
	{
		DP("[CGfxAndroidISurfOverlay::Open] dwWidth==0 || dwHeight==0\n");
		return E_FAIL;
	}
	if(m_bOpen)
	{
		DP("[CGfxAndroidISurfOverlay::Open] Invalid call \n");
		return E_FAIL;
	}

	m_dwFourCC = 0;
	m_dpFormat = 0;
	m_bytesPerPixel = 0.0f;

	m_isurface = NULL;
	m_pOverlay = NULL;
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
			case MAKEFOURCC('Y','V','1','2'):
#ifndef RENESAS_ANDROID
	#if ANDROID_VER==23
				m_dpFormat =HAL_PIXEL_FORMAT_YV12;
	#else
				m_dpFormat =HAL_PIXEL_FORMAT_YCbCr_420_P;
	#endif
#else
 				m_dpFormat = OVERLAY_FORMAT_YCbCr_420_P;
 #endif
				m_bytesPerPixel = 1.5f;
				break;

			case MAKEFOURCC('N','V','1','2'):
#ifndef RENESAS_ANDROID
				m_dpFormat = HAL_PIXEL_FORMAT_YCrCb_420_SP;
#else
				m_dpFormat = OVERLAY_FORMAT_YCbCr_420_SP;
#endif
				m_bytesPerPixel = 1.5f;
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
		DP("[CGfxAndroidISurfOverlay::Open] m_dpFormat == 0 \n");
		return E_FAIL;
	}

/* 
    m_heap =   new MemoryHeapBase(m_dwWidth*m_dwHeight*m_bytesPerPixel* ISURFACEOVL_BACKBUFF_NUM);
    if(!m_heap)
    {
      LOGE("ERROR !!! null heap \n");
    }
*/

#if defined(RENESAS_ANDROID)
	int ret;
	unsigned long phys_start;
	unsigned long phys_size = 6*1024*1024;
	int test_count;
	m_renesas_fd = open("/dev/siz", O_RDWR | O_NONBLOCK | O_NOCTTY);
	if(m_renesas_fd < 0)
	     return E_FAIL;

	DP("========= mmap dynamic ========== \n");
	ioctl(m_renesas_fd, EMXX_CIDANA_PHYSTART, &m_renesas_phymemory_addr);
	ioctl(m_renesas_fd, EMXX_CIDANA_PHYSIZE, &m_renesas_memory_size);
	m_renesas_memory_addr = (unsigned char *)mmap(0, m_renesas_memory_size, 
	    PROT_READ | PROT_WRITE, MAP_SHARED,     m_renesas_fd, m_renesas_phymemory_addr);
	DP("mmap virtual addr: %p, phys_start:%p, phys_size=%x \n", m_renesas_memory_addr, 
    	m_renesas_phymemory_addr, m_renesas_memory_size);
#endif

    for(int i=0;i<ISURFACEOVL_BACKBUFF_NUM;i++)
    {
#ifdef INFOMAX
        m_heap = new MemoryHeapBase("/dev/pmem_adsp", m_dwWidth*m_dwHeight*m_bytesPerPixel);
    if(m_heap->heapID() >= 0) {
        m_PBuffer = new MemoryBase(m_heap, 0, m_dwWidth*m_dwHeight*m_bytesPerPixel);
      m_pBuff[i] = (unsigned char*)(m_PBuffer->getMemory(NULL, NULL)->base());
      DP("malloc m_pBuff[%d]:%p \n",i,m_pBuff[i]);
    }
    else
    {
      DP("failed to new pmem_adsp");
      return E_FAIL;
    }
#elif defined(RENESAS_ANDROID)
        if(i* m_dwWidth*m_dwHeight*m_bytesPerPixel > m_renesas_memory_size)
        {
            DP("physical memory isn't enough i:%d eachBuf:%d \n",i,m_dwWidth*m_dwHeight*m_bytesPerPixel);
            return E_FAIL;
        }
    	m_pBuff[i] = m_renesas_memory_addr + (LONG)(i* m_dwWidth*m_dwHeight*m_bytesPerPixel);
#else
        m_pBuff[i] = (unsigned char*)malloc(m_dwWidth*m_dwHeight*m_bytesPerPixel);
#endif      
        LOGE("m_pBuff[%d] :%p",i,m_pBuff[i]);
        memset(m_pBuff[i],0, i*m_dwWidth*m_dwHeight*m_bytesPerPixel);
      if(!m_pBuff[i])  
      {
        	DP("[<<<<<<<<<<<<<<CGfxAndroidISurfOverlay]malloc bufer %d failed \n",i);
          return E_FAIL;
      }
    }
	
	DP("[<<<<<<<<<<<<<<CGfxAndroidISurfOverlay]Open w:%d h:%d \n",m_dwWidth, m_dwHeight);

	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CGfxAndroidISurfOverlay::Close()
{
	if(!m_bOpen)
		return E_FAIL;

 	EnterCriticalSection(&m_csDisplay);
 
#ifdef INFOMAX
    if(m_PBuffer)
    {
      delete m_PBuffer;
      m_PBuffer = NULL;
    }
  if(m_heap)
  {
    delete m_heap;
    m_heap = NULL;
  }

#elif defined(RENESAS_ANDROID)
  	munmap(m_renesas_memory_addr, m_renesas_memory_size);
     close(m_renesas_fd);
     m_renesas_fd = 0;
#else
    for(int i=0;i<ISURFACEOVL_BACKBUFF_NUM;i++)
    {
      free(m_pBuff[i]);
      m_pBuff[i] = NULL;
    }
#endif
    DP("start destroy close \n");
  	if (m_pOverlay != NULL) {
		m_pOverlay->destroy();
    DP("start clear\n");
		//m_pOverlay.clear();
    DP("finishe clear \n");
	}

    DP("finish close!!!! GOOD\n");
	LeaveCriticalSection(&m_csDisplay);

 	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxAndroidISurfOverlay::SetSrcRect(const RECT *pRect)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidISurfOverlay::SetDstRect(const RECT *pRect)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidISurfOverlay::SetScnRect(const RECT *pRect)
{
	return S_OK;	
}

STDMETHODIMP CGfxAndroidISurfOverlay::GetScnRect(RECT *pRect)
{
	return E_FAIL;
}

STDMETHODIMP CGfxAndroidISurfOverlay::SetWindowHandle(HWND hwnd)
{
	m_hwnd = hwnd;
	return S_OK;
}

STDMETHODIMP CGfxAndroidISurfOverlay::GetWindowHandle(HWND& hwnd)
{
	hwnd = m_hwnd;
	return S_OK;
}

STDMETHODIMP CGfxAndroidISurfOverlay::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidISurfOverlay::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidISurfOverlay::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroidISurfOverlay::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidISurfOverlay::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidISurfOverlay::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidISurfOverlay::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidISurfOverlay::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if (m_isurface == NULL) return E_FAIL;
  if(num >= ISURFACEOVL_BACKBUFF_NUM)  return E_INVALIDARG;
	EnterCriticalSection(&m_csDisplay);
	if(ppBuf)
	{
//   		*ppBuf = m_pOverlay->getBufferAddress(&mOverlayBuffer[num]);
   		*ppBuf = m_pBuff[num];
 	}
	
	if(pStride){
			*pStride = m_dwWidth;
	}

	return S_OK;
}

STDMETHODIMP CGfxAndroidISurfOverlay::Unlock(LONG num)
{
	LeaveCriticalSection(&m_csDisplay);
	return S_OK;
}

STDMETHODIMP CGfxAndroidISurfOverlay::Flip(LONG num, DWORD dwFlags)
{
	if (m_isurface == NULL) return E_FAIL;
	if(!m_bShow)  return S_OK;
	
  if(num >= ISURFACEOVL_BACKBUFF_NUM)  return E_INVALIDARG;
	EnterCriticalSection(&m_csDisplay);
	overlay_buffer_t buf = (overlay_buffer_t)&mOverlayBuffer[num];
    if(dwFlags == GFXOVERLAY_FLIP_FIXED)
    mOverlayBuffer[num].vaddr = m_pBuffStraight;
  else
	  mOverlayBuffer[num].vaddr = m_pBuff[num];
  
	mOverlayBuffer[num].time.tv_sec = -1;
	mOverlayBuffer[num].time.tv_usec = -1;

	m_pOverlay->queueBuffer(buf);
	m_pOverlay->dequeueBuffer(&buf);

	LeaveCriticalSection(&m_csDisplay);
  	return S_OK;
}

STDMETHODIMP CGfxAndroidISurfOverlay::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;

	m_bShow = bShow;
	return S_OK;
}

STDMETHODIMP CGfxAndroidISurfOverlay::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroidISurfOverlay::GetSurfaceInfo(GfxOverlayInfo *pInfo)
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

STDMETHODIMP CGfxAndroidISurfOverlay::IsOpen()
{
	return m_bOpen;
}

IMcGfxOverlay *CreateAndroidISurfaceOverlay()
{
	DP("[CGfxAndroidISurfOverlay] CreateAndroidISurfaceOverlay   \n");
	return new CGfxAndroidISurfOverlay();
}
#endif

