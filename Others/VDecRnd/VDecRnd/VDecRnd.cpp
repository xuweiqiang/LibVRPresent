// VDecRnd.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "VDecRnd.h"
#include "McCom.h"

#include <KD/kd.h>
#include <OMX_Core.h>
#include <OMX_Index.h>
#include <OMX_Video.h>
#include <OMX_Audio.h>
#include <OMX_Other.h>
#include <OMX_Image.h>
#include <NVOMX_IndexExtensions.h>
#include "../../inc/dp.h"
#include <list>

#pragma comment(lib, "libKD.lib")
#pragma comment(lib, "libnvkdmain.lib")
#pragma comment(lib, "libnvomx.lib")

using namespace std;
typedef struct
{
	char* pbuf;
	DWORD size;
}OMX_BUFF_STRUCT;

  
#define OMXSAFE(x) \
do { \
    if ((x) != OMX_ErrorNone) { \
        KDchar buf[512]; \
        int len; \
        kdStrcpy_s(buf, 512, "FAILED at "); \
        kdStrncat_s(buf, 512, __FILE__, kdStrlen(__FILE__)); \
        kdStrncat_s(buf, 512, ":", 1); \
        len = kdStrlen(buf); \
        kdLtostr(&buf[len], 512-len, __LINE__); \
        kdLogMessage(buf); \
        kdLogMessage("Exiting app"); \
        OMX_Deinit(); \
        kdExit(1); \
    } \
} while (0);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAC_CALLBACK_INIT		0
#define MAC_CALLBACK_START		1
#define MAC_CALLBACK_RUN		2

#define NOTSET_U8 ((OMX_U8)0xDE)
#define NOTSET_U16 ((OMX_U16)0xDEDE)
#define NOTSET_U32 ((OMX_U32)0xDEDEDEDE)
#define INIT_PARAM(_X_)  (memset(&(_X_), NOTSET_U8, sizeof(_X_)), ((_X_).nSize = sizeof (_X_)), (_X_).nVersion = m_vOMX)

static int m_callback_enable = MAC_CALLBACK_INIT;
static list<OMX_BUFF_STRUCT> g_con_list; 

// CVDecRndApp
class CVDecRndApp:
	public IMcVideoPresenter,
	public IMcVDecRnd_Decoder,
	public CMcUnknown
{
public:
	CVDecRndApp();
	virtual ~CVDecRndApp();

	DECLARE_IUNKNOWN
	STDMETHOD(NonDelegatingQueryInterface)(REFIID riid, LPVOID *ppvObj)
	{
		if(riid == IID_IMcVDecRnd_Decoder)
			return GetInterface(static_cast<IMcVDecRnd_Decoder*>(this), ppvObj);
		else if(riid == IID_IMcVideoPresenter)
			return GetInterface(static_cast<IMcVideoPresenter*>(this), ppvObj);
		
		return CMcUnknown::NonDelegatingQueryInterface(riid, ppvObj);
	}

	//Render part
	STDMETHODIMP Open(VP_OpenOptions *pOptions, DWORD dwOptionSize){return S_OK;}
	STDMETHODIMP Close(){return S_OK;}
	STDMETHODIMP CreateSurface(VR_SURFACE_TYPE type, VP_SURFACE_OpenOptions* pparam){return S_OK;}
	STDMETHODIMP DestroySurface(VR_SURFACE_TYPE type){return S_OK;}
	STDMETHODIMP Reset(){return S_OK;}
	STDMETHODIMP SetInterface(DWORD dwRoute, REFIID riid, LPUNKNOWN pUnk){return S_OK;}
	STDMETHODIMP SetVBI(VP_VIDEOBUFFER_INFO *pVBI){return S_OK;}
	STDMETHODIMP SetFPS(DWORD dwFPS){return S_OK;}
	STDMETHODIMP GetBackBuffers(DWORD *pdwBackBuffers){return S_OK;}
	STDMETHODIMP Restore(){return S_OK;}
	STDMETHODIMP GetStatus(DWORD *pdwStatus){return S_OK;}
	STDMETHODIMP GetCaps(VP_CAPS *pCaps){return S_OK;}

	STDMETHODIMP Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned);
	STDMETHODIMP Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData);
	STDMETHODIMP GetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned){return S_OK;}
	STDMETHODIMP SetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
		{return S_OK;}
	STDMETHODIMP PrepareBuffer(VR_SURFACE_TYPE type, LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStride, DWORD dwFlags){return S_OK;}
	STDMETHODIMP PresentBuffer(VR_SURFACE_TYPE type, LONG lNum, DWORD dwFlags){return S_OK;}
	STDMETHODIMP Present(VR_SURFACE_TYPE type, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags){return S_OK;}
	STDMETHODIMP PrepareBuffer(LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStride, DWORD dwFlags){return S_OK;}
	STDMETHODIMP PresentBuffer(LONG lNum, DWORD dwFlags){return S_OK;}
	STDMETHODIMP Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
	{return S_OK;}
	//Decoder Part
	STDMETHODIMP VDec_Open(DOpenOptions* pOptions, VDECRND_DECTYPE type);
	STDMETHODIMP VDec_Close();
	STDMETHODIMP VDec_Reset(DResetOptions* pOptions);
	STDMETHODIMP VDec_DecodeFrame(DDecodeOptions* pOptions, unsigned char** StreamBuffer,int BufLen);
	STDMETHODIMP VDec_GetFrame(DGetFrameOptions* pOptions, DFrame* pFrame);
	STDMETHODIMP VDec_ReleaseFrame(DFrame* pFrame);

