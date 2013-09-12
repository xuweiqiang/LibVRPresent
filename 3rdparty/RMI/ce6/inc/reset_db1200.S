/*********************************************************************
 *
 * Copyright:
 *	Advanced Micro Devices, AMD. All Rights Reserved.  
 *  You are hereby granted a copyright license to use, modify, and
 *  distribute the SOFTWARE so long as this entire notice is
 *  retained without alteration in any modified and/or redistributed
 *  versions, and that such modified versions are clearly identified
 *  as such. No licenses are granted by implication, estoppel or
 *  otherwise under any patents or trademarks of AMD. This 
 *  software is provided on an "AS IS" basis and without warranty.
 *
 *  To the maximum extent permitted by applicable law, AMD 
 *  DISCLAIMS ALL WARRANTIES WHETHER EXPRESS OR IMPLIED, INCLUDING 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR
 *  PURPOSE AND ANY WARRANTY AGAINST INFRINGEMENT WITH REGARD TO THE 
 *  SOFTWARE (INCLUDING ANY MODIFIED VERSIONS THEREOF) AND ANY 
 *  ACCOMPANYING WRITTEN MATERIALS.
 * 
 *  To the maximum extent permitted by applicable law, IN NO EVENT
 *  SHALL AMD BE LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING 
 *  WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS 
 *  INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY
 *  LOSS) ARISING OF THE USE OR INABILITY TO USE THE SOFTWARE.   
 * 
 *  AMD assumes no responsibility for the maintenance and support
 *  of this software.
 ********************************************************************/

/*

This file represents all the activities necessary to bring
the Au1200 out of reset. It sets all Au1200 resources to
known, usually disabled and safe, state.

This is an example startup file, tailored for the DBAu1200
reference board. DBAu1200-specific items are commented as
such, but in general are confined to the CPU endian
selection and memory controller values.

*/


/********************************************************************/

/*
 * MIPS ABI register defintions
 */
#define zero	$0
#define v0		$2
#define v1		$3
#define a0		$4
#define a1		$5
#define a2		$6
#define a3		$7
#define t0		$8
#define t1		$9
#define t2		$10
#define t3		$11
#define t4		$12
#define t5		$13
#define t6		$14
#define t7		$15
#define s0		$16
#define s1		$17
#define s2		$18
#define s3		$19
#define s4		$20
#define s5		$21
#define s6		$22
#define s7		$23
#define t8		$24
#define t9		$25
#define k0		$26
#define k1		$27
#define gp		$28
#define sp		$29
#define fp		$30
#define ra		$31

/********************************************************************/

/*
 * Au1200 CP0 registers
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

/********************************************************************/

/*
 * Au1200 base addresses (in KSEG1 region)
 */
#define AU1200_MEM_ADDR		0xB4000000
#define AU1200_MAEBE_ADDR	0xB4010000
#define AU1200_MAEFE_ADDR	0xB4012000
#define AU1200_DDMA_ADDR    0xB4003000
#define AU1200_USB_ADDR		0xB4020000
#define AU1200_OTG_ADDR		0xB4020020
#define AU1200_OHC_ADDR		0xB4020100
#define AU1200_EHC_ADDR		0xB4020200
#define AU1200_DEV_ADDR		0xB4022000
#define AU1200_CIM_ADDR		0xB4004000
#define AU1200_LCD_ADDR		0xB5000000
#define AU1200_IC0_ADDR     0xB0400000
#define AU1200_UART0_ADDR   0xB1100000
#define AU1200_UART1_ADDR   0xB1200000
#define AU1200_SWC_ADDR		0xB1100100
#define AU1200_GPIO2_ADDR	0xB1700000
#define AU1200_IC1_ADDR     0xB1800000
#define AU1200_SYS_ADDR		0xB1900000
#define AU1200_PSC0_ADDR    0xB1A00000
#define AU1200_PSC1_ADDR	0xB1B00000
#define AU1200_AES_ADDR		0xB0300000
#define AU1200_SD0_ADDR		0xB0600000
#define AU1200_SD1_ADDR		0xB0680000


/*
 * Au1200 memory controller register offsets
 */
#define mem_sdmode0	    	0x0800
#define mem_sdmode1	    	0x0808
#define mem_sdaddr0    		0x0820
#define mem_sdaddr1    		0x0828
#define mem_sdconfiga		0x0840
#define mem_sdconfigb		0x0848
#define mem_sdstat		    0x0850
#define mem_sdwrmd0 		0x0880
#define mem_sdwrmd1 		0x0888
#define mem_sdprecmd  	    0x08C0
#define mem_sdautoref 	    0x08C8
#define mem_sdsref  	    0x08D0

#define mem_stcfg0		    0x1000
#define mem_sttime0		    0x1004
#define mem_staddr0		    0x1008
#define mem_stcfg1		    0x1010
#define mem_sttime1		    0x1014
#define mem_staddr1		    0x1018
#define mem_stcfg2		    0x1020
#define mem_sttime2		    0x1024
#define mem_staddr2		    0x1028
#define mem_stcfg3		    0x1030
#define mem_sttime3		    0x1034
#define mem_staddr3		    0x1038
#define mem_staltime		0x1040
#define mem_stndctrl		0x1100


/*
 * Au1200 peripheral register offsets
 */
#define ddma_inten			0x000C
#define psc_enable		    0x0004		
#define uart_enable		    0x0100
#define maebe_ctlenable		0x0700
#define cim_enable			0x0000
#define lcd_screen			0x0004
#define swcnt_control		0x0000
#define usb_cfg				0x0004
#define sd_enable			0x000C
#define aes_status			0x0000

#define ic_cfg0clr		    0x0044
#define ic_cfg1clr		    0x004C
#define ic_cfg2clr		    0x0054
#define ic_srcset		    0x0058
#define ic_assignset	    0x0060
#define ic_wakeclr		    0x006C
#define ic_maskclr		    0x0074
#define ic_risingclr	    0x0078
#define ic_fallingclr	    0x007C
#define ic_testbit		    0x0080

