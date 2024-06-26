/* Copyright 2023-2024 Kinglet B.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef GL_ES2_IMPL_H
#define GL_ES2_IMPL_H

#ifndef GL_ES2_IMPL_TYPES_H_INCLUDED
#define GL_ES2_IMPL_TYPES_H_INCLUDED
#include "gl_es2_impl_types.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Declaration specifier */
#define GL_ES2_DECL_SPEC 

/* Declarator attribute */
#define GL_ES2_DECLARATOR_ATTRIB

/* Function identifier decoration */
#define GL_ES2_FUNCTION_ID(id) aex_gl_es2_##id

#define GL_ES2_FALSE 0
#define GL_ES2_TRUE  1

#define GL_ES2_NO_ERROR                      0x0000
#define GL_ES2_INVALID_ENUM                  0x0500
#define GL_ES2_INVALID_VALUE                 0x0501
#define GL_ES2_INVALID_OPERATION             0x0502
#define GL_ES2_OUT_OF_MEMORY                 0x0505
#define GL_ES2_INVALID_FRAMEBUFFER_OPERATION 0x0506
  
#define GL_ES2_FRAMEBUFFER_COMPLETE                       0x8CD5
#define GL_ES2_FRAMEBUFFER_INCOMPLETE_ATTACHMENT          0x8CD6
#define GL_ES2_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT  0x8CD7
#define GL_ES2_FRAMEBUFFER_INCOMPLETE_DIMENSIONS          0x8CD9
#define GL_ES2_FRAMEBUFFER_UNSUPPORTED                    0x8CDD

#define GL_ES2_FRAMEBUFFER  0x8D40
#define GL_ES2_RENDERBUFFER 0x8D41
#define GL_ES2_TEXTURE      0x1702
#define GL_ES2_NONE         0

#define GL_ES2_TEXTURE0                         0x84C0
#define GL_ES2_ACTIVE_TEXTURE                   0x84E0
#define GL_ES2_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D

#define GL_ES2_COLOR_ATTACHMENT0   0x8CE0
#define GL_ES2_DEPTH_ATTACHMENT    0x8D00
#define GL_ES2_STENCIL_ATTACHMENT  0x8D20

#define GL_ES2_TEXTURE_2D       0x0DE1
#define GL_ES2_TEXTURE_CUBE_MAP 0x8513

#define GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A

#define GL_ES2_ALPHA           0x1906
#define GL_ES2_LUMINANCE       0x1909
#define GL_ES2_LUMINANCE_ALPHA 0x190A
#define GL_ES2_RGB             0x1907
#define GL_ES2_RGBA            0x1908

#define GL_ES2_POINTS              0x0000
#define GL_ES2_LINE_STRIP          0x0003
#define GL_ES2_LINE_LOOP           0x0002
#define GL_ES2_LINES               0x0001
#define GL_ES2_TRIANGLE_STRIP      0x0005
#define GL_ES2_TRIANGLE_FAN        0x0006
#define GL_ES2_TRIANGLES           0x0004

#define GL_ES2_ARRAY_BUFFER         0x8892
#define GL_ES2_ELEMENT_ARRAY_BUFFER 0x8893

#define AEX_GL_DEBUG_SHADER 0x108B31 /* internal use, e.g. testing */

#define GL_ES2_FRAGMENT_SHADER 0x8B30
#define GL_ES2_VERTEX_SHADER   0x8B31

#define GL_ES2_FUNC_ADD               0x8006
#define GL_ES2_FUNC_SUBTRACT          0x800A
#define GL_ES2_FUNC_REVERSE_SUBTRACT  0x800B

