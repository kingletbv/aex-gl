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

#ifndef SL_REG_MOVE_H
#define SL_REG_MOVE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

struct sl_execution;
struct sl_reg_alloc;

void sl_reg_move_crossframe(struct sl_execution *exec,
                            uint8_t row,
                            int from_frame, struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset,
                            int to_frame, struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset,
                            int from_offset_step_size, int to_offset_step_size,
                            int array_quantity);

void sl_reg_move_crossframe_c2c(struct sl_execution *exec,
                                uint8_t row,
                                int from_frame, struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset, int from_component,
                                int to_frame, struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset, int to_component,
                                int from_offset_step_size, int to_offset_step_size,
                                int array_quantity);

void sl_reg_move(struct sl_execution *exec,
                 uint8_t row,
                 struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset,
                 struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset);

void sl_reg_move_c2c(struct sl_execution *exec,
                     uint8_t row,
                     struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset, int from_component,
                     struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset, int to_component);

void sl_reg_emit_move_crossframe(struct ir_block *blk,
                                 struct ir_temp *chain_reg,
                                 struct sl_execution_frame *from_frame, struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset,
                                 struct sl_execution_frame *to_frame, struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset,
                                 int from_offset_step_size, int to_offset_step_size,
                                 int array_quantity);

void sl_reg_emit_move_crossframe_c2c(struct ir_block *blk,
                                     struct ir_temp *chain_reg,
                                     struct sl_execution_frame *from_frame, struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset, int from_component,
                                     struct sl_execution_frame *to_frame, struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset, int to_component,
                                     int from_offset_step_size, int to_offset_step_size,
                                     int array_quantity);

void sl_reg_emit_move(struct ir_block *blk,
                      struct ir_temp *chain_reg,
                      struct sl_execution_frame *local_frame,
                      struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset,
                      struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset);

void sl_reg_emit_move_c2c(struct ir_block *blk,
                          struct ir_temp *chain_reg,
                          struct sl_execution_frame *local_frame,
                          struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset, int from_component,
                          struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset, int to_component);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_REG_MOVE_H */
