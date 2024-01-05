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

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef DIAGS_H_INCLUDED
#define DIAGS_H_INCLUDED
#include "diags.h"
#endif

#ifndef SITUS_H_INCLUDED
#define SITUS_H_INCLUDED
#include "situs.h"
#endif

static int dx_stderr_vprintf_handler(void *baton, const char *file, int line_num, const char *fmt, va_list args) {
  if (file) {
    fprintf(stderr, "%s(%d): ", file, line_num);
  }

  int r;
  r = vfprintf(stderr, fmt, args);

  return r < 0;
}

void dx_diags_init(struct diags *dx) {
  dx->have_error_ = 0;
  dx->fatal_error_ = 0;
  dx->vprintf_handler = dx_stderr_vprintf_handler;
  dx->vprintf_baton_ = NULL;
  dx->input_file_ = "";
  dx->input_line_ = 0;
}

void dx_diags_cleanup(struct diags *dx) {
}

int dx_printf(struct diags *dx, const char *fmt, ...);
int dx_error_loc(struct diags *dx, struct situs *sit, const char *fmt, ...);
int dx_error(struct diags *dx, const char *fmt, ...);
int dx_fatal_loc(struct diags *dx, struct situs *sit, const char *fmt, ...);
int dx_fatal(struct diags *dx, const char *fmt, ...);
int dx_warn_loc(struct diags *dx, struct situs *sit, const char *fmt, ...);
int dx_warn(struct diags *dx, const char *fmt, ...);
void dx_no_memory(struct diags *dx);

int dx_vprintf(struct diags *dx, const char *fmt, va_list args) {
  return dx->vprintf_handler(dx->vprintf_baton_, NULL, 0, fmt, args);
}


int dx_printf(struct diags *dx, const char *fmt, ...) {
  int r;
  va_list args;
  va_start(args, fmt);
  r = dx->vprintf_handler(dx->vprintf_baton_, NULL, 0, fmt, args);
  va_end(args);
  return r;
}

int dx_error_loc(struct diags *dx, struct situs *sit, const char *fmt, ...) {
  int r;
  va_list args;
  // Duplicate fmt to append newline.
  size_t fmt_len = strlen(fmt);
  char *dup_fmt = malloc(fmt_len + 2);
  if (!dup_fmt) {
    dx_no_memory(dx);
    return -1;
  }
  memcpy(dup_fmt, fmt, fmt_len);
  dup_fmt[fmt_len] = '\n';
  dup_fmt[fmt_len + 1] = '\0';
  dx->have_error_ = 1;
  va_start(args, fmt);
  r = dx->vprintf_handler(dx->vprintf_baton_, situs_filename(sit), situs_line(sit), dup_fmt, args);
  va_end(args);
  free(dup_fmt);
  return r;
}

int dx_error(struct diags *dx, const char *fmt, ...) {
  int r;
  va_list args;
  dx->have_error_ = 1;
  va_start(args, fmt);
  r = dx->vprintf_handler(dx->vprintf_baton_, dx->input_file_, dx->input_line_, fmt, args);
  va_end(args);
  return r;
}

int dx_fatal_loc(struct diags *dx, struct situs *sit, const char *fmt, ...) {
  int r;
  va_list args;
  dx->have_error_ = dx->fatal_error_ = 1;
  va_start(args, fmt);
  r = dx->vprintf_handler(dx->vprintf_baton_, situs_filename(sit), situs_line(sit), fmt, args);
  va_end(args);
  return r;
}

int dx_fatal(struct diags *dx, const char *fmt, ...) {
  int r;
  va_list args;
  dx->have_error_ = dx->fatal_error_ = 1;
  va_start(args, fmt);
  r = dx->vprintf_handler(dx->vprintf_baton_, dx->input_file_, dx->input_line_, fmt, args);
  va_end(args);
  return r;
}

int dx_warn_loc(struct diags *dx, struct situs *sit, const char *fmt, ...) {
  int r;
  va_list args;
  // Duplicate fmt to append newline.
  size_t fmt_len = strlen(fmt);
  char *dup_fmt = malloc(fmt_len + 2);
  if (!dup_fmt) {
    dx_no_memory(dx);
    return -1;
  }
  memcpy(dup_fmt, fmt, fmt_len);
  dup_fmt[fmt_len] = '\n';
  dup_fmt[fmt_len + 1] = '\0';
  va_start(args, fmt);
  r = dx->vprintf_handler(dx->vprintf_baton_, situs_filename(sit), situs_line(sit), dup_fmt, args);
  va_end(args);
  free(dup_fmt);
  return r;
}

int dx_warn(struct diags *dx, const char *fmt, ...) {
  int r;
  va_list args;
  va_start(args, fmt);
  r = dx->vprintf_handler(dx->vprintf_baton_, dx->input_file_, dx->input_line_, fmt, args);
  va_end(args);
  return r;
}

void dx_no_memory(struct diags *dx) {
  dx_fatal(dx, "No memory\n");
}
