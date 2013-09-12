#ifndef __THUMBNAIL_H__
#define __THUMBNAIL_H__

#include <vsample.h>

void vrenderThumbnailReset (void);

// FIXME: remove outpic!
void vrenderThumbnail (VideoSample_t *inpic, VideoSample_t *outpic,
                       void (*render)(VideoSample_t *inVs, VideoSample_t *outVs));

void vrenderThumbnailStop (void);

#endif
