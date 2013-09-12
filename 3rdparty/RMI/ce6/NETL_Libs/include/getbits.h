
/*
 *    File: include/getbits.h
 *  Author: Eric DeVolder
 * Purpose: Method for extracts variable bits from bitstream
 *   Notes: See below.
 *
 */

#ifndef _GETBITS_H
#define _GETBITS_H

//////////////////////////////////////////////////////////////////////

#if 0
There are two significant optmizations left, I believe.
1) Is to bring in more than one byte at a time, in a way that does
not involve loops. Perhaps redefine this module to only guarantee
16 bits after a gb_fill_bitCache() for example (and this seems
reasonable based on the optimizations done to mpeg2/vlc.c). By
guaranteeing only 16-bits, it then becomes possible to always
load in two bytes in an efficient manner.

2) The second is more subtle. By looking at the code in this
module, you will notice alot of (32 - num_bits) expressions. This
requires a few extra instructions at run-time, and for something
that is invoked as frequently as getbits, there is significant
savings to be had of (32 - num_bits) can be optimized. I believe
there is a way to do this, and it is re-normalizing a num_bits of
0 to be -16. Why? -16 is b1111111111100000. And all the shift
operations which operate on 32-bits only utilize the lower 5 bits
of the value as the shift amount. Thus it is easy to see that a
-16 is equivalent to 0 when dealing with shifts. Thus it becomes
possible to change all (32 - num_bits) into simply numValidBits.
#endif



// getbits context
// NOTE: Keep byte cached so that address calc can be avoided
typedef struct gbContext
{
    int numValidBits;
    // NOTE it is essential that bitCache be uint so that >> fills with zeros from the left and << fills with zeros from the right
    uint32 bitCache; // FIX if machine has native/fast 64-bit type, that is much better and solves a whole lot of issues
    uint8 *bbData;   // cached for speed purposes
    int bbDataSize;  // cached for speed purposes
    int bbByteIndex; // will track upto bbDataSize
    unsigned int bbFlags;
    unsigned int bbPacketSize;
    unsigned int bbFilepos;
    bitBuffer_t *bb;
    bitBuffer_t *(*getNextBitBuffer)(struct gbContext *gbc);
    uint8 flush; // Serialized flush request (e.g. at EOS)
    uint8 exit; // No more data will arrive

    uint64_t display_timestamp;
    uint64_t display_duration;

    char checkBytesForPattern;
    char nalState; // 0=not enabled, 1=search for start code, 2=search for end pattern
    uint32 nalBytes;
    uint32 byteIndexInNAL; // h.264 BytesInNALUnit processing or vc-1 BDU
    uint32 nalSize; // h.264 BytesInNALUnit processing or vc-1 BDU

    // For GB_DYNAMIC_NALSIZE
    int nextNalNumValidBits;
    uint32 nextNalBitCache;
    uint32 nextNalByteIndexInNAL;
    char nextNalState;

} gbContext;

#define GB_NALSTATE_DISABLED    0 // not enabled
#define GB_NALSTATE_START       1 // search for start code
#define GB_NALSTATE_END         2 // search for end pattern

// Check for basic build issues
#ifdef GB_DYNAMIC_NALSIZE
#ifndef GB_CHECK_NALSIZE
#error "need GB_CHECK_NALSIZE"
#endif
#endif

#ifdef GB_EMULATION_PREVENTION_THREE_BYTE
#ifndef GB_CHECK_BYTES_FOR_PATTERN
#error "need GB_CHECK_BYTES_FOR_PATTERN"
#endif
#endif

#ifndef GBLOG
#define GBLOG(...) printf(__VA_ARGS__)
#endif

