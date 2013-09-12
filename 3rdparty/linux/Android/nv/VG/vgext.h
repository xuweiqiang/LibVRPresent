/*
 * Copyright (c) 2007 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef _VGEXT_H
#define _VGEXT_H

#if !defined(_OPENVG_H)
#   include <VG/openvg.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Available extensions.
 */

#define OVG_KHR_egl_image       1
/*#define OVG_KHR_drawable_object 1*/
/*#define OVG_NV_open_subpath     1*/

/*
 * OVG_KHR_egl_image
 */

#if defined(OVG_KHR_egl_image)

typedef void *VGeglImageKHR;

VG_API_CALL VGImage VG_APIENTRY
    vgCreateEGLImageTargetKHR(VGeglImageKHR image) VG_APIEXIT;

#endif

/*
 * OVG_KHR_drawable_object
 */

#if defined(OVG_KHR_drawable_object)

typedef VGHandle VGDrawableKHR;
#define VG_UNSUPPORTED_DRAWABLE_FORMAT_ERROR_KHR    ((VGErrorCode)0x1008)
#define VG_DEFAULT_DRAWABLE_KHR                     ((VGHandle)1)

typedef enum {
    VG_DRAWABLE_ATTACHMENT_COLOR_KHR    = 0x2400,
    VG_DRAWABLE_ATTACHMENT_MASK_KHR     = 0x2401
} VGDrawableAttachmentKHR;

VG_API_CALL VGDrawableKHR VG_APIENTRY
    vgCreateDrawableKHR(VGint count,
        const VGDrawableAttachmentKHR* attachments,
        const VGImage* images) VG_APIEXIT;
VG_API_CALL void VG_APIENTRY
    vgDestroyDrawableKHR(VGDrawableKHR drawable) VG_APIEXIT;
VG_API_CALL VGboolean VG_APIENTRY
    vgValidDrawableFormatKHR(VGint count,
        const VGDrawableAttachmentKHR * attachments, 
        const VGImageFormat * formats) VG_APIEXIT;
VG_API_CALL void VG_APIENTRY
    vgBindDrawableKHR(VGDrawableKHR drawable) VG_APIEXIT;
VG_API_CALL VGDrawableKHR VG_APIENTRY
    vgGetBoundDrawableKHR(void) VG_APIEXIT;
VG_API_CALL VGImage VG_APIENTRY
    vgGetDrawableAttachmentKHR(VGDrawableKHR drawable,
        VGDrawableAttachmentKHR attachment) VG_APIEXIT;

#endif

/*
 * OVG_NV_open_subpath
 */

#if defined(OVG_NV_open_subpath)

#define VG_CLOSE_PATHS      ((VGParamType)0x1107)
#define VG_VALIDATE_PATHS   ((VGParamType)0x1108)

#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* _VGEXT_H */