#define sys_scratch0	    0x0018
#define sys_scratch1	    0x001c
#define sys_cntctrl		    0x0014
#define sys_freqctrl0	    0x0020
#define sys_freqctrl1	    0x0024
#define sys_clksrc		    0x0028
#define sys_pinfunc		    0x002C
#define sys_wakemsk		    0x0034
#define sys_powerctrl	    0x003C
#define sys_endian		    0x0038
#define sys_wakesrc		    0x005C
#define sys_cpupll		    0x0060
#define sys_auxpll		    0x0064
#define sys_trioutclr	    0x0100
#define sys_outputset		0x0108
#define sys_outputclr	    0x010C
#define sys_pininputen	    0x0110

#define gpio2_dir		    0x0000
#define gpio2_output	    0x0008
#define gpio2_pinstate	    0x000C
#define gpio2_inten		    0x0010
#define gpio2_enable	    0x0014

/********************************************************************/

/*
 * DBAu1200-specific values
 * NOTE: All values are for operation at 396MHz, SD=2
 */

#define SYS_CPUPLL		33	/* 396Mhz */
#define SYS_POWERCTRL	0	/* SD=2   */
#define SYS_AUXPLL		8	/* 96MHz for LCD */

/*
 * With SBUS of 198MHz and mem_stcfg0[TS]=0, static bus controller
 * clock period is 5ns
 */

/* Address Latch Timing - SN74LVC16374ADVG latch on rising edge of ALE
  Tah=000 (0   clocks)
  Tlw=000 (1+0 clocks)
 Tasu=001 (1   clocks) Latch needs 3ns
 */
#define MEM_STALTIME 0x00000001

/* RCE0: 2x32MB AM29LV256M MirrorBit - 120ns, or
         2x32MB Spansion S29GL256N10T MirrorBit Flash Vio=3V, 100ns
		 Using 120ns timing to maintain compatibility

 Tcsoe= (1+0  clocks) data sheet specs MAX(tCE 120ns - tOE 25ns), choose 5ns for stable address, AS=1
 Toecs= (0+0  clocks) data sheet specs   0ns, AH=0
  Twcs= (1+0  clocks) data sheet specs   0ns for tCH
  Tcsh= (1+6  clocks) data sheet specs MAX(tWPH 30ns, tCEH 35ns)
Tcsoff= (1+6  clocks) match Tcsh
   Twp= (1+6  clocks) data sheet specs  35ns for tWP
  Tcsw= (1+0  clocks) data sheet specs   0ns for tCS
   Tpm= (1+7  clocks) data sheet specs  40ns for tPACC
    Ta= (1+23 clocks) data sheet specs 120ns for tRC

mem_stcfg0: 0000 0000 0010 1101 0000 0000 0100 0011 : 0x002D0043
Tcsoe=000 (see above)
Toecs=000 (see above)
   AH=0   (not needed)
   NW=0   (n/a)
   AS=1   (setup needed)
    S=0   (asynchronous)
   DE=1   (de-assert)
 MBSa=1
  MBC=0
   TA=1   (Tcsh for reads and writes)
  DIV=000 (n/a)
  ALD=0   (address latch enable)
   AV=0   (no address visibility)
   BE=0   (little endian)
   TS=0   (asynchronous operation)
   EW=0   (n/a)
 MBSb=1
   BS=0   (n/a)
   PM=0   (disable page mode)
   RO=0   (writable)
  DTY=3   (NOR Flash)

mem_sttime0: 0000 0110 0110 0001 1000 0001 1001 0101 : 0x06618195
  Twcs=000
  Tcsh=0110
Tcsoff=110
   Twp=000110
  Tcsw=0000
   Tpm=0111
    Ta=010111
 */
#define MEM_STCFG0	0x002D0043 /* 16-bit little-endian */
#define MEM_STTIME0	0x066181D7
#define MEM_STADDR0	0x11C03F00

/* RCE1: Samsung K9F1208UDA-YCB0 x8 NAND Flash */
#define MEM_STCFG1	0x00420045
#define MEM_STTIME1	0x00007774
#define MEM_STADDR1	0x12000FFF

/* RCE2:   CPLD, LAN91C111, IDE PIOmode4

 Tcsoe=MAX( 0ns,      20ns,         25ns) = (1+4  clocks) AS=1
 Toecs=MAX( 0ns,       5ns,         10ns) = (1+1  clocks) AH=1
  Twcs=MAX( 0ns,       5ns,         10ns) = (1+1  clocks)
  Tcsh=MAX(10ns,      20ns,         25ns) = (1+4  clocks)
Tcsoff=MAX(10ns,      20ns,         25ns) = (1+4  clocks)
   Twp=MAX(15ns,      20ns,         70ns) = (1+13 clocks)
  Tcsw=MAX(10ns,      20ns,         20ns) = (1+3  clocks)
   Tpm=MAX( 0ns,       0ns,       25/0ns) = (1+0  clocks)
    Ta=MAX( 5ns,      35ns,         95ns) = (1+18 clocks)

mem_stcfg2: 1000 0110 0010 1101 0000 0000 1100 0110 : 0x862D00C6 
Tcsoe=100 (see above)
Toecs=001 (see above)
   AH=1   (hold needed)
   NW=0   (n/a)
   AS=1   (setup needed)
    S=0   (asynchronous)
   DE=1   (de-assert)
 MBSa=1
  MBC=0
   TA=1   (Tcsh for reads and writes)
   BE=0   (little endian)
   TS=0   (asynchronous operation)
   EW=1   (absolutely needed)
 MBSb=1
   BS=0   (n/a)
   PM=0   (disable page mode)
   RO=0   (writable)
  DTY=6   (IDE)

mem_sttime2: 0001 0100 0100 0011 0100 1100 0001 0010 : 0x14434C12
  Twcs=001
  Tcsh=0100
Tcsoff=100
   Twp=001101
  Tcsw=0011
   Tpm=0000
    Ta=010010
*/
#define MEM_STCFG2	0x862D00C6
#define MEM_STTIME2	0x14434C12
#define MEM_STADDR2	0x11803f00

/* RCE3: PCMCIA 250ns */
#define MEM_STCFG3	0x00040042
#define MEM_STTIME3	0x280E3E07
#define MEM_STADDR3	0x10000000

