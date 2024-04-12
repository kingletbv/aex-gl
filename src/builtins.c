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

#define SL_PI 3.1415926535897932384626433

void builtin_not_implemented_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  /* XXX: Currently does nothing though ideally we'd be reporting on not-implemented builtins at compile time */
}

void builtin_not_implemented_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  /* XXX: Currently does nothing though ideally we'd be reporting on not-implemented builtins at compile time */
}


void builtin_radians_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column = FLOAT_REG_PTR(x, 0);
  float * restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0); 
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) (((float)SL_PI)*(opd)/180.f)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_radians_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) (((float)SL_PI)*(opd)/180.f)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0); 

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_radians_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) (((float)SL_PI)*(opd)/180.f)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_radians_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) (((float)SL_PI)*(opd)/180.f)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 3);
  opd_column = FLOAT_REG_PTR(x->children_[0], 3);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_radians_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, ((float)SL_PI) * opd.v_.f_ / 180.f);
}

void builtin_radians_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, ((float)SL_PI) * opd.v_.v_[0] / 180.f, ((float)SL_PI) * opd.v_.v_[1] / 180.f);
}

void builtin_radians_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, ((float)SL_PI) * opd.v_.v_[0] / 180.f, ((float)SL_PI) * opd.v_.v_[1] / 180.f,
                            ((float)SL_PI) * opd.v_.v_[2] / 180.f);
}

void builtin_radians_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, ((float)SL_PI) * opd.v_.v_[0] / 180.f, ((float)SL_PI) * opd.v_.v_[1] / 180.f,
                            ((float)SL_PI) * opd.v_.v_[2] / 180.f, ((float)SL_PI) * opd.v_.v_[3] / 180.f);
}

void builtin_degrees_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) (180.f*(opd)/((float)SL_PI))
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_degrees_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) (180.f*(opd)/((float)SL_PI))
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_degrees_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) (180.f*(opd)/((float)SL_PI))
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_degrees_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) (180.f*(opd)/((float)SL_PI))
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 3);
  opd_column = FLOAT_REG_PTR(x->children_[0], 3);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_degrees_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, 180.f * opd.v_.f_ / ((float)SL_PI));
}

void builtin_degrees_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, 180.f * opd.v_.v_[0] / ((float)SL_PI),
                            180.f * opd.v_.v_[1] / ((float)SL_PI));
}

void builtin_degrees_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, 180.f * opd.v_.v_[0] / ((float)SL_PI),
                            180.f * opd.v_.v_[1] / ((float)SL_PI),
                            180.f * opd.v_.v_[2] / ((float)SL_PI));
}

void builtin_degrees_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, 180.f * opd.v_.v_[0] / ((float)SL_PI),
                            180.f * opd.v_.v_[1] / ((float)SL_PI),
                            180.f * opd.v_.v_[2] / ((float)SL_PI),
                            180.f * opd.v_.v_[3] / ((float)SL_PI));
}


void builtin_sin_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
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
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

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
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

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
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 3);
  opd_column = FLOAT_REG_PTR(x->children_[0], 3);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_sin_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, sinf(opd.v_.f_));
}

void builtin_sin_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, sinf(opd.v_.v_[0]), sinf(opd.v_.v_[1]));
}

void builtin_sin_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, sinf(opd.v_.v_[0]), sinf(opd.v_.v_[1]), sinf(opd.v_.v_[2]));
}

void builtin_sin_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, sinf(opd.v_.v_[0]), sinf(opd.v_.v_[1]), sinf(opd.v_.v_[2]), sinf(opd.v_.v_[3]));
}

void builtin_cos_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) cosf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_cos_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) cosf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_cos_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) cosf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_cos_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) cosf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 3);
  opd_column = FLOAT_REG_PTR(x->children_[0], 3);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_cos_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, cosf(opd.v_.f_));
}

void builtin_cos_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, cosf(opd.v_.v_[0]), 
                            cosf(opd.v_.v_[1]));
}

void builtin_cos_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, cosf(opd.v_.v_[0]), 
                            cosf(opd.v_.v_[1]), 
                            cosf(opd.v_.v_[2]));
}

void builtin_cos_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, cosf(opd.v_.v_[0]), 
                            cosf(opd.v_.v_[1]), 
                            cosf(opd.v_.v_[2]), 
                            cosf(opd.v_.v_[3]));
}

void builtin_tan_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column = FLOAT_REG_PTR(x, 0);
  float * restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0); 
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) tanf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_tan_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) tanf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_tan_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) tanf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_tan_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) tanf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);
  
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 3);
  opd_column = FLOAT_REG_PTR(x->children_[0], 3);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_tan_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, tanf(opd.v_.f_));
}

void builtin_tan_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, tanf(opd.v_.v_[0]), 
                            tanf(opd.v_.v_[1]));
}

