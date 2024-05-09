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


/* Exponential functions */

void builtin_pow_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict left_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict right_column = FLOAT_REG_PTR(x->children_[1], 0);
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) powf(x, y)
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_pow_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) powf(x, y)
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

void builtin_pow_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) powf(x, y)
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

void builtin_pow_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) powf(x, y)
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

void builtin_pow_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_float(r, powf(opd0.v_.f_, opd1.v_.f_));
}

void builtin_pow_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec2(r, powf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            powf(opd0.v_.v_[1], opd1.v_.v_[1]));
}

void builtin_pow_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec3(r, powf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            powf(opd0.v_.v_[1], opd1.v_.v_[1]),
                            powf(opd0.v_.v_[2], opd1.v_.v_[2]));
}

void builtin_pow_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec4(r, powf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            powf(opd0.v_.v_[1], opd1.v_.v_[1]),
                            powf(opd0.v_.v_[2], opd1.v_.v_[2]),
                            powf(opd0.v_.v_[3], opd1.v_.v_[3]));
}
void builtin_exp_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) expf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_exp_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) expf(opd)
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

void builtin_exp_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) expf(opd)
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

void builtin_exp_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) expf(opd)
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

void builtin_exp_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, expf(opd.v_.f_));
}

void builtin_exp_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, expf(opd.v_.v_[0]), expf(opd.v_.v_[1]));
}

void builtin_exp_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, expf(opd.v_.v_[0]), expf(opd.v_.v_[1]), expf(opd.v_.v_[2]));
}

void builtin_exp_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, expf(opd.v_.v_[0]), expf(opd.v_.v_[1]), expf(opd.v_.v_[2]), expf(opd.v_.v_[3]));
}

void builtin_log_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) logf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_log_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) logf(opd)
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

void builtin_log_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) logf(opd)
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

void builtin_log_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) logf(opd)
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

void builtin_log_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, logf(opd.v_.f_));
}

void builtin_log_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, logf(opd.v_.v_[0]), logf(opd.v_.v_[1]));
}

void builtin_log_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, logf(opd.v_.v_[0]), logf(opd.v_.v_[1]), logf(opd.v_.v_[2]));
}

void builtin_log_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, logf(opd.v_.v_[0]), logf(opd.v_.v_[1]), logf(opd.v_.v_[2]), logf(opd.v_.v_[3]));
}

void builtin_exp2_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) exp2f(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_exp2_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) exp2f(opd)
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

void builtin_exp2_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) exp2f(opd)
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

void builtin_exp2_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) exp2f(opd)
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

void builtin_exp2_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, exp2f(opd.v_.f_));
}

void builtin_exp2_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, exp2f(opd.v_.v_[0]), exp2f(opd.v_.v_[1]));
}

void builtin_exp2_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, exp2f(opd.v_.v_[0]), exp2f(opd.v_.v_[1]), exp2f(opd.v_.v_[2]));
}

void builtin_exp2_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, exp2f(opd.v_.v_[0]), exp2f(opd.v_.v_[1]), exp2f(opd.v_.v_[2]), exp2f(opd.v_.v_[3]));
}

void builtin_log2_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) log2f(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_log2_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) log2f(opd)
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

void builtin_log2_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) log2f(opd)
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

void builtin_log2_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) log2f(opd)
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

void builtin_log2_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, log2f(opd.v_.f_));
}

void builtin_log2_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, log2f(opd.v_.v_[0]), log2f(opd.v_.v_[1]));
}

void builtin_log2_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, log2f(opd.v_.v_[0]), log2f(opd.v_.v_[1]), log2f(opd.v_.v_[2]));
}

void builtin_log2_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, log2f(opd.v_.v_[0]), log2f(opd.v_.v_[1]), log2f(opd.v_.v_[2]), log2f(opd.v_.v_[3]));
}

void builtin_sqrt_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) sqrtf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_sqrt_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) sqrtf(opd)
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

void builtin_sqrt_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) sqrtf(opd)
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

void builtin_sqrt_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) sqrtf(opd)
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

void builtin_sqrt_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, sqrtf(opd.v_.f_));
}

void builtin_sqrt_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, sqrtf(opd.v_.v_[0]), sqrtf(opd.v_.v_[1]));
}

void builtin_sqrt_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, sqrtf(opd.v_.v_[0]), sqrtf(opd.v_.v_[1]), sqrtf(opd.v_.v_[2]));
}

void builtin_sqrt_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, sqrtf(opd.v_.v_[0]), sqrtf(opd.v_.v_[1]), sqrtf(opd.v_.v_[2]), sqrtf(opd.v_.v_[3]));
}

void builtin_inversesqrt_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) 1.f/sqrtf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_inversesqrt_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) 1.f/sqrtf(opd)
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

void builtin_inversesqrt_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) 1.f/sqrtf(opd)
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

void builtin_inversesqrt_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) 1.f/sqrtf(opd)
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

void builtin_inversesqrt_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, 1.f/sqrtf(opd.v_.f_));
}

void builtin_inversesqrt_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, 1.f/sqrtf(opd.v_.v_[0]), 1.f/sqrtf(opd.v_.v_[1]));
}

void builtin_inversesqrt_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, 1.f/sqrtf(opd.v_.v_[0]), 1.f/sqrtf(opd.v_.v_[1]), 1.f/sqrtf(opd.v_.v_[2]));
}

void builtin_inversesqrt_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, 1.f/sqrtf(opd.v_.v_[0]), 1.f/sqrtf(opd.v_.v_[1]), 1.f/sqrtf(opd.v_.v_[2]), 1.f/sqrtf(opd.v_.v_[3]));
}

/* Common Functions */

static float aex_signf(float x) {
  return (float)((x > 0.f) - (x < 0.f));
}

static float aex_fractf(float x) {
  return x - floorf(x);
}

static float aex_modf(float x, float y) {
  return x - y * floorf(x/y);
}

static float aex_clampf(float x, float minval, float maxval) {
  return fminf(fmaxf(x, minval), maxval);
}

static float aex_mixf(float x, float y, float a) {
  return x * (1.f - a) + y * a;
}

static float aex_stepf(float edge, float x) {
  return (float)(x >= edge);
}

static float aex_smoothstepf(float edge0, float edge1, float x) {
  float t;
  t = aex_clampf((x - edge0) / (edge1 - edge0), 0.f, 1.f);
  return t * t * (3.f - 2.f * t);
}

void builtin_abs_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) fabsf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_abs_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) fabsf(opd)
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

void builtin_abs_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) fabsf(opd)
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

void builtin_abs_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) fabsf(opd)
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

void builtin_abs_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, fabsf(opd.v_.f_));
}

void builtin_abs_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, fabsf(opd.v_.v_[0]), fabsf(opd.v_.v_[1]));
}

void builtin_abs_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, fabsf(opd.v_.v_[0]), fabsf(opd.v_.v_[1]), fabsf(opd.v_.v_[2]));
}

void builtin_abs_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, fabsf(opd.v_.v_[0]), fabsf(opd.v_.v_[1]), fabsf(opd.v_.v_[2]), fabsf(opd.v_.v_[3]));
}

void builtin_sign_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) aex_signf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_sign_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) aex_signf(opd)
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

void builtin_sign_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) aex_signf(opd)
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

void builtin_sign_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) aex_signf(opd)
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

void builtin_sign_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, aex_signf(opd.v_.f_));
}

void builtin_sign_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, aex_signf(opd.v_.v_[0]), aex_signf(opd.v_.v_[1]));
}

void builtin_sign_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, aex_signf(opd.v_.v_[0]), aex_signf(opd.v_.v_[1]), aex_signf(opd.v_.v_[2]));
}

void builtin_sign_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, aex_signf(opd.v_.v_[0]), aex_signf(opd.v_.v_[1]), aex_signf(opd.v_.v_[2]), aex_signf(opd.v_.v_[3]));
}

void builtin_floor_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) floorf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_floor_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) floorf(opd)
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

void builtin_floor_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) floorf(opd)
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

void builtin_floor_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) floorf(opd)
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

void builtin_floor_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, floorf(opd.v_.f_));
}

void builtin_floor_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, floorf(opd.v_.v_[0]), floorf(opd.v_.v_[1]));
}

void builtin_floor_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, floorf(opd.v_.v_[0]), floorf(opd.v_.v_[1]), floorf(opd.v_.v_[2]));
}

void builtin_floor_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, floorf(opd.v_.v_[0]), floorf(opd.v_.v_[1]), floorf(opd.v_.v_[2]), floorf(opd.v_.v_[3]));
}

void builtin_ceil_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) ceilf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_ceil_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) ceilf(opd)
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

void builtin_ceil_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) ceilf(opd)
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

void builtin_ceil_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) ceilf(opd)
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

void builtin_ceil_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, ceilf(opd.v_.f_));
}

void builtin_ceil_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, ceilf(opd.v_.v_[0]), ceilf(opd.v_.v_[1]));
}

void builtin_ceil_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, ceilf(opd.v_.v_[0]), ceilf(opd.v_.v_[1]), ceilf(opd.v_.v_[2]));
}

void builtin_ceil_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, ceilf(opd.v_.v_[0]), ceilf(opd.v_.v_[1]), ceilf(opd.v_.v_[2]), ceilf(opd.v_.v_[3]));
}

void builtin_fract_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) aex_fractf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_fract_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict result_column;
  float * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) aex_fractf(opd)
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

void builtin_fract_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) aex_fractf(opd)
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

void builtin_fract_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) aex_fractf(opd)
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

void builtin_fract_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, aex_fractf(opd.v_.f_));
}

void builtin_fract_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec2(r, aex_fractf(opd.v_.v_[0]), aex_fractf(opd.v_.v_[1]));
}

void builtin_fract_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec3(r, aex_fractf(opd.v_.v_[0]), aex_fractf(opd.v_.v_[1]), aex_fractf(opd.v_.v_[2]));
}

void builtin_fract_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_vec4(r, aex_fractf(opd.v_.v_[0]), aex_fractf(opd.v_.v_[1]), aex_fractf(opd.v_.v_[2]), aex_fractf(opd.v_.v_[3]));
}


void builtin_mod_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict left_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict right_column = FLOAT_REG_PTR(x->children_[1], 0);
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_modf(x, y)
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_mod_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_modf(x, y)
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

void builtin_mod_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_modf(x, y)
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

void builtin_mod_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_modf(x, y)
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

void builtin_mod_v2f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_modf(x, y)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_mod_v3f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_modf(x, y)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_mod_v4f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_modf(x, y)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 3);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_mod_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_float(r, aex_modf(opd0.v_.f_, opd1.v_.f_));
}

void builtin_mod_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec2(r, aex_modf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            aex_modf(opd0.v_.v_[1], opd1.v_.v_[1]));
}

void builtin_mod_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec3(r, aex_modf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            aex_modf(opd0.v_.v_[1], opd1.v_.v_[1]),
                            aex_modf(opd0.v_.v_[2], opd1.v_.v_[2]));
}

void builtin_mod_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec4(r, aex_modf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            aex_modf(opd0.v_.v_[1], opd1.v_.v_[1]),
                            aex_modf(opd0.v_.v_[2], opd1.v_.v_[2]),
                            aex_modf(opd0.v_.v_[3], opd1.v_.v_[3]));
}

void builtin_mod_v2f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec2(r, aex_modf(opd0.v_.v_[0], opd1.v_.f_),
                            aex_modf(opd0.v_.v_[1], opd1.v_.f_));
}

void builtin_mod_v3f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec3(r, aex_modf(opd0.v_.v_[0], opd1.v_.f_),
                            aex_modf(opd0.v_.v_[1], opd1.v_.f_),
                            aex_modf(opd0.v_.v_[2], opd1.v_.f_));
}

void builtin_mod_v4f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec4(r, aex_modf(opd0.v_.v_[0], opd1.v_.f_),
                            aex_modf(opd0.v_.v_[1], opd1.v_.f_),
                            aex_modf(opd0.v_.v_[2], opd1.v_.f_),
                            aex_modf(opd0.v_.v_[3], opd1.v_.f_));
}

void builtin_min_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict left_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict right_column = FLOAT_REG_PTR(x->children_[1], 0);
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fminf(x, y)
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_min_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fminf(x, y)
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

void builtin_min_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fminf(x, y)
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

void builtin_min_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fminf(x, y)
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

void builtin_min_v2f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fminf(x, y)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_min_v3f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fminf(x, y)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_min_v4f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fminf(x, y)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 3);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_min_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_float(r, fminf(opd0.v_.f_, opd1.v_.f_));
}

void builtin_min_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec2(r, fminf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            fminf(opd0.v_.v_[1], opd1.v_.v_[1]));
}

void builtin_min_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec3(r, fminf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            fminf(opd0.v_.v_[1], opd1.v_.v_[1]),
                            fminf(opd0.v_.v_[2], opd1.v_.v_[2]));
}

void builtin_min_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec4(r, fminf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            fminf(opd0.v_.v_[1], opd1.v_.v_[1]),
                            fminf(opd0.v_.v_[2], opd1.v_.v_[2]),
                            fminf(opd0.v_.v_[3], opd1.v_.v_[3]));
}

void builtin_min_v2f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec2(r, fminf(opd0.v_.v_[0], opd1.v_.f_),
                            fminf(opd0.v_.v_[1], opd1.v_.f_));
}

void builtin_min_v3f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec3(r, fminf(opd0.v_.v_[0], opd1.v_.f_),
                            fminf(opd0.v_.v_[1], opd1.v_.f_),
                            fminf(opd0.v_.v_[2], opd1.v_.f_));
}

void builtin_min_v4f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec4(r, fminf(opd0.v_.v_[0], opd1.v_.f_),
                            fminf(opd0.v_.v_[1], opd1.v_.f_),
                            fminf(opd0.v_.v_[2], opd1.v_.f_),
                            fminf(opd0.v_.v_[3], opd1.v_.f_));
}

void builtin_max_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict left_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict right_column = FLOAT_REG_PTR(x->children_[1], 0);
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fmaxf(x, y)
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_max_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fmaxf(x, y)
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

void builtin_max_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fmaxf(x, y)
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

void builtin_max_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fmaxf(x, y)
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

void builtin_max_v2f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fmaxf(x, y)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_max_v3f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fmaxf(x, y)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_max_v4f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fmaxf(x, y)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 3);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_max_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_float(r, fmaxf(opd0.v_.f_, opd1.v_.f_));
}

void builtin_max_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec2(r, fmaxf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            fmaxf(opd0.v_.v_[1], opd1.v_.v_[1]));
}

void builtin_max_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec3(r, fmaxf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            fmaxf(opd0.v_.v_[1], opd1.v_.v_[1]),
                            fmaxf(opd0.v_.v_[2], opd1.v_.v_[2]));
}

void builtin_max_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec4(r, fmaxf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            fmaxf(opd0.v_.v_[1], opd1.v_.v_[1]),
                            fmaxf(opd0.v_.v_[2], opd1.v_.v_[2]),
                            fmaxf(opd0.v_.v_[3], opd1.v_.v_[3]));
}

void builtin_max_v2f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec2(r, fmaxf(opd0.v_.v_[0], opd1.v_.f_),
                            fmaxf(opd0.v_.v_[1], opd1.v_.f_));
}

void builtin_max_v3f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec3(r, fmaxf(opd0.v_.v_[0], opd1.v_.f_),
                            fmaxf(opd0.v_.v_[1], opd1.v_.f_),
                            fmaxf(opd0.v_.v_[2], opd1.v_.f_));
}

void builtin_max_v4f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec4(r, fmaxf(opd0.v_.v_[0], opd1.v_.f_),
                            fmaxf(opd0.v_.v_[1], opd1.v_.f_),
                            fmaxf(opd0.v_.v_[2], opd1.v_.f_),
                            fmaxf(opd0.v_.v_[3], opd1.v_.f_));
}

void builtin_clamp_fff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict first_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict second_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict third_column = FLOAT_REG_PTR(x->children_[2], 0);
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_clampf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
#include "sl_ternop_snippet_inc.h"
#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_clamp_v2v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_clampf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 1);
  third_column = FLOAT_REG_PTR(x->children_[2], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_clamp_v3v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_clampf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 1);
  third_column = FLOAT_REG_PTR(x->children_[2], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 2);
  second_column = FLOAT_REG_PTR(x->children_[1], 2);
  third_column = FLOAT_REG_PTR(x->children_[2], 2);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_clamp_v4v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_clampf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 1);
  third_column = FLOAT_REG_PTR(x->children_[2], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 2);
  second_column = FLOAT_REG_PTR(x->children_[1], 2);
  third_column = FLOAT_REG_PTR(x->children_[2], 2);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 3);
  second_column = FLOAT_REG_PTR(x->children_[1], 3);
  third_column = FLOAT_REG_PTR(x->children_[2], 3);
  result_column = FLOAT_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_clamp_v2ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_clampf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_clamp_v3ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_clampf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 2);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_clamp_v4ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_clampf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 2);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 3);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_clamp_fff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_float(r, aex_clampf(opd0.v_.f_, opd1.v_.f_, opd2.v_.f_));
}

void builtin_clamp_v2v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec2(r, aex_clampf(opd0.v_.v_[0], opd1.v_.v_[0], opd2.v_.v_[0]),
                            aex_clampf(opd0.v_.v_[1], opd1.v_.v_[1], opd2.v_.v_[1]));
}

void builtin_clamp_v3v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec3(r, aex_clampf(opd0.v_.v_[0], opd1.v_.v_[0], opd2.v_.v_[0]),
                            aex_clampf(opd0.v_.v_[1], opd1.v_.v_[1], opd2.v_.v_[1]),
                            aex_clampf(opd0.v_.v_[2], opd1.v_.v_[2], opd2.v_.v_[2]));
}

void builtin_clamp_v4v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec4(r, aex_clampf(opd0.v_.v_[0], opd1.v_.v_[0], opd2.v_.v_[0]),
                            aex_clampf(opd0.v_.v_[1], opd1.v_.v_[1], opd2.v_.v_[1]),
                            aex_clampf(opd0.v_.v_[2], opd1.v_.v_[2], opd2.v_.v_[2]),
                            aex_clampf(opd0.v_.v_[3], opd1.v_.v_[3], opd2.v_.v_[3]));
}

void builtin_clamp_v2ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec2(r, aex_clampf(opd0.v_.v_[0], opd1.v_.f_, opd2.v_.f_),
                            aex_clampf(opd0.v_.v_[1], opd1.v_.f_, opd2.v_.f_));
}

void builtin_clamp_v3ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec3(r, aex_clampf(opd0.v_.v_[0], opd1.v_.f_, opd2.v_.f_),
                            aex_clampf(opd0.v_.v_[1], opd1.v_.f_, opd2.v_.f_),
                            aex_clampf(opd0.v_.v_[2], opd1.v_.f_, opd2.v_.f_));
}

void builtin_clamp_v4ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec4(r, aex_clampf(opd0.v_.v_[0], opd1.v_.f_, opd2.v_.f_),
                            aex_clampf(opd0.v_.v_[1], opd1.v_.f_, opd2.v_.f_),
                            aex_clampf(opd0.v_.v_[2], opd1.v_.f_, opd2.v_.f_),
                            aex_clampf(opd0.v_.v_[3], opd1.v_.f_, opd2.v_.f_));
}

void builtin_mix_fff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict first_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict second_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict third_column = FLOAT_REG_PTR(x->children_[2], 0);
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_mixf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
#include "sl_ternop_snippet_inc.h"
#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_mix_v2v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_mixf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 1);
  third_column = FLOAT_REG_PTR(x->children_[2], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_mix_v3v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_mixf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 1);
  third_column = FLOAT_REG_PTR(x->children_[2], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 2);
  second_column = FLOAT_REG_PTR(x->children_[1], 2);
  third_column = FLOAT_REG_PTR(x->children_[2], 2);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_mix_v4v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_mixf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 1);
  third_column = FLOAT_REG_PTR(x->children_[2], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 2);
  second_column = FLOAT_REG_PTR(x->children_[1], 2);
  third_column = FLOAT_REG_PTR(x->children_[2], 2);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 3);
  second_column = FLOAT_REG_PTR(x->children_[1], 3);
  third_column = FLOAT_REG_PTR(x->children_[2], 3);
  result_column = FLOAT_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_mix_v2v2f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_mixf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 1);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_mix_v3v3f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_mixf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 1);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 2);
  second_column = FLOAT_REG_PTR(x->children_[1], 2);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_mix_v4v4f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_mixf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 1);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 2);
  second_column = FLOAT_REG_PTR(x->children_[1], 2);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 3);
  second_column = FLOAT_REG_PTR(x->children_[1], 3);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_mix_fff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_float(r, aex_mixf(opd0.v_.f_, opd1.v_.f_, opd2.v_.f_));
}

