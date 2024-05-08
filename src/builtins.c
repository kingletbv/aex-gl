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
