#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "yuv2rgb_c.h"
#define DEFAULT_INV_TAB	5
const int32_t Inverse_Table_6_9[8][4] = {
{117504, 138453, 13954, 34903}, /* no sequence_display_extension */
{117504, 138453, 13954, 34903}, /* ITU-R Rec. 709 (1990) */
{104597, 132201, 25675, 53279}, /* unspecified */
{104597, 132201, 25675, 53279}, /* reserved */
{104448, 132798, 24759, 53109}, /* FCC */
{104597, 132201, 25675, 53279}, /* ITU-R Rec. 624-4 System B, G */
{104597, 132201, 25675, 53279}, /* SMPTE 170M */
{117579, 136230, 16907, 35559}  /* SMPTE 240M (1987) */
};

void *g_table_start = 0;
uint8_t *g_table_rV[256];
uint8_t *g_table_gU[256];
int g_table_gV[256];
uint8_t *g_table_bU[256];

static int div_round (int dividend, int divisor)
{
    if (dividend > 0)
        return (dividend + (divisor>>1)) / divisor;
    else
        return -((-dividend + (divisor>>1)) / divisor);
}

int yuv2rgb_c_uninit()
{
	if (g_table_start)
	{
		free(g_table_start);
		g_table_start = 0;
	}
	return 0;
}

int yuv2rgb_c_init(int isRgb, int bpp, int fullRange, int brightness, int contrast, int saturation)
{
    int i;
    int entry_size = 0;
    void *table_r = 0, *table_g = 0, *table_b = 0;
    void *table_start;
	uint32_t *table_32 = 0;
    uint16_t *table_16 = 0;
    uint8_t *table_8 = 0;

    uint8_t table_Y[1024];
	
    int64_t crv =  Inverse_Table_6_9[DEFAULT_INV_TAB][0];
    int64_t cbu =  Inverse_Table_6_9[DEFAULT_INV_TAB][1];
    int64_t cgu = -Inverse_Table_6_9[DEFAULT_INV_TAB][2];
    int64_t cgv = -Inverse_Table_6_9[DEFAULT_INV_TAB][3];
    int64_t cy  = 1<<16;
    int64_t oy  = 0;

    if (!fullRange){
        cy= (cy*255) / 219;
        oy= 16<<16;
    }else{
        crv= (crv*224) / 255;
        cbu= (cbu*224) / 255;
        cgu= (cgu*224) / 255;
        cgv= (cgv*224) / 255;
    }
	
    cy = (cy *contrast             )>>16;
    crv= (crv*contrast * saturation)>>32;
    cbu= (cbu*contrast * saturation)>>32;
    cgu= (cgu*contrast * saturation)>>32;
    cgv= (cgv*contrast * saturation)>>32;
    oy -= 256*brightness;
	
    for (i = 0; i < 1024; i++) 
	{
        int j;
		
        j= (cy*(((i - 384)<<16) - oy) + (1<<31))>>32;
        j = (j < 0) ? 0 : ((j > 255) ? 255 : j);
        table_Y[i] = j;
    }
    switch (bpp) 
	{
		case 32:
			table_start= table_32 = malloc ((197 + 2*682 + 256 + 132) * sizeof (uint32_t));
			
			entry_size = sizeof (uint32_t);
			table_r = table_32 + 197;
			table_b = table_32 + 197 + 685;
			table_g = table_32 + 197 + 2*682;
			
			for (i = -197; i < 256+197; i++)
				((uint32_t *)table_r)[i] = table_Y[i+384] << (isRgb ? 16 : 0);
			for (i = -132; i < 256+132; i++)
				((uint32_t *)table_g)[i] = table_Y[i+384] << 8;
			for (i = -232; i < 256+232; i++)
				((uint32_t *)table_b)[i] = table_Y[i+384] << (isRgb ? 0 : 16);
			break;
			
		case 24:
			table_start= table_8 = malloc ((256 + 2*232) * sizeof (uint8_t));
			
			entry_size = sizeof (uint8_t);
			table_r = table_g = table_b = table_8 + 232;
			
			for (i = -232; i < 256+232; i++)
				((uint8_t * )table_b)[i] = table_Y[i+384];
			break;
			
		case 15:
		case 16:
			table_start= table_16 = malloc ((197 + 2*682 + 256 + 132) * sizeof (uint16_t));
			
			entry_size = sizeof (uint16_t);
			table_r = table_16 + 197;
			table_b = table_16 + 197 + 685;
			table_g = table_16 + 197 + 2*682;
			
			for (i = -197; i < 256+197; i++) {
				int j = table_Y[i+384] >> 3;
				
				if (isRgb)
					j <<= ((bpp==16) ? 11 : 10);
				
				((uint16_t *)table_r)[i] = j;
			}
			for (i = -132; i < 256+132; i++) {
				int j = table_Y[i+384] >> ((bpp==16) ? 2 : 3);
				
				((uint16_t *)table_g)[i] = j << 5;
			}
			for (i = -232; i < 256+232; i++) {
				int j = table_Y[i+384] >> 3;
				
				if (!isRgb)
					j <<= ((bpp==16) ? 11 : 10);
				
				((uint16_t *)table_b)[i] = j;
			}
			break;
		default:
			table_start= NULL;
			//free mem?
			return -1;
    }
	
    for (i = 0; i < 256; i++) 
	{
        g_table_rV[i] = (uint8_t *)table_r + entry_size * div_round (crv * (i-128), 76309);
        g_table_gU[i] = (uint8_t *)table_g + entry_size * div_round (cgu * (i-128), 76309);
        g_table_gV[i] = entry_size * div_round (cgv * (i-128), 76309);
        g_table_bU[i] = (uint8_t *)table_b + entry_size * div_round (cbu * (i-128), 76309);
    }
	g_table_start = table_start;
	return 0;	
}

