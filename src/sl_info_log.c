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

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef STDARG_H_INCLUDED
#define STDARG_H_INCLUDED
#include <stdarg.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef SL_INFO_LOG_H_INCLUDED
#define SL_INFO_LOG_H_INCLUDED
#include "sl_info_log.h"
#endif

#ifndef DIAGS_H_INCLUDED
#define DIAGS_H_INCLUDED
#include "diags.h"
#endif

static int sl_info_log_dx_vprintf(void *log_ptr, const char *fmt, va_list args) {
  struct sl_info_log *log = (struct sl_info_log *)log_ptr;

  size_t mem_needed = 0; /* including NULL terminator */

  va_list cargs;
  // XXX: For now we'll also output to stderr for ease of debugging.
  // XXX: DELETE THIS
  va_copy(cargs, args);
  vfprintf(stderr, fmt, cargs);
  va_end(cargs);

  va_copy(cargs, args);
#ifdef _MSC_VER
  mem_needed = _vscprintf(fmt, cargs) + 1;
#else
  mem_needed = vsnprintf(NULL, 0, fmt, cargs) + 1;
#endif
  va_end(cargs);

  size_t size_needed = log->gl_info_log_size_ + mem_needed;
  if (size_needed > log->gl_info_log_size_allocated_) {
    void *newbuf = realloc(log->gl_info_log_, size_needed);
    if (!newbuf) {
      return -1;
    }
    log->gl_info_log_ = (char *)newbuf;
    log->gl_info_log_size_allocated_ = size_needed;
  }
  vsnprintf(log->gl_info_log_ + log->gl_info_log_size_, mem_needed, fmt, args);
  /* point the end *at* the null terminator, so we'll overwrite it when we write another line */
  log->gl_info_log_size_ = size_needed - 1;

  return 0;
}

static int sl_info_log_dx_printf(void *log_ptr, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int r;
  r = sl_info_log_dx_vprintf(log_ptr, fmt, args);
  va_end(args);
  return r;
}

static int sl_info_log_dx_cb(void *log_ptr, const char *file, int line_num, const char *fmt, va_list args) {
  int r;
  struct sl_info_log *log = (struct sl_info_log *)log_ptr;
  
  if (file) {
    sl_info_log_dx_printf(log_ptr, "%s(%d): ", file, line_num);
  }

  r = sl_info_log_dx_vprintf(log_ptr, fmt, args);

  return r;
}


void sl_info_log_init(struct sl_info_log *log) {
  dx_diags_init(&log->dx_);
  log->dx_.vprintf_baton_ = log;
  log->dx_.vprintf_handler = sl_info_log_dx_cb;
  log->gl_info_log_size_ = log->gl_info_log_size_allocated_ = 0;
  log->gl_info_log_ = NULL;
}

void sl_info_log_cleanup(struct sl_info_log *log) {
  dx_diags_cleanup(&log->dx_);
  if (log->gl_info_log_) free(log->gl_info_log_);
}

void sl_info_log_get_log(struct sl_info_log *log, size_t bufsize, size_t *length_used, char *buf) {
  size_t amount_to_copy = log->gl_info_log_size_;
  if (!bufsize) return;
  bufsize -= 1; /* remove space for null terminator */
  if (bufsize < amount_to_copy) {
    amount_to_copy = bufsize;
  }
  memcpy(buf, log->gl_info_log_, amount_to_copy);
  buf[amount_to_copy] = '\0'; /* noticed how above we removed one space for a null terminator, here we use it again. */
  if (length_used) *length_used = amount_to_copy;
}

void sl_info_log_clear(struct sl_info_log *log) {
  log->gl_info_log_size_ = 0;
}

