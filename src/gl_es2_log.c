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

static const char *pnametxt(gl_es2_enum pname) {
  switch (pname) {
    case GL_ES2_MAX_COMBINED_TEXTURE_IMAGE_UNITS: return "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS";
    case GL_ES2_ACTIVE_TEXTURE: return "GL_ACTIVE_TEXTURE";
    case GL_ES2_MAX_VERTEX_ATTRIBS: return "GL_MAX_VERTEX_ATTRIBS";
    case GL_ES2_NUM_COMPRESSED_TEXTURE_FORMATS: return "GL_NUM_COMPRESSED_TEXTURE_FORMATS";
    case GL_ES2_COMPRESSED_TEXTURE_FORMATS: return "GL_COMPRESSED_TEXTURE_FORMATS";
    case GL_ES2_DEPTH_RANGE: return "GL_DEPTH_RANGE";
    case GL_ES2_FRONT_FACE: return "GL_FRONT_FACE";
    case GL_ES2_CULL_FACE_MODE: return "GL_CULL_FACE_MODE";
    case GL_ES2_MAX_VIEWPORT_DIMS: return "GL_MAX_VIEWPORT_DIMS";
    case GL_ES2_MAX_TEXTURE_SIZE: return "GL_MAX_TEXTURE_SIZE";
    case GL_ES2_MAX_CUBE_MAP_TEXTURE_SIZE: return "GL_MAX_CUBE_MAP_TEXTURE_SIZE";
    case GL_ES2_VIEWPORT: return "GL_VIEWPORT";
    case GL_ES2_STENCIL_FUNC: return "GL_STENCIL_FUNC";
    case GL_ES2_STENCIL_VALUE_MASK: return "GL_STENCIL_VALUE_MASK";
    case GL_ES2_STENCIL_REF: return "GL_STENCIL_REF";
    case GL_ES2_STENCIL_BACK_FUNC: return "GL_STENCIL_BACK_FUNC";
    case GL_ES2_STENCIL_BACK_VALUE_MASK: return "GL_STENCIL_BACK_VALUE_MASK";
    case GL_ES2_STENCIL_BACK_REF: return "GL_STENCIL_BACK_REF";
    case GL_ES2_STENCIL_BITS: return "GL_STENCIL_BITS";
    case GL_ES2_STENCIL_FAIL: return "GL_STENCIL_FAIL";
    case GL_ES2_STENCIL_PASS_DEPTH_PASS: return "GL_STENCIL_PASS_DEPTH_PASS";
    case GL_ES2_STENCIL_PASS_DEPTH_FAIL: return "GL_STENCIL_PASS_DEPTH_FAIL";
    case GL_ES2_STENCIL_BACK_FAIL: return "GL_STENCIL_BACK_FAIL";
    case GL_ES2_STENCIL_BACK_PASS_DEPTH_PASS: return "GL_STENCIL_BACK_PASS_DEPTH_PASS";
    case GL_ES2_STENCIL_BACK_PASS_DEPTH_FAIL: return "GL_STENCIL_BACK_PASS_DEPTH_FAIL";
    case GL_ES2_POLYGON_OFFSET_FACTOR: return "GL_POLYGON_OFFSET_FACTOR";
    case GL_ES2_POLYGON_OFFSET_UNITS: return "GL_POLYGON_OFFSET_UNITS";
    case GL_ES2_ALIASED_LINE_WIDTH_RANGE: return "GL_ALIASED_LINE_WIDTH_RANGE";
    case GL_ES2_ALIASED_POINT_SIZE_RANGE: return "GL_ALIASED_POINT_SIZE_RANGE";
    case GL_ES2_LINE_WIDTH: return "GL_LINE_WIDTH";
    case GL_ES2_PACK_ALIGNMENT: return "GL_PACK_ALIGNMENT";
    case GL_ES2_UNPACK_ALIGNMENT: return "GL_UNPACK_ALIGNMENT";
    case GL_ES2_CURRENT_PROGRAM: return "GL_CURRENT_PROGRAM";
    case GL_ES2_IMPLEMENTATION_COLOR_READ_FORMAT: return "GL_IMPLEMENTATION_COLOR_READ_FORMAT";
    case GL_ES2_IMPLEMENTATION_COLOR_READ_TYPE: return "GL_IMPLEMENTATION_COLOR_READ_TYPE";
    case GL_ES2_SAMPLE_COVERAGE_VALUE: return "GL_SAMPLE_COVERAGE_VALUE";
    case GL_ES2_SAMPLE_COVERAGE_INVERT: return "GL_SAMPLE_COVERAGE_INVERT";
    case GL_ES2_SAMPLE_ALPHA_TO_COVERAGE: return "GL_SAMPLE_ALPHA_TO_COVERAGE";
    case GL_ES2_SAMPLE_BUFFERS: return "GL_SAMPLE_BUFFERS";
    case GL_ES2_SAMPLES: return "GL_SAMPLES";
    case GL_ES2_NUM_SHADER_BINARY_FORMATS: return "GL_NUM_SHADER_BINARY_FORMATS";
    case GL_ES2_SHADER_BINARY_FORMATS: return "GL_SHADER_BINARY_FORMATS";
    case GL_ES2_SHADER_COMPILER: return "GL_SHADER_COMPILER";
    default:
      return NULL;
  }
}

void gl_es2_log_GetBooleanv(struct gl_es2_context *c, gl_es2_enum pname, gl_es2_boolean *data) { 
  const char *mpname = pnametxt(pname);
  if (mpname) {
    apilog(c, "glGetBooleanv(%s, ", mpname);
  }
  else {
    apilog(c, "glGetBoolean(0x%04X, ", pname);
  }

  if (data) {
    int param_count = 1;
    switch (pname) {
      case GL_ES2_DEPTH_RANGE:
      case GL_ES2_MAX_VIEWPORT_DIMS:
      case GL_ES2_ALIASED_LINE_WIDTH_RANGE:
      case GL_ES2_ALIASED_POINT_SIZE_RANGE:
        param_count = 2;
        break;
      case GL_ES2_VIEWPORT:
        param_count = 4;
        break;
    }
    int n;
    apilog(c, "{ ");
    for (n = 0; n < param_count; ++n) {
      const char *boolt = NULL;
      if (data[n] == GL_ES2_TRUE) boolt = "GL_TRUE";
      else if (data[n] == GL_ES2_FALSE) boolt = "GL_FALSE";
      if (boolt) {
        apilog(c, "%s%s", n ? ", " : "", boolt);
      }
      else {
        apilog(c, "%s0x%04X", n ? ", " : "", data[n]);
      }
    }
    apilog(c, "} );\n");
  }
}

static const char *buffer_param(gl_es2_enum pname) {
  switch (pname) {
    case GL_ES2_BUFFER_SIZE:
      return "GL_BUFFER_SIZE";
    case GL_ES2_BUFFER_USAGE:
      return "GL_BUFFER_USAGE";
    default:
      return NULL;
  }
}