int yuv2rgb_c_32(uint8_t* src[], int srcStride[], int srcSliceY, int srcSliceH, uint8_t* dst[], int dstStride[], int dst_w)
{
    int y;
    for (y=0; y<srcSliceH; y+=2)
	{
        uint32_t *dst_1= (uint32_t*)(dst[0] + (y+srcSliceY  )*dstStride[0]);
        uint32_t *dst_2= (uint32_t*)(dst[0] + (y+srcSliceY+1)*dstStride[0]);
        uint32_t *r, *b;
        uint32_t *g;
        uint8_t *py_1= src[0] + y*srcStride[0];
        uint8_t *py_2= py_1 + srcStride[0];
        uint8_t *pu= src[1] + (y>>1)*srcStride[1];
        uint8_t *pv= src[2] + (y>>1)*srcStride[2];
        unsigned int h_size= dst_w>>3;
        while (h_size--) 
		{
            int U, V;
            int Y;
			// 0
			U = pu[0];
			V = pv[0];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_1[0];
			dst_1[0] = r[Y] + g[Y] + b[Y];
			Y = py_1[1];
			dst_1[1] = r[Y] + g[Y] + b[Y];
			
			Y = py_2[0];
			dst_2[0] = r[Y] + g[Y] + b[Y];
			Y = py_2[1];
			dst_2[1] = r[Y] + g[Y] + b[Y];
			
			//1
			U = pu[1];
			V = pv[1];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_2[2];
			dst_2[2] = r[Y] + g[Y] + b[Y];
			Y = py_2[2+1];
			dst_2[2+1] = r[Y] + g[Y] + b[Y];
			
			Y = py_1[2];
			dst_1[2] = r[Y] + g[Y] + b[Y];
			Y = py_1[2+1];
			dst_1[2+1] = r[Y] + g[Y] + b[Y];
			
			//2
			U = pu[2];
			V = pv[2];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_1[2*2];
			dst_1[2*2] = r[Y] + g[Y] + b[Y];
			Y = py_1[2*2+1];
			dst_1[2*2+1] = r[Y] + g[Y] + b[Y];
			
			Y = py_2[2*2];
			dst_2[2*2] = r[Y] + g[Y] + b[Y];
			Y = py_2[2*2+1];
			dst_2[2*2+1] = r[Y] + g[Y] + b[Y];
			
			//3
			U = pu[3];
			V = pv[3];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_2[2*3];
			dst_2[2*3] = r[Y] + g[Y] + b[Y];
			Y = py_2[2*3+1];
			dst_2[2*3+1] = r[Y] + g[Y] + b[Y];
			
			Y = py_1[2*3];
			dst_1[2*3] = r[Y] + g[Y] + b[Y];
			Y = py_1[2*3+1];
			dst_1[2*3+1] = r[Y] + g[Y] + b[Y];
			
			
            pu += 4;
            pv += 4;
            py_1 += 8;
            py_2 += 8;
            dst_1 += 8;
            dst_2 += 8;
        }
        if (dst_w & 4) 
		{
            int U, V;
            int Y;
			// 0
			U = pu[0];
			V = pv[0];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_1[0];
			dst_1[0] = r[Y] + g[Y] + b[Y];
			Y = py_1[1];
			dst_1[1] = r[Y] + g[Y] + b[Y];
			
			Y = py_2[0];
			dst_2[0] = r[Y] + g[Y] + b[Y];
			Y = py_2[1];
			dst_2[1] = r[Y] + g[Y] + b[Y];
			
			//1
			U = pu[1];
			V = pv[1];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_2[2];
			dst_2[2] = r[Y] + g[Y] + b[Y];
			Y = py_2[2+1];
			dst_2[2+1] = r[Y] + g[Y] + b[Y];
			
			Y = py_1[2];
			dst_1[2] = r[Y] + g[Y] + b[Y];
			Y = py_1[2+1];
			dst_1[2+1] = r[Y] + g[Y] + b[Y];
		}
	}
	return srcSliceH;
}

