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

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef SL_EXPR_H_INCLUDED
#define SL_EXPR_H_INCLUDED
#include "sl_expr.h"
#endif

#ifndef SL_STMT_H_INCLUDED
#define SL_STMT_H_INCLUDED
#include "sl_stmt.h"
#endif

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
#endif

void sl_exec_f_add(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) left + right
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_f_sub(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) left - right
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_f_mul(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) left * right
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_f_div(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) left / right
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

static int sl_exec_reserve_n(struct sl_execution *exec, size_t n) {
  size_t new_num_needed = exec->num_execution_points_ + n;
  if (new_num_needed > exec->num_execution_points_allocated_) {
    size_t new_num_allocated = exec->num_execution_points_allocated_;
    /* Keep incrementing size until we have it, checking for overflow along the way */
    do {
      size_t new_new_num_allocated = new_num_allocated * 2 + 1;
      if (new_new_num_allocated <= new_num_allocated) {
        /* overflow */
        return -1;
      }
      new_num_allocated = new_new_num_allocated;
      if (new_num_allocated == 1) {
        new_num_allocated = 15;
      }
      if (new_num_allocated >= (SIZE_MAX / sizeof(struct sl_execution_point))) {
        /* overflow */
        return -1;
      }
    } while (new_num_allocated < new_num_needed);
    size_t new_size = new_num_allocated * sizeof(struct sl_execution_point);

    struct sl_execution_point *new_execution_points = (struct sl_execution_point *)realloc(exec->execution_points_, new_size);
    if (!new_execution_points) {
      /* no memory */
      return -1;
    }
    exec->execution_points_ = new_execution_points;
    exec->num_execution_points_allocated_ = new_num_allocated;
  }
  return 0;
}

static void sl_exec_clear_ep(struct sl_execution *exec, size_t ep_index) {
  struct sl_execution_point *ep = exec->execution_points_ + ep_index;
  ep->kind_ = SLEPK_NONE;
  memset(&ep->v_, 0, sizeof(ep->v_));
  ep->enter_chain_ = ep->revisit_chain_ = ep->post_chain_ = SL_EXEC_NO_CHAIN;
  ep->continue_chain_ptr_ = 0;
}

void sl_exec_set_expr(struct sl_execution *exec, size_t ep_index, struct sl_expr *expr, uint32_t chain, size_t continuation_ptr) {
  sl_exec_clear_ep(exec, ep_index);
  struct sl_execution_point *ep = exec->execution_points_ + ep_index;
  ep->kind_ = SLEPK_EXPR;
  ep->v_.expr_ = expr;
  ep->enter_chain_ = chain;
  ep->continue_chain_ptr_ = continuation_ptr;
}

int sl_exec_push_expr(struct sl_execution *exec, struct sl_expr *expr, uint32_t chain, size_t continuation_ptr) {
  int r;
  r = sl_exec_reserve_n(exec, 1);
  if (r) return r;
  
  sl_exec_set_expr(exec, exec->num_execution_points_++, expr, chain, continuation_ptr);
}

int sl_exec_run(struct sl_execution *exec) {
  int r;
  while (exec->num_execution_points_) {
    struct sl_execution_point *ep = exec->execution_points_ + exec->num_execution_points_;
    if (ep->kind_ == SLEPK_STMT) {
      switch (ep->v_.stmt_->kind_) {
        case slsk_expression: {
          if (ep->enter_chain_ != SL_EXEC_NO_CHAIN) {
            r = sl_exec_push_expr(exec, ep->v_.stmt_->expr_, ep->enter_chain_, ((uintptr_t)&ep->post_chain_) - (uintptr_t)exec->execution_points_);
            if (r) return r;
            ep = exec->execution_points_ + exec->num_execution_points_;
            ep->enter_chain_ = SL_EXEC_NO_CHAIN;
            break;  
          }
          break;
        }
        case slsk_if:
          break;
        case slsk_while:
          break;
        case slsk_do:
          break;
        case slsk_for:
          break;
        case slsk_continue:
          break;
        case slsk_break:
          break;
        case slsk_return:
          break;
        case slsk_discard:
          break;
        case slsk_compound:
          break;
      }
    }
    else if (ep->kind_ == SLEPK_EXPR) {
    }
  }
}
