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

#ifndef SL_EXPR_H_INCLUDED
#define SL_EXPR_H_INCLUDED
#include "sl_expr.h"
#endif


void sl_expr_init(struct sl_expr *x) {
  x->op_ = exop_invalid;
  situs_init(&x->op_loc_);
  x->num_children_ = 0;
  x->children_ = NULL;
  sl_expr_temp_init_void(&x->literal_value_);
  memset(x->component_selection_, 0, sizeof(x->component_selection_));
  x->field_selection_ = NULL;
  x->constructor_type_ = NULL;
}

void sl_expr_cleanup(struct sl_expr *x) {
  if (x->children_) {
    /* We don't free the entire tree as that would assume the allocation,
     * use sl_expr_free() for this. */
    free(x->children_);
    x->children_ = NULL;
  }
  sl_expr_temp_cleanup(&x->literal_value_);
}

void sl_expr_free(struct sl_expr *x) {
  size_t n;
  if (!x) return;
  for (n = 0; n < x->num_children_; ++n) {
    sl_expr_free(x->children_[n]);
  }
  sl_expr_cleanup(x);
  free(x);
}

struct sl_expr *sl_expr_alloc(expr_op_t op, const struct situs *loc) {
  struct sl_expr *x = (struct sl_expr *)malloc(sizeof(struct sl_expr));
  if (!x) return NULL;
  sl_expr_init(x);
  x->op_ = op;
  if (situs_clone(&x->op_loc_, loc)) {
    /* Failed memory alloc */
    sl_expr_free(x);
    return NULL;
  }
  return x;
}

struct sl_expr *sl_expr_alloc_float_lit(float f, const struct situs *loc) {
  struct sl_expr *x = sl_expr_alloc(exop_literal, loc);
  if (!x) return NULL;
  sl_expr_temp_init_float(&x->literal_value_, f);
  return x;
}

struct sl_expr *sl_expr_alloc_int_lit(int64_t i, const struct situs *loc) {
  struct sl_expr *x = sl_expr_alloc(exop_literal, loc);
  if (!x) return NULL;
  sl_expr_temp_init_int(&x->literal_value_, i);
  return x;
}

struct sl_expr *sl_expr_alloc_bool_lit(int b, const struct situs *loc) {
  struct sl_expr *x = sl_expr_alloc(exop_literal, loc);
  if (!x) return NULL;
  sl_expr_temp_init_bool(&x->literal_value_, b);
  return x;
}
