////////////////////////////////////////////////////////////////////////////////
//
/**
  \file  
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////
// clang-format off
/*
** The OpenGL Extension Wrangler Library
** Copyright (C) 2008-2015, Nigel Stewart <nigels[]users sourceforge net>
** Copyright (C) 2002-2008, Milan Ikits <milan ikits[]ieee org>
** Copyright (C) 2002-2008, Marcelo E. Magallon <mmagallo[]debian org>
** Copyright (C) 2002, Lev Povalahev
** All rights reserved.
** 
** Redistribution and use in source and binary forms, with or without 
** modification, are permitted provided that the following conditions are met:
** 
** * Redistributions of source code must retain the above copyright notice, 
**   this list of conditions and the following disclaimer.
** * Redistributions in binary form must reproduce the above copyright notice, 
**   this list of conditions and the following disclaimer in the documentation 
**   and/or other materials provided with the distribution.
** * The name of the author may be used to endorse or promote products 
**   derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
** THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
** Copyright (c) 2007 The Khronos Group Inc.
** 
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
** 
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
** 
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/

#pragma once
#define __WGLEW_H__

#ifdef __wglext_h_
#error wglext.h included before wglew.h
#endif

#define __wglext_h_

#if !defined(WINAPI)
#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN 1
#  endif
#include <Windows.h>
#  undef WIN32_LEAN_AND_MEAN
#endif

/*
 * GLEW_STATIC needs to be set when using the static version.
 * GLEW_BUILD is set when building the DLL version.
 */
#ifdef GLEW_STATIC
#  define GLEWAPI extern
#else
#  ifdef GLEW_BUILD
#    define GLEWAPI extern __declspec(dllexport)
#  else
#    define GLEWAPI extern __declspec(dllimport)
#  endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------- WGL_ARB_buffer_region ------------------------- */

#ifndef WGL_ARB_buffer_region
#define WGL_ARB_buffer_region 1

#define WGL_FRONT_COLOR_BUFFER_BIT_ARB 0x00000001
#define WGL_BACK_COLOR_BUFFER_BIT_ARB 0x00000002
#define WGL_DEPTH_BUFFER_BIT_ARB 0x00000004
#define WGL_STENCIL_BUFFER_BIT_ARB 0x00000008

typedef HANDLE (WINAPI * PFNWGLCREATEBUFFERREGIONARBPROC) (HDC hDC, int iLayerPlane, UINT uType);
typedef VOID (WINAPI * PFNWGLDELETEBUFFERREGIONARBPROC) (HANDLE hRegion);
typedef BOOL (WINAPI * PFNWGLRESTOREBUFFERREGIONARBPROC) (HANDLE hRegion, int x, int y, int width, int height, int xSrc, int ySrc);
typedef BOOL (WINAPI * PFNWGLSAVEBUFFERREGIONARBPROC) (HANDLE hRegion, int x, int y, int width, int height);

#define wglCreateBufferRegionARB WGLEW_GET_FUN(__wglewCreateBufferRegionARB)
#define wglDeleteBufferRegionARB WGLEW_GET_FUN(__wglewDeleteBufferRegionARB)
#define wglRestoreBufferRegionARB WGLEW_GET_FUN(__wglewRestoreBufferRegionARB)
#define wglSaveBufferRegionARB WGLEW_GET_FUN(__wglewSaveBufferRegionARB)

#define WGLEW_ARB_buffer_region WGLEW_GET_VAR(__WGLEW_ARB_buffer_region)

#endif /* WGL_ARB_buffer_region */

/* --------------------- WGL_ARB_context_flush_control --------------------- */

#ifndef WGL_ARB_context_flush_control
#define WGL_ARB_context_flush_control 1

#define WGLEW_ARB_context_flush_control WGLEW_GET_VAR(__WGLEW_ARB_context_flush_control)

#endif /* WGL_ARB_context_flush_control */

/* ------------------------- WGL_ARB_create_context ------------------------ */

#ifndef WGL_ARB_create_context
#define WGL_ARB_create_context 1

#define WGL_CONTEXT_DEBUG_BIT_ARB 0x0001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB 0x2093
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define ERROR_INVALID_VERSION_ARB 0x2095
#define ERROR_INVALID_PROFILE_ARB 0x2096

typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);

#define wglCreateContextAttribsARB WGLEW_GET_FUN(__wglewCreateContextAttribsARB)

#define WGLEW_ARB_create_context WGLEW_GET_VAR(__WGLEW_ARB_create_context)

#endif /* WGL_ARB_create_context */

/* -------------------- WGL_ARB_create_context_no_error -------------------- */

#ifndef WGL_ARB_create_context_no_error
#define WGL_ARB_create_context_no_error 1

#define WGLEW_ARB_create_context_no_error WGLEW_GET_VAR(__WGLEW_ARB_create_context_no_error)

#endif /* WGL_ARB_create_context_no_error */

/* --------------------- WGL_ARB_create_context_profile -------------------- */

