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

  struct gl_es2_framebuffer_attachment *first_framebuffer_attached_to_;
};

struct gl_es2_context {
  gl_es2_enum current_error_;

  struct ref_range_allocator framebuffer_rra_;
  struct named_object_table framebuffer_not_;

  struct ref_range_allocator renderbuffer_rra_;
  struct named_object_table renderbuffer_not_;

  struct ref_range_allocator texture_rra_;
  struct named_object_table texture_not_;

  /* currently bound targets */

  /* glBindFramebuffer(GL_FRAMEBUFFER) */
  struct gl_es2_framebuffer *framebuffer_;

  /* glBindRenderbuffer(GL_RENDERBUFFER) */
  struct gl_es2_renderbuffer *renderbuffer_;
};

struct gl_es2_context *gl_es2_ctx(void);

void gl_es2_framebuffer_attachment_init(struct gl_es2_framebuffer *fb, struct gl_es2_framebuffer_attachment *fa);
void gl_es2_framebuffer_attachment_cleanup(struct gl_es2_framebuffer_attachment *fa);
void gl_es2_framebuffer_attachment_detach(struct gl_es2_framebuffer_attachment *fa);
void gl_es2_framebuffer_attachment_attach_texture(struct gl_es2_framebuffer_attachment *fa, struct gl_es2_texture *tex);
void gl_es2_framebuffer_attachment_attach_renderbuffer(struct gl_es2_framebuffer_attachment *fa, struct gl_es2_renderbuffer *rb);


void gl_es2_framebuffer_init(struct gl_es2_framebuffer *fb);
void gl_es2_framebuffer_cleanup(struct gl_es2_framebuffer *fb);

void gl_es2_renderbuffer_init(struct gl_es2_renderbuffer *rb);
void gl_es2_renderbuffer_cleanup(struct gl_es2_renderbuffer *rb);

void gl_es2_texture_init(struct gl_es2_texture *tex);
void gl_es2_texture_cleanup(struct gl_es2_texture *tex);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GL_ES2_CONTEXT_H */
