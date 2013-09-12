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

#ifndef _MCCOM_H_
#define _MCCOM_H_

#define E_PROP_SET_UNSUPPORTED    ((HRESULT)0x80070492L)
#define E_PROP_ID_UNSUPPORTED     ((HRESULT)0x80070490L)

/* the following is used for COM delegation.  It is not
directly called because it does not support delegation/
aggregation. */

DECLARE_INTERFACE(IMcNonDelegatingUnknown)
{
	STDMETHOD(NonDelegatingQueryInterface)(THIS_ REFIID, LPVOID *) PURE;
	STDMETHOD_(ULONG, NonDelegatingAddRef)(THIS) PURE;
	STDMETHOD_(ULONG, NonDelegatingRelease)(THIS) PURE;
};

/*
DECLARE_IUNKNOWN is required for the derived class exporting
COM functionality.  This ensures that the outermost class
which is aggregating the object gets the proper QueryInterface
request. */

#define DECLARE_IUNKNOWN                                        \
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv) {      \
        return GetOwner()->QueryInterface(riid,ppv);            \
    };                                                          \
    STDMETHODIMP_(ULONG) AddRef() {                             \
        return GetOwner()->AddRef();                            \
    };                                                          \
    STDMETHODIMP_(ULONG) Release() {                            \
        return GetOwner()->Release();                           \
    };

class CMcUnknown : IMcNonDelegatingUnknown
{
public:
	CMcUnknown(LPCTSTR pName = 0, LPUNKNOWN pUnk = 0)
	{
		m_lRef = 1;
		m_pName = pName;
	    m_pUnk = pUnk ? pUnk : reinterpret_cast<IUnknown*>(static_cast<IMcNonDelegatingUnknown *>(this));
	}
	virtual ~CMcUnknown() {}
	STDMETHODIMP_(ULONG) NonDelegatingAddRef() 
	{
		return InterlockedIncrement(&m_lRef);
	}
	STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, LPVOID *ppvObj)
	{
		if(ppvObj==0)
			return E_POINTER;
		if(riid==IID_IUnknown)
		{
			*ppvObj = static_cast<IMcNonDelegatingUnknown *>(this);
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	STDMETHODIMP_(ULONG) NonDelegatingRelease()
	{
		LONG ref;
		
		ref = InterlockedDecrement(&m_lRef);
		if(ref<=0)
			delete this;
		return ref;
	}
	LPUNKNOWN GetOwner()
	{
		return m_pUnk;
	}
	STDMETHOD (GetInterface)(LPUNKNOWN pUnk, LPVOID *ppvObj)
	{
		if(ppvObj==0)
			return E_POINTER;
		NonDelegatingAddRef();
		*ppvObj = pUnk;
		return S_OK;
	}
protected:
	LPCTSTR		m_pName;
	LONG		m_lRef;
	LPUNKNOWN	m_pUnk;
};

inline HRESULT ReturnComLong(LONG lValue, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
{
	if(pPropData==0)
		return E_POINTER;
	if(cbPropData<sizeof(LONG))
		return E_INVALIDARG;
	*reinterpret_cast<LONG *>(pPropData) = lValue;
	if(pcbReturned)
		*pcbReturned = sizeof(LONG);
	return S_OK;
}

#endif //_MCCOM_H_
