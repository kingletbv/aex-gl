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
#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED
#include <math.h>
#endif

#ifndef SL_DEFS_H_INCLUDED
#define SL_DEFS_H_INCLUDED
#include "sl_defs.h"
#endif

#ifndef SL_EXPR_H_INCLUDED
#define SL_EXPR_H_INCLUDED
#include "sl_expr.h"
#endif

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
#endif

#ifndef SAMPLER_2D_H_INCLUDED
#define SAMPLER_2D_H_INCLUDED
#include "sampler_2d.h"
#endif

#ifndef BLITTER_H_INCLUDED
#define BLITTER_H_INCLUDED
#include "blitter.h"
#endif

void sampler_2d_init(struct sampler_2d *s2d) {
  /* Defaults as per ES 2.0 v2.0.25 3.7.12 Texture State (p.84) */
  s2d->wrap_s_ = s2d_repeat;
  s2d->wrap_t_ = s2d_repeat;
  s2d->min_filter_ = s2d_nearest_mipmap_linear;
  s2d->mag_filter_ = s2d_linear;
  s2d->num_maps_ = 0;
  s2d->mipmaps_ = NULL;
  
  s2d->runtime_active_sampler_chain_ = NULL;
  s2d->runtime_rows_ = SL_EXEC_NO_CHAIN;
  s2d->last_row_ = SL_EXEC_NO_CHAIN;
}

void sampler_2d_cleanup(struct sampler_2d *s2d) {
  size_t n;
  for (n = 0; n < s2d->num_maps_; ++n) {
    struct sampler_2d_map *s2dm = s2d->mipmaps_ + n;
    free(s2dm->bitmap_);
  }
  if (s2d->mipmaps_) free(s2d->mipmaps_);
}

static int bitcount(uint32_t x) {
  int l0 = ((x & 0xAAAAAAAA) >> 1) + (x & 0x55555555);
  int l1 = ((l0 & 0xCCCCCCCC) >> 2) + (l0 & 0x33333333);
  int l2 = ((l1 & 0xF0F0F0F0) >> 4) + (l1 & 0x0F0F0F0F);
  int l3 = ((l2 & 0xFF00FF00) >> 8) + (l2 & 0x00FF00FF);
  int l4 = ((l3 & 0xFFFF0000) >> 16) + (l3 & 0x0000FFFF);
  return l4;
}

static int is_power_of_two(uint32_t x) {
  /* simple bitcount */
  return bitcount(x) == 1;
}

/* mersenne number is, for any n, the number (2**n)-1
 * not to be confused with mersenne primes which are a special case. */
static uint32_t next_greater_or_equal_mersenne_number(uint32_t x) {
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x;
}

static uint32_t isolate_msb(uint32_t x) {
  uint32_t y = next_greater_or_equal_mersenne_number(x);
  y = y & ~(y >> 1);
  return y;
}

void sampler_2d_update_level_repetition_masks(struct sampler_2d *s2d) {
  size_t n;
  
  for (n = 0; n < s2d->num_maps_; ++n) {
    if (s2d->wrap_s_ == s2d_repeat) {
      s2d->mipmaps_[n].repeat_mask_s_ = isolate_msb((uint32_t)s2d->mipmaps_[n].width_) - 1;
    }
    else {
      s2d->mipmaps_[n].repeat_mask_s_ = ~(uint32_t)0;
    }
    if (s2d->wrap_t_ == s2d_repeat) {
      s2d->mipmaps_[n].repeat_mask_t_ = isolate_msb((uint32_t)s2d->mipmaps_[n].height_) - 1;
    }
    else {
      s2d->mipmaps_[n].repeat_mask_t_ = ~(uint32_t)0;
    }
  }
}

static void sampler_2d_update_completeness(struct sampler_2d *s2d) {
  if (s2d->num_maps_ == 0) {
    s2d->is_complete_ = 0;
    return;
  }
  s2d->is_power_of_two_ = is_power_of_two((uint32_t)s2d->mipmaps_[0].width_) &&
                          is_power_of_two((uint32_t)s2d->mipmaps_[0].height_);

  if ((s2d->min_filter_ == s2d_nearest) ||
      (s2d->min_filter_ == s2d_linear)) {
    if ((s2d->mipmaps_[0].width_ > 0) &&
        (s2d->mipmaps_[0].height_ > 0)) {
      s2d->is_complete_ = 1;
      return;
    }
  }
  else if ((s2d->min_filter_ == s2d_nearest_mipmap_nearest) ||
           (s2d->min_filter_ == s2d_nearest_mipmap_linear) ||
           (s2d->min_filter_ == s2d_linear_mipmap_nearest) ||
           (s2d->min_filter_ == s2d_linear_mipmap_linear)) {
    size_t n;
    if (s2d->num_maps_ == 0) {
      s2d->is_complete_ = 0;
      return;
    }
    int current_width, current_height;
    current_width = s2d->mipmaps_[0].width_;
    current_height = s2d->mipmaps_[0].height_;
    if ((current_width <= 0) || (current_height <= 0)) {
      s2d->is_complete_ = 0;
      return;
    }
    int maxwh = (current_width > current_height) ? current_width : current_height;
    float flg2_maxwh = floorf(log2f((float)maxwh));
    if (flg2_maxwh > (float)INT_MAX) {
      s2d->is_complete_ = 0;
      return;
    }
    int lg2_maxwh = (int)flg2_maxwh;
    int num_mipmaps_needed = lg2_maxwh + 1;

    if (num_mipmaps_needed > s2d->num_maps_) {
      s2d->is_complete_ = 0;
      return;
    }

    for (n = 1; n < num_mipmaps_needed; ++n) {
      current_width = (1 > current_width / 2) ? 1 : (current_width / 2);
      current_height = (1 > current_height / 2) ? 1 : (current_height / 2);
      if ((s2d->mipmaps_[n].width_ != current_width) ||
          (s2d->mipmaps_[n].height_ != current_height)) {
        s2d->is_complete_ = 0;
        return;
      }
      if (s2d->mipmaps_[n].components_ != s2d->mipmaps_[0].components_) {
        s2d->is_complete_ = 0;
        return;
      }
    }
  }
}


