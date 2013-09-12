/* <LIC_AMD_STD>
 * Copyright (C) 2003-2005 Advanced Micro Devices, Inc.  All Rights Reserved.
 * 
 * Unless otherwise designated in writing, this software and any related 
 * documentation are the confidential proprietary information of AMD. 
 * THESE MATERIALS ARE PROVIDED "AS IS" WITHOUT ANY
 * UNLESS OTHERWISE NOTED IN WRITING, EXPRESS OR IMPLIED WARRANTY OF ANY 
 * KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
 * NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE AND IN NO 
 * EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER. 
 * 
 * AMD does not assume any responsibility for any errors which may appear 
 * in the Materials nor any responsibility to support or update the
 * Materials.  AMD retains the right to modify the Materials at any time, 
 * without notice, and is not obligated to provide such modified 
 * Materials to you. AMD is not obligated to furnish, support, or make
 * any further information available to you.
 * </LIC_AMD_STD>  */
/* <CTL_AMD_STD>
 * </CTL_AMD_STD>  */
/* <DOC_AMD_STD>
 * </DOC_AMD_STD>  */
#ifndef _MAE_INTERFACE_H
#define _MAE_INTERFACE_H

#define NEW_MAE_DRIVER 1 

// Enable this flag to generate some checker files ".hw" 
// (for the driver/hardware) & ".cm" (for the decoder/CModel)
//#define VALIDATION_DUMP

#if defined(WIN32) || defined(UNDER_CE)

// Needed for working with the Registery
#include <windows.h>
#include <Winreg.h>

#if defined(UNDER_DRIVER_CE)
#include "mae_types.h"
#else
#include "types.h"  // in travis/env directory
#endif

#else //!WIN32 && !UNDER_CE
#include "types.h"  // in travis/env directory
#include <linux/ioctl.h>
#endif //WIN32 || UNDER_CE

/* This version will be bumped on changes to MAE interface */
#define MAE_INTERFACE_VERSION            0x00010001
/* Masks can be used to check for Major or Minor version changes */
/*   Major version changes if driver & mae interface compatibility diverges */
/*   Minor version changes if only minor (no interface) changes */
#define MAE_INTERFACE_VERSION_MAJORMASK  0xFFFF0000
#define MAE_INTERFACE_VERSION_MINORMASK  0x0000FFFF

#ifdef UNDER_CE
#define maeIOCTLCode(f,m)     \
  CTL_CODE ( FILE_DEVICE_UNKNOWN, 0x800 | (f), (m), FILE_ANY_ACCESS )

