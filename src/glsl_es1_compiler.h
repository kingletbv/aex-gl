/* Copyright 2023 Kinglet B.V.
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

#ifndef GLSL_ES1_COMPILER_H
#define GLSL_ES1_COMPILER_H

#ifndef PP_H_INCLUDED
#define PP_H_INCLUDED
#include "pp.h"
#endif

#ifndef GLSL_ES_1_H_INCLUDED
#define GLSL_ES_1_H_INCLUDED
#include "glsl_es_1.h"
#endif

enum glsl_es1_compiler_result {
  GLSL_ES1_R_SUCCESS,
  GLSL_ES1_R_NEED_INPUT,
  GLSL_ES1_R_NEW_INCLUDE,
  GLSL_ES1_R_OLD_INCLUDE,
  GLSL_ES1_R_HAVE_ERRORS,
  GLSL_ES1_R_FAILED
};

struct glsl_es1_compiler {
  struct preprocessor pp_;
  struct glsl_es1_stack parser_;

  int all_done_:1;
  int have_error_:1;
  int fatal_error_:1;

  /* If true, then the next token could be a typename, provided one is
   * accepted in the current parser_ state. This is used to disambiguate
   * between a typename and a variable name in a declaration. E.g. suppose
   * we have (GLSL):
   *   struct X { .. }
   *   X X;
   * The first "X" should come in as a GLSL_ES1_TYPE_NAME, but if the second
   * "X" comes in as a GLSL_ES1_TYPE_NAME, then it will give rise to a syntax
   * error. Therefore, as soon as we see a GLSL_ES1_TYPE_NAME, we set this
   * flag to false, to prevent the next token from being a GLSL_ES1_TYPE_NAME.
   */
  int is_typename_permitted_:1;

  const char *glsl_input_file_;
  int         glsl_input_line_;

  /* Print handlers for error reporting */
  int (*vprintf_handler)(void *printf_baton, const char *file, int line_num, const char *fmt, va_list args);
  void *vprintf_baton_;
};

void glsl_es1_compiler_init(struct glsl_es1_compiler *compiler);
void glsl_es1_compiler_cleanup(struct glsl_es1_compiler *compiler);

enum glsl_es1_compiler_result glsl_es1_compiler_compile_mem(struct glsl_es1_compiler *cc, const char *glsl_input_filename, const char *glsl_input_text, size_t glsl_input_text_len);

int glsl_es1_compiler_printf(struct glsl_es1_compiler *cc, const char *fmt, ...);
int glsl_es1_compiler_error_loc(struct glsl_es1_compiler *cc, struct situs *sit, const char *fmt, ...);
int glsl_es1_compiler_error(struct glsl_es1_compiler *cc, const char *fmt, ...);
int glsl_es1_compiler_fatal_loc(struct glsl_es1_compiler *cc, struct situs *sit, const char *fmt, ...);
int glsl_es1_compiler_fatal(struct glsl_es1_compiler *cc, const char *fmt, ...);
int glsl_es1_compiler_warn_loc(struct glsl_es1_compiler *cc, struct situs *sit, const char *fmt, ...);
int glsl_es1_compiler_warn(struct glsl_es1_compiler *cc, const char *fmt, ...);
void glsl_es1_compiler_no_memory(struct glsl_es1_compiler *cc);



#endif /* GLSL_ES1_COMPILER_H */
