
/*
 *    File: include/vcodec.h
 *  Author: Eric DeVolder
 * Purpose: Private interface to vcodec/vcodec.c for use by decoders
 *   Notes:
 *
 */

#ifndef _VCODEC_H
#define _VCODEC_H

//////////////////////////////////////////////////////////////////////

#include <osal.h>

#include <vcodecPublic.h>
#include <vsample.h>
#include <logging.h>
#include <mediaErrors.h>

#define VCLOG(LEVEL,...) __LOG_##LEVEL(logZone_vcodec, 1, __VA_ARGS__)
#define VLOG(LEVEL,SUBMASK,...) __LOG_##LEVEL(logZone_vdec, SUBMASK, __VA_ARGS__)
#define VLOGGABLE(LEVEL,SUBMASK) LOGGABLE_##LEVEL(logZone_vdec, SUBMASK)

//////////////////////////////////////////////////////////////////////

typedef struct
{
    char *name;
    char *description;
    vcodecTypes which;
    int storageSize;
    int (*init)(void *context, unsigned char *codec_data, int codec_data_size);
    void (*flush)(void *context);
    int (*decode)(void *context);
    void (*deinit)(void *context);
} vcodec_t;

struct gbContext;
extern struct gbContext vgbc;
extern void *vcodec_context;

void vcodec_assert (int errCode);
void vgb_first (struct gbContext *gbc);
bitBuffer_t *vgb_next (struct gbContext *gbc);
int vgb_next_packet (void);
void vgb_check_segment (void);
void vcodec_add_bitBuffer (bitBuffer_t *bb);
void vcodec_decoded_VideoSample (VideoSample_t *vs);
int vcodec_drop_VideoSample (unsigned int vsFlags);
void vcodec_output_VideoSample (VideoSample_t *vs);
void vcodec_init_VideoSamples (int allocFlags, VideoSampleGeometry_t *geometry, int numVideoSamples);
VideoSample_t *vcodec_alloc_VideoSample (int allocFolags);
void vcodec_ref_VideoSample (VideoSample_t *vs);
void vcodec_unref_VideoSample (VideoSample_t *vs);
void vcodec_copy_VideoSample (VideoSample_t *newVs, VideoSample_t *orgVs);
void vcodec_flush_VideoSamples (void);
void vcodec_async_flush_complete (void);
void vcodec_sync_flush_complete (void);

#ifndef vcodec_alloc
#define vcodec_alloc(X) malloc(X)
#endif

#ifndef vcodec_free
#define vcodec_free(P) free(P)
#endif

// Available video components
extern vcodec_t ves_vcodec;
extern vcodec_t yuv_vcodec;
extern vcodec_t mpeg1_vcodec;
extern vcodec_t mpeg2_vcodec;
extern vcodec_t h263_vcodec;
extern vcodec_t h264_vcodec;
extern vcodec_t wmv7_vcodec;
extern vcodec_t wmv9_vcodec;
extern vcodec_t vc1_vcodec;
extern vcodec_t mpeg4_vcodec;
extern vcodec_t msmpeg4v1_vcodec;
extern vcodec_t msmpeg4v2_vcodec;
extern vcodec_t msmpeg4v3_vcodec;
extern vcodec_t jpeg_vcodec;
extern vcodec_t libjpeg_vcodec;


extern int gEnableAssertPrint;

//////////////////////////////////////////////////////////////////////

#endif // _VCODEC_H

