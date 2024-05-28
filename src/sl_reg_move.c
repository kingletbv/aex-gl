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
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict reg_bank = exec->float_regs_;
  float * restrict dst = reg_bank[dst_reg];
  const float * restrict src = reg_bank[src_reg];
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
  float * restrict * restrict reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const float * restrict src = reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
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
  float * restrict * restrict reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const float * restrict src = reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
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
  float * restrict * restrict reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const float * restrict src = reg_bank[src_reg];
  for (;;) {
    float * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
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
  float * restrict * restrict reg_bank = exec->float_regs_;
  float * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const float * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    const float * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
  float * restrict * restrict reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const float * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
  float * restrict * restrict reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    float * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const float * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
  float * restrict * restrict reg_bank = exec->float_regs_;
  float * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const float * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  float * restrict * restrict reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    const float * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const float * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  float * restrict * restrict reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const float * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  float * restrict * restrict reg_bank = exec->float_regs_;
  float * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = dst + row;
    const float * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  float * restrict * restrict reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    const float * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  float * restrict * restrict reg_bank = exec->float_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const float * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  float * restrict * restrict reg_bank = exec->float_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    float * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const float * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_i_reg_to_reg(struct sl_execution *exec,
                                     uint8_t row,
                                     int dst_reg,
                                     int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  int64_t * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src = reg_bank[src_reg];
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
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src = reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
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
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src = reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
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
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src = reg_bank[src_reg];
  for (;;) {
    int64_t * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
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
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  int64_t * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const int64_t * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    const int64_t * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const int64_t * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    int64_t * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const int64_t * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  int64_t * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const int64_t * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    const int64_t * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const int64_t * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const int64_t * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  int64_t * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const int64_t * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    const int64_t * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const int64_t * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  int64_t * restrict * restrict reg_bank = exec->int_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    int64_t * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const int64_t * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_reg_move_b_reg_to_reg(struct sl_execution *exec,
                                     uint8_t row,
                                     int dst_reg,
                                     int src_reg) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = reg_bank[dst_reg];
  const uint8_t * restrict src = reg_bank[src_reg];
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
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const uint8_t * restrict src = reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
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
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const uint8_t * restrict src = reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
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
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const uint8_t * restrict src = reg_bank[src_reg];
  for (;;) {
    uint8_t * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
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
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    const uint8_t * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const uint8_t * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  for (;;) {
    uint8_t * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const uint8_t * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    const uint8_t * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const uint8_t * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const uint8_t * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  uint8_t * restrict dst = reg_bank[dst_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    const uint8_t * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const uint8_t * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
  uint8_t * restrict chain_column = exec->exec_chain_reg_;
  uint8_t * restrict * restrict reg_bank = exec->bool_regs_;
  const int64_t * restrict dst_indir = exec->int_regs_[dst_indir_reg];
  const int64_t * restrict dst_offset = exec->int_regs_[dst_offset_reg];
  const int64_t * restrict src_indir = exec->int_regs_[src_indir_reg];
  const int64_t * restrict src_offset = exec->int_regs_[src_offset_reg];
  for (;;) {
    uint8_t * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    const uint8_t * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
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
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
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
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
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
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
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
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_fixed_offset];

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
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_base_reg + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_base_reg + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

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
    void * restrict * restrict dst_row = reg_bank[dst_indir[row] + dst_offset_stepsize * dst_offset[row] + dst_fixed_offset];
    void * const restrict * restrict src_row = reg_bank[src_indir[row] + src_offset_stepsize * src_offset[row] + src_fixed_offset];

    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

void sl_reg_move(struct sl_execution *exec,
                 uint8_t row,
                 struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset,
                 struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset,
                 int from_offset_step_size, int to_offset_step_size,
                 int array_quantity) {
  size_t num_components = 0;
  size_t n;
  int k;
  if (from_ra->kind_ != to_ra->kind_) return;
  switch (from_ra->kind_) {
    case slrak_array: {
      if (from_ra->v_.array_.num_elements_ != to_ra->v_.array_.num_elements_) return;
      int new_from_offset_step_size = (int)(from_offset_step_size * from_ra->v_.array_.num_elements_);
      int new_to_offset_step_size = (int)(to_offset_step_size * from_ra->v_.array_.num_elements_);
      int new_array_quantity = (int)(array_quantity * from_ra->v_.array_.num_elements_);
      sl_reg_move(exec, row, from_ra->v_.array_.head_, from_ra_offset, to_ra->v_.array_.head_, to_ra_offset, new_from_offset_step_size, new_to_offset_step_size, new_array_quantity);
      break;
    }
    case slrak_struct: {
      if (from_ra->v_.comp_.num_fields_ != to_ra->v_.comp_.num_fields_) return;
      for (n = 0; n < from_ra->v_.comp_.num_fields_; ++n) {
        sl_reg_move(exec, row, from_ra->v_.comp_.fields_ + n, from_ra_offset, to_ra->v_.comp_.fields_ + n, to_ra_offset, from_offset_step_size, to_offset_step_size, array_quantity);
      }
      break;
    }
    case slrak_float:
    case slrak_vec2:
    case slrak_vec3:
    case slrak_vec4:
    case slrak_mat2:
    case slrak_mat3:
    case slrak_mat4: {
      switch (from_ra->kind_) {
        case slrak_float: num_components = 1;break;
        case slrak_vec2:  num_components = 2;break;
        case slrak_vec3:  num_components = 3;break;
        case slrak_vec4:  num_components = 4;break;
        case slrak_mat2:  num_components = 4;break;
        case slrak_mat3:  num_components = 9;break;
        case slrak_mat4:  num_components = 16;break;
      }
      if (to_ra->is_indirect_) {
        /* to is indirect */
        if (from_ra->is_indirect_) {
          /* from is indirect */
          if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
            /* have a to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             INT_REG_INDEX_NRV(to_ra, n),
                                                             INT_REG_INDEX_NRV(to_ra_offset, n),
                                                             k,
                                                             to_offset_step_size,
                                                             INT_REG_INDEX_NRV(from_ra, n),
                                                             INT_REG_INDEX_NRV(from_ra_offset, n),
                                                             k,
                                                             from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_indir_to_indir_offset(exec,
                                                      row,
                                                      INT_REG_INDEX_NRV(to_ra, n),
                                                      INT_REG_INDEX_NRV(to_ra_offset, n),
                                                      k,
                                                      to_offset_step_size,
                                                      INT_REG_INDEX_NRV(from_ra, n),
                                                      k);
                }
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_indir_offset_to_indir(exec,
                                                      row,
                                                      INT_REG_INDEX_NRV(to_ra, n),
                                                      k,
                                                      INT_REG_INDEX_NRV(from_ra, n),
                                                      INT_REG_INDEX_NRV(from_ra_offset, n),
                                                      k,
                                                      from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_indir_to_indir(exec,
                                               row,
                                               INT_REG_INDEX_NRV(to_ra, n),
                                               k,
                                               INT_REG_INDEX_NRV(from_ra, n),
                                               k);
                }
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
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           INT_REG_INDEX_NRV(to_ra, n),
                                                           INT_REG_INDEX_NRV(to_ra_offset, n),
                                                           k,
                                                           to_offset_step_size,
                                                           FLOAT_REG_INDEX_NRV(from_ra, n) + k,
                                                           INT_REG_INDEX_NRV(from_ra_offset, n),
                                                           0,
                                                           from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_reg_to_indir_offset(exec,
                                                    row,
                                                    INT_REG_INDEX_NRV(to_ra, n),
                                                    INT_REG_INDEX_NRV(to_ra_offset, n),
                                                    k,
                                                    to_offset_step_size,
                                                    FLOAT_REG_INDEX_NRV(from_ra, n) + k);
                }
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_offset_reg_to_indir(exec,
                                                    row,
                                                    INT_REG_INDEX_NRV(to_ra, n),
                                                    k,
                                                    FLOAT_REG_INDEX_NRV(from_ra, n) + k,
                                                    INT_REG_INDEX_NRV(from_ra_offset, n),
                                                    0,
                                                    from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_reg_to_indir(exec,
                                             row,
                                             INT_REG_INDEX_NRV(to_ra, n),
                                             k,
                                             FLOAT_REG_INDEX_NRV(from_ra, n) + k);
                }
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
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           FLOAT_REG_INDEX_NRV(to_ra, n) + k,
                                                           INT_REG_INDEX_NRV(to_ra_offset, n),
                                                           0,
                                                           to_offset_step_size,
                                                           INT_REG_INDEX_NRV(from_ra, n),
                                                           INT_REG_INDEX_NRV(from_ra_offset, n),
                                                           k,
                                                           from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_indir_to_offset_reg(exec,
                                                    row,
                                                    FLOAT_REG_INDEX_NRV(to_ra, n) + k,
                                                    INT_REG_INDEX_NRV(to_ra_offset, n),
                                                    0,
                                                    to_offset_step_size,
                                                    INT_REG_INDEX_NRV(from_ra, n),
                                                    k);
                }
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_indir_offset_to_reg(exec,
                                                    row,
                                                    FLOAT_REG_INDEX_NRV(to_ra, n) + k,
                                                    INT_REG_INDEX_NRV(from_ra, n),
                                                    INT_REG_INDEX_NRV(from_ra_offset, n),
                                                    k,
                                                    from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_indir_to_reg(exec,
                                             row,
                                             FLOAT_REG_INDEX_NRV(to_ra, n) + k,
                                             INT_REG_INDEX_NRV(from_ra, n),
                                             k);
                }
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
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         FLOAT_REG_INDEX_NRV(to_ra, n) + k,
                                                         INT_REG_INDEX_NRV(to_ra_offset, n),
                                                         0,
                                                         to_offset_step_size,
                                                         FLOAT_REG_INDEX_NRV(from_ra, n) + k,
                                                         INT_REG_INDEX_NRV(from_ra_offset, n),
                                                         0,
                                                         from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_reg_to_offset_reg(exec,
                                                  row,
                                                  FLOAT_REG_INDEX_NRV(to_ra, n) + k,
                                                  INT_REG_INDEX_NRV(to_ra_offset, n),
                                                  0,
                                                  to_offset_step_size,
                                                  FLOAT_REG_INDEX_NRV(from_ra, n) + k);
                }
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_offset_reg_to_reg(exec,
                                                  row,
                                                  FLOAT_REG_INDEX_NRV(to_ra, n) + k,
                                                  FLOAT_REG_INDEX_NRV(from_ra, n) + k,
                                                  INT_REG_INDEX_NRV(from_ra_offset, n),
                                                  0,
                                                  from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_f_reg_to_reg(exec,
                                           row,
                                           FLOAT_REG_INDEX_NRV(to_ra, n) + k,
                                           FLOAT_REG_INDEX_NRV(from_ra, n) + k);
                }
              }
            }
          }
        }
      }
      break;
    }
    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      switch (from_ra->kind_) {
        case slrak_int: num_components = 1;break;
        case slrak_ivec2:  num_components = 2;break;
        case slrak_ivec3:  num_components = 3;break;
        case slrak_ivec4:  num_components = 4;break;
      }
      if (to_ra->is_indirect_) {
        /* to is indirect */
        if (from_ra->is_indirect_) {
          /* from is indirect */
          if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
            /* have a to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             INT_REG_INDEX_NRV(to_ra, n),
                                                             INT_REG_INDEX_NRV(to_ra_offset, n),
                                                             k,
                                                             to_offset_step_size,
                                                             INT_REG_INDEX_NRV(from_ra, n),
                                                             INT_REG_INDEX_NRV(from_ra_offset, n),
                                                             k,
                                                             from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_indir_to_indir_offset(exec,
                                                      row,
                                                      INT_REG_INDEX_NRV(to_ra, n),
                                                      INT_REG_INDEX_NRV(to_ra_offset, n),
                                                      k,
                                                      to_offset_step_size,
                                                      INT_REG_INDEX_NRV(from_ra, n),
                                                      k);
                }
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_indir_offset_to_indir(exec,
                                                      row,
                                                      INT_REG_INDEX_NRV(to_ra, n),
                                                      k,
                                                      INT_REG_INDEX_NRV(from_ra, n),
                                                      INT_REG_INDEX_NRV(from_ra_offset, n),
                                                      k,
                                                      from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_indir_to_indir(exec,
                                               row,
                                               INT_REG_INDEX_NRV(to_ra, n),
                                               k,
                                               INT_REG_INDEX_NRV(from_ra, n),
                                               k);
                }
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
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           INT_REG_INDEX_NRV(to_ra, n),
                                                           INT_REG_INDEX_NRV(to_ra_offset, n),
                                                           k,
                                                           to_offset_step_size,
                                                           INT_REG_INDEX_NRV(from_ra, n) + k,
                                                           INT_REG_INDEX_NRV(from_ra_offset, n),
                                                           0,
                                                           from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_reg_to_indir_offset(exec,
                                                    row,
                                                    INT_REG_INDEX_NRV(to_ra, n),
                                                    INT_REG_INDEX_NRV(to_ra_offset, n),
                                                    k,
                                                    to_offset_step_size,
                                                    INT_REG_INDEX_NRV(from_ra, n) + k);
                }
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_offset_reg_to_indir(exec,
                                                    row,
                                                    INT_REG_INDEX_NRV(to_ra, n),
                                                    k,
                                                    INT_REG_INDEX_NRV(from_ra, n) + k,
                                                    INT_REG_INDEX_NRV(from_ra_offset, n),
                                                    0,
                                                    from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_reg_to_indir(exec,
                                             row,
                                             INT_REG_INDEX_NRV(to_ra, n),
                                             k,
                                             INT_REG_INDEX_NRV(from_ra, n) + k);
                }
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
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           INT_REG_INDEX_NRV(to_ra, n) + k,
                                                           INT_REG_INDEX_NRV(to_ra_offset, n),
                                                           0,
                                                           to_offset_step_size,
                                                           INT_REG_INDEX_NRV(from_ra, n),
                                                           INT_REG_INDEX_NRV(from_ra_offset, n),
                                                           k,
                                                           from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_indir_to_offset_reg(exec,
                                                    row,
                                                    INT_REG_INDEX_NRV(to_ra, n) + k,
                                                    INT_REG_INDEX_NRV(to_ra_offset, n),
                                                    0,
                                                    to_offset_step_size,
                                                    INT_REG_INDEX_NRV(from_ra, n),
                                                    k);
                }
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_indir_offset_to_reg(exec,
                                                    row,
                                                    INT_REG_INDEX_NRV(to_ra, n) + k,
                                                    INT_REG_INDEX_NRV(from_ra, n),
                                                    INT_REG_INDEX_NRV(from_ra_offset, n),
                                                    k,
                                                    from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_indir_to_reg(exec,
                                             row,
                                             INT_REG_INDEX_NRV(to_ra, n) + k,
                                             INT_REG_INDEX_NRV(from_ra, n),
                                             k);
                }
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
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         INT_REG_INDEX_NRV(to_ra, n) + k,
                                                         INT_REG_INDEX_NRV(to_ra_offset, n),
                                                         0,
                                                         to_offset_step_size,
                                                         INT_REG_INDEX_NRV(from_ra, n) + k,
                                                         INT_REG_INDEX_NRV(from_ra_offset, n),
                                                         0,
                                                         from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_reg_to_offset_reg(exec,
                                                  row,
                                                  INT_REG_INDEX_NRV(to_ra, n) + k,
                                                  INT_REG_INDEX_NRV(to_ra_offset, n),
                                                  0,
                                                  to_offset_step_size,
                                                  INT_REG_INDEX_NRV(from_ra, n) + k);
                }
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_offset_reg_to_reg(exec,
                                                  row,
                                                  INT_REG_INDEX_NRV(to_ra, n) + k,
                                                  INT_REG_INDEX_NRV(from_ra, n) + k,
                                                  INT_REG_INDEX_NRV(from_ra_offset, n),
                                                  0,
                                                  from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_i_reg_to_reg(exec,
                                           row,
                                           INT_REG_INDEX_NRV(to_ra, n) + k,
                                           INT_REG_INDEX_NRV(from_ra, n) + k);
                }
              }
            }
          }
        }
      }
      break;
    }
    case slrak_bool:
    case slrak_bvec2:
    case slrak_bvec3:
    case slrak_bvec4: {
      switch (from_ra->kind_) {
        case slrak_int: num_components = 1;break;
        case slrak_ivec2:  num_components = 2;break;
        case slrak_ivec3:  num_components = 3;break;
        case slrak_ivec4:  num_components = 4;break;
      }
      if (to_ra->is_indirect_) {
        /* to is indirect */
        if (from_ra->is_indirect_) {
          /* from is indirect */
          if (to_ra_offset && (to_ra_offset->kind_ != slrak_void)) {
            /* have a to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_indir_offset_to_indir_offset(exec,
                                                             row,
                                                             INT_REG_INDEX_NRV(to_ra, n),
                                                             INT_REG_INDEX_NRV(to_ra_offset, n),
                                                             k,
                                                             to_offset_step_size,
                                                             INT_REG_INDEX_NRV(from_ra, n),
                                                             INT_REG_INDEX_NRV(from_ra_offset, n),
                                                             k,
                                                             from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_indir_to_indir_offset(exec,
                                                      row,
                                                      INT_REG_INDEX_NRV(to_ra, n),
                                                      INT_REG_INDEX_NRV(to_ra_offset, n),
                                                      k,
                                                      to_offset_step_size,
                                                      INT_REG_INDEX_NRV(from_ra, n),
                                                      k);
                }
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_indir_offset_to_indir(exec,
                                                      row,
                                                      INT_REG_INDEX_NRV(to_ra, n),
                                                      k,
                                                      INT_REG_INDEX_NRV(from_ra, n),
                                                      INT_REG_INDEX_NRV(from_ra_offset, n),
                                                      k,
                                                      from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_indir_to_indir(exec,
                                               row,
                                               INT_REG_INDEX_NRV(to_ra, n),
                                               k,
                                               INT_REG_INDEX_NRV(from_ra, n),
                                               k);
                }
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
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_offset_reg_to_indir_offset(exec,
                                                           row,
                                                           INT_REG_INDEX_NRV(to_ra, n),
                                                           INT_REG_INDEX_NRV(to_ra_offset, n),
                                                           k,
                                                           to_offset_step_size,
                                                           BOOL_REG_INDEX_NRV(from_ra, n) + k,
                                                           INT_REG_INDEX_NRV(from_ra_offset, n),
                                                           0,
                                                           from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_reg_to_indir_offset(exec,
                                                    row,
                                                    INT_REG_INDEX_NRV(to_ra, n),
                                                    INT_REG_INDEX_NRV(to_ra_offset, n),
                                                    k,
                                                    to_offset_step_size,
                                                    BOOL_REG_INDEX_NRV(from_ra, n) + k);
                }
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_offset_reg_to_indir(exec,
                                                    row,
                                                    INT_REG_INDEX_NRV(to_ra, n),
                                                    k,
                                                    BOOL_REG_INDEX_NRV(from_ra, n) + k,
                                                    INT_REG_INDEX_NRV(from_ra_offset, n),
                                                    0,
                                                    from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_reg_to_indir(exec,
                                             row,
                                             INT_REG_INDEX_NRV(to_ra, n),
                                             k,
                                             BOOL_REG_INDEX_NRV(from_ra, n) + k);
                }
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
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_indir_offset_to_offset_reg(exec,
                                                           row,
                                                           BOOL_REG_INDEX_NRV(to_ra, n) + k,
                                                           INT_REG_INDEX_NRV(to_ra_offset, n),
                                                           0,
                                                           to_offset_step_size,
                                                           INT_REG_INDEX_NRV(from_ra, n),
                                                           INT_REG_INDEX_NRV(from_ra_offset, n),
                                                           k,
                                                           from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_indir_to_offset_reg(exec,
                                                    row,
                                                    BOOL_REG_INDEX_NRV(to_ra, n) + k,
                                                    INT_REG_INDEX_NRV(to_ra_offset, n),
                                                    0,
                                                    to_offset_step_size,
                                                    INT_REG_INDEX_NRV(from_ra, n),
                                                    k);
                }
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_indir_offset_to_reg(exec,
                                                    row,
                                                    BOOL_REG_INDEX_NRV(to_ra, n) + k,
                                                    INT_REG_INDEX_NRV(from_ra, n),
                                                    INT_REG_INDEX_NRV(from_ra_offset, n),
                                                    k,
                                                    from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_indir_to_reg(exec,
                                             row,
                                             BOOL_REG_INDEX_NRV(to_ra, n) + k,
                                             INT_REG_INDEX_NRV(from_ra, n),
                                             k);
                }
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
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_offset_reg_to_offset_reg(exec,
                                                         row,
                                                         BOOL_REG_INDEX_NRV(to_ra, n) + k,
                                                         INT_REG_INDEX_NRV(to_ra_offset, n),
                                                         0,
                                                         to_offset_step_size,
                                                         BOOL_REG_INDEX_NRV(from_ra, n) + k,
                                                         INT_REG_INDEX_NRV(from_ra_offset, n),
                                                         0,
                                                         from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_reg_to_offset_reg(exec,
                                                  row,
                                                  BOOL_REG_INDEX_NRV(to_ra, n) + k,
                                                  INT_REG_INDEX_NRV(to_ra_offset, n),
                                                  0,
                                                  to_offset_step_size,
                                                  BOOL_REG_INDEX_NRV(from_ra, n) + k);
                }
              }
            }
          }
          else {
            /* no to offset */
            if (from_ra_offset && (from_ra_offset->kind_ != slrak_void)) {
              /* have a from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_offset_reg_to_reg(exec,
                                                  row,
                                                  BOOL_REG_INDEX_NRV(to_ra, n) + k,
                                                  BOOL_REG_INDEX_NRV(from_ra, n) + k,
                                                  INT_REG_INDEX_NRV(from_ra_offset, n),
                                                  0,
                                                  from_offset_step_size);
                }
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                for (n = 0; n < num_components; ++n) {
                  sl_reg_move_b_reg_to_reg(exec,
                                           row,
                                           BOOL_REG_INDEX_NRV(to_ra, n) + k,
                                           BOOL_REG_INDEX_NRV(from_ra, n) + k);
                }
              }
            }
          }
        }
      }
      break;
    }
    case slrak_sampler2D: {
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
                                                             INT_REG_INDEX_NRV(to_ra, 0),
                                                             INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                             k,
                                                             to_offset_step_size,
                                                             INT_REG_INDEX_NRV(from_ra, 0),
                                                             INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                             k,
                                                             from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_indir_to_indir_offset(exec,
                                                      row,
                                                      INT_REG_INDEX_NRV(to_ra, 0),
                                                      INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                      k,
                                                      to_offset_step_size,
                                                      INT_REG_INDEX_NRV(from_ra, 0),
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
                                                      INT_REG_INDEX_NRV(to_ra, 0),
                                                      k,
                                                      INT_REG_INDEX_NRV(from_ra, 0),
                                                      INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                      k,
                                                      from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_indir_to_indir(exec,
                                               row,
                                               INT_REG_INDEX_NRV(to_ra, 0),
                                               k,
                                               INT_REG_INDEX_NRV(from_ra, 0),
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
                                                           INT_REG_INDEX_NRV(to_ra, 0),
                                                           INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                           k,
                                                           to_offset_step_size,
                                                           SAMPLER_2D_REG_INDEX_NRV(from_ra, 0) + k,
                                                           INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                           0,
                                                           from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_reg_to_indir_offset(exec,
                                                    row,
                                                    INT_REG_INDEX_NRV(to_ra, 0),
                                                    INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                    k,
                                                    to_offset_step_size,
                                                    SAMPLER_2D_REG_INDEX_NRV(from_ra, 0) + k);
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
                                                    INT_REG_INDEX_NRV(to_ra, 0),
                                                    k,
                                                    SAMPLER_2D_REG_INDEX_NRV(from_ra, 0) + k,
                                                    INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                    0,
                                                    from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_reg_to_indir(exec,
                                             row,
                                             INT_REG_INDEX_NRV(to_ra, 0),
                                             k,
                                             SAMPLER_2D_REG_INDEX_NRV(from_ra, 0) + k);
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
                                                           SAMPLER_2D_REG_INDEX_NRV(to_ra, 0) + k,
                                                           INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                           0,
                                                           to_offset_step_size,
                                                           INT_REG_INDEX_NRV(from_ra, 0),
                                                           INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                           k,
                                                           from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_indir_to_offset_reg(exec,
                                                    row,
                                                    SAMPLER_2D_REG_INDEX_NRV(to_ra, 0) + k,
                                                    INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                    0,
                                                    to_offset_step_size,
                                                    INT_REG_INDEX_NRV(from_ra, 0),
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
                                                    SAMPLER_2D_REG_INDEX_NRV(to_ra, 0) + k,
                                                    INT_REG_INDEX_NRV(from_ra, 0),
                                                    INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                    k,
                                                    from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_indir_to_reg(exec,
                                             row,
                                             SAMPLER_2D_REG_INDEX_NRV(to_ra, 0) + k,
                                             INT_REG_INDEX_NRV(from_ra, 0),
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
                                                         SAMPLER_2D_REG_INDEX_NRV(to_ra, 0) + k,
                                                         INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                         0,
                                                         to_offset_step_size,
                                                         SAMPLER_2D_REG_INDEX_NRV(from_ra, 0) + k,
                                                         INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                         0,
                                                         from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_reg_to_offset_reg(exec,
                                                  row,
                                                  SAMPLER_2D_REG_INDEX_NRV(to_ra, 0) + k,
                                                  INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                  0,
                                                  to_offset_step_size,
                                                  SAMPLER_2D_REG_INDEX_NRV(from_ra, 0) + k);
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
                                                  SAMPLER_2D_REG_INDEX_NRV(to_ra, 0) + k,
                                                  SAMPLER_2D_REG_INDEX_NRV(from_ra, 0) + k,
                                                  INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                  0,
                                                  from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_s2d_reg_to_reg(exec,
                                           row,
                                           SAMPLER_2D_REG_INDEX_NRV(to_ra, 0) + k,
                                           SAMPLER_2D_REG_INDEX_NRV(from_ra, 0) + k);
              }
            }
          }
        }
      }
      break;
    }
    case slrak_samplerCube: {
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
                                                            INT_REG_INDEX_NRV(to_ra, 0),
                                                            INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                            k,
                                                            to_offset_step_size,
                                                            INT_REG_INDEX_NRV(from_ra, 0),
                                                            INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                            k,
                                                            from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_indir_to_indir_offset(exec,
                                                     row,
                                                     INT_REG_INDEX_NRV(to_ra, 0),
                                                     INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                     k,
                                                     to_offset_step_size,
                                                     INT_REG_INDEX_NRV(from_ra, 0),
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
                                                     INT_REG_INDEX_NRV(to_ra, 0),
                                                     k,
                                                     INT_REG_INDEX_NRV(from_ra, 0),
                                                     INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                     k,
                                                     from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_indir_to_indir(exec,
                                              row,
                                              INT_REG_INDEX_NRV(to_ra, 0),
                                              k,
                                              INT_REG_INDEX_NRV(from_ra, 0),
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
                                                          INT_REG_INDEX_NRV(to_ra, 0),
                                                          INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                          k,
                                                          to_offset_step_size,
                                                          SAMPLER_CUBE_REG_INDEX_NRV(from_ra, 0) + k,
                                                          INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                          0,
                                                          from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_reg_to_indir_offset(exec,
                                                   row,
                                                   INT_REG_INDEX_NRV(to_ra, 0),
                                                   INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                   k,
                                                   to_offset_step_size,
                                                   SAMPLER_CUBE_REG_INDEX_NRV(from_ra, 0) + k);
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
                                                   INT_REG_INDEX_NRV(to_ra, 0),
                                                   k,
                                                   SAMPLER_CUBE_REG_INDEX_NRV(from_ra, 0) + k,
                                                   INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                   0,
                                                   from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_reg_to_indir(exec,
                                            row,
                                            INT_REG_INDEX_NRV(to_ra, 0),
                                            k,
                                            SAMPLER_CUBE_REG_INDEX_NRV(from_ra, 0) + k);
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
                                                          SAMPLER_CUBE_REG_INDEX_NRV(to_ra, 0) + k,
                                                          INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                          0,
                                                          to_offset_step_size,
                                                          INT_REG_INDEX_NRV(from_ra, 0),
                                                          INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                          k,
                                                          from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_indir_to_offset_reg(exec,
                                                   row,
                                                   SAMPLER_CUBE_REG_INDEX_NRV(to_ra, 0) + k,
                                                   INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                   0,
                                                   to_offset_step_size,
                                                   INT_REG_INDEX_NRV(from_ra, 0),
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
                                                   SAMPLER_CUBE_REG_INDEX_NRV(to_ra, 0) + k,
                                                   INT_REG_INDEX_NRV(from_ra, 0),
                                                   INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                   k,
                                                   from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_indir_to_reg(exec,
                                            row,
                                            SAMPLER_CUBE_REG_INDEX_NRV(to_ra, 0) + k,
                                            INT_REG_INDEX_NRV(from_ra, 0),
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
                                                        SAMPLER_CUBE_REG_INDEX_NRV(to_ra, 0) + k,
                                                        INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                        0,
                                                        to_offset_step_size,
                                                        SAMPLER_CUBE_REG_INDEX_NRV(from_ra, 0) + k,
                                                        INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                        0,
                                                        from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_reg_to_offset_reg(exec,
                                                 row,
                                                 SAMPLER_CUBE_REG_INDEX_NRV(to_ra, 0) + k,
                                                 INT_REG_INDEX_NRV(to_ra_offset, 0),
                                                 0,
                                                 to_offset_step_size,
                                                 SAMPLER_CUBE_REG_INDEX_NRV(from_ra, 0) + k);
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
                                                 SAMPLER_CUBE_REG_INDEX_NRV(to_ra, 0) + k,
                                                 SAMPLER_CUBE_REG_INDEX_NRV(from_ra, 0) + k,
                                                 INT_REG_INDEX_NRV(from_ra_offset, 0),
                                                 0,
                                                 from_offset_step_size);
              }
            }
            else {
              /* no from offset */
              for (k = 0; k < array_quantity; ++k) {
                sl_reg_move_sc_reg_to_reg(exec,
                                          row,
                                          SAMPLER_CUBE_REG_INDEX_NRV(to_ra, 0) + k,
                                          SAMPLER_CUBE_REG_INDEX_NRV(from_ra, 0) + k);
              }
            }
          }
        }
      }
      break;
    }
  }
}
