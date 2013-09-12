/********************************************************************

	2009-07-20	huxing
	Post Driver YUV to RGB Input  Param 
	
********************************************************************/
#ifndef _SPITOUART_H_
#define _SPITOUART_H_

#if __cplusplus
	extern "C"
	{
#endif
#include <winioctl.h>

typedef struct _POSTINFO
{
	UINT	nOrgSrcWidth;
	UINT	nOrgSrcHeight;
	UINT	nSrcWidth;	
	UINT	nSrcHeight;
	UINT	nDestWidth;	
	UINT	nDestHeight;
	UINT	nSrcStartX;
	UINT	nSrcStartY;
#ifndef 	SAMSUNG_2416
	UINT	nPositionX;
	UINT	nPositionY;
#endif
	UINT    nInputFormat;
	UINT    nControlstyle;
} POSTINFO;

typedef struct _WIN1SIZE
{
	unsigned int StartPointX;
	unsigned int StartPointY;
	unsigned int Width;
	unsigned int Height;
} WIN1SIZE, *PWIN1SIZE;


#define FILE_DEVICE_VIDEO               0x00000023
#define METHOD_NEITHER                  3
#define FILE_ANY_ACCESS                 0
#define METHOD_BUFFERED                 0

#define IOCTL_POST_INIT			CTL_CODE( FILE_DEVICE_VIDEO, 1, METHOD_NEITHER,FILE_ANY_ACCESS)
#define IOCTL_POST_RUN			CTL_CODE( FILE_DEVICE_VIDEO, 2, METHOD_NEITHER,FILE_ANY_ACCESS)
#define IOCTL_POST_STOP		CTL_CODE( FILE_DEVICE_VIDEO, 3, METHOD_NEITHER,FILE_ANY_ACCESS)
#define IOCTL_POST_GETINPUTBUFFER		CTL_CODE( FILE_DEVICE_VIDEO, 4, METHOD_NEITHER,FILE_ANY_ACCESS)
#define IOCTL_POST_GETOUTPUTBUFFER		CTL_CODE( FILE_DEVICE_VIDEO, 5, METHOD_NEITHER,FILE_ANY_ACCESS)
#define	IOCTL_POST_GETHWSTATE			CTL_CODE( FILE_DEVICE_VIDEO, 6, METHOD_NEITHER,FILE_ANY_ACCESS)

#define IOCTL_WIN1_OPEN                 CTL_CODE( 32768, 2215, METHOD_BUFFERED ,FILE_ANY_ACCESS)
#define IOCTL_WIN1_CLOSE                CTL_CODE( 32768, 2216, METHOD_BUFFERED ,FILE_ANY_ACCESS)
#define IOCTL_WIN1_SETSIZE              CTL_CODE( 32768, 2217, METHOD_BUFFERED ,FILE_ANY_ACCESS)
#define IOCTL_WIN1_SETALPHA             CTL_CODE( 32768, 2218, METHOD_BUFFERED ,FILE_ANY_ACCESS)

#define CAMIF_IDLE_STATE		0
#define CAMIF_TW9910_STATE		1
#define CAMIF_PST_STATE			2

#if __cplusplus
	}
#endif

#endif
