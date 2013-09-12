// IFileOps.h: interface for the IFileOps class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IFILEOPS_H__957A1104_792C_4CB1_83A2_303D732FF6CE__INCLUDED_)
#define AFX_IFILEOPS_H__957A1104_792C_4CB1_83A2_303D732FF6CE__INCLUDED_
#include <windows.h>

enum eFileErr
{
   FERR_NOERROR,
   FERR_NOTOPEN,       //file not being opened
   FERR_ACCESS_DENY,   //access deny
   FERR_NOFOUND,       //file no found
   FERR_CORRUPT,       //file corrupted
   FERR_INVALID_FORMAT,//file format error(invalid file identity)
   FERR_DISK_FULL,     //disk full(cannot write)
   FERR_UNKNOWN,        //unknown error
   FERR_EOF
};

enum eFileSeekPos     //base position for file seek
{
   FILE_SEEK_BEGIN,   //file header
   FILE_SEEK_SET,     //current position
   FILE_SEEK_END,     //file tail 
};

class IFileOps  
{
public:
	IFileOps();
	virtual ~IFileOps();

	virtual eFileErr Open(const TCHAR* file_name, bool bCreate) = 0;
    virtual eFileErr Close() = 0;
	virtual eFileErr Read(unsigned char* pBuf, int& cnt) = 0;
    virtual eFileErr Write(unsigned char* pBuf, int& cnt) = 0;
	virtual eFileErr Truncate(int siz) = 0;
    virtual eFileErr Seek(long& distance, eFileSeekPos pos) = 0;
    virtual bool     InExist(const TCHAR* file_name) = 0;
	virtual bool     IsOpened() = 0;
	virtual int      GetFileSiz() = 0;
};

#endif // !defined(AFX_IFILEOPS_H__957A1104_792C_4CB1_83A2_303D732FF6CE__INCLUDED_)
