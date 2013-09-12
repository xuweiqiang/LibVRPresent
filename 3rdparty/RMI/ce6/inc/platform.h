/*
Copyright:

    Copyright (c) 2001-2004 BSQUARE Corporation. All rights reserved.

Module Name:

    platform.h

Abstract:


    Defines for system that are neither processor nor board specific

Author:

	GJS March 2001


*/
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

// Some defines to allow us to use AMD's reference au1x00.h file
// for all processor level defines and structures
#ifndef ASSEMBLER
typedef unsigned long  uint32;
typedef unsigned short uint16;
typedef unsigned char  uint8;
#endif

// TODO, gotta put these somewhere else I think....
#define KSEG0_OFFSET        0x80000000
#define KSEG1_OFFSET        0xA0000000
#define XTAL_FREQ           12000000

#include "au1platform.h"

// Define CPU variables used by the au1x00.h header file.
#ifdef CPU_AU1000
#define AU1000
#endif

#ifdef CPU_AU1100
#define AU1100
#endif

#ifdef CPU_AU1500
#define AU1500
#endif

#ifdef CPU_AU1550
#define AU1550
#endif

#ifdef CPU_AU1200
#define AU1200
#endif

#ifdef CPU_AU13XX
#define AU13XX
#endif

#include "au1x00.h"
#include "au1x00.s"
#include "memmap.s"
#include "au1x_common.h"

// Required for Common Drivers (5.0 and 6.0)
#define xxxStallExecution(x)	OALStallExecution(x)

#define     HAL_OEM_STRING       (TEXT("Bsquare Corp"))

// FIX!!! move to more appropriate place The 32kHz crystal
#define TICKER_SOURCE_CLOCK_FREQ     32768
#define DEFAULT_RTC_FREQ 1

//
// Processor information used by IOCTL_PROCESSOR_INFORMATION.
//

#define PROCESSOR_INFO_VERSION          1
#define PROCESSOR_INFO_CORE             (TEXT("MIPS32"))
#define PROCESSOR_INFO_CAT_NUM          (TEXT(""))
#define PROCESSOR_INFO_VENDOR           (TEXT("AMD/Alchemy"))
#define PROCESSOR_INFO_INSTRUCTION_SET  0
#define PROCESSOR_INFO_CLOCK_SPEED      0

#define PROCESSOR_INFO_PROC_NAME_MASK   0xff000000
#define PROCESSOR_INFO_PROC_NAME_RSH    24
#define PROCESSOR_INFO_PROC_NAME_UNDEF  (TEXT("???"))
#define PROCESSOR_INFO_PROC_NAME_0      (TEXT("Au1000"))
#define PROCESSOR_INFO_PROC_NAME_1      (TEXT("Au1500"))
#define PROCESSOR_INFO_PROC_NAME_2      (TEXT("Au1100"))
#define PROCESSOR_INFO_PROC_NAME_3      (TEXT("Au1550"))
#define PROCESSOR_INFO_PROC_NAME_4      (TEXT("Au1200"))

#define PROCESSOR_INFO_PROC_REV_MASK    0xff

#define EBOOT_STORE_ADDRESS	0xBFD00000
#define EBOOT_STORE_MAX_LENGTH	0x20000

#endif // __PLATFORM_H__