#ifdef DEBUG
#define CHECK_NUMVALIDBITS() ASSERTLT(gbc->numValidBits, -32, errBadBitsRequest, ;, "bad gbContext numValid=%d\n", gbc->numValidBits)
#else
#define CHECK_NUMVALIDBITS()
#endif

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
static inline void gb_attach_bitBuffer (gbContext *gbc, bitBuffer_t *bb)
{
    gbc->bb = bb;
    if (bb)
    {
        gbc->bbByteIndex  = 0;
        gbc->bbData       = bb->data;
        gbc->bbDataSize   = bb->dataSize;
        gbc->bbFlags      = bb->flags;
        gbc->bbPacketSize = bb->packetSize;
        gbc->bbFilepos    = bb->filepos;
        // These must be sticky, stay set once set...
        if (bb->flags & BB_FLAGS_FLUSH) gbc->flush = 1;
        if (bb->flags & BB_FLAGS_EXIT) gbc->exit = 1;
        if (gbc->bbFlags & (BB_FLAGS_FLUSH|BB_FLAGS_EXIT)) { gbc->bbFlags = 0; }
    }
    // if bb is NULL, do not modify as it helps in detecting END conditions
}

//////////////////////////////////////////////////////////////////////
static inline void gb_fill_bitCache (gbContext *gbc, int num_bits)
{
    // A *numValidBits* > 24 means no room for another BYTE in bitCache

    while (gbc->numValidBits <= /*num_bits*/24)
    {
        uint32 byte;

        //printf("gb_fill_bitCache() %08X nv %d, bi %d, bs %d, ns %d, ni %d\n", gbc->bitCache, gbc->numValidBits, gbc->bbByteIndex, gbc->bbDataSize, gbc->nalSize, gbc->byteIndexInNAL);

#ifdef GB_CHECK_NALSIZE
        if (gbc->nalSize && (gbc->byteIndexInNAL >= gbc->nalSize))
        {
            break;
        }
#endif

        if (gbc->bbByteIndex >= gbc->bbDataSize)
        {
            // last few bits in bitBuffer are exhausted
            if ((num_bits > gbc->numValidBits) && !gbc->flush && !gbc->exit)
            {
                bitBuffer_t *bb;
                bb = gbc->getNextBitBuffer(gbc);
                gb_attach_bitBuffer(gbc, bb);
                if ((bb == NULL) || gbc->flush || gbc->exit)
                {
#ifdef GB_DYNAMIC_NALSIZE
                    if (gbc->nalState)
                    {
                        gbc->nalSize = gbc->byteIndexInNAL;
                        gbc->nalState = GB_NALSTATE_DISABLED;
                    }
#endif
                    break;
                }
            }
            else
            {
                break;
            }
        }

        // update bitCache with latest byte
        byte = gbc->bbData[gbc->bbByteIndex++];
        ++gbc->byteIndexInNAL;

#ifdef GB_DEBUG_DATA
        printf("BYTE[%5d]: %02x\n", gbc->bbByteIndex-1, byte);
#endif

#ifdef GB_CHECK_BYTES_FOR_PATTERN
        if (gbc->checkBytesForPattern)
        {
            uint32 pattern;

            gbc->nalBytes <<= 8;
            gbc->nalBytes |= byte;
            pattern = gbc->nalBytes & 0x00FFFFFF;

#ifdef GB_JPEG
            pattern = gbc->nalBytes & 0x0000FFFF;
            if (pattern == 0x00FF00)
            {
                /* Skip stuffing byte */
                continue;
            }
#ifdef MAE2
            { extern void jpeg_mae_byte(unsigned char); jpeg_mae_byte(byte); }
#endif
#endif

#ifdef GB_DYNAMIC_NALSIZE
            if ((gbc->nalState == GB_NALSTATE_START) && (pattern == 0x000001))
            {
                // found a start code
                gbc->byteIndexInNAL = 3;
                gbc->numValidBits = 16; // there is a +=8 below for a total of 24
                gbc->nalState = GB_NALSTATE_END;
            }
            else
            if ((gbc->nalState == GB_NALSTATE_END) && ((pattern == 0x000000) || (pattern == 0x000001)))
            {
                gbc->nalSize = gbc->byteIndexInNAL - 3;
                gbc->numValidBits -= 24;
                byte = 0x00;
                gbc->nalState = GB_NALSTATE_DISABLED;

                gbc->nextNalNumValidBits = 24;
                gbc->nextNalBitCache = (pattern << (32 - 24));
                gbc->nextNalByteIndexInNAL = 3;

                if (pattern == 0x000000)
                {
                    // ended this NAL with zeros padding, start anew looking for start code
                    gbc->nextNalState = GB_NALSTATE_START;
                }
                else
                {
                    // ended this NAL with the next start code, which is needed by next NAL
                    gbc->nextNalState = GB_NALSTATE_END;
                }
            }
#endif

#ifdef GB_EMULATION_PREVENTION_THREE_BYTE
            /*
             * Emulation_prevention_three_byte removal can happen either in
             * demux, or here. For this to occur in demux, that means demux
             * must *copy* the NAL (sans emulation_prevention_three_bytes)
             * into a new buffer/RBSP. This is an expensive operation, so
             * the work is done here, without a copy. This is not any more
             * expensive since the work has to be done somewhere. The fact
             * that emulation_prevention_three_byte removal has to be done
             * at all is what is so expensive.
             */
            if (pattern == 0x000003)
            {
                /* skip emulation_prevention_three_byte */
                continue;
            }
#endif

        }
#endif

        gbc->numValidBits += 8;
        gbc->bitCache |= ((uint32)byte << (32 - gbc->numValidBits));
    }

    // When exiting, bitCache contains at least 24 bits (unless at the
    // very end of bitBuffer stream.
}

