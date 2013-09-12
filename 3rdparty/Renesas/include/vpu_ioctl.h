/* VPU IO control code for VPU driver       */
/* vpu_ioctl.h                              */
/* 2007.01.24                               */

#define IOCTL_VPU_SDRAM_WRITE			0x13d1
#define IOCTL_VPU_SDRAM_READ			0x13d2
#define IOCTL_VPU_SLEEP				0x13d3

#define IOCTL_VPU_INFO_MEMPTR			0x13d4
#define IOCTL_VPU_INFO_REGBASE			0x13d5
#define IOCTL_VPU_INFO_REGSIZE			0x13d6
#define IOCTL_VPU_INFO_MEMBASE			0x13d7
#define IOCTL_VPU_INFO_MEMSIZE			0x13d8
#define IOCTL_VPU_INFO_SYSINTR			0x13d9

#define IOCTL_VPU_TIMEOUT			0x13da
#define IOCTL_VPU_INFO_PRIORITY			0x13db
#define IOCTL_VPU_MEM_ALLOC			0x13dc
#define IOCTL_VPU_MEM_FREE			0x13de
#define IOCTL_VPC_CACHE_ENABLE			0x13e1
#define IOCTL_VPC_CACHE_DISABLE			0x13e2
#define IOCTL_VPC_CACHE_CLEAR			0x13e3