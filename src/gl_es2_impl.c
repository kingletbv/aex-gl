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

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef GL_ES2_IMPL_H_INCLUDED
#define GL_ES2_IMPL_H_INCLUDED
#include "gl_es2_impl.h"
#endif

#ifndef GL_ES2_CONTEXT_H_INCLUDED
#define GL_ES2_CONTEXT_H_INCLUDED
#include "gl_es2_context.h"
#endif

static void set_gl_err(gl_es2_enum error_code) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (c->current_error_ == GL_ES2_NO_ERROR) c->current_error_ = error_code;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ActiveTexture)(gl_es2_enum texture) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(AttachShader)(gl_es2_uint program, gl_es2_uint shader) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindAttribLocation)(gl_es2_uint program, gl_es2_uint index, const gl_es2_char *name) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindBuffer)(gl_es2_enum target, gl_es2_uint buffer) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindFramebuffer)(gl_es2_enum target, gl_es2_uint framebuffer) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindRenderbuffer)(gl_es2_enum target, gl_es2_uint renderbuffer) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindTexture)(gl_es2_enum target, gl_es2_uint texture) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendColor)(gl_es2_float red, gl_es2_float green, gl_es2_float blue, gl_es2_float alpha) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendEquation)(gl_es2_enum mode) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendEquationSeparate)(gl_es2_enum modeRGB, gl_es2_enum modeAlpha) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendFunc)(gl_es2_enum sfactor, gl_es2_enum dfactor) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendFuncSeparate)(gl_es2_enum sfactorRGB, gl_es2_enum dfactorRGB, gl_es2_enum sfactorAlpha, gl_es2_enum dfactorAlpha) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BufferData)(gl_es2_enum target, gl_es2_sizeiptr size, const void *data, gl_es2_enum usage) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BufferSubData)(gl_es2_enum target, gl_es2_intptr offset, gl_es2_sizeiptr size, const void *data) {
}

GL_ES2_DECL_SPEC gl_es2_enum GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CheckFramebufferStatus)(gl_es2_enum target) {
  return GL_ES2_FRAMEBUFFER_UNSUPPORTED;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Clear)(gl_es2_bitfield mask) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ClearColor)(gl_es2_float red, gl_es2_float green, gl_es2_float blue, gl_es2_float alpha) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ClearDepthf)(gl_es2_float d) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ClearStencil)(gl_es2_int s) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ColorMask)(gl_es2_boolean red, gl_es2_boolean green, gl_es2_boolean blue, gl_es2_boolean alpha) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CompileShader)(gl_es2_uint shader) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CompressedTexImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_enum internalformat, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border, gl_es2_sizei imageSize, const void *data) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CompressedTexSubImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_int xoffset, gl_es2_int yoffset, gl_es2_sizei width, gl_es2_sizei height, gl_es2_enum format, gl_es2_sizei imageSize, const void *data) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CopyTexImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_enum internalformat, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CopyTexSubImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_int xoffset, gl_es2_int yoffset, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height) {
}

GL_ES2_DECL_SPEC gl_es2_uint GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CreateProgram)(void) {
  return 0;
}

