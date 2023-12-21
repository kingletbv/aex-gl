
#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

typedef enum blend_eq {
  BEQ_FUNC_ADD,
  BEQ_FUNC_SUBTRACT,
  BEQ_FUNC_REVERSE_SUBTRACT
} blend_eq_t;

typedef enum blend_func {
  BF_ZERO,
  BF_ONE,
  BF_SRC_COLOR,
  BF_ONE_MINUS_SRC_COLOR,
  BF_DST_COLOR,
  BF_ONE_MINUS_DST_COLOR,
  BF_SRC_ALPHA,
  BF_ONE_MINUS_SRC_ALPHA,
  BF_DST_ALPHA,
  BF_ONE_MINUS_DST_ALPHA,
  BF_CONSTANT_COLOR,
  BF_ONE_MINUS_CONSTANT_COLOR,
  BF_CONSTANT_ALPHA,
  BF_ONE_MINUS_CONSTANT_ALPHA,
  BF_SRC_ALPHA_SATURATE
} blend_func_t;

void blend2(size_t num_fragments, uint8_t *maskp, uint8_t *rgba_srcp, uint8_t **rgba_dstpp, 
            int enable_red, int enable_green, int enable_blue, int enable_alpha,
            blend_eq_t rgb_eq, blend_eq_t alpha_eq,
            blend_func_t src_rgb_fn, blend_func_t src_alpha_fn,
            blend_func_t dst_rgb_fn, blend_func_t dst_alpha_fn,
            uint8_t constant_red, uint8_t constant_grn, uint8_t constant_blu, uint8_t constant_alpha,
            uint8_t *scratch_src_wgh_redp, uint8_t *scratch_src_wgh_grnp, uint8_t *scratch_src_wgh_blup, uint8_t *scratch_src_wgh_alpp,
            uint8_t *scratch_dst_wgh_redp, uint8_t *scratch_dst_wgh_grnp, uint8_t *scratch_dst_wgh_blup, uint8_t *scratch_dst_wgh_alpp,
            uint8_t *scratch_src_redp, uint8_t *scratch_src_grnp, uint8_t *scratch_src_blup, uint8_t *scratch_src_alpp,
            uint8_t *scratch_dst_redp, uint8_t *scratch_dst_grnp, uint8_t *scratch_dst_blup, uint8_t *scratch_dst_alpp) {
  size_t frag;
  uint8_t *restrict maskpr = (uint8_t *restrict)maskp;
  uint8_t *restrict rgba_srcpr = (uint8_t *restrict)rgba_srcp;
  uint8_t *restrict *restrict rgba_dstppr = (uint8_t *restrict *restrict) rgba_dstpp;

  uint8_t *restrict src_wgh_redp = (uint8_t *restrict)scratch_src_wgh_redp;
  uint8_t *restrict src_wgh_grnp = (uint8_t *restrict)scratch_src_wgh_grnp;
  uint8_t *restrict src_wgh_blup = (uint8_t *restrict)scratch_src_wgh_blup;
  uint8_t *restrict src_wgh_alpp = (uint8_t *restrict)scratch_src_wgh_alpp;

  uint8_t *restrict dst_wgh_redp = (uint8_t *restrict)scratch_dst_wgh_redp;
  uint8_t *restrict dst_wgh_grnp = (uint8_t *restrict)scratch_dst_wgh_grnp;
  uint8_t *restrict dst_wgh_blup = (uint8_t *restrict)scratch_dst_wgh_blup;
  uint8_t *restrict dst_wgh_alpp = (uint8_t *restrict)scratch_dst_wgh_alpp;

  uint8_t *restrict src_redp = (uint8_t *restrict)scratch_src_redp;
  uint8_t *restrict src_grnp = (uint8_t *restrict)scratch_src_grnp;
  uint8_t *restrict src_blup = (uint8_t *restrict)scratch_src_blup;
  uint8_t *restrict src_alpp = (uint8_t *restrict)scratch_src_alpp;

  uint8_t *restrict dst_redp = (uint8_t *restrict)scratch_dst_redp;
  uint8_t *restrict dst_grnp = (uint8_t *restrict)scratch_dst_grnp;
  uint8_t *restrict dst_blup = (uint8_t *restrict)scratch_dst_blup;
  uint8_t *restrict dst_alpp = (uint8_t *restrict)scratch_dst_alpp;

  /* Un-swizzle input bitmaps */
  for (frag = 0; frag < num_fragments; ++frag) {
    src_redp[frag] = rgba_srcpr[4 * frag + 0];
    src_grnp[frag] = rgba_srcpr[4 * frag + 1];
    src_blup[frag] = rgba_srcpr[4 * frag + 2];
    src_alpp[frag] = rgba_srcpr[4 * frag + 3];

    dst_redp[frag] = rgba_dstppr[frag][0];
    dst_grnp[frag] = rgba_dstppr[frag][1];
    dst_blup[frag] = rgba_dstppr[frag][2];
    dst_alpp[frag] = rgba_dstppr[frag][3];
  }

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
        src_wgh_redp[frag] = dst_redp[frag];
        src_wgh_grnp[frag] = dst_grnp[frag];
        src_wgh_blup[frag] = dst_blup[frag];
      }
      break;
    case BF_ONE_MINUS_DST_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = 255 - dst_redp[frag];
        src_wgh_grnp[frag] = 255 - dst_grnp[frag];
        src_wgh_blup[frag] = 255 - dst_blup[frag];
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
        src_wgh_redp[frag] = dst_alpp[frag];
        src_wgh_grnp[frag] = dst_alpp[frag];
        src_wgh_blup[frag] = dst_alpp[frag];
      }
      break;
    case BF_ONE_MINUS_DST_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_redp[frag] = 255 - dst_alpp[frag];
        src_wgh_grnp[frag] = 255 - dst_alpp[frag];
        src_wgh_blup[frag] = 255 - dst_alpp[frag];
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
        src_wgh_redp[frag] = 255 - dst_alpp[frag];
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
        src_wgh_alpp[frag] = dst_alpp[frag];
      }
      break;
    case BF_ONE_MINUS_DST_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = 255 - dst_alpp[frag];
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
        src_wgh_alpp[frag] = dst_alpp[frag];
      }
      break;
    case BF_ONE_MINUS_DST_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        src_wgh_alpp[frag] = 255 - dst_alpp[frag];
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
        dst_wgh_redp[frag] = dst_redp[frag];
        dst_wgh_grnp[frag] = dst_grnp[frag];
        dst_wgh_blup[frag] = dst_blup[frag];
      }
      break;
    case BF_ONE_MINUS_DST_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = 255 - dst_redp[frag];
        dst_wgh_grnp[frag] = 255 - dst_grnp[frag];
        dst_wgh_blup[frag] = 255 - dst_blup[frag];
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
        dst_wgh_redp[frag] = dst_alpp[frag];
        dst_wgh_grnp[frag] = dst_alpp[frag];
        dst_wgh_blup[frag] = dst_alpp[frag];
      }
      break;
    case BF_ONE_MINUS_DST_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_redp[frag] = 255 - dst_alpp[frag];
        dst_wgh_grnp[frag] = 255 - dst_alpp[frag];
        dst_wgh_blup[frag] = 255 - dst_alpp[frag];
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
        dst_wgh_redp[frag] = 255 - dst_alpp[frag];
        dst_wgh_redp[frag] = (dst_wgh_redp[frag] < src_alpp[frag]) ? dst_wgh_redp[frag] : src_alpp[frag];
        dst_wgh_blup[frag] = dst_wgh_grnp[frag] = dst_wgh_redp[frag];
      }
      break;
  }

  switch (src_alpha_fn) {
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
        dst_wgh_alpp[frag] = dst_alpp[frag];
      }
      break;
    case BF_ONE_MINUS_DST_COLOR:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = 255 - dst_alpp[frag];
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
        dst_wgh_alpp[frag] = dst_alpp[frag];
      }
      break;
    case BF_ONE_MINUS_DST_ALPHA:
      for (frag = 0; frag < num_fragments; ++frag) {
        dst_wgh_alpp[frag] = 255 - dst_alpp[frag];
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
          uint32_t ired = ((uint32_t)src_redp[frag]) * src_wgh_redp[frag] + ((uint32_t)dst_redp[frag]) * dst_wgh_redp[frag];
          ired = (ired + 1 + (ired >> 8)) >> 8;
          /* Saturation */
          ired = (ired > 255) ? 255 : ired;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][0] = (((uint8_t)ired) & mask) | (src_redp[frag] & ~mask);
        }
      }

      if (enable_green) {
        for (frag = 0; frag < num_fragments; ++frag) {
          uint32_t igrn = ((uint32_t)src_grnp[frag]) * src_wgh_grnp[frag] + ((uint32_t)dst_grnp[frag]) * dst_wgh_grnp[frag];
          igrn = (igrn + 1 + (igrn >> 8)) >> 8;
          /* Saturation */
          igrn = (igrn > 255) ? 255 : igrn;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][1] = (((uint8_t)igrn) & mask) | (src_grnp[frag] & ~mask);
        }
      }

      if (enable_blue) {
        for (frag = 0; frag < num_fragments; ++frag) {
          uint32_t iblu = ((uint32_t)src_blup[frag]) * src_wgh_blup[frag] + ((uint32_t)dst_blup[frag]) * dst_wgh_blup[frag];
          iblu = (iblu + 1 + (iblu >> 8)) >> 8;
          /* Saturation */
          iblu = (iblu > 255) ? 255 : iblu;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][2] = (((uint8_t)iblu) & mask) | (src_blup[frag] & ~mask);
        }
      }
      break;
    }
    case BEQ_FUNC_SUBTRACT: {
      if (enable_red) {
        for (frag = 0; frag < num_fragments; ++frag) {
          int32_t ired = ((int32_t)src_redp[frag]) * src_wgh_redp[frag] - ((int32_t)dst_redp[frag]) * dst_wgh_redp[frag];
          if (ired < 0) ired = 0;
          ired = (ired + 1 + (ired >> 8)) >> 8;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][0] = (((uint8_t)ired) & mask) | (src_redp[frag] & ~mask);
        }
      }

      if (enable_green) {
        for (frag = 0; frag < num_fragments; ++frag) {
          int32_t igrn = ((int32_t)src_grnp[frag]) * src_wgh_grnp[frag] - ((int32_t)dst_grnp[frag]) * dst_wgh_grnp[frag];
          if (igrn < 0) igrn = 0;
          igrn = (igrn + 1 + (igrn >> 8)) >> 8;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][1] = (((uint8_t)igrn) & mask) | (src_grnp[frag] & ~mask);
        }
      }

      if (enable_blue) {
        for (frag = 0; frag < num_fragments; ++frag) {
          int32_t iblu = ((int32_t)src_blup[frag]) * src_wgh_blup[frag] - ((int32_t)dst_blup[frag]) * dst_wgh_blup[frag];
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
          int32_t ired = ((int32_t)dst_redp[frag]) * dst_wgh_redp[frag] - ((int32_t)src_redp[frag]) * src_wgh_redp[frag];
          if (ired < 0) ired = 0;
          ired = (ired + 1 + (ired >> 8)) >> 8;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][0] = (((uint8_t)ired) & mask) | (src_redp[frag] & ~mask);
        }
      }

      if (enable_green) {
        for (frag = 0; frag < num_fragments; ++frag) {
          int32_t igrn = ((int32_t)dst_grnp[frag]) * dst_wgh_grnp[frag] - ((int32_t)src_grnp[frag]) * src_wgh_grnp[frag];
          if (igrn < 0) igrn = 0;
          igrn = (igrn + 1 + (igrn >> 8)) >> 8;
          uint8_t mask = maskpr[frag];
          rgba_dstppr[frag][1] = (((uint8_t)igrn) & mask) | (src_grnp[frag] & ~mask);
        }
      }

      if (enable_blue) {
        for (frag = 0; frag < num_fragments; ++frag) {
          int32_t iblu = ((int32_t)dst_blup[frag]) * dst_wgh_blup[frag] - ((int32_t)src_blup[frag]) * src_wgh_blup[frag];
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
          uint32_t ialp = ((uint32_t)src_alpp[frag]) * src_wgh_alpp[frag] + ((uint32_t)dst_alpp[frag]) * dst_wgh_alpp[frag];
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
          int32_t ialp = ((int32_t)src_alpp[frag]) * src_wgh_alpp[frag] - ((int32_t)dst_alpp[frag]) * dst_wgh_alpp[frag];
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
          int32_t ialp = ((int32_t)dst_alpp[frag]) * dst_wgh_alpp[frag] - ((int32_t)src_alpp[frag]) * src_wgh_alpp[frag];
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

void blend(size_t num_fragments, uint8_t *maskp, uint8_t *rgba_srcp, uint8_t **rgba_dstpp, 
           int enable_red, int enable_green, int enable_blue, int enable_alpha,
           blend_eq_t rgb_eq, blend_eq_t alpha_eq,
           blend_func_t src_rgb_fn, blend_func_t src_alpha_fn,
           blend_func_t dst_rgb_fn, blend_func_t dst_alpha_fn,
           uint8_t constant_red, uint8_t constant_grn, uint8_t constant_blu, uint8_t constant_alpha) {
  size_t frag;
  uint8_t *restrict maskpr = (uint8_t *restrict)maskp;
  uint8_t *restrict rgba_srcpr = (uint8_t *restrict)rgba_srcp;
  uint8_t *restrict * restrict rgba_dstppr = (uint8_t *restrict *restrict) rgba_dstpp;

  for (frag = 0; frag < num_fragments; ++frag) {
    uint8_t mask = *maskpr;
    uint8_t src_red = rgba_srcpr[0];
    uint8_t src_grn = rgba_srcpr[1];
    uint8_t src_blu = rgba_srcpr[2];
    uint8_t src_alp = rgba_srcpr[3];
    uint8_t *restrict dstpr = *rgba_dstppr;
    uint8_t dst_red = dstpr[0];
    uint8_t dst_grn = dstpr[1];
    uint8_t dst_blu = dstpr[2];
    uint8_t dst_alp = dstpr[3];
    maskpr++;
    rgba_srcp += 4;
    rgba_dstppr++;

    uint8_t dst_wgh_red;
    uint8_t dst_wgh_grn;
    uint8_t dst_wgh_blu;
    uint8_t dst_wgh_alp;

    uint8_t src_wgh_red;
    uint8_t src_wgh_grn;
    uint8_t src_wgh_blu;
    uint8_t src_wgh_alp;

    switch (src_rgb_fn) {
      case BF_ZERO:
        src_wgh_red = 0;
        src_wgh_grn = 0;
        src_wgh_blu = 0;
        break;
      case BF_ONE:
        src_wgh_red = 255;
        src_wgh_grn = 255;
        src_wgh_blu = 255;
        break;
      case BF_SRC_COLOR:
        src_wgh_red = src_red;
        src_wgh_grn = src_grn;
        src_wgh_blu = src_blu;
        break;
      case BF_ONE_MINUS_SRC_COLOR:
        src_wgh_red = 255 - src_red;
        src_wgh_grn = 255 - src_grn;
        src_wgh_blu = 255 - src_blu;
        break;
      case BF_DST_COLOR:
        src_wgh_red = dst_red;
        src_wgh_grn = dst_grn;
        src_wgh_blu = dst_blu;
        break;
      case BF_ONE_MINUS_DST_COLOR:
        src_wgh_red = 255 - dst_red;
        src_wgh_grn = 255 - dst_grn;
        src_wgh_blu = 255 - dst_blu;
        break;
      case BF_SRC_ALPHA:
        src_wgh_red = src_alp;
        src_wgh_grn = src_alp;
        src_wgh_blu = src_alp;
        break;
      case BF_ONE_MINUS_SRC_ALPHA:
        src_wgh_red = 255 - src_alp;
        src_wgh_grn = 255 - src_alp;
        src_wgh_blu = 255 - src_alp;
        break;
      case BF_DST_ALPHA:
        src_wgh_red = dst_alp;
        src_wgh_grn = dst_alp;
        src_wgh_blu = dst_alp;
        break;
      case BF_ONE_MINUS_DST_ALPHA:
        src_wgh_red = 255 - dst_alp;
        src_wgh_grn = 255 - dst_alp;
        src_wgh_blu = 255 - dst_alp;
        break;
      case BF_CONSTANT_COLOR:
        src_wgh_red = constant_red;
        src_wgh_grn = constant_grn;
        src_wgh_blu = constant_blu;
        break;
      case BF_ONE_MINUS_CONSTANT_COLOR:
        src_wgh_red = 255 - constant_red;
        src_wgh_grn = 255 - constant_grn;
        src_wgh_blu = 255 - constant_blu;
        break;
      case BF_CONSTANT_ALPHA:
        src_wgh_red = constant_alpha;
        src_wgh_grn = constant_alpha;
        src_wgh_blu = constant_alpha;
        break;
      case BF_ONE_MINUS_CONSTANT_ALPHA:
        src_wgh_red = 255 - constant_alpha;
        src_wgh_grn = 255 - constant_alpha;
        src_wgh_blu = 255 - constant_alpha;
        break;
      case BF_SRC_ALPHA_SATURATE:
        src_wgh_red = 255 - dst_alp;
        src_wgh_red = (src_wgh_red < src_alp) ? src_wgh_red : src_alp;
        src_wgh_blu = src_wgh_grn = src_wgh_red;
        break;
    }

    switch (src_alpha_fn) {
      case BF_ZERO:
        src_wgh_alp = 0;
        break;
      case BF_ONE:
        src_wgh_alp = 255;
        break;
      case BF_SRC_COLOR:
        src_wgh_alp = src_alp;
        break;
      case BF_ONE_MINUS_SRC_COLOR:
        src_wgh_alp = 255 - src_alp;
        break;
      case BF_DST_COLOR:
        src_wgh_alp = dst_alp;
        break;
      case BF_ONE_MINUS_DST_COLOR:
        src_wgh_alp = 255 - dst_alp;
        break;
      case BF_SRC_ALPHA:
        src_wgh_alp = src_alp;
        break;
      case BF_ONE_MINUS_SRC_ALPHA:
        src_wgh_alp = 255 - src_alp;
        break;
      case BF_DST_ALPHA:
        src_wgh_alp = dst_alp;
        break;
      case BF_ONE_MINUS_DST_ALPHA:
        src_wgh_alp = 255 - dst_alp;
        break;
      case BF_CONSTANT_COLOR:
        src_wgh_alp = constant_alpha;
        break;
      case BF_ONE_MINUS_CONSTANT_COLOR:
        src_wgh_alp = 255 - constant_alpha;
        break;
      case BF_CONSTANT_ALPHA:
        src_wgh_alp = constant_alpha;
        break;
      case BF_ONE_MINUS_CONSTANT_ALPHA:
        src_wgh_alp = 255 - constant_alpha;
        break;
      case BF_SRC_ALPHA_SATURATE:
        src_wgh_alp = 255;
        break;
    }

    switch (dst_alpha_fn) {
      case BF_ZERO:
        dst_wgh_alp = 0;
        break;
      case BF_ONE:
        dst_wgh_alp = 255;
        break;
      case BF_SRC_COLOR:
        dst_wgh_alp = src_alp;
        break;
      case BF_ONE_MINUS_SRC_COLOR:
        dst_wgh_alp = 255 - src_alp;
        break;
      case BF_DST_COLOR:
        dst_wgh_alp = dst_alp;
        break;
      case BF_ONE_MINUS_DST_COLOR:
        dst_wgh_alp = 255 - dst_alp;
        break;
      case BF_SRC_ALPHA:
        dst_wgh_alp = src_alp;
        break;
      case BF_ONE_MINUS_SRC_ALPHA:
        dst_wgh_alp = 255 - src_alp;
        break;
      case BF_DST_ALPHA:
        dst_wgh_alp = dst_alp;
        break;
      case BF_ONE_MINUS_DST_ALPHA:
        dst_wgh_alp = 255 - dst_alp;
        break;
      case BF_CONSTANT_COLOR:
        dst_wgh_alp = constant_alpha;
        break;
      case BF_ONE_MINUS_CONSTANT_COLOR:
        dst_wgh_alp = 255 - constant_alpha;
        break;
      case BF_CONSTANT_ALPHA:
        dst_wgh_alp = constant_alpha;
        break;
      case BF_ONE_MINUS_CONSTANT_ALPHA:
        dst_wgh_alp = 255 - constant_alpha;
        break;
      case BF_SRC_ALPHA_SATURATE:
        dst_wgh_alp = 255;
        break;
    }

    switch (dst_rgb_fn) {
      case BF_ZERO:
        dst_wgh_red = 0;
        dst_wgh_grn = 0;
        dst_wgh_blu = 0;
        break;
      case BF_ONE:
        dst_wgh_red = 255;
        dst_wgh_grn = 255;
        dst_wgh_blu = 255;
        break;
      case BF_SRC_COLOR:
        dst_wgh_red = src_red;
        dst_wgh_grn = src_grn;
        dst_wgh_blu = src_blu;
        break;
      case BF_ONE_MINUS_SRC_COLOR:
        dst_wgh_red = 255 - src_red;
        dst_wgh_grn = 255 - src_grn;
        dst_wgh_blu = 255 - src_blu;
        break;
      case BF_DST_COLOR:
        dst_wgh_red = dst_red;
        dst_wgh_grn = dst_grn;
        dst_wgh_blu = dst_blu;
        break;
      case BF_ONE_MINUS_DST_COLOR:
        dst_wgh_red = 255 - dst_red;
        dst_wgh_grn = 255 - dst_grn;
        dst_wgh_blu = 255 - dst_blu;
        break;
      case BF_SRC_ALPHA:
        dst_wgh_red = src_alp;
        dst_wgh_grn = src_alp;
        dst_wgh_blu = src_alp;
        break;
      case BF_ONE_MINUS_SRC_ALPHA:
        dst_wgh_red = 255 - src_alp;
        dst_wgh_grn = 255 - src_alp;
        dst_wgh_blu = 255 - src_alp;
        break;
      case BF_DST_ALPHA:
        dst_wgh_red = dst_alp;
        dst_wgh_grn = dst_alp;
        dst_wgh_blu = dst_alp;
        break;
      case BF_ONE_MINUS_DST_ALPHA:
        dst_wgh_red = 255 - dst_alp;
        dst_wgh_grn = 255 - dst_alp;
        dst_wgh_blu = 255 - dst_alp;
        break;
      case BF_CONSTANT_COLOR:
        dst_wgh_red = constant_red;
        dst_wgh_grn = constant_grn;
        dst_wgh_blu = constant_blu;
        break;
      case BF_ONE_MINUS_CONSTANT_COLOR:
        dst_wgh_red = 255 - constant_red;
        dst_wgh_grn = 255 - constant_grn;
        dst_wgh_blu = 255 - constant_blu;
        break;
      case BF_CONSTANT_ALPHA:
        dst_wgh_red = constant_alpha;
        dst_wgh_grn = constant_alpha;
        dst_wgh_blu = constant_alpha;
        break;
      case BF_ONE_MINUS_CONSTANT_ALPHA:
        dst_wgh_red = 255 - constant_alpha;
        dst_wgh_grn = 255 - constant_alpha;
        dst_wgh_blu = 255 - constant_alpha;
        break;
      case BF_SRC_ALPHA_SATURATE:
        dst_wgh_red = 255 - dst_alp;
        dst_wgh_red = (dst_wgh_red < src_alp) ? dst_wgh_red : dst_alp;
        dst_wgh_blu = dst_wgh_grn = dst_wgh_red;

        break;
    }


    switch (rgb_eq) {
      case BEQ_FUNC_ADD: {
        /* Red channel */
        uint32_t ired = src_red * src_wgh_red + dst_red * dst_wgh_red;
        ired = (ired + 1 + (ired >> 8)) >> 8;
        /* saturation */
        ired = ired > 255 ? 255 : ired;
        dst_red = (uint8_t)ired;

        /* Green channel */
        uint32_t igrn = src_grn * src_wgh_grn + dst_grn * dst_wgh_grn;
        igrn = (igrn + 1 + (igrn >> 8)) >> 8;
        /* saturation */
        igrn = igrn > 255 ? 255 : igrn;
        dst_grn = (uint8_t)igrn;

        /* Blue channel */
        uint32_t iblu = src_blu * src_wgh_blu + dst_blu * dst_wgh_blu;
        iblu = (iblu + 1 + (iblu >> 8)) >> 8;
        /* saturation */
        iblu = iblu > 255 ? 255 : iblu;
        dst_blu = (uint8_t)igrn;
        break;
      }
      case BEQ_FUNC_SUBTRACT: {
        /* Red channel */
        int32_t ired = src_red * src_wgh_red - dst_red * dst_wgh_red;
        if (ired < 0) ired = 0;
        ired = (ired + 1 + (ired >> 8)) >> 8;
        dst_red = (uint8_t)ired;

        /* Green channel */
        int32_t igrn = src_grn * src_wgh_grn - dst_grn * dst_wgh_grn;
        if (igrn < 0) igrn = 0;
        igrn = (igrn + 1 + (igrn >> 8)) >> 8;
        dst_grn = (uint8_t)igrn;

        /* Blue channel */
        int32_t iblu = src_blu * src_wgh_blu - dst_blu * dst_wgh_blu;
        if (iblu < 0) iblu = 0;
        iblu = (iblu + 1 + (iblu >> 8)) >> 8;
        dst_blu = (uint8_t)igrn;
        break;
      }
      case BEQ_FUNC_REVERSE_SUBTRACT: {
        /* Red channel */
        int32_t ired = dst_red * dst_wgh_red - src_red * src_wgh_red;
        if (ired < 0) ired = 0;
        ired = (ired + 1 + (ired >> 8)) >> 8;
        dst_red = (uint8_t)ired;

        /* Green channel */
        int32_t igrn = dst_grn * dst_wgh_grn - src_grn * src_wgh_grn;
        if (igrn < 0) igrn = 0;
        igrn = (igrn + 1 + (igrn >> 8)) >> 8;
        dst_grn = (uint8_t)igrn;

        /* Blue channel */
        int32_t iblu = dst_blu * dst_wgh_blu - src_blu * src_wgh_blu;
        if (iblu < 0) iblu = 0;
        iblu = (iblu + 1 + (iblu >> 8)) >> 8;
        dst_blu = (uint8_t)igrn;
        break;
      }
    }

    /* Alpha channel */
    switch (alpha_eq) {
      case BEQ_FUNC_ADD: {
        uint32_t ialp = src_alp * src_wgh_alp + dst_alp * dst_wgh_alp;
        ialp = (ialp + 1 + (ialp >> 8)) >> 8;
        dst_alp = (uint8_t)ialp;
        break;
      }
      case BEQ_FUNC_SUBTRACT: {
        int32_t ialp = src_alp * src_wgh_alp - dst_alp * dst_wgh_alp;
        if (ialp < 0) ialp = 0;
        ialp = (ialp + 1 + (ialp >> 8)) >> 8;
        dst_alp = (uint8_t)ialp;
        break;
      }
      case BEQ_FUNC_REVERSE_SUBTRACT: {
        int32_t ialp = dst_alp * dst_wgh_alp - src_alp * src_wgh_alp;
        if (ialp < 0) ialp = 0;
        ialp = (ialp + 1 + (ialp >> 8)) >> 8;
        dst_alp = (uint8_t)ialp;
        break;
      }
    }

    if (enable_red) dstpr[0] = dst_red;
    if (enable_green) dstpr[1] = dst_grn;
    if (enable_blue) dstpr[2] = dst_blu;
    if (enable_alpha) dstpr[3] = dst_alp;
  }

}
