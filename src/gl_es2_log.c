/* Copyright 2023-2024 Kinglet B.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License") { }
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

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef INTTYPES_H_INCLUDED
#define INTTYPES_H_INCLUDED
#include <inttypes.h>
#endif

#ifndef GL_ES2_LOG_H_INCLUDED
#define GL_ES2_LOG_H_INCLUDED
#include "gl_es2_log.h"
#endif

static void apilog(struct gl_es2_context *c, const char *fmt, ...) {
  va_list args;
  if (!c->log_file_) return;
  va_start(args, fmt);
  vfprintf(c->log_file_, fmt, args);
  va_end(args);
}

void gl_es2_log_ActiveTexture(struct gl_es2_context *c, gl_es2_enum texture) {
  if ((texture >= GL_ES2_TEXTURE0) && (texture < GL_ES2_MAX_COMBINED_TEXTURE_IMAGE_UNITS)) {
    apilog(c, "glActiveTexture(GL_ES2_TEXTURE%d);\n", (int)(texture - GL_ES2_TEXTURE0));
  }
  else {
    apilog(c, "glActiveTexture(0x%04X);\n", texture);
  }
}

void gl_es2_log_AttachShader(struct gl_es2_context *c, gl_es2_uint program, gl_es2_uint shader) {
  apilog(c, "glAttachShader(%d, %d);\n", program, shader);
}

void gl_es2_log_BindAttribLocation(struct gl_es2_context *c, gl_es2_uint program, gl_es2_uint index, const gl_es2_char *name) { 
  apilog(c, "glBindAttribLocation(%d, %d, %s);\n", program, index, name ? name : "NULL");
}

static const char *buffer_target(gl_es2_enum target) {
  switch (target) {
    case GL_ES2_ARRAY_BUFFER: return "GL_ARRAY_BUFFER";
    case GL_ES2_ELEMENT_ARRAY_BUFFER: return "GL_ELEMENT_ARRAY_BUFFER";
    default: return NULL;
  }
}

void gl_es2_log_BindBuffer(struct gl_es2_context *c, gl_es2_enum target, gl_es2_uint buffer) { 
  const char *mtgt = buffer_target(target);
  if (mtgt) {
    apilog(c, "glBindBuffer(%s, %d);\n", mtgt, buffer);
  }
  else {
    apilog(c, "glBindBuffer(0x%04X, %d);\n", target, buffer);
  }
}

void gl_es2_log_BindFramebuffer(struct gl_es2_context *c, gl_es2_enum target, gl_es2_uint framebuffer) { 
  apilog(c, "glBindFramebuffer(%d, %d);\n", target, framebuffer);
}

void gl_es2_log_BindRenderbuffer(struct gl_es2_context *c, gl_es2_enum target, gl_es2_uint renderbuffer) { 
  apilog(c, "glBindRenderbuffer(%d, %d);\n", target, renderbuffer);
}

void gl_es2_log_BindTexture(struct gl_es2_context *c, gl_es2_enum target, gl_es2_uint texture) { 
  apilog(c, "glBindTexture(%d, %d);\n", target, texture);
}

void gl_es2_log_BlendColor(struct gl_es2_context *c, gl_es2_float red, gl_es2_float green, gl_es2_float blue, gl_es2_float alpha) { 
  apilog(c, "glBlendColor(%f, %f, %f, %f);\n", red, green, blue, alpha);
}

static const char *blend_eq_mode(gl_es2_enum mode) {
  switch (mode) {
    case GL_ES2_FUNC_ADD: return "GL_FUNC_ADD";
    case GL_ES2_FUNC_SUBTRACT: return "GL_FUNC_SUBTRACT";
    case GL_ES2_FUNC_REVERSE_SUBTRACT: return "GL_FUNC_REVERSE_SUBTRACT";
    default: return NULL;
  }
}

void gl_es2_log_BlendEquation(struct gl_es2_context *c, gl_es2_enum mode) { 
  const char *m = blend_eq_mode(mode);
  if (m) {
    apilog(c, "glBlendEquation(%s);\n", m);
  }
  else {
    apilog(c, "glBlendEquation(0x%04X);\n", mode);
  }
}

void gl_es2_log_BlendEquationSeparate(struct gl_es2_context *c, gl_es2_enum modeRGB, gl_es2_enum modeAlpha) { 
  const char *mrgb = blend_eq_mode(modeRGB);
  const char *malpha = blend_eq_mode(modeAlpha);
  if (mrgb) {
    apilog(c, "glBlendEquationSeparate(%s, ", mrgb);
  }
  else {
    apilog(c, "glBlendEquationSeperate(0x%04X, ", modeRGB);
  }
  if (malpha) {
    apilog(c, "%s);\n", malpha);
  }
  else {
    apilog(c, "0x%04X);\n", malpha);
  }
}

static const char *blend_func(gl_es2_enum fac) {
  switch (fac) {
    case GL_ES2_ZERO: return "GL_ZERO";
    case GL_ES2_ONE: return "GL_ONE";
    case GL_ES2_SRC_COLOR: return "GL_SRC_COLOR";
    case GL_ES2_ONE_MINUS_SRC_COLOR: return "GL_ONE_MINUS_SRC_COLOR";
    case GL_ES2_DST_COLOR: return "GL_DST_COLOR";
    case GL_ES2_ONE_MINUS_DST_COLOR: return "GL_ONE_MINUS_DST_COLOR";
    case GL_ES2_SRC_ALPHA: return "GL_SRC_ALPHA";
    case GL_ES2_ONE_MINUS_SRC_ALPHA: return "GL_ONE_MINUS_SRC_ALPHA";
    case GL_ES2_DST_ALPHA: return "GL_DST_ALPHA";
    case GL_ES2_ONE_MINUS_DST_ALPHA: return "GL_ONE_MINUS_DST_ALPHA";
    case GL_ES2_CONSTANT_COLOR: return "GL_CONSTANT_COLOR";
    case GL_ES2_ONE_MINUS_CONSTANT_COLOR: return "GL_ONE_MINUS_CONSTANT_COLOR";
    case GL_ES2_CONSTANT_ALPHA: return "GL_CONSTANT_ALPHA";
    case GL_ES2_ONE_MINUS_CONSTANT_ALPHA: return "GL_ONE_MINUS_CONSTANT_ALPHA";
    case GL_ES2_SRC_ALPHA_SATURATE: return "GL_SRC_ALPHA_SATURATE";
    default: return NULL;
  }
}

void gl_es2_log_BlendFunc(struct gl_es2_context *c, gl_es2_enum sfactor, gl_es2_enum dfactor) { 
  const char *msfactor = blend_func(sfactor), *mdfactor = blend_func(dfactor);
  if (msfactor) {
    apilog(c, "glBlendFunc(%s, ", msfactor);
  }
  else {
    apilog(c, "glBlendFunc(0x%04X, ", sfactor);
  }
  if (mdfactor) {
    apilog(c, "%s);\n", mdfactor);
  }
  else {
    apilog(c, "0x%04X);\n", dfactor);
  }
}

void gl_es2_log_BlendFuncSeparate(struct gl_es2_context *c, gl_es2_enum sfactorRGB, gl_es2_enum dfactorRGB, gl_es2_enum sfactorAlpha, gl_es2_enum dfactorAlpha) { 
  const char *msfactorrgb = blend_func(sfactorRGB), *mdfactorrgb = blend_func(dfactorRGB);
  const char *msfactoralpha = blend_func(sfactorAlpha), *mdfactoralpha = blend_func(dfactorAlpha);
  if (msfactorrgb) {
    apilog(c, "glBlendFunc(%s, ", msfactorrgb);
  }
  else {
    apilog(c, "glBlendFunc(0x%04X, ", sfactorRGB);
  }
  if (mdfactorrgb) {
    apilog(c, "%s, ", mdfactorrgb);
  }
  else {
    apilog(c, "0x%04X, ", dfactorRGB);
  }
  if (msfactoralpha) {
    apilog(c, "%s, ", msfactoralpha);
  }
  else {
    apilog(c, "0x%04X, ", sfactorAlpha);
  }
  if (mdfactoralpha) {
    apilog(c, "%s);\n", mdfactoralpha);
  }
  else {
    apilog(c, "0x%04X);\n", dfactorAlpha);
  }
}

static const char *usagem(gl_es2_enum usage) {
  switch (usage) {
    case GL_ES2_STREAM_DRAW: return "GL_STREAM_DRAW";
    case GL_ES2_STATIC_DRAW: return "GL_STATIC_DRAW";
    case GL_ES2_DYNAMIC_DRAW: return "GL_DYNAMIC_DRAW";
    default: return NULL;
  }
}

void gl_es2_log_BufferData(struct gl_es2_context *c, gl_es2_enum target, gl_es2_sizeiptr size, const void *data, gl_es2_enum usage) { 
  const char *mtarget = buffer_target(target);
  const char *musage = usagem(usage);
  if (mtarget) {
    apilog(c, "glBufferData(%s, %zu, ", mtarget, size);
  }
  else {
    apilog(c, "glBufferData(0x%04X, %zu, ", target, size);
  }
  if (data) {
    apilog(c, "0x%p, ", data);
  }
  else {
    apilog(c, "NULL, ");
  }
  if (musage) {
    apilog(c, "%s);\n", musage);
  }
  else {
    apilog(c, "0x%04X);\n", usage);
  }
}

void gl_es2_log_BufferSubData(struct gl_es2_context *c, gl_es2_enum target, gl_es2_intptr offset, gl_es2_sizeiptr size, const void *data) { 
  const char *mtarget = buffer_target(target);
  if (mtarget) {
    apilog(c, "glBufferSubData(%s, 0x%" PRIx64 ", %zu, ", mtarget, (uint64_t)offset, size);
  }
  else {
    apilog(c, "glBufferSubData(0x%04X, 0x%" PRIx64 ", %zu, ", target, (uint64_t)offset, size);
  }
  if (data) {
    apilog(c, "0x%p);\n", data);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

static const char *framebuffer_status(gl_es2_enum framebuffer_status) {
  switch (framebuffer_status) {
    case GL_ES2_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
    case GL_ES2_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
      return "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS";
    case GL_ES2_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
    case GL_ES2_FRAMEBUFFER_UNSUPPORTED:
      return "GL_FRAMEBUFFER_UNSUPPORTED";
    default:
      return NULL;
  }
}

void gl_es2_log_CheckFramebufferStatus(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum framebuffer_status_assessed) {
  const char *mstatus = framebuffer_status(framebuffer_status_assessed);
  if (target == GL_ES2_FRAMEBUFFER) {
    apilog(c, "glCheckFramebufferStatus(GL_FRAMEBUFFER) = ");
  }
  else {
    apilog(c, "glCheckFramebufferStatus(0x%04X) = ", target);
  }
  if (mstatus) {
    apilog(c, "%s;\n", mstatus);
  }
  else {
    apilog(c, "0x%04X;\n", framebuffer_status_assessed);
  }
}

void gl_es2_log_Clear(struct gl_es2_context *c, gl_es2_bitfield mask) {
  int have_any_flag = 0;
  apilog(c, "glClear(");
  if ((mask & GL_ES2_COLOR_BUFFER_BIT) == GL_ES2_COLOR_BUFFER_BIT) {
    apilog(c, "%sGL_COLOR_BUFFER_BIT", have_any_flag ? " | " : "");
    mask &= ~GL_ES2_COLOR_BUFFER_BIT;
    have_any_flag = 1;
  }
  if ((mask & GL_ES2_DEPTH_BUFFER_BIT) == GL_ES2_DEPTH_BUFFER_BIT) {
    apilog(c, "%sGL_DEPTH_BUFFER_BIT", have_any_flag ? " | " : "");
    mask &= ~GL_ES2_DEPTH_BUFFER_BIT;
    have_any_flag = 1;
  }
  if ((mask & GL_ES2_STENCIL_BUFFER_BIT) == GL_ES2_STENCIL_BUFFER_BIT) {
    apilog(c, "%sGL_STENCIL_BUFFER_BIT", have_any_flag ? " | " : "");
    mask &= ~GL_ES2_STENCIL_BUFFER_BIT;
    have_any_flag = 1;
  }
  if (mask) {
    apilog(c, "%s0x%04X", have_any_flag ? " | " : "", mask);
    have_any_flag = 1;
  }
  if (!have_any_flag) {
    apilog(c, "0");
  }
  apilog(c, ");\n");
}

void gl_es2_log_ClearColor(struct gl_es2_context *c, gl_es2_float red, gl_es2_float green, gl_es2_float blue, gl_es2_float alpha) { 
  apilog(c, "glClearColor(%f, %f, %f, %f);\n", red, green, blue, alpha);
}

void gl_es2_log_ClearDepthf(struct gl_es2_context *c, gl_es2_float d) {
  apilog(c, "glClearDepthf(%f);\n", d);
}

void gl_es2_log_ClearStencil(struct gl_es2_context *c, gl_es2_int s) { 
  apilog(c, "glClearStencil(0x%04X);\n", s);
}

void gl_es2_log_ColorMask(struct gl_es2_context *c, gl_es2_boolean red, gl_es2_boolean green, gl_es2_boolean blue, gl_es2_boolean alpha) { 
  apilog(c, "glColorMask(");
  if (red == GL_ES2_TRUE) apilog(c, "GL_TRUE, ");
  else if (red == GL_ES2_FALSE) apilog(c, "GL_FALSE, ");
  else apilog(c, "0x%04X, ", red);

  if (green == GL_ES2_TRUE) apilog(c, "GL_TRUE, ");
  else if (green == GL_ES2_FALSE) apilog(c, "GL_FALSE, ");
  else apilog(c, "0x%04X, ", green);

  if (blue == GL_ES2_TRUE) apilog(c, "GL_TRUE, ");
  else if (blue == GL_ES2_FALSE) apilog(c, "GL_FALSE, ");
  else apilog(c, "0x%04X, ", blue);

  if (alpha == GL_ES2_TRUE) apilog(c, "GL_TRUE);");
  else if (alpha == GL_ES2_FALSE) apilog(c, "GL_FALSE);");
  else apilog(c, "0x%04X);", alpha);
}

void gl_es2_log_CompileShader(struct gl_es2_context *c, gl_es2_uint shader) { 
  apilog(c, "glCompileShader(%u);\n", shader);
}

static const char *tex_target(gl_es2_enum tgt) {
  switch (tgt) {
    case GL_ES2_TEXTURE_2D: return "GL_TEXTURE_2D";
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_X: return "GL_TEXTURE_CUBE_MAP_POSITIVE_X";
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_X: return "GL_TEXTURE_CUBE_MAP_NEGATIVE_X";
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Y: return "GL_TEXTURE_CUBE_MAP_POSITIVE_Y";
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Y: return "GL_TEXTURE_CUBE_MAP_NEGATIVE_Y";
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Z: return "GL_TEXTURE_CUBE_MAP_POSITIVE_Z";
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Z: return "GL_TEXTURE_CUBE_MAP_NEGATIVE_Z";
    default:
      return NULL;
  }
}

static const char *tex_format(gl_es2_enum format) {
  switch (format) {
    case GL_ES2_ALPHA: return "GL_ALPHA";
    case GL_ES2_LUMINANCE: return "GL_LUMINANCE";
    case GL_ES2_LUMINANCE_ALPHA: return "GL_LUMINANCE_ALPHA";
    case GL_ES2_RGB: return "GL_RGB";
    case GL_ES2_RGB8: return "GL_RGB8";
    case GL_ES2_RGBA: return "GL_RGBA";
    case GL_ES2_RGBA8: return "GL_RGBA8";
    default: return NULL;
  }
}

static const char *tex_type(gl_es2_enum type) {
  switch (type) {
    case GL_ES2_UNSIGNED_BYTE: return "GL_UNSIGNED_BYTE";
    case GL_ES2_UNSIGNED_SHORT_5_6_5: return "GL_UNSIGNED_SHORT_5_6_5";
    case GL_ES2_UNSIGNED_SHORT_4_4_4_4: return "GL_UNSIGNED_SHORT_4_4_4_4";
    case GL_ES2_UNSIGNED_SHORT_5_5_5_1: return "GL_UNSIGNED_SHORT_5_5_5_1";
    default:
      return NULL;
  }
}

void gl_es2_log_CompressedTexImage2D(struct gl_es2_context *c, gl_es2_enum target, gl_es2_int level, gl_es2_enum internalformat, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border, gl_es2_sizei imageSize, const void *data) {
  const char *mtgt = tex_target(target);
  const char *mifmt = tex_format(internalformat);
  if (mtgt) {
    apilog(c, "glCompressedTexImage2D(%s, ", mtgt);
  }
  else {
    apilog(c, "glCompressedTexImage2D(0x%04X, ", target);
  }
  apilog(c, "%d, ", level);
  if (mifmt) {
    apilog(c, "%s, ", mifmt);
  }
  else {
    apilog(c, "0x%04X, ", internalformat);
  }
  apilog(c, "%d, %d, %d, %d, %p);\n", width, height, border, imageSize, data);
}

void gl_es2_log_CompressedTexSubImage2D(struct gl_es2_context *c, gl_es2_enum target, gl_es2_int level, gl_es2_int xoffset, gl_es2_int yoffset, gl_es2_sizei width, gl_es2_sizei height, gl_es2_enum format, gl_es2_sizei imageSize, const void *data) { 
  const char *mtgt = tex_target(target);
  const char *mfmt = tex_format(format);
  if (mtgt) {
    apilog(c, "glCompressedTexImage2D(%s, ", mtgt);
  }
  else {
    apilog(c, "glCompressedTexImage2D(0x%04X, ", target);
  }
  apilog(c, "%d, %d, %d, %d, %d, ", level, xoffset, yoffset, width, height);
  if (mfmt) {
    apilog(c, "%s, ", mfmt);
  }
  else {
    apilog(c, "0x%04X, ", format);
  }
  apilog(c, "%d, %p);\n", imageSize, data);
}

void gl_es2_log_CopyTexImage2D(struct gl_es2_context *c, gl_es2_enum target, gl_es2_int level, gl_es2_enum internalformat, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border) { 
  const char *mtgt = tex_target(target);
  const char *mifmt = tex_format(internalformat);
  if (mtgt) {
    apilog(c, "glCopyTexImage2D(%s, ", mtgt);
  }
  else {
    apilog(c, "glCopyTexImage2D(0x%04X, ", target);
  }
  apilog(c, "%d, ", level);
  if (mifmt) {
    apilog(c, "%s, ", mifmt);
  }
  else {
    apilog(c, "0x%04X, ", internalformat);
  }
  apilog(c, "%d, %d, %d, %d, %d);\n", x, y, width, height, border);
}

void gl_es2_log_CopyTexSubImage2D(struct gl_es2_context *c, gl_es2_enum target, gl_es2_int level, gl_es2_int xoffset, gl_es2_int yoffset, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height) { 
  const char *mtgt = tex_target(target);
  if (mtgt) {
    apilog(c, "glCopyTexSubImage2D(%s, ", mtgt);  
  }
  else {
    apilog(c, "glCopyTexSubImage2D(0x%04X, ", target);
  }
  apilog(c, "%d, %d, %d, %d, %d, %d, %d);\n", level, xoffset, yoffset, x, y, width, height);
}

void gl_es2_log_CreateProgram(struct gl_es2_context *c, gl_es2_uint program_created) {
  apilog(c, "glCreateProgram() = %d\n", program_created);
}

void gl_es2_log_CreateShader(struct gl_es2_context *c, gl_es2_enum type, gl_es2_uint shader_created) {
  apilog(c, "glCreateShader() = %d\n", shader_created);
}

static const char *cull_face_mode(gl_es2_enum mode) {
  switch (mode) {
  case GL_ES2_FRONT: return "GL_FRONT";
  case GL_ES2_BACK: return "GL_BACK";
  case GL_ES2_FRONT_AND_BACK: return "GL_FRONT_AND_BACK";
  default: return NULL;
  }
}

void gl_es2_log_CullFace(struct gl_es2_context *c, gl_es2_enum mode) { 
  const char *mmode = cull_face_mode(mode);
  if (mmode) {
    apilog(c, "glCullFace(%s);\n", mmode);
  }
  else {
    apilog(c, "glCullFace(0x%04X);\n", mode);
  }
}

void gl_es2_log_DeleteBuffers(struct gl_es2_context *c, gl_es2_sizei n, const gl_es2_uint *buffers) {
  size_t k;
  apilog(c, "glDeleteBuffers(%d, {", n);
  for (k = 0; k < n; ++k) {
    if (k) apilog(c, ", ");
    apilog(c, "%u", buffers[k]);
  }
  apilog(c, "});\n");
}

void gl_es2_log_DeleteFramebuffers(struct gl_es2_context *c, gl_es2_sizei n, const gl_es2_uint *framebuffers) { 
  size_t k;
  apilog(c, "glDeleteFramebuffers(%d, {", n);
  for (k = 0; k < n; ++k) {
    if (k) apilog(c, ", ");
    apilog(c, "%u", framebuffers[k]);
  }
  apilog(c, "});\n");
}

void gl_es2_log_DeleteProgram(struct gl_es2_context *c, gl_es2_uint program) { 
  apilog(c, "glDeleteProgram(%u);\n", program);
}

void gl_es2_log_DeleteRenderbuffers(struct gl_es2_context *c, gl_es2_sizei n, const gl_es2_uint *renderbuffers) { 
  size_t k;
  apilog(c, "glDeleteRenderbuffers(%d, {", n);
  for (k = 0; k < n; ++k) {
    if (k) apilog(c, ", ");
    apilog(c, "%u", renderbuffers[k]);
  }
  apilog(c, "});\n");
}

void gl_es2_log_DeleteShader(struct gl_es2_context *c, gl_es2_uint shader) { 
  apilog(c, "glDeleteShader(%u);\n", shader);
}

void gl_es2_log_DeleteTextures(struct gl_es2_context *c, gl_es2_sizei n, const gl_es2_uint *textures) { 
  size_t k;
  apilog(c, "glDeleteTextures(%d, {", n);
  for (k = 0; k < n; ++k) {
    if (k) apilog(c, ", ");
    apilog(c, "%u", textures[k]);
  }
  apilog(c, "});\n");
}

static const char *depth_func(gl_es2_enum func) {
  switch (func) {
    case GL_ES2_NEVER: return "GL_NEVER";
    case GL_ES2_LESS: return "GL_LESS";
    case GL_ES2_EQUAL: return "GL_EQUAL";
    case GL_ES2_LEQUAL: return "GL_LEQUAL";
    case GL_ES2_GREATER: return "GL_GREATER";
    case GL_ES2_NOTEQUAL: return "GL_NOTEQUAL";
    case GL_ES2_GEQUAL: return "GL_GEQUAL";
    case GL_ES2_ALWAYS: return "GL_ALWAYS";
    default:
      return NULL;
  }
}

void gl_es2_log_DepthFunc(struct gl_es2_context *c, gl_es2_enum func) { 
  const char *mdf = depth_func(func);
  if (mdf) {
    apilog(c, "glDepthFunc(%s);\n", mdf);
  }
  else {
    apilog(c, "glDepthFunc(0x%04X);\n", func);
  }
}

void gl_es2_log_DepthMask(struct gl_es2_context *c, gl_es2_boolean flag) { 
  if (flag == GL_ES2_TRUE) {
    apilog(c, "glDepthMask(GL_TRUE);\n");
  }
  else if (flag == GL_ES2_FALSE) {
    apilog(c, "glDepthMask(GL_FALSE);\n");
  }
  else {
    apilog(c, "glDepthMask(0x%04X);\n", flag);
  }
}

void gl_es2_log_DepthRangef(struct gl_es2_context *c, gl_es2_float n, gl_es2_float f) { 
  apilog(c, "glDepthRangef(%f, %f);\n", n, f);
}

void gl_es2_log_DetachShader(struct gl_es2_context *c, gl_es2_uint program, gl_es2_uint shader) { 
  apilog(c, "glDetachShader(%u, %u);\n", program, shader);
}

static const char *mcap(gl_es2_enum cap) {
  switch (cap) {
    case GL_ES2_BLEND: return "GL_BLEND";
    case GL_ES2_CULL_FACE: return "GL_CULL_FACE";
    case GL_ES2_DEPTH_TEST: return "GL_DEPTH_TEST";
    case GL_ES2_DITHER: return "GL_DITHER";
    case GL_ES2_POLYGON_OFFSET_FILL: return "GL_POLYGON_OFFSET_FILL";
    case GL_ES2_SAMPLE_ALPHA_TO_COVERAGE: return "GL_SAMPLE_ALPHA_TO_COVERAGE";
    case GL_ES2_SAMPLE_COVERAGE: return "GL_SAMPLE_COVERAGE";
    case GL_ES2_SCISSOR_TEST: return "GL_SCISSOR_TEST";
    case GL_ES2_STENCIL_TEST: return "GL_STENCIL_TEST";
    default:
      return NULL;
  }
}

void gl_es2_log_Disable(struct gl_es2_context *c, gl_es2_enum cap) {
  const char *mc = mcap(cap);
  if (mc) {
    apilog(c, "glDisable(%s);\n", mc);
  }
  else {
    apilog(c, "glDisable(0x%04X);\n", cap);
  }
}

void gl_es2_log_DisableVertexAttribArray(struct gl_es2_context *c, gl_es2_uint index) { 
  apilog(c, "glDisableVertexAttribArray(%u);\n", index);
}

static const char *draw_mode(gl_es2_enum mode) {
  switch (mode) {
    case GL_ES2_POINTS: return "GL_POINTS";
    case GL_ES2_LINE_STRIP: return "GL_LINE_STRIP";
    case GL_ES2_LINE_LOOP: return "GL_LINE_LOOP";
    case GL_ES2_LINES: return "GL_LINES";
    case GL_ES2_TRIANGLE_STRIP: return "GL_TRIANGLE_STRIP";
    case GL_ES2_TRIANGLE_FAN: return "GL_TRIANGLE_FAN";
    case GL_ES2_TRIANGLES: return "GL_TRIANGLES";
    default: return NULL;
  }
}

void gl_es2_log_DrawArrays(struct gl_es2_context *c, gl_es2_enum mode, gl_es2_int first, gl_es2_sizei count) { 
  const char *mmode = draw_mode(mode);

  if (mmode) {
    apilog(c, "glDrawArrays(%s, %d, %d); // # %d\n", mmode, first, count, c->debug_frame_op_);
  }
  else {
    apilog(c, "glDrawArrays(0x%04X, %d, %d); // # %d\n", mode, first, count, c->debug_frame_op_);
  }
}

static const char *index_type(gl_es2_enum type) {
  switch (type) {
    case GL_ES2_UNSIGNED_BYTE: return "GL_UNSIGNED_BYTE";
    case GL_ES2_UNSIGNED_SHORT: return "GL_UNSIGNED_SHORT";
    case GL_ES2_UNSIGNED_INT: return "GL_UNSIGNED_INT";
    default:
      return NULL;
  }
}

void gl_es2_log_DrawElements(struct gl_es2_context *c, gl_es2_enum mode, gl_es2_sizei count, gl_es2_enum type, const void *indices) { 
  const char *mmode = draw_mode(mode);
  const char *mtype = index_type(type);
  if (mmode) {
    apilog(c, "glDrawElements(%s, %d, ", mmode, count);
  }
  else {
    apilog(c, "glDrawElements(0x%04X, %d, ", mode, count);
  }
  if (mtype) {
    apilog(c, "%s, ", mtype);
  }
  else {
    apilog(c, "0x%04X, ", type);
  }
  if (indices) {
    apilog(c, "0x%p); // # %d", indices, c->debug_frame_op_);
  }
  else {
    apilog(c, "NULL); // # %d", c->debug_frame_op_);
  }
  apilog(c, ", prog#%d", c->current_program_ ? c->current_program_->no_.name_: -1);
  if (c->current_program_) {
    apilog(c, ", vs#%d, fs#%d\n", 
      c->current_program_->vertex_shader_.shader_ ? c->current_program_->vertex_shader_.shader_->no_.name_ : -1, 
      c->current_program_->fragment_shader_.shader_ ? c->current_program_->fragment_shader_.shader_->no_.name_ : -1);
  }
  else {
    apilog(c, "\n");
  }
}

void gl_es2_log_Enable(struct gl_es2_context *c, gl_es2_enum cap) { 
  const char *mc = mcap(cap);
  if (mc) {
    apilog(c, "glEnable(%s);\n", mc);
  }
  else {
    apilog(c, "glEnable(0x%04X);\n", cap);
  }
}

void gl_es2_log_EnableVertexAttribArray(struct gl_es2_context *c, gl_es2_uint index) { 
  apilog(c, "glEnableVertexAttribArray(%u);\n", index);
}

void gl_es2_log_Finish(struct gl_es2_context *c) { 
  apilog(c, "glFinish();\n");
}

void gl_es2_log_Flush(struct gl_es2_context *c) { 
  apilog(c, "glFlush();\n");
}

static const char *fb_attachment(gl_es2_enum attachment) {
  switch (attachment) {
    case GL_ES2_COLOR_ATTACHMENT0:
      return "GL_COLOR_ATTACHMENT";
    case GL_ES2_DEPTH_ATTACHMENT:
      return "GL_DEPTH_ATTACHMENT";
    case GL_ES2_STENCIL_ATTACHMENT:
      return "GL_STENCIL_ATTACHMENT";
    default:
      return NULL;
  }
}

void gl_es2_log_FramebufferRenderbuffer(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum renderbuffertarget, gl_es2_uint renderbuffer) {
  const char *mfba = fb_attachment(attachment);
  if (target == GL_ES2_FRAMEBUFFER) {
    apilog(c, "glFramebufferRenderbuffer(GL_FRAMEBUFFER, ");
  }
  else {
    apilog(c, "glFramebufferRenderbuffer(0x%04X, ", target);
  }
  if (mfba) {
    apilog(c, "%s, ", mfba);
  }
  else {
    apilog(c, "0x%04X, ", attachment);
  }
  if (renderbuffertarget == GL_ES2_RENDERBUFFER) {
    apilog(c, "GL_RENDERBUFFER, ");
  }
  else {
    apilog(c, "0x%04X, ", renderbuffertarget);
  }
  apilog(c, "%u);\n", renderbuffer);
}

void gl_es2_log_FramebufferTexture2D(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum textarget, gl_es2_uint texture, gl_es2_int level) { 
  const char *mfba = fb_attachment(attachment);
  const char *mtext = tex_target(textarget);
  if (target == GL_ES2_FRAMEBUFFER) {
    apilog(c, "glFramebufferRenderbuffer(GL_FRAMEBUFFER, ");
  }
  else {
    apilog(c, "glFramebufferRenderbuffer(0x%04X, ", target);
  }
  if (mfba) {
    apilog(c, "%s, ", mfba);
  }
  else {
    apilog(c, "0x%04X, ", attachment);
  }
  if (mtext) {
    apilog(c, "%s, ", mtext);
  }
  else {
    apilog(c, "0x%04X, ", textarget);
  }
  apilog(c, "%u, %d);\n", texture, level);
}

const char *face_mode(gl_es2_enum mode) {
  switch (mode) {
    case GL_ES2_CW: return "GL_CW";
    case GL_ES2_CCW: return "GL_CCW";
    default:
      return NULL;
  }
}

void gl_es2_log_FrontFace(struct gl_es2_context *c, gl_es2_enum mode) {
  const char *mmode = face_mode(mode);
  if (mmode) {
    apilog(c, "glFrontFace(%s);\n", mmode);
  }
  else {
    apilog(c, "glFrontFace(0x%04X);\n", mode);
  }
}

void gl_es2_log_GenBuffers(struct gl_es2_context *c, gl_es2_sizei n, gl_es2_uint *buffers) { 
  size_t k;
  apilog(c, "glGenBuffers(%d, { ", n);
  for (k = 0; k < n; ++k) {
    apilog(c, "%s%u", k ? ", " : "", buffers[k]);
  }
  apilog(c, " });\n");
}

void gl_es2_log_GenerateMipmap(struct gl_es2_context *c, gl_es2_enum target) { 
  const char *mtgt = tex_target(target);
  if (mtgt) {
    apilog(c, "glGenerateMipmap(%s);\n", mtgt);
  }
  else {
    apilog(c, "glGenerateMipmap(0x%04X);\n", target);
  }
}

void gl_es2_log_GenFramebuffers(struct gl_es2_context *c, gl_es2_sizei n, gl_es2_uint *framebuffers) { 
  size_t k;
  apilog(c, "glGenFramebuffers(%d, { ", n);
  for (k = 0; k < n; ++k) {
    apilog(c, "%s%u", k ? ", " : "", framebuffers[k]);
  }
  apilog(c, " });\n");
}

void gl_es2_log_GenRenderbuffers(struct gl_es2_context *c, gl_es2_sizei n, gl_es2_uint *renderbuffers) { 
  size_t k;
  apilog(c, "glGenRenderbuffers(%d, { ", n);
  for (k = 0; k < n; ++k) {
    apilog(c, "%s%u", k ? ", " : "", renderbuffers[k]);
  }
  apilog(c, " });\n");
}

void gl_es2_log_GenTextures(struct gl_es2_context *c, gl_es2_sizei n, gl_es2_uint *textures) { 
  size_t k;
  apilog(c, "glGenTextures(%d, { ", n);
  for (k = 0; k < n; ++k) {
    apilog(c, "%s%u", k ? ", " : "", textures[k]);
    if (textures[k] == 53) {
      fprintf(stderr, "Area of interest\n");
    }
  }
  apilog(c, " });\n");
}

static const char *attrib_type(gl_es2_enum type) {
  switch (type) {
    case GL_ES2_FLOAT: return "GL_FLOAT";
    case GL_ES2_INT: return "GL_INT";
    case GL_ES2_BOOL: return "GL_BOOL";
    case GL_ES2_FLOAT_VEC2: return "GL_FLOAT_VEC2";
    case GL_ES2_FLOAT_VEC3: return "GL_FLOAT_VEC3";
    case GL_ES2_FLOAT_VEC4: return "GL_FLOAT_VEC4";
    case GL_ES2_BOOL_VEC2: return "GL_BOOL_VEC2";
    case GL_ES2_BOOL_VEC3: return "GL_BOOL_VEC3";
    case GL_ES2_BOOL_VEC4: return "GL_BOOL_VEC4";
    case GL_ES2_INT_VEC2: return "GL_INT_VEC2";
    case GL_ES2_INT_VEC3: return "GL_INT_VEC3";
    case GL_ES2_INT_VEC4: return "GL_INT_VEC4";
    case GL_ES2_FLOAT_MAT2: return "GL_FLOAT_MAT2";
    case GL_ES2_FLOAT_MAT3: return "GL_FLOAT_MAT3";
    case GL_ES2_FLOAT_MAT4: return "GL_FLOAT_MAT4";
    case GL_ES2_SAMPLER_2D: return "GL_SAMPLER_2D";
    case GL_ES2_SAMPLER_CUBE: return "GL_SAMPLER_CUBE";
    default: return NULL;
  }
}

void gl_es2_log_GetActiveAttrib(struct gl_es2_context *c, gl_es2_uint program, gl_es2_uint index, gl_es2_sizei bufsize, gl_es2_sizei *length, gl_es2_int *size, gl_es2_enum *type, gl_es2_char *name) { 
  apilog(c, "glGetActiveAttrib(0x%04X, %u, %d, ", program, index, bufsize);
  if (length) {
    apilog(c, "{ %d }, ", *length);
  }
  else {
    apilog(c, "NULL, ");
  }
  if (size) {
    apilog(c, "{ %d }, ", *size);
  }
  else {
    apilog(c, "NULL, ");
  }
  if (type) {
    const char *mtype = attrib_type(*type);
    if (mtype) {
      apilog(c, "{ %s }, ", mtype);
    }
    else {
      apilog(c, "{ 0x%04X }, ", *type);
    }
  }
  else {
    apilog(c, "NULL, ");
  }
  if (name) {
    apilog(c, "{ \"%s\" });\n", name);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_GetActiveUniform(struct gl_es2_context *c, gl_es2_uint program, gl_es2_uint index, gl_es2_sizei bufsize, gl_es2_sizei *length, gl_es2_int *size, gl_es2_enum *type, gl_es2_char *name) {
  apilog(c, "glGetActiveUniform(0x%04X, %u, %d, ", program, index, bufsize);

  if (length) {
    apilog(c, "{ %d }, ", *length);
  }
  else {
    apilog(c, "NULL, ");
  }
  if (size) {
    apilog(c, "{ %d }, ", *size);
  }
  else {
    apilog(c, "NULL, ");
  }
  if (type) {
    const char *mtype = attrib_type(*type);
    if (mtype) {
      apilog(c, "{ %s }, ", mtype);
    }
    else {
      apilog(c, "{ 0x%04X }, ", *type);
    }
  }
  else {
    apilog(c, "NULL, ");
  }
  if (name) {
    apilog(c, "{ \"%s\" });\n", name);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_GetAttachedShaders(struct gl_es2_context *c, gl_es2_uint program, gl_es2_sizei maxCount, gl_es2_sizei *count, gl_es2_uint *shaders) { 
  apilog(c, "glGetAttachedShaders(%u, %d, ", program, maxCount);
  if (count) {
    apilog(c, " { %d }, ", *count);
  }
  else {
    apilog(c, "NULL, ");
  }
  if (shaders) {
    if (count) {
      gl_es2_sizei n;
      apilog(c, "{ ");
      for (n = 0; n < *count; ++n) {
        apilog(c, "%s%d", n ? ", " : "", shaders[n]);
      }
      apilog(c, "});\n");
    }
    else {
      /* don't know how many */
      apilog(c, "{ });\n");
    }
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_GetAttribLocation(struct gl_es2_context *c, gl_es2_uint program, const gl_es2_char *name, gl_es2_int location_of_attrib_found) {
  apilog(c, "glGetAttribLocation(%u, ", program);
  if (name) {
    apilog(c, "\"%s\") = ", name);
  }
  else {
    apilog(c, "NULL) = ");
  }
  apilog(c, "%d;\n", location_of_attrib_found);
}

