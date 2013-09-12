// CEFileSys.h: interface for the windows CE File System class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CEFILESYS_H__B3C79EBF_2380_46F7_8B6A_0E18115CE049__INCLUDED_)
#define AFX_CEFILESYS_H__B3C79EBF_2380_46F7_8B6A_0E18115CE049__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IFileOps.h"
#include "windows.h"
//#define GEN_INVALID_FILEHANDLE INVALID_HANDLE_VALUE

class CEFileSys : public IFileOps  
{
public:
	CEFileSys(bool bWriteThrough = false);
	virtual ~CEFileSys();

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
	HANDLE m_hFile;

    //static CRITICAL_SECTION m_WrtLock; //write lock
    //static bool m_cs_inited; //flag to indicate cs initialized or not
    bool m_bWrtThrough;
    
    enum { WRITEBUFSIZ = 1<<16 }; //64k
    
    char m_WriteBuf[WRITEBUFSIZ];
    int  m_iWriteBuf; //buffer index
};

#endif // !defined(AFX_CEFILESYS_H__B3C79EBF_2380_46F7_8B6A_0E18115CE049__INCLUDED_)