#define MEM_SDCONFIGA_E		(1<<31)
#define MEM_SDCONFIGA_CE	(3<<28)
#define MEM_SDCONFIGB_BB	(1<<19)
#define MEM_SDCONFIGB_BA	(1<<7)

/*
 * SDCS0 - 128MB DDR2-533 Samsung K4T51163QB-GCD5 (8Mbit x 16 x 4bank x 2devices)
 * SDCS1 - 128MB DDR2-533 Samsung K4T51163QB-GCD5 (8Mbit x 16 x 4bank x 2devices)

With a DDR clock of 198MHz (sdconfigb[CR]=1), DDR clock period is 5ns

mem_sdmode: 0000 0001 0010 0111 0010 0010 0010 0100 : 0x01272224
  Twtr=001  (1+1 clocks) data sheet specs 10ns for tWTR
   Twr=010  (1+2 clocks) data sheet specs 15ns for tWR
  Tras=0111 (1+7 clocks) data sheet specs 40ns for tRAS
   Trp=010  (1+2 clocks) data sheet specs 15ns for tRP
Trcdwr=010  (1+2 clocks) data sheet specs 15ns for tRCD
Trcdrd=010  (1+2 clocks) data sheet specs 15ns for tRCD
  Tcas=100  (CL=3      ) data sheet specs CL=3 for 400mhz

mem_sdaddr: 0010 0011 0001 0000 0000 0011 1110 0000 : 0x231003E0
    BR=0    (bank,row,col)
    RS=10   (13 row)
    CS=011  (10 col)
	 E=1    (enabled)
  CSBA=0000000000 (0x00000000)
CSMASK=1111100000 (0xF8000000)

mem_sdconfiga: 0011 0001 0100 0000 0000 0110 0000 1010 : 0x3140060A
     E=0    (refresh disable)
    CE=11   (both clocks enabled)
   RPT=00   (1 refresh per cycle)
   Trc=010100 (1+20 clocks) data sheet specs 55ns for tRC, 105ns for tRFC
   REF=0x60A  (1562 clocks) data sheet specs 7.8125us intervals (8K rows in 64ms)

mem_sdconfigb: 1010 0000 0000 0010 0000 0000 0000 0000 : 0xA002000C
    CR=1    (1:1)
    BW=0    (32bit wide bus)
    MT=1    (DDR2)
  PSEL=0    (addr 10 for auto precharge)
    C2=0    (core lowest priority)
    AC=00   (default)
    HP=0    (no half-pll mode)
    PM=00   (no power modes)
CKECNT=00   (n/a)
    BB=0    (normal)
    DS=1    (full drive strength)
    FS=0    (normal)
   PDX=00   (n/a)
CKEmin=00   (n/a ?)
    CB=0    (normal)
 TXARD=000  (n/a)
    BA=0    (no block)
  TXSR=001100 (1+12 * 16=208 > 200 clocks)

mem_sdwrmd:
Mode Register 0: 0000 0100 0011 0010 : 0x0432
   PD=0     Fast Exit
   WR=010   3 Clocks
  DLL=0     Normal
   TM=0     Normal
   CL=011   CL=3
   BT=0     sequential burst type
   BL=010   burst length of 4

Mode Register 1: 0000 0100 0000 0000 : 0x0400
  OUT=0     Normal drive strength
 RDQS=0     Disable
  DQS=1     Disable
  OCD=000   Not supported
  RTT=10    150 Ohm termination needed with two ranks populated
   AL=00    0
  ODS=0     100%
  DLL=0     Normal/Enable

Mode Register 2: 0x0000
Mode Register 3: 0x0000
 */
#define MEM_SDMODE0_DDR2	0x01272224
#define MEM_SDMODE1_DDR2	0x01272224
#define MEM_SDADDR0_DDR2	0x231003E0
#define MEM_SDADDR1_DDR2	0x231083E0
#define MEM_SDCONFIGA_DDR2	0x3140060A
#define MEM_SDCONFIGB_DDR2	0xA002000C
#define MEM_MR0_DDR2		0x00000432
#define MEM_MR1_DDR2		0x40000440
#define MEM_MR2_DDR2		0x80000000
#define MEM_MR3_DDR2		0xC0000000

#define MEM_1MS			((396000000/1000000) * 1000)

/*
 * Board CPLD registers
 */
#define DB1200_BCSR_ADDR	0xB9800000

#define bcsr_whoami			0x00
#define bcsr_status			0x04
#define bcsr_switches		0x08
#define bcsr_resets			0x0C
#define bcsr_pcmcia			0x10
#define bcsr_board			0x14
#define bcsr_leds			0x18
#define bcsr_system			0x1C
#define bcsr_icer			0x20
#define bcsr_iser			0x24
#define bcsr_icmr			0x28
#define bcsr_ismr			0x2C
#define bcsr_ssr			0x30
#define bcsr_isr			0x34

/********************************************************************/
/********************************************************************/
/********************************************************************/
/********************************************************************/

	.text
	.set noreorder
//	.set mips32

	/*
	 * Step 1) Establish CPU endian mode.
	 * DBAu1200-specific:
	 * Switch S6.1 Off(bit7 reads 1) is Little Endian
	 * Switch S6.1 On (bit7 reads 0) is Big Endian
	 */
	li		t0, AU1200_MEM_ADDR
	li		t1, MEM_STCFG2
	sw		t1, mem_stcfg2(t0)
	li		t1, MEM_STTIME2
	sw		t1, mem_sttime2(t0)
	li		t1, MEM_STADDR2
	sw		t1, mem_staddr2(t0)
	sync

	li		t0, DB1200_BCSR_ADDR
	lh		t1, bcsr_switches(t0)
	andi	t1, t1, 0x80
	beq		zero, t1, big_endian
	nop

	/* Set to little endian */

little_endian:	

	/* Change Au1 core to little endian */
	li		t0, AU1200_SYS_ADDR
	li		t1, 1
	sw		t1, sys_endian(t0)
	sync
	mfc0	t2, CP0_Config
	mtc0	t2, CP0_Config
	nop
	nop

	/* Big Endian is default so nothing to do but fall through */