void gl_es2_log_GetBooleanv(struct gl_es2_context *c, gl_es2_enum pname, gl_es2_boolean *data) { 
}

void gl_es2_log_GetBufferParameteriv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params) { 
}

void gl_es2_log_GetError(struct gl_es2_context *c, gl_es2_enum error_returned) { 
}

void gl_es2_log_GetFloatv(struct gl_es2_context *c, gl_es2_enum pname, gl_es2_float *data) { 
}

void gl_es2_log_GetFramebufferAttachmentParameteriv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum pname, gl_es2_int *params) { 
}

void gl_es2_log_GetIntegerv(struct gl_es2_context *c, gl_es2_enum pname, gl_es2_int *data) { 
}

void gl_es2_log_GetProgramiv(struct gl_es2_context *c, gl_es2_uint program, gl_es2_enum pname, gl_es2_int *params) { 
}

void gl_es2_log_GetProgramInfoLog(struct gl_es2_context *c, gl_es2_uint program, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_char *infoLog) { 
}

void gl_es2_log_GetRenderbufferParameteriv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params) { 
}

void gl_es2_log_GetShaderiv(struct gl_es2_context *c, gl_es2_uint shader, gl_es2_enum pname, gl_es2_int *params) { 
}

void gl_es2_log_GetShaderInfoLog(struct gl_es2_context *c, gl_es2_uint shader, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_char *infoLog) { 
}

