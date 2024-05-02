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
#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef GL_ES2_IMPL_H_INCLUDED
#define GL_ES2_IMPL_H_INCLUDED
#include "gl_es2_impl.h"
#endif

#ifndef GL_ES2_CONTEXT_H_INCLUDED
#define GL_ES2_CONTEXT_H_INCLUDED
#include "gl_es2_context.h"
#endif

#ifndef SAMPLER_2D_H_INCLUDED
#define SAMPLER_2D_H_INCLUDED
#include "sampler_2d.h"
#endif

#ifndef SL_DEFS_H_INCLUDED
#define SL_DEFS_H_INCLUDED
#include "sl_defs.h"
#endif

/* XXX: Important: no TLS, no locks, no atomics, nary a cli/sti: no thread safety. */

static struct gl_es2_context g_ctx_ = { 0 };
static int                   g_ctx_is_initialized_ = 0;

void gl_es2_framebuffer_attachment_init(struct gl_es2_framebuffer *fb, struct gl_es2_framebuffer_attachment *fa) {
  fa->kind_ = gl_es2_faot_none;
  fa->v_.rb_ = NULL;
  fa->level_ = 0;
  fa->cube_map_face_ = gl_es2_cube_map_positive_x;
  fa->next_ = fa->prev_ = NULL;
  fa->fb_ = fb;
}

void gl_es2_framebuffer_attachment_cleanup(struct gl_es2_framebuffer_attachment *fa) {
  gl_es2_framebuffer_attachment_detach(fa);
}

void gl_es2_framebuffer_attachment_detach(struct gl_es2_framebuffer_attachment *fa) {
  if ((fa->kind_ == gl_es2_faot_renderbuffer) && (fa->v_.rb_)) {
    struct gl_es2_renderbuffer *rb;
    rb = fa->v_.rb_;
    if (fa->next_ == fa) {
      rb->first_framebuffer_attached_to_ = NULL;
    }
    else /* not the last attachment to this renderbuffer */ {
      if (rb->first_framebuffer_attached_to_ == fa) {
        rb->first_framebuffer_attached_to_ = fa->next_;
      }
      fa->next_->prev_ = fa->prev_;
      fa->prev_->next_ = fa->next_;
    }
    fa->kind_ = gl_es2_faot_none;
    fa->v_.rb_ = NULL;
    fa->next_ = fa->prev_ = NULL;
  }
  else if (fa->kind_ == gl_es2_faot_texture) {
    struct gl_es2_texture *tex;
    tex = fa->v_.tex_;
    if (fa->next_ == fa) {
      tex->first_framebuffer_attached_to_ = NULL;
    }
    else /* not the last attachment to this framebuffer */ {
      if (tex->first_framebuffer_attached_to_ == fa) {
        tex->first_framebuffer_attached_to_ = fa->next_;
      }
      fa->next_->prev_ = fa->prev_;
      fa->prev_->next_ = fa->next_;
    }
    fa->kind_ = gl_es2_faot_none;
    fa->v_.tex_ = NULL;
    fa->next_ = fa->prev_ = NULL;
  }
  else /* gl_es2_faot_none -- no attachment */ {
  }
}

void gl_es2_framebuffer_attachment_attach_texture(struct gl_es2_framebuffer_attachment *fa, struct gl_es2_texture *tex) {
  gl_es2_framebuffer_attachment_detach(fa);
  if (!tex) return;
  fa->kind_ = gl_es2_faot_texture;
  fa->v_.tex_ = tex;
  if (tex->first_framebuffer_attached_to_) {
    fa->next_ = tex->first_framebuffer_attached_to_;
    fa->prev_ = fa->next_->prev_;
    fa->prev_->next_ = fa->next_->prev_ = fa;
  }
  else {
    fa->next_ = fa->prev_ = fa;
    tex->first_framebuffer_attached_to_ = fa;
  }
  fa->level_ = 0;
  fa->cube_map_face_ = gl_es2_cube_map_positive_x; /* caller should override; this just sets the default */
}

