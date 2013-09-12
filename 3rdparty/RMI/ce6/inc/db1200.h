/*
Copyright:

    Copyright (c) 2004,2005 BSQUARE Corporation. All rights reserved.

Module Name:

    db1200.h

Abstract:

    Platform-specific definitions

Author:

*/

#ifndef __DB1200_H__
#define __DB1000_H__

#define PLATFORM_DB1200
#define CPU_AU1200

#define PLATFORM_DESCRIPTION "RMI Alchemy Au1200/Db1200"
#define EBOOT_NAME "DB1200"

// Comment out to use the TOY as the ticker source.
//#define USE_CP0_TIMER 1

// This board has a 32kHz oscillator for the TOY/RTC
#define HAVE_32KHZ_OSC

#define ETHERNET_IS_DAUGHTERCARD	1
#define LAN9210_PHYS_ADDR			0x1A000000
#define ETHERNET_PHYS_ADDR			LAN9210_PHYS_ADDR

// Define address of SMSC LAN91C111 chip
//#define LAN91C111_PHYS_ADDR			0x19000300
//#define ETHERNET_PHYS_ADDR			LAN91C111_PHYS_ADDR

// Defines for IDE phys addr and DMA Request
#define IDE_PHYS_ADDR				0x18800000
#define IDE_DMA_REQ					DDMA_REQ1_ID

// Select flash driver
#define HAVE_AMD_FLASH
#define FLASH_AM29LV256M
#define FLASH_BANKS 2
#define FLASH_WIDTH 16

// FLASH on RCE0
//
#define FLASH_BASE                 0x1C000000
#define FLASH_SIZE                 0x04000000
#define BOOT_VECTOR_OFFSET         0x1fc00000 - FLASH_BASE

// Platform-specific selection of PSC functionality
#define PLATFORM_SPI_PSC	0
#define PLATFORM_SMBUS_PSC	0
#define PLATFORM_I2S_PSC	1
#define PLATFORM_AC97_PSC	1

// Platform-specific hooks to select PSC muxes
#define PLATFORM_SPI_PSC_INIT_CODE          \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;  \
	bcsr->resets |= BCSR_RESETS_PSC0MUXSEL; \
	bcsr->resets &= ~BCSR_RESETS_SPIDEVSEL;}

#define PLATFORM_SMBUS_PSC_INIT_CODE          \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;    \
	bcsr->resets &= ~BCSR_RESETS_PSC0MUXSEL; }

#define PLATFORM_I2S_PSC_INIT_CODE           \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;   \
	bcsr->resets |= BCSR_RESETS_PSC1MUXSEL; }

#define PLATFORM_AC97_PSC_INIT_CODE           \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;    \
	bcsr->resets &= ~BCSR_RESETS_PSC1MUXSEL; }



// Platform-specific interrupt defintions for kernel/hal/intr.c
#define PLATFORM_INTR_IC1_07 irqLL | INTR_MODE_UNMASK // 39 GPIO7 Board Interrupt Controller

// change HEX LED display
#define PLATFORM_OEMINIT_CODE                      \
	{ HEXLED *hexled = (HEXLED*)HEXLED_KSEG1_ADDR; \
	hexled->leds = 0;                              \
	hexled->blank = 0; }

// Platform-specific definitions for kernel/hal/isr.s

#define PLATFORM_ISR_DEBUG(x) \
	li		a3,HEXLED_KSEG1_ADDR ; \
	li		a2, x				 ; \
	sh		a2,0(a3)             ; \
	sync


#define PLATFORM_ISR_DEBUG_CODE    \
	{	if ( HWINTR_RTCMATCH0 != HwIntr ) {								   \
			HEXLED * hexleds = (HEXLED*) (HEXLED_PHYSADDR + KSEG1_OFFSET); \
			hexleds->leds = (UCHAR)HwIntr; } }

#define PLATFORM_ISR_TICK_DEBUG_CODE \
	{	USHORT tmp;							\
		tmp = bcsr->leds & 0xFFFC;			\
		tmp |= (++TickCount >> 9) & 0x3;	\
		bcsr->leds = tmp;}

// Platform-specific definitions for pwrmng.s
// turn off LEDs when entering sleep
#define PLATFORM_OEMPOWEROFF_PRESLEEP_CODE \
	lui		t0, 0xB9C0                   ; \
	ori		t1, zero, 0x03               ; \
	sh		t1, 0x8(t0)                  ; \
	sync                                 ; \
	nop

