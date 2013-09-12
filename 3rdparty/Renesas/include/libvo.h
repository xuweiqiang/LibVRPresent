//********************************************************************
//Renesas audio output unit
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Component:	libvo
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************
#ifndef _LIBVO_H
#define _LIBVO_H

#include "player.h"
//**************************************
//Public Function
//**************************************
void VO_init(VIDEO_CONTEXT *video_info);
void VO_deinit(void);
void VO_set_video_startpos(unsigned long x , unsigned long y);
void VO_set_video_mode_change(unsigned long in_w, unsigned long in_h , unsigned long in_mem_w , unsigned long out_w, unsigned long out_h , unsigned long out_mem_w , unsigned long buff_num);
void VO_set_video_parameter(unsigned long in_w, unsigned long in_h , unsigned long in_mem_w , unsigned long out_w, unsigned long out_h , unsigned long out_mem_w , unsigned long buff_num);
bool VO_write(unsigned char *y , unsigned char *c , double dts);
bool VO_getdts(double *dts);
bool VO_checkfull(void);
bool VO_read(void);
bool VO_skip(void);

//**************************************
//Private Function
//**************************************
static void VO_sub_rb_set_buffernum(unsigned long num);
static unsigned long VO_sub_rb_enqueue(void);
static unsigned long VO_sub_rb_dequeue(void);
static DWORD WINAPI DEC_VPU_decode_thread(PVOID pArg);
static DWORD WINAPI DEC_VPU_display_thread(PVOID pArg);
inline static bool VO_sub_rb_isempty(void);
inline static bool VO_sub_rb_isfull(void);
static bool DEC_VPU_sub_changefb(void);
static void VO_sub_rb_init(void);
static void VO_sub_rb_deinit(void);

//**************************************
//Ringbuffer setting
//**************************************
#define RB_MODE_SEMAPHORE 	0
#define RB_MODE_CRITICAL 	1

//**************************************
//Behavior setting
//**************************************

#define VO_BUFFER_FRAME			30*1	//assume 30fps with 1 sec

#define VO_FRMAE_SIZE_PIXEL		3/2
#define VO_SLEEP_WHILE_WAITING	1
#define VO_THREAD_SYNC_TIME		1
#define VO_NORMAL_POS_X			240
#define VO_NORMAL_POS_Y			50
#define VO_RINGBUFFERLOCK_MODE	0

//#define DMAC_TRANSFER
#ifdef DMAC_TRANSFER
#define IOCTL_DMAC_SET_PORT	1
#define IOCTL_DMAC_SET_PAGE	2
#define IOCTL_DMAC_SET_CONTROL	3
#define IOCTL_DMAC_INTERRUPT_ENABLE	4
#define IOCTL_DMAC_INTERRUPT_DISABLE	5
#define IOCTL_DMAC_INTERRUPT_CLEAR	6
#define IOCTL_DMAC_STOP	7
#define IOCTL_DMAC_IS_FINISHED		8
#define IOCTL_DMAC_SET_DMALVLR	9
#define IOCTL_DMAC_SET_DMASPR		10
#endif //DMAC_TRANSFER

//**************************************
//Structure
//**************************************
typedef struct vo_status{
	bool playing;
}VO_STATUS;

typedef struct vo_frameinfo{
	unsigned char		*address;
	unsigned char		*phy_address;
	double				dts;
}VO_FRAMEINFO;

typedef struct vo_rb{
#if VO_RINGBUFFERLOCK_MODE == RB_MODE_SEMAPHORE
	HANDLE        handle;
#elif VO_RINGBUFFERLOCK_MODE == RB_MODE_CRITICAL
	CRITICAL_SECTION        handle; 
#endif	
	unsigned long frame_rb_num;
	unsigned long frame_rb_front;
	unsigned long frame_rb_rear;
	VO_FRAMEINFO  frame_info[VO_BUFFER_FRAME];
}VO_RB;	
//**************************************
//Desplayint type
//**************************************
enum VO_TYPE{
	VO_OUTPUT_VEU = 0,
};
//**************************************
//IOCTL command
//**************************************

//**************************************
//Error handling
//**************************************

#endif //_LIBVO_H