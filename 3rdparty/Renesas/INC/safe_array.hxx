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
#ifndef __SAFE_ARRAY__
#define __SAFE_ARRAY__

#if 0
/**/    // 1. to instantiate safe_array template specify VT and type
/**/
/**/    // safe_array of integers
/**/    ce::safe_array<long, VT_I4> arrayI4;
/**/
/**/    // default ctor used so must call create to create safe array
/**/    arrayI4.create(10);
/**/
/**/    // array of 20 BSTRs
/**/    ce::safe_array<BSTR, VT_BSTR> arrayBSTR(20);
/**/    
/**/    // 2. for array of VARAINT you can use ce::variant (variant.hxx)
/**/
/**/    ce::safe_array<ce::variant, VT_VARIANT> arrayVar;
/**/
/**/    arrayVar[1] = L"string";
/**/
/**/    // 3. safe_array interface is more or less like C array or vector
/**/
/**/    // create safe array of 10 longs
/**/    ce::safe_array<long, VT_I4> arrayI4(10);
/**/
/**/    arrayI4[0] = 10;
/**/    
/**/    x = arrayI4[0];
/**/
/**/    n = arrayI4.size();
/**/
/**/    // 4. safe arrays don't always start from index 0
/**/
/**/    for(int i = arrayI4.lbound(); i <= arrayI4.ubound(); ++i)
/**/        // ...
/**/
/**/    // 5. safe array can be resized - resize doesn't change lower bound
/**/
/**/    arrayI4.resize(30);
/**/
/**/    // 6. you can get pointers to elements within array but you must lock it
/**/
/**/    arrayI4.lock();
/**/
/**/    long* p = &arrayI4[10];
/**/
/**/    // ...
/**/
/**/    arrayI4.unlock(); // unlock will be also called if needed from dtor
/**/
/**/    // 7. you can attach safe_array object to exisiting SAFEARRAY*
/**/
/**/    ce::safe_array<long, VT_I4> arrayI4; // default ctor - no safe array created
/**/
/**/    arrayI4.attach(psa);
/**/
/**/    // when arrayI4 goes out of scope, psa safe array will NOT be destroyed 
/**/    // because it was only attached 
/**/
/**/    // if you want to destory safe array created externally pass it to ctor instead:
/**/    ce::safe_array<long, VT_I4> arrayI4(psa);
/**/
#endif

namespace ce
{

namespace details
{

template <int Size, VARTYPE vt> class validate_variant_type;

}; // namespace details

// safe_array
template <class T, VARTYPE vt>
class safe_array
{
    //
    // Compiler error about undefined validate_variant_type
    // means that type T has wrong size for VARTYPE vt
    //
    enum {_valid_variant_type = sizeof(details::validate_variant_type<sizeof(T), vt>) };
                                                       
    class _element_proxy;
public:
    safe_array()
        : m_psa(NULL), m_bOwn(true), m_lLock(0)
        {}

    safe_array(unsigned long cElements, long lLbound = 0)
        : m_psa(NULL), m_bOwn(true), m_lLock(0)
    {
        create(cElements, lLbound);
    }

    safe_array(SAFEARRAY* psa)
        : m_psa(psa), m_bOwn(true), m_lLock(0)
        {}
    
    ~safe_array()
        {destroy(); }

    // destroy
    void destroy()
    {
        while(m_lLock)
            unlock();

        if(m_bOwn && m_psa)
            SafeArrayDestroy(m_psa);

        m_psa = NULL;
    }

    // create
    void create(unsigned long cElements, long lLbound = 0)
    {
        SAFEARRAYBOUND bounds = {cElements, lLbound};

        destroy();

        m_psa = SafeArrayCreate(vt, 1, &bounds);
        m_bOwn = true;
        m_lLock = 0;
    }
    
    // attach
    void attach(SAFEARRAY* psa)
    {
        destroy();
        m_psa = psa;
        m_bOwn = false;
        m_lLock = 0;
    }

	// detach
	SAFEARRAY* detach()
	{
		m_bOwn = false;
		return m_psa;
	}