#define AU1XXXMAE_INIT                maeIOCTLCode(0, METHOD_BUFFERED)
#define AU1XXXMAE_REQUEST             maeIOCTLCode(1, METHOD_BUFFERED)
#define AU1XXXMAE_SUBMIT              maeIOCTLCode(2, METHOD_BUFFERED)
#define AU1XXXMAE_PRIME_FE            maeIOCTLCode(3, METHOD_BUFFERED)
#define AU1XXXMAE_CMODEL_ISR          maeIOCTLCode(4, METHOD_BUFFERED)
#define AU1XXXMAE_SETMODE             maeIOCTLCode(5, METHOD_BUFFERED)
#define AU1XXXMAE_GETYUVBUFADDR       maeIOCTLCode(6, METHOD_BUFFERED)
#define AU1XXXMAE_GETYUVBUF           maeIOCTLCode(7, METHOD_BUFFERED)
#define AU1XXXMAE_DISPLAYSIZE         maeIOCTLCode(8, METHOD_BUFFERED)
#define AU1XXXMAE_WAKEUP              maeIOCTLCode(9, METHOD_BUFFERED)
#define AU1XXXMAE_GETDISPLAYSIZE      maeIOCTLCode(10, METHOD_BUFFERED)
#define AU1XXXMAE_SUBMIT_BE           maeIOCTLCode(11, METHOD_BUFFERED)
#define AU1XXXMAE_REQUEST_BE          maeIOCTLCode(12, METHOD_BUFFERED)
#define AU1XXXMAE_UNINIT              maeIOCTLCode(13, METHOD_BUFFERED)
#define AU1XXXMAE_WRITE_TIMESTAMP     maeIOCTLCode(14, METHOD_BUFFERED)
#define AU1XXXMAE_SETSYNCMODE         maeIOCTLCode(15, METHOD_BUFFERED)
#define AU1XXXMAE_GETSYNCMODE         maeIOCTLCode(16, METHOD_BUFFERED)
#define AU1XXXMAE_GETSTATE            maeIOCTLCode(17, METHOD_BUFFERED)
#define AU1XXXMAE_ENABLE_COUNT        maeIOCTLCode(18, METHOD_BUFFERED)
#define AU1XXXMAE_DISABLE_COUNT       maeIOCTLCode(19, METHOD_BUFFERED)
#define AU1XXXMAE_IGNORE_PTS          maeIOCTLCode(20, METHOD_BUFFERED)
#define AU1XXXMAE_SETDISPLAYRECT      maeIOCTLCode(21, METHOD_BUFFERED)
#define AU1XXXMAE_STOP                maeIOCTLCode(22, METHOD_BUFFERED)
#define AU1XXXMAE_PAUSE               maeIOCTLCode(23, METHOD_BUFFERED)
#define AU1XXXMAE_RUN                 maeIOCTLCode(24, METHOD_BUFFERED)
#define AU1XXXMAE_ISDISPLAYED         maeIOCTLCode(25, METHOD_BUFFERED)
#define AU1XXXMAE_VIDEOROTATION       maeIOCTLCode(26, METHOD_BUFFERED)
#define AU1XXXMAE_FLUSH               maeIOCTLCode(27, METHOD_BUFFERED)
#define AU1XXXMAE_ENDOFSTREAM         maeIOCTLCode(28, METHOD_BUFFERED)
#define AU1XXXMAE_SETMODE2            maeIOCTLCode(29, METHOD_BUFFERED)
#define AU1XXXMAE_GETDISPLAYCONFIG    maeIOCTLCode(30, METHOD_BUFFERED)
#define AU1XXXMAE_GETYUVCONFIG        maeIOCTLCode(31, METHOD_BUFFERED)
#define AU1XXXMAE_GETPRID             maeIOCTLCode(32, METHOD_BUFFERED)
#define AU1XXXMAE_FLUSH_QUEUES        maeIOCTLCode(33, METHOD_BUFFERED)
#define AU1XXXMAE_GETCURRENTSTATE     maeIOCTLCode(34, METHOD_BUFFERED)
#define AU1XXXMAE_GETSCALERS          maeIOCTLCode(35, METHOD_BUFFERED)
#define AU1XXXMAE_GETDIVXSTR          maeIOCTLCode(36, METHOD_BUFFERED)
#define AU1XXXMAE_GETINTERRUPTSTATE   maeIOCTLCode(37, METHOD_BUFFERED)
#define AU1XXXMAE_SETPLAYERRECT       maeIOCTLCode(38, METHOD_BUFFERED)
#define AU1XXXMAE_SETMEDIATYPE		    maeIOCTLCode(39, METHOD_BUFFERED)
#define AU1XXXMAE_SET_YUV_FORMAT      maeIOCTLCode(40, METHOD_BUFFERED)
#define AU1XXXMAE_FORCEOVERLAY        maeIOCTLCode(41, METHOD_BUFFERED)
#define AU1XXXMAE_GETDECIMATIONFACTOR        maeIOCTLCode(42, METHOD_BUFFERED)
#define AU1XXXMAE_SETDECIMATIONFACTOR        maeIOCTLCode(43, METHOD_BUFFERED)
#define AU1XXXMAE_SUBMIT_TILE         maeIOCTLCode(44, METHOD_BUFFERED)

