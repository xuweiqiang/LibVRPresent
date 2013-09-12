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

#ifndef _GFXWRAPPER_H_
#define _GFXWRAPPER_H_

class CGfxOverlay;

class CGfxWrapper
{
public:
	CGfxWrapper(IMcGfxOverlay *pOverlay);
	virtual ~CGfxWrapper();
	STDMETHODIMP OpenBuffers(DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers);
	STDMETHODIMP CloseBuffers();
	STDMETHODIMP LockBuffer(LONG lBufNum, BYTE **ppBuf, LONG *plStride);
	STDMETHODIMP UnlockBuffer(LONG lBufNum);
	static STDMETHODIMP StaticOpenBuffers(void *pLockContext, DWORD dwWidth, DWORD dwHeight, DWORD dwBuffers);
	static STDMETHODIMP StaticCloseBuffers(void *pLockContext);
	static STDMETHODIMP StaticLockBuffer(void *pLockContext, LONG lBufNum, BYTE **ppBuf, LONG *plStride);
	static STDMETHODIMP StaticUnlockBuffer(LPVOID pLockContext, LONG lBufNum);

protected:
	IMcGfxOverlay *m_pOverlay;
	DWORD m_dwWidth, m_dwHeight;
};


#endif // _GFXWRAPPER_H_
