#include "SubtitleDecoderDVBT.h"

#define OUTPUT_W 720
#define OUTPUT_H 576
#define SUB_INPUT_BUFFER_SIZE (1024*42)
#define SUBTITLE_OUT_BUFFER_LENGTH (OUTPUT_W*OUTPUT_H*3/2)

CSubtitleDecoderDVBT::CSubtitleDecoderDVBT() :
m_hSubtitleDecoderEngine(NULL),
m_LockedBufferLength(0),
m_lSubtitleInfoChanged(0),
m_LastRemain(0),
m_hDecoder(NULL),
m_fExit(FALSE),
m_LastSample(NULL)
{
	memset(&m_Region, 0, sizeof(m_Region));
	for(int i = 0; i < SAMPLE_BUFFER_SIZE; i ++)
	{
		m_pBuffer[i] = NULL;
	}
}

CSubtitleDecoderDVBT::~CSubtitleDecoderDVBT()
{
}



void CSubtitleDecoderDVBT::DrawSample(CI_SUB_Draw_Info *pDrawInfo, void *pContext)
{
	CSubtitleDecoderDVBT *pThis = (CSubtitleDecoderDVBT*)pContext;
	pThis->DrawSampleIner(pDrawInfo);
}

void CSubtitleDecoderDVBT::DrawSampleIner(CI_SUB_Draw_Info *pDrawInfo)
{
	if(m_fExit)
		return;
	//Check last sample pointer
	while(m_LastSample == NULL)
	{
		if(m_fExit)	return;
		m_LastSample = GetFreeSample();
		if(m_LastSample)
			((CSubtitleSample*)m_LastSample)->AddRef();
		Sleep(10);

	}
	// Draw this frame
	if(!m_Region.bottom || !m_Region.right)
	{
		m_lSubtitleInfoChanged = 1;
		m_Region.top = 0;
		m_Region.bottom = OUTPUT_H;
		m_Region.left = 0;
		m_Region.right = OUTPUT_W;
		m_tSubtitleInfo.adwOutputWidth[0] = OUTPUT_W;
		m_tSubtitleInfo.adwOutputWidth[1] = m_tSubtitleInfo.adwOutputWidth[2] = OUTPUT_W / 2;
		m_tSubtitleInfo.adwOutputHeight[0] = OUTPUT_H;
		m_tSubtitleInfo.adwOutputHeight[1] = m_tSubtitleInfo.adwOutputHeight[2] = OUTPUT_H / 2;
	}
	LPBYTE pBuffer[3];	
	((CSubtitleSample*)m_LastSample)->Lock(pBuffer);
	CopyToDestBuffer((char*)pBuffer[0], (char*)pDrawInfo->pRegionBuffer, pDrawInfo->DispRect);
	((CSubtitleSample*)m_LastSample)->Unlock();
	
	// Save one copy to m_LastSample, and put finished sample to sample queue. 
	if(pDrawInfo->RegionIndex == pDrawInfo->TotalRegions && pDrawInfo->PTS != -1)
	{
		CSubtitleSample* pSample = NULL;	
		while(pSample == NULL)
		{
			if(m_fExit)	return;
			pSample = (CSubtitleSample*)GetFreeSample();
			if(pSample)
				pSample->AddRef();
			Sleep(10);
		}
		((CSubtitleSample*)m_LastSample)->SavePTS(pDrawInfo->PTS);

		LPBYTE Src[3], Dst[3];
		((CSubtitleSample*)m_LastSample)->Lock(Src);
		((CSubtitleSample*)pSample)->Lock(Dst);
		memcpy(Dst[0], Src[0], SUBTITLE_OUT_BUFFER_LENGTH);
		((CSubtitleSample*)pSample)->Unlock();
		((CSubtitleSample*)m_LastSample)->Unlock();

		m_Sample.Put(m_LastSample);

		m_LastSample = pSample;
	}
}