// added extra 4 bits for UV Order that needs to be shifted off
#define MAE_UYVY				0x440	// Interleave mode on, interleave format UYVY, and Input Format 4:2:2
#define MAE_VYUY				0x540	// Interleave mode on, interleave format VYUY, and Input Format 4:2:2
#define MAE_YUYV				0x640	// Interleave mode on, interleave format YUYV, and Input Format 4:2:2
#define MAE_YVYU				0x740	// Interleave mode on, interleave format YVYU, and Input Format 4:2:2
#define MAE_YUV420				0x001	// Interleave mode off and Input Format 4:2:0 
#define MAE_YUV422				0x041	// Interleave mode off and Input Format 4:2:2	
#define MAE_YUV411				0x081	// Interleave mode off and Input Format 4:1:1	
#define MAE_YUV444				0x0C1	// Interleave mode off and Input Format 4:4:4	
#define MAE_YVU420				0x000	// Interleave mode off and Input Format 4:2:0	
#define MAE_YVU422				0x040	// Interleave mode off and Input Format 4:2:2	
#define MAE_YVU411				0x080	// Interleave mode off and Input Format 4:1:1	
#define MAE_YVU444				0x0C0	// Interleave mode off and Input Format 4:4:4	

#else //!UNDER_CE
#define AU1XXXMAE_IOC_MAGIC 'M'
                    
#define AU1XXXMAE_INIT                _IOW(AU1XXXMAE_IOC_MAGIC, 0, int *)
#define AU1XXXMAE_REQUEST             _IOW(AU1XXXMAE_IOC_MAGIC, 1, int *)
#define AU1XXXMAE_SUBMIT              _IOW(AU1XXXMAE_IOC_MAGIC, 2, int *)
#define AU1XXXMAE_PRIME_FE            _IOW(AU1XXXMAE_IOC_MAGIC, 3, int *)
#define AU1XXXMAE_CMODEL_ISR          _IOW(AU1XXXMAE_IOC_MAGIC, 4, int *)
#define AU1XXXMAE_SETMODE             _IOW(AU1XXXMAE_IOC_MAGIC, 5, int *)
#define AU1XXXMAE_GETYUVBUFADDR       _IOW(AU1XXXMAE_IOC_MAGIC, 6, int *)
#define AU1XXXMAE_GETYUVBUF           _IOW(AU1XXXMAE_IOC_MAGIC, 7, int *)
#define AU1XXXMAE_DISPLAYSIZE         _IOW(AU1XXXMAE_IOC_MAGIC, 8, int *)
#define AU1XXXMAE_WAKEUP              _IOW(AU1XXXMAE_IOC_MAGIC, 9, int *)
#define AU1XXXMAE_GETDISPLAYSIZE      _IOW(AU1XXXMAE_IOC_MAGIC, 10, int *)
#define AU1XXXMAE_SUBMIT_BE           _IOW(AU1XXXMAE_IOC_MAGIC, 11, int *)
#define AU1XXXMAE_REQUEST_BE          _IOW(AU1XXXMAE_IOC_MAGIC, 12, int *)
#define AU1XXXMAE_WRITE_TIMESTAMP     _IOW(AU1XXXMAE_IOC_MAGIC, 13, int *)
#define AU1XXXMAE_UNINIT              _IOW(AU1XXXMAE_IOC_MAGIC, 14, int *)
#define AU1XXXMAE_SETSYNCMODE         _IOW(AU1XXXMAE_IOC_MAGIC, 15, int *)
#define AU1XXXMAE_GETSYNCMODE         _IOW(AU1XXXMAE_IOC_MAGIC, 16, int *)
#define AU1XXXMAE_GETSTATE            _IOW(AU1XXXMAE_IOC_MAGIC, 17, int *)
#endif //UNDER_CE

/* Use with AU1XXXMAE_SETSYNCMODE, AU1XXXMAE_GETSYNCMODE */
#define MAE_SYNCMODE_IGNOREPTS  0  /* No synchronization (Ignore PTS) */
#define MAE_SYNCMODE_PLAY       1  /* Synchronize to reference clock */
#define MAE_SYNCMODE_PAUSE      2  /* Pause clocks */

// MAE Driver return codes
#define MAE_OK                  0
#define MAE_TIMEOUT             1
#define MAE_NO_DATA_BUFFER      2
#define MAE_NO_MAE_BUFFER       3
#define MAE_GENERAL_ERROR       4


