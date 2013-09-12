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
//------------------------------------------------------------------------------
// 
//      Bluetooth Debug Output
// 
// 
// Module Name:
// 
//      bt_debug.h
// 
// Abstract:
// 
//      This file defines interface to Bluetooth debug subsystem
// 
// 
//------------------------------------------------------------------------------
#if ! defined (__bt_debug_H__)
#define __bt_debug_H__		1

#if defined (BT_USE_CELOG)

typedef enum {
	NONE  = 0,
	BCSP  = 1,
	NOKIA = 2,
	UART  = 3,
	USB   = 4,
	SDIO  = 5
} CELOGBasebandTransport;

#define DRIVER_STRING_SIZE		256

struct BTH_CELOG_START_DATA {
	CELOGBasebandTransport	eTransport;
	SYSTEMTIME				st;
	WCHAR					szDriverString[DRIVER_STRING_SIZE];
};

struct BTH_CELOG_STOP_DATA {
	SYSTEMTIME				st;
};

#define CELOG_FLAG_RAW_IN			0x8001
#define CELOG_FLAG_RAW_OUT			0x8002

#define CELOG_FLAG_HCI_EVENT_IN		0x8011
#define CELOG_FLAG_HCI_ACL_IN		0x8012
#define CELOG_FLAG_HCI_SCO_IN		0x8013

#define CELOG_FLAG_HCI_COMMAND_OUT	0x8014
#define CELOG_FLAG_HCI_ACL_OUT		0x8015
#define CELOG_FLAG_HCI_SCO_OUT		0x8016

#define CELOG_FLAG_HCI_PARAMS		0x8017

#define CELOG_FLAG_L2CAP_IN			0x8021
#define CELOG_FLAG_L2CAP_OUT		0x8022

#define CELOG_FLAG_SDP_IN			0x8031
#define CELOG_FLAG_SDP_OUT			0x8032

#define CELOG_FLAG_RFCOMM_IN		0x8041
#define CELOG_FLAG_RFCOMM_OUT		0x8042

#define CELOG_FLAG_START			0x8801
#define CELOG_FLAG_STOP				0x8802

#ifndef __CELOG_H__
#if defined (SDK_BUILD)
#pragma warning (disable:4200)
#endif

#include <celog.h>

#if defined (SDK_BUILD)
#pragma warning (default:4200)
#endif
#endif
#else
#undef CELOGDATAFLAGGED
#define CELOGDATAFLAGGED(Time, ID, Data, Len, Zone1, Zone2, Flag)  ((void)0)
#endif

#define DEBUG_ERROR			0x00000001
#define DEBUG_WARN			0x00000002
#define DEBUG_OUTPUT			0x00000004

#define DEBUG_STREAM_ALL		0x00000008
#define DEBUG_RFCOMM_STREAM		0x00000008

#define DEBUG_SHELL_ALL			0x00000030
#define DEBUG_SHELL_INIT		0x00000010
#define DEBUG_SHELL_TRACE		0x00000020

#define DEBUG_HCI_TRANSPORT_ALL		0x000000c0
#define DEBUG_HCI_DUMP			0x00000040
#define DEBUG_HCI_TRANSPORT		0x00000080

#define DEBUG_HCI_ALL			0x00000f00
#define DEBUG_HCI_INIT			0x00000100
#define DEBUG_HCI_TRACE			0x00000200
#define DEBUG_HCI_PACKETS		0x00000400
#define DEBUG_HCI_CALLBACK		0x00000800

#define DEBUG_L2CAP_ALL			0x000f0000
#define DEBUG_L2CAP_INIT		0x00010000
#define DEBUG_L2CAP_TRACE		0x00020000
#define DEBUG_L2CAP_PACKETS		0x00040000
#define DEBUG_L2CAP_CALLBACK		0x00080000

#define DEBUG_RFCOMM_ALL		0x00f00000
#define DEBUG_RFCOMM_INIT		0x00100000
#define DEBUG_RFCOMM_TRACE		0x00200000
#define DEBUG_RFCOMM_PACKETS		0x00400000
#define DEBUG_RFCOMM_CALLBACK		0x00800000

#define DEBUG_SDP_ALL			0x0f000000
#define DEBUG_SDP_INIT			0x01000000
#define DEBUG_SDP_TRACE			0x02000000
#define DEBUG_SDP_PACKETS		0x04000000
#define DEBUG_SDP_CALLBACK		0x08000000

#define DEBUG_TDI_ALL			0xf0000000
#define DEBUG_TDI_INIT			0x10000000
#define DEBUG_TDI_TRACE			0x20000000
#define DEBUG_TDI_PACKETS		0x40000000
#define DEBUG_TDI_CALLBACK		0x80000000

#define DEBUG_PAN_ALL			0x0000f000
#define DEBUG_PAN_INIT			0x00001000
#define DEBUG_PAN_TRACE			0x00002000
#define DEBUG_PAN_PACKETS		0x00004000
#define DEBUG_PAN_CALLBACK		0x00008000

#define OUTPUT_MODE_DEBUG	0
#define OUTPUT_MODE_CONSOLE	1
#define OUTPUT_MODE_FILE	2
#define OUTPUT_MODE_CELOG	3

#define OUTPUT_MODE_MAX		3

#define OUTPUT_MODE_REOPEN	0x80000000

#define OUTPUT_FILE_MAX		(500*1024)

#if defined (DEBUG) || defined (_DEBUG) || defined (RETAILLOG)

extern unsigned int gMask;

void DumpBuff (unsigned int cMask, unsigned char *lpBuffer, unsigned int cBuffer);
void DumpBuffPfx (unsigned int cMask, WCHAR *lpszLinePrefix, unsigned char *lpBuffer, unsigned int cBuffer);
void DebugOut (unsigned int cMask, WCHAR *lpszFormat, ...);
void DebugInitialize (void);
void DebugUninitialize (void);
void DebugSetMask (unsigned int aMask);
void DebugSetOutput (unsigned int fConsole);

#define IFDBG(c) c

#else

#pragma warning (disable:4100)

static inline void DumpBuff (unsigned int cMask, unsigned char *lpBuffer, unsigned int cBuffer) {}
static inline void DumpBuffPfx (unsigned int cMask, WCHAR *lpszLinePrefix, unsigned char *lpBuffer, unsigned int cBuffer) {}
static inline void DebugOut (unsigned int cMask, WCHAR *lpsz, ...) {}
static inline void DebugInitialize (void) {}
static inline void DebugUninitialize (void) {}
static inline void DebugSetMask (unsigned int aMask) {}
static inline void DebugSetOutput (unsigned int fConsole) {}

#pragma warning (default:4100)

#define IFDBG(c)

#endif	// DEBUG || _DEBUG

#endif