#define GL_ES2_ZERO                           0
#define GL_ES2_ONE                            1
#define GL_ES2_SRC_COLOR                 0x0300
#define GL_ES2_ONE_MINUS_SRC_COLOR       0x0301
#define GL_ES2_DST_COLOR                 0x0306
#define GL_ES2_ONE_MINUS_DST_COLOR       0x0307
#define GL_ES2_SRC_ALPHA                 0x0302
#define GL_ES2_ONE_MINUS_SRC_ALPHA       0x0303
#define GL_ES2_DST_ALPHA                 0x0304
#define GL_ES2_ONE_MINUS_DST_ALPHA       0x0305
#define GL_ES2_CONSTANT_COLOR            0x8001
#define GL_ES2_ONE_MINUS_CONSTANT_COLOR  0x8002
#define GL_ES2_CONSTANT_ALPHA            0x8003
#define GL_ES2_ONE_MINUS_CONSTANT_ALPHA  0x8004
#define GL_ES2_SRC_ALPHA_SATURATE        0x0308

#define GL_ES2_REPLACE   0x1E01
#define GL_ES2_INCR      0x1E02
#define GL_ES2_DECR      0x1E03
#define GL_ES2_INCR_WRAP 0x8507
#define GL_ES2_DECR_WRAP 0x8508
#define GL_ES2_KEEP      0x1E00
#define GL_ES2_INVERT    0x150A

#define GL_ES2_STREAM_DRAW  0x88E0
#define GL_ES2_STATIC_DRAW  0x88E4
#define GL_ES2_DYNAMIC_DRAW 0x88E8

#define GL_ES2_BLEND        0x0BE2
#define GL_ES2_DITHER       0x0BD0
#define GL_ES2_DEPTH_TEST   0x0B71
#define GL_ES2_POLYGON_OFFSET_FILL 0x8037
#define GL_ES2_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_ES2_SAMPLE_COVERAGE 0x80A0

#define GL_ES2_STENCIL_TEST 0x0B90
#define GL_ES2_SCISSOR_TEST 0x0C11
#define GL_ES2_CULL_FACE    0x0B44

#define GL_ES2_DEPTH_RANGE       0x0B70
#define GL_ES2_FRONT_FACE        0x0B46
#define GL_ES2_CULL_FACE_MODE    0x0B45
#define GL_ES2_MAX_VIEWPORT_DIMS 0x0D3A
#define GL_ES2_MAX_TEXTURE_SIZE  0x0D33
#define GL_ES2_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C

#define GL_ES2_VIEWPORT          0x0BA2
#define GL_ES2_STENCIL_FUNC            0x0B92
#define GL_ES2_STENCIL_VALUE_MASK      0x0B93
#define GL_ES2_STENCIL_REF             0x0B97
#define GL_ES2_STENCIL_BACK_FUNC       0x8800
#define GL_ES2_STENCIL_BACK_VALUE_MASK 0x8CA4
#define GL_ES2_STENCIL_BACK_REF        0x8CA3
#define GL_ES2_STENCIL_BITS      0x0D57
#define GL_ES2_STENCIL_FAIL                 0x0B94
#define GL_ES2_STENCIL_PASS_DEPTH_PASS      0x0B96
#define GL_ES2_STENCIL_PASS_DEPTH_FAIL      0x0B95
#define GL_ES2_STENCIL_BACK_FAIL            0x8801
#define GL_ES2_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
#define GL_ES2_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
#define GL_ES2_POLYGON_OFFSET_FACTOR 0x8038
#define GL_ES2_POLYGON_OFFSET_UNITS  0x2A00

#define GL_ES2_NEVER    0x0200
#define GL_ES2_LESS     0x0201
#define GL_ES2_EQUAL    0x0202
#define GL_ES2_LEQUAL   0x0203
#define GL_ES2_GREATER  0x0204
#define GL_ES2_NOTEQUAL 0x0205
#define GL_ES2_GEQUAL   0x0206
#define GL_ES2_ALWAYS   0x0207

#define GL_ES2_UNSIGNED_BYTE  0x1401
#define GL_ES2_UNSIGNED_SHORT 0x1403
#define GL_ES2_UNSIGNED_INT   0x1405

#define GL_ES2_BYTE  0x1400
#define GL_ES2_SHORT 0x1402
#define GL_ES2_FIXED 0x140C