void gl_es2_framebuffer_attachment_attach_renderbuffer(struct gl_es2_framebuffer_attachment *fa, struct gl_es2_renderbuffer *rb) {
  gl_es2_framebuffer_attachment_detach(fa);
  if (!rb) return;
  fa->kind_ = gl_es2_faot_renderbuffer;
  fa->v_.rb_ = rb;
  if (rb->first_framebuffer_attached_to_) {
    fa->next_ = rb->first_framebuffer_attached_to_;
    fa->prev_ = fa->next_->prev_;
    fa->prev_->next_ = fa->next_->prev_ = fa;
  }
  else {
    fa->next_ = fa->prev_ = fa;
    rb->first_framebuffer_attached_to_ = fa;
  }
  fa->level_ = 0;
  fa->cube_map_face_ = gl_es2_cube_map_positive_x;
}

void gl_es2_framebuffer_attachment_raw_ptr(struct gl_es2_framebuffer_attachment *fa, void **prawptr, size_t *pstride) {
  switch (fa->kind_) {
    case gl_es2_faot_none:
      *prawptr = NULL;
      *pstride = 0;
      break;
    case gl_es2_faot_renderbuffer:
      *prawptr = fa->v_.rb_->bitmap_;
      *pstride = fa->v_.rb_->num_bytes_per_bitmap_row_;
      break;
    case gl_es2_faot_texture: {
      struct sampler_2d *s2d = gl_es2_framebuffer_attachment_get_texture_sampler_2d(fa);
      if (s2d->num_maps_) {
        *prawptr = s2d->mipmaps_[0].bitmap_;
        *pstride = s2d->mipmaps_[0].num_bytes_per_bitmap_row_;
      }
      else {
        *prawptr = NULL;
        *pstride = 0;
      }
      break;
    }
    default:
      *prawptr = NULL;
      *pstride = 0;
      break;
  }
}

struct sampler_2d *gl_es2_framebuffer_attachment_get_texture_sampler_2d(struct gl_es2_framebuffer_attachment *fa) {
  if (fa->kind_ != gl_es2_faot_texture) return NULL;
  struct gl_es2_texture *tex = fa->v_.tex_;
  if (tex->kind_ == gl_es2_texture_2d) {
    return &tex->texture_2d_;
  }
  if (tex->kind_ != gl_es2_texture_cube_map) {
    return NULL;
  }
  return gl_es2_texture_get_sampler_2d_for_cube_map_face(tex, fa->cube_map_face_);
}

int gl_es2_framebuffer_get_dims(struct gl_es2_framebuffer *fb, int *pwidth, int *pheight) {
  int width, height;
  if (fb->color_attachment0_.kind_ == gl_es2_faot_renderbuffer) {
    width = fb->color_attachment0_.v_.rb_->width_;
    height = fb->color_attachment0_.v_.rb_->height_;
  }
  else if (fb->color_attachment0_.kind_ == gl_es2_faot_texture) {
    if (fb->color_attachment0_.v_.tex_->texture_2d_.num_maps_) {
      width = fb->color_attachment0_.v_.tex_->texture_2d_.mipmaps_[0].width_;
      height = fb->color_attachment0_.v_.tex_->texture_2d_.mipmaps_[0].height_;
    }
    else {
      width = height = 0;
    }
  }
  else if (fb->depth_attachment_.kind_ == gl_es2_faot_renderbuffer) {
    width = fb->depth_attachment_.v_.rb_->width_;
    height = fb->depth_attachment_.v_.rb_->height_;
  }
  else if (fb->depth_attachment_.kind_ == gl_es2_faot_texture) {
    if (fb->depth_attachment_.v_.tex_->texture_2d_.num_maps_) {
      width = fb->depth_attachment_.v_.tex_->texture_2d_.mipmaps_[0].width_;
      height = fb->depth_attachment_.v_.tex_->texture_2d_.mipmaps_[0].height_;
    }
    else {
      width = height = 0;
    }
  }
  else if (fb->stencil_attachment_.kind_ == gl_es2_faot_renderbuffer) {
    width = fb->stencil_attachment_.v_.rb_->width_;
    height = fb->stencil_attachment_.v_.rb_->height_;
  }
  else if (fb->stencil_attachment_.kind_ == gl_es2_faot_texture) {
    if (fb->stencil_attachment_.v_.tex_->texture_2d_.num_maps_) {
      width = fb->stencil_attachment_.v_.tex_->texture_2d_.mipmaps_[0].width_;
      height = fb->stencil_attachment_.v_.tex_->texture_2d_.mipmaps_[0].height_;
    }
    else {
      width = height = 0;
    }
  }
  else {
    /* No attachments */
    *pwidth = *pheight = 0;
    return 0;
  }
  *pwidth = width;
  *pheight = height;
  return 1;
}

