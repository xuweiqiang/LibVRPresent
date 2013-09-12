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
//
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:  
	CSync.h
Abstract:
	Sync object.

Notes: 
--*/

#ifndef __CSYNC_H_
#define __CSYNC_H_

class CLockObject
{
private:
    CRITICAL_SECTION    m_CSection;
public:
    CLockObject()
    {
        InitializeCriticalSection( &m_CSection );
    };
    ~CLockObject()
    {
        DeleteCriticalSection( &m_CSection );
    };
    void Lock( void )
    {
        EnterCriticalSection( &m_CSection );
    };
    void Unlock( void )
    {
        LeaveCriticalSection( &m_CSection );
    };
};

class CMuTexObject
{
private:
    HANDLE  m_hObject;
public:
    CMuTexObject( BOOL bInitiallyOwn = FALSE,
                  LPCTSTR lpszName = NULL,
                  LPSECURITY_ATTRIBUTES lpsaAttribute = NULL )
    {
        m_hObject = ::CreateMutex( lpsaAttribute, bInitiallyOwn, lpszName );
        DEBUGCHK( m_hObject != NULL );
    }
    ~CMuTexObject()
    {
        if( m_hObject != NULL )
        {
            ::CloseHandle( m_hObject );
            m_hObject = NULL;
        }
    };
    BOOL Lock( DWORD dwTimeOut = INFINITE )
    {
        if( ::WaitForSingleObject( m_hObject, dwTimeOut ) == WAIT_OBJECT_0 )
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    };
    BOOL Unlock( void )
    {
        return ::ReleaseMutex( m_hObject );
    };
};


#endif
