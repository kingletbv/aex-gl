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

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED
#include <math.h>
#endif

#ifndef BUILTINS_H_INCLUDED
#define BUILTINS_H_INCLUDED
#include "builtins.h"
#endif

#ifndef SL_EXPR_H_INCLUDED
#define SL_EXPR_H_INCLUDED
#include "sl_expr.h"
#endif

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
#endif

void builtin_not_implemented_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  /* XXX: Currently does nothing though ideally we'd be reporting on not-implemented builtins at compile time */
}

void builtin_not_implemented_eval(struct sl_type_base *tb, struct sl_expr *x, struct sl_expr_temp *r) {
  /* XXX: Currently does nothing though ideally we'd be reporting on not-implemented builtins at compile time */
}

void builtin_sin_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column = FLOAT_REG_PTR(x->children_[0], 0);
  float * restrict opd_column = FLOAT_REG_PTR(x, 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) sinf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_sin_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) sinf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x->children_[0], 0);
  opd_column = FLOAT_REG_PTR(x, 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x->children_[0], 1);
  opd_column = FLOAT_REG_PTR(x, 1);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_sin_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) sinf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x->children_[0], 0);
  opd_column = FLOAT_REG_PTR(x, 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x->children_[0], 1);
  opd_column = FLOAT_REG_PTR(x, 1);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x->children_[0], 2);
  opd_column = FLOAT_REG_PTR(x, 2);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_sin_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) sinf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x->children_[0], 0);
  opd_column = FLOAT_REG_PTR(x, 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x->children_[0], 1);
  opd_column = FLOAT_REG_PTR(x, 1);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x->children_[0], 2);
  opd_column = FLOAT_REG_PTR(x, 2);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x->children_[0], 3);
  opd_column = FLOAT_REG_PTR(x, 3);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_sin_f_eval(struct sl_type_base *tb, struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, sinf(opd.v_.f_));
}

void builtin_sin_v2_eval(struct sl_type_base *tb, struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, sinf(opd.v_.v_[0]), sinf(opd.v_.v_[1]));
}

void builtin_sin_v3_eval(struct sl_type_base *tb, struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, sinf(opd.v_.v_[0]), sinf(opd.v_.v_[1]), sinf(opd.v_.v_[2]));
}

void builtin_sin_v4_eval(struct sl_type_base *tb, struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, sinf(opd.v_.v_[0]), sinf(opd.v_.v_[1]), sinf(opd.v_.v_[2]), sinf(opd.v_.v_[3]));
}
