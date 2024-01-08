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

#ifndef SITUS_H_INCLUDED
#define SITUS_H_INCLUDED
#include "pp/situs.h"
#endif

#ifndef SL_EXPR_H_INCLUDED
#define SL_EXPR_H_INCLUDED
#include "sl_expr.h"
#endif

#ifndef GLSL_ES1_ASSIST_H_INCLUDED
#define GLSL_ES1_ASSIST_H_INCLUDED
#include "glsl_es1_assist.h"
#endif

void glsl_es1_function_call_init(struct glsl_es1_function_call *fc) {
  fc->name_ = NULL;
  situs_init(&fc->loc_);
  fc->constructor_type_ = NULL;
  fc->num_parameters_ = 0;
  fc->num_parameters_allocated_ = 0;
  fc->parameters_ = NULL;
}

void glsl_es1_function_call_move(struct glsl_es1_function_call *dst, struct glsl_es1_function_call *src) {
  if (dst->name_) free(dst->name_);
  dst->name_ = src->name_;
  src->name_ = NULL;
  situs_swap(&dst->loc_, &src->loc_);
  dst->constructor_type_ = src->constructor_type_;
  src->constructor_type_ = NULL;
  if (dst->parameters_) {
    size_t n;
    for (n = 0; n < dst->num_parameters_; ++n) {
      sl_expr_free(dst->parameters_[n]);
    }
    free(dst->parameters_);
  }
  dst->num_parameters_ = src->num_parameters_;
  src->num_parameters_ = 0;
  dst->num_parameters_allocated_ = src->num_parameters_allocated_;
  src->num_parameters_allocated_ = 0;
  dst->parameters_ = src->parameters_;
  src->parameters_ = NULL;
}

void glsl_es1_function_call_cleanup(struct glsl_es1_function_call *fc) {
  if (fc->name_) free(fc->name_);
  situs_cleanup(&fc->loc_);
  if (fc->parameters_) {
    size_t n;
    for (n = 0; n < fc->num_parameters_; ++n) {
      sl_expr_free(fc->parameters_[n]);
    }
    free(fc->parameters_);
  }
}

int glsl_es1_function_call_append_parameter(struct glsl_es1_function_call *fc, struct sl_expr *expr) {
  if (fc->num_parameters_ == fc->num_parameters_allocated_) {
    size_t new_num_parameters_allocated = fc->num_parameters_allocated_ + fc->num_parameters_allocated_ + 1;
    struct sl_expr **new_parameters = realloc(fc->parameters_, new_num_parameters_allocated * sizeof(struct sl_expr *));
    if (!new_parameters) return -1;
    fc->parameters_ = new_parameters;
    fc->num_parameters_allocated_ = new_num_parameters_allocated;
  }
  fc->parameters_[fc->num_parameters_++] = expr;
  return 0;
}