#define GL_ES2_FLOAT      0x1406
#define GL_ES2_FLOAT_VEC2 0x8B50
#define GL_ES2_FLOAT_VEC3 0x8B51
#define GL_ES2_FLOAT_VEC4 0x8B52
#define GL_ES2_FLOAT_MAT2 0x8B5A
#define GL_ES2_FLOAT_MAT3 0x8B5B
#define GL_ES2_FLOAT_MAT4 0x8B5C
#define GL_ES2_INT        0x1404
#define GL_ES2_INT_VEC2   0x8B53
#define GL_ES2_INT_VEC3   0x8B54
#define GL_ES2_INT_VEC4   0x8B55
#define GL_ES2_BOOL       0x8B56
#define GL_ES2_BOOL_VEC2  0x8B57
#define GL_ES2_BOOL_VEC3  0x8B58
#define GL_ES2_BOOL_VEC4  0x8B59
#define GL_ES2_SAMPLER_2D 0x8B5E
#define GL_ES2_SAMPLER_CUBE 0x8B60

#define GL_ES2_ACTIVE_ATTRIBUTES           0x8B89
#define GL_ES2_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_ES2_ACTIVE_UNIFORMS             0x8B86
#define GL_ES2_ACTIVE_UNIFORM_MAX_LENGTH   0x8B87
#define GL_ES2_INFO_LOG_LENGTH             0x8B84
#define GL_ES2_DELETE_STATUS               0x8B80
#define GL_ES2_LINK_STATUS                 0x8B82
#define GL_ES2_VALIDATE_STATUS             0x8B83
#define GL_ES2_ATTACHED_SHADERS            0x8B85

#define GL_ES2_SHADER_TYPE          0x8B4F
#define GL_ES2_COMPILE_STATUS       0x8B81
#define GL_ES2_SHADER_SOURCE_LENGTH 0x8B88

#define GL_ES2_RENDERBUFFER_WIDTH           0x8D42
#define GL_ES2_RENDERBUFFER_HEIGHT          0x8D43
#define GL_ES2_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#define GL_ES2_RENDERBUFFER_RED_SIZE        0x8D50
#define GL_ES2_RENDERBUFFER_GREEN_SIZE      0x8D51
#define GL_ES2_RENDERBUFFER_BLUE_SIZE       0x8D52
#define GL_ES2_RENDERBUFFER_ALPHA_SIZE      0x8D53
#define GL_ES2_RENDERBUFFER_DEPTH_SIZE      0x8D54
#define GL_ES2_RENDERBUFFER_STENCIL_SIZE    0x8D55

#define GL_ES2_RGB8     0x8051
#define GL_ES2_RGBA4    0x8056
#define GL_ES2_RGBA5_A1 0x8057
#define GL_ES2_RGBA8    0x8058
#define GL_ES2_RGB565   0x8D62
#define GL_ES2_DEPTH_COMPONENT 0x1902 /* This is from GL_OES_depth_texture, which we don't support, but we use it for glReadPixels. */
#define GL_ES2_DEPTH_COMPONENT16 0x81A5
#define GL_ES2_DEPTH_COMPONENT24 0x81A6
#define GL_ES2_DEPTH_COMPONENT32 0x81A7 /* GL_OES_depth32; not in OpenGL ES 2.0/3.0 but happens to be one we support */
#define GL_ES2_STENCIL_INDEX8  0x8D48
#define GL_ES2_STENCIL_INDEX16 0x8D49
#define GL_ES2_DEPTH24_STENCIL8 0x88F0

#define GL_ES2_NEAREST                0x2600 
#define GL_ES2_LINEAR                 0x2601
#define GL_ES2_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_ES2_LINEAR_MIPMAP_NEAREST  0x2701
#define GL_ES2_NEAREST_MIPMAP_LINEAR  0x2702
#define GL_ES2_LINEAR_MIPMAP_LINEAR   0x2703

