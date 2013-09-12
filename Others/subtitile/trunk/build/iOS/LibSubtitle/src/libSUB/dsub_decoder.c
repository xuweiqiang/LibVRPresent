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

#include "dsub_segments.h"
#include "dsub_decoder.h"
#include "dsub_display_set.h"


static int VerifyPESPacket(const unsigned char* packet) {
    int substart, packet_length;
    int ret=0;

    /* check Packet header == 0x000001 */
    if (packet[0] != 0x00 || packet[1] != 0x00 || packet[2] != 0x01) {
        SUB_DP(( "pes header error\n"));
        return 1;
    }
#if 0
    /* stream_id == private_stream_1 */
    if (packet[3] != 0xbd) {
       return 0;
    }
#endif    
    packet_length = ((unsigned short)packet[4] << 8) + packet[5] + 6;
    
    if(packet_length==7 && packet[6]==0xff){ // Null PES
       return 2;
    }
    
    /* check data_alignment_indicator == 1 */
    if ((packet[6] & 0x04) == 0) {
        SUB_DP(( "Data Alignment Indicator error\n"));
        return 3;
    }

    /* check PTS */
    if ((packet[7] & 0x80) == 0) {
        SUB_DP(( "PTS not found\n"));
        return 4;
    }
    substart = 9 + packet[8];
   
    /*check data_identifier == 0x20, subtitle_stream_id == 0x00,
       end_of_PES_data_field_marker == 0xff */
    if (packet[substart] != 0x20) {
        SUB_DP(( "Data identifier error\n"));
        return 5;
    }

    if (packet[substart + 1] != 0x00) {
        SUB_DP(( "Subtitle stream ID error\n", packet[substart+1]));
        return 6;
    }

    if (packet[packet_length - 1] != 0xff) {
        SUB_DP(( "Packet length error\n"));
        return 7;
    }

    return ret;
}


