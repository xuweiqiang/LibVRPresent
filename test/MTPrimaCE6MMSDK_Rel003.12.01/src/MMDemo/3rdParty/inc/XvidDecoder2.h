// XvidDecoder2.h: interface for the CXvidDecoder2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XVIDDECODER_H__25ECACCC_130B_406B_86DE_JGFHEHGDY4__INCLUDED_)
#define AFX_XVIDDECODER_H__25ECACCC_130B_406B_86DE_JGFHEHGDY4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MediaProcessor.h"
#include "DataReader.h"

#include "../../DSP/inc/MMGeneralDsp.h"


class CXvidDecoder2 : public CMediaProcessor
{
public:
	CXvidDecoder2(CMMGeneralDsp * pMMGeneralDsp);
	virtual ~CXvidDecoder2();

	virtual bool Init(CDataReader* pReader); 
    virtual bool ProcessFrame(BYTE** pBuf, int& siz);
    virtual eCodecResult ProcessFrame(MediaStream& InStream, MediaStream& OutStream, BYTE* pOptions, size_t opt_len);

private:
	CMMGeneralDsp * pDsp;
    CDataReader* m_pReader;
};

#endif // !defined(AFX_XVIDDECODER_H__25ECACCC_130B_406B_86DE_JGFHEHGDY4__INCLUDED_)
