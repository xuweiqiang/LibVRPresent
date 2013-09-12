//=============================================================================
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO INTERVIDEO, INC.
// 	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//	COMPANY.
//
// 	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// 	PURPOSE.
//
// 	Copyright (c) 2004 - 2005  InterVideo Corporation.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#define USE_FRAMEBUFFER_ONLY

#include <windows.h>
#include "../../inc/GfxContext.h"
#include "GxBridge.h"
//#include "XscaleOverlay.h"
//#include "Intel2700GOverlay.h"

#ifdef HARDWARE_2700G
#include "gigabyte_2700G.h"
#endif

#if defined(FREESCALE_DDRW_RGB565) || defined(FREESCALE_DDRW_UYVY) || defined(FREESCALE_DDRW_YV12)
#include "FreeScaleDirectDraw.h"
#endif
HRESULT GxBridge::Open(GFX_OPEN_OPTIONS *pOptions, DWORD dwSize)
{
	GFX_OPEN_OPTIONS Options;

	if(dwSize<sizeof(Options))
	{
		ZeroMemory(&Options,sizeof(Options));
		memcpy(&Options,pOptions,dwSize);
		pOptions = &Options;
	}
	m_hWnd = pOptions->hWnd;
	if(m_hWnd==0)
	{
#if defined(FREESCALE_DDRW_RGB565) || defined(FREESCALE_DDRW_UYVY) || defined(FREESCALE_DDRW_YV12)
		int screen_width,screen_height;
		screen_width = GetSystemMetrics(SM_CXSCREEN);
		screen_height = GetSystemMetrics(SM_CYSCREEN);
		//LOG(f_log, "screen_width=%d\t screen_height=%d\n", screen_width,screen_height);
#ifdef FREESCALE_DDRW_YV12
		int ret = (FSDirectDraw_Init(320,240,320,240,RGB(0,0,0),FORMAT_YV12) == 0); //YV12		
#else
#ifdef FREESCALE_DDRW_UYVY
		int ret = (FSDirectDraw_Init(320,240,320,240,RGB(0,0,0),FORMAT_UYVY) == 0); //UYVY		
#else
		int ret = (FSDirectDraw_Init(screen_width,screen_height,screen_width,screen_height,RGB(0,0,0),FORMAT_RGB565) == 0); //RGB565
#endif	// #ifdef FREESCALE_DDRW_UYVY
#endif	// #ifdef FREESCALE_DDRW_YV12

#else

#ifndef USE_FRAMEBUFFER_ONLY
#ifdef USE_2700G
		int ret = Intel2700GOverlayInit();//0,0,360,240,360,0,100,480,360,0);
		//Intel2700GOverlaySet(0,0,360,240,360,0,100,480,360,0);
#elif HARDWARE_2700G
		m_src_x = 0; 
		m_src_y = 0; 
		m_src_width = 0;
		m_src_height = 0; 
		m_dst_x = 0;
		m_dst_y = 0; 
		m_dst_width = 0;
		m_dst_height = 0; 
		m_rot = 0;
		m_enable_2700g = FALSE;
		int ret = Giga2700GOverlayInit();
#else
		int ret = XscaleInit();
#endif		
		if (ret == ERR_NONE)
			return S_OK;
		else
#endif
#endif


		return E_FAIL;
	}
	if(FAILED(OpenDisplay(m_hWnd)))
	{
		MessageBox(NULL,TEXT("Reset PDA!"),TEXT("Diplay did not open!"),MB_OK);
		return E_FAIL;
	}
#if 0 //ndef SMARTPHONE
	//Initialize the Hardware Buttons	
	if(FAILED(OpenInput()))
	{
		MessageBox(NULL,TEXT("Reset PDA!"),TEXT("HW buttons did not open!"),MB_OK);
	}
#endif
	m_KeyList		= GXGetDefaultKeys(GX_NORMALKEYS);
#if !defined(_WIN32_WCE)
	m_Properties	= GXGetDisplayProperties();
#endif
	m_dwLockers = 0;
	m_pBuffer = 0;
	return S_OK;
}

