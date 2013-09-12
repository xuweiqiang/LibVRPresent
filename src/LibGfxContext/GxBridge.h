//=============================================================================
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO INTERVIDEO, INC.
// 	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//	COMPANY.
//
// 	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// 	PURPOSE.
//
// 	Copyright (c) 2004 - 2005  InterVideo Corporation.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#ifndef _GXBRIDGE_H_
#define _GXBRIDGE_H_

#include "gx.h"
#include "McCom.h"

#ifndef MAKE_FOURCC
#define MAKE_FOURCC(c1,c2,c3,c4) ((c4)<<24 | (c3)<<16 | (c2)<<8 | (c1))
#endif

// ENGINE CODE
#define ESC_QUERYESCSUPPORT	8
#define GETGXINFO			0x00020000

typedef struct GXDeviceInfo 
{
	long Version;           //00
	void * pvFrameBuffer;   //04
	unsigned long cbStride; //08
	unsigned long cxWidth;  //0c
	unsigned long cyHeight; //10
	unsigned long cBPP;     //14
	unsigned long ffFormat; //18
	char Unused[0x84-7*4];
} GXDeviceInfo;

class GxBridge : 
	public IGraphicsContext, 
	public CMcUnknown
{
public:
	DECLARE_IUNKNOWN

	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, LPVOID *ppvObj);

	// IKsPropertySet
	STDMETHODIMP QuerySupported(REFGUID guidPropSet, DWORD dwPropID, DWORD *pTypeSupport);
	STDMETHODIMP Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned);
	STDMETHODIMP Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData);
	STDMETHODIMP Open(GFX_OPEN_OPTIONS *pOptions, DWORD dwSize);
	STDMETHODIMP Close();
	STDMETHODIMP BeginDraw(void ** pFrameBuffer,bool flip);
	STDMETHODIMP BeginDraw(void ** pY, void ** pU, void ** pV, int * pitch, RECT *rtSrcView, RECT *rtDstView, int rot);
	STDMETHODIMP EndDraw();
	STDMETHODIMP EndDraw(bool flip);
	STDMETHODIMP GetDisplayProperties();
	STDMETHODIMP GetDefaultKeys(int iOptions);
	STDMETHODIMP SetFocus(BOOL bFocusOn);
	STDMETHODIMP SetViewport( RECT *rtView );
	STDMETHODIMP GetGuiKey( DWORD dwIn, DWORD * pOut );
	STDMETHODIMP GetGraphicsProperties( GraphicsProperties * pGraphicsProperties );

	GxBridge();
	~GxBridge();
protected:
	STDMETHODIMP OpenDisplay(HWND hWnd);
	STDMETHODIMP CloseDisplay();
	STDMETHODIMP OpenInput();
	STDMETHODIMP CloseInput();

	HWND				m_hWnd;
	GXKeyList			m_KeyList;
	GXDisplayProperties m_Properties;
	CRITICAL_SECTION	m_csDraw;
	LPVOID				m_pBuffer;
	BOOL				m_bExclusiveMode;
	DWORD				m_dwLockers;
	char *				m_pFrameBuffer;
	int					m_rotation;
	RawFrameBufferInfo m_RawFrameBufferInfo;

#ifdef HARDWARE_2700G
	int m_src_x; 
	int m_src_y; 
	int m_src_width;
	int m_src_height; 
	int m_dst_x;
	int m_dst_y; 
	int m_dst_width;
	int m_dst_height; 
	int m_rot;
	int m_enable_2700g;
#endif
};

#endif
