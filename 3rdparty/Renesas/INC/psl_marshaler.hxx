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

#include <windows.h>
#include <auto_xxx.hxx>
#include <hash_map.hxx>
#include <singleton.hxx>
#include <sync.hxx>

#ifndef __PSL_MARSHALER__
#define __PSL_MARSHALER__

//
// Set of templates implementing generic and extensible proxy and stub for 
// PSL calls in Windows CE. 
//

#if 0
/**/
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/    // 1. Implementing "remote" API using psl_proxy and psl_stub
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/
/**/    #define XYZ_IOCTL_INVOKE    100
/**/    #define FOO                 1
/**/    #define BAR                 2
/**/
/**/    //***************
/**/    // Client process
/**/    //***************
/**/
/**/    //
/**/    // psl_proxy object for server hosted in device/service XYZ1:
/**/    // Lifetime of the object typically should be equal to the lifetime of the client.
/**/    // When the psl_proxy object is destroyed it will close device/service handle 
/**/    // (which potentially will clean client's context on the device/service).
/**/    // Usually psl_proxy object will be declared as global or singleton.
/**/    //
/**/    ce::psl_proxy<> proxy(L"XYZ1:", XYZ_IOCTL_INVOKE, NULL);
/**/
/**/    DWORD Foo(DWORD* pdwSize, VOID* pszBuffer)
/**/    {
/**/        return proxy.call(FOO, pdwSize, pszBuffer);
/**/    }
/**/    
/**/    BOOL Bar(DWORD dwCount, const Record* pRecords)
/**/    {
/**/        // proxy.call returns DWORD error even for functions that return TRUE or FALSE
/**/        // The error code is also available to the caller via GetLastError
/**/        return ERROR_SUCCESS == proxy.call(BAR, dwCount, pRecords);
/**/    }
/**/    
/**/
/**/    //***************
/**/    // Server process
/**/    //***************
/**/    
/**/    // Server side implementation of Foo API.
/**/    // Note that the name of the server side functions may be but don't have to be 
/**/    // the same as name of appropriate client side functions. Signatures of the
/**/    // functions have to be the same on server and client side.
/**/    
/**/    // Server function that returns DWORD indicating success (ERROR_SUCCESS) or failure
/**/    DWORD Foo(DWORD* pdwSize, VOID* pszBuffer)
/**/    {
/**/        ...
/**/    }
/**/    
/**/    // Server function that returns TRUE or FALSE and sets last error in case of failure
/**/    BOOL Bar(DWORD dwCount, const Record* pRecords)
/**/    {
/**/        ...
/**/    }
/**/
/**/    // IOControl function for driver/service that is hosting the server
/**/    BOOL XYZ_IOControl(..., DWORD dwCode, IN PBYTE pBufIn, IN DWORD dwLenIn, ...)
/**/    {
/**/        DWORD dwErr = ERROR_INVALID_PARAMETER;
/**/
/**/        switch(dwCode)
/**/        {
/**/            case XYZ_IOCTL_INVOKE:
/**/            {
/**/                //
/**/                // Initialize stub object from input buffer and validate that input buffer
/**/                // is valid in the context of the calling process.
/**/                //
/**/                ce::psl_stub<> stub(pBufIn, dwLenIn);
/**/                
/**/                // Get ID of API that is being called; will return ce::INVALID_FUNCTION_ID if the
/**/                // validation of the input buffer failed. 
/**/                switch(stub.function())
/**/                {
/**/                    //
/**/                    // Call appropriate API implementation based on the ID.
/**/                    // psl_stub will extract arguments from the input buffer and call the API implementation.
/**/                    // For any arguments that are pointers, psl_stub will validate that the memory
/**/                    // region they are pointing to is valid in context of the client process (by default caller
/**/                    // process is used; handle of another process can be passed as 3rd argument to psl_stub ctor). 
/**/                    // psl_stub will then map the pointer so that it can be accessed by the server process.
/**/                    // psl_stub wraps call to the API implementation in __try{}__except block. If exception 
/**/                    // happens during execution of the API call ERROR_EXCEPTION_IN_SERVICE error is returned 
/**/                    // to the client. Note that exception handler in psl_stub is a last resort. Usually server 
/**/                    // implementation should locally handle exceptions when accessing memory from the client
/**/                    // to avoid leaks.
/**/                    //
/**/                    // IMPORTANT: If an argument points to an array of items or otherwise to a region 
/**/                    //            larger than the type of pointer would indicate, it is responsibility
/**/                    //            of the server to call MapCallerPtr on the pointer with appropriate
/**/                    //            size. For example, for a LPCWSTR argument, psl_stub will only validate
/**/                    //            that the calling process has access to sizeof(wchar_t) bytes.
/**/                    //
/**/                    case FOO:       dwErr = stub.call(Foo);
/**/                                    break;
/**/
/**/                                    // Bar returns TRUE or FALSE and sets last error in case of failure.
/**/                    case BAR:       return stub.call(Bar);
/**/                }
/**/                break;
/**/            }
/**/        }
/**/        
/**/        if(ERROR_SUCCESS == dwErr)
/**/            return TRUE;
/**/        else
/**/        {
/**/            SetLastError(dwErr);
/**/            return FALSE;
/**/        }    
/**/    }
/**/
/**/
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/    // 2. Tracking per-client state on the server 
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/
/**/    // Server often needs to track per-client state so that it can be cleaned if the client
/**/    // process is terminated w/o proper shutdown. Typically server will associate client 
/**/    // state with dwOpenData argument of DeviceIoControl.
/**/    // 
/**/    // This scheme can be implemented with psl marshaler classes in a following 
/**/    // manner (modification to the server side code from example #1):
/**/
/**/
/**/    // Intermediate function that takes additional dwOpenData argument in addition to
/**/    // the parameters of the Bar function.
/**/    BOOL _Bar(DWORD dwOpenData, DWORD dwCount, const Record* pRecords)
/**/    {
/**/        // Map dwOpenData back to the client state; in this example dwOpenData contains a pointer
/**/        // to a ClientState object representing the client's state. 
/**/        ClientState* pClientState = reinterpret_cast<ClientState*>(dwOpenData);
/**/
/**/        // Call actual server implementation of Bar
/**/        BOOL bRet = Bar(dwCount, pRecords);
/**/
/**/        // Update client state in pClientState as appropriate for Bar
/**/
/**/        return bRet;
/**/    }
/**/
/**/    BOOL XYZ_Close(DWORD dwOpenData)
/**/    {
/**/        // Map dwOpenData back to the client state; in this example dwOpenData contains a pointer
/**/        // to a ClientState object representing the client's state. Value of dwOpenData
/**/        // for a client is established in XYZ_Open. 
/**/        ClientState* pClientState = reinterpret_cast<ClientState*>(dwOpenData);
/**/        
/**/        // Cleanup any outstanding state of the client in pClientState
/**/        
/**/        return TRUE;
/**/    }
/**/
/**/    BOOL XYZ_IOControl(DWORD dwOpenData, DWORD dwCode, IN PBYTE pBufIn, IN DWORD dwLenIn, ...)
/**/    {
/**/        switch(dwCode)
/**/        {
/**/            case XYZ_IOCTL_INVOKE:
/**/            {
/**/                ce::psl_stub<> stub(pBufIn, dwLenIn);
/**/                
/**/                switch(stub.function())
/**/                {
/**/                    // dwOpenData can be passed to some (or all) server functions as an extra argument 
/**/                    case BAR:   return stub.call(_Bar, dwOpenData);
/**/
/**/                    // ...
/**/                }
/**/                break;
/**/            }
/**/        }
/**/
/**/        // ...
/**/    }
/**/
/**/
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/    // 3. Passing handle via PSL calls 
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/
/**/    //
/**/    // Handles require special handling when used as arguments to calls made with psl marshaler.  
/**/    // Handles are declared as pointers and they would be mapped by psl_stub. Typically this is 
/**/    // not desired because handles must be treated as opaque values. To prevent treating handles  
/**/    // as regular pointer one needs to provide specialization of psl_data_wrapper class template
/**/    // for the handle type.
/**/    //
/**/    // IMPORTANT: Handles often are declared as simple typedef of void*. Such a handle can't be used
/**/    //            for specialization of psl_data_wrapper because it would result in all void* arguments
/**/    //            not being mapped properly by the server's psl_stub. Only handles declared using
/**/    //            DECLARE_HANDLE macro should be used to specialize psl_data_wrapper.
/**/    //
/**/    // psl_marshaler provides a predefined ce::PSL_HANDLE type and appropriate specialization of 
/**/    // psl_data_wrapper class template:
/**/    //
/**/
/**/    // Server
/**/    DWORD Foo(ce::PSL_HANDLE hpslEvent)
/**/    {
/**/        HANDLE hEvent = reinterpret_cast<HANDLE>(hpslEvent);
/**/    }
/**/
/**/    // Client
/**/    DWORD Foo(HANDLE hEvent)
/**/    {
/**/        return proxy.call(FOO, reinterpret_cast<ce::PSL_HANDLE>(hEvent));
/**/    }
/**/
/**/    // Alternatively user can declare their own handle(s) using DECALRE_HANDLE and specialize
/**/    // psl_data_wrapper class template for it:
/**/
/**/    DECLARE_HANDLE(USER_HANDLE);
/**/
/**/    template<>
/**/    class ce::psl_data_wrapper<USER_HANDLE>
/**/    {
/**/    public:
/**/        void marshal(USER_HANDLE x)
/**/            {data = x; }
/**/    
/**/        USER_HANDLE unmarshal(HANDLE)
/**/            {return data; }
/**/
/**/    private:
/**/        USER_HANDLE data;
/**/    };
/**/
/**/
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/    // 4. Structures with embedded pointers
/**/    //////////////////////////////////////////////////////////////////////////////////////////
/**/
/**/    // Structure with embedded pointers
/**/    struct Record
/**/    {
/**/        LPCWSTR pszName;
/**/        LPCWSTR pszValue;
/**/    };
/**/    
/**/    // Following specialization of psl_data_wrapper class template is required if one or more 
/**/    // functions called via psl marshaler have parameter(s) of const Record* type.
/**/    //
/**/    // IMPORTANT: The specialization must be for exact type that is being used as a function argument.
/**/    //            For example in this case it is for const Record* because this is type of argument
/**/    //            of the function Bar. 
/**/    
/**/    template<>
/**/    class ce::psl_data_wrapper<const Record*>
/**/    {
/**/    public:
/**/        void marshal(const Record* pRecord)
/**/        {
/**/            if(pRecord)
/**/            {
/**/                m_Record = *pRecord;
/**/                m_bNull = false;
/**/            }
/**/            else
/**/                m_bNull = true;
/**/        }
/**/        
/**/        const Record* unmarshal(HANDLE hClientProcess)
/**/        {
/**/            if(!m_bNull)
/**/            {
/**/                m_Record.pszName = (LPWSTR)MapPtrToProcWithSize((LPVOID)m_Record.pszName, sizeof(*m_Record.pszName), hClientProcess);
/**/                m_Record.pszValue = (LPWSTR)MapPtrToProcWithSize((LPVOID)m_Record.pszValue, sizeof(*m_Record.pszValue), hClientProcess);
/**/
/**/                return &m_Record;
/**/            }
/**/            else
/**/                return NULL;
/**/        }
/**/
/**/    private:
/**/        Record m_Record;
/**/        bool   m_bNull; 
/**/    };
/**/
/**/
#endif