/* Use with AU1XXXMAE_GETSTATE */
typedef struct _mae_state_struct
{
  uint32 version;   /* MAE_INTERFACE_VERSION */
  uint32 flags;     /* MAE_STATEFLAG_xxx */
  uint32 timestamp_lo; /* timestamp (low 32 bits) of current frame rendered */
  uint32 timestamp_hi; /* timestamp (hi 32 bits) of current frame rendered */
  uint32 encoded_format;
  uint32 encoded_width;
  uint32 encoded_height;
  uint32 reserved1; /* reserved for future use */
  uint32 reserved2; /* reserved for future use */
  uint32 reserved3; /* reserved for future use */
} MAE_STATE_STRUCT, *PMAE_STATE_STRUCT;

#define MAE_STATEFLAG_NONE           0x00000000
#define MAE_STATEFLAG_INIT           0x00000001  /* encoded format is known */
#define MAE_STATEFLAG_FORMATKNOWN    0x00000002  /* encoded format is known */
#define MAE_STATEFLAG_TIMEKNOWN      0x00000004  /* timestamp is known */
#define MAE_STATEFLAG_FRAMESUBMITTED 0x00000008
#define MAE_STATEFLAG_FRAMERENDERED  0x00000010

#define CONFIG0_MASK        0x00000001
#define CUR_Y_MASK          0x00000002
#define FWD_Y_TOP_MASK      0x00000004
#define BWD_Y_TOP_MASK      0x00000008
#define CUR_CB_MASK         0x00000010
#define FWD_CB_TOP_MASK     0x00000020
#define BWD_CB_TOP_MASK     0x00000040
#define CUR_CR_MASK         0x00000080
#define FWD_CR_TOP_MASK     0x00000100
#define BWD_CR_TOP_MASK     0x00000200

#define PICTURE_SIZE_MASK   0x00000400
#define INTENSITY_COMP_MASK 0x00000800


#define FWD_Y_BOT_MASK      0x00001000
#define FWD_CB_BOT_MASK     0x00002000
#define FWD_CR_BOT_MASK     0x00004000
#define BWD_Y_BOT_MASK      0x00008000
#define BWD_CB_BOT_MASK     0x00010000
#define BWD_CR_BOT_MASK     0x00020000

#define PQUANT_MASK         0x00040000

#define SEND_QUEUE           0   // Send one frame or n macroblocks at a time to MAE
#define SEND_BATCH           1   // Send the current and all accumulated MBs to MAE
#define SEND_FORCE           2   // Skip the current MB, but send the accumulated MBs to MAE

#ifndef boolean
typedef unsigned char boolean;
#endif

#define MAX_PICTURE_SIZE    (720 * 576)
#ifndef WM_DATA_SIZE
#define WM_DATA_SIZE      (BLOCK_SQUARE_SIZE)
#endif
#define DESCRIPTOR_ENTRY_SIZE   8   // Each descriptor entry is 64 bits

#define WRAPSKIPMVS(pWrapMB,NumWM,NumMV)\
((uint32 *)pWrapMB + (sizeof(header_words)/4) + (sizeof(Weighing_matrix)*NumWM/4) + NumMV);

// Stream Types (set by the respective decoders)
#define MAE_STREAM_TYPE_UNKNOWN         0
#define MAE_STREAM_TYPE_MPEG1           1
#define MAE_STREAM_TYPE_MPEG2           2
#define MAE_STREAM_TYPE_MPEG4           3
#define MAE_STREAM_TYPE_H263            4
#define MAE_STREAM_TYPE_DIVX            5
#define MAE_STREAM_TYPE_WMV9            6

// De-Interlacing related defines                
#define	APP_DEINTERLACE_OFF			  0x01
#define APP_DEINTERLACE_ON			  0x02
#define APP_DEINTERLACE_AUTO		  0x03
#define DECODER_DEINTERLACE_OFF		0x11
#define DECODER_DEINTERLACE_ON		0x12
            