STDMETHODIMP CSubtitleDecoderDVBT::Open(LPVOID pParam, DWORD cbParam)
{
	if (FAILED(CBaseSubtitleDecoder::Open(pParam, cbParam)))
	{
		Close();
		return E_FAIL;
	}

	m_hSubtitleDecoderEngine = LoadLibrary(TEXT("libSUBDecoder.dll"));
	if(m_hSubtitleDecoderEngine == NULL)
	{
		return E_FAIL;
	}
#ifdef UNDER_CE
	ISUBDecoder* (*pfnCreateSUBDecoder)(void) = (ISUBDecoder* (*)(void))
		GetProcAddress(m_hSubtitleDecoderEngine, _T("CreateSUBDecoder"));
#else
	ISUBDecoder* (*pfnCreateSUBDecoder)(void) = (ISUBDecoder* (*)(void))
		GetProcAddress(m_hSubtitleDecoderEngine, "CreateSUBDecoder");
#endif
	if(!pfnCreateSUBDecoder)
	{
		Close();
		return E_FAIL;
	}
	m_hDecoder = pfnCreateSUBDecoder();
	if(!m_hDecoder)
	{
		Close();
		return E_FAIL;
	}
	CI_SUB_Open_Option OpenParam;
	OpenParam.SrcRect.lTop = 0;
	OpenParam.SrcRect.lBottom = OUTPUT_H;
	OpenParam.SrcRect.lLeft = 0;
	OpenParam.SrcRect.lRight = OUTPUT_W;
	OpenParam.ColorType = CI_SUB_COLOR_SPACE_YCBCRT_HDTV;
	OpenParam.usAncillaryPageID = 1;
	OpenParam.usCompositionPageID = 1;
	if(FAILED(m_hDecoder->Open((DWORD)&OpenParam)))
	{
		Close();
		return E_FAIL;
	}
	static GUID EmptyGuid;
	if(FAILED(m_hDecoder->Set(EmptyGuid, EMC_PROPID_SUBTITLE_SETDRAWCB, this, 0, (LPVOID)DrawSample, 0)))
	{
		Close();
		return E_FAIL;
	}
	if(FAILED(m_hDecoder->Set(EmptyGuid, EMC_PROPID_SUBTITLE_DISP_ON, NULL, 0, NULL, 0)))
	{
		Close();
		return E_FAIL;
	}	
	if(FAILED(m_hDecoder->Set(EmptyGuid, EMC_PROPID_SUBTITLE_NOSYNC, NULL, 1, NULL, 0)))
	{
		Close();
		return E_FAIL;
	}
	memset(&m_tSubtitleInfo, 0, sizeof(EMC_SUBTITLE_INFO));
	m_tSubtitleInfo.adwBackgroundColor[0] = 0x80;
	m_tSubtitleInfo.adwBackgroundColor[1] = 0x80;
	m_tSubtitleInfo.adwBackgroundColor[2] = 0x80;
	m_tSubtitleInfo.eSubtitleOutputType = EMC_SUBTITLE_OUTPUT_TYPE_YUV;
	InitSamples();

	m_fExit = FALSE;
	return S_OK;
}

STDMETHODIMP CSubtitleDecoderDVBT::Close()
{
	m_fExit = TRUE;
	if(m_hDecoder)
		m_hDecoder->Close();
	CleanResource();
	return CBaseSubtitleDecoder::Close();
}

STDMETHODIMP CSubtitleDecoderDVBT::GetSampleWithTimeStamp(IEmcMediaSample **ppSample, TTimeStamp* pTSNode)
{	
	CTimeStamp*  pTimeStamp = NULL;
	if (m_ulLocalDataSize == 0)
	{
		ReloadStreamBuffer();
	}
	if (m_ulLocalDataSize > 0)
	{
		DWORD  dwUsedSize = m_ulLocalDataSize;
		m_hDecoder->SendES(m_pucLocalBuffer, dwUsedSize, (DWORD)m_tCurrentTimeStamp.llTimeStamp);
		UpdateStreamBuffer((unsigned int*)&dwUsedSize);
	}

	if(m_Sample.Size())
	{
		CSubtitleSample* pSample = m_Sample.Get();

		if(m_lSubtitleInfoChanged)
		{
			if (m_pMediaType == NULL)
				m_pMediaType = new CEmcMediaType(0, 0, &EMC_FORMAT_SUBTITLE_INFO, &m_tSubtitleInfo, sizeof(EMC_SUBTITLE_INFO));
			else
				m_pMediaType->SetFormatBuffer((LPBYTE)&m_tSubtitleInfo, sizeof(EMC_SUBTITLE_INFO));
			pSample->SetMediaType(m_pMediaType);
			m_lSubtitleInfoChanged = 0;
		}
		*ppSample = pSample;
		pTSNode->llTimeStamp = pSample->GetPTS();
		return S_OK;
	}
	return E_FAIL;
}

void CSubtitleDecoderDVBT::CleanResource()
{
	DeinitSamples();

	if(m_hDecoder)
	{
		m_hDecoder->Release();
		m_hDecoder = NULL;
	}
	if(m_hSubtitleDecoderEngine)
	{
		FreeLibrary(m_hSubtitleDecoderEngine);
		m_hSubtitleDecoderEngine = NULL;
	}
}

