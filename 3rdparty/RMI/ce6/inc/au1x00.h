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

#ifndef _AU1X00_H
#define _AU1X00_H

/***********************************************************************/

/*
 * CP0 registers
 */
#define CP0_Index		$0
#define CP0_Random		$1
#define CP0_EntryLo0	$2
#define CP0_EntryLo1	$3
#define CP0_Context		$4
#define CP0_PageMask	$5
#define CP0_Wired		$6
#define CP0_BadVAddr	$8
#define CP0_Count		$9
#define CP0_EntryHi		$10
#define CP0_Compare		$11
#define CP0_Status		$12
#define CP0_Cause		$13
#define CP0_EPC			$14
#define CP0_PRId		$15
#define CP0_Config		$16
#define CP0_Config0		$16
#define CP0_Config1		$16,1
#define CP0_LLAddr		$17
#define CP0_WatchLo		$18
#define CP0_IWatchLo	$18,1
#define CP0_WatchHi		$19
#define CP0_IWatchHi	$19,1
#define CP0_Scratch		$22
#define CP0_Debug		$23
#define CP0_DEPC		$24
#define CP0_PerfCnt		$25
#define CP0_PerfCtrl	$25,1
#define CP0_DTag		$28
#define CP0_DData		$28,1
#define CP0_ITag		$29
#define CP0_IData		$29,1
#define CP0_ErrorEPC	$30
#define CP0_DESave		$31

#define CP0_PRID_REV	0x0000000F
#define CP0_PRID_IMP	0x000000F0
#define CP0_SR_IE		0x00000001

/***********************************************************************/


#if defined(SOC_AU1000)||defined(SOC_AU1100)||defined(SOC_AU1500)

/*
 * AC97 Register Offsets
 */
#define AC97_CONFIG					(0x0000)
#define AC97_STATUS					(0x0004)
#define AC97_DATA					(0x0008)
#define AC97_CMMD					(0x000C)
#define	AC97_CMMDRESP				(0x000C)
#define AC97_ENABLE					(0x0010)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 config;
	uint32 status;
	uint32 data;
	uint32 cmmd;
#define cmmdresp cmmd
	uint32 enable;

} AU1X00_AC97;
#endif

/*
 * Register content definitions
 */
#define AC97_CONFIG_RC				(0x3FF<<13)
#define AC97_CONFIG_XS				(0x3FF<<3)
#define AC97_CONFIG_SG				(1<<2)
#define AC97_CONFIG_SN				(1<<1)
#define AC97_CONFIG_RS				(1<<0)
#define AC97_CONFIG_XS_N(N)			(N<<3)

#define AC97_STATUS_XU				(1<<11)
#define AC97_STATUS_XO				(1<<10)
#define AC97_STATUS_RU				(1<<9)
#define AC97_STATUS_RO				(1<<8)
#define AC97_STATUS_RD				(1<<7)
#define AC97_STATUS_CP				(1<<6)
#define AC97_STATUS_TE				(1<<4)
#define AC97_STATUS_TF				(1<<3)
#define AC97_STATUS_RE				(1<<1)
#define AC97_STATUS_RF				(1<<0)

#define AC97_CMMD_WRITEDATA			(0xFFFF<<16)
#define AC97_CMMD_RW				(1<<7)
#define AC97_CMMD_INDEX				(0x3F<<0)
#define AC97_CMMD_WRITEDATA_N(N)	(N<<16)
#define AC97_CMMD_RW_RD				(1<<7)
#define AC97_CMMD_RW_WR				(0<<7)
#define AC97_CMMD_INDEX_N(N)		(N<<0)

#define AC97_ENABLE_D				(1<<1)
#define AC97_ENABLE_CE				(1<<0)

/********************************************************************/

/*
 * DMA Register Offsets (from DMA module)
 */

#define	DMA_MODEREAD				(0x0000)
#define DMA_MODESET					(0x0000)
#define DMA_MODECLR					(0x0004)
#define DMA_PERADDR					(0x0008)
#define DMA_BUF0ADDR				(0x000C)
#define DMA_BUF0SIZE				(0x0010)
#define DMA_BUF1ADDR				(0x0014)
#define DMA_BUF1SIZE				(0x0018)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 moderead;
#define modeset moderead
	uint32 modeclr;
	uint32 peraddr;
	uint32 buf0addr;
	uint32 buf0size;
	uint32 buf1addr;
	uint32 buf1size;
	uint32 reserved[57];
} AU1X00_DMA;
#endif

/*
 * DMA_MODESET/DMA_MODECLR register content definitions
 */
#define DMA_MODE_DAH					(15<<20)
#define DMA_MODE_DID					(15<<16)
#define DMA_MODE_DS						(1<<15)
#define DMA_MODE_BE						(1<<13)
#define DMA_MODE_DR						(1<<12)
#define DMA_MODE_TS						(1<<11)
#define DMA_MODE_DW						(3<<9)
#define DMA_MODE_DW_8					(0<<9)
#define DMA_MODE_DW_16					(1<<9)
#define DMA_MODE_DW_32					(2<<9)
#define DMA_MODE_NC						(1<<8)
#define DMA_MODE_IE						(1<<7)
#define DMA_MODE_H						(1<<6)
#define DMA_MODE_G						(1<<5)
#define DMA_MODE_AB						(1<<4)
#define DMA_MODE_D1						(1<<3)
#define DMA_MODE_BE1					(1<<2)
#define DMA_MODE_D0						(1<<1)
#define DMA_MODE_BE0					(1<<0)
#define DMA_MODE_DAH_N(N)				(N<<20)
#define DMA_MODE_DID_N(N)				(N<<16)

// DMA Device IDs when DMA_MODE_DS==0
#define DMA_MODE_DID_UART0_TX			0
#define DMA_MODE_DID_UART0_RX			1
#define DMA_MODE_DID_GP04				2
#define DMA_MODE_DID_GP05				3
#define DMA_MODE_DID_AC97_TX			4
#define DMA_MODE_DID_AC97_RX			5
#define DMA_MODE_DID_UART3_TX			6
#define DMA_MODE_DID_UART3_RX			7
#define DMA_MODE_DID_USB_EP0_RX			8
#define DMA_MODE_DID_USB_EP0_TX			9
#define DMA_MODE_DID_USB_EP1_TX			10
#define DMA_MODE_DID_USB_EP2_TX			11
#define DMA_MODE_DID_USB_EP3_RX			12
#define DMA_MODE_DID_USB_EP4_RX			13
#define DMA_MODE_DID_I2S_TX				14
#define DMA_MODE_DID_I2S_RX				15

// DMA Device IDs when DMA_MODE_DS==1
#define DMA_MODE_DID_SD0_TX				0
#define DMA_MODE_DID_SD0_RX				1
#define DMA_MODE_DID_SD1_TX				2
#define DMA_MODE_DID_SD1_RX				3

/*
 * DMA_BUFSIZE register content definitions
 */
#define DMA_BUFSIZE_BAH			(15<<15)
#define DMA_BUFSIZE_COUNT			(65535<<0)
#define DMA_BUFSIZE_BAH_N(N)		(N<<15)
#define DMA_BUFSIZE_COUNT_N(N)		(N<<0)

#define NUM_DMA_CHANNELS		8

#endif // #if defined(SOC_xyz)


/***********************************************************************/


#if defined(SOC_AU1000)||defined(SOC_AU1100)||defined(SOC_AU1500)||defined(SOC_AU1550)||defined(SOC_AU1200)

/*
 * GPIO2 Register Offsets
 */

#define GPIO2_DIR		0x0000
#define GPIO2_OUTPUT	0x0008
#define GPIO2_PINSTATE	0x000C
#define GPIO2_INTEN		0x0010
#define GPIO2_ENABLE	0x0014

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 dir;
	uint32 reserved;
	uint32 output;
	uint32 pinstate;
	uint32 inten;
	uint32 enable;

} AU1X00_GPIO2;
#endif

/*
 * Register content definitions
 */
#define GPIO2_GPIO200	0x0001
#define GPIO2_GPIO201	0x0002
#define GPIO2_GPIO202	0x0004
#define GPIO2_GPIO203	0x0008
#define GPIO2_GPIO204	0x0010
#define GPIO2_GPIO205	0x0020
#define GPIO2_GPIO206	0x0040
#define GPIO2_GPIO207	0x0080
#define GPIO2_GPIO208	0x0100
#define GPIO2_GPIO209	0x0200
#define GPIO2_GPIO210	0x0400
#define GPIO2_GPIO211	0x0800
#define GPIO2_GPIO212	0x1000
#define GPIO2_GPIO213	0x2000
#define GPIO2_GPIO214	0x4000
#define GPIO2_GPIO215	0x8000

#define GPIO2_ENABLE_MR	0x0002
#define GPIO2_ENABLE_CE	0x0001

#define GPIO2_DIR_DATA		(0)
#define	GPIO2_DIR_DATA_MSK	(0xFFFF<<GPIO2_DIR_DATA)
#define	GPIO2_DIR_ENA		(16)
#define	GPIO2_DIR_ENA_MSK	(0xFFFF<<GPIO2_DIR_ENA)

#endif // #if defined( SOC_)

/***********************************************************************/
#if defined(SOC_AU1100)

/*
 * I2S Register Offsets
 */
#define I2S_DATA					(0x0000)
#define I2S_CONFIG					(0x0004)
#define I2S_ENABLE					(0x0008)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 data;
	uint32 config;
	uint32 enable;

} AU1X00_I2S;
#endif

/*
 * i2s_config_status register content definitions
 */
#define I2S_CONFIG_XU		(1<<25)
#define I2S_CONFIG_XO		(1<<24)
#define I2S_CONFIG_RU		(1<<23)
#define I2S_CONFIG_RO		(1<<22)
#define I2S_CONFIG_TR		(1<<21)
#define I2S_CONFIG_TE		(1<<20)
#define I2S_CONFIG_TF		(1<<19)
#define I2S_CONFIG_RR		(1<<18)
#define I2S_CONFIG_RE		(1<<17)
#define I2S_CONFIG_RF		(1<<16)
#define I2S_CONFIG_PD		(1<<11)
#define I2S_CONFIG_LB		(1<<10)
#define I2S_CONFIG_IC		(1<<9)
#define I2S_CONFIG_FM		(3<<7)
#define I2S_CONFIG_TN		(1<<6)
#define I2S_CONFIG_RN		(1<<5)
#define I2S_CONFIG_SZ		(31<<0)
#define I2S_CONFIG_FM_I2S	(0<<7)
#define I2S_CONFIG_FM_LJ	(1<<7)
#define I2S_CONFIG_FM_RJ	(2<<7)
#define I2S_CONFIG_SZ_8		(8<<0)
#define I2S_CONFIG_SZ_16	(16<<0)
#define I2S_CONFIG_SZ_18	(18<<0)
#define I2S_CONFIG_SZ_20	(20<<0)
#define I2S_CONFIG_SZ_24	(24<<0)
#define I2S_CONFIG_SZ_N(N)	(N<<0)

/*
 * I2S_ENABLE register content definitions
 */
#define I2S_ENABLE_D				(1<<1)
#define I2S_ENABLE_CE				(1<<0)

#endif // #if defined(SOC_AU1100)

/***********************************************************************/

#if defined(SOC_AU1000)||defined(SOC_AU1100)||defined(SOC_AU1500)||defined(SOC_AU1550)||defined(SOC_AU1200)

/*
 * IC Register Offsets
 */

#define	IC_CFG0RD		(0x0040)
#define	IC_CFG0SET		(0x0040)
#define	IC_CFG0CLR		(0x0044)
#define	IC_CFG1RD		(0x0048)
#define	IC_CFG1SET		(0x0048)
#define	IC_CFG1CLR		(0x004C)
#define	IC_CFG2RD		(0x0050)
#define	IC_CFG2SET		(0x0050)
#define	IC_CFG2CLR		(0x0054)
#define	IC_REQ0INT		(0x0054)
#define	IC_SRCRD		(0x0058)
#define	IC_SRCSET		(0x0058)
#define	IC_SRCCLR		(0x005C)
#define	IC_REQ1INT		(0x005C)
#define	IC_ASSIGNRD		(0x0060)
#define	IC_ASSIGNSET	(0x0060)
#define	IC_ASSIGNCLR	(0x0064)
#define	IC_WAKERD		(0x0068)
#define	IC_WAKESET		(0x0068)
#define	IC_WAKECLR		(0x006C)
#define	IC_MASKRD		(0x0070)
#define	IC_MASKSET		(0x0070)
#define	IC_MASKCLR		(0x0074)
#define	IC_RISINGRD		(0x0078)
#define	IC_RISINGCLR	(0x0078)
#define	IC_FALLINGRD	(0x007C)
#define	IC_FALLINGCLR	(0x007C)
#define IC_TESTBIT		(0x0080)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 reserved[0x40>>2];
	uint32 cfg0rd;
#define cfg0set cfg0rd
	uint32 cfg0clr;
	uint32 cfg1rd;
#define cfg1set cfg1rd
	uint32 cfg1clr;
	uint32 cfg2rd;
#define cfg2set cfg2rd
	uint32 cfg2clr;
#define req0int cfg2clr
	uint32 srcrd;
#define srcset srcrd
	uint32 srcclr;
#define req1int srcclr
	uint32 assignrd;
#define assignset assignrd
	uint32 assignclr;
	uint32 wakerd;
#define wakeset wakerd
	uint32 wakeclr;
	uint32 maskrd;
#define maskset maskrd
	uint32 maskclr;
	uint32 risingrd;
#define risingclr risingrd
	uint32 fallingrd;
#define fallingclr fallingrd
	uint32 testbit;

} AU1X00_IC;
#endif
#define INTR_AU1X00_IC 1

#endif // defined(SOC_AU1000) ...

/*********************************************************************************************
****                         Au1XXX GPINT Register Definitions                            ****
*********************************************************************************************/
#if defined(SOC_AU13XX)

/*
 *	There are a total 128 'channels' defined by the Au13xx databook. However, this requires
 *  4 sperate 32bit registers for programming. Each register is called a 'bank' for ease
 *  of use.
 */

#define GPINT_BANK0		0
#define GPINT_BANK1		1
#define GPINT_BANK2		2
#define GPINT_BANK3		3

#define GPINT_NUM_BANKS	4 /* 0-3 */
#define GPINT_MAX_BANK	(GPINT_BANK3)

#define GPINT_GPIO_PER_BANK		32
#define GPINT_INTS_PER_BANK		GPINT_GPIO_PER_BANK

/* Total number of interrupts our architecture allows */
#define GPINT_MAX_INTS				(GPINT_NUM_BANKS*GPINT_INTS_PER_BANK)

/* Current maximum supported GPIO/INTERRUPTs */
#define GPINT_NUM_GPIO				GPINT_MAX_INTS
#define GPINT_NUM_INTERRUPTS		GPINT_MAX_INTS

/* Starting GPIO/INTERRUPT for each channel
	-- channels 3,4 not used at this time
*/
#define GPINT_BANK0_START		0
#define GPINT_BANK1_START		32
#define GPINT_BANK2_START		64
#define GPINT_BANK3_START		96

#define GPINT_BANK_FROM_GPIO(n)		(n>>5)	// divide by 32 to get bank
#define GPINT_BANK_FROM_INT(n)		GPINT_BANK_FROM_GPIO(n)
#define GPINT_BIT_FROM_GPIO(b,n) 	(1<<(n-(b<<5))) // multiply by 32 to get base
#define GPINT_BIT_FROM_INT(b,n) 	GPINT_BIT_FROM_GPIO(b,n)

#ifndef ASSEMBLER
typedef volatile struct
{
    /* R/W1S */
// unsigned int pin_val0;  // 0x00
// unsigned int pin_val1;  // 0x04
// unsigned int pin_val2;  // 0x08
// unsigned int pin_val3;  // 0x0C
    unsigned int    pin_val[GPINT_NUM_BANKS];

    /* W1C */
// unsigned int pin_valclr0  // 0x10
// unsigned int pin_valclr1; // 0x14
// unsigned int pin_valclr2; // 0x18
// unsigned int pin_valclr3; // 0x1C
    unsigned int    pin_valclr[GPINT_NUM_BANKS];

    /* R/W1C */
// unsigned int int_pend0;  // 0x20
// unsigned int int_pend1;  // 0x24
// unsigned int int_pend2;  // 0x28
// unsigned int int_pend3;  // 0x2c
    unsigned int int_pend[GPINT_NUM_BANKS];

    unsigned int pri_enc;  // 0x30
    unsigned int _resvd0[3];  // 0x34-0x3c

    /* R/W1S */
// unsigned int int_mask0;  // 0x40
// unsigned int int_mask1;  // 0x44
// unsigned int int_mask2;  // 0x48
// unsigned int int_mask3;  // 0x4c
    unsigned int int_mask[GPINT_NUM_BANKS];

    /* W1C */
// unsigned int int_maskclr0; // 0x50
// unsigned int int_maskclr1; // 0x54
// unsigned int int_maskclr2; // 0x58
// unsigned int int_maskclr3; // 0x5C
    unsigned int int_maskclr[GPINT_NUM_BANKS];

    /* R/W */
    unsigned int dma_sel;  // 0x60
    unsigned int _resvd1[(0x80-0x64)/4];  // 0x64-0x7C

    /* W */
// unsigned int    dev_sel0;  // 0x80
// unsigned int    dev_sel1;  // 0x84
// unsigned int    dev_sel2;  // 0x88
// unsigned int    dev_sel3;  // 0x8C
    unsigned int    dev_sel[GPINT_NUM_BANKS];

    /* W */
// unsigned int    dev_selclr0; // 0x90
// unsigned int    dev_selclr1; // 0x94
// unsigned int    dev_selclr2; // 0x98
// unsigned int    dev_selclr3; // 0x9C
    unsigned int    dev_selclr[GPINT_NUM_BANKS];

    /* R */
// unsigned int    reset_val0;  // 0xA0
// unsigned int    reset_val1;  // 0xA4
// unsigned int    reset_val2;  // 0xA8
// unsigned int    reset_val3;  // 0xAC
    unsigned int    reset_val[GPINT_NUM_BANKS];

    unsigned int _resvd2[(0x1000-0xB0)/4]; // 0xB0-0xFFC

    /* R/W -- when interrupt mask is clear */
    /* R   -- when interrupt mask is set */
// unsigned int gp_int0;  // 0x1000
// unsigned int gp_int1;  // 0x1004
// unsigned int gp_int2;  // 0x1008
// unsigned int gp_int2;  // 0x100C
// unsigned int gp_intN;  // 0x1000 + (N*4)
    unsigned int gp_int[GPINT_MAX_INTS];
} AU13XX_GPINT, *PAU13XX_GPINT;

#endif // ASSEMBLER
#define INTR_AU13XX_GPINT 1

#define GPINT_DMASEL_DMA0			(8)
#define GPINT_DMASEL_DMA0_N(n)		(((n)&0xFF)<<GPINT_DMASEL_DMA0)
#define GPINT_DMASEL_DMA1			(0)
#define GPINT_DMASEL_DMA1_N(n)		(((n)&0xFF)<<GPINT_DMASEL_DMA1)

#define GPINT_PINCTL				(0)
#define GPINT_PINCTL_N(n)			(((n)&0x3)<<GPINT_PINCTL)
#define GPINT_PINCTL_GPIOINPUT		GPINT_PINCTL_N(0)
#define GPINT_PINCTL_DEVICE  		GPINT_PINCTL_N(1)
#define GPINT_PINCTL_GPIOOUT_0 		GPINT_PINCTL_N(2)
#define GPINT_PINCTL_GPIOOUT_1		GPINT_PINCTL_N(3)

#define GPINT_INTLINE				(2)
#define GPINT_INTLINE_N(n)			(((n)&0x3)<<GPINT_INTLINE)
#define GPINT_INTLINE_CPUINT_0		GPINT_INTLINE_N(0)
#define GPINT_INTLINE_CPUINT_1		GPINT_INTLINE_N(1)
#define GPINT_INTLINE_CPUINT_2		GPINT_INTLINE_N(2)
#define GPINT_INTLINE_CPUINT_3		GPINT_INTLINE_N(3)

#define GPINT_INTCFG				(4)
#define GPINT_INTCFG_N(n)			(((n)&0x7)<<GPINT_INTCFG)
#define GPINT_INTCFG_DISABLE		GPINT_INTCFG_N(0)
#define GPINT_INTCFG_LL				GPINT_INTCFG_N(1)
#define GPINT_INTCFG_HL				GPINT_INTCFG_N(2)
#define GPINT_INTCFG_FE				GPINT_INTCFG_N(5)
#define GPINT_INTCFG_RE				GPINT_INTCFG_N(6)
#define GPINT_INTCFG_CHANGE			GPINT_INTCFG_N(7)

#define GPINT_INTWAKE				(7)
#define GPINT_INTWAKE_ENABLE		((1)<<GPINT_INTWAKE)

/* GPIO */
#define GPIO_N(N)                   (N)
#define CHANNEL_N(N)				(N)

#endif // #if defined(SOC_AU13XX)


/*********************************************************************************************
****                         Au13XX  UDMA Register Definitions                            ****
*********************************************************************************************/
#if defined(SOC_AU13XX)

#ifndef ASSEMBLER
typedef struct
{
	unsigned int	cfg;
	unsigned int	sts;
	unsigned int	intr;
	unsigned int	acs;
	unsigned int	txrx;
	unsigned int	mdma;
	unsigned int	udma;
	unsigned int    ptr;
} AU13XX_UDMA, *PAU13XX_UDMA;
#endif


#define UDMA_CFG_UIE		(1<<21)
#define UDMA_CFG_CE			(1<<20)
#define UDMA_CFG_FOE		(1<<19)
#define UDMA_CFG_FUE		(1<<18)
#define UDMA_CFG_DOE		(1<<17)
#define UDMA_CFG_DIE		(1<<16)
#define UDMA_CFG_RST		(1<<15)
#define UDMA_CFG_ESR		(1<<12)
#define UDMA_CFG_PFS		(1<<11)
#define UDMA_CFG_DIO		(1<<10)
#define UDMA_CFG_TDMA		(1<<9)
#define UDMA_CFG_CRC		(1<<8)
#define UDMA_CFG_DMAE		(1<<7)
#define UDMA_CFG_UDMA		(1<<6)
#define UDMA_CFG_DD			(1<<5)
#define UDMA_CFG_FC			(1<<4)
#define UDMA_CFG_RXT(n)		((n&3)<<2)
#define UDMA_CFG_TXT(n)		((n&3)

#define UDMA_STS_UI			(1<<21)
#define UDMA_STS_CE			(1<<20)
#define UDMA_STS_FO			(1<<19)
#define UDMA_STS_FU			(1<<18)
#define UDMA_STS_DO			(1<<17)
#define UDMA_STS_DI			(1<<16)

#define UDMA_INTR_UI	 	(1<<21)
#define UDMA_INTR_CE	 	(1<<20)
#define UDMA_INTR_FO	 	(1<<19)
#define UDMA_INTR_FU	 	(1<<18)
#define UDMA_INTR_DO	 	(1<<17)
#define UDMA_INTR_DI	 	(1<<16)

#define UDMA_ACS_PD			(1<<24)
#define UDMA_ACS_DB(n)		((n&0xFF)<<16)
#define UDMA_ACS_MB(n)		((n&0x3F)<<6)
#define UDMA_ACS_WD(n)		((n&0x3F))

#define UDMA_MDMA_DSC(n)	((n&0xF)<<12)
#define UDMA_MDMA_DAC(n)	((n&0xF)<<6)
#define UDMA_MDMA_DRC(n)	((n&0xF))

#define UDMA_UDMA_UMOD(n)	((n&0x3)<<16)
#define UDMA_UDMA_DID(n)	((n&0x3)<<13)
#define UDMA_UDMA_DSD(n)	((n&0x3)<<10)
#define UDMA_UDMA_HSD(n)	((n&0xF)<<6)
#define UDMA_UDMA_DCC(n)	((n&0x3F))


#endif // #if defined(SOC_AU13XX)

/***********************************************************************/

#if defined(SOC_AU1100)

/*
 * IrDA Register Offsets
 */

#define IR_RNGPTRSTAT		(0x0000)
#define IR_RNGBSADRH		(0x0004)
#define IR_RNGBSADRL		(0x0008)
#define IR_RINGSIZE			(0x000C)
#define IR_RNGPROMPT		(0x0010)
#define IR_RNGADRCMP		(0x0014)
#define	IR_INTCLEAR			(0x0018)
#define IR_CONFIG1			(0x0020)
#define IR_SIRFLAGS			(0x0024)
#define IR_STATUSEN			(0x0028)
#define IR_RDPHYCFG			(0x002C)
#define IR_WRPHYCFG			(0x0030)
#define IR_MAXPKTLEN		(0x0034)
#define IR_RXBYTECNT   		(0x0038)
#define IR_CONFIG2			(0x003C)
#define IR_ENABLE			(0x0040)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 rngptrstat;
    uint32 rngbsadrh;
    uint32 rngbsadrl;
    uint32 ringsize;
    uint32 rngprompt;
    uint32 rngadrcmp;
    uint32 intclear;
	uint32 reserved;
    uint32 config1;
    uint32 sirflags;
    uint32 statusen;
    uint32 rdphycfg;
    uint32 wrphycfg;
    uint32 maxpktlen;
    uint32 rxbytecnt;
    uint32 config2;
    uint32 enable;

} AU1X00_IRDA;
#endif

/*
 * Register content definitions
 */

#define IR_RNGPTRSTAT_TRPI		(63<<8)
#define IR_RNGPTRSTAT_RRPI		(63<<0)

#define IR_RINGSIZE_TRBS_S          (12)
#define IR_RINGSIZE_TRBS_N(N)       ((N)<<IR_RINGSIZE_TRBS_S)
#define IR_RINGSIZE_RRBS_S          (8)
#define IR_RINGSIZE_RRBS_N(N)       ((N)<<IR_RINGSIZE_RRBS_S)

#define IR_RNGADRCMP_EN		(1<<15)
#define IR_RNGADRCMP_ADDR	(127<<0)

#define IR_CONFIG1_EL				(1<<15)
#define IR_CONFIG1_IL				(1<<14)
#define IR_CONFIG1_TE				(1<<12)
#define IR_CONFIG1_RE				(1<<11)
#define IR_CONFIG1_ME				(1<<10)
#define IR_CONFIG1_RA				(1<<9)
#define IR_CONFIG1_TD				(1<<8)
#define IR_CONFIG1_CM				(1<<7)
#define IR_CONFIG1_FI				(1<<6)
#define IR_CONFIG1_MI				(1<<5)
#define IR_CONFIG1_SI				(1<<4)
#define IR_CONFIG1_SF				(1<<3)
#define IR_CONFIG1_ST				(1<<2)
#define IR_CONFIG1_TI				(1<<1)
#define IR_CONFIG1_RI				(1<<0)

#define IR_SIRFLAGS_FS				(0xFF<<8)
#define IR_SIRFLAGS_HS				(0xFF<<0)
#define IR_SIRFLAGS_FS_N(N)		(N<<8)
#define IR_SIRFLAGS_HS_N(N)		(N<<0)

#define IR_STATUSEN_E					(1<<15)
#define IR_STATUSEN_CE				(1<<14)
#define IR_STATUSEN_FV				(1<<13)
#define IR_STATUSEN_MV				(1<<12)
#define IR_STATUSEN_SV				(1<<11)
#define IR_STATUSEN_TS				(1<<10)
#define IR_STATUSEN_RS				(1<<9)
#define IR_STATUSEN_CS				(1<<8)

/* ir_read_phy_config and ir_write_phy_config */
#define IR_RDPHYCFG_BR			(63<<10)
#define IR_RDPHYCFG_PW			(31<<5)
#define IR_RDPHYCFG_P				(31<<0)
#define IR_RDPHYCFG_BR_N(N)		(N<<10)
#define IR_RDPHYCFG_PW_N(N)		(N<<5)
#define IR_RDPHYCFG_P_N(N)		(N<<0)

#define IR_WRPHYCFG_BR			(63<<10)
#define IR_WRPHYCFG_PW			(31<<5)
#define IR_WRPHYCFG_P				(31<<0)
#define IR_WRPHYCFG_BR_N(N)		(N<<10)
#define IR_WRPHYCFG_PW_N(N)		(N<<5)
#define IR_WRPHYCFG_P_N(N)		(N<<0)

#define IR_CONFIG2_IE				(1<<8)
#define IR_CONFIG2_FS				(3<<6)
#define IR_CONFIG2_DA				(1<<5)
#define IR_CONFIG2_DP				(1<<4)
#define IR_CONFIG2_CS				(3<<2)
#define IR_CONFIG2_P				(1<<1)
#define IR_CONFIG2_MI				(1<<0)
#define IR_CONFIG2_FS_LO			(0<<6)
#define IR_CONFIG2_FS_ML			(1<<6)
#define IR_CONFIG2_FS_MH			(2<<6)
#define IR_CONFIG2_FS_HI			(3<<6)
#define IR_CONFIG2_CS_40			(0<<2)
#define IR_CONFIG2_CS_48			(1<<2)
#define IR_CONFIG2_CS_56			(2<<2)
#define IR_CONFIG2_CS_64			(3<<2)

#define IR_ENABLE_HC	(1<<3)
#define IR_ENABLE_CE	(1<<2)
#define IR_ENABLE_CA	(1<<1)
#define IR_ENABLE_E		(1<<0)

/* FIX!!! Need Tx and Rx ring buffer descriptions */
#ifndef ASSEMBLER
typedef volatile struct
{
    uint32  ctrl;
    uint32  addr;

} AU1X00_IRDA_RING;
#endif

#define IR_TXRING_CTRL_COUNT_N(N)   ((N)<<0)
#define IR_TXRING_CTRL_COUNT		(0xFFF)
#define IR_TXRING_CTRL_UR           (1<<24)
#define IR_TXRING_CTRL_R            (1<<26)
#define IR_TXRING_CTRL_FU           (1<<27)
#define IR_TXRING_CTRL_NP           (1<<28)
#define IR_TXRING_CTRL_BC           (1<<29)
#define IR_TXRING_CTRL_DC           (1<<30)
#define IR_TXRING_CTRL_O            (1<<31)

#define IR_RXRING_CTRL_COUNT_N(N)   ((N)<<0)
#define IR_RXRING_CTRL_COUNT		(0x1FFF)
#define IR_RXRING_CTRL_SE           (1<<26)
#define IR_RXRING_CTRL_FO           (1<<27)
#define IR_RXRING_CTRL_ML           (1<<28)
#define IR_RXRING_CTRL_CE           (1<<29)
#define IR_RXRING_CTRL_PE           (1<<30)
#define IR_RXRING_CTRL_O            (1<<31)

/***********************************************************************/

/*
 * LCD Register offsets
 */
#ifndef ASSEMBLER
typedef volatile struct
{
	uint32	control;
	uint32	intstatus;
	uint32	intenable;
	uint32	horztiming;
	uint32	verttiming;
	uint32	clkcontrol;
	uint32	dmaaddr0;
	uint32	dmaaddr1;
	uint32	words;
	uint32	pwmdiv;
	uint32	pwmhi;
	uint32	reserved[(0x0400-0x002C)/4];
	uint32	pallettebase[256];

} AU1100_LCD;
#endif

/*
 * Register bit definitions
 */

/* lcd_control */
#define LCD_CONTROL_SBPPF		(7<<18)
#define LCD_CONTROL_SBPPF_655	(0<<18)
#define LCD_CONTROL_SBPPF_565	(1<<18)
#define LCD_CONTROL_SBPPF_556	(2<<18)
#define LCD_CONTROL_SBPPF_1555	(3<<18)
#define LCD_CONTROL_SBPPF_5551	(4<<18)
#define LCD_CONTROL_WP			(1<<17)
#define LCD_CONTROL_WD			(1<<16)
#define LCD_CONTROL_C			(1<<15)
#define LCD_CONTROL_SM			(3<<13)
#define LCD_CONTROL_SM_0		(0<<13)
#define LCD_CONTROL_SM_90		(1<<13)
#define LCD_CONTROL_SM_180		(2<<13)
#define LCD_CONTROL_SM_270		(3<<13)
#define LCD_CONTROL_DB			(1<<12)
#define LCD_CONTROL_CCO			(1<<11)
#define LCD_CONTROL_DP			(1<<10)
#define LCD_CONTROL_PO			(3<<8)
#define LCD_CONTROL_PO_00		(0<<8)
#define LCD_CONTROL_PO_01		(1<<8)
#define LCD_CONTROL_PO_10		(2<<8)
#define LCD_CONTROL_PO_11		(3<<8)
#define LCD_CONTROL_MPI			(1<<7)
#define LCD_CONTROL_PT			(1<<6)
#define LCD_CONTROL_PC			(1<<5)
#define LCD_CONTROL_BPP			(7<<1)
#define LCD_CONTROL_BPP_1		(0<<1)
#define LCD_CONTROL_BPP_2		(1<<1)
#define LCD_CONTROL_BPP_4		(2<<1)
#define LCD_CONTROL_BPP_8		(3<<1)
#define LCD_CONTROL_BPP_12		(4<<1)
#define LCD_CONTROL_BPP_16		(5<<1)
#define LCD_CONTROL_GO			(1<<0)