#define GL_ES2_TEXTURE_MAG_FILTER 0x2800
#define GL_ES2_TEXTURE_MIN_FILTER 0x2801
#define GL_ES2_TEXTURE_WRAP_S     0x2802
#define GL_ES2_TEXTURE_WRAP_T     0x2803
#define GL_ES2_TEXTURE_MAX_LEVEL  0x813D

#define GL_ES2_CLAMP_TO_EDGE   0x812F
#define GL_ES2_REPEAT          0x2901
#define GL_ES2_MIRRORED_REPEAT 0x8370

#define GL_ES2_LOW_FLOAT    0x8DF0
#define GL_ES2_MEDIUM_FLOAT 0x8DF1
#define GL_ES2_HIGH_FLOAT   0x8DF2
#define GL_ES2_LOW_INT      0x8DF3
#define GL_ES2_MEDIUM_INT   0x8DF4
#define GL_ES2_HIGH_INT     0x8DF5

#define GL_ES2_FRONT          0x0404
#define GL_ES2_BACK           0x0405
#define GL_ES2_FRONT_AND_BACK 0x0408

#define GL_ES2_BUFFER_SIZE  0x8764
#define GL_ES2_BUFFER_USAGE 0x8765

#define GL_ES2_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE   0x8CD0
#define GL_ES2_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME   0x8CD1
#define GL_ES2_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_ES2_FRAMEBUFFER_ATTACHMENT_CUBE_MAP_FACE 0x8CD3

#define GL_ES2_CW  0x0900
#define GL_ES2_CCW 0x0901

#define GL_ES2_COLOR_BUFFER_BIT   0x4000
#define GL_ES2_DEPTH_BUFFER_BIT    0x100
#define GL_ES2_STENCIL_BUFFER_BIT  0x400

#define GL_ES2_MAX_VERTEX_ATTRIBS 0x8869

#define GL_ES2_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_ES2_COMPRESSED_TEXTURE_FORMATS     0x86A3

#define GL_ES2_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_ES2_VERTEX_ATTRIB_ARRAY_ENABLED        0x8622
#define GL_ES2_VERTEX_ATTRIB_ARRAY_SIZE           0x8623
#define GL_ES2_VERTEX_ATTRIB_ARRAY_STRIDE         0x8624
#define GL_ES2_VERTEX_ATTRIB_ARRAY_TYPE           0x8625
#define GL_ES2_VERTEX_ATTRIB_ARRAY_NORMALIZED     0x886A
#define GL_ES2_CURRENT_VERTEX_ATTRIB              0x8626

#define GL_ES2_VERTEX_ATTRIB_ARRAY_POINTER 0x8645

#define GL_ES2_GENERATE_MIPMAP_HINT 0x8192
#define GL_ES2_FASTEST              0x1101
#define GL_ES2_NICEST               0x1102
#define GL_ES2_DONT_CARE            0x1100

#define GL_ES2_ALIASED_LINE_WIDTH_RANGE 0x846E
#define GL_ES2_ALIASED_POINT_SIZE_RANGE 0x846D
#define GL_ES2_LINE_WIDTH               0x0B21

#define GL_ES2_PACK_ALIGNMENT   0x0D05
#define GL_ES2_UNPACK_ALIGNMENT 0x0CF5

#define GL_ES2_UNSIGNED_SHORT_5_6_5   0x8363
#define GL_ES2_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_ES2_UNSIGNED_SHORT_5_5_5_1 0x8034

#define GL_ES2_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_ES2_IMPLEMENTATION_COLOR_READ_TYPE   0x8B9A

#define GL_ES2_SAMPLE_COVERAGE_VALUE  0x80AA
#define GL_ES2_SAMPLE_COVERAGE_INVERT 0x80AB
#define GL_ES2_SAMPLE_BUFFERS         0x80A8
#define GL_ES2_SAMPLES                0x80A9

#define GL_ES2_NUM_SHADER_BINARY_FORMATS 0x8DF9
#define GL_ES2_SHADER_BINARY_FORMATS     0x8DF8
#define GL_ES2_SHADER_COMPILER           0x8DFA