int gl_es2_framebuffer_get_bitmap_row_num(struct gl_es2_framebuffer *fa, int y) {
  int width = 0, height = 0;
  if (!gl_es2_framebuffer_get_dims(fa, &width, &height)) {
    return 0;
  }
  return height - y;
}

enum gl_es2_framebuffer_completeness gl_es2_framebuffer_check_completeness(struct gl_es2_framebuffer *fb) {
  if (!fb) return gl_es2_framebuffer_incomplete_unsupported;
  int width, height;
  if (!gl_es2_framebuffer_get_dims(fb, &width, &height)) {
    /* No attachments */
    return gl_es2_framebuffer_incomplete_missing_attachment;
  }

  int have_zero_dim = 0;
  if (fb->color_attachment0_.kind_ == gl_es2_faot_renderbuffer) {
    have_zero_dim = have_zero_dim || !fb->color_attachment0_.v_.rb_->width_;
    have_zero_dim = have_zero_dim || !fb->color_attachment0_.v_.rb_->height_;
  }
  else if (fb->color_attachment0_.kind_ == gl_es2_faot_texture) {
    if (fb->color_attachment0_.v_.tex_->texture_2d_.num_maps_) {
      have_zero_dim = have_zero_dim || !fb->color_attachment0_.v_.tex_->texture_2d_.mipmaps_[0].width_;
      have_zero_dim = have_zero_dim || !fb->color_attachment0_.v_.tex_->texture_2d_.mipmaps_[0].height_;
    }
    else {
      have_zero_dim = 1;
    }
  }
  if (fb->depth_attachment_.kind_ == gl_es2_faot_renderbuffer) {
    have_zero_dim = have_zero_dim || !fb->depth_attachment_.v_.rb_->width_;
    have_zero_dim = have_zero_dim || !fb->depth_attachment_.v_.rb_->height_;
  }
  else if (fb->depth_attachment_.kind_ == gl_es2_faot_texture) {
    if (fb->depth_attachment_.v_.tex_->texture_2d_.num_maps_) {
      have_zero_dim = have_zero_dim || !fb->depth_attachment_.v_.tex_->texture_2d_.mipmaps_[0].width_;
      have_zero_dim = have_zero_dim || !fb->depth_attachment_.v_.tex_->texture_2d_.mipmaps_[0].height_;
    }
    else {
      have_zero_dim = 1;
    }
  }
  if (fb->stencil_attachment_.kind_ == gl_es2_faot_renderbuffer) {
    have_zero_dim = have_zero_dim || !fb->stencil_attachment_.v_.rb_->width_;
    have_zero_dim = have_zero_dim || !fb->stencil_attachment_.v_.rb_->height_;
  }
  else if (fb->stencil_attachment_.kind_ == gl_es2_faot_texture) {
    if (fb->stencil_attachment_.v_.tex_->texture_2d_.num_maps_) {
      have_zero_dim = have_zero_dim || !fb->stencil_attachment_.v_.tex_->texture_2d_.mipmaps_[0].width_;
      have_zero_dim = have_zero_dim || !fb->stencil_attachment_.v_.tex_->texture_2d_.mipmaps_[0].height_;
    }
    else {
      have_zero_dim = 1;
    }
  }

  if (have_zero_dim) {
    /* One of the attached images has a 0 width or height */
    return gl_es2_framebuffer_incomplete_attachment;
  }

  int not_same_dims = 0;

