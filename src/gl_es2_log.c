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

static void log(struct gl_es2_context *c, const char *fmt, ...) {
  va_list args;
  if (!c->log_file_) return;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}

void gl_es2_log_ActiveTexture(struct gl_es2_context *c, gl_es2_enum texture) {
  if ((texture >= GL_ES2_TEXTURE0) && (texture < GL_ES2_MAX_COMBINED_TEXTURE_IMAGE_UNITS)) {
    log(c, "glActiveTexture(GL_ES2_TEXTURE%d);\n", (int)(texture - GL_ES2_TEXTURE0));
  }
  else {
    log(c, "glActiveTexture(0x%04X);\n", texture);
  }
}

void gl_es2_log_AttachShader(struct gl_es2_context *c, gl_es2_uint program, gl_es2_uint shader) {
  log(c, "glAttachShader(%d, %d);\n", program, shader);
}

void gl_es2_log_BindAttribLocation(struct gl_es2_context *c, gl_es2_uint program, gl_es2_uint index, const gl_es2_char *name) { 
  log(c, "glBindAttribLocation(%d, %d, %s);\n", program, index, name ? name : "NULL");
}

void gl_es2_log_BindBuffer(struct gl_es2_context *c, gl_es2_enum target, gl_es2_uint buffer) { 
  log(c, "glBindBuffer(%d, %d);\n", target, buffer);
}

void gl_es2_log_BindFramebuffer(struct gl_es2_context *c, gl_es2_enum target, gl_es2_uint framebuffer) { 
  log(c, "glBindFramebuffer(%d, %d);\n", target, framebuffer);
}

void gl_es2_log_BindRenderbuffer(struct gl_es2_context *c, gl_es2_enum target, gl_es2_uint renderbuffer) { 
  log(c, "glBindRenderbuffer(%d, %d);\n", target, renderbuffer);
}

void gl_es2_log_BindTexture(struct gl_es2_context *c, gl_es2_enum target, gl_es2_uint texture) { 
  log(c, "glBindTexture(%d, %d);\n", target, texture);
}

void gl_es2_log_BlendColor(struct gl_es2_context *c, gl_es2_float red, gl_es2_float green, gl_es2_float blue, gl_es2_float alpha) { 
  log(c, "glBlendColor(%f, %f, %f, %f);\n", red, green, blue, alpha);
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
    log(c, "glBlendEquation(%s);\n", m);
  }
  else {
    log(c, "glBlendEquation(0x%04X);\n", mode);
  }
}

void gl_es2_log_BlendEquationSeparate(struct gl_es2_context *c, gl_es2_enum modeRGB, gl_es2_enum modeAlpha) { 
  const char *mrgb = blend_eq_mode(modeRGB);
  const char *malpha = blend_eq_mode(modeAlpha);
  if (mrgb) {
    log(c, "glBlendEquationSeparate(%s, ", mrgb);
  }
  else {
    log(c, "glBlendEquationSeperate(0x%04X, ", modeRGB);
  }
  if (malpha) {
    log(c, "%s);\n", malpha);
  }
  else {
    log(c, "0x%04X);\n", malpha);
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
    log(c, "glBlendFunc(%s, ", msfactor);
  }
  else {
    log(c, "glBlendFunc(0x%04X, ", sfactor);
  }
  if (mdfactor) {
    log(c, "%s);\n", mdfactor);
  }
  else {
    log(c, "0x%04X);\n", dfactor);
  }
}

void gl_es2_log_BlendFuncSeparate(struct gl_es2_context *c, gl_es2_enum sfactorRGB, gl_es2_enum dfactorRGB, gl_es2_enum sfactorAlpha, gl_es2_enum dfactorAlpha) { 
  const char *msfactorrgb = blend_func(sfactorRGB), *mdfactorrgb = blend_func(dfactorRGB);
  const char *msfactoralpha = blend_func(sfactorAlpha), *mdfactoralpha = blend_func(dfactorAlpha);
  if (msfactorrgb) {
    log(c, "glBlendFunc(%s, ", msfactorrgb);
  }
  else {
    log(c, "glBlendFunc(0x%04X, ", sfactorRGB);
  }
  if (mdfactorrgb) {
    log(c, "%s, ", mdfactorrgb);
  }
  else {
    log(c, "0x%04X, ", dfactorRGB);
  }
  if (msfactoralpha) {
    log(c, "%s, ", msfactoralpha);
  }
  else {
    log(c, "0x%04X, ", sfactorAlpha);
  }
  if (mdfactoralpha) {
    log(c, "%s);\n", mdfactoralpha);
  }
  else {
    log(c, "0x%04X);\n", dfactorAlpha);
  }
}

static const char *buffer_target(gl_es2_enum target) {
  switch (target) {
    case GL_ES2_ARRAY_BUFFER: return "GL_ARRAY_BUFFER";
    case GL_ES2_ELEMENT_ARRAY_BUFFER: return "GL_ELEMENT_ARRAY_BUFFER";
    default: return NULL;
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
    log(c, "glBufferData(%s, %zu, %p, ", mtarget, size, data);
  }
  else {
    log(c, "glBufferData(0x%04X, %zu, %p, ", target, size, data);
  }
  if (musage) {
    log(c, "%s);\n", musage);
  }
  else {
    log(c, "0x%04X);\n", usage);
  }
}

