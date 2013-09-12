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
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  
    CRefCon.h
Abstract:
    Reference object that inside container 
Notes: 
--*/

#ifndef __CREFCON_H_
#define __CREFCON_H_
#include <CSync.h>

class CRefObject
{
private:
    LONG   m_lRefCount;
public:
    CRefObject()
    {
        m_lRefCount = 0;
    };
    virtual ~CRefObject( void )
    {
        ;};
    DWORD AddRef( void )
    {
        return (DWORD)InterlockedIncrement(&m_lRefCount);
    };
    DWORD DeRef( void )
    {
        LONG lReturn = InterlockedDecrement(&m_lRefCount);
        if( lReturn <= 0 ) {
            delete this;
        }
        return (DWORD)lReturn;
    }
};
//
// class T have to inherit CRefObject in order to use following Template
template <class T, DWORD dwNumArray>
class CStaticContainer : public CLockObject
{
protected:
    T*      m_rgObjectArray[dwNumArray];
    DWORD   m_dwArraySize;
public:
    CStaticContainer()
    {
        m_dwArraySize = dwNumArray;
        for( DWORD dwIndex = 0; dwIndex < m_dwArraySize; dwIndex++ )
        {
            m_rgObjectArray[dwIndex] = NULL;
        }
    }
    ~CStaticContainer()
    {
        Lock();
        for( DWORD dwIndex = 0; dwIndex < m_dwArraySize; dwIndex++ )
        {
            if( m_rgObjectArray[dwIndex] != NULL )
            {
                delete m_rgObjectArray[dwIndex];
            }
        };
        Unlock();
    }
    T* InsertObject( DWORD dwIndex, T* pObject )
    {
        if( pObject )
        {
            Lock();
            T*  pReturn = NULL;
            if( dwIndex < m_dwArraySize )
                pReturn = ( ( m_rgObjectArray[dwIndex] != NULL ) ?
                            NULL :
                            ( m_rgObjectArray[dwIndex] = pObject ) );
            if( pReturn )
                pReturn->AddRef();
            Unlock();
            return pReturn;
        }
        else
        {
            return NULL;
        }
    }
    T* ObjectIndex( DWORD dwIndex )
    {
        Lock();
        T*  pReturn = dwIndex <
                      m_dwArraySize ?
                      m_rgObjectArray[dwIndex] :
                      NULL;
        if( pReturn )
        {
            pReturn->AddRef();
        }
        Unlock();
        return pReturn;
    }
    T* RemoveObjectBy( DWORD dwIndex )
    {
        Lock();
        T*  pReturn = NULL;
        if( dwIndex < m_dwArraySize )
        {
            pReturn = m_rgObjectArray[dwIndex];
            m_rgObjectArray[dwIndex] = NULL;
        };
        Unlock();
        if( pReturn )
        {
            pReturn->DeRef();
        }
        return pReturn;
    }
    T* RemoveObjectBy( T* pObject )
    {
        Lock();
        for( DWORD dwIndex = 0; dwIndex < m_dwArraySize; dwIndex++ )
        {
            if( m_rgObjectArray[dwIndex] == pObject )
            {
                m_rgObjectArray[dwIndex] = NULL;
                Unlock();
                pObject->DeRef();
                return pObject;
            };
        }
        Unlock();
        return NULL;
    }
    T* InsertObjectAtEmpty( PDWORD pdwIndex, T* pObject )
    {
        if( pObject )
        {
            Lock();
            T*  pReturn = NULL;
            for( DWORD dwIndex = 0; dwIndex < m_dwArraySize; dwIndex++ )
                if( m_rgObjectArray[dwIndex] == NULL )
                    break;
            if( dwIndex < m_dwArraySize )
            {
                pReturn = m_rgObjectArray[dwIndex] = pObject;
                pReturn->AddRef();
                if( pdwIndex )
                    *( pdwIndex ) = dwIndex;
            };
            Unlock();
            return pReturn;
        }
        else
        {
            return NULL;
        }
    }
    T* FoundObject( PDWORD pdwIndex, T* pObject )
    {
        if( pObject )
        {
            Lock();
            T*  pReturn = NULL;
            for( DWORD dwIndex = 0; dwIndex < m_dwArraySize; dwIndex++ )
                if( m_rgObjectArray[dwIndex] == pObject )
                    break;
            if( dwIndex < m_dwArraySize )
            {
                pReturn = pObject;
                pReturn->AddRef();
                if( pdwIndex )
                    *( pdwIndex ) = dwIndex;
            }
            Unlock();
            return pReturn;
        }
        else
        {
            return NULL;
        }
    }
    T* RawObjectIndex( DWORD dwIndex )
    {
        // This is only should be usded during suspend & resume.
        return ( dwIndex < m_dwArraySize ? m_rgObjectArray[dwIndex] : NULL );
    }
};

#endif
