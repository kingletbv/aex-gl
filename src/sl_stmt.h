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

#ifndef SL_STMT_H
#define SL_STMT_H

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef SITUS_H_INCLUDED
#define SITUS_H_INCLUDED
#include "pp/situs.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct sl_expr;

typedef enum sl_stmt_kind {
  slsk_invalid,
  slsk_expression,
  slsk_if,
  slsk_while,
  slsk_do,
  slsk_for,
  slsk_continue,
  slsk_break,
  slsk_return,
  slsk_discard
} sl_stmt_kind_t;

struct sl_stmt {
  sl_stmt_kind_t kind_;

  /* Location in source of if, while, do, for, continue, break, return, discard. */
  struct situs keyword_loc_;

  /* Next, and previous in the parent's chain, and a link to the parent. */
  struct sl_stmt *next_, *prev_, *parent_;

  /* Prep expression for (x;;)
   * Also the initializer assignment for slsk_for()
   * and the expression to execute for slsk_expression. */
  struct sl_expr *prep_;

  /* Condition expression for if (x), while (x), do-while(x), for (;x;). */
  struct sl_expr *condition_;

  /* Post-loop expression for: for (;;x) */
  struct sl_expr *post_;

  /* True branch is the body of the if, while, do, for. */
  struct sl_stmt *true_branch_;

  /* False branch is the else branch of the if. */
  struct sl_stmt *false_branch_;

};

void sl_stmt_init(struct sl_stmt *s);
void sl_stmt_cleanup(struct sl_stmt *s);
void sl_stmt_detach(struct sl_stmt *child);

struct sl_stmt *sl_stmt_alloc(void);

/* sl_stmt_free() detaches s from whatever datastructure it is in (e.g. parent stmt and siblings),
 * and frees it. */
void sl_stmt_free(struct sl_stmt *s);

/* sl_stmt_free_list() takes stmt_list as a list of statements and frees all of them. The list may
 * be part of a grander datastructure (in which case the corresponding head of the list in that
 * datastructure will be set to NULL), or it may exist in isolation (e.g. the stmt_list is the
 * only pointer to the list.) */
void sl_stmt_free_list(struct sl_stmt *stmt_list);

/* sl_stmt_append() detached the child, and appends to the end of the list started by head.
 * The new head of the list is returned. */
struct sl_stmt *sl_stmt_append(struct sl_stmt *head, struct sl_stmt *child);

/* sl_stmt_append_list() joins two lists together and returns the new head of the list. */
struct sl_stmt *sl_stmt_append_list(struct sl_stmt *front, struct sl_stmt *back);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_STMT_H */
