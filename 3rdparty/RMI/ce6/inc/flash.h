/*++
Copyright (c) 2002-2005  BSQUARE Corporation.  All rights reserved.

Module Name:

    flash.h

Module Description:

    Flash information for Alchemy boards

Author:
    GJS 2002

Revision History:


--*/

///******************************defines********************************************//

#ifndef __FLASH_H__
#define __FLASH_H__

// Assume FLASH width of 32bits. Boards with 16bit width should
// set this define to 16 in the board specific header file
#ifndef FLASH_WIDTH
#define FLASH_WIDTH 32
#endif


// FLASH chip specific defines
#if defined(FLASH_AM29LV640M) || defined(FLASH_AM29LV128M) || defined(FLASH_AM29LV256M)
 // Db boards, Hydrogen 2+3, Mirage
#define SECTOR_SIZE_BYTES		0x10000
#define BOOTSECTOR_COUNT		0
#define BOOTSECTOR_SIZE_BYTES	0
#define AMD_WRITE_BUFFER_SIZE   (16*(FLASH_WIDTH/8))

#elif defined(FLASH_AM29DL323DB)
 // Pb1000
#define SECTOR_SIZE_BYTES		0x20000
#define BOOTSECTOR_COUNT		8
#define BOOTSECTOR_SIZE_BYTES	0x4000
#define AMD_WRITE_BUFFER_SIZE   (16*(FLASH_WIDTH/8))

#elif defined(FLASH_S29GL512N)
 // Pb1200
#define SECTOR_SIZE_BYTES		0x20000
#define BOOTSECTOR_COUNT		0
#define BOOTSECTOR_SIZE_BYTES	0
#define AMD_WRITE_BUFFER_SIZE   (16*(FLASH_WIDTH/8))

#elif defined(FLASH_AM29GL256P)
 // DbAu13xx boards
#define SECTOR_SIZE_BYTES		0x10000
#define BOOTSECTOR_COUNT		0
#define BOOTSECTOR_SIZE_BYTES	0
#define AMD_WRITE_BUFFER_SIZE   (16*(FLASH_WIDTH/8))

#else
 // Not AMD FLASH. Currently not supported by Kernel FLASH algorithms
#define SECTOR_SIZE_BYTES		0x10000
#endif

// Location of FLASH
#define FLASH_BASE_KSEG1        (FLASH_BASE + KSEG1_OFFSET)

//
// Read flash/Write flash macros
//
#if (FLASH_WIDTH==32)

 #define WRITE_FLASH(Addr,Value) \
	do {                                                  \
		*(volatile unsigned long * const)(Addr) = (Value); \
		__asm("sync");                                    \
	} while(0)  /* (trailing ; supplied by caller) */

 #define READ_FLASH(Addr) \
	*(volatile unsigned long * const)(Addr)

#elif (FLASH_WIDTH==16)

 #define WRITE_FLASH(Addr,Value) \
	do {                                                  \
		*(volatile unsigned short * const)(Addr) = (USHORT)((Value)&0xFFFF); \
		__asm("sync");                                    \
	} while(0)  /* (trailing ; supplied by caller) */

 #define READ_FLASH(Addr) \
	*(volatile unsigned short * const)(Addr)

#else
 #error Flash Width must be either 16 or 32
#endif

#define READ_FLASH_ULONG(Addr) \
	*(volatile unsigned long * const)(Addr)

#define OFFSET_MANUFACTURER_ID 0
#define OFFSET_DEVICE_ID       4


//
// Flash Commands
//
#define  CMDRESET               0x1000
#define  CMDUNLOCK              0x1001
#define  CMDAUTOSELECT          0x1002
#define  CMDERASESET            0x1003
#define  CMDERASESECTOR         0x1004
#define  CMDSUSPENDERASE        0x1005
#define  CMDRESUMEERASE         0x1006
#define  CMDPROGRAM             0x1007
#define  CMDUNLOCKBYPASSSET     0x1008
#define  CMDUNLOCKBYPASSRESET   0x1009
#define  CMDCHIPERASE           0x1010
#define  CMDREADIDCODES         0x1011
#define  CMDCLEARSTATUS         0x1012
#define  CMDREADSTATUS          0x1013
#define  CMDWRITEWORD           0x1014
#define  CMDWRITEBUFFER         0x1015
#define  CMDCOMMITBUFFER        0x1016

//
// AMD Specifics
//
#define LATCH_OFFSET1			(0x555 * (FLASH_WIDTH/8))
#define LATCH_OFFSET2			(0x2AA * (FLASH_WIDTH/8))
#define LATCH_UNLOCKBYPASS		(0xBA * (FLASH_WIDTH/8))
#define AMD_UNLOCK1 0x00AA00AA
#define AMD_UNLOCK2 0x00550055

