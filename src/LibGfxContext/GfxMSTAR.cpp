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

#include "../../inc/GfxBuildDef.h"


#ifdef GFX_MSTAR
#include <windows.h>
#include "GfxMSTAR.h"
#include "../../inc/dp.h"



#define YUV420_BUF_SIZE		1*1024*1024
#define YUV422_BUF_SIZE		1*1024*1024

#define YUV422_SIZE		512*1024
#define YUV420_SIZE		512*1024


#define YUV422_BUF0_START		0x27D00000
#define YUV422_BUF1_START		(YUV422_BUF0_START+YUV422_SIZE)
#define YUV420_BUF0_START		(YUV422_BUF1_START+YUV422_SIZE)
#define YUV420_BUF1_START		(YUV420_BUF0_START+YUV420_SIZE)

#define DBG_INIT		1
#define DBG_DEINIT	1
#define DBG_ERR		1
#define DBG_ANALYSE_MSG	0
#define DBG_DUMP_MSG	0
#define DBG_ARGO_MSG 1


#ifdef MSTAR2521_JTD
#define USING_420
#endif

typedef struct {
	void*    pvDestMem;
	DWORD    dwPhysAddr;
	DWORD    dwSize;
	DWORD    dwFlag;
} VIRTUAL_COPY_EX_DATA;

CGfxMSTAR::CGfxMSTAR()
{
	m_bOpen = FALSE;
}

CGfxMSTAR::~CGfxMSTAR()
{
	Close();
}

STDMETHODIMP CGfxMSTAR::Open(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers, 
		const DWORD *pdwFourCC, IMcPreInit* pPrepareData, RECT* pDst)		
{
  HRESULT ret = E_FAIL;
  if(dwWidth==0 || dwHeight==0)
  return E_FAIL;
  if(m_bOpen) return E_UNEXPECTED;

  m_dwWidth = dwWidth;
  m_dwHeight = dwHeight;
  SetRect(&m_rectSrc,0,0,m_dwWidth,m_dwHeight);
  ZeroMemory(&m_rectSrc,sizeof(m_rectSrc));
  ZeroMemory(&m_rectDst,sizeof(m_rectDst));

  m_dwBackBuffers = 2;
  m_dwColorKey = 0xffffff;
  m_eBufIdx = TYPE420BUF0;
  m_bColorKeyEn = 1;
  m_bShow = false;

  AtscInit();


if(FAILED(UpdateScaler()))
	return E_FAIL;
	

  m_bOpen = TRUE;
  return S_OK;
}

STDMETHODIMP CGfxMSTAR::Close()
{
    DWORD	dwBufOut,dwActualOut;
    DWORD	dwBufIn = 0;

	if(!m_bOpen) return E_UNEXPECTED;


	memset(m_pYuv420Buf0, 0x0,m_dwWidth*m_dwHeight);
	memset((unsigned char*)m_pYuv420Buf0 + m_dwWidth*m_dwHeight, 0x80,m_dwWidth*m_dwHeight/2);
	Flip(0,0);

#ifdef MSTAR2531	

//Clear Screen 
   //Command 1
	COLOR_KEY_st	stColorKey;
	memset(&stColorKey, 0 ,sizeof(stColorKey));
	stColorKey.u8FlagEn = 0;
	
	DeviceIoControl(m_hAtscTv, IOCTL_JPD_COLOR_KEY,   &stColorKey,
		sizeof(COLOR_KEY_st),	&dwBufOut,	sizeof(DWORD),	&dwActualOut,NULL);

   //Command 2
    	dwBufIn	= 0x3f;
	DeviceIoControl(m_hAtscTv,  IOCTL_JPD_SET_ALPHA,   &dwBufIn,
					sizeof(DWORD),		&dwBufOut,	sizeof(DWORD),	&dwActualOut,NULL);
					
   //Command 3
	dwBufIn = 0;
	DeviceIoControl(m_hAtscTv,  IOCTL_JPD_SW2GOP,   &dwBufIn,
					sizeof(DWORD),		&dwBufOut,	sizeof(DWORD),	&dwActualOut,NULL);

//Clear AVIN
	Scaler_SetWindow_st pWinInfo;
	pWinInfo.bInterace = FALSE;//TRUE;
	pWinInfo.bMEM_422 = TRUE;
	pWinInfo.u16SrcRect_X = 0;
	pWinInfo.u16SrcRect_Y = 0;
	pWinInfo.u16SrcRect_Width = 720;
	pWinInfo.u16SrcRect_Height = 480;
	pWinInfo.u16Src_Width = 720;
	pWinInfo.u16Src_Height = 480;
	pWinInfo.u16DispRect_X = 20;
	pWinInfo.u16DispRect_Y = 10;
	pWinInfo.u16DispRect_Width = (GetSystemMetrics(SM_CXSCREEN)-pWinInfo.u16DispRect_X*2);//760;
	pWinInfo.u16DispRect_Height = (GetSystemMetrics(SM_CYSCREEN)-pWinInfo.u16DispRect_Y*2);//460;
	pWinInfo.SrcType = SCALER_SRC_MAX;

	DeviceIoControl (m_hAtscTv, IOCTL_SCALER_SETWINDOW,
		&pWinInfo, sizeof(Scaler_SetWindow_st),
		NULL, 0, &dwActualOut, 0);

#endif

    if(DeviceIoControl(m_hAtscTv,       IOCTL_JPD_PDOWN_ARGO,    &dwBufIn,
      	sizeof(DWORD),		&dwBufOut,		sizeof(DWORD),		&dwActualOut,						NULL))
    {
      DP("::ARGO pull down power enable pin!\r\n");
    }
    else
    {
      DP("::ARGO OnBtnExitMpifTest!\r\n");
    }

    ATSC_Denit();

  	m_bOpen = FALSE;
	return S_OK;
}

