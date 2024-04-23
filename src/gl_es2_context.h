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

#ifndef GL_ES2_IMPL_TYPES_H_INCLUDED
#define GL_ES2_IMPL_TYPES_H_INCLUDED
#include "gl_es2_impl_types.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct gl_es2_framebuffer {
  int name_;
};

struct gl_es2_context {
  gl_es2_enum current_error_;

  struct ref_range_allocator framebuffer_rra_;
};

struct gl_es2_context *gl_es2_ctx(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GL_ES2_CONTEXT_H */