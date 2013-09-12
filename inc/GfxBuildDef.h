//#define BUILD_ALL

#ifdef _MSC_VER
#pragma warning(disable:4995)
#pragma warning(disable:4996)
#pragma warning(disable: 4018)
#pragma warning (disable: 4100)     // 4100: unreferenced formal parameter
#pragma warning(disable: 4706)
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE
#endif

#if defined(RTK_TITAN_II) || defined(FY_TITAN_II) 
#define TITAN_II
#endif

#ifdef MSTAR2531_Num5
#define MSTAR2531 
#endif

#ifdef MSTAR2531
#define GFX_MSTAR
#endif

#ifdef MSTAR2521_JTD
#define GFX_MSTAR
#endif

#ifdef SAMSUNG_2451_DDNAV
#define SAMSUNG_2450_LEADER_INTL
#endif

#if defined(SAMSUNG_2416) || defined(SAMSUNG_2450) || defined(SAMSUNG_2450_LEADER_INTL)
#define SAMSUNG
#endif

#ifdef  SIRF_A4_PVEGPS
#define SIRF_A4_NEWOS
#endif

#ifdef  GFX_FB_LINUX
#define GFX_FB
#endif

#define LOADLIBRARY_ONCE

#if defined(MTK3353_SW) || defined(MTK3353_HW)
#define MT3353
#endif

#ifdef MT3351_YECON
#define MT3351_DBG
#endif

#ifdef MT3351_RGB
#define MT3351_DBG
#endif


#if defined(HUAYANG_A6)
#define SIRF_A6
#endif

#if defined(SIRF_A6) || defined(SIRF_A5)
#define SIRF_A4_NEWOS
#endif


#if defined(PRIMAII_SPD) || defined(PRIMAII_SAVECOPY) ||defined(PRIMAII_XUNZU) 
#define PRIMAII
#endif

//MT3353 USES ddraw_osd
#if defined(FREESCALE_IMX31) || defined(MT3351_DBG) || defined(MT3353) \
	|| defined(TITAN_II) ||	defined(APOLLO_SAMSUNG2450)  \
	|| defined(SIRF_A4_NEWOS) || defined(SIRF_A4_OLDOS)	\
	|| defined(SAMSUNG6443) || defined(NAVI) || defined(VNCVIEWER)	\
	|| defined(SAMSUNG_PC100) || defined(ST_SAVECOPY) \
	|| defined(OMAP3530) || defined(TI3630) || defined(PRIMAII)||defined(PRIMAII_SAVECOPY)	\
	|| defined(FREESCALE_IMX51) || defined(IEC)
#ifndef GFX_DDRAW
#define GFX_DDRAW
#define LOADLIBRARY_ONCE
#endif
#endif


#if defined(MT3351_DBG) || defined(MT3353) ||defined(PRIMAII)|| defined(SIRF_A4_NEWOS) \
	|| defined(PRIMAII_SAVECOPY)
#define GFX_DDRAW_OSD
#endif

#ifdef BUILD_ALL
	#ifdef WIN32
	#define GFX_SDL
	#define GFX_D3D
	#endif

	#ifdef _WIN32_WCE
	#define TITAN
	#define TITAN_II
	#define RMI
	#endif
#define GFX_DDRAW
#define PXA
#define GFX_GDI
#define GFX_GX
#endif