STDMETHODIMP CGfxMSTAR::SetSrcRect(const RECT *pRect)
{
	if(EqualRect(pRect,&m_rectSrc))
		return S_OK;
	m_rectSrc = *pRect;
	return Update();
}

STDMETHODIMP CGfxMSTAR::SetDstRect(const RECT *pRect)
{
	RECT pInput = *pRect;
	pInput.left &= (~(4-1));
	pInput.top &= (~(4-1));
	pInput.right &= (~(4-1));
	pInput.bottom &= (~(4-1));

	if(EqualRect(&pInput,&m_rectDst))
		return S_OK;

	#ifdef MSTAR2531	
	Sleep(50);	//Fix issue that video flash
	#endif
	m_rectDst = pInput;

	Scaler_SetWindow_st pWinInfo;

	pWinInfo.SrcType = SCALER_SRC_BT656;
	pWinInfo.bInterace = FALSE;//TRUE;
	pWinInfo.bMEM_422 = TRUE;
	pWinInfo.u16SrcRect_X = (U16)0;
	pWinInfo.u16SrcRect_Y = (U16)0;
	pWinInfo.u16SrcRect_Width = (U16)m_dwWidth;
	pWinInfo.u16SrcRect_Height = (U16)m_dwHeight;
	pWinInfo.u16Src_Width = (U16)m_dwWidth;
	pWinInfo.u16Src_Height = (U16)m_dwHeight;
	
	pWinInfo.u16DispRect_X = (U16)m_rectDst.left;		
	pWinInfo.u16DispRect_Y = (U16)m_rectDst.top;
	pWinInfo.u16DispRect_Width = (U16)(m_rectDst.right - m_rectDst.left);
	pWinInfo.u16DispRect_Height = (U16)(m_rectDst.bottom - m_rectDst.top);

	DWORD	dwBytesReturned = 0;
	DeviceIoControl (m_hAtscTv, IOCTL_SCALER_SETWINDOW_CMMB,
				&pWinInfo, sizeof(Scaler_SetWindow_st),
				NULL, 0, &dwBytesReturned, 0);




	DWORD	dwBufOut,dwActualOut;

	COLOR_KEY_st	stColorKey;

	stColorKey.u32colorkey = 0x00080008;
	stColorKey.u8FlagEn = 1;

	if(DeviceIoControl(m_hAtscTv,
		           IOCTL_JPD_COLOR_KEY,
				    &stColorKey,
					sizeof(COLOR_KEY_st),
					&dwBufOut,
					sizeof(DWORD),
					&dwActualOut,
					NULL))
		{
		DP("::ARGO Scaler set ColorKey success!\r\n");
		}
	else
		{
		DP("::ARGO Scaler set ColorKey fail!\r\n");
		}	
		
	return S_OK;
}

STDMETHODIMP CGfxMSTAR::SetScnRect(const RECT *pRect)
{
	return S_OK;
}


