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
*****************************************************************************

/* CTG TODO -- This will need to match up with new GPINT controller */


#ifndef __AU1X00_S__
#define __AU1X00_S__

/***********************************************************************/

//
// Interrupts on the AMD Alchemy processors
// These are in order as they appear in intr 0 then intr 1
//

#ifdef CPU_AU1000
#define HWINTR_UART0		0
#define HWINTR_UART1		1
#define HWINTR_UART2		2
#define HWINTR_UART3		3
#define HWINTR_SSI0			4
#define HWINTR_SSI1			5
#define HWINTR_DMA0         6
#define HWINTR_DMA1         7
#define HWINTR_DMA2         8
#define HWINTR_DMA3         9
#define HWINTR_DMA4         10
#define HWINTR_DMA5         11
#define HWINTR_DMA6         12
#define HWINTR_DMA7         13
#define HWINTR_TOYTICK      14
#define HWINTR_TOYMATCH0    15
#define HWINTR_TOYMATCH1    16
#define HWINTR_TOYMATCH2    17
#define HWINTR_RTCTICK      18
#define HWINTR_RTCMATCH0    19
#define HWINTR_RTCMATCH1    20
#define HWINTR_RTCMATCH2    21
#define HWINTR_IRDATX       22
#define HWINTR_IRDARX       23
#define HWINTR_USBDEVICEREQ 24
#define HWINTR_USBDEVICESUSP 25
#define HWINTR_USBHOST      26
#define HWINTR_AC97			27
#define HWINTR_MAC0DMA      28
#define HWINTR_MAC1DMA      29
#define HWINTR_I2S			30
#define HWINTR_AC97CMDDONE  31
#endif

#ifdef CPU_AU1500
#define HWINTR_UART0		0
#define HWINTR_INTA         1
#define HWINTR_INTB         2
#define HWINTR_UART3		3
#define HWINTR_INTC         4
#define HWINTR_INTD         5
#define HWINTR_DMA0         6
#define HWINTR_DMA1         7
#define HWINTR_DMA2         8
#define HWINTR_DMA3         9
#define HWINTR_DMA4         10
#define HWINTR_DMA5         11
#define HWINTR_DMA6         12
#define HWINTR_DMA7         13
#define HWINTR_TOYTICK      14
#define HWINTR_TOYMATCH0    15
#define HWINTR_TOYMATCH1    16
#define HWINTR_TOYMATCH2    17
#define HWINTR_RTCTICK      18
#define HWINTR_RTCMATCH0    19
#define HWINTR_RTCMATCH1    20
#define HWINTR_RTCMATCH2    21
#define HWINTR_PCIERR       22
#define HWINTR_USBDEVICEREQ 24
#define HWINTR_USBDEVICESUSP 25
#define HWINTR_USBHOST      26
#define HWINTR_AC97			27
#define HWINTR_MAC0DMA      28
#define HWINTR_MAC1DMA      29
#define HWINTR_AC97CMDDONE  31
#define HWINTR_GPIO200      (16+HWINTR_GPIOBASE)
#define HWINTR_GPIO201      (17+HWINTR_GPIOBASE)
#define HWINTR_GPIO202      (18+HWINTR_GPIOBASE)
#define HWINTR_GPIO203      (19+HWINTR_GPIOBASE)
#define HWINTR_GPIO204      (21+HWINTR_GPIOBASE)
#define HWINTR_GPIO205      (22+HWINTR_GPIOBASE)
#endif

#ifdef CPU_AU1100
#define HWINTR_UART0		0
#define HWINTR_UART1		1
#define HWINTR_SD			2
#define HWINTR_UART3		3
#define HWINTR_SSI0			4
#define HWINTR_SSI1			5
#define HWINTR_DMA0         6
#define HWINTR_DMA1         7
#define HWINTR_DMA2         8
#define HWINTR_DMA3         9
#define HWINTR_DMA4         10
#define HWINTR_DMA5         11
#define HWINTR_DMA6         12
#define HWINTR_DMA7         13
#define HWINTR_TOYTICK      14
#define HWINTR_TOYMATCH0    15
#define HWINTR_TOYMATCH1    16
#define HWINTR_TOYMATCH2    17
#define HWINTR_RTCTICK      18
#define HWINTR_RTCMATCH0    19
#define HWINTR_RTCMATCH1    20
#define HWINTR_RTCMATCH2    21
#define HWINTR_IRDATX       22
#define HWINTR_IRDARX       23
#define HWINTR_USBDEVICEREQ 24
#define HWINTR_USBDEVICESUSP 25
#define HWINTR_USBHOST      26
#define HWINTR_AC97			27
#define HWINTR_MAC0DMA      28
#define HWINTR_GPIO215208	29
#define HWINTR_LCD			30
#define HWINTR_AC97CMDDONE  31
#endif

