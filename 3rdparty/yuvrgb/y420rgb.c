#include <string.h>
#include <stdlib.h>
#include "y420rgbAPI.h"
#include "y420rgb.h"

int I420_RGB_Init(void ** pHandle, CC_RECT * src_rect, int * src_pitch, CC_RECT * dst_rect, CC_RECT * dst_dis_rect, int BPP)
{
    PSOFTCONV pconv = 0;
    if (!pHandle)
       return -1;
    *pHandle = malloc(sizeof(SOFTCONV));
    if (!(*pHandle))
       return -1;
    pconv =  *pHandle;  
    memset(pconv,0, sizeof(SOFTCONV));
    memcpy(&(pconv->src_rect),src_rect,sizeof(CC_RECT));
    memcpy(&(pconv->dst_rect),dst_rect,sizeof(CC_RECT));
    memcpy(&(pconv->dst_dis_rect),dst_dis_rect,sizeof(CC_RECT));
	pconv->src_pitch[0] = src_pitch[0];
	pconv->src_pitch[1] = src_pitch[1];
	pconv->src_pitch[2] = src_pitch[2];
	
    pconv->pCB = malloc(WIDTHOF(dst_rect) * sizeof(int) + 512);
    pconv->pOB = malloc(8192 + 512);
	pconv->pConvBuffer = (void *)((((intptr_t)pconv->pCB + 255) >> 8) << 8) ;
	pconv->pOffsetBuffer = (void *)((((intptr_t)pconv->pOB + 255) >> 8) << 8);
	
    pconv->i_source_margin = src_pitch[0] - WIDTHOF(src_rect);
    pconv->i_source_margin_c = src_pitch[1] - WIDTHOF(src_rect) / 2;
    pconv->i_right_margin = (WIDTHOF(dst_rect) -  WIDTHOF(dst_dis_rect)) * BPP;
    pconv->dst_pitch = WIDTHOF(dst_rect) * BPP;
	pconv->dst_offset = pconv->dst_pitch * dst_dis_rect->top + dst_dis_rect->left * BPP;
    SetOffset( WIDTHOF(src_rect),
			  HEIGHTOF(src_rect),
			  WIDTHOF(dst_dis_rect),
			  HEIGHTOF(dst_dis_rect),
			  &pconv->b_hscale, &pconv->i_vscale, pconv->pOffsetBuffer );
    return 0;
}

void I420_RGB_unInit(void * pHandle)
{
     PSOFTCONV pconv = pHandle;
     if (!pHandle)
        return;
     if (pconv->pCB)
     {
        free(pconv->pCB);
        pconv->pCB = 0;
     }
     if (pconv->pOB)
     {
        free(pconv->pOB);
        pconv->pOB = 0;
     }
     free(pHandle);
     pHandle = 0;
}

/*****************************************************************************
 * SetOffset: build offset array for conversion functions
 *****************************************************************************
 * This function will build an offset array used in later conversion functions.
 * It will also set horizontal and vertical scaling indicators.
 *****************************************************************************/
void SetOffset( int i_width, int i_height, int i_pic_width,
                       int i_pic_height, int *pb_hscale,
                       unsigned int *pi_vscale, int *p_offset )
{
    int i_x;                                    /* x position in destination */
    int i_scale_count;                                     /* modulo counter */

    /*
     * Prepare horizontal offset array
     */
    if( i_pic_width - i_width == 0 )
    {
        /* No horizontal scaling: YUV conversion is done directly to picture */
        *pb_hscale = 0;
    }
    else if( i_pic_width - i_width > 0 )
    {
        /* Prepare scaling array for horizontal extension */
        *pb_hscale = 1;
        i_scale_count = i_pic_width;
        for( i_x = i_width; i_x--; )
        {
            while( (i_scale_count -= i_width) > 0 )
            {
                *p_offset++ = 0;
            }
            *p_offset++ = 1;
            i_scale_count += i_pic_width;
        }
    }
    else /* if( i_pic_width - i_width < 0 ) */
    {
        /* Prepare scaling array for horizontal reduction */
        *pb_hscale = 1;
        i_scale_count = i_width;
        for( i_x = i_pic_width; i_x--; )
        {
            *p_offset = 1;
            while( (i_scale_count -= i_pic_width) > 0 )
            {
                *p_offset += 1;
            }
            p_offset++;
            i_scale_count += i_width;
        }
    }

    /*
     * Set vertical scaling indicator
     */
    if( i_pic_height - i_height == 0 )
    {
        *pi_vscale = 0;
    }
    else if( i_pic_height - i_height > 0 )
    {
        *pi_vscale = 1;
    }
    else /* if( i_pic_height - i_height < 0 ) */
    {
        *pi_vscale = -1;
    }
}
