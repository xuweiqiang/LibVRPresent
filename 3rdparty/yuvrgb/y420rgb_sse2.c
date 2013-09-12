#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "y420rgbAPI.h"
#include "y420rgb.h"
#include "y420rgb_sse2.h"

void I420_R5G5B5_SSE2(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint16_t *p_d)                                          
{
     PSOFTCONV pconv = pHandle;
    if (!pHandle)
		return;
    int  b_hscale = pconv->b_hscale;                         /* horizontal scaling type */
    unsigned int i_vscale = pconv->i_vscale;                          /* vertical scaling type */
    unsigned int i_x, i_y;                /* horizontal and vertical indexes */
	CC_RECT * dst_rect = &pconv->dst_dis_rect;
	CC_RECT * src_rect = &pconv->src_rect;
    int dst_stride    = pconv->dst_pitch;

	uint16_t *p_pic = (uint16_t *)((uint8_t *)p_d + pconv->dst_offset);
    int         i_right_margin = pconv->i_right_margin;
    int         i_rewind;
    int         i_scale_count;                       /* scale modulo counter */
    int         i_chroma_width = WIDTHOF(src_rect) / 2; /* chroma width */
    uint16_t *  p_pic_start;       /* beginning of the current line for copy */

    /* Conversion buffer pointer */
    uint16_t *  p_buffer_start = pconv->pConvBuffer;
    uint16_t *  p_buffer;

    /* Offset array pointer */
    int *       p_offset_start = pconv->pOffsetBuffer;
    int *       p_offset;

    const int i_source_margin = pconv->i_source_margin;
    const int i_source_margin_c = pconv->i_source_margin_c;

    /*
     * Perform conversion
     */
    i_scale_count = ( i_vscale == 1 ) ?
                    HEIGHTOF(dst_rect) :
                    HEIGHTOF(src_rect);


    if( WIDTHOF(src_rect) & 15 )
    {
        i_rewind = 16 - ( WIDTHOF(src_rect) & 15 );
    }
    else
    {
        i_rewind = 0;
    }

    /*
    ** SSE2 128 bits fetch/store instructions are faster
    ** if memory access is 16 bytes aligned
    */

    p_buffer = b_hscale ? p_buffer_start : p_pic;
    if( 0 == (15 & (WIDTHOF(src_rect)|
                    (WIDTHOF(dst_rect) * 2)|
                    ((intptr_t)p_y)|
                    ((intptr_t)p_buffer))) )
    {
        /* use faster SSE2 aligned fetch and store */
        for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
        {
            p_pic_start = p_pic;

            for ( i_x = WIDTHOF(src_rect)/16; i_x--; )
            {
                SSE2_CALL (
                    SSE2_INIT_16_ALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_15_ALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
                p_buffer += 16;
            }
            /* Here we do some unaligned reads and duplicate conversions, but
             * at least we have all the pixels */
            if( i_rewind )
            {
                p_y -= i_rewind;
                p_u -= i_rewind >> 1;
                p_v -= i_rewind >> 1;
                p_buffer -= i_rewind;

                SSE2_CALL (
                    SSE2_INIT_16_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_15_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
            }
            SCALE_WIDTH;
            SCALE_HEIGHT( 2 );

            p_y += i_source_margin;
            if( i_y % 2 )
            {
                p_u += i_source_margin_c;
                p_v += i_source_margin_c;
            }
            p_buffer = b_hscale ? p_buffer_start : p_pic;
        }
    }
    else
    {
        /* use slower SSE2 unaligned fetch and store */
        for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
        {
            p_pic_start = p_pic;
            p_buffer = b_hscale ? p_buffer_start : p_pic;

            for ( i_x = WIDTHOF(src_rect)/16; i_x--; )
            {
                SSE2_CALL (
                    SSE2_INIT_16_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_15_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
                p_buffer += 16;
            }
            /* Here we do some unaligned reads and duplicate conversions, but
             * at least we have all the pixels */
            if( i_rewind )
            {
                p_y -= i_rewind;
                p_u -= i_rewind >> 1;
                p_v -= i_rewind >> 1;
                p_buffer -= i_rewind;

                SSE2_CALL (
                    SSE2_INIT_16_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_15_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
            }
            SCALE_WIDTH;
            SCALE_HEIGHT( 2 );

            p_y += i_source_margin;
            if( i_y % 2 )
            {
                p_u += i_source_margin_c;
                p_v += i_source_margin_c;
            }
            p_buffer = b_hscale ? p_buffer_start : p_pic;
        }
    }

    /* make sure all SSE2 stores are visible thereafter */
    SSE2_END;
}

void I420_R5G6B5_SSE2(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint16_t *p_d)
{
     PSOFTCONV pconv = pHandle;
    if (!pHandle)
		return;
    int  b_hscale = pconv->b_hscale;                         /* horizontal scaling type */
    unsigned int i_vscale = pconv->i_vscale;                          /* vertical scaling type */
    unsigned int i_x, i_y;                /* horizontal and vertical indexes */
	CC_RECT * dst_rect = &pconv->dst_dis_rect;
	CC_RECT * src_rect = &pconv->src_rect;
    int dst_stride    = pconv->dst_pitch;
	uint16_t *p_pic = (uint16_t *)((uint8_t *)p_d + pconv->dst_offset);
    int         i_right_margin = pconv->i_right_margin;
    int         i_rewind;
    int         i_scale_count;                       /* scale modulo counter */
    int         i_chroma_width = WIDTHOF(src_rect) / 2; /* chroma width */
    uint16_t *  p_pic_start;       /* beginning of the current line for copy */

    /* Conversion buffer pointer */
    uint16_t *  p_buffer_start = pconv->pConvBuffer;
    uint16_t *  p_buffer;

    /* Offset array pointer */
    int *       p_offset_start = pconv->pOffsetBuffer;
    int *       p_offset;

    const int i_source_margin = pconv->i_source_margin;
    const int i_source_margin_c = pconv->i_source_margin_c;


    /*
     * Perform conversion
     */
    i_scale_count = ( i_vscale == 1 ) ?
                    HEIGHTOF(dst_rect) :
                    HEIGHTOF(src_rect);


    if( WIDTHOF(src_rect) & 15 )
    {
        i_rewind = 16 - ( WIDTHOF(src_rect) & 15 );
    }
    else
    {
        i_rewind = 0;
    }

    /*
    ** SSE2 128 bits fetch/store instructions are faster
    ** if memory access is 16 bytes aligned
    */

    p_buffer = b_hscale ? p_buffer_start : p_pic;
    if( 0 == (15 & (WIDTHOF(src_rect)|
                    (WIDTHOF(dst_rect) * 2)|
                    ((intptr_t)p_y)|
                    ((intptr_t)p_buffer))) )
    {
        /* use faster SSE2 aligned fetch and store */
        for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
        {
            p_pic_start = p_pic;

            for ( i_x = WIDTHOF(src_rect)/16; i_x--; )
            {
                SSE2_CALL (
                    SSE2_INIT_16_ALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_16_ALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
                p_buffer += 16;
            }
            /* Here we do some unaligned reads and duplicate conversions, but
             * at least we have all the pixels */
            if( i_rewind )
            {
                p_y -= i_rewind;
                p_u -= i_rewind >> 1;
                p_v -= i_rewind >> 1;
                p_buffer -= i_rewind;

                SSE2_CALL (
                    SSE2_INIT_16_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_16_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
            }
            SCALE_WIDTH;
            SCALE_HEIGHT( 2 );

            p_y += i_source_margin;
            if( i_y % 2 )
            {
                p_u += i_source_margin_c;
                p_v += i_source_margin_c;
            }
            p_buffer = b_hscale ? p_buffer_start : p_pic;
        }
    }
    else
    {
        /* use slower SSE2 unaligned fetch and store */
        for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
        {
            p_pic_start = p_pic;
            p_buffer = b_hscale ? p_buffer_start : p_pic;

            for ( i_x = WIDTHOF(src_rect)/16; i_x--; )
            {
                SSE2_CALL(
                    SSE2_INIT_16_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_16_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
                p_buffer += 16;
            }
            /* Here we do some unaligned reads and duplicate conversions, but
             * at least we have all the pixels */
            if( i_rewind )
            {
                p_y -= i_rewind;
                p_u -= i_rewind >> 1;
                p_v -= i_rewind >> 1;
                p_buffer -= i_rewind;

                SSE2_CALL(
                    SSE2_INIT_16_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_16_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
            }
            SCALE_WIDTH;
            SCALE_HEIGHT( 2 );

            p_y += i_source_margin;
            if( i_y % 2 )
            {
                p_u += i_source_margin_c;
                p_v += i_source_margin_c;
            }
            p_buffer = b_hscale ? p_buffer_start : p_pic;
        }
    }

    /* make sure all SSE2 stores are visible thereafter */
    SSE2_END;   
}

void I420_A8R8G8B8_SSE2(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d)
{
     PSOFTCONV pconv = pHandle;
    if (!pHandle)
		return;
    int  b_hscale = pconv->b_hscale;                         /* horizontal scaling type */
    unsigned int i_vscale = pconv->i_vscale;                          /* vertical scaling type */
    unsigned int i_x, i_y;                /* horizontal and vertical indexes */
	CC_RECT * dst_rect = &pconv->dst_dis_rect;
	CC_RECT * src_rect = &pconv->src_rect;
    int dst_stride    = pconv->dst_pitch;
	
	uint32_t *p_pic = (uint32_t *)((uint8_t *)p_d + pconv->dst_offset);
    int         i_right_margin = pconv->i_right_margin;
    int         i_rewind;
    int         i_scale_count;                       /* scale modulo counter */
    int         i_chroma_width = WIDTHOF(src_rect) / 2; /* chroma width */
    uint32_t *  p_pic_start;       /* beginning of the current line for copy */
    /* Conversion buffer pointer */
    uint32_t *  p_buffer_start = pconv->pConvBuffer;
    uint32_t *  p_buffer;

    /* Offset array pointer */
    int *       p_offset_start = pconv->pOffsetBuffer;
    int *       p_offset;

    const int i_source_margin = pconv->i_source_margin;
    const int i_source_margin_c = pconv->i_source_margin_c;


    /*
     * Perform conversion
     */
    i_scale_count = ( i_vscale == 1 ) ?
                    HEIGHTOF(dst_rect) :
                    HEIGHTOF(src_rect);


    if( WIDTHOF(src_rect) & 15 )
    {
        i_rewind = 16 - ( WIDTHOF(src_rect) & 15 );
    }
    else
    {
        i_rewind = 0;
    }

    /*
    ** SSE2 128 bits fetch/store instructions are faster
    ** if memory access is 16 bytes aligned
    */

    p_buffer = b_hscale ? p_buffer_start : p_pic;
    if( 0 == (15 & (WIDTHOF(src_rect)|
                    (WIDTHOF(dst_rect) * 4)|
                    ((intptr_t)p_y)|
                    ((intptr_t)p_buffer))) )
    {
        /* use faster SSE2 aligned fetch and store */
        for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
        {
            p_pic_start = p_pic;

            for ( i_x = WIDTHOF(src_rect) / 16; i_x--; )
            {
                SSE2_CALL (
                    SSE2_INIT_32_ALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_ARGB_ALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
                p_buffer += 16;
            }

            /* Here we do some unaligned reads and duplicate conversions, but
             * at least we have all the pixels */
            if( i_rewind )
            {
                p_y -= i_rewind;
                p_u -= i_rewind >> 1;
                p_v -= i_rewind >> 1;
                p_buffer -= i_rewind;
                SSE2_CALL (
                    SSE2_INIT_32_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_ARGB_UNALIGNED
                );
                p_y += 16;
                p_u += 4;
                p_v += 4;
            }
            SCALE_WIDTH;
            SCALE_HEIGHT( 4 );

            p_y += i_source_margin;
            if( i_y % 2 )
            {
                p_u += i_source_margin_c;
                p_v += i_source_margin_c;
            }
            p_buffer = b_hscale ? p_buffer_start : p_pic;
        }
    }
    else
    {
        /* use slower SSE2 unaligned fetch and store */
        for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
        {
            p_pic_start = p_pic;
            p_buffer = b_hscale ? p_buffer_start : p_pic;

            for ( i_x = WIDTHOF(src_rect) / 16; i_x--; )
            {
                SSE2_CALL (
                    SSE2_INIT_32_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_ARGB_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
                p_buffer += 16;
            }

            /* Here we do some unaligned reads and duplicate conversions, but
             * at least we have all the pixels */
            if( i_rewind )
            {
                p_y -= i_rewind;
                p_u -= i_rewind >> 1;
                p_v -= i_rewind >> 1;
                p_buffer -= i_rewind;
                SSE2_CALL (
                    SSE2_INIT_32_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_ARGB_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
            }
            SCALE_WIDTH;
            SCALE_HEIGHT( 4 );

            p_y += i_source_margin;
            if( i_y % 2 )
            {
                p_u += i_source_margin_c;
                p_v += i_source_margin_c;
            }
            p_buffer = b_hscale ? p_buffer_start : p_pic;
        }
    }

    /* make sure all SSE2 stores are visible thereafter */
    SSE2_END; 
}

void I420_R8G8B8A8_SSE2(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d)
{
     PSOFTCONV pconv = pHandle;
    if (!pHandle)
		return;
    int  b_hscale = pconv->b_hscale;                         /* horizontal scaling type */
    unsigned int i_vscale = pconv->i_vscale;                          /* vertical scaling type */
    unsigned int i_x, i_y;                /* horizontal and vertical indexes */
	CC_RECT * dst_rect = &pconv->dst_dis_rect;
	CC_RECT * src_rect = &pconv->src_rect;
    int dst_stride    = pconv->dst_pitch;
	
	uint32_t *p_pic = (uint32_t *)((uint8_t *)p_d + pconv->dst_offset);
    int         i_right_margin = pconv->i_right_margin;
    int         i_rewind;
    int         i_scale_count;                       /* scale modulo counter */
    int         i_chroma_width = WIDTHOF(src_rect) / 2; /* chroma width */
    uint32_t *  p_pic_start;       /* beginning of the current line for copy */
    /* Conversion buffer pointer */
    uint32_t *  p_buffer_start = pconv->pConvBuffer;
    uint32_t *  p_buffer;

    /* Offset array pointer */
    int *       p_offset_start = pconv->pOffsetBuffer;
    int *       p_offset;

    const int i_source_margin = pconv->i_source_margin;
    const int i_source_margin_c = pconv->i_source_margin_c;


    /*
     * Perform conversion
     */
    i_scale_count = ( i_vscale == 1 ) ?
                    HEIGHTOF(dst_rect) :
                    HEIGHTOF(src_rect);


    if( WIDTHOF(src_rect) & 15 )
    {
        i_rewind = 16 - ( WIDTHOF(src_rect) & 15 );
    }
    else
    {
        i_rewind = 0;
    }

    /*
    ** SSE2 128 bits fetch/store instructions are faster
    ** if memory access is 16 bytes aligned
    */

    p_buffer = b_hscale ? p_buffer_start : p_pic;
    if( 0 == (15 & (WIDTHOF(src_rect)|
                    (WIDTHOF(dst_rect) * 4)|
                    ((intptr_t)p_y)|
                    ((intptr_t)p_buffer))) )
    {
        /* use faster SSE2 aligned fetch and store */
        for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
        {
            p_pic_start = p_pic;

            for ( i_x = WIDTHOF(src_rect) / 16; i_x--; )
            {
                SSE2_CALL (
                    SSE2_INIT_32_ALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_RGBA_ALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
                p_buffer += 16;
            }

            /* Here we do some unaligned reads and duplicate conversions, but
             * at least we have all the pixels */
            if( i_rewind )
            {
                p_y -= i_rewind;
                p_u -= i_rewind >> 1;
                p_v -= i_rewind >> 1;
                p_buffer -= i_rewind;
                SSE2_CALL (
                    SSE2_INIT_32_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_RGBA_UNALIGNED
                );
                p_y += 16;
                p_u += 4;
                p_v += 4;
            }
            SCALE_WIDTH;
            SCALE_HEIGHT( 4 );

            p_y += i_source_margin;
            if( i_y % 2 )
            {
                p_u += i_source_margin_c;
                p_v += i_source_margin_c;
            }
            p_buffer = b_hscale ? p_buffer_start : p_pic;
        }
    }
    else
    {
        /* use slower SSE2 unaligned fetch and store */
        for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
        {
            p_pic_start = p_pic;
            p_buffer = b_hscale ? p_buffer_start : p_pic;

            for ( i_x = WIDTHOF(src_rect) / 16; i_x--; )
            {
                SSE2_CALL (
                    SSE2_INIT_32_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_RGBA_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
                p_buffer += 16;
            }

            /* Here we do some unaligned reads and duplicate conversions, but
             * at least we have all the pixels */
            if( i_rewind )
            {
                p_y -= i_rewind;
                p_u -= i_rewind >> 1;
                p_v -= i_rewind >> 1;
                p_buffer -= i_rewind;
                SSE2_CALL (
                    SSE2_INIT_32_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_RGBA_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
            }
            SCALE_WIDTH;
            SCALE_HEIGHT( 4 );

            p_y += i_source_margin;
            if( i_y % 2 )
            {
                p_u += i_source_margin_c;
                p_v += i_source_margin_c;
            }
            p_buffer = b_hscale ? p_buffer_start : p_pic;
        }
    }

    /* make sure all SSE2 stores are visible thereafter */
    SSE2_END; 
}

void I420_B8G8R8A8_SSE2(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d)
{
     PSOFTCONV pconv = pHandle;
    if (!pHandle)
		return;
    int  b_hscale = pconv->b_hscale;                         /* horizontal scaling type */
    unsigned int i_vscale = pconv->i_vscale;                          /* vertical scaling type */
    unsigned int i_x, i_y;                /* horizontal and vertical indexes */
	CC_RECT * dst_rect = &pconv->dst_dis_rect;
	CC_RECT * src_rect = &pconv->src_rect;
    int dst_stride    = pconv->dst_pitch;
	
	uint32_t *p_pic = (uint32_t *)((uint8_t *)p_d + pconv->dst_offset);
    int         i_right_margin = pconv->i_right_margin;
    int         i_rewind;
    int         i_scale_count;                       /* scale modulo counter */
    int         i_chroma_width = WIDTHOF(src_rect) / 2; /* chroma width */
    uint32_t *  p_pic_start;       /* beginning of the current line for copy */
    /* Conversion buffer pointer */
    uint32_t *  p_buffer_start = pconv->pConvBuffer;
    uint32_t *  p_buffer;

    /* Offset array pointer */
    int *       p_offset_start = pconv->pOffsetBuffer;
    int *       p_offset;

    const int i_source_margin = pconv->i_source_margin;
    const int i_source_margin_c = pconv->i_source_margin_c;

    /*
     * Perform conversion
     */
    i_scale_count = ( i_vscale == 1 ) ?
                    HEIGHTOF(dst_rect) :
                    HEIGHTOF(src_rect);


    if( WIDTHOF(src_rect) & 15 )
    {
        i_rewind = 16 - ( WIDTHOF(src_rect) & 15 );
    }
    else
    {
        i_rewind = 0;
    }

    /*
    ** SSE2 128 bits fetch/store instructions are faster
    ** if memory access is 16 bytes aligned
    */

    p_buffer = b_hscale ? p_buffer_start : p_pic;
    if( 0 == (15 & (WIDTHOF(src_rect)|
                    (WIDTHOF(dst_rect) * 4)|
                    ((intptr_t)p_y)|
                    ((intptr_t)p_buffer))) )
    {
        /* use faster SSE2 aligned fetch and store */
        for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
        {
            p_pic_start = p_pic;

            for ( i_x = WIDTHOF(src_rect) / 16; i_x--; )
            {
                SSE2_CALL (
                    SSE2_INIT_32_ALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_BGRA_ALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
                p_buffer += 16;
            }

            /* Here we do some unaligned reads and duplicate conversions, but
             * at least we have all the pixels */
            if( i_rewind )
            {
                p_y -= i_rewind;
                p_u -= i_rewind >> 1;
                p_v -= i_rewind >> 1;
                p_buffer -= i_rewind;
                SSE2_CALL (
                    SSE2_INIT_32_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_BGRA_UNALIGNED
                );
                p_y += 16;
                p_u += 4;
                p_v += 4;
            }
            SCALE_WIDTH;
            SCALE_HEIGHT( 4 );

            p_y += i_source_margin;
            if( i_y % 2 )
            {
                p_u += i_source_margin_c;
                p_v += i_source_margin_c;
            }
            p_buffer = b_hscale ? p_buffer_start : p_pic;
        }
    }
    else
    {
        /* use slower SSE2 unaligned fetch and store */
        for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
        {
            p_pic_start = p_pic;
            p_buffer = b_hscale ? p_buffer_start : p_pic;

            for ( i_x = WIDTHOF(src_rect) / 16; i_x--; )
            {
                SSE2_CALL (
                    SSE2_INIT_32_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_BGRA_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
                p_buffer += 16;
            }

            /* Here we do some unaligned reads and duplicate conversions, but
             * at least we have all the pixels */
            if( i_rewind )
            {
                p_y -= i_rewind;
                p_u -= i_rewind >> 1;
                p_v -= i_rewind >> 1;
                p_buffer -= i_rewind;
                SSE2_CALL (
                    SSE2_INIT_32_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_BGRA_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
            }
            SCALE_WIDTH;
            SCALE_HEIGHT( 4 );

            p_y += i_source_margin;
            if( i_y % 2 )
            {
                p_u += i_source_margin_c;
                p_v += i_source_margin_c;
            }
            p_buffer = b_hscale ? p_buffer_start : p_pic;
        }
    }
    SSE2_END; 
}

void I420_A8B8G8R8_SSE2(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d)
{
     PSOFTCONV pconv = pHandle;
    if (!pHandle)
		return;
    int  b_hscale = pconv->b_hscale;                         /* horizontal scaling type */
    unsigned int i_vscale = pconv->i_vscale;                          /* vertical scaling type */
    unsigned int i_x, i_y;                /* horizontal and vertical indexes */
	CC_RECT * dst_rect = &pconv->dst_dis_rect;
	CC_RECT * src_rect = &pconv->src_rect;
    int dst_stride    = pconv->dst_pitch;
	
	uint32_t *p_pic = (uint32_t *)((uint8_t *)p_d + pconv->dst_offset);
    int         i_right_margin = pconv->i_right_margin;
    int         i_rewind;
    int         i_scale_count;                       /* scale modulo counter */
    int         i_chroma_width = WIDTHOF(src_rect) / 2; /* chroma width */
    uint32_t *  p_pic_start;       /* beginning of the current line for copy */
    /* Conversion buffer pointer */
    uint32_t *  p_buffer_start = pconv->pConvBuffer;
    uint32_t *  p_buffer;

    /* Offset array pointer */
    int *       p_offset_start = pconv->pOffsetBuffer;
    int *       p_offset;

    const int i_source_margin = pconv->i_source_margin;
    const int i_source_margin_c = pconv->i_source_margin_c;

    /*
     * Perform conversion
     */
    i_scale_count = ( i_vscale == 1 ) ?
                    HEIGHTOF(dst_rect) :
                    HEIGHTOF(src_rect);


    if( WIDTHOF(src_rect) & 15 )
    {
        i_rewind = 16 - ( WIDTHOF(src_rect) & 15 );
    }
    else
    {
        i_rewind = 0;
    }

    /*
    ** SSE2 128 bits fetch/store instructions are faster
    ** if memory access is 16 bytes aligned
    */

    p_buffer = b_hscale ? p_buffer_start : p_pic;
    if( 0 == (15 & (WIDTHOF(src_rect)|
                    (WIDTHOF(dst_rect) * 4)|
                    ((intptr_t)p_y)|
                    ((intptr_t)p_buffer))) )
    {
        /* use faster SSE2 aligned fetch and store */
        for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
        {
            p_pic_start = p_pic;

            for ( i_x = WIDTHOF(src_rect) / 16; i_x--; )
            {
                SSE2_CALL (
                    SSE2_INIT_32_ALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_ABGR_ALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
                p_buffer += 16;
            }

            /* Here we do some unaligned reads and duplicate conversions, but
             * at least we have all the pixels */
            if( i_rewind )
            {
                p_y -= i_rewind;
                p_u -= i_rewind >> 1;
                p_v -= i_rewind >> 1;
                p_buffer -= i_rewind;
                SSE2_CALL (
                    SSE2_INIT_32_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_ABGR_UNALIGNED
                );
                p_y += 16;
                p_u += 4;
                p_v += 4;
            }
            SCALE_WIDTH;
            SCALE_HEIGHT( 4 );

            p_y += i_source_margin;
            if( i_y % 2 )
            {
                p_u += i_source_margin_c;
                p_v += i_source_margin_c;
            }
            p_buffer = b_hscale ? p_buffer_start : p_pic;
        }
    }
    else
    {
        /* use slower SSE2 unaligned fetch and store */
        for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
        {
            p_pic_start = p_pic;
            p_buffer = b_hscale ? p_buffer_start : p_pic;

            for ( i_x = WIDTHOF(src_rect) / 16; i_x--; )
            {
                SSE2_CALL (
                    SSE2_INIT_32_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_ABGR_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
                p_buffer += 16;
            }

            /* Here we do some unaligned reads and duplicate conversions, but
             * at least we have all the pixels */
            if( i_rewind )
            {
                p_y -= i_rewind;
                p_u -= i_rewind >> 1;
                p_v -= i_rewind >> 1;
                p_buffer -= i_rewind;
                SSE2_CALL (
                    SSE2_INIT_32_UNALIGNED
                    SSE2_YUV_MUL
                    SSE2_YUV_ADD
                    SSE2_UNPACK_32_ABGR_UNALIGNED
                );
                p_y += 16;
                p_u += 8;
                p_v += 8;
            }
            SCALE_WIDTH;
            SCALE_HEIGHT( 4 );

            p_y += i_source_margin;
            if( i_y % 2 )
            {
                p_u += i_source_margin_c;
                p_v += i_source_margin_c;
            }
            p_buffer = b_hscale ? p_buffer_start : p_pic;
        }
    }
    SSE2_END;
}