big_endian:

	/*
	 * NOTE: Config0[BE] now reflects endian mode
	 */

	/*
	 * Step 2) Establish Status Register
	 * (set BEV, clear ERL, clear EXL, clear IE)
	 */
	li		t1, 0x00400000
	mtc0	t1, CP0_Status

	/*
	 * Step 3) Establish CP0 Config0
	 * (set OD, set K0=3)
	 */
	li		t1, 0x00080003
	mtc0	t1, CP0_Config0

	/*
	 * Step 4) Disable Watchpoint facilities
	 */
	li t1, 0x00000000
	mtc0	t1, CP0_WatchLo
	mtc0	t1, CP0_IWatchLo

	/*
	 * Step 5) Disable the performance counters
	 */
	mtc0	zero, CP0_PerfCtrl
	nop

	/*
	 * Step 6) Establish EJTAG Debug register
	 */
	mtc0	zero, CP0_Debug
	nop

	/*
	 * Step 7) Establish Cause
	 * (set IV bit)
	 */
	li		t1, 0x00800000
	mtc0	t1, CP0_Cause

	/*
	 * Step 8) Initialize the caches
	 */
	li		t0, (16*1024)
	li		t1, 32
	li		t2, 0x80000000
	addu	t3, t0, t2
cacheloop:
	cache	0, 0(t2)
	cache	1, 0(t2)
	addu	t2, t1
	bne		t2, t3, cacheloop
	nop

	/* Run from cacheable space now */
	bal		cachehere
	nop
cachehere:
	li		t1, ~0x20000000 /* convert to KSEG0 */
	and		t0, ra, t1
	addi	t0, 5*4			/* 5 insns beyond cachehere */
	jr		t0
	nop

	/*
	 * Step 9) Initialize the TLB
	 */
	li		t0, 0		 	# index value
	li		t1, 0x00000000 	# entryhi value
	li		t2, 32		   	# 32 entries
tlbloop:
	/* Probe TLB for matching EntryHi */
	mtc0	t1, CP0_EntryHi
	tlbp
	nop

	/* Examine Index[P], 1=no matching entry */
	mfc0	t3, CP0_Index
	li		t4, 0x80000000
	and		t3, t4, t3
	addiu	t1, t1, 1		# increment t1 (asid)
	beq		zero, t3, tlbloop
	nop

	/* Initialize the TLB entry */
	mtc0	t0, CP0_Index
	mtc0	zero, CP0_EntryLo0
	mtc0	zero, CP0_EntryLo1
	mtc0	zero, CP0_PageMask
	tlbwi

	/* Do it again */
	addiu	t0, t0, 1	
	bne		t0, t2, tlbloop
	nop

	/* Establish Wired (and Random) */
	mtc0	zero, CP0_Wired
	nop

/*
 * Run-time determination of the CPU, SBUS, and DDR frequency.
 * Switch S6.[2345] is utilized to encode a value an index 0 to 15.
 */
#ifdef USE_S6
	b		lookup_table
	nop

	/* Table entry is multiple of 16 words so that offset into table is
	 * simple shift of index value.
	 * Offset Description
	 * 0: 0x00: sys_cpupll value
	 * 1: 0x04: sys_powerctrl value
	 * 2: 0x08: mem_stcfg0 value
	 * 3: 0x0C: mem_sttime0 value
	 * 4: 0x10: mem_stcfg1 value
	 * 5: 0x14: mem_sttime1 value
	 * 6: 0x18: mem_stcfg2 value
	 * 7: 0x1C: mem_sttime2 value
	 * 8: 0x20: mem_stcfg3 value
	 * 9: 0x24: mem_sttime3 value
	 * A: 0x28: mem_staltime value
	 * B: 0x2C: mem_sdconfiga value
	 * C: 0x30: mem_sdconfigb value
	 * D: 0x34: mem_sdmode value
	 * E: 0x38: mem_mr0 value
	 * F: 0x3C: mem_mr1 value
	 *
	 * When utilizing the tables, throughout this reset code, a0 will
	 * point to the selected configuration.
	 */
#undef SYS_CPUPLL
#undef SYS_POWERCTRL
#undef MEM_STCFG0
#undef MEM_STTIME0
#undef MEM_STCFG1
#undef MEM_STTIME1
#undef MEM_STCFG2
#undef MEM_STTIME2
#undef MEM_STCFG3
#undef MEM_STTIME3
#undef MEM_STALTIME
#define SYS_CPUPLL		0x00
#define SYS_POWERCTRL	0x04
#define MEM_STCFG0		0x08
#define MEM_STTIME0		0x0C
#define MEM_STCFG1		0x10
#define MEM_STTIME1		0x14
#define MEM_STCFG2		0x18
#define MEM_STTIME2		0x1C
#define MEM_STCFG3		0x20
#define MEM_STTIME3		0x24
#define MEM_STALTIME	0x28
#define MEM_SDCONFIGA	0x2C
#define MEM_SDCONFIGB	0x30
#define MEM_SDMODE		0x34
#define MEM_MR0			0x38
#define MEM_MR1			0x3C

DDR2_SPEEDS:
ddr2_396_198_198: // DDR2-400 CL3 Au1200-400
	.long 33, 0, 0x002D0043, 0x066181D7, 0x00420045, 0x00007774, 0x862D00C6, 0x14434C12, 0x00040042, 0x280E3E07, 0x00000001, 0x3140060A, 0xA002000C, 0x01272224, 0x00000432, 0x40000440
ddr2_324_162_162: // DDR2-400 CL3 Au1200-333
	.long 27, 0, 0x002D0043, 0x05514193, 0x00420045, 0x00006664, 0x862C00C6, 0x1442CC0F, 0x00040042, 0x210C34E6, 0x00000001, 0x311004F1, 0xA002000C, 0x01262224, 0x00000432, 0x40000440
ddr2_default: 
	.long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
ddr2_492_246_246: // DDR2-533 CL4 Au1200-500
	.long 41, 0, 0x202D0043, 0x0872025D, 0x00420045, 0x00009996, 0xCA2C00C6, 0x26645017, 0x00040042, 0x3214578A, 0x00000001, 0x31900781, 0xA002000C, 0x02393335, 0x00000642, 0x40000440
