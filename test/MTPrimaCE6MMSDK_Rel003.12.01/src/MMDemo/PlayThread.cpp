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




// PlayThread.cpp: implementation of the PlayThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlayThread.h"
#include "CodecI420.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



PlayThread::PlayThread()
:m_dwSreenWidth(0),
m_dwSreenHeight(0),
m_dwSurfaceMgrMode(0),
m_pFile(0),
m_hWnd(0),
m_bKilled(false),
m_bLoop(false),
m_hThread(NULL),
m_hRenderer(NULL),
m_idxFrame(0),
m_hDll(NULL),
m_hRendererDll(NULL),
m_dec(NULL),							//decoder handle
m_dec_open(NULL),						//decoder open;
m_dec_close(NULL),					//decoder close;
m_dec_process(NULL),					//decoder process;
m_dec_surface_lock(NULL),		//lock surface 
m_dec_surface_unlock(NULL),	//unlock surface
m_dec_header(NULL),
m_pPerMon(0),
m_bRender(true)
{

	InitializeCriticalSection(&m_csLck);
	memset(m_wsErr, 0, sizeof(m_wsErr));
	memset(&m_displayMode, 0, sizeof(m_displayMode));
	memset(m_frames, 0, sizeof(m_frames));
	evtUnPause = CreateEvent(NULL,TRUE, TRUE, NULL);

}

PlayThread::~PlayThread()
{
	m_bKilled = true;
	UnPause();
	if (m_hThread)
		WaitForSingleObject(m_hThread, INFINITE);
	FreeResouce();
	UnloadDecoderDll();
	UnloadRendererDll();
	CloseHandle(evtUnPause);
	delete m_pFile;
	delete m_pPerMon;

	DeleteCriticalSection(&m_csLck);

}


void PlayThread::FirePlay()
{
	m_hThread = CreateThread(NULL, 0, Run, this, 0, NULL);

}

DWORD PlayThread::Run(LPVOID pContext)
{
	PlayThread* pPlayer = (PlayThread*)pContext;
	pPlayer->Play();
	return 0;

}

#ifndef ROOF
#define ROOF(v, align)  (((v-1)|((align)-1))+1)
#endif 


