/* Copyright 2023-2024 Kinglet B.V.
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

#include <stdio.h>

#ifndef BLEND_H_INCLUDED
#define BLEND_H_INCLUDED
#include "blend.h"
#endif

void blend(size_t num_fragments, uint8_t *maskp, 
           uint8_t *rgba_src_red, uint8_t *rgba_src_green, uint8_t *rgba_src_blue, uint8_t *rgba_src_alpha,
           uint8_t **rgba_dstpp, 
           int enable_red, int enable_green, int enable_blue, int enable_alpha,
           blend_eq_t rgb_eq, blend_eq_t alpha_eq,
           blend_func_t src_rgb_fn, blend_func_t src_alpha_fn,
           blend_func_t dst_rgb_fn, blend_func_t dst_alpha_fn,
           uint8_t constant_red, uint8_t constant_grn, uint8_t constant_blu, uint8_t constant_alpha,
           uint8_t *scratch_src_wgh_redp, uint8_t *scratch_src_wgh_grnp, uint8_t *scratch_src_wgh_blup, uint8_t *scratch_src_wgh_alpp,
           uint8_t *scratch_dst_wgh_redp, uint8_t *scratch_dst_wgh_grnp, uint8_t *scratch_dst_wgh_blup, uint8_t *scratch_dst_wgh_alpp) {
  size_t frag;
  uint8_t *restrict maskpr = (uint8_t *restrict)maskp;
  uint8_t *restrict *restrict rgba_dstppr = (uint8_t *restrict *restrict) rgba_dstpp;

  uint8_t *restrict src_wgh_redp = (uint8_t *restrict)scratch_src_wgh_redp;
  uint8_t *restrict src_wgh_grnp = (uint8_t *restrict)scratch_src_wgh_grnp;
  uint8_t *restrict src_wgh_blup = (uint8_t *restrict)scratch_src_wgh_blup;
  uint8_t *restrict src_wgh_alpp = (uint8_t *restrict)scratch_src_wgh_alpp;

  uint8_t *restrict dst_wgh_redp = (uint8_t *restrict)scratch_dst_wgh_redp;
  uint8_t *restrict dst_wgh_grnp = (uint8_t *restrict)scratch_dst_wgh_grnp;
  uint8_t *restrict dst_wgh_blup = (uint8_t *restrict)scratch_dst_wgh_blup;
  uint8_t *restrict dst_wgh_alpp = (uint8_t *restrict)scratch_dst_wgh_alpp;

  uint8_t *restrict src_redp = (uint8_t *restrict)rgba_src_red;
  uint8_t *restrict src_grnp = (uint8_t *restrict)rgba_src_green;
  uint8_t *restrict src_blup = (uint8_t *restrict)rgba_src_blue;
  uint8_t *restrict src_alpp = (uint8_t *restrict)rgba_src_alpha;

  switch (src_rgb_fn) {
    case BF_ZERO:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = 0;
        src_wgh_grnp[frag] = 0;
        src_wgh_blup[frag] = 0;
      }
      break;
    case BF_ONE:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = 255;
        src_wgh_grnp[frag] = 255;
        src_wgh_blup[frag] = 255;
      }
      break;
    case BF_SRC_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = src_redp[frag];
        src_wgh_grnp[frag] = src_grnp[frag];
        src_wgh_blup[frag] = src_blup[frag];
      }
      break;
    case BF_ONE_MINUS_SRC_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = 255 - src_redp[frag];
        src_wgh_grnp[frag] = 255 - src_grnp[frag];
        src_wgh_blup[frag] = 255 - src_blup[frag];
      }
      break;
    case BF_DST_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = rgba_dstppr[frag][0];
        src_wgh_grnp[frag] = rgba_dstppr[frag][1];
        src_wgh_blup[frag] = rgba_dstppr[frag][2];
      }
      break;
    case BF_ONE_MINUS_DST_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = 255 - rgba_dstppr[frag][0];
        src_wgh_grnp[frag] = 255 - rgba_dstppr[frag][1];
        src_wgh_blup[frag] = 255 - rgba_dstppr[frag][2];
      }
      break;
    case BF_SRC_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = src_alpp[frag];
        src_wgh_grnp[frag] = src_alpp[frag];
        src_wgh_blup[frag] = src_alpp[frag];
      }
      break;
    case BF_ONE_MINUS_SRC_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = 255 - src_alpp[frag];
        src_wgh_grnp[frag] = 255 - src_alpp[frag];
        src_wgh_blup[frag] = 255 - src_alpp[frag];
      }
      break;
    case BF_DST_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = rgba_dstppr[frag][3];
        src_wgh_grnp[frag] = rgba_dstppr[frag][3];
        src_wgh_blup[frag] = rgba_dstppr[frag][3];
      }
      break;
    case BF_ONE_MINUS_DST_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = 255 - rgba_dstppr[frag][3];
        src_wgh_grnp[frag] = 255 - rgba_dstppr[frag][3];
        src_wgh_blup[frag] = 255 - rgba_dstppr[frag][3];
      }
      break;
    case BF_CONSTANT_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = constant_red;
        src_wgh_grnp[frag] = constant_grn;
        src_wgh_blup[frag] = constant_blu;
      }
      break;
    case BF_ONE_MINUS_CONSTANT_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = 255 - constant_red;
        src_wgh_grnp[frag] = 255 - constant_grn;
        src_wgh_blup[frag] = 255 - constant_blu;
      }
      break;
    case BF_CONSTANT_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = constant_alpha;
        src_wgh_grnp[frag] = constant_alpha;
        src_wgh_blup[frag] = constant_alpha;
      }
      break;
    case BF_ONE_MINUS_CONSTANT_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = 255 - constant_alpha;
        src_wgh_grnp[frag] = 255 - constant_alpha;
        src_wgh_blup[frag] = 255 - constant_alpha;
      }
      break;
    case BF_SRC_ALPHA_SATURATE:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = 255 - rgba_dstppr[frag][3];
        src_wgh_redp[frag] = (src_wgh_redp[frag] < src_alpp[frag]) ? src_wgh_redp[frag] : src_alpp[frag];
        src_wgh_blup[frag] = src_wgh_grnp[frag] = src_wgh_redp[frag];
      }
      break;
  }

  switch (src_alpha_fn) {
    case BF_ZERO:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = 0;
      }
      break;
    case BF_ONE:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = 255;
      }
      break;
    case BF_SRC_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = src_alpp[frag];
      }
      break;
    case BF_ONE_MINUS_SRC_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = 255 - src_alpp[frag];
      }
      break;
    case BF_DST_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = rgba_dstppr[frag][3];
      }
      break;
    case BF_ONE_MINUS_DST_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = 255 - rgba_dstppr[frag][3];
      }
      break;
    case BF_SRC_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = src_alpp[frag];
      }
      break;
    case BF_ONE_MINUS_SRC_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = 255 - src_alpp[frag];
      }
      break;
    case BF_DST_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = rgba_dstppr[frag][3];
      }
      break;
    case BF_ONE_MINUS_DST_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = 255 - rgba_dstppr[frag][3];
      }
      break;
    case BF_CONSTANT_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = constant_alpha;
      }
      break;
    case BF_ONE_MINUS_CONSTANT_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = 255 - constant_alpha;
      }
      break;
    case BF_CONSTANT_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = constant_alpha;
      }
      break;
    case BF_ONE_MINUS_CONSTANT_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = 255 - constant_alpha;
      }
      break;
    case BF_SRC_ALPHA_SATURATE:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = 255;
      }
      break;
  }

  switch (dst_rgb_fn) {
    case BF_ZERO:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = 0;
        dst_wgh_grnp[frag] = 0;
        dst_wgh_blup[frag] = 0;
      }
      break;
    case BF_ONE:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = 255;
        dst_wgh_grnp[frag] = 255;
        dst_wgh_blup[frag] = 255;
      }
      break;
    case BF_SRC_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = src_redp[frag];
        dst_wgh_grnp[frag] = src_grnp[frag];
        dst_wgh_blup[frag] = src_blup[frag];
      }
      break;
    case BF_ONE_MINUS_SRC_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = 255 - src_redp[frag];
        dst_wgh_grnp[frag] = 255 - src_grnp[frag];
        dst_wgh_blup[frag] = 255 - src_blup[frag];
      }
      break;
    case BF_DST_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = rgba_dstppr[frag][0];
        dst_wgh_grnp[frag] = rgba_dstppr[frag][1];
        dst_wgh_blup[frag] = rgba_dstppr[frag][0];
      }
      break;
    case BF_ONE_MINUS_DST_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = 255 - rgba_dstppr[frag][0];
        dst_wgh_grnp[frag] = 255 - rgba_dstppr[frag][1];
        dst_wgh_blup[frag] = 255 - rgba_dstppr[frag][2];
      }
      break;
    case BF_SRC_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = src_alpp[frag];
        dst_wgh_grnp[frag] = src_alpp[frag];
        dst_wgh_blup[frag] = src_alpp[frag];
      }
      break;
    case BF_ONE_MINUS_SRC_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = 255 - src_alpp[frag];
        dst_wgh_grnp[frag] = 255 - src_alpp[frag];
        dst_wgh_blup[frag] = 255 - src_alpp[frag];
      }
      break;
    case BF_DST_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = rgba_dstppr[frag][3];
        dst_wgh_grnp[frag] = rgba_dstppr[frag][3];
        dst_wgh_blup[frag] = rgba_dstppr[frag][3];
      }
      break;
    case BF_ONE_MINUS_DST_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = 255 - rgba_dstppr[frag][3];
        dst_wgh_grnp[frag] = 255 - rgba_dstppr[frag][3];
        dst_wgh_blup[frag] = 255 - rgba_dstppr[frag][3];
      }
      break;
    case BF_CONSTANT_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = constant_red;
        dst_wgh_grnp[frag] = constant_grn;
        dst_wgh_blup[frag] = constant_blu;
      }
      break;
    case BF_ONE_MINUS_CONSTANT_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = 255 - constant_red;
        dst_wgh_grnp[frag] = 255 - constant_grn;
        dst_wgh_blup[frag] = 255 - constant_blu;
      }
      break;
    case BF_CONSTANT_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = constant_alpha;
        dst_wgh_grnp[frag] = constant_alpha;
        dst_wgh_blup[frag] = constant_alpha;
      }
      break;
    case BF_ONE_MINUS_CONSTANT_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = 255 - constant_alpha;
        dst_wgh_grnp[frag] = 255 - constant_alpha;
        dst_wgh_blup[frag] = 255 - constant_alpha;
      }
      break;
    case BF_SRC_ALPHA_SATURATE:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = 255 - rgba_dstppr[frag][3];
        dst_wgh_redp[frag] = (dst_wgh_redp[frag] < src_alpp[frag]) ? dst_wgh_redp[frag] : src_alpp[frag];
        dst_wgh_blup[frag] = dst_wgh_grnp[frag] = dst_wgh_redp[frag];
      }
      break;
  }

  switch (dst_alpha_fn) {
    case BF_ZERO:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = 0;
      }
      break;
    case BF_ONE:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = 255;
      }
      break;
    case BF_SRC_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = src_alpp[frag];
      }
      break;
    case BF_ONE_MINUS_SRC_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = 255 - src_alpp[frag];
      }
      break;
    case BF_DST_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = rgba_dstppr[frag][3];
      }
      break;
    case BF_ONE_MINUS_DST_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = 255 - rgba_dstppr[frag][3];
      }
      break;
    case BF_SRC_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = src_alpp[frag];
      }
      break;
    case BF_ONE_MINUS_SRC_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = 255 - src_alpp[frag];
      }
      break;
    case BF_DST_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = rgba_dstppr[frag][3];
      }
      break;
    case BF_ONE_MINUS_DST_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = 255 - rgba_dstppr[frag][3];
      }
      break;
    case BF_CONSTANT_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = constant_alpha;
      }
      break;
    case BF_ONE_MINUS_CONSTANT_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = 255 - constant_alpha;
      }
      break;
    case BF_CONSTANT_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = constant_alpha;
      }
      break;
    case BF_ONE_MINUS_CONSTANT_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = 255 - constant_alpha;
      }
      break;
    case BF_SRC_ALPHA_SATURATE:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = 255;
      }
      break;
  }

  switch (rgb_eq) {
    case BEQ_FUNC_ADD: {
      if (enable_red) {
        for (frag = 0; frag < num_fragments; ++frag) {
          uint32_t ired = ((uint32_t)src_redp[frag]) * src_wgh_redp[frag] + ((uint32_t)rgba_dstppr[frag][0]) * dst_wgh_redp[frag];
          ired = (ired + 1 + (ired >> 8)) >> 8;
          /* Saturation */
          ired = (ired > 255) ? 255 : ired;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][0] = (((uint8_t)ired) & mask) | (rgba_dstppr[frag][0] & ~mask);
        }
      }

      if (enable_green) {
        for (frag = 0; frag < num_fragments; ++frag) {
          uint32_t igrn = ((uint32_t)src_grnp[frag]) * src_wgh_grnp[frag] + ((uint32_t)rgba_dstppr[frag][1]) * dst_wgh_grnp[frag];
          igrn = (igrn + 1 + (igrn >> 8)) >> 8;
          /* Saturation */
          igrn = (igrn > 255) ? 255 : igrn;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][1] = (((uint8_t)igrn) & mask) | (rgba_dstppr[frag][1] & ~mask);
        }
      }

      if (enable_blue) {
        for (frag = 0; frag < num_fragments; ++frag) {
          uint32_t iblu = ((uint32_t)src_blup[frag]) * src_wgh_blup[frag] + ((uint32_t)rgba_dstppr[frag][2]) * dst_wgh_blup[frag];
          iblu = (iblu + 1 + (iblu >> 8)) >> 8;
          /* Saturation */
          iblu = (iblu > 255) ? 255 : iblu;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][2] = (((uint8_t)iblu) & mask) | (rgba_dstppr[frag][2] & ~mask);
        }
      }
      break;
    }
    case BEQ_FUNC_SUBTRACT: {
      if (enable_red) {
        for (frag = 0; frag < num_fragments; ++frag) {
          int32_t ired = ((int32_t)src_redp[frag]) * src_wgh_redp[frag] - ((int32_t)rgba_dstppr[frag][0]) * dst_wgh_redp[frag];
          if (ired < 0) ired = 0;
          ired = (ired + 1 + (ired >> 8)) >> 8;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][0] = (((uint8_t)ired) & mask) | (src_redp[frag] & ~mask);
        }
      }

      if (enable_green) {
        for (frag = 0; frag < num_fragments; ++frag) {
          int32_t igrn = ((int32_t)src_grnp[frag]) * src_wgh_grnp[frag] - ((int32_t)rgba_dstppr[frag][1]) * dst_wgh_grnp[frag];
          if (igrn < 0) igrn = 0;
          igrn = (igrn + 1 + (igrn >> 8)) >> 8;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][1] = (((uint8_t)igrn) & mask) | (src_grnp[frag] & ~mask);
        }
      }

      if (enable_blue) {
        for (frag = 0; frag < num_fragments; ++frag) {
          int32_t iblu = ((int32_t)src_blup[frag]) * src_wgh_blup[frag] - ((int32_t)rgba_dstppr[frag][2]) * dst_wgh_blup[frag];
          if (iblu < 0) iblu = 0;
          iblu = (iblu + 1 + (iblu >> 8)) >> 8;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][2] = (((uint8_t)iblu) & mask) | (src_blup[frag] & ~mask);
        }
      }
      break;
    }
    case BEQ_FUNC_REVERSE_SUBTRACT: {
      if (enable_red) {
        for (frag = 0; frag < num_fragments; ++frag) {
          int32_t ired = ((int32_t)rgba_dstppr[frag][0]) * dst_wgh_redp[frag] - ((int32_t)src_redp[frag]) * src_wgh_redp[frag];
          if (ired < 0) ired = 0;
          ired = (ired + 1 + (ired >> 8)) >> 8;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][0] = (((uint8_t)ired) & mask) | (src_redp[frag] & ~mask);
        }
      }

      if (enable_green) {
        for (frag = 0; frag < num_fragments; ++frag) {
          int32_t igrn = ((int32_t)rgba_dstppr[frag][1]) * dst_wgh_grnp[frag] - ((int32_t)src_grnp[frag]) * src_wgh_grnp[frag];
          if (igrn < 0) igrn = 0;
          igrn = (igrn + 1 + (igrn >> 8)) >> 8;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][1] = (((uint8_t)igrn) & mask) | (src_grnp[frag] & ~mask);
        }
      }

      if (enable_blue) {
        for (frag = 0; frag < num_fragments; ++frag) {
          int32_t iblu = ((int32_t)rgba_dstppr[frag][2]) * dst_wgh_blup[frag] - ((int32_t)src_blup[frag]) * src_wgh_blup[frag];
          if (iblu < 0) iblu = 0;
          iblu = (iblu + 1 + (iblu >> 8)) >> 8;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][2] = (((uint8_t)iblu) & mask) | (src_blup[frag] & ~mask);
        }
      }
      break;
    }
  }

  /* Alpha channel */
  switch (alpha_eq) {
    case BEQ_FUNC_ADD: {
      if (enable_alpha) {
        for (frag = 0; frag < num_fragments; ++frag) {
          uint32_t ialp = ((uint32_t)src_alpp[frag]) * src_wgh_alpp[frag] + ((uint32_t)rgba_dstppr[frag][3]) * dst_wgh_alpp[frag];
          ialp = (ialp + 1 + (ialp >> 8)) >> 8;
          /* Saturation */
          ialp = (ialp > 255) ? 255 : ialp;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][3] = (((uint8_t)ialp) & mask) | (src_alpp[frag] & ~mask);
        }
      }
      break;
    }
    case BEQ_FUNC_SUBTRACT: {
      if (enable_alpha) {
        for (frag = 0; frag < num_fragments; ++frag) {
          int32_t ialp = ((int32_t)src_alpp[frag]) * src_wgh_alpp[frag] - ((int32_t)rgba_dstppr[frag][3]) * dst_wgh_alpp[frag];
          if (ialp < 0) ialp = 0;
          ialp = (ialp + 1 + (ialp >> 8)) >> 8;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][3] = (((uint8_t)ialp) & mask) | (src_alpp[frag] & ~mask);
        }
      }
      break;
    }
    case BEQ_FUNC_REVERSE_SUBTRACT: {
      if (enable_alpha) {
        for (frag = 0; frag < num_fragments; ++frag) {
          int32_t ialp = ((int32_t)rgba_dstppr[frag][3]) * rgba_dstppr[frag][3] - ((int32_t)src_alpp[frag]) * src_wgh_alpp[frag];
          if (ialp < 0) ialp = 0;
          ialp = (ialp + 1 + (ialp >> 8)) >> 8;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][3] = (((uint8_t)ialp) & mask) | (src_alpp[frag] & ~mask);
        }
      }
      break;
    }
  }
}

