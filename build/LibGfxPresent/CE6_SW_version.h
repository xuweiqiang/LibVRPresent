#ifndef _CE6_SW_VERSION_H_
#define _CE6_SW_VERSION_H_

#ifdef __linux__
#define _T(a) a
#define TCHAR char
#endif

#define CI_MAJOR_VERSION_NUMBER_	_T("1")
#define CI_MINOR_VERSION_NUMBER_	_T("0")
#define CI_RELEASE_VERSION_NUMBER_	_T("0")
#define CI_BUILD_VERSION_NUMBER_	_T("1")
#define CI_VERSION_STRING_	CI_MAJOR_VERSION_NUMBER_ _T(".") CI_MINOR_VERSION_NUMBER_ _T(".")\
	CI_RELEASE_VERSION_NUMBER_ _T(".") CI_BUILD_VERSION_NUMBER_ 

#define CI_VERSION_INT_ 1,0,0,1

TCHAR* CIGetVersion()
{
	return CI_VERSION_STRING_;
}

#endif //_VERSION_H_