static void texture2D(float *prgba, struct sampler_2d *s2d, float s, float t, float lg2) {

  /* c as meant in section 3.7.8 texture magnification (OpenGL ES 2.0 full spec v2.0.25 page 82) */
  float c;
  if ((s2d->mag_filter_ == s2d_linear) &&
      ((s2d->min_filter_ == s2d_nearest_mipmap_nearest) ||
       (s2d->min_filter_ == s2d_nearest_mipmap_linear))) {
    c = 0.5f;
  }
  else {
    c = 0.f;
  }

  enum s2d_filter filter;
  if ((s2d->mag_filter_ != s2d->min_filter_) && (lg2 <= c)) {
    /* Magnification filter */
    filter = s2d->mag_filter_;
  }
  else {
    /* Minification filter */
    filter = s2d->min_filter_;
  }

  if ((filter == s2d_nearest) || 
      (filter == s2d_nearest_mipmap_nearest) || 
      (filter == s2d_nearest_mipmap_linear)) {
    int num_levels;
    struct sampler_2d_map *s2dm_levels[2] = {NULL, NULL};
    struct sampler_2d_map *s2dm;
    uint32_t tau1_fract = 0;
    uint32_t tau2_fract = 0;
    uint64_t reds[2], greens[2], blues[2], alphas[2];
    if (filter == s2d_nearest) {
      s2dm_levels[0] = s2d->mipmaps_;
      num_levels = 1;
    }
    else if (filter == s2d_nearest_mipmap_nearest) {
      int last_mipmap = s2d->num_maps_ - 1;
      int nearest_mipmap = (lg2 <= 0.5f) ? 0 : (lg2 > (0.5f + (float)last_mipmap)) ? last_mipmap : ((int)(ceilf(lg2 + 0.5f)) - 1);
      s2dm_levels[0] = s2d->mipmaps_ + nearest_mipmap;
      num_levels = 1;
    }
    else if (filter == s2d_nearest_mipmap_linear) {
      int last_mipmap = s2d->num_maps_ - 1;
      float ffloor_lg2 = floorf(lg2);
      float ffract_lg2 = lg2 - ffloor_lg2;
      int floor_lg2 = (int)ffloor_lg2;
      int d1_mipmap = (floor_lg2 >= last_mipmap) ? last_mipmap : floor_lg2;
      int d2_mipmap = (floor_lg2 >= last_mipmap) ? last_mipmap : d1_mipmap + 1;
      struct sampler_2d_map *s2d1m = s2d->mipmaps_ + d1_mipmap;
      struct sampler_2d_map *s2d2m = s2d->mipmaps_ + d2_mipmap;
      tau2_fract = (uint32_t)(256.f * ffract_lg2);
      tau1_fract = 256 - tau2_fract;
      s2dm_levels[0] = s2d1m;
      s2dm_levels[1] = s2d2m;
      num_levels = 2;
    }
    int level;
    for (level = 0; level < num_levels; ++level) {
      s2dm = s2dm_levels[level];
      float sm = s, tm = t;
      switch (s2d->wrap_s_) {
        case s2d_clamp_to_edge: {
          float min_fs = 1 / (2.f * (float)(s2dm->width_));
          float max_fs = 1.f - min_fs;
          sm = (sm < min_fs) ? min_fs : ((sm > max_fs) ? max_fs : sm);
          break;
        }
        case s2d_repeat: {
          /* leave as-is, mask will take care of it */
          break;
        }
        case s2d_mirrored_repeat: {
          float floared = floorf(sm);
          float fract = sm - floared;
          sm = (1 & (int64_t)floared) ? (1.f - fract) : fract;
          break;
        }
      }
      uint32_t tex_s = (uint32_t)(sm * s2dm->width_) & s2dm->repeat_mask_s_;

      switch (s2d->wrap_t_) {
        case s2d_clamp_to_edge: {
          float min_ft = 1 / (2.f * (float)(s2dm->width_));
          float max_ft = 1.f - min_ft;
          tm = (tm < min_ft) ? min_ft : ((tm > max_ft) ? max_ft : tm);
          break;
        }
        case s2d_repeat: {
          /* leave as-is, mask will take care of it */
          break;
        }
        case s2d_mirrored_repeat: {
          float floared = floorf(tm);
          float fract = tm - floared;
          tm = (1 & (int64_t)floared) ? (1.f - fract) : fract;
          break;
        }
      }
      uint32_t tex_t = (uint32_t)(tm * s2dm->height_) & s2dm->repeat_mask_t_;

      /* Load texel at tex_s and tex_t */
      switch (s2dm->components_) {
        case s2d_alpha:
          alphas[level] = ((uint8_t * restrict)s2dm->bitmap_)[s2dm->width_ * tex_t + tex_s];
          break;
          break;
        case s2d_luminance:
          reds[level] = ((uint8_t * restrict)s2dm->bitmap_)[s2dm->width_ * tex_t + tex_s];
          break;
        case s2d_luminance_alpha: {
          uint8_t *restrict ptexel = ((uint8_t * restrict)s2dm->bitmap_) + 2 * (s2dm->width_ * tex_t + tex_s);
          reds[level]   = ptexel[0];
          alphas[level] = ptexel[1];
          break;
        }
        case s2d_rgb: {
          size_t texel_at_pixel = s2dm->width_ * tex_t + tex_s;
          size_t texel_at_offset = texel_at_pixel * 3; /* "* 4 - 1", rely on compiler to catch this optimization */
          uint8_t *restrict ptexel = ((uint8_t * restrict)s2dm->bitmap_) + texel_at_offset;
          reds[level]   = ptexel[0];
          greens[level] = ptexel[1];
          blues[level]  = ptexel[2];
          break;
        }
        case s2d_rgba: {
          uint8_t *restrict ptexel = ((uint8_t * restrict)s2dm->bitmap_) + 4 * (s2dm->width_ * tex_t + tex_s);
          reds[level]   = ptexel[0];
          greens[level] = ptexel[1];
          blues[level]  = ptexel[2];
          alphas[level] = ptexel[3];
          break;
        }
      }
    }
    if ((filter == s2d_nearest) ||
        (filter == s2d_nearest_mipmap_nearest)) {
      switch (s2dm_levels[0]->components_) {
        case s2d_alpha:
          prgba[0] = 0.f;
          prgba[1] = 0.f;
          prgba[2] = 0.f;
          prgba[3] = ((float)alphas[0]) / 255.f;
          break;
        case s2d_luminance:
          prgba[0] = ((float)reds[0]) / 255.f;
          prgba[1] = 0.f;
          prgba[2] = 0.f;
          prgba[3] = 0.f;
          break;
        case s2d_luminance_alpha: {
          prgba[0] = ((float)reds[0]) / 255.f;
          prgba[1] = 0.f;
          prgba[2] = 0.f;
          prgba[3] = ((float)alphas[0]) / 255.f;
          break;
        }
        case s2d_rgb: {
          prgba[0] = ((float)reds[0]) / 255.f;
          prgba[1] = ((float)greens[0]) / 255.f;
          prgba[2] = ((float)blues[0]) / 255.f;
          prgba[3] = 1.f;
          break;
        }
        case s2d_rgba: {
          prgba[0] = ((float)reds[0]) / 255.f;
          prgba[1] = ((float)greens[0]) / 255.f;
          prgba[2] = ((float)blues[0]) / 255.f;
          prgba[3] = ((float)alphas[0]) / 255.f;
          break;
        }
      }
    }
    else if (filter == s2d_nearest_mipmap_linear) {
      switch (s2dm_levels[0]->components_) {
        case s2d_alpha:
          prgba[0] = 0.f;
          prgba[1] = 0.f;
          prgba[2] = 0.f;
          prgba[3] = ((float)(alphas[0] * tau1_fract + alphas[1] * tau2_fract)) / (256.f * 255.f);
          break;
        case s2d_luminance:
          prgba[0] = ((float)(reds[0] * tau1_fract + reds[1] * tau2_fract)) / (256.f * 255.f);
          prgba[1] = 0.f;
          prgba[2] = 0.f;
          prgba[3] = 0.f;
          break;
        case s2d_luminance_alpha: {
          prgba[0] = ((float)(reds[0] * tau1_fract + reds[1] * tau2_fract)) / (256.f * 255.f);
          prgba[1] = 0.f;
          prgba[2] = 0.f;
          prgba[3] = ((float)(alphas[0] * tau1_fract + alphas[1] * tau2_fract)) / (256.f * 255.f);
          break;
        }
        case s2d_rgb: {
          prgba[0] = ((float)(reds[0] * tau1_fract + reds[1] * tau2_fract)) / (256.f * 255.f);
          prgba[1] = ((float)(greens[0] * tau1_fract + greens[1] * tau2_fract)) / (256.f * 255.f);
          prgba[2] = ((float)(blues[0] * tau1_fract + blues[1] * tau2_fract)) / (256.f * 255.f);
          prgba[3] = 1.f;
          break;
        }
        case s2d_rgba: {
          prgba[0] = ((float)(reds[0] * tau1_fract + reds[1] * tau2_fract)) / (256.f * 255.f);
          prgba[1] = ((float)(greens[0] * tau1_fract + greens[1] * tau2_fract)) / (256.f * 255.f);
          prgba[2] = ((float)(blues[0] * tau1_fract + blues[1] * tau2_fract)) / (256.f * 255.f);
          prgba[3] = ((float)(alphas[0] * tau1_fract + alphas[1] * tau2_fract)) / (256.f * 255.f);
          break;
        }
      }
    }
  }
  else if ((filter == s2d_linear) || 
           (filter == s2d_linear_mipmap_nearest) ||
           (filter == s2d_linear_mipmap_linear)) {
    int num_levels;
    struct sampler_2d_map *s2dm_levels[2] = {NULL, NULL};
    struct sampler_2d_map *s2dm;
    uint32_t tau1_fract = 0;
    uint32_t tau2_fract = 0;
    uint64_t reds[2], greens[2], blues[2], alphas[2];
    if (filter == s2d_linear) {
      s2dm_levels[0] = s2d->mipmaps_;
      num_levels = 1;
    }
    if (filter == s2d_linear_mipmap_nearest) {
      int last_mipmap = s2d->num_maps_ - 1;
      int nearest_mipmap = (lg2 <= 0.5f) ? 0 : (lg2 > (0.5f + (float)last_mipmap)) ? last_mipmap : ((int)(ceilf(lg2 + 0.5f)) - 1);
      s2dm_levels[0] = s2d->mipmaps_ + nearest_mipmap;
      num_levels = 1;
    }
    else if (filter == s2d_linear_mipmap_linear) {
      int last_mipmap = s2d->num_maps_ - 1;
      float ffloor_lg2 = floorf(lg2);
      float ffract_lg2 = lg2 - ffloor_lg2;
      int floor_lg2 = (int)ffloor_lg2;
      int d1_mipmap = (floor_lg2 >= last_mipmap) ? last_mipmap : floor_lg2;
      int d2_mipmap = (floor_lg2 >= last_mipmap) ? last_mipmap : d1_mipmap + 1;
      struct sampler_2d_map *s2d1m = s2d->mipmaps_ + d1_mipmap;
      struct sampler_2d_map *s2d2m = s2d->mipmaps_ + d2_mipmap;
      tau2_fract = (uint32_t)(256.f * ffract_lg2);
      tau1_fract = 256 - tau2_fract;
      s2dm_levels[0] = s2d1m;
      s2dm_levels[1] = s2d2m;
      num_levels = 2;
    }


    int level;
    for (level = 0; level < num_levels; ++level) {
      s2dm = s2dm_levels[level];
      float sm = s, tm = t;

      switch (s2d->wrap_s_) {
        case s2d_clamp_to_edge: {
          float min_fs = 1 / (2.f * (float)(s2dm->width_));
          float max_fs = 1.f - min_fs;
          sm = (sm < min_fs) ? min_fs : ((sm > max_fs) ? max_fs : sm);
          break;
        }
        case s2d_repeat: {
          /* leave as-is, mask will take care of it */
          break;
        }
        case s2d_mirrored_repeat: {
          float floared = floorf(sm);
          float fract = sm - floared;
          sm = (1 & (int64_t)floared) ? (1.f - fract) : fract;
          break;
        }
      }
      /* Note the -128: we have the texel center at (0.5, 0.5) but need
       * to offset it to (0,0) so we can range the linear interpolation from
       * 0 to 1 in terms of sub-texel coordinates; this is not the case for
       * the nearest filter, only linear. Same for t coordinate. */
      uint32_t tex_s_fp8 = ((uint32_t)(sm * s2dm->width_ * 256.f)) - 128;
      uint32_t tex_s0 = (tex_s_fp8 >> 8) & s2dm->repeat_mask_s_;
      uint32_t tex_s1 = (tex_s0 + 1) & s2dm->repeat_mask_s_;
      uint32_t tex_s1_fract = tex_s_fp8 & 255;
      uint32_t tex_s0_fract = 256 - tex_s1_fract;

      switch (s2d->wrap_t_) {
        case s2d_clamp_to_edge: {
          float min_ft = 1 / (2.f * (float)(s2dm->width_));
          float max_ft = 1.f - min_ft;
          tm = (tm < min_ft) ? min_ft : ((tm > max_ft) ? max_ft : tm);
          break;
        }
        case s2d_repeat: {
          /* leave as-is, mask will take care of it */
          break;
        }
        case s2d_mirrored_repeat: {
          float floared = floorf(tm);
          float fract = tm - floared;
          tm = (1 & (int64_t)floared) ? (1.f - fract) : fract;
          break;
        }
      }

      uint32_t tex_t_fp8 = ((uint32_t)(tm * s2dm->height_ * 256.f)) - 128;
      uint32_t tex_t0 = (tex_t_fp8 >> 8) & s2dm->repeat_mask_s_;
      uint32_t tex_t1 = (tex_t0 + 1) & s2dm->repeat_mask_t_;
      uint32_t tex_t1_fract = tex_t_fp8 & 255;
      uint32_t tex_t0_fract = 256 - tex_t1_fract;

      /* Prepare for bilinear filtering
       * range tex_st_XX_fract is 0..65536,
       * tex_st_00_fract + tex_st_01_fract + tex_st_10_fract + tex_st_11_fract = 65536
       */
      uint32_t tex_st_00_fract = tex_s0_fract * tex_t0_fract;
      uint32_t tex_st_01_fract = tex_s0_fract * tex_t1_fract;
      uint32_t tex_st_10_fract = tex_s1_fract * tex_t0_fract;
      uint32_t tex_st_11_fract = tex_s1_fract * tex_t1_fract;

      /* Load texel at tex_s and tex_t */
      switch (s2dm_levels[0]->components_) {
        case s2d_alpha: {
          uint32_t tex = ((uint8_t * restrict)s2dm->bitmap_)[s2dm->width_ * tex_t0 + tex_s0] * tex_st_00_fract
                       + ((uint8_t * restrict)s2dm->bitmap_)[s2dm->width_ * tex_t1 + tex_s0] * tex_st_01_fract
                       + ((uint8_t * restrict)s2dm->bitmap_)[s2dm->width_ * tex_t0 + tex_s1] * tex_st_10_fract
                       + ((uint8_t * restrict)s2dm->bitmap_)[s2dm->width_ * tex_t1 + tex_s1] * tex_st_11_fract;
          alphas[level] = tex;
          break;
        }
        case s2d_luminance: {
          uint32_t tex = ((uint8_t * restrict)s2dm->bitmap_)[s2dm->width_ * tex_t0 + tex_s0] * tex_st_00_fract
                       + ((uint8_t * restrict)s2dm->bitmap_)[s2dm->width_ * tex_t1 + tex_s0] * tex_st_01_fract
                       + ((uint8_t * restrict)s2dm->bitmap_)[s2dm->width_ * tex_t0 + tex_s1] * tex_st_10_fract
                       + ((uint8_t * restrict)s2dm->bitmap_)[s2dm->width_ * tex_t1 + tex_s1] * tex_st_11_fract;
          reds[level] = tex;
          break;
        }
        case s2d_luminance_alpha: {
          uint8_t * restrict ptex00 = ((uint8_t * restrict)s2dm->bitmap_) + 2 * (s2dm->width_ * tex_t0 + tex_s0);
          uint8_t * restrict ptex01 = ((uint8_t * restrict)s2dm->bitmap_) + 2 * (s2dm->width_ * tex_t1 + tex_s0);
          uint8_t * restrict ptex10 = ((uint8_t * restrict)s2dm->bitmap_) + 2 * (s2dm->width_ * tex_t0 + tex_s1);
          uint8_t * restrict ptex11 = ((uint8_t * restrict)s2dm->bitmap_) + 2 * (s2dm->width_ * tex_t1 + tex_s1);

          uint32_t lum_tex = ptex00[0] * tex_st_00_fract
                           + ptex01[0] * tex_st_01_fract
                           + ptex10[0] * tex_st_10_fract
                           + ptex11[0] * tex_st_11_fract;

          uint32_t alpha_tex = ptex00[1] * tex_st_00_fract
                             + ptex01[1] * tex_st_01_fract
                             + ptex10[1] * tex_st_10_fract
                             + ptex11[1] * tex_st_11_fract;

          reds[level] = lum_tex;
          alphas[level] = alpha_tex;
          break;
        }
        case s2d_rgb: {
          uint8_t *restrict ptex00 = ((uint8_t * restrict)s2dm->bitmap_) + 3 * (s2dm->width_ * tex_t0 + tex_s0);
          uint8_t *restrict ptex01 = ((uint8_t * restrict)s2dm->bitmap_) + 3 * (s2dm->width_ * tex_t1 + tex_s0);
          uint8_t *restrict ptex10 = ((uint8_t * restrict)s2dm->bitmap_) + 3 * (s2dm->width_ * tex_t0 + tex_s1);
          uint8_t *restrict ptex11 = ((uint8_t * restrict)s2dm->bitmap_) + 3 * (s2dm->width_ * tex_t1 + tex_s1);

          uint32_t red_tex = ptex00[0] * tex_st_00_fract
                           + ptex01[0] * tex_st_01_fract
                           + ptex10[0] * tex_st_10_fract
                           + ptex11[0] * tex_st_11_fract;

          uint32_t green_tex = ptex00[1] * tex_st_00_fract
                             + ptex01[1] * tex_st_01_fract
                             + ptex10[1] * tex_st_10_fract
                             + ptex11[1] * tex_st_11_fract;

          uint32_t blue_tex = ptex00[2] * tex_st_00_fract
                            + ptex01[2] * tex_st_01_fract
                            + ptex10[2] * tex_st_10_fract
                            + ptex11[2] * tex_st_11_fract;

          reds[level] = red_tex;
          greens[level] = green_tex;
          blues[level] = blue_tex;
          break;
        }
        case s2d_rgba: {
          uint8_t *restrict ptex00 = ((uint8_t * restrict)s2dm->bitmap_) + 4 * (s2dm->width_ * tex_t0 + tex_s0);
          uint8_t *restrict ptex01 = ((uint8_t * restrict)s2dm->bitmap_) + 4 * (s2dm->width_ * tex_t1 + tex_s0);
          uint8_t *restrict ptex10 = ((uint8_t * restrict)s2dm->bitmap_) + 4 * (s2dm->width_ * tex_t0 + tex_s1);
          uint8_t *restrict ptex11 = ((uint8_t * restrict)s2dm->bitmap_) + 4 * (s2dm->width_ * tex_t1 + tex_s1);

          uint32_t red_tex = ptex00[0] * tex_st_00_fract
                           + ptex01[0] * tex_st_01_fract
                           + ptex10[0] * tex_st_10_fract
                           + ptex11[0] * tex_st_11_fract;

          uint32_t green_tex = ptex00[1] * tex_st_00_fract
                             + ptex01[1] * tex_st_01_fract
                             + ptex10[1] * tex_st_10_fract
                             + ptex11[1] * tex_st_11_fract;

          uint32_t blue_tex = ptex00[2] * tex_st_00_fract
                            + ptex01[2] * tex_st_01_fract
                            + ptex10[2] * tex_st_10_fract
                            + ptex11[2] * tex_st_11_fract;

          uint32_t alpha_tex = ptex00[3] * tex_st_00_fract
                             + ptex01[3] * tex_st_01_fract
                             + ptex10[3] * tex_st_10_fract
                             + ptex11[3] * tex_st_11_fract;

          reds[level] = red_tex;
          greens[level] = green_tex;
          blues[level] = blue_tex;
          alphas[level] = alpha_tex;
          break;
        }
      }
    }
    if ((filter == s2d_linear) ||
        (filter == s2d_linear_mipmap_nearest)) {
      /* bilinear mixing means the magnitude is 65536 (see comment above) more */
      switch (s2dm_levels[0]->components_) {
        case s2d_alpha:
          prgba[0] = 0.f;
          prgba[1] = 0.f;
          prgba[2] = 0.f;
          prgba[3] = ((float)alphas[0]) / (65536.f * 255.f);
          break;
        case s2d_luminance:
          prgba[0] = ((float)reds[0]) / (65536.f * 255.f);
          prgba[1] = 0.f;
          prgba[2] = 0.f;
          prgba[3] = 0.f;
          break;
        case s2d_luminance_alpha: {
          prgba[0] = ((float)reds[0]) / (65536.f * 255.f);
          prgba[1] = 0.f;
          prgba[2] = 0.f;
          prgba[3] = ((float)alphas[0]) / (65536.f * 255.f);
          break;
        }
        case s2d_rgb: {
          prgba[0] = ((float)reds[0]) / (65536.f * 255.f);
          prgba[1] = ((float)greens[0]) / (65536.f * 255.f);
          prgba[2] = ((float)blues[0]) / (65536.f * 255.f);
          prgba[3] = 1.f;
          break;
        }
        case s2d_rgba: {
          prgba[0] = ((float)reds[0]) / (65536.f * 255.f);
          prgba[1] = ((float)greens[0]) / (65536.f * 255.f);
          prgba[2] = ((float)blues[0]) / (65536.f * 255.f);
          prgba[3] = ((float)alphas[0]) / (65536.f * 255.f);
          break;
        }
      }
    }
    else if (filter == s2d_linear_mipmap_linear) {
      /* trilinear mixing means the magnitude is 65336 x 256 = 16777216. */
      switch (s2dm_levels[0]->components_) {
        case s2d_alpha:
          prgba[0] = 0.f;
          prgba[1] = 0.f;
          prgba[2] = 0.f;
          prgba[3] = ((float)(alphas[0] * tau1_fract + alphas[1] * tau2_fract)) / (16777216.f * 255.f);
          break;
        case s2d_luminance:
          prgba[0] = ((float)(reds[0] * tau1_fract + reds[1] * tau2_fract)) / (16777216.f * 255.f);
          prgba[1] = 0.f;
          prgba[2] = 0.f;
          prgba[3] = 0.f;
          break;
        case s2d_luminance_alpha: {
          prgba[0] = ((float)(reds[0] * tau1_fract + reds[1] * tau2_fract)) / (16777216.f * 255.f);
          prgba[1] = 0.f;
          prgba[2] = 0.f;
          prgba[3] = ((float)(alphas[0] * tau1_fract + alphas[1] * tau2_fract)) / (16777216.f * 255.f);
          break;
        }
        case s2d_rgb: {
          prgba[0] = ((float)(reds[0] * tau1_fract + reds[1] * tau2_fract)) / (16777216.f * 255.f);
          prgba[1] = ((float)(greens[0] * tau1_fract + greens[1] * tau2_fract)) / (16777216.f * 255.f);
          prgba[2] = ((float)(blues[0] * tau1_fract + blues[1] * tau2_fract)) / (16777216.f * 255.f);
          prgba[3] = 1.f;
          break;
        }
        case s2d_rgba: {
          prgba[0] = ((float)(reds[0] * tau1_fract + reds[1] * tau2_fract)) / (16777216.f * 255.f);
          prgba[1] = ((float)(greens[0] * tau1_fract + greens[1] * tau2_fract)) / (16777216.f * 255.f);
          prgba[2] = ((float)(blues[0] * tau1_fract + blues[1] * tau2_fract)) / (16777216.f * 255.f);
          prgba[3] = ((float)(alphas[0] * tau1_fract + alphas[1] * tau2_fract)) / (16777216.f * 255.f);
          break;
        }
      }
    }
  }
}