void gl_es2_log_GetShaderPrecisionFormat(struct gl_es2_context *c, gl_es2_enum shadertype, gl_es2_enum precisiontype, gl_es2_int *range, gl_es2_int *precision) { 
}

void gl_es2_log_GetShaderSource(struct gl_es2_context *c, gl_es2_uint shader, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_char *source) { 
}

void gl_es2_log_GetString(struct gl_es2_context *c, gl_es2_enum name, const gl_es2_ubyte *string_returned) {
}

void gl_es2_log_GetTexParameterfv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, gl_es2_float *params) { 
}

void gl_es2_log_GetTexParameteriv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params) { 
}

void gl_es2_log_GetUniformfv(struct gl_es2_context *c, gl_es2_uint program, gl_es2_int location, gl_es2_float *params) { 
}

void gl_es2_log_GetUniformiv(struct gl_es2_context *c, gl_es2_uint program, gl_es2_int location, gl_es2_int *params) { 
}

void gl_es2_log_GetUniformLocation(struct gl_es2_context *c, gl_es2_uint program, const gl_es2_char *name, gl_es2_int uniform_location_returned) {
}

void gl_es2_log_GetVertexAttribfv(struct gl_es2_context *c, gl_es2_uint index, gl_es2_enum pname, gl_es2_float *params) { 
}