#ifndef WGL_ARB_create_context_profile
#define WGL_ARB_create_context_profile 1

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126

#define WGLEW_ARB_create_context_profile WGLEW_GET_VAR(__WGLEW_ARB_create_context_profile)

#endif /* WGL_ARB_create_context_profile */

/* ------------------- WGL_ARB_create_context_robustness ------------------- */

#ifndef WGL_ARB_create_context_robustness
#define WGL_ARB_create_context_robustness 1

#define WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB 0x00000004
#define WGL_LOSE_CONTEXT_ON_RESET_ARB 0x8252
#define WGL_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB 0x8256
#define WGL_NO_RESET_NOTIFICATION_ARB 0x8261

#define WGLEW_ARB_create_context_robustness WGLEW_GET_VAR(__WGLEW_ARB_create_context_robustness)

#endif /* WGL_ARB_create_context_robustness */

/* ----------------------- WGL_ARB_extensions_string ----------------------- */

#ifndef WGL_ARB_extensions_string
#define WGL_ARB_extensions_string 1

typedef const char* (WINAPI * PFNWGLGETEXTENSIONSSTRINGARBPROC) (HDC hdc);

#define wglGetExtensionsStringARB WGLEW_GET_FUN(__wglewGetExtensionsStringARB)

#define WGLEW_ARB_extensions_string WGLEW_GET_VAR(__WGLEW_ARB_extensions_string)

#endif /* WGL_ARB_extensions_string */

/* ------------------------ WGL_ARB_framebuffer_sRGB ----------------------- */

#ifndef WGL_ARB_framebuffer_sRGB
#define WGL_ARB_framebuffer_sRGB 1

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB 0x20A9

#define WGLEW_ARB_framebuffer_sRGB WGLEW_GET_VAR(__WGLEW_ARB_framebuffer_sRGB)

#endif /* WGL_ARB_framebuffer_sRGB */

/* ----------------------- WGL_ARB_make_current_read ----------------------- */

#ifndef WGL_ARB_make_current_read
#define WGL_ARB_make_current_read 1

#define ERROR_INVALID_PIXEL_TYPE_ARB 0x2043
#define ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB 0x2054

typedef HDC (WINAPI * PFNWGLGETCURRENTREADDCARBPROC) (VOID);
typedef BOOL (WINAPI * PFNWGLMAKECONTEXTCURRENTARBPROC) (HDC hDrawDC, HDC hReadDC, HGLRC hglrc);

#define wglGetCurrentReadDCARB WGLEW_GET_FUN(__wglewGetCurrentReadDCARB)
#define wglMakeContextCurrentARB WGLEW_GET_FUN(__wglewMakeContextCurrentARB)

#define WGLEW_ARB_make_current_read WGLEW_GET_VAR(__WGLEW_ARB_make_current_read)

#endif /* WGL_ARB_make_current_read */

/* -------------------------- WGL_ARB_multisample -------------------------- */

#ifndef WGL_ARB_multisample
#define WGL_ARB_multisample 1

#define WGL_SAMPLE_BUFFERS_ARB 0x2041
#define WGL_SAMPLES_ARB 0x2042

#define WGLEW_ARB_multisample WGLEW_GET_VAR(__WGLEW_ARB_multisample)

#endif /* WGL_ARB_multisample */

/* ---------------------------- WGL_ARB_pbuffer ---------------------------- */

#ifndef WGL_ARB_pbuffer
#define WGL_ARB_pbuffer 1

#define WGL_DRAW_TO_PBUFFER_ARB 0x202D
#define WGL_MAX_PBUFFER_PIXELS_ARB 0x202E
#define WGL_MAX_PBUFFER_WIDTH_ARB 0x202F
#define WGL_MAX_PBUFFER_HEIGHT_ARB 0x2030
#define WGL_PBUFFER_LARGEST_ARB 0x2033
#define WGL_PBUFFER_WIDTH_ARB 0x2034
#define WGL_PBUFFER_HEIGHT_ARB 0x2035
#define WGL_PBUFFER_LOST_ARB 0x2036

DECLARE_HANDLE(HPBUFFERARB);

typedef HPBUFFERARB (WINAPI * PFNWGLCREATEPBUFFERARBPROC) (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int* piAttribList);
typedef BOOL (WINAPI * PFNWGLDESTROYPBUFFERARBPROC) (HPBUFFERARB hPbuffer);
typedef HDC (WINAPI * PFNWGLGETPBUFFERDCARBPROC) (HPBUFFERARB hPbuffer);
typedef BOOL (WINAPI * PFNWGLQUERYPBUFFERARBPROC) (HPBUFFERARB hPbuffer, int iAttribute, int* piValue);
typedef int (WINAPI * PFNWGLRELEASEPBUFFERDCARBPROC) (HPBUFFERARB hPbuffer, HDC hDC);

