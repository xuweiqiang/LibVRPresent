
/*
 *    File: include/timestamp.h
 *  Author: Payton Quackenbush
 * Purpose:
 *   Notes:
 *
 */

#ifndef _VIDEO_SAMPLE_TIMESTAMP_H
#define _VIDEO_SAMPLE_TIMESTAMP_H

//////////////////////////////////////////////////////////////////////

#define TIMESTAMP_NONE    ((int64) -1)
#ifdef __WINDOWS__
#define TIMESTAMP_MS      ((int64) 10000)
#else
#define TIMESTAMP_MS      ((int64) 1000000)
#endif
#define TIMESTAMP_SECOND  (TIMESTAMP_MS * 1000)

#define TIMESTAMP_TO_MS(timestamp) ((timestamp) / TIMESTAMP_MS)
#define TIMESTAMP_TO_S(timestamp)  ((timestamp) / TIMESTAMP_SECOND)

//////////////////////////////////////////////////////////////////////

#endif // _VIDEO_SAMPLE_TIMESTAMP_H

