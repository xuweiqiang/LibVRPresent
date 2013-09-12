
//=============================================================================
//   THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO Cidana (Shanghai).
//   ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//   ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//   COMPANY.
//
//   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//   KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//   IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//   PURPOSE.
//
//   Copyright (c) 2007 - 2008  Cidana (Shanghai) Co., Ltd.  All Rights Reserved.
//
//-----------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#include "dsub_segments.h"
#include "dsub_renderer.h"
#include "dsub_finder.h"


static const unsigned char CLUT_2to8_Table[4] = {
    0x00, 0x77, 0x88, 0xff
};


static const unsigned char CLUT_4to8_Table[16] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};



static MUST_INLINE int BitGot(const unsigned char *data, int index) {
    int byte_idx = index / 8;
    unsigned char mask = 1 << (index & 0x07);
    return (data[byte_idx] & mask) != 0;
}


static MUST_INLINE int NibbleGot(const unsigned char *data, int index) {
    int byte_idx = index / 2;
    int result;
    if (index & 0x01)
        result = data[byte_idx] & 0x0f;
    else
        result = data[byte_idx] >> 4;
    return result;
}


static MUST_INLINE void Pixel2BitDraw(tRenderingContext *cntx, int x,
                                    int y, int length, int line_thickness,
                                    unsigned char pseudo_color) {
    unsigned char pcolor;
    if (pseudo_color == 0x01 && cntx->have_holes)
        return;
    pcolor = cntx->CLUT_2bit_to_8bit_map[pseudo_color];
    cntx->hline_callback(cntx->private_data, x, y, length, line_thickness,
                         pcolor);
}


static MUST_INLINE void Pixel4BitDraw(tRenderingContext *cntx, int x,
                                    int y, int length, int line_thickness,
                                    unsigned char pseudo_color) {
    unsigned char pcolor;
    if (pseudo_color == 0x01 && cntx->have_holes)
        return;
    pcolor = cntx->CLUT_4bit_to_8bit_map[pseudo_color];
    cntx->hline_callback(cntx->private_data, x, y, length, line_thickness,
                         pcolor);
}


static MUST_INLINE void Pixel8BitDraw(tRenderingContext *cntx, int x,
                                    int y, int length, int line_thickness,
                                    unsigned char pseudo_color) {
    if (pseudo_color == 0x01 && cntx->have_holes)
        return;
    cntx->hline_callback(cntx->private_data, x, y, length, line_thickness,
                         pseudo_color);
}


static int Pixel2BitStrDraw(tRenderingContext *cntx,
                                        int *x, int y, int line_thickness,
                                        const unsigned char *data, int length) {
    int idx = 0;

    /* Check position just in case if data is corrupted */
    while (idx/8 < length) {
        int length, color;
        if (BitGot(data, idx) != 0 || BitGot(data, idx + 1) != 0) {
            /* 01 - 11 */
            length = 1;
            color = (BitGot(data, idx) << 1) | BitGot(data, idx + 1);
            idx += 2;
        } else {
            idx += 2;
            if (BitGot(data, idx) == 1) { /* switch_1 */
                /* 00 1L LL CC */
                length = ((BitGot(data, idx + 1) << 2) |
                          (BitGot(data, idx + 2) << 1) |
                          BitGot(data, idx + 3)) + 3;
                color = (BitGot(data, idx + 4) << 1) | BitGot(data, idx + 5);
                idx += 6;
            } else {
                ++idx;
                if (BitGot(data, idx) == 0) { /* switch_2 */
                    ++idx;
                    if (BitGot(data, idx) == 1) { /* switch_3_1 */
                        ++idx;
                        if (BitGot(data, idx) == 0) { /* switch_3_2 */
                            /* 00 00 10 LL LL CC */
                            length = ((BitGot(data, idx + 1) << 3) |
                                      (BitGot(data, idx + 2) << 2) |
                                      (BitGot(data, idx + 3) << 1) |
                                      BitGot(data, idx + 4)) + 12;
                            color = (BitGot(data, idx + 5) << 1) |
                                BitGot(data, idx + 6);
                            idx += 7;
                        } else {
                            /* 00 00 11 LL LL LL LL CC */
                            length = ((BitGot(data, idx + 1) << 7) |
                                      (BitGot(data, idx + 2) << 6) |
                                      (BitGot(data, idx + 3) << 5) |
                                      (BitGot(data, idx + 4) << 4) |
                                      (BitGot(data, idx + 5) << 3) |
                                      (BitGot(data, idx + 6) << 2) |
                                      (BitGot(data, idx + 7) << 1) |
                                      BitGot(data, idx + 8)) + 29;
                            color = (BitGot(data, idx + 9) << 1) |
                                BitGot(data, idx + 10);
                            idx += 11;
                        }
                    } else {
                        ++idx;
                        if (BitGot(data, idx) == 1) {
                            /* 00 00 01 */
                            length = 2;
                            color = 0;
                            ++idx;
                        } else {
                            /* 00 00 00 */
                            return (idx + 1 + 7) / 8;
                        }
                    }
                } else {
                    /* 00 01 */
                    length = 1;
                    color = 0;
                    ++idx;
                }
            }
        }
        Pixel2BitDraw(cntx, *x, y, length, line_thickness, color);
        x += length;
    }
    return length;
}