STDMETHODIMP CGfxMSTAR::GetScnRect(RECT *pRect)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxMSTAR::SetWindowHandle(HWND hwnd)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxMSTAR::GetWindowHandle(HWND& hwnd)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxMSTAR::SetAlpha(DWORD dwAlpha)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxMSTAR::SetSrcColorKey(BOOL bEnable, DWORD dwSrcCkey)
{
	return S_OK;
}

STDMETHODIMP CGfxMSTAR::SetDstColorKey(BOOL bEnable, DWORD dwDstCkey)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxMSTAR::GetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxMSTAR::GetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxMSTAR::SetColorControl(LONG num, DDCOLORCONTROL *pCC)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxMSTAR::SetGammaControl(LONG num, DDGAMMARAMP *pGR)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxMSTAR::Lock(LONG num, LPVOID *ppBuf, LONG *pStride, DWORD *pPixCount)
{
  if(num >= (LONG)m_dwBackBuffers)
    return E_FAIL;


#ifdef USING_420
	if((num == 0) && (ppBuf))
		*ppBuf = m_pYuv420Buf0;
	else
		*ppBuf = m_pYuv420Buf1;
#else
	if((num == 0) && (ppBuf))
		*ppBuf = m_pYuv422Buf0;
	else
		*ppBuf = m_pYuv422Buf1;
#endif
  	
#ifdef USING_420
	if(pStride)
		*pStride = m_dwWidth;
#else
	if(pStride)
		*pStride = m_dwWidth*2;
#endif

	return S_OK;
}

STDMETHODIMP CGfxMSTAR::Unlock(LONG num)
{
  if(num >= (LONG)m_dwBackBuffers)
    return E_FAIL;
/*
if((num == 0) )
	memset((unsigned char*)m_pYuv422Buf0, 0 , m_dwWidth*m_dwHeight*2);
else
	memset((unsigned char*)m_pYuv422Buf1, 0 , m_dwWidth*m_dwHeight*2);
*/
	return S_OK;
}

STDMETHODIMP CGfxMSTAR::Flip(LONG num, DWORD dwFlags)
{
  if(num >= (LONG)m_dwBackBuffers)
    return E_FAIL;


#ifdef USING_420
	if(num == 0)
		m_eBufIdx = TYPE420BUF0;
	else
		m_eBufIdx = TYPE420BUF1;
#else
	if(num == 0)
		m_eBufIdx = TYPE422BUF0;
	else
		m_eBufIdx = TYPE422BUF1;
#endif
/*
	DWORD  dwBufOut,dwActualOut;
	DWORD   dwBufIn = m_eBufIdx;

	DeviceIoControl(m_hAtscTv,IOCTL_JPD_SCL_FLUSH,
		&dwBufIn,sizeof(DWORD),
		&dwBufOut,sizeof(DWORD),
		&dwActualOut, NULL);
*/

  //Find issue that flush rate is slow

	SetEventData(hAtscEvent, m_eBufIdx);
	SetEvent(hAtscEvent);

  return S_OK;
}

STDMETHODIMP CGfxMSTAR::Show(BOOL bShow)
{
	if(m_bShow==bShow)
		return S_OK;


	DWORD	dwBufOut,dwActualOut,dwBufIn;

    	dwBufIn	= 0x3f;
	DeviceIoControl(m_hAtscTv,  IOCTL_JPD_SET_ALPHA,   &dwBufIn,
					sizeof(DWORD),		&dwBufOut,	sizeof(DWORD),	&dwActualOut,NULL);

	m_bShow = bShow;
	return S_OK;
}

STDMETHODIMP CGfxMSTAR::Update()
{
	return S_OK;
}

STDMETHODIMP CGfxMSTAR::GetDDDevice(LPVOID *pDevice, DWORD *pdwSize)
{
	return E_NOTIMPL;
}

STDMETHODIMP CGfxMSTAR::GetSurfaceInfo(GfxOverlayInfo *pInfo)
{
	if(pInfo==0)
		return E_FAIL;
	if(pInfo->dwSize!=sizeof(*pInfo))
		return E_FAIL;

	pInfo->dwWidth = m_dwWidth;
	pInfo->dwHeight = m_dwHeight;
	pInfo->dwBackBuffers = m_dwBackBuffers;
#ifdef USING_420
	pInfo->dwFourCC = MAKEFOURCC('Y','V','1','2');
#else
	pInfo->dwFourCC = MAKEFOURCC('U','Y','V','Y');
#endif
	pInfo->dwCaps = 0;
	return S_OK;
}

STDMETHODIMP CGfxMSTAR::IsOpen()
{
	return E_FAIL;
}

