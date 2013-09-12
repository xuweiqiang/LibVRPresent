
#ifndef __dsub_decoder_H__
#define __dsub_decoder_H__

#include "dsub_segments.h"
#include "dsub_display_set.h"
#include "Exlist.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _tDsubDecoder_ {
    tPageDecoder     *page_decoders;
} tDsubDecoder;


tDsubDecoder *DSubDecoderCreate(void);

void DSubDecoderRelease(tDsubDecoder *dsub_decoder);


/*
 * Create new page dsub_decoder.
 * Parameters:
 *  dsub_decoder: previously created dsub_decoder
 *  page_id: ID of a page that this page dsub_decoder should handle. There shouldn't
 *           be more than one page dsub_decoder per page_id per dsub_decoder.
 *  is_ancillary_page_decoder: 1 if this page dsub_decoder is an ancillary page
 *                             dsub_decoder for some other page dsub_decoders
 *  ancillary_page_decoder: Ancillary page dsub_decoder for this page or NULL if
 *                          none exists. Note that ancillary page dsub_decoder can't
 *                          have ancillary page dsub_decoder.
 *  callback: Callback function to call when page is ready to be displayed.
 *            May be NULL, if need for display refreshes is noticed by other
 *            means.
 *  priv: Private data for callback.
 */
tPageDecoder* PageDecoderCreate(tDsubDecoder *dsub_decoder, unsigned short page_id,
                           int is_ancillary_page_decoder,
                           tPageDecoder *ancillary_page_decoder,
                           PageDecoder_CB callback,
                           void *priv);


void PageDecoderRelease(tPageDecoder *dsub_decoder);


int PESPacketPush(tDsubDecoder *dsub_decoder, const unsigned char* packet,int max_size);


#ifdef __cplusplus
}
#endif


#endif /* __dsub_decoder_H__ */
