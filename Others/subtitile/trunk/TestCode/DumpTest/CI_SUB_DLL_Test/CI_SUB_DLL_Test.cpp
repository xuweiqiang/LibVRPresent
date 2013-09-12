// CI_SUB_DLL_Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "SUBDecoder.h"
#include "Library_probe.h"

#if defined(_DEBUG) && defined(WIN32) && !defined(_WIN32_WCE)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#define CI_SUB_TEST_RES_OK			0
#define CI_SUB_TEST_RES_INPUTERR	1
#define CI_SUB_TEST_RES_INITERR		2
#define CI_SUB_TEST_RES_TERMERR		3
#define CI_SUB_TEST_RES_STARTERR	4
#define CI_SUB_TEST_RES_STOPERR		5
#define CI_SUB_TEST_RES_LOADDLLERR	6

static CLibLoad		*g_pLibLoader = NULL;
static ISUBDecoder	*g_pDecoderSUB = NULL;
static BYTE *g_pTEXTData = NULL;
static BYTE *g_pPack = NULL;
static TCHAR *pTestFileName = NULL;
static CI_SUB_COLOR_SPACE	g_ColorType = CI_SUB_COLOR_SPACE_RGBA8888;

static DWORD PumpPESThread(PVOID pArg);
static void SaveSUBPage(unsigned char *pRegionBuffer, CI_SUB_Rect DispRect, void *pContext, bool bShow);

#define COMP_DEBUG_PRINT
#ifdef COMP_DEBUG_PRINT
// #define DP_FILE

void DP_EWESG(char *szFormat,...)
{
	char buffer[256] = {0};
	va_list ap;
	va_start(ap, szFormat);
	_vsnprintf(buffer, 255, (char*)szFormat, ap);
#ifdef DP_FILE
	FILE *f_txt = fopen("ewesg_log.txt", "at");
	SYSTEMTIME st;
	GetSystemTime(&st);              // gets current time
	fprintf(f_txt,"[%d/%d/%d %d:%d:%d][EwESG]",st.wYear, st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	fprintf(f_txt, buffer);
	fclose(f_txt);
#else
	TCHAR t_buffer[256] = {0};
	MultiByteToWideChar(CP_ACP,0,buffer,-1,t_buffer,255);
	t_buffer[255]=0;
	OutputDebugString(t_buffer);
#endif
	va_end(ap);
	return;
}
#else
#define DP_EWESG(x,...) x
#endif


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2)
		return CI_SUB_TEST_RES_INPUTERR;

	DWORD dwTimeout = INFINITE; 
	if (argc > 3)
	{
		// Input timeout value
		DWORD dwdwNumTmp = _ttol(argv[3]);
		if (dwdwNumTmp == 0)
		{
			// arg #2 is not timeout number
// 			pTestFileName = argv[2];
			if (argc > 4)
				dwdwNumTmp = _ttol(argv[4]);
		}
		else
		{
			dwTimeout = dwdwNumTmp * 1000;
		}
	}
	pTestFileName = argv[1];

	g_pLibLoader = new CLibLoad();
	if (g_pLibLoader == NULL)
		return CI_SUB_TEST_RES_LOADDLLERR;

	HRESULT Res = S_OK;
	if ((Res = g_pLibLoader->Init(_T("libSUBDecoder"))) != S_OK)
		return CI_SUB_TEST_RES_LOADDLLERR;

	ISUBDecoder* (*pSUBCreator)() = NULL;
	while((Res == S_OK) && (pSUBCreator == NULL))
	{
		Res = g_pLibLoader->Probe((void **)(&pSUBCreator), _T("CreateSUBDecoder"));
		if((Res == S_OK) && (pSUBCreator != NULL))
		{
			g_pDecoderSUB = pSUBCreator();	
		}
	}

	if(!g_pDecoderSUB)
	{
		return CI_SUB_TEST_RES_INITERR;
	}

	CI_SUB_Open_Option OpenOption;
	memset(&OpenOption, 0, sizeof(CI_SUB_Open_Option));
	OpenOption.ColorType = g_ColorType = CI_SUB_COLOR_SPACE_YCBCRT_HDTV;
	OpenOption.usCompositionPageID = 1;
	OpenOption.usAncillaryPageID = 1;
	OpenOption.SrcRect.lRight = 720;
	OpenOption.SrcRect.lBottom = 576;
	if (g_pDecoderSUB->Open((DWORD)&OpenOption) != S_OK)
	{
		return CI_SUB_TEST_RES_STARTERR;
	}

	GUID UnknownGUID;
	if (g_pDecoderSUB->Set(UnknownGUID, EMC_PROPID_SUBTITLE_DISP_ON, NULL, 0, NULL, 0) != S_OK)
	{
		return CI_SUB_TEST_RES_STARTERR;
	}
	if (g_pDecoderSUB->Set(UnknownGUID, EMC_PROPID_SUBTITLE_NOSYNC, NULL, TRUE, NULL, 0) != S_OK)
	{
		return CI_SUB_TEST_RES_STARTERR;
	}
	if (g_pDecoderSUB->Set(UnknownGUID, EMC_PROPID_SUBTITLE_SETDRAWCB, (LPVOID)(argv[2]), 0, SaveSUBPage, 0) != S_OK)
	{
		return CI_SUB_TEST_RES_STARTERR;
	}

	DWORD dwPumpThreadID = 0;
	HANDLE hPumpThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PumpPESThread, (LPVOID)pTestFileName, CREATE_SUSPENDED, &dwPumpThreadID);

	if (hPumpThread != INVALID_HANDLE_VALUE)
	{
		ResumeThread(hPumpThread);
	}

	if (WaitForSingleObject(hPumpThread, dwTimeout) == WAIT_TIMEOUT)
	{
		TerminateThread(hPumpThread, dwPumpThreadID);
		CloseHandle(hPumpThread);
		printf("Timeout\n");
	}

	if (g_pDecoderSUB->Close() != S_OK)
	{
		return CI_SUB_TEST_RES_STOPERR;
	}

	g_pDecoderSUB->Release();
	g_pDecoderSUB = NULL;

	g_pLibLoader->Uninit();

	delete g_pLibLoader;
	g_pLibLoader = NULL;