    // valid
    bool valid()
        {return m_psa != NULL; }
    
    // lock
    HRESULT lock()
    {
		++m_lLock;

		return SafeArrayLock(m_psa); 
	}

    // unlock
    HRESULT unlock()
    {
		--m_lLock;

		return SafeArrayUnlock(m_psa);
	}

    // operator[]
    _element_proxy operator[](long index)
        {return _element_proxy(m_psa, index); }

    // operator&
    SAFEARRAY** operator&()
        {return &m_psa; }

    // size
    long size()
        {return ubound() - lbound() + 1; }

    // ubound
    long ubound()
    {
        long lUBound = -1;

        SafeArrayGetUBound(m_psa, 1, &lUBound);

        return lUBound;
    }

    // lbound
    long lbound()
    {
        long lLBound = 0;

        SafeArrayGetLBound(m_psa, 1, &lLBound);

        return lLBound;
    }

    // resize
    HRESULT resize(long cElements)
    {
        SAFEARRAYBOUND bounds = {cElements, lbound()};

        return SafeArrayRedim(m_psa, &bounds);
    }

private:
    class _element_proxy
    {
    public:
        _element_proxy(SAFEARRAY* psa, long index)
            : m_index(index),
              m_psa(psa)
            {}

        // assign value to safe_array element
        _element_proxy& operator=(const T& value)
        {
            SafeArrayPutElement(m_psa, &m_index, (void*)&value); 
            return *this;
        }

        // to assign one element of safe_array to another
        _element_proxy& operator=(const _element_proxy& value)
            {return operator=((T)value); }

        // get value of safe_array element
        operator T() const
        {
            T value;

            SafeArrayGetElement(m_psa, &m_index, &value);

            return value;
        }

        // get pointer to element in array - should lock array before that!
        T* operator&()
        {
            T* pElement = NULL;

            if(m_psa)
                SafeArrayPtrOfIndex(m_psa, &m_index, (void**)&pElement);

            return pElement;
        }

    private:
        mutable long        m_index;
        mutable SAFEARRAY  *m_psa;
    };

private:
    SAFEARRAY*  m_psa;
    bool        m_bOwn;
    long        m_lLock;
};


namespace details
{
    //
    // general definition for the template is not provided 
    // so that compiler error is generated for invalid combination of size/vartype
    //
    template <int Size, VARTYPE vt>
    class validate_variant_type;
    
    //
    // explicit instantiations of validate_variant_type template 
    // for valid size/vartype combinations
    //
    template <> class validate_variant_type<sizeof(LONG),           VT_I4>{};
    template <> class validate_variant_type<sizeof(BYTE),           VT_UI1>{};
    template <> class validate_variant_type<sizeof(SHORT),          VT_I2>{};
    template <> class validate_variant_type<sizeof(FLOAT),          VT_R4>{};
    template <> class validate_variant_type<sizeof(DOUBLE),         VT_R8>{};
    template <> class validate_variant_type<sizeof(VARIANT_BOOL),   VT_BOOL>{};
    template <> class validate_variant_type<sizeof(SCODE),          VT_ERROR>{};
    template <> class validate_variant_type<sizeof(CY),             VT_CY>{};
    template <> class validate_variant_type<sizeof(DATE),           VT_DATE>{};
    template <> class validate_variant_type<sizeof(BSTR),           VT_BSTR>{};
    template <> class validate_variant_type<sizeof(IUnknown*),      VT_UNKNOWN>{};
    template <> class validate_variant_type<sizeof(IDispatch*),     VT_DISPATCH>{};
    template <> class validate_variant_type<sizeof(CHAR),           VT_I1>{};
    template <> class validate_variant_type<sizeof(USHORT),         VT_UI2>{};
    template <> class validate_variant_type<sizeof(ULONG),          VT_UI4>{};
    template <> class validate_variant_type<sizeof(INT),            VT_INT>{};
    template <> class validate_variant_type<sizeof(UINT),           VT_UINT>{};
    template <> class validate_variant_type<sizeof(VARIANT),        VT_VARIANT>{};
};


}; // namespace ce

#endif // __SAFE_ARRAY__