static void DsubSegParse(tDsubDecoder* dsub_decoder, const unsigned char* segment_data, uint64_t pts) {
    tDisplaySet *display_set=NULL;
    ExLinkPtr lk=NULL;
    unsigned short page_id = ((unsigned short)segment_data[2] << 8) + segment_data[3];
    tPageDecoder *page_decoder = dsub_decoder->page_decoders;
    tDisplaySet temp_ds;
    while (page_decoder != NULL && page_decoder->page_id != page_id)
        page_decoder = page_decoder->next_dsub_decoder;
    if (page_decoder == NULL)
        return;
    memset((void *)(&temp_ds),0,sizeof(tDisplaySet));
    temp_ds.PTS=pts;
    lk = ExListLinkSearch(page_decoder->display_sets, (void*)&temp_ds);
    if(lk != NULL){
       display_set=(tDisplaySet *)lk->data;
    }

    if (display_set == NULL) {
        display_set = DisplaySetCreate(page_id, pts);
        if(display_set != NULL){
           lk=ExListAppend(page_decoder->display_sets, display_set);
        }
        else
          return; 
    }

    switch (segment_data[1]) {
    case PAGE_COMPOSITION_SEG: { /* Page composition segment */        
        tPageCompositionSeg *segment =
            PageCompositionSegParse(segment_data);        
        if (segment == NULL)
            break;
        PageCompositionSegsUpdate(&(page_decoder->rs_repo),display_set, segment);
        if(segment->page_state==PAGE_STATE_ACQUISITION_POINT || segment->page_state==PAGE_STATE_MODE_CHANGE){
           if(lk) ExListRemoveNode(page_decoder->display_sets,lk,RM_NODE_FRONT);        /* clear prev display sets list. */
        }
        return;      //  not check ready if only update PCS
    }
    case REGION_COMPOSITION_SEG: { /* Region composition segment */   
        tRegionCompositionSeg *segment =
            RegionCompositionSegParse(segment_data);
        if (segment == NULL)
            break;
        RegionCompositionSegsUpdate(&(page_decoder->rs_repo),display_set, segment);
        break;
    }
    case CLUT_DEFINITION_SEG: { /* CLUT definition segment */         
        tCLUT_DefinitionSeg *segment =
            CLUT_DefinitionSegParse(segment_data);
        if (segment == NULL)
            break;
        CLUT_DefinitionSegsUpdate(&(page_decoder->rs_repo),display_set, segment);
        break;
    }
    case OBJECT_DATA_SEG: {/* Object data segment */        
        tObjectDataSeg *segment =
            ObjectDataSegParse(segment_data);
        if (segment == NULL)
            break;
        ObjectDataSegsUpdate(&(page_decoder->rs_repo),display_set, segment);
        break;
    }
    case DISP_DEFINITION_SEG: {/* display definition segment  */        
        SUB_DP(( "display definition segment\n")); 
        break;
    }    
    case END_DISP_SET_SEG: { /* End of display set segment */
        SUB_DP(( "End of display seg, pageid : %x\n", page_id)); 
   //      if (display_set->ready_for_display == 0)
   //        IsDisplaySetOK(page_decoder, display_set);        
        break;
    }
    default:
        SUB_DP(( "Unknown seg, pageid : %x\n", page_id));
        break;
    }

    if (display_set->ready_for_display == 0){
#if 0
         if(display_set->page_composition_seg==NULL){
            if(page_decoder->rs_repo.page_composition_seg_last==NULL){
               return;  //not have PCS
            }
            display_set->page_composition_seg=PageCompositionSegCP(page_decoder->rs_repo.page_composition_seg_last);
            have_cp_pcs=1;
            if(display_set->page_composition_seg==NULL){
               return;  // copy error
            }
        }
#endif         
        IsDisplaySetOK(page_decoder, display_set);
    }

    if (display_set->ready_for_display == 1 && page_decoder->callback != NULL) { // reday now
#if 0    
        //bk the PCS of last DS
        if(have_cp_pcs==0){
           if(page_decoder->rs_repo.page_composition_seg_last){
              PageCompositionSegRelease(page_decoder->rs_repo.page_composition_seg_last);
           }
           page_decoder->rs_repo.page_composition_seg_last=PageCompositionSegCP(display_set->page_composition_seg);
        }
#endif        
        ExListUnlinkNode(page_decoder->display_sets, lk, 0);  //unlink it but not free it   
        page_decoder->callback(page_decoder->priv, display_set);
        display_set->ready_for_display = 2;
    }

}

int ESPacketPush(int packet_length, const unsigned char* packet, tDsubDecoder* dsub_decoder, uint64_t pts ) 
{
	int pos = 0;
	int start = 0;
	// Find sync_byte
	while(pos < packet_length - 2 && 
		packet[pos] != 0x20 &&
		packet[pos+1] != 0x00 &&
		packet[pos+2] != 0x0F 
		)
	{
		pos ++;
	}
	if(packet[pos] != 0x20 ||
		packet[pos+1] != 0x00 ||
		packet[pos+2] != 0x0F)
	{
		// Can not find start sync byte.
		return packet_length;
	}
	// Find End
	start = pos;
	pos += 2;
	while(packet[pos] == 0x0F  && pos < packet_length)
	{
		pos += 6 + ((unsigned short)packet[pos + 4] << 8) + packet[pos + 5];
	}
	if(packet[pos] == 0xFF)
	{
		// We get all this packet, parse it.
		pos = start + 2;
		while (pos < packet_length && packet[pos] == 0x0f) {
			DsubSegParse(dsub_decoder, packet + pos, pts);
			pos += 6 + ((unsigned short)packet[pos + 4] << 8) + packet[pos + 5];
		}	
		return pos + 1;
	}
	// Drop useless data.
	return start;
}