struct sampler_2d *split_execution_chains_to_sampler_tex_chains(struct sl_execution *exec, int exec_chain, void * restrict * restrict sampler_column) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  uint32_t row = exec_chain;
  struct sampler_2d *samplers = NULL;

  /* Split the rows to each sampler, we want to evaluate all rows such that the sampler is held
   * constant. */
  uint8_t delta;
  do {
    delta = chain_column[row];

    struct sampler_2d *s2d = sampler_column[row];
    uint8_t *restrict tex_chain_column = s2d->tex_exec_;

    if (s2d->last_row_ != SL_EXEC_NO_CHAIN) {
      tex_chain_column[s2d->last_row_] = row - s2d->last_row_;
      s2d->last_row_ = row;
    }
    else {
      s2d->runtime_rows_ = row;
      s2d->last_row_ = row;
      if (samplers) {
        s2d->runtime_active_sampler_chain_ = samplers->runtime_active_sampler_chain_;
        samplers->runtime_active_sampler_chain_ = s2d;
      }
      else {
        s2d->runtime_active_sampler_chain_ = s2d;
      }
      samplers = s2d;
    }

    row += delta;
  } while (delta);

  return samplers;
}

void builtin_texture2D_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict red_column = FLOAT_REG_PTR(x, 0);
  float *restrict green_column = FLOAT_REG_PTR(x, 1);
  float *restrict blue_column = FLOAT_REG_PTR(x, 2);
  float *restrict alpha_column = FLOAT_REG_PTR(x, 3);
  float *restrict coord_column_s = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict coord_column_t = FLOAT_REG_PTR(x->children_[1], 1);

  uint32_t row;
  struct sampler_2d *samplers;
  samplers = split_execution_chains_to_sampler_tex_chains(exec, exec_chain, SAMPLER_2D_REG_PTR(x->children_[0], 0));

  struct sampler_2d *s2d;
  s2d = samplers;
  if (s2d) {
    do {
      s2d = s2d->runtime_active_sampler_chain_;

      uint8_t *restrict tex_chain_column = s2d->tex_exec_;

      /* terminate each sampler's row chain (strictly speaking we could check for arriving at last_row_ but
       * this makes it consistent with the rest of the code. */
      tex_chain_column[s2d->last_row_] = 0;
      
      /* Check if we need to find mipmaps.. */
      int locate_log2_mipmaps = 0;
      switch (s2d->min_filter_) {
        case   s2d_nearest:                
        case   s2d_linear:
          /* Switch-over between filtering minification and magnification requires distinguishing between
           * the two */
          locate_log2_mipmaps = s2d->mag_filter_ != s2d->min_filter_; break;
        case   s2d_nearest_mipmap_nearest: locate_log2_mipmaps = 1; break;
        case   s2d_nearest_mipmap_linear:  locate_log2_mipmaps = 1; break;
        case   s2d_linear_mipmap_nearest:  locate_log2_mipmaps = 1; break;
        case   s2d_linear_mipmap_linear:   locate_log2_mipmaps = 1; break;
      }

      if (locate_log2_mipmaps) {
        /* Fragment rows are ordered per square of 4 fragments, repeating 
         * every 4 fragments. Top-Left %00, Top-Right %01, Bottom-Left %10, Bottom-Right %11
         * Fragments outside the primitive (triangle) to be generated but inside a square
         * with at least one other fragment that is inside the triangle, *ARE* still generated
         * (a separate masking register is generated to ensure these fragments do not generate
         * output down the line.)
         * Because fragments are always generated in fours, we can reach across the rows using
         * the bit pattern above to find the differentials based on the appropriate neighbouring
         * pixels. */
        row = s2d->runtime_rows_;
        for (;;) {
          uint64_t chain;
          uint8_t delta;
          if (!(row & 7) && (((chain = *(uint64_t *)(tex_chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
            do {
              const float *restrict r_s = coord_column_s + row;
              const float *restrict r_t = coord_column_t + row;

              /* Top-Left dsdx = Top-Right dsdx = Top-Right S - Top-Left S
                * 01 45
                * 23 67 */
              float fdsdx01 = r_s[1] - r_s[0];
              /* Bottom-Left dsdx = Bottom-Right dsdx = Bottom-Right S - Bottom-Left S */
              float fdsdx23 = r_s[3] - r_s[2];

              /* Follow the same pattern for the next 4 fragments .. */
              float fdsdx45 = r_s[5] - r_s[4];
              float fdsdx67 = r_s[7] - r_s[6];

              /* Follow the same pattern for T */
              float fdtdx01 = r_t[1] - r_t[0];
              float fdtdx23 = r_t[3] - r_t[2];
              float fdtdx45 = r_t[5] - r_t[4];
              float fdtdx67 = r_t[7] - r_t[6];

              /* Follow the same pattern for S and T over dy
                * 01 45
                * 23 67 */
              float fdsdy02 = r_s[2] - r_s[0];
              float fdsdy13 = r_s[3] - r_s[1];
              float fdsdy46 = r_s[6] - r_s[4];
              float fdsdy57 = r_s[7] - r_s[5];

              float fdtdy02 = r_t[2] - r_t[0];
              float fdtdy13 = r_t[3] - r_t[1];
              float fdtdy46 = r_t[6] - r_t[4];
              float fdtdy57 = r_t[7] - r_t[5];

              float dstdx_squared_len_01 = fdsdx01 * fdsdx01 + fdtdx01 * fdtdx01;
              float dstdx_squared_len_23 = fdsdx23 * fdsdx23 + fdtdx23 * fdtdx23;
              float dstdx_squared_len_45 = fdsdx45 * fdsdx45 + fdtdx45 * fdtdx45;
              float dstdx_squared_len_67 = fdsdx67 * fdsdx67 + fdtdx67 * fdtdx67;

              float dstdy_squared_len_02 = fdsdy02 * fdsdy02 + fdtdy02 * fdtdy02;
              float dstdy_squared_len_13 = fdsdy13 * fdsdy13 + fdtdy13 * fdtdy13;
              float dstdy_squared_len_46 = fdsdy46 * fdsdy46 + fdtdy46 * fdtdy46;
              float dstdy_squared_len_57 = fdsdy57 * fdsdy57 + fdtdy57 * fdtdy57;

#define S2D_DMS_MAX(a,b) (((a)>=(b)) ? (a) : (b))
              float dmax_squared_len_0 = S2D_DMS_MAX(dstdx_squared_len_01, dstdy_squared_len_02);
              float dmax_squared_len_1 = S2D_DMS_MAX(dstdx_squared_len_01, dstdy_squared_len_13);
              float dmax_squared_len_2 = S2D_DMS_MAX(dstdx_squared_len_23, dstdy_squared_len_02);
              float dmax_squared_len_3 = S2D_DMS_MAX(dstdx_squared_len_23, dstdy_squared_len_13);
              float dmax_squared_len_4 = S2D_DMS_MAX(dstdx_squared_len_45, dstdy_squared_len_46);
              float dmax_squared_len_5 = S2D_DMS_MAX(dstdx_squared_len_45, dstdy_squared_len_57);
              float dmax_squared_len_6 = S2D_DMS_MAX(dstdx_squared_len_67, dstdy_squared_len_46);
              float dmax_squared_len_7 = S2D_DMS_MAX(dstdx_squared_len_67, dstdy_squared_len_57);
#undef S2D_DMS_MAX

              /* Divide the log2 by 2 so we effectively square-root the 
               * dmax_squared prior to taking the log2. (e.g. log2(sqrt()) == 0.5 * log2()) */
              float l2[] = {
                log2f(dmax_squared_len_0) * 0.5f,
                log2f(dmax_squared_len_1) * 0.5f,
                log2f(dmax_squared_len_2) * 0.5f,
                log2f(dmax_squared_len_3) * 0.5f,
                log2f(dmax_squared_len_4) * 0.5f,
                log2f(dmax_squared_len_5) * 0.5f,
                log2f(dmax_squared_len_6) * 0.5f,
                log2f(dmax_squared_len_7) * 0.5f
              };

              int frag_idx;
              for (frag_idx = 0; frag_idx < (sizeof(l2)/sizeof(*l2)); ++frag_idx) {
                float rgba[4];
                texture2D(rgba, s2d, r_s[frag_idx], r_t[frag_idx], l2[frag_idx]);
                red_column[row + frag_idx] = rgba[0];
                green_column[row + frag_idx] = rgba[1];
                blue_column[row + frag_idx] = rgba[2];
                alpha_column[row + frag_idx] = rgba[3];
              }

              delta = (chain & 0xFF00000000000000) >> 56;

              if (!delta) break;
              row += 7 + delta;
            } while (!(row & 7) && (((chain = *(uint64_t *)(tex_chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
          }
          else if (!(row & 3) && (((chain = *(uint32_t *)(tex_chain_column + row)) & 0xFFFFFF) == 0x010101)) {
            do {
              const float *restrict r_s = coord_column_s + row;
              const float *restrict r_t = coord_column_t + row;

              /* Top-Left dsdx = Top-Right dsdx = Top-Right S - Top-Left S
               * 01 
               * 23 */
              float fdsdx01 = r_s[1] - r_s[0];
              /* Bottom-Left dsdx = Bottom-Right dsdx = Bottom-Right S - Bottom-Left S */
              float fdsdx23 = r_s[3] - r_s[2];

              /* Follow the same pattern for T */
              float fdtdx01 = r_t[1] - r_t[0];
              float fdtdx23 = r_t[3] - r_t[2];

              /* Follow the same pattern for S and T over dy
               * 01 
               * 23 */
              float fdsdy02 = r_s[2] - r_s[0];
              float fdsdy13 = r_s[3] - r_s[1];

              float fdtdy02 = r_t[2] - r_t[0];
              float fdtdy13 = r_t[3] - r_t[1];

              float dstdx_squared_len_01 = fdsdx01 * fdsdx01 + fdtdx01 * fdtdx01;
              float dstdx_squared_len_23 = fdsdx23 * fdsdx23 + fdtdx23 * fdtdx23;

              float dstdy_squared_len_02 = fdsdy02 * fdsdy02 + fdtdy02 * fdtdy02;
              float dstdy_squared_len_13 = fdsdy13 * fdsdy13 + fdtdy13 * fdtdy13;

#define S2D_DMS_MAX(a,b) (((a)>=(b)) ? (a) : (b))
              float dmax_squared_len_0 = S2D_DMS_MAX(dstdx_squared_len_01, dstdy_squared_len_02);
              float dmax_squared_len_1 = S2D_DMS_MAX(dstdx_squared_len_01, dstdy_squared_len_13);
              float dmax_squared_len_2 = S2D_DMS_MAX(dstdx_squared_len_23, dstdy_squared_len_02);
              float dmax_squared_len_3 = S2D_DMS_MAX(dstdx_squared_len_23, dstdy_squared_len_13);
#undef S2D_DMS_MAX

              /* Divide the log2 by 2 so we effectively square-root the
               * dmax_squared prior to taking the log2. (e.g. log2(sqrt()) == 0.5 * log2()) */
              float l2[] = {
                log2f(dmax_squared_len_0) * 0.5f,
                log2f(dmax_squared_len_1) * 0.5f,
                log2f(dmax_squared_len_2) * 0.5f,
                log2f(dmax_squared_len_3) * 0.5f,
              };

              int frag_idx;
              for (frag_idx = 0; frag_idx < (sizeof(l2)/sizeof(*l2)); ++frag_idx) {
                float rgba[4];
                texture2D(rgba, s2d, r_s[frag_idx], r_t[frag_idx], l2[frag_idx]);
                red_column[row + frag_idx] = rgba[0];
                green_column[row + frag_idx] = rgba[1];
                blue_column[row + frag_idx] = rgba[2];
                alpha_column[row + frag_idx] = rgba[3];
              }

              delta = (chain & 0xFF000000) >> 24;
              if (!delta) break;
              row += 3 + delta;
            } while (!(row & 3) && ((chain = (*(uint32_t *)(tex_chain_column + row)) & 0xFFFFFF) == 0x010101));
          }
          else {
            do {
              /* Stubbornly persist in reaching across rows, even if the square of fragmens is not
               * in the same execution chain (e.g. there is some stippling or such going on in the
               * fragment shader) - so even if that's the case, the interpolation of S and T is
               * still very likely a good basis for mip-mapping. Note that the GLSL spec is 
               * "undefined" as to what happens here; just try and do something reasonable. */
              const float *restrict r_s = coord_column_s + row;
              const float *restrict r_t = coord_column_t + row;
              float fdsdx = r_s[row | 1] - r_s[row & ~1];
              float fdtdx = r_t[row | 1] - r_t[row & ~1];
              float fdsdy = r_s[row | 2] - r_s[row & ~2];
              float fdtdy = r_t[row | 2] - r_t[row & ~2];
              float fdstdx_squared_len = fdsdx * fdsdx + fdtdx * fdtdx;
              float fdstdy_squared_len = fdsdy * fdsdy + fdtdy * fdtdy;
              float dmax_squared_len = (fdstdx_squared_len > fdstdy_squared_len) ? fdstdx_squared_len : fdstdy_squared_len;
              float lg2 = log2f(dmax_squared_len) * 0.5f;

              float rgba[4];
              texture2D(rgba, s2d, r_s[0], r_t[0], lg2);
              red_column[row] = rgba[0];
              green_column[row] = rgba[1];
              blue_column[row] = rgba[2];
              alpha_column[row] = rgba[3];

              delta = tex_chain_column[row];
              if (!delta) break;
              row += delta;
            } while (row & 3);
          }
          if (!delta) break;
        }
      } /* end of if (locate_log2_mipmaps) */
      else /* if lg2 not needed */ {
        row = s2d->runtime_rows_;
        for (;;) {
          uint64_t chain;
          uint8_t delta;
          if (!(row & 7) && (((chain = *(uint64_t *)(tex_chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
            do {
              const float *restrict r_s = coord_column_s + row;
              const float *restrict r_t = coord_column_t + row;

              int frag_idx;
              for (frag_idx = 0; frag_idx < 8; ++frag_idx) {
                float rgba[4];
                texture2D(rgba, s2d, r_s[frag_idx], r_t[frag_idx], 0.f);
                red_column[row + frag_idx] = rgba[0];
                green_column[row + frag_idx] = rgba[1];
                blue_column[row + frag_idx] = rgba[2];
                alpha_column[row + frag_idx] = rgba[3];
              }

              delta = (chain & 0xFF00000000000000) >> 56;

              if (!delta) break;
              row += 7 + delta;
            } while (!(row & 7) && (((chain = *(uint64_t *)(tex_chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
          }
          else if (!(row & 3) && (((chain = *(uint32_t *)(tex_chain_column + row)) & 0xFFFFFF) == 0x010101)) {
            do {
              const float *restrict r_s = coord_column_s + row;
              const float *restrict r_t = coord_column_t + row;

              int frag_idx;
              for (frag_idx = 0; frag_idx < 4; ++frag_idx) {
                float rgba[4];
                texture2D(rgba, s2d, r_s[frag_idx], r_t[frag_idx], 0.f);
                red_column[row + frag_idx] = rgba[0];
                green_column[row + frag_idx] = rgba[1];
                blue_column[row + frag_idx] = rgba[2];
                alpha_column[row + frag_idx] = rgba[3];
              }

              delta = (chain & 0xFF000000) >> 24;
              if (!delta) break;
              row += 3 + delta;
            } while (!(row & 3) && ((chain = (*(uint32_t *)(tex_chain_column + row)) & 0xFFFFFF) == 0x010101));
          }
          else {
            do {
              /* Stubbornly persist in reaching across rows, even if the square of fragmens is not
               * in the same execution chain (e.g. there is some stippling or such going on in the
               * fragment shader) - so even if that's the case, the interpolation of S and T is
               * still very likely a good basis for mip-mapping. Note that the GLSL spec is
               * "undefined" as to what happens here; just try and do something reasonable. */
              const float *restrict r_s = coord_column_s + row;
              const float *restrict r_t = coord_column_t + row;

              float rgba[4];
              texture2D(rgba, s2d, r_s[0], r_t[0], 0.f);
              red_column[row] = rgba[0];
              green_column[row] = rgba[1];
              blue_column[row] = rgba[2];
              alpha_column[row] = rgba[3];

              delta = tex_chain_column[row];
              if (!delta) break;
              row += delta;
            } while (row & 3);
          }
          if (!delta) break;
        }
      }

      s2d->runtime_rows_ = SL_EXEC_NO_CHAIN;
      s2d->last_row_ = SL_EXEC_NO_CHAIN;
    } while (s2d != samplers);
  }
}

void builtin_texture2D_bias_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
}

void builtin_texture2DProj_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
}

void builtin_texture2DProj_v3_bias_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
}

void builtin_texture2DProj_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
}

void builtin_texture2DProj_v4_bias_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
}

void builtin_texture2DLod_v2_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
}

void builtin_texture2DProjLod_v3_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
}

void builtin_texture2DProjLod_v4_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
}

int sampler_2d_set_storage(struct sampler_2d *s2d, int level, enum s2d_tex_components internal_format, int width, int height) {
  if (s2d->num_maps_ <= level) {
    /* Resize to fit level, within reason for level */
    if (level >= 32) return SL_ERR_INVALID_ARG;
    struct sampler_2d_map *s2dm = (struct sampler_2d_map *)realloc(s2d->mipmaps_, (level + 1) * sizeof(struct sampler_2d_map));
    if (!s2dm) return SL_ERR_NO_MEM;
    size_t n;
    for (n = s2d->num_maps_; n <= (size_t)level; ++n) {
      s2dm->width_ = 0;
      s2dm->height_ = 0;
      s2dm->repeat_mask_s_ = s2dm->repeat_mask_t_ = 0;
      s2dm->components_ = s2d_rgb;
      s2dm->num_bytes_per_bitmap_row_ = 0;
      s2dm->bitmap_ = NULL;
    }
    s2d->mipmaps_ = s2dm;
    s2d->num_maps_ = level + 1;
  }

  struct sampler_2d_map *lvl = s2d->mipmaps_ + level;
  size_t num_bytes_per_pixel = 0;
  switch (internal_format) {
    case s2d_alpha:
    case s2d_luminance:
      num_bytes_per_pixel = 1;
      break;
    case s2d_luminance_alpha:
      num_bytes_per_pixel = 2;
      break;
    case s2d_rgb:
      num_bytes_per_pixel = 3;
      break;
    case s2d_rgba:
      num_bytes_per_pixel = 4;
      break;
  }
  size_t num_bytes_per_row_nonaligned = num_bytes_per_pixel * width;
  size_t num_bytes_per_row_8B_aligned = (num_bytes_per_row_nonaligned + 7) & ~(size_t)7;
  void *new_bitmap = malloc(num_bytes_per_row_8B_aligned * (size_t)height);
  if (!new_bitmap) return SL_ERR_NO_MEM;
  lvl->bitmap_ = new_bitmap;
  lvl->components_ = internal_format;
  lvl->width_ = width;
  lvl->height_ = height;
  lvl->num_bytes_per_bitmap_row_ = num_bytes_per_row_8B_aligned;
  lvl->repeat_mask_s_ = isolate_msb((uint32_t)width) - 1;
  lvl->repeat_mask_t_ = isolate_msb((uint32_t)height) - 1;

  return SL_ERR_OK;
}

int sampler_2d_set_image(struct sampler_2d *s2d, int level, enum s2d_tex_components internal_format, int width, int height,
                         enum blitter_data_type src_datatype, void *src_data) {
  int r;
  r = sampler_2d_set_storage(s2d, level, internal_format, width, height);
  if (r) return r;

  struct sampler_2d_map *lvl = s2d->mipmaps_ + level;
  
  size_t num_bytes_per_src_pixel = 0;
  switch (src_datatype) {
    case blit_unsigned_byte:
      switch (lvl->components_) {
        case s2d_alpha:
        case s2d_luminance:
          num_bytes_per_src_pixel = 1;
          break;
        case s2d_luminance_alpha:
          num_bytes_per_src_pixel = 2;
          break;
        case s2d_rgb:
          num_bytes_per_src_pixel = 3;
          break;
        case s2d_rgba:
          num_bytes_per_src_pixel = 4;
          break;
      }
      break;
    case blit_unsigned_short_565:
    case blit_unsigned_short_4444:
    case blit_unsigned_short_5551:
      num_bytes_per_src_pixel = 2;
      break;
  }
  size_t num_bytes_per_src_row = num_bytes_per_src_pixel * width;

  blitter_blit(lvl->bitmap_, src_data, lvl->num_bytes_per_bitmap_row_, num_bytes_per_src_row, 
               (size_t)width, (size_t)height, lvl->components_, src_datatype);

  sampler_2d_update_completeness(s2d);
  sampler_2d_update_level_repetition_masks(s2d);

  return SL_ERR_OK;
}

int sampler_2d_generate_mipmaps(struct sampler_2d *s2d) {
  if (s2d->num_maps_ == 0) {
    return SL_ERR_INVALID_ARG;
  }
  /* Reset the number of mipmaps to 1 (keep the level 0 mipmap),
   * and then generate each as we go */
  size_t n;
  for (n = 1; n < s2d->num_maps_; ++n) {
    struct sampler_2d_map *s2dm = s2d->mipmaps_ + n;
    free(s2dm->bitmap_);
  }
  s2d->num_maps_ = 1;

  int lvl_width, lvl_height;
  lvl_width = s2d->mipmaps_[0].width_;
  lvl_height = s2d->mipmaps_[0].height_;
  int level = 1;
  int r;
  while ((lvl_width > 1) || (lvl_height > 1)) {
    lvl_width = lvl_width / 1;
    if (lvl_width == 0) lvl_width = 1;
    lvl_height = lvl_height / 2;
    if (lvl_height == 0) lvl_height = 1;

    r = sampler_2d_set_storage(s2d, level, s2d->mipmaps_[level-1].components_, lvl_width, lvl_height);
    if (r) return r;

    size_t bytes_per_pixel = 0;
    switch (s2d->mipmaps_[level].components_) {
      case s2d_alpha:
      case s2d_luminance:
        bytes_per_pixel = 1;
        break;
      case s2d_luminance_alpha:
        bytes_per_pixel = 2;
        break;
      case s2d_rgb:
        bytes_per_pixel = 3;
        break;
      case s2d_rgba:
        bytes_per_pixel = 4;
        break;
    }

    size_t child_stride = s2d->mipmaps_[level].num_bytes_per_bitmap_row_;
    size_t parent_stride = s2d->mipmaps_[level-1].num_bytes_per_bitmap_row_;
    int row, col;
    uint8_t * restrict child_ptr;
    uint8_t * restrict parent_ptr;
    uint8_t * restrict child_row_ptr;
    uint8_t * restrict parent_row_ptr;
    child_row_ptr = (uint8_t * restrict)s2d->mipmaps_[level].bitmap_;
    parent_row_ptr = (uint8_t * restrict)s2d->mipmaps_[level - 1].bitmap_;
    int is_vertically_flat = s2d->mipmaps_[level-1].height_ == 1;
    int is_horizontally_flat = s2d->mipmaps_[level-1].width_ == 1;
    for (row = 0; row < lvl_width; ++row) {
      child_ptr = child_row_ptr;
      parent_ptr = parent_row_ptr;
      child_row_ptr += child_stride;
      parent_row_ptr += parent_stride + parent_stride;
      for (col = 0; col < lvl_width; ++col) {
        uint16_t val;
        size_t k;
        for (k = 0; k < bytes_per_pixel; ++k) {
          if (!is_vertically_flat) {
            if (!is_horizontally_flat) {
              val = parent_ptr[0];
              val += parent_ptr[bytes_per_pixel];
              val += parent_ptr[parent_stride];
              val += parent_ptr[parent_stride + bytes_per_pixel];
              val /= 4;
            }
            else {
              val = parent_ptr[0];
              val += parent_ptr[parent_stride];
              val /= 2;
            }
          }
          else {
            if (!is_horizontally_flat) {
              val = parent_ptr[0];
              val += parent_ptr[bytes_per_pixel];
              val /= 2;
            }
            else {
              val = parent_ptr[0];
            }
          }
          *child_ptr++ = (uint8_t)val;
          parent_ptr++;
        }
        parent_ptr += bytes_per_pixel;
      }
    }
  }
  return SL_ERR_OK;
}