/* lcd_intstatus, lcd_intenable */
#define LCD_INT_SD				(1<<7)
#define LCD_INT_OF				(1<<6)
#define LCD_INT_UF				(1<<5)
#define LCD_INT_SA				(1<<3)
#define LCD_INT_SS				(1<<2)
#define LCD_INT_S1				(1<<1)
#define LCD_INT_S0				(1<<0)

/* lcd_horztiming */
#define LCD_HORZTIMING_HN2		(255<<24)
#define LCD_HORZTIMING_HN2_N(N)	(((N)-1)<<24)
#define LCD_HORZTIMING_HN1		(255<<16)
#define LCD_HORZTIMING_HN1_N(N)	(((N)-1)<<16)
#define LCD_HORZTIMING_HPW		(63<<10)
#define LCD_HORZTIMING_HPW_N(N)	(((N)-1)<<10)
#define LCD_HORZTIMING_PPL		(1023<<0)
#define LCD_HORZTIMING_PPL_N(N)	(((N)-1)<<0)

/* lcd_verttiming */
#define LCD_VERTTIMING_VN2		(255<<24)
#define LCD_VERTTIMING_VN2_N(N)	(((N)-1)<<24)
#define LCD_VERTTIMING_VN1		(255<<16)
#define LCD_VERTTIMING_VN1_N(N)	(((N)-1)<<16)
#define LCD_VERTTIMING_VPW		(63<<10)
#define LCD_VERTTIMING_VPW_N(N)	(((N)-1)<<10)
#define LCD_VERTTIMING_LPP		(1023<<0)
#define LCD_VERTTIMING_LPP_N(N)	(((N)-1)<<0)

/* lcd_clkcontrol */
#define LCD_CLKCONTROL_IB		(1<<18)
#define LCD_CLKCONTROL_IC		(1<<17)
#define LCD_CLKCONTROL_IH		(1<<16)
#define LCD_CLKCONTROL_IV		(1<<15)
#define LCD_CLKCONTROL_BF		(31<<10)
#define LCD_CLKCONTROL_BF_N(N)	(((N)-1)<<10)
#define LCD_CLKCONTROL_PCD		(1023<<0)
#define LCD_CLKCONTROL_PCD_N(N)	((N)<<0)

/* lcd_pwmdiv */
#define LCD_PWMDIV_EN			(1<<12)
#define LCD_PWMDIV_PWMDIV		(2047<<0)
#define LCD_PWMDIV_PWMDIV_N(N)	(((N)-1)<<0)

/* lcd_pwmhi */
#define LCD_PWMHI_PWMHI1		(2047<<12)
#define LCD_PWMHI_PWMHI1_N(N)	((N)<<12)
#define LCD_PWMHI_PWMHI0		(2047<<0)
#define LCD_PWMHI_PWMHI0_N(N)	((N)<<0)

/* lcd_pallettebase - MONOCHROME */
#define LCD_PALLETTE_MONO_MI		(15<<0)
#define LCD_PALLETTE_MONO_MI_N(N)	((N)<<0)

/* lcd_pallettebase - COLOR */
#define LCD_PALLETTE_COLOR_BI		(15<<8)
#define LCD_PALLETTE_COLOR_BI_N(N)	((N)<<8)
#define LCD_PALLETTE_COLOR_GI		(15<<4)
#define LCD_PALLETTE_COLOR_GI_N(N)	((N)<<4)
#define LCD_PALLETTE_COLOR_RI		(15<<0)
#define LCD_PALLETTE_COLOR_RI_N(N)	((N)<<0)

/* lcd_palletebase - COLOR TFT PALLETIZED */
#define LCD_PALLETTE_TFT_DC			(65535<<0)
#define LCD_PALLETTE_TFT_DC_N(N)	((N)<<0)

#endif /* SOC_AU1100 */

/********************************************************************/

#if defined(SOC_AU1000)||defined(SOC_AU1100)||defined(SOC_AU1500)||defined(SOC_AU1550)
/*
 * MAC Register Offsets
 */
#define	MAC_CONTROL		0x0000
#define	MAC_ADDRHIGH	0x0004
#define	MAC_ADDRLOW		0x0008
#define	MAC_HASHHIGH	0x000C
#define	MAC_HASHLOW		0x0010
#define	MAC_MIICTRL		0x0014
#define	MAC_MIIDATA		0x0018
#define	MAC_FLOWCTRL	0x001C
#define	MAC_VLAN1		0x0020
#define	MAC_VLAN2		0x0024

#define MACDMA_TXSTAT	0x0000
#define MACDMA_TXADDR	0x0004
#define MACDMA_TXLEN	0x0008

#define MACDMA_RXSTAT	0x0000
#define MACDMA_RXADDR	0x0004

#define MAC_RX_RING_SIZE 4
#define MAC_TX_RING_SIZE 4

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 macen0;
	uint32 macen1;

} AU1X00_MACEN;

typedef volatile struct
{
	uint32 control;
	uint32 addrhigh;
	uint32 addrlow;
	uint32 hashhigh;
	uint32 hashlow;
	uint32 miictrl;
	uint32 miidata;
	uint32 flowctrl;
	uint32 vlan1;
	uint32 vlan2;

} AU1X00_MAC;

typedef volatile struct
{
	uint32 txstat;
	uint32 txaddr;
	uint32 txlen;
	uint32 reserved;

} AU1X00_MACDMA_TX;

typedef volatile struct
{
	uint32 rxstat;
	uint32 rxaddr;
	uint32 reserved0;
	uint32 reserved1;

} AU1X00_MACDMA_RX;

typedef volatile struct
{
	AU1X00_MACDMA_TX tx[MAC_TX_RING_SIZE];
	AU1X00_MACDMA_TX treserved[16-MAC_TX_RING_SIZE];
	AU1X00_MACDMA_RX rx[MAC_RX_RING_SIZE];
	AU1X00_MACDMA_RX rreserved[16-MAC_RX_RING_SIZE];

} AU1X00_MACDMA;
#endif

/*
 * Register content definitions
 */

/* MAC_CONTROL */
#define MAC_CONTROL_RA				(1<<31)
#define MAC_CONTROL_EM				(1<<30)
#define MAC_CONTROL_DO				(1<<23)
#define MAC_CONTROL_LM				(3<<21)
#define MAC_CONTROL_F				(1<<20)
#define MAC_CONTROL_PM				(1<<19)
#define MAC_CONTROL_PR				(1<<18)
#define MAC_CONTROL_IF				(1<<17)
#define MAC_CONTROL_PB				(1<<16)
#define MAC_CONTROL_HO				(1<<15)
#define MAC_CONTROL_HP				(1<<13)
#define MAC_CONTROL_LC				(1<<12)
#define MAC_CONTROL_DB				(1<<11)
#define MAC_CONTROL_DR				(1<<10)
#define MAC_CONTROL_AP				(1<<8)
#define MAC_CONTROL_BL				(3<<6)
#define MAC_CONTROL_DC				(1<<5)
#define MAC_CONTROL_TE				(1<<3)
#define MAC_CONTROL_RE				(1<<2)
#define MAC_CONTROL_LM_normal		(0<<21)
#define MAC_CONTROL_LM_internal		(1<<21)
#define MAC_CONTROL_LM_external		(2<<21)
#define MAC_CONTROL_BL_1			(0<<21)
#define MAC_CONTROL_BL_2			(1<<21)
#define MAC_CONTROL_BL_3			(2<<21)
#define MAC_CONTROL_BL_4			(3<<21)

/* MAC_MIICTRL */
#define MAC_MIICTRL_PHYADDR			(31<<11)
#define MAC_MIICTRL_MIIREG			(31<<6)
#define MAC_MIICTRL_MW				(1<<1)
#define MAC_MIICTRL_MB				(1<<0)
#define MAC_MIICTRL_PHYADDR_N(N)	(N<<11)
#define MAC_MIICTRL_MIIREG_N(N)		(N<<6)

/* MAC_FLOWCTRL */
#define MAC_FLOWCTRL_PT				(0xFFFF<<16)
#define MAC_FLOWCTRL_PC				(1<<2)
#define MAC_FLOWCTRL_FE				(1<<1)
#define MAC_FLOWCTRL_FB				(1<<0)
#define MAC_FLOWCTRL_PT_N(N)		(N<<16)

/* MACEN_MAC */
#define MACEN_MAC_JP				(1<<6)
#define MACEN_MAC_E2				(1<<5)
#define MACEN_MAC_E1				(1<<4)
#define MACEN_MAC_C					(1<<3)
#define MACEN_MAC_TS				(1<<2)
#define MACEN_MAC_E0				(1<<1)
#define MACEN_MAC_CE				(1<<0)

/* MACDMA_RXSTAT */
#define MACDMA_RXSTAT_MI			(1<<31)
#define MACDMA_RXSTAT_PF			(1<<30)
#define MACDMA_RXSTAT_FF			(1<<29)
#define MACDMA_RXSTAT_BF			(1<<28)
#define MACDMA_RXSTAT_MF			(1<<27)
#define MACDMA_RXSTAT_UC			(1<<26)
#define MACDMA_RXSTAT_CF			(1<<25)
#define MACDMA_RXSTAT_LE			(1<<24)
#define MACDMA_RXSTAT_V2			(1<<23)
#define MACDMA_RXSTAT_V1			(1<<22)
#define MACDMA_RXSTAT_CR			(1<<21)
#define MACDMA_RXSTAT_DB			(1<<20)
#define MACDMA_RXSTAT_ME			(1<<19)
#define MACDMA_RXSTAT_FT			(1<<18)
#define MACDMA_RXSTAT_CS			(1<<17)
#define MACDMA_RXSTAT_FL			(1<<16)
#define MACDMA_RXSTAT_RF			(1<<15)
#define MACDMA_RXSTAT_WT			(1<<14)
#define MACDMA_RXSTAT_L				(0x3FFF<<0)

/* MACDMA_RXADDR */
#define MACDMA_RXADDR_ADDR			(0xFFFFFFE0)
#define MACDMA_RXADDR_CB_S			(2)
#define MACDMA_RXADDR_CB			(3<<MACDMA_RXADDR_CB_S)
#define MACDMA_RXADDR_DN			(1<<1)
#define MACDMA_RXADDR_EN			(1<<0)

/* MACDMA_TXSTAT */
#define MACDMA_TXSTAT_PR			(1<<31)
#define MACDMA_TXSTAT_CC			(15<<10)
#define MACDMA_TXSTAT_LO			(1<<9)
#define MACDMA_TXSTAT_DF			(1<<8)
#define MACDMA_TXSTAT_UR			(1<<7)
#define MACDMA_TXSTAT_EC			(1<<6)
#define MACDMA_TXSTAT_LC			(1<<5)
#define MACDMA_TXSTAT_ED			(1<<4)
#define MACDMA_TXSTAT_LS			(1<<3)
#define MACDMA_TXSTAT_NC			(1<<2)
#define MACDMA_TXSTAT_JT			(1<<1)
#define MACDMA_TXSTAT_FA			(1<<0)

/* MACDMA_TXADDR */
#define MACDMA_TXADDR_ADDR			(0xFFFFFFE0)
#define MACDMA_TXADDR_CB_S			(2)
#define MACDMA_TXADDR_CB			(3<<MACDMA_TXADDR_CB_S)
#define MACDMA_TXADDR_DN			(1<<1)
#define MACDMA_TXADDR_EN			(1<<0)

#endif /* #if defined(SOC_AU1000) ... */

/***********************************************************************/

/*
 * OHCI Register offsets
 */
#define OHC_HcRevision			0x0000
#define OHC_HcControl			0x0004
#define OHC_HcCommandStatus		0x0008
#define OHC_HcInterruptStatus	0x000C
#define OHC_HcInterruptEnable	0x0010
#define OHC_HcInterruptDisable	0x0014
#define OHC_HcHCCA				0x0018
#define OHC_HcPeriodCurrentED	0x001C
#define OHC_HcControlHeadED		0x0020
#define OHC_HcControlCurrentED	0x0024
#define OHC_HcBulkHeadED		0x0028
#define OHC_HcBulkCurrentED		0x002C
#define OHC_HcDoneHead			0x0030
#define OHC_HcRmInterval		0x0034
#define OHC_HcFmRemaining		0x0038
#define OHC_HcFmNumber			0x003C
#define OHC_HcPeriodicStart		0x0040
#define OHC_HcLSThreshold		0x0044
#define OHC_HcRhDescriptorA		0x0048
#define OHC_HcRhDescriptorB		0x004C
#define OHC_HcRhStatus			0x0050
#define OHC_HcRhPortStatus		0x0054

#define OHC_HcRhPort0Status		0x0054
#define OHC_HcRhPort1Status		0x0058
#define OHC_HcRhPort2Status		0x005C
#define OHC_HcRhPort3Status		0x0060
#define OHC_HcRhPort4Status		0x0064
#define OHC_HcRhPort5Status		0x0068
#define OHC_HcRhPort6Status		0x006C
#define OHC_HcRhPort7Status		0x0070

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 HcRevision;
	uint32 HcControl;
	uint32 HcCommandStatus;
	uint32 HcInterruptStatus;
	uint32 HcInterruptEnable;
	uint32 HcInterruptDisable;
	uint32 HcHCCA;
	uint32 HcPeriodCurrentED;
	uint32 HcControlHeadED;
	uint32 HcControlCurrentED;
	uint32 HcBulkHeadED;
	uint32 HcBulkCurrentED;
	uint32 HcDoneHead;
	uint32 HcRmInterval;
	uint32 HcFmRemaining;
	uint32 HcFmNumber;
	uint32 HcPeriodicStart;
	uint32 HcLSThreshold;
	uint32 HcRhDescriptorA;
	uint32 HcRhDescriptorB;
	uint32 HcRhStatus;

	uint32 HcRhPortStatus[8];

} OHC;

#endif

/***********************************************************************/

/*
 * SDRAM Register Offsets
 */
#if defined(SOC_AU1000) || defined(SOC_AU1500) || defined(SOC_AU1100)
#define MEM_SDMODE0		(0x0000)
#define MEM_SDMODE1		(0x0004)
#define MEM_SDMODE2		(0x0008)
#define MEM_SDADDR0		(0x000C)
#define MEM_SDADDR1		(0x0010)
#define MEM_SDADDR2		(0x0014)
#define MEM_SDREFCFG	(0x0018)
#define MEM_SDPRECMD	(0x001C)
#define MEM_SDAUTOREF	(0x0020)
#define MEM_SDWRMD0		(0x0024)
#define MEM_SDWRMD1		(0x0028)
#define MEM_SDWRMD2		(0x002C)
#define MEM_SDSLEEP		(0x0030)
#define MEM_SDSMCKE		(0x0034)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 sdmode0;
	uint32 sdmode1;
	uint32 sdmode2;
	uint32 sdaddr0;
	uint32 sdaddr1;
	uint32 sdaddr2;
	uint32 sdrefcfg;
	uint32 sdautoref;
	uint32 sdwrmd0;
	uint32 sdwrmd1;
	uint32 sdwrmd2;
	uint32 sdsleep;
	uint32 sdsmcke;

} AU1X00_SDRAM;
#endif

/*
 * MEM_SDMODE register content definitions
 */
#define MEM_SDMODE_F		(1<<22)
#define MEM_SDMODE_SR		(1<<21)
#define MEM_SDMODE_BS		(1<<20)
#define MEM_SDMODE_RS		(3<<18)
#define MEM_SDMODE_CS		(7<<15)
#define MEM_SDMODE_TRAS		(15<<11)
#define MEM_SDMODE_TMRD		(3<<9)
#define MEM_SDMODE_TWR		(3<<7)
#define MEM_SDMODE_TRP		(3<<5)
#define MEM_SDMODE_TRCD		(3<<3)
#define MEM_SDMODE_TCL		(7<<0)

#define MEM_SDMODE_BS_2Bank	(0<<20)
#define MEM_SDMODE_BS_4Bank	(1<<20)
#define MEM_SDMODE_RS_11Row	(0<<18)
#define MEM_SDMODE_RS_12Row	(1<<18)
#define MEM_SDMODE_RS_13Row	(2<<18)
#define MEM_SDMODE_RS_N(N)	((N)<<18)
#define MEM_SDMODE_CS_7Col	(0<<15)
#define MEM_SDMODE_CS_8Col	(1<<15)
#define MEM_SDMODE_CS_9Col	(2<<15)
#define MEM_SDMODE_CS_10Col	(3<<15)
#define MEM_SDMODE_CS_11Col	(4<<15)
#define MEM_SDMODE_CS_N(N)		((N)<<15)
#define MEM_SDMODE_TRAS_N(N)	((N)<<11)
#define MEM_SDMODE_TMRD_N(N)	((N)<<9)
#define MEM_SDMODE_TWR_N(N)		((N)<<7)
#define MEM_SDMODE_TRP_N(N)		((N)<<5)
#define MEM_SDMODE_TRCD_N(N)	((N)<<3)
#define MEM_SDMODE_TCL_N(N)		((N)<<0)

/*
 * MEM_SDADDR register contents definitions
 */
#define MEM_SDADDR_E			(1<<20)
#define MEM_SDADDR_CSBA			(0x03FF<<10)
#define MEM_SDADDR_CSMASK		(0x03FF<<0)
#define MEM_SDADDR_CSBA_N(N)	((N)&(0x03FF<<22)>>12)
#define MEM_SDADDR_CSMASK_N(N)	((N)&(0x03FF<<22)>>22)

/*
 * MEM_SDREFCFG register content definitions
 */
#define MEM_SDREFCFG_TRC		(15<<28)
#define MEM_SDREFCFG_TRPM		(3<<26)
#define MEM_SDREFCFG_E			(1<<25)
#define MEM_SDREFCFG_RE			(0x1ffffff<<0)
#define MEM_SDREFCFG_TRC_N(N)	((N)<<MEM_SDREFCFG_TRC)
#define MEM_SDREFCFG_TRPM_N(N)	((N)<<MEM_SDREFCFG_TRPM)
#define MEM_SDREFCFG_REF_N(N)	(N)
#endif

/***********************************************************************/

/*
 * Au1550 SDRAM Register Offsets
 */

/***********************************************************************/

#if defined(SOC_AU1550) || defined(SOC_AU1200) || defined(SOC_AU13XX)
#define MEM_SDMODE0		(0x0800)
#define MEM_SDMODE1		(0x0808)
#define MEM_SDMODE2		(0x0810)
#define MEM_SDADDR0		(0x0820)
#define MEM_SDADDR1		(0x0828)
#define MEM_SDADDR2		(0x0830)
#define MEM_SDCONFIGA	(0x0840)
#define MEM_SDCONFIGB	(0x0848)
#define MEM_SDSTAT		(0x0850)
#define MEM_SDERRADDR	(0x0858)
#define MEM_SDSTRIDE0	(0x0860)
#define MEM_SDSTRIDE1	(0x0868)
#define MEM_SDSTRIDE2	(0x0870)
#define MEM_SDPORTCFGA	MEM_SDSTRIDE1
#define MEM_SDWRMD0		(0x0880)
#define MEM_SDWRMD1		(0x0888)
#define MEM_SDWRMD2		(0x0890)
#define MEM_SDPRECMD	(0x08C0)
#define MEM_SDAUTOREF	(0x08C8)
#define MEM_SDSREF		(0x08D0)
#define MEM_SDCMD0		(0x08D8)
#define MEM_SDCMD1		(0x08DC)
#define MEM_SDSLEEP		MEM_SDSREF
#define MEM_PADCFG		(0x08E0)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 sdmode0;
	uint32 reserved0;
	uint32 sdmode1;
	uint32 reserved1;
	uint32 sdmode2;
	uint32 reserved2[3];
	uint32 sdaddr0;
	uint32 reserved3;
	uint32 sdaddr1;
	uint32 reserved4;
	uint32 sdaddr2;
	uint32 reserved5[3];
	uint32 sdconfiga;
	uint32 reserved6;
	uint32 sdconfigb;
	uint32 reserved7;
	uint32 sdstat;
	uint32 reserved8;
	uint32 sderraddr;
	uint32 reserved9;
	uint32 sdstride0;
	uint32 reserved10;
	uint32 sdstride1;
	uint32 reserved11;
	uint32 sdstride2;
	uint32 reserved12[3];
	uint32 sdwrmd0;
	uint32 reserved13;
	uint32 sdwrmd1;
	uint32 reserved14;
	uint32 sdwrmd2;
	uint32 reserved15[11];
	uint32 sdprecmd;
	uint32 reserved16;
	uint32 sdautoref;
	uint32 reserved17;
	uint32 sdsref;

} AU1550_SDRAM;
#endif

#endif  // #if defined(SOC_AU1550) || defined(SOC_AU1200)

/***********************************************************************/

#if defined(SOC_AU1500) || defined(SOC_AU1550)

/*
 * PCI Register Offsets
 */


#define PCI_CMEM				(0x0000)
#define PCI_CONFIG				(0x0004)
#define PCI_B2BMASK_CCH			(0x0008)
#define PCI_B2BBASE0_VENID		(0x000C)
#define PCI_B2BBASE1_ID			(0x0010)
#define PCI_MWMASK_DEV			(0x0014)
#define PCI_MWBASE_REV_CCL		(0x0018)
#define PCI_ERR_ADDR			(0x001C)
#define PCI_SPEC_INTACK			(0x0020)
#ifdef AU1550
 #define PCI_PR_CONFIG			(0x0024)
 #define PCI_PR_ADDR	  		(0x0028)
 #define PCI_PR_STS				(0x002C)
 //#define PCI_PR_DATA	this register is programmable through PR_ADDR & PR_CONFIG
#endif
#define PCI_ID					(0x0100)
#define PCI_STATCMD				(0x0104)
#define PCI_CLASSREV			(0x0108)
#define PCI_HDRTYPE				(0x010C)
#define PCI_MBAR				(0x0110)
#define PCI_TIMEOUT				(0x0140)


#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 cmem;
	uint32 config;
	uint32 b2bmask_cch;
	uint32 b2bbase0_venid;
	uint32 b2bbase1_subid;
	uint32 mwmask_dev;
	uint32 mwbase_rev_ccl;
	uint32 err_addr;
	uint32 spec_intack;
	uint32 pr_config;
	uint32 pr_addr;
	uint32 pr_sts;				  //offset at 0x2C
	uint32 reserved[(0x0100-0x0030)/4];
	uint32 id;
	uint32 statcmd;
	uint32 classrev;
	uint32 param;
	uint32 mbar;
	uint8 reserved1[0x2C];
	uint32 timeout;

} AU1550_PCI;
#endif

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 cmem;
	uint32 config;
	uint32 b2bmask_cch;
	uint32 b2bbase0_venid;
	uint32 b2bbase1_subid;
	uint32 mwmask_dev;
	uint32 mwbase_rev_ccl;
	uint32 err_addr;
	uint32 spec_intack;
	uint32 reserved[(0x0100-0x0024)/4];
	uint32 id;
	uint32 statcmd;
	uint32 classrev;
	uint32 hdrtype;
	uint32 mbar;
	uint8 reserved1[0x2C];
	uint32 timeout;

} AU1500_PCI;
#endif

#ifdef AU1550
 #define PCI_CMEM_HC			(1<<31)
#endif
#define PCI_CMEM_E				(1<<28)
#define PCI_CMEM_CMBASE			(0x3FFF<<14)
#define PCI_CMEM_CMMASK			(0x3FFF<<0)

#define PCI_CONFIG_ERRADDR		(0xF<<28)
#define PCI_CONFIG_ERD			(1<<27)
#define PCI_CONFIG_ET			(1<<26)
#define PCI_CONFIG_EF			(1<<25)
#define PCI_CONFIG_EP			(1<<24)
#define PCI_CONFIG_EM			(1<<23)
#define PCI_CONFIG_BM			(1<<22)
#define PCI_CONFIG_PD			(1<<20)
#define PCI_CONFIG_NC			(1<<16)
#define PCI_CONFIG_IA			(1<<15)
#define PCI_CONFIG_IP			(1<<13)
#define PCI_CONFIG_IS			(1<<12)
#define PCI_CONFIG_IMM			(1<<11)
#define PCI_CONFIG_ITM			(1<<10)
#define PCI_CONFIG_ITT			(1<<9)
#define PCI_CONFIG_IPB			(1<<8)
#define PCI_CONFIG_SIC			(3<<6)
#define PCI_CONFIG_ST			(1<<5)
#define PCI_CONFIG_SM			(1<<4)
#define PCI_CONFIG_AEN			(1<<3)
#define PCI_CONFIG_R2H			(1<<2)
#define PCI_CONFIG_R1H			(1<<1)
#define PCI_CONFIG_CH			(1<<0)

#define PCI_B2BMASK_CCH_B2BMASK	(0xFFFF<<16)
#define PCI_B2BMASK_CCH_CCH		(0xFFFF<<0)

#define PCI_B2BBASE0_VENID_B2BBASE0	(0xFFFF<<16)
#define PCI_B2BBASE0_VENID_VENID	(0xFFFF<<0)

#define PCI_B2BBASE1_SUBID_B2BBASE1	(0xFFFF<<16)
#define PCI_B2BBASE1_SUBID_SUBID	(0xFFFF<<0)

#define PCI_MWMASK_DEV_MWMASK		(0xFFFF<<16)
#define PCI_MWMASK_DEV_DEVID		(0xFFFF<<0)

#define PCI_MWBASE_REV_CCL_MWBASE	(0xFFFF<<16)
#define PCI_MWBASE_REV_CCL_REVID	(0xFF<<8)
#define PCI_MWBASE_REV_CCL_CCL		(0xFF<<0)

#ifdef AU1550
 #define	PCI_PR_AM			(1<<9)
 #define	PCI_PR_DM			(1<<8)
 #define PCI_PR_BS			(0x7<<4)
 #define	PCI_PR_ADDR_HIGH	(0xf<<0)

 #define	PCI_PR_AI			(1<<9)
 #define PCI_PR_DI			(1<<8)
 #define	PCI_PR_PND			(1<<0)

 #define	PCI_PR_AI			(1<<9)
 #define PCI_PR_DI			(1<<8)
 #define	PCI_PR_PND			(1<<0)
#endif

#define	PCI_ID_DID			(0xffff<<16)
#define PCI_ID_VID			(0xffff<<0)

#define PCI_STATCMD_STATUS	(0xffff<<16)
#define PCI_STATCMD_CMD		(0xffff<<0)

#define PCI_CLASSREV_CLASS	(0xffffff<<8)
#define PCI_CLASSREV_REV	(0xff<<0)

#define PCI_PARAM_BIST		(0xff<<24)
#define	PCI_PARAM_HT		(0xff<<16)
#define PCI_PARAM_LT		(0xff<<8)
#define PCI_PARAM_CLS		(0xff<<0)

#define PCI_TIMEOUT_TR		(0xfff<<16)
#define PCI_TIMEOUT_MR		(0xff<<8)
#define PCI_TIMEOUT_TO		(0xff<<0)

#endif // #if defined(SOC_AU1500) || defined(SOC_AU1550)

/***********************************************************************/

/*
 * SSI Register Offsets
 */
#define SSI_STATUS					(0x0000)
#define SSI_INT						(0x0004)
#define SSI_INTEN					(0x0008)
#define SSI_CONFIG					(0x0020)
#define SSI_ADATA					(0x0024)
#define SSI_CLKDIV					(0x0028)
#define SSI_ENABLE					(0x0100)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 status;
	uint32 intr;
	uint32 inten;
	uint32 reserved1[5];
	uint32 config;
	uint32 adata;
	uint32 clkdiv;
	uint32 reserved[53];
	uint32 enable;

} AU1X00_SSI;
#endif

/*
 * Register content definitions
 */
#define SSI_STATUS_BF				(1<<4)
#define SSI_STATUS_OF				(1<<3)
#define SSI_STATUS_UF				(1<<2)
#define SSI_STATUS_D				(1<<1)
#define SSI_STATUS_B				(1<<0)

/* SSI_INT */
#define SSI_INT_OI					(1<<3)
#define SSI_INT_UI					(1<<2)
#define SSI_INT_DI					(1<<1)

/* SSI_INTEN */
#define SSI_INTEN_OIE				(1<<3)
#define SSI_INTEN_UIE				(1<<2)
#define SSI_INTEN_DIE				(1<<1)

#define SSI_CONFIG_AO				(1<<24)
#define SSI_CONFIG_DO				(1<<23)
#define SSI_CONFIG_ALEN				(7<<20)
#define SSI_CONFIG_DLEN				(15<<16)
#define SSI_CONFIG_DD				(1<<11)
#define SSI_CONFIG_AD				(1<<10)
#define SSI_CONFIG_BM				(3<<8)
#define SSI_CONFIG_CE				(1<<7)
#define SSI_CONFIG_DP				(1<<6)
#define SSI_CONFIG_DL				(1<<5)
#define SSI_CONFIG_EP				(1<<4)
#define SSI_CONFIG_ALEN_N(N)		((N-1)<<20)
#define SSI_CONFIG_DLEN_N(N)		((N-1)<<16)
#define SSI_CONFIG_BM_HI			(0<<8)
#define SSI_CONFIG_BM_LO			(1<<8)
#define SSI_CONFIG_BM_CY			(2<<8)

#define SSI_ADATA_D					(1<<24)
#define SSI_ADATA_ADDR				(0xFF<<16)
#define SSI_ADATA_DATA				(0x0FFF)
#define SSI_ADATA_ADDR_N(N)			(N<<16)

#define SSI_ENABLE_CD				(1<<1)
#define SSI_ENABLE_E				(1<<0)

/***********************************************************************/

/*
 * Static Controller Register Offsets
 */

#define MEM_STCFG0				(0x1000)
#define MEM_STTIME0				(0x1004)
#define MEM_STADDR0				(0x1008)
#define MEM_STCFG1				(0x1010)
#define MEM_STTIME1				(0x1014)
#define MEM_STADDR1				(0x1018)
#define MEM_STCFG2				(0x1020)
#define MEM_STTIME2				(0x1024)
#define MEM_STADDR2				(0x1028)
#define MEM_STCFG3				(0x1030)
#define MEM_STTIME3				(0x1034)
#define MEM_STADDR3				(0x1038)
#if defined(SOC_AU1550) || defined(SOC_AU1200)
 #define MEM_STNDCTL			(0x1100)
 #define MEM_STSTAT				(0x1104)
#endif

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 stcfg0;
	uint32 sttime0;
	uint32 staddr0;
	uint32 reserved0;
	uint32 stcfg1;
	uint32 sttime1;
	uint32 staddr1;
	uint32 reserved1;
	uint32 stcfg2;
	uint32 sttime2;
	uint32 staddr2;
	uint32 reserved2;
	uint32 stcfg3;
	uint32 sttime3;
	uint32 staddr3;
#if defined(SOC_AU1550) || defined(SOC_AU1200) || defined(SOC_AU13XX)
	uint32 reserved3[(0x1100-0x103c)/4];
	uint32 stndctl;
	uint32 ststat;
#endif
} AU1X00_STATIC;
#endif

/*
 * Register content definitions
 */
#if defined(SOC_AU1550) || defined(SOC_AU1200)
 #define MEM_STCFG_NW			(1<<22)
#endif
#if defined(SOC_AU1100) || defined(SOC_AU1550)
 #define MEM_STCFG_AS			(1<<21)
 #define MEM_STCFG_S			(1<<20)
 #define MEM_STCFG_DE			(1<<19)
 #define MEM_STCFG_BEB_N(N)		((N&0x03)<<17)
 #define MEM_STCFG_TA			(1<<16)
 #define MEM_STCFG_DIV_N(N)		((N&0x07)<<13)