int yuv2rgb_c_24_rgb(uint8_t* src[], int srcStride[], int srcSliceY, int srcSliceH, uint8_t* dst[], int dstStride[], int dst_w)
{
    int y;
    for (y=0; y<srcSliceH; y+=2)
	{
        uint8_t *dst_1= (uint8_t*)(dst[0] + (y+srcSliceY  )*dstStride[0]);
        uint8_t *dst_2= (uint8_t*)(dst[0] + (y+srcSliceY+1)*dstStride[0]);
        uint8_t *r, *b;
        uint8_t *g;
        uint8_t *py_1= src[0] + y*srcStride[0];
        uint8_t *py_2= py_1 + srcStride[0];
        uint8_t *pu= src[1] + (y>>1)*srcStride[1];
        uint8_t *pv= src[2] + (y>>1)*srcStride[2];
        unsigned int h_size = dst_w>>3;
        while (h_size--) 
		{
            int U, V;
            int Y;
			// 0
			U = pu[0];
			V = pv[0];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];

			Y = py_1[0];
			dst_1[0] = r[Y]; dst_1[1] = g[Y]; dst_1[2] = b[Y];
			Y = py_1[1];
			dst_1[3] = r[Y]; dst_1[4] = g[Y]; dst_1[5] = b[Y];
			
			Y = py_2[0];
			dst_2[0] = r[Y]; dst_2[1] = g[Y]; dst_2[2] = b[Y];
			Y = py_2[1];
			dst_2[3] = r[Y]; dst_2[4] = g[Y]; dst_2[5] = b[Y];
			
			//1
			U = pu[1];
			V = pv[1];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];

			Y = py_2[2];
			dst_2[6] = r[Y]; dst_2[6+1] = g[Y]; dst_2[6+2] = b[Y];
			Y = py_2[2+1];
			dst_2[6+3] = r[Y]; dst_2[6+4] = g[Y]; dst_2[6+5] = b[Y];
			
			Y = py_1[2];
			dst_1[6] = r[Y]; dst_1[6+1] = g[Y]; dst_1[6+2] = b[Y];
			Y = py_1[2+1];
			dst_1[6+3] = r[Y]; dst_1[6+4] = g[Y]; dst_1[6+5] = b[Y];
			
		
			//2
			U = pu[2];
			V = pv[2];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_1[2*2];
			dst_1[6*2] = r[Y]; dst_1[6*2+1] = g[Y]; dst_1[6*2+2] = b[Y];
			Y = py_1[2*2+1];
			dst_1[6*2+3] = r[Y]; dst_1[6*2+4] = g[Y]; dst_1[6*2+5] = b[Y];
			
			Y = py_2[2*2];
			dst_2[6*2] = r[Y]; dst_2[6*2+1] = g[Y]; dst_2[6*2+2] = b[Y];
			Y = py_2[2*2+1];
			dst_2[6*2+3] = r[Y]; dst_2[6*2+4] = g[Y]; dst_2[6*2+5] = b[Y];
			
			//3
			U = pu[3];
			V = pv[3];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
		
			Y = py_2[2*3];
			dst_2[6*3] = r[Y]; dst_2[6*3+1] = g[Y]; dst_2[6*3+2] = b[Y];
			Y = py_2[2*3+1];
			dst_2[6*3+3] = r[Y]; dst_2[6*3+4] = g[Y]; dst_2[6*3+5] = b[Y];
			
			Y = py_1[2*3];
			dst_1[6*3] = r[Y]; dst_1[6*3+1] = g[Y]; dst_1[6*3+2] = b[Y];
			Y = py_1[2*3+1];
			dst_1[6*3+3] = r[Y]; dst_1[6*3+4] = g[Y]; dst_1[6*3+5] = b[Y];
			
			
            pu += 4;
            pv += 4;
            py_1 += 8;
            py_2 += 8;
            dst_1 += 24;
            dst_2 += 24;
        }
        if (dst_w & 4) 
		{
            int U, V;
            int Y;
			// 0
			U = pu[0];
			V = pv[0];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_1[0];
			dst_1[0] = r[Y]; dst_1[1] = g[Y]; dst_1[2] = b[Y];
			Y = py_1[1];
			dst_1[3] = r[Y]; dst_1[4] = g[Y]; dst_1[5] = b[Y];
			
			Y = py_2[0];
			dst_2[0] = r[Y]; dst_2[1] = g[Y]; dst_2[2] = b[Y];
			Y = py_2[1];
			dst_2[3] = r[Y]; dst_2[4] = g[Y]; dst_2[5] = b[Y];
			
			//1
			U = pu[1];
			V = pv[1];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_2[2];
			dst_2[6] = r[Y]; dst_2[6+1] = g[Y]; dst_2[6+2] = b[Y];
			Y = py_2[2+1];
			dst_2[6+3] = r[Y]; dst_2[6+4] = g[Y]; dst_2[6+5] = b[Y];
			
			Y = py_1[2];
			dst_1[6] = r[Y]; dst_1[6+1] = g[Y]; dst_1[6+2] = b[Y];
			Y = py_1[2+1];
			dst_1[6+3] = r[Y]; dst_1[6+4] = g[Y]; dst_1[6+5] = b[Y];
		}
	}
	return srcSliceH;
}

