//=============================================================================
//  CIDANA CONFIDENTIAL INFORMATION
//
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO CIDANA, INC.
// 	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//	COMPANY.
//
// 	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// 	PURPOSE.
//
// 	Copyright (c) 2008  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

// Mpeg2Demux.cpp : Defines the entry point for the console application.
//

#define _CRT_SECURE_NO_DEPRECATE
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#if defined(WIN32) && !defined(_WIN32_WCE)
#include  <crtdbg.h>
#endif
#ifdef _PROFILE
#include<profiler.h>
#endif

#include "MP2Dmx.h"
#include "isdbt_cc.h"

struct CC_DEC {
	FILE *outfile;
	ISDBTCC_HANDLE cc_handle;
	ISDBTCC_OpenOptions	OpenOption;
	DWORD dwBufferLength;
	DWORD dwPts;
	BYTE  aBuffer[1024];
};

#ifndef _WIN32_WCE
void __cdecl atexitfunc(void)
{
	//DP("Exiting!\n");
}
#endif

void WriteYUV(unsigned char* yuv[3], DWORD w[3], DWORD h[3], DWORD stride[3], FILE *pFile)
{
	int i, j;
	unsigned char *pData;

	if(!pFile)
		return;

	//Y
	pData = yuv[0];
	for(j=0;j<h[0];j++)
	{
		fwrite(pData, w[0], 1, pFile);
		pData += stride[0];
	}

	//U
	pData = yuv[1];
	for(j=0;j<h[1];j++)
	{
		fwrite(pData, w[1], 1, pFile);
		pData += stride[1];
	}

	//U
	pData = yuv[2];
	for(j=0;j<h[2];j++)
	{
		fwrite(pData, w[2], 1, pFile);
		pData += stride[2];
	}
}

static HRESULT CcListener(
    IN PVOID pvContext,
	IN DWORD dwID,				// replicate the matching 
	IN DWORD dwStatusFlags,		// status flags
	IN PBYTE pHeader,			// header corresponding to data (may match pData if type is TSPACKET)
	IN PBYTE pOutputData,		// "payload"
	IN DWORD dwSize,			// size of payload
	IN PVOID pExtraData,
	IN DWORD dwExtraDataSize	
	)
{
	MP2Dmx_ExtraData *Mp2ExtraData = (MP2Dmx_ExtraData*)pExtraData;
	/*
	if(dwStatusFlags&MP2DMX_TSFLAGS_DISCONTINUITY_INDICATOR)
		printf("MP2DMX_TSFLAGS_DISCONTINUITY_INDICATOR\n");

	if(dwStatusFlags&MP2DMX_TSFLAGS_DISCONTINUOUS_PACKET)
		printf("DISCONTINUOUS\n");
	*/
	CC_DEC* cc_dec = (CC_DEC*)pvContext;

	if(cc_dec==NULL)
		return S_OK;

	ISDBTCC_Output			output;	

	memset(&output, 0, sizeof(output));
	while(ISDBTCC_GetOutput(cc_dec->cc_handle, &output)>=0)
	{
		if(cc_dec->OpenOption.OutputFormat==YV12)
			WriteYUV(output.pBuffer, output.dwWidth, output.dwHeight, output.dwStride, cc_dec->outfile);
		memset(&output, 0, sizeof(output));
	};


	if(dwStatusFlags&MP2DMX_TSFLAGS_PAYLOAD_UNIT_START_INDICATOR)
	{
		if(cc_dec->dwBufferLength)
		{
			ISDBTCC_DecodeOptions	decode_option;

			memset(&decode_option, 0, sizeof(decode_option));
			
			ULONGLONG pts_cc = cc_dec->dwPts;
			decode_option.dwTimeStamp = pts_cc;			
			//do decoding
			DWORD DataLen = cc_dec->dwBufferLength;
			ISDBTCC_Decode(cc_dec->cc_handle, &decode_option, cc_dec->aBuffer, &DataLen);
		}
		cc_dec->dwBufferLength = 0;
	}	

	if(cc_dec->dwBufferLength+dwSize < sizeof(cc_dec->aBuffer))
	{
		if(dwStatusFlags&MP2DMX_PES_PTS_PRESENT)
			cc_dec->dwPts = Mp2ExtraData->ullPts*1000/90000;
		memcpy(&cc_dec->aBuffer[cc_dec->dwBufferLength], pOutputData, dwSize);
		cc_dec->dwBufferLength += dwSize;
	}
	return S_OK;
}

