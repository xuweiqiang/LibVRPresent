//===================================== ========================================
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

#include <stdio.h>
#include <windows.h>
#include "../../inc/GfxPresent.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "tchar.h"

class CVRAnimation: 
	public IMcVideoPresenterAnimation,
	public CMcUnknown
{
public:
	CVRAnimation ();
	virtual ~CVRAnimation ();

	//IUNKNOWN
	DECLARE_IUNKNOWN
	STDMETHOD(NonDelegatingQueryInterface)(REFIID riid, LPVOID *ppvObj)
	{
		if(riid == IID_IMcVideoPresenterAnimation)
			return GetInterface(static_cast<IMcVideoPresenterAnimation*>(this), ppvObj);
		
		return CMcUnknown::NonDelegatingQueryInterface(riid, ppvObj);
	}

       STDMETHODIMP Open(IMcVideoPresenterStream* *ppStream) ;
       STDMETHODIMP Close() ;
	STDMETHODIMP Start(ANIMATION_PARAMS* pAnimation ,AnimationListener* pListener) ;
	STDMETHODIMP Stop() ;
	STDMETHODIMP IsRunning(bool *pbRunning,ANIMATION_PARAMS* pCurrAnimation) ;
	STDMETHODIMP Get(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned) ;
	STDMETHODIMP Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData) ;
	static DWORD WINAPI ThreadProc( LPVOID lpParameter);
	void ShakeEffect(INT frame, RECT *pSrc,RECT *pDst);

	bool m_bOpen;
	bool m_bRuning;
	ANIMATION_PARAMS m_animation;
	AnimationListener* m_pListener;
	HANDLE m_hThread;
	IMcVideoPresenterStream* m_pStream;
};

CVRAnimation ::CVRAnimation () :
	CMcUnknown(TEXT("Video Animation1.0"),0),
	m_bOpen(false),
	m_bRuning(false),
	m_pListener(NULL),
	m_pStream(NULL),
	m_hThread (NULL)
{
	memset(&m_animation, 0 ,sizeof(m_animation));
}

CVRAnimation ::~CVRAnimation()
{
}	

STDMETHODIMP CVRAnimation ::Open(IMcVideoPresenterStream* *ppStream)
{
	if(m_bOpen)	return E_UNEXPECTED;
	if(!ppStream || (*ppStream == 0))	return E_INVALIDARG;
	
	m_pStream  = *ppStream;
	m_bOpen = true;
	return S_OK;
}

STDMETHODIMP CVRAnimation ::Close() 
{
	if(!m_bOpen)	return E_UNEXPECTED;

	Stop();
	m_pStream  = NULL;
	m_bOpen = false;
	return S_OK;
}

STDMETHODIMP CVRAnimation ::Start(ANIMATION_PARAMS* pAnimation ,AnimationListener* pListener) 
{
	if(!pAnimation )	return E_INVALIDARG;
	if(!m_bOpen || m_animation.state == MC_ANIMATION_STATE_RUNNING)		return E_UNEXPECTED;
	memcpy(&m_animation, pAnimation, sizeof(m_animation));

	m_bRuning = true;
	if(!m_hThread)
		m_hThread = ::CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
	if(!m_hThread)
		return E_FAIL;
	::SetThreadPriority(m_hThread,THREAD_PRIORITY_ABOVE_NORMAL );

	m_pListener= 	pListener;
	return S_OK;
}

STDMETHODIMP CVRAnimation ::Stop() 
{
	if(!m_bOpen )	return E_UNEXPECTED;

	m_bRuning = false;
	m_pListener = NULL;
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	m_hThread = NULL;
	return S_OK;
}

STDMETHODIMP CVRAnimation ::IsRunning(bool *pbRunning,ANIMATION_PARAMS* pCurrAnimation) 
{

	if(m_animation.state != MC_ANIMATION_STATE_FINISH)
	{
		if(pbRunning)
			*pbRunning = true;

		if(pCurrAnimation)
			*pCurrAnimation = m_animation;
	}
	else
	{
		if(pbRunning)
			*pbRunning = false;

	}
	return S_OK;
}

STDMETHODIMP CVRAnimation ::Get(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData, OUT DWORD *pcbReturned) 
{
	return E_NOTIMPL;
}

STDMETHODIMP CVRAnimation ::Set(IN REFGUID guidPropSet, IN DWORD dwPropID, IN LPVOID pInstanceData, IN DWORD cbInstanceData, IN LPVOID pPropData, IN DWORD cbPropData) 
{
	return E_NOTIMPL;
}


