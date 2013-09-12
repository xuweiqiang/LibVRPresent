//********************************************************************
//Renesas audio output unit
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Component:	AACplus	9
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************
#ifndef _LIBAO_H
#define _LIBAO_H

#include "player.h"
//**************************************
//Public Function
//**************************************
void AO_init(AUDIO_CONTEXT *audio_info);
void AO_deinit(void);
void AO_ioctl(unsigned long command,void* param,unsigned long size,unsigned long* result);

//**************************************
//Private Function
//**************************************
static void AO_sub_start(void);
static void CALLBACK AO_callback_func(HWAVEOUT hWaveOut,UINT uMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);

//**************************************
//Behavior setting
//**************************************
#define DEF_MAX_WAVE_RING_BUFFER_SIZE	128*1024
#define DEF_MAX_WAVE_RING_BUFFER_NUM	1024
#define DEF_MIN_WAVE_PREPARE_DELAY		100

//**************************************
//Structure
//**************************************
typedef struct ao_status
{
	bool		playing;
}AO_STATUS;

//**************************************
//IOCTL command
//**************************************
enum AO_IOCTL{
	//Reserver it for init module in system level
	//AO_INIT,
	//AO_DEINIG,
	AO_START,
	AO_STOP,
	AO_RESUME,
	AO_SET_CALLBACK,
	AO_SET_WAVE_RINGSIZE,
	AO_SET_WAVE_FORMAT,
	AO_SET_WAVE_FORMAT_NULL,
	AO_GET_TIME_STATUS,
};

//**************************************
//Error handling
//**************************************
enum AO_ERROR{
	AO_OK=0,
	A0_INVALID_PARAMETER,
	AO_ERROR_WAVEFORM,
	A0_CANNOT_ACQUIRE_MEMORY,
	A0_INVALID_RINGSIZE,
};
#endif //_LIBAO_H