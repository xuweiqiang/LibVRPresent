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
// 	Copyright (c) 2010  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#include "../../inc/GfxBuildDef.h"

#ifdef GFX_ANDROID_OMX

#include <stdio.h>
#include <windows.h>
#include "GfxOverlay.h"
#include "McCom.h"
#include "../../inc/dp.h"
#include "../../inc/GfxPresent.h"

#include <OMX_Core.h>
#include <OMX_Component.h>
#include <OMX_Types.h>
#include <OMX_Video.h>

#include <tsemaphore.h>
#include <user_debug_levels.h>

#ifndef MAKEFOURCC
    #define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
                ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
                ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)

/* Application's private data */
typedef struct appPrivateType{
  tsem_t* fbdevSinkEventSem;
  OMX_HANDLETYPE fbdev_sink_handle;
}appPrivateType;

/** Specification version*/
#define VERSIONMAJOR    1
#define VERSIONMINOR    1
#define VERSIONREVISION 0
#define VERSIONSTEP     0

#define NVIDIA_ANDROID
//#define PC_ANDROID

#if defined(PC_ANDROID)
#define SCN_WIDTH 1280
#define SCN_HEIGHT 800
#elif defined(T20_FIH)
#define SCN_WIDTH 1024
#define SCN_HEIGHT 600
#else
#define SCN_WIDTH 640
#define SCN_HEIGHT 480
#endif
RECT m_rectDst;

/** Initializes the semaphore at a given value
 *
 * @param tsem the semaphore to initialize
 * @param val the initial value of the semaphore
 *
 */
int tsem_init(tsem_t* tsem, unsigned int val) {
	int i;
	i = pthread_cond_init(&tsem->condition, NULL);
	if (i!=0) {
		return -1;
	}
	i = pthread_mutex_init(&tsem->mutex, NULL);
	if (i!=0) {
		return -1;
	}
	tsem->semval = val;
	return 0;
}

/** Destroy the semaphore
 *
 * @param tsem the semaphore to destroy
 */
void tsem_deinit(tsem_t* tsem) {
  pthread_cond_destroy(&tsem->condition);
  pthread_mutex_destroy(&tsem->mutex);
}

/** Decreases the value of the semaphore. Blocks if the semaphore
 * value is zero.
 *
 * @param tsem the semaphore to decrease
 */
void tsem_down(tsem_t* tsem) {
  pthread_mutex_lock(&tsem->mutex);
  while (tsem->semval == 0) {
    pthread_cond_wait(&tsem->condition, &tsem->mutex);
  }
  tsem->semval--;
  pthread_mutex_unlock(&tsem->mutex);
}

/** Increases the value of the semaphore
 *
 * @param tsem the semaphore to increase
 */
void tsem_up(tsem_t* tsem) {
  pthread_mutex_lock(&tsem->mutex);
  tsem->semval++;
  pthread_cond_signal(&tsem->condition);
  pthread_mutex_unlock(&tsem->mutex);
}

/** Reset the value of the semaphore
 *
 * @param tsem the semaphore to reset
 */
void tsem_reset(tsem_t* tsem) {
  pthread_mutex_lock(&tsem->mutex);
  tsem->semval=0;
  pthread_mutex_unlock(&tsem->mutex);
}

/** Wait on the condition.
 *
 * @param tsem the semaphore to wait
 */
void tsem_wait(tsem_t* tsem) {
  pthread_mutex_lock(&tsem->mutex);
  pthread_cond_wait(&tsem->condition, &tsem->mutex);
  pthread_mutex_unlock(&tsem->mutex);
}

/** Signal the condition,if waiting
 *
 * @param tsem the semaphore to signal
 */
void tsem_signal(tsem_t* tsem) {
  pthread_mutex_lock(&tsem->mutex);
  pthread_cond_signal(&tsem->condition);
  pthread_mutex_unlock(&tsem->mutex);
}