#endif
#define MEM_STCFG_BV			(1<<12)
#if defined(SOC_AU1000) || defined(SOC_AU1500)
 #define MEM_STCFG_D5			(1<<11)
#endif
#define MEM_STCFG_AV			(1<<10)
#define MEM_STCFG_TS			(1<<8)
#define MEM_STCFG_EW			(1<<7)
#define MEM_STCFG_H			(1<<6)
#define MEM_STCFG_BS			(1<<5)
#define MEM_STCFG_PM			(1<<4)
#define MEM_STCFG_RO			(1<<3)
#define MEM_STCFG_DTY			(7<<0)
#define MEM_STCFG_DTY_SRAM		(0<<0)
#define MEM_STCFG_DTY_IO		(1<<0)
#define MEM_STCFG_DTY_PCMCIA	(2<<0)
#define MEM_STCFG_DTY_FLASH		(3<<0)
#define MEM_STCFG_DTY_LCD		(4<<0)
#define MEM_STCFG_DTY_NAND		(5<<0)

/*
 * MEM_STTIME register definitions for use with
 * DTY=IO, Flash or SRAM.
 */
#define MEM_STTIME_TWCS			(3<<28)
#define MEM_STTIME_TCSH			(7<<24)
#define MEM_STTIME_TWP			(63<<14)
#define MEM_STTIME_TCSW			(15<<10)
#define MEM_STTIME_TPM			(15<<6)
#define MEM_STTIME_TA			(63<<0)
#define MEM_STTIME_TWCS_N(N)	(N<<28)
#define MEM_STTIME_TCSH_N(N)	(N<<24)
#define MEM_STTIME_TWP_N(N)		(N<<14)
#define MEM_STTIME_TCSW_N(N)	(N<<10)
#define MEM_STTIME_TPM_N(N)		(N<<6)
#define MEM_STTIME_TA_N(N)		(N<<0)

/*
 * MEM_STTIME register definitions for use with
 * DTY=LCD or PCMCIA.
 */
#define MEM_STTIME_TMST			(255<<24)
#define MEM_STTIME_TMSU			(127<<17)
#define MEM_STTIME_TMIH			(63<<11)
#define MEM_STTIME_TIST			(63<<5)
#define MEM_STTIME_TISU			(31<<0)
#define MEM_STTIME_TMST_N(N)	(N<<24)
#define MEM_STTIME_TMSU_N(N)	(N<<17)
#define MEM_STTIME_TMIH_N(N)	(N<<11)
#define MEM_STTIME_TIST_N(N)	(N<<5)
#define MEM_STTIME_TISU_N(N)	(N<<0)

#define MEM_STADDR_E   			(1<<28)
#define MEM_STADDR_CSBA			(0x3FFF<<14)
#define MEM_STADDR_CSMASK		(0x3FFF<<0)
#define MEM_STADDR_CSBA_N(N)	((N)&(0x3FFF<<18)>>4)
#define MEM_STADDR_CSMASK_N(N)	((N)&(0x3FFF<<18)>>18)

#define MEM_STNDCTRL_BOOT		(1)
#define MEM_STNDCTRL_IE			(1<<8)
#define MEM_STNDCTRL_IE			(1<<8)
#define MEM_STNDCTRL_ECE		(1<<16)
#define MEM_STNDCTRL_DFS		(1<<18)
#define MEM_STNDCTRL_DFIM		(1<<20)
#define MEM_STNDCTRL_SNIM		(1<<21)
#define MEM_STNDCTRL_FIM		(1<<22)
#define MEM_STNDCTRL_NEIM		(1<<23)
#define MEM_STNDCTRL_FFDM		(1<<24)
#define MEM_STNDCTRL_WFRI		(1<<25)
#define MEM_STNDCTRL_WFRD		(1<<26)
#define MEM_STNDCTRL_ECR		(1<<28)

#define MEM_STSTAT_BSY			(1)
#define MEM_STSTAT_BOOT			(3<<1)
#define MEM_STSTAT_EWT			(1<<4)
#define MEM_STSTAT_PWT			(1<<5)
#define MEM_STSTAT_NEC			(0x1F<<8)
#define MEM_STSTAT_NEC_N(N)		(((N)>>8)&0x1F)
#define MEM_STSTAT_DF			(1<<20)
#define MEM_STSTAT_SN			(1<<21)
#define MEM_STSTAT_RFF			(1<<22)
#define MEM_STSTAT_RFNE			(1<<23)

/***********************************************************************/

/*
 * SYStem Register Offsets
 */

/* Clocks  */
#define SYS_FREQCTRL0		(0x0020)
#define SYS_FREQCTRL1		(0x0024)
#define SYS_CLKSRC			(0x0028)
#define SYS_CPUPLL			(0x0060)
#define SYS_AUXPLL			(0x0064)

/* TOY & RTC */
#define SYS_TOYTRIM			(0x0000)
#define SYS_TOYWRITE		(0x0004)
#define SYS_TOYMATCH0		(0x0008)
#define SYS_TOYMATCH1		(0x000C)
#define SYS_TOYMATCH2		(0x0010)
#define SYS_CNTRCTRL		(0x0014)
#define SYS_TOYREAD			(0x0040)
#define SYS_RTCTRIM			(0x0044)
#define SYS_RTCWRITE		(0x0048)
#define SYS_RTCMATCH0		(0x004C)
#define SYS_RTCMATCH1		(0x0050)
#define SYS_RTCMATCH2		(0x0054)
#define SYS_RTCREAD			(0x0058)

#if !defined(SOC_AU13XX)
/* GPIO */
#define SYS_PINFUNC			(0x002C)
#define SYS_TRIOUTRD		(0x0100)
#define SYS_TRIOUTCLR		(0x0100)
#define SYS_OUTPUTRD		(0x0108)
#define SYS_OUTPUTSET		(0x0108)
#define SYS_OUTPUTCLR		(0x010C)
#define SYS_PINSTATERD		(0x0110)
#endif //#if !defined(SOC_AU13XX)

#define	SYS_PININPUTEN		(0x0110)

/* Power Management */
#define SYS_SCRATCH0		(0x0018)
#define SYS_SCRATCH1		(0x001C)
#define SYS_WAKEMSK			(0x0034)
#define SYS_ENDIAN			(0x0038)
#define SYS_POWERCTRL		(0x003C)
#define SYS_WAKESRC			(0x005C)
#define SYS_SLPPWR			(0x0078)
#define SYS_SLEEP			(0x007C)

#ifndef ASSEMBLER
typedef volatile struct
{
	/* 0x0000 */ uint32 toytrim;
	/* 0x0004 */ uint32 toywrite;
	/* 0x0008 */ uint32 toymatch0;
	/* 0x000C */ uint32 toymatch1;
	/* 0x0010 */ uint32 toymatch2;
	/* 0x0014 */ uint32 cntrctrl;
	/* 0x0018 */ uint32 scratch0;
	/* 0x001C */ uint32 scratch1;
	/* 0x0020 */ uint32 freqctrl0;
	/* 0x0024 */ uint32 freqctrl1;
	/* 0x0028 */ uint32 clksrc;
#if defined(SOC_AU13XX)
	/* 0x002C */ uint32 scratch2;
	/* 0x0030 */ uint32 scratch3;
#else
	/* 0x002C */ uint32 pinfunc;
	/* 0x0030 */ uint32 reserved0;
#endif
	/* 0x0034 */ uint32 wakemsk;
	/* 0x0038 */ uint32 endian;
	/* 0x003C */ uint32 powerctrl;
	/* 0x0040 */ uint32 toyread;
	/* 0x0044 */ uint32 rtctrim;
	/* 0x0048 */ uint32 rtcwrite;
	/* 0x004C */ uint32 rtcmatch0;
	/* 0x0050 */ uint32 rtcmatch1;
	/* 0x0054 */ uint32 rtcmatch2;
	/* 0x0058 */ uint32 rtcread;
	/* 0x005C */ uint32 wakesrc;
	/* 0x0060 */ uint32 cpupll;
	/* 0x0064 */ uint32 auxpll;
	/* 0x0068 */ uint32 auxpll2;
	/* 0x006C */ uint32 reserved2;
	/* 0x0070 */ uint32 reserved3;
	/* 0x0074 */ uint32 reserved4;
	/* 0x0078 */ uint32 slppwr;
	/* 0x007C */ uint32 sleep;
	/* 0x0080 */ uint32 reserved5[32];
	/* 0x0100 */ uint32 trioutrd;
#define trioutclr trioutrd
	/* 0x0104 */ uint32 reserved6;
	/* 0x0108 */ uint32 outputrd;
#define outputset outputrd
	/* 0x010C */ uint32 outputclr;
	/* 0x0110 */ uint32 pinstaterd;
#define pininputen pinstaterd
} AU1X00_SYS;
#endif

/*
 * Register content definitions
 */

/* Clocks */
#define SYS_FREQCTRL0_FRDIV2		(255<<22)
#define SYS_FREQCTRL0_FE2			(1<<21)
#define SYS_FREQCTRL0_FS2			(1<<20)
#define SYS_FREQCTRL0_FRDIV1		(255<<12)
#define SYS_FREQCTRL0_FE1			(1<<11)
#define SYS_FREQCTRL0_FS1			(1<<10)
#define SYS_FREQCTRL0_FRDIV0		(255<<2)
#define SYS_FREQCTRL0_FE0			(1<<1)
#define SYS_FREQCTRL0_FS0			(1<<0)
#define SYS_FREQCTRL0_FRDIV2_N(N)	((N/2)-1<<22)
#define SYS_FREQCTRL0_FRDIV1_N(N)	((N/2)-1<<12)
#define SYS_FREQCTRL0_FRDIV0_N(N)	((N/2)-1<<2)

#define SYS_FREQCTRL1_FRDIV5		(255<<22)
#define SYS_FREQCTRL1_FE5			(1<<21)
#define SYS_FREQCTRL1_FS5			(1<<20)
#define SYS_FREQCTRL1_FRDIV4		(255<<12)
#define SYS_FREQCTRL1_FE4			(1<<11)
#define SYS_FREQCTRL1_FS4			(1<<10)
#define SYS_FREQCTRL1_FRDIV3		(255<<2)
#define SYS_FREQCTRL1_FE3			(1<<1)
#define SYS_FREQCTRL1_FS3			(1<<0)
#define SYS_FREQCTRL1_FRDIV5_N(N)	((N/2)-1<<22)
#define SYS_FREQCTRL1_FRDIV4_N(N)	((N/2)-1<<12)
#define SYS_FREQCTRL1_FRDIV3_N(N)	((N/2)-1<<2)

#define SYS_CLKSRC_ME1			(7<<27)
#define SYS_CLKSRC_DE1			(1<<26)
#define SYS_CLKSRC_CE1			(1<<25)
#define SYS_CLKSRC_ME0			(7<<22)
#define SYS_CLKSRC_DE0			(1<<21)
#define SYS_CLKSRC_CE0			(1<<20)
#define SYS_CLKSRC_MI2			(7<<17)
#define SYS_CLKSRC_DI2			(1<<16)
#define SYS_CLKSRC_CI2			(1<<15)
#define SYS_CLKSRC_MUH			(7<<12)
#define SYS_CLKSRC_DUH			(1<<11)
#define SYS_CLKSRC_CUH			(1<<10)
#define SYS_CLKSRC_MUD			(7<<7)
#define SYS_CLKSRC_DUD			(1<<6)
#define SYS_CLKSRC_CUD			(1<<5)
#ifndef AU1550
 #define SYS_CLKSRC_MIR			(7<<2)
 #define SYS_CLKSRC_DIR			(1<<1)
 #define SYS_CLKSRC_CIR			(1<<0)
#endif
#define SYS_CLKSRC_ME1_N(N)		(N<<27)
#define SYS_CLKSRC_ME0_N(N)		(N<<22)
#define SYS_CLKSRC_MI2_N(N)		(N<<17)
#define SYS_CLKSRC_MUH_N(N)		(N<<12)
#define SYS_CLKSRC_MUD_N(N)		(N<<7)
#define SYS_CLKSRC_MIR_N(N)		(N<<2)
#define SYS_CLKSRC_MUX_RES		(0)
#define SYS_CLKSRC_MUX_AUX		(1)
#define SYS_CLKSRC_MUX_FREQ0	(2)
#define SYS_CLKSRC_MUX_FREQ1	(3)
#define SYS_CLKSRC_MUX_FREQ2	(4)
#define SYS_CLKSRC_MUX_FREQ3	(5)
#define SYS_CLKSRC_MUX_FREQ4	(6)
#define SYS_CLKSRC_MUX_FREQ5	(7)

#define SYS_CPUPLL_PLL		(0x7F<<0)

#define SYS_AUXPLL_PLL		(63<<0)

/* TOY & RTC */
#define SYS_CNTRCTRL_ERS	(1<<23)
#define SYS_CNTRCTRL_RTS	(1<<20)
#define SYS_CNTRCTRL_RM2	(1<<19)
#define SYS_CNTRCTRL_RM1	(1<<18)
#define SYS_CNTRCTRL_RM0	(1<<17)
#define SYS_CNTRCTRL_RS		(1<<16)
#define SYS_CNTRCTRL_BP		(1<<14)
#define SYS_CNTRCTRL_REN	(1<<13)
#define SYS_CNTRCTRL_BRT	(1<<12)
#define SYS_CNTRCTRL_TEN	(1<<11)
#define SYS_CNTRCTRL_BTT	(1<<10)
#define SYS_CNTRCTRL_EO		(1<<8)
#define SYS_CNTRCTRL_ETS	(1<<7)
#define SYS_CNTRCTRL_32S	(1<<5)
#define SYS_CNTRCTRL_TTS	(1<<4)
#define SYS_CNTRCTRL_TM2	(1<<3)
#define SYS_CNTRCTRL_TM1	(1<<2)
#define SYS_CNTRCTRL_TM0	(1<<1)
#define SYS_CNTRCTRL_TS		(1<<0)

#if defined(SOC_AU1000)||defined(SOC_AU1100)||defined(SOC_AU1500)||defined(SOC_AU1550)||defined(SOC_AU1200)

/* GPIO */
#define GPIO_0      (1 << 0)  // GPIO 0
#define GPIO_1      (1 << 1)  // GPIO 1
#define GPIO_2      (1 << 2)  // GPIO 2
#define GPIO_3      (1 << 3)  // GPIO 3
#define GPIO_4      (1 << 4)  // GPIO 4
#define GPIO_5      (1 << 5)  // GPIO 5
#define GPIO_6      (1 << 6)  // GPIO 6
#define GPIO_7      (1 << 7)  // GPIO 7
#define GPIO_8      (1 << 8)  // GPIO 8
#define GPIO_9      (1 << 9)  // GPIO 9
#define GPIO_10     (1 << 10) // GPIO 10
#define GPIO_11     (1 << 11) // GPIO 11
#define GPIO_12     (1 << 12) // GPIO 12
#define GPIO_13     (1 << 13) // GPIO 13
#define GPIO_14     (1 << 14) // GPIO 14
#define GPIO_15     (1 << 15) // GPIO 15
#define GPIO_16     (1 << 16) // GPIO 16
#define GPIO_17     (1 << 17) // GPIO 17
#define GPIO_18     (1 << 18) // GPIO 18
#define GPIO_19     (1 << 19) // GPIO 19
#define GPIO_20     (1 << 20) // GPIO 20
#define GPIO_21     (1 << 21) // GPIO 21
#define GPIO_22     (1 << 22) // GPIO 22
#define GPIO_23     (1 << 23) // GPIO 23
#define GPIO_24     (1 << 24) // GPIO 24
#define GPIO_25     (1 << 25) // GPIO 25
#define GPIO_26     (1 << 26) // GPIO 26
#define GPIO_27     (1 << 27) // GPIO 27
#define GPIO_28     (1 << 28) // GPIO 28
#define GPIO_29     (1 << 29) // GPIO 29
#define GPIO_30     (1 << 30) // GPIO 30
#define GPIO_31     (1 << 31) // GPIO 31

#endif // #if defined(SOC_)

/*
 * Register content definitions
 */
#if defined( AU1550 )

#define SYS_PINFUNC_S0		(1<<0)
#define SYS_PINFUNC_S1		(1<<1)
#define SYS_PINFUNC_MBS1	(1<<2)
#define SYS_PINFUNC_U0		(1<<3)
#define SYS_PINFUNC_NI2		(1<<4)
#define SYS_PINFUNC_MBS2	(1<<5)
#define SYS_PINFUNC_U3		(1<<7)
#define SYS_PINFUNC_EX0		(1<<9)
#define SYS_PINFUNC_EX1		(1<<10)
#define SYS_PINFUNC_U1T 	(1<<12)		/* 0= U1Txdata Drives GPIO[21] */
#define SYS_PINFUNC_U1R 	(1<<13)		/* 0= U1RTS Drives GPIO[22] */
#define SYS_PINFUNC_U3T 	(1<<14)		/* 0= U3Txdata Drives GPIO[23] */
#define SYS_PINFUNC_USB		(1<<15)
#define	SYS_PINFUNC_CS		(1<<16)
#define SYS_PINFUNC_PSC2	(17)
 #define SYS_PINFUNC_PSC2_GPIO	(0x7<<SYS_PINFUNC_PSC2)
 #define SYS_PINFUNC_PSC2_SM	(0x3<<SYS_PINFUNC_PSC2)
 #define SYS_PINFUNC_PSC2_SPI	(0x1<<SYS_PINFUNC_PSC2)
 #define SYS_PINFUNC_PSC2_AC97	(0x0<<SYS_PINFUNC_PSC2)
 #define SYS_PINFUNC_PSC2_I2S	(0x0<<SYS_PINFUNC_PSC2)
#define SYS_PINFUNC_PSC3	(20)
 #define SYS_PINFUNC_PSC3_GPIO	(0x7<<SYS_PINFUNC_PSC3)
 #define SYS_PINFUNC_PSC3_SM	(0x3<<SYS_PINFUNC_PSC3)
 #define SYS_PINFUNC_PSC3_SPI	(0x1<<SYS_PINFUNC_PSC3)
 #define SYS_PINFUNC_PSC3_AC97	(0x0<<SYS_PINFUNC_PSC3)
 #define SYS_PINFUNC_PSC3_I2S	(0x0<<SYS_PINFUNC_PSC3)
#define SYS_PINFUNC_RES6	(0x1FF<<23)

#define SYS_PININPUTEN_EN	(1<<0)

#elif defined( AU1200 )

#define SYS_PINFUNC_DMA		(1<<31)
#define SYS_PINFUNC_S0A		(1<<30)
#define SYS_PINFUNC_S1A		(1<<29)
#define SYS_PINFUNC_LP0		(1<<28)
#define SYS_PINFUNC_LP1		(1<<27)
#define SYS_PINFUNC_LD16	(1<<26)
#define SYS_PINFUNC_LD8		(1<<25)
#define SYS_PINFUNC_LD1		(1<<24)
#define SYS_PINFUNC_LD0		(1<<23)
#define SYS_PINFUNC_P1A		(3<<21)
#define SYS_PINFUNC_P1B		(1<<20)
#define SYS_PINFUNC_FS3		(1<<19)
#define SYS_PINFUNC_P0A		(3<<17)
#define SYS_PINFUNC_CS		(1<<16)
#define SYS_PINFUNC_CIM		(1<<15)
#define SYS_PINFUNC_P1C		(1<<14)
#define SYS_PINFUNC_U1T		(1<<12)
#define SYS_PINFUNC_U1R		(1<<11)
#define SYS_PINFUNC_EX1		(1<<10)
#define SYS_PINFUNC_EX0		(1<<9)
#define SYS_PINFUNC_U0R		(1<<8)
#define SYS_PINFUNC_MC		(1<<7)
#define SYS_PINFUNC_S0B		(1<<6)
#define SYS_PINFUNC_S0C		(1<<5)
#define SYS_PINFUNC_P0B		(1<<4)
#define SYS_PINFUNC_U0T		(1<<3)
#define SYS_PINFUNC_S1B		(1<<2)

#elif defined(SOC_AU1000)|| defined(SOC_AU1100)|| defined(SOC_AU1500)

#define SYS_PINFUNC_SD2		(3<<30)
#define SYS_PINFUNC_SD1		(3<<28)
#define SYS_PINFUNC_SD0		(3<<26)
#define SYS_PINFUNC_USB		(1<<15)
#define SYS_PINFUNC_U3		(1<<14)
#define SYS_PINFUNC_U2		(1<<13)
#define SYS_PINFUNC_U1		(1<<12)
#define SYS_PINFUNC_SRC		(1<<11)
#define SYS_PINFUNC_EX1		(1<<10)
#define SYS_PINFUNC_EX0		(1<<9)
#define SYS_PINFUNC_IRF		(1<<8)
#define SYS_PINFUNC_UR3		(1<<7)
#define SYS_PINFUNC_I2D		(1<<6)
#define SYS_PINFUNC_I2S		(1<<5)
#define SYS_PINFUNC_NI2		(1<<4)
#define SYS_PINFUNC_U0		(1<<3)
#define SYS_PINFUNC_IRD		(1<<2)
#define SYS_PINFUNC_A97		(1<<1)
#define SYS_PINFUNC_S0		(1<<0)

#endif

/* Power Management */
#if defined(SOC_AU13XX)
 #define SYS_WAKEMSK_WRE	(1<<10)
 #define SYS_WAKEMSK_D2		(1<<9)
#endif
#define SYS_WAKEMSK_M2		(1<<8)
#if !defined(SOC_AU13XX)
 #define SYS_WAKEMSK_GPIO7	(1<<7)
 #define SYS_WAKEMSK_GPIO6	(1<<6)
 #define SYS_WAKEMSK_GPIO5	(1<<5)
 #define SYS_WAKEMSK_GPIO4	(1<<4)
#endif
#define SYS_WAKEMSK_GPIO3	(1<<3)
#define SYS_WAKEMSK_GPIO2	(1<<2)
#define SYS_WAKEMSK_GPIO1	(1<<1)
#define SYS_WAKEMSK_GPIO0	(1<<0)
#define SYS_ENDIAN_EN		(1<<0)
#define SYS_ENDIAN_EN_EL	(1<<0)
#define SYS_ENDIAN_EN_EB	(0<<0)

#define SYS_POWERCTRL_VPUT			(3<<2)
#define SYS_POWERCTRL_VPUT_100ms	(0<<2)
#define SYS_POWERCTRL_VPUT_30ms		(1<<2)
#define SYS_POWERCTRL_VPUT_10ms		(2<<2)
#define SYS_POWERCTRL_VPUT_1ms		(3<<2)
#define SYS_POWERCTRL_SD	 (3<<0)
#define SYS_POWERCTRL_SD_2	 (0<<0)
#define SYS_POWERCTRL_SD_3	 (1<<0)
#define SYS_POWERCTRL_SD_4	 (2<<0)
#define SYS_POWERCTRL_SD_5	 (3<<0)

#if defined(AU1550) || defined(SOC_AU1200) || defined(SOC_AU13XX)
 #define SYS_WAKESRC_M2D		 (1<<25)
#endif
#define SYS_WAKESRC_M20		 (1<<24)
#if !defined(SOC_AU13XX)
 #define SYS_WAKESRC_GP7		 (1<<23)
 #define SYS_WAKESRC_GP6		 (1<<22)
 #define SYS_WAKESRC_GP5		 (1<<21)
 #define SYS_WAKESRC_GP4		 (1<<20)
#endif
#define SYS_WAKESRC_GP3		 (1<<19)
#define SYS_WAKESRC_GP2		 (1<<18)
#define SYS_WAKESRC_GP1		 (1<<17)
#define SYS_WAKESRC_GP0		 (1<<16)
#if defined(AU1550) || defined(SOC_AU1200) || defined(SOC_AU13XX)
 #define SYS_WAKESRC_DW		 (1<<2)
#endif
#define SYS_WAKESRC_SW		 (1<<1)
#define SYS_WAKESRC_IP		 (1<<0)

#define SYS_SLPPWR_SP		 (1<<0)

#define SYS_SLEEP_SL 		 (1<<0)

/*********************************************************************************************
****                         Au1XXX UART Register Definitions                             ****
*********************************************************************************************/
/*
 * UART Register Offsets
 */
#define UART_RXDATA		(0x0000)
#define UART_TXDATA		(0x0004)
#define UART_INTEN		(0x0008)
#define UART_INTCAUSE	(0x000C)
#define UART_FIFOCTRL	(0x0010)
#define UART_LINECTRL	(0x0014)
#define UART_MDMCTRL	(0x0018)
#define UART_LINESTAT	(0x001C)
#define UART_MDMSTAT	(0x0020)
#define UART_CLKDIV		(0x0028)
#define UART_ENABLE		(0x0100)
#define UART_MDMDEN	(0x0104)
#define UART_BIDIR	(0x0108)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 rxdata;
	uint32 txdata;
	uint32 inten;
	uint32 intcause;
	uint32 fifoctrl;
	uint32 linectrl;
	uint32 mdmctrl;
	uint32 linestat;
	uint32 mdmstat;
	uint32 reserved0;
	uint32 clkdiv;
	uint32 reserved1[53];
	uint32 enable;
	uint32 mdmen;
	uint32 bidir;
} AU1X00_UART;
#endif

/*
 * Register content definitions
 */
#define UART_INTEN_MIE			(1<<3)
#define UART_INTEN_LIE			(1<<2)
#define UART_INTEN_TIE			(1<<1)
#define UART_INTEN_RIE			(1<<0)

#define UART_INTCAUSE_IID		(7<<1)
#define UART_INTCAUSE_IP		(1<<0)
#define UART_INTCAUSE_IID_MS	(0<<1)
#define UART_INTCAUSE_IID_TBA	(1<<1)
#define UART_INTCAUSE_IID_RDA	(2<<1)
#define UART_INTCAUSE_IID_RLS	(3<<1)
#define UART_INTCAUSE_IID_CTO	(6<<1)

#define UART_FIFOCTRL_RFT		(3<<6)
#define UART_FIFOCTRL_TFT		(3<<4)
#define UART_FIFOCTRL_MS		(1<<3)
#define UART_FIFOCTRL_TR		(1<<2)
#define UART_FIFOCTRL_RR		(1<<1)
#define UART_FIFOCTRL_FE		(1<<0)
#define UART_FIFOCTRL_RFT_1		(0<<6)
#define UART_FIFOCTRL_RFT_4		(1<<6)
#define UART_FIFOCTRL_RFT_8		(2<<6)
#define UART_FIFOCTRL_RFT_14	(3<<6)
#define UART_FIFOCTRL_TFT_0		(0<<4)
#define UART_FIFOCTRL_TFT_4		(1<<4)
#define UART_FIFOCTRL_TFT_8		(2<<4)
#define UART_FIFOCTRL_TFT_12	(3<<4)

#define UART_LINECTRL_SB		(1<<6)
#define UART_LINECTRL_PAR		(3<<4)
#define UART_LINECTRL_PE		(1<<3)
#define UART_LINECTRL_ST		(1<<2)
#define UART_LINECTRL_WLS		(3<<0)
#define UART_LINECTRL_PAR_O		(0<<4)
#define UART_LINECTRL_PAR_E		(1<<4)
#define UART_LINECTRL_PAR_M		(2<<4)
#define UART_LINECTRL_PAR_Z		(3<<4)
#define UART_LINECTRL_WLS_5		(0<<0)
#define UART_LINECTRL_WLS_6		(1<<0)
#define UART_LINECTRL_WLS_7		(2<<0)
#define UART_LINECTRL_WLS_8		(3<<0)

#define UART_MDMCTRL_LB			(1<<4)
#define UART_MDMCTRL_I1			(1<<3)
#define UART_MDMCTRL_I0			(1<<2)
#define UART_MDMCTRL_RT			(1<<1)
#define UART_MDMCTRL_DT			(1<<0)

#define UART_LINESTAT_RF		(1<<7)
#define UART_LINESTAT_TE		(1<<6)
#define UART_LINESTAT_TT		(1<<5)
#define UART_LINESTAT_BI		(1<<4)
#define UART_LINESTAT_FE		(1<<3)
#define UART_LINESTAT_PE		(1<<2)
#define UART_LINESTAT_OE		(1<<1)
#define UART_LINESTAT_DR		(1<<0)

#define UART_MDMSTAT_CD			(1<<7)
#define UART_MDMSTAT_RI			(1<<6)
#define UART_MDMSTAT_DS			(1<<5)
#define UART_MDMSTAT_CT			(1<<4)
#define UART_MDMSTAT_DD			(1<<3)
#define UART_MDMSTAT_TRI		(1<<2)
#define UART_MDMSTAT_DR			(1<<1)
#define UART_MDMSTAT_DC			(1<<0)

#define UART_ENABLE_E			(1<<1)
#define UART_ENABLE_CE			(1<<0)

#define UART_MDMEN_DRI			(1<<3)
#define UART_MDMEN_DDSR			(1<<2)
#define UART_MDMEN_DDCD			(1<<1)
#define UART_MDMEN_DCTS			(1<<0)

#define UART_BIDIR_GT			(1<<2)
#define UART_BIDIR_OD			(1<<1)
#define UART_BIDIR_GE			(1<<0)

/***********************************************************************/

#if defined(SOC_AU1000)||defined(SOC_AU1100)||defined(SOC_AU1500)||defined(SOC_AU1550)

/* USB Host */

/*
 * Register Offsets, added a prefix '_' represents its offset.
 */
#ifdef AU1550
#define _USBH_ENABLE			(0x7FFC)
#else
#define _USBH_ENABLE			(0x7FFFC)
#endif

/* Register bit definitions */
#define USBH_ENABLE_RD		(1<<4)
#define USBH_ENABLE_CE		(1<<3)
#define USBH_ENABLE_E		(1<<2)
#define USBH_ENABLE_C		(1<<1)
#define USBH_ENABLE_BE		(1<<0)


/*
 * USB device Register Offsets
 */
#define USBD_EP0RD			(0x0000)
#define USBD_EP0WR			(0x0004)
#define USBD_EP1WR			(0x0008)
#define USBD_EP2WR			(0x000C)
#define USBD_EP3RD			(0x0010)
#define USBD_EP4RD			(0x0014)
#define USBD_INTEN			(0x0018)
#define USBD_INTSTAT			(0x001C)
#define USBD_CONFIG			(0x0020)
#define USBD_EP0CS			(0x0024)
#define USBD_EP1CS			(0x0028)
#define USBD_EP2CS			(0x002C)
#define USBD_EP3CS			(0x0030)
#define USBD_EP4CS			(0x0034)
#define USBD_EP0RDSTAT			(0x0040)
#define USBD_EP0WRSTAT			(0x0044)
#define USBD_EP1WRSTAT			(0x0048)
#define USBD_EP2WRSTAT			(0x004C)
#define USBD_EP3RDSTAT			(0x0050)
#define USBD_EP4RDSTAT			(0x0054)
#define USBD_ENABLE			(0x0058)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 ep0rd;
	uint32 ep0wr;
	uint32 ep1wr;
	uint32 ep2wr;
	uint32 ep3rd;
	uint32 ep4rd;

	uint32 inten;
	uint32 intstat;
	uint32 config;

	uint32 ep0cs;
	uint32 ep1cs;
	uint32 ep2cs;
	uint32 ep3cs;
	uint32 ep4cs;

	uint32 framenum;

	uint32 reserved;

	uint32 ep0rdstat;
	uint32 ep0wrstat;
	uint32 ep1wrstat;
	uint32 ep2wrstat;
	uint32 ep3rdstat;
	uint32 ep4rdstat;

	uint32 enable;
} AU1X00_USBD;
#endif

/* USBD_EPnCS */
#define USBD_EPCS_SU		(1<<14)
#define USBD_EPCS_N			(1<<13)
#define USBD_EPCS_A			(1<<12)
#define USBD_EPCS_B			(1<<11)
#define USBD_EPCS_SZ		(0x3FF<<1)
#define USBD_EPCS_FS		(1<<0)
#define USBD_EPCS_SZ_N(N)	(N<<1)