#define GL_ES2_CURRENT_PROGRAM  0x8B8D

#define GL_ES2_VENDOR                   0x1F00
#define GL_ES2_RENDERER                 0x1F01
#define GL_ES2_VERSION                  0x1F02
#define GL_ES2_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_ES2_EXTENSIONS               0x1F03
  
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ActiveTexture)(gl_es2_enum texture);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(AttachShader)(gl_es2_uint program, gl_es2_uint shader);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindAttribLocation)(gl_es2_uint program, gl_es2_uint index, const gl_es2_char *name);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindBuffer)(gl_es2_enum target, gl_es2_uint buffer);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindFramebuffer)(gl_es2_enum target, gl_es2_uint framebuffer);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindRenderbuffer)(gl_es2_enum target, gl_es2_uint renderbuffer);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindTexture)(gl_es2_enum target, gl_es2_uint texture);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendColor)(gl_es2_float red, gl_es2_float green, gl_es2_float blue, gl_es2_float alpha);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendEquation)(gl_es2_enum mode);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendEquationSeparate)(gl_es2_enum modeRGB, gl_es2_enum modeAlpha);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendFunc)(gl_es2_enum sfactor, gl_es2_enum dfactor);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendFuncSeparate)(gl_es2_enum sfactorRGB, gl_es2_enum dfactorRGB, gl_es2_enum sfactorAlpha, gl_es2_enum dfactorAlpha);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BufferData)(gl_es2_enum target, gl_es2_sizeiptr size, const void *data, gl_es2_enum usage);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BufferSubData)(gl_es2_enum target, gl_es2_intptr offset, gl_es2_sizeiptr size, const void *data);
GL_ES2_DECL_SPEC gl_es2_enum GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CheckFramebufferStatus)(gl_es2_enum target);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Clear)(gl_es2_bitfield mask);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ClearColor)(gl_es2_float red, gl_es2_float green, gl_es2_float blue, gl_es2_float alpha);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ClearDepthf)(gl_es2_float d);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ClearStencil)(gl_es2_int s);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ColorMask)(gl_es2_boolean red, gl_es2_boolean green, gl_es2_boolean blue, gl_es2_boolean alpha);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CompileShader)(gl_es2_uint shader);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CompressedTexImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_enum internalformat, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border, gl_es2_sizei imageSize, const void *data);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CompressedTexSubImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_int xoffset, gl_es2_int yoffset, gl_es2_sizei width, gl_es2_sizei height, gl_es2_enum format, gl_es2_sizei imageSize, const void *data);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CopyTexImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_enum internalformat, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CopyTexSubImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_int xoffset, gl_es2_int yoffset, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height);
GL_ES2_DECL_SPEC gl_es2_uint GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CreateProgram)(void);
GL_ES2_DECL_SPEC gl_es2_uint GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CreateShader)(gl_es2_enum type);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CullFace)(gl_es2_enum mode);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteBuffers)(gl_es2_sizei n, const gl_es2_uint *buffers);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteFramebuffers)(gl_es2_sizei n, const gl_es2_uint *framebuffers);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteProgram)(gl_es2_uint program);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteRenderbuffers)(gl_es2_sizei n, const gl_es2_uint *renderbuffers);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteShader)(gl_es2_uint shader);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteTextures)(gl_es2_sizei n, const gl_es2_uint *textures);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DepthFunc)(gl_es2_enum func);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DepthMask)(gl_es2_boolean flag);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DepthRangef)(gl_es2_float n, gl_es2_float f);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DetachShader)(gl_es2_uint program, gl_es2_uint shader);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Disable)(gl_es2_enum cap);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DisableVertexAttribArray)(gl_es2_uint index);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DrawArrays)(gl_es2_enum mode, gl_es2_int first, gl_es2_sizei count);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DrawElements)(gl_es2_enum mode, gl_es2_sizei count, gl_es2_enum type, const void *indices);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Enable)(gl_es2_enum cap);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(EnableVertexAttribArray)(gl_es2_uint index);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Finish)(void);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Flush)(void);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(FramebufferRenderbuffer)(gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum renderbuffertarget, gl_es2_uint renderbuffer);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(FramebufferTexture2D)(gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum textarget, gl_es2_uint texture, gl_es2_int level);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(FrontFace)(gl_es2_enum mode);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenBuffers)(gl_es2_sizei n, gl_es2_uint *buffers);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenerateMipmap)(gl_es2_enum target);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenFramebuffers)(gl_es2_sizei n, gl_es2_uint *framebuffers);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenRenderbuffers)(gl_es2_sizei n, gl_es2_uint *renderbuffers);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenTextures)(gl_es2_sizei n, gl_es2_uint *textures);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetActiveAttrib)(gl_es2_uint program, gl_es2_uint index, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_int *size, gl_es2_enum *type, gl_es2_char *name);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetActiveUniform)(gl_es2_uint program, gl_es2_uint index, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_int *size, gl_es2_enum *type, gl_es2_char *name);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetAttachedShaders)(gl_es2_uint program, gl_es2_sizei maxCount, gl_es2_sizei *count, gl_es2_uint *shaders);
GL_ES2_DECL_SPEC gl_es2_int GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetAttribLocation)(gl_es2_uint program, const gl_es2_char *name);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetBooleanv)(gl_es2_enum pname, gl_es2_boolean *data);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetBufferParameteriv)(gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params);
GL_ES2_DECL_SPEC gl_es2_enum GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetError)(void);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetFloatv)(gl_es2_enum pname, gl_es2_float *data);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetFramebufferAttachmentParameteriv)(gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum pname, gl_es2_int *params);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetIntegerv)(gl_es2_enum pname, gl_es2_int *data);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetProgramiv)(gl_es2_uint program, gl_es2_enum pname, gl_es2_int *params);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetProgramInfoLog)(gl_es2_uint program, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_char *infoLog);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetRenderbufferParameteriv)(gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetShaderiv)(gl_es2_uint shader, gl_es2_enum pname, gl_es2_int *params);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetShaderInfoLog)(gl_es2_uint shader, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_char *infoLog);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetShaderPrecisionFormat)(gl_es2_enum shadertype, gl_es2_enum precisiontype, gl_es2_int *range, gl_es2_int *precision);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetShaderSource)(gl_es2_uint shader, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_char *source);
GL_ES2_DECL_SPEC const gl_es2_ubyte *GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetString)(gl_es2_enum name);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetTexParameterfv)(gl_es2_enum target, gl_es2_enum pname, gl_es2_float *params);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetTexParameteriv)(gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetUniformfv)(gl_es2_uint program, gl_es2_int location, gl_es2_float *params);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetUniformiv)(gl_es2_uint program, gl_es2_int location, gl_es2_int *params);
GL_ES2_DECL_SPEC gl_es2_int GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetUniformLocation)(gl_es2_uint program, const gl_es2_char *name);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetVertexAttribfv)(gl_es2_uint index, gl_es2_enum pname, gl_es2_float *params);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetVertexAttribiv)(gl_es2_uint index, gl_es2_enum pname, gl_es2_int *params);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetVertexAttribPointerv)(gl_es2_uint index, gl_es2_enum pname, void **pointer);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Hint)(gl_es2_enum target, gl_es2_enum mode);
GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsBuffer)(gl_es2_uint buffer);
GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsEnabled)(gl_es2_enum cap);
GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsFramebuffer)(gl_es2_uint framebuffer);
GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsProgram)(gl_es2_uint program);
GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsRenderbuffer)(gl_es2_uint renderbuffer);
GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsShader)(gl_es2_uint shader);
GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsTexture)(gl_es2_uint texture);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(LineWidth)(gl_es2_float width);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(LinkProgram)(gl_es2_uint program);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(PixelStorei)(gl_es2_enum pname, gl_es2_int param);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(PolygonOffset)(gl_es2_float factor, gl_es2_float units);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ReadPixels)(gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height, gl_es2_enum format, gl_es2_enum type, void *pixels);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ReleaseShaderCompiler)(void);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(RenderbufferStorage)(gl_es2_enum target, gl_es2_enum internalformat, gl_es2_sizei width, gl_es2_sizei height);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(SampleCoverage)(gl_es2_float value, gl_es2_boolean invert);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Scissor)(gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ShaderBinary)(gl_es2_sizei count, const gl_es2_uint *shaders, gl_es2_enum binaryformat, const void *binary, gl_es2_sizei length);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ShaderSource)(gl_es2_uint shader, gl_es2_sizei count, const gl_es2_char *const *string, const gl_es2_int *length);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilFunc)(gl_es2_enum func, gl_es2_int ref, gl_es2_uint mask);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilFuncSeparate)(gl_es2_enum face, gl_es2_enum func, gl_es2_int ref, gl_es2_uint mask);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilMask)(gl_es2_uint mask);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilMaskSeparate)(gl_es2_enum face, gl_es2_uint mask);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilOp)(gl_es2_enum fail, gl_es2_enum zfail, gl_es2_enum zpass);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilOpSeparate)(gl_es2_enum face, gl_es2_enum sfail, gl_es2_enum dpfail, gl_es2_enum dppass);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_int internalformat, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border, gl_es2_enum format, gl_es2_enum type, const void *pixels);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexParameterf)(gl_es2_enum target, gl_es2_enum pname, gl_es2_float param);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexParameterfv)(gl_es2_enum target, gl_es2_enum pname, const gl_es2_float *params);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexParameteri)(gl_es2_enum target, gl_es2_enum pname, gl_es2_int param);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexParameteriv)(gl_es2_enum target, gl_es2_enum pname, const gl_es2_int *params);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexSubImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_int xoffset, gl_es2_int yoffset, gl_es2_sizei width, gl_es2_sizei height, gl_es2_enum format, gl_es2_enum type, const void *pixels);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform1f)(gl_es2_int location, gl_es2_float v0);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform1fv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform1i)(gl_es2_int location, gl_es2_int v0);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform1iv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform2f)(gl_es2_int location, gl_es2_float v0, gl_es2_float v1);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform2fv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform2i)(gl_es2_int location, gl_es2_int v0, gl_es2_int v1);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform2iv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform3f)(gl_es2_int location, gl_es2_float v0, gl_es2_float v1, gl_es2_float v2);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform3fv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform3i)(gl_es2_int location, gl_es2_int v0, gl_es2_int v1, gl_es2_int v2);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform3iv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform4f)(gl_es2_int location, gl_es2_float v0, gl_es2_float v1, gl_es2_float v2, gl_es2_float v3);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform4fv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform4i)(gl_es2_int location, gl_es2_int v0, gl_es2_int v1, gl_es2_int v2, gl_es2_int v3);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform4iv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(UniformMatrix2fv)(gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(UniformMatrix3fv)(gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(UniformMatrix4fv)(gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(UseProgram)(gl_es2_uint program);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ValidateProgram)(gl_es2_uint program);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib1f)(gl_es2_uint index, gl_es2_float x);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib1fv)(gl_es2_uint index, const gl_es2_float *v);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib2f)(gl_es2_uint index, gl_es2_float x, gl_es2_float y);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib2fv)(gl_es2_uint index, const gl_es2_float *v);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib3f)(gl_es2_uint index, gl_es2_float x, gl_es2_float y, gl_es2_float z);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib3fv)(gl_es2_uint index, const gl_es2_float *v);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib4f)(gl_es2_uint index, gl_es2_float x, gl_es2_float y, gl_es2_float z, gl_es2_float w);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib4fv)(gl_es2_uint index, const gl_es2_float *v);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttribPointer)(gl_es2_uint index, gl_es2_int size, gl_es2_enum type, gl_es2_boolean normalized, gl_es2_sizei stride, const void *pointer);
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Viewport)(gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height);