#if defined(_DEBUG) && defined(WIN32) && !defined(_WIN32_WCE)
	_CrtDumpMemoryLeaks();
#endif

#ifndef _WIN32_WCE
	//system("pause");
#endif

	return CI_SUB_TEST_RES_OK;
}

static int Get_TEX()
{
	unsigned char payload_unit_start_indicator = (g_pPack[1]&0x40)>>6;
	unsigned char adaptation_field_control = (g_pPack[3]>>4)&0x03;
	int pos = 0;

	switch(adaptation_field_control) 
	{
	case 1:
		{
			//no adaptation_field
			pos = 4;
			return pos;
		}
		break;
	case 2:
		{
			//adaptation field only, no payload
			return -1;
		}
		break;
	case 3:
		{
			//adaptation_field
			unsigned char adaptation_field_length = g_pPack[4];
			pos = 4 + adaptation_field_length + 1;
			return pos;
		}
		break;
	default:
		return -1;//do not porcess this pack
	}
}

static int GetPES(BYTE *&pBuffer, DWORD &wSize, FILE *pFile)
{
	int pos = 0;
	BOOL begin = FALSE;
	DWORD d_length = 1;
	int remain = 0; 
	int pre_len = 0; 
	size_t available = 0 ;
	int counter = 16, pre_counter = 16;

	do 
	{   
		if (pFile != NULL)
			available = fread(g_pPack, 1, 188, pFile);

		if (available > 0)
		{
			if (g_pPack[0] == 0x47)
			{
				pos = Get_TEX();

				if(pos < 0)
					continue;
				else
				{
					counter = g_pPack[3]&0xf;                     
					if (begin == 0)
					{
						if(g_pPack[pos] == 0x00 && g_pPack[pos + 1] == 0x00 && g_pPack[pos + 2] == 0x01 && g_pPack[pos + 3] == 0xBD)
						{
							begin = 1;

							if (g_pTEXTData)
							{
								memcpy(g_pTEXTData, &g_pPack[pos], 188 - pos);
								d_length = (*(g_pTEXTData + 4) << 8)| *(g_pTEXTData + 5) + 6;//the size of this section
								wSize = d_length;
								d_length = d_length - (188 - pos);
								pre_len = (188 - pos);
								pre_counter = counter;
							}
							else
								printf("memcpy error!!");
						}
					}
					else
					{
						if(counter != ((pre_counter + 1) % 16))
						{
							memset (g_pTEXTData, 0, 65536);
							wSize = 0;
							counter = pre_counter = 16;
							break;
						}
						else
						{
							if (g_pTEXTData)
							{   
								if((unsigned int)(188 - pos) > d_length)
									remain = d_length;
								else
									remain = 188 - pos;
								memcpy(g_pTEXTData + pre_len ,&g_pPack[pos] ,remain);
								pre_len = pre_len + remain;
								d_length = d_length - (remain);
								pre_counter = counter;
							}
							else
								printf("memcpy error!!");
						}
					}
				}
			}
		}
	}
	while(d_length > 0 && available > 0);

	pBuffer = g_pTEXTData;

	return wSize;
}

