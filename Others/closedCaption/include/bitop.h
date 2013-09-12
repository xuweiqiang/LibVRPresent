/***************************************************************************
 *   File Name: bitop.h
 *   Create Date: 2007-05-14 (YYYY-MM-DD)
 *   Vesion: 01.00.00 (xx.xx.xx)
 *   Copyright (C) 2006 by Cidana(ShangHai)                                      
 *   email: toby.hu@cidana.com                                                     
 *                                                                         
 *   This program is distributed in the hope that it will be useful,       
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 *                          
 *   The above copyright notice and this permission notice shall be 
 *   included in all copies or substantial portions of the Software.
 *                                                                         
 *   You should have received a copy of the Cidnana License     
 *   along with this program; if not, write to the                         
 *   ShangHai Cidana, Inc.                                       
 ***************************************************************************/
#ifndef _BITOP_H_
#define _BITOP_H_

typedef struct _NewBstr_
{		
	unsigned long code;
	long incnt;	
	unsigned char *m_SrcBufptr;
	unsigned char *m_rdptr;	
	unsigned char *m_endptr;	
	long m_SrcPos;
	long m_SrcLength;
	unsigned long m_llTotalBitsCount;

} NewBstr;

unsigned char* GetBstrBegin(NewBstr *bstr);
unsigned char* GetBstrEnd(NewBstr *bstr);
int GetBstrSize(NewBstr *bstr);
int GetBstrOffset(NewBstr *bstr);
int SeekBstr(NewBstr *bstr, int offset, int where);
void InitNewBstr(NewBstr *bstr, unsigned char *streamBuffer, long length);
void FillBuffer(NewBstr *bstr);
int bytealigned(NewBstr *bstr, int nbit);	
unsigned long Get_Bits(NewBstr *bstr, const int numBits);
int GetBytes(NewBstr *bstr, unsigned char* dst, const int bytes);
unsigned long PeekBits(NewBstr *bstr, const int numBits);
unsigned long PeekBits32(NewBstr *bstr);	
void flushbits(NewBstr *bstr, int n);
void Reinit(NewBstr *bstr);
void MakeByteAligned(NewBstr *bstr);
unsigned long PeekBitsFromByteAlign(NewBstr *bstr, int nBitsToPeek);
unsigned long PeekBitsTillByteAlign(NewBstr *bstr);
int NumOfBitsInByte(NewBstr *bstr);

#endif // _BITOP_H_