volatile LPVOID CGfxMSTAR::GetVirtualAddr(DWORD dwPhyBaseAddress, DWORD dwSize)
{
	volatile LPVOID pVirtual;
	VIRTUAL_COPY_EX_DATA vced;

	if(dwPhyBaseAddress&0xFFF)
	{
		return NULL;
	}
	pVirtual = VirtualAlloc(0,dwSize,MEM_RESERVE,PAGE_NOACCESS);
	vced.pvDestMem = pVirtual;
	vced.dwSize = dwSize;


#ifdef MSTAR2531
	vced.dwPhysAddr = dwPhyBaseAddress;
	vced.dwFlag = PAGE_READWRITE|PAGE_PHYSICAL|PAGE_NOCACHE;
#else
	vced.dwPhysAddr = dwPhyBaseAddress>>8;
#endif


	if(KernelIoControl(IOCTL_VIRTUAL_COPY_EX,&vced, sizeof(vced), NULL, NULL, NULL))
		return pVirtual;
	else
		return NULL;
}

volatile LPVOID CGfxMSTAR::GetVirtualAddr(PHYSICAL_ADDRESS pa, DWORD size, BOOL cacheEnable)
{
    VOID *pAddress = NULL;
    ULONGLONG sourcePA;
    U32 sourceSize, offset;
    //BOOL rc;
    VIRTUAL_COPY_EX_DATA vced;

    // Page align source and adjust size to compensate
    sourcePA = pa.QuadPart & ~(PAGE_SIZE - 1);

    offset = pa.LowPart & (PAGE_SIZE - 1);
    sourceSize = size + offset;
    if (sourceSize < size || sourceSize < offset) {
        DP("ERROR: MmMapIoSpace size overflow\r\n");
        goto CLEANUP;
    }
    pAddress = VirtualAlloc(0, sourceSize, MEM_RESERVE, PAGE_NOACCESS);
    if (pAddress != NULL)
    {

#ifdef MSTAR2531
	vced.dwPhysAddr = (DWORD)(pa.LowPart);
	vced.dwFlag = PAGE_READWRITE|PAGE_PHYSICAL|PAGE_NOCACHE;
#else
	vced.dwPhysAddr = (DWORD)(sourcePA >> 8);
#endif
	vced.pvDestMem = pAddress;
	vced.dwSize = sourceSize;

        if (KernelIoControl(IOCTL_VIRTUAL_COPY_EX,&vced, sizeof(vced), NULL, NULL, NULL))
            pAddress = (VOID*)((DWORD)pAddress + (pa.LowPart & (PAGE_SIZE - 1)));
        else
        {
            VirtualFree(pAddress, 0, MEM_RELEASE);
            pAddress = NULL;
        }
    }

CLEANUP:
    return pAddress;
}
void CGfxMSTAR::CloseFile(void)
{
	if (m_hAtscTv != NULL)
	{
		CloseHandle(m_hAtscTv);
		m_hAtscTv = NULL;
		DP("::::ARGO CloseFile!\r\n");
	}
}

BOOL CGfxMSTAR::OpenDevice(LPCTSTR lpDeviceName)
{
	CloseFile();

	m_hAtscTv= CreateFile(
						lpDeviceName, 				//LPCTSTR lpFileName,
						GENERIC_READ|GENERIC_WRITE, //DWORD dwDesiredAccess,
						0,							//DWORD dwShareMode,
						NULL, 						//LPSECURITY_ATTRIBUTES lpSecurityAttributes,
						OPEN_EXISTING, 				//DWORD dwCreationDispostion,
						FILE_ATTRIBUTE_NORMAL, 		//DWORD dwFlagsAndAttributes,
						NULL 						//HANDLE hTemplateFile
					);
	if(m_hAtscTv == INVALID_HANDLE_VALUE)
	{
		m_hAtscTv = NULL;
		return FALSE;
	}

	return TRUE;
}


