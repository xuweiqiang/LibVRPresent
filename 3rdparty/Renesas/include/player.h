//********************************************************************
//Renesas player
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Component:	player
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************
#ifndef _PLAYER_H
#define _PLAYER_H

#define PLAYER_DEBUG		1
#define PLAYER_INTRODUCTION_MODE	0
#define DEF_PLAYER_VERSION	L"Renesas Player v0.55 2009/7/14 05:26¤U¤È"

//**************************************
// Include file dependencies
//**************************************
#include <mmsystem.h>
#include "libaac_dsp.h"
#include "lib264_hw.h"
#include "playerui.h"


//**************************************
// MACRO
//**************************************
#define PLAYER_GET_STS(x)			x.curr_playing_status
#define PLAYER_SET_STS(x,y)			x.curr_playing_status = y
#define PLAYER_GET_STSP(x)			x->curr_playing_status
#define PLAYER_SET_STSP(x,y)		x->curr_playing_status = y

//**************************************
//Playing TYPE
//**************************************
enum PLAYER_DECODE_TYPE{
	PLAYER_NONE=0,
	PLAYER_AAC,
	PLAYER_H264,
	PLAYER_MPEG4,
	PLAYER_REC,			//Renesas Engineerning Center format
	PLAYER_MAXIMUM_SUPPORT_NUM
};	

//**************************************
//Screen TYPE
//**************************************
enum PLAYER_SCREEN_TYPE{
	PLAYER_SCREEN_NORMAL = 0,
	PLAYER_SCREEN_FULL,
};
//**************************************
//Behavior setting
//**************************************
#define DEF_PLAYER_SYSTEM_PROCESS_TIMER	500
//Behavior relative with user
#define DEF_PLAYER_DISPLAY_VERSION	1
#define DEF_PLAYER_AUTOPLAY			1
#define DEF_PLAYER_AUTOREPEAT 		1
#define DEF_PLAYER_EXTENSION_NAME_LEN	3

//Remove it?
#define DEF_PALYER_SUPPORT_AAC		1
#define DEF_PALYER_SUPPORT_H264		0
#define DEF_PALYER_SUPPORT_H264AAC	0

#define DEF_PLAYER_ENABLE_FIXED_SUPPORT_FORMAT	0
#define DEF_PLAYER_FIXED_SUPPORT_TYPE			PLAYER_REC

//Define UI behavior
#define DEF_UI_MAXFILE		100
#define DEF_UI_MAX_PATH		100
#define UI_MAX_COMPONENT		20
#define UI_MAX_PLAYING_X		45
#define UI_MAX_PLAYING_Y		305
#define UI_STATUS_STRING_X		572
#define UI_STATUS_STRING_Y		298
#define UI_TIME_STRING_X		572
#define UI_TIME_STRING_Y		316
//**************************************
//Playing status
//**************************************
enum PLAYER_STATUS{
	PLAYER_STS_STOP = 0,
	PLAYER_STS_PLAY,
	PLAYER_STS_PAUSE,
};	

//**************************************
// Structure Definition --Sub
//**************************************
typedef struct vo_output{
	bool out_enable;
	unsigned long	start_x;
	unsigned long	start_y;
	unsigned long	display_w;
	unsigned long	display_w_mem;
	unsigned long	display_h;
	unsigned long	frames_display;
	unsigned long	frame_rate;
	bool			bFullScreen;
	bool (*change_mode)(void);
}VO_OUTPUT;

typedef struct player_notify_cb{
	void	(*StartNewFile)(void);
	void	(*ReachEOF)(void);
	void	(*EmergencyStatusUpdate)(void);		//Trial
}PLAYER_NOTIFY_CB;

typedef struct video_context{
	WCHAR				curr_play_filename[DEF_UI_MAXFILE];
	//Video output device
	HANDLE				hFB;
	VO_OUTPUT			fb_context;
	double				curr_time;
	//Decoder context	
	VPU_DECODER_CONTEXT vpu_decoder;
	PLAYER_NOTIFY_CB	player_cb;
	
	//Reserve for feature
	unsigned long		reverse_arg;
}VIDEO_CONTEXT;

typedef struct audio_context{
	WCHAR				curr_play_filename[DEF_UI_MAXFILE];
	//Audio output device
	WAVEFORMATEX		wave_context;
	
	double				curr_time;
	//Decoder context
	AAC_DECODER_CONTEXT	aac_decoder;
	PLAYER_NOTIFY_CB	player_cb;
	
	//Reserve for feature
	unsigned long		reverse_arg;
}AUDIO_CONTEXT;


//**************************************
// Structure Definition --Main
//**************************************
typedef struct player_context{
	AUDIO_CONTEXT	audio_info;
	VIDEO_CONTEXT	video_info;	
	PLAYER_DECODE_TYPE	curr_decode_type;
	PLAYER_STATUS		curr_playing_status;
	WCHAR				curr_play_filename[DEF_UI_MAXFILE];
	double				*curr_time;
}PLAYER_INFO;

//**************************************
// Structure Definition --Touch Screen
//**************************************
typedef struct touch_info{
	unsigned long	name_id;
	RECT			position;
}TOUCH_INFO;

//**************************************
// Structure Definition --Status
//**************************************
typedef struct status_info{
	unsigned long	name_id;
	WCHAR			string[100];
}STATUS_INFO;
//**************************************
//Error handling
//**************************************
enum PLAYER_ERROR{
	E_OK=0,
	E_UNSUPPORT,
	E_INVALID_FORMAT,
	E_INVALID_PARAMETER,
	E_NO_FILE,
	E_ERR_PLAY,
};
#endif //_PLAYER_H