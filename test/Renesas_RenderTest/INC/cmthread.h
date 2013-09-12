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
/*-------------------------------------------------------------------------------


Module Name:

cmthread.h

Description:
Declare mini thread object

-------------------------------------------------------------------------------*/
#ifndef __CMTHREAD_H_
#define __CMTHREAD_H_
class CMiniThread
{
public :
    CMiniThread( DWORD dwStackSize = 0, BOOL bSuspended = FALSE )
    {
        m_bTerminated = FALSE;
        m_dwExitCode = ( DWORD ) - 1;
        m_hThreadHandle = ::CreateThread( NULL,
                                          dwStackSize,
                                          CMiniThread::ThreadProc,
                                          ( LPVOID )this,
                                          bSuspended ? CREATE_SUSPENDED : 0,
                                          &m_dwThreadId );
        m_bSuspendFlag = bSuspended;
    };

        
    ~CMiniThread()
    {
        ForceTerminated();
    }

    BOOL    ThreadStart()
    {
        if( m_bSuspendFlag )
        {
            if( ResumeThread( m_hThreadHandle ) == ( DWORD ) ( -1 ) )
            {
                // failure for resume somehow
                return FALSE;
            };
            m_bSuspendFlag = FALSE;
        }
        return TRUE;
    }

    BOOL    ThreadStop()
    {
        if( !m_bSuspendFlag )
        {
            if( SuspendThread( m_hThreadHandle ) == ( DWORD ) ( -1 ) )
            {
                // failure
                return FALSE;
            }
            m_bSuspendFlag = TRUE;
        }
        return TRUE;
    }
    
    BOOL    CeSetPriority( int nPriority )
    {
        if( m_hThreadHandle )
        {
            return CeSetThreadPriority( m_hThreadHandle, nPriority );
        }
        return FALSE;
    }
    
    BOOL    ThreadTerminated( DWORD dwMilliSeconds )
    {
        m_bTerminated = TRUE;
        return WaitThreadComplete( dwMilliSeconds  );
    }
    
    BOOL    WaitThreadComplete( DWORD dwMilliSeconds )
    {
        if( !m_hThreadHandle )
        {
            return FALSE;
        };

        if( ThreadStart() )
        {
            if( ::WaitForSingleObject( m_hThreadHandle, dwMilliSeconds ) ==
                WAIT_OBJECT_0 )
            {
                // thread dead
                ::CloseHandle( m_hThreadHandle );
                m_hThreadHandle = NULL;
                return TRUE;
            }
        }
        return FALSE;
    }

    BOOL    ForceTerminated()
    {
        if( m_hThreadHandle )
        {
//#pragma prefast(suppress: 258, "Try to recover from a pathological failure")
            BOOL bReturn = ::TerminateThread( m_hThreadHandle, ( DWORD ) - 1 ); // terminate abnormal
            m_dwExitCode = -1;
            ::CloseHandle( m_hThreadHandle );
            m_hThreadHandle = NULL;
            return bReturn;
        };
        return TRUE;
    }

    DWORD GetThreadId()
    {
        return m_dwThreadId;
    };
    BOOL IsTerminated()
    {
        return m_bTerminated;
    };
    HANDLE GetThreadHandle()
    {
        return m_hThreadHandle;
    };
    BOOL GetExitCodeThread( LPDWORD lpExitCode )
    {
        if( !m_hThreadHandle )
        {
            *lpExitCode = m_dwExitCode;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    };
private:
    virtual DWORD       ThreadRun() = 0; // User have to implement this function.
    
    static DWORD WINAPI ThreadProc( LPVOID dParam )
    {
        CMiniThread* pThreadPtr = ( CMiniThread* ) dParam;
        pThreadPtr->m_dwExitCode = pThreadPtr->ThreadRun();
        ::ExitThread( pThreadPtr->m_dwExitCode );
        return pThreadPtr->m_dwExitCode;
    };
    
protected:
    BOOL                m_bTerminated;
private:
    HANDLE              m_hThreadHandle;
    DWORD               m_dwThreadId;
    BOOL                m_bSuspendFlag;
    DWORD               m_dwExitCode;
};

#endif

