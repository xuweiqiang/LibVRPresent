
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

#include "dsub_segments.h"

/********************** page composition segment ******************************/

tPageCompositionSeg* PageCompositionSegParse(const unsigned char* segment_data) {
    int i;
    unsigned short pos;
    unsigned short region_compositions_length =
        ((unsigned short)segment_data[4] << 8) + segment_data[5] - 2;

    unsigned short number_of_region_compositions = region_compositions_length / 6;

    tPageCompositionSeg* segment =
        (tPageCompositionSeg*)malloc(sizeof(tPageCompositionSeg) + number_of_region_compositions*sizeof(tPageCompositionRegion));
    if (segment == NULL)
        return NULL;
    segment->page_time_out = segment_data[6];
    segment->page_version_number = segment_data[7] >> 4;
    segment->page_state = (segment_data[7] & 0x0f) >> 2;

    segment->number_of_region_compositions = number_of_region_compositions;

    for (pos=8, i=0; i < number_of_region_compositions; pos+=6, ++i) {
        segment->regions[i].region_id = segment_data[pos];
        segment->regions[i].region_horizontal_address =
            ((unsigned short)segment_data[pos + 2] << 8) | segment_data[pos + 3];
        segment->regions[i].region_vertical_address =
            ((unsigned short)segment_data[pos + 4] << 8) | segment_data[pos + 5];
    }

    return segment;
}


void PageCompositionSegRelease(tPageCompositionSeg* segment) {
    if (segment == NULL)
        return;

    free(segment);
}


tPageCompositionSeg *PageCompositionSegCP(const tPageCompositionSeg* segment) {
    tPageCompositionSeg *s;

    if (segment == NULL)
        return NULL;

    s = (tPageCompositionSeg*)malloc(sizeof(tPageCompositionSeg) + segment->number_of_region_compositions*sizeof(tPageCompositionRegion));
    if (s == NULL)
       return NULL;

       memcpy(s, segment, sizeof(tPageCompositionSeg) + segment->number_of_region_compositions*sizeof(tPageCompositionRegion));

    return s;
}



/********************** CLUT definition segment ******************************/

