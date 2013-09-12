//********************************************************************
//Renesas H.264/MPEG4 middleware
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Decoder : H264 / MPEG4 (VPU)
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************
#ifndef _DECODER_VPU_H
#define _DECODER_VPU_H

//**************************************
// Include file dependencies
//**************************************
//N/A

//**************************************
// MACRO
//**************************************
#define DEC_VPU_RTN(x)			x.result
#define DEC_VPU_RTNP(x)			x->result

//**************************************
//Control Method
//**************************************
enum DECODER_VPU_CONTROL{
	DEC_VPU_STOP=0,
	DEC_VPU_PLAY,	
	DEC_VPU_PAUSE,
	DEC_VPU_PROBE,
};	

//**************************************
// Structure Definition
//**************************************
typedef struct dec_vpu_handle{
	HANDLE	hVPU;
	HANDLE	hVPU_SEND;
	DECODER_VPU_CONTROL 	command;
	void*			*param;
	unsigned long	size;
	unsigned long	result;
}DEC_VPU_HANDLE;

//**************************************
// Send Command
//**************************************
#define DEC_VPU_SND_CMD(x)			{		SetEvent(x.hVPU);											\
											WaitForSingleObject(x.hVPU_SEND , INFINITE );				\
									}
#define DEC_VPU_SND_CMDP(x)			{		SetEvent(x->hVPU);											\
											WaitForSingleObject(x->hVPU_SEND , INFINITE );				\
									}

									
//**************************************
//Behavior Setting
//**************************************
#define DECODER_VPU_SET_DEFAULT_FRAME_W		320
#define DECODER_VPU_SET_DEFAULT_FRAME_H		240
#define DECODER_VPU_DEFAULT_FPS				24

//**************************************
//Private Function
//**************************************
static void DEC_VPU_callback_video_empty(unsigned char *addr , unsigned long count);
static void DEC_VPU_callback_video_output(unsigned char *y , unsigned char *c );
inline static void DEC_VPU_sub_time_scale_processing(void);
inline static void DEC_VPU_callback_sps(int x , int y, int mode);
static void DEC_VPU_sub_start_thread(void);
static void DEC_VPU_sub_deinitialize(void);
static void DEC_VPU_sub_close_thread(void);
static DWORD WINAPI DEC_VPU_rtclose_thread(PVOID pArg);

//**************************************
//Public Function
//**************************************
DEC_VPU_HANDLE* DEC_VPU_init(VIDEO_CONTEXT *video_info);
void DEC_VPU_deinit(VIDEO_CONTEXT *video_info);
unsigned long DEC_VPU_probe(VIDEO_CONTEXT *video_info);
unsigned long DEC_VPU_seek(VIDEO_CONTEXT *video_info);
unsigned long DEC_VPU_play(VIDEO_CONTEXT *video_info);
unsigned long DEC_VPU_stop(VIDEO_CONTEXT *video_info);

//**************************************
//Exit thread steps
//**************************************
enum VPU_THREAD_EXITDECVPU{
	VPU_KEEP_PLAY=0,
	VPU_END_DECODE,
	VPU_END_DISPLAY,
	VPU_END_FINISH,	
};
//**************************************
//Error handling
//**************************************
enum DECVPU_ERROR{
	DECVPU_OK=0,
	DECVPU_INVALID_FILE,
	DECVPU_BAD_COMMAND,
	DECVPU_FAIL,	
};

#endif //_DECODER_VPU_H