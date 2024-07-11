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

static void sl_ir_negate(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *opd) {
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
        int dst_reg = dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[n] : dst->base_regs_.v_.regs_[n];
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
        int dst_reg = dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[n] : dst->base_regs_.v_.regs_[n];
        ir_instr_append_use(instr, chain_reg);
        ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
        ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, opd_reg));
      }
      break;
    }
  }
}

static void sl_ir_not(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *opd) {
  size_t num_components = 0;
  switch (EXPR_RVALUE(opd)->kind_) {
    case slrak_bool:  num_components = 1; break;
    case slrak_bvec2: num_components = 2; break;
    case slrak_bvec3: num_components = 3; break;
    case slrak_bvec4: num_components = 4; break;
  }
  size_t n;
  for (n = 0; n < num_components; ++n) {
    struct ir_instr *instr = ir_block_append_instr(blk, SLIR_NOT);
    int opd_reg = EXPR_RVALUE(opd)->local_frame_ ? frame->local_bool_offset_ + EXPR_RVALUE(opd)->v_.regs_[n] : EXPR_RVALUE(opd)->v_.regs_[n];
    int dst_reg = dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[n] : dst->base_regs_.v_.regs_[n];
    ir_instr_append_use(instr, chain_reg);
    ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst_reg));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_bool(blk->body_, opd_reg));
  }
}

static void sl_ir_f_mul(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int left_reg, int right_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_MUL_F);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_float(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, left_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, right_reg));
}

static void sl_ir_i_mul(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int left_reg, int right_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_MUL_I);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, left_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, right_reg));
}

static void sl_ir_dot_product2(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int left0_reg, int left1_reg, int right0_reg, int right1_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_DOT2);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, left0_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, left1_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, right0_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, right1_reg));
}

static void sl_ir_dot_product3(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int left0_reg, int left1_reg, int left2_reg, int right0_reg, int right1_reg, int right2_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_DOT3);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, left0_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, left1_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, left2_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, right0_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, right1_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, right2_reg));
}

static void sl_ir_dot_product4(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int left0_reg, int left1_reg, int left2_reg, int left3_reg, int right0_reg, int right1_reg, int right2_reg, int right3_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_DOT4);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, left0_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, left1_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, left2_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, left3_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, right0_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, right1_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, right2_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, right3_reg));
}