// turn on LEDs when leaving sleep
#define PLATFORM_OEMPOWEROFF_POSTSLEEP_CODE \
	lui		t0, 0xB9C0                    ; \
	sh		zero, 0x8(t0)                 ; \
	sync                                  ; \
	nop


// Wake up on GPIO5, switch S4
#define PLATFORM_WAKEFROMSLEEP_MASK SYS_WAKEMSK_GPIO5

// Platform-specific definitions for OEMIdle()
// turn on LED when entering IDLE
#define PLATFORM_OEMIDLE_PREIDLE_DEBUG_CODE \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;  \
	bcsr->leds &= ~BCSR_LEDS_LED0; }

// turn off LED when exiting IDLE
#define PLATFORM_OEMIDLE_POSTIDLE_DEBUG_CODE \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;   \
	bcsr->leds |= BCSR_LEDS_LED0; }

// Platform-specific definitions for eboot/eboot/oemfuncs.c
#define PLATFORM_OEMSHOWPROGRESS_CODE              \
	{ HEXLED *hexled = (HEXLED*)HEXLED_KSEG1_ADDR; \
	hexled->leds = (UCHAR)dwPacketNum; }

// Platform-specific definitions for eboot/common/ldrkrnl/main.c
// Check Switch S12.8, if ON, then autoboot
#define PLATFORM_AUTOBOOT_CODE               \
	{ BCSR *bcsr = (BCSR *)BCSR_KSEG1_ADDR;  \
	if (bcsr->switches & BCSR_SWITCHES_DIP_8) return FALSE; }

#define PLATFORM_AUTOBOOT_STRING "Change S6.8 to ON for EBOOT, OFF for CLI"

// Platform-specific definitions for DRIVERS/DISPLAY/Au1200LCD/lcdgpe.cpp
// Select which panel to configure, rotary switch S11
#define PLATFORM_AU1200LCD_PANEL_SELECT_CODE \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;   \
	panel = (bcsr->switches & BCSR_SWITCHES_ROTARY) >> 8; }

#define PLATFORM_AU1200LCD_POWERUP_CODE          \
    { BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;       \
    bcsr->specific |= (BCSR_SPECIFIC_LCDVEEOFF | \
                       	BCSR_SPECIFIC_LCDVDDOFF);  }

#define PLATFORM_AU1200LCD_POWERDOWN_CODE         \
    { BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;        \
    bcsr->specific &= ~(BCSR_SPECIFIC_LCDVEEOFF | \
                        	BCSR_SPECIFIC_LCDVDDOFF);  }

#define PLATFORM_BACKLIGHT_POWERUP_CODE          \
    { BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;       \
    bcsr->specific |= BCSR_SPECIFIC_LCDBLOFF;  }

#define PLATFORM_BACKLIGHT_POWERDOWN_CODE          \
    { BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;       \
    bcsr->specific &= ~BCSR_SPECIFIC_LCDBLOFF;  }

#define PLATFORM_BACKLIGHT_STATUS_CODE		\
   {BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;	\
   if(bcsr->specific & BCSR_SPECIFIC_LCDBLOFF) return D0;   \
   	else return D4;}

// Platform-specific definitions for amplifier power management.
// These hooks support adding an amplifier to the device.
// Add custom code here.
#define PLATFORM_AMPLIFIER_POWERDOWN_CODE

#define PLATFORM_AMPLIFIER_POWERUP_CODE

//
// External board interrupt handling hooks
//
#define PLATFORM_INTR_EXTERNAL_INIT_CODE   \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR; \
	bcsr->intclrmask = 0xFFFF;             \
	bcsr->intsetenable = 0xFFFF;           \
	bcsr->intstatus = 0xFFFF; }

#define	PLATFORM_INTR_EXTERNAL_ENABLE_CODE \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR; \
	bcsr->intsetmask = (USHORT)LocalSysIntrMap[SysIntr].ExtMask; }

#define PLATFORM_INTR_EXTERNAL_DISABLE_CODE	\
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;  \
	bcsr->intclrmask = (USHORT)LocalSysIntrMap[SysIntr].ExtMask; }