void gl_es2_log_GetVertexAttribiv(struct gl_es2_context *c, gl_es2_uint index, gl_es2_enum pname, gl_es2_int *params) { 
}

void gl_es2_log_GetVertexAttribPointerv(struct gl_es2_context *c, gl_es2_uint index, gl_es2_enum pname, void **pointer) { 
}

void gl_es2_log_Hint(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum mode) { 
}

void gl_es2_log_IsBuffer(struct gl_es2_context *c, gl_es2_uint buffer, gl_es2_boolean is_it_a_buffer) {
}

void gl_es2_log_IsEnabled(struct gl_es2_context *c, gl_es2_enum cap, gl_es2_boolean is_it_enabled) {
}

void gl_es2_log_IsFramebuffer(struct gl_es2_context *c, gl_es2_uint framebuffer, gl_es2_boolean is_it_a_framebuffer) {
}

void gl_es2_log_IsProgram(struct gl_es2_context *c, gl_es2_uint program, gl_es2_boolean is_it_a_program) {
}

void gl_es2_log_IsRenderbuffer(struct gl_es2_context *c, gl_es2_uint renderbuffer, gl_es2_boolean is_it_a_renderbuffer) {
}

void gl_es2_log_IsShader(struct gl_es2_context *c, gl_es2_uint shader, gl_es2_boolean is_it_a_shader) {
}

