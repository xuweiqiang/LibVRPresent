/*++
Copyright (c) 2005  BSQUARE Corporation.  All rights reserved.

Module Name:

    maelcd.h

Module Description:

    This module defines the interface between the MAE and
    LCD drivers. 

--*/

#ifndef __MAELCD_H__
#define __MAELCD_H__

typedef struct {
	DWORD	x;
	DWORD	y;
	DWORD	w;
	DWORD	h;
	DWORD bpp;
	DWORD colorkey;
  DWORD backcolor;
  BOOL  fullscreen;
  DWORD index;
} DRV_ESCAPE_MAE_CONFIG;

#define DRV_ESCAPE_BASE	0x10000

#define DRV_ESCAPE_MAE_ENABLE		(DRV_ESCAPE_BASE+1)		// takes MAE_CONFIG struct,
#define DRV_ESCAPE_MAE_DISABLE		(DRV_ESCAPE_BASE+2)		// returns DWORD offset
#define DRV_ESCAPE_MAE_MODIFY		(DRV_ESCAPE_BASE+3)		// takes MAE_CONFIG struct
#define DRV_ESCAPE_MAE_GET_ADDRESS	(DRV_ESCAPE_BASE+4)		//  returns DWORD offset
#define DRV_ESCAPE_MAE_GET_SCREEN	(DRV_ESCAPE_BASE+5)		//  returns MAE_CONFIG offset
#define DRV_ESCAPE_MAE_GET_OVERLAY	(DRV_ESCAPE_BASE+6)		//  returns MAE_CONFIG offset
#define DRV_ESCAPE_SUBPICTURE_ENABLE (DRV_ESCAPE_BASE+7)
#define DRV_ESCAPE_SUBPICTURE_DISABLE (DRV_ESCAPE_BASE+8)
#define DRV_ESCAPE_SUBPICTURE_SET (DRV_ESCAPE_BASE+9)
#define DRV_ESCAPE_SUBPICTURE_CLEAR (DRV_ESCAPE_BASE+10)
#define DRV_ESCAPE_SUBPICTURE_ALLOCATE (DRV_ESCAPE_BASE+11)
#define DRV_ESCAPE_SUBPICTURE_DESTROY (DRV_ESCAPE_BASE+12)

#endif __MAELCD_H__