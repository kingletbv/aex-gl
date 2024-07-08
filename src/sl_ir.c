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
      break;
    case exop_literal:
    case exop_array_subscript:
    case exop_component_selection: /* e.g. myvec3.xxz */
    case exop_field_selection:     /* e.g. mystruct.myfield */
    case exop_post_inc:
    case exop_post_dec:
    case exop_pre_inc:
    case exop_pre_dec:

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
    case exop_constructor:
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
      ir_instr_append_use(cmp_instr, ir_body_alloc_temp_lit(blk->body_, SL_EXEC_NO_CHAIN));
      
      struct ir_block *true_branch = ir_body_alloc_block(blk->body_);
      struct ir_block *after = ir_body_alloc_block(blk->body_);

      struct ir_instr *beq = ir_block_append_instr(blk, GIR_BRANCH_NOT_EQUAL);
      ir_instr_append_use(beq, ir_body_alloc_temp_block(blk->body_, true_branch));
      ir_instr_append_use(beq, ir_body_alloc_temp_block(blk->body_, after));

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
      ir_instr_append_use(cmp_instr, ir_body_alloc_temp_lit(blk->body_, SL_EXEC_NO_CHAIN));

      struct ir_block *false_branch = ir_body_alloc_block(blk->body_);
      struct ir_block *after = ir_body_alloc_block(blk->body_);

      struct ir_instr *beq = ir_block_append_instr(blk, GIR_BRANCH_NOT_EQUAL);
      ir_instr_append_use(beq, ir_body_alloc_temp_block(blk->body_, false_branch));
      ir_instr_append_use(beq, ir_body_alloc_temp_block(blk->body_, after));

      struct ir_block *after_false_branch = sl_ir_expr(false_branch, false_chain, frame, x->children_[1]);
      /* Move results of true branch (those results where left expr is true, and right expr therefore determines
       * the result.) */
      sl_reg_emit_move(after_false_branch, false_chain, frame,
                       &x->children_[1]->base_regs_, &x->children_[1]->offset_reg_,
                       &x->base_regs_, &x->offset_reg_);

      struct ir_instr *bra = ir_block_append_instr(after_false_branch, GIR_JUMP);
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

    case exop_conditional:  // ternary ?: operator
      ;
  }

  return blk;
}