/* USBD_INTEN/USBD_INTSTAT */
#define USBD_INT_SF			(1<<12)
#define USBD_INT_H5			(1<<11)
#define USBD_INT_H4			(1<<10)
#define USBD_INT_H3			(1<<9)
#define USBD_INT_H2			(1<<8)
#define USBD_INT_H1			(1<<7)
#define USBD_INT_H0			(1<<6)
#define USBD_INT_C5			(1<<5)
#define USBD_INT_C4			(1<<4)
#define USBD_INT_C3			(1<<3)
#define USBD_INT_C2			(1<<2)
#define USBD_INT_C1			(1<<1)
#define USBD_INT_C0			(1<<0)

/* USBD_EPnSTAT */
#define USBD_EPnSTAT_FL		(1<<7)
#define USBD_EPnSTAT_UF		(1<<6)
#define USBD_EPnSTAT_OF		(1<<5)
#define USBD_EPnSTAT_FCNT	(31<<0)

/* USBD_ENABLE */
#define USBD_ENABLE_CE		(1<<1)
#define USBD_ENABLE_E		(1<<0)

#elif defined(SOC_AU1200)

// if a driver uses the USB configuration register outside the
//		...Init()
//		PowerUp()
//		PowerDown()
// routines, it should use a mutex with this name to provide
// exclusive access to the register. (This probably won't be
// necessary, however.)
//
#define USB_CONFIG_MUTEX_NAME	L"USB_Configuration_Mutex"

/*
 * USB Setup Register Offsets
 */
#define USB_CAP 			(0x0000)
#define USB_CONFIG			(0x0004)

#define OTG_CAP				(0x0020)
#define OTG_MUX				(0x0024)
#define OTG_STS				(0x0028)
#define OTG_CTL				(0x002C)
#define OTG_TMR				(0x0030)
#define OTG_INTR			(0x0034)
#define OTG_INTEN			(0x0038)

#if !defined(ASSEMBLER)
typedef volatile struct
{
	// setup registers
	uint32 cap;
	uint32 cfg;
	uint32 res1;
	uint32 diag;

	uint32 reserved[4];

	// OTG registers
	struct
	{
		uint32 cap;
		uint32 mux;
		uint32 sts;
		uint32 ctl;
		uint32 tmr;
		uint32 intr;
		uint32 inten;
	} otg;

} AU1200_USB;
#endif // ASSEMBLER

// define register fields as  (mask-value<<shift-value)

#define USB_CAP_REVID		(0x0FF<<0)
#define USB_CAP_ARCH		(0x00F<<8)
#define USB_CAP_CLASS		(0x0FF<<12)
#define USB_CAP_DEVID		(0x00F<<20)

#define USB_CFG_OME 		(0x001<<0)
#define USB_CFG_OBE 		(0x001<<1)
#define USB_CFG_EME 		(0x001<<2)
#define USB_CFG_EBE 		(0x001<<3)
#define USB_CFG_DME 		(0x001<<4)
#define USB_CFG_DBE 		(0x001<<5)
#define USB_CFG_GME 		(0x001<<6)
#define USB_CFG_UCAM		(0x001<<7)
#define USB_CFG_FLA 		(0x03F<<8)
#define USB_CFG_OCE 		(0x001<<16)
#define USB_CFG_ECE 		(0x001<<17)
#define USB_CFG_UCE 		(0x001<<18)
#define USB_CFG_PPE 		(0x001<<19)
#define USB_CFG_FLA_N		((N)<<8)

#define USB_DIAG_SIDDQ		(0x001<<7)

#define OTG_CAP_PPH1		(0x003<<0)
#define OTG_CAP_PPH2		(0x002<<2)
#define OTG_CAP_PEP1		(0x001<<8)
#define OTG_CAP_PEP2		(0x001<<9)
#define OTG_CAP_OCR			(0x003<<10)
#define OTG_CAP_OCP			(0x001<<14)
#define OTG_CAP_APU			(0x001<<15)
#define OTG_CAP_PPH1_N(N)	((N)<<0)
#define OTG_CAP_PPH2_N(N)	((N)<<2)
#define OTG_CAP_OCR_N(N)	((N)<<10)

#define OTG_MUX_PMUX		(0x003<<0)
#define OTG_MUX_PUE			(0x001<<2)
#define OTG_MUX_VBV			(0x001<<8)
#define OTG_MUX_PMUX_N(N)	((N)<<0)

#define OTG_STS_ID	 		(0x001<<0)
#define OTG_STS_VBV			(0x001<<1)
#define OTG_STS_SV	 		(0x001<<2)
#define OTG_STS_SE	 		(0x001<<3)
#define OTG_STS_LST			(0x003<<4)
#define OTG_STS_PSPD		(0x003<<6)
#define OTG_STS_FSOE		(0x001<<8)
#define OTG_STS_PCON		(0x001<<9)
#define OTG_STS_PSUS		(0x001<<10)
#define OTG_STS_TMH			(0x001<<11)
#define OTG_STS_HNPSTS 		(0x007<<12)
#define OTG_STS_OC	 		(0x001<<15)
#define OTG_STS_LST_N(N)	((N)<<4)
#define OTG_STS_PSPD_N(N)	((N)<<6)
#define OTG_STS_HNPSTS_N(N)	((N)<<12)

#define OTG_CTL_PMC			(0x003<<0)
#define OTG_CTL_PP 			(0x001<<2)
#define OTG_CTL_PPO			(0x001<<3)
#define OTG_CTL_CVB			(0x001<<4)
#define OTG_CTL_DVB			(0x001<<5)
#define OTG_CTL_ISE			(0x001<<6)
#define OTG_CTL_OPE			(0x001<<7)
#define OTG_CTL_PUE			(0x001<<8)
#define OTG_CTL_PDE			(0x001<<9))
#define OTG_CTL_HSF			(0x001<<10)
#define OTG_CTL_WPCS		(0x003<<16)
#define OTG_CTL_WPSS		(0x003<<18)
#define OTG_CTL_TMRCOND		(0x00F<<24)
#define OTG_CTL_TRP			(0x001<<28)
#define OTG_CTL_PMC_N(N)	((N)<<0)
#define OTG_CTL_WPCS_N(N)	((N)<<16)
#define OTG_CTL_WPSS_N(N)	((N)<<18)
#define OTG_CTL_TMRCOND_N(N) ((N)<<24)

#define OTG_TMR_PRELD		(0x3FF<<0)
#define OTG_TMR_CURTMR	 	(0x3FF<<16)
#define OTG_TMR_PRELD_N(N)	((N)<<0)
#define OTG_TMR_CURTMR_N(N)	((N)<<16)

#define OTG_INTR_ID			(0x001<<0)
#define OTG_INTR_VV			(0x001<<1)
#define OTG_INTR_SV			(0x001<<2)
#define OTG_INTR_SE			(0x001<<3)
#define OTG_INTR_LS			(0x001<<4)
#define OTG_INTR_SP			(0x001<<5)
#define OTG_INTR_FLO		(0x001<<6)
#define OTG_INTR_HSD		(0x001<<7))
#define OTG_INTR_RD			(0x001<<8)
#define OTG_INTR_PC			(0x001<<9)
#define OTG_INTR_PS			(0x001<<10)
#define OTG_INTR_TE			(0x001<<11)
#define OTG_INTR_HNP		(0x001<<12)
#define OTG_INTR_OC			(0x001<<13)
#define OTG_INTR_GIE		(0x001<<31)


/*
 * USB device Register Offsets
 */

// number of implemented endpoinds
#define UDC_EP_CNT 		5

// size of each endpoint descriptor
#define UDC_EP_SIZE			(0x0020)

// offset for each base input endpoint register
#define UDC_EPINCTRL		(0x0000)
#define UDC_EPINSTS			(0x0004)
#define UDC_EPINBS	 		(0x0008)
#define UDC_EPINMAXP		(0x000C)
#define UDC_EPINDDP			(0x0014)
#define UDC_EPINWRC			(0x001C)

// offset for each base output endpoint register
#define UDC_EPOUTCTRL		(0x0200)
#define UDC_EPOUTSTS		(0x0204)
#define UDC_EPOUTFRN		(0x0208)
#define UDC_EPOUTMAXP		(0x020C)
#define UDC_EPOUTSUBPTR		(0x0210)
#define UDC_EPOUTDESPTR		(0x0214)
#define UDC_EPOUTRDC		(0x021C)

// offset for each device register
#define UDC_CFG				(0x0400)
#define UDC_CTRL			(0x0404)
#define UDC_STS				(0x0408)
#define UDC_INTR			(0x040C)
#define UDC_INTRMSK			(0x0410)
#define UDC_EPINTR	 		(0x0414)
#define UDC_EPINTRMSK		(0x0418)

// offset for endpoint setup registers
#define UDC_EPREG			(0x0504)


#if !defined(ASSEMBLER)
typedef volatile struct
{
	// input endpoint control and status
	struct {
		uint32 ctrl;
		uint32 sts;
		uint32 bs;
		uint32 maxp;
		uint32 res_10;
		uint32 ddp;
		uint32 res_18;
		uint32 wrc;
	} epin[16];

	// output endpoint control and status
	struct {
		uint32 ctrl;
		uint32 sts;
		uint32 frn;
		uint32 maxp;
		uint32 subptr;
		uint32 desptr;
		uint32 res_18;
		uint32 rdc;
	} epout[16];

	// overall device control and status
	uint32 cfg;
	uint32 ctrl;
	uint32 sts;
	uint32 intr;
	uint32 intrmsk;
	uint32 epintr;
	uint32 epintrmsk;

	// pad out to 0x504
	char res[0x504 - 0x41C];

	// endpoint configuration
	uint32 epreg[32];

} AU1200_UDC;
#endif // !defined(ASSEMBLER)

// common endpoint control register fields
#define UDC_EPCTRL_CNAK		(0x001<<8)
#define UDC_EPCTRL_SNAK		(0x001<<7)
#define UDC_EPCTRL_NAK		(0x001<<6)
#define UDC_EPCTRL_ET		(0x003<<4)
#define UDC_EPCTRL_S	 	(0x001<<0)
#define UDC_EPCTRL_ET_N(N)	((N)<<4)

// input endpoint control register fields
#define UDC_EPINCTRL_CNAK	UDC_EPCTRL_CNAK
#define UDC_EPINCTRL_SNAK	UDC_EPCTRL_SNAK
#define UDC_EPINCTRL_NAK	UDC_EPCTRL_NAK
#define UDC_EPINCTRL_ET		UDC_EPCTRL_ET
#define UDC_EPINCTRL_P	 	(0x001<<3)
#define UDC_EPINCTRL_F	 	(0x001<<1)
#define UDC_EPINCTRL_S	 	(0x001<<0)
#define UDC_EPINCTRL_ET_N(N) UDC_EPCTRL_ET_N(N)

// input endpoint status register fields
#define UDC_EPINSTS_TDC		(0x001<<10)
#define UDC_EPINSTS_HE	 	(0x001<<9)
#define UDC_EPINSTS_BNA		(0x001<<7)
#define UDC_EPINSTS_IN	 	(0x001<<6)

// input endpoint buffer size register fields
#define UDC_EPINBS_BS		(0x3FF)

// input endpoint maximum packet size register fields
#define UDC_EPINMAXP_MAXP	(0xFFFF)

// output endpoint control register fields
#define UDC_EPOUTCTRL_CNAK	UDC_EPCTRL_CNAK
#define UDC_EPOUTCTRL_SNAK	UDC_EPCTRL_SNAK
#define UDC_EPOUTCTRL_NAK	UDC_EPCTRL_NAK
#define UDC_EPOUTCTRL_ET	UDC_EPCTRL_ET
#define UDC_EPOUTCTRL_SN	(0x001<<2)
#define UDC_EPOUTCTRL_S		(0x001<<0)
#define UDC_EPOUTCTRL_ET_N(N) UDC_EPCTRL_ET_N(N)

// output endpoint status register fields
#define UDC_EPOUTSTS_RXPKTSIZE (0xFFF<<11)
#define UDC_EPOUTSTS_HE		(0x001<<9)
#define UDC_EPOUTSTS_BNA	(0x001<<7)
#define UDC_EPOUTSTS_OUT	(0x003<<4)
#define UDC_EPOUTSTS_RXPKTSIZE_N(N) ((N)<<11)
#define UDC_EPOUTSTS_OUT_N(N) ((N)<<4)

// output endpoint frame number register fields
#define UDC_EPOUTFRN_FRN	(0x3FFF)

// output endpoint maximum packet size register fields
#define UDC_EPOUTMAXP_MAXP	(0xFFFF)

// device configuration register fields
#define UDC_CFG_SRST		(0x001<<31)
#define UDC_CFG_HSF			(0x001<<30)
#define UDC_CFG_SD			(0x001<<18)
#define UDC_CFG_PROG		(0x001<<17)
#define UDC_CFG_STAT	 	(0x003<<7)
#define UDC_CFG_SS	 		(0x001<<4)
#define UDC_CFG_SP	 		(0x001<<3)
#define UDC_CFG_RWKP		(0x001<<2)
#define UDC_CFG_SPD			(0x003<<0)
#define UDC_CFG_STAT_N(N)	((N)<<7)
#define UDC_CFG_SPD_N(N)	((N)<<0)

// device control register fields
#define UDC_CTRL_THLEN	 	(0x0FF<<24)
#define UDC_CTRL_BRLEN	 	(0x0FF<<16)
#define UDC_CTRL_DONE		(0x001<<13)
#define UDC_CTRL_NAK		(0x001<<12)
#define UDC_CTRL_SD			(0x001<<10)
#define UDC_CTRL_MODE		(0x001<<9)
#define UDC_CTRL_BREN		(0x001<<8)
#define UDC_CTRL_THE		(0x001<<7)
#define UDC_CTRL_BF			(0x001<<6)
#define UDC_CTRL_SE			(0x001<<5)
#define UDC_CTRL_DU			(0x001<<4)
#define UDC_CTRL_TDE		(0x001<<3)
#define UDC_CTRL_RDE		(0x001<<2)
#define UDC_CTRL_RES		(0x001<<0)
#define UDC_CTRL_THLEN_N(N)	((N)<<24)
#define UDC_CTRL_BRLEN_N(N)	((N)<<16)

// device status register fields
#define UDC_STS_TS	 		(0x3FFF<<18)
#define UDC_STS_PE	 		(0x001<<16)
#define UDC_STS_RXE			(0x001<<15)
#define UDC_STS_ES	 		(0x003<<13)
#define UDC_STS_SUSP		(0x001<<12)
#define UDC_STS_ALT			(0x00F<<8)
#define UDC_STS_INTF		(0x00F<<4)
#define UDC_STS_CFG			(0x00F<<0)
#define UDC_STS_TS_N(N)		((N)<<18)
#define UDC_STS_ES_N(N)		((N)<<13)
#define UDC_STS_ALT_N(N)	((N)<<8)
#define UDC_STS_INTF_N(N)	((N)<<4)
#define UDC_STS_CFG_N(N)	((N)<<0)

// device interrupt register fields
// device interrupt mask register fields
#define UDC_INTR_ENUM		(0x001<<6)
#define UDC_INTR_SOF		(0x001<<5)
#define UDC_INTR_US			(0x001<<4)
#define UDC_INTR_UR			(0x001<<3)
#define UDC_INTR_ES			(0x001<<2)
#define UDC_INTR_SI			(0x001<<1)
#define UDC_INTR_SC			(0x001<<0)

// device endpoint interrupt register fields
// device endpoint interrupt mask register fields
#define UDC_EPINTR_OUTEP	(0x01F<<16)
#define UDC_EPINTR_INEP		(0x01F<<0)
#define UDC_EPINTR_OUT_N(N)	(1<<(16+(N)))
#define UDC_EPINTR_IN_N(N)	(1<<(N))

// endpoint setup register fields
#define UDC_EPREG_MULT		(0x003<<30)
#define UDC_EPREG_MPS		(0x7FF<<19)
#define UDC_EPREG_ALTS		(0x00F<<15)
#define UDC_EPREG_IF		(0x00F<<11)
#define UDC_EPREG_CFG		(0x00F<<7)
#define UDC_EPREG_ET		(0x003<<5)
#define UDC_EPREG_ED		(0x001<<4)
#define UDC_EPREG_ED_IN		(0x001<<4)
#define UDC_EPREG_ED_OUT	(0x000<<4)
#define UDC_EPREG_EN		(0x00F<<0)
#define UDC_EPREG_MULT_N(N)	((N)<<30)
#define UDC_EPREG_MPS_N(N)	((N)<<19)
#define UDC_EPREG_ALTS_N(N)	((N)<<15)
#define UDC_EPREG_IF_N(N)	((N)<<11)
#define UDC_EPREG_CFG_N(N)	((N)<<7)
#define UDC_EPREG_ET_N(N)	((N)<<5)
#define UDC_EPREG_EN_N(N)	((N)<<0)

#endif

/*********************************************************************************************
****                         Au13XX USB Register Definitions                              ****
*********************************************************************************************/
#if defined(SOC_AU13XX)

#if !defined(ASSEMBLER)
typedef volatile struct
{
	// setup registers
	uint32 dwc_ctrl1;			//0x0000
	uint32 dwc_ctrl2;			//0x0004
	uint32 reserved0[2];		//0x08 - 0x0C

	uint32 vbus_timer;			//0x0010
	uint32 sbus_ctrl;			//0x0014
	uint32 msr_err;				//0x0018
	uint32 dwc_ctrl3;           //0x001C

	uint32 dwc_ctrl4;           //0x0020
	uint32 reserved1;			//0x0024
	uint32 otg_status;			//0x0028
	uint32 dwc_ctrl5;			//0x002C

	uint32 dwc_ctrl6;			//0x0030
	uint32 dwc_ctrl7;			//0x0034

	uint32 reserved2[(0xC0-0x38)/4]; // 0x0038 -- 0x00C0

	uint32 phy_status;			//0x00C0
	uint32 intr_status;			//0x00C4
	uint32 intr_enable;			//0x00C8

} AU13XX_USB;
#endif // ASSEMBLER

#define	USB_DWC_CTRL1_OTGD				(1<<2)
#define	USB_DWC_CTRL1_HSTRS				(1<<1)
#define	USB_DWC_CTRL1_DCRS				(1<<0)

#define	USB_DWC_CTRL2_HTBSE1			(1<<11)
#define	USB_DWC_CTRL2_HTBSE0			(1<<10)
#define	USB_DWC_CTRL2_LTBSE1			(1<<9)
#define	USB_DWC_CTRL2_LTBSE0			(1<<8)
#define	USB_DWC_CTRL2_LPBKE1			(1<<5)
#define	USB_DWC_CTRL2_LPBKE0			(1<<4)
#define	USB_DWC_CTRL2_VBUSD				(1<<3)
#define	USB_DWC_CTRL2_PH1RS				(1<<2)
#define	USB_DWC_CTRL2_PHY0RS			(1<<1)
#define	USB_DWC_CTRL2_PHYRS				(1<<0)

#define USB_VBUS_TIMER(n)				(n)

#define	USB_SBUS_CTRL_SBCA				(1<<2)
#define	USB_SBUS_CTRL_HWSZ				(1<<1)
#define	USB_SBUS_CTRL_BSZ				(1<<0)

#define USB_MSR_ERR_ILLBM				(1<<18)
#define USB_MSR_ERR_ILLBRST				(1<<17)
#define USB_MSR_ERR_UADDRSTS			(1<<16)
#define USB_MSR_ERR_BMMSK				(1<<2)
#define USB_MSR_ERR_BRSTMSK				(1<<1)
#define USB_MSR_ERR_UADMK				(1<<0)

#define	USB_DWC_CTRL3_VATEST_EN			(1<<20)
#define	USB_DWC_CTRL3_OHC1_CLKEN		(1<<19)
#define	USB_DWC_CTRL3_OHC0_CLKEN		(1<<18)
#define	USB_DWC_CTRL3_EHC_CLKEN			(1<<17)
#define	USB_DWC_CTRL3_OTG_CLKEN			(1<<16)
#define	USB_DWC_CTRL3_OHCI_SUSP			(1<<3)
#define	USB_DWC_CTRL3_VBUS_VALID_PORT1	(1<<2)
#define	USB_DWC_CTRL3_VBUS_VALID_PORT0	(1<<1)
#define	USB_DWC_CTRL3_VBUS_VALID_SEL	(1<<0)

#define	USB_DWC_CTRL4_USB_MODE			(1<<16)
#define	USB_DWC_CTRL4_AHB_CLKDIV(n)		((n&0xF)<<0)

#define USB_OTG_STATUS_IDPULLUP			(1<<8)
#define USB_OTG_STATUS_IDDIG			(1<<7)
#define USB_OTG_STATUS_DISCHRGVBUS		(1<<6)
#define USB_OTG_STATUS_CHRGVBUS			(1<<5)
#define USB_OTG_STATUS_DRVVBUS			(1<<4)
#define USB_OTG_STATUS_SESSIONEND		(1<<3)
#define USB_OTG_STATUS_VBUSVALID		(1<<2)
#define USB_OTG_STATUS_BVALID			(1<<1)
#define USB_OTG_STATUS_AVALID			(1<<0)

#define	USB_DWC_CTRL5_REFCLK_DIV(n)		((n&3)<<18)
#define	USB_DWC_CTRL5_REFCLK_EN(n)		((n&3)<<16)
#define	USB_DWC_CTRL5_SIDDQ				(1<<1)
#define	USB_DWC_CTRL5_COMMONONN			(1<<0)

#define	USB_DWC_CTRL6_DMPULLDOWN_PORT1	(1<<3)
#define	USB_DWC_CTRL6_DPPULLDOWN_PORT1	(1<<2)
#define	USB_DWC_CTRL6_DMPULLDOWN_PORT2	(1<<1)
#define	USB_DWC_CTRL6_DPPULLDOWN_PORT2	(1<<0)

#define	USB_DWC_CTRL7_OHC_STARTCLK		(1<<0)

#define USB_PHY_STATUS_VBUS				(1<<0)

// Bit defines used for status and enable registers
#define USB_INTR_S2A					(1<<6)
#define USB_INTR_FORCE					(1<<5)
#define USB_INTR_PHY					(1<<4)
#define USB_INTR_DEVICE					(1<<3)
#define USB_INTR_EHCI					(1<<2)
#define USB_INTR_OHCI1					(1<<1)
#define USB_INTR_OHCI0					(1<<0)

#endif // defined(SOC_Au13XX)

/*********************************************************************************************
****                         Au1X00 SD Register Definitions                               ****
*********************************************************************************************/

/*
 * Secure Digital Register Offsets
 */
#define SD_TXPORT		(0x0000)
#define SD_RXPORT		(0x0004)
#define SD_CONFIG		(0x0008)
#define SD_ENABLE		(0x000C)
#define SD_CONFIG2		(0x0010)
#define SD_BLKSIZE		(0x0014)
#define SD_STATUS		(0x0018)
#define SD_DEBUG		(0x001C)
#define SD_CMD			(0x0020)
#define SD_CMDARG		(0x0024)
#define SD_RESP3		(0x0028)
#define SD_RESP2		(0x002C)
#define SD_RESP1		(0x0030)
#define SD_RESP0		(0x0034)
#define SD_TIMEOUT		(0x0038)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 txport;
	uint32 rxport;
	uint32 config;
	uint32 enable;
	uint32 config2;
	uint32 blksize;
	uint32 status;
	uint32 debug;
	uint32 cmd;
	uint32 cmdarg;
	uint32 resp3;
	uint32 resp2;
	uint32 resp1;
	uint32 resp0;
	uint32 timeout;

} AU1X00_SD;
#endif

/*
 * Register content definitions
 */
#define SD_CONFIG_SI		(1<<31)
#define SD_CONFIG_CD		(1<<30)
#define SD_CONFIG_RA		(1<<29)
#define SD_CONFIG_RF		(1<<28)
#define SD_CONFIG_RH		(1<<27)
#define SD_CONFIG_TA		(1<<26)
#define SD_CONFIG_TE		(1<<25)
#define SD_CONFIG_TH		(1<<24)
#define SD_CONFIG_WC		(1<<22)
#define SD_CONFIG_RC		(1<<21)
#define SD_CONFIG_SC		(1<<20)
#define SD_CONFIG_DT		(1<<19)
#define SD_CONFIG_DD		(1<<18)
#define SD_CONFIG_RAT		(1<<17)
#define SD_CONFIG_CR		(1<<16)
#define SD_CONFIG_I			(1<<15)
#define SD_CONFIG_RO		(1<<14)
#define SD_CONFIG_RU		(1<<13)
#define SD_CONFIG_TO		(1<<12)
#define SD_CONFIG_TU		(1<<11)
#define SD_CONFIG_NE		(1<<10)
#define SD_CONFIG_DE		(1<<9)
#define SD_CONFIG_DIV		(511<<0)
#define SD_CONFIG_DIV_N(n)	(n & SD_CONFIG_DIV)

#define SD_ENABLE_R			(1<<1)
#define SD_ENABLE_CE		(1<<0)

#define SD_CONFIG2_DP		(1<<10)
#define SD_CONFIG2_RW		(1<<9)
#define SD_CONFIG2_WB		(1<<8)
#define SD_CONFIG2_BB		(1<<7)
#define SD_CONFIG2_DC		(1<<4)
#define SD_CONFIG2_DF		(1<<3)
#define SD_CONFIG2_FF		(1<<1)
#define SD_CONFIG2_EN		(1<<0)

#define SD_BLKSIZE_BC		(511<<16)
#define SD_BLKSIZE_BS		(1023<<0)
#define SD_BLKSIZE_BC_N(N)	((N-1)<<16)
#define SD_BLKSIZE_BS_N(N)	((N-1)<<0)

#define SD_STATUS_SI		(1<<31)
#define SD_STATUS_CD		(1<<30)
#define SD_STATUS_RF		(1<<29)
#define SD_STATUS_RA		(1<<28)
#define SD_STATUS_RH		(1<<27)
#define SD_STATUS_TA		(1<<26)
#define SD_STATUS_TE		(1<<25)
#define SD_STATUS_TH		(1<<24)
#define SD_STATUS_WC		(1<<22)
#define SD_STATUS_RC		(1<<21)
#define SD_STATUS_SC		(1<<20)
#define SD_STATUS_DT		(1<<19)
#define SD_STATUS_DD		(1<<18)
#define SD_STATUS_RAT		(1<<17)
#define SD_STATUS_CR		(1<<16)
#define SD_STATUS_I			(1<<15)
#define SD_STATUS_RO		(1<<14)
#define SD_STATUS_RU		(1<<13)
#define SD_STATUS_TO		(1<<12)
#define SD_STATUS_TU		(1<<11)
#define SD_STATUS_NE		(1<<10)
#define SD_STATUS_CF		(1<<6)
#define SD_STATUS_DB		(1<<5)
#define SD_STATUS_CB		(1<<4)
#define SD_STATUS_DCRCW		(7<<0)
#define SD_STATUS_DCRCW_NONE  	(2<<0)
#define SD_STATUS_DCRCW_TXERR 	(5<<0)
#define SD_STATUS_DCRCW_CRCERR	(7<<0)

#define SD_CMD_RT			(255<<16)
#define SD_CMD_CI			(255<<8)
#define SD_CMD_CT			(15<<4)
#define SD_CMD_RY			(1<<1)
#define SD_CMD_GO			(1<<0)
#define SD_CMD_BUSY			(1<<0)
#define SD_CMD_RT_NONE		(0<<16)
#define SD_CMD_RT_R1		(1<<16)
#define SD_CMD_RT_R2		(2<<16)
#define SD_CMD_RT_R3		(3<<16)
#define SD_CMD_RT_R4		(4<<16)
#define SD_CMD_RT_R5		(5<<16)
#define SD_CMD_RT_R6		(6<<16)
#define SD_CMD_RT_R1b		(0x81<<16)
#define SD_CMD_RT_N(n)		(n<<16)
#define SD_CMD_CI_N(N)		((N)<<8)
#define SD_CMD_CT_NONE		(0<<4)		/* IDLE ? */
#define SD_CMD_CT_SBW		(1<<4)		/* Single block write */
#define SD_CMD_CT_SBR		(2<<4)		/* Single block read */
#define SD_CMD_CT_MBW		(3<<4)		/* Multiple block write */
#define SD_CMD_CT_MBR		(4<<4)		/* Multiple block read */
#define SD_CMD_CT_MBIOW		(5<<4)		/* Multi block IO write */
#define SD_CMD_CT_MBIOR		(6<<4)		/* Multi block IO read */
#define SD_CMD_CT_TERM		(7<<4)
#define SD_CMD_CT_TERMIO	(8<<4)
#define SD_CMD_CT_N(n)		(n<<4)


/*********************************************************************************************
****                         Au1x  DDMA Register Definitions                               ****
*********************************************************************************************/

#define 	DDMA_NUM_CHANNELS 	16				// general registers follow channels at 0x14003000

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32	cfg;			// config
	uint32	des_ptr;		// descriptor pointer
	uint32	stat_ptr;		// status pointer
	uint32	dbell;			// doorbell
	uint32	irq;			// Interrupt
	uint32	stat;			// Status
	uint32	bytecnt;		// Remaining byte count
	uint32	reserved0[16];	//
	uint8	reserved1[0xA4];// Padding to make channels line up on 0x100 boundry
} DDMA_CHANNEL;
#endif

/*
 *	Bit definitions for Channel Config
 */
#define		DDMA_CHANCFG_EN			(1<<0)
#define		DDMA_CHANCFG_DBE	 	(1<<1)
#define		DDMA_CHANCFG_SBE		(1<<2)
#define		DDMA_CHANCFG_DFN	 	(1<<3)
#define		DDMA_CHANCFG_PPR	 	(1<<4)
#define		DDMA_CHANCFG_SYNC	 	(1<<5)
#define		DDMA_CHANCFG_DP		 	(1<<6)
#define		DDMA_CHANCFG_DED	 	(1<<7)
#define		DDMA_CHANCFG_SP		 	(1<<8)
#define		DDMA_CHANCFG_SED	 	(1<<9)

/*
 * Bit definitions for Channel Interrupt
 */
#define		DDMA_CHANINT_IN			(1<<0)

/*
 * Bit definitions for Channel Status
 */
#define		DDMA_CHANSTATUS_H		(1<<0)
#define		DDMA_CHANSTATUS_V		(1<<1)
#define		DDMA_CHANSTATUS_DB		(1<<2)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32	cmd0;				// Command
	uint32	cmd1;				// Byte count/cmd1
	uint32	source0;			// Source pointer
	uint32 	source1;			// Source pointer
	uint32	dest0;				// Destination pointer
	uint32	dest1;				// Destination stride/block
	uint32	stat;				// Status / Subroutine pointer
	uint32	nxt_ptr;			// next descriptor
} DDMA_DESCRIPTOR_STD;

typedef volatile struct
{
	uint32	cmd;				// Command
	uint32	byte_cnt;			// Byte count
	uint32	src_data0;			// Source data low
	uint32 	src_data1;			// Source data high
	uint32	dst_ptr;			// Destination pointer
	uint32	dst_strblk;			// Destination stride/block
	uint32	stat;				// Status / Subroutine pointer
	uint32	nxt_ptr;			// next descriptor
} DDMA_DESCRIPTOR_WRITEDMA;

typedef volatile struct
{
	uint32	cmd;				// Command
	uint32	branch_ptr;			// Branch pointer
	uint32	src_ptr;			// Source pointer
	uint32 	reserved;
	uint32	compare_data;		// Compare data
	uint32	data_mask;			// Data mask
	uint32	stat;				// Status / Subroutine pointer
	uint32	nxt_ptr;			// next descriptor
} DDMA_DESCRIPTOR_COMPARE_BRANCH;

typedef volatile struct
{
	union
	{
		DDMA_DESCRIPTOR_STD 			std;
		DDMA_DESCRIPTOR_WRITEDMA		wr;
		DDMA_DESCRIPTOR_COMPARE_BRANCH	cb;
	} u; //descriptor union

	union
	{
		uint8							u8[32];
		uint16							u16[16];
		uint32							u32[8];
		void *							p;
	}c;	// context
} DDMA_DESCRIPTOR;
#endif

/*
 *	Bit definitions for descriptor command
 */
#define		DDMA_DESCCMD_ST				(0 )
#define		DDMA_DESCCMD_ST_N(n)		((n&3)<<DDMA_DESCCMD_ST)
 #define		DDMA_DESCCMD_ST_WRSTAT	DDMA_DESCCMD_ST_N(1)
 #define		DDMA_DESCCMD_ST_CLEAR	DDMA_DESCCMD_ST_N(2)
 #define		DDMA_DESCCMD_ST_WRCNT	DDMA_DESCCMD_ST_N(3)
