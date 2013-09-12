//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
#ifndef __VARIANT__
#define __VARIANT__

// variant is a simple class that has the same memory representation as Windows VARIANT structure
// and provides some very basic functionality including cleanup on destruction and string assigment.

#if 0
/**/    // 1. Create variant object
/**/    
/**/    // uninitialized VARIANT
/**/    ce::variant var;
/**/    
/**/    // initialize to a string
/**/    ce::variant var(L"String");
/**/    
/**/    // initialize to a VARIANT
/**/    ce::variant var(vt);
/**/    
/**/    // 2. Assign a string
/**/    
/**/    var = L"string";
/**/    var = bstrName;
/**/    
/**/    // 3. Change type
/**/    
/**/    // in place
/**/    var.ChangeType(VT_BSTR);
/**/    
/**/    // to a new variant object
/**/    var.ChangeType(VT_I4, &var1);
/**/
#endif

// AUTOEXP.DAT
// 
// Add following line to your AUTOEXP.DAT to tell debugger how to display
// objects of this class in watch windows.
//
// ce::variant=$BUILTIN(VARIANT)

namespace ce
{

class variant : public VARIANT
{
public:	
	variant()
        {vt = VT_EMPTY; }

    ~variant()
        {Clear(); }

    variant(const VARIANT& varSrc)
	{
		vt = VT_EMPTY;
		InternalCopy(&varSrc);
	}

    variant(const variant& varSrc)
	{
		vt = VT_EMPTY;
		InternalCopy(&varSrc);
	}

    variant(LPCWSTR pwchSrc)
    {
        vt = VT_EMPTY;
        operator=(pwchSrc);
    }
    
    variant(long lSrc)
    {
        vt = VT_EMPTY;
        operator=(lSrc);
    }
    
    variant(unsigned long ulSrc)
    {
        vt = VT_EMPTY;
        operator=(ulSrc);
    }
    
    variant(short iSrc)
    {
        vt = VT_EMPTY;
        operator=(iSrc);
    }
    
    variant(unsigned short uiSrc)
    {
        vt = VT_EMPTY;
        operator=(uiSrc);
    }
    
    variant(bool boolSrc)
    {
        vt = VT_EMPTY;
        operator=(boolSrc);
    }
    
    // operator=
    variant& operator=(LPCWSTR pwchSrc)
	{
		InternalClear();
		vt = VT_BSTR;
		bstrVal = ::SysAllocString(pwchSrc);
		if (bstrVal == NULL && pwchSrc != NULL)
		{
			vt = VT_ERROR;
			scode = E_OUTOFMEMORY;
		}
		return *this;
	}
	
	variant& operator=(long lSrc)
	{
		InternalClear();
		vt = VT_I4;
		lVal = lSrc;
		return *this;
	}
	
	variant& operator=(unsigned long ulSrc)
	{
		InternalClear();
		vt = VT_UI4;
		ulVal = ulSrc;
		return *this;
	}
	
	variant& operator=(short iSrc)
	{
		InternalClear();
		vt = VT_I2;
		uiVal = iSrc;
		return *this;
	}
	
	variant& operator=(unsigned short uiSrc)
	{
		InternalClear();
		vt = VT_UI2;
		uiVal = uiSrc;
		return *this;
	}
	
	variant& operator=(bool boolSrc)
	{
		InternalClear();
		vt = VT_BOOL;
		boolVal = boolSrc;
		return *this;
	}

    variant& operator=(const variant& var)
    {
        InternalCopy(&var);
        return *this;
    }
    
    // Clear
    HRESULT Clear()
        {return VariantClear(this); }
    
    // Copy
    HRESULT Copy(const VARIANT* pSrc)
        {return ::VariantCopy(this, const_cast<VARIANT*>(pSrc)); }

    // ChangeType
    HRESULT ChangeType(VARTYPE vtNew, const VARIANT* pSrc = NULL)
    {
		VARIANT* pVar = const_cast<VARIANT*>(pSrc);
		// Convert in place if pSrc is NULL
		if (pVar == NULL)
			pVar = this;
		
		return ::VariantChangeType(this, pVar, 0, vtNew);
	}

protected:
    void InternalCopy(const VARIANT* pSrc)
	{
		HRESULT hr = Copy(pSrc);
		
        if(FAILED(hr))
		{
			vt = VT_ERROR;
			scode = hr;
		}
	}

    void InternalClear()
	{
		HRESULT hr = VariantClear(this);
		
		if(FAILED(hr))
		{
			vt = VT_ERROR;
			scode = hr;
		}
	}
};


}; // namespace ce

#endif // __VARIANT__