#define wglCreatePbufferARB WGLEW_GET_FUN(__wglewCreatePbufferARB)
#define wglDestroyPbufferARB WGLEW_GET_FUN(__wglewDestroyPbufferARB)
#define wglGetPbufferDCARB WGLEW_GET_FUN(__wglewGetPbufferDCARB)
#define wglQueryPbufferARB WGLEW_GET_FUN(__wglewQueryPbufferARB)
#define wglReleasePbufferDCARB WGLEW_GET_FUN(__wglewReleasePbufferDCARB)

#define WGLEW_ARB_pbuffer WGLEW_GET_VAR(__WGLEW_ARB_pbuffer)

#endif /* WGL_ARB_pbuffer */

/* -------------------------- WGL_ARB_pixel_format ------------------------- */

#ifndef WGL_ARB_pixel_format
#define WGL_ARB_pixel_format 1

#define WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_DRAW_TO_BITMAP_ARB 0x2002
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_NEED_PALETTE_ARB 0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB 0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB 0x2006
#define WGL_SWAP_METHOD_ARB 0x2007
#define WGL_NUMBER_OVERLAYS_ARB 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB 0x2009
#define WGL_TRANSPARENT_ARB 0x200A
#define WGL_SHARE_DEPTH_ARB 0x200C
#define WGL_SHARE_STENCIL_ARB 0x200D
#define WGL_SHARE_ACCUM_ARB 0x200E
#define WGL_SUPPORT_GDI_ARB 0x200F
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_STEREO_ARB 0x2012
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_RED_BITS_ARB 0x2015
#define WGL_RED_SHIFT_ARB 0x2016
#define WGL_GREEN_BITS_ARB 0x2017
#define WGL_GREEN_SHIFT_ARB 0x2018
#define WGL_BLUE_BITS_ARB 0x2019
#define WGL_BLUE_SHIFT_ARB 0x201A
#define WGL_ALPHA_BITS_ARB 0x201B
#define WGL_ALPHA_SHIFT_ARB 0x201C
#define WGL_ACCUM_BITS_ARB 0x201D
#define WGL_ACCUM_RED_BITS_ARB 0x201E
#define WGL_ACCUM_GREEN_BITS_ARB 0x201F
#define WGL_ACCUM_BLUE_BITS_ARB 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB 0x2021
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_AUX_BUFFERS_ARB 0x2024
#define WGL_NO_ACCELERATION_ARB 0x2025
#define WGL_GENERIC_ACCELERATION_ARB 0x2026
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_SWAP_EXCHANGE_ARB 0x2028
#define WGL_SWAP_COPY_ARB 0x2029
#define WGL_SWAP_UNDEFINED_ARB 0x202A
#define WGL_TYPE_RGBA_ARB 0x202B
#define WGL_TYPE_COLORINDEX_ARB 0x202C
#define WGL_TRANSPARENT_RED_VALUE_ARB 0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B

typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef BOOL (WINAPI * PFNWGLGETPIXELFORMATATTRIBFVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, FLOAT *pfValues);
typedef BOOL (WINAPI * PFNWGLGETPIXELFORMATATTRIBIVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int* piAttributes, int *piValues);

#define wglChoosePixelFormatARB WGLEW_GET_FUN(__wglewChoosePixelFormatARB)
#define wglGetPixelFormatAttribfvARB WGLEW_GET_FUN(__wglewGetPixelFormatAttribfvARB)
#define wglGetPixelFormatAttribivARB WGLEW_GET_FUN(__wglewGetPixelFormatAttribivARB)

#define WGLEW_ARB_pixel_format WGLEW_GET_VAR(__WGLEW_ARB_pixel_format)

#endif /* WGL_ARB_pixel_format */

/* ----------------------- WGL_ARB_pixel_format_float ---------------------- */

#ifndef WGL_ARB_pixel_format_float
#define WGL_ARB_pixel_format_float 1

#define WGL_TYPE_RGBA_FLOAT_ARB 0x21A0

#define WGLEW_ARB_pixel_format_float WGLEW_GET_VAR(__WGLEW_ARB_pixel_format_float)

#endif /* WGL_ARB_pixel_format_float */

/* ------------------------- WGL_ARB_render_texture ------------------------ */

#ifndef WGL_ARB_render_texture
#define WGL_ARB_render_texture 1

