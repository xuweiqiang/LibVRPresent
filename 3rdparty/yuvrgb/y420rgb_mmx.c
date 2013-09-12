#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "y420rgbAPI.h"
#include "y420rgb.h"
#include "y420rgb_mmx.h"

void I420_R5G5B5_MMX(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint16_t *p_d)                                          
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

    if( WIDTHOF(src_rect) & 7 )
    {
        i_rewind = 8 - ( WIDTHOF(src_rect) & 7 );
    }
    else
    {
        i_rewind = 0;
    }

    for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
    {
        p_pic_start = p_pic;
        p_buffer = b_hscale ? p_buffer_start : p_pic;

        for ( i_x = WIDTHOF(src_rect) / 8; i_x--; )
        {
            MMX_CALL (
                MMX_INIT_16
                MMX_YUV_MUL
                MMX_YUV_ADD
                MMX_UNPACK_15
            );
            p_y += 8;
            p_u += 4;
            p_v += 4;
            p_buffer += 8;
        }

        /* Here we do some unaligned reads and duplicate conversions, but
         * at least we have all the pixels */
        if( i_rewind )
        {
            p_y -= i_rewind;
            p_u -= i_rewind >> 1;
            p_v -= i_rewind >> 1;
            p_buffer -= i_rewind;

            MMX_CALL (
                MMX_INIT_16
                MMX_YUV_MUL
                MMX_YUV_ADD
                MMX_UNPACK_15
            );
            p_y += 8;
            p_u += 4;
            p_v += 4;
            p_buffer += 8;
        }
        SCALE_WIDTH;
        SCALE_HEIGHT( 2 );

        p_y += i_source_margin;
        if( i_y % 2 )
        {
            p_u += i_source_margin_c;
            p_v += i_source_margin_c;
        }
    }
    /* re-enable FPU registers */
    MMX_END;
}


void I420_R5G6B5_MMX(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint16_t *p_d)
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

    if( WIDTHOF(src_rect) & 7 )
    {
        i_rewind = 8 - ( WIDTHOF(src_rect) & 7 );
    }
    else
    {
        i_rewind = 0;
    }

    for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
    {
        p_pic_start = p_pic;
        p_buffer = b_hscale ? p_buffer_start : p_pic;

        for ( i_x = WIDTHOF(src_rect) / 8; i_x--; )
        {
            MMX_CALL (
                MMX_INIT_16
                MMX_YUV_MUL
                MMX_YUV_ADD
                MMX_UNPACK_16
            );
            p_y += 8;
            p_u += 4;
            p_v += 4;
            p_buffer += 8;
        }

        /* Here we do some unaligned reads and duplicate conversions, but
         * at least we have all the pixels */
        if( i_rewind )
        {
            p_y -= i_rewind;
            p_u -= i_rewind >> 1;
            p_v -= i_rewind >> 1;
            p_buffer -= i_rewind;

            MMX_CALL (
                MMX_INIT_16
                MMX_YUV_MUL
                MMX_YUV_ADD
                MMX_UNPACK_16
            );
            p_y += 8;
            p_u += 4;
            p_v += 4;
            p_buffer += 8;
        }
        SCALE_WIDTH;
        SCALE_HEIGHT( 2 );

        p_y += i_source_margin;
        if( i_y % 2 )
        {
            p_u += i_source_margin_c;
            p_v += i_source_margin_c;
        }
    }
    /* re-enable FPU registers */
    MMX_END;
}

void I420_A8R8G8B8_MMX(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d)
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

    if( WIDTHOF(src_rect) & 7 )
    {
        i_rewind = 8 - ( WIDTHOF(src_rect) & 7 );
    }
    else
    {
        i_rewind = 0;
    }

    for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
    {
        p_pic_start = p_pic;
        p_buffer = b_hscale ? p_buffer_start : p_pic;

        for ( i_x = WIDTHOF(src_rect) / 8; i_x--; )
        {
            MMX_CALL (
                MMX_INIT_32
                MMX_YUV_MUL
                MMX_YUV_ADD
                MMX_UNPACK_32_ARGB
            );
            p_y += 8;
            p_u += 4;
            p_v += 4;
            p_buffer += 8;
        }

        /* Here we do some unaligned reads and duplicate conversions, but
         * at least we have all the pixels */
        if( i_rewind )
        {
            p_y -= i_rewind;
            p_u -= i_rewind >> 1;
            p_v -= i_rewind >> 1;
            p_buffer -= i_rewind;
            MMX_CALL (
                MMX_INIT_32
                MMX_YUV_MUL
                MMX_YUV_ADD
                MMX_UNPACK_32_ARGB
            );
            p_y += 8;
            p_u += 4;
            p_v += 4;
            p_buffer += 8;
        }
        SCALE_WIDTH;
        SCALE_HEIGHT( 4 );

        p_y += i_source_margin;
        if( i_y % 2 )
        {
            p_u += i_source_margin_c;
            p_v += i_source_margin_c;
        }
    }

    /* re-enable FPU registers */
    MMX_END;
}

