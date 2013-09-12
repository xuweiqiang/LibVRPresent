// ST_DEMODlg.cpp : implementation file
//

#include "stdafx.h"
#include "ST_DEMO.h"
#include "ST_DEMODlg.h"
#include "../../3rdparty/st/stglib.h"
#pragma comment(lib, "../../3rdparty/st/sgalib.lib")

#pragma comment(lib, "../../3rdparty/st/STSGA_mbuffer.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define NAME                TEXT("STDEMOWndClass")
#define TITLE               TEXT("STDEMO")
// CST_DEMODlg dialog

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32) && !defined(_WIN32_WCE)
#include  <crtdbg.h>
#endif

#include "ci_codec_type.h"
#include "ci_h264bpdec.h"
#include "General_StreamInput.h"

static STHDC m_hdc ;
typedef struct
{
	STHBITMAP	bitmap;
	void*	pBuf;
	void* pPhysicalAddr;
	int size;
}ST_BITMAP_S;
ST_BITMAP_S m_bitmap[20];

#pragma comment(lib, "H264Component_armv6.lib")
#pragma comment(lib, "H264DecBL.lib")

static int g_frameCount = 0;
static long g_startTick = 0;

#define stream800

//#define SW_MODE
#ifdef stream800
#define m_vSourceWidth 800
#define m_vSourceHeight 480
#else
#define m_vSourceWidth 320
#define m_vSourceHeight 180
#endif

#define m_vDestStartX	0
#define m_vDestStartY  0
#define m_vDestWidth  800
#define m_vDestHeight 480

#define PICUTRE_QUALITY CI_DECODE_QUALITY_0
#ifdef _UNICODE
#define _tstoi		_wtoi
#define _tmain		wmain
#define _tchar		wchar_t
#define _tfopen		_wfopen
#define _tprintf	wprintf
#define _ftprintf	fwprintf
#define _T(x)		L ## x
#define _tstrlen    wcslen
#else	/* not _UNICODE */
#define _tstoi		atoi
#define _tmain		main
#define _TCHAR		char
#define _tfopen		fopen
#define _tprintf	printf
#define _ftprintf	fprintf
#define _T(x)		x
#define _tstrlen 	strlen
#endif	/* _UNICODE */

TCHAR tc_path[MAX_PATH]= {0};
TCHAR DirPath[MAX_PATH]= {0};
TCHAR FilePath[MAX_PATH] = {0};
IMcVideoPresenter* g_pPresent = NULL;
HWND g_hWnd = 0;	
static HANDLE g_event = 0;


#if defined(WIN32)||defined(UNDER_CE)
#include "windows.h"
static void PFN_InitLock_IMPL(void** ppLock)
{
	CRITICAL_SECTION *pcs;

	pcs = (CRITICAL_SECTION *)malloc(sizeof(CRITICAL_SECTION));
	InitializeCriticalSection(pcs);
	*ppLock = (void *)pcs;
}

static void PFN_DeleteLock_IMPL(void** ppLock)
{
	CRITICAL_SECTION *pcs = (CRITICAL_SECTION *)*ppLock;

	DeleteCriticalSection(pcs);
	free(pcs);
	*ppLock = 0;
}

static void PFN_Lock_IMPL(void* pLock)
{
	EnterCriticalSection((CRITICAL_SECTION*)pLock);
}

static void PFN_UnLock_IMPL(void* pLock)
{
	LeaveCriticalSection((CRITICAL_SECTION*)pLock);
}

#else

static void PFN_InitLock_IMPL(void** ppLock){}

static void PFN_DeleteLock_IMPL(void** ppLock){}

static void PFN_Lock_IMPL(void* pLock){}

static void PFN_UnLock_IMPL(void* pLock){}

#endif

static int InitFile();
static int UninitFile();

static int InitDecoder();
static int UnInitDecoder();
static int DecoderRenderLoop();
static int InitRender();
static int UninitRender();


int i;
CI_H264BPDEC_OPENOPTIONS OpenOptions;
CI_H264BPDEC_DECODEOPTIONS DecodeOptions;
CI_H264BPDEC_GETFRAMEOPTIONS GetFrameOptions;
CI_H264BPDEC_FRAME Frame;

