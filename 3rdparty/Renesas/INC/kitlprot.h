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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:   KitlProt.h

Abstract:
    This is the KITL protocol header file. Both Desktop tools and CE
    MUST agree on all the data structures or the protocol will broke
Functions:


Notes: 

--*/
#ifndef _CLASS_KITL_PROTOCOL_H_
#define _CLASS_KITL_PROTOCOL_H_


#ifdef __cplusplus
extern "C" {
#endif

#define CURRENT_KITL_VERSION 1

#define KITL_MTU            1520    // maximum size per packet (1518, rounded to DWORD aligned)
#define KITL_MAX_DATA_SIZE  1446    // maximum data size
#define KITL_ID             0x47424445 // "GBDE"
#define KITL_MAX_WINDOW_SIZE  16    // max window size

// KITL commands
#define KITL_CMD_BOOTME      0   // Initial bootup message from device
#define KITL_CMD_SETDEBUG    1 // Used to set debug zones on device (TBD)
#define KITL_CMD_JUMPIMG     2 // Command to tell bootloader to jump to existing
                               // flash or RAM image. Data is same as CMD_OS_CONFIG.
#define KITL_CMD_OS_CONFIG   3 // Configure OS for debug ethernet services
#define KITL_CMD_PING        4 // "Ping" device, and return information (same fmt as bootme)
#define KITL_CMD_RESET       5 // Command to have platform perform SW reset (e.g. so it
                               // can be reprogrammed).  Support for this command is
                               // processor dependant, and may not be implemented
                               // on all platforms (requires HW mods for Odo).
#define KITL_CMD_SVC_CONFIG  6
#define KITL_CMD_SVC_DATA    7
#define KITL_CMD_DEBUGBREAK  8 // Break into debugger
#define KITL_CMD_TRAN_CONFIG 9 // transport config
#define KITL_CMD_RETRASMIT   10 // this is a one way message. Desktop to device only. 
                                // what we're doing is to use desktop as the timer for
                                // the device. Desktop will send a retransmit package
                                // every 2 seconds and the device can use the package to
                                // determine if a retransimittion is needed while polling
                                // for response.

// Defs for Flags field.
#define KITL_FL_FROM_DEV 0x01   // Set if message is from the device
#define KITL_FL_NACK     0x02   // Set if frame is a nack
#define KITL_FL_ACK      0x04   // Set if frame is an ack
#define KITL_FL_SYNC     0x08   // Can be used to reset sequence # to 0
#define KITL_FL_ADMIN_RESP 0x10   // For admin messages, indicate whether this is a response

// maximum name length of KITL devices
#define KITL_MAX_DEV_NAMELEN 16

// Defs for CPUId
#define KITL_CPU_TYPE_SHX  0x10
#define KITL_CPU_TYPE_MIPS 0x20
#define KITL_CPU_TYPE_X86  0x30
#define KITL_CPU_TYPE_ARM  0x40
#define KITL_CPU_TYPE_PPC  0x50

#define	KITL_CPU_TYPE_MASK 0xf0

// my own extention
#define	KITL_CPU_TYPE_THUMB 0x60
    
#define KITL_CPU_SH3    (KITL_CPU_TYPE_SHX  | 0)
#define KITL_CPU_SH4    (KITL_CPU_TYPE_SHX  | 1)
#define KITL_CPU_R3000  (KITL_CPU_TYPE_MIPS | 0)
#define KITL_CPU_R4101  (KITL_CPU_TYPE_MIPS | 1)
#define KITL_CPU_R4100  (KITL_CPU_TYPE_MIPS | 2)
#define KITL_CPU_R4111  (KITL_CPU_TYPE_MIPS | 3)
#define KITL_CPU_R4200  (KITL_CPU_TYPE_MIPS | 4)
#define KITL_CPU_R4300  (KITL_CPU_TYPE_MIPS | 5)
#define KITL_CPU_R5230  (KITL_CPU_TYPE_MIPS | 6)
#define KITL_CPU_i486   (KITL_CPU_TYPE_X86  | 0)
#define KITL_CPU_SA1100 (KITL_CPU_TYPE_ARM | 0)
#define KITL_CPU_ARM720 (KITL_CPU_TYPE_ARM | 1)
#define	KITL_CPU_THUMB720	(KITL_CPU_TYPE_THUMB | 0)
#define KITL_CPU_PPC821 (KITL_CPU_TYPE_PPC | 0)
#define KITL_CPU_PPC403 (KITL_CPU_TYPE_PPC | 1)
#define KITL_CPU_UNKNOWN            0

#if defined (SH3)
#define KITL_CPUID      KITL_CPU_SH3
#elif defined (SH4)
#define KITL_CPUID      KITL_CPU_SH4
#elif defined (R3000)
#define KITL_CPUID      KITL_CPU_R3000
#elif defined (MIPSII)
#define KITL_CPUID      KITL_CPU_R4100
#elif defined (MIPS16)
#define KITL_CPUID      KITL_CPU_R4111
#elif defined (MIPSII_FP)
#define KITL_CPUID      KITL_CPU_R4300
#elif defined (x86)
#define KITL_CPUID      KITL_CPU_i486
#elif defined (ARMV4)
#define KITL_CPUID      KITL_CPU_SA1100
#elif defined (ARMV4I)
#define KITL_CPUID      KITL_CPU_ARM720
#elif defined (ARMV4T)
#define KITL_CPUID      KITL_CPU_THUMB720
#elif defined (PPC821)
#define KITL_CPUID      KITL_CPU_PPC821
#elif defined (PPC403)
#define KITL_CPUID      KITL_CPU_PPC403
#else
#define KITL_CPUID      KITL_CPU_UNKNOWN
#endif

// default service names
#define KITL_SVCNAME_DBGMSG  "DBGMSG"
#define KITL_SVCNAME_PPSH    "PPSH"
#define KITL_SVCNAME_KDBG    "KDBG"

// default services
#define KITL_SVC_DBGMSG  0   // Debug messages
#define KITL_SVC_PPSH    1   // Text shell and PPFS file system
#define KITL_SVC_KDBG    2   // Kernel debugger
#define KITL_SVC_ADMIN 0xFF  // Administrative messages 

// transport service IDs
// bit 6 is specified if passive kitl
// 0-127 reserved (0-63 for active mode, 64-127 for passive mode)
// 128-255 available for OEM use (128-191 for active mode, 192-255 for passive mode)
#define KTS_DEFAULT      0          // Default transport for platform
#define KTS_ETHER        1          // Ethernet transport
#define KTS_SERIAL       2          // Serial transport
#define KTS_USB          3          // USB transport
#define KTS_NONE         63         // no transport

#define KTS_PASSIVE_MODE 0x40       // flag for passive kitl mode

// retransmit timeouts
// will occur at 2,4,8, etc up to INTERVAL_MAX.
#define KITL_RETRANSMIT_INTERVAL_MS  1000
#define KITL_RETRANSMIT_INTERVAL_MAX 8000

// Number of milliseconds before timing out waiting for an ack
#define KITL_TIMEOUT_INTERVAL_MS     10000

#include <pshpack1.h>

// Format of KITL header
typedef struct _KITL_HDR
{
    DWORD Id;        // Protocol identifier ("KITL" on the wire)
    UCHAR Service;   // Service identifier
    UCHAR Flags;     // Flags (see defs below)
    UCHAR SeqNum;    // For detection of dropped packets
    UCHAR Cmd;       // For administrative messages
    // Cmd specific data starts here (format is determined by 
    // Cmd, len is determined by UDP packet size)
} KITL_HDR, *PKITL_HDR;

#define KITLDATA(p)     (((LPBYTE) (p)) + sizeof (KITL_HDR))
#define KITL_DATA_OFFSET    sizeof(KITL_HDR)

#define MAX_SVC_NAMELEN  19

// maximun number of KITL clients
#define  MAX_KITL_CLIENTS        128   // service id is 8 bits, reserve 1st bit for admin svc

// Format of service config message from device
typedef struct _KITL_SVC_CONFIG_DATA
{
    UCHAR   ProtocolVersion;  // Version of KITL protocol
    char    ServiceName[MAX_SVC_NAMELEN+1];  // NULL terminated service name
    UCHAR   ServiceId;       // Identifier that goes in the Service field 

    // Some protocol configuration specified by the device
    UCHAR   WindowSize;  // Protocol window size
    UCHAR   Flags;
} KITL_SVC_CONFIG_DATA;

#define KITL_CFGFL_STOP_AND_WAIT  0x01 // Stop after each transmission and wait for ack
#define KITL_CFGFL_NOACKS         0x02 // Don't ack data messages
#define KITL_CFGFL_MULTIINST      0x04 // Allow multiple instances of this service on device

// Only flags within this mask are sent in the SVC_CONFIG message. (other flags are
// used by RegisterClient for local state info)
#define CFGFL_MASK 0x0F

typedef struct _KITL_RESET_DATA
{
    UCHAR   Flags;
#define KITL_RESET_CLEAN  0x1   // Perform clean boot
} KITL_RESET_DATA;

// Format of debug message data
typedef struct _KITL_DBGMSG_INFO
{
    DWORD dwLen;        // Length of this header (not including dbgmsg text)
    DWORD dwThreadId;   // Thread Id
    DWORD dwProcId;     // Process Id
    DWORD dwTimeStamp;  // Time stamp (ms)

} KITL_DBGMSG_INFO, *PKITL_DBGMSG_INFO;

// transport config data from device
typedef struct _KITL_DEV_TRANSCFG {
    WORD wLen;          // length of the structure, including transport specific data
    WORD wCpuId;        // cpuid
    char szDevName[KITL_MAX_DEV_NAMELEN];
    // transport specific data follows
} KITL_DEV_TRANSCFG, *PKITL_DEV_TRANSCFG;

#define HOST_TRANSCFG_NUM_REGKEYS	8
#define HOST_TRANSCFG_KEYSIG		0x123456

// transport config data from host
typedef struct _KITL_HOST_TRANSCFG {
    DWORD dwLen;        // length of the structure, including transport specific data
    DWORD dwFlags;      // indicate what service to start
    DWORD dwKeySig;		// signiture indicating reg keys will be sent
	DWORD dwKeys[HOST_TRANSCFG_NUM_REGKEYS];	// registry keys: nk, filesys, fsdmgr, relfsd, device
    // transport specific data follows
} KITL_HOST_TRANSCFG, *PKITL_HOST_TRANSCFG;

// Some macros to manipulate sequence numbers
#define KITL_MAX_SEQ  255
#define SEQ_BETWEEN(start,s,end)  ((((start)<=(s)) && ((s)<(end))) || (((end)<(start)) && ((start)<=(s))) || (((s)<(end)) && ((end)<(start))))
#define SEQ_DELTA(start,end) (((start) <= (end)) ? ((end) - (start)) : ((KITL_MAX_SEQ+1) - (start) + (end)))
#define SEQ_ADD(s,amount)  (((s) + (amount)) % (KITL_MAX_SEQ+1))
#define SEQ_SUB1(s)        (((s)+KITL_MAX_SEQ)%(KITL_MAX_SEQ+1))
#define SEQ_INC(s)         ((s) = SEQ_ADD((s),1))

#ifndef ZONE_INIT
// Debug zone definitions
#define  ZONE_WARNING    0x0001   // Warnings
#define  ZONE_INIT       0x0002   // Init messages, including register
#define  ZONE_FRAMEDUMP  0x0004   // Dump of each Tx/Rx frame, including time stamp
#define  ZONE_TIMER      0x0008   // Timer insertions/removals
#define  ZONE_SEND       0x0010   // Tx state info
#define  ZONE_RECV       0x0020   // Rx state info
#define  ZONE_RETRANSMIT 0x0040   // Retransmissions
#define  ZONE_CMD        0x0080   // cesh commands
#define  ZONE_ERROR      0x8000   // Error
// These zones only apply on device
#ifdef UNDER_CE
#define  ZONE_INTR      0x0100  // Interrupts
#define  ZONE_ADAPTER   0x0200  // Must be combined with INTR,SEND,or RECV
#define  ZONE_LEDS      0x0400  // For writing to alpha LEDs (useful for measuring timings)
#define  ZONE_DHCP      0x0800
#endif
#endif

#include <poppack.h>

#ifdef __cplusplus
}
#endif

#endif // _CLASS_KITL_PROTOCOL_H_