int yuv2rgb_c_24_bgr(uint8_t* src[], int srcStride[], int srcSliceY, int srcSliceH, uint8_t* dst[], int dstStride[], int dst_w)
{
    int y;
    for (y=0; y<srcSliceH; y+=2)
	{
        uint8_t *dst_1= (uint8_t*)(dst[0] + (y+srcSliceY  )*dstStride[0]);
        uint8_t *dst_2= (uint8_t*)(dst[0] + (y+srcSliceY+1)*dstStride[0]);
        uint8_t *r, *b;
        uint8_t *g;
        uint8_t *py_1= src[0] + y*srcStride[0];
        uint8_t *py_2= py_1 + srcStride[0];
        uint8_t *pu= src[1] + (y>>1)*srcStride[1];
        uint8_t *pv= src[2] + (y>>1)*srcStride[2];
        unsigned int h_size= dst_w>>3;
        while (h_size--) 
		{
            int U, V;
            int Y;
			// 0
			U = pu[0];
			V = pv[0];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];

			Y = py_1[0];
			dst_1[0] = b[Y]; dst_1[1] = g[Y]; dst_1[2] = r[Y];
			Y = py_1[1];
			dst_1[3] = b[Y]; dst_1[4] = g[Y]; dst_1[5] = r[Y];
			
			Y = py_2[0];
			dst_2[0] = b[Y]; dst_2[1] = g[Y]; dst_2[2] = r[Y];
			Y = py_2[1];
			dst_2[3] = b[Y]; dst_2[4] = g[Y]; dst_2[5] = r[Y];
			
			// 1
			U = pu[1];
			V = pv[1];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_2[2];
			dst_2[6] = b[Y]; dst_2[6+1] = g[Y]; dst_2[6+2] = r[Y];
			Y = py_2[2+1];
			dst_2[6+3] = b[Y]; dst_2[6+4] = g[Y]; dst_2[6+5] = r[Y];
			
			Y = py_1[2];
			dst_1[6] = b[Y]; dst_1[6+1] = g[Y]; dst_1[6+2] = r[Y];
			Y = py_1[2+1];
			dst_1[6+3] = b[Y]; dst_1[6+4] = g[Y]; dst_1[6+5] = r[Y];
			
			// 2
			U = pu[2];
			V = pv[2];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];

			Y = py_1[2*2];
			dst_1[6*2] = b[Y]; dst_1[6*2+1] = g[Y]; dst_1[6*2+2] = r[Y];
			Y = py_1[2*2+1];
			dst_1[6*2+3] = b[Y]; dst_1[6*2+4] = g[Y]; dst_1[6*2+5] = r[Y];
			
			Y = py_2[2*2];
			dst_2[6*2] = b[Y]; dst_2[6*2+1] = g[Y]; dst_2[6*2+2] = r[Y];
			Y = py_2[2*2+1];
			dst_2[6*2+3] = b[Y]; dst_2[6*2+4] = g[Y]; dst_2[6*2+5] = r[Y];
			
			// 3
			U = pu[3];
			V = pv[3];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_2[2*3];
			dst_2[6*3] = b[Y]; dst_2[6*3+1] = g[Y]; dst_2[6*3+2] = r[Y];
			Y = py_2[2*3+1];
			dst_2[6*3+3] = b[Y]; dst_2[6*3+4] = g[Y]; dst_2[6*3+5] = r[Y];
			
			Y = py_1[2*3];
			dst_1[6*3] = b[Y]; dst_1[6*3+1] = g[Y]; dst_1[6*3+2] = r[Y];
			Y = py_1[2*3+1];
			dst_1[6*3+3] = b[Y]; dst_1[6*3+4] = g[Y]; dst_1[6*3+5] = r[Y];
			
            pu += 4;
            pv += 4;
            py_1 += 8;
            py_2 += 8;
            dst_1 += 24;
            dst_2 += 24;
        }
        if (dst_w & 4) 
		{
            int U, V;
            int Y;
			// 0
			U = pu[0];
			V = pv[0];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_1[0];
			dst_1[0] = b[Y]; dst_1[1] = g[Y]; dst_1[2] = r[Y];
			Y = py_1[1];
			dst_1[3] = b[Y]; dst_1[4] = g[Y]; dst_1[5] = r[Y];
			
			Y = py_2[0];
			dst_2[0] = b[Y]; dst_2[1] = g[Y]; dst_2[2] = r[Y];
			Y = py_2[1];
			dst_2[3] = b[Y]; dst_2[4] = g[Y]; dst_2[5] = r[Y];
			
			// 1
			U = pu[1];
			V = pv[1];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_2[2];
			dst_2[6] = b[Y]; dst_2[6+1] = g[Y]; dst_2[6+2] = r[Y];
			Y = py_2[2+1];
			dst_2[6+3] = b[Y]; dst_2[6+4] = g[Y]; dst_2[6+5] = r[Y];
			
			Y = py_1[2];
			dst_1[6] = b[Y]; dst_1[6+1] = g[Y]; dst_1[6+2] = r[Y];
			Y = py_1[2+1];
			dst_1[6+3] = b[Y]; dst_1[6+4] = g[Y]; dst_1[6+5] = r[Y];
        }
    }
    return srcSliceH;
}

