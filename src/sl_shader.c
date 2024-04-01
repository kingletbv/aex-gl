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

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef SL_DEFS_H_INCLUDED
#define SL_DEFS_H_INCLUDED
#include "sl_defs.h"
#endif

#ifndef SL_SHADER_H_INCLUDED
#define SL_SHADER_H_INCLUDED
#include "sl_shader.h"
#endif

#ifndef SL_COMPILATION_UNIT_H_INCLUDED
#define SL_COMPILATION_UNIT_H_INCLUDED
#include "sl_compilation_unit.h"
#endif

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
#endif

void sl_shader_init(struct sl_shader *sh) {
  sh->type_ = SLST_INVALID_SHADER;
  sh->source_length_ = 0;
  sh->source_ = NULL;
  sl_compilation_unit_init(&sh->cu_);
  sl_exec_init(&sh->exec_);
  
  sh->gl_shader_object_name_ = 0; /* no name assigned yet */
  sh->gl_delete_status_ = 0;
  sh->gl_last_compile_status_ = 0;
  sh->gl_info_log_size_ = sh->gl_info_log_size_allocated_ = 0;
  sh->gl_info_log_ = NULL;
}

void sl_shader_cleanup(struct sl_shader *sh) {
  sl_exec_cleanup(&sh->exec_);
  sl_compilation_unit_cleanup(&sh->cu_);
  if (sh->source_) free(sh->source_);
  if (sh->gl_info_log_) free(sh->gl_info_log_);
}

void sl_shader_set_type(struct sl_shader *sh, enum sl_shader_type typ) {
  sl_shader_cleanup(sh);
  sl_shader_init(sh);
  sh->type_ = typ;
}

int sl_shader_set_source(struct sl_shader *sh, size_t num_strings, const char **string_ptrs, const int *lengths) {
  size_t total_length = 0;
  size_t n;
  for (n = 0; n < num_strings; ++n) {
    if (lengths[n] < 0) return -2;
    size_t new_length = total_length + (size_t)lengths[n];
    if (new_length < total_length) {
      /* overflow */
      return SL_ERR_OVERFLOW;
    }
    if (lengths[n] && !string_ptrs[n]) {
      /* invalid argument */
      return SL_ERR_INVALID_ARG;
    }
    total_length = new_length;
  }
  size_t bufsize = total_length + 1;
  if (bufsize < total_length) return SL_ERR_OVERFLOW;

  char *buf = (char *)malloc(bufsize);
  if (!buf) return SL_ERR_NO_MEM;
  size_t at = 0;
  for (n = 0; n < num_strings; ++n) {
    /* If we get this far we can assume the buffers are good */
    memcpy(buf + at, string_ptrs[n], (size_t)lengths[n]);
    at += (size_t)lengths[n];
  }
  buf[at++] = '\0';
  assert(at == bufsize);
  if (sh->source_) free(sh->source_);
  sh->source_ = buf;
  sh->source_length_ = total_length;
  return 0;
}