/** callback prototypes for sink */
OMX_ERRORTYPE fb_sinkEventHandler(
  OMX_OUT OMX_HANDLETYPE hComponent,
  OMX_OUT OMX_PTR pAppData,
  OMX_OUT OMX_EVENTTYPE eEvent,
  OMX_OUT OMX_U32 Data1,
  OMX_OUT OMX_U32 Data2,
  OMX_OUT OMX_PTR pEventData);

OMX_ERRORTYPE fb_sinkEmptyBufferDone(
  OMX_OUT OMX_HANDLETYPE hComponent,
  OMX_OUT OMX_PTR pAppData,
  OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer);

appPrivateType* g_appPriv = NULL;
OMX_CALLBACKTYPE g_fbdev_sink_callbacks = 
{
	 fb_sinkEventHandler,
	 fb_sinkEmptyBufferDone,
	 NULL
};


#if 0
#include <signal.h>
struct sigaction sigint_act;

static void sigintHandler(int sig)
{
	if(sig == SIGINT)
	{
		DP( "----------%s \n", __FUNCTION__);
		stopRender();
	}
}

static void regSignalHandler()
{
	sigint_act.sa_flags = 0;
	sigemptyset(&sigint_act.sa_mask);
	sigaddset(&sigint_act.sa_mask, SIGINT);
	sigint_act.sa_handler = sigintHandler;
	sigaction(SIGINT, &sigint_act, 0);
}
#endif

/** callbacks implementation of video sink component */
OMX_ERRORTYPE fb_sinkEventHandler(
		OMX_OUT OMX_HANDLETYPE hComponent,
		OMX_OUT OMX_PTR pAppData,
		OMX_OUT OMX_EVENTTYPE eEvent,
		OMX_OUT OMX_U32 Data1,
		OMX_OUT OMX_U32 Data2,
		OMX_OUT OMX_PTR pEventData) 
{

	DP( "Hi there, I am in the %s callback\n", __func__);
	if(eEvent == OMX_EventCmdComplete) 
	{
		if (Data1 == OMX_CommandStateSet)
		{
			DP( "State changed in ");
			switch ((int)Data2) 
			{
				case OMX_StateInvalid:
					DP( "OMX_StateInvalid\n");
					break;
				case OMX_StateLoaded:
					DP( "OMX_StateLoaded\n");
					break;
				case OMX_StateIdle:
					DP( "OMX_StateIdle ---- fbsink\n");
					break;
				case OMX_StateExecuting:
					DP( "OMX_StateExecuting\n");
					break;
				case OMX_StatePause:
					DP( "OMX_StatePause\n");
					break;
				case OMX_StateWaitForResources:
					DP( "OMX_StateWaitForResources\n");
					break;
			}
			tsem_up(g_appPriv->fbdevSinkEventSem);
		}
		else if (OMX_CommandPortEnable || OMX_CommandPortDisable) 
		{
			DP( "In %s Received Port Enable/Disable Event\n",__func__);
			tsem_up(g_appPriv->fbdevSinkEventSem);
		}
	} 
	else if(eEvent == OMX_EventBufferFlag)
	{
		DP( "In %s OMX_BUFFERFLAG_EOS\n", __func__);
		if((int)Data2 == OMX_BUFFERFLAG_EOS) 
		{
			DP( "<<<<<<<<<<<<<<<end>>>>>>>>>>>>>\n");
			exit(0);
		}
	}
	else
	{
		DP( "Param1 is %x\n", (int)Data1);
		DP( "Param2 is %x\n", (int)Data2);
	}
	return OMX_ErrorNone;
}

OMX_ERRORTYPE fb_sinkEmptyBufferDone(
		OMX_OUT OMX_HANDLETYPE hComponent,
		OMX_OUT OMX_PTR pAppData,
		OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer) {
	return OMX_ErrorNone;
}

