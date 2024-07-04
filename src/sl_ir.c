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

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
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

void sl_ir_negate(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *opd) {
  sl_reg_alloc_kind_t kind = EXPR_RVALUE(opd)->kind_;
  switch (kind) {
    case slrak_float:
    case slrak_vec2:
    case slrak_vec3:
    case slrak_vec4:
    case slrak_mat2:
    case slrak_mat3:
    case slrak_mat4: {
      size_t num_components = 0;
      switch (kind) {
        case slrak_float: num_components = 1; break;
        case slrak_vec2: num_components = 2; break;
        case slrak_vec3: num_components = 3; break;
        case slrak_vec4: num_components = 4; break;
        case slrak_mat2: num_components = 4; break;
        case slrak_mat3: num_components = 9; break;
        case slrak_mat4: num_components = 16; break;
      }
      size_t n;
      for (n = 0; n < num_components; ++n) {
        struct ir_instr *instr = ir_block_append_instr(blk, SLIR_NEG_F);
        int opd_reg = EXPR_RVALUE(opd)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(opd)->v_.regs_[n] : EXPR_RVALUE(opd)->v_.regs_[n];
        int dst_reg = dst->base_regs_.v_.regs_[n];
        ir_instr_append_use(instr, chain_reg);
        ir_instr_append_def(instr, ir_body_alloc_temp_banked_float(blk->body_, dst_reg));
        ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, opd_reg));
      }
      break;
    }
    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      size_t num_components = 0;
      switch (kind) {
        case slrak_int:  num_components = 1; break;
        case slrak_ivec2:num_components = 2; break;
        case slrak_ivec3:num_components = 3; break;
        case slrak_ivec4:num_components = 4; break;
      }
      size_t n;
      for (n = 0; n < num_components; ++n) {
        struct ir_instr *instr = ir_block_append_instr(blk, SLIR_NEG_I);
        int opd_reg = EXPR_RVALUE(opd)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(opd)->v_.regs_[n] : EXPR_RVALUE(opd)->v_.regs_[n];
        int dst_reg = dst->base_regs_.v_.regs_[n];
        ir_instr_append_use(instr, chain_reg);
        ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
        ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, opd_reg));
      }
      break;
    }
  }
}

void sl_ir_need_rvalue(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *x) {
  sl_reg_emit_move(blk, chain_reg, frame, &x->base_regs_, &x->offset_reg_, &x->rvalue_, NULL);
}

/* Writes code starting at the end of blk, that evaluates expression x and stores the
 * results in the reg_alloc of x. */
struct ir_block *sl_ir_expr(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *x) {
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
      sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[0]);
      sl_ir_negate(blk, chain_reg, frame, x, x->children_[0]);
      break;

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

  return blk;
}