GL_ES2_DECL_SPEC gl_es2_uint GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CreateShader)(gl_es2_enum type) {
  return 0;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CullFace)(gl_es2_enum mode) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteBuffers)(gl_es2_sizei n, const gl_es2_uint *buffers) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteFramebuffers)(gl_es2_sizei n, const gl_es2_uint *framebuffers) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (n < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (!n) return;

  size_t k;
  for (k = 0; k < n; ++k) {
    uintptr_t fb_name = framebuffers[k];
    /* silently ignore 0's */
    if (fb_name) {
      int is_allocated = ref_range_get_ref_at(&c->framebuffer_rra_, fb_name);
      if (is_allocated) {
        struct gl_es2_framebuffer *fb = NULL;
        fb = (struct gl_es2_framebuffer *)not_find(&c->framebuffer_not_, fb_name);
        if (fb) {
          /* framebuffer was bound & created before; release and free it */
          not_remove(&c->framebuffer_not_, &fb->no_);
          gl_es2_framebuffer_cleanup(fb);
          free(fb);
        }
        else {
          /* framebuffer was allocated (refcount is not 0), but never bound and so the 
           * framebuffer object itself was never created (despite being allocated). This
           * is valid, e.g. glGenFramebuffers without a corresponding glBindFramebuffer
           * will put us in this state. */
        }
        ref_range_mark_range_free(&c->framebuffer_rra_, fb_name, fb_name + 1);
      }
    }
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteProgram)(gl_es2_uint program) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteRenderbuffers)(gl_es2_sizei n, const gl_es2_uint *renderbuffers) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteShader)(gl_es2_uint shader) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteTextures)(gl_es2_sizei n, const gl_es2_uint *textures) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DepthFunc)(gl_es2_enum func) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DepthMask)(gl_es2_boolean flag) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DepthRangef)(gl_es2_float n, gl_es2_float f) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DetachShader)(gl_es2_uint program, gl_es2_uint shader) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Disable)(gl_es2_enum cap) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DisableVertexAttribArray)(gl_es2_uint index) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DrawArrays)(gl_es2_enum mode, gl_es2_int first, gl_es2_sizei count) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DrawElements)(gl_es2_enum mode, gl_es2_sizei count, gl_es2_enum type, const void *indices) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Enable)(gl_es2_enum cap) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(EnableVertexAttribArray)(gl_es2_uint index) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Finish)(void) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Flush)(void) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(FramebufferRenderbuffer)(gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum renderbuffertarget, gl_es2_uint renderbuffer) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(FramebufferTexture2D)(gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum textarget, gl_es2_uint texture, gl_es2_int level) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(FrontFace)(gl_es2_enum mode) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenBuffers)(gl_es2_sizei n, gl_es2_uint *buffers) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenerateMipmap)(gl_es2_enum target) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenFramebuffers)(gl_es2_sizei n, gl_es2_uint *framebuffers) {
  struct gl_es2_context *c = gl_es2_ctx();
  int r;
  if (n < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (!n) return;

  /* We further guarantee that the allocated framebuffers are consecutive, makes implementation
   * easier, spec doesn't need it. */
  uintptr_t alloc_range_at = 0;
  r = ref_range_alloc(&c->framebuffer_rra_, (uintptr_t)n, &alloc_range_at);
  if (r) {
    set_gl_err(GL_ES2_OUT_OF_MEMORY);
    return;
  }
  gl_es2_sizei k;
  for (k = 0; k < n; ++k) {
    uintptr_t slot = alloc_range_at + k;
    if (slot < alloc_range_at) {
      /* overflow */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    gl_es2_uint slot_uint = (gl_es2_uint)slot;
    if (slot != (uintptr_t)slot_uint) {
      /* overflow because type too narrow */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    framebuffers[k] = slot_uint;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenRenderbuffers)(gl_es2_sizei n, gl_es2_uint *renderbuffers) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenTextures)(gl_es2_sizei n, gl_es2_uint *textures) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetActiveAttrib)(gl_es2_uint program, gl_es2_uint index, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_int *size, gl_es2_enum *type, gl_es2_char *name) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetActiveUniform)(gl_es2_uint program, gl_es2_uint index, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_int *size, gl_es2_enum *type, gl_es2_char *name) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetAttachedShaders)(gl_es2_uint program, gl_es2_sizei maxCount, gl_es2_sizei *count, gl_es2_uint *shaders) {
}

GL_ES2_DECL_SPEC gl_es2_int GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetAttribLocation)(gl_es2_uint program, const gl_es2_char *name) {
  return GL_ES2_INVALID_OPERATION;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetBooleanv)(gl_es2_enum pname, gl_es2_boolean *data) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetBufferParameteriv)(gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params) {
}

GL_ES2_DECL_SPEC gl_es2_enum GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetError)(void) {
  struct gl_es2_context *c = gl_es2_ctx();
  gl_es2_enum err = c->current_error_;
  c->current_error_ = GL_ES2_NO_ERROR;
  return err;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetFloatv)(gl_es2_enum pname, gl_es2_float *data) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetFramebufferAttachmentParameteriv)(gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum pname, gl_es2_int *params) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetIntegerv)(gl_es2_enum pname, gl_es2_int *data) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetProgramiv)(gl_es2_uint program, gl_es2_enum pname, gl_es2_int *params) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetProgramInfoLog)(gl_es2_uint program, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_char *infoLog) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetRenderbufferParameteriv)(gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetShaderiv)(gl_es2_uint shader, gl_es2_enum pname, gl_es2_int *params) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetShaderInfoLog)(gl_es2_uint shader, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_char *infoLog) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetShaderPrecisionFormat)(gl_es2_enum shadertype, gl_es2_enum precisiontype, gl_es2_int *range, gl_es2_int *precision) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetShaderSource)(gl_es2_uint shader, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_char *source) {
}

GL_ES2_DECL_SPEC const gl_es2_ubyte *GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetString)(gl_es2_enum name) {
  switch (name) {
    case GL_ES2_VENDOR:
      return "Kinglet B.V.";
    case GL_ES2_RENDERER:
      return "Aex";
    case GL_ES2_VERSION:
      return "OpenGL ES 2.0.0 Aex 0.01";
    case GL_ES2_SHADING_LANGUAGE_VERSION:
      return "OpenGL ES GLSL ES 1.00 (Aex 0.01)";
    case GL_ES2_EXTENSIONS:
      return "";
  }
  return NULL;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetTexParameterfv)(gl_es2_enum target, gl_es2_enum pname, gl_es2_float *params) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetTexParameteriv)(gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetUniformfv)(gl_es2_uint program, gl_es2_int location, gl_es2_float *params) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetUniformiv)(gl_es2_uint program, gl_es2_int location, gl_es2_int *params) {
}

GL_ES2_DECL_SPEC gl_es2_int GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetUniformLocation)(gl_es2_uint program, const gl_es2_char *name) {
  if (!memcmp(name, "gl_", 3)) {
    return -1;
  }
  return -1;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetVertexAttribfv)(gl_es2_uint index, gl_es2_enum pname, gl_es2_float *params) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetVertexAttribiv)(gl_es2_uint index, gl_es2_enum pname, gl_es2_int *params) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetVertexAttribPointerv)(gl_es2_uint index, gl_es2_enum pname, void **pointer) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Hint)(gl_es2_enum target, gl_es2_enum mode) {
}

GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsBuffer)(gl_es2_uint buffer) {
  return GL_ES2_FALSE;
}

GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsEnabled)(gl_es2_enum cap) {
  return GL_ES2_FALSE;
}

GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsFramebuffer)(gl_es2_uint framebuffer) {
  return GL_ES2_FALSE;
}

GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsProgram)(gl_es2_uint program) {
  return GL_ES2_FALSE;
}

GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsRenderbuffer)(gl_es2_uint renderbuffer) {
  return GL_ES2_FALSE;
}

GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsShader)(gl_es2_uint shader) {
  return GL_ES2_FALSE;
}

GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsTexture)(gl_es2_uint texture) {
  return GL_ES2_FALSE;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(LineWidth)(gl_es2_float width) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(LinkProgram)(gl_es2_uint program) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(PixelStorei)(gl_es2_enum pname, gl_es2_int param) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(PolygonOffset)(gl_es2_float factor, gl_es2_float units) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ReadPixels)(gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height, gl_es2_enum format, gl_es2_enum type, void *pixels) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ReleaseShaderCompiler)(void) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(RenderbufferStorage)(gl_es2_enum target, gl_es2_enum internalformat, gl_es2_sizei width, gl_es2_sizei height) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(SampleCoverage)(gl_es2_float value, gl_es2_boolean invert) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Scissor)(gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ShaderBinary)(gl_es2_sizei count, const gl_es2_uint *shaders, gl_es2_enum binaryformat, const void *binary, gl_es2_sizei length) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ShaderSource)(gl_es2_uint shader, gl_es2_sizei count, const gl_es2_char *const *string, const gl_es2_int *length) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilFunc)(gl_es2_enum func, gl_es2_int ref, gl_es2_uint mask) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilFuncSeparate)(gl_es2_enum face, gl_es2_enum func, gl_es2_int ref, gl_es2_uint mask) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilMask)(gl_es2_uint mask) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilMaskSeparate)(gl_es2_enum face, gl_es2_uint mask) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilOp)(gl_es2_enum fail, gl_es2_enum zfail, gl_es2_enum zpass) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilOpSeparate)(gl_es2_enum face, gl_es2_enum sfail, gl_es2_enum dpfail, gl_es2_enum dppass) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_int internalformat, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border, gl_es2_enum format, gl_es2_enum type, const void *pixels) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexParameterf)(gl_es2_enum target, gl_es2_enum pname, gl_es2_float param) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexParameterfv)(gl_es2_enum target, gl_es2_enum pname, const gl_es2_float *params) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexParameteri)(gl_es2_enum target, gl_es2_enum pname, gl_es2_int param) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexParameteriv)(gl_es2_enum target, gl_es2_enum pname, const gl_es2_int *params) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexSubImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_int xoffset, gl_es2_int yoffset, gl_es2_sizei width, gl_es2_sizei height, gl_es2_enum format, gl_es2_enum type, const void *pixels) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform1f)(gl_es2_int location, gl_es2_float v0) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform1fv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform1i)(gl_es2_int location, gl_es2_int v0) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform1iv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform2f)(gl_es2_int location, gl_es2_float v0, gl_es2_float v1) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform2fv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform2i)(gl_es2_int location, gl_es2_int v0, gl_es2_int v1) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform2iv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform3f)(gl_es2_int location, gl_es2_float v0, gl_es2_float v1, gl_es2_float v2) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform3fv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform3i)(gl_es2_int location, gl_es2_int v0, gl_es2_int v1, gl_es2_int v2) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform3iv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform4f)(gl_es2_int location, gl_es2_float v0, gl_es2_float v1, gl_es2_float v2, gl_es2_float v3) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform4fv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform4i)(gl_es2_int location, gl_es2_int v0, gl_es2_int v1, gl_es2_int v2, gl_es2_int v3) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform4iv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(UniformMatrix2fv)(gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(UniformMatrix3fv)(gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(UniformMatrix4fv)(gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(UseProgram)(gl_es2_uint program) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ValidateProgram)(gl_es2_uint program) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib1f)(gl_es2_uint index, gl_es2_float x) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib1fv)(gl_es2_uint index, const gl_es2_float *v) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib2f)(gl_es2_uint index, gl_es2_float x, gl_es2_float y) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib2fv)(gl_es2_uint index, const gl_es2_float *v) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib3f)(gl_es2_uint index, gl_es2_float x, gl_es2_float y, gl_es2_float z) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib3fv)(gl_es2_uint index, const gl_es2_float *v) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib4f)(gl_es2_uint index, gl_es2_float x, gl_es2_float y, gl_es2_float z, gl_es2_float w) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib4fv)(gl_es2_uint index, const gl_es2_float *v) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttribPointer)(gl_es2_uint index, gl_es2_int size, gl_es2_enum type, gl_es2_boolean normalized, gl_es2_sizei stride, const void *pointer) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Viewport)(gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height) {
}

