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
struct situs;

struct glsl_es1_function_call_parameter {
  struct sl_expr *expr_;
  struct situs loc_;
};

struct glsl_es1_function_call {
  char *name_;
  struct situs loc_;
  struct sl_type *constructor_type_;
  size_t num_parameters_;
  size_t num_parameters_allocated_;
  struct glsl_es1_function_call_parameter *parameters_;
};

void glsl_es1_function_call_init(struct glsl_es1_function_call *fc);
void glsl_es1_function_call_move(struct glsl_es1_function_call *dst, struct glsl_es1_function_call *src);
void glsl_es1_function_call_cleanup(struct glsl_es1_function_call *fc);
int glsl_es1_function_call_append_parameter(struct glsl_es1_function_call *fc, struct sl_expr *param_expr, const struct situs *param_loc);
struct sl_expr *glsl_es1_field_or_swizzle_selection(struct diags *dx, struct sl_type_base *tb, struct sl_expr *x, char *field_id, const struct situs *field_loc);
struct sl_expr *glsl_es1_function_call_realize(struct diags *dx, struct sym_table *st, struct sl_type_base *tb, struct glsl_es1_function_call *fs);
int glsl_es1_declare_variable(struct glsl_es1_compiler *cc, const char *name, const struct situs *loc, struct sl_type *typ, struct sl_variable **ppvar);
int glsl_es1_build_array_type(struct diags *dx, struct sl_type_base *tb, struct sl_type *base_type, struct sl_expr *size_expr, const struct situs *size_expr_loc,
                              struct sl_type **parray_type);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GLSL_ES1_ASSIST_H */