#define HWINTR_EXT_BASE_HOOK	HWINTR_GPIO7
#define HWINTR_EXT_BASE_HOOK_CODE \
	if( HWINTR_EXT_BASE_HOOK == HwIntr )						   \
	{															   \
		ULONG Ext;												   \
		e0 = bcsr->intstatus;									   \
																   \
		Ext = getMSB(e0);										   \
		bcsr->intstatus = bcsr->intclrmask = (USHORT)(1<<Ext);	   \
																   \
		HwIntr = Ext+HWINTR_EXT_BASE;							   \
	}

#define PLATFORM_POWER_EXT_ISR_SAVE_CODE  \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR; \
	PlatSpecificTemp = bcsr->intsetmask; }

#define PLATFORM_POWER_EXT_ISR_RESTORE_CODE \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;   \
	bcsr->intclrmask = 0xFFFF;               \
	bcsr->intsetenable = 0xFFFF;             \
	bcsr->intstatus = 0xFFFF;                \
	bcsr->intsetmask = (USHORT)PlatSpecificTemp; }


//
// External board interrupt sources
//
#define HWINTR_EXT_IDE			(HWINTR_EXT_BASE+0)
#define HWINTR_EXT_ETH			(HWINTR_EXT_BASE+1)
#define HWINTR_EXT_PC0			(HWINTR_EXT_BASE+2)
#define HWINTR_EXT_PC0STSCHG	(HWINTR_EXT_BASE+3)
#define HWINTR_EXT_PC1			(HWINTR_EXT_BASE+4)
#define HWINTR_EXT_PC1STSCHG	(HWINTR_EXT_BASE+5)
#define HWINTR_EXT_DC			(HWINTR_EXT_BASE+6)
#define HWINTR_EXT_FLASH		(HWINTR_EXT_BASE+7)
#define HWINTR_EXT_PC0INSERT	(HWINTR_EXT_BASE+8)
#define HWINTR_EXT_PC0EJECT		(HWINTR_EXT_BASE+9)
#define HWINTR_EXT_PC1INSERT	(HWINTR_EXT_BASE+10)
#define HWINTR_EXT_PC1EJECT		(HWINTR_EXT_BASE+11)
#define HWINTR_EXT_SD0INSERT	(HWINTR_EXT_BASE+12)
#define HWINTR_EXT_SD0EJECT		(HWINTR_EXT_BASE+13)

//
// SMBUS Devices
///
#define SMBUS_ADDR_WM8731	0x1B
#define SMBUS_ADDR_MAX3353	0x2C
#define SMBUS_ADDR_NE1619	0x2D
#define SMBUS_ADDR_EEPROM	0x52

//
// HEX LEDs, BCSR
//
#define HEXLED_PHYSADDR		0x19C00000
#define HEXLED_KSEG1_ADDR	0xB9C00000
#define BCSR_PHYSADDR		0x19800000
#define BCSR_KSEG1_ADDR		0xB9800000


/*
 * Overlay data structure of the Db1200 board registers.
 * Registers located at physical 0x198000xx, KSEG1 0xB98000xx
 */
#ifndef ASSEMBLER
typedef volatile struct
{
	/*00*/	unsigned short whoami;
			unsigned short reserved0;
	/*04*/	unsigned short status;
			unsigned short reserved1;
	/*08*/	unsigned short switches;
			unsigned short reserved2;
	/*0C*/	unsigned short resets;
			unsigned short reserved3;
	/*10*/	unsigned short pcmcia;
			unsigned short reserved4;
	/*14*/	unsigned short specific;
			unsigned short reserved5;
	/*18*/	unsigned short leds;
			unsigned short reserved6;
	/*1C*/	unsigned short system;
			unsigned short reserved7;
	/*20*/	unsigned short intclrenable;
			unsigned short reserved8;
	/*24*/	unsigned short intsetenable;
			unsigned short reserved9;
	/*28*/	unsigned short intclrmask;
			unsigned short reserved10;
	/*2C*/	unsigned short intsetmask;
			unsigned short reserved11;
	/*30*/	unsigned short sigstatus;
			unsigned short reserved12;
	/*34*/	unsigned short intstatus;
			unsigned short reserved13;
} BCSR, *pBCSR, *PBCSR;
#endif

/*
 * Overlay data structure of the Db1200 HEX LED registers.
 * Registers located at physical 0x19C000xx, KSEG1 0xB9C000xx
 */
