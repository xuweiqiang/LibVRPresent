// CEFileSys.cpp: implementation of the CEFileSys class.
//
//////////////////////////////////////////////////////////////////////
#include "../inc/CEFileSys.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEFileSys::CEFileSys(bool bWriteThrough)
:m_bWrtThrough(bWriteThrough)
{
   m_hFile = INVALID_HANDLE_VALUE;

//   if(!m_bWrtThrough && !m_cs_inited)
//   {
//      InitializeCriticalSection(&m_WrtLock);
//      m_cs_inited = true;
//   }
     m_iWriteBuf = 0;
}

CEFileSys::~CEFileSys()
{
   if(m_hFile != INVALID_HANDLE_VALUE)
	 Close();

//   if(m_cs_inited)
//   {
//      DeleteCriticalSection(&m_WrtLock);
//   }

}

eFileErr CEFileSys::Open(const TCHAR* file_name, bool bCreate)
{
   if(m_hFile == INVALID_HANDLE_VALUE)
   {
	  if(!bCreate)
	  {
	     m_hFile = CreateFile(file_name, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
         if(m_hFile == INVALID_HANDLE_VALUE && GetLastError() == ERROR_FILE_NOT_FOUND)
			return FERR_NOFOUND;
		 //ERROR_INVALID_NAME
	  }
	  else
	  {
         m_hFile = CreateFile(file_name, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
         if(m_hFile == INVALID_HANDLE_VALUE)
		 {
			DWORD err = GetLastError();
			//TRACE(L"File Open Error: %d", err);
			switch(err)
			{
			  case ERROR_SHARING_VIOLATION:
			  case ERROR_ACCESS_DENIED:
				 return FERR_ACCESS_DENY;
				 break;
			  default:
				 break;
			}  
			return FERR_UNKNOWN;
		 }
	  }
   }
   return FERR_NOERROR;
}

eFileErr CEFileSys::Close()
{
   if(m_hFile == INVALID_HANDLE_VALUE)
   {
      return FERR_NOTOPEN;
   }
   
   if(m_hFile != INVALID_HANDLE_VALUE)
   {
      FlushWrtBuf();

      BOOL ret = CloseHandle(m_hFile);

	  m_hFile = INVALID_HANDLE_VALUE;
   }
   return FERR_NOERROR;
}

eFileErr CEFileSys::Read(unsigned char* pBuf, int& cnt)
{
   if(m_hFile == INVALID_HANDLE_VALUE)
	  return FERR_NOTOPEN;
   
   FlushWrtBuf();

   BOOL ret = ReadFile(m_hFile, pBuf, cnt, (PULONG)&cnt, NULL);
   if(ret == 0)
   {
	  DWORD err = GetLastError();
//	  CP_TRACE(MSG_ERROR, L"File read error: %d", err);
	  return FERR_UNKNOWN;
   }
//   TRACE(L"CEFileSys: %d read", cnt);
   return FERR_NOERROR;
}

eFileErr CEFileSys::Write(unsigned char* pBuf, int& cnt)
{  
   if(m_hFile == INVALID_HANDLE_VALUE)
      return FERR_NOTOPEN;
   
   if(m_bWrtThrough)   
   {
      BOOL ret = WriteFile(m_hFile, pBuf, cnt, (PULONG)&cnt, NULL);
      if(ret == 0)
      {
	     DWORD err = GetLastError();
//	     CP_TRACE(MSG_ERROR, L"File write error: %d", err);
	     if(err == ERROR_DISK_FULL)
	        return FERR_DISK_FULL;
	     
         return FERR_UNKNOWN;
      }
   }
   else // write to intermediate cache first
   {
      int size = cnt;
      while(cnt>0)
      {
         int nBuf =  WRITEBUFSIZ- m_iWriteBuf;
         int nCopy = cnt>nBuf? nBuf: cnt;
         memcpy(m_WriteBuf + m_iWriteBuf, pBuf, nCopy);
         cnt -= nCopy;
         m_iWriteBuf += nCopy;
		 pBuf += nCopy;
         if(m_iWriteBuf == WRITEBUFSIZ)
         {
            FlushWrtBuf();
         }
      }
      cnt = size;   
  }

//   TRACE(L"CEFileSys: %d written", cnt);
   return FERR_NOERROR;
}
//warning: no error check here
void CEFileSys::FlushWrtBuf()
{
   if(!m_bWrtThrough)
   {
       if(m_iWriteBuf >0)
       { 
           ULONG nWritten;
           BOOL ret = WriteFile(m_hFile, m_WriteBuf, m_iWriteBuf, &nWritten, NULL);
   
//           CP_TRACE(MSG_DEV_INFO, L"*******write buffer flush: %d***********", m_iWriteBuf);
           m_iWriteBuf = 0;
       }
   }
}

eFileErr CEFileSys::Truncate(int siz)
{
   if(m_hFile == INVALID_HANDLE_VALUE)
      return FERR_NOTOPEN;
   long tmp = siz;
   long& len = tmp;
   if(Seek(len, FILE_SEEK_BEGIN) == FERR_NOERROR)
   {
	  if(SetEndOfFile(m_hFile) == 0)
	  {
		 return FERR_NOERROR;
	  }
//      CP_TRACE(MSG_ERROR, L"File truncate error: %d", GetLastError());
   }
   return FERR_NOERROR;
}

static DWORD SeekPosTab[] = 
{
	FILE_BEGIN,
	FILE_CURRENT, 
	FILE_END
};

eFileErr CEFileSys::Seek(long& distance, eFileSeekPos pos)
{
    FlushWrtBuf();
    
    distance = SetFilePointer(m_hFile, distance, NULL, SeekPosTab[pos]);  
    if(distance == 0xFFFFFFFF)
	{
//      CP_TRACE(MSG_ERROR, L"File seek error: %lu", GetLastError());
	  return FERR_UNKNOWN;
	}
	return FERR_NOERROR;  
} 

bool CEFileSys::InExist(const TCHAR* file_name)
{
    HANDLE hFile = CreateFile(file_name, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile == INVALID_HANDLE_VALUE && GetLastError() == ERROR_FILE_NOT_FOUND)
	   return false;
    else
	{
       CloseHandle(hFile);
	   return true;
	}
}

bool CEFileSys::IsOpened()
{
    if(m_hFile != INVALID_HANDLE_VALUE)
	   return true;

	return false;
}

int CEFileSys::GetFileSiz()
{
	if(m_hFile != INVALID_HANDLE_VALUE) 
	{
       FlushWrtBuf();
       return GetFileSize(m_hFile, NULL);
	}
	return -1;
}