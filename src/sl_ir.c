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

#ifndef SL_STMT_H_INCLUDED
#define SL_STMT_H_INCLUDED
#include "sl_stmt.h"
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

static struct ireg_operand load_operands[] = {
  { IREG_DEF },
  { IREG_USE }
};

static struct ireg_operand store_operands[] = {
  { IREG_USE },
  { IREG_USE }
};

static struct ireg_operand move_operands[] = {
  { IREG_DEF },
  { IREG_USE }
};

static struct ireg_operand imm_operands[] = {
  { IREG_DEF },
  { IREG_USE }
};

static struct ireg_operand bin_operands[] = {
  { IREG_DEF },
  { IREG_USE },
  { IREG_USE }
};

static struct ireg_operand unary_operands[] = {
  { IREG_DEF },
  { IREG_USE }
};

static struct ireg_operand bin_root_operands[] = {
  { IREG_USE },
  { IREG_USE }
};

static struct ireg_operand unary_root_operands[] = {
  { IREG_USE }
};

static struct ireg_operand unary_def_from_sideffect[] = {
  { IREG_DEF }
};

static struct ireg_operand quad_def_def_operands[] = {
  { IREG_DEF },
  { IREG_DEF },
  { IREG_USE },
  { IREG_USE }
};

static struct ireg_operand tri_operands[] = {
  { IREG_DEF },
  { IREG_USE },
  { IREG_USE }
};

static struct ireg_operand slir_tri[] = {
  { IREG_USE },
  { IREG_DEF },
  { IREG_USE }
};

static struct ireg_operand slir_quad[] = {
  { IREG_USE },
  { IREG_DEF },
  { IREG_USE },
  { IREG_USE }
};
static struct ireg_operand slir_5_opd[] = {
  { IREG_USE },
  { IREG_DEF },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE }
};

static struct ireg_operand slir_6_opd[] = {
  { IREG_USE },
  { IREG_DEF },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE }
};

static struct ireg_operand slir_7_opd[] = {
  { IREG_USE },
  { IREG_DEF },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE }
};

static struct ireg_operand slir_8_opd[] = {
  { IREG_USE },
  { IREG_DEF },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE }
};

static struct ireg_operand slir_10_opd[] = {
  { IREG_USE },
  { IREG_DEF },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE },
  { IREG_USE }
};


static struct ireg_instr gir_instructions_[] = {
  { IRARC_IR, GIR_LOAD8, 1, load_operands, "GIR_LOAD8", "%0, %1" },
  { IRARC_IR, GIR_LOAD16, 1, load_operands,  "GIR_LOAD16", "%0, %1" },
  { IRARC_IR, GIR_LOAD32, 1, load_operands, "GIR_LOAD32", "%0, %1" },
  { IRARC_IR, GIR_LOAD64, 1, load_operands, "GIR_LOAD64", "%0, %1" },

  { IRARC_IR, GIR_STORE, 2, store_operands, "GIR_STORE", "%0, %1" },
  { IRARC_IR, GIR_MOVE, 2, move_operands, "GIR_MOVE", "%0, %1" },

  { IRARC_IR, GIR_IMM8, 2, imm_operands, "GIR_IMM8", "%0, %1" },
  { IRARC_IR, GIR_IMM16, 2, imm_operands, "GIR_IMM16", "%0, %1" },
  { IRARC_IR, GIR_IMM32, 2, imm_operands, "GIR_IMM32", "%0, %1" },
  { IRARC_IR, GIR_IMM64, 2, imm_operands, "GIR_IMM64", "%0, %1" },

  { IRARC_IR, GIR_ADD, 3, bin_operands, "GIR_ADD", "%0, %1, %2" },
  { IRARC_IR, GIR_SUB, 3, bin_operands, "GIR_SUB", "%0, %1, %2" },
  { IRARC_IR, GIR_MULS, 3, bin_operands, "GIR_MULS", "%0, %1, %2" },
  { IRARC_IR, GIR_MULU, 3, bin_operands, "GIR_MULU", "%0, %1, %2" },
  { IRARC_IR, GIR_MUL, 3, bin_operands, "GIR_MUL", "%0, %1, %2" },
  { IRARC_IR, GIR_DIVS, 3, bin_operands, "GIR_DIVS", "%0, %1, %2" },
  { IRARC_IR, GIR_DIVU, 3, bin_operands, "GIR_DIVU", "%0, %1, %2" },
  { IRARC_IR, GIR_AND, 3, bin_operands, "GIR_AND", "%0, %1, %2" },
  { IRARC_IR, GIR_OR, 3, bin_operands,  "GIR_OR", "%0, %1, %2" },
  { IRARC_IR, GIR_XOR, 3, bin_operands, "GIR_XOR", "%0, %1, %2" },
  { IRARC_IR, GIR_LSL, 3, bin_operands, "GIR_LSL", "%0, %1, %2" },
  { IRARC_IR, GIR_LSR, 3, bin_operands, "GIR_LSR", "%0, %1, %2" },
  { IRARC_IR, GIR_ASR, 3, bin_operands, "GIR_ASR", "%0, %1, %2" },

