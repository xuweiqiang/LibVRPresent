/*
/*++
Copyright (c) 2003  BSQUARE Corporation.  All rights reserved.

Module Name:

    ssi.h

Module Description:
    prototypes for accessing the SSI dll.

  Author:
    IMR - July 2003

*/

extern HANDLE
SSI_Initialize(
    VOID);

extern BOOL
SSI_ReadData( HANDLE Handle,
			  ULONG  Address,
			  PULONG pData );