void builtin_mix_v2v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec2(r, aex_mixf(opd0.v_.v_[0], opd1.v_.v_[0], opd2.v_.v_[0]),
                            aex_mixf(opd0.v_.v_[1], opd1.v_.v_[1], opd2.v_.v_[1]));
}

void builtin_mix_v3v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec3(r, aex_mixf(opd0.v_.v_[0], opd1.v_.v_[0], opd2.v_.v_[0]),
                            aex_mixf(opd0.v_.v_[1], opd1.v_.v_[1], opd2.v_.v_[1]),
                            aex_mixf(opd0.v_.v_[2], opd1.v_.v_[2], opd2.v_.v_[2]));
}

void builtin_mix_v4v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec4(r, aex_mixf(opd0.v_.v_[0], opd1.v_.v_[0], opd2.v_.v_[0]),
                            aex_mixf(opd0.v_.v_[1], opd1.v_.v_[1], opd2.v_.v_[1]),
                            aex_mixf(opd0.v_.v_[2], opd1.v_.v_[2], opd2.v_.v_[2]),
                            aex_mixf(opd0.v_.v_[3], opd1.v_.v_[3], opd2.v_.v_[3]));
}

void builtin_mix_v2v2f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec2(r, aex_mixf(opd0.v_.v_[0], opd1.v_.v_[0], opd2.v_.f_),
                            aex_mixf(opd0.v_.v_[1], opd1.v_.v_[1], opd2.v_.f_));
}

void builtin_mix_v3v3f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec3(r, aex_mixf(opd0.v_.v_[0], opd1.v_.v_[0], opd2.v_.f_),
                            aex_mixf(opd0.v_.v_[1], opd1.v_.v_[1], opd2.v_.f_),
                            aex_mixf(opd0.v_.v_[2], opd1.v_.v_[2], opd2.v_.f_));
}

void builtin_mix_v4v4f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec4(r, aex_mixf(opd0.v_.v_[0], opd1.v_.v_[0], opd2.v_.f_),
                            aex_mixf(opd0.v_.v_[1], opd1.v_.v_[0], opd2.v_.f_),
                            aex_mixf(opd0.v_.v_[2], opd1.v_.v_[0], opd2.v_.f_),
                            aex_mixf(opd0.v_.v_[3], opd1.v_.v_[0], opd2.v_.f_));
}

void builtin_step_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict left_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict right_column = FLOAT_REG_PTR(x->children_[1], 0);
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_stepf(x, y)
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_step_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_stepf(x, y)
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

void builtin_step_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_stepf(x, y)
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

void builtin_step_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_stepf(x, y)
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

void builtin_step_fv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_stepf(x, y)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_step_fv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_stepf(x, y)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_step_fv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_stepf(x, y)
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 3);
  result_column = FLOAT_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_step_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_float(r, aex_stepf(opd0.v_.f_, opd1.v_.f_));
}

void builtin_step_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec2(r, aex_stepf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            aex_stepf(opd0.v_.v_[1], opd1.v_.v_[1]));
}

void builtin_step_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec3(r, aex_stepf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            aex_stepf(opd0.v_.v_[1], opd1.v_.v_[1]),
                            aex_stepf(opd0.v_.v_[2], opd1.v_.v_[2]));
}

void builtin_step_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec4(r, aex_stepf(opd0.v_.v_[0], opd1.v_.v_[0]),
                            aex_stepf(opd0.v_.v_[1], opd1.v_.v_[1]),
                            aex_stepf(opd0.v_.v_[2], opd1.v_.v_[2]),
                            aex_stepf(opd0.v_.v_[3], opd1.v_.v_[3]));
}

void builtin_step_fv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec2(r, aex_stepf(opd0.v_.f_, opd1.v_.v_[0]),
                            aex_stepf(opd0.v_.f_, opd1.v_.v_[1]));
}

void builtin_step_fv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec3(r, aex_stepf(opd0.v_.f_, opd1.v_.v_[0]),
                            aex_stepf(opd0.v_.f_, opd1.v_.v_[1]),
                            aex_stepf(opd0.v_.f_, opd1.v_.v_[2]));
}

void builtin_step_fv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec4(r, aex_stepf(opd0.v_.f_, opd1.v_.v_[0]),
                            aex_stepf(opd0.v_.f_, opd1.v_.v_[1]),
                            aex_stepf(opd0.v_.f_, opd1.v_.v_[2]),
                            aex_stepf(opd0.v_.f_, opd1.v_.v_[3]));
}

void builtin_smoothstep_fff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict first_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict second_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict third_column = FLOAT_REG_PTR(x->children_[2], 0);
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_smoothstepf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
#include "sl_ternop_snippet_inc.h"
#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_smoothstep_v2v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_smoothstepf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 1);
  third_column = FLOAT_REG_PTR(x->children_[2], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_smoothstep_v3v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_smoothstepf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 1);
  third_column = FLOAT_REG_PTR(x->children_[2], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 2);
  second_column = FLOAT_REG_PTR(x->children_[1], 2);
  third_column = FLOAT_REG_PTR(x->children_[2], 2);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_smoothstep_v4v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_smoothstepf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 1);
  third_column = FLOAT_REG_PTR(x->children_[2], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 2);
  second_column = FLOAT_REG_PTR(x->children_[1], 2);
  third_column = FLOAT_REG_PTR(x->children_[2], 2);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 3);
  second_column = FLOAT_REG_PTR(x->children_[1], 3);
  third_column = FLOAT_REG_PTR(x->children_[2], 3);
  result_column = FLOAT_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_smoothstep_ffv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_smoothstepf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_smoothstep_ffv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_smoothstepf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 2);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_smoothstep_ffv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, a, b) aex_smoothstepf(x, a, b)
#define TERNOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 0);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 1);
  result_column = FLOAT_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 2);
  result_column = FLOAT_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[2], 3);
  result_column = FLOAT_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_ternop_snippet_inc.h"

#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_smoothstep_fff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_float(r, aex_smoothstepf(opd0.v_.f_, opd1.v_.f_, opd2.v_.f_));
}

void builtin_smoothstep_v2v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec2(r, aex_smoothstepf(opd0.v_.v_[0], opd1.v_.v_[0], opd2.v_.v_[0]),
                            aex_smoothstepf(opd0.v_.v_[1], opd1.v_.v_[1], opd2.v_.v_[1]));
}

void builtin_smoothstep_v3v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec3(r, aex_smoothstepf(opd0.v_.v_[0], opd1.v_.v_[0], opd2.v_.v_[0]),
                            aex_smoothstepf(opd0.v_.v_[1], opd1.v_.v_[1], opd2.v_.v_[1]),
                            aex_smoothstepf(opd0.v_.v_[2], opd1.v_.v_[2], opd2.v_.v_[2]));
}

void builtin_smoothstep_v4v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec4(r, aex_smoothstepf(opd0.v_.v_[0], opd1.v_.v_[0], opd2.v_.v_[0]),
                            aex_smoothstepf(opd0.v_.v_[1], opd1.v_.v_[1], opd2.v_.v_[1]),
                            aex_smoothstepf(opd0.v_.v_[2], opd1.v_.v_[2], opd2.v_.v_[2]),
                            aex_smoothstepf(opd0.v_.v_[3], opd1.v_.v_[3], opd2.v_.v_[3]));
}

void builtin_smoothstep_ffv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec2(r, aex_smoothstepf(opd0.v_.f_, opd1.v_.f_, opd2.v_.v_[0]),
                            aex_smoothstepf(opd0.v_.f_, opd1.v_.f_, opd2.v_.v_[1]));
}

void builtin_smoothstep_ffv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec3(r, aex_smoothstepf(opd0.v_.f_, opd1.v_.f_, opd2.v_.v_[0]),
                            aex_smoothstepf(opd0.v_.f_, opd1.v_.f_, opd2.v_.v_[1]),
                            aex_smoothstepf(opd0.v_.f_, opd1.v_.f_, opd2.v_.v_[2]));
}

void builtin_smoothstep_ffv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd0, opd1, opd2;
  sl_expr_temp_init(&opd0, NULL);
  sl_expr_temp_init(&opd1, NULL);
  sl_expr_temp_init(&opd2, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd0)) {
    sl_expr_temp_cleanup(&opd0);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opd1)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opd2)) {
    sl_expr_temp_cleanup(&opd0);
    sl_expr_temp_cleanup(&opd1);
    sl_expr_temp_cleanup(&opd2);
    return;
  }
  sl_expr_temp_init_vec4(r, aex_smoothstepf(opd0.v_.f_, opd1.v_.f_, opd2.v_.v_[0]),
                            aex_smoothstepf(opd0.v_.f_, opd1.v_.f_, opd2.v_.v_[1]),
                            aex_smoothstepf(opd0.v_.f_, opd1.v_.f_, opd2.v_.v_[2]),
                            aex_smoothstepf(opd0.v_.f_, opd1.v_.f_, opd2.v_.v_[3]));
}


/* Geometric Functions */

void builtin_length_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) fabsf(opd)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_length_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict left_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict right_column = FLOAT_REG_PTR(x->children_[0], 1);
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) sqrtf(x * x + y * y)
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_length_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict first_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict second_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict third_column = FLOAT_REG_PTR(x->children_[0], 2);
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(x, y, z) sqrtf(x * x + y * y + z * z)
#define TERNOP_SNIPPET_TYPE float
#include "sl_ternop_snippet_inc.h"
#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_length_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict first_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict second_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict third_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict fourth_column = FLOAT_REG_PTR(x->children_[0], 3);
  uint8_t row = exec_chain;

#define QUADOP_SNIPPET_OPERATOR(x, y, z, w) sqrtf(x * x + y * y + z * z + w * w)
#define QUADOP_SNIPPET_TYPE float
#include "sl_quadop_snippet_inc.h"
#undef QUADOP_SNIPPET_OPERATOR
#undef QUADOP_SNIPPET_TYPE
}

void builtin_length_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, fabsf(opd.v_.f_));
}

void builtin_length_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, sqrtf( opd.v_.v_[0] * opd.v_.v_[0] + opd.v_.v_[1] * opd.v_.v_[1]));
}

void builtin_length_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, sqrtf(opd.v_.v_[0] * opd.v_.v_[0] + opd.v_.v_[1] * opd.v_.v_[1] + opd.v_.v_[2] * opd.v_.v_[2]));
}

void builtin_length_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, sqrtf(opd.v_.v_[0] * opd.v_.v_[0] + opd.v_.v_[1] * opd.v_.v_[1] + opd.v_.v_[2] * opd.v_.v_[2] + opd.v_.v_[3] * opd.v_.v_[3]));
}

void builtin_distance_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict left_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict right_column = FLOAT_REG_PTR(x->children_[1], 0);
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) fabsf(x - y)
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_distance_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict first_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict second_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict third_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict fourth_column = FLOAT_REG_PTR(x->children_[1], 1);
  uint8_t row = exec_chain;

#define QUADOP_SNIPPET_OPERATOR(x0, y0, x1, y1) sqrtf((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1))
#define QUADOP_SNIPPET_TYPE float
#include "sl_quadop_snippet_inc.h"
#undef QUADOP_SNIPPET_OPERATOR
#undef QUADOP_SNIPPET_TYPE
}

void builtin_distance_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict first_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict second_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict third_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict fourth_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict fifth_column = FLOAT_REG_PTR(x->children_[1], 1);
  float *restrict sixth_column = FLOAT_REG_PTR(x->children_[1], 2);
  uint8_t row = exec_chain;

#define SENOP_SNIPPET_OPERATOR(x0, y0, z0, x1, y1, z1) sqrtf((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1) + (z0 - z1) * (z0 - z1))
#define SENOP_SNIPPET_TYPE float
#include "sl_senop_snippet_inc.h"
#undef SENOP_SNIPPET_OPERATOR
#undef SENOP_SNIPPET_TYPE
}

void builtin_distance_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict first_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict second_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict third_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict fourth_column = FLOAT_REG_PTR(x->children_[0], 3);
  float *restrict fifth_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict sixth_column = FLOAT_REG_PTR(x->children_[1], 1);
  float *restrict seventh_column = FLOAT_REG_PTR(x->children_[1], 2);
  float *restrict eighth_column = FLOAT_REG_PTR(x->children_[1], 3);
  uint8_t row = exec_chain;

#define OCTONOP_SNIPPET_OPERATOR(x0, y0, z0, w0, x1, y1, z1, w1) sqrtf((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1) + (z0 - z1) * (z0 - z1) + (w0 - w1) * (w0 - w1))
#define OCTONOP_SNIPPET_TYPE float
#include "sl_octonop_snippet_inc.h"
#undef OCTONOP_SNIPPET_OPERATOR
#undef OCTONOP_SNIPPET_TYPE
}

void builtin_distance_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_float(r, fabsf(opd0.v_.f_ - opd1.v_.f_));
}

void builtin_distance_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_float(r, sqrtf((opd0.v_.v_[0] - opd1.v_.v_[0]) * (opd0.v_.v_[0] - opd1.v_.v_[0]) 
                                 + (opd0.v_.v_[1] - opd1.v_.v_[1]) * (opd0.v_.v_[1] - opd1.v_.v_[1])));
}

void builtin_distance_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_float(r, sqrtf((opd0.v_.v_[0] - opd1.v_.v_[0]) * (opd0.v_.v_[0] - opd1.v_.v_[0]) 
                                 + (opd0.v_.v_[1] - opd1.v_.v_[1]) * (opd0.v_.v_[1] - opd1.v_.v_[1])
                                 + (opd0.v_.v_[2] - opd1.v_.v_[2]) * (opd0.v_.v_[2] - opd1.v_.v_[2])));
}

void builtin_distance_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_float(r, sqrtf((opd0.v_.v_[0] - opd1.v_.v_[0]) * (opd0.v_.v_[0] - opd1.v_.v_[0]) 
                                 + (opd0.v_.v_[1] - opd1.v_.v_[1]) * (opd0.v_.v_[1] - opd1.v_.v_[1])
                                 + (opd0.v_.v_[2] - opd1.v_.v_[2]) * (opd0.v_.v_[2] - opd1.v_.v_[2])
                                 + (opd0.v_.v_[3] - opd1.v_.v_[3]) * (opd0.v_.v_[3] - opd1.v_.v_[3])));
}

void builtin_dot_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict left_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict right_column = FLOAT_REG_PTR(x->children_[1], 0);
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) (x*y)
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void builtin_dot_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict first_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict second_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict third_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict fourth_column = FLOAT_REG_PTR(x->children_[1], 1);
  uint8_t row = exec_chain;

#define QUADOP_SNIPPET_OPERATOR(x0, y0, x1, y1) (x0*x1 + y0*y1)
#define QUADOP_SNIPPET_TYPE float
#include "sl_quadop_snippet_inc.h"
#undef QUADOP_SNIPPET_OPERATOR
#undef QUADOP_SNIPPET_TYPE
}

void builtin_dot_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict first_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict second_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict third_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict fourth_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict fifth_column = FLOAT_REG_PTR(x->children_[1], 1);
  float *restrict sixth_column = FLOAT_REG_PTR(x->children_[1], 2);
  uint8_t row = exec_chain;

#define SENOP_SNIPPET_OPERATOR(x0, y0, z0, x1, y1, z1) (x0*x1 + y0*y1 + z0*z1)
#define SENOP_SNIPPET_TYPE float
#include "sl_senop_snippet_inc.h"
#undef SENOP_SNIPPET_OPERATOR
#undef SENOP_SNIPPET_TYPE
}

void builtin_dot_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict first_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict second_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict third_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict fourth_column = FLOAT_REG_PTR(x->children_[0], 3);
  float *restrict fifth_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict sixth_column = FLOAT_REG_PTR(x->children_[1], 1);
  float *restrict seventh_column = FLOAT_REG_PTR(x->children_[1], 2);
  float *restrict eighth_column = FLOAT_REG_PTR(x->children_[1], 3);
  uint8_t row = exec_chain;

#define OCTONOP_SNIPPET_OPERATOR(x0, y0, z0, w0, x1, y1, z1, w1) (x0*x1 + y0*y1 + z0*z1 + w0*w1)
#define OCTONOP_SNIPPET_TYPE float
#include "sl_octonop_snippet_inc.h"
#undef OCTONOP_SNIPPET_OPERATOR
#undef OCTONOP_SNIPPET_TYPE
}

void builtin_dot_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_float(r, opd0.v_.f_ * opd1.v_.f_);
}

void builtin_dot_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_float(r, opd0.v_.v_[0] * opd1.v_.v_[0]
                           + opd0.v_.v_[1] * opd1.v_.v_[1]);
}

void builtin_dot_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_float(r, opd0.v_.v_[0] * opd1.v_.v_[0]
                           + opd0.v_.v_[1] * opd1.v_.v_[1]
                           + opd0.v_.v_[2] * opd1.v_.v_[2]);
}

void builtin_dot_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_float(r, opd0.v_.v_[0] * opd1.v_.v_[0]
                           + opd0.v_.v_[1] * opd1.v_.v_[1]
                           + opd0.v_.v_[2] * opd1.v_.v_[2]
                           + opd0.v_.v_[3] * opd1.v_.v_[3]);
}

void builtin_cross_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column;
  float *restrict first_column;
  float *restrict second_column;
  float *restrict third_column;
  float *restrict fourth_column;
  uint8_t row = exec_chain;

#define QUADOP_SNIPPET_OPERATOR(a, b, c, d) (a*b - c*d)
#define QUADOP_SNIPPET_TYPE float
  first_column = FLOAT_REG_PTR(x->children_[0], 1);
  second_column = FLOAT_REG_PTR(x->children_[1], 2);
  third_column = FLOAT_REG_PTR(x->children_[0], 2);
  fourth_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = FLOAT_REG_PTR(x, 0);

#include "sl_quadop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 2);
  second_column = FLOAT_REG_PTR(x->children_[1], 0);
  third_column = FLOAT_REG_PTR(x->children_[0], 0);
  fourth_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = FLOAT_REG_PTR(x, 1);

#include "sl_quadop_snippet_inc.h"

  first_column = FLOAT_REG_PTR(x->children_[0], 0);
  second_column = FLOAT_REG_PTR(x->children_[1], 1);
  third_column = FLOAT_REG_PTR(x->children_[0], 1);
  fourth_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = FLOAT_REG_PTR(x, 2);

#include "sl_quadop_snippet_inc.h"

#undef QUADOP_SNIPPET_OPERATOR
#undef QUADOP_SNIPPET_TYPE
}

void builtin_cross_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_vec3(r, opd0.v_.v_[1] * opd1.v_.v_[2] - opd0.v_.v_[2] * opd1.v_.v_[1],
                            opd0.v_.v_[2] * opd1.v_.v_[0] - opd0.v_.v_[0] * opd1.v_.v_[2],
                            opd0.v_.v_[0] * opd1.v_.v_[1] - opd0.v_.v_[1] * opd1.v_.v_[0]);
}