protected:
	STDMETHODIMP UpdateDisplayParameters();
	static OMX_ERRORTYPE EventHandler(	\
	        OMX_IN OMX_HANDLETYPE hComponent,	\
	        OMX_IN OMX_PTR pAppData,			\
	        OMX_IN OMX_EVENTTYPE eEvent,		\
	        OMX_IN OMX_U32 nData1,				\
	        OMX_IN OMX_U32 nData2,				\
	        OMX_IN OMX_PTR pEventData);

	static OMX_ERRORTYPE EmptyBufferDone(	\
	        OMX_OUT OMX_HANDLETYPE hComponent,		\
	        OMX_OUT OMX_PTR pAppData,					\
	        OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer);	

	static OMX_ERRORTYPE FillBufferDone(	
	        OMX_OUT OMX_HANDLETYPE hComponent,				
	        OMX_OUT OMX_PTR pAppData,							
	        OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer);

	void setState(OMX_STATETYPE state, KDboolean wait);
	OMX_ERRORTYPE WaitForState(OMX_HANDLETYPE hComponent, OMX_STATETYPE eTestState, OMX_STATETYPE eTestState2);

private:
	OMX_VERSIONTYPE m_vOMX;

	OMX_CALLBACKTYPE m_oCallbacks;
	OMX_HANDLETYPE m_hVidRender ;
	OMX_BUFFERHEADERTYPE  **m_inputBuffer;
	int m_inputBufferCount;
	static OMX_HANDLETYPE m_vdecoder;

	BOOL m_bOpen;
	RECT m_rDst;		// destination rectangle to store the image, relative to video window/port.
	CRITICAL_SECTION m_csDisplay; 
	LONG m_lZoomMode;
	LONG m_lInterpolationMode;
	LONG m_lRotationMode;
	LONG m_lDeblockMode;
	LONG m_lResampleMode;
	LONG m_lRenderEnabled;
	LONG m_alCC[10];
	BOOL m_bZoomEnable;
	LONG m_overlayType;
	RECT m_rSrcSize;	// source size (also used as source clipping region) 
	RECT m_rScreen;		// size of currently active screen

};

static CVDecRndApp* g_singleObject = NULL; 

// CVDecRndApp construction

#define COMPNENT_NAME TEXT("DecRnd_Component")