int PESPacketPush(tDsubDecoder* dsub_decoder, const unsigned char* packet,int max_size) {
    uint64_t pts;
    int packet_length, pos;
    int ret;// 

    if(max_size<7) return -1;
    ret=VerifyPESPacket(packet);
    if(ret==0){
        pts = (int64_t)(packet[9] & 0x0E) << 29;
        pts |= (int64_t)packet[10] << 22;
        pts |= (int64_t)(packet[11] & 0xFE) << 14;
        pts |= (int64_t)packet[12] << 7;
        pts |= (int64_t)(packet[13] & 0xFE) >> 1;
    //  pts /= 90;   
    }
    else if(ret == 4){ // No PTS
    //  pts=0;
        return -1;
    }
    else{
      return -1;
    }
    packet_length = ((unsigned short)packet[4] << 8) + packet[5] + 6;
    if(packet_length>max_size) packet_length=max_size;

    pos = 9 + (unsigned short)packet[8];
	ESPacketPush(packet_length - pos, packet + pos, dsub_decoder, pts);

    return 0;
}

static int displayset_list_compare_valid_PTS(const void *data0, const void *data1)
{
	tDisplaySet * ds0=(tDisplaySet *)(data0);
	tDisplaySet * ds1=(tDisplaySet *)(data1);
	if (ds0->PTS < ds1->PTS)
	{
		return (-1);
	}
	else if (ds0->PTS >= ds1->PTS)
	{
		return (0);
	}
    	return 1;
}

static int displayset_list_compare_visible_ds(const void *data0, const void *data1)
{
	tDisplaySet * ds0=(tDisplaySet *)(data0);
	tDisplaySet * ds1=(tDisplaySet *)(data1);
	if (ds0->PTS <= ds1->PTS && ds0->PTS + ds0->page_composition_seg->page_time_out*1000*90<ds1->PTS)
	{
		return (-1);
	}
	else if (ds0->PTS <= ds1->PTS &&  ds0->PTS + ds0->page_composition_seg->page_time_out*1000*90 >= ds1->PTS)
	{
		return (0);
	}
    	return 1;
}


void CleanDisplaySetBeforePTS(tDsubDecoder *dsub_decoder, uint64_t pts) {
    tPageDecoder *pd = dsub_decoder->page_decoders;
    tDisplaySet temp_ds;
    memset((void *)(&temp_ds),0,sizeof(tDisplaySet));
    while (pd != NULL) {
            ExLinkPtr lk=NULL;
            temp_ds.PTS=pts;
            if(NULL!=(lk=ExListLinkSearchCustom(pd->display_sets, (void*)&temp_ds,displayset_list_compare_valid_PTS)))
                   ExListRemoveNode(pd->display_sets, lk,RM_NODE_FRONT);
        pd = pd->next_dsub_decoder;
    }            
}


static ExLinkPtr
find_visible_ds_lk(tPageDecoder *dsub_decoder, uint64_t pts) {    //find need ds,but return it's link ptr 
    tDisplaySet temp_ds;
    memset((void *)(&temp_ds),0,sizeof(tDisplaySet));
    temp_ds.PTS=pts;
    return (ExListLinkSearchCustom(dsub_decoder->display_sets, (void*)&temp_ds,displayset_list_compare_visible_ds));
}


tDisplaySet*
FindCurDisplaySet(tPageDecoder *dsub_decoder, uint64_t pts) {
    ExLinkPtr lk=NULL;
    if(NULL!=(lk=find_visible_ds_lk(dsub_decoder,pts)))
       return (tDisplaySet *)lk->data;

    return NULL;
}


tDisplaySet*
FindNextDisplaySet(tPageDecoder *dsub_decoder, uint64_t pts) {
    ExLinkPtr lk=NULL;
    if(NULL!=(lk=find_visible_ds_lk(dsub_decoder,pts))){
       if(lk!= ExListEnd(dsub_decoder->display_sets) && NULL!=lk->next){
            return (tDisplaySet *)lk->data;    
         }
    }
    return NULL;
}



tDsubDecoder *DSubDecoderCreate(void) {
    tDsubDecoder *dsub_decoder =
        (tDsubDecoder*)malloc(sizeof(tDsubDecoder));
    if (dsub_decoder == NULL)
        return NULL;
    memset(dsub_decoder,0,sizeof(tDsubDecoder));
    dsub_decoder->page_decoders = NULL;

    return dsub_decoder;
}


