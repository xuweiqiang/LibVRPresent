
#ifndef __dsub_finder_H__
#define __dsub_finder_H__

#include "dsub_segments.h"

#ifdef __cplusplus
extern "C" {
#endif


tRegionCompositionSeg* 
RegionCompositionSegFind(const tRegionCompositionSeg *segs_list,unsigned char region_id);


tCLUT_DefinitionSeg*
CLUT_DefinitionSegFind(const tCLUT_DefinitionSeg *segs_list,const tCLUT_DefinitionSeg *ancillary_segs_list,unsigned char* is_in_ancillary, unsigned char CLUT_id);


tObjectDataSeg*
ObjectDataSegFind(const tObjectDataSeg *segs_list,const tObjectDataSeg *ancillary_segs_list,unsigned char* is_in_ancillary,unsigned short object_id);

#ifdef __cplusplus
}
#endif

#endif /* __dsub_finder_H__ */