  { IRARC_IR, GIR_NEG, 2, unary_operands, "GIR_NEG", "%0, %1" },
  { IRARC_IR, GIR_NOT, 2, unary_operands, "GIR_NOT", "%0, %1" },

  { IRARC_IR, GIR_ZX_8_TO_16, 2, unary_operands, "GIR_ZERO_EXT_8_TO_16", "%0, %1" },
  { IRARC_IR, GIR_ZX_16_TO_32, 2, unary_operands, "GIR_ZERO_EXT_16_TO_32", "%0, %1" },
  { IRARC_IR, GIR_ZX_32_TO_64, 2, unary_operands, "GIR_ZERO_EXT_32_TO_64", "%0, %1" },

  { IRARC_IR, GIR_SX_8_TO_16, 2, unary_operands, "GIR_SIGN_EXT_8_TO_16", "%0, %1" },
  { IRARC_IR, GIR_SX_16_TO_32, 2, unary_operands, "GIR_SIGN_EXT_16_TO_32", "%0, %1" },
  { IRARC_IR, GIR_SX_32_TO_64, 2, unary_operands, "GIR_SIGN_EXT_32_TO_64", "%0, %1" },

  { IRARC_IR, GIR_CULL_64_TO_32, 2, unary_operands, "GIR_CULL_64_TO_32", "%0, %1" },
  { IRARC_IR, GIR_CULL_32_TO_16, 2, unary_operands, "GIR_CULL_32_TO_16", "%0, %1" },
  { IRARC_IR, GIR_CULL_16_TO_8, 2, unary_operands, "GIR_CULL_16_TO_8", "%0, %1" },

  { IRARC_IR, GIR_JUMP, 1, unary_root_operands, "GIR_JUMP", "%0"},

  { IRARC_IR, GIR_BRANCH_SIGNED_GREATER, 2, bin_root_operands, "GIR_BRANCH_SIGNED_GREATER", "%0, %1" },
  { IRARC_IR, GIR_BRANCH_SIGNED_GREATER_EQUAL, 2, bin_root_operands, "GIR_BRANCH_SIGNED_GREATER_EQUAL", "%0, %1" },
  { IRARC_IR, GIR_BRANCH_SIGNED_LESSER, 2, bin_root_operands, "GIR_BRANCH_SIGNED_LESSER", "%0, %1" },
  { IRARC_IR, GIR_BRANCH_SIGNED_LESSER_EQUAL, 2, bin_root_operands, "GIR_BRANCH_SIGNED_LESSER_EQUAL", "%0, %1" },

  { IRARC_IR, GIR_BRANCH_UNSIGNED_GREATER, 2, bin_root_operands, "GIR_BRANCH_UNSIGNED_GREATER", "%0, %1" },
  { IRARC_IR, GIR_BRANCH_UNSIGNED_GREATER_EQUAL, 2, bin_root_operands, "GIR_BRANCH_UNSIGNED_GREATER_EQUAL", "%0, %1" },
  { IRARC_IR, GIR_BRANCH_UNSIGNED_LESSER, 2, bin_root_operands, "GIR_BRANCH_UNSIGNED_LESSER", "%0, %1" },
  { IRARC_IR, GIR_BRANCH_UNSIGNED_LESSER_EQUAL, 2, bin_root_operands, "GIR_BRANCH_UNSIGNED_LESSER_EQUAL", "%0, %1" },

  { IRARC_IR, GIR_BRANCH_EQUAL, 2, bin_root_operands, "GIR_BRANCH_EQUAL", "%0, %1" },
  { IRARC_IR, GIR_BRANCH_NOT_EQUAL, 2, bin_root_operands, "GIR_BRANCH_NOT_EQUAL", "%0, %1" },

  { IRARC_IR, GIR_COMPARE, 2, bin_root_operands, "GIR_COMPARE", "%0, %1" },

  /* Bit of a split here, currently these are defined as the low-level primitive almost-machine language forms
   * such that we can match their tiles to the actual X64 equivalents;
   */
  { IRARC_IR, GIR_CALL, 1, unary_root_operands, "GIR_CALL", "%0" },
  { IRARC_IR, GIR_RETURN, 0, NULL, "GIR_RETURN", "" },
  { IRARC_IR, GIR_PUSH, 1, unary_root_operands, "GIR_PUSH", "%0" },
  { IRARC_IR, GIR_POP, 1, unary_def_from_sideffect, "GIR_POP", "%0" }
};

