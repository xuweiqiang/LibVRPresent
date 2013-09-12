#include <string.h>
#include "bitop.h"
#include "cc_1seg.h"

int drcs_data_structure(NewBstr *bstr, tCaptionManagement* pCaptionManagement)
{
	int i, j, k;
	int NumberOfCode;
	int CharacterCode;
	int NumberOfFont;
	int FondId;
	int mode;
	int depth, width, height;
	int regionX, regionY, geometricData_length;
	int index;
	
	tDRCSdata *pDRCS;


	NumberOfCode = Get_Bits(bstr, 8);

	for(i=0;i<NumberOfCode;i++)
	{
		CharacterCode = Get_Bits(bstr, 16);
		NumberOfFont = Get_Bits(bstr, 8);

		index = (CharacterCode&0xFF);
		if(index<0x21 || index>0x7E)
			return -1;
		pDRCS = &pCaptionManagement->m_DRCS1[index-0x21];

		for(j=0;j<NumberOfFont;j++)
		{
			FondId = Get_Bits(bstr, 4);
			mode = Get_Bits(bstr, 4);

			if(mode==0||mode==1)
			{
				pDRCS->m_depth = depth = Get_Bits(bstr, 8);
				pDRCS->m_width = width = Get_Bits(bstr, 8);
				pDRCS->m_height = height = Get_Bits(bstr, 8);
				pDRCS->m_used = 1;

				for(k=0;k<(width*height/8);k++)
					pDRCS->m_data[k] = (unsigned char)Get_Bits(bstr, 8);
			}
			else
			{
				regionX = Get_Bits(bstr, 8);
				regionY = Get_Bits(bstr, 8);
				geometricData_length = Get_Bits(bstr, 16);
				for(k=0;k<geometricData_length;k++)
					Get_Bits(bstr, 8);
			}
		}
	}

	return 0;
}

int data_unit(NewBstr *bstr, int *total_len, unsigned char *buffer, tCaptionManagement* pCaptionManagement)
{
	int unit_separator;
	int data_unit_parameter;
	int data_unit_size;	

	unit_separator = Get_Bits(bstr, 8);
	data_unit_parameter = Get_Bits(bstr, 8);
	data_unit_size = Get_Bits(bstr, 24);
	
	if(data_unit_size>640)
		return -1;
	if(data_unit_parameter==0x30||data_unit_parameter==0x31)
	{
		if(drcs_data_structure(bstr, pCaptionManagement)<0)
			return -1;
	}
	else if(data_unit_parameter==0x20)
	{
#if 0
		for(i=0;i<data_unit_size;i++)
		{	
			buffer[*total_len] = Get_Bits(bstr, 8);	
			(*total_len)++;
		}
#else
		(*total_len) += GetBytes(bstr, buffer+(*total_len), data_unit_size);
#endif		
	}
	return (1+1+3+data_unit_size);
}

int caption_management_data(NewBstr *bstr, int *total_len, unsigned char *buffer, tCaptionManagement* pCaptionManagement)
{
	int i;

	int TMD;
	int reserved;
	int num_languages;
	int language_tag;
	int DMF;	
	int data_unit_loop_length;
	int data_unit_len;

	pCaptionManagement->TMD = TMD = Get_Bits(bstr, 2);
	reserved = Get_Bits(bstr, 6);

	if(TMD==2)
	{
		pCaptionManagement->OTM = Get_Bits(bstr, 36);
		reserved = Get_Bits(bstr, 4);
	}

	pCaptionManagement->num_languages = num_languages = Get_Bits(bstr, 8);
	
	if(num_languages>2||num_languages<=0)
		return -1;

	for(i=0;i<num_languages;i++)
	{
		pCaptionManagement->language_tag[i] = language_tag = Get_Bits(bstr, 3);
		if(language_tag!=0&&language_tag!=1)
			return -1;
		reserved = Get_Bits(bstr, 1);
		pCaptionManagement->DMF[i] = DMF = Get_Bits(bstr, 4);

		if(DMF==0xC || DMF==0xD || DMF==0xE)
		{
			pCaptionManagement->DC[i] = Get_Bits(bstr, 8);
		}

		pCaptionManagement->ISO_639_language_code[i] = Get_Bits(bstr, 24);

		pCaptionManagement->Format[i] = Get_Bits(bstr, 4);
		pCaptionManagement->TCS[i] = Get_Bits(bstr, 2);
		pCaptionManagement->rollup_mode[i] = Get_Bits(bstr, 2);
	}

	data_unit_loop_length = Get_Bits(bstr, 24);

	while(data_unit_loop_length>0)
	{
		data_unit_len = data_unit(bstr, total_len, buffer, pCaptionManagement);
		if(data_unit_len<0)
			return -1;
		data_unit_loop_length-=data_unit_len;
	}
	
	return 1;

}

