#ifndef _CC_1SEG_H_
#define _CC_1SEG_H_

#define MAX_CC_LANGUAGE	2

#include "bitop.h"

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
	int rollup_mode[MAX_CC_LANGUAGE];
	
	int current_select;
	int current_language_tag;	
	
	tDRCSdata m_DRCS1[110]; //0x21~0x7E
} tCaptionManagement;

void InitClosedCaption(tCaptionManagement* pCaptionManagement);
int GetCCData(unsigned char *input, unsigned char *output, int input_len, tCaptionManagement* pCaptionManagement);
int CCGetNumberOfLanguage(tCaptionManagement* pCaptionManagement);
int CCDirectSetLanguage(tCaptionManagement* pCaptionManagement, int mode);
int CCSelectNextLanguage(tCaptionManagement* pCaptionManagement, int mode);
int SetDefaultManagementData(tCaptionManagement* pCaptionManagement, int language_code);

int drcs_data_structure(NewBstr *bstr, tCaptionManagement* pCaptionManagement);
int data_unit(NewBstr *bstr, int *total_len, unsigned char *buffer, tCaptionManagement* pCaptionManagement);
int caption_management_data(NewBstr *bstr, int *total_len, unsigned char *buffer, tCaptionManagement* pCaptionManagement);
int caption_data_group(NewBstr *bstr, int *total_len, unsigned char *buffer, tCaptionManagement* pCaptionManagement);
int caption_statement_data(NewBstr *bstr, int *total_len, unsigned char *buffer, tCaptionManagement* pCaptionManagement);


#endif //_CC_1SEG_H_
