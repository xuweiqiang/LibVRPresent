#ifndef _SUBTITLEDECODERDVBT_H_
#define _SUBTITLEDECODERDVBT_H_
#include "Player.h"
#include "BaseSubtitleDecoder.h"
#include "CISubtitleDecoder.h"
#define SAMPLE_BUFFER_SIZE 4

class CSubtitleSample;

class CSampleQueue
{
public:
	CSampleQueue() { m_Count = 0; }
	~CSampleQueue() { }
	bool Put(CSubtitleSample* pSample)
	{
		if(m_Count >= SAMPLE_BUFFER_SIZE)
			return false;
		else
		{
			m_Queue[m_Count] = pSample;
			m_Count++;
		}
		return true;
	}
	CSubtitleSample* Get()
	{
		if(m_Count == 0)
		{
			return NULL;
		}
		CSubtitleSample* ret = m_Queue[0];
		m_Count --;
		for(int i = 0; i < m_Count; i++)
		{
			m_Queue[i] = m_Queue[i+1];
		}
		return ret;
	}
	int Size() { return m_Count; }
private:
	CSubtitleSample* m_Queue[SAMPLE_BUFFER_SIZE];
	int m_Count;
};


class CSubtitleDecoderDVBT :
	public CBaseSubtitleDecoder
{
public:
	CSubtitleDecoderDVBT();
	virtual ~CSubtitleDecoderDVBT();

	STDMETHODIMP Open(LPVOID pParam, DWORD cbParam);

	void InitSamples();
	STDMETHODIMP Close();
	STDMETHODIMP GetSampleWithTimeStamp(IEmcMediaSample **ppSample, TTimeStamp* pTSNode);

	void PutFreeSampel(CSubtitleSample* pSample);
private:
	void DrawSampleIner(CI_SUB_Draw_Info *pDrawInfo);
	static void DrawSample(CI_SUB_Draw_Info *pDrawInfo, void *pContext);
	CSubtitleSample* GetFreeSample();

	void CopyToDestBuffer(char* pDest, char* pSrc, CI_SUB_Rect &DispRect);
	void CleanResource();

	void DeinitSamples();
	HMODULE m_hSubtitleDecoderEngine;
	ISUBDecoder* m_hDecoder;
	unsigned int m_LockedBufferLength;
	long m_lSubtitleInfoChanged;
	long m_LastRemain;
	BOOL m_fExit;
	RECT m_Region;

	CSampleQueue m_FreeSample;
	CSampleQueue m_Sample;
	CSubtitleSample* m_LastSample;

	char* m_pBuffer[SAMPLE_BUFFER_SIZE];
};

class CSubtitleSample : public CEmcMediaSample
{
public:
	CSubtitleSample(CSubtitleDecoderDVBT* pOwner):CEmcMediaSample(3)
	{
		m_pOwner = pOwner;
	}
	~CSubtitleSample() {}
	STDMETHODIMP_(ULONG) NonDelegatingRelease(void)
	{
		LONG ref = InterlockedDecrement(&m_lRef);
		if(ref<=0)
		{
			m_PTS = 0;
			m_pOwner->PutFreeSampel(this);
		}
		return ref;
	}
	void SavePTS(DWORD PTS) { m_PTS = PTS; }
	DWORD GetPTS() { return m_PTS; }
private:
	CSubtitleDecoderDVBT* m_pOwner;
	DWORD m_PTS;
};
#endif