void gl_es2_log_IsTexture(struct gl_es2_context *c, gl_es2_uint texture, gl_es2_boolean is_it_a_texture) {
}

void gl_es2_log_LineWidth(struct gl_es2_context *c, gl_es2_float width) { 
}

void gl_es2_log_LinkProgram(struct gl_es2_context *c, gl_es2_uint program) { 
}

void gl_es2_log_PixelStorei(struct gl_es2_context *c, gl_es2_enum pname, gl_es2_int param) { 
}

void gl_es2_log_PolygonOffset(struct gl_es2_context *c, gl_es2_float factor, gl_es2_float units) { 
}

void gl_es2_log_ReadPixels(struct gl_es2_context *c, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height, gl_es2_enum format, gl_es2_enum type, void *pixels) { 
}

void gl_es2_log_ReleaseShaderCompiler(struct gl_es2_context *c) { 
}

void gl_es2_log_RenderbufferStorage(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum internalformat, gl_es2_sizei width, gl_es2_sizei height) { 
}

void gl_es2_log_SampleCoverage(struct gl_es2_context *c, gl_es2_float value, gl_es2_boolean invert) { 
}

void gl_es2_log_Scissor(struct gl_es2_context *c, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height) { 
}

void gl_es2_log_ShaderBinary(struct gl_es2_context *c, gl_es2_sizei count, const gl_es2_uint *shaders, gl_es2_enum binaryformat, const void *binary, gl_es2_sizei length) { 
}