static void sl_ir_mul(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  int r, c;
  sl_reg_alloc_kind_t left_kind = EXPR_RVALUE(left)->kind_;
  sl_reg_alloc_kind_t right_kind = EXPR_RVALUE(right)->kind_;
  if (left_kind == right_kind) {
    switch (left_kind) {
      case slrak_float:
        sl_ir_f_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
        break;
      case slrak_int:
        sl_ir_i_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
        break;
      case slrak_vec2:
        sl_ir_f_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
        sl_ir_f_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[1] : dst->base_regs_.v_.regs_[1],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[1] : EXPR_RVALUE(left)->v_.regs_[1],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[1] : EXPR_RVALUE(right)->v_.regs_[1]);
        break;
      case slrak_vec3:
        sl_ir_f_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
        sl_ir_f_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[1] : dst->base_regs_.v_.regs_[1],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[1] : EXPR_RVALUE(left)->v_.regs_[1],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[1] : EXPR_RVALUE(right)->v_.regs_[1]);
        sl_ir_f_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[2] : dst->base_regs_.v_.regs_[2],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[2] : EXPR_RVALUE(left)->v_.regs_[2],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[2] : EXPR_RVALUE(right)->v_.regs_[2]);
        break;
      case slrak_vec4:
        sl_ir_f_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
        sl_ir_f_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[1] : dst->base_regs_.v_.regs_[1],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[1] : EXPR_RVALUE(left)->v_.regs_[1],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[1] : EXPR_RVALUE(right)->v_.regs_[1]);
        sl_ir_f_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[2] : dst->base_regs_.v_.regs_[2],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[2] : EXPR_RVALUE(left)->v_.regs_[2],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[2] : EXPR_RVALUE(right)->v_.regs_[2]);
        sl_ir_f_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[3] : dst->base_regs_.v_.regs_[3],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[3] : EXPR_RVALUE(left)->v_.regs_[3],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[3] : EXPR_RVALUE(right)->v_.regs_[3]);
        break;
      case slrak_ivec2:
        sl_ir_i_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
        sl_ir_i_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[1] : dst->base_regs_.v_.regs_[1],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[1] : EXPR_RVALUE(left)->v_.regs_[1],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[1] : EXPR_RVALUE(right)->v_.regs_[1]);
        break;
      case slrak_ivec3:
        sl_ir_i_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
        sl_ir_i_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[1] : dst->base_regs_.v_.regs_[1],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[1] : EXPR_RVALUE(left)->v_.regs_[1],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[1] : EXPR_RVALUE(right)->v_.regs_[1]);
        sl_ir_i_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[2] : dst->base_regs_.v_.regs_[2],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[2] : EXPR_RVALUE(left)->v_.regs_[2],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[2] : EXPR_RVALUE(right)->v_.regs_[2]);
        break;
      case slrak_ivec4:
        sl_ir_i_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
        sl_ir_i_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[1] : dst->base_regs_.v_.regs_[1],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[1] : EXPR_RVALUE(left)->v_.regs_[1],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[1] : EXPR_RVALUE(right)->v_.regs_[1]);
        sl_ir_i_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[2] : dst->base_regs_.v_.regs_[2],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[2] : EXPR_RVALUE(left)->v_.regs_[2],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[2] : EXPR_RVALUE(right)->v_.regs_[2]);
        sl_ir_i_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[3] : dst->base_regs_.v_.regs_[3],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[3] : EXPR_RVALUE(left)->v_.regs_[3],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[3] : EXPR_RVALUE(right)->v_.regs_[3]);
        break;
      case slrak_mat2:
        for (c = 0; c < 2; ++c) {
          for (r = 0; r < 2; ++r) {
            sl_ir_dot_product2(blk, chain_reg,
                               dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[c*2+r] : dst->base_regs_.v_.regs_[c*2+r],
                               EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r+0] : EXPR_RVALUE(left)->v_.regs_[r+0],
                               EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r+2] : EXPR_RVALUE(left)->v_.regs_[r+2],
                               EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[c*2 + 0] : EXPR_RVALUE(right)->v_.regs_[c*2 + 0],
                               EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[c*2 + 1] : EXPR_RVALUE(right)->v_.regs_[c*2 + 1]);
          }
        }
        break;
      case slrak_mat3:
        for (c = 0; c < 3; ++c) {
          for (r = 0; r < 3; ++r) {
            sl_ir_dot_product3(blk, chain_reg,
                               dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[c*3+r] : dst->base_regs_.v_.regs_[c*3+r],
                               EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r+0] : EXPR_RVALUE(left)->v_.regs_[r+0],
                               EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r+3] : EXPR_RVALUE(left)->v_.regs_[r+3],
                               EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r+6] : EXPR_RVALUE(left)->v_.regs_[r+6],
                               EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[c*3 + 0] : EXPR_RVALUE(right)->v_.regs_[c*3 + 0],
                               EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[c*3 + 1] : EXPR_RVALUE(right)->v_.regs_[c*3 + 1],
                               EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[c*3 + 2] : EXPR_RVALUE(right)->v_.regs_[c*3 + 2]);
          }
        }
        break;
      case slrak_mat4:
        for (c = 0; c < 4; ++c) {
          for (r = 0; r < 4; ++r) {
            sl_ir_dot_product4(blk, chain_reg,
                               dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[c*4+r] : dst->base_regs_.v_.regs_[c*4+r],
                               EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r+0] : EXPR_RVALUE(left)->v_.regs_[r+0],
                               EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r+4] : EXPR_RVALUE(left)->v_.regs_[r+4],
                               EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r+8] : EXPR_RVALUE(left)->v_.regs_[r+8],
                               EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r+12] : EXPR_RVALUE(left)->v_.regs_[r+12],
                               EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[c*4 + 0] : EXPR_RVALUE(right)->v_.regs_[c*4 + 0],
                               EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[c*4 + 1] : EXPR_RVALUE(right)->v_.regs_[c*4 + 1],
                               EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[c*4 + 2] : EXPR_RVALUE(right)->v_.regs_[c*4 + 2],
                               EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[c*4 + 3] : EXPR_RVALUE(right)->v_.regs_[c*4 + 3]);
          }
        }
        break;
    }
  }
  else if (left_kind == slrak_float) {
    int num_coords = 0;
    switch (right_kind) {
      case slrak_vec2: num_coords = 2; break;
      case slrak_vec3: num_coords = 3; break;
      case slrak_vec4: num_coords = 4; break;
      case slrak_mat2: num_coords = 4; break;
      case slrak_mat3: num_coords = 9; break;
      case slrak_mat4: num_coords = 16; break;
    }
    for (r = 0; r < num_coords; ++r) {
      sl_ir_f_mul(blk, chain_reg,  
                  dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
    }
  }
  else if (right_kind == slrak_float) {
    int num_coords = 0;
    switch (left_kind) {
      case slrak_vec2: num_coords = 2; break;
      case slrak_vec3: num_coords = 3; break;
      case slrak_vec4: num_coords = 4; break;
      case slrak_mat2: num_coords = 4; break;
      case slrak_mat3: num_coords = 9; break;
      case slrak_mat4: num_coords = 16; break;
    }
    for (r = 0; r < num_coords; ++r) {
      sl_ir_f_mul(blk, chain_reg,  
                  dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
    }
  }
  else if (left_kind == slrak_int) {
    int num_coords = 0;
    switch (right_kind) {
      case slrak_ivec2: num_coords = 2; break;
      case slrak_ivec3: num_coords = 3; break;
      case slrak_ivec4: num_coords = 4; break;
    }
    for (r = 0; r < num_coords; ++r) {
        sl_ir_i_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
    }
  }
  else if (right_kind == slrak_int) {
    int num_coords = 0;
    switch (left_kind) {
      case slrak_ivec2: num_coords = 2; break;
      case slrak_ivec3: num_coords = 3; break;
      case slrak_ivec4: num_coords = 4; break;
    }
    for (r = 0; r < num_coords; ++r) {
        sl_ir_i_mul(blk, chain_reg,  
                    dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                    EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                    EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
    }
  }
  else if ((left_kind == slrak_vec2) && (right_kind == slrak_mat2)) {
    for (r = 0; r < 2; ++r) {
      sl_ir_dot_product2(blk, chain_reg,
                         dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[1] : EXPR_RVALUE(left)->v_.regs_[1],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r*2 + 0] : EXPR_RVALUE(right)->v_.regs_[r*2 + 0],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r*2 + 1] : EXPR_RVALUE(right)->v_.regs_[r*2 + 1]);
    }
  }
  else if ((left_kind == slrak_mat2) && (right_kind == slrak_vec2)) {
    for (r = 0; r < 2; ++r) {
      sl_ir_dot_product2(blk, chain_reg,
                         dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0 + r] : EXPR_RVALUE(left)->v_.regs_[0 + r],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[2 + r] : EXPR_RVALUE(left)->v_.regs_[2 + r],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[1] : EXPR_RVALUE(right)->v_.regs_[1]);
    }
  }
  else if ((left_kind == slrak_vec3) && (right_kind == slrak_mat3)) {
    for (r = 0; r < 3; ++r) {
      sl_ir_dot_product3(blk, chain_reg,
                         dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[1] : EXPR_RVALUE(left)->v_.regs_[1],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[2] : EXPR_RVALUE(left)->v_.regs_[2],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r*3 + 0] : EXPR_RVALUE(right)->v_.regs_[r*3 + 0],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r*3 + 1] : EXPR_RVALUE(right)->v_.regs_[r*3 + 1],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r*3 + 2] : EXPR_RVALUE(right)->v_.regs_[r*3 + 2]);
    }
  }
  else if ((left_kind == slrak_mat3) && (right_kind == slrak_vec3)) {
    for (r = 0; r < 3; ++r) {
      sl_ir_dot_product3(blk, chain_reg,
                         dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0 + r] : EXPR_RVALUE(left)->v_.regs_[0 + r],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[3 + r] : EXPR_RVALUE(left)->v_.regs_[3 + r],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[6 + r] : EXPR_RVALUE(left)->v_.regs_[6 + r],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[1] : EXPR_RVALUE(right)->v_.regs_[1],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[2] : EXPR_RVALUE(right)->v_.regs_[2]);
    }
  }
  else if ((left_kind == slrak_vec4) && (right_kind == slrak_mat4)) {
    for (r = 0; r < 4; ++r) {
      sl_ir_dot_product4(blk, chain_reg,
                         dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[1] : EXPR_RVALUE(left)->v_.regs_[1],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[2] : EXPR_RVALUE(left)->v_.regs_[2],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[3] : EXPR_RVALUE(left)->v_.regs_[3],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r*4 + 0] : EXPR_RVALUE(right)->v_.regs_[r*4 + 0],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r*4 + 1] : EXPR_RVALUE(right)->v_.regs_[r*4 + 1],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r*4 + 2] : EXPR_RVALUE(right)->v_.regs_[r*4 + 2],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r*4 + 3] : EXPR_RVALUE(right)->v_.regs_[r*4 + 3]);
    }
  }
  else if ((left_kind == slrak_mat4) && (right_kind == slrak_vec4)) {
    for (r = 0; r < 4; ++r) {
      sl_ir_dot_product4(blk, chain_reg,
                         dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0 + r] : EXPR_RVALUE(left)->v_.regs_[0 + r],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[4 + r] : EXPR_RVALUE(left)->v_.regs_[3 + r],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[8 + r] : EXPR_RVALUE(left)->v_.regs_[6 + r],
                         EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[12+ r] : EXPR_RVALUE(left)->v_.regs_[12+ r],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[1] : EXPR_RVALUE(right)->v_.regs_[1],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[2] : EXPR_RVALUE(right)->v_.regs_[2],
                         EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[3] : EXPR_RVALUE(right)->v_.regs_[3]);
    }
  }
}

