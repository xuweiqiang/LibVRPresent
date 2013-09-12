/********************************************************************************/
/* SH-4A aacPlus Decode Middleware                                              */
/* R0M04A0SACPD2SAC Ver.2.0                                                     */
/* (C) copyright Coding Technologies (1998-2005)                                */
/* Licensed material of Coding Technologies                                     */
/* Copyright(C) 2008. Renesas Technology Corp., All Rights Reserved.            */
/* File Name : RSACPFD_ADL.h                                                    */
/********************************************************************************/
#ifndef RSACPFD_ADL_H
#define RSACPFD_ADL_H

typedef float SPEC;
/********************************************************************************/
/* Error return code                                                            */
/********************************************************************************/
#define	RSACPFD_ERR_DATA_EMPTY      -1
#define	RSACPFD_ERR_NO_SYNC         -2
#define RSACPFD_ERR_PARAM           -3
#define RSACPFD_ERR_NOT_INIT        -4

#define	RSACPFD_ERR_NO_ADIF         -10
#define	RSACPFD_ERR_FI_SYNC         -30

#define	RSACPFD_ERR_FI_ADIF         -50
#define	RSACPFD_ERR_ADTS_DATA       -60

#define	RSACPFD_ERR_SAMPLE_INDEX    -90
#define	RSACPFD_ERR_OBJ_TYPE        -95

#define	RSACPFD_ERR_AAC_LC          -200
#define	RSACPFD_ERR_NOT_SUPPORT     -201
#define RSACPFD_ERR_CRC_CHECK       -202

#define	RSACPFD_ERR_NOT_READY       -100
#define	RSACPFD_ERR_STREAM_DATA     -110
#define	RSACPFD_ERR_SFB_TBL         -111
#define	RSACPFD_ERR_HUFFMAN         -112

#define	RSACPFD_ERR_FI_CCE          -210
#define	RSACPFD_ERR_FI_LFE          -211
#define	RSACPFD_ERR_FI_GAIN         -212
#define	RSACPFD_ERR_FI_PREDI        -213

#define	RSACPFD_WNG_MLT_SCECPE      -250

#define	RSACPFD_ERR_SBR             -300

#define	RSACPFD_ERR_LIMITER         -400

/********************************************************************************/
/* normal return code                                                           */
/********************************************************************************/
#define	RSACPFD_RTN_GOOD            0
#define	RSACPFD_RTN_SCE             1
#define	RSACPFD_RTN_CPE             2
#define	RSACPFD_RTN_PCE_SCE         3
#define	RSACPFD_RTN_PCE_CPE         4
#define	RSACPFD_RTN_PCE             5
#define	RSACPFD_RTN_FI_SYNC         6
#define	RSACPFD_RTN_NO_AUDIO_DATA   7

/********************************************************************************/
/* API Structures                                                               */
/********************************************************************************/
/********************************************************************************/
/* Structure name : RSACPFD_AdifHeader                                          */
/* Description    : ADIF(Audio Data Interchange Format) Header Structure        */
/********************************************************************************/
typedef struct {
	unsigned char adif_id[4];
	unsigned char copyright_id_present;
	unsigned char copyright_id[9];
	unsigned char original_copy;
	unsigned char home;
	unsigned char bitstream_type;
	unsigned long bitrate;
	unsigned char num_program_config_elements;
	unsigned char ID;
} RSACPFD_AdifHeader;

/********************************************************************************/
/* Structure name : RSACPFD_AdtsHeader                                          */
/* Description    : ADTS(Audio Data Transport Stream) Header Structure          */
/********************************************************************************/
typedef struct {
	unsigned char  ID;
	unsigned char  layer;
	unsigned char  protection_absent;
	unsigned char  profile;
	unsigned char  sampling_frequency_index;
	unsigned char  private_bit;
	unsigned char  channel_configuration;
	unsigned char  original_copy;
	unsigned char  home;
	unsigned char  copyright_identification_bit;
	unsigned char  copyright_identification_start;
	unsigned short frame_length;
	unsigned short adts_buffer_fullness;
	unsigned char  number_of_raw_data_blocks_in_frame;
} RSACPFD_AdtsHeader;