void gl_es2_log_ShaderSource(struct gl_es2_context *c, gl_es2_uint shader, gl_es2_sizei count, const gl_es2_char *const *string, const gl_es2_int *length) { 
}

void gl_es2_log_StencilFunc(struct gl_es2_context *c, gl_es2_enum func, gl_es2_int ref, gl_es2_uint mask) { 
}

void gl_es2_log_StencilFuncSeparate(struct gl_es2_context *c, gl_es2_enum face, gl_es2_enum func, gl_es2_int ref, gl_es2_uint mask) { 
}

void gl_es2_log_StencilMask(struct gl_es2_context *c, gl_es2_uint mask) { 
}

void gl_es2_log_StencilMaskSeparate(struct gl_es2_context *c, gl_es2_enum face, gl_es2_uint mask) { 
}

void gl_es2_log_StencilOp(struct gl_es2_context *c, gl_es2_enum fail, gl_es2_enum zfail, gl_es2_enum zpass) { 
}

void gl_es2_log_StencilOpSeparate(struct gl_es2_context *c, gl_es2_enum face, gl_es2_enum sfail, gl_es2_enum dpfail, gl_es2_enum dppass) { 
}

void gl_es2_log_TexImage2D(struct gl_es2_context *c, gl_es2_enum target, gl_es2_int level, gl_es2_int internalformat, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border, gl_es2_enum format, gl_es2_enum type, const void *pixels) { 
  const char *mtgt = tex_target(target);
  const char *mifmt = tex_format(internalformat);
  const char *mfmt = tex_format(format);
  const char *mtype = tex_type(type);
  if (mtgt) {
    apilog(c, "glTexImage2D(%s, ", mtgt);
  }
  else {
    apilog(c, "glTexImage2D(0x%04X, ", target);
  }
  apilog(c, "%d, ", level);
  if (mifmt) {
    apilog(c, "%s, ", mifmt);
  }
  else {
    apilog(c, "0x%04X, ", internalformat);
  }
  apilog(c, "%d, %d, %d, ", width, height, border);
  if (mfmt) {
    apilog(c, "%s, ", mfmt);
  }
  else {
    apilog(c, "0x%04X, ", format);
  }
  if (mtype) {
    apilog(c, "%s, ", mtype);
  }
  else {
    apilog(c, "0x%04X, ", mtype);
  }
  if (pixels) {
    apilog(c, "0x%p);\n", pixels);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_TexParameterf(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, gl_es2_float param) { 
}

void gl_es2_log_TexParameterfv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, const gl_es2_float *params) { 
}

void gl_es2_log_TexParameteri(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, gl_es2_int param) { 
}

void gl_es2_log_TexParameteriv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, const gl_es2_int *params) { 
}