BOOL CGfxMSTAR::AtscInit(void)
{
	hAtscEvent =  CreateEvent(NULL, FALSE, FALSE, TEXT("AtscNotify"));

	if(OpenDevice(_T("SCA0:")))
	{
		{
			DWORD	dwActualOut,dwBufOut;

			if(DeviceIoControl(m_hAtscTv,
				           IOCTL_JPD_SET_BUFF,
						    NULL, 			//NULL=7*1024*1024,else = actual size.
							0,
							&dwBufOut,
							sizeof(DWORD),
							&dwActualOut,
							NULL))
			{
				DP("[app]::ARGO get yuv buf success,MEM pointer is 0x%x!\r\n",dwBufOut);

#if (_WIN32_WCE == 0x600)
				m_pYuv422Buf0 = GetVirtualAddr(dwBufOut,YUV422_SIZE);
				m_pYuv422Buf1 = GetVirtualAddr(dwBufOut+YUV422_SIZE,YUV422_SIZE);
				m_pYuv420Buf0 = GetVirtualAddr(dwBufOut+YUV422_BUF_SIZE,YUV420_SIZE);
				m_pYuv420Buf1 = GetVirtualAddr(dwBufOut+YUV422_BUF_SIZE+YUV420_SIZE,YUV420_SIZE);

#else

                          m_pYuv422Buf0 = VirtualAlloc(0,YUV422_BUF_SIZE/2, MEM_RESERVE, PAGE_NOACCESS);
                          m_pYuv422Buf1 = VirtualAlloc(0,YUV422_BUF_SIZE/2, MEM_RESERVE, PAGE_NOACCESS);
                          m_pYuv420Buf0 = VirtualAlloc(0,YUV420_BUF_SIZE/2, MEM_RESERVE, PAGE_NOACCESS);
                          m_pYuv420Buf1 = VirtualAlloc(0,YUV420_BUF_SIZE/2, MEM_RESERVE, PAGE_NOACCESS);


				if(m_pYuv422Buf0 != NULL)
				{
					if(VirtualCopy((PVOID)m_pYuv422Buf0, (PVOID)(dwBufOut >> 8),
						YUV422_BUF_SIZE/2, PAGE_PHYSICAL|PAGE_READWRITE|PAGE_NOCACHE ))

					DP("::ARGO Init success VirtualAlloc	m_pYuv422Buf0=%x!!!\r\n",m_pYuv422Buf0);
				}

				if(m_pYuv422Buf1 != NULL)
				{
					if(VirtualCopy((PVOID)m_pYuv422Buf1, (PVOID)((dwBufOut + YUV422_BUF_SIZE/2) >>
						8), YUV422_BUF_SIZE/2, PAGE_PHYSICAL|PAGE_READWRITE|PAGE_NOCACHE ))


					DP("::ARGO Init success VirtualAlloc m_pYuv422Buf1=%x!!!\r\n",m_pYuv422Buf1);
				}

				//1.map yuv420 buffer,

				if(m_pYuv420Buf0 != NULL)
				{
					if(VirtualCopy((PVOID)m_pYuv420Buf0, (PVOID)((dwBufOut + YUV422_BUF_SIZE) >>
						8), YUV420_BUF_SIZE/2, PAGE_PHYSICAL|PAGE_READWRITE|PAGE_NOCACHE ))

					DP("::ARGO Init success VirtualAlloc m_pYuv420Buf0=%x!!!\r\n",m_pYuv420Buf0);
				}

				if(m_pYuv420Buf1 != NULL)
				{
					if(VirtualCopy((PVOID)m_pYuv420Buf1, (PVOID)((dwBufOut + YUV422_BUF_SIZE +
						YUV420_BUF_SIZE/2) >> 8), YUV420_BUF_SIZE/2,
						PAGE_PHYSICAL|PAGE_READWRITE|PAGE_NOCACHE ))

					DP("::ARGO Init success VirtualAlloc	m_pYuv420Buf1=%x!!!\r\n",m_pYuv420Buf1);
				}
#endif
			}
			else
			{
#if (_WIN32_WCE == 0x600)
				m_pYuv422Buf0 = GetVirtualAddr(YUV422_BUF0_START,YUV422_SIZE);
				m_pYuv422Buf1 = GetVirtualAddr(YUV422_BUF1_START,YUV422_SIZE);
				m_pYuv420Buf0 = GetVirtualAddr(YUV420_BUF0_START,YUV420_SIZE);
				m_pYuv420Buf1 = GetVirtualAddr(YUV420_BUF1_START,YUV420_SIZE);
#else
				return E_FAIL;
#endif
			}
	DP("Get m_pYuv422Buf0  0x%x!\r\n",m_pYuv422Buf0);
	DP("Get m_pYuv422Buf1 0x%x!\r\n",m_pYuv422Buf1);
	DP("Get m_pYuv420Buf0 0x%x!\r\n",m_pYuv420Buf0);
	DP("Get m_pYuv420Buf1 0x%x!\r\n",m_pYuv420Buf1);


	memset(m_pYuv420Buf0, 0x0,m_dwWidth*m_dwHeight);
	memset((unsigned char*)m_pYuv420Buf0 + m_dwWidth*m_dwHeight, 0x80,m_dwWidth*m_dwHeight/2);
	Flip(0,0);
	memset(m_pYuv420Buf1, 0x0,m_dwWidth*m_dwHeight);
	memset((unsigned char*)m_pYuv420Buf1 + m_dwWidth*m_dwHeight, 0x80,m_dwWidth*m_dwHeight/2);
	Flip(1,0);


#if (_WIN32_WCE == 0x600)          
          PHYSICAL_ADDRESS    sPMUPhysicalAddress = { COLUMBUS_BASE_REG_PMU_PA, 0 };
			g_pPMU = GetVirtualAddr(sPMUPhysicalAddress,sizeof(REG_PMU_st),FALSE);

			if(m_pYuv422Buf0 != NULL && m_pYuv422Buf1 != NULL && 
			m_pYuv420Buf0 != NULL && m_pYuv420Buf1 != NULL && g_pPMU != NULL)
			{
				//g_pPMURegs = (PREG_PMU_st)g_pPMU;
				DP("::ARGO get YUV buf success!\r\n");
				return TRUE;
			}
#else
			if(m_pYuv422Buf0 != NULL && m_pYuv422Buf1 != NULL && 
				m_pYuv420Buf0 != NULL && m_pYuv420Buf1 != NULL )
			{
				DP("::ARGO get YUV buf success!\r\n");
				return TRUE;
			}

#endif
		}
	}
	DP("::ARGO Init fasle (ERROR[0X%X])!!!\r\n",GetLastError());
	return FALSE;
}


