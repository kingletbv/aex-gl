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

#ifndef SL_STMT_H_INCLUDED
#define SL_STMT_H_INCLUDED
#include "sl_stmt.h"
#endif

#ifndef SL_EXPR_H_INCLUDED
#define SL_EXPR_H_INCLUDED
#include "sl_expr.h"
#endif

#ifndef SYM_TABLE_H_INCLUDED
#define SYM_TABLE_H_INCLUDED
#include "sym_table.h"
#endif

void sl_stmt_init(struct sl_stmt *s) {
  s->kind_ = slsk_invalid;
  situs_init(&s->keyword_loc_);
  s->next_ = s->prev_ = NULL;
  s->parent_ = NULL;
  s->expr_ = s->condition_ = s->post_ = NULL;
  s->prep_ = NULL;
  s->prep_cond_ = NULL;
  s->true_branch_ = NULL;
  s->false_branch_ = NULL;
  s->scope_ = NULL;
}

void sl_stmt_cleanup(struct sl_stmt *s) {
  sl_stmt_detach(s);
  sl_expr_free(s->expr_);
  sl_expr_free(s->condition_);
  sl_expr_free(s->post_);
  while (s->prep_) sl_stmt_cleanup(s->prep_);
  while (s->prep_cond_) sl_stmt_cleanup(s->prep_cond_);
  while (s->true_branch_) sl_stmt_cleanup(s->true_branch_);
  while (s->false_branch_) sl_stmt_cleanup(s->false_branch_);
  if (s->scope_) {
    st_cleanup(s->scope_);
    free(s->scope_);
  }
}

void sl_stmt_detach(struct sl_stmt *child) {
  if (!child || !child->next_) {
    return;
  }
  if (child->parent_ && child->parent_->true_branch_ == child) {
    child->parent_->true_branch_ = (child->next_ == child) ? NULL : child->next_;
  }
  else if (child->parent_ && child->parent_->false_branch_ == child) {
    child->parent_->false_branch_ = (child->next_ == child) ? NULL : child->next_;
  }
  else if (child->parent_ && child->parent_->prep_ == child) {
    child->parent_->prep_ = (child->next_ == child) ? NULL : child->next_;
  }
  child->next_->prev_ = child->prev_;
  child->prev_->next_ = child->next_;
  child->next_ = child->prev_ = NULL;
}

struct sl_stmt *sl_stmt_append(struct sl_stmt *head, struct sl_stmt *child) {
  if (child->next_) {
    sl_stmt_detach(child);
  }
  if (!head) {
    child->next_ = child->prev_ = child;
    return child;
  }
  child->next_ = head;
  child->prev_ = head->prev_;
  child->next_->prev_ = child->prev_->next_ = child;
  return head;
}

struct sl_stmt *sl_stmt_append_list(struct sl_stmt *front, struct sl_stmt *back) {
  if (!back) return front;
  if (!front) return back;
  struct sl_stmt *front_head = front;
  struct sl_stmt *front_tail = front->prev_;
  struct sl_stmt *back_head = back;
  struct sl_stmt *back_tail = back->prev_;

  back_tail->next_ = front_head;
  front_head->prev_ = back_tail;

  front_tail->next_ = back_head;
  back_head->prev_ = front_tail;

  return front_head;
}

void sl_stmt_set_list_parent(struct sl_stmt *list, struct sl_stmt *parent) {
  struct sl_stmt *s = list;
  if (s) {
    do {
      s->parent_ = parent;

      s = s->next_;
    } while (s != list);
  }
}

struct sl_stmt *sl_stmt_alloc(void) {
  struct sl_stmt *s = malloc(sizeof(*s));
  if (!s) return NULL;
  sl_stmt_init(s);
  return s;
}

void sl_stmt_free(struct sl_stmt *s) {
  if (!s) return;
  sl_stmt_cleanup(s);
  free(s);
}

void sl_stmt_free_list(struct sl_stmt *stmt_list) {
  struct sl_stmt *s = stmt_list;
  if (s) {
    struct sl_stmt *next = s;
    /* Free the next head of the list until the
     * node we freed had a next pointer to itself.
     * (Thus implying it was the last node of the
     * circular list.)
     */
    do {
      s = next;
      next = s->next_;

      sl_stmt_free(s);
    } while ((next != s) && next);
  }
}

