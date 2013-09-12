#ifndef __DISPLAY_SET_H__
#define __DISPLAY_SET_H__

#include "dsub_segments.h"
#include "Exlist.h"

typedef struct{ // use to stor the related resource of one epoch
   // record the last PCS for display set which not have PCS, ref ETSI 300 743 sec5.1.1
   tPageCompositionSeg *page_composition_seg_last; 
   tRegionCompositionSeg *region_composition_segs;
   tCLUT_DefinitionSeg *CLUT_definition_segs;
   tObjectDataSeg *object_data_segs;
}tEpochRsRepo;


typedef struct _tDisplaySet_ {
    unsigned short                    page_id;
    uint64_t                    PTS; /* milliseconds */
    int                         ready_for_display; /* 1 if page is complete */
    tPageCompositionSeg *page_composition_seg;
    tRegionCompositionSeg *region_composition_segs;
    tCLUT_DefinitionSeg *CLUT_definition_segs;
    tObjectDataSeg *object_data_segs;
    struct _tDisplaySet_ *ancillary_display_set;
    unsigned int magic;
} tDisplaySet;

typedef void (*PageDecoder_CB)(void *priv, tDisplaySet *display_set);

struct _tDsubDecoder_;

typedef struct _tPageDecoder_ {
    unsigned short                  page_id;
    int                       is_ancillary_page_decoder;
    ExListPtr display_sets;
    tEpochRsRepo rs_repo; 
    
    PageDecoder_CB callback;
    void                      *priv;
    
    struct _tDsubDecoder_   *dsub_decoder;
    /* This page dsub_decoder is part of a circular list of page dsub_decoders where one
       of the dsub_decoders is an ancillary page dsub_decoder for other dsub_decoders. */
    struct _tPageDecoder_ *ancillary_page_decoders;
    struct _tPageDecoder_ *next_dsub_decoder;
} tPageDecoder;


void RegionCompositionSegsUpdate(tEpochRsRepo* rs_repo,tDisplaySet *display_set,tRegionCompositionSeg *segment);


void CLUT_DefinitionSegsUpdate(tEpochRsRepo* rs_repo,tDisplaySet *display_set, tCLUT_DefinitionSeg *segment);


void ObjectDataSegsUpdate(tEpochRsRepo* rs_repo,tDisplaySet *display_set,tObjectDataSeg *segment);


void PageCompositionSegsUpdate(tEpochRsRepo* rs_repo,tDisplaySet *display_set,tPageCompositionSeg *segment);


int IsDisplaySetOK(tPageDecoder *dsub_decoder,tDisplaySet *display_set);

tDisplaySet* DisplaySetCreate(unsigned short page_id, uint64_t pts);

void DisplaySetInsert(tPageDecoder *dsub_decoder,tDisplaySet *display_set);

void DisplaySetRelease(tDisplaySet *display_set);

ExListPtr DisplaySetListCreate();

void RsRepoRelease(tEpochRsRepo *rs_repo);


#endif /* __DISPLAY_SET_H__ */