void gl_es2_log_BufferSubData(struct gl_es2_context *c, gl_es2_enum target, gl_es2_intptr offset, gl_es2_sizeiptr size, const void *data) { 
  const char *mtarget = buffer_target(target);
  if (mtarget) {
    log(c, "glBufferSubData(%s, 0x%" PRIx64 ", %zu, %p, ", mtarget, (uint64_t)offset, size, data);
  }
  else {
    log(c, "glBufferSubData(0x%04X, 0x%" PRIx64 ", %zu, %p, ", target, (uint64_t)offset, size, data);
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
    log(c, "glCheckFramebufferStatus(GL_FRAMEBUFFER) = ");
  }
  else {
    log(c, "glCheckFramebufferStatus(0x%04X) = ", target);
  }
  if (mstatus) {
    log(c, "%s);\n", mstatus);
  }
  else {
    log(c, "0x%04X);\n", framebuffer_status_assessed);
  }
}

void gl_es2_log_Clear(struct gl_es2_context *c, gl_es2_bitfield mask) {
  int have_any_flag = 0;
  log(c, "glClear(");
  if ((mask & GL_ES2_COLOR_BUFFER_BIT) == GL_ES2_COLOR_BUFFER_BIT) {
    log(c, "%sGL_COLOR_BUFFER_BIT", have_any_flag ? " | " : "");
    mask &= ~GL_ES2_COLOR_BUFFER_BIT;
    have_any_flag = 1;
  }
  if ((mask & GL_ES2_DEPTH_BUFFER_BIT) == GL_ES2_DEPTH_BUFFER_BIT) {
    log(c, "%sGL_DEPTH_BUFFER_BIT", have_any_flag ? " | " : "");
    mask &= ~GL_ES2_DEPTH_BUFFER_BIT;
    have_any_flag = 1;
  }
  if ((mask & GL_ES2_STENCIL_BUFFER_BIT) == GL_ES2_STENCIL_BUFFER_BIT) {
    log(c, "%sGL_STENCIL_BUFFER_BIT", have_any_flag ? " | " : "");
    mask &= ~GL_ES2_STENCIL_BUFFER_BIT;
    have_any_flag = 1;
  }
  if (mask) {
    log(c, "%s0x%04X", have_any_flag ? " | " : "", mask);
    have_any_flag = 1;
  }
  if (!have_any_flag) {
    log(c, "0");
  }
  log(c, ");\n");
}

void gl_es2_log_ClearColor(struct gl_es2_context *c, gl_es2_float red, gl_es2_float green, gl_es2_float blue, gl_es2_float alpha) { 
  log(c, "glClearColor(%f, %f, %f, %f);\n", red, green, blue, alpha);
}

void gl_es2_log_ClearDepthf(struct gl_es2_context *c, gl_es2_float d) {
  log(c, "glClearDepthf(%f);\n", d);
}

void gl_es2_log_ClearStencil(struct gl_es2_context *c, gl_es2_int s) { 
  log(c, "glClearStencil(0x%04X);\n", s);
}

void gl_es2_log_ColorMask(struct gl_es2_context *c, gl_es2_boolean red, gl_es2_boolean green, gl_es2_boolean blue, gl_es2_boolean alpha) { 
  log(c, "glColorMask(");
  if (red == GL_ES2_TRUE) log(c, "GL_TRUE, ");
  else if (red == GL_ES2_FALSE) log(c, "GL_FALSE, ");
  else log(c, "0x%04X, ", red);

  if (green == GL_ES2_TRUE) log(c, "GL_TRUE, ");
  else if (green == GL_ES2_FALSE) log(c, "GL_FALSE, ");
  else log(c, "0x%04X, ", green);

  if (blue == GL_ES2_TRUE) log(c, "GL_TRUE, ");
  else if (blue == GL_ES2_FALSE) log(c, "GL_FALSE, ");
  else log(c, "0x%04X, ", blue);

  if (alpha == GL_ES2_TRUE) log(c, "GL_TRUE);");
  else if (alpha == GL_ES2_FALSE) log(c, "GL_FALSE);");
  else log(c, "0x%04X);", alpha);
}

void gl_es2_log_CompileShader(struct gl_es2_context *c, gl_es2_uint shader) { 
  log(c, "glCompileShader(%u);\n", shader);
}

void gl_es2_log_CompressedTexImage2D(struct gl_es2_context *c, gl_es2_enum target, gl_es2_int level, gl_es2_enum internalformat, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border, gl_es2_sizei imageSize, const void *data) {

}

void gl_es2_log_CompressedTexSubImage2D(struct gl_es2_context *c, gl_es2_enum target, gl_es2_int level, gl_es2_int xoffset, gl_es2_int yoffset, gl_es2_sizei width, gl_es2_sizei height, gl_es2_enum format, gl_es2_sizei imageSize, const void *data) { 
}

