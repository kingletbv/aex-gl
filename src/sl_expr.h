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

#ifndef SL_EXPR_H
#define SL_EXPR_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef SITUS_H_INCLUDED
#define SITUS_H_INCLUDED
#include "pp/situs.h"
#endif

#ifndef SL_EXPR_TEMP_H_INCLUDED
#define SL_EXPR_TEMP_H_INCLUDED
#include "sl_expr_temp.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum expr_op {
  exop_invalid,

  exop_variable,
  exop_literal,
  exop_array_subscript,
  exop_component_selection, /* e.g. myvec3.xxz */
  exop_field_selection,     /* e.g. mystruct.myfield */
  exop_post_inc,
  exop_post_dec,
  exop_pre_inc,
  exop_pre_dec,

  exop_negate,
  exop_logical_not,

  exop_multiply,
  exop_divide,

  exop_add,
  exop_subtract,

  exop_lt,
  exop_le,
  exop_ge,
  exop_gt,

  exop_eq,
  exop_ne,

  exop_function_call,
  exop_constructor,

  exop_logical_and,
  exop_logical_or,
  exop_logical_xor,

  exop_assign,
  exop_mul_assign,
  exop_div_assign,
  exop_add_assign,
  exop_sub_assign,
        
  exop_sequence,    // comma operator

  exop_conditional  // ternary ?: operator
} expr_op_t;

struct sl_expr {
  expr_op_t op_;
  struct situs op_loc_;

  size_t num_children_;
  struct sl_expr **children_;

  /* exop_literal */
  struct sl_expr_temp literal_value_;

  /* exop_component_selection */
  uint8_t component_selection_[4];

  /* exop_field_selection */
  struct sl_type_field *field_selection_;

  /* exop_constructor, the type constructed */
  struct sl_type *constructor_type_;

};

void sl_expr_init(struct sl_expr *x);
void sl_expr_cleanup(struct sl_expr *x);
void sl_expr_free(struct sl_expr *x);
struct sl_expr *sl_expr_alloc_float_lit(float f, const struct situs *loc);
struct sl_expr *sl_expr_alloc_int_lit(int64_t i, const struct situs *loc);
struct sl_expr *sl_expr_alloc_bool_lit(int b, const struct situs *loc);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_EXPR_H */
