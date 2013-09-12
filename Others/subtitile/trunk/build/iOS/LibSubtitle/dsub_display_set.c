
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

#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#include "dsub_decoder.h"
#include "dsub_segments.h"
#include "dsub_finder.h"
#include "Exlist.h"

static void RegionCompositionSegListRelease(tRegionCompositionSeg *segs_list) {
    tRegionCompositionSeg *rcs;

    if (segs_list == NULL)
        return;

    rcs = segs_list;
    while (rcs != NULL) {
        tRegionCompositionSeg *tmp = rcs;
        rcs = rcs->next_seg;
        RegionCompositionSegRelease(tmp);
    }
}


static void CLUT_DefinitionSegListRelease(tCLUT_DefinitionSeg *segs_list) {
    tCLUT_DefinitionSeg *cds;

    if (segs_list == NULL)
        return;

    cds = segs_list;
    while (cds != NULL) {
        tCLUT_DefinitionSeg *tmp = cds;
        cds = cds->next_seg;
        CLUT_DefinitionSegRelease(tmp);
    }

}


static void ObjectDataSegListRelease(tObjectDataSeg *segs_list) {
    tObjectDataSeg *ods;

    if (segs_list == NULL)
        return;

    ods = segs_list;
    while (ods != NULL) {
        tObjectDataSeg *tmp = ods;
        ods = ods->next_seg;
        ObjectDataSegRelease(tmp);
    }

}

static void displayset_list_deallocator(ExLinkPtr lk)
{
	tDisplaySet * display_set=(tDisplaySet *)(lk->data);
   	 if(NULL!=display_set){
          PageCompositionSegRelease(display_set->page_composition_seg);
          RegionCompositionSegListRelease(display_set->region_composition_segs);
          CLUT_DefinitionSegListRelease(display_set->CLUT_definition_segs);
          ObjectDataSegListRelease(display_set->object_data_segs);		
          free(display_set);
    }
}

static int displayset_list_compare(const void *data0, const void *data1)
{
	tDisplaySet * ds0=(tDisplaySet *)(data0);
	tDisplaySet * ds1=(tDisplaySet *)(data1);
	if (ds0->PTS < ds1->PTS)
	{
		return (-1);
	}
	else if (ds0->PTS == ds1->PTS)
	{
		return (0);
	}
    	return 1;
}

ExListPtr DisplaySetListCreate()
{
      return ExListCreate(displayset_list_deallocator, displayset_list_compare);
}



void RegionCompositionSegsUpdate(tEpochRsRepo* rs_repo,tDisplaySet *display_set,tRegionCompositionSeg *segment) {
    tRegionCompositionSeg *s, *prev;

    if (segment == NULL)
        return;

    if (rs_repo->region_composition_segs == NULL) {
        rs_repo->region_composition_segs = segment;
        return;
    }

    s = rs_repo->region_composition_segs;
    prev = NULL;
    while (s != NULL && s->region_id != segment->region_id) {
        prev = s;
        s = s->next_seg;
    }

    if (s == NULL) {
        /* Note that we don't have to set ready_for_display to 0, because
           it is 0 already, or if 1, this region composition doesn't belong to
           current visible page. */
        segment->next_seg = rs_repo->region_composition_segs;
        rs_repo->region_composition_segs = segment;
    } else if (segment->region_version_number != s->region_version_number) {
        segment->next_seg = s->next_seg;
        if (prev == NULL)
            rs_repo->region_composition_segs = segment;
        else
            prev->next_seg = segment;
        RegionCompositionSegRelease(s);
        display_set->ready_for_display = 0;
    } else
        RegionCompositionSegRelease(segment); /* No update */
}


void CLUT_DefinitionSegsUpdate(tEpochRsRepo* rs_repo,tDisplaySet *display_set, tCLUT_DefinitionSeg *segment) {
    tCLUT_DefinitionSeg *s, *prev;

    if (segment == NULL)
        return;

    if (rs_repo->CLUT_definition_segs == NULL) {
        rs_repo->CLUT_definition_segs = segment;
        return;
    }

    s = rs_repo->CLUT_definition_segs;
    prev = NULL;
    while (s != NULL && s->CLUT_id != segment->CLUT_id) {
        prev = s;
        s = s->next_seg;
    }

    if (s == NULL) {
        segment->next_seg = rs_repo->CLUT_definition_segs;
        rs_repo->CLUT_definition_segs = segment;
    } else if (segment->CLUT_version_number != s->CLUT_version_number) {
        segment->next_seg = s->next_seg;
        if (prev == NULL)
            rs_repo->CLUT_definition_segs = segment;
        else
            prev->next_seg = segment;
        CLUT_DefinitionSegRelease(s);
        display_set->ready_for_display = 0;
    } else
        CLUT_DefinitionSegRelease(segment); /* No update */
}


