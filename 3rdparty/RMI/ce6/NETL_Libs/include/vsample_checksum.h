#ifndef __VSAMPLE_CHECKSUM__
#define __VSAMPLE_CHECKSUM__

#include <vsample.h>

void vsample_checksum_init(const char *video_codec,
                           const char *checksum_path,
                           unsigned int max_frames,
                           unsigned int dump_components);
void vsample_checksum_stop(void);

void dump_component(osal_file_handle_t *h,
                    uint8_t *data,
                    unsigned int data_increment,
                    unsigned int stride,
                    unsigned int data_width,
                    unsigned int data_height);

void vsample_checksum(VideoSample_t *yuv_sample);

typedef uint32_t(* checksum_function)(const uint8_t *data,
                                      unsigned int data_increment,
                                      unsigned int stride,
                                      unsigned int data_width,
                                      unsigned int data_height);

checksum_function get_video_checksum(const char *video_codec);

#endif

