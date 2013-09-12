
/*
 *    File: include/vsample.h
 *  Author: Eric DeVolder
 * Purpose: Main picture structure
 *   Notes:
 *
 */

#ifndef _VSAMPLE_H
#define _VSAMPLE_H

//////////////////////////////////////////////////////////////////////

#include <ctypes.h>

#define VSLOG(LEVEL,...) __LOG_##LEVEL(logZone_vsample, 1, __VA_ARGS__)

#define MAX_VIDEO_SAMPLES 25

//////////////////////////////////////////////////////////////////////
#define ALL_COLORSPACES \
    COLORSPACE(YUV420)  \
    COLORSPACE(YUV422)  \
    COLORSPACE(YUV444)  \
    COLORSPACE(YUV411)  \
    COLORSPACE(YUV422V) \
    COLORSPACE(YUV400)  \
    COLORSPACE(RGB555)  \
    COLORSPACE(RGB565)  \
    COLORSPACE(RGB888)

typedef enum
{
    csUnknown = 0,
#define COLORSPACE(C) cs##C,
    ALL_COLORSPACES
#undef COLORSPACE
} ColorSpace_t;

typedef enum
{
    arSample,
    arDisplay
} AspectRatioType_t;

typedef struct
{
    uint32 num;
    uint32 denom;
} VideoSampleFrameRate_t;

typedef struct
{
    int left;
    int top;
    int right;
    int bottom;
} CroppingRect_t;

typedef enum
{
    VS_YUV_FORMAT_Y_U_V, // Y,U,V each in own buffer

    VS_YUV_FORMAT_Y_UV,  // Y in own buffer, UV interleaved in a second buffer
    VS_YUV_FORMAT_Y_VU,  // Y in own buffer, UV interleaved in a second buffer

    VS_YUV_FORMAT_YUYV,  // Y/U/V interleaved in a single buffer
    VS_YUV_FORMAT_YVYU,  // Y/U/V interleaved in a single buffer
    VS_YUV_FORMAT_UYVY,  // Y/U/V interleaved in a single buffer
    VS_YUV_FORMAT_VYUY,  // Y/U/V interleaved in a single buffer

    VS_YUV_FORMAT_LAST
} yuv_format_t;

typedef struct VideoSampleGeometry
{
    ColorSpace_t colorSpace;
    int width; // of displayable region
    int height; // of displayable region

    int alloc_width; // if 0, width will be used
    int alloc_height; // if 0, height will be used
    int alloc_width_scale_factor; // round-up width to multiple of this value
    int alloc_height_scale_factor; // round-up height to multiple of this value

    VideoSampleFrameRate_t frame_rate;

    // Aspect ratio (square pixels are 1:1)
    AspectRatioType_t aspect_ratio_type;
    unsigned int aspect_ratio_width;
    unsigned int aspect_ratio_height;

    union
    {
        struct
        {
			yuv_format_t yuv_format;
            unsigned int luma_alignment_mask;
            unsigned int chroma_alignment_mask;
            int apron_luma_left_width;
            int apron_luma_right_width;
            int apron_luma_top_height;
            int apron_luma_bottom_height;
            int apron_chroma_left_width;
            int apron_chroma_right_width;
            int apron_chroma_top_height;
            int apron_chroma_bottom_height;

            // Luma cropping rectangle
            CroppingRect_t cropping_rect;
        } yuv;

        struct
        {
            int bpp;
        } rgb;
    };

} VideoSampleGeometry_t;

#define VS_FLAGS_NONE			0
#define VS_FLAGS_INTERLACED     (1 <<  0) // 1=interlaced, 0=progressive
#define VS_FLAGS_TOPFIELDFIRST  (1 <<  1) // 1=top field first, 0=bottom field first
#define VS_FLAGS_REPEATFIELD    (1 <<  2) // 1=output field twice (note for a frame this means 1.5 frames) 0=output field once (default)
#define VS_FLAGS_I_PICTURE      (1 <<  4)
#define VS_FLAGS_P_PICTURE      (1 <<  5)
#define VS_FLAGS_B_PICTURE      (1 <<  6)
#define VS_FLAGS_GOP_START      (1 <<  7)
#define VS_FLAGS_SLICE_ERROR    (1 <<  8)
#define VS_FLAGS_BAD_REFERENCE  (1 <<  9)
#define VS_FLAGS_DROPPED_FRAME  (1 << 10)
#define VS_FLAGS_MODIFIED       (1 << 14) // In HW-acclerated environments, used to indicate SW modified frame (may need dcache writeback)
#define VS_FLAGS_EMPTY          (1 << 15) // Facilitates maintaining proper video rate for SKIPped pictures
#define VS_FLAGS_MAE            (1 << 16) // Indication of an MAE-accelerated image (needed by MAEBE)
#define VS_FLAGS_EL             (1 << 17) // image is in little-endian format
#define VS_FLAGS_STILLIMAGE     (1 << 18)
#define VS_FLAGS_SEGMENT        (1 << 19) // Image segment (partial image)
#define VS_FLAGS_EXIT           (1 << 21)