typedef struct _mae_desc_struct
{
  // MAE DMA descriptor word 0
  uint32 ByteCount:22;       // Number of bytes pointed to by the address pointer
  uint32 reserved:6;         // this must be zero
  uint32 reserved2:1;
  uint32 reserved3:2;
  uint32 Valid:1;            // Indicates if this is a valid descriptor

  // MAE DMA descriptor word 1
  uint32 address;            // Physical address where the data is located
}mae_desc;

typedef struct _mae_disp_struct
{
  uint32 display_linesize;
  uint32 display_height;
  uint32 rotation; /* 0=normal, 1=90 deg, 2=180 deg, 3=270 deg */
  uint32 format;   /* display format (i.e. 16bit, 32bit, etc.) */
}MAE_DISP_STRUCT, *PMAE_DISP_STRUCT;

typedef struct _mae_mode_struct
{
  uint32 operating_mode;
  uint32 playback_speed;
}MAE_MODE_STRUCT, *PMAE_MODE_STRUCT;

/* Use with AU1XXXMAE_GETDIVXSTR */
typedef struct _divx_drm_struct
{
    uint32 reg30;
    uint32 reg34;
    uint32 reg60;
    uint32 prid;
    uint32 config1;
    uint32 reg1000;
    uint32 reg100c;
    uint32 reg101c;
}DIVX_DRM_STRUCT, *PDIVX_DRM_STRUCT;

typedef struct header_word0_struct
{
  uint32 dcchroma:6;
  uint32 dcluma:6;
  uint32 iqadd1:6;
  uint32 wtchgmsk:4;
  uint32 iqmul2:9;
  uint32 bblk:1;
}_header_word0;

typedef struct header_word1_struct
{
  uint32 iqadd2:9;
  uint32 iqdiv3:3;
  uint32 cbp:8;
  uint32 reserved1:12;
}_header_word1;

typedef struct header_word2_struct
{
  uint32 xformsize:16;
  uint32 mbmode:16;
}_header_word2;

typedef struct header_word3_struct
{
  uint32 ypos:8;
  uint32 xpos:8;
  uint32 mcprecuv:2;
  uint32 mcprecy:2;
  uint32 mbtype:3;
  uint32 bb:1;
  uint32 bt:1;
  uint32 fb:1;
  uint32 ft:1;
  uint32 fp:1;
  uint32 dctt:1;
  uint32 ps:1;        // picture structure replaces old tff bit
  uint32 rnd:1;
  uint32 reserved2:1; // must be zero
}_header_word3;

typedef struct header_words_struct
{
  _header_word0 header_word0;
  _header_word1 header_word1;
  _header_word2 header_word2;
  _header_word3 header_word3;
} header_words;

typedef struct _reg_info_struct
{
  struct config0_struct
  {
    uint32      reserved4:11;
    uint32      satiq:1;                // 1-bit
    uint32      smooth:1;               // 1-bit
    uint32      loopfilt:1;             // 1-bit
    uint32      reserved3:2;
    uint32      mis:1;                  // 1-bit
    uint32      codstyl:1;
    uint32      picture_level_mbmode:2;  // 2 bits
    uint32      reserved2:4;
    uint32      iqmul1:2;               // 2-bits
    uint32      bc:1;                   // 1-bit
    uint32      reserved1:5;
  }config0;

  uint32        cur_y_frame_ptr;      // 32-bits
  uint32        fwd_y_top_ptr;        // 32-bits
  uint32        bwd_y_top_ptr;        // 32-bits
  uint32        cur_cb_frame_ptr;     // 32-bits
  uint32        fwd_cb_top_ptr;       // 32-bits
  uint32        bwd_cb_top_ptr;       // 32-bits
  uint32        cur_cr_frame_ptr;     // 32-bits
  uint32        fwd_cr_top_ptr;       // 32-bits
  uint32        bwd_cr_top_ptr;       // 32-bits

  struct picture_size_struct
  {
    uint32      linesiz:10;              // 16-bits
    uint32      reserved6:6;
    uint32      height:10;               // 16-bits
    uint32      reserved5:6;
  }picture_size;

  struct intensity_comp_struct
  {
    uint32      lumscale:6;             // 6-bits
    uint32      lumshift:6;             // 6-bits
    uint32      icomp:1;                // 1-bit
    uint32      reserved7:19;
  }intensity_comp;

  uint32        fwd_y_bot_ptr;
  uint32        fwd_cb_bot_ptr;
  uint32        fwd_cr_bot_ptr;
  uint32        bwd_y_bot_ptr;
  uint32        bwd_cb_bot_ptr;
  uint32        bwd_cr_bot_ptr;
  
  struct pquant_struct
  {
    uint32      pquant:5;       // 5-bits
    uint32      reserved:27;      // 27-bits
  }pquant_reg;

  uint32        regmask;      // Indicates which registers are valid
}reg_info;