#define WGL_BIND_TO_TEXTURE_RGB_ARB 0x2070
#define WGL_BIND_TO_TEXTURE_RGBA_ARB 0x2071
#define WGL_TEXTURE_FORMAT_ARB 0x2072
#define WGL_TEXTURE_TARGET_ARB 0x2073
#define WGL_MIPMAP_TEXTURE_ARB 0x2074
#define WGL_TEXTURE_RGB_ARB 0x2075
#define WGL_TEXTURE_RGBA_ARB 0x2076
#define WGL_NO_TEXTURE_ARB 0x2077
#define WGL_TEXTURE_CUBE_MAP_ARB 0x2078
#define WGL_TEXTURE_1D_ARB 0x2079
#define WGL_TEXTURE_2D_ARB 0x207A
#define WGL_MIPMAP_LEVEL_ARB 0x207B
#define WGL_CUBE_MAP_FACE_ARB 0x207C
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x207D
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x207E
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x207F
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x2080
#define WGL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x2081
#define WGL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x2082
#define WGL_FRONT_LEFT_ARB 0x2083
#define WGL_FRONT_RIGHT_ARB 0x2084
#define WGL_BACK_LEFT_ARB 0x2085
#define WGL_BACK_RIGHT_ARB 0x2086
#define WGL_AUX0_ARB 0x2087
#define WGL_AUX1_ARB 0x2088
#define WGL_AUX2_ARB 0x2089
#define WGL_AUX3_ARB 0x208A
#define WGL_AUX4_ARB 0x208B
#define WGL_AUX5_ARB 0x208C
#define WGL_AUX6_ARB 0x208D
#define WGL_AUX7_ARB 0x208E
#define WGL_AUX8_ARB 0x208F
#define WGL_AUX9_ARB 0x2090

typedef BOOL (WINAPI * PFNWGLBINDTEXIMAGEARBPROC) (HPBUFFERARB hPbuffer, int iBuffer);
typedef BOOL (WINAPI * PFNWGLRELEASETEXIMAGEARBPROC) (HPBUFFERARB hPbuffer, int iBuffer);
typedef BOOL (WINAPI * PFNWGLSETPBUFFERATTRIBARBPROC) (HPBUFFERARB hPbuffer, const int* piAttribList);

#define wglBindTexImageARB WGLEW_GET_FUN(__wglewBindTexImageARB)
#define wglReleaseTexImageARB WGLEW_GET_FUN(__wglewReleaseTexImageARB)
#define wglSetPbufferAttribARB WGLEW_GET_FUN(__wglewSetPbufferAttribARB)

#define WGLEW_ARB_render_texture WGLEW_GET_VAR(__WGLEW_ARB_render_texture)

#endif /* WGL_ARB_render_texture */

/* ---------------- WGL_ARB_robustness_application_isolation --------------- */

#ifndef WGL_ARB_robustness_application_isolation
#define WGL_ARB_robustness_application_isolation 1

#define WGL_CONTEXT_RESET_ISOLATION_BIT_ARB 0x00000008

#define WGLEW_ARB_robustness_application_isolation WGLEW_GET_VAR(__WGLEW_ARB_robustness_application_isolation)

#endif /* WGL_ARB_robustness_application_isolation */

/* ---------------- WGL_ARB_robustness_share_group_isolation --------------- */

#ifndef WGL_ARB_robustness_share_group_isolation
#define WGL_ARB_robustness_share_group_isolation 1

#define WGL_CONTEXT_RESET_ISOLATION_BIT_ARB 0x00000008

#define WGLEW_ARB_robustness_share_group_isolation WGLEW_GET_VAR(__WGLEW_ARB_robustness_share_group_isolation)

#endif /* WGL_ARB_robustness_share_group_isolation */

/* --------------------------- WGL_EXT_colorspace -------------------------- */

#ifndef WGL_EXT_colorspace
#define WGL_EXT_colorspace 1

#define WGL_COLORSPACE_SRGB_EXT 0x3089
#define WGL_COLORSPACE_LINEAR_EXT 0x308A
#define WGL_COLORSPACE_EXT 0x309D

#define WGLEW_EXT_colorspace WGLEW_GET_VAR(__WGLEW_EXT_colorspace)

#endif /* WGL_EXT_colorspace */

/* ------------------- WGL_EXT_create_context_es2_profile ------------------ */

#ifndef WGL_EXT_create_context_es2_profile
#define WGL_EXT_create_context_es2_profile 1

#define WGL_CONTEXT_ES2_PROFILE_BIT_EXT 0x00000004

#define WGLEW_EXT_create_context_es2_profile WGLEW_GET_VAR(__WGLEW_EXT_create_context_es2_profile)

#endif /* WGL_EXT_create_context_es2_profile */

/* ------------------- WGL_EXT_create_context_es_profile ------------------- */

#ifndef WGL_EXT_create_context_es_profile
#define WGL_EXT_create_context_es_profile 1

#define WGL_CONTEXT_ES_PROFILE_BIT_EXT 0x00000004

#define WGLEW_EXT_create_context_es_profile WGLEW_GET_VAR(__WGLEW_EXT_create_context_es_profile)

#endif /* WGL_EXT_create_context_es_profile */

/* -------------------------- WGL_EXT_depth_float -------------------------- */

#ifndef WGL_EXT_depth_float
#define WGL_EXT_depth_float 1

#define WGL_DEPTH_FLOAT_EXT 0x2040

#define WGLEW_EXT_depth_float WGLEW_GET_VAR(__WGLEW_EXT_depth_float)

#endif /* WGL_EXT_depth_float */

/* ---------------------- WGL_EXT_display_color_table ---------------------- */

