
/*
 *    File: vcodec/alchemy/maeioctl.h
 *  Author: Eric DeVolder
 * Purpose: MAE hardware ioctls
 *   Notes:
 *
 * Each OS has its own MAE driver(s) and instance of this file.
 * Builds must use the corresponding OS version of this file.
 * This file is used primarily when building for NO_MAE_HW.
 *
 */

/*
 * Common header file for MAE
 */

#ifndef _MAEIOCTL_H
#define _MAEIOCTL_H

// Interface for talking to Linux drivers
// NOTE: Linux drivers directly include this file...

#ifndef ENDIAN_STR

#ifdef MIPSEB
#define ENDIAN_STR "EB"

#else
#define ENDIAN_STR "EL"
#endif

#endif

//////////////////////////////////////////////////////////////////////
typedef struct
{
    uint32 magic;
#define MAEMEM_MAGIC ((0x0124 << 16) | (sizeof(mae_mem_request_t)))
    uint32 instance;

    uint32 physAddr;
    uint32 physSize;
    uint32 YUVphysAddr;
    uint32 YUVphysSize;
    uint32 flags;
#define MAEMEM_FLAGS_CACHEABLE 0x1
    uint32 counterFreq;

} mae_mem_request_t;

//////////////////////////////////////////////////////////////////////

// lifted from drivers/video/au1200fb.c
#define AU1200_LCD_FB_IOCTL 0x46FF

#define AU1200_LCD_SET_SCREEN 1
#define AU1200_LCD_GET_SCREEN 2
#define AU1200_LCD_SET_WINDOW 3
#define AU1200_LCD_GET_WINDOW 4
#define AU1200_LCD_SET_PANEL  5
#define AU1200_LCD_GET_PANEL  6

#define SCREEN_SIZE		    (1<< 1)
#define SCREEN_BACKCOLOR    (1<< 2)
#define SCREEN_BRIGHTNESS   (1<< 3)
#define SCREEN_COLORKEY     (1<< 4)
#define SCREEN_MASK         (1<< 5)
typedef struct au1200_lcd_global_regs_t
{
    unsigned int flags;
    unsigned int xsize;
    unsigned int ysize;
    unsigned int backcolor;
    unsigned int brightness;
	unsigned int colorkey;
	unsigned int mask;
    unsigned int panel_choice;
    char panel_desc[80];

} au1200_lcd_global_regs_t;

#define WIN_POSITION            (1<< 0)
#define WIN_ALPHA_COLOR         (1<< 1)
#define WIN_ALPHA_MODE          (1<< 2)
#define WIN_PRIORITY            (1<< 3)
#define WIN_CHANNEL             (1<< 4)
#define WIN_BUFFER_FORMAT       (1<< 5)
#define WIN_COLOR_ORDER         (1<< 6)
#define WIN_PIXEL_ORDER         (1<< 7)
#define WIN_SIZE                (1<< 8)
#define WIN_COLORKEY_MODE       (1<< 9)
#define WIN_DOUBLE_BUFFER_MODE  (1<< 10)
#define WIN_RAM_ARRAY_MODE      (1<< 11)
#define WIN_BUFFER_SCALE        (1<< 12)
#define WIN_ENABLE	            (1<< 13)

typedef struct au1200_lcd_window_regs_t
{
    unsigned int flags;
    unsigned int xpos;
    unsigned int ypos;
    unsigned int alpha_color;
    unsigned int alpha_mode;
    unsigned int priority;
    unsigned int channel;
    unsigned int buffer_format;
    unsigned int color_order;
    unsigned int pixel_order;
    unsigned int xsize;
    unsigned int ysize;
    unsigned int colorkey_mode;
    unsigned int double_buffer_mode;
    unsigned int ram_array_mode;
    unsigned int xscale;
    unsigned int yscale;
    unsigned int enable;
} au1200_lcd_window_regs_t;

typedef struct au1200_lcd_iodata_t
{

    unsigned int subcmd;
    au1200_lcd_global_regs_t global;
    au1200_lcd_window_regs_t window;

} au1200_lcd_iodata_t;

typedef struct
{
    int request;
#define MAEHAL_LCD_GET_MEMORY       1
#define MAEHAL_LCD_UPDATE_WINDOW    2

    union
    {
        struct
        {
            int cached;
            uint32 physAddr;
            uint32 physSize;

            uint32 lcdwin0phys;
            uint32 lcdwin1phys;
            uint32 lcdwin2phys;
            uint32 lcdwin3phys;
        } memory;

        struct
        {
            int window;
            int buffer;
            uint32 physAddr;
        } update;
    } ;

} mae_lcd_request_t;

//////////////////////////////////////////////////////////////////////

typedef struct mae_fe_request_t
{
    uint32  magic;
#define MAEFE_MAGIC ((0x0203 << 16) | (sizeof(mae_fe_request_t)))

    uint32  flags;
#define MAEFE_FLAGS_DCWB        0x1
#define MAEFE_FLAGS_DCWBINV     0x2

    uint32  config;
    uint32  cury;
    uint32  frefy;
    uint32  brefy;
    uint32  curcb;
    uint32  frefcb;
    uint32  brefcb;
    uint32  curcr;
    uint32  frefcr;
    uint32  brefcr;
    uint32  pictsize;
    uint32  intenscomp;
    uint32  frefboty;
    uint32  frefbotcb;
    uint32  frefbotcr;
    uint32  brefboty;
    uint32  brefbotcb;
    uint32  brefbotcr;
    uint32  intstat;
    uint32  intenable;
    uint32  scratchpad;
    uint32  wmv9pquant;
    uint32  dmadscr;
    uint32  dmadbell;

    // returned by driver
    uint32 begMipsCounter;
    uint32 endMipsCounter;

} mae_fe_request_t;