HRESULT GxBridge::Close()
{
	if(m_hWnd)
	{
#if 0 // ndef SMARTPHONE
		CloseInput();
#endif
		CloseDisplay();
		m_hWnd = 0;
	}
	ZeroMemory(&m_KeyList,sizeof(m_KeyList));
	ZeroMemory(&m_Properties,sizeof(m_Properties));
	m_dwLockers = 0;
	m_pBuffer = 0;
#ifndef USE_FRAMEBUFFER_ONLY
#ifdef USE_2700G
	Intel2700GOverlayUninit();
#else
#if defined(FREESCALE_DDRW_RGB565) || defined(FREESCALE_DDRW_UYVY) || defined(FREESCALE_DDRW_YV12)
	FSDirectDraw_Close(); //RGB565
#elif HARDWARE_2700G
	m_src_x = 0; 
	m_src_y = 0; 
	m_src_width = 0;
	m_src_height = 0; 
	m_dst_x = 0;
	m_dst_y = 0; 
	m_dst_width = 0;
	m_dst_height = 0; 
	m_rot = 0;
	m_enable_2700g = FALSE;
	Giga2700GOverlayUninit();
#else
	XscaleUnInit();
#endif
#endif
#endif
	return S_OK;
}

HRESULT GxBridge::OpenDisplay(HWND hWnd)
{
	int res;

	res = GXOpenDisplay(hWnd, GX_FULLSCREEN);
#if 1// def SMARTPHONE
	return S_OK;	// smartphone can return strange results, e.g. 0; pocket pc can return nonzero value (but not 1)
#else
	return res==0 ? E_FAIL : S_OK;
#endif
}

HRESULT GxBridge::CloseDisplay()
{ 
	return GXCloseDisplay()==1 ? S_OK : E_FAIL;
}

HRESULT GxBridge::BeginDraw(void ** pY, void ** pU, void ** pV, int * pitch, RECT *rtSrcView, RECT *rtDstView, int rot)
{ 
	HRESULT hr = E_FAIL;
#ifdef USE_2700G
	m_rotation = rot;
	Intel2700GOverlaySet(rtSrcView->left,rtSrcView->top,WidthRect(rtSrcView),HeightRect(rtSrcView),rtDstView->left,rtDstView->top,WidthRect(rtDstView),HeightRect(rtDstView),rot);
	hr = Intel2700GBeginDraw((unsigned char **)pY,(unsigned char **)pU,(unsigned char **)pV,pitch);
#elif HARDWARE_2700G
	if ((m_src_x != 0) ||
		(m_src_y != 0) || 
		(m_src_width != WidthRect(rtSrcView)) ||
		(m_src_height != HeightRect(rtSrcView)) || 
		(m_dst_x != rtDstView->left) ||
		(m_dst_y != rtDstView->top) || 
		(m_dst_width != WidthRect(rtDstView)) ||
		(m_dst_height != HeightRect(rtDstView)) || 
		(m_rot != rot))
	{
		m_src_x = 0;
		m_src_y = 0;
		m_src_width = WidthRect(rtSrcView);
		m_src_height = HeightRect(rtSrcView);
		m_dst_x = rtDstView->left;
		m_dst_y = rtDstView->top;
		m_rot = rot;
		if (m_enable_2700g == FALSE)
		{
			m_dst_width = 0;
			m_dst_height = 0;
			rot = 90;
			m_enable_2700g = TRUE;
		}
		else
		{
			m_dst_width = WidthRect(rtDstView);
			m_dst_height = HeightRect(rtDstView);
		}
		hr = Giga2700GOverlaySet((unsigned int)*pitch, 0, 0, m_src_width, m_src_height, m_dst_x, m_dst_y, m_dst_width, m_dst_height, rot);
	}
	hr = Giga2700GBeginDraw((unsigned int)*pitch);
#elif defined(FREESCALE_DDRW_RGB565) || defined(FREESCALE_DDRW_UYVY) || defined(FREESCALE_DDRW_YV12)
	unsigned char * pDst[3];
	hr = FSDirectDraw_BeginDraw(pDst, rtSrcView, rtDstView, rot);
	if (hr == ERR_NONE)
	{
		m_pBuffer = *pY = pDst[0];
		return S_OK;
	}
	else
	{
		FSDirectDraw_Close();
		FSDirectDraw_Init(320,240,320,240,RGB(0,0,0),FORMAT_YV12);
		return E_FAIL;
	}
#endif
	return hr;
}

