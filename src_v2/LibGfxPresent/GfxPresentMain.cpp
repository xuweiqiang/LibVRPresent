//===================================== ========================================
//  CIDANA CONFIDENTIAL INFORMATION
//
//	THIS SOURCE CODE IS PROPRIETARY INFORMATION BELONGING TO CIDANA, INC.
// 	ANY USE INCLUDING BUT NOT LIMITED TO COPYING OF CODE, CONCEPTS, AND/OR
//	ALGORITHMS IS PROHIBITED EXCEPT WITH EXPRESS WRITTEN PERMISSION BY THE 
//	COMPANY.
//
// 	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
// 	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// 	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// 	PURPOSE.
//
// 	Copyright (c) 2008  Cidana, Inc.  All Rights Reserved.
//
//-----------------------------------------------------------------------------

#include "../../inc/GfxBuildDef.h"


#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0502		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.  
#define _WIN32_WINNT 0x0502	// Change this to the appropriate value to target other versions of Windows.
#endif			

#include <stdio.h>
#include <windows.h>
#include "../../inc/GfxPresent.h"
#include "../../inc/McCom.h"
#include "../../inc/dp.h"
#include "tchar.h"
#include "GfxPresentProcSW.h"
#include "GfxPresentProcEbd.h"
#include "GfxPresentProcEbdSW.h"
#ifndef _WIN32_WCE
#ifndef __linux__
#include "GfxPresentProcOpenGL.h"
#endif
#endif

#ifdef ANDROID_VM
#include "GfxPresentProcAndroid.h"
#endif


#ifdef ANDROID_NDK
#include <cpu-features.h>

bool hasNeon()
{
    AndroidCpuFamily family = android_getCpuFamily();
    switch (family) {
       case ANDROID_CPU_FAMILY_ARM:
    	   DP("CPU family is ARM\n");
           break;
       case ANDROID_CPU_FAMILY_X86:
    	   DP("CPU family is x86\n");
           break;
       case ANDROID_CPU_FAMILY_MIPS:
    	   DP("CPU family is MIPS\n");
           break;
       default:
    	   DP("Unsupported CPU family: %d\n", family);
           return 0;
       }
    	if (family == ANDROID_CPU_FAMILY_ARM) {
           uint64_t features = android_getCpuFeatures();
           DP( "Supported ARM features:\n");
           if ((features & ANDROID_CPU_ARM_FEATURE_ARMv7) != 0) {
               DP( "  ARMv7\n" );
           }
           if ((features & ANDROID_CPU_ARM_FEATURE_VFPv3) != 0) {
        	   DP( "  VFPv3\n" );
           }
           if ((features & ANDROID_CPU_ARM_FEATURE_NEON) != 0) {
        	   DP( "  NEON\n" );
               return 1;
           }
           if ((features & ANDROID_CPU_ARM_FEATURE_LDREX_STREX) != 0) {
        	   DP( "  ldrex/strex\n" );
           }
       }
    	return 0;
}

#endif
enum
{
	eOS_WIN2000_HOME = 0x1,
	eOS_WIN2000_SRV,
	eOS_WIN2000_PROF,
	eOS_WIN2000_DATASRV,
	eOS_WIN2000_ADVSRV,

	eOS_WINSRV_2003,
	eOS_WINSRV_2003_R2 ,
	eOS_WIN_STORAGE_SRV_2003 ,
	eOS_WIN_HOME_SRV ,
	eOS_WINSRV_2008 ,
	eOS_WINSRV_2008_R2 ,

	eOS_WINXP_HOME,
	eOS_WINXP_PROF ,
	eOS_WINXP_PROF_X64 ,

	eOS_VISTA ,
	eOS_WIN7 ,
};

enum
{
	eOS_X32bit = 0x1<<6,
	eOS_X64bit= 0x1<<7,
};

