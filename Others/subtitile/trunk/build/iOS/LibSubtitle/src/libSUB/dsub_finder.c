
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


#include "dsub_decoder.h"
#include "dsub_segments.h"
#include "dsub_finder.h"

tRegionCompositionSeg*
RegionCompositionSegFind(const tRegionCompositionSeg *segs_list,unsigned char region_id)
{
    tRegionCompositionSeg *rcs;

    if (segs_list == NULL)
        return NULL;

    rcs = (tRegionCompositionSeg*)segs_list;
    while (rcs != NULL && rcs->region_id != region_id) {
        rcs = rcs->next_seg;
    }

    return rcs;
}


tCLUT_DefinitionSeg*
CLUT_DefinitionSegFind(const tCLUT_DefinitionSeg *segs_list,const tCLUT_DefinitionSeg *ancillary_segs_list,unsigned char* is_in_ancillary, unsigned char CLUT_id)
{
    tCLUT_DefinitionSeg *cds;

    if (segs_list == NULL)
        return NULL;

    cds = (tCLUT_DefinitionSeg*)segs_list;
    while (cds != NULL && cds->CLUT_id != CLUT_id)
        cds = cds->next_seg;
    if(is_in_ancillary)*is_in_ancillary=0;
    if (cds == NULL && ancillary_segs_list) {
        cds = (tCLUT_DefinitionSeg*)ancillary_segs_list;
        if(is_in_ancillary)*is_in_ancillary=1;
        while (cds != NULL && cds->CLUT_id != CLUT_id)
            cds = cds->next_seg;
    }

    return cds;
}


tObjectDataSeg*
ObjectDataSegFind(const tObjectDataSeg *segs_list,const tObjectDataSeg *ancillary_segs_list,unsigned char* is_in_ancillary,unsigned short object_id)
{
    tObjectDataSeg *ods;

    if (segs_list == NULL)
        return NULL;

    ods = (tObjectDataSeg*)segs_list;
    while (ods != NULL && ods->object_id != object_id)
        ods = ods->next_seg;
    if(is_in_ancillary)*is_in_ancillary=0;
    if (ods == NULL && ancillary_segs_list) {
        ods = (tObjectDataSeg*)ancillary_segs_list;
        if(is_in_ancillary)*is_in_ancillary=1;
        while (ods != NULL && ods->object_id != object_id)
            ods = ods->next_seg;
    }

    return ods;
}

