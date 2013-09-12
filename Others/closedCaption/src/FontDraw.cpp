#include <stdlib.h>
#include <string.h>
#include "dmgl_jis.h"
#include "draw.h"
#include "FontDraw.h"
#include "jis_font16_1bit.h"
#include "alpha_font16_1bit.h"
#include "latin_ext_font16_1bit.h"

#define SAFEFREE(x)           if (x != NULL) {free(x); x = NULL;}

FontDraw::FontDraw()
{
	m_dwLanguageCode = 0;
	m_OutType = YV12; //set default output type to YV12
	m_FontData = 0;	
	m_pDraw = NULL;
	m_FontWidth = m_FontHeight = 0;
}

FontDraw::~FontDraw()
{
	Close();
}

DWORD FontDraw::GetFontType() 
{
	return m_dwLanguageCode;
}

int FontDraw::Open(DWORD width, DWORD height, DWORD num_of_buffer)
{
	m_pDraw = CreateFontDraw(m_OutType);

	if(!m_pDraw)
		return -1;

	return m_pDraw->Open(width, height, num_of_buffer);	
}

int FontDraw::Close()
{
	if(m_pDraw)
	{
		m_pDraw->Close();
		delete m_pDraw;
		m_pDraw = NULL;
	}

	return 0;
}

int FontDraw::Clear()
{
	m_pDraw->DrawClear();
	m_pDraw->SetPosition(0, 0);

	return 0;
}

void FontDraw::SetFontBackgroundColor(int color[4])
{
	m_pDraw->SetFontBackgroundColor(color);
}

void FontDraw::SetFontBackgroundColor(ISDBTCC_DEFAULT_COLOR default_color)
{
	m_pDraw->SetFontBackgroundColor(default_color);
}

void FontDraw::SetFontColor(int color[4])
{
	m_pDraw->SetFontColor(color);
}

void FontDraw::SetFontColor(ISDBTCC_DEFAULT_COLOR default_color)
{
	m_pDraw->SetFontColor(default_color);
}

void FontDraw::SetBackgroundColor(int color[4])
{
	m_pDraw->SetBackgroundColor(color);
}

void FontDraw::SetBackgroundColor(ISDBTCC_DEFAULT_COLOR default_color)
{
	m_pDraw->SetBackgroundColor(default_color);
}

void FontDraw::SetOutputFormat(OUT_FORMAT format)
{
	m_OutType = format;
}

void FontDraw::SetFont(DWORD language_code, DWORD font_width, DWORD font_height)
{
	int font_data_size;

	switch (language_code)
	{	
	case 'por':
	case 'bra':
		m_FontData = (unsigned char*)LatinEXt_Font_1bit;
		font_data_size = sizeof(LatinEXt_Font_1bit);
		font_width = 8;
		font_height = 16;
		break;
	case 'jpn':
	default:
#ifndef USE_APPHIC_FONT
		m_FontData = (unsigned char*)JIS_Font16_1bit_NEC;
		font_data_size = sizeof(JIS_Font16_1bit_NEC);
#else
		m_FontData = (unsigned char*)JIS_Font16_1bit_ARPHIC;
		font_data_size = sizeof(JIS_Font16_1bit_ARPHIC);
#endif
		font_width = 16;
		font_height = 16;
		break;
	}	

	m_dwLanguageCode = language_code;
	m_FontWidth = font_width;
	m_FontHeight = font_height;
	m_FontMaxIndex = font_data_size/((font_width>>3)*font_height);

	m_pDraw->SetFontSize(font_width, font_height);	
}

void FontDraw::SetPosition(int x, int y)
{
	m_pDraw->SetPosition(x, y);	
}

void FontDraw::SetMaxPos(int lines, int fonts_perline)
{
	m_pDraw->SetMaxPos(lines, fonts_perline);	
}

void FontDraw::GetFrameBuffer(unsigned char *buffer[3])
{
	unsigned char* p[3];
	
	buffer[0] = buffer[1] = buffer[2] = 0;

	if(m_pDraw)
	{
		m_pDraw->GetOutputBuffer(p);
		buffer[0] = p[0];
		buffer[1] = p[1];
		buffer[2] = p[2];	
	}
}

