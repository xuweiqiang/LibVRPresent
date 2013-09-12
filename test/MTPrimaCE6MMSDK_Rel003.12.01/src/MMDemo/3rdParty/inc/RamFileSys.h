// StdFileSys.h: interface for the windows CE File System class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RamFileSys_H__B3C79EBF_2380_46F7_8B6A_0E18115CE049__INCLUDED_)
#define AFX_RamFileSys_H__B3C79EBF_2380_46F7_8B6A_0E18115CE049__INCLUDED_



#include "IFileOps.h"
//#include "BiosLib/BIOS_Generic/CP_TypeDef.h"

#define GEN_INVALID_FILEHANDLE NULL

class RamFileSys : public IFileOps  
{
public:
	RamFileSys(unsigned char* pStart, int siz);
	virtual ~RamFileSys();

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
    bool  m_bOpened;
    unsigned char* m_pStart;//RAM chunk start address
    int   m_siz;
    unsigned char* m_pPos; //pointer to next to write
    static unsigned char* m_pEOF; //pointer to end of file
};

#endif // !defined(AFX_RamFileSys_H__B3C79EBF_2380_46F7_8B6A_0E18115CE049__INCLUDED_)