void builtin_normalize_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) (opd < 0.f) ? -1.f : 1.f
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_normalize_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_x_column = FLOAT_REG_PTR(x, 0);
  float *restrict result_y_column = FLOAT_REG_PTR(x, 1);
  float *restrict x_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict y_column = FLOAT_REG_PTR(x->children_[0], 1);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result_x = result_x_column + row;
        float *restrict result_y = result_y_column + row;
        const float *restrict x = x_column + row;
        const float *restrict y = y_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float len = sqrtf(x[n] * x[n] + y[n] * y[n]);
          result_x[n] = x[n] / len;
          result_y[n] = y[n] / len;
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result_x = result_x_column + row;
        float *restrict result_y = result_y_column + row;
        const float *restrict x = x_column + row;
        const float *restrict y = y_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float len = sqrtf(x[n] * x[n] + y[n] * y[n]);
          result_x[n] = x[n] / len;
          result_y[n] = y[n] / len;
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float len = sqrtf(x_column[row] * x_column[row] + y_column[row] * y_column[row]);
        result_x_column[row] = x_column[row] / len;
        result_y_column[row] = y_column[row] / len;
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_normalize_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_x_column = FLOAT_REG_PTR(x, 0);
  float *restrict result_y_column = FLOAT_REG_PTR(x, 1);
  float *restrict result_z_column = FLOAT_REG_PTR(x, 2);
  float *restrict x_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict y_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict z_column = FLOAT_REG_PTR(x->children_[0], 2);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result_x = result_x_column + row;
        float *restrict result_y = result_y_column + row;
        float *restrict result_z = result_z_column + row;
        const float *restrict x = x_column + row;
        const float *restrict y = y_column + row;
        const float *restrict z = z_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float len = sqrtf(x[n] * x[n] + y[n] * y[n] + z[n] * z[n]);
          result_x[n] = x[n] / len;
          result_y[n] = y[n] / len;
          result_z[n] = z[n] / len;
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result_x = result_x_column + row;
        float *restrict result_y = result_y_column + row;
        float *restrict result_z = result_z_column + row;
        const float *restrict x = x_column + row;
        const float *restrict y = y_column + row;
        const float *restrict z = z_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float len = sqrtf(x[n] * x[n] + y[n] * y[n] + z[n] * z[n]);
          result_x[n] = x[n] / len;
          result_y[n] = y[n] / len;
          result_z[n] = z[n] / len;
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float len = sqrtf(x_column[row] * x_column[row] + y_column[row] * y_column[row] + z_column[row] * z_column[row]);
        result_x_column[row] = x_column[row] / len;
        result_y_column[row] = y_column[row] / len;
        result_z_column[row] = z_column[row] / len;
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_normalize_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_x_column = FLOAT_REG_PTR(x, 0);
  float *restrict result_y_column = FLOAT_REG_PTR(x, 1);
  float *restrict result_z_column = FLOAT_REG_PTR(x, 2);
  float *restrict result_w_column = FLOAT_REG_PTR(x, 3);
  float *restrict x_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict y_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict z_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict w_column = FLOAT_REG_PTR(x->children_[0], 3);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result_x = result_x_column + row;
        float *restrict result_y = result_y_column + row;
        float *restrict result_z = result_z_column + row;
        float *restrict result_w = result_w_column + row;
        const float *restrict x = x_column + row;
        const float *restrict y = y_column + row;
        const float *restrict z = z_column + row;
        const float *restrict w = w_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float len = sqrtf(x[n] * x[n] + y[n] * y[n] + z[n] * z[n] + w[n] * w[n]);
          result_x[n] = x[n] / len;
          result_y[n] = y[n] / len;
          result_z[n] = z[n] / len;
          result_w[n] = w[n] / len;
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result_x = result_x_column + row;
        float *restrict result_y = result_y_column + row;
        float *restrict result_z = result_z_column + row;
        float *restrict result_w = result_w_column + row;
        const float *restrict x = x_column + row;
        const float *restrict y = y_column + row;
        const float *restrict z = z_column + row;
        const float *restrict w = w_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float len = sqrtf(x[n] * x[n] + y[n] * y[n] + z[n] * z[n] + w[n] * w[n]);
          result_x[n] = x[n] / len;
          result_y[n] = y[n] / len;
          result_z[n] = z[n] / len;
          result_w[n] = w[n] / len;
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float len = sqrtf(x_column[row] * x_column[row] + y_column[row] * y_column[row] + z_column[row] * z_column[row] + w_column[row] * w_column[row]);
        result_x_column[row] = x_column[row] / len;
        result_y_column[row] = y_column[row] / len;
        result_z_column[row] = z_column[row] / len;
        result_w_column[row] = w_column[row] / len;
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_normalize_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_float(r, (opd.v_.f_ < 0.f) ? -1.f : 1.f);
}

void builtin_normalize_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  float len = sqrtf(opd.v_.v_[0] * opd.v_.v_[0] + opd.v_.v_[1] * opd.v_.v_[1]);
  sl_expr_temp_init_vec2(r, opd.v_.v_[0] / len, opd.v_.v_[1] / len);
}

void builtin_normalize_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  float len = sqrtf(opd.v_.v_[0] * opd.v_.v_[0] + opd.v_.v_[1] * opd.v_.v_[1] + opd.v_.v_[2] * opd.v_.v_[2]);
  sl_expr_temp_init_vec3(r, opd.v_.v_[0] / len, opd.v_.v_[1] / len, opd.v_.v_[2] / len);
}

void builtin_normalize_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  float len = sqrtf(opd.v_.v_[0] * opd.v_.v_[0] + opd.v_.v_[1] * opd.v_.v_[1] + opd.v_.v_[2] * opd.v_.v_[2] + opd.v_.v_[3] * opd.v_.v_[3]);
  sl_expr_temp_init_vec4(r, opd.v_.v_[0] / len, opd.v_.v_[1] / len, opd.v_.v_[2] / len, opd.v_.v_[3] / len);
}

void builtin_faceforward_fff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict N_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict I_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict Nref_column = FLOAT_REG_PTR(x->children_[2], 0);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict N = N_column + row;
        const float *restrict I = I_column + row;
        const float *restrict Nref = Nref_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float dot_nref_i = Nref[n] * I[n];
          result[n] = (dot_nref_i < 0) ? N[n] : -N[n];
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict N = N_column + row;
        const float *restrict I = I_column + row;
        const float *restrict Nref = Nref_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float dot_nref_i = Nref[n] * I[n];
          result[n] = (dot_nref_i < 0) ? N[n] : -N[n];
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float dot_nref_i = Nref_column[row] * I_column[row];
        result_column[row] = (dot_nref_i < 0) ? N_column[row] : -N_column[row];
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_faceforward_v2v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict resultx_column = FLOAT_REG_PTR(x, 0);
  float *restrict resulty_column = FLOAT_REG_PTR(x, 1);
  float *restrict Nx_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict Ny_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict Ix_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict Iy_column = FLOAT_REG_PTR(x->children_[1], 1);
  float *restrict Nrefx_column = FLOAT_REG_PTR(x->children_[2], 0);
  float *restrict Nrefy_column = FLOAT_REG_PTR(x->children_[2], 1);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result_x = resultx_column + row;
        float *restrict result_y = resulty_column + row;
        const float *restrict N_x = Nx_column + row;
        const float *restrict N_y = Ny_column + row;
        const float *restrict I_x = Ix_column + row;
        const float *restrict I_y = Iy_column + row;
        const float *restrict Nref_x = Nrefx_column + row;
        const float *restrict Nref_y = Nrefy_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float dot_nref_i = Nref_x[n] * I_x[n] + Nref_y[n] * I_y[n];
          result_x[n] = (dot_nref_i < 0) ? N_x[n] : -N_x[n];
          result_y[n] = (dot_nref_i < 0) ? N_y[n] : -N_y[n];
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result_x = resultx_column + row;
        float *restrict result_y = resulty_column + row;
        const float *restrict N_x = Nx_column + row;
        const float *restrict N_y = Ny_column + row;
        const float *restrict I_x = Ix_column + row;
        const float *restrict I_y = Iy_column + row;
        const float *restrict Nref_x = Nrefx_column + row;
        const float *restrict Nref_y = Nrefy_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float dot_nref_i = Nref_x[n] * I_x[n] + Nref_y[n] * I_y[n];
          result_x[n] = (dot_nref_i < 0) ? N_x[n] : -N_x[n];
          result_y[n] = (dot_nref_i < 0) ? N_y[n] : -N_y[n];
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float dot_nref_i = Nrefx_column[row] * Ix_column[row] + Nrefy_column[row] * Iy_column[row];
        resultx_column[row] = (dot_nref_i < 0) ? Nx_column[row] : -Nx_column[row];
        resulty_column[row] = (dot_nref_i < 0) ? Ny_column[row] : -Ny_column[row];
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_faceforward_v3v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict resultx_column = FLOAT_REG_PTR(x, 0);
  float *restrict resulty_column = FLOAT_REG_PTR(x, 1);
  float *restrict resultz_column = FLOAT_REG_PTR(x, 2);
  float *restrict Nx_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict Ny_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict Nz_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict Ix_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict Iy_column = FLOAT_REG_PTR(x->children_[1], 1);
  float *restrict Iz_column = FLOAT_REG_PTR(x->children_[1], 2);
  float *restrict Nrefx_column = FLOAT_REG_PTR(x->children_[2], 0);
  float *restrict Nrefy_column = FLOAT_REG_PTR(x->children_[2], 1);
  float *restrict Nrefz_column = FLOAT_REG_PTR(x->children_[2], 2);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result_x = resultx_column + row;
        float *restrict result_y = resulty_column + row;
        float *restrict result_z = resultz_column + row;
        const float *restrict N_x = Nx_column + row;
        const float *restrict N_y = Ny_column + row;
        const float *restrict N_z = Nz_column + row;
        const float *restrict I_x = Ix_column + row;
        const float *restrict I_y = Iy_column + row;
        const float *restrict I_z = Iz_column + row;
        const float *restrict Nref_x = Nrefx_column + row;
        const float *restrict Nref_y = Nrefy_column + row;
        const float *restrict Nref_z = Nrefz_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float dot_nref_i = Nref_x[n] * I_x[n] + Nref_y[n] * I_y[n] + Nref_z[n] * I_z[n];
          result_x[n] = (dot_nref_i < 0) ? N_x[n] : -N_x[n];
          result_y[n] = (dot_nref_i < 0) ? N_y[n] : -N_y[n];
          result_z[n] = (dot_nref_i < 0) ? N_z[n] : -N_z[n];
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result_x = resultx_column + row;
        float *restrict result_y = resulty_column + row;
        float *restrict result_z = resultz_column + row;
        const float *restrict N_x = Nx_column + row;
        const float *restrict N_y = Ny_column + row;
        const float *restrict N_z = Nz_column + row;
        const float *restrict I_x = Ix_column + row;
        const float *restrict I_y = Iy_column + row;
        const float *restrict I_z = Iz_column + row;
        const float *restrict Nref_x = Nrefx_column + row;
        const float *restrict Nref_y = Nrefy_column + row;
        const float *restrict Nref_z = Nrefz_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float dot_nref_i = Nref_x[n] * I_x[n] + Nref_y[n] * I_y[n] + Nref_z[n] * I_z[n];
          result_x[n] = (dot_nref_i < 0) ? N_x[n] : -N_x[n];
          result_y[n] = (dot_nref_i < 0) ? N_y[n] : -N_y[n];
          result_z[n] = (dot_nref_i < 0) ? N_z[n] : -N_z[n];
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float dot_nref_i = Nrefx_column[row] * Ix_column[row] + Nrefy_column[row] * Iy_column[row] + Nrefz_column[row] * Iz_column[row];
        resultx_column[row] = (dot_nref_i < 0) ? Nx_column[row] : -Nx_column[row];
        resulty_column[row] = (dot_nref_i < 0) ? Ny_column[row] : -Ny_column[row];
        resultz_column[row] = (dot_nref_i < 0) ? Nz_column[row] : -Nz_column[row];
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_faceforward_v4v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict resultx_column = FLOAT_REG_PTR(x, 0);
  float *restrict resulty_column = FLOAT_REG_PTR(x, 1);
  float *restrict resultz_column = FLOAT_REG_PTR(x, 2);
  float *restrict resultw_column = FLOAT_REG_PTR(x, 3);
  float *restrict Nx_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict Ny_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict Nz_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict Nw_column = FLOAT_REG_PTR(x->children_[0], 3);
  float *restrict Ix_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict Iy_column = FLOAT_REG_PTR(x->children_[1], 1);
  float *restrict Iz_column = FLOAT_REG_PTR(x->children_[1], 2);
  float *restrict Iw_column = FLOAT_REG_PTR(x->children_[1], 3);
  float *restrict Nrefx_column = FLOAT_REG_PTR(x->children_[2], 0);
  float *restrict Nrefy_column = FLOAT_REG_PTR(x->children_[2], 1);
  float *restrict Nrefz_column = FLOAT_REG_PTR(x->children_[2], 2);
  float *restrict Nrefw_column = FLOAT_REG_PTR(x->children_[2], 3);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result_x = resultx_column + row;
        float *restrict result_y = resulty_column + row;
        float *restrict result_z = resultz_column + row;
        float *restrict result_w = resultz_column + row;
        const float *restrict N_x = Nx_column + row;
        const float *restrict N_y = Ny_column + row;
        const float *restrict N_z = Nz_column + row;
        const float *restrict N_w = Nz_column + row;
        const float *restrict I_x = Ix_column + row;
        const float *restrict I_y = Iy_column + row;
        const float *restrict I_z = Iz_column + row;
        const float *restrict I_w = Iz_column + row;
        const float *restrict Nref_x = Nrefx_column + row;
        const float *restrict Nref_y = Nrefy_column + row;
        const float *restrict Nref_z = Nrefz_column + row;
        const float *restrict Nref_w = Nrefz_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float dot_nref_i = Nref_x[n] * I_x[n] + Nref_y[n] * I_y[n] + Nref_z[n] * I_z[n] + Nref_w[n] * I_w[n];
          result_x[n] = (dot_nref_i < 0) ? N_x[n] : -N_x[n];
          result_y[n] = (dot_nref_i < 0) ? N_y[n] : -N_y[n];
          result_z[n] = (dot_nref_i < 0) ? N_z[n] : -N_z[n];
          result_w[n] = (dot_nref_i < 0) ? N_w[n] : -N_w[n];
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result_x = resultx_column + row;
        float *restrict result_y = resulty_column + row;
        float *restrict result_z = resultz_column + row;
        float *restrict result_w = resultz_column + row;
        const float *restrict N_x = Nx_column + row;
        const float *restrict N_y = Ny_column + row;
        const float *restrict N_z = Nz_column + row;
        const float *restrict N_w = Nz_column + row;
        const float *restrict I_x = Ix_column + row;
        const float *restrict I_y = Iy_column + row;
        const float *restrict I_z = Iz_column + row;
        const float *restrict I_w = Iz_column + row;
        const float *restrict Nref_x = Nrefx_column + row;
        const float *restrict Nref_y = Nrefy_column + row;
        const float *restrict Nref_z = Nrefz_column + row;
        const float *restrict Nref_w = Nrefz_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float dot_nref_i = Nref_x[n] * I_x[n] + Nref_y[n] * I_y[n] + Nref_z[n] * I_z[n] + Nref_w[n] * I_w[n];
          result_x[n] = (dot_nref_i < 0) ? N_x[n] : -N_x[n];
          result_y[n] = (dot_nref_i < 0) ? N_y[n] : -N_y[n];
          result_z[n] = (dot_nref_i < 0) ? N_z[n] : -N_z[n];
          result_w[n] = (dot_nref_i < 0) ? N_w[n] : -N_w[n];
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float dot_nref_i = Nrefx_column[row] * Ix_column[row] + Nrefy_column[row] * Iy_column[row] + Nrefz_column[row] * Iz_column[row] + Nrefw_column[row] * Iw_column[row];
        resultx_column[row] = (dot_nref_i < 0) ? Nx_column[row] : -Nx_column[row];
        resulty_column[row] = (dot_nref_i < 0) ? Ny_column[row] : -Ny_column[row];
        resultz_column[row] = (dot_nref_i < 0) ? Nz_column[row] : -Nz_column[row];
        resultw_column[row] = (dot_nref_i < 0) ? Nw_column[row] : -Nw_column[row];
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_faceforward_fff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opdN, opdI, opdNref;
  sl_expr_temp_init(&opdN, NULL);
  sl_expr_temp_init(&opdI, NULL);
  sl_expr_temp_init(&opdNref, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opdN)) {
    sl_expr_temp_cleanup(&opdN);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opdI)) {
    sl_expr_temp_cleanup(&opdN);
    sl_expr_temp_cleanup(&opdI);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opdNref)) {
    sl_expr_temp_cleanup(&opdN);
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdNref);
    return;
  }
  float dot_nref_i =  opdNref.v_.f_ * opdI.v_.f_;
  
  sl_expr_temp_init_float(r, (dot_nref_i < 0) ? opdN.v_.f_ : -opdN.v_.f_);
}

void builtin_faceforward_v2v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opdN, opdI, opdNref;
  sl_expr_temp_init(&opdN, NULL);
  sl_expr_temp_init(&opdI, NULL);
  sl_expr_temp_init(&opdNref, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opdN)) {
    sl_expr_temp_cleanup(&opdN);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opdI)) {
    sl_expr_temp_cleanup(&opdN);
    sl_expr_temp_cleanup(&opdI);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opdNref)) {
    sl_expr_temp_cleanup(&opdN);
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdNref);
    return;
  }
  float dot_nref_i = opdNref.v_.v_[0] * opdI.v_.v_[0] + opdNref.v_.v_[1] * opdI.v_.v_[1];

  sl_expr_temp_init_vec2(r, (dot_nref_i < 0) ? opdN.v_.v_[0] : -opdN.v_.v_[0],
                            (dot_nref_i < 0) ? opdN.v_.v_[1] : -opdN.v_.v_[1]);
}

void builtin_faceforward_v3v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opdN, opdI, opdNref;
  sl_expr_temp_init(&opdN, NULL);
  sl_expr_temp_init(&opdI, NULL);
  sl_expr_temp_init(&opdNref, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opdN)) {
    sl_expr_temp_cleanup(&opdN);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opdI)) {
    sl_expr_temp_cleanup(&opdN);
    sl_expr_temp_cleanup(&opdI);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opdNref)) {
    sl_expr_temp_cleanup(&opdN);
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdNref);
    return;
  }
  float dot_nref_i = opdNref.v_.v_[0] * opdI.v_.v_[0] + opdNref.v_.v_[1] * opdI.v_.v_[1] + opdNref.v_.v_[2] * opdI.v_.v_[2];

  sl_expr_temp_init_vec3(r, (dot_nref_i < 0) ? opdN.v_.v_[0] : -opdN.v_.v_[0],
                            (dot_nref_i < 0) ? opdN.v_.v_[1] : -opdN.v_.v_[1],
                            (dot_nref_i < 0) ? opdN.v_.v_[2] : -opdN.v_.v_[2]);
}

void builtin_faceforward_v4v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opdN, opdI, opdNref;
  sl_expr_temp_init(&opdN, NULL);
  sl_expr_temp_init(&opdI, NULL);
  sl_expr_temp_init(&opdNref, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opdN)) {
    sl_expr_temp_cleanup(&opdN);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opdI)) {
    sl_expr_temp_cleanup(&opdN);
    sl_expr_temp_cleanup(&opdI);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opdNref)) {
    sl_expr_temp_cleanup(&opdN);
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdNref);
    return;
  }
  float dot_nref_i = opdNref.v_.v_[0] * opdI.v_.v_[0] + opdNref.v_.v_[1] * opdI.v_.v_[1] + opdNref.v_.v_[2] * opdI.v_.v_[2] + opdNref.v_.v_[3] * opdI.v_.v_[3];

  sl_expr_temp_init_vec4(r, (dot_nref_i < 0) ? opdN.v_.v_[0] : -opdN.v_.v_[0],
                            (dot_nref_i < 0) ? opdN.v_.v_[1] : -opdN.v_.v_[1],
                            (dot_nref_i < 0) ? opdN.v_.v_[2] : -opdN.v_.v_[2],
                            (dot_nref_i < 0) ? opdN.v_.v_[3] : -opdN.v_.v_[3]);
}

