
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
#include "dsub_renderer.h"
#include "dsub_finder.h"


typedef void (*DisplaySetRecv_CB)(void *priv, void *display_set,UINT64 pts);
typedef struct{
    unsigned int region_id;
    /* Horizontal region composition position in page 0..719 */
    unsigned int region_horizontal_address;
    /* Vertical region composition position in page 0..575 */
    unsigned int region_vertical_address;
    unsigned int region_width;
    unsigned int region_height;
    int bkcolor; //background color for region. Value is either in range 0-255 for pseudo color background
} region_info;

typedef struct
{
    int  page_id, ancillary_page_id;
    tDsubDecoder *dsub_decoder;
    tPageDecoder *page_decoder;
    tPageDecoder *ancillary_page_decoder;
    unsigned int sub_en;
    DisplaySetRecv_CB ds_recv_cb;
    void* ds_recv_priv;
    unsigned int magic;
}SUBCtxt,*pSUBCtxt;

#define IS_SUB_PID_VALID(pid) ((pid)<0x1fff && (pid)>0)

#include "ALI_DTV_SUB_api.h"

static void page_decoder_cb_self(void *priv, tDisplaySet * ds)
{
   if(priv){
    pSUBCtxt hr=(pSUBCtxt)priv;
    if(hr->sub_en && hr->ds_recv_cb){
      hr->ds_recv_cb(hr->ds_recv_priv,(void*)ds,ds->PTS);
    }
    else{
      DisplaySetRelease((tDisplaySet*)ds); 
    }
   }
}


/**
* ALI_DTV_SUB_Create:
* @ page_id:        IN, ID of a page that SUB module should handle.
* @ ancillary_page_id:   IN, ID of a ancillary page that SUB module should handle.
* @ ds_recv_cb:     IN, Callback function to call when page is ready to be displayed, 
*                       a ready display set will be pushed and the PTS is also provided
* @ ds_recv_priv:   IN, Private data for callback.
*
*
* Description: Create & Init the SUB module;
*
* Returns:  NULL, error
*          !NULL, SUBHandle
*/
HANDLE ALI_DTV_SUB_Create(int page_id,int ancillary_page_id,DisplaySetRecv_CB ds_recv_cb,void* ds_recv_priv)
{
    pSUBCtxt hr=(pSUBCtxt)malloc(sizeof(SUBCtxt));
    if(hr){
        memset(hr,0,sizeof(SUBCtxt));
        hr->page_id=page_id;
        hr->ancillary_page_id=ancillary_page_id;
        hr->ds_recv_cb=ds_recv_cb;
        hr->ds_recv_priv=ds_recv_priv;
        if(ds_recv_cb){
            /* Create subtitle dsub_decoder. */
           hr->dsub_decoder = DSubDecoderCreate();
           if(hr->dsub_decoder == NULL){
              goto errclear1;
           }
           if (hr->ancillary_page_id != -1) {
               /* Create dsub_decoder for ancillary pages */
               hr->ancillary_page_decoder = PageDecoderCreate(hr->dsub_decoder,hr->ancillary_page_id,
                                                                  1, NULL, NULL, NULL);
               if(hr->ancillary_page_decoder == NULL) {
                 goto errclear2;
               }
               hr->page_decoder = PageDecoderCreate(hr->dsub_decoder,hr->page_id,
                                                         0,hr->ancillary_page_decoder,
                                                         page_decoder_cb_self,
                                                         (void*)hr);
               if (hr->page_decoder == NULL){
                 goto errclear3;
               }
           }else{
               /* Create a page dsub_decoder */
               hr->page_decoder = PageDecoderCreate(hr->dsub_decoder,hr->page_id,
                                                         0, NULL,
                                                         page_decoder_cb_self,
                                                         (void*)hr);
              if (hr->page_decoder == NULL) {
                goto errclear2;
              }
           }          
        }
        else{
            goto errclear1;
        }
        hr->sub_en=1;
        hr->magic=DSUB_MODULE_MAGIC;
        return hr;
    }
    else{
     goto errclear1;
    }
errclear3:
    PageDecoderRelease(hr->ancillary_page_decoder);
errclear2:  
    DSubDecoderRelease(hr->dsub_decoder);
errclear1:
    free(hr);
    hr=NULL;
    return NULL;
}