int yuv2rgb_c_16(uint8_t* src[], int srcStride[], int srcSliceY, int srcSliceH, uint8_t* dst[], int dstStride[], int dst_w)
{
    int y;
    for (y=0; y<srcSliceH; y+=2)
	{
        uint16_t *dst_1= (uint16_t*)(dst[0] + (y+srcSliceY  )*dstStride[0]);
        uint16_t *dst_2= (uint16_t*)(dst[0] + (y+srcSliceY+1)*dstStride[0]);
        uint16_t *r, *b;
        uint16_t *g;
        uint8_t *py_1= src[0] + y*srcStride[0];
        uint8_t *py_2= py_1 + srcStride[0];
        uint8_t *pu= src[1] + (y>>1)*srcStride[1];
        uint8_t *pv= src[2] + (y>>1)*srcStride[2];
        unsigned int h_size = dst_w>>3;
        while (h_size--) 
		{
            int U, V;
            int Y;
			// 0
			U = pu[0];
			V = pv[0];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_1[0];
			dst_1[0] = r[Y] + g[Y] + b[Y];
			Y = py_1[1];
			dst_1[1] = r[Y] + g[Y] + b[Y];
			
			Y = py_2[0];
			dst_2[0] = r[Y] + g[Y] + b[Y];
			Y = py_2[1];
			dst_2[1] = r[Y] + g[Y] + b[Y];
			
			//1
			U = pu[1];
			V = pv[1];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_2[2];
			dst_2[2] = r[Y] + g[Y] + b[Y];
			Y = py_2[2+1];
			dst_2[2+1] = r[Y] + g[Y] + b[Y];
			
			Y = py_1[2];
			dst_1[2] = r[Y] + g[Y] + b[Y];
			Y = py_1[2+1];
			dst_1[2+1] = r[Y] + g[Y] + b[Y];
			
			//2
			U = pu[2];
			V = pv[2];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_1[2*2];
			dst_1[2*2] = r[Y] + g[Y] + b[Y];
			Y = py_1[2*2+1];
			dst_1[2*2+1] = r[Y] + g[Y] + b[Y];
			
			Y = py_2[2*2];
			dst_2[2*2] = r[Y] + g[Y] + b[Y];
			Y = py_2[2*2+1];
			dst_2[2*2+1] = r[Y] + g[Y] + b[Y];
			
			//3
			U = pu[3];
			V = pv[3];
			r = (void *)g_table_rV[V];
			g = (void *)(g_table_gU[U] + g_table_gV[V]);
			b = (void *)g_table_bU[U];
			
			Y = py_2[2*3];
			dst_2[2*3] = r[Y] + g[Y] + b[Y];
			Y = py_2[2*3+1];
			dst_2[2*3+1] = r[Y] + g[Y] + b[Y];
			
			Y = py_1[2*3];
			dst_1[2*3] = r[Y] + g[Y] + b[Y];
			Y = py_1[2*3+1];
			dst_1[2*3+1] = r[Y] + g[Y] + b[Y];
			
            pu += 4;
            pv += 4;
            py_1 += 8;
            py_2 += 8;
            dst_1 += 8;
            dst_2 += 8;
        }
	}
	return srcSliceH;
}