void DSubDecoderRelease(tDsubDecoder *dsub_decoder) {
    tPageDecoder *d = dsub_decoder->page_decoders;
    while (d != NULL) {
        tPageDecoder *d2 = d;
        d = d->next_dsub_decoder;
        PageDecoderRelease(d2);
    }
    if(dsub_decoder)
        free(dsub_decoder);
}

//extern ExListPtr DisplaySetListCreate();

tPageDecoder*
PageDecoderCreate(tDsubDecoder *dsub_decoder, unsigned short page_id,
                           int is_ancillary_page_decoder,
                           tPageDecoder *ancillary_page_decoder,
                           PageDecoder_CB callback,
                           void *priv) {
    tPageDecoder *page_decoder =
        (tPageDecoder*)malloc(sizeof(tPageDecoder));
    if (page_decoder == NULL)
        return NULL;

    memset(page_decoder,0,sizeof(tPageDecoder));
    page_decoder->page_id = page_id;
    page_decoder->is_ancillary_page_decoder = is_ancillary_page_decoder;
    page_decoder->display_sets =DisplaySetListCreate();
    page_decoder->callback = callback;
    page_decoder->priv = priv;
    page_decoder->dsub_decoder = dsub_decoder;

    if (is_ancillary_page_decoder) /* make a loop */
        page_decoder->ancillary_page_decoders = page_decoder;
    else if (ancillary_page_decoder) {
        page_decoder->ancillary_page_decoders =
       ancillary_page_decoder->ancillary_page_decoders;
       ancillary_page_decoder->ancillary_page_decoders = page_decoder;    
    } else
        page_decoder->ancillary_page_decoders = NULL;

    page_decoder->next_dsub_decoder = dsub_decoder->page_decoders;
    dsub_decoder->page_decoders = page_decoder;

    return page_decoder;
}


void PageDecoderRelease(tPageDecoder *dsub_decoder) {
    tDisplaySet *ds;
    tPageDecoder *pd=NULL;
    tEpochRsRepo *rs_repo=&(dsub_decoder->rs_repo);

    tDsubDecoder *d = dsub_decoder->dsub_decoder;
    if (d->page_decoders == dsub_decoder)
        d->page_decoders = dsub_decoder->next_dsub_decoder;
    else {
        tPageDecoder *pd = d->page_decoders;
        while (pd != NULL) {
            if (pd->next_dsub_decoder == dsub_decoder) {
                pd->next_dsub_decoder = dsub_decoder->next_dsub_decoder;
                break;
            }
            pd = pd->next_dsub_decoder;
        }
    }

    /* delest display sets list. */
    ExListDelete(dsub_decoder->display_sets);

    RsRepoRelease(rs_repo);

    if (dsub_decoder->is_ancillary_page_decoder) {
        /* Destroy ancillary dsub_decoder list */
        pd = dsub_decoder->ancillary_page_decoders;
        while (pd != dsub_decoder) {
            tPageDecoder *pd2;

            /* Remove all references to ancillary display sets */
          ExLinkPtr  curlinkp=NULL;
       	 if(0==ExListEmpty(pd->display_sets))
       	 {
       	       curlinkp=ExListFront(pd->display_sets);
       		do{
       	       	ds=(tDisplaySet *)(curlinkp->data);
                if(ds){
                  ds->ancillary_display_set = NULL;
                }
                if(curlinkp==ExListEnd(pd->display_sets)){
                    break;
                }
                else{
                curlinkp=curlinkp->next;
                }
       		}while(1);
       	 }  

           pd2 = pd;
           pd = pd->ancillary_page_decoders;
           pd2->ancillary_page_decoders = NULL;
        }        
    } 
#if 1    
    else if (dsub_decoder->ancillary_page_decoders){    
        pd = dsub_decoder->ancillary_page_decoders;
        while (pd != dsub_decoder) {
            /* Remove only self from ancillary dsub_decoder list */
            if (pd->ancillary_page_decoders == dsub_decoder) {
                pd->ancillary_page_decoders = dsub_decoder->ancillary_page_decoders;
                break;
            }
            pd = pd->ancillary_page_decoders;
        }
    }
#endif
    free(dsub_decoder);
}
