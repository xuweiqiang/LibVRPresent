/*
 * Copyright (c) 2007 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef EGLPLATFORM_H
#define EGLPLATFORM_H

/*
** eglplatform.h is platform dependent. It defines:
**
**     - Native types
**     - EGL and native handle values
**
** EGLNativeDisplayType, EGLNativeWindowType and EGLNativePixmapType are to be
** replaced with corresponding types of the native window system in egl.h.
**
** EGL and native handle values must match their types.
**
** This version of eglplatform.h is for Microsoft Windows*
**
*/

#ifdef __cplusplus
extern "C" {
#endif

// Operating system check
#if !defined(_WIN32)
#   error "Windows only available on WIN32 systems"
#endif

// System headers/types
#undef  WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#ifdef __cplusplus
    }
    #include <windows.h>
    extern "C" {
#else
    #include <windows.h>
#endif

// Define storage class specifiers
#ifndef APIENTRY
#define APIENTRY
#endif

#ifndef EGLAPIENTRY
#define EGLAPIENTRY 
#endif
#ifndef EGLAPIENTRYP
#define EGLAPIENTRYP EGLAPIENTRY *
#endif

#ifdef __GL_EXPORTS
#   define EGLAPI
#else
#   define EGLAPI __declspec(dllimport)
#endif

// Define native window system types
typedef HDC EGLNativeDisplayType;
typedef HWND EGLNativeWindowType;
typedef HDC EGLNativePixmapType;

// Define 64-bit integer extensions
typedef signed __int64   EGLint64NV;
typedef unsigned __int64 EGLuint64NV;

// Define the pre-EGL 1.3 Native handle types for backwards compatibility
typedef EGLNativeDisplayType NativeDisplayType;
typedef EGLNativePixmapType  NativePixmapType;
typedef EGLNativeWindowType  NativeWindowType;

// stdint.h doesn't exist, but EGL 1.4 headers use int32_t, so define it here
typedef __int32 int32_t;
          
#ifdef __cplusplus
}
#endif

#endif /* EGLPLATFORM_H */
