/*------------------------------------------------------------------------
 *
 * OpenVG 1.0.1 Reference Implementation sample code
 * -------------------------------------------------
 *
 * Copyright (c) 2007 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and /or associated documentation files
 * (the "Materials "), to deal in the Materials without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Materials,
 * and to permit persons to whom the Materials are furnished to do so,
 * subject to the following conditions: 
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Materials. 
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR
 * THE USE OR OTHER DEALINGS IN THE MATERIALS.
 *
 *//**
 * \file
 * \brief	Tiger sample application. Resizing the application window
 *			rerenders the tiger in the new resolution. Pressing 1,2,3
 *			or 4 sets pixel zoom factor, mouse moves inside the zoomed
 *			image (mouse move works on OpenGL >= 1.2).
 * \note	
 *//*-------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include "windows.h"
#define UNREF(X) ((void)(X))

#ifdef HG_FLAT_INCLUDES
#	include "openvg.h"
#	include "vgu.h"
#	include "egl.h"
#else
#	include "VG/openvg.h"
#	include "VG/vgu.h"
#	include "EGL/egl.h"
#endif

#define SRC_WIDTH	320
#define SRC_HEIGHT	240
#define DST_WIDTH	320
#define DST_HEIGHT	240

static LPBYTE pBuff = NULL;
static FILE* pReadFile = NULL;

/*--------------------------------------------------------------*/
EGLDisplay			egldisplay;
EGLConfig			eglconfig;
EGLSurface			eglsurface;
EGLContext			eglcontext;

/*--------------------------------------------------------------*/
const unsigned long cimg[];

int render(int width, int height)
{
	static DWORD startTick =0; 
	static DWORD frames = 0;
	VGImage image;
	char buf[256];

	if((startTick == 0)||(frames > 50))
	{
		if(frames > 50)
			frames = 0;
		startTick = GetTickCount();
		frames++;
	}
	else
	{
		sprintf(buf, "fps:%2.2f frames/sec \n", (float)(frames++)*1000/(GetTickCount() - startTick));
		OutputDebugString(buf);
	}

	if(pReadFile == NULL)
	{
		pReadFile = fopen("test.rgb","rb");
		if(pReadFile)
			fseek(pReadFile, 0 , SEEK_SET);
	}

	if(pReadFile && (feof(pReadFile)))
		fseek(pReadFile, 0 , SEEK_SET);

	if(pReadFile)
		fread(pBuff, sizeof(BYTE),SRC_WIDTH*SRC_HEIGHT*2,pReadFile);

	image = vgCreateImage( VG_sRGB_565, SRC_WIDTH, SRC_HEIGHT, VG_IMAGE_QUALITY_BETTER );
	if(image == NULL)
		return -1;
	vgImageSubData( image, pBuff, SRC_WIDTH*2, VG_sRGB_565, 0, 0, SRC_WIDTH, SRC_HEIGHT);

	vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
	vgLoadIdentity();
	vgScale((VGfloat)width/SRC_WIDTH, (VGfloat)height/SRC_HEIGHT);

	vgTranslate(SRC_WIDTH, SRC_HEIGHT);
	vgRotate(180.0f);
	vgDrawImage( image );

	vgDestroyImage( image );

    if ( vgGetError() == VG_NO_ERROR ) 
        eglSwapBuffers( egldisplay, eglsurface );

	return 0;
}

/*--------------------------------------------------------------*/
void init(NativeWindowType window)
{
	NativePixmapType pixmap = NULL;
	static const EGLint s_configAttribs[] =
	{
		EGL_RED_SIZE,		8,
		EGL_GREEN_SIZE, 	8,
		EGL_BLUE_SIZE,		8,
		EGL_ALPHA_SIZE, 	8,
		EGL_LUMINANCE_SIZE, EGL_DONT_CARE,			//EGL_DONT_CARE
		EGL_SURFACE_TYPE,	EGL_PIXMAP_BIT ,
		EGL_SAMPLES,		1,
		EGL_NONE
	};

	static const EGLint surfAttribs[] =
    {
		EGL_COLORSPACE,		EGL_COLORSPACE_sRGB,
		EGL_ALPHA_FORMAT,	EGL_ALPHA_FORMAT_PRE,
		EGL_NONE
	};

	EGLint numconfigs;

	egldisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(egldisplay, NULL, NULL);
	assert(eglGetError() == EGL_SUCCESS);
	eglBindAPI(EGL_OPENVG_API);

	eglChooseConfig(egldisplay, s_configAttribs, &eglconfig, 1, &numconfigs);
	assert(eglGetError() == EGL_SUCCESS);
	assert(numconfigs == 1);

	eglsurface = eglCreateWindowSurface(egldisplay, eglconfig, window, surfAttribs);
	assert(eglGetError() == EGL_SUCCESS);
	eglcontext = eglCreateContext(egldisplay, eglconfig, NULL, NULL);
	assert(eglGetError() == EGL_SUCCESS);
	eglMakeCurrent(egldisplay, eglsurface, eglsurface, eglcontext);
	assert(eglGetError() == EGL_SUCCESS);

	if(pBuff == NULL)
	{
		pBuff = malloc(SRC_WIDTH*SRC_HEIGHT*2);
	}
}

/*--------------------------------------------------------------*/

void deinit(void)
{
	if(pBuff)
	{
		free(pBuff);
		pBuff = NULL;
	}

	if(pReadFile)
	{
		fclose(pReadFile);
		pReadFile = NULL;
	}

	eglMakeCurrent(egldisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	assert(eglGetError() == EGL_SUCCESS);
	eglTerminate(egldisplay);
	assert(eglGetError() == EGL_SUCCESS);
	eglReleaseThread();
}

/*--------------------------------------------------------------*/

#ifdef WIN32
#pragma warning(disable:4115)	/* named type definition in parentheses (this comes from a visual studio include file) */
#include <windows.h>

static LONG WINAPI windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		{
			RECT r;
			GetClientRect(hWnd, &r);
			return render(r.right - r.left, r.bottom - r.top);
		}
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*--------------------------------------------------------------*/

int main(void)
{
	HWND window;
	{
		WNDCLASS wndclass;
		wndclass.style		   = 0;
		wndclass.lpfnWndProc   = windowProc;
		wndclass.cbClsExtra    = 0;
		wndclass.cbWndExtra    = 0;
		wndclass.hInstance	   = (HINSTANCE)GetModuleHandle(NULL);
		wndclass.hIcon		   = LoadIcon(wndclass.hInstance, MAKEINTRESOURCE(101));
		wndclass.hCursor	   = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
		wndclass.lpszMenuName  = NULL;
		wndclass.lpszClassName = "OpenVGWndClass";
		if (!wndclass.hIcon)
			wndclass.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
		RegisterClass(&wndclass);
	}

	window = CreateWindow(
		"OpenVGWndClass",
		"OpenVG Render Sample",
		WS_OVERLAPPEDWINDOW,
		200, 200, DST_WIDTH, DST_HEIGHT,
		NULL,
		NULL,
		(HINSTANCE)GetModuleHandle(NULL),
		NULL);
	if (!window)
		return -1;

	init((NativeWindowType)window);

	{
		MSG msg;
		ShowWindow(window, SW_SHOW);
		while (GetMessage(&msg, NULL, 0, 0))
		{
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				break;
		}
	}

	deinit();

	DestroyWindow(window);
	return 0;
}

/*--------------------------------------------------------------*/

#elif defined __APPLE__

/*--------------------------------------------------------------*/

#include <OpenGL/gl.h>

//TODO

#endif

