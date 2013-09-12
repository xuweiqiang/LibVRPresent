
/*
 *    File: include/osal.h
 *  Author: Eric DeVolder
 * Purpose: Operating System Abstraction Layer
 *   Notes:
 *
 */

#ifndef _OSAL_H
#define _OSAL_H

//////////////////////////////////////////////////////////////////////

#include <appcfg.h>
#include <ctypes.h>

#ifdef DEBUG
#define BUILD_NAME "DEBUG"
#else

#define BUILD_NAME "Release"
#endif

//////////////////////////////////////////////////////////////////////
// Endianness detection and swizzling
#ifdef MIPSEL
#define EL
#endif

#ifdef MIPSEB
#define EB
#endif

#ifdef i386
#define EL
#endif

#if !defined(EL) && !defined(EB)
// Default to little endian
#define EL
#endif

#ifdef EL
#define ENDIAN_STR "EL"
#define ENDIAN_SWIZZLE(size, VALUE) VALUE

#else
#define ENDIAN_STR "EB"
#define ENDIAN_SWIZZLE2(IN) ( (((IN) << 8) & 0xff00) | (((IN) >> 8) & 0xff) )
#define ENDIAN_SWIZZLE4(IN) ( (((IN) & 0xff) << 24) | (((IN) & 0xff00) << 8) | (((IN) & 0xff0000) >> 8) | ((IN) >> 24) )

#define ENDIAN_SWIZZLE(SIZE, VALUE) (          \
        (SIZE) == 4 ? ENDIAN_SWIZZLE4(VALUE) : \
        (SIZE) == 2 ? ENDIAN_SWIZZLE2(VALUE) : \
        VALUE)

#endif

#define ENDIAN_SET(STRUCT, MEMBER, VALUE) STRUCT->MEMBER = ENDIAN_SWIZZLE(sizeof(STRUCT->MEMBER), VALUE)


//////////////////////////////////////////////////////////////////////
// For the osal_handle_t structures, must declare these here prior to
// structure definition in linuxcfg.h or win32cfg.h.
// NOTE: REQUIRED_ELEMENTS can be freely referenced by the application.
#define OSAL_FILE_HANDLE_REQUIRED_ELEMENTS \
    int valid;
#define OSAL_MUTEX_HANDLE_REQUIRED_ELEMENTS /* none required */
#define OSAL_EVENT_HANDLE_REQUIRED_ELEMENTS /* none required */
#define OSAL_MAEMEM_HANDLE_REQUIRED_ELEMENTS \
    uint32 physAddr; \
    uint32 physSize; \
    void *virtAddr; \
    uint32 YUVphysAddr; \
    uint32 YUVphysSize; \
    void *YUVvirtAddr; \
    uint32 counterFrequency;
#define OSAL_MAEFE_HANDLE_REQUIRED_ELEMENTS \
    uint32 counterFrequency;
#define OSAL_MAEBSA_HANDLE_REQUIRED_ELEMENTS \
    uint32 counterFrequency;
#define OSAL_MAEMPE_HANDLE_REQUIRED_ELEMENTS \
    uint32 counterFrequency;
#define OSAL_MAEBE_HANDLE_REQUIRED_ELEMENTS \
    uint32 memPhysAddr; \
    uint32 memPhysSize; \
    void *memVirtAddr; \
    uint32 counterFrequency;
#define OSAL_MAELCD_HANDLE_REQUIRED_ELEMENTS \
    uint32 physAddr; \
    uint32 physSize; \
    void *virtAddr; \
    int screen_width; \
    int screen_height; \
    int screen_size; \
    int win_width; \
    int win_height; \
    uint32 win_physAddr; \
    int win_depth32;


#if defined(WIN32) || defined(_WIN32_WCE)
#include "../osal/win32cfg.h"
#ifndef __WINDOWS__
#define __WINDOWS__
#endif
#else
#include "../osal/linuxcfg.h"
#endif

#include "mediaErrors.h"

//////////////////////////////////////////////////////////////////////

// Endian support routines.

// Undef any swap implementations by the OS headers
#undef swap16
#undef swap24
#undef swap32
#undef SWAP16
#undef SWAP24
#undef SWAP32

#define swap16(X) \
    ( (((uint16)(X) & 0x00FF) << 8) | \
      (((uint16)(X) & 0xFF00) >> 8) )

#define swap24(X) \
    ( (((uint32)(X) & 0x000000FF) << 16) | \
      (((uint32)(X) & 0x0000FF00) << 0)  | \
      (((uint32)(X) & 0x00FF0000) >> 16) )

#define swap32(X) \
    ( (((uint32)(X) & 0x000000FF) << 24) | \
      (((uint32)(X) & 0x0000FF00) << 8)  | \
      (((uint32)(X) & 0x00FF0000) >> 8)  | \
      (((uint32)(X) & 0xFF000000) >> 24) )