BOOL CGfxMSTAR::ATSC_Denit(void)
{
  DP("::ARGO ATSC_Denit !\r\n");
  CloseFile();

  if(m_pYuv422Buf0 != NULL)
  {
    VirtualFree(m_pYuv422Buf0, 0, MEM_RELEASE);
    m_pYuv422Buf0 = NULL;
  }

  if(m_pYuv422Buf1!= NULL)
  {
    VirtualFree(m_pYuv422Buf1, 0, MEM_RELEASE);
    m_pYuv422Buf1 = NULL;
  }

  if(m_pYuv420Buf0!= NULL)
  {
    VirtualFree(m_pYuv420Buf0, 0, MEM_RELEASE);
    m_pYuv420Buf0 = NULL;
  }

  if(m_pYuv420Buf1!= NULL)
  {
    VirtualFree(m_pYuv420Buf1, 0, MEM_RELEASE);
    m_pYuv420Buf1 = NULL;
  }

  CloseHandle(hAtscEvent);
  hAtscEvent = 0;
  return TRUE;
}

HRESULT CGfxMSTAR::UpdateScaler()
{

  DWORD		dwBufIn = (m_dwHeight<<16) | (m_dwWidth);
  DWORD	dwBufOut,dwActualOut;


  if(DeviceIoControl(m_hAtscTv,  IOCTL_JPD_SCALER_SIZE,      &dwBufIn,  	sizeof(DWORD),
    	&dwBufOut,  	sizeof(DWORD),  	&dwActualOut,  	NULL))
  {
    DP("::ARGO set yuv image size success!\r\n");
  }
  else
  {
    DP("::ARGO set yuv image size fail!\r\n");
    return E_FAIL;
  }


  //1.SW to scaler path
  if(DeviceIoControl(m_hAtscTv,  IOCTL_JPD_SCALER_PATH,  NULL,  0,
  &dwBufOut,  sizeof(DWORD),  &dwActualOut,  NULL))
  {
    DP("::ARGO SW to Scaler path success!\r\n");
  }
  else
  {
    DP("::ARGO SW to Scaler path fail!\r\n");
    return E_FAIL;
  }

  dwBufIn = m_eBufIdx;

  //1.update yuv H&W size.
  if(DeviceIoControl(m_hAtscTv,     IOCTL_JPD_SW2SCALER,  &dwBufIn,
    sizeof(DWORD),  &dwBufOut,  sizeof(DWORD),  &dwActualOut,  NULL))
  {
    DP("::ARGO sw to scaler success!\r\n");
  }
  else
  {
    DP("::ARGO sw to scaler fail!\r\n");
    return E_FAIL;
  }

return S_OK;
}

IMcGfxOverlay *CreateOverlayMSTAR()
{
	return new CGfxMSTAR;
}
#endif
