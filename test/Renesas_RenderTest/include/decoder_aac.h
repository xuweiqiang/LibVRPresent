//********************************************************************
//Renesas AAC/AAC+ middleware
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Decoder : AAC
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************
#ifndef _DECODER_AAC_H
#define _DECODER_AAC_H

//**************************************
// Include file dependencies
//**************************************
//N/A

//**************************************
// MACRO
//**************************************
#define DEC_AAC_RTN(x)			x.result
#define DEC_AAC_RTNP(x)			x->result

//**************************************
//Control Method
//**************************************
enum DECODER_AAC_CONTROL{
	DEC_AAC_STOP=0,
	DEC_AAC_PLAY,	
	DEC_AAC_PAUSE,
	DEC_AAC_PROBE,
};	

//**************************************
// Structure Definition
//**************************************
typedef struct dec_aac_handle{
	HANDLE	hAAC;
	HANDLE	hAAC_SEND;
	DECODER_AAC_CONTROL 	command;
	void*			*param;
	unsigned long	size;
	unsigned long	result;
}DEC_AAC_HANDLE;

//**************************************
// Send Command
//**************************************
#define DEC_AAC_SND_CMD(x)			{		SetEvent(x.hAAC);											\
											WaitForSingleObject(x.hAAC_SEND , INFINITE );				\
									}
#define DEC_AAC_SND_CMDP(x)			{		SetEvent(x->hAAC);											\
											WaitForSingleObject(x->hAAC_SEND , INFINITE );				\
									}

									
//**************************************
//Behavior Setting
//**************************************
//DO NOT CHANGE THIS IF YOU DON'T KNOW THE PURPOSE OF SETTING
#define DECODER_AAC_SET_FIXED_SAMPLING_RATE	1	//Recommandation
#define DECODER_AAC_OUTPUT_SAMPLING_RATE	44100
#define DECODER_AAC_OUTPUT_AUDIO_CHANNEL	2
#define DECODER_AAC_OUTPUT_BIT				16
#define DECODER_AAC_SKIP_DATA_AS_OUTPUT		1	//If you got gap of audio from speaker, please try to disable this funtion first.
#define DECODER_AAC_SKIP_DATA_PROTECTION	10

//**************************************
//Private Function
//**************************************
static unsigned long DEC_AAC_callback_fill_aac_data(unsigned char *wpt,void* pID);
static unsigned long DEC_AAC_callback_audio_empty(char* addr,unsigned long length);
static void DEC_AAC_sub_EOF(void);
static void DEC_AAC_sub_initialize(AUDIO_CONTEXT *audio_info);
static void DEC_AAC_sub_set_sampling_rate(AUDIO_CONTEXT *audio_info);
static void DEC_AAC_sub_deinitialize(void);
inline static void DEC_AAC_sub_time_scale_processing(void);

//**************************************
//Public Function
//**************************************
DEC_AAC_HANDLE* DEC_AAC_init(AUDIO_CONTEXT *audio_info);
void DEC_AAC_deinit(AUDIO_CONTEXT *audio_info);
unsigned long DEC_AAC_probe(AUDIO_CONTEXT *audio_info);
unsigned long DEC_AAC_seek(AUDIO_CONTEXT *audio_info);
unsigned long DEC_AAC_play(AUDIO_CONTEXT *audio_info);
unsigned long DEC_AAC_stop(AUDIO_CONTEXT *audio_info);



//**************************************
//Error handling
//**************************************
enum DECAAC_ERROR{
	DECAAC_OK=0,
	DECAAC_INVALID_FILE,
	DECAAC_BAD_COMMAND,
	DECAAC_FAIL,
};

#endif //_DECODER_AAC_H