static int Pixel4BitStrDraw(tRenderingContext *cntx,
                                        int *x, int y, int line_thickness,
                                        const unsigned char *data, int length) {
    int idx = 0;

    int t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;

    /* Check position just in case if data is corrupted */
    while (idx/2 < length) {
        int length, color;
        if (NibbleGot(data, idx) > 0x0) { /* 0001 - 1111 */
            length = 1;
            color = NibbleGot(data, idx);
            ++idx;
            ++t1;
        } else { /* 0000 */
            int nibble = NibbleGot(data, idx + 1);
            idx += 2;
            if (nibble == 0x0) { /* 0000 0000 */
/*                 SUB_DP(( "%d %d %d %d %d %d %d\n", t1, t2, t3, t4, t5, t6, t7)); */
                return (idx + 1)/2;
            } else if (nibble == 0xc) { /* 0000 1100 */
                length = 1;
                color = 0;
                ++t2;
            } else if (nibble == 0xd) { /* 0000 1101 */
                length = 2;
                color = 0;
                ++t3;
            } else if (nibble == 0xe) { /* 0000 1110 LLLL CCCC */
                length = NibbleGot(data, idx) + 9;
                color = NibbleGot(data, idx + 1);
                idx += 2;
                ++t4;
            } else if (nibble == 0xf) { /* 0000 1111 LLLL LLLL CCCC */
                length = (NibbleGot(data, idx) << 4 |
                    NibbleGot(data, idx + 1)) + 25;
                color = NibbleGot(data, idx + 2);
                idx += 3;
                ++t5;
            } else if (nibble & 0x8) { /* 0000 10LL CCCC */
                length = (nibble & 0x3) + 4;
                color = NibbleGot(data, idx);
                ++idx;
                ++t6;
            } else { /* 0000 0LLL */
                length = nibble + 2; /* Specs say +3 */
                color = 0;
                ++t7;
            }
        }
        Pixel4BitDraw(cntx, *x, y, length, line_thickness, color);
        *x += length;
    }
    return length;
}

static int Pixel8BitStrDraw(tRenderingContext *cntx,
                                        int *x, int y, int line_thickness,
                                        const unsigned char *data, int length) {
    int idx = 0;

    /* Check position just in case if data is corrupted */
    while (idx < length) {
        if (data[idx] != 0x00) {
            Pixel8BitDraw(cntx, *x, y, 1, line_thickness, data[idx]);
            ++*x;
        } else {
            ++idx;
            if (data[idx] == 0x00) {
                return idx + 1;
            } else if (data[idx] & 0x80) {
                int length = (data[idx] & 0x7f) + 3;
                Pixel8BitDraw(cntx, *x, y, length, line_thickness,
                                 data[idx + 1]);
                *x += length;
                ++idx;
            } else {
                int length = data[idx] + 1;
                Pixel8BitDraw(cntx, *x, y, length, line_thickness, 0x00);
                *x += length;
            }
        }
        ++idx;
    }
    return length;
}