static void sl_ir_f_div(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int left_reg, int right_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_DIV_F);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_float(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, left_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, right_reg));
}

static void sl_ir_i_div(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int left_reg, int right_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_DIV_I);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, left_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, right_reg));
}

static void sl_ir_div(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  int r;

  sl_reg_alloc_kind_t left_kind = EXPR_RVALUE(left)->kind_;
  sl_reg_alloc_kind_t right_kind = EXPR_RVALUE(right)->kind_;

  if (left_kind == right_kind) {
    switch (left_kind) {
      case slrak_float:
      case slrak_vec2:
      case slrak_vec3:
      case slrak_vec4:
      case slrak_mat2:
      case slrak_mat3:
      case slrak_mat4: {
        int num_components = 0;
        switch (left_kind) {
          case slrak_float: num_components = 1; break;
          case slrak_vec2: num_components = 2; break;
          case slrak_vec3: num_components = 3; break;
          case slrak_vec4: num_components = 4; break;
          case slrak_mat2: num_components = 2; break;
          case slrak_mat3: num_components = 3; break;
          case slrak_mat4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_ir_f_div(blk, chain_reg, 
                      dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                      EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                      EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
        }
        break;
      }
      case slrak_int:
      case slrak_ivec2:
      case slrak_ivec3:
      case slrak_ivec4: {
        int num_components = 0;
        switch (left_kind) {
          case slrak_int:   num_components = 1; break;
          case slrak_ivec2: num_components = 2; break;
          case slrak_ivec3: num_components = 3; break;
          case slrak_ivec4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_ir_i_div(blk, chain_reg, 
                      dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                      EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                      EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
        }
        break;
      }
    }
  }
  else if (left_kind == slrak_float) {
    int num_components = 0;
    switch (right_kind) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_ir_f_div(blk, chain_reg, 
                  dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
    }
  }
  else if (right_kind == slrak_float) {
    int num_components = 0;
    switch (left_kind) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_ir_f_div(blk, chain_reg, 
                  dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
    }
  }
  else if (left_kind == slrak_int) {
    int num_components = 0;
    switch (right_kind) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_ir_i_div(blk, chain_reg, 
                  dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
    }
  }
  else if (right_kind == slrak_int) {
    int num_components = 0;
    switch (left_kind) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_ir_i_div(blk, chain_reg, 
                  dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
    }
  }
}

static void sl_ir_f_add(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int left_reg, int right_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_ADD_F);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_float(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, left_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, right_reg));
}

static void sl_ir_i_add(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int left_reg, int right_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_ADD_I);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, left_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, right_reg));
}

static void sl_ir_add(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  int r;

  sl_reg_alloc_kind_t left_kind = EXPR_RVALUE(left)->kind_;
  sl_reg_alloc_kind_t right_kind = EXPR_RVALUE(right)->kind_;
  if (left_kind == right_kind) {
    switch (left_kind) {
      case slrak_float:
      case slrak_vec2:
      case slrak_vec3:
      case slrak_vec4:
      case slrak_mat2:
      case slrak_mat3:
      case slrak_mat4: {
        int num_components = 0;
        switch (left_kind) {
          case slrak_float: num_components = 1; break;
          case slrak_vec2: num_components = 2; break;
          case slrak_vec3: num_components = 3; break;
          case slrak_vec4: num_components = 4; break;
          case slrak_mat2: num_components = 2; break;
          case slrak_mat3: num_components = 3; break;
          case slrak_mat4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_ir_f_add(blk, chain_reg, 
                      dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                      EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                      EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
        }
        break;
      }
      case slrak_int:
      case slrak_ivec2:
      case slrak_ivec3:
      case slrak_ivec4: {
        int num_components = 0;
        switch (left_kind) {
          case slrak_int:   num_components = 1; break;
          case slrak_ivec2: num_components = 2; break;
          case slrak_ivec3: num_components = 3; break;
          case slrak_ivec4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_ir_i_add(blk, chain_reg, 
                      dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                      EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                      EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
        }
        break;
      }
    }
  }
  else if (left_kind == slrak_float) {
    int num_components = 0;
    switch (right_kind) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_ir_f_add(blk, chain_reg, 
                  dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
    }
  }
  else if (right_kind == slrak_float) {
    int num_components = 0;
    switch (left_kind) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_ir_f_add(blk, chain_reg, 
                  dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
    }
  }
  else if (left_kind == slrak_int) {
    int num_components = 0;
    switch (right_kind) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_ir_i_add(blk, chain_reg, 
                  dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
    }
  }
  else if (right_kind == slrak_int) {
    int num_components = 0;
    switch (left_kind) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_ir_i_add(blk, chain_reg, 
                  dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
    }
  }
}

static void sl_ir_f_sub(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int left_reg, int right_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_SUB_F);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_float(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, left_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, right_reg));
}

static void sl_ir_i_sub(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int left_reg, int right_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_SUB_I);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, left_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, right_reg));
}