/**
* ALI_DTV_SUB_Release:
* @ SUBHandle:   IN, the context ptr of SUB module
*
* Description: Release the SUB module;
*
* Returns:  -1, error
*            0, ok
*          
*/
int ALI_DTV_SUB_Release(HANDLE SUBHandle)
{
  	pSUBCtxt hr = (pSUBCtxt)SUBHandle;
	if(IS_HANDLE_VALID(hr))
	{
  	   hr->sub_en=0;
       Sleep(1);
       if(hr->page_decoder)
        PageDecoderRelease(hr->page_decoder);
  	   if(hr->ancillary_page_decoder)
   	    PageDecoderRelease(hr->ancillary_page_decoder);
       if(hr->dsub_decoder)
  		DSubDecoderRelease(hr->dsub_decoder);
       free(hr);
       return 0;
	}
	return -1;
}

/**
* ALI_DTV_SUB_Push_PES_Pkt:
* @ SUBHandle:   IN, the context ptr of SUB module
* @ pkt:         IN, the PES buffer to decode.
* @ max_size:    IN, the max size of the pkt buffer.
*
* Description: push sub PES packet to ALI SUB moudle;
* Note:    the DisplaySetRecv_CB will be call back when one page ready     
*
* Returns:  -1, error
*            0, ok
*/

int ALI_DTV_SUB_Push_PES_Pkt(HANDLE SUBHandle,char* pkt,int max_size)
{
  	pSUBCtxt hr = (pSUBCtxt)SUBHandle;
	if(IS_HANDLE_VALID(hr)&& hr->dsub_decoder && pkt){
       if(hr->sub_en)
	    return PESPacketPush(hr->dsub_decoder,(const unsigned char*)pkt,max_size);
    }
    return -1;
}

int ALI_DTV_SUB_Push_ES_Pkt(HANDLE SUBHandle,char* pkt,int max_size, unsigned int PTS)
{
	pSUBCtxt hr = (pSUBCtxt)SUBHandle;
	if(IS_HANDLE_VALID(hr)&& hr->dsub_decoder && pkt){
		if(hr->sub_en)
		{
			uint64_t TempPts = PTS * 90;
			return ESPacketPush(max_size, (const unsigned char*)pkt, hr->dsub_decoder, TempPts);
		}
	}
	return -1;
}


/**
* ALI_DTV_SUB_Get_Region_Count_Of_DS:
* @ SUBHandle:   IN, the context ptr of SUB module
* @ display_set:     IN, the ready display set.
*
* Description: get the number of regions of this display set
*
* Returns:  -1, error
*           >=0, ok
*/
int ALI_DTV_SUB_Get_Region_Count_Of_DS(HANDLE SUBHandle,void* display_set)
{
  	pSUBCtxt hr = (pSUBCtxt)SUBHandle;
    tDisplaySet *ds=(tDisplaySet *)display_set;    
	if(IS_HANDLE_VALID(hr)&& IS_HANDLE_VALID(ds)){    
       return (ds->page_composition_seg->number_of_region_compositions);
    }
    return -1;
}


/**
* ALI_DTV_SUB_Get_Time_Out_Of_DS:
* @ SUBHandle:   IN, the context ptr of SUB module
* @ display_set:     IN, the ready display set.
*
* Description: get the time out seconds of this display set
*
* Returns:  -1,  error
*           >=0, Maximum time to show this page in seconds
*/
int ALI_DTV_SUB_Get_Time_Out_Of_DS(HANDLE SUBHandle,void* display_set)

{
  	pSUBCtxt hr = (pSUBCtxt)SUBHandle;
    tDisplaySet *ds=(tDisplaySet *)display_set;    
	if(IS_HANDLE_VALID(hr)&& IS_HANDLE_VALID(ds)){    
       return (ds->page_composition_seg->page_time_out);
    }
    return -1;
}