#define SWAP16(X) (X) = swap16(X)
#define SWAP24(X) (X) = swap24(X)
#define SWAP32(X) (X) = swap32(X)

//////////////////////////////////////////////////////////////////////

void queue_reset(void);
void queue_register(const char *name, volatile unsigned int *size);
unsigned queue_dump(char *result);

//////////////////////////////////////////////////////////////////////

void osal_sleep_seconds (int seconds);
void osal_sleep_milliseconds (int milliseconds);
int osal_random (void);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
struct osal_file_handle_t;

#define OSAL_FILE_FLAGS_READ        0x01
#define OSAL_FILE_FLAGS_WRITE       0x02
#define OSAL_FILE_FLAGS_CREATE      0x04
#define OSAL_FILE_FLAGS_TRUNCATE    0x08

int osal_file_open (struct osal_file_handle_t *h, const char *path, int flags);
int osal_file_read (struct osal_file_handle_t *h, unsigned int position, void *buffer, int size);
int osal_file_write (struct osal_file_handle_t *h, const void *buffer, int size);
void osal_file_sync (struct osal_file_handle_t *h);
void osal_file_close (struct osal_file_handle_t *h);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

// Thread priorities should be such that the end of the pipeline has
// the highest priority so that that infrequent yet time critical
// activities occur on time, and also prevent backing up the picture
// pipeline.
// However, the reader thread needs high priority to make sure the
// pipeline is never stalled as well.

#define OSAL_THREAD_PRIORITY_NAVIGATION         19 // be responsive to user input

#define OSAL_THREAD_PRIORITY_READER             20
#define OSAL_THREAD_PRIORITY_DEMUX              2

#define OSAL_THREAD_PRIORITY_ACODEC             5
#define OSAL_THREAD_PRIORITY_ACODEC_HARDWARE    6 // allow HW thread to continue parallel processing
#define OSAL_THREAD_PRIORITY_ARENDER            7
#define OSAL_THREAD_PRIORITY_ARENDER_HARDWARE   8 // allow HW thread to continue parallel processing
#define OSAL_THREAD_PRIORITY_AOUTPUT            9

#define OSAL_THREAD_PRIORITY_VCODEC             10
#define OSAL_THREAD_PRIORITY_VCODEC_HARDWARE    11 // allow HW thread to continue parallel processing
#define OSAL_THREAD_PRIORITY_VRENDER            14
#define OSAL_THREAD_PRIORITY_VRENDER_HARDWARE   15 // allow HW thread to continue parallel processing
#define OSAL_THREAD_PRIORITY_VOUTPUT            16

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//struct osal_maemem_handle_t;

void osal_maemem_driver_open (osal_maemem_handle_t *h, int mae_size, int yuv_size);
void osal_maemem_driver_close (osal_maemem_handle_t *h);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
struct mae_fe_request_t;

void osal_maefe_driver_open (osal_maefe_handle_t *h);
void osal_maefe_driver_submit (osal_maefe_handle_t *h, struct mae_fe_request_t *req);
void osal_maefe_driver_close (osal_maefe_handle_t *h);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
struct mae_bsa_request_t;

void osal_maebsa_driver_open (osal_maebsa_handle_t *h);
void osal_maebsa_driver_submit (osal_maebsa_handle_t *h, struct mae_bsa_request_t *req);
void osal_maebsa_driver_close (osal_maebsa_handle_t *h);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
struct mae_mpe_request_t;

void osal_maempe_driver_open (osal_maempe_handle_t *h);
void osal_maempe_driver_submit (osal_maempe_handle_t *h, struct mae_mpe_request_t *req);
void osal_maempe_driver_close (osal_maempe_handle_t *h);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
struct mae_be_request_t;

void osal_maebe_driver_open (osal_maebe_handle_t *h);
void osal_maebe_driver_submit (osal_maebe_handle_t *h, struct mae_be_request_t *req);
void osal_maebe_driver_close (osal_maebe_handle_t *h);
void osal_maebe_mem_open (osal_maebe_handle_t *h, int yuv_size);
void osal_maebe_mem_close (osal_maebe_handle_t *h);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
struct osal_maelcd_handle_t;

void osal_maelcd_driver_open (osal_maelcd_handle_t *h);
void osal_maelcd_set_output_window (osal_maelcd_handle_t *h, int width, int height, int xpos, int ypos);
void osal_maelcd_flip_window_pointer (osal_maelcd_handle_t *h, uint32 physAddr);
void osal_maelcd_driver_close (osal_maelcd_handle_t *h);

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#define OSAL_VIDEO_CAPS_720P	0
#define OSAL_VIDEO_CAPS_D1		1
#define OSAL_VIDEO_CAPS_WCIF	2

int osal_get_video_caps(void);

//////////////////////////////////////////////////////////////////////

#endif // _OSAL_H

