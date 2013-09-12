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

#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <windows.h>
//#include <yuv2rgb_c.h>

#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
 ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))

#include "McCom.h"
#include "GfxPresent.h"

#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <X11/Xlib.h>

inline int WidthRect(const RECT *r)
{
	return r->right - r->left;
}

inline int HeightRect(const RECT *r)
{
	return r->bottom - r->top;
}

static SDL_SysWMinfo sdlwminfo;


#define MAX_FOURCC_NUMBER	30
enum
{
	HARDWARE_VIDEO_EFFECT,
		SOFTWARE_VIDEO_EFFECT,
};
typedef struct
{
	RECT m_rSrcSize;	
		RECT m_rSrcClip;	
		RECT m_rDst;
		BOOL bOpen;
}VP_HANDLE;


//-----------------------------------------------------
//#define pr_func printf("[INFO]\t%s\n", __FUNCTION__);
#define pr_func

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
//#define OVERLAY_ON_QTWIDGET

#ifdef OVERLAY_ON_QTWIDGET

#define SHM_KEY 19491001
#define ENVSTR_SIZE     64
struct package_t {
	pid_t pid;
	int strleng;
	char envstr[ENVSTR_SIZE];
} *shm;
#endif
//-----------------------------------------------------

class CVRPresentSDL : 
	public IMcVideoPresenter ,
	public CMcUnknown
{
	public:
		DECLARE_IUNKNOWN

		CVRPresentSDL();
		virtual ~CVRPresentSDL();
		STDMETHODIMP Open(VP_OpenOptions *pOptions, DWORD dwOptionSize);
		STDMETHODIMP Close();
		STDMETHODIMP  CreateSurface(VR_SURFACE_TYPE type, VP_SURFACE_OpenOptions* pparam);
		STDMETHODIMP  DestroySurface(VR_SURFACE_TYPE type);
		STDMETHODIMP Reset();
		STDMETHODIMP SetInterface(DWORD dwRoute, REFIID riid, LPUNKNOWN pUnk);
		STDMETHODIMP SetVBI(VP_VIDEOBUFFER_INFO *pVBI);
		STDMETHODIMP SetFPS(DWORD dwFPS);
		STDMETHODIMP GetBackBuffers(DWORD *pdwBackBuffers);
		STDMETHODIMP Restore();
		STDMETHODIMP GetStatus(DWORD *pdwStatus);
		STDMETHODIMP GetCaps(VP_CAPS *pCaps);

		STDMETHODIMP Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned);
		STDMETHODIMP Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData);
		STDMETHODIMP GetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned);
		STDMETHODIMP SetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData);
		STDMETHODIMP PrepareBuffer(VR_SURFACE_TYPE type, LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStride, DWORD dwFlags);
		STDMETHODIMP PresentBuffer(VR_SURFACE_TYPE type, LONG lNum, DWORD dwFlags);
		STDMETHODIMP Present(VR_SURFACE_TYPE type, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);

		STDMETHODIMP Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
		STDMETHODIMP PrepareBuffer(LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
		STDMETHODIMP PresentBuffer(LONG lNum, DWORD dwFlags);

	protected:
		STDMETHODIMP Present_Internal(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
		STDMETHODIMP PrepareBuffer_Internal(VR_SURFACE_TYPE type, LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags);
		STDMETHODIMP PresentBuffer_Internal(VR_SURFACE_TYPE type, LONG lNum, DWORD dwFlags);
		HRESULT SetSrcSize(DWORD dwWidth, DWORD dwHeight);
		HRESULT UpdateDisplayParameters();
		HRESULT ConvertPrecision(LONG& lSrcVal, const LONG lDstMax, const LONG lDstMin, const LONG lSrcMax,const LONG lSrcMin);
		HRESULT InitColorControl();
		HRESULT RestoreColorControl();

		CRITICAL_SECTION m_csDisplay; 

		LONG m_lZoomMode;
		LONG m_lInterpolationMode;
		LONG m_lRotationMode;
		LONG m_lDeblockMode;
		LONG m_lResampleMode;
		LONG m_lRenderEnabled;
		LONG m_alCC[10];
		BOOL m_bZoomEnable;

		VP_VIDEOBUFFER_INFO m_VBI;

		//
		// Video Port information
		//
		RECT m_rDst;		// destination rectangle to store the image, relative to video window/port.
		RECT m_rSrcSize;	// source size (also used as source clipping region) 
		RECT m_rScreen;		// size of currently active screen
		DWORD m_dwStatus;
		DWORD m_dwPresentIndex;	// the current frame to be used by Present

		unsigned int    m_FPS;
		VP_OpenOptions m_OpenOptions;
		LONG m_overlayType;

		unsigned char * m_pBuff;
		int m_iBufSize;

//----------------------------------------------
		SDL_Surface *m_sdlsurface;
		SDL_Overlay *m_sdlyuvoverlay;
		SDL_Thread *m_eventloop_thread;
		#ifdef OVERLAY_ON_QTWIDGET

		char envstr[ENVSTR_SIZE];
//		char *envstr;
#endif

//----------------------------------------------

		DWORD surfaceFourCC;
		static const DWORD const_SurfFourCC[];
		static const DWORD const_InputFourCC[];

		BOOL m_bOpen;
};


const DWORD CVRPresentSDL::const_SurfFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
		MAKEFOURCC('Y','U','Y','2'),
		MAKEFOURCC('R','5','6','5'),
		MAKEFOURCC('R','8','8','8'),
		0
};

