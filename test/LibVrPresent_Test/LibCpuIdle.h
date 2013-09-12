#if !defined(__LIBCPUIDLE_H__INCLUDED_) 
 
#include "windows.h"

#define HISTORY_MAX 500
  
typedef struct _ThreadPack 
{ 
	HINSTANCE g_hInst; 
	HWND hDlg; 
	UINT nMode; 
	BOOL bEnd; 
 
	HANDLE hIdleThread; 
	 
	HWND hwndTab; 
	HWND hwndProcessList; 
	HWND hwndCpupower; 
	HWND hwndTaskList; 
	HWND hwndNetStatus;	//+NM 
	HWND hwndDskStatus;	//+NM 
 
	DWORD dwInterval; 
	HICON hIcon[12]; 
 
	char chPowHistory[HISTORY_MAX]; 
 
} ThreadPack; 


class CCPUIDLE
{
public:
	CCPUIDLE();
	virtual ~CCPUIDLE();
	BOOL Open();
	BOOL Close();
	DWORD Measure();

private:
    ThreadPack m_TP;   
    DWORD    dwLastThreadTime ;   
    DWORD    dwLastTickTime;   

};


#endif 


