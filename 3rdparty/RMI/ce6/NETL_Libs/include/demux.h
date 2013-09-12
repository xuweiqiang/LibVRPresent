
/*
 *    File: include/demux.h
 *  Author: Eric DeVolder
 * Purpose: Interface to demux/demux.c and components
 *   Notes:
 *
 */

#ifndef _DEMUX_H
#define _DEMUX_H

//////////////////////////////////////////////////////////////////////

#include <osal.h>

#include <ctypes.h>
#include <asserts.h>
#include <logging.h>
#include <mediaErrors.h>

#define DLOG(LEVEL,...) __LOG_##LEVEL(logZone_demux, 1, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////

// demux.c
typedef enum
{
    demux_reserved = 0,
    demux_vraw,
    demux_araw,
    demux_mpeg,
    demux_mp4,
    demux_avi,
    demux_asf,
    demux_mkv,
    demux_rcv,

    MAX_DEMUXS
} demux_type;

typedef struct
{
    char *name;
    char *description;
    demux_type which;
    int detectOrder;
    int storageSize;
    int (*detect)(uint8 *buffer, int length);
    void (*init)(void *context);
    int (*demux)(void *context);
    void (*deinit)(void *context);
} demuxer_t;

int demux_get_type (void);
int demuxSelect (demux_type which);

void demuxStart (const char *url);
void demuxStop (void);

#define DEMUX_DETECTORDER_FIRST  0
#define DEMUX_DETECTORDER_SECOND 1
#define DEMUX_DETECTORDER_THIRD  2
#define DEMUX_DETECTORDER_LAST   3

int demux_bits_end_of_stream (unsigned int position);
void demux_bits_read (unsigned int position, unsigned int length);
void demux_bits_discard (void);

#ifndef demux_alloc
#define demux_alloc(X) malloc(X)
#endif

#ifndef demux_free
#define demux_free(X) free(X)
#endif

struct bitBuffer;
struct gbContext;
extern struct gbContext dgbc;
void demux_add_bitBuffer (bitBuffer_t *bb);
bitBuffer_t *demux_get_bitBuffer (void);
void dgb_handle_payload_data (int length, int add_to_video_stream);
void dgb_stop (void);

// Available demux components
extern demuxer_t vraw_demux;
extern demuxer_t araw_demux;
extern demuxer_t mpeg_demux;
extern demuxer_t mp4_demux;
extern demuxer_t avi_demux;
extern demuxer_t asf_demux;
extern demuxer_t mkv_demux;
extern demuxer_t rcv_demux;

//////////////////////////////////////////////////////////////////////
// Interface for individual demuxers to provide data to demux.c

#define FOURCC(a,b,c,d) (((uint8)a<<24)|((uint8)b<<16)|((uint8)c<<8)|((uint8)d<<0))

// Default values used to fill in properties
#define VIDEO_FOURCC_YUV        FOURCC('y','u','v',' ')
#define VIDEO_FOURCC_MPEG1      FOURCC('m','p','1','v')
#define VIDEO_FOURCC_MPEG2      FOURCC('m','p','2','v')
#define VIDEO_FOURCC_MPEG4      FOURCC('m','p','4','v')
#define VIDEO_FOURCC_MSMPEG4V3  FOURCC('m','p','4','3')
#define VIDEO_FOURCC_H263       FOURCC('h','2','6','3')
#define VIDEO_FOURCC_H264       FOURCC('h','2','6','4')
#define VIDEO_FOURCC_VC1        FOURCC('W','V','C','1')
#define VIDEO_FOURCC_JPEG       FOURCC('j','p','e','g')

typedef union
{
    struct
    {
        uint32 codecType; // A FOURCC or BOXTYPE code
        int width, height;
        int rateNumerator, rateDenominator;
        unsigned int flags;
#define DEMUX_TRACK_VIDEO_FLAGS_ELEMSTREAM  0x01
#define DEMUX_TRACK_VIDEO_FLAGS_DEMUXQUEUE  0x02
    } video;

    struct
    {
        int sampleRate;
    } audio;

} demuxTrackProperties_t;

static inline void
demux_zero_track_properties (demuxTrackProperties_t *prop)
{
    int *p = (int *)prop;
    int i;
    for (i = 0; i < sizeof(demuxTrackProperties_t)/sizeof(int); ++i)
        *p++ = 0;
}

// This structure is the list of sample fragments (must allocate an array of these)
typedef struct
{
    uint32 offset;
    uint32 size;

} demuxSampleFragment_t;

void *demux_alloc_audio_track (int trackId);
void *demux_alloc_video_track (int trackId);
void demux_add_parse_buffer (void *trackHandle, uint8 *bits, int length);
void demux_set_numberOfSamples(void *trackHandle, int totalNumberOfSamples);
void demux_set_sample (void *trackHandle, int sampleIndex, uint32 offset, uint32 size);
void demux_set_fragmented_sample (void *trackHandle, int sampleIndex, demuxSampleFragment_t *fragmentList, int numberOfFragments, int sumOfFragments);
void demux_set_track_properties (void *trackHandle, demuxTrackProperties_t *properties);
void demux_end_track_properties (void);

//////////////////////////////////////////////////////////////////////

#endif // _DEMUX_H

