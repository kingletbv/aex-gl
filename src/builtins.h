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

#ifndef BUILTINS_H
#define BUILTINS_H

#ifdef __cplusplus
extern "C" {
#endif

struct sl_execution;
struct sl_expr;

typedef void (*builtin_runtime_fn)(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
typedef void (*builtin_eval_fn)(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_not_implemented_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_not_implemented_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);


/* Angle and Trigonometry functions */
void builtin_radians_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_radians_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_radians_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_radians_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_radians_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_radians_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_radians_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_radians_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_degrees_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_degrees_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_degrees_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_degrees_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_degrees_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_degrees_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_degrees_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_degrees_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_sin_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_sin_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_sin_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_sin_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_sin_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_sin_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_sin_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_sin_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_cos_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_cos_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_cos_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_cos_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_cos_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_cos_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_cos_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_cos_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_tan_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_tan_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_tan_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_tan_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_tan_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_tan_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_tan_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_tan_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_asin_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_asin_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_asin_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_asin_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_asin_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_asin_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_asin_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_asin_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_acos_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_acos_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_acos_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_acos_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_acos_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_acos_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_acos_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_acos_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_atan_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_atan_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_atan_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_atan_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_atan_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_atan_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_atan_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_atan_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_atan_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_atan_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_atan_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_atan_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_atan_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_atan_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_atan_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_atan_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);


/* Exponential functions */
void builtin_pow_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_pow_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_pow_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_pow_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_pow_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_pow_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_pow_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_pow_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_exp_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_exp_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_exp_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_exp_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_exp_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_exp_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_exp_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_exp_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_log_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_log_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_log_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_log_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_log_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_log_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_log_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_log_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_exp2_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_exp2_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_exp2_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_exp2_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_exp2_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_exp2_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_exp2_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_exp2_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_log2_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_log2_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_log2_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_log2_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_log2_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_log2_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_log2_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_log2_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_sqrt_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_sqrt_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_sqrt_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_sqrt_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_sqrt_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_sqrt_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_sqrt_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_sqrt_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_inversesqrt_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_inversesqrt_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_inversesqrt_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_inversesqrt_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_inversesqrt_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_inversesqrt_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_inversesqrt_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_inversesqrt_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

/* Common Functions */
void builtin_abs_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_abs_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_abs_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_abs_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_abs_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_abs_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_abs_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_abs_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_sign_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_sign_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_sign_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_sign_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_sign_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_sign_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_sign_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_sign_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_floor_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_floor_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_floor_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_floor_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_floor_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_floor_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_floor_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_floor_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_ceil_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_ceil_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_ceil_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_ceil_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_ceil_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_ceil_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_ceil_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_ceil_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_fract_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_fract_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_fract_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_fract_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_fract_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_fract_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_fract_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_fract_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_mod_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mod_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mod_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mod_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mod_v2f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mod_v3f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mod_v4f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mod_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_mod_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_mod_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_mod_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_mod_v2f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_mod_v3f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_mod_v4f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_min_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_min_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_min_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_min_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_min_v2f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_min_v3f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_min_v4f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_min_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_min_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_min_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_min_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_min_v2f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_min_v3f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_min_v4f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_max_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_max_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_max_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_max_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_max_v2f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_max_v3f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_max_v4f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_max_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_max_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_max_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_max_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_max_v2f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_max_v3f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_max_v4f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_clamp_fff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_clamp_v2v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_clamp_v3v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_clamp_v4v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_clamp_v2ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_clamp_v3ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_clamp_v4ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_clamp_fff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_clamp_v2v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_clamp_v3v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_clamp_v4v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_clamp_v2ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_clamp_v3ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_clamp_v4ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_mix_fff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mix_v2v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mix_v3v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mix_v4v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mix_v2v2f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mix_v3v3f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mix_v4v4f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_mix_fff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_mix_v2v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_mix_v3v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_mix_v4v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_mix_v2v2f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_mix_v3v3f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_mix_v4v4f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_step_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_step_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_step_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_step_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_step_fv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_step_fv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_step_fv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_step_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_step_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_step_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_step_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_step_fv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_step_fv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_step_fv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_smoothstep_fff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_smoothstep_v2v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_smoothstep_v3v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_smoothstep_v4v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_smoothstep_ffv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_smoothstep_ffv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_smoothstep_ffv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_smoothstep_fff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_smoothstep_v2v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_smoothstep_v3v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_smoothstep_v4v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_smoothstep_ffv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_smoothstep_ffv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_smoothstep_ffv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

/* Geometric Functions */
void builtin_length_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_length_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_length_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_length_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_length_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_length_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_length_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_length_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_distance_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_distance_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_distance_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_distance_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_distance_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_distance_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_distance_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_distance_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_dot_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_dot_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_dot_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_dot_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_dot_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_dot_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_dot_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_dot_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_cross_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_cross_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_normalize_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_normalize_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_normalize_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_normalize_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_normalize_f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_normalize_v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_normalize_v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_normalize_v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_faceforward_fff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_faceforward_v2v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_faceforward_v3v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_faceforward_v4v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_faceforward_fff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_faceforward_v2v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_faceforward_v3v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_faceforward_v4v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_reflect_ff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_reflect_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_reflect_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_reflect_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_reflect_ff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_reflect_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_reflect_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_reflect_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_refract_fff_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_refract_v2v2f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_refract_v3v3f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_refract_v4v4f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_refract_fff_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_refract_v2v2f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_refract_v3v3f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_refract_v4v4f_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

/* Matrix Functions */
void builtin_matrixCompMult_m2m2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_matrixCompMult_m3m3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_matrixCompMult_m4m4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_matrixCompMult_m2m2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_matrixCompMult_m3m3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_matrixCompMult_m4m4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

/* Vector Relational functions */
void builtin_lessThan_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_lessThan_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_lessThan_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_lessThan_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_lessThan_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_lessThan_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_lessThan_iv2iv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_lessThan_iv3iv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_lessThan_iv4iv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_lessThan_iv2iv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_lessThan_iv3iv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_lessThan_iv4iv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_lessThanEqual_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_lessThanEqual_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_lessThanEqual_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_lessThanEqual_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_lessThanEqual_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_lessThanEqual_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_lessThanEqual_iv2iv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_lessThanEqual_iv3iv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_lessThanEqual_iv4iv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_lessThanEqual_iv2iv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_lessThanEqual_iv3iv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_lessThanEqual_iv4iv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_greaterThan_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_greaterThan_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_greaterThan_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_greaterThan_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_greaterThan_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_greaterThan_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_greaterThan_iv2iv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_greaterThan_iv3iv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_greaterThan_iv4iv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_greaterThan_iv2iv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_greaterThan_iv3iv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_greaterThan_iv4iv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_greaterThanEqual_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_greaterThanEqual_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_greaterThanEqual_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_greaterThanEqual_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_greaterThanEqual_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_greaterThanEqual_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_greaterThanEqual_iv2iv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_greaterThanEqual_iv3iv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_greaterThanEqual_iv4iv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_greaterThanEqual_iv2iv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_greaterThanEqual_iv3iv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_greaterThanEqual_iv4iv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_equal_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_equal_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_equal_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_equal_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_equal_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_equal_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_equal_iv2iv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_equal_iv3iv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_equal_iv4iv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_equal_iv2iv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_equal_iv3iv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_equal_iv4iv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_equal_bv2bv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_equal_bv3bv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_equal_bv4bv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_equal_bv2bv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_equal_bv3bv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_equal_bv4bv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_notEqual_v2v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_notEqual_v3v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_notEqual_v4v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_notEqual_v2v2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_notEqual_v3v3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_notEqual_v4v4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_notEqual_iv2iv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_notEqual_iv3iv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_notEqual_iv4iv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_notEqual_iv2iv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_notEqual_iv3iv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_notEqual_iv4iv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_notEqual_bv2bv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_notEqual_bv3bv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_notEqual_bv4bv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_notEqual_bv2bv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_notEqual_bv3bv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_notEqual_bv4bv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_any_bv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_any_bv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_any_bv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_any_bv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_any_bv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_any_bv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_all_bv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_all_bv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_all_bv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_all_bv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_all_bv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_all_bv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_not_bv2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_not_bv3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_not_bv4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_not_bv2_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_not_bv3_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);
void builtin_not_bv4_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

void builtin_dFdx_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_dFdx_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_dFdx_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_dFdx_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_dFdy_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_dFdy_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_dFdy_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_dFdy_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_fwidth_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_fwidth_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_fwidth_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_fwidth_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BUILTINS_H */