void CSubtitleDecoderDVBT::CopyToDestBuffer(char* pDest, char* pSrc, CI_SUB_Rect &DispRect)
{
	char *DestY, *DestU, *DestV;
	char *SrcY, *SrcU, *SrcV;
	long DestStretch = OUTPUT_W;
	long SrcStretch = DispRect.lRight - DispRect.lLeft;
	long TotalLine = DispRect.lBottom - DispRect.lTop;
	
	DestY = pDest + DestStretch * DispRect.lTop + DispRect.lLeft;	
	DestU = pDest + DestStretch * OUTPUT_H + // skip Y
		((DestStretch>>1) * ((DispRect.lTop+1)>>1)) + (DispRect.lLeft>>1);
	DestV = DestU + (DestStretch>>1)*(OUTPUT_H>>1);	
	
	if(pSrc)
	{
		SrcY = (char*)pSrc;
		SrcU = (char*)pSrc + TotalLine*SrcStretch;
		SrcV = (char*)SrcU + (TotalLine>>1)*(SrcStretch>>1);
	

		//Luma
		for(int i = 0; i < TotalLine; i ++)
		{		
			memcpy(DestY, SrcY, SrcStretch);			
			DestY += DestStretch;
			SrcY += SrcStretch;			
		}

		//Chroma		
		for(int i = 0; i < TotalLine>>1; i ++)
		{			
			memcpy(DestU, SrcU, SrcStretch>>1);
			memcpy(DestV, SrcV, SrcStretch>>1);			
			SrcU += SrcStretch>>1;
			SrcV += SrcStretch>>1;
			DestU += DestStretch>>1;
			DestV += DestStretch>>1;
		}		
	}
	else
	{
		for(int i = 0; i < TotalLine; i ++)
		{
			memset(DestY, 0x80, SrcStretch);			
			DestY += DestStretch;
		}
		for(int i = 0; i < TotalLine>>1; i ++)
		{
			memset(DestU, 0x80, SrcStretch>>1);
			memset(DestV, 0x80, SrcStretch>>1);
			DestU += DestStretch>>1;
			DestV += DestStretch>>1;
		}
	}
}

void CSubtitleDecoderDVBT::InitSamples()
{
	for(int i = 0; i < SAMPLE_BUFFER_SIZE; i ++)
	{
		m_pBuffer[i] = new char[SUBTITLE_OUT_BUFFER_LENGTH];
		memset(m_pBuffer[i], 0x80, SUBTITLE_OUT_BUFFER_LENGTH);
		CSubtitleSample* pSample = new CSubtitleSample(this);
		LPBYTE pBuffer[3];
		DWORD  aulSubtitleYuvSize[3];
		pBuffer[0] = (LPBYTE)m_pBuffer[i];
		pBuffer[1] = (LPBYTE)m_pBuffer[i] + OUTPUT_W*OUTPUT_H;
		pBuffer[2] = (LPBYTE)m_pBuffer[i] + OUTPUT_W*OUTPUT_H * 5 /4;;
		aulSubtitleYuvSize[0] = OUTPUT_W*OUTPUT_H;
		aulSubtitleYuvSize[1] = aulSubtitleYuvSize[2] = OUTPUT_W*OUTPUT_H/4;
		pSample->SetUnmanagedBuffer(pBuffer, aulSubtitleYuvSize);
		// Release it will make it enter free sample list
		pSample->Release();
	}
}

void CSubtitleDecoderDVBT::PutFreeSampel(CSubtitleSample* pSample)
{
	m_FreeSample.Put(pSample);
}

CSubtitleSample* CSubtitleDecoderDVBT::GetFreeSample()
{
	return m_FreeSample.Get();
}

void CSubtitleDecoderDVBT::DeinitSamples()
{
	while(m_Sample.Size())
	{
		CSubtitleSample* pSample = m_Sample.Get();
		pSample->Release();
	}
	while(m_FreeSample.Size())
	{
		CSubtitleSample* pSample = m_FreeSample.Get();
		delete pSample;
	}
	if(m_LastSample)
	{
		delete m_LastSample;
		m_LastSample = NULL;
	}
	for(int i = 0; i < SAMPLE_BUFFER_SIZE; i ++)
	{
		if(m_pBuffer[i] != NULL)
		{
			delete [] m_pBuffer[i];
			m_pBuffer[i] = NULL;
		}
	}
}

IEmcDecoder *CreateDVBTSubtitleDecoder(TCHAR* tcDllPath)
{
	return new CSubtitleDecoderDVBT();
}