class CGfxAndroid : public IMcGfxOverlay, public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	CGfxAndroid();
	virtual ~CGfxAndroid();
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
	STDMETHODIMP GetSurfaceInfo(GfxOverlayInfo *pInfo);
	STDMETHODIMP GetDDCaps(LONG* pCaps)
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
	STDMETHODIMP SetVRHandle(LPVOID pHandle)
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
	STDMETHODIMP Update();
	OMX_ERRORTYPE  PreInit();

	OMX_ERRORTYPE SetPortParameters();
	OMX_ERRORTYPE StopRender();
	void SetHeader(OMX_PTR header, OMX_U32 size);
	OMX_ERRORTYPE test_OMX_ComponentNameEnum() ;

	
	RECT				m_rectSrc, m_rectScn;
	DWORD				m_dwWidth, m_dwHeight, m_dwFourCC;
	const DWORD			*m_pdwFourCCList;
	DWORD				m_dwBackBuffers;
	BOOL				m_bShow, m_bLastShow;
	HWND				m_hwnd;
	BOOL m_bOpen; 	
	FILE *m_logfile;

	OMX_COLOR_FORMATTYPE m_inputFormat; 
	float m_bytesPerPixel;
	OMX_PARAM_PORTDEFINITIONTYPE m_omx_sinkPortDefinition;
	OMX_VIDEO_PARAM_PORTFORMATTYPE m_omxVideoParam;
	OMX_BUFFERHEADERTYPE *m_pInBufferSink1, *m_pInBufferSink2;
};

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
	MAKEFOURCC('R','8','8','8'),
	0
};

CGfxAndroid::CGfxAndroid() : CMcUnknown(0,0)
{
	m_dwWidth = m_dwHeight = 0;
	m_dwBackBuffers = 2;
	m_bShow = m_bLastShow = TRUE;
	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));
	ZeroMemory(&m_rectScn,sizeof(m_rectScn));
 
	m_hwnd = 0;
	m_bOpen = FALSE;
	m_dwFourCC = 0;
	m_logfile = NULL;

	m_inputFormat = OMX_COLOR_FormatUnused; 
	m_bytesPerPixel = 0.0f;
	ZeroMemory(&m_omx_sinkPortDefinition,sizeof(m_omx_sinkPortDefinition));
	ZeroMemory(&m_omxVideoParam,sizeof(m_omxVideoParam));
	m_pInBufferSink1 = NULL;
	m_pInBufferSink2 = NULL;

}


CGfxAndroid::~CGfxAndroid()
{
	Close();
}