tCLUT_DefinitionSeg *CLUT_DefinitionSegParse(const unsigned char* segment_data) {
    int i;
    tCLUT_DefinitionSeg *segment=NULL;
    unsigned short CLUT_entries_length =
        ((unsigned short)segment_data[4] << 8) + segment_data[5] - 2;

    int number_of_CLUT_entries = 0;

#if 1    
    unsigned short pos = 8;
    while (pos < 8 + CLUT_entries_length) {
        ++number_of_CLUT_entries;
        if (segment_data[pos + 1] & 0x01)
            pos += 6;
        else
            pos += 4;
    }

    segment = (tCLUT_DefinitionSeg*)malloc(sizeof(tCLUT_DefinitionSeg) + number_of_CLUT_entries*sizeof(tCLUT_Entry));

    if (segment == NULL)
        return NULL;

    segment->next_seg = NULL;
    segment->CLUT_id = segment_data[6];
    segment->CLUT_version_number = segment_data[7] >> 4;
    segment->number_of_CLUT_entries = number_of_CLUT_entries;

    // SUB_DP(( "CLUT definition segment: id=%d, version=%d\n ", segment->CLUT_id, segment->CLUT_version_number));

    for (pos=8, i=0; i < segment->number_of_CLUT_entries; pos+=2, ++i) {
        segment->CLUT_entries[i].CLUT_entry_id = segment_data[pos];
        segment->CLUT_entries[i].CLUT_entry_2bit_flag =
            (segment_data[pos + 1] & 0x80) != 0;
        segment->CLUT_entries[i].CLUT_entry_4bit_flag =
            (segment_data[pos + 1] & 0x40) != 0;
        segment->CLUT_entries[i].CLUT_entry_8bit_flag =
            (segment_data[pos + 1] & 0x20) != 0;
        if (segment_data[pos + 1] & 0x01) { /* full_range_flag */
            segment->CLUT_entries[i].Y_value = segment_data[pos+2];
            segment->CLUT_entries[i].Cr_value = segment_data[pos+3];
            segment->CLUT_entries[i].Cb_value = segment_data[pos+4];
            segment->CLUT_entries[i].T_value = segment_data[pos+5];
            pos += 4;
        } else {

            segment->CLUT_entries[i].Y_value = segment_data[pos+2] & 0xfc;
            segment->CLUT_entries[i].Cr_value = (((segment_data[pos+2] & 3) << 2) | ((segment_data[pos+3] >> 6) & 3)) << 4;
            segment->CLUT_entries[i].Cb_value = (segment_data[pos+3] << 2) & 0xf0;
            segment->CLUT_entries[i].T_value = (segment_data[pos+3]<< 6) & 0xc0;
            pos += 2;
        }
    }

#else

  char *buf=segment_data+8;
  unsigned char *buf_end = buf + CLUT_entries_length+2;
    int clut_id;
    int entry_id, depth , full_range;
    int y, cr, cb, alpha;
    int r, g, b, r_add, g_add, b_add;
        
  while (buf + 4 < buf_end)
    {
        entry_id = *buf++;
        
        depth = (*buf) & 0xe0;
                    
        if (depth == 0) {
            printf( "Invalid clut depth 0x%x!\n", *buf);
            return NULL;
        }
        else if(depth & 0x80){
             number_of_CLUT_entries=256;             
        }
        else if(depth & 0x40){
             number_of_CLUT_entries=16;
        }
       else if(depth & 0x20){
             number_of_CLUT_entries=4;
        }
 
       if (segment == NULL){
           segment = (tCLUT_DefinitionSeg*)malloc(sizeof(tCLUT_DefinitionSeg) + number_of_CLUT_entries*sizeof(tCLUT_Entry));
           if(segment == NULL)
              return NULL;
           else{
            segment->next_seg = NULL;
            segment->CLUT_id = segment_data[6];
            segment->CLUT_version_number = segment_data[7] >> 4;
            segment->number_of_CLUT_entries = number_of_CLUT_entries;
            segment->CLUT_entries[entry_id].CLUT_entry_8bit_flag= ((depth & 0x80) != 0);
            segment->CLUT_entries[entry_id].CLUT_entry_4bit_flag= ((depth & 0x40) != 0);
            segment->CLUT_entries[entry_id].CLUT_entry_2bit_flag= ((depth & 0x20) != 0);
           }
       }
        full_range = (*buf++) & 1;
        
        if (full_range) {
            y = *buf++;
            cr = *buf++;
            cb = *buf++;
            alpha = *buf++;
        } else {
            y = buf[0] & 0xfc;
            cr = (((buf[0] & 3) << 2) | ((buf[1] >> 6) & 3)) << 4;
            cb = (buf[1] << 2) & 0xf0;
            alpha = (buf[1] << 6) & 0xc0;
            
            buf += 2;
        }
        
        if (y == 0)
            alpha = 0xff;
        
         segment->CLUT_entries[entry_id].Y_value =y;
         segment->CLUT_entries[entry_id].Cr_value = cr;
         segment->CLUT_entries[entry_id].Cb_value = cb;
         segment->CLUT_entries[entry_id].T_value = alpha;    
         DPRINTF( " (entry_id=%d, 2bit=%d, 4bit=%d, 8bit=%d, Y=%d, Cr=%d, Cb=%d, T=%d)", segment->CLUT_entries[entry_id].CLUT_entry_id, segment->CLUT_entries[entry_id].CLUT_entry_2bit_flag, segment->CLUT_entries[entry_id].CLUT_entry_4bit_flag, segment->CLUT_entries[entry_id].CLUT_entry_8bit_flag, segment->CLUT_entries[entry_id].Y_value, segment->CLUT_entries[entry_id].Cr_value, segment->CLUT_entries[entry_id].Cb_value, segment->CLUT_entries[entry_id].T_value); 
    }

#endif
    return segment;
}