typedef struct Weighing_matrix_struct
{
  uint8  wm[64];
}Weighing_matrix;

typedef struct motion_vector_struct
{
  uint32 mv;
}motion_vector;

typedef struct data_block_struct
{
  int16 blk_data[64];
}data_block;

typedef struct tag_bufq_t 
{
  uint32 *addr;
  struct tag_bufq_t *next;
  struct tag_bufq_t *prev;
} YUVQ_T, *PYUVQ_T;

typedef struct tag_dataq_t 
{
  uint32 *addr;
  struct tag_dataq_t *next;
  struct tag_dataq_t *prev;
} DATAQ_T, *PDATAQ_T;

typedef struct tag_feq_t
{
  LONGLONG        pts;
  LONGLONG        dts;
  uint32          tnum;
  uint32          state_change; // set by decoder to turn on/off modules
  uint32          endframe_flag;
  uint32          anchor_flag;
  uint32          update_mode_flag;
  uint32          interlacing_on;
  uint32          interlaced_frame_done;
  uint32          topfield_length;
#ifdef NEW_MAE_DRIVER
  uint32          stream_type; // unkown - 0, mpeg1 - 1, mpeg2 - 2, mpeg4 - 3, H.263 - 4, divx - 5, wmv9 - 6
  uint32          frametype[2]; // For field picture - field prediction, we set both frametype1 and frametype2
  uint32          picture_structure; // 1 - top field, 2 - bottom field, 3 - frame picture
  uint32          chroma_format; // 0 - 4:2:0, 1 - 4:2:2
  uint32          dual_prime_field1;
  uint32          nFlushMAE;
  uint32          prog_frame;  // Is this a progressive frame?
//  WMV9 Stuff
  unsigned char   bOverlap;   // Overlap Smoothing filter enabled?
  unsigned char   bDeblock;   // DeBlocking filter enabled?

  uint32          IntensityComp;   // Intensity Compensation enabled?
  uint32          LumScale;        // Luminance Scale
  uint32          LumShift;        // Luminance Shift

  uint32          StepSize;         // Step Size for PQuant
// ~WMV9 Stuff
#else
  uint32          frametype;
#endif
  uint32          dummysubmit; // Added for trick mode support
  uint32          list_entries;
  uint32          sent_to_be; // internal flag set when buffer is processed by BE
  reg_info        fe_reglist;
  reg_info        fe_reglist_field;

#ifdef REG_DEBUG
  reg_info        mae_fe_reglist;
  reg_info        mae_fe_reglist_field;
#endif

  uint32        * extra_info; // address ptr to head of contiguous block of extra info for cmodel
  uint32          extra_info_size;  // size (in bytes) of extra info

  uint32        * descriptors; // address ptr to head of contiguous descriptors
  uint32          desc_size;  // size (in bytes) of descriptors
  mae_desc      * desc_ptr;
  PDATAQ_T      pdataq;
  uint32        * buffer;
  uint32          buffer_offset;
  uint32        * pMB;
  header_words  * pHeader;
  uint32        * pWM;
  uint32        * pMV;
  uint16        * pDataBlock;
  uint32        * pWrapExtraInfo;

  uint32          status;
  uint32          start_time;
  uint32          end_time;

  PYUVQ_T         pyuvq; // associated YUV_Q entry
  uint32          * y;  // address ptr to head of Y data
  uint32          * u;  // address ptr to head of Y data
  uint32          * v;  // address ptr to head of Y data

  uint32          encoded_picture_linesize;
  uint32          encoded_picture_height;
  uint32          aspect;
  uint32          chopped_picture_linesize;
  uint32          chopped_picture_height;
  uint32          in_frame_width;     // decoder's In-Frame X boundary
  uint32          in_frame_height;    // decoder's In-Frame Y boundary
  uint32          tweak_oof_region;   // modify MAE-FE decoded output

  // Added for YUV Tiling
  uint32          nXPosInRGBBuffer;  // X Position in the RGB buffer for this tile
  uint32          nYPosInRGBBuffer;  // Y Position in the RGB buffer for this tile
  uint32          nTileWidth;        // The width of this tile
  uint32          nTileHeight;       // The height of this tile
  char            bNotFinal;            // Set this if it's NOT the final tile going into the RGB buffer.  Should be set to 0 for all non-tile submits
    
  //uint32        scf_hsr; // Horz Scale Ratio
  //uint32        scf_vsr; // Vert Scale Ratio
  //uint32        scf_dis; // SCF Disable
  uint32          scf_coeffs[256]; // scf filter coefficients for 0x100-0x400
  uint32          csc_coeffs[13]; // csc filter coefficients for 0x400-0x434
  // the list below is for the source/destination/control registers
  //REGLIST_T     be_reglist[19];  // array of (address, value) structs
  uint32        * rgb;   // address ptr to head of rgb data
  //uint32        start_time;  // for debug, time clock is written before BE is enabled
  uint32          total_time;  // for debug, time clock is written when MAE completes
  uint32          be_time;  // for debug, time clock is written if be_time completes early

  struct tag_feq_t  *next;
  struct tag_feq_t  *prev;
} MAEQ_T, *PMAEQ_T;

