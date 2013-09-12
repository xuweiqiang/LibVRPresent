//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//

#ifndef _NETLOGIOCTL_H
#define _NETLOGIOCTL_H
typedef struct _NETLOG_GLOBAL_STATE
{
    BOOL		bLogUSB;  //  Not IOCTL Controled.
    BOOL        bStopped; //  Default we are stopped. 
    DWORD       dwMaxPacketSize; // Log Packet upto len - dwMaxPacketSize;
    DWORD       dwHalfCaptureSize; // Size at which we need to roll over to alternate packetcapture.
    WCHAR       wszCapFileBaseName[MAX_PATH+1]; // Set using an IOCTL
    DWORD       dwCapFileIndex; // a 0 or 1 depending on file rollover.
} NETLOG_GLOBAL_STATE;
// Specifies IOCTL interface between netlog and netlogctl.

#define   IOCTL_NETLOG_START                        1
#define   IOCTL_NETLOG_STOP                         2
#define   IOCTL_NETLOG_FILE_NAME_CHANGE             3
#define   IOCTL_NETLOG_FILE_SET_PARAM               4
#define   IOCTL_NETLOG_CAPTURE_PACKET_SIZE          5
#define   IOCTL_NETLOG_HALF_CAPTURE_SIZE            6
#define   IOCTL_NETLOG_LOG_USB                      7
#define   IOCTL_NETLOG_GET_STATE                    8

#endif 