//
// Flash status
//
#define STATUSREADY         100
#define STATUSERASESUSPEND  101
#define STATUSTIMEOUT       102
#define STATUSBUSY          103
#define STATUSERROR         104

// defines for GetFlashStatus() function
#define STATUS_READY            0x00000000
#define STATUS_ERASESUSPEND     0x00040004 // DQ2
#define STATUS_TIMEOUTVALUE     0x00200020 // DQ5
#define STATUS_TOGGLEVALUE      0x00400040 // DQ6 - toggle bit one


#define AMD_AUTOSELECT32        0x00900090
#define AMD_RESET32             0x00F000F0
#define AMD_ERASESET            0x00800080
#define AMD_ERASESECTOR         0x00300030
#define AMD_SUSPENDERASE        0x00B000B0
#define AMD_RESUMEERASE         0x00300030
#define AMD_PROGRAM             0x00A000A0
#define AMD_PROGRAMBUFFER       0x00250025
#define AMD_UNLOCKBYPASS        0x00200020
#define AMD_UNLOCKBYPASSPROGRAM 0x00A000A0
#define AMD_UNLOCKBYPASSRESET1  0x00900090
#define AMD_UNLOCKBYPASSRESET2  0x00000000
#define AMD_CHIPERASE           0x00100010
#define AMD_COMMITBUFFER        0x00290029

//
// Intel Strata definitions, not used in the Kernel yet
//
#define INTEL_READ_ARRAY                0x00ff00ff
#define INTEL_READ_IDENTIFIER_CODES     0x00900090
#define INTEL_READ_STATUS_REGISTER      0x00700070
#define INTEL_CLEAR_STATUS_REGISTER     0x00500050
#define INTEL_WRITE_TO_BUFFER           0x00E800E8
#define INTEL_WRITE_TO_BUFFER_CONFIRM   0x00D000D0
#define INTEL_WORD_PROGRAM              0x00400040
#define INTEL_BLOCK_ERASE               0x00200020
#define INTEL_BLOCK_ERASE2              0x00d000D0
#define INTEL_SET_BLOCK_LOCK_BIT        0x00600060
#define INTEL_SET_BLOCK_LOCK_BIT2       0x00010001
#define INTEL_CLEAR_BLOCK_LOCK_BIT      0x00600060
#define INTEL_CLEAR_BLOCK_LOCK_BIT2     0x00D000D0
#define STATUS_WRITE_STATE_MACHINE      0x00800080
#define STATUS_ERASE_SUSPEND            0x00400040
#define STATUS_ERASE_CLEAR_LOCK_BIT     0x00200020
#define STATUS_PROGRAM_SET_LOCK_BIT     0x00100010
#define STATUS_PROGRAM_VOLTAGE          0x00080008
#define STATUS_PROGRAM_SUSPEND          0x00040004
#define STATUS_DEVICE_PROTECT           0x00020002
#define STATUS_ERROR                    0x00200020

#define INTEL_WRITE_BUFFER_SIZE         0x10
#define FLASH_TIMEOUT_PERIOD            3




//
// erased location value
//
#define FLASH_ERASE_VALUE       0xFF

//
//  Persistent registry info
//

typedef struct {
    DWORD dwLength;     // holds length of data
    DWORD dwNumWrites;
    DWORD dwCheckSum;   // holds checksum
} FLASHREG_HDR, *PFLASHREG_HDR;

#define REGISTRY_SIZE_IN_FLASH			0x80000		// half a meg, including the header
#define REGISTRY_FLASH_START_ADDRESS    FLASH_BASE_KSEG1 // will normally be 0xBE000000
#define REGISTRY_FLASH_HEADER_SIZE      sizeof(FLASHREG_HDR)
#define REGISTRY_SIZE_MAX               (REGISTRY_SIZE_IN_FLASH -  REGISTRY_FLASH_HEADER_SIZE)
#define ERASEFLASHLOCATIONVALUE         0xFFFFFFFF
#define REGISTRY_FLASH_DATA_OFFSET      (REGISTRY_FLASH_START_ADDRESS+REGISTRY_FLASH_HEADER_SIZE)

//
// FLASH resident nk.bin defines
//
// First word will be programmed to "NK.B" once image is programmed
// Second word will contain file length
#define NK_BIN_FLASH_OFFSET				0 // Let's have this right at the start FLASH
#define NK_BIN_FLASH_ADDR				(FLASH_BASE_KSEG1+NK_BIN_FLASH_OFFSET)
#define NK_BIN_SIG_WORD					0x4E4B2E42 // "NK.B"
#define NK_BIN_RAM_BUFFER				0x82000000


#endif  // _FLASH_H__