void builtin_reflect_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict I_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict N_column = FLOAT_REG_PTR(x->children_[1], 0);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict I = I_column + row;
        const float *restrict N = N_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float two_dot_n_i = 2.f * N[n] * I[n];
          result[n] = I[n] - two_dot_n_i * N[n];
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict I = I_column + row;
        const float *restrict N = N_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float two_dot_n_i = 2.f * N[n] * I[n];
          result[n] = I[n] - two_dot_n_i * N[n];
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float two_dot_n_i = 2.f * N_column[row] * I_column[row];
        result_column[row] = I_column[row] - two_dot_n_i * N_column[row];
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_reflect_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict resultx_column = FLOAT_REG_PTR(x, 0);
  float *restrict resulty_column = FLOAT_REG_PTR(x, 1);
  float *restrict Ix_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict Iy_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict Nx_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict Ny_column = FLOAT_REG_PTR(x->children_[1], 1);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict resultx = resultx_column + row;
        float *restrict resulty = resulty_column + row;
        const float *restrict Ix = Ix_column + row;
        const float *restrict Iy = Iy_column + row;
        const float *restrict Nx = Nx_column + row;
        const float *restrict Ny = Ny_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float two_dot_n_i = 2.f * (Nx[n] * Ix[n] + Ny[n] * Iy[n]);
          resultx[n] = Ix[n] - two_dot_n_i * Nx[n];
          resulty[n] = Iy[n] - two_dot_n_i * Ny[n];
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict resultx = resultx_column + row;
        float *restrict resulty = resulty_column + row;
        const float *restrict Ix = Ix_column + row;
        const float *restrict Iy = Iy_column + row;
        const float *restrict Nx = Nx_column + row;
        const float *restrict Ny = Ny_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float two_dot_n_i = 2.f * (Nx[n] * Ix[n] + Ny[n] * Iy[n]);
          resultx[n] = Ix[n] - two_dot_n_i * Nx[n];
          resulty[n] = Iy[n] - two_dot_n_i * Ny[n];
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float two_dot_n_i = 2.f * (Nx_column[row] * Ix_column[row] + Ny_column[row] * Iy_column[row]);
        resultx_column[row] = Ix_column[row] - two_dot_n_i * Nx_column[row];
        resulty_column[row] = Iy_column[row] - two_dot_n_i * Ny_column[row];
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_reflect_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict resultx_column = FLOAT_REG_PTR(x, 0);
  float *restrict resulty_column = FLOAT_REG_PTR(x, 1);
  float *restrict resultz_column = FLOAT_REG_PTR(x, 2);
  float *restrict Ix_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict Iy_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict Iz_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict Nx_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict Ny_column = FLOAT_REG_PTR(x->children_[1], 1);
  float *restrict Nz_column = FLOAT_REG_PTR(x->children_[1], 2);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict resultx = resultx_column + row;
        float *restrict resulty = resulty_column + row;
        float *restrict resultz = resultz_column + row;
        const float *restrict Ix = Ix_column + row;
        const float *restrict Iy = Iy_column + row;
        const float *restrict Iz = Iz_column + row;
        const float *restrict Nx = Nx_column + row;
        const float *restrict Ny = Ny_column + row;
        const float *restrict Nz = Nz_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float two_dot_n_i = 2.f * (Nx[n] * Ix[n] + Ny[n] * Iy[n] + Nz[n] * Iz[n]);
          resultx[n] = Ix[n] - two_dot_n_i * Nx[n];
          resulty[n] = Iy[n] - two_dot_n_i * Ny[n];
          resultz[n] = Iz[n] - two_dot_n_i * Nz[n];
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict resultx = resultx_column + row;
        float *restrict resulty = resulty_column + row;
        float *restrict resultz = resultz_column + row;
        const float *restrict Ix = Ix_column + row;
        const float *restrict Iy = Iy_column + row;
        const float *restrict Iz = Iz_column + row;
        const float *restrict Nx = Nx_column + row;
        const float *restrict Ny = Ny_column + row;
        const float *restrict Nz = Nz_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float two_dot_n_i = 2.f * (Nx[n] * Ix[n] + Ny[n] * Iy[n] + Nz[n] * Iz[n]);
          resultx[n] = Ix[n] - two_dot_n_i * Nx[n];
          resulty[n] = Iy[n] - two_dot_n_i * Ny[n];
          resultz[n] = Iz[n] - two_dot_n_i * Nz[n];
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float two_dot_n_i = 2.f * (Nx_column[row] * Ix_column[row] + Ny_column[row] * Iy_column[row] + Nz_column[row] * Iz_column[row]);
        resultx_column[row] = Ix_column[row] - two_dot_n_i * Nx_column[row];
        resulty_column[row] = Iy_column[row] - two_dot_n_i * Ny_column[row];
        resultz_column[row] = Iz_column[row] - two_dot_n_i * Nz_column[row];
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_reflect_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict resultx_column = FLOAT_REG_PTR(x, 0);
  float *restrict resulty_column = FLOAT_REG_PTR(x, 1);
  float *restrict resultz_column = FLOAT_REG_PTR(x, 2);
  float *restrict resultw_column = FLOAT_REG_PTR(x, 3);
  float *restrict Ix_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict Iy_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict Iz_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict Iw_column = FLOAT_REG_PTR(x->children_[0], 3);
  float *restrict Nx_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict Ny_column = FLOAT_REG_PTR(x->children_[1], 1);
  float *restrict Nz_column = FLOAT_REG_PTR(x->children_[1], 2);
  float *restrict Nw_column = FLOAT_REG_PTR(x->children_[1], 3);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict resultx = resultx_column + row;
        float *restrict resulty = resulty_column + row;
        float *restrict resultz = resultz_column + row;
        float *restrict resultw = resultw_column + row;
        const float *restrict Ix = Ix_column + row;
        const float *restrict Iy = Iy_column + row;
        const float *restrict Iz = Iz_column + row;
        const float *restrict Iw = Iw_column + row;
        const float *restrict Nx = Nx_column + row;
        const float *restrict Ny = Ny_column + row;
        const float *restrict Nz = Nz_column + row;
        const float *restrict Nw = Nw_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float two_dot_n_i = 2.f * (Nx[n] * Ix[n] + Ny[n] * Iy[n] + Nz[n] * Iz[n] + Nw[n] * Iw[n]);
          resultx[n] = Ix[n] - two_dot_n_i * Nx[n];
          resulty[n] = Iy[n] - two_dot_n_i * Ny[n];
          resultz[n] = Iz[n] - two_dot_n_i * Nz[n];
          resultw[n] = Iw[n] - two_dot_n_i * Nw[n];
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict resultx = resultx_column + row;
        float *restrict resulty = resulty_column + row;
        float *restrict resultz = resultz_column + row;
        float *restrict resultw = resultw_column + row;
        const float *restrict Ix = Ix_column + row;
        const float *restrict Iy = Iy_column + row;
        const float *restrict Iz = Iz_column + row;
        const float *restrict Iw = Iw_column + row;
        const float *restrict Nx = Nx_column + row;
        const float *restrict Ny = Ny_column + row;
        const float *restrict Nz = Nz_column + row;
        const float *restrict Nw = Nw_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float two_dot_n_i = 2.f * (Nx[n] * Ix[n] + Ny[n] * Iy[n] + Nz[n] * Iz[n] + Nw[n] * Iw[n]);
          resultx[n] = Ix[n] - two_dot_n_i * Nx[n];
          resulty[n] = Iy[n] - two_dot_n_i * Ny[n];
          resultz[n] = Iz[n] - two_dot_n_i * Nz[n];
          resultw[n] = Iw[n] - two_dot_n_i * Nw[n];
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float two_dot_n_i = 2.f * (Nx_column[row] * Ix_column[row] + Ny_column[row] * Iy_column[row] + Nz_column[row] * Iz_column[row] + Nw_column[row] * Iw_column[row]);
        resultx_column[row] = Ix_column[row] - two_dot_n_i * Nx_column[row];
        resulty_column[row] = Iy_column[row] - two_dot_n_i * Ny_column[row];
        resultz_column[row] = Iz_column[row] - two_dot_n_i * Nz_column[row];
        resultw_column[row] = Iw_column[row] - two_dot_n_i * Nw_column[row];
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_reflect_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opdI, opdN;
  sl_expr_temp_init(&opdI, NULL);
  sl_expr_temp_init(&opdN, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opdI)) {
    sl_expr_temp_cleanup(&opdI);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opdN)) {
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdN);
    return;
  }
  float two_dot_n_i = 2.f * opdN.v_.f_ * opdI.v_.f_;
  
  sl_expr_temp_init_float(r, opdI.v_.f_ - two_dot_n_i * opdN.v_.f_);
}

void builtin_reflect_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opdI, opdN;
  sl_expr_temp_init(&opdI, NULL);
  sl_expr_temp_init(&opdN, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opdI)) {
    sl_expr_temp_cleanup(&opdI);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opdN)) {
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdN);
    return;
  }
  float two_dot_n_i = 2.f * (opdN.v_.v_[0] * opdI.v_.v_[0] + opdN.v_.v_[1] * opdI.v_.v_[1]);

  sl_expr_temp_init_vec2(r, opdI.v_.v_[0] - two_dot_n_i * opdN.v_.v_[0],
                            opdI.v_.v_[1] - two_dot_n_i * opdN.v_.v_[1]);
}

void builtin_reflect_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opdI, opdN;
  sl_expr_temp_init(&opdI, NULL);
  sl_expr_temp_init(&opdN, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opdI)) {
    sl_expr_temp_cleanup(&opdI);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opdN)) {
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdN);
    return;
  }
  float two_dot_n_i = 2.f * (opdN.v_.v_[0] * opdI.v_.v_[0] + opdN.v_.v_[1] * opdI.v_.v_[1] + opdN.v_.v_[2] * opdI.v_.v_[2]);

  sl_expr_temp_init_vec3(r, opdI.v_.v_[0] - two_dot_n_i * opdN.v_.v_[0],
                            opdI.v_.v_[1] - two_dot_n_i * opdN.v_.v_[1],
                            opdI.v_.v_[2] - two_dot_n_i * opdN.v_.v_[2]);
}

void builtin_reflect_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opdI, opdN;
  sl_expr_temp_init(&opdI, NULL);
  sl_expr_temp_init(&opdN, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opdI)) {
    sl_expr_temp_cleanup(&opdI);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opdN)) {
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdN);
    return;
  }
  float two_dot_n_i = 2.f * (opdN.v_.v_[0] * opdI.v_.v_[0] + opdN.v_.v_[1] * opdI.v_.v_[1] + opdN.v_.v_[2] * opdI.v_.v_[2] + opdN.v_.v_[3] * opdI.v_.v_[3]);

  sl_expr_temp_init_vec4(r, opdI.v_.v_[0] - two_dot_n_i * opdN.v_.v_[0],
                            opdI.v_.v_[1] - two_dot_n_i * opdN.v_.v_[1],
                            opdI.v_.v_[2] - two_dot_n_i * opdN.v_.v_[2],
                            opdI.v_.v_[3] - two_dot_n_i * opdN.v_.v_[3]);
}

void builtin_refract_fff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict I_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict N_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict eta_column = FLOAT_REG_PTR(x->children_[2], 0);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict N = N_column + row;
        const float *restrict I = I_column + row;
        const float *restrict eta = eta_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float dot_n_i = N[n] * I[n];
          float k = 1.f - eta[n] * eta[n] * (1.f - dot_n_i * dot_n_i);
          if (k < 0.f) {
            result[n] = 0.f;
          }
          else {
            result[n] = eta[n] * I[n] - (eta[n] * dot_n_i + sqrtf(k)) * N[n];
          }
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict N = N_column + row;
        const float *restrict I = I_column + row;
        const float *restrict eta = eta_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float dot_n_i = N[n] * I[n];
          float k = 1.f - eta[n] * eta[n] * (1.f - dot_n_i * dot_n_i);
          if (k < 0.f) {
            result[n] = 0.f;
          }
          else {
            result[n] = eta[n] * I[n] - (eta[n] * dot_n_i + sqrtf(k)) * N[n];
          }
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float dot_n_i = N_column[row] * I_column[row];
        float k = 1.f - eta_column[row] * eta_column[row] * (1.f - dot_n_i * dot_n_i);
        if (k < 0.f) {
          result_column[row] = 0.f;
        }
        else {
          result_column[row] = eta_column[row] * I_column[row] - (eta_column[row] * dot_n_i + sqrtf(k)) * N_column[row];
        }
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_refract_v2v2f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict resultx_column = FLOAT_REG_PTR(x, 0);
  float *restrict resulty_column = FLOAT_REG_PTR(x, 1);
  float *restrict Ix_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict Iy_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict Nx_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict Ny_column = FLOAT_REG_PTR(x->children_[1], 1);
  float *restrict eta_column = FLOAT_REG_PTR(x->children_[2], 0);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict resultx = resultx_column + row;
        float *restrict resulty = resulty_column + row;
        const float *restrict Nx = Nx_column + row;
        const float *restrict Ny = Ny_column + row;
        const float *restrict Ix = Ix_column + row;
        const float *restrict Iy = Iy_column + row;
        const float *restrict eta = eta_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float dot_n_i = Nx[n] * Ix[n] + Ny[n] * Iy[n];
          float k = 1.f - eta[n] * eta[n] * (1.f - dot_n_i * dot_n_i);
          if (k < 0.f) {
            resultx[n] = 0.f;
            resulty[n] = 0.f;
          }
          else {
            resultx[n] = eta[n] * Ix[n] - (eta[n] * dot_n_i + sqrtf(k)) * Nx[n];
            resulty[n] = eta[n] * Iy[n] - (eta[n] * dot_n_i + sqrtf(k)) * Ny[n];
          }
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict resultx = resultx_column + row;
        float *restrict resulty = resulty_column + row;
        const float *restrict Nx = Nx_column + row;
        const float *restrict Ny = Ny_column + row;
        const float *restrict Ix = Ix_column + row;
        const float *restrict Iy = Iy_column + row;
        const float *restrict eta = eta_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float dot_n_i = Nx[n] * Ix[n] + Ny[n] * Iy[n];
          float k = 1.f - eta[n] * eta[n] * (1.f - dot_n_i * dot_n_i);
          if (k < 0.f) {
            resultx[n] = 0.f;
            resulty[n] = 0.f;
          }
          else {
            resultx[n] = eta[n] * Ix[n] - (eta[n] * dot_n_i + sqrtf(k)) * Nx[n];
            resulty[n] = eta[n] * Iy[n] - (eta[n] * dot_n_i + sqrtf(k)) * Ny[n];
          }
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float dot_n_i = Nx_column[row] * Ix_column[row] + Ny_column[row] * Iy_column[row];
        float k = 1.f - eta_column[row] * eta_column[row] * (1.f - dot_n_i * dot_n_i);
        if (k < 0.f) {
          resultx_column[row] = 0.f;
          resulty_column[row] = 0.f;
        }
        else {
          resultx_column[row] = eta_column[row] * Ix_column[row] - (eta_column[row] * dot_n_i + sqrtf(k)) * Nx_column[row];
          resulty_column[row] = eta_column[row] * Iy_column[row] - (eta_column[row] * dot_n_i + sqrtf(k)) * Ny_column[row];
        }
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_refract_v3v3f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict resultx_column = FLOAT_REG_PTR(x, 0);
  float *restrict resulty_column = FLOAT_REG_PTR(x, 1);
  float *restrict resultz_column = FLOAT_REG_PTR(x, 2);
  float *restrict Ix_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict Iy_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict Iz_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict Nx_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict Ny_column = FLOAT_REG_PTR(x->children_[1], 1);
  float *restrict Nz_column = FLOAT_REG_PTR(x->children_[1], 2);
  float *restrict eta_column = FLOAT_REG_PTR(x->children_[2], 0);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict resultx = resultx_column + row;
        float *restrict resulty = resulty_column + row;
        float *restrict resultz = resultz_column + row;
        const float *restrict Nx = Nx_column + row;
        const float *restrict Ny = Ny_column + row;
        const float *restrict Nz = Nz_column + row;
        const float *restrict Ix = Ix_column + row;
        const float *restrict Iy = Iy_column + row;
        const float *restrict Iz = Iz_column + row;
        const float *restrict eta = eta_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float dot_n_i = Nx[n] * Ix[n] + Ny[n] * Iy[n] + Nz[n] * Iz[n];
          float k = 1.f - eta[n] * eta[n] * (1.f - dot_n_i * dot_n_i);
          if (k < 0.f) {
            resultx[n] = 0.f;
            resulty[n] = 0.f;
            resultz[n] = 0.f;
          }
          else {
            resultx[n] = eta[n] * Ix[n] - (eta[n] * dot_n_i + sqrtf(k)) * Nx[n];
            resulty[n] = eta[n] * Iy[n] - (eta[n] * dot_n_i + sqrtf(k)) * Ny[n];
            resultz[n] = eta[n] * Iz[n] - (eta[n] * dot_n_i + sqrtf(k)) * Nz[n];
          }
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict resultx = resultx_column + row;
        float *restrict resulty = resulty_column + row;
        float *restrict resultz = resultz_column + row;
        const float *restrict Nx = Nx_column + row;
        const float *restrict Ny = Ny_column + row;
        const float *restrict Nz = Nz_column + row;
        const float *restrict Ix = Ix_column + row;
        const float *restrict Iy = Iy_column + row;
        const float *restrict Iz = Iz_column + row;
        const float *restrict eta = eta_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float dot_n_i = Nx[n] * Ix[n] + Ny[n] * Iy[n] + Nz[n] * Iz[n];
          float k = 1.f - eta[n] * eta[n] * (1.f - dot_n_i * dot_n_i);
          if (k < 0.f) {
            resultx[n] = 0.f;
            resulty[n] = 0.f;
            resultz[n] = 0.f;
          }
          else {
            resultx[n] = eta[n] * Ix[n] - (eta[n] * dot_n_i + sqrtf(k)) * Nx[n];
            resulty[n] = eta[n] * Iy[n] - (eta[n] * dot_n_i + sqrtf(k)) * Ny[n];
            resultz[n] = eta[n] * Iz[n] - (eta[n] * dot_n_i + sqrtf(k)) * Nz[n];
          }
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float dot_n_i = Nx_column[row] * Ix_column[row] + Ny_column[row] * Iy_column[row] + Nz_column[row] * Iz_column[row];
        float k = 1.f - eta_column[row] * eta_column[row] * (1.f - dot_n_i * dot_n_i);
        if (k < 0.f) {
          resultx_column[row] = 0.f;
          resulty_column[row] = 0.f;
          resultz_column[row] = 0.f;
        }
        else {
          resultx_column[row] = eta_column[row] * Ix_column[row] - (eta_column[row] * dot_n_i + sqrtf(k)) * Nx_column[row];
          resulty_column[row] = eta_column[row] * Iy_column[row] - (eta_column[row] * dot_n_i + sqrtf(k)) * Ny_column[row];
          resultz_column[row] = eta_column[row] * Iz_column[row] - (eta_column[row] * dot_n_i + sqrtf(k)) * Nz_column[row];
        }
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_refract_v4v4f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict resultx_column = FLOAT_REG_PTR(x, 0);
  float *restrict resulty_column = FLOAT_REG_PTR(x, 1);
  float *restrict resultz_column = FLOAT_REG_PTR(x, 2);
  float *restrict resultw_column = FLOAT_REG_PTR(x, 3);
  float *restrict Ix_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict Iy_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict Iz_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict Iw_column = FLOAT_REG_PTR(x->children_[0], 3);
  float *restrict Nx_column = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict Ny_column = FLOAT_REG_PTR(x->children_[1], 1);
  float *restrict Nz_column = FLOAT_REG_PTR(x->children_[1], 2);
  float *restrict Nw_column = FLOAT_REG_PTR(x->children_[1], 3);
  float *restrict eta_column = FLOAT_REG_PTR(x->children_[2], 0);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict resultx = resultx_column + row;
        float *restrict resulty = resulty_column + row;
        float *restrict resultz = resultz_column + row;
        float *restrict resultw = resultw_column + row;
        const float *restrict Nx = Nx_column + row;
        const float *restrict Ny = Ny_column + row;
        const float *restrict Nz = Nz_column + row;
        const float *restrict Nw = Nw_column + row;
        const float *restrict Ix = Ix_column + row;
        const float *restrict Iy = Iy_column + row;
        const float *restrict Iz = Iz_column + row;
        const float *restrict Iw = Iw_column + row;
        const float *restrict eta = eta_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          float dot_n_i = Nx[n] * Ix[n] + Ny[n] * Iy[n] + Nz[n] * Iz[n] + Nw[n] * Iw[n];
          float k = 1.f - eta[n] * eta[n] * (1.f - dot_n_i * dot_n_i);
          if (k < 0.f) {
            resultx[n] = 0.f;
            resulty[n] = 0.f;
            resultz[n] = 0.f;
            resultw[n] = 0.f;
          }
          else {
            resultx[n] = eta[n] * Ix[n] - (eta[n] * dot_n_i + sqrtf(k)) * Nx[n];
            resulty[n] = eta[n] * Iy[n] - (eta[n] * dot_n_i + sqrtf(k)) * Ny[n];
            resultz[n] = eta[n] * Iz[n] - (eta[n] * dot_n_i + sqrtf(k)) * Nz[n];
            resultw[n] = eta[n] * Iw[n] - (eta[n] * dot_n_i + sqrtf(k)) * Nw[n];
          }
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict resultx = resultx_column + row;
        float *restrict resulty = resulty_column + row;
        float *restrict resultz = resultz_column + row;
        float *restrict resultw = resultw_column + row;
        const float *restrict Nx = Nx_column + row;
        const float *restrict Ny = Ny_column + row;
        const float *restrict Nz = Nz_column + row;
        const float *restrict Nw = Nw_column + row;
        const float *restrict Ix = Ix_column + row;
        const float *restrict Iy = Iy_column + row;
        const float *restrict Iz = Iz_column + row;
        const float *restrict Iw = Iw_column + row;
        const float *restrict eta = eta_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          float dot_n_i = Nx[n] * Ix[n] + Ny[n] * Iy[n] + Nz[n] * Iz[n] + Nw[n] * Iw[n];
          float k = 1.f - eta[n] * eta[n] * (1.f - dot_n_i * dot_n_i);
          if (k < 0.f) {
            resultx[n] = 0.f;
            resulty[n] = 0.f;
            resultz[n] = 0.f;
            resultw[n] = 0.f;
          }
          else {
            resultx[n] = eta[n] * Ix[n] - (eta[n] * dot_n_i + sqrtf(k)) * Nx[n];
            resulty[n] = eta[n] * Iy[n] - (eta[n] * dot_n_i + sqrtf(k)) * Ny[n];
            resultz[n] = eta[n] * Iz[n] - (eta[n] * dot_n_i + sqrtf(k)) * Nz[n];
            resultw[n] = eta[n] * Iw[n] - (eta[n] * dot_n_i + sqrtf(k)) * Nw[n];
          }
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        float dot_n_i = Nx_column[row] * Ix_column[row] + Ny_column[row] * Iy_column[row] + Nz_column[row] * Iz_column[row] + Nw_column[row] * Iw_column[row];
        float k = 1.f - eta_column[row] * eta_column[row] * (1.f - dot_n_i * dot_n_i);
        if (k < 0.f) {
          resultx_column[row] = 0.f;
          resulty_column[row] = 0.f;
          resultz_column[row] = 0.f;
          resultw_column[row] = 0.f;
        }
        else {
          resultx_column[row] = eta_column[row] * Ix_column[row] - (eta_column[row] * dot_n_i + sqrtf(k)) * Nx_column[row];
          resulty_column[row] = eta_column[row] * Iy_column[row] - (eta_column[row] * dot_n_i + sqrtf(k)) * Ny_column[row];
          resultz_column[row] = eta_column[row] * Iz_column[row] - (eta_column[row] * dot_n_i + sqrtf(k)) * Nz_column[row];
          resultw_column[row] = eta_column[row] * Iw_column[row] - (eta_column[row] * dot_n_i + sqrtf(k)) * Nw_column[row];
        }
        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_refract_fff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opdI, opdN, opdeta;
  sl_expr_temp_init(&opdI, NULL);
  sl_expr_temp_init(&opdN, NULL);
  sl_expr_temp_init(&opdeta, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opdI)) {
    sl_expr_temp_cleanup(&opdI);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opdN)) {
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdN);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opdeta)) {
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdN);
    sl_expr_temp_cleanup(&opdeta);
    return;
  }
  float dot_n_i = opdN.v_.f_ * opdI.v_.f_;
  float k = 1.f - opdeta.v_.f_ * opdeta.v_.f_ * (1.f - dot_n_i * dot_n_i);
  if (k < 0.f) {
    sl_expr_temp_init_float(r, 0.f);
  }
  else {
    sl_expr_temp_init_float(r, opdeta.v_.f_ * opdI.v_.f_ - (opdeta.v_.f_ * dot_n_i + sqrtf(k)) * opdN.v_.f_);
  }
}

