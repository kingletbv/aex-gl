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

#ifndef LIMITS_H_INCLUDED
#define LIMITS_H_INCLUDED
#include <limits.h>
#endif

#ifndef FLOAT_H_INCLUDED
#define FLOAT_H_INCLUDED
#include <float.h>
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

static gl_es2_enum stencil_func_pasf_to_gl(primitive_assembly_stencil_func_t pasf) {
  switch (pasf) {
    case PASF_EQUAL: return GL_ES2_EQUAL;
    case PASF_NOTEQUAL: return GL_ES2_NOTEQUAL;
    case PASF_LESS: return GL_ES2_LESS;
    case PASF_GREATER: return GL_ES2_GREATER;
    case PASF_LEQUAL: return GL_ES2_LEQUAL;
    case PASF_GEQUAL: return GL_ES2_GEQUAL;
    case PASF_ALWAYS: return GL_ES2_ALWAYS;
    case PASF_NEVER: return GL_ES2_NEVER;
  }
  return 0;
}

static gl_es2_enum stencil_op_paso_to_gl(primitive_assembly_stencil_op_t paso) {
  switch (paso) {
    case PASO_ZERO:      return GL_ES2_ZERO;
    case PASO_REPLACE:   return GL_ES2_REPLACE;
    case PASO_INCR:      return GL_ES2_INCR;
    case PASO_DECR:      return GL_ES2_DECR;
    case PASO_INCR_WRAP: return GL_ES2_INCR_WRAP;
    case PASO_DECR_WRAP: return GL_ES2_DECR_WRAP;
    case PASO_KEEP:      return GL_ES2_KEEP;
    case PASO_INVERT:    return GL_ES2_INVERT;
  }
  return 0;
}

static int stencil_op_gl_to_paso(gl_es2_enum gl_op, primitive_assembly_stencil_op_t *ppaso) {
  switch (gl_op) {
    case GL_ES2_ZERO: *ppaso = PASO_ZERO; break;
    case GL_ES2_REPLACE: *ppaso = PASO_REPLACE; break;
    case GL_ES2_INCR: *ppaso = PASO_INCR; break;
    case GL_ES2_DECR: *ppaso = PASO_DECR; break;
    case GL_ES2_INCR_WRAP: *ppaso = PASO_INCR_WRAP; break;
    case GL_ES2_DECR_WRAP: *ppaso = PASO_DECR_WRAP; break;
    case GL_ES2_KEEP: *ppaso = PASO_KEEP; break;
    case GL_ES2_INVERT: *ppaso = PASO_INVERT; break;
    default:
      return 0;
  }
  return 1;
}

static int get_tex_target(gl_es2_enum target, struct gl_es2_texture *tex, struct sampler_2d **ps2d) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_texture_unit *tu = &c->active_texture_units_[c->current_active_texture_unit_];
  struct sampler_2d *s2d = NULL;
  switch (target) {
    case GL_ES2_TEXTURE_2D:
      if (tex->kind_ != gl_es2_texture_2d) break;
      s2d = &tex->texture_2d_;
      break;
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Z:
      tex = tu->texture_cube_map_;
      if (tex->kind_ != gl_es2_texture_cube_map) break;
      switch (target) {
        case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_X: s2d = &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_x]; break;
        case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_X: s2d = &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_x]; break;
        case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Y: s2d = &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_y]; break;
        case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Y: s2d = &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_y]; break;
        case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Z: s2d = &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_z]; break;
        case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Z: s2d = &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_z]; break;
      }
      break;
    default:
      /* target is invalid */
      set_gl_err(GL_ES2_INVALID_ENUM);
      return 0;
  }
  if (!s2d) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return 0;
  }
  *ps2d = s2d;
  return 1;
}

static int get_active_tex_target(gl_es2_enum target, struct gl_es2_texture **ptex, struct sampler_2d **ps2d) {
  int r = 0;
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_texture_unit *tu = &c->active_texture_units_[c->current_active_texture_unit_];
  struct gl_es2_texture *tex = NULL;
  struct sampler_2d *s2d = NULL;
  switch (target) {
    case GL_ES2_TEXTURE_2D:
      tex = tu->texture_2d_;
      r = get_tex_target(target, tex, ps2d);
      break;
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Z:
      tex = tu->texture_cube_map_;
      if (tex->kind_ != gl_es2_texture_cube_map) break;
      r = get_tex_target(target, tex, ps2d);
      break;
    default:
      /* target is invalid */
      set_gl_err(GL_ES2_INVALID_ENUM);
      return 0;
  }
  if (r) *ptex = tex;
  return r;
}