STDMETHODIMP CGfxAndroid::Open(DWORD dwWidth, DWORD dwHeight, 
	DWORD dwBuffers, const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst);	
{
	DP("[CGfxAndroid::Open]\n");
	int res = 0;

	if(dwWidth==0 || dwHeight==0)
	{
		DP("[CGfxAndroid::Open] dwWidth==0 || dwHeight==0\n");
		return E_FAIL;
	}

	if(m_bOpen)
	{
		DP("[CGfxAndroid::Open] Invalid call \n");
		return E_FAIL;
	}

	m_dwFourCC = 0;
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	SetRect(&m_rectScn,0,0,SCN_WIDTH,SCN_HEIGHT);

	ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
	ZeroMemory(&m_rectDst,sizeof(m_rectDst));

	SetRect(&m_rectSrc, 0, 0 ,m_dwWidth, m_dwHeight);

	if(pDst && (pDst->right - pDst->left != 0) && (pDst->bottom - pDst->top != 0))
	{
		m_rectDst = *pDst;
	}
	else
	{
		m_rectDst.right = m_rectScn.right;
		m_rectDst.bottom = m_rectScn.bottom;
	}
	
	if(pdwFourCC)
		m_pdwFourCCList = pdwFourCC;
	else
		m_pdwFourCCList = const_dwFourCC;

	m_inputFormat = OMX_COLOR_FormatUnused;
	m_bytesPerPixel = 0;
	for(DWORD i=0;m_dwFourCC=m_pdwFourCCList[i];i++)
	{
		switch(m_dwFourCC)
		{
			case MAKEFOURCC('R','8','8','8'):
				m_inputFormat = OMX_COLOR_Format24bitRGB888;
				m_bytesPerPixel = 3.0f;
				break;
			case MAKEFOURCC('Y','V','1','2'):
				m_inputFormat = OMX_COLOR_FormatYUV420Planar;
				m_bytesPerPixel = 1.5f;
				break;
			default:
				break;
		}
		if(m_inputFormat  != OMX_COLOR_FormatUnused)
			break;
	}

	if(m_inputFormat   == OMX_COLOR_FormatUnused)
		return E_FAIL;

	DP("[CGfxAndroid]Open w:%d h:%d fmt:%d  \n",m_dwWidth, m_dwHeight,m_inputFormat );

	OMX_ERRORTYPE ret = OMX_ErrorNone;
	ret = PreInit();
	if(ret != OMX_ErrorNone)
		return E_FAIL;
	
	ret = SetPortParameters();
	if(ret != 0)
		return E_FAIL;

//	regSignalHandler();
	
	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CGfxAndroid::Close()
{
	if(m_bOpen == TRUE)
	{
		StopRender();
	}
	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxAndroid::SetSrcRect(const RECT *pRect)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroid::SetDstRect(const RECT *pRect)
{
	RECT rd;

	DP("[CGfxAndroid]Set destRect (%d,%d, %d, %d) \n", pRect->left, pRect->top, pRect->right, pRect->bottom);
	if(EqualRect(pRect,&m_rectDst) ){
		DP("[CGfxAndroid]Set destRect equal rect\n");
		return S_OK;
	}
	
	DWORD width = pRect->right - pRect->left;
	DWORD height = pRect->bottom - pRect->top;

	if((width == 0) || (height ==0 )){
		DP("[CGfxAndroid]Set destRect (width == 0) || (height ==0 )\n");
		return E_FAIL;
	}

	rd = *pRect;
	IntersectRect(&m_rectDst,&rd,&m_rectScn);

	OMX_CONFIG_POINTTYPE pointType;	
	OMX_ERRORTYPE err;
	err = OMX_GetConfig(g_appPriv->fbdev_sink_handle, OMX_IndexConfigCommonOutputPosition, &pointType);
	if(err != OMX_ErrorNone) 
		DP("-----------error in this line %d\n", __LINE__);

	pointType.nX = m_rectDst.left;
	pointType.nY = m_rectDst.top;
	err = OMX_SetConfig(g_appPriv->fbdev_sink_handle, OMX_IndexConfigCommonOutputPosition, &pointType);
	if(err != OMX_ErrorNone) 
		DP("-----------error in this line %d\n", __LINE__);

	OMX_FRAMESIZETYPE frameSizeType;
	err = OMX_GetConfig(g_appPriv->fbdev_sink_handle, OMX_IndexConfigCommonOutputSize, &frameSizeType);
	if(err != OMX_ErrorNone) 
		DP("-----------error in this line %d\n", __LINE__);

	frameSizeType.nWidth  = m_rectDst.right - m_rectDst.left;
	frameSizeType.nHeight  = m_rectDst.bottom  - m_rectDst.top;
	err= OMX_SetConfig(g_appPriv->fbdev_sink_handle, OMX_IndexConfigCommonOutputSize, &frameSizeType);
	if(err != OMX_ErrorNone) 
		DP("-----------error in this line %d\n", __LINE__);

	return S_OK;
}

STDMETHODIMP CGfxAndroid::SetScnRect(const RECT *pRect)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroid::GetScnRect(RECT *pRect)
{
	if(pRect == NULL)
		return E_FAIL;
	*pRect = m_rectScn;
	return S_OK;
}

STDMETHODIMP CGfxAndroid::SetWindowHandle(HWND hwnd)
{
	m_hwnd = hwnd;
	return S_OK;
}

STDMETHODIMP CGfxAndroid::GetWindowHandle(HWND& hwnd)
{
	hwnd = m_hwnd;
	return S_OK;
}

STDMETHODIMP CGfxAndroid::SetAlpha(DWORD dwAlpha)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroid::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroid::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroid::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroid::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroid::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroid::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroid::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
	if(ppBuf)
	{
		if(num == 0)
			*ppBuf = m_pInBufferSink1->pBuffer;
		else
			*ppBuf = m_pInBufferSink2->pBuffer;
	}
	
	if(pStride)
		*pStride = m_dwWidth;

	return S_OK;
}

STDMETHODIMP CGfxAndroid::Unlock(LONG num)
{
	return S_OK;
}

STDMETHODIMP CGfxAndroid::Flip(LONG num, DWORD dwFlags)
{
	OMX_ERRORTYPE err = OMX_ErrorNone;

	OMX_BUFFERHEADERTYPE *pBuff = NULL;
	pBuff = m_pInBufferSink1;
		
	int filledBufSize = pBuff->nAllocLen;
	pBuff->nFilledLen = filledBufSize;
	pBuff->nOffset = 0;

	retry:
	err = OMX_EmptyThisBuffer(g_appPriv->fbdev_sink_handle, pBuff);
	if(err != OMX_ErrorNone) 
	DP("-----------error in this line %d error code = %x\n", __LINE__, err);

	if(err == OMX_ErrorNotReady)
		goto retry;
	   
	return S_OK;
}

STDMETHODIMP CGfxAndroid::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;

	m_bShow = bShow;
	return Update();
}

