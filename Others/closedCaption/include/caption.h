#ifndef _CAPTION_H_
#define _CAPTION_H_

#include "tsparser.h"
#include "OneSegSubTitleIF.h"

#define MAX_CC_LANGUAGE	8

typedef struct _tDRCSdata
{
	unsigned char m_depth;
	unsigned char m_width;
	unsigned char m_height;	
	unsigned char m_used;
	unsigned char m_data[24*24];
} tDRCSdata;

typedef struct _tCaptionManagement
{
	int data_identifier;
	int private_stream_id;
	int data_group_id;
	int data_group_version;
	int data_group_link_number;
	int last_data_group_link_number;
	int data_group_size;
	
	int TMD;
	int STM;
	int OTM;
	int num_languages;
	int language_tag[MAX_CC_LANGUAGE];
	int DMF[MAX_CC_LANGUAGE];
	int DC[MAX_CC_LANGUAGE];
	int ISO_639_language_code[MAX_CC_LANGUAGE];
	int Format[MAX_CC_LANGUAGE];
	int TCS[MAX_CC_LANGUAGE];
	
	int current_select;
	int current_language_tag;	
	
	tDRCSdata m_DRCS1[110]; //0x21~0x7E
} tCaptionManagement;


void caption_data_group(dmx_stream *bstr, tOneSegSubTitleData* pOneSegSubtileData);
int parser_CC_data(unsigned char* pdata, long len, tOneSegSubTitleData* pOneSegSubtileData);

#endif //_CAPTION_H_