/* reallocates the framebuffer storage for the context if the specifications mismatch the current context.
 * num_rgba_bits: must be 32 bits.
 * num_stencil_bits: 0  for no stencil buffer,
 *                   16 for a 16-bit stencil buffer.
 * num_depth_bits: 0  for no depth buffer.
 *                 16 for a 16-bit depth buffer.
 *                 32 for a 32-bit depth buffer.
 * width: value from 0 to 4194303 (GL_ES2_IMPL_MAX_VIEWPORT_DIMS) for the width in pixels of the framebuffer.
 * height: value from 0 to 4194303 (GL_ES2_IMPL_MAX_VIEWPORT_DIMS) for the height in pixels of the framebuffer.
 * Return values:
 *   0: successful initialization (SL_ERR_OK in sl_defs.h)
 *  -1: invalid argument (SL_ERR_INVALID_ARG in sl_defs.h)
 *  -2: an overflow occurred (SL_ERR_OVERFLOW in sl_defs.h)
 *  -3: not enough memory (SL_ERR_NO_MEM in sl_defs.h)
 *  -4: an internal error occurred (SL_ERR_INTERNAL in sl_defs.h)
 * Currently -5 (SL_ERR_HAD_ERRORS) cannot occur as this is returned for compilation errors. */
GL_ES2_DECL_SPEC int GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(context_set_framebuffer_storage)(int num_rgba_bits,
                                                                                                  int num_stencil_bits,
                                                                                                  int num_depth_bits,
                                                                                                  int width, int height);