  if (fb->color_attachment0_.kind_ == gl_es2_faot_renderbuffer) {
    not_same_dims = not_same_dims || (width != fb->color_attachment0_.v_.rb_->width_);
    not_same_dims = not_same_dims || (height != fb->color_attachment0_.v_.rb_->height_);
  }
  else if (fb->color_attachment0_.kind_ == gl_es2_faot_texture) {
    if (fb->color_attachment0_.v_.tex_->texture_2d_.num_maps_) {
      not_same_dims = not_same_dims || (width != fb->color_attachment0_.v_.tex_->texture_2d_.mipmaps_[0].width_);
      not_same_dims = not_same_dims || (height != fb->color_attachment0_.v_.tex_->texture_2d_.mipmaps_[0].height_);
    }
  }
  if (fb->depth_attachment_.kind_ == gl_es2_faot_renderbuffer) {
    not_same_dims = not_same_dims || (width != fb->depth_attachment_.v_.rb_->width_);
    not_same_dims = not_same_dims || (height != fb->depth_attachment_.v_.rb_->height_);
  }
  else if (fb->depth_attachment_.kind_ == gl_es2_faot_texture) {
    if (fb->depth_attachment_.v_.tex_->texture_2d_.num_maps_) {
      not_same_dims = not_same_dims || (width != fb->depth_attachment_.v_.tex_->texture_2d_.mipmaps_[0].width_);
      not_same_dims = not_same_dims || (height != fb->depth_attachment_.v_.tex_->texture_2d_.mipmaps_[0].height_);
    }
  }
  if (fb->stencil_attachment_.kind_ == gl_es2_faot_renderbuffer) {
    not_same_dims = not_same_dims || (width != fb->stencil_attachment_.v_.rb_->width_);
    not_same_dims = not_same_dims || (height != fb->stencil_attachment_.v_.rb_->height_);
  }
  else if (fb->stencil_attachment_.kind_ == gl_es2_faot_texture) {
    if (fb->stencil_attachment_.v_.tex_->texture_2d_.num_maps_) {
      not_same_dims = not_same_dims || (width != fb->stencil_attachment_.v_.tex_->texture_2d_.mipmaps_[0].width_);
      not_same_dims = not_same_dims || (height != fb->stencil_attachment_.v_.tex_->texture_2d_.mipmaps_[0].height_);
    }
  }

  if (not_same_dims) {
    return gl_es2_framebuffer_incomplete_dimensions;
  }

  /* Check the formats are ok */
  int format_ok = 1;
  if (fb->color_attachment0_.kind_ == gl_es2_faot_renderbuffer) {
    struct gl_es2_renderbuffer *rb = fb->color_attachment0_.v_.rb_;
    format_ok = format_ok && (rb->format_ == gl_es2_renderbuffer_format_rgba32);
  }
  else if (fb->color_attachment0_.kind_ == gl_es2_faot_texture) {
    struct gl_es2_texture *tex = fb->color_attachment0_.v_.tex_;
    format_ok = format_ok && ((tex->texture_2d_.components_ == s2d_rgb) ||
                              (tex->texture_2d_.components_ == s2d_rgba));
  }
  if (fb->depth_attachment_.kind_ == gl_es2_faot_renderbuffer) {
    struct gl_es2_renderbuffer *rb = fb->depth_attachment_.v_.rb_;
    format_ok = format_ok && ((rb->format_ == gl_es2_renderbuffer_format_depth16) ||
                              (rb->format_ == gl_es2_renderbuffer_format_depth32));
  }
  else if (fb->depth_attachment_.kind_ == gl_es2_faot_texture) {
    /* currently no textures supported with depth-buffer characteristics */
    format_ok = 0;
  }
  if (fb->stencil_attachment_.kind_ == gl_es2_faot_renderbuffer) {
    struct gl_es2_renderbuffer *rb = fb->stencil_attachment_.v_.rb_;
    format_ok = format_ok && (rb->format_ == gl_es2_renderbuffer_format_stencil16);
  }
  else if (fb->stencil_attachment_.kind_ == gl_es2_faot_texture) {
    /* currently no textures supported with stencil-buffer characteristics */
    format_ok = 0;
  }
  if (!format_ok) return gl_es2_framebuffer_incomplete_attachment;

  return gl_es2_framebuffer_complete;
}

void gl_es2_program_shader_attachment_init(struct gl_es2_program *prog, struct gl_es2_program_shader_attachment *psa) {
  psa->program_ = prog;
  psa->shader_ = NULL;
  psa->next_ = psa->prev_ = NULL;
}

void gl_es2_program_shader_attachment_cleanup(struct gl_es2_program_shader_attachment *psa) {
  gl_es2_program_shader_attachment_detach(psa);
}

