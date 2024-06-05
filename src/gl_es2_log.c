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

#ifndef GL_ES2_LOG_H_INCLUDED
#define GL_ES2_LOG_H_INCLUDED
#include "gl_es2_log.h"
#endif

void gl_es2_log_ActiveTexture(struct gl_es2_context *c, gl_es2_enum texture) {
}

void gl_es2_log_AttachShader(struct gl_es2_context *c, gl_es2_uint program, gl_es2_uint shader) {
}

void gl_es2_log_BindAttribLocation(struct gl_es2_context *c, gl_es2_uint program, gl_es2_uint index, const gl_es2_char *name) { 
}

void gl_es2_log_BindBuffer(struct gl_es2_context *c, gl_es2_enum target, gl_es2_uint buffer) { 
}

void gl_es2_log_BindFramebuffer(struct gl_es2_context *c, gl_es2_enum target, gl_es2_uint framebuffer) { 
}

void gl_es2_log_BindRenderbuffer(struct gl_es2_context *c, gl_es2_enum target, gl_es2_uint renderbuffer) { 
}

void gl_es2_log_BindTexture(struct gl_es2_context *c, gl_es2_enum target, gl_es2_uint texture) { 
}

void gl_es2_log_BlendColor(struct gl_es2_context *c, gl_es2_float red, gl_es2_float green, gl_es2_float blue, gl_es2_float alpha) { 
}

void gl_es2_log_BlendEquation(struct gl_es2_context *c, gl_es2_enum mode) { 
}

void gl_es2_log_BlendEquationSeparate(struct gl_es2_context *c, gl_es2_enum modeRGB, gl_es2_enum modeAlpha) { 
}

void gl_es2_log_BlendFunc(struct gl_es2_context *c, gl_es2_enum sfactor, gl_es2_enum dfactor) { 
}

void gl_es2_log_BlendFuncSeparate(struct gl_es2_context *c, gl_es2_enum sfactorRGB, gl_es2_enum dfactorRGB, gl_es2_enum sfactorAlpha, gl_es2_enum dfactorAlpha) { 
}

void gl_es2_log_BufferData(struct gl_es2_context *c, gl_es2_enum target, gl_es2_sizeiptr size, const void *data, gl_es2_enum usage) { 
}

void gl_es2_log_BufferSubData(struct gl_es2_context *c, gl_es2_enum target, gl_es2_intptr offset, gl_es2_sizeiptr size, const void *data) { 
}

void gl_es2_log_CheckFramebufferStatus(struct gl_es2_context *c, gl_es2_enum target, gl_es2_enum framebuffer_status_assessed) {
}

void gl_es2_log_Clear(struct gl_es2_context *c, gl_es2_bitfield mask) { 
}

void gl_es2_log_ClearColor(struct gl_es2_context *c, gl_es2_float red, gl_es2_float green, gl_es2_float blue, gl_es2_float alpha) { 
}

void gl_es2_log_ClearDepthf(struct gl_es2_context *c, gl_es2_float d) { 
}

void gl_es2_log_ClearStencil(struct gl_es2_context *c, gl_es2_int s) { 
}

void gl_es2_log_ColorMask(struct gl_es2_context *c, gl_es2_boolean red, gl_es2_boolean green, gl_es2_boolean blue, gl_es2_boolean alpha) { 
}

void gl_es2_log_CompileShader(struct gl_es2_context *c, gl_es2_uint shader) { 
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



