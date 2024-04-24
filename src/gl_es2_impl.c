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

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED
#include <math.h>
#endif

#ifndef GL_ES2_IMPL_H_INCLUDED
#define GL_ES2_IMPL_H_INCLUDED
#include "gl_es2_impl.h"
#endif

#ifndef GL_ES2_CONTEXT_H_INCLUDED
#define GL_ES2_CONTEXT_H_INCLUDED
#include "gl_es2_context.h"
#endif

#ifndef SL_DEFS_H_INCLUDED
#define SL_DEFS_H_INCLUDED
#include "sl_defs.h"
#endif

static void set_gl_err(gl_es2_enum error_code) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (c->current_error_ == GL_ES2_NO_ERROR) c->current_error_ = error_code;
}

static void generic_name_gen(struct ref_range_allocator *rra, gl_es2_sizei n, gl_es2_uint *names) {
  struct gl_es2_context *c = gl_es2_ctx();
  int r;
  if (n < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (!n) return;

  /* We further guarantee that the allocated renderbuffers are consecutive, makes implementation
   * easier, spec doesn't need it. */
  uintptr_t alloc_range_at = 0;
  r = ref_range_alloc(rra, (uintptr_t)n, &alloc_range_at);
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
    names[k] = slot_uint;
  }
}

static void detach_program_shader_with_cascaded_delete(struct gl_es2_program_shader_attachment *psa) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_shader *shad;
  shad = psa->shader_;
  if (!shad) return;
  gl_es2_program_shader_attachment_detach(psa);
  if (!shad->first_program_attached_to_ && shad->flagged_for_deletion_) {
    ref_range_mark_range_free(&c->shader_rra_, shad->no_.name_, shad->no_.name_ + 1);
    not_remove(&c->shader_not_, &shad->no_);
  }
}