void gl_es2_program_shader_attachment_detach(struct gl_es2_program_shader_attachment *psa) {
  struct gl_es2_shader *shad;
  shad = psa->shader_;
  if (shad) {
    if (psa->next_ == psa) {
      shad->first_program_attached_to_ = NULL;
    }
    else {
      if (shad->first_program_attached_to_ == psa) {
        shad->first_program_attached_to_ = psa->next_;
      }
      psa->next_->prev_ = psa->prev_;
      psa->prev_->next_ = psa->next_;
    }
    psa->shader_ = NULL;
    psa->next_ = psa->prev_ = NULL;
  }
}

void gl_es2_program_shader_attachment_attach(struct gl_es2_program_shader_attachment *psa, struct gl_es2_shader *shad) {
  gl_es2_program_shader_attachment_detach(psa);
  if (!shad) return;
  psa->shader_ = shad;
  if (shad->first_program_attached_to_) {
    psa->next_ = shad->first_program_attached_to_;
    psa->prev_ = psa->next_->prev_;
    psa->prev_->next_ = psa->next_->prev_ = psa;
  }
  else {
    psa->next_ = psa->prev_ = psa;
    shad->first_program_attached_to_ = psa;
  }
}


void gl_es2_framebuffer_init(struct gl_es2_framebuffer *fb) {
  gl_es2_framebuffer_attachment_init(fb, &fb->color_attachment0_);
  gl_es2_framebuffer_attachment_init(fb, &fb->depth_attachment_);
  gl_es2_framebuffer_attachment_init(fb, &fb->stencil_attachment_);
}

void gl_es2_framebuffer_cleanup(struct gl_es2_framebuffer *fb) {
  gl_es2_framebuffer_attachment_cleanup(&fb->color_attachment0_);
  gl_es2_framebuffer_attachment_cleanup(&fb->depth_attachment_);
  gl_es2_framebuffer_attachment_cleanup(&fb->stencil_attachment_);
}

void gl_es2_renderbuffer_init(struct gl_es2_renderbuffer *rb) {
  rb->first_framebuffer_attached_to_ = NULL;
  rb->format_ = gl_es2_renderbuffer_format_none;
  rb->width_ = 0;
  rb->height_ = 0;
  rb->bitmap_ = NULL;
  rb->num_bytes_per_bitmap_row_ = 0;
}

void gl_es2_renderbuffer_cleanup(struct gl_es2_renderbuffer *rb) {
  while (rb->first_framebuffer_attached_to_) {
    gl_es2_framebuffer_attachment_detach(rb->first_framebuffer_attached_to_);
  }
}

void gl_es2_texture_init(struct gl_es2_texture *tex) {
  tex->kind_ = gl_es2_texture_invalid;
  tex->first_framebuffer_attached_to_ = NULL;
  sampler_2d_init(&tex->texture_2d_);
  size_t n;
  for (n = 0; n < 6; ++n) {
    sampler_2d_init(&tex->texture_cube_maps_[n]);
  }
}

void gl_es2_texture_cleanup(struct gl_es2_texture *tex) {
  while (tex->first_framebuffer_attached_to_) {
    gl_es2_framebuffer_attachment_detach(tex->first_framebuffer_attached_to_);
  }
  sampler_2d_cleanup(&tex->texture_2d_);
  size_t n;
  for (n = 0; n < 6; ++n) {
    sampler_2d_cleanup(&tex->texture_cube_maps_[n]);
  }
}

struct sampler_2d *gl_es2_texture_get_sampler_2d_for_cube_map_face(struct gl_es2_texture *tex, enum gl_es2_cube_map_face cube_map_face) {
  switch (cube_map_face) {
    case gl_es2_cube_map_positive_x: return &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_x];
    case gl_es2_cube_map_negative_x: return &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_x];
    case gl_es2_cube_map_positive_y: return &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_y];
    case gl_es2_cube_map_negative_y: return &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_y];
    case gl_es2_cube_map_positive_z: return &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_z];
    case gl_es2_cube_map_negative_z: return &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_z];
  }
  return NULL;
}

struct sampler_2d *gl_es2_texture_get_sampler_2d_for_gl_es2_cube_map_face(struct gl_es2_texture *tex, int cube_map_face) {
  switch (cube_map_face) {
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_X: return &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_x];
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_X: return &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_x];
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Y: return &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_y];
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Y: return &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_y];
    case GL_ES2_TEXTURE_CUBE_MAP_POSITIVE_Z: return &tex->texture_cube_maps_[gl_es2_texture_cube_map_positive_z];
    case GL_ES2_TEXTURE_CUBE_MAP_NEGATIVE_Z: return &tex->texture_cube_maps_[gl_es2_texture_cube_map_negative_z];
  }
  return NULL;
}

