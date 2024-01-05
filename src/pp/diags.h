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

#ifndef DIAGS_H
#define DIAGS_H

#ifndef STDARG_H_INCLUDED
#define STDARG_H_INCLUDED
#include <stdarg.h> /* va_list */
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct situs;

/* Diagnostics across the various stages of compilation and/or different compilers */

struct diags {
  /* Error flags, indicates whether compilation should be aborted,
   * the fatal_error_ indicates if compilation should be aborted _now_. */
  int have_error_:1;
  int fatal_error_:1;

  /* Print handlers for error reporting; dx_diags_init() will assign a default one that
   * logs to stderr. */
  int (*vprintf_handler)(void *printf_baton, const char *file, int line_num, const char *fmt, va_list args);
  void *vprintf_baton_;

  /* Default line and file to use for error reporting, when not specifying situs explicitly
   * Callers should update these continuously with what makes sense (e.g. tracking the parser or 
   * following #line directives and such.) */
  const char *input_file_;
  int input_line_;
};

void dx_diags_init(struct diags *dx);
void dx_diags_cleanup(struct diags *dx);

int dx_vprintf(struct diags *dx, const char *fmt, va_list args);
int dx_printf(struct diags *dx, const char *fmt, ...);
int dx_error_loc(struct diags *dx, const struct situs *sit, const char *fmt, ...);
int dx_error(struct diags *dx, const char *fmt, ...);
int dx_fatal_loc(struct diags *dx, const struct situs *sit, const char *fmt, ...);
int dx_fatal(struct diags *dx, const char *fmt, ...);
int dx_warn_loc(struct diags *dx, const struct situs *sit, const char *fmt, ...);
int dx_warn(struct diags *dx, const char *fmt, ...);
void dx_no_memory(struct diags *dx);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DIAGS_H */
