/* Copyright 2024 Kinglet B.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License", builtin_not_implemented_runtime, builtin_not_implemented_eval);
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
#ifndef SAMPLER_2D_H
#define SAMPLER_2D_H

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum s2d_wrap {
  s2d_clamp_to_edge,
  s2d_repeat,
  s2d_mirrored_repeat
};

enum s2d_filter {
  s2d_nearest,
  s2d_linear,
  s2d_nearest_mipmap_nearest,
  s2d_nearest_mipmap_linear,
  s2d_linear_mipmap_nearest,
  s2d_linear_mipmap_linear
};

struct sampler_2d {
  enum s2d_wrap wrap_s_, wrap_t_;           
  enum s2d_filter min_filter_;  /* any of s2d_filter */
  enum s2d_filter mag_filter_;  /* s2d_nearest, s2d_linear, no other */

  int width_;
  int height_;

  int q_; /* maximum mip-map level, counting from 0 for the original texture upwards */

  /* list of all samplers part of the current evaluation, each sampler
   * contains the set of rows it is currently being evaluated by. */
  struct sampler_2d *runtime_active_sampler_chain_;
  uint8_t runtime_rows_;
  uint32_t last_row_;

  float ds_dx_[SL_EXEC_CHAIN_MAX_NUM_ROWS];
  float ds_dy_[SL_EXEC_CHAIN_MAX_NUM_ROWS];
  float dt_dx_[SL_EXEC_CHAIN_MAX_NUM_ROWS];
  float dt_dy_[SL_EXEC_CHAIN_MAX_NUM_ROWS];
  float dst_log2_[SL_EXEC_CHAIN_MAX_NUM_ROWS];
  float ranged_s_[SL_EXEC_CHAIN_MAX_NUM_ROWS];
  float ranged_t_[SL_EXEC_CHAIN_MAX_NUM_ROWS];
  int64_t floored_st_[SL_EXEC_CHAIN_MAX_NUM_ROWS];
};

void builtin_texture2D_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2D_bias_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DProj_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DProj_v3_bias_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DProj_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DProj_v4_bias_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DLod_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DProjLod_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DProjLod_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SAMPLER_2D_H */