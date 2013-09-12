// VideoMixer.cpp : Defines the entry point for the DLL application.
//

//#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <../../inc/McCom.h>
#include "../../inc/GfxPresent.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern ICivmVideoMixer *CreateVideoMixerOpenGL();

/*BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}*/

extern ICivmVideoMixer *CreateVideoMixerOpenGL();

ICivmVideoMixer *CreateVideoMixerPRI(E_MIXERTYPE type)
{
	switch(type)
	{
		//	case eWND:
		//		return CreateVideoMixerWnd(); 
#ifdef VM_SUPPORT
	case eMIXERTYPE_OPENGL:
		return CreateVideoMixerOpenGL(); 
#endif
	case eMIXERTYPE_WND:
	default:
		break;
	}

	return NULL;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

