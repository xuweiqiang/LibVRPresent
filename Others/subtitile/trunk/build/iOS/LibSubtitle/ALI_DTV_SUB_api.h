
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


#ifndef _ALI_DTV_SUB_API_H_
#define _ALI_DTV_SUB_API_H_

#ifdef __cplusplus
extern "C" {
#endif

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
HANDLE ALI_DTV_SUB_Create(int page_id,int ancillary_page_id,DisplaySetRecv_CB ds_recv_cb,void* ds_recv_priv);



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
int ALI_DTV_SUB_Release(HANDLE SUBHandle);


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

int ALI_DTV_SUB_Push_PES_Pkt(HANDLE SUBHandle,char* pkt,int max_size);

/**
* ALI_DTV_SUB_Push_ES_Pkt:
* @ SUBHandle:   IN, the context ptr of SUB module
* @ pkt:         IN, the ES buffer to decode.
* @ PTS:         IN, time stamp, with ms as unit.
* @ max_size:    IN, the max size of the pkt buffer.
*
* Description: push sub PES packet to ALI SUB moudle;
* Note:    the DisplaySetRecv_CB will be call back when one page ready     
*
* Returns:  -1, error
*        other, used length
*/
int ALI_DTV_SUB_Push_ES_Pkt(HANDLE SUBHandle,char* pkt,int max_size, unsigned int PTS);

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
int ALI_DTV_SUB_Get_Region_Count_Of_DS(HANDLE SUBHandle,void* display_set);



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
int ALI_DTV_SUB_Get_Time_Out_Of_DS(HANDLE SUBHandle,void* display_set);



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
int ALI_DTV_SUB_Get_Region_Info(HANDLE SUBHandle,void* display_set,region_info* region,unsigned int region_index);


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
                                                               ColorSet_CB set_color_callback,void *priv);

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
int ALI_DTV_SUB_Region_Actual_Rendering(HANDLE SUBHandle,void* ctxt);



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
int ALI_DTV_SUB_Region_Render_Context_Destroy(HANDLE SUBHandle,void* ctxt);


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
int ALI_DTV_SUB_Display_Set_Destroy(HANDLE SUBHandle,void* display_set);


#ifdef __cplusplus
}
#endif

#endif