void ObjectDataSegsUpdate(tEpochRsRepo* rs_repo,tDisplaySet *display_set,tObjectDataSeg *segment) {
    tObjectDataSeg *s, *prev;

    if (segment == NULL)
        return;

    if (rs_repo->object_data_segs == NULL) {
        rs_repo->object_data_segs = segment;
        return;
    }

    s = rs_repo->object_data_segs;
    prev = NULL;
    while (s != NULL && s->object_id != segment->object_id) {
        prev = s;
        s = s->next_seg;
    }

    if (s == NULL) {
        segment->next_seg = rs_repo->object_data_segs;
        rs_repo->object_data_segs = segment;
    } else if (segment->object_version_number != s->object_version_number) {
        segment->next_seg = s->next_seg;
        if (prev == NULL)
            rs_repo->object_data_segs = segment;
        else
            prev->next_seg = segment;
        ObjectDataSegRelease(s);
        display_set->ready_for_display = 0;
    } else
        ObjectDataSegRelease(segment); /* No update */
}



void PageCompositionSegsUpdate(tEpochRsRepo* rs_repo,tDisplaySet *display_set,tPageCompositionSeg *segment) {
    switch (segment->page_state) {
    case PAGE_STATE_NORMAL_CASE: {
        if (display_set->page_composition_seg == NULL) {
            display_set->page_composition_seg = segment;
        } else if (segment->page_version_number !=
                   display_set->page_composition_seg->page_version_number) {
            PageCompositionSegRelease(display_set->page_composition_seg);
            display_set->page_composition_seg = segment;
            
        } else {
            PageCompositionSegRelease(segment);
            return;
        }
        break;
    }
    case PAGE_STATE_ACQUISITION_POINT:
    case PAGE_STATE_MODE_CHANGE: 
        PageCompositionSegRelease(rs_repo->page_composition_seg_last);
        RegionCompositionSegListRelease(rs_repo->region_composition_segs);
        CLUT_DefinitionSegListRelease(rs_repo->CLUT_definition_segs);
        ObjectDataSegListRelease(rs_repo->object_data_segs);      
        memset(rs_repo,0,sizeof(tEpochRsRepo));
        display_set->page_composition_seg = segment;
        break;
    }
    /* If page has been updated, redraw is required */
    display_set->ready_for_display = 0;
}

#define MAX_DS_SEGS  1024
typedef struct{
unsigned int how_many; 
int ptrs[MAX_DS_SEGS];
}tDS_SEGS_PTR_Rec; //only for temp record the ptrs 

