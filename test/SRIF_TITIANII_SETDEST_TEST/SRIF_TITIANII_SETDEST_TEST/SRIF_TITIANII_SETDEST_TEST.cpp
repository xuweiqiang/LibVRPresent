// SRIF_TITIANII_SETDEST_TEST.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>

#include "RendererSDK.h"
#include "BHSurfMgr.h"

#pragma comment(lib, "BHVidRenderer.lib")
#pragma comment(lib, "BHSurfMgr.lib")

#define SRC_WIDTH	320
#define SRC_HEIGHT	240

#define BACKBUFFER_NUM	2

#define DST1_X 0
#define DST1_Y 0
#define DST1_W 320
#define DST1_H 240

#define DST2_X 0
#define DST2_Y 0
#define DST2_W 480
#define DST2_H 272

#define SCN_WIDTH 480
#define SCN_HEIGHT 272
RECT dstArray[]= {
	{DST1_X,DST1_Y,DST1_X+DST1_W, DST1_Y+DST1_H},
	{DST2_X,DST2_Y,DST2_X+DST2_W, DST2_Y+DST2_H}
};

typedef VR_FRAME_SURFACE *LPVR_FRAME_SURFACE;

int _tmain(int argc, _TCHAR* argv[])
{
	VR_HANDLE			m_pVr;
	HRESULT hr = S_OK;

	//CVR_CreateVideoRender
	hr = CVR_CreateVideoRender(&m_pVr, NULL);
	if(FAILED(hr))
		return -1;

	//CVR_SetRenderLayer
	hr = CVR_SetRenderLayer(m_pVr, VR_LAYER_OVERLAY,1);
	if(FAILED(hr))
		return -1;

	//CVR_SetDisplayMode
	VR_DISP_MODE_DESC mode;
	mode.dmode = VR_DISP_FULL;
	mode.rmode = VR_REN_NORMAL;
	mode.smode = VR_SCR_NORMAL;
	hr = CVR_SetDisplayMode(m_pVr, mode); 
	if(FAILED(hr))
		return -1;

	//CVR_SetVideoFormat

	int m_iYPitch = SRC_WIDTH;
	int m_iUVPitch = m_iYPitch>>1;

	VR_SRCRECT rect;
	rect.left= 0;
	rect.right = SRC_WIDTH;
	rect.top = 0;
	rect.bottom= SRC_HEIGHT;
	hr = CVR_SetVideoFormat(m_pVr, VR_FORMAT_I420, SRC_WIDTH, SRC_HEIGHT, &rect,  &m_iYPitch, &m_iUVPitch);
	if(FAILED(hr))
		return -1;

	//CVR_CreateSurface
	VR_FRAME_SURFACE	**m_pBackBuffer;
    m_pBackBuffer = new LPVR_FRAME_SURFACE[BACKBUFFER_NUM];
	if(m_pBackBuffer == NULL)
		return -1;
    ZeroMemory(m_pBackBuffer,sizeof(m_pBackBuffer[0])*BACKBUFFER_NUM);

	int i;
    for(i=0;i<(signed)BACKBUFFER_NUM;i++)
    {
	    hr = CVR_CreateSurface(m_pVr,&m_pBackBuffer[i],1);
	    if(hr!=VR_OK)
		    break;
    }
    if(i==0)
    {
	    delete[] m_pBackBuffer;
	    m_pBackBuffer = 0;
	    return -1;
    }

	//Clear screen
	for(int i=0;i<(signed)BACKBUFFER_NUM;i++)
	{
		void *pSurf = m_pBackBuffer[i];
		hr = CVR_LockSurface(m_pVr, (VR_FRAME_SURFACE*)pSurf);
	    if(hr!=VR_OK)
		    break;
		memset(((VR_FRAME_SURFACE*)pSurf)->pFrameBuf, 0x00, SRC_WIDTH*SRC_HEIGHT*3/2);
	    hr = CVR_UnlockSurface(m_pVr, (VR_FRAME_SURFACE*)pSurf);
	    if(hr!=VR_OK)
		    break;
 	}


	while(1)
	{
		for(int i=0;i<sizeof(dstArray)/sizeof(RECT);i++)
		{
			VR_DSTRECT			m_rectBltDst;
			m_rectBltDst.left = (float)dstArray[i].left/SCN_WIDTH;
			m_rectBltDst.right = (float)dstArray[i].right/SCN_WIDTH;
			m_rectBltDst.top = (float)dstArray[i].top/SCN_HEIGHT;
			m_rectBltDst.bottom = (float)dstArray[i].bottom/SCN_HEIGHT;
			hr = CVR_SetDstRect(m_pVr,m_rectBltDst);
			if(hr!=VR_OK)
				break;

			hr = CVR_SetVideoFormat(m_pVr, VR_FORMAT_I420, SRC_WIDTH, SRC_HEIGHT, &rect,  &m_iYPitch, &m_iUVPitch);
			if(hr!=VR_OK)
				break;

			hr = CVR_Render(m_pVr, (VR_FRAME_SURFACE*)m_pBackBuffer[0],VR_NOWAIT_VSYNC);
			if(hr!=VR_OK)
				break;
			Sleep(300);
		}

	}

	return 0;
}