#define		DDMA_DESCCMD_CV				(1<<2)
#define		DDMA_DESCCMD_RP				(1<<3)
#define		DDMA_DESCCMD_SP				(1<<4)
#define		DDMA_DESCCMD_NR				(1<<5)
#define		DDMA_DESCCMD_SR				(1<<6)
#define		DDMA_DESCCMD_SRS		 	(1<<7)
#define		DDMA_DESCCMD_IE				(1<<8)						// Set interrupt bit upon completion
#define		DDMA_DESCCMD_RES		 	(1<<9)
#define		DDMA_DESCCMD_SM				(1<<10)
#define		DDMA_DESCCMD_DN				(1<<11)
#define		DDMA_DESCCMD_SN				(1<<12)
 #define	DDMA_DESCCMD_DT				(13)
 #define	DDMA_DESCCMD_DT_N(n)		((n&3)<<DDMA_DESCCMD_DT)
 #define		DDMA_DESCCMD_DT_REG		DDMA_DESCCMD_DT_N(0)
 #define		DDMA_DESCCMD_DT_WR		DDMA_DESCCMD_DT_N(1)
 #define		DDMA_DESCCMD_DT_COMPWR	DDMA_DESCCMD_DT_N(2)
 #define		DDMA_DESCCMD_DT_RES		DDMA_DESCCMD_DT_N(3)
#define		DDMA_DESCCMD_ARB		 	(1<<15)
#define		DDMA_DESCCMD_DW				(16)
#define		DDMA_DESCCMD_DW_N(n)		((n&3)<<DDMA_DESCCMD_DW)
 #define		DDMA_DESCCMD_DW_BYTE 	DDMA_DESCCMD_DW_N(0)
 #define		DDMA_DESCCMD_DW_HWORD	DDMA_DESCCMD_DW_N(1)
 #define		DDMA_DESCCMD_DW_WORD 	DDMA_DESCCMD_DW_N(2)
#define		DDMA_DESCCMD_SW				(18)
#define		DDMA_DESCCMD_SW_N(n)		((n&3)<<DDMA_DESCCMD_SW)
 #define		DDMA_DESCCMD_SW_BYTE 	DDMA_DESCCMD_SW_N(0)
 #define		DDMA_DESCCMD_SW_HWORD	DDMA_DESCCMD_SW_N(1)
 #define		DDMA_DESCCMD_SW_WORD 	DDMA_DESCCMD_SW_N(2)
#define		DDMA_DESCCMD_DID		 	(20)
#define		DDMA_DESCCMD_DID_N(n)	 	((n&0x1F)<<DDMA_DESCCMD_DID)
#define		DDMA_DESCCMD_SID		 	(25)
#define		DDMA_DESCCMD_SID_N(n)	 	((n&0x1F)<<DDMA_DESCCMD_SID)
#define		DDMA_DESCCMD_M				(1<<30)
#define		DDMA_DESCCMD_V				(1<<31)

/*
 *	Bit masks for descriptor count
 */
#define		DDMA_DESCCNT_BC		   		(0)
#define		DDMA_DESCCNT_BC_N(n)   		((n&0x1FFFF)<<DDMA_DESCCNT_BC)
#define		DDMA_DESCCNT_FL		   		(22)
#define		DDMA_DESCCNT_FL_N(n)   		((n&3)<<DDMA_DESCCNT_FL)
#define		DDMA_DESCCNT_DUPTR	   		(24)
#define		DDMA_DESCCNT_DUPTR_N(n)  	((n&0x0f)<<DDMA_DESCCNT_DUPTR)
#define		DDMA_DESCCNT_SUPTR	   		(28)
#define		DDMA_DESCCNT_SUPTR_N(n)  	((n&0x0f)<<DDMA_DESCCNT_SUPTR)

/*
 *	Bit definitions for branch pointer
 */
#define		DDMA_DESCDSRC_BP			(0)
#define		DDMA_DESCDSRC_BP_N(n)   	((n&0x7ffffff)<<DDMA_DESCDSRC_BP)
#define		DDMA_DESCDSRC_SUPTR	   		(28)
#define		DDMA_DESCDSRC_SUPTR_N(n)  	((n&0x0f)<<DDMA_DESCDSRC_SUPTR)

/*
 *	Bit definitions for source stride/block 1 dimensional
 */
#define		DDMA_DESCSRC_STRIDE_SS				(0)
#define		DDMA_DESCSRC_STRIDE_SS_N(n)			((n&0x3fff)<<DDMA_DESCSRC_STRIDE_SS)
#define		DDMA_DESCSRC_STRIDE_SB				(14)
#define		DDMA_DESCSRC_STRIDE_SB_N(n)			((n&0x3fff)<<DDMA_DESCSRC_STRIDE_SB)
#define		DDMA_DESCSRC_STRIDE_SAM				(28)
#define		DDMA_DESCSRC_STRIDE_SAM_N(n)		((n&3)<<DDMA_DESCSRC_STRIDE_SAM)
 #define	 	DDMA_DESCSRC_STRIDE_SAM_INC		DDMA_DESCSRC_STRIDE_SAM_N(0)
 #define     	DDMA_DESCSRC_STRIDE_SAM_DEC		DDMA_DESCSRC_STRIDE_SAM_N(1)
 #define	 	DDMA_DESCSRC_STRIDE_SAM_STATIC	DDMA_DESCSRC_STRIDE_SAM_N(2)
 #define	 	DDMA_DESCSRC_STRIDE_SAM_BURST	DDMA_DESCSRC_STRIDE_SAM_N(3)
#define		DDMA_DESCSRC_STRIDE_STS				(30)
#define		DDMA_DESCSRC_STRIDE_STS_N(n)  		((n&3)<<DDMA_DESCSRC_STRIDE_STS)
 #define		DDMA_DESCSRC_STRIDE_STS_1		DDMA_DESCSRC_STRIDE_STS_N(0)
 #define		DDMA_DESCSRC_STRIDE_STS_2		DDMA_DESCSRC_STRIDE_STS_N(1)
 #define		DDMA_DESCSRC_STRIDE_STS_4		DDMA_DESCSRC_STRIDE_STS_N(2)
 #define		DDMA_DESCSRC_STRIDE_STS_8		DDMA_DESCSRC_STRIDE_STS_N(3)

/*
 *	Bit definitions for source stride/block 2 dimensional
 */
#define		DDMA_DESCSRC_STRIDE2DIM_SC				(0)
#define		DDMA_DESCSRC_STRIDE2DIM_SC_N(n)			((n&0x7ff)<<DDMA_DESCSRC_STRIDE2DIM_SC)
#define		DDMA_DESCSRC_STRIDE2DIM_SS				(11)
#define		DDMA_DESCSRC_STRIDE2DIM_SS_N(n)			((n&0x3ff)<<DDMA_DESCSRC_STRIDE2DIM_SS)
#define		DDMA_DESCSRC_STRIDE2DIM_SB				(21)
#define		DDMA_DESCSRC_STRIDE2DIM_SB_N(n)			((n&0x07f)<<DDMA_DESCSRC_STRIDE2DIM_SB)
#define		DDMA_DESCSRC_STRIDE2DIM_SAM				(28)
#define		DDMA_DESCSRC_STRIDE2DIM_SAM_N(n)  		((n&3)<<DDMA_DESCSRC_STRIDE2DIM_SAM)
 #define	 	DDMA_DESCSRC_STRIDE2DIM_SAM_INC		DDMA_DESCSRC_STRIDE2DIM_SAM_N(0)
 #define     	DDMA_DESCSRC_STRIDE2DIM_SAM_DEC		DDMA_DESCSRC_STRIDE2DIM_SAM_N(1)
 #define	 	DDMA_DESCSRC_STRIDE2DIM_SAM_STATIC	DDMA_DESCSRC_STRIDE2DIM_SAM_N(2)
 #define	 	DDMA_DESCSRC_STRIDE2DIM_SAM_BURST	DDMA_DESCSRC_STRIDE2DIM_SAM_N(3)
#define		DDMA_DESCSRC_STRIDE2DIM_STS				(30)
#define		DDMA_DESCSRC_STRIDE2DIM_STS_N(n)		((n&3)<<DDMA_DESCSRC_STRIDE2DIM_STS)
 #define		DDMA_DESCSRC_STRIDE2DIM_STS_1		DDMA_DESCSRC_STRIDE2DIM_STS_N(0)
 #define		DDMA_DESCSRC_STRIDE2DIM_STS_2		DDMA_DESCSRC_STRIDE2DIM_STS_N(1)
 #define		DDMA_DESCSRC_STRIDE2DIM_STS_4		DDMA_DESCSRC_STRIDE2DIM_STS_N(2)
 #define		DDMA_DESCSRC_STRIDE2DIM_STS_8		DDMA_DESCSRC_STRIDE2DIM_STS_N(3)


/*
 *	Bit definitions for dest	 stride/block 1 dimensional
 */
#define		DDMA_DESCDST_STRIDE_DS				(0)
#define		DDMA_DESCDST_STRIDE_DS_N(n)			((n&0x3fff)<<DDMA_DESCDST_STRIDE_DS)
#define		DDMA_DESCDST_STRIDE_DB				(14)
#define		DDMA_DESCDST_STRIDE_DB_N(n)			((n&0x3fff)<<DDMA_DESCDST_STRIDE_DB)
#define		DDMA_DESCDST_STRIDE_DAM				(28)
#define		DDMA_DESCDST_STRIDE_DAM_N(n)		((n&3)<<DDMA_DESCDST_STRIDE_DAM)
 #define	 	DDMA_DESCDST_STRIDE_DAM_INC		DDMA_DESCDST_STRIDE_DAM_N(0)
 #define     	DDMA_DESCDST_STRIDE_DAM_DEC		DDMA_DESCDST_STRIDE_DAM_N(1)
 #define	 	DDMA_DESCDST_STRIDE_DAM_STATIC	DDMA_DESCDST_STRIDE_DAM_N(2)
 #define	 	DDMA_DESCDST_STRIDE_DAM_BURST	DDMA_DESCDST_STRIDE_DAM_N(3)
#define		DDMA_DESCDST_STRIDE_DTS				(30)
#define		DDMA_DESCDST_STRIDE_DTS_N(n)		((n&3)<<DDMA_DESCDST_STRIDE_DTS)
 #define		DDMA_DESCDST_STRIDE_DTS_1		DDMA_DESCDST_STRIDE_DTS_N(0)
 #define		DDMA_DESCDST_STRIDE_DTS_2		DDMA_DESCDST_STRIDE_DTS_N(1)
 #define		DDMA_DESCDST_STRIDE_DTS_4		DDMA_DESCDST_STRIDE_DTS_N(2)
 #define		DDMA_DESCDST_STRIDE_DTS_8		DDMA_DESCDST_STRIDE_DTS_N(3)

/*
 *	Bit definitions for dest	 stride/block 2 dimensional
 */
#define		DDMA_DESCDST_STRIDE2DIM_DC				(0)
#define		DDMA_DESCDST_STRIDE2DIM_DC_N(n)			((n&0x7ff)<<DDMA_DESCDST_STRIDE2DIM_DC)
#define		DDMA_DESCDST_STRIDE2DIM_DS				(11)
#define		DDMA_DESCDST_STRIDE2DIM_DS_N(n)			((n&0x3ff)<<DDMA_DESCDST_STRIDE2DIM_DS)
#define		DDMA_DESCDST_STRIDE2DIM_DB				(21)
#define		DDMA_DESCDST_STRIDE2DIM_DB_N(n)			((n&0x7f)<<DDMA_DESCDST_STRIDE2DIM_DB)
#define		DDMA_DESCDST_STRIDE2DIM_DAM				(28)
#define		DDMA_DESCDST_STRIDE2DIM_DAM_N(n) 		((n&3)<<DDMA_DESCDST_STRIDE2DIM_DAM)
 #define	 	DDMA_DESCDST_STRIDE2DIM_DAM_INC		DDMA_DESCDST_STRIDE2DIM_DAM_N(0)
 #define     	DDMA_DESCDST_STRIDE2DIM_DAM_DEC		DDMA_DESCDST_STRIDE2DIM_DAM_N(1)
 #define	 	DDMA_DESCDST_STRIDE2DIM_DAM_STATIC	DDMA_DESCDST_STRIDE2DIM_DAM_N(2)
 #define	 	DDMA_DESCDST_STRIDE2DIM_DAM_BURST	DDMA_DESCDST_STRIDE2DIM_DAM_N(3)
#define		DDMA_DESCDST_STRIDE2DIM_DTS				(30)
#define		DDMA_DESCDST_STRIDE2DIM_DTS_N(n)		((n&3)<<DDMA_DESCDST_STRIDE2DIM_DTS)
 #define		DDMA_DESCDST_STRIDE2DIM_DTS_1		DDMA_DESCDST_STRIDE2DIM_DTS_N(0)
 #define		DDMA_DESCDST_STRIDE2DIM_DTS_2		DDMA_DESCDST_STRIDE2DIM_DTS_N(1)
 #define		DDMA_DESCDST_STRIDE2DIM_DTS_4		DDMA_DESCDST_STRIDE2DIM_DTS_N(2)
 #define		DDMA_DESCDST_STRIDE2DIM_DTS_8		DDMA_DESCDST_STRIDE2DIM_DTS_N(3)

/*
 *	Bit definitions for descriptor "next" pointer
 */
#define		DDMA_DESCNEXTPTR_NPTR	 		(0)
#define		DDMA_DESCNEXTPTR_NPTR_N(n) 		((n&0x1ffffff)<<DDMA_DESCNEXTPTR_NPTR)
#define		DDMA_DESCNEXTPTR_MS		 		(1<<27)
#define		DDMA_DESCNEXTPTR_BBC		 	(28)
#define		DDMA_DESCNEXTPTR_BBC_N(n)	 	((n&3)<<DDMA_DESCNEXTPTR_BBC)
 #define		DDMA_DESCNEXTPTR_BBC_1		DDMA_DESCNEXTPTR_BBC_N(0)
 #define		DDMA_DESCNEXTPTR_BBC_2		DDMA_DESCNEXTPTR_BBC_N(1)
 #define		DDMA_DESCNEXTPTR_BBC_3		DDMA_DESCNEXTPTR_BBC_N(2)
 #define		DDMA_DESCNEXTPTR_BBC_4		DDMA_DESCNEXTPTR_BBC_N(3)

/*
 *  DDMA Controller register offsets
 */
#define DDMA_CONFIG			(0x1000)
#define DDMA_INTSTAT		(0x1004)
#define DDMA_THROTTLE		(0x1008)
#define DDMA_INTEN			(0x100C)

#ifndef ASSEMBLER
typedef volatile struct
{
	DDMA_CHANNEL	channel[DDMA_NUM_CHANNELS];			// Start at offset 1400 2000 -- 1400 2f00
	uint32	config;										// 0x1400 3000
	uint32	intstat;									// 0x1400 3004
	uint32	throttle;									// 0x1400 3008
	uint32	inten;										// 0x1400 300c
} AU1X00_DDMA;
#endif

/*
 *	Bit definitions for General Configuration
 */
#define		DDMA_CONFIG_AL 			(1<<0)
#define		DDMA_CONFIG_AH			(1<<1)
#define		DDMA_CONFIG_AF			(1<<2)
#define		DDMA_CONFIG_C64			(1<<8)

/*
 *	Bit definitions for Interrupt Status
 */
#define		DDMA_INTSTAT_CHAN0		(1<<0)
#define		DDMA_INTSTAT_CHAN1		(1<<1)
#define		DDMA_INTSTAT_CHAN2		(1<<2)
#define		DDMA_INTSTAT_CHAN3		(1<<3)
#define		DDMA_INTSTAT_CHAN4		(1<<4)
#define		DDMA_INTSTAT_CHAN5		(1<<5)
#define		DDMA_INTSTAT_CHAN6		(1<<6)
#define		DDMA_INTSTAT_CHAN7		(1<<7)
#define		DDMA_INTSTAT_CHAN8		(1<<8)
#define		DDMA_INTSTAT_CHAN9		(1<<9)
#define		DDMA_INTSTAT_CHAN10		(1<<10)
#define		DDMA_INTSTAT_CHAN11		(1<<11)
#define		DDMA_INTSTAT_CHAN12		(1<<12)
#define		DDMA_INTSTAT_CHAN13		(1<<13)
#define		DDMA_INTSTAT_CHAN14		(1<<14)
#define		DDMA_INTSTAT_CHAN15		(1<<15)


/*
 *	DDMA Peripheral Addresses
 */
#if defined(SOC_AU1200)

#define		DDMA_UART0_TX_ID					(0)
#define		DDMA_UART0_RX_ID					(1)
#define		DDMA_UART1_TX_ID					(2)
#define		DDMA_UART1_RX_ID					(3)
#define		DDMA_REQ0_ID						(4)
#define		DDMA_GPIO4_ID						DDMA_REQ0_ID
#define		DDMA_REQ1_ID						(5)
#define		DDMA_GPIO12_ID						DDMA_REQ1_ID
#define		DDMA_MAE_BE_ID						(6)
#define		DDMA_MAE_FE_ID						(7)
#define 	DDMA_SDMS0_TX_ID					(8)
#define  	DDMA_SDMS0_RX_ID					(9)
#define 	DDMA_SDMS1_TX_ID					(10)
#define 	DDMA_SDMS1_RX_ID					(11)
#define 	DDMA_AES_TX_ID						(12)
#define 	DDMA_AES_RX_ID						(13)
#define		DDMA_PSC0_TX_ID						(14)
#define		DDMA_PSC0_RX_ID						(15)
#define		DDMA_PSC1_TX_ID						(16)
#define		DDMA_PSC1_RX_ID						(17)
#define		DDMA_CIM_A_RX_ID					(18)
#define		DDMA_CIM_B_RX_ID					(19)
#define		DDMA_CIM_C_RX_ID					(20)
#define		DDMA_MAE_BOTH_DONE_ID				(21)
#define		DDMA_LCD_RETRACE_ID					(22)
#define		DDMA_NAND_FLASH_ID					(23)
#define		DDMA_PSC0_SYNC_ID					(24)
#define		DDMA_PSC1_SYNC_ID					(25)
#define		DDMA_CIM_FRAME_SYNC_ID				(26)
#define		DDMA_INT0_ID						(27)
#define		DDMA_SDMS0_DATA_OUT_ID				(28)
#define		DDMA_INT1_ID						(29)
/* IDs 24-29 are reserved */
#define 	DDMA_MEMORY_THROTTLE_ID				(30)
#define 	DDMA_ALWAYS_HIGH_ID					(31)

/*
 *	Physical address of ddma peripherals
 */
#define		DDMA_UART0_TX_ADDR					(0x11100004)
#define		DDMA_UART0_RX_ADDR					(0x11100000)
#define		DDMA_UART1_TX_ADDR					(0x11200004)
#define		DDMA_UART1_RX_ADDR					(0x11200000)
#define		DDMA_PSC0_TX_ADDR					(0x11A0001C)
#define		DDMA_PSC0_RX_ADDR					(0x11A0001C)
#define		DDMA_PSC1_TX_ADDR					(0x11B0001C)
#define		DDMA_PSC1_RX_ADDR					(0x11B0001C)
#define		DDMA_SDMS0_TX_ADDR					(0x10600000)
#define		DDMA_SDMS0_RX_ADDR					(0x10600004)
#define		DDMA_SDMS1_TX_ADDR					(0x10680000)
#define		DDMA_SDMS1_RX_ADDR					(0x10680004)

#elif defined(SOC_AU1550)

#define		DDMA_UART0_TX_ID					(0)
#define		DDMA_UART0_RX_ID					(1)
#define		DDMA_UART3_TX_ID					(2)
#define		DDMA_UART3_RX_ID					(3)
#define		DDMA_REQ0_ID						(4)
#define		DDMA_GPIO4_ID						DDMA_REQ0_ID
#define		DDMA_REQ1_ID						(5)
#define		DDMA_GPIO5_ID						DDMA_REQ1_ID
#define		DDMA_REQ2_ID						(6)
#define		DDMA_GPIO208_ID						DDMA_REQ2_ID
#define		DDMA_REQ3_ID						(7)
#define		DDMA_GPIO209_ID						DDMA_REQ3_ID
#define 	DDMA_USB_DEVICE_ENDPOINT_0_RX_ID	(8)
#define  	DDMA_USB_DEVICE_ENDPOINT_0_TX_ID	(9)
#define 	DDMA_USB_DEVICE_ENDPOINT_1_TX_ID	(10)
#define 	DDMA_USB_DEVICE_ENDPOINT_2_TX_ID	(11)
#define 	DDMA_USB_DEVICE_ENDPOINT_3_RX_ID	(12)
#define 	DDMA_USB_DEVICE_ENDPOINT_4_RX_ID	(13)
#define		DDMA_PSC0_TX_ID						(14)
#define		DDMA_PSC0_RX_ID						(15)
#define		DDMA_PSC1_TX_ID						(16)
#define		DDMA_PSC1_RX_ID						(17)
#define		DDMA_PSC2_TX_ID						(18)
#define		DDMA_PSC2_RX_ID						(19)
#define		DDMA_PSC3_TX_ID						(20)
#define		DDMA_PSC3_RX_ID						(21)
#define		DDMA_PCI_PREAD_ID					(22)
#define		DDMA_NAND_FLASH_ID					(23)
/* IDs 24-29 are reserved */
#define 	DDMA_MEMORY_THROTTLE_ID				(30)
#define 	DDMA_ALWAYS_HIGH_ID					(31)

/*
 *	Physical address of ddma peripherals
 */
#define		DDMA_UART0_TX_ADDR					(0x11100004)
#define		DDMA_UART0_RX_ADDR					(0x11100000)
#define		DDMA_UART3_TX_ADDR					(0x11400004)
#define		DDMA_UART3_RX_ADDR					(0x11400000)
#define 	DDMA_USB_DEVICE_ENDPOINT_0_RX_ADDR	(0x10200000)
#define  	DDMA_USB_DEVICE_ENDPOINT_0_TX_ADDR	(0x10200004)
#define 	DDMA_USB_DEVICE_ENDPOINT_1_TX_ADDR	(0x10200008)
#define 	DDMA_USB_DEVICE_ENDPOINT_2_TX_ADDR	(0x1020000C)
#define 	DDMA_USB_DEVICE_ENDPOINT_3_RX_ADDR	(0x10200010)
#define 	DDMA_USB_DEVICE_ENDPOINT_4_RX_ADDR	(0x10200014)
#define		DDMA_PSC0_TX_ADDR					(0x11A0001C)
#define		DDMA_PSC0_RX_ADDR					(0x11A0001C)
#define		DDMA_PSC1_TX_ADDR					(0x11B0001C)
#define		DDMA_PSC1_RX_ADDR					(0x11B0001C)
#define		DDMA_PSC2_TX_ADDR					(0x10A0001C)
#define		DDMA_PSC2_RX_ADDR					(0x10A0001C)
#define		DDMA_PSC3_TX_ADDR					(0x10B0001C)
#define		DDMA_PSC3_RX_ADDR					(0x10B0001C)

#elif defined(SOC_AU13XX)

#define		DDMA_UART0_TX_ID					(0)
#define		DDMA_UART0_RX_ID					(1)
#define		DDMA_UART1_TX_ID					(2)
#define		DDMA_UART1_RX_ID					(3)
#define		DDMA_UART2_TX_ID					(4)
#define		DDMA_UART2_RX_ID					(5)
#define		DDMA_UART3_TX_ID					(6)
#define		DDMA_UART3_RX_ID					(7)
#define 	DDMA_SDMS0_TX_ID					(8)
#define  	DDMA_SDMS0_RX_ID					(9)
#define 	DDMA_SDMS1_TX_ID					(10)
#define 	DDMA_SDMS1_RX_ID					(11)
#define 	DDMA_AES_TX_ID						(12)
#define 	DDMA_AES_RX_ID						(13)
#define		DDMA_PSC0_TX_ID						(14)
#define		DDMA_PSC0_RX_ID						(15)
#define		DDMA_PSC1_TX_ID						(16)
#define		DDMA_PSC1_RX_ID						(17)
#define		DDMA_PSC2_TX_ID						(18)
#define		DDMA_PSC2_RX_ID						(19)
#define		DDMA_PSC3_TX_ID						(20)
#define		DDMA_PSC3_RX_ID						(21)
#define		DDMA_LCD_RETRACE_ID					(22)
#define		DDMA_NAND_FLASH_ID					(23)
#define 	DDMA_SDMS2_TX_ID					(24)
#define 	DDMA_SDMS2_RX_ID					(25)
#define		DDMA_CIM_FRAME_SYNC_ID				(26)
#define		DDMA_UDMA							(27)
#define		DDMA_REQ0_ID						(28)
#define		DDMA_REQ1_ID						(29)
#define 	DDMA_MEMORY_THROTTLE_ID				(30)
#define 	DDMA_ALWAYS_HIGH_ID					(31)

/*
 *	Physical address of ddma peripherals
 */
#define		DDMA_UART0_TX_ADDR					(0x10100004)
#define		DDMA_UART0_RX_ADDR					(0x10100000)
#define		DDMA_UART1_TX_ADDR					(0x10101004)
#define		DDMA_UART1_RX_ADDR					(0x10101000)
#define		DDMA_UART2_TX_ADDR					(0x10102004)
#define		DDMA_UART2_RX_ADDR					(0x10102000)
#define		DDMA_UART3_TX_ADDR					(0x10103004)
#define		DDMA_UART3_RX_ADDR					(0x10103000)
#define		DDMA_SDMS0_TX_ADDR					(0x10600000)
#define		DDMA_SDMS0_RX_ADDR					(0x10600004)
#define		DDMA_SDMS1_TX_ADDR					(0x10601000)
#define		DDMA_SDMS1_RX_ADDR					(0x10601004)
#define		DDMA_AES_TX_ADDR					(0x10300008)
#define		DDMA_AES_RX_ADDR					(0x10300004)
#define		DDMA_PSC0_TX_ADDR					(0x10a0001c)
#define		DDMA_PSC0_RX_ADDR					(0x10a0001c)
#define		DDMA_PSC1_TX_ADDR					(0x10a0101c)
#define		DDMA_PSC1_RX_ADDR					(0x10a0101c)
#define		DDMA_PSC2_TX_ADDR					(0x10a0201c)
#define		DDMA_PSC2_RX_ADDR					(0x10a0201c)
#define		DDMA_PSC3_TX_ADDR					(0x10a0301c)
#define		DDMA_PSC3_RX_ADDR					(0x10a0301c)
#define		DDMA_SDMS2_TX_ADDR					(0x10602000)
#define		DDMA_SDMS2_RX_ADDR					(0x10602004)
#define		DDMA_UDMA_ADDR						(0x140018a0)

#endif //SOC_AU13XX
/***********************************************************************/

#if defined(SOC_AU1550)

/*
PACKET ENGINE AND SYSTEM INTERFACE REGISTERS
*/
#define PE_CTRLSTAT			(0x0000)
#define PE_SOURCE			(0x0004)
#define PE_DEST				(0x0008)
#define PE_SA				(0x000C)
#define PE_LENGTH			(0x0010)

#define PE_DIVIDER			(0x0020)
#define PE_HOSTINT			(0x0024)
#define PE_BUFFSTAT			(0x0028)
#define PE_SBUSCFG			(0x002C)

#define PE_GLBDMACFG   	(0x0040)
#define PE_GLBDMASTAT  	(0x0044)
#define PE_GLBPDRBASE  	(0x0048)
#define PE_GLBRDRBASE  	(0x004C)
#define PE_GLBRSIZE		(0x0050)
#define PE_GLBRPOLL		(0x0054)
#define PE_GLBQSTAT		(0x0058)
#define PE_GLBEXTRSTAT	(0x005C)
#define PE_GLBTHRESH	(0x0060)

#define PE_GLBRAMTEST	(0x0074)				// DATABOOK ERROR? NOT DEFINED ANYWHERE?

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32	ctrlstat;			//(0x0000)
	uint32	source;				//(0x0004)
	uint32 	dest;				//(0x0008)
	uint32 	sa;					//(0x000C)
	uint32 	length;				//(0x0010)
	uint8	reserved[0x0C];		//(0x0014) 18 1C
	uint32 	divider;			//(0x0020)
	uint32 	hostint;			//(0x0024)
	uint32 	buffstat;			//(0x0028)
	uint32 	sbuscfg;			//(0x002C)
	uint8	reserved1[0x10];	//(0x0030) 34 38 3C
	uint32 	glbdmacfg;			//(0x0040)
	uint32 	glbdmastat;			//(0x0044)
	uint32 	glbpdrbase;			//(0x0048)
	uint32 	glbrdrbase;			//(0x004C)
	uint32 	glbrsize;			//(0x0050)
	uint32 	glbrpoll;			//(0x0054)
	uint32 	glbqstat;			//(0x0058)
	uint32 	glbextrstat;		//(0x005C)
	uint32 	glbthresh;			//(0x0060)
	uint8	reserved2[0x10];	//(0x0064) 68 6C 70
	uint32	glbramtest;			//(0x0074) // DATABOOK ERROR? NOT DEFINED ANYWHERE?
	uint8	reserved3[0x08];	//(0x0078) 7C
	uint32	devctrl;			//(0x0080)
	uint32	devid;				//(0x0084)
	uint32	devinfo;			//(0x0088)
} PE_CONFIG;
#endif

/*
DEVICE ID AND CONTROL REGISTERS
*/
#define PE_CRYPTO_CNTL		(0x0080)
#define PE_DEV_ID			(0x0084)
#define PE_DEV_INFO			(0x0088)


/*
INTERRUPT CONTROL REGISTERS
*/
#define PE_INT_HU_STAT		(0x00A0)
#define PE_INT_HM_STAT		(0x00A4) // 2 #defines at same address...
#define PE_INT_HI_CLR		(0x00A4) // Will this work? Or change to PE_INT_HM_STAT_HI_CLR?
#define PE_INT_HI_MASK		(0x00A8)
#define PE_INT_HI_CFG		(0x00AC)
#define PE_INT_HI_RD_DESC	(0x00B4)
#define PE_INT_HI_DESC_CNT	(0x00B8)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32	hu_stat;		//(0x00A0)
	union {
		uint32	hm_stat;   	//(0x00A4)
		uint32	hi_clr;	   	//(0x00A4)
	} u;
	uint32	hi_mask;		//(0x00A8)
	uint32	hi_cfg;			//(0x00AC)
	uint8	reserved;			//(0x00B0)
	uint32	hi_rd_desc;		//(0x00B4)
	uint32	hi_desc_cnt;	//(0x00B8)
} PE_INT_CNTL;
#endif

/*
DMA CONTROLLER REGISTERS
*/
#define PE_DMA_SOURCE		(0x00C4)
#define PE_DMA_DEST			(0x00C8)
#define PE_DMA_STAT			(0x00CC)

#define PE_DMA_BURST		(0x00D4)
#define PE_ENDIAN			(0x00E0)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32	source;			//(0x00C4)
	uint32	dest;			//(0x00C8)
	uint32	stat;			//(0x00CC)
	uint8	reserved;			//(0x00D0)
	uint32	burst;			//(0x00D4)
	uint8	reserved1[0x08];	//(0x00D8) DC
	uint32	endian;				//(0x00E0)
} PE_DMA;
#endif
/*
RNG REGISTERS
*/
#define PE_RNG_OUT			(0x0100)
#define PE_RNG_STAT			(0x0104)
#define PE_RNG_TEST_CNTL	(0x0108)
#define PE_RNG_ENTA			(0x010C)
#define PE_RNG_ENTB			(0x0110)
#define PE_RNG_SEED0		(0x0114)
#define PE_RNG_SEED1		(0x0118)
#define PE_RNG_SEED2		(0x011C)
#define PE_RNG_COUNT		(0x0120)
#define PE_RNG_ALARM		(0x0124)
#define PE_RNG_CFG			(0x0128)
#define PE_RNG_LFSR1_0		(0x012C)
#define PE_RNG_LFSR1_1		(0x0130)
#define PE_RNG_LFSR2_0		(0x0134)
#define PE_RNG_LFSR2_1		(0x0138)

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32	out;			//(0x0100)
	uint32	stat;			//(0x0104)
	uint32	test_cntl;		//(0x0108)
	uint32	enta;			//(0x010C)
	uint32	entb;			//(0x0110)
	uint32	seed0;			//(0x0114)
	uint32	seed1;			//(0x0118)
	uint32	seed2;			//(0x011C)
	uint32	count;			//(0x0120)
	uint32	alarm;			//(0x0124)
	uint32	cfg;			//(0x0128)
	uint32	lfsr1_0;		//(0x012C)
	uint32	lfsr1_1;		//(0x0130)
	uint32	lfsr2_0;		//(0x0134)
	uint32	lfsr2_1;		//(0x0138)
} PE_RNG;
#endif
/*
SA RECORD FORMAT
*/
#define PE_SA_CMD0			(0x0600)			// DATABOOT ERROR? USES 0X0000?
#define PE_SA_CMD1			(0x0604)			// DATABOOK ERROR? USES 0X0004?