/********************************************************************************/
/* Structure name : RSACPFD_PCE                                                 */
/* Description    : PCE(Program Config Element) Header Structure                */
/********************************************************************************/
typedef struct {
	unsigned char  element_instance_tag;
	unsigned char  profile;
	unsigned char  sampling_frequency_index;
	unsigned char  num_front_channel_elements;
	unsigned char  num_side_channel_elements;
	unsigned char  num_back_channel_elements;
	unsigned char  num_lfe_channel_elements;
	unsigned char  num_assoc_data_elements;
	unsigned char  num_valid_cc_elements;
	unsigned char  mono_mixdown_present;
	unsigned char  mono_mixdown_element_number;
	unsigned char  stereo_mixdown_present;
	unsigned char  stereo_mixdown_element_number;
	unsigned char  matrix_mixdown_idx_present;
	unsigned char  matrix_mixdown_idx;
	unsigned char  pseudo_surround_enable;
	unsigned char  front_element_is_cpe[16];
	unsigned char  front_element_tag_select[16];
	unsigned char  side_element_is_cpe[16];
	unsigned char  side_element_tag_select[16];
	unsigned char  back_element_is_cpe[16];
	unsigned char  back_element_tag_select[16];
	unsigned char  lfe_element_tag_select[4];
	unsigned char  assoc_data_element_tag_select[8];
	unsigned char  cc_element_is_ind_sw[16];
	unsigned char  valid_cc_element_tag_select[16];
	unsigned char  comment_field_bytes;
	unsigned char  comment_field_data[256];
} RSACPFD_PCE;

struct RSACPFD_ChannelData1{
/********************************************************************************/
/* ics_info                                                                     */
/********************************************************************************/
	unsigned char  ics_reserved_bit;                                 /* 1 bslbf */
	unsigned char  window_sequence;                                 /* 2 uimsbf */
	unsigned char  window_shape;                                    /* 1 uimsbf */
	unsigned char  max_sfb;                                    /* 4 or 6 uimsbf */
	unsigned char  scale_factor_grouping;                           /* 7 uimsbf */
/********************************************************************************/
/* help_element                                                                 */
/********************************************************************************/
	unsigned char  num_windows;                                 /* help element */
	unsigned char  num_window_groups;                           /* help element */
	unsigned char  window_group_length[8];                      /* help element */
	unsigned char  num_swb;                                     /* help element */
	unsigned short sect_sfb_offset[128];
	unsigned short swb_offset[52];                              /* help element */
/********************************************************************************/
/* section_data                                                                 */
/********************************************************************************/
	unsigned char  sfb_cb[120];                                 /* help element */
/********************************************************************************/
/* scale_factor_data                                                            */
/********************************************************************************/
	long dpcm_is_position[120];                                 /* help element */
/********************************************************************************/
/* tns_data                                                                     */
/********************************************************************************/
	unsigned char  n_filt[8];                                    /* 1..2 uimsbf */
	unsigned char  coef_res[8];                                     /* 1 uimsbf */
	unsigned char  length[24];                                  /* {4;6} uimsbf */
	unsigned char  order[24];                                   /* {3;5} uimsbf */
	unsigned char  direction[24];                                   /* 1 uimsbf */
	unsigned char  coef_compress[24];                               /* 1 uimsbf */
	unsigned char  coef[24][31];                                 /* 2..4 uimsbf */
/********************************************************************************/
/* help_element                                                                 */
/********************************************************************************/
	unsigned char  pre_window_shape;                            /* help element */
	unsigned char  preWindowSequence;
/********************************************************************************/
/* pns_data                                                                     */
/********************************************************************************/
	int            m_NoiseEnergy[8][64];
	int            m_CurrentEnergy;
	int            m_PnsActive;
};

struct RSACPFD_ChannelData2{
	unsigned char  chno;
/********************************************************************************/
/* individual_channel_stream                                                    */
/********************************************************************************/
	unsigned char  global_gain;                                     /* 8 uimsbf */
	unsigned char  pulse_data_present;                              /* 1 uismbf */
	unsigned char  tns_data_present;                                /* 1 uimsbf */
	unsigned char  gain_control_data_present;                       /* 1 uimsbf */
/********************************************************************************/
/* section_data                                                                 */
/********************************************************************************/
	unsigned char  sect_cb[120];                                    /* 4 uimsbf */
	unsigned char  sect_start[120];                             /* help element */
	unsigned char  sect_end[120];                               /* help element */
	unsigned char  num_sec[8];                                  /* help element */
/********************************************************************************/
/* scale_factor_data                                                            */
/********************************************************************************/
	long sf[120];                                               /* help element */
/********************************************************************************/
/* pulse_data                                                                   */
/********************************************************************************/
	unsigned char  number_pulse;                                    /* 2 uimsbf */
	unsigned char  pulse_start_sfb;                                 /* 6 uimsbf */
	unsigned char  pulse_offset[4];                                 /* 5 uimsbf */
	unsigned char  pulse_amp[4];                                    /* 4 uimsbf */
};

