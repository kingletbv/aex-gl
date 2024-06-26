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

#ifndef SL_REG_ALLOC_H_INCLUDED
#define SL_REG_ALLOC_H_INCLUDED
#include "sl_reg_alloc.h"
#endif

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
#endif

static void sl_reg_move_f_reg_to_reg(struct sl_execution *exec,
                                     uint8_t row,
                                     int dst_reg,
                                     int src_reg) {
  if (dst_reg == src_reg) return;
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  float * restrict dst = dst_reg_bank[dst_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const float * restrict src = src_reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const float * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_reg_to_indir(struct sl_execution *exec,
                                       uint8_t row,
                                       int dst_indir_reg,
                                       int dst_fixed_offset,
                                       int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const float * restrict src = src_reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_reg_to_offset_reg(struct sl_execution *exec,
                                            uint8_t row,
                                            int dst_base_reg,
                                            int dst_offset_reg,
                                            int dst_fixed_offset,
                                            int dst_offset_stepsize,
                                            int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const float * restrict src = src_reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_reg_to_indir_offset(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_indir_reg,
                                              int dst_offset_reg,
                                              int dst_fixed_offset,
                                              int dst_offset_stepsize,
                                              int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const float * restrict src = src_reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_indir_to_reg(struct sl_execution *exec,
                                       uint8_t row,
                                       int dst_reg,
                                       int src_indir_reg,
                                       int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  float * restrict dst = dst_reg_bank[dst_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_indir_to_indir(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_indir_reg,
                                         int dst_fixed_offset,
                                         int src_indir_reg,
                                         int src_fixed_offset) {
  if ((dst_indir_reg == src_indir_reg) && (dst_fixed_offset == src_fixed_offset)) return;
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_indir_to_offset_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_base_reg,
                                              int dst_offset_reg,
                                              int dst_fixed_offset,
                                              int dst_offset_stepsize,
                                              int src_indir_reg,
                                              int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_indir_to_indir_offset(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_indir_reg,
                                                int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_offset_reg_to_reg(struct sl_execution *exec,
                                            uint8_t row,
                                            int dst_reg,
                                            int src_base_reg,
                                            int src_offset_reg,
                                            int src_fixed_offset,
                                            int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  float * restrict dst = dst_reg_bank[dst_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const float * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_offset_reg_to_indir(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_indir_reg,
                                              int dst_fixed_offset,
                                              int src_base_reg,
                                              int src_offset_reg,
                                              int src_fixed_offset,
                                              int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_offset_reg_to_offset_reg(struct sl_execution *exec,
                                                   uint8_t row,
                                                   int dst_base_reg,
                                                   int dst_offset_reg,
                                                   int dst_fixed_offset,
                                                   int dst_offset_stepsize,
                                                   int src_base_reg,
                                                   int src_offset_reg,
                                                   int src_fixed_offset,
                                                   int src_offset_stepsize) {
  if ((dst_base_reg == src_base_reg) && (dst_offset_reg == src_offset_reg) && 
      (dst_fixed_offset == src_fixed_offset) && (dst_offset_stepsize == src_offset_stepsize))
    return;
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_offset_reg_to_indir_offset(struct sl_execution *exec,
                                                     uint8_t row,
                                                     int dst_indir_reg,
                                                     int dst_offset_reg,
                                                     int dst_fixed_offset,
                                                     int dst_offset_stepsize,
                                                     int src_base_reg,
                                                     int src_offset_reg,
                                                     int src_fixed_offset,
                                                     int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_indir_offset_to_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_reg,
                                              int src_indir_reg,
                                              int src_offset_reg,
                                              int src_fixed_offset,
                                              int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  float * restrict dst = dst_reg_bank[dst_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_indir_offset_to_indir(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_fixed_offset,
                                                int src_indir_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_indir_offset_to_offset_reg(struct sl_execution *exec,
                                                     uint8_t row,
                                                     int dst_base_reg,
                                                     int dst_offset_reg,
                                                     int dst_fixed_offset,
                                                     int dst_offset_stepsize,
                                                     int src_indir_reg,
                                                     int src_offset_reg,
                                                     int src_fixed_offset,
                                                     int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f_indir_offset_to_indir_offset(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_indir_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_indir_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  if ((dst_indir_reg == src_indir_reg) && (dst_offset_reg == src_offset_reg) && 
      (dst_fixed_offset == src_fixed_offset) && (dst_offset_stepsize == src_offset_stepsize))
    return;
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_reg_to_reg(struct sl_execution *exec,
                                       uint8_t row,
                                       int dst_reg,
                                       int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  int64_t * restrict dst = dst_reg_bank[dst_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const float * restrict src = src_reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const float * restrict src_row = src + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_reg_to_indir(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_indir_reg,
                                         int dst_fixed_offset,
                                         int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const float * restrict src = src_reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_reg_to_offset_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_base_reg,
                                              int dst_offset_reg,
                                              int dst_fixed_offset,
                                              int dst_offset_stepsize,
                                              int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const float * restrict src = src_reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_reg_to_indir_offset(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const float * restrict src = src_reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_indir_to_reg(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_reg,
                                         int src_indir_reg,
                                         int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  int64_t * restrict dst = dst_reg_bank[dst_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_indir_to_indir(struct sl_execution *exec,
                                           uint8_t row,
                                           int dst_indir_reg,
                                           int dst_fixed_offset,
                                           int src_indir_reg,
                                           int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_indir_to_offset_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_base_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_indir_reg,
                                                int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_indir_to_indir_offset(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_offset_reg,
                                                  int dst_fixed_offset,
                                                  int dst_offset_stepsize,
                                                  int src_indir_reg,
                                                  int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_offset_reg_to_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_reg,
                                              int src_base_reg,
                                              int src_offset_reg,
                                              int src_fixed_offset,
                                              int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  int64_t * restrict dst = dst_reg_bank[dst_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const float * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_offset_reg_to_indir(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_fixed_offset,
                                                int src_base_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_offset_reg_to_offset_reg(struct sl_execution *exec,
                                                     uint8_t row,
                                                     int dst_base_reg,
                                                     int dst_offset_reg,
                                                     int dst_fixed_offset,
                                                     int dst_offset_stepsize,
                                                     int src_base_reg,
                                                     int src_offset_reg,
                                                     int src_fixed_offset,
                                                     int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_offset_reg_to_indir_offset(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_indir_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_base_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_indir_offset_to_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_reg,
                                                int src_indir_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  int64_t * restrict dst = dst_reg_bank[dst_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_indir_offset_to_indir(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_fixed_offset,
                                                  int src_indir_reg,
                                                  int src_offset_reg,
                                                  int src_fixed_offset,
                                                  int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_indir_offset_to_offset_reg(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_base_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_indir_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2i_indir_offset_to_indir_offset(struct sl_execution *exec,
                                                         uint8_t row,
                                                         int dst_indir_reg,
                                                         int dst_offset_reg,
                                                         int dst_fixed_offset,
                                                         int dst_offset_stepsize,
                                                         int src_indir_reg,
                                                         int src_offset_reg,
                                                         int src_fixed_offset,
                                                         int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_reg_to_reg(struct sl_execution *exec,
                                       uint8_t row,
                                       int dst_reg,
                                       int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = dst_reg_bank[dst_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const float * restrict src = src_reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const float * restrict src_row = src + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_reg_to_indir(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_indir_reg,
                                         int dst_fixed_offset,
                                         int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const float * restrict src = src_reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_reg_to_offset_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_base_reg,
                                              int dst_offset_reg,
                                              int dst_fixed_offset,
                                              int dst_offset_stepsize,
                                              int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const float * restrict src = src_reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_reg_to_indir_offset(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const float * restrict src = src_reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_indir_to_reg(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_reg,
                                         int src_indir_reg,
                                         int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = dst_reg_bank[dst_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_indir_to_indir(struct sl_execution *exec,
                                           uint8_t row,
                                           int dst_indir_reg,
                                           int dst_fixed_offset,
                                           int src_indir_reg,
                                           int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_indir_to_offset_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_base_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_indir_reg,
                                                int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_indir_to_indir_offset(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_offset_reg,
                                                  int dst_fixed_offset,
                                                  int dst_offset_stepsize,
                                                  int src_indir_reg,
                                                  int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_offset_reg_to_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_reg,
                                              int src_base_reg,
                                              int src_offset_reg,
                                              int src_fixed_offset,
                                              int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = dst_reg_bank[dst_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const float * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_offset_reg_to_indir(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_fixed_offset,
                                                int src_base_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_offset_reg_to_offset_reg(struct sl_execution *exec,
                                                     uint8_t row,
                                                     int dst_base_reg,
                                                     int dst_offset_reg,
                                                     int dst_fixed_offset,
                                                     int dst_offset_stepsize,
                                                     int src_base_reg,
                                                     int src_offset_reg,
                                                     int src_fixed_offset,
                                                     int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_offset_reg_to_indir_offset(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_indir_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_base_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_indir_offset_to_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_reg,
                                                int src_indir_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = dst_reg_bank[dst_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_indir_offset_to_indir(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_fixed_offset,
                                                  int src_indir_reg,
                                                  int src_offset_reg,
                                                  int src_fixed_offset,
                                                  int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_indir_offset_to_offset_reg(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_base_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_indir_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_f2b_indir_offset_to_indir_offset(struct sl_execution *exec,
                                                         uint8_t row,
                                                         int dst_indir_reg,
                                                         int dst_offset_reg,
                                                         int dst_fixed_offset,
                                                         int dst_offset_stepsize,
                                                         int src_indir_reg,
                                                         int src_offset_reg,
                                                         int src_fixed_offset,
                                                         int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  float * restrict * restrict src_reg_bank = exec->float_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const float * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(*src_row != 0.f);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_reg_to_reg(struct sl_execution *exec,
                                       uint8_t row,
                                       int dst_reg,
                                       int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  float * restrict dst = dst_reg_bank[dst_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const int64_t * restrict src_row = src + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_reg_to_indir(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_indir_reg,
                                         int dst_fixed_offset,
                                         int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_reg_to_offset_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_base_reg,
                                              int dst_offset_reg,
                                              int dst_fixed_offset,
                                              int dst_offset_stepsize,
                                              int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_reg_to_indir_offset(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_indir_to_reg(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_reg,
                                         int src_indir_reg,
                                         int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  float * restrict dst = dst_reg_bank[dst_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_indir_to_indir(struct sl_execution *exec,
                                           uint8_t row,
                                           int dst_indir_reg,
                                           int dst_fixed_offset,
                                           int src_indir_reg,
                                           int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_indir_to_offset_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_base_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_indir_reg,
                                                int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_indir_to_indir_offset(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_offset_reg,
                                                  int dst_fixed_offset,
                                                  int dst_offset_stepsize,
                                                  int src_indir_reg,
                                                  int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_offset_reg_to_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_reg,
                                              int src_base_reg,
                                              int src_offset_reg,
                                              int src_fixed_offset,
                                              int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  float * restrict dst = dst_reg_bank[dst_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const int64_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_offset_reg_to_indir(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_fixed_offset,
                                                int src_base_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_offset_reg_to_offset_reg(struct sl_execution *exec,
                                                     uint8_t row,
                                                     int dst_base_reg,
                                                     int dst_offset_reg,
                                                     int dst_fixed_offset,
                                                     int dst_offset_stepsize,
                                                     int src_base_reg,
                                                     int src_offset_reg,
                                                     int src_fixed_offset,
                                                     int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_offset_reg_to_indir_offset(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_indir_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_base_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_indir_offset_to_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_reg,
                                                int src_indir_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  float * restrict dst = dst_reg_bank[dst_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_indir_offset_to_indir(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_fixed_offset,
                                                  int src_indir_reg,
                                                  int src_offset_reg,
                                                  int src_fixed_offset,
                                                  int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_indir_offset_to_offset_reg(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_base_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_indir_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2f_indir_offset_to_indir_offset(struct sl_execution *exec,
                                                         uint8_t row,
                                                         int dst_indir_reg,
                                                         int dst_offset_reg,
                                                         int dst_fixed_offset,
                                                         int dst_offset_stepsize,
                                                         int src_indir_reg,
                                                         int src_offset_reg,
                                                         int src_fixed_offset,
                                                         int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)*src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_reg_to_reg(struct sl_execution *exec,
                                     uint8_t row,
                                     int dst_reg,
                                     int src_reg) {
  if (dst_reg == src_reg) return;
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  int64_t * restrict dst = dst_reg_bank[dst_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const int64_t * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_reg_to_indir(struct sl_execution *exec,
                                       uint8_t row,
                                       int dst_indir_reg,
                                       int dst_fixed_offset,
                                       int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_reg_to_offset_reg(struct sl_execution *exec,
                                            uint8_t row,
                                            int dst_base_reg,
                                            int dst_offset_reg,
                                            int dst_fixed_offset,
                                            int dst_offset_stepsize,
                                            int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_reg_to_indir_offset(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_indir_reg,
                                              int dst_offset_reg,
                                              int dst_fixed_offset,
                                              int dst_offset_stepsize,
                                              int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_indir_to_reg(struct sl_execution *exec,
                                       uint8_t row,
                                       int dst_reg,
                                       int src_indir_reg,
                                       int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  int64_t * restrict dst = dst_reg_bank[dst_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_indir_to_indir(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_indir_reg,
                                         int dst_fixed_offset,
                                         int src_indir_reg,
                                         int src_fixed_offset) {
  if ((dst_indir_reg == src_indir_reg) && (dst_fixed_offset == src_fixed_offset)) return;
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_indir_to_offset_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_base_reg,
                                              int dst_offset_reg,
                                              int dst_fixed_offset,
                                              int dst_offset_stepsize,
                                              int src_indir_reg,
                                              int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_indir_to_indir_offset(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_indir_reg,
                                                int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_offset_reg_to_reg(struct sl_execution *exec,
                                            uint8_t row,
                                            int dst_reg,
                                            int src_base_reg,
                                            int src_offset_reg,
                                            int src_fixed_offset,
                                            int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  int64_t * restrict dst = dst_reg_bank[dst_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const int64_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_offset_reg_to_indir(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_indir_reg,
                                              int dst_fixed_offset,
                                              int src_base_reg,
                                              int src_offset_reg,
                                              int src_fixed_offset,
                                              int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_offset_reg_to_offset_reg(struct sl_execution *exec,
                                                   uint8_t row,
                                                   int dst_base_reg,
                                                   int dst_offset_reg,
                                                   int dst_fixed_offset,
                                                   int dst_offset_stepsize,
                                                   int src_base_reg,
                                                   int src_offset_reg,
                                                   int src_fixed_offset,
                                                   int src_offset_stepsize) {
  if ((dst_base_reg == src_base_reg) && (dst_offset_reg == src_offset_reg) && 
      (dst_fixed_offset == src_fixed_offset) && (dst_offset_stepsize == src_offset_stepsize))
    return;
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_offset_reg_to_indir_offset(struct sl_execution *exec,
                                                     uint8_t row,
                                                     int dst_indir_reg,
                                                     int dst_offset_reg,
                                                     int dst_fixed_offset,
                                                     int dst_offset_stepsize,
                                                     int src_base_reg,
                                                     int src_offset_reg,
                                                     int src_fixed_offset,
                                                     int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_indir_offset_to_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_reg,
                                              int src_indir_reg,
                                              int src_offset_reg,
                                              int src_fixed_offset,
                                              int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  int64_t * restrict dst = dst_reg_bank[dst_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_indir_offset_to_indir(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_fixed_offset,
                                                int src_indir_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_indir_offset_to_offset_reg(struct sl_execution *exec,
                                                     uint8_t row,
                                                     int dst_base_reg,
                                                     int dst_offset_reg,
                                                     int dst_fixed_offset,
                                                     int dst_offset_stepsize,
                                                     int src_indir_reg,
                                                     int src_offset_reg,
                                                     int src_fixed_offset,
                                                     int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_indir_offset_to_indir_offset(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_indir_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_indir_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  if ((dst_indir_reg == src_indir_reg) && (dst_offset_reg == src_offset_reg) && 
      (dst_fixed_offset == src_fixed_offset) && (dst_offset_stepsize == src_offset_stepsize))
    return;
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_reg_to_reg(struct sl_execution *exec,
                                       uint8_t row,
                                       int dst_reg,
                                       int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = dst_reg_bank[dst_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const int64_t * restrict src_row = src + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_reg_to_indir(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_indir_reg,
                                         int dst_fixed_offset,
                                         int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_reg_to_offset_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_base_reg,
                                              int dst_offset_reg,
                                              int dst_fixed_offset,
                                              int dst_offset_stepsize,
                                              int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_reg_to_indir_offset(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_indir_to_reg(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_reg,
                                         int src_indir_reg,
                                         int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = dst_reg_bank[dst_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_indir_to_indir(struct sl_execution *exec,
                                           uint8_t row,
                                           int dst_indir_reg,
                                           int dst_fixed_offset,
                                           int src_indir_reg,
                                           int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_indir_to_offset_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_base_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_indir_reg,
                                                int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_indir_to_indir_offset(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_offset_reg,
                                                  int dst_fixed_offset,
                                                  int dst_offset_stepsize,
                                                  int src_indir_reg,
                                                  int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_offset_reg_to_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_reg,
                                              int src_base_reg,
                                              int src_offset_reg,
                                              int src_fixed_offset,
                                              int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = dst_reg_bank[dst_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const int64_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_offset_reg_to_indir(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_fixed_offset,
                                                int src_base_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_offset_reg_to_offset_reg(struct sl_execution *exec,
                                                     uint8_t row,
                                                     int dst_base_reg,
                                                     int dst_offset_reg,
                                                     int dst_fixed_offset,
                                                     int dst_offset_stepsize,
                                                     int src_base_reg,
                                                     int src_offset_reg,
                                                     int src_fixed_offset,
                                                     int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_offset_reg_to_indir_offset(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_indir_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_base_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_indir_offset_to_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_reg,
                                                int src_indir_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = dst_reg_bank[dst_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_indir_offset_to_indir(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_fixed_offset,
                                                  int src_indir_reg,
                                                  int src_offset_reg,
                                                  int src_fixed_offset,
                                                  int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_indir_offset_to_offset_reg(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_base_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_indir_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i2b_indir_offset_to_indir_offset(struct sl_execution *exec,
                                                         uint8_t row,
                                                         int dst_indir_reg,
                                                         int dst_offset_reg,
                                                         int dst_fixed_offset,
                                                         int dst_offset_stepsize,
                                                         int src_indir_reg,
                                                         int src_offset_reg,
                                                         int src_fixed_offset,
                                                         int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  int64_t * restrict * restrict src_reg_bank = exec->int_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const int64_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (uint8_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_reg_to_reg(struct sl_execution *exec,
                                       uint8_t row,
                                       int dst_reg,
                                       int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  float * restrict dst = dst_reg_bank[dst_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const uint8_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = src + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_reg_to_indir(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_indir_reg,
                                         int dst_fixed_offset,
                                         int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const uint8_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_reg_to_offset_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_base_reg,
                                              int dst_offset_reg,
                                              int dst_fixed_offset,
                                              int dst_offset_stepsize,
                                              int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const uint8_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_reg_to_indir_offset(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const uint8_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_indir_to_reg(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_reg,
                                         int src_indir_reg,
                                         int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  float * restrict dst = dst_reg_bank[dst_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_indir_to_indir(struct sl_execution *exec,
                                           uint8_t row,
                                           int dst_indir_reg,
                                           int dst_fixed_offset,
                                           int src_indir_reg,
                                           int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_indir_to_offset_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_base_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_indir_reg,
                                                int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_indir_to_indir_offset(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_offset_reg,
                                                  int dst_fixed_offset,
                                                  int dst_offset_stepsize,
                                                  int src_indir_reg,
                                                  int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_offset_reg_to_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_reg,
                                              int src_base_reg,
                                              int src_offset_reg,
                                              int src_fixed_offset,
                                              int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  float * restrict dst = dst_reg_bank[dst_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_offset_reg_to_indir(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_fixed_offset,
                                                int src_base_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_offset_reg_to_offset_reg(struct sl_execution *exec,
                                                     uint8_t row,
                                                     int dst_base_reg,
                                                     int dst_offset_reg,
                                                     int dst_fixed_offset,
                                                     int dst_offset_stepsize,
                                                     int src_base_reg,
                                                     int src_offset_reg,
                                                     int src_fixed_offset,
                                                     int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_offset_reg_to_indir_offset(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_indir_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_base_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_indir_offset_to_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_reg,
                                                int src_indir_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  float * restrict dst = dst_reg_bank[dst_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_indir_offset_to_indir(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_fixed_offset,
                                                  int src_indir_reg,
                                                  int src_offset_reg,
                                                  int src_fixed_offset,
                                                  int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_indir_offset_to_offset_reg(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_base_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_indir_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2f_indir_offset_to_indir_offset(struct sl_execution *exec,
                                                         uint8_t row,
                                                         int dst_indir_reg,
                                                         int dst_offset_reg,
                                                         int dst_fixed_offset,
                                                         int dst_offset_stepsize,
                                                         int src_indir_reg,
                                                         int src_offset_reg,
                                                         int src_fixed_offset,
                                                         int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict dst_reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (float)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_reg_to_reg(struct sl_execution *exec,
                                       uint8_t row,
                                       int dst_reg,
                                       int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  int64_t * restrict dst = dst_reg_bank[dst_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const uint8_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = src + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_reg_to_indir(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_indir_reg,
                                         int dst_fixed_offset,
                                         int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const uint8_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_reg_to_offset_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_base_reg,
                                              int dst_offset_reg,
                                              int dst_fixed_offset,
                                              int dst_offset_stepsize,
                                              int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const uint8_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_reg_to_indir_offset(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const uint8_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_indir_to_reg(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_reg,
                                         int src_indir_reg,
                                         int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  int64_t * restrict dst = dst_reg_bank[dst_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_indir_to_indir(struct sl_execution *exec,
                                           uint8_t row,
                                           int dst_indir_reg,
                                           int dst_fixed_offset,
                                           int src_indir_reg,
                                           int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_indir_to_offset_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_base_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_indir_reg,
                                                int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_indir_to_indir_offset(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_offset_reg,
                                                  int dst_fixed_offset,
                                                  int dst_offset_stepsize,
                                                  int src_indir_reg,
                                                  int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_offset_reg_to_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_reg,
                                              int src_base_reg,
                                              int src_offset_reg,
                                              int src_fixed_offset,
                                              int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  int64_t * restrict dst = dst_reg_bank[dst_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_offset_reg_to_indir(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_fixed_offset,
                                                int src_base_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_offset_reg_to_offset_reg(struct sl_execution *exec,
                                                     uint8_t row,
                                                     int dst_base_reg,
                                                     int dst_offset_reg,
                                                     int dst_fixed_offset,
                                                     int dst_offset_stepsize,
                                                     int src_base_reg,
                                                     int src_offset_reg,
                                                     int src_fixed_offset,
                                                     int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_offset_reg_to_indir_offset(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_indir_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_base_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_indir_offset_to_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_reg,
                                                int src_indir_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  int64_t * restrict dst = dst_reg_bank[dst_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_indir_offset_to_indir(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_fixed_offset,
                                                  int src_indir_reg,
                                                  int src_offset_reg,
                                                  int src_fixed_offset,
                                                  int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_indir_offset_to_offset_reg(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_base_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_indir_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b2i_indir_offset_to_indir_offset(struct sl_execution *exec,
                                                         uint8_t row,
                                                         int dst_indir_reg,
                                                         int dst_offset_reg,
                                                         int dst_fixed_offset,
                                                         int dst_offset_stepsize,
                                                         int src_indir_reg,
                                                         int src_offset_reg,
                                                         int src_fixed_offset,
                                                         int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict dst_reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = (int64_t)(!!*src_row);

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_reg_to_reg(struct sl_execution *exec,
                                     uint8_t row,
                                     int dst_reg,
                                     int src_reg) {
  if (dst_reg == src_reg) return;
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = dst_reg_bank[dst_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const uint8_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_reg_to_indir(struct sl_execution *exec,
                                       uint8_t row,
                                       int dst_indir_reg,
                                       int dst_fixed_offset,
                                       int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const uint8_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_reg_to_offset_reg(struct sl_execution *exec,
                                            uint8_t row,
                                            int dst_base_reg,
                                            int dst_offset_reg,
                                            int dst_fixed_offset,
                                            int dst_offset_stepsize,
                                            int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const uint8_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_reg_to_indir_offset(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_indir_reg,
                                              int dst_offset_reg,
                                              int dst_fixed_offset,
                                              int dst_offset_stepsize,
                                              int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const uint8_t * restrict src = src_reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_indir_to_reg(struct sl_execution *exec,
                                       uint8_t row,
                                       int dst_reg,
                                       int src_indir_reg,
                                       int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = dst_reg_bank[dst_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_indir_to_indir(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_indir_reg,
                                         int dst_fixed_offset,
                                         int src_indir_reg,
                                         int src_fixed_offset) {
  if ((dst_indir_reg == src_indir_reg) && (dst_fixed_offset == src_fixed_offset)) return;
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_indir_to_offset_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_base_reg,
                                              int dst_offset_reg,
                                              int dst_fixed_offset,
                                              int dst_offset_stepsize,
                                              int src_indir_reg,
                                              int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_indir_to_indir_offset(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_indir_reg,
                                                int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_offset_reg_to_reg(struct sl_execution *exec,
                                            uint8_t row,
                                            int dst_reg,
                                            int src_base_reg,
                                            int src_offset_reg,
                                            int src_fixed_offset,
                                            int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = dst_reg_bank[dst_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_offset_reg_to_indir(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_indir_reg,
                                              int dst_fixed_offset,
                                              int src_base_reg,
                                              int src_offset_reg,
                                              int src_fixed_offset,
                                              int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_offset_reg_to_offset_reg(struct sl_execution *exec,
                                                   uint8_t row,
                                                   int dst_base_reg,
                                                   int dst_offset_reg,
                                                   int dst_fixed_offset,
                                                   int dst_offset_stepsize,
                                                   int src_base_reg,
                                                   int src_offset_reg,
                                                   int src_fixed_offset,
                                                   int src_offset_stepsize) {
  if ((dst_base_reg == src_base_reg) && (dst_offset_reg == src_offset_reg) && 
      (dst_fixed_offset == src_fixed_offset) && (dst_offset_stepsize == src_offset_stepsize))
    return;
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_offset_reg_to_indir_offset(struct sl_execution *exec,
                                                     uint8_t row,
                                                     int dst_indir_reg,
                                                     int dst_offset_reg,
                                                     int dst_fixed_offset,
                                                     int dst_offset_stepsize,
                                                     int src_base_reg,
                                                     int src_offset_reg,
                                                     int src_fixed_offset,
                                                     int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_indir_offset_to_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_reg,
                                              int src_indir_reg,
                                              int src_offset_reg,
                                              int src_fixed_offset,
                                              int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = dst_reg_bank[dst_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_indir_offset_to_indir(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_fixed_offset,
                                                int src_indir_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_indir_offset_to_offset_reg(struct sl_execution *exec,
                                                     uint8_t row,
                                                     int dst_base_reg,
                                                     int dst_offset_reg,
                                                     int dst_fixed_offset,
                                                     int dst_offset_stepsize,
                                                     int src_indir_reg,
                                                     int src_offset_reg,
                                                     int src_fixed_offset,
                                                     int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_indir_offset_to_indir_offset(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_indir_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_indir_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  if ((dst_indir_reg == src_indir_reg) && (dst_offset_reg == src_offset_reg) && 
      (dst_fixed_offset == src_fixed_offset) && (dst_offset_stepsize == src_offset_stepsize))
    return;
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict dst_reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  uint8_t * restrict * restrict src_reg_bank = exec->bool_regs_;
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst_reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    const uint8_t * restrict src_row = src_reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_reg_to_reg(struct sl_execution *exec,
                                       uint8_t row,
                                       int dst_reg,
                                       int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  void * restrict * restrict dst = reg_bank[dst_reg];
  void * const restrict * restrict src = reg_bank[src_reg];
  for (;;) {
    void * restrict * restrict dst_row = dst + row;
    void * const restrict * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_reg_to_indir(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_indir_reg,
                                         int dst_fixed_offset,
                                         int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  void * const restrict * restrict src = reg_bank[src_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_reg_to_offset_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_base_reg,
                                              int dst_offset_reg,
                                              int dst_fixed_offset,
                                              int dst_offset_stepsize,
                                              int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  void * const restrict * restrict src = reg_bank[src_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_reg_to_indir_offset(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  void * const restrict * restrict src = reg_bank[src_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_indir_to_reg(struct sl_execution *exec,
                                         uint8_t row,
                                         int dst_reg,
                                         int src_indir_reg,
                                         int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  void * restrict * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    void * restrict * restrict dst_row = dst + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_indir_to_indir(struct sl_execution *exec,
                                           uint8_t row,
                                           int dst_indir_reg,
                                           int dst_fixed_offset,
                                           int src_indir_reg,
                                           int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_indir_to_offset_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_base_reg,
                                                int dst_offset_reg,
                                                int dst_fixed_offset,
                                                int dst_offset_stepsize,
                                                int src_indir_reg,
                                                int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_indir_to_indir_offset(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_offset_reg,
                                                  int dst_fixed_offset,
                                                  int dst_offset_stepsize,
                                                  int src_indir_reg,
                                                  int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_offset_reg_to_reg(struct sl_execution *exec,
                                              uint8_t row,
                                              int dst_reg,
                                              int src_base_reg,
                                              int src_offset_reg,
                                              int src_fixed_offset,
                                              int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  void * restrict * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = dst + row;
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_offset_reg_to_indir(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_indir_reg,
                                                int dst_fixed_offset,
                                                int src_base_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_offset_reg_to_offset_reg(struct sl_execution *exec,
                                                     uint8_t row,
                                                     int dst_base_reg,
                                                     int dst_offset_reg,
                                                     int dst_fixed_offset,
                                                     int dst_offset_stepsize,
                                                     int src_base_reg,
                                                     int src_offset_reg,
                                                     int src_fixed_offset,
                                                     int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_offset_reg_to_indir_offset(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_indir_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_base_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_indir_offset_to_reg(struct sl_execution *exec,
                                                uint8_t row,
                                                int dst_reg,
                                                int src_indir_reg,
                                                int src_offset_reg,
                                                int src_fixed_offset,
                                                int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  void * restrict * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = dst + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_indir_offset_to_indir(struct sl_execution *exec,
                                                  uint8_t row,
                                                  int dst_indir_reg,
                                                  int dst_fixed_offset,
                                                  int src_indir_reg,
                                                  int src_offset_reg,
                                                  int src_fixed_offset,
                                                  int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_indir_offset_to_offset_reg(struct sl_execution *exec,
                                                       uint8_t row,
                                                       int dst_base_reg,
                                                       int dst_offset_reg,
                                                       int dst_fixed_offset,
                                                       int dst_offset_stepsize,
                                                       int src_indir_reg,
                                                       int src_offset_reg,
                                                       int src_fixed_offset,
                                                       int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_s2d_indir_offset_to_indir_offset(struct sl_execution *exec,
                                                         uint8_t row,
                                                         int dst_indir_reg,
                                                         int dst_offset_reg,
                                                         int dst_fixed_offset,
                                                         int dst_offset_stepsize,
                                                         int src_indir_reg,
                                                         int src_offset_reg,
                                                         int src_fixed_offset,
                                                         int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_2D_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_reg_to_reg(struct sl_execution *exec,
                                      uint8_t row,
                                      int dst_reg,
                                      int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  void * restrict * restrict dst = reg_bank[dst_reg];
  void * const restrict * restrict src = reg_bank[src_reg];
  for (;;) {
    void * restrict * restrict dst_row = dst + row;
    void * const restrict * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_reg_to_indir(struct sl_execution *exec,
                                        uint8_t row,
                                        int dst_indir_reg,
                                        int dst_fixed_offset,
                                        int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  void * const restrict * restrict src = reg_bank[src_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_reg_to_offset_reg(struct sl_execution *exec,
                                             uint8_t row,
                                             int dst_base_reg,
                                             int dst_offset_reg,
                                             int dst_fixed_offset,
                                             int dst_offset_stepsize,
                                             int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  void * const restrict * restrict src = reg_bank[src_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_reg_to_indir_offset(struct sl_execution *exec,
                                               uint8_t row,
                                               int dst_indir_reg,
                                               int dst_offset_reg,
                                               int dst_fixed_offset,
                                               int dst_offset_stepsize,
                                               int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  void * const restrict * restrict src = reg_bank[src_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = src + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_indir_to_reg(struct sl_execution *exec,
                                        uint8_t row,
                                        int dst_reg,
                                        int src_indir_reg,
                                        int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  void * restrict * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    void * restrict * restrict dst_row = dst + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_indir_to_indir(struct sl_execution *exec,
                                          uint8_t row,
                                          int dst_indir_reg,
                                          int dst_fixed_offset,
                                          int src_indir_reg,
                                          int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_indir_to_offset_reg(struct sl_execution *exec,
                                               uint8_t row,
                                               int dst_base_reg,
                                               int dst_offset_reg,
                                               int dst_fixed_offset,
                                               int dst_offset_stepsize,
                                               int src_indir_reg,
                                               int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_indir_to_indir_offset(struct sl_execution *exec,
                                                 uint8_t row,
                                                 int dst_indir_reg,
                                                 int dst_offset_reg,
                                                 int dst_fixed_offset,
                                                 int dst_offset_stepsize,
                                                 int src_indir_reg,
                                                 int src_fixed_offset) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_offset_reg_to_reg(struct sl_execution *exec,
                                             uint8_t row,
                                             int dst_reg,
                                             int src_base_reg,
                                             int src_offset_reg,
                                             int src_fixed_offset,
                                             int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  void * restrict * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = dst + row;
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_offset_reg_to_indir(struct sl_execution *exec,
                                               uint8_t row,
                                               int dst_indir_reg,
                                               int dst_fixed_offset,
                                               int src_base_reg,
                                               int src_offset_reg,
                                               int src_fixed_offset,
                                               int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_offset_reg_to_offset_reg(struct sl_execution *exec,
                                                    uint8_t row,
                                                    int dst_base_reg,
                                                    int dst_offset_reg,
                                                    int dst_fixed_offset,
                                                    int dst_offset_stepsize,
                                                    int src_base_reg,
                                                    int src_offset_reg,
                                                    int src_fixed_offset,
                                                    int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_offset_reg_to_indir_offset(struct sl_execution *exec,
                                                      uint8_t row,
                                                      int dst_indir_reg,
                                                      int dst_offset_reg,
                                                      int dst_fixed_offset,
                                                      int dst_offset_stepsize,
                                                      int src_base_reg,
                                                      int src_offset_reg,
                                                      int src_fixed_offset,
                                                      int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_indir_offset_to_reg(struct sl_execution *exec,
                                               uint8_t row,
                                               int dst_reg,
                                               int src_indir_reg,
                                               int src_offset_reg,
                                               int src_fixed_offset,
                                               int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  void * restrict * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = dst + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_indir_offset_to_indir(struct sl_execution *exec,
                                                 uint8_t row,
                                                 int dst_indir_reg,
                                                 int dst_fixed_offset,
                                                 int src_indir_reg,
                                                 int src_offset_reg,
                                                 int src_fixed_offset,
                                                 int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_indir_offset_to_offset_reg(struct sl_execution *exec,
                                                      uint8_t row,
                                                      int dst_base_reg,
                                                      int dst_offset_reg,
                                                      int dst_fixed_offset,
                                                      int dst_offset_stepsize,
                                                      int src_indir_reg,
                                                      int src_offset_reg,
                                                      int src_fixed_offset,
                                                      int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_sc_indir_offset_to_indir_offset(struct sl_execution *exec,
                                                        uint8_t row,
                                                        int dst_indir_reg,
                                                        int dst_offset_reg,
                                                        int dst_fixed_offset,
                                                        int dst_offset_stepsize,
                                                        int src_indir_reg,
                                                        int src_offset_reg,
                                                        int src_fixed_offset,
                                                        int src_offset_stepsize) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  void * restrict * restrict * restrict reg_bank = exec->sampler_cube_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset] + row;
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset] + row;

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

void sl_reg_move_crossframe(struct sl_execution *exec,
                            uint8_t row,
                            int from_frame, struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset,
                            int to_frame, struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset,
                            int from_offset_step_size, int to_offset_step_size,
                            int array_quantity) {
  size_t num_components = 0;
  int scalar_to_scalar_conversion = 0;
  size_t n;
  int k;
  switch (from_ra->kind_) {
    case slrak_array: {
      if (from_ra->kind_ != to_ra->kind_) return;
      if (from_ra->v_.array_.num_elements_ != to_ra->v_.array_.num_elements_) return;
      int new_from_offset_step_size = (int)(from_offset_step_size * from_ra->v_.array_.num_elements_);
      int new_to_offset_step_size = (int)(to_offset_step_size * from_ra->v_.array_.num_elements_);
      int new_array_quantity = (int)(array_quantity * from_ra->v_.array_.num_elements_);
      sl_reg_move_crossframe(exec, row, from_frame, from_ra->v_.array_.head_, from_ra_offset, to_frame, to_ra->v_.array_.head_, to_ra_offset, new_from_offset_step_size, new_to_offset_step_size, new_array_quantity);
      return;
    }
    case slrak_struct: {
      if (from_ra->kind_ != to_ra->kind_) return;
      if (from_ra->v_.comp_.num_fields_ != to_ra->v_.comp_.num_fields_) return;
      for (n = 0; n < from_ra->v_.comp_.num_fields_; ++n) {
        sl_reg_move_crossframe(exec, row, from_frame, from_ra->v_.comp_.fields_ + n, from_ra_offset, to_frame, to_ra->v_.comp_.fields_ + n, to_ra_offset, from_offset_step_size, to_offset_step_size, array_quantity);
      }
      return;
    }
    case slrak_float: num_components = 1; scalar_to_scalar_conversion = 0x10; break;
    case slrak_vec2:  num_components = 2; scalar_to_scalar_conversion = 0x10; break;
    case slrak_vec3:  num_components = 3; scalar_to_scalar_conversion = 0x10; break;
    case slrak_vec4:  num_components = 4; scalar_to_scalar_conversion = 0x10; break;
    case slrak_mat2:  num_components = 4; scalar_to_scalar_conversion = 0x10; break;
    case slrak_mat3:  num_components = 9; scalar_to_scalar_conversion = 0x10; break;
    case slrak_mat4:  num_components = 16; scalar_to_scalar_conversion = 0x10; break;
    case slrak_int:   num_components = 1; scalar_to_scalar_conversion = 0x20; break;
    case slrak_ivec2: num_components = 2; scalar_to_scalar_conversion = 0x20; break;
    case slrak_ivec3: num_components = 3; scalar_to_scalar_conversion = 0x20; break;
    case slrak_ivec4: num_components = 4; scalar_to_scalar_conversion = 0x20; break;
    case slrak_bool:  num_components = 1; scalar_to_scalar_conversion = 0x30; break;
    case slrak_bvec2: num_components = 2; scalar_to_scalar_conversion = 0x30; break;
    case slrak_bvec3: num_components = 3; scalar_to_scalar_conversion = 0x30; break;
    case slrak_bvec4: num_components = 4; scalar_to_scalar_conversion = 0x30; break;
  }
  switch (to_ra->kind_) {
    case slrak_float: if (num_components != 1) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_vec2:  if (num_components != 2) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_vec3:  if (num_components != 3) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_vec4:  if (num_components != 4) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_mat2:  if (num_components != 4) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_mat3:  if (num_components != 9) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_mat4:  if (num_components !=16) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_int:   if (num_components != 1) return; scalar_to_scalar_conversion += 0x2; break;
    case slrak_ivec2: if (num_components != 2) return; scalar_to_scalar_conversion += 0x2; break;
    case slrak_ivec3: if (num_components != 3) return; scalar_to_scalar_conversion += 0x2; break;
    case slrak_ivec4: if (num_components != 4) return; scalar_to_scalar_conversion += 0x2; break;
    case slrak_bool:  if (num_components != 1) return; scalar_to_scalar_conversion += 0x3; break;
    case slrak_bvec2: if (num_components != 2) return; scalar_to_scalar_conversion += 0x3; break;
    case slrak_bvec3: if (num_components != 3) return; scalar_to_scalar_conversion += 0x3; break;
    case slrak_bvec4: if (num_components != 4) return; scalar_to_scalar_conversion += 0x3; break;
    default: if (scalar_to_scalar_conversion) return;
  }
  switch (from_ra->kind_) {
    case slrak_float:
    case slrak_vec2:
    case slrak_vec3:
    case slrak_vec4:
    case slrak_mat2:
    case slrak_mat3:
    case slrak_mat4:
    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4:
    case slrak_bool:
    case slrak_bvec2:
    case slrak_bvec3:
    case slrak_bvec4: {
if (to_ra->is_indirect_) {
  /* to is indirect */
  if (from_ra->is_indirect_) {
    /* from is indirect */
    if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
      /* have a to offset */
      if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
        /* have a from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_indir_offset_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                             to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                             k,
                                                             to_offset_step_size,
                                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                             from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                             k,
                                                             from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                             to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                             k,
                                                             to_offset_step_size,
                                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                             from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                             k,
                                                             from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                             to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                             k,
                                                             to_offset_step_size,
                                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                             from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                             k,
                                                             from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_indir_offset_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                             to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                             k,
                                                             to_offset_step_size,
                                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                             from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                             k,
                                                             from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                             to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                             k,
                                                             to_offset_step_size,
                                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                             from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                             k,
                                                             from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                             to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                             k,
                                                             to_offset_step_size,
                                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                             from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                             k,
                                                             from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_indir_offset_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
      else {
        /* no from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_indir_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_indir_to_indir_offset(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                      to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                      k,
                                                      to_offset_step_size,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                      k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_indir_to_indir_offset(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                      to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                      k,
                                                      to_offset_step_size,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                      k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_indir_to_indir_offset(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                      to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                      k,
                                                      to_offset_step_size,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                      k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_indir_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_indir_to_indir_offset(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                      to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                      k,
                                                      to_offset_step_size,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                      k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_indir_to_indir_offset(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                      to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                      k,
                                                      to_offset_step_size,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                      k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_indir_to_indir_offset(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                      to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                      k,
                                                      to_offset_step_size,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                      k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_indir_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
    }
    else {
      /* no to offset */
      if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
        /* have a from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_indir_offset_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_indir_offset_to_indir(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                      k,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                      from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                      k,
                                                      from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_indir_offset_to_indir(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                      k,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                      from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                      k,
                                                      from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_indir_offset_to_indir(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                      k,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                      from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                      k,
                                                      from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_indir_offset_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_indir_offset_to_indir(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                      k,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                      from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                      k,
                                                      from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_indir_offset_to_indir(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                      k,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                      from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                      k,
                                                      from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_indir_offset_to_indir(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                      k,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                      from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                      k,
                                                      from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_indir_offset_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
      else {
        /* no from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_indir_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                             k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_indir_to_indir(exec,
                                               row,
                                               to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                               k,
                                               from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                               k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_indir_to_indir(exec,
                                               row,
                                               to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                               k,
                                               from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                               k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_indir_to_indir(exec,
                                               row,
                                               to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                               k,
                                               from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                               k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_indir_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                             k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_indir_to_indir(exec,
                                               row,
                                               to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                               k,
                                               from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                               k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_indir_to_indir(exec,
                                               row,
                                               to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                               k,
                                               from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                               k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_indir_to_indir(exec,
                                               row,
                                               to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                               k,
                                               from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                               k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_indir_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                             k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
    }
  }
  else {
    /* from is direct */
    if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
      /* have a to offset */
      if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
        /* have a from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_offset_reg_to_indir_offset(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         k,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           0,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           0,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           0,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_offset_reg_to_indir_offset(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         k,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           0,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           0,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           0,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_offset_reg_to_indir_offset(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         k,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
      else {
        /* no from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_reg_to_indir_offset(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  k,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_reg_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_reg_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_reg_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_reg_to_indir_offset(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  k,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_reg_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_reg_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_reg_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_reg_to_indir_offset(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] : to_ra->v_.regs_[n],
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  k,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
    }
    else {
      /* no to offset */
      if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
        /* have a from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_offset_reg_to_indir(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                  k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_offset_reg_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    0,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_offset_reg_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    0,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_offset_reg_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    0,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_offset_reg_to_indir(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                  k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_offset_reg_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    0,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_offset_reg_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    0,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_offset_reg_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    0,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_offset_reg_to_indir(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                  k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
      else {
        /* no from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_reg_to_indir(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                           k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_reg_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_reg_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_reg_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_reg_to_indir(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                           k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_reg_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_reg_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_reg_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_reg_to_indir(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                           k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
    }
  }
}
else {
  /* to is direct */
  if (from_ra->is_indirect_) {
    /* from is indirect */
    if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
      /* have a to offset */
      if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
        /* have a from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_indir_offset_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         k,
                                                         from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           0,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           0,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           0,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_indir_offset_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         k,
                                                         from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           0,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           0,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           0,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_indir_offset_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         k,
                                                         from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
      else {
        /* no from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_indir_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                  k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_indir_to_offset_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    0,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_indir_to_offset_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    0,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_indir_to_offset_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    0,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_indir_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                  k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_indir_to_offset_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    0,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_indir_to_offset_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    0,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_indir_to_offset_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    0,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_indir_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                  k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
    }
    else {
      /* no to offset */
      if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
        /* have a from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_indir_offset_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  k,
                                                  from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_indir_offset_to_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_indir_offset_to_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_indir_offset_to_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_indir_offset_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  k,
                                                  from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_indir_offset_to_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_indir_offset_to_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_indir_offset_to_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_indir_offset_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  k,
                                                  from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
      else {
        /* no from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_indir_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                           k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_indir_to_reg(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                             k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_indir_to_reg(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                             k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_indir_to_reg(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                             k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_indir_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                           k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_indir_to_reg(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                             k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_indir_to_reg(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                             k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_indir_to_reg(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                             k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_indir_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] : from_ra->v_.regs_[n],
                                           k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
    }
  }
  else {
    /* from is direct */
    if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
      /* have a to offset */
      if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
        /* have a from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_offset_reg_to_offset_reg(exec,
                                                       row,
                                                       to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                       to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                       0,
                                                       to_offset_step_size,
                                                       from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                       from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                       0,
                                                       from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_offset_reg_to_offset_reg(exec,
                                                       row,
                                                       to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                       to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                       0,
                                                       to_offset_step_size,
                                                       from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                       from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                       0,
                                                       from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_offset_reg_to_offset_reg(exec,
                                                       row,
                                                       to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                       to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                       0,
                                                       to_offset_step_size,
                                                       from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                       from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                       0,
                                                       from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
      else {
        /* no from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_reg_to_offset_reg(exec,
                                                row,
                                                to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                0,
                                                to_offset_step_size,
                                                from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_reg_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_reg_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_reg_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_reg_to_offset_reg(exec,
                                                row,
                                                to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                0,
                                                to_offset_step_size,
                                                from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_reg_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_reg_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_reg_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_reg_to_offset_reg(exec,
                                                row,
                                                to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                0,
                                                to_offset_step_size,
                                                from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
    }
    else {
      /* no to offset */
      if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
        /* have a from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_offset_reg_to_reg(exec,
                                                row,
                                                to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                0,
                                                from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_offset_reg_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_offset_reg_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_offset_reg_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_offset_reg_to_reg(exec,
                                                row,
                                                to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                0,
                                                from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_offset_reg_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_offset_reg_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_offset_reg_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_offset_reg_to_reg(exec,
                                                row,
                                                to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                                from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k,
                                                from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                0,
                                                from_offset_step_size);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
      else {
        /* no from offset */
        switch (scalar_to_scalar_conversion) {
          case 0x11: /* float to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f_reg_to_reg(exec,
                                         row,
                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x11: float to float */
          case 0x12: /* float to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2i_reg_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x12: float to int */
          case 0x13: /* float to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_f2b_reg_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x13: float to bool */
          case 0x21: /* int to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2f_reg_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x21: int to float */
          case 0x22: /* int to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i_reg_to_reg(exec,
                                         row,
                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x22: int to int */
          case 0x23: /* int to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_i2b_reg_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x23: int to bool */
          case 0x31: /* bool to float */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2f_reg_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x31: bool to float */
          case 0x32: /* bool to int */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b2i_reg_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x32: bool to int */
          case 0x33: /* bool to bool */ {
            for (k = 0; k < array_quantity; ++k) {
              for (n = 0; n < num_components; ++n) {
                sl_reg_move_b_reg_to_reg(exec,
                                         row,
                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[n] + k : to_ra->v_.regs_[n] + k,
                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[n] + k : from_ra->v_.regs_[n] + k);
              } /* end for n components */
            } /* end for k array_quantity */
            break;
          } /* end case 0x33: bool to bool */
        } /* end scalar to scalar conversion switch */
      }
    }
  }
}
      break;
    }
    case slrak_sampler2D: {
      if (from_ra->kind_ != to_ra->kind_) return;
      if (to_ra->is_indirect_) {
        /* to is indirect */
        if (from_ra->is_indirect_) {
          /* from is indirect */
          if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
            /* have a to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                             to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                             k,
                                                             to_offset_step_size,
                                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                                             from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                             k,
                                                             from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_indir_to_indir_offset(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                      to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                      k,
                                                      to_offset_step_size,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                                      k);
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_indir_offset_to_indir(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                      k,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                                      from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                      k,
                                                      from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_indir_to_indir(exec,
                                               row,
                                               to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                               k,
                                               from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                               k);
              }
            }
          }
        }
        else {
          /* from is direct */
          if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
            /* have a to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_sampler2D_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k,
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           0,
                                                           from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_reg_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_sampler2D_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k);
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_offset_reg_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_sampler2D_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k,
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    0,
                                                    from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_reg_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_sampler2D_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k);
              }
            }
          }
        }
      }
      else {
        /* to is direct */
        if (from_ra->is_indirect_) {
          /* from is indirect */
          if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
            /* have a to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_sampler2D_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           0,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_indir_to_offset_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_sampler2D_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    0,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                                    k);
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_indir_offset_to_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_sampler2D_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_indir_to_reg(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_sampler2D_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                             k);
              }
            }
          }
        }
        else {
          /* from is direct */
          if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
            /* have a to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_sampler2D_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_sampler2D_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_reg_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_sampler2D_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_sampler2D_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k);
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_offset_reg_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_sampler2D_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_sampler2D_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_reg_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_sampler2D_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_sampler2D_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k);
              }
            }
          }
        }
      }
      break;
    }
    case slrak_samplerCube: {
      if (from_ra->kind_ != to_ra->kind_) return;
      if (to_ra->is_indirect_) {
        /* to is indirect */
        if (from_ra->is_indirect_) {
          /* from is indirect */
          if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
            /* have a to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_indir_offset_to_indir_offset(exec,
                                                            row,
                                                            to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                            to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                            k,
                                                            to_offset_step_size,
                                                            from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                                            from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                            k,
                                                            from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_indir_to_indir_offset(exec,
                                                     row,
                                                     to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                     to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                     k,
                                                     to_offset_step_size,
                                                     from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                                     k);
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_indir_offset_to_indir(exec,
                                                     row,
                                                     to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                     k,
                                                     from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                                     from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                     k,
                                                     from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_indir_to_indir(exec,
                                              row,
                                              to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                              k,
                                              from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                              k);
              }
            }
          }
        }
        else {
          /* from is direct */
          if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
            /* have a to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_offset_reg_to_indir_offset(exec,
                                                          row,
                                                          to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                          to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                          k,
                                                          to_offset_step_size,
                                                          from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_samplerCube_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k,
                                                          from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                          0,
                                                          from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_reg_to_indir_offset(exec,
                                                   row,
                                                   to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                   to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                   k,
                                                   to_offset_step_size,
                                                   from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_samplerCube_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k);
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_offset_reg_to_indir(exec,
                                                   row,
                                                   to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                                   k,
                                                   from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_samplerCube_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k,
                                                   from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                   0,
                                                   from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_reg_to_indir(exec,
                                            row,
                                            to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[0] : to_ra->v_.regs_[0],
                                            k,
                                            from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_samplerCube_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k);
              }
            }
          }
        }
      }
      else {
        /* to is direct */
        if (from_ra->is_indirect_) {
          /* from is indirect */
          if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
            /* have a to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_indir_offset_to_offset_reg(exec,
                                                          row,
                                                          to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_samplerCube_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                                          to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                          0,
                                                          to_offset_step_size,
                                                          from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                                          from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                          k,
                                                          from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_indir_to_offset_reg(exec,
                                                   row,
                                                   to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_samplerCube_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                                   to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                   0,
                                                   to_offset_step_size,
                                                   from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                                   k);
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_indir_offset_to_reg(exec,
                                                   row,
                                                   to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_samplerCube_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                                   from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                                   from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                   k,
                                                   from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_indir_to_reg(exec,
                                            row,
                                            to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_samplerCube_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                            from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[0] : from_ra->v_.regs_[0],
                                            k);
              }
            }
          }
        }
        else {
          /* from is direct */
          if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
            /* have a to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_offset_reg_to_offset_reg(exec,
                                                        row,
                                                        to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_samplerCube_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                                        to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                        0,
                                                        to_offset_step_size,
                                                        from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_samplerCube_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k,
                                                        from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                        0,
                                                        from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_reg_to_offset_reg(exec,
                                                 row,
                                                 to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_samplerCube_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                                 to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                 0,
                                                 to_offset_step_size,
                                                 from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_samplerCube_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k);
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_offset_reg_to_reg(exec,
                                                 row,
                                                 to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_samplerCube_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                                 from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_samplerCube_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k,
                                                 from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                 0,
                                                 from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_reg_to_reg(exec,
                                          row,
                                          to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_samplerCube_offset_ + to_ra->v_.regs_[0] + k : to_ra->v_.regs_[0] + k,
                                          from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_samplerCube_offset_ + from_ra->v_.regs_[0] + k : from_ra->v_.regs_[0] + k);
              }
            }
          }
        }
      }
      break;
    }
  }
}
void sl_reg_move_crossframe_c2c(struct sl_execution *exec,
                              uint8_t row,
                              int from_frame, struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset, int from_component,
                              int to_frame, struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset, int to_component,
                              int from_offset_step_size, int to_offset_step_size,
                              int array_quantity) {
  size_t num_components = 0;
  int scalar_to_scalar_conversion = 0;
  int k;
  switch (from_ra->kind_) {
    case slrak_array:
    case slrak_struct: {
      return;
    }
    case slrak_float: if (from_component >= 1) return; scalar_to_scalar_conversion = 0x10; break;
    case slrak_vec2:  if (from_component >= 2) return; scalar_to_scalar_conversion = 0x10; break;
    case slrak_vec3:  if (from_component >= 3) return; scalar_to_scalar_conversion = 0x10; break;
    case slrak_vec4:  if (from_component >= 4) return; scalar_to_scalar_conversion = 0x10; break;
    case slrak_mat2:  if (from_component >= 4) return; scalar_to_scalar_conversion = 0x10; break;
    case slrak_mat3:  if (from_component >= 9) return; scalar_to_scalar_conversion = 0x10; break;
    case slrak_mat4:  if (from_component >= 16) return; scalar_to_scalar_conversion = 0x10; break;
    case slrak_int:   if (from_component >= 1) return; scalar_to_scalar_conversion = 0x20; break;
    case slrak_ivec2: if (from_component >= 2) return; scalar_to_scalar_conversion = 0x20; break;
    case slrak_ivec3: if (from_component >= 3) return; scalar_to_scalar_conversion = 0x20; break;
    case slrak_ivec4: if (from_component >= 4) return; scalar_to_scalar_conversion = 0x20; break;
    case slrak_bool:  if (from_component >= 1) return; scalar_to_scalar_conversion = 0x30; break;
    case slrak_bvec2: if (from_component >= 2) return; scalar_to_scalar_conversion = 0x30; break;
    case slrak_bvec3: if (from_component >= 3) return; scalar_to_scalar_conversion = 0x30; break;
    case slrak_bvec4: if (from_component >= 4) return; scalar_to_scalar_conversion = 0x30; break;
  }
  switch (to_ra->kind_) {
    case slrak_float: if (to_component >= 1) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_vec2:  if (to_component >= 2) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_vec3:  if (to_component >= 3) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_vec4:  if (to_component >= 4) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_mat2:  if (to_component >= 4) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_mat3:  if (to_component >= 9) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_mat4:  if (to_component >=16) return; scalar_to_scalar_conversion += 0x1; break;
    case slrak_int:   if (to_component >= 1) return; scalar_to_scalar_conversion += 0x2; break;
    case slrak_ivec2: if (to_component >= 2) return; scalar_to_scalar_conversion += 0x2; break;
    case slrak_ivec3: if (to_component >= 3) return; scalar_to_scalar_conversion += 0x2; break;
    case slrak_ivec4: if (to_component >= 4) return; scalar_to_scalar_conversion += 0x2; break;
    case slrak_bool:  if (to_component >= 1) return; scalar_to_scalar_conversion += 0x3; break;
    case slrak_bvec2: if (to_component >= 2) return; scalar_to_scalar_conversion += 0x3; break;
    case slrak_bvec3: if (to_component >= 3) return; scalar_to_scalar_conversion += 0x3; break;
    case slrak_bvec4: if (to_component >= 4) return; scalar_to_scalar_conversion += 0x3; break;
    default: return;
  }
  if (to_ra->is_indirect_) {
    /* to is indirect */
    if (from_ra->is_indirect_) {
      /* from is indirect */
      if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
        /* have a to offset */
        if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
          /* have a from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_indir_offset_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                             to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                             k,
                                                             to_offset_step_size,
                                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                             from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                             k,
                                                             from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                             to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                             k,
                                                             to_offset_step_size,
                                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                             from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                             k,
                                                             from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                             to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                             k,
                                                             to_offset_step_size,
                                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                             from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                             k,
                                                             from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_indir_offset_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                             to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                             k,
                                                             to_offset_step_size,
                                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                             from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                             k,
                                                             from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                             to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                             k,
                                                             to_offset_step_size,
                                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                             from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                             k,
                                                             from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                             to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                             k,
                                                             to_offset_step_size,
                                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                             from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                             k,
                                                             from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_indir_offset_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
        else {
          /* no from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_indir_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_indir_to_indir_offset(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                      to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                      k,
                                                      to_offset_step_size,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                      k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_indir_to_indir_offset(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                      to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                      k,
                                                      to_offset_step_size,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                      k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_indir_to_indir_offset(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                      to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                      k,
                                                      to_offset_step_size,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                      k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_indir_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_indir_to_indir_offset(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                      to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                      k,
                                                      to_offset_step_size,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                      k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_indir_to_indir_offset(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                      to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                      k,
                                                      to_offset_step_size,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                      k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_indir_to_indir_offset(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                      to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                      k,
                                                      to_offset_step_size,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                      k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_indir_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
      }
      else {
        /* no to offset */
        if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
          /* have a from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_indir_offset_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_indir_offset_to_indir(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                      k,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                      from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                      k,
                                                      from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_indir_offset_to_indir(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                      k,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                      from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                      k,
                                                      from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_indir_offset_to_indir(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                      k,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                      from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                      k,
                                                      from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_indir_offset_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_indir_offset_to_indir(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                      k,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                      from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                      k,
                                                      from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_indir_offset_to_indir(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                      k,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                      from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                      k,
                                                      from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_indir_offset_to_indir(exec,
                                                      row,
                                                      to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                      k,
                                                      from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                      from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                      k,
                                                      from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_indir_offset_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
        else {
          /* no from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_indir_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                             k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_indir_to_indir(exec,
                                               row,
                                               to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                               k,
                                               from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                               k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_indir_to_indir(exec,
                                               row,
                                               to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                               k,
                                               from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                               k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_indir_to_indir(exec,
                                               row,
                                               to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                               k,
                                               from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                               k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_indir_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                             k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_indir_to_indir(exec,
                                               row,
                                               to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                               k,
                                               from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                               k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_indir_to_indir(exec,
                                               row,
                                               to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                               k,
                                               from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                               k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_indir_to_indir(exec,
                                               row,
                                               to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                               k,
                                               from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                               k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_indir_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                             k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
      }
    }
    else {
      /* from is direct */
      if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
        /* have a to offset */
        if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
          /* have a from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_offset_reg_to_indir_offset(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         k,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           0,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           0,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           0,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_offset_reg_to_indir_offset(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         k,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           0,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           0,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           k,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           0,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_offset_reg_to_indir_offset(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         k,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
        else {
          /* no from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_reg_to_indir_offset(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  k,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_reg_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_reg_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_reg_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_reg_to_indir_offset(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  k,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_reg_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_reg_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_reg_to_indir_offset(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    k,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_reg_to_indir_offset(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  k,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
      }
      else {
        /* no to offset */
        if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
          /* have a from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_offset_reg_to_indir(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                  k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_offset_reg_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    0,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_offset_reg_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    0,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_offset_reg_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    0,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_offset_reg_to_indir(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                  k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_offset_reg_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    0,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_offset_reg_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    0,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_offset_reg_to_indir(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                    k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    0,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_offset_reg_to_indir(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                                  k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
        else {
          /* no from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_reg_to_indir(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                           k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_reg_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_reg_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_reg_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_reg_to_indir(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                           k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_reg_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_reg_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_reg_to_indir(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                             k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_reg_to_indir(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] : to_ra->v_.regs_[to_component],
                                           k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
      }
    }
  }
  else {
    /* to is direct */
    if (from_ra->is_indirect_) {
      /* from is indirect */
      if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
        /* have a to offset */
        if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
          /* have a from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_indir_offset_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         k,
                                                         from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           0,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           0,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           0,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_indir_offset_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         k,
                                                         from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           0,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           0,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                           to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                           0,
                                                           to_offset_step_size,
                                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                           from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                           k,
                                                           from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_indir_offset_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         k,
                                                         from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
        else {
          /* no from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_indir_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                  k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_indir_to_offset_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    0,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_indir_to_offset_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    0,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_indir_to_offset_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    0,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_indir_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                  k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_indir_to_offset_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    0,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_indir_to_offset_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    0,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_indir_to_offset_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                    to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                    0,
                                                    to_offset_step_size,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_indir_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                  k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
      }
      else {
        /* no to offset */
        if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
          /* have a from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_indir_offset_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  k,
                                                  from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_indir_offset_to_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_indir_offset_to_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_indir_offset_to_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_indir_offset_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  k,
                                                  from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_indir_offset_to_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_indir_offset_to_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_indir_offset_to_reg(exec,
                                                    row,
                                                    to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                    from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                    from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                    k,
                                                    from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_indir_offset_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  k,
                                                  from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
        else {
          /* no from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_indir_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                           k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_indir_to_reg(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                             k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_indir_to_reg(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                             k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_indir_to_reg(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                             k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_indir_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                           k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_indir_to_reg(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                             k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_indir_to_reg(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                             k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_indir_to_reg(exec,
                                             row,
                                             to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                             from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                             k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_indir_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] : from_ra->v_.regs_[from_component],
                                           k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
      }
    }
    else {
      /* from is direct */
      if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
        /* have a to offset */
        if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
          /* have a from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_offset_reg_to_offset_reg(exec,
                                                       row,
                                                       to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                       to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                       0,
                                                       to_offset_step_size,
                                                       from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                       from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                       0,
                                                       from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_offset_reg_to_offset_reg(exec,
                                                       row,
                                                       to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                       to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                       0,
                                                       to_offset_step_size,
                                                       from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                       from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                       0,
                                                       from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                         to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                         0,
                                                         to_offset_step_size,
                                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                         from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                         0,
                                                         from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_offset_reg_to_offset_reg(exec,
                                                       row,
                                                       to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                       to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                       0,
                                                       to_offset_step_size,
                                                       from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                       from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                       0,
                                                       from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
        else {
          /* no from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_reg_to_offset_reg(exec,
                                                row,
                                                to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                0,
                                                to_offset_step_size,
                                                from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_reg_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_reg_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_reg_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_reg_to_offset_reg(exec,
                                                row,
                                                to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                0,
                                                to_offset_step_size,
                                                from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_reg_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_reg_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_reg_to_offset_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                  0,
                                                  to_offset_step_size,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_reg_to_offset_reg(exec,
                                                row,
                                                to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                to_ra_offset->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra_offset->v_.regs_[0] : to_ra_offset->v_.regs_[0],
                                                0,
                                                to_offset_step_size,
                                                from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
      }
      else {
        /* no to offset */
        if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
          /* have a from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_offset_reg_to_reg(exec,
                                                row,
                                                to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                0,
                                                from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_offset_reg_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_offset_reg_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_offset_reg_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_offset_reg_to_reg(exec,
                                                row,
                                                to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                0,
                                                from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_offset_reg_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_offset_reg_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_offset_reg_to_reg(exec,
                                                  row,
                                                  to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                  from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                  from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                  0,
                                                  from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_offset_reg_to_reg(exec,
                                                row,
                                                to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                                from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k,
                                                from_ra_offset->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra_offset->v_.regs_[0] : from_ra_offset->v_.regs_[0],
                                                0,
                                                from_offset_step_size);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
        else {
          /* no from offset */
          switch (scalar_to_scalar_conversion) {
            case 0x11: /* float to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f_reg_to_reg(exec,
                                         row,
                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x11: float to float */
            case 0x12: /* float to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2i_reg_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x12: float to int */
            case 0x13: /* float to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_f2b_reg_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_float_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x13: float to bool */
            case 0x21: /* int to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2f_reg_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x21: int to float */
            case 0x22: /* int to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i_reg_to_reg(exec,
                                         row,
                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x22: int to int */
            case 0x23: /* int to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_i2b_reg_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_int_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x23: int to bool */
            case 0x31: /* bool to float */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2f_reg_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_float_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x31: bool to float */
            case 0x32: /* bool to int */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b2i_reg_to_reg(exec,
                                           row,
                                           to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_int_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                           from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x32: bool to int */
            case 0x33: /* bool to bool */ {
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_b_reg_to_reg(exec,
                                         row,
                                         to_ra->local_frame_ ? exec->execution_frames_[to_frame].local_bool_offset_ + to_ra->v_.regs_[to_component] + k : to_ra->v_.regs_[to_component] + k,
                                         from_ra->local_frame_ ? exec->execution_frames_[from_frame].local_bool_offset_ + from_ra->v_.regs_[from_component] + k : from_ra->v_.regs_[from_component] + k);
              } /* end for k array_quantity */
              break;
            } /* end case 0x33: bool to bool */
          } /* end scalar to scalar conversion switch */
        }
      }
    }
  }
}
void sl_reg_move(struct sl_execution *exec,
                 uint8_t row,
                 struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset,
                 struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset) {
  sl_reg_move_crossframe(exec, row, (int)(exec->num_execution_frames_ - 1), from_ra, from_ra_offset, (int)(exec->num_execution_frames_ - 1), to_ra, to_ra_offset, 1, 1, 1);
}

void sl_reg_move_c2c(struct sl_execution *exec,
                     uint8_t row,
                     struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset, int from_component,
                     struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset, int to_component) {
  sl_reg_move_crossframe_c2c(exec, row, (int)(exec->num_execution_frames_ - 1), from_ra, from_ra_offset, from_component, (int)(exec->num_execution_frames_ - 1), to_ra, to_ra_offset, to_component, 1, 1, 1);
}