void gl_es2_log_CopyTexImage2D(struct gl_es2_context *c, gl_es2_enum target, gl_es2_int level, gl_es2_enum internalformat, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border) { 
}

void gl_es2_log_CopyTexSubImage2D(struct gl_es2_context *c, gl_es2_enum target, gl_es2_int level, gl_es2_int xoffset, gl_es2_int yoffset, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height) { 
}

void gl_es2_log_CreateProgram(struct gl_es2_context *c, gl_es2_uint program_created) {
}

void gl_es2_log_CreateShader(struct gl_es2_context *c, gl_es2_enum type, gl_es2_uint shader_created) {
}

void gl_es2_log_CullFace(struct gl_es2_context *c, gl_es2_enum mode) { 
}

void gl_es2_log_DeleteBuffers(struct gl_es2_context *c, gl_es2_sizei n, const gl_es2_uint *buffers) { 
}

void gl_es2_log_DeleteFramebuffers(struct gl_es2_context *c, gl_es2_sizei n, const gl_es2_uint *framebuffers) { 
}

void gl_es2_log_DeleteProgram(struct gl_es2_context *c, gl_es2_uint program) { 
}

void gl_es2_log_DeleteRenderbuffers(struct gl_es2_context *c, gl_es2_sizei n, const gl_es2_uint *renderbuffers) { 
}

void gl_es2_log_DeleteShader(struct gl_es2_context *c, gl_es2_uint shader) { 
}

void gl_es2_log_DeleteTextures(struct gl_es2_context *c, gl_es2_sizei n, const gl_es2_uint *textures) { 
}

void gl_es2_log_DepthFunc(struct gl_es2_context *c, gl_es2_enum func) { 
}

void gl_es2_log_DepthMask(struct gl_es2_context *c, gl_es2_boolean flag) { 
}

void gl_es2_log_DepthRangef(struct gl_es2_context *c, gl_es2_float n, gl_es2_float f) { 
}

void gl_es2_log_DetachShader(struct gl_es2_context *c, gl_es2_uint program, gl_es2_uint shader) { 
}

void gl_es2_log_Disable(struct gl_es2_context *c, gl_es2_enum cap) { 
}

void gl_es2_log_DisableVertexAttribArray(struct gl_es2_context *c, gl_es2_uint index) { 
}

void gl_es2_log_DrawArrays(struct gl_es2_context *c, gl_es2_enum mode, gl_es2_int first, gl_es2_sizei count) { 
}

void gl_es2_log_DrawElements(struct gl_es2_context *c, gl_es2_enum mode, gl_es2_sizei count, gl_es2_enum type, const void *indices) { 
}

void gl_es2_log_Enable(struct gl_es2_context *c, gl_es2_enum cap) { 
}

void gl_es2_log_EnableVertexAttribArray(struct gl_es2_context *c, gl_es2_uint index) { 
}

void gl_es2_log_Finish(struct gl_es2_context *c) { 
}

void gl_es2_log_Flush(struct gl_es2_context *c) { 
}

void gl_es2_log_FramebufferRenderbuffer(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum renderbuffertarget, gl_es2_uint renderbuffer) { 
}

void gl_es2_log_FramebufferTexture2D(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum textarget, gl_es2_uint texture, gl_es2_int level) { 
}

void gl_es2_log_FrontFace(struct gl_es2_context *c, gl_es2_enum mode) { 
}

void gl_es2_log_GenBuffers(struct gl_es2_context *c, gl_es2_sizei n, gl_es2_uint *buffers) { 
}

void gl_es2_log_GenerateMipmap(struct gl_es2_context *c, gl_es2_enum target) { 
}

void gl_es2_log_GenFramebuffers(struct gl_es2_context *c, gl_es2_sizei n, gl_es2_uint *framebuffers) { 
}

void gl_es2_log_GenRenderbuffers(struct gl_es2_context *c, gl_es2_sizei n, gl_es2_uint *renderbuffers) { 
}

void gl_es2_log_GenTextures(struct gl_es2_context *c, gl_es2_sizei n, gl_es2_uint *textures) { 
}

void gl_es2_log_GetActiveAttrib(struct gl_es2_context *c, gl_es2_uint program, gl_es2_uint index, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_int *size, gl_es2_enum *type, gl_es2_char *name) { 
}

void gl_es2_log_GetActiveUniform(struct gl_es2_context *c, gl_es2_uint program, gl_es2_uint index, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_int *size, gl_es2_enum *type, gl_es2_char *name) { 
}

void gl_es2_log_GetAttachedShaders(struct gl_es2_context *c, gl_es2_uint program, gl_es2_sizei maxCount, gl_es2_sizei *count, gl_es2_uint *shaders) { 
}

void gl_es2_log_GetAttribLocation(struct gl_es2_context *c, gl_es2_uint program, const gl_es2_char *name, gl_es2_int location_of_attrib_found) {
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

void gl_es2_log_VertexAttribPointer(struct gl_es2_context *c, gl_es2_uint index, gl_es2_int size, gl_es2_enum type, gl_es2_boolean normalized, gl_es2_sizei stride, const void *pointer) { 
}

void gl_es2_log_Viewport(struct gl_es2_context *c, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height) {
}



