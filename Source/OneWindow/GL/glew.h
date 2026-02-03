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
 * Mesa 3-D graphics library
 * Version:  7.0
 *
 * Copyright (C) 1999-2007  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
#pragma system_header
#define __GLEW_H__

#if defined(__gl_h_) || defined(__GL_H__) || defined(_GL_H) || defined(__X_GL_H)
#error gl.h included before glew.h
#endif
#if defined(__gl2_h_)
#error gl2.h included before glew.h
#endif
#if defined(__gltypes_h_)
#error gltypes.h included before glew.h
#endif
#if defined(__REGAL_H__)
#error Regal.h included before glew.h
#endif
#if defined(__glext_h_) || defined(__GLEXT_H_)
#error glext.h included before glew.h
#endif
#if defined(__gl_ATI_h_)
#error glATI.h included before glew.h
#endif

#define __gl_h_
#define __gl2_h_
#define __GL_H__
#define _GL_H
#define __gltypes_h_
#define __REGAL_H__
#define __X_GL_H
#define __glext_h_
#define __GLEXT_H_
#define __gl_ATI_h_

#if defined(_WIN32)

/*
 * GLEW does not include <windows.h> to avoid name space pollution.
 * GL needs GLAPI and GLAPIENTRY, GLU needs APIENTRY, CALLBACK, and wchar_t
 * defined properly.
 */
/* <windef.h> and <gl.h>*/
#ifdef APIENTRY
#  ifndef GLAPIENTRY
#    define GLAPIENTRY APIENTRY
#  endif
#  ifndef GLEWAPIENTRY
#    define GLEWAPIENTRY APIENTRY
#  endif
#else
#define GLEW_APIENTRY_DEFINED
#  if defined(__MINGW32__) || defined(__CYGWIN__) || (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
#    define APIENTRY __stdcall
#    ifndef GLAPIENTRY
#      define GLAPIENTRY __stdcall
#    endif
#    ifndef GLEWAPIENTRY
#      define GLEWAPIENTRY __stdcall
#    endif
#  else
#    define APIENTRY
#  endif
#endif
#ifndef GLAPI
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define GLAPI extern
#  endif
#endif
/* <winnt.h> */
#ifndef CALLBACK
#define GLEW_CALLBACK_DEFINED
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define CALLBACK __attribute__ ((__stdcall__))
#  elif (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
#    define CALLBACK __stdcall
#  else
#    define CALLBACK
#  endif
#endif
/* <wingdi.h> and <winnt.h> */
#ifndef WINGDIAPI
#define GLEW_WINGDIAPI_DEFINED
#define WINGDIAPI __declspec(dllimport)
#endif
/* <ctype.h> */
#if (defined(_MSC_VER) || defined(__BORLANDC__)) && !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#  define _WCHAR_T_DEFINED
#endif
/* <stddef.h> */
#if !defined(_W64)
#  if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && defined(_MSC_VER) && _MSC_VER >= 1300
#    define _W64 __w64
#  else
#    define _W64
#  endif
#endif
#if !defined(_PTRDIFF_T_DEFINED) && !defined(_PTRDIFF_T_) && !defined(__MINGW64__)
#  ifdef _WIN64
typedef __int64 ptrdiff_t;
#  else
typedef _W64 int ptrdiff_t;
#  endif
#  define _PTRDIFF_T_DEFINED
#  define _PTRDIFF_T_
#endif

#ifndef GLAPI
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define GLAPI extern
#  else
#    define GLAPI WINGDIAPI
#  endif
#endif

/*
 * GLEW_STATIC is defined for static library.
 * GLEW_BUILD  is defined for building the DLL library.
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

#else /* _UNIX */

/*
 * Needed for ptrdiff_t in turn needed by VBO.  This is defined by ISO
 * C.  On my system, this amounts to _3 lines_ of included code, all of
 * them pretty much harmless.  If you know of a way of detecting 32 vs
 * 64 _targets_ at compile time you are free to replace this with
 * something that's portable.  For now, _this_ is the portable solution.
 * (mem, 2004-01-04)
 */

#include <stddef.h>

/* SGI MIPSPro doesn't like stdint.h in C++ mode          */
/* ID: 3376260 Solaris 9 has inttypes.h, but not stdint.h */

#if (defined(__sgi) || defined(__sun)) && !defined(__GNUC__)
#include <inttypes.h>
#else
#include <stdint.h>
#endif

#define GLEW_APIENTRY_DEFINED
#define APIENTRY

/*
 * GLEW_STATIC is defined for static library.
 */

#ifdef GLEW_STATIC
#  define GLEWAPI extern
#else
#  if defined(__GNUC__) && __GNUC__>=4
#   define GLEWAPI extern __attribute__ ((visibility("default")))
#  elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#   define GLEWAPI extern __global
#  else
#   define GLEWAPI extern
#  endif
#endif

/* <glu.h> */
#ifndef GLAPI
#define GLAPI extern
#endif

#endif /* _WIN32 */

#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif

#ifndef GLEWAPIENTRY
#define GLEWAPIENTRY
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------- GL_VERSION_1_1 ---------------------------- */

#ifndef GL_VERSION_1_1
#define GL_VERSION_1_1 1

typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
#if defined(_MSC_VER) && _MSC_VER < 1400
typedef __int64 GLint64EXT;
typedef unsigned __int64 GLuint64EXT;
#elif defined(_MSC_VER) || defined(__BORLANDC__)
typedef signed long long GLint64EXT;
typedef unsigned long long GLuint64EXT;
#else
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#include <inttypes.h>
#  endif
typedef int64_t GLint64EXT;
typedef uint64_t GLuint64EXT;
#endif
typedef GLint64EXT  GLint64;
typedef GLuint64EXT GLuint64;
typedef struct __GLsync *GLsync;

typedef char GLchar;

#define GL_ZERO 0
#define GL_FALSE 0
#define GL_LOGIC_OP 0x0BF1
#define GL_NONE 0
#define GL_TEXTURE_COMPONENTS 0x1003
#define GL_NO_ERROR 0
#define GL_POINTS 0x0000
#define GL_CURRENT_BIT 0x00000001
#define GL_TRUE 1
#define GL_ONE 1
#define GL_CLIENT_PIXEL_STORE_BIT 0x00000001
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_POINT_BIT 0x00000002
#define GL_CLIENT_VERTEX_ARRAY_BIT 0x00000002
#define GL_LINE_STRIP 0x0003
#define GL_LINE_BIT 0x00000004
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_QUADS 0x0007
#define GL_QUAD_STRIP 0x0008
#define GL_POLYGON_BIT 0x00000008
#define GL_POLYGON 0x0009
#define GL_POLYGON_STIPPLE_BIT 0x00000010
#define GL_PIXEL_MODE_BIT 0x00000020
#define GL_LIGHTING_BIT 0x00000040
#define GL_FOG_BIT 0x00000080
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_ACCUM 0x0100
#define GL_LOAD 0x0101
#define GL_RETURN 0x0102
#define GL_MULT 0x0103
#define GL_ADD 0x0104
#define GL_NEVER 0x0200
#define GL_ACCUM_BUFFER_BIT 0x00000200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_FRONT_LEFT 0x0400
#define GL_FRONT_RIGHT 0x0401
#define GL_BACK_LEFT 0x0402
#define GL_BACK_RIGHT 0x0403
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_LEFT 0x0406
#define GL_RIGHT 0x0407
#define GL_FRONT_AND_BACK 0x0408
#define GL_AUX0 0x0409
#define GL_AUX1 0x040A
#define GL_AUX2 0x040B
#define GL_AUX3 0x040C
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE 0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_OUT_OF_MEMORY 0x0505
#define GL_2D 0x0600
#define GL_3D 0x0601
#define GL_3D_COLOR 0x0602
#define GL_3D_COLOR_TEXTURE 0x0603
#define GL_4D_COLOR_TEXTURE 0x0604
#define GL_PASS_THROUGH_TOKEN 0x0700
#define GL_POINT_TOKEN 0x0701
#define GL_LINE_TOKEN 0x0702
#define GL_POLYGON_TOKEN 0x0703
#define GL_BITMAP_TOKEN 0x0704
#define GL_DRAW_PIXEL_TOKEN 0x0705
#define GL_COPY_PIXEL_TOKEN 0x0706
#define GL_LINE_RESET_TOKEN 0x0707
#define GL_EXP 0x0800
#define GL_VIEWPORT_BIT 0x00000800
#define GL_EXP2 0x0801
#define GL_CW 0x0900
#define GL_CCW 0x0901
#define GL_COEFF 0x0A00
#define GL_ORDER 0x0A01
#define GL_DOMAIN 0x0A02
#define GL_CURRENT_COLOR 0x0B00
#define GL_CURRENT_INDEX 0x0B01
#define GL_CURRENT_NORMAL 0x0B02
#define GL_CURRENT_TEXTURE_COORDS 0x0B03
#define GL_CURRENT_RASTER_COLOR 0x0B04
#define GL_CURRENT_RASTER_INDEX 0x0B05
#define GL_CURRENT_RASTER_TEXTURE_COORDS 0x0B06
#define GL_CURRENT_RASTER_POSITION 0x0B07
#define GL_CURRENT_RASTER_POSITION_VALID 0x0B08
#define GL_CURRENT_RASTER_DISTANCE 0x0B09
#define GL_POINT_SMOOTH 0x0B10
#define GL_POINT_SIZE 0x0B11
#define GL_POINT_SIZE_RANGE 0x0B12
#define GL_POINT_SIZE_GRANULARITY 0x0B13
#define GL_LINE_SMOOTH 0x0B20
#define GL_LINE_WIDTH 0x0B21
#define GL_LINE_WIDTH_RANGE 0x0B22
#define GL_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_LINE_STIPPLE 0x0B24
#define GL_LINE_STIPPLE_PATTERN 0x0B25
#define GL_LINE_STIPPLE_REPEAT 0x0B26
#define GL_LIST_MODE 0x0B30
#define GL_MAX_LIST_NESTING 0x0B31
#define GL_LIST_BASE 0x0B32
#define GL_LIST_INDEX 0x0B33
#define GL_POLYGON_MODE 0x0B40
#define GL_POLYGON_SMOOTH 0x0B41
#define GL_POLYGON_STIPPLE 0x0B42
#define GL_EDGE_FLAG 0x0B43
#define GL_CULL_FACE 0x0B44
#define GL_CULL_FACE_MODE 0x0B45
#define GL_FRONT_FACE 0x0B46
#define GL_LIGHTING 0x0B50
#define GL_LIGHT_MODEL_LOCAL_VIEWER 0x0B51
#define GL_LIGHT_MODEL_TWO_SIDE 0x0B52
#define GL_LIGHT_MODEL_AMBIENT 0x0B53
#define GL_SHADE_MODEL 0x0B54
#define GL_COLOR_MATERIAL_FACE 0x0B55
#define GL_COLOR_MATERIAL_PARAMETER 0x0B56
#define GL_COLOR_MATERIAL 0x0B57
#define GL_FOG 0x0B60
#define GL_FOG_INDEX 0x0B61
#define GL_FOG_DENSITY 0x0B62
#define GL_FOG_START 0x0B63
#define GL_FOG_END 0x0B64
#define GL_FOG_MODE 0x0B65
#define GL_FOG_COLOR 0x0B66
#define GL_DEPTH_RANGE 0x0B70
#define GL_DEPTH_TEST 0x0B71
#define GL_DEPTH_WRITEMASK 0x0B72
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#define GL_DEPTH_FUNC 0x0B74
#define GL_ACCUM_CLEAR_VALUE 0x0B80
#define GL_STENCIL_TEST 0x0B90
#define GL_STENCIL_CLEAR_VALUE 0x0B91
#define GL_STENCIL_FUNC 0x0B92
#define GL_STENCIL_VALUE_MASK 0x0B93
#define GL_STENCIL_FAIL 0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF 0x0B97
#define GL_STENCIL_WRITEMASK 0x0B98
#define GL_MATRIX_MODE 0x0BA0
#define GL_NORMALIZE 0x0BA1
#define GL_VIEWPORT 0x0BA2
#define GL_MODELVIEW_STACK_DEPTH 0x0BA3
#define GL_PROJECTION_STACK_DEPTH 0x0BA4
#define GL_TEXTURE_STACK_DEPTH 0x0BA5
#define GL_MODELVIEW_MATRIX 0x0BA6
#define GL_PROJECTION_MATRIX 0x0BA7
#define GL_TEXTURE_MATRIX 0x0BA8
#define GL_ATTRIB_STACK_DEPTH 0x0BB0
#define GL_CLIENT_ATTRIB_STACK_DEPTH 0x0BB1
#define GL_ALPHA_TEST 0x0BC0
#define GL_ALPHA_TEST_FUNC 0x0BC1
#define GL_ALPHA_TEST_REF 0x0BC2
#define GL_DITHER 0x0BD0
#define GL_BLEND_DST 0x0BE0
#define GL_BLEND_SRC 0x0BE1
#define GL_BLEND 0x0BE2
#define GL_LOGIC_OP_MODE 0x0BF0
#define GL_INDEX_LOGIC_OP 0x0BF1
#define GL_COLOR_LOGIC_OP 0x0BF2
#define GL_AUX_BUFFERS 0x0C00
#define GL_DRAW_BUFFER 0x0C01
#define GL_READ_BUFFER 0x0C02
#define GL_SCISSOR_BOX 0x0C10
#define GL_SCISSOR_TEST 0x0C11
#define GL_INDEX_CLEAR_VALUE 0x0C20
#define GL_INDEX_WRITEMASK 0x0C21
#define GL_COLOR_CLEAR_VALUE 0x0C22
#define GL_COLOR_WRITEMASK 0x0C23
#define GL_INDEX_MODE 0x0C30
#define GL_RGBA_MODE 0x0C31
#define GL_DOUBLEBUFFER 0x0C32
#define GL_STEREO 0x0C33
#define GL_RENDER_MODE 0x0C40
#define GL_PERSPECTIVE_CORRECTION_HINT 0x0C50
#define GL_POINT_SMOOTH_HINT 0x0C51
#define GL_LINE_SMOOTH_HINT 0x0C52
#define GL_POLYGON_SMOOTH_HINT 0x0C53
#define GL_FOG_HINT 0x0C54
#define GL_TEXTURE_GEN_S 0x0C60
#define GL_TEXTURE_GEN_T 0x0C61
#define GL_TEXTURE_GEN_R 0x0C62
#define GL_TEXTURE_GEN_Q 0x0C63
#define GL_PIXEL_MAP_I_TO_I 0x0C70
#define GL_PIXEL_MAP_S_TO_S 0x0C71
#define GL_PIXEL_MAP_I_TO_R 0x0C72
#define GL_PIXEL_MAP_I_TO_G 0x0C73
#define GL_PIXEL_MAP_I_TO_B 0x0C74
#define GL_PIXEL_MAP_I_TO_A 0x0C75
#define GL_PIXEL_MAP_R_TO_R 0x0C76
#define GL_PIXEL_MAP_G_TO_G 0x0C77
#define GL_PIXEL_MAP_B_TO_B 0x0C78
#define GL_PIXEL_MAP_A_TO_A 0x0C79
#define GL_PIXEL_MAP_I_TO_I_SIZE 0x0CB0
#define GL_PIXEL_MAP_S_TO_S_SIZE 0x0CB1
#define GL_PIXEL_MAP_I_TO_R_SIZE 0x0CB2
#define GL_PIXEL_MAP_I_TO_G_SIZE 0x0CB3
#define GL_PIXEL_MAP_I_TO_B_SIZE 0x0CB4
#define GL_PIXEL_MAP_I_TO_A_SIZE 0x0CB5
#define GL_PIXEL_MAP_R_TO_R_SIZE 0x0CB6
#define GL_PIXEL_MAP_G_TO_G_SIZE 0x0CB7
#define GL_PIXEL_MAP_B_TO_B_SIZE 0x0CB8
#define GL_PIXEL_MAP_A_TO_A_SIZE 0x0CB9
#define GL_UNPACK_SWAP_BYTES 0x0CF0
#define GL_UNPACK_LSB_FIRST 0x0CF1
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_PACK_SWAP_BYTES 0x0D00
#define GL_PACK_LSB_FIRST 0x0D01
#define GL_PACK_ROW_LENGTH 0x0D02
#define GL_PACK_SKIP_ROWS 0x0D03
#define GL_PACK_SKIP_PIXELS 0x0D04
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_MAP_COLOR 0x0D10
#define GL_MAP_STENCIL 0x0D11
#define GL_INDEX_SHIFT 0x0D12
#define GL_INDEX_OFFSET 0x0D13
#define GL_RED_SCALE 0x0D14
#define GL_RED_BIAS 0x0D15
#define GL_ZOOM_X 0x0D16
#define GL_ZOOM_Y 0x0D17
#define GL_GREEN_SCALE 0x0D18
#define GL_GREEN_BIAS 0x0D19
#define GL_BLUE_SCALE 0x0D1A
#define GL_BLUE_BIAS 0x0D1B
#define GL_ALPHA_SCALE 0x0D1C
#define GL_ALPHA_BIAS 0x0D1D
#define GL_DEPTH_SCALE 0x0D1E
#define GL_DEPTH_BIAS 0x0D1F
#define GL_MAX_EVAL_ORDER 0x0D30
#define GL_MAX_LIGHTS 0x0D31
#define GL_MAX_CLIP_PLANES 0x0D32
#define GL_MAX_TEXTURE_SIZE 0x0D33
#define GL_MAX_PIXEL_MAP_TABLE 0x0D34
#define GL_MAX_ATTRIB_STACK_DEPTH 0x0D35
#define GL_MAX_MODELVIEW_STACK_DEPTH 0x0D36
#define GL_MAX_NAME_STACK_DEPTH 0x0D37
#define GL_MAX_PROJECTION_STACK_DEPTH 0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH 0x0D39
#define GL_MAX_VIEWPORT_DIMS 0x0D3A
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH 0x0D3B
#define GL_SUBPIXEL_BITS 0x0D50
#define GL_INDEX_BITS 0x0D51
#define GL_RED_BITS 0x0D52
#define GL_GREEN_BITS 0x0D53
#define GL_BLUE_BITS 0x0D54
#define GL_ALPHA_BITS 0x0D55
#define GL_DEPTH_BITS 0x0D56
#define GL_STENCIL_BITS 0x0D57
#define GL_ACCUM_RED_BITS 0x0D58
#define GL_ACCUM_GREEN_BITS 0x0D59
#define GL_ACCUM_BLUE_BITS 0x0D5A
#define GL_ACCUM_ALPHA_BITS 0x0D5B
#define GL_NAME_STACK_DEPTH 0x0D70
#define GL_AUTO_NORMAL 0x0D80
#define GL_MAP1_COLOR_4 0x0D90
#define GL_MAP1_INDEX 0x0D91
#define GL_MAP1_NORMAL 0x0D92
#define GL_MAP1_TEXTURE_COORD_1 0x0D93
#define GL_MAP1_TEXTURE_COORD_2 0x0D94
#define GL_MAP1_TEXTURE_COORD_3 0x0D95
#define GL_MAP1_TEXTURE_COORD_4 0x0D96
#define GL_MAP1_VERTEX_3 0x0D97
#define GL_MAP1_VERTEX_4 0x0D98
#define GL_MAP2_COLOR_4 0x0DB0
#define GL_MAP2_INDEX 0x0DB1
#define GL_MAP2_NORMAL 0x0DB2
#define GL_MAP2_TEXTURE_COORD_1 0x0DB3
#define GL_MAP2_TEXTURE_COORD_2 0x0DB4
#define GL_MAP2_TEXTURE_COORD_3 0x0DB5
#define GL_MAP2_TEXTURE_COORD_4 0x0DB6
#define GL_MAP2_VERTEX_3 0x0DB7
#define GL_MAP2_VERTEX_4 0x0DB8
#define GL_MAP1_GRID_DOMAIN 0x0DD0
#define GL_MAP1_GRID_SEGMENTS 0x0DD1
#define GL_MAP2_GRID_DOMAIN 0x0DD2
#define GL_MAP2_GRID_SEGMENTS 0x0DD3
#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_FEEDBACK_BUFFER_POINTER 0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE 0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE 0x0DF2
#define GL_SELECTION_BUFFER_POINTER 0x0DF3
#define GL_SELECTION_BUFFER_SIZE 0x0DF4
#define GL_TEXTURE_WIDTH 0x1000
#define GL_TRANSFORM_BIT 0x00001000
#define GL_TEXTURE_HEIGHT 0x1001
#define GL_TEXTURE_INTERNAL_FORMAT 0x1003
#define GL_TEXTURE_BORDER_COLOR 0x1004
#define GL_TEXTURE_BORDER 0x1005
#define GL_DONT_CARE 0x1100
#define GL_FASTEST 0x1101
#define GL_NICEST 0x1102
#define GL_AMBIENT 0x1200
#define GL_DIFFUSE 0x1201
#define GL_SPECULAR 0x1202
#define GL_POSITION 0x1203
#define GL_SPOT_DIRECTION 0x1204
#define GL_SPOT_EXPONENT 0x1205
#define GL_SPOT_CUTOFF 0x1206
#define GL_CONSTANT_ATTENUATION 0x1207
#define GL_LINEAR_ATTENUATION 0x1208
#define GL_QUADRATIC_ATTENUATION 0x1209
#define GL_COMPILE 0x1300
#define GL_COMPILE_AND_EXECUTE 0x1301
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_2_BYTES 0x1407
#define GL_3_BYTES 0x1408
#define GL_4_BYTES 0x1409
#define GL_DOUBLE 0x140A
#define GL_CLEAR 0x1500
#define GL_AND 0x1501
#define GL_AND_REVERSE 0x1502
#define GL_COPY 0x1503
#define GL_AND_INVERTED 0x1504
#define GL_NOOP 0x1505
#define GL_XOR 0x1506
#define GL_OR 0x1507
#define GL_NOR 0x1508
#define GL_EQUIV 0x1509
#define GL_INVERT 0x150A
#define GL_OR_REVERSE 0x150B
#define GL_COPY_INVERTED 0x150C
#define GL_OR_INVERTED 0x150D
#define GL_NAND 0x150E
#define GL_SET 0x150F
#define GL_EMISSION 0x1600
#define GL_SHININESS 0x1601
#define GL_AMBIENT_AND_DIFFUSE 0x1602
#define GL_COLOR_INDEXES 0x1603
#define GL_MODELVIEW 0x1700
#define GL_PROJECTION 0x1701
#define GL_TEXTURE 0x1702
#define GL_COLOR 0x1800
#define GL_DEPTH 0x1801
#define GL_STENCIL 0x1802
#define GL_COLOR_INDEX 0x1900
#define GL_STENCIL_INDEX 0x1901
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RED 0x1903
#define GL_GREEN 0x1904
#define GL_BLUE 0x1905
#define GL_ALPHA 0x1906
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_LUMINANCE 0x1909
#define GL_LUMINANCE_ALPHA 0x190A
#define GL_BITMAP 0x1A00
#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
#define GL_RENDER 0x1C00
#define GL_FEEDBACK 0x1C01
#define GL_SELECT 0x1C02
#define GL_FLAT 0x1D00
#define GL_SMOOTH 0x1D01
#define GL_KEEP 0x1E00
#define GL_REPLACE 0x1E01
#define GL_INCR 0x1E02
#define GL_DECR 0x1E03
#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03
#define GL_S 0x2000
#define GL_ENABLE_BIT 0x00002000
#define GL_T 0x2001
#define GL_R 0x2002
#define GL_Q 0x2003
#define GL_MODULATE 0x2100
#define GL_DECAL 0x2101
#define GL_TEXTURE_ENV_MODE 0x2200
#define GL_TEXTURE_ENV_COLOR 0x2201
#define GL_TEXTURE_ENV 0x2300
#define GL_EYE_LINEAR 0x2400
#define GL_OBJECT_LINEAR 0x2401
#define GL_SPHERE_MAP 0x2402
#define GL_TEXTURE_GEN_MODE 0x2500
#define GL_OBJECT_PLANE 0x2501
#define GL_EYE_PLANE 0x2502
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_CLAMP 0x2900
#define GL_REPEAT 0x2901
#define GL_POLYGON_OFFSET_UNITS 0x2A00
#define GL_POLYGON_OFFSET_POINT 0x2A01
#define GL_POLYGON_OFFSET_LINE 0x2A02
#define GL_R3_G3_B2 0x2A10
#define GL_V2F 0x2A20
#define GL_V3F 0x2A21
#define GL_C4UB_V2F 0x2A22
#define GL_C4UB_V3F 0x2A23
#define GL_C3F_V3F 0x2A24
#define GL_N3F_V3F 0x2A25
#define GL_C4F_N3F_V3F 0x2A26
#define GL_T2F_V3F 0x2A27
#define GL_T4F_V4F 0x2A28
#define GL_T2F_C4UB_V3F 0x2A29
#define GL_T2F_C3F_V3F 0x2A2A
#define GL_T2F_N3F_V3F 0x2A2B
#define GL_T2F_C4F_N3F_V3F 0x2A2C
#define GL_T4F_C4F_N3F_V4F 0x2A2D
#define GL_CLIP_PLANE0 0x3000
#define GL_CLIP_PLANE1 0x3001
#define GL_CLIP_PLANE2 0x3002
#define GL_CLIP_PLANE3 0x3003
#define GL_CLIP_PLANE4 0x3004
#define GL_CLIP_PLANE5 0x3005
#define GL_LIGHT0 0x4000
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_LIGHT1 0x4001
#define GL_LIGHT2 0x4002
#define GL_LIGHT3 0x4003
#define GL_LIGHT4 0x4004
#define GL_LIGHT5 0x4005
#define GL_LIGHT6 0x4006
#define GL_LIGHT7 0x4007
#define GL_HINT_BIT 0x00008000
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_ALPHA4 0x803B
#define GL_ALPHA8 0x803C
#define GL_ALPHA12 0x803D
#define GL_ALPHA16 0x803E
#define GL_LUMINANCE4 0x803F
#define GL_LUMINANCE8 0x8040
#define GL_LUMINANCE12 0x8041
#define GL_LUMINANCE16 0x8042
#define GL_LUMINANCE4_ALPHA4 0x8043
#define GL_LUMINANCE6_ALPHA2 0x8044
#define GL_LUMINANCE8_ALPHA8 0x8045
#define GL_LUMINANCE12_ALPHA4 0x8046
#define GL_LUMINANCE12_ALPHA12 0x8047
#define GL_LUMINANCE16_ALPHA16 0x8048
#define GL_INTENSITY 0x8049
#define GL_INTENSITY4 0x804A
#define GL_INTENSITY8 0x804B
#define GL_INTENSITY12 0x804C
#define GL_INTENSITY16 0x804D
#define GL_RGB4 0x804F
#define GL_RGB5 0x8050
#define GL_RGB8 0x8051
#define GL_RGB10 0x8052
#define GL_RGB12 0x8053
#define GL_RGB16 0x8054
#define GL_RGBA2 0x8055
#define GL_RGBA4 0x8056
#define GL_RGB5_A1 0x8057
#define GL_RGBA8 0x8058
#define GL_RGB10_A2 0x8059
#define GL_RGBA12 0x805A
#define GL_RGBA16 0x805B
#define GL_TEXTURE_RED_SIZE 0x805C
#define GL_TEXTURE_GREEN_SIZE 0x805D
#define GL_TEXTURE_BLUE_SIZE 0x805E
#define GL_TEXTURE_ALPHA_SIZE 0x805F
#define GL_TEXTURE_LUMINANCE_SIZE 0x8060
#define GL_TEXTURE_INTENSITY_SIZE 0x8061
#define GL_PROXY_TEXTURE_1D 0x8063
#define GL_PROXY_TEXTURE_2D 0x8064
#define GL_TEXTURE_PRIORITY 0x8066
#define GL_TEXTURE_RESIDENT 0x8067
#define GL_TEXTURE_BINDING_1D 0x8068
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_VERTEX_ARRAY 0x8074
#define GL_NORMAL_ARRAY 0x8075
#define GL_COLOR_ARRAY 0x8076
#define GL_INDEX_ARRAY 0x8077
#define GL_TEXTURE_COORD_ARRAY 0x8078
#define GL_EDGE_FLAG_ARRAY 0x8079
#define GL_VERTEX_ARRAY_SIZE 0x807A
#define GL_VERTEX_ARRAY_TYPE 0x807B
#define GL_VERTEX_ARRAY_STRIDE 0x807C
#define GL_NORMAL_ARRAY_TYPE 0x807E
#define GL_NORMAL_ARRAY_STRIDE 0x807F
#define GL_COLOR_ARRAY_SIZE 0x8081
#define GL_COLOR_ARRAY_TYPE 0x8082
#define GL_COLOR_ARRAY_STRIDE 0x8083
#define GL_INDEX_ARRAY_TYPE 0x8085
#define GL_INDEX_ARRAY_STRIDE 0x8086
#define GL_TEXTURE_COORD_ARRAY_SIZE 0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE 0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE 0x808A
#define GL_EDGE_FLAG_ARRAY_STRIDE 0x808C
#define GL_VERTEX_ARRAY_POINTER 0x808E
#define GL_NORMAL_ARRAY_POINTER 0x808F
#define GL_COLOR_ARRAY_POINTER 0x8090
#define GL_INDEX_ARRAY_POINTER 0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER 0x8093
#define GL_COLOR_INDEX1_EXT 0x80E2
#define GL_COLOR_INDEX2_EXT 0x80E3
#define GL_COLOR_INDEX4_EXT 0x80E4
#define GL_COLOR_INDEX8_EXT 0x80E5
#define GL_COLOR_INDEX12_EXT 0x80E6
#define GL_COLOR_INDEX16_EXT 0x80E7
#define GL_EVAL_BIT 0x00010000
#define GL_LIST_BIT 0x00020000
#define GL_TEXTURE_BIT 0x00040000
#define GL_SCISSOR_BIT 0x00080000
#define GL_ALL_ATTRIB_BITS 0x000fffff
#define GL_CLIENT_ALL_ATTRIB_BITS 0xffffffff

GLAPI void GLAPIENTRY glAccum (GLenum op, GLfloat value);
GLAPI void GLAPIENTRY glAlphaFunc (GLenum func, GLclampf ref);
GLAPI GLboolean GLAPIENTRY glAreTexturesResident (GLsizei n, const GLuint *textures, GLboolean *residences);
GLAPI void GLAPIENTRY glArrayElement (GLint i);
GLAPI void GLAPIENTRY glBegin (GLenum mode);
GLAPI void GLAPIENTRY glBindTexture (GLenum target, GLuint texture);
GLAPI void GLAPIENTRY glBitmap (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
GLAPI void GLAPIENTRY glBlendFunc (GLenum sfactor, GLenum dfactor);
GLAPI void GLAPIENTRY glCallList (GLuint list);
GLAPI void GLAPIENTRY glCallLists (GLsizei n, GLenum type, const void *lists);
GLAPI void GLAPIENTRY glClear (GLbitfield mask);
GLAPI void GLAPIENTRY glClearAccum (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI void GLAPIENTRY glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
GLAPI void GLAPIENTRY glClearDepth (GLclampd depth);
GLAPI void GLAPIENTRY glClearIndex (GLfloat c);
GLAPI void GLAPIENTRY glClearStencil (GLint s);
GLAPI void GLAPIENTRY glClipPlane (GLenum plane, const GLdouble *equation);
GLAPI void GLAPIENTRY glColor3b (GLbyte red, GLbyte green, GLbyte blue);
GLAPI void GLAPIENTRY glColor3bv (const GLbyte *v);
GLAPI void GLAPIENTRY glColor3d (GLdouble red, GLdouble green, GLdouble blue);
GLAPI void GLAPIENTRY glColor3dv (const GLdouble *v);
GLAPI void GLAPIENTRY glColor3f (GLfloat red, GLfloat green, GLfloat blue);
GLAPI void GLAPIENTRY glColor3fv (const GLfloat *v);
GLAPI void GLAPIENTRY glColor3i (GLint red, GLint green, GLint blue);
GLAPI void GLAPIENTRY glColor3iv (const GLint *v);
GLAPI void GLAPIENTRY glColor3s (GLshort red, GLshort green, GLshort blue);
GLAPI void GLAPIENTRY glColor3sv (const GLshort *v);
GLAPI void GLAPIENTRY glColor3ub (GLubyte red, GLubyte green, GLubyte blue);
GLAPI void GLAPIENTRY glColor3ubv (const GLubyte *v);
GLAPI void GLAPIENTRY glColor3ui (GLuint red, GLuint green, GLuint blue);
GLAPI void GLAPIENTRY glColor3uiv (const GLuint *v);
GLAPI void GLAPIENTRY glColor3us (GLushort red, GLushort green, GLushort blue);
GLAPI void GLAPIENTRY glColor3usv (const GLushort *v);
GLAPI void GLAPIENTRY glColor4b (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
GLAPI void GLAPIENTRY glColor4bv (const GLbyte *v);
GLAPI void GLAPIENTRY glColor4d (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
GLAPI void GLAPIENTRY glColor4dv (const GLdouble *v);
GLAPI void GLAPIENTRY glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI void GLAPIENTRY glColor4fv (const GLfloat *v);
GLAPI void GLAPIENTRY glColor4i (GLint red, GLint green, GLint blue, GLint alpha);
GLAPI void GLAPIENTRY glColor4iv (const GLint *v);
GLAPI void GLAPIENTRY glColor4s (GLshort red, GLshort green, GLshort blue, GLshort alpha);
GLAPI void GLAPIENTRY glColor4sv (const GLshort *v);
GLAPI void GLAPIENTRY glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
GLAPI void GLAPIENTRY glColor4ubv (const GLubyte *v);
GLAPI void GLAPIENTRY glColor4ui (GLuint red, GLuint green, GLuint blue, GLuint alpha);
GLAPI void GLAPIENTRY glColor4uiv (const GLuint *v);
GLAPI void GLAPIENTRY glColor4us (GLushort red, GLushort green, GLushort blue, GLushort alpha);
GLAPI void GLAPIENTRY glColor4usv (const GLushort *v);
GLAPI void GLAPIENTRY glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
GLAPI void GLAPIENTRY glColorMaterial (GLenum face, GLenum mode);
GLAPI void GLAPIENTRY glColorPointer (GLint size, GLenum type, GLsizei stride, const void *pointer);
GLAPI void GLAPIENTRY glCopyPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
GLAPI void GLAPIENTRY glCopyTexImage1D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
GLAPI void GLAPIENTRY glCopyTexImage2D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
GLAPI void GLAPIENTRY glCopyTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
GLAPI void GLAPIENTRY glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void GLAPIENTRY glCullFace (GLenum mode);
GLAPI void GLAPIENTRY glDeleteLists (GLuint list, GLsizei range);
GLAPI void GLAPIENTRY glDeleteTextures (GLsizei n, const GLuint *textures);
GLAPI void GLAPIENTRY glDepthFunc (GLenum func);
GLAPI void GLAPIENTRY glDepthMask (GLboolean flag);
GLAPI void GLAPIENTRY glDepthRange (GLclampd zNear, GLclampd zFar);
GLAPI void GLAPIENTRY glDisable (GLenum cap);
GLAPI void GLAPIENTRY glDisableClientState (GLenum array);
GLAPI void GLAPIENTRY glDrawArrays (GLenum mode, GLint first, GLsizei count);
GLAPI void GLAPIENTRY glDrawBuffer (GLenum mode);
GLAPI void GLAPIENTRY glDrawElements (GLenum mode, GLsizei count, GLenum type, const void *indices);
GLAPI void GLAPIENTRY glDrawPixels (GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
GLAPI void GLAPIENTRY glEdgeFlag (GLboolean flag);
GLAPI void GLAPIENTRY glEdgeFlagPointer (GLsizei stride, const void *pointer);
GLAPI void GLAPIENTRY glEdgeFlagv (const GLboolean *flag);
GLAPI void GLAPIENTRY glEnable (GLenum cap);
GLAPI void GLAPIENTRY glEnableClientState (GLenum array);
GLAPI void GLAPIENTRY glEnd (void);
GLAPI void GLAPIENTRY glEndList (void);
GLAPI void GLAPIENTRY glEvalCoord1d (GLdouble u);
GLAPI void GLAPIENTRY glEvalCoord1dv (const GLdouble *u);
GLAPI void GLAPIENTRY glEvalCoord1f (GLfloat u);
GLAPI void GLAPIENTRY glEvalCoord1fv (const GLfloat *u);
GLAPI void GLAPIENTRY glEvalCoord2d (GLdouble u, GLdouble v);
GLAPI void GLAPIENTRY glEvalCoord2dv (const GLdouble *u);
GLAPI void GLAPIENTRY glEvalCoord2f (GLfloat u, GLfloat v);
GLAPI void GLAPIENTRY glEvalCoord2fv (const GLfloat *u);
GLAPI void GLAPIENTRY glEvalMesh1 (GLenum mode, GLint i1, GLint i2);
GLAPI void GLAPIENTRY glEvalMesh2 (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
GLAPI void GLAPIENTRY glEvalPoint1 (GLint i);
GLAPI void GLAPIENTRY glEvalPoint2 (GLint i, GLint j);
GLAPI void GLAPIENTRY glFeedbackBuffer (GLsizei size, GLenum type, GLfloat *buffer);
GLAPI void GLAPIENTRY glFinish (void);
GLAPI void GLAPIENTRY glFlush (void);
GLAPI void GLAPIENTRY glFogf (GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glFogfv (GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glFogi (GLenum pname, GLint param);
GLAPI void GLAPIENTRY glFogiv (GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glFrontFace (GLenum mode);
GLAPI void GLAPIENTRY glFrustum (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLAPI GLuint GLAPIENTRY glGenLists (GLsizei range);
GLAPI void GLAPIENTRY glGenTextures (GLsizei n, GLuint *textures);
GLAPI void GLAPIENTRY glGetBooleanv (GLenum pname, GLboolean *params);
GLAPI void GLAPIENTRY glGetClipPlane (GLenum plane, GLdouble *equation);
GLAPI void GLAPIENTRY glGetDoublev (GLenum pname, GLdouble *params);
GLAPI GLenum GLAPIENTRY glGetError (void);
GLAPI void GLAPIENTRY glGetFloatv (GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetIntegerv (GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glGetLightfv (GLenum light, GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetLightiv (GLenum light, GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glGetMapdv (GLenum target, GLenum query, GLdouble *v);
GLAPI void GLAPIENTRY glGetMapfv (GLenum target, GLenum query, GLfloat *v);
GLAPI void GLAPIENTRY glGetMapiv (GLenum target, GLenum query, GLint *v);
GLAPI void GLAPIENTRY glGetMaterialfv (GLenum face, GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetMaterialiv (GLenum face, GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glGetPixelMapfv (GLenum map, GLfloat *values);
GLAPI void GLAPIENTRY glGetPixelMapuiv (GLenum map, GLuint *values);
GLAPI void GLAPIENTRY glGetPixelMapusv (GLenum map, GLushort *values);
GLAPI void GLAPIENTRY glGetPointerv (GLenum pname, void* *params);
GLAPI void GLAPIENTRY glGetPolygonStipple (GLubyte *mask);
const GLAPI GLubyte * GLAPIENTRY glGetString (GLenum name);
GLAPI void GLAPIENTRY glGetTexEnvfv (GLenum target, GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetTexEnviv (GLenum target, GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glGetTexGendv (GLenum coord, GLenum pname, GLdouble *params);
GLAPI void GLAPIENTRY glGetTexGenfv (GLenum coord, GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetTexGeniv (GLenum coord, GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glGetTexImage (GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
GLAPI void GLAPIENTRY glGetTexLevelParameterfv (GLenum target, GLint level, GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetTexParameteriv (GLenum target, GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glHint (GLenum target, GLenum mode);
GLAPI void GLAPIENTRY glIndexMask (GLuint mask);
GLAPI void GLAPIENTRY glIndexPointer (GLenum type, GLsizei stride, const void *pointer);
GLAPI void GLAPIENTRY glIndexd (GLdouble c);
GLAPI void GLAPIENTRY glIndexdv (const GLdouble *c);
GLAPI void GLAPIENTRY glIndexf (GLfloat c);
GLAPI void GLAPIENTRY glIndexfv (const GLfloat *c);
GLAPI void GLAPIENTRY glIndexi (GLint c);
GLAPI void GLAPIENTRY glIndexiv (const GLint *c);
GLAPI void GLAPIENTRY glIndexs (GLshort c);
GLAPI void GLAPIENTRY glIndexsv (const GLshort *c);
GLAPI void GLAPIENTRY glIndexub (GLubyte c);
GLAPI void GLAPIENTRY glIndexubv (const GLubyte *c);
GLAPI void GLAPIENTRY glInitNames (void);
GLAPI void GLAPIENTRY glInterleavedArrays (GLenum format, GLsizei stride, const void *pointer);
GLAPI GLboolean GLAPIENTRY glIsEnabled (GLenum cap);
GLAPI GLboolean GLAPIENTRY glIsList (GLuint list);
GLAPI GLboolean GLAPIENTRY glIsTexture (GLuint texture);
GLAPI void GLAPIENTRY glLightModelf (GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glLightModelfv (GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glLightModeli (GLenum pname, GLint param);
GLAPI void GLAPIENTRY glLightModeliv (GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glLightf (GLenum light, GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glLightfv (GLenum light, GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glLighti (GLenum light, GLenum pname, GLint param);
GLAPI void GLAPIENTRY glLightiv (GLenum light, GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glLineStipple (GLint factor, GLushort pattern);
GLAPI void GLAPIENTRY glLineWidth (GLfloat width);
GLAPI void GLAPIENTRY glListBase (GLuint base);
GLAPI void GLAPIENTRY glLoadIdentity (void);
GLAPI void GLAPIENTRY glLoadMatrixd (const GLdouble *m);
GLAPI void GLAPIENTRY glLoadMatrixf (const GLfloat *m);
GLAPI void GLAPIENTRY glLoadName (GLuint name);
GLAPI void GLAPIENTRY glLogicOp (GLenum opcode);
GLAPI void GLAPIENTRY glMap1d (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
GLAPI void GLAPIENTRY glMap1f (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
GLAPI void GLAPIENTRY glMap2d (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
GLAPI void GLAPIENTRY glMap2f (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
GLAPI void GLAPIENTRY glMapGrid1d (GLint un, GLdouble u1, GLdouble u2);
GLAPI void GLAPIENTRY glMapGrid1f (GLint un, GLfloat u1, GLfloat u2);
GLAPI void GLAPIENTRY glMapGrid2d (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
GLAPI void GLAPIENTRY glMapGrid2f (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
GLAPI void GLAPIENTRY glMaterialf (GLenum face, GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glMaterialfv (GLenum face, GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glMateriali (GLenum face, GLenum pname, GLint param);
GLAPI void GLAPIENTRY glMaterialiv (GLenum face, GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glMatrixMode (GLenum mode);
GLAPI void GLAPIENTRY glMultMatrixd (const GLdouble *m);
GLAPI void GLAPIENTRY glMultMatrixf (const GLfloat *m);
GLAPI void GLAPIENTRY glNewList (GLuint list, GLenum mode);
GLAPI void GLAPIENTRY glNormal3b (GLbyte nx, GLbyte ny, GLbyte nz);
GLAPI void GLAPIENTRY glNormal3bv (const GLbyte *v);
GLAPI void GLAPIENTRY glNormal3d (GLdouble nx, GLdouble ny, GLdouble nz);
GLAPI void GLAPIENTRY glNormal3dv (const GLdouble *v);
GLAPI void GLAPIENTRY glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz);
GLAPI void GLAPIENTRY glNormal3fv (const GLfloat *v);
GLAPI void GLAPIENTRY glNormal3i (GLint nx, GLint ny, GLint nz);
GLAPI void GLAPIENTRY glNormal3iv (const GLint *v);
GLAPI void GLAPIENTRY glNormal3s (GLshort nx, GLshort ny, GLshort nz);
GLAPI void GLAPIENTRY glNormal3sv (const GLshort *v);
GLAPI void GLAPIENTRY glNormalPointer (GLenum type, GLsizei stride, const void *pointer);
GLAPI void GLAPIENTRY glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLAPI void GLAPIENTRY glPassThrough (GLfloat token);
GLAPI void GLAPIENTRY glPixelMapfv (GLenum map, GLsizei mapsize, const GLfloat *values);
GLAPI void GLAPIENTRY glPixelMapuiv (GLenum map, GLsizei mapsize, const GLuint *values);
GLAPI void GLAPIENTRY glPixelMapusv (GLenum map, GLsizei mapsize, const GLushort *values);
GLAPI void GLAPIENTRY glPixelStoref (GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glPixelStorei (GLenum pname, GLint param);
GLAPI void GLAPIENTRY glPixelTransferf (GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glPixelTransferi (GLenum pname, GLint param);
GLAPI void GLAPIENTRY glPixelZoom (GLfloat xfactor, GLfloat yfactor);
GLAPI void GLAPIENTRY glPointSize (GLfloat size);
GLAPI void GLAPIENTRY glPolygonMode (GLenum face, GLenum mode);
GLAPI void GLAPIENTRY glPolygonOffset (GLfloat factor, GLfloat units);
GLAPI void GLAPIENTRY glPolygonStipple (const GLubyte *mask);
GLAPI void GLAPIENTRY glPopAttrib (void);
GLAPI void GLAPIENTRY glPopClientAttrib (void);
GLAPI void GLAPIENTRY glPopMatrix (void);
GLAPI void GLAPIENTRY glPopName (void);
GLAPI void GLAPIENTRY glPrioritizeTextures (GLsizei n, const GLuint *textures, const GLclampf *priorities);
GLAPI void GLAPIENTRY glPushAttrib (GLbitfield mask);
GLAPI void GLAPIENTRY glPushClientAttrib (GLbitfield mask);
GLAPI void GLAPIENTRY glPushMatrix (void);
GLAPI void GLAPIENTRY glPushName (GLuint name);
GLAPI void GLAPIENTRY glRasterPos2d (GLdouble x, GLdouble y);
GLAPI void GLAPIENTRY glRasterPos2dv (const GLdouble *v);
GLAPI void GLAPIENTRY glRasterPos2f (GLfloat x, GLfloat y);
GLAPI void GLAPIENTRY glRasterPos2fv (const GLfloat *v);
GLAPI void GLAPIENTRY glRasterPos2i (GLint x, GLint y);
GLAPI void GLAPIENTRY glRasterPos2iv (const GLint *v);
GLAPI void GLAPIENTRY glRasterPos2s (GLshort x, GLshort y);
GLAPI void GLAPIENTRY glRasterPos2sv (const GLshort *v);
GLAPI void GLAPIENTRY glRasterPos3d (GLdouble x, GLdouble y, GLdouble z);
GLAPI void GLAPIENTRY glRasterPos3dv (const GLdouble *v);
GLAPI void GLAPIENTRY glRasterPos3f (GLfloat x, GLfloat y, GLfloat z);
GLAPI void GLAPIENTRY glRasterPos3fv (const GLfloat *v);
GLAPI void GLAPIENTRY glRasterPos3i (GLint x, GLint y, GLint z);
GLAPI void GLAPIENTRY glRasterPos3iv (const GLint *v);
GLAPI void GLAPIENTRY glRasterPos3s (GLshort x, GLshort y, GLshort z);
GLAPI void GLAPIENTRY glRasterPos3sv (const GLshort *v);
GLAPI void GLAPIENTRY glRasterPos4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void GLAPIENTRY glRasterPos4dv (const GLdouble *v);
GLAPI void GLAPIENTRY glRasterPos4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void GLAPIENTRY glRasterPos4fv (const GLfloat *v);
GLAPI void GLAPIENTRY glRasterPos4i (GLint x, GLint y, GLint z, GLint w);
GLAPI void GLAPIENTRY glRasterPos4iv (const GLint *v);
GLAPI void GLAPIENTRY glRasterPos4s (GLshort x, GLshort y, GLshort z, GLshort w);
GLAPI void GLAPIENTRY glRasterPos4sv (const GLshort *v);
GLAPI void GLAPIENTRY glReadBuffer (GLenum mode);
GLAPI void GLAPIENTRY glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
GLAPI void GLAPIENTRY glRectd (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
GLAPI void GLAPIENTRY glRectdv (const GLdouble *v1, const GLdouble *v2);
GLAPI void GLAPIENTRY glRectf (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
GLAPI void GLAPIENTRY glRectfv (const GLfloat *v1, const GLfloat *v2);
GLAPI void GLAPIENTRY glRecti (GLint x1, GLint y1, GLint x2, GLint y2);
GLAPI void GLAPIENTRY glRectiv (const GLint *v1, const GLint *v2);
GLAPI void GLAPIENTRY glRects (GLshort x1, GLshort y1, GLshort x2, GLshort y2);
GLAPI void GLAPIENTRY glRectsv (const GLshort *v1, const GLshort *v2);
GLAPI GLint GLAPIENTRY glRenderMode (GLenum mode);
GLAPI void GLAPIENTRY glRotated (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
GLAPI void GLAPIENTRY glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
GLAPI void GLAPIENTRY glScaled (GLdouble x, GLdouble y, GLdouble z);
GLAPI void GLAPIENTRY glScalef (GLfloat x, GLfloat y, GLfloat z);
GLAPI void GLAPIENTRY glScissor (GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void GLAPIENTRY glSelectBuffer (GLsizei size, GLuint *buffer);
GLAPI void GLAPIENTRY glShadeModel (GLenum mode);
GLAPI void GLAPIENTRY glStencilFunc (GLenum func, GLint ref, GLuint mask);
GLAPI void GLAPIENTRY glStencilMask (GLuint mask);
GLAPI void GLAPIENTRY glStencilOp (GLenum fail, GLenum zfail, GLenum zpass);
GLAPI void GLAPIENTRY glTexCoord1d (GLdouble s);
GLAPI void GLAPIENTRY glTexCoord1dv (const GLdouble *v);
GLAPI void GLAPIENTRY glTexCoord1f (GLfloat s);
GLAPI void GLAPIENTRY glTexCoord1fv (const GLfloat *v);
GLAPI void GLAPIENTRY glTexCoord1i (GLint s);
GLAPI void GLAPIENTRY glTexCoord1iv (const GLint *v);
GLAPI void GLAPIENTRY glTexCoord1s (GLshort s);
GLAPI void GLAPIENTRY glTexCoord1sv (const GLshort *v);
GLAPI void GLAPIENTRY glTexCoord2d (GLdouble s, GLdouble t);
GLAPI void GLAPIENTRY glTexCoord2dv (const GLdouble *v);
GLAPI void GLAPIENTRY glTexCoord2f (GLfloat s, GLfloat t);
GLAPI void GLAPIENTRY glTexCoord2fv (const GLfloat *v);
GLAPI void GLAPIENTRY glTexCoord2i (GLint s, GLint t);
GLAPI void GLAPIENTRY glTexCoord2iv (const GLint *v);
GLAPI void GLAPIENTRY glTexCoord2s (GLshort s, GLshort t);
GLAPI void GLAPIENTRY glTexCoord2sv (const GLshort *v);
GLAPI void GLAPIENTRY glTexCoord3d (GLdouble s, GLdouble t, GLdouble r);
GLAPI void GLAPIENTRY glTexCoord3dv (const GLdouble *v);
GLAPI void GLAPIENTRY glTexCoord3f (GLfloat s, GLfloat t, GLfloat r);
GLAPI void GLAPIENTRY glTexCoord3fv (const GLfloat *v);
GLAPI void GLAPIENTRY glTexCoord3i (GLint s, GLint t, GLint r);
GLAPI void GLAPIENTRY glTexCoord3iv (const GLint *v);
GLAPI void GLAPIENTRY glTexCoord3s (GLshort s, GLshort t, GLshort r);
GLAPI void GLAPIENTRY glTexCoord3sv (const GLshort *v);
GLAPI void GLAPIENTRY glTexCoord4d (GLdouble s, GLdouble t, GLdouble r, GLdouble q);
GLAPI void GLAPIENTRY glTexCoord4dv (const GLdouble *v);
GLAPI void GLAPIENTRY glTexCoord4f (GLfloat s, GLfloat t, GLfloat r, GLfloat q);
GLAPI void GLAPIENTRY glTexCoord4fv (const GLfloat *v);
GLAPI void GLAPIENTRY glTexCoord4i (GLint s, GLint t, GLint r, GLint q);
GLAPI void GLAPIENTRY glTexCoord4iv (const GLint *v);
GLAPI void GLAPIENTRY glTexCoord4s (GLshort s, GLshort t, GLshort r, GLshort q);
GLAPI void GLAPIENTRY glTexCoord4sv (const GLshort *v);
GLAPI void GLAPIENTRY glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const void *pointer);
GLAPI void GLAPIENTRY glTexEnvf (GLenum target, GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glTexEnvi (GLenum target, GLenum pname, GLint param);
GLAPI void GLAPIENTRY glTexEnviv (GLenum target, GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glTexGend (GLenum coord, GLenum pname, GLdouble param);
GLAPI void GLAPIENTRY glTexGendv (GLenum coord, GLenum pname, const GLdouble *params);
GLAPI void GLAPIENTRY glTexGenf (GLenum coord, GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glTexGenfv (GLenum coord, GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glTexGeni (GLenum coord, GLenum pname, GLint param);
GLAPI void GLAPIENTRY glTexGeniv (GLenum coord, GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glTexImage1D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
GLAPI void GLAPIENTRY glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
GLAPI void GLAPIENTRY glTexParameterf (GLenum target, GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glTexParameteri (GLenum target, GLenum pname, GLint param);
GLAPI void GLAPIENTRY glTexParameteriv (GLenum target, GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
GLAPI void GLAPIENTRY glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
GLAPI void GLAPIENTRY glTranslated (GLdouble x, GLdouble y, GLdouble z);
GLAPI void GLAPIENTRY glTranslatef (GLfloat x, GLfloat y, GLfloat z);
GLAPI void GLAPIENTRY glVertex2d (GLdouble x, GLdouble y);
GLAPI void GLAPIENTRY glVertex2dv (const GLdouble *v);
GLAPI void GLAPIENTRY glVertex2f (GLfloat x, GLfloat y);
GLAPI void GLAPIENTRY glVertex2fv (const GLfloat *v);
GLAPI void GLAPIENTRY glVertex2i (GLint x, GLint y);
GLAPI void GLAPIENTRY glVertex2iv (const GLint *v);
GLAPI void GLAPIENTRY glVertex2s (GLshort x, GLshort y);
GLAPI void GLAPIENTRY glVertex2sv (const GLshort *v);
GLAPI void GLAPIENTRY glVertex3d (GLdouble x, GLdouble y, GLdouble z);
GLAPI void GLAPIENTRY glVertex3dv (const GLdouble *v);
GLAPI void GLAPIENTRY glVertex3f (GLfloat x, GLfloat y, GLfloat z);
GLAPI void GLAPIENTRY glVertex3fv (const GLfloat *v);
GLAPI void GLAPIENTRY glVertex3i (GLint x, GLint y, GLint z);
GLAPI void GLAPIENTRY glVertex3iv (const GLint *v);
GLAPI void GLAPIENTRY glVertex3s (GLshort x, GLshort y, GLshort z);
GLAPI void GLAPIENTRY glVertex3sv (const GLshort *v);
GLAPI void GLAPIENTRY glVertex4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void GLAPIENTRY glVertex4dv (const GLdouble *v);
GLAPI void GLAPIENTRY glVertex4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void GLAPIENTRY glVertex4fv (const GLfloat *v);
GLAPI void GLAPIENTRY glVertex4i (GLint x, GLint y, GLint z, GLint w);
GLAPI void GLAPIENTRY glVertex4iv (const GLint *v);
GLAPI void GLAPIENTRY glVertex4s (GLshort x, GLshort y, GLshort z, GLshort w);
GLAPI void GLAPIENTRY glVertex4sv (const GLshort *v);
GLAPI void GLAPIENTRY glVertexPointer (GLint size, GLenum type, GLsizei stride, const void *pointer);
GLAPI void GLAPIENTRY glViewport (GLint x, GLint y, GLsizei width, GLsizei height);

#define GLEW_VERSION_1_1 GLEW_GET_VAR(__GLEW_VERSION_1_1)

#endif /* GL_VERSION_1_1 */

/* ---------------------------------- GLU ---------------------------------- */

#ifndef GLEW_NO_GLU
#  ifdef __APPLE__
#    include <Availability.h>
#    if defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
#      define GLEW_NO_GLU
#    endif
#  endif
#endif

#ifndef GLEW_NO_GLU
/* this is where we can safely include GLU */
#  if defined(__APPLE__) && defined(__MACH__)
#    include <OpenGL/glu.h>
#  else
#    include <GL/glu.h>
#  endif
#endif

/* ----------------------------- GL_VERSION_1_2 ---------------------------- */

#ifndef GL_VERSION_1_2
#define GL_VERSION_1_2 1

#define GL_SMOOTH_POINT_SIZE_RANGE 0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY 0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE 0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_UNSIGNED_BYTE_3_3_2 0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_INT_8_8_8_8 0x8035
#define GL_UNSIGNED_INT_10_10_10_2 0x8036
#define GL_RESCALE_NORMAL 0x803A
#define GL_TEXTURE_BINDING_3D 0x806A
#define GL_PACK_SKIP_IMAGES 0x806B
#define GL_PACK_IMAGE_HEIGHT 0x806C
#define GL_UNPACK_SKIP_IMAGES 0x806D
#define GL_UNPACK_IMAGE_HEIGHT 0x806E
#define GL_TEXTURE_3D 0x806F
#define GL_PROXY_TEXTURE_3D 0x8070
#define GL_TEXTURE_DEPTH 0x8071
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1
#define GL_MAX_ELEMENTS_VERTICES 0x80E8
#define GL_MAX_ELEMENTS_INDICES 0x80E9
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE_MIN_LOD 0x813A
#define GL_TEXTURE_MAX_LOD 0x813B
#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#define GL_SINGLE_COLOR 0x81F9
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#define GL_UNSIGNED_BYTE_2_3_3_REV 0x8362
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV 0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#define GL_ALIASED_POINT_SIZE_RANGE 0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E

typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);

#define glCopyTexSubImage3D GLEW_GET_FUN(__glewCopyTexSubImage3D)
#define glDrawRangeElements GLEW_GET_FUN(__glewDrawRangeElements)
#define glTexImage3D GLEW_GET_FUN(__glewTexImage3D)
#define glTexSubImage3D GLEW_GET_FUN(__glewTexSubImage3D)

#define GLEW_VERSION_1_2 GLEW_GET_VAR(__GLEW_VERSION_1_2)

#endif /* GL_VERSION_1_2 */

/* ---------------------------- GL_VERSION_1_2_1 --------------------------- */

#ifndef GL_VERSION_1_2_1
#define GL_VERSION_1_2_1 1

#define GLEW_VERSION_1_2_1 GLEW_GET_VAR(__GLEW_VERSION_1_2_1)

#endif /* GL_VERSION_1_2_1 */

/* ----------------------------- GL_VERSION_1_3 ---------------------------- */

#ifndef GL_VERSION_1_3
#define GL_VERSION_1_3 1

#define GL_MULTISAMPLE 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
#define GL_SAMPLE_COVERAGE 0x80A0
#define GL_SAMPLE_BUFFERS 0x80A8
#define GL_SAMPLES 0x80A9
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE 0x84E1
#define GL_MAX_TEXTURE_UNITS 0x84E2
#define GL_TRANSPOSE_MODELVIEW_MATRIX 0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX 0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX 0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX 0x84E6
#define GL_SUBTRACT 0x84E7
#define GL_COMPRESSED_ALPHA 0x84E9
#define GL_COMPRESSED_LUMINANCE 0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA 0x84EB
#define GL_COMPRESSED_INTENSITY 0x84EC
#define GL_COMPRESSED_RGB 0x84ED
#define GL_COMPRESSED_RGBA 0x84EE
#define GL_TEXTURE_COMPRESSION_HINT 0x84EF
#define GL_NORMAL_MAP 0x8511
#define GL_REFLECTION_MAP 0x8512
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#define GL_COMBINE 0x8570
#define GL_COMBINE_RGB 0x8571
#define GL_COMBINE_ALPHA 0x8572
#define GL_RGB_SCALE 0x8573
#define GL_ADD_SIGNED 0x8574
#define GL_INTERPOLATE 0x8575
#define GL_CONSTANT 0x8576
#define GL_PRIMARY_COLOR 0x8577
#define GL_PREVIOUS 0x8578
#define GL_SOURCE0_RGB 0x8580
#define GL_SOURCE1_RGB 0x8581
#define GL_SOURCE2_RGB 0x8582
#define GL_SOURCE0_ALPHA 0x8588
#define GL_SOURCE1_ALPHA 0x8589
#define GL_SOURCE2_ALPHA 0x858A
#define GL_OPERAND0_RGB 0x8590
#define GL_OPERAND1_RGB 0x8591
#define GL_OPERAND2_RGB 0x8592
#define GL_OPERAND0_ALPHA 0x8598
#define GL_OPERAND1_ALPHA 0x8599
#define GL_OPERAND2_ALPHA 0x859A
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE 0x86A0
#define GL_TEXTURE_COMPRESSED 0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#define GL_DOT3_RGB 0x86AE
#define GL_DOT3_RGBA 0x86AF
#define GL_MULTISAMPLE_BIT 0x20000000

typedef void (GLAPIENTRY * PFNGLACTIVETEXTUREPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLCLIENTACTIVETEXTUREPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE1DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE3DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXIMAGEPROC) (GLenum target, GLint lod, void *img);
typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXDPROC) (const GLdouble m[16]);
typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXFPROC) (const GLfloat m[16]);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXDPROC) (const GLdouble m[16]);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXFPROC) (const GLfloat m[16]);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DPROC) (GLenum target, GLdouble s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DVPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FPROC) (GLenum target, GLfloat s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FVPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IPROC) (GLenum target, GLint s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IVPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SPROC) (GLenum target, GLshort s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SVPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DVPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FVPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IPROC) (GLenum target, GLint s, GLint t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IVPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SPROC) (GLenum target, GLshort s, GLshort t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SVPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DVPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FVPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IPROC) (GLenum target, GLint s, GLint t, GLint r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IVPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SPROC) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SVPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DVPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FVPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IVPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SVPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLSAMPLECOVERAGEPROC) (GLclampf value, GLboolean invert);

#define glActiveTexture GLEW_GET_FUN(__glewActiveTexture)
#define glClientActiveTexture GLEW_GET_FUN(__glewClientActiveTexture)
#define glCompressedTexImage1D GLEW_GET_FUN(__glewCompressedTexImage1D)
#define glCompressedTexImage2D GLEW_GET_FUN(__glewCompressedTexImage2D)
#define glCompressedTexImage3D GLEW_GET_FUN(__glewCompressedTexImage3D)
#define glCompressedTexSubImage1D GLEW_GET_FUN(__glewCompressedTexSubImage1D)
#define glCompressedTexSubImage2D GLEW_GET_FUN(__glewCompressedTexSubImage2D)
#define glCompressedTexSubImage3D GLEW_GET_FUN(__glewCompressedTexSubImage3D)
#define glGetCompressedTexImage GLEW_GET_FUN(__glewGetCompressedTexImage)
#define glLoadTransposeMatrixd GLEW_GET_FUN(__glewLoadTransposeMatrixd)
#define glLoadTransposeMatrixf GLEW_GET_FUN(__glewLoadTransposeMatrixf)
#define glMultTransposeMatrixd GLEW_GET_FUN(__glewMultTransposeMatrixd)
#define glMultTransposeMatrixf GLEW_GET_FUN(__glewMultTransposeMatrixf)
#define glMultiTexCoord1d GLEW_GET_FUN(__glewMultiTexCoord1d)
#define glMultiTexCoord1dv GLEW_GET_FUN(__glewMultiTexCoord1dv)
#define glMultiTexCoord1f GLEW_GET_FUN(__glewMultiTexCoord1f)
#define glMultiTexCoord1fv GLEW_GET_FUN(__glewMultiTexCoord1fv)
#define glMultiTexCoord1i GLEW_GET_FUN(__glewMultiTexCoord1i)
#define glMultiTexCoord1iv GLEW_GET_FUN(__glewMultiTexCoord1iv)
#define glMultiTexCoord1s GLEW_GET_FUN(__glewMultiTexCoord1s)
#define glMultiTexCoord1sv GLEW_GET_FUN(__glewMultiTexCoord1sv)
#define glMultiTexCoord2d GLEW_GET_FUN(__glewMultiTexCoord2d)
#define glMultiTexCoord2dv GLEW_GET_FUN(__glewMultiTexCoord2dv)
#define glMultiTexCoord2f GLEW_GET_FUN(__glewMultiTexCoord2f)
#define glMultiTexCoord2fv GLEW_GET_FUN(__glewMultiTexCoord2fv)
#define glMultiTexCoord2i GLEW_GET_FUN(__glewMultiTexCoord2i)
#define glMultiTexCoord2iv GLEW_GET_FUN(__glewMultiTexCoord2iv)
#define glMultiTexCoord2s GLEW_GET_FUN(__glewMultiTexCoord2s)
#define glMultiTexCoord2sv GLEW_GET_FUN(__glewMultiTexCoord2sv)
#define glMultiTexCoord3d GLEW_GET_FUN(__glewMultiTexCoord3d)
#define glMultiTexCoord3dv GLEW_GET_FUN(__glewMultiTexCoord3dv)
#define glMultiTexCoord3f GLEW_GET_FUN(__glewMultiTexCoord3f)
#define glMultiTexCoord3fv GLEW_GET_FUN(__glewMultiTexCoord3fv)
#define glMultiTexCoord3i GLEW_GET_FUN(__glewMultiTexCoord3i)
#define glMultiTexCoord3iv GLEW_GET_FUN(__glewMultiTexCoord3iv)
#define glMultiTexCoord3s GLEW_GET_FUN(__glewMultiTexCoord3s)
#define glMultiTexCoord3sv GLEW_GET_FUN(__glewMultiTexCoord3sv)
#define glMultiTexCoord4d GLEW_GET_FUN(__glewMultiTexCoord4d)
#define glMultiTexCoord4dv GLEW_GET_FUN(__glewMultiTexCoord4dv)
#define glMultiTexCoord4f GLEW_GET_FUN(__glewMultiTexCoord4f)
#define glMultiTexCoord4fv GLEW_GET_FUN(__glewMultiTexCoord4fv)
#define glMultiTexCoord4i GLEW_GET_FUN(__glewMultiTexCoord4i)
#define glMultiTexCoord4iv GLEW_GET_FUN(__glewMultiTexCoord4iv)
#define glMultiTexCoord4s GLEW_GET_FUN(__glewMultiTexCoord4s)
#define glMultiTexCoord4sv GLEW_GET_FUN(__glewMultiTexCoord4sv)
#define glSampleCoverage GLEW_GET_FUN(__glewSampleCoverage)

#define GLEW_VERSION_1_3 GLEW_GET_VAR(__GLEW_VERSION_1_3)

#endif /* GL_VERSION_1_3 */

/* ----------------------------- GL_VERSION_1_4 ---------------------------- */

#ifndef GL_VERSION_1_4
#define GL_VERSION_1_4 1

#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_SRC_RGB 0x80C9
#define GL_BLEND_DST_ALPHA 0x80CA
#define GL_BLEND_SRC_ALPHA 0x80CB
#define GL_POINT_SIZE_MIN 0x8126
#define GL_POINT_SIZE_MAX 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE 0x8128
#define GL_POINT_DISTANCE_ATTENUATION 0x8129
#define GL_GENERATE_MIPMAP 0x8191
#define GL_GENERATE_MIPMAP_HINT 0x8192
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7
#define GL_MIRRORED_REPEAT 0x8370
#define GL_FOG_COORDINATE_SOURCE 0x8450
#define GL_FOG_COORDINATE 0x8451
#define GL_FRAGMENT_DEPTH 0x8452
#define GL_CURRENT_FOG_COORDINATE 0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE 0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE 0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER 0x8456
#define GL_FOG_COORDINATE_ARRAY 0x8457
#define GL_COLOR_SUM 0x8458
#define GL_CURRENT_SECONDARY_COLOR 0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE 0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE 0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE 0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER 0x845D
#define GL_SECONDARY_COLOR_ARRAY 0x845E
#define GL_MAX_TEXTURE_LOD_BIAS 0x84FD
#define GL_TEXTURE_FILTER_CONTROL 0x8500
#define GL_TEXTURE_LOD_BIAS 0x8501
#define GL_INCR_WRAP 0x8507
#define GL_DECR_WRAP 0x8508
#define GL_TEXTURE_DEPTH_SIZE 0x884A
#define GL_DEPTH_TEXTURE_MODE 0x884B
#define GL_TEXTURE_COMPARE_MODE 0x884C
#define GL_TEXTURE_COMPARE_FUNC 0x884D
#define GL_COMPARE_R_TO_TEXTURE 0x884E

typedef void (GLAPIENTRY * PFNGLBLENDCOLORPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (GLAPIENTRY * PFNGLFOGCOORDPOINTERPROC) (GLenum type, GLsizei stride, const void *pointer);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDPROC) (GLdouble coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDVPROC) (const GLdouble *coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFPROC) (GLfloat coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFVPROC) (const GLfloat *coord);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSPROC) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSPROC) (GLenum mode, const GLsizei *count, GLenum type, const void *const* indices, GLsizei drawcount);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFVPROC) (GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERIPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERIVPROC) (GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BPROC) (GLbyte red, GLbyte green, GLbyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BVPROC) (const GLbyte *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DPROC) (GLdouble red, GLdouble green, GLdouble blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IPROC) (GLint red, GLint green, GLint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SPROC) (GLshort red, GLshort green, GLshort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBPROC) (GLubyte red, GLubyte green, GLubyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBVPROC) (const GLubyte *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIPROC) (GLuint red, GLuint green, GLuint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIVPROC) (const GLuint *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USPROC) (GLushort red, GLushort green, GLushort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USVPROC) (const GLushort *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORPOINTERPROC) (GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DPROC) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DVPROC) (const GLdouble *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FPROC) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FVPROC) (const GLfloat *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IPROC) (GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IVPROC) (const GLint *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SPROC) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SVPROC) (const GLshort *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DVPROC) (const GLdouble *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FVPROC) (const GLfloat *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IPROC) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IVPROC) (const GLint *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SPROC) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SVPROC) (const GLshort *p);

#define glBlendColor GLEW_GET_FUN(__glewBlendColor)
#define glBlendEquation GLEW_GET_FUN(__glewBlendEquation)
#define glBlendFuncSeparate GLEW_GET_FUN(__glewBlendFuncSeparate)
#define glFogCoordPointer GLEW_GET_FUN(__glewFogCoordPointer)
#define glFogCoordd GLEW_GET_FUN(__glewFogCoordd)
#define glFogCoorddv GLEW_GET_FUN(__glewFogCoorddv)
#define glFogCoordf GLEW_GET_FUN(__glewFogCoordf)
#define glFogCoordfv GLEW_GET_FUN(__glewFogCoordfv)
#define glMultiDrawArrays GLEW_GET_FUN(__glewMultiDrawArrays)
#define glMultiDrawElements GLEW_GET_FUN(__glewMultiDrawElements)
#define glPointParameterf GLEW_GET_FUN(__glewPointParameterf)
#define glPointParameterfv GLEW_GET_FUN(__glewPointParameterfv)
#define glPointParameteri GLEW_GET_FUN(__glewPointParameteri)
#define glPointParameteriv GLEW_GET_FUN(__glewPointParameteriv)
#define glSecondaryColor3b GLEW_GET_FUN(__glewSecondaryColor3b)
#define glSecondaryColor3bv GLEW_GET_FUN(__glewSecondaryColor3bv)
#define glSecondaryColor3d GLEW_GET_FUN(__glewSecondaryColor3d)
#define glSecondaryColor3dv GLEW_GET_FUN(__glewSecondaryColor3dv)
#define glSecondaryColor3f GLEW_GET_FUN(__glewSecondaryColor3f)
#define glSecondaryColor3fv GLEW_GET_FUN(__glewSecondaryColor3fv)
#define glSecondaryColor3i GLEW_GET_FUN(__glewSecondaryColor3i)
#define glSecondaryColor3iv GLEW_GET_FUN(__glewSecondaryColor3iv)
#define glSecondaryColor3s GLEW_GET_FUN(__glewSecondaryColor3s)
#define glSecondaryColor3sv GLEW_GET_FUN(__glewSecondaryColor3sv)
#define glSecondaryColor3ub GLEW_GET_FUN(__glewSecondaryColor3ub)
#define glSecondaryColor3ubv GLEW_GET_FUN(__glewSecondaryColor3ubv)
#define glSecondaryColor3ui GLEW_GET_FUN(__glewSecondaryColor3ui)
#define glSecondaryColor3uiv GLEW_GET_FUN(__glewSecondaryColor3uiv)
#define glSecondaryColor3us GLEW_GET_FUN(__glewSecondaryColor3us)
#define glSecondaryColor3usv GLEW_GET_FUN(__glewSecondaryColor3usv)
#define glSecondaryColorPointer GLEW_GET_FUN(__glewSecondaryColorPointer)
#define glWindowPos2d GLEW_GET_FUN(__glewWindowPos2d)
#define glWindowPos2dv GLEW_GET_FUN(__glewWindowPos2dv)
#define glWindowPos2f GLEW_GET_FUN(__glewWindowPos2f)
#define glWindowPos2fv GLEW_GET_FUN(__glewWindowPos2fv)
#define glWindowPos2i GLEW_GET_FUN(__glewWindowPos2i)
#define glWindowPos2iv GLEW_GET_FUN(__glewWindowPos2iv)
#define glWindowPos2s GLEW_GET_FUN(__glewWindowPos2s)
#define glWindowPos2sv GLEW_GET_FUN(__glewWindowPos2sv)
#define glWindowPos3d GLEW_GET_FUN(__glewWindowPos3d)
#define glWindowPos3dv GLEW_GET_FUN(__glewWindowPos3dv)
#define glWindowPos3f GLEW_GET_FUN(__glewWindowPos3f)
#define glWindowPos3fv GLEW_GET_FUN(__glewWindowPos3fv)
#define glWindowPos3i GLEW_GET_FUN(__glewWindowPos3i)
#define glWindowPos3iv GLEW_GET_FUN(__glewWindowPos3iv)
#define glWindowPos3s GLEW_GET_FUN(__glewWindowPos3s)
#define glWindowPos3sv GLEW_GET_FUN(__glewWindowPos3sv)

#define GLEW_VERSION_1_4 GLEW_GET_VAR(__GLEW_VERSION_1_4)

#endif /* GL_VERSION_1_4 */

/* ----------------------------- GL_VERSION_1_5 ---------------------------- */

#ifndef GL_VERSION_1_5
#define GL_VERSION_1_5 1

#define GL_CURRENT_FOG_COORD GL_CURRENT_FOG_COORDINATE
#define GL_FOG_COORD GL_FOG_COORDINATE
#define GL_FOG_COORD_ARRAY GL_FOG_COORDINATE_ARRAY
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING
#define GL_FOG_COORD_ARRAY_POINTER GL_FOG_COORDINATE_ARRAY_POINTER
#define GL_FOG_COORD_ARRAY_STRIDE GL_FOG_COORDINATE_ARRAY_STRIDE
#define GL_FOG_COORD_ARRAY_TYPE GL_FOG_COORDINATE_ARRAY_TYPE
#define GL_FOG_COORD_SRC GL_FOG_COORDINATE_SOURCE
#define GL_SRC0_ALPHA GL_SOURCE0_ALPHA
#define GL_SRC0_RGB GL_SOURCE0_RGB
#define GL_SRC1_ALPHA GL_SOURCE1_ALPHA
#define GL_SRC1_RGB GL_SOURCE1_RGB
#define GL_SRC2_ALPHA GL_SOURCE2_ALPHA
#define GL_SRC2_RGB GL_SOURCE2_RGB
#define GL_BUFFER_SIZE 0x8764
#define GL_BUFFER_USAGE 0x8765
#define GL_QUERY_COUNTER_BITS 0x8864
#define GL_CURRENT_QUERY 0x8865
#define GL_QUERY_RESULT 0x8866
#define GL_QUERY_RESULT_AVAILABLE 0x8867
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_READ_ONLY 0x88B8
#define GL_WRITE_ONLY 0x88B9
#define GL_READ_WRITE 0x88BA
#define GL_BUFFER_ACCESS 0x88BB
#define GL_BUFFER_MAPPED 0x88BC
#define GL_BUFFER_MAP_POINTER 0x88BD
#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_COPY 0x88EA
#define GL_SAMPLES_PASSED 0x8914

#include <GL/gltypes.hpp>
//typedef ptrdiff_t GLintptr;
//typedef ptrdiff_t GLsizeiptr;

typedef void (GLAPIENTRY * PFNGLBEGINQUERYPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (GLAPIENTRY * PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
typedef void (GLAPIENTRY * PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLDELETEQUERIESPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLENDQUERYPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLGENQUERIESPROC) (GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPOINTERVPROC) (GLenum target, GLenum pname, void** params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, void* data);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTIVPROC) (GLuint id, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUIVPROC) (GLuint id, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISBUFFERPROC) (GLuint buffer);
typedef GLboolean (GLAPIENTRY * PFNGLISQUERYPROC) (GLuint id);
typedef void* (GLAPIENTRY * PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef GLboolean (GLAPIENTRY * PFNGLUNMAPBUFFERPROC) (GLenum target);

#define glBeginQuery GLEW_GET_FUN(__glewBeginQuery)
#define glBindBuffer GLEW_GET_FUN(__glewBindBuffer)
#define glBufferData GLEW_GET_FUN(__glewBufferData)
#define glBufferSubData GLEW_GET_FUN(__glewBufferSubData)
#define glDeleteBuffers GLEW_GET_FUN(__glewDeleteBuffers)
#define glDeleteQueries GLEW_GET_FUN(__glewDeleteQueries)
#define glEndQuery GLEW_GET_FUN(__glewEndQuery)
#define glGenBuffers GLEW_GET_FUN(__glewGenBuffers)
#define glGenQueries GLEW_GET_FUN(__glewGenQueries)
#define glGetBufferParameteriv GLEW_GET_FUN(__glewGetBufferParameteriv)
#define glGetBufferPointerv GLEW_GET_FUN(__glewGetBufferPointerv)
#define glGetBufferSubData GLEW_GET_FUN(__glewGetBufferSubData)
#define glGetQueryObjectiv GLEW_GET_FUN(__glewGetQueryObjectiv)
#define glGetQueryObjectuiv GLEW_GET_FUN(__glewGetQueryObjectuiv)
#define glGetQueryiv GLEW_GET_FUN(__glewGetQueryiv)
#define glIsBuffer GLEW_GET_FUN(__glewIsBuffer)
#define glIsQuery GLEW_GET_FUN(__glewIsQuery)
#define glMapBuffer GLEW_GET_FUN(__glewMapBuffer)
#define glUnmapBuffer GLEW_GET_FUN(__glewUnmapBuffer)

#define GLEW_VERSION_1_5 GLEW_GET_VAR(__GLEW_VERSION_1_5)

#endif /* GL_VERSION_1_5 */

/* ----------------------------- GL_VERSION_2_0 ---------------------------- */

#ifndef GL_VERSION_2_0
#define GL_VERSION_2_0 1

#define GL_BLEND_EQUATION_RGB GL_BLEND_EQUATION
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE 0x8625
#define GL_CURRENT_VERTEX_ATTRIB 0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE 0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE 0x8643
#define GL_VERTEX_ATTRIB_ARRAY_POINTER 0x8645
#define GL_STENCIL_BACK_FUNC 0x8800
#define GL_STENCIL_BACK_FAIL 0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
#define GL_MAX_DRAW_BUFFERS 0x8824
#define GL_DRAW_BUFFER0 0x8825
#define GL_DRAW_BUFFER1 0x8826
#define GL_DRAW_BUFFER2 0x8827
#define GL_DRAW_BUFFER3 0x8828
#define GL_DRAW_BUFFER4 0x8829
#define GL_DRAW_BUFFER5 0x882A
#define GL_DRAW_BUFFER6 0x882B
#define GL_DRAW_BUFFER7 0x882C
#define GL_DRAW_BUFFER8 0x882D
#define GL_DRAW_BUFFER9 0x882E
#define GL_DRAW_BUFFER10 0x882F
#define GL_DRAW_BUFFER11 0x8830
#define GL_DRAW_BUFFER12 0x8831
#define GL_DRAW_BUFFER13 0x8832
#define GL_DRAW_BUFFER14 0x8833
#define GL_DRAW_BUFFER15 0x8834
#define GL_BLEND_EQUATION_ALPHA 0x883D
#define GL_POINT_SPRITE 0x8861
#define GL_COORD_REPLACE 0x8862
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_MAX_TEXTURE_COORDS 0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS 0x8B4A
#define GL_MAX_VARYING_FLOATS 0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_SHADER_TYPE 0x8B4F
#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_INT_VEC2 0x8B53
#define GL_INT_VEC3 0x8B54
#define GL_INT_VEC4 0x8B55
#define GL_BOOL 0x8B56
#define GL_BOOL_VEC2 0x8B57
#define GL_BOOL_VEC3 0x8B58
#define GL_BOOL_VEC4 0x8B59
#define GL_FLOAT_MAT2 0x8B5A
#define GL_FLOAT_MAT3 0x8B5B
#define GL_FLOAT_MAT4 0x8B5C
#define GL_SAMPLER_1D 0x8B5D
#define GL_SAMPLER_2D 0x8B5E
#define GL_SAMPLER_3D 0x8B5F
#define GL_SAMPLER_CUBE 0x8B60
#define GL_SAMPLER_1D_SHADOW 0x8B61
#define GL_SAMPLER_2D_SHADOW 0x8B62
#define GL_DELETE_STATUS 0x8B80
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_VALIDATE_STATUS 0x8B83
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ATTACHED_SHADERS 0x8B85
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
#define GL_SHADER_SOURCE_LENGTH 0x8B88
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN 0x8CA0
#define GL_LOWER_LEFT 0x8CA1
#define GL_UPPER_LEFT 0x8CA2
#define GL_STENCIL_BACK_REF 0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK 0x8CA4
#define GL_STENCIL_BACK_WRITEMASK 0x8CA5

typedef void (GLAPIENTRY * PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (GLAPIENTRY * PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEPROC) (GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef GLuint (GLAPIENTRY * PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint (GLAPIENTRY * PFNGLCREATESHADERPROC) (GLenum type);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (GLAPIENTRY * PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (GLAPIENTRY * PFNGLDRAWBUFFERSPROC) (GLsizei n, const GLenum* bufs);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (GLAPIENTRY * PFNGLGETACTIVEATTRIBPROC) (GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMPROC) (GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETATTACHEDSHADERSPROC) (GLuint program, GLsizei maxCount, GLsizei* count, GLuint* shaders);
typedef GLint (GLAPIENTRY * PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (GLAPIENTRY * PFNGLGETSHADERSOURCEPROC) (GLuint obj, GLsizei maxLength, GLsizei* length, GLchar* source);
typedef void (GLAPIENTRY * PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* param);
typedef GLint (GLAPIENTRY * PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMFVPROC) (GLuint program, GLint location, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMIVPROC) (GLuint program, GLint location, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBPOINTERVPROC) (GLuint index, GLenum pname, void** pointer);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBDVPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBFVPROC) (GLuint index, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIVPROC) (GLuint index, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMPROC) (GLuint program);
typedef GLboolean (GLAPIENTRY * PFNGLISSHADERPROC) (GLuint shader);
typedef void (GLAPIENTRY * PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar *const* string, const GLint* length);
typedef void (GLAPIENTRY * PFNGLSTENCILFUNCSEPARATEPROC) (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
typedef void (GLAPIENTRY * PFNGLSTENCILMASKSEPARATEPROC) (GLenum face, GLuint mask);
typedef void (GLAPIENTRY * PFNGLSTENCILOPSEPARATEPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IPROC) (GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLVALIDATEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FPROC) (GLuint index, GLfloat x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SPROC) (GLuint index, GLshort x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SPROC) (GLuint index, GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NBVPROC) (GLuint index, const GLbyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NIVPROC) (GLuint index, const GLint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NSVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBVPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUIVPROC) (GLuint index, const GLuint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUSVPROC) (GLuint index, const GLushort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4BVPROC) (GLuint index, const GLbyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4IVPROC) (GLuint index, const GLint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBVPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UIVPROC) (GLuint index, const GLuint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4USVPROC) (GLuint index, const GLushort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

#define glAttachShader GLEW_GET_FUN(__glewAttachShader)
#define glBindAttribLocation GLEW_GET_FUN(__glewBindAttribLocation)
#define glBlendEquationSeparate GLEW_GET_FUN(__glewBlendEquationSeparate)
#define glCompileShader GLEW_GET_FUN(__glewCompileShader)
#define glCreateProgram GLEW_GET_FUN(__glewCreateProgram)
#define glCreateShader GLEW_GET_FUN(__glewCreateShader)
#define glDeleteProgram GLEW_GET_FUN(__glewDeleteProgram)
#define glDeleteShader GLEW_GET_FUN(__glewDeleteShader)
#define glDetachShader GLEW_GET_FUN(__glewDetachShader)
#define glDisableVertexAttribArray GLEW_GET_FUN(__glewDisableVertexAttribArray)
#define glDrawBuffers GLEW_GET_FUN(__glewDrawBuffers)
#define glEnableVertexAttribArray GLEW_GET_FUN(__glewEnableVertexAttribArray)
#define glGetActiveAttrib GLEW_GET_FUN(__glewGetActiveAttrib)
#define glGetActiveUniform GLEW_GET_FUN(__glewGetActiveUniform)
#define glGetAttachedShaders GLEW_GET_FUN(__glewGetAttachedShaders)
#define glGetAttribLocation GLEW_GET_FUN(__glewGetAttribLocation)
#define glGetProgramInfoLog GLEW_GET_FUN(__glewGetProgramInfoLog)
#define glGetProgramiv GLEW_GET_FUN(__glewGetProgramiv)
#define glGetShaderInfoLog GLEW_GET_FUN(__glewGetShaderInfoLog)
#define glGetShaderSource GLEW_GET_FUN(__glewGetShaderSource)
#define glGetShaderiv GLEW_GET_FUN(__glewGetShaderiv)
#define glGetUniformLocation GLEW_GET_FUN(__glewGetUniformLocation)
#define glGetUniformfv GLEW_GET_FUN(__glewGetUniformfv)
#define glGetUniformiv GLEW_GET_FUN(__glewGetUniformiv)
#define glGetVertexAttribPointerv GLEW_GET_FUN(__glewGetVertexAttribPointerv)
#define glGetVertexAttribdv GLEW_GET_FUN(__glewGetVertexAttribdv)
#define glGetVertexAttribfv GLEW_GET_FUN(__glewGetVertexAttribfv)
#define glGetVertexAttribiv GLEW_GET_FUN(__glewGetVertexAttribiv)
#define glIsProgram GLEW_GET_FUN(__glewIsProgram)
#define glIsShader GLEW_GET_FUN(__glewIsShader)
#define glLinkProgram GLEW_GET_FUN(__glewLinkProgram)
#define glShaderSource GLEW_GET_FUN(__glewShaderSource)
#define glStencilFuncSeparate GLEW_GET_FUN(__glewStencilFuncSeparate)
#define glStencilMaskSeparate GLEW_GET_FUN(__glewStencilMaskSeparate)
#define glStencilOpSeparate GLEW_GET_FUN(__glewStencilOpSeparate)
#define glUniform1f GLEW_GET_FUN(__glewUniform1f)
#define glUniform1fv GLEW_GET_FUN(__glewUniform1fv)
#define glUniform1i GLEW_GET_FUN(__glewUniform1i)
#define glUniform1iv GLEW_GET_FUN(__glewUniform1iv)
#define glUniform2f GLEW_GET_FUN(__glewUniform2f)
#define glUniform2fv GLEW_GET_FUN(__glewUniform2fv)
#define glUniform2i GLEW_GET_FUN(__glewUniform2i)
#define glUniform2iv GLEW_GET_FUN(__glewUniform2iv)
#define glUniform3f GLEW_GET_FUN(__glewUniform3f)
#define glUniform3fv GLEW_GET_FUN(__glewUniform3fv)
#define glUniform3i GLEW_GET_FUN(__glewUniform3i)
#define glUniform3iv GLEW_GET_FUN(__glewUniform3iv)
#define glUniform4f GLEW_GET_FUN(__glewUniform4f)
#define glUniform4fv GLEW_GET_FUN(__glewUniform4fv)
#define glUniform4i GLEW_GET_FUN(__glewUniform4i)
#define glUniform4iv GLEW_GET_FUN(__glewUniform4iv)
#define glUniformMatrix2fv GLEW_GET_FUN(__glewUniformMatrix2fv)
#define glUniformMatrix3fv GLEW_GET_FUN(__glewUniformMatrix3fv)
#define glUniformMatrix4fv GLEW_GET_FUN(__glewUniformMatrix4fv)
#define glUseProgram GLEW_GET_FUN(__glewUseProgram)
#define glValidateProgram GLEW_GET_FUN(__glewValidateProgram)
#define glVertexAttrib1d GLEW_GET_FUN(__glewVertexAttrib1d)
#define glVertexAttrib1dv GLEW_GET_FUN(__glewVertexAttrib1dv)
#define glVertexAttrib1f GLEW_GET_FUN(__glewVertexAttrib1f)
#define glVertexAttrib1fv GLEW_GET_FUN(__glewVertexAttrib1fv)
#define glVertexAttrib1s GLEW_GET_FUN(__glewVertexAttrib1s)
#define glVertexAttrib1sv GLEW_GET_FUN(__glewVertexAttrib1sv)
#define glVertexAttrib2d GLEW_GET_FUN(__glewVertexAttrib2d)
#define glVertexAttrib2dv GLEW_GET_FUN(__glewVertexAttrib2dv)
#define glVertexAttrib2f GLEW_GET_FUN(__glewVertexAttrib2f)
#define glVertexAttrib2fv GLEW_GET_FUN(__glewVertexAttrib2fv)
#define glVertexAttrib2s GLEW_GET_FUN(__glewVertexAttrib2s)
#define glVertexAttrib2sv GLEW_GET_FUN(__glewVertexAttrib2sv)
#define glVertexAttrib3d GLEW_GET_FUN(__glewVertexAttrib3d)
#define glVertexAttrib3dv GLEW_GET_FUN(__glewVertexAttrib3dv)
#define glVertexAttrib3f GLEW_GET_FUN(__glewVertexAttrib3f)
#define glVertexAttrib3fv GLEW_GET_FUN(__glewVertexAttrib3fv)
#define glVertexAttrib3s GLEW_GET_FUN(__glewVertexAttrib3s)
#define glVertexAttrib3sv GLEW_GET_FUN(__glewVertexAttrib3sv)
#define glVertexAttrib4Nbv GLEW_GET_FUN(__glewVertexAttrib4Nbv)
#define glVertexAttrib4Niv GLEW_GET_FUN(__glewVertexAttrib4Niv)
#define glVertexAttrib4Nsv GLEW_GET_FUN(__glewVertexAttrib4Nsv)
#define glVertexAttrib4Nub GLEW_GET_FUN(__glewVertexAttrib4Nub)
#define glVertexAttrib4Nubv GLEW_GET_FUN(__glewVertexAttrib4Nubv)
#define glVertexAttrib4Nuiv GLEW_GET_FUN(__glewVertexAttrib4Nuiv)
#define glVertexAttrib4Nusv GLEW_GET_FUN(__glewVertexAttrib4Nusv)
#define glVertexAttrib4bv GLEW_GET_FUN(__glewVertexAttrib4bv)
#define glVertexAttrib4d GLEW_GET_FUN(__glewVertexAttrib4d)
#define glVertexAttrib4dv GLEW_GET_FUN(__glewVertexAttrib4dv)
#define glVertexAttrib4f GLEW_GET_FUN(__glewVertexAttrib4f)
#define glVertexAttrib4fv GLEW_GET_FUN(__glewVertexAttrib4fv)
#define glVertexAttrib4iv GLEW_GET_FUN(__glewVertexAttrib4iv)
#define glVertexAttrib4s GLEW_GET_FUN(__glewVertexAttrib4s)
#define glVertexAttrib4sv GLEW_GET_FUN(__glewVertexAttrib4sv)
#define glVertexAttrib4ubv GLEW_GET_FUN(__glewVertexAttrib4ubv)
#define glVertexAttrib4uiv GLEW_GET_FUN(__glewVertexAttrib4uiv)
#define glVertexAttrib4usv GLEW_GET_FUN(__glewVertexAttrib4usv)
#define glVertexAttribPointer GLEW_GET_FUN(__glewVertexAttribPointer)

#define GLEW_VERSION_2_0 GLEW_GET_VAR(__GLEW_VERSION_2_0)

#endif /* GL_VERSION_2_0 */

/* ----------------------------- GL_VERSION_2_1 ---------------------------- */

#ifndef GL_VERSION_2_1
#define GL_VERSION_2_1 1

#define GL_CURRENT_RASTER_SECONDARY_COLOR 0x845F
#define GL_PIXEL_PACK_BUFFER 0x88EB
#define GL_PIXEL_UNPACK_BUFFER 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING 0x88EF
#define GL_FLOAT_MAT2x3 0x8B65
#define GL_FLOAT_MAT2x4 0x8B66
#define GL_FLOAT_MAT3x2 0x8B67
#define GL_FLOAT_MAT3x4 0x8B68
#define GL_FLOAT_MAT4x2 0x8B69
#define GL_FLOAT_MAT4x3 0x8B6A
#define GL_SRGB 0x8C40
#define GL_SRGB8 0x8C41
#define GL_SRGB_ALPHA 0x8C42
#define GL_SRGB8_ALPHA8 0x8C43
#define GL_SLUMINANCE_ALPHA 0x8C44
#define GL_SLUMINANCE8_ALPHA8 0x8C45
#define GL_SLUMINANCE 0x8C46
#define GL_SLUMINANCE8 0x8C47
#define GL_COMPRESSED_SRGB 0x8C48
#define GL_COMPRESSED_SRGB_ALPHA 0x8C49
#define GL_COMPRESSED_SLUMINANCE 0x8C4A
#define GL_COMPRESSED_SLUMINANCE_ALPHA 0x8C4B

typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

#define glUniformMatrix2x3fv GLEW_GET_FUN(__glewUniformMatrix2x3fv)
#define glUniformMatrix2x4fv GLEW_GET_FUN(__glewUniformMatrix2x4fv)
#define glUniformMatrix3x2fv GLEW_GET_FUN(__glewUniformMatrix3x2fv)
#define glUniformMatrix3x4fv GLEW_GET_FUN(__glewUniformMatrix3x4fv)
#define glUniformMatrix4x2fv GLEW_GET_FUN(__glewUniformMatrix4x2fv)
#define glUniformMatrix4x3fv GLEW_GET_FUN(__glewUniformMatrix4x3fv)

#define GLEW_VERSION_2_1 GLEW_GET_VAR(__GLEW_VERSION_2_1)

#endif /* GL_VERSION_2_1 */

/* ----------------------------- GL_VERSION_3_0 ---------------------------- */

#ifndef GL_VERSION_3_0
#define GL_VERSION_3_0 1

#define GL_CLIP_DISTANCE0 GL_CLIP_PLANE0
#define GL_CLIP_DISTANCE1 GL_CLIP_PLANE1
#define GL_CLIP_DISTANCE2 GL_CLIP_PLANE2
#define GL_CLIP_DISTANCE3 GL_CLIP_PLANE3
#define GL_CLIP_DISTANCE4 GL_CLIP_PLANE4
#define GL_CLIP_DISTANCE5 GL_CLIP_PLANE5
#define GL_COMPARE_REF_TO_TEXTURE GL_COMPARE_R_TO_TEXTURE_ARB
#define GL_MAX_CLIP_DISTANCES GL_MAX_CLIP_PLANES
#define GL_MAX_VARYING_COMPONENTS GL_MAX_VARYING_FLOATS
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x0001
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
#define GL_NUM_EXTENSIONS 0x821D
#define GL_CONTEXT_FLAGS 0x821E
#define GL_DEPTH_BUFFER 0x8223
#define GL_STENCIL_BUFFER 0x8224
#define GL_RGBA32F 0x8814
#define GL_RGB32F 0x8815
#define GL_RGBA16F 0x881A
#define GL_RGB16F 0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER 0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET 0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET 0x8905
#define GL_CLAMP_VERTEX_COLOR 0x891A
#define GL_CLAMP_FRAGMENT_COLOR 0x891B
#define GL_CLAMP_READ_COLOR 0x891C
#define GL_FIXED_ONLY 0x891D
#define GL_TEXTURE_RED_TYPE 0x8C10
#define GL_TEXTURE_GREEN_TYPE 0x8C11
#define GL_TEXTURE_BLUE_TYPE 0x8C12
#define GL_TEXTURE_ALPHA_TYPE 0x8C13
#define GL_TEXTURE_LUMINANCE_TYPE 0x8C14
#define GL_TEXTURE_INTENSITY_TYPE 0x8C15
#define GL_TEXTURE_DEPTH_TYPE 0x8C16
#define GL_TEXTURE_1D_ARRAY 0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY 0x8C19
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY 0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY 0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
#define GL_R11F_G11F_B10F 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
#define GL_RGB9_E5 0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
#define GL_TEXTURE_SHARED_SIZE 0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_PRIMITIVES_GENERATED 0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS 0x8C8C
#define GL_SEPARATE_ATTRIBS 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI 0x8D70
#define GL_RGB32UI 0x8D71
#define GL_RGBA16UI 0x8D76
#define GL_RGB16UI 0x8D77
#define GL_RGBA8UI 0x8D7C
#define GL_RGB8UI 0x8D7D
#define GL_RGBA32I 0x8D82
#define GL_RGB32I 0x8D83
#define GL_RGBA16I 0x8D88
#define GL_RGB16I 0x8D89
#define GL_RGBA8I 0x8D8E
#define GL_RGB8I 0x8D8F
#define GL_RED_INTEGER 0x8D94
#define GL_GREEN_INTEGER 0x8D95
#define GL_BLUE_INTEGER 0x8D96
#define GL_ALPHA_INTEGER 0x8D97
#define GL_RGB_INTEGER 0x8D98
#define GL_RGBA_INTEGER 0x8D99
#define GL_BGR_INTEGER 0x8D9A
#define GL_BGRA_INTEGER 0x8D9B
#define GL_SAMPLER_1D_ARRAY 0x8DC0
#define GL_SAMPLER_2D_ARRAY 0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW 0x8DC5
#define GL_UNSIGNED_INT_VEC2 0x8DC6
#define GL_UNSIGNED_INT_VEC3 0x8DC7
#define GL_UNSIGNED_INT_VEC4 0x8DC8
#define GL_INT_SAMPLER_1D 0x8DC9
#define GL_INT_SAMPLER_2D 0x8DCA
#define GL_INT_SAMPLER_3D 0x8DCB
#define GL_INT_SAMPLER_CUBE 0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
#define GL_QUERY_WAIT 0x8E13
#define GL_QUERY_NO_WAIT 0x8E14
#define GL_QUERY_BY_REGION_WAIT 0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT 0x8E16

typedef void (GLAPIENTRY * PFNGLBEGINCONDITIONALRENDERPROC) (GLuint id, GLenum mode);
typedef void (GLAPIENTRY * PFNGLBEGINTRANSFORMFEEDBACKPROC) (GLenum primitiveMode);
typedef void (GLAPIENTRY * PFNGLBINDFRAGDATALOCATIONPROC) (GLuint program, GLuint colorNumber, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLCLAMPCOLORPROC) (GLenum target, GLenum clamp);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERFIPROC) (GLenum buffer, GLint drawBuffer, GLfloat depth, GLint stencil);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERFVPROC) (GLenum buffer, GLint drawBuffer, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERIVPROC) (GLenum buffer, GLint drawBuffer, const GLint* value);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERUIVPROC) (GLenum buffer, GLint drawBuffer, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLCOLORMASKIPROC) (GLuint buf, GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (GLAPIENTRY * PFNGLDISABLEIPROC) (GLenum cap, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEIPROC) (GLenum cap, GLuint index);
typedef void (GLAPIENTRY * PFNGLENDCONDITIONALRENDERPROC) (void);
typedef void (GLAPIENTRY * PFNGLENDTRANSFORMFEEDBACKPROC) (void);
typedef void (GLAPIENTRY * PFNGLGETBOOLEANI_VPROC) (GLenum pname, GLuint index, GLboolean* data);
typedef GLint (GLAPIENTRY * PFNGLGETFRAGDATALOCATIONPROC) (GLuint program, const GLchar* name);
typedef const GLubyte* (GLAPIENTRY * PFNGLGETSTRINGIPROC) (GLenum name, GLuint index);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIUIVPROC) (GLenum target, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKVARYINGPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMUIVPROC) (GLuint program, GLint location, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIIVPROC) (GLuint index, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIUIVPROC) (GLuint index, GLenum pname, GLuint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISENABLEDIPROC) (GLenum cap, GLuint index);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIIVPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIUIVPROC) (GLenum target, GLenum pname, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKVARYINGSPROC) (GLuint program, GLsizei count, const GLchar *const* varyings, GLenum bufferMode);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIPROC) (GLint location, GLuint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIVPROC) (GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIPROC) (GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIVPROC) (GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIVPROC) (GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIVPROC) (GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IPROC) (GLuint index, GLint v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IVPROC) (GLuint index, const GLint* v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIPROC) (GLuint index, GLuint v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIVPROC) (GLuint index, const GLuint* v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IPROC) (GLuint index, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IVPROC) (GLuint index, const GLint* v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIPROC) (GLuint index, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIVPROC) (GLuint index, const GLuint* v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IPROC) (GLuint index, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IVPROC) (GLuint index, const GLint* v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIPROC) (GLuint index, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIVPROC) (GLuint index, const GLuint* v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4BVPROC) (GLuint index, const GLbyte* v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IPROC) (GLuint index, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IVPROC) (GLuint index, const GLint* v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4SVPROC) (GLuint index, const GLshort* v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UBVPROC) (GLuint index, const GLubyte* v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIPROC) (GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIVPROC) (GLuint index, const GLuint* v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4USVPROC) (GLuint index, const GLushort* v0);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBIPOINTERPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void*pointer);

#define glBeginConditionalRender GLEW_GET_FUN(__glewBeginConditionalRender)
#define glBeginTransformFeedback GLEW_GET_FUN(__glewBeginTransformFeedback)
#define glBindFragDataLocation GLEW_GET_FUN(__glewBindFragDataLocation)
#define glClampColor GLEW_GET_FUN(__glewClampColor)
#define glClearBufferfi GLEW_GET_FUN(__glewClearBufferfi)
#define glClearBufferfv GLEW_GET_FUN(__glewClearBufferfv)
#define glClearBufferiv GLEW_GET_FUN(__glewClearBufferiv)
#define glClearBufferuiv GLEW_GET_FUN(__glewClearBufferuiv)
#define glColorMaski GLEW_GET_FUN(__glewColorMaski)
#define glDisablei GLEW_GET_FUN(__glewDisablei)
#define glEnablei GLEW_GET_FUN(__glewEnablei)
#define glEndConditionalRender GLEW_GET_FUN(__glewEndConditionalRender)
#define glEndTransformFeedback GLEW_GET_FUN(__glewEndTransformFeedback)
#define glGetBooleani_v GLEW_GET_FUN(__glewGetBooleani_v)
#define glGetFragDataLocation GLEW_GET_FUN(__glewGetFragDataLocation)
#define glGetStringi GLEW_GET_FUN(__glewGetStringi)
#define glGetTexParameterIiv GLEW_GET_FUN(__glewGetTexParameterIiv)
#define glGetTexParameterIuiv GLEW_GET_FUN(__glewGetTexParameterIuiv)
#define glGetTransformFeedbackVarying GLEW_GET_FUN(__glewGetTransformFeedbackVarying)
#define glGetUniformuiv GLEW_GET_FUN(__glewGetUniformuiv)
#define glGetVertexAttribIiv GLEW_GET_FUN(__glewGetVertexAttribIiv)
#define glGetVertexAttribIuiv GLEW_GET_FUN(__glewGetVertexAttribIuiv)
#define glIsEnabledi GLEW_GET_FUN(__glewIsEnabledi)
#define glTexParameterIiv GLEW_GET_FUN(__glewTexParameterIiv)
#define glTexParameterIuiv GLEW_GET_FUN(__glewTexParameterIuiv)
#define glTransformFeedbackVaryings GLEW_GET_FUN(__glewTransformFeedbackVaryings)
#define glUniform1ui GLEW_GET_FUN(__glewUniform1ui)
#define glUniform1uiv GLEW_GET_FUN(__glewUniform1uiv)
#define glUniform2ui GLEW_GET_FUN(__glewUniform2ui)
#define glUniform2uiv GLEW_GET_FUN(__glewUniform2uiv)
#define glUniform3ui GLEW_GET_FUN(__glewUniform3ui)
#define glUniform3uiv GLEW_GET_FUN(__glewUniform3uiv)
#define glUniform4ui GLEW_GET_FUN(__glewUniform4ui)
#define glUniform4uiv GLEW_GET_FUN(__glewUniform4uiv)
#define glVertexAttribI1i GLEW_GET_FUN(__glewVertexAttribI1i)
#define glVertexAttribI1iv GLEW_GET_FUN(__glewVertexAttribI1iv)
#define glVertexAttribI1ui GLEW_GET_FUN(__glewVertexAttribI1ui)
#define glVertexAttribI1uiv GLEW_GET_FUN(__glewVertexAttribI1uiv)
#define glVertexAttribI2i GLEW_GET_FUN(__glewVertexAttribI2i)
#define glVertexAttribI2iv GLEW_GET_FUN(__glewVertexAttribI2iv)
#define glVertexAttribI2ui GLEW_GET_FUN(__glewVertexAttribI2ui)
#define glVertexAttribI2uiv GLEW_GET_FUN(__glewVertexAttribI2uiv)
#define glVertexAttribI3i GLEW_GET_FUN(__glewVertexAttribI3i)
#define glVertexAttribI3iv GLEW_GET_FUN(__glewVertexAttribI3iv)
#define glVertexAttribI3ui GLEW_GET_FUN(__glewVertexAttribI3ui)
#define glVertexAttribI3uiv GLEW_GET_FUN(__glewVertexAttribI3uiv)
#define glVertexAttribI4bv GLEW_GET_FUN(__glewVertexAttribI4bv)
#define glVertexAttribI4i GLEW_GET_FUN(__glewVertexAttribI4i)
#define glVertexAttribI4iv GLEW_GET_FUN(__glewVertexAttribI4iv)
#define glVertexAttribI4sv GLEW_GET_FUN(__glewVertexAttribI4sv)
#define glVertexAttribI4ubv GLEW_GET_FUN(__glewVertexAttribI4ubv)
#define glVertexAttribI4ui GLEW_GET_FUN(__glewVertexAttribI4ui)
#define glVertexAttribI4uiv GLEW_GET_FUN(__glewVertexAttribI4uiv)
#define glVertexAttribI4usv GLEW_GET_FUN(__glewVertexAttribI4usv)
#define glVertexAttribIPointer GLEW_GET_FUN(__glewVertexAttribIPointer)

#define GLEW_VERSION_3_0 GLEW_GET_VAR(__GLEW_VERSION_3_0)

#endif /* GL_VERSION_3_0 */

/* ----------------------------- GL_VERSION_3_1 ---------------------------- */

#ifndef GL_VERSION_3_1
#define GL_VERSION_3_1 1

#define GL_TEXTURE_RECTANGLE 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE 0x84F8
#define GL_SAMPLER_2D_RECT 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW 0x8B64
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 0x8C2D
#define GL_TEXTURE_BUFFER_FORMAT 0x8C2E
#define GL_SAMPLER_BUFFER 0x8DC2
#define GL_INT_SAMPLER_2D_RECT 0x8DCD
#define GL_INT_SAMPLER_BUFFER 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT 0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_BUFFER 0x8DD8
#define GL_RED_SNORM 0x8F90
#define GL_RG_SNORM 0x8F91
#define GL_RGB_SNORM 0x8F92
#define GL_RGBA_SNORM 0x8F93
#define GL_R8_SNORM 0x8F94
#define GL_RG8_SNORM 0x8F95
#define GL_RGB8_SNORM 0x8F96
#define GL_RGBA8_SNORM 0x8F97
#define GL_R16_SNORM 0x8F98
#define GL_RG16_SNORM 0x8F99
#define GL_RGB16_SNORM 0x8F9A
#define GL_RGBA16_SNORM 0x8F9B
#define GL_SIGNED_NORMALIZED 0x8F9C
#define GL_PRIMITIVE_RESTART 0x8F9D
#define GL_PRIMITIVE_RESTART_INDEX 0x8F9E
#define GL_BUFFER_ACCESS_FLAGS 0x911F
#define GL_BUFFER_MAP_LENGTH 0x9120
#define GL_BUFFER_MAP_OFFSET 0x9121

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDPROC) (GLenum mode, GLint first, GLsizei count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLPRIMITIVERESTARTINDEXPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLTEXBUFFERPROC) (GLenum target, GLenum internalFormat, GLuint buffer);

#define glDrawArraysInstanced GLEW_GET_FUN(__glewDrawArraysInstanced)
#define glDrawElementsInstanced GLEW_GET_FUN(__glewDrawElementsInstanced)
#define glPrimitiveRestartIndex GLEW_GET_FUN(__glewPrimitiveRestartIndex)
#define glTexBuffer GLEW_GET_FUN(__glewTexBuffer)

#define GLEW_VERSION_3_1 GLEW_GET_VAR(__GLEW_VERSION_3_1)

#endif /* GL_VERSION_3_1 */

/* ----------------------------- GL_VERSION_3_2 ---------------------------- */

#ifndef GL_VERSION_3_2
#define GL_VERSION_3_2 1

#define GL_CONTEXT_CORE_PROFILE_BIT 0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
#define GL_LINES_ADJACENCY 0x000A
#define GL_LINE_STRIP_ADJACENCY 0x000B
#define GL_TRIANGLES_ADJACENCY 0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY 0x000D
#define GL_PROGRAM_POINT_SIZE 0x8642
#define GL_GEOMETRY_VERTICES_OUT 0x8916
#define GL_GEOMETRY_INPUT_TYPE 0x8917
#define GL_GEOMETRY_OUTPUT_TYPE 0x8918
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS 0x9122
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS 0x9125
#define GL_CONTEXT_PROFILE_MASK 0x9126

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPARAMETERI64VPROC) (GLenum target, GLenum value, GLint64 * data);
typedef void (GLAPIENTRY * PFNGLGETINTEGER64I_VPROC) (GLenum pname, GLuint index, GLint64 * data);

#define glFramebufferTexture GLEW_GET_FUN(__glewFramebufferTexture)
#define glGetBufferParameteri64v GLEW_GET_FUN(__glewGetBufferParameteri64v)
#define glGetInteger64i_v GLEW_GET_FUN(__glewGetInteger64i_v)

#define GLEW_VERSION_3_2 GLEW_GET_VAR(__GLEW_VERSION_3_2)

#endif /* GL_VERSION_3_2 */

/* ----------------------------- GL_VERSION_3_3 ---------------------------- */

#ifndef GL_VERSION_3_3
#define GL_VERSION_3_3 1

#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR 0x88FE
#define GL_RGB10_A2UI 0x906F

typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBDIVISORPROC) (GLuint index, GLuint divisor);

#define glVertexAttribDivisor GLEW_GET_FUN(__glewVertexAttribDivisor)

#define GLEW_VERSION_3_3 GLEW_GET_VAR(__GLEW_VERSION_3_3)

#endif /* GL_VERSION_3_3 */

/* ----------------------------- GL_VERSION_4_0 ---------------------------- */

#ifndef GL_VERSION_4_0
#define GL_VERSION_4_0 1

#define GL_SAMPLE_SHADING 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE 0x8C37
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS 0x8F9F
#define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY 0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY 0x900F

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEIPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONIPROC) (GLuint buf, GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEIPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCIPROC) (GLuint buf, GLenum src, GLenum dst);
typedef void (GLAPIENTRY * PFNGLMINSAMPLESHADINGPROC) (GLclampf value);

#define glBlendEquationSeparatei GLEW_GET_FUN(__glewBlendEquationSeparatei)
#define glBlendEquationi GLEW_GET_FUN(__glewBlendEquationi)
#define glBlendFuncSeparatei GLEW_GET_FUN(__glewBlendFuncSeparatei)
#define glBlendFunci GLEW_GET_FUN(__glewBlendFunci)
#define glMinSampleShading GLEW_GET_FUN(__glewMinSampleShading)

#define GLEW_VERSION_4_0 GLEW_GET_VAR(__GLEW_VERSION_4_0)

#endif /* GL_VERSION_4_0 */

/* ----------------------------- GL_VERSION_4_1 ---------------------------- */

#ifndef GL_VERSION_4_1
#define GL_VERSION_4_1 1

#define GLEW_VERSION_4_1 GLEW_GET_VAR(__GLEW_VERSION_4_1)

#endif /* GL_VERSION_4_1 */

/* ----------------------------- GL_VERSION_4_2 ---------------------------- */

#ifndef GL_VERSION_4_2
#define GL_VERSION_4_2 1

#define GL_TRANSFORM_FEEDBACK_PAUSED 0x8E23
#define GL_TRANSFORM_FEEDBACK_ACTIVE 0x8E24
#define GL_COMPRESSED_RGBA_BPTC_UNORM 0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM 0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT 0x8E8F
#define GL_COPY_READ_BUFFER_BINDING 0x8F36
#define GL_COPY_WRITE_BUFFER_BINDING 0x8F37

#define GLEW_VERSION_4_2 GLEW_GET_VAR(__GLEW_VERSION_4_2)

#endif /* GL_VERSION_4_2 */

/* ----------------------------- GL_VERSION_4_3 ---------------------------- */

#ifndef GL_VERSION_4_3
#define GL_VERSION_4_3 1

#define GL_NUM_SHADING_LANGUAGE_VERSIONS 0x82E9
#define GL_VERTEX_ATTRIB_ARRAY_LONG 0x874E

#define GLEW_VERSION_4_3 GLEW_GET_VAR(__GLEW_VERSION_4_3)

#endif /* GL_VERSION_4_3 */

/* ----------------------------- GL_VERSION_4_4 ---------------------------- */

#ifndef GL_VERSION_4_4
#define GL_VERSION_4_4 1

#define GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED 0x8221
#define GL_MAX_VERTEX_ATTRIB_STRIDE 0x82E5
#define GL_TEXTURE_BUFFER_BINDING 0x8C2A

#define GLEW_VERSION_4_4 GLEW_GET_VAR(__GLEW_VERSION_4_4)

#endif /* GL_VERSION_4_4 */

/* ----------------------------- GL_VERSION_4_5 ---------------------------- */

#ifndef GL_VERSION_4_5
#define GL_VERSION_4_5 1

#define GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT 0x00000004

typedef GLenum (GLAPIENTRY * PFNGLGETGRAPHICSRESETSTATUSPROC) (void);
typedef void (GLAPIENTRY * PFNGLGETNCOMPRESSEDTEXIMAGEPROC) (GLenum target, GLint lod, GLsizei bufSize, GLvoid *pixels);
typedef void (GLAPIENTRY * PFNGLGETNTEXIMAGEPROC) (GLenum tex, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid *pixels);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMDVPROC) (GLuint program, GLint location, GLsizei bufSize, GLdouble *params);

#define glGetGraphicsResetStatus GLEW_GET_FUN(__glewGetGraphicsResetStatus)
#define glGetnCompressedTexImage GLEW_GET_FUN(__glewGetnCompressedTexImage)
#define glGetnTexImage GLEW_GET_FUN(__glewGetnTexImage)
#define glGetnUniformdv GLEW_GET_FUN(__glewGetnUniformdv)

#define GLEW_VERSION_4_5 GLEW_GET_VAR(__GLEW_VERSION_4_5)

#endif /* GL_VERSION_4_5 */

/* ----------------------------- GL_VERSION_4_6 ---------------------------- */

#ifndef GL_VERSION_4_6
#define GL_VERSION_4_6 1

#define GL_CONTEXT_FLAG_NO_ERROR_BIT 0x00000008
#define GL_PARAMETER_BUFFER 0x80EE
#define GL_PARAMETER_BUFFER_BINDING 0x80EF
#define GL_TRANSFORM_FEEDBACK_OVERFLOW 0x82EC
#define GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW 0x82ED
#define GL_VERTICES_SUBMITTED 0x82EE
#define GL_PRIMITIVES_SUBMITTED 0x82EF
#define GL_VERTEX_SHADER_INVOCATIONS 0x82F0
#define GL_TESS_CONTROL_SHADER_PATCHES 0x82F1
#define GL_TESS_EVALUATION_SHADER_INVOCATIONS 0x82F2
#define GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED 0x82F3
#define GL_FRAGMENT_SHADER_INVOCATIONS 0x82F4
#define GL_COMPUTE_SHADER_INVOCATIONS 0x82F5
#define GL_CLIPPING_INPUT_PRIMITIVES 0x82F6
#define GL_CLIPPING_OUTPUT_PRIMITIVES 0x82F7
#define GL_TEXTURE_MAX_ANISOTROPY 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY 0x84FF
#define GL_POLYGON_OFFSET_CLAMP 0x8E1B
#define GL_SHADER_BINARY_FORMAT_SPIR_V 0x9551
#define GL_SPIR_V_BINARY 0x9552
#define GL_SPIR_V_EXTENSIONS 0x9553
#define GL_NUM_SPIR_V_EXTENSIONS 0x9554

typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC) (GLenum mode, const GLvoid *indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC) (GLenum mode, GLenum type, const GLvoid *indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLSPECIALIZESHADERPROC) (GLuint shader, const GLchar *pEntryPoint, GLuint numSpecializationConstants, const GLuint *pConstantIndex, const GLuint *pConstantValue);

#define glMultiDrawArraysIndirectCount GLEW_GET_FUN(__glewMultiDrawArraysIndirectCount)
#define glMultiDrawElementsIndirectCount GLEW_GET_FUN(__glewMultiDrawElementsIndirectCount)
#define glSpecializeShader GLEW_GET_FUN(__glewSpecializeShader)

#define GLEW_VERSION_4_6 GLEW_GET_VAR(__GLEW_VERSION_4_6)

#endif /* GL_VERSION_4_6 */

/* ------------------- GL_AMD_stencil_operation_extended ------------------- */

#ifndef GL_AMD_stencil_operation_extended
#define GL_AMD_stencil_operation_extended 1

#define GL_SET_AMD 0x874A
#define GL_REPLACE_VALUE_AMD 0x874B
#define GL_STENCIL_OP_VALUE_AMD 0x874C
#define GL_STENCIL_BACK_OP_VALUE_AMD 0x874D

typedef void (GLAPIENTRY * PFNGLSTENCILOPVALUEAMDPROC) (GLenum face, GLuint value);

#define glStencilOpValueAMD GLEW_GET_FUN(__glewStencilOpValueAMD)

#define GLEW_AMD_stencil_operation_extended GLEW_GET_VAR(__GLEW_AMD_stencil_operation_extended)

#endif /* GL_AMD_stencil_operation_extended */

/* -------------------- GL_ANDROID_extension_pack_es31a -------------------- */

#ifndef GL_ANDROID_extension_pack_es31a
#define GL_ANDROID_extension_pack_es31a 1

#define GLEW_ANDROID_extension_pack_es31a GLEW_GET_VAR(__GLEW_ANDROID_extension_pack_es31a)

#endif /* GL_ANDROID_extension_pack_es31a */

/* ------------------------ GL_ARB_ES2_compatibility ----------------------- */

#ifndef GL_ARB_ES2_compatibility
#define GL_ARB_ES2_compatibility 1

#define GL_FIXED 0x140C
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_RGB565 0x8D62
#define GL_LOW_FLOAT 0x8DF0
#define GL_MEDIUM_FLOAT 0x8DF1
#define GL_HIGH_FLOAT 0x8DF2
#define GL_LOW_INT 0x8DF3
#define GL_MEDIUM_INT 0x8DF4
#define GL_HIGH_INT 0x8DF5
#define GL_SHADER_BINARY_FORMATS 0x8DF8
#define GL_NUM_SHADER_BINARY_FORMATS 0x8DF9
#define GL_SHADER_COMPILER 0x8DFA
#define GL_MAX_VERTEX_UNIFORM_VECTORS 0x8DFB
#define GL_MAX_VARYING_VECTORS 0x8DFC
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS 0x8DFD

typedef int GLfixed;

typedef void (GLAPIENTRY * PFNGLCLEARDEPTHFPROC) (GLclampf d);
typedef void (GLAPIENTRY * PFNGLDEPTHRANGEFPROC) (GLclampf n, GLclampf f);
typedef void (GLAPIENTRY * PFNGLGETSHADERPRECISIONFORMATPROC) (GLenum shadertype, GLenum precisiontype, GLint* range, GLint *precision);
typedef void (GLAPIENTRY * PFNGLRELEASESHADERCOMPILERPROC) (void);
typedef void (GLAPIENTRY * PFNGLSHADERBINARYPROC) (GLsizei count, const GLuint* shaders, GLenum binaryformat, const void*binary, GLsizei length);

#define glClearDepthf GLEW_GET_FUN(__glewClearDepthf)
#define glDepthRangef GLEW_GET_FUN(__glewDepthRangef)
#define glGetShaderPrecisionFormat GLEW_GET_FUN(__glewGetShaderPrecisionFormat)
#define glReleaseShaderCompiler GLEW_GET_FUN(__glewReleaseShaderCompiler)
#define glShaderBinary GLEW_GET_FUN(__glewShaderBinary)

#define GLEW_ARB_ES2_compatibility GLEW_GET_VAR(__GLEW_ARB_ES2_compatibility)

#endif /* GL_ARB_ES2_compatibility */

/* ----------------------- GL_ARB_ES3_1_compatibility ---------------------- */

#ifndef GL_ARB_ES3_1_compatibility
#define GL_ARB_ES3_1_compatibility 1

typedef void (GLAPIENTRY * PFNGLMEMORYBARRIERBYREGIONPROC) (GLbitfield barriers);

#define glMemoryBarrierByRegion GLEW_GET_FUN(__glewMemoryBarrierByRegion)

#define GLEW_ARB_ES3_1_compatibility GLEW_GET_VAR(__GLEW_ARB_ES3_1_compatibility)

#endif /* GL_ARB_ES3_1_compatibility */

/* ----------------------- GL_ARB_ES3_2_compatibility ---------------------- */

#ifndef GL_ARB_ES3_2_compatibility
#define GL_ARB_ES3_2_compatibility 1

#define GL_PRIMITIVE_BOUNDING_BOX_ARB 0x92BE
#define GL_MULTISAMPLE_LINE_WIDTH_RANGE_ARB 0x9381
#define GL_MULTISAMPLE_LINE_WIDTH_GRANULARITY_ARB 0x9382

typedef void (GLAPIENTRY * PFNGLPRIMITIVEBOUNDINGBOXARBPROC) (GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW);

#define glPrimitiveBoundingBoxARB GLEW_GET_FUN(__glewPrimitiveBoundingBoxARB)

#define GLEW_ARB_ES3_2_compatibility GLEW_GET_VAR(__GLEW_ARB_ES3_2_compatibility)

#endif /* GL_ARB_ES3_2_compatibility */

/* ------------------------ GL_ARB_ES3_compatibility ----------------------- */

#ifndef GL_ARB_ES3_compatibility
#define GL_ARB_ES3_compatibility 1

#define GL_TEXTURE_IMMUTABLE_LEVELS 0x82DF
#define GL_PRIMITIVE_RESTART_FIXED_INDEX 0x8D69
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE 0x8D6A
#define GL_MAX_ELEMENT_INDEX 0x8D6B
#define GL_COMPRESSED_R11_EAC 0x9270
#define GL_COMPRESSED_SIGNED_R11_EAC 0x9271
#define GL_COMPRESSED_RG11_EAC 0x9272
#define GL_COMPRESSED_SIGNED_RG11_EAC 0x9273
#define GL_COMPRESSED_RGB8_ETC2 0x9274
#define GL_COMPRESSED_SRGB8_ETC2 0x9275
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9277
#define GL_COMPRESSED_RGBA8_ETC2_EAC 0x9278
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0x9279

#define GLEW_ARB_ES3_compatibility GLEW_GET_VAR(__GLEW_ARB_ES3_compatibility)

#endif /* GL_ARB_ES3_compatibility */

/* ------------------------ GL_ARB_arrays_of_arrays ------------------------ */

#ifndef GL_ARB_arrays_of_arrays
#define GL_ARB_arrays_of_arrays 1

#define GLEW_ARB_arrays_of_arrays GLEW_GET_VAR(__GLEW_ARB_arrays_of_arrays)

#endif /* GL_ARB_arrays_of_arrays */

/* -------------------------- GL_ARB_base_instance ------------------------- */

#ifndef GL_ARB_base_instance
#define GL_ARB_base_instance 1

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC) (GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount, GLuint baseinstance);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance);

#define glDrawArraysInstancedBaseInstance GLEW_GET_FUN(__glewDrawArraysInstancedBaseInstance)
#define glDrawElementsInstancedBaseInstance GLEW_GET_FUN(__glewDrawElementsInstancedBaseInstance)
#define glDrawElementsInstancedBaseVertexBaseInstance GLEW_GET_FUN(__glewDrawElementsInstancedBaseVertexBaseInstance)

#define GLEW_ARB_base_instance GLEW_GET_VAR(__GLEW_ARB_base_instance)

#endif /* GL_ARB_base_instance */

/* ------------------------ GL_ARB_bindless_texture ------------------------ */

#ifndef GL_ARB_bindless_texture
#define GL_ARB_bindless_texture 1

#define GL_UNSIGNED_INT64_ARB 0x140F

typedef GLuint64 (GLAPIENTRY * PFNGLGETIMAGEHANDLEARBPROC) (GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
typedef GLuint64 (GLAPIENTRY * PFNGLGETTEXTUREHANDLEARBPROC) (GLuint texture);
typedef GLuint64 (GLAPIENTRY * PFNGLGETTEXTURESAMPLERHANDLEARBPROC) (GLuint texture, GLuint sampler);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBLUI64VARBPROC) (GLuint index, GLenum pname, GLuint64EXT* params);
typedef GLboolean (GLAPIENTRY * PFNGLISIMAGEHANDLERESIDENTARBPROC) (GLuint64 handle);
typedef GLboolean (GLAPIENTRY * PFNGLISTEXTUREHANDLERESIDENTARBPROC) (GLuint64 handle);
typedef void (GLAPIENTRY * PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC) (GLuint64 handle);
typedef void (GLAPIENTRY * PFNGLMAKEIMAGEHANDLERESIDENTARBPROC) (GLuint64 handle, GLenum access);
typedef void (GLAPIENTRY * PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC) (GLuint64 handle);
typedef void (GLAPIENTRY * PFNGLMAKETEXTUREHANDLERESIDENTARBPROC) (GLuint64 handle);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC) (GLuint program, GLint location, GLuint64 value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC) (GLuint program, GLint location, GLsizei count, const GLuint64* values);
typedef void (GLAPIENTRY * PFNGLUNIFORMHANDLEUI64ARBPROC) (GLint location, GLuint64 value);
typedef void (GLAPIENTRY * PFNGLUNIFORMHANDLEUI64VARBPROC) (GLint location, GLsizei count, const GLuint64* value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1UI64ARBPROC) (GLuint index, GLuint64EXT x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1UI64VARBPROC) (GLuint index, const GLuint64EXT* v);

#define glGetImageHandleARB GLEW_GET_FUN(__glewGetImageHandleARB)
#define glGetTextureHandleARB GLEW_GET_FUN(__glewGetTextureHandleARB)
#define glGetTextureSamplerHandleARB GLEW_GET_FUN(__glewGetTextureSamplerHandleARB)
#define glGetVertexAttribLui64vARB GLEW_GET_FUN(__glewGetVertexAttribLui64vARB)
#define glIsImageHandleResidentARB GLEW_GET_FUN(__glewIsImageHandleResidentARB)
#define glIsTextureHandleResidentARB GLEW_GET_FUN(__glewIsTextureHandleResidentARB)
#define glMakeImageHandleNonResidentARB GLEW_GET_FUN(__glewMakeImageHandleNonResidentARB)
#define glMakeImageHandleResidentARB GLEW_GET_FUN(__glewMakeImageHandleResidentARB)
#define glMakeTextureHandleNonResidentARB GLEW_GET_FUN(__glewMakeTextureHandleNonResidentARB)
#define glMakeTextureHandleResidentARB GLEW_GET_FUN(__glewMakeTextureHandleResidentARB)
#define glProgramUniformHandleui64ARB GLEW_GET_FUN(__glewProgramUniformHandleui64ARB)
#define glProgramUniformHandleui64vARB GLEW_GET_FUN(__glewProgramUniformHandleui64vARB)
#define glUniformHandleui64ARB GLEW_GET_FUN(__glewUniformHandleui64ARB)
#define glUniformHandleui64vARB GLEW_GET_FUN(__glewUniformHandleui64vARB)
#define glVertexAttribL1ui64ARB GLEW_GET_FUN(__glewVertexAttribL1ui64ARB)
#define glVertexAttribL1ui64vARB GLEW_GET_FUN(__glewVertexAttribL1ui64vARB)

#define GLEW_ARB_bindless_texture GLEW_GET_VAR(__GLEW_ARB_bindless_texture)

#endif /* GL_ARB_bindless_texture */

/* ----------------------- GL_ARB_blend_func_extended ---------------------- */

#ifndef GL_ARB_blend_func_extended
#define GL_ARB_blend_func_extended 1

#define GL_SRC1_COLOR 0x88F9
#define GL_ONE_MINUS_SRC1_COLOR 0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA 0x88FB
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS 0x88FC

typedef void (GLAPIENTRY * PFNGLBINDFRAGDATALOCATIONINDEXEDPROC) (GLuint program, GLuint colorNumber, GLuint index, const GLchar * name);
typedef GLint (GLAPIENTRY * PFNGLGETFRAGDATAINDEXPROC) (GLuint program, const GLchar * name);

#define glBindFragDataLocationIndexed GLEW_GET_FUN(__glewBindFragDataLocationIndexed)
#define glGetFragDataIndex GLEW_GET_FUN(__glewGetFragDataIndex)

#define GLEW_ARB_blend_func_extended GLEW_GET_VAR(__GLEW_ARB_blend_func_extended)

#endif /* GL_ARB_blend_func_extended */

/* ------------------------- GL_ARB_buffer_storage ------------------------- */

#ifndef GL_ARB_buffer_storage
#define GL_ARB_buffer_storage 1

#define GL_MAP_READ_BIT 0x0001
#define GL_MAP_WRITE_BIT 0x0002
#define GL_MAP_PERSISTENT_BIT 0x00000040
#define GL_MAP_COHERENT_BIT 0x00000080
#define GL_DYNAMIC_STORAGE_BIT 0x0100
#define GL_CLIENT_STORAGE_BIT 0x0200
#define GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT 0x00004000
#define GL_BUFFER_IMMUTABLE_STORAGE 0x821F
#define GL_BUFFER_STORAGE_FLAGS 0x8220

typedef void (GLAPIENTRY * PFNGLBUFFERSTORAGEPROC) (GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);

#define glBufferStorage GLEW_GET_FUN(__glewBufferStorage)

#define GLEW_ARB_buffer_storage GLEW_GET_VAR(__GLEW_ARB_buffer_storage)

#endif /* GL_ARB_buffer_storage */

/* ---------------------------- GL_ARB_cl_event ---------------------------- */

#ifndef GL_ARB_cl_event
#define GL_ARB_cl_event 1

#define GL_SYNC_CL_EVENT_ARB 0x8240
#define GL_SYNC_CL_EVENT_COMPLETE_ARB 0x8241

typedef struct _cl_context *cl_context;
typedef struct _cl_event *cl_event;

typedef GLsync (GLAPIENTRY * PFNGLCREATESYNCFROMCLEVENTARBPROC) (cl_context context, cl_event event, GLbitfield flags);

#define glCreateSyncFromCLeventARB GLEW_GET_FUN(__glewCreateSyncFromCLeventARB)

#define GLEW_ARB_cl_event GLEW_GET_VAR(__GLEW_ARB_cl_event)

#endif /* GL_ARB_cl_event */

/* ----------------------- GL_ARB_clear_buffer_object ---------------------- */

#ifndef GL_ARB_clear_buffer_object
#define GL_ARB_clear_buffer_object 1

typedef void (GLAPIENTRY * PFNGLCLEARBUFFERDATAPROC) (GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERSUBDATAPROC) (GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
typedef void (GLAPIENTRY * PFNGLCLEARNAMEDBUFFERDATAEXTPROC) (GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
typedef void (GLAPIENTRY * PFNGLCLEARNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);

#define glClearBufferData GLEW_GET_FUN(__glewClearBufferData)
#define glClearBufferSubData GLEW_GET_FUN(__glewClearBufferSubData)
#define glClearNamedBufferDataEXT GLEW_GET_FUN(__glewClearNamedBufferDataEXT)
#define glClearNamedBufferSubDataEXT GLEW_GET_FUN(__glewClearNamedBufferSubDataEXT)

#define GLEW_ARB_clear_buffer_object GLEW_GET_VAR(__GLEW_ARB_clear_buffer_object)

#endif /* GL_ARB_clear_buffer_object */

/* -------------------------- GL_ARB_clear_texture ------------------------- */

#ifndef GL_ARB_clear_texture
#define GL_ARB_clear_texture 1

#define GL_CLEAR_TEXTURE 0x9365

typedef void (GLAPIENTRY * PFNGLCLEARTEXIMAGEPROC) (GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
typedef void (GLAPIENTRY * PFNGLCLEARTEXSUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);

#define glClearTexImage GLEW_GET_FUN(__glewClearTexImage)
#define glClearTexSubImage GLEW_GET_FUN(__glewClearTexSubImage)

#define GLEW_ARB_clear_texture GLEW_GET_VAR(__GLEW_ARB_clear_texture)

#endif /* GL_ARB_clear_texture */

/* -------------------------- GL_ARB_clip_control -------------------------- */

#ifndef GL_ARB_clip_control
#define GL_ARB_clip_control 1

#define GL_LOWER_LEFT 0x8CA1
#define GL_UPPER_LEFT 0x8CA2
#define GL_CLIP_ORIGIN 0x935C
#define GL_CLIP_DEPTH_MODE 0x935D
#define GL_NEGATIVE_ONE_TO_ONE 0x935E
#define GL_ZERO_TO_ONE 0x935F

typedef void (GLAPIENTRY * PFNGLCLIPCONTROLPROC) (GLenum origin, GLenum depth);

#define glClipControl GLEW_GET_FUN(__glewClipControl)

#define GLEW_ARB_clip_control GLEW_GET_VAR(__GLEW_ARB_clip_control)

#endif /* GL_ARB_clip_control */

/* ----------------------- GL_ARB_color_buffer_float ----------------------- */

#ifndef GL_ARB_color_buffer_float
#define GL_ARB_color_buffer_float 1

#define GL_RGBA_FLOAT_MODE_ARB 0x8820
#define GL_CLAMP_VERTEX_COLOR_ARB 0x891A
#define GL_CLAMP_FRAGMENT_COLOR_ARB 0x891B
#define GL_CLAMP_READ_COLOR_ARB 0x891C
#define GL_FIXED_ONLY_ARB 0x891D

typedef void (GLAPIENTRY * PFNGLCLAMPCOLORARBPROC) (GLenum target, GLenum clamp);

#define glClampColorARB GLEW_GET_FUN(__glewClampColorARB)

#define GLEW_ARB_color_buffer_float GLEW_GET_VAR(__GLEW_ARB_color_buffer_float)

#endif /* GL_ARB_color_buffer_float */

/* -------------------------- GL_ARB_compatibility ------------------------- */

#ifndef GL_ARB_compatibility
#define GL_ARB_compatibility 1

#define GLEW_ARB_compatibility GLEW_GET_VAR(__GLEW_ARB_compatibility)

#endif /* GL_ARB_compatibility */

/* ---------------- GL_ARB_compressed_texture_pixel_storage ---------------- */

#ifndef GL_ARB_compressed_texture_pixel_storage
#define GL_ARB_compressed_texture_pixel_storage 1

#define GL_UNPACK_COMPRESSED_BLOCK_WIDTH 0x9127
#define GL_UNPACK_COMPRESSED_BLOCK_HEIGHT 0x9128
#define GL_UNPACK_COMPRESSED_BLOCK_DEPTH 0x9129
#define GL_UNPACK_COMPRESSED_BLOCK_SIZE 0x912A
#define GL_PACK_COMPRESSED_BLOCK_WIDTH 0x912B
#define GL_PACK_COMPRESSED_BLOCK_HEIGHT 0x912C
#define GL_PACK_COMPRESSED_BLOCK_DEPTH 0x912D
#define GL_PACK_COMPRESSED_BLOCK_SIZE 0x912E

#define GLEW_ARB_compressed_texture_pixel_storage GLEW_GET_VAR(__GLEW_ARB_compressed_texture_pixel_storage)

#endif /* GL_ARB_compressed_texture_pixel_storage */

/* ------------------------- GL_ARB_compute_shader ------------------------- */

#ifndef GL_ARB_compute_shader
#define GL_ARB_compute_shader 1

#define GL_COMPUTE_SHADER_BIT 0x00000020
#define GL_MAX_COMPUTE_SHARED_MEMORY_SIZE 0x8262
#define GL_MAX_COMPUTE_UNIFORM_COMPONENTS 0x8263
#define GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS 0x8264
#define GL_MAX_COMPUTE_ATOMIC_COUNTERS 0x8265
#define GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS 0x8266
#define GL_COMPUTE_WORK_GROUP_SIZE 0x8267
#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB
#define GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER 0x90EC
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER 0x90ED
#define GL_DISPATCH_INDIRECT_BUFFER 0x90EE
#define GL_DISPATCH_INDIRECT_BUFFER_BINDING 0x90EF
#define GL_COMPUTE_SHADER 0x91B9
#define GL_MAX_COMPUTE_UNIFORM_BLOCKS 0x91BB
#define GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS 0x91BC
#define GL_MAX_COMPUTE_IMAGE_UNIFORMS 0x91BD
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT 0x91BE
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE 0x91BF

typedef void (GLAPIENTRY * PFNGLDISPATCHCOMPUTEPROC) (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
typedef void (GLAPIENTRY * PFNGLDISPATCHCOMPUTEINDIRECTPROC) (GLintptr indirect);

#define glDispatchCompute GLEW_GET_FUN(__glewDispatchCompute)
#define glDispatchComputeIndirect GLEW_GET_FUN(__glewDispatchComputeIndirect)

#define GLEW_ARB_compute_shader GLEW_GET_VAR(__GLEW_ARB_compute_shader)

#endif /* GL_ARB_compute_shader */

/* ------------------- GL_ARB_compute_variable_group_size ------------------ */

#ifndef GL_ARB_compute_variable_group_size
#define GL_ARB_compute_variable_group_size 1

#define GL_MAX_COMPUTE_FIXED_GROUP_INVOCATIONS_ARB 0x90EB
#define GL_MAX_COMPUTE_FIXED_GROUP_SIZE_ARB 0x91BF
#define GL_MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS_ARB 0x9344
#define GL_MAX_COMPUTE_VARIABLE_GROUP_SIZE_ARB 0x9345

typedef void (GLAPIENTRY * PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC) (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLuint group_size_x, GLuint group_size_y, GLuint group_size_z);

#define glDispatchComputeGroupSizeARB GLEW_GET_FUN(__glewDispatchComputeGroupSizeARB)

#define GLEW_ARB_compute_variable_group_size GLEW_GET_VAR(__GLEW_ARB_compute_variable_group_size)

#endif /* GL_ARB_compute_variable_group_size */

/* ------------------- GL_ARB_conditional_render_inverted ------------------ */

#ifndef GL_ARB_conditional_render_inverted
#define GL_ARB_conditional_render_inverted 1

#define GL_QUERY_WAIT_INVERTED 0x8E17
#define GL_QUERY_NO_WAIT_INVERTED 0x8E18
#define GL_QUERY_BY_REGION_WAIT_INVERTED 0x8E19
#define GL_QUERY_BY_REGION_NO_WAIT_INVERTED 0x8E1A

#define GLEW_ARB_conditional_render_inverted GLEW_GET_VAR(__GLEW_ARB_conditional_render_inverted)

#endif /* GL_ARB_conditional_render_inverted */

/* ----------------------- GL_ARB_conservative_depth ----------------------- */

#ifndef GL_ARB_conservative_depth
#define GL_ARB_conservative_depth 1

#define GLEW_ARB_conservative_depth GLEW_GET_VAR(__GLEW_ARB_conservative_depth)

#endif /* GL_ARB_conservative_depth */

/* --------------------------- GL_ARB_copy_buffer -------------------------- */

#ifndef GL_ARB_copy_buffer
#define GL_ARB_copy_buffer 1

#define GL_COPY_READ_BUFFER 0x8F36
#define GL_COPY_WRITE_BUFFER 0x8F37

typedef void (GLAPIENTRY * PFNGLCOPYBUFFERSUBDATAPROC) (GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);

#define glCopyBufferSubData GLEW_GET_FUN(__glewCopyBufferSubData)

#define GLEW_ARB_copy_buffer GLEW_GET_VAR(__GLEW_ARB_copy_buffer)

#endif /* GL_ARB_copy_buffer */

/* --------------------------- GL_ARB_copy_image --------------------------- */

#ifndef GL_ARB_copy_image
#define GL_ARB_copy_image 1

typedef void (GLAPIENTRY * PFNGLCOPYIMAGESUBDATAPROC) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);

#define glCopyImageSubData GLEW_GET_FUN(__glewCopyImageSubData)

#define GLEW_ARB_copy_image GLEW_GET_VAR(__GLEW_ARB_copy_image)

#endif /* GL_ARB_copy_image */

/* -------------------------- GL_ARB_cull_distance ------------------------- */

#ifndef GL_ARB_cull_distance
#define GL_ARB_cull_distance 1

#define GL_MAX_CULL_DISTANCES 0x82F9
#define GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES 0x82FA

#define GLEW_ARB_cull_distance GLEW_GET_VAR(__GLEW_ARB_cull_distance)

#endif /* GL_ARB_cull_distance */

/* -------------------------- GL_ARB_debug_output -------------------------- */

#ifndef GL_ARB_debug_output
#define GL_ARB_debug_output 1

#define GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB 0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION_ARB 0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM_ARB 0x8245
#define GL_DEBUG_SOURCE_API_ARB 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER_ARB 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY_ARB 0x8249
#define GL_DEBUG_SOURCE_APPLICATION_ARB 0x824A
#define GL_DEBUG_SOURCE_OTHER_ARB 0x824B
#define GL_DEBUG_TYPE_ERROR_ARB 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB 0x824E
#define GL_DEBUG_TYPE_PORTABILITY_ARB 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE_ARB 0x8250
#define GL_DEBUG_TYPE_OTHER_ARB 0x8251
#define GL_MAX_DEBUG_MESSAGE_LENGTH_ARB 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES_ARB 0x9144
#define GL_DEBUG_LOGGED_MESSAGES_ARB 0x9145
#define GL_DEBUG_SEVERITY_HIGH_ARB 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM_ARB 0x9147
#define GL_DEBUG_SEVERITY_LOW_ARB 0x9148

typedef void (GLAPIENTRY *GLDEBUGPROCARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECALLBACKARBPROC) (GLDEBUGPROCARB callback, const void *userParam);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECONTROLARBPROC) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGEINSERTARBPROC) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
typedef GLuint (GLAPIENTRY * PFNGLGETDEBUGMESSAGELOGARBPROC) (GLuint count, GLsizei bufSize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog);

#define glDebugMessageCallbackARB GLEW_GET_FUN(__glewDebugMessageCallbackARB)
#define glDebugMessageControlARB GLEW_GET_FUN(__glewDebugMessageControlARB)
#define glDebugMessageInsertARB GLEW_GET_FUN(__glewDebugMessageInsertARB)
#define glGetDebugMessageLogARB GLEW_GET_FUN(__glewGetDebugMessageLogARB)

#define GLEW_ARB_debug_output GLEW_GET_VAR(__GLEW_ARB_debug_output)

#endif /* GL_ARB_debug_output */

/* ----------------------- GL_ARB_depth_buffer_float ----------------------- */

#ifndef GL_ARB_depth_buffer_float
#define GL_ARB_depth_buffer_float 1

#define GL_DEPTH_COMPONENT32F 0x8CAC
#define GL_DEPTH32F_STENCIL8 0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD

#define GLEW_ARB_depth_buffer_float GLEW_GET_VAR(__GLEW_ARB_depth_buffer_float)

#endif /* GL_ARB_depth_buffer_float */

/* --------------------------- GL_ARB_depth_clamp -------------------------- */

#ifndef GL_ARB_depth_clamp
#define GL_ARB_depth_clamp 1

#define GL_DEPTH_CLAMP 0x864F

#define GLEW_ARB_depth_clamp GLEW_GET_VAR(__GLEW_ARB_depth_clamp)

#endif /* GL_ARB_depth_clamp */

/* -------------------------- GL_ARB_depth_texture ------------------------- */

#ifndef GL_ARB_depth_texture
#define GL_ARB_depth_texture 1

#define GL_DEPTH_COMPONENT16_ARB 0x81A5
#define GL_DEPTH_COMPONENT24_ARB 0x81A6
#define GL_DEPTH_COMPONENT32_ARB 0x81A7
#define GL_TEXTURE_DEPTH_SIZE_ARB 0x884A
#define GL_DEPTH_TEXTURE_MODE_ARB 0x884B

#define GLEW_ARB_depth_texture GLEW_GET_VAR(__GLEW_ARB_depth_texture)

#endif /* GL_ARB_depth_texture */

/* ----------------------- GL_ARB_derivative_control ----------------------- */

#ifndef GL_ARB_derivative_control
#define GL_ARB_derivative_control 1

#define GLEW_ARB_derivative_control GLEW_GET_VAR(__GLEW_ARB_derivative_control)

#endif /* GL_ARB_derivative_control */

/* ----------------------- GL_ARB_direct_state_access ---------------------- */

#ifndef GL_ARB_direct_state_access
#define GL_ARB_direct_state_access 1

#define GL_TEXTURE_TARGET 0x1006
#define GL_QUERY_TARGET 0x82EA

typedef void (GLAPIENTRY * PFNGLBINDTEXTUREUNITPROC) (GLuint unit, GLuint texture);
typedef void (GLAPIENTRY * PFNGLBLITNAMEDFRAMEBUFFERPROC) (GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef GLenum (GLAPIENTRY * PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC) (GLuint framebuffer, GLenum target);
typedef void (GLAPIENTRY * PFNGLCLEARNAMEDBUFFERDATAPROC) (GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
typedef void (GLAPIENTRY * PFNGLCLEARNAMEDBUFFERSUBDATAPROC) (GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
typedef void (GLAPIENTRY * PFNGLCLEARNAMEDFRAMEBUFFERFIPROC) (GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
typedef void (GLAPIENTRY * PFNGLCLEARNAMEDFRAMEBUFFERFVPROC) (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLCLEARNAMEDFRAMEBUFFERIVPROC) (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint* value);
typedef void (GLAPIENTRY * PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC) (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC) (GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOPYNAMEDBUFFERSUBDATAPROC) (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE1DPROC) (GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE2DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE3DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCREATEBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLCREATEFRAMEBUFFERSPROC) (GLsizei n, GLuint* framebuffers);
typedef void (GLAPIENTRY * PFNGLCREATEPROGRAMPIPELINESPROC) (GLsizei n, GLuint* pipelines);
typedef void (GLAPIENTRY * PFNGLCREATEQUERIESPROC) (GLenum target, GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLCREATERENDERBUFFERSPROC) (GLsizei n, GLuint* renderbuffers);
typedef void (GLAPIENTRY * PFNGLCREATESAMPLERSPROC) (GLsizei n, GLuint* samplers);
typedef void (GLAPIENTRY * PFNGLCREATETEXTURESPROC) (GLenum target, GLsizei n, GLuint* textures);
typedef void (GLAPIENTRY * PFNGLCREATETRANSFORMFEEDBACKSPROC) (GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLCREATEVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXARRAYATTRIBPROC) (GLuint vaobj, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXARRAYATTRIBPROC) (GLuint vaobj, GLuint index);
typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void (GLAPIENTRY * PFNGLGENERATETEXTUREMIPMAPPROC) (GLuint texture);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC) (GLuint texture, GLint level, GLsizei bufSize, void *pixels);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPARAMETERI64VPROC) (GLuint buffer, GLenum pname, GLint64* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPARAMETERIVPROC) (GLuint buffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPOINTERVPROC) (GLuint buffer, GLenum pname, void** params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERSUBDATAPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);
typedef void (GLAPIENTRY * PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLuint framebuffer, GLenum attachment, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC) (GLuint framebuffer, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC) (GLuint renderbuffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYBUFFEROBJECTI64VPROC) (GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLGETQUERYBUFFEROBJECTIVPROC) (GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLGETQUERYBUFFEROBJECTUI64VPROC) (GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLGETQUERYBUFFEROBJECTUIVPROC) (GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREIMAGEPROC) (GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERFVPROC) (GLuint texture, GLint level, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERIVPROC) (GLuint texture, GLint level, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIIVPROC) (GLuint texture, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIUIVPROC) (GLuint texture, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERFVPROC) (GLuint texture, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIVPROC) (GLuint texture, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKI64_VPROC) (GLuint xfb, GLenum pname, GLuint index, GLint64* param);
typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKI_VPROC) (GLuint xfb, GLenum pname, GLuint index, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKIVPROC) (GLuint xfb, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYINDEXED64IVPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint64* param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYINDEXEDIVPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYIVPROC) (GLuint vaobj, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC) (GLuint framebuffer, GLsizei numAttachments, const GLenum* attachments);
typedef void (GLAPIENTRY * PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC) (GLuint framebuffer, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void * (GLAPIENTRY * PFNGLMAPNAMEDBUFFERPROC) (GLuint buffer, GLenum access);
typedef void * (GLAPIENTRY * PFNGLMAPNAMEDBUFFERRANGEPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERDATAPROC) (GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERSTORAGEPROC) (GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERSUBDATAPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC) (GLuint framebuffer, GLsizei n, const GLenum* bufs);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC) (GLuint framebuffer, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC) (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEPROC) (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLTEXTUREBUFFERPROC) (GLuint texture, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLTEXTUREBUFFERRANGEPROC) (GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIIVPROC) (GLuint texture, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIUIVPROC) (GLuint texture, GLenum pname, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFPROC) (GLuint texture, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFVPROC) (GLuint texture, GLenum pname, const GLfloat* param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIPROC) (GLuint texture, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIVPROC) (GLuint texture, GLenum pname, const GLint* param);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE1DPROC) (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE2DPROC) (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC) (GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE3DPROC) (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC) (GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE1DPROC) (GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE2DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE3DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC) (GLuint xfb, GLuint index, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC) (GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef GLboolean (GLAPIENTRY * PFNGLUNMAPNAMEDBUFFERPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYATTRIBBINDINGPROC) (GLuint vaobj, GLuint attribindex, GLuint bindingindex);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYATTRIBFORMATPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYATTRIBIFORMATPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYATTRIBLFORMATPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYBINDINGDIVISORPROC) (GLuint vaobj, GLuint bindingindex, GLuint divisor);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYELEMENTBUFFERPROC) (GLuint vaobj, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXBUFFERPROC) (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXBUFFERSPROC) (GLuint vaobj, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr *offsets, const GLsizei *strides);

#define glBindTextureUnit GLEW_GET_FUN(__glewBindTextureUnit)
#define glBlitNamedFramebuffer GLEW_GET_FUN(__glewBlitNamedFramebuffer)
#define glCheckNamedFramebufferStatus GLEW_GET_FUN(__glewCheckNamedFramebufferStatus)
#define glClearNamedBufferData GLEW_GET_FUN(__glewClearNamedBufferData)
#define glClearNamedBufferSubData GLEW_GET_FUN(__glewClearNamedBufferSubData)
#define glClearNamedFramebufferfi GLEW_GET_FUN(__glewClearNamedFramebufferfi)
#define glClearNamedFramebufferfv GLEW_GET_FUN(__glewClearNamedFramebufferfv)
#define glClearNamedFramebufferiv GLEW_GET_FUN(__glewClearNamedFramebufferiv)
#define glClearNamedFramebufferuiv GLEW_GET_FUN(__glewClearNamedFramebufferuiv)
#define glCompressedTextureSubImage1D GLEW_GET_FUN(__glewCompressedTextureSubImage1D)
#define glCompressedTextureSubImage2D GLEW_GET_FUN(__glewCompressedTextureSubImage2D)
#define glCompressedTextureSubImage3D GLEW_GET_FUN(__glewCompressedTextureSubImage3D)
#define glCopyNamedBufferSubData GLEW_GET_FUN(__glewCopyNamedBufferSubData)
#define glCopyTextureSubImage1D GLEW_GET_FUN(__glewCopyTextureSubImage1D)
#define glCopyTextureSubImage2D GLEW_GET_FUN(__glewCopyTextureSubImage2D)
#define glCopyTextureSubImage3D GLEW_GET_FUN(__glewCopyTextureSubImage3D)
#define glCreateBuffers GLEW_GET_FUN(__glewCreateBuffers)
#define glCreateFramebuffers GLEW_GET_FUN(__glewCreateFramebuffers)
#define glCreateProgramPipelines GLEW_GET_FUN(__glewCreateProgramPipelines)
#define glCreateQueries GLEW_GET_FUN(__glewCreateQueries)
#define glCreateRenderbuffers GLEW_GET_FUN(__glewCreateRenderbuffers)
#define glCreateSamplers GLEW_GET_FUN(__glewCreateSamplers)
#define glCreateTextures GLEW_GET_FUN(__glewCreateTextures)
#define glCreateTransformFeedbacks GLEW_GET_FUN(__glewCreateTransformFeedbacks)
#define glCreateVertexArrays GLEW_GET_FUN(__glewCreateVertexArrays)
#define glDisableVertexArrayAttrib GLEW_GET_FUN(__glewDisableVertexArrayAttrib)
#define glEnableVertexArrayAttrib GLEW_GET_FUN(__glewEnableVertexArrayAttrib)
#define glFlushMappedNamedBufferRange GLEW_GET_FUN(__glewFlushMappedNamedBufferRange)
#define glGenerateTextureMipmap GLEW_GET_FUN(__glewGenerateTextureMipmap)
#define glGetCompressedTextureImage GLEW_GET_FUN(__glewGetCompressedTextureImage)
#define glGetNamedBufferParameteri64v GLEW_GET_FUN(__glewGetNamedBufferParameteri64v)
#define glGetNamedBufferParameteriv GLEW_GET_FUN(__glewGetNamedBufferParameteriv)
#define glGetNamedBufferPointerv GLEW_GET_FUN(__glewGetNamedBufferPointerv)
#define glGetNamedBufferSubData GLEW_GET_FUN(__glewGetNamedBufferSubData)
#define glGetNamedFramebufferAttachmentParameteriv GLEW_GET_FUN(__glewGetNamedFramebufferAttachmentParameteriv)
#define glGetNamedFramebufferParameteriv GLEW_GET_FUN(__glewGetNamedFramebufferParameteriv)
#define glGetNamedRenderbufferParameteriv GLEW_GET_FUN(__glewGetNamedRenderbufferParameteriv)
#define glGetQueryBufferObjecti64v GLEW_GET_FUN(__glewGetQueryBufferObjecti64v)
#define glGetQueryBufferObjectiv GLEW_GET_FUN(__glewGetQueryBufferObjectiv)
#define glGetQueryBufferObjectui64v GLEW_GET_FUN(__glewGetQueryBufferObjectui64v)
#define glGetQueryBufferObjectuiv GLEW_GET_FUN(__glewGetQueryBufferObjectuiv)
#define glGetTextureImage GLEW_GET_FUN(__glewGetTextureImage)
#define glGetTextureLevelParameterfv GLEW_GET_FUN(__glewGetTextureLevelParameterfv)
#define glGetTextureLevelParameteriv GLEW_GET_FUN(__glewGetTextureLevelParameteriv)
#define glGetTextureParameterIiv GLEW_GET_FUN(__glewGetTextureParameterIiv)
#define glGetTextureParameterIuiv GLEW_GET_FUN(__glewGetTextureParameterIuiv)
#define glGetTextureParameterfv GLEW_GET_FUN(__glewGetTextureParameterfv)
#define glGetTextureParameteriv GLEW_GET_FUN(__glewGetTextureParameteriv)
#define glGetTransformFeedbacki64_v GLEW_GET_FUN(__glewGetTransformFeedbacki64_v)
#define glGetTransformFeedbacki_v GLEW_GET_FUN(__glewGetTransformFeedbacki_v)
#define glGetTransformFeedbackiv GLEW_GET_FUN(__glewGetTransformFeedbackiv)
#define glGetVertexArrayIndexed64iv GLEW_GET_FUN(__glewGetVertexArrayIndexed64iv)
#define glGetVertexArrayIndexediv GLEW_GET_FUN(__glewGetVertexArrayIndexediv)
#define glGetVertexArrayiv GLEW_GET_FUN(__glewGetVertexArrayiv)
#define glInvalidateNamedFramebufferData GLEW_GET_FUN(__glewInvalidateNamedFramebufferData)
#define glInvalidateNamedFramebufferSubData GLEW_GET_FUN(__glewInvalidateNamedFramebufferSubData)
#define glMapNamedBuffer GLEW_GET_FUN(__glewMapNamedBuffer)
#define glMapNamedBufferRange GLEW_GET_FUN(__glewMapNamedBufferRange)
#define glNamedBufferData GLEW_GET_FUN(__glewNamedBufferData)
#define glNamedBufferStorage GLEW_GET_FUN(__glewNamedBufferStorage)
#define glNamedBufferSubData GLEW_GET_FUN(__glewNamedBufferSubData)
#define glNamedFramebufferDrawBuffer GLEW_GET_FUN(__glewNamedFramebufferDrawBuffer)
#define glNamedFramebufferDrawBuffers GLEW_GET_FUN(__glewNamedFramebufferDrawBuffers)
#define glNamedFramebufferParameteri GLEW_GET_FUN(__glewNamedFramebufferParameteri)
#define glNamedFramebufferReadBuffer GLEW_GET_FUN(__glewNamedFramebufferReadBuffer)
#define glNamedFramebufferRenderbuffer GLEW_GET_FUN(__glewNamedFramebufferRenderbuffer)
#define glNamedFramebufferTexture GLEW_GET_FUN(__glewNamedFramebufferTexture)
#define glNamedFramebufferTextureLayer GLEW_GET_FUN(__glewNamedFramebufferTextureLayer)
#define glNamedRenderbufferStorage GLEW_GET_FUN(__glewNamedRenderbufferStorage)
#define glNamedRenderbufferStorageMultisample GLEW_GET_FUN(__glewNamedRenderbufferStorageMultisample)
#define glTextureBuffer GLEW_GET_FUN(__glewTextureBuffer)
#define glTextureBufferRange GLEW_GET_FUN(__glewTextureBufferRange)
#define glTextureParameterIiv GLEW_GET_FUN(__glewTextureParameterIiv)
#define glTextureParameterIuiv GLEW_GET_FUN(__glewTextureParameterIuiv)
#define glTextureParameterf GLEW_GET_FUN(__glewTextureParameterf)
#define glTextureParameterfv GLEW_GET_FUN(__glewTextureParameterfv)
#define glTextureParameteri GLEW_GET_FUN(__glewTextureParameteri)
#define glTextureParameteriv GLEW_GET_FUN(__glewTextureParameteriv)
#define glTextureStorage1D GLEW_GET_FUN(__glewTextureStorage1D)
#define glTextureStorage2D GLEW_GET_FUN(__glewTextureStorage2D)
#define glTextureStorage2DMultisample GLEW_GET_FUN(__glewTextureStorage2DMultisample)
#define glTextureStorage3D GLEW_GET_FUN(__glewTextureStorage3D)
#define glTextureStorage3DMultisample GLEW_GET_FUN(__glewTextureStorage3DMultisample)
#define glTextureSubImage1D GLEW_GET_FUN(__glewTextureSubImage1D)
#define glTextureSubImage2D GLEW_GET_FUN(__glewTextureSubImage2D)
#define glTextureSubImage3D GLEW_GET_FUN(__glewTextureSubImage3D)
#define glTransformFeedbackBufferBase GLEW_GET_FUN(__glewTransformFeedbackBufferBase)
#define glTransformFeedbackBufferRange GLEW_GET_FUN(__glewTransformFeedbackBufferRange)
#define glUnmapNamedBuffer GLEW_GET_FUN(__glewUnmapNamedBuffer)
#define glVertexArrayAttribBinding GLEW_GET_FUN(__glewVertexArrayAttribBinding)
#define glVertexArrayAttribFormat GLEW_GET_FUN(__glewVertexArrayAttribFormat)
#define glVertexArrayAttribIFormat GLEW_GET_FUN(__glewVertexArrayAttribIFormat)
#define glVertexArrayAttribLFormat GLEW_GET_FUN(__glewVertexArrayAttribLFormat)
#define glVertexArrayBindingDivisor GLEW_GET_FUN(__glewVertexArrayBindingDivisor)
#define glVertexArrayElementBuffer GLEW_GET_FUN(__glewVertexArrayElementBuffer)
#define glVertexArrayVertexBuffer GLEW_GET_FUN(__glewVertexArrayVertexBuffer)
#define glVertexArrayVertexBuffers GLEW_GET_FUN(__glewVertexArrayVertexBuffers)

#define GLEW_ARB_direct_state_access GLEW_GET_VAR(__GLEW_ARB_direct_state_access)

#endif /* GL_ARB_direct_state_access */

/* -------------------------- GL_ARB_draw_buffers -------------------------- */

#ifndef GL_ARB_draw_buffers
#define GL_ARB_draw_buffers 1

#define GL_MAX_DRAW_BUFFERS_ARB 0x8824
#define GL_DRAW_BUFFER0_ARB 0x8825
#define GL_DRAW_BUFFER1_ARB 0x8826
#define GL_DRAW_BUFFER2_ARB 0x8827
#define GL_DRAW_BUFFER3_ARB 0x8828
#define GL_DRAW_BUFFER4_ARB 0x8829
#define GL_DRAW_BUFFER5_ARB 0x882A
#define GL_DRAW_BUFFER6_ARB 0x882B
#define GL_DRAW_BUFFER7_ARB 0x882C
#define GL_DRAW_BUFFER8_ARB 0x882D
#define GL_DRAW_BUFFER9_ARB 0x882E
#define GL_DRAW_BUFFER10_ARB 0x882F
#define GL_DRAW_BUFFER11_ARB 0x8830
#define GL_DRAW_BUFFER12_ARB 0x8831
#define GL_DRAW_BUFFER13_ARB 0x8832
#define GL_DRAW_BUFFER14_ARB 0x8833
#define GL_DRAW_BUFFER15_ARB 0x8834

typedef void (GLAPIENTRY * PFNGLDRAWBUFFERSARBPROC) (GLsizei n, const GLenum* bufs);

#define glDrawBuffersARB GLEW_GET_FUN(__glewDrawBuffersARB)

#define GLEW_ARB_draw_buffers GLEW_GET_VAR(__GLEW_ARB_draw_buffers)

#endif /* GL_ARB_draw_buffers */

/* ----------------------- GL_ARB_draw_buffers_blend ----------------------- */

#ifndef GL_ARB_draw_buffers_blend
#define GL_ARB_draw_buffers_blend 1

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEIARBPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONIARBPROC) (GLuint buf, GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEIARBPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCIARBPROC) (GLuint buf, GLenum src, GLenum dst);

#define glBlendEquationSeparateiARB GLEW_GET_FUN(__glewBlendEquationSeparateiARB)
#define glBlendEquationiARB GLEW_GET_FUN(__glewBlendEquationiARB)
#define glBlendFuncSeparateiARB GLEW_GET_FUN(__glewBlendFuncSeparateiARB)
#define glBlendFunciARB GLEW_GET_FUN(__glewBlendFunciARB)

#define GLEW_ARB_draw_buffers_blend GLEW_GET_VAR(__GLEW_ARB_draw_buffers_blend)

#endif /* GL_ARB_draw_buffers_blend */

/* -------------------- GL_ARB_draw_elements_base_vertex ------------------- */

#ifndef GL_ARB_draw_elements_base_vertex
#define GL_ARB_draw_elements_base_vertex 1

typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSBASEVERTEXPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount, GLint basevertex);
typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC) (GLenum mode, const GLsizei* count, GLenum type, const void *const *indices, GLsizei primcount, const GLint *basevertex);

#define glDrawElementsBaseVertex GLEW_GET_FUN(__glewDrawElementsBaseVertex)
#define glDrawElementsInstancedBaseVertex GLEW_GET_FUN(__glewDrawElementsInstancedBaseVertex)
#define glDrawRangeElementsBaseVertex GLEW_GET_FUN(__glewDrawRangeElementsBaseVertex)
#define glMultiDrawElementsBaseVertex GLEW_GET_FUN(__glewMultiDrawElementsBaseVertex)

#define GLEW_ARB_draw_elements_base_vertex GLEW_GET_VAR(__GLEW_ARB_draw_elements_base_vertex)

#endif /* GL_ARB_draw_elements_base_vertex */

/* -------------------------- GL_ARB_draw_indirect ------------------------- */

#ifndef GL_ARB_draw_indirect
#define GL_ARB_draw_indirect 1

#define GL_DRAW_INDIRECT_BUFFER 0x8F3F
#define GL_DRAW_INDIRECT_BUFFER_BINDING 0x8F43

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINDIRECTPROC) (GLenum mode, const void *indirect);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINDIRECTPROC) (GLenum mode, GLenum type, const void *indirect);

#define glDrawArraysIndirect GLEW_GET_FUN(__glewDrawArraysIndirect)
#define glDrawElementsIndirect GLEW_GET_FUN(__glewDrawElementsIndirect)

#define GLEW_ARB_draw_indirect GLEW_GET_VAR(__GLEW_ARB_draw_indirect)

#endif /* GL_ARB_draw_indirect */

/* ------------------------- GL_ARB_draw_instanced ------------------------- */

#ifndef GL_ARB_draw_instanced
#define GL_ARB_draw_instanced 1

#define GLEW_ARB_draw_instanced GLEW_GET_VAR(__GLEW_ARB_draw_instanced)

#endif /* GL_ARB_draw_instanced */

/* ------------------------ GL_ARB_enhanced_layouts ------------------------ */

#ifndef GL_ARB_enhanced_layouts
#define GL_ARB_enhanced_layouts 1

#define GL_LOCATION_COMPONENT 0x934A
#define GL_TRANSFORM_FEEDBACK_BUFFER_INDEX 0x934B
#define GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE 0x934C

#define GLEW_ARB_enhanced_layouts GLEW_GET_VAR(__GLEW_ARB_enhanced_layouts)

#endif /* GL_ARB_enhanced_layouts */

/* -------------------- GL_ARB_explicit_attrib_location -------------------- */

#ifndef GL_ARB_explicit_attrib_location
#define GL_ARB_explicit_attrib_location 1

#define GLEW_ARB_explicit_attrib_location GLEW_GET_VAR(__GLEW_ARB_explicit_attrib_location)

#endif /* GL_ARB_explicit_attrib_location */

/* -------------------- GL_ARB_explicit_uniform_location ------------------- */

#ifndef GL_ARB_explicit_uniform_location
#define GL_ARB_explicit_uniform_location 1

#define GL_MAX_UNIFORM_LOCATIONS 0x826E

#define GLEW_ARB_explicit_uniform_location GLEW_GET_VAR(__GLEW_ARB_explicit_uniform_location)

#endif /* GL_ARB_explicit_uniform_location */

/* ------------------- GL_ARB_fragment_coord_conventions ------------------- */

#ifndef GL_ARB_fragment_coord_conventions
#define GL_ARB_fragment_coord_conventions 1

#define GLEW_ARB_fragment_coord_conventions GLEW_GET_VAR(__GLEW_ARB_fragment_coord_conventions)

#endif /* GL_ARB_fragment_coord_conventions */

/* --------------------- GL_ARB_fragment_layer_viewport -------------------- */

#ifndef GL_ARB_fragment_layer_viewport
#define GL_ARB_fragment_layer_viewport 1

#define GLEW_ARB_fragment_layer_viewport GLEW_GET_VAR(__GLEW_ARB_fragment_layer_viewport)

#endif /* GL_ARB_fragment_layer_viewport */

/* ------------------------ GL_ARB_fragment_program ------------------------ */

#ifndef GL_ARB_fragment_program
#define GL_ARB_fragment_program 1

#define GL_FRAGMENT_PROGRAM_ARB 0x8804
#define GL_PROGRAM_ALU_INSTRUCTIONS_ARB 0x8805
#define GL_PROGRAM_TEX_INSTRUCTIONS_ARB 0x8806
#define GL_PROGRAM_TEX_INDIRECTIONS_ARB 0x8807
#define GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x8808
#define GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x8809
#define GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x880A
#define GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB 0x880B
#define GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB 0x880C
#define GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB 0x880D
#define GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x880E
#define GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x880F
#define GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x8810
#define GL_MAX_TEXTURE_COORDS_ARB 0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB 0x8872

#define GLEW_ARB_fragment_program GLEW_GET_VAR(__GLEW_ARB_fragment_program)

#endif /* GL_ARB_fragment_program */

/* --------------------- GL_ARB_fragment_program_shadow -------------------- */

#ifndef GL_ARB_fragment_program_shadow
#define GL_ARB_fragment_program_shadow 1

#define GLEW_ARB_fragment_program_shadow GLEW_GET_VAR(__GLEW_ARB_fragment_program_shadow)

#endif /* GL_ARB_fragment_program_shadow */

/* ------------------------- GL_ARB_fragment_shader ------------------------ */

#ifndef GL_ARB_fragment_shader
#define GL_ARB_fragment_shader 1

#define GL_FRAGMENT_SHADER_ARB 0x8B30
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB 0x8B49
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB 0x8B8B

#define GLEW_ARB_fragment_shader GLEW_GET_VAR(__GLEW_ARB_fragment_shader)

#endif /* GL_ARB_fragment_shader */

/* -------------------- GL_ARB_fragment_shader_interlock ------------------- */

#ifndef GL_ARB_fragment_shader_interlock
#define GL_ARB_fragment_shader_interlock 1

#define GLEW_ARB_fragment_shader_interlock GLEW_GET_VAR(__GLEW_ARB_fragment_shader_interlock)

#endif /* GL_ARB_fragment_shader_interlock */

/* ------------------- GL_ARB_framebuffer_no_attachments ------------------- */

#ifndef GL_ARB_framebuffer_no_attachments
#define GL_ARB_framebuffer_no_attachments 1

#define GL_FRAMEBUFFER_DEFAULT_WIDTH 0x9310
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT 0x9311
#define GL_FRAMEBUFFER_DEFAULT_LAYERS 0x9312
#define GL_FRAMEBUFFER_DEFAULT_SAMPLES 0x9313
#define GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS 0x9314
#define GL_MAX_FRAMEBUFFER_WIDTH 0x9315
#define GL_MAX_FRAMEBUFFER_HEIGHT 0x9316
#define GL_MAX_FRAMEBUFFER_LAYERS 0x9317
#define GL_MAX_FRAMEBUFFER_SAMPLES 0x9318

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERPARAMETERIPROC) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERPARAMETERIEXTPROC) (GLuint framebuffer, GLenum pname, GLint param);

#define glFramebufferParameteri GLEW_GET_FUN(__glewFramebufferParameteri)
#define glGetFramebufferParameteriv GLEW_GET_FUN(__glewGetFramebufferParameteriv)
#define glGetNamedFramebufferParameterivEXT GLEW_GET_FUN(__glewGetNamedFramebufferParameterivEXT)
#define glNamedFramebufferParameteriEXT GLEW_GET_FUN(__glewNamedFramebufferParameteriEXT)

#define GLEW_ARB_framebuffer_no_attachments GLEW_GET_VAR(__GLEW_ARB_framebuffer_no_attachments)

#endif /* GL_ARB_framebuffer_no_attachments */

/* ----------------------- GL_ARB_framebuffer_object ----------------------- */

#ifndef GL_ARB_framebuffer_object
#define GL_ARB_framebuffer_object 1

#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT 0x8218
#define GL_FRAMEBUFFER_UNDEFINED 0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#define GL_INDEX 0x8222
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#define GL_DEPTH_STENCIL 0x84F9
#define GL_UNSIGNED_INT_24_8 0x84FA
#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_TEXTURE_STENCIL_SIZE 0x88F1
#define GL_UNSIGNED_NORMALIZED 0x8C17
#define GL_SRGB 0x8C40
#define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_RENDERBUFFER_BINDING 0x8CA7
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#define GL_RENDERBUFFER_SAMPLES 0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_RENDERBUFFER_WIDTH 0x8D42
#define GL_RENDERBUFFER_HEIGHT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#define GL_STENCIL_INDEX1 0x8D46
#define GL_STENCIL_INDEX4 0x8D47
#define GL_STENCIL_INDEX8 0x8D48
#define GL_STENCIL_INDEX16 0x8D49
#define GL_RENDERBUFFER_RED_SIZE 0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES 0x8D57

typedef void (GLAPIENTRY * PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void (GLAPIENTRY * PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLBLITFRAMEBUFFERPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef GLenum (GLAPIENTRY * PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint* framebuffers);
typedef void (GLAPIENTRY * PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE1DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE3DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURELAYERPROC) (GLenum target,GLenum attachment, GLuint texture,GLint level,GLint layer);
typedef void (GLAPIENTRY * PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint* framebuffers);
typedef void (GLAPIENTRY * PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint* renderbuffers);
typedef void (GLAPIENTRY * PFNGLGENERATEMIPMAPPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETRENDERBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISFRAMEBUFFERPROC) (GLuint framebuffer);
typedef GLboolean (GLAPIENTRY * PFNGLISRENDERBUFFERPROC) (GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

#define glBindFramebuffer GLEW_GET_FUN(__glewBindFramebuffer)
#define glBindRenderbuffer GLEW_GET_FUN(__glewBindRenderbuffer)
#define glBlitFramebuffer GLEW_GET_FUN(__glewBlitFramebuffer)
#define glCheckFramebufferStatus GLEW_GET_FUN(__glewCheckFramebufferStatus)
#define glDeleteFramebuffers GLEW_GET_FUN(__glewDeleteFramebuffers)
#define glDeleteRenderbuffers GLEW_GET_FUN(__glewDeleteRenderbuffers)
#define glFramebufferRenderbuffer GLEW_GET_FUN(__glewFramebufferRenderbuffer)
#define glFramebufferTexture1D GLEW_GET_FUN(__glewFramebufferTexture1D)
#define glFramebufferTexture2D GLEW_GET_FUN(__glewFramebufferTexture2D)
#define glFramebufferTexture3D GLEW_GET_FUN(__glewFramebufferTexture3D)
#define glFramebufferTextureLayer GLEW_GET_FUN(__glewFramebufferTextureLayer)
#define glGenFramebuffers GLEW_GET_FUN(__glewGenFramebuffers)
#define glGenRenderbuffers GLEW_GET_FUN(__glewGenRenderbuffers)
#define glGenerateMipmap GLEW_GET_FUN(__glewGenerateMipmap)
#define glGetFramebufferAttachmentParameteriv GLEW_GET_FUN(__glewGetFramebufferAttachmentParameteriv)
#define glGetRenderbufferParameteriv GLEW_GET_FUN(__glewGetRenderbufferParameteriv)
#define glIsFramebuffer GLEW_GET_FUN(__glewIsFramebuffer)
#define glIsRenderbuffer GLEW_GET_FUN(__glewIsRenderbuffer)
#define glRenderbufferStorage GLEW_GET_FUN(__glewRenderbufferStorage)
#define glRenderbufferStorageMultisample GLEW_GET_FUN(__glewRenderbufferStorageMultisample)

#define GLEW_ARB_framebuffer_object GLEW_GET_VAR(__GLEW_ARB_framebuffer_object)

#endif /* GL_ARB_framebuffer_object */

/* ------------------------ GL_ARB_framebuffer_sRGB ------------------------ */

#ifndef GL_ARB_framebuffer_sRGB
#define GL_ARB_framebuffer_sRGB 1

#define GL_FRAMEBUFFER_SRGB 0x8DB9

#define GLEW_ARB_framebuffer_sRGB GLEW_GET_VAR(__GLEW_ARB_framebuffer_sRGB)

#endif /* GL_ARB_framebuffer_sRGB */

/* ------------------------ GL_ARB_geometry_shader4 ------------------------ */

#ifndef GL_ARB_geometry_shader4
#define GL_ARB_geometry_shader4 1

#define GL_LINES_ADJACENCY_ARB 0xA
#define GL_LINE_STRIP_ADJACENCY_ARB 0xB
#define GL_TRIANGLES_ADJACENCY_ARB 0xC
#define GL_TRIANGLE_STRIP_ADJACENCY_ARB 0xD
#define GL_PROGRAM_POINT_SIZE_ARB 0x8642
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_ARB 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED_ARB 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB 0x8DA8
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB 0x8DA9
#define GL_GEOMETRY_SHADER_ARB 0x8DD9
#define GL_GEOMETRY_VERTICES_OUT_ARB 0x8DDA
#define GL_GEOMETRY_INPUT_TYPE_ARB 0x8DDB
#define GL_GEOMETRY_OUTPUT_TYPE_ARB 0x8DDC
#define GL_MAX_GEOMETRY_VARYING_COMPONENTS_ARB 0x8DDD
#define GL_MAX_VERTEX_VARYING_COMPONENTS_ARB 0x8DDE
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_ARB 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES_ARB 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_ARB 0x8DE1

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREFACEARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURELAYERARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERIARBPROC) (GLuint program, GLenum pname, GLint value);

#define glFramebufferTextureARB GLEW_GET_FUN(__glewFramebufferTextureARB)
#define glFramebufferTextureFaceARB GLEW_GET_FUN(__glewFramebufferTextureFaceARB)
#define glFramebufferTextureLayerARB GLEW_GET_FUN(__glewFramebufferTextureLayerARB)
#define glProgramParameteriARB GLEW_GET_FUN(__glewProgramParameteriARB)

#define GLEW_ARB_geometry_shader4 GLEW_GET_VAR(__GLEW_ARB_geometry_shader4)

#endif /* GL_ARB_geometry_shader4 */

/* ----------------------- GL_ARB_get_program_binary ----------------------- */

#ifndef GL_ARB_get_program_binary
#define GL_ARB_get_program_binary 1

#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT 0x8257
#define GL_PROGRAM_BINARY_LENGTH 0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS 0x87FE
#define GL_PROGRAM_BINARY_FORMATS 0x87FF

typedef void (GLAPIENTRY * PFNGLGETPROGRAMBINARYPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLenum *binaryFormat, void*binary);
typedef void (GLAPIENTRY * PFNGLPROGRAMBINARYPROC) (GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERIPROC) (GLuint program, GLenum pname, GLint value);

#define glGetProgramBinary GLEW_GET_FUN(__glewGetProgramBinary)
#define glProgramBinary GLEW_GET_FUN(__glewProgramBinary)
#define glProgramParameteri GLEW_GET_FUN(__glewProgramParameteri)

#define GLEW_ARB_get_program_binary GLEW_GET_VAR(__GLEW_ARB_get_program_binary)

#endif /* GL_ARB_get_program_binary */

/* ---------------------- GL_ARB_get_texture_sub_image --------------------- */

#ifndef GL_ARB_get_texture_sub_image
#define GL_ARB_get_texture_sub_image 1

typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels);
typedef void (GLAPIENTRY * PFNGLGETTEXTURESUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void *pixels);

#define glGetCompressedTextureSubImage GLEW_GET_FUN(__glewGetCompressedTextureSubImage)
#define glGetTextureSubImage GLEW_GET_FUN(__glewGetTextureSubImage)

#define GLEW_ARB_get_texture_sub_image GLEW_GET_VAR(__GLEW_ARB_get_texture_sub_image)

#endif /* GL_ARB_get_texture_sub_image */

/* ---------------------------- GL_ARB_gl_spirv ---------------------------- */

#ifndef GL_ARB_gl_spirv
#define GL_ARB_gl_spirv 1

#define GL_SHADER_BINARY_FORMAT_SPIR_V_ARB 0x9551
#define GL_SPIR_V_BINARY_ARB 0x9552

typedef void (GLAPIENTRY * PFNGLSPECIALIZESHADERARBPROC) (GLuint shader, const GLchar* pEntryPoint, GLuint numSpecializationConstants, const GLuint* pConstantIndex, const GLuint* pConstantValue);

#define glSpecializeShaderARB GLEW_GET_FUN(__glewSpecializeShaderARB)

#define GLEW_ARB_gl_spirv GLEW_GET_VAR(__GLEW_ARB_gl_spirv)

#endif /* GL_ARB_gl_spirv */

/* --------------------------- GL_ARB_gpu_shader5 -------------------------- */

#ifndef GL_ARB_gpu_shader5
#define GL_ARB_gpu_shader5 1

#define GL_GEOMETRY_SHADER_INVOCATIONS 0x887F
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS 0x8E5A
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET 0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET 0x8E5C
#define GL_FRAGMENT_INTERPOLATION_OFFSET_BITS 0x8E5D
#define GL_MAX_VERTEX_STREAMS 0x8E71

#define GLEW_ARB_gpu_shader5 GLEW_GET_VAR(__GLEW_ARB_gpu_shader5)

#endif /* GL_ARB_gpu_shader5 */

/* ------------------------- GL_ARB_gpu_shader_fp64 ------------------------ */

#ifndef GL_ARB_gpu_shader_fp64
#define GL_ARB_gpu_shader_fp64 1

#define GL_DOUBLE_MAT2 0x8F46
#define GL_DOUBLE_MAT3 0x8F47
#define GL_DOUBLE_MAT4 0x8F48
#define GL_DOUBLE_MAT2x3 0x8F49
#define GL_DOUBLE_MAT2x4 0x8F4A
#define GL_DOUBLE_MAT3x2 0x8F4B
#define GL_DOUBLE_MAT3x4 0x8F4C
#define GL_DOUBLE_MAT4x2 0x8F4D
#define GL_DOUBLE_MAT4x3 0x8F4E
#define GL_DOUBLE_VEC2 0x8FFC
#define GL_DOUBLE_VEC3 0x8FFD
#define GL_DOUBLE_VEC4 0x8FFE

typedef void (GLAPIENTRY * PFNGLGETUNIFORMDVPROC) (GLuint program, GLint location, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLUNIFORM1DPROC) (GLint location, GLdouble x);
typedef void (GLAPIENTRY * PFNGLUNIFORM1DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2DPROC) (GLint location, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLUNIFORM2DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3DPROC) (GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLUNIFORM3DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4DPROC) (GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLUNIFORM4DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);

#define glGetUniformdv GLEW_GET_FUN(__glewGetUniformdv)
#define glUniform1d GLEW_GET_FUN(__glewUniform1d)
#define glUniform1dv GLEW_GET_FUN(__glewUniform1dv)
#define glUniform2d GLEW_GET_FUN(__glewUniform2d)
#define glUniform2dv GLEW_GET_FUN(__glewUniform2dv)
#define glUniform3d GLEW_GET_FUN(__glewUniform3d)
#define glUniform3dv GLEW_GET_FUN(__glewUniform3dv)
#define glUniform4d GLEW_GET_FUN(__glewUniform4d)
#define glUniform4dv GLEW_GET_FUN(__glewUniform4dv)
#define glUniformMatrix2dv GLEW_GET_FUN(__glewUniformMatrix2dv)
#define glUniformMatrix2x3dv GLEW_GET_FUN(__glewUniformMatrix2x3dv)
#define glUniformMatrix2x4dv GLEW_GET_FUN(__glewUniformMatrix2x4dv)
#define glUniformMatrix3dv GLEW_GET_FUN(__glewUniformMatrix3dv)
#define glUniformMatrix3x2dv GLEW_GET_FUN(__glewUniformMatrix3x2dv)
#define glUniformMatrix3x4dv GLEW_GET_FUN(__glewUniformMatrix3x4dv)
#define glUniformMatrix4dv GLEW_GET_FUN(__glewUniformMatrix4dv)
#define glUniformMatrix4x2dv GLEW_GET_FUN(__glewUniformMatrix4x2dv)
#define glUniformMatrix4x3dv GLEW_GET_FUN(__glewUniformMatrix4x3dv)

#define GLEW_ARB_gpu_shader_fp64 GLEW_GET_VAR(__GLEW_ARB_gpu_shader_fp64)

#endif /* GL_ARB_gpu_shader_fp64 */

/* ------------------------ GL_ARB_gpu_shader_int64 ------------------------ */

#ifndef GL_ARB_gpu_shader_int64
#define GL_ARB_gpu_shader_int64 1

#define GL_INT64_ARB 0x140E
#define GL_UNSIGNED_INT64_ARB 0x140F
#define GL_INT64_VEC2_ARB 0x8FE9
#define GL_INT64_VEC3_ARB 0x8FEA
#define GL_INT64_VEC4_ARB 0x8FEB
#define GL_UNSIGNED_INT64_VEC2_ARB 0x8FF5
#define GL_UNSIGNED_INT64_VEC3_ARB 0x8FF6
#define GL_UNSIGNED_INT64_VEC4_ARB 0x8FF7

typedef void (GLAPIENTRY * PFNGLGETUNIFORMI64VARBPROC) (GLuint program, GLint location, GLint64* params);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMUI64VARBPROC) (GLuint program, GLint location, GLuint64* params);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMI64VARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLint64* params);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMUI64VARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLuint64* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1I64ARBPROC) (GLuint program, GLint location, GLint64 x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1I64VARBPROC) (GLuint program, GLint location, GLsizei count, const GLint64* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UI64ARBPROC) (GLuint program, GLint location, GLuint64 x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UI64VARBPROC) (GLuint program, GLint location, GLsizei count, const GLuint64* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2I64ARBPROC) (GLuint program, GLint location, GLint64 x, GLint64 y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2I64VARBPROC) (GLuint program, GLint location, GLsizei count, const GLint64* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UI64ARBPROC) (GLuint program, GLint location, GLuint64 x, GLuint64 y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UI64VARBPROC) (GLuint program, GLint location, GLsizei count, const GLuint64* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3I64ARBPROC) (GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3I64VARBPROC) (GLuint program, GLint location, GLsizei count, const GLint64* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UI64ARBPROC) (GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UI64VARBPROC) (GLuint program, GLint location, GLsizei count, const GLuint64* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4I64ARBPROC) (GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4I64VARBPROC) (GLuint program, GLint location, GLsizei count, const GLint64* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UI64ARBPROC) (GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UI64VARBPROC) (GLuint program, GLint location, GLsizei count, const GLuint64* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1I64ARBPROC) (GLint location, GLint64 x);
typedef void (GLAPIENTRY * PFNGLUNIFORM1I64VARBPROC) (GLint location, GLsizei count, const GLint64* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UI64ARBPROC) (GLint location, GLuint64 x);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UI64VARBPROC) (GLint location, GLsizei count, const GLuint64* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2I64ARBPROC) (GLint location, GLint64 x, GLint64 y);
typedef void (GLAPIENTRY * PFNGLUNIFORM2I64VARBPROC) (GLint location, GLsizei count, const GLint64* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UI64ARBPROC) (GLint location, GLuint64 x, GLuint64 y);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UI64VARBPROC) (GLint location, GLsizei count, const GLuint64* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3I64ARBPROC) (GLint location, GLint64 x, GLint64 y, GLint64 z);
typedef void (GLAPIENTRY * PFNGLUNIFORM3I64VARBPROC) (GLint location, GLsizei count, const GLint64* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UI64ARBPROC) (GLint location, GLuint64 x, GLuint64 y, GLuint64 z);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UI64VARBPROC) (GLint location, GLsizei count, const GLuint64* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4I64ARBPROC) (GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w);
typedef void (GLAPIENTRY * PFNGLUNIFORM4I64VARBPROC) (GLint location, GLsizei count, const GLint64* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UI64ARBPROC) (GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UI64VARBPROC) (GLint location, GLsizei count, const GLuint64* value);

#define glGetUniformi64vARB GLEW_GET_FUN(__glewGetUniformi64vARB)
#define glGetUniformui64vARB GLEW_GET_FUN(__glewGetUniformui64vARB)
#define glGetnUniformi64vARB GLEW_GET_FUN(__glewGetnUniformi64vARB)
#define glGetnUniformui64vARB GLEW_GET_FUN(__glewGetnUniformui64vARB)
#define glProgramUniform1i64ARB GLEW_GET_FUN(__glewProgramUniform1i64ARB)
#define glProgramUniform1i64vARB GLEW_GET_FUN(__glewProgramUniform1i64vARB)
#define glProgramUniform1ui64ARB GLEW_GET_FUN(__glewProgramUniform1ui64ARB)
#define glProgramUniform1ui64vARB GLEW_GET_FUN(__glewProgramUniform1ui64vARB)
#define glProgramUniform2i64ARB GLEW_GET_FUN(__glewProgramUniform2i64ARB)
#define glProgramUniform2i64vARB GLEW_GET_FUN(__glewProgramUniform2i64vARB)
#define glProgramUniform2ui64ARB GLEW_GET_FUN(__glewProgramUniform2ui64ARB)
#define glProgramUniform2ui64vARB GLEW_GET_FUN(__glewProgramUniform2ui64vARB)
#define glProgramUniform3i64ARB GLEW_GET_FUN(__glewProgramUniform3i64ARB)
#define glProgramUniform3i64vARB GLEW_GET_FUN(__glewProgramUniform3i64vARB)
#define glProgramUniform3ui64ARB GLEW_GET_FUN(__glewProgramUniform3ui64ARB)
#define glProgramUniform3ui64vARB GLEW_GET_FUN(__glewProgramUniform3ui64vARB)
#define glProgramUniform4i64ARB GLEW_GET_FUN(__glewProgramUniform4i64ARB)
#define glProgramUniform4i64vARB GLEW_GET_FUN(__glewProgramUniform4i64vARB)
#define glProgramUniform4ui64ARB GLEW_GET_FUN(__glewProgramUniform4ui64ARB)
#define glProgramUniform4ui64vARB GLEW_GET_FUN(__glewProgramUniform4ui64vARB)
#define glUniform1i64ARB GLEW_GET_FUN(__glewUniform1i64ARB)
#define glUniform1i64vARB GLEW_GET_FUN(__glewUniform1i64vARB)
#define glUniform1ui64ARB GLEW_GET_FUN(__glewUniform1ui64ARB)
#define glUniform1ui64vARB GLEW_GET_FUN(__glewUniform1ui64vARB)
#define glUniform2i64ARB GLEW_GET_FUN(__glewUniform2i64ARB)
#define glUniform2i64vARB GLEW_GET_FUN(__glewUniform2i64vARB)
#define glUniform2ui64ARB GLEW_GET_FUN(__glewUniform2ui64ARB)
#define glUniform2ui64vARB GLEW_GET_FUN(__glewUniform2ui64vARB)
#define glUniform3i64ARB GLEW_GET_FUN(__glewUniform3i64ARB)
#define glUniform3i64vARB GLEW_GET_FUN(__glewUniform3i64vARB)
#define glUniform3ui64ARB GLEW_GET_FUN(__glewUniform3ui64ARB)
#define glUniform3ui64vARB GLEW_GET_FUN(__glewUniform3ui64vARB)
#define glUniform4i64ARB GLEW_GET_FUN(__glewUniform4i64ARB)
#define glUniform4i64vARB GLEW_GET_FUN(__glewUniform4i64vARB)
#define glUniform4ui64ARB GLEW_GET_FUN(__glewUniform4ui64ARB)
#define glUniform4ui64vARB GLEW_GET_FUN(__glewUniform4ui64vARB)

#define GLEW_ARB_gpu_shader_int64 GLEW_GET_VAR(__GLEW_ARB_gpu_shader_int64)

#endif /* GL_ARB_gpu_shader_int64 */

/* ------------------------ GL_ARB_half_float_pixel ------------------------ */

#ifndef GL_ARB_half_float_pixel
#define GL_ARB_half_float_pixel 1

#define GL_HALF_FLOAT_ARB 0x140B

#define GLEW_ARB_half_float_pixel GLEW_GET_VAR(__GLEW_ARB_half_float_pixel)

#endif /* GL_ARB_half_float_pixel */

/* ------------------------ GL_ARB_half_float_vertex ----------------------- */

#ifndef GL_ARB_half_float_vertex
#define GL_ARB_half_float_vertex 1

#define GL_HALF_FLOAT 0x140B

#define GLEW_ARB_half_float_vertex GLEW_GET_VAR(__GLEW_ARB_half_float_vertex)

#endif /* GL_ARB_half_float_vertex */

/* ----------------------------- GL_ARB_imaging ---------------------------- */

#ifndef GL_ARB_imaging
#define GL_ARB_imaging 1

#define GL_CONSTANT_COLOR 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR 0x8002
#define GL_CONSTANT_ALPHA 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA 0x8004
#define GL_BLEND_COLOR 0x8005
#define GL_FUNC_ADD 0x8006
#define GL_MIN 0x8007
#define GL_MAX 0x8008
#define GL_BLEND_EQUATION 0x8009
#define GL_FUNC_SUBTRACT 0x800A
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_CONVOLUTION_1D 0x8010
#define GL_CONVOLUTION_2D 0x8011
#define GL_SEPARABLE_2D 0x8012
#define GL_CONVOLUTION_BORDER_MODE 0x8013
#define GL_CONVOLUTION_FILTER_SCALE 0x8014
#define GL_CONVOLUTION_FILTER_BIAS 0x8015
#define GL_REDUCE 0x8016
#define GL_CONVOLUTION_FORMAT 0x8017
#define GL_CONVOLUTION_WIDTH 0x8018
#define GL_CONVOLUTION_HEIGHT 0x8019
#define GL_MAX_CONVOLUTION_WIDTH 0x801A
#define GL_MAX_CONVOLUTION_HEIGHT 0x801B
#define GL_POST_CONVOLUTION_RED_SCALE 0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE 0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE 0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE 0x801F
#define GL_POST_CONVOLUTION_RED_BIAS 0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS 0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS 0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS 0x8023
#define GL_HISTOGRAM 0x8024
#define GL_PROXY_HISTOGRAM 0x8025
#define GL_HISTOGRAM_WIDTH 0x8026
#define GL_HISTOGRAM_FORMAT 0x8027
#define GL_HISTOGRAM_RED_SIZE 0x8028
#define GL_HISTOGRAM_GREEN_SIZE 0x8029
#define GL_HISTOGRAM_BLUE_SIZE 0x802A
#define GL_HISTOGRAM_ALPHA_SIZE 0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE 0x802C
#define GL_HISTOGRAM_SINK 0x802D
#define GL_MINMAX 0x802E
#define GL_MINMAX_FORMAT 0x802F
#define GL_MINMAX_SINK 0x8030
#define GL_TABLE_TOO_LARGE 0x8031
#define GL_COLOR_MATRIX 0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH 0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH 0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE 0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE 0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE 0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE 0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS 0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS 0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS 0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS 0x80BB
#define GL_COLOR_TABLE 0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE 0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE 0x80D2
#define GL_PROXY_COLOR_TABLE 0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE 0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE 0x80D5
#define GL_COLOR_TABLE_SCALE 0x80D6
#define GL_COLOR_TABLE_BIAS 0x80D7
#define GL_COLOR_TABLE_FORMAT 0x80D8
#define GL_COLOR_TABLE_WIDTH 0x80D9
#define GL_COLOR_TABLE_RED_SIZE 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE 0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE 0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE 0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE 0x80DF
#define GL_IGNORE_BORDER 0x8150
#define GL_CONSTANT_BORDER 0x8151
#define GL_WRAP_BORDER 0x8152
#define GL_REPLICATE_BORDER 0x8153
#define GL_CONVOLUTION_BORDER_COLOR 0x8154

typedef void (GLAPIENTRY * PFNGLCOLORSUBTABLEPROC) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void *data);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *table);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPARAMETERFVPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPARAMETERIVPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER1DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFPROC) (GLenum target, GLenum pname, GLfloat params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFVPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIPROC) (GLenum target, GLenum pname, GLint params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIVPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLCOPYCOLORSUBTABLEPROC) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCOLORTABLEPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER1DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER2DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPROC) (GLenum target, GLenum format, GLenum type, void *table);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONFILTERPROC) (GLenum target, GLenum format, GLenum type, void *image);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, void *values);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPROC) (GLenum target, GLboolean reset, GLenum format, GLenum types, void *values);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETSEPARABLEFILTERPROC) (GLenum target, GLenum format, GLenum type, void *row, void *column, void *span);
typedef void (GLAPIENTRY * PFNGLHISTOGRAMPROC) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLMINMAXPROC) (GLenum target, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLRESETHISTOGRAMPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLRESETMINMAXPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLSEPARABLEFILTER2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *row, const void *column);

#define glColorSubTable GLEW_GET_FUN(__glewColorSubTable)
#define glColorTable GLEW_GET_FUN(__glewColorTable)
#define glColorTableParameterfv GLEW_GET_FUN(__glewColorTableParameterfv)
#define glColorTableParameteriv GLEW_GET_FUN(__glewColorTableParameteriv)
#define glConvolutionFilter1D GLEW_GET_FUN(__glewConvolutionFilter1D)
#define glConvolutionFilter2D GLEW_GET_FUN(__glewConvolutionFilter2D)
#define glConvolutionParameterf GLEW_GET_FUN(__glewConvolutionParameterf)
#define glConvolutionParameterfv GLEW_GET_FUN(__glewConvolutionParameterfv)
#define glConvolutionParameteri GLEW_GET_FUN(__glewConvolutionParameteri)
#define glConvolutionParameteriv GLEW_GET_FUN(__glewConvolutionParameteriv)
#define glCopyColorSubTable GLEW_GET_FUN(__glewCopyColorSubTable)
#define glCopyColorTable GLEW_GET_FUN(__glewCopyColorTable)
#define glCopyConvolutionFilter1D GLEW_GET_FUN(__glewCopyConvolutionFilter1D)
#define glCopyConvolutionFilter2D GLEW_GET_FUN(__glewCopyConvolutionFilter2D)
#define glGetColorTable GLEW_GET_FUN(__glewGetColorTable)
#define glGetColorTableParameterfv GLEW_GET_FUN(__glewGetColorTableParameterfv)
#define glGetColorTableParameteriv GLEW_GET_FUN(__glewGetColorTableParameteriv)
#define glGetConvolutionFilter GLEW_GET_FUN(__glewGetConvolutionFilter)
#define glGetConvolutionParameterfv GLEW_GET_FUN(__glewGetConvolutionParameterfv)
#define glGetConvolutionParameteriv GLEW_GET_FUN(__glewGetConvolutionParameteriv)
#define glGetHistogram GLEW_GET_FUN(__glewGetHistogram)
#define glGetHistogramParameterfv GLEW_GET_FUN(__glewGetHistogramParameterfv)
#define glGetHistogramParameteriv GLEW_GET_FUN(__glewGetHistogramParameteriv)
#define glGetMinmax GLEW_GET_FUN(__glewGetMinmax)
#define glGetMinmaxParameterfv GLEW_GET_FUN(__glewGetMinmaxParameterfv)
#define glGetMinmaxParameteriv GLEW_GET_FUN(__glewGetMinmaxParameteriv)
#define glGetSeparableFilter GLEW_GET_FUN(__glewGetSeparableFilter)
#define glHistogram GLEW_GET_FUN(__glewHistogram)
#define glMinmax GLEW_GET_FUN(__glewMinmax)
#define glResetHistogram GLEW_GET_FUN(__glewResetHistogram)
#define glResetMinmax GLEW_GET_FUN(__glewResetMinmax)
#define glSeparableFilter2D GLEW_GET_FUN(__glewSeparableFilter2D)

#define GLEW_ARB_imaging GLEW_GET_VAR(__GLEW_ARB_imaging)

#endif /* GL_ARB_imaging */

/* ----------------------- GL_ARB_indirect_parameters ---------------------- */

#ifndef GL_ARB_indirect_parameters
#define GL_ARB_indirect_parameters 1

#define GL_PARAMETER_BUFFER_ARB 0x80EE
#define GL_PARAMETER_BUFFER_BINDING_ARB 0x80EF

typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC) (GLenum mode, const void *indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC) (GLenum mode, GLenum type, const void *indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);

#define glMultiDrawArraysIndirectCountARB GLEW_GET_FUN(__glewMultiDrawArraysIndirectCountARB)
#define glMultiDrawElementsIndirectCountARB GLEW_GET_FUN(__glewMultiDrawElementsIndirectCountARB)

#define GLEW_ARB_indirect_parameters GLEW_GET_VAR(__GLEW_ARB_indirect_parameters)

#endif /* GL_ARB_indirect_parameters */

/* ------------------------ GL_ARB_instanced_arrays ------------------------ */

#ifndef GL_ARB_instanced_arrays
#define GL_ARB_instanced_arrays 1

#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ARB 0x88FE

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDARBPROC) (GLenum mode, GLint first, GLsizei count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDARBPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBDIVISORARBPROC) (GLuint index, GLuint divisor);

#define glDrawArraysInstancedARB GLEW_GET_FUN(__glewDrawArraysInstancedARB)
#define glDrawElementsInstancedARB GLEW_GET_FUN(__glewDrawElementsInstancedARB)
#define glVertexAttribDivisorARB GLEW_GET_FUN(__glewVertexAttribDivisorARB)

#define GLEW_ARB_instanced_arrays GLEW_GET_VAR(__GLEW_ARB_instanced_arrays)

#endif /* GL_ARB_instanced_arrays */

/* ---------------------- GL_ARB_internalformat_query ---------------------- */

#ifndef GL_ARB_internalformat_query
#define GL_ARB_internalformat_query 1

#define GL_NUM_SAMPLE_COUNTS 0x9380

typedef void (GLAPIENTRY * PFNGLGETINTERNALFORMATIVPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params);

#define glGetInternalformativ GLEW_GET_FUN(__glewGetInternalformativ)

#define GLEW_ARB_internalformat_query GLEW_GET_VAR(__GLEW_ARB_internalformat_query)

#endif /* GL_ARB_internalformat_query */

/* ---------------------- GL_ARB_internalformat_query2 --------------------- */

#ifndef GL_ARB_internalformat_query2
#define GL_ARB_internalformat_query2 1

#define GL_INTERNALFORMAT_SUPPORTED 0x826F
#define GL_INTERNALFORMAT_PREFERRED 0x8270
#define GL_INTERNALFORMAT_RED_SIZE 0x8271
#define GL_INTERNALFORMAT_GREEN_SIZE 0x8272
#define GL_INTERNALFORMAT_BLUE_SIZE 0x8273
#define GL_INTERNALFORMAT_ALPHA_SIZE 0x8274
#define GL_INTERNALFORMAT_DEPTH_SIZE 0x8275
#define GL_INTERNALFORMAT_STENCIL_SIZE 0x8276
#define GL_INTERNALFORMAT_SHARED_SIZE 0x8277
#define GL_INTERNALFORMAT_RED_TYPE 0x8278
#define GL_INTERNALFORMAT_GREEN_TYPE 0x8279
#define GL_INTERNALFORMAT_BLUE_TYPE 0x827A
#define GL_INTERNALFORMAT_ALPHA_TYPE 0x827B
#define GL_INTERNALFORMAT_DEPTH_TYPE 0x827C
#define GL_INTERNALFORMAT_STENCIL_TYPE 0x827D
#define GL_MAX_WIDTH 0x827E
#define GL_MAX_HEIGHT 0x827F
#define GL_MAX_DEPTH 0x8280
#define GL_MAX_LAYERS 0x8281
#define GL_MAX_COMBINED_DIMENSIONS 0x8282
#define GL_COLOR_COMPONENTS 0x8283
#define GL_DEPTH_COMPONENTS 0x8284
#define GL_STENCIL_COMPONENTS 0x8285
#define GL_COLOR_RENDERABLE 0x8286
#define GL_DEPTH_RENDERABLE 0x8287
#define GL_STENCIL_RENDERABLE 0x8288
#define GL_FRAMEBUFFER_RENDERABLE 0x8289
#define GL_FRAMEBUFFER_RENDERABLE_LAYERED 0x828A
#define GL_FRAMEBUFFER_BLEND 0x828B
#define GL_READ_PIXELS 0x828C
#define GL_READ_PIXELS_FORMAT 0x828D
#define GL_READ_PIXELS_TYPE 0x828E
#define GL_TEXTURE_IMAGE_FORMAT 0x828F
#define GL_TEXTURE_IMAGE_TYPE 0x8290
#define GL_GET_TEXTURE_IMAGE_FORMAT 0x8291
#define GL_GET_TEXTURE_IMAGE_TYPE 0x8292
#define GL_MIPMAP 0x8293
#define GL_MANUAL_GENERATE_MIPMAP 0x8294
#define GL_AUTO_GENERATE_MIPMAP 0x8295
#define GL_COLOR_ENCODING 0x8296
#define GL_SRGB_READ 0x8297
#define GL_SRGB_WRITE 0x8298
#define GL_SRGB_DECODE_ARB 0x8299
#define GL_FILTER 0x829A
#define GL_VERTEX_TEXTURE 0x829B
#define GL_TESS_CONTROL_TEXTURE 0x829C
#define GL_TESS_EVALUATION_TEXTURE 0x829D
#define GL_GEOMETRY_TEXTURE 0x829E
#define GL_FRAGMENT_TEXTURE 0x829F
#define GL_COMPUTE_TEXTURE 0x82A0
#define GL_TEXTURE_SHADOW 0x82A1
#define GL_TEXTURE_GATHER 0x82A2
#define GL_TEXTURE_GATHER_SHADOW 0x82A3
#define GL_SHADER_IMAGE_LOAD 0x82A4
#define GL_SHADER_IMAGE_STORE 0x82A5
#define GL_SHADER_IMAGE_ATOMIC 0x82A6
#define GL_IMAGE_TEXEL_SIZE 0x82A7
#define GL_IMAGE_COMPATIBILITY_CLASS 0x82A8
#define GL_IMAGE_PIXEL_FORMAT 0x82A9
#define GL_IMAGE_PIXEL_TYPE 0x82AA
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST 0x82AC
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST 0x82AD
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE 0x82AE
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE 0x82AF
#define GL_TEXTURE_COMPRESSED_BLOCK_WIDTH 0x82B1
#define GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT 0x82B2
#define GL_TEXTURE_COMPRESSED_BLOCK_SIZE 0x82B3
#define GL_CLEAR_BUFFER 0x82B4
#define GL_TEXTURE_VIEW 0x82B5
#define GL_VIEW_COMPATIBILITY_CLASS 0x82B6
#define GL_FULL_SUPPORT 0x82B7
#define GL_CAVEAT_SUPPORT 0x82B8
#define GL_IMAGE_CLASS_4_X_32 0x82B9
#define GL_IMAGE_CLASS_2_X_32 0x82BA
#define GL_IMAGE_CLASS_1_X_32 0x82BB
#define GL_IMAGE_CLASS_4_X_16 0x82BC
#define GL_IMAGE_CLASS_2_X_16 0x82BD
#define GL_IMAGE_CLASS_1_X_16 0x82BE
#define GL_IMAGE_CLASS_4_X_8 0x82BF
#define GL_IMAGE_CLASS_2_X_8 0x82C0
#define GL_IMAGE_CLASS_1_X_8 0x82C1
#define GL_IMAGE_CLASS_11_11_10 0x82C2
#define GL_IMAGE_CLASS_10_10_10_2 0x82C3
#define GL_VIEW_CLASS_128_BITS 0x82C4
#define GL_VIEW_CLASS_96_BITS 0x82C5
#define GL_VIEW_CLASS_64_BITS 0x82C6
#define GL_VIEW_CLASS_48_BITS 0x82C7
#define GL_VIEW_CLASS_32_BITS 0x82C8
#define GL_VIEW_CLASS_24_BITS 0x82C9
#define GL_VIEW_CLASS_16_BITS 0x82CA
#define GL_VIEW_CLASS_8_BITS 0x82CB
#define GL_VIEW_CLASS_S3TC_DXT1_RGB 0x82CC
#define GL_VIEW_CLASS_S3TC_DXT1_RGBA 0x82CD
#define GL_VIEW_CLASS_S3TC_DXT3_RGBA 0x82CE
#define GL_VIEW_CLASS_S3TC_DXT5_RGBA 0x82CF
#define GL_VIEW_CLASS_RGTC1_RED 0x82D0
#define GL_VIEW_CLASS_RGTC2_RG 0x82D1
#define GL_VIEW_CLASS_BPTC_UNORM 0x82D2
#define GL_VIEW_CLASS_BPTC_FLOAT 0x82D3

typedef void (GLAPIENTRY * PFNGLGETINTERNALFORMATI64VPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64* params);

#define glGetInternalformati64v GLEW_GET_FUN(__glewGetInternalformati64v)

#define GLEW_ARB_internalformat_query2 GLEW_GET_VAR(__GLEW_ARB_internalformat_query2)

#endif /* GL_ARB_internalformat_query2 */

/* ----------------------- GL_ARB_invalidate_subdata ----------------------- */

#ifndef GL_ARB_invalidate_subdata
#define GL_ARB_invalidate_subdata 1

typedef void (GLAPIENTRY * PFNGLINVALIDATEBUFFERDATAPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLINVALIDATEBUFFERSUBDATAPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void (GLAPIENTRY * PFNGLINVALIDATEFRAMEBUFFERPROC) (GLenum target, GLsizei numAttachments, const GLenum* attachments);
typedef void (GLAPIENTRY * PFNGLINVALIDATESUBFRAMEBUFFERPROC) (GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLINVALIDATETEXIMAGEPROC) (GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLINVALIDATETEXSUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);

#define glInvalidateBufferData GLEW_GET_FUN(__glewInvalidateBufferData)
#define glInvalidateBufferSubData GLEW_GET_FUN(__glewInvalidateBufferSubData)
#define glInvalidateFramebuffer GLEW_GET_FUN(__glewInvalidateFramebuffer)
#define glInvalidateSubFramebuffer GLEW_GET_FUN(__glewInvalidateSubFramebuffer)
#define glInvalidateTexImage GLEW_GET_FUN(__glewInvalidateTexImage)
#define glInvalidateTexSubImage GLEW_GET_FUN(__glewInvalidateTexSubImage)

#define GLEW_ARB_invalidate_subdata GLEW_GET_VAR(__GLEW_ARB_invalidate_subdata)

#endif /* GL_ARB_invalidate_subdata */

/* ---------------------- GL_ARB_map_buffer_alignment ---------------------- */

#ifndef GL_ARB_map_buffer_alignment
#define GL_ARB_map_buffer_alignment 1

#define GL_MIN_MAP_BUFFER_ALIGNMENT 0x90BC

#define GLEW_ARB_map_buffer_alignment GLEW_GET_VAR(__GLEW_ARB_map_buffer_alignment)

#endif /* GL_ARB_map_buffer_alignment */

/* ------------------------ GL_ARB_map_buffer_range ------------------------ */

#ifndef GL_ARB_map_buffer_range
#define GL_ARB_map_buffer_range 1

#define GL_MAP_READ_BIT 0x0001
#define GL_MAP_WRITE_BIT 0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT 0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT 0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT 0x0020

typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length);
typedef void * (GLAPIENTRY * PFNGLMAPBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);

#define glFlushMappedBufferRange GLEW_GET_FUN(__glewFlushMappedBufferRange)
#define glMapBufferRange GLEW_GET_FUN(__glewMapBufferRange)

#define GLEW_ARB_map_buffer_range GLEW_GET_VAR(__GLEW_ARB_map_buffer_range)

#endif /* GL_ARB_map_buffer_range */

/* ------------------------- GL_ARB_matrix_palette ------------------------- */

#ifndef GL_ARB_matrix_palette
#define GL_ARB_matrix_palette 1

#define GL_MATRIX_PALETTE_ARB 0x8840
#define GL_MAX_MATRIX_PALETTE_STACK_DEPTH_ARB 0x8841
#define GL_MAX_PALETTE_MATRICES_ARB 0x8842
#define GL_CURRENT_PALETTE_MATRIX_ARB 0x8843
#define GL_MATRIX_INDEX_ARRAY_ARB 0x8844
#define GL_CURRENT_MATRIX_INDEX_ARB 0x8845
#define GL_MATRIX_INDEX_ARRAY_SIZE_ARB 0x8846
#define GL_MATRIX_INDEX_ARRAY_TYPE_ARB 0x8847
#define GL_MATRIX_INDEX_ARRAY_STRIDE_ARB 0x8848
#define GL_MATRIX_INDEX_ARRAY_POINTER_ARB 0x8849

typedef void (GLAPIENTRY * PFNGLCURRENTPALETTEMATRIXARBPROC) (GLint index);
typedef void (GLAPIENTRY * PFNGLMATRIXINDEXPOINTERARBPROC) (GLint size, GLenum type, GLsizei stride, void *pointer);
typedef void (GLAPIENTRY * PFNGLMATRIXINDEXUBVARBPROC) (GLint size, GLubyte *indices);
typedef void (GLAPIENTRY * PFNGLMATRIXINDEXUIVARBPROC) (GLint size, GLuint *indices);
typedef void (GLAPIENTRY * PFNGLMATRIXINDEXUSVARBPROC) (GLint size, GLushort *indices);

#define glCurrentPaletteMatrixARB GLEW_GET_FUN(__glewCurrentPaletteMatrixARB)
#define glMatrixIndexPointerARB GLEW_GET_FUN(__glewMatrixIndexPointerARB)
#define glMatrixIndexubvARB GLEW_GET_FUN(__glewMatrixIndexubvARB)
#define glMatrixIndexuivARB GLEW_GET_FUN(__glewMatrixIndexuivARB)
#define glMatrixIndexusvARB GLEW_GET_FUN(__glewMatrixIndexusvARB)

#define GLEW_ARB_matrix_palette GLEW_GET_VAR(__GLEW_ARB_matrix_palette)

#endif /* GL_ARB_matrix_palette */

/* --------------------------- GL_ARB_multi_bind --------------------------- */

#ifndef GL_ARB_multi_bind
#define GL_ARB_multi_bind 1

typedef void (GLAPIENTRY * PFNGLBINDBUFFERSBASEPROC) (GLenum target, GLuint first, GLsizei count, const GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERSRANGEPROC) (GLenum target, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr *offsets, const GLsizeiptr *sizes);
typedef void (GLAPIENTRY * PFNGLBINDIMAGETEXTURESPROC) (GLuint first, GLsizei count, const GLuint* textures);
typedef void (GLAPIENTRY * PFNGLBINDSAMPLERSPROC) (GLuint first, GLsizei count, const GLuint* samplers);
typedef void (GLAPIENTRY * PFNGLBINDTEXTURESPROC) (GLuint first, GLsizei count, const GLuint* textures);
typedef void (GLAPIENTRY * PFNGLBINDVERTEXBUFFERSPROC) (GLuint first, GLsizei count, const GLuint* buffers, const GLintptr *offsets, const GLsizei *strides);

#define glBindBuffersBase GLEW_GET_FUN(__glewBindBuffersBase)
#define glBindBuffersRange GLEW_GET_FUN(__glewBindBuffersRange)
#define glBindImageTextures GLEW_GET_FUN(__glewBindImageTextures)
#define glBindSamplers GLEW_GET_FUN(__glewBindSamplers)
#define glBindTextures GLEW_GET_FUN(__glewBindTextures)
#define glBindVertexBuffers GLEW_GET_FUN(__glewBindVertexBuffers)

#define GLEW_ARB_multi_bind GLEW_GET_VAR(__GLEW_ARB_multi_bind)

#endif /* GL_ARB_multi_bind */

/* ----------------------- GL_ARB_multi_draw_indirect ---------------------- */

#ifndef GL_ARB_multi_draw_indirect
#define GL_ARB_multi_draw_indirect 1

typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSINDIRECTPROC) (GLenum mode, const void *indirect, GLsizei primcount, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSINDIRECTPROC) (GLenum mode, GLenum type, const void *indirect, GLsizei primcount, GLsizei stride);

#define glMultiDrawArraysIndirect GLEW_GET_FUN(__glewMultiDrawArraysIndirect)
#define glMultiDrawElementsIndirect GLEW_GET_FUN(__glewMultiDrawElementsIndirect)

#define GLEW_ARB_multi_draw_indirect GLEW_GET_VAR(__GLEW_ARB_multi_draw_indirect)

#endif /* GL_ARB_multi_draw_indirect */

/* --------------------------- GL_ARB_multisample -------------------------- */

#ifndef GL_ARB_multisample
#define GL_ARB_multisample 1

#define GL_MULTISAMPLE_ARB 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE_ARB 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_ARB 0x809F
#define GL_SAMPLE_COVERAGE_ARB 0x80A0
#define GL_SAMPLE_BUFFERS_ARB 0x80A8
#define GL_SAMPLES_ARB 0x80A9
#define GL_SAMPLE_COVERAGE_VALUE_ARB 0x80AA
#define GL_SAMPLE_COVERAGE_INVERT_ARB 0x80AB
#define GL_MULTISAMPLE_BIT_ARB 0x20000000

typedef void (GLAPIENTRY * PFNGLSAMPLECOVERAGEARBPROC) (GLclampf value, GLboolean invert);

#define glSampleCoverageARB GLEW_GET_FUN(__glewSampleCoverageARB)

#define GLEW_ARB_multisample GLEW_GET_VAR(__GLEW_ARB_multisample)

#endif /* GL_ARB_multisample */

/* -------------------------- GL_ARB_multitexture -------------------------- */

#ifndef GL_ARB_multitexture
#define GL_ARB_multitexture 1

#define GL_TEXTURE0_ARB 0x84C0
#define GL_TEXTURE1_ARB 0x84C1
#define GL_TEXTURE2_ARB 0x84C2
#define GL_TEXTURE3_ARB 0x84C3
#define GL_TEXTURE4_ARB 0x84C4
#define GL_TEXTURE5_ARB 0x84C5
#define GL_TEXTURE6_ARB 0x84C6
#define GL_TEXTURE7_ARB 0x84C7
#define GL_TEXTURE8_ARB 0x84C8
#define GL_TEXTURE9_ARB 0x84C9
#define GL_TEXTURE10_ARB 0x84CA
#define GL_TEXTURE11_ARB 0x84CB
#define GL_TEXTURE12_ARB 0x84CC
#define GL_TEXTURE13_ARB 0x84CD
#define GL_TEXTURE14_ARB 0x84CE
#define GL_TEXTURE15_ARB 0x84CF
#define GL_TEXTURE16_ARB 0x84D0
#define GL_TEXTURE17_ARB 0x84D1
#define GL_TEXTURE18_ARB 0x84D2
#define GL_TEXTURE19_ARB 0x84D3
#define GL_TEXTURE20_ARB 0x84D4
#define GL_TEXTURE21_ARB 0x84D5
#define GL_TEXTURE22_ARB 0x84D6
#define GL_TEXTURE23_ARB 0x84D7
#define GL_TEXTURE24_ARB 0x84D8
#define GL_TEXTURE25_ARB 0x84D9
#define GL_TEXTURE26_ARB 0x84DA
#define GL_TEXTURE27_ARB 0x84DB
#define GL_TEXTURE28_ARB 0x84DC
#define GL_TEXTURE29_ARB 0x84DD
#define GL_TEXTURE30_ARB 0x84DE
#define GL_TEXTURE31_ARB 0x84DF
#define GL_ACTIVE_TEXTURE_ARB 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB 0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB 0x84E2

typedef void (GLAPIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DARBPROC) (GLenum target, GLdouble s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FARBPROC) (GLenum target, GLfloat s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IARBPROC) (GLenum target, GLint s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IVARBPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SARBPROC) (GLenum target, GLshort s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SVARBPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DARBPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IARBPROC) (GLenum target, GLint s, GLint t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IVARBPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SARBPROC) (GLenum target, GLshort s, GLshort t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SVARBPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IARBPROC) (GLenum target, GLint s, GLint t, GLint r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IVARBPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SVARBPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IARBPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IVARBPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SVARBPROC) (GLenum target, const GLshort *v);

#define glActiveTextureARB GLEW_GET_FUN(__glewActiveTextureARB)
#define glClientActiveTextureARB GLEW_GET_FUN(__glewClientActiveTextureARB)
#define glMultiTexCoord1dARB GLEW_GET_FUN(__glewMultiTexCoord1dARB)
#define glMultiTexCoord1dvARB GLEW_GET_FUN(__glewMultiTexCoord1dvARB)
#define glMultiTexCoord1fARB GLEW_GET_FUN(__glewMultiTexCoord1fARB)
#define glMultiTexCoord1fvARB GLEW_GET_FUN(__glewMultiTexCoord1fvARB)
#define glMultiTexCoord1iARB GLEW_GET_FUN(__glewMultiTexCoord1iARB)
#define glMultiTexCoord1ivARB GLEW_GET_FUN(__glewMultiTexCoord1ivARB)
#define glMultiTexCoord1sARB GLEW_GET_FUN(__glewMultiTexCoord1sARB)
#define glMultiTexCoord1svARB GLEW_GET_FUN(__glewMultiTexCoord1svARB)
#define glMultiTexCoord2dARB GLEW_GET_FUN(__glewMultiTexCoord2dARB)
#define glMultiTexCoord2dvARB GLEW_GET_FUN(__glewMultiTexCoord2dvARB)
#define glMultiTexCoord2fARB GLEW_GET_FUN(__glewMultiTexCoord2fARB)
#define glMultiTexCoord2fvARB GLEW_GET_FUN(__glewMultiTexCoord2fvARB)
#define glMultiTexCoord2iARB GLEW_GET_FUN(__glewMultiTexCoord2iARB)
#define glMultiTexCoord2ivARB GLEW_GET_FUN(__glewMultiTexCoord2ivARB)
#define glMultiTexCoord2sARB GLEW_GET_FUN(__glewMultiTexCoord2sARB)
#define glMultiTexCoord2svARB GLEW_GET_FUN(__glewMultiTexCoord2svARB)
#define glMultiTexCoord3dARB GLEW_GET_FUN(__glewMultiTexCoord3dARB)
#define glMultiTexCoord3dvARB GLEW_GET_FUN(__glewMultiTexCoord3dvARB)
#define glMultiTexCoord3fARB GLEW_GET_FUN(__glewMultiTexCoord3fARB)
#define glMultiTexCoord3fvARB GLEW_GET_FUN(__glewMultiTexCoord3fvARB)
#define glMultiTexCoord3iARB GLEW_GET_FUN(__glewMultiTexCoord3iARB)
#define glMultiTexCoord3ivARB GLEW_GET_FUN(__glewMultiTexCoord3ivARB)
#define glMultiTexCoord3sARB GLEW_GET_FUN(__glewMultiTexCoord3sARB)
#define glMultiTexCoord3svARB GLEW_GET_FUN(__glewMultiTexCoord3svARB)
#define glMultiTexCoord4dARB GLEW_GET_FUN(__glewMultiTexCoord4dARB)
#define glMultiTexCoord4dvARB GLEW_GET_FUN(__glewMultiTexCoord4dvARB)
#define glMultiTexCoord4fARB GLEW_GET_FUN(__glewMultiTexCoord4fARB)
#define glMultiTexCoord4fvARB GLEW_GET_FUN(__glewMultiTexCoord4fvARB)
#define glMultiTexCoord4iARB GLEW_GET_FUN(__glewMultiTexCoord4iARB)
#define glMultiTexCoord4ivARB GLEW_GET_FUN(__glewMultiTexCoord4ivARB)
#define glMultiTexCoord4sARB GLEW_GET_FUN(__glewMultiTexCoord4sARB)
#define glMultiTexCoord4svARB GLEW_GET_FUN(__glewMultiTexCoord4svARB)

#define GLEW_ARB_multitexture GLEW_GET_VAR(__GLEW_ARB_multitexture)

#endif /* GL_ARB_multitexture */

/* ------------------------- GL_ARB_occlusion_query ------------------------ */

#ifndef GL_ARB_occlusion_query
#define GL_ARB_occlusion_query 1

#define GL_QUERY_COUNTER_BITS_ARB 0x8864
#define GL_CURRENT_QUERY_ARB 0x8865
#define GL_QUERY_RESULT_ARB 0x8866
#define GL_QUERY_RESULT_AVAILABLE_ARB 0x8867
#define GL_SAMPLES_PASSED_ARB 0x8914

typedef void (GLAPIENTRY * PFNGLBEGINQUERYARBPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETEQUERIESARBPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLENDQUERYARBPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGENQUERIESARBPROC) (GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTIVARBPROC) (GLuint id, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUIVARBPROC) (GLuint id, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYIVARBPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISQUERYARBPROC) (GLuint id);

#define glBeginQueryARB GLEW_GET_FUN(__glewBeginQueryARB)
#define glDeleteQueriesARB GLEW_GET_FUN(__glewDeleteQueriesARB)
#define glEndQueryARB GLEW_GET_FUN(__glewEndQueryARB)
#define glGenQueriesARB GLEW_GET_FUN(__glewGenQueriesARB)
#define glGetQueryObjectivARB GLEW_GET_FUN(__glewGetQueryObjectivARB)
#define glGetQueryObjectuivARB GLEW_GET_FUN(__glewGetQueryObjectuivARB)
#define glGetQueryivARB GLEW_GET_FUN(__glewGetQueryivARB)
#define glIsQueryARB GLEW_GET_FUN(__glewIsQueryARB)

#define GLEW_ARB_occlusion_query GLEW_GET_VAR(__GLEW_ARB_occlusion_query)

#endif /* GL_ARB_occlusion_query */

/* ------------------------ GL_ARB_occlusion_query2 ------------------------ */

#ifndef GL_ARB_occlusion_query2
#define GL_ARB_occlusion_query2 1

#define GL_ANY_SAMPLES_PASSED 0x8C2F

#define GLEW_ARB_occlusion_query2 GLEW_GET_VAR(__GLEW_ARB_occlusion_query2)

#endif /* GL_ARB_occlusion_query2 */

/* --------------------- GL_ARB_parallel_shader_compile -------------------- */

#ifndef GL_ARB_parallel_shader_compile
#define GL_ARB_parallel_shader_compile 1

#define GL_MAX_SHADER_COMPILER_THREADS_ARB 0x91B0
#define GL_COMPLETION_STATUS_ARB 0x91B1

typedef void (GLAPIENTRY * PFNGLMAXSHADERCOMPILERTHREADSARBPROC) (GLuint count);

#define glMaxShaderCompilerThreadsARB GLEW_GET_FUN(__glewMaxShaderCompilerThreadsARB)

#define GLEW_ARB_parallel_shader_compile GLEW_GET_VAR(__GLEW_ARB_parallel_shader_compile)

#endif /* GL_ARB_parallel_shader_compile */

/* -------------------- GL_ARB_pipeline_statistics_query ------------------- */

#ifndef GL_ARB_pipeline_statistics_query
#define GL_ARB_pipeline_statistics_query 1

#define GL_VERTICES_SUBMITTED_ARB 0x82EE
#define GL_PRIMITIVES_SUBMITTED_ARB 0x82EF
#define GL_VERTEX_SHADER_INVOCATIONS_ARB 0x82F0
#define GL_TESS_CONTROL_SHADER_PATCHES_ARB 0x82F1
#define GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB 0x82F2
#define GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB 0x82F3
#define GL_FRAGMENT_SHADER_INVOCATIONS_ARB 0x82F4
#define GL_COMPUTE_SHADER_INVOCATIONS_ARB 0x82F5
#define GL_CLIPPING_INPUT_PRIMITIVES_ARB 0x82F6
#define GL_CLIPPING_OUTPUT_PRIMITIVES_ARB 0x82F7
#define GL_GEOMETRY_SHADER_INVOCATIONS 0x887F

#define GLEW_ARB_pipeline_statistics_query GLEW_GET_VAR(__GLEW_ARB_pipeline_statistics_query)

#endif /* GL_ARB_pipeline_statistics_query */

/* ----------------------- GL_ARB_pixel_buffer_object ---------------------- */

#ifndef GL_ARB_pixel_buffer_object
#define GL_ARB_pixel_buffer_object 1

#define GL_PIXEL_PACK_BUFFER_ARB 0x88EB
#define GL_PIXEL_UNPACK_BUFFER_ARB 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_ARB 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_ARB 0x88EF

#define GLEW_ARB_pixel_buffer_object GLEW_GET_VAR(__GLEW_ARB_pixel_buffer_object)

#endif /* GL_ARB_pixel_buffer_object */

/* ------------------------ GL_ARB_point_parameters ------------------------ */

#ifndef GL_ARB_point_parameters
#define GL_ARB_point_parameters 1

#define GL_POINT_SIZE_MIN_ARB 0x8126
#define GL_POINT_SIZE_MAX_ARB 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_ARB 0x8128
#define GL_POINT_DISTANCE_ATTENUATION_ARB 0x8129

typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFARBPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFVARBPROC) (GLenum pname, const GLfloat* params);

#define glPointParameterfARB GLEW_GET_FUN(__glewPointParameterfARB)
#define glPointParameterfvARB GLEW_GET_FUN(__glewPointParameterfvARB)

#define GLEW_ARB_point_parameters GLEW_GET_VAR(__GLEW_ARB_point_parameters)

#endif /* GL_ARB_point_parameters */

/* -------------------------- GL_ARB_point_sprite -------------------------- */

#ifndef GL_ARB_point_sprite
#define GL_ARB_point_sprite 1

#define GL_POINT_SPRITE_ARB 0x8861
#define GL_COORD_REPLACE_ARB 0x8862

#define GLEW_ARB_point_sprite GLEW_GET_VAR(__GLEW_ARB_point_sprite)

#endif /* GL_ARB_point_sprite */

/* ---------------------- GL_ARB_polygon_offset_clamp ---------------------- */

#ifndef GL_ARB_polygon_offset_clamp
#define GL_ARB_polygon_offset_clamp 1

#define GL_POLYGON_OFFSET_CLAMP 0x8E1B

typedef void (GLAPIENTRY * PFNGLPOLYGONOFFSETCLAMPPROC) (GLfloat factor, GLfloat units, GLfloat clamp);

#define glPolygonOffsetClamp GLEW_GET_FUN(__glewPolygonOffsetClamp)

#define GLEW_ARB_polygon_offset_clamp GLEW_GET_VAR(__GLEW_ARB_polygon_offset_clamp)

#endif /* GL_ARB_polygon_offset_clamp */

/* ----------------------- GL_ARB_post_depth_coverage ---------------------- */

#ifndef GL_ARB_post_depth_coverage
#define GL_ARB_post_depth_coverage 1

#define GLEW_ARB_post_depth_coverage GLEW_GET_VAR(__GLEW_ARB_post_depth_coverage)

#endif /* GL_ARB_post_depth_coverage */

/* --------------------- GL_ARB_program_interface_query -------------------- */

#ifndef GL_ARB_program_interface_query
#define GL_ARB_program_interface_query 1

#define GL_UNIFORM 0x92E1
#define GL_UNIFORM_BLOCK 0x92E2
#define GL_PROGRAM_INPUT 0x92E3
#define GL_PROGRAM_OUTPUT 0x92E4
#define GL_BUFFER_VARIABLE 0x92E5
#define GL_SHADER_STORAGE_BLOCK 0x92E6
#define GL_IS_PER_PATCH 0x92E7
#define GL_VERTEX_SUBROUTINE 0x92E8
#define GL_TESS_CONTROL_SUBROUTINE 0x92E9
#define GL_TESS_EVALUATION_SUBROUTINE 0x92EA
#define GL_GEOMETRY_SUBROUTINE 0x92EB
#define GL_FRAGMENT_SUBROUTINE 0x92EC
#define GL_COMPUTE_SUBROUTINE 0x92ED
#define GL_VERTEX_SUBROUTINE_UNIFORM 0x92EE
#define GL_TESS_CONTROL_SUBROUTINE_UNIFORM 0x92EF
#define GL_TESS_EVALUATION_SUBROUTINE_UNIFORM 0x92F0
#define GL_GEOMETRY_SUBROUTINE_UNIFORM 0x92F1
#define GL_FRAGMENT_SUBROUTINE_UNIFORM 0x92F2
#define GL_COMPUTE_SUBROUTINE_UNIFORM 0x92F3
#define GL_TRANSFORM_FEEDBACK_VARYING 0x92F4
#define GL_ACTIVE_RESOURCES 0x92F5
#define GL_MAX_NAME_LENGTH 0x92F6
#define GL_MAX_NUM_ACTIVE_VARIABLES 0x92F7
#define GL_MAX_NUM_COMPATIBLE_SUBROUTINES 0x92F8
#define GL_NAME_LENGTH 0x92F9
#define GL_TYPE 0x92FA
#define GL_ARRAY_SIZE 0x92FB
#define GL_OFFSET 0x92FC
#define GL_BLOCK_INDEX 0x92FD
#define GL_ARRAY_STRIDE 0x92FE
#define GL_MATRIX_STRIDE 0x92FF
#define GL_IS_ROW_MAJOR 0x9300
#define GL_ATOMIC_COUNTER_BUFFER_INDEX 0x9301
#define GL_BUFFER_BINDING 0x9302
#define GL_BUFFER_DATA_SIZE 0x9303
#define GL_NUM_ACTIVE_VARIABLES 0x9304
#define GL_ACTIVE_VARIABLES 0x9305
#define GL_REFERENCED_BY_VERTEX_SHADER 0x9306
#define GL_REFERENCED_BY_TESS_CONTROL_SHADER 0x9307
#define GL_REFERENCED_BY_TESS_EVALUATION_SHADER 0x9308
#define GL_REFERENCED_BY_GEOMETRY_SHADER 0x9309
#define GL_REFERENCED_BY_FRAGMENT_SHADER 0x930A
#define GL_REFERENCED_BY_COMPUTE_SHADER 0x930B
#define GL_TOP_LEVEL_ARRAY_SIZE 0x930C
#define GL_TOP_LEVEL_ARRAY_STRIDE 0x930D
#define GL_LOCATION 0x930E
#define GL_LOCATION_INDEX 0x930F

typedef void (GLAPIENTRY * PFNGLGETPROGRAMINTERFACEIVPROC) (GLuint program, GLenum programInterface, GLenum pname, GLint* params);
typedef GLuint (GLAPIENTRY * PFNGLGETPROGRAMRESOURCEINDEXPROC) (GLuint program, GLenum programInterface, const GLchar* name);
typedef GLint (GLAPIENTRY * PFNGLGETPROGRAMRESOURCELOCATIONPROC) (GLuint program, GLenum programInterface, const GLchar* name);
typedef GLint (GLAPIENTRY * PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC) (GLuint program, GLenum programInterface, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMRESOURCENAMEPROC) (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei* length, GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMRESOURCEIVPROC) (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum* props, GLsizei bufSize, GLsizei *length, GLint *params);

#define glGetProgramInterfaceiv GLEW_GET_FUN(__glewGetProgramInterfaceiv)
#define glGetProgramResourceIndex GLEW_GET_FUN(__glewGetProgramResourceIndex)
#define glGetProgramResourceLocation GLEW_GET_FUN(__glewGetProgramResourceLocation)
#define glGetProgramResourceLocationIndex GLEW_GET_FUN(__glewGetProgramResourceLocationIndex)
#define glGetProgramResourceName GLEW_GET_FUN(__glewGetProgramResourceName)
#define glGetProgramResourceiv GLEW_GET_FUN(__glewGetProgramResourceiv)

#define GLEW_ARB_program_interface_query GLEW_GET_VAR(__GLEW_ARB_program_interface_query)

#endif /* GL_ARB_program_interface_query */

/* ------------------------ GL_ARB_provoking_vertex ------------------------ */

#ifndef GL_ARB_provoking_vertex
#define GL_ARB_provoking_vertex 1

#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION 0x8E4C
#define GL_FIRST_VERTEX_CONVENTION 0x8E4D
#define GL_LAST_VERTEX_CONVENTION 0x8E4E
#define GL_PROVOKING_VERTEX 0x8E4F

typedef void (GLAPIENTRY * PFNGLPROVOKINGVERTEXPROC) (GLenum mode);

#define glProvokingVertex GLEW_GET_FUN(__glewProvokingVertex)

#define GLEW_ARB_provoking_vertex GLEW_GET_VAR(__GLEW_ARB_provoking_vertex)

#endif /* GL_ARB_provoking_vertex */

/* ----------------------- GL_ARB_query_buffer_object ---------------------- */

#ifndef GL_ARB_query_buffer_object
#define GL_ARB_query_buffer_object 1

#define GL_QUERY_BUFFER_BARRIER_BIT 0x00008000
#define GL_QUERY_BUFFER 0x9192
#define GL_QUERY_BUFFER_BINDING 0x9193
#define GL_QUERY_RESULT_NO_WAIT 0x9194

#define GLEW_ARB_query_buffer_object GLEW_GET_VAR(__GLEW_ARB_query_buffer_object)

#endif /* GL_ARB_query_buffer_object */

/* ------------------ GL_ARB_robust_buffer_access_behavior ----------------- */

#ifndef GL_ARB_robust_buffer_access_behavior
#define GL_ARB_robust_buffer_access_behavior 1

#define GLEW_ARB_robust_buffer_access_behavior GLEW_GET_VAR(__GLEW_ARB_robust_buffer_access_behavior)

#endif /* GL_ARB_robust_buffer_access_behavior */

/* --------------------------- GL_ARB_robustness --------------------------- */

#ifndef GL_ARB_robustness
#define GL_ARB_robustness 1

#define GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB 0x00000004
#define GL_LOSE_CONTEXT_ON_RESET_ARB 0x8252
#define GL_GUILTY_CONTEXT_RESET_ARB 0x8253
#define GL_INNOCENT_CONTEXT_RESET_ARB 0x8254
#define GL_UNKNOWN_CONTEXT_RESET_ARB 0x8255
#define GL_RESET_NOTIFICATION_STRATEGY_ARB 0x8256
#define GL_NO_RESET_NOTIFICATION_ARB 0x8261

typedef GLenum (GLAPIENTRY * PFNGLGETGRAPHICSRESETSTATUSARBPROC) (void);
typedef void (GLAPIENTRY * PFNGLGETNCOLORTABLEARBPROC) (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void* table);
typedef void (GLAPIENTRY * PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC) (GLenum target, GLint lod, GLsizei bufSize, void* img);
typedef void (GLAPIENTRY * PFNGLGETNCONVOLUTIONFILTERARBPROC) (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void* image);
typedef void (GLAPIENTRY * PFNGLGETNHISTOGRAMARBPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void* values);
typedef void (GLAPIENTRY * PFNGLGETNMAPDVARBPROC) (GLenum target, GLenum query, GLsizei bufSize, GLdouble* v);
typedef void (GLAPIENTRY * PFNGLGETNMAPFVARBPROC) (GLenum target, GLenum query, GLsizei bufSize, GLfloat* v);
typedef void (GLAPIENTRY * PFNGLGETNMAPIVARBPROC) (GLenum target, GLenum query, GLsizei bufSize, GLint* v);
typedef void (GLAPIENTRY * PFNGLGETNMINMAXARBPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void* values);
typedef void (GLAPIENTRY * PFNGLGETNPIXELMAPFVARBPROC) (GLenum map, GLsizei bufSize, GLfloat* values);
typedef void (GLAPIENTRY * PFNGLGETNPIXELMAPUIVARBPROC) (GLenum map, GLsizei bufSize, GLuint* values);
typedef void (GLAPIENTRY * PFNGLGETNPIXELMAPUSVARBPROC) (GLenum map, GLsizei bufSize, GLushort* values);
typedef void (GLAPIENTRY * PFNGLGETNPOLYGONSTIPPLEARBPROC) (GLsizei bufSize, GLubyte* pattern);
typedef void (GLAPIENTRY * PFNGLGETNSEPARABLEFILTERARBPROC) (GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void* row, GLsizei columnBufSize, void*column, void*span);
typedef void (GLAPIENTRY * PFNGLGETNTEXIMAGEARBPROC) (GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* img);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMDVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMFVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMIVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMUIVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLuint* params);
typedef void (GLAPIENTRY * PFNGLREADNPIXELSARBPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void* data);

#define glGetGraphicsResetStatusARB GLEW_GET_FUN(__glewGetGraphicsResetStatusARB)
#define glGetnColorTableARB GLEW_GET_FUN(__glewGetnColorTableARB)
#define glGetnCompressedTexImageARB GLEW_GET_FUN(__glewGetnCompressedTexImageARB)
#define glGetnConvolutionFilterARB GLEW_GET_FUN(__glewGetnConvolutionFilterARB)
#define glGetnHistogramARB GLEW_GET_FUN(__glewGetnHistogramARB)
#define glGetnMapdvARB GLEW_GET_FUN(__glewGetnMapdvARB)
#define glGetnMapfvARB GLEW_GET_FUN(__glewGetnMapfvARB)
#define glGetnMapivARB GLEW_GET_FUN(__glewGetnMapivARB)
#define glGetnMinmaxARB GLEW_GET_FUN(__glewGetnMinmaxARB)
#define glGetnPixelMapfvARB GLEW_GET_FUN(__glewGetnPixelMapfvARB)
#define glGetnPixelMapuivARB GLEW_GET_FUN(__glewGetnPixelMapuivARB)
#define glGetnPixelMapusvARB GLEW_GET_FUN(__glewGetnPixelMapusvARB)
#define glGetnPolygonStippleARB GLEW_GET_FUN(__glewGetnPolygonStippleARB)
#define glGetnSeparableFilterARB GLEW_GET_FUN(__glewGetnSeparableFilterARB)
#define glGetnTexImageARB GLEW_GET_FUN(__glewGetnTexImageARB)
#define glGetnUniformdvARB GLEW_GET_FUN(__glewGetnUniformdvARB)
#define glGetnUniformfvARB GLEW_GET_FUN(__glewGetnUniformfvARB)
#define glGetnUniformivARB GLEW_GET_FUN(__glewGetnUniformivARB)
#define glGetnUniformuivARB GLEW_GET_FUN(__glewGetnUniformuivARB)
#define glReadnPixelsARB GLEW_GET_FUN(__glewReadnPixelsARB)

#define GLEW_ARB_robustness GLEW_GET_VAR(__GLEW_ARB_robustness)

#endif /* GL_ARB_robustness */

/* ---------------- GL_ARB_robustness_application_isolation ---------------- */

#ifndef GL_ARB_robustness_application_isolation
#define GL_ARB_robustness_application_isolation 1

#define GLEW_ARB_robustness_application_isolation GLEW_GET_VAR(__GLEW_ARB_robustness_application_isolation)

#endif /* GL_ARB_robustness_application_isolation */

/* ---------------- GL_ARB_robustness_share_group_isolation ---------------- */

#ifndef GL_ARB_robustness_share_group_isolation
#define GL_ARB_robustness_share_group_isolation 1

#define GLEW_ARB_robustness_share_group_isolation GLEW_GET_VAR(__GLEW_ARB_robustness_share_group_isolation)

#endif /* GL_ARB_robustness_share_group_isolation */

/* ------------------------ GL_ARB_sample_locations ------------------------ */

#ifndef GL_ARB_sample_locations
#define GL_ARB_sample_locations 1

#define GL_SAMPLE_LOCATION_ARB 0x8E50
#define GL_SAMPLE_LOCATION_SUBPIXEL_BITS_ARB 0x933D
#define GL_SAMPLE_LOCATION_PIXEL_GRID_WIDTH_ARB 0x933E
#define GL_SAMPLE_LOCATION_PIXEL_GRID_HEIGHT_ARB 0x933F
#define GL_PROGRAMMABLE_SAMPLE_LOCATION_TABLE_SIZE_ARB 0x9340
#define GL_PROGRAMMABLE_SAMPLE_LOCATION_ARB 0x9341
#define GL_FRAMEBUFFER_PROGRAMMABLE_SAMPLE_LOCATIONS_ARB 0x9342
#define GL_FRAMEBUFFER_SAMPLE_LOCATION_PIXEL_GRID_ARB 0x9343

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERSAMPLELOCATIONSFVARBPROC) (GLenum target, GLuint start, GLsizei count, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERSAMPLELOCATIONSFVARBPROC) (GLuint framebuffer, GLuint start, GLsizei count, const GLfloat* v);

#define glFramebufferSampleLocationsfvARB GLEW_GET_FUN(__glewFramebufferSampleLocationsfvARB)
#define glNamedFramebufferSampleLocationsfvARB GLEW_GET_FUN(__glewNamedFramebufferSampleLocationsfvARB)

#define GLEW_ARB_sample_locations GLEW_GET_VAR(__GLEW_ARB_sample_locations)

#endif /* GL_ARB_sample_locations */

/* ------------------------- GL_ARB_sample_shading ------------------------- */

#ifndef GL_ARB_sample_shading
#define GL_ARB_sample_shading 1

#define GL_SAMPLE_SHADING_ARB 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE_ARB 0x8C37

typedef void (GLAPIENTRY * PFNGLMINSAMPLESHADINGARBPROC) (GLclampf value);

#define glMinSampleShadingARB GLEW_GET_FUN(__glewMinSampleShadingARB)

#define GLEW_ARB_sample_shading GLEW_GET_VAR(__GLEW_ARB_sample_shading)

#endif /* GL_ARB_sample_shading */

/* ------------------------- GL_ARB_sampler_objects ------------------------ */

#ifndef GL_ARB_sampler_objects
#define GL_ARB_sampler_objects 1

#define GL_SAMPLER_BINDING 0x8919

typedef void (GLAPIENTRY * PFNGLBINDSAMPLERPROC) (GLuint unit, GLuint sampler);
typedef void (GLAPIENTRY * PFNGLDELETESAMPLERSPROC) (GLsizei count, const GLuint * samplers);
typedef void (GLAPIENTRY * PFNGLGENSAMPLERSPROC) (GLsizei count, GLuint* samplers);
typedef void (GLAPIENTRY * PFNGLGETSAMPLERPARAMETERIIVPROC) (GLuint sampler, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETSAMPLERPARAMETERIUIVPROC) (GLuint sampler, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISSAMPLERPROC) (GLuint sampler);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERIIVPROC) (GLuint sampler, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERIUIVPROC) (GLuint sampler, GLenum pname, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERFPROC) (GLuint sampler, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERIPROC) (GLuint sampler, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, const GLint* params);

#define glBindSampler GLEW_GET_FUN(__glewBindSampler)
#define glDeleteSamplers GLEW_GET_FUN(__glewDeleteSamplers)
#define glGenSamplers GLEW_GET_FUN(__glewGenSamplers)
#define glGetSamplerParameterIiv GLEW_GET_FUN(__glewGetSamplerParameterIiv)
#define glGetSamplerParameterIuiv GLEW_GET_FUN(__glewGetSamplerParameterIuiv)
#define glGetSamplerParameterfv GLEW_GET_FUN(__glewGetSamplerParameterfv)
#define glGetSamplerParameteriv GLEW_GET_FUN(__glewGetSamplerParameteriv)
#define glIsSampler GLEW_GET_FUN(__glewIsSampler)
#define glSamplerParameterIiv GLEW_GET_FUN(__glewSamplerParameterIiv)
#define glSamplerParameterIuiv GLEW_GET_FUN(__glewSamplerParameterIuiv)
#define glSamplerParameterf GLEW_GET_FUN(__glewSamplerParameterf)
#define glSamplerParameterfv GLEW_GET_FUN(__glewSamplerParameterfv)
#define glSamplerParameteri GLEW_GET_FUN(__glewSamplerParameteri)
#define glSamplerParameteriv GLEW_GET_FUN(__glewSamplerParameteriv)

#define GLEW_ARB_sampler_objects GLEW_GET_VAR(__GLEW_ARB_sampler_objects)

#endif /* GL_ARB_sampler_objects */

/* ------------------------ GL_ARB_seamless_cube_map ----------------------- */

#ifndef GL_ARB_seamless_cube_map
#define GL_ARB_seamless_cube_map 1

#define GL_TEXTURE_CUBE_MAP_SEAMLESS 0x884F

#define GLEW_ARB_seamless_cube_map GLEW_GET_VAR(__GLEW_ARB_seamless_cube_map)

#endif /* GL_ARB_seamless_cube_map */

/* ------------------ GL_ARB_seamless_cubemap_per_texture ------------------ */

#ifndef GL_ARB_seamless_cubemap_per_texture
#define GL_ARB_seamless_cubemap_per_texture 1

#define GL_TEXTURE_CUBE_MAP_SEAMLESS 0x884F

#define GLEW_ARB_seamless_cubemap_per_texture GLEW_GET_VAR(__GLEW_ARB_seamless_cubemap_per_texture)

#endif /* GL_ARB_seamless_cubemap_per_texture */

/* --------------------- GL_ARB_separate_shader_objects -------------------- */

#ifndef GL_ARB_separate_shader_objects
#define GL_ARB_separate_shader_objects 1

#define GL_VERTEX_SHADER_BIT 0x00000001
#define GL_FRAGMENT_SHADER_BIT 0x00000002
#define GL_GEOMETRY_SHADER_BIT 0x00000004
#define GL_TESS_CONTROL_SHADER_BIT 0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT 0x00000010
#define GL_PROGRAM_SEPARABLE 0x8258
#define GL_ACTIVE_PROGRAM 0x8259
#define GL_PROGRAM_PIPELINE_BINDING 0x825A
#define GL_ALL_SHADER_BITS 0xFFFFFFFF

typedef void (GLAPIENTRY * PFNGLACTIVESHADERPROGRAMPROC) (GLuint pipeline, GLuint program);
typedef void (GLAPIENTRY * PFNGLBINDPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef GLuint (GLAPIENTRY * PFNGLCREATESHADERPROGRAMVPROC) (GLenum type, GLsizei count, const GLchar * const * strings);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMPIPELINESPROC) (GLsizei n, const GLuint* pipelines);
typedef void (GLAPIENTRY * PFNGLGENPROGRAMPIPELINESPROC) (GLsizei n, GLuint* pipelines);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMPIPELINEINFOLOGPROC) (GLuint pipeline, GLsizei bufSize, GLsizei* length, GLchar *infoLog);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMPIPELINEIVPROC) (GLuint pipeline, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1DPROC) (GLuint program, GLint location, GLdouble x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FPROC) (GLuint program, GLint location, GLfloat x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IPROC) (GLuint program, GLint location, GLint x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIPROC) (GLuint program, GLint location, GLuint x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IPROC) (GLuint program, GLint location, GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIPROC) (GLuint program, GLint location, GLuint x, GLuint y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IPROC) (GLuint program, GLint location, GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIPROC) (GLuint program, GLint location, GLuint x, GLuint y, GLuint z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IPROC) (GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIPROC) (GLuint program, GLint location, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUSEPROGRAMSTAGESPROC) (GLuint pipeline, GLbitfield stages, GLuint program);
typedef void (GLAPIENTRY * PFNGLVALIDATEPROGRAMPIPELINEPROC) (GLuint pipeline);

#define glActiveShaderProgram GLEW_GET_FUN(__glewActiveShaderProgram)
#define glBindProgramPipeline GLEW_GET_FUN(__glewBindProgramPipeline)
#define glCreateShaderProgramv GLEW_GET_FUN(__glewCreateShaderProgramv)
#define glDeleteProgramPipelines GLEW_GET_FUN(__glewDeleteProgramPipelines)
#define glGenProgramPipelines GLEW_GET_FUN(__glewGenProgramPipelines)
#define glGetProgramPipelineInfoLog GLEW_GET_FUN(__glewGetProgramPipelineInfoLog)
#define glGetProgramPipelineiv GLEW_GET_FUN(__glewGetProgramPipelineiv)
#define glIsProgramPipeline GLEW_GET_FUN(__glewIsProgramPipeline)
#define glProgramUniform1d GLEW_GET_FUN(__glewProgramUniform1d)
#define glProgramUniform1dv GLEW_GET_FUN(__glewProgramUniform1dv)
#define glProgramUniform1f GLEW_GET_FUN(__glewProgramUniform1f)
#define glProgramUniform1fv GLEW_GET_FUN(__glewProgramUniform1fv)
#define glProgramUniform1i GLEW_GET_FUN(__glewProgramUniform1i)
#define glProgramUniform1iv GLEW_GET_FUN(__glewProgramUniform1iv)
#define glProgramUniform1ui GLEW_GET_FUN(__glewProgramUniform1ui)
#define glProgramUniform1uiv GLEW_GET_FUN(__glewProgramUniform1uiv)
#define glProgramUniform2d GLEW_GET_FUN(__glewProgramUniform2d)
#define glProgramUniform2dv GLEW_GET_FUN(__glewProgramUniform2dv)
#define glProgramUniform2f GLEW_GET_FUN(__glewProgramUniform2f)
#define glProgramUniform2fv GLEW_GET_FUN(__glewProgramUniform2fv)
#define glProgramUniform2i GLEW_GET_FUN(__glewProgramUniform2i)
#define glProgramUniform2iv GLEW_GET_FUN(__glewProgramUniform2iv)
#define glProgramUniform2ui GLEW_GET_FUN(__glewProgramUniform2ui)
#define glProgramUniform2uiv GLEW_GET_FUN(__glewProgramUniform2uiv)
#define glProgramUniform3d GLEW_GET_FUN(__glewProgramUniform3d)
#define glProgramUniform3dv GLEW_GET_FUN(__glewProgramUniform3dv)
#define glProgramUniform3f GLEW_GET_FUN(__glewProgramUniform3f)
#define glProgramUniform3fv GLEW_GET_FUN(__glewProgramUniform3fv)
#define glProgramUniform3i GLEW_GET_FUN(__glewProgramUniform3i)
#define glProgramUniform3iv GLEW_GET_FUN(__glewProgramUniform3iv)
#define glProgramUniform3ui GLEW_GET_FUN(__glewProgramUniform3ui)
#define glProgramUniform3uiv GLEW_GET_FUN(__glewProgramUniform3uiv)
#define glProgramUniform4d GLEW_GET_FUN(__glewProgramUniform4d)
#define glProgramUniform4dv GLEW_GET_FUN(__glewProgramUniform4dv)
#define glProgramUniform4f GLEW_GET_FUN(__glewProgramUniform4f)
#define glProgramUniform4fv GLEW_GET_FUN(__glewProgramUniform4fv)
#define glProgramUniform4i GLEW_GET_FUN(__glewProgramUniform4i)
#define glProgramUniform4iv GLEW_GET_FUN(__glewProgramUniform4iv)
#define glProgramUniform4ui GLEW_GET_FUN(__glewProgramUniform4ui)
#define glProgramUniform4uiv GLEW_GET_FUN(__glewProgramUniform4uiv)
#define glProgramUniformMatrix2dv GLEW_GET_FUN(__glewProgramUniformMatrix2dv)
#define glProgramUniformMatrix2fv GLEW_GET_FUN(__glewProgramUniformMatrix2fv)
#define glProgramUniformMatrix2x3dv GLEW_GET_FUN(__glewProgramUniformMatrix2x3dv)
#define glProgramUniformMatrix2x3fv GLEW_GET_FUN(__glewProgramUniformMatrix2x3fv)
#define glProgramUniformMatrix2x4dv GLEW_GET_FUN(__glewProgramUniformMatrix2x4dv)
#define glProgramUniformMatrix2x4fv GLEW_GET_FUN(__glewProgramUniformMatrix2x4fv)
#define glProgramUniformMatrix3dv GLEW_GET_FUN(__glewProgramUniformMatrix3dv)
#define glProgramUniformMatrix3fv GLEW_GET_FUN(__glewProgramUniformMatrix3fv)
#define glProgramUniformMatrix3x2dv GLEW_GET_FUN(__glewProgramUniformMatrix3x2dv)
#define glProgramUniformMatrix3x2fv GLEW_GET_FUN(__glewProgramUniformMatrix3x2fv)
#define glProgramUniformMatrix3x4dv GLEW_GET_FUN(__glewProgramUniformMatrix3x4dv)
#define glProgramUniformMatrix3x4fv GLEW_GET_FUN(__glewProgramUniformMatrix3x4fv)
#define glProgramUniformMatrix4dv GLEW_GET_FUN(__glewProgramUniformMatrix4dv)
#define glProgramUniformMatrix4fv GLEW_GET_FUN(__glewProgramUniformMatrix4fv)
#define glProgramUniformMatrix4x2dv GLEW_GET_FUN(__glewProgramUniformMatrix4x2dv)
#define glProgramUniformMatrix4x2fv GLEW_GET_FUN(__glewProgramUniformMatrix4x2fv)
#define glProgramUniformMatrix4x3dv GLEW_GET_FUN(__glewProgramUniformMatrix4x3dv)
#define glProgramUniformMatrix4x3fv GLEW_GET_FUN(__glewProgramUniformMatrix4x3fv)
#define glUseProgramStages GLEW_GET_FUN(__glewUseProgramStages)
#define glValidateProgramPipeline GLEW_GET_FUN(__glewValidateProgramPipeline)

#define GLEW_ARB_separate_shader_objects GLEW_GET_VAR(__GLEW_ARB_separate_shader_objects)

#endif /* GL_ARB_separate_shader_objects */

/* -------------------- GL_ARB_shader_atomic_counter_ops ------------------- */

#ifndef GL_ARB_shader_atomic_counter_ops
#define GL_ARB_shader_atomic_counter_ops 1

#define GLEW_ARB_shader_atomic_counter_ops GLEW_GET_VAR(__GLEW_ARB_shader_atomic_counter_ops)

#endif /* GL_ARB_shader_atomic_counter_ops */

/* --------------------- GL_ARB_shader_atomic_counters --------------------- */

#ifndef GL_ARB_shader_atomic_counters
#define GL_ARB_shader_atomic_counters 1

#define GL_ATOMIC_COUNTER_BUFFER 0x92C0
#define GL_ATOMIC_COUNTER_BUFFER_BINDING 0x92C1
#define GL_ATOMIC_COUNTER_BUFFER_START 0x92C2
#define GL_ATOMIC_COUNTER_BUFFER_SIZE 0x92C3
#define GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE 0x92C4
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS 0x92C5
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES 0x92C6
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER 0x92C7
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER 0x92C8
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER 0x92C9
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER 0x92CA
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER 0x92CB
#define GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS 0x92CC
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS 0x92CD
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS 0x92CE
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS 0x92CF
#define GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS 0x92D0
#define GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS 0x92D1
#define GL_MAX_VERTEX_ATOMIC_COUNTERS 0x92D2
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS 0x92D3
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS 0x92D4
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS 0x92D5
#define GL_MAX_FRAGMENT_ATOMIC_COUNTERS 0x92D6
#define GL_MAX_COMBINED_ATOMIC_COUNTERS 0x92D7
#define GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE 0x92D8
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS 0x92D9
#define GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX 0x92DA
#define GL_UNSIGNED_INT_ATOMIC_COUNTER 0x92DB
#define GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS 0x92DC

typedef void (GLAPIENTRY * PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC) (GLuint program, GLuint bufferIndex, GLenum pname, GLint* params);

#define glGetActiveAtomicCounterBufferiv GLEW_GET_FUN(__glewGetActiveAtomicCounterBufferiv)

#define GLEW_ARB_shader_atomic_counters GLEW_GET_VAR(__GLEW_ARB_shader_atomic_counters)

#endif /* GL_ARB_shader_atomic_counters */

/* -------------------------- GL_ARB_shader_ballot ------------------------- */

#ifndef GL_ARB_shader_ballot
#define GL_ARB_shader_ballot 1

#define GLEW_ARB_shader_ballot GLEW_GET_VAR(__GLEW_ARB_shader_ballot)

#endif /* GL_ARB_shader_ballot */

/* ----------------------- GL_ARB_shader_bit_encoding ---------------------- */

#ifndef GL_ARB_shader_bit_encoding
#define GL_ARB_shader_bit_encoding 1

#define GLEW_ARB_shader_bit_encoding GLEW_GET_VAR(__GLEW_ARB_shader_bit_encoding)

#endif /* GL_ARB_shader_bit_encoding */

/* -------------------------- GL_ARB_shader_clock -------------------------- */

#ifndef GL_ARB_shader_clock
#define GL_ARB_shader_clock 1

#define GLEW_ARB_shader_clock GLEW_GET_VAR(__GLEW_ARB_shader_clock)

#endif /* GL_ARB_shader_clock */

/* --------------------- GL_ARB_shader_draw_parameters --------------------- */

#ifndef GL_ARB_shader_draw_parameters
#define GL_ARB_shader_draw_parameters 1

#define GLEW_ARB_shader_draw_parameters GLEW_GET_VAR(__GLEW_ARB_shader_draw_parameters)

#endif /* GL_ARB_shader_draw_parameters */

/* ------------------------ GL_ARB_shader_group_vote ----------------------- */

#ifndef GL_ARB_shader_group_vote
#define GL_ARB_shader_group_vote 1

#define GLEW_ARB_shader_group_vote GLEW_GET_VAR(__GLEW_ARB_shader_group_vote)

#endif /* GL_ARB_shader_group_vote */

/* --------------------- GL_ARB_shader_image_load_store -------------------- */

#ifndef GL_ARB_shader_image_load_store
#define GL_ARB_shader_image_load_store 1

#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT 0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT 0x00000002
#define GL_UNIFORM_BARRIER_BIT 0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT 0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#define GL_COMMAND_BARRIER_BIT 0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT 0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT 0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT 0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT 0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT 0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT 0x00001000
#define GL_MAX_IMAGE_UNITS 0x8F38
#define GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS 0x8F39
#define GL_IMAGE_BINDING_NAME 0x8F3A
#define GL_IMAGE_BINDING_LEVEL 0x8F3B
#define GL_IMAGE_BINDING_LAYERED 0x8F3C
#define GL_IMAGE_BINDING_LAYER 0x8F3D
#define GL_IMAGE_BINDING_ACCESS 0x8F3E
#define GL_IMAGE_1D 0x904C
#define GL_IMAGE_2D 0x904D
#define GL_IMAGE_3D 0x904E
#define GL_IMAGE_2D_RECT 0x904F
#define GL_IMAGE_CUBE 0x9050
#define GL_IMAGE_BUFFER 0x9051
#define GL_IMAGE_1D_ARRAY 0x9052
#define GL_IMAGE_2D_ARRAY 0x9053
#define GL_IMAGE_CUBE_MAP_ARRAY 0x9054
#define GL_IMAGE_2D_MULTISAMPLE 0x9055
#define GL_IMAGE_2D_MULTISAMPLE_ARRAY 0x9056
#define GL_INT_IMAGE_1D 0x9057
#define GL_INT_IMAGE_2D 0x9058
#define GL_INT_IMAGE_3D 0x9059
#define GL_INT_IMAGE_2D_RECT 0x905A
#define GL_INT_IMAGE_CUBE 0x905B
#define GL_INT_IMAGE_BUFFER 0x905C
#define GL_INT_IMAGE_1D_ARRAY 0x905D
#define GL_INT_IMAGE_2D_ARRAY 0x905E
#define GL_INT_IMAGE_CUBE_MAP_ARRAY 0x905F
#define GL_INT_IMAGE_2D_MULTISAMPLE 0x9060
#define GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x9061
#define GL_UNSIGNED_INT_IMAGE_1D 0x9062
#define GL_UNSIGNED_INT_IMAGE_2D 0x9063
#define GL_UNSIGNED_INT_IMAGE_3D 0x9064
#define GL_UNSIGNED_INT_IMAGE_2D_RECT 0x9065
#define GL_UNSIGNED_INT_IMAGE_CUBE 0x9066
#define GL_UNSIGNED_INT_IMAGE_BUFFER 0x9067
#define GL_UNSIGNED_INT_IMAGE_1D_ARRAY 0x9068
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY 0x9069
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY 0x906A
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE 0x906B
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x906C
#define GL_MAX_IMAGE_SAMPLES 0x906D
#define GL_IMAGE_BINDING_FORMAT 0x906E
#define GL_IMAGE_FORMAT_COMPATIBILITY_TYPE 0x90C7
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE 0x90C8
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS 0x90C9
#define GL_MAX_VERTEX_IMAGE_UNIFORMS 0x90CA
#define GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS 0x90CB
#define GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS 0x90CC
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS 0x90CD
#define GL_MAX_FRAGMENT_IMAGE_UNIFORMS 0x90CE
#define GL_MAX_COMBINED_IMAGE_UNIFORMS 0x90CF
#define GL_ALL_BARRIER_BITS 0xFFFFFFFF

typedef void (GLAPIENTRY * PFNGLBINDIMAGETEXTUREPROC) (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
typedef void (GLAPIENTRY * PFNGLMEMORYBARRIERPROC) (GLbitfield barriers);

#define glBindImageTexture GLEW_GET_FUN(__glewBindImageTexture)
#define glMemoryBarrier GLEW_GET_FUN(__glewMemoryBarrier)

#define GLEW_ARB_shader_image_load_store GLEW_GET_VAR(__GLEW_ARB_shader_image_load_store)

#endif /* GL_ARB_shader_image_load_store */

/* ------------------------ GL_ARB_shader_image_size ----------------------- */

#ifndef GL_ARB_shader_image_size
#define GL_ARB_shader_image_size 1

#define GLEW_ARB_shader_image_size GLEW_GET_VAR(__GLEW_ARB_shader_image_size)

#endif /* GL_ARB_shader_image_size */

/* ------------------------- GL_ARB_shader_objects ------------------------- */

#ifndef GL_ARB_shader_objects
#define GL_ARB_shader_objects 1

#define GL_PROGRAM_OBJECT_ARB 0x8B40
#define GL_SHADER_OBJECT_ARB 0x8B48
#define GL_OBJECT_TYPE_ARB 0x8B4E
#define GL_OBJECT_SUBTYPE_ARB 0x8B4F
#define GL_FLOAT_VEC2_ARB 0x8B50
#define GL_FLOAT_VEC3_ARB 0x8B51
#define GL_FLOAT_VEC4_ARB 0x8B52
#define GL_INT_VEC2_ARB 0x8B53
#define GL_INT_VEC3_ARB 0x8B54
#define GL_INT_VEC4_ARB 0x8B55
#define GL_BOOL_ARB 0x8B56
#define GL_BOOL_VEC2_ARB 0x8B57
#define GL_BOOL_VEC3_ARB 0x8B58
#define GL_BOOL_VEC4_ARB 0x8B59
#define GL_FLOAT_MAT2_ARB 0x8B5A
#define GL_FLOAT_MAT3_ARB 0x8B5B
#define GL_FLOAT_MAT4_ARB 0x8B5C
#define GL_SAMPLER_1D_ARB 0x8B5D
#define GL_SAMPLER_2D_ARB 0x8B5E
#define GL_SAMPLER_3D_ARB 0x8B5F
#define GL_SAMPLER_CUBE_ARB 0x8B60
#define GL_SAMPLER_1D_SHADOW_ARB 0x8B61
#define GL_SAMPLER_2D_SHADOW_ARB 0x8B62
#define GL_SAMPLER_2D_RECT_ARB 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW_ARB 0x8B64
#define GL_OBJECT_DELETE_STATUS_ARB 0x8B80
#define GL_OBJECT_COMPILE_STATUS_ARB 0x8B81
#define GL_OBJECT_LINK_STATUS_ARB 0x8B82
#define GL_OBJECT_VALIDATE_STATUS_ARB 0x8B83
#define GL_OBJECT_INFO_LOG_LENGTH_ARB 0x8B84
#define GL_OBJECT_ATTACHED_OBJECTS_ARB 0x8B85
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB 0x8B86
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB 0x8B87
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB 0x8B88

typedef char GLcharARB;
typedef unsigned int GLhandleARB;

typedef void (GLAPIENTRY * PFNGLATTACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB obj);
typedef void (GLAPIENTRY * PFNGLCOMPILESHADERARBPROC) (GLhandleARB shaderObj);
typedef GLhandleARB (GLAPIENTRY * PFNGLCREATEPROGRAMOBJECTARBPROC) (void);
typedef GLhandleARB (GLAPIENTRY * PFNGLCREATESHADEROBJECTARBPROC) (GLenum shaderType);
typedef void (GLAPIENTRY * PFNGLDELETEOBJECTARBPROC) (GLhandleARB obj);
typedef void (GLAPIENTRY * PFNGLDETACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB attachedObj);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMARBPROC) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei* length, GLint *size, GLenum *type, GLcharARB *name);
typedef void (GLAPIENTRY * PFNGLGETATTACHEDOBJECTSARBPROC) (GLhandleARB containerObj, GLsizei maxCount, GLsizei* count, GLhandleARB *obj);
typedef GLhandleARB (GLAPIENTRY * PFNGLGETHANDLEARBPROC) (GLenum pname);
typedef void (GLAPIENTRY * PFNGLGETINFOLOGARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *infoLog);
typedef void (GLAPIENTRY * PFNGLGETOBJECTPARAMETERFVARBPROC) (GLhandleARB obj, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETOBJECTPARAMETERIVARBPROC) (GLhandleARB obj, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETSHADERSOURCEARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *source);
typedef GLint (GLAPIENTRY * PFNGLGETUNIFORMLOCATIONARBPROC) (GLhandleARB programObj, const GLcharARB* name);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMFVARBPROC) (GLhandleARB programObj, GLint location, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMIVARBPROC) (GLhandleARB programObj, GLint location, GLint* params);
typedef void (GLAPIENTRY * PFNGLLINKPROGRAMARBPROC) (GLhandleARB programObj);
typedef void (GLAPIENTRY * PFNGLSHADERSOURCEARBPROC) (GLhandleARB shaderObj, GLsizei count, const GLcharARB ** string, const GLint *length);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FARBPROC) (GLint location, GLfloat v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IARBPROC) (GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FARBPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IARBPROC) (GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUSEPROGRAMOBJECTARBPROC) (GLhandleARB programObj);
typedef void (GLAPIENTRY * PFNGLVALIDATEPROGRAMARBPROC) (GLhandleARB programObj);

#define glAttachObjectARB GLEW_GET_FUN(__glewAttachObjectARB)
#define glCompileShaderARB GLEW_GET_FUN(__glewCompileShaderARB)
#define glCreateProgramObjectARB GLEW_GET_FUN(__glewCreateProgramObjectARB)
#define glCreateShaderObjectARB GLEW_GET_FUN(__glewCreateShaderObjectARB)
#define glDeleteObjectARB GLEW_GET_FUN(__glewDeleteObjectARB)
#define glDetachObjectARB GLEW_GET_FUN(__glewDetachObjectARB)
#define glGetActiveUniformARB GLEW_GET_FUN(__glewGetActiveUniformARB)
#define glGetAttachedObjectsARB GLEW_GET_FUN(__glewGetAttachedObjectsARB)
#define glGetHandleARB GLEW_GET_FUN(__glewGetHandleARB)
#define glGetInfoLogARB GLEW_GET_FUN(__glewGetInfoLogARB)
#define glGetObjectParameterfvARB GLEW_GET_FUN(__glewGetObjectParameterfvARB)
#define glGetObjectParameterivARB GLEW_GET_FUN(__glewGetObjectParameterivARB)
#define glGetShaderSourceARB GLEW_GET_FUN(__glewGetShaderSourceARB)
#define glGetUniformLocationARB GLEW_GET_FUN(__glewGetUniformLocationARB)
#define glGetUniformfvARB GLEW_GET_FUN(__glewGetUniformfvARB)
#define glGetUniformivARB GLEW_GET_FUN(__glewGetUniformivARB)
#define glLinkProgramARB GLEW_GET_FUN(__glewLinkProgramARB)
#define glShaderSourceARB GLEW_GET_FUN(__glewShaderSourceARB)
#define glUniform1fARB GLEW_GET_FUN(__glewUniform1fARB)
#define glUniform1fvARB GLEW_GET_FUN(__glewUniform1fvARB)
#define glUniform1iARB GLEW_GET_FUN(__glewUniform1iARB)
#define glUniform1ivARB GLEW_GET_FUN(__glewUniform1ivARB)
#define glUniform2fARB GLEW_GET_FUN(__glewUniform2fARB)
#define glUniform2fvARB GLEW_GET_FUN(__glewUniform2fvARB)
#define glUniform2iARB GLEW_GET_FUN(__glewUniform2iARB)
#define glUniform2ivARB GLEW_GET_FUN(__glewUniform2ivARB)
#define glUniform3fARB GLEW_GET_FUN(__glewUniform3fARB)
#define glUniform3fvARB GLEW_GET_FUN(__glewUniform3fvARB)
#define glUniform3iARB GLEW_GET_FUN(__glewUniform3iARB)
#define glUniform3ivARB GLEW_GET_FUN(__glewUniform3ivARB)
#define glUniform4fARB GLEW_GET_FUN(__glewUniform4fARB)
#define glUniform4fvARB GLEW_GET_FUN(__glewUniform4fvARB)
#define glUniform4iARB GLEW_GET_FUN(__glewUniform4iARB)
#define glUniform4ivARB GLEW_GET_FUN(__glewUniform4ivARB)
#define glUniformMatrix2fvARB GLEW_GET_FUN(__glewUniformMatrix2fvARB)
#define glUniformMatrix3fvARB GLEW_GET_FUN(__glewUniformMatrix3fvARB)
#define glUniformMatrix4fvARB GLEW_GET_FUN(__glewUniformMatrix4fvARB)
#define glUseProgramObjectARB GLEW_GET_FUN(__glewUseProgramObjectARB)
#define glValidateProgramARB GLEW_GET_FUN(__glewValidateProgramARB)

#define GLEW_ARB_shader_objects GLEW_GET_VAR(__GLEW_ARB_shader_objects)

#endif /* GL_ARB_shader_objects */

/* ------------------------ GL_ARB_shader_precision ------------------------ */

#ifndef GL_ARB_shader_precision
#define GL_ARB_shader_precision 1

#define GLEW_ARB_shader_precision GLEW_GET_VAR(__GLEW_ARB_shader_precision)

#endif /* GL_ARB_shader_precision */

/* ---------------------- GL_ARB_shader_stencil_export --------------------- */

#ifndef GL_ARB_shader_stencil_export
#define GL_ARB_shader_stencil_export 1

#define GLEW_ARB_shader_stencil_export GLEW_GET_VAR(__GLEW_ARB_shader_stencil_export)

#endif /* GL_ARB_shader_stencil_export */

/* ------------------ GL_ARB_shader_storage_buffer_object ------------------ */

#ifndef GL_ARB_shader_storage_buffer_object
#define GL_ARB_shader_storage_buffer_object 1

#define GL_SHADER_STORAGE_BARRIER_BIT 0x2000
#define GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES 0x8F39
#define GL_SHADER_STORAGE_BUFFER 0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING 0x90D3
#define GL_SHADER_STORAGE_BUFFER_START 0x90D4
#define GL_SHADER_STORAGE_BUFFER_SIZE 0x90D5
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS 0x90D7
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS 0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS 0x90D9
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE 0x90DE
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT 0x90DF

typedef void (GLAPIENTRY * PFNGLSHADERSTORAGEBLOCKBINDINGPROC) (GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);

#define glShaderStorageBlockBinding GLEW_GET_FUN(__glewShaderStorageBlockBinding)

#define GLEW_ARB_shader_storage_buffer_object GLEW_GET_VAR(__GLEW_ARB_shader_storage_buffer_object)

#endif /* GL_ARB_shader_storage_buffer_object */

/* ------------------------ GL_ARB_shader_subroutine ----------------------- */

#ifndef GL_ARB_shader_subroutine
#define GL_ARB_shader_subroutine 1

#define GL_ACTIVE_SUBROUTINES 0x8DE5
#define GL_ACTIVE_SUBROUTINE_UNIFORMS 0x8DE6
#define GL_MAX_SUBROUTINES 0x8DE7
#define GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS 0x8DE8
#define GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS 0x8E47
#define GL_ACTIVE_SUBROUTINE_MAX_LENGTH 0x8E48
#define GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH 0x8E49
#define GL_NUM_COMPATIBLE_SUBROUTINES 0x8E4A
#define GL_COMPATIBLE_SUBROUTINES 0x8E4B

typedef void (GLAPIENTRY * PFNGLGETACTIVESUBROUTINENAMEPROC) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC) (GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint* values);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMSTAGEIVPROC) (GLuint program, GLenum shadertype, GLenum pname, GLint* values);
typedef GLuint (GLAPIENTRY * PFNGLGETSUBROUTINEINDEXPROC) (GLuint program, GLenum shadertype, const GLchar* name);
typedef GLint (GLAPIENTRY * PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC) (GLuint program, GLenum shadertype, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMSUBROUTINEUIVPROC) (GLenum shadertype, GLint location, GLuint* params);
typedef void (GLAPIENTRY * PFNGLUNIFORMSUBROUTINESUIVPROC) (GLenum shadertype, GLsizei count, const GLuint* indices);

#define glGetActiveSubroutineName GLEW_GET_FUN(__glewGetActiveSubroutineName)
#define glGetActiveSubroutineUniformName GLEW_GET_FUN(__glewGetActiveSubroutineUniformName)
#define glGetActiveSubroutineUniformiv GLEW_GET_FUN(__glewGetActiveSubroutineUniformiv)
#define glGetProgramStageiv GLEW_GET_FUN(__glewGetProgramStageiv)
#define glGetSubroutineIndex GLEW_GET_FUN(__glewGetSubroutineIndex)
#define glGetSubroutineUniformLocation GLEW_GET_FUN(__glewGetSubroutineUniformLocation)
#define glGetUniformSubroutineuiv GLEW_GET_FUN(__glewGetUniformSubroutineuiv)
#define glUniformSubroutinesuiv GLEW_GET_FUN(__glewUniformSubroutinesuiv)

#define GLEW_ARB_shader_subroutine GLEW_GET_VAR(__GLEW_ARB_shader_subroutine)

#endif /* GL_ARB_shader_subroutine */

/* ------------------ GL_ARB_shader_texture_image_samples ------------------ */

#ifndef GL_ARB_shader_texture_image_samples
#define GL_ARB_shader_texture_image_samples 1

#define GLEW_ARB_shader_texture_image_samples GLEW_GET_VAR(__GLEW_ARB_shader_texture_image_samples)

#endif /* GL_ARB_shader_texture_image_samples */

/* ----------------------- GL_ARB_shader_texture_lod ----------------------- */

#ifndef GL_ARB_shader_texture_lod
#define GL_ARB_shader_texture_lod 1

#define GLEW_ARB_shader_texture_lod GLEW_GET_VAR(__GLEW_ARB_shader_texture_lod)

#endif /* GL_ARB_shader_texture_lod */

/* ------------------- GL_ARB_shader_viewport_layer_array ------------------ */

#ifndef GL_ARB_shader_viewport_layer_array
#define GL_ARB_shader_viewport_layer_array 1

#define GLEW_ARB_shader_viewport_layer_array GLEW_GET_VAR(__GLEW_ARB_shader_viewport_layer_array)

#endif /* GL_ARB_shader_viewport_layer_array */

/* ---------------------- GL_ARB_shading_language_100 ---------------------- */

#ifndef GL_ARB_shading_language_100
#define GL_ARB_shading_language_100 1

#define GL_SHADING_LANGUAGE_VERSION_ARB 0x8B8C

#define GLEW_ARB_shading_language_100 GLEW_GET_VAR(__GLEW_ARB_shading_language_100)

#endif /* GL_ARB_shading_language_100 */

/* -------------------- GL_ARB_shading_language_420pack -------------------- */

#ifndef GL_ARB_shading_language_420pack
#define GL_ARB_shading_language_420pack 1

#define GLEW_ARB_shading_language_420pack GLEW_GET_VAR(__GLEW_ARB_shading_language_420pack)

#endif /* GL_ARB_shading_language_420pack */

/* -------------------- GL_ARB_shading_language_include -------------------- */

#ifndef GL_ARB_shading_language_include
#define GL_ARB_shading_language_include 1

#define GL_SHADER_INCLUDE_ARB 0x8DAE
#define GL_NAMED_STRING_LENGTH_ARB 0x8DE9
#define GL_NAMED_STRING_TYPE_ARB 0x8DEA

typedef void (GLAPIENTRY * PFNGLCOMPILESHADERINCLUDEARBPROC) (GLuint shader, GLsizei count, const GLchar* const *path, const GLint *length);
typedef void (GLAPIENTRY * PFNGLDELETENAMEDSTRINGARBPROC) (GLint namelen, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETNAMEDSTRINGARBPROC) (GLint namelen, const GLchar* name, GLsizei bufSize, GLint *stringlen, GLchar *string);
typedef void (GLAPIENTRY * PFNGLGETNAMEDSTRINGIVARBPROC) (GLint namelen, const GLchar* name, GLenum pname, GLint *params);
typedef GLboolean (GLAPIENTRY * PFNGLISNAMEDSTRINGARBPROC) (GLint namelen, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLNAMEDSTRINGARBPROC) (GLenum type, GLint namelen, const GLchar* name, GLint stringlen, const GLchar *string);

#define glCompileShaderIncludeARB GLEW_GET_FUN(__glewCompileShaderIncludeARB)
#define glDeleteNamedStringARB GLEW_GET_FUN(__glewDeleteNamedStringARB)
#define glGetNamedStringARB GLEW_GET_FUN(__glewGetNamedStringARB)
#define glGetNamedStringivARB GLEW_GET_FUN(__glewGetNamedStringivARB)
#define glIsNamedStringARB GLEW_GET_FUN(__glewIsNamedStringARB)
#define glNamedStringARB GLEW_GET_FUN(__glewNamedStringARB)

#define GLEW_ARB_shading_language_include GLEW_GET_VAR(__GLEW_ARB_shading_language_include)

#endif /* GL_ARB_shading_language_include */

/* -------------------- GL_ARB_shading_language_packing -------------------- */

#ifndef GL_ARB_shading_language_packing
#define GL_ARB_shading_language_packing 1

#define GLEW_ARB_shading_language_packing GLEW_GET_VAR(__GLEW_ARB_shading_language_packing)

#endif /* GL_ARB_shading_language_packing */

/* ----------------------------- GL_ARB_shadow ----------------------------- */

#ifndef GL_ARB_shadow
#define GL_ARB_shadow 1

#define GL_TEXTURE_COMPARE_MODE_ARB 0x884C
#define GL_TEXTURE_COMPARE_FUNC_ARB 0x884D
#define GL_COMPARE_R_TO_TEXTURE_ARB 0x884E

#define GLEW_ARB_shadow GLEW_GET_VAR(__GLEW_ARB_shadow)

#endif /* GL_ARB_shadow */

/* ------------------------- GL_ARB_shadow_ambient ------------------------- */

#ifndef GL_ARB_shadow_ambient
#define GL_ARB_shadow_ambient 1

#define GL_TEXTURE_COMPARE_FAIL_VALUE_ARB 0x80BF

#define GLEW_ARB_shadow_ambient GLEW_GET_VAR(__GLEW_ARB_shadow_ambient)

#endif /* GL_ARB_shadow_ambient */

/* -------------------------- GL_ARB_sparse_buffer ------------------------- */

#ifndef GL_ARB_sparse_buffer
#define GL_ARB_sparse_buffer 1

#define GL_SPARSE_STORAGE_BIT_ARB 0x0400
#define GL_SPARSE_BUFFER_PAGE_SIZE_ARB 0x82F8

typedef void (GLAPIENTRY * PFNGLBUFFERPAGECOMMITMENTARBPROC) (GLenum target, GLintptr offset, GLsizeiptr size, GLboolean commit);

#define glBufferPageCommitmentARB GLEW_GET_FUN(__glewBufferPageCommitmentARB)

#define GLEW_ARB_sparse_buffer GLEW_GET_VAR(__GLEW_ARB_sparse_buffer)

#endif /* GL_ARB_sparse_buffer */

/* ------------------------- GL_ARB_sparse_texture ------------------------- */

#ifndef GL_ARB_sparse_texture
#define GL_ARB_sparse_texture 1

#define GL_VIRTUAL_PAGE_SIZE_X_ARB 0x9195
#define GL_VIRTUAL_PAGE_SIZE_Y_ARB 0x9196
#define GL_VIRTUAL_PAGE_SIZE_Z_ARB 0x9197
#define GL_MAX_SPARSE_TEXTURE_SIZE_ARB 0x9198
#define GL_MAX_SPARSE_3D_TEXTURE_SIZE_ARB 0x9199
#define GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_ARB 0x919A
#define GL_TEXTURE_SPARSE_ARB 0x91A6
#define GL_VIRTUAL_PAGE_SIZE_INDEX_ARB 0x91A7
#define GL_NUM_VIRTUAL_PAGE_SIZES_ARB 0x91A8
#define GL_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_ARB 0x91A9
#define GL_NUM_SPARSE_LEVELS_ARB 0x91AA

typedef void (GLAPIENTRY * PFNGLTEXPAGECOMMITMENTARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit);

#define glTexPageCommitmentARB GLEW_GET_FUN(__glewTexPageCommitmentARB)

#define GLEW_ARB_sparse_texture GLEW_GET_VAR(__GLEW_ARB_sparse_texture)

#endif /* GL_ARB_sparse_texture */

/* ------------------------- GL_ARB_sparse_texture2 ------------------------ */

#ifndef GL_ARB_sparse_texture2
#define GL_ARB_sparse_texture2 1

#define GLEW_ARB_sparse_texture2 GLEW_GET_VAR(__GLEW_ARB_sparse_texture2)

#endif /* GL_ARB_sparse_texture2 */

/* ---------------------- GL_ARB_sparse_texture_clamp ---------------------- */

#ifndef GL_ARB_sparse_texture_clamp
#define GL_ARB_sparse_texture_clamp 1

#define GLEW_ARB_sparse_texture_clamp GLEW_GET_VAR(__GLEW_ARB_sparse_texture_clamp)

#endif /* GL_ARB_sparse_texture_clamp */

/* ------------------------ GL_ARB_spirv_extensions ------------------------ */

#ifndef GL_ARB_spirv_extensions
#define GL_ARB_spirv_extensions 1

#define GL_SPIR_V_EXTENSIONS 0x9553
#define GL_NUM_SPIR_V_EXTENSIONS 0x9554

#define GLEW_ARB_spirv_extensions GLEW_GET_VAR(__GLEW_ARB_spirv_extensions)

#endif /* GL_ARB_spirv_extensions */

/* ------------------------ GL_ARB_stencil_texturing ----------------------- */

#ifndef GL_ARB_stencil_texturing
#define GL_ARB_stencil_texturing 1

#define GL_DEPTH_STENCIL_TEXTURE_MODE 0x90EA

#define GLEW_ARB_stencil_texturing GLEW_GET_VAR(__GLEW_ARB_stencil_texturing)

#endif /* GL_ARB_stencil_texturing */

/* ------------------------------ GL_ARB_sync ------------------------------ */

#ifndef GL_ARB_sync
#define GL_ARB_sync 1

#define GL_SYNC_FLUSH_COMMANDS_BIT 0x00000001
#define GL_MAX_SERVER_WAIT_TIMEOUT 0x9111
#define GL_OBJECT_TYPE 0x9112
#define GL_SYNC_CONDITION 0x9113
#define GL_SYNC_STATUS 0x9114
#define GL_SYNC_FLAGS 0x9115
#define GL_SYNC_FENCE 0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE 0x9117
#define GL_UNSIGNALED 0x9118
#define GL_SIGNALED 0x9119
#define GL_ALREADY_SIGNALED 0x911A
#define GL_TIMEOUT_EXPIRED 0x911B
#define GL_CONDITION_SATISFIED 0x911C
#define GL_WAIT_FAILED 0x911D
#define GL_TIMEOUT_IGNORED 0xFFFFFFFFFFFFFFFF

typedef GLenum (GLAPIENTRY * PFNGLCLIENTWAITSYNCPROC) (GLsync GLsync,GLbitfield flags,GLuint64 timeout);
typedef void (GLAPIENTRY * PFNGLDELETESYNCPROC) (GLsync GLsync);
typedef GLsync (GLAPIENTRY * PFNGLFENCESYNCPROC) (GLenum condition,GLbitfield flags);
typedef void (GLAPIENTRY * PFNGLGETINTEGER64VPROC) (GLenum pname, GLint64* params);
typedef void (GLAPIENTRY * PFNGLGETSYNCIVPROC) (GLsync GLsync,GLenum pname,GLsizei bufSize,GLsizei* length, GLint *values);
typedef GLboolean (GLAPIENTRY * PFNGLISSYNCPROC) (GLsync GLsync);
typedef void (GLAPIENTRY * PFNGLWAITSYNCPROC) (GLsync GLsync,GLbitfield flags,GLuint64 timeout);

#define glClientWaitSync GLEW_GET_FUN(__glewClientWaitSync)
#define glDeleteSync GLEW_GET_FUN(__glewDeleteSync)
#define glFenceSync GLEW_GET_FUN(__glewFenceSync)
#define glGetInteger64v GLEW_GET_FUN(__glewGetInteger64v)
#define glGetSynciv GLEW_GET_FUN(__glewGetSynciv)
#define glIsSync GLEW_GET_FUN(__glewIsSync)
#define glWaitSync GLEW_GET_FUN(__glewWaitSync)

#define GLEW_ARB_sync GLEW_GET_VAR(__GLEW_ARB_sync)

#endif /* GL_ARB_sync */

/* ----------------------- GL_ARB_tessellation_shader ---------------------- */

#ifndef GL_ARB_tessellation_shader
#define GL_ARB_tessellation_shader 1

#define GL_PATCHES 0xE
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER 0x84F0
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER 0x84F1
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS 0x886C
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS 0x886D
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E1E
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E1F
#define GL_PATCH_VERTICES 0x8E72
#define GL_PATCH_DEFAULT_INNER_LEVEL 0x8E73
#define GL_PATCH_DEFAULT_OUTER_LEVEL 0x8E74
#define GL_TESS_CONTROL_OUTPUT_VERTICES 0x8E75
#define GL_TESS_GEN_MODE 0x8E76
#define GL_TESS_GEN_SPACING 0x8E77
#define GL_TESS_GEN_VERTEX_ORDER 0x8E78
#define GL_TESS_GEN_POINT_MODE 0x8E79
#define GL_ISOLINES 0x8E7A
#define GL_FRACTIONAL_ODD 0x8E7B
#define GL_FRACTIONAL_EVEN 0x8E7C
#define GL_MAX_PATCH_VERTICES 0x8E7D
#define GL_MAX_TESS_GEN_LEVEL 0x8E7E
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E7F
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E80
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS 0x8E81
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS 0x8E82
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS 0x8E83
#define GL_MAX_TESS_PATCH_COMPONENTS 0x8E84
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS 0x8E85
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS 0x8E86
#define GL_TESS_EVALUATION_SHADER 0x8E87
#define GL_TESS_CONTROL_SHADER 0x8E88
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS 0x8E89
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS 0x8E8A

typedef void (GLAPIENTRY * PFNGLPATCHPARAMETERFVPROC) (GLenum pname, const GLfloat* values);
typedef void (GLAPIENTRY * PFNGLPATCHPARAMETERIPROC) (GLenum pname, GLint value);

#define glPatchParameterfv GLEW_GET_FUN(__glewPatchParameterfv)
#define glPatchParameteri GLEW_GET_FUN(__glewPatchParameteri)

#define GLEW_ARB_tessellation_shader GLEW_GET_VAR(__GLEW_ARB_tessellation_shader)

#endif /* GL_ARB_tessellation_shader */

/* ------------------------- GL_ARB_texture_barrier ------------------------ */

#ifndef GL_ARB_texture_barrier
#define GL_ARB_texture_barrier 1

typedef void (GLAPIENTRY * PFNGLTEXTUREBARRIERPROC) (void);

#define glTextureBarrier GLEW_GET_FUN(__glewTextureBarrier)

#define GLEW_ARB_texture_barrier GLEW_GET_VAR(__GLEW_ARB_texture_barrier)

#endif /* GL_ARB_texture_barrier */

/* ---------------------- GL_ARB_texture_border_clamp ---------------------- */

#ifndef GL_ARB_texture_border_clamp
#define GL_ARB_texture_border_clamp 1

#define GL_CLAMP_TO_BORDER_ARB 0x812D

#define GLEW_ARB_texture_border_clamp GLEW_GET_VAR(__GLEW_ARB_texture_border_clamp)

#endif /* GL_ARB_texture_border_clamp */

/* ---------------------- GL_ARB_texture_buffer_object --------------------- */

#ifndef GL_ARB_texture_buffer_object
#define GL_ARB_texture_buffer_object 1

#define GL_TEXTURE_BUFFER_ARB 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE_ARB 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER_ARB 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING_ARB 0x8C2D
#define GL_TEXTURE_BUFFER_FORMAT_ARB 0x8C2E

typedef void (GLAPIENTRY * PFNGLTEXBUFFERARBPROC) (GLenum target, GLenum internalformat, GLuint buffer);

#define glTexBufferARB GLEW_GET_FUN(__glewTexBufferARB)

#define GLEW_ARB_texture_buffer_object GLEW_GET_VAR(__GLEW_ARB_texture_buffer_object)

#endif /* GL_ARB_texture_buffer_object */

/* ------------------- GL_ARB_texture_buffer_object_rgb32 ------------------ */

#ifndef GL_ARB_texture_buffer_object_rgb32
#define GL_ARB_texture_buffer_object_rgb32 1

#define GLEW_ARB_texture_buffer_object_rgb32 GLEW_GET_VAR(__GLEW_ARB_texture_buffer_object_rgb32)

#endif /* GL_ARB_texture_buffer_object_rgb32 */

/* ---------------------- GL_ARB_texture_buffer_range ---------------------- */

#ifndef GL_ARB_texture_buffer_range
#define GL_ARB_texture_buffer_range 1

#define GL_TEXTURE_BUFFER_OFFSET 0x919D
#define GL_TEXTURE_BUFFER_SIZE 0x919E
#define GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT 0x919F

typedef void (GLAPIENTRY * PFNGLTEXBUFFERRANGEPROC) (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLTEXTUREBUFFERRANGEEXTPROC) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);

#define glTexBufferRange GLEW_GET_FUN(__glewTexBufferRange)
#define glTextureBufferRangeEXT GLEW_GET_FUN(__glewTextureBufferRangeEXT)

#define GLEW_ARB_texture_buffer_range GLEW_GET_VAR(__GLEW_ARB_texture_buffer_range)

#endif /* GL_ARB_texture_buffer_range */

/* ----------------------- GL_ARB_texture_compression ---------------------- */

#ifndef GL_ARB_texture_compression
#define GL_ARB_texture_compression 1

#define GL_COMPRESSED_ALPHA_ARB 0x84E9
#define GL_COMPRESSED_LUMINANCE_ARB 0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA_ARB 0x84EB
#define GL_COMPRESSED_INTENSITY_ARB 0x84EC
#define GL_COMPRESSED_RGB_ARB 0x84ED
#define GL_COMPRESSED_RGBA_ARB 0x84EE
#define GL_TEXTURE_COMPRESSION_HINT_ARB 0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB 0x86A0
#define GL_TEXTURE_COMPRESSED_ARB 0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS_ARB 0x86A3

typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE1DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE3DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXIMAGEARBPROC) (GLenum target, GLint lod, void *img);

#define glCompressedTexImage1DARB GLEW_GET_FUN(__glewCompressedTexImage1DARB)
#define glCompressedTexImage2DARB GLEW_GET_FUN(__glewCompressedTexImage2DARB)
#define glCompressedTexImage3DARB GLEW_GET_FUN(__glewCompressedTexImage3DARB)
#define glCompressedTexSubImage1DARB GLEW_GET_FUN(__glewCompressedTexSubImage1DARB)
#define glCompressedTexSubImage2DARB GLEW_GET_FUN(__glewCompressedTexSubImage2DARB)
#define glCompressedTexSubImage3DARB GLEW_GET_FUN(__glewCompressedTexSubImage3DARB)
#define glGetCompressedTexImageARB GLEW_GET_FUN(__glewGetCompressedTexImageARB)

#define GLEW_ARB_texture_compression GLEW_GET_VAR(__GLEW_ARB_texture_compression)

#endif /* GL_ARB_texture_compression */

/* -------------------- GL_ARB_texture_compression_bptc -------------------- */

#ifndef GL_ARB_texture_compression_bptc
#define GL_ARB_texture_compression_bptc 1

#define GL_COMPRESSED_RGBA_BPTC_UNORM_ARB 0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB 0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB 0x8E8F

#define GLEW_ARB_texture_compression_bptc GLEW_GET_VAR(__GLEW_ARB_texture_compression_bptc)

#endif /* GL_ARB_texture_compression_bptc */

/* -------------------- GL_ARB_texture_compression_rgtc -------------------- */

#ifndef GL_ARB_texture_compression_rgtc
#define GL_ARB_texture_compression_rgtc 1

#define GL_COMPRESSED_RED_RGTC1 0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1 0x8DBC
#define GL_COMPRESSED_RG_RGTC2 0x8DBD
#define GL_COMPRESSED_SIGNED_RG_RGTC2 0x8DBE

#define GLEW_ARB_texture_compression_rgtc GLEW_GET_VAR(__GLEW_ARB_texture_compression_rgtc)

#endif /* GL_ARB_texture_compression_rgtc */

/* ------------------------ GL_ARB_texture_cube_map ------------------------ */

#ifndef GL_ARB_texture_cube_map
#define GL_ARB_texture_cube_map 1

#define GL_NORMAL_MAP_ARB 0x8511
#define GL_REFLECTION_MAP_ARB 0x8512
#define GL_TEXTURE_CUBE_MAP_ARB 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB 0x851C

#define GLEW_ARB_texture_cube_map GLEW_GET_VAR(__GLEW_ARB_texture_cube_map)

#endif /* GL_ARB_texture_cube_map */

/* --------------------- GL_ARB_texture_cube_map_array --------------------- */

#ifndef GL_ARB_texture_cube_map_array
#define GL_ARB_texture_cube_map_array 1

#define GL_TEXTURE_CUBE_MAP_ARRAY_ARB 0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY_ARB 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY_ARB 0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_ARB 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900F

#define GLEW_ARB_texture_cube_map_array GLEW_GET_VAR(__GLEW_ARB_texture_cube_map_array)

#endif /* GL_ARB_texture_cube_map_array */

/* ------------------------- GL_ARB_texture_env_add ------------------------ */

#ifndef GL_ARB_texture_env_add
#define GL_ARB_texture_env_add 1

#define GLEW_ARB_texture_env_add GLEW_GET_VAR(__GLEW_ARB_texture_env_add)

#endif /* GL_ARB_texture_env_add */

/* ----------------------- GL_ARB_texture_env_combine ---------------------- */

#ifndef GL_ARB_texture_env_combine
#define GL_ARB_texture_env_combine 1

#define GL_SUBTRACT_ARB 0x84E7
#define GL_COMBINE_ARB 0x8570
#define GL_COMBINE_RGB_ARB 0x8571
#define GL_COMBINE_ALPHA_ARB 0x8572
#define GL_RGB_SCALE_ARB 0x8573
#define GL_ADD_SIGNED_ARB 0x8574
#define GL_INTERPOLATE_ARB 0x8575
#define GL_CONSTANT_ARB 0x8576
#define GL_PRIMARY_COLOR_ARB 0x8577
#define GL_PREVIOUS_ARB 0x8578
#define GL_SOURCE0_RGB_ARB 0x8580
#define GL_SOURCE1_RGB_ARB 0x8581
#define GL_SOURCE2_RGB_ARB 0x8582
#define GL_SOURCE0_ALPHA_ARB 0x8588
#define GL_SOURCE1_ALPHA_ARB 0x8589
#define GL_SOURCE2_ALPHA_ARB 0x858A
#define GL_OPERAND0_RGB_ARB 0x8590
#define GL_OPERAND1_RGB_ARB 0x8591
#define GL_OPERAND2_RGB_ARB 0x8592
#define GL_OPERAND0_ALPHA_ARB 0x8598
#define GL_OPERAND1_ALPHA_ARB 0x8599
#define GL_OPERAND2_ALPHA_ARB 0x859A

#define GLEW_ARB_texture_env_combine GLEW_GET_VAR(__GLEW_ARB_texture_env_combine)

#endif /* GL_ARB_texture_env_combine */

/* ---------------------- GL_ARB_texture_env_crossbar ---------------------- */

#ifndef GL_ARB_texture_env_crossbar
#define GL_ARB_texture_env_crossbar 1

#define GLEW_ARB_texture_env_crossbar GLEW_GET_VAR(__GLEW_ARB_texture_env_crossbar)

#endif /* GL_ARB_texture_env_crossbar */

/* ------------------------ GL_ARB_texture_env_dot3 ------------------------ */

#ifndef GL_ARB_texture_env_dot3
#define GL_ARB_texture_env_dot3 1

#define GL_DOT3_RGB_ARB 0x86AE
#define GL_DOT3_RGBA_ARB 0x86AF

#define GLEW_ARB_texture_env_dot3 GLEW_GET_VAR(__GLEW_ARB_texture_env_dot3)

#endif /* GL_ARB_texture_env_dot3 */

/* ------------------- GL_ARB_texture_filter_anisotropic ------------------- */

#ifndef GL_ARB_texture_filter_anisotropic
#define GL_ARB_texture_filter_anisotropic 1

#define GL_TEXTURE_MAX_ANISOTROPY 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY 0x84FF

#define GLEW_ARB_texture_filter_anisotropic GLEW_GET_VAR(__GLEW_ARB_texture_filter_anisotropic)

#endif /* GL_ARB_texture_filter_anisotropic */

/* ---------------------- GL_ARB_texture_filter_minmax --------------------- */

#ifndef GL_ARB_texture_filter_minmax
#define GL_ARB_texture_filter_minmax 1

#define GL_TEXTURE_REDUCTION_MODE_ARB 0x9366
#define GL_WEIGHTED_AVERAGE_ARB 0x9367

#define GLEW_ARB_texture_filter_minmax GLEW_GET_VAR(__GLEW_ARB_texture_filter_minmax)

#endif /* GL_ARB_texture_filter_minmax */

/* -------------------------- GL_ARB_texture_float ------------------------- */

#ifndef GL_ARB_texture_float
#define GL_ARB_texture_float 1

#define GL_RGBA32F_ARB 0x8814
#define GL_RGB32F_ARB 0x8815
#define GL_ALPHA32F_ARB 0x8816
#define GL_INTENSITY32F_ARB 0x8817
#define GL_LUMINANCE32F_ARB 0x8818
#define GL_LUMINANCE_ALPHA32F_ARB 0x8819
#define GL_RGBA16F_ARB 0x881A
#define GL_RGB16F_ARB 0x881B
#define GL_ALPHA16F_ARB 0x881C
#define GL_INTENSITY16F_ARB 0x881D
#define GL_LUMINANCE16F_ARB 0x881E
#define GL_LUMINANCE_ALPHA16F_ARB 0x881F
#define GL_TEXTURE_RED_TYPE_ARB 0x8C10
#define GL_TEXTURE_GREEN_TYPE_ARB 0x8C11
#define GL_TEXTURE_BLUE_TYPE_ARB 0x8C12
#define GL_TEXTURE_ALPHA_TYPE_ARB 0x8C13
#define GL_TEXTURE_LUMINANCE_TYPE_ARB 0x8C14
#define GL_TEXTURE_INTENSITY_TYPE_ARB 0x8C15
#define GL_TEXTURE_DEPTH_TYPE_ARB 0x8C16
#define GL_UNSIGNED_NORMALIZED_ARB 0x8C17

#define GLEW_ARB_texture_float GLEW_GET_VAR(__GLEW_ARB_texture_float)

#endif /* GL_ARB_texture_float */

/* ------------------------- GL_ARB_texture_gather ------------------------- */

#ifndef GL_ARB_texture_gather
#define GL_ARB_texture_gather 1

#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_ARB 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_ARB 0x8E5F
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS_ARB 0x8F9F

#define GLEW_ARB_texture_gather GLEW_GET_VAR(__GLEW_ARB_texture_gather)

#endif /* GL_ARB_texture_gather */

/* ------------------ GL_ARB_texture_mirror_clamp_to_edge ------------------ */

#ifndef GL_ARB_texture_mirror_clamp_to_edge
#define GL_ARB_texture_mirror_clamp_to_edge 1

#define GL_MIRROR_CLAMP_TO_EDGE 0x8743

#define GLEW_ARB_texture_mirror_clamp_to_edge GLEW_GET_VAR(__GLEW_ARB_texture_mirror_clamp_to_edge)

#endif /* GL_ARB_texture_mirror_clamp_to_edge */

/* --------------------- GL_ARB_texture_mirrored_repeat -------------------- */

#ifndef GL_ARB_texture_mirrored_repeat
#define GL_ARB_texture_mirrored_repeat 1

#define GL_MIRRORED_REPEAT_ARB 0x8370

#define GLEW_ARB_texture_mirrored_repeat GLEW_GET_VAR(__GLEW_ARB_texture_mirrored_repeat)

#endif /* GL_ARB_texture_mirrored_repeat */

/* ----------------------- GL_ARB_texture_multisample ---------------------- */

#ifndef GL_ARB_texture_multisample
#define GL_ARB_texture_multisample 1

#define GL_SAMPLE_POSITION 0x8E50
#define GL_SAMPLE_MASK 0x8E51
#define GL_SAMPLE_MASK_VALUE 0x8E52
#define GL_MAX_SAMPLE_MASK_WORDS 0x8E59
#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE 0x9101
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9103
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY 0x9105
#define GL_TEXTURE_SAMPLES 0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
#define GL_SAMPLER_2D_MULTISAMPLE 0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE 0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
#define GL_MAX_COLOR_TEXTURE_SAMPLES 0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES 0x910F
#define GL_MAX_INTEGER_SAMPLES 0x9110

typedef void (GLAPIENTRY * PFNGLGETMULTISAMPLEFVPROC) (GLenum pname, GLuint index, GLfloat* val);
typedef void (GLAPIENTRY * PFNGLSAMPLEMASKIPROC) (GLuint index, GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE2DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE3DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);

#define glGetMultisamplefv GLEW_GET_FUN(__glewGetMultisamplefv)
#define glSampleMaski GLEW_GET_FUN(__glewSampleMaski)
#define glTexImage2DMultisample GLEW_GET_FUN(__glewTexImage2DMultisample)
#define glTexImage3DMultisample GLEW_GET_FUN(__glewTexImage3DMultisample)

#define GLEW_ARB_texture_multisample GLEW_GET_VAR(__GLEW_ARB_texture_multisample)

#endif /* GL_ARB_texture_multisample */

/* -------------------- GL_ARB_texture_non_power_of_two -------------------- */

#ifndef GL_ARB_texture_non_power_of_two
#define GL_ARB_texture_non_power_of_two 1

#define GLEW_ARB_texture_non_power_of_two GLEW_GET_VAR(__GLEW_ARB_texture_non_power_of_two)

#endif /* GL_ARB_texture_non_power_of_two */

/* ---------------------- GL_ARB_texture_query_levels ---------------------- */

#ifndef GL_ARB_texture_query_levels
#define GL_ARB_texture_query_levels 1

#define GLEW_ARB_texture_query_levels GLEW_GET_VAR(__GLEW_ARB_texture_query_levels)

#endif /* GL_ARB_texture_query_levels */

/* ------------------------ GL_ARB_texture_query_lod ----------------------- */

#ifndef GL_ARB_texture_query_lod
#define GL_ARB_texture_query_lod 1

#define GLEW_ARB_texture_query_lod GLEW_GET_VAR(__GLEW_ARB_texture_query_lod)

#endif /* GL_ARB_texture_query_lod */

/* ------------------------ GL_ARB_texture_rectangle ----------------------- */

#ifndef GL_ARB_texture_rectangle
#define GL_ARB_texture_rectangle 1

#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_ARB 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_ARB 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB 0x84F8
#define GL_SAMPLER_2D_RECT_ARB 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW_ARB 0x8B64

#define GLEW_ARB_texture_rectangle GLEW_GET_VAR(__GLEW_ARB_texture_rectangle)

#endif /* GL_ARB_texture_rectangle */

/* --------------------------- GL_ARB_texture_rg --------------------------- */

#ifndef GL_ARB_texture_rg
#define GL_ARB_texture_rg 1

#define GL_COMPRESSED_RED 0x8225
#define GL_COMPRESSED_RG 0x8226
#define GL_RG 0x8227
#define GL_RG_INTEGER 0x8228
#define GL_R8 0x8229
#define GL_R16 0x822A
#define GL_RG8 0x822B
#define GL_RG16 0x822C
#define GL_R16F 0x822D
#define GL_R32F 0x822E
#define GL_RG16F 0x822F
#define GL_RG32F 0x8230
#define GL_R8I 0x8231
#define GL_R8UI 0x8232
#define GL_R16I 0x8233
#define GL_R16UI 0x8234
#define GL_R32I 0x8235
#define GL_R32UI 0x8236
#define GL_RG8I 0x8237
#define GL_RG8UI 0x8238
#define GL_RG16I 0x8239
#define GL_RG16UI 0x823A
#define GL_RG32I 0x823B
#define GL_RG32UI 0x823C

#define GLEW_ARB_texture_rg GLEW_GET_VAR(__GLEW_ARB_texture_rg)

#endif /* GL_ARB_texture_rg */

/* ----------------------- GL_ARB_texture_rgb10_a2ui ----------------------- */

#ifndef GL_ARB_texture_rgb10_a2ui
#define GL_ARB_texture_rgb10_a2ui 1

#define GL_RGB10_A2UI 0x906F

#define GLEW_ARB_texture_rgb10_a2ui GLEW_GET_VAR(__GLEW_ARB_texture_rgb10_a2ui)

#endif /* GL_ARB_texture_rgb10_a2ui */

/* ------------------------ GL_ARB_texture_stencil8 ------------------------ */

#ifndef GL_ARB_texture_stencil8
#define GL_ARB_texture_stencil8 1

#define GL_STENCIL_INDEX 0x1901
#define GL_STENCIL_INDEX8 0x8D48

#define GLEW_ARB_texture_stencil8 GLEW_GET_VAR(__GLEW_ARB_texture_stencil8)

#endif /* GL_ARB_texture_stencil8 */

/* ------------------------- GL_ARB_texture_storage ------------------------ */

#ifndef GL_ARB_texture_storage
#define GL_ARB_texture_storage 1

#define GL_TEXTURE_IMMUTABLE_FORMAT 0x912F

typedef void (GLAPIENTRY * PFNGLTEXSTORAGE1DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGE2DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGE3DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

#define glTexStorage1D GLEW_GET_FUN(__glewTexStorage1D)
#define glTexStorage2D GLEW_GET_FUN(__glewTexStorage2D)
#define glTexStorage3D GLEW_GET_FUN(__glewTexStorage3D)

#define GLEW_ARB_texture_storage GLEW_GET_VAR(__GLEW_ARB_texture_storage)

#endif /* GL_ARB_texture_storage */

/* ------------------- GL_ARB_texture_storage_multisample ------------------ */

#ifndef GL_ARB_texture_storage_multisample
#define GL_ARB_texture_storage_multisample 1

typedef void (GLAPIENTRY * PFNGLTEXSTORAGE2DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGE3DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE2DMULTISAMPLEEXTPROC) (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE3DMULTISAMPLEEXTPROC) (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);

#define glTexStorage2DMultisample GLEW_GET_FUN(__glewTexStorage2DMultisample)
#define glTexStorage3DMultisample GLEW_GET_FUN(__glewTexStorage3DMultisample)
#define glTextureStorage2DMultisampleEXT GLEW_GET_FUN(__glewTextureStorage2DMultisampleEXT)
#define glTextureStorage3DMultisampleEXT GLEW_GET_FUN(__glewTextureStorage3DMultisampleEXT)

#define GLEW_ARB_texture_storage_multisample GLEW_GET_VAR(__GLEW_ARB_texture_storage_multisample)

#endif /* GL_ARB_texture_storage_multisample */

/* ------------------------- GL_ARB_texture_swizzle ------------------------ */

#ifndef GL_ARB_texture_swizzle
#define GL_ARB_texture_swizzle 1

#define GL_TEXTURE_SWIZZLE_R 0x8E42
#define GL_TEXTURE_SWIZZLE_G 0x8E43
#define GL_TEXTURE_SWIZZLE_B 0x8E44
#define GL_TEXTURE_SWIZZLE_A 0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46

#define GLEW_ARB_texture_swizzle GLEW_GET_VAR(__GLEW_ARB_texture_swizzle)

#endif /* GL_ARB_texture_swizzle */

/* -------------------------- GL_ARB_texture_view -------------------------- */

#ifndef GL_ARB_texture_view
#define GL_ARB_texture_view 1

#define GL_TEXTURE_VIEW_MIN_LEVEL 0x82DB
#define GL_TEXTURE_VIEW_NUM_LEVELS 0x82DC
#define GL_TEXTURE_VIEW_MIN_LAYER 0x82DD
#define GL_TEXTURE_VIEW_NUM_LAYERS 0x82DE
#define GL_TEXTURE_IMMUTABLE_LEVELS 0x82DF

typedef void (GLAPIENTRY * PFNGLTEXTUREVIEWPROC) (GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);

#define glTextureView GLEW_GET_FUN(__glewTextureView)

#define GLEW_ARB_texture_view GLEW_GET_VAR(__GLEW_ARB_texture_view)

#endif /* GL_ARB_texture_view */

/* --------------------------- GL_ARB_timer_query -------------------------- */

#ifndef GL_ARB_timer_query
#define GL_ARB_timer_query 1

#define GL_TIME_ELAPSED 0x88BF
#define GL_TIMESTAMP 0x8E28

typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTI64VPROC) (GLuint id, GLenum pname, GLint64* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUI64VPROC) (GLuint id, GLenum pname, GLuint64* params);
typedef void (GLAPIENTRY * PFNGLQUERYCOUNTERPROC) (GLuint id, GLenum target);

#define glGetQueryObjecti64v GLEW_GET_FUN(__glewGetQueryObjecti64v)
#define glGetQueryObjectui64v GLEW_GET_FUN(__glewGetQueryObjectui64v)
#define glQueryCounter GLEW_GET_FUN(__glewQueryCounter)

#define GLEW_ARB_timer_query GLEW_GET_VAR(__GLEW_ARB_timer_query)

#endif /* GL_ARB_timer_query */

/* ----------------------- GL_ARB_transform_feedback2 ---------------------- */

#ifndef GL_ARB_transform_feedback2
#define GL_ARB_transform_feedback2 1

#define GL_TRANSFORM_FEEDBACK 0x8E22
#define GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED 0x8E23
#define GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE 0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING 0x8E25

typedef void (GLAPIENTRY * PFNGLBINDTRANSFORMFEEDBACKPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETETRANSFORMFEEDBACKSPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKPROC) (GLenum mode, GLuint id);
typedef void (GLAPIENTRY * PFNGLGENTRANSFORMFEEDBACKSPROC) (GLsizei n, GLuint* ids);
typedef GLboolean (GLAPIENTRY * PFNGLISTRANSFORMFEEDBACKPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLPAUSETRANSFORMFEEDBACKPROC) (void);
typedef void (GLAPIENTRY * PFNGLRESUMETRANSFORMFEEDBACKPROC) (void);

#define glBindTransformFeedback GLEW_GET_FUN(__glewBindTransformFeedback)
#define glDeleteTransformFeedbacks GLEW_GET_FUN(__glewDeleteTransformFeedbacks)
#define glDrawTransformFeedback GLEW_GET_FUN(__glewDrawTransformFeedback)
#define glGenTransformFeedbacks GLEW_GET_FUN(__glewGenTransformFeedbacks)
#define glIsTransformFeedback GLEW_GET_FUN(__glewIsTransformFeedback)
#define glPauseTransformFeedback GLEW_GET_FUN(__glewPauseTransformFeedback)
#define glResumeTransformFeedback GLEW_GET_FUN(__glewResumeTransformFeedback)

#define GLEW_ARB_transform_feedback2 GLEW_GET_VAR(__GLEW_ARB_transform_feedback2)

#endif /* GL_ARB_transform_feedback2 */

/* ----------------------- GL_ARB_transform_feedback3 ---------------------- */

#ifndef GL_ARB_transform_feedback3
#define GL_ARB_transform_feedback3 1

#define GL_MAX_TRANSFORM_FEEDBACK_BUFFERS 0x8E70
#define GL_MAX_VERTEX_STREAMS 0x8E71

typedef void (GLAPIENTRY * PFNGLBEGINQUERYINDEXEDPROC) (GLenum target, GLuint index, GLuint id);
typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC) (GLenum mode, GLuint id, GLuint stream);
typedef void (GLAPIENTRY * PFNGLENDQUERYINDEXEDPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLGETQUERYINDEXEDIVPROC) (GLenum target, GLuint index, GLenum pname, GLint* params);

#define glBeginQueryIndexed GLEW_GET_FUN(__glewBeginQueryIndexed)
#define glDrawTransformFeedbackStream GLEW_GET_FUN(__glewDrawTransformFeedbackStream)
#define glEndQueryIndexed GLEW_GET_FUN(__glewEndQueryIndexed)
#define glGetQueryIndexediv GLEW_GET_FUN(__glewGetQueryIndexediv)

#define GLEW_ARB_transform_feedback3 GLEW_GET_VAR(__GLEW_ARB_transform_feedback3)

#endif /* GL_ARB_transform_feedback3 */

/* ------------------ GL_ARB_transform_feedback_instanced ------------------ */

#ifndef GL_ARB_transform_feedback_instanced
#define GL_ARB_transform_feedback_instanced 1

typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC) (GLenum mode, GLuint id, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC) (GLenum mode, GLuint id, GLuint stream, GLsizei primcount);

#define glDrawTransformFeedbackInstanced GLEW_GET_FUN(__glewDrawTransformFeedbackInstanced)
#define glDrawTransformFeedbackStreamInstanced GLEW_GET_FUN(__glewDrawTransformFeedbackStreamInstanced)

#define GLEW_ARB_transform_feedback_instanced GLEW_GET_VAR(__GLEW_ARB_transform_feedback_instanced)

#endif /* GL_ARB_transform_feedback_instanced */

/* ---------------- GL_ARB_transform_feedback_overflow_query --------------- */

#ifndef GL_ARB_transform_feedback_overflow_query
#define GL_ARB_transform_feedback_overflow_query 1

#define GL_TRANSFORM_FEEDBACK_OVERFLOW_ARB 0x82EC
#define GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW_ARB 0x82ED

#define GLEW_ARB_transform_feedback_overflow_query GLEW_GET_VAR(__GLEW_ARB_transform_feedback_overflow_query)

#endif /* GL_ARB_transform_feedback_overflow_query */

/* ------------------------ GL_ARB_transpose_matrix ------------------------ */

#ifndef GL_ARB_transpose_matrix
#define GL_ARB_transpose_matrix 1

#define GL_TRANSPOSE_MODELVIEW_MATRIX_ARB 0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX_ARB 0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX_ARB 0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX_ARB 0x84E6

typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXDARBPROC) (GLdouble m[16]);
typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXFARBPROC) (GLfloat m[16]);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXDARBPROC) (GLdouble m[16]);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXFARBPROC) (GLfloat m[16]);

#define glLoadTransposeMatrixdARB GLEW_GET_FUN(__glewLoadTransposeMatrixdARB)
#define glLoadTransposeMatrixfARB GLEW_GET_FUN(__glewLoadTransposeMatrixfARB)
#define glMultTransposeMatrixdARB GLEW_GET_FUN(__glewMultTransposeMatrixdARB)
#define glMultTransposeMatrixfARB GLEW_GET_FUN(__glewMultTransposeMatrixfARB)

#define GLEW_ARB_transpose_matrix GLEW_GET_VAR(__GLEW_ARB_transpose_matrix)

#endif /* GL_ARB_transpose_matrix */

/* ---------------------- GL_ARB_uniform_buffer_object --------------------- */

#ifndef GL_ARB_uniform_buffer_object
#define GL_ARB_uniform_buffer_object 1

#define GL_UNIFORM_BUFFER 0x8A11
#define GL_UNIFORM_BUFFER_BINDING 0x8A28
#define GL_UNIFORM_BUFFER_START 0x8A29
#define GL_UNIFORM_BUFFER_SIZE 0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS 0x8A2B
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS 0x8A2C
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS 0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS 0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS 0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE 0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS 0x8A32
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS 0x8A36
#define GL_UNIFORM_TYPE 0x8A37
#define GL_UNIFORM_SIZE 0x8A38
#define GL_UNIFORM_NAME_LENGTH 0x8A39
#define GL_UNIFORM_BLOCK_INDEX 0x8A3A
#define GL_UNIFORM_OFFSET 0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE 0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE 0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR 0x8A3E
#define GL_UNIFORM_BLOCK_BINDING 0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE 0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH 0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS 0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER 0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX 0xFFFFFFFF

typedef void (GLAPIENTRY * PFNGLBINDBUFFERBASEPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERRANGEPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMBLOCKIVPROC) (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMNAMEPROC) (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMSIVPROC) (GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETINTEGERI_VPROC) (GLenum target, GLuint index, GLint* data);
typedef GLuint (GLAPIENTRY * PFNGLGETUNIFORMBLOCKINDEXPROC) (GLuint program, const GLchar* uniformBlockName);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMINDICESPROC) (GLuint program, GLsizei uniformCount, const GLchar* const * uniformNames, GLuint* uniformIndices);
typedef void (GLAPIENTRY * PFNGLUNIFORMBLOCKBINDINGPROC) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

#define glBindBufferBase GLEW_GET_FUN(__glewBindBufferBase)
#define glBindBufferRange GLEW_GET_FUN(__glewBindBufferRange)
#define glGetActiveUniformBlockName GLEW_GET_FUN(__glewGetActiveUniformBlockName)
#define glGetActiveUniformBlockiv GLEW_GET_FUN(__glewGetActiveUniformBlockiv)
#define glGetActiveUniformName GLEW_GET_FUN(__glewGetActiveUniformName)
#define glGetActiveUniformsiv GLEW_GET_FUN(__glewGetActiveUniformsiv)
#define glGetIntegeri_v GLEW_GET_FUN(__glewGetIntegeri_v)
#define glGetUniformBlockIndex GLEW_GET_FUN(__glewGetUniformBlockIndex)
#define glGetUniformIndices GLEW_GET_FUN(__glewGetUniformIndices)
#define glUniformBlockBinding GLEW_GET_FUN(__glewUniformBlockBinding)

#define GLEW_ARB_uniform_buffer_object GLEW_GET_VAR(__GLEW_ARB_uniform_buffer_object)

#endif /* GL_ARB_uniform_buffer_object */

/* ------------------------ GL_ARB_vertex_array_bgra ----------------------- */

#ifndef GL_ARB_vertex_array_bgra
#define GL_ARB_vertex_array_bgra 1

#define GL_BGRA 0x80E1

#define GLEW_ARB_vertex_array_bgra GLEW_GET_VAR(__GLEW_ARB_vertex_array_bgra)

#endif /* GL_ARB_vertex_array_bgra */

/* ----------------------- GL_ARB_vertex_array_object ---------------------- */

#ifndef GL_ARB_vertex_array_object
#define GL_ARB_vertex_array_object 1

#define GL_VERTEX_ARRAY_BINDING 0x85B5

typedef void (GLAPIENTRY * PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (GLAPIENTRY * PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint* arrays);
typedef void (GLAPIENTRY * PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
typedef GLboolean (GLAPIENTRY * PFNGLISVERTEXARRAYPROC) (GLuint array);

#define glBindVertexArray GLEW_GET_FUN(__glewBindVertexArray)
#define glDeleteVertexArrays GLEW_GET_FUN(__glewDeleteVertexArrays)
#define glGenVertexArrays GLEW_GET_FUN(__glewGenVertexArrays)
#define glIsVertexArray GLEW_GET_FUN(__glewIsVertexArray)

#define GLEW_ARB_vertex_array_object GLEW_GET_VAR(__GLEW_ARB_vertex_array_object)

#endif /* GL_ARB_vertex_array_object */

/* ----------------------- GL_ARB_vertex_attrib_64bit ---------------------- */

#ifndef GL_ARB_vertex_attrib_64bit
#define GL_ARB_vertex_attrib_64bit 1

typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBLDVPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1DPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2DPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBLPOINTERPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);

#define glGetVertexAttribLdv GLEW_GET_FUN(__glewGetVertexAttribLdv)
#define glVertexAttribL1d GLEW_GET_FUN(__glewVertexAttribL1d)
#define glVertexAttribL1dv GLEW_GET_FUN(__glewVertexAttribL1dv)
#define glVertexAttribL2d GLEW_GET_FUN(__glewVertexAttribL2d)
#define glVertexAttribL2dv GLEW_GET_FUN(__glewVertexAttribL2dv)
#define glVertexAttribL3d GLEW_GET_FUN(__glewVertexAttribL3d)
#define glVertexAttribL3dv GLEW_GET_FUN(__glewVertexAttribL3dv)
#define glVertexAttribL4d GLEW_GET_FUN(__glewVertexAttribL4d)
#define glVertexAttribL4dv GLEW_GET_FUN(__glewVertexAttribL4dv)
#define glVertexAttribLPointer GLEW_GET_FUN(__glewVertexAttribLPointer)

#define GLEW_ARB_vertex_attrib_64bit GLEW_GET_VAR(__GLEW_ARB_vertex_attrib_64bit)

#endif /* GL_ARB_vertex_attrib_64bit */

/* ---------------------- GL_ARB_vertex_attrib_binding --------------------- */

#ifndef GL_ARB_vertex_attrib_binding
#define GL_ARB_vertex_attrib_binding 1

#define GL_VERTEX_ATTRIB_BINDING 0x82D4
#define GL_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D5
#define GL_VERTEX_BINDING_DIVISOR 0x82D6
#define GL_VERTEX_BINDING_OFFSET 0x82D7
#define GL_VERTEX_BINDING_STRIDE 0x82D8
#define GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D9
#define GL_MAX_VERTEX_ATTRIB_BINDINGS 0x82DA
#define GL_VERTEX_BINDING_BUFFER 0x8F4F

typedef void (GLAPIENTRY * PFNGLBINDVERTEXBUFFERPROC) (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYBINDVERTEXBUFFEREXTPROC) (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBBINDINGEXTPROC) (GLuint vaobj, GLuint attribindex, GLuint bindingindex);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBFORMATEXTPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBIFORMATEXTPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBLFORMATEXTPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXBINDINGDIVISOREXTPROC) (GLuint vaobj, GLuint bindingindex, GLuint divisor);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBBINDINGPROC) (GLuint attribindex, GLuint bindingindex);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBIFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBLFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXBINDINGDIVISORPROC) (GLuint bindingindex, GLuint divisor);

#define glBindVertexBuffer GLEW_GET_FUN(__glewBindVertexBuffer)
#define glVertexArrayBindVertexBufferEXT GLEW_GET_FUN(__glewVertexArrayBindVertexBufferEXT)
#define glVertexArrayVertexAttribBindingEXT GLEW_GET_FUN(__glewVertexArrayVertexAttribBindingEXT)
#define glVertexArrayVertexAttribFormatEXT GLEW_GET_FUN(__glewVertexArrayVertexAttribFormatEXT)
#define glVertexArrayVertexAttribIFormatEXT GLEW_GET_FUN(__glewVertexArrayVertexAttribIFormatEXT)
#define glVertexArrayVertexAttribLFormatEXT GLEW_GET_FUN(__glewVertexArrayVertexAttribLFormatEXT)
#define glVertexArrayVertexBindingDivisorEXT GLEW_GET_FUN(__glewVertexArrayVertexBindingDivisorEXT)
#define glVertexAttribBinding GLEW_GET_FUN(__glewVertexAttribBinding)
#define glVertexAttribFormat GLEW_GET_FUN(__glewVertexAttribFormat)
#define glVertexAttribIFormat GLEW_GET_FUN(__glewVertexAttribIFormat)
#define glVertexAttribLFormat GLEW_GET_FUN(__glewVertexAttribLFormat)
#define glVertexBindingDivisor GLEW_GET_FUN(__glewVertexBindingDivisor)

#define GLEW_ARB_vertex_attrib_binding GLEW_GET_VAR(__GLEW_ARB_vertex_attrib_binding)

#endif /* GL_ARB_vertex_attrib_binding */

/* -------------------------- GL_ARB_vertex_blend -------------------------- */

#ifndef GL_ARB_vertex_blend
#define GL_ARB_vertex_blend 1

#define GL_MODELVIEW0_ARB 0x1700
#define GL_MODELVIEW1_ARB 0x850A
#define GL_MAX_VERTEX_UNITS_ARB 0x86A4
#define GL_ACTIVE_VERTEX_UNITS_ARB 0x86A5
#define GL_WEIGHT_SUM_UNITY_ARB 0x86A6
#define GL_VERTEX_BLEND_ARB 0x86A7
#define GL_CURRENT_WEIGHT_ARB 0x86A8
#define GL_WEIGHT_ARRAY_TYPE_ARB 0x86A9
#define GL_WEIGHT_ARRAY_STRIDE_ARB 0x86AA
#define GL_WEIGHT_ARRAY_SIZE_ARB 0x86AB
#define GL_WEIGHT_ARRAY_POINTER_ARB 0x86AC
#define GL_WEIGHT_ARRAY_ARB 0x86AD
#define GL_MODELVIEW2_ARB 0x8722
#define GL_MODELVIEW3_ARB 0x8723
#define GL_MODELVIEW4_ARB 0x8724
#define GL_MODELVIEW5_ARB 0x8725
#define GL_MODELVIEW6_ARB 0x8726
#define GL_MODELVIEW7_ARB 0x8727
#define GL_MODELVIEW8_ARB 0x8728
#define GL_MODELVIEW9_ARB 0x8729
#define GL_MODELVIEW10_ARB 0x872A
#define GL_MODELVIEW11_ARB 0x872B
#define GL_MODELVIEW12_ARB 0x872C
#define GL_MODELVIEW13_ARB 0x872D
#define GL_MODELVIEW14_ARB 0x872E
#define GL_MODELVIEW15_ARB 0x872F
#define GL_MODELVIEW16_ARB 0x8730
#define GL_MODELVIEW17_ARB 0x8731
#define GL_MODELVIEW18_ARB 0x8732
#define GL_MODELVIEW19_ARB 0x8733
#define GL_MODELVIEW20_ARB 0x8734
#define GL_MODELVIEW21_ARB 0x8735
#define GL_MODELVIEW22_ARB 0x8736
#define GL_MODELVIEW23_ARB 0x8737
#define GL_MODELVIEW24_ARB 0x8738
#define GL_MODELVIEW25_ARB 0x8739
#define GL_MODELVIEW26_ARB 0x873A
#define GL_MODELVIEW27_ARB 0x873B
#define GL_MODELVIEW28_ARB 0x873C
#define GL_MODELVIEW29_ARB 0x873D
#define GL_MODELVIEW30_ARB 0x873E
#define GL_MODELVIEW31_ARB 0x873F

typedef void (GLAPIENTRY * PFNGLVERTEXBLENDARBPROC) (GLint count);
typedef void (GLAPIENTRY * PFNGLWEIGHTPOINTERARBPROC) (GLint size, GLenum type, GLsizei stride, void *pointer);
typedef void (GLAPIENTRY * PFNGLWEIGHTBVARBPROC) (GLint size, GLbyte *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTDVARBPROC) (GLint size, GLdouble *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTFVARBPROC) (GLint size, GLfloat *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTIVARBPROC) (GLint size, GLint *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTSVARBPROC) (GLint size, GLshort *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTUBVARBPROC) (GLint size, GLubyte *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTUIVARBPROC) (GLint size, GLuint *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTUSVARBPROC) (GLint size, GLushort *weights);

#define glVertexBlendARB GLEW_GET_FUN(__glewVertexBlendARB)
#define glWeightPointerARB GLEW_GET_FUN(__glewWeightPointerARB)
#define glWeightbvARB GLEW_GET_FUN(__glewWeightbvARB)
#define glWeightdvARB GLEW_GET_FUN(__glewWeightdvARB)
#define glWeightfvARB GLEW_GET_FUN(__glewWeightfvARB)
#define glWeightivARB GLEW_GET_FUN(__glewWeightivARB)
#define glWeightsvARB GLEW_GET_FUN(__glewWeightsvARB)
#define glWeightubvARB GLEW_GET_FUN(__glewWeightubvARB)
#define glWeightuivARB GLEW_GET_FUN(__glewWeightuivARB)
#define glWeightusvARB GLEW_GET_FUN(__glewWeightusvARB)

#define GLEW_ARB_vertex_blend GLEW_GET_VAR(__GLEW_ARB_vertex_blend)

#endif /* GL_ARB_vertex_blend */

/* ---------------------- GL_ARB_vertex_buffer_object ---------------------- */

#ifndef GL_ARB_vertex_buffer_object
#define GL_ARB_vertex_buffer_object 1

#define GL_BUFFER_SIZE_ARB 0x8764
#define GL_BUFFER_USAGE_ARB 0x8765
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB 0x8893
#define GL_ARRAY_BUFFER_BINDING_ARB 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB 0x889F
#define GL_READ_ONLY_ARB 0x88B8
#define GL_WRITE_ONLY_ARB 0x88B9
#define GL_READ_WRITE_ARB 0x88BA
#define GL_BUFFER_ACCESS_ARB 0x88BB
#define GL_BUFFER_MAPPED_ARB 0x88BC
#define GL_BUFFER_MAP_POINTER_ARB 0x88BD
#define GL_STREAM_DRAW_ARB 0x88E0
#define GL_STREAM_READ_ARB 0x88E1
#define GL_STREAM_COPY_ARB 0x88E2
#define GL_STATIC_DRAW_ARB 0x88E4
#define GL_STATIC_READ_ARB 0x88E5
#define GL_STATIC_COPY_ARB 0x88E6
#define GL_DYNAMIC_DRAW_ARB 0x88E8
#define GL_DYNAMIC_READ_ARB 0x88E9
#define GL_DYNAMIC_COPY_ARB 0x88EA

typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;

typedef void (GLAPIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptrARB size, const void *data, GLenum usage);
typedef void (GLAPIENTRY * PFNGLBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const void *data);
typedef void (GLAPIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPARAMETERIVARBPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPOINTERVARBPROC) (GLenum target, GLenum pname, void** params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, void *data);
typedef GLboolean (GLAPIENTRY * PFNGLISBUFFERARBPROC) (GLuint buffer);
typedef void * (GLAPIENTRY * PFNGLMAPBUFFERARBPROC) (GLenum target, GLenum access);
typedef GLboolean (GLAPIENTRY * PFNGLUNMAPBUFFERARBPROC) (GLenum target);

#define glBindBufferARB GLEW_GET_FUN(__glewBindBufferARB)
#define glBufferDataARB GLEW_GET_FUN(__glewBufferDataARB)
#define glBufferSubDataARB GLEW_GET_FUN(__glewBufferSubDataARB)
#define glDeleteBuffersARB GLEW_GET_FUN(__glewDeleteBuffersARB)
#define glGenBuffersARB GLEW_GET_FUN(__glewGenBuffersARB)
#define glGetBufferParameterivARB GLEW_GET_FUN(__glewGetBufferParameterivARB)
#define glGetBufferPointervARB GLEW_GET_FUN(__glewGetBufferPointervARB)
#define glGetBufferSubDataARB GLEW_GET_FUN(__glewGetBufferSubDataARB)
#define glIsBufferARB GLEW_GET_FUN(__glewIsBufferARB)
#define glMapBufferARB GLEW_GET_FUN(__glewMapBufferARB)
#define glUnmapBufferARB GLEW_GET_FUN(__glewUnmapBufferARB)

#define GLEW_ARB_vertex_buffer_object GLEW_GET_VAR(__GLEW_ARB_vertex_buffer_object)

#endif /* GL_ARB_vertex_buffer_object */

/* ------------------------- GL_ARB_vertex_program ------------------------- */

#ifndef GL_ARB_vertex_program
#define GL_ARB_vertex_program 1

#define GL_COLOR_SUM_ARB 0x8458
#define GL_VERTEX_PROGRAM_ARB 0x8620
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB 0x8625
#define GL_CURRENT_VERTEX_ATTRIB_ARB 0x8626
#define GL_PROGRAM_LENGTH_ARB 0x8627
#define GL_PROGRAM_STRING_ARB 0x8628
#define GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB 0x862E
#define GL_MAX_PROGRAM_MATRICES_ARB 0x862F
#define GL_CURRENT_MATRIX_STACK_DEPTH_ARB 0x8640
#define GL_CURRENT_MATRIX_ARB 0x8641
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB 0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_ARB 0x8643
#define GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB 0x8645
#define GL_PROGRAM_ERROR_POSITION_ARB 0x864B
#define GL_PROGRAM_BINDING_ARB 0x8677
#define GL_MAX_VERTEX_ATTRIBS_ARB 0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB 0x886A
#define GL_PROGRAM_ERROR_STRING_ARB 0x8874
#define GL_PROGRAM_FORMAT_ASCII_ARB 0x8875
#define GL_PROGRAM_FORMAT_ARB 0x8876
#define GL_PROGRAM_INSTRUCTIONS_ARB 0x88A0
#define GL_MAX_PROGRAM_INSTRUCTIONS_ARB 0x88A1
#define GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB 0x88A2
#define GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB 0x88A3
#define GL_PROGRAM_TEMPORARIES_ARB 0x88A4
#define GL_MAX_PROGRAM_TEMPORARIES_ARB 0x88A5
#define GL_PROGRAM_NATIVE_TEMPORARIES_ARB 0x88A6
#define GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB 0x88A7
#define GL_PROGRAM_PARAMETERS_ARB 0x88A8
#define GL_MAX_PROGRAM_PARAMETERS_ARB 0x88A9
#define GL_PROGRAM_NATIVE_PARAMETERS_ARB 0x88AA
#define GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB 0x88AB
#define GL_PROGRAM_ATTRIBS_ARB 0x88AC
#define GL_MAX_PROGRAM_ATTRIBS_ARB 0x88AD
#define GL_PROGRAM_NATIVE_ATTRIBS_ARB 0x88AE
#define GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB 0x88AF
#define GL_PROGRAM_ADDRESS_REGISTERS_ARB 0x88B0
#define GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB 0x88B1
#define GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB 0x88B2
#define GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB 0x88B3
#define GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB 0x88B4
#define GL_MAX_PROGRAM_ENV_PARAMETERS_ARB 0x88B5
#define GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB 0x88B6
#define GL_TRANSPOSE_CURRENT_MATRIX_ARB 0x88B7
#define GL_MATRIX0_ARB 0x88C0
#define GL_MATRIX1_ARB 0x88C1
#define GL_MATRIX2_ARB 0x88C2
#define GL_MATRIX3_ARB 0x88C3
#define GL_MATRIX4_ARB 0x88C4
#define GL_MATRIX5_ARB 0x88C5
#define GL_MATRIX6_ARB 0x88C6
#define GL_MATRIX7_ARB 0x88C7
#define GL_MATRIX8_ARB 0x88C8
#define GL_MATRIX9_ARB 0x88C9
#define GL_MATRIX10_ARB 0x88CA
#define GL_MATRIX11_ARB 0x88CB
#define GL_MATRIX12_ARB 0x88CC
#define GL_MATRIX13_ARB 0x88CD
#define GL_MATRIX14_ARB 0x88CE
#define GL_MATRIX15_ARB 0x88CF
#define GL_MATRIX16_ARB 0x88D0
#define GL_MATRIX17_ARB 0x88D1
#define GL_MATRIX18_ARB 0x88D2
#define GL_MATRIX19_ARB 0x88D3
#define GL_MATRIX20_ARB 0x88D4
#define GL_MATRIX21_ARB 0x88D5
#define GL_MATRIX22_ARB 0x88D6
#define GL_MATRIX23_ARB 0x88D7
#define GL_MATRIX24_ARB 0x88D8
#define GL_MATRIX25_ARB 0x88D9
#define GL_MATRIX26_ARB 0x88DA
#define GL_MATRIX27_ARB 0x88DB
#define GL_MATRIX28_ARB 0x88DC
#define GL_MATRIX29_ARB 0x88DD
#define GL_MATRIX30_ARB 0x88DE
#define GL_MATRIX31_ARB 0x88DF

typedef void (GLAPIENTRY * PFNGLBINDPROGRAMARBPROC) (GLenum target, GLuint program);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMSARBPROC) (GLsizei n, const GLuint* programs);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void (GLAPIENTRY * PFNGLGENPROGRAMSARBPROC) (GLsizei n, GLuint* programs);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMENVPARAMETERDVARBPROC) (GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMENVPARAMETERFVARBPROC) (GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC) (GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC) (GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMSTRINGARBPROC) (GLenum target, GLenum pname, void *string);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMIVARBPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBPOINTERVARBPROC) (GLuint index, GLenum pname, void** pointer);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBDVARBPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBFVARBPROC) (GLuint index, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIVARBPROC) (GLuint index, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMARBPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4DARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4DVARBPROC) (GLenum target, GLuint index, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4FARBPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4FVARBPROC) (GLenum target, GLuint index, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4DARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4DVARBPROC) (GLenum target, GLuint index, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4FARBPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4FVARBPROC) (GLenum target, GLuint index, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMSTRINGARBPROC) (GLenum target, GLenum format, GLsizei len, const void *string);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DARBPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DVARBPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FARBPROC) (GLuint index, GLfloat x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FVARBPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SARBPROC) (GLuint index, GLshort x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DARBPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DVARBPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FARBPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FVARBPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SARBPROC) (GLuint index, GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DVARBPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FVARBPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NBVARBPROC) (GLuint index, const GLbyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NIVARBPROC) (GLuint index, const GLint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NSVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBARBPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBVARBPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUIVARBPROC) (GLuint index, const GLuint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUSVARBPROC) (GLuint index, const GLushort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4BVARBPROC) (GLuint index, const GLbyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DVARBPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FVARBPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4IVARBPROC) (GLuint index, const GLint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBVARBPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UIVARBPROC) (GLuint index, const GLuint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4USVARBPROC) (GLuint index, const GLushort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBPOINTERARBPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

#define glBindProgramARB GLEW_GET_FUN(__glewBindProgramARB)
#define glDeleteProgramsARB GLEW_GET_FUN(__glewDeleteProgramsARB)
#define glDisableVertexAttribArrayARB GLEW_GET_FUN(__glewDisableVertexAttribArrayARB)
#define glEnableVertexAttribArrayARB GLEW_GET_FUN(__glewEnableVertexAttribArrayARB)
#define glGenProgramsARB GLEW_GET_FUN(__glewGenProgramsARB)
#define glGetProgramEnvParameterdvARB GLEW_GET_FUN(__glewGetProgramEnvParameterdvARB)
#define glGetProgramEnvParameterfvARB GLEW_GET_FUN(__glewGetProgramEnvParameterfvARB)
#define glGetProgramLocalParameterdvARB GLEW_GET_FUN(__glewGetProgramLocalParameterdvARB)
#define glGetProgramLocalParameterfvARB GLEW_GET_FUN(__glewGetProgramLocalParameterfvARB)
#define glGetProgramStringARB GLEW_GET_FUN(__glewGetProgramStringARB)
#define glGetProgramivARB GLEW_GET_FUN(__glewGetProgramivARB)
#define glGetVertexAttribPointervARB GLEW_GET_FUN(__glewGetVertexAttribPointervARB)
#define glGetVertexAttribdvARB GLEW_GET_FUN(__glewGetVertexAttribdvARB)
#define glGetVertexAttribfvARB GLEW_GET_FUN(__glewGetVertexAttribfvARB)
#define glGetVertexAttribivARB GLEW_GET_FUN(__glewGetVertexAttribivARB)
#define glIsProgramARB GLEW_GET_FUN(__glewIsProgramARB)
#define glProgramEnvParameter4dARB GLEW_GET_FUN(__glewProgramEnvParameter4dARB)
#define glProgramEnvParameter4dvARB GLEW_GET_FUN(__glewProgramEnvParameter4dvARB)
#define glProgramEnvParameter4fARB GLEW_GET_FUN(__glewProgramEnvParameter4fARB)
#define glProgramEnvParameter4fvARB GLEW_GET_FUN(__glewProgramEnvParameter4fvARB)
#define glProgramLocalParameter4dARB GLEW_GET_FUN(__glewProgramLocalParameter4dARB)
#define glProgramLocalParameter4dvARB GLEW_GET_FUN(__glewProgramLocalParameter4dvARB)
#define glProgramLocalParameter4fARB GLEW_GET_FUN(__glewProgramLocalParameter4fARB)
#define glProgramLocalParameter4fvARB GLEW_GET_FUN(__glewProgramLocalParameter4fvARB)
#define glProgramStringARB GLEW_GET_FUN(__glewProgramStringARB)
#define glVertexAttrib1dARB GLEW_GET_FUN(__glewVertexAttrib1dARB)
#define glVertexAttrib1dvARB GLEW_GET_FUN(__glewVertexAttrib1dvARB)
#define glVertexAttrib1fARB GLEW_GET_FUN(__glewVertexAttrib1fARB)
#define glVertexAttrib1fvARB GLEW_GET_FUN(__glewVertexAttrib1fvARB)
#define glVertexAttrib1sARB GLEW_GET_FUN(__glewVertexAttrib1sARB)
#define glVertexAttrib1svARB GLEW_GET_FUN(__glewVertexAttrib1svARB)
#define glVertexAttrib2dARB GLEW_GET_FUN(__glewVertexAttrib2dARB)
#define glVertexAttrib2dvARB GLEW_GET_FUN(__glewVertexAttrib2dvARB)
#define glVertexAttrib2fARB GLEW_GET_FUN(__glewVertexAttrib2fARB)
#define glVertexAttrib2fvARB GLEW_GET_FUN(__glewVertexAttrib2fvARB)
#define glVertexAttrib2sARB GLEW_GET_FUN(__glewVertexAttrib2sARB)
#define glVertexAttrib2svARB GLEW_GET_FUN(__glewVertexAttrib2svARB)
#define glVertexAttrib3dARB GLEW_GET_FUN(__glewVertexAttrib3dARB)
#define glVertexAttrib3dvARB GLEW_GET_FUN(__glewVertexAttrib3dvARB)
#define glVertexAttrib3fARB GLEW_GET_FUN(__glewVertexAttrib3fARB)
#define glVertexAttrib3fvARB GLEW_GET_FUN(__glewVertexAttrib3fvARB)
#define glVertexAttrib3sARB GLEW_GET_FUN(__glewVertexAttrib3sARB)
#define glVertexAttrib3svARB GLEW_GET_FUN(__glewVertexAttrib3svARB)
#define glVertexAttrib4NbvARB GLEW_GET_FUN(__glewVertexAttrib4NbvARB)
#define glVertexAttrib4NivARB GLEW_GET_FUN(__glewVertexAttrib4NivARB)
#define glVertexAttrib4NsvARB GLEW_GET_FUN(__glewVertexAttrib4NsvARB)
#define glVertexAttrib4NubARB GLEW_GET_FUN(__glewVertexAttrib4NubARB)
#define glVertexAttrib4NubvARB GLEW_GET_FUN(__glewVertexAttrib4NubvARB)
#define glVertexAttrib4NuivARB GLEW_GET_FUN(__glewVertexAttrib4NuivARB)
#define glVertexAttrib4NusvARB GLEW_GET_FUN(__glewVertexAttrib4NusvARB)
#define glVertexAttrib4bvARB GLEW_GET_FUN(__glewVertexAttrib4bvARB)
#define glVertexAttrib4dARB GLEW_GET_FUN(__glewVertexAttrib4dARB)
#define glVertexAttrib4dvARB GLEW_GET_FUN(__glewVertexAttrib4dvARB)
#define glVertexAttrib4fARB GLEW_GET_FUN(__glewVertexAttrib4fARB)
#define glVertexAttrib4fvARB GLEW_GET_FUN(__glewVertexAttrib4fvARB)
#define glVertexAttrib4ivARB GLEW_GET_FUN(__glewVertexAttrib4ivARB)
#define glVertexAttrib4sARB GLEW_GET_FUN(__glewVertexAttrib4sARB)
#define glVertexAttrib4svARB GLEW_GET_FUN(__glewVertexAttrib4svARB)
#define glVertexAttrib4ubvARB GLEW_GET_FUN(__glewVertexAttrib4ubvARB)
#define glVertexAttrib4uivARB GLEW_GET_FUN(__glewVertexAttrib4uivARB)
#define glVertexAttrib4usvARB GLEW_GET_FUN(__glewVertexAttrib4usvARB)
#define glVertexAttribPointerARB GLEW_GET_FUN(__glewVertexAttribPointerARB)

#define GLEW_ARB_vertex_program GLEW_GET_VAR(__GLEW_ARB_vertex_program)

#endif /* GL_ARB_vertex_program */

/* ------------------- GL_ARB_vertex_type_10f_11f_11f_rev ------------------ */

#ifndef GL_ARB_vertex_type_10f_11f_11f_rev
#define GL_ARB_vertex_type_10f_11f_11f_rev 1

#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B

#define GLEW_ARB_vertex_type_10f_11f_11f_rev GLEW_GET_VAR(__GLEW_ARB_vertex_type_10f_11f_11f_rev)

#endif /* GL_ARB_vertex_type_10f_11f_11f_rev */

/* ------------------- GL_ARB_vertex_type_2_10_10_10_rev ------------------- */

#ifndef GL_ARB_vertex_type_2_10_10_10_rev
#define GL_ARB_vertex_type_2_10_10_10_rev 1

#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#define GL_INT_2_10_10_10_REV 0x8D9F

typedef void (GLAPIENTRY * PFNGLCOLORP3UIPROC) (GLenum type, GLuint color);
typedef void (GLAPIENTRY * PFNGLCOLORP3UIVPROC) (GLenum type, const GLuint* color);
typedef void (GLAPIENTRY * PFNGLCOLORP4UIPROC) (GLenum type, GLuint color);
typedef void (GLAPIENTRY * PFNGLCOLORP4UIVPROC) (GLenum type, const GLuint* color);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP1UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP1UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP2UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP2UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP3UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP3UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP4UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP4UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLNORMALP3UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLNORMALP3UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORP3UIPROC) (GLenum type, GLuint color);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORP3UIVPROC) (GLenum type, const GLuint* color);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP1UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP1UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP2UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP2UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP3UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP3UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP4UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP4UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP1UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP1UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP2UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP2UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP3UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP3UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP4UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP4UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXP2UIPROC) (GLenum type, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXP2UIVPROC) (GLenum type, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXP3UIPROC) (GLenum type, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXP3UIVPROC) (GLenum type, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXP4UIPROC) (GLenum type, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXP4UIVPROC) (GLenum type, const GLuint* value);

#define glColorP3ui GLEW_GET_FUN(__glewColorP3ui)
#define glColorP3uiv GLEW_GET_FUN(__glewColorP3uiv)
#define glColorP4ui GLEW_GET_FUN(__glewColorP4ui)
#define glColorP4uiv GLEW_GET_FUN(__glewColorP4uiv)
#define glMultiTexCoordP1ui GLEW_GET_FUN(__glewMultiTexCoordP1ui)
#define glMultiTexCoordP1uiv GLEW_GET_FUN(__glewMultiTexCoordP1uiv)
#define glMultiTexCoordP2ui GLEW_GET_FUN(__glewMultiTexCoordP2ui)
#define glMultiTexCoordP2uiv GLEW_GET_FUN(__glewMultiTexCoordP2uiv)
#define glMultiTexCoordP3ui GLEW_GET_FUN(__glewMultiTexCoordP3ui)
#define glMultiTexCoordP3uiv GLEW_GET_FUN(__glewMultiTexCoordP3uiv)
#define glMultiTexCoordP4ui GLEW_GET_FUN(__glewMultiTexCoordP4ui)
#define glMultiTexCoordP4uiv GLEW_GET_FUN(__glewMultiTexCoordP4uiv)
#define glNormalP3ui GLEW_GET_FUN(__glewNormalP3ui)
#define glNormalP3uiv GLEW_GET_FUN(__glewNormalP3uiv)
#define glSecondaryColorP3ui GLEW_GET_FUN(__glewSecondaryColorP3ui)
#define glSecondaryColorP3uiv GLEW_GET_FUN(__glewSecondaryColorP3uiv)
#define glTexCoordP1ui GLEW_GET_FUN(__glewTexCoordP1ui)
#define glTexCoordP1uiv GLEW_GET_FUN(__glewTexCoordP1uiv)
#define glTexCoordP2ui GLEW_GET_FUN(__glewTexCoordP2ui)
#define glTexCoordP2uiv GLEW_GET_FUN(__glewTexCoordP2uiv)
#define glTexCoordP3ui GLEW_GET_FUN(__glewTexCoordP3ui)
#define glTexCoordP3uiv GLEW_GET_FUN(__glewTexCoordP3uiv)
#define glTexCoordP4ui GLEW_GET_FUN(__glewTexCoordP4ui)
#define glTexCoordP4uiv GLEW_GET_FUN(__glewTexCoordP4uiv)
#define glVertexAttribP1ui GLEW_GET_FUN(__glewVertexAttribP1ui)
#define glVertexAttribP1uiv GLEW_GET_FUN(__glewVertexAttribP1uiv)
#define glVertexAttribP2ui GLEW_GET_FUN(__glewVertexAttribP2ui)
#define glVertexAttribP2uiv GLEW_GET_FUN(__glewVertexAttribP2uiv)
#define glVertexAttribP3ui GLEW_GET_FUN(__glewVertexAttribP3ui)
#define glVertexAttribP3uiv GLEW_GET_FUN(__glewVertexAttribP3uiv)
#define glVertexAttribP4ui GLEW_GET_FUN(__glewVertexAttribP4ui)
#define glVertexAttribP4uiv GLEW_GET_FUN(__glewVertexAttribP4uiv)
#define glVertexP2ui GLEW_GET_FUN(__glewVertexP2ui)
#define glVertexP2uiv GLEW_GET_FUN(__glewVertexP2uiv)
#define glVertexP3ui GLEW_GET_FUN(__glewVertexP3ui)
#define glVertexP3uiv GLEW_GET_FUN(__glewVertexP3uiv)
#define glVertexP4ui GLEW_GET_FUN(__glewVertexP4ui)
#define glVertexP4uiv GLEW_GET_FUN(__glewVertexP4uiv)

#define GLEW_ARB_vertex_type_2_10_10_10_rev GLEW_GET_VAR(__GLEW_ARB_vertex_type_2_10_10_10_rev)

#endif /* GL_ARB_vertex_type_2_10_10_10_rev */

/* ------------------------- GL_ARB_viewport_array ------------------------- */

#ifndef GL_ARB_viewport_array
#define GL_ARB_viewport_array 1

#define GL_DEPTH_RANGE 0x0B70
#define GL_VIEWPORT 0x0BA2
#define GL_SCISSOR_BOX 0x0C10
#define GL_SCISSOR_TEST 0x0C11
#define GL_MAX_VIEWPORTS 0x825B
#define GL_VIEWPORT_SUBPIXEL_BITS 0x825C
#define GL_VIEWPORT_BOUNDS_RANGE 0x825D
#define GL_LAYER_PROVOKING_VERTEX 0x825E
#define GL_VIEWPORT_INDEX_PROVOKING_VERTEX 0x825F
#define GL_UNDEFINED_VERTEX 0x8260
#define GL_FIRST_VERTEX_CONVENTION 0x8E4D
#define GL_LAST_VERTEX_CONVENTION 0x8E4E
#define GL_PROVOKING_VERTEX 0x8E4F

typedef void (GLAPIENTRY * PFNGLDEPTHRANGEARRAYVPROC) (GLuint first, GLsizei count, const GLclampd * v);
typedef void (GLAPIENTRY * PFNGLDEPTHRANGEINDEXEDPROC) (GLuint index, GLclampd n, GLclampd f);
typedef void (GLAPIENTRY * PFNGLGETDOUBLEI_VPROC) (GLenum target, GLuint index, GLdouble* data);
typedef void (GLAPIENTRY * PFNGLGETFLOATI_VPROC) (GLenum target, GLuint index, GLfloat* data);
typedef void (GLAPIENTRY * PFNGLSCISSORARRAYVPROC) (GLuint first, GLsizei count, const GLint * v);
typedef void (GLAPIENTRY * PFNGLSCISSORINDEXEDPROC) (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLSCISSORINDEXEDVPROC) (GLuint index, const GLint * v);
typedef void (GLAPIENTRY * PFNGLVIEWPORTARRAYVPROC) (GLuint first, GLsizei count, const GLfloat * v);
typedef void (GLAPIENTRY * PFNGLVIEWPORTINDEXEDFPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
typedef void (GLAPIENTRY * PFNGLVIEWPORTINDEXEDFVPROC) (GLuint index, const GLfloat * v);

#define glDepthRangeArrayv GLEW_GET_FUN(__glewDepthRangeArrayv)
#define glDepthRangeIndexed GLEW_GET_FUN(__glewDepthRangeIndexed)
#define glGetDoublei_v GLEW_GET_FUN(__glewGetDoublei_v)
#define glGetFloati_v GLEW_GET_FUN(__glewGetFloati_v)
#define glScissorArrayv GLEW_GET_FUN(__glewScissorArrayv)
#define glScissorIndexed GLEW_GET_FUN(__glewScissorIndexed)
#define glScissorIndexedv GLEW_GET_FUN(__glewScissorIndexedv)
#define glViewportArrayv GLEW_GET_FUN(__glewViewportArrayv)
#define glViewportIndexedf GLEW_GET_FUN(__glewViewportIndexedf)
#define glViewportIndexedfv GLEW_GET_FUN(__glewViewportIndexedfv)

#define GLEW_ARB_viewport_array GLEW_GET_VAR(__GLEW_ARB_viewport_array)

#endif /* GL_ARB_viewport_array */

/* --------------------------- GL_ARB_window_pos --------------------------- */

#ifndef GL_ARB_window_pos
#define GL_ARB_window_pos 1

typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DARBPROC) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DVARBPROC) (const GLdouble* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FARBPROC) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FVARBPROC) (const GLfloat* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IARBPROC) (GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IVARBPROC) (const GLint* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SARBPROC) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SVARBPROC) (const GLshort* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DARBPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DVARBPROC) (const GLdouble* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FARBPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FVARBPROC) (const GLfloat* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IARBPROC) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IVARBPROC) (const GLint* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SARBPROC) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SVARBPROC) (const GLshort* p);

#define glWindowPos2dARB GLEW_GET_FUN(__glewWindowPos2dARB)
#define glWindowPos2dvARB GLEW_GET_FUN(__glewWindowPos2dvARB)
#define glWindowPos2fARB GLEW_GET_FUN(__glewWindowPos2fARB)
#define glWindowPos2fvARB GLEW_GET_FUN(__glewWindowPos2fvARB)
#define glWindowPos2iARB GLEW_GET_FUN(__glewWindowPos2iARB)
#define glWindowPos2ivARB GLEW_GET_FUN(__glewWindowPos2ivARB)
#define glWindowPos2sARB GLEW_GET_FUN(__glewWindowPos2sARB)
#define glWindowPos2svARB GLEW_GET_FUN(__glewWindowPos2svARB)
#define glWindowPos3dARB GLEW_GET_FUN(__glewWindowPos3dARB)
#define glWindowPos3dvARB GLEW_GET_FUN(__glewWindowPos3dvARB)
#define glWindowPos3fARB GLEW_GET_FUN(__glewWindowPos3fARB)
#define glWindowPos3fvARB GLEW_GET_FUN(__glewWindowPos3fvARB)
#define glWindowPos3iARB GLEW_GET_FUN(__glewWindowPos3iARB)
#define glWindowPos3ivARB GLEW_GET_FUN(__glewWindowPos3ivARB)
#define glWindowPos3sARB GLEW_GET_FUN(__glewWindowPos3sARB)
#define glWindowPos3svARB GLEW_GET_FUN(__glewWindowPos3svARB)

#define GLEW_ARB_window_pos GLEW_GET_VAR(__GLEW_ARB_window_pos)

#endif /* GL_ARB_window_pos */

/* -------------------- GL_EGL_KHR_context_flush_control ------------------- */

#ifndef GL_EGL_KHR_context_flush_control
#define GL_EGL_KHR_context_flush_control 1

#define GLEW_EGL_KHR_context_flush_control GLEW_GET_VAR(__GLEW_EGL_KHR_context_flush_control)

#endif /* GL_EGL_KHR_context_flush_control */

/* --------------------------- GL_EXT_422_pixels --------------------------- */

#ifndef GL_EXT_422_pixels
#define GL_EXT_422_pixels 1

#define GL_422_EXT 0x80CC
#define GL_422_REV_EXT 0x80CD
#define GL_422_AVERAGE_EXT 0x80CE
#define GL_422_REV_AVERAGE_EXT 0x80CF

#define GLEW_EXT_422_pixels GLEW_GET_VAR(__GLEW_EXT_422_pixels)

#endif /* GL_EXT_422_pixels */

/* ---------------------------- GL_EXT_Cg_shader --------------------------- */

#ifndef GL_EXT_Cg_shader
#define GL_EXT_Cg_shader 1

#define GL_CG_VERTEX_SHADER_EXT 0x890E
#define GL_CG_FRAGMENT_SHADER_EXT 0x890F

#define GLEW_EXT_Cg_shader GLEW_GET_VAR(__GLEW_EXT_Cg_shader)

#endif /* GL_EXT_Cg_shader */

/* ------------------------- GL_EXT_EGL_image_array ------------------------ */

#ifndef GL_EXT_EGL_image_array
#define GL_EXT_EGL_image_array 1

#define GLEW_EXT_EGL_image_array GLEW_GET_VAR(__GLEW_EXT_EGL_image_array)

#endif /* GL_EXT_EGL_image_array */

/* --------------------------- GL_EXT_YUV_target --------------------------- */

#ifndef GL_EXT_YUV_target
#define GL_EXT_YUV_target 1

#define GL_SAMPLER_EXTERNAL_2D_Y2Y_EXT 0x8BE7

#define GLEW_EXT_YUV_target GLEW_GET_VAR(__GLEW_EXT_YUV_target)

#endif /* GL_EXT_YUV_target */

/* ------------------------------ GL_EXT_abgr ------------------------------ */

#ifndef GL_EXT_abgr
#define GL_EXT_abgr 1

#define GL_ABGR_EXT 0x8000

#define GLEW_EXT_abgr GLEW_GET_VAR(__GLEW_EXT_abgr)

#endif /* GL_EXT_abgr */

/* -------------------------- GL_EXT_base_instance ------------------------- */

#ifndef GL_EXT_base_instance
#define GL_EXT_base_instance 1

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEEXTPROC) (GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEEXTPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEEXTPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);

#define glDrawArraysInstancedBaseInstanceEXT GLEW_GET_FUN(__glewDrawArraysInstancedBaseInstanceEXT)
#define glDrawElementsInstancedBaseInstanceEXT GLEW_GET_FUN(__glewDrawElementsInstancedBaseInstanceEXT)
#define glDrawElementsInstancedBaseVertexBaseInstanceEXT GLEW_GET_FUN(__glewDrawElementsInstancedBaseVertexBaseInstanceEXT)

#define GLEW_EXT_base_instance GLEW_GET_VAR(__GLEW_EXT_base_instance)

#endif /* GL_EXT_base_instance */

/* ------------------------------ GL_EXT_bgra ------------------------------ */

#ifndef GL_EXT_bgra
#define GL_EXT_bgra 1

#define GL_BGR_EXT 0x80E0
#define GL_BGRA_EXT 0x80E1

#define GLEW_EXT_bgra GLEW_GET_VAR(__GLEW_EXT_bgra)

#endif /* GL_EXT_bgra */

/* ------------------------ GL_EXT_bindable_uniform ------------------------ */

#ifndef GL_EXT_bindable_uniform
#define GL_EXT_bindable_uniform 1

#define GL_MAX_VERTEX_BINDABLE_UNIFORMS_EXT 0x8DE2
#define GL_MAX_FRAGMENT_BINDABLE_UNIFORMS_EXT 0x8DE3
#define GL_MAX_GEOMETRY_BINDABLE_UNIFORMS_EXT 0x8DE4
#define GL_MAX_BINDABLE_UNIFORM_SIZE_EXT 0x8DED
#define GL_UNIFORM_BUFFER_EXT 0x8DEE
#define GL_UNIFORM_BUFFER_BINDING_EXT 0x8DEF

typedef GLint (GLAPIENTRY * PFNGLGETUNIFORMBUFFERSIZEEXTPROC) (GLuint program, GLint location);
typedef GLintptr (GLAPIENTRY * PFNGLGETUNIFORMOFFSETEXTPROC) (GLuint program, GLint location);
typedef void (GLAPIENTRY * PFNGLUNIFORMBUFFEREXTPROC) (GLuint program, GLint location, GLuint buffer);

#define glGetUniformBufferSizeEXT GLEW_GET_FUN(__glewGetUniformBufferSizeEXT)
#define glGetUniformOffsetEXT GLEW_GET_FUN(__glewGetUniformOffsetEXT)
#define glUniformBufferEXT GLEW_GET_FUN(__glewUniformBufferEXT)

#define GLEW_EXT_bindable_uniform GLEW_GET_VAR(__GLEW_EXT_bindable_uniform)

#endif /* GL_EXT_bindable_uniform */

/* --------------------------- GL_EXT_blend_color -------------------------- */

#ifndef GL_EXT_blend_color
#define GL_EXT_blend_color 1

#define GL_CONSTANT_COLOR_EXT 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR_EXT 0x8002
#define GL_CONSTANT_ALPHA_EXT 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA_EXT 0x8004
#define GL_BLEND_COLOR_EXT 0x8005

typedef void (GLAPIENTRY * PFNGLBLENDCOLOREXTPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

#define glBlendColorEXT GLEW_GET_FUN(__glewBlendColorEXT)

#define GLEW_EXT_blend_color GLEW_GET_VAR(__GLEW_EXT_blend_color)

#endif /* GL_EXT_blend_color */

/* --------------------- GL_EXT_blend_equation_separate -------------------- */

#ifndef GL_EXT_blend_equation_separate
#define GL_EXT_blend_equation_separate 1

#define GL_BLEND_EQUATION_RGB_EXT 0x8009
#define GL_BLEND_EQUATION_ALPHA_EXT 0x883D

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEEXTPROC) (GLenum modeRGB, GLenum modeAlpha);

#define glBlendEquationSeparateEXT GLEW_GET_FUN(__glewBlendEquationSeparateEXT)

#define GLEW_EXT_blend_equation_separate GLEW_GET_VAR(__GLEW_EXT_blend_equation_separate)

#endif /* GL_EXT_blend_equation_separate */

/* ----------------------- GL_EXT_blend_func_extended ---------------------- */

#ifndef GL_EXT_blend_func_extended
#define GL_EXT_blend_func_extended 1

#define GL_SRC_ALPHA_SATURATE_EXT 0x0308
#define GL_SRC1_ALPHA_EXT 0x8589
#define GL_SRC1_COLOR_EXT 0x88F9
#define GL_ONE_MINUS_SRC1_COLOR_EXT 0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA_EXT 0x88FB
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS_EXT 0x88FC
#define GL_LOCATION_INDEX_EXT 0x930F

typedef void (GLAPIENTRY * PFNGLBINDFRAGDATALOCATIONINDEXEDEXTPROC) (GLuint program, GLuint colorNumber, GLuint index, const GLchar * name);
typedef GLint (GLAPIENTRY * PFNGLGETFRAGDATAINDEXEXTPROC) (GLuint program, const GLchar * name);
typedef GLint (GLAPIENTRY * PFNGLGETPROGRAMRESOURCELOCATIONINDEXEXTPROC) (GLuint program, GLenum programInterface, const GLchar* name);

#define glBindFragDataLocationIndexedEXT GLEW_GET_FUN(__glewBindFragDataLocationIndexedEXT)
#define glGetFragDataIndexEXT GLEW_GET_FUN(__glewGetFragDataIndexEXT)
#define glGetProgramResourceLocationIndexEXT GLEW_GET_FUN(__glewGetProgramResourceLocationIndexEXT)

#define GLEW_EXT_blend_func_extended GLEW_GET_VAR(__GLEW_EXT_blend_func_extended)

#endif /* GL_EXT_blend_func_extended */

/* ----------------------- GL_EXT_blend_func_separate ---------------------- */

#ifndef GL_EXT_blend_func_separate
#define GL_EXT_blend_func_separate 1

#define GL_BLEND_DST_RGB_EXT 0x80C8
#define GL_BLEND_SRC_RGB_EXT 0x80C9
#define GL_BLEND_DST_ALPHA_EXT 0x80CA
#define GL_BLEND_SRC_ALPHA_EXT 0x80CB

typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEEXTPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

#define glBlendFuncSeparateEXT GLEW_GET_FUN(__glewBlendFuncSeparateEXT)

#define GLEW_EXT_blend_func_separate GLEW_GET_VAR(__GLEW_EXT_blend_func_separate)

#endif /* GL_EXT_blend_func_separate */

/* ------------------------- GL_EXT_blend_logic_op ------------------------- */

#ifndef GL_EXT_blend_logic_op
#define GL_EXT_blend_logic_op 1

#define GLEW_EXT_blend_logic_op GLEW_GET_VAR(__GLEW_EXT_blend_logic_op)

#endif /* GL_EXT_blend_logic_op */

/* -------------------------- GL_EXT_blend_minmax -------------------------- */

#ifndef GL_EXT_blend_minmax
#define GL_EXT_blend_minmax 1

#define GL_FUNC_ADD_EXT 0x8006
#define GL_MIN_EXT 0x8007
#define GL_MAX_EXT 0x8008
#define GL_BLEND_EQUATION_EXT 0x8009

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONEXTPROC) (GLenum mode);

#define glBlendEquationEXT GLEW_GET_FUN(__glewBlendEquationEXT)

#define GLEW_EXT_blend_minmax GLEW_GET_VAR(__GLEW_EXT_blend_minmax)

#endif /* GL_EXT_blend_minmax */

/* ------------------------- GL_EXT_blend_subtract ------------------------- */

#ifndef GL_EXT_blend_subtract
#define GL_EXT_blend_subtract 1

#define GL_FUNC_SUBTRACT_EXT 0x800A
#define GL_FUNC_REVERSE_SUBTRACT_EXT 0x800B

#define GLEW_EXT_blend_subtract GLEW_GET_VAR(__GLEW_EXT_blend_subtract)

#endif /* GL_EXT_blend_subtract */

/* ------------------------- GL_EXT_buffer_storage ------------------------- */

#ifndef GL_EXT_buffer_storage
#define GL_EXT_buffer_storage 1

#define GL_MAP_READ_BIT 0x0001
#define GL_MAP_WRITE_BIT 0x0002
#define GL_MAP_PERSISTENT_BIT_EXT 0x0040
#define GL_MAP_COHERENT_BIT_EXT 0x0080
#define GL_DYNAMIC_STORAGE_BIT_EXT 0x0100
#define GL_CLIENT_STORAGE_BIT_EXT 0x0200
#define GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT_EXT 0x00004000
#define GL_BUFFER_IMMUTABLE_STORAGE_EXT 0x821F
#define GL_BUFFER_STORAGE_FLAGS_EXT 0x8220

typedef void (GLAPIENTRY * PFNGLBUFFERSTORAGEEXTPROC) (GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERSTORAGEEXTPROC) (GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);

#define glBufferStorageEXT GLEW_GET_FUN(__glewBufferStorageEXT)
#define glNamedBufferStorageEXT GLEW_GET_FUN(__glewNamedBufferStorageEXT)

#define GLEW_EXT_buffer_storage GLEW_GET_VAR(__GLEW_EXT_buffer_storage)

#endif /* GL_EXT_buffer_storage */

/* -------------------------- GL_EXT_clear_texture ------------------------- */

#ifndef GL_EXT_clear_texture
#define GL_EXT_clear_texture 1

typedef void (GLAPIENTRY * PFNGLCLEARTEXIMAGEEXTPROC) (GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
typedef void (GLAPIENTRY * PFNGLCLEARTEXSUBIMAGEEXTPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);

#define glClearTexImageEXT GLEW_GET_FUN(__glewClearTexImageEXT)
#define glClearTexSubImageEXT GLEW_GET_FUN(__glewClearTexSubImageEXT)

#define GLEW_EXT_clear_texture GLEW_GET_VAR(__GLEW_EXT_clear_texture)

#endif /* GL_EXT_clear_texture */

/* ----------------------- GL_EXT_clip_cull_distance ----------------------- */

#ifndef GL_EXT_clip_cull_distance
#define GL_EXT_clip_cull_distance 1

#define GL_MAX_CLIP_DISTANCES_EXT 0x0D32
#define GL_CLIP_DISTANCE0_EXT 0x3000
#define GL_CLIP_DISTANCE1_EXT 0x3001
#define GL_CLIP_DISTANCE2_EXT 0x3002
#define GL_CLIP_DISTANCE3_EXT 0x3003
#define GL_CLIP_DISTANCE4_EXT 0x3004
#define GL_CLIP_DISTANCE5_EXT 0x3005
#define GL_CLIP_DISTANCE6_EXT 0x3006
#define GL_CLIP_DISTANCE7_EXT 0x3007
#define GL_MAX_CULL_DISTANCES_EXT 0x82F9
#define GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES_EXT 0x82FA

#define GLEW_EXT_clip_cull_distance GLEW_GET_VAR(__GLEW_EXT_clip_cull_distance)

#endif /* GL_EXT_clip_cull_distance */

/* ------------------------ GL_EXT_clip_volume_hint ------------------------ */

#ifndef GL_EXT_clip_volume_hint
#define GL_EXT_clip_volume_hint 1

#define GL_CLIP_VOLUME_CLIPPING_HINT_EXT 0x80F0

#define GLEW_EXT_clip_volume_hint GLEW_GET_VAR(__GLEW_EXT_clip_volume_hint)

#endif /* GL_EXT_clip_volume_hint */

/* ------------------------------ GL_EXT_cmyka ----------------------------- */

#ifndef GL_EXT_cmyka
#define GL_EXT_cmyka 1

#define GL_CMYK_EXT 0x800C
#define GL_CMYKA_EXT 0x800D
#define GL_PACK_CMYK_HINT_EXT 0x800E
#define GL_UNPACK_CMYK_HINT_EXT 0x800F

#define GLEW_EXT_cmyka GLEW_GET_VAR(__GLEW_EXT_cmyka)

#endif /* GL_EXT_cmyka */

/* ----------------------- GL_EXT_color_buffer_float ----------------------- */

#ifndef GL_EXT_color_buffer_float
#define GL_EXT_color_buffer_float 1

#define GLEW_EXT_color_buffer_float GLEW_GET_VAR(__GLEW_EXT_color_buffer_float)

#endif /* GL_EXT_color_buffer_float */

/* --------------------- GL_EXT_color_buffer_half_float -------------------- */

#ifndef GL_EXT_color_buffer_half_float
#define GL_EXT_color_buffer_half_float 1

#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE_EXT 0x8211
#define GL_R16F_EXT 0x822D
#define GL_RG16F_EXT 0x822F
#define GL_RGBA16F_EXT 0x881A
#define GL_RGB16F_EXT 0x881B
#define GL_UNSIGNED_NORMALIZED_EXT 0x8C17

#define GLEW_EXT_color_buffer_half_float GLEW_GET_VAR(__GLEW_EXT_color_buffer_half_float)

#endif /* GL_EXT_color_buffer_half_float */

/* ------------------------- GL_EXT_color_subtable ------------------------- */

#ifndef GL_EXT_color_subtable
#define GL_EXT_color_subtable 1

typedef void (GLAPIENTRY * PFNGLCOLORSUBTABLEEXTPROC) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void *data);
typedef void (GLAPIENTRY * PFNGLCOPYCOLORSUBTABLEEXTPROC) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);

#define glColorSubTableEXT GLEW_GET_FUN(__glewColorSubTableEXT)
#define glCopyColorSubTableEXT GLEW_GET_FUN(__glewCopyColorSubTableEXT)

#define GLEW_EXT_color_subtable GLEW_GET_VAR(__GLEW_EXT_color_subtable)

#endif /* GL_EXT_color_subtable */

/* ---------------------- GL_EXT_compiled_vertex_array --------------------- */

#ifndef GL_EXT_compiled_vertex_array
#define GL_EXT_compiled_vertex_array 1

#define GL_ARRAY_ELEMENT_LOCK_FIRST_EXT 0x81A8
#define GL_ARRAY_ELEMENT_LOCK_COUNT_EXT 0x81A9

typedef void (GLAPIENTRY * PFNGLLOCKARRAYSEXTPROC) (GLint first, GLsizei count);
typedef void (GLAPIENTRY * PFNGLUNLOCKARRAYSEXTPROC) (void);

#define glLockArraysEXT GLEW_GET_FUN(__glewLockArraysEXT)
#define glUnlockArraysEXT GLEW_GET_FUN(__glewUnlockArraysEXT)

#define GLEW_EXT_compiled_vertex_array GLEW_GET_VAR(__GLEW_EXT_compiled_vertex_array)

#endif /* GL_EXT_compiled_vertex_array */

/* ---------------- GL_EXT_compressed_ETC1_RGB8_sub_texture ---------------- */

#ifndef GL_EXT_compressed_ETC1_RGB8_sub_texture
#define GL_EXT_compressed_ETC1_RGB8_sub_texture 1

#define GLEW_EXT_compressed_ETC1_RGB8_sub_texture GLEW_GET_VAR(__GLEW_EXT_compressed_ETC1_RGB8_sub_texture)

#endif /* GL_EXT_compressed_ETC1_RGB8_sub_texture */

/* ----------------------- GL_EXT_conservative_depth ----------------------- */

#ifndef GL_EXT_conservative_depth
#define GL_EXT_conservative_depth 1

#define GLEW_EXT_conservative_depth GLEW_GET_VAR(__GLEW_EXT_conservative_depth)

#endif /* GL_EXT_conservative_depth */

/* --------------------------- GL_EXT_convolution -------------------------- */

#ifndef GL_EXT_convolution
#define GL_EXT_convolution 1

#define GL_CONVOLUTION_1D_EXT 0x8010
#define GL_CONVOLUTION_2D_EXT 0x8011
#define GL_SEPARABLE_2D_EXT 0x8012
#define GL_CONVOLUTION_BORDER_MODE_EXT 0x8013
#define GL_CONVOLUTION_FILTER_SCALE_EXT 0x8014
#define GL_CONVOLUTION_FILTER_BIAS_EXT 0x8015
#define GL_REDUCE_EXT 0x8016
#define GL_CONVOLUTION_FORMAT_EXT 0x8017
#define GL_CONVOLUTION_WIDTH_EXT 0x8018
#define GL_CONVOLUTION_HEIGHT_EXT 0x8019
#define GL_MAX_CONVOLUTION_WIDTH_EXT 0x801A
#define GL_MAX_CONVOLUTION_HEIGHT_EXT 0x801B
#define GL_POST_CONVOLUTION_RED_SCALE_EXT 0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE_EXT 0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE_EXT 0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE_EXT 0x801F
#define GL_POST_CONVOLUTION_RED_BIAS_EXT 0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS_EXT 0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS_EXT 0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS_EXT 0x8023

typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER1DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void *image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER2DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFEXTPROC) (GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFVEXTPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIEXTPROC) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIVEXTPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER1DEXTPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER2DEXTPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONFILTEREXTPROC) (GLenum target, GLenum format, GLenum type, void *image);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETSEPARABLEFILTEREXTPROC) (GLenum target, GLenum format, GLenum type, void *row, void *column, void *span);
typedef void (GLAPIENTRY * PFNGLSEPARABLEFILTER2DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *row, const void *column);

#define glConvolutionFilter1DEXT GLEW_GET_FUN(__glewConvolutionFilter1DEXT)
#define glConvolutionFilter2DEXT GLEW_GET_FUN(__glewConvolutionFilter2DEXT)
#define glConvolutionParameterfEXT GLEW_GET_FUN(__glewConvolutionParameterfEXT)
#define glConvolutionParameterfvEXT GLEW_GET_FUN(__glewConvolutionParameterfvEXT)
#define glConvolutionParameteriEXT GLEW_GET_FUN(__glewConvolutionParameteriEXT)
#define glConvolutionParameterivEXT GLEW_GET_FUN(__glewConvolutionParameterivEXT)
#define glCopyConvolutionFilter1DEXT GLEW_GET_FUN(__glewCopyConvolutionFilter1DEXT)
#define glCopyConvolutionFilter2DEXT GLEW_GET_FUN(__glewCopyConvolutionFilter2DEXT)
#define glGetConvolutionFilterEXT GLEW_GET_FUN(__glewGetConvolutionFilterEXT)
#define glGetConvolutionParameterfvEXT GLEW_GET_FUN(__glewGetConvolutionParameterfvEXT)
#define glGetConvolutionParameterivEXT GLEW_GET_FUN(__glewGetConvolutionParameterivEXT)
#define glGetSeparableFilterEXT GLEW_GET_FUN(__glewGetSeparableFilterEXT)
#define glSeparableFilter2DEXT GLEW_GET_FUN(__glewSeparableFilter2DEXT)

#define GLEW_EXT_convolution GLEW_GET_VAR(__GLEW_EXT_convolution)

#endif /* GL_EXT_convolution */

/* ------------------------ GL_EXT_coordinate_frame ------------------------ */

#ifndef GL_EXT_coordinate_frame
#define GL_EXT_coordinate_frame 1

#define GL_TANGENT_ARRAY_EXT 0x8439
#define GL_BINORMAL_ARRAY_EXT 0x843A
#define GL_CURRENT_TANGENT_EXT 0x843B
#define GL_CURRENT_BINORMAL_EXT 0x843C
#define GL_TANGENT_ARRAY_TYPE_EXT 0x843E
#define GL_TANGENT_ARRAY_STRIDE_EXT 0x843F
#define GL_BINORMAL_ARRAY_TYPE_EXT 0x8440
#define GL_BINORMAL_ARRAY_STRIDE_EXT 0x8441
#define GL_TANGENT_ARRAY_POINTER_EXT 0x8442
#define GL_BINORMAL_ARRAY_POINTER_EXT 0x8443
#define GL_MAP1_TANGENT_EXT 0x8444
#define GL_MAP2_TANGENT_EXT 0x8445
#define GL_MAP1_BINORMAL_EXT 0x8446
#define GL_MAP2_BINORMAL_EXT 0x8447

typedef void (GLAPIENTRY * PFNGLBINORMALPOINTEREXTPROC) (GLenum type, GLsizei stride, void *pointer);
typedef void (GLAPIENTRY * PFNGLTANGENTPOINTEREXTPROC) (GLenum type, GLsizei stride, void *pointer);

#define glBinormalPointerEXT GLEW_GET_FUN(__glewBinormalPointerEXT)
#define glTangentPointerEXT GLEW_GET_FUN(__glewTangentPointerEXT)

#define GLEW_EXT_coordinate_frame GLEW_GET_VAR(__GLEW_EXT_coordinate_frame)

#endif /* GL_EXT_coordinate_frame */

/* --------------------------- GL_EXT_copy_image --------------------------- */

#ifndef GL_EXT_copy_image
#define GL_EXT_copy_image 1

typedef void (GLAPIENTRY * PFNGLCOPYIMAGESUBDATAEXTPROC) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);

#define glCopyImageSubDataEXT GLEW_GET_FUN(__glewCopyImageSubDataEXT)

#define GLEW_EXT_copy_image GLEW_GET_VAR(__GLEW_EXT_copy_image)

#endif /* GL_EXT_copy_image */

/* -------------------------- GL_EXT_copy_texture -------------------------- */

#ifndef GL_EXT_copy_texture
#define GL_EXT_copy_texture 1

typedef void (GLAPIENTRY * PFNGLCOPYTEXIMAGE1DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXIMAGE2DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE1DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE2DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE3DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

#define glCopyTexImage1DEXT GLEW_GET_FUN(__glewCopyTexImage1DEXT)
#define glCopyTexImage2DEXT GLEW_GET_FUN(__glewCopyTexImage2DEXT)
#define glCopyTexSubImage1DEXT GLEW_GET_FUN(__glewCopyTexSubImage1DEXT)
#define glCopyTexSubImage2DEXT GLEW_GET_FUN(__glewCopyTexSubImage2DEXT)
#define glCopyTexSubImage3DEXT GLEW_GET_FUN(__glewCopyTexSubImage3DEXT)

#define GLEW_EXT_copy_texture GLEW_GET_VAR(__GLEW_EXT_copy_texture)

#endif /* GL_EXT_copy_texture */

/* --------------------------- GL_EXT_cull_vertex -------------------------- */

#ifndef GL_EXT_cull_vertex
#define GL_EXT_cull_vertex 1

#define GL_CULL_VERTEX_EXT 0x81AA
#define GL_CULL_VERTEX_EYE_POSITION_EXT 0x81AB
#define GL_CULL_VERTEX_OBJECT_POSITION_EXT 0x81AC

typedef void (GLAPIENTRY * PFNGLCULLPARAMETERDVEXTPROC) (GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLCULLPARAMETERFVEXTPROC) (GLenum pname, GLfloat* params);

#define glCullParameterdvEXT GLEW_GET_FUN(__glewCullParameterdvEXT)
#define glCullParameterfvEXT GLEW_GET_FUN(__glewCullParameterfvEXT)

#define GLEW_EXT_cull_vertex GLEW_GET_VAR(__GLEW_EXT_cull_vertex)

#endif /* GL_EXT_cull_vertex */

/* --------------------------- GL_EXT_debug_label -------------------------- */

#ifndef GL_EXT_debug_label
#define GL_EXT_debug_label 1

#define GL_PROGRAM_PIPELINE_OBJECT_EXT 0x8A4F
#define GL_PROGRAM_OBJECT_EXT 0x8B40
#define GL_SHADER_OBJECT_EXT 0x8B48
#define GL_BUFFER_OBJECT_EXT 0x9151
#define GL_QUERY_OBJECT_EXT 0x9153
#define GL_VERTEX_ARRAY_OBJECT_EXT 0x9154

typedef void (GLAPIENTRY * PFNGLGETOBJECTLABELEXTPROC) (GLenum type, GLuint object, GLsizei bufSize, GLsizei* length, GLchar *label);
typedef void (GLAPIENTRY * PFNGLLABELOBJECTEXTPROC) (GLenum type, GLuint object, GLsizei length, const GLchar* label);

#define glGetObjectLabelEXT GLEW_GET_FUN(__glewGetObjectLabelEXT)
#define glLabelObjectEXT GLEW_GET_FUN(__glewLabelObjectEXT)

#define GLEW_EXT_debug_label GLEW_GET_VAR(__GLEW_EXT_debug_label)

#endif /* GL_EXT_debug_label */

/* -------------------------- GL_EXT_debug_marker -------------------------- */

#ifndef GL_EXT_debug_marker
#define GL_EXT_debug_marker 1

typedef void (GLAPIENTRY * PFNGLINSERTEVENTMARKEREXTPROC) (GLsizei length, const GLchar* marker);
typedef void (GLAPIENTRY * PFNGLPOPGROUPMARKEREXTPROC) (void);
typedef void (GLAPIENTRY * PFNGLPUSHGROUPMARKEREXTPROC) (GLsizei length, const GLchar* marker);

#define glInsertEventMarkerEXT GLEW_GET_FUN(__glewInsertEventMarkerEXT)
#define glPopGroupMarkerEXT GLEW_GET_FUN(__glewPopGroupMarkerEXT)
#define glPushGroupMarkerEXT GLEW_GET_FUN(__glewPushGroupMarkerEXT)

#define GLEW_EXT_debug_marker GLEW_GET_VAR(__GLEW_EXT_debug_marker)

#endif /* GL_EXT_debug_marker */

/* ------------------------ GL_EXT_depth_bounds_test ----------------------- */

#ifndef GL_EXT_depth_bounds_test
#define GL_EXT_depth_bounds_test 1

#define GL_DEPTH_BOUNDS_TEST_EXT 0x8890
#define GL_DEPTH_BOUNDS_EXT 0x8891

typedef void (GLAPIENTRY * PFNGLDEPTHBOUNDSEXTPROC) (GLclampd zmin, GLclampd zmax);

#define glDepthBoundsEXT GLEW_GET_FUN(__glewDepthBoundsEXT)

#define GLEW_EXT_depth_bounds_test GLEW_GET_VAR(__GLEW_EXT_depth_bounds_test)

#endif /* GL_EXT_depth_bounds_test */

/* ----------------------- GL_EXT_direct_state_access ---------------------- */

#ifndef GL_EXT_direct_state_access
#define GL_EXT_direct_state_access 1

#define GL_PROGRAM_MATRIX_EXT 0x8E2D
#define GL_TRANSPOSE_PROGRAM_MATRIX_EXT 0x8E2E
#define GL_PROGRAM_MATRIX_STACK_DEPTH_EXT 0x8E2F

typedef void (GLAPIENTRY * PFNGLBINDMULTITEXTUREEXTPROC) (GLenum texunit, GLenum target, GLuint texture);
typedef GLenum (GLAPIENTRY * PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC) (GLuint framebuffer, GLenum target);
typedef void (GLAPIENTRY * PFNGLCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLECLIENTSTATEIEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC) (GLuint vaobj, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXARRAYEXTPROC) (GLuint vaobj, GLenum array);
typedef void (GLAPIENTRY * PFNGLENABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLECLIENTSTATEIEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXARRAYATTRIBEXTPROC) (GLuint vaobj, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXARRAYEXTPROC) (GLuint vaobj, GLenum array);
typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC) (GLuint framebuffer, GLsizei n, const GLenum* bufs);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERREADBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * PFNGLGENERATEMULTITEXMIPMAPEXTPROC) (GLenum texunit, GLenum target);
typedef void (GLAPIENTRY * PFNGLGENERATETEXTUREMIPMAPEXTPROC) (GLuint texture, GLenum target);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint level, void *img);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC) (GLuint texture, GLenum target, GLint level, void *img);
typedef void (GLAPIENTRY * PFNGLGETDOUBLEINDEXEDVEXTPROC) (GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETDOUBLEI_VEXTPROC) (GLenum pname, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETFLOATINDEXEDVEXTPROC) (GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFLOATI_VEXTPROC) (GLenum pname, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC) (GLuint buffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPOINTERVEXTPROC) (GLuint buffer, GLenum pname, void** params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);
typedef void (GLAPIENTRY * PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum pname, void *string);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMIVEXTPROC) (GLuint program, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC) (GLuint renderbuffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETPOINTERINDEXEDVEXTPROC) (GLenum target, GLuint index, void** params);
typedef void (GLAPIENTRY * PFNGLGETPOINTERI_VEXTPROC) (GLenum pname, GLuint index, void** params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREIMAGEEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYINTEGERVEXTPROC) (GLuint vaobj, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, void** param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYPOINTERVEXTPROC) (GLuint vaobj, GLenum pname, void** param);
typedef void * (GLAPIENTRY * PFNGLMAPNAMEDBUFFEREXTPROC) (GLuint buffer, GLenum access);
typedef void * (GLAPIENTRY * PFNGLMAPNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (GLAPIENTRY * PFNGLMATRIXFRUSTUMEXTPROC) (GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADIDENTITYEXTPROC) (GLenum matrixMode);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADTRANSPOSEDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADTRANSPOSEFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTTRANSPOSEDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTTRANSPOSEFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXORTHOEXTPROC) (GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
typedef void (GLAPIENTRY * PFNGLMATRIXPOPEXTPROC) (GLenum matrixMode);
typedef void (GLAPIENTRY * PFNGLMATRIXPUSHEXTPROC) (GLenum matrixMode);
typedef void (GLAPIENTRY * PFNGLMATRIXROTATEDEXTPROC) (GLenum matrixMode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXROTATEFEXTPROC) (GLenum matrixMode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMATRIXSCALEDEXTPROC) (GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXSCALEFEXTPROC) (GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMATRIXTRANSLATEDEXTPROC) (GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXTRANSLATEFEXTPROC) (GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMULTITEXBUFFEREXTPROC) (GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDPOINTEREXTPROC) (GLenum texunit, GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENDEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENFEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENIEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLfloat* param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* param);
typedef void (GLAPIENTRY * PFNGLMULTITEXRENDERBUFFEREXTPROC) (GLenum texunit, GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERDATAEXTPROC) (GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
typedef void (GLAPIENTRY * PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC) (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC) (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC) (GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum format, GLsizei len, const void *string);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC) (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC) (GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FEXTPROC) (GLuint program, GLint location, GLfloat v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IEXTPROC) (GLuint program, GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIEXTPROC) (GLuint program, GLint location, GLuint v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLTEXTUREBUFFEREXTPROC) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLfloat* param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLint* param);
typedef void (GLAPIENTRY * PFNGLTEXTURERENDERBUFFEREXTPROC) (GLuint texture, GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
typedef GLboolean (GLAPIENTRY * PFNGLUNMAPNAMEDBUFFEREXTPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYCOLOROFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYEDGEFLAGOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYFOGCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYINDEXOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYMULTITEXCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYNORMALOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYSECONDARYCOLOROFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYTEXCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBDIVISOREXTPROC) (GLuint vaobj, GLuint index, GLuint divisor);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);

#define glBindMultiTextureEXT GLEW_GET_FUN(__glewBindMultiTextureEXT)
#define glCheckNamedFramebufferStatusEXT GLEW_GET_FUN(__glewCheckNamedFramebufferStatusEXT)
#define glClientAttribDefaultEXT GLEW_GET_FUN(__glewClientAttribDefaultEXT)
#define glCompressedMultiTexImage1DEXT GLEW_GET_FUN(__glewCompressedMultiTexImage1DEXT)
#define glCompressedMultiTexImage2DEXT GLEW_GET_FUN(__glewCompressedMultiTexImage2DEXT)
#define glCompressedMultiTexImage3DEXT GLEW_GET_FUN(__glewCompressedMultiTexImage3DEXT)
#define glCompressedMultiTexSubImage1DEXT GLEW_GET_FUN(__glewCompressedMultiTexSubImage1DEXT)
#define glCompressedMultiTexSubImage2DEXT GLEW_GET_FUN(__glewCompressedMultiTexSubImage2DEXT)
#define glCompressedMultiTexSubImage3DEXT GLEW_GET_FUN(__glewCompressedMultiTexSubImage3DEXT)
#define glCompressedTextureImage1DEXT GLEW_GET_FUN(__glewCompressedTextureImage1DEXT)
#define glCompressedTextureImage2DEXT GLEW_GET_FUN(__glewCompressedTextureImage2DEXT)
#define glCompressedTextureImage3DEXT GLEW_GET_FUN(__glewCompressedTextureImage3DEXT)
#define glCompressedTextureSubImage1DEXT GLEW_GET_FUN(__glewCompressedTextureSubImage1DEXT)
#define glCompressedTextureSubImage2DEXT GLEW_GET_FUN(__glewCompressedTextureSubImage2DEXT)
#define glCompressedTextureSubImage3DEXT GLEW_GET_FUN(__glewCompressedTextureSubImage3DEXT)
#define glCopyMultiTexImage1DEXT GLEW_GET_FUN(__glewCopyMultiTexImage1DEXT)
#define glCopyMultiTexImage2DEXT GLEW_GET_FUN(__glewCopyMultiTexImage2DEXT)
#define glCopyMultiTexSubImage1DEXT GLEW_GET_FUN(__glewCopyMultiTexSubImage1DEXT)
#define glCopyMultiTexSubImage2DEXT GLEW_GET_FUN(__glewCopyMultiTexSubImage2DEXT)
#define glCopyMultiTexSubImage3DEXT GLEW_GET_FUN(__glewCopyMultiTexSubImage3DEXT)
#define glCopyTextureImage1DEXT GLEW_GET_FUN(__glewCopyTextureImage1DEXT)
#define glCopyTextureImage2DEXT GLEW_GET_FUN(__glewCopyTextureImage2DEXT)
#define glCopyTextureSubImage1DEXT GLEW_GET_FUN(__glewCopyTextureSubImage1DEXT)
#define glCopyTextureSubImage2DEXT GLEW_GET_FUN(__glewCopyTextureSubImage2DEXT)
#define glCopyTextureSubImage3DEXT GLEW_GET_FUN(__glewCopyTextureSubImage3DEXT)
#define glDisableClientStateIndexedEXT GLEW_GET_FUN(__glewDisableClientStateIndexedEXT)
#define glDisableClientStateiEXT GLEW_GET_FUN(__glewDisableClientStateiEXT)
#define glDisableVertexArrayAttribEXT GLEW_GET_FUN(__glewDisableVertexArrayAttribEXT)
#define glDisableVertexArrayEXT GLEW_GET_FUN(__glewDisableVertexArrayEXT)
#define glEnableClientStateIndexedEXT GLEW_GET_FUN(__glewEnableClientStateIndexedEXT)
#define glEnableClientStateiEXT GLEW_GET_FUN(__glewEnableClientStateiEXT)
#define glEnableVertexArrayAttribEXT GLEW_GET_FUN(__glewEnableVertexArrayAttribEXT)
#define glEnableVertexArrayEXT GLEW_GET_FUN(__glewEnableVertexArrayEXT)
#define glFlushMappedNamedBufferRangeEXT GLEW_GET_FUN(__glewFlushMappedNamedBufferRangeEXT)
#define glFramebufferDrawBufferEXT GLEW_GET_FUN(__glewFramebufferDrawBufferEXT)
#define glFramebufferDrawBuffersEXT GLEW_GET_FUN(__glewFramebufferDrawBuffersEXT)
#define glFramebufferReadBufferEXT GLEW_GET_FUN(__glewFramebufferReadBufferEXT)
#define glGenerateMultiTexMipmapEXT GLEW_GET_FUN(__glewGenerateMultiTexMipmapEXT)
#define glGenerateTextureMipmapEXT GLEW_GET_FUN(__glewGenerateTextureMipmapEXT)
#define glGetCompressedMultiTexImageEXT GLEW_GET_FUN(__glewGetCompressedMultiTexImageEXT)
#define glGetCompressedTextureImageEXT GLEW_GET_FUN(__glewGetCompressedTextureImageEXT)
#define glGetDoubleIndexedvEXT GLEW_GET_FUN(__glewGetDoubleIndexedvEXT)
#define glGetDoublei_vEXT GLEW_GET_FUN(__glewGetDoublei_vEXT)
#define glGetFloatIndexedvEXT GLEW_GET_FUN(__glewGetFloatIndexedvEXT)
#define glGetFloati_vEXT GLEW_GET_FUN(__glewGetFloati_vEXT)
#define glGetFramebufferParameterivEXT GLEW_GET_FUN(__glewGetFramebufferParameterivEXT)
#define glGetMultiTexEnvfvEXT GLEW_GET_FUN(__glewGetMultiTexEnvfvEXT)
#define glGetMultiTexEnvivEXT GLEW_GET_FUN(__glewGetMultiTexEnvivEXT)
#define glGetMultiTexGendvEXT GLEW_GET_FUN(__glewGetMultiTexGendvEXT)
#define glGetMultiTexGenfvEXT GLEW_GET_FUN(__glewGetMultiTexGenfvEXT)
#define glGetMultiTexGenivEXT GLEW_GET_FUN(__glewGetMultiTexGenivEXT)
#define glGetMultiTexImageEXT GLEW_GET_FUN(__glewGetMultiTexImageEXT)
#define glGetMultiTexLevelParameterfvEXT GLEW_GET_FUN(__glewGetMultiTexLevelParameterfvEXT)
#define glGetMultiTexLevelParameterivEXT GLEW_GET_FUN(__glewGetMultiTexLevelParameterivEXT)
#define glGetMultiTexParameterIivEXT GLEW_GET_FUN(__glewGetMultiTexParameterIivEXT)
#define glGetMultiTexParameterIuivEXT GLEW_GET_FUN(__glewGetMultiTexParameterIuivEXT)
#define glGetMultiTexParameterfvEXT GLEW_GET_FUN(__glewGetMultiTexParameterfvEXT)
#define glGetMultiTexParameterivEXT GLEW_GET_FUN(__glewGetMultiTexParameterivEXT)
#define glGetNamedBufferParameterivEXT GLEW_GET_FUN(__glewGetNamedBufferParameterivEXT)
#define glGetNamedBufferPointervEXT GLEW_GET_FUN(__glewGetNamedBufferPointervEXT)
#define glGetNamedBufferSubDataEXT GLEW_GET_FUN(__glewGetNamedBufferSubDataEXT)
#define glGetNamedFramebufferAttachmentParameterivEXT GLEW_GET_FUN(__glewGetNamedFramebufferAttachmentParameterivEXT)
#define glGetNamedProgramLocalParameterIivEXT GLEW_GET_FUN(__glewGetNamedProgramLocalParameterIivEXT)
#define glGetNamedProgramLocalParameterIuivEXT GLEW_GET_FUN(__glewGetNamedProgramLocalParameterIuivEXT)
#define glGetNamedProgramLocalParameterdvEXT GLEW_GET_FUN(__glewGetNamedProgramLocalParameterdvEXT)
#define glGetNamedProgramLocalParameterfvEXT GLEW_GET_FUN(__glewGetNamedProgramLocalParameterfvEXT)
#define glGetNamedProgramStringEXT GLEW_GET_FUN(__glewGetNamedProgramStringEXT)
#define glGetNamedProgramivEXT GLEW_GET_FUN(__glewGetNamedProgramivEXT)
#define glGetNamedRenderbufferParameterivEXT GLEW_GET_FUN(__glewGetNamedRenderbufferParameterivEXT)
#define glGetPointerIndexedvEXT GLEW_GET_FUN(__glewGetPointerIndexedvEXT)
#define glGetPointeri_vEXT GLEW_GET_FUN(__glewGetPointeri_vEXT)
#define glGetTextureImageEXT GLEW_GET_FUN(__glewGetTextureImageEXT)
#define glGetTextureLevelParameterfvEXT GLEW_GET_FUN(__glewGetTextureLevelParameterfvEXT)
#define glGetTextureLevelParameterivEXT GLEW_GET_FUN(__glewGetTextureLevelParameterivEXT)
#define glGetTextureParameterIivEXT GLEW_GET_FUN(__glewGetTextureParameterIivEXT)
#define glGetTextureParameterIuivEXT GLEW_GET_FUN(__glewGetTextureParameterIuivEXT)
#define glGetTextureParameterfvEXT GLEW_GET_FUN(__glewGetTextureParameterfvEXT)
#define glGetTextureParameterivEXT GLEW_GET_FUN(__glewGetTextureParameterivEXT)
#define glGetVertexArrayIntegeri_vEXT GLEW_GET_FUN(__glewGetVertexArrayIntegeri_vEXT)
#define glGetVertexArrayIntegervEXT GLEW_GET_FUN(__glewGetVertexArrayIntegervEXT)
#define glGetVertexArrayPointeri_vEXT GLEW_GET_FUN(__glewGetVertexArrayPointeri_vEXT)
#define glGetVertexArrayPointervEXT GLEW_GET_FUN(__glewGetVertexArrayPointervEXT)
#define glMapNamedBufferEXT GLEW_GET_FUN(__glewMapNamedBufferEXT)
#define glMapNamedBufferRangeEXT GLEW_GET_FUN(__glewMapNamedBufferRangeEXT)
#define glMatrixFrustumEXT GLEW_GET_FUN(__glewMatrixFrustumEXT)
#define glMatrixLoadIdentityEXT GLEW_GET_FUN(__glewMatrixLoadIdentityEXT)
#define glMatrixLoadTransposedEXT GLEW_GET_FUN(__glewMatrixLoadTransposedEXT)
#define glMatrixLoadTransposefEXT GLEW_GET_FUN(__glewMatrixLoadTransposefEXT)
#define glMatrixLoaddEXT GLEW_GET_FUN(__glewMatrixLoaddEXT)
#define glMatrixLoadfEXT GLEW_GET_FUN(__glewMatrixLoadfEXT)
#define glMatrixMultTransposedEXT GLEW_GET_FUN(__glewMatrixMultTransposedEXT)
#define glMatrixMultTransposefEXT GLEW_GET_FUN(__glewMatrixMultTransposefEXT)
#define glMatrixMultdEXT GLEW_GET_FUN(__glewMatrixMultdEXT)
#define glMatrixMultfEXT GLEW_GET_FUN(__glewMatrixMultfEXT)
#define glMatrixOrthoEXT GLEW_GET_FUN(__glewMatrixOrthoEXT)
#define glMatrixPopEXT GLEW_GET_FUN(__glewMatrixPopEXT)
#define glMatrixPushEXT GLEW_GET_FUN(__glewMatrixPushEXT)
#define glMatrixRotatedEXT GLEW_GET_FUN(__glewMatrixRotatedEXT)
#define glMatrixRotatefEXT GLEW_GET_FUN(__glewMatrixRotatefEXT)
#define glMatrixScaledEXT GLEW_GET_FUN(__glewMatrixScaledEXT)
#define glMatrixScalefEXT GLEW_GET_FUN(__glewMatrixScalefEXT)
#define glMatrixTranslatedEXT GLEW_GET_FUN(__glewMatrixTranslatedEXT)
#define glMatrixTranslatefEXT GLEW_GET_FUN(__glewMatrixTranslatefEXT)
#define glMultiTexBufferEXT GLEW_GET_FUN(__glewMultiTexBufferEXT)
#define glMultiTexCoordPointerEXT GLEW_GET_FUN(__glewMultiTexCoordPointerEXT)
#define glMultiTexEnvfEXT GLEW_GET_FUN(__glewMultiTexEnvfEXT)
#define glMultiTexEnvfvEXT GLEW_GET_FUN(__glewMultiTexEnvfvEXT)
#define glMultiTexEnviEXT GLEW_GET_FUN(__glewMultiTexEnviEXT)
#define glMultiTexEnvivEXT GLEW_GET_FUN(__glewMultiTexEnvivEXT)
#define glMultiTexGendEXT GLEW_GET_FUN(__glewMultiTexGendEXT)
#define glMultiTexGendvEXT GLEW_GET_FUN(__glewMultiTexGendvEXT)
#define glMultiTexGenfEXT GLEW_GET_FUN(__glewMultiTexGenfEXT)
#define glMultiTexGenfvEXT GLEW_GET_FUN(__glewMultiTexGenfvEXT)
#define glMultiTexGeniEXT GLEW_GET_FUN(__glewMultiTexGeniEXT)
#define glMultiTexGenivEXT GLEW_GET_FUN(__glewMultiTexGenivEXT)
#define glMultiTexImage1DEXT GLEW_GET_FUN(__glewMultiTexImage1DEXT)
#define glMultiTexImage2DEXT GLEW_GET_FUN(__glewMultiTexImage2DEXT)
#define glMultiTexImage3DEXT GLEW_GET_FUN(__glewMultiTexImage3DEXT)
#define glMultiTexParameterIivEXT GLEW_GET_FUN(__glewMultiTexParameterIivEXT)
#define glMultiTexParameterIuivEXT GLEW_GET_FUN(__glewMultiTexParameterIuivEXT)
#define glMultiTexParameterfEXT GLEW_GET_FUN(__glewMultiTexParameterfEXT)
#define glMultiTexParameterfvEXT GLEW_GET_FUN(__glewMultiTexParameterfvEXT)
#define glMultiTexParameteriEXT GLEW_GET_FUN(__glewMultiTexParameteriEXT)
#define glMultiTexParameterivEXT GLEW_GET_FUN(__glewMultiTexParameterivEXT)
#define glMultiTexRenderbufferEXT GLEW_GET_FUN(__glewMultiTexRenderbufferEXT)
#define glMultiTexSubImage1DEXT GLEW_GET_FUN(__glewMultiTexSubImage1DEXT)
#define glMultiTexSubImage2DEXT GLEW_GET_FUN(__glewMultiTexSubImage2DEXT)
#define glMultiTexSubImage3DEXT GLEW_GET_FUN(__glewMultiTexSubImage3DEXT)
#define glNamedBufferDataEXT GLEW_GET_FUN(__glewNamedBufferDataEXT)
#define glNamedBufferSubDataEXT GLEW_GET_FUN(__glewNamedBufferSubDataEXT)
#define glNamedCopyBufferSubDataEXT GLEW_GET_FUN(__glewNamedCopyBufferSubDataEXT)
#define glNamedFramebufferRenderbufferEXT GLEW_GET_FUN(__glewNamedFramebufferRenderbufferEXT)
#define glNamedFramebufferTexture1DEXT GLEW_GET_FUN(__glewNamedFramebufferTexture1DEXT)
#define glNamedFramebufferTexture2DEXT GLEW_GET_FUN(__glewNamedFramebufferTexture2DEXT)
#define glNamedFramebufferTexture3DEXT GLEW_GET_FUN(__glewNamedFramebufferTexture3DEXT)
#define glNamedFramebufferTextureEXT GLEW_GET_FUN(__glewNamedFramebufferTextureEXT)
#define glNamedFramebufferTextureFaceEXT GLEW_GET_FUN(__glewNamedFramebufferTextureFaceEXT)
#define glNamedFramebufferTextureLayerEXT GLEW_GET_FUN(__glewNamedFramebufferTextureLayerEXT)
#define glNamedProgramLocalParameter4dEXT GLEW_GET_FUN(__glewNamedProgramLocalParameter4dEXT)
#define glNamedProgramLocalParameter4dvEXT GLEW_GET_FUN(__glewNamedProgramLocalParameter4dvEXT)
#define glNamedProgramLocalParameter4fEXT GLEW_GET_FUN(__glewNamedProgramLocalParameter4fEXT)
#define glNamedProgramLocalParameter4fvEXT GLEW_GET_FUN(__glewNamedProgramLocalParameter4fvEXT)
#define glNamedProgramLocalParameterI4iEXT GLEW_GET_FUN(__glewNamedProgramLocalParameterI4iEXT)
#define glNamedProgramLocalParameterI4ivEXT GLEW_GET_FUN(__glewNamedProgramLocalParameterI4ivEXT)
#define glNamedProgramLocalParameterI4uiEXT GLEW_GET_FUN(__glewNamedProgramLocalParameterI4uiEXT)
#define glNamedProgramLocalParameterI4uivEXT GLEW_GET_FUN(__glewNamedProgramLocalParameterI4uivEXT)
#define glNamedProgramLocalParameters4fvEXT GLEW_GET_FUN(__glewNamedProgramLocalParameters4fvEXT)
#define glNamedProgramLocalParametersI4ivEXT GLEW_GET_FUN(__glewNamedProgramLocalParametersI4ivEXT)
#define glNamedProgramLocalParametersI4uivEXT GLEW_GET_FUN(__glewNamedProgramLocalParametersI4uivEXT)
#define glNamedProgramStringEXT GLEW_GET_FUN(__glewNamedProgramStringEXT)
#define glNamedRenderbufferStorageEXT GLEW_GET_FUN(__glewNamedRenderbufferStorageEXT)
#define glNamedRenderbufferStorageMultisampleCoverageEXT GLEW_GET_FUN(__glewNamedRenderbufferStorageMultisampleCoverageEXT)
#define glNamedRenderbufferStorageMultisampleEXT GLEW_GET_FUN(__glewNamedRenderbufferStorageMultisampleEXT)
#define glProgramUniform1fEXT GLEW_GET_FUN(__glewProgramUniform1fEXT)
#define glProgramUniform1fvEXT GLEW_GET_FUN(__glewProgramUniform1fvEXT)
#define glProgramUniform1iEXT GLEW_GET_FUN(__glewProgramUniform1iEXT)
#define glProgramUniform1ivEXT GLEW_GET_FUN(__glewProgramUniform1ivEXT)
#define glProgramUniform1uiEXT GLEW_GET_FUN(__glewProgramUniform1uiEXT)
#define glProgramUniform1uivEXT GLEW_GET_FUN(__glewProgramUniform1uivEXT)
#define glProgramUniform2fEXT GLEW_GET_FUN(__glewProgramUniform2fEXT)
#define glProgramUniform2fvEXT GLEW_GET_FUN(__glewProgramUniform2fvEXT)
#define glProgramUniform2iEXT GLEW_GET_FUN(__glewProgramUniform2iEXT)
#define glProgramUniform2ivEXT GLEW_GET_FUN(__glewProgramUniform2ivEXT)
#define glProgramUniform2uiEXT GLEW_GET_FUN(__glewProgramUniform2uiEXT)
#define glProgramUniform2uivEXT GLEW_GET_FUN(__glewProgramUniform2uivEXT)
#define glProgramUniform3fEXT GLEW_GET_FUN(__glewProgramUniform3fEXT)
#define glProgramUniform3fvEXT GLEW_GET_FUN(__glewProgramUniform3fvEXT)
#define glProgramUniform3iEXT GLEW_GET_FUN(__glewProgramUniform3iEXT)
#define glProgramUniform3ivEXT GLEW_GET_FUN(__glewProgramUniform3ivEXT)
#define glProgramUniform3uiEXT GLEW_GET_FUN(__glewProgramUniform3uiEXT)
#define glProgramUniform3uivEXT GLEW_GET_FUN(__glewProgramUniform3uivEXT)
#define glProgramUniform4fEXT GLEW_GET_FUN(__glewProgramUniform4fEXT)
#define glProgramUniform4fvEXT GLEW_GET_FUN(__glewProgramUniform4fvEXT)
#define glProgramUniform4iEXT GLEW_GET_FUN(__glewProgramUniform4iEXT)
#define glProgramUniform4ivEXT GLEW_GET_FUN(__glewProgramUniform4ivEXT)
#define glProgramUniform4uiEXT GLEW_GET_FUN(__glewProgramUniform4uiEXT)
#define glProgramUniform4uivEXT GLEW_GET_FUN(__glewProgramUniform4uivEXT)
#define glProgramUniformMatrix2fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix2fvEXT)
#define glProgramUniformMatrix2x3fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix2x3fvEXT)
#define glProgramUniformMatrix2x4fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix2x4fvEXT)
#define glProgramUniformMatrix3fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix3fvEXT)
#define glProgramUniformMatrix3x2fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix3x2fvEXT)
#define glProgramUniformMatrix3x4fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix3x4fvEXT)
#define glProgramUniformMatrix4fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix4fvEXT)
#define glProgramUniformMatrix4x2fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix4x2fvEXT)
#define glProgramUniformMatrix4x3fvEXT GLEW_GET_FUN(__glewProgramUniformMatrix4x3fvEXT)
#define glPushClientAttribDefaultEXT GLEW_GET_FUN(__glewPushClientAttribDefaultEXT)
#define glTextureBufferEXT GLEW_GET_FUN(__glewTextureBufferEXT)
#define glTextureImage1DEXT GLEW_GET_FUN(__glewTextureImage1DEXT)
#define glTextureImage2DEXT GLEW_GET_FUN(__glewTextureImage2DEXT)
#define glTextureImage3DEXT GLEW_GET_FUN(__glewTextureImage3DEXT)
#define glTextureParameterIivEXT GLEW_GET_FUN(__glewTextureParameterIivEXT)
#define glTextureParameterIuivEXT GLEW_GET_FUN(__glewTextureParameterIuivEXT)
#define glTextureParameterfEXT GLEW_GET_FUN(__glewTextureParameterfEXT)
#define glTextureParameterfvEXT GLEW_GET_FUN(__glewTextureParameterfvEXT)
#define glTextureParameteriEXT GLEW_GET_FUN(__glewTextureParameteriEXT)
#define glTextureParameterivEXT GLEW_GET_FUN(__glewTextureParameterivEXT)
#define glTextureRenderbufferEXT GLEW_GET_FUN(__glewTextureRenderbufferEXT)
#define glTextureSubImage1DEXT GLEW_GET_FUN(__glewTextureSubImage1DEXT)
#define glTextureSubImage2DEXT GLEW_GET_FUN(__glewTextureSubImage2DEXT)
#define glTextureSubImage3DEXT GLEW_GET_FUN(__glewTextureSubImage3DEXT)
#define glUnmapNamedBufferEXT GLEW_GET_FUN(__glewUnmapNamedBufferEXT)
#define glVertexArrayColorOffsetEXT GLEW_GET_FUN(__glewVertexArrayColorOffsetEXT)
#define glVertexArrayEdgeFlagOffsetEXT GLEW_GET_FUN(__glewVertexArrayEdgeFlagOffsetEXT)
#define glVertexArrayFogCoordOffsetEXT GLEW_GET_FUN(__glewVertexArrayFogCoordOffsetEXT)
#define glVertexArrayIndexOffsetEXT GLEW_GET_FUN(__glewVertexArrayIndexOffsetEXT)
#define glVertexArrayMultiTexCoordOffsetEXT GLEW_GET_FUN(__glewVertexArrayMultiTexCoordOffsetEXT)
#define glVertexArrayNormalOffsetEXT GLEW_GET_FUN(__glewVertexArrayNormalOffsetEXT)
#define glVertexArraySecondaryColorOffsetEXT GLEW_GET_FUN(__glewVertexArraySecondaryColorOffsetEXT)
#define glVertexArrayTexCoordOffsetEXT GLEW_GET_FUN(__glewVertexArrayTexCoordOffsetEXT)
#define glVertexArrayVertexAttribDivisorEXT GLEW_GET_FUN(__glewVertexArrayVertexAttribDivisorEXT)
#define glVertexArrayVertexAttribIOffsetEXT GLEW_GET_FUN(__glewVertexArrayVertexAttribIOffsetEXT)
#define glVertexArrayVertexAttribOffsetEXT GLEW_GET_FUN(__glewVertexArrayVertexAttribOffsetEXT)
#define glVertexArrayVertexOffsetEXT GLEW_GET_FUN(__glewVertexArrayVertexOffsetEXT)

#define GLEW_EXT_direct_state_access GLEW_GET_VAR(__GLEW_EXT_direct_state_access)

#endif /* GL_EXT_direct_state_access */

/* ----------------------- GL_EXT_discard_framebuffer ---------------------- */

#ifndef GL_EXT_discard_framebuffer
#define GL_EXT_discard_framebuffer 1

#define GL_COLOR_EXT 0x1800
#define GL_DEPTH_EXT 0x1801
#define GL_STENCIL_EXT 0x1802

typedef void (GLAPIENTRY * PFNGLDISCARDFRAMEBUFFEREXTPROC) (GLenum target, GLsizei numAttachments, const GLenum* attachments);

#define glDiscardFramebufferEXT GLEW_GET_FUN(__glewDiscardFramebufferEXT)

#define GLEW_EXT_discard_framebuffer GLEW_GET_VAR(__GLEW_EXT_discard_framebuffer)

#endif /* GL_EXT_discard_framebuffer */

/* -------------------------- GL_EXT_draw_buffers -------------------------- */

#ifndef GL_EXT_draw_buffers
#define GL_EXT_draw_buffers 1

#define GL_MAX_DRAW_BUFFERS_EXT 0x8824
#define GL_DRAW_BUFFER0_EXT 0x8825
#define GL_DRAW_BUFFER1_EXT 0x8826
#define GL_DRAW_BUFFER2_EXT 0x8827
#define GL_DRAW_BUFFER3_EXT 0x8828
#define GL_DRAW_BUFFER4_EXT 0x8829
#define GL_DRAW_BUFFER5_EXT 0x882A
#define GL_DRAW_BUFFER6_EXT 0x882B
#define GL_DRAW_BUFFER7_EXT 0x882C
#define GL_DRAW_BUFFER8_EXT 0x882D
#define GL_DRAW_BUFFER9_EXT 0x882E
#define GL_DRAW_BUFFER10_EXT 0x882F
#define GL_DRAW_BUFFER11_EXT 0x8830
#define GL_DRAW_BUFFER12_EXT 0x8831
#define GL_DRAW_BUFFER13_EXT 0x8832
#define GL_DRAW_BUFFER14_EXT 0x8833
#define GL_DRAW_BUFFER15_EXT 0x8834
#define GL_MAX_COLOR_ATTACHMENTS_EXT 0x8CDF
#define GL_COLOR_ATTACHMENT0_EXT 0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT 0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT 0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT 0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT 0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT 0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT 0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT 0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT 0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT 0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT 0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT 0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT 0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT 0x8CED
#define GL_COLOR_ATTACHMENT14_EXT 0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT 0x8CEF

typedef void (GLAPIENTRY * PFNGLDRAWBUFFERSEXTPROC) (GLsizei n, const GLenum* bufs);

#define glDrawBuffersEXT GLEW_GET_FUN(__glewDrawBuffersEXT)

#define GLEW_EXT_draw_buffers GLEW_GET_VAR(__GLEW_EXT_draw_buffers)

#endif /* GL_EXT_draw_buffers */

/* -------------------------- GL_EXT_draw_buffers2 ------------------------- */

#ifndef GL_EXT_draw_buffers2
#define GL_EXT_draw_buffers2 1

typedef void (GLAPIENTRY * PFNGLCOLORMASKINDEXEDEXTPROC) (GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void (GLAPIENTRY * PFNGLDISABLEINDEXEDEXTPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEINDEXEDEXTPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLGETBOOLEANINDEXEDVEXTPROC) (GLenum value, GLuint index, GLboolean* data);
typedef void (GLAPIENTRY * PFNGLGETINTEGERINDEXEDVEXTPROC) (GLenum value, GLuint index, GLint* data);
typedef GLboolean (GLAPIENTRY * PFNGLISENABLEDINDEXEDEXTPROC) (GLenum target, GLuint index);

#define glColorMaskIndexedEXT GLEW_GET_FUN(__glewColorMaskIndexedEXT)
#define glDisableIndexedEXT GLEW_GET_FUN(__glewDisableIndexedEXT)
#define glEnableIndexedEXT GLEW_GET_FUN(__glewEnableIndexedEXT)
#define glGetBooleanIndexedvEXT GLEW_GET_FUN(__glewGetBooleanIndexedvEXT)
#define glGetIntegerIndexedvEXT GLEW_GET_FUN(__glewGetIntegerIndexedvEXT)
#define glIsEnabledIndexedEXT GLEW_GET_FUN(__glewIsEnabledIndexedEXT)

#define GLEW_EXT_draw_buffers2 GLEW_GET_VAR(__GLEW_EXT_draw_buffers2)

#endif /* GL_EXT_draw_buffers2 */

/* ---------------------- GL_EXT_draw_buffers_indexed ---------------------- */

#ifndef GL_EXT_draw_buffers_indexed
#define GL_EXT_draw_buffers_indexed 1

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEIEXTPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONIEXTPROC) (GLuint buf, GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEIEXTPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCIEXTPROC) (GLuint buf, GLenum src, GLenum dst);
typedef void (GLAPIENTRY * PFNGLCOLORMASKIEXTPROC) (GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void (GLAPIENTRY * PFNGLDISABLEIEXTPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEIEXTPROC) (GLenum target, GLuint index);
typedef GLboolean (GLAPIENTRY * PFNGLISENABLEDIEXTPROC) (GLenum target, GLuint index);

#define glBlendEquationSeparateiEXT GLEW_GET_FUN(__glewBlendEquationSeparateiEXT)
#define glBlendEquationiEXT GLEW_GET_FUN(__glewBlendEquationiEXT)
#define glBlendFuncSeparateiEXT GLEW_GET_FUN(__glewBlendFuncSeparateiEXT)
#define glBlendFunciEXT GLEW_GET_FUN(__glewBlendFunciEXT)
#define glColorMaskiEXT GLEW_GET_FUN(__glewColorMaskiEXT)
#define glDisableiEXT GLEW_GET_FUN(__glewDisableiEXT)
#define glEnableiEXT GLEW_GET_FUN(__glewEnableiEXT)
#define glIsEnablediEXT GLEW_GET_FUN(__glewIsEnablediEXT)

#define GLEW_EXT_draw_buffers_indexed GLEW_GET_VAR(__GLEW_EXT_draw_buffers_indexed)

#endif /* GL_EXT_draw_buffers_indexed */

/* -------------------- GL_EXT_draw_elements_base_vertex ------------------- */

#ifndef GL_EXT_draw_elements_base_vertex
#define GL_EXT_draw_elements_base_vertex 1

typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSBASEVERTEXEXTPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXEXTPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTSBASEVERTEXEXTPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSBASEVERTEXEXTPROC) (GLenum mode, const GLsizei* count, GLenum type, const void *const *indices, GLsizei primcount, const GLint *basevertex);

#define glDrawElementsBaseVertexEXT GLEW_GET_FUN(__glewDrawElementsBaseVertexEXT)
#define glDrawElementsInstancedBaseVertexEXT GLEW_GET_FUN(__glewDrawElementsInstancedBaseVertexEXT)
#define glDrawRangeElementsBaseVertexEXT GLEW_GET_FUN(__glewDrawRangeElementsBaseVertexEXT)
#define glMultiDrawElementsBaseVertexEXT GLEW_GET_FUN(__glewMultiDrawElementsBaseVertexEXT)

#define GLEW_EXT_draw_elements_base_vertex GLEW_GET_VAR(__GLEW_EXT_draw_elements_base_vertex)

#endif /* GL_EXT_draw_elements_base_vertex */

/* ------------------------- GL_EXT_draw_instanced ------------------------- */

#ifndef GL_EXT_draw_instanced
#define GL_EXT_draw_instanced 1

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDEXTPROC) (GLenum mode, GLint start, GLsizei count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDEXTPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei primcount);

#define glDrawArraysInstancedEXT GLEW_GET_FUN(__glewDrawArraysInstancedEXT)
#define glDrawElementsInstancedEXT GLEW_GET_FUN(__glewDrawElementsInstancedEXT)

#define GLEW_EXT_draw_instanced GLEW_GET_VAR(__GLEW_EXT_draw_instanced)

#endif /* GL_EXT_draw_instanced */

/* ----------------------- GL_EXT_draw_range_elements ---------------------- */

#ifndef GL_EXT_draw_range_elements
#define GL_EXT_draw_range_elements 1

#define GL_MAX_ELEMENTS_VERTICES_EXT 0x80E8
#define GL_MAX_ELEMENTS_INDICES_EXT 0x80E9

typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTSEXTPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);

#define glDrawRangeElementsEXT GLEW_GET_FUN(__glewDrawRangeElementsEXT)

#define GLEW_EXT_draw_range_elements GLEW_GET_VAR(__GLEW_EXT_draw_range_elements)

#endif /* GL_EXT_draw_range_elements */

/* ------------------------- GL_EXT_external_buffer ------------------------ */

#ifndef GL_EXT_external_buffer
#define GL_EXT_external_buffer 1

typedef void* GLeglClientBufferEXT;

typedef void (GLAPIENTRY * PFNGLBUFFERSTORAGEEXTERNALEXTPROC) (GLenum target, GLintptr offset, GLsizeiptr size, GLeglClientBufferEXT clientBuffer, GLbitfield flags);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERSTORAGEEXTERNALEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, GLeglClientBufferEXT clientBuffer, GLbitfield flags);

#define glBufferStorageExternalEXT GLEW_GET_FUN(__glewBufferStorageExternalEXT)
#define glNamedBufferStorageExternalEXT GLEW_GET_FUN(__glewNamedBufferStorageExternalEXT)

#define GLEW_EXT_external_buffer GLEW_GET_VAR(__GLEW_EXT_external_buffer)

#endif /* GL_EXT_external_buffer */

/* --------------------------- GL_EXT_float_blend -------------------------- */

#ifndef GL_EXT_float_blend
#define GL_EXT_float_blend 1

#define GLEW_EXT_float_blend GLEW_GET_VAR(__GLEW_EXT_float_blend)

#endif /* GL_EXT_float_blend */

/* ---------------------------- GL_EXT_fog_coord --------------------------- */

#ifndef GL_EXT_fog_coord
#define GL_EXT_fog_coord 1

#define GL_FOG_COORDINATE_SOURCE_EXT 0x8450
#define GL_FOG_COORDINATE_EXT 0x8451
#define GL_FRAGMENT_DEPTH_EXT 0x8452
#define GL_CURRENT_FOG_COORDINATE_EXT 0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE_EXT 0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE_EXT 0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER_EXT 0x8456
#define GL_FOG_COORDINATE_ARRAY_EXT 0x8457

typedef void (GLAPIENTRY * PFNGLFOGCOORDPOINTEREXTPROC) (GLenum type, GLsizei stride, const void *pointer);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDEXTPROC) (GLdouble coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDVEXTPROC) (const GLdouble *coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFEXTPROC) (GLfloat coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFVEXTPROC) (const GLfloat *coord);

#define glFogCoordPointerEXT GLEW_GET_FUN(__glewFogCoordPointerEXT)
#define glFogCoorddEXT GLEW_GET_FUN(__glewFogCoorddEXT)
#define glFogCoorddvEXT GLEW_GET_FUN(__glewFogCoorddvEXT)
#define glFogCoordfEXT GLEW_GET_FUN(__glewFogCoordfEXT)
#define glFogCoordfvEXT GLEW_GET_FUN(__glewFogCoordfvEXT)

#define GLEW_EXT_fog_coord GLEW_GET_VAR(__GLEW_EXT_fog_coord)

#endif /* GL_EXT_fog_coord */

/* --------------------------- GL_EXT_frag_depth --------------------------- */

#ifndef GL_EXT_frag_depth
#define GL_EXT_frag_depth 1

#define GLEW_EXT_frag_depth GLEW_GET_VAR(__GLEW_EXT_frag_depth)

#endif /* GL_EXT_frag_depth */

/* ------------------------ GL_EXT_fragment_lighting ----------------------- */

#ifndef GL_EXT_fragment_lighting
#define GL_EXT_fragment_lighting 1

#define GL_FRAGMENT_LIGHTING_EXT 0x8400
#define GL_FRAGMENT_COLOR_MATERIAL_EXT 0x8401
#define GL_FRAGMENT_COLOR_MATERIAL_FACE_EXT 0x8402
#define GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_EXT 0x8403
#define GL_MAX_FRAGMENT_LIGHTS_EXT 0x8404
#define GL_MAX_ACTIVE_LIGHTS_EXT 0x8405
#define GL_CURRENT_RASTER_NORMAL_EXT 0x8406
#define GL_LIGHT_ENV_MODE_EXT 0x8407
#define GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_EXT 0x8408
#define GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_EXT 0x8409
#define GL_FRAGMENT_LIGHT_MODEL_AMBIENT_EXT 0x840A
#define GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_EXT 0x840B
#define GL_FRAGMENT_LIGHT0_EXT 0x840C
#define GL_FRAGMENT_LIGHT7_EXT 0x8413

typedef void (GLAPIENTRY * PFNGLFRAGMENTCOLORMATERIALEXTPROC) (GLenum face, GLenum mode);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELFEXTPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELFVEXTPROC) (GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELIEXTPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELIVEXTPROC) (GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTFEXTPROC) (GLenum light, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTFVEXTPROC) (GLenum light, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTIEXTPROC) (GLenum light, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTIVEXTPROC) (GLenum light, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALFEXTPROC) (GLenum face, GLenum pname, const GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALFVEXTPROC) (GLenum face, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALIEXTPROC) (GLenum face, GLenum pname, const GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALIVEXTPROC) (GLenum face, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTLIGHTFVEXTPROC) (GLenum light, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTLIGHTIVEXTPROC) (GLenum light, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTMATERIALFVEXTPROC) (GLenum face, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTMATERIALIVEXTPROC) (GLenum face, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLLIGHTENVIEXTPROC) (GLenum pname, GLint param);

#define glFragmentColorMaterialEXT GLEW_GET_FUN(__glewFragmentColorMaterialEXT)
#define glFragmentLightModelfEXT GLEW_GET_FUN(__glewFragmentLightModelfEXT)
#define glFragmentLightModelfvEXT GLEW_GET_FUN(__glewFragmentLightModelfvEXT)
#define glFragmentLightModeliEXT GLEW_GET_FUN(__glewFragmentLightModeliEXT)
#define glFragmentLightModelivEXT GLEW_GET_FUN(__glewFragmentLightModelivEXT)
#define glFragmentLightfEXT GLEW_GET_FUN(__glewFragmentLightfEXT)
#define glFragmentLightfvEXT GLEW_GET_FUN(__glewFragmentLightfvEXT)
#define glFragmentLightiEXT GLEW_GET_FUN(__glewFragmentLightiEXT)
#define glFragmentLightivEXT GLEW_GET_FUN(__glewFragmentLightivEXT)
#define glFragmentMaterialfEXT GLEW_GET_FUN(__glewFragmentMaterialfEXT)
#define glFragmentMaterialfvEXT GLEW_GET_FUN(__glewFragmentMaterialfvEXT)
#define glFragmentMaterialiEXT GLEW_GET_FUN(__glewFragmentMaterialiEXT)
#define glFragmentMaterialivEXT GLEW_GET_FUN(__glewFragmentMaterialivEXT)
#define glGetFragmentLightfvEXT GLEW_GET_FUN(__glewGetFragmentLightfvEXT)
#define glGetFragmentLightivEXT GLEW_GET_FUN(__glewGetFragmentLightivEXT)
#define glGetFragmentMaterialfvEXT GLEW_GET_FUN(__glewGetFragmentMaterialfvEXT)
#define glGetFragmentMaterialivEXT GLEW_GET_FUN(__glewGetFragmentMaterialivEXT)
#define glLightEnviEXT GLEW_GET_FUN(__glewLightEnviEXT)

#define GLEW_EXT_fragment_lighting GLEW_GET_VAR(__GLEW_EXT_fragment_lighting)

#endif /* GL_EXT_fragment_lighting */

/* ------------------------ GL_EXT_framebuffer_blit ------------------------ */

#ifndef GL_EXT_framebuffer_blit
#define GL_EXT_framebuffer_blit 1

#define GL_DRAW_FRAMEBUFFER_BINDING_EXT 0x8CA6
#define GL_READ_FRAMEBUFFER_EXT 0x8CA8
#define GL_DRAW_FRAMEBUFFER_EXT 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING_EXT 0x8CAA

typedef void (GLAPIENTRY * PFNGLBLITFRAMEBUFFEREXTPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

#define glBlitFramebufferEXT GLEW_GET_FUN(__glewBlitFramebufferEXT)

#define GLEW_EXT_framebuffer_blit GLEW_GET_VAR(__GLEW_EXT_framebuffer_blit)

#endif /* GL_EXT_framebuffer_blit */

/* --------------------- GL_EXT_framebuffer_multisample -------------------- */

#ifndef GL_EXT_framebuffer_multisample
#define GL_EXT_framebuffer_multisample 1

#define GL_RENDERBUFFER_SAMPLES_EXT 0x8CAB
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT 0x8D56
#define GL_MAX_SAMPLES_EXT 0x8D57

typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

#define glRenderbufferStorageMultisampleEXT GLEW_GET_FUN(__glewRenderbufferStorageMultisampleEXT)

#define GLEW_EXT_framebuffer_multisample GLEW_GET_VAR(__GLEW_EXT_framebuffer_multisample)

#endif /* GL_EXT_framebuffer_multisample */

/* --------------- GL_EXT_framebuffer_multisample_blit_scaled -------------- */

#ifndef GL_EXT_framebuffer_multisample_blit_scaled
#define GL_EXT_framebuffer_multisample_blit_scaled 1

#define GL_SCALED_RESOLVE_FASTEST_EXT 0x90BA
#define GL_SCALED_RESOLVE_NICEST_EXT 0x90BB

#define GLEW_EXT_framebuffer_multisample_blit_scaled GLEW_GET_VAR(__GLEW_EXT_framebuffer_multisample_blit_scaled)

#endif /* GL_EXT_framebuffer_multisample_blit_scaled */

/* ----------------------- GL_EXT_framebuffer_object ----------------------- */

#ifndef GL_EXT_framebuffer_object
#define GL_EXT_framebuffer_object 1

#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT 0x0506
#define GL_MAX_RENDERBUFFER_SIZE_EXT 0x84E8
#define GL_FRAMEBUFFER_BINDING_EXT 0x8CA6
#define GL_RENDERBUFFER_BINDING_EXT 0x8CA7
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE_EXT 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT 0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT 0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT 0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS_EXT 0x8CDF
#define GL_COLOR_ATTACHMENT0_EXT 0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT 0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT 0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT 0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT 0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT 0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT 0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT 0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT 0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT 0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT 0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT 0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT 0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT 0x8CED
#define GL_COLOR_ATTACHMENT14_EXT 0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT 0x8CEF
#define GL_DEPTH_ATTACHMENT_EXT 0x8D00
#define GL_STENCIL_ATTACHMENT_EXT 0x8D20
#define GL_FRAMEBUFFER_EXT 0x8D40
#define GL_RENDERBUFFER_EXT 0x8D41
#define GL_RENDERBUFFER_WIDTH_EXT 0x8D42
#define GL_RENDERBUFFER_HEIGHT_EXT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT_EXT 0x8D44
#define GL_STENCIL_INDEX1_EXT 0x8D46
#define GL_STENCIL_INDEX4_EXT 0x8D47
#define GL_STENCIL_INDEX8_EXT 0x8D48
#define GL_STENCIL_INDEX16_EXT 0x8D49
#define GL_RENDERBUFFER_RED_SIZE_EXT 0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE_EXT 0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE_EXT 0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE_EXT 0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE_EXT 0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE_EXT 0x8D55

typedef void (GLAPIENTRY * PFNGLBINDFRAMEBUFFEREXTPROC) (GLenum target, GLuint framebuffer);
typedef void (GLAPIENTRY * PFNGLBINDRENDERBUFFEREXTPROC) (GLenum target, GLuint renderbuffer);
typedef GLenum (GLAPIENTRY * PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLDELETEFRAMEBUFFERSEXTPROC) (GLsizei n, const GLuint* framebuffers);
typedef void (GLAPIENTRY * PFNGLDELETERENDERBUFFERSEXTPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE1DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE3DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLAPIENTRY * PFNGLGENFRAMEBUFFERSEXTPROC) (GLsizei n, GLuint* framebuffers);
typedef void (GLAPIENTRY * PFNGLGENRENDERBUFFERSEXTPROC) (GLsizei n, GLuint* renderbuffers);
typedef void (GLAPIENTRY * PFNGLGENERATEMIPMAPEXTPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISFRAMEBUFFEREXTPROC) (GLuint framebuffer);
typedef GLboolean (GLAPIENTRY * PFNGLISRENDERBUFFEREXTPROC) (GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

#define glBindFramebufferEXT GLEW_GET_FUN(__glewBindFramebufferEXT)
#define glBindRenderbufferEXT GLEW_GET_FUN(__glewBindRenderbufferEXT)
#define glCheckFramebufferStatusEXT GLEW_GET_FUN(__glewCheckFramebufferStatusEXT)
#define glDeleteFramebuffersEXT GLEW_GET_FUN(__glewDeleteFramebuffersEXT)
#define glDeleteRenderbuffersEXT GLEW_GET_FUN(__glewDeleteRenderbuffersEXT)
#define glFramebufferRenderbufferEXT GLEW_GET_FUN(__glewFramebufferRenderbufferEXT)
#define glFramebufferTexture1DEXT GLEW_GET_FUN(__glewFramebufferTexture1DEXT)
#define glFramebufferTexture2DEXT GLEW_GET_FUN(__glewFramebufferTexture2DEXT)
#define glFramebufferTexture3DEXT GLEW_GET_FUN(__glewFramebufferTexture3DEXT)
#define glGenFramebuffersEXT GLEW_GET_FUN(__glewGenFramebuffersEXT)
#define glGenRenderbuffersEXT GLEW_GET_FUN(__glewGenRenderbuffersEXT)
#define glGenerateMipmapEXT GLEW_GET_FUN(__glewGenerateMipmapEXT)
#define glGetFramebufferAttachmentParameterivEXT GLEW_GET_FUN(__glewGetFramebufferAttachmentParameterivEXT)
#define glGetRenderbufferParameterivEXT GLEW_GET_FUN(__glewGetRenderbufferParameterivEXT)
#define glIsFramebufferEXT GLEW_GET_FUN(__glewIsFramebufferEXT)
#define glIsRenderbufferEXT GLEW_GET_FUN(__glewIsRenderbufferEXT)
#define glRenderbufferStorageEXT GLEW_GET_FUN(__glewRenderbufferStorageEXT)

#define GLEW_EXT_framebuffer_object GLEW_GET_VAR(__GLEW_EXT_framebuffer_object)

#endif /* GL_EXT_framebuffer_object */

/* ------------------------ GL_EXT_framebuffer_sRGB ------------------------ */

#ifndef GL_EXT_framebuffer_sRGB
#define GL_EXT_framebuffer_sRGB 1

#define GL_FRAMEBUFFER_SRGB_EXT 0x8DB9
#define GL_FRAMEBUFFER_SRGB_CAPABLE_EXT 0x8DBA

#define GLEW_EXT_framebuffer_sRGB GLEW_GET_VAR(__GLEW_EXT_framebuffer_sRGB)

#endif /* GL_EXT_framebuffer_sRGB */

/* ----------------------- GL_EXT_geometry_point_size ---------------------- */

#ifndef GL_EXT_geometry_point_size
#define GL_EXT_geometry_point_size 1

#define GL_GEOMETRY_SHADER_BIT_EXT 0x00000004
#define GL_LINES_ADJACENCY_EXT 0xA
#define GL_LINE_STRIP_ADJACENCY_EXT 0xB
#define GL_TRIANGLES_ADJACENCY_EXT 0xC
#define GL_TRIANGLE_STRIP_ADJACENCY_EXT 0xD
#define GL_LAYER_PROVOKING_VERTEX_EXT 0x825E
#define GL_UNDEFINED_VERTEX_EXT 0x8260
#define GL_GEOMETRY_SHADER_INVOCATIONS_EXT 0x887F
#define GL_GEOMETRY_LINKED_VERTICES_OUT_EXT 0x8916
#define GL_GEOMETRY_LINKED_INPUT_TYPE_EXT 0x8917
#define GL_GEOMETRY_LINKED_OUTPUT_TYPE_EXT 0x8918
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS_EXT 0x8A2C
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS_EXT 0x8A32
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT 0x8C29
#define GL_PRIMITIVES_GENERATED_EXT 0x8C87
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT 0x8DA8
#define GL_GEOMETRY_SHADER_EXT 0x8DD9
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_EXT 0x8DE1
#define GL_FIRST_VERTEX_CONVENTION_EXT 0x8E4D
#define GL_LAST_VERTEX_CONVENTION_EXT 0x8E4E
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS_EXT 0x8E5A
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS_EXT 0x90CD
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS_EXT 0x90D7
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS_EXT 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS_EXT 0x9124
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS_EXT 0x92CF
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS_EXT 0x92D5
#define GL_REFERENCED_BY_GEOMETRY_SHADER_EXT 0x9309
#define GL_FRAMEBUFFER_DEFAULT_LAYERS_EXT 0x9312
#define GL_MAX_FRAMEBUFFER_LAYERS_EXT 0x9317

#define GLEW_EXT_geometry_point_size GLEW_GET_VAR(__GLEW_EXT_geometry_point_size)

#endif /* GL_EXT_geometry_point_size */

/* ------------------------- GL_EXT_geometry_shader ------------------------ */

#ifndef GL_EXT_geometry_shader
#define GL_EXT_geometry_shader 1

#define GL_GEOMETRY_SHADER_BIT_EXT 0x00000004
#define GL_LINES_ADJACENCY_EXT 0xA
#define GL_LINE_STRIP_ADJACENCY_EXT 0xB
#define GL_TRIANGLES_ADJACENCY_EXT 0xC
#define GL_TRIANGLE_STRIP_ADJACENCY_EXT 0xD
#define GL_LAYER_PROVOKING_VERTEX_EXT 0x825E
#define GL_UNDEFINED_VERTEX_EXT 0x8260
#define GL_GEOMETRY_SHADER_INVOCATIONS_EXT 0x887F
#define GL_GEOMETRY_LINKED_VERTICES_OUT_EXT 0x8916
#define GL_GEOMETRY_LINKED_INPUT_TYPE_EXT 0x8917
#define GL_GEOMETRY_LINKED_OUTPUT_TYPE_EXT 0x8918
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS_EXT 0x8A2C
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS_EXT 0x8A32
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT 0x8C29
#define GL_PRIMITIVES_GENERATED_EXT 0x8C87
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT 0x8DA8
#define GL_GEOMETRY_SHADER_EXT 0x8DD9
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_EXT 0x8DE1
#define GL_FIRST_VERTEX_CONVENTION_EXT 0x8E4D
#define GL_LAST_VERTEX_CONVENTION_EXT 0x8E4E
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS_EXT 0x8E5A
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS_EXT 0x90CD
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS_EXT 0x90D7
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS_EXT 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS_EXT 0x9124
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS_EXT 0x92CF
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS_EXT 0x92D5
#define GL_REFERENCED_BY_GEOMETRY_SHADER_EXT 0x9309
#define GL_FRAMEBUFFER_DEFAULT_LAYERS_EXT 0x9312
#define GL_MAX_FRAMEBUFFER_LAYERS_EXT 0x9317

#define GLEW_EXT_geometry_shader GLEW_GET_VAR(__GLEW_EXT_geometry_shader)

#endif /* GL_EXT_geometry_shader */

/* ------------------------ GL_EXT_geometry_shader4 ------------------------ */

#ifndef GL_EXT_geometry_shader4
#define GL_EXT_geometry_shader4 1

#define GL_LINES_ADJACENCY_EXT 0xA
#define GL_LINE_STRIP_ADJACENCY_EXT 0xB
#define GL_TRIANGLES_ADJACENCY_EXT 0xC
#define GL_TRIANGLE_STRIP_ADJACENCY_EXT 0xD
#define GL_PROGRAM_POINT_SIZE_EXT 0x8642
#define GL_MAX_VARYING_COMPONENTS_EXT 0x8B4B
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_EXT 0x8CD4
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT 0x8DA8
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_EXT 0x8DA9
#define GL_GEOMETRY_SHADER_EXT 0x8DD9
#define GL_GEOMETRY_VERTICES_OUT_EXT 0x8DDA
#define GL_GEOMETRY_INPUT_TYPE_EXT 0x8DDB
#define GL_GEOMETRY_OUTPUT_TYPE_EXT 0x8DDC
#define GL_MAX_GEOMETRY_VARYING_COMPONENTS_EXT 0x8DDD
#define GL_MAX_VERTEX_VARYING_COMPONENTS_EXT 0x8DDE
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_EXT 0x8DE1

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREEXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERIEXTPROC) (GLuint program, GLenum pname, GLint value);

#define glFramebufferTextureEXT GLEW_GET_FUN(__glewFramebufferTextureEXT)
#define glFramebufferTextureFaceEXT GLEW_GET_FUN(__glewFramebufferTextureFaceEXT)
#define glProgramParameteriEXT GLEW_GET_FUN(__glewProgramParameteriEXT)

#define GLEW_EXT_geometry_shader4 GLEW_GET_VAR(__GLEW_EXT_geometry_shader4)

#endif /* GL_EXT_geometry_shader4 */

/* --------------------- GL_EXT_gpu_program_parameters --------------------- */

#ifndef GL_EXT_gpu_program_parameters
#define GL_EXT_gpu_program_parameters 1

typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERS4FVEXTPROC) (GLenum target, GLuint index, GLsizei count, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERS4FVEXTPROC) (GLenum target, GLuint index, GLsizei count, const GLfloat* params);

#define glProgramEnvParameters4fvEXT GLEW_GET_FUN(__glewProgramEnvParameters4fvEXT)
#define glProgramLocalParameters4fvEXT GLEW_GET_FUN(__glewProgramLocalParameters4fvEXT)

#define GLEW_EXT_gpu_program_parameters GLEW_GET_VAR(__GLEW_EXT_gpu_program_parameters)

#endif /* GL_EXT_gpu_program_parameters */

/* --------------------------- GL_EXT_gpu_shader4 -------------------------- */

#ifndef GL_EXT_gpu_shader4
#define GL_EXT_gpu_shader4 1

#define GL_VERTEX_ATTRIB_ARRAY_INTEGER_EXT 0x88FD
#define GL_SAMPLER_1D_ARRAY_EXT 0x8DC0
#define GL_SAMPLER_2D_ARRAY_EXT 0x8DC1
#define GL_SAMPLER_BUFFER_EXT 0x8DC2
#define GL_SAMPLER_1D_ARRAY_SHADOW_EXT 0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW_EXT 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW_EXT 0x8DC5
#define GL_UNSIGNED_INT_VEC2_EXT 0x8DC6
#define GL_UNSIGNED_INT_VEC3_EXT 0x8DC7
#define GL_UNSIGNED_INT_VEC4_EXT 0x8DC8
#define GL_INT_SAMPLER_1D_EXT 0x8DC9
#define GL_INT_SAMPLER_2D_EXT 0x8DCA
#define GL_INT_SAMPLER_3D_EXT 0x8DCB
#define GL_INT_SAMPLER_CUBE_EXT 0x8DCC
#define GL_INT_SAMPLER_2D_RECT_EXT 0x8DCD
#define GL_INT_SAMPLER_1D_ARRAY_EXT 0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY_EXT 0x8DCF
#define GL_INT_SAMPLER_BUFFER_EXT 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_1D_EXT 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D_EXT 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D_EXT 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE_EXT 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT_EXT 0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY_EXT 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY_EXT 0x8DD7
#define GL_UNSIGNED_INT_SAMPLER_BUFFER_EXT 0x8DD8

typedef void (GLAPIENTRY * PFNGLBINDFRAGDATALOCATIONEXTPROC) (GLuint program, GLuint color, const GLchar *name);
typedef GLint (GLAPIENTRY * PFNGLGETFRAGDATALOCATIONEXTPROC) (GLuint program, const GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMUIVEXTPROC) (GLuint program, GLint location, GLuint *params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIIVEXTPROC) (GLuint index, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIUIVEXTPROC) (GLuint index, GLenum pname, GLuint *params);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIEXTPROC) (GLint location, GLuint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIEXTPROC) (GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIEXTPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIEXTPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IEXTPROC) (GLuint index, GLint x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IVEXTPROC) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIEXTPROC) (GLuint index, GLuint x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IEXTPROC) (GLuint index, GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IVEXTPROC) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIEXTPROC) (GLuint index, GLuint x, GLuint y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IEXTPROC) (GLuint index, GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IVEXTPROC) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIEXTPROC) (GLuint index, GLuint x, GLuint y, GLuint z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4BVEXTPROC) (GLuint index, const GLbyte *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IEXTPROC) (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IVEXTPROC) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4SVEXTPROC) (GLuint index, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UBVEXTPROC) (GLuint index, const GLubyte *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIEXTPROC) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4USVEXTPROC) (GLuint index, const GLushort *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBIPOINTEREXTPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);

#define glBindFragDataLocationEXT GLEW_GET_FUN(__glewBindFragDataLocationEXT)
#define glGetFragDataLocationEXT GLEW_GET_FUN(__glewGetFragDataLocationEXT)
#define glGetUniformuivEXT GLEW_GET_FUN(__glewGetUniformuivEXT)
#define glGetVertexAttribIivEXT GLEW_GET_FUN(__glewGetVertexAttribIivEXT)
#define glGetVertexAttribIuivEXT GLEW_GET_FUN(__glewGetVertexAttribIuivEXT)
#define glUniform1uiEXT GLEW_GET_FUN(__glewUniform1uiEXT)
#define glUniform1uivEXT GLEW_GET_FUN(__glewUniform1uivEXT)
#define glUniform2uiEXT GLEW_GET_FUN(__glewUniform2uiEXT)
#define glUniform2uivEXT GLEW_GET_FUN(__glewUniform2uivEXT)
#define glUniform3uiEXT GLEW_GET_FUN(__glewUniform3uiEXT)
#define glUniform3uivEXT GLEW_GET_FUN(__glewUniform3uivEXT)
#define glUniform4uiEXT GLEW_GET_FUN(__glewUniform4uiEXT)
#define glUniform4uivEXT GLEW_GET_FUN(__glewUniform4uivEXT)
#define glVertexAttribI1iEXT GLEW_GET_FUN(__glewVertexAttribI1iEXT)
#define glVertexAttribI1ivEXT GLEW_GET_FUN(__glewVertexAttribI1ivEXT)
#define glVertexAttribI1uiEXT GLEW_GET_FUN(__glewVertexAttribI1uiEXT)
#define glVertexAttribI1uivEXT GLEW_GET_FUN(__glewVertexAttribI1uivEXT)
#define glVertexAttribI2iEXT GLEW_GET_FUN(__glewVertexAttribI2iEXT)
#define glVertexAttribI2ivEXT GLEW_GET_FUN(__glewVertexAttribI2ivEXT)
#define glVertexAttribI2uiEXT GLEW_GET_FUN(__glewVertexAttribI2uiEXT)
#define glVertexAttribI2uivEXT GLEW_GET_FUN(__glewVertexAttribI2uivEXT)
#define glVertexAttribI3iEXT GLEW_GET_FUN(__glewVertexAttribI3iEXT)
#define glVertexAttribI3ivEXT GLEW_GET_FUN(__glewVertexAttribI3ivEXT)
#define glVertexAttribI3uiEXT GLEW_GET_FUN(__glewVertexAttribI3uiEXT)
#define glVertexAttribI3uivEXT GLEW_GET_FUN(__glewVertexAttribI3uivEXT)
#define glVertexAttribI4bvEXT GLEW_GET_FUN(__glewVertexAttribI4bvEXT)
#define glVertexAttribI4iEXT GLEW_GET_FUN(__glewVertexAttribI4iEXT)
#define glVertexAttribI4ivEXT GLEW_GET_FUN(__glewVertexAttribI4ivEXT)
#define glVertexAttribI4svEXT GLEW_GET_FUN(__glewVertexAttribI4svEXT)
#define glVertexAttribI4ubvEXT GLEW_GET_FUN(__glewVertexAttribI4ubvEXT)
#define glVertexAttribI4uiEXT GLEW_GET_FUN(__glewVertexAttribI4uiEXT)
#define glVertexAttribI4uivEXT GLEW_GET_FUN(__glewVertexAttribI4uivEXT)
#define glVertexAttribI4usvEXT GLEW_GET_FUN(__glewVertexAttribI4usvEXT)
#define glVertexAttribIPointerEXT GLEW_GET_FUN(__glewVertexAttribIPointerEXT)

#define GLEW_EXT_gpu_shader4 GLEW_GET_VAR(__GLEW_EXT_gpu_shader4)

#endif /* GL_EXT_gpu_shader4 */

/* --------------------------- GL_EXT_gpu_shader5 -------------------------- */

#ifndef GL_EXT_gpu_shader5
#define GL_EXT_gpu_shader5 1

#define GLEW_EXT_gpu_shader5 GLEW_GET_VAR(__GLEW_EXT_gpu_shader5)

#endif /* GL_EXT_gpu_shader5 */

/* ---------------------------- GL_EXT_histogram --------------------------- */

#ifndef GL_EXT_histogram
#define GL_EXT_histogram 1

#define GL_HISTOGRAM_EXT 0x8024
#define GL_PROXY_HISTOGRAM_EXT 0x8025
#define GL_HISTOGRAM_WIDTH_EXT 0x8026
#define GL_HISTOGRAM_FORMAT_EXT 0x8027
#define GL_HISTOGRAM_RED_SIZE_EXT 0x8028
#define GL_HISTOGRAM_GREEN_SIZE_EXT 0x8029
#define GL_HISTOGRAM_BLUE_SIZE_EXT 0x802A
#define GL_HISTOGRAM_ALPHA_SIZE_EXT 0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE_EXT 0x802C
#define GL_HISTOGRAM_SINK_EXT 0x802D
#define GL_MINMAX_EXT 0x802E
#define GL_MINMAX_FORMAT_EXT 0x802F
#define GL_MINMAX_SINK_EXT 0x8030

typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMEXTPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, void *values);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMINMAXEXTPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, void *values);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLHISTOGRAMEXTPROC) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLMINMAXEXTPROC) (GLenum target, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLRESETHISTOGRAMEXTPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLRESETMINMAXEXTPROC) (GLenum target);

#define glGetHistogramEXT GLEW_GET_FUN(__glewGetHistogramEXT)
#define glGetHistogramParameterfvEXT GLEW_GET_FUN(__glewGetHistogramParameterfvEXT)
#define glGetHistogramParameterivEXT GLEW_GET_FUN(__glewGetHistogramParameterivEXT)
#define glGetMinmaxEXT GLEW_GET_FUN(__glewGetMinmaxEXT)
#define glGetMinmaxParameterfvEXT GLEW_GET_FUN(__glewGetMinmaxParameterfvEXT)
#define glGetMinmaxParameterivEXT GLEW_GET_FUN(__glewGetMinmaxParameterivEXT)
#define glHistogramEXT GLEW_GET_FUN(__glewHistogramEXT)
#define glMinmaxEXT GLEW_GET_FUN(__glewMinmaxEXT)
#define glResetHistogramEXT GLEW_GET_FUN(__glewResetHistogramEXT)
#define glResetMinmaxEXT GLEW_GET_FUN(__glewResetMinmaxEXT)

#define GLEW_EXT_histogram GLEW_GET_VAR(__GLEW_EXT_histogram)

#endif /* GL_EXT_histogram */

/* ----------------------- GL_EXT_index_array_formats ---------------------- */

#ifndef GL_EXT_index_array_formats
#define GL_EXT_index_array_formats 1

#define GLEW_EXT_index_array_formats GLEW_GET_VAR(__GLEW_EXT_index_array_formats)

#endif /* GL_EXT_index_array_formats */

/* --------------------------- GL_EXT_index_func --------------------------- */

#ifndef GL_EXT_index_func
#define GL_EXT_index_func 1

typedef void (GLAPIENTRY * PFNGLINDEXFUNCEXTPROC) (GLenum func, GLfloat ref);

#define glIndexFuncEXT GLEW_GET_FUN(__glewIndexFuncEXT)

#define GLEW_EXT_index_func GLEW_GET_VAR(__GLEW_EXT_index_func)

#endif /* GL_EXT_index_func */

/* ------------------------- GL_EXT_index_material ------------------------- */

#ifndef GL_EXT_index_material
#define GL_EXT_index_material 1

typedef void (GLAPIENTRY * PFNGLINDEXMATERIALEXTPROC) (GLenum face, GLenum mode);

#define glIndexMaterialEXT GLEW_GET_FUN(__glewIndexMaterialEXT)

#define GLEW_EXT_index_material GLEW_GET_VAR(__GLEW_EXT_index_material)

#endif /* GL_EXT_index_material */

/* -------------------------- GL_EXT_index_texture ------------------------- */

#ifndef GL_EXT_index_texture
#define GL_EXT_index_texture 1

#define GLEW_EXT_index_texture GLEW_GET_VAR(__GLEW_EXT_index_texture)

#endif /* GL_EXT_index_texture */

/* ------------------------ GL_EXT_instanced_arrays ------------------------ */

#ifndef GL_EXT_instanced_arrays
#define GL_EXT_instanced_arrays 1

#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR_EXT 0x88FE

typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBDIVISOREXTPROC) (GLuint index, GLuint divisor);

#define glVertexAttribDivisorEXT GLEW_GET_FUN(__glewVertexAttribDivisorEXT)

#define GLEW_EXT_instanced_arrays GLEW_GET_VAR(__GLEW_EXT_instanced_arrays)

#endif /* GL_EXT_instanced_arrays */

/* -------------------------- GL_EXT_light_texture ------------------------- */

#ifndef GL_EXT_light_texture
#define GL_EXT_light_texture 1

#define GL_FRAGMENT_MATERIAL_EXT 0x8349
#define GL_FRAGMENT_NORMAL_EXT 0x834A
#define GL_FRAGMENT_COLOR_EXT 0x834C
#define GL_ATTENUATION_EXT 0x834D
#define GL_SHADOW_ATTENUATION_EXT 0x834E
#define GL_TEXTURE_APPLICATION_MODE_EXT 0x834F
#define GL_TEXTURE_LIGHT_EXT 0x8350
#define GL_TEXTURE_MATERIAL_FACE_EXT 0x8351
#define GL_TEXTURE_MATERIAL_PARAMETER_EXT 0x8352

typedef void (GLAPIENTRY * PFNGLAPPLYTEXTUREEXTPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLTEXTURELIGHTEXTPROC) (GLenum pname);
typedef void (GLAPIENTRY * PFNGLTEXTUREMATERIALEXTPROC) (GLenum face, GLenum mode);

#define glApplyTextureEXT GLEW_GET_FUN(__glewApplyTextureEXT)
#define glTextureLightEXT GLEW_GET_FUN(__glewTextureLightEXT)
#define glTextureMaterialEXT GLEW_GET_FUN(__glewTextureMaterialEXT)

#define GLEW_EXT_light_texture GLEW_GET_VAR(__GLEW_EXT_light_texture)

#endif /* GL_EXT_light_texture */

/* ------------------------ GL_EXT_map_buffer_range ------------------------ */

#ifndef GL_EXT_map_buffer_range
#define GL_EXT_map_buffer_range 1

#define GL_MAP_READ_BIT_EXT 0x0001
#define GL_MAP_WRITE_BIT_EXT 0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT_EXT 0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT_EXT 0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT_EXT 0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT_EXT 0x0020

typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDBUFFERRANGEEXTPROC) (GLenum target, GLintptr offset, GLsizeiptr length);
typedef void * (GLAPIENTRY * PFNGLMAPBUFFERRANGEEXTPROC) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);

#define glFlushMappedBufferRangeEXT GLEW_GET_FUN(__glewFlushMappedBufferRangeEXT)
#define glMapBufferRangeEXT GLEW_GET_FUN(__glewMapBufferRangeEXT)

#define GLEW_EXT_map_buffer_range GLEW_GET_VAR(__GLEW_EXT_map_buffer_range)

#endif /* GL_EXT_map_buffer_range */

/* -------------------------- GL_EXT_memory_object ------------------------- */

#ifndef GL_EXT_memory_object
#define GL_EXT_memory_object 1

#define GL_UUID_SIZE_EXT 16
#define GL_TEXTURE_TILING_EXT 0x9580
#define GL_DEDICATED_MEMORY_OBJECT_EXT 0x9581
#define GL_NUM_TILING_TYPES_EXT 0x9582
#define GL_TILING_TYPES_EXT 0x9583
#define GL_OPTIMAL_TILING_EXT 0x9584
#define GL_LINEAR_TILING_EXT 0x9585
#define GL_LAYOUT_GENERAL_EXT 0x958D
#define GL_LAYOUT_COLOR_ATTACHMENT_EXT 0x958E
#define GL_LAYOUT_DEPTH_STENCIL_ATTACHMENT_EXT 0x958F
#define GL_LAYOUT_DEPTH_STENCIL_READ_ONLY_EXT 0x9590
#define GL_LAYOUT_SHADER_READ_ONLY_EXT 0x9591
#define GL_LAYOUT_TRANSFER_SRC_EXT 0x9592
#define GL_LAYOUT_TRANSFER_DST_EXT 0x9593
#define GL_NUM_DEVICE_UUIDS_EXT 0x9596
#define GL_DEVICE_UUID_EXT 0x9597
#define GL_DRIVER_UUID_EXT 0x9598
#define GL_PROTECTED_MEMORY_OBJECT_EXT 0x959B

typedef void (GLAPIENTRY * PFNGLBUFFERSTORAGEMEMEXTPROC) (GLenum target, GLsizeiptr size, GLuint memory, GLuint64 offset);
typedef void (GLAPIENTRY * PFNGLCREATEMEMORYOBJECTSEXTPROC) (GLsizei n, GLuint* memoryObjects);
typedef void (GLAPIENTRY * PFNGLDELETEMEMORYOBJECTSEXTPROC) (GLsizei n, const GLuint* memoryObjects);
typedef void (GLAPIENTRY * PFNGLGETMEMORYOBJECTPARAMETERIVEXTPROC) (GLuint memoryObject, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETUNSIGNEDBYTEI_VEXTPROC) (GLenum target, GLuint index, GLubyte* data);
typedef void (GLAPIENTRY * PFNGLGETUNSIGNEDBYTEVEXTPROC) (GLenum pname, GLubyte* data);
typedef GLboolean (GLAPIENTRY * PFNGLISMEMORYOBJECTEXTPROC) (GLuint memoryObject);
typedef void (GLAPIENTRY * PFNGLMEMORYOBJECTPARAMETERIVEXTPROC) (GLuint memoryObject, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERSTORAGEMEMEXTPROC) (GLuint buffer, GLsizeiptr size, GLuint memory, GLuint64 offset);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGEMEM1DEXTPROC) (GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGEMEM2DEXTPROC) (GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGEMEM2DMULTISAMPLEEXTPROC) (GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGEMEM3DEXTPROC) (GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGEMEM3DMULTISAMPLEEXTPROC) (GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGEMEM1DEXTPROC) (GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGEMEM2DEXTPROC) (GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGEMEM2DMULTISAMPLEEXTPROC) (GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGEMEM3DEXTPROC) (GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGEMEM3DMULTISAMPLEEXTPROC) (GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset);

#define glBufferStorageMemEXT GLEW_GET_FUN(__glewBufferStorageMemEXT)
#define glCreateMemoryObjectsEXT GLEW_GET_FUN(__glewCreateMemoryObjectsEXT)
#define glDeleteMemoryObjectsEXT GLEW_GET_FUN(__glewDeleteMemoryObjectsEXT)
#define glGetMemoryObjectParameterivEXT GLEW_GET_FUN(__glewGetMemoryObjectParameterivEXT)
#define glGetUnsignedBytei_vEXT GLEW_GET_FUN(__glewGetUnsignedBytei_vEXT)
#define glGetUnsignedBytevEXT GLEW_GET_FUN(__glewGetUnsignedBytevEXT)
#define glIsMemoryObjectEXT GLEW_GET_FUN(__glewIsMemoryObjectEXT)
#define glMemoryObjectParameterivEXT GLEW_GET_FUN(__glewMemoryObjectParameterivEXT)
#define glNamedBufferStorageMemEXT GLEW_GET_FUN(__glewNamedBufferStorageMemEXT)
#define glTexStorageMem1DEXT GLEW_GET_FUN(__glewTexStorageMem1DEXT)
#define glTexStorageMem2DEXT GLEW_GET_FUN(__glewTexStorageMem2DEXT)
#define glTexStorageMem2DMultisampleEXT GLEW_GET_FUN(__glewTexStorageMem2DMultisampleEXT)
#define glTexStorageMem3DEXT GLEW_GET_FUN(__glewTexStorageMem3DEXT)
#define glTexStorageMem3DMultisampleEXT GLEW_GET_FUN(__glewTexStorageMem3DMultisampleEXT)
#define glTextureStorageMem1DEXT GLEW_GET_FUN(__glewTextureStorageMem1DEXT)
#define glTextureStorageMem2DEXT GLEW_GET_FUN(__glewTextureStorageMem2DEXT)
#define glTextureStorageMem2DMultisampleEXT GLEW_GET_FUN(__glewTextureStorageMem2DMultisampleEXT)
#define glTextureStorageMem3DEXT GLEW_GET_FUN(__glewTextureStorageMem3DEXT)
#define glTextureStorageMem3DMultisampleEXT GLEW_GET_FUN(__glewTextureStorageMem3DMultisampleEXT)

#define GLEW_EXT_memory_object GLEW_GET_VAR(__GLEW_EXT_memory_object)

#endif /* GL_EXT_memory_object */

/* ------------------------ GL_EXT_memory_object_fd ------------------------ */

#ifndef GL_EXT_memory_object_fd
#define GL_EXT_memory_object_fd 1

#define GL_HANDLE_TYPE_OPAQUE_FD_EXT 0x9586

typedef void (GLAPIENTRY * PFNGLIMPORTMEMORYFDEXTPROC) (GLuint memory, GLuint64 size, GLenum handleType, GLint fd);

#define glImportMemoryFdEXT GLEW_GET_FUN(__glewImportMemoryFdEXT)

#define GLEW_EXT_memory_object_fd GLEW_GET_VAR(__GLEW_EXT_memory_object_fd)

#endif /* GL_EXT_memory_object_fd */

/* ----------------------- GL_EXT_memory_object_win32 ---------------------- */

#ifndef GL_EXT_memory_object_win32
#define GL_EXT_memory_object_win32 1

#define GL_LUID_SIZE_EXT 8
#define GL_HANDLE_TYPE_OPAQUE_WIN32_EXT 0x9587
#define GL_HANDLE_TYPE_OPAQUE_WIN32_KMT_EXT 0x9588
#define GL_HANDLE_TYPE_D3D12_TILEPOOL_EXT 0x9589
#define GL_HANDLE_TYPE_D3D12_RESOURCE_EXT 0x958A
#define GL_HANDLE_TYPE_D3D11_IMAGE_EXT 0x958B
#define GL_HANDLE_TYPE_D3D11_IMAGE_KMT_EXT 0x958C
#define GL_HANDLE_TYPE_D3D12_FENCE_EXT 0x9594
#define GL_D3D12_FENCE_VALUE_EXT 0x9595
#define GL_DEVICE_LUID_EXT 0x9599
#define GL_DEVICE_NODE_MASK_EXT 0x959A

typedef void (GLAPIENTRY * PFNGLIMPORTMEMORYWIN32HANDLEEXTPROC) (GLuint memory, GLuint64 size, GLenum handleType, void *handle);
typedef void (GLAPIENTRY * PFNGLIMPORTMEMORYWIN32NAMEEXTPROC) (GLuint memory, GLuint64 size, GLenum handleType, const void *name);

#define glImportMemoryWin32HandleEXT GLEW_GET_FUN(__glewImportMemoryWin32HandleEXT)
#define glImportMemoryWin32NameEXT GLEW_GET_FUN(__glewImportMemoryWin32NameEXT)

#define GLEW_EXT_memory_object_win32 GLEW_GET_VAR(__GLEW_EXT_memory_object_win32)

#endif /* GL_EXT_memory_object_win32 */

/* ------------------------- GL_EXT_misc_attribute ------------------------- */

#ifndef GL_EXT_misc_attribute
#define GL_EXT_misc_attribute 1

#define GLEW_EXT_misc_attribute GLEW_GET_VAR(__GLEW_EXT_misc_attribute)

#endif /* GL_EXT_misc_attribute */

/* ------------------------ GL_EXT_multi_draw_arrays ----------------------- */

#ifndef GL_EXT_multi_draw_arrays
#define GL_EXT_multi_draw_arrays 1

typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSEXTPROC) (GLenum mode, const GLint* first, const GLsizei *count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSEXTPROC) (GLenum mode, GLsizei* count, GLenum type, const void *const *indices, GLsizei primcount);

#define glMultiDrawArraysEXT GLEW_GET_FUN(__glewMultiDrawArraysEXT)
#define glMultiDrawElementsEXT GLEW_GET_FUN(__glewMultiDrawElementsEXT)

#define GLEW_EXT_multi_draw_arrays GLEW_GET_VAR(__GLEW_EXT_multi_draw_arrays)

#endif /* GL_EXT_multi_draw_arrays */

/* ----------------------- GL_EXT_multi_draw_indirect ---------------------- */

#ifndef GL_EXT_multi_draw_indirect
#define GL_EXT_multi_draw_indirect 1

typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSINDIRECTEXTPROC) (GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSINDIRECTEXTPROC) (GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride);

#define glMultiDrawArraysIndirectEXT GLEW_GET_FUN(__glewMultiDrawArraysIndirectEXT)
#define glMultiDrawElementsIndirectEXT GLEW_GET_FUN(__glewMultiDrawElementsIndirectEXT)

#define GLEW_EXT_multi_draw_indirect GLEW_GET_VAR(__GLEW_EXT_multi_draw_indirect)

#endif /* GL_EXT_multi_draw_indirect */

/* ------------------------ GL_EXT_multiple_textures ----------------------- */

#ifndef GL_EXT_multiple_textures
#define GL_EXT_multiple_textures 1

#define GLEW_EXT_multiple_textures GLEW_GET_VAR(__GLEW_EXT_multiple_textures)

#endif /* GL_EXT_multiple_textures */

/* --------------------------- GL_EXT_multisample -------------------------- */

#ifndef GL_EXT_multisample
#define GL_EXT_multisample 1

#define GL_MULTISAMPLE_EXT 0x809D
#define GL_SAMPLE_ALPHA_TO_MASK_EXT 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_EXT 0x809F
#define GL_SAMPLE_MASK_EXT 0x80A0
#define GL_1PASS_EXT 0x80A1
#define GL_2PASS_0_EXT 0x80A2
#define GL_2PASS_1_EXT 0x80A3
#define GL_4PASS_0_EXT 0x80A4
#define GL_4PASS_1_EXT 0x80A5
#define GL_4PASS_2_EXT 0x80A6
#define GL_4PASS_3_EXT 0x80A7
#define GL_SAMPLE_BUFFERS_EXT 0x80A8
#define GL_SAMPLES_EXT 0x80A9
#define GL_SAMPLE_MASK_VALUE_EXT 0x80AA
#define GL_SAMPLE_MASK_INVERT_EXT 0x80AB
#define GL_SAMPLE_PATTERN_EXT 0x80AC
#define GL_MULTISAMPLE_BIT_EXT 0x20000000

typedef void (GLAPIENTRY * PFNGLSAMPLEMASKEXTPROC) (GLclampf value, GLboolean invert);
typedef void (GLAPIENTRY * PFNGLSAMPLEPATTERNEXTPROC) (GLenum pattern);

#define glSampleMaskEXT GLEW_GET_FUN(__glewSampleMaskEXT)
#define glSamplePatternEXT GLEW_GET_FUN(__glewSamplePatternEXT)

#define GLEW_EXT_multisample GLEW_GET_VAR(__GLEW_EXT_multisample)

#endif /* GL_EXT_multisample */

/* -------------------- GL_EXT_multisample_compatibility ------------------- */

#ifndef GL_EXT_multisample_compatibility
#define GL_EXT_multisample_compatibility 1

#define GL_MULTISAMPLE_EXT 0x809D
#define GL_SAMPLE_ALPHA_TO_ONE_EXT 0x809F

#define GLEW_EXT_multisample_compatibility GLEW_GET_VAR(__GLEW_EXT_multisample_compatibility)

#endif /* GL_EXT_multisample_compatibility */

/* ----------------- GL_EXT_multisampled_render_to_texture ----------------- */

#ifndef GL_EXT_multisampled_render_to_texture
#define GL_EXT_multisampled_render_to_texture 1

#define GL_RENDERBUFFER_SAMPLES_EXT 0x8CAB
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT 0x8D56
#define GL_MAX_SAMPLES_EXT 0x8D57
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_SAMPLES_EXT 0x8D6C

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples);

#define glFramebufferTexture2DMultisampleEXT GLEW_GET_FUN(__glewFramebufferTexture2DMultisampleEXT)

#define GLEW_EXT_multisampled_render_to_texture GLEW_GET_VAR(__GLEW_EXT_multisampled_render_to_texture)

#endif /* GL_EXT_multisampled_render_to_texture */

/* ----------------- GL_EXT_multisampled_render_to_texture2 ---------------- */

#ifndef GL_EXT_multisampled_render_to_texture2
#define GL_EXT_multisampled_render_to_texture2 1

#define GLEW_EXT_multisampled_render_to_texture2 GLEW_GET_VAR(__GLEW_EXT_multisampled_render_to_texture2)

#endif /* GL_EXT_multisampled_render_to_texture2 */

/* --------------------- GL_EXT_multiview_draw_buffers --------------------- */

#ifndef GL_EXT_multiview_draw_buffers
#define GL_EXT_multiview_draw_buffers 1

#define GL_DRAW_BUFFER_EXT 0x0C01
#define GL_READ_BUFFER_EXT 0x0C02
#define GL_COLOR_ATTACHMENT_EXT 0x90F0
#define GL_MULTIVIEW_EXT 0x90F1
#define GL_MAX_MULTIVIEW_BUFFERS_EXT 0x90F2

typedef void (GLAPIENTRY * PFNGLDRAWBUFFERSINDEXEDEXTPROC) (GLint n, const GLenum* location, const GLint *indices);
typedef void (GLAPIENTRY * PFNGLGETINTEGERI_VEXTPROC) (GLenum target, GLuint index, GLint* data);
typedef void (GLAPIENTRY * PFNGLREADBUFFERINDEXEDEXTPROC) (GLenum src, GLint index);

#define glDrawBuffersIndexedEXT GLEW_GET_FUN(__glewDrawBuffersIndexedEXT)
#define glGetIntegeri_vEXT GLEW_GET_FUN(__glewGetIntegeri_vEXT)
#define glReadBufferIndexedEXT GLEW_GET_FUN(__glewReadBufferIndexedEXT)

#define GLEW_EXT_multiview_draw_buffers GLEW_GET_VAR(__GLEW_EXT_multiview_draw_buffers)

#endif /* GL_EXT_multiview_draw_buffers */

/* ---------------------- GL_EXT_packed_depth_stencil ---------------------- */

#ifndef GL_EXT_packed_depth_stencil
#define GL_EXT_packed_depth_stencil 1

#define GL_DEPTH_STENCIL_EXT 0x84F9
#define GL_UNSIGNED_INT_24_8_EXT 0x84FA
#define GL_DEPTH24_STENCIL8_EXT 0x88F0
#define GL_TEXTURE_STENCIL_SIZE_EXT 0x88F1

#define GLEW_EXT_packed_depth_stencil GLEW_GET_VAR(__GLEW_EXT_packed_depth_stencil)

#endif /* GL_EXT_packed_depth_stencil */

/* -------------------------- GL_EXT_packed_float -------------------------- */

#ifndef GL_EXT_packed_float
#define GL_EXT_packed_float 1

#define GL_R11F_G11F_B10F_EXT 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV_EXT 0x8C3B
#define GL_RGBA_SIGNED_COMPONENTS_EXT 0x8C3C

#define GLEW_EXT_packed_float GLEW_GET_VAR(__GLEW_EXT_packed_float)

#endif /* GL_EXT_packed_float */

/* -------------------------- GL_EXT_packed_pixels ------------------------- */

#ifndef GL_EXT_packed_pixels
#define GL_EXT_packed_pixels 1

#define GL_UNSIGNED_BYTE_3_3_2_EXT 0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4_EXT 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT 0x8034
#define GL_UNSIGNED_INT_8_8_8_8_EXT 0x8035
#define GL_UNSIGNED_INT_10_10_10_2_EXT 0x8036

#define GLEW_EXT_packed_pixels GLEW_GET_VAR(__GLEW_EXT_packed_pixels)

#endif /* GL_EXT_packed_pixels */

/* ------------------------ GL_EXT_paletted_texture ------------------------ */

#ifndef GL_EXT_paletted_texture
#define GL_EXT_paletted_texture 1

#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_PROXY_TEXTURE_1D 0x8063
#define GL_PROXY_TEXTURE_2D 0x8064
#define GL_COLOR_TABLE_FORMAT_EXT 0x80D8
#define GL_COLOR_TABLE_WIDTH_EXT 0x80D9
#define GL_COLOR_TABLE_RED_SIZE_EXT 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE_EXT 0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE_EXT 0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE_EXT 0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE_EXT 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE_EXT 0x80DF
#define GL_COLOR_INDEX1_EXT 0x80E2
#define GL_COLOR_INDEX2_EXT 0x80E3
#define GL_COLOR_INDEX4_EXT 0x80E4
#define GL_COLOR_INDEX8_EXT 0x80E5
#define GL_COLOR_INDEX12_EXT 0x80E6
#define GL_COLOR_INDEX16_EXT 0x80E7
#define GL_TEXTURE_INDEX_SIZE_EXT 0x80ED
#define GL_TEXTURE_CUBE_MAP_ARB 0x8513
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB 0x851B

typedef void (GLAPIENTRY * PFNGLCOLORTABLEEXTPROC) (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const void *data);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEEXTPROC) (GLenum target, GLenum format, GLenum type, void *data);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);

#define glColorTableEXT GLEW_GET_FUN(__glewColorTableEXT)
#define glGetColorTableEXT GLEW_GET_FUN(__glewGetColorTableEXT)
#define glGetColorTableParameterfvEXT GLEW_GET_FUN(__glewGetColorTableParameterfvEXT)
#define glGetColorTableParameterivEXT GLEW_GET_FUN(__glewGetColorTableParameterivEXT)

#define GLEW_EXT_paletted_texture GLEW_GET_VAR(__GLEW_EXT_paletted_texture)

#endif /* GL_EXT_paletted_texture */

/* ----------------------- GL_EXT_pixel_buffer_object ---------------------- */

#ifndef GL_EXT_pixel_buffer_object
#define GL_EXT_pixel_buffer_object 1

#define GL_PIXEL_PACK_BUFFER_EXT 0x88EB
#define GL_PIXEL_UNPACK_BUFFER_EXT 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_EXT 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_EXT 0x88EF

#define GLEW_EXT_pixel_buffer_object GLEW_GET_VAR(__GLEW_EXT_pixel_buffer_object)

#endif /* GL_EXT_pixel_buffer_object */

/* ------------------------- GL_EXT_pixel_transform ------------------------ */

#ifndef GL_EXT_pixel_transform
#define GL_EXT_pixel_transform 1

#define GL_PIXEL_TRANSFORM_2D_EXT 0x8330
#define GL_PIXEL_MAG_FILTER_EXT 0x8331
#define GL_PIXEL_MIN_FILTER_EXT 0x8332
#define GL_PIXEL_CUBIC_WEIGHT_EXT 0x8333
#define GL_CUBIC_EXT 0x8334
#define GL_AVERAGE_EXT 0x8335
#define GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8336
#define GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8337
#define GL_PIXEL_TRANSFORM_2D_MATRIX_EXT 0x8338

typedef void (GLAPIENTRY * PFNGLGETPIXELTRANSFORMPARAMETERFVEXTPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETPIXELTRANSFORMPARAMETERIVEXTPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFORMPARAMETERFEXTPROC) (GLenum target, GLenum pname, const GLfloat param);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFORMPARAMETERFVEXTPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFORMPARAMETERIEXTPROC) (GLenum target, GLenum pname, const GLint param);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFORMPARAMETERIVEXTPROC) (GLenum target, GLenum pname, const GLint* params);

#define glGetPixelTransformParameterfvEXT GLEW_GET_FUN(__glewGetPixelTransformParameterfvEXT)
#define glGetPixelTransformParameterivEXT GLEW_GET_FUN(__glewGetPixelTransformParameterivEXT)
#define glPixelTransformParameterfEXT GLEW_GET_FUN(__glewPixelTransformParameterfEXT)
#define glPixelTransformParameterfvEXT GLEW_GET_FUN(__glewPixelTransformParameterfvEXT)
#define glPixelTransformParameteriEXT GLEW_GET_FUN(__glewPixelTransformParameteriEXT)
#define glPixelTransformParameterivEXT GLEW_GET_FUN(__glewPixelTransformParameterivEXT)

#define GLEW_EXT_pixel_transform GLEW_GET_VAR(__GLEW_EXT_pixel_transform)

#endif /* GL_EXT_pixel_transform */

/* ------------------- GL_EXT_pixel_transform_color_table ------------------ */

#ifndef GL_EXT_pixel_transform_color_table
#define GL_EXT_pixel_transform_color_table 1

#define GLEW_EXT_pixel_transform_color_table GLEW_GET_VAR(__GLEW_EXT_pixel_transform_color_table)

#endif /* GL_EXT_pixel_transform_color_table */

/* ------------------------ GL_EXT_point_parameters ------------------------ */

#ifndef GL_EXT_point_parameters
#define GL_EXT_point_parameters 1

#define GL_POINT_SIZE_MIN_EXT 0x8126
#define GL_POINT_SIZE_MAX_EXT 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT 0x8128
#define GL_DISTANCE_ATTENUATION_EXT 0x8129

typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFEXTPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFVEXTPROC) (GLenum pname, const GLfloat* params);

#define glPointParameterfEXT GLEW_GET_FUN(__glewPointParameterfEXT)
#define glPointParameterfvEXT GLEW_GET_FUN(__glewPointParameterfvEXT)

#define GLEW_EXT_point_parameters GLEW_GET_VAR(__GLEW_EXT_point_parameters)

#endif /* GL_EXT_point_parameters */

/* ------------------------- GL_EXT_polygon_offset ------------------------- */

#ifndef GL_EXT_polygon_offset
#define GL_EXT_polygon_offset 1

#define GL_POLYGON_OFFSET_EXT 0x8037
#define GL_POLYGON_OFFSET_FACTOR_EXT 0x8038
#define GL_POLYGON_OFFSET_BIAS_EXT 0x8039

typedef void (GLAPIENTRY * PFNGLPOLYGONOFFSETEXTPROC) (GLfloat factor, GLfloat bias);

#define glPolygonOffsetEXT GLEW_GET_FUN(__glewPolygonOffsetEXT)

#define GLEW_EXT_polygon_offset GLEW_GET_VAR(__GLEW_EXT_polygon_offset)

#endif /* GL_EXT_polygon_offset */

/* ---------------------- GL_EXT_polygon_offset_clamp ---------------------- */

#ifndef GL_EXT_polygon_offset_clamp
#define GL_EXT_polygon_offset_clamp 1

#define GL_POLYGON_OFFSET_CLAMP_EXT 0x8E1B

typedef void (GLAPIENTRY * PFNGLPOLYGONOFFSETCLAMPEXTPROC) (GLfloat factor, GLfloat units, GLfloat clamp);

#define glPolygonOffsetClampEXT GLEW_GET_FUN(__glewPolygonOffsetClampEXT)

#define GLEW_EXT_polygon_offset_clamp GLEW_GET_VAR(__GLEW_EXT_polygon_offset_clamp)

#endif /* GL_EXT_polygon_offset_clamp */

/* ----------------------- GL_EXT_post_depth_coverage ---------------------- */

#ifndef GL_EXT_post_depth_coverage
#define GL_EXT_post_depth_coverage 1

#define GLEW_EXT_post_depth_coverage GLEW_GET_VAR(__GLEW_EXT_post_depth_coverage)

#endif /* GL_EXT_post_depth_coverage */

/* ------------------------ GL_EXT_provoking_vertex ------------------------ */

#ifndef GL_EXT_provoking_vertex
#define GL_EXT_provoking_vertex 1

#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION_EXT 0x8E4C
#define GL_FIRST_VERTEX_CONVENTION_EXT 0x8E4D
#define GL_LAST_VERTEX_CONVENTION_EXT 0x8E4E
#define GL_PROVOKING_VERTEX_EXT 0x8E4F

typedef void (GLAPIENTRY * PFNGLPROVOKINGVERTEXEXTPROC) (GLenum mode);

#define glProvokingVertexEXT GLEW_GET_FUN(__glewProvokingVertexEXT)

#define GLEW_EXT_provoking_vertex GLEW_GET_VAR(__GLEW_EXT_provoking_vertex)

#endif /* GL_EXT_provoking_vertex */

/* --------------------------- GL_EXT_pvrtc_sRGB --------------------------- */

#ifndef GL_EXT_pvrtc_sRGB
#define GL_EXT_pvrtc_sRGB 1

#define GL_COMPRESSED_SRGB_PVRTC_2BPPV1_EXT 0x8A54
#define GL_COMPRESSED_SRGB_PVRTC_4BPPV1_EXT 0x8A55
#define GL_COMPRESSED_SRGB_ALPHA_PVRTC_2BPPV1_EXT 0x8A56
#define GL_COMPRESSED_SRGB_ALPHA_PVRTC_4BPPV1_EXT 0x8A57

#define GLEW_EXT_pvrtc_sRGB GLEW_GET_VAR(__GLEW_EXT_pvrtc_sRGB)

#endif /* GL_EXT_pvrtc_sRGB */

/* ----------------------- GL_EXT_raster_multisample ----------------------- */

#ifndef GL_EXT_raster_multisample
#define GL_EXT_raster_multisample 1

#define GL_COLOR_SAMPLES_NV 0x8E20
#define GL_RASTER_MULTISAMPLE_EXT 0x9327
#define GL_RASTER_SAMPLES_EXT 0x9328
#define GL_MAX_RASTER_SAMPLES_EXT 0x9329
#define GL_RASTER_FIXED_SAMPLE_LOCATIONS_EXT 0x932A
#define GL_MULTISAMPLE_RASTERIZATION_ALLOWED_EXT 0x932B
#define GL_EFFECTIVE_RASTER_SAMPLES_EXT 0x932C
#define GL_DEPTH_SAMPLES_NV 0x932D
#define GL_STENCIL_SAMPLES_NV 0x932E
#define GL_MIXED_DEPTH_SAMPLES_SUPPORTED_NV 0x932F
#define GL_MIXED_STENCIL_SAMPLES_SUPPORTED_NV 0x9330
#define GL_COVERAGE_MODULATION_TABLE_NV 0x9331
#define GL_COVERAGE_MODULATION_NV 0x9332
#define GL_COVERAGE_MODULATION_TABLE_SIZE_NV 0x9333

typedef void (GLAPIENTRY * PFNGLCOVERAGEMODULATIONNVPROC) (GLenum components);
typedef void (GLAPIENTRY * PFNGLCOVERAGEMODULATIONTABLENVPROC) (GLsizei n, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLGETCOVERAGEMODULATIONTABLENVPROC) (GLsizei bufsize, GLfloat* v);
typedef void (GLAPIENTRY * PFNGLRASTERSAMPLESEXTPROC) (GLuint samples, GLboolean fixedsamplelocations);

#define glCoverageModulationNV GLEW_GET_FUN(__glewCoverageModulationNV)
#define glCoverageModulationTableNV GLEW_GET_FUN(__glewCoverageModulationTableNV)
#define glGetCoverageModulationTableNV GLEW_GET_FUN(__glewGetCoverageModulationTableNV)
#define glRasterSamplesEXT GLEW_GET_FUN(__glewRasterSamplesEXT)

#define GLEW_EXT_raster_multisample GLEW_GET_VAR(__GLEW_EXT_raster_multisample)

#endif /* GL_EXT_raster_multisample */

/* ------------------------ GL_EXT_read_format_bgra ------------------------ */

#ifndef GL_EXT_read_format_bgra
#define GL_EXT_read_format_bgra 1

#define GL_BGRA_EXT 0x80E1
#define GL_UNSIGNED_SHORT_4_4_4_4_REV_EXT 0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV_EXT 0x8366

#define GLEW_EXT_read_format_bgra GLEW_GET_VAR(__GLEW_EXT_read_format_bgra)

#endif /* GL_EXT_read_format_bgra */

/* -------------------------- GL_EXT_render_snorm -------------------------- */

#ifndef GL_EXT_render_snorm
#define GL_EXT_render_snorm 1

#define GL_BYTE 0x1400
#define GL_SHORT 0x1402
#define GL_R8_SNORM 0x8F94
#define GL_RG8_SNORM 0x8F95
#define GL_RGBA8_SNORM 0x8F97
#define GL_R16_SNORM_EXT 0x8F98
#define GL_RG16_SNORM_EXT 0x8F99
#define GL_RGBA16_SNORM_EXT 0x8F9B

#define GLEW_EXT_render_snorm GLEW_GET_VAR(__GLEW_EXT_render_snorm)

#endif /* GL_EXT_render_snorm */

/* ------------------------- GL_EXT_rescale_normal ------------------------- */

#ifndef GL_EXT_rescale_normal
#define GL_EXT_rescale_normal 1

#define GL_RESCALE_NORMAL_EXT 0x803A

#define GLEW_EXT_rescale_normal GLEW_GET_VAR(__GLEW_EXT_rescale_normal)

#endif /* GL_EXT_rescale_normal */

/* ------------------------------ GL_EXT_sRGB ------------------------------ */

#ifndef GL_EXT_sRGB
#define GL_EXT_sRGB 1

#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_EXT 0x8210
#define GL_SRGB_EXT 0x8C40
#define GL_SRGB_ALPHA_EXT 0x8C42
#define GL_SRGB8_ALPHA8_EXT 0x8C43

#define GLEW_EXT_sRGB GLEW_GET_VAR(__GLEW_EXT_sRGB)

#endif /* GL_EXT_sRGB */

/* ----------------------- GL_EXT_sRGB_write_control ----------------------- */

#ifndef GL_EXT_sRGB_write_control
#define GL_EXT_sRGB_write_control 1

#define GL_FRAMEBUFFER_SRGB_EXT 0x8DB9

#define GLEW_EXT_sRGB_write_control GLEW_GET_VAR(__GLEW_EXT_sRGB_write_control)

#endif /* GL_EXT_sRGB_write_control */

/* -------------------------- GL_EXT_scene_marker -------------------------- */

#ifndef GL_EXT_scene_marker
#define GL_EXT_scene_marker 1

typedef void (GLAPIENTRY * PFNGLBEGINSCENEEXTPROC) (void);
typedef void (GLAPIENTRY * PFNGLENDSCENEEXTPROC) (void);

#define glBeginSceneEXT GLEW_GET_FUN(__glewBeginSceneEXT)
#define glEndSceneEXT GLEW_GET_FUN(__glewEndSceneEXT)

#define GLEW_EXT_scene_marker GLEW_GET_VAR(__GLEW_EXT_scene_marker)

#endif /* GL_EXT_scene_marker */

/* ------------------------- GL_EXT_secondary_color ------------------------ */

#ifndef GL_EXT_secondary_color
#define GL_EXT_secondary_color 1

#define GL_COLOR_SUM_EXT 0x8458
#define GL_CURRENT_SECONDARY_COLOR_EXT 0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE_EXT 0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE_EXT 0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE_EXT 0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER_EXT 0x845D
#define GL_SECONDARY_COLOR_ARRAY_EXT 0x845E

typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BEXTPROC) (GLbyte red, GLbyte green, GLbyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BVEXTPROC) (const GLbyte *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DEXTPROC) (GLdouble red, GLdouble green, GLdouble blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DVEXTPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FEXTPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FVEXTPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IEXTPROC) (GLint red, GLint green, GLint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IVEXTPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SEXTPROC) (GLshort red, GLshort green, GLshort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SVEXTPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBEXTPROC) (GLubyte red, GLubyte green, GLubyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBVEXTPROC) (const GLubyte *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIEXTPROC) (GLuint red, GLuint green, GLuint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIVEXTPROC) (const GLuint *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USEXTPROC) (GLushort red, GLushort green, GLushort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USVEXTPROC) (const GLushort *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, const void *pointer);

#define glSecondaryColor3bEXT GLEW_GET_FUN(__glewSecondaryColor3bEXT)
#define glSecondaryColor3bvEXT GLEW_GET_FUN(__glewSecondaryColor3bvEXT)
#define glSecondaryColor3dEXT GLEW_GET_FUN(__glewSecondaryColor3dEXT)
#define glSecondaryColor3dvEXT GLEW_GET_FUN(__glewSecondaryColor3dvEXT)
#define glSecondaryColor3fEXT GLEW_GET_FUN(__glewSecondaryColor3fEXT)
#define glSecondaryColor3fvEXT GLEW_GET_FUN(__glewSecondaryColor3fvEXT)
#define glSecondaryColor3iEXT GLEW_GET_FUN(__glewSecondaryColor3iEXT)
#define glSecondaryColor3ivEXT GLEW_GET_FUN(__glewSecondaryColor3ivEXT)
#define glSecondaryColor3sEXT GLEW_GET_FUN(__glewSecondaryColor3sEXT)
#define glSecondaryColor3svEXT GLEW_GET_FUN(__glewSecondaryColor3svEXT)
#define glSecondaryColor3ubEXT GLEW_GET_FUN(__glewSecondaryColor3ubEXT)
#define glSecondaryColor3ubvEXT GLEW_GET_FUN(__glewSecondaryColor3ubvEXT)
#define glSecondaryColor3uiEXT GLEW_GET_FUN(__glewSecondaryColor3uiEXT)
#define glSecondaryColor3uivEXT GLEW_GET_FUN(__glewSecondaryColor3uivEXT)
#define glSecondaryColor3usEXT GLEW_GET_FUN(__glewSecondaryColor3usEXT)
#define glSecondaryColor3usvEXT GLEW_GET_FUN(__glewSecondaryColor3usvEXT)
#define glSecondaryColorPointerEXT GLEW_GET_FUN(__glewSecondaryColorPointerEXT)

#define GLEW_EXT_secondary_color GLEW_GET_VAR(__GLEW_EXT_secondary_color)

#endif /* GL_EXT_secondary_color */

/* ---------------------------- GL_EXT_semaphore --------------------------- */

#ifndef GL_EXT_semaphore
#define GL_EXT_semaphore 1

typedef void (GLAPIENTRY * PFNGLDELETESEMAPHORESEXTPROC) (GLsizei n, const GLuint* semaphores);
typedef void (GLAPIENTRY * PFNGLGENSEMAPHORESEXTPROC) (GLsizei n, GLuint* semaphores);
typedef void (GLAPIENTRY * PFNGLGETSEMAPHOREPARAMETERUI64VEXTPROC) (GLuint semaphore, GLenum pname, GLuint64* params);
typedef GLboolean (GLAPIENTRY * PFNGLISSEMAPHOREEXTPROC) (GLuint semaphore);
typedef void (GLAPIENTRY * PFNGLSEMAPHOREPARAMETERUI64VEXTPROC) (GLuint semaphore, GLenum pname, const GLuint64* params);
typedef void (GLAPIENTRY * PFNGLSIGNALSEMAPHOREEXTPROC) (GLuint semaphore, GLuint numBufferBarriers, const GLuint* buffers, GLuint numTextureBarriers, const GLuint *textures, const GLenum *dstLayouts);
typedef void (GLAPIENTRY * PFNGLWAITSEMAPHOREEXTPROC) (GLuint semaphore, GLuint numBufferBarriers, const GLuint* buffers, GLuint numTextureBarriers, const GLuint *textures, const GLenum *srcLayouts);

#define glDeleteSemaphoresEXT GLEW_GET_FUN(__glewDeleteSemaphoresEXT)
#define glGenSemaphoresEXT GLEW_GET_FUN(__glewGenSemaphoresEXT)
#define glGetSemaphoreParameterui64vEXT GLEW_GET_FUN(__glewGetSemaphoreParameterui64vEXT)
#define glIsSemaphoreEXT GLEW_GET_FUN(__glewIsSemaphoreEXT)
#define glSemaphoreParameterui64vEXT GLEW_GET_FUN(__glewSemaphoreParameterui64vEXT)
#define glSignalSemaphoreEXT GLEW_GET_FUN(__glewSignalSemaphoreEXT)
#define glWaitSemaphoreEXT GLEW_GET_FUN(__glewWaitSemaphoreEXT)

#define GLEW_EXT_semaphore GLEW_GET_VAR(__GLEW_EXT_semaphore)

#endif /* GL_EXT_semaphore */

/* -------------------------- GL_EXT_semaphore_fd -------------------------- */

#ifndef GL_EXT_semaphore_fd
#define GL_EXT_semaphore_fd 1

typedef void (GLAPIENTRY * PFNGLIMPORTSEMAPHOREFDEXTPROC) (GLuint semaphore, GLenum handleType, GLint fd);

#define glImportSemaphoreFdEXT GLEW_GET_FUN(__glewImportSemaphoreFdEXT)

#define GLEW_EXT_semaphore_fd GLEW_GET_VAR(__GLEW_EXT_semaphore_fd)

#endif /* GL_EXT_semaphore_fd */

/* ------------------------- GL_EXT_semaphore_win32 ------------------------ */

#ifndef GL_EXT_semaphore_win32
#define GL_EXT_semaphore_win32 1

typedef void (GLAPIENTRY * PFNGLIMPORTSEMAPHOREWIN32HANDLEEXTPROC) (GLuint semaphore, GLenum handleType, void *handle);
typedef void (GLAPIENTRY * PFNGLIMPORTSEMAPHOREWIN32NAMEEXTPROC) (GLuint semaphore, GLenum handleType, const void *name);

#define glImportSemaphoreWin32HandleEXT GLEW_GET_FUN(__glewImportSemaphoreWin32HandleEXT)
#define glImportSemaphoreWin32NameEXT GLEW_GET_FUN(__glewImportSemaphoreWin32NameEXT)

#define GLEW_EXT_semaphore_win32 GLEW_GET_VAR(__GLEW_EXT_semaphore_win32)

#endif /* GL_EXT_semaphore_win32 */

/* --------------------- GL_EXT_separate_shader_objects -------------------- */

#ifndef GL_EXT_separate_shader_objects
#define GL_EXT_separate_shader_objects 1

#define GL_ACTIVE_PROGRAM_EXT 0x8B8D

typedef void (GLAPIENTRY * PFNGLACTIVEPROGRAMEXTPROC) (GLuint program);
typedef GLuint (GLAPIENTRY * PFNGLCREATESHADERPROGRAMEXTPROC) (GLenum type, const GLchar* string);
typedef void (GLAPIENTRY * PFNGLUSESHADERPROGRAMEXTPROC) (GLenum type, GLuint program);

#define glActiveProgramEXT GLEW_GET_FUN(__glewActiveProgramEXT)
#define glCreateShaderProgramEXT GLEW_GET_FUN(__glewCreateShaderProgramEXT)
#define glUseShaderProgramEXT GLEW_GET_FUN(__glewUseShaderProgramEXT)

#define GLEW_EXT_separate_shader_objects GLEW_GET_VAR(__GLEW_EXT_separate_shader_objects)

#endif /* GL_EXT_separate_shader_objects */

/* --------------------- GL_EXT_separate_specular_color -------------------- */

#ifndef GL_EXT_separate_specular_color
#define GL_EXT_separate_specular_color 1

#define GL_LIGHT_MODEL_COLOR_CONTROL_EXT 0x81F8
#define GL_SINGLE_COLOR_EXT 0x81F9
#define GL_SEPARATE_SPECULAR_COLOR_EXT 0x81FA

#define GLEW_EXT_separate_specular_color GLEW_GET_VAR(__GLEW_EXT_separate_specular_color)

#endif /* GL_EXT_separate_specular_color */

/* -------------------- GL_EXT_shader_framebuffer_fetch -------------------- */

#ifndef GL_EXT_shader_framebuffer_fetch
#define GL_EXT_shader_framebuffer_fetch 1

#define GL_FRAGMENT_SHADER_DISCARDS_SAMPLES_EXT 0x8A52

#define GLEW_EXT_shader_framebuffer_fetch GLEW_GET_VAR(__GLEW_EXT_shader_framebuffer_fetch)

#endif /* GL_EXT_shader_framebuffer_fetch */

/* ------------------------ GL_EXT_shader_group_vote ----------------------- */

#ifndef GL_EXT_shader_group_vote
#define GL_EXT_shader_group_vote 1

#define GLEW_EXT_shader_group_vote GLEW_GET_VAR(__GLEW_EXT_shader_group_vote)

#endif /* GL_EXT_shader_group_vote */

/* ------------------- GL_EXT_shader_image_load_formatted ------------------ */

#ifndef GL_EXT_shader_image_load_formatted
#define GL_EXT_shader_image_load_formatted 1

#define GLEW_EXT_shader_image_load_formatted GLEW_GET_VAR(__GLEW_EXT_shader_image_load_formatted)

#endif /* GL_EXT_shader_image_load_formatted */

/* --------------------- GL_EXT_shader_image_load_store -------------------- */

#ifndef GL_EXT_shader_image_load_store
#define GL_EXT_shader_image_load_store 1

#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT_EXT 0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT_EXT 0x00000002
#define GL_UNIFORM_BARRIER_BIT_EXT 0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT_EXT 0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT_EXT 0x00000020
#define GL_COMMAND_BARRIER_BIT_EXT 0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT_EXT 0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT_EXT 0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT_EXT 0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT_EXT 0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT_EXT 0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT_EXT 0x00001000
#define GL_MAX_IMAGE_UNITS_EXT 0x8F38
#define GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS_EXT 0x8F39
#define GL_IMAGE_BINDING_NAME_EXT 0x8F3A
#define GL_IMAGE_BINDING_LEVEL_EXT 0x8F3B
#define GL_IMAGE_BINDING_LAYERED_EXT 0x8F3C
#define GL_IMAGE_BINDING_LAYER_EXT 0x8F3D
#define GL_IMAGE_BINDING_ACCESS_EXT 0x8F3E
#define GL_IMAGE_1D_EXT 0x904C
#define GL_IMAGE_2D_EXT 0x904D
#define GL_IMAGE_3D_EXT 0x904E
#define GL_IMAGE_2D_RECT_EXT 0x904F
#define GL_IMAGE_CUBE_EXT 0x9050
#define GL_IMAGE_BUFFER_EXT 0x9051
#define GL_IMAGE_1D_ARRAY_EXT 0x9052
#define GL_IMAGE_2D_ARRAY_EXT 0x9053
#define GL_IMAGE_CUBE_MAP_ARRAY_EXT 0x9054
#define GL_IMAGE_2D_MULTISAMPLE_EXT 0x9055
#define GL_IMAGE_2D_MULTISAMPLE_ARRAY_EXT 0x9056
#define GL_INT_IMAGE_1D_EXT 0x9057
#define GL_INT_IMAGE_2D_EXT 0x9058
#define GL_INT_IMAGE_3D_EXT 0x9059
#define GL_INT_IMAGE_2D_RECT_EXT 0x905A
#define GL_INT_IMAGE_CUBE_EXT 0x905B
#define GL_INT_IMAGE_BUFFER_EXT 0x905C
#define GL_INT_IMAGE_1D_ARRAY_EXT 0x905D
#define GL_INT_IMAGE_2D_ARRAY_EXT 0x905E
#define GL_INT_IMAGE_CUBE_MAP_ARRAY_EXT 0x905F
#define GL_INT_IMAGE_2D_MULTISAMPLE_EXT 0x9060
#define GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY_EXT 0x9061
#define GL_UNSIGNED_INT_IMAGE_1D_EXT 0x9062
#define GL_UNSIGNED_INT_IMAGE_2D_EXT 0x9063
#define GL_UNSIGNED_INT_IMAGE_3D_EXT 0x9064
#define GL_UNSIGNED_INT_IMAGE_2D_RECT_EXT 0x9065
#define GL_UNSIGNED_INT_IMAGE_CUBE_EXT 0x9066
#define GL_UNSIGNED_INT_IMAGE_BUFFER_EXT 0x9067
#define GL_UNSIGNED_INT_IMAGE_1D_ARRAY_EXT 0x9068
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY_EXT 0x9069
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY_EXT 0x906A
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_EXT 0x906B
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY_EXT 0x906C
#define GL_MAX_IMAGE_SAMPLES_EXT 0x906D
#define GL_IMAGE_BINDING_FORMAT_EXT 0x906E
#define GL_ALL_BARRIER_BITS_EXT 0xFFFFFFFF

typedef void (GLAPIENTRY * PFNGLBINDIMAGETEXTUREEXTPROC) (GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format);
typedef void (GLAPIENTRY * PFNGLMEMORYBARRIEREXTPROC) (GLbitfield barriers);

#define glBindImageTextureEXT GLEW_GET_FUN(__glewBindImageTextureEXT)
#define glMemoryBarrierEXT GLEW_GET_FUN(__glewMemoryBarrierEXT)

#define GLEW_EXT_shader_image_load_store GLEW_GET_VAR(__GLEW_EXT_shader_image_load_store)

#endif /* GL_EXT_shader_image_load_store */

/* ------------------- GL_EXT_shader_implicit_conversions ------------------ */

#ifndef GL_EXT_shader_implicit_conversions
#define GL_EXT_shader_implicit_conversions 1

#define GLEW_EXT_shader_implicit_conversions GLEW_GET_VAR(__GLEW_EXT_shader_implicit_conversions)

#endif /* GL_EXT_shader_implicit_conversions */

/* ----------------------- GL_EXT_shader_integer_mix ----------------------- */

#ifndef GL_EXT_shader_integer_mix
#define GL_EXT_shader_integer_mix 1

#define GLEW_EXT_shader_integer_mix GLEW_GET_VAR(__GLEW_EXT_shader_integer_mix)

#endif /* GL_EXT_shader_integer_mix */

/* ------------------------ GL_EXT_shader_io_blocks ------------------------ */

#ifndef GL_EXT_shader_io_blocks
#define GL_EXT_shader_io_blocks 1

#define GLEW_EXT_shader_io_blocks GLEW_GET_VAR(__GLEW_EXT_shader_io_blocks)

#endif /* GL_EXT_shader_io_blocks */

/* ------------- GL_EXT_shader_non_constant_global_initializers ------------ */

#ifndef GL_EXT_shader_non_constant_global_initializers
#define GL_EXT_shader_non_constant_global_initializers 1

#define GLEW_EXT_shader_non_constant_global_initializers GLEW_GET_VAR(__GLEW_EXT_shader_non_constant_global_initializers)

#endif /* GL_EXT_shader_non_constant_global_initializers */

/* ------------------- GL_EXT_shader_pixel_local_storage ------------------- */

#ifndef GL_EXT_shader_pixel_local_storage
#define GL_EXT_shader_pixel_local_storage 1

#define GL_MAX_SHADER_PIXEL_LOCAL_STORAGE_FAST_SIZE_EXT 0x8F63
#define GL_SHADER_PIXEL_LOCAL_STORAGE_EXT 0x8F64
#define GL_MAX_SHADER_PIXEL_LOCAL_STORAGE_SIZE_EXT 0x8F67

#define GLEW_EXT_shader_pixel_local_storage GLEW_GET_VAR(__GLEW_EXT_shader_pixel_local_storage)

#endif /* GL_EXT_shader_pixel_local_storage */

/* ------------------- GL_EXT_shader_pixel_local_storage2 ------------------ */

#ifndef GL_EXT_shader_pixel_local_storage2
#define GL_EXT_shader_pixel_local_storage2 1

#define GL_MAX_SHADER_COMBINED_LOCAL_STORAGE_FAST_SIZE_EXT 0x9650
#define GL_MAX_SHADER_COMBINED_LOCAL_STORAGE_SIZE_EXT 0x9651
#define GL_FRAMEBUFFER_INCOMPLETE_INSUFFICIENT_SHADER_COMBINED_LOCAL_STORAGE_EXT 0x9652

typedef void (GLAPIENTRY * PFNGLCLEARPIXELLOCALSTORAGEUIEXTPROC) (GLsizei offset, GLsizei n, const GLuint* values);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERPIXELLOCALSTORAGESIZEEXTPROC) (GLuint target, GLsizei size);
typedef GLsizei (GLAPIENTRY * PFNGLGETFRAMEBUFFERPIXELLOCALSTORAGESIZEEXTPROC) (GLuint target);

#define glClearPixelLocalStorageuiEXT GLEW_GET_FUN(__glewClearPixelLocalStorageuiEXT)
#define glFramebufferPixelLocalStorageSizeEXT GLEW_GET_FUN(__glewFramebufferPixelLocalStorageSizeEXT)
#define glGetFramebufferPixelLocalStorageSizeEXT GLEW_GET_FUN(__glewGetFramebufferPixelLocalStorageSizeEXT)

#define GLEW_EXT_shader_pixel_local_storage2 GLEW_GET_VAR(__GLEW_EXT_shader_pixel_local_storage2)

#endif /* GL_EXT_shader_pixel_local_storage2 */

/* ----------------------- GL_EXT_shader_texture_lod ----------------------- */

#ifndef GL_EXT_shader_texture_lod
#define GL_EXT_shader_texture_lod 1

#define GLEW_EXT_shader_texture_lod GLEW_GET_VAR(__GLEW_EXT_shader_texture_lod)

#endif /* GL_EXT_shader_texture_lod */

/* -------------------------- GL_EXT_shadow_funcs -------------------------- */

#ifndef GL_EXT_shadow_funcs
#define GL_EXT_shadow_funcs 1

#define GLEW_EXT_shadow_funcs GLEW_GET_VAR(__GLEW_EXT_shadow_funcs)

#endif /* GL_EXT_shadow_funcs */

/* ------------------------- GL_EXT_shadow_samplers ------------------------ */

#ifndef GL_EXT_shadow_samplers
#define GL_EXT_shadow_samplers 1

#define GL_TEXTURE_COMPARE_MODE_EXT 0x884C
#define GL_TEXTURE_COMPARE_FUNC_EXT 0x884D
#define GL_COMPARE_REF_TO_TEXTURE_EXT 0x884E
#define GL_SAMPLER_2D_SHADOW_EXT 0x8B62

#define GLEW_EXT_shadow_samplers GLEW_GET_VAR(__GLEW_EXT_shadow_samplers)

#endif /* GL_EXT_shadow_samplers */

/* --------------------- GL_EXT_shared_texture_palette --------------------- */

#ifndef GL_EXT_shared_texture_palette
#define GL_EXT_shared_texture_palette 1

#define GL_SHARED_TEXTURE_PALETTE_EXT 0x81FB

#define GLEW_EXT_shared_texture_palette GLEW_GET_VAR(__GLEW_EXT_shared_texture_palette)

#endif /* GL_EXT_shared_texture_palette */

/* ------------------------- GL_EXT_sparse_texture ------------------------- */

#ifndef GL_EXT_sparse_texture
#define GL_EXT_sparse_texture 1

#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_3D 0x806F
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_TEXTURE_CUBE_MAP_ARRAY_OES 0x9009
#define GL_VIRTUAL_PAGE_SIZE_X_EXT 0x9195
#define GL_VIRTUAL_PAGE_SIZE_Y_EXT 0x9196
#define GL_VIRTUAL_PAGE_SIZE_Z_EXT 0x9197
#define GL_MAX_SPARSE_TEXTURE_SIZE_EXT 0x9198
#define GL_MAX_SPARSE_3D_TEXTURE_SIZE_EXT 0x9199
#define GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_EXT 0x919A
#define GL_TEXTURE_SPARSE_EXT 0x91A6
#define GL_VIRTUAL_PAGE_SIZE_INDEX_EXT 0x91A7
#define GL_NUM_VIRTUAL_PAGE_SIZES_EXT 0x91A8
#define GL_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_EXT 0x91A9
#define GL_NUM_SPARSE_LEVELS_EXT 0x91AA

typedef void (GLAPIENTRY * PFNGLTEXPAGECOMMITMENTEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit);
typedef void (GLAPIENTRY * PFNGLTEXTUREPAGECOMMITMENTEXTPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit);

#define glTexPageCommitmentEXT GLEW_GET_FUN(__glewTexPageCommitmentEXT)
#define glTexturePageCommitmentEXT GLEW_GET_FUN(__glewTexturePageCommitmentEXT)

#define GLEW_EXT_sparse_texture GLEW_GET_VAR(__GLEW_EXT_sparse_texture)

#endif /* GL_EXT_sparse_texture */

/* ------------------------- GL_EXT_sparse_texture2 ------------------------ */

#ifndef GL_EXT_sparse_texture2
#define GL_EXT_sparse_texture2 1

#define GLEW_EXT_sparse_texture2 GLEW_GET_VAR(__GLEW_EXT_sparse_texture2)

#endif /* GL_EXT_sparse_texture2 */

/* ------------------------ GL_EXT_stencil_clear_tag ----------------------- */

#ifndef GL_EXT_stencil_clear_tag
#define GL_EXT_stencil_clear_tag 1

#define GL_STENCIL_TAG_BITS_EXT 0x88F2
#define GL_STENCIL_CLEAR_TAG_VALUE_EXT 0x88F3

#define GLEW_EXT_stencil_clear_tag GLEW_GET_VAR(__GLEW_EXT_stencil_clear_tag)

#endif /* GL_EXT_stencil_clear_tag */

/* ------------------------ GL_EXT_stencil_two_side ------------------------ */

#ifndef GL_EXT_stencil_two_side
#define GL_EXT_stencil_two_side 1

#define GL_STENCIL_TEST_TWO_SIDE_EXT 0x8910
#define GL_ACTIVE_STENCIL_FACE_EXT 0x8911

typedef void (GLAPIENTRY * PFNGLACTIVESTENCILFACEEXTPROC) (GLenum face);

#define glActiveStencilFaceEXT GLEW_GET_FUN(__glewActiveStencilFaceEXT)

#define GLEW_EXT_stencil_two_side GLEW_GET_VAR(__GLEW_EXT_stencil_two_side)

#endif /* GL_EXT_stencil_two_side */

/* -------------------------- GL_EXT_stencil_wrap -------------------------- */

#ifndef GL_EXT_stencil_wrap
#define GL_EXT_stencil_wrap 1

#define GL_INCR_WRAP_EXT 0x8507
#define GL_DECR_WRAP_EXT 0x8508

#define GLEW_EXT_stencil_wrap GLEW_GET_VAR(__GLEW_EXT_stencil_wrap)

#endif /* GL_EXT_stencil_wrap */

/* --------------------------- GL_EXT_subtexture --------------------------- */

#ifndef GL_EXT_subtexture
#define GL_EXT_subtexture 1

typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE1DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE2DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE3DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);

#define glTexSubImage1DEXT GLEW_GET_FUN(__glewTexSubImage1DEXT)
#define glTexSubImage2DEXT GLEW_GET_FUN(__glewTexSubImage2DEXT)
#define glTexSubImage3DEXT GLEW_GET_FUN(__glewTexSubImage3DEXT)

#define GLEW_EXT_subtexture GLEW_GET_VAR(__GLEW_EXT_subtexture)

#endif /* GL_EXT_subtexture */

/* ----------------------------- GL_EXT_texture ---------------------------- */

#ifndef GL_EXT_texture
#define GL_EXT_texture 1

#define GL_ALPHA4_EXT 0x803B
#define GL_ALPHA8_EXT 0x803C
#define GL_ALPHA12_EXT 0x803D
#define GL_ALPHA16_EXT 0x803E
#define GL_LUMINANCE4_EXT 0x803F
#define GL_LUMINANCE8_EXT 0x8040
#define GL_LUMINANCE12_EXT 0x8041
#define GL_LUMINANCE16_EXT 0x8042
#define GL_LUMINANCE4_ALPHA4_EXT 0x8043
#define GL_LUMINANCE6_ALPHA2_EXT 0x8044
#define GL_LUMINANCE8_ALPHA8_EXT 0x8045
#define GL_LUMINANCE12_ALPHA4_EXT 0x8046
#define GL_LUMINANCE12_ALPHA12_EXT 0x8047
#define GL_LUMINANCE16_ALPHA16_EXT 0x8048
#define GL_INTENSITY_EXT 0x8049
#define GL_INTENSITY4_EXT 0x804A
#define GL_INTENSITY8_EXT 0x804B
#define GL_INTENSITY12_EXT 0x804C
#define GL_INTENSITY16_EXT 0x804D
#define GL_RGB2_EXT 0x804E
#define GL_RGB4_EXT 0x804F
#define GL_RGB5_EXT 0x8050
#define GL_RGB8_EXT 0x8051
#define GL_RGB10_EXT 0x8052
#define GL_RGB12_EXT 0x8053
#define GL_RGB16_EXT 0x8054
#define GL_RGBA2_EXT 0x8055
#define GL_RGBA4_EXT 0x8056
#define GL_RGB5_A1_EXT 0x8057
#define GL_RGBA8_EXT 0x8058
#define GL_RGB10_A2_EXT 0x8059
#define GL_RGBA12_EXT 0x805A
#define GL_RGBA16_EXT 0x805B
#define GL_TEXTURE_RED_SIZE_EXT 0x805C
#define GL_TEXTURE_GREEN_SIZE_EXT 0x805D
#define GL_TEXTURE_BLUE_SIZE_EXT 0x805E
#define GL_TEXTURE_ALPHA_SIZE_EXT 0x805F
#define GL_TEXTURE_LUMINANCE_SIZE_EXT 0x8060
#define GL_TEXTURE_INTENSITY_SIZE_EXT 0x8061
#define GL_REPLACE_EXT 0x8062
#define GL_PROXY_TEXTURE_1D_EXT 0x8063
#define GL_PROXY_TEXTURE_2D_EXT 0x8064

#define GLEW_EXT_texture GLEW_GET_VAR(__GLEW_EXT_texture)

#endif /* GL_EXT_texture */

/* ---------------------------- GL_EXT_texture3D --------------------------- */

#ifndef GL_EXT_texture3D
#define GL_EXT_texture3D 1

#define GL_PACK_SKIP_IMAGES_EXT 0x806B
#define GL_PACK_IMAGE_HEIGHT_EXT 0x806C
#define GL_UNPACK_SKIP_IMAGES_EXT 0x806D
#define GL_UNPACK_IMAGE_HEIGHT_EXT 0x806E
#define GL_TEXTURE_3D_EXT 0x806F
#define GL_PROXY_TEXTURE_3D_EXT 0x8070
#define GL_TEXTURE_DEPTH_EXT 0x8071
#define GL_TEXTURE_WRAP_R_EXT 0x8072
#define GL_MAX_3D_TEXTURE_SIZE_EXT 0x8073

typedef void (GLAPIENTRY * PFNGLTEXIMAGE3DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);

#define glTexImage3DEXT GLEW_GET_FUN(__glewTexImage3DEXT)

#define GLEW_EXT_texture3D GLEW_GET_VAR(__GLEW_EXT_texture3D)

#endif /* GL_EXT_texture3D */

/* -------------------------- GL_EXT_texture_array ------------------------- */

#ifndef GL_EXT_texture_array
#define GL_EXT_texture_array 1

#define GL_COMPARE_REF_DEPTH_TO_TEXTURE_EXT 0x884E
#define GL_MAX_ARRAY_TEXTURE_LAYERS_EXT 0x88FF
#define GL_TEXTURE_1D_ARRAY_EXT 0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY_EXT 0x8C19
#define GL_TEXTURE_2D_ARRAY_EXT 0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY_EXT 0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY_EXT 0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY_EXT 0x8C1D

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

#define glFramebufferTextureLayerEXT GLEW_GET_FUN(__glewFramebufferTextureLayerEXT)

#define GLEW_EXT_texture_array GLEW_GET_VAR(__GLEW_EXT_texture_array)

#endif /* GL_EXT_texture_array */

/* ---------------------- GL_EXT_texture_buffer_object --------------------- */

#ifndef GL_EXT_texture_buffer_object
#define GL_EXT_texture_buffer_object 1

#define GL_TEXTURE_BUFFER_EXT 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE_EXT 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER_EXT 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING_EXT 0x8C2D
#define GL_TEXTURE_BUFFER_FORMAT_EXT 0x8C2E

typedef void (GLAPIENTRY * PFNGLTEXBUFFEREXTPROC) (GLenum target, GLenum internalformat, GLuint buffer);

#define glTexBufferEXT GLEW_GET_FUN(__glewTexBufferEXT)

#define GLEW_EXT_texture_buffer_object GLEW_GET_VAR(__GLEW_EXT_texture_buffer_object)

#endif /* GL_EXT_texture_buffer_object */

/* -------------- GL_EXT_texture_compression_astc_decode_mode -------------- */

#ifndef GL_EXT_texture_compression_astc_decode_mode
#define GL_EXT_texture_compression_astc_decode_mode 1

#define GL_TEXTURE_ASTC_DECODE_PRECISION_EXT 0x8F69

#define GLEW_EXT_texture_compression_astc_decode_mode GLEW_GET_VAR(__GLEW_EXT_texture_compression_astc_decode_mode)

#endif /* GL_EXT_texture_compression_astc_decode_mode */

/* ----------- GL_EXT_texture_compression_astc_decode_mode_rgb9e5 ---------- */

#ifndef GL_EXT_texture_compression_astc_decode_mode_rgb9e5
#define GL_EXT_texture_compression_astc_decode_mode_rgb9e5 1

#define GL_TEXTURE_ASTC_DECODE_PRECISION_EXT 0x8F69

#define GLEW_EXT_texture_compression_astc_decode_mode_rgb9e5 GLEW_GET_VAR(__GLEW_EXT_texture_compression_astc_decode_mode_rgb9e5)

#endif /* GL_EXT_texture_compression_astc_decode_mode_rgb9e5 */

/* -------------------- GL_EXT_texture_compression_bptc -------------------- */

#ifndef GL_EXT_texture_compression_bptc
#define GL_EXT_texture_compression_bptc 1

#define GL_COMPRESSED_RGBA_BPTC_UNORM_EXT 0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_EXT 0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_EXT 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_EXT 0x8E8F

#define GLEW_EXT_texture_compression_bptc GLEW_GET_VAR(__GLEW_EXT_texture_compression_bptc)

#endif /* GL_EXT_texture_compression_bptc */

/* -------------------- GL_EXT_texture_compression_dxt1 -------------------- */

#ifndef GL_EXT_texture_compression_dxt1
#define GL_EXT_texture_compression_dxt1 1

#define GLEW_EXT_texture_compression_dxt1 GLEW_GET_VAR(__GLEW_EXT_texture_compression_dxt1)

#endif /* GL_EXT_texture_compression_dxt1 */

/* -------------------- GL_EXT_texture_compression_latc -------------------- */

#ifndef GL_EXT_texture_compression_latc
#define GL_EXT_texture_compression_latc 1

#define GL_COMPRESSED_LUMINANCE_LATC1_EXT 0x8C70
#define GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT 0x8C71
#define GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT 0x8C72
#define GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT 0x8C73

#define GLEW_EXT_texture_compression_latc GLEW_GET_VAR(__GLEW_EXT_texture_compression_latc)

#endif /* GL_EXT_texture_compression_latc */

/* -------------------- GL_EXT_texture_compression_rgtc -------------------- */

#ifndef GL_EXT_texture_compression_rgtc
#define GL_EXT_texture_compression_rgtc 1

#define GL_COMPRESSED_RED_RGTC1_EXT 0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1_EXT 0x8DBC
#define GL_COMPRESSED_RED_GREEN_RGTC2_EXT 0x8DBD
#define GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT 0x8DBE

#define GLEW_EXT_texture_compression_rgtc GLEW_GET_VAR(__GLEW_EXT_texture_compression_rgtc)

#endif /* GL_EXT_texture_compression_rgtc */

/* -------------------- GL_EXT_texture_compression_s3tc -------------------- */

#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc 1

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

#define GLEW_EXT_texture_compression_s3tc GLEW_GET_VAR(__GLEW_EXT_texture_compression_s3tc)

#endif /* GL_EXT_texture_compression_s3tc */

/* ------------------------ GL_EXT_texture_cube_map ------------------------ */

#ifndef GL_EXT_texture_cube_map
#define GL_EXT_texture_cube_map 1

#define GL_NORMAL_MAP_EXT 0x8511
#define GL_REFLECTION_MAP_EXT 0x8512
#define GL_TEXTURE_CUBE_MAP_EXT 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_EXT 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_EXT 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT 0x851C

#define GLEW_EXT_texture_cube_map GLEW_GET_VAR(__GLEW_EXT_texture_cube_map)

#endif /* GL_EXT_texture_cube_map */

/* --------------------- GL_EXT_texture_cube_map_array --------------------- */

#ifndef GL_EXT_texture_cube_map_array
#define GL_EXT_texture_cube_map_array 1

#define GL_TEXTURE_CUBE_MAP_ARRAY_EXT 0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY_EXT 0x900A
#define GL_SAMPLER_CUBE_MAP_ARRAY_EXT 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_EXT 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY_EXT 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_EXT 0x900F
#define GL_IMAGE_CUBE_MAP_ARRAY_EXT 0x9054
#define GL_INT_IMAGE_CUBE_MAP_ARRAY_EXT 0x905F
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY_EXT 0x906A

#define GLEW_EXT_texture_cube_map_array GLEW_GET_VAR(__GLEW_EXT_texture_cube_map_array)

#endif /* GL_EXT_texture_cube_map_array */

/* ----------------------- GL_EXT_texture_edge_clamp ----------------------- */

#ifndef GL_EXT_texture_edge_clamp
#define GL_EXT_texture_edge_clamp 1

#define GL_CLAMP_TO_EDGE_EXT 0x812F

#define GLEW_EXT_texture_edge_clamp GLEW_GET_VAR(__GLEW_EXT_texture_edge_clamp)

#endif /* GL_EXT_texture_edge_clamp */

/* --------------------------- GL_EXT_texture_env -------------------------- */

#ifndef GL_EXT_texture_env
#define GL_EXT_texture_env 1

#define GLEW_EXT_texture_env GLEW_GET_VAR(__GLEW_EXT_texture_env)

#endif /* GL_EXT_texture_env */

/* ------------------------- GL_EXT_texture_env_add ------------------------ */

#ifndef GL_EXT_texture_env_add
#define GL_EXT_texture_env_add 1

#define GLEW_EXT_texture_env_add GLEW_GET_VAR(__GLEW_EXT_texture_env_add)

#endif /* GL_EXT_texture_env_add */

/* ----------------------- GL_EXT_texture_env_combine ---------------------- */

#ifndef GL_EXT_texture_env_combine
#define GL_EXT_texture_env_combine 1

#define GL_COMBINE_EXT 0x8570
#define GL_COMBINE_RGB_EXT 0x8571
#define GL_COMBINE_ALPHA_EXT 0x8572
#define GL_RGB_SCALE_EXT 0x8573
#define GL_ADD_SIGNED_EXT 0x8574
#define GL_INTERPOLATE_EXT 0x8575
#define GL_CONSTANT_EXT 0x8576
#define GL_PRIMARY_COLOR_EXT 0x8577
#define GL_PREVIOUS_EXT 0x8578
#define GL_SOURCE0_RGB_EXT 0x8580
#define GL_SOURCE1_RGB_EXT 0x8581
#define GL_SOURCE2_RGB_EXT 0x8582
#define GL_SOURCE0_ALPHA_EXT 0x8588
#define GL_SOURCE1_ALPHA_EXT 0x8589
#define GL_SOURCE2_ALPHA_EXT 0x858A
#define GL_OPERAND0_RGB_EXT 0x8590
#define GL_OPERAND1_RGB_EXT 0x8591
#define GL_OPERAND2_RGB_EXT 0x8592
#define GL_OPERAND0_ALPHA_EXT 0x8598
#define GL_OPERAND1_ALPHA_EXT 0x8599
#define GL_OPERAND2_ALPHA_EXT 0x859A

#define GLEW_EXT_texture_env_combine GLEW_GET_VAR(__GLEW_EXT_texture_env_combine)

#endif /* GL_EXT_texture_env_combine */

/* ------------------------ GL_EXT_texture_env_dot3 ------------------------ */

#ifndef GL_EXT_texture_env_dot3
#define GL_EXT_texture_env_dot3 1

#define GL_DOT3_RGB_EXT 0x8740
#define GL_DOT3_RGBA_EXT 0x8741

#define GLEW_EXT_texture_env_dot3 GLEW_GET_VAR(__GLEW_EXT_texture_env_dot3)

#endif /* GL_EXT_texture_env_dot3 */

/* ------------------- GL_EXT_texture_filter_anisotropic ------------------- */

#ifndef GL_EXT_texture_filter_anisotropic
#define GL_EXT_texture_filter_anisotropic 1

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

#define GLEW_EXT_texture_filter_anisotropic GLEW_GET_VAR(__GLEW_EXT_texture_filter_anisotropic)

#endif /* GL_EXT_texture_filter_anisotropic */

/* ---------------------- GL_EXT_texture_filter_minmax --------------------- */

#ifndef GL_EXT_texture_filter_minmax
#define GL_EXT_texture_filter_minmax 1

#define GL_TEXTURE_REDUCTION_MODE_EXT 0x9366
#define GL_WEIGHTED_AVERAGE_EXT 0x9367

#define GLEW_EXT_texture_filter_minmax GLEW_GET_VAR(__GLEW_EXT_texture_filter_minmax)

#endif /* GL_EXT_texture_filter_minmax */

/* --------------------- GL_EXT_texture_format_BGRA8888 -------------------- */

#ifndef GL_EXT_texture_format_BGRA8888
#define GL_EXT_texture_format_BGRA8888 1

#define GL_BGRA_EXT 0x80E1

#define GLEW_EXT_texture_format_BGRA8888 GLEW_GET_VAR(__GLEW_EXT_texture_format_BGRA8888)

#endif /* GL_EXT_texture_format_BGRA8888 */

/* ------------------------- GL_EXT_texture_integer ------------------------ */

#ifndef GL_EXT_texture_integer
#define GL_EXT_texture_integer 1

#define GL_RGBA32UI_EXT 0x8D70
#define GL_RGB32UI_EXT 0x8D71
#define GL_ALPHA32UI_EXT 0x8D72
#define GL_INTENSITY32UI_EXT 0x8D73
#define GL_LUMINANCE32UI_EXT 0x8D74
#define GL_LUMINANCE_ALPHA32UI_EXT 0x8D75
#define GL_RGBA16UI_EXT 0x8D76
#define GL_RGB16UI_EXT 0x8D77
#define GL_ALPHA16UI_EXT 0x8D78
#define GL_INTENSITY16UI_EXT 0x8D79
#define GL_LUMINANCE16UI_EXT 0x8D7A
#define GL_LUMINANCE_ALPHA16UI_EXT 0x8D7B
#define GL_RGBA8UI_EXT 0x8D7C
#define GL_RGB8UI_EXT 0x8D7D
#define GL_ALPHA8UI_EXT 0x8D7E
#define GL_INTENSITY8UI_EXT 0x8D7F
#define GL_LUMINANCE8UI_EXT 0x8D80
#define GL_LUMINANCE_ALPHA8UI_EXT 0x8D81
#define GL_RGBA32I_EXT 0x8D82
#define GL_RGB32I_EXT 0x8D83
#define GL_ALPHA32I_EXT 0x8D84
#define GL_INTENSITY32I_EXT 0x8D85
#define GL_LUMINANCE32I_EXT 0x8D86
#define GL_LUMINANCE_ALPHA32I_EXT 0x8D87
#define GL_RGBA16I_EXT 0x8D88
#define GL_RGB16I_EXT 0x8D89
#define GL_ALPHA16I_EXT 0x8D8A
#define GL_INTENSITY16I_EXT 0x8D8B
#define GL_LUMINANCE16I_EXT 0x8D8C
#define GL_LUMINANCE_ALPHA16I_EXT 0x8D8D
#define GL_RGBA8I_EXT 0x8D8E
#define GL_RGB8I_EXT 0x8D8F
#define GL_ALPHA8I_EXT 0x8D90
#define GL_INTENSITY8I_EXT 0x8D91
#define GL_LUMINANCE8I_EXT 0x8D92
#define GL_LUMINANCE_ALPHA8I_EXT 0x8D93
#define GL_RED_INTEGER_EXT 0x8D94
#define GL_GREEN_INTEGER_EXT 0x8D95
#define GL_BLUE_INTEGER_EXT 0x8D96
#define GL_ALPHA_INTEGER_EXT 0x8D97
#define GL_RGB_INTEGER_EXT 0x8D98
#define GL_RGBA_INTEGER_EXT 0x8D99
#define GL_BGR_INTEGER_EXT 0x8D9A
#define GL_BGRA_INTEGER_EXT 0x8D9B
#define GL_LUMINANCE_INTEGER_EXT 0x8D9C
#define GL_LUMINANCE_ALPHA_INTEGER_EXT 0x8D9D
#define GL_RGBA_INTEGER_MODE_EXT 0x8D9E

typedef void (GLAPIENTRY * PFNGLCLEARCOLORIIEXTPROC) (GLint red, GLint green, GLint blue, GLint alpha);
typedef void (GLAPIENTRY * PFNGLCLEARCOLORIUIEXTPROC) (GLuint red, GLuint green, GLuint blue, GLuint alpha);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIIVEXTPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIUIVEXTPROC) (GLenum target, GLenum pname, GLuint *params);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIIVEXTPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIUIVEXTPROC) (GLenum target, GLenum pname, const GLuint *params);

#define glClearColorIiEXT GLEW_GET_FUN(__glewClearColorIiEXT)
#define glClearColorIuiEXT GLEW_GET_FUN(__glewClearColorIuiEXT)
#define glGetTexParameterIivEXT GLEW_GET_FUN(__glewGetTexParameterIivEXT)
#define glGetTexParameterIuivEXT GLEW_GET_FUN(__glewGetTexParameterIuivEXT)
#define glTexParameterIivEXT GLEW_GET_FUN(__glewTexParameterIivEXT)
#define glTexParameterIuivEXT GLEW_GET_FUN(__glewTexParameterIuivEXT)

#define GLEW_EXT_texture_integer GLEW_GET_VAR(__GLEW_EXT_texture_integer)

#endif /* GL_EXT_texture_integer */

/* ------------------------ GL_EXT_texture_lod_bias ------------------------ */

#ifndef GL_EXT_texture_lod_bias
#define GL_EXT_texture_lod_bias 1

#define GL_MAX_TEXTURE_LOD_BIAS_EXT 0x84FD
#define GL_TEXTURE_FILTER_CONTROL_EXT 0x8500
#define GL_TEXTURE_LOD_BIAS_EXT 0x8501

#define GLEW_EXT_texture_lod_bias GLEW_GET_VAR(__GLEW_EXT_texture_lod_bias)

#endif /* GL_EXT_texture_lod_bias */

/* ---------------------- GL_EXT_texture_mirror_clamp ---------------------- */

#ifndef GL_EXT_texture_mirror_clamp
#define GL_EXT_texture_mirror_clamp 1

#define GL_MIRROR_CLAMP_EXT 0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_EXT 0x8743
#define GL_MIRROR_CLAMP_TO_BORDER_EXT 0x8912

#define GLEW_EXT_texture_mirror_clamp GLEW_GET_VAR(__GLEW_EXT_texture_mirror_clamp)

#endif /* GL_EXT_texture_mirror_clamp */

/* ------------------------- GL_EXT_texture_norm16 ------------------------- */

#ifndef GL_EXT_texture_norm16
#define GL_EXT_texture_norm16 1

#define GL_RGB16_EXT 0x8054
#define GL_RGBA16_EXT 0x805B
#define GL_R16_EXT 0x822A
#define GL_RG16_EXT 0x822C
#define GL_R16_SNORM_EXT 0x8F98
#define GL_RG16_SNORM_EXT 0x8F99
#define GL_RGB16_SNORM_EXT 0x8F9A
#define GL_RGBA16_SNORM_EXT 0x8F9B

#define GLEW_EXT_texture_norm16 GLEW_GET_VAR(__GLEW_EXT_texture_norm16)

#endif /* GL_EXT_texture_norm16 */

/* ------------------------- GL_EXT_texture_object ------------------------- */

#ifndef GL_EXT_texture_object
#define GL_EXT_texture_object 1

#define GL_TEXTURE_PRIORITY_EXT 0x8066
#define GL_TEXTURE_RESIDENT_EXT 0x8067
#define GL_TEXTURE_1D_BINDING_EXT 0x8068
#define GL_TEXTURE_2D_BINDING_EXT 0x8069
#define GL_TEXTURE_3D_BINDING_EXT 0x806A

typedef GLboolean (GLAPIENTRY * PFNGLARETEXTURESRESIDENTEXTPROC) (GLsizei n, const GLuint* textures, GLboolean* residences);
typedef void (GLAPIENTRY * PFNGLBINDTEXTUREEXTPROC) (GLenum target, GLuint texture);
typedef void (GLAPIENTRY * PFNGLDELETETEXTURESEXTPROC) (GLsizei n, const GLuint* textures);
typedef void (GLAPIENTRY * PFNGLGENTEXTURESEXTPROC) (GLsizei n, GLuint* textures);
typedef GLboolean (GLAPIENTRY * PFNGLISTEXTUREEXTPROC) (GLuint texture);
typedef void (GLAPIENTRY * PFNGLPRIORITIZETEXTURESEXTPROC) (GLsizei n, const GLuint* textures, const GLclampf* priorities);

#define glAreTexturesResidentEXT GLEW_GET_FUN(__glewAreTexturesResidentEXT)
#define glBindTextureEXT GLEW_GET_FUN(__glewBindTextureEXT)
#define glDeleteTexturesEXT GLEW_GET_FUN(__glewDeleteTexturesEXT)
#define glGenTexturesEXT GLEW_GET_FUN(__glewGenTexturesEXT)
#define glIsTextureEXT GLEW_GET_FUN(__glewIsTextureEXT)
#define glPrioritizeTexturesEXT GLEW_GET_FUN(__glewPrioritizeTexturesEXT)

#define GLEW_EXT_texture_object GLEW_GET_VAR(__GLEW_EXT_texture_object)

#endif /* GL_EXT_texture_object */

/* --------------------- GL_EXT_texture_perturb_normal --------------------- */

#ifndef GL_EXT_texture_perturb_normal
#define GL_EXT_texture_perturb_normal 1

#define GL_PERTURB_EXT 0x85AE
#define GL_TEXTURE_NORMAL_EXT 0x85AF

typedef void (GLAPIENTRY * PFNGLTEXTURENORMALEXTPROC) (GLenum mode);

#define glTextureNormalEXT GLEW_GET_FUN(__glewTextureNormalEXT)

#define GLEW_EXT_texture_perturb_normal GLEW_GET_VAR(__GLEW_EXT_texture_perturb_normal)

#endif /* GL_EXT_texture_perturb_normal */

/* ------------------------ GL_EXT_texture_rectangle ----------------------- */

#ifndef GL_EXT_texture_rectangle
#define GL_EXT_texture_rectangle 1

#define GL_TEXTURE_RECTANGLE_EXT 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_EXT 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_EXT 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_EXT 0x84F8

#define GLEW_EXT_texture_rectangle GLEW_GET_VAR(__GLEW_EXT_texture_rectangle)

#endif /* GL_EXT_texture_rectangle */

/* --------------------------- GL_EXT_texture_rg --------------------------- */

#ifndef GL_EXT_texture_rg
#define GL_EXT_texture_rg 1

#define GL_RED_EXT 0x1903
#define GL_RG_EXT 0x8227
#define GL_R8_EXT 0x8229
#define GL_RG8_EXT 0x822B

#define GLEW_EXT_texture_rg GLEW_GET_VAR(__GLEW_EXT_texture_rg)

#endif /* GL_EXT_texture_rg */

/* -------------------------- GL_EXT_texture_sRGB -------------------------- */

#ifndef GL_EXT_texture_sRGB
#define GL_EXT_texture_sRGB 1

#define GL_SRGB_EXT 0x8C40
#define GL_SRGB8_EXT 0x8C41
#define GL_SRGB_ALPHA_EXT 0x8C42
#define GL_SRGB8_ALPHA8_EXT 0x8C43
#define GL_SLUMINANCE_ALPHA_EXT 0x8C44
#define GL_SLUMINANCE8_ALPHA8_EXT 0x8C45
#define GL_SLUMINANCE_EXT 0x8C46
#define GL_SLUMINANCE8_EXT 0x8C47
#define GL_COMPRESSED_SRGB_EXT 0x8C48
#define GL_COMPRESSED_SRGB_ALPHA_EXT 0x8C49
#define GL_COMPRESSED_SLUMINANCE_EXT 0x8C4A
#define GL_COMPRESSED_SLUMINANCE_ALPHA_EXT 0x8C4B
#define GL_COMPRESSED_SRGB_S3TC_DXT1_EXT 0x8C4C
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT 0x8C4D
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT 0x8C4E
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT 0x8C4F

#define GLEW_EXT_texture_sRGB GLEW_GET_VAR(__GLEW_EXT_texture_sRGB)

#endif /* GL_EXT_texture_sRGB */

/* ------------------------- GL_EXT_texture_sRGB_R8 ------------------------ */

#ifndef GL_EXT_texture_sRGB_R8
#define GL_EXT_texture_sRGB_R8 1

#define GL_SR8_EXT 0x8FBD

#define GLEW_EXT_texture_sRGB_R8 GLEW_GET_VAR(__GLEW_EXT_texture_sRGB_R8)

#endif /* GL_EXT_texture_sRGB_R8 */

/* ------------------------ GL_EXT_texture_sRGB_RG8 ------------------------ */

#ifndef GL_EXT_texture_sRGB_RG8
#define GL_EXT_texture_sRGB_RG8 1

#define GL_SRG8_EXT 0x8FBE

#define GLEW_EXT_texture_sRGB_RG8 GLEW_GET_VAR(__GLEW_EXT_texture_sRGB_RG8)

#endif /* GL_EXT_texture_sRGB_RG8 */

/* ----------------------- GL_EXT_texture_sRGB_decode ---------------------- */

#ifndef GL_EXT_texture_sRGB_decode
#define GL_EXT_texture_sRGB_decode 1

#define GL_TEXTURE_SRGB_DECODE_EXT 0x8A48
#define GL_DECODE_EXT 0x8A49
#define GL_SKIP_DECODE_EXT 0x8A4A

#define GLEW_EXT_texture_sRGB_decode GLEW_GET_VAR(__GLEW_EXT_texture_sRGB_decode)

#endif /* GL_EXT_texture_sRGB_decode */

/* --------------------- GL_EXT_texture_shared_exponent -------------------- */

#ifndef GL_EXT_texture_shared_exponent
#define GL_EXT_texture_shared_exponent 1

#define GL_RGB9_E5_EXT 0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV_EXT 0x8C3E
#define GL_TEXTURE_SHARED_SIZE_EXT 0x8C3F

#define GLEW_EXT_texture_shared_exponent GLEW_GET_VAR(__GLEW_EXT_texture_shared_exponent)

#endif /* GL_EXT_texture_shared_exponent */

/* -------------------------- GL_EXT_texture_snorm ------------------------- */

#ifndef GL_EXT_texture_snorm
#define GL_EXT_texture_snorm 1

#define GL_RED_SNORM 0x8F90
#define GL_RG_SNORM 0x8F91
#define GL_RGB_SNORM 0x8F92
#define GL_RGBA_SNORM 0x8F93
#define GL_R8_SNORM 0x8F94
#define GL_RG8_SNORM 0x8F95
#define GL_RGB8_SNORM 0x8F96
#define GL_RGBA8_SNORM 0x8F97
#define GL_R16_SNORM 0x8F98
#define GL_RG16_SNORM 0x8F99
#define GL_RGB16_SNORM 0x8F9A
#define GL_RGBA16_SNORM 0x8F9B
#define GL_SIGNED_NORMALIZED 0x8F9C
#define GL_ALPHA_SNORM 0x9010
#define GL_LUMINANCE_SNORM 0x9011
#define GL_LUMINANCE_ALPHA_SNORM 0x9012
#define GL_INTENSITY_SNORM 0x9013
#define GL_ALPHA8_SNORM 0x9014
#define GL_LUMINANCE8_SNORM 0x9015
#define GL_LUMINANCE8_ALPHA8_SNORM 0x9016
#define GL_INTENSITY8_SNORM 0x9017
#define GL_ALPHA16_SNORM 0x9018
#define GL_LUMINANCE16_SNORM 0x9019
#define GL_LUMINANCE16_ALPHA16_SNORM 0x901A
#define GL_INTENSITY16_SNORM 0x901B

#define GLEW_EXT_texture_snorm GLEW_GET_VAR(__GLEW_EXT_texture_snorm)

#endif /* GL_EXT_texture_snorm */

/* ------------------------- GL_EXT_texture_storage ------------------------ */

#ifndef GL_EXT_texture_storage
#define GL_EXT_texture_storage 1

#define GL_ALPHA8_EXT 0x803C
#define GL_LUMINANCE8_EXT 0x8040
#define GL_LUMINANCE8_ALPHA8_EXT 0x8045
#define GL_RGB10_EXT 0x8052
#define GL_RGB10_A2_EXT 0x8059
#define GL_R8_EXT 0x8229
#define GL_RG8_EXT 0x822B
#define GL_R16F_EXT 0x822D
#define GL_R32F_EXT 0x822E
#define GL_RG16F_EXT 0x822F
#define GL_RG32F_EXT 0x8230
#define GL_RGBA32F_EXT 0x8814
#define GL_RGB32F_EXT 0x8815
#define GL_ALPHA32F_EXT 0x8816
#define GL_LUMINANCE32F_EXT 0x8818
#define GL_LUMINANCE_ALPHA32F_EXT 0x8819
#define GL_RGBA16F_EXT 0x881A
#define GL_RGB16F_EXT 0x881B
#define GL_ALPHA16F_EXT 0x881C
#define GL_LUMINANCE16F_EXT 0x881E
#define GL_LUMINANCE_ALPHA16F_EXT 0x881F
#define GL_RGB_RAW_422_APPLE 0x8A51
#define GL_TEXTURE_IMMUTABLE_FORMAT_EXT 0x912F
#define GL_BGRA8_EXT 0x93A1

typedef void (GLAPIENTRY * PFNGLTEXSTORAGE1DEXTPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGE2DEXTPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGE3DEXTPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE1DEXTPROC) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE2DEXTPROC) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE3DEXTPROC) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

#define glTexStorage1DEXT GLEW_GET_FUN(__glewTexStorage1DEXT)
#define glTexStorage2DEXT GLEW_GET_FUN(__glewTexStorage2DEXT)
#define glTexStorage3DEXT GLEW_GET_FUN(__glewTexStorage3DEXT)
#define glTextureStorage1DEXT GLEW_GET_FUN(__glewTextureStorage1DEXT)
#define glTextureStorage2DEXT GLEW_GET_FUN(__glewTextureStorage2DEXT)
#define glTextureStorage3DEXT GLEW_GET_FUN(__glewTextureStorage3DEXT)

#define GLEW_EXT_texture_storage GLEW_GET_VAR(__GLEW_EXT_texture_storage)

#endif /* GL_EXT_texture_storage */

/* ------------------------- GL_EXT_texture_swizzle ------------------------ */

#ifndef GL_EXT_texture_swizzle
#define GL_EXT_texture_swizzle 1

#define GL_TEXTURE_SWIZZLE_R_EXT 0x8E42
#define GL_TEXTURE_SWIZZLE_G_EXT 0x8E43
#define GL_TEXTURE_SWIZZLE_B_EXT 0x8E44
#define GL_TEXTURE_SWIZZLE_A_EXT 0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA_EXT 0x8E46

#define GLEW_EXT_texture_swizzle GLEW_GET_VAR(__GLEW_EXT_texture_swizzle)

#endif /* GL_EXT_texture_swizzle */

/* ------------------- GL_EXT_texture_type_2_10_10_10_REV ------------------ */

#ifndef GL_EXT_texture_type_2_10_10_10_REV
#define GL_EXT_texture_type_2_10_10_10_REV 1

#define GL_UNSIGNED_INT_2_10_10_10_REV_EXT 0x8368

#define GLEW_EXT_texture_type_2_10_10_10_REV GLEW_GET_VAR(__GLEW_EXT_texture_type_2_10_10_10_REV)

#endif /* GL_EXT_texture_type_2_10_10_10_REV */

/* -------------------------- GL_EXT_texture_view -------------------------- */

#ifndef GL_EXT_texture_view
#define GL_EXT_texture_view 1

#define GL_TEXTURE_VIEW_MIN_LEVEL_EXT 0x82DB
#define GL_TEXTURE_VIEW_NUM_LEVELS_EXT 0x82DC
#define GL_TEXTURE_VIEW_MIN_LAYER_EXT 0x82DD
#define GL_TEXTURE_VIEW_NUM_LAYERS_EXT 0x82DE
#define GL_TEXTURE_IMMUTABLE_LEVELS 0x82DF

typedef void (GLAPIENTRY * PFNGLTEXTUREVIEWEXTPROC) (GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);

#define glTextureViewEXT GLEW_GET_FUN(__glewTextureViewEXT)

#define GLEW_EXT_texture_view GLEW_GET_VAR(__GLEW_EXT_texture_view)

#endif /* GL_EXT_texture_view */

/* --------------------------- GL_EXT_timer_query -------------------------- */

#ifndef GL_EXT_timer_query
#define GL_EXT_timer_query 1

#define GL_TIME_ELAPSED_EXT 0x88BF

typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTI64VEXTPROC) (GLuint id, GLenum pname, GLint64EXT *params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUI64VEXTPROC) (GLuint id, GLenum pname, GLuint64EXT *params);

#define glGetQueryObjecti64vEXT GLEW_GET_FUN(__glewGetQueryObjecti64vEXT)
#define glGetQueryObjectui64vEXT GLEW_GET_FUN(__glewGetQueryObjectui64vEXT)

#define GLEW_EXT_timer_query GLEW_GET_VAR(__GLEW_EXT_timer_query)

#endif /* GL_EXT_timer_query */

/* ----------------------- GL_EXT_transform_feedback ----------------------- */

#ifndef GL_EXT_transform_feedback
#define GL_EXT_transform_feedback 1

#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH_EXT 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE_EXT 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_EXT 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS_EXT 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START_EXT 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_EXT 0x8C85
#define GL_PRIMITIVES_GENERATED_EXT 0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_EXT 0x8C88
#define GL_RASTERIZER_DISCARD_EXT 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_EXT 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_EXT 0x8C8B
#define GL_INTERLEAVED_ATTRIBS_EXT 0x8C8C
#define GL_SEPARATE_ATTRIBS_EXT 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER_EXT 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_EXT 0x8C8F

typedef void (GLAPIENTRY * PFNGLBEGINTRANSFORMFEEDBACKEXTPROC) (GLenum primitiveMode);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERBASEEXTPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBINDBUFFEROFFSETEXTPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERRANGEEXTPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLENDTRANSFORMFEEDBACKEXTPROC) (void);
typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKVARYINGEXTPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei *size, GLenum *type, GLchar *name);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKVARYINGSEXTPROC) (GLuint program, GLsizei count, const GLchar * const* varyings, GLenum bufferMode);

#define glBeginTransformFeedbackEXT GLEW_GET_FUN(__glewBeginTransformFeedbackEXT)
#define glBindBufferBaseEXT GLEW_GET_FUN(__glewBindBufferBaseEXT)
#define glBindBufferOffsetEXT GLEW_GET_FUN(__glewBindBufferOffsetEXT)
#define glBindBufferRangeEXT GLEW_GET_FUN(__glewBindBufferRangeEXT)
#define glEndTransformFeedbackEXT GLEW_GET_FUN(__glewEndTransformFeedbackEXT)
#define glGetTransformFeedbackVaryingEXT GLEW_GET_FUN(__glewGetTransformFeedbackVaryingEXT)
#define glTransformFeedbackVaryingsEXT GLEW_GET_FUN(__glewTransformFeedbackVaryingsEXT)

#define GLEW_EXT_transform_feedback GLEW_GET_VAR(__GLEW_EXT_transform_feedback)

#endif /* GL_EXT_transform_feedback */

/* ------------------------- GL_EXT_unpack_subimage ------------------------ */

#ifndef GL_EXT_unpack_subimage
#define GL_EXT_unpack_subimage 1

#define GL_UNPACK_ROW_LENGTH_EXT 0x0CF2
#define GL_UNPACK_SKIP_ROWS_EXT 0x0CF3
#define GL_UNPACK_SKIP_PIXELS_EXT 0x0CF4

#define GLEW_EXT_unpack_subimage GLEW_GET_VAR(__GLEW_EXT_unpack_subimage)

#endif /* GL_EXT_unpack_subimage */

/* -------------------------- GL_EXT_vertex_array -------------------------- */

#ifndef GL_EXT_vertex_array
#define GL_EXT_vertex_array 1

#define GL_DOUBLE_EXT 0x140A
#define GL_VERTEX_ARRAY_EXT 0x8074
#define GL_NORMAL_ARRAY_EXT 0x8075
#define GL_COLOR_ARRAY_EXT 0x8076
#define GL_INDEX_ARRAY_EXT 0x8077
#define GL_TEXTURE_COORD_ARRAY_EXT 0x8078
#define GL_EDGE_FLAG_ARRAY_EXT 0x8079
#define GL_VERTEX_ARRAY_SIZE_EXT 0x807A
#define GL_VERTEX_ARRAY_TYPE_EXT 0x807B
#define GL_VERTEX_ARRAY_STRIDE_EXT 0x807C
#define GL_VERTEX_ARRAY_COUNT_EXT 0x807D
#define GL_NORMAL_ARRAY_TYPE_EXT 0x807E
#define GL_NORMAL_ARRAY_STRIDE_EXT 0x807F
#define GL_NORMAL_ARRAY_COUNT_EXT 0x8080
#define GL_COLOR_ARRAY_SIZE_EXT 0x8081
#define GL_COLOR_ARRAY_TYPE_EXT 0x8082
#define GL_COLOR_ARRAY_STRIDE_EXT 0x8083
#define GL_COLOR_ARRAY_COUNT_EXT 0x8084
#define GL_INDEX_ARRAY_TYPE_EXT 0x8085
#define GL_INDEX_ARRAY_STRIDE_EXT 0x8086
#define GL_INDEX_ARRAY_COUNT_EXT 0x8087
#define GL_TEXTURE_COORD_ARRAY_SIZE_EXT 0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE_EXT 0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE_EXT 0x808A
#define GL_TEXTURE_COORD_ARRAY_COUNT_EXT 0x808B
#define GL_EDGE_FLAG_ARRAY_STRIDE_EXT 0x808C
#define GL_EDGE_FLAG_ARRAY_COUNT_EXT 0x808D
#define GL_VERTEX_ARRAY_POINTER_EXT 0x808E
#define GL_NORMAL_ARRAY_POINTER_EXT 0x808F
#define GL_COLOR_ARRAY_POINTER_EXT 0x8090
#define GL_INDEX_ARRAY_POINTER_EXT 0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER_EXT 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER_EXT 0x8093

typedef void (GLAPIENTRY * PFNGLARRAYELEMENTEXTPROC) (GLint i);
typedef void (GLAPIENTRY * PFNGLCOLORPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void *pointer);
typedef void (GLAPIENTRY * PFNGLDRAWARRAYSEXTPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (GLAPIENTRY * PFNGLEDGEFLAGPOINTEREXTPROC) (GLsizei stride, GLsizei count, const GLboolean* pointer);
typedef void (GLAPIENTRY * PFNGLINDEXPOINTEREXTPROC) (GLenum type, GLsizei stride, GLsizei count, const void *pointer);
typedef void (GLAPIENTRY * PFNGLNORMALPOINTEREXTPROC) (GLenum type, GLsizei stride, GLsizei count, const void *pointer);
typedef void (GLAPIENTRY * PFNGLTEXCOORDPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void *pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void *pointer);

#define glArrayElementEXT GLEW_GET_FUN(__glewArrayElementEXT)
#define glColorPointerEXT GLEW_GET_FUN(__glewColorPointerEXT)
#define glDrawArraysEXT GLEW_GET_FUN(__glewDrawArraysEXT)
#define glEdgeFlagPointerEXT GLEW_GET_FUN(__glewEdgeFlagPointerEXT)
#define glIndexPointerEXT GLEW_GET_FUN(__glewIndexPointerEXT)
#define glNormalPointerEXT GLEW_GET_FUN(__glewNormalPointerEXT)
#define glTexCoordPointerEXT GLEW_GET_FUN(__glewTexCoordPointerEXT)
#define glVertexPointerEXT GLEW_GET_FUN(__glewVertexPointerEXT)

#define GLEW_EXT_vertex_array GLEW_GET_VAR(__GLEW_EXT_vertex_array)

#endif /* GL_EXT_vertex_array */

/* ------------------------ GL_EXT_vertex_array_bgra ----------------------- */

#ifndef GL_EXT_vertex_array_bgra
#define GL_EXT_vertex_array_bgra 1

#define GL_BGRA 0x80E1

#define GLEW_EXT_vertex_array_bgra GLEW_GET_VAR(__GLEW_EXT_vertex_array_bgra)

#endif /* GL_EXT_vertex_array_bgra */

/* ----------------------- GL_EXT_vertex_array_setXXX ---------------------- */

#ifndef GL_EXT_vertex_array_setXXX
#define GL_EXT_vertex_array_setXXX 1

typedef void (GLAPIENTRY * PFNGLBINDARRAYSETEXTPROC) (const void *arrayset);
typedef const void * (GLAPIENTRY * PFNGLCREATEARRAYSETEXTPROC) (void);
typedef void (GLAPIENTRY * PFNGLDELETEARRAYSETSEXTPROC) (GLsizei n, const void *arrayset[]);

#define glBindArraySetEXT GLEW_GET_FUN(__glewBindArraySetEXT)
#define glCreateArraySetExt GLEW_GET_FUN(__glewCreateArraySetExt)
#define glDeleteArraySetsEXT GLEW_GET_FUN(__glewDeleteArraySetsEXT)

#define GLEW_EXT_vertex_array_setXXX GLEW_GET_VAR(__GLEW_EXT_vertex_array_setXXX)

#endif /* GL_EXT_vertex_array_setXXX */

/* ----------------------- GL_EXT_vertex_attrib_64bit ---------------------- */

#ifndef GL_EXT_vertex_attrib_64bit
#define GL_EXT_vertex_attrib_64bit 1

#define GL_DOUBLE_MAT2_EXT 0x8F46
#define GL_DOUBLE_MAT3_EXT 0x8F47
#define GL_DOUBLE_MAT4_EXT 0x8F48
#define GL_DOUBLE_MAT2x3_EXT 0x8F49
#define GL_DOUBLE_MAT2x4_EXT 0x8F4A
#define GL_DOUBLE_MAT3x2_EXT 0x8F4B
#define GL_DOUBLE_MAT3x4_EXT 0x8F4C
#define GL_DOUBLE_MAT4x2_EXT 0x8F4D
#define GL_DOUBLE_MAT4x3_EXT 0x8F4E
#define GL_DOUBLE_VEC2_EXT 0x8FFC
#define GL_DOUBLE_VEC3_EXT 0x8FFD
#define GL_DOUBLE_VEC4_EXT 0x8FFE

typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBLDVEXTPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBLOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1DEXTPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2DEXTPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3DEXTPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4DEXTPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBLPOINTEREXTPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);

#define glGetVertexAttribLdvEXT GLEW_GET_FUN(__glewGetVertexAttribLdvEXT)
#define glVertexArrayVertexAttribLOffsetEXT GLEW_GET_FUN(__glewVertexArrayVertexAttribLOffsetEXT)
#define glVertexAttribL1dEXT GLEW_GET_FUN(__glewVertexAttribL1dEXT)
#define glVertexAttribL1dvEXT GLEW_GET_FUN(__glewVertexAttribL1dvEXT)
#define glVertexAttribL2dEXT GLEW_GET_FUN(__glewVertexAttribL2dEXT)
#define glVertexAttribL2dvEXT GLEW_GET_FUN(__glewVertexAttribL2dvEXT)
#define glVertexAttribL3dEXT GLEW_GET_FUN(__glewVertexAttribL3dEXT)
#define glVertexAttribL3dvEXT GLEW_GET_FUN(__glewVertexAttribL3dvEXT)
#define glVertexAttribL4dEXT GLEW_GET_FUN(__glewVertexAttribL4dEXT)
#define glVertexAttribL4dvEXT GLEW_GET_FUN(__glewVertexAttribL4dvEXT)
#define glVertexAttribLPointerEXT GLEW_GET_FUN(__glewVertexAttribLPointerEXT)

#define GLEW_EXT_vertex_attrib_64bit GLEW_GET_VAR(__GLEW_EXT_vertex_attrib_64bit)

#endif /* GL_EXT_vertex_attrib_64bit */

/* -------------------------- GL_EXT_vertex_shader ------------------------- */

#ifndef GL_EXT_vertex_shader
#define GL_EXT_vertex_shader 1

#define GL_VERTEX_SHADER_EXT 0x8780
#define GL_VERTEX_SHADER_BINDING_EXT 0x8781
#define GL_OP_INDEX_EXT 0x8782
#define GL_OP_NEGATE_EXT 0x8783
#define GL_OP_DOT3_EXT 0x8784
#define GL_OP_DOT4_EXT 0x8785
#define GL_OP_MUL_EXT 0x8786
#define GL_OP_ADD_EXT 0x8787
#define GL_OP_MADD_EXT 0x8788
#define GL_OP_FRAC_EXT 0x8789
#define GL_OP_MAX_EXT 0x878A
#define GL_OP_MIN_EXT 0x878B
#define GL_OP_SET_GE_EXT 0x878C
#define GL_OP_SET_LT_EXT 0x878D
#define GL_OP_CLAMP_EXT 0x878E
#define GL_OP_FLOOR_EXT 0x878F
#define GL_OP_ROUND_EXT 0x8790
#define GL_OP_EXP_BASE_2_EXT 0x8791
#define GL_OP_LOG_BASE_2_EXT 0x8792
#define GL_OP_POWER_EXT 0x8793
#define GL_OP_RECIP_EXT 0x8794
#define GL_OP_RECIP_SQRT_EXT 0x8795
#define GL_OP_SUB_EXT 0x8796
#define GL_OP_CROSS_PRODUCT_EXT 0x8797
#define GL_OP_MULTIPLY_MATRIX_EXT 0x8798
#define GL_OP_MOV_EXT 0x8799
#define GL_OUTPUT_VERTEX_EXT 0x879A
#define GL_OUTPUT_COLOR0_EXT 0x879B
#define GL_OUTPUT_COLOR1_EXT 0x879C
#define GL_OUTPUT_TEXTURE_COORD0_EXT 0x879D
#define GL_OUTPUT_TEXTURE_COORD1_EXT 0x879E
#define GL_OUTPUT_TEXTURE_COORD2_EXT 0x879F
#define GL_OUTPUT_TEXTURE_COORD3_EXT 0x87A0
#define GL_OUTPUT_TEXTURE_COORD4_EXT 0x87A1
#define GL_OUTPUT_TEXTURE_COORD5_EXT 0x87A2
#define GL_OUTPUT_TEXTURE_COORD6_EXT 0x87A3
#define GL_OUTPUT_TEXTURE_COORD7_EXT 0x87A4
#define GL_OUTPUT_TEXTURE_COORD8_EXT 0x87A5
#define GL_OUTPUT_TEXTURE_COORD9_EXT 0x87A6
#define GL_OUTPUT_TEXTURE_COORD10_EXT 0x87A7
#define GL_OUTPUT_TEXTURE_COORD11_EXT 0x87A8
#define GL_OUTPUT_TEXTURE_COORD12_EXT 0x87A9
#define GL_OUTPUT_TEXTURE_COORD13_EXT 0x87AA
#define GL_OUTPUT_TEXTURE_COORD14_EXT 0x87AB
#define GL_OUTPUT_TEXTURE_COORD15_EXT 0x87AC
#define GL_OUTPUT_TEXTURE_COORD16_EXT 0x87AD
#define GL_OUTPUT_TEXTURE_COORD17_EXT 0x87AE
#define GL_OUTPUT_TEXTURE_COORD18_EXT 0x87AF
#define GL_OUTPUT_TEXTURE_COORD19_EXT 0x87B0
#define GL_OUTPUT_TEXTURE_COORD20_EXT 0x87B1
#define GL_OUTPUT_TEXTURE_COORD21_EXT 0x87B2
#define GL_OUTPUT_TEXTURE_COORD22_EXT 0x87B3
#define GL_OUTPUT_TEXTURE_COORD23_EXT 0x87B4
#define GL_OUTPUT_TEXTURE_COORD24_EXT 0x87B5
#define GL_OUTPUT_TEXTURE_COORD25_EXT 0x87B6
#define GL_OUTPUT_TEXTURE_COORD26_EXT 0x87B7
#define GL_OUTPUT_TEXTURE_COORD27_EXT 0x87B8
#define GL_OUTPUT_TEXTURE_COORD28_EXT 0x87B9
#define GL_OUTPUT_TEXTURE_COORD29_EXT 0x87BA
#define GL_OUTPUT_TEXTURE_COORD30_EXT 0x87BB
#define GL_OUTPUT_TEXTURE_COORD31_EXT 0x87BC
#define GL_OUTPUT_FOG_EXT 0x87BD
#define GL_SCALAR_EXT 0x87BE
#define GL_VECTOR_EXT 0x87BF
#define GL_MATRIX_EXT 0x87C0
#define GL_VARIANT_EXT 0x87C1
#define GL_INVARIANT_EXT 0x87C2
#define GL_LOCAL_CONSTANT_EXT 0x87C3
#define GL_LOCAL_EXT 0x87C4
#define GL_MAX_VERTEX_SHADER_INSTRUCTIONS_EXT 0x87C5
#define GL_MAX_VERTEX_SHADER_VARIANTS_EXT 0x87C6
#define GL_MAX_VERTEX_SHADER_INVARIANTS_EXT 0x87C7
#define GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT 0x87C8
#define GL_MAX_VERTEX_SHADER_LOCALS_EXT 0x87C9
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT 0x87CA
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_VARIANTS_EXT 0x87CB
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT 0x87CC
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCAL_CONSTANTS_EXT 0x87CD
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCALS_EXT 0x87CE
#define GL_VERTEX_SHADER_INSTRUCTIONS_EXT 0x87CF
#define GL_VERTEX_SHADER_VARIANTS_EXT 0x87D0
#define GL_VERTEX_SHADER_INVARIANTS_EXT 0x87D1
#define GL_VERTEX_SHADER_LOCAL_CONSTANTS_EXT 0x87D2
#define GL_VERTEX_SHADER_LOCALS_EXT 0x87D3
#define GL_VERTEX_SHADER_OPTIMIZED_EXT 0x87D4
#define GL_X_EXT 0x87D5
#define GL_Y_EXT 0x87D6
#define GL_Z_EXT 0x87D7
#define GL_W_EXT 0x87D8
#define GL_NEGATIVE_X_EXT 0x87D9
#define GL_NEGATIVE_Y_EXT 0x87DA
#define GL_NEGATIVE_Z_EXT 0x87DB
#define GL_NEGATIVE_W_EXT 0x87DC
#define GL_ZERO_EXT 0x87DD
#define GL_ONE_EXT 0x87DE
#define GL_NEGATIVE_ONE_EXT 0x87DF
#define GL_NORMALIZED_RANGE_EXT 0x87E0
#define GL_FULL_RANGE_EXT 0x87E1
#define GL_CURRENT_VERTEX_EXT 0x87E2
#define GL_MVP_MATRIX_EXT 0x87E3
#define GL_VARIANT_VALUE_EXT 0x87E4
#define GL_VARIANT_DATATYPE_EXT 0x87E5
#define GL_VARIANT_ARRAY_STRIDE_EXT 0x87E6
#define GL_VARIANT_ARRAY_TYPE_EXT 0x87E7
#define GL_VARIANT_ARRAY_EXT 0x87E8
#define GL_VARIANT_ARRAY_POINTER_EXT 0x87E9
#define GL_INVARIANT_VALUE_EXT 0x87EA
#define GL_INVARIANT_DATATYPE_EXT 0x87EB
#define GL_LOCAL_CONSTANT_VALUE_EXT 0x87EC
#define GL_LOCAL_CONSTANT_DATATYPE_EXT 0x87ED

typedef void (GLAPIENTRY * PFNGLBEGINVERTEXSHADEREXTPROC) (void);
typedef GLuint (GLAPIENTRY * PFNGLBINDLIGHTPARAMETEREXTPROC) (GLenum light, GLenum value);
typedef GLuint (GLAPIENTRY * PFNGLBINDMATERIALPARAMETEREXTPROC) (GLenum face, GLenum value);
typedef GLuint (GLAPIENTRY * PFNGLBINDPARAMETEREXTPROC) (GLenum value);
typedef GLuint (GLAPIENTRY * PFNGLBINDTEXGENPARAMETEREXTPROC) (GLenum unit, GLenum coord, GLenum value);
typedef GLuint (GLAPIENTRY * PFNGLBINDTEXTUREUNITPARAMETEREXTPROC) (GLenum unit, GLenum value);
typedef void (GLAPIENTRY * PFNGLBINDVERTEXSHADEREXTPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETEVERTEXSHADEREXTPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLENABLEVARIANTCLIENTSTATEEXTPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLENDVERTEXSHADEREXTPROC) (void);
typedef void (GLAPIENTRY * PFNGLEXTRACTCOMPONENTEXTPROC) (GLuint res, GLuint src, GLuint num);
typedef GLuint (GLAPIENTRY * PFNGLGENSYMBOLSEXTPROC) (GLenum dataType, GLenum storageType, GLenum range, GLuint components);
typedef GLuint (GLAPIENTRY * PFNGLGENVERTEXSHADERSEXTPROC) (GLuint range);
typedef void (GLAPIENTRY * PFNGLGETINVARIANTBOOLEANVEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void (GLAPIENTRY * PFNGLGETINVARIANTFLOATVEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void (GLAPIENTRY * PFNGLGETINVARIANTINTEGERVEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void (GLAPIENTRY * PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void (GLAPIENTRY * PFNGLGETLOCALCONSTANTFLOATVEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void (GLAPIENTRY * PFNGLGETLOCALCONSTANTINTEGERVEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void (GLAPIENTRY * PFNGLGETVARIANTBOOLEANVEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void (GLAPIENTRY * PFNGLGETVARIANTFLOATVEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void (GLAPIENTRY * PFNGLGETVARIANTINTEGERVEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void (GLAPIENTRY * PFNGLGETVARIANTPOINTERVEXTPROC) (GLuint id, GLenum value, void **data);
typedef void (GLAPIENTRY * PFNGLINSERTCOMPONENTEXTPROC) (GLuint res, GLuint src, GLuint num);
typedef GLboolean (GLAPIENTRY * PFNGLISVARIANTENABLEDEXTPROC) (GLuint id, GLenum cap);
typedef void (GLAPIENTRY * PFNGLSETINVARIANTEXTPROC) (GLuint id, GLenum type, void *addr);
typedef void (GLAPIENTRY * PFNGLSETLOCALCONSTANTEXTPROC) (GLuint id, GLenum type, void *addr);
typedef void (GLAPIENTRY * PFNGLSHADEROP1EXTPROC) (GLenum op, GLuint res, GLuint arg1);
typedef void (GLAPIENTRY * PFNGLSHADEROP2EXTPROC) (GLenum op, GLuint res, GLuint arg1, GLuint arg2);
typedef void (GLAPIENTRY * PFNGLSHADEROP3EXTPROC) (GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3);
typedef void (GLAPIENTRY * PFNGLSWIZZLEEXTPROC) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
typedef void (GLAPIENTRY * PFNGLVARIANTPOINTEREXTPROC) (GLuint id, GLenum type, GLuint stride, void *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTBVEXTPROC) (GLuint id, GLbyte *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTDVEXTPROC) (GLuint id, GLdouble *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTFVEXTPROC) (GLuint id, GLfloat *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTIVEXTPROC) (GLuint id, GLint *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTSVEXTPROC) (GLuint id, GLshort *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTUBVEXTPROC) (GLuint id, GLubyte *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTUIVEXTPROC) (GLuint id, GLuint *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTUSVEXTPROC) (GLuint id, GLushort *addr);
typedef void (GLAPIENTRY * PFNGLWRITEMASKEXTPROC) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);

#define glBeginVertexShaderEXT GLEW_GET_FUN(__glewBeginVertexShaderEXT)
#define glBindLightParameterEXT GLEW_GET_FUN(__glewBindLightParameterEXT)
#define glBindMaterialParameterEXT GLEW_GET_FUN(__glewBindMaterialParameterEXT)
#define glBindParameterEXT GLEW_GET_FUN(__glewBindParameterEXT)
#define glBindTexGenParameterEXT GLEW_GET_FUN(__glewBindTexGenParameterEXT)
#define glBindTextureUnitParameterEXT GLEW_GET_FUN(__glewBindTextureUnitParameterEXT)
#define glBindVertexShaderEXT GLEW_GET_FUN(__glewBindVertexShaderEXT)
#define glDeleteVertexShaderEXT GLEW_GET_FUN(__glewDeleteVertexShaderEXT)
#define glDisableVariantClientStateEXT GLEW_GET_FUN(__glewDisableVariantClientStateEXT)
#define glEnableVariantClientStateEXT GLEW_GET_FUN(__glewEnableVariantClientStateEXT)
#define glEndVertexShaderEXT GLEW_GET_FUN(__glewEndVertexShaderEXT)
#define glExtractComponentEXT GLEW_GET_FUN(__glewExtractComponentEXT)
#define glGenSymbolsEXT GLEW_GET_FUN(__glewGenSymbolsEXT)
#define glGenVertexShadersEXT GLEW_GET_FUN(__glewGenVertexShadersEXT)
#define glGetInvariantBooleanvEXT GLEW_GET_FUN(__glewGetInvariantBooleanvEXT)
#define glGetInvariantFloatvEXT GLEW_GET_FUN(__glewGetInvariantFloatvEXT)
#define glGetInvariantIntegervEXT GLEW_GET_FUN(__glewGetInvariantIntegervEXT)
#define glGetLocalConstantBooleanvEXT GLEW_GET_FUN(__glewGetLocalConstantBooleanvEXT)
#define glGetLocalConstantFloatvEXT GLEW_GET_FUN(__glewGetLocalConstantFloatvEXT)
#define glGetLocalConstantIntegervEXT GLEW_GET_FUN(__glewGetLocalConstantIntegervEXT)
#define glGetVariantBooleanvEXT GLEW_GET_FUN(__glewGetVariantBooleanvEXT)
#define glGetVariantFloatvEXT GLEW_GET_FUN(__glewGetVariantFloatvEXT)
#define glGetVariantIntegervEXT GLEW_GET_FUN(__glewGetVariantIntegervEXT)
#define glGetVariantPointervEXT GLEW_GET_FUN(__glewGetVariantPointervEXT)
#define glInsertComponentEXT GLEW_GET_FUN(__glewInsertComponentEXT)
#define glIsVariantEnabledEXT GLEW_GET_FUN(__glewIsVariantEnabledEXT)
#define glSetInvariantEXT GLEW_GET_FUN(__glewSetInvariantEXT)
#define glSetLocalConstantEXT GLEW_GET_FUN(__glewSetLocalConstantEXT)
#define glShaderOp1EXT GLEW_GET_FUN(__glewShaderOp1EXT)
#define glShaderOp2EXT GLEW_GET_FUN(__glewShaderOp2EXT)
#define glShaderOp3EXT GLEW_GET_FUN(__glewShaderOp3EXT)
#define glSwizzleEXT GLEW_GET_FUN(__glewSwizzleEXT)
#define glVariantPointerEXT GLEW_GET_FUN(__glewVariantPointerEXT)
#define glVariantbvEXT GLEW_GET_FUN(__glewVariantbvEXT)
#define glVariantdvEXT GLEW_GET_FUN(__glewVariantdvEXT)
#define glVariantfvEXT GLEW_GET_FUN(__glewVariantfvEXT)
#define glVariantivEXT GLEW_GET_FUN(__glewVariantivEXT)
#define glVariantsvEXT GLEW_GET_FUN(__glewVariantsvEXT)
#define glVariantubvEXT GLEW_GET_FUN(__glewVariantubvEXT)
#define glVariantuivEXT GLEW_GET_FUN(__glewVariantuivEXT)
#define glVariantusvEXT GLEW_GET_FUN(__glewVariantusvEXT)
#define glWriteMaskEXT GLEW_GET_FUN(__glewWriteMaskEXT)

#define GLEW_EXT_vertex_shader GLEW_GET_VAR(__GLEW_EXT_vertex_shader)

#endif /* GL_EXT_vertex_shader */

/* ------------------------ GL_EXT_vertex_weighting ------------------------ */

#ifndef GL_EXT_vertex_weighting
#define GL_EXT_vertex_weighting 1

#define GL_MODELVIEW0_STACK_DEPTH_EXT 0x0BA3
#define GL_MODELVIEW0_MATRIX_EXT 0x0BA6
#define GL_MODELVIEW0_EXT 0x1700
#define GL_MODELVIEW1_STACK_DEPTH_EXT 0x8502
#define GL_MODELVIEW1_MATRIX_EXT 0x8506
#define GL_VERTEX_WEIGHTING_EXT 0x8509
#define GL_MODELVIEW1_EXT 0x850A
#define GL_CURRENT_VERTEX_WEIGHT_EXT 0x850B
#define GL_VERTEX_WEIGHT_ARRAY_EXT 0x850C
#define GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT 0x850D
#define GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT 0x850E
#define GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT 0x850F
#define GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT 0x8510

typedef void (GLAPIENTRY * PFNGLVERTEXWEIGHTPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, void *pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXWEIGHTFEXTPROC) (GLfloat weight);
typedef void (GLAPIENTRY * PFNGLVERTEXWEIGHTFVEXTPROC) (GLfloat* weight);

#define glVertexWeightPointerEXT GLEW_GET_FUN(__glewVertexWeightPointerEXT)
#define glVertexWeightfEXT GLEW_GET_FUN(__glewVertexWeightfEXT)
#define glVertexWeightfvEXT GLEW_GET_FUN(__glewVertexWeightfvEXT)

#define GLEW_EXT_vertex_weighting GLEW_GET_VAR(__GLEW_EXT_vertex_weighting)

#endif /* GL_EXT_vertex_weighting */

/* ------------------------ GL_EXT_win32_keyed_mutex ----------------------- */

#ifndef GL_EXT_win32_keyed_mutex
#define GL_EXT_win32_keyed_mutex 1

typedef GLboolean (GLAPIENTRY * PFNGLACQUIREKEYEDMUTEXWIN32EXTPROC) (GLuint memory, GLuint64 key, GLuint timeout);
typedef GLboolean (GLAPIENTRY * PFNGLRELEASEKEYEDMUTEXWIN32EXTPROC) (GLuint memory, GLuint64 key);

#define glAcquireKeyedMutexWin32EXT GLEW_GET_FUN(__glewAcquireKeyedMutexWin32EXT)
#define glReleaseKeyedMutexWin32EXT GLEW_GET_FUN(__glewReleaseKeyedMutexWin32EXT)

#define GLEW_EXT_win32_keyed_mutex GLEW_GET_VAR(__GLEW_EXT_win32_keyed_mutex)

#endif /* GL_EXT_win32_keyed_mutex */

/* ------------------------ GL_EXT_window_rectangles ----------------------- */

#ifndef GL_EXT_window_rectangles
#define GL_EXT_window_rectangles 1

#define GL_INCLUSIVE_EXT 0x8F10
#define GL_EXCLUSIVE_EXT 0x8F11
#define GL_WINDOW_RECTANGLE_EXT 0x8F12
#define GL_WINDOW_RECTANGLE_MODE_EXT 0x8F13
#define GL_MAX_WINDOW_RECTANGLES_EXT 0x8F14
#define GL_NUM_WINDOW_RECTANGLES_EXT 0x8F15

typedef void (GLAPIENTRY * PFNGLWINDOWRECTANGLESEXTPROC) (GLenum mode, GLsizei count, const GLint box[]);

#define glWindowRectanglesEXT GLEW_GET_FUN(__glewWindowRectanglesEXT)

#define GLEW_EXT_window_rectangles GLEW_GET_VAR(__GLEW_EXT_window_rectangles)

#endif /* GL_EXT_window_rectangles */

/* ------------------------- GL_EXT_x11_sync_object ------------------------ */

#ifndef GL_EXT_x11_sync_object
#define GL_EXT_x11_sync_object 1

#define GL_SYNC_X11_FENCE_EXT 0x90E1

typedef GLsync (GLAPIENTRY * PFNGLIMPORTSYNCEXTPROC) (GLenum external_sync_type, GLintptr external_sync, GLbitfield flags);

#define glImportSyncEXT GLEW_GET_FUN(__glewImportSyncEXT)

#define GLEW_EXT_x11_sync_object GLEW_GET_VAR(__GLEW_EXT_x11_sync_object)

#endif /* GL_EXT_x11_sync_object */

/* --------------------- GL_KHR_blend_equation_advanced -------------------- */

#ifndef GL_KHR_blend_equation_advanced
#define GL_KHR_blend_equation_advanced 1

#define GL_BLEND_ADVANCED_COHERENT_KHR 0x9285
#define GL_MULTIPLY_KHR 0x9294
#define GL_SCREEN_KHR 0x9295
#define GL_OVERLAY_KHR 0x9296
#define GL_DARKEN_KHR 0x9297
#define GL_LIGHTEN_KHR 0x9298
#define GL_COLORDODGE_KHR 0x9299
#define GL_COLORBURN_KHR 0x929A
#define GL_HARDLIGHT_KHR 0x929B
#define GL_SOFTLIGHT_KHR 0x929C
#define GL_DIFFERENCE_KHR 0x929E
#define GL_EXCLUSION_KHR 0x92A0
#define GL_HSL_HUE_KHR 0x92AD
#define GL_HSL_SATURATION_KHR 0x92AE
#define GL_HSL_COLOR_KHR 0x92AF
#define GL_HSL_LUMINOSITY_KHR 0x92B0

typedef void (GLAPIENTRY * PFNGLBLENDBARRIERKHRPROC) (void);

#define glBlendBarrierKHR GLEW_GET_FUN(__glewBlendBarrierKHR)

#define GLEW_KHR_blend_equation_advanced GLEW_GET_VAR(__GLEW_KHR_blend_equation_advanced)

#endif /* GL_KHR_blend_equation_advanced */

/* ---------------- GL_KHR_blend_equation_advanced_coherent ---------------- */

#ifndef GL_KHR_blend_equation_advanced_coherent
#define GL_KHR_blend_equation_advanced_coherent 1

#define GLEW_KHR_blend_equation_advanced_coherent GLEW_GET_VAR(__GLEW_KHR_blend_equation_advanced_coherent)

#endif /* GL_KHR_blend_equation_advanced_coherent */

/* ---------------------- GL_KHR_context_flush_control --------------------- */

#ifndef GL_KHR_context_flush_control
#define GL_KHR_context_flush_control 1

#define GLEW_KHR_context_flush_control GLEW_GET_VAR(__GLEW_KHR_context_flush_control)

#endif /* GL_KHR_context_flush_control */

/* ------------------------------ GL_KHR_debug ----------------------------- */

#ifndef GL_KHR_debug
#define GL_KHR_debug 1

#define GL_CONTEXT_FLAG_DEBUG_BIT 0x00000002
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION 0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM 0x8245
#define GL_DEBUG_SOURCE_API 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#define GL_DEBUG_SOURCE_APPLICATION 0x824A
#define GL_DEBUG_SOURCE_OTHER 0x824B
#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_DEBUG_TYPE_MARKER 0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP 0x8269
#define GL_DEBUG_TYPE_POP_GROUP 0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH 0x826C
#define GL_DEBUG_GROUP_STACK_DEPTH 0x826D
#define GL_BUFFER 0x82E0
#define GL_SHADER 0x82E1
#define GL_PROGRAM 0x82E2
#define GL_QUERY 0x82E3
#define GL_PROGRAM_PIPELINE 0x82E4
#define GL_SAMPLER 0x82E6
#define GL_DISPLAY_LIST 0x82E7
#define GL_MAX_LABEL_LENGTH 0x82E8
#define GL_MAX_DEBUG_MESSAGE_LENGTH 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES 0x9144
#define GL_DEBUG_LOGGED_MESSAGES 0x9145
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_LOW 0x9148
#define GL_DEBUG_OUTPUT 0x92E0

typedef void (GLAPIENTRY *GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECALLBACKPROC) (GLDEBUGPROC callback, const void *userParam);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECONTROLPROC) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGEINSERTPROC) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
typedef GLuint (GLAPIENTRY * PFNGLGETDEBUGMESSAGELOGPROC) (GLuint count, GLsizei bufSize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog);
typedef void (GLAPIENTRY * PFNGLGETOBJECTLABELPROC) (GLenum identifier, GLuint name, GLsizei bufSize, GLsizei* length, GLchar *label);
typedef void (GLAPIENTRY * PFNGLGETOBJECTPTRLABELPROC) (const void *ptr, GLsizei bufSize, GLsizei* length, GLchar *label);
typedef void (GLAPIENTRY * PFNGLOBJECTLABELPROC) (GLenum identifier, GLuint name, GLsizei length, const GLchar* label);
typedef void (GLAPIENTRY * PFNGLOBJECTPTRLABELPROC) (const void *ptr, GLsizei length, const GLchar* label);
typedef void (GLAPIENTRY * PFNGLPOPDEBUGGROUPPROC) (void);
typedef void (GLAPIENTRY * PFNGLPUSHDEBUGGROUPPROC) (GLenum source, GLuint id, GLsizei length, const GLchar * message);

#define glDebugMessageCallback GLEW_GET_FUN(__glewDebugMessageCallback)
#define glDebugMessageControl GLEW_GET_FUN(__glewDebugMessageControl)
#define glDebugMessageInsert GLEW_GET_FUN(__glewDebugMessageInsert)
#define glGetDebugMessageLog GLEW_GET_FUN(__glewGetDebugMessageLog)
#define glGetObjectLabel GLEW_GET_FUN(__glewGetObjectLabel)
#define glGetObjectPtrLabel GLEW_GET_FUN(__glewGetObjectPtrLabel)
#define glObjectLabel GLEW_GET_FUN(__glewObjectLabel)
#define glObjectPtrLabel GLEW_GET_FUN(__glewObjectPtrLabel)
#define glPopDebugGroup GLEW_GET_FUN(__glewPopDebugGroup)
#define glPushDebugGroup GLEW_GET_FUN(__glewPushDebugGroup)

#define GLEW_KHR_debug GLEW_GET_VAR(__GLEW_KHR_debug)

#endif /* GL_KHR_debug */

/* ---------------------------- GL_KHR_no_error ---------------------------- */

#ifndef GL_KHR_no_error
#define GL_KHR_no_error 1

#define GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR 0x00000008

#define GLEW_KHR_no_error GLEW_GET_VAR(__GLEW_KHR_no_error)

#endif /* GL_KHR_no_error */

/* --------------------- GL_KHR_parallel_shader_compile -------------------- */

#ifndef GL_KHR_parallel_shader_compile
#define GL_KHR_parallel_shader_compile 1

#define GL_MAX_SHADER_COMPILER_THREADS_KHR 0x91B0
#define GL_COMPLETION_STATUS_KHR 0x91B1

typedef void (GLAPIENTRY * PFNGLMAXSHADERCOMPILERTHREADSKHRPROC) (GLuint count);

#define glMaxShaderCompilerThreadsKHR GLEW_GET_FUN(__glewMaxShaderCompilerThreadsKHR)

#define GLEW_KHR_parallel_shader_compile GLEW_GET_VAR(__GLEW_KHR_parallel_shader_compile)

#endif /* GL_KHR_parallel_shader_compile */

/* ------------------ GL_KHR_robust_buffer_access_behavior ----------------- */

#ifndef GL_KHR_robust_buffer_access_behavior
#define GL_KHR_robust_buffer_access_behavior 1

#define GLEW_KHR_robust_buffer_access_behavior GLEW_GET_VAR(__GLEW_KHR_robust_buffer_access_behavior)

#endif /* GL_KHR_robust_buffer_access_behavior */

/* --------------------------- GL_KHR_robustness --------------------------- */

#ifndef GL_KHR_robustness
#define GL_KHR_robustness 1

#define GL_CONTEXT_LOST 0x0507
#define GL_LOSE_CONTEXT_ON_RESET 0x8252
#define GL_GUILTY_CONTEXT_RESET 0x8253
#define GL_INNOCENT_CONTEXT_RESET 0x8254
#define GL_UNKNOWN_CONTEXT_RESET 0x8255
#define GL_RESET_NOTIFICATION_STRATEGY 0x8256
#define GL_NO_RESET_NOTIFICATION 0x8261
#define GL_CONTEXT_ROBUST_ACCESS 0x90F3

typedef void (GLAPIENTRY * PFNGLGETNUNIFORMFVPROC) (GLuint program, GLint location, GLsizei bufSize, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMIVPROC) (GLuint program, GLint location, GLsizei bufSize, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMUIVPROC) (GLuint program, GLint location, GLsizei bufSize, GLuint* params);
typedef void (GLAPIENTRY * PFNGLREADNPIXELSPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);

#define glGetnUniformfv GLEW_GET_FUN(__glewGetnUniformfv)
#define glGetnUniformiv GLEW_GET_FUN(__glewGetnUniformiv)
#define glGetnUniformuiv GLEW_GET_FUN(__glewGetnUniformuiv)
#define glReadnPixels GLEW_GET_FUN(__glewReadnPixels)

#define GLEW_KHR_robustness GLEW_GET_VAR(__GLEW_KHR_robustness)

#endif /* GL_KHR_robustness */

/* ------------------ GL_KHR_texture_compression_astc_hdr ------------------ */

#ifndef GL_KHR_texture_compression_astc_hdr
#define GL_KHR_texture_compression_astc_hdr 1

#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR 0x93B0
#define GL_COMPRESSED_RGBA_ASTC_5x4_KHR 0x93B1
#define GL_COMPRESSED_RGBA_ASTC_5x5_KHR 0x93B2
#define GL_COMPRESSED_RGBA_ASTC_6x5_KHR 0x93B3
#define GL_COMPRESSED_RGBA_ASTC_6x6_KHR 0x93B4
#define GL_COMPRESSED_RGBA_ASTC_8x5_KHR 0x93B5
#define GL_COMPRESSED_RGBA_ASTC_8x6_KHR 0x93B6
#define GL_COMPRESSED_RGBA_ASTC_8x8_KHR 0x93B7
#define GL_COMPRESSED_RGBA_ASTC_10x5_KHR 0x93B8
#define GL_COMPRESSED_RGBA_ASTC_10x6_KHR 0x93B9
#define GL_COMPRESSED_RGBA_ASTC_10x8_KHR 0x93BA
#define GL_COMPRESSED_RGBA_ASTC_10x10_KHR 0x93BB
#define GL_COMPRESSED_RGBA_ASTC_12x10_KHR 0x93BC
#define GL_COMPRESSED_RGBA_ASTC_12x12_KHR 0x93BD
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR 0x93D0
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR 0x93D1
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR 0x93D2
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR 0x93D3
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR 0x93D4
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR 0x93D5
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR 0x93D6
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR 0x93D7
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR 0x93D8
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR 0x93D9
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR 0x93DA
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR 0x93DB
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR 0x93DC
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR 0x93DD

#define GLEW_KHR_texture_compression_astc_hdr GLEW_GET_VAR(__GLEW_KHR_texture_compression_astc_hdr)

#endif /* GL_KHR_texture_compression_astc_hdr */

/* ------------------ GL_KHR_texture_compression_astc_ldr ------------------ */

#ifndef GL_KHR_texture_compression_astc_ldr
#define GL_KHR_texture_compression_astc_ldr 1

#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR 0x93B0
#define GL_COMPRESSED_RGBA_ASTC_5x4_KHR 0x93B1
#define GL_COMPRESSED_RGBA_ASTC_5x5_KHR 0x93B2
#define GL_COMPRESSED_RGBA_ASTC_6x5_KHR 0x93B3
#define GL_COMPRESSED_RGBA_ASTC_6x6_KHR 0x93B4
#define GL_COMPRESSED_RGBA_ASTC_8x5_KHR 0x93B5
#define GL_COMPRESSED_RGBA_ASTC_8x6_KHR 0x93B6
#define GL_COMPRESSED_RGBA_ASTC_8x8_KHR 0x93B7
#define GL_COMPRESSED_RGBA_ASTC_10x5_KHR 0x93B8
#define GL_COMPRESSED_RGBA_ASTC_10x6_KHR 0x93B9
#define GL_COMPRESSED_RGBA_ASTC_10x8_KHR 0x93BA
#define GL_COMPRESSED_RGBA_ASTC_10x10_KHR 0x93BB
#define GL_COMPRESSED_RGBA_ASTC_12x10_KHR 0x93BC
#define GL_COMPRESSED_RGBA_ASTC_12x12_KHR 0x93BD
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR 0x93D0
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR 0x93D1
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR 0x93D2
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR 0x93D3
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR 0x93D4
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR 0x93D5
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR 0x93D6
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR 0x93D7
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR 0x93D8
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR 0x93D9
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR 0x93DA
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR 0x93DB
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR 0x93DC
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR 0x93DD

#define GLEW_KHR_texture_compression_astc_ldr GLEW_GET_VAR(__GLEW_KHR_texture_compression_astc_ldr)

#endif /* GL_KHR_texture_compression_astc_ldr */

/* --------------- GL_KHR_texture_compression_astc_sliced_3d --------------- */

#ifndef GL_KHR_texture_compression_astc_sliced_3d
#define GL_KHR_texture_compression_astc_sliced_3d 1

#define GLEW_KHR_texture_compression_astc_sliced_3d GLEW_GET_VAR(__GLEW_KHR_texture_compression_astc_sliced_3d)

#endif /* GL_KHR_texture_compression_astc_sliced_3d */

/* ------------------- GL_NV_EGL_stream_consumer_external ------------------ */

#ifndef GL_NV_EGL_stream_consumer_external
#define GL_NV_EGL_stream_consumer_external 1

#define GL_TEXTURE_EXTERNAL_OES 0x8D65
#define GL_SAMPLER_EXTERNAL_OES 0x8D66
#define GL_TEXTURE_BINDING_EXTERNAL_OES 0x8D67
#define GL_REQUIRED_TEXTURE_IMAGE_UNITS_OES 0x8D68

#define GLEW_NV_EGL_stream_consumer_external GLEW_GET_VAR(__GLEW_NV_EGL_stream_consumer_external)

#endif /* GL_NV_EGL_stream_consumer_external */

/* -------------------- GL_NV_gpu_program5_mem_extended -------------------- */

#ifndef GL_NV_gpu_program5_mem_extended
#define GL_NV_gpu_program5_mem_extended 1

#define GLEW_NV_gpu_program5_mem_extended GLEW_GET_VAR(__GLEW_NV_gpu_program5_mem_extended)

#endif /* GL_NV_gpu_program5_mem_extended */

/* ---------------------- GL_NV_shader_atomic_float64 ---------------------- */

#ifndef GL_NV_shader_atomic_float64
#define GL_NV_shader_atomic_float64 1

#define GLEW_NV_shader_atomic_float64 GLEW_GET_VAR(__GLEW_NV_shader_atomic_float64)

#endif /* GL_NV_shader_atomic_float64 */

/* ----------------------- GL_NV_shader_atomic_int64 ----------------------- */

#ifndef GL_NV_shader_atomic_int64
#define GL_NV_shader_atomic_int64 1

#define GLEW_NV_shader_atomic_int64 GLEW_GET_VAR(__GLEW_NV_shader_atomic_int64)

#endif /* GL_NV_shader_atomic_int64 */

/* -------------------------- GL_QCOM_extended_get ------------------------- */

#ifndef GL_QCOM_extended_get
#define GL_QCOM_extended_get 1

#define GL_TEXTURE_WIDTH_QCOM 0x8BD2
#define GL_TEXTURE_HEIGHT_QCOM 0x8BD3
#define GL_TEXTURE_DEPTH_QCOM 0x8BD4
#define GL_TEXTURE_INTERNAL_FORMAT_QCOM 0x8BD5
#define GL_TEXTURE_FORMAT_QCOM 0x8BD6
#define GL_TEXTURE_TYPE_QCOM 0x8BD7
#define GL_TEXTURE_IMAGE_VALID_QCOM 0x8BD8
#define GL_TEXTURE_NUM_LEVELS_QCOM 0x8BD9
#define GL_TEXTURE_TARGET_QCOM 0x8BDA
#define GL_TEXTURE_OBJECT_VALID_QCOM 0x8BDB
#define GL_STATE_RESTORE 0x8BDC

typedef void (GLAPIENTRY * PFNGLEXTGETBUFFERPOINTERVQCOMPROC) (GLenum target, void** params);
typedef void (GLAPIENTRY * PFNGLEXTGETBUFFERSQCOMPROC) (GLuint* buffers, GLint maxBuffers, GLint* numBuffers);
typedef void (GLAPIENTRY * PFNGLEXTGETFRAMEBUFFERSQCOMPROC) (GLuint* framebuffers, GLint maxFramebuffers, GLint* numFramebuffers);
typedef void (GLAPIENTRY * PFNGLEXTGETRENDERBUFFERSQCOMPROC) (GLuint* renderbuffers, GLint maxRenderbuffers, GLint* numRenderbuffers);
typedef void (GLAPIENTRY * PFNGLEXTGETTEXLEVELPARAMETERIVQCOMPROC) (GLuint texture, GLenum face, GLint level, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLEXTGETTEXSUBIMAGEQCOMPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, void *texels);
typedef void (GLAPIENTRY * PFNGLEXTGETTEXTURESQCOMPROC) (GLuint* textures, GLint maxTextures, GLint* numTextures);
typedef void (GLAPIENTRY * PFNGLEXTTEXOBJECTSTATEOVERRIDEIQCOMPROC) (GLenum target, GLenum pname, GLint param);

#define glExtGetBufferPointervQCOM GLEW_GET_FUN(__glewExtGetBufferPointervQCOM)
#define glExtGetBuffersQCOM GLEW_GET_FUN(__glewExtGetBuffersQCOM)
#define glExtGetFramebuffersQCOM GLEW_GET_FUN(__glewExtGetFramebuffersQCOM)
#define glExtGetRenderbuffersQCOM GLEW_GET_FUN(__glewExtGetRenderbuffersQCOM)
#define glExtGetTexLevelParameterivQCOM GLEW_GET_FUN(__glewExtGetTexLevelParameterivQCOM)
#define glExtGetTexSubImageQCOM GLEW_GET_FUN(__glewExtGetTexSubImageQCOM)
#define glExtGetTexturesQCOM GLEW_GET_FUN(__glewExtGetTexturesQCOM)
#define glExtTexObjectStateOverrideiQCOM GLEW_GET_FUN(__glewExtTexObjectStateOverrideiQCOM)

#define GLEW_QCOM_extended_get GLEW_GET_VAR(__GLEW_QCOM_extended_get)

#endif /* GL_QCOM_extended_get */

/* ------------------------- GL_QCOM_extended_get2 ------------------------- */

#ifndef GL_QCOM_extended_get2
#define GL_QCOM_extended_get2 1

typedef void (GLAPIENTRY * PFNGLEXTGETPROGRAMBINARYSOURCEQCOMPROC) (GLuint program, GLenum shadertype, GLchar* source, GLint* length);
typedef void (GLAPIENTRY * PFNGLEXTGETPROGRAMSQCOMPROC) (GLuint* programs, GLint maxPrograms, GLint* numPrograms);
typedef void (GLAPIENTRY * PFNGLEXTGETSHADERSQCOMPROC) (GLuint* shaders, GLint maxShaders, GLint* numShaders);
typedef GLboolean (GLAPIENTRY * PFNGLEXTISPROGRAMBINARYQCOMPROC) (GLuint program);

#define glExtGetProgramBinarySourceQCOM GLEW_GET_FUN(__glewExtGetProgramBinarySourceQCOM)
#define glExtGetProgramsQCOM GLEW_GET_FUN(__glewExtGetProgramsQCOM)
#define glExtGetShadersQCOM GLEW_GET_FUN(__glewExtGetShadersQCOM)
#define glExtIsProgramBinaryQCOM GLEW_GET_FUN(__glewExtIsProgramBinaryQCOM)

#define GLEW_QCOM_extended_get2 GLEW_GET_VAR(__GLEW_QCOM_extended_get2)

#endif /* GL_QCOM_extended_get2 */

/* ------------------------ GL_REGAL_extension_query ----------------------- */

#ifndef GL_REGAL_extension_query
#define GL_REGAL_extension_query 1

typedef GLboolean (GLAPIENTRY * PFNGLGETEXTENSIONREGALPROC) (const GLchar* ext);
typedef GLboolean (GLAPIENTRY * PFNGLISSUPPORTEDREGALPROC) (const GLchar* ext);

#define glGetExtensionREGAL GLEW_GET_FUN(__glewGetExtensionREGAL)
#define glIsSupportedREGAL GLEW_GET_FUN(__glewIsSupportedREGAL)

#define GLEW_REGAL_extension_query GLEW_GET_VAR(__GLEW_REGAL_extension_query)

#endif /* GL_REGAL_extension_query */

/* -------------------------- GL_WIN_phong_shading ------------------------- */

#ifndef GL_WIN_phong_shading
#define GL_WIN_phong_shading 1

#define GL_PHONG_WIN 0x80EA
#define GL_PHONG_HINT_WIN 0x80EB

#define GLEW_WIN_phong_shading GLEW_GET_VAR(__GLEW_WIN_phong_shading)

#endif /* GL_WIN_phong_shading */

/* ------------------------- GL_WIN_scene_markerXXX ------------------------ */

#ifndef GL_WIN_scene_markerXXX
#define GL_WIN_scene_markerXXX 1

#define GLEW_WIN_scene_markerXXX GLEW_GET_VAR(__GLEW_WIN_scene_markerXXX)

#endif /* GL_WIN_scene_markerXXX */

/* -------------------------- GL_WIN_specular_fog -------------------------- */

#ifndef GL_WIN_specular_fog
#define GL_WIN_specular_fog 1

#define GL_FOG_SPECULAR_TEXTURE_WIN 0x80EC

#define GLEW_WIN_specular_fog GLEW_GET_VAR(__GLEW_WIN_specular_fog)

#endif /* GL_WIN_specular_fog */

/* ---------------------------- GL_WIN_swap_hint --------------------------- */

#ifndef GL_WIN_swap_hint
#define GL_WIN_swap_hint 1

typedef void (GLAPIENTRY * PFNGLADDSWAPHINTRECTWINPROC) (GLint x, GLint y, GLsizei width, GLsizei height);

#define glAddSwapHintRectWIN GLEW_GET_FUN(__glewAddSwapHintRectWIN)

#define GLEW_WIN_swap_hint GLEW_GET_VAR(__GLEW_WIN_swap_hint)

#endif /* GL_WIN_swap_hint */

/* ------------------------------------------------------------------------- */

#if defined(GLEW_MX)
#define GLEW_FUN_EXPORT
#else
#define GLEW_FUN_EXPORT GLEWAPI
#endif /* GLEW_MX */

#if defined(GLEW_MX)
#define GLEW_VAR_EXPORT
#else
#define GLEW_VAR_EXPORT GLEWAPI
#endif /* GLEW_MX */

#if defined(GLEW_MX)
struct GLEWContextStruct
{
#endif /* GLEW_MX */

GLEW_FUN_EXPORT PFNGLCOPYTEXSUBIMAGE3DPROC __glewCopyTexSubImage3D;
GLEW_FUN_EXPORT PFNGLDRAWRANGEELEMENTSPROC __glewDrawRangeElements;
GLEW_FUN_EXPORT PFNGLTEXIMAGE3DPROC __glewTexImage3D;
GLEW_FUN_EXPORT PFNGLTEXSUBIMAGE3DPROC __glewTexSubImage3D;

GLEW_FUN_EXPORT PFNGLACTIVETEXTUREPROC __glewActiveTexture;
GLEW_FUN_EXPORT PFNGLCLIENTACTIVETEXTUREPROC __glewClientActiveTexture;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE1DPROC __glewCompressedTexImage1D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE2DPROC __glewCompressedTexImage2D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE3DPROC __glewCompressedTexImage3D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC __glewCompressedTexSubImage1D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC __glewCompressedTexSubImage2D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC __glewCompressedTexSubImage3D;
GLEW_FUN_EXPORT PFNGLGETCOMPRESSEDTEXIMAGEPROC __glewGetCompressedTexImage;
GLEW_FUN_EXPORT PFNGLLOADTRANSPOSEMATRIXDPROC __glewLoadTransposeMatrixd;
GLEW_FUN_EXPORT PFNGLLOADTRANSPOSEMATRIXFPROC __glewLoadTransposeMatrixf;
GLEW_FUN_EXPORT PFNGLMULTTRANSPOSEMATRIXDPROC __glewMultTransposeMatrixd;
GLEW_FUN_EXPORT PFNGLMULTTRANSPOSEMATRIXFPROC __glewMultTransposeMatrixf;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1DPROC __glewMultiTexCoord1d;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1DVPROC __glewMultiTexCoord1dv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1FPROC __glewMultiTexCoord1f;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1FVPROC __glewMultiTexCoord1fv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1IPROC __glewMultiTexCoord1i;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1IVPROC __glewMultiTexCoord1iv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1SPROC __glewMultiTexCoord1s;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1SVPROC __glewMultiTexCoord1sv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2DPROC __glewMultiTexCoord2d;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2DVPROC __glewMultiTexCoord2dv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2FPROC __glewMultiTexCoord2f;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2FVPROC __glewMultiTexCoord2fv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2IPROC __glewMultiTexCoord2i;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2IVPROC __glewMultiTexCoord2iv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2SPROC __glewMultiTexCoord2s;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2SVPROC __glewMultiTexCoord2sv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3DPROC __glewMultiTexCoord3d;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3DVPROC __glewMultiTexCoord3dv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3FPROC __glewMultiTexCoord3f;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3FVPROC __glewMultiTexCoord3fv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3IPROC __glewMultiTexCoord3i;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3IVPROC __glewMultiTexCoord3iv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3SPROC __glewMultiTexCoord3s;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3SVPROC __glewMultiTexCoord3sv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4DPROC __glewMultiTexCoord4d;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4DVPROC __glewMultiTexCoord4dv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4FPROC __glewMultiTexCoord4f;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4FVPROC __glewMultiTexCoord4fv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4IPROC __glewMultiTexCoord4i;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4IVPROC __glewMultiTexCoord4iv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4SPROC __glewMultiTexCoord4s;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4SVPROC __glewMultiTexCoord4sv;
GLEW_FUN_EXPORT PFNGLSAMPLECOVERAGEPROC __glewSampleCoverage;

GLEW_FUN_EXPORT PFNGLBLENDCOLORPROC __glewBlendColor;
GLEW_FUN_EXPORT PFNGLBLENDEQUATIONPROC __glewBlendEquation;
GLEW_FUN_EXPORT PFNGLBLENDFUNCSEPARATEPROC __glewBlendFuncSeparate;
GLEW_FUN_EXPORT PFNGLFOGCOORDPOINTERPROC __glewFogCoordPointer;
GLEW_FUN_EXPORT PFNGLFOGCOORDDPROC __glewFogCoordd;
GLEW_FUN_EXPORT PFNGLFOGCOORDDVPROC __glewFogCoorddv;
GLEW_FUN_EXPORT PFNGLFOGCOORDFPROC __glewFogCoordf;
GLEW_FUN_EXPORT PFNGLFOGCOORDFVPROC __glewFogCoordfv;
GLEW_FUN_EXPORT PFNGLMULTIDRAWARRAYSPROC __glewMultiDrawArrays;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTSPROC __glewMultiDrawElements;
GLEW_FUN_EXPORT PFNGLPOINTPARAMETERFPROC __glewPointParameterf;
GLEW_FUN_EXPORT PFNGLPOINTPARAMETERFVPROC __glewPointParameterfv;
GLEW_FUN_EXPORT PFNGLPOINTPARAMETERIPROC __glewPointParameteri;
GLEW_FUN_EXPORT PFNGLPOINTPARAMETERIVPROC __glewPointParameteriv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3BPROC __glewSecondaryColor3b;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3BVPROC __glewSecondaryColor3bv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3DPROC __glewSecondaryColor3d;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3DVPROC __glewSecondaryColor3dv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3FPROC __glewSecondaryColor3f;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3FVPROC __glewSecondaryColor3fv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3IPROC __glewSecondaryColor3i;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3IVPROC __glewSecondaryColor3iv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3SPROC __glewSecondaryColor3s;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3SVPROC __glewSecondaryColor3sv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UBPROC __glewSecondaryColor3ub;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UBVPROC __glewSecondaryColor3ubv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UIPROC __glewSecondaryColor3ui;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UIVPROC __glewSecondaryColor3uiv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3USPROC __glewSecondaryColor3us;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3USVPROC __glewSecondaryColor3usv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLORPOINTERPROC __glewSecondaryColorPointer;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2DPROC __glewWindowPos2d;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2DVPROC __glewWindowPos2dv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2FPROC __glewWindowPos2f;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2FVPROC __glewWindowPos2fv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2IPROC __glewWindowPos2i;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2IVPROC __glewWindowPos2iv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2SPROC __glewWindowPos2s;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2SVPROC __glewWindowPos2sv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3DPROC __glewWindowPos3d;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3DVPROC __glewWindowPos3dv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3FPROC __glewWindowPos3f;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3FVPROC __glewWindowPos3fv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3IPROC __glewWindowPos3i;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3IVPROC __glewWindowPos3iv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3SPROC __glewWindowPos3s;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3SVPROC __glewWindowPos3sv;

GLEW_FUN_EXPORT PFNGLBEGINQUERYPROC __glewBeginQuery;
GLEW_FUN_EXPORT PFNGLBINDBUFFERPROC __glewBindBuffer;
GLEW_FUN_EXPORT PFNGLBUFFERDATAPROC __glewBufferData;
GLEW_FUN_EXPORT PFNGLBUFFERSUBDATAPROC __glewBufferSubData;
GLEW_FUN_EXPORT PFNGLDELETEBUFFERSPROC __glewDeleteBuffers;
GLEW_FUN_EXPORT PFNGLDELETEQUERIESPROC __glewDeleteQueries;
GLEW_FUN_EXPORT PFNGLENDQUERYPROC __glewEndQuery;
GLEW_FUN_EXPORT PFNGLGENBUFFERSPROC __glewGenBuffers;
GLEW_FUN_EXPORT PFNGLGENQUERIESPROC __glewGenQueries;
GLEW_FUN_EXPORT PFNGLGETBUFFERPARAMETERIVPROC __glewGetBufferParameteriv;
GLEW_FUN_EXPORT PFNGLGETBUFFERPOINTERVPROC __glewGetBufferPointerv;
GLEW_FUN_EXPORT PFNGLGETBUFFERSUBDATAPROC __glewGetBufferSubData;
GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTIVPROC __glewGetQueryObjectiv;
GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTUIVPROC __glewGetQueryObjectuiv;
GLEW_FUN_EXPORT PFNGLGETQUERYIVPROC __glewGetQueryiv;
GLEW_FUN_EXPORT PFNGLISBUFFERPROC __glewIsBuffer;
GLEW_FUN_EXPORT PFNGLISQUERYPROC __glewIsQuery;
GLEW_FUN_EXPORT PFNGLMAPBUFFERPROC __glewMapBuffer;
GLEW_FUN_EXPORT PFNGLUNMAPBUFFERPROC __glewUnmapBuffer;

GLEW_FUN_EXPORT PFNGLATTACHSHADERPROC __glewAttachShader;
GLEW_FUN_EXPORT PFNGLBINDATTRIBLOCATIONPROC __glewBindAttribLocation;
GLEW_FUN_EXPORT PFNGLBLENDEQUATIONSEPARATEPROC __glewBlendEquationSeparate;
GLEW_FUN_EXPORT PFNGLCOMPILESHADERPROC __glewCompileShader;
GLEW_FUN_EXPORT PFNGLCREATEPROGRAMPROC __glewCreateProgram;
GLEW_FUN_EXPORT PFNGLCREATESHADERPROC __glewCreateShader;
GLEW_FUN_EXPORT PFNGLDELETEPROGRAMPROC __glewDeleteProgram;
GLEW_FUN_EXPORT PFNGLDELETESHADERPROC __glewDeleteShader;
GLEW_FUN_EXPORT PFNGLDETACHSHADERPROC __glewDetachShader;
GLEW_FUN_EXPORT PFNGLDISABLEVERTEXATTRIBARRAYPROC __glewDisableVertexAttribArray;
GLEW_FUN_EXPORT PFNGLDRAWBUFFERSPROC __glewDrawBuffers;
GLEW_FUN_EXPORT PFNGLENABLEVERTEXATTRIBARRAYPROC __glewEnableVertexAttribArray;
GLEW_FUN_EXPORT PFNGLGETACTIVEATTRIBPROC __glewGetActiveAttrib;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMPROC __glewGetActiveUniform;
GLEW_FUN_EXPORT PFNGLGETATTACHEDSHADERSPROC __glewGetAttachedShaders;
GLEW_FUN_EXPORT PFNGLGETATTRIBLOCATIONPROC __glewGetAttribLocation;
GLEW_FUN_EXPORT PFNGLGETPROGRAMINFOLOGPROC __glewGetProgramInfoLog;
GLEW_FUN_EXPORT PFNGLGETPROGRAMIVPROC __glewGetProgramiv;
GLEW_FUN_EXPORT PFNGLGETSHADERINFOLOGPROC __glewGetShaderInfoLog;
GLEW_FUN_EXPORT PFNGLGETSHADERSOURCEPROC __glewGetShaderSource;
GLEW_FUN_EXPORT PFNGLGETSHADERIVPROC __glewGetShaderiv;
GLEW_FUN_EXPORT PFNGLGETUNIFORMLOCATIONPROC __glewGetUniformLocation;
GLEW_FUN_EXPORT PFNGLGETUNIFORMFVPROC __glewGetUniformfv;
GLEW_FUN_EXPORT PFNGLGETUNIFORMIVPROC __glewGetUniformiv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBPOINTERVPROC __glewGetVertexAttribPointerv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBDVPROC __glewGetVertexAttribdv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBFVPROC __glewGetVertexAttribfv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIVPROC __glewGetVertexAttribiv;
GLEW_FUN_EXPORT PFNGLISPROGRAMPROC __glewIsProgram;
GLEW_FUN_EXPORT PFNGLISSHADERPROC __glewIsShader;
GLEW_FUN_EXPORT PFNGLLINKPROGRAMPROC __glewLinkProgram;
GLEW_FUN_EXPORT PFNGLSHADERSOURCEPROC __glewShaderSource;
GLEW_FUN_EXPORT PFNGLSTENCILFUNCSEPARATEPROC __glewStencilFuncSeparate;
GLEW_FUN_EXPORT PFNGLSTENCILMASKSEPARATEPROC __glewStencilMaskSeparate;
GLEW_FUN_EXPORT PFNGLSTENCILOPSEPARATEPROC __glewStencilOpSeparate;
GLEW_FUN_EXPORT PFNGLUNIFORM1FPROC __glewUniform1f;
GLEW_FUN_EXPORT PFNGLUNIFORM1FVPROC __glewUniform1fv;
GLEW_FUN_EXPORT PFNGLUNIFORM1IPROC __glewUniform1i;
GLEW_FUN_EXPORT PFNGLUNIFORM1IVPROC __glewUniform1iv;
GLEW_FUN_EXPORT PFNGLUNIFORM2FPROC __glewUniform2f;
GLEW_FUN_EXPORT PFNGLUNIFORM2FVPROC __glewUniform2fv;
GLEW_FUN_EXPORT PFNGLUNIFORM2IPROC __glewUniform2i;
GLEW_FUN_EXPORT PFNGLUNIFORM2IVPROC __glewUniform2iv;
GLEW_FUN_EXPORT PFNGLUNIFORM3FPROC __glewUniform3f;
GLEW_FUN_EXPORT PFNGLUNIFORM3FVPROC __glewUniform3fv;
GLEW_FUN_EXPORT PFNGLUNIFORM3IPROC __glewUniform3i;
GLEW_FUN_EXPORT PFNGLUNIFORM3IVPROC __glewUniform3iv;
GLEW_FUN_EXPORT PFNGLUNIFORM4FPROC __glewUniform4f;
GLEW_FUN_EXPORT PFNGLUNIFORM4FVPROC __glewUniform4fv;
GLEW_FUN_EXPORT PFNGLUNIFORM4IPROC __glewUniform4i;
GLEW_FUN_EXPORT PFNGLUNIFORM4IVPROC __glewUniform4iv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2FVPROC __glewUniformMatrix2fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3FVPROC __glewUniformMatrix3fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4FVPROC __glewUniformMatrix4fv;
GLEW_FUN_EXPORT PFNGLUSEPROGRAMPROC __glewUseProgram;
GLEW_FUN_EXPORT PFNGLVALIDATEPROGRAMPROC __glewValidateProgram;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1DPROC __glewVertexAttrib1d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1DVPROC __glewVertexAttrib1dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1FPROC __glewVertexAttrib1f;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1FVPROC __glewVertexAttrib1fv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1SPROC __glewVertexAttrib1s;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1SVPROC __glewVertexAttrib1sv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2DPROC __glewVertexAttrib2d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2DVPROC __glewVertexAttrib2dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2FPROC __glewVertexAttrib2f;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2FVPROC __glewVertexAttrib2fv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2SPROC __glewVertexAttrib2s;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2SVPROC __glewVertexAttrib2sv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3DPROC __glewVertexAttrib3d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3DVPROC __glewVertexAttrib3dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3FPROC __glewVertexAttrib3f;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3FVPROC __glewVertexAttrib3fv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3SPROC __glewVertexAttrib3s;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3SVPROC __glewVertexAttrib3sv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NBVPROC __glewVertexAttrib4Nbv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NIVPROC __glewVertexAttrib4Niv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NSVPROC __glewVertexAttrib4Nsv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUBPROC __glewVertexAttrib4Nub;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUBVPROC __glewVertexAttrib4Nubv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUIVPROC __glewVertexAttrib4Nuiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUSVPROC __glewVertexAttrib4Nusv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4BVPROC __glewVertexAttrib4bv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4DPROC __glewVertexAttrib4d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4DVPROC __glewVertexAttrib4dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4FPROC __glewVertexAttrib4f;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4FVPROC __glewVertexAttrib4fv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4IVPROC __glewVertexAttrib4iv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4SPROC __glewVertexAttrib4s;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4SVPROC __glewVertexAttrib4sv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4UBVPROC __glewVertexAttrib4ubv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4UIVPROC __glewVertexAttrib4uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4USVPROC __glewVertexAttrib4usv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBPOINTERPROC __glewVertexAttribPointer;

GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2X3FVPROC __glewUniformMatrix2x3fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2X4FVPROC __glewUniformMatrix2x4fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3X2FVPROC __glewUniformMatrix3x2fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3X4FVPROC __glewUniformMatrix3x4fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4X2FVPROC __glewUniformMatrix4x2fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4X3FVPROC __glewUniformMatrix4x3fv;

GLEW_FUN_EXPORT PFNGLBEGINCONDITIONALRENDERPROC __glewBeginConditionalRender;
GLEW_FUN_EXPORT PFNGLBEGINTRANSFORMFEEDBACKPROC __glewBeginTransformFeedback;
GLEW_FUN_EXPORT PFNGLBINDFRAGDATALOCATIONPROC __glewBindFragDataLocation;
GLEW_FUN_EXPORT PFNGLCLAMPCOLORPROC __glewClampColor;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERFIPROC __glewClearBufferfi;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERFVPROC __glewClearBufferfv;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERIVPROC __glewClearBufferiv;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERUIVPROC __glewClearBufferuiv;
GLEW_FUN_EXPORT PFNGLCOLORMASKIPROC __glewColorMaski;
GLEW_FUN_EXPORT PFNGLDISABLEIPROC __glewDisablei;
GLEW_FUN_EXPORT PFNGLENABLEIPROC __glewEnablei;
GLEW_FUN_EXPORT PFNGLENDCONDITIONALRENDERPROC __glewEndConditionalRender;
GLEW_FUN_EXPORT PFNGLENDTRANSFORMFEEDBACKPROC __glewEndTransformFeedback;
GLEW_FUN_EXPORT PFNGLGETBOOLEANI_VPROC __glewGetBooleani_v;
GLEW_FUN_EXPORT PFNGLGETFRAGDATALOCATIONPROC __glewGetFragDataLocation;
GLEW_FUN_EXPORT PFNGLGETSTRINGIPROC __glewGetStringi;
GLEW_FUN_EXPORT PFNGLGETTEXPARAMETERIIVPROC __glewGetTexParameterIiv;
GLEW_FUN_EXPORT PFNGLGETTEXPARAMETERIUIVPROC __glewGetTexParameterIuiv;
GLEW_FUN_EXPORT PFNGLGETTRANSFORMFEEDBACKVARYINGPROC __glewGetTransformFeedbackVarying;
GLEW_FUN_EXPORT PFNGLGETUNIFORMUIVPROC __glewGetUniformuiv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIIVPROC __glewGetVertexAttribIiv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIUIVPROC __glewGetVertexAttribIuiv;
GLEW_FUN_EXPORT PFNGLISENABLEDIPROC __glewIsEnabledi;
GLEW_FUN_EXPORT PFNGLTEXPARAMETERIIVPROC __glewTexParameterIiv;
GLEW_FUN_EXPORT PFNGLTEXPARAMETERIUIVPROC __glewTexParameterIuiv;
GLEW_FUN_EXPORT PFNGLTRANSFORMFEEDBACKVARYINGSPROC __glewTransformFeedbackVaryings;
GLEW_FUN_EXPORT PFNGLUNIFORM1UIPROC __glewUniform1ui;
GLEW_FUN_EXPORT PFNGLUNIFORM1UIVPROC __glewUniform1uiv;
GLEW_FUN_EXPORT PFNGLUNIFORM2UIPROC __glewUniform2ui;
GLEW_FUN_EXPORT PFNGLUNIFORM2UIVPROC __glewUniform2uiv;
GLEW_FUN_EXPORT PFNGLUNIFORM3UIPROC __glewUniform3ui;
GLEW_FUN_EXPORT PFNGLUNIFORM3UIVPROC __glewUniform3uiv;
GLEW_FUN_EXPORT PFNGLUNIFORM4UIPROC __glewUniform4ui;
GLEW_FUN_EXPORT PFNGLUNIFORM4UIVPROC __glewUniform4uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1IPROC __glewVertexAttribI1i;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1IVPROC __glewVertexAttribI1iv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1UIPROC __glewVertexAttribI1ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1UIVPROC __glewVertexAttribI1uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2IPROC __glewVertexAttribI2i;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2IVPROC __glewVertexAttribI2iv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2UIPROC __glewVertexAttribI2ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2UIVPROC __glewVertexAttribI2uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3IPROC __glewVertexAttribI3i;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3IVPROC __glewVertexAttribI3iv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3UIPROC __glewVertexAttribI3ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3UIVPROC __glewVertexAttribI3uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4BVPROC __glewVertexAttribI4bv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4IPROC __glewVertexAttribI4i;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4IVPROC __glewVertexAttribI4iv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4SVPROC __glewVertexAttribI4sv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UBVPROC __glewVertexAttribI4ubv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UIPROC __glewVertexAttribI4ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UIVPROC __glewVertexAttribI4uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4USVPROC __glewVertexAttribI4usv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBIPOINTERPROC __glewVertexAttribIPointer;

GLEW_FUN_EXPORT PFNGLDRAWARRAYSINSTANCEDPROC __glewDrawArraysInstanced;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDPROC __glewDrawElementsInstanced;
GLEW_FUN_EXPORT PFNGLPRIMITIVERESTARTINDEXPROC __glewPrimitiveRestartIndex;
GLEW_FUN_EXPORT PFNGLTEXBUFFERPROC __glewTexBuffer;

GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTUREPROC __glewFramebufferTexture;
GLEW_FUN_EXPORT PFNGLGETBUFFERPARAMETERI64VPROC __glewGetBufferParameteri64v;
GLEW_FUN_EXPORT PFNGLGETINTEGER64I_VPROC __glewGetInteger64i_v;

GLEW_FUN_EXPORT PFNGLVERTEXATTRIBDIVISORPROC __glewVertexAttribDivisor;

GLEW_FUN_EXPORT PFNGLBLENDEQUATIONSEPARATEIPROC __glewBlendEquationSeparatei;
GLEW_FUN_EXPORT PFNGLBLENDEQUATIONIPROC __glewBlendEquationi;
GLEW_FUN_EXPORT PFNGLBLENDFUNCSEPARATEIPROC __glewBlendFuncSeparatei;
GLEW_FUN_EXPORT PFNGLBLENDFUNCIPROC __glewBlendFunci;
GLEW_FUN_EXPORT PFNGLMINSAMPLESHADINGPROC __glewMinSampleShading;

GLEW_FUN_EXPORT PFNGLGETGRAPHICSRESETSTATUSPROC __glewGetGraphicsResetStatus;
GLEW_FUN_EXPORT PFNGLGETNCOMPRESSEDTEXIMAGEPROC __glewGetnCompressedTexImage;
GLEW_FUN_EXPORT PFNGLGETNTEXIMAGEPROC __glewGetnTexImage;
GLEW_FUN_EXPORT PFNGLGETNUNIFORMDVPROC __glewGetnUniformdv;

GLEW_FUN_EXPORT PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC __glewMultiDrawArraysIndirectCount;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC __glewMultiDrawElementsIndirectCount;
GLEW_FUN_EXPORT PFNGLSPECIALIZESHADERPROC __glewSpecializeShader;

GLEW_FUN_EXPORT PFNGLSTENCILOPVALUEAMDPROC __glewStencilOpValueAMD;

GLEW_FUN_EXPORT PFNGLCLEARDEPTHFPROC __glewClearDepthf;
GLEW_FUN_EXPORT PFNGLDEPTHRANGEFPROC __glewDepthRangef;
GLEW_FUN_EXPORT PFNGLGETSHADERPRECISIONFORMATPROC __glewGetShaderPrecisionFormat;
GLEW_FUN_EXPORT PFNGLRELEASESHADERCOMPILERPROC __glewReleaseShaderCompiler;
GLEW_FUN_EXPORT PFNGLSHADERBINARYPROC __glewShaderBinary;

GLEW_FUN_EXPORT PFNGLMEMORYBARRIERBYREGIONPROC __glewMemoryBarrierByRegion;

GLEW_FUN_EXPORT PFNGLPRIMITIVEBOUNDINGBOXARBPROC __glewPrimitiveBoundingBoxARB;

GLEW_FUN_EXPORT PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC __glewDrawArraysInstancedBaseInstance;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC __glewDrawElementsInstancedBaseInstance;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC __glewDrawElementsInstancedBaseVertexBaseInstance;

GLEW_FUN_EXPORT PFNGLGETIMAGEHANDLEARBPROC __glewGetImageHandleARB;
GLEW_FUN_EXPORT PFNGLGETTEXTUREHANDLEARBPROC __glewGetTextureHandleARB;
GLEW_FUN_EXPORT PFNGLGETTEXTURESAMPLERHANDLEARBPROC __glewGetTextureSamplerHandleARB;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBLUI64VARBPROC __glewGetVertexAttribLui64vARB;
GLEW_FUN_EXPORT PFNGLISIMAGEHANDLERESIDENTARBPROC __glewIsImageHandleResidentARB;
GLEW_FUN_EXPORT PFNGLISTEXTUREHANDLERESIDENTARBPROC __glewIsTextureHandleResidentARB;
GLEW_FUN_EXPORT PFNGLMAKEIMAGEHANDLENONRESIDENTARBPROC __glewMakeImageHandleNonResidentARB;
GLEW_FUN_EXPORT PFNGLMAKEIMAGEHANDLERESIDENTARBPROC __glewMakeImageHandleResidentARB;
GLEW_FUN_EXPORT PFNGLMAKETEXTUREHANDLENONRESIDENTARBPROC __glewMakeTextureHandleNonResidentARB;
GLEW_FUN_EXPORT PFNGLMAKETEXTUREHANDLERESIDENTARBPROC __glewMakeTextureHandleResidentARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMHANDLEUI64ARBPROC __glewProgramUniformHandleui64ARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMHANDLEUI64VARBPROC __glewProgramUniformHandleui64vARB;
GLEW_FUN_EXPORT PFNGLUNIFORMHANDLEUI64ARBPROC __glewUniformHandleui64ARB;
GLEW_FUN_EXPORT PFNGLUNIFORMHANDLEUI64VARBPROC __glewUniformHandleui64vARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1UI64ARBPROC __glewVertexAttribL1ui64ARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1UI64VARBPROC __glewVertexAttribL1ui64vARB;

GLEW_FUN_EXPORT PFNGLBINDFRAGDATALOCATIONINDEXEDPROC __glewBindFragDataLocationIndexed;
GLEW_FUN_EXPORT PFNGLGETFRAGDATAINDEXPROC __glewGetFragDataIndex;

GLEW_FUN_EXPORT PFNGLBUFFERSTORAGEPROC __glewBufferStorage;

GLEW_FUN_EXPORT PFNGLCREATESYNCFROMCLEVENTARBPROC __glewCreateSyncFromCLeventARB;

GLEW_FUN_EXPORT PFNGLCLEARBUFFERDATAPROC __glewClearBufferData;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERSUBDATAPROC __glewClearBufferSubData;
GLEW_FUN_EXPORT PFNGLCLEARNAMEDBUFFERDATAEXTPROC __glewClearNamedBufferDataEXT;
GLEW_FUN_EXPORT PFNGLCLEARNAMEDBUFFERSUBDATAEXTPROC __glewClearNamedBufferSubDataEXT;

GLEW_FUN_EXPORT PFNGLCLEARTEXIMAGEPROC __glewClearTexImage;
GLEW_FUN_EXPORT PFNGLCLEARTEXSUBIMAGEPROC __glewClearTexSubImage;

GLEW_FUN_EXPORT PFNGLCLIPCONTROLPROC __glewClipControl;

GLEW_FUN_EXPORT PFNGLCLAMPCOLORARBPROC __glewClampColorARB;

GLEW_FUN_EXPORT PFNGLDISPATCHCOMPUTEPROC __glewDispatchCompute;
GLEW_FUN_EXPORT PFNGLDISPATCHCOMPUTEINDIRECTPROC __glewDispatchComputeIndirect;

GLEW_FUN_EXPORT PFNGLDISPATCHCOMPUTEGROUPSIZEARBPROC __glewDispatchComputeGroupSizeARB;

GLEW_FUN_EXPORT PFNGLCOPYBUFFERSUBDATAPROC __glewCopyBufferSubData;

GLEW_FUN_EXPORT PFNGLCOPYIMAGESUBDATAPROC __glewCopyImageSubData;

GLEW_FUN_EXPORT PFNGLDEBUGMESSAGECALLBACKARBPROC __glewDebugMessageCallbackARB;
GLEW_FUN_EXPORT PFNGLDEBUGMESSAGECONTROLARBPROC __glewDebugMessageControlARB;
GLEW_FUN_EXPORT PFNGLDEBUGMESSAGEINSERTARBPROC __glewDebugMessageInsertARB;
GLEW_FUN_EXPORT PFNGLGETDEBUGMESSAGELOGARBPROC __glewGetDebugMessageLogARB;

GLEW_FUN_EXPORT PFNGLBINDTEXTUREUNITPROC __glewBindTextureUnit;
GLEW_FUN_EXPORT PFNGLBLITNAMEDFRAMEBUFFERPROC __glewBlitNamedFramebuffer;
GLEW_FUN_EXPORT PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC __glewCheckNamedFramebufferStatus;
GLEW_FUN_EXPORT PFNGLCLEARNAMEDBUFFERDATAPROC __glewClearNamedBufferData;
GLEW_FUN_EXPORT PFNGLCLEARNAMEDBUFFERSUBDATAPROC __glewClearNamedBufferSubData;
GLEW_FUN_EXPORT PFNGLCLEARNAMEDFRAMEBUFFERFIPROC __glewClearNamedFramebufferfi;
GLEW_FUN_EXPORT PFNGLCLEARNAMEDFRAMEBUFFERFVPROC __glewClearNamedFramebufferfv;
GLEW_FUN_EXPORT PFNGLCLEARNAMEDFRAMEBUFFERIVPROC __glewClearNamedFramebufferiv;
GLEW_FUN_EXPORT PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC __glewClearNamedFramebufferuiv;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC __glewCompressedTextureSubImage1D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC __glewCompressedTextureSubImage2D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC __glewCompressedTextureSubImage3D;
GLEW_FUN_EXPORT PFNGLCOPYNAMEDBUFFERSUBDATAPROC __glewCopyNamedBufferSubData;
GLEW_FUN_EXPORT PFNGLCOPYTEXTURESUBIMAGE1DPROC __glewCopyTextureSubImage1D;
GLEW_FUN_EXPORT PFNGLCOPYTEXTURESUBIMAGE2DPROC __glewCopyTextureSubImage2D;
GLEW_FUN_EXPORT PFNGLCOPYTEXTURESUBIMAGE3DPROC __glewCopyTextureSubImage3D;
GLEW_FUN_EXPORT PFNGLCREATEBUFFERSPROC __glewCreateBuffers;
GLEW_FUN_EXPORT PFNGLCREATEFRAMEBUFFERSPROC __glewCreateFramebuffers;
GLEW_FUN_EXPORT PFNGLCREATEPROGRAMPIPELINESPROC __glewCreateProgramPipelines;
GLEW_FUN_EXPORT PFNGLCREATEQUERIESPROC __glewCreateQueries;
GLEW_FUN_EXPORT PFNGLCREATERENDERBUFFERSPROC __glewCreateRenderbuffers;
GLEW_FUN_EXPORT PFNGLCREATESAMPLERSPROC __glewCreateSamplers;
GLEW_FUN_EXPORT PFNGLCREATETEXTURESPROC __glewCreateTextures;
GLEW_FUN_EXPORT PFNGLCREATETRANSFORMFEEDBACKSPROC __glewCreateTransformFeedbacks;
GLEW_FUN_EXPORT PFNGLCREATEVERTEXARRAYSPROC __glewCreateVertexArrays;
GLEW_FUN_EXPORT PFNGLDISABLEVERTEXARRAYATTRIBPROC __glewDisableVertexArrayAttrib;
GLEW_FUN_EXPORT PFNGLENABLEVERTEXARRAYATTRIBPROC __glewEnableVertexArrayAttrib;
GLEW_FUN_EXPORT PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC __glewFlushMappedNamedBufferRange;
GLEW_FUN_EXPORT PFNGLGENERATETEXTUREMIPMAPPROC __glewGenerateTextureMipmap;
GLEW_FUN_EXPORT PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC __glewGetCompressedTextureImage;
GLEW_FUN_EXPORT PFNGLGETNAMEDBUFFERPARAMETERI64VPROC __glewGetNamedBufferParameteri64v;
GLEW_FUN_EXPORT PFNGLGETNAMEDBUFFERPARAMETERIVPROC __glewGetNamedBufferParameteriv;
GLEW_FUN_EXPORT PFNGLGETNAMEDBUFFERPOINTERVPROC __glewGetNamedBufferPointerv;
GLEW_FUN_EXPORT PFNGLGETNAMEDBUFFERSUBDATAPROC __glewGetNamedBufferSubData;
GLEW_FUN_EXPORT PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC __glewGetNamedFramebufferAttachmentParameteriv;
GLEW_FUN_EXPORT PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC __glewGetNamedFramebufferParameteriv;
GLEW_FUN_EXPORT PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC __glewGetNamedRenderbufferParameteriv;
GLEW_FUN_EXPORT PFNGLGETQUERYBUFFEROBJECTI64VPROC __glewGetQueryBufferObjecti64v;
GLEW_FUN_EXPORT PFNGLGETQUERYBUFFEROBJECTIVPROC __glewGetQueryBufferObjectiv;
GLEW_FUN_EXPORT PFNGLGETQUERYBUFFEROBJECTUI64VPROC __glewGetQueryBufferObjectui64v;
GLEW_FUN_EXPORT PFNGLGETQUERYBUFFEROBJECTUIVPROC __glewGetQueryBufferObjectuiv;
GLEW_FUN_EXPORT PFNGLGETTEXTUREIMAGEPROC __glewGetTextureImage;
GLEW_FUN_EXPORT PFNGLGETTEXTURELEVELPARAMETERFVPROC __glewGetTextureLevelParameterfv;
GLEW_FUN_EXPORT PFNGLGETTEXTURELEVELPARAMETERIVPROC __glewGetTextureLevelParameteriv;
GLEW_FUN_EXPORT PFNGLGETTEXTUREPARAMETERIIVPROC __glewGetTextureParameterIiv;
GLEW_FUN_EXPORT PFNGLGETTEXTUREPARAMETERIUIVPROC __glewGetTextureParameterIuiv;
GLEW_FUN_EXPORT PFNGLGETTEXTUREPARAMETERFVPROC __glewGetTextureParameterfv;
GLEW_FUN_EXPORT PFNGLGETTEXTUREPARAMETERIVPROC __glewGetTextureParameteriv;
GLEW_FUN_EXPORT PFNGLGETTRANSFORMFEEDBACKI64_VPROC __glewGetTransformFeedbacki64_v;
GLEW_FUN_EXPORT PFNGLGETTRANSFORMFEEDBACKI_VPROC __glewGetTransformFeedbacki_v;
GLEW_FUN_EXPORT PFNGLGETTRANSFORMFEEDBACKIVPROC __glewGetTransformFeedbackiv;
GLEW_FUN_EXPORT PFNGLGETVERTEXARRAYINDEXED64IVPROC __glewGetVertexArrayIndexed64iv;
GLEW_FUN_EXPORT PFNGLGETVERTEXARRAYINDEXEDIVPROC __glewGetVertexArrayIndexediv;
GLEW_FUN_EXPORT PFNGLGETVERTEXARRAYIVPROC __glewGetVertexArrayiv;
GLEW_FUN_EXPORT PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC __glewInvalidateNamedFramebufferData;
GLEW_FUN_EXPORT PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC __glewInvalidateNamedFramebufferSubData;
GLEW_FUN_EXPORT PFNGLMAPNAMEDBUFFERPROC __glewMapNamedBuffer;
GLEW_FUN_EXPORT PFNGLMAPNAMEDBUFFERRANGEPROC __glewMapNamedBufferRange;
GLEW_FUN_EXPORT PFNGLNAMEDBUFFERDATAPROC __glewNamedBufferData;
GLEW_FUN_EXPORT PFNGLNAMEDBUFFERSTORAGEPROC __glewNamedBufferStorage;
GLEW_FUN_EXPORT PFNGLNAMEDBUFFERSUBDATAPROC __glewNamedBufferSubData;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC __glewNamedFramebufferDrawBuffer;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC __glewNamedFramebufferDrawBuffers;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC __glewNamedFramebufferParameteri;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC __glewNamedFramebufferReadBuffer;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC __glewNamedFramebufferRenderbuffer;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTUREPROC __glewNamedFramebufferTexture;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC __glewNamedFramebufferTextureLayer;
GLEW_FUN_EXPORT PFNGLNAMEDRENDERBUFFERSTORAGEPROC __glewNamedRenderbufferStorage;
GLEW_FUN_EXPORT PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC __glewNamedRenderbufferStorageMultisample;
GLEW_FUN_EXPORT PFNGLTEXTUREBUFFERPROC __glewTextureBuffer;
GLEW_FUN_EXPORT PFNGLTEXTUREBUFFERRANGEPROC __glewTextureBufferRange;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERIIVPROC __glewTextureParameterIiv;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERIUIVPROC __glewTextureParameterIuiv;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERFPROC __glewTextureParameterf;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERFVPROC __glewTextureParameterfv;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERIPROC __glewTextureParameteri;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERIVPROC __glewTextureParameteriv;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE1DPROC __glewTextureStorage1D;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE2DPROC __glewTextureStorage2D;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC __glewTextureStorage2DMultisample;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE3DPROC __glewTextureStorage3D;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC __glewTextureStorage3DMultisample;
GLEW_FUN_EXPORT PFNGLTEXTURESUBIMAGE1DPROC __glewTextureSubImage1D;
GLEW_FUN_EXPORT PFNGLTEXTURESUBIMAGE2DPROC __glewTextureSubImage2D;
GLEW_FUN_EXPORT PFNGLTEXTURESUBIMAGE3DPROC __glewTextureSubImage3D;
GLEW_FUN_EXPORT PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC __glewTransformFeedbackBufferBase;
GLEW_FUN_EXPORT PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC __glewTransformFeedbackBufferRange;
GLEW_FUN_EXPORT PFNGLUNMAPNAMEDBUFFERPROC __glewUnmapNamedBuffer;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYATTRIBBINDINGPROC __glewVertexArrayAttribBinding;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYATTRIBFORMATPROC __glewVertexArrayAttribFormat;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYATTRIBIFORMATPROC __glewVertexArrayAttribIFormat;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYATTRIBLFORMATPROC __glewVertexArrayAttribLFormat;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYBINDINGDIVISORPROC __glewVertexArrayBindingDivisor;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYELEMENTBUFFERPROC __glewVertexArrayElementBuffer;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXBUFFERPROC __glewVertexArrayVertexBuffer;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXBUFFERSPROC __glewVertexArrayVertexBuffers;

GLEW_FUN_EXPORT PFNGLDRAWBUFFERSARBPROC __glewDrawBuffersARB;

GLEW_FUN_EXPORT PFNGLBLENDEQUATIONSEPARATEIARBPROC __glewBlendEquationSeparateiARB;
GLEW_FUN_EXPORT PFNGLBLENDEQUATIONIARBPROC __glewBlendEquationiARB;
GLEW_FUN_EXPORT PFNGLBLENDFUNCSEPARATEIARBPROC __glewBlendFuncSeparateiARB;
GLEW_FUN_EXPORT PFNGLBLENDFUNCIARBPROC __glewBlendFunciARB;

GLEW_FUN_EXPORT PFNGLDRAWELEMENTSBASEVERTEXPROC __glewDrawElementsBaseVertex;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC __glewDrawElementsInstancedBaseVertex;
GLEW_FUN_EXPORT PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC __glewDrawRangeElementsBaseVertex;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC __glewMultiDrawElementsBaseVertex;

GLEW_FUN_EXPORT PFNGLDRAWARRAYSINDIRECTPROC __glewDrawArraysIndirect;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINDIRECTPROC __glewDrawElementsIndirect;

GLEW_FUN_EXPORT PFNGLFRAMEBUFFERPARAMETERIPROC __glewFramebufferParameteri;
GLEW_FUN_EXPORT PFNGLGETFRAMEBUFFERPARAMETERIVPROC __glewGetFramebufferParameteriv;
GLEW_FUN_EXPORT PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVEXTPROC __glewGetNamedFramebufferParameterivEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERPARAMETERIEXTPROC __glewNamedFramebufferParameteriEXT;

GLEW_FUN_EXPORT PFNGLBINDFRAMEBUFFERPROC __glewBindFramebuffer;
GLEW_FUN_EXPORT PFNGLBINDRENDERBUFFERPROC __glewBindRenderbuffer;
GLEW_FUN_EXPORT PFNGLBLITFRAMEBUFFERPROC __glewBlitFramebuffer;
GLEW_FUN_EXPORT PFNGLCHECKFRAMEBUFFERSTATUSPROC __glewCheckFramebufferStatus;
GLEW_FUN_EXPORT PFNGLDELETEFRAMEBUFFERSPROC __glewDeleteFramebuffers;
GLEW_FUN_EXPORT PFNGLDELETERENDERBUFFERSPROC __glewDeleteRenderbuffers;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERRENDERBUFFERPROC __glewFramebufferRenderbuffer;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE1DPROC __glewFramebufferTexture1D;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE2DPROC __glewFramebufferTexture2D;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE3DPROC __glewFramebufferTexture3D;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURELAYERPROC __glewFramebufferTextureLayer;
GLEW_FUN_EXPORT PFNGLGENFRAMEBUFFERSPROC __glewGenFramebuffers;
GLEW_FUN_EXPORT PFNGLGENRENDERBUFFERSPROC __glewGenRenderbuffers;
GLEW_FUN_EXPORT PFNGLGENERATEMIPMAPPROC __glewGenerateMipmap;
GLEW_FUN_EXPORT PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC __glewGetFramebufferAttachmentParameteriv;
GLEW_FUN_EXPORT PFNGLGETRENDERBUFFERPARAMETERIVPROC __glewGetRenderbufferParameteriv;
GLEW_FUN_EXPORT PFNGLISFRAMEBUFFERPROC __glewIsFramebuffer;
GLEW_FUN_EXPORT PFNGLISRENDERBUFFERPROC __glewIsRenderbuffer;
GLEW_FUN_EXPORT PFNGLRENDERBUFFERSTORAGEPROC __glewRenderbufferStorage;
GLEW_FUN_EXPORT PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC __glewRenderbufferStorageMultisample;

GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTUREARBPROC __glewFramebufferTextureARB;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTUREFACEARBPROC __glewFramebufferTextureFaceARB;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURELAYERARBPROC __glewFramebufferTextureLayerARB;
GLEW_FUN_EXPORT PFNGLPROGRAMPARAMETERIARBPROC __glewProgramParameteriARB;

GLEW_FUN_EXPORT PFNGLGETPROGRAMBINARYPROC __glewGetProgramBinary;
GLEW_FUN_EXPORT PFNGLPROGRAMBINARYPROC __glewProgramBinary;
GLEW_FUN_EXPORT PFNGLPROGRAMPARAMETERIPROC __glewProgramParameteri;

GLEW_FUN_EXPORT PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC __glewGetCompressedTextureSubImage;
GLEW_FUN_EXPORT PFNGLGETTEXTURESUBIMAGEPROC __glewGetTextureSubImage;

GLEW_FUN_EXPORT PFNGLSPECIALIZESHADERARBPROC __glewSpecializeShaderARB;

GLEW_FUN_EXPORT PFNGLGETUNIFORMDVPROC __glewGetUniformdv;
GLEW_FUN_EXPORT PFNGLUNIFORM1DPROC __glewUniform1d;
GLEW_FUN_EXPORT PFNGLUNIFORM1DVPROC __glewUniform1dv;
GLEW_FUN_EXPORT PFNGLUNIFORM2DPROC __glewUniform2d;
GLEW_FUN_EXPORT PFNGLUNIFORM2DVPROC __glewUniform2dv;
GLEW_FUN_EXPORT PFNGLUNIFORM3DPROC __glewUniform3d;
GLEW_FUN_EXPORT PFNGLUNIFORM3DVPROC __glewUniform3dv;
GLEW_FUN_EXPORT PFNGLUNIFORM4DPROC __glewUniform4d;
GLEW_FUN_EXPORT PFNGLUNIFORM4DVPROC __glewUniform4dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2DVPROC __glewUniformMatrix2dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2X3DVPROC __glewUniformMatrix2x3dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2X4DVPROC __glewUniformMatrix2x4dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3DVPROC __glewUniformMatrix3dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3X2DVPROC __glewUniformMatrix3x2dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3X4DVPROC __glewUniformMatrix3x4dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4DVPROC __glewUniformMatrix4dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4X2DVPROC __glewUniformMatrix4x2dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4X3DVPROC __glewUniformMatrix4x3dv;

GLEW_FUN_EXPORT PFNGLGETUNIFORMI64VARBPROC __glewGetUniformi64vARB;
GLEW_FUN_EXPORT PFNGLGETUNIFORMUI64VARBPROC __glewGetUniformui64vARB;
GLEW_FUN_EXPORT PFNGLGETNUNIFORMI64VARBPROC __glewGetnUniformi64vARB;
GLEW_FUN_EXPORT PFNGLGETNUNIFORMUI64VARBPROC __glewGetnUniformui64vARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1I64ARBPROC __glewProgramUniform1i64ARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1I64VARBPROC __glewProgramUniform1i64vARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UI64ARBPROC __glewProgramUniform1ui64ARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UI64VARBPROC __glewProgramUniform1ui64vARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2I64ARBPROC __glewProgramUniform2i64ARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2I64VARBPROC __glewProgramUniform2i64vARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UI64ARBPROC __glewProgramUniform2ui64ARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UI64VARBPROC __glewProgramUniform2ui64vARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3I64ARBPROC __glewProgramUniform3i64ARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3I64VARBPROC __glewProgramUniform3i64vARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UI64ARBPROC __glewProgramUniform3ui64ARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UI64VARBPROC __glewProgramUniform3ui64vARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4I64ARBPROC __glewProgramUniform4i64ARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4I64VARBPROC __glewProgramUniform4i64vARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UI64ARBPROC __glewProgramUniform4ui64ARB;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UI64VARBPROC __glewProgramUniform4ui64vARB;
GLEW_FUN_EXPORT PFNGLUNIFORM1I64ARBPROC __glewUniform1i64ARB;
GLEW_FUN_EXPORT PFNGLUNIFORM1I64VARBPROC __glewUniform1i64vARB;
GLEW_FUN_EXPORT PFNGLUNIFORM1UI64ARBPROC __glewUniform1ui64ARB;
GLEW_FUN_EXPORT PFNGLUNIFORM1UI64VARBPROC __glewUniform1ui64vARB;
GLEW_FUN_EXPORT PFNGLUNIFORM2I64ARBPROC __glewUniform2i64ARB;
GLEW_FUN_EXPORT PFNGLUNIFORM2I64VARBPROC __glewUniform2i64vARB;
GLEW_FUN_EXPORT PFNGLUNIFORM2UI64ARBPROC __glewUniform2ui64ARB;
GLEW_FUN_EXPORT PFNGLUNIFORM2UI64VARBPROC __glewUniform2ui64vARB;
GLEW_FUN_EXPORT PFNGLUNIFORM3I64ARBPROC __glewUniform3i64ARB;
GLEW_FUN_EXPORT PFNGLUNIFORM3I64VARBPROC __glewUniform3i64vARB;
GLEW_FUN_EXPORT PFNGLUNIFORM3UI64ARBPROC __glewUniform3ui64ARB;
GLEW_FUN_EXPORT PFNGLUNIFORM3UI64VARBPROC __glewUniform3ui64vARB;
GLEW_FUN_EXPORT PFNGLUNIFORM4I64ARBPROC __glewUniform4i64ARB;
GLEW_FUN_EXPORT PFNGLUNIFORM4I64VARBPROC __glewUniform4i64vARB;
GLEW_FUN_EXPORT PFNGLUNIFORM4UI64ARBPROC __glewUniform4ui64ARB;
GLEW_FUN_EXPORT PFNGLUNIFORM4UI64VARBPROC __glewUniform4ui64vARB;

GLEW_FUN_EXPORT PFNGLCOLORSUBTABLEPROC __glewColorSubTable;
GLEW_FUN_EXPORT PFNGLCOLORTABLEPROC __glewColorTable;
GLEW_FUN_EXPORT PFNGLCOLORTABLEPARAMETERFVPROC __glewColorTableParameterfv;
GLEW_FUN_EXPORT PFNGLCOLORTABLEPARAMETERIVPROC __glewColorTableParameteriv;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONFILTER1DPROC __glewConvolutionFilter1D;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONFILTER2DPROC __glewConvolutionFilter2D;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERFPROC __glewConvolutionParameterf;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERFVPROC __glewConvolutionParameterfv;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERIPROC __glewConvolutionParameteri;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERIVPROC __glewConvolutionParameteriv;
GLEW_FUN_EXPORT PFNGLCOPYCOLORSUBTABLEPROC __glewCopyColorSubTable;
GLEW_FUN_EXPORT PFNGLCOPYCOLORTABLEPROC __glewCopyColorTable;
GLEW_FUN_EXPORT PFNGLCOPYCONVOLUTIONFILTER1DPROC __glewCopyConvolutionFilter1D;
GLEW_FUN_EXPORT PFNGLCOPYCONVOLUTIONFILTER2DPROC __glewCopyConvolutionFilter2D;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEPROC __glewGetColorTable;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEPARAMETERFVPROC __glewGetColorTableParameterfv;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEPARAMETERIVPROC __glewGetColorTableParameteriv;
GLEW_FUN_EXPORT PFNGLGETCONVOLUTIONFILTERPROC __glewGetConvolutionFilter;
GLEW_FUN_EXPORT PFNGLGETCONVOLUTIONPARAMETERFVPROC __glewGetConvolutionParameterfv;
GLEW_FUN_EXPORT PFNGLGETCONVOLUTIONPARAMETERIVPROC __glewGetConvolutionParameteriv;
GLEW_FUN_EXPORT PFNGLGETHISTOGRAMPROC __glewGetHistogram;
GLEW_FUN_EXPORT PFNGLGETHISTOGRAMPARAMETERFVPROC __glewGetHistogramParameterfv;
GLEW_FUN_EXPORT PFNGLGETHISTOGRAMPARAMETERIVPROC __glewGetHistogramParameteriv;
GLEW_FUN_EXPORT PFNGLGETMINMAXPROC __glewGetMinmax;
GLEW_FUN_EXPORT PFNGLGETMINMAXPARAMETERFVPROC __glewGetMinmaxParameterfv;
GLEW_FUN_EXPORT PFNGLGETMINMAXPARAMETERIVPROC __glewGetMinmaxParameteriv;
GLEW_FUN_EXPORT PFNGLGETSEPARABLEFILTERPROC __glewGetSeparableFilter;
GLEW_FUN_EXPORT PFNGLHISTOGRAMPROC __glewHistogram;
GLEW_FUN_EXPORT PFNGLMINMAXPROC __glewMinmax;
GLEW_FUN_EXPORT PFNGLRESETHISTOGRAMPROC __glewResetHistogram;
GLEW_FUN_EXPORT PFNGLRESETMINMAXPROC __glewResetMinmax;
GLEW_FUN_EXPORT PFNGLSEPARABLEFILTER2DPROC __glewSeparableFilter2D;

GLEW_FUN_EXPORT PFNGLMULTIDRAWARRAYSINDIRECTCOUNTARBPROC __glewMultiDrawArraysIndirectCountARB;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTARBPROC __glewMultiDrawElementsIndirectCountARB;

GLEW_FUN_EXPORT PFNGLDRAWARRAYSINSTANCEDARBPROC __glewDrawArraysInstancedARB;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDARBPROC __glewDrawElementsInstancedARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBDIVISORARBPROC __glewVertexAttribDivisorARB;

GLEW_FUN_EXPORT PFNGLGETINTERNALFORMATIVPROC __glewGetInternalformativ;

GLEW_FUN_EXPORT PFNGLGETINTERNALFORMATI64VPROC __glewGetInternalformati64v;

GLEW_FUN_EXPORT PFNGLINVALIDATEBUFFERDATAPROC __glewInvalidateBufferData;
GLEW_FUN_EXPORT PFNGLINVALIDATEBUFFERSUBDATAPROC __glewInvalidateBufferSubData;
GLEW_FUN_EXPORT PFNGLINVALIDATEFRAMEBUFFERPROC __glewInvalidateFramebuffer;
GLEW_FUN_EXPORT PFNGLINVALIDATESUBFRAMEBUFFERPROC __glewInvalidateSubFramebuffer;
GLEW_FUN_EXPORT PFNGLINVALIDATETEXIMAGEPROC __glewInvalidateTexImage;
GLEW_FUN_EXPORT PFNGLINVALIDATETEXSUBIMAGEPROC __glewInvalidateTexSubImage;

GLEW_FUN_EXPORT PFNGLFLUSHMAPPEDBUFFERRANGEPROC __glewFlushMappedBufferRange;
GLEW_FUN_EXPORT PFNGLMAPBUFFERRANGEPROC __glewMapBufferRange;

GLEW_FUN_EXPORT PFNGLCURRENTPALETTEMATRIXARBPROC __glewCurrentPaletteMatrixARB;
GLEW_FUN_EXPORT PFNGLMATRIXINDEXPOINTERARBPROC __glewMatrixIndexPointerARB;
GLEW_FUN_EXPORT PFNGLMATRIXINDEXUBVARBPROC __glewMatrixIndexubvARB;
GLEW_FUN_EXPORT PFNGLMATRIXINDEXUIVARBPROC __glewMatrixIndexuivARB;
GLEW_FUN_EXPORT PFNGLMATRIXINDEXUSVARBPROC __glewMatrixIndexusvARB;

GLEW_FUN_EXPORT PFNGLBINDBUFFERSBASEPROC __glewBindBuffersBase;
GLEW_FUN_EXPORT PFNGLBINDBUFFERSRANGEPROC __glewBindBuffersRange;
GLEW_FUN_EXPORT PFNGLBINDIMAGETEXTURESPROC __glewBindImageTextures;
GLEW_FUN_EXPORT PFNGLBINDSAMPLERSPROC __glewBindSamplers;
GLEW_FUN_EXPORT PFNGLBINDTEXTURESPROC __glewBindTextures;
GLEW_FUN_EXPORT PFNGLBINDVERTEXBUFFERSPROC __glewBindVertexBuffers;

GLEW_FUN_EXPORT PFNGLMULTIDRAWARRAYSINDIRECTPROC __glewMultiDrawArraysIndirect;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTSINDIRECTPROC __glewMultiDrawElementsIndirect;

GLEW_FUN_EXPORT PFNGLSAMPLECOVERAGEARBPROC __glewSampleCoverageARB;

GLEW_FUN_EXPORT PFNGLACTIVETEXTUREARBPROC __glewActiveTextureARB;
GLEW_FUN_EXPORT PFNGLCLIENTACTIVETEXTUREARBPROC __glewClientActiveTextureARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1DARBPROC __glewMultiTexCoord1dARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1DVARBPROC __glewMultiTexCoord1dvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1FARBPROC __glewMultiTexCoord1fARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1FVARBPROC __glewMultiTexCoord1fvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1IARBPROC __glewMultiTexCoord1iARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1IVARBPROC __glewMultiTexCoord1ivARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1SARBPROC __glewMultiTexCoord1sARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1SVARBPROC __glewMultiTexCoord1svARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2DARBPROC __glewMultiTexCoord2dARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2DVARBPROC __glewMultiTexCoord2dvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2FARBPROC __glewMultiTexCoord2fARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2FVARBPROC __glewMultiTexCoord2fvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2IARBPROC __glewMultiTexCoord2iARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2IVARBPROC __glewMultiTexCoord2ivARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2SARBPROC __glewMultiTexCoord2sARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2SVARBPROC __glewMultiTexCoord2svARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3DARBPROC __glewMultiTexCoord3dARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3DVARBPROC __glewMultiTexCoord3dvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3FARBPROC __glewMultiTexCoord3fARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3FVARBPROC __glewMultiTexCoord3fvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3IARBPROC __glewMultiTexCoord3iARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3IVARBPROC __glewMultiTexCoord3ivARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3SARBPROC __glewMultiTexCoord3sARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3SVARBPROC __glewMultiTexCoord3svARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4DARBPROC __glewMultiTexCoord4dARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4DVARBPROC __glewMultiTexCoord4dvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4FARBPROC __glewMultiTexCoord4fARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4FVARBPROC __glewMultiTexCoord4fvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4IARBPROC __glewMultiTexCoord4iARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4IVARBPROC __glewMultiTexCoord4ivARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4SARBPROC __glewMultiTexCoord4sARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4SVARBPROC __glewMultiTexCoord4svARB;

GLEW_FUN_EXPORT PFNGLBEGINQUERYARBPROC __glewBeginQueryARB;
GLEW_FUN_EXPORT PFNGLDELETEQUERIESARBPROC __glewDeleteQueriesARB;
GLEW_FUN_EXPORT PFNGLENDQUERYARBPROC __glewEndQueryARB;
GLEW_FUN_EXPORT PFNGLGENQUERIESARBPROC __glewGenQueriesARB;
GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTIVARBPROC __glewGetQueryObjectivARB;
GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTUIVARBPROC __glewGetQueryObjectuivARB;
GLEW_FUN_EXPORT PFNGLGETQUERYIVARBPROC __glewGetQueryivARB;
GLEW_FUN_EXPORT PFNGLISQUERYARBPROC __glewIsQueryARB;

GLEW_FUN_EXPORT PFNGLMAXSHADERCOMPILERTHREADSARBPROC __glewMaxShaderCompilerThreadsARB;

GLEW_FUN_EXPORT PFNGLPOINTPARAMETERFARBPROC __glewPointParameterfARB;
GLEW_FUN_EXPORT PFNGLPOINTPARAMETERFVARBPROC __glewPointParameterfvARB;

GLEW_FUN_EXPORT PFNGLPOLYGONOFFSETCLAMPPROC __glewPolygonOffsetClamp;

GLEW_FUN_EXPORT PFNGLGETPROGRAMINTERFACEIVPROC __glewGetProgramInterfaceiv;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCEINDEXPROC __glewGetProgramResourceIndex;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCELOCATIONPROC __glewGetProgramResourceLocation;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC __glewGetProgramResourceLocationIndex;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCENAMEPROC __glewGetProgramResourceName;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCEIVPROC __glewGetProgramResourceiv;

GLEW_FUN_EXPORT PFNGLPROVOKINGVERTEXPROC __glewProvokingVertex;

GLEW_FUN_EXPORT PFNGLGETGRAPHICSRESETSTATUSARBPROC __glewGetGraphicsResetStatusARB;
GLEW_FUN_EXPORT PFNGLGETNCOLORTABLEARBPROC __glewGetnColorTableARB;
GLEW_FUN_EXPORT PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC __glewGetnCompressedTexImageARB;
GLEW_FUN_EXPORT PFNGLGETNCONVOLUTIONFILTERARBPROC __glewGetnConvolutionFilterARB;
GLEW_FUN_EXPORT PFNGLGETNHISTOGRAMARBPROC __glewGetnHistogramARB;
GLEW_FUN_EXPORT PFNGLGETNMAPDVARBPROC __glewGetnMapdvARB;
GLEW_FUN_EXPORT PFNGLGETNMAPFVARBPROC __glewGetnMapfvARB;
GLEW_FUN_EXPORT PFNGLGETNMAPIVARBPROC __glewGetnMapivARB;
GLEW_FUN_EXPORT PFNGLGETNMINMAXARBPROC __glewGetnMinmaxARB;
GLEW_FUN_EXPORT PFNGLGETNPIXELMAPFVARBPROC __glewGetnPixelMapfvARB;
GLEW_FUN_EXPORT PFNGLGETNPIXELMAPUIVARBPROC __glewGetnPixelMapuivARB;
GLEW_FUN_EXPORT PFNGLGETNPIXELMAPUSVARBPROC __glewGetnPixelMapusvARB;
GLEW_FUN_EXPORT PFNGLGETNPOLYGONSTIPPLEARBPROC __glewGetnPolygonStippleARB;
GLEW_FUN_EXPORT PFNGLGETNSEPARABLEFILTERARBPROC __glewGetnSeparableFilterARB;
GLEW_FUN_EXPORT PFNGLGETNTEXIMAGEARBPROC __glewGetnTexImageARB;
GLEW_FUN_EXPORT PFNGLGETNUNIFORMDVARBPROC __glewGetnUniformdvARB;
GLEW_FUN_EXPORT PFNGLGETNUNIFORMFVARBPROC __glewGetnUniformfvARB;
GLEW_FUN_EXPORT PFNGLGETNUNIFORMIVARBPROC __glewGetnUniformivARB;
GLEW_FUN_EXPORT PFNGLGETNUNIFORMUIVARBPROC __glewGetnUniformuivARB;
GLEW_FUN_EXPORT PFNGLREADNPIXELSARBPROC __glewReadnPixelsARB;

GLEW_FUN_EXPORT PFNGLFRAMEBUFFERSAMPLELOCATIONSFVARBPROC __glewFramebufferSampleLocationsfvARB;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERSAMPLELOCATIONSFVARBPROC __glewNamedFramebufferSampleLocationsfvARB;

GLEW_FUN_EXPORT PFNGLMINSAMPLESHADINGARBPROC __glewMinSampleShadingARB;

GLEW_FUN_EXPORT PFNGLBINDSAMPLERPROC __glewBindSampler;
GLEW_FUN_EXPORT PFNGLDELETESAMPLERSPROC __glewDeleteSamplers;
GLEW_FUN_EXPORT PFNGLGENSAMPLERSPROC __glewGenSamplers;
GLEW_FUN_EXPORT PFNGLGETSAMPLERPARAMETERIIVPROC __glewGetSamplerParameterIiv;
GLEW_FUN_EXPORT PFNGLGETSAMPLERPARAMETERIUIVPROC __glewGetSamplerParameterIuiv;
GLEW_FUN_EXPORT PFNGLGETSAMPLERPARAMETERFVPROC __glewGetSamplerParameterfv;
GLEW_FUN_EXPORT PFNGLGETSAMPLERPARAMETERIVPROC __glewGetSamplerParameteriv;
GLEW_FUN_EXPORT PFNGLISSAMPLERPROC __glewIsSampler;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERIIVPROC __glewSamplerParameterIiv;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERIUIVPROC __glewSamplerParameterIuiv;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERFPROC __glewSamplerParameterf;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERFVPROC __glewSamplerParameterfv;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERIPROC __glewSamplerParameteri;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERIVPROC __glewSamplerParameteriv;

GLEW_FUN_EXPORT PFNGLACTIVESHADERPROGRAMPROC __glewActiveShaderProgram;
GLEW_FUN_EXPORT PFNGLBINDPROGRAMPIPELINEPROC __glewBindProgramPipeline;
GLEW_FUN_EXPORT PFNGLCREATESHADERPROGRAMVPROC __glewCreateShaderProgramv;
GLEW_FUN_EXPORT PFNGLDELETEPROGRAMPIPELINESPROC __glewDeleteProgramPipelines;
GLEW_FUN_EXPORT PFNGLGENPROGRAMPIPELINESPROC __glewGenProgramPipelines;
GLEW_FUN_EXPORT PFNGLGETPROGRAMPIPELINEINFOLOGPROC __glewGetProgramPipelineInfoLog;
GLEW_FUN_EXPORT PFNGLGETPROGRAMPIPELINEIVPROC __glewGetProgramPipelineiv;
GLEW_FUN_EXPORT PFNGLISPROGRAMPIPELINEPROC __glewIsProgramPipeline;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1DPROC __glewProgramUniform1d;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1DVPROC __glewProgramUniform1dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1FPROC __glewProgramUniform1f;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1FVPROC __glewProgramUniform1fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1IPROC __glewProgramUniform1i;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1IVPROC __glewProgramUniform1iv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UIPROC __glewProgramUniform1ui;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UIVPROC __glewProgramUniform1uiv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2DPROC __glewProgramUniform2d;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2DVPROC __glewProgramUniform2dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2FPROC __glewProgramUniform2f;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2FVPROC __glewProgramUniform2fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2IPROC __glewProgramUniform2i;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2IVPROC __glewProgramUniform2iv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UIPROC __glewProgramUniform2ui;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UIVPROC __glewProgramUniform2uiv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3DPROC __glewProgramUniform3d;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3DVPROC __glewProgramUniform3dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3FPROC __glewProgramUniform3f;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3FVPROC __glewProgramUniform3fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3IPROC __glewProgramUniform3i;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3IVPROC __glewProgramUniform3iv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UIPROC __glewProgramUniform3ui;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UIVPROC __glewProgramUniform3uiv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4DPROC __glewProgramUniform4d;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4DVPROC __glewProgramUniform4dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4FPROC __glewProgramUniform4f;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4FVPROC __glewProgramUniform4fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4IPROC __glewProgramUniform4i;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4IVPROC __glewProgramUniform4iv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UIPROC __glewProgramUniform4ui;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UIVPROC __glewProgramUniform4uiv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2DVPROC __glewProgramUniformMatrix2dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2FVPROC __glewProgramUniformMatrix2fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC __glewProgramUniformMatrix2x3dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC __glewProgramUniformMatrix2x3fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC __glewProgramUniformMatrix2x4dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC __glewProgramUniformMatrix2x4fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3DVPROC __glewProgramUniformMatrix3dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3FVPROC __glewProgramUniformMatrix3fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC __glewProgramUniformMatrix3x2dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC __glewProgramUniformMatrix3x2fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC __glewProgramUniformMatrix3x4dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC __glewProgramUniformMatrix3x4fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4DVPROC __glewProgramUniformMatrix4dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4FVPROC __glewProgramUniformMatrix4fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC __glewProgramUniformMatrix4x2dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC __glewProgramUniformMatrix4x2fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC __glewProgramUniformMatrix4x3dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC __glewProgramUniformMatrix4x3fv;
GLEW_FUN_EXPORT PFNGLUSEPROGRAMSTAGESPROC __glewUseProgramStages;
GLEW_FUN_EXPORT PFNGLVALIDATEPROGRAMPIPELINEPROC __glewValidateProgramPipeline;

GLEW_FUN_EXPORT PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC __glewGetActiveAtomicCounterBufferiv;

GLEW_FUN_EXPORT PFNGLBINDIMAGETEXTUREPROC __glewBindImageTexture;
GLEW_FUN_EXPORT PFNGLMEMORYBARRIERPROC __glewMemoryBarrier;

GLEW_FUN_EXPORT PFNGLATTACHOBJECTARBPROC __glewAttachObjectARB;
GLEW_FUN_EXPORT PFNGLCOMPILESHADERARBPROC __glewCompileShaderARB;
GLEW_FUN_EXPORT PFNGLCREATEPROGRAMOBJECTARBPROC __glewCreateProgramObjectARB;
GLEW_FUN_EXPORT PFNGLCREATESHADEROBJECTARBPROC __glewCreateShaderObjectARB;
GLEW_FUN_EXPORT PFNGLDELETEOBJECTARBPROC __glewDeleteObjectARB;
GLEW_FUN_EXPORT PFNGLDETACHOBJECTARBPROC __glewDetachObjectARB;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMARBPROC __glewGetActiveUniformARB;
GLEW_FUN_EXPORT PFNGLGETATTACHEDOBJECTSARBPROC __glewGetAttachedObjectsARB;
GLEW_FUN_EXPORT PFNGLGETHANDLEARBPROC __glewGetHandleARB;
GLEW_FUN_EXPORT PFNGLGETINFOLOGARBPROC __glewGetInfoLogARB;
GLEW_FUN_EXPORT PFNGLGETOBJECTPARAMETERFVARBPROC __glewGetObjectParameterfvARB;
GLEW_FUN_EXPORT PFNGLGETOBJECTPARAMETERIVARBPROC __glewGetObjectParameterivARB;
GLEW_FUN_EXPORT PFNGLGETSHADERSOURCEARBPROC __glewGetShaderSourceARB;
GLEW_FUN_EXPORT PFNGLGETUNIFORMLOCATIONARBPROC __glewGetUniformLocationARB;
GLEW_FUN_EXPORT PFNGLGETUNIFORMFVARBPROC __glewGetUniformfvARB;
GLEW_FUN_EXPORT PFNGLGETUNIFORMIVARBPROC __glewGetUniformivARB;
GLEW_FUN_EXPORT PFNGLLINKPROGRAMARBPROC __glewLinkProgramARB;
GLEW_FUN_EXPORT PFNGLSHADERSOURCEARBPROC __glewShaderSourceARB;
GLEW_FUN_EXPORT PFNGLUNIFORM1FARBPROC __glewUniform1fARB;
GLEW_FUN_EXPORT PFNGLUNIFORM1FVARBPROC __glewUniform1fvARB;
GLEW_FUN_EXPORT PFNGLUNIFORM1IARBPROC __glewUniform1iARB;
GLEW_FUN_EXPORT PFNGLUNIFORM1IVARBPROC __glewUniform1ivARB;
GLEW_FUN_EXPORT PFNGLUNIFORM2FARBPROC __glewUniform2fARB;
GLEW_FUN_EXPORT PFNGLUNIFORM2FVARBPROC __glewUniform2fvARB;
GLEW_FUN_EXPORT PFNGLUNIFORM2IARBPROC __glewUniform2iARB;
GLEW_FUN_EXPORT PFNGLUNIFORM2IVARBPROC __glewUniform2ivARB;
GLEW_FUN_EXPORT PFNGLUNIFORM3FARBPROC __glewUniform3fARB;
GLEW_FUN_EXPORT PFNGLUNIFORM3FVARBPROC __glewUniform3fvARB;
GLEW_FUN_EXPORT PFNGLUNIFORM3IARBPROC __glewUniform3iARB;
GLEW_FUN_EXPORT PFNGLUNIFORM3IVARBPROC __glewUniform3ivARB;
GLEW_FUN_EXPORT PFNGLUNIFORM4FARBPROC __glewUniform4fARB;
GLEW_FUN_EXPORT PFNGLUNIFORM4FVARBPROC __glewUniform4fvARB;
GLEW_FUN_EXPORT PFNGLUNIFORM4IARBPROC __glewUniform4iARB;
GLEW_FUN_EXPORT PFNGLUNIFORM4IVARBPROC __glewUniform4ivARB;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2FVARBPROC __glewUniformMatrix2fvARB;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3FVARBPROC __glewUniformMatrix3fvARB;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4FVARBPROC __glewUniformMatrix4fvARB;
GLEW_FUN_EXPORT PFNGLUSEPROGRAMOBJECTARBPROC __glewUseProgramObjectARB;
GLEW_FUN_EXPORT PFNGLVALIDATEPROGRAMARBPROC __glewValidateProgramARB;

GLEW_FUN_EXPORT PFNGLSHADERSTORAGEBLOCKBINDINGPROC __glewShaderStorageBlockBinding;

GLEW_FUN_EXPORT PFNGLGETACTIVESUBROUTINENAMEPROC __glewGetActiveSubroutineName;
GLEW_FUN_EXPORT PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC __glewGetActiveSubroutineUniformName;
GLEW_FUN_EXPORT PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC __glewGetActiveSubroutineUniformiv;
GLEW_FUN_EXPORT PFNGLGETPROGRAMSTAGEIVPROC __glewGetProgramStageiv;
GLEW_FUN_EXPORT PFNGLGETSUBROUTINEINDEXPROC __glewGetSubroutineIndex;
GLEW_FUN_EXPORT PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC __glewGetSubroutineUniformLocation;
GLEW_FUN_EXPORT PFNGLGETUNIFORMSUBROUTINEUIVPROC __glewGetUniformSubroutineuiv;
GLEW_FUN_EXPORT PFNGLUNIFORMSUBROUTINESUIVPROC __glewUniformSubroutinesuiv;

GLEW_FUN_EXPORT PFNGLCOMPILESHADERINCLUDEARBPROC __glewCompileShaderIncludeARB;
GLEW_FUN_EXPORT PFNGLDELETENAMEDSTRINGARBPROC __glewDeleteNamedStringARB;
GLEW_FUN_EXPORT PFNGLGETNAMEDSTRINGARBPROC __glewGetNamedStringARB;
GLEW_FUN_EXPORT PFNGLGETNAMEDSTRINGIVARBPROC __glewGetNamedStringivARB;
GLEW_FUN_EXPORT PFNGLISNAMEDSTRINGARBPROC __glewIsNamedStringARB;
GLEW_FUN_EXPORT PFNGLNAMEDSTRINGARBPROC __glewNamedStringARB;

GLEW_FUN_EXPORT PFNGLBUFFERPAGECOMMITMENTARBPROC __glewBufferPageCommitmentARB;

GLEW_FUN_EXPORT PFNGLTEXPAGECOMMITMENTARBPROC __glewTexPageCommitmentARB;

GLEW_FUN_EXPORT PFNGLCLIENTWAITSYNCPROC __glewClientWaitSync;
GLEW_FUN_EXPORT PFNGLDELETESYNCPROC __glewDeleteSync;
GLEW_FUN_EXPORT PFNGLFENCESYNCPROC __glewFenceSync;
GLEW_FUN_EXPORT PFNGLGETINTEGER64VPROC __glewGetInteger64v;
GLEW_FUN_EXPORT PFNGLGETSYNCIVPROC __glewGetSynciv;
GLEW_FUN_EXPORT PFNGLISSYNCPROC __glewIsSync;
GLEW_FUN_EXPORT PFNGLWAITSYNCPROC __glewWaitSync;

GLEW_FUN_EXPORT PFNGLPATCHPARAMETERFVPROC __glewPatchParameterfv;
GLEW_FUN_EXPORT PFNGLPATCHPARAMETERIPROC __glewPatchParameteri;

GLEW_FUN_EXPORT PFNGLTEXTUREBARRIERPROC __glewTextureBarrier;

GLEW_FUN_EXPORT PFNGLTEXBUFFERARBPROC __glewTexBufferARB;

GLEW_FUN_EXPORT PFNGLTEXBUFFERRANGEPROC __glewTexBufferRange;
GLEW_FUN_EXPORT PFNGLTEXTUREBUFFERRANGEEXTPROC __glewTextureBufferRangeEXT;

GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE1DARBPROC __glewCompressedTexImage1DARB;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE2DARBPROC __glewCompressedTexImage2DARB;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE3DARBPROC __glewCompressedTexImage3DARB;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC __glewCompressedTexSubImage1DARB;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC __glewCompressedTexSubImage2DARB;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC __glewCompressedTexSubImage3DARB;
GLEW_FUN_EXPORT PFNGLGETCOMPRESSEDTEXIMAGEARBPROC __glewGetCompressedTexImageARB;

GLEW_FUN_EXPORT PFNGLGETMULTISAMPLEFVPROC __glewGetMultisamplefv;
GLEW_FUN_EXPORT PFNGLSAMPLEMASKIPROC __glewSampleMaski;
GLEW_FUN_EXPORT PFNGLTEXIMAGE2DMULTISAMPLEPROC __glewTexImage2DMultisample;
GLEW_FUN_EXPORT PFNGLTEXIMAGE3DMULTISAMPLEPROC __glewTexImage3DMultisample;

GLEW_FUN_EXPORT PFNGLTEXSTORAGE1DPROC __glewTexStorage1D;
GLEW_FUN_EXPORT PFNGLTEXSTORAGE2DPROC __glewTexStorage2D;
GLEW_FUN_EXPORT PFNGLTEXSTORAGE3DPROC __glewTexStorage3D;

GLEW_FUN_EXPORT PFNGLTEXSTORAGE2DMULTISAMPLEPROC __glewTexStorage2DMultisample;
GLEW_FUN_EXPORT PFNGLTEXSTORAGE3DMULTISAMPLEPROC __glewTexStorage3DMultisample;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE2DMULTISAMPLEEXTPROC __glewTextureStorage2DMultisampleEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE3DMULTISAMPLEEXTPROC __glewTextureStorage3DMultisampleEXT;

GLEW_FUN_EXPORT PFNGLTEXTUREVIEWPROC __glewTextureView;

GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTI64VPROC __glewGetQueryObjecti64v;
GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTUI64VPROC __glewGetQueryObjectui64v;
GLEW_FUN_EXPORT PFNGLQUERYCOUNTERPROC __glewQueryCounter;

GLEW_FUN_EXPORT PFNGLBINDTRANSFORMFEEDBACKPROC __glewBindTransformFeedback;
GLEW_FUN_EXPORT PFNGLDELETETRANSFORMFEEDBACKSPROC __glewDeleteTransformFeedbacks;
GLEW_FUN_EXPORT PFNGLDRAWTRANSFORMFEEDBACKPROC __glewDrawTransformFeedback;
GLEW_FUN_EXPORT PFNGLGENTRANSFORMFEEDBACKSPROC __glewGenTransformFeedbacks;
GLEW_FUN_EXPORT PFNGLISTRANSFORMFEEDBACKPROC __glewIsTransformFeedback;
GLEW_FUN_EXPORT PFNGLPAUSETRANSFORMFEEDBACKPROC __glewPauseTransformFeedback;
GLEW_FUN_EXPORT PFNGLRESUMETRANSFORMFEEDBACKPROC __glewResumeTransformFeedback;

GLEW_FUN_EXPORT PFNGLBEGINQUERYINDEXEDPROC __glewBeginQueryIndexed;
GLEW_FUN_EXPORT PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC __glewDrawTransformFeedbackStream;
GLEW_FUN_EXPORT PFNGLENDQUERYINDEXEDPROC __glewEndQueryIndexed;
GLEW_FUN_EXPORT PFNGLGETQUERYINDEXEDIVPROC __glewGetQueryIndexediv;

GLEW_FUN_EXPORT PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC __glewDrawTransformFeedbackInstanced;
GLEW_FUN_EXPORT PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC __glewDrawTransformFeedbackStreamInstanced;

GLEW_FUN_EXPORT PFNGLLOADTRANSPOSEMATRIXDARBPROC __glewLoadTransposeMatrixdARB;
GLEW_FUN_EXPORT PFNGLLOADTRANSPOSEMATRIXFARBPROC __glewLoadTransposeMatrixfARB;
GLEW_FUN_EXPORT PFNGLMULTTRANSPOSEMATRIXDARBPROC __glewMultTransposeMatrixdARB;
GLEW_FUN_EXPORT PFNGLMULTTRANSPOSEMATRIXFARBPROC __glewMultTransposeMatrixfARB;

GLEW_FUN_EXPORT PFNGLBINDBUFFERBASEPROC __glewBindBufferBase;
GLEW_FUN_EXPORT PFNGLBINDBUFFERRANGEPROC __glewBindBufferRange;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC __glewGetActiveUniformBlockName;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMBLOCKIVPROC __glewGetActiveUniformBlockiv;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMNAMEPROC __glewGetActiveUniformName;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMSIVPROC __glewGetActiveUniformsiv;
GLEW_FUN_EXPORT PFNGLGETINTEGERI_VPROC __glewGetIntegeri_v;
GLEW_FUN_EXPORT PFNGLGETUNIFORMBLOCKINDEXPROC __glewGetUniformBlockIndex;
GLEW_FUN_EXPORT PFNGLGETUNIFORMINDICESPROC __glewGetUniformIndices;
GLEW_FUN_EXPORT PFNGLUNIFORMBLOCKBINDINGPROC __glewUniformBlockBinding;

GLEW_FUN_EXPORT PFNGLBINDVERTEXARRAYPROC __glewBindVertexArray;
GLEW_FUN_EXPORT PFNGLDELETEVERTEXARRAYSPROC __glewDeleteVertexArrays;
GLEW_FUN_EXPORT PFNGLGENVERTEXARRAYSPROC __glewGenVertexArrays;
GLEW_FUN_EXPORT PFNGLISVERTEXARRAYPROC __glewIsVertexArray;

GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBLDVPROC __glewGetVertexAttribLdv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1DPROC __glewVertexAttribL1d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1DVPROC __glewVertexAttribL1dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL2DPROC __glewVertexAttribL2d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL2DVPROC __glewVertexAttribL2dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL3DPROC __glewVertexAttribL3d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL3DVPROC __glewVertexAttribL3dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL4DPROC __glewVertexAttribL4d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL4DVPROC __glewVertexAttribL4dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBLPOINTERPROC __glewVertexAttribLPointer;

GLEW_FUN_EXPORT PFNGLBINDVERTEXBUFFERPROC __glewBindVertexBuffer;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYBINDVERTEXBUFFEREXTPROC __glewVertexArrayBindVertexBufferEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXATTRIBBINDINGEXTPROC __glewVertexArrayVertexAttribBindingEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXATTRIBFORMATEXTPROC __glewVertexArrayVertexAttribFormatEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXATTRIBIFORMATEXTPROC __glewVertexArrayVertexAttribIFormatEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXATTRIBLFORMATEXTPROC __glewVertexArrayVertexAttribLFormatEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXBINDINGDIVISOREXTPROC __glewVertexArrayVertexBindingDivisorEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBBINDINGPROC __glewVertexAttribBinding;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBFORMATPROC __glewVertexAttribFormat;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBIFORMATPROC __glewVertexAttribIFormat;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBLFORMATPROC __glewVertexAttribLFormat;
GLEW_FUN_EXPORT PFNGLVERTEXBINDINGDIVISORPROC __glewVertexBindingDivisor;

GLEW_FUN_EXPORT PFNGLVERTEXBLENDARBPROC __glewVertexBlendARB;
GLEW_FUN_EXPORT PFNGLWEIGHTPOINTERARBPROC __glewWeightPointerARB;
GLEW_FUN_EXPORT PFNGLWEIGHTBVARBPROC __glewWeightbvARB;
GLEW_FUN_EXPORT PFNGLWEIGHTDVARBPROC __glewWeightdvARB;
GLEW_FUN_EXPORT PFNGLWEIGHTFVARBPROC __glewWeightfvARB;
GLEW_FUN_EXPORT PFNGLWEIGHTIVARBPROC __glewWeightivARB;
GLEW_FUN_EXPORT PFNGLWEIGHTSVARBPROC __glewWeightsvARB;
GLEW_FUN_EXPORT PFNGLWEIGHTUBVARBPROC __glewWeightubvARB;
GLEW_FUN_EXPORT PFNGLWEIGHTUIVARBPROC __glewWeightuivARB;
GLEW_FUN_EXPORT PFNGLWEIGHTUSVARBPROC __glewWeightusvARB;

GLEW_FUN_EXPORT PFNGLBINDBUFFERARBPROC __glewBindBufferARB;
GLEW_FUN_EXPORT PFNGLBUFFERDATAARBPROC __glewBufferDataARB;
GLEW_FUN_EXPORT PFNGLBUFFERSUBDATAARBPROC __glewBufferSubDataARB;
GLEW_FUN_EXPORT PFNGLDELETEBUFFERSARBPROC __glewDeleteBuffersARB;
GLEW_FUN_EXPORT PFNGLGENBUFFERSARBPROC __glewGenBuffersARB;
GLEW_FUN_EXPORT PFNGLGETBUFFERPARAMETERIVARBPROC __glewGetBufferParameterivARB;
GLEW_FUN_EXPORT PFNGLGETBUFFERPOINTERVARBPROC __glewGetBufferPointervARB;
GLEW_FUN_EXPORT PFNGLGETBUFFERSUBDATAARBPROC __glewGetBufferSubDataARB;
GLEW_FUN_EXPORT PFNGLISBUFFERARBPROC __glewIsBufferARB;
GLEW_FUN_EXPORT PFNGLMAPBUFFERARBPROC __glewMapBufferARB;
GLEW_FUN_EXPORT PFNGLUNMAPBUFFERARBPROC __glewUnmapBufferARB;

GLEW_FUN_EXPORT PFNGLBINDPROGRAMARBPROC __glewBindProgramARB;
GLEW_FUN_EXPORT PFNGLDELETEPROGRAMSARBPROC __glewDeleteProgramsARB;
GLEW_FUN_EXPORT PFNGLDISABLEVERTEXATTRIBARRAYARBPROC __glewDisableVertexAttribArrayARB;
GLEW_FUN_EXPORT PFNGLENABLEVERTEXATTRIBARRAYARBPROC __glewEnableVertexAttribArrayARB;
GLEW_FUN_EXPORT PFNGLGENPROGRAMSARBPROC __glewGenProgramsARB;
GLEW_FUN_EXPORT PFNGLGETPROGRAMENVPARAMETERDVARBPROC __glewGetProgramEnvParameterdvARB;
GLEW_FUN_EXPORT PFNGLGETPROGRAMENVPARAMETERFVARBPROC __glewGetProgramEnvParameterfvARB;
GLEW_FUN_EXPORT PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC __glewGetProgramLocalParameterdvARB;
GLEW_FUN_EXPORT PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC __glewGetProgramLocalParameterfvARB;
GLEW_FUN_EXPORT PFNGLGETPROGRAMSTRINGARBPROC __glewGetProgramStringARB;
GLEW_FUN_EXPORT PFNGLGETPROGRAMIVARBPROC __glewGetProgramivARB;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBPOINTERVARBPROC __glewGetVertexAttribPointervARB;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBDVARBPROC __glewGetVertexAttribdvARB;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBFVARBPROC __glewGetVertexAttribfvARB;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIVARBPROC __glewGetVertexAttribivARB;
GLEW_FUN_EXPORT PFNGLISPROGRAMARBPROC __glewIsProgramARB;
GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETER4DARBPROC __glewProgramEnvParameter4dARB;
GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETER4DVARBPROC __glewProgramEnvParameter4dvARB;
GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETER4FARBPROC __glewProgramEnvParameter4fARB;
GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETER4FVARBPROC __glewProgramEnvParameter4fvARB;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETER4DARBPROC __glewProgramLocalParameter4dARB;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETER4DVARBPROC __glewProgramLocalParameter4dvARB;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETER4FARBPROC __glewProgramLocalParameter4fARB;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETER4FVARBPROC __glewProgramLocalParameter4fvARB;
GLEW_FUN_EXPORT PFNGLPROGRAMSTRINGARBPROC __glewProgramStringARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1DARBPROC __glewVertexAttrib1dARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1DVARBPROC __glewVertexAttrib1dvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1FARBPROC __glewVertexAttrib1fARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1FVARBPROC __glewVertexAttrib1fvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1SARBPROC __glewVertexAttrib1sARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1SVARBPROC __glewVertexAttrib1svARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2DARBPROC __glewVertexAttrib2dARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2DVARBPROC __glewVertexAttrib2dvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2FARBPROC __glewVertexAttrib2fARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2FVARBPROC __glewVertexAttrib2fvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2SARBPROC __glewVertexAttrib2sARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2SVARBPROC __glewVertexAttrib2svARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3DARBPROC __glewVertexAttrib3dARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3DVARBPROC __glewVertexAttrib3dvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3FARBPROC __glewVertexAttrib3fARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3FVARBPROC __glewVertexAttrib3fvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3SARBPROC __glewVertexAttrib3sARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3SVARBPROC __glewVertexAttrib3svARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NBVARBPROC __glewVertexAttrib4NbvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NIVARBPROC __glewVertexAttrib4NivARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NSVARBPROC __glewVertexAttrib4NsvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUBARBPROC __glewVertexAttrib4NubARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUBVARBPROC __glewVertexAttrib4NubvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUIVARBPROC __glewVertexAttrib4NuivARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUSVARBPROC __glewVertexAttrib4NusvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4BVARBPROC __glewVertexAttrib4bvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4DARBPROC __glewVertexAttrib4dARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4DVARBPROC __glewVertexAttrib4dvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4FARBPROC __glewVertexAttrib4fARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4FVARBPROC __glewVertexAttrib4fvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4IVARBPROC __glewVertexAttrib4ivARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4SARBPROC __glewVertexAttrib4sARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4SVARBPROC __glewVertexAttrib4svARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4UBVARBPROC __glewVertexAttrib4ubvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4UIVARBPROC __glewVertexAttrib4uivARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4USVARBPROC __glewVertexAttrib4usvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBPOINTERARBPROC __glewVertexAttribPointerARB;

GLEW_FUN_EXPORT PFNGLCOLORP3UIPROC __glewColorP3ui;
GLEW_FUN_EXPORT PFNGLCOLORP3UIVPROC __glewColorP3uiv;
GLEW_FUN_EXPORT PFNGLCOLORP4UIPROC __glewColorP4ui;
GLEW_FUN_EXPORT PFNGLCOLORP4UIVPROC __glewColorP4uiv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP1UIPROC __glewMultiTexCoordP1ui;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP1UIVPROC __glewMultiTexCoordP1uiv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP2UIPROC __glewMultiTexCoordP2ui;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP2UIVPROC __glewMultiTexCoordP2uiv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP3UIPROC __glewMultiTexCoordP3ui;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP3UIVPROC __glewMultiTexCoordP3uiv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP4UIPROC __glewMultiTexCoordP4ui;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP4UIVPROC __glewMultiTexCoordP4uiv;
GLEW_FUN_EXPORT PFNGLNORMALP3UIPROC __glewNormalP3ui;
GLEW_FUN_EXPORT PFNGLNORMALP3UIVPROC __glewNormalP3uiv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLORP3UIPROC __glewSecondaryColorP3ui;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLORP3UIVPROC __glewSecondaryColorP3uiv;
GLEW_FUN_EXPORT PFNGLTEXCOORDP1UIPROC __glewTexCoordP1ui;
GLEW_FUN_EXPORT PFNGLTEXCOORDP1UIVPROC __glewTexCoordP1uiv;
GLEW_FUN_EXPORT PFNGLTEXCOORDP2UIPROC __glewTexCoordP2ui;
GLEW_FUN_EXPORT PFNGLTEXCOORDP2UIVPROC __glewTexCoordP2uiv;
GLEW_FUN_EXPORT PFNGLTEXCOORDP3UIPROC __glewTexCoordP3ui;
GLEW_FUN_EXPORT PFNGLTEXCOORDP3UIVPROC __glewTexCoordP3uiv;
GLEW_FUN_EXPORT PFNGLTEXCOORDP4UIPROC __glewTexCoordP4ui;
GLEW_FUN_EXPORT PFNGLTEXCOORDP4UIVPROC __glewTexCoordP4uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP1UIPROC __glewVertexAttribP1ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP1UIVPROC __glewVertexAttribP1uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP2UIPROC __glewVertexAttribP2ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP2UIVPROC __glewVertexAttribP2uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP3UIPROC __glewVertexAttribP3ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP3UIVPROC __glewVertexAttribP3uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP4UIPROC __glewVertexAttribP4ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP4UIVPROC __glewVertexAttribP4uiv;
GLEW_FUN_EXPORT PFNGLVERTEXP2UIPROC __glewVertexP2ui;
GLEW_FUN_EXPORT PFNGLVERTEXP2UIVPROC __glewVertexP2uiv;
GLEW_FUN_EXPORT PFNGLVERTEXP3UIPROC __glewVertexP3ui;
GLEW_FUN_EXPORT PFNGLVERTEXP3UIVPROC __glewVertexP3uiv;
GLEW_FUN_EXPORT PFNGLVERTEXP4UIPROC __glewVertexP4ui;
GLEW_FUN_EXPORT PFNGLVERTEXP4UIVPROC __glewVertexP4uiv;

GLEW_FUN_EXPORT PFNGLDEPTHRANGEARRAYVPROC __glewDepthRangeArrayv;
GLEW_FUN_EXPORT PFNGLDEPTHRANGEINDEXEDPROC __glewDepthRangeIndexed;
GLEW_FUN_EXPORT PFNGLGETDOUBLEI_VPROC __glewGetDoublei_v;
GLEW_FUN_EXPORT PFNGLGETFLOATI_VPROC __glewGetFloati_v;
GLEW_FUN_EXPORT PFNGLSCISSORARRAYVPROC __glewScissorArrayv;
GLEW_FUN_EXPORT PFNGLSCISSORINDEXEDPROC __glewScissorIndexed;
GLEW_FUN_EXPORT PFNGLSCISSORINDEXEDVPROC __glewScissorIndexedv;
GLEW_FUN_EXPORT PFNGLVIEWPORTARRAYVPROC __glewViewportArrayv;
GLEW_FUN_EXPORT PFNGLVIEWPORTINDEXEDFPROC __glewViewportIndexedf;
GLEW_FUN_EXPORT PFNGLVIEWPORTINDEXEDFVPROC __glewViewportIndexedfv;

GLEW_FUN_EXPORT PFNGLWINDOWPOS2DARBPROC __glewWindowPos2dARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2DVARBPROC __glewWindowPos2dvARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2FARBPROC __glewWindowPos2fARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2FVARBPROC __glewWindowPos2fvARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2IARBPROC __glewWindowPos2iARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2IVARBPROC __glewWindowPos2ivARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2SARBPROC __glewWindowPos2sARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2SVARBPROC __glewWindowPos2svARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3DARBPROC __glewWindowPos3dARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3DVARBPROC __glewWindowPos3dvARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3FARBPROC __glewWindowPos3fARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3FVARBPROC __glewWindowPos3fvARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3IARBPROC __glewWindowPos3iARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3IVARBPROC __glewWindowPos3ivARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3SARBPROC __glewWindowPos3sARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3SVARBPROC __glewWindowPos3svARB;

GLEW_FUN_EXPORT PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEEXTPROC __glewDrawArraysInstancedBaseInstanceEXT;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEEXTPROC __glewDrawElementsInstancedBaseInstanceEXT;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEEXTPROC __glewDrawElementsInstancedBaseVertexBaseInstanceEXT;

GLEW_FUN_EXPORT PFNGLGETUNIFORMBUFFERSIZEEXTPROC __glewGetUniformBufferSizeEXT;
GLEW_FUN_EXPORT PFNGLGETUNIFORMOFFSETEXTPROC __glewGetUniformOffsetEXT;
GLEW_FUN_EXPORT PFNGLUNIFORMBUFFEREXTPROC __glewUniformBufferEXT;

GLEW_FUN_EXPORT PFNGLBLENDCOLOREXTPROC __glewBlendColorEXT;

GLEW_FUN_EXPORT PFNGLBLENDEQUATIONSEPARATEEXTPROC __glewBlendEquationSeparateEXT;

GLEW_FUN_EXPORT PFNGLBINDFRAGDATALOCATIONINDEXEDEXTPROC __glewBindFragDataLocationIndexedEXT;
GLEW_FUN_EXPORT PFNGLGETFRAGDATAINDEXEXTPROC __glewGetFragDataIndexEXT;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCELOCATIONINDEXEXTPROC __glewGetProgramResourceLocationIndexEXT;

GLEW_FUN_EXPORT PFNGLBLENDFUNCSEPARATEEXTPROC __glewBlendFuncSeparateEXT;

GLEW_FUN_EXPORT PFNGLBLENDEQUATIONEXTPROC __glewBlendEquationEXT;

GLEW_FUN_EXPORT PFNGLBUFFERSTORAGEEXTPROC __glewBufferStorageEXT;
GLEW_FUN_EXPORT PFNGLNAMEDBUFFERSTORAGEEXTPROC __glewNamedBufferStorageEXT;

GLEW_FUN_EXPORT PFNGLCLEARTEXIMAGEEXTPROC __glewClearTexImageEXT;
GLEW_FUN_EXPORT PFNGLCLEARTEXSUBIMAGEEXTPROC __glewClearTexSubImageEXT;

GLEW_FUN_EXPORT PFNGLCOLORSUBTABLEEXTPROC __glewColorSubTableEXT;
GLEW_FUN_EXPORT PFNGLCOPYCOLORSUBTABLEEXTPROC __glewCopyColorSubTableEXT;

GLEW_FUN_EXPORT PFNGLLOCKARRAYSEXTPROC __glewLockArraysEXT;
GLEW_FUN_EXPORT PFNGLUNLOCKARRAYSEXTPROC __glewUnlockArraysEXT;

GLEW_FUN_EXPORT PFNGLCONVOLUTIONFILTER1DEXTPROC __glewConvolutionFilter1DEXT;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONFILTER2DEXTPROC __glewConvolutionFilter2DEXT;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERFEXTPROC __glewConvolutionParameterfEXT;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERFVEXTPROC __glewConvolutionParameterfvEXT;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERIEXTPROC __glewConvolutionParameteriEXT;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERIVEXTPROC __glewConvolutionParameterivEXT;
GLEW_FUN_EXPORT PFNGLCOPYCONVOLUTIONFILTER1DEXTPROC __glewCopyConvolutionFilter1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYCONVOLUTIONFILTER2DEXTPROC __glewCopyConvolutionFilter2DEXT;
GLEW_FUN_EXPORT PFNGLGETCONVOLUTIONFILTEREXTPROC __glewGetConvolutionFilterEXT;
GLEW_FUN_EXPORT PFNGLGETCONVOLUTIONPARAMETERFVEXTPROC __glewGetConvolutionParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETCONVOLUTIONPARAMETERIVEXTPROC __glewGetConvolutionParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETSEPARABLEFILTEREXTPROC __glewGetSeparableFilterEXT;
GLEW_FUN_EXPORT PFNGLSEPARABLEFILTER2DEXTPROC __glewSeparableFilter2DEXT;

GLEW_FUN_EXPORT PFNGLBINORMALPOINTEREXTPROC __glewBinormalPointerEXT;
GLEW_FUN_EXPORT PFNGLTANGENTPOINTEREXTPROC __glewTangentPointerEXT;

GLEW_FUN_EXPORT PFNGLCOPYIMAGESUBDATAEXTPROC __glewCopyImageSubDataEXT;

GLEW_FUN_EXPORT PFNGLCOPYTEXIMAGE1DEXTPROC __glewCopyTexImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXIMAGE2DEXTPROC __glewCopyTexImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXSUBIMAGE1DEXTPROC __glewCopyTexSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXSUBIMAGE2DEXTPROC __glewCopyTexSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXSUBIMAGE3DEXTPROC __glewCopyTexSubImage3DEXT;

GLEW_FUN_EXPORT PFNGLCULLPARAMETERDVEXTPROC __glewCullParameterdvEXT;
GLEW_FUN_EXPORT PFNGLCULLPARAMETERFVEXTPROC __glewCullParameterfvEXT;

GLEW_FUN_EXPORT PFNGLGETOBJECTLABELEXTPROC __glewGetObjectLabelEXT;
GLEW_FUN_EXPORT PFNGLLABELOBJECTEXTPROC __glewLabelObjectEXT;

GLEW_FUN_EXPORT PFNGLINSERTEVENTMARKEREXTPROC __glewInsertEventMarkerEXT;
GLEW_FUN_EXPORT PFNGLPOPGROUPMARKEREXTPROC __glewPopGroupMarkerEXT;
GLEW_FUN_EXPORT PFNGLPUSHGROUPMARKEREXTPROC __glewPushGroupMarkerEXT;

GLEW_FUN_EXPORT PFNGLDEPTHBOUNDSEXTPROC __glewDepthBoundsEXT;

GLEW_FUN_EXPORT PFNGLBINDMULTITEXTUREEXTPROC __glewBindMultiTextureEXT;
GLEW_FUN_EXPORT PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC __glewCheckNamedFramebufferStatusEXT;
GLEW_FUN_EXPORT PFNGLCLIENTATTRIBDEFAULTEXTPROC __glewClientAttribDefaultEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC __glewCompressedMultiTexImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC __glewCompressedMultiTexImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC __glewCompressedMultiTexImage3DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC __glewCompressedMultiTexSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC __glewCompressedMultiTexSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC __glewCompressedMultiTexSubImage3DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC __glewCompressedTextureImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC __glewCompressedTextureImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC __glewCompressedTextureImage3DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC __glewCompressedTextureSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC __glewCompressedTextureSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC __glewCompressedTextureSubImage3DEXT;
GLEW_FUN_EXPORT PFNGLCOPYMULTITEXIMAGE1DEXTPROC __glewCopyMultiTexImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYMULTITEXIMAGE2DEXTPROC __glewCopyMultiTexImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC __glewCopyMultiTexSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC __glewCopyMultiTexSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC __glewCopyMultiTexSubImage3DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXTUREIMAGE1DEXTPROC __glewCopyTextureImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXTUREIMAGE2DEXTPROC __glewCopyTextureImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC __glewCopyTextureSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC __glewCopyTextureSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC __glewCopyTextureSubImage3DEXT;
GLEW_FUN_EXPORT PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC __glewDisableClientStateIndexedEXT;
GLEW_FUN_EXPORT PFNGLDISABLECLIENTSTATEIEXTPROC __glewDisableClientStateiEXT;
GLEW_FUN_EXPORT PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC __glewDisableVertexArrayAttribEXT;
GLEW_FUN_EXPORT PFNGLDISABLEVERTEXARRAYEXTPROC __glewDisableVertexArrayEXT;
GLEW_FUN_EXPORT PFNGLENABLECLIENTSTATEINDEXEDEXTPROC __glewEnableClientStateIndexedEXT;
GLEW_FUN_EXPORT PFNGLENABLECLIENTSTATEIEXTPROC __glewEnableClientStateiEXT;
GLEW_FUN_EXPORT PFNGLENABLEVERTEXARRAYATTRIBEXTPROC __glewEnableVertexArrayAttribEXT;
GLEW_FUN_EXPORT PFNGLENABLEVERTEXARRAYEXTPROC __glewEnableVertexArrayEXT;
GLEW_FUN_EXPORT PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC __glewFlushMappedNamedBufferRangeEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC __glewFramebufferDrawBufferEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC __glewFramebufferDrawBuffersEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERREADBUFFEREXTPROC __glewFramebufferReadBufferEXT;
GLEW_FUN_EXPORT PFNGLGENERATEMULTITEXMIPMAPEXTPROC __glewGenerateMultiTexMipmapEXT;
GLEW_FUN_EXPORT PFNGLGENERATETEXTUREMIPMAPEXTPROC __glewGenerateTextureMipmapEXT;
GLEW_FUN_EXPORT PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC __glewGetCompressedMultiTexImageEXT;
GLEW_FUN_EXPORT PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC __glewGetCompressedTextureImageEXT;
GLEW_FUN_EXPORT PFNGLGETDOUBLEINDEXEDVEXTPROC __glewGetDoubleIndexedvEXT;
GLEW_FUN_EXPORT PFNGLGETDOUBLEI_VEXTPROC __glewGetDoublei_vEXT;
GLEW_FUN_EXPORT PFNGLGETFLOATINDEXEDVEXTPROC __glewGetFloatIndexedvEXT;
GLEW_FUN_EXPORT PFNGLGETFLOATI_VEXTPROC __glewGetFloati_vEXT;
GLEW_FUN_EXPORT PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC __glewGetFramebufferParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXENVFVEXTPROC __glewGetMultiTexEnvfvEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXENVIVEXTPROC __glewGetMultiTexEnvivEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXGENDVEXTPROC __glewGetMultiTexGendvEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXGENFVEXTPROC __glewGetMultiTexGenfvEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXGENIVEXTPROC __glewGetMultiTexGenivEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXIMAGEEXTPROC __glewGetMultiTexImageEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC __glewGetMultiTexLevelParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC __glewGetMultiTexLevelParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXPARAMETERIIVEXTPROC __glewGetMultiTexParameterIivEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXPARAMETERIUIVEXTPROC __glewGetMultiTexParameterIuivEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXPARAMETERFVEXTPROC __glewGetMultiTexParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXPARAMETERIVEXTPROC __glewGetMultiTexParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC __glewGetNamedBufferParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDBUFFERPOINTERVEXTPROC __glewGetNamedBufferPointervEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDBUFFERSUBDATAEXTPROC __glewGetNamedBufferSubDataEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC __glewGetNamedFramebufferAttachmentParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC __glewGetNamedProgramLocalParameterIivEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC __glewGetNamedProgramLocalParameterIuivEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC __glewGetNamedProgramLocalParameterdvEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC __glewGetNamedProgramLocalParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDPROGRAMSTRINGEXTPROC __glewGetNamedProgramStringEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDPROGRAMIVEXTPROC __glewGetNamedProgramivEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC __glewGetNamedRenderbufferParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETPOINTERINDEXEDVEXTPROC __glewGetPointerIndexedvEXT;
GLEW_FUN_EXPORT PFNGLGETPOINTERI_VEXTPROC __glewGetPointeri_vEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTUREIMAGEEXTPROC __glewGetTextureImageEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC __glewGetTextureLevelParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC __glewGetTextureLevelParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTUREPARAMETERIIVEXTPROC __glewGetTextureParameterIivEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTUREPARAMETERIUIVEXTPROC __glewGetTextureParameterIuivEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTUREPARAMETERFVEXTPROC __glewGetTextureParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTUREPARAMETERIVEXTPROC __glewGetTextureParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC __glewGetVertexArrayIntegeri_vEXT;
GLEW_FUN_EXPORT PFNGLGETVERTEXARRAYINTEGERVEXTPROC __glewGetVertexArrayIntegervEXT;
GLEW_FUN_EXPORT PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC __glewGetVertexArrayPointeri_vEXT;
GLEW_FUN_EXPORT PFNGLGETVERTEXARRAYPOINTERVEXTPROC __glewGetVertexArrayPointervEXT;
GLEW_FUN_EXPORT PFNGLMAPNAMEDBUFFEREXTPROC __glewMapNamedBufferEXT;
GLEW_FUN_EXPORT PFNGLMAPNAMEDBUFFERRANGEEXTPROC __glewMapNamedBufferRangeEXT;
GLEW_FUN_EXPORT PFNGLMATRIXFRUSTUMEXTPROC __glewMatrixFrustumEXT;
GLEW_FUN_EXPORT PFNGLMATRIXLOADIDENTITYEXTPROC __glewMatrixLoadIdentityEXT;
GLEW_FUN_EXPORT PFNGLMATRIXLOADTRANSPOSEDEXTPROC __glewMatrixLoadTransposedEXT;
GLEW_FUN_EXPORT PFNGLMATRIXLOADTRANSPOSEFEXTPROC __glewMatrixLoadTransposefEXT;
GLEW_FUN_EXPORT PFNGLMATRIXLOADDEXTPROC __glewMatrixLoaddEXT;
GLEW_FUN_EXPORT PFNGLMATRIXLOADFEXTPROC __glewMatrixLoadfEXT;
GLEW_FUN_EXPORT PFNGLMATRIXMULTTRANSPOSEDEXTPROC __glewMatrixMultTransposedEXT;
GLEW_FUN_EXPORT PFNGLMATRIXMULTTRANSPOSEFEXTPROC __glewMatrixMultTransposefEXT;
GLEW_FUN_EXPORT PFNGLMATRIXMULTDEXTPROC __glewMatrixMultdEXT;
GLEW_FUN_EXPORT PFNGLMATRIXMULTFEXTPROC __glewMatrixMultfEXT;
GLEW_FUN_EXPORT PFNGLMATRIXORTHOEXTPROC __glewMatrixOrthoEXT;
GLEW_FUN_EXPORT PFNGLMATRIXPOPEXTPROC __glewMatrixPopEXT;
GLEW_FUN_EXPORT PFNGLMATRIXPUSHEXTPROC __glewMatrixPushEXT;
GLEW_FUN_EXPORT PFNGLMATRIXROTATEDEXTPROC __glewMatrixRotatedEXT;
GLEW_FUN_EXPORT PFNGLMATRIXROTATEFEXTPROC __glewMatrixRotatefEXT;
GLEW_FUN_EXPORT PFNGLMATRIXSCALEDEXTPROC __glewMatrixScaledEXT;
GLEW_FUN_EXPORT PFNGLMATRIXSCALEFEXTPROC __glewMatrixScalefEXT;
GLEW_FUN_EXPORT PFNGLMATRIXTRANSLATEDEXTPROC __glewMatrixTranslatedEXT;
GLEW_FUN_EXPORT PFNGLMATRIXTRANSLATEFEXTPROC __glewMatrixTranslatefEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXBUFFEREXTPROC __glewMultiTexBufferEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDPOINTEREXTPROC __glewMultiTexCoordPointerEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXENVFEXTPROC __glewMultiTexEnvfEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXENVFVEXTPROC __glewMultiTexEnvfvEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXENVIEXTPROC __glewMultiTexEnviEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXENVIVEXTPROC __glewMultiTexEnvivEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXGENDEXTPROC __glewMultiTexGendEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXGENDVEXTPROC __glewMultiTexGendvEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXGENFEXTPROC __glewMultiTexGenfEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXGENFVEXTPROC __glewMultiTexGenfvEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXGENIEXTPROC __glewMultiTexGeniEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXGENIVEXTPROC __glewMultiTexGenivEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXIMAGE1DEXTPROC __glewMultiTexImage1DEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXIMAGE2DEXTPROC __glewMultiTexImage2DEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXIMAGE3DEXTPROC __glewMultiTexImage3DEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXPARAMETERIIVEXTPROC __glewMultiTexParameterIivEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXPARAMETERIUIVEXTPROC __glewMultiTexParameterIuivEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXPARAMETERFEXTPROC __glewMultiTexParameterfEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXPARAMETERFVEXTPROC __glewMultiTexParameterfvEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXPARAMETERIEXTPROC __glewMultiTexParameteriEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXPARAMETERIVEXTPROC __glewMultiTexParameterivEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXRENDERBUFFEREXTPROC __glewMultiTexRenderbufferEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXSUBIMAGE1DEXTPROC __glewMultiTexSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXSUBIMAGE2DEXTPROC __glewMultiTexSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXSUBIMAGE3DEXTPROC __glewMultiTexSubImage3DEXT;
GLEW_FUN_EXPORT PFNGLNAMEDBUFFERDATAEXTPROC __glewNamedBufferDataEXT;
GLEW_FUN_EXPORT PFNGLNAMEDBUFFERSUBDATAEXTPROC __glewNamedBufferSubDataEXT;
GLEW_FUN_EXPORT PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC __glewNamedCopyBufferSubDataEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC __glewNamedFramebufferRenderbufferEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC __glewNamedFramebufferTexture1DEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC __glewNamedFramebufferTexture2DEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC __glewNamedFramebufferTexture3DEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC __glewNamedFramebufferTextureEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC __glewNamedFramebufferTextureFaceEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC __glewNamedFramebufferTextureLayerEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC __glewNamedProgramLocalParameter4dEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC __glewNamedProgramLocalParameter4dvEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC __glewNamedProgramLocalParameter4fEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC __glewNamedProgramLocalParameter4fvEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC __glewNamedProgramLocalParameterI4iEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC __glewNamedProgramLocalParameterI4ivEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC __glewNamedProgramLocalParameterI4uiEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC __glewNamedProgramLocalParameterI4uivEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC __glewNamedProgramLocalParameters4fvEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC __glewNamedProgramLocalParametersI4ivEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC __glewNamedProgramLocalParametersI4uivEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMSTRINGEXTPROC __glewNamedProgramStringEXT;
GLEW_FUN_EXPORT PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC __glewNamedRenderbufferStorageEXT;
GLEW_FUN_EXPORT PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC __glewNamedRenderbufferStorageMultisampleCoverageEXT;
GLEW_FUN_EXPORT PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC __glewNamedRenderbufferStorageMultisampleEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1FEXTPROC __glewProgramUniform1fEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1FVEXTPROC __glewProgramUniform1fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1IEXTPROC __glewProgramUniform1iEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1IVEXTPROC __glewProgramUniform1ivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UIEXTPROC __glewProgramUniform1uiEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UIVEXTPROC __glewProgramUniform1uivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2FEXTPROC __glewProgramUniform2fEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2FVEXTPROC __glewProgramUniform2fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2IEXTPROC __glewProgramUniform2iEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2IVEXTPROC __glewProgramUniform2ivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UIEXTPROC __glewProgramUniform2uiEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UIVEXTPROC __glewProgramUniform2uivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3FEXTPROC __glewProgramUniform3fEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3FVEXTPROC __glewProgramUniform3fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3IEXTPROC __glewProgramUniform3iEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3IVEXTPROC __glewProgramUniform3ivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UIEXTPROC __glewProgramUniform3uiEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UIVEXTPROC __glewProgramUniform3uivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4FEXTPROC __glewProgramUniform4fEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4FVEXTPROC __glewProgramUniform4fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4IEXTPROC __glewProgramUniform4iEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4IVEXTPROC __glewProgramUniform4ivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UIEXTPROC __glewProgramUniform4uiEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UIVEXTPROC __glewProgramUniform4uivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC __glewProgramUniformMatrix2fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC __glewProgramUniformMatrix2x3fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC __glewProgramUniformMatrix2x4fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC __glewProgramUniformMatrix3fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC __glewProgramUniformMatrix3x2fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC __glewProgramUniformMatrix3x4fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC __glewProgramUniformMatrix4fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC __glewProgramUniformMatrix4x2fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC __glewProgramUniformMatrix4x3fvEXT;
GLEW_FUN_EXPORT PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC __glewPushClientAttribDefaultEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREBUFFEREXTPROC __glewTextureBufferEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREIMAGE1DEXTPROC __glewTextureImage1DEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREIMAGE2DEXTPROC __glewTextureImage2DEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREIMAGE3DEXTPROC __glewTextureImage3DEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERIIVEXTPROC __glewTextureParameterIivEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERIUIVEXTPROC __glewTextureParameterIuivEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERFEXTPROC __glewTextureParameterfEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERFVEXTPROC __glewTextureParameterfvEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERIEXTPROC __glewTextureParameteriEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERIVEXTPROC __glewTextureParameterivEXT;
GLEW_FUN_EXPORT PFNGLTEXTURERENDERBUFFEREXTPROC __glewTextureRenderbufferEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESUBIMAGE1DEXTPROC __glewTextureSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESUBIMAGE2DEXTPROC __glewTextureSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESUBIMAGE3DEXTPROC __glewTextureSubImage3DEXT;
GLEW_FUN_EXPORT PFNGLUNMAPNAMEDBUFFEREXTPROC __glewUnmapNamedBufferEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYCOLOROFFSETEXTPROC __glewVertexArrayColorOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYEDGEFLAGOFFSETEXTPROC __glewVertexArrayEdgeFlagOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYFOGCOORDOFFSETEXTPROC __glewVertexArrayFogCoordOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYINDEXOFFSETEXTPROC __glewVertexArrayIndexOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYMULTITEXCOORDOFFSETEXTPROC __glewVertexArrayMultiTexCoordOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYNORMALOFFSETEXTPROC __glewVertexArrayNormalOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYSECONDARYCOLOROFFSETEXTPROC __glewVertexArraySecondaryColorOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYTEXCOORDOFFSETEXTPROC __glewVertexArrayTexCoordOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXATTRIBDIVISOREXTPROC __glewVertexArrayVertexAttribDivisorEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC __glewVertexArrayVertexAttribIOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC __glewVertexArrayVertexAttribOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC __glewVertexArrayVertexOffsetEXT;

GLEW_FUN_EXPORT PFNGLDISCARDFRAMEBUFFEREXTPROC __glewDiscardFramebufferEXT;

GLEW_FUN_EXPORT PFNGLDRAWBUFFERSEXTPROC __glewDrawBuffersEXT;

GLEW_FUN_EXPORT PFNGLCOLORMASKINDEXEDEXTPROC __glewColorMaskIndexedEXT;
GLEW_FUN_EXPORT PFNGLDISABLEINDEXEDEXTPROC __glewDisableIndexedEXT;
GLEW_FUN_EXPORT PFNGLENABLEINDEXEDEXTPROC __glewEnableIndexedEXT;
GLEW_FUN_EXPORT PFNGLGETBOOLEANINDEXEDVEXTPROC __glewGetBooleanIndexedvEXT;
GLEW_FUN_EXPORT PFNGLGETINTEGERINDEXEDVEXTPROC __glewGetIntegerIndexedvEXT;
GLEW_FUN_EXPORT PFNGLISENABLEDINDEXEDEXTPROC __glewIsEnabledIndexedEXT;

GLEW_FUN_EXPORT PFNGLBLENDEQUATIONSEPARATEIEXTPROC __glewBlendEquationSeparateiEXT;
GLEW_FUN_EXPORT PFNGLBLENDEQUATIONIEXTPROC __glewBlendEquationiEXT;
GLEW_FUN_EXPORT PFNGLBLENDFUNCSEPARATEIEXTPROC __glewBlendFuncSeparateiEXT;
GLEW_FUN_EXPORT PFNGLBLENDFUNCIEXTPROC __glewBlendFunciEXT;
GLEW_FUN_EXPORT PFNGLCOLORMASKIEXTPROC __glewColorMaskiEXT;
GLEW_FUN_EXPORT PFNGLDISABLEIEXTPROC __glewDisableiEXT;
GLEW_FUN_EXPORT PFNGLENABLEIEXTPROC __glewEnableiEXT;
GLEW_FUN_EXPORT PFNGLISENABLEDIEXTPROC __glewIsEnablediEXT;

GLEW_FUN_EXPORT PFNGLDRAWELEMENTSBASEVERTEXEXTPROC __glewDrawElementsBaseVertexEXT;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXEXTPROC __glewDrawElementsInstancedBaseVertexEXT;
GLEW_FUN_EXPORT PFNGLDRAWRANGEELEMENTSBASEVERTEXEXTPROC __glewDrawRangeElementsBaseVertexEXT;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTSBASEVERTEXEXTPROC __glewMultiDrawElementsBaseVertexEXT;

GLEW_FUN_EXPORT PFNGLDRAWARRAYSINSTANCEDEXTPROC __glewDrawArraysInstancedEXT;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDEXTPROC __glewDrawElementsInstancedEXT;

GLEW_FUN_EXPORT PFNGLDRAWRANGEELEMENTSEXTPROC __glewDrawRangeElementsEXT;

GLEW_FUN_EXPORT PFNGLBUFFERSTORAGEEXTERNALEXTPROC __glewBufferStorageExternalEXT;
GLEW_FUN_EXPORT PFNGLNAMEDBUFFERSTORAGEEXTERNALEXTPROC __glewNamedBufferStorageExternalEXT;

GLEW_FUN_EXPORT PFNGLFOGCOORDPOINTEREXTPROC __glewFogCoordPointerEXT;
GLEW_FUN_EXPORT PFNGLFOGCOORDDEXTPROC __glewFogCoorddEXT;
GLEW_FUN_EXPORT PFNGLFOGCOORDDVEXTPROC __glewFogCoorddvEXT;
GLEW_FUN_EXPORT PFNGLFOGCOORDFEXTPROC __glewFogCoordfEXT;
GLEW_FUN_EXPORT PFNGLFOGCOORDFVEXTPROC __glewFogCoordfvEXT;

GLEW_FUN_EXPORT PFNGLFRAGMENTCOLORMATERIALEXTPROC __glewFragmentColorMaterialEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTMODELFEXTPROC __glewFragmentLightModelfEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTMODELFVEXTPROC __glewFragmentLightModelfvEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTMODELIEXTPROC __glewFragmentLightModeliEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTMODELIVEXTPROC __glewFragmentLightModelivEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTFEXTPROC __glewFragmentLightfEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTFVEXTPROC __glewFragmentLightfvEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTIEXTPROC __glewFragmentLightiEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTIVEXTPROC __glewFragmentLightivEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTMATERIALFEXTPROC __glewFragmentMaterialfEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTMATERIALFVEXTPROC __glewFragmentMaterialfvEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTMATERIALIEXTPROC __glewFragmentMaterialiEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTMATERIALIVEXTPROC __glewFragmentMaterialivEXT;
GLEW_FUN_EXPORT PFNGLGETFRAGMENTLIGHTFVEXTPROC __glewGetFragmentLightfvEXT;
GLEW_FUN_EXPORT PFNGLGETFRAGMENTLIGHTIVEXTPROC __glewGetFragmentLightivEXT;
GLEW_FUN_EXPORT PFNGLGETFRAGMENTMATERIALFVEXTPROC __glewGetFragmentMaterialfvEXT;
GLEW_FUN_EXPORT PFNGLGETFRAGMENTMATERIALIVEXTPROC __glewGetFragmentMaterialivEXT;
GLEW_FUN_EXPORT PFNGLLIGHTENVIEXTPROC __glewLightEnviEXT;

GLEW_FUN_EXPORT PFNGLBLITFRAMEBUFFEREXTPROC __glewBlitFramebufferEXT;

GLEW_FUN_EXPORT PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC __glewRenderbufferStorageMultisampleEXT;

GLEW_FUN_EXPORT PFNGLBINDFRAMEBUFFEREXTPROC __glewBindFramebufferEXT;
GLEW_FUN_EXPORT PFNGLBINDRENDERBUFFEREXTPROC __glewBindRenderbufferEXT;
GLEW_FUN_EXPORT PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC __glewCheckFramebufferStatusEXT;
GLEW_FUN_EXPORT PFNGLDELETEFRAMEBUFFERSEXTPROC __glewDeleteFramebuffersEXT;
GLEW_FUN_EXPORT PFNGLDELETERENDERBUFFERSEXTPROC __glewDeleteRenderbuffersEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC __glewFramebufferRenderbufferEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE1DEXTPROC __glewFramebufferTexture1DEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE2DEXTPROC __glewFramebufferTexture2DEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE3DEXTPROC __glewFramebufferTexture3DEXT;
GLEW_FUN_EXPORT PFNGLGENFRAMEBUFFERSEXTPROC __glewGenFramebuffersEXT;
GLEW_FUN_EXPORT PFNGLGENRENDERBUFFERSEXTPROC __glewGenRenderbuffersEXT;
GLEW_FUN_EXPORT PFNGLGENERATEMIPMAPEXTPROC __glewGenerateMipmapEXT;
GLEW_FUN_EXPORT PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC __glewGetFramebufferAttachmentParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC __glewGetRenderbufferParameterivEXT;
GLEW_FUN_EXPORT PFNGLISFRAMEBUFFEREXTPROC __glewIsFramebufferEXT;
GLEW_FUN_EXPORT PFNGLISRENDERBUFFEREXTPROC __glewIsRenderbufferEXT;
GLEW_FUN_EXPORT PFNGLRENDERBUFFERSTORAGEEXTPROC __glewRenderbufferStorageEXT;

GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTUREEXTPROC __glewFramebufferTextureEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC __glewFramebufferTextureFaceEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMPARAMETERIEXTPROC __glewProgramParameteriEXT;

GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETERS4FVEXTPROC __glewProgramEnvParameters4fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETERS4FVEXTPROC __glewProgramLocalParameters4fvEXT;

GLEW_FUN_EXPORT PFNGLBINDFRAGDATALOCATIONEXTPROC __glewBindFragDataLocationEXT;
GLEW_FUN_EXPORT PFNGLGETFRAGDATALOCATIONEXTPROC __glewGetFragDataLocationEXT;
GLEW_FUN_EXPORT PFNGLGETUNIFORMUIVEXTPROC __glewGetUniformuivEXT;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIIVEXTPROC __glewGetVertexAttribIivEXT;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIUIVEXTPROC __glewGetVertexAttribIuivEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM1UIEXTPROC __glewUniform1uiEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM1UIVEXTPROC __glewUniform1uivEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM2UIEXTPROC __glewUniform2uiEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM2UIVEXTPROC __glewUniform2uivEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM3UIEXTPROC __glewUniform3uiEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM3UIVEXTPROC __glewUniform3uivEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM4UIEXTPROC __glewUniform4uiEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM4UIVEXTPROC __glewUniform4uivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1IEXTPROC __glewVertexAttribI1iEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1IVEXTPROC __glewVertexAttribI1ivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1UIEXTPROC __glewVertexAttribI1uiEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1UIVEXTPROC __glewVertexAttribI1uivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2IEXTPROC __glewVertexAttribI2iEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2IVEXTPROC __glewVertexAttribI2ivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2UIEXTPROC __glewVertexAttribI2uiEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2UIVEXTPROC __glewVertexAttribI2uivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3IEXTPROC __glewVertexAttribI3iEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3IVEXTPROC __glewVertexAttribI3ivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3UIEXTPROC __glewVertexAttribI3uiEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3UIVEXTPROC __glewVertexAttribI3uivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4BVEXTPROC __glewVertexAttribI4bvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4IEXTPROC __glewVertexAttribI4iEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4IVEXTPROC __glewVertexAttribI4ivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4SVEXTPROC __glewVertexAttribI4svEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UBVEXTPROC __glewVertexAttribI4ubvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UIEXTPROC __glewVertexAttribI4uiEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UIVEXTPROC __glewVertexAttribI4uivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4USVEXTPROC __glewVertexAttribI4usvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBIPOINTEREXTPROC __glewVertexAttribIPointerEXT;

GLEW_FUN_EXPORT PFNGLGETHISTOGRAMEXTPROC __glewGetHistogramEXT;
GLEW_FUN_EXPORT PFNGLGETHISTOGRAMPARAMETERFVEXTPROC __glewGetHistogramParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETHISTOGRAMPARAMETERIVEXTPROC __glewGetHistogramParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETMINMAXEXTPROC __glewGetMinmaxEXT;
GLEW_FUN_EXPORT PFNGLGETMINMAXPARAMETERFVEXTPROC __glewGetMinmaxParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETMINMAXPARAMETERIVEXTPROC __glewGetMinmaxParameterivEXT;
GLEW_FUN_EXPORT PFNGLHISTOGRAMEXTPROC __glewHistogramEXT;
GLEW_FUN_EXPORT PFNGLMINMAXEXTPROC __glewMinmaxEXT;
GLEW_FUN_EXPORT PFNGLRESETHISTOGRAMEXTPROC __glewResetHistogramEXT;
GLEW_FUN_EXPORT PFNGLRESETMINMAXEXTPROC __glewResetMinmaxEXT;

GLEW_FUN_EXPORT PFNGLINDEXFUNCEXTPROC __glewIndexFuncEXT;

GLEW_FUN_EXPORT PFNGLINDEXMATERIALEXTPROC __glewIndexMaterialEXT;

GLEW_FUN_EXPORT PFNGLVERTEXATTRIBDIVISOREXTPROC __glewVertexAttribDivisorEXT;

GLEW_FUN_EXPORT PFNGLAPPLYTEXTUREEXTPROC __glewApplyTextureEXT;
GLEW_FUN_EXPORT PFNGLTEXTURELIGHTEXTPROC __glewTextureLightEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREMATERIALEXTPROC __glewTextureMaterialEXT;

GLEW_FUN_EXPORT PFNGLFLUSHMAPPEDBUFFERRANGEEXTPROC __glewFlushMappedBufferRangeEXT;
GLEW_FUN_EXPORT PFNGLMAPBUFFERRANGEEXTPROC __glewMapBufferRangeEXT;

GLEW_FUN_EXPORT PFNGLBUFFERSTORAGEMEMEXTPROC __glewBufferStorageMemEXT;
GLEW_FUN_EXPORT PFNGLCREATEMEMORYOBJECTSEXTPROC __glewCreateMemoryObjectsEXT;
GLEW_FUN_EXPORT PFNGLDELETEMEMORYOBJECTSEXTPROC __glewDeleteMemoryObjectsEXT;
GLEW_FUN_EXPORT PFNGLGETMEMORYOBJECTPARAMETERIVEXTPROC __glewGetMemoryObjectParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETUNSIGNEDBYTEI_VEXTPROC __glewGetUnsignedBytei_vEXT;
GLEW_FUN_EXPORT PFNGLGETUNSIGNEDBYTEVEXTPROC __glewGetUnsignedBytevEXT;
GLEW_FUN_EXPORT PFNGLISMEMORYOBJECTEXTPROC __glewIsMemoryObjectEXT;
GLEW_FUN_EXPORT PFNGLMEMORYOBJECTPARAMETERIVEXTPROC __glewMemoryObjectParameterivEXT;
GLEW_FUN_EXPORT PFNGLNAMEDBUFFERSTORAGEMEMEXTPROC __glewNamedBufferStorageMemEXT;
GLEW_FUN_EXPORT PFNGLTEXSTORAGEMEM1DEXTPROC __glewTexStorageMem1DEXT;
GLEW_FUN_EXPORT PFNGLTEXSTORAGEMEM2DEXTPROC __glewTexStorageMem2DEXT;
GLEW_FUN_EXPORT PFNGLTEXSTORAGEMEM2DMULTISAMPLEEXTPROC __glewTexStorageMem2DMultisampleEXT;
GLEW_FUN_EXPORT PFNGLTEXSTORAGEMEM3DEXTPROC __glewTexStorageMem3DEXT;
GLEW_FUN_EXPORT PFNGLTEXSTORAGEMEM3DMULTISAMPLEEXTPROC __glewTexStorageMem3DMultisampleEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGEMEM1DEXTPROC __glewTextureStorageMem1DEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGEMEM2DEXTPROC __glewTextureStorageMem2DEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGEMEM2DMULTISAMPLEEXTPROC __glewTextureStorageMem2DMultisampleEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGEMEM3DEXTPROC __glewTextureStorageMem3DEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGEMEM3DMULTISAMPLEEXTPROC __glewTextureStorageMem3DMultisampleEXT;

GLEW_FUN_EXPORT PFNGLIMPORTMEMORYFDEXTPROC __glewImportMemoryFdEXT;

GLEW_FUN_EXPORT PFNGLIMPORTMEMORYWIN32HANDLEEXTPROC __glewImportMemoryWin32HandleEXT;
GLEW_FUN_EXPORT PFNGLIMPORTMEMORYWIN32NAMEEXTPROC __glewImportMemoryWin32NameEXT;

GLEW_FUN_EXPORT PFNGLMULTIDRAWARRAYSEXTPROC __glewMultiDrawArraysEXT;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTSEXTPROC __glewMultiDrawElementsEXT;

GLEW_FUN_EXPORT PFNGLMULTIDRAWARRAYSINDIRECTEXTPROC __glewMultiDrawArraysIndirectEXT;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTSINDIRECTEXTPROC __glewMultiDrawElementsIndirectEXT;

GLEW_FUN_EXPORT PFNGLSAMPLEMASKEXTPROC __glewSampleMaskEXT;
GLEW_FUN_EXPORT PFNGLSAMPLEPATTERNEXTPROC __glewSamplePatternEXT;

GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE2DMULTISAMPLEEXTPROC __glewFramebufferTexture2DMultisampleEXT;

GLEW_FUN_EXPORT PFNGLDRAWBUFFERSINDEXEDEXTPROC __glewDrawBuffersIndexedEXT;
GLEW_FUN_EXPORT PFNGLGETINTEGERI_VEXTPROC __glewGetIntegeri_vEXT;
GLEW_FUN_EXPORT PFNGLREADBUFFERINDEXEDEXTPROC __glewReadBufferIndexedEXT;

GLEW_FUN_EXPORT PFNGLCOLORTABLEEXTPROC __glewColorTableEXT;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEEXTPROC __glewGetColorTableEXT;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEPARAMETERFVEXTPROC __glewGetColorTableParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEPARAMETERIVEXTPROC __glewGetColorTableParameterivEXT;

GLEW_FUN_EXPORT PFNGLGETPIXELTRANSFORMPARAMETERFVEXTPROC __glewGetPixelTransformParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETPIXELTRANSFORMPARAMETERIVEXTPROC __glewGetPixelTransformParameterivEXT;
GLEW_FUN_EXPORT PFNGLPIXELTRANSFORMPARAMETERFEXTPROC __glewPixelTransformParameterfEXT;
GLEW_FUN_EXPORT PFNGLPIXELTRANSFORMPARAMETERFVEXTPROC __glewPixelTransformParameterfvEXT;
GLEW_FUN_EXPORT PFNGLPIXELTRANSFORMPARAMETERIEXTPROC __glewPixelTransformParameteriEXT;
GLEW_FUN_EXPORT PFNGLPIXELTRANSFORMPARAMETERIVEXTPROC __glewPixelTransformParameterivEXT;

GLEW_FUN_EXPORT PFNGLPOINTPARAMETERFEXTPROC __glewPointParameterfEXT;
GLEW_FUN_EXPORT PFNGLPOINTPARAMETERFVEXTPROC __glewPointParameterfvEXT;

GLEW_FUN_EXPORT PFNGLPOLYGONOFFSETEXTPROC __glewPolygonOffsetEXT;

GLEW_FUN_EXPORT PFNGLPOLYGONOFFSETCLAMPEXTPROC __glewPolygonOffsetClampEXT;

GLEW_FUN_EXPORT PFNGLPROVOKINGVERTEXEXTPROC __glewProvokingVertexEXT;

GLEW_FUN_EXPORT PFNGLCOVERAGEMODULATIONNVPROC __glewCoverageModulationNV;
GLEW_FUN_EXPORT PFNGLCOVERAGEMODULATIONTABLENVPROC __glewCoverageModulationTableNV;
GLEW_FUN_EXPORT PFNGLGETCOVERAGEMODULATIONTABLENVPROC __glewGetCoverageModulationTableNV;
GLEW_FUN_EXPORT PFNGLRASTERSAMPLESEXTPROC __glewRasterSamplesEXT;

GLEW_FUN_EXPORT PFNGLBEGINSCENEEXTPROC __glewBeginSceneEXT;
GLEW_FUN_EXPORT PFNGLENDSCENEEXTPROC __glewEndSceneEXT;

GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3BEXTPROC __glewSecondaryColor3bEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3BVEXTPROC __glewSecondaryColor3bvEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3DEXTPROC __glewSecondaryColor3dEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3DVEXTPROC __glewSecondaryColor3dvEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3FEXTPROC __glewSecondaryColor3fEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3FVEXTPROC __glewSecondaryColor3fvEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3IEXTPROC __glewSecondaryColor3iEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3IVEXTPROC __glewSecondaryColor3ivEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3SEXTPROC __glewSecondaryColor3sEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3SVEXTPROC __glewSecondaryColor3svEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UBEXTPROC __glewSecondaryColor3ubEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UBVEXTPROC __glewSecondaryColor3ubvEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UIEXTPROC __glewSecondaryColor3uiEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UIVEXTPROC __glewSecondaryColor3uivEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3USEXTPROC __glewSecondaryColor3usEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3USVEXTPROC __glewSecondaryColor3usvEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLORPOINTEREXTPROC __glewSecondaryColorPointerEXT;

GLEW_FUN_EXPORT PFNGLDELETESEMAPHORESEXTPROC __glewDeleteSemaphoresEXT;
GLEW_FUN_EXPORT PFNGLGENSEMAPHORESEXTPROC __glewGenSemaphoresEXT;
GLEW_FUN_EXPORT PFNGLGETSEMAPHOREPARAMETERUI64VEXTPROC __glewGetSemaphoreParameterui64vEXT;
GLEW_FUN_EXPORT PFNGLISSEMAPHOREEXTPROC __glewIsSemaphoreEXT;
GLEW_FUN_EXPORT PFNGLSEMAPHOREPARAMETERUI64VEXTPROC __glewSemaphoreParameterui64vEXT;
GLEW_FUN_EXPORT PFNGLSIGNALSEMAPHOREEXTPROC __glewSignalSemaphoreEXT;
GLEW_FUN_EXPORT PFNGLWAITSEMAPHOREEXTPROC __glewWaitSemaphoreEXT;

GLEW_FUN_EXPORT PFNGLIMPORTSEMAPHOREFDEXTPROC __glewImportSemaphoreFdEXT;

GLEW_FUN_EXPORT PFNGLIMPORTSEMAPHOREWIN32HANDLEEXTPROC __glewImportSemaphoreWin32HandleEXT;
GLEW_FUN_EXPORT PFNGLIMPORTSEMAPHOREWIN32NAMEEXTPROC __glewImportSemaphoreWin32NameEXT;

GLEW_FUN_EXPORT PFNGLACTIVEPROGRAMEXTPROC __glewActiveProgramEXT;
GLEW_FUN_EXPORT PFNGLCREATESHADERPROGRAMEXTPROC __glewCreateShaderProgramEXT;
GLEW_FUN_EXPORT PFNGLUSESHADERPROGRAMEXTPROC __glewUseShaderProgramEXT;

GLEW_FUN_EXPORT PFNGLBINDIMAGETEXTUREEXTPROC __glewBindImageTextureEXT;
GLEW_FUN_EXPORT PFNGLMEMORYBARRIEREXTPROC __glewMemoryBarrierEXT;

GLEW_FUN_EXPORT PFNGLCLEARPIXELLOCALSTORAGEUIEXTPROC __glewClearPixelLocalStorageuiEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERPIXELLOCALSTORAGESIZEEXTPROC __glewFramebufferPixelLocalStorageSizeEXT;
GLEW_FUN_EXPORT PFNGLGETFRAMEBUFFERPIXELLOCALSTORAGESIZEEXTPROC __glewGetFramebufferPixelLocalStorageSizeEXT;

GLEW_FUN_EXPORT PFNGLTEXPAGECOMMITMENTEXTPROC __glewTexPageCommitmentEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREPAGECOMMITMENTEXTPROC __glewTexturePageCommitmentEXT;

GLEW_FUN_EXPORT PFNGLACTIVESTENCILFACEEXTPROC __glewActiveStencilFaceEXT;

GLEW_FUN_EXPORT PFNGLTEXSUBIMAGE1DEXTPROC __glewTexSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLTEXSUBIMAGE2DEXTPROC __glewTexSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLTEXSUBIMAGE3DEXTPROC __glewTexSubImage3DEXT;

GLEW_FUN_EXPORT PFNGLTEXIMAGE3DEXTPROC __glewTexImage3DEXT;

GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC __glewFramebufferTextureLayerEXT;

GLEW_FUN_EXPORT PFNGLTEXBUFFEREXTPROC __glewTexBufferEXT;

GLEW_FUN_EXPORT PFNGLCLEARCOLORIIEXTPROC __glewClearColorIiEXT;
GLEW_FUN_EXPORT PFNGLCLEARCOLORIUIEXTPROC __glewClearColorIuiEXT;
GLEW_FUN_EXPORT PFNGLGETTEXPARAMETERIIVEXTPROC __glewGetTexParameterIivEXT;
GLEW_FUN_EXPORT PFNGLGETTEXPARAMETERIUIVEXTPROC __glewGetTexParameterIuivEXT;
GLEW_FUN_EXPORT PFNGLTEXPARAMETERIIVEXTPROC __glewTexParameterIivEXT;
GLEW_FUN_EXPORT PFNGLTEXPARAMETERIUIVEXTPROC __glewTexParameterIuivEXT;

GLEW_FUN_EXPORT PFNGLARETEXTURESRESIDENTEXTPROC __glewAreTexturesResidentEXT;
GLEW_FUN_EXPORT PFNGLBINDTEXTUREEXTPROC __glewBindTextureEXT;
GLEW_FUN_EXPORT PFNGLDELETETEXTURESEXTPROC __glewDeleteTexturesEXT;
GLEW_FUN_EXPORT PFNGLGENTEXTURESEXTPROC __glewGenTexturesEXT;
GLEW_FUN_EXPORT PFNGLISTEXTUREEXTPROC __glewIsTextureEXT;
GLEW_FUN_EXPORT PFNGLPRIORITIZETEXTURESEXTPROC __glewPrioritizeTexturesEXT;

GLEW_FUN_EXPORT PFNGLTEXTURENORMALEXTPROC __glewTextureNormalEXT;

GLEW_FUN_EXPORT PFNGLTEXSTORAGE1DEXTPROC __glewTexStorage1DEXT;
GLEW_FUN_EXPORT PFNGLTEXSTORAGE2DEXTPROC __glewTexStorage2DEXT;
GLEW_FUN_EXPORT PFNGLTEXSTORAGE3DEXTPROC __glewTexStorage3DEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE1DEXTPROC __glewTextureStorage1DEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE2DEXTPROC __glewTextureStorage2DEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE3DEXTPROC __glewTextureStorage3DEXT;

GLEW_FUN_EXPORT PFNGLTEXTUREVIEWEXTPROC __glewTextureViewEXT;

GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTI64VEXTPROC __glewGetQueryObjecti64vEXT;
GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTUI64VEXTPROC __glewGetQueryObjectui64vEXT;

GLEW_FUN_EXPORT PFNGLBEGINTRANSFORMFEEDBACKEXTPROC __glewBeginTransformFeedbackEXT;
GLEW_FUN_EXPORT PFNGLBINDBUFFERBASEEXTPROC __glewBindBufferBaseEXT;
GLEW_FUN_EXPORT PFNGLBINDBUFFEROFFSETEXTPROC __glewBindBufferOffsetEXT;
GLEW_FUN_EXPORT PFNGLBINDBUFFERRANGEEXTPROC __glewBindBufferRangeEXT;
GLEW_FUN_EXPORT PFNGLENDTRANSFORMFEEDBACKEXTPROC __glewEndTransformFeedbackEXT;
GLEW_FUN_EXPORT PFNGLGETTRANSFORMFEEDBACKVARYINGEXTPROC __glewGetTransformFeedbackVaryingEXT;
GLEW_FUN_EXPORT PFNGLTRANSFORMFEEDBACKVARYINGSEXTPROC __glewTransformFeedbackVaryingsEXT;

GLEW_FUN_EXPORT PFNGLARRAYELEMENTEXTPROC __glewArrayElementEXT;
GLEW_FUN_EXPORT PFNGLCOLORPOINTEREXTPROC __glewColorPointerEXT;
GLEW_FUN_EXPORT PFNGLDRAWARRAYSEXTPROC __glewDrawArraysEXT;
GLEW_FUN_EXPORT PFNGLEDGEFLAGPOINTEREXTPROC __glewEdgeFlagPointerEXT;
GLEW_FUN_EXPORT PFNGLINDEXPOINTEREXTPROC __glewIndexPointerEXT;
GLEW_FUN_EXPORT PFNGLNORMALPOINTEREXTPROC __glewNormalPointerEXT;
GLEW_FUN_EXPORT PFNGLTEXCOORDPOINTEREXTPROC __glewTexCoordPointerEXT;
GLEW_FUN_EXPORT PFNGLVERTEXPOINTEREXTPROC __glewVertexPointerEXT;

GLEW_FUN_EXPORT PFNGLBINDARRAYSETEXTPROC __glewBindArraySetEXT;
GLEW_FUN_EXPORT PFNGLCREATEARRAYSETEXTPROC __glewCreateArraySetExt;
GLEW_FUN_EXPORT PFNGLDELETEARRAYSETSEXTPROC __glewDeleteArraySetsEXT;

GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBLDVEXTPROC __glewGetVertexAttribLdvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXATTRIBLOFFSETEXTPROC __glewVertexArrayVertexAttribLOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1DEXTPROC __glewVertexAttribL1dEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1DVEXTPROC __glewVertexAttribL1dvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL2DEXTPROC __glewVertexAttribL2dEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL2DVEXTPROC __glewVertexAttribL2dvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL3DEXTPROC __glewVertexAttribL3dEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL3DVEXTPROC __glewVertexAttribL3dvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL4DEXTPROC __glewVertexAttribL4dEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL4DVEXTPROC __glewVertexAttribL4dvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBLPOINTEREXTPROC __glewVertexAttribLPointerEXT;

GLEW_FUN_EXPORT PFNGLBEGINVERTEXSHADEREXTPROC __glewBeginVertexShaderEXT;
GLEW_FUN_EXPORT PFNGLBINDLIGHTPARAMETEREXTPROC __glewBindLightParameterEXT;
GLEW_FUN_EXPORT PFNGLBINDMATERIALPARAMETEREXTPROC __glewBindMaterialParameterEXT;
GLEW_FUN_EXPORT PFNGLBINDPARAMETEREXTPROC __glewBindParameterEXT;
GLEW_FUN_EXPORT PFNGLBINDTEXGENPARAMETEREXTPROC __glewBindTexGenParameterEXT;
GLEW_FUN_EXPORT PFNGLBINDTEXTUREUNITPARAMETEREXTPROC __glewBindTextureUnitParameterEXT;
GLEW_FUN_EXPORT PFNGLBINDVERTEXSHADEREXTPROC __glewBindVertexShaderEXT;
GLEW_FUN_EXPORT PFNGLDELETEVERTEXSHADEREXTPROC __glewDeleteVertexShaderEXT;
GLEW_FUN_EXPORT PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC __glewDisableVariantClientStateEXT;
GLEW_FUN_EXPORT PFNGLENABLEVARIANTCLIENTSTATEEXTPROC __glewEnableVariantClientStateEXT;
GLEW_FUN_EXPORT PFNGLENDVERTEXSHADEREXTPROC __glewEndVertexShaderEXT;
GLEW_FUN_EXPORT PFNGLEXTRACTCOMPONENTEXTPROC __glewExtractComponentEXT;
GLEW_FUN_EXPORT PFNGLGENSYMBOLSEXTPROC __glewGenSymbolsEXT;
GLEW_FUN_EXPORT PFNGLGENVERTEXSHADERSEXTPROC __glewGenVertexShadersEXT;
GLEW_FUN_EXPORT PFNGLGETINVARIANTBOOLEANVEXTPROC __glewGetInvariantBooleanvEXT;
GLEW_FUN_EXPORT PFNGLGETINVARIANTFLOATVEXTPROC __glewGetInvariantFloatvEXT;
GLEW_FUN_EXPORT PFNGLGETINVARIANTINTEGERVEXTPROC __glewGetInvariantIntegervEXT;
GLEW_FUN_EXPORT PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC __glewGetLocalConstantBooleanvEXT;
GLEW_FUN_EXPORT PFNGLGETLOCALCONSTANTFLOATVEXTPROC __glewGetLocalConstantFloatvEXT;
GLEW_FUN_EXPORT PFNGLGETLOCALCONSTANTINTEGERVEXTPROC __glewGetLocalConstantIntegervEXT;
GLEW_FUN_EXPORT PFNGLGETVARIANTBOOLEANVEXTPROC __glewGetVariantBooleanvEXT;
GLEW_FUN_EXPORT PFNGLGETVARIANTFLOATVEXTPROC __glewGetVariantFloatvEXT;
GLEW_FUN_EXPORT PFNGLGETVARIANTINTEGERVEXTPROC __glewGetVariantIntegervEXT;
GLEW_FUN_EXPORT PFNGLGETVARIANTPOINTERVEXTPROC __glewGetVariantPointervEXT;
GLEW_FUN_EXPORT PFNGLINSERTCOMPONENTEXTPROC __glewInsertComponentEXT;
GLEW_FUN_EXPORT PFNGLISVARIANTENABLEDEXTPROC __glewIsVariantEnabledEXT;
GLEW_FUN_EXPORT PFNGLSETINVARIANTEXTPROC __glewSetInvariantEXT;
GLEW_FUN_EXPORT PFNGLSETLOCALCONSTANTEXTPROC __glewSetLocalConstantEXT;
GLEW_FUN_EXPORT PFNGLSHADEROP1EXTPROC __glewShaderOp1EXT;
GLEW_FUN_EXPORT PFNGLSHADEROP2EXTPROC __glewShaderOp2EXT;
GLEW_FUN_EXPORT PFNGLSHADEROP3EXTPROC __glewShaderOp3EXT;
GLEW_FUN_EXPORT PFNGLSWIZZLEEXTPROC __glewSwizzleEXT;
GLEW_FUN_EXPORT PFNGLVARIANTPOINTEREXTPROC __glewVariantPointerEXT;
GLEW_FUN_EXPORT PFNGLVARIANTBVEXTPROC __glewVariantbvEXT;
GLEW_FUN_EXPORT PFNGLVARIANTDVEXTPROC __glewVariantdvEXT;
GLEW_FUN_EXPORT PFNGLVARIANTFVEXTPROC __glewVariantfvEXT;
GLEW_FUN_EXPORT PFNGLVARIANTIVEXTPROC __glewVariantivEXT;
GLEW_FUN_EXPORT PFNGLVARIANTSVEXTPROC __glewVariantsvEXT;
GLEW_FUN_EXPORT PFNGLVARIANTUBVEXTPROC __glewVariantubvEXT;
GLEW_FUN_EXPORT PFNGLVARIANTUIVEXTPROC __glewVariantuivEXT;
GLEW_FUN_EXPORT PFNGLVARIANTUSVEXTPROC __glewVariantusvEXT;
GLEW_FUN_EXPORT PFNGLWRITEMASKEXTPROC __glewWriteMaskEXT;

GLEW_FUN_EXPORT PFNGLVERTEXWEIGHTPOINTEREXTPROC __glewVertexWeightPointerEXT;
GLEW_FUN_EXPORT PFNGLVERTEXWEIGHTFEXTPROC __glewVertexWeightfEXT;
GLEW_FUN_EXPORT PFNGLVERTEXWEIGHTFVEXTPROC __glewVertexWeightfvEXT;

GLEW_FUN_EXPORT PFNGLACQUIREKEYEDMUTEXWIN32EXTPROC __glewAcquireKeyedMutexWin32EXT;
GLEW_FUN_EXPORT PFNGLRELEASEKEYEDMUTEXWIN32EXTPROC __glewReleaseKeyedMutexWin32EXT;

GLEW_FUN_EXPORT PFNGLWINDOWRECTANGLESEXTPROC __glewWindowRectanglesEXT;

GLEW_FUN_EXPORT PFNGLIMPORTSYNCEXTPROC __glewImportSyncEXT;

GLEW_FUN_EXPORT PFNGLBLENDBARRIERKHRPROC __glewBlendBarrierKHR;

GLEW_FUN_EXPORT PFNGLDEBUGMESSAGECALLBACKPROC __glewDebugMessageCallback;
GLEW_FUN_EXPORT PFNGLDEBUGMESSAGECONTROLPROC __glewDebugMessageControl;
GLEW_FUN_EXPORT PFNGLDEBUGMESSAGEINSERTPROC __glewDebugMessageInsert;
GLEW_FUN_EXPORT PFNGLGETDEBUGMESSAGELOGPROC __glewGetDebugMessageLog;
GLEW_FUN_EXPORT PFNGLGETOBJECTLABELPROC __glewGetObjectLabel;
GLEW_FUN_EXPORT PFNGLGETOBJECTPTRLABELPROC __glewGetObjectPtrLabel;
GLEW_FUN_EXPORT PFNGLOBJECTLABELPROC __glewObjectLabel;
GLEW_FUN_EXPORT PFNGLOBJECTPTRLABELPROC __glewObjectPtrLabel;
GLEW_FUN_EXPORT PFNGLPOPDEBUGGROUPPROC __glewPopDebugGroup;
GLEW_FUN_EXPORT PFNGLPUSHDEBUGGROUPPROC __glewPushDebugGroup;

GLEW_FUN_EXPORT PFNGLMAXSHADERCOMPILERTHREADSKHRPROC __glewMaxShaderCompilerThreadsKHR;

GLEW_FUN_EXPORT PFNGLGETNUNIFORMFVPROC __glewGetnUniformfv;
GLEW_FUN_EXPORT PFNGLGETNUNIFORMIVPROC __glewGetnUniformiv;
GLEW_FUN_EXPORT PFNGLGETNUNIFORMUIVPROC __glewGetnUniformuiv;
GLEW_FUN_EXPORT PFNGLREADNPIXELSPROC __glewReadnPixels;

GLEW_FUN_EXPORT PFNGLEXTGETBUFFERPOINTERVQCOMPROC __glewExtGetBufferPointervQCOM;
GLEW_FUN_EXPORT PFNGLEXTGETBUFFERSQCOMPROC __glewExtGetBuffersQCOM;
GLEW_FUN_EXPORT PFNGLEXTGETFRAMEBUFFERSQCOMPROC __glewExtGetFramebuffersQCOM;
GLEW_FUN_EXPORT PFNGLEXTGETRENDERBUFFERSQCOMPROC __glewExtGetRenderbuffersQCOM;
GLEW_FUN_EXPORT PFNGLEXTGETTEXLEVELPARAMETERIVQCOMPROC __glewExtGetTexLevelParameterivQCOM;
GLEW_FUN_EXPORT PFNGLEXTGETTEXSUBIMAGEQCOMPROC __glewExtGetTexSubImageQCOM;
GLEW_FUN_EXPORT PFNGLEXTGETTEXTURESQCOMPROC __glewExtGetTexturesQCOM;
GLEW_FUN_EXPORT PFNGLEXTTEXOBJECTSTATEOVERRIDEIQCOMPROC __glewExtTexObjectStateOverrideiQCOM;

GLEW_FUN_EXPORT PFNGLEXTGETPROGRAMBINARYSOURCEQCOMPROC __glewExtGetProgramBinarySourceQCOM;
GLEW_FUN_EXPORT PFNGLEXTGETPROGRAMSQCOMPROC __glewExtGetProgramsQCOM;
GLEW_FUN_EXPORT PFNGLEXTGETSHADERSQCOMPROC __glewExtGetShadersQCOM;
GLEW_FUN_EXPORT PFNGLEXTISPROGRAMBINARYQCOMPROC __glewExtIsProgramBinaryQCOM;

GLEW_FUN_EXPORT PFNGLGETEXTENSIONREGALPROC __glewGetExtensionREGAL;
GLEW_FUN_EXPORT PFNGLISSUPPORTEDREGALPROC __glewIsSupportedREGAL;

GLEW_FUN_EXPORT PFNGLADDSWAPHINTRECTWINPROC __glewAddSwapHintRectWIN;

GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_1_1;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_1_2;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_1_2_1;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_1_3;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_1_4;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_1_5;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_2_0;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_2_1;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_3_0;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_3_1;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_3_2;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_3_3;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_4_0;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_4_1;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_4_2;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_4_3;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_4_4;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_4_5;
GLEW_VAR_EXPORT GLboolean __GLEW_VERSION_4_6;
GLEW_VAR_EXPORT GLboolean __GLEW_AMD_stencil_operation_extended;
GLEW_VAR_EXPORT GLboolean __GLEW_ANDROID_extension_pack_es31a;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_ES2_compatibility;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_ES3_1_compatibility;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_ES3_2_compatibility;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_ES3_compatibility;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_arrays_of_arrays;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_base_instance;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_bindless_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_blend_func_extended;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_buffer_storage;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_cl_event;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_clear_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_clear_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_clip_control;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_color_buffer_float;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_compatibility;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_compressed_texture_pixel_storage;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_compute_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_compute_variable_group_size;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_conditional_render_inverted;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_conservative_depth;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_copy_buffer;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_copy_image;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_cull_distance;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_debug_output;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_depth_buffer_float;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_depth_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_depth_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_derivative_control;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_direct_state_access;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_draw_buffers;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_draw_buffers_blend;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_draw_elements_base_vertex;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_draw_indirect;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_draw_instanced;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_enhanced_layouts;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_explicit_attrib_location;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_explicit_uniform_location;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_fragment_coord_conventions;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_fragment_layer_viewport;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_fragment_program;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_fragment_program_shadow;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_fragment_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_fragment_shader_interlock;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_framebuffer_no_attachments;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_framebuffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_framebuffer_sRGB;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_geometry_shader4;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_get_program_binary;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_get_texture_sub_image;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_gl_spirv;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_gpu_shader5;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_gpu_shader_fp64;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_gpu_shader_int64;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_half_float_pixel;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_half_float_vertex;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_imaging;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_indirect_parameters;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_instanced_arrays;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_internalformat_query;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_internalformat_query2;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_invalidate_subdata;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_map_buffer_alignment;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_map_buffer_range;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_matrix_palette;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_multi_bind;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_multi_draw_indirect;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_multitexture;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_occlusion_query;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_occlusion_query2;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_parallel_shader_compile;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_pipeline_statistics_query;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_pixel_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_point_parameters;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_point_sprite;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_polygon_offset_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_post_depth_coverage;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_program_interface_query;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_provoking_vertex;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_query_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_robust_buffer_access_behavior;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_robustness;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_robustness_application_isolation;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_robustness_share_group_isolation;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_sample_locations;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_sample_shading;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_sampler_objects;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_seamless_cube_map;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_seamless_cubemap_per_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_separate_shader_objects;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_atomic_counter_ops;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_atomic_counters;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_ballot;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_bit_encoding;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_clock;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_draw_parameters;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_group_vote;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_image_load_store;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_image_size;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_objects;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_precision;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_stencil_export;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_storage_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_subroutine;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_texture_image_samples;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_texture_lod;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shader_viewport_layer_array;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shading_language_100;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shading_language_420pack;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shading_language_include;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shading_language_packing;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shadow;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_shadow_ambient;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_sparse_buffer;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_sparse_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_sparse_texture2;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_sparse_texture_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_spirv_extensions;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_stencil_texturing;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_sync;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_tessellation_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_barrier;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_border_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_buffer_object_rgb32;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_buffer_range;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_compression;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_compression_bptc;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_compression_rgtc;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_cube_map;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_cube_map_array;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_env_add;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_env_combine;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_env_crossbar;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_env_dot3;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_filter_anisotropic;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_filter_minmax;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_float;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_gather;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_mirror_clamp_to_edge;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_mirrored_repeat;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_non_power_of_two;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_query_levels;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_query_lod;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_rectangle;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_rg;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_rgb10_a2ui;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_stencil8;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_storage;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_storage_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_swizzle;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_texture_view;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_timer_query;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_transform_feedback2;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_transform_feedback3;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_transform_feedback_instanced;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_transform_feedback_overflow_query;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_transpose_matrix;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_uniform_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_array_bgra;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_array_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_attrib_64bit;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_attrib_binding;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_blend;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_program;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_type_10f_11f_11f_rev;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_vertex_type_2_10_10_10_rev;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_viewport_array;
GLEW_VAR_EXPORT GLboolean __GLEW_ARB_window_pos;
GLEW_VAR_EXPORT GLboolean __GLEW_EGL_KHR_context_flush_control;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_422_pixels;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_Cg_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_EGL_image_array;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_YUV_target;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_abgr;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_base_instance;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_bgra;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_bindable_uniform;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_blend_color;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_blend_equation_separate;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_blend_func_extended;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_blend_func_separate;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_blend_logic_op;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_blend_minmax;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_blend_subtract;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_buffer_storage;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_clear_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_clip_cull_distance;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_clip_volume_hint;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_cmyka;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_color_buffer_float;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_color_buffer_half_float;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_color_subtable;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_compiled_vertex_array;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_compressed_ETC1_RGB8_sub_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_conservative_depth;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_convolution;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_coordinate_frame;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_copy_image;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_copy_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_cull_vertex;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_debug_label;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_debug_marker;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_depth_bounds_test;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_direct_state_access;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_discard_framebuffer;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_draw_buffers;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_draw_buffers2;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_draw_buffers_indexed;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_draw_elements_base_vertex;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_draw_instanced;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_draw_range_elements;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_external_buffer;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_float_blend;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_fog_coord;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_frag_depth;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_fragment_lighting;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_framebuffer_blit;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_framebuffer_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_framebuffer_multisample_blit_scaled;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_framebuffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_framebuffer_sRGB;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_geometry_point_size;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_geometry_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_geometry_shader4;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_gpu_program_parameters;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_gpu_shader4;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_gpu_shader5;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_histogram;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_index_array_formats;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_index_func;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_index_material;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_index_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_instanced_arrays;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_light_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_map_buffer_range;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_memory_object;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_memory_object_fd;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_memory_object_win32;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_misc_attribute;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_multi_draw_arrays;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_multi_draw_indirect;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_multiple_textures;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_multisample_compatibility;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_multisampled_render_to_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_multisampled_render_to_texture2;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_multiview_draw_buffers;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_packed_depth_stencil;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_packed_float;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_packed_pixels;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_paletted_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_pixel_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_pixel_transform;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_pixel_transform_color_table;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_point_parameters;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_polygon_offset;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_polygon_offset_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_post_depth_coverage;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_provoking_vertex;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_pvrtc_sRGB;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_raster_multisample;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_read_format_bgra;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_render_snorm;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_rescale_normal;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_sRGB;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_sRGB_write_control;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_scene_marker;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_secondary_color;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_semaphore;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_semaphore_fd;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_semaphore_win32;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_separate_shader_objects;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_separate_specular_color;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shader_framebuffer_fetch;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shader_group_vote;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shader_image_load_formatted;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shader_image_load_store;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shader_implicit_conversions;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shader_integer_mix;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shader_io_blocks;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shader_non_constant_global_initializers;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shader_pixel_local_storage;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shader_pixel_local_storage2;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shader_texture_lod;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shadow_funcs;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shadow_samplers;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_shared_texture_palette;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_sparse_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_sparse_texture2;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_stencil_clear_tag;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_stencil_two_side;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_stencil_wrap;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_subtexture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture3D;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_array;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_buffer_object;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_compression_astc_decode_mode;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_compression_astc_decode_mode_rgb9e5;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_compression_bptc;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_compression_dxt1;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_compression_latc;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_compression_rgtc;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_compression_s3tc;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_cube_map;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_cube_map_array;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_edge_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_env;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_env_add;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_env_combine;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_env_dot3;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_filter_anisotropic;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_filter_minmax;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_format_BGRA8888;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_integer;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_lod_bias;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_mirror_clamp;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_norm16;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_object;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_perturb_normal;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_rectangle;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_rg;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_sRGB;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_sRGB_R8;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_sRGB_RG8;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_sRGB_decode;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_shared_exponent;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_snorm;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_storage;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_swizzle;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_type_2_10_10_10_REV;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_texture_view;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_timer_query;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_transform_feedback;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_unpack_subimage;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_vertex_array;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_vertex_array_bgra;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_vertex_array_setXXX;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_vertex_attrib_64bit;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_vertex_shader;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_vertex_weighting;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_win32_keyed_mutex;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_window_rectangles;
GLEW_VAR_EXPORT GLboolean __GLEW_EXT_x11_sync_object;
GLEW_VAR_EXPORT GLboolean __GLEW_KHR_blend_equation_advanced;
GLEW_VAR_EXPORT GLboolean __GLEW_KHR_blend_equation_advanced_coherent;
GLEW_VAR_EXPORT GLboolean __GLEW_KHR_context_flush_control;
GLEW_VAR_EXPORT GLboolean __GLEW_KHR_debug;
GLEW_VAR_EXPORT GLboolean __GLEW_KHR_no_error;
GLEW_VAR_EXPORT GLboolean __GLEW_KHR_parallel_shader_compile;
GLEW_VAR_EXPORT GLboolean __GLEW_KHR_robust_buffer_access_behavior;
GLEW_VAR_EXPORT GLboolean __GLEW_KHR_robustness;
GLEW_VAR_EXPORT GLboolean __GLEW_KHR_texture_compression_astc_hdr;
GLEW_VAR_EXPORT GLboolean __GLEW_KHR_texture_compression_astc_ldr;
GLEW_VAR_EXPORT GLboolean __GLEW_KHR_texture_compression_astc_sliced_3d;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_EGL_stream_consumer_external;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_gpu_program5_mem_extended;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_shader_atomic_float64;
GLEW_VAR_EXPORT GLboolean __GLEW_NV_shader_atomic_int64;
GLEW_VAR_EXPORT GLboolean __GLEW_QCOM_extended_get;
GLEW_VAR_EXPORT GLboolean __GLEW_QCOM_extended_get2;
GLEW_VAR_EXPORT GLboolean __GLEW_REGAL_extension_query;
GLEW_VAR_EXPORT GLboolean __GLEW_WIN_phong_shading;
GLEW_VAR_EXPORT GLboolean __GLEW_WIN_scene_markerXXX;
GLEW_VAR_EXPORT GLboolean __GLEW_WIN_specular_fog;
GLEW_VAR_EXPORT GLboolean __GLEW_WIN_swap_hint;

#ifdef GLEW_MX
}; /* GLEWContextStruct */
#endif /* GLEW_MX */

/* ------------------------------------------------------------------------- */

/* error codes */
#define GLEW_OK 0
#define GLEW_NO_ERROR 0
#define GLEW_ERROR_NO_GL_VERSION 1  /* missing GL version */
#define GLEW_ERROR_GL_VERSION_10_ONLY 2  /* Need at least OpenGL 1.1 */
#define GLEW_ERROR_GLX_VERSION_11_ONLY 3  /* Need at least GLX 1.2 */

/* string codes */
#define GLEW_VERSION 1
#define GLEW_VERSION_MAJOR 2
#define GLEW_VERSION_MINOR 3
#define GLEW_VERSION_MICRO 4

/* ------------------------------------------------------------------------- */

/* GLEW version info */

/*
VERSION 1.13.0
VERSION_MAJOR 1
VERSION_MINOR 13
VERSION_MICRO 1
*/

/* API */
#ifdef GLEW_MX

typedef struct GLEWContextStruct GLEWContext;
GLEWAPI GLenum GLEWAPIENTRY glewContextInit (GLEWContext *ctx);
GLEWAPI GLboolean GLEWAPIENTRY glewContextIsSupported (const GLEWContext *ctx, const char *name);

#define glewInit() glewContextInit(glewGetContext())
#define glewIsSupported(x) glewContextIsSupported(glewGetContext(), x)
#define glewIsExtensionSupported(x) glewIsSupported(x)

#define GLEW_GET_VAR(x) (*reinterpret_cast<const GLboolean*>(&(glewGetContext()->x)))
#define GLEW_GET_FUN(x) glewGetContext()->x

#else /* GLEW_MX */

GLEWAPI GLenum GLEWAPIENTRY glewInit (void);
GLEWAPI GLboolean GLEWAPIENTRY glewIsSupported (const char *name);
#define glewIsExtensionSupported(x) glewIsSupported(x)

#define GLEW_GET_VAR(x) (*(const GLboolean*)&x)
#define GLEW_GET_FUN(x) x

#endif /* GLEW_MX */

GLEWAPI GLboolean glewExperimental;
GLEWAPI GLboolean GLEWAPIENTRY glewGetExtension (const char *name);
const GLEWAPI GLubyte * GLEWAPIENTRY glewGetErrorString (GLenum error);
const GLEWAPI GLubyte * GLEWAPIENTRY glewGetString (GLenum name);

#ifdef __cplusplus
}
#endif

#ifdef GLEW_APIENTRY_DEFINED
#undef GLEW_APIENTRY_DEFINED
#undef APIENTRY
#endif

#ifdef GLEW_CALLBACK_DEFINED
#undef GLEW_CALLBACK_DEFINED
#undef CALLBACK
#endif

#ifdef GLEW_WINGDIAPI_DEFINED
#undef GLEW_WINGDIAPI_DEFINED
#undef WINGDIAPI
#endif

#undef GLAPI
/* #undef GLEWAPI */