ddr2_600_200_200: // DDR2-400 CL3 Au1200-600
	.long 50, 1, 0x202D0043, 0x066181D7, 0x00420045, 0x00007774, 0x862C00C6, 0x14434C12, 0x00040042, 0x280E3E07, 0x00000001, 0x3140061A, 0xA002000C, 0x01272224, 0x00000432, 0x40000440
	.long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.long 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

lookup_table:
	la		a0, DDR2_SPEEDS
	li		t1, 0x20000000 /* convert to KSEG1 */
	or		a0, a0, t1
	/* Read S6 to determine which setting, test if valid */
	li		t0, DB1200_BCSR_ADDR
	lh		t1, bcsr_switches(t0)
	andi	t1, t1, 0x0078
	xori	t1, t1, 0x0078 /* invert to get user perspective of value */
	sll		t2, t1, 3
	addu	t3, a0, t2
	lw		t3, 0(t3)
	beq		zero, t3, default_config
	nop
	addu	a0, a0, t2
default_config:

	/* The correct table and configuration setting is pointed to by a0 */

	/*
	 * Step 10) Establish CPU PLL frequency
	 *
	 * Upon arriving here, the processor is one of these two situations:
	 * a) reset with core at 192MHz, default timing on RCE0, or
	 * b) core at previous freq, optimized timing for previous freq for RCE0
	 * Situation A happens with either hardware-, runtime-, hibernate- or
	 * wake-up reset.
	 * Situation B happens when software jumps to 0xBFC00000 to reboot.
	 *
	 * This code allows for selecting processor frequency (from a table of
	 * settings) at boot-time. To do so, the S12 dip switches select
	 * the configuration, and the sys_cpupll, sys_powerctrl, and RCE0 are
	 * updated [here] to reflect the new frequency. These three items must
	 * be properly configured for the Au1200 to change frequency and continue
	 * fetching code from Flash on RCE0.
	 *
	 * The configuration table contains values for all registers that affect
	 * timing in some fashion.
	 */

	li		t0, AU1200_SYS_ADDR

	lw		t1, SYS_CPUPLL(a0)
	lw		t2, SYS_POWERCTRL(a0)
	lw		t3, MEM_STCFG0(a0)
	lw		t4, MEM_STTIME0(a0)

	li		t5, AU1200_MEM_ADDR

	/*
	 * Reduce system bus to /4 (for large PLL jumps) and then
	 * jump to cache-aligned code which changes the frequency
	 */
	li		t6, 2
	sw		t6, sys_powerctrl(t0)
	sync
	beq		zero, zero, rampPLL
	nop

	/*
	 * Place the following code on an icache line boundary to force
	 * an icache fetch of the 8 insns below which change critical
	 * timing.
	 *
	 * If these insns are not in cache, then an icache miss
	 * results in potentially unusually-timed access to Flash,
	 * depending upon where the icache miss happens in the
	 * sequence below, which can have unpredictable results...
	 */
	.align 5
rampPLL:

	/*
	 * Update sys_cpupll, RCE0, and sys_powerctrl (order is important)
	 */
	sw		t1, sys_cpupll(t0)
	sync
	sw		t4, mem_sttime0(t5)
	sw		t3, mem_stcfg0(t5)
	sw		t2, sys_powerctrl(t0)
	sync

	/* End of critical timing code */

#else
	/*
	 * Step 10) Establish CPU PLL frequency
	 */
	li		t0, AU1200_SYS_ADDR
	li		t1, SYS_CPUPLL
	sw		t1, sys_cpupll(t0)
	sync
	nop
	nop

	/*
	 * Step 11) Establish system bus divider
	 */
	li		t1, SYS_POWERCTRL
	sw		t1, sys_powerctrl(t0)
	sync
#endif

	/*
	 * Step 12) Establish AUX PLL frequency
	 */
	li		t0, AU1200_SYS_ADDR
	li		t1, SYS_AUXPLL
	sw		t1, sys_auxpll(t0)
	sync

	/*
	 * Step 13) Start the 32kHz oscillator
	 */
	li		t1, 0x00000100
	sw		t1, sys_cntctrl(t0)
	sync

	/*
	 * Step 14) Initialize static memory controller
	 */
	li		t0, AU1200_MEM_ADDR
#ifndef USE_S6
	li		t1, MEM_STCFG0
	li		t2, MEM_STTIME0
#else
	lw		t1, MEM_STCFG0(a0)
	lw		t2, MEM_STTIME0(a0)
#endif
	li		t3, MEM_STADDR0
	sw		t1, mem_stcfg0(t0)
	sw		t2, mem_sttime0(t0)
	sw		t3, mem_staddr0(t0)
	sync

	/* RCE1 */
#ifndef USE_S6
	li		t1, MEM_STCFG1
	li		t2, MEM_STTIME1
#else
	lw		t1, MEM_STCFG1(a0)
	lw		t2, MEM_STTIME1(a0)
#endif
	li		t3, MEM_STADDR1
	sw		t1, mem_stcfg1(t0)
	sw		t2, mem_sttime1(t0)
	sw		t3, mem_staddr1(t0)

	/* RCE2 */
#ifndef USE_S6
	li		t1, MEM_STCFG2
	li		t2, MEM_STTIME2
#else
	lw		t1, MEM_STCFG2(a0)
	lw		t2, MEM_STTIME2(a0)
#endif
	li		t3, MEM_STADDR2
	sw		t1, mem_stcfg2(t0)
	sw		t2, mem_sttime2(t0)
	sw		t3, mem_staddr2(t0)

	/* RCE3 */
#ifndef USE_S6
	li		t1, MEM_STCFG3
	li		t2, MEM_STTIME3
#else
	lw		t1, MEM_STCFG3(a0)
	lw		t2, MEM_STTIME3(a0)
#endif
	li		t3, MEM_STADDR3
	sw		t1, mem_stcfg3(t0)
	sw		t2, mem_sttime3(t0)
	sw		t3, mem_staddr3(t0)

	/* Address latch */
#ifndef USE_S6
	li		t1, MEM_STALTIME
#else
	lw		t1, MEM_STALTIME(a0)