STDMETHODIMP CGfxAndroid::Update()
{
	return S_OK;
}

STDMETHODIMP CGfxAndroid::GetSurfScaleRects(LONG num, RECT *pSrc, RECT *pDst, DWORD *pdwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroid::SetSurfScale(LONG num, RECT *pScale, DWORD dwFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxAndroid::GetSurfaceInfo(GfxOverlayInfo *pInfo)
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

STDMETHODIMP CGfxAndroid::IsOpen()
{
	return m_bOpen;
}

void CGfxAndroid::SetHeader(OMX_PTR header, OMX_U32 size) 
{
	OMX_VERSIONTYPE* ver = (OMX_VERSIONTYPE*)(header + sizeof(OMX_U32));
	*((OMX_U32*)header) = size;

	ver->s.nVersionMajor = VERSIONMAJOR;
	ver->s.nVersionMinor = VERSIONMINOR;
	ver->s.nRevision = VERSIONREVISION;
	ver->s.nStep = VERSIONSTEP;
	return;
}


OMX_ERRORTYPE CGfxAndroid::SetPortParameters() 
{
	OMX_ERRORTYPE err = OMX_ErrorNone;
	OMX_U32 outbuf_colorconv_size;

	outbuf_colorconv_size = m_dwWidth * m_dwHeight * m_bytesPerPixel;
	OMX_COLOR_FORMATTYPE colorFormatType =OMX_COLOR_FormatYUV420Planar;

	SetHeader(&m_omx_sinkPortDefinition, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));	
	m_omx_sinkPortDefinition.nPortIndex = 0; //sink input port index
	err = OMX_GetParameter(g_appPriv->fbdev_sink_handle, OMX_IndexParamPortDefinition, &m_omx_sinkPortDefinition);
	if(err != OMX_ErrorNone) 
	{
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
		return err;
	}

	m_omx_sinkPortDefinition.format.video.nFrameWidth = m_dwWidth;
	m_omx_sinkPortDefinition.format.video.nFrameHeight = m_dwHeight;
	m_omx_sinkPortDefinition.format.video.eColorFormat = colorFormatType;
	err = OMX_SetParameter(g_appPriv->fbdev_sink_handle, OMX_IndexParamPortDefinition, &m_omx_sinkPortDefinition);
	if(err != OMX_ErrorNone) {
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
		return err;
	}

	SetHeader(&m_omxVideoParam, sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE));

	m_omxVideoParam.nPortIndex = 0; //sink input port index
	err = OMX_GetParameter(g_appPriv->fbdev_sink_handle, OMX_IndexParamVideoPortFormat, &m_omxVideoParam);
	if(err != OMX_ErrorNone) 
	{
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
		return err;
	}

	m_omxVideoParam.eColorFormat = colorFormatType;
	err = OMX_SetParameter(g_appPriv->fbdev_sink_handle, OMX_IndexParamVideoPortFormat, &m_omxVideoParam);
	if(err != OMX_ErrorNone) 
	{
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
		return err;
	}
#if 0
	OMX_CONFIG_POINTTYPE pointType;	
	err = OMX_GetConfig(g_appPriv->fbdev_sink_handle, OMX_IndexConfigCommonOutputPosition, &pointType);
	if(err != OMX_ErrorNone) {
	DP("-----------error in this line %d\n", __LINE__);
	}

	pointType.nX = m_rectDst.left;
	pointType.nY = m_rectDst.top;
	err = OMX_SetConfig(g_appPriv->fbdev_sink_handle, OMX_IndexConfigCommonOutputPosition, &pointType);
	if(err != OMX_ErrorNone) 
	{
		DP("-----------error in this line %d\n", __LINE__);
	}
	err = OMX_GetConfig(g_appPriv->fbdev_sink_handle, OMX_IndexConfigCommonOutputPosition, &pointType);
	if(err != OMX_ErrorNone) 
	{
		DP("-----------error in this line %d\n", __LINE__);
	}

	OMX_FRAMESIZETYPE frameSizeType;
	err = OMX_GetConfig(g_appPriv->fbdev_sink_handle, OMX_IndexConfigCommonOutputSize, &frameSizeType);
	if(err != OMX_ErrorNone) 
	{
		DP("-----------error in this line %d\n", __LINE__);
	}

	frameSizeType.nWidth = 640;
	frameSizeType.nHeight = 360;
	err= OMX_SetConfig(g_appPriv->fbdev_sink_handle, OMX_IndexConfigCommonOutputSize, &frameSizeType);
	if(err != OMX_ErrorNone) {
	DP("-----------error in this line %d\n", __LINE__);
	}

	err = OMX_GetConfig(g_appPriv->fbdev_sink_handle, OMX_IndexConfigCommonOutputSize, &frameSizeType);
	DP("frameSizeTypeOMX_GetConfig------------------%d--\n", (int)frameSizeType.nWidth);
	DP("frameSizeTypeOMX_GetConfig------------------%d--\n", (int)frameSizeType.nHeight);
	if(err != OMX_ErrorNone) {
	DP("-----------error in this line %d\n", __LINE__);
	}
#endif
	err = OMX_SendCommand(g_appPriv->fbdev_sink_handle, OMX_CommandStateSet, OMX_StateIdle, NULL);
	if(err != OMX_ErrorNone) 
	{
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
		return err;
	}

	DP("====== reached here %d\n", __LINE__);

	err = OMX_AllocateBuffer(g_appPriv->fbdev_sink_handle, &m_pInBufferSink1, 0, NULL, outbuf_colorconv_size);
	if(err != OMX_ErrorNone) 
	{
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
		return err;
	}

	err = OMX_AllocateBuffer(g_appPriv->fbdev_sink_handle, &m_pInBufferSink2, 0, NULL, outbuf_colorconv_size);
	if(err != OMX_ErrorNone) 
	{
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
		return err;
	}

	err = OMX_SendCommand(g_appPriv->fbdev_sink_handle, OMX_CommandStateSet, OMX_StateExecuting, NULL);
	if(err != OMX_ErrorNone) 
	{
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
		return err;
	}

	tsem_down(g_appPriv->fbdevSinkEventSem);	

	return err;
}

