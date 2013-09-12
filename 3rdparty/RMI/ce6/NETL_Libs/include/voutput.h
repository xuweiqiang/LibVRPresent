
/*
 *    File: include/voutput.h
 *  Author: Eric DeVolder
 * Purpose: Interface to voutput/voutput.c
 *   Notes:
 *
 */

#ifndef _VOUTPUT_H
#define _VOUTPUT_H

//////////////////////////////////////////////////////////////////////

#include <osal.h>

#include <vsample.h>
#include <logging.h>
#include <voutputPublic.h>
#include <mediaErrors.h>

#define VOLOG(LEVEL,...) __LOG_##LEVEL(logZone_voutput, 1, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////

typedef enum
{
    voutput_reserved = 0,
    voutput_null,
    voutput_file,
    voutput_sdl,
    voutput_maelcd,
    voutput_callback,

    MAX_VOUTPUTS
} voutput_type;

typedef struct
{
    char *name;
    char *description;
    voutput_type which;
    void (*init)(void);
    int (*allocBuffer)(void **pVA, phys_t *pPA, unsigned int alignment);
    int (*getVideoSampleFormat)(void);
    void (*getMaxDimensions)(int *width, int *height);
    int (*initOutputVideoSample)(VideoSample_t *yuvVs, VideoSample_t *rgbVs);
    int (*outputVideoSample)(VideoSample_t *vs);
    void (*deinit)(void);
} voutput_t;

extern int voNumerator;
extern int voDenominator;

void voutputInit (void);
void voutputStart (void);
void voutputStop (void);
voutput_type voutputLookupByName (const char *name);
int voutputSelect (voutput_type which);
int voutputGetVideoSampleFormat (void);
void voutputGetMaxDimensions (int *width, int *height);
int voutputAllocVideoSample (VideoSample_t **outVs);
int voutputInitVideoSample (VideoSample_t *inVs, VideoSample_t *outVs);
int voutputOutputVideoSample (VideoSample_t *vs);

// Available video output components
extern voutput_t null_voutput;
extern voutput_t file_voutput;
extern voutput_t sdl_voutput;
extern voutput_t maelcd_voutput;
extern voutput_t callback_voutput;

#ifndef voDone
#define voDone() exit(-6)
#endif

//////////////////////////////////////////////////////////////////////

#endif // _VOUTPUT_H

