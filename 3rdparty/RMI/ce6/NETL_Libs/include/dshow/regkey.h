//
// regkey.h
//
// Simple registry access
//
// Copyright GDCL 2008. Freely reusable.


// simple reading of registry keys
class RegKey
{
public:
    RegKey()
    : m_hkey(NULL)
    {
    }
    ~RegKey()
    {
        if (m_hkey != NULL)
        {
            RegCloseKey(m_hkey);
        }
    }

    operator HKEY ()
    {
        return m_hkey;
    }

    bool Open(HKEY hkey, const TCHAR* pszSubkey)
    {
        if (m_hkey != NULL)
        {
            RegCloseKey(m_hkey);
        }
        long e = RegOpenKeyEx(hkey, pszSubkey, 0, KEY_READ|KEY_SET_VALUE, &m_hkey);
        if (e != ERROR_SUCCESS)
        {
            return false;
        }
        return true;
    }
    bool Create(HKEY hkey, const TCHAR* pszSubkey)
    {
        if (m_hkey != NULL)
        {
            RegCloseKey(m_hkey);
        }
        DWORD dwDisposition;
        long e = RegCreateKeyEx(hkey, pszSubkey, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &m_hkey, &dwDisposition);
        if (e != ERROR_SUCCESS)
        {
            return false;
        }
        return true;
    }

    DWORD NumericValue(const TCHAR* pszValue, DWORD dwDefault)
    {
        DWORD dwReturn = dwDefault;
        DWORD dw;
        DWORD dwType;
        DWORD cLen = sizeof(dw);
        long e = RegQueryValueEx(m_hkey, pszValue, NULL, &dwType, (BYTE*)&dw, &cLen);
        if ((e == ERROR_SUCCESS) && (dwType == REG_DWORD) && (cLen == sizeof(dw)))
        {
            dwReturn = dw;
        }
        return dwReturn;
    }
	void WriteString(const TCHAR* pName, const TCHAR* pValue)
	{
		RegSetValueEx(m_hkey, pName, 0, REG_SZ, (const BYTE*)pValue,  lstrlen(pValue) * sizeof(TCHAR));
	}
private:
    HKEY m_hkey;
};

