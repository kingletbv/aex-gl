/* Copyright 2024 Kinglet B.V.
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
#ifndef GL_ES2_CONTEXT_H
#define GL_ES2_CONTEXT_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef REF_RANGE_ALLOCATOR_H_INCLUDED
#define REF_RANGE_ALLOCATOR_H_INCLUDED
#include "ref_range_allocator.h"
#endif

#ifndef NAMED_OBJECT_TABLE_H_INCLUDED
#define NAMED_OBJECT_TABLE_H_INCLUDED
#include "named_object_table.h"
#endif

#ifndef GL_ES2_IMPL_TYPES_H_INCLUDED
#define GL_ES2_IMPL_TYPES_H_INCLUDED
#include "gl_es2_impl_types.h"
#endif

#ifndef SL_PROGRAM_H_INCLUDED
#define SL_PROGRAM_H_INCLUDED
#include "sl_program.h"
#endif

/* glGet(GL_MAX_TEXTURE_IMAGE_UNITS)
 * glGet(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS)
 */
#define GL_ES2_MAX_NUM_TEXTURE_UNITS 64

/* glGet(GL_MAX_VERTEX_ATTRIBS) */
#define GL_ES2_MAX_NUM_VERTEX_ATTRIBS 1024


#ifdef __cplusplus
extern "C" {
#endif

enum gl_es2_framebuffer_attachment_object_type {
  gl_es2_faot_none,         /* GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE of GL_NONE */
  gl_es2_faot_renderbuffer, /* GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE of GL_RENDERBUFFER */
  gl_es2_faot_texture       /* GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE of GL_TEXTURE */
};

enum gl_es2_cube_map_face {
  gl_es2_cube_map_positive_x,
  gl_es2_cube_map_negative_x,
  gl_es2_cube_map_positive_y,
  gl_es2_cube_map_negative_y,
  gl_es2_cube_map_positive_z,
  gl_es2_cube_map_negative_z
};

enum gl_es2_texture_kind {
  gl_es2_texture_invalid,
  gl_es2_texture_2d,
  gl_es2_texture_cube_map
};

struct gl_es2_framebuffer_attachment {
  enum gl_es2_framebuffer_attachment_object_type kind_;

  /* Pointer to renderbuffer or texture that is attached to the framebuffer */
  union {
    struct gl_es2_renderbuffer *rb_;
    struct gl_es2_texture *tex_;
  } v_;
    
  /* The following fields are for kind_ == gl_es2_faot_texture only */
  gl_es2_int level_;                        /* GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL */
  enum gl_es2_cube_map_face cube_map_face_; /* GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE */

  /* Next and previous attachment for the framebuffer
   * attachments that share this same renderbuffer or 
   * texture. */
  struct gl_es2_framebuffer_attachment *next_, *prev_;
  
  /* Convenience backpointer to the framebuffer of which this gl_es2_framebuffer_attachment is a field */
  struct gl_es2_framebuffer *fb_;
};

struct gl_es2_program_shader_attachment {
  /* Pointer to the attached shader; NULL if no shader is attached */
  struct gl_es2_shader *shader_;

  /* Next and previous attachment for other programs sharing this same shader */
  struct gl_es2_program_shader_attachment *next_, *prev_;

  /* Convenience backpointer to the program of which this gl_es2_program_shader_attachment is a field */
  struct gl_es2_program *program_;
};

struct gl_es2_framebuffer {
  struct named_object no_;

  struct gl_es2_framebuffer_attachment color_attachment0_;
  struct gl_es2_framebuffer_attachment depth_attachment_;
  struct gl_es2_framebuffer_attachment stencil_attachment_;
};

struct gl_es2_renderbuffer {
  struct named_object no_;

  struct gl_es2_framebuffer_attachment *first_framebuffer_attached_to_;
};

struct gl_es2_texture {
  struct named_object no_;

  enum gl_es2_texture_kind kind_;

  struct gl_es2_framebuffer_attachment *first_framebuffer_attached_to_;
};

struct gl_es2_texture_unit {
  struct gl_es2_texture *texture_2d_;       /* glBindTexture() target == GL_TEXTURE_2D */
  struct gl_es2_texture *texture_cube_map_; /* glBindTexture() target == GL_TEXTURE_CUBE_MAP */
};

struct gl_es2_buffer {
  struct named_object no_;
};

struct gl_es2_program {
  struct named_object no_;

  /* glGetProgramiv(GL_DELETE_STATUS) */
  int flagged_for_deletion_:1;

  struct gl_es2_program_shader_attachment vertex_shader_;
  struct gl_es2_program_shader_attachment fragment_shader_;

  struct sl_program program_;
};

struct gl_es2_shader {
  struct named_object no_;

  /* GL_ES2_VERTEX_SHADER or GL_ES2_FRAGMENT_SHADER */
  gl_es2_enum type_;

  struct gl_es2_program_shader_attachment *first_program_attached_to_;

  /* glGetShaderiv(GL_DELETE_STATUS) */
  int flagged_for_deletion_:1;
};

struct gl_es2_context {
  gl_es2_enum current_error_;

  /* If non-zero, then internals become accessible:
   * - glBindAttribLocation on attributes that start with "gl_".
   * - glGetUniformLocation on uniforms that start with "gl_".
   *
   * default is 0.
   */
  int allow_internals_:1;

  struct ref_range_allocator framebuffer_rra_;
  struct named_object_table framebuffer_not_;

  struct ref_range_allocator renderbuffer_rra_;
  struct named_object_table renderbuffer_not_;

  struct ref_range_allocator texture_rra_;
  struct named_object_table texture_not_;

  struct ref_range_allocator buffer_rra_;
  struct named_object_table buffer_not_;

  struct ref_range_allocator program_rra_;
  struct named_object_table program_not_;

  struct ref_range_allocator shader_rra_;
  struct named_object_table shader_not_;

  /* currently bound targets */

  /* glBindFramebuffer(GL_FRAMEBUFFER) */
  struct gl_es2_framebuffer *framebuffer_;

  /* glBindRenderbuffer(GL_RENDERBUFFER) */
  struct gl_es2_renderbuffer *renderbuffer_;

  /* Number of active texture units; currently always GL_ES2_MAX_NUM_TEXTURE_UNITS */
  size_t num_active_texture_units_;

  /* Currently active texture unit, starts out as 0 */
  size_t current_active_texture_unit_;
  struct gl_es2_texture_unit active_texture_units_[GL_ES2_MAX_NUM_TEXTURE_UNITS];

  /* glBindBuffer(GL_ARRAY_BUFFER); NULL means use client memory */
  struct gl_es2_buffer *array_buffer_;

  /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER); NULL means use client memory */
  struct gl_es2_buffer *element_array_buffer_;

  /* glUseProgram() - Currently used program */
  struct gl_es2_program *current_program_;

  /* glBlendEquation & glGet(GL_BLEND_EQUATION_RGB) & glGet(GL_BLEND_EQUATION_ALPHA) */
  blend_eq_t blend_rgb_eq_;
  blend_eq_t blend_alpha_eq_;

  /* glBlendFunc/glBlendFuncSeparate & glGet(GL_BLEND_SRC_RGB) & glGet(GL_BLEND_SRC_ALPHA) & 
   * glGet(GL_BLEND_DST_RGB) & glGet(GL_BLEND_DST_ALPHA) */
  blend_func_t blend_src_rgb_fn_;
  blend_func_t blend_src_alpha_fn_;
  blend_func_t blend_dst_rgb_fn_;
  blend_func_t blend_dst_alpha_fn_;

  /* glBlendColor() & glGet(GL_BLEND_COLOR) */
  uint8_t blend_color_red_;
  uint8_t blend_color_grn_;
  uint8_t blend_color_blu_;
  uint8_t blend_color_alpha_;
};