void PlayThread::Play()
{

	
	if (!LoadDecoderDll(m_pFile->Codec()))
	{
		ReportErr(L"Can't Load Decoder Dll");
		return;
	}

	if (!LoadRendererDll())
	{
		ReportErr(L"Can't Load Decoder Dll");
		return;
	}
	

	m_dec = m_dec_open(m_pFile->Width(), m_pFile->Height(), m_dwSurfaceMgrMode);

	if (!m_dec)
	{
		ReportErr(L"Can't Load Open decoder");
		return;
	}


	//get src width and height for CVR_SetVideoFormat
	UINT32 dwSrcWidth; 
	UINT32 dwSrcHeight;
	if (m_pFile->IsRawStream())						//??why i can't use dynamic_case ????
	{
		VideoFileRaw* pRaw = static_cast<VideoFileRaw*>(m_pFile);

		char* pFrame;
		int bKey;
		int siz;
		while ((siz = pRaw->PeekFrame(&pFrame, &bKey)) > 0)
		{
			VIDEO_DEC_CONTEXT ctx;
			memset(&ctx, 0, sizeof(ctx));
			INT32 iRet;
			ctx.buf_ptr = (UINT8*)pFrame;
			ctx.buf_len = siz;			
			if ((iRet = m_dec_header(m_dec, &ctx, NULL)) == CODEC_STATE_OK)
			{
				dwSrcWidth = ctx.width;
				dwSrcHeight = ctx.height;
				break;
			}
			else if (CODEC_STAT_FAT_ERROR(iRet))
			{
				WCHAR buf[256];
				wsprintf(buf, L"codec return a fatal error, iRet = %d", iRet);
				ReportErr(buf);
				return;
			}
			pRaw->Skip(siz);
		}
		if (siz <= 0)
		{
			ReportErr(L"Invalid File");
			return;
		}
	}
	else
	{
		//avi file
		dwSrcWidth = m_pFile->Width();
		dwSrcHeight = m_pFile->Height(); 
	}

	if (!dwSrcHeight || !dwSrcWidth)
	{
		ReportErr(L"Invalid File");
		return;
	}


	do 
	{

		//create the renderer
		
		if (m_bRender)
		{
			VR_RESULT r = m_CVR_CreateVideoRender(&m_hRenderer, /*m_hWnd*/ NULL);
			if (r != VR_OK)
			{
				ReportErr(L"Can't Create Renderer");	
				return;
			}
			
		
			
			//if (m_dwSurfaceMgrMode == SURFACE_MGR_RENDER )
			//{
			//	if (m_BSM_Init(m_hRenderer) != BSM_ERR_NOERROR)
			//	{
			//		ReportErr(L"Can't int surface manager");
			//		return;
			//	}
			//}

			if (m_CVR_SetRenderLayer(m_hRenderer, m_rlLayer,1) !=  VR_OK)
			{

				ReportErr(L"CVR_SetRenderLayer Failed");
				return;
			}
			VR_RESULT ret = m_CVR_SetDisplayMode(m_hRenderer, m_displayMode);			
			if (ret != VR_OK)
			{
				
				WCHAR buf[256];
				wsprintf(buf, L"Set Display Mode Failed, ret = 0x%x", ret);
				RETAILMSG(1, (L"%s\r\n", buf));
				ReportErr(buf);
				return;
			}
			
			//macro block align
			int frame_width, frame_height;
			frame_width = ROOF(dwSrcWidth, 16);
			frame_height = ROOF(dwSrcHeight, 16);
			
			int luma_stride, chroma_stride;
			luma_stride = frame_width;						
			chroma_stride = frame_width >> 1;
			
			VR_RECT rect;
			rect.left = rect.top = 0;
			rect.right = dwSrcWidth;
			rect.bottom = dwSrcHeight;
			
			
			if (m_CVR_SetVideoFormat(m_hRenderer, VR_FORMAT_I420, frame_width, frame_height,	//macro block align
				&rect, &luma_stride, &chroma_stride) != VR_OK)
			{
				ReportErr(L"Set Video Format Failed");
				return;
				
			}
			if (m_bDeinterlace)
			{
				m_CVR_SetDeinterlaceMode(m_hRenderer, VR_DEINTERLACE_FIELD_EVEN);
			}

			

			
			if (m_dwSurfaceMgrMode != SURFACE_MGR_RENDER )
			{
				for (int i = 0; i < DIM(m_frames); i++)
				{
					m_CVR_CreateSurface(m_hRenderer,&m_frames[i],1);
					m_CVR_LockSurface(m_hRenderer, m_frames[i]);
					memset(m_frames[i]->pFrameBuf,0, frame_width*frame_height * 3 / 2);
					m_CVR_UnlockSurface(m_hRenderer, m_frames[i]);

				}
				
			}
			//full screen
			VR_DSTRECT rcDest = {0, 0, 1.0, 1.0};
			
			if (!m_bFullScreen)
			{
				rcDest.right = ((float)dwSrcWidth)/m_dwSreenWidth;
				rcDest.bottom = ((float)dwSrcHeight)/m_dwSreenHeight;
				
				//normalization it 
				if (rcDest.right > 1)
				{
					rcDest.bottom /= rcDest.right;
					rcDest.right = 1;
				}
				if (rcDest.bottom > 1)
				{
					rcDest.right /= rcDest.bottom;
					rcDest.bottom = 1;
				}
				
			}
			m_CVR_SetDstRect(m_hRenderer, rcDest);
			
			

		}
		

		
	
	
	

		

		m_pPerMon->Clear();

		//decode-render loop
		char* pFrame;
		int bKey; 
		int nFrame = 0;
		int siz;
		bool bLastFrame = false;
		while (!m_bKilled)
		{
			
			m_pPerMon->StartReadFile();
			siz = m_pFile->ReadFrame(&pFrame, &bKey);
			m_pPerMon->EndReadFile();

			


			if (siz < 0)					//reach end or read error
			{
				bLastFrame = true;
				pFrame = NULL;
				siz = 0;
			}

			


			while ( siz>0 || bLastFrame )
			{
				INT32 iRet;
				VIDEO_DEC_CONTEXT context_ptr;
				memset(&context_ptr, 0, sizeof(context_ptr));
				context_ptr.size = sizeof(context_ptr);
				context_ptr.buf_ptr = (UINT8*)pFrame;
				context_ptr.buf_len = siz;
				
				m_pPerMon->StartDecode();
				iRet = m_dec_process(m_dec, &context_ptr);
				m_pPerMon->EndDecode();
				if (bLastFrame)
					iRet = CODEC_STATE_OK;
				
	
				

						

				//avi will pack some B and P Frame together, 
				pFrame	+= context_ptr.buf_len;
				siz		-= context_ptr.buf_len;

				if (CODEC_STATE_OK == iRet)
				{
					if (nFrame)
						WaitUnPause();							//we not wait here, if it's first frame,
					if (m_bKilled)
						break;

					//ok, render it.
					m_pPerMon->StartRender();
					if (m_bRender)
					{

						
						if(m_dwSurfaceMgrMode == SURFACE_MGR_RENDER )
						{
					
							if (context_ptr.pSurfHdl)
							{
								//ZY::Added by me
								//m_CVR_WaitComplete(m_hRenderer);
								m_BSM_OutputSurf((SXMemSurfDesc*)context_ptr.pSurfHdl,(SX_REN_SYNC_MODE)m_smSyncMode);
								
							}
						}
						else
						{
							UINT8 *ptr[3];
							UINT32 stride[3];
							ASSERT(m_dwSurfaceMgrMode == SURFACE_MGR_DRIVER);
							if (m_dec_surface_lock(m_dec,&context_ptr,ptr, stride) ==  CODEC_STATE_OK)
							{
								context_ptr.width = ROOF(context_ptr.width, 16);		//macro block align				
								context_ptr.height = ROOF(context_ptr.height, 16);

								int idx;
								idx = m_idxFrame;

								if (m_CVR_LockSurface(m_hRenderer, m_frames[idx]) != VR_OK)
								{
									ReportErr(L"CVR_LockSurface Failed");
									return;
								}
								memcpy(m_frames[idx]->pFrameBuf,ptr[0], context_ptr.height*context_ptr.width);
								memcpy(m_frames[idx]->pFrameBuf + context_ptr.height*context_ptr.width ,ptr[1], context_ptr.height*context_ptr.width>>2);
								memcpy(m_frames[idx]->pFrameBuf + context_ptr.height*context_ptr.width + (context_ptr.height*context_ptr.width>>2) ,ptr[2], context_ptr.height*context_ptr.width>>2);
								m_CVR_UnlockSurface(m_hRenderer, m_frames[idx]);
								m_dec_surface_unlock(m_dec);
								//ZY::Added by me
								m_CVR_WaitComplete(m_hRenderer);
								m_CVR_Render(m_hRenderer, m_frames[idx] ,m_smSyncMode);

								m_idxFrame =  !m_idxFrame;			//pingpong buffer.
							}
							
						
						}
					}
					m_pPerMon->EndRender();
					nFrame++;
					if (!(nFrame%100))
					{
						m_pPerMon->PrintStatInfo();
						m_pPerMon->FlushToFile();
					}
					
				}
				else if (CODEC_STAT_FAT_ERROR(iRet))
				{
					WCHAR buf[256];
					wsprintf(buf, L"codec return a fatal error, iRet = %d", iRet);
					RETAILMSG(1, (L"%s\r\n", buf));
					ReportErr(buf);
					return;
				}

				if (bLastFrame)
					break;

			}
			
			if (bLastFrame)
				break;
			

		}

	


		m_pPerMon->FlushToFile();
		m_pPerMon->PrintStatInfo();
		m_pPerMon->LogStatInfo();

		if (!m_bKilled)
		{
		
			if (m_bLoop)
			{
				FreeResouce();
				m_pFile->SeekStart();
				m_dec = m_dec_open(m_pFile->Width(), m_pFile->Height(), m_dwSurfaceMgrMode);

				if (!m_dec)
				{
					ReportErr(L"Can't Load Open decoder");
					return;
				}
			}
		}
	} while (!m_bKilled && m_bLoop);
	FreeResouce();
	if (!m_bKilled)
		ReportFinished();

}