static void sl_ir_sub(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  int r;

  sl_reg_alloc_kind_t left_kind = EXPR_RVALUE(left)->kind_;
  sl_reg_alloc_kind_t right_kind = EXPR_RVALUE(right)->kind_;
  if (left_kind == right_kind) {
    switch (left_kind) {
      case slrak_float:
      case slrak_vec2:
      case slrak_vec3:
      case slrak_vec4:
      case slrak_mat2:
      case slrak_mat3:
      case slrak_mat4: {
        int num_components = 0;
        switch (left_kind) {
          case slrak_float: num_components = 1; break;
          case slrak_vec2: num_components = 2; break;
          case slrak_vec3: num_components = 3; break;
          case slrak_vec4: num_components = 4; break;
          case slrak_mat2: num_components = 2; break;
          case slrak_mat3: num_components = 3; break;
          case slrak_mat4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_ir_f_sub(blk, chain_reg, 
                      dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                      EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                      EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
        }
        break;
      }
      case slrak_int:
      case slrak_ivec2:
      case slrak_ivec3:
      case slrak_ivec4: {
        int num_components = 0;
        switch (left_kind) {
          case slrak_int:   num_components = 1; break;
          case slrak_ivec2: num_components = 2; break;
          case slrak_ivec3: num_components = 3; break;
          case slrak_ivec4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_ir_i_sub(blk, chain_reg, 
                      dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                      EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                      EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
        }
        break;
      }
    }
  }
  else if (left_kind == slrak_float) {
    int num_components = 0;
    switch (right_kind) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_ir_f_sub(blk, chain_reg, 
                  dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
    }
  }
  else if (right_kind == slrak_float) {
    int num_components = 0;
    switch (left_kind) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_ir_f_sub(blk, chain_reg, 
                  dst->base_regs_.local_frame_ ? frame->local_float_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
    }
  }
  else if (left_kind == slrak_int) {
    int num_components = 0;
    switch (right_kind) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_ir_i_sub(blk, chain_reg, 
                  dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[r] : EXPR_RVALUE(right)->v_.regs_[r]);
    }
  }
  else if (right_kind == slrak_int) {
    int num_components = 0;
    switch (left_kind) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_ir_i_sub(blk, chain_reg, 
                  dst->base_regs_.local_frame_ ? frame->local_int_offset_ + dst->base_regs_.v_.regs_[r] : dst->base_regs_.v_.regs_[r],
                  EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[r] : EXPR_RVALUE(left)->v_.regs_[r],
                  EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]);
    }
  }
}

static void sl_ir_lt(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  struct ir_instr *instr = NULL;

  if (left->base_regs_.kind_ == slrak_float) {
    instr = ir_block_append_instr(blk, SLIR_LT_F);
    ir_instr_append_use(instr, chain_reg);
    ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]));
  }
  else if (left->base_regs_.kind_ == slrak_int) {
    instr = ir_block_append_instr(blk, SLIR_LT_I);
    ir_instr_append_use(instr, chain_reg);
    ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]));
  }
  else {
    return;
  }
}

static void sl_ir_le(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  struct ir_instr *instr = NULL;

  if (left->base_regs_.kind_ == slrak_float) {
    instr = ir_block_append_instr(blk, SLIR_LE_F);
    ir_instr_append_use(instr, chain_reg);
    ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]));
  }
  else if (left->base_regs_.kind_ == slrak_int) {
    instr = ir_block_append_instr(blk, SLIR_LE_I);
    ir_instr_append_use(instr, chain_reg);
    ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]));
  }
  else {
    return;
  }
}

static void sl_ir_ge(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  struct ir_instr *instr = NULL;

  if (left->base_regs_.kind_ == slrak_float) {
    instr = ir_block_append_instr(blk, SLIR_GE_F);
    ir_instr_append_use(instr, chain_reg);
    ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]));
  }
  else if (left->base_regs_.kind_ == slrak_int) {
    instr = ir_block_append_instr(blk, SLIR_GE_I);
    ir_instr_append_use(instr, chain_reg);
    ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]));
  }
  else {
    return;
  }
}

static void sl_ir_gt(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  struct ir_instr *instr = NULL;

  if (left->base_regs_.kind_ == slrak_float) {
    instr = ir_block_append_instr(blk, SLIR_GT_F);
    ir_instr_append_use(instr, chain_reg);
    ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]));
  }
  else if (left->base_regs_.kind_ == slrak_int) {
    instr = ir_block_append_instr(blk, SLIR_GT_I);
    ir_instr_append_use(instr, chain_reg);
    ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]));
  }
  else {
    return;
  }
}

static void sl_ir_eq(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  struct ir_instr *instr = NULL;

  if (left->base_regs_.kind_ == slrak_float) {
    instr = ir_block_append_instr(blk, SLIR_EQ_F);
    ir_instr_append_use(instr, chain_reg);
    ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]));
  }
  else if (left->base_regs_.kind_ == slrak_int) {
    instr = ir_block_append_instr(blk, SLIR_EQ_I);
    ir_instr_append_use(instr, chain_reg);
    ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]));
  }
  else {
    return;
  }
}

static void sl_ir_ne(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  struct ir_instr *instr = NULL;

  if (left->base_regs_.kind_ == slrak_float) {
    instr = ir_block_append_instr(blk, SLIR_NE_F);
    ir_instr_append_use(instr, chain_reg);
    ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, EXPR_RVALUE(left)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, EXPR_RVALUE(right)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]));
  }
  else if (left->base_regs_.kind_ == slrak_int) {
    instr = ir_block_append_instr(blk, SLIR_NE_I);
    ir_instr_append_use(instr, chain_reg);
    ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, EXPR_RVALUE(left)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0]));
    ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, EXPR_RVALUE(right)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]));
  }
  else {
    return;
  }
}

static void sl_ir_log_xor(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_LOG_XOR);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_bool(blk->body_, dst->base_regs_.local_frame_ ? frame->local_bool_offset_ + dst->base_regs_.v_.regs_[0] : dst->base_regs_.v_.regs_[0]));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_bool(blk->body_, EXPR_RVALUE(left)->local_frame_ ? frame->local_bool_offset_ + EXPR_RVALUE(left)->v_.regs_[0] : EXPR_RVALUE(left)->v_.regs_[0]));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_bool(blk->body_, EXPR_RVALUE(right)->local_frame_ ? frame->local_bool_offset_ + EXPR_RVALUE(right)->v_.regs_[0] : EXPR_RVALUE(right)->v_.regs_[0]));
}

static void sl_ir_f_inc(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int opd_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_INC_F);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_float(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, opd_reg));
}

static void sl_ir_i_inc(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int opd_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_INC_I);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, opd_reg));
}

