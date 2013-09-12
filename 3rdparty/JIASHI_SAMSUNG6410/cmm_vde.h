// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the CMM_VDE_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// CMM_VDE_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef CMM_VDE_EXPORTS
#define CMM_VDE_API __declspec(dllexport)
#else
#define CMM_VDE_API __declspec(dllimport)
#endif
#include <windows.h>
// typedef enum
// {
// 	POST_SRC_RGB16 = 0,
// 		POST_SRC_RGB24,
// 		POST_SRC_YUV420,
// 		POST_SRC_YUV422_YCBYCR,
// 		POST_SRC_YUV422_CBYCRY,
// 		POST_SRC_YUV422_YCRYCB,
// 		POST_SRC_YUV422_CRYCBY
// } POST_SRC_TYPE;


//alpha 0--15
//extern "C"

CMM_VDE_API void set_video_alpha(UINT alpha);
CMM_VDE_API UINT8* init_cmm();//可看单例模式
CMM_VDE_API HANDLE init_windows0();//可看单例模式

//src_type 参考 POST_SRC_TYPE
CMM_VDE_API int StartDisplay(int dx,int dy,int dwidth,int dheight,UINT8* syuvbuff,UINT srcyuv_type,int srcwidth,int srcheight);
CMM_VDE_API void toggle_visable_lay(BOOL visable);
CMM_VDE_API void uinit_cmm();
CMM_VDE_API BOOL uinit_windows0();

CMM_VDE_API int clear_video_buffer();