//////////////////////////////////////////////////////////////////////
static inline void gb_zero_gbc (gbContext *gbc, bitBuffer_t *(*getNextBitBuffer)(struct gbContext *gbc))
{
    uint32 *p = (uint32 *)gbc;
    int i;

    for (i = 0; i < (sizeof(gbContext)/sizeof(uint32)); ++i)
        *p++ = 0;

    gbc->getNextBitBuffer = getNextBitBuffer;
}

static inline void gb_init_gbc (gbContext *gbc, bitBuffer_t *bb)
{
    gbc->numValidBits = 0;
    gbc->bitCache = 0;
    gbc->bbData = NULL;
    gbc->bbDataSize = 0;
    gbc->bbFlags = 0;
    gbc->bbPacketSize = 0;
    gbc->bbByteIndex = 0;
    gbc->bb = NULL;
    gb_attach_bitBuffer(gbc, bb);
    gbc->checkBytesForPattern = 0;
    gbc->nalState = GB_NALSTATE_DISABLED;
    gbc->nalBytes = ~0;
    gbc->byteIndexInNAL = 0;
    gbc->nalSize = 0;

    // Originally there was a call to gb_fill_bitCache() here, but this
    // can not be done now since the gbContext are/can be initialized in a
    // common spot (e.g. vcodec.c), away from where getbits routines are
    // actually used (e.g. in the decoders). The result of this was that
    // the common initialization code (e.g. vcodec.c) would fill the
    // bitcache in a manner inconsistent with what was intended with
    // various #ifdef's specified by the consumers (e.g. decoders)
    // gb_fill_bitCache() routine, thus causing problems.
    //gb_fill_bitCache(gbc, 32);
}

static inline void gb_init_local_gbc (gbContext *gbc, uint8 *buffer, int length, bitBuffer_t *(*getNextBitBuffer)(struct gbContext *gbc))
{
    // Initialize GBC used in a very local scope; no bitBuffers, just an array of bytes
    gb_zero_gbc(gbc, getNextBitBuffer);
    gb_init_gbc(gbc, NULL);
    gbc->bbData = buffer;
    gbc->bbDataSize = length;
    gbc->bbFlags = BB_FLAGS_PACKET | BB_FLAGS_PACKETSTART | BB_FLAGS_PACKETEND;
}

//////////////////////////////////////////////////////////////////////
static inline void gb_set_pattern (gbContext *gbc, int checkBytesForPattern)
{
    gbc->checkBytesForPattern = checkBytesForPattern;
}

static inline void gb_set_nalsize (gbContext *gbc, int nalSize)
{
    gbc->nalSize = nalSize;
    gbc->byteIndexInNAL = (gbc->numValidBits + 7) / 8;
}

static inline void gb_start_dynamic_nalsize (gbContext *gbc)
{
    gbc->nalState = GB_NALSTATE_START;
}

static inline void gb_stop_dynamic_nalsize (gbContext *gbc)
{
    gbc->nalState = GB_NALSTATE_DISABLED;
}

