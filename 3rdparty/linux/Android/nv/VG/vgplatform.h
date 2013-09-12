/*
 * Copyright (c) 2007 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */


#ifndef _VGPLATFORM_H
#define _VGPLATFORM_H

/* Definitions of the OpenVG and VGU API call macros. */

#if !defined(VG_API_CALL)
#   define VG_API_CALL extern
#endif

#if !defined(VG_APIENTRY)
#   define VG_APIENTRY
#endif

#if !defined(VG_APIEXIT)
#   if defined (__ARMCC_2__)
#       define VG_APIEXIT __softfp
#   else
#       define VG_APIEXIT
#   endif
#endif

#if !defined(VGU_API_CALL)
#   define VGU_API_CALL VG_API_CALL
#endif

#if !defined(VGU_APIENTRY)
#   define VGU_APIENTRY VG_APIENTRY
#endif

#if !defined(VGU_APIEXIT)
#   define VGU_APIEXIT VG_APIEXIT
#endif

#endif /* _VGPLATFORM_H */