namespace ce
{

enum {INVALID_FUNCTION_ID = 0xFFFF};

//
// data wrapper for "simple" types
// 
template<typename T>
class psl_data_wrapper
{
public:
    void marshal(T x)
        {data = x; }
    
    T unmarshal(HANDLE)
        {return data; }

protected:
    T data;
};


//
// partial specialization of data wrapper for pointers
// 
template<typename T>
class psl_data_wrapper<T*>
{
public:
    void marshal(T* x)
        {data = x; }
    
    T* unmarshal(HANDLE hClientProcess)
        {return unmarshal_pointer(hClientProcess, size_of<T>::value); }
    
protected:
    T* unmarshal_pointer(HANDLE hClientProcess, int nSize)
    {
        //
        // Check whether the memory region is valid in context of the client process
        // and map the pointer for access from the server process.
        //
        return (T*)MapPtrToProcWithSize((LPVOID)data, nSize, hClientProcess);
    } 

    //
    // version of sizeof that returns 1 for type void
    //
    template<typename T>
    struct size_of
    {
        enum {value = sizeof(T)};
    };

    template<>
    struct size_of<void>
    {
        enum {value = 1};
    };

protected:
    T* data;
};

DECLARE_HANDLE(PSL_HANDLE);

//
// data wrapper specialization for PSL_HANDLE
//
template<>
class ce::psl_data_wrapper<PSL_HANDLE>
{
public:
    void marshal(PSL_HANDLE x)
        {data = x; }
    