int IsDisplaySetOK(tPageDecoder *dsub_decoder,
                                      tDisplaySet *display_set) {
    tPageCompositionSeg *pcs;
    int i;
    tDS_SEGS_PTR_Rec rcs_segs_rec;
    tDS_SEGS_PTR_Rec ods_segs_rec;
    tDS_SEGS_PTR_Rec cds_segs_rec;
    tDisplaySet *ancillary_display_set;
    unsigned char is_in_anc=0;
    tEpochRsRepo* rs_repo=&(dsub_decoder->rs_repo);

    if (display_set == NULL || display_set->page_composition_seg == NULL)
        return 0;

    if (display_set->ready_for_display)
        return 1;

    /* Find ancillary page */
    if (display_set->ancillary_display_set == NULL &&
        dsub_decoder->ancillary_page_decoders != NULL) {
        tPageDecoder *pd = dsub_decoder->ancillary_page_decoders;       
        tDisplaySet temp_ds;
        memset((void *)(&temp_ds),0,sizeof(tDisplaySet));
        while (pd != dsub_decoder) {
            if (pd->is_ancillary_page_decoder) {
                 tDisplaySet *ds=NULL;
                 temp_ds.PTS=display_set->PTS;
                 ds=(tDisplaySet *)ExListSearch(pd->display_sets, (void*)&temp_ds);                
                 if(NULL!=ds){
                    display_set->ancillary_display_set = ds;
                 }             
                break;
            }
            pd = pd->ancillary_page_decoders;
        }
    }
    rcs_segs_rec.how_many=0;
    ods_segs_rec.how_many=0;
    cds_segs_rec.how_many=0;
    ancillary_display_set=display_set->ancillary_display_set; 

    pcs = display_set->page_composition_seg;
    for (i = 0; i < pcs->number_of_region_compositions; ++i) {
        tRegionCompositionSeg *rcs =
          RegionCompositionSegFind(rs_repo->region_composition_segs,pcs->regions[i].region_id);            
        if (rcs != NULL) {
            tCLUT_DefinitionSeg *cds;
            int j;
            rcs_segs_rec.ptrs[rcs_segs_rec.how_many]=rcs;  // record rcs
            rcs_segs_rec.how_many++;
            for (j = 0; j < rcs->number_of_regions; ++j) {
                tObjectDataSeg *ods =
                  ObjectDataSegFind(rs_repo->object_data_segs,((ancillary_display_set!=NULL)? ancillary_display_set->object_data_segs : NULL),
                                    &is_in_anc,rcs->regions[j].object_id);
                if (ods == NULL) {
                    goto not_ready;
                }
                else{
                    if(is_in_anc==0){
                       ods_segs_rec.ptrs[ods_segs_rec.how_many]=ods;  // record ods
                       ods_segs_rec.how_many++;
                    }
                }
            }
            cds = CLUT_DefinitionSegFind(rs_repo->CLUT_definition_segs,((ancillary_display_set!=NULL)? ancillary_display_set->CLUT_definition_segs : NULL),
                                         &is_in_anc,rcs->CLUT_id);
            if (cds == NULL) {
                goto not_ready;
            }
            else{
                if(is_in_anc==0){
                   cds_segs_rec.ptrs[cds_segs_rec.how_many]=cds;  // record cds
                   cds_segs_rec.how_many++;
                }
            }
        }
        else {        
            goto not_ready;
        }
    }
    for(i=0;i<rcs_segs_rec.how_many;i++){ // copy rcs to ds
       tRegionCompositionSeg *rcs =RegionCompositionSegCP((tRegionCompositionSeg *)rcs_segs_rec.ptrs[i]);
       rcs->next_seg=display_set->region_composition_segs;
       display_set->region_composition_segs= rcs; 
    }
    for(i=0;i<ods_segs_rec.how_many;i++){ // copy ods to ds
       tObjectDataSeg *ods =ObjectDataSegCP((tObjectDataSeg *)ods_segs_rec.ptrs[i]);
       ods->next_seg=display_set->object_data_segs;
       display_set->object_data_segs= ods;
    }
    for(i=0;i<cds_segs_rec.how_many;i++){ // copy cds to ds
       tCLUT_DefinitionSeg *cds =CLUT_DefinitionSegCP((tCLUT_DefinitionSeg *)cds_segs_rec.ptrs[i]);
       cds->next_seg=display_set->CLUT_definition_segs;
       display_set->CLUT_definition_segs= cds;
     } 
    display_set->ready_for_display = 1;
    return 1;
not_ready:
    return 0;
}


tDisplaySet *DisplaySetCreate(unsigned short page_id, uint64_t pts) {
    tDisplaySet *display_set = (tDisplaySet*)malloc(sizeof(tDisplaySet));
    if (display_set == NULL)
        return NULL;

    display_set->page_id = page_id;
    display_set->PTS = pts;
    display_set->ready_for_display = 0;
    display_set->page_composition_seg = NULL;
    display_set->region_composition_segs = NULL;
    display_set->CLUT_definition_segs = NULL;
    display_set->object_data_segs = NULL;
    display_set->ancillary_display_set = NULL;
    display_set->magic = DSUB_MODULE_MAGIC;

    SUB_DP(( "\n--- New DS Created, PTS(ms)=%llu, pageid=%u ---\n\n",pts/90,page_id)); 
    return display_set;
}


void DisplaySetInsert(tPageDecoder *dsub_decoder,
                               tDisplaySet *display_set) {
    tDisplaySet *dset;

    if (dsub_decoder == NULL) {
        return;
    }

    if (display_set == NULL) {
        return;
    }

    ExListAppend(dsub_decoder->display_sets, display_set);
}



void DisplaySetRelease(tDisplaySet *display_set) {
    /* Assume that display sets pointing to this ancillary display set
       won't be used anymore and so we don't have to modify
       ancillary_display_set pointers. */
  if(IS_HANDLE_VALID(display_set)){  
    PageCompositionSegRelease(display_set->page_composition_seg);
    RegionCompositionSegListRelease(display_set->region_composition_segs);
    CLUT_DefinitionSegListRelease(display_set->CLUT_definition_segs);
    ObjectDataSegListRelease(display_set->object_data_segs);
    free(display_set);
    return;
  }
  SUB_DP(( "DisplaySetRelease: display_set ERR\n"));
}

void RsRepoRelease(tEpochRsRepo *rs_repo) {
  if(rs_repo){  
     PageCompositionSegRelease(rs_repo->page_composition_seg_last);
     RegionCompositionSegListRelease(rs_repo->region_composition_segs);
     CLUT_DefinitionSegListRelease(rs_repo->CLUT_definition_segs);
     ObjectDataSegListRelease(rs_repo->object_data_segs);  
    return;
  }
}