struct DecoderEntry
{
	int		nCodecType;
	WCHAR*	pszDllName;
	WCHAR*	pszDecOpen;
	WCHAR*	pszDecProcess;
	WCHAR*	pszDecClose;
	WCHAR*	pszDecSurfaceLock;
	WCHAR*	pszDecSurfaceUnlock;
	WCHAR*  pszDecHeader;

};


const static DecoderEntry DecoderList[] =
{
	{
		CODEC_MPEG2, L"mpeg2decdll.dll", L"mpeg2_dec_open", L"mpeg2_dec_process", L"mpeg2_dec_close", L"mpeg2_dec_surface_lock", L"mpeg2_dec_surface_unlock", L"mpeg2_dec_header"
	},
	{
			CODEC_H264, L"h264decdll.dll", L"h264_dec_open", L"h264_dec_process", L"h264_dec_close", L"h264_dec_surface_lock", L"h264_dec_surface_unlock", L"h264_dec_header"
		},
		{
				CODEC_MPEG4, L"mpeg4decdll.dll", L"mpeg4_dec_open", L"mpeg4_dec_process", L"mpeg4_dec_close", L"mpeg4_dec_surface_lock", L"mpeg4_dec_surface_unlock", L"mpeg4_dec_header"
			},
			{
					CODEC_WMV, L"wmvdecdll.dll", L"wmv_dec_open", L"wmv_dec_process", L"wmv_dec_close", L"wmv_dec_surface_lock", L"wmv_dec_surface_unlock", L"wmv_dec_header"
				}
				
				
};