#ifndef ASSEMBLER
typedef volatile struct
{
	/*00*/	unsigned char  leds;
			unsigned char  reserved[7];
	/*08*/	unsigned short blank;
} HEXLED, *pHEXLED;
#endif

/*
 * Register bit definitions for the BCSRs
 */
#define BCSR_WHOAMI_DCID	0x000F
#define BCSR_WHOAMI_CPLD	0x00F0
#define BCSR_WHOAMI_BOARD	0x0F00

#define BCSR_STATUS_PCMCIA0VS	0x0003
#define BCSR_STATUS_PCMCIA1VS	0x000C
#define BCSR_STATUS_SWAPBOOT	0x0040
#define BCSR_STATUS_FLASHBUSY	0x0100
#define BCSR_STATUS_IDECBLID	0x0200
#define BCSR_STATUS_SD0WP		0x0400
#define BCSR_STATUS_U0RXD		0x1000
#define BCSR_STATUS_U3RXD		0x2000

#define BCSR_SWITCHES_OCTAL		0x00FF
#define BCSR_SWITCHES_DIP_1		0x0080
#define BCSR_SWITCHES_DIP_2		0x0040
#define BCSR_SWITCHES_DIP_3		0x0020
#define BCSR_SWITCHES_DIP_4		0x0010
#define BCSR_SWITCHES_DIP_5		0x0008
#define BCSR_SWITCHES_DIP_6		0x0004
#define BCSR_SWITCHES_DIP_7		0x0002
#define BCSR_SWITCHES_DIP_8		0x0001
#define BCSR_SWITCHES_ROTARY	0x0F00

#define BCSR_RESETS_ETHERNET	0x0001
#define BCSR_RESETS_CAMERA		0x0002
#define BCSR_RESETS_DC			0x0004
#define BCSR_RESETS_IDE			0x0008
#define BCSR_RESETS_TV			0x0010
#define BCSR_RESETS_PWM1RIMUXSEL	0x1000
#define BCSR_RESETS_PSC0MUXSEL	0x1000
#define BCSR_RESETS_PSC1MUXSEL	0x2000
#define BCSR_RESETS_SPIDEVSEL	0x4000

#define BCSR_PCMCIA_PC0VPP		0x0003
#define BCSR_PCMCIA_PC0VCC		0x000C
#define BCSR_PCMCIA_PC0DRVEN	0x0010
#define BCSR_PCMCIA_PC0RST		0x0080
#define BCSR_PCMCIA_PC1VPP		0x0300
#define BCSR_PCMCIA_PC1VCC		0x0C00
#define BCSR_PCMCIA_PC1DRVEN	0x1000
#define BCSR_PCMCIA_PC1RST		0x8000

#define BCSR_SPECIFIC_LCDVEEOFF	0x0001
#define BCSR_SPECIFIC_LCDVDDOFF	0x0002
#define BCSR_SPECIFIC_LCDBLOFF	0x0004
#define BCSR_SPECIFIC_CAMSNAPSHOT	0x0010
#define BCSR_SPECIFIC_CAMPWRDOWN	0x0020
#define BCSR_SPECIFIC_SD0PWR	0x0040

#define BCSR_LEDS_DECIMALS		0x0003
#define BCSR_LEDS_LED0			0x0100
#define BCSR_LEDS_LED1			0x0200
#define BCSR_LEDS_LED2			0x0400
#define BCSR_LEDS_LED3			0x0800

#define BCSR_SYSTEM_POWEROFF	0x4000
#define BCSR_SYSTEM_RESET		0x8000

/* Board Interrupt Controller Registers */
#define BCSR_BIC_IDE			0x0001
#define BCSR_BIC_ETH			0x0002
#define BCSR_BIC_PC0			0x0004
#define BCSR_BIC_PC0STSCHG		0x0008
#define BCSR_BIC_PC1			0x0010
#define BCSR_BIC_PC1STSCHG		0x0020
#define BCSR_BIC_DC				0x0040
#define BCSR_BIC_FLASH			0x0080
#define BCSR_BIC_PC0INSERT		0x0100
#define BCSR_BIC_PC0EJECT		0x0200
#define BCSR_BIC_PC1INSERT		0x0400
#define BCSR_BIC_PC1EJECT		0x0800
#define BCSR_BIC_SD0INSERT		0x1000
#define BCSR_BIC_SD0EJECT		0x2000

#endif // __DB1200_H__