/* initialize the context, this initializes the Aex context and default framebuffer to the indicated specifications.
 * num_rgba_bits: must be 32 bits.
 * num_stencil_bits: 0  for no stencil buffer,
 *                   16 for a 16-bit stencil buffer.
 * num_depth_bits: 0  for no depth buffer.
 *                 16 for a 16-bit depth buffer.
 *                 32 for a 32-bit depth buffer.
 * width: value from 0 to 4194303 (GL_ES2_IMPL_MAX_VIEWPORT_DIMS) for the width in pixels of the framebuffer.
 * height: value from 0 to 4194303 (GL_ES2_IMPL_MAX_VIEWPORT_DIMS) for the height in pixels of the framebuffer.
 * Return values:
 *   0: successful initialization (SL_ERR_OK in sl_defs.h)
 *  -1: invalid argument (SL_ERR_INVALID_ARG in sl_defs.h)
 *  -2: an overflow occurred (SL_ERR_OVERFLOW in sl_defs.h)
 *  -3: not enough memory (SL_ERR_NO_MEM in sl_defs.h)
 *  -4: an internal error occurred (SL_ERR_INTERNAL in sl_defs.h)
 * Currently -5 (SL_ERR_HAD_ERRORS) cannot occur as this is returned for compilation errors. */
GL_ES2_DECL_SPEC int GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(context_init)(int num_rgba_bits,
                                                                               int num_stencil_bits,
                                                                               int num_depth_bits,
                                                                               int width, int height);

/* runs the debug shader inside the specified program. This requires that the shader was created
 * using glCreateShader(AEX_GL_DEBUG_SHADER) and is the only shader attached to the program.
 */
GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(run_debug_shader)(int program);

/* retrieves the debug dump string as logged by the "dump()" builtin function (available only on
 * glCreateShader(AEX_GL_DEBUG_SHADER) type shaders). If no dump() call was made, or an error
 * occurs, an empty string "" is returned. Otherwise a multi-line string is returned whereby,
 * for each line, a single dump() call is specified.
 * For instance, "dump(vec2(1,2))" will result in the dump string "vec2(1.000000, 2.000000)\n"
 * (so, important: including a newline at the end!)
 * similarly, "dump(ivec4(1,2,3,4))" will result in the dump string "ivec4(1, 2, 3, 4)\n"
 * and so on.
 */
GL_ES2_DECL_SPEC const char *GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(get_shader_debug_string)(int shader);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GL_ES2_IMPL_H */
