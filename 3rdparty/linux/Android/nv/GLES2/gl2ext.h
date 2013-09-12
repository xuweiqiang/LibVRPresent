/*
 * Copyright (c) 2005-2007 NVIDIA Corporation.  All rights reserved.
 *
 * NVIDIA Corporation and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA Corporation is strictly prohibited.
 */

#ifndef __gl2ext_h_
#define __gl2ext_h_

/* $Id$ */

#ifdef __cplusplus
extern "C" {
#endif

/*
** License Applicability. Except to the extent portions of this file are
** made subject to an alternative license as permitted in the SGI Free
** Software License B, Version 1.0 (the "License"), the contents of this
** file are subject only to the provisions of the License. You may not use
** this file except in compliance with the License. You may obtain a copy
** of the License at Silicon Graphics, Inc., attn: Legal Services, 1600
** Amphitheatre Parkway, Mountain View, CA 94043-1351, or at:
**
** http://oss.sgi.com/projects/FreeB
**
** Note that, as provided in the License, the Software is distributed on an
** "AS IS" basis, with ALL EXPRESS AND IMPLIED WARRANTIES AND CONDITIONS
** DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES AND
** CONDITIONS OF MERCHANTABILITY, SATISFACTORY QUALITY, FITNESS FOR A
** PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
**
** Original Code. The Original Code is: OpenGL Sample Implementation,
** Version 1.2.1, released January 26, 2000, developed by Silicon Graphics,
** Inc. The Original Code is Copyright (c) 1991-2000 Silicon Graphics, Inc.
** Copyright in any portions created by third parties is as indicated
** elsewhere herein. All Rights Reserved.
**
** Additional Notice Provisions: The application programming interfaces
** established by SGI in conjunction with the Original Code are The
** OpenGL(R) Graphics System: A Specification (Version 1.2.1), released
** April 1, 1999; The OpenGL(R) Graphics System Utility Library (Version
** 1.3), released November 4, 1998; and OpenGL(R) Graphics with the X
** Window System(R) (Version 1.3), released October 19, 1998. This software
** was created using the OpenGL(R) version 1.2.1 Sample Implementation
** published by SGI, but has not been independently verified as being
** compliant with the OpenGL(R) version 1.2.1 Specification.
*/

#ifndef GL_APIENTRYP
#   define GL_APIENTRYP GL_APIENTRY*
#endif

#define GL_NVIDIA_PLATFORM_BINARY_NV                            0x890B
/*------------------------------------------------------------------------*
 * OES extension tokens
 *------------------------------------------------------------------------*/

/* GL_OES_compressed_ETC1_RGB8_texture */
#ifndef GL_OES_compressed_ETC1_RGB8_texture
#define GL_ETC1_RGB8_OES                                        0x8D64
#endif

/* GL_OES_compressed_paletted_texture */
#ifndef GL_OES_compressed_paletted_texture
#define GL_PALETTE4_RGB8_OES                                    0x8B90
#define GL_PALETTE4_RGBA8_OES                                   0x8B91
#define GL_PALETTE4_R5_G6_B5_OES                                0x8B92
#define GL_PALETTE4_RGBA4_OES                                   0x8B93
#define GL_PALETTE4_RGB5_A1_OES                                 0x8B94
#define GL_PALETTE8_RGB8_OES                                    0x8B95
#define GL_PALETTE8_RGBA8_OES                                   0x8B96
#define GL_PALETTE8_R5_G6_B5_OES                                0x8B97
#define GL_PALETTE8_RGBA4_OES                                   0x8B98
#define GL_PALETTE8_RGB5_A1_OES                                 0x8B99
#endif

/* GL_OES_EGL_image */
#ifndef GL_OES_EGL_image
typedef void* GLeglImageOES;
#endif

/* GL_OES_depth24 */
#ifndef GL_OES_depth24
#define GL_DEPTH_COMPONENT24_OES                                0x81A6
#endif

/* GL_OES_depth32 */
#ifndef GL_OES_depth32
#define GL_DEPTH_COMPONENT32_OES                                0x81A7
#endif

/* GL_OES_depth_texture */
/* No new tokens introduced by this extension. */

/* GL_OES_mapbuffer */
#ifndef GL_OES_mapbuffer
/* GL_READ_ONLY and GL_READ_WRITE not supported */
#define GL_WRITE_ONLY_OES                                       0x88B9
#define GL_BUFFER_ACCESS_OES                                    0x88BB
#define GL_BUFFER_MAPPED_OES                                    0x88BC
#define GL_BUFFER_MAP_POINTER_OES                               0x88BD
#endif

/* GL_OES_packed_depth_stencil */
#ifndef GL_OES_packed_depth_stencil
#define GL_DEPTH_STENCIL_OES                                    0x84F9
#define GL_UNSIGNED_INT_24_8_OES                                0x84FA
#define GL_DEPTH24_STENCIL8_OES                                 0x88F0
#endif

/* GL_OES_rgb8_rgba8 */
#ifndef GL_OES_rgb8_rgba8
#define GL_RGB8_OES                                             0x8051
#define GL_RGBA8_OES                                            0x8058
#endif

/* GL_OES_standard_derivatives */
#ifndef GL_OES_standard_derivatives
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES                  0x8B8B
#endif

/* GL_OES_stencil1 */
#ifndef GL_OES_stencil1
#define GL_STENCIL_INDEX1_OES                                   0x8D46
#endif

/* GL_OES_stencil4 */
#ifndef GL_OES_stencil4
#define GL_STENCIL_INDEX4_OES                                   0x8D47
#endif

/* GL_OES_texture3D */
#ifndef GL_OES_texture3D
#define GL_TEXTURE_WRAP_R_OES                                   0x8072
#define GL_TEXTURE_3D_OES                                       0x806F
#define GL_TEXTURE_BINDING_3D_OES                               0x806A
#define GL_MAX_3D_TEXTURE_SIZE_OES                              0x8073
#define GL_SAMPLER_3D_OES                                       0x8B5F
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_OES        0x8CD4
#endif

/* GL_OES_texture_half_float */
#ifndef GL_OES_texture_half_float
#define GL_HALF_FLOAT_OES                                       0x8D61
#endif

/* GL_OES_vertex_half_float */
/* GL_HALF_FLOAT_OES defined in GL_OES_texture_half_float already. */

/* GL_OES_vertex_type_10_10_10_2 */
#ifndef GL_OES_vertex_type_10_10_10_2
#define GL_UNSIGNED_INT_10_10_10_2_OES                          0x8DF6
#define GL_INT_10_10_10_2_OES                                   0x8DF7
#endif

/*------------------------------------------------------------------------*
 * AMD extension tokens
 *------------------------------------------------------------------------*/

/* GL_AMD_compressed_3DC_texture */
#ifndef GL_AMD_compressed_3DC_texture
#define GL_3DC_X_AMD                                            0x87F9
#define GL_3DC_XY_AMD                                           0x87FA
#endif

/* GL_AMD_compressed_ATC_texture */
#ifndef GL_AMD_compressed_ATC_texture
#define GL_ATC_RGB_AMD                                          0x8C92
#define GL_ATC_RGBA_EXPLICIT_ALPHA_AMD                          0x8C93
#define GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD                      0x87EE
#endif

/*------------------------------------------------------------------------*
 * EXT extension tokens
 *------------------------------------------------------------------------*/

/* GL_EXT_texture_filter_anisotropic */
#ifndef GL_EXT_texture_filter_anisotropic
#define GL_TEXTURE_MAX_ANISOTROPY_EXT                           0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT                       0x84FF
#endif

/* GL_EXT_texture_type_2_10_10_10_REV */
#ifndef GL_EXT_texture_type_2_10_10_10_REV
#define GL_UNSIGNED_INT_2_10_10_10_REV_EXT                      0x8368
#endif

/* GL_EXT_packed_float */
#ifndef GL_EXT_packed_float
#define GL_R11F_G11F_B10F_EXT                           0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV_EXT             0x8C3B
#define GL_RGBA_SIGNED_COMPONENTS_EXT                   0x8C3C
#endif

/* GL_EXT_texture_array */
#ifndef GL_EXT_texture_array
#define GL_TEXTURE_2D_ARRAY_EXT           0x8C1A
#define GL_SAMPLER_2D_ARRAY_EXT           0x8DC1
#define GL_TEXTURE_BINDING_2D_ARRAY_EXT   0x8C1D
#define GL_MAX_ARRAY_TEXTURE_LAYERS_EXT   0x88FF
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_EXT 0x8CD4
#endif

/* GL_EXT_texture_compression_dxt1 */
#ifndef GL_EXT_texture_compression_dxt1
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#endif

/* GL_EXT_texture_compression_latc */
#ifndef GL_EXT_texture_compression_latc
#define GL_COMPRESSED_LUMINANCE_LATC1_EXT               0x8C70
#define GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT        0x8C71
#define GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT         0x8C72
#define GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT  0x8C73
#endif

/* GL_EXT_texture_compression_s3tc */
#ifndef GL_EXT_texture_compression_s3tc
/* GL_COMPRESSED_RGB_S3TC_DXT1_EXT defined in GL_EXT_texture_compression_dxt1 already. */
/* GL_COMPRESSED_RGBA_S3TC_DXT1_EXT defined in GL_EXT_texture_compression_dxt1 already. */
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3
#endif

/* GL_EXT_texture_lod_bias */
#ifndef GL_EXT_texture_lod_bias
#define GL_MAX_TEXTURE_LOD_BIAS_EXT       0x84FD
#define GL_TEXTURE_FILTER_CONTROL_EXT     0x8500
#define GL_TEXTURE_LOD_BIAS_EXT           0x8501
#endif

/* GL_EXT_bgra */
#ifndef GL_EXT_bgra
#define GL_BGR_EXT                          0x80E0
#define GL_BGRA_EXT                         0x80E1
#endif

/*------------------------------------------------------------------------*
 * ARB extension tokens
 *------------------------------------------------------------------------*/
 
/* GL_ARB_draw_buffers */
#ifndef GL_ARB_draw_buffers
#define GL_MAX_DRAW_BUFFERS_ARB                    0x8824
#define GL_DRAW_BUFFER0_ARB                        0x8825
#define GL_DRAW_BUFFER1_ARB                        0x8826
#define GL_DRAW_BUFFER2_ARB                        0x8827
#define GL_DRAW_BUFFER3_ARB                        0x8828
#define GL_DRAW_BUFFER4_ARB                        0x8829
#define GL_DRAW_BUFFER5_ARB                        0x882A
#define GL_DRAW_BUFFER6_ARB                        0x882B
#define GL_DRAW_BUFFER7_ARB                        0x882C
#define GL_DRAW_BUFFER8_ARB                        0x882D
#define GL_DRAW_BUFFER9_ARB                        0x882E
#define GL_DRAW_BUFFER10_ARB                       0x882F
#define GL_DRAW_BUFFER11_ARB                       0x8830
#define GL_DRAW_BUFFER12_ARB                       0x8831
#define GL_DRAW_BUFFER13_ARB                       0x8832
#define GL_DRAW_BUFFER14_ARB                       0x8833
#define GL_DRAW_BUFFER15_ARB                       0x8834
// TODO: Move this out to a new extension?  We don't currently ever
//       return it because we're not doing strict FBO completeness.
//       It seems prudent to see what Khronos does with multiple color
//       attachments before creating a new extension for just this.
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_NV   0x8CDB
#endif

/* GL_ARB_half_float_pixel */
#ifndef GL_ARB_half_float_pixel
#define GL_HALF_FLOAT_ARB                          0x140B
#endif

/*------------------------------------------------------------------------*
 * NV extension tokens
 *------------------------------------------------------------------------*/

/* GL_NV_coverage_sample */
#ifndef GL_NV_coverage_sample
#define GL_COVERAGE_COMPONENT_NV          0x8ED0
#define GL_COVERAGE_COMPONENT4_NV         0x8ED1
#define GL_COVERAGE_ATTACHMENT_NV         0x8ED2
#define GL_COVERAGE_BUFFERS_NV            0x8ED3
#define GL_COVERAGE_SAMPLES_NV            0x8ED4
#define GL_COVERAGE_ALL_FRAGMENTS_NV      0x8ED5
#define GL_COVERAGE_EDGE_FRAGMENTS_NV     0x8ED6
#define GL_COVERAGE_AUTOMATIC_NV          0x8ED7
#define GL_COVERAGE_BUFFER_BIT_NV         0x8000
#endif

/* GL_NV_depth_nonlinear */
#ifndef GL_NV_depth_nonlinear
#define GL_DEPTH_COMPONENT16_NONLINEAR_NV 0x8E2C
#endif

/* GL_NV_draw_path */
#ifndef GL_NV_draw_path
#define GL_PATH_QUALITY_NV          0x8ED8
#define GL_FILL_RULE_NV             0x8ED9
#define GL_STROKE_CAP0_STYLE_NV     0x8EE0
#define GL_STROKE_CAP1_STYLE_NV     0x8EE1
#define GL_STROKE_CAP2_STYLE_NV     0x8EE2
#define GL_STROKE_CAP3_STYLE_NV     0x8EE3
#define GL_STROKE_JOIN_STYLE_NV     0x8EE8
#define GL_STROKE_MITER_LIMIT_NV    0x8EE9
#define GL_EVEN_ODD_NV              0x8EF0
#define GL_NON_ZERO_NV              0x8EF1
#define GL_CAP_BUTT_NV              0x8EF4
#define GL_CAP_ROUND_NV             0x8EF5
#define GL_CAP_SQUARE_NV            0x8EF6
#define GL_CAP_TRIANGLE_NV          0x8EF7
#define GL_JOIN_MITER_NV            0x8EFC
#define GL_JOIN_ROUND_NV            0x8EFD
#define GL_JOIN_BEVEL_NV            0x8EFE
#define GL_JOIN_CLIPPED_MITER_NV    0x8EFF
#define GL_MATRIX_PATH_TO_CLIP_NV   0x8F04
#define GL_MATRIX_STROKE_TO_PATH_NV 0x8F05
#define GL_MATRIX_PATH_COORD0_NV    0x8F08
#define GL_MATRIX_PATH_COORD1_NV    0x8F09
#define GL_MATRIX_PATH_COORD2_NV    0x8F0A
#define GL_MATRIX_PATH_COORD3_NV    0x8F0B
#define GL_FILL_PATH_NV             0x8F18
#define GL_STROKE_PATH_NV           0x8F19
#define GL_MOVE_TO_NV               0x00
#define GL_LINE_TO_NV               0x01
#define GL_QUADRATIC_BEZIER_TO_NV   0x02
#define GL_CUBIC_BEZIER_TO_NV       0x03
#define GL_START_MARKER_NV          0x20
#define GL_CLOSE_NV                 0x21
#define GL_STROKE_CAP0_NV           0x40
#define GL_STROKE_CAP1_NV           0x41
#define GL_STROKE_CAP2_NV           0x42
#define GL_STROKE_CAP3_NV           0x43
#endif

/* GL_NV_fbo_color_attachments */
#ifndef GL_NV_fbo_color_attachments
#define GL_MAX_COLOR_ATTACHMENTS_NV                     0x8CDF
#define GL_COLOR_ATTACHMENT0_NV                         0x8CE0
#define GL_COLOR_ATTACHMENT1_NV                         0x8CE1
#define GL_COLOR_ATTACHMENT2_NV                         0x8CE2
#define GL_COLOR_ATTACHMENT3_NV                         0x8CE3
#define GL_COLOR_ATTACHMENT4_NV                         0x8CE4
#define GL_COLOR_ATTACHMENT5_NV                         0x8CE5
#define GL_COLOR_ATTACHMENT6_NV                         0x8CE6
#define GL_COLOR_ATTACHMENT7_NV                         0x8CE7
#define GL_COLOR_ATTACHMENT8_NV                         0x8CE8
#define GL_COLOR_ATTACHMENT9_NV                         0x8CE9
#define GL_COLOR_ATTACHMENT10_NV                        0x8CEA
#define GL_COLOR_ATTACHMENT11_NV                        0x8CEB
#define GL_COLOR_ATTACHMENT12_NV                        0x8CEC
#define GL_COLOR_ATTACHMENT13_NV                        0x8CED
#define GL_COLOR_ATTACHMENT14_NV                        0x8CEE
#define GL_COLOR_ATTACHMENT15_NV                        0x8CEF
#endif

/* GL_NV_framebuffer_vertex_attrib_array */
#ifndef GL_NV_framebuffer_vertex_attrib_array
#define GL_FRAMEBUFFER_ATTACHABLE_NV                                  0x852A
#define GL_VERTEX_ATTRIB_ARRAY_NV                                     0x852B
#define GL_FRAMEBUFFER_ATTACHMENT_VERTEX_ATTRIB_ARRAY_SIZE_NV         0x852C
#define GL_FRAMEBUFFER_ATTACHMENT_VERTEX_ATTRIB_ARRAY_TYPE_NV         0x852D
#define GL_FRAMEBUFFER_ATTACHMENT_VERTEX_ATTRIB_ARRAY_NORMALIZED_NV   0x852E
#define GL_FRAMEBUFFER_ATTACHMENT_VERTEX_ATTRIB_ARRAY_OFFSET_NV       0x852F
#define GL_FRAMEBUFFER_ATTACHMENT_VERTEX_ATTRIB_ARRAY_WIDTH_NV        0x8530
#define GL_FRAMEBUFFER_ATTACHMENT_VERTEX_ATTRIB_ARRAY_STRIDE_NV       0x8531
#define GL_FRAMEBUFFER_ATTACHMENT_VERTEX_ATTRIB_ARRAY_HEIGHT_NV       0x8532
#endif

/* GL_NV_read_buffer */
#ifndef GL_NV_read_buffer
#define GL_READ_BUFFER_NV                          0x0C02
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_NV   0x8CDC
#endif

/*------------------------------------------------------------------------*
 * Other extension tokens
 *------------------------------------------------------------------------*/
 
/* GL_SGIS_texture_lod */
#ifndef GL_SGIS_texture_lod
#define GL_TEXTURE_MIN_LOD_SGIS           0x813A
#define GL_TEXTURE_MAX_LOD_SGIS           0x813B
#define GL_TEXTURE_BASE_LEVEL_SGIS        0x813C
#define GL_TEXTURE_MAX_LEVEL_SGIS         0x813D
#endif

/*------------------------------------------------------------------------*
 * OES extension functions
 *------------------------------------------------------------------------*/

/* GL_OES_compressed_ETC1_RGB8_texture */
#ifndef GL_OES_compressed_ETC1_RGB8_texture
#define GL_OES_compressed_ETC1_RGB8_texture 1
#endif

/* GL_OES_compressed_paletted_texture */
#ifndef GL_OES_compressed_paletted_texture
#define GL_OES_compressed_paletted_texture 1
#endif

/* GL_OES_EGL_image */
#ifndef GL_OES_EGL_image
#define GL_OES_EGL_image 1
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glEGLImageTargetTexture2DOES (GLenum target, GLeglImageOES image);
GL_APICALL void GL_APIENTRY glEGLImageTargetRenderbufferStorageOES (GLenum target, GLeglImageOES image);
#endif
typedef void (GL_APIENTRYP PFNGLEGLIMAGETARGETTEXTURE2DOESPROC) (GLenum target, GLeglImageOES image);
typedef void (GL_APIENTRYP PFNGLEGLIMAGETARGETRENDERBUFFERSTORAGEOESPROC) (GLenum target, GLeglImageOES image);
#endif

/* GL_OES_depth24 */
#ifndef GL_OES_depth24
#define GL_OES_depth24 1
#endif

/* GL_OES_depth32 */
#ifndef GL_OES_depth32
#define GL_OES_depth32 1
#endif

/* GL_OES_depth_texture */
#ifndef GL_OES_depth_texture
#define GL_OES_depth_texture 1
#endif

/* GL_OES_element_index_uint */
#ifndef GL_OES_element_index_uint
#define GL_OES_element_index_uint 1
#endif

/* GL_OES_fbo_render_mipmap */
#ifndef GL_OES_fbo_render_mipmap
#define GL_OES_fbo_render_mipmap 1
#endif

/* GL_OES_fragment_precision_high */
#ifndef GL_OES_fragment_precision_high
#define GL_OES_fragment_precision_high 1
#endif

/* GL_OES_mapbuffer */
#ifndef GL_OES_mapbuffer
#define GL_OES_mapbuffer 1
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void* GL_APIENTRY glMapBufferOES (GLenum target, GLenum access);
GL_APICALL GLboolean GL_APIENTRY glUnmapBufferOES (GLenum target);
GL_APICALL void GL_APIENTRY glGetBufferPointervOES (GLenum target, GLenum pname, void** params);
#endif
typedef void* (GL_APIENTRYP PFNGLMAPBUFFEROESPROC) (GLenum target, GLenum access);
typedef GLboolean (GL_APIENTRYP PFNGLUNMAPBUFFEROESPROC) (GLenum target);
typedef void (GL_APIENTRYP PFNGLGETBUFFERPOINTERVOESPROC) (GLenum target, GLenum pname, void** params);
#endif

/* GL_OES_packed_depth_stencil */
#ifndef GL_OES_packed_depth_stencil
#define GL_OES_packed_depth_stencil 1
#endif

/* GL_OES_rgb8_rgba8 */
#ifndef GL_OES_rgb8_rgba8
#define GL_OES_rgb8_rgba8 1
#endif

/* GL_OES_standard_derivatives */
#ifndef GL_OES_standard_derivatives
#define GL_OES_standard_derivatives 1
#endif

/* GL_OES_stencil1 */
#ifndef GL_OES_stencil1
#define GL_OES_stencil1 1
#endif

/* GL_OES_stencil4 */
#ifndef GL_OES_stencil4
#define GL_OES_stencil4 1
#endif

/* GL_OES_texture_3D */
#ifndef GL_OES_texture_3D
#define GL_OES_texture_3D 1
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glTexImage3DOES (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
GL_APICALL void GL_APIENTRY glTexSubImage3DOES (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
GL_APICALL void GL_APIENTRY glCopyTexSubImage3DOES (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GL_APICALL void GL_APIENTRY glCompressedTexImage3DOES (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
GL_APICALL void GL_APIENTRY glCompressedTexSubImage3DOES (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
GL_APICALL void GL_APIENTRY glFramebufferTexture3DOES (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
#endif
typedef void (GL_APIENTRYP PFNGLTEXIMAGE3DOESPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
typedef void (GL_APIENTRYP PFNGLTEXSUBIMAGE3DOESPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
typedef void (GL_APIENTRYP PFNGLCOPYTEXSUBIMAGE3DOESPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GL_APIENTRYP PFNGLCOMPRESSEDTEXIMAGE3DOESPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
typedef void (GL_APIENTRYP PFNGLCOMPRESSEDTEXSUBIMAGE3DOESPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
typedef void (GL_APIENTRYP PFNGLFRAMEBUFFERTEXTURE3DOES) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
#endif

/* GL_OES_texture_float_linear */
#ifndef GL_OES_texture_float_linear
#define GL_OES_texture_float_linear 1
#endif

/* GL_OES_texture_half_float_linear */
#ifndef GL_OES_texture_half_float_linear
#define GL_OES_texture_half_float_linear 1
#endif

/* GL_OES_texture_float */
#ifndef GL_OES_texture_float
#define GL_OES_texture_float 1
#endif

/* GL_OES_texture_half_float */
#ifndef GL_OES_texture_half_float
#define GL_OES_texture_half_float 1
#endif

/* GL_OES_texture_npot */
#ifndef GL_OES_texture_npot
#define GL_OES_texture_npot 1
#endif

/* GL_OES_vertex_half_float */
#ifndef GL_OES_vertex_half_float
#define GL_OES_vertex_half_float 1
#endif

/* GL_OES_vertex_type_10_10_10_2 */
#ifndef GL_OES_vertex_type_10_10_10_2
#define GL_OES_vertex_type_10_10_10_2 1
#endif

/*------------------------------------------------------------------------*
 * AMD extension functions
 *------------------------------------------------------------------------*/

/* GL_AMD_compressed_3DC_texture */
#ifndef GL_AMD_compressed_3DC_texture
#define GL_AMD_compressed_3DC_texture 1
#endif

/* GL_AMD_compressed_ATC_texture */
#ifndef GL_AMD_compressed_ATC_texture
#define GL_AMD_compressed_ATC_texture 1
#endif

/*------------------------------------------------------------------------*
 * EXT extension functions
 *------------------------------------------------------------------------*/

/* GL_EXT_texture_filter_anisotropic */
#ifndef GL_EXT_texture_filter_anisotropic
#define GL_EXT_texture_filter_anisotropic 1
#endif

/* GL_EXT_texture_type_2_10_10_10_REV */
#ifndef GL_EXT_texture_type_2_10_10_10_REV
#define GL_EXT_texture_type_2_10_10_10_REV 1
#endif

/* GL_EXT_packed_float */
#ifndef GL_EXT_packed_float
#define GL_EXT_packed_float 1
#endif

/* GL_EXT_texture_array */
#ifndef GL_EXT_texture_array
#define GL_EXT_texture_array 1
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glFramebufferTextureLayerEXT (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
#endif
typedef void (GL_APIENTRYP PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
#endif

/* GL_EXT_texture_compression_dxt1 */
#ifndef GL_EXT_texture_compression_dxt1
#define GL_EXT_texture_compression_dxt1 1
#endif

/* GL_EXT_texture_compression_latc */
#ifndef GL_EXT_texture_compression_latc
#define GL_EXT_texture_compression_latc 1
#endif

/* GL_EXT_texture_compression_s3tc */
#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc 1
#endif

/* GL_EXT_texture_lod_bias */
#ifndef GL_EXT_texture_lod_bias
#define GL_EXT_texture_lod_bias 1
#endif

/* GL_EXT_bgra */
#ifndef GL_EXT_bgra
#define GL_EXT_bgra 1
#endif

/*------------------------------------------------------------------------*
 * ARB extension functions
 *------------------------------------------------------------------------*/

/* GL_ARB_draw_buffers */
#ifndef GL_ARB_draw_buffers
#define GL_ARB_draw_buffers 1
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glDrawBuffersARB (GLsizei n, const GLenum *bufs);
#endif
typedef void (GL_APIENTRYP PFNGLDRAWBUFFERSARBPROC) (GLsizei n, const GLenum *bufs);
#endif

/* GL_ARB_half_float_pixel */
#ifndef GL_ARB_half_float_pixel
#define GL_ARB_half_float_pixel 1
#endif

/*------------------------------------------------------------------------*
 * NV extension functions
 *------------------------------------------------------------------------*/
 
/* GL_NV_coverage_sample */
#ifndef GL_NV_coverage_sample
#define GL_NV_coverage_sample 1
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glCoverageMaskNV (GLboolean mask);
GL_APICALL void GL_APIENTRY glCoverageOperationNV (GLenum operation);
#endif
typedef void (GL_APIENTRYP PFNGLCOVERAGEMASKNVPROC) (GLboolean mask);
typedef void (GL_APIENTRYP PFNGLCOVERAGEOPERATIONNVPROC) (GLenum operation);
#endif

/* GL_NV_depth_nonlinear */
#ifndef GL_NV_depth_nonlinear
#define GL_NV_depth_nonlinear 1
#endif

/* GL_NV_get_tex_image */
#ifndef GL_NV_get_tex_image 
#define GL_NV_get_tex_image 1
//those enums are the same as the big gl.h
#define GL_TEXTURE_WIDTH_NV                  0x1000
#define GL_TEXTURE_HEIGHT_NV                 0x1001
#define GL_TEXTURE_INTERNAL_FORMAT_NV        0x1003
#define GL_TEXTURE_COMPONENTS_NV             GL_TEXTURE_INTERNAL_FORMAT_NV
#define GL_TEXTURE_BORDER_NV                 0x1005
#define GL_TEXTURE_RED_SIZE_NV               0x805C
#define GL_TEXTURE_GREEN_SIZE_NV             0x805D
#define GL_TEXTURE_BLUE_SIZE_NV              0x805E
#define GL_TEXTURE_ALPHA_SIZE_NV             0x805F
#define GL_TEXTURE_LUMINANCE_SIZE_NV         0x8060
#define GL_TEXTURE_INTENSITY_SIZE_NV         0x8061
#define GL_TEXTURE_DEPTH_NV                  0x8071
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE_NV  0x86A0
#define GL_TEXTURE_COMPRESSED_NV             0x86A1
#define GL_TEXTURE_DEPTH_SIZE_NV             0x884A
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glGetTexImageNV (GLenum target, GLint level, GLenum format, GLenum type, GLvoid* img);
GL_APICALL void GL_APIENTRY glGetCompressedTexImageNV (GLenum target, GLint level, GLvoid* img);
GL_APICALL void GL_APIENTRY glGetTexLevelParameterfvNV (GLenum target, GLint level, GLenum pname, GLfloat* params);
GL_APICALL void GL_APIENTRY glGetTexLevelParameterivNV (GLenum target, GLint level, GLenum pname, GLint* params);
#endif
typedef void (GL_APIENTRYP PFNGLGETTEXIMAGENVPROC) (GLenum target, GLint level, GLenum format, GLenum type, GLvoid* img);
typedef void (GL_APIENTRYP PFNGLGETCOMPRESSEDTEXIMAGENVPROC) (GLenum target, GLint level, GLvoid* img);
typedef void (GL_APIENTRYP PFNGLGETTEXLEVELPARAMETERFVNVPROC) (GLenum target, GLint level, GLenum pname, GLfloat* params);
typedef void (GL_APIENTRYP PFNGLGETTEXLEVELPARAMETERiVNVPROC) (GLenum target, GLint level, GLenum pname, GLint* params);
#endif


/* GL_NV_draw_path */
#ifndef GL_NV_draw_path
#define GL_NV_draw_path 1
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL GLuint GL_APIENTRY glCreatePathNV (GLenum datatype, GLsizei numCommands, const GLubyte* commands);
GL_APICALL void GL_APIENTRY glDeletePathNV (GLuint path);
GL_APICALL void GL_APIENTRY glPathVerticesNV (GLuint path, const void* vertices);
GL_APICALL void GL_APIENTRY glPathParameterfNV (GLuint path, GLenum paramType, GLfloat param);
GL_APICALL void GL_APIENTRY glPathParameteriNV (GLuint path, GLenum paramType, GLint param);
GL_APICALL GLuint GL_APIENTRY glCreatePathProgramNV (void);
GL_APICALL void GL_APIENTRY glPathMatrixNV (GLenum target, const GLfloat* value);
GL_APICALL void GL_APIENTRY glDrawPathNV (GLuint path, GLenum mode);
GL_APICALL GLuint GL_APIENTRY glCreatePathbufferNV (GLsizei capacity);
GL_APICALL void GL_APIENTRY glDeletePathbufferNV (GLuint buffer);
GL_APICALL void GL_APIENTRY glPathbufferPathNV (GLuint buffer, GLint index, GLuint path);
GL_APICALL void GL_APIENTRY glPathbufferPositionNV (GLuint buffer, GLint index, GLfloat x, GLfloat y);
GL_APICALL void GL_APIENTRY glDrawPathbufferNV (GLuint buffer, GLenum mode);
#endif
typedef GLuint (GL_APIENTRYP PFNGLCREATEPATHNVPROC) (GLenum datatype, GLsizei numCommands, const GLubyte* commands);
typedef void (GL_APIENTRYP PFNGLDELETEPATHNVPROC) (GLuint path);
typedef void (GL_APIENTRYP PFNGLPATHVERTICESNVPROC) (GLuint path, const void* vertices);
typedef void (GL_APIENTRYP PFNGLPATHPARAMETERFNVPROC) (GLuint path, GLenum paramType, GLfloat param);
typedef void (GL_APIENTRYP PFNGLPATHPARAMETERINVPROC) (GLuint path, GLenum paramType, GLint param);
typedef GLuint (GL_APIENTRYP PFNGLCREATEPATHPROGRAMNVPROC) (void);
typedef void (GL_APIENTRYP PFNGLPATHMATRIXNVPROC) (GLenum target, const GLfloat* value);
typedef void (GL_APIENTRYP PFNGLDRAWPATHNVPROC) (GLuint path, GLenum mode);
typedef GLuint (GL_APIENTRYP PFNGLCREATEPATHBUFFERNVPROC) (GLsizei capacity);
typedef void (GL_APIENTRYP PFNGLDELETEPATHBUFFERNVPROC) (GLuint buffer);
typedef void (GL_APIENTRYP PFNGLPATHBUFFERPATHNVPROC) (GLuint buffer, GLint index, GLuint path);
typedef void (GL_APIENTRYP PFNGLPATHBUFFERPOSITIONNVPROC) (GLuint buffer, GLint index, GLfloat x, GLfloat y);
typedef void (GL_APIENTRYP PFNGLDRAWPATHBUFFERPROC) (GLuint buffer, GLenum mode);
#endif

/* GL_NV_extended_point_sprite */
#ifndef GL_NV_extended_point_sprite
#define GL_NV_extended_point_sprite 1
#endif

/* GL_NV_fbo_color_attachments */
#ifndef GL_NV_fbo_color_attachments
#define GL_NV_fbo_color_attachments 1
#endif

/* GL_NV_framebuffer_vertex_attrib_array */
#ifndef GL_NV_framebuffer_vertex_attrib_array
#define GL_NV_framebuffer_vertex_attrib_array 1
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glFramebufferVertexAttribArrayNV (GLenum target, GLenum attachment, GLenum buffertarget, GLuint bufferobject, GLint size, GLenum type, GLboolean normalized, GLintptr offset, GLsizeiptr width, GLsizeiptr height, GLsizei stride);
#endif
typedef void (GL_APIENTRYP PFNGLFRAMEBUFFERVERTEXATTRIBARRAYNVPROC) (GLenum target, GLenum attachment, GLenum buffertarget, GLuint bufferobject, GLint size, GLenum type, GLboolean normalized, GLintptr offset, GLsizeiptr width, GLsizeiptr height, GLsizei stride);
#endif

/* GL_NV_read_buffer */
#ifndef GL_NV_read_buffer
#define GL_NV_read_buffer 1
#ifdef GL_GLEXT_PROTOTYPES
GL_APICALL void GL_APIENTRY glReadBufferNV (GLenum mode);
#endif
typedef void (GL_APIENTRYP PFNGLREADBUFFERNVPROC) (GLenum mode);
#endif

/* GL_NV_shader_framebuffer_fetch */
#ifndef GL_NV_shader_framebuffer_fetch
#define GL_NV_shader_framebuffer_fetch 1
#endif

/*------------------------------------------------------------------------*
 * Other extension functions
 *------------------------------------------------------------------------*/
 
/* GL_SGIS_texture_lod */
#ifndef GL_SGIS_texture_lod
#define GL_SGIS_texture_lod 1
#endif

#ifdef __cplusplus
}
#endif

#endif /* __gl2ext_h_ */
