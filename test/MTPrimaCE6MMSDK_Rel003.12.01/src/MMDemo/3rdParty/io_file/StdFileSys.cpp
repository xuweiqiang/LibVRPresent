// StdFileSys.cpp: implementation of the StdFileSys class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "../inc/StdFileSys.h"
//#include "BiosLib/BIOS_Generic/CP_TypeDef.h"
//#include "Util.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StdFileSys::StdFileSys(bool bWriteThrough)
:m_bWrtThrough(bWriteThrough)
{
   m_hFile = NULL;
   m_iWriteBuf = 0;
}

StdFileSys::~StdFileSys()
{
   if(m_hFile != NULL)
	 Close();
}

eFileErr StdFileSys::Open(const TCHAR* file_name, bool bCreate)
{
   if(m_hFile == NULL)
   {
	  if(!bCreate)
	  {
	     m_hFile = fopen((char*)file_name, "r+b");
         if(m_hFile == NULL)
			return FERR_NOFOUND;
	  }
	  else
	  {
         m_hFile = fopen((char*)file_name, "w+r+b");
         if(m_hFile == NULL)
		 {
			return FERR_UNKNOWN;
		 }
	  }
   }
   return FERR_NOERROR;
}

eFileErr StdFileSys::Close()
{
   if(m_hFile == NULL)
   {
      return FERR_NOTOPEN;
   }
   
   if(m_hFile != NULL)
   {
      FlushWrtBuf();

      int ret = fclose(m_hFile);
      ASSERT(ret != 0);
	  m_hFile = NULL;
      if(ret == 0)
      {
         return FERR_UNKNOWN;
      }
   }
   return FERR_NOERROR;
}

eFileErr StdFileSys::Read(unsigned char* pBuf, int& cnt)
{
   if(m_hFile == NULL)
	  return FERR_NOTOPEN;
   
   FlushWrtBuf();

   int in_cnt = cnt;
   cnt = fread(pBuf, 1, cnt, m_hFile);
   if(cnt != in_cnt)
   {
	  if(feof(m_hFile))
         return FERR_EOF;
      
//      CP_TRACE(MSG_ERROR, L"File read error");
	  return FERR_UNKNOWN;
   }
   
//   CP_TRACE(MSG_DEV_INFO, L"StdFileSys: %d read", cnt);
   return FERR_NOERROR;
}

eFileErr StdFileSys::Write(unsigned char* pBuf, int& cnt)
{  
   if(m_hFile == NULL)
      return FERR_NOTOPEN;
   
   if(m_bWrtThrough)   
   {
      int in_cnt = cnt;
      cnt = fwrite(pBuf, 1, cnt, m_hFile);
      if(cnt != in_cnt)
      {
//	     CP_TRACE(MSG_ERROR, L"File write error");
	     //if(err == ERROR_DISK_FULL)
	     //   return FERR_DISK_FULL;
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

//   CP_TRACE(MSG_DEV_INFO, L"StdFileSys: %d written", cnt);
   return FERR_NOERROR;
}
//warning: no error check here
void StdFileSys::FlushWrtBuf()
{
   if(!m_bWrtThrough)
   {
       if(m_iWriteBuf >0)
       { 
           int nWritten = fwrite(m_WriteBuf, 1, m_iWriteBuf, m_hFile);
           ASSERT(nWritten == m_iWriteBuf);
//           CP_TRACE(MSG_DEV_INFO, L"*******write buffer flush: %d***********", m_iWriteBuf);
           m_iWriteBuf = 0;
       }
   }
}

//there's no standard way to do truncate, so, do nothing, :(
eFileErr StdFileSys::Truncate(int siz)
{
   if(m_hFile == NULL)
      return FERR_NOTOPEN;

   return FERR_NOERROR;
}

static DWORD SeekPosTab[] = 
{
    SEEK_SET,   
    SEEK_CUR,
    SEEK_END
};

eFileErr StdFileSys::Seek(long& distance, eFileSeekPos pos)
{
    FlushWrtBuf();
    
    if(pos == FILE_SEEK_SET && distance == 0)
    {
       //report current position
       distance = ftell(m_hFile);
       if(distance == -1L)
          return FERR_UNKNOWN;

       return FERR_NOERROR;
    }
    
    int err = fseek(m_hFile, distance, SeekPosTab[pos]);  
    if(err != 0)
	{
//      CP_TRACE(MSG_ERROR, L"File seek error");
	  return FERR_UNKNOWN;
	}
	return FERR_NOERROR;  
} 

bool StdFileSys::InExist(const TCHAR* file_name)
{
    FILE* hFile = fopen((char*)file_name, "r+b");
    if(hFile == NULL)
	   return false;
    else
	{
       fclose(hFile);
	   return true;
	}
}

bool StdFileSys::IsOpened()
{
    if(m_hFile != NULL)
	   return true;

	return false;
}

//emulate a get file size operation with standard function
int StdFileSys::GetFileSiz()
{
	if(m_hFile != NULL) 
	{
       FlushWrtBuf();
       long siz = 0;
       long distance = 0;
       //save file position
       distance = ftell(m_hFile);
       if(distance != -1L)
       {
          if(fseek(m_hFile, 0, SEEK_END) == 0)
          {
             siz = ftell(m_hFile);
          }
          //restore file position
          if(fseek(m_hFile, distance, SEEK_SET) == 0)
          {
             return siz;
          }
       }
    }
	return -1;
}