    PSL_HANDLE unmarshal(HANDLE)
        {return data; }

private:
    PSL_HANDLE data;
};


namespace detail
{

//
// psl_message0
//
// This is minimal message send from proxy on the client side to stub on the server side.
// The message carries ID of function, handle to client process and extra info.
// The client process is the process that owns memory pointed to by any pointers parameters.
//
template<typename _extra_info_t>
class psl_message0
{
public:
    psl_message0(WORD wFunction, _extra_info_t ExtraInfo)
        : m_wFunction(wFunction)
    {
        // INVALID_FUNCTION_ID is used to indicate invalid message and shouldn't be used 
        assert(m_wFunction != INVALID_FUNCTION_ID);
        
        // Extra info is marshaled using the same mechanism as function parameters
        m_ExtraInfo.marshal(ExtraInfo);
    }
    
    WORD function() const
        {return m_wFunction; }
    
    _extra_info_t unmarshal_extra_info(HANDLE hClientProcess)
        {return m_ExtraInfo.unmarshal(hClientProcess); }
    
protected:
    WORD                            m_wFunction;
    psl_data_wrapper<_extra_info_t> m_ExtraInfo;
};


//
// psl_message1
//
template<typename _extra_info_t, typename T1>
class psl_message1 : public psl_message0<_extra_info_t>
{
public:
    typedef psl_message0<_extra_info_t> my_base;
     
