
/*
 *    File: include/bsbuffer.h
 *  Author: Eric DeVolder
 * Purpose: Interface to demux/bsbuffer.c
 *   Notes: 
 *
 */

#ifndef _BSBUFFER_H
#define _BSBUFFER_H

//////////////////////////////////////////////////////////////////////

#include <osal.h>

#include "ctypes.h"
#include <logging.h>

#include <bitbuffer.h>

//////////////////////////////////////////////////////////////////////

/*
 * The byteStreamBuffer contains the bulk data, e.g. byte-stream, from what
 * ever the input source might be (disk, network, etc.)
 *
 * Rules governing byteStreamBuffer:
 * - alloced by demux from freeList onto inuseList
 * - populated by demux
 * - codecs only READ dataSize and data
 * - when a codec is finished with byteStreamBuffer, it releases it
 * - to release a byteStreamBuffer, codec uses critical section lock, and
 *   decrements users. if users == 0, it then removes from inuseList
 *   and places on freeList.
 * - when (demux or codec) changing next/prev, must use critical section lock
 */

#define BSBUFFER_SIZE (256*1024) // small for debug purposes, large for performance
//#define BSBUFFER_READ (4*1024) // read/request 4KB at a time

typedef struct byteStreamBuffer
{
    int users;
    int dataValid;
    int dataSize;
    int end_of_file;
    unsigned int filepos;
    uint8 *vaBuffer;

    struct byteStreamBuffer *poolNext, *poolPrev; // for mgmt by buffer.c
} byteStreamBuffer;

void byteStreamBufferAttach (struct bitBuffer *bb, byteStreamBuffer *bs, int incrementBsUsers);
void byteStreamBufferUse (byteStreamBuffer *bs);
byteStreamBuffer *byteStreamBufferFill (unsigned int position);
void byteStreamBuffer_init (void);
void byteStreamBuffer_deinit (void);

//////////////////////////////////////////////////////////////////////

#endif // _BSBUFFER_H