#define DEFAULT_INPUT_FOURCC	MAKEFOURCC('R','8','8','8')
const DWORD CVRPresentSDL::const_InputFourCC[] =
{
	MAKEFOURCC('R','8','8','8'),
		0
};

static const DWORD const_dwFourCC[] =
{
	MAKEFOURCC('Y','V','1','2'),
		MAKEFOURCC('Y','U','Y','2'),
		MAKEFOURCC('R','5','6','5'),
		MAKEFOURCC('R','8','8','8'),
		0
};

#define VIDEO_PRESENTER_NAME TEXT("Video Presenter")
#define HARDWARE_VIDEO_EFFECT_INIT_VALUE	128
#define SOFTWARE_VIDEO_EFFECT_INIT_VALUE	128
//CVRPresentSDL *pvp = NULL;
static bool quit = false;

int eventloop_func(void* arg)
{
	SDL_Event event;
	int i=0;

	atexit(SDL_Quit);

	while( !quit )
	{
	        while( SDL_PollEvent( &event ) )
	        {
	                switch( event.type )
	                {
	/*
	                    case SDL_KEYDOWN:
	                            switch( event.key.keysym.sym )
	                            {
	                                case SDLK_ESCAPE:
	                                case SDLK_q:
							{
								i++;
	                                        quit = true;
							printf("SDLK_q down %d\n", i);
		                                }
	                                        break;
	                                default:
	                                        break;
	                            }
	                            break;
	*/
				case SDL_MOUSEBUTTONDOWN:
					{
						i++;
						if(i < 10000)
							printf("mouse down %d\n", i);
						
/*
						sdlwminfo.info.x11.lock_func();
						XMoveWindow(sdlwminfo.info.x11.display, sdlwminfo.info.x11.window, i, i);
						sdlwminfo.info.x11.unlock_func();
*/
					}
					break;
	                    case SDL_QUIT:
	                            quit = true;
	                            break;
	                    default:
	                            break;
	                }
	        }
	        //SDL_Delay(1000);
	}	
//	SDL_Quit();
	return 0;
}


static SDL_SysWMinfo wminfo;
#ifdef OVERLAY_ON_QTWIDGET

static void getwinid(void)
{pr_func
	int shmid;
	key_t key;
	//char *s;

	key = SHM_KEY;

	if ((shmid = shmget(key, sizeof(struct package_t), 0666)) < 0) {
	    perror("shmget");
	    exit(1);
	}

	if ((shm = (struct package_t *)shmat(shmid, NULL, 0)) == (struct package_t *) -1) {
	    perror("shmat");
	    exit(1);
	}
	
	//printf("lib pid %d, envstr %s\n", shm->pid, shm->envstr);
	
}
#endif