#define QUEUE_SIZE 	100
CI_H264BPDEC_FRAME RenderFrame[QUEUE_SIZE];
int g_WriteCnt = 0;
int g_ReadCnt = 0;
CI_VOID *Decoder = NULL;
CI_U32 dwTotalDecodedFrames;
CI_U32 dwStartTime;
FILE *fpOutput = NULL;
FILE *fpRsz = NULL;
FILE *fpLog = NULL;
CI_U32 DispQueueLock;

FILE *pInFile = NULL;
unsigned char *StreamBuffer = NULL;
unsigned char *StreamCurPos = NULL;
unsigned char *FrameEndPos = NULL;
int BufLen = 0;
int time1 = 0, time2 = 0, sum_time = 1,	starttime = 0, endtime = 0,diftime = 0;
char * pname;
int decode_count = 0;

CI_U32 confidence = 0;
CI_H264BPDEC_STREAMINFO StreamInfo = {0};
CI_RESULT ret;

long FAR PASCAL
WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int                         NewAngle;
    DEVMODE                     DevMode;

    switch (message)
    {
#ifdef UNDER_CE
        case WM_ACTIVATE:
#else
        case WM_ACTIVATEAPP:
#endif
            // Pause if minimized or not the top window
            return 0L;

        case WM_KILLFOCUS:
            // We do not allow anyone else to have the keyboard focus until
            // we are done.
            SetFocus(hWnd);
            return 0L;

        case WM_DESTROY:
            // Clean up and close the app
            PostQuitMessage(0);
            return 0L;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}


CST_DEMODlg::CST_DEMODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CST_DEMODlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(tc_path, 0 ,sizeof(tc_path));
	memset(DirPath, 0 ,sizeof(DirPath));
	memset(FilePath, 0 ,sizeof(FilePath));
}

void CST_DEMODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CST_DEMODlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CST_DEMODlg::OnBnClickedButtonOpen)
END_MESSAGE_MAP()


// CST_DEMODlg message handlers

BOOL CST_DEMODlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	OnBnClickedButtonOpen();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CST_DEMODlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_ST_DEMO_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_ST_DEMO_DIALOG));
	}
}
#endif
DWORD WINAPI DecoderThreadProc( LPVOID lpParameter)
{
	DecoderRenderLoop();

	return 0;
}