#endif
	sw		t1, mem_staltime(t0)
	sync

	/* NAND */
	sw		zero, mem_stndctrl(t0)
	sync

	/*
	 * Step 15) Set peripherals to a known state
	 */
	li		t0, AU1200_IC0_ADDR
	li		t1, 0xFFFFFFFF
	sw		t1, ic_cfg0clr(t0)
	sw		t1, ic_cfg1clr(t0)
	sw		t1, ic_cfg2clr(t0)
	sw		t1, ic_srcset(t0)
	sw		t1, ic_assignset(t0)
	sw		t1, ic_wakeclr(t0)
	sw		t1, ic_maskclr(t0)
	sw		t1, ic_risingclr(t0)
	sw		t1, ic_fallingclr(t0)
	sw		zero, ic_testbit(t0)
	sync

	li		t0, AU1200_IC1_ADDR
	li		t1, 0xFFFFFFFF
	sw		t1, ic_cfg0clr(t0)
	sw		t1, ic_cfg1clr(t0)
	sw		t1, ic_cfg2clr(t0)
	sw		t1, ic_srcset(t0)
	sw		t1, ic_assignset(t0)
	sw		t1, ic_wakeclr(t0)
	sw		t1, ic_maskclr(t0)
	sw		t1, ic_risingclr(t0)
	sw		t1, ic_fallingclr(t0)
	sw		zero, ic_testbit(t0)
	sync

	li		t0, AU1200_SYS_ADDR
	sw		zero, sys_freqctrl0(t0)
	sw		zero, sys_freqctrl1(t0)
	sw		zero, sys_clksrc(t0)
	sw		zero, sys_pininputen(t0)
	sync

	li		t0, AU1200_DDMA_ADDR
	sw		zero, ddma_inten(t0)

  	li		t0, AU1200_UART0_ADDR
	sw		zero, uart_enable(t0)

	li		t0, AU1200_UART1_ADDR
	sw		zero, uart_enable(t0)

	li		t0, AU1200_PSC0_ADDR
	sw		zero, psc_enable(t0)

	li		t0, AU1200_PSC1_ADDR
	sw		zero, psc_enable(t0)

	li		t0, AU1200_USB_ADDR
	li		t1, 0x00D02000
	sw		t1, usb_cfg(t0)

	li		t0, AU1200_LCD_ADDR
	sw		zero, lcd_screen(t0)

	li		t0, AU1200_SD0_ADDR
	sw		zero, sd_enable(t0)

	li		t0, AU1200_SD1_ADDR
	sw		zero, sd_enable(t0)

	li		t0, AU1200_SWC_ADDR
	sw		zero, swcnt_control(t0)

	li		t0, AU1200_AES_ADDR
	sw		zero, aes_status(t0)

	li		t0, AU1200_CIM_ADDR
	sw		zero, cim_enable(t0)
	sync


	/*
	 * Step 16) Determine cause of reset
	 */
	/* wait 10mS to debounce external signals */
	li		t1, MEM_1MS*10
1:	add		t1, -1
	bne		t1, zero, 1b
	nop

	li		t0, AU1200_SYS_ADDR
	lw		t1, sys_wakesrc(t0)

	/* Clear sys_wakemsk to prevent false events */
	sw		zero, sys_wakemsk(t0)
	sync

	/* Clear sys_wakesrc */
	//sw		zero, sys_wakesrc(t0)
	sync

	/* Check for Hibernate Reset first */
	andi	t2, t1, 0x04
	bne		zero, t2, hibernatereset
	nop

	/* Check for Hardware Reset */
	andi	t2, t1, 0x01
	bne		zero, t2, hardwarereset
	nop

	/* Check for Sleep Wakeup */
	andi	t2, t1, 0x02
	bne		zero, t2, sleepwakeup
	nop

	/* Assume run-time reset */
	beq		zero, zero, runtimereset
	nop

/************************************************************************/

hardwarereset:
runtimereset:
hibernatereset:
	/*
	 * Step 1) Initialize DRAM
	 * Step 2) Initialize board
	 * Step 3) Invoke application
	 */
	bal 	initDRAM
	nop 
   	bal		initBOARD
	nop
	beq		zero, zero, alldone
	nop	 

sleepwakeup:
	/*
	 * Step 1) Wakeup DRAM
	 * Step 2) Initialize board
	 * Step 3) Resume application
	 */
  	bal		wakeupDRAM	
  	nop
	bal		initBOARD
	nop
	la		t0, AU1200_SYS_ADDR
	lw		sp, sys_scratch0(t0)
	lw		ra, sys_scratch1(t0)
	jr		ra
	nop

/*************************************************************************/

initDRAM:

	/* Only perform DRAM init if running from ROM/Flash */
	addu	t2, ra, zero	/* preserve ra */
    b		getPC
    nop

getPC:
    lui		t0, 0x1F00      /* ROM/flash address? */
    and		t1, t0, ra
	addu	ra, t2, zero	/* restore ra */
    bne		t0, t1, initDDRdone
    nop

	/* wait 1mS before setup */
	li		t1, MEM_1MS
1:	add		t1, -1
	bne		t1, zero, 1b
	nop

initDDR2:
#ifndef USE_S6	   
	li		t1, MEM_SDCONFIGA_DDR2
	li		t2, MEM_SDCONFIGB_DDR2
	li		t3, MEM_SDMODE0_DDR2
	li		t4, MEM_SDMODE1_DDR2
	li		v0, MEM_MR0_DDR2
	li		v1, MEM_MR1_DDR2
#else
	lw		t1, MEM_SDCONFIGA(a0)
	lw		t2, MEM_SDCONFIGB(a0)
	lw		t3, MEM_SDMODE(a0)
	lw		t4, MEM_SDMODE(a0)
	lw		v0, MEM_MR0(a0)
	lw		v1, MEM_MR1(a0)
