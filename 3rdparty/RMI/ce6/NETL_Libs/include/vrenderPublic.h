#ifndef _VRENDER_PUBLIC_H
#define _VRENDER_PUBLIC_H

#include <ctypes.h>

struct VideoSample;

typedef struct
{
    void *pointer; // Memory pointer to render RGB to
    char *path; // Path to store bitmap file to

    int max_width;
    int max_height;

    int interval; // Render every Nth picture to file
    int count;

    int depth;
    int full_size;

    int enable_html;
    int max_html_width;
} Thumbnail_t;

typedef struct
{
    int csc;    // colorspace coefficients
    int crop;   // crop image based on cropping region
    int submit;
    Thumbnail_t thumbnail;

    unsigned numInterlaced;
    unsigned numProgressive;
    unsigned numDropped;

    int enable_checksum;
    int dump_components;

    int crc;
    int dump;
    int dumpLimit;

    uint8 *dumpY;
    uint8 *dumpU;
    uint8 *dumpV;

    int dumpPart;
    int dumpBytes;

    uint32_t picture_count;
    uint32_t peak_bitrate;
    uint64_t total_bits;

    uint32_t bitstream_buffer_count;

    const char *codec_name;

    char *yuv_checksum_path;
    int yuv_checksum_max_frames;

    int stretch;
    int maxpect;
    int zoom;

    unsigned int yuv_little_endian;

    int flushing;
    int paused;

    int enableBePerf;
} vrenderConfig_t;

extern vrenderConfig_t vrenderConfig;

void vrenderConfigReset (void);
int vrenderConfigValid (void);

#endif
