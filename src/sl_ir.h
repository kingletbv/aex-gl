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
#ifndef SL_IR_H
#define SL_IR_H

#ifdef __cplusplus
extern "C" {
#endif

enum generic_ir_instr_code {
  GIR_LOAD8 = 1,
  GIR_LOAD16,
  GIR_LOAD32,
  GIR_LOAD64,

  GIR_STORE,
  GIR_MOVE,

  GIR_IMM8,
  GIR_IMM16,
  GIR_IMM32,
  GIR_IMM64,

  GIR_ADD,
  GIR_SUB,
  GIR_MULS,
  GIR_MULU,
  GIR_MUL,
  GIR_DIVS,
  GIR_DIVU,

  GIR_AND,
  GIR_OR,
  GIR_XOR,
  GIR_LSL,
  GIR_LSR,
  GIR_ASR,

  GIR_NEG,
  GIR_NOT,

  GIR_ZX_8_TO_16,
  GIR_ZX_16_TO_32,
  GIR_ZX_32_TO_64,

  GIR_SX_8_TO_16,
  GIR_SX_16_TO_32,
  GIR_SX_32_TO_64,

  GIR_CULL_64_TO_32,
  GIR_CULL_32_TO_16,
  GIR_CULL_16_TO_8,

  GIR_JUMP,
  GIR_BRANCH_SIGNED_GREATER,
  GIR_BRANCH_SIGNED_GREATER_EQUAL,
  GIR_BRANCH_SIGNED_LESSER,
  GIR_BRANCH_SIGNED_LESSER_EQUAL,

  GIR_BRANCH_UNSIGNED_GREATER,
  GIR_BRANCH_UNSIGNED_GREATER_EQUAL,
  GIR_BRANCH_UNSIGNED_LESSER,
  GIR_BRANCH_UNSIGEND_LESSER_EQUAL,

  GIR_BRANCH_EQUAL,
  GIR_BRANCH_NOT_EQUAL,

  GIR_COMPARE,

  GIR_CALL,
  GIR_RETURN,
  GIR_PUSH,
  GIR_POP,

  GIR_END_OF_GIR_ENUMS
};

enum sl_ir_instr_code {
  SLIR_SPLIT_EXEC_CHAIN_BY_CONDITION = GIR_END_OF_GIR_ENUMS,
  SLIR_JOIN_EXEC_CHAINS,

  SLIR_NEG_F,
  SLIR_NEG_I,

  SLIR_NOT,

  SLIR_MUL_F,
  SLIR_MUL_I,
  SLIR_DOT2,
  SLIR_DOT3,
  SLIR_DOT4,

  SLIR_DIV_F,
  SLIR_DIV_I,

  SLIR_ADD_F,
  SLIR_ADD_I,
  SLIR_SUB_F,
  SLIR_SUB_I,

  SLIR_LT_F,
  SLIR_LT_I,
  SLIR_LE_F,
  SLIR_LE_I,
  SLIR_GE_F,
  SLIR_GE_I,
  SLIR_GT_F,
  SLIR_GT_I,
  SLIR_EQ_F,
  SLIR_EQ_I,
  SLIR_NE_F,
  SLIR_NE_I,


  SLIR_REG_MOVE_F_REG_TO_REG,
  SLIR_REG_MOVE_F_REG_TO_INDIR,
  SLIR_REG_MOVE_F_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_F_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_F_INDIR_TO_REG,
  SLIR_REG_MOVE_F_INDIR_TO_INDIR,
  SLIR_REG_MOVE_F_INDIR_TO_OFFSET_REG,
  SLIR_REG_MOVE_F_INDIR_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_F_OFFSET_REG_TO_REG,
  SLIR_REG_MOVE_F_OFFSET_REG_TO_INDIR,
  SLIR_REG_MOVE_F_OFFSET_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_F_OFFSET_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_F_INDIR_OFFSET_TO_REG,
  SLIR_REG_MOVE_F_INDIR_OFFSET_TO_INDIR,
  SLIR_REG_MOVE_F_INDIR_OFFSET_TO_OFFSET_REG,
  SLIR_REG_MOVE_F_INDIR_OFFSET_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_F2I_REG_TO_REG,
  SLIR_REG_MOVE_F2I_REG_TO_INDIR,
  SLIR_REG_MOVE_F2I_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_F2I_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_F2I_INDIR_TO_REG,
  SLIR_REG_MOVE_F2I_INDIR_TO_INDIR,
  SLIR_REG_MOVE_F2I_INDIR_TO_OFFSET_REG,
  SLIR_REG_MOVE_F2I_INDIR_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_F2I_OFFSET_REG_TO_REG,
  SLIR_REG_MOVE_F2I_OFFSET_REG_TO_INDIR,
  SLIR_REG_MOVE_F2I_OFFSET_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_F2I_OFFSET_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_F2I_INDIR_OFFSET_TO_REG,
  SLIR_REG_MOVE_F2I_INDIR_OFFSET_TO_INDIR,
  SLIR_REG_MOVE_F2I_INDIR_OFFSET_TO_OFFSET_REG,
  SLIR_REG_MOVE_F2I_INDIR_OFFSET_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_F2B_REG_TO_REG,
  SLIR_REG_MOVE_F2B_REG_TO_INDIR,
  SLIR_REG_MOVE_F2B_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_F2B_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_F2B_INDIR_TO_REG,
  SLIR_REG_MOVE_F2B_INDIR_TO_INDIR,
  SLIR_REG_MOVE_F2B_INDIR_TO_OFFSET_REG,
  SLIR_REG_MOVE_F2B_INDIR_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_F2B_OFFSET_REG_TO_REG,
  SLIR_REG_MOVE_F2B_OFFSET_REG_TO_INDIR,
  SLIR_REG_MOVE_F2B_OFFSET_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_F2B_OFFSET_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_F2B_INDIR_OFFSET_TO_REG,
  SLIR_REG_MOVE_F2B_INDIR_OFFSET_TO_INDIR,
  SLIR_REG_MOVE_F2B_INDIR_OFFSET_TO_OFFSET_REG,
  SLIR_REG_MOVE_F2B_INDIR_OFFSET_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_I2F_REG_TO_REG,
  SLIR_REG_MOVE_I2F_REG_TO_INDIR,
  SLIR_REG_MOVE_I2F_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_I2F_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_I2F_INDIR_TO_REG,
  SLIR_REG_MOVE_I2F_INDIR_TO_INDIR,
  SLIR_REG_MOVE_I2F_INDIR_TO_OFFSET_REG,
  SLIR_REG_MOVE_I2F_INDIR_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_I2F_OFFSET_REG_TO_REG,
  SLIR_REG_MOVE_I2F_OFFSET_REG_TO_INDIR,
  SLIR_REG_MOVE_I2F_OFFSET_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_I2F_OFFSET_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_I2F_INDIR_OFFSET_TO_REG,
  SLIR_REG_MOVE_I2F_INDIR_OFFSET_TO_INDIR,
  SLIR_REG_MOVE_I2F_INDIR_OFFSET_TO_OFFSET_REG,
  SLIR_REG_MOVE_I2F_INDIR_OFFSET_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_I_REG_TO_REG,
  SLIR_REG_MOVE_I_REG_TO_INDIR,
  SLIR_REG_MOVE_I_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_I_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_I_INDIR_TO_REG,
  SLIR_REG_MOVE_I_INDIR_TO_INDIR,
  SLIR_REG_MOVE_I_INDIR_TO_OFFSET_REG,
  SLIR_REG_MOVE_I_INDIR_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_I_OFFSET_REG_TO_REG,
  SLIR_REG_MOVE_I_OFFSET_REG_TO_INDIR,
  SLIR_REG_MOVE_I_OFFSET_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_I_OFFSET_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_I_INDIR_OFFSET_TO_REG,
  SLIR_REG_MOVE_I_INDIR_OFFSET_TO_INDIR,
  SLIR_REG_MOVE_I_INDIR_OFFSET_TO_OFFSET_REG,
  SLIR_REG_MOVE_I_INDIR_OFFSET_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_I2B_REG_TO_REG,
  SLIR_REG_MOVE_I2B_REG_TO_INDIR,
  SLIR_REG_MOVE_I2B_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_I2B_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_I2B_INDIR_TO_REG,
  SLIR_REG_MOVE_I2B_INDIR_TO_INDIR,
  SLIR_REG_MOVE_I2B_INDIR_TO_OFFSET_REG,
  SLIR_REG_MOVE_I2B_INDIR_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_I2B_OFFSET_REG_TO_REG,
  SLIR_REG_MOVE_I2B_OFFSET_REG_TO_INDIR,
  SLIR_REG_MOVE_I2B_OFFSET_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_I2B_OFFSET_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_I2B_INDIR_OFFSET_TO_REG,
  SLIR_REG_MOVE_I2B_INDIR_OFFSET_TO_INDIR,
  SLIR_REG_MOVE_I2B_INDIR_OFFSET_TO_OFFSET_REG,
  SLIR_REG_MOVE_I2B_INDIR_OFFSET_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_B2F_REG_TO_REG,
  SLIR_REG_MOVE_B2F_REG_TO_INDIR,
  SLIR_REG_MOVE_B2F_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_B2F_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_B2F_INDIR_TO_REG,
  SLIR_REG_MOVE_B2F_INDIR_TO_INDIR,
  SLIR_REG_MOVE_B2F_INDIR_TO_OFFSET_REG,
  SLIR_REG_MOVE_B2F_INDIR_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_B2F_OFFSET_REG_TO_REG,
  SLIR_REG_MOVE_B2F_OFFSET_REG_TO_INDIR,
  SLIR_REG_MOVE_B2F_OFFSET_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_B2F_OFFSET_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_B2F_INDIR_OFFSET_TO_REG,
  SLIR_REG_MOVE_B2F_INDIR_OFFSET_TO_INDIR,
  SLIR_REG_MOVE_B2F_INDIR_OFFSET_TO_OFFSET_REG,
  SLIR_REG_MOVE_B2F_INDIR_OFFSET_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_B2I_REG_TO_REG,
  SLIR_REG_MOVE_B2I_REG_TO_INDIR,
  SLIR_REG_MOVE_B2I_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_B2I_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_B2I_INDIR_TO_REG,
  SLIR_REG_MOVE_B2I_INDIR_TO_INDIR,
  SLIR_REG_MOVE_B2I_INDIR_TO_OFFSET_REG,
  SLIR_REG_MOVE_B2I_INDIR_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_B2I_OFFSET_REG_TO_REG,
  SLIR_REG_MOVE_B2I_OFFSET_REG_TO_INDIR,
  SLIR_REG_MOVE_B2I_OFFSET_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_B2I_OFFSET_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_B2I_INDIR_OFFSET_TO_REG,
  SLIR_REG_MOVE_B2I_INDIR_OFFSET_TO_INDIR,
  SLIR_REG_MOVE_B2I_INDIR_OFFSET_TO_OFFSET_REG,
  SLIR_REG_MOVE_B2I_INDIR_OFFSET_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_B_REG_TO_REG,
  SLIR_REG_MOVE_B_REG_TO_INDIR,
  SLIR_REG_MOVE_B_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_B_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_B_INDIR_TO_REG,
  SLIR_REG_MOVE_B_INDIR_TO_INDIR,
  SLIR_REG_MOVE_B_INDIR_TO_OFFSET_REG,
  SLIR_REG_MOVE_B_INDIR_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_B_OFFSET_REG_TO_REG,
  SLIR_REG_MOVE_B_OFFSET_REG_TO_INDIR,
  SLIR_REG_MOVE_B_OFFSET_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_B_OFFSET_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_B_INDIR_OFFSET_TO_REG,
  SLIR_REG_MOVE_B_INDIR_OFFSET_TO_INDIR,
  SLIR_REG_MOVE_B_INDIR_OFFSET_TO_OFFSET_REG,
  SLIR_REG_MOVE_B_INDIR_OFFSET_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_S2D_REG_TO_REG,
  SLIR_REG_MOVE_S2D_REG_TO_INDIR,
  SLIR_REG_MOVE_S2D_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_S2D_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_S2D_INDIR_TO_REG,
  SLIR_REG_MOVE_S2D_INDIR_TO_INDIR,
  SLIR_REG_MOVE_S2D_INDIR_TO_OFFSET_REG,
  SLIR_REG_MOVE_S2D_INDIR_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_S2D_OFFSET_REG_TO_REG,
  SLIR_REG_MOVE_S2D_OFFSET_REG_TO_INDIR,
  SLIR_REG_MOVE_S2D_OFFSET_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_S2D_OFFSET_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_S2D_INDIR_OFFSET_TO_REG,
  SLIR_REG_MOVE_S2D_INDIR_OFFSET_TO_INDIR,
  SLIR_REG_MOVE_S2D_INDIR_OFFSET_TO_OFFSET_REG,
  SLIR_REG_MOVE_S2D_INDIR_OFFSET_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_SC_REG_TO_REG,
  SLIR_REG_MOVE_SC_REG_TO_INDIR,
  SLIR_REG_MOVE_SC_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_SC_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_SC_INDIR_TO_REG,
  SLIR_REG_MOVE_SC_INDIR_TO_INDIR,
  SLIR_REG_MOVE_SC_INDIR_TO_OFFSET_REG,
  SLIR_REG_MOVE_SC_INDIR_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_SC_OFFSET_REG_TO_REG,
  SLIR_REG_MOVE_SC_OFFSET_REG_TO_INDIR,
  SLIR_REG_MOVE_SC_OFFSET_REG_TO_OFFSET_REG,
  SLIR_REG_MOVE_SC_OFFSET_REG_TO_INDIR_OFFSET,
  SLIR_REG_MOVE_SC_INDIR_OFFSET_TO_REG,
  SLIR_REG_MOVE_SC_INDIR_OFFSET_TO_INDIR,
  SLIR_REG_MOVE_SC_INDIR_OFFSET_TO_OFFSET_REG,
  SLIR_REG_MOVE_SC_INDIR_OFFSET_TO_INDIR_OFFSET
};


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_IR_H */
