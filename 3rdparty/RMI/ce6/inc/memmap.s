/*
Copyright:

    Copyright (c) 2001-2003 BSQUARE Corporation. All rights reserved.

Module Name:

    memmap.s

Abstract:

    
    Defines for the memory layout fo the system
    Board level memory layout is defined in the pltform
    level headers (platform.s/.h)
    
   

Author:

	GJS March 2001	
    
	    
*/

#ifndef __MEMMAP_S__
#define __MEMMAP_S__

//
//  Address regions for CLI and CE reserved areas
//

#define SDRAM1_PHYSADDR            0x00000000
#define SDRAM2_PHYSADDR            0x02000000
#define SDRAM1_SIZE                0x02000000
#define SDRAM2_SIZE                0x02000000

#define SDRAM_BASE_ADDRESS         SDRAM1_PHYSADDR
#define SDRAM_SIZE	                0x04000000

#define BOOTARG_SIZE               0x1000
#define LOADER_IMAGE_SIZE          0x20000
#define STACK_SIZE                 0x10000
#define HEAP_SIZE                  0x40000
// 8 packets. Tx and Rx
#define KITL_SIZE                  1536*8*2
#define NIC_PACKET_LEN             0x800

//
// Physical addresses ensure that these match config.bib/cli.bib
// The first 2 entries are required for CE and the loader
// the rest are loader only and may be overwritten by CE
// Also, for the the ethernet size is different (EDBG_DFLT_BUFFER_POOL_SIZE)
//
// Start       End         Size
// ============================
// 80000000    80001000    1000    Boot args // MOVED TO AVOID CLASH WITH YAMON!!!!

// 80001000    80001000    4000    Ethernet
// 80006000    80011000    10000   Stack area / KITL
// 80026000    80031000    20000   Heap 
//
//

#define    BOOTARG_PHYSADDR_BASE       (0x000FFC00)

#define    RAM_PHYSADDR_BASE           SDRAM_BASE_ADDRESS      
#define    NIC_RX_PHYSADDR_BASE        (0x1000 + BOOTARG_SIZE)
#define    NIC_TX_PHYSADDR_BASE        (NIC_RX_PHYSADDR_BASE + \
                                       (NIC_PACKET_LEN*4))
#define    KITL_PHYSADDR_BASE          (NIC_TX_PHYSADDR_BASE + \
                                       (NIC_PACKET_LEN*4))
#define    STACK_PHYSADDR_BASE         (NIC_TX_PHYSADDR_BASE + \
                                       STACK_SIZE + KITL_SIZE - 4)    // Grows down
                                       
#define    HEAP_PHYSADDR_BASE          (STACK_PHYSADDR_BASE)
#define    DEFAULT_DOWNLOAD_ADDRESS    0x81000000


//
// This area is used by CE only, it's for intermediate packets
//
#define KITL_MEMORY_START                  (KITL_PHYSADDR_BASE + KSEG0_OFFSET)
#define KITL_MEMORY_SIZE                   0x20000


//
// EDBG args passed from CLI to CE live here, it's 0x1000 in length
//
#define    BOOT_ARG_PTR            (BOOTARG_PHYSADDR_BASE)

//
// If loader is running from RAM (used during development of loader)
//
#ifdef RUNNINGFROMRAM
#define RAM_BASE                   (KSEG1_OFFSET + RAM_PHYSADDR_BASE)
#else 
#define RAM_BASE                   (KSEG1_OFFSET + RAM_PHYSADDR_BASE)
#endif

//
// Stack
//
#define STACK_BASE                 (STACK_PHYSADDR_BASE + KSEG0_OFFSET)

// put the heap under the stack
// align heap on a decent boundary (0x1000)
//
#define     HEAP_BASE              (HEAP_PHYSADDR_BASE + KSEG0_OFFSET) 

//
// Ethernet memory locations
// These need to be physical. not KSEG for the DMA
// nees to be cache aligned
//
// This should be on a cache line boundary (32 words)
//
#define NIC_RX_BASE                (NIC_RX_PHYSADDR_BASE + KSEG0_OFFSET)
#define NIC_TX_BASE                (NIC_TX_PHYSADDR_BASE + KSEG0_OFFSET)


#define FREEMEM_START              (HEAP_PHYSADDR_BASE + HEAP_SIZE)

// Cross reference this value with platform.reg and config.bib
#if FREEMEM_START > (0x80000)
#error ("ERROR: Loader using more than 1/2 meg, change cli.bib")
#endif

//
// For the Ethernet debugging in CE the area reserved for the buffers
// must not interfere with anything else.
// Change config.bib and the > 0x8xxxxxxx value to a non conflicting 
// size and address upon error
//

#if (((EDBG_DFLT_BUFFER_POOL_SIZE + NIC_RX_BASE) & ~0xf0000000)  > 0x18000)
#error "The Ethernet debug packet area is too large, change config.bib"
#endif


  



#endif __MEMMAP_S__