/*++
Copyright (c) 2005  BSQUARE Corporation.  All rights reserved.

Module Name:

    lcd.h

Module Description:

    This module defines the interface between the MAE and
    LCD drivers. 

--*/

#ifndef __LCD_H__
#define __LCD_H__

#define SIZE_RAM_ARRAY 256

#define DISPLAY_REGKEY (TEXT("Drivers\\Display\\AU13XXLCD"))
#define DISPLAY_WINDOWS_REGKEY (TEXT("Drivers\\Display\\AU13XXLCD\\Windows"))

/*
 *	Data definitions and IOCTLs for new OVERLAY functionality within LCD driver
 */
typedef struct {
	RECT OvlRect;
  ULONG nBpp;
  ULONG nKeyColor;
  ULONG nBackColor;
  ULONG nOvlNum;
  ULONG nRamArrayMode;
  ULONG nRamArray[SIZE_RAM_ARRAY];
} DRV_ESCAPE_OVERLAY_CONFIG;

typedef struct {
	DWORD	OverlayNumber;
  BOOL  OverlayEnable; // 1 enable 0 disable
} DRV_ESCAPE_OVERLAY_EN;

typedef struct {
	DWORD	DataAddress;
  BOOL  OverlayNumber; 
} DRV_ESCAPE_OVERLAY_ADDRESS;

#define DRV_ESCAPE_BASE	0x10000

#define DRV_ESCAPE_OVERLAY_ENABLE		    (DRV_ESCAPE_BASE+1)	// takes DRV_ESCAPE_OVERLAY_EN struct,
#define DRV_ESCAPE_SET_OVERLAY_CONFIG   (DRV_ESCAPE_BASE+2) // takes DRV_ESCAPE_OVERLAY_CONFIG struct
#define DRV_ESCAPE_SET_OVERLAY_ADDRESS	(DRV_ESCAPE_BASE+3)	// takes DRV_ESCAPE_OVERLAY_ADDRESS struct
#define DRV_ESCAPE_SET_GLOBAL_CONFIG	  (DRV_ESCAPE_BASE+4)	//  takes DRV_ESCAPE_OVERLAY_CONFIG offset
#define DRV_ESCAPE_GET_GLOBAL_CONFIG	  (DRV_ESCAPE_BASE+5)	//  returns DRV_ESCAPE_OVERLAY_CONFIG offset
#define DRV_ESCAPE_GET_OVERLAY_INFO	    (DRV_ESCAPE_BASE+6)	//  returns DRV_ESCAPE_OVERLAY_CONFIG offset

#endif __LCD_H__