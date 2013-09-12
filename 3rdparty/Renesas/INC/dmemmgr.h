//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*==========================================================================;
 *
 *  File:	dmemmgr.h
 *  Content:	Direct Memory Manager include file
 *
 ***************************************************************************/

#ifndef __DMEMMGR_INCLUDED__
#define __DMEMMGR_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __NTDDKCOMP__
    
/*
 * pointer to video memory
 */
//typedef unsigned long FLATPTR;
typedef ULONG_PTR FLATPTR;

typedef struct _VIDMEM FAR *LPVIDMEM;

#else

/*
 * pointer to video memory, potentially 64-bit
 */
typedef ULONG_PTR FLATPTR;

typedef struct _VIDEOMEMORY *LPVIDMEM;

#endif

/*
 * Structure for querying extended heap alignment requirements
 */

typedef struct _SURFACEALIGNMENT
{
    union
    {
        struct
        {
            DWORD       dwStartAlignment;
            DWORD       dwPitchAlignment;
            DWORD       dwReserved1;
            DWORD       dwReserved2;
        } Linear;
        struct
        {
            DWORD       dwXAlignment;
            DWORD       dwYAlignment;
            DWORD       dwReserved1;
            DWORD       dwReserved2;
        } Rectangular;
    };
} SURFACEALIGNMENT;
typedef struct _SURFACEALIGNMENT FAR *LPSURFACEALIGNMENT;

typedef struct _HEAPALIGNMENT
{
    DWORD                dwSize;
    DDSCAPS              ddsCaps;       /* Indicates which alignment fields are valid.*/
    DWORD                dwReserved;
    SURFACEALIGNMENT     ExecuteBuffer; /* Surfaces tagged with DDSCAPS_EXECUTEBUFFER */
    SURFACEALIGNMENT     Overlay;       /* Surfaces tagged with DDSCAPS_OVERLAY       */
    SURFACEALIGNMENT     Texture;       /* Surfaces tagged with DDSCAPS_TEXTURE       */
    SURFACEALIGNMENT     ZBuffer;       /* Surfaces tagged with DDSCAPS_ZBUFFER       */
    SURFACEALIGNMENT     AlphaBuffer;   /* Surfaces tagged with DDSCAPS_ALPHA         */
    SURFACEALIGNMENT     Offscreen;     /* Surfaces tagged with DDSCAPS_OFFSCREENPLAIN*/
    SURFACEALIGNMENT     FlipTarget;    /* Surfaces whose bits are potential primaries i.e. back buffers*/
} HEAPALIGNMENT;
typedef struct _HEAPALIGNMENT FAR *LPHEAPALIGNMENT;

/*
 * video memory manager structures
 */
typedef struct _VMEML
{
    struct _VMEML 	FAR *next;
    FLATPTR		ptr;
    DWORD		size;
} VMEML, FAR *LPVMEML, FAR * FAR *LPLPVMEML;

typedef struct _VMEMR
{
    struct _VMEMR 	FAR *next;
    struct _VMEMR       FAR *prev;
    /*
     * The pUp, pDown, pLeft and pRight members were removed in DX5
     */
    struct _VMEMR 	FAR *pUp;
    struct _VMEMR 	FAR *pDown;
    struct _VMEMR 	FAR *pLeft;
    struct _VMEMR 	FAR *pRight;
    FLATPTR		ptr;
    DWORD		size;
    DWORD               x;
    DWORD               y;
    DWORD               cx;
    DWORD               cy;
    DWORD		flags;
    FLATPTR             pBits;
} VMEMR, FAR *LPVMEMR, FAR * FAR *LPLPVMEMR;

typedef struct _VMEMHEAP
{
    DWORD		        dwFlags;
    DWORD                       stride;
    LPVOID		        freeList;
    LPVOID		        allocList;
    DWORD                       dwTotalSize;
    FLATPTR                     fpGARTLin;      /* AGP: GART linear base of heap (app. visible)   */
    FLATPTR                     fpGARTDev;      /* AGP: GART device base of heap (driver visible) */
    DWORD                       dwCommitedSize; /* AGP: Number of bytes commited to heap          */
    /*
     * Extended alignment data:
     * Filled in by DirectDraw in response to a GetHeapAlignment HAL call.
     */
    DWORD                       dwCoalesceCount;
    HEAPALIGNMENT               Alignment;
    /*
     * These are analogous to VIDMEM.ddsCaps and VIDMEM.ddsCapsAlt. These values are queried from the
     * driver by a GetDriverInfo call. See the documentation for GUID_DDMoreSurfaceCaps
     */
    DDSCAPSEX                   ddsCapsEx;
    DDSCAPSEX                   ddsCapsExAlt;
} VMEMHEAP;

typedef VMEMHEAP FAR *LPVMEMHEAP;

#define VMEMHEAP_LINEAR			0x00000001l /* Heap is linear                    */
#define VMEMHEAP_RECTANGULAR		0x00000002l /* Heap is rectangular               */
#define VMEMHEAP_ALIGNMENT  		0x00000004l /* Heap has extended alignment info  */

/*
 * This legacy export doesn't handle nonlocal heaps
 * This function is not available on Windows NT
 */
#ifndef __NTDDKCOMP__
extern FLATPTR WINAPI VidMemAlloc( LPVMEMHEAP pvmh, DWORD width, DWORD height );
#endif

/*
 * This export can be used by drivers to allocate aligned surfaces from heaps which
 * they have previously exposed to DirectDraw. This function can allocate from nonlocal heaps.
 */
extern FLATPTR WINAPI HeapVidMemAllocAligned( 
                LPVIDMEM lpVidMem,
                DWORD dwWidth, 
                DWORD dwHeight, 
                LPSURFACEALIGNMENT lpAlignment , 
                LPLONG lpNewPitch );

/*
 * This export can free memory allocated via either allocation function
 */
extern void WINAPI VidMemFree( LPVMEMHEAP pvmh, FLATPTR ptr );

#ifdef __cplusplus
};
#endif

#endif