void gl_es2_log_TexSubImage2D(struct gl_es2_context *c, gl_es2_enum target, gl_es2_int level, gl_es2_int xoffset, gl_es2_int yoffset, gl_es2_sizei width, gl_es2_sizei height, gl_es2_enum format, gl_es2_enum type, const void *pixels) { 
  const char *mtgt = tex_target(target);
  const char *mfmt = tex_format(format);
  const char *mtype = tex_type(type);
  if (mtgt) {
    apilog(c, "glTexSubImage2D(%s, ", mtgt);
  }
  else {
    apilog(c, "glTexSubImage2D(0x%04X, ", target);
  }
  apilog(c, "%d, ", level);
  apilog(c, "%d, %d, %d, %d, %d, ", xoffset, yoffset, width, height);
  if (mfmt) {
    apilog(c, "%s, ", mfmt);
  }
  else {
    apilog(c, "0x%04X, ", format);
  }
  if (mtype) {
    apilog(c, "%s, ", mtype);
  }
  else {
    apilog(c, "0x%04X, ", mtype);
  }
  if (pixels) {
    apilog(c, "0x%p);\n", pixels);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_Uniform1f(struct gl_es2_context *c, gl_es2_int location, gl_es2_float v0) { 
}

void gl_es2_log_Uniform1fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) { 
}

void gl_es2_log_Uniform1i(struct gl_es2_context *c, gl_es2_int location, gl_es2_int v0) { 
}

void gl_es2_log_Uniform1iv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) { 
}