/**
* ALI_DTV_SUB_Get_Region_Info:
* @ SUBHandle:       IN, the context ptr of SUB module
* @ display_set:     IN, the ready display set.
* @ region:          IN/OUT, the ptr of struct region_info will be filled.
* @ region_index:    IN, the index of region want to find.
*
* Description: get the region info of display set from index 
*
* Returns:  -1, error
*           >=0, ok
*/
int ALI_DTV_SUB_Get_Region_Info(HANDLE SUBHandle,void* display_set,region_info* region,unsigned int region_index)
{
  	pSUBCtxt hr = (pSUBCtxt)SUBHandle;
    tDisplaySet *ds=(tDisplaySet *)display_set;    
    tPageCompositionSeg* pcs=NULL;
	if(IS_HANDLE_VALID(hr)&& IS_HANDLE_VALID(ds)&& region){
       pcs=ds->page_composition_seg;
       if(region_index<pcs->number_of_region_compositions){
           tRegionCompositionSeg* rcs=NULL;
           rcs = RegionCompositionSegFind(
                 ds->region_composition_segs,pcs->regions[region_index].region_id);
           if(rcs){
             memset(region,0,sizeof(region_info));
             region->region_id=rcs->region_id;
             region->region_horizontal_address=pcs->regions[region_index].region_horizontal_address;
             region->region_vertical_address=pcs->regions[region_index].region_vertical_address;
             region->region_width=rcs->region_width;
             region->region_height=rcs->region_height;
             region->bkcolor=RegionBackGroundColorGet(rcs);
             return 0;
           }
           else{
             return -1;
           }
       }             
    }
    return -1;
}

/**
* ALI_DTV_SUB_Region_Render_Context_Create:
* @ SUBHandle:       IN, the context ptr of SUB module
* @ display_set:     IN, the ready display set.
* @ region_id:       IN, the ID of the region you want to render.
* @ hline_callback:     IN, function to draw a horizontal line (may be NULL)
* @ string_callback:    IN, function to draw a string (may be NULL)
* @ set_color_callback: IN, function to map pseudo color to display color
* @ priv:               IN, private data to callback functions
*
* Description: Create a rendering context of a region
*
* Returns:  !NULL, the Context 
*            NULL, err
*/
void* ALI_DTV_SUB_Region_Render_Context_Create(HANDLE SUBHandle,void* display_set,unsigned int region_id,
                                                               DrawHLine_CB hline_callback,DrawString_CB string_callback,
                                                               ColorSet_CB set_color_callback,void *priv)
{
  	pSUBCtxt hr = (pSUBCtxt)SUBHandle;
    tDisplaySet *ds=(tDisplaySet *)display_set;    
	if(IS_HANDLE_VALID(hr)&& IS_HANDLE_VALID(ds)){    
       return CreateRender(ds,region_id,hline_callback,string_callback,set_color_callback,priv);
    }
    return NULL;
}

/**
* ALI_DTV_SUB_Region_Actual_Rendering:
* @ SUBHandle:       IN, the context ptr of SUB module
* @ ctxt:            IN, the context ptr of region render.
*
* Description: Actual Rendering of this region
*
* Returns:  -1, error
*           >=0, ok
*/
int ALI_DTV_SUB_Region_Actual_Rendering(HANDLE SUBHandle,void* ctxt)
{
  	pSUBCtxt hr = (pSUBCtxt)SUBHandle;
    tRenderingContext *r_cntx=(tRenderingContext *)ctxt;
	if(IS_HANDLE_VALID(hr)&& IS_HANDLE_VALID(r_cntx)){  
       RenderRegion(r_cntx);
       return 0;
    }
    return -1;
}


/**
* ALI_DTV_SUB_Region_Render_Context_Destroy:
* @ SUBHandle:       IN, the context ptr of SUB module
* @ ctxt:            IN, the context ptr of region render.
*
* Description: Destroy the render context
*
* Returns:  -1, error
*           >=0, ok
*/
int ALI_DTV_SUB_Region_Render_Context_Destroy(HANDLE SUBHandle,void* ctxt)
{
  	pSUBCtxt hr = (pSUBCtxt)SUBHandle;
    tRenderingContext *r_cntx=(tRenderingContext *)ctxt;
	if(IS_HANDLE_VALID(hr)&& IS_HANDLE_VALID(r_cntx)){  
       ReleaseRender(r_cntx);
       return 0;
    }
    return -1;
}


/**
* ALI_DTV_SUB_Display_Set_Destroy:
* @ SUBHandle:       IN, the context ptr of SUB module
* @ display_set:     IN, the display set will be handle.
*
* Description: Destroy this display set
*
* Returns:  -1, error
*           >=0, ok
*/
int ALI_DTV_SUB_Display_Set_Destroy(HANDLE SUBHandle,void* display_set)
{
  	pSUBCtxt hr = (pSUBCtxt)SUBHandle;
    tDisplaySet *ds=(tDisplaySet *)display_set;    
	if(IS_HANDLE_VALID(hr)&& IS_HANDLE_VALID(ds)){   
      DisplaySetRelease(ds);
      return 0;
    }
    return -1;
}









