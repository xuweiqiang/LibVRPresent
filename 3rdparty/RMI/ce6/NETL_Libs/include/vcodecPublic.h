
/*
 *    File: include/vcodecPublic.h
 *  Author: Eric DeVolder
 * Purpose: Interface to be used by plug-ins/filters for vcodec.c
 *   Notes: Keep this file free of external dependencies
 *
 */

#ifndef _VCODEC_PUBLIC_H
#define _VCODEC_PUBLIC_H

//////////////////////////////////////////////////////////////////////

#include "bitbuffer.h"

//////////////////////////////////////////////////////////////////////

#define ALL_VCODECS        \
    VCODEC_NAME(yuv)       \
    VCODEC_NAME(mpeg1)     \
    VCODEC_NAME(mpeg2)     \
    VCODEC_NAME(mpeg4)     \
    VCODEC_NAME(h263)      \
    VCODEC_NAME(h264)      \
    VCODEC_NAME(wmv7)      \
    VCODEC_NAME(wmv9)      \
    VCODEC_NAME(vc1)       \
    VCODEC_NAME(msmpeg4v1) \
    VCODEC_NAME(msmpeg4v2) \
    VCODEC_NAME(msmpeg4v3) \
    VCODEC_NAME(jpeg)      \
    VCODEC_NAME(libjpeg)   \

typedef enum
{
    vcodec_reserved = 0,

#define VCODEC_NAME(Z) vcodec_##Z,
    ALL_VCODECS
#undef VCODEC_NAME

    MAX_VCODECS
} vcodecTypes;

typedef struct
{
    // Discovered by demux/container
    vcodecTypes codec;
    unsigned char *codec_data;
    int codec_data_size;
    int is_streaming;
    int width;
    int height;
    int framerate_numerator;
    int framerate_denominator;
	double playback_rate;

    // Init-time options
    int watchdog;
    int watchdogPeriod;
    int dumpBitBuffers;
    int decodeOrder; // display in decode order instead of display order
    int writeVideoElementaryStream;

    int enable_log;
    int firstError;
    int lastError;

    // Timestamping
    int enable_timestamp_debug;
    int reverse_timestamps;
    int64 segment_start_timestamp;
    int64 segment_end_timestamp;

    // Quality of Service
    int dropIs;
    int dropPs;
    int dropBs;

    // Statistics
    int frames;
    int pictures;
    int dropCount;
    int maxBytesPerSample;

    // MAE settings
    int maeBufferPercentage;
    int maeExtraAU2MAE;
    int maePerf;
    int maePipelined;
    int maeTraceIoctl;
    int feDisabled;
    int bsaDisabled;
    int mpeDisabled;
    int bsaStopOnError;
    int bsaBuffers;

    // Callbacks
    void (*report_error)(int errCode, const char *errMsg);

} vcodecConfig_t;

extern vcodecConfig_t vcodecConfig;

//////////////////////////////////////////////////////////////////////
const char *vcodecName (vcodecTypes codec);

void vcodecConfigReset (void);
int  vcodecConfigValid (void);

void vcodecInit(void);
void vcodecStart (void);
void vcodecStop (void);
void vcodecReset (void);

void vcodecAppendBitBuffer (
    void (*unRef)(void *bufferObject), void *bufferObject,
    void *buffer, unsigned int buffer_size,
    int64 display_timestamp, int64 display_duration);

void vcodecSyncFlush (void);
void vcodecAsyncFlushStart (void);
void vcodecAsyncFlushEnd (void);

void vcodecAssert (int errCode);

// FIX!!! also need vcodec_ref/unref_VideoSample
// FIX!!! probably should end up migrating this back into vcodec.h???

struct VideoSample;
void app_attach_VideoSample (struct VideoSample *vs);
void app_detach_VideoSample (struct VideoSample *vs);

//////////////////////////////////////////////////////////////////////

#endif // _VCODEC_PUBLIC_H