void builtin_refract_v2v2f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opdI, opdN, opdeta;
  sl_expr_temp_init(&opdI, NULL);
  sl_expr_temp_init(&opdN, NULL);
  sl_expr_temp_init(&opdeta, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opdI)) {
    sl_expr_temp_cleanup(&opdI);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opdN)) {
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdN);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opdeta)) {
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdN);
    sl_expr_temp_cleanup(&opdeta);
    return;
  }
  float dot_n_i = opdN.v_.v_[0] * opdI.v_.v_[0] + opdN.v_.v_[1] * opdI.v_.v_[1];
  float k = 1.f - opdeta.v_.f_ * opdeta.v_.f_ * (1.f - dot_n_i * dot_n_i);
  if (k < 0.f) {
    sl_expr_temp_init_vec2(r, 0.f, 0.f);
  }
  else {
    sl_expr_temp_init_vec2(r, opdeta.v_.f_ * opdI.v_.v_[0] - (opdeta.v_.f_ * dot_n_i + sqrtf(k)) * opdN.v_.v_[0],
                              opdeta.v_.f_ * opdI.v_.v_[1] - (opdeta.v_.f_ * dot_n_i + sqrtf(k)) * opdN.v_.v_[1]);
  }
}

void builtin_refract_v3v3f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opdI, opdN, opdeta;
  sl_expr_temp_init(&opdI, NULL);
  sl_expr_temp_init(&opdN, NULL);
  sl_expr_temp_init(&opdeta, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opdI)) {
    sl_expr_temp_cleanup(&opdI);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opdN)) {
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdN);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opdeta)) {
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdN);
    sl_expr_temp_cleanup(&opdeta);
    return;
  }
  float dot_n_i = opdN.v_.v_[0] * opdI.v_.v_[0] + opdN.v_.v_[1] * opdI.v_.v_[1] + opdN.v_.v_[2] * opdI.v_.v_[2];
  float k = 1.f - opdeta.v_.f_ * opdeta.v_.f_ * (1.f - dot_n_i * dot_n_i);
  if (k < 0.f) {
    sl_expr_temp_init_vec3(r, 0.f, 0.f, 0.f);
  }
  else {
    sl_expr_temp_init_vec3(r, opdeta.v_.f_ * opdI.v_.v_[0] - (opdeta.v_.f_ * dot_n_i + sqrtf(k)) * opdN.v_.v_[0],
                              opdeta.v_.f_ * opdI.v_.v_[1] - (opdeta.v_.f_ * dot_n_i + sqrtf(k)) * opdN.v_.v_[1],
                              opdeta.v_.f_ * opdI.v_.v_[2] - (opdeta.v_.f_ * dot_n_i + sqrtf(k)) * opdN.v_.v_[2]);
  }
}

void builtin_refract_v4v4f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opdI, opdN, opdeta;
  sl_expr_temp_init(&opdI, NULL);
  sl_expr_temp_init(&opdN, NULL);
  sl_expr_temp_init(&opdeta, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opdI)) {
    sl_expr_temp_cleanup(&opdI);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &opdN)) {
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdN);
    return;
  }
  if (sl_expr_eval(tb, x->children_[2], &opdeta)) {
    sl_expr_temp_cleanup(&opdI);
    sl_expr_temp_cleanup(&opdN);
    sl_expr_temp_cleanup(&opdeta);
    return;
  }
  float dot_n_i = opdN.v_.v_[0] * opdI.v_.v_[0] + opdN.v_.v_[1] * opdI.v_.v_[1] + opdN.v_.v_[2] * opdI.v_.v_[2] + opdN.v_.v_[3] * opdI.v_.v_[3];
  float k = 1.f - opdeta.v_.f_ * opdeta.v_.f_ * (1.f - dot_n_i * dot_n_i);
  if (k < 0.f) {
    sl_expr_temp_init_vec4(r, 0.f, 0.f, 0.f, 0.f);
  }
  else {
    sl_expr_temp_init_vec4(r, opdeta.v_.f_ * opdI.v_.v_[0] - (opdeta.v_.f_ * dot_n_i + sqrtf(k)) * opdN.v_.v_[0],
                              opdeta.v_.f_ * opdI.v_.v_[1] - (opdeta.v_.f_ * dot_n_i + sqrtf(k)) * opdN.v_.v_[1],
                              opdeta.v_.f_ * opdI.v_.v_[2] - (opdeta.v_.f_ * dot_n_i + sqrtf(k)) * opdN.v_.v_[2],
                              opdeta.v_.f_ * opdI.v_.v_[3] - (opdeta.v_.f_ * dot_n_i + sqrtf(k)) * opdN.v_.v_[3]);
  }
}

/* Matrix Functions */
static void builtin_matrixCompMult_mNmN_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x, size_t N) {
  size_t component_index;

  for (component_index = 0; component_index < N; ++component_index) {
    uint8_t *restrict chain_column = exec->exec_chain_reg_;
    float *restrict result_column = FLOAT_REG_PTR(x, component_index);
    float *restrict first_opd_column = FLOAT_REG_PTR(x->children_[0], component_index);
    float *restrict second_opd_column = FLOAT_REG_PTR(x->children_[1], component_index);
    uint8_t row = exec_chain;

    for (;;) {
      uint64_t chain;
      uint8_t delta;

      if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
        do {
          float *restrict result = result_column + row;
          const float *first_opd = first_opd_column + row;
          const float *second_opd = second_opd_column + row;
          int n;
          /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
          for (n = 0; n < 8; n++) {
            result[n] = first_opd[n] * second_opd[n];
          }

          delta = (chain & 0xFF00000000000000) >> 56;
          if (!delta) break;
          row += 7 + delta;
        } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
      }
      else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
        do {
          float *restrict result = result_column + row;
          const float *first_opd = first_opd_column + row;
          const float *second_opd = second_opd_column + row;
          int n;
          /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
          for (n = 0; n < 4; n++) {
            result[n] = first_opd[n] * second_opd[n];
          }
          delta = (chain & 0xFF000000) >> 24;
          if (!delta) break;
          row += 3 + delta;
        } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
      }
      else {
        do {
          /* Not trying to evoke auto-vectorization, just get it done. */
          result_column[row] = first_opd_column[row] * second_opd_column[row];

          delta = chain_column[row];
          if (!delta) break;
          row += delta;
        } while (row & 3);
      }
      if (!delta) break;
    }
  }
}

void builtin_matrixCompMult_m2m2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  builtin_matrixCompMult_mNmN_runtime(exec, exec_chain, x, 4);
}


void builtin_matrixCompMult_m3m3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  builtin_matrixCompMult_mNmN_runtime(exec, exec_chain, x, 9);
}

void builtin_matrixCompMult_m4m4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  builtin_matrixCompMult_mNmN_runtime(exec, exec_chain, x, 16);
}

static void builtin_matrixCompMult_mNmN_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r, sl_expr_temp_kind_t kind, size_t N) {
  struct sl_expr_temp first_opd, second_opd;
  sl_expr_temp_init(&first_opd, NULL);
  sl_expr_temp_init(&second_opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &first_opd)) {
    sl_expr_temp_cleanup(&first_opd);
    return;
  }
  if (sl_expr_eval(tb, x->children_[1], &second_opd)) {
    sl_expr_temp_cleanup(&first_opd);
    sl_expr_temp_cleanup(&second_opd);
    return;
  }
  sl_expr_temp_init_void(r);
  r->kind_ = kind;
  size_t component_index;
  for (component_index = 0; component_index < N; ++component_index) {
    r->v_.m_[component_index] = first_opd.v_.m_[component_index] * second_opd.v_.m_[component_index];
  }
}

void builtin_matrixCompMult_m2m2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  builtin_matrixCompMult_mNmN_eval(tb, x, r, sletk_mat2, 4);
}

void builtin_matrixCompMult_m3m3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  builtin_matrixCompMult_mNmN_eval(tb, x, r, sletk_mat3, 9);
}

void builtin_matrixCompMult_m4m4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  builtin_matrixCompMult_mNmN_eval(tb, x, r, sletk_mat4, 16);
}

/* Vector Relational Functions */

#define aex_less_thanf(a,b) (uint8_t)(0x100 - ((a)<(b)))
#define aex_less_thani(a,b) (uint8_t)(0x100 - ((a)<(b)))
#define aex_less_than_equalf(a,b) (uint8_t)(0x100 - ((a)<=(b)))
#define aex_less_than_equali(a,b) (uint8_t)(0x100 - ((a)<=(b)))
#define aex_greater_thanf(a,b) (uint8_t)(0x100 - ((a)>(b)))
#define aex_greater_thani(a,b) (uint8_t)(0x100 - ((a)>(b)))
#define aex_greater_than_equalf(a,b) (uint8_t)(0x100 - ((a)>=(b)))
#define aex_greater_than_equali(a,b) (uint8_t)(0x100 - ((a)>=(b)))
#define aex_equalf(a,b) (uint8_t)(0x100 - ((a)==(b)))
#define aex_equali(a,b) (uint8_t)(0x100 - ((a)==(b)))
#define aex_equalb(a,b) (uint8_t)(0x100 - (!(a)==!(b)))
#define aex_not_equalf(a,b) (uint8_t)(0x100 - ((a)!=(b)))
#define aex_not_equali(a,b) (uint8_t)(0x100 - ((a)!=(b)))
#define aex_not_equalb(a,b) (uint8_t)(0x100 - (!(a)!=!(b)))

void builtin_lessThan_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_less_thanf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_lessThan_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_less_thanf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_lessThan_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_less_thanf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 3);
  right_column = FLOAT_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_lessThan_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_less_thanf(opd0.v_.v_[0], opd1.v_.v_[0]),
                             aex_less_thanf(opd0.v_.v_[1], opd1.v_.v_[1]));
}

void builtin_lessThan_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_less_thanf(opd0.v_.v_[0], opd1.v_.v_[0]),
                             aex_less_thanf(opd0.v_.v_[1], opd1.v_.v_[1]),
                             aex_less_thanf(opd0.v_.v_[2], opd1.v_.v_[2]));
}

void builtin_lessThan_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_less_thanf(opd0.v_.v_[0], opd1.v_.v_[0]),
                             aex_less_thanf(opd0.v_.v_[1], opd1.v_.v_[1]),
                             aex_less_thanf(opd0.v_.v_[2], opd1.v_.v_[2]),
                             aex_less_thanf(opd0.v_.v_[3], opd1.v_.v_[3]));
}

void builtin_lessThan_iv2iv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_less_thani(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_lessThan_iv3iv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_less_thani(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 2);
  right_column = INT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_lessThan_iv4iv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_less_thani(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t 
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 2);
  right_column = INT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 3);
  right_column = INT_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_lessThan_iv2iv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_less_thani(opd0.v_.iv_[0], opd1.v_.iv_[0]),
                             aex_less_thani(opd0.v_.iv_[1], opd1.v_.iv_[1]));
}

void builtin_lessThan_iv3iv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_less_thani(opd0.v_.iv_[0], opd1.v_.iv_[0]),
                             aex_less_thani(opd0.v_.iv_[1], opd1.v_.iv_[1]),
                             aex_less_thani(opd0.v_.iv_[2], opd1.v_.iv_[2]));
}

void builtin_lessThan_iv4iv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_less_thani(opd0.v_.iv_[0], opd1.v_.iv_[0]),
                             aex_less_thani(opd0.v_.iv_[1], opd1.v_.iv_[1]),
                             aex_less_thani(opd0.v_.iv_[2], opd1.v_.iv_[2]),
                             aex_less_thani(opd0.v_.iv_[3], opd1.v_.iv_[3]));
}

void builtin_lessThanEqual_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_less_than_equalf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_lessThanEqual_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_less_than_equalf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_lessThanEqual_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_less_than_equalf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 3);
  right_column = FLOAT_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_lessThanEqual_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_less_than_equalf(opd0.v_.v_[0], opd1.v_.v_[0]),
                             aex_less_than_equalf(opd0.v_.v_[1], opd1.v_.v_[1]));
}

void builtin_lessThanEqual_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_less_than_equalf(opd0.v_.v_[0], opd1.v_.v_[0]),
                             aex_less_than_equalf(opd0.v_.v_[1], opd1.v_.v_[1]),
                             aex_less_than_equalf(opd0.v_.v_[2], opd1.v_.v_[2]));
}

void builtin_lessThanEqual_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_less_than_equalf(opd0.v_.v_[0], opd1.v_.v_[0]),
                             aex_less_than_equalf(opd0.v_.v_[1], opd1.v_.v_[1]),
                             aex_less_than_equalf(opd0.v_.v_[2], opd1.v_.v_[2]),
                             aex_less_than_equalf(opd0.v_.v_[3], opd1.v_.v_[3]));
}

void builtin_lessThanEqual_iv2iv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_less_than_equali(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_lessThanEqual_iv3iv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_less_than_equali(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 2);
  right_column = INT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_lessThanEqual_iv4iv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_less_than_equali(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t 
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 2);
  right_column = INT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 3);
  right_column = INT_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_lessThanEqual_iv2iv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_less_than_equali(opd0.v_.iv_[0], opd1.v_.iv_[0]),
                             aex_less_than_equali(opd0.v_.iv_[1], opd1.v_.iv_[1]));
}

void builtin_lessThanEqual_iv3iv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_less_than_equali(opd0.v_.iv_[0], opd1.v_.iv_[0]),
                             aex_less_than_equali(opd0.v_.iv_[1], opd1.v_.iv_[1]),
                             aex_less_than_equali(opd0.v_.iv_[2], opd1.v_.iv_[2]));
}

void builtin_lessThanEqual_iv4iv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_less_than_equali(opd0.v_.iv_[0], opd1.v_.iv_[0]),
                             aex_less_than_equali(opd0.v_.iv_[1], opd1.v_.iv_[1]),
                             aex_less_than_equali(opd0.v_.iv_[2], opd1.v_.iv_[2]),
                             aex_less_than_equali(opd0.v_.iv_[3], opd1.v_.iv_[3]));
}

void builtin_greaterThan_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_greater_thanf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_greaterThan_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_greater_thanf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_greaterThan_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_greater_thanf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 3);
  right_column = FLOAT_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_greaterThan_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_greater_thanf(opd0.v_.v_[0], opd1.v_.v_[0]),
                             aex_greater_thanf(opd0.v_.v_[1], opd1.v_.v_[1]));
}

void builtin_greaterThan_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_greater_thanf(opd0.v_.v_[0], opd1.v_.v_[0]),
                             aex_greater_thanf(opd0.v_.v_[1], opd1.v_.v_[1]),
                             aex_greater_thanf(opd0.v_.v_[2], opd1.v_.v_[2]));
}

void builtin_greaterThan_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_greater_thanf(opd0.v_.v_[0], opd1.v_.v_[0]),
                             aex_greater_thanf(opd0.v_.v_[1], opd1.v_.v_[1]),
                             aex_greater_thanf(opd0.v_.v_[2], opd1.v_.v_[2]),
                             aex_greater_thanf(opd0.v_.v_[3], opd1.v_.v_[3]));
}

void builtin_greaterThan_iv2iv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_greater_thani(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_greaterThan_iv3iv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_greater_thani(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 2);
  right_column = INT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_greaterThan_iv4iv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_greater_thani(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t 
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 2);
  right_column = INT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 3);
  right_column = INT_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_greaterThan_iv2iv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_greater_thani(opd0.v_.iv_[0], opd1.v_.iv_[0]),
                             aex_greater_thani(opd0.v_.iv_[1], opd1.v_.iv_[1]));
}

void builtin_greaterThan_iv3iv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_greater_thani(opd0.v_.iv_[0], opd1.v_.iv_[0]),
                             aex_greater_thani(opd0.v_.iv_[1], opd1.v_.iv_[1]),
                             aex_greater_thani(opd0.v_.iv_[2], opd1.v_.iv_[2]));
}

