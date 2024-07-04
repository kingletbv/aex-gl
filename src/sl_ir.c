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
#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef IR_H_INCLUDED
#define IR_H_INCLUDED
#include "ir.h"
#endif

#ifndef SL_IR_H_INCLUDED
#define SL_IR_H_INCLUDED
#include "sl_ir.h"
#endif

#ifndef SL_EXPR_H_INCLUDED
#define SL_EXPR_H_INCLUDED
#include "sl_expr.h"
#endif

#ifndef SL_REG_MOVE_H_INCLUDED
#define SL_REG_MOVE_H_INCLUDED
#include "sl_reg_move.h"
#endif

void sl_ir_x(struct ir_body *body) {
  struct ir_block *eval_condition = ir_body_alloc_block(body);
  // .. eval condition here in eval_condition ..
  struct ir_instr *split_instr = ir_block_append_instr(eval_condition, SLIR_SPLIT_EXEC_CHAIN_BY_CONDITION);
  
  struct ir_temp *condition_var = NULL; /* SL bool reg */
  struct ir_temp *exec_chain = NULL; /* Exec chain coming in */
  struct ir_temp *true_chain = NULL; /* Exec chain where condition_var is true (regular int) */
  struct ir_temp *false_chain = NULL; /* Exec chain where condition_var is false (regular int) */
  ir_instr_append_def(split_instr, true_chain);
  ir_instr_append_def(split_instr, false_chain);
  ir_instr_append_use(split_instr, condition_var);
  ir_instr_append_use(split_instr, exec_chain);
  
  struct ir_instr *cmp_instr = ir_block_append_instr(eval_condition, GIR_COMPARE);
  ir_instr_append_use(cmp_instr, true_chain);
  struct ir_temp *NO_CHAIN = ir_body_alloc_temp(body);
  ir_instr_append_use(cmp_instr, NO_CHAIN);

  struct ir_block *true_branch = ir_body_alloc_block(body);
  struct ir_block *false_branch = ir_body_alloc_block(body);
  struct ir_block *after = ir_body_alloc_block(body);

  struct ir_instr *beq_instr = ir_block_append_instr(eval_condition, GIR_BRANCH_NOT_EQUAL);
  ir_instr_append_use(beq_instr, ir_body_alloc_temp_block(body, true_branch));
  ir_instr_append_use(beq_instr, ir_body_alloc_temp_block(body, false_branch));


}

void sl_ir_need_rvalue(struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct ir_block *blk, struct sl_expr *x) {
  sl_reg_emit_move(blk, chain_reg, frame, &x->base_regs_, &x->offset_reg_, &x->rvalue_, NULL);
}

/* Writes code starting at the end of blk, that evaluates expression x and stores the
 * results in the reg_alloc of x. */
struct ir_block *sl_ir_expr(struct ir_body *body, struct ir_block *blk, struct sl_expr *x) {
  if (!x) return blk;
  switch (x->op_) {
    case exop_variable:
    case exop_literal:
    case exop_array_subscript:
    case exop_component_selection: /* e.g. myvec3.xxz */
    case exop_field_selection:     /* e.g. mystruct.myfield */
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

    case exop_sequence:    // comma operator

    case exop_conditional:  // ternary ?: operator
      ;
  }
}
