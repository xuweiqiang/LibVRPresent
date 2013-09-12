#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cc_decode.h"

#define SAFEFREE(x)           if (x != NULL) {free(x); x = NULL;}

IsdbtCCDecode::IsdbtCCDecode()
{
	m_uiDataLen = 0;
	m_uiDecodedLen = 0;
	memset(m_aData, 0, sizeof(m_aData));
	memset(m_aDecodedData, 0, sizeof(m_aDecodedData));

	InitClosedCaption(&m_tCaptionMgn);	
	m_pFontDraw = NULL;

	m_FrameBuffer = NULL;
}

IsdbtCCDecode::~IsdbtCCDecode()
{
	if(m_pFontDraw)
	{
		delete m_pFontDraw;
	}
}


STDMETHODIMP IsdbtCCDecode::Open(ISDBTCC_OpenOptions* option)
{
	SetDefaultManagementData(&m_tCaptionMgn, option->dwDefaultLanguageCode);
	m_pFontDraw = new FontDraw();	

	m_pFontDraw->SetOutputFormat(option->OutputFormat);
	if(m_pFontDraw->Open(option->dwWidth, option->dwHeight, option->dwOutputBufferNumber)<0)
		return E_FAIL;
	m_pFontDraw->SetFont(option->dwDefaultLanguageCode, option->dwFontWidth, option->dwFontHeight);
	m_pFontDraw->SetMaxPos(option->dwMaxLines, option->dwCharacterPerLine);
	m_pFontDraw->SetUpsideDown((BOOL)(!!option->dwFlags&ISDBTCC_FLAG_OUTPUT_UPSIDEDOWN));

	//create output buffer
	m_dwBufferNum = option->dwOutputBufferNumber;
	if(m_dwBufferNum==0)
		m_dwBufferNum=1;
	m_dwBufferIndex = 0;

	m_FrameBuffer = (FrameBuffer*)malloc(sizeof(FrameBuffer)*m_dwBufferNum);

	if(option->OutputFormat==YV12)
	{
		for(unsigned int i=0;i<m_dwBufferNum;i++)
		{
			memset(&m_FrameBuffer[i], 0, sizeof(m_FrameBuffer[0]));

			m_FrameBuffer[i].m_OutBuffer[0] = (BYTE*)malloc(option->dwWidth*option->dwHeight*3/2);
			m_FrameBuffer[i].m_OutBuffer[1] = m_FrameBuffer[i].m_OutBuffer[0] + option->dwWidth*option->dwHeight;
			m_FrameBuffer[i].m_OutBuffer[2] = m_FrameBuffer[i].m_OutBuffer[1] + option->dwWidth*option->dwHeight/4;
			m_FrameBuffer[i].m_OutBuffer[3] = NULL;

			m_FrameBuffer[i].m_dwWidth[0] = option->dwWidth;
			m_FrameBuffer[i].m_dwWidth[1] = option->dwWidth>>1;
			m_FrameBuffer[i].m_dwWidth[2] = option->dwWidth>>1;

			m_FrameBuffer[i].m_dwHeight[0] = option->dwHeight;
			m_FrameBuffer[i].m_dwHeight[1] = option->dwHeight>>1;
			m_FrameBuffer[i].m_dwHeight[2] = option->dwHeight>>1;

			m_FrameBuffer[i].m_dwStride[0] = option->dwWidth;
			m_FrameBuffer[i].m_dwStride[1] = option->dwWidth>>1;
			m_FrameBuffer[i].m_dwStride[2] = option->dwWidth>>1;
		}
	}
	if(option->OutputFormat==XRGB)
	{
		for(unsigned int i=0;i<m_dwBufferNum;i++)
		{
			memset(&m_FrameBuffer[i], 0, sizeof(m_FrameBuffer[0]));

			m_FrameBuffer[i].m_OutBuffer[0] = (PBYTE)malloc(option->dwWidth*option->dwHeight*4);
			m_FrameBuffer[i].m_OutBuffer[1] = NULL;
			m_FrameBuffer[i].m_OutBuffer[2] = NULL;
			m_FrameBuffer[i].m_OutBuffer[3] = NULL;

			m_FrameBuffer[i].m_dwWidth[0] = option->dwWidth;
			m_FrameBuffer[i].m_dwHeight[0] = option->dwHeight;
			m_FrameBuffer[i].m_dwStride[0] = option->dwWidth;			
		}
	}

	memcpy(&m_tOpenOption, option, sizeof(m_tOpenOption));
	return S_OK;
}

STDMETHODIMP IsdbtCCDecode::Close()
{
	InitClosedCaption(&m_tCaptionMgn);
	if(m_pFontDraw)
	{
		if(m_tOpenOption.OutputFormat==YV12)
		{
			for(unsigned int i=0;i<m_dwBufferNum;i++)
			{
				SAFEFREE(m_FrameBuffer[i].m_OutBuffer[0]);				
			}
		}
		if(m_tOpenOption.OutputFormat==XRGB)
		{
			for(unsigned int i=0;i<m_dwBufferNum;i++)
			{
				SAFEFREE(m_FrameBuffer[i].m_OutBuffer[0]);				
			}
		}

		SAFEFREE(m_FrameBuffer);
		m_FrameBuffer = 0;

		m_pFontDraw->Close();
		delete m_pFontDraw;
		m_pFontDraw = NULL;
	}
	return S_OK;
}