void gl_es2_buffer_init(struct gl_es2_buffer *buf) {
  data_buffer_init(&buf->buf_);
  buf->usage_ = GL_ES2_STATIC_DRAW;
}

void gl_es2_buffer_cleanup(struct gl_es2_buffer *buf) {
  data_buffer_cleanup(&buf->buf_);
}

void gl_es2_program_init(struct gl_es2_program *prog) {
  prog->flagged_for_deletion_ = 0;
  gl_es2_program_shader_attachment_init(prog, &prog->vertex_shader_);
  gl_es2_program_shader_attachment_init(prog, &prog->fragment_shader_);
  sl_program_init(&prog->program_);
}

void gl_es2_program_cleanup(struct gl_es2_program *prog) {
  gl_es2_program_shader_attachment_cleanup(&prog->vertex_shader_);
  gl_es2_program_shader_attachment_cleanup(&prog->fragment_shader_);
  sl_program_cleanup(&prog->program_);
}

void gl_es2_shader_init(struct gl_es2_shader *shad) {
  shad->flagged_for_deletion_ = 0;
  shad->first_program_attached_to_ = NULL;

  shad->compilation_status_ = 0;

  sl_shader_init(&shad->shader_);
}

void gl_es2_shader_cleanup(struct gl_es2_shader *shad) {
  while (shad->first_program_attached_to_) {
    gl_es2_program_shader_attachment_detach(shad->first_program_attached_to_);
  }

  sl_shader_cleanup(&shad->shader_);
}


void gl_es2_ctx_init(struct gl_es2_context *c) {
  c->current_error_ = GL_ES2_NO_ERROR;

  ref_range_allocator_init(&c->framebuffer_rra_);
  not_init(&c->framebuffer_not_);

  ref_range_allocator_init(&c->renderbuffer_rra_);
  not_init(&c->renderbuffer_not_);

  ref_range_allocator_init(&c->texture_rra_);
  not_init(&c->texture_not_);

  ref_range_allocator_init(&c->buffer_rra_);
  not_init(&c->buffer_not_);

  ref_range_allocator_init(&c->program_rra_);
  not_init(&c->program_not_);

  ref_range_allocator_init(&c->shader_rra_);
  not_init(&c->shader_not_);

  attrib_set_init(&c->attribs_);

  rasterizer_init(&c->ras_);

  c->framebuffer_ = NULL;
  c->renderbuffer_ = NULL;

  c->num_active_texture_units_ = GL_ES2_IMPL_MAX_NUM_TEXTURE_UNITS;
  c->current_active_texture_unit_ = 0;
  size_t n;
  for (n = 0; n < c->num_active_texture_units_; ++n) {
    c->active_texture_units_[n].texture_2d_ = NULL;
    c->active_texture_units_[n].texture_cube_map_ = NULL;
  }
  for (n = 0; n < c->num_active_texture_units_; ++n) {
    c->sampler_2D_uniform_loading_table_[n] = NULL;
    c->sampler_Cube_uniform_loading_table_[n] = NULL;
  }

  c->array_buffer_ = NULL;
  c->element_array_buffer_ = NULL;
  c->current_program_ = NULL;

  c->is_depth_test_enabled_ = 0;
  c->zbuf_func_ = PAZF_LESS;

  c->is_culling_enabled_ = 0;
  c->cull_face_mode_ = gl_es2_cull_face_back;
  c->front_face_ = gl_es2_front_face_counterclockwise;

  c->near_plane_ = 0.f;
  c->far_plane_ = 1.f;

  c->vp_x_ = c->vp_y_ = 0;
  c->vp_width_ = c->vp_height_ = 0;

  c->is_scissor_test_enabled_ = 0; /* default off */

  /* should initialize to window-supplied-framebuffer as soon as it's attached */
  c->scissor_left_ = 0;
  c->scissor_bottom_counted_from_bottom_ = 0;
  c->scissor_width_ = 0;
  c->scissor_height_ = 0;

  c->stencil_writemask_ = ~(uint32_t)0;
  c->stencil_back_writemask_ = ~(uint32_t)0;
  c->stencil_func_ = PASF_ALWAYS;
  c->stencil_func_ref_ = 0;
  c->stencil_func_mask_ = ~(uint32_t)0;
  c->stencil_back_func_ = PASF_ALWAYS;
  c->stencil_back_func_ref_ = 0;
  c->stencil_back_func_mask_ = ~(uint32_t)0;
  c->stencil_sfail_ = PASO_KEEP;
  c->stencil_zfail_ = PASO_KEEP;
  c->stencil_zpass_ = PASO_KEEP;
  c->stencil_back_sfail_ = PASO_KEEP;
  c->stencil_back_zfail_ = PASO_KEEP;
  c->stencil_back_zpass_ = PASO_KEEP;

  c->depth_mask_ = 1;
  c->red_mask_ = c->green_mask_ = c->blue_mask_ = c->alpha_mask_ = 1;

  c->clear_color_red_ = c->clear_color_grn_ = c->clear_color_blu_ = c->clear_color_alpha_ = 0;
  c->clear_depth_ = 1.f;
  c->clear_stencil_ = 0;

  c->is_blend_enabled_ = 0;

  c->blend_rgb_eq_ = BEQ_FUNC_ADD;
  c->blend_alpha_eq_ = BEQ_FUNC_ADD;

  c->blend_src_rgb_fn_ = BF_ONE;
  c->blend_src_alpha_fn_ = BF_ONE;
  c->blend_dst_rgb_fn_ = BF_ZERO;
  c->blend_dst_alpha_fn_ = BF_ZERO;

  c->blend_color_red_ = c->blend_color_grn_ = c->blend_color_blu_ = c->blend_color_alpha_ = 0;

  c->polygon_offset_factor_ = 0.f;
  c->polygon_offset_units_ = 0.f;

  c->line_width_ = 1.f;

  c->is_stencil_test_enabled_ = 0;
  c->is_polygon_offset_fill_enabled_ = 0;

  c->is_dither_enabled_ = 0;
  c->is_sample_alpha_to_coverage_enabled_ = 0;
  c->is_sample_coverage_enabled_ = 0;
}