#ifndef WGL_EXT_display_color_table
#define WGL_EXT_display_color_table 1

typedef GLboolean (WINAPI * PFNWGLBINDDISPLAYCOLORTABLEEXTPROC) (GLushort id);
typedef GLboolean (WINAPI * PFNWGLCREATEDISPLAYCOLORTABLEEXTPROC) (GLushort id);
typedef void (WINAPI * PFNWGLDESTROYDISPLAYCOLORTABLEEXTPROC) (GLushort id);
typedef GLboolean (WINAPI * PFNWGLLOADDISPLAYCOLORTABLEEXTPROC) (GLushort* table, GLuint length);

#define wglBindDisplayColorTableEXT WGLEW_GET_FUN(__wglewBindDisplayColorTableEXT)
#define wglCreateDisplayColorTableEXT WGLEW_GET_FUN(__wglewCreateDisplayColorTableEXT)
#define wglDestroyDisplayColorTableEXT WGLEW_GET_FUN(__wglewDestroyDisplayColorTableEXT)
#define wglLoadDisplayColorTableEXT WGLEW_GET_FUN(__wglewLoadDisplayColorTableEXT)

#define WGLEW_EXT_display_color_table WGLEW_GET_VAR(__WGLEW_EXT_display_color_table)

#endif /* WGL_EXT_display_color_table */

/* ----------------------- WGL_EXT_extensions_string ----------------------- */

#ifndef WGL_EXT_extensions_string
#define WGL_EXT_extensions_string 1

typedef const char* (WINAPI * PFNWGLGETEXTENSIONSSTRINGEXTPROC) (void);

#define wglGetExtensionsStringEXT WGLEW_GET_FUN(__wglewGetExtensionsStringEXT)

#define WGLEW_EXT_extensions_string WGLEW_GET_VAR(__WGLEW_EXT_extensions_string)

#endif /* WGL_EXT_extensions_string */

/* ------------------------ WGL_EXT_framebuffer_sRGB ----------------------- */

#ifndef WGL_EXT_framebuffer_sRGB
#define WGL_EXT_framebuffer_sRGB 1

#define WGL_FRAMEBUFFER_SRGB_CAPABLE_EXT 0x20A9

#define WGLEW_EXT_framebuffer_sRGB WGLEW_GET_VAR(__WGLEW_EXT_framebuffer_sRGB)

#endif /* WGL_EXT_framebuffer_sRGB */

/* ----------------------- WGL_EXT_make_current_read ----------------------- */

#ifndef WGL_EXT_make_current_read
#define WGL_EXT_make_current_read 1

#define ERROR_INVALID_PIXEL_TYPE_EXT 0x2043

typedef HDC (WINAPI * PFNWGLGETCURRENTREADDCEXTPROC) (VOID);
typedef BOOL (WINAPI * PFNWGLMAKECONTEXTCURRENTEXTPROC) (HDC hDrawDC, HDC hReadDC, HGLRC hglrc);

#define wglGetCurrentReadDCEXT WGLEW_GET_FUN(__wglewGetCurrentReadDCEXT)
#define wglMakeContextCurrentEXT WGLEW_GET_FUN(__wglewMakeContextCurrentEXT)

#define WGLEW_EXT_make_current_read WGLEW_GET_VAR(__WGLEW_EXT_make_current_read)

#endif /* WGL_EXT_make_current_read */

/* -------------------------- WGL_EXT_multisample -------------------------- */

#ifndef WGL_EXT_multisample
#define WGL_EXT_multisample 1

#define WGL_SAMPLE_BUFFERS_EXT 0x2041
#define WGL_SAMPLES_EXT 0x2042

#define WGLEW_EXT_multisample WGLEW_GET_VAR(__WGLEW_EXT_multisample)

#endif /* WGL_EXT_multisample */

/* ---------------------------- WGL_EXT_pbuffer ---------------------------- */

#ifndef WGL_EXT_pbuffer
#define WGL_EXT_pbuffer 1

#define WGL_DRAW_TO_PBUFFER_EXT 0x202D
#define WGL_MAX_PBUFFER_PIXELS_EXT 0x202E
#define WGL_MAX_PBUFFER_WIDTH_EXT 0x202F
#define WGL_MAX_PBUFFER_HEIGHT_EXT 0x2030
#define WGL_OPTIMAL_PBUFFER_WIDTH_EXT 0x2031
#define WGL_OPTIMAL_PBUFFER_HEIGHT_EXT 0x2032
#define WGL_PBUFFER_LARGEST_EXT 0x2033
#define WGL_PBUFFER_WIDTH_EXT 0x2034
#define WGL_PBUFFER_HEIGHT_EXT 0x2035

DECLARE_HANDLE(HPBUFFEREXT);

