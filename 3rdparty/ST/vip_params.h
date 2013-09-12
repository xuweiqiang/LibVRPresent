
/*******************************************************************************************************/
/**
*	Copyright (c) 2006 STMicroelectronics - All Rights Reserved
*	Reproduction and Communication of this document is strictly prohibited
*	unless specifically authorized in writing by STMicroelectronics
*
*	Module Name		 : vip_params.h
*	Version			 : 1.02.01
*	Author			 : STMicroelectronics
*	Description		 : Header with support for configuring the Video-Input-Port
*
*	Remark(s)		 : For WinCE 6.0
*/
/*******************************************************************************************************/


/* VIP control register settings */
#define EMBEDDED_SYNCHRONIZATION_CODES		1<<4
#define LATCHING_PCK_ACTIVE_EDGE_RISE		1<<5
#define HSYNC_ACTIVE_STATE_HIGH				1<<6
#define VSYNC_ACTIVE_STATE_HIGH				1<<7

#define TRANSFORMATION_INACTIVE				0<<8
#define TRANSF_CbYCrY16_TO_CbYCrY16			1<<8
#define TRANSF_PbYPrY16_TO_PbYPrY16			1<<8
#define TRANSF_CbYCrY16_TO_CrYCbY16			2<<8
#define TRANSF_PbYPrY16_TO_PrYPbY16			2<<8
#define TRANSF_RGB888_TO_ARGB8888			3<<8
#define TRANSF_RGB888_TO_ABGR8888			4<<8
#define TRANSF_RGB888_TO_BGRA8888			5<<8
#define TRANSF_RGB888_TO_RGBA8888			6<<8
#define TRANSF_RGB565LSBFirst_TO_RGB565		1<<8
#define TRANSF_RGB565MSBFirst_TO_RGB565		8<<8
#define TRANSF_YPbYPr16_TO_PbYPrY16			8<<8
#define TRANSF_RGB565MSBFirst_TO_BGR565		10<<8
#define TRANSF_JPEG_TO_JPEG					9<<8
#define INPUT_3C_COLOR_FILLER				15<<8

#define ODD_EVEN_FIELD_DETECTION_ENABLED	1<<23

/* Typedefinition */
typedef struct tagVipInitParams {
	DWORD dwVideoMapsNum;
	DWORD dwVipControlReg;
}VIP_INIT_PARAMS, *PVIP_INIT_PARAMS;

typedef struct tagVipPipeParams {
	DWORD dwPhyMemBuff;
	DWORD dwTransferSize;
	DWORD dwBufferSize;
	DWORD dwVideoMapIndex;
}VIP_PIPE_PARAMS, *PVIP_PIPE_PARAMS;

//EOF