static inline void gb_reset_dynamic_nalsize (gbContext *gbc)
{
    gbc->numValidBits = gbc->nextNalNumValidBits;
    gbc->bitCache = gbc->nextNalBitCache;
    gbc->byteIndexInNAL = gbc->nextNalByteIndexInNAL;
	if (gbc->nextNalState == GB_NALSTATE_DISABLED)
	{
		gbc->nextNalState = GB_NALSTATE_START;
	}
    gbc->nalState = gbc->nextNalState;
    gbc->nalSize = 0;
    gbc->nextNalNumValidBits = 0;
    gbc->nextNalBitCache = 0;
    gbc->nextNalByteIndexInNAL = 0;
    gbc->nextNalState = GB_NALSTATE_DISABLED;
}

static inline int gb_bytes_in_nal (gbContext *gbc, int reset)
{
    int bytes_in_nal;

    bytes_in_nal = gbc->byteIndexInNAL;

    if (reset) gbc->byteIndexInNAL = 0;

    return bytes_in_nal;
}

//////////////////////////////////////////////////////////////////////
static inline uint32 gb_fast_read_bits1(gbContext *gbc)
{
    uint32 bits;

    bits = (gbc->bitCache >> (32 - 1));

    gbc->numValidBits -= 1;
    gbc->bitCache <<= 1;

    CHECK_NUMVALIDBITS();

    return bits;
}

//////////////////////////////////////////////////////////////////////
static inline uint32 gb_slow_read_bits1(gbContext *gbc)
{
    uint32 bits;

    gb_fill_bitCache(gbc, 1);

    bits = (gbc->bitCache >> (32 - 1));

    gbc->numValidBits -= 1;
    gbc->bitCache <<= 1;

    CHECK_NUMVALIDBITS();

    return bits;
}

//////////////////////////////////////////////////////////////////////
static inline uint32 gb_fast_read_bits8(gbContext *gbc)
{
    uint32 bits;

    bits = (gbc->bitCache >> (32 - 8));

    gbc->numValidBits -= 8;
    gbc->bitCache <<= 8;

    CHECK_NUMVALIDBITS();

    return bits;
}

//////////////////////////////////////////////////////////////////////
static inline uint32 gb_slow_read_bits8(gbContext *gbc)
{
    uint32 bits;

    gb_fill_bitCache(gbc, 8);

    bits = (gbc->bitCache >> (32 - 8));

    gbc->numValidBits -= 8;
    gbc->bitCache <<= 8;

    CHECK_NUMVALIDBITS();

    return bits;
}

//////////////////////////////////////////////////////////////////////
static inline uint32 gb_fast_read_bits (gbContext *gbc, int num_bits)
{
    uint32 bits;

    bits = (gbc->bitCache >> (32 - num_bits));

    gbc->numValidBits -= num_bits;
    gbc->bitCache <<= num_bits;

    CHECK_NUMVALIDBITS();

    return bits;
}

//////////////////////////////////////////////////////////////////////
static inline uint32 gb_slow_read_bits (gbContext *gbc, int num_bits)
{
    uint32 bits;

    gb_fill_bitCache(gbc, num_bits);
    bits = (gbc->bitCache >> (32 - num_bits));

    gbc->numValidBits -= num_bits;
    gbc->bitCache <<= num_bits;

    if (num_bits == 32) gbc->bitCache = 0;

    CHECK_NUMVALIDBITS();

    return bits;
}

//////////////////////////////////////////////////////////////////////
static inline uint32 gb_slow_read_bits32 (gbContext *gbc, int num_bits)
{
    // This works with any value of num_bits up to 32 (even 0).
    uint32 bits = 0;

    while (num_bits-- > 0)
    {
        bits <<= 1;
        bits |= gb_slow_read_bits1(gbc);
    }

    CHECK_NUMVALIDBITS();

    return bits;
}

//////////////////////////////////////////////////////////////////////
static inline uint32 gb_fast_next_bits (gbContext *gbc, int num_bits)
{
    // Do NOT touch, else GCC inlining performance worsens
    return (uint32)(gbc->bitCache >> (32 - num_bits));
}

