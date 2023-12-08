/* Copyright 2020-2023 Kinglet B.V.
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
  int fatal_error_:1;

  const char *glsl_input_file_;
  int         glsl_input_line_;
};

void glsl_es1_compiler_init(struct glsl_es1_compiler *compiler);
void glsl_es1_compiler_cleanup(struct glsl_es1_compiler *compiler);

#endif /* GLSL_ES1_COMPILER_H */