static enum gl_es2_cube_map_face tex_target_gl_to_cubemap_face(gl_es2_enum textarget) {
  switch (textarget) {
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_X: return gl_es2_cube_map_positive_x;
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_X: return gl_es2_cube_map_negative_x;
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Y: return gl_es2_cube_map_positive_y;
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Y: return gl_es2_cube_map_negative_y;
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Z: return gl_es2_cube_map_positive_z;
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Z: return gl_es2_cube_map_negative_z;
  }
  return gl_es2_cube_map_positive_x;
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
  if (index >= GL_ES2_IMPL_MAX_NUM_VERTEX_ATTRIBS) {
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
  struct gl_es2_context *c = gl_es2_ctx();

  switch (sfactor) {
    case GL_ES2_ZERO:
    case GL_ES2_ONE:
    case GL_ES2_SRC_COLOR:
    case GL_ES2_ONE_MINUS_SRC_COLOR:
    case GL_ES2_DST_COLOR:
    case GL_ES2_ONE_MINUS_DST_COLOR:
    case GL_ES2_SRC_ALPHA:
    case GL_ES2_ONE_MINUS_SRC_ALPHA:
    case GL_ES2_DST_ALPHA:
    case GL_ES2_ONE_MINUS_DST_ALPHA:
    case GL_ES2_CONSTANT_COLOR:
    case GL_ES2_ONE_MINUS_CONSTANT_COLOR:
    case GL_ES2_CONSTANT_ALPHA:
    case GL_ES2_ONE_MINUS_CONSTANT_ALPHA:
    case GL_ES2_SRC_ALPHA_SATURATE:
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  switch (dfactor) {
    case GL_ES2_ZERO:
    case GL_ES2_ONE:
    case GL_ES2_SRC_COLOR:
    case GL_ES2_ONE_MINUS_SRC_COLOR:
    case GL_ES2_DST_COLOR:
    case GL_ES2_ONE_MINUS_DST_COLOR:
    case GL_ES2_SRC_ALPHA:
    case GL_ES2_ONE_MINUS_SRC_ALPHA:
    case GL_ES2_DST_ALPHA:
    case GL_ES2_ONE_MINUS_DST_ALPHA:
    case GL_ES2_CONSTANT_COLOR:
    case GL_ES2_ONE_MINUS_CONSTANT_COLOR:
    case GL_ES2_CONSTANT_ALPHA:
    case GL_ES2_ONE_MINUS_CONSTANT_ALPHA:
    case GL_ES2_SRC_ALPHA_SATURATE:
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  switch (sfactor) {
    case GL_ES2_ZERO:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_ZERO;
      break;
    case GL_ES2_ONE:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_ONE;
      break;
    case GL_ES2_SRC_COLOR:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_SRC_COLOR;
      break;
    case GL_ES2_ONE_MINUS_SRC_COLOR:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_ONE_MINUS_SRC_COLOR;
      break;
    case GL_ES2_DST_COLOR:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_DST_COLOR;
      break;
    case GL_ES2_ONE_MINUS_DST_COLOR:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_ONE_MINUS_DST_COLOR;
      break;
    case GL_ES2_SRC_ALPHA:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_SRC_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_SRC_ALPHA:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_ONE_MINUS_SRC_ALPHA;
      break;
    case GL_ES2_DST_ALPHA:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_DST_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_DST_ALPHA:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_ONE_MINUS_DST_ALPHA;
      break;
    case GL_ES2_CONSTANT_COLOR:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_CONSTANT_COLOR;
      break;
    case GL_ES2_ONE_MINUS_CONSTANT_COLOR:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_ONE_MINUS_CONSTANT_COLOR;
      break;
    case GL_ES2_CONSTANT_ALPHA:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_CONSTANT_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_CONSTANT_ALPHA:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_ONE_MINUS_CONSTANT_ALPHA;
      break;
    case GL_ES2_SRC_ALPHA_SATURATE:
      c->blend_src_rgb_fn_ = c->blend_src_alpha_fn_ = BF_SRC_ALPHA_SATURATE;
      break;
  }

  switch (dfactor) {
    case GL_ES2_ZERO:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_ZERO;
      break;
    case GL_ES2_ONE:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_ONE;
      break;
    case GL_ES2_SRC_COLOR:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_SRC_COLOR;
      break;
    case GL_ES2_ONE_MINUS_SRC_COLOR:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_ONE_MINUS_SRC_COLOR;
      break;
    case GL_ES2_DST_COLOR:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_DST_COLOR;
      break;
    case GL_ES2_ONE_MINUS_DST_COLOR:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_ONE_MINUS_DST_COLOR;
      break;
    case GL_ES2_SRC_ALPHA:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_SRC_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_SRC_ALPHA:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_ONE_MINUS_SRC_ALPHA;
      break;
    case GL_ES2_DST_ALPHA:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_DST_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_DST_ALPHA:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_ONE_MINUS_DST_ALPHA;
      break;
    case GL_ES2_CONSTANT_COLOR:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_CONSTANT_COLOR;
      break;
    case GL_ES2_ONE_MINUS_CONSTANT_COLOR:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_ONE_MINUS_CONSTANT_COLOR;
      break;
    case GL_ES2_CONSTANT_ALPHA:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_CONSTANT_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_CONSTANT_ALPHA:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_ONE_MINUS_CONSTANT_ALPHA;
      break;
    case GL_ES2_SRC_ALPHA_SATURATE:
      c->blend_dst_rgb_fn_ = c->blend_dst_alpha_fn_ = BF_SRC_ALPHA_SATURATE;
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BlendFuncSeparate)(gl_es2_enum sfactorRGB, gl_es2_enum dfactorRGB, gl_es2_enum sfactorAlpha, gl_es2_enum dfactorAlpha) {
  struct gl_es2_context *c = gl_es2_ctx();

  switch (sfactorRGB) {
    case GL_ES2_ZERO:
    case GL_ES2_ONE:
    case GL_ES2_SRC_COLOR:
    case GL_ES2_ONE_MINUS_SRC_COLOR:
    case GL_ES2_DST_COLOR:
    case GL_ES2_ONE_MINUS_DST_COLOR:
    case GL_ES2_SRC_ALPHA:
    case GL_ES2_ONE_MINUS_SRC_ALPHA:
    case GL_ES2_DST_ALPHA:
    case GL_ES2_ONE_MINUS_DST_ALPHA:
    case GL_ES2_CONSTANT_COLOR:
    case GL_ES2_ONE_MINUS_CONSTANT_COLOR:
    case GL_ES2_CONSTANT_ALPHA:
    case GL_ES2_ONE_MINUS_CONSTANT_ALPHA:
    case GL_ES2_SRC_ALPHA_SATURATE:
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  switch (dfactorRGB) {
    case GL_ES2_ZERO:
    case GL_ES2_ONE:
    case GL_ES2_SRC_COLOR:
    case GL_ES2_ONE_MINUS_SRC_COLOR:
    case GL_ES2_DST_COLOR:
    case GL_ES2_ONE_MINUS_DST_COLOR:
    case GL_ES2_SRC_ALPHA:
    case GL_ES2_ONE_MINUS_SRC_ALPHA:
    case GL_ES2_DST_ALPHA:
    case GL_ES2_ONE_MINUS_DST_ALPHA:
    case GL_ES2_CONSTANT_COLOR:
    case GL_ES2_ONE_MINUS_CONSTANT_COLOR:
    case GL_ES2_CONSTANT_ALPHA:
    case GL_ES2_ONE_MINUS_CONSTANT_ALPHA:
    case GL_ES2_SRC_ALPHA_SATURATE:
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  switch (sfactorAlpha) {
    case GL_ES2_ZERO:
    case GL_ES2_ONE:
    case GL_ES2_SRC_COLOR:
    case GL_ES2_ONE_MINUS_SRC_COLOR:
    case GL_ES2_DST_COLOR:
    case GL_ES2_ONE_MINUS_DST_COLOR:
    case GL_ES2_SRC_ALPHA:
    case GL_ES2_ONE_MINUS_SRC_ALPHA:
    case GL_ES2_DST_ALPHA:
    case GL_ES2_ONE_MINUS_DST_ALPHA:
    case GL_ES2_CONSTANT_COLOR:
    case GL_ES2_ONE_MINUS_CONSTANT_COLOR:
    case GL_ES2_CONSTANT_ALPHA:
    case GL_ES2_ONE_MINUS_CONSTANT_ALPHA:
    case GL_ES2_SRC_ALPHA_SATURATE:
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  switch (dfactorAlpha) {
    case GL_ES2_ZERO:
    case GL_ES2_ONE:
    case GL_ES2_SRC_COLOR:
    case GL_ES2_ONE_MINUS_SRC_COLOR:
    case GL_ES2_DST_COLOR:
    case GL_ES2_ONE_MINUS_DST_COLOR:
    case GL_ES2_SRC_ALPHA:
    case GL_ES2_ONE_MINUS_SRC_ALPHA:
    case GL_ES2_DST_ALPHA:
    case GL_ES2_ONE_MINUS_DST_ALPHA:
    case GL_ES2_CONSTANT_COLOR:
    case GL_ES2_ONE_MINUS_CONSTANT_COLOR:
    case GL_ES2_CONSTANT_ALPHA:
    case GL_ES2_ONE_MINUS_CONSTANT_ALPHA:
    case GL_ES2_SRC_ALPHA_SATURATE:
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  switch (sfactorRGB) {
    case GL_ES2_ZERO:
      c->blend_src_rgb_fn_ = BF_ZERO;
      break;
    case GL_ES2_ONE:
      c->blend_src_rgb_fn_ = BF_ONE;
      break;
    case GL_ES2_SRC_COLOR:
      c->blend_src_rgb_fn_ = BF_SRC_COLOR;
      break;
    case GL_ES2_ONE_MINUS_SRC_COLOR:
      c->blend_src_rgb_fn_ = BF_ONE_MINUS_SRC_COLOR;
      break;
    case GL_ES2_DST_COLOR:
      c->blend_src_rgb_fn_ = BF_DST_COLOR;
      break;
    case GL_ES2_ONE_MINUS_DST_COLOR:
      c->blend_src_rgb_fn_ = BF_ONE_MINUS_DST_COLOR;
      break;
    case GL_ES2_SRC_ALPHA:
      c->blend_src_rgb_fn_ = BF_SRC_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_SRC_ALPHA:
      c->blend_src_rgb_fn_ = BF_ONE_MINUS_SRC_ALPHA;
      break;
    case GL_ES2_DST_ALPHA:
      c->blend_src_rgb_fn_ = BF_DST_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_DST_ALPHA:
      c->blend_src_rgb_fn_ = BF_ONE_MINUS_DST_ALPHA;
      break;
    case GL_ES2_CONSTANT_COLOR:
      c->blend_src_rgb_fn_ = BF_CONSTANT_COLOR;
      break;
    case GL_ES2_ONE_MINUS_CONSTANT_COLOR:
      c->blend_src_rgb_fn_ = BF_ONE_MINUS_CONSTANT_COLOR;
      break;
    case GL_ES2_CONSTANT_ALPHA:
      c->blend_src_rgb_fn_ = BF_CONSTANT_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_CONSTANT_ALPHA:
      c->blend_src_rgb_fn_ = BF_ONE_MINUS_CONSTANT_ALPHA;
      break;
    case GL_ES2_SRC_ALPHA_SATURATE:
      c->blend_src_rgb_fn_ = BF_SRC_ALPHA_SATURATE;
      break;
  }

  switch (sfactorAlpha) {
    case GL_ES2_ZERO:
      c->blend_src_alpha_fn_ = BF_ZERO;
      break;
    case GL_ES2_ONE:
      c->blend_src_alpha_fn_ = BF_ONE;
      break;
    case GL_ES2_SRC_COLOR:
      c->blend_src_alpha_fn_ = BF_SRC_COLOR;
      break;
    case GL_ES2_ONE_MINUS_SRC_COLOR:
      c->blend_src_alpha_fn_ = BF_ONE_MINUS_SRC_COLOR;
      break;
    case GL_ES2_DST_COLOR:
      c->blend_src_alpha_fn_ = BF_DST_COLOR;
      break;
    case GL_ES2_ONE_MINUS_DST_COLOR:
      c->blend_src_alpha_fn_ = BF_ONE_MINUS_DST_COLOR;
      break;
    case GL_ES2_SRC_ALPHA:
      c->blend_src_alpha_fn_ = BF_SRC_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_SRC_ALPHA:
      c->blend_src_alpha_fn_ = BF_ONE_MINUS_SRC_ALPHA;
      break;
    case GL_ES2_DST_ALPHA:
      c->blend_src_alpha_fn_ = BF_DST_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_DST_ALPHA:
      c->blend_src_alpha_fn_ = BF_ONE_MINUS_DST_ALPHA;
      break;
    case GL_ES2_CONSTANT_COLOR:
      c->blend_src_alpha_fn_ = BF_CONSTANT_COLOR;
      break;
    case GL_ES2_ONE_MINUS_CONSTANT_COLOR:
      c->blend_src_alpha_fn_ = BF_ONE_MINUS_CONSTANT_COLOR;
      break;
    case GL_ES2_CONSTANT_ALPHA:
      c->blend_src_alpha_fn_ = BF_CONSTANT_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_CONSTANT_ALPHA:
      c->blend_src_alpha_fn_ = BF_ONE_MINUS_CONSTANT_ALPHA;
      break;
    case GL_ES2_SRC_ALPHA_SATURATE:
      c->blend_src_alpha_fn_ = BF_SRC_ALPHA_SATURATE;
      break;
  }

  switch (dfactorRGB) {
    case GL_ES2_ZERO:
      c->blend_dst_rgb_fn_ = BF_ZERO;
      break;
    case GL_ES2_ONE:
      c->blend_dst_rgb_fn_ = BF_ONE;
      break;
    case GL_ES2_SRC_COLOR:
      c->blend_dst_rgb_fn_ = BF_SRC_COLOR;
      break;
    case GL_ES2_ONE_MINUS_SRC_COLOR:
      c->blend_dst_rgb_fn_ = BF_ONE_MINUS_SRC_COLOR;
      break;
    case GL_ES2_DST_COLOR:
      c->blend_dst_rgb_fn_ = BF_DST_COLOR;
      break;
    case GL_ES2_ONE_MINUS_DST_COLOR:
      c->blend_dst_rgb_fn_ = BF_ONE_MINUS_DST_COLOR;
      break;
    case GL_ES2_SRC_ALPHA:
      c->blend_dst_rgb_fn_ = BF_SRC_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_SRC_ALPHA:
      c->blend_dst_rgb_fn_ = BF_ONE_MINUS_SRC_ALPHA;
      break;
    case GL_ES2_DST_ALPHA:
      c->blend_dst_rgb_fn_ = BF_DST_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_DST_ALPHA:
      c->blend_dst_rgb_fn_ = BF_ONE_MINUS_DST_ALPHA;
      break;
    case GL_ES2_CONSTANT_COLOR:
      c->blend_dst_rgb_fn_ = BF_CONSTANT_COLOR;
      break;
    case GL_ES2_ONE_MINUS_CONSTANT_COLOR:
      c->blend_dst_rgb_fn_ = BF_ONE_MINUS_CONSTANT_COLOR;
      break;
    case GL_ES2_CONSTANT_ALPHA:
      c->blend_dst_rgb_fn_ = BF_CONSTANT_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_CONSTANT_ALPHA:
      c->blend_dst_rgb_fn_ = BF_ONE_MINUS_CONSTANT_ALPHA;
      break;
    case GL_ES2_SRC_ALPHA_SATURATE:
      c->blend_dst_rgb_fn_ = BF_SRC_ALPHA_SATURATE;
      break;
  }

  switch (dfactorAlpha) {
    case GL_ES2_ZERO:
      c->blend_dst_alpha_fn_ = BF_ZERO;
      break;
    case GL_ES2_ONE:
      c->blend_dst_alpha_fn_ = BF_ONE;
      break;
    case GL_ES2_SRC_COLOR:
      c->blend_dst_alpha_fn_ = BF_SRC_COLOR;
      break;
    case GL_ES2_ONE_MINUS_SRC_COLOR:
      c->blend_dst_alpha_fn_ = BF_ONE_MINUS_SRC_COLOR;
      break;
    case GL_ES2_DST_COLOR:
      c->blend_dst_alpha_fn_ = BF_DST_COLOR;
      break;
    case GL_ES2_ONE_MINUS_DST_COLOR:
      c->blend_dst_alpha_fn_ = BF_ONE_MINUS_DST_COLOR;
      break;
    case GL_ES2_SRC_ALPHA:
      c->blend_dst_alpha_fn_ = BF_SRC_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_SRC_ALPHA:
      c->blend_dst_alpha_fn_ = BF_ONE_MINUS_SRC_ALPHA;
      break;
    case GL_ES2_DST_ALPHA:
      c->blend_dst_alpha_fn_ = BF_DST_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_DST_ALPHA:
      c->blend_dst_alpha_fn_ = BF_ONE_MINUS_DST_ALPHA;
      break;
    case GL_ES2_CONSTANT_COLOR:
      c->blend_dst_alpha_fn_ = BF_CONSTANT_COLOR;
      break;
    case GL_ES2_ONE_MINUS_CONSTANT_COLOR:
      c->blend_dst_alpha_fn_ = BF_ONE_MINUS_CONSTANT_COLOR;
      break;
    case GL_ES2_CONSTANT_ALPHA:
      c->blend_dst_alpha_fn_ = BF_CONSTANT_ALPHA;
      break;
    case GL_ES2_ONE_MINUS_CONSTANT_ALPHA:
      c->blend_dst_alpha_fn_ = BF_ONE_MINUS_CONSTANT_ALPHA;
      break;
    case GL_ES2_SRC_ALPHA_SATURATE:
      c->blend_dst_alpha_fn_ = BF_SRC_ALPHA_SATURATE;
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BufferData)(gl_es2_enum target, gl_es2_sizeiptr size, const void *data, gl_es2_enum usage) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_buffer *buf = NULL;
  switch (target) {
    case GL_ES2_ARRAY_BUFFER:
      buf = c->array_buffer_;
      break;
    case GL_ES2_ELEMENT_ARRAY_BUFFER:
      buf = c->element_array_buffer_;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
  if (!buf) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  switch (usage) {
    case GL_ES2_STREAM_DRAW:
    case GL_ES2_STATIC_DRAW:
    case GL_ES2_DYNAMIC_DRAW:
      /* this is fine */
      break;
    default:
      /* this is not */
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
  if (data_buffer_set_data(&buf->buf_, size, data)) {
    /* no memory */
    set_gl_err(GL_ES2_OUT_OF_MEMORY);
    return;
  }
  buf->usage_ = usage;

  return;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(BufferSubData)(gl_es2_enum target, gl_es2_intptr offset, gl_es2_sizeiptr size, const void *data) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_buffer *buf = NULL;
  switch (target) {
    case GL_ES2_ARRAY_BUFFER:
      buf = c->array_buffer_;
      break;
    case GL_ES2_ELEMENT_ARRAY_BUFFER:
      buf = c->element_array_buffer_;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
  if (!buf) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if ((offset < 0) || (size < 0)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  uintptr_t end_of_range = offset + size;
  if (end_of_range < (uintptr_t)offset) {
    /* overflow */
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (((size_t)offset) > buf->buf_.size_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (((size_t)end_of_range) > buf->buf_.size_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (!data) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  data_buffer_copy_data(&buf->buf_, (size_t)offset, (size_t)size, data);
}

GL_ES2_DECL_SPEC gl_es2_enum GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CheckFramebufferStatus)(gl_es2_enum target) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (target != GL_ES2_FRAMEBUFFER) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return 0;
  }

  if (!c->framebuffer_) {
    /* Initialization failure, should always have a framebuffer set, the default window-system-provided framebuffer
     * of "0" is an actual framebuffer that should have been initialized. framebuffer_ should therefore never be NULL */
    set_gl_err(GL_ES2_INVALID_FRAMEBUFFER_OPERATION);
    return 0;
  }

  switch (gl_es2_framebuffer_check_completeness(c->framebuffer_)) {
    case gl_es2_framebuffer_complete:
      return GL_ES2_FRAMEBUFFER_COMPLETE;
    case gl_es2_framebuffer_incomplete_attachment:
      return GL_ES2_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
    case gl_es2_framebuffer_incomplete_dimensions:
      return GL_ES2_FRAMEBUFFER_INCOMPLETE_DIMENSIONS;
    case gl_es2_framebuffer_incomplete_missing_attachment:
      return GL_ES2_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT;
    case gl_es2_framebuffer_incomplete_unsupported:
      return GL_ES2_FRAMEBUFFER_UNSUPPORTED;
  }
  set_gl_err(GL_ES2_INVALID_FRAMEBUFFER_OPERATION);
  return 0;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Clear)(gl_es2_bitfield mask) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (gl_es2_framebuffer_complete != gl_es2_framebuffer_check_completeness(c->framebuffer_)) {
    set_gl_err(GL_ES2_INVALID_FRAMEBUFFER_OPERATION);
    return;
  }
  uint32_t clear_rect_left;
  uint32_t clear_rect_top;
  uint32_t clear_rect_right;
  uint32_t clear_rect_bottom;
  gl_es2_ctx_get_normalized_scissor_rect(c, &clear_rect_left, &clear_rect_top, &clear_rect_right, &clear_rect_bottom);
  uint32_t clear_rect_width = clear_rect_right - clear_rect_left;
  uint32_t clear_rect_height = clear_rect_bottom - clear_rect_top;

  if (mask & GL_ES2_COLOR_BUFFER_BIT) {
    void *bitmap;
    size_t stride;
    gl_es2_framebuffer_attachment_raw_ptr(&c->framebuffer_->color_attachment0_, &bitmap, &stride);
    if (bitmap) {
      if (c->framebuffer_->color_attachment0_.kind_ == gl_es2_faot_renderbuffer) {
        struct gl_es2_renderbuffer *rb = c->framebuffer_->color_attachment0_.v_.rb_;
        switch (rb->format_) {
          case gl_es2_renderbuffer_format_rgba32:
            blitter_blit_apply_mask4x8(bitmap, stride, 
                                       c->red_mask_ ? 0xFF : 0x00, 
                                       c->green_mask_ ? 0xFF : 0x00,
                                       c->blue_mask_ ? 0xFF : 0x00,
                                       c->alpha_mask_ ? 0xFF : 0x00,
                                       c->clear_color_red_,
                                       c->clear_color_grn_,
                                       c->clear_color_blu_,
                                       c->clear_color_alpha_,
                                       clear_rect_left, clear_rect_top, 
                                       clear_rect_width, clear_rect_height);
            break;
        }
      }
      else if (c->framebuffer_->color_attachment0_.kind_ == gl_es2_faot_texture) {
        struct gl_es2_texture *tex = c->framebuffer_->color_attachment0_.v_.tex_;
        switch (tex->texture_2d_.mipmaps_[0].components_) {
          case s2d_rgb:
            blitter_blit_apply_mask3x8(bitmap, stride, 
                                       c->red_mask_ ? 0xFF : 0x00, 
                                       c->green_mask_ ? 0xFF : 0x00,
                                       c->blue_mask_ ? 0xFF : 0x00,
                                       c->clear_color_red_,
                                       c->clear_color_grn_,
                                       c->clear_color_blu_,
                                       clear_rect_left, clear_rect_top, 
                                       clear_rect_width, clear_rect_height);
            break;
          case s2d_rgba:
            blitter_blit_apply_mask4x8(bitmap, stride, 
                                       c->red_mask_ ? 0xFF : 0x00, 
                                       c->green_mask_ ? 0xFF : 0x00,
                                       c->blue_mask_ ? 0xFF : 0x00,
                                       c->alpha_mask_ ? 0xFF : 0x00,
                                       c->clear_color_red_,
                                       c->clear_color_grn_,
                                       c->clear_color_blu_,
                                       c->clear_color_alpha_,
                                       clear_rect_left, clear_rect_top, 
                                       clear_rect_width, clear_rect_height);
            break;
        }
      }
      else {
        /* Not sure how we got a bitmap yet still got here */
        set_gl_err(GL_ES2_INVALID_FRAMEBUFFER_OPERATION);
        return;
      }
    }
  }
  if (mask & GL_ES2_DEPTH_BUFFER_BIT) {
    void *bitmap;
    size_t stride;
    gl_es2_framebuffer_attachment_raw_ptr(&c->framebuffer_->depth_attachment_, &bitmap, &stride);
    if (bitmap) {
      float clear_depth_f = c->clear_depth_;
      if (clear_depth_f < 0.f) clear_depth_f = 0.f;
      if (clear_depth_f > 1.f) clear_depth_f = 1.f;

      if (c->framebuffer_->depth_attachment_.kind_ == gl_es2_faot_renderbuffer) {
        struct gl_es2_renderbuffer *rb = c->framebuffer_->depth_attachment_.v_.rb_;
        switch (rb->format_) {
          case gl_es2_renderbuffer_format_depth16: {
            uint16_t clear_depth16;
            uint32_t cd = (uint32_t)floorf(clear_depth_f * 0x10000);
            clear_depth16 = ((uint16_t)cd) - (uint16_t)(cd >> 16);
            blitter_blit_apply_mask16(bitmap, stride, c->depth_mask_ ? 0xFFFF : 0x0000, clear_depth16,
                                      clear_rect_left, clear_rect_top, 
                                      clear_rect_width, clear_rect_height);
                                      
            break;
          }
          case gl_es2_renderbuffer_format_depth32: {
            uint32_t clear_depth32;
            uint64_t cd = (uint64_t)floorf(clear_depth_f * 0x10000);
            clear_depth32 = ((uint32_t)cd) - (uint32_t)(cd >> 32);

            blitter_blit_apply_mask32(bitmap, stride, c->depth_mask_ ? 0xFFFFFFFF : 0x00000000, clear_depth32,
                                      clear_rect_left, clear_rect_top, 
                                      clear_rect_width, clear_rect_height);
            break;
          }
        }
      }
      else if (c->framebuffer_->depth_attachment_.kind_ == gl_es2_faot_texture) {
        /* Not sure how we got a depth texture given we don't support it yet?
         * (Would expect framebuffer to be incomplete and not get here.) */
        set_gl_err(GL_ES2_INVALID_FRAMEBUFFER_OPERATION);
        return;
      }
      else {
        /* Not sure how we got a bitmap yet still got here */
        set_gl_err(GL_ES2_INVALID_FRAMEBUFFER_OPERATION);
        return;
      }
    }
  }

  if (mask & GL_ES2_STENCIL_BUFFER_BIT) {
    void *bitmap;
    size_t stride;
    gl_es2_framebuffer_attachment_raw_ptr(&c->framebuffer_->stencil_attachment_, &bitmap, &stride);
    if (bitmap) {
      if (c->framebuffer_->depth_attachment_.kind_ == gl_es2_faot_renderbuffer) {
        struct gl_es2_renderbuffer *rb = c->framebuffer_->stencil_attachment_.v_.rb_;
        switch (rb->format_) {
          case gl_es2_renderbuffer_format_stencil16: {
            blitter_blit_apply_mask16(bitmap, stride, (uint16_t)c->stencil_writemask_, c->clear_stencil_,
                                      clear_rect_left, clear_rect_top,
                                      clear_rect_width, clear_rect_height);

            break;
          }
        }
      }
      else if (c->framebuffer_->depth_attachment_.kind_ == gl_es2_faot_texture) {
        /* Not sure how we got a stencil texture given we don't support it yet?
         * (Would expect framebuffer to be incomplete and not get here.) */
        set_gl_err(GL_ES2_INVALID_FRAMEBUFFER_OPERATION);
        return;
      }
      else {
        /* Not sure how we got a bitmap yet still got here */
        set_gl_err(GL_ES2_INVALID_FRAMEBUFFER_OPERATION);
        return;
      }
    }
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ClearColor)(gl_es2_float red, gl_es2_float green, gl_es2_float blue, gl_es2_float alpha) {
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
  c->clear_color_red_ = ((uint8_t)ured) - (uint8_t)(ured >> 8);
  c->clear_color_grn_ = ((uint8_t)ugreen) - (uint8_t)(ugreen >> 8);
  c->clear_color_blu_ = ((uint8_t)ublue) - (uint8_t)(ublue >> 8);
  c->clear_color_alpha_ = ((uint8_t)ualpha) - (uint8_t)(ualpha >> 8);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ClearDepthf)(gl_es2_float d) {
  struct gl_es2_context *c = gl_es2_ctx();
  /* Clamping occurs inside glClear() */
  c->clear_depth_ = d;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ClearStencil)(gl_es2_int s) {
  struct gl_es2_context *c = gl_es2_ctx();
  c->clear_stencil_ = (uint16_t)s;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ColorMask)(gl_es2_boolean red, gl_es2_boolean green, gl_es2_boolean blue, gl_es2_boolean alpha) {
  struct gl_es2_context *c = gl_es2_ctx();
  c->red_mask_ = !!red;
  c->green_mask_ = !!green;
  c->blue_mask_ = !!blue;
  c->alpha_mask_ = !!alpha;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CompileShader)(gl_es2_uint shader) {
  struct gl_es2_context *c = gl_es2_ctx();

  uintptr_t shad_name = (uintptr_t)shader;
  struct gl_es2_shader *shad = NULL;
  shad = (struct gl_es2_shader *)not_find(&c->shader_not_, shad_name);
  if (!shad) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  shad->compilation_status_ = 0;

  int r;
  r = sl_shader_compile(&shad->shader_);
  switch (r) {
    case SL_ERR_INVALID_ARG:
    case SL_ERR_INTERNAL:
      dx_error(&shad->shader_.gl_info_log_.dx_, "An internal error occurred\n");
      return;
    case SL_ERR_NO_MEM:
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    case SL_ERR_OK:
      break;
  }

  /* check that we have a main function */
  struct sl_function *f;
  f = sl_compilation_unit_find_function(&shad->shader_.cu_, "main");
  if (!f) {
    dx_error(&shad->shader_.gl_info_log_.dx_, "Error: shader does not have a main function\n");
    return;
  }

  if (f->num_parameters_) {
    dx_error(&shad->shader_.gl_info_log_.dx_, "Error: shader main() function should not have parameters\n");
    return;
  }

  if (f->return_type_->kind_ != sltk_void) {
    dx_error(&shad->shader_.gl_info_log_.dx_, "Error: shader main() function should have 'void' return type\n");
    return;
  }

  shad->compilation_status_ = 1;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CompressedTexImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_enum internalformat, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border, gl_es2_sizei imageSize, const void *data) {
  /* Don't support compressed textures */
  set_gl_err(GL_ES2_INVALID_ENUM);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CompressedTexSubImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_int xoffset, gl_es2_int yoffset, gl_es2_sizei width, gl_es2_sizei height, gl_es2_enum format, gl_es2_sizei imageSize, const void *data) {
  /* Don't support compressed textures */
  set_gl_err(GL_ES2_INVALID_ENUM);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CopyTexImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_enum internalformat, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_texture *tex = NULL;
  struct sampler_2d *s2d = NULL;;
  struct gl_es2_texture_unit *tu = &c->active_texture_units_[c->current_active_texture_unit_];
  if ((level < 0) || (level >= SAMPLER_2D_MAX_NUM_MIPMAPS)) {
    /* level is not in range */
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (border) {
    /* border is not 0 */
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (!get_active_tex_target(target, &tex, &s2d)) {
    return;
  }
  if ((tex->kind_ == gl_es2_texture_cube_map) && (width != height)) {
    /* target is one of the six cube map 2D image targets and the width and height parameters are not equal. */
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  if (gl_es2_framebuffer_check_completeness(c->framebuffer_) != gl_es2_framebuffer_complete) {
    set_gl_err(GL_ES2_INVALID_FRAMEBUFFER_OPERATION);
    return;
  }

  enum blitter_format src_format;

  if (c->framebuffer_->color_attachment0_.kind_ == gl_es2_faot_none) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  else if (c->framebuffer_->color_attachment0_.kind_ == gl_es2_faot_renderbuffer) {
    struct gl_es2_renderbuffer *rb = c->framebuffer_->color_attachment0_.v_.rb_;
    if (rb->format_ == gl_es2_renderbuffer_format_rgba32) {
      src_format = blit_format_rgba;
    }
    else {
      /* invalid renderbuffer framebuffer attachment */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  else if (c->framebuffer_->color_attachment0_.kind_ == gl_es2_faot_texture) {
    struct gl_es2_texture *tex = c->framebuffer_->color_attachment0_.v_.tex_;
    struct sampler_2d *src = NULL;
    if (tex->kind_ == gl_es2_texture_2d) {
      src = &tex->texture_2d_;
    }
    else if (tex->kind_ == gl_es2_texture_cube_map) {
      switch (c->framebuffer_->color_attachment0_.cube_map_face_) {
        case gl_es2_cube_map_positive_x: src = &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_x]; break;
        case gl_es2_cube_map_negative_x: src = &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_x]; break;
        case gl_es2_cube_map_positive_y: src = &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_y]; break;
        case gl_es2_cube_map_negative_y: src = &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_y]; break;
        case gl_es2_cube_map_positive_z: src = &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_z]; break;
        case gl_es2_cube_map_negative_z: src = &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_z]; break;
      }
    }
    if (!src) {
      /* invalid texture framebuffer attachment */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    switch (src->components_) {
      case s2d_alpha:           src_format = blit_format_alpha; break;
      case s2d_luminance:       src_format = blit_format_alpha; break;
      case s2d_luminance_alpha: src_format = blit_format_luminance_alpha; break;
      case s2d_rgb:             src_format = blit_format_rgb; break;
      case s2d_rgba:            src_format = blit_format_rgba; break;
      default:
        set_gl_err(GL_ES2_INVALID_OPERATION);
        return;
    }
  }
  else {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  int r;
  enum s2d_tex_components s2d_format;
  enum blitter_format dst_format;
  switch (internalformat) {
    case GL_ES2_ALPHA:
      s2d_format = s2d_alpha;
      dst_format = blit_format_alpha;
      break;
    case GL_ES2_LUMINANCE:
      s2d_format = s2d_luminance;
      dst_format = blit_format_luminance;
      break;
    case GL_ES2_LUMINANCE_ALPHA:
      s2d_format = s2d_luminance_alpha;
      dst_format = blit_format_luminance_alpha;
      break;
    case GL_ES2_RGB:
      s2d_format = s2d_rgb;
      dst_format = blit_format_rgb;
      break;
    case GL_ES2_RGBA:
      s2d_format = s2d_rgba;
      dst_format = blit_format_rgba;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  if ((width < 0) || (height < 0)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  r = sampler_2d_set_storage(s2d, level, s2d_format, width, height);
  if (r == SL_ERR_NO_MEM) {
    set_gl_err(GL_ES2_OUT_OF_MEMORY);
    return;
  }
  else if (r == SL_ERR_INVALID_ARG) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  void *src_ptr;
  size_t src_stride;

  gl_es2_framebuffer_attachment_raw_ptr(&c->framebuffer_->color_attachment0_, &src_ptr, &src_stride);
  blitter_blit_format(s2d->mipmaps_[level].bitmap_, dst_format, src_ptr, src_format,
                      s2d->mipmaps_[level].num_bytes_per_bitmap_row_, 0, 0,
                      src_stride, x, gl_es2_framebuffer_get_bitmap_row_num(c->framebuffer_, y + height), width, height);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CopyTexSubImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_int xoffset, gl_es2_int yoffset, gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_texture *tex = NULL;
  struct sampler_2d *s2d = NULL;;
  struct gl_es2_texture_unit *tu = &c->active_texture_units_[c->current_active_texture_unit_];
  if ((level < 0) || (level >= SAMPLER_2D_MAX_NUM_MIPMAPS)) {
    /* level is not in range */
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  switch (target) {
    case GL_ES2_TEXTURE_2D:
      tex = tu->texture_2d_;
      if (tex->kind_ != gl_es2_texture_2d) break;
      s2d = &tex->texture_2d_;
      break;
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_X:
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_X:
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Y:
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Y:
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Z:
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Z:
      tex = tu->texture_cube_map_;
      if (tex->kind_ != gl_es2_texture_cube_map) break;
      if (width != height) {
        /* target is one of the six cube map 2D image targets and the width and height parameters are not equal. */
        set_gl_err(GL_ES2_INVALID_VALUE);
        return;
      }
      switch (target) {
        case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_X: s2d = &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_x]; break;
        case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_X: s2d = &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_x]; break;
        case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Y: s2d = &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_y]; break;
        case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Y: s2d = &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_y]; break;
        case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Z: s2d = &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_z]; break;
        case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Z: s2d = &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_z]; break;
      }
      break;
    default:
      /* target is invalid */
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
  if (!s2d) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (level >= s2d->num_maps_) {
    /* texture array not yet created by glTexImage2D().. or CopyTexImage2D().. */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (gl_es2_framebuffer_check_completeness(c->framebuffer_) != gl_es2_framebuffer_complete) {
    set_gl_err(GL_ES2_INVALID_FRAMEBUFFER_OPERATION);
    return;
  }

  enum blitter_format src_format;
  if (c->framebuffer_->color_attachment0_.kind_ == gl_es2_faot_none) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  else if (c->framebuffer_->color_attachment0_.kind_ == gl_es2_faot_renderbuffer) {
    struct gl_es2_renderbuffer *rb = c->framebuffer_->color_attachment0_.v_.rb_;
    if (rb->format_ == gl_es2_renderbuffer_format_rgba32) {
      src_format = blit_format_rgba;
    }
    else {
      /* invalid renderbuffer framebuffer attachment */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  else if (c->framebuffer_->color_attachment0_.kind_ == gl_es2_faot_texture) {
    struct gl_es2_texture *tex = c->framebuffer_->color_attachment0_.v_.tex_;
    struct sampler_2d *src = NULL;
    if (tex->kind_ == gl_es2_texture_2d) {
      src = &tex->texture_2d_;
    }
    else if (tex->kind_ == gl_es2_texture_cube_map) {
      switch (c->framebuffer_->color_attachment0_.cube_map_face_) {
        case gl_es2_cube_map_positive_x: src = &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_x]; break;
        case gl_es2_cube_map_negative_x: src = &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_x]; break;
        case gl_es2_cube_map_positive_y: src = &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_y]; break;
        case gl_es2_cube_map_negative_y: src = &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_y]; break;
        case gl_es2_cube_map_positive_z: src = &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_z]; break;
        case gl_es2_cube_map_negative_z: src = &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_z]; break;
      }
    }
    if (!src) {
      /* invalid texture framebuffer attachment */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    switch (src->components_) {
      case s2d_alpha:           src_format = blit_format_alpha; break;
      case s2d_luminance:       src_format = blit_format_alpha; break;
      case s2d_luminance_alpha: src_format = blit_format_luminance_alpha; break;
      case s2d_rgb:             src_format = blit_format_rgb; break;
      case s2d_rgba:            src_format = blit_format_rgba; break;
      default:
        set_gl_err(GL_ES2_INVALID_OPERATION);
        return;
    }
  }
  else {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  enum blitter_format dst_format;
  switch (s2d->mipmaps_[level].components_) {
    case s2d_alpha:
      dst_format = blit_format_alpha;
      break;
    case s2d_luminance:
      dst_format = blit_format_luminance;
      break;
    case s2d_luminance_alpha:
      dst_format = blit_format_luminance_alpha;
      break;
    case s2d_rgb:
      dst_format = blit_format_rgb;
      break;
    case s2d_rgba:
      dst_format = blit_format_rgba;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  if ((x < 0) || (y < 0)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  if ((width < 0) || (height < 0)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  if (((x + width) > s2d->mipmaps_[level].width_) ||
      ((y + height) > s2d->mipmaps_[level].height_)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  void *src_ptr;
  size_t src_stride;

  gl_es2_framebuffer_attachment_raw_ptr(&c->framebuffer_->color_attachment0_, &src_ptr, &src_stride);
  blitter_blit_format(s2d->mipmaps_[level].bitmap_, dst_format, src_ptr, src_format,
                      s2d->mipmaps_[level].num_bytes_per_bitmap_row_, 0, 0,
                      src_stride, x, gl_es2_framebuffer_get_bitmap_row_num(c->framebuffer_, y + height), width, height);

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

  switch (type) {
    case GL_ES2_VERTEX_SHADER:
      sl_shader_set_type(&shad->shader_, SLST_VERTEX_SHADER);
      break;
    case GL_ES2_FRAGMENT_SHADER:
      sl_shader_set_type(&shad->shader_, SLST_FRAGMENT_SHADER);
      break;
  }

  return shad_name;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(CullFace)(gl_es2_enum mode) {
  struct gl_es2_context *c = gl_es2_ctx();
  switch (mode) {
    case GL_ES2_FRONT:
      c->cull_face_mode_ = gl_es2_cull_face_front;
      break;
    case GL_ES2_BACK:
      c->cull_face_mode_ = gl_es2_cull_face_back;
      break;
    case GL_ES2_FRONT_AND_BACK:
      c->cull_face_mode_ = gl_es2_cull_face_front_and_back;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
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
  struct gl_es2_context *c = gl_es2_ctx();
  primitive_assembly_zbuf_func_t zbuf_func;
  switch (func) {
    case GL_ES2_NEVER:    zbuf_func = PAZF_NEVER; break;
    case GL_ES2_LESS:     zbuf_func = PAZF_LESS; break;
    case GL_ES2_EQUAL:    zbuf_func = PAZF_EQUAL; break;
    case GL_ES2_LEQUAL:   zbuf_func = PAZF_LEQUAL; break;
    case GL_ES2_GREATER:  zbuf_func = PAZF_GREATER; break;
    case GL_ES2_NOTEQUAL: zbuf_func = PAZF_NOTEQUAL; break;
    case GL_ES2_GEQUAL:   zbuf_func = PAZF_GEQUAL; break;
    case GL_ES2_ALWAYS:   zbuf_func = PAZF_ALWAYS; break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
  c->zbuf_func_ = zbuf_func;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DepthMask)(gl_es2_boolean flag) {
  struct gl_es2_context *c = gl_es2_ctx();
  c->depth_mask_ = !!flag;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DepthRangef)(gl_es2_float n, gl_es2_float f) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (n < 0.f) n = 0.f;
  if (n > 1.f) n = 1.f;
  if (f < 0.f) f = 0.f;
  if (f > 1.f) f = 1.f;
  c->near_plane_ = n;
  c->far_plane_ = f;
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
  struct gl_es2_context *c = gl_es2_ctx();
  switch (cap) {
    case GL_ES2_BLEND:
      c->is_blend_enabled_ = 0;
      break;
    case GL_ES2_CULL_FACE:
      c->is_culling_enabled_ = 0;
      break;
    case GL_ES2_DEPTH_TEST:
      c->is_depth_test_enabled_ = 0;
      break;
    case GL_ES2_DITHER:
      c->is_dither_enabled_ = 0;
      break;
    case GL_ES2_POLYGON_OFFSET_FILL:
      c->is_polygon_offset_fill_enabled_ = 0;
      break;
    case GL_ES2_SAMPLE_ALPHA_TO_COVERAGE:
      c->is_sample_alpha_to_coverage_enabled_ = 0;
      break;
    case GL_ES2_SAMPLE_COVERAGE:
      c->is_sample_coverage_enabled_ = 0;
      break;
    case GL_ES2_SCISSOR_TEST:
      c->is_scissor_test_enabled_ = 0;
      break;
    case GL_ES2_STENCIL_TEST:
      c->is_stencil_test_enabled_ = 0;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DisableVertexAttribArray)(gl_es2_uint index) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (index > UINT_MAX) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  unsigned int i_index = (unsigned int)index;
  if (i_index >= c->attribs_.num_attribs_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  c->attribs_.attribs_[i_index].enabled_ = 0;
}

static int gl_mode_to_pam(gl_es2_enum mode, primitive_assembly_mode_t *ppam) {
  primitive_assembly_mode_t pam;
  switch (mode) {
    case GL_ES2_POINTS:         pam = PAM_POINTS; break;
    case GL_ES2_LINE_STRIP:     pam = PAM_LINE_STRIP; break;
    case GL_ES2_LINE_LOOP:      pam = PAM_LINE_LOOP; break;
    case GL_ES2_LINES:          pam = PAM_LINES; break;
    case GL_ES2_TRIANGLE_STRIP: pam = PAM_TRIANGLE_STRIP; break;
    case GL_ES2_TRIANGLE_FAN:   pam = PAM_TRIANGLE_FAN; break;
    case GL_ES2_TRIANGLES:      pam = PAM_TRIANGLES; break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return 0;
  }
  *ppam = pam;
  return 1;
}

static void perform_draw(primitive_assembly_mode_t mode, 
                         size_t num_elements,
                         primitive_assembly_index_type_t index_type,
                         size_t arrayed_starting_index,
                         const void *indices) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (!c->current_program_) {
    /* Current program is invalid, however, no error is generated for this case (as per spec.) */
    return;
  }
  if (gl_es2_framebuffer_complete != gl_es2_framebuffer_check_completeness(c->framebuffer_)) {
    set_gl_err(GL_ES2_INVALID_FRAMEBUFFER_OPERATION);
    return;
  }

  int width, height;
  gl_es2_framebuffer_get_dims(c->framebuffer_, &width, &height);

  struct sl_program *prog = &c->current_program_->program_;

  int32_t scissor_left, scissor_bottom, scissor_width, scissor_height;

  scissor_left = 0; scissor_bottom = 0; scissor_width = width; scissor_height = height;
  if (c->is_scissor_test_enabled_) {
    scissor_left = c->scissor_left_;
    scissor_bottom = c->scissor_bottom_counted_from_bottom_;
    scissor_width = c->scissor_width_;
    scissor_height = c->scissor_height_;
  }

  /* Get these from viewport settings */
  int32_t vp_x, vp_y, vp_width, vp_height;
  vp_x = c->vp_x_; 
  vp_y = c->vp_y_; 
  vp_width = c->vp_width_; 
  vp_height = c->vp_height_;

  /* Default max_z is 32 bits to set it to some value, the actual max_z
   * is the depth buffer attachment's bit depth */
  uint32_t max_z = 0xFFFFFFFF;
  size_t zbuf_step = 0;
  int have_zbuf = 0;
  if (c->framebuffer_->depth_attachment_.kind_ == gl_es2_faot_none) {
  }
  else if (c->framebuffer_->depth_attachment_.kind_ == gl_es2_faot_renderbuffer) {
    struct gl_es2_renderbuffer *rb = c->framebuffer_->depth_attachment_.v_.rb_;
    if (rb) {
      switch (rb->format_) {
        case gl_es2_renderbuffer_format_depth16:
          max_z = 0xFFFF;
          zbuf_step = 2;
          have_zbuf = 1;
          break;
        case gl_es2_renderbuffer_format_depth32:
          max_z = 0xFFFFFFFF;
          zbuf_step = 4;
          have_zbuf =1;
          break;
      }
    }
  }
  else if (c->framebuffer_->depth_attachment_.kind_ == gl_es2_faot_texture) {
    /* No known texture formats for depth buffers as of yet */
  }
  void *rgba_buffer_ptr = NULL;
  size_t rgba_buffer_stride = 0;
  gl_es2_framebuffer_attachment_raw_ptr(&c->framebuffer_->color_attachment0_, &rgba_buffer_ptr, &rgba_buffer_stride);
  int is_red_enabled, is_green_enabled, is_blue_enabled, is_alpha_enabled;
  is_red_enabled = !!rgba_buffer_ptr && c->red_mask_;
  is_green_enabled = !!rgba_buffer_ptr && c->green_mask_;
  is_blue_enabled = !!rgba_buffer_ptr && c->blue_mask_;
  is_alpha_enabled = !!rgba_buffer_ptr && c->alpha_mask_;

  void *depth_buffer_ptr = NULL;
  size_t depth_buffer_stride = 0;
  gl_es2_framebuffer_attachment_raw_ptr(&c->framebuffer_->depth_attachment_, &depth_buffer_ptr, &depth_buffer_stride);
  int is_zbuf_enabled = (!!depth_buffer_ptr) && c->is_depth_test_enabled_;

  void  *stencil_buffer_ptr = NULL;
  size_t stencil_buffer_stride = 0;
  gl_es2_framebuffer_attachment_raw_ptr(&c->framebuffer_->stencil_attachment_, &stencil_buffer_ptr, &stencil_buffer_stride);
  int is_stencil_enabled = (!!stencil_buffer_ptr) && c->is_stencil_test_enabled_;

  struct stencil_settings {
    uint32_t mask;
    primitive_assembly_stencil_func_t func;
    uint32_t func_ref;
    uint32_t func_mask;
    primitive_assembly_stencil_op_t sfail;
    primitive_assembly_stencil_op_t zfail;
    primitive_assembly_stencil_op_t zpass;
  } stencil_cw, stencil_ccw;
  struct stencil_settings *ss_front, *ss_back;

  if (c->front_face_ == gl_es2_front_face_clockwise) {
    ss_front = &stencil_cw;
    ss_back = &stencil_ccw;
  }
  else {
    ss_front = &stencil_ccw;
    ss_back = &stencil_cw;
  }

  ss_front->mask = c->stencil_writemask_;
  ss_back->mask = c->stencil_back_writemask_;

  /* XXX: Fill out ss_front and ss_back when glStencilXXX functions have been implemented. */
  ss_front->func = c->stencil_func_;
  ss_front->func_mask = c->stencil_func_mask_;
  ss_front->func_ref = c->stencil_func_ref_;
  ss_front->sfail = c->stencil_sfail_;
  ss_front->zfail = c->stencil_zfail_;
  ss_front->zpass = c->stencil_zpass_;
  ss_back->func = c->stencil_back_func_;
  ss_back->func_mask = c->stencil_back_func_mask_;
  ss_back->func_ref = c->stencil_back_func_ref_;
  ss_back->sfail = c->stencil_back_sfail_;
  ss_back->zfail = c->stencil_back_zfail_;
  ss_back->zpass = c->stencil_back_zpass_;

  blend_eq_t rgb_eq = c->blend_rgb_eq_; 
  blend_eq_t alpha_eq = c->blend_alpha_eq_;
  blend_func_t src_rgb_fn = c->blend_src_rgb_fn_; 
  blend_func_t src_alpha_fn = c->blend_src_alpha_fn_;
  blend_func_t dst_rgb_fn = c->blend_dst_rgb_fn_; 
  blend_func_t dst_alpha_fn = c->blend_dst_alpha_fn_;
  if (!c->is_blend_enabled_) {
    /* Blending is disabled, revert to default settings, these are just a plain copy of the fragment */
    rgb_eq = BEQ_FUNC_ADD;
    alpha_eq = BEQ_FUNC_ADD;

    src_rgb_fn = BF_ONE;
    src_alpha_fn = BF_ONE;
    dst_rgb_fn = BF_ZERO;
    dst_alpha_fn = BF_ZERO;
  }

  size_t n;
  for (n = 0; n < GL_ES2_IMPL_MAX_NUM_TEXTURE_UNITS; ++n) {
    c->sampler_2D_uniform_loading_table_[n] = &c->active_texture_units_[n].texture_2d_->texture_2d_;
    c->sampler_Cube_uniform_loading_table_[n] = c->active_texture_units_[n].texture_cube_map_->texture_cube_maps_;
  }
  sl_program_load_uniforms_for_execution(prog, GL_ES2_IMPL_MAX_NUM_TEXTURE_UNITS, c->sampler_2D_uniform_loading_table_, c->sampler_Cube_uniform_loading_table_);

  primitive_assembly_draw_elements(&prog->pa_, &c->attribs_, prog->vertex_shader_, &prog->ar_, &prog->cs_, &c->ras_, 
                                   &prog->fragbuf_, prog->fragment_shader_,
                                   vp_x, vp_y, vp_width, vp_height, 
                                   c->near_plane_, c->far_plane_,
                                   (uint32_t)width, (uint32_t)height, 
                                   scissor_left, scissor_bottom, scissor_width, scissor_height,
                                   max_z,
                                   rgba_buffer_ptr, rgba_buffer_stride,
                                   depth_buffer_ptr, depth_buffer_stride, zbuf_step,
                                   stencil_buffer_ptr, stencil_buffer_stride, 2,
                                   is_stencil_enabled, /* no stencil test */
                                   /* Settings for stencil on clockwise triangles: */
                                   stencil_cw.mask, /* clockwise stencil mask: all output bits enabled */
                                   stencil_cw.func, /* clockwise stencil test: never pass */
                                   stencil_cw.func_ref, stencil_cw.func_mask, /* clockwise stencil function reference value and function mask value */
                                   stencil_cw.sfail, stencil_cw.zfail, stencil_cw.zpass, /* clockwise stencil-fail, stencil-success, stencil&depth-success: all keep */
                                   /* Same for counter-clockwise: */
                                   stencil_ccw.mask,
                                   stencil_ccw.func,
                                   stencil_ccw.func_ref, stencil_ccw.func_mask,
                                   stencil_ccw.sfail, stencil_ccw.zfail, stencil_ccw.zpass,
                                   is_zbuf_enabled, c->zbuf_func_, is_zbuf_enabled && c->depth_mask_, /* enable_zbuf_test, zbuf_func, enable_zbuf_write */
                                   is_red_enabled, is_green_enabled, is_blue_enabled, is_alpha_enabled,
                                   rgb_eq, alpha_eq,
                                   src_rgb_fn, src_alpha_fn,
                                   dst_rgb_fn, dst_alpha_fn,
                                   c->blend_color_red_, c->blend_color_grn_, c->blend_color_blu_, c->blend_color_alpha_,
                                   c->is_polygon_offset_fill_enabled_ ? c->polygon_offset_factor_ : 0.f, 
                                   c->is_polygon_offset_fill_enabled_ ? c->polygon_offset_units_ : 0.f,
                                   mode, num_elements, index_type, arrayed_starting_index, indices);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DrawArrays)(gl_es2_enum mode, gl_es2_int first, gl_es2_sizei count) {
  struct gl_es2_context *c = gl_es2_ctx();
  primitive_assembly_mode_t pam;
  if (!gl_mode_to_pam(mode, &pam)) return;

  perform_draw(pam, count, PAIT_UNSIGNED_INT, first, NULL);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(DrawElements)(gl_es2_enum mode, gl_es2_sizei count, gl_es2_enum type, const void *indices) {
  struct gl_es2_context *c = gl_es2_ctx();
  primitive_assembly_mode_t pam;
  if (!gl_mode_to_pam(mode, &pam)) return;

  primitive_assembly_index_type_t pait;

  switch (type) {
    case GL_ES2_UNSIGNED_BYTE:
      pait = PAIT_UNSIGNED_BYTE;
      break;
    case GL_ES2_UNSIGNED_SHORT:
      pait = PAIT_UNSIGNED_SHORT;
      break;
    case GL_ES2_UNSIGNED_INT: /* strictly speaking not in spec but we'll support it because it's useful */
      pait = PAIT_UNSIGNED_INT;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  if (c->element_array_buffer_) {
    /* indices is an offset into te GL_ELEMENT_ARRAY_BUFFER and not a pointer itself */
    uintptr_t indices_offset = (uintptr_t)indices;
    char *indices_ptr = ((char *)c->element_array_buffer_->buf_.data_) + indices_offset;
    perform_draw(pam, count, pait, 0, indices);

    return;
  }

  perform_draw(pam, count, pait, 0, indices);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Enable)(gl_es2_enum cap) {
  struct gl_es2_context *c = gl_es2_ctx();
  switch (cap) {
    case GL_ES2_BLEND:
      c->is_blend_enabled_ = 1;
      break;
    case GL_ES2_CULL_FACE:
      c->is_culling_enabled_ = 1;
      break;
    case GL_ES2_DEPTH_TEST:
      c->is_depth_test_enabled_ = 1;
      break;
    case GL_ES2_DITHER:
      c->is_dither_enabled_ = 1;
      break;
    case GL_ES2_POLYGON_OFFSET_FILL:
      c->is_polygon_offset_fill_enabled_ = 1;
      break;
    case GL_ES2_SAMPLE_ALPHA_TO_COVERAGE:
      c->is_sample_alpha_to_coverage_enabled_ = 1;
      break;
    case GL_ES2_SAMPLE_COVERAGE:
      c->is_sample_coverage_enabled_ = 1;
      break;
    case GL_ES2_SCISSOR_TEST:
      c->is_scissor_test_enabled_ = 1;
      break;
    case GL_ES2_STENCIL_TEST:
      c->is_stencil_test_enabled_ = 1;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(EnableVertexAttribArray)(gl_es2_uint index) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (index > UINT_MAX) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  unsigned int i_index = (unsigned int)index;
  if (i_index >= c->attribs_.num_attribs_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  c->attribs_.attribs_[i_index].enabled_ = 1;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Finish)(void) {
  /* Would wait for the drawing to complete, however: all drawing starts and completes immediately at the draw call. So we're done. */
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Flush)(void) {
  /* Would flush for execution all accummulated draw commands, however: all drawing completes immediately at the draw call, so there's
   * nothing to flush. */
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
  /* Note: type is not checked here, but at glCheckFramebufferStatus() */
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
  struct gl_es2_context *c = gl_es2_ctx();
  if (target != GL_ES2_FRAMEBUFFER) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }

  if (!texture) {
    /* Detach operation */
    struct gl_es2_framebuffer_attachment *fa = NULL;
    if (attachment == GL_ES2_COLOR_ATTACHMENT0) {
      fa = &c->framebuffer_->color_attachment0_;
    }
    else if (attachment == GL_ES2_DEPTH_ATTACHMENT) {
      fa = &c->framebuffer_->depth_attachment_;
    }
    else if (attachment == GL_ES2_STENCIL_ATTACHMENT) {
      fa = &c->framebuffer_->stencil_attachment_;
    }
    else {
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
    }
    gl_es2_framebuffer_attachment_detach(fa);
    return;
  }

  struct gl_es2_texture *tex = NULL;
  uintptr_t tex_name = (uintptr_t)texture;
  tex = (struct gl_es2_texture *)not_find(&c->texture_not_, tex_name);
  if (!tex) {
    /* texture is not the name of an existing texture object */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  struct sampler_2d *s2d = NULL;
  if (!get_tex_target(textarget, tex, &s2d)) {
    return;
  }
  struct gl_es2_framebuffer_attachment *fa = NULL;
  if (attachment == GL_ES2_COLOR_ATTACHMENT0) {
    fa = &c->framebuffer_->color_attachment0_;
  }
  else if (attachment == GL_ES2_DEPTH_ATTACHMENT) {
    fa = &c->framebuffer_->depth_attachment_;
  }
  else if (attachment == GL_ES2_STENCIL_ATTACHMENT) {
    fa = &c->framebuffer_->stencil_attachment_;
  }
  else {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }
  gl_es2_framebuffer_attachment_attach_texture(fa, tex);
  fa->cube_map_face_ = tex_target_gl_to_cubemap_face(textarget);
  fa->level_ = level;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(FrontFace)(gl_es2_enum mode) {
  struct gl_es2_context *c = gl_es2_ctx();
  switch (mode) {
    case GL_ES2_CW:
      c->front_face_ = gl_es2_front_face_clockwise;
      break;
    case GL_ES2_CCW:
      c->front_face_ = gl_es2_front_face_counterclockwise;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenBuffers)(gl_es2_sizei n, gl_es2_uint *buffers) {
  struct gl_es2_context *c = gl_es2_ctx();
  generic_name_gen(&c->buffer_rra_, n, buffers);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GenerateMipmap)(gl_es2_enum target) {
  gl_es2_enum t2d_targets[] = { GL_ES2_TEXTURE_2D };
  gl_es2_enum cube_targets[] = { GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_X, GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_X,
                                 GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                 GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Z };

  gl_es2_enum *targets = NULL;
  size_t num_targets = 0;
  if (target == GL_ES2_TEXTURE_2D) {
    targets = t2d_targets;
    num_targets = sizeof(t2d_targets)/sizeof(*t2d_targets);
  }
  else if (target == GL_ES2_TEXTURE_CUBE_MAP) {
    targets = cube_targets;
    num_targets = sizeof(cube_targets)/sizeof(*cube_targets);
  }
  else {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }

  size_t n;
  for (n = 0; n < num_targets; ++n) {
    struct gl_es2_texture *tex = NULL;
    struct sampler_2d *s2d = NULL;

    if (!get_active_tex_target(targets[n], &tex, &s2d)) {
      return;
    }
    switch (sampler_2d_generate_mipmaps(s2d)) {
      case SL_ERR_INVALID_ARG:
        /* no level 0 */
        set_gl_err(GL_ES2_INVALID_OPERATION);
        return;
      case SL_ERR_OVERFLOW:
      case SL_ERR_NO_MEM:
        set_gl_err(GL_ES2_OUT_OF_MEMORY);
        return;
      default:
        set_gl_err(GL_ES2_INVALID_OPERATION);
        return;
    }
  }
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

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetActiveAttrib)(gl_es2_uint program, gl_es2_uint index, gl_es2_sizei bufsize, gl_es2_sizei *length, gl_es2_int *size, gl_es2_enum *type, gl_es2_char *name) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (bufsize < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (((size_t)index) >= prog->program_.abt_.num_attrib_bindings_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  struct attrib_binding *ab = prog->program_.abt_.attrib_bindings_[index];

  if (!ab->var_) {
    /* Attribute binding with no variable.. we'll assume the link failed though 
     * this could also be a glBindAttribLocation() that later did not link / was
     * not found in the program. */
    if (size) *size = 0;
    if (type) *type = GL_ES2_FLOAT;
  }
  else {
    struct sl_type *t = sl_type_unqualified(ab->var_->type_);
    uint64_t num_elements_in_array = 1;
    if (t->kind_ == sltk_array) {
      /* We'll tolerate arrays even though they're not supported in ES 2.0 */
      num_elements_in_array = t->array_size_;
      if (num_elements_in_array > INT_MAX) {
        set_gl_err(GL_ES2_INVALID_OPERATION);
        return;
      }
      t = sl_type_unqualified(t->derived_type_);
    }
    switch (t->kind_) {
      case sltk_float:
        if (type) *type = GL_ES2_FLOAT;
        break;
      case sltk_vec2:
        if (type) *type = GL_ES2_FLOAT_VEC2;
        break;
      case sltk_vec3:
        if (type) *type = GL_ES2_FLOAT_VEC3;
        break;
      case sltk_vec4:
        if (type) *type = GL_ES2_FLOAT_VEC4;
        break;
      case sltk_mat2:
        if (type) *type = GL_ES2_FLOAT_MAT2;
        break;
      case sltk_mat3:
        if (type) *type = GL_ES2_FLOAT_MAT3;
        break;
      case sltk_mat4:
        if (type) *type = GL_ES2_FLOAT_MAT4;
        break;
      default:
        set_gl_err(GL_ES2_INVALID_OPERATION);
        return;
    }
    if (size) *size = (gl_es2_int)num_elements_in_array;
  }
  size_t len = strlen(ab->name_);
  if (length) *length = (gl_es2_sizei)len;
  if (name) {
    if (bufsize > len) {
      strcpy(name, ab->name_);
    }
    else if (bufsize > 0) {
      memcpy(name, ab->name_, bufsize - 1);
      name[bufsize - 1] = '\0';
    }
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetActiveUniform)(gl_es2_uint program, gl_es2_uint index, gl_es2_sizei bufsize, gl_es2_sizei *length, gl_es2_int *size, gl_es2_enum *type, gl_es2_char *name) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (bufsize < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  void *slab_mem = NULL;
  sl_reg_alloc_kind_t location_type;
  size_t name_length = 0;
  size_t final_array_size = 0;
  size_t entry_in_final_array = 0;
  /* First call to check name length, then, if name is requested, call again to get name */
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, (size_t)index, &slab_mem, &location_type, &name_length, NULL, &final_array_size, &entry_in_final_array);
  switch (r) {
    case SL_ERR_INVALID_ARG:
      set_gl_err(GL_ES2_INVALID_VALUE);
      return;
    case SL_ERR_OVERFLOW:
    case SL_ERR_NO_MEM:
    case SL_ERR_INTERNAL:
    default:
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    case SL_ERR_OK:
      break;
  }
  if (bufsize && name) {
    if (bufsize > (name_length + 1)) {
      /* Size is big enough */
      sl_uniform_get_location_info(&prog->program_.uniforms_, (size_t)index, NULL, NULL, NULL, name, NULL, NULL);
    }
    else {
      /* Name is to be truncated, allocate full buffer & then copy truncated version */
      char *fullnamebuf = (char *)malloc(name_length + 1);
      if (!fullnamebuf) {
        set_gl_err(GL_ES2_OUT_OF_MEMORY);
        return;
      }
      fullnamebuf[0] = '\0';
      sl_uniform_get_location_info(&prog->program_.uniforms_, (size_t)index, NULL, NULL, NULL, fullnamebuf, NULL, NULL);
      fullnamebuf[name_length] = '\0';
      memcpy(name, fullnamebuf, bufsize - 1);
      name[bufsize - 1] = '\0';
      free(fullnamebuf);
    }
  }
  if (length) *length = (gl_es2_sizei)name_length;
  if (size) {
    if (!entry_in_final_array) {
      /* Leads an array, therefore gets the size */
      *size = (gl_es2_int)final_array_size;
    }
    else {
      *size = 1;
    }
  }
  if (type) {
    switch (location_type) {
      case slrak_float:       *type = GL_ES2_FLOAT; break;
      case slrak_int:         *type = GL_ES2_INT; break;
      case slrak_bool:        *type = GL_ES2_BOOL; break;
      case slrak_vec2:        *type = GL_ES2_FLOAT_VEC2; break;
      case slrak_vec3:        *type = GL_ES2_FLOAT_VEC3; break;
      case slrak_vec4:        *type = GL_ES2_FLOAT_VEC4; break;
      case slrak_bvec2:       *type = GL_ES2_BOOL_VEC2; break;
      case slrak_bvec3:       *type = GL_ES2_BOOL_VEC3; break;
      case slrak_bvec4:       *type = GL_ES2_BOOL_VEC4; break;
      case slrak_ivec2:       *type = GL_ES2_INT_VEC2; break;
      case slrak_ivec3:       *type = GL_ES2_INT_VEC3; break;
      case slrak_ivec4:       *type = GL_ES2_INT_VEC4; break;
      case slrak_mat2:        *type = GL_ES2_FLOAT_MAT2; break;
      case slrak_mat3:        *type = GL_ES2_FLOAT_MAT3; break;
      case slrak_mat4:        *type = GL_ES2_FLOAT_MAT4; break;
      case slrak_sampler2D:   *type = GL_ES2_SAMPLER_2D; break;
      case slrak_samplerCube: *type = GL_ES2_SAMPLER_CUBE; break;
      default:
        set_gl_err(GL_ES2_INVALID_OPERATION);
        return;
    }
  }


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
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return -1;
  }

  if (!prog->program_.gl_last_link_status_) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return -1;
  }

  if (!c->allow_internals_ && !memcmp(name, "gl_", 3)) {
    /* Not a name we are allowed to find */
    return -1;
  }

  struct attrib_binding *ab = abt_find(&prog->program_.abt_, name, strlen(name));

  if (!ab) {
    /* Not a name we could find */
    return -1;
  }

  return GL_ES2_INVALID_OPERATION;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetBooleanv)(gl_es2_enum pname, gl_es2_boolean *data) {
  struct gl_es2_context *c = gl_es2_ctx();
  switch (pname) {
    case GL_ES2_MAX_VERTEX_ATTRIBS:
      *data = (!GL_ES2_IMPL_MAX_NUM_VERTEX_ATTRIBS) ? GL_ES2_FALSE : GL_ES2_TRUE;
      break;
    case GL_ES2_NUM_COMPRESSED_TEXTURE_FORMATS:
      /* No compressed texture formats support */
      *data = GL_ES2_FALSE;
      break;
    case GL_ES2_COMPRESSED_TEXTURE_FORMATS:
      /* Array size, as per above, is 0; no compressed texture formats supported. */
      break;
    case GL_ES2_DEPTH_RANGE:
      data[0] = (c->near_plane_ == 0.f) ? GL_ES2_FALSE : GL_ES2_TRUE;
      data[1] = (c->far_plane_ == 0.f) ? GL_ES2_FALSE : GL_ES2_TRUE;
      break;
    case GL_ES2_FRONT_FACE:
      switch (c->front_face_) {
        case gl_es2_front_face_clockwise:        data[0] = (gl_es2_boolean)(GL_ES2_CW ? GL_ES2_TRUE : GL_ES2_FALSE); break;
        case gl_es2_front_face_counterclockwise: data[0] = (gl_es2_boolean)(GL_ES2_CCW ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      }
      break;
    case GL_ES2_MAX_VIEWPORT_DIMS:
      data[0] = (gl_es2_boolean)(GL_ES2_IMPL_MAX_VIEWPORT_DIMS ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      data[1] = (gl_es2_boolean)(GL_ES2_IMPL_MAX_VIEWPORT_DIMS ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      break;
    case GL_ES2_MAX_TEXTURE_SIZE:
    case GL_ES2_MAX_CUBE_MAP_TEXTURE_SIZE:
      data[0] = (gl_es2_boolean)(GL_ES2_IMPL_MAX_VIEWPORT_DIMS ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      break;
    case GL_ES2_VIEWPORT:
      data[0] = (gl_es2_boolean)(c->vp_x_ ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      data[1] = (gl_es2_boolean)(c->vp_y_ ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      data[2] = (gl_es2_boolean)(c->vp_width_ ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      data[3] = (gl_es2_boolean)(c->vp_height_ ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      break;
    case GL_ES2_STENCIL_FUNC:
      data[0] = (gl_es2_boolean)(stencil_func_pasf_to_gl(c->stencil_func_) ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      break;
    case GL_ES2_STENCIL_VALUE_MASK:
      data[0] = (gl_es2_boolean)(c->stencil_back_func_mask_ ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      break;
    case GL_ES2_STENCIL_REF:
      data[0] = (gl_es2_boolean)(c->stencil_back_func_ref_ ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      break;
    case GL_ES2_STENCIL_BACK_FUNC:
      data[0] = (gl_es2_boolean)(c->stencil_back_func_ ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      break;
    case GL_ES2_STENCIL_BACK_VALUE_MASK:
      data[0] = (gl_es2_boolean)(c->stencil_back_func_mask_ ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      break;
    case GL_ES2_STENCIL_BACK_REF:
      data[0] = (gl_es2_boolean)(c->stencil_back_func_ref_ ? GL_ES2_TRUE : GL_ES2_FALSE); break;
      break;
    case GL_ES2_STENCIL_BITS:
      if (c->framebuffer_ &&
        c->framebuffer_->stencil_attachment_.kind_ == gl_es2_faot_renderbuffer &&
        c->framebuffer_->stencil_attachment_.v_.rb_->format_ == gl_es2_renderbuffer_format_stencil16) {
        data[0] = GL_ES2_TRUE;
      }
      else {
        data[0] = GL_ES2_FALSE;
      }
      break;
    case GL_ES2_STENCIL_FAIL:
      data[0] = (gl_es2_boolean)(stencil_op_paso_to_gl(c->stencil_sfail_) ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_STENCIL_PASS_DEPTH_PASS:
      data[0] = (gl_es2_boolean)(stencil_op_paso_to_gl(c->stencil_zpass_) ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_STENCIL_PASS_DEPTH_FAIL:
      data[0] = (gl_es2_boolean)(stencil_op_paso_to_gl(c->stencil_zfail_) ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_STENCIL_BACK_FAIL:
      data[0] = (gl_es2_boolean)(stencil_op_paso_to_gl(c->stencil_back_sfail_) ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_STENCIL_BACK_PASS_DEPTH_PASS:
      data[0] = (gl_es2_boolean)(stencil_op_paso_to_gl(c->stencil_back_zpass_) ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_STENCIL_BACK_PASS_DEPTH_FAIL:
      data[0] = (gl_es2_boolean)(stencil_op_paso_to_gl(c->stencil_back_zfail_) ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_POLYGON_OFFSET_FACTOR:
      data[0] = (gl_es2_boolean)((c->polygon_offset_factor_ == 0.f) ? GL_ES2_FALSE : GL_ES2_TRUE);
      break;
    case GL_ES2_POLYGON_OFFSET_UNITS:
      data[0] = (gl_es2_boolean)((c->polygon_offset_units_ == 0.f) ? GL_ES2_FALSE : GL_ES2_TRUE);
      break;
    case GL_ES2_ALIASED_LINE_WIDTH_RANGE:
      data[0] = (gl_es2_boolean)((GL_ES2_IMPL_MIN_LINE_WIDTH) ? GL_ES2_TRUE : GL_ES2_FALSE);
      data[1] = (gl_es2_boolean)((GL_ES2_IMPL_MAX_LINE_WIDTH) ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_ALIASED_POINT_SIZE_RANGE:
      data[0] = (gl_es2_boolean)((GL_ES2_IMPL_MIN_POINT_SIZE) ? GL_ES2_TRUE : GL_ES2_FALSE);
      data[1] = (gl_es2_boolean)((GL_ES2_IMPL_MAX_POINT_SIZE) ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_LINE_WIDTH:
      data[0] = (gl_es2_boolean)((c->line_width_) ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_PACK_ALIGNMENT:
      data[0] = (gl_es2_boolean)((c->pack_alignment_) ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_UNPACK_ALIGNMENT:
      data[0] = (gl_es2_boolean)((c->unpack_alignment_) ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_CURRENT_PROGRAM:
      data[0] = (gl_es2_boolean)((c->current_program_ && c->current_program_->no_.name_) ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_IMPLEMENTATION_COLOR_READ_FORMAT:
      /* We support more, but this is one of them */
      data[0] = (gl_es2_boolean)(GL_ES2_RGBA ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_IMPLEMENTATION_COLOR_READ_TYPE:
      /* We support more, but this is one of them */
      data[0] = (gl_es2_boolean)(GL_ES2_UNSIGNED_SHORT_5_5_5_1 ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_SAMPLE_COVERAGE_VALUE:
      data[0] = (gl_es2_boolean)(c->sample_coverage_value_ ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_SAMPLE_COVERAGE_INVERT:
      data[0] = (gl_es2_boolean)(c->sample_coverage_invert_ ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_SAMPLE_ALPHA_TO_COVERAGE:
      data[0] = (gl_es2_boolean)(c->is_sample_alpha_to_coverage_enabled_ ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_SAMPLE_BUFFERS:
      /* not supported as of yet */
      data[0] = (gl_es2_boolean)GL_ES2_FALSE;
      break;
    case GL_ES2_SAMPLES:
      /* not supported as of yet */
      data[0] = (gl_es2_boolean)GL_ES2_FALSE;
      break;
    case GL_ES2_NUM_SHADER_BINARY_FORMATS:
      data[0] = (gl_es2_boolean)GL_ES2_FALSE;
      break;
    case GL_ES2_SHADER_BINARY_FORMATS:
      break;
    case GL_ES2_SHADER_COMPILER:
      data[0] = (gl_es2_boolean)GL_ES2_TRUE;
      break;


    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetBufferParameteriv)(gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_buffer *buf;

  switch (target) {
    case GL_ES2_ARRAY_BUFFER:
      buf = c->array_buffer_;
      break;
    case GL_ES2_ELEMENT_ARRAY_BUFFER:
      buf = c->element_array_buffer_;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  if (!buf) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (pname) {
    case GL_ES2_BUFFER_SIZE:
      *params = (gl_es2_int)buf->buf_.size_;
      return;
    case GL_ES2_BUFFER_USAGE:
      *params = (gl_es2_int)buf->usage_;
      return;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC gl_es2_enum GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetError)(void) {
  struct gl_es2_context *c = gl_es2_ctx();
  gl_es2_enum err = c->current_error_;
  c->current_error_ = GL_ES2_NO_ERROR;
  return err;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetFloatv)(gl_es2_enum pname, gl_es2_float *data) {
  struct gl_es2_context *c = gl_es2_ctx();
  switch (pname) {
    case GL_ES2_MAX_VERTEX_ATTRIBS:
      *data = (float)GL_ES2_IMPL_MAX_NUM_VERTEX_ATTRIBS;
      break;
    case GL_ES2_NUM_COMPRESSED_TEXTURE_FORMATS:
      /* No compressed texture formats support */
      *data = 0.f;
      break;
    case GL_ES2_COMPRESSED_TEXTURE_FORMATS:
      /* Array size, as per above, is 0; no compressed texture formats supported; so data should not be set. */
      break;
    case GL_ES2_DEPTH_RANGE:
      data[0] = c->near_plane_;
      data[1] = c->far_plane_;
      break;
    case GL_ES2_FRONT_FACE:
      switch (c->front_face_) {
        case gl_es2_front_face_clockwise:        data[0] = (float)GL_ES2_CW; break;
        case gl_es2_front_face_counterclockwise: data[0] = (float)GL_ES2_CCW; break;
      }
      break;
    case GL_ES2_MAX_VIEWPORT_DIMS:
      data[0] = (float)GL_ES2_IMPL_MAX_VIEWPORT_DIMS;
      data[1] = (float)GL_ES2_IMPL_MAX_VIEWPORT_DIMS;
      break;
    case GL_ES2_MAX_TEXTURE_SIZE:
    case GL_ES2_MAX_CUBE_MAP_TEXTURE_SIZE:
      data[0] = (float)GL_ES2_IMPL_MAX_VIEWPORT_DIMS;
      break;
    case GL_ES2_VIEWPORT:
      data[0] = (float)c->vp_x_;
      data[1] = (float)c->vp_y_;
      data[2] = (float)c->vp_width_;
      data[3] = (float)c->vp_height_;
      break;
    case GL_ES2_STENCIL_FUNC:
      data[0] = (float)stencil_func_pasf_to_gl(c->stencil_func_);
      break;
    case GL_ES2_STENCIL_VALUE_MASK:
      data[0] = (float)c->stencil_back_func_mask_;
      break;
    case GL_ES2_STENCIL_REF:
      data[0] = (float)c->stencil_back_func_ref_;
      break;
    case GL_ES2_STENCIL_BACK_FUNC:
      data[0] = (float)c->stencil_back_func_;
      break;
    case GL_ES2_STENCIL_BACK_VALUE_MASK:
      data[0] = (float)c->stencil_back_func_mask_;
      break;
    case GL_ES2_STENCIL_BACK_REF:
      data[0] = (float)c->stencil_back_func_ref_;
      break;
    case GL_ES2_STENCIL_BITS:
      if (c->framebuffer_ && 
          c->framebuffer_->stencil_attachment_.kind_ == gl_es2_faot_renderbuffer &&
          c->framebuffer_->stencil_attachment_.v_.rb_->format_ == gl_es2_renderbuffer_format_stencil16) {
        data[0] = (float)16;
      }
      else {
        data[0] = 0.f;
      }
      break;
    case GL_ES2_STENCIL_FAIL:
      data[0] = (float)stencil_op_paso_to_gl(c->stencil_sfail_);
      break;
    case GL_ES2_STENCIL_PASS_DEPTH_PASS:
      data[0] = (float)stencil_op_paso_to_gl(c->stencil_zpass_);
      break;
    case GL_ES2_STENCIL_PASS_DEPTH_FAIL:
      data[0] = (float)stencil_op_paso_to_gl(c->stencil_zfail_);
      break;
    case GL_ES2_STENCIL_BACK_FAIL:
      data[0] = (float)stencil_op_paso_to_gl(c->stencil_back_sfail_);
      break;
    case GL_ES2_STENCIL_BACK_PASS_DEPTH_PASS:
      data[0] = (float)stencil_op_paso_to_gl(c->stencil_back_zpass_);
      break;
    case GL_ES2_STENCIL_BACK_PASS_DEPTH_FAIL:
      data[0] = (float)stencil_op_paso_to_gl(c->stencil_back_zfail_);
      break;
    case GL_ES2_POLYGON_OFFSET_FACTOR:
      data[0] = c->polygon_offset_factor_;
      break;
    case GL_ES2_POLYGON_OFFSET_UNITS:
      data[0] = c->polygon_offset_units_;
      break;
    case GL_ES2_ALIASED_LINE_WIDTH_RANGE:
      data[0] = (float)GL_ES2_IMPL_MIN_LINE_WIDTH;
      data[1] = (float)GL_ES2_IMPL_MAX_LINE_WIDTH;
      break;
    case GL_ES2_ALIASED_POINT_SIZE_RANGE:
      data[0] = (float)GL_ES2_IMPL_MIN_POINT_SIZE;
      data[1] = (float)GL_ES2_IMPL_MAX_POINT_SIZE;
      break;
    case GL_ES2_LINE_WIDTH:
      data[0] = (float)c->line_width_;
      break;
    case GL_ES2_PACK_ALIGNMENT:
      data[0] = (float)c->pack_alignment_;
      break;
    case GL_ES2_UNPACK_ALIGNMENT:
      data[0] = (float)c->unpack_alignment_;
      break;
    case GL_ES2_CURRENT_PROGRAM:
      data[0] = (float)(c->current_program_ ? c->current_program_->no_.name_ : 0);
      break;
    case GL_ES2_IMPLEMENTATION_COLOR_READ_FORMAT:
      /* We support more, but this is one of them */
      data[0] = (float)GL_ES2_RGBA;
      break;
    case GL_ES2_IMPLEMENTATION_COLOR_READ_TYPE:
      /* We support more, but this is one of them */
      data[0] = (float)GL_ES2_UNSIGNED_SHORT_5_5_5_1;
      break;
    case GL_ES2_SAMPLE_COVERAGE_VALUE:
      data[0] = c->sample_coverage_value_;
      break;
    case GL_ES2_SAMPLE_COVERAGE_INVERT:
      data[0] = (float)(c->sample_coverage_invert_ ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_SAMPLE_ALPHA_TO_COVERAGE:
      data[0] = (float)(c->is_sample_alpha_to_coverage_enabled_ ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_SAMPLE_BUFFERS:
      /* not supported as of yet */
      data[0] = 0.f;
      break;
    case GL_ES2_SAMPLES:
      /* not supported as of yet */
      data[0] = 0.f;
      break;
    case GL_ES2_NUM_SHADER_BINARY_FORMATS:
      data[0] = 0.f;
      break;
    case GL_ES2_SHADER_BINARY_FORMATS:
      break;
    case GL_ES2_SHADER_COMPILER:
      data[0] = (float)GL_ES2_TRUE;
      break;

    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetFramebufferAttachmentParameteriv)(gl_es2_enum target, gl_es2_enum attachment, gl_es2_enum pname, gl_es2_int *params) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (target != GL_ES2_FRAMEBUFFER) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }

  if (!c->framebuffer_) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  struct gl_es2_framebuffer_attachment *fa = NULL;
  switch (attachment) {
    case GL_ES2_COLOR_ATTACHMENT0:
      fa = &c->framebuffer_->color_attachment0_;
      break;
    case GL_ES2_DEPTH_ATTACHMENT:
      fa = &c->framebuffer_->depth_attachment_;
      break;
    case GL_ES2_STENCIL_ATTACHMENT:
      fa = &c->framebuffer_->stencil_attachment_;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
  
  switch (pname) {
    case GL_ES2_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
      switch (fa->kind_) {
        case gl_es2_faot_none:
          *params = GL_ES2_NONE;
          return;
        case gl_es2_faot_renderbuffer:
          *params = GL_ES2_RENDERBUFFER;
          return;
        case gl_es2_faot_texture:
          *params = GL_ES2_TEXTURE;
          return;
      }
    case GL_ES2_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:
      switch (fa->kind_) {
        case gl_es2_faot_none:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
        case gl_es2_faot_renderbuffer:
          *params = (gl_es2_int)fa->v_.rb_->no_.name_;
          return;
        case gl_es2_faot_texture:
          *params = (gl_es2_int)fa->v_.tex_->no_.name_;
          return;
      }
    case GL_ES2_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL:
      switch (fa->kind_) {
        case gl_es2_faot_none:
        case gl_es2_faot_renderbuffer:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
        case gl_es2_faot_texture:
          *params = (gl_es2_int)fa->level_;
          return;
      }
    case GL_ES2_FRAMEBUFFER_ATTACHMENT_CUBE_MAP_FACE:
      switch (fa->kind_) {
        case gl_es2_faot_none:
        case gl_es2_faot_renderbuffer:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
        case gl_es2_faot_texture:
          switch (fa->cube_map_face_) {
            case gl_es2_cube_map_positive_x:
              *params = GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_X;
              break;
            case gl_es2_cube_map_negative_x:
              *params = GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_X;
              break;
            case gl_es2_cube_map_positive_y:
              *params = GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Y;
              break;
            case gl_es2_cube_map_negative_y:
              *params = GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Y;
              break;
            case gl_es2_cube_map_positive_z:
              *params = GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Z;
              break;
            case gl_es2_cube_map_negative_z:
              *params = GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Z;
              break;
          }
          return;
      }

  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetIntegerv)(gl_es2_enum pname, gl_es2_int *data) {
  struct gl_es2_context *c = gl_es2_ctx();
  switch (pname) {
    case GL_ES2_MAX_VERTEX_ATTRIBS:
      *data = GL_ES2_IMPL_MAX_NUM_VERTEX_ATTRIBS;
      break;
    case GL_ES2_NUM_COMPRESSED_TEXTURE_FORMATS:
      /* No compressed texture formats support */
      *data = 0;
      break;
    case GL_ES2_COMPRESSED_TEXTURE_FORMATS:
      /* Array size, as per above, is 0; no compressed texture formats supported. */
      break;
    case GL_ES2_DEPTH_RANGE:
      data[0] = (gl_es2_int)floorf(0.5f + c->near_plane_);
      data[1] = (gl_es2_int)floorf(0.5f + c->far_plane_);
      break;
    case GL_ES2_FRONT_FACE:
      switch (c->front_face_) {
        case gl_es2_front_face_clockwise:        data[0] = (gl_es2_int)GL_ES2_CW; break;
        case gl_es2_front_face_counterclockwise: data[0] = (gl_es2_int)GL_ES2_CCW; break;
      }
      break;
    case GL_ES2_MAX_VIEWPORT_DIMS:
      data[0] = (gl_es2_int)GL_ES2_IMPL_MAX_VIEWPORT_DIMS;
      data[1] = (gl_es2_int)GL_ES2_IMPL_MAX_VIEWPORT_DIMS;
      break;
    case GL_ES2_MAX_TEXTURE_SIZE:
    case GL_ES2_MAX_CUBE_MAP_TEXTURE_SIZE:
      data[0] = (gl_es2_int)GL_ES2_IMPL_MAX_VIEWPORT_DIMS;
      break;
    case GL_ES2_VIEWPORT:
      data[0] = (gl_es2_int)c->vp_x_;
      data[1] = (gl_es2_int)c->vp_y_;
      data[2] = (gl_es2_int)c->vp_width_;
      data[3] = (gl_es2_int)c->vp_height_;
      break;
    case GL_ES2_STENCIL_FUNC:
      data[0] = stencil_func_pasf_to_gl(c->stencil_func_);
      break;
    case GL_ES2_STENCIL_VALUE_MASK: 
      data[0] = (gl_es2_int)c->stencil_back_func_mask_;
      break;
    case GL_ES2_STENCIL_REF: 
      data[0] = (gl_es2_int)c->stencil_back_func_ref_;
      break;
    case GL_ES2_STENCIL_BACK_FUNC: 
      data[0] = (gl_es2_int)c->stencil_back_func_;
      break;
    case GL_ES2_STENCIL_BACK_VALUE_MASK: 
      data[0] = (gl_es2_int)c->stencil_back_func_mask_;
      break;
    case GL_ES2_STENCIL_BACK_REF: 
      data[0] = (gl_es2_int)c->stencil_back_func_ref_;
      break;
    case GL_ES2_STENCIL_BITS:
      if (c->framebuffer_ && 
          c->framebuffer_->stencil_attachment_.kind_ == gl_es2_faot_renderbuffer &&
          c->framebuffer_->stencil_attachment_.v_.rb_->format_ == gl_es2_renderbuffer_format_stencil16) {
        data[0] = (gl_es2_int)16;
      }
      else {
        data[0] = 0;
      }
      break;
    case GL_ES2_STENCIL_FAIL:
      data[0] = (gl_es2_int)stencil_op_paso_to_gl(c->stencil_sfail_);
      break;
    case GL_ES2_STENCIL_PASS_DEPTH_PASS:
      data[0] = (gl_es2_int)stencil_op_paso_to_gl(c->stencil_zpass_);
      break;
    case GL_ES2_STENCIL_PASS_DEPTH_FAIL:
      data[0] = (gl_es2_int)stencil_op_paso_to_gl(c->stencil_zfail_);
      break;
    case GL_ES2_STENCIL_BACK_FAIL:
      data[0] = (gl_es2_int)stencil_op_paso_to_gl(c->stencil_back_sfail_);
      break;
    case GL_ES2_STENCIL_BACK_PASS_DEPTH_PASS:
      data[0] = (gl_es2_int)stencil_op_paso_to_gl(c->stencil_back_zpass_);
      break;
    case GL_ES2_STENCIL_BACK_PASS_DEPTH_FAIL:
      data[0] = (gl_es2_int)stencil_op_paso_to_gl(c->stencil_back_zfail_);
      break;
    case GL_ES2_POLYGON_OFFSET_FACTOR:
      data[0] = (gl_es2_int)c->polygon_offset_factor_;
      break;
    case GL_ES2_POLYGON_OFFSET_UNITS:
      data[0] = (gl_es2_int)c->polygon_offset_units_;
      break;
    case GL_ES2_ALIASED_LINE_WIDTH_RANGE:
      data[0] = (gl_es2_int)GL_ES2_IMPL_MIN_LINE_WIDTH;
      data[1] = (gl_es2_int)GL_ES2_IMPL_MAX_LINE_WIDTH;
      break;
    case GL_ES2_ALIASED_POINT_SIZE_RANGE:
      data[0] = (gl_es2_int)GL_ES2_IMPL_MIN_POINT_SIZE;
      data[1] = (gl_es2_int)GL_ES2_IMPL_MAX_POINT_SIZE;
      break;
    case GL_ES2_LINE_WIDTH:
      data[0] = (gl_es2_int)c->line_width_;
      break;
    case GL_ES2_PACK_ALIGNMENT:
      data[0] = (gl_es2_int)c->pack_alignment_;
      break;
    case GL_ES2_UNPACK_ALIGNMENT:
      data[0] = (gl_es2_int)c->unpack_alignment_;
      break;
    case GL_ES2_CURRENT_PROGRAM:
      data[0] = (gl_es2_int)(c->current_program_ ? c->current_program_->no_.name_ : 0);
      break;
    case GL_ES2_IMPLEMENTATION_COLOR_READ_FORMAT:
      /* We support more, but this is one of them */
      data[0] = (gl_es2_int)GL_ES2_RGBA;
      break;
    case GL_ES2_IMPLEMENTATION_COLOR_READ_TYPE:
      /* We support more, but this is one of them */
      data[0] = (gl_es2_int)GL_ES2_UNSIGNED_SHORT_5_5_5_1;
      break;
    case GL_ES2_SAMPLE_COVERAGE_VALUE:
      data[0] = (gl_es2_int)c->sample_coverage_value_;
      break;
    case GL_ES2_SAMPLE_COVERAGE_INVERT:
      data[0] = (gl_es2_int)(c->sample_coverage_invert_ ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_SAMPLE_ALPHA_TO_COVERAGE:
      data[0] = (gl_es2_int)(c->is_sample_alpha_to_coverage_enabled_ ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_SAMPLE_BUFFERS:
      /* not supported as of yet */
      data[0] = (gl_es2_int)0;
      break;
    case GL_ES2_SAMPLES:
      /* not supported as of yet */
      data[0] = (gl_es2_int)0;
      break;
    case GL_ES2_NUM_SHADER_BINARY_FORMATS:
      data[0] = (gl_es2_int)0;
      break;
    case GL_ES2_SHADER_BINARY_FORMATS:
      break;
    case GL_ES2_SHADER_COMPILER:
      data[0] = (gl_es2_int)GL_ES2_TRUE;
      break;

    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetProgramiv)(gl_es2_uint program, gl_es2_enum pname, gl_es2_int *params) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  switch (pname) {
    case GL_ES2_DELETE_STATUS: {
      *params = prog->flagged_for_deletion_ ? GL_ES2_TRUE : GL_ES2_FALSE;
      return;
    }
    case GL_ES2_LINK_STATUS: {
      *params = prog->program_.gl_last_link_status_ ? GL_ES2_TRUE : GL_ES2_FALSE;
      return;
    }
    case GL_ES2_VALIDATE_STATUS: {
      /* XXX: IMPLEMENT THIS */
      return;
    }
    case GL_ES2_INFO_LOG_LENGTH: {
      *params = 0; /* if no program log, 0 is returned .. */
      if (prog->program_.log_.gl_info_log_size_) {
        /* .. otherwise the size of the log plus a null terminator is returned (this is different from GetProgramInfoLog) */
        *params = (gl_es2_int)(1 + prog->program_.log_.gl_info_log_size_);
      }
      break;
    }
    case GL_ES2_ATTACHED_SHADERS: {
      /* Number of attached shaders */
      *params = 0;
      if (prog->fragment_shader_.shader_) (*params)++;
      if (prog->vertex_shader_.shader_) (*params)++;
      break;
    }
    case GL_ES2_ACTIVE_ATTRIBUTES: {
      *params = (gl_es2_int)prog->program_.abt_.num_attrib_bindings_;
      break;
    }
    case GL_ES2_ACTIVE_ATTRIBUTE_MAX_LENGTH: {
      struct attrib_binding *ab = prog->program_.abt_.seq_;
      size_t max_length = 1;
      if (ab) {
        do {
          size_t len = ab->name_len_;
          if ((len + 1) > max_length) max_length = len + 1;

          ab = ab->next_;
        } while (ab != prog->program_.abt_.seq_);
      }
      *params = (gl_es2_int)max_length;
      break;
    }
    case GL_ES2_ACTIVE_UNIFORMS: {
      int r;
      size_t num_locations = 0;
      r = sl_uniform_table_num_locations(&prog->program_.uniforms_, &num_locations);
      if (r == SL_ERR_NO_MEM) {
        set_gl_err(GL_ES2_OUT_OF_MEMORY);
        return;
      }
      else if (r) {
        set_gl_err(GL_ES2_INVALID_OPERATION);
        return;
      }
      if (num_locations > INT_MAX) {
        set_gl_err(GL_ES2_INVALID_VALUE);
        return;
      }
      *params = (gl_es2_int)num_locations;
      break;
    }
    case GL_ES2_ACTIVE_UNIFORM_MAX_LENGTH: {
      size_t max_name_length = 0;
      int r;
      r = sl_uniform_table_max_name_length(&prog->program_.uniforms_, &max_name_length);
      if (r == SL_ERR_NO_MEM) {
        set_gl_err(GL_ES2_OUT_OF_MEMORY);
        return;
      }
      else if (r) {
        set_gl_err(GL_ES2_INVALID_OPERATION);
        return;
      }
      if (max_name_length > INT_MAX) {
        set_gl_err(GL_ES2_INVALID_VALUE);
        return;
      }
      *params = (gl_es2_int)max_name_length;
      break;
    }
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetProgramInfoLog)(gl_es2_uint program, gl_es2_sizei bufsize, gl_es2_sizei *length, gl_es2_char *infoLog) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  size_t slength = 0;
  sl_info_log_get_log(&prog->program_.log_, bufsize, &slength, (char *)infoLog);
  if (length) *length = (gl_es2_sizei)slength;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetRenderbufferParameteriv)(gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (target != GL_ES2_RENDERBUFFER) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }
  struct gl_es2_renderbuffer *rb = c->renderbuffer_;
  if (!rb) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  switch (pname) {
    case GL_ES2_RENDERBUFFER_WIDTH:
      *params = (gl_es2_int)rb->width_;
      return;
    case GL_ES2_RENDERBUFFER_HEIGHT:
      *params = (gl_es2_int)rb->height_;
      return;
    case GL_ES2_RENDERBUFFER_INTERNAL_FORMAT:
      switch (rb->format_) {
        case gl_es2_renderbuffer_format_none:
          *params = GL_ES2_RGBA4;
          break;
          /* "An implementation may vary its allocation of internal component resolution based on any glRenderbufferStorage parameter" */
        case gl_es2_renderbuffer_format_rgba32:
          *params = (gl_es2_int)GL_ES2_RGBA8;
          break;
        case gl_es2_renderbuffer_format_depth16:
          *params = (gl_es2_int)GL_ES2_DEPTH_COMPONENT16;
          break;
        case gl_es2_renderbuffer_format_depth32:
          *params = (gl_es2_int)GL_ES2_DEPTH_COMPONENT32; /* note: not in OpenGL ES 2.0 ; but it's what we use internally. */
          break;
        case gl_es2_renderbuffer_format_stencil16:
          *params = (gl_es2_int)GL_ES2_STENCIL_INDEX16;
          break;
      }
    case GL_ES2_RENDERBUFFER_RED_SIZE: {
      switch (rb->format_) {
        case gl_es2_renderbuffer_format_none:
          /* default: GL_ES2_RGBA4 */
          *params = 4;
          break;
        case gl_es2_renderbuffer_format_rgba32:
          *params = 8;
          break;
        case gl_es2_renderbuffer_format_depth16:
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_depth32:
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_stencil16:
          *params = 0;
          break;
      }
      break;
    }
    case GL_ES2_RENDERBUFFER_GREEN_SIZE: {
      switch (rb->format_) {
        case gl_es2_renderbuffer_format_none:
          /* default: GL_ES2_RGBA4 */
          *params = 4;
          break;
        case gl_es2_renderbuffer_format_rgba32:
          *params = 8;
          break;
        case gl_es2_renderbuffer_format_depth16:
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_depth32:
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_stencil16:
          *params = 0;
          break;
      }
      break;
    }
    case GL_ES2_RENDERBUFFER_BLUE_SIZE: {
      switch (rb->format_) {
        case gl_es2_renderbuffer_format_none:
          /* default: GL_ES2_RGBA4 */
          *params = 4;
          break;
        case gl_es2_renderbuffer_format_rgba32:
          *params = 8;
          break;
        case gl_es2_renderbuffer_format_depth16:
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_depth32:
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_stencil16:
          *params = 0;
          break;
      }
      break;
    }
    case GL_ES2_RENDERBUFFER_ALPHA_SIZE: {
      switch (rb->format_) {
        case gl_es2_renderbuffer_format_none:
          /* default: GL_ES2_RGBA4 */
          *params = 4;
          break;
        case gl_es2_renderbuffer_format_rgba32:
          *params = 8;
          break;
        case gl_es2_renderbuffer_format_depth16:
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_depth32:
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_stencil16:
          *params = 0;
          break;
      }
      break;
    }
    case GL_ES2_RENDERBUFFER_DEPTH_SIZE: {
      switch (rb->format_) {
        case gl_es2_renderbuffer_format_none:
          /* default: GL_ES2_RGBA4 */
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_rgba32:
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_depth16:
          *params = 16;
          break;
        case gl_es2_renderbuffer_format_depth32:
          *params = 32;
          break;
        case gl_es2_renderbuffer_format_stencil16:
          *params = 0;
          break;
      }
      break;
    }
    case GL_ES2_RENDERBUFFER_STENCIL_SIZE: {
      switch (rb->format_) {
        case gl_es2_renderbuffer_format_none:
          /* default: GL_ES2_RGBA4 */
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_rgba32:
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_depth16:
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_depth32:
          *params = 0;
          break;
        case gl_es2_renderbuffer_format_stencil16:
          *params = 16;
          break;
      }
      break;
    }
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetShaderiv)(gl_es2_uint shader, gl_es2_enum pname, gl_es2_int *params) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t shad_name = (uintptr_t)shader;
  struct gl_es2_shader *shad = (struct gl_es2_shader *)not_find(&c->shader_not_, shad_name);
  if (!shad) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  switch (pname) {
    case GL_ES2_SHADER_TYPE:
      *params = (gl_es2_int)shad->type_;
      break;
    case GL_ES2_DELETE_STATUS:
      *params = shad->flagged_for_deletion_ ? GL_ES2_TRUE : GL_ES2_FALSE;
      break;
    case GL_ES2_COMPILE_STATUS:
      *params = shad->shader_.gl_last_compile_status_ ? GL_ES2_TRUE : GL_ES2_FALSE;
      break;
    case GL_ES2_INFO_LOG_LENGTH:
      *params = 0; /* if no shader log, 0 is returned .. */
      if (shad->shader_.gl_info_log_.gl_info_log_size_) {
        /* .. otherwise the size of the log plus a null terminator is returned (this is different from GetShaderInfoLog) */
        *params = (gl_es2_int)(1 + shad->shader_.gl_info_log_.gl_info_log_size_);
      }
      break;
    case GL_ES2_SHADER_SOURCE_LENGTH:
      *params = (gl_es2_int)shad->shader_.source_length_;
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetShaderInfoLog)(gl_es2_uint shader, gl_es2_sizei bufsize, gl_es2_sizei *length, gl_es2_char *infoLog) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t shad_name = (uintptr_t)shader;
  struct gl_es2_shader *shad = (struct gl_es2_shader *)not_find(&c->shader_not_, shad_name);
  if (!shad) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  size_t slength = 0;
  sl_info_log_get_log(&shad->shader_.gl_info_log_, bufsize, &slength, (char *)infoLog);
  if (length) *length = (gl_es2_sizei)slength;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetShaderPrecisionFormat)(gl_es2_enum shadertype, gl_es2_enum precisiontype, gl_es2_int *range, gl_es2_int *precision) {
  if ((shadertype != GL_ES2_VERTEX_SHADER) && (shadertype != GL_ES2_FRAGMENT_SHADER)) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }
  switch (precisiontype) {
    case GL_ES2_LOW_FLOAT:
    case GL_ES2_MEDIUM_FLOAT:
    case GL_ES2_HIGH_FLOAT:
      /* Filling in with the example 32 bit float from the spec.. */
      range[0] = 127;
      range[1] = 127;
      *precision = 0;
      break;
    case GL_ES2_LOW_INT:
    case GL_ES2_MEDIUM_INT:
    case GL_ES2_HIGH_INT:
      /* Expanding the spec to 64 bit numbers.. */
      range[0] = 63;
      range[1] = 62;
      *precision = 0;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetShaderSource)(gl_es2_uint shader, gl_es2_sizei bufsize, gl_es2_sizei *length, gl_es2_char *source) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t shad_name = (uintptr_t)shader;
  struct gl_es2_shader *shad = (struct gl_es2_shader *)not_find(&c->shader_not_, shad_name);
  if (!shad) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (bufsize > shad->shader_.source_length_) {
    memcpy(source, shad->shader_.source_, shad->shader_.source_length_);
    source[shad->shader_.source_length_] = '\0';
    if (length) *length = (gl_es2_sizei)shad->shader_.source_length_;
    return;
  }
  if (bufsize) {
    memcpy(source, shad->shader_.source_, bufsize - 1);
    source[bufsize - 1] = '\0';
    if (length) *length = (gl_es2_sizei)(bufsize - 1);
    return;
  }
  if (length) *length = 0;
  return;
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
      return "GL_OES_depth32";
  }
  return NULL;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetTexParameterfv)(gl_es2_enum target, gl_es2_enum pname, gl_es2_float *params) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_texture *tex;
  struct sampler_2d *s2d = NULL;
  switch (target) {
    case GL_ES2_TEXTURE_2D:
      tex = c->active_texture_units_[c->current_active_texture_unit_].texture_2d_;
      s2d = &tex->texture_2d_;
      break;
    case GL_ES2_TEXTURE_CUBE_MAP:
      tex = c->active_texture_units_[c->current_active_texture_unit_].texture_cube_map_;
      s2d = tex->texture_cube_maps_;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  switch (pname) {
    case GL_ES2_TEXTURE_MAG_FILTER:
      switch (s2d->mag_filter_) {
        case s2d_nearest:                *params = (gl_es2_float)GL_ES2_NEAREST; break;
        case s2d_linear:                 *params = (gl_es2_float)GL_ES2_LINEAR; break;
        case s2d_nearest_mipmap_nearest: *params = (gl_es2_float)GL_ES2_NEAREST_MIPMAP_NEAREST; break;
        case s2d_nearest_mipmap_linear:  *params = (gl_es2_float)GL_ES2_LINEAR_MIPMAP_NEAREST; break;
        case s2d_linear_mipmap_nearest:  *params = (gl_es2_float)GL_ES2_NEAREST_MIPMAP_LINEAR; break;
        case s2d_linear_mipmap_linear:   *params = (gl_es2_float)GL_ES2_LINEAR_MIPMAP_LINEAR; break;
      }
      break;
    case GL_ES2_TEXTURE_MIN_FILTER:
      switch (s2d->min_filter_) {
        case s2d_nearest:                *params = (gl_es2_float)GL_ES2_NEAREST; break;
        case s2d_linear:                 *params = (gl_es2_float)GL_ES2_LINEAR; break;
        case s2d_nearest_mipmap_nearest: *params = (gl_es2_float)GL_ES2_NEAREST_MIPMAP_NEAREST; break;
        case s2d_nearest_mipmap_linear:  *params = (gl_es2_float)GL_ES2_LINEAR_MIPMAP_NEAREST; break;
        case s2d_linear_mipmap_nearest:  *params = (gl_es2_float)GL_ES2_NEAREST_MIPMAP_LINEAR; break;
        case s2d_linear_mipmap_linear:   *params = (gl_es2_float)GL_ES2_LINEAR_MIPMAP_LINEAR; break;
      }
      break;
    case GL_ES2_TEXTURE_WRAP_S:
      switch (s2d->wrap_s_) {
        case s2d_clamp_to_edge:   *params = (gl_es2_float)GL_ES2_CLAMP_TO_EDGE; break;
        case s2d_repeat:          *params = (gl_es2_float)GL_ES2_REPEAT; break;
        case s2d_mirrored_repeat: *params = (gl_es2_float)GL_ES2_MIRRORED_REPEAT; break;
      }
      break;
    case GL_ES2_TEXTURE_WRAP_T:
      switch (s2d->wrap_t_) {
        case s2d_clamp_to_edge:   *params = (gl_es2_float)GL_ES2_CLAMP_TO_EDGE; break;
        case s2d_repeat:          *params = (gl_es2_float)GL_ES2_REPEAT; break;
        case s2d_mirrored_repeat: *params = (gl_es2_float)GL_ES2_MIRRORED_REPEAT; break;
      }
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetTexParameteriv)(gl_es2_enum target, gl_es2_enum pname, gl_es2_int *params) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_texture *tex;
  struct sampler_2d *s2d = NULL;
  switch (target) {
    case GL_ES2_TEXTURE_2D:
      tex = c->active_texture_units_[c->current_active_texture_unit_].texture_2d_;
      s2d = &tex->texture_2d_;
      break;
    case GL_ES2_TEXTURE_CUBE_MAP:
      tex = c->active_texture_units_[c->current_active_texture_unit_].texture_cube_map_;
      s2d = tex->texture_cube_maps_;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  switch (pname) {
    case GL_ES2_TEXTURE_MAG_FILTER:
      switch (s2d->mag_filter_) {
        case s2d_nearest:                *params = (gl_es2_int)GL_ES2_NEAREST; break;
        case s2d_linear:                 *params = (gl_es2_int)GL_ES2_LINEAR; break;
        case s2d_nearest_mipmap_nearest: *params = (gl_es2_int)GL_ES2_NEAREST_MIPMAP_NEAREST; break;
        case s2d_nearest_mipmap_linear:  *params = (gl_es2_int)GL_ES2_LINEAR_MIPMAP_NEAREST; break;
        case s2d_linear_mipmap_nearest:  *params = (gl_es2_int)GL_ES2_NEAREST_MIPMAP_LINEAR; break;
        case s2d_linear_mipmap_linear:   *params = (gl_es2_int)GL_ES2_LINEAR_MIPMAP_LINEAR; break;
      }
      break;
    case GL_ES2_TEXTURE_MIN_FILTER:
      switch (s2d->min_filter_) {
        case s2d_nearest:                *params = (gl_es2_int)GL_ES2_NEAREST; break;
        case s2d_linear:                 *params = (gl_es2_int)GL_ES2_LINEAR; break;
        case s2d_nearest_mipmap_nearest: *params = (gl_es2_int)GL_ES2_NEAREST_MIPMAP_NEAREST; break;
        case s2d_nearest_mipmap_linear:  *params = (gl_es2_int)GL_ES2_LINEAR_MIPMAP_NEAREST; break;
        case s2d_linear_mipmap_nearest:  *params = (gl_es2_int)GL_ES2_NEAREST_MIPMAP_LINEAR; break;
        case s2d_linear_mipmap_linear:   *params = (gl_es2_int)GL_ES2_LINEAR_MIPMAP_LINEAR; break;
      }
      break;
    case GL_ES2_TEXTURE_WRAP_S:
      switch (s2d->wrap_s_) {
        case s2d_clamp_to_edge:   *params = (gl_es2_int)GL_ES2_CLAMP_TO_EDGE; break;
        case s2d_repeat:          *params = (gl_es2_int)GL_ES2_REPEAT; break;
        case s2d_mirrored_repeat: *params = (gl_es2_int)GL_ES2_MIRRORED_REPEAT; break;
      }
      break;
    case GL_ES2_TEXTURE_WRAP_T:
      switch (s2d->wrap_t_) {
        case s2d_clamp_to_edge:   *params = (gl_es2_int)GL_ES2_CLAMP_TO_EDGE; break;
        case s2d_repeat:          *params = (gl_es2_int)GL_ES2_REPEAT; break;
        case s2d_mirrored_repeat: *params = (gl_es2_int)GL_ES2_MIRRORED_REPEAT; break;
      }
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetUniformfv)(gl_es2_uint program, gl_es2_int location, gl_es2_float *params) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, NULL, NULL);
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_float:*params = (gl_es2_float)*(float *)mem; break;
    case slrak_int:  *params = (gl_es2_float)*(int64_t *)mem; break;
    case slrak_bool: *params = (gl_es2_float)(!!*(uint8_t *)mem); break;
    case slrak_vec2: {
      params[0] = (gl_es2_float)((float *)mem)[0];
      params[1] = (gl_es2_float)((float *)mem)[1];
      break;
    }
    case slrak_vec3: {
      params[0] = (gl_es2_float)((float *)mem)[0];
      params[1] = (gl_es2_float)((float *)mem)[1];
      params[2] = (gl_es2_float)((float *)mem)[2];
      break;
    }
    case slrak_vec4: {
      params[0] = (gl_es2_float)((float *)mem)[0];
      params[1] = (gl_es2_float)((float *)mem)[1];
      params[2] = (gl_es2_float)((float *)mem)[2];
      params[3] = (gl_es2_float)((float *)mem)[3];
      break;
    }
    case slrak_bvec2: {
      params[0] = (gl_es2_float)!!((uint8_t *)mem)[0];
      params[1] = (gl_es2_float)!!((uint8_t *)mem)[1];
      break;
    }
    case slrak_bvec3: {
      params[0] = (gl_es2_float)!!((uint8_t *)mem)[0];
      params[1] = (gl_es2_float)!!((uint8_t *)mem)[1];
      params[2] = (gl_es2_float)!!((uint8_t *)mem)[2];
      break;
    }
    case slrak_bvec4: {
      params[0] = (gl_es2_float)!!((uint8_t *)mem)[0];
      params[1] = (gl_es2_float)!!((uint8_t *)mem)[1];
      params[2] = (gl_es2_float)!!((uint8_t *)mem)[2];
      params[3] = (gl_es2_float)!!((uint8_t *)mem)[3];
      break;
    }
    case slrak_ivec2: {
      params[0] = (gl_es2_float)((int64_t *)mem)[0];
      params[1] = (gl_es2_float)((int64_t *)mem)[1];
      break;
    }
    case slrak_ivec3: {
      params[0] = (gl_es2_float)((int64_t *)mem)[0];
      params[1] = (gl_es2_float)((int64_t *)mem)[1];
      params[2] = (gl_es2_float)((int64_t *)mem)[2];
      break;
    }
    case slrak_ivec4: {
      params[0] = (gl_es2_float)((int64_t *)mem)[0];
      params[1] = (gl_es2_float)((int64_t *)mem)[1];
      params[2] = (gl_es2_float)((int64_t *)mem)[2];
      params[3] = (gl_es2_float)((int64_t *)mem)[3];
      break;
    }
    case slrak_mat2: {
      int n;
      for (n = 0; n < 4; ++n) {
        params[n] = (gl_es2_float)((float *)mem)[n];
      }
      break;
    }
    case slrak_mat3: {
      int n;
      for (n = 0; n < 9; ++n) {
        params[n] = (gl_es2_float)((float *)mem)[n];
      }
      break;
    }
    case slrak_mat4: {
      int n;
      for (n = 0; n < 16; ++n) {
        params[n] = (gl_es2_float)((float *)mem)[n];
      }
      break;
    }
    case slrak_sampler2D: {
      *params = (gl_es2_float)*(int32_t *)mem; 
      break;
    }
    case slrak_samplerCube: {
      *params = (gl_es2_float)*(int32_t *)mem;
      break;
    }
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetUniformiv)(gl_es2_uint program, gl_es2_int location, gl_es2_int *params) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, NULL, NULL);
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_float:*params = (gl_es2_int)*(float *)mem; break;
    case slrak_int:  *params = (gl_es2_int)*(int64_t *)mem; break;
    case slrak_bool: *params = (gl_es2_int)(!!*(uint8_t *)mem); break;
    case slrak_vec2: {
      params[0] = (gl_es2_int)((float *)mem)[0];
      params[1] = (gl_es2_int)((float *)mem)[1];
      break;
    }
    case slrak_vec3: {
      params[0] = (gl_es2_int)((float *)mem)[0];
      params[1] = (gl_es2_int)((float *)mem)[1];
      params[2] = (gl_es2_int)((float *)mem)[2];
      break;
    }
    case slrak_vec4: {
      params[0] = (gl_es2_int)((float *)mem)[0];
      params[1] = (gl_es2_int)((float *)mem)[1];
      params[2] = (gl_es2_int)((float *)mem)[2];
      params[3] = (gl_es2_int)((float *)mem)[3];
      break;
    }
    case slrak_bvec2: {
      params[0] = (gl_es2_int)!!((uint8_t *)mem)[0];
      params[1] = (gl_es2_int)!!((uint8_t *)mem)[1];
      break;
    }
    case slrak_bvec3: {
      params[0] = (gl_es2_int)!!((uint8_t *)mem)[0];
      params[1] = (gl_es2_int)!!((uint8_t *)mem)[1];
      params[2] = (gl_es2_int)!!((uint8_t *)mem)[2];
      break;
    }
    case slrak_bvec4: {
      params[0] = (gl_es2_int)!!((uint8_t *)mem)[0];
      params[1] = (gl_es2_int)!!((uint8_t *)mem)[1];
      params[2] = (gl_es2_int)!!((uint8_t *)mem)[2];
      params[3] = (gl_es2_int)!!((uint8_t *)mem)[3];
      break;
    }
    case slrak_ivec2: {
      params[0] = (gl_es2_int)((int64_t *)mem)[0];
      params[1] = (gl_es2_int)((int64_t *)mem)[1];
      break;
    }
    case slrak_ivec3: {
      params[0] = (gl_es2_int)((int64_t *)mem)[0];
      params[1] = (gl_es2_int)((int64_t *)mem)[1];
      params[2] = (gl_es2_int)((int64_t *)mem)[2];
      break;
    }
    case slrak_ivec4: {
      params[0] = (gl_es2_int)((int64_t *)mem)[0];
      params[1] = (gl_es2_int)((int64_t *)mem)[1];
      params[2] = (gl_es2_int)((int64_t *)mem)[2];
      params[3] = (gl_es2_int)((int64_t *)mem)[3];
      break;
    }
    case slrak_mat2: {
      int n;
      for (n = 0; n < 4; ++n) {
        params[n] = (gl_es2_int)((float *)mem)[n];
      }
      break;
    }
    case slrak_mat3: {
      int n;
      for (n = 0; n < 9; ++n) {
        params[n] = (gl_es2_int)((float *)mem)[n];
      }
      break;
    }
    case slrak_mat4: {
      int n;
      for (n = 0; n < 16; ++n) {
        params[n] = (gl_es2_int)((float *)mem)[n];
      }
      break;
    }
    case slrak_sampler2D: {
      *params = (gl_es2_int)*(int32_t *)mem; 
      break;
    }
    case slrak_samplerCube: {
      *params = (gl_es2_int)*(int32_t *)mem;
      break;
    }
  }
}

GL_ES2_DECL_SPEC gl_es2_int GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetUniformLocation)(gl_es2_uint program, const gl_es2_char *name) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return -1;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return -1;
  }
  if (!c->allow_internals_ && !memcmp(name, "gl_", 3)) {
    return -1;
  }
  int r;
  size_t location = SIZE_MAX;
  r = sl_uniform_get_named_location(&prog->program_.uniforms_, name, &location);
  if (r == SL_ERR_INVALID_ARG) {
    /* No corresponding uniform was found. */
    return -1;
  }
  return (gl_es2_int)location;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetVertexAttribfv)(gl_es2_uint index, gl_es2_enum pname, gl_es2_float *params) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (index > c->attribs_.num_attribs_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  struct attrib *ab = c->attribs_.attribs_ + index;

  switch (pname) {
    case GL_ES2_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING: {
      if (!ab->buf_) {
        *params = (float)0;
        return;
      }
      /* All buffers are a struct data_buffer sitting as a "struct data_buffer buf_"
       * field in an gl_es2_buffer structure; use that to revert from the buf_ to the
       * gl_es2_buffer that contains it.
       * NOTE: this is fragile and will implode if, for whatever reason, we stray
       *       and start using data_buffer structs outside of a gl_es2_buffer. 
       * NOTE2: If you have problems with it here, you're going to have problems with it at glGetVertexAttribiv!!*/
      uintptr_t offset_to_buf_field = (uintptr_t)&(((struct gl_es2_buffer *)0)->buf_);
      struct gl_es2_buffer *gl_buf = (struct gl_es2_buffer *)(((char *)ab->buf_) - offset_to_buf_field);

      *params = (float)gl_buf->no_.name_;
      break;
    }
    case GL_ES2_VERTEX_ATTRIB_ARRAY_ENABLED:
      *params = (float)(ab->enabled_ ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_VERTEX_ATTRIB_ARRAY_SIZE:
      *params = (float)(ab->size_);
      break;
    case GL_ES2_VERTEX_ATTRIB_ARRAY_STRIDE:
      *params = (float)(ab->stride_);
      break;
    case GL_ES2_VERTEX_ATTRIB_ARRAY_TYPE:
      switch (ab->data_type_) {
        case ADT_BYTE:
          *params = (float)GL_ES2_BYTE;
          break;
        case ADT_UNSIGNED_BYTE:
          *params = (float)GL_ES2_UNSIGNED_BYTE;
          break;
        case ADT_SHORT:
          *params = (float)GL_ES2_SHORT;
          break;
        case ADT_UNSIGNED_SHORT:
          *params = (float)GL_ES2_UNSIGNED_SHORT;
          break;
        case ADT_INT:
          *params = (float)GL_ES2_INT;
          break;
        case ADT_UNSIGNED_INT:
          *params = (float)GL_ES2_UNSIGNED_INT;
          break;
        case ADT_FIXED:
          *params = (float)GL_ES2_FIXED;
          break;
        case ADT_FLOAT:
          *params = (float)GL_ES2_FLOAT;
          break;
      }
      break;
    case GL_ES2_VERTEX_ATTRIB_ARRAY_NORMALIZED:
      *params = (float)(ab->normalize_ ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_CURRENT_VERTEX_ATTRIB:
      params[0] = ab->generic_values_[0];
      params[1] = ab->generic_values_[1];
      params[2] = ab->generic_values_[2];
      params[3] = ab->generic_values_[3];
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetVertexAttribiv)(gl_es2_uint index, gl_es2_enum pname, gl_es2_int *params) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (index > c->attribs_.num_attribs_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  struct attrib *ab = c->attribs_.attribs_ + index;

  switch (pname) {
    case GL_ES2_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING: {
      if (!ab->buf_) {
        *params = 0;
        return;
      }
      /* All buffers are a struct data_buffer sitting as a "struct data_buffer buf_"
       * field in an gl_es2_buffer structure; use that to revert from the buf_ to the
       * gl_es2_buffer that contains it.
       * NOTE: this is fragile and will implode if, for whatever reason, we stray
       *       and start using data_buffer structs outside of a gl_es2_buffer.
       * NOTE2: If you have problems with it here, you're going to have problems with it at glGetVertexAttribfv!!*/
      uintptr_t offset_to_buf_field = (uintptr_t)&(((struct gl_es2_buffer *)0)->buf_);
      struct gl_es2_buffer *gl_buf = (struct gl_es2_buffer *)(((char *)ab->buf_) - offset_to_buf_field);

      *params = (gl_es2_int)gl_buf->no_.name_;
      break;
    }
    case GL_ES2_VERTEX_ATTRIB_ARRAY_ENABLED:
      *params = (gl_es2_int)(ab->enabled_ ? GL_ES2_TRUE : GL_ES2_FALSE);
      break;
    case GL_ES2_VERTEX_ATTRIB_ARRAY_SIZE:
      *params = (gl_es2_int)(ab->size_);
      break;
    case GL_ES2_VERTEX_ATTRIB_ARRAY_STRIDE:
      *params = (gl_es2_int)(ab->stride_);
      break;
    case GL_ES2_VERTEX_ATTRIB_ARRAY_TYPE:
      switch (ab->data_type_) {
        case ADT_BYTE:
          *params = GL_ES2_BYTE;
          break;
        case ADT_UNSIGNED_BYTE:
          *params = GL_ES2_UNSIGNED_BYTE;
          break;
        case ADT_SHORT:
          *params = GL_ES2_SHORT;
          break;
        case ADT_UNSIGNED_SHORT:
          *params = GL_ES2_UNSIGNED_SHORT;
          break;
        case ADT_INT:
          *params = GL_ES2_INT;
          break;
        case ADT_UNSIGNED_INT:
          *params = GL_ES2_UNSIGNED_INT;
          break;
        case ADT_FIXED:
          *params = GL_ES2_FIXED;
          break;
        case ADT_FLOAT:
          *params = GL_ES2_FLOAT;
          break;
      }
      break;
    case GL_ES2_VERTEX_ATTRIB_ARRAY_NORMALIZED:
      *params = ab->normalize_ ? GL_ES2_TRUE : GL_ES2_FALSE;
      break;
    case GL_ES2_CURRENT_VERTEX_ATTRIB:
      params[0] = (gl_es2_int)ab->generic_values_[0];
      params[1] = (gl_es2_int)ab->generic_values_[1];
      params[2] = (gl_es2_int)ab->generic_values_[2];
      params[3] = (gl_es2_int)ab->generic_values_[3];
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(GetVertexAttribPointerv)(gl_es2_uint index, gl_es2_enum pname, void **pointer) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (index > c->attribs_.num_attribs_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  struct attrib *ab = c->attribs_.attribs_ + index;

  switch (pname) {
    case GL_ES2_VERTEX_ATTRIB_ARRAY_POINTER:
      *pointer = ab->ptr_;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Hint)(gl_es2_enum target, gl_es2_enum mode) {
  switch (target) {
    case GL_ES2_GENERATE_MIPMAP_HINT:
      switch (mode) {
        case GL_ES2_FASTEST:
        case GL_ES2_NICEST:
        case GL_ES2_DONT_CARE:
          /* Thank you for this valid hint. */
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
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
  struct gl_es2_context *c = gl_es2_ctx();
  int result = 0;

  switch (cap) {
    case GL_ES2_BLEND:
      result = c->is_blend_enabled_;
      break;
    case GL_ES2_CULL_FACE:
      result = c->is_culling_enabled_;
      break;
    case GL_ES2_DEPTH_TEST:
      result = c->is_depth_test_enabled_;
      break;
    case GL_ES2_DITHER:
      result = c->is_dither_enabled_;
      break;
    case GL_ES2_POLYGON_OFFSET_FILL:
      result = c->is_polygon_offset_fill_enabled_;
      break;
    case GL_ES2_SAMPLE_ALPHA_TO_COVERAGE:
      result = c->is_sample_alpha_to_coverage_enabled_;
      break;
    case GL_ES2_SAMPLE_COVERAGE:
      result = c->is_sample_coverage_enabled_;
      break;
    case GL_ES2_SCISSOR_TEST:
      result = c->is_scissor_test_enabled_;
      break;
    case GL_ES2_STENCIL_TEST:
      result = c->is_stencil_test_enabled_;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return 0;
  }

  return result ? GL_ES2_TRUE : GL_ES2_FALSE;
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
  struct gl_es2_context *c = gl_es2_ctx();
  if (!program) return GL_ES2_FALSE;
  if (not_find(&c->program_not_, program)) return GL_ES2_TRUE;
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
  struct gl_es2_context *c = gl_es2_ctx();
  if (!shader) return GL_ES2_FALSE;
  if (not_find(&c->shader_not_, shader)) return GL_ES2_TRUE;
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
  struct gl_es2_context *c = gl_es2_ctx();
  c->line_width_ = width;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(LinkProgram)(gl_es2_uint program) {
  struct gl_es2_context *c = gl_es2_ctx();

  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = NULL;
  prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  prog->program_.gl_last_link_status_ = 0;

  sl_info_log_clear(&prog->program_.log_);

  int r;
  r = sl_program_link(&prog->program_);
  switch (r) {
    case SL_ERR_INVALID_ARG:
    case SL_ERR_INTERNAL:
      dx_error(&prog->program_.log_.dx_, "An internal error occurred\n");
      return;
    case SL_ERR_NO_MEM:
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    case SL_ERR_OK:
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(PixelStorei)(gl_es2_enum pname, gl_es2_int param) {
  struct gl_es2_context *c = gl_es2_ctx();

  switch (param) {
    case 1:
    case 2:
    case 4:
    case 8:
      /* valid value */
      break;
    default:
      set_gl_err(GL_ES2_INVALID_VALUE);
      return;
  }

  switch (pname) {
    case GL_ES2_PACK_ALIGNMENT:
      c->pack_alignment_ = (int)param;
      break;
    case GL_ES2_UNPACK_ALIGNMENT:
      c->unpack_alignment_ = (int)param;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(PolygonOffset)(gl_es2_float factor, gl_es2_float units) {
  struct gl_es2_context *c = gl_es2_ctx();
  c->polygon_offset_factor_ = factor;
  c->polygon_offset_units_ = units;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ReadPixels)(gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height, gl_es2_enum format, gl_es2_enum type, void *pixels) {
  struct gl_es2_context *c = gl_es2_ctx();

  if (gl_es2_framebuffer_complete != gl_es2_framebuffer_check_completeness(c->framebuffer_)) {
    set_gl_err(GL_ES2_INVALID_FRAMEBUFFER_OPERATION);
    return;
  }

  if ((width < 0) || (height < 0)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  if ((type == GL_ES2_UNSIGNED_SHORT_5_6_5) && (format != GL_ES2_RGB)) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  if (((type == GL_ES2_UNSIGNED_SHORT_4_4_4_4) ||
       (type == GL_ES2_UNSIGNED_SHORT_5_5_5_1)) &&
      (format != GL_ES2_RGBA)) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  size_t bytes_per_pixel = 0;
  enum blitter_format blit_dst_format;
  switch (type) {
    case GL_ES2_UNSIGNED_BYTE:
      switch (format) {
        case GL_ES2_ALPHA:
          bytes_per_pixel = 1;
          blit_dst_format = blit_format_alpha;
          break;
        case GL_ES2_RGB:
          bytes_per_pixel = 3;
          blit_dst_format = blit_format_rgb;
          break;
        case GL_ES2_RGBA:
          bytes_per_pixel = 4;
          blit_dst_format = blit_format_rgba;
          break;
      }
      break;
    case GL_ES2_UNSIGNED_SHORT_5_6_5:
      bytes_per_pixel = 2;
      blit_dst_format = blit_format_565;
      break;
    case GL_ES2_UNSIGNED_SHORT_4_4_4_4:
      bytes_per_pixel = 2;
      blit_dst_format = blit_format_4444;
      break;
    case GL_ES2_UNSIGNED_SHORT_5_5_5_1:
      bytes_per_pixel = 2;
      blit_dst_format = blit_format_5551;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  size_t bytes_per_row = bytes_per_pixel * (size_t)width;
  size_t dst_stride = (bytes_per_row + (size_t)c->pack_alignment_) & ~(((size_t)c->pack_alignment_) - 1);

  void *fb_data = NULL;
  size_t fb_stride = 0;

  gl_es2_framebuffer_attachment_raw_ptr(&c->framebuffer_->color_attachment0_, &fb_data, &fb_stride);

  int bottom_row = gl_es2_framebuffer_get_bitmap_row_num(c->framebuffer_, y);

  /* framebuffer pointer to last row, stride negative (so increasing Y goes back in memory) */
  void *fb_data_bottom_left = ((char *)fb_data) + ((size_t)(height - 1)) * fb_stride;
  size_t fb_stride_going_up = (size_t)-(intptr_t)fb_stride; /* two's complement, this'll work, cast to prevent signed/unsigned warnings on - */

  blitter_blit_format(pixels, blit_dst_format, fb_data_bottom_left, blit_format_rgba, dst_stride, 0, 0, fb_stride_going_up, x, bottom_row, width, height);

  /* Check if we need to mask anything */
  switch (format) {
    case GL_ES2_ALPHA:
      /* mask out RGB to zero */
      switch (type) {
        case GL_ES2_UNSIGNED_BYTE:
          /* no need to mask, result was just alpha bytes */
          break;
        case GL_ES2_UNSIGNED_SHORT_5_6_5:
          /* Set everything to 0; there is no alpha channel yet everything else is 0 */
          blitter_blit_apply_mask16(pixels, dst_stride, 0xFFFF, 0x0000, 0, 0, width, height);
          break;
        case GL_ES2_UNSIGNED_SHORT_4_4_4_4:
          /* Clear everything but the alpha channel */
          blitter_blit_apply_mask16(pixels, dst_stride, 0xFFF0, 0x0000, 0, 0, width, height);
          break;
        case GL_ES2_UNSIGNED_SHORT_5_5_5_1:
          /* Clear everything but the alpha bit */
          blitter_blit_apply_mask16(pixels, dst_stride, 0xFFFE, 0x0000, 0, 0, width, height);
          break;
      }
    case GL_ES2_RGB:
      /* mask out alpha to zero */
      switch (type) {
        case GL_ES2_UNSIGNED_BYTE:
          /* no need to mask, result was just RGB bytes, no alpha channel */
          break;
        case GL_ES2_UNSIGNED_SHORT_5_6_5:
          /* Set alpha to 0; but there is no alpha so we're done */
          break;
        case GL_ES2_UNSIGNED_SHORT_4_4_4_4:
          /* Clear alpha channel */
          blitter_blit_apply_mask16(pixels, dst_stride, 0x000F, 0x0000, 0, 0, width, height);
          break;
        case GL_ES2_UNSIGNED_SHORT_5_5_5_1:
          /* Clear the alpha bit */
          blitter_blit_apply_mask16(pixels, dst_stride, 0x0001, 0x0000, 0, 0, width, height);
          break;
      }
      break;
    case GL_ES2_RGBA:
      /* nothing to mask, keep all the bits */
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ReleaseShaderCompiler)(void) {
  /* The hint is taken ... and ignored. */
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(RenderbufferStorage)(gl_es2_enum target, gl_es2_enum internalformat, gl_es2_sizei width, gl_es2_sizei height) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (target != GL_ES2_RENDERBUFFER) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }
  if (!c->renderbuffer_) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  if ((width < 0) || (height < 0)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  if ((width >= GL_ES2_IMPL_MAX_VIEWPORT_DIMS) || (height >= GL_ES2_IMPL_MAX_VIEWPORT_DIMS)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  enum gl_es2_renderbuffer_format actual_format;

  switch (internalformat) {
    case GL_ES2_RGBA4:
    case GL_ES2_RGB565:
    case GL_ES2_RGBA5_A1:
      /* You can have any format you want, as long as it is RGBA32 */
      actual_format = gl_es2_renderbuffer_format_rgba32;
      break;
    case GL_ES2_DEPTH_COMPONENT16:
      actual_format = gl_es2_renderbuffer_format_depth16;
      break;
    case GL_ES2_DEPTH_COMPONENT24:
    case GL_ES2_DEPTH_COMPONENT32:
      actual_format = gl_es2_renderbuffer_format_depth32;
      break;
    case GL_ES2_STENCIL_INDEX8:
      actual_format = gl_es2_renderbuffer_format_stencil16;
      break;
  }
  int r;
  r = gl_es2_renderbuffer_storage(c->renderbuffer_, actual_format, width, height);
  switch (r) {
    case SL_ERR_INVALID_ARG:
    case SL_ERR_OVERFLOW:
    case SL_ERR_INTERNAL:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    case SL_ERR_NO_MEM:
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(SampleCoverage)(gl_es2_float value, gl_es2_boolean invert) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (value < 0.f) value = 0.f;
  if (value > 1.f) value = 1.f;
  c->sample_coverage_value_ = value;
  c->sample_coverage_invert_ = invert ? 1 : 0;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Scissor)(gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height) {
  struct gl_es2_context *c = gl_es2_ctx();
  if ((width < 0) || (height < 0)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  /* Some more sanity checking beyond the spec, so we can feed it into internals without too much hassle */

  /* can it fit our 32 bit numbers.. */
  if ((width > INT32_MAX) ||
      (height > INT32_MAX) ||
      (x > INT32_MAX) ||
      (x < INT32_MIN) ||
      (y > INT32_MAX) ||
      (y < INT32_MIN)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  /* do the extents not exceed our 32 bit numbers... */
  int32_t w32 = (int32_t)width;
  int32_t h32 = (int32_t)height;

  /* width and height guaranteed positive; so only need to check overflow */
  int32_t x32 = w32 + x;
  int32_t y32 = h32 + y;
  if ((x32 < x) ||
      (y32 < y)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  c->scissor_left_ = x;
  c->scissor_bottom_counted_from_bottom_ = y;
  c->scissor_width_ = width;
  c->scissor_height_ = height;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ShaderBinary)(gl_es2_sizei count, const gl_es2_uint *shaders, gl_es2_enum binaryformat, const void *binary, gl_es2_sizei length) {
  /* No shader binary support */
  set_gl_err(GL_ES2_INVALID_OPERATION);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(ShaderSource)(gl_es2_uint shader, gl_es2_sizei count, const gl_es2_char *const *string, const gl_es2_int *length) {
  struct gl_es2_context *c = gl_es2_ctx();
  uintptr_t shad_name = (uintptr_t)shader;
  struct gl_es2_shader *shad = NULL;
  shad = (struct gl_es2_shader *)not_find(&c->shader_not_, shad_name);
  if (!shad) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (count < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  /* guarantee that sizeof(gl_es2_int) == sizeof(int), that is to say, that we can pass the array
   * (poor man's compile time assert) */
  char check_gl_es2_int_is_an_int[1 - 2 * !(sizeof(gl_es2_int) == sizeof(int))];
  (void)check_gl_es2_int_is_an_int; /* dismiss unreferenced local variable */
  r = sl_shader_set_source(&shad->shader_, count, (const char * const *)string, length);
  switch (r) {
    case SL_ERR_INVALID_ARG:
    case SL_ERR_OVERFLOW:
      set_gl_err(GL_ES2_INVALID_VALUE);
      return;
    case SL_ERR_INTERNAL:
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    case SL_ERR_NO_MEM:
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    case SL_ERR_OK:
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilFunc)(gl_es2_enum func, gl_es2_int ref, gl_es2_uint mask) {
  struct gl_es2_context *c = gl_es2_ctx();
  primitive_assembly_stencil_func_t stencil_func;
  switch (func) {
    case GL_ES2_NEVER:    stencil_func = PASF_NEVER; break;
    case GL_ES2_LESS:     stencil_func = PASF_LESS; break;
    case GL_ES2_LEQUAL:   stencil_func = PASF_LEQUAL; break;
    case GL_ES2_GREATER:  stencil_func = PASF_GREATER; break;
    case GL_ES2_GEQUAL:   stencil_func = PASF_GEQUAL; break;
    case GL_ES2_EQUAL:    stencil_func = PASF_EQUAL; break;
    case GL_ES2_NOTEQUAL: stencil_func = PASF_NOTEQUAL; break;
    case GL_ES2_ALWAYS:   stencil_func = PASF_ALWAYS; break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  c->stencil_func_ = stencil_func;
  c->stencil_func_ref_ = (uint32_t)ref;
  c->stencil_func_mask_ = (uint32_t)mask;
  c->stencil_back_func_ = stencil_func;
  c->stencil_back_func_ref_ = (uint32_t)ref;
  c->stencil_back_func_mask_ = (uint32_t)mask;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilFuncSeparate)(gl_es2_enum face, gl_es2_enum func, gl_es2_int ref, gl_es2_uint mask) {
  struct gl_es2_context *c = gl_es2_ctx();
  primitive_assembly_stencil_func_t stencil_func;
  switch (func) {
    case GL_ES2_NEVER:    stencil_func = PASF_NEVER; break;
    case GL_ES2_LESS:     stencil_func = PASF_LESS; break;
    case GL_ES2_LEQUAL:   stencil_func = PASF_LEQUAL; break;
    case GL_ES2_GREATER:  stencil_func = PASF_GREATER; break;
    case GL_ES2_GEQUAL:   stencil_func = PASF_GEQUAL; break;
    case GL_ES2_EQUAL:    stencil_func = PASF_EQUAL; break;
    case GL_ES2_NOTEQUAL: stencil_func = PASF_NOTEQUAL; break;
    case GL_ES2_ALWAYS:   stencil_func = PASF_ALWAYS; break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  if ((face != GL_ES2_FRONT) && (face != GL_ES2_BACK) && (face != GL_ES2_FRONT_AND_BACK)) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }

  if ((face == GL_ES2_FRONT) || (face == GL_ES2_FRONT_AND_BACK)) {
    c->stencil_func_ = stencil_func;
    c->stencil_func_ref_ = (uint32_t)ref;
    c->stencil_func_mask_ = (uint32_t)mask;
  }
  if ((face == GL_ES2_BACK) || (face == GL_ES2_FRONT_AND_BACK)) {
    c->stencil_back_func_ = stencil_func;
    c->stencil_back_func_ref_ = (uint32_t)ref;
    c->stencil_back_func_mask_ = (uint32_t)mask;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilMask)(gl_es2_uint mask) {
  struct gl_es2_context *c = gl_es2_ctx();
  c->stencil_writemask_ = (uint32_t)mask;
  c->stencil_back_writemask_ = (uint32_t)mask;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilMaskSeparate)(gl_es2_enum face, gl_es2_uint mask) {
  struct gl_es2_context *c = gl_es2_ctx();
  switch (face) {
    case GL_ES2_FRONT_AND_BACK:
      c->stencil_writemask_ = (uint32_t)mask;
      c->stencil_back_writemask_ = (uint32_t)mask;
      break;
    case GL_ES2_FRONT:
      c->stencil_writemask_ = (uint32_t)mask;
      break;
    case GL_ES2_BACK:
      c->stencil_back_writemask_ = (uint32_t)mask;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilOp)(gl_es2_enum sfail, gl_es2_enum dpfail, gl_es2_enum dppass) {
  struct gl_es2_context *c = gl_es2_ctx();
  primitive_assembly_stencil_op_t sfail_op, dpfail_op, dppass_op;
  if (!stencil_op_gl_to_paso(sfail, &sfail_op) ||
    !stencil_op_gl_to_paso(dpfail, &dpfail_op) ||
    !stencil_op_gl_to_paso(dppass, &dppass_op)) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }

  c->stencil_sfail_ = sfail_op;
  c->stencil_zfail_ = dpfail_op;
  c->stencil_zpass_ = dppass_op;
  c->stencil_back_sfail_ = sfail_op;
  c->stencil_back_zfail_ = dpfail_op;
  c->stencil_back_zpass_ = dppass_op;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(StencilOpSeparate)(gl_es2_enum face, gl_es2_enum sfail, gl_es2_enum dpfail, gl_es2_enum dppass) {
  struct gl_es2_context *c = gl_es2_ctx();
  if ((face != GL_ES2_FRONT) && (face != GL_ES2_BACK) && (face != GL_ES2_FRONT_AND_BACK)) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }
  primitive_assembly_stencil_op_t sfail_op, dpfail_op, dppass_op;
  if (!stencil_op_gl_to_paso(sfail, &sfail_op) ||
      !stencil_op_gl_to_paso(dpfail, &dpfail_op) ||
      !stencil_op_gl_to_paso(dppass, &dppass_op)) {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }

  if ((face == GL_ES2_FRONT) || (face == GL_ES2_FRONT_AND_BACK)) {
    c->stencil_sfail_ = sfail_op;
    c->stencil_zfail_ = dpfail_op;
    c->stencil_zpass_ = dppass_op;
  }
  if ((face == GL_ES2_BACK) || (face == GL_ES2_FRONT_AND_BACK)) {
    c->stencil_back_sfail_ = sfail_op;
    c->stencil_back_zfail_ = dpfail_op;
    c->stencil_back_zpass_ = dppass_op;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexImage2D)(gl_es2_enum target, gl_es2_int level, gl_es2_int internalformat, gl_es2_sizei width, gl_es2_sizei height, gl_es2_int border, gl_es2_enum format, gl_es2_enum type, const void *pixels) {
  struct gl_es2_context *c = gl_es2_ctx();
  int r;
  struct gl_es2_texture *tex = NULL;
  struct sampler_2d *s2d = NULL;
  if (!get_active_tex_target(target, &tex, &s2d)) {
    /* error already set */
    return;
  }

  if ((target == GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_X) ||
      (target == GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_X) ||
      (target == GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Y) ||
      (target == GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Y) ||
      (target == GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Z) ||
      (target == GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Z)) {
    if (height != width) {
      /* one of the cube map targets and width & height are not equal */
      set_gl_err(GL_ES2_INVALID_VALUE);
      return;
    }
  }

  if ((type == GL_ES2_UNSIGNED_SHORT_5_6_5) && (format != GL_ES2_RGB)) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  if (((type == GL_ES2_UNSIGNED_SHORT_4_4_4_4) ||
       (type == GL_ES2_UNSIGNED_SHORT_5_5_5_1)) &&
      (format != GL_ES2_RGBA)) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  if (internalformat != format) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  if ((width < 0) || (height < 0)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  if ((width >= GL_ES2_IMPL_MAX_VIEWPORT_DIMS) || (height >= GL_ES2_IMPL_MAX_VIEWPORT_DIMS)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  if (border != 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  enum s2d_tex_components internal_components;
  enum blitter_format internal_blit_format;
  switch (internalformat) {
    case GL_ES2_ALPHA:
      internal_components = s2d_alpha;
      internal_blit_format = blit_format_alpha;
      break;
    case GL_ES2_LUMINANCE:
      internal_components = s2d_luminance;
      internal_blit_format = blit_format_luminance;
      break;
    case GL_ES2_LUMINANCE_ALPHA:
      internal_components = s2d_luminance_alpha;
      internal_blit_format = blit_format_luminance_alpha;
      break;
    case GL_ES2_RGB:
      internal_components = s2d_rgb;
      internal_blit_format = blit_format_rgb;
      break;
    case GL_ES2_RGBA:
      internal_components = s2d_rgba;
      internal_blit_format = blit_format_rgba;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  enum blitter_format src_blit_format;
  size_t src_bytes_per_pixel;
  switch (type) {
    case GL_ES2_UNSIGNED_BYTE:
      switch (format) {
        case GL_ES2_ALPHA:
          src_bytes_per_pixel = 1;
          src_blit_format = blit_format_alpha;
          break;
        case GL_ES2_LUMINANCE:
          src_bytes_per_pixel = 1;
          src_blit_format = blit_format_luminance;
          break;
        case GL_ES2_LUMINANCE_ALPHA:
          src_bytes_per_pixel = 2;
          src_blit_format = blit_format_luminance_alpha;
          break;
        case GL_ES2_RGB:
          src_bytes_per_pixel = 3;
          src_blit_format = blit_format_rgb;
          break;
        case GL_ES2_RGBA:
          src_bytes_per_pixel = 4;
          src_blit_format = blit_format_rgba;
          break;
      }
      break;
    case GL_ES2_UNSIGNED_SHORT_5_6_5:
      src_bytes_per_pixel = 2;
      src_blit_format = blit_format_565;
      break;
    case GL_ES2_UNSIGNED_SHORT_4_4_4_4:
      src_bytes_per_pixel = 2;
      src_blit_format = blit_format_4444;
      break;
    case GL_ES2_UNSIGNED_SHORT_5_5_5_1:
      src_bytes_per_pixel = 2;
      src_blit_format = blit_format_5551;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  r = sampler_2d_set_storage(s2d, level, internal_components, width, height);
  if (r == SL_ERR_INVALID_ARG) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  else if (r == SL_ERR_NO_MEM) {
    set_gl_err(GL_ES2_OUT_OF_MEMORY);
    return;
  }
  else if (r != SL_ERR_OK) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  size_t src_stride = (src_bytes_per_pixel * width + c->unpack_alignment_ - 1) & ~(size_t)(c->unpack_alignment_ - 1);
  /* Invert the stride so it goes down the bitmap, and not up */
  size_t src_downwards_stride = (size_t)-(intptr_t)src_stride;

  blitter_blit_format(s2d->mipmaps_[level].bitmap_, internal_blit_format, ((char *)pixels) + src_stride * (height - 1), src_blit_format,
                      s2d->mipmaps_[level].num_bytes_per_bitmap_row_, 0, 0,
                      src_downwards_stride,
                      0, 0, width, height);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexParameterf)(gl_es2_enum target, gl_es2_enum pname, gl_es2_float param) {
  struct gl_es2_context *c = gl_es2_ctx();

  size_t num_s2ds = 0;
  struct sampler_2d *s2ds = NULL;

  if (target == GL_ES2_TEXTURE_2D) {
    if (c->active_texture_units_[c->current_active_texture_unit_].texture_2d_) {
      s2ds = &c->active_texture_units_[c->current_active_texture_unit_].texture_2d_->texture_2d_;
      num_s2ds = 1;
    }
    else {
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  else if (target == GL_ES2_TEXTURE_CUBE_MAP) {
    if (c->active_texture_units_[c->current_active_texture_unit_].texture_cube_map_) {
      s2ds = c->active_texture_units_[c->current_active_texture_unit_].texture_cube_map_->texture_cube_maps_;
      num_s2ds = 6;
    }
    else {
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  else {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }

  size_t n;
  switch (pname) {
    case GL_ES2_TEXTURE_MIN_FILTER:
      switch ((int)param) {
        case GL_ES2_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_nearest;
          break;
        case GL_ES2_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_linear;
          break;
        case GL_ES2_NEAREST_MIPMAP_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_nearest_mipmap_nearest;
          break;
        case GL_ES2_LINEAR_MIPMAP_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_linear_mipmap_nearest;
          break;
        case GL_ES2_NEAREST_MIPMAP_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_nearest_mipmap_linear;
          break;
        case GL_ES2_LINEAR_MIPMAP_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_linear_mipmap_linear;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    case GL_ES2_TEXTURE_MAG_FILTER:
      switch ((int)param) {
        case GL_ES2_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].mag_filter_ = s2d_nearest;
          break;
        case GL_ES2_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].mag_filter_ = s2d_linear;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    case GL_ES2_TEXTURE_WRAP_S:
      switch ((int)param) {
        case GL_ES2_CLAMP_TO_EDGE:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_s_ = s2d_clamp_to_edge;
          break;
        case GL_ES2_MIRRORED_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_s_ = s2d_mirrored_repeat;
          break;
        case GL_ES2_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_s_ = s2d_repeat;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    case GL_ES2_TEXTURE_WRAP_T:
      switch ((int)param) {
        case GL_ES2_CLAMP_TO_EDGE:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_t_ = s2d_clamp_to_edge;
          break;
        case GL_ES2_MIRRORED_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_t_ = s2d_mirrored_repeat;
          break;
        case GL_ES2_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_t_ = s2d_repeat;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexParameterfv)(gl_es2_enum target, gl_es2_enum pname, const gl_es2_float *params) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (!params) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  size_t num_s2ds = 0;
  struct sampler_2d *s2ds = NULL;

  if (target == GL_ES2_TEXTURE_2D) {
    if (c->active_texture_units_[c->current_active_texture_unit_].texture_2d_) {
      s2ds = &c->active_texture_units_[c->current_active_texture_unit_].texture_2d_->texture_2d_;
      num_s2ds = 1;
    }
    else {
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  else if (target == GL_ES2_TEXTURE_CUBE_MAP) {
    if (c->active_texture_units_[c->current_active_texture_unit_].texture_cube_map_) {
      s2ds = c->active_texture_units_[c->current_active_texture_unit_].texture_cube_map_->texture_cube_maps_;
      num_s2ds = 6;
    }
    else {
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  else {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }

  size_t n;
  switch (pname) {
    case GL_ES2_TEXTURE_MIN_FILTER:
      switch ((int)params[0]) {
        case GL_ES2_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_nearest;
          break;
        case GL_ES2_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_linear;
          break;
        case GL_ES2_NEAREST_MIPMAP_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_nearest_mipmap_nearest;
          break;
        case GL_ES2_LINEAR_MIPMAP_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_linear_mipmap_nearest;
          break;
        case GL_ES2_NEAREST_MIPMAP_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_nearest_mipmap_linear;
          break;
        case GL_ES2_LINEAR_MIPMAP_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_linear_mipmap_linear;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    case GL_ES2_TEXTURE_MAG_FILTER:
      switch ((int)params[0]) {
        case GL_ES2_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].mag_filter_ = s2d_nearest;
          break;
        case GL_ES2_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].mag_filter_ = s2d_linear;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    case GL_ES2_TEXTURE_WRAP_S:
      switch ((int)params[0]) {
        case GL_ES2_CLAMP_TO_EDGE:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_s_ = s2d_clamp_to_edge;
          break;
        case GL_ES2_MIRRORED_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_s_ = s2d_mirrored_repeat;
          break;
        case GL_ES2_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_s_ = s2d_repeat;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    case GL_ES2_TEXTURE_WRAP_T:
      switch ((int)params[0]) {
        case GL_ES2_CLAMP_TO_EDGE:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_t_ = s2d_clamp_to_edge;
          break;
        case GL_ES2_MIRRORED_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_t_ = s2d_mirrored_repeat;
          break;
        case GL_ES2_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_t_ = s2d_repeat;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexParameteri)(gl_es2_enum target, gl_es2_enum pname, gl_es2_int param) {
  struct gl_es2_context *c = gl_es2_ctx();

  size_t num_s2ds = 0;
  struct sampler_2d *s2ds = NULL;

  if (target == GL_ES2_TEXTURE_2D) {
    if (c->active_texture_units_[c->current_active_texture_unit_].texture_2d_) {
      s2ds = &c->active_texture_units_[c->current_active_texture_unit_].texture_2d_->texture_2d_;
      num_s2ds = 1;
    }
    else {
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  else if (target == GL_ES2_TEXTURE_CUBE_MAP) {
    if (c->active_texture_units_[c->current_active_texture_unit_].texture_cube_map_) {
      s2ds = c->active_texture_units_[c->current_active_texture_unit_].texture_cube_map_->texture_cube_maps_;
      num_s2ds = 6;
    }
    else {
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  else {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }

  size_t n;
  switch (pname) {
    case GL_ES2_TEXTURE_MIN_FILTER:
      switch (param) {
        case GL_ES2_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_nearest;
          break;
        case GL_ES2_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_linear;
          break;
        case GL_ES2_NEAREST_MIPMAP_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_nearest_mipmap_nearest;
          break;
        case GL_ES2_LINEAR_MIPMAP_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_linear_mipmap_nearest;
          break;
        case GL_ES2_NEAREST_MIPMAP_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_nearest_mipmap_linear;
          break;
        case GL_ES2_LINEAR_MIPMAP_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_linear_mipmap_linear;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    case GL_ES2_TEXTURE_MAG_FILTER:
      switch (param) {
        case GL_ES2_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].mag_filter_ = s2d_nearest;
          break;
        case GL_ES2_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].mag_filter_ = s2d_linear;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    case GL_ES2_TEXTURE_WRAP_S:
      switch (param) {
        case GL_ES2_CLAMP_TO_EDGE:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_s_ = s2d_clamp_to_edge;
          break;
        case GL_ES2_MIRRORED_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_s_ = s2d_mirrored_repeat;
          break;
        case GL_ES2_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_s_ = s2d_repeat;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    case GL_ES2_TEXTURE_WRAP_T:
      switch (param) {
        case GL_ES2_CLAMP_TO_EDGE:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_t_ = s2d_clamp_to_edge;
          break;
        case GL_ES2_MIRRORED_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_t_ = s2d_mirrored_repeat;
          break;
        case GL_ES2_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_t_ = s2d_repeat;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexParameteriv)(gl_es2_enum target, gl_es2_enum pname, const gl_es2_int *params) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (!params) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  size_t num_s2ds = 0;
  struct sampler_2d *s2ds = NULL;

  if (target == GL_ES2_TEXTURE_2D) {
    if (c->active_texture_units_[c->current_active_texture_unit_].texture_2d_) {
      s2ds = &c->active_texture_units_[c->current_active_texture_unit_].texture_2d_->texture_2d_;
      num_s2ds = 1;
    }
    else {
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  else if (target == GL_ES2_TEXTURE_CUBE_MAP) {
    if (c->active_texture_units_[c->current_active_texture_unit_].texture_cube_map_) {
      s2ds = c->active_texture_units_[c->current_active_texture_unit_].texture_cube_map_->texture_cube_maps_;
      num_s2ds = 6;
    }
    else {
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  else {
    set_gl_err(GL_ES2_INVALID_ENUM);
    return;
  }

  size_t n;
  switch (pname) {
    case GL_ES2_TEXTURE_MIN_FILTER:
      switch (params[0]) {
        case GL_ES2_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_nearest;
          break;
        case GL_ES2_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_linear;
          break;
        case GL_ES2_NEAREST_MIPMAP_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_nearest_mipmap_nearest;
          break;
        case GL_ES2_LINEAR_MIPMAP_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_linear_mipmap_nearest;
          break;
        case GL_ES2_NEAREST_MIPMAP_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_nearest_mipmap_linear;
          break;
        case GL_ES2_LINEAR_MIPMAP_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].min_filter_ = s2d_linear_mipmap_linear;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    case GL_ES2_TEXTURE_MAG_FILTER:
      switch (params[0]) {
        case GL_ES2_NEAREST:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].mag_filter_ = s2d_nearest;
          break;
        case GL_ES2_LINEAR:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].mag_filter_ = s2d_linear;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    case GL_ES2_TEXTURE_WRAP_S:
      switch (params[0]) {
        case GL_ES2_CLAMP_TO_EDGE:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_s_ = s2d_clamp_to_edge;
          break;
        case GL_ES2_MIRRORED_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_s_ = s2d_mirrored_repeat;
          break;
        case GL_ES2_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_s_ = s2d_repeat;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    case GL_ES2_TEXTURE_WRAP_T:
      switch (params[0]) {
        case GL_ES2_CLAMP_TO_EDGE:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_t_ = s2d_clamp_to_edge;
          break;
        case GL_ES2_MIRRORED_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_t_ = s2d_mirrored_repeat;
          break;
        case GL_ES2_REPEAT:
          for (n = 0; n < num_s2ds; ++n) s2ds[n].wrap_t_ = s2d_repeat;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(TexSubImage2D)(gl_es2_enum target, gl_es2_int level, 
                                                                                 gl_es2_int x, gl_es2_int y, 
                                                                                 gl_es2_sizei width, gl_es2_sizei height, 
                                                                                 gl_es2_enum format, gl_es2_enum type, const void *pixels) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_texture *tex = NULL;
  struct sampler_2d *s2d = NULL;
  if (!get_active_tex_target(target, &tex, &s2d)) {
    /* error already set */
    return;
  }

  if ((level < 0) || (level >= SAMPLER_2D_MAX_NUM_MIPMAPS)) {
    /* level is not in range */
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  if (level >= s2d->num_maps_) {
    /* Level texture array has not yet been defined */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  if ((x < 0) || (y < 0)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  if ((width < 0) || (height < 0)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  if (((x + width) > s2d->mipmaps_[level].width_) ||
      ((y + height) > s2d->mipmaps_[level].height_)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }

  switch (format) {
    case GL_ES2_ALPHA:
    case GL_ES2_LUMINANCE:
    case GL_ES2_LUMINANCE_ALPHA:
    case GL_ES2_RGB:
    case GL_ES2_RGBA:
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  if ((type == GL_ES2_UNSIGNED_SHORT_5_6_5) && (format != GL_ES2_RGB)) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  if (((type == GL_ES2_UNSIGNED_SHORT_4_4_4_4) ||
       (type == GL_ES2_UNSIGNED_SHORT_5_5_5_1)) &&
      (format != GL_ES2_RGBA)) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  enum blitter_format src_blit_format;
  size_t src_bytes_per_pixel;
  switch (type) {
    case GL_ES2_UNSIGNED_BYTE:
      switch (format) {
        case GL_ES2_ALPHA:
          src_bytes_per_pixel = 1;
          src_blit_format = blit_format_alpha;
          break;
        case GL_ES2_LUMINANCE:
          src_bytes_per_pixel = 1;
          src_blit_format = blit_format_luminance;
          break;
        case GL_ES2_LUMINANCE_ALPHA:
          src_bytes_per_pixel = 2;
          src_blit_format = blit_format_luminance_alpha;
          break;
        case GL_ES2_RGB:
          src_bytes_per_pixel = 3;
          src_blit_format = blit_format_rgb;
          break;
        case GL_ES2_RGBA:
          src_bytes_per_pixel = 4;
          src_blit_format = blit_format_rgba;
          break;
        default:
          set_gl_err(GL_ES2_INVALID_ENUM);
          return;
      }
      break;
    case GL_ES2_UNSIGNED_SHORT_5_6_5:
      src_bytes_per_pixel = 2;
      src_blit_format = blit_format_565;
      break;
    case GL_ES2_UNSIGNED_SHORT_4_4_4_4:
      src_bytes_per_pixel = 2;
      src_blit_format = blit_format_4444;
      break;
    case GL_ES2_UNSIGNED_SHORT_5_5_5_1:
      src_bytes_per_pixel = 2;
      src_blit_format = blit_format_5551;
      break;
    default:
      set_gl_err(GL_ES2_INVALID_ENUM);
      return;
  }

  enum blitter_format texture_blit_format;
  switch (s2d->mipmaps_[level].components_) {
    case s2d_alpha:
      texture_blit_format = blit_format_alpha;
      break;
    case s2d_luminance:
      texture_blit_format = blit_format_luminance;
      break;
    case s2d_luminance_alpha:
      texture_blit_format = blit_format_luminance_alpha;
      break;
    case s2d_rgb:
      texture_blit_format = blit_format_rgb;
      break;
    case s2d_rgba:
      texture_blit_format = blit_format_rgba;
      break;
  }

  size_t src_stride = (src_bytes_per_pixel * width + c->unpack_alignment_ - 1) & ~(size_t)(c->unpack_alignment_ - 1);
  /* Invert the stride so it goes down the bitmap, and not up */
  size_t src_downwards_stride = (size_t)-(intptr_t)src_stride;
  char *src_top_left = ((char *)pixels) + src_stride * (height - 1);

  blitter_blit_format(s2d->mipmaps_[level].bitmap_, texture_blit_format, src_top_left, src_blit_format,
                      s2d->mipmaps_[level].num_bytes_per_bitmap_row_, x, y,
                      src_downwards_stride,
                      0, 0, width, height);
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform1f)(gl_es2_int location, gl_es2_float v0) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, NULL, NULL);
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_float: *(float *)mem = v0; break;
    case slrak_bool: *(uint8_t *)mem = (uint8_t)(v0 ? 0xFF : 0x00); break;
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform1fv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  if (count < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  size_t final_array_size = 0, entry_in_final_array = 0;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, &final_array_size, &entry_in_final_array);
  size_t entries_remaining = final_array_size - entry_in_final_array;
  size_t num_entries = (size_t)count;
  size_t n;
  if (entries_remaining < (size_t)count) {
    /* This would overflow the array, or this is not an array; if not an array, report error,
     * otherwise limit the range of what we'll copy. */
    if (entries_remaining > 1) {
      /* This is an array, copy what we can */
      num_entries = entries_remaining;
    }
    else {
      /* This is not an array, error out */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_float: {
      for (n = 0; n < num_entries; ++n) {
        ((float *)mem)[n] = value[n];
      }
      break;
    }
    case slrak_bool: {
      for (n = 0; n < num_entries; ++n) {
        ((uint8_t *)mem)[n] = (uint8_t)(value[n] ? 0xFF : 0x00);
      }
      break;
    }
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform1i)(gl_es2_int location, gl_es2_int v0) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, NULL, NULL);
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_int: *(int64_t *)mem = v0; break;
    case slrak_bool: *(uint8_t *)mem = (uint8_t)(v0 ? 0xFF : 0x00); break;
    case slrak_sampler2D: *(uint32_t *)mem = (uint32_t)v0; break;
    case slrak_samplerCube: *(uint32_t *)mem = (uint32_t)v0; break;
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform1iv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  if (count < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  size_t final_array_size = 0, entry_in_final_array = 0;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, &final_array_size, &entry_in_final_array);
  size_t entries_remaining = final_array_size - entry_in_final_array;
  size_t num_entries = (size_t)count;
  size_t n;
  if (entries_remaining < (size_t)count) {
    /* This would overflow the array, or this is not an array; if not an array, report error,
     * otherwise limit the range of what we'll copy. */
    if (entries_remaining > 1) {
      /* This is an array, copy what we can */
      num_entries = entries_remaining;
    }
    else {
      /* This is not an array, error out */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_int: {
      for (n = 0; n < num_entries; ++n) {
        ((int64_t *)mem)[n] = (int64_t)value[n];
      }
      break;
    }
    case slrak_bool: {
      for (n = 0; n < num_entries; ++n) {
        ((uint8_t *)mem)[n] = (uint8_t)(value[n] ? 0xFF : 0x00);
      }
      break;
    }
    case slrak_sampler2D: 
      for (n = 0; n < num_entries; ++n) {
        ((uint32_t *)mem)[n] = (uint32_t)value[n];
      }
      break;

    case slrak_samplerCube: 
      for (n = 0; n < num_entries; ++n) {
        ((uint32_t *)mem)[n] = (uint32_t)value[n];
      }
      break;
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform2f)(gl_es2_int location, gl_es2_float v0, gl_es2_float v1) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, NULL, NULL);
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_vec2: 
      ((float *)mem)[0] = v0;
      ((float *)mem)[1] = v1;
      break;
    case slrak_bvec2: 
      ((uint8_t *)mem)[0] = (uint8_t)(v0 ? 0xFF : 0x00);
      ((uint8_t *)mem)[1] = (uint8_t)(v1 ? 0xFF : 0x00);
      break;
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform2fv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  if (count < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  size_t final_array_size = 0, entry_in_final_array = 0;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, &final_array_size, &entry_in_final_array);
  size_t entries_remaining = final_array_size - entry_in_final_array;
  size_t num_entries = (size_t)count;
  size_t n;
  if (entries_remaining < (size_t)count) {
    /* This would overflow the array, or this is not an array; if not an array, report error,
     * otherwise limit the range of what we'll copy. */
    if (entries_remaining > 1) {
      /* This is an array, copy what we can */
      num_entries = entries_remaining;
    }
    else {
      /* This is not an array, error out */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_vec2: {
      for (n = 0; n < num_entries; ++n) {
        ((float *)mem)[n * 2 + 0] = value[n * 2 + 0];
        ((float *)mem)[n * 2 + 1] = value[n * 2 + 1];
      }
      break;
    }
    case slrak_bvec2: {
      for (n = 0; n < num_entries; ++n) {
        ((uint8_t *)mem)[n * 2 + 0] = (uint8_t)(value[n * 2 + 0] ? 0xFF : 0x00);
        ((uint8_t *)mem)[n * 2 + 1] = (uint8_t)(value[n * 2 + 1] ? 0xFF : 0x00);
      }
      break;
    }
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform2i)(gl_es2_int location, gl_es2_int v0, gl_es2_int v1) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, NULL, NULL);
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_ivec2: 
      ((int64_t *)mem)[0] = v0;
      ((int64_t *)mem)[1] = v1;
      break;
    case slrak_bvec2: 
      ((uint8_t *)mem)[0] = (uint8_t)(v0 ? 0xFF : 0x00);
      ((uint8_t *)mem)[1] = (uint8_t)(v1 ? 0xFF : 0x00);
      break;
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform2iv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  if (count < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  size_t final_array_size = 0, entry_in_final_array = 0;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, &final_array_size, &entry_in_final_array);
  size_t entries_remaining = final_array_size - entry_in_final_array;
  size_t num_entries = (size_t)count;
  size_t n;
  if (entries_remaining < (size_t)count) {
    /* This would overflow the array, or this is not an array; if not an array, report error,
     * otherwise limit the range of what we'll copy. */
    if (entries_remaining > 1) {
      /* This is an array, copy what we can */
      num_entries = entries_remaining;
    }
    else {
      /* This is not an array, error out */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_ivec2: {
      for (n = 0; n < num_entries; ++n) {
        ((int64_t *)mem)[n * 2 + 0] = (int64_t)value[n * 2 + 0];
        ((int64_t *)mem)[n * 2 + 1] = (int64_t)value[n * 2 + 1];
      }
      break;
    }
    case slrak_bvec2: {
      for (n = 0; n < num_entries; ++n) {
        ((uint8_t *)mem)[n * 2 + 0] = (uint8_t)(value[n * 2 + 0] ? 0xFF : 0x00);
        ((uint8_t *)mem)[n * 2 + 1] = (uint8_t)(value[n * 2 + 1] ? 0xFF : 0x00);
      }
      break;
    }
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform3f)(gl_es2_int location, gl_es2_float v0, gl_es2_float v1, gl_es2_float v2) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, NULL, NULL);
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_vec3:
      ((float *)mem)[0] = v0;
      ((float *)mem)[1] = v1;
      ((float *)mem)[2] = v2;
      break;
    case slrak_bvec3:
      ((uint8_t *)mem)[0] = (uint8_t)(v0 ? 0xFF : 0x00);
      ((uint8_t *)mem)[1] = (uint8_t)(v1 ? 0xFF : 0x00);
      ((uint8_t *)mem)[2] = (uint8_t)(v2 ? 0xFF : 0x00);
      break;
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform3fv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  if (count < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  size_t final_array_size = 0, entry_in_final_array = 0;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, &final_array_size, &entry_in_final_array);
  size_t entries_remaining = final_array_size - entry_in_final_array;
  size_t num_entries = (size_t)count;
  size_t n;
  if (entries_remaining < (size_t)count) {
    /* This would overflow the array, or this is not an array; if not an array, report error,
     * otherwise limit the range of what we'll copy. */
    if (entries_remaining > 1) {
      /* This is an array, copy what we can */
      num_entries = entries_remaining;
    }
    else {
      /* This is not an array, error out */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_vec3: {
      for (n = 0; n < num_entries; ++n) {
        ((float *)mem)[n * 3 + 0] = value[n * 3 + 0];
        ((float *)mem)[n * 3 + 1] = value[n * 3 + 1];
        ((float *)mem)[n * 3 + 2] = value[n * 3 + 2];
      }
      break;
    }
    case slrak_bvec3: {
      for (n = 0; n < num_entries; ++n) {
        ((uint8_t *)mem)[n * 3 + 0] = (uint8_t)(value[n * 3 + 0] ? 0xFF : 0x00);
        ((uint8_t *)mem)[n * 3 + 1] = (uint8_t)(value[n * 3 + 1] ? 0xFF : 0x00);
        ((uint8_t *)mem)[n * 3 + 2] = (uint8_t)(value[n * 3 + 2] ? 0xFF : 0x00);
      }
      break;
    }
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform3i)(gl_es2_int location, gl_es2_int v0, gl_es2_int v1, gl_es2_int v2) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, NULL, NULL);
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_ivec3: 
      ((int64_t *)mem)[0] = v0;
      ((int64_t *)mem)[1] = v1;
      ((int64_t *)mem)[2] = v2;
      break;
    case slrak_bvec3: 
      ((uint8_t *)mem)[0] = (uint8_t)(v0 ? 0xFF : 0x00);
      ((uint8_t *)mem)[1] = (uint8_t)(v1 ? 0xFF : 0x00);
      ((uint8_t *)mem)[2] = (uint8_t)(v2 ? 0xFF : 0x00);
      break;
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform3iv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  if (count < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  size_t final_array_size = 0, entry_in_final_array = 0;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, &final_array_size, &entry_in_final_array);
  size_t entries_remaining = final_array_size - entry_in_final_array;
  size_t num_entries = (size_t)count;
  size_t n;
  if (entries_remaining < (size_t)count) {
    /* This would overflow the array, or this is not an array; if not an array, report error,
     * otherwise limit the range of what we'll copy. */
    if (entries_remaining > 1) {
      /* This is an array, copy what we can */
      num_entries = entries_remaining;
    }
    else {
      /* This is not an array, error out */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_ivec3: {
      for (n = 0; n < num_entries; ++n) {
        ((int64_t *)mem)[n * 3 + 0] = (int64_t)value[n * 3 + 0];
        ((int64_t *)mem)[n * 3 + 1] = (int64_t)value[n * 3 + 1];
        ((int64_t *)mem)[n * 3 + 2] = (int64_t)value[n * 3 + 2];
      }
      break;
    }
    case slrak_bvec3: {
      for (n = 0; n < num_entries; ++n) {
        ((uint8_t *)mem)[n * 3 + 0] = (uint8_t)(value[n * 3 + 0] ? 0xFF : 0x00);
        ((uint8_t *)mem)[n * 3 + 1] = (uint8_t)(value[n * 3 + 1] ? 0xFF : 0x00);
        ((uint8_t *)mem)[n * 3 + 2] = (uint8_t)(value[n * 3 + 2] ? 0xFF : 0x00);
      }
      break;
    }
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform4f)(gl_es2_int location, gl_es2_float v0, gl_es2_float v1, gl_es2_float v2, gl_es2_float v3) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, NULL, NULL);
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_vec4:
      ((float *)mem)[0] = v0;
      ((float *)mem)[1] = v1;
      ((float *)mem)[2] = v2;
      ((float *)mem)[3] = v3;
      break;
    case slrak_bvec4:
      ((uint8_t *)mem)[0] = (uint8_t)(v0 ? 0xFF : 0x00);
      ((uint8_t *)mem)[1] = (uint8_t)(v1 ? 0xFF : 0x00);
      ((uint8_t *)mem)[2] = (uint8_t)(v2 ? 0xFF : 0x00);
      ((uint8_t *)mem)[3] = (uint8_t)(v3 ? 0xFF : 0x00);
      break;
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform4fv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_float *value) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  if (count < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  size_t final_array_size = 0, entry_in_final_array = 0;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, &final_array_size, &entry_in_final_array);
  size_t entries_remaining = final_array_size - entry_in_final_array;
  size_t num_entries = (size_t)count;
  size_t n;
  if (entries_remaining < (size_t)count) {
    /* This would overflow the array, or this is not an array; if not an array, report error,
     * otherwise limit the range of what we'll copy. */
    if (entries_remaining > 1) {
      /* This is an array, copy what we can */
      num_entries = entries_remaining;
    }
    else {
      /* This is not an array, error out */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_vec4: {
      for (n = 0; n < num_entries; ++n) {
        ((float *)mem)[n * 4 + 0] = value[n * 4 + 0];
        ((float *)mem)[n * 4 + 1] = value[n * 4 + 1];
        ((float *)mem)[n * 4 + 2] = value[n * 4 + 2];
        ((float *)mem)[n * 4 + 3] = value[n * 4 + 3];
      }
      break;
    }
    case slrak_bvec4: {
      for (n = 0; n < num_entries; ++n) {
        ((uint8_t *)mem)[n * 4 + 0] = (uint8_t)(value[n * 4 + 0] ? 0xFF : 0x00);
        ((uint8_t *)mem)[n * 4 + 1] = (uint8_t)(value[n * 4 + 1] ? 0xFF : 0x00);
        ((uint8_t *)mem)[n * 4 + 2] = (uint8_t)(value[n * 4 + 2] ? 0xFF : 0x00);
        ((uint8_t *)mem)[n * 4 + 3] = (uint8_t)(value[n * 4 + 3] ? 0xFF : 0x00);
      }
      break;
    }
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform4i)(gl_es2_int location, gl_es2_int v0, gl_es2_int v1, gl_es2_int v2, gl_es2_int v3) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, NULL, NULL);
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_ivec4: 
      ((int64_t *)mem)[0] = v0;
      ((int64_t *)mem)[1] = v1;
      ((int64_t *)mem)[2] = v2;
      ((int64_t *)mem)[3] = v3;
      break;
    case slrak_bvec4: 
      ((uint8_t *)mem)[0] = (uint8_t)(v0 ? 0xFF : 0x00);
      ((uint8_t *)mem)[1] = (uint8_t)(v1 ? 0xFF : 0x00);
      ((uint8_t *)mem)[2] = (uint8_t)(v2 ? 0xFF : 0x00);
      ((uint8_t *)mem)[3] = (uint8_t)(v3 ? 0xFF : 0x00);
      break;
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Uniform4iv)(gl_es2_int location, gl_es2_sizei count, const gl_es2_int *value) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  if (count < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  size_t final_array_size = 0, entry_in_final_array = 0;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, &final_array_size, &entry_in_final_array);
  size_t entries_remaining = final_array_size - entry_in_final_array;
  size_t num_entries = (size_t)count;
  size_t n;
  if (entries_remaining < (size_t)count) {
    /* This would overflow the array, or this is not an array; if not an array, report error,
     * otherwise limit the range of what we'll copy. */
    if (entries_remaining > 1) {
      /* This is an array, copy what we can */
      num_entries = entries_remaining;
    }
    else {
      /* This is not an array, error out */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_ivec4: {
      for (n = 0; n < num_entries; ++n) {
        ((int64_t *)mem)[n * 4 + 0] = (int64_t)value[n * 4 + 0];
        ((int64_t *)mem)[n * 4 + 1] = (int64_t)value[n * 4 + 1];
        ((int64_t *)mem)[n * 4 + 2] = (int64_t)value[n * 4 + 2];
        ((int64_t *)mem)[n * 4 + 3] = (int64_t)value[n * 4 + 3];
      }
      break;
    }
    case slrak_bvec4: {
      for (n = 0; n < num_entries; ++n) {
        ((uint8_t *)mem)[n * 4 + 0] = (uint8_t)(value[n * 4 + 0] ? 0xFF : 0x00);
        ((uint8_t *)mem)[n * 4 + 1] = (uint8_t)(value[n * 4 + 1] ? 0xFF : 0x00);
        ((uint8_t *)mem)[n * 4 + 2] = (uint8_t)(value[n * 4 + 2] ? 0xFF : 0x00);
        ((uint8_t *)mem)[n * 4 + 3] = (uint8_t)(value[n * 4 + 3] ? 0xFF : 0x00);
      }
      break;
    }
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(UniformMatrix2fv)(gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  if (count < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (transpose) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  size_t final_array_size = 0, entry_in_final_array = 0;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, &final_array_size, &entry_in_final_array);
  size_t entries_remaining = final_array_size - entry_in_final_array;
  size_t num_entries = (size_t)count;
  size_t n;
  if (entries_remaining < (size_t)count) {
    /* This would overflow the array, or this is not an array; if not an array, report error,
     * otherwise limit the range of what we'll copy. */
    if (entries_remaining > 1) {
      /* This is an array, copy what we can */
      num_entries = entries_remaining;
    }
    else {
      /* This is not an array, error out */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_mat2: {
      for (n = 0; n < num_entries; ++n) {
        ((float *)mem)[n * 4 + 0] = value[n * 4 + 0];
        ((float *)mem)[n * 4 + 1] = value[n * 4 + 1];
        ((float *)mem)[n * 4 + 2] = value[n * 4 + 2];
        ((float *)mem)[n * 4 + 3] = value[n * 4 + 3];
      }
      break;
    }
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(UniformMatrix3fv)(gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  if (count < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (transpose) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  size_t final_array_size = 0, entry_in_final_array = 0;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, &final_array_size, &entry_in_final_array);
  size_t entries_remaining = final_array_size - entry_in_final_array;
  size_t num_entries = (size_t)count;
  size_t n;
  if (entries_remaining < (size_t)count) {
    /* This would overflow the array, or this is not an array; if not an array, report error,
     * otherwise limit the range of what we'll copy. */
    if (entries_remaining > 1) {
      /* This is an array, copy what we can */
      num_entries = entries_remaining;
    }
    else {
      /* This is not an array, error out */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_mat3: {
      for (n = 0; n < num_entries; ++n) {
        ((float *)mem)[n * 9 + 0] = value[n * 9 + 0];
        ((float *)mem)[n * 9 + 1] = value[n * 9 + 1];
        ((float *)mem)[n * 9 + 2] = value[n * 9 + 2];
        ((float *)mem)[n * 9 + 3] = value[n * 9 + 3];
        ((float *)mem)[n * 9 + 4] = value[n * 9 + 4];
        ((float *)mem)[n * 9 + 5] = value[n * 9 + 5];
        ((float *)mem)[n * 9 + 6] = value[n * 9 + 6];
        ((float *)mem)[n * 9 + 7] = value[n * 9 + 7];
        ((float *)mem)[n * 9 + 8] = value[n * 9 + 8];
      }
      break;
    }
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(UniformMatrix4fv)(gl_es2_int location, gl_es2_sizei count, gl_es2_boolean transpose, const gl_es2_float *value) {
  struct gl_es2_context *c = gl_es2_ctx();
  struct gl_es2_program *prog = c->current_program_;
  if (!prog) {
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!prog->program_.gl_last_link_status_) {
    /* program has not been successfully linked */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (location == -1) {
    /* silently ignore */
    return;
  }
  if (count < 0) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  if (transpose) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  int r;
  void *mem = NULL;
  sl_reg_alloc_kind_t slrak = slrak_void;
  size_t final_array_size = 0, entry_in_final_array = 0;
  r = sl_uniform_get_location_info(&prog->program_.uniforms_, location, &mem, &slrak, NULL, NULL, &final_array_size, &entry_in_final_array);
  size_t entries_remaining = final_array_size - entry_in_final_array;
  size_t num_entries = (size_t)count;
  size_t n;
  if (entries_remaining < (size_t)count) {
    /* This would overflow the array, or this is not an array; if not an array, report error,
     * otherwise limit the range of what we'll copy. */
    if (entries_remaining > 1) {
      /* This is an array, copy what we can */
      num_entries = entries_remaining;
    }
    else {
      /* This is not an array, error out */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
  }
  if (r != SL_ERR_OK) {
    if (r == SL_ERR_NO_MEM) {
      set_gl_err(GL_ES2_OUT_OF_MEMORY);
      return;
    }
    else if (r == SL_ERR_INVALID_ARG) {
      /* invalid location */
      set_gl_err(GL_ES2_INVALID_OPERATION);
      return;
    }
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }
  if (!mem) {
    /* Never got the mem ?? */
    set_gl_err(GL_ES2_INVALID_OPERATION);
    return;
  }

  switch (slrak) {
    case slrak_mat3: {
      for (n = 0; n < num_entries; ++n) {
        ((float *)mem)[n * 16 + 0] = value[n * 16 + 0];
        ((float *)mem)[n * 16 + 1] = value[n * 16 + 1];
        ((float *)mem)[n * 16 + 2] = value[n * 16 + 2];
        ((float *)mem)[n * 16 + 3] = value[n * 16 + 3];
        ((float *)mem)[n * 16 + 4] = value[n * 16 + 4];
        ((float *)mem)[n * 16 + 5] = value[n * 16 + 5];
        ((float *)mem)[n * 16 + 6] = value[n * 16 + 6];
        ((float *)mem)[n * 16 + 7] = value[n * 16 + 7];
        ((float *)mem)[n * 16 + 8] = value[n * 16 + 8];
        ((float *)mem)[n * 16 + 9] = value[n * 16 + 9];
        ((float *)mem)[n * 16 + 10] = value[n * 16 + 10];
        ((float *)mem)[n * 16 + 11] = value[n * 16 + 11];
        ((float *)mem)[n * 16 + 12] = value[n * 16 + 12];
        ((float *)mem)[n * 16 + 13] = value[n * 16 + 13];
        ((float *)mem)[n * 16 + 14] = value[n * 16 + 14];
        ((float *)mem)[n * 16 + 15] = value[n * 16 + 15];
      }
      break;
    }
    default:
      set_gl_err(GL_ES2_INVALID_OPERATION);
      break;
  }
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
  /* XXX: Don't forget GL_ES2_VALIDATE_STATUS on glGetProgramiv() */
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib1f)(gl_es2_uint index, gl_es2_float x) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (index > c->attribs_.num_attribs_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  c->attribs_.attribs_[index].generic_values_[0] = x;
  c->attribs_.attribs_[index].generic_values_[1] = 0.f;
  c->attribs_.attribs_[index].generic_values_[2] = 0.f;
  c->attribs_.attribs_[index].generic_values_[3] = 1.f;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib1fv)(gl_es2_uint index, const gl_es2_float *v) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (index > c->attribs_.num_attribs_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  c->attribs_.attribs_[index].generic_values_[0] = v[0];
  c->attribs_.attribs_[index].generic_values_[1] = 0.f;
  c->attribs_.attribs_[index].generic_values_[2] = 0.f;
  c->attribs_.attribs_[index].generic_values_[3] = 1.f;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib2f)(gl_es2_uint index, gl_es2_float x, gl_es2_float y) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (index > c->attribs_.num_attribs_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  c->attribs_.attribs_[index].generic_values_[0] = x;
  c->attribs_.attribs_[index].generic_values_[1] = y;
  c->attribs_.attribs_[index].generic_values_[2] = 0.f;
  c->attribs_.attribs_[index].generic_values_[3] = 1.f;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib2fv)(gl_es2_uint index, const gl_es2_float *v) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (index > c->attribs_.num_attribs_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  c->attribs_.attribs_[index].generic_values_[0] = v[0];
  c->attribs_.attribs_[index].generic_values_[1] = v[1];
  c->attribs_.attribs_[index].generic_values_[2] = 0.f;
  c->attribs_.attribs_[index].generic_values_[3] = 1.f;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib3f)(gl_es2_uint index, gl_es2_float x, gl_es2_float y, gl_es2_float z) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (index > c->attribs_.num_attribs_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  c->attribs_.attribs_[index].generic_values_[0] = x;
  c->attribs_.attribs_[index].generic_values_[1] = y;
  c->attribs_.attribs_[index].generic_values_[2] = z;
  c->attribs_.attribs_[index].generic_values_[3] = 1.f;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib3fv)(gl_es2_uint index, const gl_es2_float *v) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (index > c->attribs_.num_attribs_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  c->attribs_.attribs_[index].generic_values_[0] = v[0];
  c->attribs_.attribs_[index].generic_values_[1] = v[1];
  c->attribs_.attribs_[index].generic_values_[2] = v[2];
  c->attribs_.attribs_[index].generic_values_[3] = 1.f;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib4f)(gl_es2_uint index, gl_es2_float x, gl_es2_float y, gl_es2_float z, gl_es2_float w) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (index > c->attribs_.num_attribs_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  c->attribs_.attribs_[index].generic_values_[0] = x;
  c->attribs_.attribs_[index].generic_values_[1] = y;
  c->attribs_.attribs_[index].generic_values_[2] = z;
  c->attribs_.attribs_[index].generic_values_[3] = w;
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttrib4fv)(gl_es2_uint index, const gl_es2_float *v) {
  struct gl_es2_context *c = gl_es2_ctx();
  if (index > c->attribs_.num_attribs_) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  c->attribs_.attribs_[index].generic_values_[0] = v[0];
  c->attribs_.attribs_[index].generic_values_[1] = v[1];
  c->attribs_.attribs_[index].generic_values_[2] = v[2];
  c->attribs_.attribs_[index].generic_values_[3] = v[3];
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(VertexAttribPointer)(gl_es2_uint index, gl_es2_int size, gl_es2_enum type, gl_es2_boolean normalized, gl_es2_sizei stride, const void *pointer) {
}

GL_ES2_DECL_SPEC void GL_ES2_DECLARATOR_ATTRIB GL_ES2_FUNCTION_ID(Viewport)(gl_es2_int x, gl_es2_int y, gl_es2_sizei width, gl_es2_sizei height) {
  struct gl_es2_context *c = gl_es2_ctx();
  
  if ((width < 0) || (height < 0)) {
    set_gl_err(GL_ES2_INVALID_VALUE);
    return;
  }
  
  if (x < INT_MIN) x = INT_MIN;
  if (x > INT_MAX) x = INT_MAX;
  if (y < INT_MIN) y = INT_MIN;
  if (y > INT_MAX) y = INT_MAX;

  if (width > GL_ES2_IMPL_MAX_VIEWPORT_DIMS) {
    width = GL_ES2_IMPL_MAX_VIEWPORT_DIMS;
  }
  if (height > GL_ES2_IMPL_MAX_VIEWPORT_DIMS) {
    height = GL_ES2_IMPL_MAX_VIEWPORT_DIMS;
  }
  
  c->vp_x_ = (int32_t)x;
  c->vp_y_ = (int32_t)y;
  c->vp_width_ = (int32_t)width;
  c->vp_height_ = (int32_t)height;
}

