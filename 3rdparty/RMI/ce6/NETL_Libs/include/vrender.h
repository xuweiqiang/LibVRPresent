
/*
 *    File: include/vrender.h
 *  Author: Eric DeVolder
 * Purpose: Interface to vrender/vrender.c
 *   Notes:
 *
 */

#ifndef _VRENDER_H
#define _VRENDER_H

//////////////////////////////////////////////////////////////////////

#include <osal.h>

#include <vsample.h>
#include <logging.h>
#include <vrenderPublic.h>
#include <mediaErrors.h>

#define VRLOG(LEVEL,...) __LOG_##LEVEL(logZone_vrender, 1, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////

typedef enum
{
    vrender_reserved = 0,
    vrender_software,
    vrender_maebe,

    MAX_VRENDERS
} vrender_type;

typedef struct
{
    char *name;
    char *description;
    vrender_type which;
    int max_upscale;
    int max_downscale;

    void (*init)(void);
    void (*reset)(void);
    int (*allocBuffer)(void **pVA, phys_t *pPA, unsigned int size, unsigned int alignment);
    void (*render)(VideoSample_t *inVs, VideoSample_t *outVs);
    void (*deinit)(void);
} vrender_t;

void vrenderInit (void);
void vrenderStart (void);
void vrenderStop (void);
int vrenderAllocBuffer (void **va, phys_t *pa, unsigned int size, unsigned int alignment);
void vrenderVideoSample (VideoSample_t *vs);

void vrenderStartDump (int width, int height);
void vrenderWriteDump (uint8 *Y, uint8 *U, uint8 *V, int width, int height);
void vrenderEndDump (void);

#ifndef vrender_alloc
#define vrender_alloc(X) malloc(X)
#endif

#ifndef vrender_free
#define vrender_free(P) free(P)
#endif

// Available video render components
extern vrender_t software_vrender;
extern vrender_t maebe_vrender;

// Colour space transfer characteristics
enum
{
    CSC_SMPTE170 = 0,
    CSC_SMPTE240,
    CSC_STUDIORGB,
    CSC_BT709,
    CSC_SMPTE170noscale,
    CSC_BT601,
    CSC_GRAYSCALE,

    MAX_CSC
};

//////////////////////////////////////////////////////////////////////

#endif // _VRENDER_H

