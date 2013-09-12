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

#include <assert.h>
#include <algorithm>
#include "allocator.hxx"

#pragma warning(push)

// 4284 warning for operator-> returning non-pointer;
//      compiler issues it even if -> is not used for the specific instance
#pragma warning(disable: 4284) 

#if 0
/**/
/**/    ///////////////////////////////////////////////////////
/**/    // 1. instances of auto_xxx template can be used to automatically close a resource
/**/    
/**/    ce::auto_handle hEvent;
/**/    
/**/    hEvent = CreateEvent(/*...*/)
/**/    
/**/    // event handle will be closed when hEvent goes out of scope
/**/    // if hEvent was a class member it would be closed when object
/**/    // of the class is destroyed
/**/
/**/    ///////////////////////////////////////////////////////
/**/    // 2. use valid() to check if resource is valid
/**/    
/**/    ce::auto_bstr bstr;
/**/    
/**/    bstr = SysAllocString(/*...*/);
/**/    
/**/    if(!bstr.valid())
/**/        return E_OUT_OF_MEMORY;
/**/
/**/    ///////////////////////////////////////////////////////
/**/    // 3. use close() to explicitly close resource
/**/    
/**/    ce::auto_hfile hFile;
/**/
/**/    hFile = CreateFile(L"filename", /*...*/);
/**/                                
/**/    /*...*/
/**/
/**/    hFile.close();           // close file
/**/    DeleteFile(L"filename"); // and delete it
/**/
/**/    ///////////////////////////////////////////////////////
/**/    // 4. auto_xxx has the same memory representation as the managed resource
/**/    //    Note, this is true only for the version w/o copy semantics (see #6 below)
/**/
/**/    ce::auto_handle hEvents[3];
/**/
/**/    hEvents[0] = CreateEvent(/*...*/);
/**/    hEvents[1] = CreateEvent(/*...*/);
/**/    hEvents[2] = CreateEvent(/*...*/);
/**/
/**/    WaitForMultipleObjects(sizeof(hEvents)/sizeof(hEvents[0]), &hEvents[0], false, INFINITE);
/**/
/**/    ///////////////////////////////////////////////////////
/**/    // 5. auto pointers and smart pointers
/**/
/**/    // auto pointer to an object
/**/    ce::auto_ptr<A> p = new A;
/**/
/**/    // auto pointer to an array of objects
/**/    ce::auto_array_ptr<A> p = new A[10];
/**/
/**/    // smart (ref counted) pointer to an object
/**/    ce::smart_ptr<A> p = new A;
/**/
/**/    ///////////////////////////////////////////////////////
/**/    // 6. define your own instances of auto_xxx as needed
/**/
/**/    ce::auto_hfile   hFile;
/**/    ce::auto_handle  hMap;
/**/
/**/    // char*                        - type of resource
/**/    // BOOL (__stdcall*)(LPCVOID)   - type of pointer to "close" function for resource
/**/    // UnmapViewOfFile              - "close" function for resource
/**/    // NULL                         - invalid value for resource
/**/    ce::auto_xxx<char*, BOOL (__stdcall*)(LPCVOID), UnmapViewOfFile, NULL> pchFile;
/**/    
/**/    hFile = CreateFile(/*...*/);
/**/    hMap = CreateFileMapping(/*...*/);
/**/    pchFile = reinterpret_cast<char*>(MapViewOfFile(hMap, /*...*/);
/**/
/**/    // 7. use version of auto_xxx with appropriate copy semantincs
/**/    // 
/**/    // You can specify copy semantics of auto_xxx using copy_traits argument of the template.
/**/    // There are two predefined traits classes that implement common semantics. If neither
/**/    // fits your needs you can define your own.
/**/
/**/    // no_copy - this is the default for auto_xxx. When auto_xxx is instantiated with no_copy
/**/    // traits, objects of this type can’t be copied (which means for example that you won’t 
/**/    // be able to store them in STL containers). The big advantage of using the no_copy version
/**/    // is that memory representation of auto_xxx object is the same as for the resource it 
/**/    // manages. This allows usage like in example #4 above. This version also generates the
/**/    // most compact code. This version is recommended unless you need copy semantics. 
/**/    // Note that you will get a compiler error when you use this version in a context that 
/**/    // requires copy semantics.
/**/ 
/**/    // ref_counting - implements reference counting for the resource. When auto_xxx is instantiated 
/**/    // with ref_counting traits, reference counting semantics are used to manage the resource. 
/**/    // The resource is released when the last auto_xxx object referring to the resource is destroyed. 
/**/    // This version is most fool-proof but also has biggest overhead.
/**/
/**/    // reference counted variant of auto_handle
/**/    typedef ce::auto_xxx<HANDLE, BOOL (__stdcall*)(HANDLE), CloseHandle, NULL, ce::ref_counting>       smart_handle;
#endif

// AUTOEXP.DAT
// 
// Add following line to your AUTOEXP.DAT to tell debugger how to display
// objects of this class in watch windows.
//
// ce::auto_xxx<*>=<x>

namespace ce
{

#ifndef _AUTO_XXX_
#define _AUTO_XXX_

//
// following copy trait classes define copy semantics for auto_xxx objects
// (see comment #7 above)
//

// no_copy
// auto_xxx objects can't be copied
class no_copy
{
protected:
    void create() const
        {}
    
    bool release() const
    {
        // always release resource
        return true;
    }
};


/* strict_ownership
strict_ownership was an attempt to define a trait class for auto_xxx 
that would implement ownership based copy semantics. The approach was 
similar to the implementation of auto_ptr in VC6 (read: broken).
For this reason support for ownership in auto_xxx is deprecated.

class strict_ownership
{
protected:
    strict_ownership()
        : m_bOwner(true)
    {}
    
    ~strict_ownership()
        {assert(m_bOwner == false); }

    void create() const
        {m_bOwner = true; }
    
    bool release() const
    {
        bool bOwner = m_bOwner;
        
        m_bOwner = false;
        
        // release resource only if the owner
        return bOwner;
    }
        
    void copy(const strict_ownership& p) const
    {
        // take over ownership from p
        m_bOwner = p.release();
    }
    
    void swap(strict_ownership& p) const
    {
        std::swap(m_bOwner, p.m_bOwner);
    }
        
protected:
    mutable bool m_bOwner;
};*/


// ref_counting
// objects are reference counted
template <class _Al>
class _ref_counting_t : _Al
{
protected:
    typedef _ref_counting_t<_Al>    _Myt;
    
    _ref_counting_t()
        : m_pRefCount(NULL)
    {}
    
    ~_ref_counting_t()
    {
        assert(m_pRefCount == NULL);
    }
    
    void create()
    {
        assert(m_pRefCount == NULL);
        m_pRefCount = reinterpret_cast<long*>(this->allocate(sizeof(*m_pRefCount)));
        if(m_pRefCount)
            *m_pRefCount = 1;
    }
    
    bool release()
    {
        // release resource only if ref count goes to zero
        if(m_pRefCount && (0 == InterlockedDecrement(m_pRefCount)))
        {
            this->deallocate(m_pRefCount, sizeof(*m_pRefCount));
            m_pRefCount = NULL;
            
            return true;
        }
        
        m_pRefCount = NULL;
        
        return false;
    }
        
    void copy(const _Myt& p)
    {
        assert(m_pRefCount == NULL);

        m_pRefCount = p.m_pRefCount;
        if(m_pRefCount)
        {
            assert(*m_pRefCount > 0);
            
            InterlockedIncrement(m_pRefCount);
        }
    }
    
    void swap(_Myt& p)
    {
        std::swap(m_pRefCount, p.m_pRefCount);
    }
        
protected:
    long* m_pRefCount;
};


//
// forward declarations
//
class allocator;
template <typename _Al, size_t Unique>  class singleton_allocator;
template <size_t Count, typename _Al>   class free_list;


//
// instance of _ref_couting_t template using a free list allocator to minimize heap fragmentation
//
typedef _ref_counting_t<ce::singleton_allocator<ce::free_list<100, allocator>, sizeof(long)> > ref_counting;


// 
// function that deletes objects of type T 
//
template<class T>
inline void _delete(T* p)
    {delete p; }
    
// 
// function that deletes array of objects of type T 
//
template<class T>
inline void _delete_array(T* p)
    {delete[] p; }


//
// template class auto_xxx
//
// T            - resource type
// Fn           - type of pointer to "close" function for the resource
// fnClose      - "close" function for the resource
// _Invalid     - invalid value for the resource
// copy_traits  - traits class defining copy semantics for the objects
//

template<class T, class Fn, Fn fnClose, T _Invalid, class copy_traits = no_copy>
class auto_xxx : protected copy_traits
{
public:
    typedef auto_xxx<T, Fn, fnClose, _Invalid, copy_traits> _Myt;

    auto_xxx()
        : x(_Invalid)
        {}
        
    auto_xxx(T _Y)
        : x(_Y) 
        {copy_traits::create(); }

    auto_xxx(const _Myt& _Y)
        : x(_Y.x) 
    {
        // If you get compiler error it means that you are using an instance of auto_xxx
        // that doesn't implement copy semantics in a context that requires coping/assigning objects.
        // You can instantiate auto_xxx with ref_counting traits to support the copy semantics.
        copy_traits::copy(_Y);
    }
    
    ~auto_xxx()
    {
        if(copy_traits::release() && x != _Invalid)
            (*fnClose)(x);
    }
    
    _Myt& operator=(const _Myt& _Y)
    {
        _Myt(_Y).swap(*this);
        
        return (*this);
    }
    
    _Myt& operator=(const T& _Y)
    {
        if(copy_traits::release() && x != _Invalid)
            (*fnClose)(x);

        x = _Y;
        
        copy_traits::create();

        return (*this); 
    }
    
    void close()
    {
        if(copy_traits::release() && x != _Invalid)
            (*fnClose)(x);
        
        x = _Invalid;
    }
    
    T release()
    {
        T temp = x;
        
        copy_traits::release();
        x = _Invalid;
        
        return temp;
    }
    
    BOOL valid() const
        {return x != _Invalid; }

    // for use when auto_xxx appears in a conditional
    bool operator!() const
        {return x == _Invalid; }
        
    T operator->() const
        {return x; }

    const T* operator&() const
        {return &x; }

    T* operator&()
    {
        copy_traits::create();
        return &x;
    }
        
    operator T() const
        {return x; }
        
protected:
    void swap(_Myt& _Y)
    {
        std::swap(x, _Y.x);
        copy_traits::swap(_Y);
    }

protected:
    T x;
};


//
// template class auto_ptr
//
template <typename T>
class auto_ptr : public auto_xxx<T*, void (*)(T*), _delete, NULL, no_copy>
{
    typedef auto_ptr<T>                                         _Myt;
    typedef auto_xxx<T*, void (*)(T*), _delete, NULL, no_copy>  _Mybase;
    
public:
    auto_ptr()
        {}
        
    auto_ptr(T* _Y)
        : _Mybase(_Y) 
        {}
        
    _Myt& operator=(T* _Y)
    {
        _Mybase::operator=(_Y);
        return *this;
    }
        
    T& operator*()
        {return *x; }
};


//
// template class auto_array_ptr
//
template <typename T>
class auto_array_ptr : public auto_xxx<T*, void (*)(T*), _delete_array, NULL, no_copy>
{
    typedef auto_array_ptr<T>                                           _Myt;
    typedef auto_xxx<T*, void (*)(T*), _delete_array, NULL, no_copy>    _Mybase;
    
public:
    auto_array_ptr()
        {}
        
    auto_array_ptr(T* _Y)
        : _Mybase(_Y) 
        {}
        
    _Myt& operator=(T* _Y)
    {
        _Mybase::operator=(_Y);
        return *this;
    }
    
    T& operator*()
        {return *x; }
};


//
// template class smart_ptr
//
template <typename T>
class smart_ptr : public auto_xxx<T*, void (*)(T*), _delete, NULL, ref_counting>
{
    typedef smart_ptr<T>                                            _Myt;
    typedef auto_xxx<T*, void (*)(T*), _delete, NULL, ref_counting> _Mybase;
    
public:
    smart_ptr()
        {}
        
    smart_ptr(T* _Y)
        : _Mybase(_Y) 
        {}
        
    _Myt& operator=(T* _Y)
    {
        _Mybase::operator=(_Y);
        return *this;
    }
    
    T& operator*()
        {return *x; }
};


//
// template class smart_array_ptr
//
template <typename T>
class smart_array_ptr : public auto_xxx<T*, void (*)(T*), _delete_array, NULL, ref_counting>
{
    typedef smart_array_ptr<T>                                              _Myt;
    typedef auto_xxx<T*, void (*)(T*), _delete_array, NULL, ref_counting>   _Mybase;
    
public:
    smart_array_ptr()
        {}
        
    smart_array_ptr(T* _Y)
        : _Mybase(_Y) 
        {}
        
    _Myt& operator=(T* _Y)
    {
        _Mybase::operator=(_Y);
        return *this;
    }
        
    T& operator*()
        {return *x; }
};


//
// Below are predefined instances of auto_xxx template for some common Windows CE resource types.
// Types with names starting with auto_ have not copy semantics.
// Types with names starting with smart_ are reference counted.
//

#endif // _AUTO_XXX_

#ifdef _WINDEF_
#   ifndef _AUTO_WINDEF_H_
#   define _AUTO_WINDEF_H_

    typedef auto_xxx<HINSTANCE, BOOL(__stdcall*)(HINSTANCE), FreeLibrary, 0>                auto_hlibrary;
    typedef auto_xxx<HANDLE, BOOL (__stdcall*)(HANDLE), CloseHandle, NULL>                  auto_handle;
    typedef auto_xxx<HKEY, LONG (__stdcall*)(HKEY), RegCloseKey, NULL>                      auto_hkey;
    typedef auto_xxx<HANDLE, BOOL (__stdcall*)(HANDLE), CloseHandle, INVALID_HANDLE_VALUE>  auto_hfile;
    typedef auto_xxx<HANDLE, BOOL (__stdcall*)(HANDLE), FindClose, INVALID_HANDLE_VALUE>    auto_hfind;
    typedef auto_xxx<DWORD, BOOL (__stdcall*)(DWORD), TlsFree, 0xFFFFFFFF>                  auto_tls;
    typedef auto_xxx<HLOCAL, HLOCAL (__stdcall*)(HLOCAL), LocalFree, NULL>                  auto_local_mem;
    typedef auto_xxx<HANDLE, BOOL (__stdcall*)(HANDLE), HeapDestroy, NULL>                  auto_hheap;

    typedef auto_xxx<HINSTANCE, BOOL(__stdcall*)(HINSTANCE), FreeLibrary, 0, ref_counting>                  smart_hlibrary;
    typedef auto_xxx<HANDLE, BOOL (__stdcall*)(HANDLE), CloseHandle, NULL, ref_counting>                    smart_handle;
    typedef auto_xxx<HKEY, LONG (__stdcall*)(HKEY), RegCloseKey, NULL, ref_counting>                        smart_hkey;
    typedef auto_xxx<HANDLE, BOOL (__stdcall*)(HANDLE), CloseHandle, INVALID_HANDLE_VALUE, ref_counting>    smart_hfile;
    typedef auto_xxx<HANDLE, BOOL (__stdcall*)(HANDLE), FindClose, INVALID_HANDLE_VALUE, ref_counting>      smart_hfind;
    typedef auto_xxx<DWORD, BOOL (__stdcall*)(DWORD), TlsFree, 0xFFFFFFFF, ref_counting>                    smart_tls;
    typedef auto_xxx<HLOCAL, HLOCAL (__stdcall*)(HLOCAL), LocalFree, NULL, ref_counting>                    smart_local_mem;
    typedef auto_xxx<HANDLE, BOOL (__stdcall*)(HANDLE), HeapDestroy, NULL, ref_counting>                    smart_hheap;

#   endif // _AUTO_WINDEF_H_
#endif // _WINDEF_


#ifdef __MSGQUEUE_H__
#   ifndef _AUTO_MSG_QUEUE_
#   define _AUTO_MSG_QUEUE_

    typedef auto_xxx<HANDLE, BOOL (__stdcall*)(HANDLE), CloseMsgQueue, NULL>                auto_msg_queue;
    typedef auto_xxx<HANDLE, BOOL (__stdcall*)(HANDLE), CloseMsgQueue, NULL, ref_counting>  smart_msg_queue;

#   endif // _AUTO_MSG_QUEUE_
#endif // __MSGQUEUE_H__

#ifdef _OLEAUTO_H_
#   ifndef _AUTO_OLEAUTO_
#   define _AUTO_OLEAUTO_

    typedef auto_xxx<BSTR, void (__stdcall*)(BSTR), SysFreeString, NULL>                    auto_bstr;
    typedef auto_xxx<BSTR, void (__stdcall*)(BSTR), SysFreeString, NULL, ref_counting>      smart_bstr;

#   endif // _AUTO_OLEAUTO_
#endif  // _OLEAUTO_H_

#if defined _WININET_ || defined _DUBINET_
#   ifndef _AUTO_WININET_
#   define _AUTO_WININET_

    typedef auto_xxx<HINTERNET, BOOL (__stdcall*)(HINTERNET), InternetCloseHandle, NULL>    auto_hinternet;
    typedef auto_xxx<HINTERNET, BOOL (__stdcall*)(HINTERNET), InternetCloseHandle, NULL, ref_counting> smart_hinternet;

#   endif // _AUTO_WININET_
#endif // _WININET_ || _DUBINET_

#if defined _WINSOCKAPI_
#   ifndef _AUTO_WINSOCKAPI_
#   define _AUTO_WINSOCKAPI_

    typedef auto_xxx<SOCKET, int (__stdcall*)(SOCKET), closesocket, INVALID_SOCKET>         auto_socket;
    typedef auto_xxx<SOCKET, int (__stdcall*)(SOCKET), closesocket, INVALID_SOCKET, ref_counting> smart_socket;

#   endif // _AUTO_WINSOCKAPI_
#endif // _WINSOCKAPI_

#if defined WINGDIAPI
#   ifndef _AUTO_GDIHANDLE_
#   define _AUTO_GDIHANDLE_

    typedef auto_xxx<HPEN,    BOOL (__stdcall*)(HGDIOBJ), DeleteObject, NULL>               auto_hpen;
    typedef auto_xxx<HBRUSH,  BOOL (__stdcall*)(HGDIOBJ), DeleteObject, NULL>               auto_hbrush;
    typedef auto_xxx<HFONT,   BOOL (__stdcall*)(HGDIOBJ), DeleteObject, NULL>               auto_hfont;
    typedef auto_xxx<HBITMAP, BOOL (__stdcall*)(HGDIOBJ), DeleteObject, NULL>               auto_hbitmap;
    typedef auto_xxx<HRGN,    BOOL (__stdcall*)(HGDIOBJ), DeleteObject, NULL>               auto_hrgn;
    typedef auto_xxx<HPALETTE,BOOL (__stdcall*)(HGDIOBJ), DeleteObject, NULL>               auto_hpalette;
    typedef auto_xxx<HICON,   BOOL (__stdcall*)(HICON),   DestroyIcon,  NULL>               auto_hicon;
    typedef auto_xxx<HDC,     BOOL (__stdcall*)(HDC),     DeleteDC,     NULL>               auto_hdc;
    
    typedef auto_xxx<HPEN,    BOOL (__stdcall*)(HGDIOBJ), DeleteObject, NULL, ref_counting> smart_hpen;
    typedef auto_xxx<HBRUSH,  BOOL (__stdcall*)(HGDIOBJ), DeleteObject, NULL, ref_counting> smart_hbrush;
    typedef auto_xxx<HFONT,   BOOL (__stdcall*)(HGDIOBJ), DeleteObject, NULL, ref_counting> smart_hfont;
    typedef auto_xxx<HBITMAP, BOOL (__stdcall*)(HGDIOBJ), DeleteObject, NULL, ref_counting> smart_hbitmap;
    typedef auto_xxx<HRGN,    BOOL (__stdcall*)(HGDIOBJ), DeleteObject, NULL, ref_counting> smart_hrgn;
    typedef auto_xxx<HPALETTE,BOOL (__stdcall*)(HGDIOBJ), DeleteObject, NULL, ref_counting> smart_hpalette;
    typedef auto_xxx<HICON,   BOOL (__stdcall*)(HICON),   DestroyIcon,  NULL, ref_counting> smart_hicon;
    typedef auto_xxx<HDC,     BOOL (__stdcall*)(HDC),     DeleteDC,     NULL, ref_counting> smart_hdc;

#   endif // _AUTO_GDIHANDLE_
#endif // WINGDIAPI

#if defined _WINSCARD_H_
#   ifndef _AUTO_SCARDHANDLE_
#   define _AUTO_SCARDHANDLE_

    typedef auto_xxx<SCARDCONTEXT, LONG (__stdcall*)(SCARDCONTEXT), SCardReleaseContext, 0> auto_scardctx;
    typedef auto_xxx<SCARDCONTEXT, LONG (__stdcall*)(SCARDCONTEXT), SCardReleaseContext, 0, ref_counting> smart_scardctx;
    
#   endif // _AUTO_SCARDHANDLE_
#endif 
#if defined __WINCRYPT_H__
#   ifndef _AUTO_CRYPTHANDLE_
#   define _AUTO_CRYPTHANDLE_

    inline void CertCloseStoreNoParam(const HCERTSTORE hCertStore)
    {
        CertCloseStore(hCertStore,NULL);
    }

    inline void CloseCryptConext(const HCRYPTPROV hCryptCtx)
    {
        CryptReleaseContext(hCryptCtx,0);
    }
    typedef auto_xxx<PCCERT_CONTEXT, BOOL (__stdcall*)(PCCERT_CONTEXT), CertFreeCertificateContext, NULL> auto_cert_context;
    typedef auto_xxx<PCCERT_CHAIN_CONTEXT, VOID (__stdcall*)(PCCERT_CHAIN_CONTEXT), CertFreeCertificateChain, NULL> auto_cert_chain_context;
    typedef auto_xxx<HCERTSTORE, void (__cdecl*)(const HCERTSTORE), CertCloseStoreNoParam, NULL> auto_hcertstore;
    typedef auto_xxx<HCRYPTPROV, void (__cdecl*)(const HCRYPTPROV), CloseCryptConext, NULL> auto_hcryptprov;
    typedef auto_xxx<HCRYPTKEY, BOOL (__stdcall*)(const HCRYPTKEY), CryptDestroyKey, NULL> auto_hcryptkey;
    typedef auto_xxx<HCRYPTHASH, BOOL (__stdcall*)(const HCRYPTHASH), CryptDestroyHash, NULL> auto_hcrypthash;
    
    typedef auto_xxx<PCCERT_CONTEXT, BOOL (__stdcall*)(PCCERT_CONTEXT), CertFreeCertificateContext,NULL, ref_counting> smart_cert_context;
    typedef auto_xxx<PCCERT_CHAIN_CONTEXT, VOID (__stdcall*)(PCCERT_CHAIN_CONTEXT), CertFreeCertificateChain, NULL,ref_counting> smart_cert_chain_context;
    typedef auto_xxx<HCERTSTORE, void (__cdecl*)(const HCERTSTORE), CertCloseStoreNoParam,NULL, ref_counting> smart_hcertstore;
    typedef auto_xxx<HCRYPTPROV, void (__cdecl*)(const HCRYPTPROV), CloseCryptConext,NULL, ref_counting> smart_hcryptprov;
    typedef auto_xxx<HCRYPTKEY, BOOL (__stdcall*)(const HCRYPTKEY), CryptDestroyKey,NULL, ref_counting> smart_hcryptkey;
    typedef auto_xxx<HCRYPTHASH, BOOL (__stdcall*)(const HCRYPTHASH), CryptDestroyHash,NULL, ref_counting> smart_hcrypthash;
#   endif // _AUTO_CRYPTHANDLE_
#endif 
}; // namespace ce

#pragma warning(pop)

