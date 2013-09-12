/***************************************************************************
*                                                                         *
*                   SiRF Technology, Inc. Platform Software               *
*                                                                         *
*    Copyright (c) 2008 by SiRF Technology, Inc.  All rights reserved.    *
*                                                                         *
*    This Software is protected by United States copyright laws and       *
*    international treaties.  You may not reverse engineer, decompile     *
*    or disassemble this Software.                                        *
*                                                                         *
*    WARNING:                                                             *
*    This Software contains SiRF Technology, Inc.'s confidential and      *
*    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
*    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
*    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
*    Software without SiRF Technology, Inc.'s  express written            *
*    permission.   Use of any portion of the contents of this Software    *
*    is subject to and restricted by your written agreement with          *
*    SiRF Technology, Inc.                                                *
*                                                                         *
 ***************************************************************************/

#ifndef __BH_SURF_MGR_H__
#define __BH_SURF_MGR_H__
#include "platform.h"

#define BSM_ERR_NOERROR       0
#define BSM_ERR_OUTOFMEM      1
#define BSM_ERR_INVALIDPARAM  2
#define BSM_ERR_INVALIDFORMAT 3
#define BSM_ERR_UNKNOWN       4
#define BSM_ERR_NOTINIT       5
#define BSM_ERR_LOCKED        6 //try to output a locked surface

//assume we are using c++ compiler
typedef enum
{
  SX_SURFFORMAT_YV12 = 0,
  SX_SURFFORMAT_I420,
  SX_SURFFORMAT_IMC3,
  SX_SURFFORMAT_UYVY,  
  SX_SURFFORMAT_YUYV, 
}SX_SURF_FORMATID;

typedef enum
{
  SX_SURF_PROGRESSIVE = 0,
  SX_SURF_FIELD_ODD, 
  SX_SURF_FIELD_EVEN,
}SX_SURF_FIELDID;


typedef enum
{
	SX_ASYNC_WAITVSYNC = 0,
	SX_SYNC_WAITVSYNC,
	SX_NOWAIT_VSYNC,
}SX_REN_SYNC_MODE;

//assuming the edge for U,V will be half of Y for YV12 format?
typedef struct sSXMemSurfDesc
{
	 SX_INT32	        struct_siz;
	 SX_SURF_FORMATID	format_id;		
	 SX_SURF_FIELDID  field_id;

	 SX_INT32	        width;			//with edge			
	 SX_INT32	        height;			//with edge		

	 SX_BYTE*	        pFrmBuf;		//Virtual address
	 SX_BYTE*	        pPhyAddr;	//Physical address 
	 SX_INT32		      reserved[8]; 
}SXMemSurfDesc;

#ifdef BSM_HOST
#include "RendererSDK.h"
//only used by application who will host BSM, renderer and codec
#ifdef __cplusplus
extern "C" {
#endif

SXDLLIMPORT SX_UINT32 BSM_Init(VR_HANDLE handle);
SXDLLIMPORT void BSM_DeInit();

#ifdef __cplusplus
};
#endif

#endif



//caller propose the following field:
//format_id;field_id,width,height
//and Y_stride,UV_stride if format_id is SX_SURFFORMAT_YV12
#ifdef __cplusplus
extern "C" {
#endif 

SXDLL SX_UINT32 BSM_AllocSurf(SXMemSurfDesc* surf);
//release surface, unlock if locked
SXDLL SX_UINT32 BSM_ReleaseSurf(SXMemSurfDesc* surf);
//lock single surface, always lock before you retrieve the frame buffer address and manipulate the contents of it
SXDLL SX_UINT32 BSM_LockSurf(SXMemSurfDesc* surf);

SXDLL SX_UINT32 BSM_UnlockSurf(SXMemSurfDesc* surf);
//output decoded surface
SXDLL SX_UINT32 BSM_OutputSurf(SXMemSurfDesc* surf, SX_REN_SYNC_MODE WaitMode);

#ifdef BSM_HOST
SXDLLIMPORT VR_FRAME_SURFACE* CvtToRendererSurf(SXMemSurfDesc* surf);
#endif

#ifdef __cplusplus
};
#endif




typedef SX_UINT32 (*fnAllocSurf)(SXMemSurfDesc* );
typedef SX_UINT32 (*fnReleaseSurf)(SXMemSurfDesc* );
typedef SX_UINT32 (*fnLockSurf)(SXMemSurfDesc* );
typedef SX_UINT32 (*fnUnlockSurf)(SXMemSurfDesc* );
typedef SX_UINT32 (*fnOutputSurf)(SXMemSurfDesc* );


typedef struct 
{
	fnAllocSurf	BSM_AllocSurf;
	fnReleaseSurf BSM_ReleaseSurf;
	fnLockSurf	BSM_LockSurf;
	fnUnlockSurf 	BSM_UnlockSurf;
	fnOutputSurf	BSM_OutputSurf;
	
}DSMgrSurfFunctors;




#endif //__BH_SURF_MGR_H__