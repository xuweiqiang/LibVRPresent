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


/******************************************************************************/
/*                                                                            */
/* FILE: wmv_dec_api.h                                                        */
/*                                                                            */
/* DESCRIPTION: API interface										          */
/*                                                                            */
/* HISTORY:                                                                   */
/* lshu 17/03/2008 Created                                                    */
/*                                                                            */
/******************************************************************************/
#ifndef WMV_DEC_API_H
#define WMV_DEC_API_H

#include "mm_codec_common.h"

#ifdef __cplusplus
extern "C" {
#endif

//
//	decoder API
//
typedef void *  wmv_dec_handle;
#ifdef linux
/* Bit values for setting the DEBUG PERFORMANCE parameters */
#define DEBUG_PERFORMANCE          0x1
#define DEBUG_PRINT_PER_SEC        0x2
#define DEBUG_MAX_FRAMES_PER_SEC   0x4
#endif
//
//		Name:		wmv_dec_open	
//		Purpose: 		Create decoder handler.
//		parameters:	none.
//		Returns:		handle of the decoder. If failed, returns NULL
//		Notes:
//
DECL_CODEC_API(wmv_dec_handle, wmv_dec_open)(UINT32 width, UINT32 height, UINT32 flag);
//
//		Name:		wmv_dec_get_caps	
//		Purpose: 		get capability of the decoder.
//		parameters:	VIDEO_CAPS structure ptr.
//		Returns:		CODEC_STATE_OK. If failed, returns CODEC_FATEL_ERROR
//		Notes:
//
DECL_CODEC_API(INT32, wmv_dec_get_caps)(VIDEO_CAPS *caps_ptr);
//
//		Name:		wmv_dec_set_params	
//		Purpose: 		set parameters of the decoder.
//		parameters:	param string.
//		Returns:		CODEC_STATE_OK. If failed, returns CODEC_FATEL_ERROR
//		Notes:
//
DECL_CODEC_API(INT32, wmv_dec_set_params)(wmv_dec_handle h, const UINT8 *params, UINT32 size);
//
//		Name:		wmv_dec_process
//		Purpose: 		decode one frame.
//		parameters:	context structure.
//		Returns:		CODEC_STATE_OK. If failed, the return codes depends on the syntax.
//		Notes:
//
DECL_CODEC_API(INT32, wmv_dec_process)(wmv_dec_handle h, VIDEO_DEC_CONTEXT *context_ptr);

#ifdef linux
//
//              Name:           wmv_dec_get_surface_physical_addr_surface_lock
//              Purpose:        Return physical address of surface and locks it
//              parameters:     handle of the encoder.
//              Returns:        none.
//              Notes:
//
DECL_CODEC_API(INT32, wmv_dec_get_surface_physical_addr_surface_lock)(wmv_dec_handle h, VIDEO_DEC_CONTEXT *context_ptr, void **psurface, UINT32 *PhyAddr[3], UINT8 *ptr[3], UINT32 stride[3]);

DECL_CODEC_API(INT32, wmv_dec_buffer_surface_unlock)(void *surface);
#endif
//
//		Name:		wmv_dec_surface_lock
//		Purpose: 	Lock surface to get frame buffer.
//		parameters:	handle of the encoder.
//		Returns:	CODEC_STATE_OK.
//		Notes:
//
DECL_CODEC_API(INT32, wmv_dec_surface_lock)(wmv_dec_handle h,VIDEO_DEC_CONTEXT *context_ptr, UINT8 *ptr[3], UINT32 stride[3]);

//
//		Name:		wmv_dec_surface_unlock
//		Purpose: 	unLock surface.
//		parameters:	handle of the encoder.
//		Returns:	CODEC_STATE_OK ,If failed, returns CODEC_FATEL_ERROR.
//		Notes:
//
DECL_CODEC_API(INT32, wmv_dec_surface_unlock)(wmv_dec_handle h);

//
//		Name:		wmv_dec_close
//		Purpose: 	Destroy decoder handler.
//		parameters:	handle of the decoder.
//		Returns:		none.
//		Notes:
//
DECL_CODEC_API(void, wmv_dec_close)(wmv_dec_handle h);

#ifdef linux
//
//		Name:		wmv_debug_performance
//		Purpose: 	Debug decoder performance
//		parameters:	debug true/false
//		Returns:	none.
//		Notes:
//
DECL_CODEC_API(void, wmv_debug_performance)(INT32 val);

//
//		Name:		wmv_debug_performance_eos
//		Purpose: 	Debug decoder performance
//		parameters:	debug true/false
//		Returns:	none.
//		Notes:
//
DECL_CODEC_API(void, wmv_debug_performance_eos)(INT32 val);
#endif
#ifdef __cplusplus
}
#endif

#endif	// WMV_DEC_API_H
