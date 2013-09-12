/***************************************************************************
*                                                                         *
*                   SiRF Technology, Inc. Platform Software               *
*                                                                         *
*    Copyright (c) 2008 by SiRF Technology, Inc.  All rights reserved.    *
*                                                                         *
*    This Software is protected by United States copyright laws and       *
*    international treaties.  You may not reverse engineer, decompile     *
*    or disassemble this Software.                                        *
*                                                                         *
*    WARNING:                                                             *
*    This Software contains SiRF Technology, Inc.'s confidential and      *
*    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
*    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
*    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
*    Software without SiRF Technology, Inc.'s  express written            *
*    permission.   Use of any portion of the contents of this Software    *
*    is subject to and restricted by your written agreement with          *
*    SiRF Technology, Inc.                                                *
*                                                                         *
 ***************************************************************************/


#ifndef __XPLATFORM_H__
#define __XPLATFORM_H__




#ifdef WIN32_WCE
#include <xplatform_wce.h>
#endif

#ifdef GNU_C
#include <xplatform_gcc.h>
#endif

#ifdef LINUX
#include <xplatform_linux.h>
#endif

/*****************************************************************************************
  Cross platform data type defination
******************************************************************************************/
#ifndef SX_LONG
typedef long				SX_LONG;
#endif


#ifndef SX_INT16
typedef short	SX_INT16;
#endif

#ifndef SX_UINT16
typedef unsigned short	SX_UINT16;
#endif

#ifndef SX_INT32
typedef int	SX_INT32;
#endif

#ifndef SX_UINT32
typedef unsigned int	SX_UINT32;
#endif

#ifndef _MSC_VER
#ifndef SX_UINT64
typedef unsigned long long		SX_UINT64;
#endif

#ifndef SX_INT64
typedef long long	SX_INT64;
#endif

#else

#ifndef SX_UINT64
typedef unsigned __int64	SX_UINT64;
#endif

#ifndef SX_INT64
typedef __int64	SX_INT64;
#endif

#endif //_MSC_VER



#ifndef VOID
typedef void					VOID;
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef SX_BOOL
typedef int SX_BOOL;
#endif

#ifndef SX_CHAR
#ifdef UNICODE
#if _WIN32_WCE < 600
#ifndef wchar_t
typedef unsigned short wchar_t;
#endif
#endif
typedef wchar_t SX_CHAR;
#define SX_TEXT(quote)  L##quote 
#else
typedef char SX_CHAR;
#define SX_TEXT(quote)  quote 
#endif
#endif

#ifndef SX_BYTE
typedef unsigned char SX_BYTE;
#endif


/*****************************************************************************************
  Cross platform debug support routines
******************************************************************************************/
typedef enum tagSXDbgLevel
{
	MSG_FATAL = 0,    //fatal error
	MSG_ERROR,
	MSG_RETAIL_INFO,  //information should appear in retail version
	MSG_DEV_INFO,     //information useful for developer
	MSG_TEMP,         //use it while debug a module
	MSG_NONE,         //don't output message, change MSG_TEMP to it after debug 
}eSXDbgLevel; 

#ifdef __cplusplus
extern "C" {
#endif
	
void SxSetDebugLevel(eSXDbgLevel level);
void SxResetDebugLevel(eSXDbgLevel level);
void SxDisableDebugInfo();
void SX_TRACE(eSXDbgLevel level, SX_CHAR* msg, ...);

#ifdef __cplusplus
}
#endif
/*****************************************************************************************
  Cross platform timing support routines
******************************************************************************************/
typedef struct sSxTimeHolder
{
	 int sig;
   float sum;
   float last;
   int   cnt;
   	
}SxTimeHolder;

#ifdef __cplusplus
extern "C" {
#endif
//get one-shot interval info
void SxStartTiming(sSxTimeHolder* timeholder);
//return interval in ms
float SxEndTiming(sSxTimeHolder* timeholder);

//get average interval info for 
void SxStartTimingAvg(sSxTimeHolder* timeholder);
float SxEndTimingAvg(sSxTimeHolder* timeholder);

void SxClearTimeholder(sSxTimeHolder* timeholder);

#ifdef __cplusplus
}
#endif
/*****************************************************************************************
  Cross platform memory allocation support routines
******************************************************************************************/
//TODO: pool support?
#ifdef __cplusplus
extern "C" {
#endif
SX_BYTE* Sx_AllocMem(SX_UINT32 siz);
void Sx_FreeMem(SX_BYTE* pMem);
SX_BYTE* Sx_AllocPhyMem(SX_BYTE** ppPhy, SX_UINT32 siz);
SX_BYTE* Sx_AllocPhyMemCached(SX_BYTE** ppPhy, SX_UINT32 siz);
void Sx_FlushPhyMem(SX_BYTE* pVirt, SX_UINT32 siz);
void Sx_FreePhyMem(SX_BYTE* pVirt);
void Sx_FreePhyMemCached(SX_BYTE* pVirt);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
//in some case we will have to redirect memory allocation to a 3rd-party provided memory pool
class SxMemHandler 	
{
public:
    void* operator new(size_t siz)
	{
	   return Sx_AllocMem(siz);
	}
	
	void operator delete(void* p, size_t)
	{
	   Sx_FreeMem((SX_BYTE*)p);	
	}
	
	void operator delete(void* p)
	{
       Sx_FreeMem((SX_BYTE*)p);	
	}

	void* operator new[](size_t siz)
	{
	   return Sx_AllocMem(siz);
	}
    
    void  operator delete[](void* p)
	{
       Sx_FreeMem((SX_BYTE*)p);
	}
};

#endif //__cplusplus
/*****************************************************************************************
  Cross platform dynamic linked lib support routines
******************************************************************************************/
typedef union SX_DLL
{
   unsigned long u1;
   void*         u2;
}SX_DLL; 
SX_DLL SxLoadLib(SX_CHAR* LibNm);
void SxUnloadLib(SX_DLL hLib);
void* SxGetFunctor(SX_DLL hLib, SX_CHAR* func_nm);

#ifdef _MSC_VER
#define SXDLLEXPORT __declspec(dllexport)
#define SXDLLIMPORT __declspec(dllimport)

#endif

#ifndef SXDLL_IMPORTER 
#define SXDLL SXDLLEXPORT
#else
#define SXDLL SXDLLIMPORT 
#endif

#endif //__PLATFORM_H__