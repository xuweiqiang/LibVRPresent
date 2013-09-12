
#ifndef __dsub_segs_H__
#define __dsub_segs_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ALI_DEBUG
#define SUB_DP(__X__) printf __X__;  
#else
#define SUB_DP(__X__)
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined (_WINCE) || defined(_WIN32_WCE)
#include  <windows.h>
#elif defined(WIN32) || defined(__WIN32__) || defined(_WIN32)
#include  <windows.h>
#include <time.h>
#include <io.h>
#include <sys/stat.h>
#include <process.h>
#else
#include <windows.h>
#endif

#ifdef MEMWATCH
#include "memwatch.h"
#endif
#ifdef HAVE_STDINT_H
#include <stdint.h>
#define MUST_INLINE inline 
#else
#define MUST_INLINE 
typedef UINT64   uint64_t;
typedef INT64   int64_t;
#endif

#define DSUB_MODULE_MAGIC 0xa5b295aa
#define IS_HANDLE_VALID(handle)((handle)&&((handle)->magic==DSUB_MODULE_MAGIC))

typedef enum  {
    PAGE_COMPOSITION_SEG = 0x10,
    REGION_COMPOSITION_SEG,
    CLUT_DEFINITION_SEG,
    OBJECT_DATA_SEG,
    DISP_DEFINITION_SEG,
    END_DISP_SET_SEG = 0x80,
}eSubSegmentType;

typedef enum{
    PAGE_STATE_NORMAL_CASE = 0x00,
    PAGE_STATE_ACQUISITION_POINT ,
    PAGE_STATE_MODE_CHANGE,
}ePageState;

/*******************************************************************************/

typedef struct{
    unsigned char region_id;
    /* Horizontal region composition position in page 0..719 */
    unsigned short region_horizontal_address;
    /* Vertical region composition position in page 0..575 */
    unsigned short region_vertical_address;
} tPageCompositionRegion;


typedef struct{
    /* Maximum time to show this page in seconds */
    unsigned char page_time_out;
    unsigned char page_version_number;
    ePageState page_state;
    int number_of_region_compositions;
    tPageCompositionRegion regions[0];
} tPageCompositionSeg;

tPageCompositionSeg *PageCompositionSegParse(const unsigned char* segment_data);
void PageCompositionSegRelease(tPageCompositionSeg* segment);
tPageCompositionSeg *PageCompositionSegCP(const tPageCompositionSeg* segment);


/*******************************************************************************/


typedef enum {
    REGION_LEVEL_COMPABILITY_2BIT_CLUT = 0x01,
    REGION_LEVEL_COMPABILITY_4BIT_CLUT,
    REGION_LEVEL_COMPABILITY_5BIT_CLUT,
} eRegionLevelCompatibility;


typedef enum {
    DBSUB_REGION_DEPTH_2BIT = 0x1,
    DBSUB_REGION_DEPTH_4BIT,
    DBSUB_REGION_DEPTH_8BIT,
} eRegionDepth;


typedef enum {
    OBJECT_TYPE_BASIC_BITMAP = 0x00,
    OBJECT_TYPE_BASIC_CHARACTER,
    OBJECT_TYPE_COMPOSITE_STRING,
} eObjectType;


typedef enum {
    OBJECT_PROVIDER_SUBTITLING_STREAM = 0x00,
    OBJECT_PROVIDER_ROM,
} eObjectProvider;


typedef struct {
    unsigned short        object_id;
    eObjectType object_type;
    eObjectProvider object_provider_flag;
    /* Object position inside region */
    unsigned short        object_horizontal_position;
    unsigned short        object_vertical_position;
    unsigned char         foreground_pixel_code;
    unsigned char         background_pixel_code;
} tRegionCompositionRegion;


/* Region composition segment (ETSI EN 300 743 section 7.2.2) */
typedef struct _tRegionCompositionSeg_ {
    struct _tRegionCompositionSeg_ *next_seg;

    unsigned char         region_id;
    unsigned char         region_version_number;
    int             region_fill_flag;
    unsigned short        region_width;
    unsigned short        region_height;
    eRegionLevelCompatibility region_level_of_compatibility;
    eRegionDepth region_depth;
    unsigned char         CLUT_id;
    unsigned char         region_8bit_pixel_code;
    unsigned char         region_4bit_pixel_code;
    unsigned char         region_2bit_pixel_code;
    unsigned short        number_of_regions;
    tRegionCompositionRegion regions[0];
}tRegionCompositionSeg;


tRegionCompositionSeg *RegionCompositionSegParse(const unsigned char* segment_data);
void RegionCompositionSegRelease(tRegionCompositionSeg* segment);
tRegionCompositionSeg *RegionCompositionSegCP(const tRegionCompositionSeg* segment);


/**************************************/


typedef struct{
    unsigned char         CLUT_entry_id;
    int             CLUT_entry_2bit_flag;
    int             CLUT_entry_4bit_flag;
    int             CLUT_entry_8bit_flag;
    unsigned char         Y_value;
    unsigned char         Cr_value;
    unsigned char         Cb_value;
    unsigned char         T_value;
} tCLUT_Entry;


/* CLUT definition segment (ETSI EN 300 743 section 7.2.3) */
typedef struct _tCLUT_DefinitionSeg_ {
    struct _tCLUT_DefinitionSeg_ *next_seg;
    unsigned char         CLUT_id;
    unsigned char         CLUT_version_number;
    unsigned short        number_of_CLUT_entries;
    tCLUT_Entry     CLUT_entries[0];
} tCLUT_DefinitionSeg;


tCLUT_DefinitionSeg *CLUT_DefinitionSegParse(const unsigned char* segment_data);
void CLUT_DefinitionSegRelease(tCLUT_DefinitionSeg* segment);
tCLUT_DefinitionSeg *CLUT_DefinitionSegCP(const tCLUT_DefinitionSeg* segment);


/**************************************/


typedef enum {
    OBJECT_CODING_PIXELS = 0x00,
    OBJECT_CODING_STRING,
} eObjectCoding;


typedef struct {
    unsigned short        top_field_data_block_length;
    unsigned short        bottom_field_data_block_length;
    unsigned char         sub_blocks[0];
} tObjectDataPixels;


typedef struct {
    unsigned char         number_of_codes;
    unsigned short        character_codes[0];
} tObjectDataString;


typedef struct _tObjectDataSeg_ {
    struct _tObjectDataSeg_ *next_seg;

    unsigned short        object_id;
    unsigned char         object_version_number;
    eObjectCoding object_coding_method;
    int             non_modifying_colour_flag;
    union {
        tObjectDataPixels pixels;
        tObjectDataString string;
    } data;
} tObjectDataSeg;


tObjectDataSeg *ObjectDataSegParse(const unsigned char* segment_data);
void ObjectDataSegRelease(tObjectDataSeg* segment);
tObjectDataSeg *ObjectDataSegCP(const tObjectDataSeg* segment);





#ifdef __cplusplus
}
#endif


#endif /* __dsub_segs_H__ */
