#ifndef _VOUTPUT_PUBLIC_H
#define _VOUTPUT_PUBLIC_H

#include <ctypes.h>

typedef struct
{
    // Inputs
    unsigned rgb_width;
    unsigned rgb_height;
    unsigned rgb_bytes_per_pixel;

    unsigned framerate_num;
    unsigned framerate_denom;

    // Outputs
    uint32_t rgb_phys;
    uint8_t *rgb_virt;

    void *tag;
} RgbBufferParams_t;

typedef struct
{
    int scaleNumerator;
    int scaleDenominator;
    int delay_ms;
    int pause;
    int vsync;

    int rgb_bytes_per_pixel;
    int rgb_panel_width;
    int rgb_panel_height;

    int64_t segment_start_timestamp;
    int64_t segment_end_timestamp;

    int64_t qos_ms;

    uint32_t picture_count;
    int fast_output;

    int on_screen_debug;
    int osd_verbosity;

    // Callback functions
    int (*output_rgb_buffer)(void *buffer, int64_t display_timestamp, int64_t display_duration);
    int (*alloc_rgb_buffer)(RgbBufferParams_t *params);
} voutputConfig_t;

extern voutputConfig_t voutputConfig;

void voutputConfigReset (void);

#endif