/*
#include <signal.h>
struct sigaction sigint_act;
void sig_handler(int sig)
{
	printf("%s sig %d\n", __FUNCTION__, sig);
}
void reg_sigaction(void)
{
	sigint_act.sa_flags = 0;
	sigemptyset(&sigint_act.sa_mask);
	sigaddset(&sigint_act.sa_mask, SIGUSR2);
	sigint_act.sa_handler = sig_handler;
	sigaction(SIGINT, &sigint_act, 0);
}
*/
CVRPresentSDL::CVRPresentSDL() :
	m_lInterpolationMode(0),
	m_lRotationMode(VP_ROTATION_0),
	m_lDeblockMode(0),
	m_lResampleMode(0),
	m_lRenderEnabled(TRUE),
	m_bZoomEnable(TRUE),
	m_dwStatus(NULL),
	m_dwPresentIndex(0),
	m_overlayType(0),
	CMcUnknown(VIDEO_PRESENTER_NAME,0),
	m_bOpen(FALSE),
	m_pBuff(NULL),
	m_iBufSize(0),
	m_eventloop_thread(0)
{
		ZeroMemory(&m_rSrcSize,sizeof(m_rSrcSize));
		ZeroMemory(&m_rSrcSize,sizeof(m_rSrcSize));
		ZeroMemory(&m_rDst,sizeof(m_rDst));
		ZeroMemory(&m_rScreen,sizeof(m_rScreen));
		ZeroMemory(&m_alCC,sizeof(m_alCC));
		ZeroMemory(&m_VBI,sizeof(m_VBI));
		ZeroMemory(&m_OpenOptions,sizeof(m_OpenOptions));
		//	ZeroMemory(&m_screeninfo,sizeof(m_screeninfo));
		
		InitializeCriticalSection(&m_csDisplay);
		
}

CVRPresentSDL::~CVRPresentSDL()
{	
	Close();
	DeleteCriticalSection(&m_csDisplay);
}

STDMETHODIMP CVRPresentSDL::Open(VP_OpenOptions *pOptions, DWORD dwOptionSize)
{pr_func
	//printf("CVRPresentSDL::Open\n");//fflush(stdout);
	
//printf("CVRPresentSDL::Open()\n");
	if (pOptions == NULL || sizeof(m_OpenOptions) != dwOptionSize)
		return E_FAIL;
	if(pOptions != &m_OpenOptions)
	{
		memset(&m_OpenOptions,0,sizeof(m_OpenOptions));
	}
	memcpy(&m_OpenOptions,pOptions,sizeof(m_OpenOptions));

	int ret;
/*
	if(!envstr)
	{	
		envstr = (char*)malloc(ENVSTR_SIZE);
		printf("malloc for envstr %x\n", envstr);
		
	}
*/
#ifdef OVERLAY_ON_QTWIDGET

	if(shm->envstr)
	{
		memset(envstr, 0, ENVSTR_SIZE);
		getwinid();
		memcpy(envstr, shm->envstr, ENVSTR_SIZE);
		ret = SDL_putenv(envstr);
		if(ret)
			printf("SDL_putenv error\n");
		
	}
#endif

/*
	ret = SDL_putenv("SDL_VIDEO_CENTERED=1");
	//ret = SDL_putenv("SDL_VIDEO_WINDOW_POS=center");
	if(ret)
		printf("SDL_putenv error\n");
*/
	
	SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL_Init error\n");
		return E_FAIL;
	}
	
	// TODO: replace 16 to depth
//	if (!(m_sdlsurface = SDL_SetVideoMode(m_OpenOptions.dwWidth, m_OpenOptions.dwHeight, 0, SDL_HWSURFACE|SDL_ASYNCBLIT|SDL_HWACCEL|SDL_NOFRAME)))
	if (!(m_sdlsurface = SDL_SetVideoMode(m_OpenOptions.dwWidth, m_OpenOptions.dwHeight, 0, SDL_HWSURFACE | SDL_DOUBLEBUF|SDL_RESIZABLE)))
	{
		SDL_Quit();
		printf("SDL_SetVideoMode error\n");
		return E_FAIL;
	}
	//printf("surface rect %d, %d, %d, %d\n", m_sdlsurface->clip_rect.x, m_sdlsurface->clip_rect.y, m_sdlsurface->clip_rect.w, m_sdlsurface->clip_rect.h);

	m_sdlyuvoverlay = SDL_CreateYUVOverlay(m_OpenOptions.dwWidth, m_OpenOptions.dwHeight, SDL_IYUV_OVERLAY, m_sdlsurface);
	if(NULL == m_sdlyuvoverlay)
	{
		SDL_Quit();
		printf("SDL_CreateYUVOverlay error\n");
		return E_FAIL;
	}
	//m_sdlsurface->clip_rect.x = 200; //, m_sdlsurface->clip_rect.y, m_sdlsurface->clip_rect.w, m_sdlsurface->clip_rect.h