void gl_es2_ctx_cleanup(struct gl_es2_context *c) {
  ref_range_allocator_cleanup(&c->framebuffer_rra_);
  while (c->framebuffer_not_.seq_) {
    struct gl_es2_framebuffer *fb = (struct gl_es2_framebuffer *)c->framebuffer_not_.seq_;
    not_remove(&c->framebuffer_not_, &fb->no_);
    gl_es2_framebuffer_cleanup(fb);
    free(fb);
  }

  ref_range_allocator_cleanup(&c->renderbuffer_rra_);
  while (c->renderbuffer_not_.seq_) {
    struct gl_es2_renderbuffer *rb = (struct gl_es2_renderbuffer *)c->renderbuffer_not_.seq_;
    not_remove(&c->renderbuffer_not_, &rb->no_);
    gl_es2_renderbuffer_cleanup(rb);
    free(rb);
  }

  ref_range_allocator_cleanup(&c->texture_rra_);
  while (c->texture_not_.seq_) {
    struct gl_es2_texture *tex = (struct gl_es2_texture *)c->texture_not_.seq_;
    not_remove(&c->texture_not_, &tex->no_);
    gl_es2_texture_cleanup(tex);
    free(tex);
  }

  ref_range_allocator_cleanup(&c->buffer_rra_);
  while (c->buffer_not_.seq_) {
    struct gl_es2_buffer *buf = (struct gl_es2_buffer *)c->buffer_not_.seq_;
    not_remove(&c->buffer_not_, &buf->no_);
    gl_es2_buffer_cleanup(buf);
    free(buf);
  }

  ref_range_allocator_cleanup(&c->program_rra_);
  while (c->program_not_.seq_) {
    struct gl_es2_program *prog = (struct gl_es2_program *)c->program_not_.seq_;
    not_remove(&c->program_not_, &prog->no_);
    gl_es2_program_cleanup(prog);
    free(prog);
  }

  ref_range_allocator_cleanup(&c->shader_rra_);
  while (c->shader_not_.seq_) {
    struct gl_es2_shader *shad = (struct gl_es2_shader *)c->program_not_.seq_;
    not_remove(&c->shader_not_, &shad->no_);
    gl_es2_shader_cleanup(shad);
    free(shad);
  }

  attrib_set_cleanup(&c->attribs_);

  rasterizer_cleanup(&c->ras_);
}