static void sl_ir_inc(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_reg_alloc *dst_ra, struct sl_expr *opd) {
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
        sl_ir_f_inc(blk, chain_reg,  
                    dst_ra->local_frame_ ? frame->local_float_offset_ + dst_ra->v_.regs_[n] : dst_ra->v_.regs_[n],
                    EXPR_RVALUE(opd)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(opd)->v_.regs_[n] : EXPR_RVALUE(opd)->v_.regs_[n]);
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
        sl_ir_f_inc(blk, chain_reg, 
                    dst_ra->local_frame_ ? frame->local_int_offset_ + dst_ra->v_.regs_[n] : dst_ra->v_.regs_[n],
                    EXPR_RVALUE(opd)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(opd)->v_.regs_[n] : EXPR_RVALUE(opd)->v_.regs_[n]);
      }
      break;
    }
  }
}

static void sl_ir_f_dec(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int opd_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_DEC_F);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_float(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_float(blk->body_, opd_reg));
}

static void sl_ir_i_dec(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int opd_reg) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_DEC_I);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, opd_reg));
}

static void sl_ir_dec(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_reg_alloc *dst_ra, struct sl_expr *opd) {
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
        sl_ir_f_dec(blk, chain_reg, 
                    dst_ra->local_frame_ ? frame->local_float_offset_ + dst_ra->v_.regs_[n] : dst_ra->v_.regs_[n],
                    EXPR_RVALUE(opd)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(opd)->v_.regs_[n] : EXPR_RVALUE(opd)->v_.regs_[n]);
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
        sl_ir_f_dec(blk, chain_reg, 
                    dst_ra->local_frame_ ? frame->local_int_offset_ + dst_ra->v_.regs_[n] : dst_ra->v_.regs_[n],
                    EXPR_RVALUE(opd)->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(opd)->v_.regs_[n] : EXPR_RVALUE(opd)->v_.regs_[n]);
      }
      break;
    }
  }
}

static void sl_ir_f_init_lit(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, float lit) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_INIT_LIT_F);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_litf(blk->body_, lit));
}

static void sl_ir_i_init_lit(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int64_t lit) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_INIT_LIT_I);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_liti(blk->body_, lit));
}

static void sl_ir_b_init_lit(struct ir_block *blk, struct ir_temp *chain_reg, int dst_reg, int lit) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_INIT_LIT_F);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_litb(blk->body_, lit));
}

static void sl_ir_init_lit(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_reg_alloc *dst, struct sl_expr_temp *src, int offset) {
  int num_components = 0;
  int n;

  switch (src->kind_) {
    case slrak_array: {
      if ((INT_MAX / dst->v_.array_.num_elements_) < offset) {
        /* overflow */
        return;
      }
      int array_start = offset * (int)dst->v_.array_.num_elements_;
      for (n = 0; n < dst->v_.array_.num_elements_; ++n) {
        sl_ir_init_lit(blk, chain_reg, frame, dst, src->v_.comp_.elements_ + n, array_start + n);
      }
      break;
    }
    case slrak_struct: {
      size_t index;
      for (index = 0; index < src->v_.comp_.num_elements_; ++index) {
        sl_ir_init_lit(blk, chain_reg, frame, dst->v_.comp_.fields_ + index, src->v_.comp_.elements_ + index, offset);
      }
      break;
    }
    case slrak_float:
      sl_ir_f_init_lit(blk, chain_reg, dst->local_frame_ ? frame->local_float_offset_ + dst->v_.regs_[0] + offset : dst->v_.regs_[0] + offset, src->v_.f_);
      break;
    case slrak_vec2:
    case slrak_vec3:
    case slrak_vec4: {
      switch (src->kind_) {
        case slrak_vec2:  num_components = 2; break;
        case slrak_vec3:  num_components = 3; break;
        case slrak_vec4:  num_components = 4; break;
        case slrak_mat2:  num_components = 4; break;
        case slrak_mat3:  num_components = 9; break;
        case slrak_mat4:  num_components = 16; break;
          break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_ir_f_init_lit(blk, chain_reg, dst->local_frame_ ? frame->local_float_offset_ + dst->v_.regs_[n] + offset : dst->v_.regs_[n] + offset, src->v_.v_[n]);
      }
      break;
    }
    case slrak_mat2:
    case slrak_mat3:
    case slrak_mat4: {
      switch (src->kind_) {
        case slrak_float: num_components = 1; break;
        case slrak_vec2:  num_components = 2; break;
        case slrak_vec3:  num_components = 3; break;
        case slrak_vec4:  num_components = 4; break;
        case slrak_mat2:  num_components = 4; break;
        case slrak_mat3:  num_components = 9; break;
        case slrak_mat4:  num_components = 16; break;
          break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_ir_f_init_lit(blk, chain_reg, dst->local_frame_ ? frame->local_float_offset_ + dst->v_.regs_[n] + offset : dst->v_.regs_[n] + offset, src->v_.m_[n]);
      }
      break;
    }
    case slrak_int:
      sl_ir_i_init_lit(blk, chain_reg, dst->local_frame_ ? frame->local_int_offset_ + dst->v_.regs_[0] + offset : dst->v_.regs_[0] + offset, src->v_.i_);
      break;
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      switch (src->kind_) {
        case slrak_ivec2: num_components = 2; break;
        case slrak_ivec3: num_components = 3; break;
        case slrak_ivec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_ir_i_init_lit(blk, chain_reg, dst->local_frame_ ? frame->local_int_offset_ + dst->v_.regs_[n] + offset : dst->v_.regs_[n] + offset, src->v_.iv_[n]);
      }
      break;
    }
    case slrak_bool:
      sl_ir_b_init_lit(blk, chain_reg, dst->local_frame_ ? frame->local_bool_offset_ + dst->v_.regs_[0] + offset : dst->v_.regs_[0] + offset, src->v_.b_);
      break;
    case slrak_bvec2:
    case slrak_bvec3:
    case slrak_bvec4: {
      switch (src->kind_) {
        case slrak_bvec2: num_components = 2; break;
        case slrak_bvec3: num_components = 3; break;
        case slrak_bvec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_ir_b_init_lit(blk, chain_reg, dst->local_frame_ ? frame->local_bool_offset_ + dst->v_.regs_[n] + offset : dst->v_.regs_[n] + offset, src->v_.bv_[n]);
      }
      break;
    }
  }
}

static void sl_ir_i_mul_constant_and_add(struct ir_block *blk, struct ir_temp *chain_reg, 
                                         int dst_reg, int src_base, uint64_t src_base_multiplier, int offset) {
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_MUL_C_AND_ADD);
  ir_instr_append_use(instr, chain_reg);
  ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_reg));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, src_base));
  ir_instr_append_use(instr, ir_body_alloc_temp_liti(blk->body_, src_base_multiplier));
  ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, offset));
}