#if 0
	SDL_VERSION(&wminfo.version);
	//SDL_GetWMInfo(&wminfo);
	//printf("wminfo %x\n", wminfo.info.x11.window);
	if (SDL_GetWMInfo(&wminfo) > 0 ) {
		if (wminfo.subsystem == SDL_SYSWM_X11) {
			printf("wminfo %x\n", wminfo.info.x11.window);
			wminfo.info.x11.lock_func();
			XMoveWindow(wminfo.info.x11.display, wminfo.info.x11.window, 100, 100);
			wminfo.info.x11.unlock_func();
		}
	}
	else
		printf("SDL_GetWMInfo error\n");
#endif

/*
	m_eventloop_thread = SDL_CreateThread(eventloop_func, NULL);
	if ( m_eventloop_thread == NULL ) {
	    fprintf(stderr, "Unable to create thread: %s\n", SDL_GetError());
	    return E_FAIL;
	}
*/

	
//	printf("m_sdlsurface->pitch %d\n", m_sdlsurface->pitch);
//	printf("m_sdlyuvoverlay->pitches[0] %d, m_sdlyuvoverlay->pitches[1] %d, m_sdlyuvoverlay->pitches[2] %d\n", m_sdlyuvoverlay->pitches[0], m_sdlyuvoverlay->pitches[1], m_sdlyuvoverlay->pitches[2]);
//	printf("m_sdlyuvoverlay->format %d, m_sdlyuvoverlay->w %d , m_sdlyuvoverlay->h %d, m_sdlyuvoverlay->pitches %x, m_sdlyuvoverlay->pixels %x\n", m_sdlyuvoverlay->format, m_sdlyuvoverlay->w, m_sdlyuvoverlay->h, m_sdlyuvoverlay->pitches, m_sdlyuvoverlay->pixels);
//	printf("m_OpenOptions.dwWidth=%d, m_OpenOptions.dwHeight=%d, m_sdlsurface->format->BitsPerPixel=%d\n", m_OpenOptions.dwWidth, m_OpenOptions.dwHeight, m_sdlsurface->format->BitsPerPixel);

	m_bOpen = TRUE;
	return S_OK;
}

STDMETHODIMP CVRPresentSDL::Close()
{
	if(!m_bOpen)
		return S_OK;
	//printf("CVRPresentSDL::Close()\n");
	if(m_sdlyuvoverlay)
	{
		SDL_FreeYUVOverlay(m_sdlyuvoverlay);
		m_sdlyuvoverlay = 0;
	}

/*
	if(envstr){
		printf("CVRPresentSDL::Close() envstr %x\n", envstr);
		free(envstr);
		envstr=0;
		printf("CVRPresentSDL::Close() envstr freed\n");
	}
*/
	#ifdef OVERLAY_ON_QTWIDGET

	int ret;
	ret = shmdt((void*)shm);
	if(ret){
		perror("shmdt");
	}else{
		//printf("shm %x detached from lib side\n", shm);
	}
	#endif

	quit = TRUE;
	if(m_eventloop_thread)
		SDL_WaitThread(m_eventloop_thread, NULL);
//	SDL_KillThread(m_eventloop_thread);
	SDL_Quit();
	
//	yuv2rgb_c_uninit();
	
	m_bOpen = FALSE;
	
	return S_OK;
}

STDMETHODIMP  CVRPresentSDL::CreateSurface(VR_SURFACE_TYPE type, VP_SURFACE_OpenOptions* pparam)
{
	if(!m_bOpen)
		return E_FAIL;
			
	if(pparam == NULL)
		return E_FAIL;		
					
	return S_OK;
}

