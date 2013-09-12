
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RTKTUNERDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RTKTUNERDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef RTKTUNERDLL_EXPORTS
#define RTKTUNERDLL_API __declspec(dllexport)
#else
#define RTKTUNERDLL_API __declspec(dllimport)
#endif

enum RTK_TunerStatus
{
    RTK_TUNER_FAIL = 0,
    RTK_TUNER_SUCCESS,
    RTK_TUNER_LOCKED,
    RTK_TUNER_UNLOCKED,
	RTK_TUNER_NOSIGNAL
};

#define	TUNER_FREQ_MAIN		473143L	// 473.143Mhz
#define	TUNER_FREQ_BW		1000L	// 1 MHZ
#define	TUNER_CH_START		13
#define	TUNER_FREQ_CHNUM	50
#define	TUNER_MAX_SNR		100

#define	TUNER_DATA_SIZE		188
#define	TUNER_QUEUE_NUM		20
#define	TUNER_PACKET_SIZE	(TUNER_DATA_SIZE*TUNER_QUEUE_NUM)

#define	TUNER_UUID_LEN		16

extern RTKTUNERDLL_API int nRTKTunerDll;

RTKTUNERDLL_API HANDLE			RTK_InitTuner(BYTE *PCKey);
RTKTUNERDLL_API void			RTK_UnInitTuner(HANDLE hTunerDevice);
RTKTUNERDLL_API RTK_TunerStatus RTK_SetTunerFrequency(HANDLE hTunerDevice, ULONG Freq, BYTE BW);
RTKTUNERDLL_API RTK_TunerStatus RTK_StartStopStream(HANDLE hTunerDevice, BOOL bStart);
RTKTUNERDLL_API RTK_TunerStatus RTK_ReadTSStreamData(HANDLE hTunerDevice, BYTE *pBuffer, int &nBufLength, BOOL bGetDecrypt);
RTKTUNERDLL_API RTK_TunerStatus RTK_GetSignalLevel(HANDLE hTunerDevice, BYTE *SNR_A);
RTKTUNERDLL_API RTK_TunerStatus RTK_GetSignalLockCheck(HANDLE hTunerDevice);
RTKTUNERDLL_API RTK_TunerStatus RTK_GetTunerCN_BER(HANDLE hTunerDevice, LONG *CN, LONG *BER);
RTKTUNERDLL_API RTK_TunerStatus RTK_GetDeviceUniqueID (HANDLE hTunerDevice, BYTE* pDevUniqueID, int nIDLen);
RTKTUNERDLL_API RTK_TunerStatus RTK_DecryptTSStreamData (HANDLE hTunerDevice, BYTE* pInputData,int nInputFrameNum, BYTE* pOutputData);

RTKTUNERDLL_API TCHAR			*RTK_TunerGetVersion(void);   // get tuner dll version string
RTKTUNERDLL_API TCHAR			*RTK_TunerGetLogName(void);   // get tuner log file name setting in ini file

RTKTUNERDLL_API int init_buffer(char **yuvbuf, char **rgbbuf);
RTKTUNERDLL_API int convert_yuv420_rgb565(char *yuvbuf, char *rgbbuf, unsigned int src_width, unsigned int src_height, unsigned int dst_width, unsigned int dst_height);
RTKTUNERDLL_API int change_yuv420_rgb565_size(unsigned int src_width, unsigned int src_height, unsigned int dst_width, unsigned int dst_height);