#define PE_SA_KEY1_1		(0x0610)
#define PE_SA_KEY1_2		(0x0614)
#define PE_SA_KEY2_1		(0x0618)
#define PE_SA_KEY2_2		(0x061C)
#define PE_SA_KEY3_1		(0x0620)
#define PE_SA_KEY3_2		(0x0624)
#define PE_SA_KEY4_1		(0x0628)
#define PE_SA_KEY4_2		(0x062C)
#define PE_SA_IHD_1			(0x0630)
#define PE_SA_IHD_2			(0x0634)
#define PE_SA_IHD_3			(0x0638)
#define PE_SA_IHD_4			(0x063C)
#define PE_SA_IHD_5			(0x0640)
#define PE_SA_OHD_1			(0x0644)
#define PE_SA_OHD_2			(0x0648)
#define PE_SA_OHD_3			(0x064C)
#define PE_SA_OHD_4			(0x0650)
#define PE_SA_OHD_5			(0x0654)

#define PE_SA_IV_1			(0x066C)
#define PE_SA_IV_2			(0x0670)
#define PE_SA_IV_3			(0x0674)
#define PE_SA_IV_4			(0x0678)
#define PE_SA_HASH_BYTE_CNT	(0x067C)
#define PE_SA_IHD_1_MIR		(0x0680)
#define PE_SA_IHD_2_MIR		(0x0684)
#define PE_SA_IHD_3_MIR		(0x0688)
#define PE_SA_IHD_4_MIR		(0x068C)
#define PE_SA_IHD_5_MIR		(0x0690)
#define PE_SA_ICV_1			(0x0694)
#define PE_SA_ICV_2			(0x0698)
#define PE_SA_ICV_3			(0x069C)

#define PE_SA_DATA_IN_FIFO	(0x06A0)
#define PE_SA_DATA_OUT_FIFO	(0x06A4)


#ifndef ASSEMBLER
typedef volatile struct
{
	uint32	iv_1;			//not sure
	uint32	iv_2;			//not sure
	uint32	iv_3;			//not sure
	uint32	iv_4;			//not sure
	uint32	hash_byte_cnt;	//not sure
	uint32	ihd_1_mir;		//not sure
	uint32	ihd_2_mir;		//not sure
	uint32	ihd_3_mir;		//not sure
	uint32	ihd_4_mir;		//not sure
	uint32	ihd_5_mir;		//not sure
} STATE_RECORD;
#endif

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32	cmd0;				//(0x0600)			// DATABOOT ERROR? USES 0X0000?
	uint32	cmd1;				//(0x0604)			// DATABOOK ERROR? USES 0X0004?
	uint32	res1;
	uint32  res2;
//	uint8	reserved[0x8];		//(0x0608) 0C
	uint32	key1_1;				//(0x0610)
	uint32	key1_2;				//(0x0614)
	uint32	key2_1;				//(0x0618)
	uint32	key2_2;				//(0x061C)
	uint32	key3_1;				//(0x0620)
	uint32	key3_2;				//(0x0624)
	uint32	key4_1;				//(0x0628)
	uint32	key4_2;				//(0x062C)
	uint32	ihd_1;				//(0x0630)
	uint32	ihd_2;				//(0x0634)
	uint32	ihd_3;				//(0x0638)
	uint32	ihd_4;				//(0x063C)
	uint32	ihd_5;				//(0x0640)
	uint32	ohd_1;				//(0x0644)
	uint32	ohd_2;				//(0x0648)
	uint32	ohd_3;				//(0x064C)
	uint32	ohd_4;				//(0x0650)
	uint32	ohd_5;				//(0x0654)
	uint32	spi;				//(0x0658)
	uint32	seq_num;			//(0x065C)
	uint32	seq_num_mask1;		//(0x0660)
	uint32	seq_num_mask2;		//(0x0664)
	uint8	reserved1;			//(0x0668)
	uint32	ptr_st_rec;			//(0x066C)
	uint8	reserved2;			//(0x0670)
	uint8	reserved3;			//(0x0674)
	uint32	mgmt0;				//(0x0678)
	uint32	mgmt1;				//(0x067C)
} PE_SA_REV1;
#endif


#ifndef ASSEMBLER
typedef volatile struct
{
	uint32	cmd0;				//(0x0600)			// DATABOOT ERROR? USES 0X0000?
	uint32	cmd1;				//(0x0604)			// DATABOOK ERROR? USES 0X0004?
	uint8	reserved[0x8];		//(0x0608) 0C
	uint32	key1_1;				//(0x0610)
	uint32	key1_2;				//(0x0614)
	uint32	key2_1;				//(0x0618)
	uint32	key2_2;				//(0x061C)
	uint32	key3_1;				//(0x0620)
	uint32	key3_2;				//(0x0624)
	uint32	ihd_1;				//(0x0628)
	uint32	ihd_2;				//(0x062C)
	uint32	ihd_3;				//(0x0630)
	uint32	ihd_4;				//(0x0634)
	uint32	ihd_5;				//(0x0638)
	uint32	ohd_1;				//(0x063C)
	uint32	ohd_2;				//(0x0640)
	uint32	ohd_3;				//(0x0644)
	uint32	ohd_4;				//(0x0648)
	uint32	ohd_5;				//(0x064C)
	uint32	spi;				//(0x0650)
	uint32	seq_num;			//(0x0654)
	uint32	seq_num_mask1;		//(0x0658)
	uint32	seq_num_mask2;		//(0x065C)
	uint8	reserved1;			//(0x0660)
	uint32	sa_ptr_st_rec;		//(0x0664)
	uint8	reserved2;			//(0x0668)
	uint8	reserved3;			//(0x066C)
	uint8	reserved4;			//(0x0670)
	uint8	reserved5;			//(0x0674)
	uint32	mgmt0;				//(0x0678)
	uint32	mgmt1;				//(0x067C)

} PE_SA_REV0;
 #endif


#ifndef ASSEMBLER
typedef volatile struct
{
	uint32	cmd0;				//(0x0600)			// DATABOOT ERROR? USES 0X0000?
	uint32	cmd1;				//(0x0604)			// DATABOOK ERROR? USES 0X0004?
	uint8	reserved[0x8];		//(0x0608) 0C
	uint32	key1_1;				//(0x0610)
	uint32	key1_2;				//(0x0614)
	uint32	key2_1;				//(0x0618)
	uint32	key2_2;				//(0x061C)
	uint8	reserved1[0x10];	//(0x0620) 24 28 2C
	uint32	arc4_ij;			//(0x0630)
	uint32	ptr_st_rec;			//(0x0634)
	uint8	reserved2[0x40];	//(0x0638)
	uint32	mgmt0;				//(0x0678)
	uint32	mgmt1;				//(0x067C)

} PE_SA_ARC4;
#endif



/************************************
*									*
* AU1550 ENCRYPTION BIT DEFINITIONS	*
*									*
************************************/


/*
SYSTEM INTERFACE REGISTERS
*/
#define PE_DIVIDER_CDIV			(3<<1)				// Clock divider for packet engine
#define PE_DIVIDER_CE			(1<<0)				// Clock enable for packet engine
#define PE_HOSTINT_HINT			(1<<0)				// Host interrupt
#define PE_BUFFSTAT_STAT		(1<<0)				// Buffer status
#define PE_SBUSCFG_ADDR			(0xF<<4)			// Most significant nibble of system address
#define PE_SBUSCFG_C			(1<<1)				// Coherent (enable caching)
#define PE_SBUSCFG_PRI			(1<<0)				// Priority boost for system bus arbitration


/*
PACKET ENGINE GLOBAL REGISTERS
*/
#define PE_DMA_CFG_SUP			(1<<20)				// Suppress PDR ownership update
#define PE_DMA_CFG_EP			(1<<18)				// Apply endian swap for packet data
#define PE_DMA_CFG_ESA			(1<<17)				// Apply endian swap for SA data
#define PE_DMA_CFG_EPD			(1<<16)				// Apply endian swap for packet descriptors
#define PE_DMA_CFG_PFD			(1<<10)				// Packet follows descriptor
#define PE_DMA_CFG_SAP			(1<<9)				// SA precedes packet
#define PE_DMA_CFG_PE			(1<<8)				// Packet engine mode
#define PE_DMA_CFG_RP			(1<<1)				// Reset packet counter/pointers
#define PE_DMA_CFG_RPE			(1<<0)

#define PE_DMA_STAT_OUTSIZ		(0x3FF<<22)			// Output size
#define PE_DMA_STAT_INSIZ		(0x3FF<<12)			// Input size
#define PE_DMA_STAT_CA			(1<<9)				// Command queue active
#define PE_DMA_STAT_SPI			(1<<7)				// SPI mismatch
#define PE_DMA_STAT_ICV			(1<<6)				// ICV fault
#define PE_DMA_STAT_PF			(1<<5)				// Crypto pad fault
#define PE_DMA_STAT_OHD			(1<<4)				// Outer hash done
#define PE_DMA_STAT_IHD			(1<<3)				// Inner hash done
#define PE_DMA_STAT_ED			(1<<2)				// Encryption done
#define PE_DMA_STAT_OD			(1<<1)				// Output done
#define PE_DMA_STAT_ID			(1<<0)				// Input done

#define PE_PDR_BASE				(0xFFFFFFFF<<0)		// PDR base address
#define PE_RDR_BASE				(0xFFFFFFFF<<0)		// RDR base address

#define PE_RING_SIZE_OFFSET		(0xFFFF<<16)		// Descriptor ring offset
#define PE_RING_SIZE_SIZE	   	(0x1FF<<0)			// Descriptor ring size

#define PE_RING_POLL_C			(1<<31)				// Continuous
#define PE_RING_POLL_RETDIV		(0x3FF<<16)			// Ring retry divisor
#define PE_RING_POLL_POLLDIV	(0xFFF<<0)			// Ring poll divisor

#define PE_INT_RING_STAT_CQA	(1<<0)				// Command queue available

#define PE_EXT_RING_STAT		(0x3FF<<16)			// Index of next packet descriptor

#define PE_IO_THRESHOLD_OT		(0xF<<16)			// Output threshold
#define PE_IO_THRESHOLD_IT		(0xF<<0)			// Input threshold


/*
PACKET ENGINE DESCRIPTOR REGISTERS
*/
#define PE_CTRLSTAT_PCS			(0x1F<<27)			// Pad control/status
#define PE_CTRLSTAT_EC			(0xF<<20)			// Extended code
#define PE_CTRLSTAT_EE			(1<<19)				// Extended error
#define PE_CTRLSTAT_SN			(1<<18)				// Sequence number fail
#define PE_CTRLSTAT_PV			(1<<17)				// Pad verify fail
#define PE_CTRLSTAT_A			(1<<16)				// Authentication fail
#define PE_CTRLSTAT_NH			(0xFF<<8)			// Next header/pad value
#define PE_CTRLSTAT_HF			(1<<4)				// Hash final
#define PE_CTRLSTAT_NK			(1<<3)				// New key ARC4
#define PE_CTRLSTAT_LHD			(1<<2)				// Load SA hash digests
#define PE_CTRLSTAT_CD_HR  		(3<<0)				// Encryption core done and Host ready
#define PE_CTRLSTAT_HD			(1<<0)
#define PE_CTRLSTAT_ED			(1<<1)

#define PE_LENGTH_CD_HR			(3<<22)				// Encryption core done and host ready
#define PE_LENGTH_HD			(1<<22)
#define PE_LENGTH_ED			(1<<23)

#define PE_LENGTH_LENGTH		(0xFFFFF<<0)		// Length


/*
SA RECORD FORMAT
*/

#define PE_SA_CMD0_SH			(1<<29)				// Save hash state
#define PE_SA_CMD0_SIV			(1<<28)				// Save IV
#define PE_SA_CMD0_LH			(0x3<<26)			// Load hash state
#define PE_SA_CMD0_LIV			(0x3<<24)			// Load IV
#define PE_SA_CMD0_HP			(1<<19)				// Header processing
#define PE_SA_CMD0_HA			(0xF<<12)			// Hash algorithm
#define PE_SA_CMD0_CA			(0xF<<8)			// Crypto algorithm
#define PE_SA_CMD0_CP			(0x3<<6)			// Crypto pad
#define PE_SA_CMD0_OPG			(0x3<<4)			// Operation group
#define PE_SA_CMD0_IO			(1<<3)				// Inbound/Outbound
#define PE_SA_CMD0_OPC			(0x7<<0)			// Operation code

#define PE_SA_CMD1_SS			(1<<30)				// Save ARC4 state
#define PE_SA_CMD1_AS			(1<<29)				// ARC4 stateless/stateful
#define PE_SA_CMD1_KLEN			(0x1F<<24)			// ARC4 key length
#define PE_SA_CMD1_CHOFF		(0xFF<<16)			// Hash / encrypt offset
#define PE_SA_CMD1_SA			(1<<15)				// SA revision
#define PE_SA_CMD1_HM			(1<<12)				// HMAC control
#define PE_SA_CMD1_CM			(3<<1)				// Cryptographic mode
#define PE_SA_CMD1_MB			(1<<5)				// Mutable bit handling
#define PE_SA_CMD1_IP			(1<<4)				// IPv4 / IPv6
#define PE_SA_CMD1_PAD			(1<<3)				// Copy inbound pad to output
#define PE_SA_CMD1_PAY  		(1<<2)				// Copy payload to output
#define PE_SA_CMD1_HD   		(1<<1)				// Copy header to output



/*
DMA REGISTERS
*/


#define PE_DMA_STAT_MTA			(1<<19)				// Master transfer active
#define PE_DMA_STAT_TLEN		(0xFFF<<0)			// Transfew length

#define PE_DMA_BURST_MAX_TSIZE	(0x3FF<<2)			// Maximum transfer size

#define PE_ENDIAN_SBL3			(3<<22)				// Byte lane sources for byte 3
#define PE_ENDIAN_SBL2			(3<<20)				// Byte lane sources for byte 2
#define PE_ENDIAN_SBL1			(3<<18)				// Byte lane sources for byte 1
#define PE_ENDIAN_SBL0			(3<<16)				// Byte lane sources for byte 0
#define PE_ENDIAN_MBL3			(3<<6)				// Byte lane sources for byte 3
#define PE_ENDIAN_MBL2			(3<<4)				// Byte lane sources for byte 2
#define PE_ENDIAN_MBL1  		(3<<2)				// Byte lane sources for byte 1
#define PE_ENDIAN_MBL0			(3<<0)				// Byte lane sources for byte 0

/*
RANDOM NUMBER GENERATOR REGISTERS
*/

#define PE_RNG_STAT_B			(1<<0)				// Busy

#define PE_RNG_TEST_CNTL_RL		(1<<10)				// Reset LFSRs
#define PE_RNG_TEST_CNTL_TL		(1<<9)				// Test LFSRs
#define PE_RNG_TEST_CNTL_TA		(1<<8)				// Test alarm
#define PE_RNG_TEST_CNTL_SC		(1<<7)				// Short cycle
#define PE_RNG_TEST_CNTL_TC		(1<<6)				// Test counter
#define PE_RNG_TEST_CNTL_DA		(1<<5)				// Disable alarm
#define PE_RNG_TEST_CNTL_TR2	(1<<4)				// Test ring1
#define PE_RNG_TEST_CNTL_TR1	(1<<3)				// Test ring2
#define PE_RNG_TEST_CNTL_T		(1<<2)				// Test run
#define PE_RNG_TEST_CNTL_TM		(1<<1)				// Test mode
#define PE_RNG_TEST_CNTL_TO		(1<<0)				// Test ring output

#define PE_RNG_ENTA_ENTA		(0xFFFF<<0)			// Entropy A
#define PE_RNG_ENTB_ENTB		(0xFFFF<<0)			// Entropy B

#define PE_RNG_CFG_RCOUNT		(0x3F<<0)			// Reset count
#define PE_RNG_CFG_R2D		    (0x3<<3)			// Ring2 delay
#define PE_RNG_CFG_R1D  		(0x3<<0)			// Ring1 delay
#define PE_RNG_CFG_LFSR1_0 		(0xFFFFFFFF<<0)		// Bits [31:0] of 49-bit LFSR1
#define PE_RNG_CFG_LFSR1_1 		(0x1FFFF<<0)		// Bits [48:32] of 49-bit LFSR1
													// DATABOOK ERROR? NOT CONSISTENT WITH LFSR2

#define PE_RNG_CFG_LFSR2_0 		(0xFFFFFFFF<<0)		// Bits [31:0] of 48-bit LFSR2
#define PE_RNG_CFG_LFSR2_1 		(0x1FFFF<<0)		// Bits [47:32] of 48-bit LFSR2
													// DATABOOK ERROR? NOT CONSISTENT WITH LFSR1

/*
INTERRUPT CONTROL REGISTERS
*/
#define PE_INT_HU_STAT_CD		(1<<9)				// Packet engine context done
#define PE_INT_HU_STAT_ER		(1<<4)				// Packet engine error
#define PE_INT_HU_STAT_DD		(1<<3)				// Packet engine descriptor done
#define PE_INT_HU_STAT_QD		(1<<1)				// Command queue done

#define PE_INT_HM_STAT_CD		(1<<9)				// Packet engine context done
#define PE_INT_HM_STAT_ER		(1<<4)				// Packet engine error
#define PE_INT_HM_STAT_DD		(1<<3)				// Packet engine descriptor done
#define PE_INT_HM_STAT_QD		(1<<1)				// Command queue done

#define PE_INT_HI_CLR_CD		(1<<9)				// Packet engine context done
#define PE_INT_HI_CLR_ER		(1<<4)				// Packet engine error
#define PE_INT_HI_CLR_DD		(1<<3)				// Packet engine descriptor done
#define PE_INT_HI_CLR_QD		(1<<1)				// Command queue done

#define PE_INT_HI_MASK_CD		(1<<9)				// Packet engine context done
#define PE_INT_HI_MASK_ER		(1<<4)				// Packet engine error
#define PE_INT_HI_MASK_DD		(1<<3)				// Packet engine descriptor done
#define PE_INT_HI_MASK_QD		(1<<1)				// Command queue done

#define PE_INT_HI_CFG_PSC		(1<<1)				// Pulse self-clear
#define PE_INT_HI_CFG_TYP		(1<<0)				// Interrupt host output type


/*
DEVICE ID AND CONTROL REGISTERS
*/

#define PE_CRYPTO_CNTL_RNE		(1<<17)				// RNG enable
#define PE_CRYPTO_CNTL_3DE		(1<<17)				// 3-DES enable

#define PE_DEV_ID_VENID			(0xFFFF<<16)		// Vendor ID
#define PE_DEV_ID_DEVID			(0xFFFF<<0)			// Device ID

#define PE_DEV_INFO_SFC			(0xFFFF<<8)			// Supported function code
#define PE_DEV_INFO_REV			(0xFFFF<<0)			// Revision

#ifndef ASSEMBLER
typedef volatile struct
{
	PE_CONFIG 	cfg;			//0x0000 - 0x0088
	uint8		res0[20];		//0x008c - 0x0090
    PE_INT_CNTL intctl;			//0x00A0 - 0x00B8
	uint8		res1[8];		//0x00BC - 0x00C0
	PE_DMA		dma;			//0x00C4 - 0x00E0
	uint8		res2[28];		//0x00E4 - 0x00FC
	PE_RNG		rng;			//0x0100 - 0x0138
	/* There are some context registers that are not defined in this sturcture */
	/* The space is included so the mapping to virtual address will cover entire block */
	uint8		context[1388];	//0x013C - 0x06A4

} AU1550_CRYPTO;
#endif

#endif //#if defined(SOC_AU1550)

/***********************************************************************/

#define PSC_SEL					0x00000000
#define PSC_CTL					0x00000004

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32 	sel;							// PSC Select
	uint32	ctl;							// PSC control
} PSC_SELECT;
#endif

#define	PSC_SEL_PS				(0x7<<0)	// Protocol select
#define PSC_SEL_PS_N(n)			((n&0x7)<<0)
#define	PSC_SEL_CLK				(0x3<<4)	// Clock Source
#define PSC_SEL_CLK_N(n) 		((n&0x3)<<4)
#define PSC_SEL_CLK_TOY			PSC_SEL_CLK_N(0)
#define PSC_SEL_CLK_OFFCHIP		PSC_SEL_CLK_N(1)
#define PSC_SEL_CLK_SERIAL		PSC_SEL_CLK_N(2)
#define PSC_CTL_CE				(1<<0)		// Clock Enable
#define PSC_CTL_EN				(1<<1)		// PSC Enable

#define PSC_SEL_PS_SPI			(PSC_SEL_PS_N(2))
#define PSC_SEL_PS_I2S			(PSC_SEL_PS_N(3))
#define PSC_SEL_PS_AC97			(PSC_SEL_PS_N(4))
#define PSC_SEL_PS_SMB			(PSC_SEL_PS_N(5))

/*
 *	Some simple defines for passing to functions
 */
#define _PSC_SPI 				PSC_SEL_PS_SPI
#define _PSC_I2S			  	PSC_SEL_PS_I2S
#define _PSC_AC97			  	PSC_SEL_PS_AC97
#define _PSC_SMB				PSC_SEL_PS_SMB


/*
################################################################################################
#####                         SPI  Register Definitions                                    #####
################################################################################################
*/

#define PSC_SPI_SEL			PSC_SEL
#define PSC_SPI_CTL			PSC_CTL
#define PSC_SPI_CFG			0x00000008
#define PSC_SPI_MSK			0x0000000C
#define PSC_SPI_PCR			0x00000010
#define PSC_SPI_STS			0x00000014
#define PSC_SPI_EVNT		0x00000018
#define PSC_SPI_TXRX		0x0000001C

#ifndef ASSEMBLER
typedef volatile struct
{
	PSC_SELECT	psc;		//00,04
	uint32		cfg;		//08
	uint32		msk;		//0C
	uint32		pcr;		//10
	uint32		sts;		//14
	uint32		evnt;		//18
	uint32		txrx;		//1C
} PSC_SPI;
#endif

#define PSC_SPI_CFG_MO			(1<<0)		// Master Only Mode
#define PSC_SPI_CFG_MLF			(1<<1)		// MSB/LSB Data First
#define PSC_SPI_CFG_LB			(1<<3)		// Loopback mode
#define PSC_SPI_CFG_LEN			(0x1F<<4)	// Length mask
#define PSC_SPI_CFG_LEN_N(n)	((n&0x1F)<<4)	// Length 'n'
#define PSC_SPI_CFG_CDE			(1<<9)		// Clock Phase Delay
#define	PSC_SPI_CFG_CCE			(1<<10)		// Clock Chop Enable
#define	PSC_SPI_CFG_PSE			(1<<11)		// Port Swap Enable
#define	PSC_SPI_CFG_BI			(1<<12)		// Bit Clock Invert
#define	PSC_SPI_CFG_DIV			(0x3<<13)	// Clock Divider
#define	PSC_SPI_CFG_DIV_N(n)	((n&0x3)<<13)
#define	PSC_SPI_CFG_BRG			(0x3F<<15)	// Baud Rate Generator
#define	PSC_SPI_CFG_BRG_N(n)	((n&0x3F)<<15)
#define	PSC_SPI_CFG_DE			(1<<26)		//Device Enable
#define	PSC_SPI_CFG_DD			(1<<27)		// Diable DMA
#define	PSC_SPI_CFG_TRD			(0x3<<28)	// TX Request Depth
#define	PSC_SPI_CFG_TRD_N(n)	((n&0x3)<<28)
#define	PSC_SPI_CFG_RRD			(0x3<<30)	// Rx Request Depth
#define	PSC_SPI_CFG_RRD_N(n)	((n&0x3)<<30)

#define	PSC_SPI_MSK_MD			(1<<4)		// Master Done
#define	PSC_SPI_MSK_SD			(1<<5)		// Slave done
#define	PSC_SPI_MSK_TU			(1<<8)		// TX Fifo underflow
#define	PSC_SPI_MSK_TO			(1<<9)		// TX fifo overflow
#define	PSC_SPI_MSK_TR			(1<<10)		// TX Fifo request
#define	PSC_SPI_MSK_RU			(1<<11)		// RX Fifo underflow
#define	PSC_SPI_MSK_RO			(1<<12)		// RX fifo overflow
#define	PSC_SPI_MSK_RR			(1<<13)		// RX Fifo request
#define	PSC_SPI_MSK_MM			(1<<16)		// Multiple Master Error

#define PSC_SPI_PCR_MS			(1<<0)		// Master Start
#define PSC_SPI_PCR_TC			(1<<2)		// Tx Data Clear
#define PSC_SPI_PCR_SS			(1<<4)		// Slave Start
#define PSC_SPI_PCR_SP			(1<<5)		// Slave Stop
#define PSC_SPI_PCR_RC			(1<<6)		// Rx Data Clear

// Status Register is Read Only //
#define PSC_SPI_STS_SR			(1<<0)		// PSC Ready
#define PSC_SPI_STS_DR			(1<<1) 		// Device Ready
#define PSC_SPI_STS_DI			(1<<2)		// Device INterrupt
#define PSC_SPI_STS_MB			(1<<4)		// Master BUsy
#define PSC_SPI_STS_SB			(1<<5)		// Slave Busy
#define PSC_SPI_STS_TR			(1<<8)		// Tx Rrequest
#define PSC_SPI_STS_TE			(1<<9)		// Tx Fifo Empty
#define PSC_SPI_STS_TF			(1<<10)		// Tx Fifo Full
#define PSC_SPI_STS_RR			(1<<11)		// Rx Rrequest
#define PSC_SPI_STS_RE			(1<<12)		// Rx Fifo Empty
#define PSC_SPI_STS_RF			(1<<13)		// Rx Fifo Full

#define	PSC_SPI_EVNT_MD			(1<<4)		// Master Done
#define	PSC_SPI_EVNT_SD			(1<<5)		// Slave done
#define	PSC_SPI_EVNT_TU			(1<<8)		// TX Fifo underflow
#define	PSC_SPI_EVNT_TO			(1<<9)		// TX fifo overflow
#define	PSC_SPI_EVNT_TR			(1<<10)		// TX Fifo request
#define	PSC_SPI_EVNT_RU			(1<<11)		// RX Fifo underflow
#define	PSC_SPI_EVNT_RO			(1<<12)		// RX fifo overflow
#define	PSC_SPI_EVNT_RR			(1<<13)		// RX Fifo request
#define	PSC_SPI_EVNT_MM			(1<<16)		// Multiple Master Error

#define PSC_SPI_TXRX_DATA		(0xFFFFFF<<0)	// Data
#define PSC_SPI_TXRX_DATA_N(n)	((n&0xFFFFFF)<<0)
#define PSC_SPI_TXRX_ST			(1<<28)		// Slave Select Toggle
#define PSC_SPI_TXRX_LC			(1<<29)		// Last Character -- Non-DMA only


/*
################################################################################################
#####                          I2S Register Definitions                                    #####
################################################################################################
*/

#define PSC_I2S_SEL			PSC_SEL
#define PSC_I2S_CTL			PSC_CTL
#define PSC_I2S_CFG			0x00000008
#define PSC_I2S_MSK			0x0000000C
#define PSC_I2S_PCR			0x00000010
#define PSC_I2S_STS			0x00000014
#define PSC_I2S_EVNT		0x00000018
#define PSC_I2S_TXRX		0x0000001C

#ifndef ASSEMBLER
typedef volatile struct
{
	PSC_SELECT	psc;		//00,04
	uint32		cfg;		//08
	uint32		msk;		//0C
	uint32		pcr;		//10
	uint32		sts;		//14
	uint32		evnt;		//18
	uint32		txrx;		//1C
} PSC_I2S;
#endif

#define	PSC_I2S_CFG_MS			(1<<0)		// Master/Slafe
#define	PSC_I2S_CFG_MLF			(1<<1)		// MSB/LSB first
#define	PSC_I2S_CFG_LB			(1<<2)		// Loopback Mode
#define PSC_I2S_CFG_LEN			(0x1F<<4)	// Length mask
#define PSC_I2S_CFG_LEN_N(n)	((n&0x1F)<<4)	// Length 'n'
#define	PSC_I2S_CFG_XM			(1<<9)		// Transfer Mode
#define	PSC_I2S_CFG_MLJ			(1<<10)		// MSB/LSB Justified
#define	PSC_I2S_CFG_BUF			(1<<11)		// L/R Channel Buffer
#define	PSC_I2S_CFG_BI			(1<<12)		// Bit clock invert
#define PSC_I2S_CFG_BDIV  		(0x3<<13)	// Bit Clock Divider
#define PSC_I2S_CFG_BDIV_N(n)	((n&0x3)<<13)
#define	PSC_I2S_CFG_WI			(1<<15)		// Word Strobe Invert
#define	PSC_I2S_CFG_WS			(1<<16)		// Word Strobe
#define	PSC_I2S_CFG_WS_N(n)		((n&0xFF)<<16)
#define	PSC_I2S_CFG_DE			(1<<26)		// Device Enable
#define PSC_I2S_CFG_DD			(1<<27)     // Disable DDMA
#define	PSC_I2S_CFG_TBS			(0x3<<28)	// TX Burst Size
#define	PSC_I2S_CFG_TBS_N(n)	((n&0x3)<<28)
#define	PSC_I2S_CFG_RBS			(0x3<<30)	// Rx Burst Size
#define	PSC_I2S_CFG_RBS_N(n)	((n&0x3)<<30)

#define PSC_I2S_CFG_DIV2		PSC_I2S_CFG_BDIV_N(0)
#define PSC_I2S_CFG_DIV4		PSC_I2S_CFG_BDIV_N(1)
#define PSC_I2S_CFG_DIV8		PSC_I2S_CFG_BDIV_N(2)
#define PSC_I2S_CFG_DIV16		PSC_I2S_CFG_BDIV_N(3)


#define	PSC_I2S_MSK_TD			(1<<4)		// Tx Done
#define	PSC_I2S_MSK_RD			(1<<5)		// Rx Done
#define	PSC_I2S_MSK_TU			(1<<8)		// TX Fifo underflow
#define	PSC_I2S_MSK_TO			(1<<9)		// TX fifo overflow
#define	PSC_I2S_MSK_TR			(1<<10)		// TX Fifo request
#define	PSC_I2S_MSK_RU			(1<<11)		// RX Fifo underflow
#define	PSC_I2S_MSK_RO			(1<<12)		// RX fifo overflow
#define	PSC_I2S_MSK_RR			(1<<13)		// RX Fifo request

#define PSC_I2S_PCR_TS			(1<<0)		// Tx start
#define PSC_I2S_PCR_TP			(1<<1)		// Tx Stop
#define PSC_I2S_PCR_TC			(1<<2)		// Tx Data Clear
#define PSC_I2S_PCR_RS			(1<<4)		// Rx Start
#define PSC_I2S_PCR_RP			(1<<5)		// Rx Stop
#define PSC_I2S_PCR_RC			(1<<6)		// Rx Data Clear

// Status Register is Read Only //
#define PSC_I2S_STS_SR			(1<<0)		// PSC Ready
#define PSC_I2S_STS_DR			(1<<1) 		// Device Ready
#define PSC_I2S_STS_DI			(1<<2)		// Device INterrupt
#define PSC_I2S_STS_TB			(1<<4)		// Tx BUsy
#define PSC_I2S_STS_RB			(1<<5)		// Rx Busy
#define PSC_I2S_STS_TR			(1<<8)		// Tx Rrequest
#define PSC_I2S_STS_TE			(1<<9)		// Tx Fifo Empty
#define PSC_I2S_STS_TF			(1<<10)		// Tx Fifo Full
#define PSC_I2S_STS_RR			(1<<11)		// Rx Rrequest
#define PSC_I2S_STS_RE			(1<<12)		// Rx Fifo Empty
#define PSC_I2S_STS_RF			(1<<13)		// Rx Fifo Full

