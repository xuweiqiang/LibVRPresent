//********************************************************************
//Renesas AAC/AAC+ middleware
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Component:	AAC/AAC+
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************
#ifndef _LIBAACD_H
#define _LIBAACD_H

//**************************************
// Include file dependencies
//**************************************
#include "RSACPFD_ADL.h"
#include "player.h"

//**************************************
//Public Function
//**************************************
void AACD_init(void);
void AACD_deinit(void);
void AACD_ioctl(unsigned long command,void* param,unsigned long param_size,unsigned long* result);

//**************************************
//Private Function
//**************************************
static int AACD_sub_callback(unsigned char *wpt,void* pID);
static unsigned long VPUD_sub_getpacket264(void);
static unsigned long VPUD_sub_getpacketmpeg4(void);
inline static unsigned long ACCD_sub_decode_result(int err);

//**************************************
//Structure
//**************************************
typedef struct aac_header{
	RSACPFD_AdifHeader		adif_info;
	RSACPFD_AdtsHeader		adts_info;
}AAC_HEADER;

enum AAC_TYPE{
	AACD_DECODE_ADIF,
	AACD_DECODE_ADTS,
	AACD_DECODE_RAW,
	AACD_DECODE_NONE,
};

typedef struct aac_context{
	char			*working_memory;
	unsigned char	*stream_buffer;
	int 			(*callback)(unsigned char *wpt,void* pID);	//callback function
	void			(*fill_buffer)(unsigned char *addr,unsigned long *size);
	RSACPFD_AAC		decoder_info;
	RSACPFD_PCE		pce_info;
	AAC_HEADER		aac_header;
	AAC_TYPE		type;
}AAC_DECODER_CONTEXT;

typedef struct aac_decode_1frame{
	short			*address;
	int				size;
	int				result;
}AAC_DECODER_1FRAME;

//**************************************
//Behavior setting
//**************************************
#define DEF_AACD_WORKING_MEMORY			131000
#define DEF_AACD_STREMING_BUFFER_SIZE	512*1024
#define DEF_AACD_SKIP_ERROR				1

//**************************************
//Sampling Frequency
//**************************************
enum FREQ_IDX{
	AACD_FREQ_96000=0,
	AACD_FREQ_88200,
	AACD_FREQ_64000,
	AACD_FREQ_48000,
	AACD_FREQ_44100,
	AACD_FREQ_32000,
	AACD_FREQ_24000,
	AACD_FREQ_22050,
	AACD_FREQ_16000,
	AACD_FREQ_12000,
	AACD_FREQ_11025,
	AACD_FREQ_8000,
	AACD_FREQ_C,
	AACD_FREQ_D,
	AACD_FREQ_E,
	AACD_FREQ_F
};

//**************************************
//IOCTL command
//**************************************
enum AACD_IOCTL{
	AACD_Init,
	AACD_SetFormat,
	AACD_Decode,
	AACD_SetCallback,
	AACD_GetStatus,
	AACD_Skip,
	AACD_ProbeADIF,
	AACD_ProbeADTS,
	AACD_DecodeOneFrame_ADTS,
	AACD_DecodeOneFrame_ADIF,
	AACD_GetHeader,
	AACD_GetPCE,
	AACD_RetriveHeader_ADIF
};

//**************************************
//Error handling
//**************************************
enum AACD_ERROR{
	AACD_OK=0,
	AACD_DECODE_AUDIO,
	AACD_DECODE_DATA,
	AACD_DECODE_ERROR,
	AACD_DECODE_END,
	AACD_INVALID_ADDRESS,
};
#endif //_LIBAACD_H