int IsdbtCCDecode::Draw(unsigned int *data, unsigned int length, int index)
{	
	return m_pFontDraw->DMGL_StartDraw(0, 0, data, length, m_FrameBuffer[index].m_OutBuffer);		
}

STDMETHODIMP IsdbtCCDecode::Decode(PBYTE pData, DWORD *pLen, ISDBTCC_DecodeOptions* option)
{
	unsigned int language_code1, language_code2;

	//check if there is remaining data in the buffer
	if(m_uiDecodedLen>0)
	{	
		*pLen = 0;
		return E_PENDING;
	}

	language_code1 = m_tCaptionMgn.ISO_639_language_code[m_tCaptionMgn.current_select-1];

	//get the data unit
	m_uiDataLen = GetCCData(pData, (unsigned char*)m_aData, int(*pLen), &m_tCaptionMgn);	

	language_code2 = m_tCaptionMgn.ISO_639_language_code[m_tCaptionMgn.current_select-1];
	if (language_code1 != language_code2)
	{
		m_pFontDraw->SetFont(language_code2, m_tOpenOption.dwFontWidth, m_tOpenOption.dwFontHeight);
	}
	
	//translate to the codeword drawing system understands
	if (language_code2 == 'jpn')  // jpn
	{
		m_uiDecodedLen = CharacterDecode_JIS(m_aDecodedData, sizeof(m_aDecodedData)/sizeof(m_aDecodedData[0]), (unsigned char*)m_aData, m_uiDataLen, DRCS_FONT, KANJI, (void*)m_tCaptionMgn.m_DRCS1);
	}
	else if (language_code2 == 'por' || language_code2 == 'bra')  // por
	{
		m_uiDecodedLen = CharacterDecode_Latin(m_aDecodedData, sizeof(m_aDecodedData)/sizeof(m_aDecodedData[0]), (unsigned char*)m_aData, m_uiDataLen, ALPHANUMERIC_LATIN, LATIN_EXTENSION, (void*)m_tCaptionMgn.m_DRCS1);		
	}
	else
		return E_FAIL;

	m_uiLastTimeStamp = option->dwTimeStamp;

	*pLen = m_uiDataLen;
	return S_OK;
}

STDMETHODIMP IsdbtCCDecode::GetOutput(ISDBTCC_Output* pOutput)
{	
	DWORD dwTimeStamp;
	DWORD frame_index = m_dwBufferIndex;
	if(!m_pFontDraw)
		return E_FAIL;
	if(m_uiDecodedLen<=0)
		return E_FAIL;

	dwTimeStamp = m_uiLastTimeStamp;

	//check if there is remaining data in the buffer
	if(m_uiDecodedLen>0)
	{
		int used_length;
		int time_code;

		time_code = m_pFontDraw->DMGL_CheckTimeCode(m_aDecodedData, m_uiDecodedLen, &used_length);
		Draw(m_aDecodedData, used_length, m_dwBufferIndex);
		m_dwBufferIndex++;
		if(m_dwBufferIndex>=m_dwBufferNum)
			m_dwBufferIndex = 0;

		if(time_code)
		{
			dwTimeStamp += time_code;
			memmove(m_aDecodedData, m_aDecodedData+used_length, (m_uiDecodedLen-used_length)*sizeof(m_aDecodedData[0]));
			m_uiDecodedLen -= used_length;
			if(m_uiDecodedLen<=0)
			{
				m_uiDecodedLen = 0;
			}
			if (m_tOpenOption.dwOutputBufferNumber > 1)
			{
				if (m_tOpenOption.OutputFormat == YV12)
				{
					memcpy(m_FrameBuffer[m_dwBufferIndex].m_OutBuffer[0], m_FrameBuffer[frame_index].m_OutBuffer[0], m_tOpenOption.dwWidth*m_tOpenOption.dwHeight*3/2);
				}
				else if (m_tOpenOption.OutputFormat == XRGB)
				{
					memcpy(m_FrameBuffer[m_dwBufferIndex].m_OutBuffer[0], m_FrameBuffer[frame_index].m_OutBuffer[0], m_tOpenOption.dwWidth*m_tOpenOption.dwHeight*4);
				}
			}
		}
		else
		{
			m_uiDecodedLen = 0;
			m_pFontDraw->SetPosition(0, 0);
		}
	}
	
	for(int i=0;i<3;i++)
	{
		pOutput->pBuffer[i]	 = m_FrameBuffer[frame_index].m_OutBuffer[i];
		pOutput->dwWidth[i]	 = m_FrameBuffer[frame_index].m_dwWidth[i];
		pOutput->dwHeight[i] = m_FrameBuffer[frame_index].m_dwHeight[i];
		pOutput->dwStride[i] = m_FrameBuffer[frame_index].m_dwStride[i];
	}

	pOutput->OutputFormat = m_tOpenOption.OutputFormat;
	pOutput->dwTimeStamp = m_uiLastTimeStamp;

	//update m_uiLastTimeStamp if there is time_code
	m_uiLastTimeStamp = dwTimeStamp;

	if(pOutput->OutputFormat==YV12)
		pOutput->dwBitsPerPixel = 8;
	else if(pOutput->OutputFormat==XRGB)
		pOutput->dwBitsPerPixel = 32;

	return S_OK;
}