typedef HPBUFFEREXT (WINAPI * PFNWGLCREATEPBUFFEREXTPROC) (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int* piAttribList);
typedef BOOL (WINAPI * PFNWGLDESTROYPBUFFEREXTPROC) (HPBUFFEREXT hPbuffer);
typedef HDC (WINAPI * PFNWGLGETPBUFFERDCEXTPROC) (HPBUFFEREXT hPbuffer);
typedef BOOL (WINAPI * PFNWGLQUERYPBUFFEREXTPROC) (HPBUFFEREXT hPbuffer, int iAttribute, int* piValue);
typedef int (WINAPI * PFNWGLRELEASEPBUFFERDCEXTPROC) (HPBUFFEREXT hPbuffer, HDC hDC);

#define wglCreatePbufferEXT WGLEW_GET_FUN(__wglewCreatePbufferEXT)
#define wglDestroyPbufferEXT WGLEW_GET_FUN(__wglewDestroyPbufferEXT)
#define wglGetPbufferDCEXT WGLEW_GET_FUN(__wglewGetPbufferDCEXT)
#define wglQueryPbufferEXT WGLEW_GET_FUN(__wglewQueryPbufferEXT)
#define wglReleasePbufferDCEXT WGLEW_GET_FUN(__wglewReleasePbufferDCEXT)

#define WGLEW_EXT_pbuffer WGLEW_GET_VAR(__WGLEW_EXT_pbuffer)

#endif /* WGL_EXT_pbuffer */

/* -------------------------- WGL_EXT_pixel_format ------------------------- */

#ifndef WGL_EXT_pixel_format
#define WGL_EXT_pixel_format 1

#define WGL_NUMBER_PIXEL_FORMATS_EXT 0x2000
#define WGL_DRAW_TO_WINDOW_EXT 0x2001
#define WGL_DRAW_TO_BITMAP_EXT 0x2002
#define WGL_ACCELERATION_EXT 0x2003
#define WGL_NEED_PALETTE_EXT 0x2004
#define WGL_NEED_SYSTEM_PALETTE_EXT 0x2005
#define WGL_SWAP_LAYER_BUFFERS_EXT 0x2006
#define WGL_SWAP_METHOD_EXT 0x2007
#define WGL_NUMBER_OVERLAYS_EXT 0x2008
#define WGL_NUMBER_UNDERLAYS_EXT 0x2009
#define WGL_TRANSPARENT_EXT 0x200A
#define WGL_TRANSPARENT_VALUE_EXT 0x200B
#define WGL_SHARE_DEPTH_EXT 0x200C
#define WGL_SHARE_STENCIL_EXT 0x200D
#define WGL_SHARE_ACCUM_EXT 0x200E
#define WGL_SUPPORT_GDI_EXT 0x200F
#define WGL_SUPPORT_OPENGL_EXT 0x2010
#define WGL_DOUBLE_BUFFER_EXT 0x2011
#define WGL_STEREO_EXT 0x2012
#define WGL_PIXEL_TYPE_EXT 0x2013
#define WGL_COLOR_BITS_EXT 0x2014
#define WGL_RED_BITS_EXT 0x2015
#define WGL_RED_SHIFT_EXT 0x2016
#define WGL_GREEN_BITS_EXT 0x2017
#define WGL_GREEN_SHIFT_EXT 0x2018
#define WGL_BLUE_BITS_EXT 0x2019
#define WGL_BLUE_SHIFT_EXT 0x201A
#define WGL_ALPHA_BITS_EXT 0x201B
#define WGL_ALPHA_SHIFT_EXT 0x201C
#define WGL_ACCUM_BITS_EXT 0x201D
#define WGL_ACCUM_RED_BITS_EXT 0x201E
#define WGL_ACCUM_GREEN_BITS_EXT 0x201F
#define WGL_ACCUM_BLUE_BITS_EXT 0x2020
#define WGL_ACCUM_ALPHA_BITS_EXT 0x2021
#define WGL_DEPTH_BITS_EXT 0x2022
#define WGL_STENCIL_BITS_EXT 0x2023
#define WGL_AUX_BUFFERS_EXT 0x2024
#define WGL_NO_ACCELERATION_EXT 0x2025
#define WGL_GENERIC_ACCELERATION_EXT 0x2026
#define WGL_FULL_ACCELERATION_EXT 0x2027
#define WGL_SWAP_EXCHANGE_EXT 0x2028
#define WGL_SWAP_COPY_EXT 0x2029
#define WGL_SWAP_UNDEFINED_EXT 0x202A
#define WGL_TYPE_RGBA_EXT 0x202B
#define WGL_TYPE_COLORINDEX_EXT 0x202C

typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATEXTPROC) (HDC hdc, const int* piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
typedef BOOL (WINAPI * PFNWGLGETPIXELFORMATATTRIBFVEXTPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int* piAttributes, FLOAT *pfValues);
typedef BOOL (WINAPI * PFNWGLGETPIXELFORMATATTRIBIVEXTPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, int* piAttributes, int *piValues);

#define wglChoosePixelFormatEXT WGLEW_GET_FUN(__wglewChoosePixelFormatEXT)
#define wglGetPixelFormatAttribfvEXT WGLEW_GET_FUN(__wglewGetPixelFormatAttribfvEXT)
#define wglGetPixelFormatAttribivEXT WGLEW_GET_FUN(__wglewGetPixelFormatAttribivEXT)

#define WGLEW_EXT_pixel_format WGLEW_GET_VAR(__WGLEW_EXT_pixel_format)

#endif /* WGL_EXT_pixel_format */

/* ------------------- WGL_EXT_pixel_format_packed_float ------------------- */

#ifndef WGL_EXT_pixel_format_packed_float
#define WGL_EXT_pixel_format_packed_float 1

#define WGL_TYPE_RGBA_UNSIGNED_FLOAT_EXT 0x20A8

#define WGLEW_EXT_pixel_format_packed_float WGLEW_GET_VAR(__WGLEW_EXT_pixel_format_packed_float)

#endif /* WGL_EXT_pixel_format_packed_float */

/* -------------------------- WGL_EXT_swap_control ------------------------- */

#ifndef WGL_EXT_swap_control
#define WGL_EXT_swap_control 1

typedef int (WINAPI * PFNWGLGETSWAPINTERVALEXTPROC) (void);
typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);

#define wglGetSwapIntervalEXT WGLEW_GET_FUN(__wglewGetSwapIntervalEXT)
#define wglSwapIntervalEXT WGLEW_GET_FUN(__wglewSwapIntervalEXT)

#define WGLEW_EXT_swap_control WGLEW_GET_VAR(__WGLEW_EXT_swap_control)

#endif /* WGL_EXT_swap_control */

/* ----------------------- WGL_EXT_swap_control_tear ----------------------- */

#ifndef WGL_EXT_swap_control_tear
#define WGL_EXT_swap_control_tear 1

#define WGLEW_EXT_swap_control_tear WGLEW_GET_VAR(__WGLEW_EXT_swap_control_tear)

#endif /* WGL_EXT_swap_control_tear */

/* ------------------------------------------------------------------------- */

#ifdef GLEW_MX
#define WGLEW_FUN_EXPORT
#define WGLEW_VAR_EXPORT
#else
#define WGLEW_FUN_EXPORT GLEW_FUN_EXPORT
#define WGLEW_VAR_EXPORT GLEW_VAR_EXPORT
#endif /* GLEW_MX */