static int gl_es2_ctx_complete_initialization(struct gl_es2_context *c) {
  if (attrib_alloc_fixed_num_attribs(&c->attribs_, GL_ES2_IMPL_MAX_NUM_VERTEX_ATTRIBS)) {
    c->current_error_ = GL_ES2_OUT_OF_MEMORY;
    return SL_ERR_NO_MEM;
  }
  return SL_ERR_OK;
}

void gl_es2_ctx_get_normalized_scissor_rect(struct gl_es2_context *c, uint32_t *left, uint32_t *top, uint32_t *right, uint32_t *bottom) {
  /* Normalize scissor rect from "bottom-left positive-y is up" coordinate system to
   * "top-left positive-y is down" coordinate system that rasterizer expects.
   * Clamp to the actual screen while doing so. */
  int screen_width, screen_height;
  if ((!c->framebuffer_) ||
      !gl_es2_framebuffer_get_dims(c->framebuffer_, &screen_width, &screen_height)) {
    *left = *top = *right = *bottom = 0;
    return;
  }

  if (!c->is_scissor_test_enabled_) {
    *left = 0;
    *top = 0;
    *right = screen_width;
    *bottom = screen_height;
    return;
  }

  uint32_t norm_scissor_left, norm_scissor_top, norm_scissor_right, norm_scissor_bottom;
  norm_scissor_left = (c->scissor_left_ >= 0) ? (uint32_t)c->scissor_left_: 0;
  if (c->scissor_bottom_counted_from_bottom_ <= (int32_t)screen_height) {
    norm_scissor_bottom = (c->scissor_bottom_counted_from_bottom_  >= 0) ? (screen_height - c->scissor_bottom_counted_from_bottom_) : screen_height;
  }
  else {
    norm_scissor_bottom = 0;
  }
  if ((c->scissor_bottom_counted_from_bottom_ + c->scissor_height_) <= (int32_t)screen_height) {
    norm_scissor_top = ((c->scissor_bottom_counted_from_bottom_ + c->scissor_height_) >= 0) ? (screen_height - (c->scissor_bottom_counted_from_bottom_ + c->scissor_height_)) : screen_height;
  }
  else {
    norm_scissor_top = 0;
  }
  norm_scissor_right = ((c->scissor_left_ + c->scissor_width_) >= 0) ? (uint32_t)(c->scissor_left_ + c->scissor_width_) : 0;

  *left = norm_scissor_left;
  *top = norm_scissor_top;
  *right = norm_scissor_right;
  *bottom = norm_scissor_bottom;
}

static void gl_es2_at_exit_cleanup(void) {
  if (g_ctx_is_initialized_) {
    gl_es2_ctx_cleanup(&g_ctx_);
  }
}

struct gl_es2_context *gl_es2_ctx(void) {
  if (!g_ctx_is_initialized_) {
    /* Fixed initialization to known-good state */
    gl_es2_ctx_init(&g_ctx_);

    /* Complete initialization with may-fail allocations */
    if (SL_ERR_OK != gl_es2_ctx_complete_initialization(&g_ctx_)) {
      /* Failed allocations -- error will have been
       * set. We will allow this to continue to fail as things
       * will implode if we return NULL. */
    }
    g_ctx_is_initialized_ = 1;
    atexit(gl_es2_at_exit_cleanup);
  }

  return &g_ctx_;
}

int gl_es2_initialize_context(void) {
  if (!g_ctx_is_initialized_) {
    /* Fixed initialization to known-good state */
    gl_es2_ctx_init(&g_ctx_);

    /* Complete initialization with may-fail allocations */
    if (!gl_es2_ctx_complete_initialization(&g_ctx_)) {
      /* Failed allocations.
       * NOTE: Unlike the above, here we cleanup and return 
       *       the -1; reason is that we can now expect the
       *       caller to anticipate and handle any failure,
       *       unlike gl_es2_ctx() where callers may assume
       *       success. */
      gl_es2_ctx_cleanup(&g_ctx_);
      return SL_ERR_NO_MEM;
    }
    g_ctx_is_initialized_ = 1;
    atexit(gl_es2_at_exit_cleanup);
  }
  return SL_ERR_OK;
}