    psl_message1(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1)
        : my_base(wFunction, ExtraInfo)
    {
        m_x1.marshal(arg1);
    }
    
    T1 unmarshal_arg1(HANDLE hClientProcess)
        {return m_x1.unmarshal(hClientProcess); }
        
protected:
    psl_data_wrapper<T1>   m_x1;
};


//
// psl_message2
//
template<typename _extra_info_t, typename T1, typename T2>
class psl_message2 : public psl_message1<_extra_info_t, T1>
{
public:
    typedef psl_message1<_extra_info_t, T1> my_base;
    
    psl_message2(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2)
        : my_base(wFunction, ExtraInfo, arg1)
    {
        m_x2.marshal(arg2);
    }
    
    T2 unmarshal_arg2(HANDLE hClientProcess)
        {return m_x2.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T2>   m_x2;
};


//
// psl_message3
//
template<typename _extra_info_t, typename T1, typename T2, typename T3>
class psl_message3 : public psl_message2<_extra_info_t, T1, T2>
{
public:
    typedef psl_message2<_extra_info_t, T1, T2> my_base;
    
    psl_message3(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3)
        : my_base(wFunction, ExtraInfo, arg1, arg2)
    {
        m_x3.marshal(arg3);
    }
    
    T3 unmarshal_arg3(HANDLE hClientProcess)
        {return m_x3.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T3>   m_x3;
};


//
// psl_message4
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4>
class psl_message4 : public psl_message3<_extra_info_t, T1, T2, T3>
{
public:
    typedef psl_message3<_extra_info_t, T1, T2, T3> my_base;
    
    psl_message4(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3)
    {
        m_x4.marshal(arg4);
    }
    
    T4 unmarshal_arg4(HANDLE hClientProcess)
        {return m_x4.unmarshal(hClientProcess); }
      
protected:
    psl_data_wrapper<T4>   m_x4;
};


//
// psl_message5
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5>
class psl_message5 : public psl_message4<_extra_info_t, T1, T2, T3, T4>
{
public:
    typedef psl_message4<_extra_info_t, T1, T2, T3, T4> my_base;
    
    psl_message5(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4)
    {
        m_x5.marshal(arg5);
    }
    
