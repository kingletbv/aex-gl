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


void gl_es2_framebuffer_init(struct gl_es2_framebuffer *fb) {
}

void gl_es2_framebuffer_cleanup(struct gl_es2_framebuffer *fb) {
}

void gl_es2_renderbuffer_init(struct gl_es2_renderbuffer *rb) {
}

void gl_es2_renderbuffer_cleanup(struct gl_es2_renderbuffer *rb) {
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