/********************************************************************************/
/* Structure name : RSACPFD_AAC                                                 */
/* Description    : AAC Decode Work Table                                       */
/********************************************************************************/
typedef struct {
	unsigned char  *pBsBufIdx;
	unsigned long  BsBufIdx;
	unsigned char  *preBsBufIdx;
	unsigned long  Bs4Byte;
	unsigned char  *BsBuf;
	unsigned long  BsBufSize;
	int            nNoUseBit;
	int            SkipFlag;
	short          *PcmBuf;
	short          ChannelNumber;
	short          prevChannelNumber;
	int            PcmLen;
	RSACPFD_PCE    *pcebuf;
	char           AudioObjectType;                                 /* 5 bslbf */
	char           sampling_frequency_index;                       /* 4 uimsbf */
	unsigned char  id;
	unsigned char  common_window;                                  /* 1 uimsbf */
	unsigned char  ms_mask_present;                                /* 2 uimsbf */
	struct RSACPFD_ChannelData1 *pCD1;
	struct RSACPFD_ChannelData2 *pCD2;
	struct RSACPFD_ChannelData1 *pCDCW1;
	struct RSACPFD_ChannelData2 *pCDCW2;
	SPEC           *spec;                                      /* help element */
	int            sequence_number;
	int            DErrorCode;
	unsigned char  ms_used[120];                                   /* 1 uimsbf */
	int            hcod[1024];                                 /* help element */
	int            x_quant[1024];                              /* help element */
	long           is_position[120];
	SPEC           overLapBuffer[1024*2];
	SPEC           *l_spec;
	SPEC           *r_spec;
	float          pcm[4096];
	struct RSACPFD_ChannelData1 CD1[2];
	struct RSACPFD_ChannelData2 CD2[2];
	char           ID;
	char           header_done;
	char           header_type;
	unsigned long  buf_len;
	int            (*UserFunc)();
	int            data_empty_flag;
	unsigned int   nByteFrame;
        unsigned short frame_length;
	char           RSACPFD_flag;
	int            bDownSample;
	int            channelMode;
	unsigned char  useHqSbr;
	int            MonoOut2ch;
	int            ForceSbrOff;
	int            Seamless;
	int            MonoOut2ch_opt;
	int            bDoUpsampling;
	int	       bDoUpsampling_mode;
	int            nOutputFormat;
	long           crc[256];
	unsigned short FileCRC;
	unsigned short CalcCRC;
	int            BRDetect;
	int            nBitrate;
	unsigned int   m_LastBufferFullness ;
	unsigned int   m_Index ;
	unsigned int   m_validEntries ;
	float          m_Value [16] ;
	int            m_WantLimiter;
	int            ErrContinue;
	int            CrcErr;
	unsigned short refValue;
	unsigned short crcReg;
	unsigned char  CrcFlag;
	int            SBRCrcErr;
	int            initialized;
	short          m_RandomSeed[8][64];
	short          m_CurrentSeed;
	short          m_frameNumber;
	long 	       FrameByteCount;
	int            fpscr_save;
	float          RSACPFD_WorkBufferCore[2048];
	long           RSACPFD_sbrDecSettings[19];
	long           *RSACPFD_m_hLimiter;
	long           streamSBR[521];         
	long           *RSACPFD_sbrDecoderInfo;
	long           RSACPFD_crcReg[12];
	int            RSACPFD_SBRCrcErr;
	char           *RSACPFD_LimStaticStartAdd;
	char           *RSACPFD_LimStaticCurrentAdd;
	char           *RSACPFD_StaticStartAdd;
	char           *RSACPFD_StaticCurrentAdd;
	unsigned short adts_decodesize;
	unsigned char  frameLengthCheck;
	unsigned char  remainRDB;
	short          PrevChannelNumber_lim;
	int            streamSBR_preNrElements;        
	void           *UserObjID;
} RSACPFD_AAC;