#endif
	li		t5, MEM_SDADDR0_DDR2
	li		t6, MEM_SDADDR1_DDR2
	li		a2, MEM_MR2_DDR2
	li		a3, MEM_MR3_DDR2

	li		t0, AU1200_MEM_ADDR
	or		t2, MEM_SDCONFIGB_BB /* block LCD/MAE during init */
	sw		t1, mem_sdconfiga(t0)
	sync
	sw		t2, mem_sdconfigb(t0)
	sync
	sw		t3, mem_sdmode0(t0)
	sw		t4, mem_sdmode1(t0)
	sw		t5, mem_sdaddr0(t0)
	sw		t6, mem_sdaddr1(t0)
	sync

	/*
	 * Initialization per Micron data sheet, page 72:
	 * 0. Power sequence
	 * 1. 200us delay
	 * 2. NOP w/ CKE
	 * 3. PRECHARGE ALL
	 * 4. LOAD MODE REGISTER for the extended mode register (normal, not reduced, drive strength)
	 * 5. LOAD MODE REGISTER for the normal mode register (DLL reset activated)
	 * 6. 200 clocks of idle
	 * 7. PRECHARGE ALL
	 * 8. AUTO REFRESH x 2
	 * 9. LOAD MODE REGISTER (DLL reset de-activated)
	 * Devices are now ready for use.
	 */

	/* NOP w/ CKE */
	lw		t1, mem_sdconfigb(t0)
	li		t2, MEM_SDCONFIGB_BA
	or		t1, t2, t1
	sw		t1, mem_sdconfigb(t0)
	sync

	/* PRECHARGE ALL */
	sw		zero, mem_sdprecmd(t0)
	sync

	/* LOAD MODE REGISTER extended mode register 3 */
	sw		a3, mem_sdwrmd0(t0)		
	sync
	sw		a3, mem_sdwrmd1(t0)		
	sync

	/* LOAD MODE REGISTER extended mode register 2 */
	sw		a2, mem_sdwrmd0(t0)		
	sync
	sw		a2, mem_sdwrmd1(t0)		
	sync

	/* LOAD MODE REGISTER extended mode register 1 */
	sw		v1, mem_sdwrmd0(t0)		
	sync
	sw		v1, mem_sdwrmd1(t0)		
	sync

	/* LOAD MODE REGISTER normal mode register DLL reset */
#define MR0_DLL 0x0100
	ori		t1, v0, MR0_DLL
	sw		t1, mem_sdwrmd0(t0)
	sync
	sw		t1, mem_sdwrmd1(t0)
	sync

	/* 200 clocks of idle */
	lw		t1, mem_sdconfigb(t0)
	li		t2, MEM_SDCONFIGB_BA
	or		t1, t2, t1
	sw		t1, mem_sdconfigb(t0)
	sync

	/* PRECHARGE ALL */
	sw		zero, mem_sdprecmd(t0)
	sync

	/* AUTO REFRESH x 2 */
	sw		zero, mem_sdautoref(t0)
	sync
	sw		zero, mem_sdautoref(t0)
	sync

	/* LOAD MODE REGISTER normal mode register */
	sw		v0, mem_sdwrmd0(t0)
	sync
	sw		v0, mem_sdwrmd1(t0)
	sync

initDDRdone:
	/* Enable refresh */
	li		t0, AU1200_MEM_ADDR
	lw		t1, mem_sdconfiga(t0)
	li		t2, MEM_SDCONFIGA_E
	or		t1, t2, t1
	sw		t1, mem_sdconfiga(t0)
	sync

	/* Allow MAE/LCD */
	lw		t1, mem_sdconfigb(t0)
	li		t2, ~MEM_SDCONFIGB_BB
	and		t1, t2, t1
	sw		t1, mem_sdconfigb(t0)
	sync

	/* wait 1mS after setup */
	li		t1, MEM_1MS
1:	add		t1, -1
	bne		t1, zero, 1b
	nop

	jr		ra
	nop

/********************************************************************/

wakeupDRAM:	

#ifndef USE_S6
	li		t1, MEM_SDCONFIGA_DDR2
	li		t2, MEM_SDCONFIGB_DDR2
	li		t3, MEM_SDMODE0_DDR2
	li		t4, MEM_SDMODE1_DDR2
#else
	lw		t1, MEM_SDCONFIGA(a0)
	lw		t2, MEM_SDCONFIGB(a0)
	lw		t3, MEM_SDMODE(a0)
	lw		t4, MEM_SDMODE(a0)
#endif
	li		t5, MEM_SDADDR0_DDR2
	li		t6, MEM_SDADDR1_DDR2

	li		t0, AU1200_MEM_ADDR
	sw		t1, mem_sdconfiga(t0)
	sync
	sw		t2, mem_sdconfigb(t0)
	sync
	sw		t3, mem_sdmode0(t0)
	sw		t4, mem_sdmode1(t0)
	sw		t5, mem_sdaddr0(t0)
	sw		t6, mem_sdaddr1(t0)
	sync

	/* Assert DCKE - bring DDR out of self refresh */
	/* Note that two mem_sdsref are needed since state lost during sleep */
	sw		zero, mem_sdsref(t0)
	sw		zero, mem_sdsref(t0)

	/*
	 * Issue 80ns of NOPs
	 */ 
	lw		t1, mem_sdconfigb(t0)
	li		t2, MEM_SDCONFIGB_BA
	or		t1, t2, t1
 	sw		t1, mem_sdconfigb(t0)
	sync 

	/*
	 * Perform burst refresh of 8K rows
	 */
	li		t1, 8192
burstrefresh:
	sw		zero, mem_sdautoref(t0)
	bne		zero, t1, burstrefresh
	addi	t1, t1, -1

	/* Enable refresh */
	lw		t1, mem_sdconfiga(t0)
	li		t2, MEM_SDCONFIGA_E
	or		t1, t2, t1
	sw		t1, mem_sdconfiga(t0)
	sync

 	jr	ra
	nop

/********************************************************************/

initBOARD:

	/*
	 * External and/or board-specific peripheral initialization
	 */

	/*
	 * Adjust 16-bit chip-selects if EB (RCE0 handled specially in yamon/init/reset/reset.S)
	 */
	mfc0 t0, CP0_Config
	andi t0, 0x8000
	beq zero, t0, 1f
	nop
	li		t2, 0x0200		/* mem_stcfg[BE]=1 */
	li		t0, AU1200_MEM_ADDR
	lw		t1, mem_stcfg1(t0)
	or		t1, t1, t2
	sw		t1, mem_stcfg1(t0)
	lw		t1, mem_stcfg2(t0)
	or		t1, t1, t2
	sw		t1, mem_stcfg2(t0)
	sync
