#ifndef _LIBRARYPROBE_H_
#define _LIBRARYPROBE_H_

class CLibLoad
{
public:
	CLibLoad();
	virtual ~CLibLoad();
	STDMETHODIMP Init(TCHAR* Prefix);
	STDMETHODIMP Uninit();
	STDMETHODIMP Reset();
	STDMETHODIMP Probe(void** proc, TCHAR* funcName);

protected:
	HMODULE         m_hDll;
	DWORD           m_dwFileIndex;
    TCHAR           m_szDllPrefix[21];
};

#endif
