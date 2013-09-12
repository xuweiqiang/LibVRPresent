/*
 * Cartesio (STA206x) Architecture and Board Support Package.
 *
 * Smart Graphics Accelerator driver for Cartesio Plus SoC.
 *
 * Copyright (C) 2009-2010 STMicroelectronics
 *
 * Written by: Giuseppe Gorgoglione (giuseppe.gorgoglione@st.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MACH_SGA_H
#define __MACH_SGA_H

#include <linux/ioctl.h>
#include <linux/types.h>

#define SGA_OP(x)			U32_C((x) << 24)
#define SGA_ADDRESS(x)		U32_C((x) >>  3)

/*
 * Smart Graphics Accelerator opcodes
 */
#define SGA_OP_STOP					SGA_OP(0x00)
#define SGA_OP_GOTO					SGA_OP(0x20)
#define SGA_OP_GOSUB   				SGA_OP(0x40)
#define SGA_OP_NO_OP				SGA_OP(0x60)
#define SGA_OP_RETURN				SGA_OP(0x61)
#define SGA_OP_WAIT_SYNCHRO			SGA_OP(0x80)
#define SGA_OP_WAIT_NEW_SYNCHRO		SGA_OP(0x81)
#define SGA_OP_WAIT_PIPE_EMPTY		SGA_OP(0x82)
#define SGA_OP_WAIT_N_CYCLES		SGA_OP(0x83)
#define SGA_OP_SEND_SYNCHRO			SGA_OP(0x84)
#define SGA_OP_SEND_INTERRUPT		SGA_OP(0x85)
#define SGA_OP_AHB 					SGA_OP(0x86)
#define SGA_OP_CACHE_CTRL 		 	SGA_OP(0x87)
#define SGA_OP_SET_INSTR_TEST_REG	SGA_OP(0x88)
#define SGA_OP_CLR_INSTR_TEST_REG	SGA_OP(0x89)
#define SGA_OP_TST_INSTR_TEST_REG	SGA_OP(0x8A)
#define SGA_OP_WAIT_INSTR_TEST_REG	SGA_OP(0x8B)
#define SGA_OP_IN0_BASE_ADD_MSB		SGA_OP(0xA0)
#define SGA_OP_IN0_BASE_ADD			SGA_OP(0xA1)
#define SGA_OP_IN0_SET_LINE_JUMP	SGA_OP(0xA2)
#define SGA_OP_IN0_SET_SIZE_XY		SGA_OP(0xA3)
#define SGA_OP_IN0_SET_DELTA_XY		SGA_OP(0xA4)
#define SGA_OP_IN0_SET_PIXEL_TYPE	SGA_OP(0xA5)
#define SGA_OP_IN1_BASE_ADD_MSB		SGA_OP(0xA8)
#define SGA_OP_IN1_BASE_ADD			SGA_OP(0xA9)
#define SGA_OP_IN1_SET_LINE_JUMP	SGA_OP(0xAA)
#define SGA_OP_IN1_SET_SIZE_XY		SGA_OP(0xAB)
#define SGA_OP_IN1_SET_DELTA_XY		SGA_OP(0xAC)
#define SGA_OP_IN1_SET_PIXEL_TYPE	SGA_OP(0xAD)
#define SGA_OP_IN2_BASE_ADD_MSB		SGA_OP(0xB0)
#define SGA_OP_IN2_BASE_ADD			SGA_OP(0xB1)
#define SGA_OP_IN2_SET_LINE_JUMP	SGA_OP(0xB2)
#define SGA_OP_IN2_SET_SIZE_XY		SGA_OP(0xB3)
#define SGA_OP_IN2_SET_DELTA_XY		SGA_OP(0xB4)
#define SGA_OP_IN2_SET_PIXEL_TYPE	SGA_OP(0xB5)
#define SGA_OP_OUT_BASE_ADD_MSB		SGA_OP(0xB8)
#define SGA_OP_OUT_BASE_ADD			SGA_OP(0xB9)
#define SGA_OP_OUT_SET_LINE_JUMP	SGA_OP(0xBA)
#define SGA_OP_OUT_SET_SIZE_XY		SGA_OP(0xBB)
#define SGA_OP_OUT_SET_BASE_XY		SGA_OP(0xBC)
#define SGA_OP_OUT_SET_PIXEL_TYPE	SGA_OP(0xBD)
#define SGA_OP_SET_POINT0			SGA_OP(0xC0)
#define SGA_OP_SET_POINT1			SGA_OP(0xC1)
#define SGA_OP_SET_POINT2			SGA_OP(0xC2)
#define SGA_OP_SET_COLOR			SGA_OP(0xC3)
#define SGA_OP_SET_BYPASS_ZS		SGA_OP(0xC4)
#define SGA_OP_LINE_STIPPLING		SGA_OP(0xC5)
#define SGA_OP_DRAW_RECTANGLE		SGA_OP(0xC6)
#define SGA_OP_DRAW_TRIANGLE		SGA_OP(0xC7)
#define SGA_OP_DRAW_LINE			SGA_OP(0xC8)
#define SGA_OP_DRAW_POINT			SGA_OP(0xC9)
#define SGA_OP_DRAW_TRIANGLE_SHIFT	SGA_OP(0xCA)
#define SGA_OP_DRAW_LINE_SHIFT		SGA_OP(0xCB)
#define SGA_OP_SET_ZX_COEF			SGA_OP(0xCC)
#define SGA_OP_SET_ZY_COEF			SGA_OP(0xCD)
#define SGA_OP_SET_Z_OFFSET			SGA_OP(0xCE)
#define SGA_OP_SET_Z_DYN			SGA_OP(0xCF)
#define SGA_OP_SET_XX_COEF			SGA_OP(0xD0)
#define SGA_OP_SET_XY_COEF			SGA_OP(0xD1)
#define SGA_OP_SET_YX_COEF			SGA_OP(0xD2)
#define SGA_OP_SET_YY_COEF			SGA_OP(0xD3)
#define SGA_OP_SET_WX_COEF			SGA_OP(0xD4)
#define SGA_OP_SET_WY_COEF			SGA_OP(0xD5)
#define SGA_OP_SET_X_OFFSET			SGA_OP(0xD6)
#define SGA_OP_SET_Y_OFFSET			SGA_OP(0xD7)
#define SGA_OP_SET_W_OFFSET			SGA_OP(0xD8)
#define SGA_OP_SET_XY_MODE			SGA_OP(0xD9)
#define SGA_OP_SET_XY_DYN			SGA_OP(0xDA)
#define SGA_OP_TRANSP_COLORMSB		SGA_OP(0xE0)
#define SGA_OP_TRANSP_IN_COLOR		SGA_OP(0xE1)
#define SGA_OP_TRANSP_OUT_COLOR		SGA_OP(0xE2)
#define SGA_OP_TRANSP_MODE			SGA_OP(0xE3)
#define SGA_OP_FLASH_COLOR_MSB		SGA_OP(0xE4)
#define SGA_OP_FLASH_COLOR_ID		SGA_OP(0xE5)
#define SGA_OP_FLASH_COLOR_NEW		SGA_OP(0xE6)
#define SGA_OP_FLASH_MODE			SGA_OP(0xE7)
#define SGA_OP_SET_COEF_AXAY		SGA_OP(0xE8)
#define SGA_OP_SET_COEF_A0			SGA_OP(0xE9)
#define SGA_OP_SET_COEF_RXRY		SGA_OP(0xEA)
#define SGA_OP_SET_COEF_R0			SGA_OP(0xEB)
#define SGA_OP_SET_COEF_GXGY		SGA_OP(0xEC)
#define SGA_OP_SET_COEF_G0			SGA_OP(0xED)
#define SGA_OP_SET_COEF_BXBY		SGA_OP(0xEE)
#define SGA_OP_SET_COEF_B0			SGA_OP(0xEF)
#define SGA_OP_SET_COEF_DYN			SGA_OP(0xF0)
#define SGA_OP_SET_TEX_COLORMSB		SGA_OP(0xF1)
#define SGA_OP_SET_TEX_COLORLSB		SGA_OP(0xF2)
#define SGA_OP_SET_TEX_ENV_MSB		SGA_OP(0xF3)
#define SGA_OP_SET_TEX_ENV_LSB		SGA_OP(0xF4)
#define SGA_OP_SET_TEX_SCALE		SGA_OP(0xF5)
#define SGA_OP_SET_COEF_FXFY		SGA_OP(0xF6)
#define SGA_OP_SET_COEF_F0			SGA_OP(0xF7)
#define SGA_OP_SET_COLOR_F0			SGA_OP(0xF8)
#define SGA_OP_SET_BLEND_COLORMSB	SGA_OP(0xF9)
#define SGA_OP_SET_BLEND_ENV		SGA_OP(0xFA)
#define SGA_OP_PIXEL_OP_MODE		SGA_OP(0xFB)
#define SGA_OP_SET_ALPHA_TEST		SGA_OP(0xFC)
#define SGA_OP_SET_STENCIL_TEST		SGA_OP(0xFD)
#define SGA_OP_SET_DEPTH_TEST		SGA_OP(0xFE)