DWORD WINAPI CVRAnimation ::ThreadProc( LPVOID lpParameter)
{
    CVRAnimation * pThis = (CVRAnimation *)lpParameter;
	RECT* m_pLineMatrix = NULL;
	int currIdx = 0;
	long step = 1;

	DWORD* alphaMatrix = NULL;
	int alphaId = 0;
	long alphaStep = 0;

    while(pThis->m_bRuning)
    	{
		if(pThis->m_animation.state == MC_ANIMATION_STATE_INIT)
		{
			pThis->m_animation.state = MC_ANIMATION_STATE_RUNNING;

		//Line
			if(pThis->m_animation.type & MC_ANIMATION_LINE)
			{
				pThis->m_animation.sLine.state = MC_ANIMATION_STATE_RUNNING;
				if(pThis->m_animation.sLine.repeatTimes)
				{
					long srcCenterX, srcCenterY;
					long dstCenterX, dstCenterY;
					srcCenterX = (pThis->m_animation.sLine.rSrc.left + pThis->m_animation.sLine.rSrc.right)/2;
					srcCenterY = (pThis->m_animation.sLine.rSrc.top + pThis->m_animation.sLine.rSrc.bottom )/2;
					dstCenterX = (pThis->m_animation.sLine.rDst.left + pThis->m_animation.sLine.rDst.right)/2;
					dstCenterY = (pThis->m_animation.sLine.rDst.top  + pThis->m_animation.sLine.rDst.bottom)/2;
					
					long srcWidth = pThis->m_animation.sLine.rSrc.right - pThis->m_animation.sLine.rSrc.left ;
					long srcHeight =  pThis->m_animation.sLine.rSrc.bottom  -  pThis->m_animation.sLine.rSrc.top;
					long dstWidth = pThis->m_animation.sLine.rDst.right - pThis->m_animation.sLine.rDst.left;
					long dstHeight = pThis->m_animation.sLine.rDst.bottom - pThis->m_animation.sLine.rDst.top;
		
					step = max(abs(srcCenterX - dstCenterX), abs(srcCenterY - dstCenterY));
Testagain:			if(pThis->m_animation.sLine.duration/step < 10)
					{
						step /= 2;
						if(step != 0)
							goto Testagain;
						else
							step = 10;
					}
					long i =0;
					m_pLineMatrix = (RECT*)malloc(sizeof(RECT)*(step+2));
					memset(m_pLineMatrix, 0,sizeof(RECT)*(step+2) );
					for(i=0;i<=step;i++)
					{
						SetRect(&m_pLineMatrix[i], 
							(srcCenterX + (dstCenterX- srcCenterX)*i/step )- (srcWidth + (dstWidth - srcWidth)*i/step)/2,
							(srcCenterY + (dstCenterY- srcCenterY)*i/step )- (srcHeight + (dstHeight - srcHeight)*i/step)/2,
							(srcCenterX + (dstCenterX- srcCenterX)*i/step ) + (srcWidth + (dstWidth - srcWidth)*i/step)/2,
							(srcCenterY + (dstCenterY- srcCenterY)*i/step )+ (srcHeight + (dstHeight - srcHeight)*i/step)/2);

//Scaling library needs 4 pixcels alignment
//						m_pLineMatrix[i].left &= (~(4-1));
//						m_pLineMatrix[i].top &= (~(4-1));
//						m_pLineMatrix[i].right &= (~(4-1));
//						m_pLineMatrix[i].bottom &= (~(4-1));
						
					}	
					currIdx  = 0;
				}
			}

		//shake	
			if(pThis->m_animation.type & MC_ANIMATION_SHAKE)
			{
				pThis->m_animation.sShake.state = MC_ANIMATION_STATE_RUNNING;
			}
			
		//alpha
			if(pThis->m_animation.type & MC_ANIMATION_ALPHA)
			{
				if(pThis->m_animation.sAlpha.srcAlpha == pThis->m_animation.sAlpha.dstAlpha)
					pThis->m_animation.sAlpha.dstAlpha = pThis->m_animation.sAlpha.dstAlpha -1;

				alphaStep = (long)abs((long)(pThis->m_animation.sAlpha.srcAlpha  - pThis->m_animation.sAlpha.dstAlpha));

AlphaTestagain:			if(pThis->m_animation.sAlpha.duration/alphaStep < 10)
					{
						step /= 2;
						if(step != 0)
							goto AlphaTestagain;
						else
							step = 10;
					}

				alphaMatrix = (DWORD*)malloc(sizeof(DWORD)*(alphaStep+1));
				memset(alphaMatrix , 0,sizeof(DWORD)*(alphaStep+1));
				alphaMatrix[alphaStep] = 512;
				int direct =(pThis->m_animation.sAlpha.dstAlpha >  pThis->m_animation.sAlpha.srcAlpha  ? 1:-1);
				for(long i=0;i<alphaStep;i++)
				{
					alphaMatrix[i] =  pThis->m_animation.sAlpha.srcAlpha  + direct*i;
				}	
				alphaId  = 0;

				pThis->m_animation.sAlpha.state = MC_ANIMATION_STATE_RUNNING;
			}
			
		}
		else if(pThis->m_animation.state == MC_ANIMATION_STATE_RUNNING)
		{
			bool bAniRunning = false;
			if(pThis->m_animation.sLine.state == MC_ANIMATION_STATE_RUNNING)
			{
				bAniRunning = true;
				if(pThis->m_animation.sLine.repeatTimes)
				{
					if((pThis->m_animation.sLine.duration/step == 0) || (GetTickCount() -pThis->m_animation.sLine.lastUpdateTime  >pThis->m_animation.sLine.duration/step))
					{
						if(m_pLineMatrix[currIdx].left == 0 &&m_pLineMatrix[currIdx].top == 0 &&
							m_pLineMatrix[currIdx].right == 0 &&m_pLineMatrix[currIdx].bottom == 0 )
						{
							currIdx = 0;
							pThis->m_animation.sLine.repeatTimes--;
						}
						else
						{
							pThis->m_pStream->Set(IID_IMcVideoPresenterAnimation, MC_PROPID_VP_RECTDST,0,0,&m_pLineMatrix[currIdx],0);
							//Sleep(1);
							pThis->m_animation.sLine.lastUpdateTime = GetTickCount();
							currIdx++;
						}
					}
				}
				else
				{
					pThis->m_animation.sLine.state = MC_ANIMATION_STATE_UNINIT;
				}
			}

			if(pThis->m_animation.sShake.state == MC_ANIMATION_STATE_RUNNING)
			{
				bAniRunning = true;
				if(pThis->m_animation.sShake.repeatTimes)
				{

					if(GetTickCount() -pThis->m_animation.sShake.lastUpdateTime  >pThis->m_animation.sShake.Gap)
					{
						RECT rDst;
						pThis->m_pStream->Get(IID_IMcVideoPresenterAnimation, MC_PROPID_VP_RECTDST,0,0,&rDst,0,0);
						RECT rShake = rDst;
						
						int ShakeFreq = (int)pThis->m_animation.sShake.duration/5;
						DWORD startTick = GetTickCount();
						DWORD lastTick;
						while(ShakeFreq--)
						{
							pThis->ShakeEffect(0,&rDst,&rShake);
							pThis->m_pStream->Set(IID_IMcVideoPresenterAnimation, MC_PROPID_VP_RECTDST,0,0,&rShake,0);
							lastTick = GetTickCount() - startTick ;
							if(lastTick < 4)
								Sleep(4 - lastTick);
							else
							{
								if((DWORD)ShakeFreq > lastTick/4)							
									ShakeFreq -= lastTick/4;
							}
							Sleep(1);
						}
						pThis->m_pStream->Set(IID_IMcVideoPresenterAnimation, MC_PROPID_VP_RECTDST,0,0,&rDst,0);
						Sleep(1);
						pThis->m_animation.sShake.lastUpdateTime  = GetTickCount();
						
						pThis->m_animation.sShake.repeatTimes--;

					}
				}
				else
				{
					pThis->m_animation.sShake.state = MC_ANIMATION_STATE_UNINIT;
				}
			}

			if(pThis->m_animation.sAlpha.state == MC_ANIMATION_STATE_RUNNING)
			{
				bAniRunning = true;
				if(pThis->m_animation.sAlpha.repeatTimes)
				{
					if( (GetTickCount() -pThis->m_animation.sAlpha.lastUpdateTime  >pThis->m_animation.sAlpha.duration/alphaStep))
					{
						if(alphaMatrix[alphaId] == 512)
						{
							alphaId = 0;
							pThis->m_animation.sAlpha.repeatTimes--;
						}
						else
						{
							pThis->m_pStream->Set(IID_IMcVideoPresenterAnimation, MC_PROPID_VP_ALPHA,0,0,&alphaMatrix[alphaId],0);
							Sleep(1);
							pThis->m_animation.sAlpha.lastUpdateTime = GetTickCount();
							alphaId++;
						}
					}
				}
				else
				{
					pThis->m_animation.sAlpha.state = MC_ANIMATION_STATE_UNINIT;
				}
			}

			if(!bAniRunning)
				pThis->m_animation.state = MC_ANIMATION_STATE_UNINIT;
		}
		else if(pThis->m_animation.state == MC_ANIMATION_STATE_UNINIT)
		{
			if(pThis->m_animation.state == MC_ANIMATION_STATE_UNINIT)
			{
				free(m_pLineMatrix);
				m_pLineMatrix= NULL;
				pThis->m_animation.state = MC_ANIMATION_STATE_FINISH;
			}
			
			pThis->m_animation.state = MC_ANIMATION_STATE_FINISH ;
		}
		else if(pThis->m_animation.state == MC_ANIMATION_STATE_FINISH || pThis->m_animation.state == MC_ANIMATION_STATE_PAUSE)
		{
			Sleep(40);
		}
	}
	return 0;
}


void CVRAnimation::ShakeEffect(INT frame, RECT *pSrc,RECT *pDst)
{
	static int flag = 1;
	if(flag == 1)	flag = -1;
	else		flag = 1;
	
	frame = rand() % 30*flag;
    	SetRect(pDst, pSrc->left + frame ,pSrc->top+ frame , pSrc->right+ frame , pSrc->bottom+ frame );
}

IMcVideoPresenterAnimation* CreateVideoAnimation()
{
	IMcVideoPresenterAnimation* pObj = NULL;
	pObj = new CVRAnimation();
	return pObj;
}


