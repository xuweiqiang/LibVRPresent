#ifndef __VIDEO_CHECKSUM__
#define __VIDEO_CHECKSUM__

#include <ctypes.h>

uint32_t vc_average32(const uint8_t *data,
                      unsigned int data_increment,
                      unsigned int stride,
                      unsigned int data_width,
                      unsigned int data_height);

uint32_t vc_crc32(const uint8_t *data,
                  unsigned int data_increment,
                  unsigned int stride,
                  unsigned int data_width,
                  unsigned int data_height);

#endif