static void check_old_program_for_deletion(struct gl_es2_program *old_prog) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (old_prog && old_prog->flagged_for_deletion_) {
    detach_program_shader_with_cascaded_delete(&old_prog->vertex_shader_);
    detach_program_shader_with_cascaded_delete(&old_prog->fragment_shader_);
    ref_range_mark_range_free(&c->program_rra_, old_prog->no_.name_, old_prog->no_.name_ + 1);
    not_remove(&c->program_not_, &old_prog->no_);
    gl_es2_program_cleanup(old_prog);
    free(old_prog);
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ActiveTexture)(gl_es2_enum texture) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (texture >= c->num_active_texture_units_) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }
  c->current_active_texture_unit_ = (size_t)texture;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(AttachShader)(gl_es2_uint program, gl_es2_uint shader) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t shad_name = (uintptr_t)shader;
  struct gl_es2_shader *shad = NULL;
  shad = (struct gl_es2_shader *)not_find(&c->shader_not_, shad_name);
  if (!shad) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = NULL;
  prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  struct gl_es2_program_shader_attachment *psa = NULL;
  switch (shad->type_) {
    case GL_ES2_VERTEX_SHADER:
      psa = &prog->vertex_shader_;
      break;
    case GL_ES2_FRAGMENT_SHADER:
      psa = &prog->fragment_shader_;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
  }
  if (psa->shader_) {
    /* shader already attached to program, or another shader object of the same type is already attached to program */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  gl_es2_program_shader_attachment_attach(psa, shad);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindAttribLocation)(gl_es2_uint program, gl_es2_uint index, const gl_es2_char *name) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (!name) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (!c->allow_internals_ && !memcmp(name, "gl_", 3)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (index >= GL_ES2_MAX_NUM_VERTEX_ATTRIBS) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog;
  prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  int r;
  r = sl_program_set_attrib_binding_index(&prog->program_, name, index);
  switch (r) {
    case SL_ERR_NO_MEM:
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    case SL_ERR_INTERNAL:
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    case SL_ERR_OK:
      /* yay */
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindBuffer)(gl_es2_enum target, gl_es2_uint buffer) {
  struct gl_es2_context *c = gl_es2_ctx();
  if ((target != GL_ES2_ARRAY_BUFFER) && (target != GL_ES2_ELEMENT_ARRAY_BUFFER)) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }

  uintptr_t buf_name = (uintptr_t)buffer;
  if (buf_name == UINTPTR_MAX) {
    /* invalid number because we cannot make a range buf_name to buf_name+1. */
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (buf_name == 0) {
    /* There is no "default" buffer at name 0, instead it should use client memory,
     * this means setting the corresponding target to NULL */
    switch (target) {
      case GL_ES2_ARRAY_BUFFER:
        c->array_buffer_ = NULL;
        break;
      case GL_ES2_ELEMENT_ARRAY_BUFFER:
        c->element_array_buffer_ = NULL;
        break;
    }
    return;
  }
  int r;
  int is_allocated = ref_range_get_ref_at(&c->buffer_rra_, buf_name);
  if (!is_allocated) {
    /* Caller hasn't reserved this but is just taking the name */
    r = ref_range_mark_range_allocated(&c->buffer_rra_, buf_name, buf_name + 1);
    if (r) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
  }
  struct gl_es2_buffer *buf = NULL;
  r = not_find_or_insert(&c->buffer_not_, buf_name, sizeof(struct gl_es2_buffer), (struct named_object **)&buf);
  if (r == NOT_NOMEM) {
    set_gl_err(GL_ES2_OUT_OF_MEMORY);
    return;
  }
  else if (r == NOT_DUPLICATE) {
    /* Common path if is_allocated, strange path we'll run with if not */
  }
  else if (r == NOT_OK) {
    /* Newly created buffer */
    gl_es2_buffer_init(buf);
  }
  switch (target) {
    case GL_ES2_ARRAY_BUFFER:
      c->array_buffer_ = buf;
      break;
    case GL_ES2_ELEMENT_ARRAY_BUFFER:
      c->element_array_buffer_ = buf;
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindFramebuffer)(gl_es2_enum target, gl_es2_uint framebuffer) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (target != GL_ES2_FRAMEBUFFER) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }
  uintptr_t fb_name = (uintptr_t)framebuffer;
  if (fb_name == UINTPTR_MAX) {
    /* invalid number because we cannot make a range fb_name to fb_name+1. */
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;;
  int is_allocated = ref_range_get_ref_at(&c->framebuffer_rra_, fb_name);
  if (!is_allocated) {
    /* Caller hasn't reserved this but is just taking the name, which is fine, allocate it */
    r = ref_range_mark_range_allocated(&c->framebuffer_rra_, fb_name, fb_name + 1);
    if (r) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
  }
  struct gl_es2_framebuffer *fb = NULL;
  r = not_find_or_insert(&c->framebuffer_not_, fb_name, sizeof(struct gl_es2_framebuffer), (struct named_object **)&fb);
  if (r == NOT_NOMEM) {
    set_gl_err(GL_ES2_OUT_OF_MEMORY);
    return;
  }
  else if (r == NOT_DUPLICATE) {
    /* Common path if is_allocated, strange path we'll just run with if not */
    c->framebuffer_ = fb;
  }
  else if (r == NOT_OK) {
    /* Newly created framebuffer */
    gl_es2_framebuffer_init(fb);
    c->framebuffer_ = fb;
  }
  return;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindRenderbuffer)(gl_es2_enum target, gl_es2_uint renderbuffer) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (target != GL_ES2_RENDERBUFFER) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }
  uintptr_t rb_name = (uintptr_t)renderbuffer;
  if (rb_name == UINTPTR_MAX) {
    /* invalid number because we cannot make a range rb_name to rb_name+1. */
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  int is_allocated = ref_range_get_ref_at(&c->renderbuffer_rra_, rb_name);
  if (!is_allocated) {
    /* Caller hasn't reserved this but is just taking the name, which is fine, allocate it */
    r = ref_range_mark_range_allocated(&c->renderbuffer_rra_, rb_name, rb_name + 1);
    if (r) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
  }
  struct gl_es2_renderbuffer *rb = NULL;
  r = not_find_or_insert(&c->renderbuffer_not_, rb_name, sizeof(struct gl_es2_renderbuffer), (struct named_object **)&rb);
  if (r == NOT_NOMEM) {
    set_gl_err(GL_ES2_OUT_OF_MEMORY);
    return;
  }
  else if (r == NOT_DUPLICATE) {
    /* Common path if is_allocated, strange path we'll just run with if not */
    c->renderbuffer_ = rb;
  }
  else if (r == NOT_OK) {
    /* Newly created renderbuffer */
    gl_es2_renderbuffer_init(rb);
    c->renderbuffer_ = rb;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BindTexture)(gl_es2_enum target, gl_es2_uint texture) {
  struct gl_es2_context *c = gl_es2_ctx();
  if ((target != GL_ES2_TEXTURE_2D) && (target != GL_ES2_TEXTURE_CUBE_MAP)) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }

  uintptr_t tex_name = (uintptr_t)texture;
  if (tex_name == UINTPTR_MAX) {
    /* invalid number because we cannot make a range tex_name to tex_name+1. */
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  int is_allocated = ref_range_get_ref_at(&c->texture_rra_, tex_name);
  if (!is_allocated) {
    /* Caller hasn't reserved this but is just taking the name, which is fine, allocate it */
    r = ref_range_mark_range_allocated(&c->texture_rra_, tex_name, tex_name + 1);
    if (r) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
  }
  struct gl_es2_texture *tex = NULL;
  r = not_find_or_insert(&c->texture_not_, tex_name, sizeof(struct gl_es2_texture), (struct named_object **)&tex);
  if (r == NOT_NOMEM) {
    set_gl_err(GL_ES2_OUT_OF_MEMORY);
    return;
  }
  else if (r == NOT_DUPLICATE) {
    /* Common path if is_allocated, strange path we'll just run with if not */
  }
  else if (r == NOT_OK) {
    /* Newly created texture */
    gl_es2_texture_init(tex);
    switch (target) {
      case GL_ES2_TEXTURE_2D:
        tex->kind_ = gl_es2_texture_2d;
        break;
      case GL_ES2_TEXTURE_CUBE_MAP:
        tex->kind_ = gl_es2_texture_cube_map;
        break;
    }
  }
  switch (target) {
    case GL_ES2_TEXTURE_2D:
      if (tex->kind_ != gl_es2_texture_2d) {
        /* Texture previously created with a target kind that doesn't match target */
        set_gl_err(GL_ES2_INVALID_OPERATION);
        return;
      }
      c->active_texture_units_[c->current_active_texture_unit_].texture_2d_ = tex;
      break;
    case GL_ES2_TEXTURE_CUBE_MAP:
      if (tex->kind_ != gl_es2_texture_cube_map) {
        /* Texture previously created with a target kind that doesn't match target */
        set_gl_err(GL_ES2_INVALID_OPERATION);
        return;
      }
      c->active_texture_units_[c->current_active_texture_unit_].texture_cube_map_ = tex;
      break;
    default:
      /* other cases already ruled out (see check at entry) */
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendColor)(gl_es2_float red, gl_es2_float green, gl_es2_float blue, gl_es2_float alpha) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (red < 0.f) red = 0.f;
  if (red > 1.f) red = 1.f;
  if (green < 0.f) green = 0.f;
  if (green > 1.f) green = 1.f;
  if (blue < 0.f) blue = 0.f;
  if (blue > 1.f) blue = 1.f;
  if (alpha < 0.f) alpha = 0.f;
  if (alpha > 1.f) alpha = 1.f;
  uint32_t ured = (uint32_t)floorf(256.f * red);
  uint32_t ugreen = (uint32_t)floorf(256.f * green);
  uint32_t ublue = (uint32_t)floorf(256.f * blue);
  uint32_t ualpha = (uint32_t)floorf(256.f * alpha);
  /* Clamp integer to range 0..255 as it could be 0..256
   * 256 occurs when the input is exactly 1.f, in which case 
   * floorf doesn't floor down. Note that variations where we
   * multiply by 255 are wrong because that mis-assigns the
   * range where we would hit maximum brightness (255) for only
   * the case where the value is *exactly* 1, rather than a range
   * sized 1/256.
   * Code below casts to 8 bits, if the input is 256, that value
   * is 0x00 (because 256==0x100). To ensure a value of 256 becomes
   * 255, we take the input value u, shift right 8 bits, and arrive
   * at a bit that will only be 1 if the value is 256. We subtract
   * that bit from the previous result. The previous result will be
   * 0x00 for an input of 0x100, resulting in the desired 0xFF.
   */
  c->blend_color_red_ = ((uint8_t)ured) - (uint8_t)(ured >> 8);
  c->blend_color_grn_ = ((uint8_t)ugreen) - (uint8_t)(ugreen >> 8);
  c->blend_color_blu_ = ((uint8_t)ublue) - (uint8_t)(ublue >> 8);
  c->blend_color_alpha_ = ((uint8_t)ualpha) - (uint8_t)(ualpha >> 8);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendEquation)(gl_es2_enum mode) {
  struct gl_es2_context *c = gl_es2_ctx();
  switch (mode) {
    case GL_ES2_FUNC_ADD:
      c->blend_rgb_eq_ = BEQ_FUNC_ADD;
      c->blend_alpha_eq_ = BEQ_FUNC_ADD;
      break;
    case GL_ES2_FUNC_SUBTRACT:
      c->blend_rgb_eq_ = BEQ_FUNC_SUBTRACT;
      c->blend_alpha_eq_ = BEQ_FUNC_SUBTRACT;
      break;
    case GL_ES2_FUNC_REVERSE_SUBTRACT:
      c->blend_rgb_eq_ = BEQ_FUNC_REVERSE_SUBTRACT;
      c->blend_alpha_eq_ = BEQ_FUNC_REVERSE_SUBTRACT;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendEquationSeparate)(gl_es2_enum modeRGB, gl_es2_enum modeAlpha) {
  struct gl_es2_context *c = gl_es2_ctx();

  switch (modeRGB) {
    case GL_ES2_FUNC_ADD:
    case GL_ES2_FUNC_SUBTRACT:
    case GL_ES2_FUNC_REVERSE_SUBTRACT:
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
  switch (modeAlpha) {
    case GL_ES2_FUNC_ADD:
    case GL_ES2_FUNC_SUBTRACT:
    case GL_ES2_FUNC_REVERSE_SUBTRACT:
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  switch (modeRGB) {
    case GL_ES2_FUNC_ADD:
      c->blend_rgb_eq_ = BEQ_FUNC_ADD;
      break;
    case GL_ES2_FUNC_SUBTRACT:
      c->blend_rgb_eq_ = BEQ_FUNC_SUBTRACT;
      break;
    case GL_ES2_FUNC_REVERSE_SUBTRACT:
      c->blend_rgb_eq_ = BEQ_FUNC_REVERSE_SUBTRACT;
      break;
  }

  switch (modeAlpha) {
    case GL_ES2_FUNC_ADD:
      c->blend_alpha_eq_ = BEQ_FUNC_ADD;
      break;
    case GL_ES2_FUNC_SUBTRACT:
      c->blend_alpha_eq_ = BEQ_FUNC_SUBTRACT;
      break;
    case GL_ES2_FUNC_REVERSE_SUBTRACT:
      c->blend_alpha_eq_ = BEQ_FUNC_REVERSE_SUBTRACT;
      break;
  }
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
  struct gl_es2_context *c = gl_es2_ctx();
  gl_es2_uint prog_name = 0;
  generic_name_gen(&c->program_rra_, 1, &prog_name);
  if (!prog_name) return 0;

  struct gl_es2_program *prog = NULL;
  int r;
  r = not_find_or_insert(&c->program_not_, prog_name, sizeof(struct gl_es2_program), (struct named_object **)&prog);
  if (r == NOT_NOMEM) {
    set_gl_err(GL_ES2_OUT_OF_MEMORY);
    return 0;
  }
  else if (r == NOT_DUPLICATE) {
    /* Should never be returned as we just allocated the program anew */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return 0;
  }
  /* else not_ok */
  gl_es2_program_init(prog);

  return prog_name;
}

GL_ES2_DECL_SPEC gl_es2_uint GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CreateShader)(gl_es2_enum type) {
  struct gl_es2_context *c = gl_es2_ctx();
  if ((type != GL_ES2_VERTEX_SHADER) && (type != GL_ES2_FRAGMENT_SHADER)) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return 0;
  }
  gl_es2_uint shad_name = 0;
  generic_name_gen(&c->shader_rra_, 1, &shad_name);
  if (!shad_name) return 0;

  struct gl_es2_shader *shad = NULL;
  int r;
  r = not_find_or_insert(&c->shader_not_, shad_name, sizeof(struct gl_es2_shader), (struct named_object **)&shad);
  if (r == NOT_NOMEM) {
    set_gl_err(GL_ES2_OUT_OF_MEMORY);
    return 0;
  }
  else if (r == NOT_DUPLICATE) {
    /* Should never be returned as we just allocated the program anew */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return 0;
  }
  /* else not_ok */
  gl_es2_shader_init(shad);
  shad->type_ = type;

  return shad_name;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CullFace)(gl_es2_enum mode) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteBuffers)(gl_es2_sizei n, const gl_es2_uint *buffers) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (n < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (!n) return;

  size_t k;
  for (k = 0; k < n; ++k) {
    uintptr_t buf_name = buffers[k];
    if (buf_name) {
      int is_allocated = ref_range_get_ref_at(&c->buffer_rra_, buf_name);
      if (is_allocated) {
        struct gl_es2_buffer *buf = NULL;
        buf = (struct gl_es2_buffer *)not_find(&c->buffer_not_, buf_name);
        if (buf) {
          /* buffer was bound and created before; release & free it */
          if (buf == c->array_buffer_) {
            c->array_buffer_ = NULL;
          }
          if (buf == c->element_array_buffer_) {
            c->element_array_buffer_ = NULL;
          }
          not_remove(&c->buffer_not_, &buf->no_);
          gl_es2_buffer_cleanup(buf);
          free(buf);
        }
        else {
          /* buffer name was allocated, but never bound */
        }
        ref_range_mark_range_free(&c->buffer_rra_, buf_name, buf_name + 1);
      }
    }
  }
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
          if (fb == c->framebuffer_) {
            c->framebuffer_ = (struct gl_es2_framebuffer *)not_find(&c->framebuffer_not_, 0);
          }
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
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t prog_name = (uintptr_t)program;
  int is_allocated = ref_range_get_ref_at(&c->program_rra_, prog_name);
  if (is_allocated) {
    struct gl_es2_program *prog = NULL;
    prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
    if (prog) {
      if (c->current_program_ == prog) {
        prog->flagged_for_deletion_ = 1;
      }
      else {
        detach_program_shader_with_cascaded_delete(&prog->vertex_shader_);
        detach_program_shader_with_cascaded_delete(&prog->fragment_shader_);
        ref_range_mark_range_free(&c->program_rra_, prog_name, prog_name + 1);
        not_remove(&c->program_not_, &prog->no_);
        gl_es2_program_cleanup(prog);
        free(prog);
      }
    }
    else {
      ref_range_mark_range_free(&c->program_rra_, prog_name, prog_name + 1);
    }
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteRenderbuffers)(gl_es2_sizei n, const gl_es2_uint *renderbuffers) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (n < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (!n) return;

  size_t k;
  for (k = 0; k < n; ++k) {
    uintptr_t rb_name = renderbuffers[k];
    /* silently ignore 0's */
    if (rb_name) {
      int is_allocated = ref_range_get_ref_at(&c->framebuffer_rra_, rb_name);
      if (is_allocated) {
        struct gl_es2_renderbuffer *rb = NULL;
        rb = (struct gl_es2_renderbuffer *)not_find(&c->framebuffer_not_, rb_name);
        if (rb) {
          /* renderbuffer was bound & created before; release and free it */
          if (rb == c->renderbuffer_) {
            c->renderbuffer_ = (struct gl_es2_renderbuffer *)not_find(&c->renderbuffer_not_, 0);
          }
          not_remove(&c->renderbuffer_not_, &rb->no_);
          gl_es2_renderbuffer_cleanup(rb);
          free(rb);
        }
        ref_range_mark_range_free(&c->renderbuffer_rra_, rb_name, rb_name + 1);
      }
    }
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteShader)(gl_es2_uint shader) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t shad_name = (uintptr_t)shader;
  int is_allocated = ref_range_get_ref_at(&c->shader_rra_, shad_name);
  if (is_allocated) {
    struct gl_es2_shader *shad = NULL;
    shad = (struct gl_es2_shader *)not_find(&c->shader_not_, shad_name);
    if (shad) {
      if (shad->first_program_attached_to_) {
        shad->flagged_for_deletion_ = 1;
      }
      else {
        ref_range_mark_range_free(&c->shader_rra_, shad_name, shad_name + 1);
        not_remove(&c->shader_not_, &shad->no_);
        gl_es2_shader_cleanup(shad);
        free(shad);
      }
    }
    else {
      ref_range_mark_range_free(&c->shader_rra_, shad_name, shad_name + 1);
    }
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DeleteTextures)(gl_es2_sizei n, const gl_es2_uint *textures) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (n < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (!n) return;

  size_t k;
  for (k = 0; k < n; ++k) {
    uintptr_t tex_name = textures[k];
    /* silently ignore 0's */
    if (tex_name) {
      int is_allocated = ref_range_get_ref_at(&c->texture_rra_, tex_name);
      if (is_allocated) {
        struct gl_es2_texture *tex = NULL;
        tex = (struct gl_es2_texture *)not_find(&c->texture_not_, tex_name);
        if (tex) {
          /* texture was bound & created before; release and free it */
          struct gl_es2_texture *default_tex = (struct gl_es2_texture *)not_find(&c->texture_not_, 0);
          size_t m;
          for (m = 0; m < c->num_active_texture_units_; ++m) {
            if (c->active_texture_units_[m].texture_2d_ == tex) {
              c->active_texture_units_[m].texture_2d_ = default_tex;
            }
            else if (c->active_texture_units_[m].texture_cube_map_ == tex) {
              c->active_texture_units_[m].texture_cube_map_ = default_tex;
            }
          }
          not_remove(&c->texture_not_, &tex->no_);
          gl_es2_texture_cleanup(tex);
          free(tex);
        }
        ref_range_mark_range_free(&c->texture_rra_, tex_name, tex_name + 1);
      }
    }
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DepthFunc)(gl_es2_enum func) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DepthMask)(gl_es2_boolean flag) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DepthRangef)(gl_es2_float n, gl_es2_float f) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DetachShader)(gl_es2_uint program, gl_es2_uint shader) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = NULL;
  prog = (struct gl_es2_program *)not_find(&c->program_not_, (uintptr_t)program);
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return ;
  }
  struct gl_es2_shader *shad = NULL;
  shad = (struct gl_es2_shader *)not_find(&c->shader_not_, (uintptr_t)shader);
  if (!shad) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int found = 0;
  if (shad == prog->vertex_shader_.shader_) {
    detach_program_shader_with_cascaded_delete(&prog->vertex_shader_);
    found = 1;
  }
  if (shad == prog->fragment_shader_.shader_) {
    detach_program_shader_with_cascaded_delete(&prog->fragment_shader_);
    found = 1;
  }
  if (!found) set_gl_err(GL_ES2_INVALID_OPERATION); /* shader is not attached to program */
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
  struct gl_es2_context *c = gl_es2_ctx();
  if (target != GL_ES2_FRAMEBUFFER) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }
  if ((renderbuffertarget != GL_ES2_RENDERBUFFER) && renderbuffer) {
    /* renderbuffertarget is not GL_RENDERBUFFER and renderbuffer is not 0 */
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }
  if (!c->framebuffer_) {
    /* In boot-up state, no framebuffer bound, not even default fb 0 */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (c->framebuffer_->no_.name_ == 0) {
    /* The default framebuffer is bound (in which case you're not allowed to set any attachments) */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  struct gl_es2_renderbuffer *rb = NULL;
  if (renderbuffer) {
    rb = (struct gl_es2_renderbuffer *)not_find(&c->renderbuffer_not_, renderbuffer);
    if (!rb) {
      /* renderbuffer is neither 0, nor the name of an existing renderbuffer object */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  switch (attachment) {
    case GL_ES2_COLOR_ATTACHMENT0:
      gl_es2_framebuffer_attachment_attach_renderbuffer(&c->framebuffer_->color_attachment0_, rb);
      break;
    case GL_ES2_DEPTH_ATTACHMENT:
      gl_es2_framebuffer_attachment_attach_renderbuffer(&c->framebuffer_->depth_attachment_, rb);
      break;
    case GL_ES2_STENCIL_ATTACHMENT:
      gl_es2_framebuffer_attachment_attach_renderbuffer(&c->framebuffer_->stencil_attachment_, rb);
      break;
    default:
      /* attachment is not an accepted attachment point */
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(FramebufferTexture2D)(gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum textarget, gl_es2_uint texture, gl_es2_int level) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(FrontFace)(gl_es2_enum mode) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenBuffers)(gl_es2_sizei n, gl_es2_uint *buffers) {
  struct gl_es2_context *c = gl_es2_ctx();
  generic_name_gen(&c->buffer_rra_, n, buffers);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenerateMipmap)(gl_es2_enum target) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenFramebuffers)(gl_es2_sizei n, gl_es2_uint *framebuffers) {
  struct gl_es2_context *c = gl_es2_ctx();
  generic_name_gen(&c->framebuffer_rra_, n, framebuffers);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenRenderbuffers)(gl_es2_sizei n, gl_es2_uint *renderbuffers) {
  struct gl_es2_context *c = gl_es2_ctx();
  generic_name_gen(&c->renderbuffer_rra_, n, renderbuffers);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenTextures)(gl_es2_sizei n, gl_es2_uint *textures) {
  struct gl_es2_context *c = gl_es2_ctx();
  generic_name_gen(&c->texture_rra_, n, textures);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetActiveAttrib)(gl_es2_uint program, gl_es2_uint index, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_int *size, gl_es2_enum *type, gl_es2_char *name) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetActiveUniform)(gl_es2_uint program, gl_es2_uint index, gl_es2_sizei bufSize, gl_es2_sizei *length, gl_es2_int *size, gl_es2_enum *type, gl_es2_char *name) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetAttachedShaders)(gl_es2_uint program, gl_es2_sizei maxCount, gl_es2_sizei *count, gl_es2_uint *shaders) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int num_shaders = 0;
  if (!shaders) maxCount = 0;
  if (prog->vertex_shader_.shader_) {
    if (num_shaders < maxCount) shaders[num_shaders] = (gl_es2_uint)prog->vertex_shader_.shader_->no_.name_;
    num_shaders++;
  }
  if (prog->fragment_shader_.shader_) {
    if (num_shaders < maxCount) shaders[num_shaders] = (gl_es2_uint)prog->vertex_shader_.shader_->no_.name_;
    num_shaders++;
  }
  if (count) *count = (gl_es2_sizei)num_shaders;
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
  struct gl_es2_context *c = gl_es2_ctx();
  if (!c->allow_internals_ && !memcmp(name, "gl_", 3)) {
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
  struct gl_es2_context *c = gl_es2_ctx();
  if (!buffer) return GL_ES2_FALSE;
  /* Note: a name returned by glGenBuffers but not yet associated, is not the name of a buffer object. Consequently,
   * we must use the buffer named object table buffer_not_ and not the ref-range allocator buffer_rra_ to perform this
   * check */
  if (not_find(&c->buffer_not_, buffer)) return GL_ES2_TRUE;
  return GL_ES2_FALSE;
}

GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsEnabled)(gl_es2_enum cap) {
  return GL_ES2_FALSE;
}

GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsFramebuffer)(gl_es2_uint framebuffer) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (!framebuffer) return GL_ES2_FALSE;
  /* Note: a name returned by glGenFramebuffers but not yet associated, is not the name of a framebuffer object. 
   * Consequently, we must use the framebuffer named object table framebuffer_not_ and not the ref-range allocator 
   * framebuffer_rra_ to perform this check */
  if (not_find(&c->framebuffer_not_, framebuffer)) return GL_ES2_TRUE;
  return GL_ES2_FALSE;
}

GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsProgram)(gl_es2_uint program) {
  return GL_ES2_FALSE;
}

GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsRenderbuffer)(gl_es2_uint renderbuffer) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (!renderbuffer) return GL_ES2_FALSE;
  /* Note: a name returned by glGenRenderbuffers but not yet associated, is not the name of a renderbuffer object.
   * Consequently, we must use the renderbuffer named object table renderbuffer_not_ and not the ref-range allocator
   * renderbuffer_rra_ to perform this check */
  if (not_find(&c->renderbuffer_not_, renderbuffer)) return GL_ES2_TRUE;
  return GL_ES2_FALSE;
}

GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsShader)(gl_es2_uint shader) {
  return GL_ES2_FALSE;
}

GL_ES2_DECL_SPEC gl_es2_boolean GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(IsTexture)(gl_es2_uint texture) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (!texture) return GL_ES2_FALSE;
  /* Note: a name returned by glGenTextures but not yet associated, is not the name of a texture object.
   * Consequently, we must use the texture named object table texture_not_ and not the ref-range allocator
   * texture_rra_ to perform this check */
  if (not_find(&c->texture_not_, texture)) return GL_ES2_TRUE;
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
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t prog_name = (uintptr_t)program;
  if (!prog_name) {
    check_old_program_for_deletion(c->current_program_);
    c->current_program_ = NULL;
    return;
  }
  struct gl_es2_program *prog = NULL;
  prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (prog) {
    if (c->current_program_ != prog) check_old_program_for_deletion(c->current_program_);
    c->current_program_ = prog;
  }
  else {
    set_gl_err(GL_ES2_INVALID_VALUE);
  }
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