void builtin_greaterThan_iv4iv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_greater_thani(opd0.v_.iv_[0], opd1.v_.iv_[0]),
                             aex_greater_thani(opd0.v_.iv_[1], opd1.v_.iv_[1]),
                             aex_greater_thani(opd0.v_.iv_[2], opd1.v_.iv_[2]),
                             aex_greater_thani(opd0.v_.iv_[3], opd1.v_.iv_[3]));
}

void builtin_greaterThanEqual_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_greater_than_equalf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_greaterThanEqual_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_greater_than_equalf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_greaterThanEqual_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_greater_than_equalf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 3);
  right_column = FLOAT_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_greaterThanEqual_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_greater_than_equalf(opd0.v_.v_[0], opd1.v_.v_[0]),
                             aex_greater_than_equalf(opd0.v_.v_[1], opd1.v_.v_[1]));
}

void builtin_greaterThanEqual_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_greater_than_equalf(opd0.v_.v_[0], opd1.v_.v_[0]),
                             aex_greater_than_equalf(opd0.v_.v_[1], opd1.v_.v_[1]),
                             aex_greater_than_equalf(opd0.v_.v_[2], opd1.v_.v_[2]));
}

void builtin_greaterThanEqual_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_greater_than_equalf(opd0.v_.v_[0], opd1.v_.v_[0]),
                             aex_greater_than_equalf(opd0.v_.v_[1], opd1.v_.v_[1]),
                             aex_greater_than_equalf(opd0.v_.v_[2], opd1.v_.v_[2]),
                             aex_greater_than_equalf(opd0.v_.v_[3], opd1.v_.v_[3]));
}

void builtin_greaterThanEqual_iv2iv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_greater_than_equali(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_greaterThanEqual_iv3iv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_greater_than_equali(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 2);
  right_column = INT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_greaterThanEqual_iv4iv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_greater_than_equali(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t 
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 2);
  right_column = INT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 3);
  right_column = INT_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_greaterThanEqual_iv2iv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_greater_than_equali(opd0.v_.iv_[0], opd1.v_.iv_[0]),
                             aex_greater_than_equali(opd0.v_.iv_[1], opd1.v_.iv_[1]));
}

void builtin_greaterThanEqual_iv3iv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_greater_than_equali(opd0.v_.iv_[0], opd1.v_.iv_[0]),
                             aex_greater_than_equali(opd0.v_.iv_[1], opd1.v_.iv_[1]),
                             aex_greater_than_equali(opd0.v_.iv_[2], opd1.v_.iv_[2]));
}

void builtin_greaterThanEqual_iv4iv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_greater_than_equali(opd0.v_.iv_[0], opd1.v_.iv_[0]),
                             aex_greater_than_equali(opd0.v_.iv_[1], opd1.v_.iv_[1]),
                             aex_greater_than_equali(opd0.v_.iv_[2], opd1.v_.iv_[2]),
                             aex_greater_than_equali(opd0.v_.iv_[3], opd1.v_.iv_[3]));
}


void builtin_equal_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_equalf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_equal_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_equalf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_equal_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_equalf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 3);
  right_column = FLOAT_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_equal_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_equalf(opd0.v_.v_[0], opd1.v_.v_[0]),
    aex_equalf(opd0.v_.v_[1], opd1.v_.v_[1]));
}

void builtin_equal_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_equalf(opd0.v_.v_[0], opd1.v_.v_[0]),
    aex_equalf(opd0.v_.v_[1], opd1.v_.v_[1]),
    aex_equalf(opd0.v_.v_[2], opd1.v_.v_[2]));
}

void builtin_equal_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_equalf(opd0.v_.v_[0], opd1.v_.v_[0]),
    aex_equalf(opd0.v_.v_[1], opd1.v_.v_[1]),
    aex_equalf(opd0.v_.v_[2], opd1.v_.v_[2]),
    aex_equalf(opd0.v_.v_[3], opd1.v_.v_[3]));
}

void builtin_equal_iv2iv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_equali(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_equal_iv3iv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_equali(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 2);
  right_column = INT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_equal_iv4iv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_equali(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t 
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 2);
  right_column = INT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 3);
  right_column = INT_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_equal_iv2iv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_equali(opd0.v_.iv_[0], opd1.v_.iv_[0]),
    aex_equali(opd0.v_.iv_[1], opd1.v_.iv_[1]));
}

void builtin_equal_iv3iv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_equali(opd0.v_.iv_[0], opd1.v_.iv_[0]),
    aex_equali(opd0.v_.iv_[1], opd1.v_.iv_[1]),
    aex_equali(opd0.v_.iv_[2], opd1.v_.iv_[2]));
}

void builtin_equal_iv4iv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_equali(opd0.v_.iv_[0], opd1.v_.iv_[0]),
    aex_equali(opd0.v_.iv_[1], opd1.v_.iv_[1]),
    aex_equali(opd0.v_.iv_[2], opd1.v_.iv_[2]),
    aex_equali(opd0.v_.iv_[3], opd1.v_.iv_[3]));
}

void builtin_equal_bv2bv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  uint8_t *restrict left_column;
  uint8_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_equalb(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE uint8_t
  left_column = BOOL_REG_PTR(x->children_[0], 0);
  right_column = BOOL_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = BOOL_REG_PTR(x->children_[0], 1);
  right_column = BOOL_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_equal_bv3bv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  uint8_t *restrict left_column;
  uint8_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_equalb(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE uint8_t
  left_column = BOOL_REG_PTR(x->children_[0], 0);
  right_column = BOOL_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = BOOL_REG_PTR(x->children_[0], 1);
  right_column = BOOL_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = BOOL_REG_PTR(x->children_[0], 2);
  right_column = BOOL_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_equal_bv4bv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  uint8_t *restrict left_column;
  uint8_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_equalb(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE uint8_t
  left_column = BOOL_REG_PTR(x->children_[0], 0);
  right_column = BOOL_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = BOOL_REG_PTR(x->children_[0], 1);
  right_column = BOOL_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = BOOL_REG_PTR(x->children_[0], 2);
  right_column = BOOL_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = BOOL_REG_PTR(x->children_[0], 3);
  right_column = BOOL_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_equal_bv2bv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_equalb(opd0.v_.bv_[0], opd1.v_.bv_[0]),
    aex_equalb(opd0.v_.bv_[1], opd1.v_.bv_[1]));
}

void builtin_equal_bv3bv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_equalb(opd0.v_.bv_[0], opd1.v_.bv_[0]),
    aex_equalb(opd0.v_.bv_[1], opd1.v_.bv_[1]),
    aex_equalb(opd0.v_.bv_[2], opd1.v_.bv_[2]));
}

void builtin_equal_bv4bv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_equalb(opd0.v_.bv_[0], opd1.v_.bv_[0]),
    aex_equalb(opd0.v_.bv_[1], opd1.v_.bv_[1]),
    aex_equalb(opd0.v_.bv_[2], opd1.v_.bv_[2]),
    aex_equalb(opd0.v_.bv_[3], opd1.v_.bv_[3]));
}

void builtin_notEqual_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_not_equalf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_notEqual_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_not_equalf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_notEqual_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  float *restrict left_column;
  float *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_not_equalf(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE float
  left_column = FLOAT_REG_PTR(x->children_[0], 0);
  right_column = FLOAT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 1);
  right_column = FLOAT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 2);
  right_column = FLOAT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = FLOAT_REG_PTR(x->children_[0], 3);
  right_column = FLOAT_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_notEqual_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_not_equalf(opd0.v_.v_[0], opd1.v_.v_[0]),
    aex_not_equalf(opd0.v_.v_[1], opd1.v_.v_[1]));
}

void builtin_notEqual_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_not_equalf(opd0.v_.v_[0], opd1.v_.v_[0]),
    aex_not_equalf(opd0.v_.v_[1], opd1.v_.v_[1]),
    aex_not_equalf(opd0.v_.v_[2], opd1.v_.v_[2]));
}

void builtin_notEqual_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_not_equalf(opd0.v_.v_[0], opd1.v_.v_[0]),
    aex_not_equalf(opd0.v_.v_[1], opd1.v_.v_[1]),
    aex_not_equalf(opd0.v_.v_[2], opd1.v_.v_[2]),
    aex_not_equalf(opd0.v_.v_[3], opd1.v_.v_[3]));
}

void builtin_notEqual_iv2iv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_not_equali(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_notEqual_iv3iv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_not_equali(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 2);
  right_column = INT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_notEqual_iv4iv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  int64_t *restrict left_column;
  int64_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_not_equali(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE int64_t 
  left_column = INT_REG_PTR(x->children_[0], 0);
  right_column = INT_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 1);
  right_column = INT_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 2);
  right_column = INT_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = INT_REG_PTR(x->children_[0], 3);
  right_column = INT_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_notEqual_iv2iv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_not_equali(opd0.v_.iv_[0], opd1.v_.iv_[0]),
    aex_not_equali(opd0.v_.iv_[1], opd1.v_.iv_[1]));
}

void builtin_notEqual_iv3iv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_not_equali(opd0.v_.iv_[0], opd1.v_.iv_[0]),
    aex_not_equali(opd0.v_.iv_[1], opd1.v_.iv_[1]),
    aex_not_equali(opd0.v_.iv_[2], opd1.v_.iv_[2]));
}

void builtin_notEqual_iv4iv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_not_equali(opd0.v_.iv_[0], opd1.v_.iv_[0]),
    aex_not_equali(opd0.v_.iv_[1], opd1.v_.iv_[1]),
    aex_not_equali(opd0.v_.iv_[2], opd1.v_.iv_[2]),
    aex_not_equali(opd0.v_.iv_[3], opd1.v_.iv_[3]));
}

void builtin_notEqual_bv2bv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  uint8_t *restrict left_column;
  uint8_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_not_equalb(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE uint8_t
  left_column = BOOL_REG_PTR(x->children_[0], 0);
  right_column = BOOL_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = BOOL_REG_PTR(x->children_[0], 1);
  right_column = BOOL_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_notEqual_bv3bv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  uint8_t *restrict left_column;
  uint8_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_not_equalb(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE uint8_t
  left_column = BOOL_REG_PTR(x->children_[0], 0);
  right_column = BOOL_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = BOOL_REG_PTR(x->children_[0], 1);
  right_column = BOOL_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = BOOL_REG_PTR(x->children_[0], 2);
  right_column = BOOL_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_notEqual_bv4bv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column;
  uint8_t *restrict left_column;
  uint8_t *restrict right_column;
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) aex_not_equalb(x, y)
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE uint8_t
  left_column = BOOL_REG_PTR(x->children_[0], 0);
  right_column = BOOL_REG_PTR(x->children_[1], 0);
  result_column = BOOL_REG_PTR(x, 0);

#include "sl_binop_snippet_inc.h"

  left_column = BOOL_REG_PTR(x->children_[0], 1);
  right_column = BOOL_REG_PTR(x->children_[1], 1);
  result_column = BOOL_REG_PTR(x, 1);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = BOOL_REG_PTR(x->children_[0], 2);
  right_column = BOOL_REG_PTR(x->children_[1], 2);
  result_column = BOOL_REG_PTR(x, 2);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

  left_column = BOOL_REG_PTR(x->children_[0], 3);
  right_column = BOOL_REG_PTR(x->children_[1], 3);
  result_column = BOOL_REG_PTR(x, 3);

  row = exec_chain;
#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_notEqual_bv2bv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec2(r, aex_not_equalb(opd0.v_.bv_[0], opd1.v_.bv_[0]),
    aex_not_equalb(opd0.v_.bv_[1], opd1.v_.bv_[1]));
}

void builtin_notEqual_bv3bv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec3(r, aex_not_equalb(opd0.v_.bv_[0], opd1.v_.bv_[0]),
    aex_not_equalb(opd0.v_.bv_[1], opd1.v_.bv_[1]),
    aex_not_equalb(opd0.v_.bv_[2], opd1.v_.bv_[2]));
}

void builtin_notEqual_bv4bv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
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
  sl_expr_temp_init_bvec4(r, aex_not_equalb(opd0.v_.bv_[0], opd1.v_.bv_[0]),
    aex_not_equalb(opd0.v_.bv_[1], opd1.v_.bv_[1]),
    aex_not_equalb(opd0.v_.bv_[2], opd1.v_.bv_[2]),
    aex_not_equalb(opd0.v_.bv_[3], opd1.v_.bv_[3]));
}

void builtin_any_bv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column = BOOL_REG_PTR(x, 0);
  uint8_t *restrict left_column = BOOL_REG_PTR(x->children_[0], 0);
  uint8_t *restrict right_column = BOOL_REG_PTR(x->children_[0], 1);
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) ((x) || (y))
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE uint8_t

#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_any_bv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column = BOOL_REG_PTR(x, 0);
  uint8_t *restrict first_column = BOOL_REG_PTR(x->children_[0], 0);
  uint8_t *restrict second_column = BOOL_REG_PTR(x->children_[0], 1);
  uint8_t *restrict third_column = BOOL_REG_PTR(x->children_[0], 2);
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(a, b, c) ((a) || (b) || (c))
#define TERNOP_SNIPPET_TYPE uint8_t
#include "sl_ternop_snippet_inc.h"
#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_any_bv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column = BOOL_REG_PTR(x, 0);
  uint8_t *restrict first_column = BOOL_REG_PTR(x->children_[0], 0);
  uint8_t *restrict second_column = BOOL_REG_PTR(x->children_[0], 1);
  uint8_t *restrict third_column = BOOL_REG_PTR(x->children_[0], 2);
  uint8_t *restrict fourth_column = BOOL_REG_PTR(x->children_[0], 3);
  uint8_t row = exec_chain;

#define QUADOP_SNIPPET_OPERATOR(a, b, c, d) ((a) || (b) || (c) || (d))
#define QUADOP_SNIPPET_TYPE uint8_t
#include "sl_quadop_snippet_inc.h"
#undef QUADOP_SNIPPET_OPERATOR
#undef QUADOP_SNIPPET_TYPE
}

void builtin_any_bv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_bool(r, opd.v_.bv_[0] || opd.v_.bv_[1]);
}

void builtin_any_bv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_bool(r, opd.v_.bv_[0] || opd.v_.bv_[1] || opd.v_.bv_[2]);
}

void builtin_any_bv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_bool(r, opd.v_.bv_[0] || opd.v_.bv_[1] || opd.v_.bv_[2] || opd.v_.bv_[3]);
}

void builtin_all_bv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column = BOOL_REG_PTR(x, 0);
  uint8_t *restrict left_column = BOOL_REG_PTR(x->children_[0], 0);
  uint8_t *restrict right_column = BOOL_REG_PTR(x->children_[0], 1);
  uint8_t row = exec_chain;

#define BINOP_SNIPPET_OPERATOR(x, y) ((x) && (y))
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#define BINOP_SNIPPET_TYPE uint8_t

#include "sl_binop_snippet_inc.h"

#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_RESULT_TYPE
#undef BINOP_SNIPPET_TYPE
}

void builtin_all_bv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column = BOOL_REG_PTR(x, 0);
  uint8_t *restrict first_column = BOOL_REG_PTR(x->children_[0], 0);
  uint8_t *restrict second_column = BOOL_REG_PTR(x->children_[0], 1);
  uint8_t *restrict third_column = BOOL_REG_PTR(x->children_[0], 2);
  uint8_t row = exec_chain;

#define TERNOP_SNIPPET_OPERATOR(a, b, c) ((a) && (b) && (c))
#define TERNOP_SNIPPET_TYPE uint8_t
#include "sl_ternop_snippet_inc.h"
#undef TERNOP_SNIPPET_OPERATOR
#undef TERNOP_SNIPPET_TYPE
}

void builtin_all_bv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint8_t *restrict result_column = BOOL_REG_PTR(x, 0);
  uint8_t *restrict first_column = BOOL_REG_PTR(x->children_[0], 0);
  uint8_t *restrict second_column = BOOL_REG_PTR(x->children_[0], 1);
  uint8_t *restrict third_column = BOOL_REG_PTR(x->children_[0], 2);
  uint8_t *restrict fourth_column = BOOL_REG_PTR(x->children_[0], 3);
  uint8_t row = exec_chain;

#define QUADOP_SNIPPET_OPERATOR(a, b, c, d) ((a) && (b) && (c) && (d))
#define QUADOP_SNIPPET_TYPE uint8_t
#include "sl_quadop_snippet_inc.h"
#undef QUADOP_SNIPPET_OPERATOR
#undef QUADOP_SNIPPET_TYPE
}

void builtin_all_bv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_bool(r, opd.v_.bv_[0] && opd.v_.bv_[1]);
}

void builtin_all_bv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_bool(r, opd.v_.bv_[0] && opd.v_.bv_[1] && opd.v_.bv_[2]);
}

void builtin_all_bv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_bool(r, opd.v_.bv_[0] && opd.v_.bv_[1] && opd.v_.bv_[2] && opd.v_.bv_[3]);
}


void builtin_not_bv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict result_column;
  uint8_t * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) (uint8_t)(0x100 - !(opd))
#define UNOP_SNIPPET_TYPE uint8_t 
  result_column = BOOL_REG_PTR(x, 0);
  opd_column = BOOL_REG_PTR(x->children_[0], 0); 

#include "sl_unop_snippet_inc.h"

  result_column = BOOL_REG_PTR(x, 1);
  opd_column = BOOL_REG_PTR(x->children_[0], 1);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_not_bv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict result_column;
  uint8_t * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) (uint8_t)(0x100 - !(opd))
#define UNOP_SNIPPET_TYPE uint8_t 
  result_column = BOOL_REG_PTR(x, 0);
  opd_column = BOOL_REG_PTR(x->children_[0], 0); 

#include "sl_unop_snippet_inc.h"

  result_column = BOOL_REG_PTR(x, 1);
  opd_column = BOOL_REG_PTR(x->children_[0], 1);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = BOOL_REG_PTR(x, 2);
  opd_column = BOOL_REG_PTR(x->children_[0], 2);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_not_bv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict result_column;
  uint8_t * restrict opd_column;
  uint8_t row = exec_chain;

#define UNOP_SNIPPET_OPERATOR(opd) (uint8_t)(0x100 - !(opd))
#define UNOP_SNIPPET_TYPE uint8_t 
  result_column = BOOL_REG_PTR(x, 0);
  opd_column = BOOL_REG_PTR(x->children_[0], 0); 

#include "sl_unop_snippet_inc.h"

  result_column = BOOL_REG_PTR(x, 1);
  opd_column = BOOL_REG_PTR(x->children_[0], 1);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

  result_column = BOOL_REG_PTR(x, 2);
  opd_column = BOOL_REG_PTR(x->children_[0], 1);
  row = exec_chain;
#include "sl_unop_snippet_inc.h"

#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void builtin_not_bv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_bvec2(r, (uint8_t)(0x100 - !opd.v_.bv_[0]),
                             (uint8_t)(0x100 - !opd.v_.bv_[1]));
}

void builtin_not_bv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_bvec3(r, (uint8_t)(0x100 - !opd.v_.bv_[0]),
                             (uint8_t)(0x100 - !opd.v_.bv_[1]),
                             (uint8_t)(0x100 - !opd.v_.bv_[2]));
}

void builtin_not_bv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  struct sl_expr_temp opd;
  sl_expr_temp_init(&opd, NULL);
  if (sl_expr_eval(tb, x->children_[0], &opd)) {
    sl_expr_temp_cleanup(&opd);
    return;
  }
  sl_expr_temp_init_bvec4(r, (uint8_t)(0x100 - !opd.v_.bv_[0]),
                             (uint8_t)(0x100 - !opd.v_.bv_[1]),
                             (uint8_t)(0x100 - !opd.v_.bv_[2]),
                             (uint8_t)(0x100 - !opd.v_.bv_[3]));
}