static void __cdecl DP(LPCSTR szFormat, ...)
{
#ifndef ANDROID
#ifdef __linux__
	TCHAR szBuffer[256];
	va_list vl;
	va_start(vl, szFormat);
	_vsntprintf(szBuffer,255,szFormat, vl);
	szBuffer[255]=0;
	fprintf(stderr,"%s",szBuffer);
	va_end(vl);
#else	
	char buffer[256] = {0};
	char bufferTime[256] = {0};
	SYSTEMTIME time;	
	va_list ap;
	va_start(ap, szFormat);
	_vsnprintf(buffer, 255, (char*)szFormat, ap);
	GetSystemTime(&time);
	sprintf(bufferTime, "%d/%d/%d, %d:%d:%d ", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

#if 1//def _DEBUG_CONSOLE
	TCHAR tBufferTime[MAX_PATH];
	TCHAR tBuffer[MAX_PATH];

	MultiByteToWideChar(CP_UTF8, 0, bufferTime, -1, tBufferTime, sizeof(tBufferTime));
	MultiByteToWideChar(CP_UTF8, 0, buffer, -1, tBuffer, sizeof(tBuffer));

	//OutputDebugString(tBufferTime);
	//OutputDebugString(tBuffer);
#endif
#if 1//def WRITE_FILE
	FILE* f_txt = fopen(".\\log.txt", "a+");
	if(f_txt)
	{
		fprintf(f_txt, bufferTime);
		fprintf(f_txt, buffer);
		fclose(f_txt);
	}
#endif

	va_end(ap);
#endif
#else
#if 1
#include "utils/Log.h"

	char szBuffer[256]; 
	va_list vl; 
	va_start(vl, szFormat);

	memset(szBuffer, 0, sizeof(szBuffer));

	vsnprintf(szBuffer,255,szFormat,vl);
	szBuffer[255]=0;

	LOGD("%s", szBuffer);
	va_end(vl);
#endif
#endif
	return;
}

DWORD WINAPI RenderThreadProc( LPVOID lpParameter)
{
	wchar_t pBuf[MAX_PATH];
	memset(&RenderFrame, 0 ,sizeof(RenderFrame));
	DWORD starttick = 0;//GetTickCount();
	DWORD startIdle = 0;//GetIdleTime();
	DWORD counttick = 0;//GetTickCount();
	DWORD countIdle = 0;//GetIdleTime();
	DWORD cntStTick = 0;//GetTickCount();

	
//	STScreenColorEnhancement(m_hdc,false);
	OutputDebugString(_T("STScreenColorEnhancement(m_hdc,false) \n"));

	while(1)
	{
		if(WaitForSingleObject(g_event,INFINITE) == WAIT_OBJECT_0)
		{


			static bool enable = false;
			if(enable == false)
			{
				enable = true;
				g_pPresent->Set(IID_IMcVideoPresenter,MC_PROPID_VP_ENABLE,0,0,&enable ,0);

				RECT Dstrect;
				Dstrect.left = m_vDestStartX;
				Dstrect.right= Dstrect.left + m_vDestWidth;
				Dstrect.top = m_vDestStartY;
				Dstrect.bottom = Dstrect.top + m_vDestHeight;
				g_pPresent->Set(IID_IMcVideoPresenter,MC_PROPID_VP_RECTDST,0,0,&Dstrect,0);

			}

			{
				unsigned int i, j, k;
				unsigned char *ptr, *temp;

				DWORD strides[3];
				LPBYTE pBuffer[3];
				pBuffer[0] = RenderFrame[g_ReadCnt].apu8Frame[0];
				pBuffer[1] = RenderFrame[g_ReadCnt].apu8Frame[1];
				pBuffer[2] = RenderFrame[g_ReadCnt].apu8Frame[2];

				strides[0] = RenderFrame[g_ReadCnt].au32Stride[0];
				strides[1] = RenderFrame[g_ReadCnt].au32Stride[1];
				strides[2] = RenderFrame[g_ReadCnt].au32Stride[2];
				if(g_frameCount == 0)
				{
					starttick = GetTickCount();
					startIdle = GetIdleTime();
					counttick = GetTickCount();
					countIdle = GetIdleTime();
				}
				else
				{
					LONG sleepTick = 41.7*g_frameCount - (GetTickCount() - starttick );
//					LONG sleepTick = 55.5*g_frameCount - (GetTickCount() - starttick );
					if(sleepTick > 0)
						Sleep(sleepTick);
		
				}

				g_frameCount++;

#if 0

				int count = 0; 
				while(count < 20){

					if((((DWORD)(m_bitmap[count].pBuf) ) <=((DWORD)pBuffer[0])) && 
						(((DWORD)(m_bitmap[count].pBuf) + m_bitmap[count].size) >=((DWORD)pBuffer[0])))
						break;
					count++;

				};
				if(count >= 20)
				{
					OutputDebugString(_T("NOT FOUND "));
					break;
				}

				DWORD presentTick = GetTickCount();

/*
				VP_STRUCT_ST_PARMS input;
				memset(&input, 0 ,sizeof(input));
				input.pPhyAddr = (void*)(((DWORD)m_bitmap[count].pPhysicalAddr + offset) );
				input.pVirtualAddr = (void*)(((DWORD)m_bitmap[count].pBuf+ offset));
				input.hnd = m_hdc;
				input.yStride = strides[0];
				input.uPlaneOffset = pBuffer[2] - pBuffer[0];
				input.vPlaneOffset = pBuffer[1] - pBuffer[0];
				
				g_pPresent->Set(IID_IMcVideoPresenter,MC_PROPID_VP_VP_HND,0,0,&input ,0);
*/
				LPBYTE pBufferPhy[3];

				DWORD offsetY = pBuffer[0] - m_bitmap[count].pBuf;
				DWORD offsetU = pBuffer[1] - m_bitmap[count].pBuf;
				DWORD offsetV = pBuffer[2] - m_bitmap[count].pBuf;
				pBufferPhy[0] = (LPBYTE)(((DWORD)m_bitmap[count].pPhysicalAddr + offsetY));
				pBufferPhy[1] = (LPBYTE)(((DWORD)m_bitmap[count].pPhysicalAddr + offsetU));
				pBufferPhy[2] = (LPBYTE)(((DWORD)m_bitmap[count].pPhysicalAddr + offsetV));
				
				
				g_pPresent->Present(pBufferPhy,strides,0);
#endif
				g_pPresent->Present(pBuffer,strides,0);

				DWORD time = 10;
				if(GetTickCount() - g_startTick> time*60*1000)
				{
					swprintf(pBuf,_T("%d min render:%d "),time, g_frameCount );
					OutputDebugString(pBuf);
					exit(0);
				}

			}	
			if(CI_SOK != CI_H264BPDEC_ReleaseFrame(Decoder, &RenderFrame[g_ReadCnt]))
				break;

			g_ReadCnt++;
			if(g_ReadCnt>=QUEUE_SIZE)
			{
				g_ReadCnt = 0;
			}

		}
	}

	return 0;
}

void CST_DEMODlg::OnBnClickedButtonOpen()
{
	int ret = 0;

	ret = InitFile();
	if(FAILED(ret))	return;
	
	ret = InitDecoder();
	if(FAILED(ret))	return;

	ret = InitRender();
	if(FAILED(ret))	return;

	g_event = CreateSemaphore(NULL, 0, QUEUE_SIZE, NULL);
	
	HANDLE decodeHnd = ::CreateThread(NULL, 0, DecoderThreadProc, 0, 0, NULL);
	//DecoderRenderLoop();
	RenderThreadProc(0);
	CloseHandle(g_event);

	CloseHandle(decodeHnd);
	UninitRender();
	UnInitDecoder();
	UninitFile();

}
/*
void temp_malloc(CI_U32 size, CI_VOID *pBuf)
{
	STSGA_MEMADR *pAdr = (STSGA_MEMADR*)pBuf;
	pAdr->addr_vir = malloc(size);
	pAdr->addr_phy = pAdr->addr_vir;
	if(!pAdr->addr_vir)
		return;
	return;
	//return malloc(size);
}

void temp_free(void* pBuf)
{
	STSGA_MEMADR *pAdr = (STSGA_MEMADR*)pBuf;
	if(pAdr->addr_vir)
		free(pAdr->addr_vir);
	pAdr->addr_phy = pAdr->addr_vir = NULL;
}
*/

CI_VOID* STMalloc(
	IN CI_U32 size)
{
	TCHAR log[MAX_PATH];
#if 0
	temp_malloc(size,pBuf);
	return;
#else
	static bool binit = false;


	if(binit == FALSE)
	{
		memset(&m_bitmap, 0,sizeof(m_bitmap));
		m_hdc = STGetDC(/*g_hWnd*/ 0);
		if (!m_hdc)
			return 0;
		binit = true;
	}

	int	m_dwBackBuffers = 1;
	STHBITMAP m_hbitYUV_Block;


	int count = 0;
	while(m_bitmap[count].bitmap && count <20)
	{
		count++;
	};

	if(count == 20)
		return 0;

	m_bitmap[count].bitmap  = STCreateDMABitmap(m_hdc, 800, 480, SGA_DIS_FMT_YUV422, size + 1024);
	if(!m_bitmap[count].bitmap)
		return 0;

	int ret = STGetBufferAddress(m_hdc, m_bitmap[count].bitmap , &m_bitmap[count].pBuf, &m_bitmap[count].pPhysicalAddr);

	swprintf(log,_T("Malloc m_bitmap[%d].pBuf:0x%x pPhysicalAddr:0x%x"), count, m_bitmap[count].pBuf, m_bitmap[count].pPhysicalAddr);
	//OutputDebugString(log);
	
	DWORD tick = GetTickCount();

	memset(m_bitmap[count].pBuf, 0,size);
	swprintf(log, _T("time:%d \n"),GetTickCount() - tick);
	//OutputDebugString(log);

	
//	return m_bitmap[count].pBuf;

	m_bitmap[count].size = size + 1024;
	return m_bitmap[count].pBuf;
#endif
}

CI_VOID STFree(IN CI_VOID *pbuf)
{
	TCHAR log[MAX_PATH];
#if 0
	temp_free(pbuf);
	return;
#else
	int count = 0; 
	while(m_bitmap[count].pBuf ==pbuf && count < 20)
	{
		swprintf(log,_T("Free m_bitmap[%d].pBuf:0x%x pPhysicalAddr:0x%x"), count, m_bitmap[count].pBuf, m_bitmap[count].pPhysicalAddr);
		//OutputDebugString(log);

		STDestroyDMABitmap(m_hdc, m_bitmap[count].bitmap);
		memset(&m_bitmap[count], 0, sizeof(ST_BITMAP_S));
	}
	

#endif
}

static inline int rshift_rnd_sf(int x, int a)
{
  return ((x + (1 << (a-1) )) >> a);
}

static inline int iClip1(int high, int x)
{
  x = max(x, 0);
  x = min(x, high);

  return x;
}

static void Chroma_Interpolation_SW(unsigned char *pRef, unsigned char *pDst, int dx, int dy, int srcStep,
	int dstStep)
{

	// Suppose dx & dy are alreayd in the range of 0-7 ??? and pRef is only 8x8 block data. 
	//int subpel_x =  8;
	//int subpel_y =  8;
	int i, j;

	int total_scale = 6;
	int dxcur = (8 - dx);
	int dycur = (8 - dy);

	int tmp_pos;
	int ipos, ipos_p1;

	int result;

	unsigned char *cur_line, *cur_line_p1;

	for (j = 0; j < 8; j++)
	{
		cur_line    = pRef + (j)*srcStep;
		cur_line_p1 = pRef + (j + 1)*srcStep; //If whole frame, stride should be considered

		tmp_pos = 0;

		for (i = 0; i < 8; i++)
		{
			ipos    = tmp_pos++;
			ipos_p1 = tmp_pos;

			result = (
				dxcur * dycur * cur_line   [ipos   ] + 
				dx    * dycur * cur_line   [ipos_p1] +
				dxcur * dy    * cur_line_p1[ipos   ] +
				dx    * dy    * cur_line_p1[ipos_p1]);

			pDst[j*dstStep+i] = iClip1(255, rshift_rnd_sf(result, 6)); // Max value is 255???
		}
	}
}

	
CI_RESULT CI_CheckSkippedFrame(CI_VOID *pContext, CI_U32 u32FrameType, CI_DECODE_PTS *pPts, CI_U32 *pu32Skipped, CI_U32 *pu32PictureQuality)
{
	static CI_U32 g_RefCount = 0;
	static CI_U32 g_PredCount = 0;
	static CI_U32 g_SkipCount = 0;
	if(pu32Skipped == NULL || pPts == NULL)
		return CI_EPOINTER;

	*pu32Skipped = 0;
	*pu32PictureQuality = PICUTRE_QUALITY;

	return CI_SOK;
	if(pPts->u8Valid)
	{
		//Do check with PTS
	}

	if(u32FrameType == CI_VIDEC_SKIP_FRAMETYPE_REFERENCE)
	{
		g_RefCount++;
		if(g_RefCount == 10)
			*pu32Skipped = 1;
	}
	else if(u32FrameType == CI_VIDEC_SKIP_FRAMETYPE_PREDICTION)
	{
		g_PredCount++;
		if(g_PredCount == 20)
			*pu32Skipped = 1;
	}
	else if(u32FrameType == CI_VIDEC_SKIP_FRAMETYPE_SKIPPABLE)
	{
		g_SkipCount++;
		if(g_SkipCount)
			*pu32Skipped = 1;
	}

	return CI_SOK;
}
static int InitDecoder()
{

#if defined(_DEBUG) && defined(WIN32) && !defined(_WIN32_WCE)
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag); 	
#endif

	memset(&OpenOptions, 0, sizeof(OpenOptions));
	OpenOptions.u32Size = sizeof(OpenOptions);
	OpenOptions.u32DoQuickOut = 0;
	OpenOptions.u32DisplayQueueSize	= 1;
	//OpenOptions.DispQueue.pDispQueueLock = &(DispQueueLock);
	OpenOptions.DispQueue.pfnCreateLock		= PFN_InitLock_IMPL;
	OpenOptions.DispQueue.pfnDestroyLock	= PFN_DeleteLock_IMPL;
	OpenOptions.DispQueue.pfnLock			= PFN_Lock_IMPL;
	OpenOptions.DispQueue.pfnUnlock			= PFN_UnLock_IMPL;
	OpenOptions.pfnCheckSkippedFrame		= CI_CheckSkippedFrame;

#ifdef SW_MODE	
	OpenOptions.pfn_malloc = temp_malloc;
	OpenOptions.pfn_free = temp_free;
#else
//	OpenOptions.pfn_malloc = STMalloc;
//	OpenOptions.pfn_free = STFree;
#endif
	DecodeOptions.u32Size = sizeof(DecodeOptions);
	GetFrameOptions.u32Size = sizeof(GetFrameOptions);
	Frame.u32Size = sizeof(Frame);
	memset(&DecodeOptions, 0, sizeof(DecodeOptions));
	memset(&GetFrameOptions, 0, sizeof(GetFrameOptions));
	memset(&Frame, 0, sizeof(Frame));
	
	ret = StreamInitBuffer(&StreamBuffer, &StreamCurPos, &BufLen);
	if (ret != 0) {
		printf("Fail to initialize stream buffer! \n");
		fprintf(fpLog, "Fail to initialize stream buffer! \n");
	}
//#define ACCESS_UNIT
#define PUSH_MODE
//#define PULL_MODE

#if defined(PUSH_MODE)
	wchar_t pBuf[MAX_PATH];

	//DWORD tick = GetTickCount();
	StreamReloadBuffer(StreamBuffer, &StreamCurPos, &BufLen, pInFile);
	//swprintf(pBuf,_T("reload cost:%d \n"), GetTickCount() - tick );
	//OutputDebugString(pBuf);

	CI_H264BPDEC_ParseInfo(NULL, StreamBuffer, BufLen, 0, NULL, 0, &confidence, &StreamInfo);
#endif

	CI_H264BPDEC_Create(&Decoder, NULL);
	if(Decoder == NULL)
	{
		printf("Fail to open the decoder! \n");
		fprintf(fpLog, "Fail to open the decoder! \n");
		return -1;
	}

	ret = CI_H264BPDEC_Open(Decoder, &OpenOptions);
	if(CI_SOK != ret)
	{
		printf("Fail to open the decoder! \n");
		fprintf(fpLog, "Fail to open the decoder! \n");
		CI_H264BPDEC_Release(Decoder);
		return -1;
	}

	dwTotalDecodedFrames = 0;
	return 0;

}

