#ifndef CC_DECODE_H_
#define CC_DECODE_H_

#include "isdbt_cc.h"
#include "cc_1seg.h"
#include "FontDraw.h"

struct FrameBuffer {
	BYTE *m_OutBuffer[4];
	DWORD m_dwWidth[3];
	DWORD m_dwHeight[3];
	DWORD m_dwStride[3];
};


class IsdbtCCDecode 
{
public:
	IsdbtCCDecode();
	~IsdbtCCDecode();

	STDMETHODIMP Open(ISDBTCC_OpenOptions* option);
	STDMETHODIMP Close();
	STDMETHODIMP Decode(PBYTE pData, DWORD *pLen, ISDBTCC_DecodeOptions* option);
	STDMETHODIMP GetOutput(ISDBTCC_Output* pOutput);
	STDMETHODIMP Reset();
	STDMETHODIMP Set(DWORD dwPropID, LPVOID pPropData, DWORD cbPropData);
	STDMETHODIMP Get(DWORD dwPropID, LPVOID pPropData, DWORD cbPropData);	


private:
	int Draw(unsigned int *data, unsigned int length, int index);
	//input buffer
	unsigned char m_aData[2048];
	unsigned int m_uiDataLen;

	//decoded buffer
	unsigned int m_aDecodedData[2048];
	unsigned int m_uiDecodedLen;

	//output buffer
	FrameBuffer *m_FrameBuffer;
	DWORD m_dwBufferNum;
	DWORD m_dwBufferIndex;

	ISDBTCC_OpenOptions m_tOpenOption;
	tCaptionManagement m_tCaptionMgn;
	FontDraw* m_pFontDraw;
	DWORD m_uiLastTimeStamp;
};

#endif //CC_DECODE_H_
