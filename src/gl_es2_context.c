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
      fa->next_ = fa->prev_ = NULL;
    }
    else /* not the last attachment to this renderbuffer */ {
      if (rb->first_framebuffer_attached_to_ == fa) {
        rb->first_framebuffer_attached_to_ = fa->next_;
      }
      fa->next_->prev_ = fa->prev_;
      fa->prev_->next_ = fa->next_;
    }
    fa->kind_ = gl_es2_faot_none;
  }
  else if (fa->kind_ == gl_es2_faot_texture) {
    struct gl_es2_texture *tex;
    tex = fa->v_.tex_;
    if (fa->next_ == fa) {
      tex->first_framebuffer_attached_to_ = NULL;
      fa->next_ = fa->prev_ = NULL;
    }
    else /* not the last attachment to this framebuffer */ {
      if (tex->first_framebuffer_attached_to_ == fa) {
        tex->first_framebuffer_attached_to_ = fa->next_;
      }
      fa->next_->prev_ = fa->prev_;
      fa->prev_->next_ = fa->next_;
    }
    fa->kind_ = gl_es2_faot_none;
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
  tex->first_framebuffer_attached_to_ = NULL;
}

void gl_es2_texture_cleanup(struct gl_es2_texture *tex) {
  while (tex->first_framebuffer_attached_to_) {
    gl_es2_framebuffer_attachment_detach(tex->first_framebuffer_attached_to_);
  }
}

void gl_es2_ctx_init(struct gl_es2_context *c) {
  c->current_error_ = GL_ES2_NO_ERROR;

  ref_range_allocator_init(&c->framebuffer_rra_);
  not_init(&c->framebuffer_not_);

  ref_range_allocator_init(&c->renderbuffer_rra_);
  not_init(&c->renderbuffer_not_);

  c->framebuffer_ = NULL;
  c->renderbuffer_ = NULL;
}

void gl_es2_ctx_cleanup(struct gl_es2_context *c) {
  ref_range_allocator_cleanup(&c->framebuffer_rra_);
  while (c->framebuffer_not_.seq_) {
    struct gl_es2_framebuffer *fb = (struct gl_es2_framebuffer *)c->framebuffer_not_.seq_;
    not_remove(&c->framebuffer_not_, &fb->no_);
    gl_es2_framebuffer_cleanup(fb);
    free(fb);
  }
  while (c->renderbuffer_not_.seq_) {
    struct gl_es2_renderbuffer *rb = (struct gl_es2_renderbuffer *)c->renderbuffer_not_.seq_;
    not_remove(&c->renderbuffer_not_, &rb->no_);
    gl_es2_renderbuffer_cleanup(rb);
    free(rb);
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