//////////////////////////////////////////////////////////////////////
typedef struct mae_bsa_request_t
{
    uint32 magic;
#define MAEBSA_MAGIC ((0x0321 << 16) | (sizeof(mae_bsa_request_t)))

    uint32 flags;
#define MAEBSA_FLAGS_DCWB       0x1
#define MAEBSA_FLAGS_DCWBINV    0x2

    uint32 watchdog;
    uint32 initPC;
    uint32 irqMask;
    uint32 mailbox1;
    uint32 mailbox2;
    uint32 mailbox3;
    uint32 gpdmaControl;
    uint32 iramPhysAddr;
    uint32 iramSize;
    uint32 dramPhysAddr;
    uint32 dramSize;
    uint32 neighPhysAddr;
    uint32 neighSize;
    uint32 ofifo0StartPhysAddr;
    uint32 ofifo0EndPhysAddr;
    uint32 ofifo0Control;
    uint32 ofifo1StartPhysAddr;
    uint32 ofifo1EndPhysAddr;
    uint32 ofifo1Control;
    uint32 pmmPhysAddr;
    uint32 descPhysAddr;
    uint32 numEntries;
    uint32 ififoControl;
    uint32 ififoPattern;

    // returned by driver
    uint32 bsaStatus;
    uint32 endOfifo0PhysAddr;
    uint32 endOfifo1PhysAddr;
    uint32 endMailbox0;
    uint32 endMailbox1;
    uint32 endMailbox2;
    uint32 endMailbox3;
    uint32 endBitcounter;
    uint32 begMipsCounter;
    uint32 endMipsCounter;

} mae_bsa_request_t;

//////////////////////////////////////////////////////////////////////
typedef struct mae_mpe_request_t
{
    uint32 magic;
#define MAEMPE_MAGIC ((0x0405 << 16) | (sizeof(mae_mpe_request_t)))

    uint32 flags;
#define MAEMPE_FLAGS_DCWB       0x01
#define MAEMPE_FLAGS_DCWBINV    0x02

    uint32 cfg1;
    uint32 cfg2;
    uint32 cfg3;
    uint32 cfg4;
    uint32 cfg5;
    uint32 cfg6;
    uint32 cfg7;
    uint32 cfg8;
    uint32 cfg9;
    uint32 cfg10;
    uint32 cfg11;
    uint32 cfg12;
    uint32 cfg13;
    uint32 cfg14;
    uint32 cfg15;
    uint32 cfg18;

    // returned by driver
    uint32 begMipsCounter;
    uint32 endMipsCounter;

} mae_mpe_request_t;

//////////////////////////////////////////////////////////////////////
typedef struct mae_be_request_t
{
    uint32 magic;
#define MAEBE_MAGIC ((0x0509 << 16) | (sizeof(mae_be_request_t)))

    uint32 flags;
#define MAEBE_FLAGS_DCWB        0x01
#define MAEBE_FLAGS_DCWBINV     0x02
#define MAEBE_FLAGS_CACHEABLE   0x04

    uint32  scfhsr;
    uint32  scfvsr;
    uint32  scfdisable;
    uint32  scfhalut[32];
    uint32  scfvalut[32];
    uint32  scfhblut[32];
    uint32  scfvblut[32];
    uint32  scfhclut[32];
    uint32  scfvclut[32];

    uint32  cscxcffa;
    uint32  cscxcffb;
    uint32  cscxcffc;
    uint32  cscycffa;
    uint32  cscycffb;
    uint32  cscycffc;
    uint32  csczcffa;
    uint32  csczcffb;
    uint32  csczcffc;
    uint32  cscxoff;
    uint32  cscyoff;
    uint32  csczoff;
    uint32  cscalpha;

    uint32  srccfg;
    uint32  srcfhw;
    uint32  srcaaddr;
    uint32  srcastr;
    uint32  srcbaddr;
    uint32  srcbstr;
    uint32  srccaddr;
    uint32  srccstr;

    uint32  dstcfg;
    uint32  dstheight;
    uint32  dstaddr;
    uint32  dststr;

    uint32  ctlenable;
    uint32  ctlfpc;
    uint32  ctlstat;
    uint32  ctlintenable;
    uint32  ctlintstat;

    // returned by driver
    uint32 memPhysAddr;
    uint32 memPhysSize;
    uint32 counterFreq;
    uint32 begMipsCounter;
    uint32 endMipsCounter;

} mae_be_request_t;


#ifdef WINCE
	#define maeIOCTLCode(f,m)     \
	  CTL_CODE ( FILE_DEVICE_VIDEO, (f), (m), FILE_ANY_ACCESS )

	#define	MAE_MAGIC(n)			(n|0x800)

	#define MAEFE_IOCTL_SUBMIT_TRANSACTION				maeIOCTLCode(MAE_MAGIC(1), METHOD_BUFFERED)
	#define MAEBE_IOCTL_SUBMIT_TRANSACTION				maeIOCTLCode(MAE_MAGIC(2), METHOD_BUFFERED)
	#define MAEBSA_IOCTL_SUBMIT_TRANSACTION				maeIOCTLCode(MAE_MAGIC(3), METHOD_BUFFERED)
	#define MAEMPE_IOCTL_SUBMIT_TRANSACTION				maeIOCTLCode(MAE_MAGIC(4), METHOD_BUFFERED)
#else
	#define MAEFE_IOCTL_SUBMIT_TRANSACTION 1
	#define MAEBE_IOCTL_SUBMIT_TRANSACTION 2
	#define MAEBSA_IOCTL_SUBMIT_TRANSACTION 1
	#define MAEMPE_IOCTL_SUBMIT_TRANSACTION 2
#endif
//////////////////////////////////////////////////////////////////////

#endif // _MAEIOCTL_H