#define VS_FLAGS_BAD_FRAME (VS_FLAGS_SLICE_ERROR | VS_FLAGS_BAD_REFERENCE | VS_FLAGS_DROPPED_FRAME)

typedef struct VideoSample
{
    VideoSampleGeometry_t geometry;

    ColorSpace_t colorSpace;
    int width, height;
    int alloc_width, alloc_height;
    int bytesForSample;
    int64 display_timestamp;
    int64 display_duration;
    unsigned int flags;

    union
    {
        struct
        {
            // Values of just the image components (no apron, no alignment)
            int y_image_width, y_image_height;
            int uv_image_width, uv_image_height;

            // Values of the image components with apron, if any (no alignment)
            int y_imageplusapron_width, y_imageplusapron_height;
            int uv_imageplusapron_width, uv_imageplusapron_height;

            // Virtual pointers to top-left corner pixel of image
            uint8 *y;
            uint8 *u;
            uint8 *v;

            // Physical pointers to top-left corner pixel of image
            phys_t y_phys;
            phys_t u_phys;
            phys_t v_phys;

            // The memory buffer can be exactly as large as image, or larger
            // due to the apron surrounding the image such that the image is
            // contained within the buffer.
            uint8 *y_mem;
            uint8 *u_mem;
            uint8 *v_mem;

            phys_t y_mem_phys;
            phys_t u_mem_phys;
            phys_t v_mem_phys;

            // Values of the image components with apron and alignment (i.e. mem_width is stride)
            int y_mem_width, y_mem_height, y_mem_size;
            int u_mem_width, u_mem_height, u_mem_size;
            int v_mem_width, v_mem_height, v_mem_size;

        } yuv;

        struct
        {
            void *virt; // RGB virtual pointer
            phys_t phys;
            int bpp; // FIX!!! fold into colorSpace

            void *handle;
        } rgb;
    } memory;

    int poolIndex;
    struct VideoSample *originalVideoSample; // this VS is a copy that points to original

    void *vsEx; // extensions for video decoder only
    void (*initEx)(struct VideoSample *vs);
    void (*deinitEx)(struct VideoSample *vs);

    int picSize;
    int decodeOrder;

    int ref_cnt;
    struct VideoSample *next, *prev;
    struct VideoSample *qNext, *qPrev;
    struct VideoSample *poolNext, *poolPrev; // for use only by vsample.c
    int which;
    void *tag; // TODO: Merge with rgb.handle above

} VideoSample_t;

//////////////////////////////////////////////////////////////////////

void vsampleInit (void);
void vsampleDeinit (void);
void vsampleDump (void);

void vsampleInitAllocator (uint8 *virtAddr, uint32 physAddr, uint32 physSize, int align);
void vsampleResetAllocator (void);
int vsampleAllocYUV (VideoSample_t *vs);

// see description in vsample.c
#define VS_FLAGS_ALLOC_NOPICTURE    (1<<0)
#define VS_FLAGS_ALLOC_NOAUXILIARY  (1<<1)
#define VS_FLAGS_ALLOC_STILLIMAGE   (1<<2)

#define vsampleZeroGeometry(pGeometry) memset(pGeometry, 0x00, sizeof(VideoSampleGeometry_t))
void vsampleCreateVideoSamples (int allocFlags, VideoSampleGeometry_t *geometry, int numVideoSamples, void (*attachVsEx)(VideoSample_t *vs), void (*initVs)(VideoSample_t *vs), void (*deinitVs)(VideoSample_t *vs));
void vsampleDestroyVideoSamples (void (*exDeinit)(VideoSample_t *vs));

VideoSample_t *vsampleAllocVideoSample (int allocFlags);

void vsampleWaitForAllVideoSamples (void);
void vsampleRef (VideoSample_t *vs);
void vsampleUnRef (VideoSample_t *vs);
void vsampleCopyByReferenceVideoSample (VideoSample_t *copyVs, VideoSample_t *orgVs);

char vsamplePictureType (VideoSample_t *vs);
int colorspaceBPP(ColorSpace_t colorspace);
const char *colorspaceName(ColorSpace_t colorspace);

//////////////////////////////////////////////////////////////////////

#endif // _VSAMPLE_H

