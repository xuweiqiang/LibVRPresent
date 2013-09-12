//********************************************************************
//Renesas Engineerning Center format (R.E.C.)
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Decoder : R.E.C
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************
#ifndef _DECODER_REC_H
#define _DECODER_REC_H

//**************************************
// Include file dependencies
//**************************************
//N/A

//**************************************
// MACRO
//**************************************
#define DEC_REC_RTN(x)			x.result
#define DEC_REC_RTNP(x)			x->result

//**************************************
//Control Method
//**************************************
enum DECODER_REC_CONTROL{
	DEC_REC_STOP=0,
	DEC_REC_PLAY,	
	DEC_REC_PAUSE,
	DEC_REC_PROBE,
};	

//**************************************
// Structure Definition
//**************************************
typedef struct dec_rec_handle{
	HANDLE	hREC;
	HANDLE	hREC_SEND;
	DECODER_REC_CONTROL 	command;
	void*			*param;
	unsigned long	size;
	unsigned long	result;
}DEC_REC_HANDLE;

//**************************************
// Send Command
//**************************************
#define DEC_REC_SND_CMD(x)			{		SetEvent(x.hREC);											\
											WaitForSingleObject(x.hREC_SEND , INFINITE );				\
									}
#define DEC_REC_SND_CMDP(x)			{		SetEvent(x->hREC);											\
											WaitForSingleObject(x->hREC_SEND , INFINITE );				\
									}

									
//**************************************
//Behavior Setting
//**************************************
#define DECODER_REC_SET_DEFAULT_FRAME_W		320
#define DECODER_REC_SET_DEFAULT_FRAME_H		240
#define DECODER_REC_DEFAULT_FPS				12
#define DECODER_REC_SYNC_FRAME_NUM			5
#define DECODER_REC_SYNC_ADVANCE_NUM		12
#define DECODER_REC_SYNC_DELAYFPS			((double) (1 / (double)DECODER_REC_DEFAULT_FPS) * (double)DECODER_REC_SYNC_FRAME_NUM)
#define DECODER_REC_SYNC_ADVANCE_FRAME		((double) (1 / (double)DECODER_REC_DEFAULT_FPS) * (double)DECODER_REC_SYNC_ADVANCE_NUM)

#define DECODER_REC_SET_FIXED_SAMPLING_RATE	1	//Recommandation
#define DECODER_REC_OUTPUT_SAMPLING_RATE	44100
#define DECODER_REC_OUTPUT_AUDIO_CHANNEL	2
#define DECODER_REC_OUTPUT_BIT				16
#define DECODER_REC_SKIP_DATA_AS_OUTPUT		1	//If you got gap of audio from speaker, please try to disable this funtion first.
#define DECODER_REC_SKIP_DATA_PROTECTION	10

//**************************************
//Private Function
//**************************************
//VPU
static void DEC_REC_callback_video_empty(unsigned char *addr , unsigned long count);
static void DEC_REC_callback_video_output(unsigned char *y , unsigned char *c );
inline static void DEC_REC_sub_time_scale_processing(void);
inline static void DEC_REC_callback_sps(int x , int y, int mode);
static void DEC_REC_sub_start_thread(void);
static void DEC_REC_sub_vpu_deinitialize(void);
static void DEC_REC_sub_close_thread(void);
inline static void DEC_REC_sub_vpu_time_processing(void);

//AAC
static unsigned long DEC_REC_callback_audio_empty(char* addr,unsigned long length);
static unsigned long DEC_REC_callback_fill_aac_data(unsigned char *wpt,void* pID);
static void DEC_REC_sub_aacinit(AUDIO_CONTEXT *audio_info);
inline static void DEC_REC_sub_aac_time_processing(void);
static void DEC_AAC_sub_initialize(AUDIO_CONTEXT *audio_info);
static void DEC_REC_sub_aac_deinitialize(void);
static void DEC_AAC_sub_set_sampling_rate(AUDIO_CONTEXT *audio_info);
static void DEC_REC_sub_EOF(void);
//REC
static bool DEC_REC_sub_changefb(void);
static bool DEC_REC_sub_parsing_av_file(PLAYER_INFO * player_info);

static DWORD WINAPI DEC_REC_rtclose_thread(PVOID pArg);
//**************************************
//Public Function
//**************************************
DEC_REC_HANDLE* DEC_REC_init(PLAYER_INFO *player_info);
void DEC_REC_deinit(PLAYER_INFO *player_info);
unsigned long DEC_REC_probe(PLAYER_INFO *player_info);
unsigned long DEC_REC_seek(VIDEO_CONTEXT *video_info);
unsigned long DEC_REC_play(PLAYER_INFO *player_info);
unsigned long DEC_REC_stop(VIDEO_CONTEXT *video_info);

//**************************************
//Exit thread steps
//**************************************
enum REC_THREAD_EXITDECVPU{
	REC_KEEP_PLAY=0,
	REC_END_DECODE,
	REC_END_DISPLAY,
	REC_END_FINISH,	
};

//**************************************
//Error handling
//**************************************
enum DECREC_ERROR{
	DECREC_OK=0,
	DECREC_INVALID_FILE,
	DECREC_BAD_COMMAND,
	DECREC_FAIL,	
};

#endif //_DECODER_REC_H