typedef MAEQ_T wrap_context;


#ifdef UNDER_CE
typedef unsigned int *   PUINT32;

typedef struct tag_rgbq_t 
{
  LONGLONG pts;  // Presentation Time Stamp
  LONGLONG dts;  // Decode Time Stamp
  uint32 tnum;   // Temporal Number
  uint32 ready_to_display; // set once the buffer is completed
  uint32 start_time; // time clock is written when LCD is updated to display this frame
  uint32 resync_clock;
  int32 x;
  int32 y;
  int32 w;
  int32 h;
  PUINT32 addr; // address ptr for head of rgb data
  struct tag_rgbq_t *next;
  struct tag_rgbq_t *prev;
} RGBQ_T, *PRGBQ_T;

#ifndef UNDER_DRIVER_CE // do'nt define for the driver 
#define IPB_FRAME_MODE    (0)

typedef struct reg_data_struct
{
  uint32 offset;
  char szRegName[24];
}reg_data;

typedef struct tag_yuv_table
{
  PYUVQ_T pyuvq;
  char szBufName[4];
}YUV_TABLE;
#if 0
typedef struct tag_dispq_t 
{
  uint32 pts;  // Presentation Time Stamp
  uint32 dts;  // Decode Time Stamp
  uint32 tnum; // Temporal Number
  uint32 ready_to_display; // set once the buffer is completed
  uint32 start_time; // time clock is written when LCD is updated to display this frame
  PUINT32 rgb; // address ptr for head of rgb data
  struct tag_dispq_t *next;
  struct tag_dispq_t *prev;
} DISPQ_T, *PDISPQ_T;

#endif

// KK - this is used in special cases where debug prints can cause the timing to change (and not show up a certain problem)
#define NUM_DUMP_ENTRIES 150

typedef struct tag_dumpq_t 
{
  uint32 numEntries;
  char szDumpStr[NUM_DUMP_ENTRIES][80];
}DUMPQ_T, *PDUMPQ_T;