void gl_es2_log_Uniform2f(struct gl_es2_context *c, gl_es2_int location, gl_es2_float v0, gl_es2_float v1) { 
}

void gl_es2_log_Uniform2fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) { 
}

void gl_es2_log_Uniform2i(struct gl_es2_context *c, gl_es2_int location, gl_es2_int v0, gl_es2_int v1) { 
}

void gl_es2_log_Uniform2iv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) { 
}

void gl_es2_log_Uniform3f(struct gl_es2_context *c, gl_es2_int location, gl_es2_float v0, gl_es2_float v1, gl_es2_float v2) { 
}

void gl_es2_log_Uniform3fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) { 
}

void gl_es2_log_Uniform3i(struct gl_es2_context *c, gl_es2_int location, gl_es2_int v0, gl_es2_int v1, gl_es2_int v2) { 
}

void gl_es2_log_Uniform3iv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) { 
}

void gl_es2_log_Uniform4f(struct gl_es2_context *c, gl_es2_int location, gl_es2_float v0, gl_es2_float v1, gl_es2_float v2, gl_es2_float v3) { 
}

void gl_es2_log_Uniform4fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) { 
}

void gl_es2_log_Uniform4i(struct gl_es2_context *c, gl_es2_int location, gl_es2_int v0, gl_es2_int v1, gl_es2_int v2, gl_es2_int v3) { 
}

void gl_es2_log_Uniform4iv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) { 
}

void gl_es2_log_UniformMatrix2fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value) { 
}

void gl_es2_log_UniformMatrix3fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value) { 
}

void gl_es2_log_UniformMatrix4fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value) { 
}

void gl_es2_log_UseProgram(struct gl_es2_context *c, gl_es2_uint program) { 
}

void gl_es2_log_ValidateProgram(struct gl_es2_context *c, gl_es2_uint program) { 
}

void gl_es2_log_VertexAttrib1f(struct gl_es2_context *c, gl_es2_uint index, gl_es2_float x) { 
}

void gl_es2_log_VertexAttrib1fv(struct gl_es2_context *c, gl_es2_uint index, const gl_es2_float *v) { 
}

void gl_es2_log_VertexAttrib2f(struct gl_es2_context *c, gl_es2_uint index, gl_es2_float x, gl_es2_float y) { 
}

void gl_es2_log_VertexAttrib2fv(struct gl_es2_context *c, gl_es2_uint index, const gl_es2_float *v) { 
}

void gl_es2_log_VertexAttrib3f(struct gl_es2_context *c, gl_es2_uint index, gl_es2_float x, gl_es2_float y, gl_es2_float z) { 
}

void gl_es2_log_VertexAttrib3fv(struct gl_es2_context *c, gl_es2_uint index, const gl_es2_float *v) { 
}

void gl_es2_log_VertexAttrib4f(struct gl_es2_context *c, gl_es2_uint index, gl_es2_float x, gl_es2_float y, gl_es2_float z, gl_es2_float w) { 
}

void gl_es2_log_VertexAttrib4fv(struct gl_es2_context *c, gl_es2_uint index, const gl_es2_float *v) { 
}

static const char *vtx_attrib_type(gl_es2_enum type) {
  switch (type) {
    case GL_ES2_BYTE: return "GL_BYTE";
    case GL_ES2_UNSIGNED_BYTE: return "GL_UNSIGNED_BYTE";
    case GL_ES2_SHORT: return "GL_SHORT";
    case GL_ES2_UNSIGNED_SHORT: return "GL_UNSIGNED_SHORT";
    case GL_ES2_FIXED: return "GL_FIXED";
    case GL_ES2_FLOAT: return "GL_FLOAT";
    default:
      return NULL;
  }
}

void gl_es2_log_VertexAttribPointer(struct gl_es2_context *c, gl_es2_uint index, gl_es2_int size, gl_es2_enum type, gl_es2_boolean normalized, gl_es2_sizei stride, const void *pointer) { 
  const char *mtype = vtx_attrib_type(type);
  if (type) {
    apilog(c, "glVertexAttribPointer(%u, %d, %s, ", index, size, mtype);
  }
  else {
    apilog(c, "glVertexAttribPointer(%u, %d, 0x%04X, ", index, size, type);
  }
  if (normalized == GL_ES2_TRUE) {
    apilog(c, "GL_TRUE, ");
  }
  else if (normalized == GL_ES2_FALSE) {
    apilog(c, "GL_FALSE, ");
  }
  else {
    apilog(c, "%d, ", normalized);
  }
  apilog(c, "%d, ", stride);
  if (pointer) {
    apilog(c, "0x%p);\n", pointer);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_Viewport(struct gl_es2_context *c, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height) {
}