1:

	/*
	 * Establish MUXed pin functionality
	 *
	 * 31:DMA=1  DMA_REQ1
	 * 30:S0A=1  SD0
	 * 29:S1A=0  PCMCIA
	 * 28:LP0=1  LCD_PWM0
	 * 27:LP1=1  LCD_PWM1
	 * 26:LD=1   LCD_D16
	 * 25:LD8=1  LCD_D8
	 * 24:LD1=1  LCD_D1
	 * 23:LD0=1  LCD_D0
	 * 21:P1A=00 PSC1
	 * 20:P1B=0  PSC1
	 * 19:FS3=1  FREQ3
	 * 17:P0A=01 PSC0
	 * 16:CS=0   EXTCLK0
	 * 15:CIM=1  CIM
	 * 14:P1C=1  PSC1
	 * 13:Reserved=1
	 * 12:U1T=0  UART1
	 * 11:U1R=1  UART1
	 * 10:EX1=0  GPIO3
	 *  9:EX0=1  EXTCLK0
	 *  8:U0R=1  UART0
	 *  7:MC=1   UART1
	 *  6:S0B=1  SD0
	 *  5:S0C=0  SD0
	 *  4:P0B=0  PSC0
	 *  3:U0T=0  U0TXD
	 *  2:S1B=1  PCMCIA
	 *  1:Reserved=0
	 *  0:Reserved=0
	 */
	li		t0, AU1200_SYS_ADDR
	li		t1, 0xDF8AEBC4
	sw		t1, sys_pinfunc(t0)

	/*
	 * Establish GPIO direction
	 *
	 * GPIO0 Input for CIM_D0
	 * GPIO1 Input for CIM_D1
	 * GPIO2 EXTCLK0 - I2S_MCLK
	 * GPIO3 Output LCD_CLK_IN_LED#
	 * GPIO4 Input DC DMA_REQ
	 * GPIO5 Input GPIO5_Switch
	 * GPIO6 SD_Card0_DAT2
	 * GPIO7 Input Board_IRQ#
	 * GPIO8 SD_CARD0_DAT1
	 * GPIO9  U1_CTS
	 * GPIO10 U1_DSR
	 * GPIO11 PSC1_D0
	 * GPIO12 DMA_REQ1 for IDE
	 * GPIO13 U1_RTS
	 * GPIO14 U1_DTR
	 * GPIO15 U1_TXD
	 * GPIO16 PSC0_SYNC1
	 * GPIO17 SD_Card0_CLK
	 * GPIO18 PSC0_D0
	 * GPIO19 SD_Card0_DAT0
	 * GPIO20 PSC1_SYNC0
	 * GPIO21 PSC1_SYNC1
	 * GPIO22 PSC1_D1
	 * GPIO23 I2S_MCLK_IN 
	 * GPIO24 PSC1_CLK
	 * GPIO25 PSC0_CLK
	 * GPIO26 SD_Card0_DAT3
	 * GPIO27 U0_TXD
	 * GPIO28 SD_Card0_CMD
	 * GPIO29 U0_RXD
	 * GPIO30 U1_RXD
	 * GPIO31 PSC0_D1
	 */
	li		t1, 0x000000B3
	sw		t1, sys_trioutclr(t0)		/* inputs enabled */
	li		t1, 0x00000008				/* turn on green led D3 */
	sw		t1, sys_outputclr(t0)
	sync

	/*
	 * Establish GPIO2 direction
	 *
	 * GPIO200 LCD_D0
	 * GPIO201 LCD_D1
	 * GPIO202 CIM_D2
	 * GPIO203 CIM_D3
	 * GPIO204 CIM_D4
	 * GPIO205 CIM_D5
	 * GPIO206 CIM_D6
	 * GPIO207 CIM_D7
	 * GPIO208 CIM_D8
	 * GPIO209 CIM_D9
	 * GPIO210 LCD_D8
	 * GPIO211 LCD_D16
	 * GPIO212 CIM_LS
	 * GPIO213 CIM_FS
	 * GPIO214 CIM_CLK
	 * GPIO215 Output NAND_FORCE_CE# and OTG_VBUS_ON#
	 */
	li		t0, AU1200_GPIO2_ADDR
	li		t1,3
	sw		t1,gpio2_enable(t0)
	sync
	li		t1,1
	sw		t1,gpio2_enable(t0)
	sync
	sw		zero,gpio2_inten(t0)
	sync
	li		t1,(1<<15)
	sw		t1,gpio2_dir(t0)
	sync
	li		t1,(1<<31)|(1<<15)		/* do not assert GPIO215 */
	sw		t1,gpio2_output(t0)
	sync

	/*
	 * Establish CLOCKing
	 *
	 * FREQ5: unused
	 * FREQ4: unused
	 * FREQ3: unused
	 * FREQ2: unused
	 * FREQ1: unused - 24MHz for PSC0 (SMBus and SPI)
	 * FREQ0: unused
	 */
	li		t0, AU1200_SYS_ADDR
	li		t1, (1<<12)|(1<<11)|(1<<10)
	sw		t1, sys_freqctrl0(t0)
	li		t1, (3<<22)|(1<<21)|(1<<20)
	sw		t1, sys_clksrc(t0)
	sync

	/* Take IDE/LAN/DC/CIM out of reset */
	li		t0, DB1200_BCSR_ADDR
	lh		t1, bcsr_resets(t0)
	ori		t1, 0x000F
	sh		t1, bcsr_resets(t0)
	sync

	/* Ensure PCMCIA interface disabled */
	li		t0, DB1200_BCSR_ADDR
	sh		zero, bcsr_pcmcia(t0)
	sync

	/* Ensure SD, CIM, LCD powered-off */
	li		t0, DB1200_BCSR_ADDR
	li		t1, 0x0020
	sh		t1, bcsr_board(t0)
	sync

	/* Ensure Board Interrupt Controller is disabled */
	li		t0, DB1200_BCSR_ADDR
	li		t1, 0xFFFF
	sh		t1, bcsr_icer(t0)
	sh		t1, bcsr_icmr(t0)
	sync

	jr		ra
	nop

/********************************************************************/

alldone:

	/*
	 * Prepare to invoke application main()
	 */
//	 .set reorder

/********************************************************************/

