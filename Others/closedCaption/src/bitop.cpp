#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitop.h"

static const unsigned long msk[33] =
{
  0x00000000, 0x00000001, 0x00000003, 0x00000007,
  0x0000000f, 0x0000001f, 0x0000003f, 0x0000007f,
  0x000000ff, 0x000001ff, 0x000003ff, 0x000007ff,
  0x00000fff, 0x00001fff, 0x00003fff, 0x00007fff,
  0x0000ffff, 0x0001ffff, 0x0003ffff, 0x0007ffff,
  0x000fffff, 0x001fffff, 0x003fffff, 0x007fffff,
  0x00ffffff, 0x01ffffff, 0x03ffffff, 0x07ffffff,
  0x0fffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff,
  0xffffffff
};

unsigned char* GetBstrBegin(NewBstr *bstr)
{
	return bstr->m_SrcBufptr;
}

unsigned char* GetBstrEnd(NewBstr *bstr)
{
	return bstr->m_SrcBufptr+bstr->m_SrcLength;
}

int GetBstrSize(NewBstr *bstr)
{
	return (bstr->m_SrcLength);
}

int GetBstrOffset(NewBstr *bstr)
{
	return (bstr->m_rdptr-bstr->m_SrcBufptr);
}

int SeekBstr(NewBstr *bstr, int offset, int where)
{
	/*
	ts_buffer * tbuf = (ts_buffer *)fp;

	if(offset<0 || offset > dmx_buffer_size(fp))
		return -1;

	switch(where)
		{
		case DMX_FILE_SEEK_SET:
			tbuf->cur = tbuf->begin + offset;		
			return 0;
		case DMX_FILE_SEEK_END:
			tbuf->cur = tbuf->end - offset;					
			return 0;
		}

	return -1;
	*/
	if(offset<0 || offset > GetBstrSize(bstr))
		return -1;

	switch(where)
	{
		case 0x1:
			bstr->m_rdptr = bstr->m_SrcBufptr + offset;		
			return 0;
		case 0x2:
			bstr->m_rdptr = bstr->m_SrcBufptr +  GetBstrSize(bstr) - offset;					
			return 0;
	}

	return -1;
}


///////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////
void InitNewBstr(NewBstr *bstr, unsigned char *streamBuffer, long length)
{ 
	memset(bstr, 0, sizeof(*bstr));
	bstr->m_rdptr = streamBuffer;
	bstr->m_SrcBufptr = streamBuffer;
	bstr->m_SrcLength = length;
	bstr->m_endptr = streamBuffer+length;
}

///////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////
void flushbits (NewBstr *bstr, int n)
{	
	bstr->incnt -= n;
}

///////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////
unsigned long Get_Bits(NewBstr *bstr, const int numBits)
{
	while(bstr->incnt<numBits)
	{		
		bstr->incnt += 8; 
		bstr->code = (bstr->code<<8)|*bstr->m_rdptr++;
	}
	bstr->incnt -= numBits;	
	return (bstr->code>>(bstr->incnt)) & msk[numBits];
}


int GetBytes(NewBstr *bstr, unsigned char* dst, const int bytes)
{
	if(bstr->m_rdptr+bytes > bstr->m_endptr)
		return 0;
	
	memcpy(dst, bstr->m_rdptr, bytes);
	bstr->m_rdptr += bytes;

	return bytes;
}

///////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////
unsigned long PeekBits (NewBstr *bstr, const int numBits) 
{	
	while(bstr->incnt<numBits)
	{		
		bstr->incnt += 8;
		bstr->code = (bstr->code<<8)|*bstr->m_rdptr++;
	}
	return (bstr->code>>(bstr->incnt-numBits)) & msk[numBits];
}


///////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////
int bytealigned(NewBstr *bstr, int nbit) 
{
	return (((bstr->incnt - nbit) % 8) == 0);
}



///////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////
unsigned long PeekBitsFromByteAlign(NewBstr *bstr, int nBitsToPeek) 
{
	int code;
	int skipcnt = 0;

	if (bytealigned(bstr, skipcnt))
	{
		// stuffing bits		
		if (PeekBits(bstr, 8) == 127) 
		{
			skipcnt = 8;
		}		
	}
	else
	{
		skipcnt = (bstr->incnt%8);
	}

	code = PeekBits(bstr, nBitsToPeek + skipcnt);
	return (code & msk[nBitsToPeek]);
}


///////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////
unsigned long PeekBitsTillByteAlign(NewBstr *bstr)
{	
	int a = NumOfBitsInByte(bstr);
	if(a==0)
		return PeekBits(bstr, 8);
	
	return PeekBits(bstr, a);
}

///////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////
int NumOfBitsInByte(NewBstr *bstr)
{
	return (bstr->incnt%8);
}


//////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////
void MakeByteAligned(NewBstr *bstr)
{
	int skipcnt = 0;

	if (!bytealigned(bstr, skipcnt))	
	{
		// bytealign
		while (! bytealigned(bstr, skipcnt)) 
		{
			skipcnt += 1;
		}
	}
	flushbits(bstr, skipcnt);

	if(bstr->incnt!=0)
	{
		bstr->m_rdptr -= bstr->incnt>>3;
		bstr->incnt = 0;
	}
}