static struct ireg_instr slir_instructions_[] = {
  { IRARC_SLIR, SLIR_SPLIT_EXEC_CHAIN_BY_CONDITION, 4, quad_def_def_operands, "SLIR_SPLIT", "%0, %1, %2, %3" },
  { IRARC_SLIR, SLIR_JOIN_EXEC_CHAINS, 3, tri_operands, "SLIR_JOIN", "%0, %1, %2" },

  { IRARC_SLIR, SLIR_NEG_F, 3, slir_tri, "SLIR_NEG", "%0, %1, %2" },
  { IRARC_SLIR, SLIR_NEG_I, 3, slir_tri, "SLIR_NEG", "%0, %1, %2" },

  { IRARC_SLIR, SLIR_NOT, 3, slir_tri, "SLIR_NOT", "%0, %1, %2" },

  { IRARC_SLIR, SLIR_MUL_F, 4, slir_quad, "SLIR_MUL", "%0, %1, %2, %3" },
  { IRARC_SLIR, SLIR_MUL_I, 4, slir_quad, "SLIR_MUL", "%0, %1, %2, %3" },

  { IRARC_SLIR, SLIR_DOT2, 6, slir_6_opd, "SLIR_DOT", "%0, %1, %2, %3, %4, %5" },
  { IRARC_SLIR, SLIR_DOT3, 8, slir_8_opd, "SLIR_DOT", "%0, %1, %2, %3, %4, %5, %6, %7" },
  { IRARC_SLIR, SLIR_DOT4, 10, slir_10_opd, "SLIR_DOT", "%0, %1, %2, %3, %4, %5, %6, %7, %8, %9" },

  { IRARC_SLIR, SLIR_DIV_F, 4, slir_quad, "SLIR_DIV", "%0, %1, %2, %3" },
  { IRARC_SLIR, SLIR_DIV_I, 4, slir_quad, "SLIR_DIV", "%0, %1, %2, %3" },

  { IRARC_SLIR, SLIR_ADD_F, 4, slir_quad, "SLIR_ADD", "%0, %1, %2, %3" },
  { IRARC_SLIR, SLIR_ADD_I, 4, slir_quad, "SLIR_ADD", "%0, %1, %2, %3" },

  { IRARC_SLIR, SLIR_SUB_F, 4, slir_quad, "SLIR_SUB", "%0, %1, %2, %3" },
  { IRARC_SLIR, SLIR_SUB_I, 4, slir_quad, "SLIR_SUB", "%0, %1, %2, %3" },

  { IRARC_SLIR, SLIR_LT_F, 4, slir_quad, "SLIR_LT", "%0, %1, %2, %3" },
  { IRARC_SLIR, SLIR_LT_I, 4, slir_quad, "SLIR_LT", "%0, %1, %2, %3" },

  { IRARC_SLIR, SLIR_LE_F, 4, slir_quad, "SLIR_LE", "%0, %1, %2, %3" },
  { IRARC_SLIR, SLIR_LE_I, 4, slir_quad, "SLIR_LE", "%0, %1, %2, %3" },

  { IRARC_SLIR, SLIR_GE_F, 4, slir_quad, "SLIR_GE", "%0, %1, %2, %3" },
  { IRARC_SLIR, SLIR_GE_I, 4, slir_quad, "SLIR_GE", "%0, %1, %2, %3" },

  { IRARC_SLIR, SLIR_GT_F, 4, slir_quad, "SLIR_GT", "%0, %1, %2, %3" },
  { IRARC_SLIR, SLIR_GT_I, 4, slir_quad, "SLIR_GT", "%0, %1, %2, %3" },

  { IRARC_SLIR, SLIR_EQ_F, 4, slir_quad, "SLIR_EQ", "%0, %1, %2, %3" },
  { IRARC_SLIR, SLIR_EQ_I, 4, slir_quad, "SLIR_EQ", "%0, %1, %2, %3" },

  { IRARC_SLIR, SLIR_NE_F, 4, slir_quad, "SLIR_NE", "%0, %1, %2, %3" },
  { IRARC_SLIR, SLIR_NE_I, 4, slir_quad, "SLIR_NE", "%0, %1, %2, %3" },

  { IRARC_SLIR, SLIR_LOG_XOR, 4, slir_quad, "SLIR_LOG_XOR", "%0, %1, %2, %3" },

  { IRARC_SLIR, SLIR_DEC_F, 3, slir_tri, "SLIR_DEC", "%0, %1, %2" },
  { IRARC_SLIR, SLIR_DEC_I, 3, slir_tri, "SLIR_DEC", "%0, %1, %2" },

  { IRARC_SLIR, SLIR_INC_F, 3, slir_tri, "SLIR_INC", "%0, %1, %2" },
  { IRARC_SLIR, SLIR_INC_I, 3, slir_tri, "SLIR_INC", "%0, %1, %2" },

