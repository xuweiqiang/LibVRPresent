/*****************************************************************************
Copyright 2003-2009 RMI Corporation. All rights reserved.

Any transfer or redistribution of the source code, with or without modification,
IS PROHIBITED, unless prior written consent was obtained. Any transfer or
redistribution of the binary code for use on the RMI Alchemy Family,
with or without modification, is permitted, provided that the following
condition is met:

Redistributions in binary form must reproduce the above copyright notice,

this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution:

THIS SOFTWARE IS PROVIDED BY RMI Corporation 'AS IS' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL RMI OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#ifndef __DB13XX_H__
#define __DB13XX_H__

#pragma message ("")
#pragma message ("Platform support for db13xx included.")
#pragma message ("")

#define PLATFORM_DB13XX
#define CPU_AU13XX

#define PLATFORM_DESCRIPTION "RMI Alchemy Au1300/DbAu13xx"
#define EBOOT_NAME "DbAu13xx"

#define GPIO_CTL_INPUT 0
#define GPIO_CTL_DEV 1
#define GPIO_CTL_OUTPUT0 2
#define GPIO_CTL_OUTPUT1 3
#define GPIO_CTL(x) (*((volatile int*) (0xb0201000+4*x)) )

// Comment out to use the TOY as the ticker source.
//#define USE_CP0_TIMER 1

// This board has a 32kHz oscillator for the TOY/RTC
#define HAVE_32KHZ_OSC

// Define address of External LAN Chip
#define LAN9210_PHYS_ADDR			0x19000000
#define ETHERNET_PHYS_ADDR			LAN9210_PHYS_ADDR

// Defines for IDE phys addr and DMA Request
#define IDE_PHYS_ADDR				0x18800000
#define IDE_DMA_REQ					DDMA_REQ1_ID
#define PLATFORM_DMA1_REQ			(10)

// Select flash driver
#define HAVE_AMD_FLASH
#define FLASH_AM29GL256P
#define FLASH_BANKS 2
#define FLASH_WIDTH 16

// #define NAND_BOOT 1
#ifndef NAND_BOOT
	#define NAND_PHYS_ADDR 0x20000000
#else
	#define NAND_PHYS_ADDR 0x12000000
#endif

// FLASH on RCE0
//

#define FLASH_BASE                 0x1C000000
#define FLASH_SIZE               0x04000000 // 64 MB
#define BOOT_VECTOR_OFFSET         0x1fc00000 - FLASH_BASE

// Platform-specific selection of PSC functionality
#define PLATFORM_SPI_PSC	0
#define PLATFORM_AC97_PSC	PSC1_PHYS_ADDR
#define PLATFORM_I2S_PSC	2
#define PLATFORM_SMBUS_PSC	3

// change HEX LED display
#define PLATFORM_OEMINIT_CODE                      \
	{ HEXLED *hexled = (HEXLED*)HEXLED_KSEG1_ADDR; \
	hexled->leds = 0;                              \
	hexled->blank = 0; }

// Platform-specific definitions for kernel/hal/intr

#define PLATFORM_ISR_DEBUG_CODE    									       \
	{	if ( HWINTR_RTCMATCH0 != HwIntr) {								   \
			HEXLED * hexleds = (HEXLED*) (HEXLED_PHYSADDR + KSEG1_OFFSET); \
			hexleds->leds = (UCHAR)HwIntr; } }

#define PLATFORM_ISR_TICK_DEBUG_CODE 		\
	{	USHORT tmp;							\
		tmp = bcsr->leds & 0xFFFC;			\
		tmp |= (++TickCount >> 9) & 0x3;	\
		bcsr->leds = tmp;}

#define PLATFORM_WAKESRC	GPIO_N(0)

#define PLATFORM_WAKEFROMSLEEP_MASK SYS_WAKEMSK_GPIO0

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

#define PLATFORM_USBHOST_POWER_ENABLE \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;  \
	bcsr->resets |= BCSR_RESETS_USBHOST_PWR; }

#define PLATFORM_USBHOST_POWER_DISABLE \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;  \
	bcsr->resets &= ~BCSR_RESETS_USBHOST_PWR; }

#define PLATFORM_USBOTG_POWER_ENABLE \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;  \
	bcsr->resets |= BCSR_RESETS_OTG_PWR; }

#define PLATFORM_USBOTG_POWER_DISABLE \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;  \
	bcsr->resets &= ~BCSR_RESETS_OTG_PWR; }


// Wake up on GPIO0, switch S22
#define PLATFORM_WAKEUP_IRQ			HWINTR_GPIO(0)

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

#define PLATFORM_AUTOBOOT_STRING "Change S18.1 to ON for EBOOT, OFF for CLI"

// Platform-specific definitions for DRIVERS/DISPLAY/display.cpp
// Select which panel to configure
#define PLATFORM_LCD_PANEL_SELECT_CODE \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;   \
	panel = (bcsr->switches & BCSR_SWITCHES_ROTARY) >> 8; }

#define PLATFORM_LCD_POWERUP_CODE          \
    { BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;       \
    bcsr->specific |= (BCSR_SPECIFIC_LCDVEEOFF | \
                       	BCSR_SPECIFIC_LCDVDDOFF);  }

#define PLATFORM_LCD_POWERDOWN_CODE         \
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

#define HWINTR_EXT_BASE_HOOK	HWINTR_GPIO(5)
#define HWINTR_EXT_INTR_CFG 	irqHL

#define HWINTR_EXT_BASE_HOOK_CODE \
	if( HWINTR_EXT_BASE_HOOK == HwIntr )	       				   \
	{															   \
		BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;                       \
		ULONG Ext;												   \
		ULONG e0;												   \
		e0 = bcsr->intstatus;									   \
		Ext = getMSB(e0);										   \
		bcsr->intstatus = bcsr->intclrmask = (USHORT)(1<<Ext);	   \
																   \
		HwIntr = Ext+HWINTR_EXT_BASE;							   \
	}

#define PLATFORM_POWER_EXT_ISR_DATA \
	static USHORT PlatformExtIsrState = 0;

#define PLATFORM_POWER_EXT_ISR_SAVE_CODE  \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR; \
	PlatformExtIsrState = bcsr->intsetmask; }

#define PLATFORM_POWER_EXT_ISR_RESTORE_CODE \
	{ BCSR *bcsr = (BCSR*)BCSR_KSEG1_ADDR;   \
	bcsr->intclrmask = 0xFFFF;               \
	bcsr->intsetenable = 0xFFFF;             \
	bcsr->intstatus = 0xFFFF;                \
	bcsr->intsetmask = (USHORT)PlatformExtIsrState; }


//
// External board interrupt sources
//
#define HWINTR_EXT_IDE			(HWINTR_EXT_BASE+0)
#define HWINTR_EXT_ETH			(HWINTR_EXT_BASE+1)
#define HWINTR_EXT_PC0			(HWINTR_EXT_BASE+2)
#define HWINTR_EXT_VIDEO		(HWINTR_EXT_BASE+4)
#define HWINTR_EXT_HDMI			(HWINTR_EXT_BASE+5)
#define HWINTR_EXT_DC			(HWINTR_EXT_BASE+6)
#define HWINTR_EXT_FLASH		(HWINTR_EXT_BASE+7)
#define HWINTR_EXT_PC0INSERT	(HWINTR_EXT_BASE+8)
#define HWINTR_EXT_PC0EJECT		(HWINTR_EXT_BASE+9)
#define HWINTR_EXT_AC97			(HWINTR_EXT_BASE+10)
#define HWINTR_EXT_AC97_PEN		(HWINTR_EXT_BASE+11)
// Poorly named. on Db13xx this is actually SD1.
#define HWINTR_EXT_SD0INSERT	(HWINTR_EXT_BASE+12)
#define HWINTR_EXT_SD0EJECT		(HWINTR_EXT_BASE+13)
 #define HWINTR_EXT_SD1INSERT	HWINTR_EXT_SD0INSERT
 #define HWINTR_EXT_SD1EJECT		HWINTR_EXT_SD0EJECT

//
// SMBUS Devices
///
#define SMBUS_ADDR_WM8731	0x1B
#define SMBUS_ADDR_NE1619	0x2D
#define SMBUS_ADDR_VIDEO	0x42
#define SMBUS_ADDR_FOCUS	0x4A
#define SMBUS_ADDR_HDMI		0x72


//
// HEX LEDs, BCSR
//
#define HEXLED_PHYSADDR		0x19C00000
#define HEXLED_KSEG1_ADDR	0xB9C00000
#define BCSR_PHYSADDR		0x19800000
#define BCSR_KSEG1_ADDR		0xB9800000


/*
 * Overlay data structure of the DbAu13xx board registers.
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
} BCSR, *pBCSR,*PBCSR;

#define OTP_CONFIG0_HDR		(1 << 4)
#define OTP_CONFIG0_MGP		(1 << 3)
#define OTP_CONFIG0_GPE		(1 << 2)
#define OTP_CONFIG0_SDR		(1 << 1)
#define OTP_CONFIG0_AES		(1 << 0)

typedef volatile struct
{
	unsigned int config0;
	unsigned int ChipIDLo;
	unsigned int ChipIDHi;
} OTP, *pOTP,*POTP;
#endif

/*
 * Overlay data structure of the DbAu13xx HEX LED registers.
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
#define BCSR_WHOAMI_PACKAGE 0xF000

#define BCSR_STATUS_DCDMAREQ 	0x0010
#define BCSR_STATUS_IDEDMAREQ	0x0020
#define BCSR_STATUS_SWAPBOOT	0x0040
#define BCSR_STATUS_FLASHBUSY	0x0100
#define BCSR_STATUS_IDECBLID	0x0200
// Poorly named bitfield
#define BCSR_STATUS_SD0WP		0x0400
 #define BCSR_STATUS_SD1WP		BCSR_STATUS_SD0WP
#define BCSR_STATUS_INPACK		0x0800
#define BCSR_STATUS_OTG_OC		0x1000
#define BCSR_STATUS_USBHOST_OC	0x2000
#define BCSR_STATUS_PCWP		0x4000

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
#define BCSR_RESETS_VDDQ_SHDN	0x0200
#define BCSR_RESETS_OTP_PGM		0x0400
#define BCSR_RESETS_OTP_SCLK	0x0800
#define BCSR_RESETS_OTP_WPROT	0x1000
#define BCSR_RESETS_OTP_CSB		0x2000
#define BCSR_RESETS_OTG_PWR		0x4000
#define BCSR_RESETS_USBHOST_PWR 0x8000

#define BCSR_PCMCIA_PC0DRVEN	0x1000
#define BCSR_PCMCIA_PC0RST		0x8000

#define BCSR_SPECIFIC_LCDVEEOFF	0x0001
#define BCSR_SPECIFIC_LCDVDDOFF	0x0002
#define BCSR_SPECIFIC_LCDBLOFF	0x0004
#define BCSR_SPECIFIC_ETH_FIFO  0x0008
#define BCSR_SPECIFIC_CAM_CS	0x0010
#define BCSR_SPECIFIC_CAMPWRDOWN 0x0020

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
#define BCSR_BIC_CF				0x0004
 #define BCSR_BIC_PC0			BCSR_BIC_CF
 #define BCSR_BIC_PC1			0
#define BCSR_BIC_VIDEO			0x0010
#define BCSR_BIC_HDMI			0x0020
#define BCSR_BIC_DC				0x0040
#define BCSR_BIC_FLASH			0x0080
#define BCSR_BIC_PC0INSERT		0x0100
#define BCSR_BIC_PC0EJECT		0x0200
#define BCSR_BIC_AC97			0x0400
#define BCSR_BIC_AC97PEN		0x0800
// Poorly named. Actually SD1
#define BCSR_BIC_SD0INSERT		0x1000
#define BCSR_BIC_SD0EJECT		0x2000
 #define BCSR_BIC_SD1INSERT		BCSR_BIC_SD0INSERT
 #define BCSR_BIC_SD1EJECT		BCSR_BIC_SD0EJECT


#define MALI_PP_PHYS_ADDR		MALI_PHYS_ADDR
#define MALI_GP2_PHYS_ADDR		MALI_PHYS_ADDR + 0x2000

#define MALI_PP_ID_OFFSET		0x1000
#define MALI_GP2_ID_OFFSET		0x006C

#define	OTG_ID_OFFSET			0x0040
#define OTG_CONFIG1_OFFSET		0x0044	
#define OTG_CONFIG2_OFFSET		0x0048	
#define OTG_CONFIG3_OFFSET		0x004C	
#define OTG_CONFIG4_OFFSET		0x0050	


#define PERIPHERAL_INFO_3D_GP_ID		0x0A070004
#define PERIPHERAL_INFO_3D_PP_ID		0x08070004
#define PERIPHERAL_INFO_OTG_ID			0x4F54271A
#define PERIPHERAL_INFO_OTG_CONFIG1		0xA0EF002B
#define PERIPHERAL_INFO_OTG_CONFIG2		0x228CD050
#define PERIPHERAL_INFO_OTG_CONFIG3		0x05F60CE8
#define PERIPHERAL_INFO_OTG_CONFIG4		0x13F08030
#define PERIPHERAL_INFO_CP0_CONFIG1		0x05030776


#endif // __DB13XX_H__

