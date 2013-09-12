// Copyright (c) 2005  BSQUARE Corporation.  All rights reserved.

#ifndef __CAMERA_H
#define __CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

// Supported configurations.

#define RAW_SXGA              1
#define RAW_VGA               2
#define RAW_CIF               3
#define RAW_QVGA              4
#define RAW_QCIF              5
#define RAW_QQVGA             6
#define RAW_QQCIF             7
#define BAYER_SXGA            8
#define BAYER_VGA             9
#define BAYER_CIF             10
#define BAYER_QVGA            11
#define BAYER_QCIF            12
#define BAYER_QQVG            13
#define BAYER_QQCI            14
#define YCbCr_SXGA_RAW        18
#define YCbCr_VGA_RAW         19
#define YCbCr_CIF_RAW         20
#define YCbCr_QVGA_RAW        21
#define YCbCr_QCIF_RAW        22
#define YCbCr_SXGA            23
#define YCbCr_VGA             24
#define YCbCr_CIF             25
#define YCbCr_QVGA            26
#define YCbCr_QCIF            27
#define YCbCr_NTSC			  28

// Driver IOCTLS. codes 0-2047 are reserved for Microsoft Corporation
#define IOCTL_CAMERA_QUERY         \
    CTL_CODE(FILE_DEVICE_HAL, 2048, METHOD_BUFFERED, FILE_ANY_ACCESS)

#define IOCTL_CAMERA_CONFIGURE     \
    CTL_CODE(FILE_DEVICE_HAL, 2049, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_CAMERA_CAPTURE       \
    CTL_CODE(FILE_DEVICE_HAL, 2050, METHOD_BUFFERED, FILE_WRITE_ACCESS)

#define IOCTL_CAMERA_M2M_WEAVE      \
    CTL_CODE(FILE_DEVICE_HAL, 2060, METHOD_BUFFERED, FILE_WRITE_ACCESS)

//Weaved at camera driver
//#define MEMCPY_WEAVED_AT_RENDER_THREAD

//Weaved at Render thread
//#define DDMA_WEAVE_AT_RENDER_THREAD // not use memcpy

#define BSA_WEAVE_AT_DRIVER

// Structure returned by the QUERY command
typedef struct {
    DWORD mode;                 // currently configured camera mode
    TCHAR modename[32];         // the mode's name, if any
    DWORD width;                // selected resolution
    DWORD height;               // ditto
    DWORD nPlanes;              // number of channels on which pixel info is received
} CameraMode;

// Output formats.
#define  CMOS_RAW               0
#define  CMOS_CCIR656           1
#define	 INT_CCIR656			2

#define FRAME_SIZE (720 * 512 * 2)

#define M2M_ALIGNMENT 32
#define NUM_CAM_INPUT_BUFFERS 4

typedef struct _VP_PTR
{
	PBYTE pMem;
	ULONG Phys;
} VP_PTR,*P_VP_PTR;	
#ifdef __cplusplus
}
#endif

#endif