int main(int argc, char* argv[])
{
	int chars, offset;
	FILE *inp=0, *out=0;
	unsigned char buffer[1024];	
	MP2Dmx_HANDLE hDemux = 0;
	MP2Dmx_OpenOptions OpenOptions;
	MP2Dmx_AddListenerOptions CcOptions;
	COOKIE CcCookie=0;

#if defined(_DEBUG) && defined(WIN32) && !defined(_WIN32_WCE)
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag); 
	atexit(atexitfunc);
#endif
#ifdef _PROFILE
	__profiler_start();
#endif
	if(argc<2)
		return -1;
	if((inp=fopen(argv[1],"rb"))==0)
		return -1;	
	if(argc==3)
	{
		if((out=fopen(argv[2],"wb"))==0)
		{
			fclose(inp);			
			fclose(out);			
			return -1;
		}
	}

	// construct demux
	if(FAILED(MP2Dmx_Create(&hDemux)))
		return -1;
	// open demux
	ZeroMemory(&OpenOptions,sizeof(OpenOptions));	
	OpenOptions.dwOpenFlags = MP2DMX_OPENFLAGS_TSSTREAM;
	if(FAILED(MP2Dmx_Open(hDemux,&OpenOptions,sizeof(OpenOptions))))
		return -1;

	// add closed caption listener
	CC_DEC cc_dec;
	memset(&cc_dec, 0, sizeof(cc_dec));
	cc_dec.outfile = out;
	ZeroMemory(&CcOptions,sizeof(CcOptions));
	//CcOptions.dwID= 0x287;	
	CcOptions.dwID= 647;
	CcOptions.dwFlags = MP2DMX_PIDFLAGS_DELIVER_TSELEMENTARY;
	CcOptions.pListener = CcListener;
	CcOptions.pvListenerContext = &cc_dec;
	MP2Dmx_AddListener(hDemux,&CcOptions,sizeof(CcOptions),&CcCookie);	

	MP2Dmx_Stop(hDemux, MP2DMX_STOPFLAGS_ALL);	

	//create cc decoder
	ISDBTCC_HANDLE hCc;
	ISDBTCC_OpenOptions CcOpenOption;
	ISDBTCC_DecodeOptions CcDecodeOption;
	ISDBTCC_Output CcOutput;

	memset(&CcOpenOption, 0, sizeof(CcOpenOption));
	memset(&CcDecodeOption, 0, sizeof(CcDecodeOption));
	memset(&CcOutput, 0, sizeof(CcOutput));

	if(ISDBTCC_Create(&hCc)<0)
		return -1;

	cc_dec.cc_handle = hCc;
	
	CcOpenOption.dwDefaultLanguageCode = 'jpn';
	CcOpenOption.dwOutputBufferNumber = 2;
	CcOpenOption.dwFontWidth = 16;
	CcOpenOption.dwFontHeight = 16;
	CcOpenOption.dwHeight = 64;
	CcOpenOption.dwWidth = 320;
	CcOpenOption.OutputFormat = YV12;
	CcOpenOption.dwCharacterPerLine = 20;
	CcOpenOption.dwMaxLines = 4;

	if(ISDBTCC_Open(hCc, &CcOpenOption)<0)
	{
		return -1;
	}

	ISDBTCC_DEFAULT_COLOR background_color = CC_BLACK;

	ISDBTCC_Set(hCc, ISDBTCC_BACKGROUND_COLOR, &background_color, sizeof(DWORD));

	memcpy(&cc_dec.OpenOption, &CcOpenOption, sizeof(CcOpenOption));
	
	// pump data 
	for(offset=0;(chars=(int)fread(buffer,1,sizeof(buffer),inp));offset+=chars)
	{
		if(FAILED(MP2Dmx_ProcessInputData(hDemux,buffer,chars)))
			return -1;		
		//break;
	}
	// close demux
	MP2Dmx_RemoveListener(hDemux,CcCookie);
	
	if(FAILED(MP2Dmx_Close(hDemux)))
		return -1;
	// deallocate demux
	MP2Dmx_Release(hDemux);
	hDemux = 0;

	//close closed caption
	ISDBTCC_Close(hCc);
	ISDBTCC_Release(hCc);


	if(inp)
		fclose(inp);
	if(out)
		fclose(out);	
#ifdef _PROFILE
	__profiler_dump();
#endif
}