OMX_ERRORTYPE CGfxAndroid::StopRender()
{
	OMX_ERRORTYPE err;

	err = OMX_SendCommand(g_appPriv->fbdev_sink_handle, OMX_CommandStateSet, OMX_StateIdle, NULL);
	if(err != OMX_ErrorNone) 
	{
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
		return err;
	}
	tsem_down(g_appPriv->fbdevSinkEventSem);	

	err = OMX_SendCommand(g_appPriv->fbdev_sink_handle, OMX_CommandStateSet, OMX_StateLoaded, NULL);
	if(err != OMX_ErrorNone) 
	{
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
		return err;
	}
	tsem_down(g_appPriv->fbdevSinkEventSem);	

	err = OMX_FreeBuffer(g_appPriv->fbdev_sink_handle, 0, m_pInBufferSink1);
	if(err != OMX_ErrorNone) 
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
	
	err = OMX_FreeBuffer(g_appPriv->fbdev_sink_handle, 0, m_pInBufferSink2);
	if(err != OMX_ErrorNone) 
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
	

	err = OMX_FreeHandle(g_appPriv->fbdev_sink_handle);
	if(err != OMX_ErrorNone) 
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
	

	err = OMX_Deinit();
	if(err != OMX_ErrorNone) 
		DP("-----------error in this line %d error code = %x\n", __LINE__, err);
	
	if(g_appPriv->fbdevSinkEventSem)
	{
		free(g_appPriv->fbdevSinkEventSem);
		g_appPriv->fbdevSinkEventSem = 0;
		free(g_appPriv);
		g_appPriv = NULL;
	}

	return OMX_ErrorNone;
}