typedef struct tag_mae_master_t 
{
  // User memory pointer indicies
  // we need these to help the decoder and cmodel point into the current working data locations.
  // Because the decoder and cmodel are asynchronous
  // the cmodel and decoder will point to different buffers at any given time.
  uint32           decoder_maeq_offset;
  uint32           decoder_desc_offset;
  uint32           buffer_offset;
#ifndef MAE_HW
  uint32           decoder_extra_info_offset;
#endif
  uint32           cmodel_maeq_offset;
  uint32           cmodel_desc_offset;
#ifndef MAE_HW
  uint32           cmodel_extra_info_offset;
  uint32           MAEConfig_offset;
#endif
  uint32           yuv_region_offset; // KK

  // Status of MAE hardware
  uint32           state; // MAE_IDLE, MAE_FEBE_BUSY, MAE_FE_BUSY, FE_ENABLED, BE_ENABLED, LCD_ENABLED
  uint32           video_out_linesize;
  uint32           video_out_height;
  uint32           initial_display_frame;
  uint32           operating_mode;
  uint32           operating_mode_status;
  uint32           pipeline_be_delay;
  uint32           pipeline_discard_delay;
  uint32           playback_speed;

  uint32           StillSleeping;
  void (*wake_up_function)(void);
  uint32           nFramesInSystem;
  uint32           InterruptMask;

  uint32           fe_regmask; // KK - this makes it easier to switch from decoder address space to cmodel
  uint32           decoder_pmms;
  uint32           cmodel_pmms;

  // MAE Queue
  PMAEQ_T          sw_proc;
  PMAEQ_T          fe_wait;
  PMAEQ_T          fe_proc;
  PMAEQ_T          be_proc;
  PMAEQ_T          maeq_free;

  // Display Queue
  PRGBQ_T          rgb_write;
  PRGBQ_T          rgb_timer;
  PRGBQ_T          rgb_draw;
  PRGBQ_T          rgb_free;

  // Anchor Frames
  PMAEQ_T          b_anchor;
  PMAEQ_T          f_anchor;

  struct fasync_struct *async_queue; // KK

  // Descriptors memory usage
  PUINT32          pDescRingStart;
  PUINT32          pDescRingEnd;
  PUINT32          pCurDescPtr;
  PUINT32          inuse_base;
  PUINT32          inuse_end;
  PUINT32          inuse_bufferbase;
  PUINT32          inuse_bufferend;

  PDATAQ_T         data_inuse;
  PDATAQ_T         data_free;
  PYUVQ_T          yuv_inuse;
  PYUVQ_T          yuv_free;

#ifndef MAE_HW
  PUINT32          extra_info_inuse_base;
  PUINT32          extra_info_inuse_end;
#endif

  PDUMPQ_T pDumpPtr;

} MAE_MASTER_T, *PMAE_MASTER_T;

#endif // under_driver_ce

typedef struct au1xxxmae_ioctl_t
{
  char magic;
  unsigned long ksegbase;
  unsigned long addrpmms;
} au1xxxmae_ioctl_t;

#endif //UNDER_CE

// ***************************************************************************
//  declarations
// ***************************************************************************
#ifdef __cplusplus
extern "C" {
#endif
uint32 InitDescriptor(int);
wrap_context *WrapGetContext(uint32);
void WrapGetNextContext(uint32, wrap_context *);
uint32 *WrapGetMB(uint32);
uint32 *WrapSkipMVs(uint32 *, int, int);
int WrapGetNumWMs(int);
void WrapSetRegisters(reg_info *, unsigned char);
void WrapSubmitMBs (void);
uint32 request_mae_flush(void);
uint32 request_mae_state(uint32 *);
#ifdef UNDER_CE
int mae_ioctl(unsigned int, unsigned long);
int mae_init(void);
void mae_exit(void);
extern wrap_context *g_pWrapContext;

#ifdef VALIDATION_DUMP
extern int g_iBCArray[], g_iMVArray[];
#endif

#endif //UNDER_CE
#ifdef __cplusplus
}
#endif

// ***************************************************************************
//  data
// ***************************************************************************
extern mae_desc *desc_ptr;
#endif