STDMETHODIMP  CVRPresentSDL::DestroySurface(VR_SURFACE_TYPE type)
{
	if(!m_bOpen)
		return E_FAIL;
			
	return S_OK;
}

STDMETHODIMP CVRPresentSDL::Reset()
{
	HRESULT res = S_OK;
	res = Close();
	if(FAILED(res))
		return E_FAIL;
				
	res = Open(&m_OpenOptions,sizeof(m_OpenOptions));
	if(FAILED(res))
		return E_FAIL;
						
	return S_OK;
}

HRESULT CVRPresentSDL::InitColorControl()
{
	return S_OK;
}

HRESULT CVRPresentSDL::RestoreColorControl()
{
	LONG value;
 	return S_OK;
}

STDMETHODIMP CVRPresentSDL::SetInterface(DWORD dwRoute, REFIID riid, LPUNKNOWN pUnk)
{
	return E_FAIL;
}

STDMETHODIMP CVRPresentSDL::Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
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
	case MC_PROPID_VP_BRIGHTNESS:
	case MC_PROPID_VP_CONTRAST:
	case MC_PROPID_VP_HUE:
	case MC_PROPID_VP_SATURATION:
	case MC_PROPID_VP_GAMMA:
	case MC_PROPID_VP_COLORENABLE:
	case MC_PROPID_VP_SHARPNESS:
		{
			return S_OK;
		}
	case MC_PROPID_VP_COLORSYS_CAP:
		{
			return S_OK;
		}
	case MC_PROPID_VP_WND:
		{
			return S_OK;
		}
	}
	return E_PROP_ID_UNSUPPORTED;
}

static void printrectinfo(RECT r)
{
	printf("^^^^^^^^^^^^^^^^^^^^^^r=%d,%d,%d,%d\n", r.top, r.left, r.right-r.left, r.bottom-r.top);	
}


STDMETHODIMP CVRPresentSDL::Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
{
	if(!m_bOpen)
		return E_FAIL;

	BOOL bRefresh = FALSE;

	switch (dwPropID)
	{
	case MC_PROPID_VP_ZOOM_ENABLE:
		{
			LONG lZoomEnable = *(BOOL *)pPropData;
			m_bZoomEnable = lZoomEnable ;
			return S_OK;
		}
	case MC_PROPID_VP_ZOOM:
		{
			LONG lZoomMode = *(LONG *)pPropData;

			return  S_FALSE;
		}
	case MC_PROPID_VP_ROTATION:
		{
			LONG lRotationMode = *(LONG *)pPropData;

			return S_FALSE;
		}
	case MC_PROPID_VP_RECTDST:
		{
			RECT *dstrect = (RECT*)pPropData;
			printrectinfo(*dstrect);
			memcpy(&m_rDst, dstrect, sizeof(m_rDst));
			return S_OK;
		}
	case MC_PROPID_VP_RECTDST_INTERNAL:
		{
			return S_FALSE;
		}
	case MC_PROPID_VP_RECTSRC:
		{
			RECT *pr = reinterpret_cast<RECT *>(pPropData);
			return S_FALSE;
		}
	case MC_PROPID_VP_ENABLE:
		{
			LONG lEnable = *(LONG *)pPropData;
			return S_FALSE;
		}
	case MC_PROPID_VP_BRIGHTNESS:
	case MC_PROPID_VP_CONTRAST:
	case MC_PROPID_VP_HUE:
	case MC_PROPID_VP_SATURATION:
	case MC_PROPID_VP_GAMMA:
	case MC_PROPID_VP_COLORENABLE:
	case MC_PROPID_VP_SHARPNESS:
		{
			return S_FALSE;
		}

	case MC_PROPID_VP_RESAMPLE:
		{
			LONG lResampleMode = *(LONG *)pPropData;
			return S_FALSE;
		}
	case MC_PROPID_VP_INTERPOLATION:
		{
			LONG lInterpolationMode = *(LONG *)pPropData;

			m_lInterpolationMode = lInterpolationMode;
			return S_OK;
		}
	case MC_PROPID_VP_DEBLOCK:
		{
			LONG lDeblockMode = *(LONG *)pPropData;

			m_lDeblockMode = lDeblockMode;
			return S_OK;
		}
	case MC_PROPID_VP_COLORKEYDST:
		{
			DWORD dwCkeyDst = *(DWORD *)pPropData;
			return E_FAIL;
		}
	case MC_PROPID_VP_COLORKEYSRC:
		{
			DWORD dwCkeySrc = *(DWORD *)pPropData;
			return E_FAIL;
		}
	case MC_PROPID_VP_WND:
		{
			return E_FAIL;
		}
	case MC_PROPID_VP_ALPHA:
		{
			LONG lAlpha = *(LONG *)pPropData;
			return E_FAIL;
		}
	}
	return E_PROP_ID_UNSUPPORTED;
}