#define	PSC_I2S_EVNT_TD			(1<<4)		// Tx Done
#define	PSC_I2S_EVNT_RD			(1<<5)		// Rx Done
#define	PSC_I2S_EVNT_TU			(1<<8)		// TX Fifo underflow
#define	PSC_I2S_EVNT_TO			(1<<9)		// TX fifo overflow
#define	PSC_I2S_EVNT_TR			(1<<10)		// TX Fifo request
#define	PSC_I2S_EVNT_RU			(1<<11)		// RX Fifo underflow
#define	PSC_I2S_EVNT_RO			(1<<12)		// RX fifo overflow
#define	PSC_I2S_EVNT_RR			(1<<13)		// RX Fifo request

#define PSC_I2S_TXRX_DATA		(0xFFFFFF<<0)	// Data
#define PSC_I2S_TXRX_DATA_N(n)	((n&0xFFFFFF)<<0)

/*
################################################################################################
#####                         AC97 Register Definitions                                    #####
################################################################################################
*/

#define PSC_AC97_SEL	 	PSC_SEL
#define PSC_AC97_CTL	 	PSC_CTL
#define PSC_AC97_CFG	 	0x00000008
#define PSC_AC97_MSK	 	0x0000000C
#define PSC_AC97_PCR	 	0x00000010
#define PSC_AC97_STS	 	0x00000014
#define PSC_AC97_EVNT	 	0x00000018
#define PSC_AC97_TXRX	 	0x0000001C
#define PSC_AC97_CDC		0x00000020
#define PSC_AC97_RST		0x00000024
#define PSC_AC97_GPO		0x00000028
#define PSC_AC97_GPI		0x0000002C

#ifndef ASSEMBLER
typedef volatile struct
{
	PSC_SELECT	psc;		//00,04
	uint32		cfg;		//08
	uint32		msk;		//0C
	uint32		pcr;		//10
	uint32		sts;		//14
	uint32		evnt;		//18
	uint32		txrx;		//1C
	uint32		cdc;		//20
	uint32		rst;		//24
	uint32		gpo;		//28
	uint32		gpi;		//2C
} PSC_AC97;
#endif

#define PSC_AC97_CFG_GE			(1<<0)		// GPIO Register Enable
#define PSC_AC97_CFG_RXSLOT		(0x3FF<<1)	// Valid Rx slots
#define PSC_AC97_CFG_RXSLOT_N(n) ((n&0x3FF)<<1)
#define PSC_AC97_CFG_TXSLOT		(0x3FF<<11)	// Valid Tx slots
#define PSC_AC97_CFG_TXSLOT_N(n) ((n&0x3FF)<<11)
#define PSC_AC97_CFG_LEN		(0x1F<<21)	// Data Length
#define PSC_AC97_CFG_LEN_N(n) ((n&0x1F)<<21)
#define PSC_AC97_CFG_DE			(1<<26)		// Device Enable
#define PSC_AC97_CFG_DD			(1<<27)		// DMA Disable
#define	PSC_AC97_CFG_TRD	  	(0x3<<28)	// TX Request Depth
#define	PSC_AC97_CFG_TRD_N(n)	((n&0x3)<<28)
#define	PSC_AC97_CFG_RRD	  	(0x3<<30)	// Rx Request Depth
#define	PSC_AC97_CFG_RRD_N(n)	((n&0x3)<<30)

#define	PSC_AC97_MSK_TD			(1<<4)		// Tx Done
#define	PSC_AC97_MSK_RD			(1<<5)		// Rx Done
#define	PSC_AC97_MSK_TU			(1<<8)		// TX Fifo underflow
#define	PSC_AC97_MSK_TO			(1<<9)		// TX fifo overflow
#define	PSC_AC97_MSK_TR			(1<<10)		// TX Fifo request
#define	PSC_AC97_MSK_RU			(1<<11)		// RX Fifo underflow
#define	PSC_AC97_MSK_RO			(1<<12)		// RX fifo overflow
#define	PSC_AC97_MSK_RR			(1<<13)		// RX Fifo request
#define	PSC_AC97_MSK_CD			(1<<24)		// CODEC Command done
#define	PSC_AC97_MSK_GR			(1<<25)		// GPI Data Ready Interrupt

#define PSC_AC97_PCR_TS			(1<<0)		// Tx start
#define PSC_AC97_PCR_TP			(1<<1)		// Tx Stop
#define PSC_AC97_PCR_TC			(1<<2)		// Tx Data Clear
#define PSC_AC97_PCR_RS			(1<<4)		// Rx Start
#define PSC_AC97_PCR_RP			(1<<5)		// Rx Stop
#define PSC_AC97_PCR_RC			(1<<6)		// Rx Data Clear

// Status Register is Read Only //
#define PSC_AC97_STS_SR			(1<<0)		// PSC Ready
#define PSC_AC97_STS_DR			(1<<1) 		// Device Ready
#define PSC_AC97_STS_DI			(1<<2)		// Device INterrupt
#define PSC_AC97_STS_TB			(1<<4)		// Tx BUsy
#define PSC_AC97_STS_RB			(1<<5)		// Rx Busy
#define PSC_AC97_STS_TR			(1<<8)		// Tx Rrequest
#define PSC_AC97_STS_TE			(1<<9)		// Tx Fifo Empty
#define PSC_AC97_STS_TF			(1<<10)		// Tx Fifo Full
#define PSC_AC97_STS_RR			(1<<11)		// Rx Rrequest
#define PSC_AC97_STS_RE			(1<<12)		// Rx Fifo Empty
#define PSC_AC97_STS_RF			(1<<13)		// Rx Fifo Full
#define PSC_AC97_STS_CR			(1<<24)		// Codec ready
#define PSC_AC97_STS_CP			(1<<25)		// Command Pending
#define PSC_AC97_STS_CB			(1<<26)		// Codec Bit Clock detected

#define	PSC_AC97_EVNT_TD   		(1<<4)		// Tx Done
#define	PSC_AC97_EVNT_RD   		(1<<5)		// Rx Done
#define	PSC_AC97_EVNT_TU   		(1<<8)		// TX Fifo underflow
#define	PSC_AC97_EVNT_TO   		(1<<9)		// TX fifo overflow
#define	PSC_AC97_EVNT_TR   		(1<<10)		// TX Fifo request
#define	PSC_AC97_EVNT_RU   		(1<<11)		// RX Fifo underflow
#define	PSC_AC97_EVNT_RO   		(1<<12)		// RX fifo overflow
#define	PSC_AC97_EVNT_RR   		(1<<13)		// RX Fifo request
#define	PSC_AC97_EVNT_CD   		(1<<24)		// CODEC Command done
#define	PSC_AC97_EVNT_GR   		(1<<25)		// GPI Data Ready Interrupt

#define PSC_AC97_TXRX_DATA		(0xFFFFF<<0)	// Data
#define PSC_AC97_TXRX_DATA_N(n)	((n&0xFFFFF)<<0)

#define PSC_AC97_CDC_DATA		(0xFFFF<<0)	// data
#define PSC_AC97_CDC_DATA_N(n) 	((n&0xFFFF)<<0)
#define PSC_AC97_CDC_INDX		(0x7F<<16)	// register index
#define PSC_AC97_CDC_INDX_N(n) 	((n&0x7F)<<16)
#define PSC_AC97_CDC_ID			(0x3<<23)	// 2bit id for codec
#define PSC_AC97_CDC_ID_N(n) 	((n&0x3)<<23)
#define PSC_AC97_CDC_RD			(1<<25)		// Codec Read/Write

#define PSC_AC97_RST_SNC		(1<<0)		// Sync signal control
#define PSC_AC97_RST_RST		(1<<1)		// AC link reset

#define PSC_AC97_GPO_DATA		(0xFFFFF<<0)
#define PSC_AC97_GPO_DATA_N(n)	((n&0xFFFFF)<<0)

#define PSC_AC97_GPI_DATA		(0xFFFFF<<0)
#define PSC_AC97_GPI_DATA_N(n)	((n&0xFFFFF)<<0)


/*
################################################################################################
#####                        SMBus Register Definitions                                    #####
################################################################################################
*/

#define PSC_SMB_SEL	 		PSC_SEL
#define PSC_SMB_CTL	 		PSC_CTL
#define PSC_SMB_CFG	 		0x00000008
#define PSC_SMB_MSK	 		0x0000000C
#define PSC_SMB_PCR	 		0x00000010
#define PSC_SMB_STS	 		0x00000014
#define PSC_SMB_EVNT	 	0x00000018
#define PSC_SMB_TXRX	 	0x0000001C
#define PSC_SMB_TMR			0x00000030

#ifndef ASSEMBLER
typedef volatile struct
{
	PSC_SELECT	psc;		//00,04
	uint32		cfg;		//08
	uint32		msk;		//0C
	uint32		pcr;		//10
	uint32		sts;		//14
	uint32		evnt;		//18
	uint32		txrx;		//1C
	uint32		tmr;		//20
} PSC_SMB;
#endif

#define PSC_SMB_CFG_SLV			(0x7F<<4)	// Slave Address
#define PSC_SMB_CFG_SLV_N(n)	((n&0x7F)<<1)
#define PSC_SMB_CFG_SFM			(1<<8)		// Standard or Fast Mode
#define PSC_SMB_CFG_CGE			(1<<9)		// General Call Enable
#define	PSC_SMB_CFG_SDIV		(0x3<<13)	// Clock Divider
#define	PSC_SMB_CFG_SDIV_N(n)	((n&0x3)<<13)
#define PSC_SMB_CFG_DE			(1<<26)		// Device Enable
#define PSC_SMB_CFG_DD			(1<<27)		// DMA Disable
#define	PSC_SMB_CFG_TRD			(0x3<<28)	// TX Request Depth
#define	PSC_SMB_CFG_TRD_N(n)	((n&0x3)<<28)
#define	PSC_SMB_CFG_RRD			(0x3<<30)	// Rx Request Depth
#define	PSC_SMB_CFG_RRD_N(n)	((n&0x3)<<30)


#define	PSC_SMB_MSK_MD			(1<<4)		// Master Done
#define	PSC_SMB_MSK_SD			(1<<5)		// Slave done
#define	PSC_SMB_MSK_TU			(1<<8)		// TX Fifo underflow
#define	PSC_SMB_MSK_TO			(1<<9)		// TX fifo overflow
#define	PSC_SMB_MSK_RU			(1<<11)		// RX Fifo underflow
#define	PSC_SMB_MSK_RO			(1<<12)		// RX fifo overflow
#define	PSC_SMB_MSK_MM			(1<<16)		// Multiple Master Error
#define	PSC_SMB_MSK_AL			(1<<28)		// Arbitration Lost (MasterOnly)
#define	PSC_SMB_MSK_AN			(1<<29)		// Address not ack'd
#define	PSC_SMB_MSK_DN			(1<<30)		// Data nont ack'd

#define PSC_SMB_PCR_MS			(1<<0)		// Master Start
#define PSC_SMB_PCR_DC			(1<<2)		// Tx Data Clear

// Status Register is Read Only //
#define PSC_SMB_STS_SR			(1<<0)		// PSC Ready
#define PSC_SMB_STS_DR			(1<<1) 		// Device Ready
#define PSC_SMB_STS_DI			(1<<2)		// Device INterrupt
#define PSC_SMB_STS_MB			(1<<4)		// Master BUsy
#define PSC_SMB_STS_SB			(1<<5)		// Slave Busy
#define PSC_SMB_STS_TR			(1<<8)	 	// Tx Rrequest
#define PSC_SMB_STS_TE			(1<<9)		// Tx Fifo Empty
#define PSC_SMB_STS_TF			(1<<10)		// Tx Fifo Full
#define PSC_SMB_STS_RR			(1<<11)		// Rx Rrequest
#define PSC_SMB_STS_RE			(1<<12)		// Rx Fifo Empty
#define PSC_SMB_STS_RF			(1<<13)		// Rx Fifo Full
#define PSC_SMB_STS_BB			(1<<28)		// Bus Busy

#define	PSC_SMB_EVNT_MD			(1<<4)		// Master Done
#define	PSC_SMB_EVNT_SD			(1<<5)		// Slave done
#define	PSC_SMB_EVNT_TU			(1<<8)		// TX Fifo underflow
#define	PSC_SMB_EVNT_TO			(1<<9)		// TX fifo overflow
#define	PSC_SMB_EVNT_RU			(1<<11)		// RX Fifo underflow
#define	PSC_SMB_EVNT_RO			(1<<12)		// RX fifo overflow
#define	PSC_SMB_EVNT_MM			(1<<16)		// Multiple Master Error
#define	PSC_SMB_EVNT_AL			(1<<28)		// Arbitration Lost (MasterOnly)
#define	PSC_SMB_EVNT_AN			(1<<29)		// Address not ack'd
#define	PSC_SMB_EVNT_DN			(1<<30)		// Data nont ack'd

#define PSC_SMB_TXRX_AD			(0xFF<<0)	// Addr/Data
#define PSC_SMB_TXRX_AD_N(n)	((n&0xFF)<<0)
#define PSC_SMB_TXRX_STP		(1<<29)		// Stop
#define PSC_SMB_TXRX_RSR		(1<<30)		// Restart

#define PSC_SMB_TMR_CH			(0x1F<<0)	// Clock High
#define PSC_SMB_TMR_CH_N(n)		((n&0x1F)<<0)
#define PSC_SMB_TMR_CL			(0x1F<<5)	// Clock Low
#define PSC_SMB_TMR_CL_N(n)		((n&0x1F)<<5)
#define PSC_SMB_TMR_SU			(0x1F<<10)	// Start Setup
#define PSC_SMB_TMR_SU_N(n)		((n&0x1F)<<10)
#define PSC_SMB_TMR_SH			(0x1F<<15)	// Start Hold
#define PSC_SMB_TMR_SH_N(n)		((n&0x1F)<<15)
#define PSC_SMB_TMR_PU			(0x1F<<20)	// Stop Setup
#define PSC_SMB_TMR_PU_N(n)		((n&0x1F)<<20)
#define PSC_SMB_TMR_PS			(0x1F<<25)	// Stop Start Buffer
#define PSC_SMB_TMR_PS_N(n)		((n&0x1F)<<25)
#define PSC_SMB_TMR_TH			(0x3<<30)	// Transmit Hold
#define PSC_SMB_TMR_TH_N(n)		((n&0x3)<<30)

/***********************************************************************/
#if defined(SOC_AU1200) || defined(SOC_AU13XX)

/*
 * LCD controller
 */
#ifndef ASSEMBLER
#pragma pack (1)
typedef volatile struct
{
	uint32	reserved0;
	uint32	screen;
	uint32	backcolor;
	uint32	horztiming;
	uint32	verttiming;
	uint32	clkcontrol;
	uint32	pwmdiv;
	uint32	pwmhi;
	uint32	reserved1;
	uint32	winenable;
	uint32	colorkey;
	uint32	colorkeymsk;
	struct
	{
		uint32	cursorctrl;
		uint32	cursorpos;
		uint32	cursorcolor0;
		uint32	cursorcolor1;
		uint32	cursorcolor2;
		uint32	cursorcolor3;
	} hwc;
	uint32	intstatus;
	uint32	intenable;
	uint32	outmask;
	uint32	fifoctrl;
	uint32	reserved2[(0x0100-0x0058)/4];
	struct
	{
		uint32	winctrl0;
		uint32	winctrl1;
		uint32	winctrl2;
		uint32	winbuf0;
		uint32	winbuf1;
		uint32	winbufctrl;
		uint32	winreserved0;
		uint32	winreserved1;
	} window[4];

	uint32	reserved3[(0x0400-0x0180)/4];

	uint32	palette[(0x0800-0x0400)/4];

	uint32	cursorpattern[(0x0900-0x0800)/4];

} AU1200_LCD;
#pragma pack ()
#endif

/* lcd_screen */
#define LCD_SCREEN_SEN		(1<<31)
#define LCD_SCREEN_SX		(0x07FF<<19)
#define LCD_SCREEN_SY		(0x07FF<< 8)
#define LCD_SCREEN_SWP		(1<<7)
#define LCD_SCREEN_SWD		(1<<6)
#define LCD_SCREEN_ST		(7<<0)
#define LCD_SCREEN_ST_TFT	(0<<0)
#define LCD_SCREEN_SX_N(WIDTH)	((WIDTH-1)<<19)
#define LCD_SCREEN_SY_N(HEIGHT)	((HEIGHT-1)<<8)
#define LCD_SCREEN_ST_CSTN	(1<<0)
#define LCD_SCREEN_ST_CDSTN	(2<<0)
#define LCD_SCREEN_ST_M8STN	(3<<0)
#define LCD_SCREEN_ST_M4STN	(4<<0)

/* lcd_backcolor */
#define LCD_BACKCOLOR_SBGR		(0xFF<<16)
#define LCD_BACKCOLOR_SBGG		(0xFF<<8)
#define LCD_BACKCOLOR_SBGB		(0xFF<<0)
#define LCD_BACKCOLOR_SBGR_N(N)	((N)<<16)
#define LCD_BACKCOLOR_SBGG_N(N)	((N)<<8)
#define LCD_BACKCOLOR_SBGB_N(N)	((N)<<0)

/* lcd_winenable */
#define LCD_WINENABLE_WEN3		(1<<3)
#define LCD_WINENABLE_WEN2		(1<<2)
#define LCD_WINENABLE_WEN1		(1<<1)
#define LCD_WINENABLE_WEN0		(1<<0)
#define LCD_WINENABLE_N(N)		(1<<N)

/* lcd_colorkey */
#define LCD_COLORKEY_CKR		(0xFF<<16)
#define LCD_COLORKEY_CKG		(0xFF<<8)
#define LCD_COLORKEY_CKB		(0xFF<<0)
#define LCD_COLORKEY_CKR_N(N)	((N)<<16)
#define LCD_COLORKEY_CKG_N(N)	((N)<<8)
#define LCD_COLORKEY_CKB_N(N)	((N)<<0)

/* lcd_colorkeymsk */
#define LCD_COLORKEYMSK_CKMR		(0xFF<<16)
#define LCD_COLORKEYMSK_CKMG		(0xFF<<8)
#define LCD_COLORKEYMSK_CKMB		(0xFF<<0)
#define LCD_COLORKEYMSK_CKMR_N(N)	((N)<<16)
#define LCD_COLORKEYMSK_CKMG_N(N)	((N)<<8)
#define LCD_COLORKEYMSK_CKMB_N(N)	((N)<<0)

/* lcd windows control 0 */
#define LCD_WINCTRL0_OX		(0x07FF<<21)
#define LCD_WINCTRL0_OY		(0x07FF<<10)
#define LCD_WINCTRL0_A		(0x00FF<<2)
#define LCD_WINCTRL0_AEN	(1<<1)
#define LCD_WINCTRL0_OX_N(N) ((N)<<21)
#define LCD_WINCTRL0_OY_N(N) ((N)<<10)
#define LCD_WINCTRL0_A_N(N) ((N)<<2)

#define GET_LCD_WINCTRL0_OX_N(N) ((N & LCD_WINCTRL0_OX) >> 21)
#define GET_LCD_WINCTRL0_OY_N(N) ((N & LCD_WINCTRL0_OY) >> 10)

/* lcd windows control 1 */
#define LCD_WINCTRL1_PRI	(3<<30)
#define LCD_WINCTRL1_PIPE	(1<<29)
#define LCD_WINCTRL1_FRM	(0xF<<25)
#define LCD_WINCTRL1_CCO	(1<<24)
#define LCD_WINCTRL1_PO		(3<<22)
#define LCD_WINCTRL1_SZX	(0x07FF<<11)
#define LCD_WINCTRL1_SZY	(0x07FF<<0)
#define LCD_WINCTRL1_FRM_1BPP	(0<<25)
#define LCD_WINCTRL1_FRM_2BPP	(1<<25)
#define LCD_WINCTRL1_FRM_4BPP	(2<<25)
#define LCD_WINCTRL1_FRM_8BPP	(3<<25)
#define LCD_WINCTRL1_FRM_12BPP	(4<<25)
#define LCD_WINCTRL1_FRM_16BPP655	(5<<25)
#define LCD_WINCTRL1_FRM_16BPP565	(6<<25)
#define LCD_WINCTRL1_FRM_16BPP556	(7<<25)
#define LCD_WINCTRL1_FRM_16BPPI1555	(8<<25)
#define LCD_WINCTRL1_FRM_16BPPI5551	(9<<25)
#define LCD_WINCTRL1_FRM_16BPPA1555	(10<<25)
#define LCD_WINCTRL1_FRM_16BPPA5551	(11<<25)
#define LCD_WINCTRL1_FRM_24BPP		(12<<25)
#define LCD_WINCTRL1_FRM_32BPP		(13<<25)
#define LCD_WINCTRL1_FRM_N(N)	(N<<25)
#define LCD_WINCTRL1_SZX_N(N)	((N-1)<<11)
#define LCD_WINCTRL1_SZY_N(N)	((N-1)<<0)
#define LCD_WINCTRL1_PRI_N(N) ((N)<<30)
#define LCD_WINCTRL1_PIPE_N(N)((N)<<29)
#define LCD_WINCTRL1_PRI_U(N)	(((N)>>30)&0x3)
#define LCD_WINCTRL1_PO_00    (0<<22)
#define LCD_WINCTRL1_PO_01    (1<<22)
#define LCD_WINCTRL1_PO_10    (2<<22)
#define LCD_WINCTRL1_PO_11    (3<<22)
#define LCD_WINCTRL1_PO_N(N)	(N<<22)

#define GET_LCD_WINCTRL1_SZX_N(N)	((N & LCD_WINCTRL1_SZX) >> 11)
#define GET_LCD_WINCTRL1_SZY_N(N)	(N & LCD_WINCTRL1_SZY)

/* lcd windows control 2 */
#define LCD_WINCTRL2_CKMODE		(3<<24)
#define LCD_WINCTRL2_DBM		(1<<23)
#define LCD_WINCTRL2_RAM		(3<<21)
#define LCD_WINCTRL2_BX			(0x1FFF<<8)
#define LCD_WINCTRL2_SCX		(0xF<<4)
#define LCD_WINCTRL2_SCY		(0xF<<0)
#define LCD_WINCTRL2_RAM_NONE		(0<<21)
#define LCD_WINCTRL2_RAM_PALETTE	(1<<21)
#define LCD_WINCTRL2_RAM_GAMMA		(2<<21)
#define LCD_WINCTRL2_RAM_BUFFER		(3<<21)
#define LCD_WINCTRL2_RAM_N(N)		(N<<21)
#define LCD_WINCTRL2_CKMODE_00                (0<<24)
#define LCD_WINCTRL2_CKMODE_01                (1<<24)
#define LCD_WINCTRL2_CKMODE_10                (2<<24)
#define LCD_WINCTRL2_CKMODE_11                (3<<24)
#define LCD_WINCTRL2_CKMODE_N(N)	(N<<24)
#define LCD_WINCTRL2_BX_N(N)  ((N)<<8)
#define LCD_WINCTRL2_SCX_1    (0<<4)
#define LCD_WINCTRL2_SCX_2    (1<<4)
#define LCD_WINCTRL2_SCX_4    (2<<4)
#define LCD_WINCTRL2_SCX_N(N) (N<<4)
#define LCD_WINCTRL2_SCY_1    (0<<0)
#define LCD_WINCTRL2_SCY_2    (1<<0)
#define LCD_WINCTRL2_SCY_4    (2<<0)
#define LCD_WINCTRL2_SCY_N(N) (N<<0)

#define GET_LCD_WINCTRL2_SCX_N(N) ((N & LCD_WINCTRL2_SCX) >> 4)
#define GET_LCD_WINCTRL2_SCY_N(N) (N & LCD_WINCTRL2_SCY)
#define GET_LCD_WINCTRL2_BX_N(N)  ((N & LCD_WINCTRL2_BX) >> 8)

/* lcd windows buffer control */
#define LCD_WINBUFCTRL_DB		(1<<1)
#define LCD_WINBUFCTRL_DBN		(1<<0)

/* lcd_intstatus, lcd_intenable */
#define LCD_INT_IFO				(0xF<<14)
#define LCD_INT_IFU				(0xF<<10)
#define LCD_INT_OFO				(1<<9)
#define LCD_INT_OFU				(1<<8)
#define LCD_INT_WAIT			(1<<3)
#define LCD_INT_SD				(1<<2)
#define LCD_INT_SA				(1<<1)
#define LCD_INT_SS				(1<<0)

/* lcd_horztiming */
#define LCD_HORZTIMING_HND2		(0x1FF<<18)
#define LCD_HORZTIMING_HND1		(0x1FF<<9)
#define LCD_HORZTIMING_HPW		(0x1FF<<0)
#define LCD_HORZTIMING_HND2_N(N)(((N)-1)<<18)
#define LCD_HORZTIMING_HND1_N(N)(((N)-1)<<9)
#define LCD_HORZTIMING_HPW_N(N)	(((N)-1)<<0)

/* lcd_verttiming */
#define LCD_VERTTIMING_VND2		(0x1FF<<18)
#define LCD_VERTTIMING_VND1		(0x1FF<<9)
#define LCD_VERTTIMING_VPW		(0x1FF<<0)
#define LCD_VERTTIMING_VND2_N(N)(((N)-1)<<18)
#define LCD_VERTTIMING_VND1_N(N)(((N)-1)<<9)
#define LCD_VERTTIMING_VPW_N(N)	(((N)-1)<<0)

/* lcd_clkcontrol */
#define LCD_CLKCONTROL_EXT		(1<<22)
#define LCD_CLKCONTROL_DELAY	(3<<20)
#define LCD_CLKCONTROL_CDD		(1<<19)
#define LCD_CLKCONTROL_IB		(1<<18)
#define LCD_CLKCONTROL_IC		(1<<17)
#define LCD_CLKCONTROL_IH		(1<<16)
#define LCD_CLKCONTROL_IV		(1<<15)
#define LCD_CLKCONTROL_BF		(0x1F<<10)
#define LCD_CLKCONTROL_PCD		(0x3FF<<0)
#define LCD_CLKCONTROL_BF_N(N)	(((N)-1)<<10)
#define LCD_CLKCONTROL_PCD_N(N)	((N)<<0)

/* lcd_pwmdiv */
#define LCD_PWMDIV_EN			(1<<31)
#define LCD_PWMDIV_PWMDIV		(0x1FFFF<<0)
#define LCD_PWMDIV_PWMDIV_N(N)	((N)<<0)

/* lcd_pwmhi */
#define LCD_PWMHI_PWMHI1		(0xFFFF<<16)
#define LCD_PWMHI_PWMHI0		(0xFFFF<<0)
#define LCD_PWMHI_PWMHI1_N(N)	((N)<<16)
#define LCD_PWMHI_PWMHI0_N(N)	((N)<<0)

/* lcd_hwccon */
#define LCD_HWCCON_EN			(1<<0)

/* lcd_cursorpos */
#define LCD_CURSORPOS_HWCXOFF		(0x1F<<27)
#define LCD_CURSORPOS_HWCXPOS		(0x07FF<<16)
#define LCD_CURSORPOS_HWCYOFF		(0x1F<<11)
#define LCD_CURSORPOS_HWCYPOS		(0x07FF<<0)
#define LCD_CURSORPOS_HWCXOFF_N(N)	((N)<<27)
#define LCD_CURSORPOS_HWCXPOS_N(N)	((N)<<16)
#define LCD_CURSORPOS_HWCYOFF_N(N)	((N)<<11)
#define LCD_CURSORPOS_HWCYPOS_N(N)	((N)<<0)

/* lcd_cursorcolor */
#define LCD_CURSORCOLOR_HWCA		(0xFF<<24)
#define LCD_CURSORCOLOR_HWCR		(0xFF<<16)
#define LCD_CURSORCOLOR_HWCG		(0xFF<<8)
#define LCD_CURSORCOLOR_HWCB		(0xFF<<0)
#define LCD_CURSORCOLOR_HWCA_N(N)	((N)<<24)
#define LCD_CURSORCOLOR_HWCR_N(N)	((N)<<16)
#define LCD_CURSORCOLOR_HWCG_N(N)	((N)<<8)
#define LCD_CURSORCOLOR_HWCB_N(N)	((N)<<0)

/* lcd_fifoctrl */
#define LCD_FIFOCTRL_F3IF		(1<<29)
#define LCD_FIFOCTRL_F3REQ		(0x1F<<24)
#define LCD_FIFOCTRL_F2IF		(1<<29)
#define LCD_FIFOCTRL_F2REQ		(0x1F<<16)
#define LCD_FIFOCTRL_F1IF		(1<<29)
#define LCD_FIFOCTRL_F1REQ		(0x1F<<8)
#define LCD_FIFOCTRL_F0IF		(1<<29)
#define LCD_FIFOCTRL_F0REQ		(0x1F<<0)
#define LCD_FIFOCTRL_F3REQ_N(N)	((N-1)<<24)
#define LCD_FIFOCTRL_F2REQ_N(N)	((N-1)<<16)
#define LCD_FIFOCTRL_F1REQ_N(N)	((N-1)<<8)
#define LCD_FIFOCTRL_F0REQ_N(N)	((N-1)<<0)

/* lcd_outmask */
#define LCD_OUTMASK_MASK		(0x00FFFFFF)

#endif //#if defined(SOC_AU1200) || defined(SOC_AU13XX)

/***********************************************************************/
#if defined(SOC_AU1200)
/*
 * Au1200 Software Counter Block Register Definitions
 */
#define SWCNT_CONTROL		0x0000
#define SWCNT_COUNT		0x0004
#define SWCNT_MATCH		0x0008
#define	SWCNT_INTSTAT		0x000C

#ifndef ASSEMBLER
typedef volatile struct
{
	uint32		control;			//0x10C
	uint32		count;				//0x110
	uint32		match;				//0x114
	uint32		intstat;			//0x118
} AU1200_SWCNT;
#endif

/* swcnt_control */
#define SWCNT_CONTROL_EN		(1<<0)
#define SWCNT_CONTROL_IE		(1<<1)

/* swcnt_count */
#define SWCNT_COUNT_COUNT		(0x03FFFFFF<<0)
#define SWCNT_COUNT_COUNT_N(n)		((n&0x03FFFFFF)<<0)

/* swcnt_match */
#define SWCNT_MATCH_MATCH		(0x03FFFFFF<<0)
#define SWCNT_MATCH_MATCH_N(n)		((n&0x03FFFFFF)<<0)

/* swcnt_intstat */
#define SWCNT_INTSTAT_INT		(1<<0)

#endif //#if defined(SOC_AU1200)

/***********************************************************************/

/*
 * AES Block Register Definitions
 */
#define AES_STATUS                         0x00000000
#define AES_INDATA                         0x00000004
#define AES_OUTDATA                        0x00000008
#define AES_INTCAUSE                       0x0000000C
#define AES_CONFIG                         0x00000010

#ifndef ASSEMBLER
typedef volatile struct
{
        unsigned long         status;                /* 08 */
        unsigned long         indata;                /* 0C */
        unsigned long         outdata;               /* 10 */
        unsigned long         intcause;              /* 14 */
        unsigned long         config;                /* 18 */
} AU1200_AES;
#endif


#define AES_STATUS_PS                (1<<0)                /* AES Process start/stop */
#define AES_STATUS_IE                (1<<1)                /* Interrupt Enable */
#define AES_STATUS_CR_N(n)           ((n&0x03)<<2)         /* clock ratio/divider */
#define AES_STATUS_OUT               (1<<4)                /* TX FIFO ready (at least 16 bytes) */
#define AES_STATUS_IN                (1<<5)                /* RX FIFO ready (at least 16 bytes) */

#define AES_INT_RDY                  (1<<0)                /* En/Decryption Completion */
#define AES_INT_OVR                  (1<<1)                /* Input data FIFO underflow */
#define AES_INT_UND                  (1<<2)                /* Output data FIFO overflow */

#define AES_CONFIG_ED                (1<<0)                /* En/Decryption Select */
#define AES_CONFIG_IKG               (1<<1)                /* Internal Key Generation */
#define AES_CONFIG_RPK               (1<<2)                /* Software Replay Key */
#define AES_CONFIG_RK                (1<<3)                /* Internal reuse key */
#define AES_CONFIG_UC                (1<<4)                /* Undefined block count */
#define AES_CONFIG_OP_N(n)           ((n&0x03)<<5)         /* Mode of operation */