void gl_es2_log_GetBufferParameteriv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params) { 
  const char *mtgt = buffer_target(target);
  const char *mpname = buffer_param(pname);
  if (mtgt) {
    apilog(c, "glGetBufferParameteriv(%s, ", mtgt);
  }
  else {
    apilog(c, "glGetBufferParameteriv(0x%04X, ", target);
  }
  if (mpname) {
    apilog(c, "%s, ", mpname);
  }
  else {
    apilog(c, "0x%04X, ", pname);
  }
  if (params) {
    apilog(c, "{ 0x%04X });\n", params[0]);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_GetError(struct gl_es2_context *c, gl_es2_enum error_returned) { 
  const char *msg = NULL;
  switch (error_returned) {
    case GL_ES2_NO_ERROR: msg = "GL_NO_ERROR"; break;
    case GL_ES2_INVALID_ENUM: msg = "GL_INVALID_ENUM"; break;
    case GL_ES2_INVALID_VALUE: msg = "GL_INVALID_VALUE"; break;
    case GL_ES2_INVALID_OPERATION: msg = "GL_INVALID_OPERATION"; break;
    case GL_ES2_INVALID_FRAMEBUFFER_OPERATION: msg = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
    case GL_ES2_OUT_OF_MEMORY: msg = "GL_OUT_OF_MEMORY"; break;
  }
  if (msg) {
    apilog(c, "glGetError() = %s;\n", msg);
  }
  else {
    apilog(c, "glGetError() = 0x%04X;\n", error_returned);
  }
}

void gl_es2_log_GetFloatv(struct gl_es2_context *c, gl_es2_enum pname, gl_es2_float *data) { 
  const char *mpname = pnametxt(pname);
  if (mpname) {
    apilog(c, "glGetFloatv(%s, ", mpname);
  }
  else {
    apilog(c, "glGetFloatv(0x%04X, ", pname);
  }

  if (data) {
    int param_count = 1;
    switch (pname) {
      case GL_ES2_DEPTH_RANGE:
      case GL_ES2_MAX_VIEWPORT_DIMS:
      case GL_ES2_ALIASED_LINE_WIDTH_RANGE:
      case GL_ES2_ALIASED_POINT_SIZE_RANGE:
        param_count = 2;
        break;
      case GL_ES2_VIEWPORT:
        param_count = 4;
        break;
    }
    int n;
    apilog(c, "{ ");
    for (n = 0; n < param_count; ++n) {
      apilog(c, "%s%f", n ? ", " : "", data[n]);
    }
    apilog(c, "} );\n");
  }
}

static const char *fbattachment(gl_es2_enum attachment) {
  switch (attachment) {
    case GL_ES2_COLOR_ATTACHMENT0: return "GL_COLOR_ATTACHMENT0";
    case GL_ES2_DEPTH_ATTACHMENT: return "GL_DEPTH_ATTACHMENT";
    case GL_ES2_STENCIL_ATTACHMENT: return "GL_STENCIL_ATTACHMENT";
    default: return NULL;
  }
}

static const char *fbattach_pname(gl_es2_enum pname) {
  switch (pname) {
    case GL_ES2_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:
      return "GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME";
    case GL_ES2_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL:
      return "GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL";
    case GL_ES2_FRAMEBUFFER_ATTACHMENT_CUBE_MAP_FACE:
      return "GL_FRAMEBUFFER_ATTACHMENT_CUBE_MAP_FACE";
    default:
      return NULL;
  }
}

void gl_es2_log_GetFramebufferAttachmentParameteriv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum pname, gl_es2_int *params) { 
  const char *mattachment = fbattachment(attachment);
  const char *mpname = fbattach_pname(pname);
  if (target == GL_ES2_FRAMEBUFFER) {
    apilog(c, "glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, ");
  }
  else {
    apilog(c, "glGetFramebufferAttachmentParameteriv(0x%04X, ", target);
  }
  if (mattachment) {
    apilog(c, "%s, ", mattachment);
  }
  else {
    apilog(c, "0x%04X, ", attachment);
  }
  if (mpname) {
    apilog(c, "%s, ", mpname);
  }
  else {
    apilog(c, "0x%04X, ", pname);
  }
  if (!params) {
    apilog(c, "NULL);\n");
  }
  else if (pname == GL_ES2_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE) {
    switch (*params) {
      case GL_ES2_NONE: apilog(c, "{ GL_NONE });\n"); break;
      case GL_ES2_RENDERBUFFER: apilog(c, "{ GL_RENDERBUFFER });\n"); break;
      case GL_ES2_TEXTURE: apilog(c, " { GL_TEXTURE });\n"); break;
      default: apilog(c, "{ 0x%04X });\n", *params); break;
    }
  }
  else if (pname == GL_ES2_FRAMEBUFFER_ATTACHMENT_CUBE_MAP_FACE) {
    switch (*params) {
      case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_X: apilog(c, "{ GL_TEXTURE_CUBE_MAP_POSITIVE_X });\n"); break;
      case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_X: apilog(c, "{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X });\n"); break;
      case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Y: apilog(c, "{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y });\n"); break;
      case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Y: apilog(c, "{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y });\n"); break;
      case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Z: apilog(c, "{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z });\n"); break;
      case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Z: apilog(c, "{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z });\n"); break;
      default: apilog(c, "{ 0x%04X });\n", *params); break;
    }
  }
  else {
    apilog(c, "{ %d });\n", *params);
  }
}

void gl_es2_log_GetIntegerv(struct gl_es2_context *c, gl_es2_enum pname, gl_es2_int *data) { 
  const char *mpname = pnametxt(pname);
  if (mpname) {
    apilog(c, "glGetIntegerv(%s, ", mpname);
  }
  else {
    apilog(c, "glGetIntegerv(0x%04X, ", pname);
  }

  if (data) {
    int param_count = 1;
    switch (pname) {
      case GL_ES2_DEPTH_RANGE:
      case GL_ES2_MAX_VIEWPORT_DIMS:
      case GL_ES2_ALIASED_LINE_WIDTH_RANGE:
      case GL_ES2_ALIASED_POINT_SIZE_RANGE:
        param_count = 2;
        break;
      case GL_ES2_VIEWPORT:
        param_count = 4;
        break;
    }
    int n;
    apilog(c, "{ ");
    for (n = 0; n < param_count; ++n) {
      apilog(c, "%s%d", n ? ", " : "", data[n]);
    }
    apilog(c, "} );\n");
  }
}

static const char *prog_pname(gl_es2_enum pname) {
  switch (pname) {
    case GL_ES2_DELETE_STATUS: return "GL_DELETE_STATUS";
    case GL_ES2_LINK_STATUS: return "GL_LINK_STATUS";
    case GL_ES2_VALIDATE_STATUS: return "GL_VALIDATE_STATUS";
    case GL_ES2_INFO_LOG_LENGTH: return "GL_INFO_LOG_LENGTH";
    case GL_ES2_ATTACHED_SHADERS: return "GL_ATTACHED_SHADERS";
    case GL_ES2_ACTIVE_ATTRIBUTES: return "GL_ACTIVE_ATTRIBUTES";
    case GL_ES2_ACTIVE_ATTRIBUTE_MAX_LENGTH: return "GL_ACTIVE_ATTRIBUTE_MAX_LENGTH";
    case GL_ES2_ACTIVE_UNIFORMS: return "GL_ACTIVE_UNIFORMS";
    case GL_ES2_ACTIVE_UNIFORM_MAX_LENGTH: return "GL_ACTIVE_UNIFORM_MAX_LENGTH";
    default: return NULL;
  }
}

void gl_es2_log_GetProgramiv(struct gl_es2_context *c, gl_es2_uint program, gl_es2_enum pname, gl_es2_int *params) { 
  const char *mpname = prog_pname(pname);
  apilog(c, "glGetProgramiv(%u, ", program);
  if (mpname) {
    apilog(c, "%s, ", mpname);
  }
  else {
    apilog(c, "0x%04X, ", pname);
  }
  if (params) {
    apilog(c, "{ %d });\n", *params);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_GetProgramInfoLog(struct gl_es2_context *c, gl_es2_uint program, gl_es2_sizei bufsize, gl_es2_sizei *length, gl_es2_char *infoLog) { 
  apilog(c, "glGetProgramInfoLog(%u, %d, ", program, bufsize);
  if (length) {
    apilog(c, "{ %d }, ", *length);
  }
  else {
    apilog(c, "NULL, ");
  }
  if (infoLog) {
    apilog(c, "\"%s\");\n", infoLog);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

static const char *renderbuf_pname(gl_es2_enum pname) {
  switch (pname) {
    case GL_ES2_RENDERBUFFER_WIDTH: return "GL_RENDERBUFFER_WIDTH";
    case GL_ES2_RENDERBUFFER_HEIGHT: return "GL_RENDERBUFFER_HEIGHT";
    case GL_ES2_RENDERBUFFER_INTERNAL_FORMAT: return "GL_RENDERBUFFER_INTERNAL_FORMAT";
    case GL_ES2_RENDERBUFFER_RED_SIZE: return "GL_RENDERBUFFER_RED_SIZE";
    case GL_ES2_RENDERBUFFER_GREEN_SIZE: return "GL_RENDERBUFFER_GREEN_SIZE";
    case GL_ES2_RENDERBUFFER_BLUE_SIZE: return "GL_RENDERBUFFER_BLUE_SIZE";
    case GL_ES2_RENDERBUFFER_ALPHA_SIZE: return "GL_RENDERBUFFER_ALPHA_SIZE";
    case GL_ES2_RENDERBUFFER_DEPTH_SIZE: return "GL_RENDERBUFFER_DEPTH_SIZE";
    case GL_ES2_RENDERBUFFER_STENCIL_SIZE: return "GL_RENDERBUFFER_STENCIL_SIZE";
    default: return NULL;
  }  
}

void gl_es2_log_GetRenderbufferParameteriv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params) { 
  const char *mpname = renderbuf_pname(pname);
  if (target == GL_ES2_RENDERBUFFER) {
    apilog(c, "glGetRenderbufferParameteriv(GL_RENDERBUFFER, ");
  }
  else {
    apilog(c, "glGetRenderbufferParameteriv(0x%04X, ", target);
  }
  if (mpname) {
    apilog(c, "%s, ", mpname);
  }
  else {
    apilog(c, "0x%04X, ", pname);
  }
  if (!params) {
    apilog(c, "NULL);\n");
  }
  else if (pname == GL_ES2_RENDERBUFFER_INTERNAL_FORMAT) {
    switch (*params) {
      case GL_ES2_RGBA4: apilog(c, "{ GL_RGBA4 });\n"); break;
      case GL_ES2_RGBA8: apilog(c, "{ GL_RGBA8 });\n"); break;
      case GL_ES2_DEPTH_COMPONENT16: apilog(c, "{ GL_DEPTH_COMPONENT16 });\n"); break;
      case GL_ES2_DEPTH_COMPONENT32: apilog(c, "{ GL_OES_depth32 });\n"); break;
      case GL_ES2_STENCIL_INDEX16: apilog(c, "{ GL_STENCIL_INDEX16 });\n"); break;
      default: apilog(c, "{ 0x%04X });\n", *params); break;
    }
  }
  else {
   apilog(c, "{ %d });\n", *params); 
  }
}

static const char *shader_pname(gl_es2_enum pname) {
  switch (pname) {
    case GL_ES2_SHADER_TYPE: return "GL_SHADER_TYPE";
    case GL_ES2_DELETE_STATUS: return "GL_DELETE_STATUS";
    case GL_ES2_COMPILE_STATUS: return "GL_COMPILE_STATUS";
    case GL_ES2_INFO_LOG_LENGTH: return "GL_INFO_LOG_LENGTH";
    case GL_ES2_SHADER_SOURCE_LENGTH: return "GL_SHADER_SOURCE_LENGTH";
    default: return NULL;
  }
}

static const char *shader_type(gl_es2_enum shadertype) {
  switch (shadertype) {
    case GL_ES2_VERTEX_SHADER: return "GL_VERTEX_SHADER";
    case GL_ES2_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
    case AEX_GL_DEBUG_SHADER: return "0x108B31 /* AEX_GL_DEBUG_SHADER */";
    default: return NULL;
  }
}

void gl_es2_log_GetShaderiv(struct gl_es2_context *c, gl_es2_uint shader, gl_es2_enum pname, gl_es2_int *params) { 
  const char *mpname = shader_pname(pname);
  if (mpname) {
    apilog(c, "glGetShaderiv(%u, %s, ", shader, mpname);
  }
  else {
    apilog(c, "glGetShaderiv(%u, 0x%04X, ", shader, pname);
  }
  if (!params) {
    apilog(c, "NULL);\n");
  }
  else if (pname == GL_ES2_SHADER_TYPE) {
    const char *mshadertype = shader_type(*params);
    if (mshadertype) {
      apilog(c, "{ %s });\n", mshadertype);
    }
    else {
      apilog(c, "{ 0x%04X });\n", *params);
    }
  }
  else {
    apilog(c, "{ %d });\n", *params);
  }
}

void gl_es2_log_GetShaderInfoLog(struct gl_es2_context *c, gl_es2_uint shader, gl_es2_sizei bufsize, gl_es2_sizei *length, gl_es2_char *infoLog) { 
  apilog(c, "glGetShaderInfoLog(%u, %d, ", shader, bufsize);
  if (length) {
    apilog(c, "{ %d }, ", *length);
  }
  else {
    apilog(c, "NULL, ");
  }
  if (infoLog) {
    apilog(c, "\"%s\");\n", infoLog);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

static const char *precision_type(gl_es2_enum precisiontype) {
  switch (precisiontype) {
    case GL_ES2_LOW_FLOAT: return "GL_LOW_FLOAT";
    case GL_ES2_MEDIUM_FLOAT: return "GL_MEDIUM_FLOAT";
    case GL_ES2_HIGH_FLOAT: return "GL_HIGH_FLOAT";
    case GL_ES2_LOW_INT: return "GL_LOW_INT";
    case GL_ES2_MEDIUM_INT: return "GL_MEDIUM_INT";
    case GL_ES2_HIGH_INT: return "GL_HIGH_INT";
    default: return NULL;
  }
}

void gl_es2_log_GetShaderPrecisionFormat(struct gl_es2_context *c, gl_es2_enum shadertype, gl_es2_enum precisiontype, gl_es2_int *range, gl_es2_int *precision) { 
  const char *mshadertype = shader_type(shadertype);
  const char *mprecisiontype = precision_type(precisiontype);
  if (mshadertype) {
    apilog(c, "glGetShaderPrecisionFormat(%s, ", mshadertype);
  }
  else {
    apilog(c, "glGetShaderPrecisionFormat(0x%04X, ", shadertype);
  }
  if (mprecisiontype) {
    apilog(c, "%s, ", mprecisiontype);
  }
  else {
    apilog(c, "0x%04X, ", precisiontype);
  }
  if (!range) {
    apilog(c, "NULL, ");
  }
  else {
    apilog(c, "{ %d, %d }, ", range[0], range[1]);
  }
  if (!precision) {
    apilog(c, "NULL);\n");
  }
  else {
    apilog(c, "{ %d });\n", *precision);
  }
}

void gl_es2_log_GetShaderSource(struct gl_es2_context *c, gl_es2_uint shader, gl_es2_sizei bufsize, gl_es2_sizei *length, gl_es2_char *source) {
  apilog(c, "glGetShaderSource(%u, %d, ", shader, bufsize);
  if (length) {
    apilog(c, "{ %d }, ", *length);
  }
  else {
    apilog(c, "NULL, ");
  }
  if (source) {
    apilog(c, "\"%s\");\n", source);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_GetString(struct gl_es2_context *c, gl_es2_enum name, const gl_es2_ubyte *string_returned) {
  switch (name) {
    case GL_ES2_VENDOR: apilog(c, "glGetString(GL_VENDOR) = "); break;
    case GL_ES2_RENDERER: apilog(c, "glGetString(GL_RENDERER) = "); break;
    case GL_ES2_VERSION: apilog(c, "glGetString(GL_VERSION) = "); break;
    case GL_ES2_SHADING_LANGUAGE_VERSION: apilog(c, "glGetString(GL_SHADING_LANGUAGE_VERSION) = "); break;
    case GL_ES2_EXTENSIONS: apilog(c, "glGetString(GL_EXTENSIONS) = "); break;
    default: apilog(c, "glGetString(0x%04X) = ", name); break;
  }
  if (string_returned) {
    apilog(c, "\"%s\");\n", string_returned);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

static const char *tex_parameter_pname(gl_es2_enum pname) {
  switch (pname) {
    case GL_ES2_TEXTURE_MAG_FILTER: return "GL_TEXTURE_MAG_FILTER";
    case GL_ES2_TEXTURE_MIN_FILTER: return "GL_TEXTURE_MIN_FILTER";
    case GL_ES2_TEXTURE_WRAP_S: return "GL_TEXTURE_WRAP_S";
    case GL_ES2_TEXTURE_WRAP_T: return "GL_TEXTURE_WRAP_T";
    case GL_ES2_TEXTURE_MAX_LEVEL: return "GL_TEXTURE_MAX_LEVEL";
    default: return NULL;
  }
}

void gl_es2_log_GetTexParameterfv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, gl_es2_float *params) { 
  const char *mtextarget = tex_target(target);
  const char *mpname = tex_parameter_pname(pname);
  if (mtextarget) {
    apilog(c, "glGetTexParameterfv(%s, ", mtextarget);
  }
  else {
    apilog(c, "glGetTexParamterfv(0x%04X, ", target);
  }
  if (mpname) {
    apilog(c, "%s, ", mpname);
  }
  else {
    apilog(c, "0x%04X, ", pname);
  }
  if (!params) {
    apilog(c, "NULL);\n");
  }
  else if (pname == GL_ES2_TEXTURE_MAX_LEVEL) {
    apilog(c, "{ %f });\n", *params);
  }
  else if ((pname == GL_ES2_TEXTURE_MIN_FILTER) || (pname == GL_ES2_TEXTURE_MAG_FILTER)) {
    if ((*params) == (float)GL_ES2_NEAREST) apilog(c, "{ GL_NEAREST });\n");
    else if ((*params) == (float)GL_ES2_LINEAR) apilog(c, "{ GL_LINEAR });\n");
    else if ((*params) == (float)GL_ES2_NEAREST_MIPMAP_NEAREST) apilog(c, "{ GL_NEAREST_MIPMAP_NEAREST });\n");
    else if ((*params) == (float)GL_ES2_LINEAR_MIPMAP_NEAREST) apilog(c, "{ GL_LINEAR_MIPMAP_NEAREST });\n");
    else if ((*params) == (float)GL_ES2_LINEAR_MIPMAP_LINEAR) apilog(c, "{ GL_LINEAR_MIPMAP_LINEAR });\n");
    else apilog(c, "{ %f });\n", *params);
  }
  else if ((pname == GL_ES2_TEXTURE_WRAP_S) || (pname == GL_ES2_TEXTURE_WRAP_T)) {
    if ((*params) == (float)GL_ES2_CLAMP_TO_EDGE) apilog(c, "{ GL_CLAMP_TO_EDGE });\n");
    else if ((*params) == (float)GL_ES2_REPEAT) apilog(c, "{ GL_REPEAT });\n");
    else if ((*params) == (float)GL_ES2_MIRRORED_REPEAT) apilog(c, "{ GL_MIRRORED_REPEAT });\n");
    else apilog(c, "{ %f });\n", *params);
  }
  else {
    apilog(c, "{ %f });\n", *params);
  }
}

void gl_es2_log_GetTexParameteriv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params) { 
  const char *mtextarget = tex_target(target);
  const char *mpname = tex_parameter_pname(pname);
  if (mtextarget) {
    apilog(c, "glGetTexParameteriv(%s, ", mtextarget);
  }
  else {
    apilog(c, "glGetTexParamteriv(0x%04X, ", target);
  }
  if (mpname) {
    apilog(c, "%s, ", mpname);
  }
  else {
    apilog(c, "0x%04X, ", pname);
  }
  if (!params) {
    apilog(c, "NULL);\n");
  }
  else if (pname == GL_ES2_TEXTURE_MAX_LEVEL) {
    apilog(c, "{ %d });\n", *params);
  }
  else if ((pname == GL_ES2_TEXTURE_MIN_FILTER) || (pname == GL_ES2_TEXTURE_MAG_FILTER)) {
    if ((*params) == GL_ES2_NEAREST) apilog(c, "{ GL_NEAREST });\n");
    else if ((*params) == GL_ES2_LINEAR) apilog(c, "{ GL_LINEAR });\n");
    else if ((*params) == GL_ES2_NEAREST_MIPMAP_NEAREST) apilog(c, "{ GL_NEAREST_MIPMAP_NEAREST });\n");
    else if ((*params) == GL_ES2_LINEAR_MIPMAP_NEAREST) apilog(c, "{ GL_LINEAR_MIPMAP_NEAREST });\n");
    else if ((*params) == GL_ES2_LINEAR_MIPMAP_LINEAR) apilog(c, "{ GL_LINEAR_MIPMAP_LINEAR });\n");
    else apilog(c, "{ %d });\n", *params);
  }
  else if ((pname == GL_ES2_TEXTURE_WRAP_S) || (pname == GL_ES2_TEXTURE_WRAP_T)) {
    if ((*params) == GL_ES2_CLAMP_TO_EDGE) apilog(c, "{ GL_CLAMP_TO_EDGE });\n");
    else if ((*params) == GL_ES2_REPEAT) apilog(c, "{ GL_REPEAT });\n");
    else if ((*params) == GL_ES2_MIRRORED_REPEAT) apilog(c, "{ GL_MIRRORED_REPEAT });\n");
    else apilog(c, "{ %d });\n", *params);
  }
  else {
    apilog(c, "{ %d });\n", *params);
  }
}

void gl_es2_log_GetUniformfv(struct gl_es2_context *c, gl_es2_uint program, gl_es2_int location, gl_es2_float *params) { 
  /* Replicate a bunch of what the API does for logging purposes as we have to get to the type of the uniform
   * if we're to log it accurately. */
  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    apilog(c, "glGetUniformfv(%u /* invalid */, %d, ", program, location);
    if (params) {
      apilog(c, "0x%p {..});\n", params);
    }
    else {
      apilog(c, "NULL);\n");
    }
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    apilog(c, "glGetUniformfv(%u /* program not linked */, %d, ", program, location);
    if (params) {
      apilog(c, "0x%p {..});\n", params);
    }
    else {
      apilog(c, "NULL);\n");
    }
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, NULL, NULL);
  if (r) {
    apilog(c, "glGetUniformfv(%u, %d /* failed to get location */, ", program, location);
    if (params) {
      apilog(c, "0x%p {..});\n", params);
    }
    else {
      apilog(c, "NULL);\n");
    }
    return;
  }
  if (!params) {
    apilog(c, "glGetUniformfv(%u, %d, NULL);\n", program, location);
    return;
  }
  int cardinality;
  switch (slrak) {
    case slrak_float:
    case slrak_int:
    case slrak_bool:
    case slrak_sampler2D:
    case slrak_samplerCube:
      cardinality = 1;
      break;
    case slrak_vec2:
    case slrak_bvec2:
    case slrak_ivec2:
      cardinality = 2;
      break;
    case slrak_vec3:
    case slrak_bvec3:
    case slrak_ivec3:
      cardinality = 3;
      break;
    case slrak_vec4:
    case slrak_bvec4:
    case slrak_ivec4:
      cardinality = 4;
      break;
    case slrak_mat2:
      cardinality = 4;
      break;
    case slrak_mat3:
      cardinality = 9;
      break;
    case slrak_mat4:
      cardinality = 16;
      break;
    default:
      apilog(c, "glGetUniformfv(%u, %d /* failed to get location */, 0x%p {..});\n", program, location, params);
      return;
  }
  apilog(c, "glGetUniformfv(%u, %d, 0x%p { ", program, location, params);
  int k;
  for (k = 0; k < cardinality; ++k) {
    apilog(c, "%s%f", k ? ", " : "", params[k]);
  }
  apilog(c, " });\n");
}

void gl_es2_log_GetUniformiv(struct gl_es2_context *c, gl_es2_uint program, gl_es2_int location, gl_es2_int *params) { 
  /* Replicate a bunch of what the API does for logging purposes as we have to get to the type of the uniform
   * if we're to log it accurately. */
  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    apilog(c, "glGetUniformiv(%u /* invalid */, %d, ", program, location);
    if (params) {
      apilog(c, "0x%p {..});\n", params);
    }
    else {
      apilog(c, "NULL);\n");
    }
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    apilog(c, "glGetUniformiv(%u /* program not linked */, %d, ", program, location);
    if (params) {
      apilog(c, "0x%p {..});\n", params);
    }
    else {
      apilog(c, "NULL);\n");
    }
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, NULL, NULL);
  if (r) {
    apilog(c, "glGetUniformiv(%u, %d /* failed to get location */, ", program, location);
    if (params) {
      apilog(c, "0x%p {..});\n", params);
    }
    else {
      apilog(c, "NULL);\n");
    }
    return;
  }
  if (!params) {
    apilog(c, "glGetUniformiv(%u, %d, NULL);\n", program, location);
    return;
  }
  int cardinality;
  switch (slrak) {
    case slrak_float:
    case slrak_int:
    case slrak_bool:
    case slrak_sampler2D:
    case slrak_samplerCube:
      cardinality = 1;
      break;
    case slrak_vec2:
    case slrak_bvec2:
    case slrak_ivec2:
      cardinality = 2;
      break;
    case slrak_vec3:
    case slrak_bvec3:
    case slrak_ivec3:
      cardinality = 3;
      break;
    case slrak_vec4:
    case slrak_bvec4:
    case slrak_ivec4:
      cardinality = 4;
      break;
    case slrak_mat2:
      cardinality = 4;
      break;
    case slrak_mat3:
      cardinality = 9;
      break;
    case slrak_mat4:
      cardinality = 16;
      break;
    default:
      apilog(c, "glGetUniformiv(%u, %d /* failed to get location */, 0x%p {..});\n", program, location, params);
      return;
  }
  apilog(c, "glGetUniformiv(%u, %d, 0x%p { ", program, location, params);
  int k;
  for (k = 0; k < cardinality; ++k) {
    apilog(c, "%s%d", k ? ", " : "", params[k]);
  }
  apilog(c, " });\n");
}

void gl_es2_log_GetUniformLocation(struct gl_es2_context *c, gl_es2_uint program, const gl_es2_char *name, gl_es2_int uniform_location_returned) {
  if (name) {
    apilog(c, "glGetUniformLocation(%u, \"%s\") = %d;\n", program, name, uniform_location_returned);
  }
  else {
    apilog(c, "glGetUniformLocation(%u, NULL) = %d;\n", program, uniform_location_returned);
  }
}

static const char *vertex_attrib_pname(gl_es2_enum pname) {
  switch (pname) {
    case GL_ES2_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING:
      return "GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING";
    case GL_ES2_VERTEX_ATTRIB_ARRAY_ENABLED:
      return "GL_VERTEX_ATTRIB_ARRAY_ENABLED";
    case GL_ES2_VERTEX_ATTRIB_ARRAY_SIZE:
      return "GL_VERTEX_ATTRIB_ARRAY_SIZE";
    case GL_ES2_VERTEX_ATTRIB_ARRAY_STRIDE:
      return "GL_VERTEX_ATTRIB_ARRAY_STRIDE";
    case GL_ES2_VERTEX_ATTRIB_ARRAY_TYPE:
      return "GL_VERTEX_ATTRIB_ARRAY_TYPE";
    case GL_ES2_VERTEX_ATTRIB_ARRAY_NORMALIZED:
      return "GL_VERTEX_ATTRIB_ARRAY_NORMALIZED";
    case GL_ES2_CURRENT_VERTEX_ATTRIB:
      return "GL_CURRENT_VERTEX_ATTRIB";
    default:
      return NULL;
  }
}

void gl_es2_log_GetVertexAttribfv(struct gl_es2_context *c, gl_es2_uint index, gl_es2_enum pname, gl_es2_float *params) { 
  const char *mpname = vertex_attrib_pname(pname);
  if (mpname) {
    apilog(c, "glGetVertexAttribfv(%u, %s, ", index, mpname);
  }
  else {
    apilog(c, "glGetVertexAttribfv(%u, 0x%04X, ", index, pname);
  }
  if (params && (pname == GL_ES2_CURRENT_VERTEX_ATTRIB)) {
    apilog(c, "{ %f, %f, %f, %f });\n", params[0], params[1], params[2], params[3]);
  }
  else if (params) {
    apilog(c, "{ %f });\n", *params);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_GetVertexAttribiv(struct gl_es2_context *c, gl_es2_uint index, gl_es2_enum pname, gl_es2_int *params) { 
  const char *mpname = vertex_attrib_pname(pname);
  if (mpname) {
    apilog(c, "glGetVertexAttribiv(%u, %s, ", index, mpname);
  }
  else {
    apilog(c, "glGetVertexAttribiv(%u, 0x%04X, ", index, pname);
  }
  if (params && (pname == GL_ES2_CURRENT_VERTEX_ATTRIB)) {
    apilog(c, "{ %d, %d, %d, %d });\n", params[0], params[1], params[2], params[3]);
  }
  else if (params) {
    apilog(c, "{ %d });\n", *params);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_GetVertexAttribPointerv(struct gl_es2_context *c, gl_es2_uint index, gl_es2_enum pname, void **pointer) { 
  if (pname == GL_ES2_VERTEX_ATTRIB_ARRAY_POINTER) {
    apilog(c, "glGetVertexAttribPointerv(%u, GL_VERTEX_ATTRIB_ARRAY_POINTER, ", index);
  }
  else {
    apilog(c, "glGetVertexAttribPointerv(%u, 0x%04X, ", index, pname);
  }
  if (pointer) {
    if (*pointer) {
      apilog(c, "{ 0x%p });\n", *pointer);
    }
    else {
      apilog(c, "{ NULL });\n");
    }
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_Hint(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum mode) {
  if (target == GL_ES2_GENERATE_MIPMAP_HINT) {
    switch (mode) {
      case GL_ES2_FASTEST: apilog(c, "glHint(GL_GENERATE_MIPMAP_HINT, GL_FASTEST);\n"); return;
      case GL_ES2_NICEST:  apilog(c, "glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);\n"); return;
      case GL_ES2_DONT_CARE: apilog(c, "glHint(GL_GENERATE_MIPMAP_HINT, GL_DONT_CARE);\n"); return;
      default: apilog(c, "glHint(GL_GENERATE_MIPMAP_HINT, 0x%04X);\n", mode); return;
    }
  }
  else {
    apilog(c, "glHint(0x%04X, 0x%04X);\n", target, mode);
  }
}

static void print_bool_semicolon_lf(struct gl_es2_context *c, gl_es2_boolean b) {
  if (b == GL_ES2_TRUE) apilog(c, "GL_TRUE;\n");
  else if (b == GL_ES2_FALSE) apilog(c, "GL_FALSE;\n");
  else apilog(c, "0x%X;\n", b);
}

void gl_es2_log_IsBuffer(struct gl_es2_context *c, gl_es2_uint buffer, gl_es2_boolean is_it_a_buffer) {
  apilog(c, "glIsBuffer(%u) = ", buffer);
  print_bool_semicolon_lf(c, is_it_a_buffer);
}

static const char *enabled_cap(gl_es2_enum cap) {
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
    default: return NULL;
  }
}

void gl_es2_log_IsEnabled(struct gl_es2_context *c, gl_es2_enum cap, gl_es2_boolean is_it_enabled) {
  const char *mcap = enabled_cap(cap);
  if (mcap) {
    apilog(c, "glIsEnabled(%s) = ", mcap);
  }
  else {
    apilog(c, "glIsEnabled(0x%04X) = ", cap);
  }
  print_bool_semicolon_lf(c, is_it_enabled);
}

void gl_es2_log_IsFramebuffer(struct gl_es2_context *c, gl_es2_uint framebuffer, gl_es2_boolean is_it_a_framebuffer) {
  apilog(c, "glIsFramebuffer(%u) = ", framebuffer);
  print_bool_semicolon_lf(c, is_it_a_framebuffer);
}

void gl_es2_log_IsProgram(struct gl_es2_context *c, gl_es2_uint program, gl_es2_boolean is_it_a_program) {
  apilog(c, "glIsProgram(%u) = ", program);
  print_bool_semicolon_lf(c, is_it_a_program);
}

void gl_es2_log_IsRenderbuffer(struct gl_es2_context *c, gl_es2_uint renderbuffer, gl_es2_boolean is_it_a_renderbuffer) {
  apilog(c, "glIsRenderbuffer(%u) = ", renderbuffer);
  print_bool_semicolon_lf(c, is_it_a_renderbuffer);
}

void gl_es2_log_IsShader(struct gl_es2_context *c, gl_es2_uint shader, gl_es2_boolean is_it_a_shader) {
  apilog(c, "glIsShader(%u) = ", shader);
  print_bool_semicolon_lf(c, is_it_a_shader);
}

void gl_es2_log_IsTexture(struct gl_es2_context *c, gl_es2_uint texture, gl_es2_boolean is_it_a_texture) {
  apilog(c, "glIsTexture(%u) = ", texture);
  print_bool_semicolon_lf(c, is_it_a_texture);
}

void gl_es2_log_LineWidth(struct gl_es2_context *c, gl_es2_float width) { 
  apilog(c, "glLineWidth(%f);\n", width);
}

void gl_es2_log_LinkProgram(struct gl_es2_context *c, gl_es2_uint program) { 
  apilog(c, "glLinkProgram(%u);\n", program);
}

void gl_es2_log_PixelStorei(struct gl_es2_context *c, gl_es2_enum pname, gl_es2_int param) { 
  switch (pname) {
    case GL_ES2_PACK_ALIGNMENT:
      apilog(c, "glPixelStorei(GL_PACK_ALIGNMENT, %d);\n", param);
      break;
    case GL_ES2_UNPACK_ALIGNMENT:
      apilog(c, "glPixelStorei(GL_UNPACK_ALIGNMENT, %d);\n", param);
      break;
    default:
      apilog(c, "glPixelStorei(0x%04X, %d);\n", pname, param);
      break;
  }
}

void gl_es2_log_PolygonOffset(struct gl_es2_context *c, gl_es2_float factor, gl_es2_float units) {
  apilog(c, "glPolygonOffset(%f, %f);\n", factor, units);
}

void gl_es2_log_ReadPixels(struct gl_es2_context *c, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height, gl_es2_enum format, gl_es2_enum type, void *pixels) { 
  apilog(c, "glReadPixels(%d, %d, %d, %d, ", x, y, width, height);
  switch (format) {
    case GL_ES2_ALPHA: apilog(c, "GL_ALPHA"); break;
    case GL_ES2_RGB: apilog(c, "GL_RGB"); break;
    case GL_ES2_RGBA: apilog(c, "GL_RGBA"); break;
    case GL_ES2_DEPTH_COMPONENT: apilog(c, "GL_DEPTH_COMPONENT /* non-standard Aex-GL extension */"); break;
    default:
      apilog(c, "0x%04X", format);
      break;
  }
  const char *mtype = tex_type(type);
  if (mtype) {
    apilog(c, ", %s, ", mtype);
  }
  else {
    apilog(c, ", 0x%04X, ", type);
  }
  if (pixels) {
    apilog(c, "0x%p);\n", pixels);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_ReleaseShaderCompiler(struct gl_es2_context *c) { 
  apilog(c, "glReleaseShaderCompiler();\n");
}

void gl_es2_log_RenderbufferStorage(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum internalformat, gl_es2_sizei width, gl_es2_sizei height) { 
  if (target == GL_ES2_RENDERBUFFER) {
    apilog(c, "glRenderbufferStorage(GL_RENDERBUFFER, ");
  }
  else {
    apilog(c, "glRenderbufferStorage(0x%04X, ", target);
  }
  switch (internalformat) {
    case GL_ES2_RGBA4: apilog(c, "GL_RGBA4, "); break;
    case GL_ES2_RGBA8: apilog(c, "GL_RGBA8, "); break;
    case GL_ES2_RGB565: apilog(c, "GL_RGB565, "); break;
    case GL_ES2_RGBA5_A1: apilog(c, "GL_RGB5_A1, "); break;
    case GL_ES2_DEPTH_COMPONENT16: apilog(c, "GL_DEPTH_COMPONENT16, "); break;
    case GL_ES2_DEPTH_COMPONENT32: apilog(c, "GL_OES_depth32, "); break;
    case GL_ES2_STENCIL_INDEX8: apilog(c, "GL_STENCIL_INDEX8, "); break;
    case GL_ES2_STENCIL_INDEX16: apilog(c, "GL_STENCIL_INDEX16, "); break;
    default:
      apilog(c, "0x%04X, ", internalformat);
      break;
  }
  apilog(c, "%d, %d);\n", width, height);
}

void gl_es2_log_SampleCoverage(struct gl_es2_context *c, gl_es2_float value, gl_es2_boolean invert) { 
  if (invert == GL_ES2_TRUE) {
    apilog(c, "glSampleCoverage(%f, GL_TRUE);\n", value);
  }
  else if (invert == GL_ES2_FALSE) {
    apilog(c, "glSampleCoverage(%f, GL_FALSE);\n", value);
  }
  else {
    apilog(c, "glSampleCoverage(%f, 0x%X);\n", value, invert);
  }
}

void gl_es2_log_Scissor(struct gl_es2_context *c, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height) { 
  apilog(c, "glScissor(%d, %d, %d, %d);\n", x, y, width, height);
}

void gl_es2_log_ShaderBinary(struct gl_es2_context *c, gl_es2_sizei count, const gl_es2_uint *shaders, gl_es2_enum binaryformat, const void *binary, gl_es2_sizei length) { 
  if (shaders) {
    apilog(c, "glShaderBinary(%d, { ");
    int n;
    for (n = 0; n < count; ++n) {
      apilog(c, "%s%u", n ? ", " : "", shaders[n]);
    }
    apilog(c, " }, 0x%04X, ", binaryformat);
  }
  else {
    apilog(c, "glShaderBinary(%d, NULL, 0x%04X, ", count, binaryformat);
  }

  if (binary) {
    apilog(c, "0x%p, %d);\n", binary, length);
  }
  else {
    apilog(c, "NULL, %d);\n", length);
  }
}

void gl_es2_log_ShaderSource(struct gl_es2_context *c, gl_es2_uint shader, gl_es2_sizei count, const gl_es2_char *const *string, const gl_es2_int *length) { 
  apilog(c, "glShaderSource(%u, %d, ", shader, count);
  if (string) {
    int n;
    apilog(c, "{ ");
    for (n = 0; n < count; ++n) {
      if (string[n]) {
        /* shader source is too large to appear in a log file so skip doing that and just log the pointer */
        apilog(c, "%s0x%p", n ? ", " : "", string[n]);
      }
      else {
        apilog(c, "%sNULL", n ? ", " : "");
      }
    }
    apilog(c, " }, ");
  }
  if (length) {
    apilog(c, "{ %d });\n", *length);
  }
  else {
    apilog(c, "NULL);\n");
  }
}

static const char *stencil_func(gl_es2_enum func) {
  switch (func) {
    case GL_ES2_NEVER: return "GL_NEVER";
    case GL_ES2_LESS: return "GL_LESS";
    case GL_ES2_LEQUAL: return "GL_LEQUAL";
    case GL_ES2_GREATER: return "GL_GREATER";
    case GL_ES2_GEQUAL: return "GL_GEQUAL";
    case GL_ES2_EQUAL: return "GL_EQUAL";
    case GL_ES2_NOTEQUAL: return "GL_NOTEQUAL";
    case GL_ES2_ALWAYS: return "GL_ALWAYS";
    default: return NULL;
  }
}

void gl_es2_log_StencilFunc(struct gl_es2_context *c, gl_es2_enum func, gl_es2_int ref, gl_es2_uint mask) { 
  const char *mfunc = stencil_func(func);
  if (mfunc) {
    apilog(c, "glStencilFunc(%s, 0x%04X, 0x%04X);\n", mfunc, ref, mask);
  }
  else {
    apilog(c, "glStencilFunc(0x%04X, 0x%04X, 0x%04X);\n", func, ref, mask);
  }
}

void gl_es2_log_StencilFuncSeparate(struct gl_es2_context *c, gl_es2_enum face, gl_es2_enum func, gl_es2_int ref, gl_es2_uint mask) { 
  const char *mface = cull_face_mode(face);
  if (mface) {
    apilog(c, "glStencilFuncSeparate(%s, ", mface);
  }
  else {
    apilog(c, "glStencilFuncSeparate(0x%04X, ", face);
  }
  const char *mfunc = stencil_func(func);
  if (mfunc) {
    apilog(c, "%s, 0x%04X, 0x%04X);\n", mfunc, ref, mask);
  }
  else {
    apilog(c, "0x%04X, 0x%04X, 0x%04X);\n", func, ref, mask);
  }
}

void gl_es2_log_StencilMask(struct gl_es2_context *c, gl_es2_uint mask) { 
  apilog(c, "glStencilMask(0x%04X);\n", mask);
}

void gl_es2_log_StencilMaskSeparate(struct gl_es2_context *c, gl_es2_enum face, gl_es2_uint mask) { 
  const char *mface = cull_face_mode(face);
  if (mface) {
    apilog(c, "glStencilMaskSeparate(%s, 0x%04X);\n", mface, mask);
  }
  else {
    apilog(c, "glStencilMaskSeparate(0x%04X, 0x%04X);\n", face, mask);
  }
}

static const char *stencil_op(gl_es2_enum op) {
  switch (op) {
    case GL_ES2_KEEP: return "GL_KEEP";
    case GL_ES2_ZERO: return "GL_ZERO";
    case GL_ES2_REPLACE: return "GL_REPLACE";
    case GL_ES2_INCR: return "GL_INCR";
    case GL_ES2_INCR_WRAP: return "GL_INCR_WRAP";
    case GL_ES2_DECR: return "GL_DECR";
    case GL_ES2_DECR_WRAP: return "GL_DECR_WRAP";
    case GL_ES2_INVERT: return "GL_INVERT";
    default:
      return NULL;
  }
}

void gl_es2_log_StencilOp(struct gl_es2_context *c, gl_es2_enum sfail, gl_es2_enum zfail, gl_es2_enum zpass) { 
  const char *msfail = stencil_op(sfail);
  const char *mzfail = stencil_op(zfail);
  const char *mzpass = stencil_op(zpass);
  if (msfail) {
    apilog(c, "glStencilOp(%s, ", msfail);
  }
  else {
    apilog(c, "glStencilOp(0x%04X, ", sfail);
  }
  if (mzfail) {
    apilog(c, "%s, ", mzfail);
  }
  else {
    apilog(c, "0x%04X, ", zfail);
  }
  if (mzpass) {
    apilog(c, "%s);\n", mzpass);
  }
  else {
    apilog(c, "0x%04X);\n", zpass);
  }
}

void gl_es2_log_StencilOpSeparate(struct gl_es2_context *c, gl_es2_enum face, gl_es2_enum sfail, gl_es2_enum zfail, gl_es2_enum zpass) { 
  const char *msfail = stencil_op(sfail);
  const char *mzfail = stencil_op(zfail);
  const char *mzpass = stencil_op(zpass);
  const char *mface = cull_face_mode(face);
  if (mface) {
    apilog(c, "glStencilOpSeparate(%s, ", mface);
  }
  else {
    apilog(c, "glStencilOpSeparate(0x%04X, ", face);
  }
  if (msfail) {
    apilog(c, "%s, ", msfail);
  }
  else {
    apilog(c, "0x%04X, ", sfail);
  }
  if (mzfail) {
    apilog(c, "%s, ", mzfail);
  }
  else {
    apilog(c, "0x%04X, ", zfail);
  }
  if (mzpass) {
    apilog(c, "%s);\n", mzpass);
  }
  else {
    apilog(c, "0x%04X);\n", zpass);
  }
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

static const char *tp_tex_target(gl_es2_enum tgt) {
  switch (tgt) {
    case GL_ES2_TEXTURE_2D: return "GL_TEXTURE_2D";
    case GL_ES2_TEXTURE_CUBE_MAP: return "GL_TEXTURE_CUBE_MAP";
    default: return NULL;
  }
}

static const char *tp_pname(gl_es2_enum pname) {
  switch (pname) {
    case GL_ES2_TEXTURE_MIN_FILTER: return "GL_TEXTURE_MIN_FILTER";
    case GL_ES2_TEXTURE_MAG_FILTER: return "GL_TEXTURE_MAG_FILTER";
    case GL_ES2_TEXTURE_WRAP_S: return "GL_TEXTURE_WRAP_S";
    case GL_ES2_TEXTURE_WRAP_T: return "GL_TEXTURE_WRAP_T";
    default: return NULL;
  }
}

void gl_es2_log_TexParameterf(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, gl_es2_float param) { 
  const char *mtgt = tp_tex_target(target);
  const char *mpname = tp_pname(pname);
  if (mtgt) {
    apilog(c, "glTexParameterf(%s, ");
  }
  else {
    apilog(c, "0x%04X, ", target);
  }
  if (mpname) {
    apilog(c, "%s, ", mpname);
  }
  else {
    apilog(c, "0x%04X, ", pname);
  }
  if (pname == GL_ES2_TEXTURE_MIN_FILTER) {
    if (param == (float)GL_ES2_NEAREST) apilog(c, "GL_NEAREST");
    else if (param == (float)GL_ES2_LINEAR) apilog(c, "GL_LINEAR");
    else if (param == (float)GL_ES2_NEAREST_MIPMAP_NEAREST) apilog(c, "GL_NEAREST_MIPMAP_NEAREST");
    else if (param == (float)GL_ES2_LINEAR_MIPMAP_NEAREST) apilog(c, "GL_LINEAR_MIPMAP_NEAREST");
    else if (param == (float)GL_ES2_LINEAR_MIPMAP_LINEAR) apilog(c, "GL_LINEAR_MIPMAP_LINEAR");
    else apilog(c, "%f", param);
  }
  else if (pname == GL_ES2_TEXTURE_MAG_FILTER) {
    if (param == (float)GL_ES2_NEAREST) apilog(c, "GL_NEAREST");
    else if (param == (float)GL_ES2_LINEAR) apilog(c, "GL_LINEAR");
    else apilog(c, "%f", param);
  }
  else if ((pname == GL_ES2_TEXTURE_WRAP_S) || (pname == GL_ES2_TEXTURE_WRAP_T)) {
    if (param == (float)GL_ES2_CLAMP_TO_EDGE) apilog(c, "GL_CLAMP_TO_EDGE");
    else if (param == (float)GL_ES2_MIRRORED_REPEAT) apilog(c, "GL_MIRRORED_REPEAT");
    else if (param == (float)GL_ES2_REPEAT) apilog(c, "GL_REPEAT");
    else apilog(c, "%f", param);
  }
  else {
    apilog(c, "%f", param);
  }
  apilog(c, ");\n");
}

void gl_es2_log_TexParameterfv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, const gl_es2_float *params) { 
  const char *mtgt = tp_tex_target(target);
  const char *mpname = tp_pname(pname);
  if (mtgt) {
    apilog(c, "glTexParameterfv(%s, ");
  }
  else {
    apilog(c, "0x%04X, ", target);
  }
  if (mpname) {
    apilog(c, "%s, ", mpname);
  }
  else {
    apilog(c, "0x%04X, ", pname);
  }
  if (pname == GL_ES2_TEXTURE_MIN_FILTER) {
    if ((*params) == (float)GL_ES2_NEAREST) apilog(c, "{ GL_NEAREST }");
    else if ((*params) == (float)GL_ES2_LINEAR) apilog(c, "{ GL_LINEAR }");
    else if ((*params) == (float)GL_ES2_NEAREST_MIPMAP_NEAREST) apilog(c, "{ GL_NEAREST_MIPMAP_NEAREST }");
    else if ((*params) == (float)GL_ES2_LINEAR_MIPMAP_NEAREST) apilog(c, "{ GL_LINEAR_MIPMAP_NEAREST }");
    else if ((*params) == (float)GL_ES2_LINEAR_MIPMAP_LINEAR) apilog(c, "{ GL_LINEAR_MIPMAP_LINEAR }");
    else apilog(c, "{ %f }", *params);
  }
  else if (pname == GL_ES2_TEXTURE_MAG_FILTER) {
    if ((*params) == (float)GL_ES2_NEAREST) apilog(c, "{ GL_NEAREST }");
    else if ((*params) == (float)GL_ES2_LINEAR) apilog(c, "{ GL_LINEAR }");
    else apilog(c, "{ %f }", *params);
  }
  else if ((pname == GL_ES2_TEXTURE_WRAP_S) || (pname == GL_ES2_TEXTURE_WRAP_T)) {
    if ((*params) == (float)GL_ES2_CLAMP_TO_EDGE) apilog(c, "{ GL_CLAMP_TO_EDGE }");
    else if ((*params) == (float)GL_ES2_MIRRORED_REPEAT) apilog(c, "{ GL_MIRRORED_REPEAT }");
    else if ((*params) == (float)GL_ES2_REPEAT) apilog(c, "{ GL_REPEAT }");
    else apilog(c, "{ %f }", *params);
  }
  else {
    apilog(c, "{ %f }", *params);
  }
  apilog(c, ");\n");
}

void gl_es2_log_TexParameteri(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, gl_es2_int param) { 
  const char *mtgt = tp_tex_target(target);
  const char *mpname = tp_pname(pname);
  if (mtgt) {
    apilog(c, "glTexParameteri(%s, ");
  }
  else {
    apilog(c, "0x%04X, ", target);
  }
  if (mpname) {
    apilog(c, "%s, ", mpname);
  }
  else {
    apilog(c, "0x%04X, ", pname);
  }
  if (pname == GL_ES2_TEXTURE_MIN_FILTER) {
    if (param == GL_ES2_NEAREST) apilog(c, "GL_NEAREST");
    else if (param == GL_ES2_LINEAR) apilog(c, "GL_LINEAR");
    else if (param == GL_ES2_NEAREST_MIPMAP_NEAREST) apilog(c, "GL_NEAREST_MIPMAP_NEAREST");
    else if (param == GL_ES2_LINEAR_MIPMAP_NEAREST) apilog(c, "GL_LINEAR_MIPMAP_NEAREST");
    else if (param == GL_ES2_LINEAR_MIPMAP_LINEAR) apilog(c, "GL_LINEAR_MIPMAP_LINEAR");
    else apilog(c, "0x%04X", param);
  }
  else if (pname == GL_ES2_TEXTURE_MAG_FILTER) {
    if (param == GL_ES2_NEAREST) apilog(c, "GL_NEAREST");
    else if (param == GL_ES2_LINEAR) apilog(c, "GL_LINEAR");
    else apilog(c, "0x%04X", param);
  }
  else if ((pname == GL_ES2_TEXTURE_WRAP_S) || (pname == GL_ES2_TEXTURE_WRAP_T)) {
    if (param == GL_ES2_CLAMP_TO_EDGE) apilog(c, "GL_CLAMP_TO_EDGE");
    else if (param == GL_ES2_MIRRORED_REPEAT) apilog(c, "GL_MIRRORED_REPEAT");
    else if (param == GL_ES2_REPEAT) apilog(c, "GL_REPEAT");
    else apilog(c, "0x%04X", param);
  }
  else {
    apilog(c, "%d", param);
  }
  apilog(c, ");\n");
}

void gl_es2_log_TexParameteriv(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum pname, const gl_es2_int *params) { 
  const char *mtgt = tp_tex_target(target);
  const char *mpname = tp_pname(pname);
  if (mtgt) {
    apilog(c, "glTexParameteriv(%s, ");
  }
  else {
    apilog(c, "0x%04X, ", target);
  }
  if (mpname) {
    apilog(c, "%s, ", mpname);
  }
  else {
    apilog(c, "0x%04X, ", pname);
  }
  if (pname == GL_ES2_TEXTURE_MIN_FILTER) {
    if ((*params) == GL_ES2_NEAREST) apilog(c, "{ GL_NEAREST }");
    else if ((*params) == GL_ES2_LINEAR) apilog(c, "{ GL_LINEAR }");
    else if ((*params) == GL_ES2_NEAREST_MIPMAP_NEAREST) apilog(c, "{ GL_NEAREST_MIPMAP_NEAREST }");
    else if ((*params) == GL_ES2_LINEAR_MIPMAP_NEAREST) apilog(c, "{ GL_LINEAR_MIPMAP_NEAREST }");
    else if ((*params) == GL_ES2_LINEAR_MIPMAP_LINEAR) apilog(c, "{ GL_LINEAR_MIPMAP_LINEAR }");
    else apilog(c, "{ 0x%04X }", *params);
  }
  else if (pname == GL_ES2_TEXTURE_MAG_FILTER) {
    if ((*params) == GL_ES2_NEAREST) apilog(c, "{ GL_NEAREST }");
    else if ((*params) == GL_ES2_LINEAR) apilog(c, "{ GL_LINEAR }");
    else apilog(c, "{ 0x%04X }", *params);
  }
  else if ((pname == GL_ES2_TEXTURE_WRAP_S) || (pname == GL_ES2_TEXTURE_WRAP_T)) {
    if ((*params) == GL_ES2_CLAMP_TO_EDGE) apilog(c, "{ GL_CLAMP_TO_EDGE }");
    else if ((*params) == GL_ES2_MIRRORED_REPEAT) apilog(c, "{ GL_MIRRORED_REPEAT }");
    else if ((*params) == GL_ES2_REPEAT) apilog(c, "{ GL_REPEAT }");
    else apilog(c, "{ 0x%04X }", *params);
  }
  else {
    apilog(c, "{ 0x%04X }", *params);
  }
  apilog(c, ");\n");
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
  apilog(c, "glUniform1f(%d, %f);\n", location, v0);
}

void gl_es2_log_Uniform1fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) { 
  int n;
  apilog(c, "glUniform1fv(%d, %d, ", location, count);
  if (value) {
    apilog(c, "{ ");
    for (n = 0; n < count; ++n) {
      apilog(c, "%s%f", n ? ", " : "", value[n]);
    }
    apilog(c, " });\n");
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_Uniform1i(struct gl_es2_context *c, gl_es2_int location, gl_es2_int v0) { 
  apilog(c, "glUniform1i(%d, %d);\n", location, v0);
}

void gl_es2_log_Uniform1iv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) { 
  int n;
  apilog(c, "glUniform1iv(%d, %d, ", location, count);
  if (value) {
    apilog(c, "{ ");
    for (n = 0; n < count; ++n) {
      apilog(c, "%s%d", n ? ", " : "", value[n]);
    }
    apilog(c, " });\n");
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_Uniform2f(struct gl_es2_context *c, gl_es2_int location, gl_es2_float v0, gl_es2_float v1) { 
  apilog(c, "glUniform2f(%d, %f, %f);\n", location, v0, v1);
}

void gl_es2_log_Uniform2fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) { 
  int n;
  apilog(c, "glUniform2fv(%d, %d, ", location, count);
  if (value) {
    apilog(c, "{ ");
    for (n = 0; n < count; ++n) {
      apilog(c, "%s%f, %f", n ? ",  " : "", value[n * 2 + 0], value[n * 2 + 1]);
    }
    apilog(c, " });\n");
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_Uniform2i(struct gl_es2_context *c, gl_es2_int location, gl_es2_int v0, gl_es2_int v1) { 
  apilog(c, "glUniform2i(%d, %d, %d);\n", location, v0, v1);
}

void gl_es2_log_Uniform2iv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) { 
  int n;
  apilog(c, "glUniform2iv(%d, %d, ", location, count);
  if (value) {
    apilog(c, "{ ");
    for (n = 0; n < count; ++n) {
      apilog(c, "%s%d, %d", n ? ",  " : "", value[n * 2 + 0], value[n * 2 + 1]);
    }
    apilog(c, " });\n");
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_Uniform3f(struct gl_es2_context *c, gl_es2_int location, gl_es2_float v0, gl_es2_float v1, gl_es2_float v2) { 
  apilog(c, "glUniform2f(%d, %f, %f, %f);\n", location, v0, v1, v2);
}

void gl_es2_log_Uniform3fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) { 
  int n;
  apilog(c, "glUniform3fv(%d, %d, ", location, count);
  if (value) {
    apilog(c, "{ ");
    for (n = 0; n < count; ++n) {
      apilog(c, "%s%f, %f, %f", n ? ",  " : "", value[n * 3 + 0], value[n * 3 + 1], value[n * 3 + 2]);
    }
    apilog(c, " });\n");
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_Uniform3i(struct gl_es2_context *c, gl_es2_int location, gl_es2_int v0, gl_es2_int v1, gl_es2_int v2) { 
  apilog(c, "glUniform3i(%d, %d, %d, %d);\n", location, v0, v1, v2);
}

void gl_es2_log_Uniform3iv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) { 
  int n;
  apilog(c, "glUniform3iv(%d, %d, ", location, count);
  if (value) {
    apilog(c, "{ ");
    for (n = 0; n < count; ++n) {
      apilog(c, "%s%d, %d, %d", n ? ",  " : "", value[n * 3 + 0], value[n * 3 + 1], value[n * 3 + 2]);
    }
    apilog(c, " });\n");
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_Uniform4f(struct gl_es2_context *c, gl_es2_int location, gl_es2_float v0, gl_es2_float v1, gl_es2_float v2, gl_es2_float v3) { 
  apilog(c, "glUniform4f(%d, %f, %f, %f, %f);\n", location, v0, v1, v2, v3);
}

void gl_es2_log_Uniform4fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) { 
  int n;
  apilog(c, "glUniform4fv(%d, %d, ", location, count);
  if (value) {
    apilog(c, "{ ");
    for (n = 0; n < count; ++n) {
      apilog(c, "%s%f, %f, %f, %f", n ? ",  " : "", value[n * 4 + 0], value[n * 4 + 1], value[n * 4 + 2], value[n * 4 + 3]);
    }
    apilog(c, " });\n");
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_Uniform4i(struct gl_es2_context *c, gl_es2_int location, gl_es2_int v0, gl_es2_int v1, gl_es2_int v2, gl_es2_int v3) { 
  apilog(c, "glUniform4i(%d, %d, %d, %d, %d);\n", location, v0, v1, v2, v3);
}

void gl_es2_log_Uniform4iv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) { 
  int n;
  apilog(c, "glUniform4iv(%d, %d, ", location, count);
  if (value) {
    apilog(c, "{ ");
    for (n = 0; n < count; ++n) {
      apilog(c, "%s%d, %d, %d, %d", n ? ",  " : "", value[n * 4 + 0], value[n * 4 + 1], value[n * 4 + 2], value[n * 4 + 3]);
    }
    apilog(c, " });\n");
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_UniformMatrix2fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value) { 
  int n;
  apilog(c, "glUniform4fv(%d, %d, ", location, count);
  if (value) {
    apilog(c, "{ ");
    for (n = 0; n < count; ++n) {
      apilog(c, "%s%f, %f, %f, %f", n ? ",  " : "", value[n * 4 + 0], value[n * 4 + 1], value[n * 4 + 2], value[n * 4 + 3]);
    }
    apilog(c, " });\n");
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_UniformMatrix3fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value) { 
  int n;
  apilog(c, "glUniform4fv(%d, %d, ", location, count);
  if (value) {
    apilog(c, "{ ");
    for (n = 0; n < count; ++n) {
      int k;
      for (k = 0; k < 9; ++k) {
        apilog(c, "%s%f", (n+k) ? (k ? ", " : ",  ") : "", value[9 * n + k]);
      }
    }
    apilog(c, " });\n");
  }
  else {
    apilog(c, "NULL);\n");
  }
}

void gl_es2_log_UniformMatrix4fv(struct gl_es2_context *c, gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value) { 
  int n;
  apilog(c, "glUniform4fv(%d, %d, ", location, count);
  if (value) {
    apilog(c, "{ ");
    for (n = 0; n < count; ++n) {
      int k;
      for (k = 0; k < 16; ++k) {
        apilog(c, "%s%f", (n+k) ? (k ? ", " : ",  ") : "", value[16 * n + k]);
      }
    }
    apilog(c, " });\n");
  }
  else {
    apilog(c, "NULL);\n");
  }
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