/*
 * Parameters for IOCTL commands
 */
struct sga_statistics {
	__u32 triangle_requests;
	__u32 fragment_raw_requests;
	__u32 fragment_zpassed_requests;
	__u32 texture_cache_requests;
	__u32 framebuffer_cache_refills;
	__u32 texture_cache_refills;
	__u32 instruction_refills;
	__u32 elapsed_clock_cycles;
};

struct sga_surface {
	__u32 handle;
	__u32 size;		/* bytes */
};

struct sga_batch {
	__u32 handle;
	__u32 size;		/* bytes */
};

/*
 * This is free according to Documentation/ioctl/ioctl-number.txt
 */
#define	SGA_IOCTL_BASE			0xEE

/*
 * These IOCTL commands should be actually defined in a user side exported
 * kernel include file, but since they are meant to be private and used by
 * proprietary graphics software only, they are defined here.
 */
#define	SGA_IOCTL_ALLOC_SURFACE	_IOWR(SGA_IOCTL_BASE, 0, struct sga_surface)
#define	SGA_IOCTL_FREE_SURFACE	_IOW(SGA_IOCTL_BASE, 1, struct sga_surface)
#define	SGA_IOCTL_ALLOC_BATCH	_IOWR(SGA_IOCTL_BASE, 2, struct sga_batch)
#define	SGA_IOCTL_FREE_BATCH	_IOW(SGA_IOCTL_BASE, 3, struct sga_batch)
#define	SGA_IOCTL_START_BATCH	_IOW(SGA_IOCTL_BASE, 4, struct sga_batch)
#define	SGA_IOCTL_WAIT_BATCH	_IOW(SGA_IOCTL_BASE, 5, struct sga_batch)
#define	SGA_IOCTL_GET_STATS	_IOR(SGA_IOCTL_BASE, 6, struct sga_statistics)
#define	SGA_IOCTL_GET_VRAM_SIZE	_IOR(SGA_IOCTL_BASE, 7, unsigned int)

#endif /* __MACH_SGA_H */