OMX_ERRORTYPE CGfxAndroid::test_OMX_ComponentNameEnum() 
{
	char * name;
	int index;

	OMX_ERRORTYPE err = OMX_ErrorNone;

	name = (char*)malloc(OMX_MAX_STRINGNAME_SIZE);
	index = 0;
	while(1) 
	{
		err = OMX_ComponentNameEnum (name, OMX_MAX_STRINGNAME_SIZE, index);
		if ((name != NULL) && (err == OMX_ErrorNone)) 
		{
			DP( "component %2d is %s\n", index, name);
		} 
		else 
		{
			break;
		}
		if (err != OMX_ErrorNone) 
		{
			break;
		}
		index++;
	}
	free(name);
	name = NULL;
	return err;
}

OMX_ERRORTYPE  CGfxAndroid::PreInit()
{
	OMX_ERRORTYPE err;
	int data_read;
	int argn_dec;

	/* Initialize application private data */
	g_appPriv = (appPrivateType*)malloc(sizeof(appPrivateType));
	if(g_appPriv == NULL)
		return OMX_ErrorUndefined;
	
	g_appPriv->fbdevSinkEventSem = (tsem_t*)malloc(sizeof(tsem_t));
	if(g_appPriv->fbdevSinkEventSem  == NULL)
		return OMX_ErrorUndefined;
	tsem_init(g_appPriv->fbdevSinkEventSem, 0);

	err = OMX_Init();
	if (err != OMX_ErrorNone) {
		DP( "The OpenMAX core can not be initialized. Exiting...\n");
		return err;
	} 

	#ifdef NVIDIA_ANDROID
	char renderName[] = "OMX.Nvidia.std.iv_renderer.overlay.yuv420";
	#else
	char renderName[] = "OMX.Nvidia.std.iv_renderer.overlay.yuv420";
	#endif
	
	err = OMX_GetHandle(&g_appPriv->fbdev_sink_handle, renderName, NULL, &g_fbdev_sink_callbacks);
	if(err != OMX_ErrorNone){
		DP( "No video sink component component found. Exiting...\n");
		return err;
	} 
	return OMX_ErrorNone;
}


IMcGfxOverlay *CreateOverlayAndroid()
{
	return new CGfxAndroid();
}
#endif