#ifdef CPU_AU1550
#define HWINTR_UART0		0
#define HWINTR_INTA         1
#define HWINTR_INTB         2
#define HWINTR_DDMA			3
#define HWINTR_ENC          4
#define HWINTR_INTC         5
#define HWINTR_INTD         6
#define HWINTR_PCIRST		7
#define HWINTR_UART1		8
#define HWINTR_UART3		9
#define HWINTR_PSC0			10
#define HWINTR_PSC1         11
#define HWINTR_PSC2         12
#define HWINTR_PSC3         13
#define HWINTR_TOYTICK      14
#define HWINTR_TOYMATCH0    15
#define HWINTR_TOYMATCH1    16
#define HWINTR_TOYMATCH2    17
#define HWINTR_RTCTICK      18
#define HWINTR_RTCMATCH0    19
#define HWINTR_RTCMATCH1    20
#define HWINTR_RTCMATCH2    21
#define HWINTR_PCIERR       22
#define HWINTR_NAND			23
#define HWINTR_USBDEVICEREQ 24
#define HWINTR_USBDEVICESUSP 25
#define HWINTR_USBHOST      26
#define HWINTR_MAC0DMA      27
#define HWINTR_MAC1DMA      28
#define HWINTR_GPIO200      (16+HWINTR_GPIOBASE)
#define HWINTR_GPIO201205   (17+HWINTR_GPIOBASE)
#define HWINTR_GPIO206      (29+HWINTR_GPIOBASE)
#define HWINTR_GPIO207      (30+HWINTR_GPIOBASE)
#define HWINTR_GPIO208215   (31+HWINTR_GPIOBASE)
#endif

#ifdef CPU_AU1200
#define HWINTR_UART0		0
#define HWINTR_SWC			1
#define HWINTR_SD			2
#define HWINTR_DDMA			3
#define HWINTR_MAEBE		4
#define HWINTR_GPIO200		5
#define HWINTR_GPIO201		6
#define HWINTR_GPIO202		7
#define HWINTR_UART1		8
#define HWINTR_MAEFE		9
#define HWINTR_PSC0			10
#define HWINTR_PSC1			11
#define HWINTR_AES			12
#define HWINTR_CIM			13
#define HWINTR_TOYTICK      14
#define HWINTR_TOYMATCH0    15
#define HWINTR_TOYMATCH1    16
#define HWINTR_TOYMATCH2    17
#define HWINTR_RTCTICK      18
#define HWINTR_RTCMATCH0    19
#define HWINTR_RTCMATCH1    20
#define HWINTR_RTCMATCH2    21
#define HWINTR_GPIO203		22
#define HWINTR_NAND			23
#define HWINTR_GPIO204		24
#define HWINTR_GPIO205		25
#define HWINTR_GPIO206		26
#define HWINTR_GPIO207		27
#define HWINTR_GPIO208215	28
#define HWINTR_USB			29
#define HWINTR_USBHOST		29
#define HWINTR_LCD			30
#define HWINTR_MAEDONE		31
#endif

#ifdef CPU_AU13XX
#define HWINTR_UART1		17
#define HWINTR_UART2		25
#define HWINTR_UART3		27
#define HWINTR_SD1			32
#define HWINTR_SD2			38
#define HWINTR_PSC0			48
#define HWINTR_PSC1			52
#define HWINTR_PSC2			56
#define HWINTR_PSC3			60
#define HWINTR_NAND			62
#define HWINTR_DDMA			75
#define HWINTR_MMU			76
#define HWINTR_MGP			77
#define HWINTR_GPU			78
#define HWINTR_UDMA			79
#define HWINTR_TOYTICK      80
#define HWINTR_TOYMATCH0    81
#define HWINTR_TOYMATCH1    82
#define HWINTR_TOYMATCH2    83
#define HWINTR_RTCTICK      84
#define HWINTR_RTCMATCH0    85
#define HWINTR_RTCMATCH1    86
#define HWINTR_RTCMATCH2    87
#define HWINTR_UART0		88
#define HWINTR_SD0			89
 #define HWINTR_SD			HWINTR_SD0
#define HWINTR_USB			90
#define HWINTR_LCD			91
#define HWINTR_SCF			92
#define HWINTR_MPE			93
#define HWINTR_BSA			94
#define HWINTR_AES			95
#define HWINTR_CIM			96

#define HWINTR_GPIO(n)		n

