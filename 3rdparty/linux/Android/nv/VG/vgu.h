/* 
* Copyright (c) 2007 The Khronos Group Inc. 
* 
* Permission is hereby granted, free of charge, to any person obtaining 
* a copy of this software and associated documentation files (the 
* "Software"), to deal in the Software without restriction, including 
* without limitation the rights to use, copy, modify, merge, publish, 
* distribute, sublicense, and/or sell copies of the Software, and to 
* permit persons to whom the Software is furnished to do so, subject 
* to the following conditions: 
* The above copyright notice and this permission notice shall be included 
* in all copies or substantial portions of the Software. 
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY 
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
* 
*/

/*
 * Copyright (c) 2007 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef _VGU_H
#define _VGU_H

#ifdef __cplusplus
extern "C" {
#endif

#include <VG/openvg.h>

#define VGU_VERSION_1_0 1
#define VGU_VERSION_1_1 2

#ifndef VGU_API_CALL
#define VGU_API_CALL extern
#endif
#ifndef VGU_APIENTRY
#define VGU_APIENTRY /* nothing */
#endif
#ifndef VGU_APIEXIT
#define VGU_APIEXIT /* nothing */
#endif

typedef enum {
  VGU_NO_ERROR                                 = 0,
  VGU_BAD_HANDLE_ERROR                         = 0xF000,
  VGU_ILLEGAL_ARGUMENT_ERROR                   = 0xF001,
  VGU_OUT_OF_MEMORY_ERROR                      = 0xF002,
  VGU_PATH_CAPABILITY_ERROR                    = 0xF003,
  VGU_BAD_WARP_ERROR                           = 0xF004
} VGUErrorCode;

typedef enum {
  VGU_ARC_OPEN                                 = 0xF100,
  VGU_ARC_CHORD                                = 0xF101,
  VGU_ARC_PIE                                  = 0xF102
} VGUArcType;

VGU_API_CALL VGUErrorCode VGU_APIENTRY
     vguLine(VGPath path,
        VGfloat x0, VGfloat y0,
        VGfloat x1, VGfloat y1) VGU_APIEXIT;

VGU_API_CALL VGUErrorCode VGU_APIENTRY
     vguPolygon(VGPath path,
        const VGfloat * points,
        VGint count,
        VGboolean closed) VGU_APIEXIT;

VGU_API_CALL VGUErrorCode VGU_APIENTRY
     vguRect(VGPath path,
        VGfloat x, VGfloat y,
        VGfloat width, VGfloat height) VG_APIEXIT;

VGU_API_CALL VGUErrorCode VGU_APIENTRY
     vguRoundRect(VGPath path,
        VGfloat x, VGfloat y,
        VGfloat width,
        VGfloat height,
        VGfloat arcWidth,
        VGfloat arcHeight) VGU_APIEXIT;

VGU_API_CALL VGUErrorCode VGU_APIENTRY
     vguEllipse(VGPath path,
        VGfloat cx, VGfloat cy,
        VGfloat width,
        VGfloat height) VGU_APIEXIT;

VGU_API_CALL VGUErrorCode VGU_APIENTRY
     vguArc(VGPath path,
        VGfloat x, VGfloat y,
        VGfloat width, VGfloat height,
        VGfloat startAngle,
        VGfloat angleExtent,
        VGUArcType arcType) VGU_APIEXIT;

VGU_API_CALL VGUErrorCode VGU_APIENTRY
    vguComputeWarpQuadToSquare(VGfloat sx0, VGfloat sy0,
        VGfloat sx1, VGfloat sy1,
        VGfloat sx2, VGfloat sy2,
        VGfloat sx3, VGfloat sy3,
        VGfloat * matrix) VGU_APIEXIT;

VGU_API_CALL VGUErrorCode VGU_APIENTRY
    vguComputeWarpSquareToQuad(VGfloat dx0, VGfloat dy0,
        VGfloat dx1, VGfloat dy1,
        VGfloat dx2, VGfloat dy2,
        VGfloat dx3, VGfloat dy3,
        VGfloat * matrix) VGU_APIEXIT;

VGU_API_CALL VGUErrorCode VGU_APIENTRY
    vguComputeWarpQuadToQuad(VGfloat dx0, VGfloat dy0,
        VGfloat dx1, VGfloat dy1,
        VGfloat dx2, VGfloat dy2,
        VGfloat dx3, VGfloat dy3,
        VGfloat sx0, VGfloat sy0,
        VGfloat sx1, VGfloat sy1,
        VGfloat sx2, VGfloat sy2,
        VGfloat sx3, VGfloat sy3,
        VGfloat * matrix) VGU_APIEXIT;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* #ifndef _VGU_H */
