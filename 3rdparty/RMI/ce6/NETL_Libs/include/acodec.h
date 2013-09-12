
/*
 *    File: include/acodec.h
 *  Author: Eric DeVolder
 * Purpose: Audio decoder infrastructure interface
 *   Notes: 
 *
 */

#ifndef _ACODEC_H
#define _ACODEC_H

//////////////////////////////////////////////////////////////////////

#include <osal.h>

#include <ctypes.h>
#include <logging.h>

#define ACLOG(LEVEL,...) __LOG_##LEVEL(logZone_acodec, __VA_ARGS__)
#define ALOG(LEVEL,...) __LOG_##LEVEL(logZone_auddec, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////

struct bitBuffer;
struct gbContext;
extern struct gbContext agbc;

typedef enum
{
    acodec_reserved = 0,

    MAX_ACODECS
} acodec_types;

//void acodec_add_bitBuffer (bitBuffer *bb);
#define acodec_add_bitBuffer(bb)

#define acodecLookupByName(N) -1
#define acodecSelect(X) 0
#define acodecInit()
#define acodecStart()
#define acodecStop()

//////////////////////////////////////////////////////////////////////

#endif // _ACODEC_H

