// RamFileSys.cpp: implementation of the RamFileSys class.
//
//////////////////////////////////////////////////////////////////////

#include "../inc/RamFileSys.h"
//#include "BiosLib/BIOS_Generic/CP_TypeDef.h"
//#include "Util.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
unsigned char* RamFileSys::m_pEOF = NULL;

RamFileSys::RamFileSys(unsigned char* pStart, int siz)
:m_pStart(pStart), m_siz(siz)
{
   m_pPos = pStart;
   if(m_pEOF == NULL) m_pEOF = pStart;
   m_bOpened = false;
}

RamFileSys::~RamFileSys()
{
   Close();
}

eFileErr RamFileSys::Open(const TCHAR* file_name, bool bCreate)
{
   if(m_bOpened) 
      Close();
   
   m_pPos = m_pStart; //reset to header
   
   if(bCreate) //emulate a create and truncate operation
      m_pEOF = m_pStart;
      
   m_bOpened = true;
   
   return FERR_NOERROR;
}

eFileErr RamFileSys::Close()
{
   if(m_bOpened)
   {
      m_pPos = m_pStart;
      m_bOpened = false;
      return FERR_NOERROR;
   }
   return FERR_NOTOPEN;
}

eFileErr RamFileSys::Read(unsigned char* pBuf, int& cnt)
{
   if(!m_bOpened)
      return FERR_NOTOPEN;
      
   eFileErr err = FERR_NOERROR;
   int copy_siz = cnt;
   if(m_pPos+cnt > m_pEOF) //beyond end of file
   {
      err = FERR_EOF;
      copy_siz = m_pEOF - m_pPos; //cut
   }
   if(copy_siz > 0) 
      memcpy(pBuf, m_pPos, copy_siz);
   
   m_pPos += copy_siz;
   cnt = copy_siz;

   return err;
}

eFileErr RamFileSys::Write(unsigned char* pBuf, int& cnt)
{  
   if(!m_bOpened)
      return FERR_NOTOPEN;
       
   eFileErr err = FERR_NOERROR;
   int copy_siz = cnt;
   if(m_pPos+cnt >= m_pStart+m_siz) //beyond end of file
   {
      err = FERR_DISK_FULL;
      copy_siz = m_pStart + m_siz - m_pPos; //cut
   }
   memcpy(m_pPos, pBuf, copy_siz);
   m_pPos += copy_siz;
   cnt = copy_siz;
   
   m_pEOF += cnt;
#ifdef DEBUGMSG
//   CP_TRACE(MSG_DEV_INFO, L"RamFileSys: %d written", cnt);
#endif
   return err;
}

//there's no standard way to do truncate, so, do nothing, :(
eFileErr RamFileSys::Truncate(int siz)
{
   if(!m_bOpened)
      return FERR_NOTOPEN;

   m_pEOF = m_pStart+siz;
   return FERR_NOERROR;
}

eFileErr RamFileSys::Seek(long& distance, eFileSeekPos pos)
{
   if(!m_bOpened)
      return FERR_NOTOPEN;
   
    if(pos == FILE_SEEK_SET && distance == 0)
    {
       //report current position
       distance = m_pPos-m_pStart;
       return FERR_NOERROR;
    }
    
    eFileErr err = FERR_NOERROR;
    switch(pos)
    {
       case FILE_SEEK_BEGIN:
          if(m_pStart+distance < m_pEOF)
          { 
             m_pPos = m_pStart+distance;
             distance = m_pPos-m_pStart;
          }
          else
             err = FERR_UNKNOWN;
             
          break;
          
       case FILE_SEEK_SET:
          if(m_pPos + distance < m_pEOF && m_pPos + distance >= m_pStart) 
          {  
             m_pPos += distance;
             distance = m_pPos-m_pStart;
          }
          else
             err = FERR_UNKNOWN;
             
          break;
          
       case FILE_SEEK_END:
          if(distance <= 0 && m_pPos + distance >= m_pStart)
          {
             m_pPos = m_pEOF + distance;
             distance = m_pPos-m_pStart;
          }
          else
             err = FERR_UNKNOWN;;
             
       default:
             return FERR_UNKNOWN;	
    }
    return err;  
} 

bool RamFileSys::InExist(const TCHAR* file_name)
{
    return true;
}

bool RamFileSys::IsOpened()
{
    return m_bOpened;
}

int RamFileSys::GetFileSiz()
{
    if(!m_bOpened)  
	return -1;
    
    return m_pEOF-m_pStart;
}