  { IRARC_SLIR, SLIR_INIT_LIT_F, 3, slir_tri, "SLIR_INIT_LIT", "%0, %1, %2" },
  { IRARC_SLIR, SLIR_INIT_LIT_I, 3, slir_tri, "SLIR_INIT_LIT", "%0, %1, %2" },
  { IRARC_SLIR, SLIR_INIT_LIT_B, 3, slir_tri, "SLIR_INIT_LIT", "%0, %1, %2" },

  { IRARC_SLIR, SLIR_MUL_C_AND_ADD, 5, slir_5_opd, "SLIR_MUL_C_AND_ADD", "%0, %1, %2, %3, %4" },

  { IRARC_SLIR, SLIR_PICK_FROM_2, 5, slir_5_opd, "SLIR_PICK", "%0, %1, %2:{ %3, %4 }" },
  { IRARC_SLIR, SLIR_PICK_FROM_3, 6, slir_6_opd, "SLIR_PICK", "%0, %1, %2:{ %3, %4, %5 }" },
  { IRARC_SLIR, SLIR_PICK_FROM_4, 7, slir_7_opd, "SLIR_PICK", "%0, %1, %2:{ %3, %4, %5, %6 }" },

  { IRARC_SLIR, SLIR_PICK_FROM_2_INDIRECT, 5, slir_5_opd, "SLIR_PICK", "%0, %1, [%2:{ %3, %4 }]" },
  { IRARC_SLIR, SLIR_PICK_FROM_3_INDIRECT, 6, slir_6_opd, "SLIR_PICK", "%0, %1, [%2:{ %3, %4, %5 }]" },
  { IRARC_SLIR, SLIR_PICK_FROM_4_INDIRECT, 7, slir_7_opd, "SLIR_PICK", "%0, %1, [%2:{ %3, %4, %5, %6 }]" }
};


