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
struct sym_table;

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
  slsk_discard,
  slsk_compound
} sl_stmt_kind_t;

struct sl_stmt {
  sl_stmt_kind_t kind_;

  /* Location in source of if, while, do, for, continue, break, return, discard. */
  struct situs keyword_loc_;

  /* Next, and previous in the parent's chain, and a link to the parent. */
  struct sl_stmt *next_, *prev_, *parent_;

  /* The expression to execute for slsk_expression (which can be NULL if the expression is empty, just a semicolon.)
   * and the value (if any) to return for slsk_return. */
  struct sl_expr *expr_;

  /* Condition expression for if (x), while (x), do-while(x), for (;x;). */
  struct sl_expr *condition_;

  /* Post-loop expression for: for (;;x) */
  struct sl_expr *post_;

  /* Preparation statements for slsk_for's init-stmt ("for (int x=1;;)", the assignment "x=1"
   * goes here.) */
  struct sl_stmt *prep_;

  /* Preparation statement for slsk_for's condition ("for (;x;)") - it is legal for that to be
   * a declaration with initializer, in which case there is an initializer assignment to be executed 
   * prior to each iteration's condition evaluation. 
   */
  struct sl_stmt *prep_cond_;

  /* True branch is the body of the if, while, do, for. */
  struct sl_stmt *true_branch_;

  /* False branch is the else branch of the if. */
  struct sl_stmt *false_branch_;

  /* Scope block for compound statements.
   * slsk_compound: the scope for the compound statement, may be NULL. 
   * slsk_for: the scope for the for statement and its body, starting with the condition (e.g. "for (int x=0; ;) .. body ..", the
   *           "int x" goes into this symtable and the scope is used in the for's body.. That the for's body uses the same
   *           scope has semantic consequences, e.g. "for (int x;;) int x = 10; <-- Error, duplicate identfier "x" declaration"
   *           instead of hiding the original "x" declaration, it conflicts, because both are in the same scope. Note that the
   *           spec specifies the body of the for loop as "statement-no-new-scope" - this can be either a statement (e.g. 
   *           the declaration-statement "int x = 10;" above), or, confusingly, a compound-statement-with-scope (e.g. "{ int x = 10; }"
   *           so while "for (int x=0;;) int x=4;" is invalid for the scoping reasons outlined, "for (int x=0;;) { int x=4; }" is valid.
   */
  struct sym_table *scope_;
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

/* Sets the parent_ member of the entire list to parent; use this when assigning an entire
 * list to the prep_, true_branch_, or false_branch_ members. */
void sl_stmt_set_list_parent(struct sl_stmt *list, struct sl_stmt *parent);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_STMT_H */
