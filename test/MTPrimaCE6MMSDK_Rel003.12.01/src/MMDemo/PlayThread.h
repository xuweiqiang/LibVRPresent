/***************************************************************************
 *                                                                         *
 *                   SiRF Technology, Inc. Platform Software               *
 *                                                                         *
 *    Copyright (c) 2008 by SiRF Technology, Inc.  All rights reserved.    *
 *                                                                         *
 *    This Software is protected by United States copyright laws and       *
 *    international treaties.  You may not reverse engineer, decompile     *
 *    or disassemble this Software.                                        *
 *                                                                         *
 *    WARNING:                                                             *
 *    This Software contains SiRF Technology, Inc.'s confidential and      *
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
 *    Software without SiRF Technology, Inc.'s  express written            *
 *    permission.   Use of any portion of the contents of this Software    *
 *    is subject to and restricted by your written agreement with          *
 *    SiRF Technology, Inc.                                                *
 *                                                                         *
 ***************************************************************************/




// PlayThread.h: interface for the PlayThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYTHREAD_H__899ED3E8_6826_4A25_AB9A_B3D7D5E965E6__INCLUDED_)
#define AFX_PLAYTHREAD_H__899ED3E8_6826_4A25_AB9A_B3D7D5E965E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "mediafile.h"
#include "PerMon.h"
#include "inc/mm_codec_common.h"
#include "inc/RendererSDK.h"
#define BSM_HOST
#include "inc/BHSurfMgr.h"




#define  WM_USER_PLAY_ERROR		(WM_USER+100)
#define  WM_USER_PLAY_FINISHED	(WM_USER+101)

//decoder functions
typedef void* dec_handle;

typedef dec_handle (*dec_open_fxn)(UINT32 width, UINT32 height, UINT32 flag);

typedef INT32 (*dec_process_fxn)(dec_handle h, VIDEO_DEC_CONTEXT *context_ptr);

typedef void (*dec_close_fxn)(dec_handle h);

typedef INT32 (*dec_surface_lock_fxn)(dec_handle h,VIDEO_DEC_CONTEXT *context_ptr, UINT8 *ptr[3], UINT32 stride[3]);

typedef INT32 (*dec_surface_unlock_fxn)(dec_handle h);


typedef INT32 (*dec_header_fxn)(dec_handle h, VIDEO_DEC_CONTEXT *context_ptr, void *info_ptr);



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


typedef SX_UINT32 (*BSM_Init_fxn)(VR_HANDLE handle);
typedef void (*BSM_DeInit_fxn)();
typedef void (*BSM_OutputSurf_fxn)(SXMemSurfDesc* surf, SX_REN_SYNC_MODE WaitMode);
typedef VR_FRAME_SURFACE* (*CvtToRendererSurf_fxn)(SXMemSurfDesc* surf);

typedef SX_UINT32 (*BSM_LockSurf_fxn)(SXMemSurfDesc* surf);
typedef SX_UINT32 (*BSM_UnlockSurf_fxn)(SXMemSurfDesc* surf);







/*
typedef VR_RESULT	(*CVR_SetOSDRegionNum_fxn)(VR_HANDLE handle,  int num);

//Set all OSD related information into it.
SXDLL VR_RESULT CVR_SetOSDInfo(
						 VR_HANDLE handle,		
						 int region_num,	
						 PVR_OSD_INFO inf	
						 );


// Return: VR_OK if successful else failed.
// Remark: On Titan, we can control the underlying scaling method being used and the coefficient for the scaler through this function.
SXDLL VR_RESULT CVR_EscapeCtrl(
						 VR_HANDLE handle,				//[in] The handle of video renderer
						 long ctrl_id,					//[in] The Renderer you want to switch. See RENDERER_NAME
						 unsigned char* pInBuf,			//[in] Now just pass NULL. Pointer to a buffer to hold parameter for specified escape code.
						 int dat_siz,					//[in] Now just pass 0. Buffer size.
						 unsigned char* pOutBuf			//[out] Now just pass NULL. Pointer to a output buffer to hold returned data.
						 );

//Get the internal error code for the last operation, which is platform dependent and is provided as a manner to help the malfunction diagnosis. 
// Return: The error code.
// Remark: It will be DirectDraw or Direct3D Mobile error code on WinCE platform.
SXDLL unsigned long CVR_GetLastError(
							   VR_HANDLE handle			//[in] The handle of video renderer
							   );
							
*/




class PlayThread  
{
public:
	PlayThread();
	virtual ~PlayThread();

	void FirePlay();
	
	void Pause();
	void UnPause();
	BOOL IsPaused()
	{
		return WaitForSingleObject(evtUnPause, 0) == WAIT_TIMEOUT;
	}
	void ReportErr(WCHAR* msg);

	void Kill()
	{
		m_bKilled = true;
	}

	WCHAR* GetErrMsg()
	{
		return m_wsErr;
	}
	DWORD		m_dwSreenWidth;
	DWORD		m_dwSreenHeight;
	DWORD		m_dwSurfaceMgrMode;
	VideoFile*	m_pFile;
	HWND		m_hWnd;
	volatile  bool		m_bKilled;
	bool		m_bLoop;
	bool		m_bDeinterlace;
	bool		m_bFullScreen;
	VR_RENDER_LAYER m_rlLayer;
	double		m_fAlpha;
	VR_REN_SYNC_MODE m_smSyncMode;

	bool		m_bRender;
	VR_DISP_MODE_DESC m_displayMode;

	PerMon*      m_pPerMon;



private:
	HANDLE m_hThread;
	VR_HANDLE m_hRenderer;
	VR_FRAME_SURFACE* m_frames[2];					//pingpong buffer.
	int				  m_idxFrame;					//frame index;

	CRITICAL_SECTION m_csLck;

	HANDLE evtUnPause;							//for pause

	

	HMODULE			m_hDll;							//decoder dll
	dec_handle		m_dec;							//decoder handle
	dec_open_fxn	m_dec_open;						//decoder open;
	dec_close_fxn	m_dec_close;					//decoder close;
	dec_process_fxn m_dec_process;					//decoder process;
	dec_surface_lock_fxn	m_dec_surface_lock;		//lock surface 
	dec_surface_unlock_fxn	m_dec_surface_unlock;	//unlock surface
	dec_header_fxn			m_dec_header;			//decoder header.


	HMODULE			m_hRendererDll;				   //renderer dll
	HMODULE			m_hSurfMgrDll;					//surf mgr dll


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
	DECLEAR_RENDERER_FXN(BSM_Init)
	DECLEAR_RENDERER_FXN(BSM_DeInit)
	DECLEAR_RENDERER_FXN(BSM_OutputSurf)
	DECLEAR_RENDERER_FXN(CvtToRendererSurf)
	DECLEAR_RENDERER_FXN(BSM_LockSurf)
	DECLEAR_RENDERER_FXN(BSM_UnlockSurf)


	




	WCHAR			m_wsErr[256];


	bool LoadDecoderDll(DWORD nCodecType);
	void UnloadDecoderDll();

	bool LoadRendererDll();
	void UnloadRendererDll();
	


	void Play();
	void WaitUnPause();
	void ReportFinished();
	static DWORD Run(LPVOID pContext);
	void FreeResouce();
	


};

#endif // !defined(AFX_PLAYTHREAD_H__899ED3E8_6826_4A25_AB9A_B3D7D5E965E6__INCLUDED_)
