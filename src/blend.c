
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
        dst_wgh_red = (src_wgh_red < src_alp) ? src_wgh_red : src_alp;
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