static int SubBlockPs(tRenderingContext *cntx,
                           int start_x, int y, int line_thickness,
                           const unsigned char *data, int length) {
    int x = start_x;
    int idx = 0;

    memcpy(cntx->CLUT_2bit_to_8bit_map,
           CLUT_2to8_Table, 4);
    memcpy(cntx->CLUT_4bit_to_8bit_map,
           CLUT_4to8_Table, 16);

    while (idx < length) {
        switch (data[idx++]) { /* Note: idx++ */
        case 0x10:
            idx += Pixel2BitStrDraw(cntx, &x, y, line_thickness,
                                                data + idx, length - idx);
            break;
        case 0x11:
            idx += Pixel4BitStrDraw(cntx, &x, y, line_thickness,
                                                data + idx, length - idx);
            break;
        case 0x12:
            idx += Pixel8BitStrDraw(cntx, &x, y, line_thickness,
                                                data + idx, length - idx);
            break;
        case 0x20: /* We don't care about 2 to 4 bit conversions */
            idx += 2;
            break;
        case 0x21: /* 2 to 8 bit */
            cntx->CLUT_2bit_to_8bit_map[0] = data[idx++];
            cntx->CLUT_2bit_to_8bit_map[1] = data[idx++];
            cntx->CLUT_2bit_to_8bit_map[2] = data[idx++];
            cntx->CLUT_2bit_to_8bit_map[3] = data[idx++];
            break;
        case 0x22: { /* 4 to 8 bit */
            int i;
            for (i = 0; i < 16; ++i)
                cntx->CLUT_4bit_to_8bit_map[i] = data[idx++];
            break;
        }
        case 0xf0: /* end of object line code */
            return idx;
        default:
            break;
        }
    }
    return length;
}


static void RenderPixel(tRenderingContext *cntx,
                                     int start_x, int start_y,
                                     const tObjectDataPixels *data) {
    int top_idx = 0, bottom_idx = 0;
    int top_length = data->top_field_data_block_length;
    int bottom_length = data->bottom_field_data_block_length;
    int line_thickness = bottom_length == 0 ? 2 : 1;
    int y = 0;

    while (start_y + y < cntx->region_composition->region_height &&
           (top_idx < top_length || bottom_idx < bottom_length)) 
	{
        if (top_idx < top_length) {
            top_idx += SubBlockPs(cntx, start_x, start_y + y,
                                       line_thickness,
                                       data->sub_blocks + top_idx,
                                       top_length - top_idx);
            y += line_thickness;
        }
        if (bottom_idx < bottom_length) {
            bottom_idx += SubBlockPs(cntx, start_x, start_y + y, 1,
                                          data->sub_blocks + top_length + 
                                          bottom_idx,
                                          bottom_length - bottom_idx);
            ++y;
        }
		
    }

	//cntx->hline_callback(cntx->private_data, 0,  start_y + y, 0, 0, 0, region_complete);
}


void RenderRegion(tRenderingContext *cntx) {
    tObjectDataSeg *ods;
    tObjectDataSeg *ancillary_ods_list=NULL;
    unsigned char is_in_anc=0;
    const tRegionCompositionSeg *rcs = cntx->region_composition;
	//int region_complete = 0;     //[Repco] to handle: two or more objects in one region, then we send the whole region pixel in one time.
    int i;
    for (i = 0; i < rcs->number_of_regions; ++i) {
        const tRegionCompositionRegion *region = &(rcs->regions[i]);
        if (region->object_provider_flag !=
            OBJECT_PROVIDER_SUBTITLING_STREAM)
            continue;
        if(cntx->display_set->ancillary_display_set){
            ancillary_ods_list=cntx->display_set->ancillary_display_set->object_data_segs;
        }
        ods = ObjectDataSegFind(cntx->display_set->object_data_segs,ancillary_ods_list,&is_in_anc,
                                region->object_id);
        if (ods == NULL)
            continue;

        cntx->have_holes = ods->non_modifying_colour_flag;

		///*[Repco]merge objects into one region*/
		//if (rcs->number_of_regions == 1)
		//	region_complete = 1;
		//else
		//	if (i == rcs->number_of_regions - 1)
		//		region_complete = 1;


        switch (ods->object_coding_method) {
        case OBJECT_CODING_PIXELS:
            if (cntx->hline_callback)
			{
                RenderPixel(cntx,
                             region->object_horizontal_position,
                             region->object_vertical_position,
                             &ods->data.pixels);
				//printf("Render region: horizontal_position = %d, vertical_position = %d\n",region->object_horizontal_position, region->object_vertical_position);
			}
            break;
        case OBJECT_CODING_STRING:
            if (cntx->string_callback)
                cntx->string_callback(cntx->private_data,
                                      region->object_horizontal_position,
                                      region->object_vertical_position,
                                      rcs->region_width - region->object_horizontal_position,
                                      rcs->region_height - region->object_vertical_position,
                                      ods->data.string.character_codes,
                                      ods->data.string.number_of_codes,
                                      region->foreground_pixel_code,
                                      region->background_pixel_code);
            break;
        }
    }
}


