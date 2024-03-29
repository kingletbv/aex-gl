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
typedef void (*builtin_eval_fn)(struct sl_type_base *tb, struct sl_expr *x, struct sl_expr_temp *r);

void builtin_not_implemented_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_not_implemented_eval(struct sl_type_base *tb, struct sl_expr *x, struct sl_expr_temp *r);

void builtin_sin_f_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_sin_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_sin_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_sin_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_sin_f_eval(struct sl_type_base *tb, struct sl_expr *x, struct sl_expr_temp *r);
void builtin_sin_v2_eval(struct sl_type_base *tb, struct sl_expr *x, struct sl_expr_temp *r);
void builtin_sin_v3_eval(struct sl_type_base *tb, struct sl_expr *x, struct sl_expr_temp *r);
void builtin_sin_v4_eval(struct sl_type_base *tb, struct sl_expr *x, struct sl_expr_temp *r);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BUILTINS_H */
