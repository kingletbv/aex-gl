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

/* XXX: For now we grab the sampler_2d from the first argument (the sampler) - this should
 *      however become an integer type and not a void* pointer type - so this will have to
 *      change. */

void builtin_texture2D_runtime(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  uint8_t *restrict chain_column = exec->exec_chain_reg_;
  float *restrict result_column = FLOAT_REG_PTR(x, 0);
  void *restrict *restrict sampler_column = SAMPLER_2D_REG_PTR(x->children_[0], 0);
  float *restrict coord_column_s = FLOAT_REG_PTR(x->children_[1], 0);
  float *restrict coord_column_t = FLOAT_REG_PTR(x->children_[1], 1);

  // XXX: Should split this by different sampler type (!!)

  uint8_t row = exec_chain;
  struct sampler_2d *samplers = NULL;

  /* Split the rows to each sampler, we want to evaluate all rows such that the sampler is held
   * constant. */
  uint8_t delta;
  do {
    delta = chain_column[row];

    struct sampler_2d *s2d = sampler_column[row];
    if (s2d->last_row_ != SL_EXEC_NO_CHAIN) {
      chain_column[s2d->last_row_] = row - s2d->last_row_;
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

  struct sampler_2d *s2d;
  s2d = samplers;
  if (s2d) {
    do {
      s2d = s2d->runtime_active_sampler_chain_;

      /* terminate each sampler's row chain (strictly speaking we could check for arriving at last_row_ but
       * this makes it consistent with the rest of the code. */
      chain_column[s2d->last_row_] = 0;

      /* Wrap S coordinate according to sampler setting */
      switch (s2d->wrap_s_) {
        case s2d_clamp_to_edge: {
          float min_fs = 1 / (2.f * (float)(s2d->width_));
          float max_fs = 1.f - min_fs;
          float * restrict result_column = s2d->ranged_s_;
          float * restrict opd_column = coord_column_s;
#define UNOP_SNIPPET_OPERATOR(opd) ((opd < min_fs) ? min_fs : ((opd > max_fs) ? max_fs : opd))
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
          break;
        }
        case s2d_repeat: {
          float * restrict result_column = s2d->ranged_s_;
          float * restrict opd_column = coord_column_s;
#define UNOP_SNIPPET_OPERATOR(opd) (opd - floorf(opd))
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
          break;
        }
        case s2d_mirrored_repeat: {
          /* Check if integral part of number is even or odd */ 
          float * restrict result_column = s2d->ranged_s_;
          float * restrict opd_column = coord_column_s;
#define UNOP_SNIPPET_OPERATOR(opd) ((1 & (int64_t)floorf(opd)) ? (1.f - (opd - floorf(opd))) : (opd - floorf(opd)))
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE

          break;
        }
      }

      /* Wrap T coordinate according to sampler setting */
      switch (s2d->wrap_t_) {
        case s2d_clamp_to_edge: {
          float min_ft = 1 / (2.f * (float)(s2d->width_));
          float max_ft = 1.f - min_ft;
          float * restrict result_column = s2d->ranged_t_;
          float * restrict opd_column = coord_column_t;
#define UNOP_SNIPPET_OPERATOR(opd) ((opd < min_ft) ? min_ft : ((opd > max_ft) ? max_ft : opd))
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
          break;
        }
        case s2d_repeat: {
          float * restrict result_column = s2d->ranged_t_;
          float * restrict opd_column = coord_column_t;
#define UNOP_SNIPPET_OPERATOR(opd) (opd - floorf(opd))
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
          break;
        }
        case s2d_mirrored_repeat: {
          /* Check if integral part of number is even or odd */ 
          float * restrict result_column = s2d->ranged_t_;
          float * restrict opd_column = coord_column_t;
#define UNOP_SNIPPET_OPERATOR(opd) ((1 & (int64_t)floorf(opd)) ? (1.f - (opd - floorf(opd))) : (opd - floorf(opd)))
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE

          break;
        }
      }

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

      uint32_t minification_chain = SL_EXEC_NO_CHAIN;
      uint32_t minification_chain_tail = SL_EXEC_NO_CHAIN;
      uint32_t magnification_chain = SL_EXEC_NO_CHAIN;
      uint32_t magnification_chain_tail = SL_EXEC_NO_CHAIN;

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
        float * restrict result_column = s2d->ds_dx_;
        float * restrict opd_column = coord_column_s;
        float * restrict log2_column = s2d->dst_log2_;

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

        for (;;) {
          uint64_t chain;
          uint8_t delta;
          if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
            do {
              float * restrict l2 = log2_column + row;
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

              /* Write out results, divide the log2 by 2 so we effectively square-root the 
               * dmax_squared prior to taking the log2. (e.g. log2(sqrt()) == 0.5 * log2()) */
              float l2_0 = log2f(dmax_squared_len_0) * 0.5f;
              float l2_1 = log2f(dmax_squared_len_1) * 0.5f;
              float l2_2 = log2f(dmax_squared_len_2) * 0.5f;
              float l2_3 = log2f(dmax_squared_len_3) * 0.5f;
              float l2_4 = log2f(dmax_squared_len_4) * 0.5f;
              float l2_5 = log2f(dmax_squared_len_5) * 0.5f;
              float l2_6 = log2f(dmax_squared_len_6) * 0.5f;
              float l2_7 = log2f(dmax_squared_len_7) * 0.5f;

              l2[0] = l2_0;
              l2[1] = l2_1;
              l2[2] = l2_2;
              l2[3] = l2_3;
              l2[4] = l2_4;
              l2[5] = l2_5;
              l2[6] = l2_6;
              l2[7] = l2_7;

              delta = (chain & 0xFF00000000000000) >> 56;

              /* Dispatch between minification and magnification
               * Attempt here is to keep it within integer bitwise logic, perhaps
               * even parallelising, and avoid the compiler from going down a 
               * logical branching path. */
              int magnification = ((l2_0 > c) ? 0: 1)
                                + ((l2_1 > c) ? 0: 2)
                                + ((l2_2 > c) ? 0: 4)
                                + ((l2_3 > c) ? 0: 8)
                                + ((l2_4 > c) ? 0: 16)
                                + ((l2_5 > c) ? 0: 32)
                                + ((l2_6 > c) ? 0: 64)
                                + ((l2_7 > c) ? 0: 128);
              if (magnification == 0xFF) {
                /* All magnify */
                if (magnification_chain_tail != SL_EXEC_NO_CHAIN) {
                  chain_column[magnification_chain_tail] = row - magnification_chain_tail;
                }
                else {
                  magnification_chain = row;
                }
                magnification_chain_tail = row + 7;
              }
              else if (!magnification) {
                /* All minify */
                if (minification_chain_tail != SL_EXEC_NO_CHAIN) {
                  chain_column[minification_chain_tail] = row - minification_chain_tail;
                }
                else {
                  minification_chain = row;
                }
                minification_chain_tail = row + 7;
              }
              else {
                /* A tragic mix / cross-over of magnification and minification; slowly split the two one by one */
                int frag_idx;
                for (frag_idx = 0; frag_idx < 8; frag_idx++) {
                  if (magnification & 1) {
                    if (magnification_chain_tail != SL_EXEC_NO_CHAIN) {
                      chain_column[magnification_chain_tail] = row + frag_idx - magnification_chain_tail;
                    }
                    else {
                      magnification_chain = row + frag_idx;
                    }
                    magnification_chain_tail = row + frag_idx;
                  }
                  else {
                    if (minification_chain_tail != SL_EXEC_NO_CHAIN) {
                      chain_column[minification_chain_tail] = row + frag_idx - minification_chain_tail;
                    }
                    else {
                      minification_chain = row + frag_idx;
                    }
                    minification_chain_tail = row + frag_idx;
                  }
                }
              }

              if (!delta) break;
              row += 7 + delta;
            } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
          }
          else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
            do {
              float *restrict l2 = log2_column + row;
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

              /* Write out results, divide the log2 by 2 so we effectively square-root the
               * dmax_squared prior to taking the log2. (e.g. log2(sqrt()) == 0.5 * log2()) */
              float l2_0 = log2f(dmax_squared_len_0) * 0.5f;
              float l2_1 = log2f(dmax_squared_len_1) * 0.5f;
              float l2_2 = log2f(dmax_squared_len_2) * 0.5f;
              float l2_3 = log2f(dmax_squared_len_3) * 0.5f;
              l2[0] = l2_0;
              l2[1] = l2_1;
              l2[2] = l2_2;
              l2[3] = l2_3;

              /* Dispatch between minification and magnification */
              int magnification = ((l2_0 > c) ? 0: 1)
                                + ((l2_1 > c) ? 0: 2)
                                + ((l2_2 > c) ? 0: 4)
                                + ((l2_3 > c) ? 0: 8);
              if (magnification == 0xF) {
                /* All magnify */
                if (magnification_chain_tail != SL_EXEC_NO_CHAIN) {
                  chain_column[magnification_chain_tail] = row - magnification_chain_tail;
                }
                else {
                  magnification_chain = row;
                }
                magnification_chain_tail = row + 3;
              }
              else if (!magnification) {
                /* All minify */
                if (minification_chain_tail != SL_EXEC_NO_CHAIN) {
                  chain_column[minification_chain_tail] = row - minification_chain_tail;
                }
                else {
                  minification_chain = row;
                }
                minification_chain_tail = row + 3;
              }
              else {
                /* A tragic mix / cross-over of magnification and minification; slowly split the two one by one */
                int frag_idx;
                for (frag_idx = 0; frag_idx < 4; frag_idx++) {
                  if (magnification & 1) {
                    if (magnification_chain_tail != SL_EXEC_NO_CHAIN) {
                      chain_column[magnification_chain_tail] = row + frag_idx - magnification_chain_tail;
                    }
                    else {
                      magnification_chain = row + frag_idx;
                    }
                    magnification_chain_tail = row + frag_idx;
                  }
                  else {
                    if (minification_chain_tail != SL_EXEC_NO_CHAIN) {
                      chain_column[minification_chain_tail] = row + frag_idx - minification_chain_tail;
                    }
                    else {
                      minification_chain = row + frag_idx;
                    }
                    minification_chain_tail = row + frag_idx;
                  }
                }
              }

              delta = (chain & 0xFF000000) >> 24;
              if (!delta) break;
              row += 3 + delta;
            } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
          }
          else {
            do {
              /* Stubbornly persist in reaching across rows, even if the square of fragmens is not
               * in the same execution chain (e.g. there is some stippling or such going on in the
               * fragment shader) - so even if that's the case, the interpolation of S and T is
               * still very likely a good basis for mip-mapping. Note that the GLSL spec is 
               * "undefined" as to what happens here; just try and do something reasonable. */
              float *restrict l2 = log2_column + row;
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

              *l2 = lg2;

              if (lg2 <= c) {
                /* Magnification */
                if (magnification_chain_tail != SL_EXEC_NO_CHAIN) {
                  chain_column[magnification_chain_tail] = row - magnification_chain_tail;
                }
                else {
                  magnification_chain = row;
                }
                magnification_chain_tail = row;
              }
              else {
                /* Minification */
                if (minification_chain_tail != SL_EXEC_NO_CHAIN) {
                  chain_column[minification_chain_tail] = row - minification_chain_tail;
                }
                else {
                  minification_chain = row;
                }
                minification_chain_tail = row;
              }

              delta = chain_column[row];
              if (!delta) break;
              row += delta;
            } while (row & 3);
          }
          if (!delta) break;
        }
      } /* end of if (locate_log2_mipmaps) */
      else {
        minification_chain = exec_chain;
      }

      if ((s2d->min_filter_ == s2d_nearest_mipmap_nearest) ||
          (s2d->min_filter_ == s2d_linear_mipmap_nearest)) {
        float max_q  = 0.5f + (float)s2d->q_;
        int q = s2d->q_;

        float * restrict opd_column = coord_column_s;
        float * restrict log2_column = s2d->dst_log2_;
        for (;;) {
          uint64_t chain;
          uint8_t delta;
          if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
            do {
              float * restrict l2 = log2_column + row;
              const float *restrict r_s = coord_column_s + row;
              const float *restrict r_t = coord_column_t + row;

              float l2_0 = l2[0];
              float l2_1 = l2[1];
              float l2_2 = l2[2];
              float l2_3 = l2[3];
              float l2_4 = l2[4];
              float l2_5 = l2[5];
              float l2_6 = l2[6];
              float l2_7 = l2[7];

              int nearest_d0 = (l2_0 <= 0.5f) ? 0 : (l2_0 > max_q) ? q : ((int)(ceilf(l2_0 + 0.5f)) - 1);
              int nearest_d1 = (l2_1 <= 0.5f) ? 0 : (l2_1 > max_q) ? q : ((int)(ceilf(l2_1 + 0.5f)) - 1);
              int nearest_d2 = (l2_2 <= 0.5f) ? 0 : (l2_2 > max_q) ? q : ((int)(ceilf(l2_2 + 0.5f)) - 1);
              int nearest_d3 = (l2_3 <= 0.5f) ? 0 : (l2_3 > max_q) ? q : ((int)(ceilf(l2_3 + 0.5f)) - 1);
              int nearest_d4 = (l2_4 <= 0.5f) ? 0 : (l2_4 > max_q) ? q : ((int)(ceilf(l2_4 + 0.5f)) - 1);
              int nearest_d5 = (l2_5 <= 0.5f) ? 0 : (l2_5 > max_q) ? q : ((int)(ceilf(l2_5 + 0.5f)) - 1);
              int nearest_d6 = (l2_6 <= 0.5f) ? 0 : (l2_6 > max_q) ? q : ((int)(ceilf(l2_6 + 0.5f)) - 1);
              int nearest_d7 = (l2_7 <= 0.5f) ? 0 : (l2_7 > max_q) ? q : ((int)(ceilf(l2_7 + 0.5f)) - 1);

              delta = (chain & 0xFF00000000000000) >> 56;
              if (!delta) break;
              row += 7 + delta;
            } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
          }
          else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
            do {
              float *restrict l2 = log2_column + row;
              const float *restrict r_s = coord_column_s + row;
              const float *restrict r_t = coord_column_t + row;

              float l2_0 = l2[0];
              float l2_1 = l2[1];
              float l2_2 = l2[2];
              float l2_3 = l2[3];

              int nearest_d0 = (l2_0 <= 0.5f) ? 0 : (l2_0 > max_q) ? q : ((int)(ceilf(l2_0 + 0.5f)) - 1);
              int nearest_d1 = (l2_1 <= 0.5f) ? 0 : (l2_1 > max_q) ? q : ((int)(ceilf(l2_1 + 0.5f)) - 1);
              int nearest_d2 = (l2_2 <= 0.5f) ? 0 : (l2_2 > max_q) ? q : ((int)(ceilf(l2_2 + 0.5f)) - 1);
              int nearest_d3 = (l2_3 <= 0.5f) ? 0 : (l2_3 > max_q) ? q : ((int)(ceilf(l2_3 + 0.5f)) - 1);

              delta = (chain & 0xFF000000) >> 24;
              if (!delta) break;
              row += 3 + delta;
            } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
          }
          else {
            do {
              float *restrict l2 = log2_column + row;
              const float *restrict r_s = coord_column_s + row;
              const float *restrict r_t = coord_column_t + row;

              float lg2 = l2[0];
              int nearest_d = (lg2 <= 0.5f) ? 0 : (lg2 > max_q) ? q : ((int)(ceilf(lg2 + 0.5f)) - 1);

              delta = chain_column[row];
              if (!delta) break;
              row += delta;
            } while (row & 3);
          }
          if (!delta) break;
        }
      } /* end of if (XXX_MIPMAP_NEAREST) */

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