    T5 unmarshal_arg5(HANDLE hClientProcess)
        {return m_x5.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T5>   m_x5;
};


//
// psl_message6
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class psl_message6 : public psl_message5<_extra_info_t, T1, T2, T3, T4, T5>
{
public:
    typedef psl_message5<_extra_info_t, T1, T2, T3, T4, T5> my_base;
    
    psl_message6(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5)
    {
        m_x6.marshal(arg6);
    }
    
    T6 unmarshal_arg6(HANDLE hClientProcess)
        {return m_x6.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T6>   m_x6;
};


//
// psl_message7
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class psl_message7 : public psl_message6<_extra_info_t, T1, T2, T3, T4, T5, T6>
{
public:
    typedef psl_message6<_extra_info_t, T1, T2, T3, T4, T5, T6> my_base;
    
    psl_message7(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6)
    {
        m_x7.marshal(arg7);
    }
    
    T7 unmarshal_arg7(HANDLE hClientProcess)
        {return m_x7.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T7>   m_x7;
};


//
// psl_message8
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class psl_message8 : public psl_message7<_extra_info_t, T1, T2, T3, T4, T5, T6, T7>
{
public:
    typedef psl_message7<_extra_info_t, T1, T2, T3, T4, T5, T6, T7> my_base;
    
    psl_message8(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7)
    {
        m_x8.marshal(arg8);
    }
    
    T8 unmarshal_arg8(HANDLE hClientProcess)
        {return m_x8.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T8>   m_x8;
};


//
// psl_message9
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class psl_message9 : public psl_message8<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8>
{
public:
    typedef psl_message8<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8> my_base;
    
    psl_message9(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)
    {
        m_x9.marshal(arg9);
    }
    
    T9 unmarshal_arg9(HANDLE hClientProcess)
        {return m_x9.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T9>   m_x9;
};


//
// psl_message10
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class psl_message10 : public psl_message9<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9>
{
public:
    typedef psl_message9<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9> my_base;
    
    psl_message10(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
    {
        m_x10.marshal(arg10);
    }
    
    T10 unmarshal_arg10(HANDLE hClientProcess)
        {return m_x10.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T10>   m_x10;
};


//
// psl_message11
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
class psl_message11 : public psl_message10<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>
{
public:
    typedef psl_message10<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> my_base;
    
    psl_message11(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10, T11 arg11)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10)
    {
        m_x11.marshal(arg11);
    }
    
    T11 unmarshal_arg11(HANDLE hClientProcess)
        {return m_x11.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T11>   m_x11;
};


//
// psl_message12
//
template<typename _extra_info_t, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
class psl_message12 : public psl_message11<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>
{
public:
    typedef psl_message11<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> my_base;
    
    psl_message12(WORD wFunction, _extra_info_t ExtraInfo, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10, T11 arg11, T12 arg12)
        : my_base(wFunction, ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11)
    {
        m_x12.marshal(arg12);
    }
    
    T12 unmarshal_arg12(HANDLE hClientProcess)
        {return m_x12.unmarshal(hClientProcess); }

protected:
    psl_data_wrapper<T12>   m_x12;
};

//
// Helper that models a simple thread local storage usable within PSL call
//
template<typename T>
class psl_tls
{
    typedef hash_map<DWORD, T> MAP;
    
public:
    T* get_thread_data()
    {
        gate<critical_section> gate(cs);
        
        return map[GetCurrentThreadId()];
    }
    
    bool set_thread_data(const T& x)
    {
        gate<critical_section> gate(cs);
        
        DWORD           dwThreadId = GetCurrentThreadId();
        MAP::iterator   it;
        
        if(map.end() != (it = map.find(dwThreadId)))
        {
            *it = x;
            return true;
        }
        else
            return map.insert(dwThreadId, x);
    }
    
    void clean_thread_data()
    {
        gate<critical_section> gate(cs);
        
        map.erase(GetCurrentThreadId());
    }
    
protected:
    MAP                 map;
    critical_section    cs;
};

}; // namespace detail


//
// psl_stub
//
template <typename _extra_info_t = void*>
class psl_stub
{
public:
    typedef psl_stub<_extra_info_t> _Myt;
    typedef list<_Myt*>             psl_stub_stack;
    //
    // Singleton instance of thread local storage helper. 
    // Singleton guarantees that the instance is created in an atomic way on the first usage.
    //
    typedef singleton<detail::psl_tls<psl_stub_stack> > psl_stub_tls;

    psl_stub(LPVOID pInBuffer, DWORD nInBufferSize, HANDLE hClientProcess = GetCallerProcess())
        : m_pMessage0(NULL),
          m_hClientProcess(hClientProcess),
          m_nInBufferSize(nInBufferSize)
    {
        //
        // Static instance of singleton_lifetime assures that psl_stub_tls singleton 
        // is destroyed during module unload. singleton_lifetime ctor is thread safe.
        //
        static singleton_lifetime<psl_stub_tls> psl_tls_lifetime;
        
        //
        // Push this instance of psl_stub on the thread local stack.
        //
        if(psl_stub_stack* pPslStubStack = psl_stub_tls::instance().get_thread_data())
        {
            if(pPslStubStack->push_front(this))
            {
                //
                // Check whether the input buffer is valid in context of the caller process
                //
                if(PSLGetCallerTrust() != OEM_CERTIFY_TRUST)
                    pInBuffer = MapCallerPtr(pInBuffer, nInBufferSize);
                        
                //
                // Check if the size of input buffer is at least the size of minimal message
                //
                if(nInBufferSize >= sizeof(detail::psl_message0<_extra_info_t>))
                    m_pMessage0 = reinterpret_cast<detail::psl_message0<_extra_info_t>*>(pInBuffer);
            }
        }
    }
    
    ~psl_stub()
    {
        if(psl_stub_stack* pPslStubStack = psl_stub_tls::instance().get_thread_data())
        {
            // Pop his instance of the stub from the thread local stack
            if(!pPslStubStack->empty())
                pPslStubStack->pop_front();
        
            // Clean the thread local data if the stack is empty
            if(pPslStubStack->empty())
                psl_stub_tls::instance().clean_thread_data();
        }
    }
    
    static _Myt* get_thread_stub()
    {
        //
        // Static instance of singleton_lifetime assures that psl_stub_tls singleton 
        // is destroyed during module unload. singleton_lifetime ctor is thread safe.
        //
        static singleton_lifetime<psl_stub_tls> psl_tls_lifetime;
        
        // Return pointer to the instance of stub on top of thread local stack
        if(psl_stub_stack* pPslStubStack = psl_stub_tls::instance().get_thread_data())
        {
            if(!pPslStubStack->empty())
                return pPslStubStack->front();
        }
        
        return NULL;
    }
    
    WORD function() const
    {
        if(m_pMessage0)
            return m_pMessage0->function();
        else
            return INVALID_FUNCTION_ID;
    }
    
    bool extra_info(_extra_info_t& info) const
    {
        if(m_pMessage0)
            return NULL != (info = m_pMessage0->unmarshal_extra_info(m_hClientProcess));
        else
            return false;
    }
    
    HANDLE client_process() const
    {
        return m_hClientProcess;
    }
    
    
    // 0
    template<typename T>
    T call(T(*Fun)(void))
    {
        detail::psl_message0<_extra_info_t>* pMessage = reinterpret_cast<detail::psl_message0<_extra_info_t>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
        
        __try
        {
            return (*Fun)();
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    template<typename T, typename T0>
    T call(T(*Fun)(T0), T0 arg)
    {
        detail::psl_message0<_extra_info_t>* pMessage = reinterpret_cast<detail::psl_message0<_extra_info_t>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
        
        __try
        {
            return (*Fun)(arg);
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    
    // 1
    template<typename T, typename T1>
    T call(T(*Fun)(T1))
    {
        detail::psl_message1<_extra_info_t, T1>* pMessage = reinterpret_cast<detail::psl_message1<_extra_info_t, T1>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
        
        __try
        {
            return (*Fun)(pMessage->unmarshal_arg1(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    template<typename T, typename T0, typename T1>
    T call(T(*Fun)(T0, T1), T0 arg)
    {
        detail::psl_message1<_extra_info_t, T1>* pMessage = reinterpret_cast<detail::psl_message1<_extra_info_t, T1>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
        
        __try
        {
            return (*Fun)(arg, pMessage->unmarshal_arg1(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    
    // 2
    template<typename T, typename T1, typename T2>
    T call(T(*Fun)(T1, T2))
    {
        detail::psl_message2<_extra_info_t, T1, T2>* pMessage = reinterpret_cast<detail::psl_message2<_extra_info_t, T1, T2>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    template<typename T, typename T0, typename T1, typename T2>
    T call(T(*Fun)(T0, T1, T2), T0 arg)
    {
        detail::psl_message2<_extra_info_t, T1, T2>* pMessage = reinterpret_cast<detail::psl_message2<_extra_info_t, T1, T2>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(arg,
                          pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    
    // 3
    template<typename T, typename T1, typename T2, typename T3>
    T call(T(*Fun)(T1, T2, T3))
    {
        detail::psl_message3<_extra_info_t, T1, T2, T3>* pMessage = reinterpret_cast<detail::psl_message3<_extra_info_t, T1, T2, T3>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    template<typename T, typename T0, typename T1, typename T2, typename T3>
    T call(T(*Fun)(T0, T1, T2, T3), T0 arg)
    {
        detail::psl_message3<_extra_info_t, T1, T2, T3>* pMessage = reinterpret_cast<detail::psl_message3<_extra_info_t, T1, T2, T3>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(arg,
                          pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    
    // 4
    template<typename T, typename T1, typename T2, typename T3, typename T4>
    T call(T(*Fun)(T1, T2, T3, T4))
    {
        detail::psl_message4<_extra_info_t, T1, T2, T3, T4>* pMessage = reinterpret_cast<detail::psl_message4<_extra_info_t, T1, T2, T3, T4>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
    T call(T(*Fun)(T0, T1, T2, T3, T4), T0 arg)
    {
        detail::psl_message4<_extra_info_t, T1, T2, T3, T4>* pMessage = reinterpret_cast<detail::psl_message4<_extra_info_t, T1, T2, T3, T4>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(arg, 
                          pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    
    // 5
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5>
    T call(T(*Fun)(T1, T2, T3, T4, T5))
    {
        detail::psl_message5<_extra_info_t, T1, T2, T3, T4, T5>* pMessage = reinterpret_cast<detail::psl_message5<_extra_info_t, T1, T2, T3, T4, T5>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5), T0 arg)
    {
        detail::psl_message5<_extra_info_t, T1, T2, T3, T4, T5>* pMessage = reinterpret_cast<detail::psl_message5<_extra_info_t, T1, T2, T3, T4, T5>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(arg,
                          pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    
    // 6
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6))
    {
        detail::psl_message6<_extra_info_t, T1, T2, T3, T4, T5, T6>* pMessage = reinterpret_cast<detail::psl_message6<_extra_info_t, T1, T2, T3, T4, T5, T6>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6), T0 arg)
    {
        detail::psl_message6<_extra_info_t, T1, T2, T3, T4, T5, T6>* pMessage = reinterpret_cast<detail::psl_message6<_extra_info_t, T1, T2, T3, T4, T5, T6>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(arg,
                          pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    
    // 7
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6, T7))
    {
        detail::psl_message7<_extra_info_t, T1, T2, T3, T4, T5, T6, T7>* pMessage = reinterpret_cast<detail::psl_message7<_extra_info_t, T1, T2, T3, T4, T5, T6, T7>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess),
                          pMessage->unmarshal_arg7(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6, T7), T0 arg)
    {
        detail::psl_message7<_extra_info_t, T1, T2, T3, T4, T5, T6, T7>* pMessage = reinterpret_cast<detail::psl_message7<_extra_info_t, T1, T2, T3, T4, T5, T6, T7>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(arg, 
                          pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess),
                          pMessage->unmarshal_arg7(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    
    // 8
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6, T7, T8))
    {
        detail::psl_message8<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8>* pMessage = reinterpret_cast<detail::psl_message8<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess),
                          pMessage->unmarshal_arg7(m_hClientProcess),
                          pMessage->unmarshal_arg8(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8), T0 arg)
    {
        detail::psl_message8<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8>* pMessage = reinterpret_cast<detail::psl_message8<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(arg,
                          pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess),
                          pMessage->unmarshal_arg7(m_hClientProcess),
                          pMessage->unmarshal_arg8(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    
    // 9
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9))
    {
        detail::psl_message9<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9>* pMessage = reinterpret_cast<detail::psl_message9<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
        
        __try
        {    
            return (*Fun)(pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess),
                          pMessage->unmarshal_arg7(m_hClientProcess),
                          pMessage->unmarshal_arg8(m_hClientProcess),
                          pMessage->unmarshal_arg9(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9), T0 arg)
    {
        detail::psl_message9<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9>* pMessage = reinterpret_cast<detail::psl_message9<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
        
        __try
        {    
            return (*Fun)(arg,
                          pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess),
                          pMessage->unmarshal_arg7(m_hClientProcess),
                          pMessage->unmarshal_arg8(m_hClientProcess),
                          pMessage->unmarshal_arg9(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    
    // 10
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10))
    {
        detail::psl_message10<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>* pMessage = reinterpret_cast<detail::psl_message10<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess),
                          pMessage->unmarshal_arg7(m_hClientProcess),
                          pMessage->unmarshal_arg8(m_hClientProcess),
                          pMessage->unmarshal_arg9(m_hClientProcess),
                          pMessage->unmarshal_arg10(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10), T0 arg)
    {
        detail::psl_message10<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>* pMessage = reinterpret_cast<detail::psl_message10<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(arg,
                          pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess),
                          pMessage->unmarshal_arg7(m_hClientProcess),
                          pMessage->unmarshal_arg8(m_hClientProcess),
                          pMessage->unmarshal_arg9(m_hClientProcess),
                          pMessage->unmarshal_arg10(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    
    // 11
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11))
    {
        detail::psl_message11<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>* pMessage = reinterpret_cast<detail::psl_message11<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess),
                          pMessage->unmarshal_arg7(m_hClientProcess),
                          pMessage->unmarshal_arg8(m_hClientProcess),
                          pMessage->unmarshal_arg9(m_hClientProcess),
                          pMessage->unmarshal_arg10(m_hClientProcess),
                          pMessage->unmarshal_arg11(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11), T0 arg)
    {
        detail::psl_message11<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>* pMessage = reinterpret_cast<detail::psl_message11<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(arg,
                          pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess),
                          pMessage->unmarshal_arg7(m_hClientProcess),
                          pMessage->unmarshal_arg8(m_hClientProcess),
                          pMessage->unmarshal_arg9(m_hClientProcess),
                          pMessage->unmarshal_arg10(m_hClientProcess),
                          pMessage->unmarshal_arg11(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    
    // 12
    template<typename T, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
    T call(T(*Fun)(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12))
    {
        detail::psl_message12<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>* pMessage = reinterpret_cast<detail::psl_message12<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess),
                          pMessage->unmarshal_arg7(m_hClientProcess),
                          pMessage->unmarshal_arg8(m_hClientProcess),
                          pMessage->unmarshal_arg9(m_hClientProcess),
                          pMessage->unmarshal_arg10(m_hClientProcess),
                          pMessage->unmarshal_arg11(m_hClientProcess),
                          pMessage->unmarshal_arg12(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
    template<typename T, typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
    T call(T(*Fun)(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12), T0 arg)
    {
        detail::psl_message12<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>* pMessage = reinterpret_cast<detail::psl_message12<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>*>(m_pMessage0);
        
        if(!pMessage || m_nInBufferSize != sizeof(*pMessage))
            return error<T>(ERROR_INVALID_PARAMETER);
            
        __try
        {
            return (*Fun)(arg,
                          pMessage->unmarshal_arg1(m_hClientProcess),
                          pMessage->unmarshal_arg2(m_hClientProcess),
                          pMessage->unmarshal_arg3(m_hClientProcess),
                          pMessage->unmarshal_arg4(m_hClientProcess),
                          pMessage->unmarshal_arg5(m_hClientProcess),
                          pMessage->unmarshal_arg6(m_hClientProcess),
                          pMessage->unmarshal_arg7(m_hClientProcess),
                          pMessage->unmarshal_arg8(m_hClientProcess),
                          pMessage->unmarshal_arg9(m_hClientProcess),
                          pMessage->unmarshal_arg10(m_hClientProcess),
                          pMessage->unmarshal_arg11(m_hClientProcess),
                          pMessage->unmarshal_arg12(m_hClientProcess));
        }
        __except(EXCEPTION_EXECUTE_HANDLER)
        {
            return error<T>(ERROR_EXCEPTION_IN_SERVICE);
        }
    }
    
protected:
    // general template not defined
    template<typename T>
    T error(DWORD dwError);
    
    // specialization for DWORD
    template<>
    DWORD error<DWORD>(DWORD dwError)
        {return dwError; }
        
    // specialization for HRESULT
    template<>
    HRESULT error<HRESULT>(DWORD dwError)
        {return HRESULT_FROM_WIN32(dwError); }
    
    // specialization for BOOL
    template<>
    BOOL error<BOOL>(DWORD dwError)
    {
        SetLastError(dwError);
        return dwError == ERROR_SUCCESS;
    }
    
protected:
    detail::psl_message0<_extra_info_t>*    m_pMessage0;
    DWORD                                   m_nInBufferSize;
    HANDLE                                  m_hClientProcess;
};


//
// psl_proxy
//
template<typename _extra_info_t = void*>
class psl_proxy
{
public:
    psl_proxy(LPCTSTR pszDevice, DWORD dwIoControlCode, _extra_info_t ExtraInfo)
        : m_pszDevice(pszDevice),
          m_dwIoControlCode(dwIoControlCode),
          m_ExtraInfo(ExtraInfo),
          m_hDevice(INVALID_HANDLE_VALUE)
    {}
    
    ~psl_proxy()
    {
        if(INVALID_HANDLE_VALUE != m_hDevice)
            CloseHandle(m_hDevice);
    }
        
    DWORD call(WORD wFunction)
    {
        detail::psl_message0<_extra_info_t> message(wFunction, m_ExtraInfo);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1>
    DWORD call(WORD wFunction, T1 arg1)
    {
        detail::psl_message1<_extra_info_t, T1> message(wFunction, m_ExtraInfo, arg1);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2)
    {
        detail::psl_message2<_extra_info_t, T1, T2> message(wFunction, m_ExtraInfo, arg1, arg2);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3)
    {
        detail::psl_message3<_extra_info_t, T1, T2, T3> message(wFunction, m_ExtraInfo, arg1, arg2, arg3);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
    {
        detail::psl_message4<_extra_info_t, T1, T2, T3, T4> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
    {
        detail::psl_message5<_extra_info_t, T1, T2, T3, T4, T5> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6)
    {
        detail::psl_message6<_extra_info_t, T1, T2, T3, T4, T5, T6> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7)
    {
        detail::psl_message7<_extra_info_t, T1, T2, T3, T4, T5, T6, T7> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8)
    {
        detail::psl_message8<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9)
    {
        detail::psl_message9<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10)
    {
        detail::psl_message10<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10, T11 arg11)
    {
        detail::psl_message11<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
        
        return psl_call(&message, sizeof(message));
    };
    
    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
    DWORD call(WORD wFunction, T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5, T6 arg6, T7 arg7, T8 arg8, T9 arg9, T10 arg10, T11 arg11, T12 arg12)
    {
        detail::psl_message12<_extra_info_t, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> message(wFunction, m_ExtraInfo, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
        
        return psl_call(&message, sizeof(message));
    };

protected:
    DWORD psl_call(LPVOID pInBuffer, DWORD nInBufferSize)
    {
        if(INVALID_HANDLE_VALUE == m_hDevice)
        {
            HANDLE hDevice = CreateFile(m_pszDevice, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
            
            if(INVALID_HANDLE_VALUE != hDevice)
            {
                if(INVALID_HANDLE_VALUE != InterlockedCompareExchangePointer(&m_hDevice, hDevice, INVALID_HANDLE_VALUE))
                    CloseHandle(hDevice);
        
                assert(INVALID_HANDLE_VALUE != m_hDevice);
            }
        }
        
        if(DeviceIoControl(m_hDevice, 
                           m_dwIoControlCode, 
                           pInBuffer, 
                           nInBufferSize, 
                           NULL, 
                           0, 
                           NULL, 
                           NULL))
        {
            return ERROR_SUCCESS;
        }
        else
        {
            // Server function returned FALSE but didn't set last error
            assert(ERROR_SUCCESS != GetLastError());
            
            return GetLastError();
        }
    }
    
protected:
    HANDLE          m_hDevice;
    LPCTSTR         m_pszDevice;
    DWORD           m_dwIoControlCode;
    _extra_info_t   m_ExtraInfo;
};

}; // namespace ce

#endif // __PSL_MARSHALER__
