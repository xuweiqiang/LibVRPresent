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

#ifndef H264_DEC_API_H
#define H264_DEC_API_H

#include "mm_codec_common.h"

#ifdef __cplusplus
extern "C" {
#endif
#define CODEC_RCV_H264_DEC									(0x2000)
#define CODEC_RCV_H264_DEC_ERROR_MB_SKIP					(CODEC_RCV_H264_DEC + 1)
#define CODEC_RCV_H264_DEC_ERROR_MB_TYPE					(CODEC_RCV_H264_DEC + 2)
#define CODEC_RCV_H264_DEC_ERROR_MB_MODE_UV					(CODEC_RCV_H264_DEC + 3)
#define CODEC_RCV_H264_DEC_ERROR_MB_QP_DELTA				(CODEC_RCV_H264_DEC + 4)
#define CODEC_RCV_H264_DEC_ERROR_MB_CBP_CODE				(CODEC_RCV_H264_DEC + 5)
#define CODEC_RCV_H264_DEC_ERROR_MB_CBP						(CODEC_RCV_H264_DEC + 6)
#define CODEC_RCV_H264_DEC_ERROR_BLK_CAVLC					(CODEC_RCV_H264_DEC + 7)
#define CODEC_RCV_H264_DEC_ERROR_BLK_CHROMDC_CAVLC			(CODEC_RCV_H264_DEC + 8)
#define CODEC_RCV_H264_DEC_ERROR_SUMMB_TYPE					(CODEC_RCV_H264_DEC + 9)
#define CODEC_RCV_H264_DEC_ERROR_REF_IDX					(CODEC_RCV_H264_DEC + 10)


#define CODEC_STATE_H264_MULTIFRAMES			(-11)			//for H264 POC :more than one frame output
#define CODEC_STATE_H264_NOOUT				(-10)			//for H264 POC :no  frame output
//
//	decoder API
//
typedef void *  h264_dec_handle;
#ifdef linux
#define H264DEC_API
#else
#define H264DEC_API __declspec(dllexport)
#endif
//
//		Name:		h264_dec_open
//		Purpose: 		Create decoder handler.
//		parameters:	none.
//		Returns:		handle of the decoder. If failed, returns NULL
//		Notes:
//
h264_dec_handle H264DEC_API h264_dec_open(UINT32 width, UINT32 height, UINT32 flag);

//
//		Name:		h264_dec_get_caps
//		Purpose: 		get capability of the decoder.
//		parameters:	VIDEO_CAPS structure ptr.
//		Returns:		CODEC_STATE_OK. If failed, returns VC_FATEL_INVALID_PARAMS
//		Notes:
//
INT32 H264DEC_API h264_dec_get_caps(h264_dec_handle h,VIDEO_CAPS *caps_ptr);


//
//		Name:		h264_dec_header
//		Purpose: 	Get some information from the headers.
//		parameters:	context structure. 
//		Returns:		CODEC_STATE_OK. If not, means the sps header not found.
//		Notes:
//
INT32 H264DEC_API h264_dec_header(h264_dec_handle h, VIDEO_DEC_CONTEXT *context_ptr, void *info_ptr);

//
//		Name:		h264_dec_process
//		Purpose: 		decode one frame.
//		parameters:	context structure.
//		Returns:		CODEC_STATE_OK. If failed, the return codes depends on the syntax.
//		Notes:
//
INT32 H264DEC_API h264_dec_process(h264_dec_handle h, VIDEO_DEC_CONTEXT *context_ptr);

//
//		Name:		h264_dec_close
//		Purpose: 		Destroy decoder handler.
//		parameters:	handle of the decoder.
//		Returns:		none.
//		Notes:
//
void H264DEC_API h264_dec_close(h264_dec_handle h);

//
//		Name:		h264_dec_surface_lock
//		Purpose: 	Lock surface to get frame buffer.
//		parameters:	handle of the encoder.
//		Returns:	none.
//		Notes:
//
INT32 H264DEC_API h264_dec_surface_lock(h264_dec_handle h,VIDEO_DEC_CONTEXT *context_ptr, UINT8 *ptr[3], UINT32 stride[3]);

//
//		Name:		h264_dec_surface_unlock
//		Purpose: 	Unlock surface.
//		parameters:	handle of the encoder.
//		Returns:	none.
//		Notes:
//
INT32 H264DEC_API h264_dec_surface_unlock(h264_dec_handle h);

#ifdef linux
//
//		Name:		h264_debug_performance
//		Purpose: 	Debug decoder performance
//		parameters:	debug true/false
//		Returns:		none.
//		Notes:
//
void H264DEC_API h264_debug_performance(INT32 val);

//
//		Name:		h264_debug_performance_eos
//		Purpose: 	Debug decoder performance
//		parameters:	debug true/false
//		Returns:		none.
//		Notes:
//
void H264DEC_API h264_debug_performance_eos(INT32 val);

//
//		Name:		h264_dec_get_surface_physical_addr_surface_lock
//		Purpose: 	Return physical address of surface and locks it
//		parameters:	handle of the encoder.
//		Returns:	none.
//		Notes:
//
INT32 H264DEC_API h264_dec_get_surface_physical_addr_surface_lock(h264_dec_handle h, VIDEO_DEC_CONTEXT *context_ptr, void **psurface, UINT32 *PhyAddr[3], UINT8 *ptr[3], UINT32 stride[3]);

//
//		Name:		h264_dec_buffer_surface_unlock
//		Purpose: 	Unlock surface.
//		parameters:	handle of the surface.
//		Returns:	none.
//		Notes:
//
INT32 H264DEC_API h264_dec_buffer_surface_unlock(void *surface);
#endif


#ifdef __cplusplus
}
#endif

#endif	// H264_DEC_API_H