//////////////////////////////////////////////////////////////////////
static inline uint32 gb_slow_next_bits (gbContext *gbc, int num_bits)
{
    // Do NOT touch, else GCC inlining performance worsens
    gb_fill_bitCache(gbc, num_bits);
    return (uint32)(gbc->bitCache >> (32 - num_bits));
}

//////////////////////////////////////////////////////////////////////
static inline uint32 gb_peek_bits (gbContext *gbc, int num_bits)
{
    // Do NOT touch, else GCC inlining performance worsens
    return (uint32)(gbc->bitCache >> (32 - num_bits));
}

//////////////////////////////////////////////////////////////////////
static inline void gb_skip_bits (gbContext *gbc, int num_bits)
{
    gbc->numValidBits -= num_bits;
    gbc->bitCache <<= num_bits;
}

//////////////////////////////////////////////////////////////////////
static inline int gb_byte_aligned (gbContext *gbc)
{
    // numValidBits values of 32, 24, 16 and 8 (and  0) indicate byte aligned
    return ((gbc->numValidBits & 0x7) == 0);
}

//////////////////////////////////////////////////////////////////////
static inline int gb_bits_to_bytealign (gbContext *gbc)
{
    return (gbc->numValidBits & 7);
}

//////////////////////////////////////////////////////////////////////
static inline int gb_bytes_in_bitCache (gbContext *gbc)
{
    return (gbc->numValidBits + 7) >> 3;
}

//////////////////////////////////////////////////////////////////////

/*
 * The FLUSH and EXIT in-line indicators arrive in *separate*
 * bitBuffer of their own (i.e. not with any normal data
 * bitBuffers). In order for the decoder to see these, these
 * bitBuffers must be pulled into the gbContext. One can not
 * rely on the decoder to always pull in the FLUSH or EXIT
 * bitBuffers into the gbContext (i.e. the decoder could end
 * consumption on exactly the last data byte, and therefore
 * never pull in the next bitBuffer). Thus the gb_transition()
 * exists as a graceful method of pulling the next bitBuffer,
 * if appropriate, so the gb_exit() and gb_flush() macros
 * can work properly.
 */
#define gb_transition(gbc) gb_fill_bitCache(gbc, 32)

// an EXIT or FLUSH is pending
#define gb_no_more_data(gbc) ((int)((gbc)->exit || (gbc)->flush))

// gbc.flush set by bitBuffer with BB_FLAGS_FLUSH flag set
#define gb_flush(gbc) ((gbc)->flush)
#define gb_flush_complete(gbc) (gbc)->flush = 0

// gbc.exit set by bitBuffer with BB_FLAGS_EXIT flag set
#define gb_exit(gbc) ((gbc)->exit)

// (last bitBuffer of packet) && (all bytes of bitBuffer in gbContext) && (no bits left in bitCache)
#define gb_end_of_packet(gbc) (((gbc)->bbFlags & BB_FLAGS_PACKETEND) && ((gbc)->bbByteIndex >= (gbc)->bbDataSize) && ((gbc)->numValidBits < 8))

// (NAL size is known) && (all bytes of NAL in gbContext) && (no bits left in bitCache)
#define gb_end_of_nal(gbc) (((gbc)->nalSize) && ((gbc)->byteIndexInNAL >= (gbc)->nalSize) && ((gbc)->numValidBits < 8))

// Determine if bitstream is presented in packets or raw [elementary stream]
#define gb_packetized(gbc) ((gbc)->bbFlags & BB_FLAGS_PACKET)

//////////////////////////////////////////////////////////////////////
static inline unsigned int gb_byte_position (gbContext *gbc)
{
    // Compute approximate byte offset of current bit. This is
    // approximate since with bitCache, can be off by up to 3
    // bytes, and those 3 bytes could have come from a non-
    // contiguous bitBuffer.
    unsigned int pos;

    pos = gbc->bbFilepos;
    pos += gbc->bbByteIndex;
    pos -= gb_bytes_in_bitCache(gbc);

    return pos;
}

//////////////////////////////////////////////////////////////////////

#endif // _GETBITS_H

