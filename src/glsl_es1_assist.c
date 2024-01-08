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

#ifndef DIAGS_H_INCLUDED
#define DIAGS_H_INCLUDED
#include "pp/diags.h"
#endif

#ifndef SYM_TABLE_H_INCLUDED
#define SYM_TABLE_H_INCLUDED
#include "sym_table.h"
#endif

#ifndef SL_EXPR_H_INCLUDED
#define SL_EXPR_H_INCLUDED
#include "sl_expr.h"
#endif

#ifndef SL_FRAME_H_INCLUDED
#define SL_FRAME_H_INCLUDED
#include "sl_frame.h"
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
      sl_expr_free(dst->parameters_[n].expr_);
      situs_cleanup(&dst->parameters_[n].loc_);
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
      sl_expr_free(fc->parameters_[n].expr_);
      situs_cleanup(&fc->parameters_[n].loc_);
    }
    free(fc->parameters_);
  }
}

int glsl_es1_function_call_append_parameter(struct glsl_es1_function_call *fc, struct sl_expr *param_expr, const struct situs *param_loc) {
  if (fc->num_parameters_ == fc->num_parameters_allocated_) {
    size_t new_num_parameters_allocated = fc->num_parameters_allocated_ + fc->num_parameters_allocated_ + 1;
    struct glsl_es1_function_call_parameter *new_parameters = realloc(fc->parameters_, new_num_parameters_allocated * sizeof(struct glsl_es1_function_call_parameter));
    if (!new_parameters) return -1;
    fc->parameters_ = new_parameters;
    fc->num_parameters_allocated_ = new_num_parameters_allocated;
  }
  fc->parameters_[fc->num_parameters_].expr_ = param_expr;
  situs_init(&fc->parameters_[fc->num_parameters_].loc_);
  situs_clone(&fc->parameters_[fc->num_parameters_].loc_, param_loc);
  fc->num_parameters_++;
  return 0;
}

struct sl_expr *glsl_es1_function_call_realize(struct diags *dx, struct sym_table *st, struct sl_type_base *tb, struct glsl_es1_function_call *fs) {
  if (fs->constructor_type_) {
    /* Constructor call. */
    /* XXX: Do this */
    dx_error_loc(dx, &fs->loc_, "XXX: Constructor \"%s\" not implemented %s(%d)", fs->name_, __FILE__, __LINE__);
    return NULL;
  }
  else {
    /* Regular function call. */
    if (!fs->name_) {
      /* No name, assume this was an issue that's already reported. */
      return NULL;
    }
    struct sl_type **param_types = (struct sl_type **)malloc(fs->num_parameters_ * sizeof(struct sl_type *));
    if (!param_types) {
      dx_no_memory(dx);
      return NULL;
    }
    size_t n;

    for (n = 0; n < fs->num_parameters_; ++n) {
      /* XXX: Find some way of validating the expression */
      param_types[n] = NULL;
    }

    for (n = 0; n < fs->num_parameters_; ++n) {
      if (!fs->parameters_[n].expr_) {
        /* No expression, assume this was an issue that's already reported. */
        free(param_types);
        return NULL;
      }
      param_types[n] = sl_expr_type(tb, fs->parameters_[n].expr_);

      if (!param_types[n]) {
        /* No type, assume this was an issue that's already reported. */
        free(param_types);
        return NULL;
      }
    }

    struct sym_table *st_found = NULL;
    struct sym *sym_found = NULL;
    struct sl_function *func_found = NULL;
    sl_function_call_search(st, fs->name_, fs->num_parameters_, param_types, &st_found, &sym_found, &func_found);

    if (!func_found) {
      /* Function not found, but symbol found. */
      if (sym_found && sym_found->kind_ == SK_FUNCTION) {
        /* Symbol is a function, but not a match. */
        dx_error_loc(dx, &fs->loc_, "Function \"%s\" is not a match for the parameter types, see function at line: %d", fs->name_, situs_line(&fs->loc_));
        free(param_types);
        return NULL;
      }
      else if (sym_found) {
        /* Symbol is not a function. */
        dx_error_loc(dx, &fs->loc_, "Symbol \"%s\" is not a function", fs->name_);
        free(param_types);
        return NULL;
      }
      else {
        /* Symbol not found. */
        dx_error_loc(dx, &fs->loc_, "Function \"%s\" not found", fs->name_);
        free(param_types);
        return NULL;
      }
    }

    struct sl_expr *function_expr = sl_expr_alloc_function_call(func_found, &fs->loc_, &fs->parameters_->expr_, sizeof(*fs->parameters_));

    if (!function_expr) {
      /* No memory. */
      dx_no_memory(dx);
      free(param_types);
      return NULL;
    }

    for (n = 0; n < func_found->num_parameters_; ++n) {
      function_expr->children_[n] = fs->parameters_[n].expr_;
      fs->parameters_[n].expr_ = NULL; /* Ownership transferred to function_expr */
    }

    free(param_types);

    return function_expr;
  }
}