void CLUT_DefinitionSegRelease(tCLUT_DefinitionSeg* segment) {
    if (segment == NULL)
        return;

    free(segment);
}


tCLUT_DefinitionSeg *CLUT_DefinitionSegCP(const tCLUT_DefinitionSeg* segment) {
    tCLUT_DefinitionSeg *s;

    if (segment == NULL)
        return NULL;

    s = (tCLUT_DefinitionSeg*)malloc(sizeof(tCLUT_DefinitionSeg) + segment->number_of_CLUT_entries*sizeof(tCLUT_Entry));
    if (s == NULL)
        return NULL;

    memcpy(s, segment, sizeof(tCLUT_DefinitionSeg) + segment->number_of_CLUT_entries*sizeof(tCLUT_Entry));

    return s;
}


/********************** region composition segment ******************************/

tRegionCompositionSeg *RegionCompositionSegParse(const unsigned char* segment_data) {
    int i;
    tRegionCompositionSeg *segment;

    unsigned short regions_length =
        ((unsigned short)segment_data[4] << 8) + segment_data[5] - 10;

    int number_of_regions = 0;
    unsigned short pos = 16;
    while (pos < 16 + regions_length) {
        ++number_of_regions;
        if (segment_data[pos + 2] == 0x01 ||
            segment_data[pos + 2] == 0x02)
            pos += 8;
        else
            pos += 6;
    }

/*     tRegionCompositionSeg *segment = */
/*         (tRegionCompositionSeg*)malloc(sizeof(tRegionCompositionSeg)); */

    segment = (tRegionCompositionSeg*)malloc(sizeof(tRegionCompositionSeg) + number_of_regions*sizeof(tRegionCompositionRegion));

    if (segment == NULL)
        return NULL;

    segment->region_id = segment_data[6];
    segment->region_version_number = segment_data[7] >> 4;
    segment->region_fill_flag = (segment_data[7] & 0x08) != 0;
    segment->region_width =
        ((unsigned short)segment_data[8] << 8) | segment_data[9];
    segment->region_height =
        ((unsigned short)segment_data[10] << 8) | segment_data[11];
    segment->region_level_of_compatibility = segment_data[12] >> 5;
    segment->region_depth = (segment_data[12] & 0x0c) >> 2;
    segment->CLUT_id = segment_data[13];
    segment->region_8bit_pixel_code = segment_data[14];
    segment->region_4bit_pixel_code = segment_data[15] >> 4;
    segment->region_2bit_pixel_code = (segment_data[15] & 0x0c) >> 2;
    segment->number_of_regions = number_of_regions;

/*     SUB_DP(( "Region composition segment: region_id=%d, version=%d, fill=%d, w=%d, h=%d, compatibility=%d, depth=%d, CLUT id=%d, 8bit=%d, 4bit=%d, 2bit=%d\n ", segment->region_id, segment->region_version_number, segment->region_fill_flag, segment->region_width, segment->region_height, segment->region_level_of_compatibility, segment->region_depth, segment->CLUT_id, segment->region_8bit_pixel_code, segment->region_4bit_pixel_code, segment->region_2bit_pixel_code)); */

    for (pos=16, i=0; i < segment->number_of_regions; pos+=6, ++i) {
        segment->regions[i].object_id =
            ((unsigned short)segment_data[pos] << 8) | segment_data[pos + 1];
        segment->regions[i].object_type = segment_data[pos + 2] >> 6;
        segment->regions[i].object_provider_flag =
            (segment_data[pos + 2] & 0x30) >> 4;
        segment->regions[i].object_horizontal_position =
            (((unsigned short)segment_data[pos + 2] & 0x0f) << 8) |
            segment_data[pos + 3];
        segment->regions[i].object_vertical_position =
            (((unsigned short)segment_data[pos + 4] & 0x0f) << 8) |
            segment_data[pos + 5];
        if (segment->regions[i].object_type == 0x01 ||
            segment->regions[i].object_type == 0x02) {
            segment->regions[i].foreground_pixel_code = segment_data[pos + 6];
            segment->regions[i].background_pixel_code = segment_data[pos + 7];
            pos += 2;
        } else {
            segment->regions[i].foreground_pixel_code = 0;
            segment->regions[i].background_pixel_code = 0;
        }

    }

    segment->next_seg = NULL;

    return segment;
}