struct gl_es2_context *gl_es2_ctx(void);

void gl_es2_framebuffer_attachment_init(struct gl_es2_framebuffer *fb, struct gl_es2_framebuffer_attachment *fa);
void gl_es2_framebuffer_attachment_cleanup(struct gl_es2_framebuffer_attachment *fa);
void gl_es2_framebuffer_attachment_detach(struct gl_es2_framebuffer_attachment *fa);
void gl_es2_framebuffer_attachment_attach_texture(struct gl_es2_framebuffer_attachment *fa, struct gl_es2_texture *tex);
void gl_es2_framebuffer_attachment_attach_renderbuffer(struct gl_es2_framebuffer_attachment *fa, struct gl_es2_renderbuffer *rb);

void gl_es2_program_shader_attachment_init(struct gl_es2_program *prog, struct gl_es2_program_shader_attachment *psa);
void gl_es2_program_shader_attachment_cleanup(struct gl_es2_program_shader_attachment *psa);
void gl_es2_program_shader_attachment_detach(struct gl_es2_program_shader_attachment *psa);
void gl_es2_program_shader_attachment_attach(struct gl_es2_program_shader_attachment *psa, struct gl_es2_shader *shad);

void gl_es2_framebuffer_init(struct gl_es2_framebuffer *fb);
void gl_es2_framebuffer_cleanup(struct gl_es2_framebuffer *fb);

void gl_es2_renderbuffer_init(struct gl_es2_renderbuffer *rb);
void gl_es2_renderbuffer_cleanup(struct gl_es2_renderbuffer *rb);

void gl_es2_texture_init(struct gl_es2_texture *tex);
void gl_es2_texture_cleanup(struct gl_es2_texture *tex);

void gl_es2_buffer_init(struct gl_es2_buffer *buf);
void gl_es2_buffer_cleanup(struct gl_es2_buffer *buf);

void gl_es2_program_init(struct gl_es2_program *prog);
void gl_es2_program_cleanup(struct gl_es2_program *prog);

void gl_es2_shader_init(struct gl_es2_shader *shad);
void gl_es2_shader_cleanup(struct gl_es2_shader *shad);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GL_ES2_CONTEXT_H */