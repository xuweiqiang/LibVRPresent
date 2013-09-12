#ifndef __bit_packer_h__
#define __bit_packer_h__

/*
 *    File: common/bit_packer.c
 *  Author: Payton Quackenbush
 * Purpose: Little endian bit packer
 *   Notes:
 *
 */

#ifdef WINCE
#ifndef inline
#define inline	__inline
#endif
#endif
#include <ctypes.h>

typedef struct
{
    uint32 word;
    uint32 word_size;

    uint32 *buffer;
    uint32 *ptr;

    const char *name;
} PackState_t;

void packer_init(PackState_t *pack_state, const char *name);
void packer_deinit(PackState_t *pack_state);

//#define MAE2_MPE_DEBUG

#ifdef MAE2_MPE_DEBUG
#include <stdio.h>
extern const char *PACK_LOG_FORMAT;
#define PACK_COMPLETE(PACK_STATE)  printf(PACK_LOG_FORMAT, (PACK_STATE)->name, (PACK_STATE)->ptr - (PACK_STATE)->buffer, (PACK_STATE)->word)

#else

#define PACK_COMPLETE(PACK_STATE)
#endif

#define packbits(pack_state, bitsize, value)        \
{ \
    uint32 V = (value); /* cache the value locally so shifts don't repeat work */  \
    (pack_state)->word |= (V << (pack_state)->word_size); \
    (pack_state)->word_size += (bitsize); \
\
    if((pack_state)->word_size >= 32) \
    { \
        PACK_COMPLETE(pack_state); \
        *(pack_state)->ptr++ = (pack_state)->word; \
        (pack_state)->word_size -= 32; \
        (pack_state)->word = V >> (bitsize - (pack_state)->word_size); \
    } \
}

/*ASSERT((pack_state)->buffer_size < BUFFER_MAX, "Pack buffer overflow: %d\n", (pack_state)->buffer_size);*/

// Flush out the current word (if any)
static inline void
packer_flush(PackState_t *pack_state)
{
    if(pack_state->word_size > 0)
    {
        // Align to 32-bits
        packbits(pack_state, 32 - pack_state->word_size, 0x0);
    }

    if((pack_state->ptr - pack_state->buffer) & 1)
    {
        // Align to 64-bits
        packbits(pack_state, 32, 0x0);
    }
}

#endif
