// StdFileSys.h: interface for the windows CE File System class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_StdFileSys_H__B3C79EBF_2380_46F7_8B6A_0E18115CE049__INCLUDED_)
#define AFX_StdFileSys_H__B3C79EBF_2380_46F7_8B6A_0E18115CE049__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IFileOps.h"
#include <stdio.h>

//#define GEN_INVALID_FILEHANDLE NULL

class StdFileSys : public IFileOps  
{
public:
	StdFileSys(bool bWriteThrough = false);
	virtual ~StdFileSys();

	eFileErr Open(const TCHAR* file_name, bool bCreate);
    eFileErr Close();
	eFileErr Read(unsigned char* pBuf, int& cnt);
    eFileErr Write(unsigned char* pBuf, int& cnt);
	eFileErr Truncate(int siz);
    eFileErr Seek(long& distance, eFileSeekPos pos);
    bool     InExist(const TCHAR* file_name);
    bool     IsOpened();
    int      GetFileSiz();
private:
    void     FlushWrtBuf();
    FILE* m_hFile;

    //static CRITICAL_SECTION m_WrtLock; //write lock
    //static bool m_cs_inited; //flag to indicate cs initialized or not
    bool m_bWrtThrough;
    
    enum { WRITEBUFSIZ = 1<<16 }; //64k
    
    char m_WriteBuf[WRITEBUFSIZ];
    int  m_iWriteBuf; //buffer index
};

#endif // !defined(AFX_StdFileSys_H__B3C79EBF_2380_46F7_8B6A_0E18115CE049__INCLUDED_)