HRESULT GxBridge::BeginDraw(void ** pFrameBuffer,bool flip)
{ 
	HRESULT hr;
#if defined(USE_FRAMEBUFFER_ONLY) || defined (USE_2700G)
	if(pFrameBuffer==NULL)
		return E_FAIL;
	if(m_bExclusiveMode)
	{
		EnterCriticalSection(&m_csDraw);
		if(m_dwLockers)
		{
			*pFrameBuffer = m_pBuffer;
			m_dwLockers++;
			LeaveCriticalSection(&m_csDraw);
		}
#if defined(_WIN32_WCE)
		m_pBuffer = *pFrameBuffer = m_pFrameBuffer;
#else
		m_pBuffer = *pFrameBuffer = GXBeginDraw();
#endif
		hr = *pFrameBuffer ? S_OK : E_FAIL; 
		if(SUCCEEDED(hr))
			m_dwLockers++;
		else
			LeaveCriticalSection(&m_csDraw);
	}
	else
	{
#if defined(_WIN32_WCE)
		m_pBuffer = *pFrameBuffer = m_pFrameBuffer;
#else
		m_pBuffer = *pFrameBuffer = GXBeginDraw();
#endif
		hr = *pFrameBuffer ? S_OK : E_FAIL; 
	}
#elif HARDWARE_2700G
	return S_OK;
#else
#if defined(FREESCALE_DDRW_RGB565) || defined(FREESCALE_DDRW_UYVY) || defined(FREESCALE_DDRW_YV12)
	unsigned char * pDst[3];
	int pitch = 0;
	int BPP = 0;
	hr = FSDirectDraw_BeginDraw(pDst);
	if (hr == ERR_NONE)
	{
		m_pBuffer = *pFrameBuffer = pDst[0];
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
#else
	unsigned char * pDst[3];
	int pitch = 0;
	int BPP = 0;
	hr = XscaleBeginDraw(pDst,&BPP,&pitch,flip);
	if (hr == ERR_NONE)
	{
		m_pBuffer = *pFrameBuffer = pDst[0];
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
#endif
#endif
	return hr;
}

HRESULT GxBridge::EndDraw(bool flip)
{
	HRESULT hr = S_OK;
#ifdef USE_2700G
	Intel2700GEndDraw();
#endif
	return hr;
}

HRESULT GxBridge::EndDraw()
{ 
	HRESULT hr = S_OK;
#if defined(USE_FRAMEBUFFER_ONLY) || defined (USE_2700G)
	if(m_bExclusiveMode)
	{
		if(m_dwLockers==0)
			return E_FAIL;
		m_dwLockers--;
		if(m_dwLockers==0)
		{
			if(GXEndDraw()!=1)
				hr = E_FAIL;
			m_pBuffer = 0;
			LeaveCriticalSection(&m_csDraw);
		}
	}
	else
	{
		if(GXEndDraw()!=1)
			hr = E_FAIL;
	}
#elif HARDWARE_2700G
	Giga2700GEndDraw();
#else
#if defined(FREESCALE_DDRW_RGB565) || defined(FREESCALE_DDRW_UYVY) || defined(FREESCALE_DDRW_YV12)
	FSDirectDraw_EndDraw();
#else
	XscaleEndDraw();
#endif
#endif
	return hr;
}

// This function turns on the unfiltered button message mode. 
// All hardware button presses will generate appropriate WM_KEY messages 
// to the application and will not be sent to the shell.
HRESULT GxBridge::OpenInput()
{ 
	return GXOpenInput()==1 ? S_OK : E_FAIL;
}

// This function releases any input-related resources 
// used by GAPI and returns button-press messages to their filtered mode.
HRESULT GxBridge::CloseInput()
{ 
	return GXCloseInput()==1 ? S_OK : E_FAIL;
}

HRESULT GxBridge::GetDisplayProperties()
{ 
	return S_OK; 
}

// GXSuspend must be called when the application receives a WM_KILLFOCUS message. 
// Failure to do so could result in undesired consequences. 
// It is acceptable to call GXSuspend automatically in the WM_KILLFOCUS handler 
// without regard to nesting it properly with a GXResume call.
HRESULT GxBridge::SetFocus(BOOL bFocusOn)
{
	if(m_hWnd==0)
		return S_OK;
	if(m_bExclusiveMode)
		EnterCriticalSection(&m_csDraw);
	if(bFocusOn)
		GXResume();
	else
		GXSuspend();
	if(m_bExclusiveMode)
		LeaveCriticalSection(&m_csDraw);
	return S_OK;
}

HRESULT GxBridge::SetViewport(RECT *rtView)
{ 
	/*
	HRESULT hr;

	if(m_bExclusiveMode)
		EnterCriticalSection(&m_csDraw);
	hr = GXSetViewport(rtView->top,rtView->bottom-rtView->top,0,0)==1 ? S_OK : E_FAIL; 
	if(m_bExclusiveMode)
		LeaveCriticalSection(&m_csDraw);
		*/
#if defined(FREESCALE_DDRW_UYVY) || defined(FREESCALE_DDRW_YV12)
	FSDirectDraw_SetView(rtView);
#endif
	return S_OK;
}

HRESULT GxBridge::GetGuiKey( DWORD dwIn, DWORD * pOut )
{
	if(pOut==NULL)
		return E_FAIL;
	*pOut = GUI_KEY_UNDEFINED;
	if(dwIn==(unsigned)m_KeyList.vkUp)
	{
		*pOut = GUI_KEY_UP;
		return S_OK;
	}
	if(dwIn==(unsigned)m_KeyList.vkDown) 
	{
		*pOut = GUI_KEY_DOWN;
		return S_OK;
	}
	if(dwIn==(unsigned)m_KeyList.vkLeft) 
	{
		*pOut = GUI_KEY_LEFT;
		return S_OK;
	}
	if(dwIn==(unsigned)m_KeyList.vkRight) 
	{
		*pOut = GUI_KEY_RIGHT;
		return S_OK;
	}
	if(dwIn==(unsigned)m_KeyList.vkStart) 
	{
		*pOut = GUI_KEY_START;
		return S_OK;
	}
	return S_OK;
}

HRESULT GxBridge::GetGraphicsProperties( GraphicsProperties * pGraphicsProperties )
{
	if(pGraphicsProperties==NULL)
		return E_FAIL;

	pGraphicsProperties->cBPP		= m_Properties.cBPP;
	pGraphicsProperties->cbxPitch	= m_Properties.cbxPitch;
	pGraphicsProperties->cbyPitch	= m_Properties.cbyPitch;
	pGraphicsProperties->cxWidth	= m_Properties.cxWidth;
	pGraphicsProperties->cyHeight	= m_Properties.cyHeight;
	pGraphicsProperties->ffFormat	= 0;

	if(m_Properties.ffFormat&kfDirect555)
		pGraphicsProperties->ffFormat |= RGB_555;
	if(m_Properties.ffFormat&kfDirect565)
		pGraphicsProperties->ffFormat |= RGB_565;
	if(m_Properties.ffFormat&kfDirect888)
		pGraphicsProperties->ffFormat |= RGB_888;
	if(m_Properties.ffFormat&kfDirect444)
		pGraphicsProperties->ffFormat |= RGB_444;

	return S_OK;
}


HRESULT GxBridge::QuerySupported(REFGUID guidPropSet, DWORD dwPropID, DWORD *pTypeSupport)
{
	return E_NOTIMPL;
}

HRESULT GxBridge::Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
{
	if(guidPropSet!=EMC_PROPSET_GFXCTXT)
		return E_PROP_SET_UNSUPPORTED;
	switch(dwPropID)
	{
	case EMC_PROPID_GFXCTXT_GFXINFO:
		{
		EMC_GRAPHICS_INFO *pInfo;
		
		pInfo = reinterpret_cast<EMC_GRAPHICS_INFO *>(pPropData);
		if(pcbReturned)
			*pcbReturned = sizeof(EMC_GRAPHICS_INFO);
		pInfo->dwAspectX = 1;
		pInfo->dwAspectY = 1;
		pInfo->dwCopyProtect = 0;
#ifdef USE_2700G
		pInfo->dwFourCC = MAKE_FOURCC('Y','V','1','2');
#else
#ifdef  FREESCALE_DDRW_UYVY
		pInfo->dwFourCC = MAKE_FOURCC('U','Y','V','Y');
#else
#ifdef  FREESCALE_DDRW_YV12
		pInfo->dwFourCC = MAKE_FOURCC('Y','V','1','2');
#else
		pInfo->dwFourCC = MAKE_FOURCC('P','5','6','5');
#endif	// #ifdef  FREESCALE_DDRW_YV12
#endif	// #ifdef  FREESCALE_DDRW_UYVY
#endif	// #ifdef USE_2700G
		pInfo->dwInterlace = 0;
		return S_OK;
		}
	}
	return E_PROP_ID_UNSUPPORTED;
}

HRESULT GxBridge::Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
{
	if(guidPropSet!=EMC_PROPSET_GFXCTXT)
		return E_PROP_SET_UNSUPPORTED;
	return E_PROP_ID_UNSUPPORTED;
}

HRESULT GxBridge::NonDelegatingQueryInterface(REFIID riid, LPVOID *ppvObj)
{
//	if(riid==IID_IEmcPropertySet)
//		return GetInterface(static_cast<IEmcPropertySet*>(this), ppvObj);
	if(riid==IID_IGraphicsContext)
		return GetInterface(static_cast<IGraphicsContext*>(this), ppvObj);
	return CMcUnknown::NonDelegatingQueryInterface(riid, ppvObj);
}

GxBridge::GxBridge() : CMcUnknown(0,0)
{
	ZeroMemory(&m_KeyList,sizeof(m_KeyList));
	ZeroMemory(&m_Properties,sizeof(m_Properties));
	InitializeCriticalSection(&m_csDraw);
	m_hWnd = 0;
	m_dwLockers = 0;
	m_pBuffer = 0;
	m_bExclusiveMode = FALSE;
	m_pFrameBuffer = 0;
	m_rotation = 0;
	HDC DC = ::GetDC(NULL);
	memset((void*)&m_RawFrameBufferInfo, 0, sizeof(RawFrameBufferInfo));
#if defined(FREESCALE_DDRW_RGB565) || defined(FREESCALE_DDRW_UYVY) || defined(FREESCALE_DDRW_YV12)
	//if (m_Properties.cxWidth == 0 ||m_Properties.cyHeight == 0)
	{
		m_Properties.cBPP = m_RawFrameBufferInfo.wBPP;
		m_Properties.cxWidth  = m_RawFrameBufferInfo.cxPixels = GetSystemMetrics(SM_CXSCREEN);
		m_Properties.cyHeight = m_RawFrameBufferInfo.cyPixels = GetSystemMetrics(SM_CYSCREEN);
#ifdef FREESCALE_DDRW_RGB565
		m_Properties.ffFormat = m_RawFrameBufferInfo.wFormat = kfDirect565;
		m_Properties.cbxPitch = m_RawFrameBufferInfo.cxStride = 2;
		m_Properties.cbyPitch = m_RawFrameBufferInfo.cyStride = 2*GetSystemMetrics(SM_CXSCREEN);
#else
		m_Properties.ffFormat = m_RawFrameBufferInfo.wFormat = FORMAT_OTHER;
		m_Properties.cbxPitch = m_RawFrameBufferInfo.cxStride = 2;
		m_Properties.cbyPitch = m_RawFrameBufferInfo.cyStride = 2*GetSystemMetrics(SM_CXSCREEN);
#endif
	}
	//DP("GxBridge()->m_Properties.cBPP:%x,cxWidth:%x,cyHeight:%x,cbxPitch:%x, cbyPitch:%x,ffFormat:%x\n",
	//	m_Properties.cBPP,m_Properties.cxWidth,m_Properties.cyHeight,m_Properties.cbxPitch,m_Properties.cbyPitch,m_Properties.ffFormat);

#else

	ExtEscape(DC, GETRAWFRAMEBUFFER, 0, NULL, sizeof(RawFrameBufferInfo), (char*)&m_RawFrameBufferInfo);
	if (m_RawFrameBufferInfo.pFramePointer)
	{
		m_pFrameBuffer = (char*)m_RawFrameBufferInfo.pFramePointer;
#if defined(_WIN32_WCE)
		m_Properties.cBPP = m_RawFrameBufferInfo.wBPP;
		m_Properties.cxWidth = m_RawFrameBufferInfo.cxPixels;
		m_Properties.cyHeight = m_RawFrameBufferInfo.cyPixels;
		m_Properties.cbxPitch = m_RawFrameBufferInfo.cxStride;
		m_Properties.cbyPitch = m_RawFrameBufferInfo.cyStride;
		m_Properties.ffFormat = m_RawFrameBufferInfo.wFormat;
#endif
	}
	else
	{
		DWORD Code = GETGXINFO;
		if (ExtEscape(DC, ESC_QUERYESCSUPPORT, sizeof(DWORD), (char*)&Code, 0, NULL) > 0)
		{
			DWORD DWidth = GetDeviceCaps(DC,HORZRES);
			DWORD DHeight = GetDeviceCaps(DC,VERTRES);
			GXDeviceInfo GXInfo;
			memset(&GXInfo,0,sizeof(GXInfo));
			GXInfo.Version = 100;
			ExtEscape(DC, GETGXINFO, 0, NULL, sizeof(GXInfo), (char*)&GXInfo);
				
			m_pFrameBuffer = (char *)GXInfo.pvFrameBuffer;
#if defined(_WIN32_WCE)		
			m_Properties.cBPP = GXInfo.cBPP;
			m_Properties.cxWidth = GXInfo.cxWidth;
			m_Properties.cyHeight = GXInfo.cyHeight;
			m_Properties.cbxPitch = GXInfo.cBPP / 8;
			m_Properties.cbyPitch = GXInfo.cbStride;
			m_Properties.ffFormat = GXInfo.ffFormat;
#endif			
		}			
	}
#endif
}

GxBridge::~GxBridge()
{
	DeleteCriticalSection(&m_csDraw);
}

IGraphicsContext *CreateGraphicsContext()
{
    return new GxBridge();
}