#ifdef GLEW_MX
struct WGLEWContextStruct
{
#endif /* GLEW_MX */

WGLEW_FUN_EXPORT PFNWGLCREATEBUFFERREGIONARBPROC __wglewCreateBufferRegionARB;
WGLEW_FUN_EXPORT PFNWGLDELETEBUFFERREGIONARBPROC __wglewDeleteBufferRegionARB;
WGLEW_FUN_EXPORT PFNWGLRESTOREBUFFERREGIONARBPROC __wglewRestoreBufferRegionARB;
WGLEW_FUN_EXPORT PFNWGLSAVEBUFFERREGIONARBPROC __wglewSaveBufferRegionARB;

WGLEW_FUN_EXPORT PFNWGLCREATECONTEXTATTRIBSARBPROC __wglewCreateContextAttribsARB;

WGLEW_FUN_EXPORT PFNWGLGETEXTENSIONSSTRINGARBPROC __wglewGetExtensionsStringARB;

WGLEW_FUN_EXPORT PFNWGLGETCURRENTREADDCARBPROC __wglewGetCurrentReadDCARB;
WGLEW_FUN_EXPORT PFNWGLMAKECONTEXTCURRENTARBPROC __wglewMakeContextCurrentARB;

WGLEW_FUN_EXPORT PFNWGLCREATEPBUFFERARBPROC __wglewCreatePbufferARB;
WGLEW_FUN_EXPORT PFNWGLDESTROYPBUFFERARBPROC __wglewDestroyPbufferARB;
WGLEW_FUN_EXPORT PFNWGLGETPBUFFERDCARBPROC __wglewGetPbufferDCARB;
WGLEW_FUN_EXPORT PFNWGLQUERYPBUFFERARBPROC __wglewQueryPbufferARB;
WGLEW_FUN_EXPORT PFNWGLRELEASEPBUFFERDCARBPROC __wglewReleasePbufferDCARB;

WGLEW_FUN_EXPORT PFNWGLCHOOSEPIXELFORMATARBPROC __wglewChoosePixelFormatARB;
WGLEW_FUN_EXPORT PFNWGLGETPIXELFORMATATTRIBFVARBPROC __wglewGetPixelFormatAttribfvARB;
WGLEW_FUN_EXPORT PFNWGLGETPIXELFORMATATTRIBIVARBPROC __wglewGetPixelFormatAttribivARB;

WGLEW_FUN_EXPORT PFNWGLBINDTEXIMAGEARBPROC __wglewBindTexImageARB;
WGLEW_FUN_EXPORT PFNWGLRELEASETEXIMAGEARBPROC __wglewReleaseTexImageARB;
WGLEW_FUN_EXPORT PFNWGLSETPBUFFERATTRIBARBPROC __wglewSetPbufferAttribARB;

WGLEW_FUN_EXPORT PFNWGLBINDDISPLAYCOLORTABLEEXTPROC __wglewBindDisplayColorTableEXT;
WGLEW_FUN_EXPORT PFNWGLCREATEDISPLAYCOLORTABLEEXTPROC __wglewCreateDisplayColorTableEXT;
WGLEW_FUN_EXPORT PFNWGLDESTROYDISPLAYCOLORTABLEEXTPROC __wglewDestroyDisplayColorTableEXT;
WGLEW_FUN_EXPORT PFNWGLLOADDISPLAYCOLORTABLEEXTPROC __wglewLoadDisplayColorTableEXT;

WGLEW_FUN_EXPORT PFNWGLGETEXTENSIONSSTRINGEXTPROC __wglewGetExtensionsStringEXT;

WGLEW_FUN_EXPORT PFNWGLGETCURRENTREADDCEXTPROC __wglewGetCurrentReadDCEXT;
WGLEW_FUN_EXPORT PFNWGLMAKECONTEXTCURRENTEXTPROC __wglewMakeContextCurrentEXT;

WGLEW_FUN_EXPORT PFNWGLCREATEPBUFFEREXTPROC __wglewCreatePbufferEXT;
WGLEW_FUN_EXPORT PFNWGLDESTROYPBUFFEREXTPROC __wglewDestroyPbufferEXT;
WGLEW_FUN_EXPORT PFNWGLGETPBUFFERDCEXTPROC __wglewGetPbufferDCEXT;
WGLEW_FUN_EXPORT PFNWGLQUERYPBUFFEREXTPROC __wglewQueryPbufferEXT;
WGLEW_FUN_EXPORT PFNWGLRELEASEPBUFFERDCEXTPROC __wglewReleasePbufferDCEXT;

WGLEW_FUN_EXPORT PFNWGLCHOOSEPIXELFORMATEXTPROC __wglewChoosePixelFormatEXT;
WGLEW_FUN_EXPORT PFNWGLGETPIXELFORMATATTRIBFVEXTPROC __wglewGetPixelFormatAttribfvEXT;
WGLEW_FUN_EXPORT PFNWGLGETPIXELFORMATATTRIBIVEXTPROC __wglewGetPixelFormatAttribivEXT;

WGLEW_FUN_EXPORT PFNWGLGETSWAPINTERVALEXTPROC __wglewGetSwapIntervalEXT;
WGLEW_FUN_EXPORT PFNWGLSWAPINTERVALEXTPROC __wglewSwapIntervalEXT;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_buffer_region;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_context_flush_control;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_create_context;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_create_context_no_error;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_create_context_profile;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_create_context_robustness;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_extensions_string;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_framebuffer_sRGB;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_make_current_read;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_multisample;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_pbuffer;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_pixel_format;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_pixel_format_float;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_render_texture;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_robustness_application_isolation;
WGLEW_VAR_EXPORT GLboolean __WGLEW_ARB_robustness_share_group_isolation;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_colorspace;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_create_context_es2_profile;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_create_context_es_profile;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_depth_float;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_display_color_table;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_extensions_string;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_framebuffer_sRGB;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_make_current_read;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_multisample;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_pbuffer;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_pixel_format;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_pixel_format_packed_float;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_swap_control;
WGLEW_VAR_EXPORT GLboolean __WGLEW_EXT_swap_control_tear;

#ifdef GLEW_MX
}; /* WGLEWContextStruct */
#endif /* GLEW_MX */

/* ------------------------------------------------------------------------- */

#ifdef GLEW_MX

typedef struct WGLEWContextStruct WGLEWContext;
GLEWAPI GLenum GLEWAPIENTRY wglewContextInit (WGLEWContext *ctx);
GLEWAPI GLboolean GLEWAPIENTRY wglewContextIsSupported (const WGLEWContext *ctx, const char *name);

#define wglewInit() wglewContextInit(wglewGetContext())
#define wglewIsSupported(x) wglewContextIsSupported(wglewGetContext(), x)

#define WGLEW_GET_VAR(x) (*(const GLboolean*)&(wglewGetContext()->x))
#define WGLEW_GET_FUN(x) wglewGetContext()->x

#else /* GLEW_MX */

GLEWAPI GLenum GLEWAPIENTRY wglewInit ();
GLEWAPI GLboolean GLEWAPIENTRY wglewIsSupported (const char *name);

#define WGLEW_GET_VAR(x) (*(const GLboolean*)&x)
#define WGLEW_GET_FUN(x) x

#endif /* GLEW_MX */

GLEWAPI GLboolean GLEWAPIENTRY wglewGetExtension (const char *name);

#ifdef __cplusplus
}
#endif

#undef GLEWAPI

