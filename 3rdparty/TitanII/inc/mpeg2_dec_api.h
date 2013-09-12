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
#ifndef _MPEG2_DEC_API_H_
#define _MPEG2_DEC_API_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "mm_codec_common.h"

typedef void *  mpeg2_dec_handle;

#ifdef linux
#define MPEG2DEC_API
#else
#define MPEG2DEC_API __declspec(dllexport)
#endif

#ifdef linux
/* Bit values for setting the DEBUG PERFORMANCE parameters */
#define DEBUG_PERFORMANCE          0x1
#define DEBUG_PRINT_PER_SEC        0x2
#define DEBUG_MAX_FRAMES_PER_SEC   0x4
#endif

//
//		Name:		mpeg2_dec_open
//		Purpose: 		Create decoder handler.
//		parameters:	none.
//		Returns:		handle of the decoder. If failed, returns NULL
//		Notes:
//
DECL_CODEC_API(mpeg2_dec_handle, mpeg2_dec_open)(UINT32 width, UINT32 height, UINT32 flag);
//
//		Name:		mpeg2_dec_close
//		Purpose: 		Destroy decoder handler.
//		parameters:	handle of the decoder.
//		Returns:		none.
//		Notes:
//
DECL_CODEC_API(void, mpeg2_dec_close)(mpeg2_dec_handle h);
//
//		Name:		mpeg2_dec_process
//		Purpose: 		decode one frame.
//		parameters:	context structure.
//		Returns:		VC_STATE_OK. If failed, the return codes depends on the syntax.
//		Notes:
//
DECL_CODEC_API(INT32, mpeg2_dec_process)(mpeg2_dec_handle h, VIDEO_DEC_CONTEXT *context_ptr);
//
//		Name:		mpeg2_dec_surface_lock
//		Purpose: 	Lock surface to get frame buffer.
//		parameters:	handle of the encoder.
//		Returns:	none.
//		Notes:
//
DECL_CODEC_API(INT32, mpeg2_dec_surface_lock)(mpeg2_dec_handle h, VIDEO_DEC_CONTEXT *context_ptr, UINT8 *ptr[3], UINT32 stride[3]);

//
//		Name:		mpeg2_dec_surface_unlock
//		Purpose: 	Unlock surface.
//		parameters:	handle of the encoder.
//		Returns:	none.
//		Notes:
//
DECL_CODEC_API(INT32, mpeg2_dec_surface_unlock)(mpeg2_dec_handle h);


//
//		Name:		mpeg2_dec_header	
//		Purpose: 	get header information from the stream.
//		parameters:	VIDEO_DEC_CONTEXT structure ptr.
//		Returns:	If returns CODEC_STATE_HEADER, the sequence header and group header are found and decoded correctly
//                  otherwise, the header information may not be found completely.
//		Notes:
//
DECL_CODEC_API(INT32, mpeg2_dec_header)(mpeg2_dec_handle h, VIDEO_DEC_CONTEXT *context_ptr, void *info_ptr);


DECL_CODEC_API(void, mpeg2_dec_lastframe)(mpeg2_dec_handle h, VIDEO_DEC_CONTEXT *context_ptr);


#ifdef linux
//
//              Name:           mpeg2_dec_get_surface_physical_addr_surface_lock
//              Purpose:        Return physical address of surface and locks it
//              parameters:     handle of the encoder.
//              Returns:        none.
//              Notes:
//
DECL_CODEC_API(INT32, mpeg2_dec_get_surface_physical_addr_surface_lock)(mpeg2_dec_handle h, VIDEO_DEC_CONTEXT *context_ptr, void **psurface, UINT32 *PhyAddr[3], UINT8 *ptr[3], UINT32 stride[3]);

//
//		Name:		mpeg2_debug_performance
//		Purpose: 	Debug decoder performance
//		parameters:	debug true/false
//		Returns:	none.
//		Notes:
//
DECL_CODEC_API(void, mpeg2_debug_performance)(INT32 val);

//
//		Name:		mpeg2_debug_performance_eos
//		Purpose: 	Debug decoder performance
//		parameters:	debug true/false
//		Returns:	none.
//		Notes:
//
DECL_CODEC_API(void, mpeg2_debug_performance_eos)(INT32 val);


DECL_CODEC_API(INT32, mpeg2_dec_buffer_surface_unlock)(void *surface);
#endif
#ifdef __cplusplus
}
#endif

#endif

