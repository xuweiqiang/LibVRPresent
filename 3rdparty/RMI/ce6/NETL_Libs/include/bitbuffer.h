
/*
 *    File: include/bitbuffer.h
 *  Author: Eric DeVolder
 * Purpose: Interface to common/bitbuffer.h
 *   Notes:
 *
 */

#ifndef _BITBUFFER_H
#define _BITBUFFER_H

//////////////////////////////////////////////////////////////////////

typedef struct bitBuffer
{
    unsigned char *data;
    int dataSize;
    uint64 display_timestamp;
    uint64 display_duration;
    unsigned int flags;
#define BB_FLAGS_EXIT           (1 <<  0) // Exit/done indication for decoder
#define BB_FLAGS_FLUSH          (1 <<  1) // Non-discarding flush (no more data until a discontinuity buffer)
#define BB_FLAGS_DISCONTINUITY  (1 <<  2) // Discontinuity marker
#define BB_FLAGS_PACKET         (1 <<  4) // This BB is part of a demux packet
#define BB_FLAGS_PACKETSTART    (1 <<  5) // This BB contains the beginning data of a demux packet
#define BB_FLAGS_PACKETEND      (1 <<  6) // This BB contains the last data of a demux packet

    unsigned int packetSize; // For BB_FLAGS_PACKET, indicates number of bytes in packet (of which this BB could be just one of many in packet)
    unsigned int filepos;

    // For managing the underlying storage to which bitBuffer points
    void (*refBufferCallback)(void *bufferObject);
    void (*unrefBufferCallback)(void *bufferObject);
    void *bufferObject;

    struct bitBuffer *next, *prev; // for creating seamless stream
    struct bitBuffer *poolNext, *poolPrev; // for bitbuffer.c use only

} bitBuffer_t;

bitBuffer_t *bitBufferAlloc (void);
void bitBufferFree (bitBuffer_t *bb);
void bitBufferInit (void);
void bitBufferDeinit (void);

//////////////////////////////////////////////////////////////////////

#endif // _BITBUFFER_H

