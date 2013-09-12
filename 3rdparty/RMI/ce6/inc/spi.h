/*
/*++
Copyright (c) 2004  BSQUARE Corporation.  All rights reserved.

Module Name:

    spi.h

Module Description:
    prototypes for accessing the SPI driver

  Author:
    IMR - July 2004

*/

// Struct used to send/revceive data
typedef struct {
	UCHAR	Address;
	UCHAR   Register;
	ULONG   DataSize;
	UCHAR   Data[1];
} SPI_TRANSFER;	


extern HANDLE
SPI_Initialize( ULONG SPIConfiguration);

extern BOOL
SPI_DoTransfer( HANDLE Handle,
                ULONG  Size,
                PULONG pTxBuffer,
				PULONG pRxBuffer );


