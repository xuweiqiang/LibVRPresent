// Utility.cpp: implementation of the Utility class.
//
//////////////////////////////////////////////////////////////////////
//#include "../inc/Utility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define LONG_PACK(a,b,c,d) ((long) (((long)(a))<<24) | (((long)(b))<<16) | \
(((long)(c))<<8)  |((long)(d)))

#define SWAP(a) ( (((a)&0x000000ff)<<24) | (((a)&0x0000ff00)<<8) | \
(((a)&0x00ff0000)>>8)  | (((a)&0xff000000)>>24) )
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Utility::Utility()
{

}

Utility::~Utility()
{

}

bool Utility::IsBigEndian()
{
	long test = LONG_PACK('M','P','4','U');
	char* pTest = (char*)&test;

	if(*pTest == 'M')
	   return true;
	else
	   return false;
}

DWORD Utility::ToBigEndian(DWORD data)
{
    return SWAP(data);
}

void Utility::SwapWordEndian(BYTE* pBuf, size_t len)
{
	//swap endian 
    for(size_t i = 0; i < len; i += 2)
	{
		BYTE tmp = pBuf[i];
		pBuf[i] = pBuf[i+1];
		pBuf[i+1] = tmp;
	}
} 


