#ifndef __Y420RGB_H__
#define __Y420RGB_H__

typedef struct _tagSoftConversion
{
	CC_RECT src_rect;
	CC_RECT dst_rect;
	CC_RECT dst_dis_rect;
	
	void * pCB;
	void * pOB;
	void * pConvBuffer;
	void * pOffsetBuffer;

	int  b_hscale;                         /* horizontal scaling type */
    unsigned int i_vscale;                          /* vertical scaling type */

    int src_offset;
	int src_pitch[3];
    int dst_offset;
    int dst_pitch;
    int i_source_margin;
    int i_source_margin_c;
    int i_right_margin;
	
} SOFTCONV, *PSOFTCONV;


#define WIDTHOF(x) (x->right - x->left)
#define HEIGHTOF(x) (x->bottom - x->top)



/*****************************************************************************
 * SCALE_WIDTH: scale a line horizontally
 *****************************************************************************
 * This macro scales a line using rendering buffer and offset array. It works
 * for 1, 2 and 4 Bpp.
 *****************************************************************************/
#define SCALE_WIDTH                                                           \
    if( b_hscale )                                                            \
    {                                                                         \
        /* Horizontal scaling, conversion has been done to buffer.            \
         * Rewind buffer and offset, then copy and scale line */              \
        p_buffer = p_buffer_start;                                            \
        p_offset = p_offset_start;                                            \
        for( i_x = WIDTHOF(dst_rect) / 16; i_x--; )             \
        {                                                                     \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
        }                                                                     \
        for( i_x = WIDTHOF(dst_rect) & 15; i_x--; )             \
        {                                                                     \
            *p_pic++ = *p_buffer;   p_buffer += *p_offset++;                  \
        }                                                                     \
        p_pic = (void*)((uint8_t*)p_pic + i_right_margin );                   \
    }                                                                         \
    else                                                                      \
    {                                                                         \
        /* No scaling, conversion has been done directly in picture memory.   \
         * Increment of picture pointer to end of line is still needed */     \
        p_pic = (void*)((uint8_t*)p_pic + dst_stride );               \
    }                                                                         \

/*****************************************************************************
 * SCALE_HEIGHT: handle vertical scaling
 *****************************************************************************
 * This macro handle vertical scaling for a picture. CHROMA may be 420, 422 or
 * 444 for RGB conversion, or 400 for gray conversion. It works for 1, 2, 3
 * and 4 Bpp.
 *****************************************************************************/
#define SCALE_HEIGHT( BPP )                                           \
    /* If line is odd, rewind 4:2:0 U and V samples */                        \
    if( !(i_y & 0x1) )                \
    {                                                                         \
        p_u -= i_chroma_width;                                                \
        p_v -= i_chroma_width;                                                \
    }                                                                         \
                                                                              \
    /*                                                                        \
     * Handle vertical scaling. The current line can be copied or next one    \
     * can be ignored.                                                        \
     */                                                                       \
    switch( i_vscale )                                                        \
    {                                                                         \
    case -1:                             /* vertical scaling factor is < 1 */ \
        while( (i_scale_count -= HEIGHTOF(dst_rect)) > 0 )      \
        {                                                                     \
            /* Height reduction: skip next source line */                     \
            p_y += WIDTHOF(src_rect);                            \
            i_y++;                                                            \
                if( i_y & 0x1 )                                               \
                {                                                             \
                    p_u += i_chroma_width;                                    \
                    p_v += i_chroma_width;                                    \
                }                                                             \
        }                                                                     \
        i_scale_count += HEIGHTOF(src_rect);                                  \
        break;                                                                \
    case 1:                              /* vertical scaling factor is > 1 */ \
        while( (i_scale_count -= HEIGHTOF(src_rect)) > 0 )                    \
        {                                                                     \
            /* Height increment: copy previous picture line */                \
            memcpy( p_pic, p_pic_start, WIDTHOF(dst_rect) * BPP );        \
            p_pic = (void*)((uint8_t*)p_pic + dst_stride );                   \
        }                                                                     \
        i_scale_count += HEIGHTOF(dst_rect);                                  \
        break;                                                                \
    }                                                                         \


#ifdef __cplusplus
extern "C" {
#endif
void SetOffset( int i_width, int i_height, int i_pic_width,
                       int i_pic_height, int *pb_hscale,
                       unsigned int *pi_vscale, int *p_offset );

#ifdef __cplusplus
}
#endif

#endif /* __Y420RGB_H__ */
