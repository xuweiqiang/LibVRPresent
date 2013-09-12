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
 
#ifndef _CODEC_I420_H_
#define _CODEC_I420_H_

#include "inc\BHSurfMgr.h"
#include "inc\mm_codec_common.h"
#include <assert.h>
#include <stdio.h>

typedef SX_UINT32 (*fnBHSurf)(SXMemSurfDesc* );

#define LOADPROC(_LIBINST_ ,  _API_ , _FN_ ) _API_->_FN_ = (fnBHSurf) GetProcAddress(_LIBINST_, TEXT(#_FN_) )


typedef void*  i420_dec_handle;

struct i420_dec_internal
{
	UINT32 m_width;
	UINT32 m_height;
	HMODULE m_hBHSurfMgr;
	DSMgrSurfFunctors*  m_pDSMgrSurfAPI;
	int  m_cur;								//current frame;
	vector<void*>   m_Frames;
	vector<SXMemSurfDesc>  m_Surfs;			//surface;

	i420_dec_internal():m_width(0), m_height(0), m_hBHSurfMgr(0), m_pDSMgrSurfAPI(0), m_cur(0)
	{}
	enum {REF_FRAME_NUM = 7};
	

};



i420_dec_handle  I420_dec_open(UINT32 width, UINT32 height, UINT32 flag)
{
	i420_dec_internal* p = new i420_dec_internal;
	if (!p)
		return false;
	
	p->m_width = width;
	p->m_height = height;

	if (flag == SURFACE_MGR_DRIVER)
		return p;
	
	
	TCHAR pDllName[40] = TEXT("BHSurfMgr.dll");//TEXT("BHSurfMgr.dll");
	
	p->m_hBHSurfMgr = LoadLibrary(pDllName);
	if (!p->m_hBHSurfMgr)
	{
		free(p);
		return NULL;
	}
	
	DSMgrSurfFunctors *pDSMgrSurfAPI;
		
	pDSMgrSurfAPI = (DSMgrSurfFunctors*)malloc(sizeof(DSMgrSurfFunctors));
	memset(pDSMgrSurfAPI, 0, sizeof(DSMgrSurfFunctors));
	p->m_pDSMgrSurfAPI = pDSMgrSurfAPI;
		
	LOADPROC(p->m_hBHSurfMgr, pDSMgrSurfAPI, BSM_AllocSurf);
	LOADPROC(p->m_hBHSurfMgr, pDSMgrSurfAPI, BSM_ReleaseSurf);
	LOADPROC(p->m_hBHSurfMgr, pDSMgrSurfAPI, BSM_LockSurf);
	LOADPROC(p->m_hBHSurfMgr, pDSMgrSurfAPI, BSM_UnlockSurf);
	LOADPROC(p->m_hBHSurfMgr, pDSMgrSurfAPI, BSM_OutputSurf);
	return p;

}

INT32 I420_dec_process(i420_dec_handle h, VIDEO_DEC_CONTEXT *context_ptr)
{
	i420_dec_internal* p = (i420_dec_internal*)h;


	assert(context_ptr->buf_len == ROOF(p->m_width, 16)*ROOF(p->m_height, 16)*3/2);
	//assert(!(p->m_width % 16) && !(p->m_height%16));
	context_ptr->height = p->m_height;
	context_ptr->width = p->m_width;
	
	//RETAILMSG(1, (L"width = %d, height = %d\r\n", 	context_ptr->height, context_ptr->width));

	if (!p->m_Frames.size())
	{
		for (int i = 0; i < i420_dec_internal::REF_FRAME_NUM; i++)
		{
			if (p->m_pDSMgrSurfAPI)
			{
				SX_UINT32 ReturnCode;
				SXMemSurfDesc surf;
				ReturnCode = p->m_pDSMgrSurfAPI->BSM_AllocSurf(&surf);
				if (ReturnCode == BSM_ERR_NOERROR)
				{
							
					ReturnCode = p->m_pDSMgrSurfAPI->BSM_LockSurf(&surf);
					if (ReturnCode == BSM_ERR_NOERROR)
					{
						
						p->m_Frames.push_back(surf.pFrmBuf);
						p->m_pDSMgrSurfAPI->BSM_UnlockSurf(&surf);
					}
				}
				if (ReturnCode != BSM_ERR_NOERROR)
				{
					return CODEC_FAT_MEMORY;
				}
				p->m_Surfs.push_back(surf);
			}
			else
			{
				void* pFrame = malloc(context_ptr->buf_len);
				if (!pFrame)
					return CODEC_FAT_MEMORY;
				p->m_Frames.push_back(pFrame);
				
			}


		}
	}
	p->m_cur++;
	p->m_cur %= p->m_Frames.size();
	memcpy(p->m_Frames[p->m_cur], context_ptr->buf_ptr, context_ptr->buf_len);
	if (p->m_pDSMgrSurfAPI)
	{
		context_ptr->pSurfHdl = (void*)&p->m_Surfs[p->m_cur];
	}
	return CODEC_STATE_OK;
	
}

void I420_dec_close(i420_dec_handle h)
{
	i420_dec_internal* p = (i420_dec_internal*)h;
	if (p->m_pDSMgrSurfAPI)
	{
		for (int i = 0; i < p->m_Surfs.size(); i++)
		{
			p->m_pDSMgrSurfAPI->BSM_ReleaseSurf(&p->m_Surfs[i]);
		}
		p->m_Surfs.clear();
		free(p->m_pDSMgrSurfAPI);
		p->m_pDSMgrSurfAPI = NULL;	

	}
	else
	{
		for (int i = 0; i < p->m_Frames.size(); i++)
		{
			free(p->m_Frames[i]);
		}
	}
	if (p->m_hBHSurfMgr)
		FreeLibrary(p->m_hBHSurfMgr);
	p->m_Frames.clear();
	delete p;


	
}


INT32 I420_dec_surface_lock(i420_dec_handle h,VIDEO_DEC_CONTEXT *context_ptr, UINT8 *ptr[3], UINT32 stride[3])
{
	i420_dec_internal* p = (i420_dec_internal*)h;
	if (!p->m_Frames.size())
		return CODEC_FAT_MEMORY;
	context_ptr->width = p->m_width;
	context_ptr->height = p->m_height;
	ptr[0] = (UINT8*)p->m_Frames[p->m_cur];
	ptr[1] = ptr[0] + p->m_width*p->m_height;
	ptr[2] = ptr[1] + p->m_width*p->m_height/4;
	stride[0] = p->m_width;
	stride[1] = p->m_width/2;
	stride[2] = p->m_width/2;
//	RETAILMSG(1, (L"0x%x, 0x%x, 0x%x, stride = %d, %d, %d\r\n", ptr[0], ptr[1], ptr[2], stride[0], stride[1], stride[2]));
	return CODEC_STATE_OK;
}

//
//		Name:		h264_dec_surface_unlock
//		Purpose: 	Unlock surface.
//		parameters:	handle of the encoder.
//		Returns:	none.
//		Notes:
//
INT32 I420_dec_surface_unlock(i420_dec_handle h)
{
	return CODEC_STATE_OK;

}



#endif