void I420_R8G8B8A8_MMX(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d)
{
    PSOFTCONV pconv = pHandle;
    if (!pHandle)
		return;
    int  b_hscale = pconv->b_hscale;                         /* horizontal scaling type */
    unsigned int i_vscale =  pconv->i_vscale;                          /* vertical scaling type */
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

    if( WIDTHOF(src_rect) & 7 )
    {
        i_rewind = 8 - ( WIDTHOF(src_rect) & 7 );
    }
    else
    {
        i_rewind = 0;
    }

    for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
    {
        p_pic_start = p_pic;
        p_buffer = b_hscale ? p_buffer_start : p_pic;

        for ( i_x = WIDTHOF(src_rect) / 8; i_x--; )
        {
            MMX_CALL (
                MMX_INIT_32
                MMX_YUV_MUL
                MMX_YUV_ADD
                MMX_UNPACK_32_RGBA
            );
            p_y += 8;
            p_u += 4;
            p_v += 4;
            p_buffer += 8;
        }

        /* Here we do some unaligned reads and duplicate conversions, but
         * at least we have all the pixels */
        if( i_rewind )
        {
            p_y -= i_rewind;
            p_u -= i_rewind >> 1;
            p_v -= i_rewind >> 1;
            p_buffer -= i_rewind;
            MMX_CALL (
                MMX_INIT_32
                MMX_YUV_MUL
                MMX_YUV_ADD
                MMX_UNPACK_32_RGBA
            );
            p_y += 8;
            p_u += 4;
            p_v += 4;
            p_buffer += 8;
        }
        SCALE_WIDTH;
        SCALE_HEIGHT( 4 );

        p_y += i_source_margin;
        if( i_y % 2 )
        {
            p_u += i_source_margin_c;
            p_v += i_source_margin_c;
        }
    }

    /* re-enable FPU registers */
    MMX_END;
}

void I420_B8G8R8A8_MMX(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d)
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

    if( WIDTHOF(src_rect) & 7 )
    {
        i_rewind = 8 - ( WIDTHOF(src_rect) & 7 );
    }
    else
    {
        i_rewind = 0;
    }

    for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
    {
        p_pic_start = p_pic;
        p_buffer = b_hscale ? p_buffer_start : p_pic;

        for ( i_x = WIDTHOF(src_rect) / 8; i_x--; )
        {
            MMX_CALL (
                MMX_INIT_32
                MMX_YUV_MUL
                MMX_YUV_ADD
                MMX_UNPACK_32_BGRA
            );
            p_y += 8;
            p_u += 4;
            p_v += 4;
            p_buffer += 8;
        }

        /* Here we do some unaligned reads and duplicate conversions, but
         * at least we have all the pixels */
        if( i_rewind )
        {
            p_y -= i_rewind;
            p_u -= i_rewind >> 1;
            p_v -= i_rewind >> 1;
            p_buffer -= i_rewind;
            MMX_CALL (
                MMX_INIT_32
                MMX_YUV_MUL
                MMX_YUV_ADD
                MMX_UNPACK_32_BGRA
            );
            p_y += 8;
            p_u += 4;
            p_v += 4;
            p_buffer += 8;
        }
        SCALE_WIDTH;
        SCALE_HEIGHT( 4 );

        p_y += i_source_margin;
        if( i_y % 2 )
        {
            p_u += i_source_margin_c;
            p_v += i_source_margin_c;
        }
    }

    /* re-enable FPU registers */
    MMX_END;  
}

void I420_A8B8G8R8_MMX(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d)
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

    if( WIDTHOF(src_rect) & 7 )
    {
        i_rewind = 8 - ( WIDTHOF(src_rect) & 7 );
    }
    else
    {
        i_rewind = 0;
    }

    for( i_y = 0; i_y < HEIGHTOF(src_rect); i_y++ )
    {
        p_pic_start = p_pic;
        p_buffer = b_hscale ? p_buffer_start : p_pic;

        for ( i_x = WIDTHOF(src_rect) / 8; i_x--; )
        {
            MMX_CALL (
                MMX_INIT_32
                MMX_YUV_MUL
                MMX_YUV_ADD
                MMX_UNPACK_32_ABGR
            );
            p_y += 8;
            p_u += 4;
            p_v += 4;
            p_buffer += 8;
        }

        /* Here we do some unaligned reads and duplicate conversions, but
         * at least we have all the pixels */
        if( i_rewind )
        {
            p_y -= i_rewind;
            p_u -= i_rewind >> 1;
            p_v -= i_rewind >> 1;
            p_buffer -= i_rewind;
            MMX_CALL (
                MMX_INIT_32
                MMX_YUV_MUL
                MMX_YUV_ADD
                MMX_UNPACK_32_ABGR
            );
            p_y += 8;
            p_u += 4;
            p_v += 4;
            p_buffer += 8;
        }
        SCALE_WIDTH;
        SCALE_HEIGHT( 4 );

        p_y += i_source_margin;
        if( i_y % 2 )
        {
            p_u += i_source_margin_c;
            p_v += i_source_margin_c;
        }
    }

    /* re-enable FPU registers */
    MMX_END;
}


