#ifndef __BMP_H__
#define __BMP_H__

#include <ctypes.h>
#include <vsample.h>

typedef enum
{
    BMP_RGB_555 = 2,
    BMP_RGB_888A = 4,
    //BMP_RGB_16 = 16565,
} BitmapDepth_t;

typedef struct
{
    int width;
    int height;
    BitmapDepth_t depth;
    int stride;
    uint8 *pixels;
} Bitmap_t;

void bmp_create(Bitmap_t *bmp, const char *dest_path);
void vsample_to_bmp(VideoSample_t *bmppic, const char *dest_path);

#endif