STDMETHODIMP CVRPresentSDL::GetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, 
		DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
{
	return S_OK;
}

STDMETHODIMP CVRPresentSDL::SetSurface(VR_SURFACE_TYPE type, REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, 
		DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
{
	return S_OK;
}

HRESULT CVRPresentSDL::SetSrcSize(DWORD dwWidth, DWORD dwHeight)
{
	RECT r;
		
	SetRect(&r, 0, 0, dwWidth, dwHeight);
	if(EqualRect(&r,&m_rSrcSize))
		return S_OK;
	m_rSrcSize = r;
	
	return S_OK;
}

HRESULT CVRPresentSDL::Restore()
{
	if(!m_bOpen)
		return E_FAIL;
			
	return S_OK;
}

HRESULT CVRPresentSDL::UpdateDisplayParameters()
{
	HRESULT hr = S_OK;
		return hr;
}

STDMETHODIMP CVRPresentSDL::PrepareBuffer(VR_SURFACE_TYPE type, LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;
			
	return S_OK;
}

STDMETHODIMP CVRPresentSDL::PresentBuffer(VR_SURFACE_TYPE type, LONG lNum, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;
			
	return S_OK;
}

STDMETHODIMP CVRPresentSDL::Present(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	return Present(VP_SURF_LAYERTOP,ppbPlanes, pdwStrides,  dwFlags);
}

STDMETHODIMP CVRPresentSDL::PrepareBuffer(LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	return E_FAIL;
}

STDMETHODIMP CVRPresentSDL::PresentBuffer(LONG lNum, DWORD dwFlags)
{
	return E_FAIL;
}

STDMETHODIMP CVRPresentSDL::Present(VR_SURFACE_TYPE type, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;

	unsigned char *spy, *dpy;
	unsigned char *spu, *dpu;
	unsigned char *spv, *dpv;

	//printf("pdwStrides[0] %d, pdwStrides[1] %d, pdwStrides[2] %d, pdwStrides[3] %d\n", pdwStrides[0], pdwStrides[1], pdwStrides[2], pdwStrides[3]);

	if(SDL_MUSTLOCK(m_sdlsurface)) 
	{
		if(SDL_LockSurface(m_sdlsurface) < 0) 
		{
			printf("SDL_LockSurface error\n");
			return E_FAIL;
		}
	}
	SDL_LockYUVOverlay(m_sdlyuvoverlay);

	//printf("m_sdlyuvoverlay->format %d, m_sdlyuvoverlay->w %d , m_sdlyuvoverlay->h %d, m_sdlyuvoverlay->pitches %x, m_sdlyuvoverlay->pixels %x\n", m_sdlyuvoverlay->format, m_sdlyuvoverlay->w, m_sdlyuvoverlay->h, m_sdlyuvoverlay->pitches, m_sdlyuvoverlay->pixels);

	spy = ppbPlanes[0];
	dpy = m_sdlyuvoverlay->pixels[0];
	spu = ppbPlanes[1];
	dpu = m_sdlyuvoverlay->pixels[1];
	spv = ppbPlanes[2];
	dpv = m_sdlyuvoverlay->pixels[2];

	//printf("ppbPlanes[0] %a, ppbPlanes[1] %x, ppbPlanes[2] %x\n", ppbPlanes[0], ppbPlanes[1], ppbPlanes[2]);
	
	int i;
	for(i=0; i<m_OpenOptions.dwHeight; ++i)
	{
		memcpy(dpy, spy, m_OpenOptions.dwWidth);
		spy += pdwStrides[0];
		dpy += m_OpenOptions.dwWidth;

		if(i%2)
		{
			memcpy(dpu, spu, m_OpenOptions.dwWidth/2);
			spu += pdwStrides[1];
			dpu += m_OpenOptions.dwWidth/2;
			memcpy(dpv, spv, m_OpenOptions.dwWidth/2);
			spv += pdwStrides[2];
			dpv += m_OpenOptions.dwWidth/2;
		}
	}

	//printf("m_sdlyuvoverlay->pitches[0] %d, m_sdlyuvoverlay->pitches[1] %d, m_sdlyuvoverlay->pitches[2] %d\n", m_sdlyuvoverlay->pitches[0], m_sdlyuvoverlay->pitches[1], m_sdlyuvoverlay->pitches[2]);
      
	//384, 176

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = m_OpenOptions.dwWidth;
	rect.h = m_OpenOptions.dwHeight;
	SDL_UnlockYUVOverlay(m_sdlyuvoverlay);
	SDL_DisplayYUVOverlay(m_sdlyuvoverlay, &rect);
		
	if(SDL_MUSTLOCK(m_sdlsurface)) 
	{
		SDL_UnlockSurface(m_sdlsurface);
	}

	//SDL_Flip(m_sdlsurface); 
	
	return S_OK;
}

STDMETHODIMP CVRPresentSDL::Present_Internal(LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	HRESULT hr = E_FAIL;
		
	if(!m_bOpen)
		return E_FAIL;
				
	return hr;
}

HRESULT CVRPresentSDL::ConvertPrecision(LONG& lSrcVal, const LONG lDstMax, const LONG lDstMin, const LONG lSrcMax,const LONG lSrcMin)
{
	LONG lSrcNum = lSrcMax - lSrcMin;
	LONG lDstNum = lDstMax - lDstMin;
	
	if(lSrcVal>lSrcMax)
		lSrcVal = lDstMax;
	else if(lSrcVal < lSrcMin)
		lSrcVal = lDstMin;
	else		
		lSrcVal = lDstMin + (lSrcVal-lSrcMin)*lDstNum/lSrcNum;
	return S_OK;
}


STDMETHODIMP CVRPresentSDL::PrepareBuffer_Internal(VR_SURFACE_TYPE type, LONG lNum, LPBYTE *ppbPlanes, DWORD *pdwStrides, DWORD dwFlags)
{
	return E_FAIL;
}

STDMETHODIMP CVRPresentSDL::PresentBuffer_Internal(VR_SURFACE_TYPE type, LONG lNum, DWORD dwFlags)
{
	if(!m_bOpen)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CVRPresentSDL::SetVBI(VP_VIDEOBUFFER_INFO *pVBI)
{
	if(!m_bOpen)
		return E_FAIL;
			
	HRESULT hr;
	
	memcpy(&m_VBI, pVBI, sizeof(m_VBI));
	
	
	m_OpenOptions.dwWidth = m_VBI.dwWidth[0];
	m_OpenOptions.dwHeight = m_VBI.dwHeight[0];
	m_OpenOptions.dwAspectX = m_VBI.dwAspectX;
	m_OpenOptions.dwAspectY = m_VBI.dwAspectY;
	
	return Restore();
}

STDMETHODIMP CVRPresentSDL::SetFPS(DWORD dwFPS)
{
	m_FPS = dwFPS;
	return S_OK;
}

STDMETHODIMP CVRPresentSDL::GetBackBuffers(DWORD *pdwBackBuffers)
{
	if(!m_bOpen)
		return E_FAIL;
			
	return S_OK;
}

STDMETHODIMP CVRPresentSDL::GetStatus(DWORD *pdwStatus)
{
	if(pdwStatus==0)
		return E_POINTER;
	*pdwStatus = m_dwStatus;
	return S_OK;
}

STDMETHODIMP CVRPresentSDL::GetCaps(VP_CAPS *pCaps)
{
	if(!m_bOpen)
		return E_FAIL;
			
	return S_OK;
}

IMcVideoPresenter *CreateVideoPresenter()
{
	return new CVRPresentSDL();
}
