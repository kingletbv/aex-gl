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

#ifndef BLITTER_H_INCLUDED
#define BLITTER_H_INCLUDED
#include "blitter.h"
#endif

#define SAMPLER_2D_MAX_NUM_MIPMAPS 32

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

enum s2d_tex_components {
  s2d_alpha,
  s2d_luminance,
  s2d_luminance_alpha,
  s2d_rgb,
  s2d_rgba
};

struct sampler_2d_map {
  int width_;
  int height_;
  uint32_t repeat_mask_s_, repeat_mask_t_;
  enum s2d_tex_components components_;
  size_t num_bytes_per_bitmap_row_;
  void *bitmap_;
};

struct sampler_2d {
  enum s2d_wrap wrap_s_, wrap_t_;           
  enum s2d_filter min_filter_;  /* any of s2d_filter */
  enum s2d_filter mag_filter_;  /* s2d_nearest, s2d_linear, no other */
  enum s2d_tex_components components_;

  /* A sampler (texture) is complete if, when min_filter_
   * indicates mipmapping, all levels are filled and have the same components_
   * value (components_ maps directly onto opengl internalformat.)
   * Additionally, all mipmap levels should have bitmaps set, and appropriate
   * (positive) dimensions. 
   * If min_filter_ does not imply mip-mapping, then we should have a map
   * in mipmaps_[0] and it should be valid (positive dimensions, and have
   * bitmap_ specified.)
   * See 3.7.10 Texture Completeness. */
  int is_complete_:1;

  /* Indicates if the dimensions of level 0 (and by extension any other if
   * the texture is complete), if the dimensions of level 0 are a power of
   * 2. Need not be the same power (e.g. if width is 128 and height is 64,
   * this will be true.) */
  int is_power_of_two_:1;

  int num_maps_;
  struct sampler_2d_map *mipmaps_;

  /* list of all samplers part of the current evaluation, each sampler
   * contains the set of rows it is currently being evaluated by. */
  struct sampler_2d *runtime_active_sampler_chain_;
  uint32_t runtime_rows_;
  uint32_t last_row_;

  uint8_t tex_exec_[SL_EXEC_CHAIN_MAX_NUM_ROWS];
};

void sampler_2d_init(struct sampler_2d *s2d);
void sampler_2d_cleanup(struct sampler_2d *s2d);

int sampler_2d_set_storage(struct sampler_2d *s2d, int level, enum s2d_tex_components internal_format, int width, int height);
int sampler_2d_set_image(struct sampler_2d *s2d, int level, enum s2d_tex_components internal_format, int width, int height,
                         enum blitter_data_type src_datatype, void *src_data);
int sampler_2d_generate_mipmaps(struct sampler_2d *s2d);

void builtin_texture2D_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2D_bias_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DProj_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DProj_v3_bias_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DProj_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DProj_v4_bias_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DLod_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DProjLod_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_texture2DProjLod_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

void builtin_textureCube_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_textureCube_v3_bias_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
void builtin_textureCubeLod_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SAMPLER_2D_H */
