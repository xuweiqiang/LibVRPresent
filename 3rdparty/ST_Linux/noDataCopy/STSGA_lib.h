
#ifndef __STSGA_LIB_H__
#define __STSGA_LIB_H__

#include <stdio.h>
#include <stdlib.h>

#define STSGA_EN

//#define _GNU_LINUX
#ifndef _GNU_LINUX
#include <Windows.h>
#include "stglib_wince.h"
#else
#include "stglib_lnx.h"
#endif

#ifdef STSGA_EN

#define STSGA_MEM_MAXCNT		20
#define STSGA_MEM_DISP_WIDTH	800
#define STSGA_MEM_DISP_HEIGHT	480
#define STSGA_MEM_EXTSIZE		1024

typedef struct
{
	STHDC		m_hdc;
	STHBITMAP	m_bitmap;
	void*		pVirAdr;
#ifndef _GNU_LINUX
	void*		pPhyAdr;
#else
	unsigned int *pPhyAdr;
#endif
}STSGA_MEM_S;

typedef struct
{
	STHDC		m_hdc;
	STHBITMAP	m_bitmap[STSGA_MEM_MAXCNT];
}STSGA_MEMHANDLE;

#ifdef __cplusplus
extern "C" {
#endif

void* STSGA_Init();
void  STSGA_Uninit();
void* STMalloc(unsigned int size);
void  STFree(void *pbuf);

#ifdef __cplusplus
}
#endif
#endif

#endif //__STSGA_LIB_H__

