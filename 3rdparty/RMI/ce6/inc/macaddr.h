/*++
Copyright (c) 2001-2003 BSQUARE Corporation. All rights reserved.

Module:

    macaddr.h

Description:

	
    The MAC address for each of the MACs is contained here. 
    It should be unique, no attempt to track these addresses is
    made by Bsquare.
	Alchemy owns 00:50:C2:xx:yy:zz
    
Authors:

    GJS May 2001

Revision History

--*/
    

//
//  The MAC address for each board should be different
//  this is the bottom word
//

// The fixed part
#define PB1000_MAC_ADDR_UPPER		0x0050C2

// The variable part, use the time stamp for now FIXME
#define PB1000_MAC_ADDR_LOWER		(((unsigned long)__TIME__) & 0xff) \
                                | (((((unsigned long)__TIME__) >> 12) & 0xff) << 8) \
                                | (((((unsigned long)__TIME__) >> 24) & 0xff) << 12)

//
// MAC address storage area
// 4 consecutive words
//

typedef struct {
    union {
        struct {
            
            unsigned short Mac0[3];
            unsigned short Filler0;
            unsigned short Mac1[3];
            unsigned short Filler1;
        } AsShort;
        struct {
            unsigned long Mac0[2];
            unsigned long Mac1[2];        
        } AsLong;
    } u; 
} MACADDR_IN_FLASH;

#ifndef MAC_ADDR_BASE
#define MAC_ADDR_BASE 0x1fe00000
#endif