DWORD PumpPESThread(PVOID pArg)
{	
	_TCHAR *pTSFileName = (_TCHAR *)pArg;

	if (pTSFileName == NULL)
		return -1;

	FILE *pFile = _tfopen(pTSFileName, _T("rb"));

	if (!g_pTEXTData)
	{
		g_pTEXTData = new BYTE[65536];
		memset(g_pTEXTData, 0, 65536);
	}

	if(!g_pPack)
	{
		g_pPack = new  BYTE[188];
		memset(g_pPack, 0, 188);
	}


	while (!feof(pFile))
	{
		BYTE *pPESPacket = NULL;
		DWORD dwPacketSize = 0;
		GetPES(pPESPacket, dwPacketSize, pFile);
		if (dwPacketSize > 0)
		{
			g_pDecoderSUB->SendPES(pPESPacket, dwPacketSize);
			g_pDecoderSUB->SendPCR(0);
			Sleep(1000);
		}
		else
		{
			Sleep(10);
		}
	}

	if(g_pPack)
	{
		delete []g_pPack;
		g_pPack = NULL;
	}

	if (g_pTEXTData)
	{
		delete []g_pTEXTData;
		g_pTEXTData = NULL;
	}

	fclose(pFile);

	return 0;
}

void CopyToDestBuffer(char* pDestBuffer, CI_SUB_Rect &DispRect, unsigned char * pRegionBuffer )
{
#define OUTPUT_W 720
#define OUTPUT_H 576
	char *DestY, *DestU, *DestV;
	char *SrcY, *SrcU, *SrcV;
	long DestStretch = OUTPUT_W;
	long SrcStretch = DispRect.lRight - DispRect.lLeft;
	long TotalLine = DispRect.lBottom - DispRect.lTop;
	memset(pDestBuffer, 0x00, OUTPUT_W * OUTPUT_H * 3 / 2);
	DestY = pDestBuffer + DestStretch * DispRect.lTop + DispRect.lLeft;
	DestU = pDestBuffer + DestStretch * OUTPUT_H + // skip Y
		DestStretch * DispRect.lTop / 4 + DispRect.lLeft / 2;
	DestV = pDestBuffer + DestStretch * OUTPUT_H * 5 / 4 + // Skip Y and U
		DestStretch * DispRect.lTop / 4 + DispRect.lLeft / 2;
	if(pRegionBuffer)
	{
		SrcY = (char*)pRegionBuffer;
		SrcU = (char*)pRegionBuffer + TotalLine*SrcStretch;
		SrcV = (char*)pRegionBuffer + TotalLine*SrcStretch*5/4;
	}
	for(int i = 0; i < TotalLine; i ++)
	{
		if(pRegionBuffer)
		{
			memcpy(DestY, SrcY, SrcStretch);
		}
		else
		{
			memset(DestY, 0x80, SrcStretch);
		}
		DestY += DestStretch;
		if(pRegionBuffer)
			SrcY += SrcStretch;
		if(i%2 == 0)
		{
			if(pRegionBuffer)
			{
				memcpy(DestU, SrcU, SrcStretch/2);
			}
			else
			{
				memset(DestU, 0x80, SrcStretch/2);
			}
			DestU += DestStretch/2;
			if(pRegionBuffer)
				SrcU += SrcStretch/2;
		}
		else
		{
			if(pRegionBuffer)
			{
				memcpy(DestV, SrcV, SrcStretch/2);
			}
			else
			{
				memset(DestV, 0x80, SrcStretch/2);
			}
			DestV += DestStretch/2;
			if(pRegionBuffer)
				SrcV += SrcStretch/2;
		}
	}
}
void SaveSUBPage(unsigned char *pRegionBuffer, CI_SUB_Rect DispRect, void *pContext, bool bShow)
{
	if (pContext)
	{
		TCHAR szDumpFileName[MAX_PATH] = {0};
		_tcscpy(szDumpFileName, (TCHAR*)pContext);

		FILE *pfDump = _tfopen(szDumpFileName, _T("ab"));
		if (pfDump)
		{
			int nBufSize = 0;
			switch(g_ColorType)
			{
			case CI_SUB_COLOR_SPACE_YCBCRT_SDTV:
			case CI_SUB_COLOR_SPACE_YCBCRT_HDTV:
				{
					nBufSize = (DispRect.lRight - DispRect.lLeft) * (DispRect.lBottom - DispRect.lTop) * 3/2;
				}
				break;
			case CI_SUB_COLOR_SPACE_RGBA8888:
				{
					// Prepare BMP header
					nBufSize = (DispRect.lRight - DispRect.lLeft) * (DispRect.lBottom - DispRect.lTop) * 4;
				}
				break;
			default:
				break;
			}
			static DWORD dwTick = 0;
			if (nBufSize > 0)
			{
				dwTick = GetTickCount();
				// check empty header
				char* outBuffer = new char[720*576*3/2];
				CopyToDestBuffer(outBuffer, DispRect, pRegionBuffer);
				fwrite(outBuffer, 1, 720*576*3/2, pfDump);
				delete [] outBuffer;
			}
			else
			{
				DP_EWESG("Subtitle Takes %u ms\n", GetTickCount() - dwTick);
			}
			fclose(pfDump);
		}
	}
}