// GPINT_MAX_INTS defined in Au1x00.h ( 128 total )
#define HWINTR_INTERNAL_MAX GPINT_MAX_INTS-1

#endif


#ifndef CPU_AU13XX
//
// Primary GPIOs
//
#define HWINTR_GPIOBASE   32
#define HWINTR_GPIO0      (0+HWINTR_GPIOBASE)
#define HWINTR_GPIO1      (1+HWINTR_GPIOBASE)
#define HWINTR_GPIO2      (2+HWINTR_GPIOBASE)
#define HWINTR_GPIO3      (3+HWINTR_GPIOBASE)
#define HWINTR_GPIO4      (4+HWINTR_GPIOBASE)
#define HWINTR_GPIO5      (5+HWINTR_GPIOBASE)
#define HWINTR_GPIO6      (6+HWINTR_GPIOBASE)
#define HWINTR_GPIO7      (7+HWINTR_GPIOBASE)
#define HWINTR_GPIO8      (8+HWINTR_GPIOBASE)
#define HWINTR_GPIO9      (9+HWINTR_GPIOBASE)
#define HWINTR_GPIO10     (10+HWINTR_GPIOBASE)
#define HWINTR_GPIO11     (11+HWINTR_GPIOBASE)
#define HWINTR_GPIO12     (12+HWINTR_GPIOBASE)
#define HWINTR_GPIO13     (13+HWINTR_GPIOBASE)
#define HWINTR_GPIO14     (14+HWINTR_GPIOBASE)
#define HWINTR_GPIO15     (15+HWINTR_GPIOBASE)
#define HWINTR_GPIO16     (16+HWINTR_GPIOBASE)
#define HWINTR_GPIO17     (17+HWINTR_GPIOBASE)
#define HWINTR_GPIO18     (18+HWINTR_GPIOBASE)
#define HWINTR_GPIO19     (19+HWINTR_GPIOBASE)
#define HWINTR_GPIO20     (20+HWINTR_GPIOBASE)
#define HWINTR_GPIO21     (21+HWINTR_GPIOBASE)
#define HWINTR_GPIO22     (22+HWINTR_GPIOBASE)
#define HWINTR_GPIO23     (23+HWINTR_GPIOBASE)
#define HWINTR_GPIO24     (24+HWINTR_GPIOBASE)
#define HWINTR_GPIO25     (25+HWINTR_GPIOBASE)
#define HWINTR_GPIO26     (26+HWINTR_GPIOBASE)
#define HWINTR_GPIO27     (27+HWINTR_GPIOBASE)
#define HWINTR_GPIO28     (28+HWINTR_GPIOBASE)
#define HWINTR_GPIO29     (29+HWINTR_GPIOBASE)
#define HWINTR_GPIO30     (30+HWINTR_GPIOBASE)
#define HWINTR_GPIO31     (31+HWINTR_GPIOBASE)

// Used for bounds checking in intr.c and isr.s
#define HWINTR_IC0_BASE   0
#define HWINTR_IC0_NUM    32
#define HWINTR_IC0_MAX    (HWINTR_IC0_BASE+HWINTR_IC0_NUM-1)

#define HWINTR_IC1_BASE   (HWINTR_IC0_MAX+1)
#define HWINTR_IC1_NUM    32
#define HWINTR_IC1_MAX    (HWINTR_IC1_BASE+HWINTR_IC1_NUM-1)

#define HWINTR_INTERNAL_MAX HWINTR_IC1_MAX

#endif // CPU_AU13XX
//
// External Interrupts
// These are to be defined, as appropriate, in <platform.h>
//
#define HWINTR_EXT_BASE    (HWINTR_INTERNAL_MAX+1)
#define HWINTR_EXT_NUM     32
#define HWINTR_EXT_MAX     (HWINTR_EXT_BASE+HWINTR_EXT_NUM-1)

// We allocate an extra 32 hwintrs for external
// interrupt controller use
#define HWINTR_MAXIMUM    (HWINTR_EXT_MAX)

// DDMA controller interrupts (16 of) treated as a 3rd interrupt controller
#ifdef DDMA_PHYS_ADDR
#define HWINTR_DDMA_BASE   (HWINTR_EXT_MAX+1)
#define HWINTR_DDMA_NUM    (DDMA_NUM_CHANNELS)
#define HWINTR_DDMA_MAX    (HWINTR_DDMA_BASE+HWINTR_DDMA_NUM-1)
#undef HWINTR_MAXIMUM
#define HWINTR_MAXIMUM    (HWINTR_DDMA_MAX)
#endif

#define HWINTR_UNASSIGNED  0xffffffff

#endif