static int UnInitDecoder()
{
	CI_U32 iResetFlag  = 0; 
	//iResetFlag |= CI_RESET_CLEARSTATE;
	// Reset or Stop, flush out all the frames
	//CI_H264BPDEC_Reset(Decoder, iResetFlag);

	for (;;)
	{
		if(CI_SOK != CI_H264BPDEC_GetFrame(Decoder, &GetFrameOptions, &Frame))
			break;

		dwTotalDecodedFrames++;

	//	printf("Frame# %d, TimeScale# %d, TimeStamp# %u\n", dwTotalDecodedFrames, Frame.Pts.u32TimeScale, Frame.Pts.u64Timestamp);

		if(CI_SOK != CI_H264BPDEC_ReleaseFrame(Decoder, &Frame))
			break;
	}
	StreamFreeBuffer(StreamBuffer);

	CI_H264BPDEC_Close(Decoder);
	CI_H264BPDEC_Release(Decoder);

	fclose(pInFile);

	if (fpOutput)
		fclose(fpOutput);
	if (fpLog)
		fclose(fpLog);

#if defined(_DEBUG) && defined(WIN32) && !defined(_WIN32_WCE)
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}

static int tick = 0;
static int tick2 = 0;
static long duration2 ;
static int DecoderRenderLoop()
{

	CI_U32 iConsumedBytes = 0, iDecodedFrame = 0, iSkippedFrame = 0, iStatus = 0;
	DecodeOptions.u32DecodeFlag = 0;
	wchar_t pBuf[MAX_PATH];

	g_frameCount = 0;
	g_startTick = GetTickCount();
	DWORD decTick = 0;
	tick = GetTickCount();
	tick2 = 0;

	for(i=0;;)
	{
		int framelen = 0;

		framelen = BufLen - (StreamCurPos - StreamBuffer);

#if defined(ACCESS_UNIT)
		ret = CI_H264BPDEC_ParseAccessUnit(0, StreamCurPos, framelen, 
			0, 0, &iConsumedBytes, &iDecodedFrame, 0, 0);
		if(ret == CI_SOK)
		{
			printf("Frame# %d, Offset# %d, Len# %d\n", dwTotalDecodedFrames, iConsumedBytes, iDecodedFrame);
			StreamCurPos += (iConsumedBytes + iDecodedFrame);
			dwTotalDecodedFrames++;
		}
		else
		{
			//DWORD tick = GetTickCount();
			ret = StreamReloadBuffer(StreamBuffer, &StreamCurPos, &BufLen, pInFile);
			//swprintf(pBuf,_T("reload cost:%d \n"), GetTickCount() - tick );
			//OutputDebugString(pBuf);
			if(ret == -2)
			{
				if(DecodeOptions.u32DecodeFlag & CI_VIDEC_DECODEFLAG_ENDOFSTREAM)
					break;
				else
					DecodeOptions.u32DecodeFlag |= CI_VIDEC_DECODEFLAG_ENDOFSTREAM;
			}
		}
#else

#if defined(PUSH_MODE)
		DecodeOptions.Pts.u8Valid = 1;
		DecodeOptions.Pts.u64Timestamp = decode_count;
		DecodeOptions.Pts.u32TimeScale = decode_count;
		time1 = GetTickCount();
		ret = CI_H264BPDEC_DecodeBuffer(Decoder, &DecodeOptions, StreamCurPos, framelen, &iConsumedBytes, &iDecodedFrame, &iSkippedFrame, &iStatus);
		sum_time += GetTickCount() - time1;
		decode_count++;
		framelen -= iConsumedBytes;
		StreamCurPos += iConsumedBytes;

		if(ret != CI_EOUTBUFTOOSMALL)
		{
			//DWORD tick = GetTickCount();
			ret = StreamReloadBuffer(StreamBuffer, &StreamCurPos, &BufLen, pInFile);
			//swprintf(pBuf,_T("reload cost:%d \n"), GetTickCount() - tick );
			//OutputDebugString(pBuf);
			if(ret == -2)
			{
				if(DecodeOptions.u32DecodeFlag & CI_VIDEC_DECODEFLAG_ENDOFSTREAM)
					break;
				else
					DecodeOptions.u32DecodeFlag |= CI_VIDEC_DECODEFLAG_ENDOFSTREAM;
			}
		}

		if(CI_EOUTOFMEMORY == ret || CI_EFAIL == ret)
			break;
#endif

#if defined(PULL_MODE)
		DecodeOptions.pGetDataContext = (CI_VOID *)pInFile;
		DecodeOptions.pfnGetData = GetData;

		ret = CI_H264BPDEC_DecodeFrame(Decoder, &DecodeOptions, &iDecodedFrame, &iSkippedFrame, &iStatus);

		if(CI_EOUTBUFTOOSMALL != ret)
			break;
#endif

		for (;;)
		{
			if(CI_SOK != CI_H264BPDEC_GetFrame(Decoder, &GetFrameOptions, &Frame))
				break;

			dwTotalDecodedFrames++;
			RenderFrame[g_WriteCnt] = Frame;

			g_WriteCnt++;
			if(g_WriteCnt>=QUEUE_SIZE)
				g_WriteCnt = 0;

			ReleaseSemaphore( g_event, 1 ,NULL);

		}
		Sleep(1);

#endif
	}
	//DWORD duration = GetTickCount() - g_startTick;
	//swprintf(pBuf,_T("fps:%f \n"), (g_frameCount*1000/(duration)) );
	//OutputDebugString(pBuf);
	return 0;
}

