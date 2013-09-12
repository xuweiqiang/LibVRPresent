/*
/*++
Copyright (c) 2004  BSQUARE Corporation.  All rights reserved.

Module Name:

    smbus.h

Module Description:
    prototypes for accessing the SMBus dll.

  Author:
    IMR - January 2004

*/

// Struct used to send/revceive data
typedef struct {
	UCHAR	Address;
	ULONG   DataSize;
	UCHAR   Data[512];
} SMBUS_TRANSFER;	


extern HANDLE
SMBus_Initialize(
    VOID);

extern BOOL
SMBus_ReadData( HANDLE Handle,
                SMBUS_TRANSFER *pTransfer);

extern BOOL
SMBus_WriteData( HANDLE Handle,
                 SMBUS_TRANSFER *pTransfer);