static void ColorMapCreate(const tRenderingContext *cntx) {
    tCLUT_DefinitionSeg *cds;
    int i;
    cds = cntx->display_set->CLUT_definition_segs;
    while (cds != NULL) {
        if (cds->CLUT_id == cntx->region_composition->CLUT_id) {
            for (i = 0; i < cds->number_of_CLUT_entries; ++i) {
                if (cds->CLUT_entries[i].CLUT_entry_8bit_flag) {
                    int pcolor = cds->CLUT_entries[i].CLUT_entry_id;               
                    cntx->set_color_callback(cntx->private_data, pcolor,
                                             cds->CLUT_entries[i].Y_value,
                                             cds->CLUT_entries[i].Cb_value,
                                             cds->CLUT_entries[i].Cr_value,
                                             cds->CLUT_entries[i].T_value);
 //                   break;
                }
                 if (cds->CLUT_entries[i].CLUT_entry_4bit_flag) {
                    int pcolor = cds->CLUT_entries[i].CLUT_entry_id;
//                     SUB_DP(( "CLUT map 4 bit%3d: (%3d, %3d, %3d, %3d) \n", 
//                             pcolor, cds->CLUT_entries[i].Y_value, cds->CLUT_entries[i].Cb_value, cds->CLUT_entries[i].Cr_value, cds->CLUT_entries[i].T_value));                     
                    cntx->set_color_callback(cntx->private_data, CLUT_4to8_Table[pcolor],
                                             cds->CLUT_entries[i].Y_value,
                                             cds->CLUT_entries[i].Cb_value,
                                             cds->CLUT_entries[i].Cr_value,
                                             cds->CLUT_entries[i].T_value);
 //                   break;
                }
            }
            break;
        }
        cds = cds->next_seg;
    }
}


tRenderingContext*
CreateRender(const tDisplaySet *display_set,
                                unsigned char region_id,
                                DrawHLine_CB hline_callback,
                                DrawString_CB string_callback,
                                ColorSet_CB set_color_callback,
                                void *priv) {
    tRenderingContext *cntx;
    tRegionCompositionSeg *rcs =
        RegionCompositionSegFind(display_set->region_composition_segs, region_id);
    if (rcs == NULL)
        return NULL;

    cntx = (tRenderingContext*)malloc(sizeof(tRenderingContext));
    if (cntx == NULL)
        return NULL;

    cntx->hline_callback = hline_callback;
    cntx->string_callback = string_callback;
    cntx->set_color_callback = set_color_callback;
    cntx->private_data = priv;

    cntx->display_set = display_set;
    cntx->region_composition = rcs;

    if (cntx->set_color_callback)
        ColorMapCreate(cntx);
    cntx->magic=DSUB_MODULE_MAGIC;
    return cntx;
}


void ReleaseRender(tRenderingContext *cntx) {
    free(cntx);
}



int RegionBackGroundColorGet(const tRegionCompositionSeg *region) {
    int bkcolor;
    if (region == NULL)
        bkcolor = -1;
    else if (region->region_fill_flag) {
        switch (region->region_depth) {
        case 0x01:
            bkcolor = CLUT_2to8_Table[region->region_2bit_pixel_code];
            break;
        case 0x02:
            bkcolor = CLUT_4to8_Table[region->region_4bit_pixel_code];
            break;
        case 0x03:
            bkcolor = region->region_8bit_pixel_code;
            break;
        default:
            bkcolor = -1;
        }
    } else
        bkcolor = -1;

    return bkcolor;
}