void RegionCompositionSegRelease(tRegionCompositionSeg* segment) {
    if (segment == NULL)
        return;

    free(segment);
}


tRegionCompositionSeg *RegionCompositionSegCP(const tRegionCompositionSeg* segment) {
    tRegionCompositionSeg *s;

    if (segment == NULL)
        return NULL;

    s = (tRegionCompositionSeg*)malloc(sizeof(tRegionCompositionSeg) + segment->number_of_regions*sizeof(tRegionCompositionRegion));
    if (s == NULL)
        return NULL;

    memcpy(s, segment, sizeof(tRegionCompositionSeg) + segment->number_of_regions*sizeof(tRegionCompositionRegion));
    s->next_seg = NULL;

    return s;
}


/********************** object data segment ******************************/


tObjectDataSeg *ObjectDataSegParse(const unsigned char *segment_data) {
    unsigned char object_coding_method = (segment_data[8] & 0x0f) >> 2;
    unsigned char number_of_codes;
    unsigned short top_field_data_block_length, bottom_field_data_block_length;
    unsigned short segment_length = sizeof(tObjectDataSeg);
    tObjectDataSeg *segment;

    if (object_coding_method == 0x00) {
        top_field_data_block_length =
            ((unsigned short)segment_data[9] << 8) | segment_data[10];
        bottom_field_data_block_length =
            ((unsigned short)segment_data[11] << 8) | segment_data[12];
        segment_length +=
            top_field_data_block_length + bottom_field_data_block_length;
    } else if (object_coding_method == 0x01) {
        number_of_codes = segment_data[9];
        segment_length += 2 * number_of_codes;
    }

    segment = (tObjectDataSeg*)malloc(segment_length);
    if (segment == NULL)
        return NULL;

    segment->object_id = ((unsigned short)segment_data[6] << 8) | segment_data[7];
    segment->object_version_number = segment_data[8] >> 4;
    segment->object_coding_method = object_coding_method;
    segment->non_modifying_colour_flag = (segment_data[8] & 0x02) != 0;

/*     SUB_DP(( "Object data segment: object_id=%d, version=%d, coding method=%d, non-modifying colour=%d", segment->object_id, segment->object_version_number, segment->object_coding_method, segment->non_modifying_colour_flag));  */

    if (segment->object_coding_method == 0x00) { /* pixels */
        segment->data.pixels.top_field_data_block_length =
            top_field_data_block_length;
        segment->data.pixels.bottom_field_data_block_length =
            bottom_field_data_block_length;
        memcpy(segment->data.pixels.sub_blocks, segment_data + 13,
               top_field_data_block_length + bottom_field_data_block_length);
    } else if (segment->object_coding_method == 0x01) { /* string */
        segment->data.string.number_of_codes = number_of_codes;
        memcpy(segment->data.string.character_codes, segment_data + 10,
               segment->data.string.number_of_codes * 2);

    }

    segment->next_seg = NULL;

    return segment;
}


void ObjectDataSegRelease(tObjectDataSeg* segment) {
    if (segment == NULL)
        return;

    free(segment);
}


tObjectDataSeg *ObjectDataSegCP(const tObjectDataSeg* segment) {
    unsigned short segment_length;
    tObjectDataSeg *s;

    if (segment == NULL)
        return NULL;

    segment_length = sizeof(tObjectDataSeg);
    if (segment->object_coding_method == 0x00)
        segment_length +=
            segment->data.pixels.top_field_data_block_length +
            segment->data.pixels.bottom_field_data_block_length;
    else
        segment_length += 2 * segment->data.string.number_of_codes;

    s = (tObjectDataSeg*)malloc(segment_length);
    if (s == NULL)
        return NULL;

    memcpy(s, segment, segment_length);
    s->next_seg = NULL;

    return s;
}


/********************** display definition segment ******************************/



