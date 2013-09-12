/*************************************************************************
**
** STGLIB, Accelerated 2D Graphics Library for Cartesio Plus SGA Engine.
**
** Copyright (C) 2009-2010 STMicroelectronics.  All rights reserved.
**
** This software is the confidential and/or proprietary information of
** STMicroelectronics. You shall not disclose such information and shall
** use it only in accordance with the terms of the license agreement you
** entered into with STMicroelectronics.
**
*************************************************************************/

#ifndef __STGLIB_IOCTL_H
#define __STGLIB_IOCTL_H

#pragma warning (disable : 4996 )

#include "stglib_common.h"

#include "vip_params.h"
#include "cam_params.h"

typedef struct s_ioctlContext {
	HDC			m_ioctlHDC;
	int			m_refCount;
} t_ioctlContext;

typedef struct {
	sga_uint32	lockID;
	HANDLE		sgaBatchHandleID;
} t_sgaDriverParameters;


/*
 * IOCTL Drivers codes
 */
#define IOCTL_VIP_INIT_PORT				0x84962004
#define IOCTL_VIP_START_PORT			0x84962008
#define IOCTL_VIP_STOP_PORT				0x8496200C
#define IOCTL_VIP_TERMINATE				0x84962010
#define IOCTL_VIP_GET_BUF_INDEX			0x84962014
#define IOCTL_VIP_INIT_DMA				0x84962018
#define IOCTL_VIP_DEINIT_DMA			0x8496201C

#define IOCTL_CAM_MODULE_INIT			0x84C32004
#define IOCTL_CAM_SENSOR_CONTROL		0x84C32008
#define IOCTL_CAM_MODULE_DEINIT			0x84C3200C

/*
 * IOCTL Escape codes
 */
#define IOCTL_SGA_START_BATCH_SYNC		6900
#define IOCTL_SGA_START_BATCH_ASYNC		6901
#define IOCTL_SGA_CREATE_BATCH			6902
#define IOCTL_SGA_DESTROY_BATCH			6903
#define IOCTL_SGA_WAIT_BATCH			6904
#define IOCTL_SGA_VIRT2PHYS				6905
#define IOCTL_ALLOC_MEM					6906
#define IOCTL_FREE_MEM					6907
#define IOCTL_SGA_GET_STATISTICS		6908
#define IOCTL_SGA_GET_SRAM_ADDRESS		6909
#define IOCTL_SGA_GET_TCM_ADDRESS		6910
#define IOCTL_SGA_QUERY_STA2065_SUPPORT	6911
#define IOCTL_SGA_GET_CUT_VERSION		6912

int InitIoctlContext(void);
int ReleaseIoctlContext(void);

t_sgaMemoryPool *AllocateSGAMemory(LPSTHDC pDC, const size_t size);
int FreeSGAMemory(LPSTHDC pDC, t_sgaMemoryPool *pool);
int CreateSGACommandBatch(LPSTHDC pDC);
int DestroySGACommandBatch(LPSTHDC pDC);
int FlushSGACommandBatch(LPSTHDC pDC);
int WaitSGACommandBatch(LPSTHDC pDC);
int GetSGAPhysicalAddress(void* dst, void* src);
int GetSGAStatistics(LPSTHDC pDC, t_sgaStatistics *stats);
DWORD GetSramBuffer(DWORD dwSize);
DWORD GetTCMBuffer(DWORD dwSize);

DWORD QuerySTA2065Support(DWORD *pdwSTA2065Support);
DWORD GetCutVersion(DWORD *pdwCutVersion);
BOOL DoInitCameraModule(STHBITMAP hBitmap);
BOOL DoInitVideoIn(DWORD dwVideoMapsNum);
BOOL DoInitVideoPipe(STHBITMAP hBitmap, DWORD dwVideoMapIndex);
BOOL DoStartVideoIn(VOID);
BOOL DoStopVideoIn(VOID);
BOOL DoDeinitCameraModule(VOID);
BOOL DoDeinitVideoPipe(DWORD dwVideoMapIndex);
BOOL DoDeinitVideoIn(VOID);
DWORD DoGetBufIndex(VOID);

#endif /* __STGLIB_IOCTL_H */
