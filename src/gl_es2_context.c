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
}

void gl_es2_renderbuffer_cleanup(struct gl_es2_renderbuffer *rb) {
  while (rb->first_framebuffer_attached_to_) {
    gl_es2_framebuffer_attachment_detach(rb->first_framebuffer_attached_to_);
  }
}

void gl_es2_texture_init(struct gl_es2_texture *tex) {
  tex->kind_ = gl_es2_texture_invalid;
  tex->first_framebuffer_attached_to_ = NULL;
}

void gl_es2_texture_cleanup(struct gl_es2_texture *tex) {
  while (tex->first_framebuffer_attached_to_) {
    gl_es2_framebuffer_attachment_detach(tex->first_framebuffer_attached_to_);
  }
}

void gl_es2_buffer_init(struct gl_es2_buffer *buf) {
}

void gl_es2_buffer_cleanup(struct gl_es2_buffer *buf) {
}

void gl_es2_program_init(struct gl_es2_program *prog) {
  prog->flagged_for_deletion_ = 0;
  gl_es2_program_shader_attachment_init(prog, &prog->vertex_shader_);
  gl_es2_program_shader_attachment_init(prog, &prog->fragment_shader_);
}

void gl_es2_program_cleanup(struct gl_es2_program *prog) {
  gl_es2_program_shader_attachment_cleanup(&prog->vertex_shader_);
  gl_es2_program_shader_attachment_cleanup(&prog->fragment_shader_);
}

void gl_es2_shader_init(struct gl_es2_shader *shad) {
  shad->flagged_for_deletion_ = 0;
  shad->first_program_attached_to_ = NULL;
}

void gl_es2_shader_cleanup(struct gl_es2_shader *shad) {
  while (shad->first_program_attached_to_) {
    gl_es2_program_shader_attachment_detach(shad->first_program_attached_to_);
  }
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

  c->framebuffer_ = NULL;
  c->renderbuffer_ = NULL;

  c->num_active_texture_units_ = GL_ES2_MAX_NUM_TEXTURE_UNITS;
  c->current_active_texture_unit_ = 0;
  size_t n;
  for (n = 0; n < c->num_active_texture_units_; ++n) {
    c->active_texture_units_[n].texture_2d_ = NULL;
    c->active_texture_units_[n].texture_cube_map_ = NULL;
  }

  c->array_buffer_ = NULL;
  c->element_array_buffer_ = NULL;
  c->current_program_ = NULL;
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
}

static void gl_es2_at_exit_cleanup(void) {
  if (g_ctx_is_initialized_) {
    gl_es2_ctx_cleanup(&g_ctx_);
  }
}

struct gl_es2_context *gl_es2_ctx(void) {
  if (!g_ctx_is_initialized_) {
    gl_es2_ctx_init(&g_ctx_);
    g_ctx_is_initialized_ = 1;
    atexit(gl_es2_at_exit_cleanup);
  }

  return &g_ctx_;
}