void builtin_tan_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, tanf(opd.v_.v_[0]), 
                            tanf(opd.v_.v_[1]), 
                            tanf(opd.v_.v_[2]));
}

void builtin_tan_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, tanf(opd.v_.v_[0]), 
                            tanf(opd.v_.v_[1]), 
                            tanf(opd.v_.v_[2]), 
                            tanf(opd.v_.v_[3]));
}

void builtin_asin_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) asinf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_asin_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) asinf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_asin_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) asinf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_asin_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) asinf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 3);
  opd_column = FLOAT_REG_PTR(x->children_[0], 3);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_asin_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, asinf(opd.v_.f_));
}

void builtin_asin_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, asinf(opd.v_.v_[0]), 
                            asinf(opd.v_.v_[1]));
}

void builtin_asin_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, asinf(opd.v_.v_[0]), 
                            asinf(opd.v_.v_[1]), 
                            asinf(opd.v_.v_[2]));
}

void builtin_asin_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, asinf(opd.v_.v_[0]), 
                            asinf(opd.v_.v_[1]), 
                            asinf(opd.v_.v_[2]), 
                            asinf(opd.v_.v_[3]));
}

void builtin_acos_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column = FLOAT_REG_PTR(x, 0);
  float * restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0); 
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) acosf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_acos_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) acosf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_acos_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) acosf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_acos_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) acosf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 3);
  opd_column = FLOAT_REG_PTR(x->children_[0], 3);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_acos_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, acosf(opd.v_.f_));
}

void builtin_acos_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, acosf(opd.v_.v_[0]), 
                            acosf(opd.v_.v_[1]));
}

void builtin_acos_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, acosf(opd.v_.v_[0]), 
                            acosf(opd.v_.v_[1]), 
                            acosf(opd.v_.v_[2]));
}

void builtin_acos_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, acosf(opd.v_.v_[0]), 
                            acosf(opd.v_.v_[1]), 
                            acosf(opd.v_.v_[2]), 
                            acosf(opd.v_.v_[3]));
}

void builtin_atan_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column = FLOAT_REG_PTR(x, 0);
  float * restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0); 
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) atanf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_atan_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) atanf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_atan_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) atanf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_atan_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) atanf(opd)
#define UNOP_SNIPPET_TYPE float
  result_column = FLOAT_REG_PTR(x, 0);
  opd_column = FLOAT_REG_PTR(x->children_[0], 0);

#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 1);
  opd_column = FLOAT_REG_PTR(x->children_[0], 1);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 2);
  opd_column = FLOAT_REG_PTR(x->children_[0], 2);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = FLOAT_REG_PTR(x, 3);
  opd_column = FLOAT_REG_PTR(x->children_[0], 3);

  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_atan_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, atanf(opd.v_.f_));
}

void builtin_atan_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, atanf(opd.v_.v_[0]), 
                            atanf(opd.v_.v_[1]));
}

void builtin_atan_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, atanf(opd.v_.v_[0]), 
                            atanf(opd.v_.v_[1]), 
                            atanf(opd.v_.v_[2]));
}

void builtin_atan_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, atanf(opd.v_.v_[0]), 
                            atanf(opd.v_.v_[1]), 
                            atanf(opd.v_.v_[2]), 
                            atanf(opd.v_.v_[3]));
}

void builtin_atan_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column = FLOAT_REG_PTR(x, 0);
  float * restrict left_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict right_column = FLOAT_REG_PTR(x->children_[1], 0);
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(y, x) atanf(y/x)
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_atan_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(y, x) atanf(y/x)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_atan_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(y, x) atanf(y/x)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_atan_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(y, x) atanf(y/x)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 3);
  right_column = FLOAT_REG_PTR(x->children_[1], 3);
  result_column = FLOAT_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_atan_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  sl_expr_temp_init_float(r, atanf(opd0.v_.f_/opd1.v_.f_));
}

void builtin_atan_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  sl_expr_temp_init_vec2(r, atanf(opd0.v_.v_[0]/opd1.v_.v_[0]),
                            atanf(opd0.v_.v_[1]/opd1.v_.v_[1]));
}

void builtin_atan_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  sl_expr_temp_init_vec3(r, atanf(opd0.v_.v_[0]/opd1.v_.v_[0]),
                            atanf(opd0.v_.v_[1]/opd1.v_.v_[1]),
                            atanf(opd0.v_.v_[2]/opd1.v_.v_[2]));
}

void builtin_atan_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  sl_expr_temp_init_vec4(r, atanf(opd0.v_.v_[0]/opd1.v_.v_[0]),
                            atanf(opd0.v_.v_[1]/opd1.v_.v_[1]),
                            atanf(opd0.v_.v_[2]/opd1.v_.v_[2]),
                            atanf(opd0.v_.v_[3]/opd1.v_.v_[3]));
}

