#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include "Library_probe.h"


CLibLoad::CLibLoad():
m_hDll(NULL),
m_dwFileIndex(0)
{
}

CLibLoad::~CLibLoad()
{
	Uninit();
}

STDMETHODIMP CLibLoad::Reset()
{
	Uninit();
	return S_OK;
}

STDMETHODIMP CLibLoad::Init(TCHAR* Prefix)
{
    memset(m_szDllPrefix, 0, sizeof(m_szDllPrefix));
    if(Prefix)
    {
        _tcsncpy(m_szDllPrefix, Prefix, sizeof(m_szDllPrefix) / 2 - 1);
    }
	return S_OK;
}

STDMETHODIMP CLibLoad::Uninit()
{
	m_dwFileIndex = 0;
	if(m_hDll == NULL)
		return S_OK;
	FreeLibrary(m_hDll);
	m_hDll = NULL;
	return S_OK;
}

STDMETHODIMP CLibLoad::Probe(void** Proc, TCHAR* FuncName)
{
    WIN32_FIND_DATA FindData = { 0 };
	HANDLE hFileSearch = NULL;
    TCHAR  FileFilter[MAX_PATH] = {0};
	DWORD  dwCurrentFileIndex = 0;

	if((Proc==NULL) || (FuncName == NULL))
		return E_INVALIDARG;

    if(m_hDll)
    {
//         FreeLibrary(m_hDll);
//         m_hDll = NULL;
		goto ADD_FUNCTION;
    }

	// Get current path
	GetModuleFileName(NULL, FileFilter, MAX_PATH);
	TCHAR* p = _tcsrchr(FileFilter, '\\');
    if(p)
    {
	    p++;
	    p[0] = 0;
    }
    _tcscat(FileFilter, m_szDllPrefix);
    _tcscat(FileFilter, _T("*.dll"));

    // Search DLL
	hFileSearch = FindFirstFile(FileFilter, &FindData);
	if(hFileSearch == INVALID_HANDLE_VALUE)  // no Tuners
	{
		return E_FAIL;
	}

    // Try load DLL
	do 
	{
		if(m_dwFileIndex == dwCurrentFileIndex)
		{
			m_dwFileIndex ++;
			m_hDll = LoadLibrary(FindData.cFileName);
			if(m_hDll)
            {
ADD_FUNCTION:
#if defined(_WIN32) && !defined(_WIN32_WCE) && defined(UNICODE)
                size_t Len = _tcslen(FuncName) + 1;
                char *pFuncName = new char[Len];
                wcstombs(pFuncName, FuncName, Len);
                *Proc = GetProcAddress(m_hDll, pFuncName);
                delete [] pFuncName;
#else
                *Proc = GetProcAddress(m_hDll, FuncName);
#endif
				if(*Proc == NULL)
                {
                    FreeLibrary(m_hDll);
                    m_hDll = NULL;
				}
				else
				{
					break;
				}
			}
		}

		dwCurrentFileIndex++;

	} while(FindNextFile(hFileSearch, &FindData)&&(*Proc==NULL));

	if (hFileSearch)
	{
		FindClose(hFileSearch);
	}

	if(*Proc)
		return S_OK;
	else
		return E_FAIL;
}
