/******************************************************************************/
/*                                                                            */
/* SiRF Technology, Inc. GPS Software                                         */
/*                                                                            */
/* SiRF Technology, Inc. CONFIDENTIAL AND PROPRIETARY. This source is the     */
/* sole property of SiRF Technology, Inc. Reproduction or utilization of      */
/* this source in whole or in part is forbidden without the written consent   */
/* of SiRF Technology, Inc.                                                   */
/*                                                                            */
/******************************************************************************/
/*                                                                            */
/* (c) SiRF Technology, Inc. 1996 -- All Rights Reserved                      */
/*                                                                            */
/******************************************************************************/

#ifndef __MM_CODEC_COMMON_H__
#define __MM_CODEC_COMMON_H__

#include "sirf_portable.h"

#if defined (CODEC_API_DLL)
#ifdef linux
#define DECL_CODEC_API(T, F)		T F
#else
#define DECL_CODEC_API(T, F)		__declspec(dllexport) T _stdcall F
#endif
#else
#ifdef linux
#define DECL_CODEC_API(T, F)		T F
#else
#define DECL_CODEC_API(T, F)		T _stdcall F
#endif
#endif


//
// common definitions
//

#define CODEC_STAT_RECOV_ERROR(ret)			((ret) >= CODEC_RCV_ERROR)
#define CODEC_STAT_OK(ret)					((ret) <= CODEC_STATE_OK)
#define CODEC_STAT_FAT_ERROR(ret)			((ret >= CODEC_FAT_ERROR) && (ret) < CODEC_RCV_ERROR)


// normal return codes
#define CODEC_STATE_SLICE				(-3)			// only slice, not end of a frame
#define CODEC_STATE_HEADER              (-2)			// only header info, no frame data
#define CODEC_STATE_BUFFER              (-1)			// need more video bitstream
#define CODEC_STATE_OK                  (0)


// fatal error codes
#define CODEC_FAT_ERROR                 (0x10)
#define CODEC_FAT_CAPS                  (CODEC_FAT_ERROR + 1)
#define CODEC_FAT_PARAMS                (CODEC_FAT_ERROR + 2)
#define CODEC_FAT_BUFFER                (CODEC_FAT_ERROR + 3)
#define CODEC_FAT_MEMORY                (CODEC_FAT_ERROR + 4)

#define CODEC_FAT_HW                    (0x21)

// video caps
typedef struct tagVIDEO_CAPS
{
    UINT32      size;
    UINT32      version;
    UINT32      buffer_attrib;          // cacheable
    UINT32      addr_align_bytes;
    UINT32      stride_align_bytes;
    UINT32      hor_edge_pels;
    UINT32      vert_edge_pels;
    UINT32      frame_buffer_num;
    CHAR        description[128];
} VIDEO_CAPS;


//
// decoder API
//

// recoverable error codes
#define CODEC_RCV_ERROR                 (0x100)
#define CODEC_RCV_SYNTAX_HEADER         (CODEC_RCV_ERROR + 0x20)
#define CODEC_RCV_SYNTAX_FRAME          (CODEC_RCV_ERROR + 0x40)
#define CODEC_RCV_SYNTAX_MACROBLOCK     (CODEC_RCV_ERROR + 0x60)
#define CODEC_RCV_SYNTAX_BLOCK          (CODEC_RCV_ERROR + 0x80)



//surface management
#define SURFACE_MGR_DRIVER              0
#define SURFACE_MGR_RENDER              1


typedef struct
{
    UINT32  size;                       // size of this struct
    UINT32  width;                      // [out], width from decoder
    UINT32  height;                     // [out], height from decoder
    UINT8   *buf_ptr;                   // [in]
    UINT32  buf_len;                    // [in, out]
    void    *pSurfHdl;                  // [out]
} VIDEO_DEC_CONTEXT;


//
// encoder API
//


// frame type
#define FT_ANY          -1
#define FT_I            0
#define FT_P            1
#define FT_N            2
#define FT_B            3


typedef struct
{
    UINT32  size;                       // size of this struct
    UINT8   *buf_ptr;                   // [in]
    UINT32  buf_len;                    // [in/out]
    UINT32  type;                       // [in/out]
} VIDEO_ENC_CONTEXT;


#endif // __MM_CODEC_COMMON_H__