OMX_HANDLETYPE CVDecRndApp::m_vdecoder = 0;

CVDecRndApp::CVDecRndApp():
	CMcUnknown(COMPNENT_NAME,0)
{
	memset(&m_vOMX,0, sizeof(m_vOMX));
	memset(&m_oCallbacks,0, sizeof(m_oCallbacks));;
	memset(&m_hVidRender, 0,sizeof(m_hVidRender));
	m_inputBuffer = 0;
	m_inputBufferCount = 0;
	m_bOpen = 0;
	memset(&m_rDst,0, sizeof(m_rDst));
	m_callback_enable = MAC_CALLBACK_INIT;
}

CVDecRndApp::~CVDecRndApp()
{

}


STDMETHODIMP CVDecRndApp::Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
{
	if(!m_bOpen)
			return E_FAIL;
	switch (dwPropID)
	{
	case MC_PROPID_VP_RECTDST:
		{
			if(pPropData == NULL)
				return E_INVALIDARG;
			RECT r = *reinterpret_cast<RECT *>(pPropData);
			if(!EqualRect(&m_rDst,&r))
			{
				m_rDst = r;
				UpdateDisplayParameters();
			}
			return S_OK;
		}
	default:
		break;
	}

	return E_FAIL;
}


STDMETHODIMP CVDecRndApp::Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
{

	switch (dwPropID)
	{
	case MC_PROPID_VP_ZOOM_ENABLE:
		return ReturnComLong(m_bZoomEnable,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_ZOOM:
		return ReturnComLong(m_lZoomMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_ROTATION:
		return ReturnComLong(m_lRotationMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_INTERPOLATION:
		return ReturnComLong(m_lInterpolationMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_DEBLOCK:
		return ReturnComLong(m_lDeblockMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_RESAMPLE:
		return ReturnComLong(m_lResampleMode,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_ENABLE:
		return ReturnComLong(m_lRenderEnabled,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_TYPE:
		return ReturnComLong(m_overlayType,pPropData,cbPropData,pcbReturned);
	case MC_PROPID_VP_RECTDST:
		{
			if(pPropData==0)
				return E_POINTER;
			*reinterpret_cast<RECT *>(pPropData) = m_rDst;
			if(pcbReturned)
				*pcbReturned = sizeof(RECT);
			return S_OK;
		}
	case MC_PROPID_VP_RECTSRC:
		{
			if(pPropData==0)
				return E_POINTER;
			*reinterpret_cast<RECT *>(pPropData) = m_rSrcSize;
			if(pcbReturned)
				*pcbReturned = sizeof(RECT);
			return S_OK;
		}
	case MC_PROPID_VP_RECT_SCREEN:
		{
			if(pPropData==0)
				return E_POINTER;
			*reinterpret_cast<RECT *>(pPropData) = m_rScreen;
			if(pcbReturned)
				*pcbReturned = sizeof(RECT);
			return S_OK;
		}
	case MC_PROPID_VP_WND:
		{
			HRESULT ret = E_FAIL;
		#ifdef __linux__
			Window wnd;
		#else
			HWND wnd;
		#endif
			if(pPropData==0)
				return E_POINTER;
			ZeroMemory(&wnd,sizeof(wnd));
		#ifdef __linux__
			*reinterpret_cast<Window *>(pPropData) = wnd;
		#else			
			*reinterpret_cast<HWND *>(pPropData) = wnd;
		#endif
			if(pcbReturned)
				*pcbReturned = sizeof(HWND);
			return S_OK;
		}
	}
	return E_PROP_ID_UNSUPPORTED;
}

OMX_ERRORTYPE CVDecRndApp::EventHandler(
        OMX_IN OMX_HANDLETYPE hComponent,
        OMX_IN OMX_PTR pAppData,
        OMX_IN OMX_EVENTTYPE eEvent,
        OMX_IN OMX_U32 nData1,
        OMX_IN OMX_U32 nData2,
        OMX_IN OMX_PTR pEventData)
{
    //switch (eEvent)
    //{
    //default:
        {
            OMX_VERSIONTYPE v1, v2;
            OMX_UUIDTYPE uuid;
            KDchar name[128];

            OMX_GetComponentVersion(hComponent, name, &v1, &v2, &uuid);
            DP("%s event: 0x%x, nData1: 0x%x, nData2: 0x%x\n", name, eEvent, nData1, nData2);
            //break;
        }
    //}

    return OMX_ErrorNone;
}


OMX_ERRORTYPE CVDecRndApp::EmptyBufferDone(
        OMX_OUT OMX_HANDLETYPE hComponent,
        OMX_OUT OMX_PTR pAppData,
        OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer)
{
	//if(m_callback_enable == MAC_CALLBACK_INIT)
	//	m_callback_enable = MAC_CALLBACK_START; 	
	if(m_callback_enable != MAC_CALLBACK_RUN)
	    return OMX_ErrorNone;


	OMX_BUFF_STRUCT buff = g_con_list.front();
	memcpy(pBuffer->pBuffer, buff.pbuf, buff.size);
	pBuffer->nFilledLen = buff.size;
	g_con_list.pop_front(); 
	pBuffer->nOffset = 0;
	pBuffer->nFlags = OMX_BUFFERFLAG_ENDOFFRAME;
	OMXSAFE(OMX_EmptyThisBuffer(m_vdecoder, pBuffer));
    	DP("send in buffer %d\n", GetTickCount());

    return OMX_ErrorNone;
}

OMX_ERRORTYPE CVDecRndApp::FillBufferDone(
        OMX_OUT OMX_HANDLETYPE hComponent,
        OMX_OUT OMX_PTR pAppData,
        OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer)
{
	//FILE * fp;
    static int framecount = 0;
    // TODO:
    // - Handle the data in pBuffer.
    // - call OMX_FillThisBuffer(hComponent, pBuffer) to refill it with new data
/*
    if(!framecount)
    {
        fp = fopen("\\Release\\wuniang.yuv", "wb");
	    fwrite(pBuffer->pBuffer, 1, pBuffer->nFilledLen, fp);
	    fclose(fp);
    }
*/
	DP("get one frame %d, %d, %d\n", ++framecount, pBuffer->nFilledLen, GetTickCount());
    if(pBuffer->nFilledLen)
    {
    	OMX_FillThisBuffer(hComponent, pBuffer);
    }

    return OMX_ErrorNone;
}


OMX_ERRORTYPE CVDecRndApp::WaitForState(OMX_HANDLETYPE hComponent, OMX_STATETYPE eTestState,
                                  OMX_STATETYPE eTestState2)
{
       OMX_STATETYPE eState;
    OMX_ERRORTYPE eError = OMX_ErrorNone;


    OMXSAFE(eError = OMX_GetState(hComponent, &eState));

    while (eState != eTestState && eState != eTestState2) {
        Sleep(10);

        OMXSAFE(eError = OMX_GetState(hComponent, &eState));
    }
    kdLogMessage("Done waiting!\n");

    return eError;
}

void CVDecRndApp::setState(OMX_STATETYPE state, KDboolean wait)
{
    char buf[20];
    kdLogMessage("Changing state to:");
    kdLtostr(buf, 20, (int) state);
    kdLogMessage(buf);
    OMXSAFE(OMX_SendCommand(m_vdecoder,  OMX_CommandStateSet, state, 0));
    OMXSAFE(OMX_SendCommand(m_hVidRender,  OMX_CommandStateSet, state, 0));

    if (wait)
    {
	    WaitForState(m_hVidRender, state, OMX_StateInvalid);
	    WaitForState(m_vdecoder,  state, OMX_StateInvalid);
    }
}

STDMETHODIMP CVDecRndApp::VDec_Open(DOpenOptions* pOptions, VDECRND_DECTYPE type)
{
	if(m_bOpen == TRUE)
		return E_FAIL;

    m_vOMX.s.nVersionMajor = 1;
    m_vOMX.s.nVersionMinor = 1;
    m_vOMX.s.nRevision = 0;
    m_vOMX.s.nStep = 0;


    m_oCallbacks.EventHandler    = EventHandler;
    m_oCallbacks.EmptyBufferDone = EmptyBufferDone;
    m_oCallbacks.FillBufferDone  = FillBufferDone;


	m_callback_enable = MAC_CALLBACK_INIT;

	OMXSAFE(OMX_Init());

	if(type == MPEG2)
	{
		OMXSAFE(OMX_GetHandle(&m_vdecoder,  "OMX.Nvidia.mpeg2v.decode",     KD_NULL, &m_oCallbacks));
	}
	else
	{
		OMXSAFE(OMX_GetHandle(&m_vdecoder,  "OMX.Nvidia.h264.decode",     KD_NULL, &m_oCallbacks));
	}

	OMXSAFE(OMX_GetHandle(&m_hVidRender, "OMX.Nvidia.std.iv_renderer.overlay.yuv420",  KD_NULL, &m_oCallbacks));

	OMX_INDEXTYPE eIndex;
	NVX_CONFIG_KEEPASPECT keepAspect;
	INIT_PARAM(keepAspect);
	keepAspect.bKeepAspect = OMX_FALSE;
	if (OMX_ErrorNone == OMX_GetExtensionIndex(m_hVidRender, 
	    NVX_INDEX_CONFIG_KEEPASPECT, &eIndex))
	{
	    OMX_SetConfig(m_hVidRender, eIndex,
	        &keepAspect); 
	}

    // Tunnel the video decoder and renderer
	OMX_ERRORTYPE  omx_error_type = OMX_ErrorNone;
	omx_error_type = OMX_SetupTunnel(m_vdecoder, 1, m_hVidRender, 0);

    kdLogMessage("Request to transition to idle");
    setState(OMX_StateIdle, KD_FALSE);

    kdLogMessage("Setup buffers");
   {
        OMX_PARAM_PORTDEFINITIONTYPE decPortDef;
        unsigned int i;
        //int bufferSize = 0;

        // Find out how many buffers and what sizes they should be for the h264 decoder on port 0
	INIT_PARAM(decPortDef);
        decPortDef.nPortIndex = 0;
        OMXSAFE(OMX_GetParameter(m_vdecoder, OMX_IndexParamPortDefinition, &decPortDef));


        m_inputBufferCount = decPortDef.nBufferCountMin;
       m_inputBuffer = (OMX_BUFFERHEADERTYPE **) kdMalloc(decPortDef.nBufferCountMin * sizeof(OMX_BUFFERHEADERTYPE*));

        // Allocate buffers for the video decoder on port 0
        for (i = 0; i < decPortDef.nBufferCountMin; i++) {
            // Note: the buffer size might have to be tweaked just as for the output buffers if the default
            // buffersize is too small to fit the h264 frame.
            OMXSAFE(OMX_AllocateBuffer(m_vdecoder, &m_inputBuffer[i], decPortDef.nPortIndex, NULL, decPortDef.nBufferSize));
        }

    }

     WaitForState(m_hVidRender, OMX_StateIdle, OMX_StateInvalid);
    WaitForState(m_vdecoder,  OMX_StateIdle, OMX_StateInvalid);

    kdLogMessage("set state to executing");
    setState(OMX_StateExecuting, KD_TRUE);

	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CVDecRndApp::VDec_Close()
{
	if(m_bOpen == FALSE)
		return S_OK;

	setState(OMX_StateIdle, KD_TRUE);
    setState(OMX_StateLoaded, KD_FALSE);

    // Again, we need to manually free the buffers during this transition
    kdLogMessage("Freeing buffers\n");

	for (int i = 0; i < m_inputBufferCount; i++)
	{
		OMX_FreeBuffer(m_vdecoder, 0, m_inputBuffer[i]);
		m_inputBuffer[i] = NULL;
	}
    kdFree(m_inputBuffer);


    WaitForState(m_hVidRender, OMX_StateLoaded, OMX_StateInvalid);
    WaitForState(m_vdecoder,  OMX_StateLoaded, OMX_StateInvalid);
	m_inputBufferCount = 0;

	OMXSAFE(OMX_FreeHandle(m_vdecoder));
    OMXSAFE(OMX_FreeHandle(m_hVidRender));
    OMXSAFE(OMX_Deinit());


	m_bOpen= FALSE;
	return S_OK;
}

STDMETHODIMP CVDecRndApp::UpdateDisplayParameters()
{
	RECT zero_rect;
	SetRect(&zero_rect, 0, 0, 0, 0);
	if(memcmp(&zero_rect , &m_rDst , sizeof(RECT)) == 0)
		return S_OK;
	OMX_ERRORTYPE ret = OMX_ErrorNone;
	OMX_CONFIG_POINTTYPE pos;
	INIT_PARAM(pos);
	pos.nX = m_rDst.left;
	pos.nY = m_rDst.top;
	ret = OMX_SetConfig(m_hVidRender, OMX_IndexConfigCommonOutputPosition, &pos); 

	OMX_FRAMESIZETYPE size;
	INIT_PARAM(size);
	size.nWidth = m_rDst.right - m_rDst.left;
	size.nHeight = m_rDst.bottom - m_rDst.top;
	ret = OMX_SetConfig(m_hVidRender, OMX_IndexConfigCommonOutputSize, &size); 

	return S_OK;
}


STDMETHODIMP CVDecRndApp::VDec_Reset(DResetOptions* pOptions)
{
	return S_OK;
}

STDMETHODIMP CVDecRndApp::VDec_DecodeFrame(DDecodeOptions* pOptions, unsigned char** StreamBuffer,int BufLen)
{
	if(m_callback_enable == MAC_CALLBACK_INIT)
	{
		static int index = 0;

		int i = index;
		{
			// TODO:
			// - Fill a decoderBuffer[i] with data, set the timestamp, buffer length, buffer flags etc
			// - call OMX_EmptyThisBuffer(vdecoder, decoderBuffer[i]);
			memcpy(m_inputBuffer[i]->pBuffer, *StreamBuffer /*+ size*i*/, BufLen);
			m_inputBuffer[i]->nFilledLen = BufLen;
			m_inputBuffer[i]->nOffset = 0;
			m_inputBuffer[i]->nFlags = OMX_BUFFERFLAG_ENDOFFRAME;
			m_inputBuffer[i]->nTimeStamp = 0;
			OMXSAFE(OMX_EmptyThisBuffer(m_vdecoder, m_inputBuffer[i]));
			DP("[ED}send in buffer %d\n", i);
		}
		if(++index >= m_inputBufferCount)
			index = 0;
		
	}
	else
	{
		OMX_BUFF_STRUCT buff_info;
		memset(&buff_info, 0, sizeof(buff_info));
		buff_info.size = BufLen;
		buff_info.pbuf = (char*)malloc(buff_info.size);
		memcpy(buff_info.pbuf, *StreamBuffer,buff_info.size);
		g_con_list.push_back(buff_info);
		m_callback_enable =MAC_CALLBACK_RUN; 
	}

return S_OK;
}
STDMETHODIMP CVDecRndApp::VDec_GetFrame(DGetFrameOptions* pOptions, DFrame* pFrame)
{
	return S_OK;
}
STDMETHODIMP CVDecRndApp::VDec_ReleaseFrame(DFrame* pFrame)
{
	return S_OK;
}


IMcVDecRnd_Decoder *CreateVDecRnd()
{
	if(g_singleObject == NULL)
		g_singleObject = new CVDecRndApp();
	else
		g_singleObject->AddRef();
	return g_singleObject;
}


// The one and only CVDecRndApp object

//CVDecRndApp theApp;