static int InitFile()
{
	//LPCTSTR  szFilter( 
	//	TEXT("All Files(*.*)|*.*|")
	//	TEXT("|")
	//	);
	//CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_EXPLORER, szFilter);
	//if (FileDlg.DoModal() != IDOK )
	//{
	//	MessageBox(NULL,_T("Can't open input file!\n"), _T("Error"), MB_OK);
	//	return E_FAIL;
	//}

	//_tcscpy(tc_path, FileDlg.GetPathName());

#ifdef stream800
	_stprintf(tc_path,_T("\\Storage Card2\\KongFu_Dream.cd1.pdahd.800x480.h264"));
#else
	_stprintf(tc_path,_T("\\Storage Card2\\ANB_0.h264"));
#endif	
	pInFile = _tfopen(tc_path, _T("rb"));
	if(pInFile == NULL)
	{
		MessageBox(NULL,_T("Can't open input file!\n"), _T("Error"), MB_OK);
		return E_FAIL;
	}

	return 0;
}

static int UninitFile()
{
	if(pInFile)
	{
		fclose(pInFile);
		pInFile = NULL;
	}
	return 0;
}

static int InitRender()
{

	WNDCLASS                    wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;//hInstance;
	wc.hIcon = NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = NAME;
	RegisterClass(&wc);

/*
    g_hWnd = CreateWindowEx(WS_EX_TOPMOST,
                          NAME,
                          TITLE,
                          WS_POPUP,
                          0,
                          0,
                          GetSystemMetrics(SM_CXSCREEN),
                          GetSystemMetrics(SM_CYSCREEN),
                          NULL,
                          NULL,
                          NULL,//hInstance,
                          NULL);
    if (!g_hWnd)
        return FALSE;

	MoveWindow(g_hWnd, 0,0,800,480,true);
	ShowWindow(g_hWnd,1);
*/

	// Get Tuners folder path
	GetModuleFileName(NULL, DirPath, MAX_PATH);
	TCHAR*	p = _tcsrchr(DirPath, '\\');
	p++;
	p[0] = 0;
	_tcscpy(FilePath, DirPath);
	_tcscat(FilePath, _T("LibVRPresent.dll"));

	HMODULE h_dll;
	h_dll = LoadLibrary(FilePath); 
	if(h_dll == NULL)
	{
		MessageBox(NULL,_T("Failed to load library \n"), _T("Error"), MB_OK);
		return E_FAIL;
	}
	fpCreateVideoPresenter fpProcCreate;

	fpProcCreate = (fpCreateVideoPresenter)GetProcAddress(h_dll, _T("CreateVideoPresenter"));
	if(fpProcCreate==0)
	{
		MessageBox(NULL,_T("Entry function is not found \n"), _T("Error"), MB_OK);
		return E_FAIL;
	}
	g_pPresent = fpProcCreate();
	if(g_pPresent==0)
	{
		MessageBox(NULL,_T("Can't create Video Presenter!\n"), _T("Error"), MB_OK);
		return E_FAIL;
	}

	
	VP_OpenOptions sInit;
	ZeroMemory(&sInit, sizeof(sInit));
	sInit.dwWidth = m_vSourceWidth;
	sInit.dwHeight= m_vSourceHeight;
	sInit.hWnd = /*g_hWnd*/0;
	sInit.dwFourCC = MAKE_FOURCC('Y','V','1','2');

	if(g_pPresent->Open(&sInit,sizeof(sInit)))
		return E_FAIL;

	return 0;

}

static int UninitRender()
{
	if(g_pPresent)
	{
		g_pPresent->Close();
		g_pPresent->Release();
		g_pPresent = NULL;
	}
	
	if(g_hWnd)
	{
		
		g_hWnd = 0;
	}
	return 0;
}