int caption_statement_data(NewBstr *bstr, int *total_len, unsigned char *buffer, tCaptionManagement* pCaptionManagement)
{	
	int reserved;
	int data_unit_loop_length;
	int data_unit_len;

	pCaptionManagement->TMD = Get_Bits(bstr, 2);
	reserved = Get_Bits(bstr, 6);

	if(pCaptionManagement->TMD==2 || pCaptionManagement->TMD==1)
	{
		pCaptionManagement->STM = Get_Bits(bstr, 36);
		reserved = Get_Bits(bstr, 4);
	}

	data_unit_loop_length = Get_Bits(bstr, 24);

	while(data_unit_loop_length>0)
	{
		data_unit_len = data_unit(bstr, total_len, buffer, pCaptionManagement);
		if(data_unit_len<0)
			return -1;
		data_unit_loop_length-=data_unit_len;
	}
	
	return 1;
}

int caption_data_group(NewBstr *bstr, int *total_len, unsigned char *buffer, tCaptionManagement* pCaptionManagement)
{	
	int data_group_id;
	int input_len = bstr->m_SrcLength;

	if(input_len<=8)
		return -1;

	pCaptionManagement->data_identifier = Get_Bits(bstr, 8);
	if(pCaptionManagement->data_identifier!=0x80)
		return -1;
	pCaptionManagement->private_stream_id = Get_Bits(bstr, 8);
	if(pCaptionManagement->private_stream_id!=0xFF)
		return -1;

	Get_Bits(bstr, 8);

	data_group_id = pCaptionManagement->data_group_id = Get_Bits(bstr, 6);
	data_group_id &= 0xF;
	pCaptionManagement->data_group_version = Get_Bits(bstr, 2);

	pCaptionManagement->data_group_link_number = Get_Bits(bstr, 8);
	if(pCaptionManagement->data_group_link_number!=0)
		return -1;
	pCaptionManagement->last_data_group_link_number = Get_Bits(bstr, 8);
	if(pCaptionManagement->last_data_group_link_number!=0)
		return -1;
		
	pCaptionManagement->data_group_size = Get_Bits(bstr, 16);

	//8 bytes parsed, 2 bytes crc16
	if(input_len < (pCaptionManagement->data_group_size+8+2))
		return -1;
	
	if(pCaptionManagement->data_group_size>640)
		return -1;

	if(pCaptionManagement->data_group_id==0 || pCaptionManagement->data_group_id==0x20)
	{
		if(caption_management_data(bstr, total_len, buffer, pCaptionManagement)<0)
			return -1;
	}
	else if( (data_group_id>=0x01&&data_group_id<=0x08) )
	{ 	
		//if there is cc	
		if(pCaptionManagement->current_select>0)
		{
			if(pCaptionManagement->current_select>pCaptionManagement->num_languages)
				return -1;
			else
				pCaptionManagement->current_language_tag = pCaptionManagement->language_tag[pCaptionManagement->current_select-1]+1;
		}
		else
		{
			pCaptionManagement->current_language_tag = 0;
		}
		
		//if language selection same as data_group_id
		if(pCaptionManagement->current_language_tag==data_group_id)
		{
			if(caption_statement_data(bstr, total_len, buffer, pCaptionManagement)<0)
				return -1;
		}
		else
			return -1;
	}
	else
		return -1;
		
	return 1;
}

int GetCCData(unsigned char *input, unsigned char *output, int input_len, tCaptionManagement* pCaptionManagement)
{
	NewBstr ts_bstr;
	int total_len = 0;
	InitNewBstr(&ts_bstr, input, input_len);		
	if(caption_data_group(&ts_bstr, &total_len, output, pCaptionManagement)<0)
		total_len = 0;
	return total_len;
}


void InitClosedCaption(tCaptionManagement* pCaptionManagement)
{
	memset(pCaptionManagement, 0, sizeof(tCaptionManagement));
}

int CCGetNumberOfLanguage(tCaptionManagement* pCaptionManagement)
{
	return pCaptionManagement->num_languages;
}

int CCDirectSetLanguage(tCaptionManagement* pCaptionManagement, int mode)
{
	pCaptionManagement->current_select = mode;
	return mode;
}

int CCSelectNextLanguage(tCaptionManagement* pCaptionManagement, int mode)
{
	//if disabled, then enable it
	if(mode==0)
		mode=1;
	else
	{
		mode++;
		if(mode>pCaptionManagement->num_languages)
			mode = 0;
	}	
	pCaptionManagement->current_select = mode;	
	return mode;
}

int SetDefaultManagementData(tCaptionManagement* pCaptionManagement, int language_code)
{
	/*	
	TMD ‘00’ (free)
	num_languages 1 - 2
	language_tag 0 - 1
	DMF
	‘1010’ (the choice of caption and display on reception
	and the choice of caption and display on recording
	playback)
	ISO_639_language_code Used language code (“jpn” fixed)
	Format Not operated ‘1111’
	TCS ‘00’ (8-bit character codes)
	rollup_mode ‘00’ (non-rollup)
	data_unit_loop_length ‘0’
	data_unit Do not allocate data units.
	*/
	pCaptionManagement->current_language_tag = 0;
	pCaptionManagement->TMD = 0;
	pCaptionManagement->num_languages = 1;
	pCaptionManagement->language_tag[0] = 0;
	pCaptionManagement->DMF[0] = 0xA;
	pCaptionManagement->ISO_639_language_code[0] = language_code;
	pCaptionManagement->Format[0] = 0xF;
	pCaptionManagement->TCS[0] = 0;
	pCaptionManagement->rollup_mode[0] = 0;

	pCaptionManagement->current_select = 1;

	return 0;
}