void sl_ir_register_instructions(struct ireg_registry *reg) {
  size_t n;
  for (n = 0; n < sizeof(gir_instructions_)/sizeof(*gir_instructions_); ++n) {
    ireg_set_instruction(reg, gir_instructions_ + n);
  }

  for (n = 0; n < sizeof(slir_instructions_)/sizeof(*slir_instructions_); ++n) {
    ireg_set_instruction(reg, slir_instructions_ + n);
  }
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
  struct ir_instr *instr = ir_block_append_instr(blk, SLIR_INIT_LIT_B);
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

/* Writes code starting at the end of blk, emits all statements in the list, and returns the blk at the end of which
 * all statements have executed. */
struct ir_block *sl_ir_stmt(struct ir_block *blk, struct ir_temp *chain_reg, struct sl_execution_frame *frame, struct sl_stmt *stmt) {
  if (!blk) return NULL;
  while (stmt) {
    switch (stmt->kind_) {
      case slsk_expression:
        blk = sl_ir_expr(blk, chain_reg, frame, stmt->expr_);
        break;
      case slsk_if: {
        /* 1. evaluate condition
         * 2. split chain_reg into a true_chain (where condition is true) and a false_chain (where it is false)
         * 3. if the true_chain is not empty, evaluate the true branch, otherwise jump to after the true branch (#5)
         * 4. execute the true branch using the true_chain
         * 5. if the false_chain is not empty, evaluate the false branch, otherwise jump to after the false branch (#7)
         * 6. execute the false branch using the false_chain
         * 7. join the true and false chain back into the chain_reg (so the same chain we had before is once more
         *    restored; the act of splitting the chain rendered the original chain invalid, and some rows might not
         *    have made it back here (e.g. discard, return, etc.)
         */
        blk = sl_ir_expr(blk, chain_reg, frame, stmt->condition_);
        sl_ir_need_rvalue(blk, chain_reg, frame, stmt->condition_);
        struct ir_temp *true_chain, *false_chain;
        true_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
        false_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
        struct ir_instr *split = ir_block_append_instr(blk, SLIR_SPLIT_EXEC_CHAIN_BY_CONDITION);
        ir_instr_append_def(split, true_chain);
        ir_instr_append_def(split, false_chain);
        ir_instr_append_use(split, ir_body_alloc_temp_banked_bool(blk->body_, EXPR_RVALUE(stmt->condition_)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(stmt->condition_)->v_.regs_[0] : EXPR_RVALUE(stmt->condition_)->v_.regs_[0]));
        ir_instr_append_use(split, chain_reg);

        struct ir_instr *cmp_instr = ir_block_append_instr(blk, GIR_COMPARE);
        struct ir_temp *no_chain_lit = ir_body_alloc_temp_liti(blk->body_, SL_EXEC_NO_CHAIN);
        ir_instr_append_use(cmp_instr, true_chain);
        ir_instr_append_use(cmp_instr, no_chain_lit);

        struct ir_block *true_branch = ir_body_alloc_block(blk->body_);
        struct ir_block *after_true_branch = ir_body_alloc_block(blk->body_);

        struct ir_instr *bne = ir_block_append_instr(blk, GIR_BRANCH_NOT_EQUAL);
        ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, true_branch));
        ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, after_true_branch));

        true_branch = sl_ir_stmt(true_branch, true_chain, frame, stmt->true_branch_);

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

        false_branch = sl_ir_stmt(false_branch, false_chain, frame, stmt->false_branch_);

        bra = ir_block_append_instr(false_branch, GIR_JUMP);
        ir_instr_append_use(bra, ir_body_alloc_temp_block(blk->body_, after_false_branch));

        blk = after_false_branch;
        struct ir_instr *rejoin = ir_block_append_instr(blk, SLIR_JOIN_EXEC_CHAINS);
        ir_instr_append_def(rejoin, chain_reg);
        ir_instr_append_use(rejoin, true_chain);
        ir_instr_append_use(rejoin, false_chain);
        break;
      }
      case slsk_while: {
        struct sl_execution_frame loop_frame = *frame;
        loop_frame.continue_chain_ = ir_body_alloc_temp_unused_virtual(blk->body_);
        loop_frame.break_chain_ = chain_reg;

        /* 1. evaluate condition
         * 2. split chain_reg into a true_chain (where the condition is true) and a false chain back in chain_reg (where it is false)
         * 3. if the true_chain is empty, go to #9
         * 4. execute the true branch using the true_chain
         * 5. evaluate condition using the true_chain
         * 6. split true_chain into a true_chain (where the condition continues to be true), and a loop_exit_chain (where it is false)
         * 7. join chain_reg chain and the loop_exit_chain into a new chain_reg chain.
         * 8. if the true_chain is not empty, go to #4
         * 9. done with while loop (nop).
         * 
         */
        blk = sl_ir_stmt(blk, chain_reg, frame, stmt->prep_cond_);
        blk = sl_ir_expr(blk, chain_reg, frame, stmt->condition_);
        sl_ir_need_rvalue(blk, chain_reg, frame, stmt->condition_);
        struct ir_temp *true_chain, *loop_exit_chain;
        true_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
        loop_exit_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
        struct ir_temp *condition = ir_body_alloc_temp_banked_bool(blk->body_, EXPR_RVALUE(stmt->condition_)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(stmt->condition_)->v_.regs_[0] : EXPR_RVALUE(stmt->condition_)->v_.regs_[0]);
        struct ir_instr *split = ir_block_append_instr(blk, SLIR_SPLIT_EXEC_CHAIN_BY_CONDITION);
        ir_instr_append_def(split, true_chain);
        ir_instr_append_def(split, chain_reg);
        ir_instr_append_use(split, condition);
        ir_instr_append_use(split, chain_reg);

        struct ir_instr *cmp_instr = ir_block_append_instr(blk, GIR_COMPARE);
        struct ir_temp *no_chain_lit = ir_body_alloc_temp_liti(blk->body_, SL_EXEC_NO_CHAIN);
        ir_instr_append_use(cmp_instr, true_chain);
        ir_instr_append_use(cmp_instr, no_chain_lit);

        struct ir_block *loop_entry = ir_body_alloc_block(blk->body_);
        struct ir_block *true_branch = loop_entry;
        struct ir_block *after_true_branch = ir_body_alloc_block(blk->body_);

        struct ir_instr *bne = ir_block_append_instr(blk, GIR_BRANCH_NOT_EQUAL);
        ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, true_branch));
        ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, after_true_branch));

        struct ir_instr *clear_chain = ir_block_append_instr(true_branch, GIR_MOVE);
        ir_instr_append_def(clear_chain, frame->continue_chain_);
        ir_instr_append_use(clear_chain, no_chain_lit);

        true_branch = sl_ir_stmt(true_branch, true_chain, &loop_frame, stmt->true_branch_);

        struct ir_instr *rejoin = ir_block_append_instr(blk, SLIR_JOIN_EXEC_CHAINS);
        ir_instr_append_def(rejoin, true_chain);
        ir_instr_append_use(rejoin, true_chain);
        ir_instr_append_use(rejoin, frame->continue_chain_);

        true_branch = sl_ir_stmt(true_branch, true_chain, frame, stmt->prep_cond_);
        true_branch = sl_ir_expr(true_branch, true_chain, frame, stmt->condition_);
        sl_ir_need_rvalue(true_branch, true_chain, frame, stmt->condition_);

        split = ir_block_append_instr(true_branch, SLIR_SPLIT_EXEC_CHAIN_BY_CONDITION);
        ir_instr_append_def(split, true_chain);
        ir_instr_append_def(split, loop_exit_chain);
        ir_instr_append_use(split, condition);
        ir_instr_append_use(split, true_chain);

        struct ir_instr *join = ir_block_append_instr(true_branch, SLIR_JOIN_EXEC_CHAINS);
        ir_instr_append_def(join, chain_reg);
        ir_instr_append_use(join, chain_reg);
        ir_instr_append_use(join, loop_exit_chain);

        cmp_instr = ir_block_append_instr(true_branch, GIR_COMPARE);
        ir_instr_append_use(cmp_instr, true_chain);
        ir_instr_append_use(cmp_instr, no_chain_lit);

        bne = ir_block_append_instr(true_branch, GIR_BRANCH_NOT_EQUAL);
        ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, loop_entry));
        ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, after_true_branch));

        blk = after_true_branch;
        break;
      }
      case slsk_do: {
        struct sl_execution_frame loop_frame = *frame;
        loop_frame.continue_chain_ = ir_body_alloc_temp_unused_virtual(blk->body_);
        loop_frame.break_chain_ = chain_reg;

        /* 1. move chain_reg to true_chain
         * 2. set chain_reg to SL_EXEC_NO_CHAIN (clearing it empty)
         * 3. execute the true branch using the true_chain
         * 4. evaluate condition using the true_chain
         * 5. split true_chain into a true_chain (where the condition continues to be true), and a loop_exit_chain (where it is false)
         * 6. join chain_reg chain, and the loop_exit_chain into a new chain_reg chain.
         * 7. if the true_chain is not empty, go to #3
         * 8. done with do-while loop (nop)
         * so this is a lot like a while loop, save for the first few steps.
         */
        struct ir_temp *true_chain, *loop_exit_chain;
        true_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
        loop_exit_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
        struct ir_temp *condition = ir_body_alloc_temp_banked_bool(blk->body_, EXPR_RVALUE(stmt->condition_)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(stmt->condition_)->v_.regs_[0] : EXPR_RVALUE(stmt->condition_)->v_.regs_[0]);

        struct ir_temp *no_chain_lit = ir_body_alloc_temp_liti(blk->body_, SL_EXEC_NO_CHAIN);

        struct ir_instr *move = ir_block_append_instr(blk, GIR_MOVE);
        ir_instr_append_def(move, true_chain);
        ir_instr_append_use(move, chain_reg);

        move = ir_block_append_instr(blk, GIR_MOVE);
        ir_instr_append_def(move, chain_reg);
        ir_instr_append_use(move, no_chain_lit);

        struct ir_block *loop_entry = ir_body_alloc_block(blk->body_);
        struct ir_block *true_branch = loop_entry;
        struct ir_block *after_true_branch = ir_body_alloc_block(blk->body_);

        struct ir_instr *clear_chain = ir_block_append_instr(true_branch, GIR_MOVE);
        ir_instr_append_def(clear_chain, frame->continue_chain_);
        ir_instr_append_use(clear_chain, no_chain_lit);

        true_branch = sl_ir_stmt(true_branch, true_chain, &loop_frame, stmt->true_branch_);

        struct ir_instr *rejoin = ir_block_append_instr(blk, SLIR_JOIN_EXEC_CHAINS);
        ir_instr_append_def(rejoin, true_chain);
        ir_instr_append_use(rejoin, true_chain);
        ir_instr_append_use(rejoin, frame->continue_chain_);

        true_branch = sl_ir_expr(true_branch, true_chain, frame, stmt->condition_);
        sl_ir_need_rvalue(true_branch, true_chain, frame, stmt->condition_);

        struct ir_instr *split = ir_block_append_instr(true_branch, SLIR_SPLIT_EXEC_CHAIN_BY_CONDITION);
        ir_instr_append_def(split, true_chain);
        ir_instr_append_def(split, loop_exit_chain);
        ir_instr_append_use(split, condition);
        ir_instr_append_use(split, true_chain);

        struct ir_instr *join = ir_block_append_instr(true_branch, SLIR_JOIN_EXEC_CHAINS);
        ir_instr_append_def(join, chain_reg);
        ir_instr_append_use(join, chain_reg);
        ir_instr_append_use(join, loop_exit_chain);

        struct ir_instr *cmp_instr = ir_block_append_instr(true_branch, GIR_COMPARE);
        ir_instr_append_use(cmp_instr, true_chain);
        ir_instr_append_use(cmp_instr, no_chain_lit);

        struct ir_instr *bne = ir_block_append_instr(true_branch, GIR_BRANCH_NOT_EQUAL);
        ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, loop_entry));
        ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, after_true_branch));

        blk = after_true_branch;
        break;
      }
      case slsk_for: {
        struct sl_execution_frame loop_frame = *frame;
        loop_frame.continue_chain_ = ir_body_alloc_temp_unused_virtual(blk->body_);
        /* we loop on true_chain, and accrue into chain_reg all those rows that no longer meet the loop condition,
         * therefore chain_reg is the appropriate chain to join when we "break" from the loop body */
        loop_frame.break_chain_ = chain_reg;

        sl_ir_stmt(blk, chain_reg, frame, stmt->prep_);
        sl_ir_stmt(blk, chain_reg, frame, stmt->prep_cond_);

        blk = sl_ir_expr(blk, chain_reg, frame, stmt->condition_);
        sl_ir_need_rvalue(blk, chain_reg, frame, stmt->condition_);
        struct ir_temp *true_chain, *loop_exit_chain;
        true_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
        loop_exit_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
        struct ir_temp *condition = ir_body_alloc_temp_banked_bool(blk->body_, EXPR_RVALUE(stmt->condition_)->local_frame_ ? frame->local_float_offset_ + EXPR_RVALUE(stmt->condition_)->v_.regs_[0] : EXPR_RVALUE(stmt->condition_)->v_.regs_[0]);
        struct ir_instr *split = ir_block_append_instr(blk, SLIR_SPLIT_EXEC_CHAIN_BY_CONDITION);
        ir_instr_append_def(split, true_chain);
        ir_instr_append_def(split, chain_reg);
        ir_instr_append_use(split, condition);
        ir_instr_append_use(split, chain_reg);

        struct ir_instr *cmp_instr = ir_block_append_instr(blk, GIR_COMPARE);
        struct ir_temp *no_chain_lit = ir_body_alloc_temp_liti(blk->body_, SL_EXEC_NO_CHAIN);
        ir_instr_append_use(cmp_instr, true_chain);
        ir_instr_append_use(cmp_instr, no_chain_lit);

        struct ir_block *loop_entry = ir_body_alloc_block(blk->body_);
        struct ir_block *true_branch = loop_entry;
        struct ir_block *after_true_branch = ir_body_alloc_block(blk->body_);

        struct ir_instr *bne = ir_block_append_instr(blk, GIR_BRANCH_NOT_EQUAL);
        ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, true_branch));
        ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, after_true_branch));

        struct ir_instr *clear_chain = ir_block_append_instr(true_branch, GIR_MOVE);
        ir_instr_append_def(clear_chain, frame->continue_chain_);
        ir_instr_append_use(clear_chain, no_chain_lit);

        true_branch = sl_ir_stmt(true_branch, true_chain, &loop_frame, stmt->true_branch_);

        struct ir_instr *rejoin = ir_block_append_instr(blk, SLIR_JOIN_EXEC_CHAINS);
        ir_instr_append_def(rejoin, true_chain);
        ir_instr_append_use(rejoin, true_chain);
        ir_instr_append_use(rejoin, frame->continue_chain_);

        true_branch = sl_ir_expr(true_branch, true_chain, frame, stmt->post_);

        true_branch = sl_ir_stmt(true_branch, true_chain, frame, stmt->prep_cond_);

        true_branch = sl_ir_expr(true_branch, true_chain, frame, stmt->condition_);
        sl_ir_need_rvalue(true_branch, true_chain, frame, stmt->condition_);

        split = ir_block_append_instr(true_branch, SLIR_SPLIT_EXEC_CHAIN_BY_CONDITION);
        ir_instr_append_def(split, true_chain);
        ir_instr_append_def(split, loop_exit_chain);
        ir_instr_append_use(split, condition);
        ir_instr_append_use(split, true_chain);

        struct ir_instr *join = ir_block_append_instr(true_branch, SLIR_JOIN_EXEC_CHAINS);
        ir_instr_append_def(join, chain_reg);
        ir_instr_append_use(join, chain_reg);
        ir_instr_append_use(join, loop_exit_chain);

        cmp_instr = ir_block_append_instr(true_branch, GIR_COMPARE);
        ir_instr_append_use(cmp_instr, true_chain);
        ir_instr_append_use(cmp_instr, no_chain_lit);

        bne = ir_block_append_instr(true_branch, GIR_BRANCH_NOT_EQUAL);
        ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, loop_entry));
        ir_instr_append_use(bne, ir_body_alloc_temp_block(blk->body_, after_true_branch));

        blk = after_true_branch;
        break;
      }
      case slsk_continue:
      case slsk_break:
      case slsk_return:
      case slsk_discard: {
        struct ir_temp *join_chain = NULL;
        switch (stmt->kind_) {
          case slsk_continue: join_chain = frame->continue_chain_; break;
          case slsk_break:    join_chain = frame->break_chain_; break;
          case slsk_return:   join_chain = frame->return_chain_; break;
          case slsk_discard:  join_chain = frame->discard_chain_; break;
        }
        struct ir_instr *rejoin = ir_block_append_instr(blk, SLIR_JOIN_EXEC_CHAINS);
        ir_instr_append_def(rejoin, join_chain);
        ir_instr_append_use(rejoin, chain_reg);
        ir_instr_append_use(rejoin, join_chain);
        struct ir_temp *no_chain_lit = ir_body_alloc_temp_liti(blk->body_, SL_EXEC_NO_CHAIN);
        struct ir_instr *clear_return_chain = ir_block_append_instr(blk, GIR_MOVE);
        ir_instr_append_def(clear_return_chain, chain_reg);
        ir_instr_append_use(clear_return_chain, no_chain_lit);
        break;
      }
      case slsk_compound:
        blk = sl_ir_stmt(blk, chain_reg, frame, stmt->true_branch_);
        break;
    }

    stmt = sl_stmt_next_execution_sibling(stmt);
  }

  return blk;
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
      if (x->function_->builtin_emit_fn_) {
        blk = x->function_->builtin_emit_fn_(blk, chain_reg, frame, x);
      }
      else {
        struct sl_execution_frame child_frame = *frame;
        child_frame.f_ = x->function_;
        /* Establish new frame */
        child_frame.local_float_offset_ += (int)x->function_->frame_.ract_.rra_floats_.watermark_;
        child_frame.local_int_offset_ += (int)x->function_->frame_.ract_.rra_ints_.watermark_;
        child_frame.local_bool_offset_ += (int)x->function_->frame_.ract_.rra_bools_.watermark_;
        child_frame.local_sampler2D_offset_ += (int)x->function_->frame_.ract_.rra_sampler2D_.watermark_;
        child_frame.local_samplerCube_offset_ += (int)x->function_->frame_.ract_.rra_samplerCube_.watermark_;
        size_t n;
        for (n = 0; n < x->function_->num_parameters_; ++n) {
          struct sl_variable *param = x->function_->parameters_[n].variable_;
          int param_qualifiers = sl_type_qualifiers(param->type_);
          struct sl_reg_alloc *param_ra = &param->reg_alloc_;
          sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[n]);
          struct sl_reg_alloc *call_arg_ra = EXPR_RVALUE(x->children_[n]);

          if ((param_qualifiers & SL_PARAMETER_QUALIFIER_IN) ||
              (param_qualifiers & SL_PARAMETER_QUALIFIER_INOUT) ||
              !(param_qualifiers & SL_PARAMETER_QUALIFIER_MASK)) {
            /* Either in, inout, or no parameter qualifier specified: copy the argument in */
            sl_reg_emit_move_crossframe(blk, chain_reg, frame, &x->children_[n]->base_regs_, &x->children_[n]->offset_reg_, &child_frame, param_ra, NULL, 1, 1, 1);
          }
        }

        /* Invoke function body and emit the function inline here */

        struct ir_temp *return_chain = ir_body_alloc_temp_unused_virtual(blk->body_);
        struct ir_temp *no_chain_lit = ir_body_alloc_temp_liti(blk->body_, SL_EXEC_NO_CHAIN);
        struct ir_instr *clear_return_chain = ir_block_append_instr(blk, GIR_MOVE);
        ir_instr_append_def(clear_return_chain, return_chain);
        ir_instr_append_use(clear_return_chain, no_chain_lit);
        child_frame.return_chain_ = return_chain;

        blk = sl_ir_stmt(blk, chain_reg, &child_frame, x->function_->body_);

        struct ir_instr *rejoin = ir_block_append_instr(blk, SLIR_JOIN_EXEC_CHAINS);
        ir_instr_append_def(rejoin, chain_reg);
        ir_instr_append_use(rejoin, chain_reg);
        ir_instr_append_use(rejoin, return_chain);

        for (n = 0; n < x->function_->num_parameters_; ++n) {
          struct sl_variable *param = x->function_->parameters_[n].variable_;
          int param_qualifiers = sl_type_qualifiers(param->type_);
          struct sl_reg_alloc *param_ra = &param->reg_alloc_;
          sl_ir_need_rvalue(blk, chain_reg, frame, x->children_[n]);
          struct sl_reg_alloc *call_arg_ra = EXPR_RVALUE(x->children_[n]);

          if ((param_qualifiers & SL_PARAMETER_QUALIFIER_OUT) ||
              (param_qualifiers & SL_PARAMETER_QUALIFIER_INOUT)) {
            /* Either out, or inout parameter qualifier specified: copy the argument back out into the parameter passed in.. */
            sl_reg_emit_move_crossframe(blk, chain_reg, &child_frame, param_ra, NULL, frame, &x->children_[n]->base_regs_, &x->children_[n]->offset_reg_, 1, 1, 1);
          }
        }
      }

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