static void sl_ir_component_subscript(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_expr *dst, struct sl_expr *arr, struct sl_expr *subscript) {
  struct sl_reg_alloc *dst_ra = &dst->base_regs_;
  struct sl_reg_alloc *array_ra = EXPR_RVALUE(arr);
  struct sl_reg_alloc *subscript_ra = EXPR_RVALUE(subscript);
  switch (array_ra->kind_) {
    case slrak_vec2:
    case slrak_ivec2:
    case slrak_bvec2: {
      struct ir_instr *instr;
      instr = ir_block_append_instr(blk, array_ra->is_indirect_ ? SLIR_PICK_FROM_2_INDIRECT : SLIR_PICK_FROM_2);
      ir_instr_append_use(instr, chain_reg);
      ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, subscript_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[1]));
      break;
    }
    case slrak_vec3:
    case slrak_ivec3:
    case slrak_bvec3: {
      struct ir_instr *instr;
      instr = ir_block_append_instr(blk, array_ra->is_indirect_ ? SLIR_PICK_FROM_3_INDIRECT : SLIR_PICK_FROM_3);
      ir_instr_append_use(instr, chain_reg);
      ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, subscript_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[1]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[2]));
      break;
    }
    case slrak_vec4:
    case slrak_ivec4:
    case slrak_bvec4: {
      struct ir_instr *instr;
      instr = ir_block_append_instr(blk, array_ra->is_indirect_ ? SLIR_PICK_FROM_4_INDIRECT : SLIR_PICK_FROM_4);
      ir_instr_append_use(instr, chain_reg);
      ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, subscript_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[1]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[2]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[3]));
      break;
    }
    case slrak_mat2: {
      struct ir_instr *instr;
      instr = ir_block_append_instr(blk, array_ra->is_indirect_ ? SLIR_PICK_FROM_2_INDIRECT : SLIR_PICK_FROM_2);
      ir_instr_append_use(instr, chain_reg);
      ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, subscript_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[2]));

      instr = ir_block_append_instr(blk, array_ra->is_indirect_ ? SLIR_PICK_FROM_2_INDIRECT : SLIR_PICK_FROM_2);
      ir_instr_append_use(instr, chain_reg);
      ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_ra->v_.regs_[1]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, subscript_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[1]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[3]));
    }
    case slrak_mat3: {
      struct ir_instr *instr;
      instr = ir_block_append_instr(blk, array_ra->is_indirect_ ? SLIR_PICK_FROM_3_INDIRECT : SLIR_PICK_FROM_3);
      ir_instr_append_use(instr, chain_reg);
      ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, subscript_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[3]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[9]));

      instr = ir_block_append_instr(blk, array_ra->is_indirect_ ? SLIR_PICK_FROM_3_INDIRECT : SLIR_PICK_FROM_3);
      ir_instr_append_use(instr, chain_reg);
      ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_ra->v_.regs_[1]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, subscript_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[1]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[4]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[10]));

      instr = ir_block_append_instr(blk, array_ra->is_indirect_ ? SLIR_PICK_FROM_3_INDIRECT : SLIR_PICK_FROM_3);
      ir_instr_append_use(instr, chain_reg);
      ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_ra->v_.regs_[2]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, subscript_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[2]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[5]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[11]));
    }
    case slrak_mat4: {
      struct ir_instr *instr;
      instr = ir_block_append_instr(blk, array_ra->is_indirect_ ? SLIR_PICK_FROM_4_INDIRECT : SLIR_PICK_FROM_4);
      ir_instr_append_use(instr, chain_reg);
      ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, subscript_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[4]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[8]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[12]));

      instr = ir_block_append_instr(blk, array_ra->is_indirect_ ? SLIR_PICK_FROM_4_INDIRECT : SLIR_PICK_FROM_4);
      ir_instr_append_use(instr, chain_reg);
      ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_ra->v_.regs_[1]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, subscript_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[1]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[5]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[9]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[13]));

      instr = ir_block_append_instr(blk, array_ra->is_indirect_ ? SLIR_PICK_FROM_4_INDIRECT : SLIR_PICK_FROM_4);
      ir_instr_append_use(instr, chain_reg);
      ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_ra->v_.regs_[2]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, subscript_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[2]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[6]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[10]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[14]));

      instr = ir_block_append_instr(blk, array_ra->is_indirect_ ? SLIR_PICK_FROM_4_INDIRECT : SLIR_PICK_FROM_4);
      ir_instr_append_use(instr, chain_reg);
      ir_instr_append_def(instr, ir_body_alloc_temp_banked_int(blk->body_, dst_ra->v_.regs_[3]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, subscript_ra->v_.regs_[0]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[3]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[7]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[9]));
      ir_instr_append_use(instr, ir_body_alloc_temp_banked_int(blk->body_, array_ra->v_.regs_[15]));
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
  if (!blk) return NULL;
  switch (x->op_) {
    case exop_variable:
      /* Should be a no-op in almost all cases, save for situations where the parent imposes an allocation, e.g. exop_conditional */
      sl_reg_emit_move(blk, chain_reg, frame, &x->variable_->reg_alloc_, NULL, &x->base_regs_, &x->offset_reg_);
      break;
    case exop_literal:
      sl_ir_init_lit(blk, chain_reg, frame, &x->base_regs_, &x->literal_value_, 0);
      break;
    case exop_array_subscript:
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[0]);
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[1]);
      if (x->children_[0]->base_regs_.kind_ == slrak_array) {
        if (x->children_[0]->offset_reg_.kind_ != slrak_void) {
          /* the array being subscripted has already been subscripted */
          sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[1]);
          sl_ir_i_mul_constant_and_add(blk, chain_reg, 
                                       x->offset_reg_.local_frame_ ? frame->local_int_offset_ + x->offset_reg_.v_.regs_[0] : x->offset_reg_.v_.regs_[0],
                                       x->children_[0]->offset_reg_.local_frame_ ? frame->local_int_offset_ + x->children_[0]->offset_reg_.v_.regs_[0] : x->children_[0]->offset_reg_.v_.regs_[0],
                                       (uint64_t)x->children_[0]->base_regs_.v_.array_.num_elements_,
                                       EXPR_RVALUE(x->children_[1])->local_frame_ ? frame->local_int_offset_ + EXPR_RVALUE(x->children_[1])->v_.regs_[0] : EXPR_RVALUE(x->children_[1])->v_.regs_[0]);
        }
        else {
          /* First subscript of the array */
          sl_reg_emit_move(blk, chain_reg, frame, &x->children_[1]->base_regs_, &x->children_[1]->offset_reg_, &x->offset_reg_, NULL);
        }
      }
      else {
        /* Not an array, base must be indirect and this array subscript is an access to components,
         * e.g. "vec4 v; v[3] // access w" */
        sl_ir_component_subscript(blk, chain_reg, frame, x, x->children_[0], x->children_[1]);
      }
      break;
    case exop_component_selection: /* e.g. myvec3.xxz */
    case exop_constructor: {
      size_t ci;
      for (ci = 0; ci < x->num_components_; ++ci) {
        struct sl_component_selection *cs = x->swizzle_ + ci;
        sl_reg_emit_move_c2c(blk, chain_reg, frame,
                              &x->children_[cs->parameter_index_]->base_regs_, &x->children_[cs->parameter_index_]->offset_reg_,
                              cs->component_index_,
                              &x->base_regs_, &x->offset_reg_,
                              (int)ci);
      }
      break;
    }
    case exop_field_selection:     /* e.g. mystruct.myfield */
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[0]);
      sl_reg_emit_move(blk, chain_reg, frame, 
                       &x->children_[0]->base_regs_.v_.comp_.fields_[x->field_index_], &x->children_[0]->offset_reg_,
                       &x->base_regs_, &x->offset_reg_);
      break;

    case exop_post_inc:
    case exop_post_dec:
      /* Retrieve value from child base/offset into x and then dec/increment from x into the child and store,
       * thus, the value that remains in x is pre dec/increment and the lvalue in the child is
       * post dec/increment */
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[0]);
      sl_reg_emit_move(blk, chain_reg, frame,
                       &x->children_[0]->base_regs_, &x->children_[0]->offset_reg_, 
                       &x->base_regs_, NULL);

      if (x->op_ == exop_post_inc) {
        sl_ir_inc(blk, chain_reg, frame, EXPR_RVALUE(x->children_[0]), x);
      }
      else /* (x->op_ == exop_post_dec) */ {
        sl_ir_dec(blk, chain_reg, frame, EXPR_RVALUE(x->children_[0]), x);
      }
      sl_reg_emit_move(blk, chain_reg, frame, 
                       EXPR_RVALUE(x->children_[0]), NULL, 
                       &x->children_[0]->base_regs_, &x->children_[0]->offset_reg_);
      break;

    case exop_pre_inc:
    case exop_pre_dec:
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[0]);
      sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[0]);
      if (x->op_ == exop_pre_inc) {
        sl_ir_inc(blk, chain_reg, frame, &x->base_regs_, x->children_[0]);
      }
      else /* (x->op_ == exop_pre_dec) */ {
        sl_ir_dec(blk, chain_reg, frame, &x->base_regs_, x->children_[0]);
      }
      sl_reg_emit_move(blk, chain_reg, frame, 
                       &x->base_regs_, &x->offset_reg_,
                       &x->children_[0]->base_regs_, &x->children_[0]->offset_reg_);
      break;

    case exop_negate:
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[0]);
      sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[0]);
      sl_ir_negate(blk, chain_reg, frame, x, x->children_[0]);
      break;

    case exop_logical_not:
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[0]);
      sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[0]);
      sl_ir_not(blk, chain_reg, frame, x, x->children_[0]);
      break;

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
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[0]);
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[1]);
      sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[0]);
      sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[1]);
      switch (x->op_) {
        case exop_multiply: sl_ir_mul(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
        case exop_divide: sl_ir_div(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
        case exop_add: sl_ir_add(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
        case exop_subtract: sl_ir_sub(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
        case exop_lt: sl_ir_lt(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
        case exop_le: sl_ir_le(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
        case exop_ge: sl_ir_ge(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
        case exop_gt: sl_ir_gt(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
        case exop_eq: sl_ir_eq(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
        case exop_ne: sl_ir_ne(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
      }
      break;

    case exop_function_call:
      break;

    case exop_logical_and: {
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[0]);
      sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[0]);
      /* Exec chain where condition_var is true (regular int), and where it is false.. */
      struct ir_temp *true_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
      struct ir_temp *false_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
      struct ir_instr *split_left = ir_block_append_instr(blk, SLIR_SPLIT_EXEC_CHAIN_BY_CONDITION);
      ir_instr_append_def(split_left, true_chain);
      ir_instr_append_def(split_left, false_chain);
      ir_instr_append_use(split_left, ir_body_alloc_temp_banked_bool(blk->body_, EXPR_RVALUE(x->children_[0])->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(x->children_[0])->v_.regs_[0] : EXPR_RVALUE(x->children_[0])->v_.regs_[0]));
      ir_instr_append_use(split_left, chain_reg);

      /* Where the first child expr (left) is false, we can already assign the outcome. */
      sl_reg_emit_move(blk, false_chain, frame, &x->children_[0]->base_regs_, &x->children_[0]->offset_reg_,
                       &x->base_regs_, &x->offset_reg_);

      struct ir_instr *cmp_instr = ir_block_append_instr(blk, GIR_COMPARE);
      ir_instr_append_use(cmp_instr, true_chain);
      ir_instr_append_use(cmp_instr, ir_body_alloc_temp_liti(blk->body_, SL_EXEC_NO_CHAIN));
      
      struct ir_block *true_branch = ir_body_alloc_block(blk->body_);
      struct ir_block *after = ir_body_alloc_block(blk->body_);

      struct ir_instr *bne = ir_block_append_instr(blk, GIR_BRANCH_NOT_EQUAL);
      ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, true_branch));
      ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, after));

      struct ir_block *after_true_branch = sl_ir_expr(true_branch, true_chain, frame, x->children_[1]);
      /* Move results of true branch (those results where left expr is true, and right expr therefore determines
       * the result.) */
      sl_reg_emit_move(after_true_branch, true_chain, frame,
                       &x->children_[1]->base_regs_, &x->children_[1]->offset_reg_,
                       &x->base_regs_, &x->offset_reg_);

      struct ir_instr *bra = ir_block_append_instr(after_true_branch, GIR_JUMP);
      ir_instr_append_use(bra, ir_body_alloc_temp_block(blk->body_, after));

      /* All done */
      return after;

      break;
    }

    case exop_logical_or: {
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[0]);
      sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[0]);

      /* Exec chain where condition_var is true (regular int), and where it is false.. */
      struct ir_temp *true_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
      struct ir_temp *false_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
      struct ir_instr *split_left = ir_block_append_instr(blk, SLIR_SPLIT_EXEC_CHAIN_BY_CONDITION);
      ir_instr_append_def(split_left, true_chain);
      ir_instr_append_def(split_left, false_chain);
      ir_instr_append_use(split_left, ir_body_alloc_temp_banked_bool(blk->body_, EXPR_RVALUE(x->children_[0])->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(x->children_[0])->v_.regs_[0] : EXPR_RVALUE(x->children_[0])->v_.regs_[0]));
      ir_instr_append_use(split_left, chain_reg);

      /* Where the first child expr (left) is true, we can already assign the outcome */
      sl_reg_emit_move(blk, true_chain, frame, &x->children_[0]->base_regs_, &x->children_[0]->offset_reg_,
                       &x->base_regs_, &x->offset_reg_);

      struct ir_instr *cmp_instr = ir_block_append_instr(blk, GIR_COMPARE);
      ir_instr_append_use(cmp_instr, false_chain);
      ir_instr_append_use(cmp_instr, ir_body_alloc_temp_liti(blk->body_, SL_EXEC_NO_CHAIN));

      struct ir_block *false_branch = ir_body_alloc_block(blk->body_);
      struct ir_block *after = ir_body_alloc_block(blk->body_);

      struct ir_instr *bne = ir_block_append_instr(blk, GIR_BRANCH_NOT_EQUAL);
      ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, false_branch));
      ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, after));

      false_branch = sl_ir_expr(false_branch, false_chain, frame, x->children_[1]);
      /* Move results of true branch (those results where left expr is true, and right expr therefore determines
       * the result.) */
      sl_reg_emit_move(false_branch, false_chain, frame,
                       &x->children_[1]->base_regs_, &x->children_[1]->offset_reg_,
                       &x->base_regs_, &x->offset_reg_);

      struct ir_instr *bra = ir_block_append_instr(false_branch, GIR_JUMP);
      ir_instr_append_use(bra, ir_body_alloc_temp_block(blk->body_, after));

      /* All done */
      return after;

    }

    case exop_logical_xor:
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[0]);
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[1]);
      sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[0]);
      sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[1]);
      sl_ir_log_xor(blk, chain_reg, frame, x, x->children_[0], x->children_[1]);
      break;

    case exop_assign:
      sl_reg_emit_move(blk, chain_reg, frame, 
                       &x->children_[1]->base_regs_, &x->children_[1]->offset_reg_,
                       &x->children_[0]->base_regs_, &x->children_[0]->offset_reg_);
      break;

    case exop_mul_assign:
    case exop_div_assign:
    case exop_add_assign:
    case exop_sub_assign:
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[0]);
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[1]);
      sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[0]);
      sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[1]);
      switch (x->op_) {
        case exop_mul_assign: sl_ir_mul(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
        case exop_div_assign: sl_ir_div(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
        case exop_add_assign: sl_ir_add(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
        case exop_sub_assign: sl_ir_sub(blk, chain_reg, frame, x, x->children_[0], x->children_[1]); break;
      }
      sl_reg_emit_move(blk, chain_reg, frame, 
                       &x->base_regs_, &x->offset_reg_,
                       &x->children_[0]->base_regs_, &x->children_[0]->offset_reg_);
      break;

    case exop_sequence:    // comma operator
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[0]);
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[1]);
      break;

    case exop_conditional: { // ternary ?: operator
      blk = sl_ir_expr(blk, chain_reg, frame, x->children_[0]);
      sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[0]);

      struct ir_temp *true_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
      struct ir_temp *false_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
      struct ir_instr *split_left = ir_block_append_instr(blk, SLIR_SPLIT_EXEC_CHAIN_BY_CONDITION);
      ir_instr_append_def(split_left, true_chain);
      ir_instr_append_def(split_left, false_chain);
      ir_instr_append_use(split_left, ir_body_alloc_temp_banked_bool(blk->body_, EXPR_RVALUE(x->children_[0])->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(x->children_[0])->v_.regs_[0] : EXPR_RVALUE(x->children_[0])->v_.regs_[0]));
      ir_instr_append_use(split_left, chain_reg);

      struct ir_instr *cmp_instr = ir_block_append_instr(blk, GIR_COMPARE);
      struct ir_temp *no_chain_lit = ir_body_alloc_temp_liti(blk->body_, SL_EXEC_NO_CHAIN);
      ir_instr_append_use(cmp_instr, true_chain);
      ir_instr_append_use(cmp_instr, no_chain_lit);

      struct ir_block *true_branch = ir_body_alloc_block(blk->body_);
      struct ir_block *after_true_branch = ir_body_alloc_block(blk->body_);

      struct ir_instr *bne = ir_block_append_instr(blk, GIR_BRANCH_NOT_EQUAL);
      ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, true_branch));
      ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, after_true_branch));

      // .. true_branch
      true_branch = sl_ir_expr(true_branch, true_chain, frame, x->children_[1]);

      sl_reg_emit_move(true_branch, true_chain, frame, &x->children_[1]->base_regs_, &x->children_[1]->offset_reg_,
                       &x->base_regs_, &x->offset_reg_);

      struct ir_instr *bra = ir_block_append_instr(true_branch, GIR_JUMP);
      ir_instr_append_use(bra, ir_body_alloc_temp_block(blk->body_, after_true_branch));

      cmp_instr = ir_block_append_instr(after_true_branch, GIR_COMPARE);
      ir_instr_append_use(cmp_instr, false_chain);
      ir_instr_append_use(cmp_instr, no_chain_lit);

      struct ir_block *false_branch = ir_body_alloc_block(blk->body_);
      struct ir_block *after_false_branch = ir_body_alloc_block(blk->body_);

      bne = ir_block_append_instr(blk, GIR_BRANCH_NOT_EQUAL);
      ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, false_branch));
      ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, after_false_branch));

      // .. false_branch
      false_branch = sl_ir_expr(false_branch, false_chain, frame, x->children_[2]);

      sl_reg_emit_move(false_branch, false_chain, frame, &x->children_[2]->base_regs_, &x->children_[2]->offset_reg_,
                       &x->base_regs_, &x->offset_reg_);

      bra = ir_block_append_instr(false_branch, GIR_JUMP);
      ir_instr_append_use(bra, ir_body_alloc_temp_block(blk->body_, after_false_branch));

      return after_false_branch;
    }
  }

  return blk;
}