#ifndef DIM
#define DIM(_array)	(sizeof(_array)/sizeof(_array[0]))
#endif



bool PlayThread::LoadDecoderDll(DWORD nCodecType)
{
	ASSERT(!m_hDll);
	if (nCodecType == CODEC_I420)
	{
		m_dec_open = I420_dec_open;
		m_dec_close = I420_dec_close;
		m_dec_process = I420_dec_process;
		m_dec_surface_lock = I420_dec_surface_lock;
		m_dec_surface_unlock = I420_dec_surface_unlock;
		return true;
	}
	for (int i = 0; i < DIM(DecoderList); i++)
	{
		const DecoderEntry* p = DecoderList + i;
		if (nCodecType == p->nCodecType)
		{
			
			m_hDll =  LoadLibrary(p->pszDllName);
			if (!m_hDll)
			{
				RETAILMSG(1, (L"Can't Load %s, GetLastErr() = 0x%x\r\n", p->pszDllName, GetLastError()));
				return false;
			}
			m_dec_open = (dec_open_fxn)GetProcAddress(m_hDll, p->pszDecOpen);
			m_dec_close = (dec_close_fxn)GetProcAddress(m_hDll, p->pszDecClose);
			m_dec_process = (dec_process_fxn)GetProcAddress(m_hDll, p->pszDecProcess);
			m_dec_surface_lock = (dec_surface_lock_fxn)GetProcAddress(m_hDll, p->pszDecSurfaceLock);
			m_dec_surface_unlock = (dec_surface_unlock_fxn)GetProcAddress(m_hDll, p->pszDecSurfaceUnlock);
			m_dec_header		 = (dec_header_fxn)GetProcAddress(m_hDll, p->pszDecHeader);
			if (m_dec_open && m_dec_close && m_dec_process && m_dec_surface_lock && m_dec_surface_unlock)// && m_dec_header)
			{
				return true;
			}
		}
	}
	return false;
}

void PlayThread::UnloadDecoderDll()
{
	m_dec_open = NULL;
	m_dec_close = NULL;
	m_dec_process = NULL;
	m_dec_surface_lock = NULL;
	m_dec_surface_unlock = NULL;
	m_dec_header = NULL;
	if (m_hDll)
	{
		FreeLibrary(m_hDll);
		m_hDll = NULL;
	}
	
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



bool PlayThread::LoadRendererDll()
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

void PlayThread::UnloadRendererDll()
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

void PlayThread::ReportErr(WCHAR* msg)
{
	wcscpy(m_wsErr, msg);
	//PostMessage(m_hWnd, WM_USER_PLAY_ERROR, 0, 0);
}

void PlayThread::ReportFinished()
{
	//PostMessage(m_hWnd, WM_USER_PLAY_FINISHED, 0, 0);

}


void PlayThread::FreeResouce()
{

	EnterCriticalSection(&m_csLck);				//non-reentrant.
	if (m_dec)
	{
		m_dec_close(m_dec);							//make sure decoder close before renderer deinit
		m_dec = 0;
	}
	
	if (m_dwSurfaceMgrMode == SURFACE_MGR_RENDER )
	{
		if (m_hRenderer)
		{
			m_CVR_WaitComplete(m_hRenderer);
			m_BSM_DeInit();
		}
	}
	else
	{
		ASSERT(m_dwSurfaceMgrMode == SURFACE_MGR_DRIVER);
		bool bWaited = false;
		for (int i = 0; i < DIM(m_frames); i++)
		{	
			if (m_frames[i])
			{

				if (!bWaited)
				{
					m_CVR_WaitComplete(m_hRenderer);
					bWaited = true;
				}

				
				m_CVR_DestroySurface(m_hRenderer,m_frames[i]);
				m_frames[i] = NULL;
			}

		}
		
	}
	if (m_hRenderer)
	{
		m_CVR_DeleteVideoRender(m_hRenderer);
		m_hRenderer = NULL;
	}

	LeaveCriticalSection(&m_csLck);
}



void PlayThread::Pause()
{
	ResetEvent(evtUnPause);
}

void PlayThread::UnPause()
{
	SetEvent(evtUnPause);
}

void PlayThread::WaitUnPause()
{
	WaitForSingleObject(evtUnPause, INFINITE);
}


