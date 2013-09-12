//********************************************************************
//Renesas AAC/AAC+ middleware
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Component:	AAC/AAC+
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************
#ifndef _LIB264D_H
#define _LIB264D_H

//**************************************
// Include file dependencies
//**************************************
#include "player.h"
#include "avcbd.h"
#include "avcbd_optionaldata.h"

//**************************************
//Typedef
//**************************************
typedef unsigned char uint8;
typedef unsigned int uint32;
typedef TAVCBD_FMEM FrameInfo;

//**************************************
//Public Function
//**************************************
void VPUD_init(void);
void VPUD_deinit(void);
void VPUD_ioctl(unsigned long command,void* param,unsigned long param_size,unsigned long* result);
 
//**************************************
//Private Function
//**************************************
static unsigned long VPUD_sub_init(void);
static unsigned long VPUD_sub_deinit(void);
static unsigned long VPUD_sub_init_decoder(void);
static unsigned long VPUD_sub_decodeSPSframe(void);
static unsigned long VPUD_sub_decode1idr(void);
inline static unsigned long VPUD_sub_incrase_position(unsigned long len);

//**************************************
//Decoding type
//**************************************
enum VPU_TYPE{
	/* stream_type of StreamInfo */
	TYPE_H264 = 0,
	TYPE_MPEG4 = 1,
	TYPE_H263 = 2
};
enum VPU_DECTYPE{
	VPU_DECODE_H264,
	VPU_DECODE_MPEG4,
};
//**************************************
//Behavior setting
//**************************************
#define VPU_MAX_FRAME_W	720
#define VPU_MAX_FRAME_H	576
#define VPU_NUM_INPUT	1
#define VPU_MAX_SIZE_SEARCH_BITS	512*8
#define VPU_SIZE_NALBUF	256 * 1024
#define VPU_FRAME_NUM	3
#define VPU_FRAME_X		VPU_MAX_FRAME_W
#define VPU_FRAME_Y		VPU_MAX_FRAME_H
#define VPU_WATCHDOG	4095
#define TO_CACHE(addr) ((void *)((long)addr & ~0xa0000000))
#define TO_NONCACHE(addr) ((void *)((long)addr | 0xa0000000))

//**************************************
//Structure
//**************************************
typedef struct vpu_stream_point_info{
	unsigned char *addr;
	unsigned long size;
}VPU_STREAM_POINT_INFO;

typedef struct vpu_decoder_context{
	//Stream type
	VPU_TYPE		stream_type;
	
	//Decode memory pointer
	//unsigned char	*input;
	
	//Working memory
	unsigned long	*context_mem;
	unsigned long	context_size;
	
	//Working buffer
	unsigned char	*buffer_mem;
	unsigned long	buffer_size;
	unsigned long	buffer_pos;
	
	//H264 information
	TAVCBD_VUI_PARAMETERS	vui;
	TAVCBD_SEI				sei;
	unsigned long			current_extra_length;
	
	//MPEG4 information
	long *dp1, *dp2;
	
	//Frame info
	unsigned long	frame_num;
	FrameInfo		frame_list[VPU_FRAME_NUM];
	FrameInfo		filtered_frame;
	//FrameInfo		frameinfo[VPU_FRAME_NUM];
	int				frame_x;
	int				frame_y;
	int				mb_num; //Macroblock
	
	//Callback function
	void*			(*video_output_cb)(unsigned char *Y , unsigned char *C );
	unsigned long	(*reading_cb)(unsigned char *addr , unsigned long count);
	void			(*retrieve_cb)(int x , int y , int mode);
}VPU_DECODER_CONTEXT;

//**************************************
//Macro
//**************************************
#if !defined(__RENESAS_VERSION__) || defined(SOFT_ONLY)
void workaround_vpu4_cmodel(void);
#define usr_log printf
#else
#ifndef VPU4
#define VPU4
#endif
#define usr_log
#endif

#define ALIGN(a, w) (void *)(((long)(a) + (w) - 1) & ~((w) - 1))

//**************************************
//IOCTL command
//**************************************
enum VPUD_IOCTL{
	VPUD_Init = 0,
	VPUD_Deinit,
	VPUD_SetFormat,
	VPUD_DecodeOneFrame,
	VPUD_Probe,
	VPUD_SetSPS_Callback,
	VPUD_SetVideoOutput_Callback,
	VPUD_SetReading_Callback,
	VPUD_DecodeSPS,
	VPUD_SetDecodePointInfo,
	VPUD_SetFrameX,
	VPUD_SetFrameY,
	//HACK!
	VPUD_SETOFFSETZERO,
	VPUD_SkipOnePackage,
};

//**************************************
//Error handling
//**************************************
enum VPU_ERROR{
	VPUD_OK = 0,
	VPUD_EOF,
	VPUD_ERR_DEVICE,
	VPUD_INVALID_ADDRESS,
	VPUD_EXCESS_FRAME_SIZE,
	VPUD_INVALID_FORMAT,
	VPUD_NAL_OVERFLOW,
	VPUD_DECODE_FAIL,
	VPUD_I,
};

#endif //_LIB264D_H