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

#ifndef GLSL_ES1_ASSIST_H
#define GLSL_ES1_ASSIST_H

/* Helper functions for parsing GLSL ES 1. */

#ifdef __cplusplus
extern "C" {
#endif

struct sl_expr;

struct glsl_es1_function_call {
  char *name_;
  struct situs loc_;
  struct sl_type *constructor_type_;
  size_t num_parameters_;
  size_t num_parameters_allocated_;
  struct sl_expr **parameters_;
};

void glsl_es1_function_call_init(struct glsl_es1_function_call *fc);
void glsl_es1_function_call_move(struct glsl_es1_function_call *dst, struct glsl_es1_function_call *src);
void glsl_es1_function_call_cleanup(struct glsl_es1_function_call *fc);
int glsl_es1_function_call_append_parameter(struct glsl_es1_function_call *fc, struct sl_expr *expr);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GLSL_ES1_ASSIST_H */