/********************************************************************************/
/* API Functions                                                                */
/********************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/********************************************************************************/
/* Name        : RSACPFD_Open                                                   */
/* Function    : Initializes the aacPlus decoding middleware                    */
/* Parameter   : RSACPFD_AAC *     (Pointer to the RSACPFD_AdifHeader)          */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
typedef	int (*RSACPFD_UserFunc)( unsigned char *, void *ID );
extern int RSACPFD_Open    ( RSACPFD_AAC *aac,  
                          unsigned char *buf_adr, 
                          RSACPFD_UserFunc UserFunc );
extern int RSACPFD_Open_ID ( RSACPFD_AAC *aac,  
                          unsigned char *buf_adr, 
                          RSACPFD_UserFunc UserFunc,
			  void *ID );

/********************************************************************************/
/* Name        : RSACPFD_GetAdifHeader                                          */
/* Function    : Acquires the header of the bit stream in the ADIF format       */
/* Parameter   : RSACPFD_AAC *        (Pointer to the RSACPFD_AAC)              */
/*               RSACPFD_AdifHeader * (Pointer to the RSACPFD_AdifHeader)       */
/*               unsigned char *      (Starting address of in input buffer)     */
/*               int *                (Number of effective bytes for reading)   */
/*               int                  (Specifies MPEG mode  0:MPEG4, 1:MPEG2)   */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_GetAdifHeader ( RSACPFD_AAC *aac,
                                   RSACPFD_AdifHeader *header,
                                   unsigned char *bs,
                                   int *bnum,
                                   int mpeg );

/********************************************************************************/
/* Name        : RSACPFD_GetAdtsHeader                                          */
/* Function    : Acquires the header of the bit stream in the ADTS format       */
/* Parameter   : RSACPFD_AAC *        (Pointer to the RSACPFD_AdifHeader)       */
/*               RSACPFD_AdtsHeader * (Pointer to the RSACPFD_AdtsHeader)       */
/*               unsigned char *      (Starting address of in input buffer)     */
/*               int *                (Number of effective bytes for reading)   */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_GetAdtsHeader ( RSACPFD_AAC *aac,
                                   RSACPFD_AdtsHeader *header,
                                   unsigned char *bs,
                                   int *bnum );

/********************************************************************************/
/* Name        : RSACPFD_Decode                                                 */
/* Function    : Decodes one block of the bit stream                            */
/* Parameter   : RSACPFD_AAC *   (Pointer to the RSACPFD_AAC)                   */
/*               unsigned char * (Starting address of in input buffer)          */
/*               int *           (Number of effective bytes for reading)        */
/*               void *          (Starting address of the output buffer)        */
/*               int *           (Number of the short data items)               */
/* Return Code : 0 - 6 (Normal)                                                 */
/*               minus (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_Decode( RSACPFD_AAC *aac,
                           unsigned char *bs,
                           int *bnum,
                           short *pcm,
                           int *pnum );

/********************************************************************************/
/* Name        : RSACPFD_Skip                                                   */
/* Function    : Skips one block of the bit stream                              */
/* Parameter   : RSACPFD_AAC *   (Pointer to the RSACPFD_AAC)                   */
/*               unsigned char * (Starting address of in input buffer)          */
/*               int *           (Number of effective bytes for reading)        */
/* Return Code : 0 - 6 (Normal)                                                 */
/*               minus (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_Skip( RSACPFD_AAC *aac,
                         unsigned char *bs,
                         int *bnum );

/********************************************************************************/
/* Name        : RSACPFD_SetPCEArea                                             */
/* Function    : Allocates an area to acquire the PCE information               */
/* Parameter   : RSACPFD_AAC * (Pointer to the RSACPFD_AAC)                     */
/*               RSACPFD_PCE * (Pointer to the RSACPFD_PCE)                     */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_SetPCEArea( RSACPFD_AAC *aac,
                               RSACPFD_PCE *pce ); 

/********************************************************************************/
/* Name        : RSACPFD_SetFormat                                              */
/* Function    : Specifies information for RawDataStream decoding               */
/* Parameter   : RSACPFD_AAC * (Pointer to the RSACPFD_AAC)                     */
/*               int           (Number of channels)                             */
/*               int           (Sampling index number)                          */
/*               int           (Profile specification,Fixed 1)                  */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_SetFormat( RSACPFD_AAC *aac,
                              int chNum,
                              int sampleRate,
                              int objType );

/********************************************************************************/
/* Name        : RSACPFD_get_version                                            */
/* Function    : Acquires the version of the aacPlus decoding middleware        */
/* Parameter   : Nothing                                                        */
/* Return code : Version Information                                            */
/********************************************************************************/
extern int RSACPFD_get_version(void);