void builtin_dFdx_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict opd = opd_column + row;
        int n;
        /* Fragment rows are ordered per square of 4 fragments, repeating
         * every 4 fragments. Top-Left %00, Top-Right %01, Bottom-Left %10, Bottom-Right %11.
         * Even if, in such a square, some fragments might be outside the primitive being rasterized, it
         * is still generated, and the fragment shader is executed (but masked, so it won't generate
         * output.) */
        for (n = 0; n < 8; n += 2) {
          float Fx_dx = opd[n + 1];
          float Fx = opd[n];
          float dFdx = Fx_dx - Fx;
          result[n] = dFdx;
          result[n + 1] = dFdx;
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict opd = opd_column + row;
        int n;
        
        for (n = 0; n < 4; n++) {
          float Fx_dx = opd[n + 1];
          float Fx = opd[n];
          float dFdx = Fx_dx - Fx;
          result[n] = dFdx;
          result[n + 1] = dFdx;
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* We get here then in the square of fragments, not all four fragments have followed the
         * same execution path, with only some arriving at the dFdx, and others going elsewhere.
         * We will still execute as if they all arrived at the dFdx and rely on the undefined 
         * results producing at least *something* that can be reasoned about. */
        float Fx_dx = opd_column[row | 1];
        float Fx = opd_column[row & ~1];
        float dFdx = Fx_dx - Fx;
        result_column[row] = dFdx;

        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_dFdx_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result0_column = FLOAT_REG_PTR(x, 0);
  float *restrict result1_column = FLOAT_REG_PTR(x, 1);
  float *restrict opd0_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict opd1_column = FLOAT_REG_PTR(x->children_[0], 1);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        int n;
        /* Fragment rows are ordered per square of 4 fragments, repeating
         * every 4 fragments. Top-Left %00, Top-Right %01, Bottom-Left %10, Bottom-Right %11.
         * Even if, in such a square, some fragments might be outside the primitive being rasterized, it
         * is still generated, and the fragment shader is executed (but masked, so it won't generate
         * output.) */
        for (n = 0; n < 8; n += 2) {
          float Fx_dx_0 = opd0[n + 1];
          float Fx_0 = opd0[n];
          float dFdx_0 = Fx_dx_0 - Fx_0;
          result0[n] = dFdx_0;
          result0[n + 1] = dFdx_0;

          float Fx_dx_1 = opd1[n + 1];
          float Fx_1 = opd1[n];
          float dFdx_1 = Fx_dx_1 - Fx_1;
          result1[n] = dFdx_1;
          result1[n + 1] = dFdx_1;
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        int n;
        
        for (n = 0; n < 4; n++) {
          float Fx_dx_0 = opd0[n + 1];
          float Fx_0 = opd0[n];
          float dFdx_0 = Fx_dx_0 - Fx_0;
          result0[n] = dFdx_0;
          result0[n + 1] = dFdx_0;

          float Fx_dx_1 = opd1[n + 1];
          float Fx_1 = opd1[n];
          float dFdx_1 = Fx_dx_1 - Fx_1;
          result1[n] = dFdx_1;
          result1[n + 1] = dFdx_1;
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* We get here then in the square of fragments, not all four fragments have followed the
         * same execution path, with only some arriving at the dFdx, and others going elsewhere.
         * We will still execute as if they all arrived at the dFdx and rely on the undefined 
         * results producing at least *something* that can be reasoned about. */
        float Fx_dx_0 = opd0_column[row | 1];
        float Fx_0 = opd0_column[row & ~1];
        float dFdx_0 = Fx_dx_0 - Fx_0;
        result0_column[row] = dFdx_0;

        float Fx_dx_1 = opd1_column[row | 1];
        float Fx_1 = opd1_column[row & ~1];
        float dFdx_1 = Fx_dx_1 - Fx_1;
        result1_column[row] = dFdx_1;

        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_dFdx_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result0_column = FLOAT_REG_PTR(x, 0);
  float *restrict result1_column = FLOAT_REG_PTR(x, 1);
  float *restrict result2_column = FLOAT_REG_PTR(x, 2);
  float *restrict opd0_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict opd1_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict opd2_column = FLOAT_REG_PTR(x->children_[0], 2);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        float *restrict result2 = result2_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        const float *restrict opd2 = opd2_column + row;
        int n;
        /* Fragment rows are ordered per square of 4 fragments, repeating
         * every 4 fragments. Top-Left %00, Top-Right %01, Bottom-Left %10, Bottom-Right %11.
         * Even if, in such a square, some fragments might be outside the primitive being rasterized, it
         * is still generated, and the fragment shader is executed (but masked, so it won't generate
         * output.) */
        for (n = 0; n < 8; n += 2) {
          float Fx_dx_0 = opd0[n + 1];
          float Fx_0 = opd0[n];
          float dFdx_0 = Fx_dx_0 - Fx_0;
          result0[n] = dFdx_0;
          result0[n + 1] = dFdx_0;

          float Fx_dx_1 = opd1[n + 1];
          float Fx_1 = opd1[n];
          float dFdx_1 = Fx_dx_1 - Fx_1;
          result1[n] = dFdx_1;
          result1[n + 1] = dFdx_1;

          float Fx_dx_2 = opd2[n + 1];
          float Fx_2 = opd2[n];
          float dFdx_2 = Fx_dx_2 - Fx_2;
          result2[n] = dFdx_2;
          result2[n + 1] = dFdx_2;
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        float *restrict result2 = result2_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        const float *restrict opd2 = opd2_column + row;
        int n;
        
        for (n = 0; n < 4; n++) {
          float Fx_dx_0 = opd0[n + 1];
          float Fx_0 = opd0[n];
          float dFdx_0 = Fx_dx_0 - Fx_0;
          result0[n] = dFdx_0;
          result0[n + 1] = dFdx_0;

          float Fx_dx_1 = opd1[n + 1];
          float Fx_1 = opd1[n];
          float dFdx_1 = Fx_dx_1 - Fx_1;
          result1[n] = dFdx_1;
          result1[n + 1] = dFdx_1;

          float Fx_dx_2 = opd2[n + 1];
          float Fx_2 = opd2[n];
          float dFdx_2 = Fx_dx_2 - Fx_2;
          result2[n] = dFdx_2;
          result2[n + 1] = dFdx_2;
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* We get here then in the square of fragments, not all four fragments have followed the
         * same execution path, with only some arriving at the dFdx, and others going elsewhere.
         * We will still execute as if they all arrived at the dFdx and rely on the undefined 
         * results producing at least *something* that can be reasoned about. */
        float Fx_dx_0 = opd0_column[row | 1];
        float Fx_0 = opd0_column[row & ~1];
        float dFdx_0 = Fx_dx_0 - Fx_0;
        result0_column[row] = dFdx_0;

        float Fx_dx_1 = opd1_column[row | 1];
        float Fx_1 = opd1_column[row & ~1];
        float dFdx_1 = Fx_dx_1 - Fx_1;
        result1_column[row] = dFdx_1;

        float Fx_dx_2 = opd2_column[row | 1];
        float Fx_2 = opd2_column[row & ~1];
        float dFdx_2 = Fx_dx_2 - Fx_2;
        result2_column[row] = dFdx_2;

        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_dFdx_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result0_column = FLOAT_REG_PTR(x, 0);
  float *restrict result1_column = FLOAT_REG_PTR(x, 1);
  float *restrict result2_column = FLOAT_REG_PTR(x, 2);
  float *restrict result3_column = FLOAT_REG_PTR(x, 3);
  float *restrict opd0_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict opd1_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict opd2_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict opd3_column = FLOAT_REG_PTR(x->children_[0], 3);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        float *restrict result2 = result2_column + row;
        float *restrict result3 = result3_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        const float *restrict opd2 = opd2_column + row;
        const float *restrict opd3 = opd3_column + row;
        int n;
        /* Fragment rows are ordered per square of 4 fragments, repeating
         * every 4 fragments. Top-Left %00, Top-Right %01, Bottom-Left %10, Bottom-Right %11.
         * Even if, in such a square, some fragments might be outside the primitive being rasterized, it
         * is still generated, and the fragment shader is executed (but masked, so it won't generate
         * output.) */
        for (n = 0; n < 8; n += 2) {
          float Fx_dx_0 = opd0[n + 1];
          float Fx_0 = opd0[n];
          float dFdx_0 = Fx_dx_0 - Fx_0;
          result0[n] = dFdx_0;
          result0[n + 1] = dFdx_0;

          float Fx_dx_1 = opd1[n + 1];
          float Fx_1 = opd1[n];
          float dFdx_1 = Fx_dx_1 - Fx_1;
          result1[n] = dFdx_1;
          result1[n + 1] = dFdx_1;

          float Fx_dx_2 = opd2[n + 1];
          float Fx_2 = opd2[n];
          float dFdx_2 = Fx_dx_2 - Fx_2;
          result2[n] = dFdx_2;
          result2[n + 1] = dFdx_2;
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        float *restrict result2 = result2_column + row;
        float *restrict result3 = result3_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        const float *restrict opd2 = opd2_column + row;
        const float *restrict opd3 = opd3_column + row;
        int n;
        
        for (n = 0; n < 4; n++) {
          float Fx_dx_0 = opd0[n + 1];
          float Fx_0 = opd0[n];
          float dFdx_0 = Fx_dx_0 - Fx_0;
          result0[n] = dFdx_0;
          result0[n + 1] = dFdx_0;

          float Fx_dx_1 = opd1[n + 1];
          float Fx_1 = opd1[n];
          float dFdx_1 = Fx_dx_1 - Fx_1;
          result1[n] = dFdx_1;
          result1[n + 1] = dFdx_1;

          float Fx_dx_2 = opd2[n + 1];
          float Fx_2 = opd2[n];
          float dFdx_2 = Fx_dx_2 - Fx_2;
          result2[n] = dFdx_2;
          result2[n + 1] = dFdx_2;

          float Fx_dx_3 = opd3[n + 1];
          float Fx_3 = opd3[n];
          float dFdx_3 = Fx_dx_3 - Fx_3;
          result3[n] = dFdx_3;
          result3[n + 1] = dFdx_3;
        }
        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* We get here then in the square of fragments, not all four fragments have followed the
         * same execution path, with only some arriving at the dFdx, and others going elsewhere.
         * We will still execute as if they all arrived at the dFdx and rely on the undefined 
         * results producing at least *something* that can be reasoned about. */
        float Fx_dx_0 = opd0_column[row | 1];
        float Fx_0 = opd0_column[row & ~1];
        float dFdx_0 = Fx_dx_0 - Fx_0;
        result0_column[row] = dFdx_0;

        float Fx_dx_1 = opd1_column[row | 1];
        float Fx_1 = opd1_column[row & ~1];
        float dFdx_1 = Fx_dx_1 - Fx_1;
        result1_column[row] = dFdx_1;

        float Fx_dx_2 = opd2_column[row | 1];
        float Fx_2 = opd2_column[row & ~1];
        float dFdx_2 = Fx_dx_2 - Fx_2;
        result2_column[row] = dFdx_2;

        float Fx_dx_3 = opd3_column[row | 1];
        float Fx_3 = opd3_column[row & ~1];
        float dFdx_3 = Fx_dx_3 - Fx_3;
        result3_column[row] = dFdx_3;

        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_dFdy_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict opd = opd_column + row;
        int n, k;
        /* Fragment rows are ordered per square of 4 fragments, repeating
         * every 4 fragments. Top-Left %00, Top-Right %01, Bottom-Left %10, Bottom-Right %11.
         * Even if, in such a square, some fragments might be outside the primitive being rasterized, it
         * is still generated, and the fragment shader is executed (but masked, so it won't generate
         * output.) */
        for (n = 0; n < 8; n += 4) {
          for (k = 0; k < 2; k++) {
            float Fv_dy = opd[n + k + 2];
            float Fv = opd[n + k];
            float dFdy = Fv_dy - Fv;
            result[n + k] = dFdy;
            result[n + k + 2] = dFdy;
          }
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict opd = opd_column + row;
        int k;

        for (k = 0; k < 2; k++) {
          float Fv_dy = opd[k + 2];
          float Fv = opd[k];
          float dFdy = Fv_dy - Fv;
          result[k] = dFdy;
          result[k + 2] = dFdy;
        }

        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* We get here then in the square of fragments, not all four fragments have followed the
         * same execution path, with only some arriving at the dFdy, and others going elsewhere.
         * We will still execute as if they all arrived at the dFdy and rely on the undefined
         * results producing at least *something* that can be reasoned about. */
        float Fv_dy = opd_column[row | 2];
        float Fv = opd_column[row & ~2];
        float dFdy = Fv_dy - Fv;
        result_column[row] = dFdy;

        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_dFdy_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result0_column = FLOAT_REG_PTR(x, 0);
  float *restrict result1_column = FLOAT_REG_PTR(x, 1);
  float *restrict opd0_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict opd1_column = FLOAT_REG_PTR(x->children_[0], 1);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        int n, k;
        /* Fragment rows are ordered per square of 4 fragments, repeating
         * every 4 fragments. Top-Left %00, Top-Right %01, Bottom-Left %10, Bottom-Right %11.
         * Even if, in such a square, some fragments might be outside the primitive being rasterized, it
         * is still generated, and the fragment shader is executed (but masked, so it won't generate
         * output.) */
        for (n = 0; n < 8; n += 4) {
          for (k = 0; k < 2; k++) {
            float Fv_dy_0 = opd0[n + k + 2];
            float Fv_0 = opd0[n + k];
            float dFdy_0 = Fv_dy_0 - Fv_0;
            result0[n + k] = dFdy_0;
            result0[n + k + 2] = dFdy_0;

            float Fv_dy_1 = opd1[n + k + 2];
            float Fv_1 = opd1[n + k];
            float dFdy_1 = Fv_dy_1 - Fv_1;
            result1[n + k] = dFdy_1;
            result1[n + k + 2] = dFdy_1;
          }
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        int k;

        for (k = 0; k < 2; k++) {
          float Fv_dy_0 = opd0[k + 2];
          float Fv_0 = opd0[k];
          float dFdy_0 = Fv_dy_0 - Fv_0;
          result0[k] = dFdy_0;
          result0[k + 2] = dFdy_0;

          float Fv_dy_1 = opd1[k + 2];
          float Fv_1 = opd1[k];
          float dFdy_1 = Fv_dy_1 - Fv_1;
          result1[k] = dFdy_1;
          result1[k + 2] = dFdy_1;
        }

        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* We get here then in the square of fragments, not all four fragments have followed the
         * same execution path, with only some arriving at the dFdy, and others going elsewhere.
         * We will still execute as if they all arrived at the dFdy and rely on the undefined
         * results producing at least *something* that can be reasoned about. */
        float Fv_dy_0 = opd0_column[row | 2];
        float Fv_0 = opd0_column[row & ~2];
        float dFdy_0 = Fv_dy_0 - Fv_0;
        result0_column[row] = dFdy_0;

        float Fv_dy_1 = opd1_column[row | 2];
        float Fv_1 = opd1_column[row & ~2];
        float dFdy_1 = Fv_dy_1 - Fv_1;
        result1_column[row] = dFdy_1;

        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_dFdy_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result0_column = FLOAT_REG_PTR(x, 0);
  float *restrict result1_column = FLOAT_REG_PTR(x, 1);
  float *restrict result2_column = FLOAT_REG_PTR(x, 2);
  float *restrict opd0_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict opd1_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict opd2_column = FLOAT_REG_PTR(x->children_[0], 2);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        float *restrict result2 = result2_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        const float *restrict opd2 = opd2_column + row;
        int n, k;
        /* Fragment rows are ordered per square of 4 fragments, repeating
         * every 4 fragments. Top-Left %00, Top-Right %01, Bottom-Left %10, Bottom-Right %11.
         * Even if, in such a square, some fragments might be outside the primitive being rasterized, it
         * is still generated, and the fragment shader is executed (but masked, so it won't generate
         * output.) */
        for (n = 0; n < 8; n += 4) {
          for (k = 0; k < 2; k++) {
            float Fv_dy_0 = opd0[n + k + 2];
            float Fv_0 = opd0[n + k];
            float dFdy_0 = Fv_dy_0 - Fv_0;
            result0[n + k] = dFdy_0;
            result0[n + k + 2] = dFdy_0;

            float Fv_dy_1 = opd1[n + k + 2];
            float Fv_1 = opd1[n + k];
            float dFdy_1 = Fv_dy_1 - Fv_1;
            result1[n + k] = dFdy_1;
            result1[n + k + 2] = dFdy_1;

            float Fv_dy_2 = opd2[n + k + 2];
            float Fv_2 = opd2[n + k];
            float dFdy_2 = Fv_dy_2 - Fv_2;
            result2[n + k] = dFdy_2;
            result2[n + k + 2] = dFdy_2;
          }
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        float *restrict result2 = result2_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        const float *restrict opd2 = opd2_column + row;
        int k;

        for (k = 0; k < 2; k++) {
          float Fv_dy_0 = opd0[k + 2];
          float Fv_0 = opd0[k];
          float dFdy_0 = Fv_dy_0 - Fv_0;
          result0[k] = dFdy_0;
          result0[k + 2] = dFdy_0;

          float Fv_dy_1 = opd1[k + 2];
          float Fv_1 = opd1[k];
          float dFdy_1 = Fv_dy_1 - Fv_1;
          result1[k] = dFdy_1;
          result1[k + 2] = dFdy_1;

          float Fv_dy_2 = opd2[k + 2];
          float Fv_2 = opd2[k];
          float dFdy_2 = Fv_dy_2 - Fv_2;
          result2[k] = dFdy_2;
          result2[k + 2] = dFdy_2;
        }

        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* We get here then in the square of fragments, not all four fragments have followed the
         * same execution path, with only some arriving at the dFdy, and others going elsewhere.
         * We will still execute as if they all arrived at the dFdy and rely on the undefined
         * results producing at least *something* that can be reasoned about. */
        float Fv_dy_0 = opd0_column[row | 2];
        float Fv_0 = opd0_column[row & ~2];
        float dFdy_0 = Fv_dy_0 - Fv_0;
        result0_column[row] = dFdy_0;

        float Fv_dy_1 = opd1_column[row | 2];
        float Fv_1 = opd1_column[row & ~2];
        float dFdy_1 = Fv_dy_1 - Fv_1;
        result1_column[row] = dFdy_1;

        float Fv_dy_2 = opd2_column[row | 2];
        float Fv_2 = opd2_column[row & ~2];
        float dFdy_2 = Fv_dy_2 - Fv_2;
        result2_column[row] = dFdy_2;

        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_dFdy_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result0_column = FLOAT_REG_PTR(x, 0);
  float *restrict result1_column = FLOAT_REG_PTR(x, 1);
  float *restrict result2_column = FLOAT_REG_PTR(x, 2);
  float *restrict result3_column = FLOAT_REG_PTR(x, 3);
  float *restrict opd0_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict opd1_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict opd2_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict opd3_column = FLOAT_REG_PTR(x->children_[0], 3);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        float *restrict result2 = result2_column + row;
        float *restrict result3 = result3_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        const float *restrict opd2 = opd2_column + row;
        const float *restrict opd3 = opd3_column + row;
        int n, k;
        /* Fragment rows are ordered per square of 4 fragments, repeating
         * every 4 fragments. Top-Left %00, Top-Right %01, Bottom-Left %10, Bottom-Right %11.
         * Even if, in such a square, some fragments might be outside the primitive being rasterized, it
         * is still generated, and the fragment shader is executed (but masked, so it won't generate
         * output.) */
        for (n = 0; n < 8; n += 4) {
          for (k = 0; k < 2; k++) {
            float Fv_dy_0 = opd0[n + k + 2];
            float Fv_0 = opd0[n + k];
            float dFdy_0 = Fv_dy_0 - Fv_0;
            result0[n + k] = dFdy_0;
            result0[n + k + 2] = dFdy_0;

            float Fv_dy_1 = opd1[n + k + 2];
            float Fv_1 = opd1[n + k];
            float dFdy_1 = Fv_dy_1 - Fv_1;
            result1[n + k] = dFdy_1;
            result1[n + k + 2] = dFdy_1;

            float Fv_dy_2 = opd2[n + k + 2];
            float Fv_2 = opd2[n + k];
            float dFdy_2 = Fv_dy_2 - Fv_2;
            result2[n + k] = dFdy_2;
            result2[n + k + 2] = dFdy_2;

            float Fv_dy_3 = opd3[n + k + 2];
            float Fv_3 = opd3[n + k];
            float dFdy_3 = Fv_dy_3 - Fv_3;
            result3[n + k] = dFdy_3;
            result3[n + k + 2] = dFdy_3;
          }
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        float *restrict result2 = result2_column + row;
        float *restrict result3 = result3_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        const float *restrict opd2 = opd2_column + row;
        const float *restrict opd3 = opd3_column + row;
        int k;

        for (k = 0; k < 2; k++) {
          float Fv_dy_0 = opd0[k + 2];
          float Fv_0 = opd0[k];
          float dFdy_0 = Fv_dy_0 - Fv_0;
          result0[k] = dFdy_0;
          result0[k + 2] = dFdy_0;

          float Fv_dy_1 = opd1[k + 2];
          float Fv_1 = opd1[k];
          float dFdy_1 = Fv_dy_1 - Fv_1;
          result1[k] = dFdy_1;
          result1[k + 2] = dFdy_1;

          float Fv_dy_2 = opd2[k + 2];
          float Fv_2 = opd2[k];
          float dFdy_2 = Fv_dy_2 - Fv_2;
          result2[k] = dFdy_2;
          result2[k + 2] = dFdy_2;

          float Fv_dy_3 = opd3[k + 2];
          float Fv_3 = opd3[k];
          float dFdy_3 = Fv_dy_3 - Fv_3;
          result3[k] = dFdy_3;
          result3[k + 2] = dFdy_3;
        }

        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* We get here then in the square of fragments, not all four fragments have followed the
         * same execution path, with only some arriving at the dFdy, and others going elsewhere.
         * We will still execute as if they all arrived at the dFdy and rely on the undefined
         * results producing at least *something* that can be reasoned about. */
        float Fv_dy_0 = opd0_column[row | 2];
        float Fv_0 = opd0_column[row & ~2];
        float dFdy_0 = Fv_dy_0 - Fv_0;
        result0_column[row] = dFdy_0;

        float Fv_dy_1 = opd1_column[row | 2];
        float Fv_1 = opd1_column[row & ~2];
        float dFdy_1 = Fv_dy_1 - Fv_1;
        result1_column[row] = dFdy_1;

        float Fv_dy_2 = opd2_column[row | 2];
        float Fv_2 = opd2_column[row & ~2];
        float dFdy_2 = Fv_dy_2 - Fv_2;
        result2_column[row] = dFdy_2;

        float Fv_dy_3 = opd3_column[row | 2];
        float Fv_3 = opd3_column[row & ~2];
        float dFdy_3 = Fv_dy_3 - Fv_3;
        result3_column[row] = dFdy_3;

        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_fwidth_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  float *restrict opd_column = FLOAT_REG_PTR(x->children_[0], 0);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict opd = opd_column + row;
        int n;
        /* Fragment rows are ordered per square of 4 fragments, repeating
         * every 4 fragments. Top-Left %00, Top-Right %01, Bottom-Left %10, Bottom-Right %11.
         * Even if, in such a square, some fragments might be outside the primitive being rasterized, it
         * is still generated, and the fragment shader is executed (but masked, so it won't generate
         * output.) */
        for (n = 0; n < 8; n++) {
          float Fv_dx = opd[n | 1];
          float Fvx = opd[n & ~1];
          float Fv_dy = opd[n | 2];
          float Fvy = opd[n & ~2];
          float dFdx = Fv_dx - Fvx;
          float dFdy = Fv_dy - Fvy;

          result[n] = fabsf(dFdx) + fabsf(dFdy);
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict opd = opd_column + row;
        int n;

        for (n = 0; n < 4; n++) {
          float Fv_dx = opd[n | 1];
          float Fvx = opd[n & ~1];
          float Fv_dy = opd[n | 2];
          float Fvy = opd[n & ~2];
          float dFdx = Fv_dx - Fvx;
          float dFdy = Fv_dy - Fvy;

          result[n] = fabsf(dFdx) + fabsf(dFdy);
        }

        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* We get here then in the square of fragments, not all four fragments have followed the
         * same execution path, with only some arriving at the dFdx, and others going elsewhere.
         * We will still execute as if they all arrived at the dFdx and rely on the undefined
         * results producing at least *something* that can be reasoned about. */
        float Fv_dx = opd_column[row | 1];
        float Fvx = opd_column[row & ~1];
        float Fv_dy = opd_column[row | 2];
        float Fvy = opd_column[row & ~2];
        float dFdx = Fv_dx - Fvx;
        float dFdy = Fv_dy - Fvy;
        result_column[row] = fabsf(dFdx) + fabsf(dFdy);

        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_fwidth_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result0_column = FLOAT_REG_PTR(x, 0);
  float *restrict result1_column = FLOAT_REG_PTR(x, 1);
  float *restrict opd0_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict opd1_column = FLOAT_REG_PTR(x->children_[0], 1);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        int n;
        /* Fragment rows are ordered per square of 4 fragments, repeating
         * every 4 fragments. Top-Left %00, Top-Right %01, Bottom-Left %10, Bottom-Right %11.
         * Even if, in such a square, some fragments might be outside the primitive being rasterized, it
         * is still generated, and the fragment shader is executed (but masked, so it won't generate
         * output.) */
        for (n = 0; n < 8; n++) {
          float Fv_dx_0 = opd0[n | 1];
          float Fvx_0 = opd0[n & ~1];
          float Fv_dy_0 = opd0[n | 2];
          float Fvy_0 = opd0[n & ~2];
          float dFdx_0 = Fv_dx_0 - Fvx_0;
          float dFdy_0 = Fv_dy_0 - Fvy_0;

          result0[n] = fabsf(dFdx_0) + fabsf(dFdy_0);

          float Fv_dx_1 = opd1[n | 1];
          float Fvx_1 = opd1[n & ~1];
          float Fv_dy_1 = opd1[n | 2];
          float Fvy_1 = opd1[n & ~2];
          float dFdx_1 = Fv_dx_1 - Fvx_1;
          float dFdy_1 = Fv_dy_1 - Fvy_1;

          result1[n] = fabsf(dFdx_1) + fabsf(dFdy_1);
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        int n;

        for (n = 0; n < 4; n++) {
          float Fv_dx_0 = opd0[n | 1];
          float Fvx_0 = opd0[n & ~1];
          float Fv_dy_0 = opd0[n | 2];
          float Fvy_0 = opd0[n & ~2];
          float dFdx_0 = Fv_dx_0 - Fvx_0;
          float dFdy_0 = Fv_dy_0 - Fvy_0;

          result0[n] = fabsf(dFdx_0) + fabsf(dFdy_0);

          float Fv_dx_1 = opd1[n | 1];
          float Fvx_1 = opd1[n & ~1];
          float Fv_dy_1 = opd1[n | 2];
          float Fvy_1 = opd1[n & ~2];
          float dFdx_1 = Fv_dx_1 - Fvx_1;
          float dFdy_1 = Fv_dy_1 - Fvy_1;

          result1[n] = fabsf(dFdx_1) + fabsf(dFdy_1);
        }

        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* We get here then in the square of fragments, not all four fragments have followed the
         * same execution path, with only some arriving at the dFdx, and others going elsewhere.
         * We will still execute as if they all arrived at the dFdx and rely on the undefined
         * results producing at least *something* that can be reasoned about. */
        float Fv_dx_0 = opd0_column[row | 1];
        float Fvx_0 = opd0_column[row & ~1];
        float Fv_dy_0 = opd0_column[row | 2];
        float Fvy_0 = opd0_column[row & ~2];
        float dFdx_0 = Fv_dx_0 - Fvx_0;
        float dFdy_0 = Fv_dy_0 - Fvy_0;
        result0_column[row] = fabsf(dFdx_0) + fabsf(dFdy_0);

        float Fv_dx_1 = opd1_column[row | 1];
        float Fvx_1 = opd1_column[row & ~1];
        float Fv_dy_1 = opd1_column[row | 2];
        float Fvy_1 = opd1_column[row & ~2];
        float dFdx_1 = Fv_dx_1 - Fvx_1;
        float dFdy_1 = Fv_dy_1 - Fvy_1;
        result1_column[row] = fabsf(dFdx_1) + fabsf(dFdy_1);

        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_fwidth_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result0_column = FLOAT_REG_PTR(x, 0);
  float *restrict result1_column = FLOAT_REG_PTR(x, 1);
  float *restrict result2_column = FLOAT_REG_PTR(x, 2);
  float *restrict opd0_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict opd1_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict opd2_column = FLOAT_REG_PTR(x->children_[0], 2);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        float *restrict result2 = result2_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        const float *restrict opd2 = opd2_column + row;
        int n;
        /* Fragment rows are ordered per square of 4 fragments, repeating
         * every 4 fragments. Top-Left %00, Top-Right %01, Bottom-Left %10, Bottom-Right %11.
         * Even if, in such a square, some fragments might be outside the primitive being rasterized, it
         * is still generated, and the fragment shader is executed (but masked, so it won't generate
         * output.) */
        for (n = 0; n < 8; n++) {
          float Fv_dx_0 = opd0[n | 1];
          float Fvx_0 = opd0[n & ~1];
          float Fv_dy_0 = opd0[n | 2];
          float Fvy_0 = opd0[n & ~2];
          float dFdx_0 = Fv_dx_0 - Fvx_0;
          float dFdy_0 = Fv_dy_0 - Fvy_0;

          result0[n] = fabsf(dFdx_0) + fabsf(dFdy_0);

          float Fv_dx_1 = opd1[n | 1];
          float Fvx_1 = opd1[n & ~1];
          float Fv_dy_1 = opd1[n | 2];
          float Fvy_1 = opd1[n & ~2];
          float dFdx_1 = Fv_dx_1 - Fvx_1;
          float dFdy_1 = Fv_dy_1 - Fvy_1;

          result1[n] = fabsf(dFdx_1) + fabsf(dFdy_1);

          float Fv_dx_2 = opd2[n | 1];
          float Fvx_2 = opd2[n & ~1];
          float Fv_dy_2 = opd2[n | 2];
          float Fvy_2 = opd2[n & ~2];
          float dFdx_2 = Fv_dx_2 - Fvx_2;
          float dFdy_2 = Fv_dy_2 - Fvy_2;

          result2[n] = fabsf(dFdx_2) + fabsf(dFdy_2);
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        float *restrict result2 = result2_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        const float *restrict opd2 = opd2_column + row;
        int n;

        for (n = 0; n < 4; n++) {
          float Fv_dx_0 = opd0[n | 1];
          float Fvx_0 = opd0[n & ~1];
          float Fv_dy_0 = opd0[n | 2];
          float Fvy_0 = opd0[n & ~2];
          float dFdx_0 = Fv_dx_0 - Fvx_0;
          float dFdy_0 = Fv_dy_0 - Fvy_0;

          result0[n] = fabsf(dFdx_0) + fabsf(dFdy_0);

          float Fv_dx_1 = opd1[n | 1];
          float Fvx_1 = opd1[n & ~1];
          float Fv_dy_1 = opd1[n | 2];
          float Fvy_1 = opd1[n & ~2];
          float dFdx_1 = Fv_dx_1 - Fvx_1;
          float dFdy_1 = Fv_dy_1 - Fvy_1;

          result1[n] = fabsf(dFdx_1) + fabsf(dFdy_1);

          float Fv_dx_2 = opd2[n | 1];
          float Fvx_2 = opd2[n & ~1];
          float Fv_dy_2 = opd2[n | 2];
          float Fvy_2 = opd2[n & ~2];
          float dFdx_2 = Fv_dx_2 - Fvx_2;
          float dFdy_2 = Fv_dy_2 - Fvy_2;

          result2[n] = fabsf(dFdx_2) + fabsf(dFdy_2);
        }

        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* We get here then in the square of fragments, not all four fragments have followed the
         * same execution path, with only some arriving at the dFdx, and others going elsewhere.
         * We will still execute as if they all arrived at the dFdx and rely on the undefined
         * results producing at least *something* that can be reasoned about. */
        float Fv_dx_0 = opd0_column[row | 1];
        float Fvx_0 = opd0_column[row & ~1];
        float Fv_dy_0 = opd0_column[row | 2];
        float Fvy_0 = opd0_column[row & ~2];
        float dFdx_0 = Fv_dx_0 - Fvx_0;
        float dFdy_0 = Fv_dy_0 - Fvy_0;
        result0_column[row] = fabsf(dFdx_0) + fabsf(dFdy_0);

        float Fv_dx_1 = opd1_column[row | 1];
        float Fvx_1 = opd1_column[row & ~1];
        float Fv_dy_1 = opd1_column[row | 2];
        float Fvy_1 = opd1_column[row & ~2];
        float dFdx_1 = Fv_dx_1 - Fvx_1;
        float dFdy_1 = Fv_dy_1 - Fvy_1;
        result1_column[row] = fabsf(dFdx_1) + fabsf(dFdy_1);

        float Fv_dx_2 = opd2_column[row | 1];
        float Fvx_2 = opd2_column[row & ~1];
        float Fv_dy_2 = opd2_column[row | 2];
        float Fvy_2 = opd2_column[row & ~2];
        float dFdx_2 = Fv_dx_2 - Fvx_2;
        float dFdy_2 = Fv_dy_2 - Fvy_2;
        result2_column[row] = fabsf(dFdx_2) + fabsf(dFdy_2);

        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

void builtin_fwidth_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result0_column = FLOAT_REG_PTR(x, 0);
  float *restrict result1_column = FLOAT_REG_PTR(x, 1);
  float *restrict result2_column = FLOAT_REG_PTR(x, 2);
  float *restrict result3_column = FLOAT_REG_PTR(x, 3);
  float *restrict opd0_column = FLOAT_REG_PTR(x->children_[0], 0);
  float *restrict opd1_column = FLOAT_REG_PTR(x->children_[0], 1);
  float *restrict opd2_column = FLOAT_REG_PTR(x->children_[0], 2);
  float *restrict opd3_column = FLOAT_REG_PTR(x->children_[0], 3);
  uint8_t row = exec_chain;

  for (;;) {
    uint64_t chain;
    uint8_t delta;

    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        float *restrict result2 = result2_column + row;
        float *restrict result3 = result3_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        const float *restrict opd2 = opd2_column + row;
        const float *restrict opd3 = opd3_column + row;
        int n;
        /* Fragment rows are ordered per square of 4 fragments, repeating
         * every 4 fragments. Top-Left %00, Top-Right %01, Bottom-Left %10, Bottom-Right %11.
         * Even if, in such a square, some fragments might be outside the primitive being rasterized, it
         * is still generated, and the fragment shader is executed (but masked, so it won't generate
         * output.) */
        for (n = 0; n < 8; n++) {
          float Fv_dx_0 = opd0[n | 1];
          float Fvx_0 = opd0[n & ~1];
          float Fv_dy_0 = opd0[n | 2];
          float Fvy_0 = opd0[n & ~2];
          float dFdx_0 = Fv_dx_0 - Fvx_0;
          float dFdy_0 = Fv_dy_0 - Fvy_0;

          result0[n] = fabsf(dFdx_0) + fabsf(dFdy_0);

          float Fv_dx_1 = opd1[n | 1];
          float Fvx_1 = opd1[n & ~1];
          float Fv_dy_1 = opd1[n | 2];
          float Fvy_1 = opd1[n & ~2];
          float dFdx_1 = Fv_dx_1 - Fvx_1;
          float dFdy_1 = Fv_dy_1 - Fvy_1;

          result1[n] = fabsf(dFdx_1) + fabsf(dFdy_1);

          float Fv_dx_2 = opd2[n | 1];
          float Fvx_2 = opd2[n & ~1];
          float Fv_dy_2 = opd2[n | 2];
          float Fvy_2 = opd2[n & ~2];
          float dFdx_2 = Fv_dx_2 - Fvx_2;
          float dFdy_2 = Fv_dy_2 - Fvy_2;

          result2[n] = fabsf(dFdx_2) + fabsf(dFdy_2);

          float Fv_dx_3 = opd3[n | 1];
          float Fvx_3 = opd3[n & ~1];
          float Fv_dy_3 = opd3[n | 2];
          float Fvy_3 = opd3[n & ~2];
          float dFdx_3 = Fv_dx_3 - Fvx_3;
          float dFdy_3 = Fv_dy_3 - Fvy_3;

          result3[n] = fabsf(dFdx_3) + fabsf(dFdy_3);
        }

        delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) break;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result0 = result0_column + row;
        float *restrict result1 = result1_column + row;
        float *restrict result2 = result2_column + row;
        float *restrict result3 = result3_column + row;
        const float *restrict opd0 = opd0_column + row;
        const float *restrict opd1 = opd1_column + row;
        const float *restrict opd2 = opd2_column + row;
        const float *restrict opd3 = opd3_column + row;
        int n;

        for (n = 0; n < 4; n++) {
          float Fv_dx_0 = opd0[n | 1];
          float Fvx_0 = opd0[n & ~1];
          float Fv_dy_0 = opd0[n | 2];
          float Fvy_0 = opd0[n & ~2];
          float dFdx_0 = Fv_dx_0 - Fvx_0;
          float dFdy_0 = Fv_dy_0 - Fvy_0;

          result0[n] = fabsf(dFdx_0) + fabsf(dFdy_0);

          float Fv_dx_1 = opd1[n | 1];
          float Fvx_1 = opd1[n & ~1];
          float Fv_dy_1 = opd1[n | 2];
          float Fvy_1 = opd1[n & ~2];
          float dFdx_1 = Fv_dx_1 - Fvx_1;
          float dFdy_1 = Fv_dy_1 - Fvy_1;

          result1[n] = fabsf(dFdx_1) + fabsf(dFdy_1);

          float Fv_dx_2 = opd2[n | 1];
          float Fvx_2 = opd2[n & ~1];
          float Fv_dy_2 = opd2[n | 2];
          float Fvy_2 = opd2[n & ~2];
          float dFdx_2 = Fv_dx_2 - Fvx_2;
          float dFdy_2 = Fv_dy_2 - Fvy_2;

          result2[n] = fabsf(dFdx_2) + fabsf(dFdy_2);

          float Fv_dx_3 = opd3[n | 1];
          float Fvx_3 = opd3[n & ~1];
          float Fv_dy_3 = opd3[n | 2];
          float Fvy_3 = opd3[n & ~2];
          float dFdx_3 = Fv_dx_3 - Fvx_3;
          float dFdy_3 = Fv_dy_3 - Fvy_3;

          result3[n] = fabsf(dFdx_3) + fabsf(dFdy_3);
        }

        delta = (chain & 0xFF000000) >> 24;
        if (!delta) break;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* We get here then in the square of fragments, not all four fragments have followed the
         * same execution path, with only some arriving at the dFdx, and others going elsewhere.
         * We will still execute as if they all arrived at the dFdx and rely on the undefined
         * results producing at least *something* that can be reasoned about. */
        float Fv_dx_0 = opd0_column[row | 1];
        float Fvx_0 = opd0_column[row & ~1];
        float Fv_dy_0 = opd0_column[row | 2];
        float Fvy_0 = opd0_column[row & ~2];
        float dFdx_0 = Fv_dx_0 - Fvx_0;
        float dFdy_0 = Fv_dy_0 - Fvy_0;
        result0_column[row] = fabsf(dFdx_0) + fabsf(dFdy_0);

        float Fv_dx_1 = opd1_column[row | 1];
        float Fvx_1 = opd1_column[row & ~1];
        float Fv_dy_1 = opd1_column[row | 2];
        float Fvy_1 = opd1_column[row & ~2];
        float dFdx_1 = Fv_dx_1 - Fvx_1;
        float dFdy_1 = Fv_dy_1 - Fvy_1;
        result1_column[row] = fabsf(dFdx_1) + fabsf(dFdy_1);

        float Fv_dx_2 = opd2_column[row | 1];
        float Fvx_2 = opd2_column[row & ~1];
        float Fv_dy_2 = opd2_column[row | 2];
        float Fvy_2 = opd2_column[row & ~2];
        float dFdx_2 = Fv_dx_2 - Fvx_2;
        float dFdy_2 = Fv_dy_2 - Fvy_2;
        result2_column[row] = fabsf(dFdx_2) + fabsf(dFdy_2);

        float Fv_dx_3 = opd3_column[row | 1];
        float Fvx_3 = opd3_column[row & ~1];
        float Fv_dy_3 = opd3_column[row | 2];
        float Fvy_3 = opd3_column[row & ~2];
        float dFdx_3 = Fv_dx_3 - Fvx_3;
        float dFdy_3 = Fv_dy_3 - Fvy_3;
        result3_column[row] = fabsf(dFdx_3) + fabsf(dFdy_3);

        delta = chain_column[row];
        if (!delta) break;
        row += delta;
      } while (row & 3);
    }
    if (!delta) break;
  }
}