STDMETHODIMP IsdbtCCDecode::Reset()
{
	m_uiDataLen = 0;
	m_uiDecodedLen = 0;

	memset(m_aData, 0, sizeof(m_aData));
	memset(m_aDecodedData, 0, sizeof(m_aDecodedData));

	return 0;
}


STDMETHODIMP IsdbtCCDecode::Set(DWORD dwPropID, LPVOID pPropData, DWORD cbPropData)
{
	switch(dwPropID)
	{
	case ISDBTCC_DEFAULT_LANGUAGE_CODE:
		if(m_tCaptionMgn.num_languages == 0)
		{
			SetDefaultManagementData(&m_tCaptionMgn, *(DWORD*)pPropData);
			return S_OK;
		}
		return E_FAIL;

	case ISDBTCC_FONT_BACKGROUND_COLOR:
		{
			if(!m_pFontDraw)
				return E_FAIL;
			m_pFontDraw->SetFontBackgroundColor(ISDBTCC_DEFAULT_COLOR(*(DWORD*)pPropData));		
		}
		return S_OK;

	case ISDBTCC_FONT_COLOR:
		{
			if(!m_pFontDraw)
				return E_FAIL;
			m_pFontDraw->SetFontColor(ISDBTCC_DEFAULT_COLOR(*(DWORD*)pPropData));		
		}
		return S_OK;

	case ISDBTCC_BACKGROUND_COLOR:
		{
			if(!m_pFontDraw)
				return E_FAIL;
			m_pFontDraw->SetBackgroundColor(ISDBTCC_DEFAULT_COLOR(*(DWORD*)pPropData));
		}
		return S_OK;

	case ISDBTCC_BACKGROUND_COLOR_CUSTOMIZED:
		{
			if(!m_pFontDraw)
				return E_FAIL;
			if(cbPropData!=sizeof(DWORD)*4)
				return E_FAIL;

			int color[4];
			color[0] = *((DWORD*)pPropData+0);
			color[1] = *((DWORD*)pPropData+1);
			color[2] = *((DWORD*)pPropData+2);
			color[3] = *((DWORD*)pPropData+3);
			m_pFontDraw->SetBackgroundColor(color);
		}
		return S_OK;

	case ISDBTCC_FONT_COLOR_CUSTOMIZED:
		{
			if(!m_pFontDraw)
				return E_FAIL;
			if(cbPropData!=sizeof(DWORD)*4)
				return E_FAIL;

			int color[4];
			color[0] = *((DWORD*)pPropData+0);
			color[1] = *((DWORD*)pPropData+1);
			color[2] = *((DWORD*)pPropData+2);
			color[3] = *((DWORD*)pPropData+3);
			m_pFontDraw->SetFontColor(color);
		}
		return S_OK;

	case ISDBTCC_FONT_BACKGROUND_COLOR_CUSTOMIZED:
		{
			if(!m_pFontDraw)
				return E_FAIL;
			if(cbPropData!=sizeof(DWORD)*4)
				return E_FAIL;

			int color[4];
			color[0] = *((DWORD*)pPropData+0);
			color[1] = *((DWORD*)pPropData+1);
			color[2] = *((DWORD*)pPropData+2);
			color[3] = *((DWORD*)pPropData+3);
			m_pFontDraw->SetFontBackgroundColor(color);
		}
		return S_OK;

	case ISDBTCC_LANGUAGE_SELECT:
		m_tCaptionMgn.current_select = *(DWORD*)pPropData;
		return S_OK;
	};
	return E_NOTIMPL;
}

STDMETHODIMP IsdbtCCDecode::Get(DWORD dwPropID, LPVOID pPropData, DWORD cbPropData)
{
	switch(dwPropID)
	{
	case ISDBTCC_LANGUAGE_NUM:
		*(DWORD*)pPropData = m_tCaptionMgn.num_languages;
		return S_OK;			

	case ISDBTCC_LANGUAGE_SELECT:
		*(DWORD*)pPropData = m_tCaptionMgn.current_select;		
		return S_OK;

	case ISDBTCC_LANGUAGE_CODES:
		{
			if(cbPropData < sizeof(DWORD)*m_tCaptionMgn.num_languages)
				return E_FAIL;
			for(int i=0;i<m_tCaptionMgn.num_languages;i++)
			{
				*((DWORD*)pPropData+i)=m_tCaptionMgn.ISO_639_language_code[i];
			}
		}
		return S_OK;
	};
	return E_NOTIMPL;
}