/********************************************************************************/
/* Name        : RSACPFD_DecodeStatus                                           */
/* Function    : Get the status of decoding processing                          */
/* Parameter   : RSACPFD_AAC* (Pointer to the HAACD_AAC)                        */
/*             : int*      (Status of decode processing)                        */
/* Return Code : 0         (Normal)                                             */
/*               not 0     (Error)                                              */
/********************************************************************************/
extern int RSACPFD_DecodeStatus ( RSACPFD_AAC *aac, int *decodeStatus );

/********************************************************************************/
/* Name        : RSACPFD_UseDSSBR                                               */
/* Function    : Sets the Down Sampled SBR Mode                                 */
/* Parameter   : RSACPFD_AAC * (Pointer to the RSACPFD_AAC)                     */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_UseDSSBR ( RSACPFD_AAC *aac );

/********************************************************************************/
/* Name        : RSACPFD_UseForceSBROff                                         */
/* Function    : Set the ForceSBRoff                                            */
/* Parameter   : RSACPFD_AAC* (Pointer to the RSACPFD_AAC)                      */
/* Return Code : 0         (Normal)                                             */
/*               -3        (Error)                                              */
/********************************************************************************/
extern int RSACPFD_UseForceSBROff( RSACPFD_AAC *aac );

/********************************************************************************/
/* Name        : RSACPFD_MonoOut2ch                                             */
/* Function    : Convert Mono to Mono+Blank                                     */
/* Parameter   : RSACPFD_AAC * (Pointer to the RSACPFD_AAC)                     */
/*               int           (Parameter of Rch Output)                        */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_MonoOut2ch ( RSACPFD_AAC *aac, int opt );

/********************************************************************************/
/* Name        : RSACPFD_UseUpSampling                                          */
/* Function    : Sets the Up Sampling Mode                                      */
/* Parameter   : RSACPFD_AAC * (Pointer to the RSACPFD_AAC)                     */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_UseUpSampling ( RSACPFD_AAC *aac );

/********************************************************************************/
/* Name        : RSACPFD_MonoOut2ch                                             */
/* Function    : Convert Mono to Mono+Blank                                     */
/* Parameter   : RSACPFD_AAC * (Pointer to the RSACPFD_AAC)                     */
/*               int           (Parameter of Rch Output)                        */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_MonoOut2ch ( RSACPFD_AAC *aac, int opt );

/********************************************************************************/
/* Name        : RSACPFD_UseUpSampling                                          */
/* Function    : Sets the Up Sampling Mode                                      */
/* Parameter   : RSACPFD_AAC * (Pointer to the RSACPFD_AAC)                     */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_UseUpSampling ( RSACPFD_AAC *aac );

/********************************************************************************/
/* Name        : RSACPFD_UseBRDetect                                            */
/* Function    : The bit rate reference is enabled                              */
/* Parameter   : RSACPFD_AAC * (Pointer to the RSACPFD_AAC)                     */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_UseBRDetect ( RSACPFD_AAC *aac );

/********************************************************************************/
/* Name        : RSACPFD_UseLimiter                                             */
/* Function    : Sets the Limiter Mode                                          */
/* Parameter   : RSACPFD_AAC * (Pointer to the RSACPFD_AAC)                     */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_UseLimiter ( RSACPFD_AAC *aac );

/********************************************************************************/
/* Name        : RSACPFD_UseErrContinuation                                     */
/* Function    : Sets the Decode continuance mode when crc error is detected    */
/* Parameter   : RSACPFD_AAC * (Pointer to the RSACPFD_AAC)                     */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_UseErrContinuation ( RSACPFD_AAC *aac );

/********************************************************************************/
/* Name        : RSACPFD_AllocWorkArea                                          */
/* Function    : Allocate Work area                                             */
/* Parameter   : char * (Pointer to the allocated Work area)                    */
/*               int    (Size of allocated Work area)                           */
/* Return Code : 0     (Normal)                                                 */
/*               not 0 (Error)                                                  */
/********************************************************************************/
extern int RSACPFD_AllocWorkArea( RSACPFD_AAC *aac, char *pwork, int size );

#ifdef __cplusplus
}
#endif

#endif
/********************************************************************************/
/* Copyright(C) 2001-2008. Renesas Technology Corp., All Rights Reserved.       */
/********************************************************************************/