#ifndef _WIN32_WCE
#ifndef __linux__
static BOOL  GetOSVersion(DWORD *pOs)
{
	if(!pOs)	return FALSE;

	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if(!GetVersionEx((LPOSVERSIONINFOW)&osvi))
		return FALSE;
	if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && 
		osvi.dwMajorVersion > 4 )
	{

		if ( osvi.dwMajorVersion == 6 )
		{
			if( osvi.dwMinorVersion == 0 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					*pOs = eOS_VISTA;
				else 
					*pOs = eOS_WINSRV_2008;
			}

			if ( osvi.dwMinorVersion == 1 )
			{
			if( osvi.wProductType == VER_NT_WORKSTATION )
				*pOs = eOS_WIN7;
			else 
				*pOs = eOS_WINSRV_2008_R2;
			}

		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		{
#ifdef WIN7_SDK
			if( GetSystemMetrics(SM_SERVERR2) )
				*pOs = eOS_WINSRV_2003_R2;
			else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
				*pOs = eOS_WINSRV_2003;
			else if ( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
				*pOs = eOS_WIN_STORAGE_SRV_2003;
			else if( osvi.wProductType == VER_NT_WORKSTATION &&
				si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
				*pOs = eOS_WINXP_PROF_X64;
			else 
				*pOs = eOS_WINSRV_2003;
#endif
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{
			if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
				*pOs = eOS_WINXP_HOME;
			else 
				*pOs = eOS_WINXP_PROF;
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		{
			if ( osvi.wProductType == VER_NT_WORKSTATION )
			{
				*pOs = eOS_WIN2000_PROF;
			}
			else 
			{
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					*pOs = eOS_WIN2000_DATASRV;
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					*pOs = eOS_WIN2000_ADVSRV;
				else 
					*pOs = eOS_WIN2000_SRV;
			}
		}

		// Include service pack (if any) and build number.

		if ( osvi.dwMajorVersion >= 6 )
		{
			if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				*pOs |= eOS_X64bit;
			else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
				*pOs |= eOS_X32bit;
		}

		return TRUE; 
	}

	return FALSE;
}
#endif
#endif


HRESULT ConvertPrecision(LONG& lSrcVal, const LONG lDstMax, const LONG lDstMin, const LONG lSrcMax,const LONG lSrcMin)
{
	LONG lSrcNum = lSrcMax - lSrcMin;
	LONG lDstNum = lDstMax - lDstMin;

	if(lSrcVal>lSrcMax)
		lSrcVal = lDstMax;
	else if(lSrcVal < lSrcMin)
		lSrcVal = lDstMin;
	else		
		lSrcVal = lDstMin + (lSrcVal-lSrcMin)*lDstNum/lSrcNum;
	return S_OK;
}

RECT ScaleRectangle(const RECT& input, const RECT& src, const RECT& dst)
{
    RECT rect;

    UINT src_dx = src.right - src.left;
    UINT src_dy = src.bottom - src.top;

    UINT dst_dx = dst.right - dst.left;
    UINT dst_dy = dst.bottom - dst.top;

    // Scale the input rectangle by dst / src.

    rect.left   = input.left   * dst_dx / src_dx;
    rect.right  = input.right  * dst_dx / src_dx;
    rect.top    = input.top    * dst_dy / src_dy;
    rect.bottom = input.bottom * dst_dy / src_dy;

    return rect;
}

STDMETHODIMP LoadV1Library(HMODULE& h_dll, IMcVideoPresenter** ppPresenter)
{
#ifndef ANDROID
#ifndef __linux__
	HRESULT hr = S_OK;

#ifdef LOADLIBRARY_ONCE
	if(h_dll  == 0)
#endif
	h_dll = LoadLibrary(LIB_NAME_V1); 
	if(h_dll == NULL)
		return E_FAIL;

	if(ppPresenter)
	{
		fpCreateVideoPresenter fpProcCreate;
		fpProcCreate = (fpCreateVideoPresenter)GetProcAddress(h_dll, ENTRY_FUNC_NAME_V1);
		if(fpProcCreate==0)
			{hr = E_FAIL; goto RET;}
		*ppPresenter = fpProcCreate();
		if(*ppPresenter ==0)
			{hr = E_FAIL; goto RET;}
		return S_OK;
	RET:
		FreeLibrary(h_dll);
		return hr;

	}
#endif
#endif
	return S_OK;
}

STDMETHODIMP UnLoadV1Library(HMODULE& h_dll, IMcVideoPresenter** ppPresenter)
{
	if(ppPresenter && *ppPresenter)
	{
		IMcVideoPresenter* pPresent = *ppPresenter;
		pPresent->Close();
		pPresent->Release();
	}

#ifndef LOADLIBRARY_ONCE
	FreeLibrary(h_dll);
#endif
	return S_OK;
}

STDMETHODIMP LoadDXVALibrary(HMODULE* pDll, IMcVideoPresenterAdv** ppPresenter)
{
	if(!pDll || !ppPresenter)	return E_FAIL;
	HRESULT hr = S_OK;
	*pDll = LoadLibrary(LIB_NAME_DXVA); 
	if(*pDll == NULL)
		return E_FAIL;

	if(ppPresenter)
	{
		fpCreateVideoPresenterAdv fpProcCreate;
		fpProcCreate = (fpCreateVideoPresenterAdv)GetProcAddress(*pDll, ENTRY_FUNC_NAME_V2);
		if(fpProcCreate==0)
			{hr = E_FAIL; goto RET;}
		*ppPresenter = fpProcCreate();
		if(*ppPresenter ==0)
			{hr = E_FAIL; goto RET;}
		return S_OK;
	RET:
		FreeLibrary(*pDll);
		return hr;

	}
	return S_OK;

}

#ifndef __linux__
IMcVideoPresenter* CreateVideoPresenter()
{
	HMODULE h_dll;
	if(FAILED(LoadV1Library(h_dll, NULL)))
		return NULL;

	fpCreateVideoPresenter fpProcCreate;
	fpProcCreate = (fpCreateVideoPresenter)GetProcAddress(h_dll, ENTRY_FUNC_NAME_V1);
	if(fpProcCreate==0)
	{
		return NULL;
	}
   	DP("%s: %s \n",__FILE__, __FUNCTION__);
	return (fpProcCreate());
}
#endif
IMcVideoPresenterAdv* CreateVideoPresenterAdv()
{
	IMcVideoPresenterAdv* pObj = NULL;
#ifdef _WIN32_WCE
	#if defined(TITAN_II) || defined(MT3351_DBG) || defined(MT3353) || defined(PRIMAII)	\
	|| defined(SIRF_A4_NEWOS) || defined(VNCVIEWER) || defined(IEC) ||defined(GFX_MSTAR)
		pObj = new CVRPresentProcessorEbd();
    	DP("%s new :CVRPresentProcessorEbd\n", __FUNCTION__);
	#else
		#if 0// defined(SIRF_A4_NEWOS) 
		pObj = new CVRPresentOvlProcessorEbdSW();
    	DP("%s new :CVRPresentOvlProcessorEbdSW\n", __FUNCTION__);
		#else
		pObj = new CVRPresentOvlProcessorSW();
    	DP("%s new :CVRPresentOvlProcessorSW\n", __FUNCTION__);
		#endif
	#endif

#else

	#ifdef VM_OPENGL
	pObj = new CVRPresentProcOpenGL();
	DP("%s new :CVRPresentProcOpenGL\n", __FUNCTION__);

   #elif ANDROID_VM
	pObj = new CVRProcAndroid();
	DP("%s new :CVRProcAndroid \n", __FUNCTION__);

	#elif defined(ANDROID22) ||defined(ANDROID23)
	    #ifdef ONLYONE_SURF
		pObj = new CVRPresentOvlProcessorEbdSW();
    	DP("%s new :CVRPresentOvlProcessorEbdSW\n", __FUNCTION__);
    	#else
    	pObj = new CVRPresentOvlProcessorSW();
    	DP("%s new :CVRPresentOvlProcessorSW \n", __FUNCTION__);
    	#endif
	#else
	//DWORD osVersion;
	//if(GetOSVersion(&osVersion))
	//{
	//	if(osVersion >= eOS_WIN7)
	//	{
	//		//Try DXVA
	//		HMODULE dll;
	//		HRESULT hr = LoadDXVALibrary(&dll, &pObj);
	//		if(SUCCEEDED(hr))	
	//		{
	//			HWND hwnd = CreateWindow(_T("BUTTON"), _T("D3D in Cidana!"), 
	//				/*WS_OVERLAPPEDWINDOW*/WS_EX_OVERLAPPEDWINDOW, 0, 0, 320, 240,
	//				NULL, NULL, NULL, NULL);


	//			VP_OpenOptionsAdv  params;
	//			memset(&params, 0 ,sizeof(params));
	//			params.canvasWidth = 720;
	//			params.canvasHeight = 576;
	//			params.hWnd = hwnd;
	//			hr = pObj->Open(&params,sizeof(params));
	//			if(SUCCEEDED(hr))	
	//			{
	//				pObj->Close();
	//				DestroyWindow(hwnd);
	//				return pObj;
	//			}
	//			else
	//				FreeLibrary(dll);
	//			DP("DXVA initilization failed \n");
	//	}
	//	}
	//}

	//Use SW mode
	pObj = new CVRPresentOvlProcessorSW();
	DP("%s new :CVRPresentOvlProcessorSW \n", __FUNCTION__);
	#endif
#endif

	return pObj;
}

#ifdef _MANAGED
#pragma managed(push, off)
#endif

static TCHAR g_szModulePath[MAX_PATH+1];  // present only for WinCE

#ifndef ARRAYSIZE
#define ARRAYSIZE(a)	 sizeof(a)/sizeof(a[0]) 
#endif
static BOOL UpdateDllDirectory(HMODULE hLib)
{
	TCHAR *tp;
#ifndef __linux__
	if(GetModuleFileName(hLib,g_szModulePath,ARRAYSIZE(g_szModulePath))==0)
		return FALSE;
#endif
	if((tp = _tcsrchr(g_szModulePath,_T('\\')))==NULL &&
		(tp = _tcsrchr(g_szModulePath,_T('/')))==NULL)
	{
		g_szModulePath[0] = 0;
		return FALSE;
	}
	tp[1] = 0;
#if defined(WINCE) || defined(__linux__)
	return TRUE;
#else
	return SetDllDirectory(g_szModulePath);
#endif
}

#if defined(WINCE)
BOOL APIENTRY DllMain( HANDLE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
#else
BOOL APIENTRY DllMain( HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					  )
#endif
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		UpdateDllDirectory((HMODULE)hModule); // adds current dll directory to dll search path
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

