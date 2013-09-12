/******************************************************************************
* Copyright (C) 2008 by UBIQUIX, Inc. All rights reserved.
* Module name:	VEU2H Driver
* File name:	VEU2H_ioctl.h
* Author:		Ubiquix, Inc.
* Date:			2008-03-21
* Version:		1.0
* Comment:		VEU2H Driver for REVE_R2 (SH7723)
******************************************************************************/

#ifndef _VEU2H_IOCTL_H_
#define _VEU2H_IOCTL_H_
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
* INCLUDE FILES
******************************************************************************/

/******************************************************************************
* CONSTANT DEFINITIONS
******************************************************************************/
/* VEU IO control Code  
 * No bigger than a WORD value.  
 * The values used by Microsoft are in the range 0-32767; the values 32768-65535 are reserved for use by OEMs 
*/
#define IOCTL_VEU_CONVERTIMG			0x8000
#define IOCTL_VEU_SWAPVALUE				0x8001
#define IOCTL_VEU_TIMEOUT				0x8002
#define IOCTL_VEU_SET_SRCBUF			0x8003
/******************************************************************************
* MACRO DEFINITIONS
******************************************************************************/

/******************************************************************************
* TYPE DEFINITIONS
******************************************************************************/
typedef struct _VEU_DRIVER_INFO {
	CRITICAL_SECTION	csVEU2H;				// Critical Section for protecting VEU2H
	ULONG				OpenCount;				// Count of simultaneous opens
	DWORD				dwChannel;				// VEU2H channel number		
	DWORD				nFrameHeight;			// input image height for converting image
	DWORD				nFrameWidth;			// input image width for converting image
	PVOID				pDispBufferBase;		// physical continous memory for VEU2H src
	UCHAR				*yBuf, *yBufPhysical;
	UCHAR				*uBuf, *uBufPhysical;
	UCHAR				*rgbBuf, *rgbBufPhysical;
	PHYSICAL_ADDRESS	m_physAddr;
	DMA_ADAPTER_OBJECT	dmaAdapterObject;	
} VEU_DRIVER_INFO, *PVEU_DRIVER_INFO;

/******************************************************************************
* GLOBAL VARIABLE PROTOTYPES
******************************************************************************/

/******************************************************************************
* FUNCTION PROTOTYPES
******************************************************************************/

#ifdef __cplusplus
}
#endif
#endif	/*_VEU2H_IOCTL_H_*/