/***********************************************************************/

#if defined(SOC_AU1200)	|| defined(SOC_AU13XX)

/*
 * AU1200 CIM Block Register Definitions
 */

#define CIM_ENABLE				0x00000000
#define CIM_CONFIG			   	0x00000004
#define CIM_CAPTURE				0x00000010
#define CIM_STAT				0x00000014
#define CIM_INTEN				0x00000018
#define CIM_INSTAT				0x0000001C
#define CIM_FIFOA				0x00000020
#define CIM_FIFOB				0x00000040
#define CIM_FIFOC				0x00000060

#ifndef ASSEMBLER
typedef volatile struct
{
    unsigned long         enable;                /* 00 */
    unsigned long         config;                /* 04 */

#if defined(SOC_AU1200)
    unsigned long         reserve0;              /* 08 */
    unsigned long         reserve1;              /* 0C */
    unsigned long         capture;               /* 10 */
    unsigned long         stat;                  /* 14 */
    unsigned long         inten;                 /* 18 */
    unsigned long         instat;                /* 1C */
    unsigned long         fifoa;                 /* 20 */
    unsigned long         reserve2;              /* 24 */
    unsigned long         reserve3;              /* 28 */
    unsigned long         reserve4;              /* 2C */
    unsigned long         reserve5;              /* 30 */
    unsigned long         reserve6;              /* 34 */
    unsigned long         reserve7;              /* 38 */
    unsigned long         reserve8;              /* 3C */
    unsigned long         fifob;                 /* 40 */
    unsigned long         reserve9;              /* 44 */
    unsigned long         reserve10;             /* 48 */
    unsigned long         reserve11;             /* 4C */
    unsigned long         reserve12;             /* 50 */
    unsigned long         reserve13;             /* 54 */
    unsigned long         reserve14;             /* 58 */
    unsigned long         reserve15;             /* 5C */
    unsigned long         fifoc;                 /* 60 */
#elif defined(SOC_AU13XX)
    unsigned long         config2;               /* 08 */
    unsigned long         reserve1;              /* 0C */
    unsigned long         capture;               /* 10 */
    unsigned long         stat;                  /* 14 */
    unsigned long         inten;                 /* 18 */
    unsigned long         instat;                /* 1C */
	unsigned long		  reserve2[(0x80-0x20)/4]; /* 20 - 80 */

	unsigned long 		  cim_rxa_buf0;			 /* 80 */
	unsigned long 		  cim_rxa_buf1; 		 /* 84 */
	unsigned long	      cim_rxa_addr;          /* 88 */
	unsigned long	      cim_rxa_max;           /* 8C */
	unsigned long	      cim_rxa_cnt;           /* 90 */

	unsigned long		  reserve3[3];		     /* 94-9C */

	unsigned long 		  cim_rxb_buf0;			 /* A0 */
	unsigned long 		  cim_rxb_buf1; 		 /* A4 */
	unsigned long	      cim_rxb_addr;          /* A8 */
	unsigned long	      cim_rxb_max;           /* AC */
	unsigned long	      cim_rxb_cnt;           /* B0 */

	unsigned long		  reserve4[3];		     /* B4-BC */

	unsigned long 		  cim_rxc_buf0;			 /* C0 */
	unsigned long 		  cim_rxc_buf1;			 /* C4 */
	unsigned long	      cim_rxc_addr;          /* C8 */
	unsigned long	      cim_rxc_max;           /* CC */
	unsigned long	      cim_rxc_cnt;           /* D0 */
#endif	 // #if defined(SOC_Au1200) and elif defined(SOC_AU13XX)
}  AU1200_CIM, AU13XX_CIM;
#endif

#define   CIM_ENABLE_EN			(1<<0) /* enable/disable/reset the block*/

/* CIM Configuration Register */

#define   CIM_CONFIG_PM 		(1<<0)  // Pulse mode:  0=level, 1=pulse
#define   CIM_CONFIG_CLK    	(1<<1)  // Clock input: 0=falling, 1=rising
#define   CIM_CONFIG_LS			(1<<2)  // Line sync:   1=active low
#define   CIM_CONFIG_FS			(1<<3)  // Frame sync:  1=active low

// Data pattern select:
#define   CIM_CONFIG_RAW        0
#define   CIM_CONFIG_BAYER      1
#define   CIM_CONFIG_656        2

#define   CIM_CONFIG_DPS    	(3<<6)
#define   CIM_CONFIG_DPS_N(n)   ((n & 0x03)<<6)

// Bayer mode select:
#define   CIM_CONFIG_RGGB       0
#define   CIM_CONFIG_GRBG       1
#define   CIM_CONFIG_BGGR       2
#define   CIM_CONFIG_GBRG       3

#define   CIM_CONFIG_BAY		(3<<8)
#define   CIM_CONFIG_BAY_N(n)   ((n & 0x03)<<8)

// Data length:
#define   CIM_CONFIG_LEN_8BIT   0
#define   CIM_CONFIG_LEN_9BIT   1
#define   CIM_CONFIG_LEN_10BIT  2

#define   CIM_CONFIG_LEN		(0xF<<10)
#define   CIM_CONFIG_LEN_N(n)   (((n) & 0x0F)<<10)

#define   CIM_CONFIG_BYT		(1<<14)     // Byte capture mode
#define   CIM_CONFIG_SF			(1<<15)     // Single frame capture

// Field select (656 only):
#define   CIM_CONFIG_FIELD1     0  // Capture from Field 1
#define   CIM_CONFIG_FIELD2     1  // Capture from Field 2
#define   CIM_CONFIG_FIELD12    2  // Capture from Either Field

#define   CIM_CONFIG_FSEL		(3<<16)
#define   CIM_CONFIG_FS_N(n)	((n & 0x03)<<16)

#define   CIM_CONFIG_SI			(1<<18)

/* CIM Capture Control Register */

#define CIM_CAPTURE_VCE			(1<<0)
#define CIM_CAPTURE_SCE			(1<<1)
#define CIM_CAPTURE_CLR			(1<<2)

/* CIM Status Register */

#define CIM_STATUS_VC			(1<<0)
#define CIM_STATUS_SC			(1<<1)
#define CIM_STATUS_AF			(1<<2)
#define CIM_STATUS_AE			(1<<3)
#define CIM_STATUS_AR			(1<<4)
#define CIM_STATUS_BF			(1<<5)
#define CIM_STATUS_BE			(1<<6)
#define CIM_STATUS_BR			(1<<7)
#define CIM_STATUS_CF			(1<<8)
#define CIM_STATUS_CE			(1<<9)
#define CIM_STATUS_CR			(1<<10)

/* Interrupt Enable Register */

#define CIM_INTEN_CD            (1<<0)
#define CIM_INTEN_FD            (1<<1)
#define CIM_INTEN_UFA           (1<<2)
#define CIM_INTEN_OFA           (1<<3)
#define CIM_INTEN_UFB           (1<<4)
#define CIM_INTEN_OFB           (1<<5)
#define CIM_INTEN_UFC           (1<<6)
#define CIM_INTEN_OFC           (1<<7)
#define CIM_INTEN_ERR           (1<<8)

/* Interrupt Status Rgister */

#define CIM_INSTAT_CD			(1<<0)
#define CIM_INSTAT_FD			(1<<1)
#define CIM_INSTAT_UFA			(1<<2)
#define CIM_INSTAT_OFA			(1<<3)
#define CIM_INSTAT_UFB			(1<<4)
#define CIM_INSTAT_OFB			(1<<5)
#define CIM_INSTAT_UFC			(1<<6)
#define CIM_INSTAT_OFC			(1<<7)
#define CIM_INSTAT_ERR			(1<<8)


#if defined(SOC_AU13XX)
#define CIM_CONFIG2_PPB			(1<<8)
#define CIM_CONFIG2_DW			(1<<7)
#define CIM_CONFIG2_BSM(n)		((n&3)<<5)
#define CIM_CONFIG2_SW			(1<<4)
#define CIM_CONFIG2_SHW			(1<<3)
#define CIM_CONFIG2_SB			(1<<2)
#define CIM_CONFIG2_BS(n)		((n&3)<<0)


#endif // #if defined(SOC_AU13XX)

#endif // #if defined(SOC_AU1200)  || defined(SOC_AU13XX)


/***********************************************************************/

#if defined(SOC_AU1200)

/*
 * AU1200 MAE Front End Register Definitions
 */
#define MAEFE_CONFIG			0x0000
#define MAEFE_CURY			0x0004
#define MAEFE_FREFY			0x0008
#define MAEFE_BREFY			0x000C
#define MAEFE_CURCB			0x0010
#define MAEFE_FREFCB			0x0014
#define MAEFE_BREFCB			0x0018
#define MAEFE_CURCR			0x001C
#define MAEFE_FREFCR			0x0020
#define MAEFE_BREFCR			0x0024
#define MAEFE_PICTSIZE			0x0028
#define MAEFE_INTENSCOMP		0x002C

#define MAEFE_FREFBOTY			0x0038
#define MAEFE_FREFBOTCB			0x003C
#define MAEFE_FREFBOTCR			0x0040
#define MAEFE_BREFBOTY			0x0044
#define MAEFE_BREFBOTCB			0x0048
#define MAEFE_BREFBOTCR			0x004C
#define MAEFE_INTSTAT			0x0050
#define MAEFE_INTENABLE			0x0054
#define MAEFE_SCRATCHPAD		0x0058
#define MAEFE_WMV9PQUANT		0x005C

#define MAEFE_DMADSCR			0x1004
#define MAEFE_DMADBELL			0x1008

#ifndef ASSEMBLER
typedef volatile struct {
  uint32	config;
  uint32	cury;
  uint32	frefy;
  uint32	brefy;
  uint32	curcb;
  uint32	frefcb;
  uint32	brefcb;
  uint32	curcr;
  uint32	frefcr;
  uint32	brefcr;
  uint32	pictsize;
  uint32	intenscomp;
  uint8 	 reserved0[(MAEFE_FREFBOTY - MAEFE_INTENSCOMP) - 4];
  uint32	frefboty;
  uint32	frefbotcb;
  uint32	frefbotcr;
  uint32	brefboty;
  uint32	brefbotcb;
  uint32	brefbotcr;
  uint32	intstat;
  uint32	intenable;
  uint32	scratchpad;
  uint32	wmv9pquant;
  uint8 	 reserved1[(MAEFE_DMADSCR - MAEFE_WMV9PQUANT) - 4];
  uint32	dmadscr;
  uint32	dmadbell;
} AU1200_MAEFE;
#endif

#endif // #if defined(SOC_AU1200)

/***********************************************************************/

#if defined(SOC_AU1200) || defined(TMP_MEDIA_V20)
/*
 * AU1200 MAE Back End Register Definitions
 */

/* SCF Registers */
#define MAEBE_SCFHSR			0x0000
#define MAEBE_SCFVSR			0x0004
#define MAEBE_SCFDISABLE		0x0008
#define MAEBE_SCFHALUT			0x0100
#define MAEBE_SCFVALUT			0x0180
#define MAEBE_SCFHBLUT			0x0200
#define MAEBE_SFCVBLUT			0x0280
#define MAEBE_SCFHCLUT			0x0300
#define MAEBE_SCFVCLUT			0x0380

/* CSC Registers */
#define MAEBE_CSCXCFFA			0x0400
#define MAEBE_CSCXCFFB			0x0404
#define MAEBE_CSCXCFFC			0x0408
#define MAEBE_CSCYCFFA			0x040C
#define MAEBE_CSCYCFFB			0x0410
#define MAEBE_CSCYCFFC			0x0414
#define MAEBE_CSCZCFFA			0x0418
#define MAEBE_CSCZCFFB			0x041C
#define MAEBE_CSCZCFFC			0x0420
#define MAEBE_CSCXOFF			0x0424
#define MAEBE_CSCYOFF			0x0428
#define MAEBE_CSCZOFF			0x042C
#define MAEBE_CSCALPHA			0x0430

/* SRC Registers */
#define MAEBE_SRCCFG			0x0500
#define MAEBE_SRCFHW			0x0504
#define MAEBE_SRCAADDR			0x0508
#define MAEBE_SRCASTR			0x050C
#define MAEBE_SRCBADDR			0x0510
#define MAEBE_SRCBSTR			0x0514
#define MAEBE_SRCCADDR			0x0518
#define MAEBE_SRCCSTR			0x051C

/* DST Registers */
#define MAEBE_DSTCFG			0x0600
#define MAEBE_DSTHEIGHT			0x0604
#define MAEBE_DSTADDR			0x0608
#define MAEBE_DSTSTR			0x060C

/* CTL Registers */
#define MAEBE_CTLENABLE			0x0700
#define MAEBE_CTLFPC			0x0704
#define MAEBE_CTLSTAT			0x0708
#define MAEBE_CTLINTENABLE		0x070C
#define MAEBE_CTLINTSTAT		0x0710

#ifndef ASSEMBLER
typedef volatile struct
{
  uint32		scfhsr;		// 0x0000
  uint32		scfvsr;		// 0x0004
  uint32		scfdisable;	// 0x0008
  uint8 		 reserved0[(MAEBE_SCFHALUT - MAEBE_SCFDISABLE) - 4];	// 0x000C 0xF4
  uint32		scfhalut[32];	// 0x0100
  uint32		scfvalut[32];	// 0x0180
  uint32		scfhblut[32];	// 0x0200
  uint32		scfvblut[32];	// 0x0280
  uint32		scfhclut[32];	// 0x0300
  uint32		scfvclut[32];	// 0x0380

  uint32		cscxcffa;	// 0x0400
  uint32		cscxcffb;	// 0x0404
  uint32		cscxcffc;	// 0x0408
  uint32		cscycffa;	// 0x040C
  uint32		cscycffb;	// 0x0410
  uint32		cscycffc;	// 0x0414
  uint32		csczcffa;	// 0x0418
  uint32		csczcffb;	// 0x041C
  uint32		csczcffc;	// 0x0420
  uint32		cscxoff;	// 0x0424
  uint32		cscyoff;	// 0x0428
  uint32		csczoff;	// 0x042C
  uint32		cscalpha;	// 0x0430

  uint8 		 reserved1[(MAEBE_SRCCFG - MAEBE_CSCALPHA) - 4];	// 0x0434 +0xCC
  uint32		srccfg;		// 0x0500
  uint32		srcfhw;		// 0x0504
  uint32		srcaaddr;	// 0x0508
  uint32		srcastr;	// 0x050C
  uint32		srcbaddr;	// 0x0510
  uint32		srcbstr;	// 0x0514
  uint32		srccaddr;	// 0x0518
  uint32		srccstr;	// 0x051C

  uint8 		 reserved2[(MAEBE_DSTCFG - MAEBE_SRCCSTR) - 4]; 	// 0x0520 +0xE0
  uint32		dstcfg;		// 0x0600
  uint32		dstfh;		// 0x0604
  uint32		dstaddr;	// 0x0608
  uint32		dststr;		// 0x060C

  uint8 		 reserved3[(MAEBE_CTLENABLE - MAEBE_DSTSTR) - 4];	// 0x0610 +0xF0
  uint32		ctlenable;	// 0x0700
  uint32		ctlfpc;		// 0x0704
  uint32		ctlstat;	// 0x0708
  uint32		ctlintenable;	// 0x070C
  uint32		ctlintstat;	// 0x0710
} AU1200_MAEBE;
#endif

#ifdef MCP_DRIVER
#define MAEBE_CTLENABLE_EN		(1 << 0)

#define MAEBE_CTLFPC_FRST		(1 << 1)
#define MAEBE_CTLFPC_STR		(1 << 0)

#define MAEBE_CTLSTAT_FP		(1 << 1)
#define MAEBE_CTLSTAT_FB		(1 << 0)

#define MAEBE_CTLINTSTAT_FC		(1 << 0)

#define MAEBE_DSTCFG_EF_SHIFT		(1)
#define MAEBE_DSTCFG_OF_SHIFT		(2)
#define MAEBE_DSTCFG_BGR_SHIFT		(4)
#define MAEBE_DSTCFG_ROT_SHIFT		(5)
#define MAEBE_DSTCFG_R_SHIFT		(7)
#define MAEBE_DSTCFG_EF			(1 << MAEBE_DSTCFG_EF_SHIFT)
#define MAEBE_DSTCFG_OF			(3 << MAEBE_DSTCFG_OF_SHIFT)
#define MAEBE_DSTCFG_BGR		(1 << MAEBE_DSTCFG_BGR_SHIFT)
#define MAEBE_DSTCFG_ROT		(3 << MAEBE_DSTCFG_ROT_SHIFT)
#define MAEBE_DSTCFG_R			(1 << MAEBE_DSTCFG_R_SHIFT)

#define MAEBE_SRCCFG_IF			(2)
#define MAEBE_SRCCFG_IF_MASK	(0x3<<2)
#define MAEBE_SRCCFG_IF_N(n)	((n&3)<<MAEBE_SRCCFG_IF)

#define MAEBE_SRCCFG_ILM	 	(4)
#define MAEBE_SRCCFG_ILM_N(n)	((n&3)<<MAEBE_SRCCFG_ILM)
#define MAEBE_SRCCFG_ILE		(1<<6)
#define MAEBE_SRCCFG_BM			(7)
#define MAEBE_SRCCFG_BM_N(n)	((n&3)<<MAEBE_SRCCFG_BM)
#define MAEBE_SRCCFG_BYE		(1<<9)
#define MAEBE_SRCCFG_EF			(1<<10)

#define MAEBE_SRCFHW_W			(0)
#define MAEBE_SRCFHW_W_N(n)		((n&0x7FF)<<MAEBE_SRCFHW_W)
#define MAEBE_SRCFHW_H			(16)
#define MAEBE_SRCFHW_H_N(n)		((n&0x7FF)<<MAEBE_SRCFHW_H)

#define MAEBE_SCFHSR_SRF		(0)
#define MAEBE_SCFHSR_SRF_N(n)	((n&0xFFFF)<<MAEBE_SCFHSR_SRF)
#define MAEBE_SCFHSR_SRI		(16)
#define MAEBE_SCFHSR_SRI_N(n)	((n&0xFFFF)<<MAEBE_SCFHSR_SRI)

#define MAEBE_SCFVSR_SRF		(0)
#define MAEBE_SCFVSR_SRF_N(n)	((n&0xFFFF)<<MAEBE_SCFVSR_SRF)
#define MAEBE_SCFVSR_SRI		(16)
#define MAEBE_SCFVSR_SRI_N(n)	((n&0xFFFF)<<MAEBE_SCFVSR_SRI)
#endif

#endif // #if defined(SOC_AU1200)


/***********************************************************************/
#ifdef SOC_AU13XX

#ifndef ASSEMBLER
typedef volatile struct
{
	/* 0x0000 */ unsigned long gate;
	/* 0x0004 */ unsigned long clkrst;
	/* 0x0008 */ unsigned long ftr;
} AU13XX_VSS_BLOCK,*PAU13XX_VSS_BLOCK;

typedef volatile struct
{
	AU13XX_VSS_BLOCK	mpe;
	AU13XX_VSS_BLOCK    bsa;
	AU13XX_VSS_BLOCK    gpu;
	AU13XX_VSS_BLOCK    mgp;
} AU13XX_VSSCTRL, *PAU13XX_VSSCTRL;
#endif

// gate defines
#define VSS_GATE_NWAIT(n)	((n&0x3f)<<19)
#define VSS_GATE_MWAIT(n)	((n&0x3f)<<13)
#define VSS_GATE_LWAIT(n)	((n&0x3f)<<7)
#define VSS_GATE_KWAIT(n)	((n&0x3f)<<1)
#define VSS_GATE_EN			(1<<0)

// clkrst defines
#define VSS_CLOCK_EN		(1<<1)
#define VSS_BLOCK_RST		(1<<0)

// ftr defines
#define VSS_ISO_EN			(1<<4)
#define VSS_FTR_EN(n)		(1<<(n))

// Non-register defines used for indexing
#define	VSS_MPE		0
#define VSS_BSA		1
#define VSS_GPU		2
#define VSS_MGP		3
#define VSS_MAX		VSS_MGP

#define OTP_AES	(1 << 0)
#define OTP_SDR (1 << 1)
#define OTP_GPE (1 << 2)
#define OTP_MGP (1 << 3)
#define OTP_HDR (1 << 4)

#endif // #ifdef SOC_AU13XX


/***********************************************************************/


/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/

/*
 * Physical base addresses for integrated peripherals
 */

#ifdef SOC_AU1000
#define	MEM_PHYS_ADDR		0x14000000
#define	STATIC_MEM_PHYS_ADDR	0x14001000
#define	DMA0_PHYS_ADDR		0x14002000
#define	DMA1_PHYS_ADDR		0x14002100
#define	DMA2_PHYS_ADDR		0x14002200
#define	DMA3_PHYS_ADDR		0x14002300
#define	DMA4_PHYS_ADDR		0x14002400
#define	DMA5_PHYS_ADDR		0x14002500
#define	DMA6_PHYS_ADDR		0x14002600
#define	DMA7_PHYS_ADDR		0x14002700
#define	IC0_PHYS_ADDR		0x10400000
#define	IC1_PHYS_ADDR		0x11800000
#define	AC97_PHYS_ADDR		0x10000000
#define	USBH_PHYS_ADDR		0x10100000
#define	USBD_PHYS_ADDR		0x10200000
#define	IRDA_PHYS_ADDR		0x10300000
#define	MAC0_PHYS_ADDR		0x10500000
#define	MAC1_PHYS_ADDR		0x10510000
#define	MACEN_PHYS_ADDR		0x10520000
#define	MACDMA0_PHYS_ADDR	0x14004000
#define	MACDMA1_PHYS_ADDR	0x14004200
#define	I2S_PHYS_ADDR		0x11000000
#define	UART0_PHYS_ADDR		0x11100000
#define	UART1_PHYS_ADDR		0x11200000
#define	UART2_PHYS_ADDR		0x11300000
#define	UART3_PHYS_ADDR		0x11400000
#define	SSI0_PHYS_ADDR		0x11600000
#define	SSI1_PHYS_ADDR		0x11680000
#define	SYS_PHYS_ADDR		0x11900000
#define PCMCIA_IO_PHYS_ADDR   0xF00000000
#define PCMCIA_ATTR_PHYS_ADDR 0xF40000000
#define PCMCIA_MEM_PHYS_ADDR  0xF80000000
#endif

/********************************************************************/

#ifdef SOC_AU1500
#define	MEM_PHYS_ADDR		0x14000000
#define	STATIC_MEM_PHYS_ADDR	0x14001000
#define	DMA0_PHYS_ADDR		0x14002000
#define	DMA1_PHYS_ADDR		0x14002100
#define	DMA2_PHYS_ADDR		0x14002200
#define	DMA3_PHYS_ADDR		0x14002300
#define	DMA4_PHYS_ADDR		0x14002400
#define	DMA5_PHYS_ADDR		0x14002500
#define	DMA6_PHYS_ADDR		0x14002600
#define	DMA7_PHYS_ADDR		0x14002700
#define	IC0_PHYS_ADDR		0x10400000
#define	IC1_PHYS_ADDR		0x11800000
#define	AC97_PHYS_ADDR		0x10000000
#define	USBH_PHYS_ADDR		0x10100000
#define	USBD_PHYS_ADDR		0x10200000
#define PCI_PHYS_ADDR		0x14005000
#define	MAC0_PHYS_ADDR		0x11500000
#define	MAC1_PHYS_ADDR		0x11510000
#define	MACEN_PHYS_ADDR		0x11520000
#define	MACDMA0_PHYS_ADDR	0x14004000
#define	MACDMA1_PHYS_ADDR	0x14004200
#define	I2S_PHYS_ADDR		0x11000000
#define	UART0_PHYS_ADDR		0x11100000
#define	UART3_PHYS_ADDR		0x11400000
#define GPIO2_PHYS_ADDR		0x11700000
#define	SYS_PHYS_ADDR		0x11900000
#define PCI_MEM_PHYS_ADDR     0x400000000
#define PCI_IO_PHYS_ADDR      0x500000000
#define PCI_CONFIG0_PHYS_ADDR 0x600000000
#define PCI_CONFIG1_PHYS_ADDR 0x680000000
#define PCMCIA_IO_PHYS_ADDR   0xF00000000
#define PCMCIA_ATTR_PHYS_ADDR 0xF40000000
#define PCMCIA_MEM_PHYS_ADDR  0xF80000000
#endif

/********************************************************************/

#ifdef SOC_AU1100
#define	MEM_PHYS_ADDR		0x14000000
#define	STATIC_MEM_PHYS_ADDR	0x14001000
#define	DMA0_PHYS_ADDR		0x14002000
#define	DMA1_PHYS_ADDR		0x14002100
#define	DMA2_PHYS_ADDR		0x14002200
#define	DMA3_PHYS_ADDR		0x14002300
#define	DMA4_PHYS_ADDR		0x14002400
#define	DMA5_PHYS_ADDR		0x14002500
#define	DMA6_PHYS_ADDR		0x14002600
#define	DMA7_PHYS_ADDR		0x14002700
#define	IC0_PHYS_ADDR		0x10400000
#define SD0_PHYS_ADDR		0x10600000
#define SD1_PHYS_ADDR		0x10680000
#define	IC1_PHYS_ADDR		0x11800000
#define	AC97_PHYS_ADDR		0x10000000
#define	USBH_PHYS_ADDR		0x10100000
#define	USBD_PHYS_ADDR		0x10200000
#define	IRDA_PHYS_ADDR		0x10300000
#define	MAC0_PHYS_ADDR		0x10500000
#define	MACEN_PHYS_ADDR		0x10520000
#define	MACDMA0_PHYS_ADDR	0x14004000
#define	MACDMA1_PHYS_ADDR	0x14004200
#define	I2S_PHYS_ADDR		0x11000000
#define	UART0_PHYS_ADDR		0x11100000
#define	UART1_PHYS_ADDR		0x11200000
#define	UART3_PHYS_ADDR		0x11400000
#define	SSI0_PHYS_ADDR		0x11600000
#define	SSI1_PHYS_ADDR		0x11680000
#define GPIO2_PHYS_ADDR		0x11700000
#define	SYS_PHYS_ADDR		0x11900000
#define LCD_PHYS_ADDR		0x15000000
#define PCMCIA_IO_PHYS_ADDR   0xF00000000
#define PCMCIA_ATTR_PHYS_ADDR 0xF40000000
#define PCMCIA_MEM_PHYS_ADDR  0xF80000000
#endif

/***********************************************************************/

#ifdef SOC_AU1550
#define	MEM_PHYS_ADDR		0x14000000
#define	STATIC_MEM_PHYS_ADDR	0x14001000
#define	IC0_PHYS_ADDR		0x10400000
#define	IC1_PHYS_ADDR		0x11800000
#define	USBH_PHYS_ADDR		0x14020000
#define	USBD_PHYS_ADDR		0x10200000
#define PCI_PHYS_ADDR		0x14005000
#define	MAC0_PHYS_ADDR		0x10500000
#define	MAC1_PHYS_ADDR		0x10510000
#define	MACEN_PHYS_ADDR		0x10520000
#define	MACDMA0_PHYS_ADDR	0x14004000
#define	MACDMA1_PHYS_ADDR	0x14004200
#define	UART0_PHYS_ADDR		0x11100000
#define	UART1_PHYS_ADDR		0x11200000
#define	UART3_PHYS_ADDR		0x11400000
#define GPIO2_PHYS_ADDR		0x11700000
#define	SYS_PHYS_ADDR		0x11900000
#define	DDMA_PHYS_ADDR		0x14002000
#define PE_PHYS_ADDR		0x14008000
#define PSC0_PHYS_ADDR	 	0x11A00000
#define PSC1_PHYS_ADDR	 	0x11B00000
#define PSC2_PHYS_ADDR	 	0x10A00000
#define PSC3_PHYS_ADDR	 	0x10B00000
#define PCI_MEM_PHYS_ADDR     0x400000000
#define PCI_IO_PHYS_ADDR      0x500000000
#define PCI_CONFIG0_PHYS_ADDR 0x600000000
#define PCI_CONFIG1_PHYS_ADDR 0x680000000
#define PCMCIA_IO_PHYS_ADDR   0xF00000000
#define PCMCIA_ATTR_PHYS_ADDR 0xF40000000
#define PCMCIA_MEM_PHYS_ADDR  0xF80000000
#endif

/***********************************************************************/

#ifdef SOC_AU1200
#define	MEM_PHYS_ADDR		0x14000000
#define	STATIC_MEM_PHYS_ADDR	0x14001000
#define AES_PHYS_ADDR		0x10300000
#define CIM_PHYS_ADDR		0x14004000
#define	IC0_PHYS_ADDR		0x10400000
#define	IC1_PHYS_ADDR		0x11800000
#define USB_PHYS_ADDR		0x14020000
#define OTG_PHYS_ADDR		0x14020020
#define OHC_PHYS_ADDR		0x14020100
#define EHC_PHYS_ADDR		0x14020200
#define UDC_PHYS_ADDR		0x14022000
#define	UART0_PHYS_ADDR		0x11100000
#define	UART1_PHYS_ADDR		0x11200000
#define GPIO2_PHYS_ADDR		0x11700000
#define	SYS_PHYS_ADDR		0x11900000
#define	DDMA_PHYS_ADDR		0x14002000
#define PSC0_PHYS_ADDR	 	0x11A00000
#define PSC1_PHYS_ADDR	 	0x11B00000
#define PCMCIA_IO_PHYS_ADDR   0xF00000000
#define PCMCIA_ATTR_PHYS_ADDR 0xF40000000
#define PCMCIA_MEM_PHYS_ADDR  0xF80000000
#define SD0_PHYS_ADDR		0x10600000
#define SD1_PHYS_ADDR		0x10680000
#define LCD_PHYS_ADDR		0x15000000
#define SWCNT_PHYS_ADDR		0x1110010C
#define MAEFE_PHYS_ADDR		0x14012000
#define MAEBE_PHYS_ADDR		0x14010000
#endif

/***********************************************************************/

#ifdef SOC_AU13XX
/*
 * System Bus
 */
#define	MEM_PHYS_ADDR			0x14000000
#define	STATIC_MEM_PHYS_ADDR	0x14001000
#define UDMA_PHYS_ADDR			0x14001800
#define	DDMA_PHYS_ADDR			0x14002000
#define CIM_PHYS_ADDR			0x14004000
#define USB_PHYS_ADDR			0x14021000
#define OTG_PHYS_ADDR			0x14022000
#define OHC_PHYS_ADDR			0x14020400
#define OHC2_PHYS_ADDR			0x14020800
#define EHC_PHYS_ADDR			0x14020000
#define UDC_PHYS_ADDR			OHC2_PHYS_ADDR
#define MAE2BSA_PHYS_ADDR 		0x14030000
#define LCD_PHYS_ADDR			0x15000000

/*
 * Peripheral Bus
 */
#define ROM_PHYS_ADDR			0x10000000
#define OTP_PHYS_ADDR			0x10002000
#define VSS_PHYS_ADDR			0x11003000
#define SENSE_PHYS_ADDR			0x10003800
#define	UART0_PHYS_ADDR			0x10100000
#define	UART1_PHYS_ADDR			0x10101000
#define	UART2_PHYS_ADDR			0x10102000
#define	UART3_PHYS_ADDR			0x10103000
#define	GPINT_PHYS_ADDR			0x10200000
#define AES_PHYS_ADDR			0x10300000
#define MALI_PHYS_ADDR			0x10500000
#define SD0_PHYS_ADDR			0x10600000
#define SD1_PHYS_ADDR			0x10601000
#define SD2_PHYS_ADDR			0x10602000
#define	SYS_PHYS_ADDR			0x10900000
#define PSC0_PHYS_ADDR	 		0x10A00000
#define PSC1_PHYS_ADDR	 		0x10A01000
#define PSC2_PHYS_ADDR	 		0x10A02000
#define PSC3_PHYS_ADDR	 		0x10A03000

#define PCMCIA_IO_PHYS_ADDR   	0xF00000000
#define PCMCIA_ATTR_PHYS_ADDR 	0xF40000000
#define PCMCIA_MEM_PHYS_ADDR  	0xF80000000
#endif


#endif /* _AU1X00_H */
