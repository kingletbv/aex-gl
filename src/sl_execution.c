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

void sl_exec_init(struct sl_execution *exec) {
  exec->num_execution_points_ = exec->num_execution_points_allocated_ = 0;
  exec->execution_points_ = NULL;
  exec->num_float_regs_ = 0;
  exec->float_regs_ = NULL;
  exec->num_int_regs_ = 0;
  exec->int_regs_ = NULL;
  exec->num_bool_regs_ = 0;
  exec->bool_regs_ = NULL;
}

void sl_exec_cleanup(struct sl_execution *exec) {
  if (exec->execution_points_) free(exec->execution_points_);
  if (exec->float_regs_) free(exec->float_regs_);
  if (exec->int_regs_) free(exec->int_regs_);
  if (exec->bool_regs_) free(exec->bool_regs_);
}

int sl_exec_prep(struct sl_execution *exec, struct sl_reg_allocator *rac) {
  void *new_float_regs = NULL;
  void *new_int_regs = NULL;
  void *new_bool_regs = NULL;
  void *new_sampler2D_regs = NULL;
  void *new_samplerCube_regs = NULL;
  if (rac->current_max_float_reg_) {
    new_float_regs = malloc(sizeof(float *) * rac->current_max_float_reg_);
    if (!new_float_regs) goto fail;
    memset(new_float_regs, 0, sizeof(float *) * rac->current_max_float_reg_);
  }
  if (rac->current_max_int_reg_) {
    new_int_regs = malloc(sizeof(int *) * rac->current_max_int_reg_);
    if (!new_int_regs) goto fail;
    memset(new_int_regs, 0, sizeof(int *) * rac->current_max_int_reg_);
  }
  if (rac->current_max_bool_reg_) {
    new_bool_regs = malloc(sizeof(unsigned char *) * rac->current_max_bool_reg_);
    if (!new_bool_regs) goto fail;
    memset(new_bool_regs, 0, sizeof(int *) * rac->current_max_bool_reg_);
  }
  if (rac->current_max_sampler2D_reg_) {
    new_sampler2D_regs = malloc(sizeof(void *) * rac->current_max_sampler2D_reg_);
    if (!new_sampler2D_regs) goto fail;
    memset(new_sampler2D_regs, 0, sizeof(void *) * rac->current_max_sampler2D_reg_);
  }
  if (rac->current_max_samplerCube_reg_) {
    new_samplerCube_regs = malloc(sizeof(void *) * rac->current_max_samplerCube_reg_);
    if (!new_samplerCube_regs) goto fail;
    memset(new_samplerCube_regs, 0, sizeof(void *) * rac->current_max_samplerCube_reg_);
  }

  if (exec->float_regs_) free(exec->float_regs_);
  if (exec->int_regs_) free(exec->int_regs_);
  if (exec->bool_regs_) free(exec->bool_regs_);
  if (exec->sampler_2D_regs_) free(exec->sampler_2D_regs_);
  if (exec->sampler_cube_regs_) free(exec->sampler_cube_regs_);

  exec->num_float_regs_ = (size_t)rac->current_max_float_reg_;
  exec->float_regs_ = (float **)new_float_regs;

  exec->num_int_regs_ = (size_t)rac->current_max_int_reg_;
  exec->int_regs_ = (int **)new_int_regs;

  exec->num_bool_regs_ = (size_t)rac->current_max_bool_reg_;
  exec->bool_regs_ = (unsigned char **)new_bool_regs;

  exec->num_sampler_2D_regs_ = (size_t)rac->current_max_sampler2D_reg_;
  exec->sampler_2D_regs_ = (void **)new_sampler2D_regs;

  exec->num_sampler_cube_regs_ = (size_t)rac->current_max_samplerCube_reg_;
  exec->sampler_cube_regs_ = (void **)new_samplerCube_regs;

  return 0;
fail:
  if (new_float_regs) free(new_float_regs);
  if (new_int_regs) free(new_int_regs);
  if (new_bool_regs) free(new_bool_regs);
  if (new_sampler2D_regs) free(new_sampler2D_regs);
  if (new_samplerCube_regs) free(new_samplerCube_regs);

  return -1;
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

  return 0;
}

uint32_t sl_exec_join_chains(struct sl_execution *exec, uint32_t a, uint32_t b) {
  /* Join chain starting at a and chain starting at b, into a single chain, the start
   * of which is returned. This is like a merge sort in that we need to maintain
   * the relative order.
   * Sentinel value inside the execution chain is 0, however, this cannot be used
   * to indicate an empty list as row 0 is a valid row. Consequently, to indicate
   * "no list", a or b should be set to SL_EXEC_NO_CHAIN. This is also why their
   * type is uint32_t. */
  uint8_t *ec = exec->exec_chain_reg_;

  uint32_t r;
  if (a == SL_EXEC_NO_CHAIN) return b;
  if (b == SL_EXEC_NO_CHAIN) return a;

  if (a < b) {
    r = a;
    uint8_t step = ec[a];
    if (step) {
      a += step;
    }
    else {
      a = SL_EXEC_NO_CHAIN;
    }
  }
  else if (a > b) {
    r = b;
    uint8_t step = ec[b];
    if (step) {
      b += step;
    }
    else {
      b = SL_EXEC_NO_CHAIN;
    }
  }
  else /* (a == b) */ {
    /* No merging necessary when they're the same chain */
    return a;
  }

  uint32_t q;
  q = r;
  for (;;) {
    if (a < b) {
      ec[q] = (uint8_t)(a - q);
      q = a;
      uint8_t step = ec[a];
      if (step) a += step;
      else a = SL_EXEC_NO_CHAIN;
    }
    else if (a > b) {
      ec[q] = (uint8_t)(b - q);
      q = b;
      uint8_t step = ec[b];
      if (step) b += step;
      else b = SL_EXEC_NO_CHAIN;
    }
    else /* (a == b) */ {
      /* This happens when both a and b are SL_EXEC_NO_CHAIN, it could also happen
       * when a or b are part of the same list (in which case the earlier one
       * will catch up to the later one until they are identical) - that latter
       * case means there's no further point in merging (as they're already 
       * merged. */
      
      /* No need to terminate ec[q] as it is guaranteed to be the tail of one or
       * the other list (and so is already 0) */
      return q;
    }
  }
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
      switch (ep->v_.expr_->op_) {
        case exop_invalid:

        case exop_variable:
        case exop_literal:
        case exop_array_subscript:
        case exop_component_selection:
        case exop_field_selection:
        case exop_post_inc:
        case exop_post_dec:
        case exop_pre_inc:
        case exop_pre_dec:

        case exop_negate:
        case exop_logical_not:

        case exop_multiply:
        case exop_divide:

        case exop_add:
        case exop_subtract:

        case exop_lt:
        case exop_le:
        case exop_ge:
        case exop_gt:

        case exop_eq:
        case exop_ne:

        case exop_function_call:
        case exop_constructor:

        case exop_logical_and:
        case exop_logical_or:
        case exop_logical_xor:

        case exop_assign:
        case exop_mul_assign:
        case exop_div_assign:
        case exop_add_assign:
        case exop_sub_assign:

        case exop_sequence:

        case exop_conditional:
          ;
      }
    }
  }
}