int FontDraw::DMGL_CheckFLCCode(int *input, int *output, int length)
{
	int *data = input;
	int *out_start = output;
	int *end = input+length;
	int ret = 0;	
//	int out_len = 0;
	int flc_start = 0;
	int flc_end = 0;

	while(data<end)
	{
		switch (data[0])
		{
		case I_FLC_ON:
			data++;
			flc_start = 1;
			break;

		case I_FLC_OFF:
			data++;
			flc_end = 1;
			break;

		case I_DRCS:				
			data+=4;
			break;

		default:
			data++;
			break;
		}
	}

	//no FLC, return 0
	if(flc_start==0 || flc_end==0)
		return 0;

	flc_start = flc_end = 0;
	data = input;

	while(data<end)
	{
		if(data[0] < I_CS)
		{		
			if(flc_start)
				*output++ = I_SP;
			else
				*output++ = data[0];			
			data++;
		}
		else
		{
			//special code
			switch (data[0])
			{
			case I_CS:				
				*output++ = *data++;
				break;

			case I_APR:				
				*output++ = *data++;
				break;

			case I_SP:				
				*output++ = *data++;
				break;

			case I_DEL:				
				if(flc_start)
					*output++ = I_SP;
				else
					*output++ = data[0];
				data++;
				break;

			case I_BKF:				
				data++;
				break;

			case I_RDF:				
				data++;
				break;

			case I_GRF:				
				data++;
				break;

			case I_YLF:				
				data++;
				break;

			case I_BLF:				
				data++;
				break;

			case I_MGF:				
				data++;
				break;

			case I_CNF:				
				data++;
				break;

			case I_WHF:				
				data++;
				break;

			case I_FLC_ON:
				flc_start = 1;
				data++;
				break;

			case I_FLC_OFF:
				flc_start = 0;
				data++;
				break;

			case I_MSZ:
				data++;
				break;

			case I_NSZ:
				data++;
				break;

			case I_TIM:
				data++;
				break;

			case I_DRCS:
				if(flc_start)
				{
					*output++ = I_SP;
				}
				else
				{
					*output++ = data[0];
					*output++ = data[1];
					*output++ = data[2];
					*output++ = data[3];
				}				
				data+=4;
				break;
			default:				
				data++;
				break;
			}
		}

		if(ret<0)
			break;
	}
	
	return (int)(output-out_start);
}

int FontDraw::DMGL_CheckTimeCode(unsigned int *data, int length, int *used_length)
{
	unsigned int *end = data+length;
	int ret = 0;

	//SetPosition(start_x, start_y);

	while(data<end)
	{
		if(data[0] < I_CS)
		{		
			data++;
		}
		else
		{
			//special code
			switch (data[0])
			{
			case I_CS:				
				data++;
				break;

			case I_APR:				
				data++;
				break;

			case I_SP:				
				data++;
				break;

			case I_DEL:				
				data++;
				break;

			case I_BKF:				
				data++;
				break;

			case I_RDF:				
				data++;
				break;

			case I_GRF:				
				data++;
				break;

			case I_YLF:				
				data++;
				break;

			case I_BLF:				
				data++;
				break;

			case I_MGF:				
				data++;
				break;

			case I_CNF:				
				data++;
				break;

			case I_WHF:				
				data++;
				break;

			case I_FLC_ON:
				data++;
				break;

			case I_FLC_OFF:
				data++;
				break;

			case I_MSZ:
				data++;
				break;

			case I_NSZ:
				data++;
				break;

			case I_TIM:
				{
				int time_code;

				time_code = data[1];
				time_code *= 100;
				data+=2;

				//if time_code is 0, don't do anything
				if(time_code)
				{
					*used_length = length-(int)(end-data);
					return time_code;
				}
				}
				break;

			case I_DRCS:				
				data+=4;
				break;
			default:
				data++;
				break;
			}
		}

		if(ret<0)
			break;
	}

	*used_length = length-(int)(end-data);
	return 0;
}


int FontDraw::DMGL_StartDraw(int start_x, int start_y, unsigned int *data, int length, PBYTE buffer[3])
{
	unsigned int *end = data+length;
	int ret = 0;

	m_pDraw->SetOutputBuffer(buffer);

	//SetPosition(start_x, start_y);

	while(data<end)
	{
		if(data[0] < I_CS)
		{
			int font_index;
			unsigned char *font_address;

			font_index = data[0];

			if(font_index>=0 && font_index<m_FontMaxIndex)
			{
				font_address = m_FontData + font_index*(m_FontWidth>>3)*m_FontHeight;
				ret = m_pDraw->DrawFont(font_address, m_FontWidth, m_FontHeight);
			}
			data++;
		}
		else
		{
			//special code
			switch (data[0])
			{
			case I_CS:
				ret = Clear();
				data++;
				break;

			case I_APR:
				ret = m_pDraw->NewLine();
				data++;
				break;

			case I_SP:
				ret = m_pDraw->DrawSpace();
				data++;
				break;

			case I_DEL:
				ret = m_pDraw->DrawDel();
				data++;
				break;

			case I_BKF:
				m_pDraw->SetFontColor(CC_BLACK);
				data++;
				break;

			case I_RDF:
				m_pDraw->SetFontColor(CC_RED);
				data++;
				break;

			case I_GRF:
				m_pDraw->SetFontColor(CC_GREEN);
				data++;
				break;

			case I_YLF:
				m_pDraw->SetFontColor(CC_YELLOW);
				data++;
				break;

			case I_BLF:
				m_pDraw->SetFontColor(CC_BLUE);
				data++;
				break;

			case I_MGF:
				m_pDraw->SetFontColor(CC_MAGENTA);
				data++;
				break;

			case I_CNF:
				m_pDraw->SetFontColor(CC_CYAN);
				data++;
				break;

			case I_WHF:
				m_pDraw->SetFontColor(CC_WHITE);
				data++;
				break;

			case I_FLC_ON:
				data++;
				break;

			case I_FLC_OFF:
				data++;
				break;

			case I_MSZ:
				data++;
				break;

			case I_NSZ:
				data++;
				break;

			case I_TIM:
				data+=2;
				break;

			case I_DRCS:
				m_pDraw->DrawUserData((unsigned char*)data[3], data[1], data[2]);
				data+=4;
				break;
			default:
				data++;
				break;
			}
		}

		if(ret<0)
			break;
	}
	m_pDraw->CenterThisLine();
	m_pDraw->AlignToBottom();

	return 0;
}