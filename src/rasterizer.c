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

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef RASTERIZER_H_INCLUDED
#define RASTERIZER_H_INCLUDED
#include "rasterizer.h"
#endif

#ifndef FRAGMENT_BUFFER_H_INCLUDED
#define FRAGMENT_BUFFER_H_INCLUDED
#include "fragment_buffer.h"
#endif

#ifndef PRIMITIVE_ASSEMBLY_H_INCLUDED
#define PRIMITIVE_ASSEMBLY_H_INCLUDED
#include "primitive_assembly.h"
#endif

static void adds128(int64_t ahi, int64_t alo, int64_t bhi, int64_t blo, int64_t *rhi, int64_t *rlo) {
  /* Implement signed addition as unsigned addition (identical if reg sizes are same) */
  uint64_t result_lo = ((uint64_t)alo) + ((uint64_t)blo);
  uint64_t result_hi = ((uint64_t)ahi) + ((uint64_t)bhi);
  if (result_lo < (uint64_t)alo) {
    /* carry set */
    result_hi++;
  }
  if (rhi) {
    *rhi = (int64_t)result_hi;
  }
  if (rlo) {
    *rlo = (int64_t)result_lo;
  }
}

static void subs128(int64_t ahi, int64_t alo, int64_t bhi, int64_t blo, int64_t *rhi, int64_t *rlo) {
  /* Two's complement negation */
  blo = ~blo;
  bhi = ~bhi;
  blo++;
  if (!blo /* following +1, is carry set? */) {
    bhi++;
  }

  /* adding the negated value is the same as subtracting */
  uint64_t result_lo = ((uint64_t)alo) + ((uint64_t)blo);
  uint64_t result_hi = ((uint64_t)ahi) + ((uint64_t)bhi);
  if (result_lo < (uint64_t)alo) {
    /* carry set */
    result_hi++;
  }
  if (rhi) {
    *rhi = (int64_t)result_hi;
  }
  if (rlo) {
    *rlo = (int64_t)result_lo;
  }
}


static int mulu64(uint64_t a, uint64_t b, uint64_t *rhi, uint64_t *rlo) {
  /* Multiplication:
   * xy = z2 b^2 + z1 b + z0
   * where:
   * z2 = x1y1
   * z1 = x1y0 + x0y1
   * z0 = x0y0
   * and b is base 2^32 */
  uint32_t x0 = a & 0xFFFFFFFF;
  uint32_t x1 = (a >> 32) & 0xFFFFFFFF;
  uint32_t y0 = b & 0xFFFFFFFF;
  uint32_t y1 = (b >> 32) & 0xFFFFFFFF;

  uint64_t z2 = ((uint64_t)x1) * ((uint64_t)y1);
  uint64_t z0 = ((uint64_t)x0) * ((uint64_t)y0);
  uint64_t x1y0 = ((uint64_t)x1) * ((uint64_t)y0);
  uint64_t x0y1 = ((uint64_t)x0) * ((uint64_t)y1);
  uint64_t z1 = x1y0 + x0y1;
  if (z1 < x1y0) {
    /* Carry overflow from z1; add in to z2, note also that z2
     * is at most 0xFFFFFFFE00000001 and so consequently would
     * hereby become 0xFFFFFFFF00000001 in the worst case (we
     * therefore don't need to check for overflow) */
    z2 += ((uint64_t)1) << 32;
  }
  uint64_t result0;
  uint64_t result1;
  result0 = z0 + (z1 << 32);
  if (result0 < z0) {
    /* Overflow from z0 + loword(z1) into upper word of result */
    z2++;
  }
  result1 = (z1 >> 32) + z2;

  if (rlo) {
    *rlo = result0;
  }
  if (rhi) {
    /* note: can skip z2 if not interested in rhi */
    *rhi = result1;
  }

  return !!result1;
}

static int muls64(int64_t a, int64_t b, int64_t *rhi, int64_t *rlo) {
  int is_neg = 0;
  if (a < 0) {
    a = -a;
    is_neg ^= 1;
  }
  if (b < 0) {
    b = -b;
    is_neg ^= 1;
  }
  uint64_t result_hi, result_lo;
  int r = mulu64((uint64_t)a, (uint64_t)b, &result_hi, &result_lo);
  if (is_neg) {
    // Two's complement negation.
    result_lo = ~result_lo;
    result_hi = ~result_hi;
    result_lo++;
    if (!result_lo /* following +1, is carry set? */) {
      result_hi++;
    }
  }

  if (rlo) {
    *rlo = (int64_t)result_lo;
  }
  if (rhi) {
    /* note: can skip z2 if not interested in rhi */
    *rhi = (int64_t)result_hi;
  }

  return r;
}

static void mulu128(uint64_t ahi, uint64_t alo, uint64_t bhi, uint64_t blo, uint64_t *rhi, uint64_t *rlo) {
  //                 ahi         alo
  //                 bhi         blo x
  //                 ---------------
  //                         blo*alo
  //                 blo*ahi
  //                 bhi*alo
  //         bhi*ahi
  //      
  //                [--- 128 bits ---]
  // [-- 128 bits --]
  //     upper            lower
  // we only care about the lower 128 bits; 
  // this simplifies things for us.
  uint64_t blo_alo_lo, blo_alo_hi;
  mulu64(blo, alo, &blo_alo_hi, &blo_alo_lo);

  uint64_t r_lo, r_hi;
  r_lo = blo_alo_lo;
  r_hi = blo_alo_hi;
  r_hi += blo * ahi;
  r_hi += bhi * alo;
  if (rhi) *rhi = r_hi;
  if (rlo) *rlo = r_lo;
}

static void muls128(int64_t ahi, int64_t alo, int64_t bhi, int64_t blo, int64_t *rhi, int64_t *rlo) {
  int is_neg = 0;
  if (ahi < 0) {
    is_neg ^= 1;
    alo = ~alo;
    ahi = ~ahi;
    alo++;
    if (!alo) ahi++;
  }
  if (bhi < 0) {
    is_neg ^= 1;
    blo = ~blo;
    bhi = ~bhi;
    blo++;
    if (!blo) bhi++;
  }
  uint64_t res_hi, res_lo;
  mulu128(ahi, alo, bhi, blo, &res_hi, &res_lo);
  if (is_neg) {
    res_lo = ~res_lo;
    res_hi = ~res_hi;
    res_lo++;
    if (!res_lo) res_hi++;
  }
  if (rhi) *rhi = res_hi;
  if (rlo) *rlo = res_lo;
}

/* Performs an unsigned division of a 128 bit numerator by a 64 bit denominator, returning
 * a 128 bit number, split into a high and a low 64 bit number, and the remainder as a
 * 64 bit number.
 * Return value == -1 for division by zero.
 *                  1 if the result requires more than 64 bits
 *                  0 if the result requires less than 64 bits.
 * *rhi is optional, you can check the return value for 1 to see if the result fits into *rlo.
 */
static int divremu128by64(uint64_t numhi, uint64_t numlo, uint64_t den, uint64_t *rhi, uint64_t *rlo, uint64_t *rem) {
  /* numerator shifted - note that the digit ordering, as is for all arrays, is little endian, with
   * the least significant digit first. */
  uint32_t sn[5] ={ 0 };
  sn[0] = (uint32_t)(numlo & 0xFFFFFFFF);
  sn[1] = (uint32_t)(numlo >> 32);
  sn[2] = (uint32_t)(numhi & 0xFFFFFFFF);
  sn[3] = (uint32_t)(numhi >> 32);
  sn[4] = 0;

  /* denominator shifted */
  uint32_t sd[4] ={ 0 };
  sd[0] = (uint32_t)(den & 0xFFFFFFFF);
  sd[1] = (uint32_t)(den >> 32);
  sd[2] = 0;
  sd[3] = 0;

  /* subtraction register */
  uint32_t sr[5];
  /* result register */
  uint32_t r[4] ={ 0 };

  uint32_t digit;
  uint64_t acc;
  int shift, lead_den_digit, lead_num_digit, n;

  for (lead_den_digit = 3; lead_den_digit >= 0; --lead_den_digit) {
    if (sd[lead_den_digit]) break;
  }

  if (lead_den_digit == -1) {
    /* Division by zero */
    return -1;
  }

  for (lead_num_digit = 3; lead_num_digit >= 0; --lead_num_digit) {
    if (sn[lead_num_digit]) break;
  }

  if (lead_den_digit > lead_num_digit) {
    /* Denominator is larger than numerator, the result is therefore zero. */
    if (rhi) {
      *rhi = 0x0ULL;
    }
    if (rlo) {
      *rlo = 0x0ULL;
    }
    if (rem) {
      *rem = numlo;
    }
    return 0;
  }

  /* Shift numerator and denominator left until the denominator's lead bit is in the
   * most significant bit slot. We do this so that our "guessing" division in the core
   * division algorithm is as accurate as possible because the denominator of *that*
   * division (the lead digit of the overall denominator) is as big as possible. */
  digit = sd[lead_den_digit];
  if (digit & 0xFFFF0000) {
    shift = 0;
  }
  else {
    shift = 16;
    digit <<= 16;
  }
  if (!(digit & 0xFF000000)) {
    shift += 8;
    digit <<= 8;
  }
  if (!(digit & 0xF0000000)) {
    shift += 4;
    digit <<= 4;
  }
  if (!(digit & 0xC0000000)) {
    shift += 2;
    digit <<= 2;
  }
  if (!(digit & 0x80000000)) {
    shift++;
  }
  /* the "& (-shift >> 31)" part of the equation handles the case for shift == 0,
   * during which we'd attempt a right-shift of (32 - 0) == 32, which is undefined
   * over a 32 bit number (and likely to equal a shift of 0). The desired behavior
   * is for the entire result to be 0, as-if all 0's would have shifted in. */
  sn[lead_num_digit + 1] = (sn[lead_num_digit] >> (32 - shift)) & (-shift >> 31);
  for (n = lead_num_digit; n > 0; --n) {
    sn[n] = (sn[n] << shift) | ((sn[n - 1] >> (32 - shift)) & (-shift >> 31));
  }
  sn[0] = sn[0] << shift;
  for (n = lead_den_digit; n > 0; --n) {
    sd[n] = (sd[n] << shift) | ((sd[n - 1] >> (32 - shift)) & (-shift >> 31));
  }
  sd[0] = sd[0] << shift;

  for (n = lead_num_digit; n >= lead_den_digit; --n) {
    uint64_t div;
    uint64_t mul;
    int k;
    int sr_is_bigger;
    div = (((uint64_t)sn[n + 1]) << 32) | sn[n];
    div /= sd[lead_den_digit];
    mul = 0;
    for (k = 0; k <= lead_den_digit; ++k) {
      uint64_t factor = div * ((uint64_t)sd[k]);
      mul += factor;
      sr[k] = (uint32_t)mul;
      mul >>= 32;
    }
    sr[k] = (uint32_t)mul;

    do {
      sr_is_bigger = 0;
      for (k = lead_den_digit + 1; k >= 0; --k) {
        if (sr[k] < sn[k - lead_den_digit + n]) {
          sr_is_bigger = 0;
          break;
        }
        else if (sr[k] > sn[k - lead_den_digit + n]) {
          sr_is_bigger = 1;
          break;
        }
      }

      if (sr_is_bigger) {
        /* Estimate is off, subtract denominator from subtraction register */
        acc = 0;
        --div;
        for (k = 0; k <= lead_den_digit; ++k) {
          acc += ((uint64_t)sr[k]) - (uint64_t)sd[k];
          sr[k] = (uint32_t)acc;
          acc = (uint64_t)(((int64_t)acc) >> 32);
        }
        sr[k] += (uint32_t)acc;
      }
    } while (sr_is_bigger);

    /* Subtract the subtraction register (containing the current dividend digit
     * estimate times the denominator) from the numerator to leave the remainder
     * for subsequent digits. */
    acc = 0;
    for (k = 0; k <= (lead_den_digit + 1); ++k) {
      acc += ((uint64_t)sn[k - lead_den_digit + n]) - ((uint64_t)sr[k]);
      sn[k - lead_den_digit + n] = (uint32_t)acc;
      acc = (uint64_t)(((int64_t)acc) >> 32);
    }

    r[n - lead_den_digit] = (uint32_t)div;
  }

  if (rhi) {
    /* Again: notice the little-endian convention for digits */
    *rhi = (((uint64_t)r[3]) << 32) | ((uint64_t)r[2]);
  }
  if (rlo) {
    *rlo = (((uint64_t)r[1]) << 32) | ((uint64_t)r[0]);
  }
  if (rem) {
    *rem = (sn[0] >> shift) | 
           (((uint64_t)sn[1]) << (32 - shift));
    if (shift) {
      *rem |= (((uint64_t)sn[2]) << (64 - shift));
    }
  }

  /* Return 0 if result fits in 64 bits, 1 if more bits are needed. */
  return (r[3] || r[2]) ? 1 : 0;
}


static int divrems128by64(int64_t numhi, int64_t numlo, int64_t den, int64_t *rhi, int64_t *rlo, int64_t *rem) {
  int is_neg = 0;
  if (numhi < 0) {
    // Two's complement negation.
    numhi = ~numhi;
    numlo = ~numlo;
    numlo++;
    if (!numlo /* following +1, is carry set? */) {
      numhi++;
    }
    is_neg ^= 1;
  }
  if (den < 0) {
    den = -den;
    is_neg ^= 1;
  }
  int64_t result_hi, result_lo;
  int64_t remainder;
  int r = divremu128by64((uint64_t)numhi, (uint64_t)numlo, (uint64_t)den, (uint64_t *)&result_hi, (uint64_t *)&result_lo, (uint64_t *)&remainder);
  if (is_neg) {
    // Two's complement negation.
    result_lo = ~result_lo;
    result_hi = ~result_hi;
    result_lo++;
    if (!result_lo /* following +1, is carry set? */) {
      result_hi++;
    }
    remainder = -remainder;
  }
  if (rhi) {
    *rhi = result_hi;
  }
  if (rlo) {
    *rlo = result_lo;
  }
  if (rem) {
    *rem = remainder;
  }
  return r;
}

static int divmods128by64(int64_t numhi, int64_t numlo, int64_t den, int64_t *rhi, int64_t *rlo, int64_t *mod) {
  int r;
  int64_t remainder = 0;
  r = divrems128by64(numhi, numlo, den, rhi, rlo, &remainder);
  if (remainder < 0) {
    remainder += den;
  }
  if (mod) *mod = remainder;
  return r;
}



void rasterizer_init(struct rasterizer *rasterizer) {
  /* must set this to 0 for the function to work correctly ... */
  rasterizer->resume_at_ = 0; 

  /* ... but we also set everything else to 0 just to bring it
   * into a well-defined state. */
  memset(rasterizer, 0, sizeof(struct rasterizer));
}

void rasterizer_reset(struct rasterizer *rasterizer) {
  rasterizer_init(rasterizer);
}

void rasterizer_cleanup(struct rasterizer *rasterizer) {
  /* nothing to do */
}

int64_t rasterizer_compute_D012(int32_t px0, int32_t py0, uint32_t pz0,
                                int32_t px1, int32_t py1, uint32_t pz1,
                                int32_t px2, int32_t py2, uint32_t pz2) {
  int64_t D012 = ((int64_t)px1) * ((int64_t)py2) - ((int64_t)px2) * ((int64_t)py1) - ((int64_t)px0) * ((int64_t)py2) + ((int64_t)px2) * ((int64_t)py0) + ((int64_t)px0) * ((int64_t)py1) - ((int64_t)px1) * ((int64_t)py0);
  if (D012 < 0) return -D012;
  return D012;
}

/* Returns non-zero if the fragment buffer fragbf is full and needs to be processed, zero otherwise. */
int rasterizer_triangle(struct rasterizer *rasterizer,
                        struct fragment_buffer *fragbf,
                        uint8_t *rgba, size_t stride,
                        rasterizer_early_zbuf_func_t early_z_zbuf_func,
                        uint8_t *zbuf, size_t zstride, size_t zstep,
                        uint8_t *stencilbuf, size_t stencil_stride, size_t stencil_step,
                        uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
                        int32_t px0, int32_t py0, uint32_t pz0,
                        int32_t px1, int32_t py1, uint32_t pz1,
                        int32_t px2, int32_t py2, uint32_t pz2,
                        int permitted_orientations,
                        int32_t offset_factor_fixed8, int32_t offset_units_fixed8) {

  int direction_xy_flips;
  int64_t D012;
  int orientation; /* RASTERIZER_CLOCKWISE or RASTERIZER_COUNTERCLOCKWISE */
  int32_t z_offset;
  int32_t x0, y0, z0;
  int32_t x1, y1, z1;
  int32_t x2, y2, z2;
  uint8_t *pixel_TL, *pixel_TR, *pixel_BL, *pixel_BR;
  uint8_t *zbuf_TL, *zbuf_TR, *zbuf_BL, *zbuf_BR;
  uint8_t *stencil_TL, *stencil_TR, *stencil_BL, *stencil_BR;
  int64_t pixel_mod, zbuf_mod, stencil_mod;
  int64_t px, py;

  int64_t Dp01_TL, Dp12_TL, Dp20_TL;
  int64_t Dp01_TR, Dp12_TR, Dp20_TR;
  int64_t Dp01_BL, Dp12_BL, Dp20_BL;
  int64_t Dp01_BR, Dp12_BR, Dp20_BR;

  int64_t Dp01_row_TL, Dp12_row_TL, Dp20_row_TL;
  int64_t Dp01_row_TR, Dp12_row_TR, Dp20_row_TR;
  int64_t Dp01_row_BL, Dp12_row_BL, Dp20_row_BL;
  int64_t Dp01_row_BR, Dp12_row_BR, Dp20_row_BR;

  int64_t z_TL, z_TR, z_BL, z_BR;
  int64_t z_s_TL, z_s_TR, z_s_BL, z_s_BR;
  int64_t z_sx_TL, z_sx_TR, z_sx_BL, z_sx_BR;
  int64_t z_x_TL, z_x_TR, z_x_BL, z_x_BR;

  int64_t scissor_top_row_mask, scissor_bottom_row_mask;
  int64_t scissor_left_column_mask, scissor_right_column_mask;

  int64_t TL_Mask, TR_Mask, BL_Mask, BR_Mask;
  int64_t Dp01_dx_2, Dp12_dx_2, Dp20_dx_2;
  int64_t Dp01_dy_2, Dp12_dy_2, Dp20_dy_2;
  int64_t z_xp, z_xq, z_xi;
  int64_t z_yp, z_yq, z_yi;

  int64_t left, top, right, bottom;

  /* Recover context for resuming from yield */
  direction_xy_flips = rasterizer->direction_xy_flips_;
  D012 = rasterizer->D012_;
  orientation = rasterizer->orientation_;
  z_offset = rasterizer->z_offset_;
  x0 = rasterizer->x0_;
  y0 = rasterizer->y0_;
  z0 = rasterizer->z0_;
  x1 = rasterizer->x1_;
  y1 = rasterizer->y1_;
  z1 = rasterizer->z1_;
  x2 = rasterizer->x2_;
  y2 = rasterizer->y2_;
  z2 = rasterizer->z2_;
  pixel_TL = rasterizer->pixel_TL_;
  pixel_TR = rasterizer->pixel_TR_;
  pixel_BL = rasterizer->pixel_BL_;
  pixel_BR = rasterizer->pixel_BR_;
  zbuf_TL = rasterizer->zbuf_TL_;
  zbuf_TR = rasterizer->zbuf_TR_;
  zbuf_BL = rasterizer->zbuf_BL_;
  zbuf_BR = rasterizer->zbuf_BR_;
  stencil_TL = rasterizer->stencil_TL_;
  stencil_TR = rasterizer->stencil_TR_;
  stencil_BL = rasterizer->stencil_BL_;
  stencil_BR = rasterizer->stencil_BR_;
  pixel_mod = rasterizer->pixel_mod_;
  zbuf_mod = rasterizer->zbuf_mod_;
  stencil_mod = rasterizer->stencil_mod_;
  px = rasterizer->px_;
  py = rasterizer->py_;
  Dp01_TL = rasterizer->Dp01_TL_;
  Dp12_TL = rasterizer->Dp12_TL_;
  Dp20_TL = rasterizer->Dp20_TL_;
  Dp01_TR = rasterizer->Dp01_TR_;
  Dp12_TR = rasterizer->Dp12_TR_;
  Dp20_TR = rasterizer->Dp20_TR_;
  Dp01_BL = rasterizer->Dp01_BL_;
  Dp12_BL = rasterizer->Dp12_BL_;
  Dp20_BL = rasterizer->Dp20_BL_;
  Dp01_BR = rasterizer->Dp01_BR_;
  Dp12_BR = rasterizer->Dp12_BR_;
  Dp20_BR = rasterizer->Dp20_BR_;
  Dp01_row_TL = rasterizer->Dp01_row_TL_;
  Dp12_row_TL = rasterizer->Dp12_row_TL_;
  Dp20_row_TL = rasterizer->Dp20_row_TL_;
  Dp01_row_TR = rasterizer->Dp01_row_TR_;
  Dp12_row_TR = rasterizer->Dp12_row_TR_;
  Dp20_row_TR = rasterizer->Dp20_row_TR_;
  Dp01_row_BL = rasterizer->Dp01_row_BL_;
  Dp12_row_BL = rasterizer->Dp12_row_BL_;
  Dp20_row_BL = rasterizer->Dp20_row_BL_;
  Dp01_row_BR = rasterizer->Dp01_row_BR_;
  Dp12_row_BR = rasterizer->Dp12_row_BR_;
  Dp20_row_BR = rasterizer->Dp20_row_BR_;
  z_TL = rasterizer->z_TL_;
  z_TR = rasterizer->z_TR_;
  z_BL = rasterizer->z_BL_;
  z_BR = rasterizer->z_BR_;
  z_s_TL = rasterizer->z_s_TL_;
  z_s_TR = rasterizer->z_s_TR_;
  z_s_BL = rasterizer->z_s_BL_;
  z_s_BR = rasterizer->z_s_BR_;
  z_sx_TL = rasterizer->z_sx_TL_;
  z_sx_TR = rasterizer->z_sx_TR_;
  z_sx_BL = rasterizer->z_sx_BL_;
  z_sx_BR = rasterizer->z_sx_BR_;
  z_x_TL = rasterizer->z_x_TL_;
  z_x_TR = rasterizer->z_x_TR_;
  z_x_BL = rasterizer->z_x_BL_;
  z_x_BR = rasterizer->z_x_BR_;
  scissor_top_row_mask = rasterizer->scissor_top_row_mask_;
  scissor_bottom_row_mask = rasterizer->scissor_bottom_row_mask_;
  scissor_left_column_mask = rasterizer->scissor_left_column_mask_;
  scissor_right_column_mask = rasterizer->scissor_right_column_mask_;
  left = rasterizer->left_;
  top = rasterizer->top_;
  right = rasterizer->right_;
  bottom = rasterizer->bottom_;
  TL_Mask = rasterizer->TL_Mask_;
  TR_Mask = rasterizer->TR_Mask_;
  BL_Mask = rasterizer->BL_Mask_;
  BR_Mask = rasterizer->BR_Mask_;
  Dp01_dx_2 = rasterizer->Dp01_dx_2_;
  Dp12_dx_2 = rasterizer->Dp12_dx_2_;
  Dp20_dx_2 = rasterizer->Dp20_dx_2_;
  Dp01_dy_2 = rasterizer->Dp01_dy_2_;
  Dp12_dy_2 = rasterizer->Dp12_dy_2_;
  Dp20_dy_2 = rasterizer->Dp20_dy_2_;
  z_xp = rasterizer->z_xp_;
  z_xq = rasterizer->z_xq_;
  z_xi = rasterizer->z_xi_;
  z_yp = rasterizer->z_yp_;
  z_yq = rasterizer->z_yq_;
  z_yi = rasterizer->z_yi_;

  switch (rasterizer->resume_at_) {
    case 1: goto ez_less16;
    case 2: goto ez_less24;
    case 3: goto ez_less32;
    case 4: goto ez_lequal16;
    case 5: goto ez_lequal24;
    case 6: goto ez_lequal32;
    case 7: goto ez_always;

    case 0: ;
    // Go 4 pixels at a time, the four fragments form a square, the square is valid if a single fragment
    // is inside the triangle and scissor (we will therefore intentionally be generating fragments that
    // are outside the triangle, or the scissor; we will discard those fragments later - reason for this
    // is to be able to do mip-mapping and dFdx/dFdy type stuff.)

    /* Determine bounding box, and intersect with the scissor rect */
    int64_t bleft, btop, bright, bbottom;
    bleft = bright = px0;
    btop = bbottom = py0;
    if (px1 < bleft) bleft = px1;
    if (px1 > bright) bright = px1;
    if (py1 < btop) btop = py1;
    if (py1 > bbottom) bbottom = py1;
    if (px2 < bleft) bleft = px2;
    if (px2 > bright) bright = px2;
    if (py2 < btop) btop = py2;
    if (py2 > bbottom) bbottom = py2;
    bleft = bleft >> RASTERIZER_SUBPIXEL_BITS;
    btop = btop >> RASTERIZER_SUBPIXEL_BITS;
    bright = (bright + ((1 << RASTERIZER_SUBPIXEL_BITS) - 1)) >> RASTERIZER_SUBPIXEL_BITS;
    bbottom = (bbottom + ((1 << RASTERIZER_SUBPIXEL_BITS) - 1)) >> RASTERIZER_SUBPIXEL_BITS;

    left = (bleft < scissor_left) ? scissor_left : bleft;
    right = (bright > scissor_right) ? scissor_right : bright;
    top = (btop < scissor_top) ? scissor_top : btop;
    bottom = (bbottom > scissor_bottom) ? scissor_bottom : bbottom;

    // Be inclusive of odd edges on the scissor rectangle.
    left = left & ~(int64_t)1;
    top = top & ~(int64_t)1;
    right = (right + 1) & ~(int64_t)1;
    bottom = (bottom + 1) & ~(int64_t)1;

    // The scissor rectangle is still in pixel coordinates, convert it into sub-pixel coordinates
    int64_t left_sp = left << RASTERIZER_SUBPIXEL_BITS;
    int64_t top_sp = top << RASTERIZER_SUBPIXEL_BITS;
    int64_t right_sp = right << RASTERIZER_SUBPIXEL_BITS;
    int64_t bottom_sp = bottom << RASTERIZER_SUBPIXEL_BITS;

    // Location where we go "Pen down" - convenience short-hand.
    // (1 << (RASTERIZER_SUBPIXEL_BITS-1)) gets us to put the pen at the center of the pixel,
    // rather than the top-left corner of it.
    int64_t Px = (1 << (RASTERIZER_SUBPIXEL_BITS-1)) + left_sp;
    int64_t Py = (1 << (RASTERIZER_SUBPIXEL_BITS-1)) + top_sp;

    // D012 = determinant of x and y coordinates (this is twice the area of the triangle (e.g. the area of the paralellogram))
    //        | x0 y0 1 |
    // D012 = | x1 y1 1 |
    //        | x2 y2 1 |
    D012 = ((int64_t)px1) * ((int64_t)py2) - ((int64_t)px2) * ((int64_t)py1) - ((int64_t)px0) * ((int64_t)py2) + ((int64_t)px2) * ((int64_t)py0) + ((int64_t)px0) * ((int64_t)py1) - ((int64_t)px1) * ((int64_t)py0);

    if (D012 < 0) {
      // Counterclockwise, check if permitted and convert to clockwise if not
      if (permitted_orientations & RASTERIZER_COUNTERCLOCKWISE) {
        orientation = RASTERIZER_COUNTERCLOCKWISE;
        D012 = -D012;
      }
      else {
        // Counterclockwise not permitted, reject
        return 0;
      }
    }
    else if (D012 > 0) {
      // Clockwise, check if permitted.
      if (permitted_orientations & RASTERIZER_CLOCKWISE) {
        // Adopt xyz012 as is.
        orientation = RASTERIZER_CLOCKWISE;
      }
      else {
        // Clockwise not permitted, reject.
      }
    }
    else {
      // Colinear, reject.
      return 0;
    }

    x0 = px0;
    y0 = py0;
    z0 = pz0;
    x1 = px1;
    y1 = py1;
    z1 = pz1;
    x2 = px2;
    y2 = py2;
    z2 = pz2;

    if (fragbf->num_rows_ && fragbf->fragment_orientation_ != orientation) {
      /* Mismatch of triangle orientation with other existing fragments in the buffer.
       * Clear out the buffer before continuing. */
      return orientation; /* resume at start is fine */
    }
    fragbf->fragment_orientation_ = orientation;

    // Dzx = determinant of z and y coordinates (change in numerator for each successive column (x))
    //       | z0 y0 1 |
    // Dzx = | z1 y1 1 |
    //       | z2 y2 1 |
    int64_t Dzx_1 =  ((int64_t)z1) * ((int64_t)y2) /* 30 + 32 = 62 bits */;
    int64_t Dzx_2 = -((int64_t)z2) * ((int64_t)y1) /* 30 + 32 = 62 bits */;
    int64_t Dzx_3 = -((int64_t)z0) * ((int64_t)y2) /* 30 + 32 = 62 bits */;
    int64_t Dzx_4 =  ((int64_t)z2) * ((int64_t)y0) /* 30 + 32 = 62 bits */;
    int64_t Dzx_5 =  ((int64_t)z0) * ((int64_t)y1) /* 30 + 32 = 62 bits */;
    int64_t Dzx_6 = -((int64_t)z1) * ((int64_t)y0) /* 30 + 32 = 62 bits */;

    int64_t Dzx_a = Dzx_1 + Dzx_2 + Dzx_3 /* 62 + 62 + 62 = 64 bits */;
    int64_t Dzx_b = Dzx_4 + Dzx_5 + Dzx_6 /* 62 + 62 + 62 = 64 bits */;
    int64_t Dzx_lo_sp, Dzx_hi_sp;
    adds128(Dzx_a>>63, Dzx_a, Dzx_b>>63, Dzx_b, &Dzx_hi_sp, &Dzx_lo_sp);

    int64_t Dzx_Px_a_hi, Dzx_Px_a_lo;
    int64_t Dzx_Px_b_hi, Dzx_Px_b_lo;
    muls64(Dzx_a, Px, &Dzx_Px_a_hi, &Dzx_Px_a_lo);
    muls64(Dzx_b, Px, &Dzx_Px_b_hi, &Dzx_Px_b_lo);
    int64_t Dzx_Px_hi, Dzx_Px_lo;
    adds128(Dzx_Px_a_hi, Dzx_Px_a_lo, Dzx_Px_b_hi, Dzx_Px_b_lo, &Dzx_Px_hi, &Dzx_Px_lo);

    if (orientation == RASTERIZER_COUNTERCLOCKWISE) {
      /* Flip signs so all are consistent after counter-clockwise correction earlier */
      subs128(0, 0, Dzx_Px_hi, Dzx_Px_lo, &Dzx_Px_hi, &Dzx_Px_lo);
      subs128(0, 0, Dzx_hi_sp, Dzx_lo_sp, &Dzx_hi_sp, &Dzx_lo_sp);
    }

    // Dzy = determinant of x and z coordinates (change in numerator for each successive row (y))
    //       | x0 z0 1 |
    // Dzy = | x1 z1 1 |
    //       | x2 z2 1 |
    int64_t Dzy_1 =  ((int64_t)x1) * ((int64_t)z2) /* 30 + 32 = 62 bits */;
    int64_t Dzy_2 = -((int64_t)x2) * ((int64_t)z1) /* 30 + 32 = 62 bits */;
    int64_t Dzy_3 = -((int64_t)x0) * ((int64_t)z2) /* 30 + 32 = 62 bits */;
    int64_t Dzy_4 =  ((int64_t)x2) * ((int64_t)z0) /* 30 + 32 = 62 bits */;
    int64_t Dzy_5 =  ((int64_t)x0) * ((int64_t)z1) /* 30 + 32 = 62 bits */;
    int64_t Dzy_6 = -((int64_t)x1) * ((int64_t)z0) /* 30 + 32 = 62 bits */;

    int64_t Dzy_a = Dzy_1 + Dzy_2 + Dzy_3 /* 62 + 62 + 62 = 64 bits */;
    int64_t Dzy_b = Dzy_4 + Dzy_5 + Dzy_6 /* 62 + 62 + 62 = 64 bits */;
    int64_t Dzy_lo_sp, Dzy_hi_sp;               /* 64 + 64 = 65 bits */
    adds128(Dzy_a>>63, Dzy_a, Dzy_b>>63, Dzy_b, &Dzy_hi_sp, &Dzy_lo_sp);

    int64_t Dzy_Py_a_hi, Dzy_Py_a_lo;
    int64_t Dzy_Py_b_hi, Dzy_Py_b_lo;
    muls64(Dzy_a, Py, &Dzy_Py_a_hi, &Dzy_Py_a_lo);
    muls64(Dzy_b, Py, &Dzy_Py_b_hi, &Dzy_Py_b_lo);
    int64_t Dzy_Py_hi, Dzy_Py_lo;
    adds128(Dzy_Py_a_hi, Dzy_Py_a_lo, Dzy_Py_b_hi, Dzy_Py_b_lo, &Dzy_Py_hi, &Dzy_Py_lo);

    if (orientation == RASTERIZER_COUNTERCLOCKWISE) {
      /* ... continue to flip signs after counter-clockwise correction ... */
      subs128(0, 0, Dzy_Py_hi, Dzy_Py_lo, &Dzy_Py_hi, &Dzy_Py_lo);
      subs128(0, 0, Dzy_hi_sp, Dzy_lo_sp, &Dzy_hi_sp, &Dzy_lo_sp);
    }

    // Dxyz = determinant of x, y and z-buffer coordinates
    //        | x0 y0 z0 |
    // Dxyz = | x1 y1 z1 |
    //        | x2 y2 z2 |
    int64_t Dxyz_x0_hi, Dxyz_x0_lo;
    int64_t Dxyz_x1_hi, Dxyz_x1_lo;
    int64_t Dxyz_x2_hi, Dxyz_x2_lo;
    muls64(x0, ((int64_t)y1) * ((int64_t)z2) - ((int64_t)y2) * ((int64_t)z1), &Dxyz_x0_hi, &Dxyz_x0_lo);
    muls64(x1, ((int64_t)y0) * ((int64_t)z2) - ((int64_t)y2) * ((int64_t)z0), &Dxyz_x1_hi, &Dxyz_x1_lo);
    muls64(x2, ((int64_t)y0) * ((int64_t)z1) - ((int64_t)y1) * ((int64_t)z0), &Dxyz_x2_hi, &Dxyz_x2_lo);

    int64_t Dxyz_hi = Dxyz_x0_hi;
    int64_t Dxyz_lo = Dxyz_x0_lo;
    subs128(Dxyz_hi, Dxyz_lo, Dxyz_x1_hi, Dxyz_x1_lo, &Dxyz_hi, &Dxyz_lo);
    adds128(Dxyz_hi, Dxyz_lo, Dxyz_x2_hi, Dxyz_x2_lo, &Dxyz_hi, &Dxyz_lo);
    // Dxyz_hi/lo now holds 95 bit Dxyz value.

    if (orientation == RASTERIZER_COUNTERCLOCKWISE) {
      /* ... continue, still, to flip more signs after counter-clockwise correction ... */
      subs128(0, 0, Dxyz_hi, Dxyz_lo, &Dxyz_hi, &Dxyz_lo);
    }

    // z_num = Dzx * Px + Dzy * Py + Dxyz
    // we have the first two terms as Dzx_Px_hi/lo and Dzy_Py_hi/lo, and have Dzxy_hi/lo, but need to add them all together.
    int64_t z_num_hi, z_num_lo;
    adds128(Dzx_Px_hi, Dzx_Px_lo, Dzy_Py_hi, Dzy_Py_lo, &z_num_hi, &z_num_lo);
    adds128(z_num_hi, z_num_lo, Dxyz_hi, Dxyz_lo, &z_num_hi, &z_num_lo);

    /* Ensure the Z-Buffer value rounds to nearest, rather than truncating. To do this, we would like to add half
     * denominator to the numerator (which has the effect of rounding to nearest.) A problem with this is we don't
     * know if the denominator (D012 in this case) is an odd number, if it is, then halving it would create a
     * round-off error as we dispose of the least significant bit.
     * To solve this, we double everything, D012, all the numerators, and all the numerator increments. Then, we
     * add "half double D012" (i.e. just the original D012) to the numerator and we're done.
     * D012, before doubling, uses 2n+3 = 2*30+3 = 63 bits, so we have space for the additional bit. */
    /* double, and add D012 */
    adds128(z_num_hi, z_num_lo, z_num_hi, z_num_lo, &z_num_hi, &z_num_lo); /* z_num = z_num * 2 */
    adds128(       0,     D012, z_num_hi, z_num_lo, &z_num_hi, &z_num_lo);            /* z_num += D012 */
    D012 += D012;
    adds128(Dzy_hi_sp, Dzy_lo_sp, Dzy_hi_sp, Dzy_lo_sp, &Dzy_hi_sp, &Dzy_lo_sp);  /* Dzy = Dzy * 2 */
    adds128(Dzx_hi_sp, Dzx_lo_sp, Dzx_hi_sp, Dzx_lo_sp, &Dzx_hi_sp, &Dzx_lo_sp);  /* Dzx = Dzx * 2 */

    // We'd like to divide z_num by D012, and take its modulo D012.
    // Do the division first, then multiply back out to get the modulo.
    int64_t z_hi, z_lo;
    int64_t z_mod;
    // Note that division by zero should be impossible as we already checked for D012 <= 0 earlier.
    int r = divmods128by64(z_num_hi, z_num_lo, D012, &z_hi, &z_lo, &z_mod);
    if (r) {
      // Result doesn't fit in 64 bits, this is possible, and we will ignore the high bits. Ignoring
      // the high-bits _is fine_. The reason this works is because we're only interested in the z-buffer
      // values when they are in the range of the triangle, in that range, the values (through possibly
      // many incremental, overflowing, steps) will always be in the range of 0 to Z-Buffer max (e.g. 
      // 0xFFFFFFFF if 32 bits, 0xFFFFFF if 24 bits, and so on.) The one exception to this is when we 
      // start processing fragments in quadruples, in which case some of the fragments will be outside 
      // the triangle (and we would record the z-buffer value and start processing the fragment as-if 
      // it had passed for sake of the fragments that did, but would not use the z-buffer value itself 
      // as part of the z-buffer test.)
      ;
    }

    z_TL = z_lo;

    // Will contain the maximum absolute gradient of z in either x or y directions, per pixel step.
    // This is used for computing the polygon offset. The value is always positive.
    int64_t Dz_gradient_hi, Dz_gradient_lo;

    // outer loop is rows so we start with initialization for Y.
    int64_t Dzy_hi, Dzy_lo;
    muls128(Dzy_hi_sp, Dzy_lo_sp, 0, 1 << RASTERIZER_SUBPIXEL_BITS, &Dzy_hi, &Dzy_lo);

    direction_xy_flips;
    divmods128by64(Dzy_hi, Dzy_lo, D012, NULL, &z_yq, &z_yp);
    if ((Dzy_hi > 0) || ((Dzy_hi == 0) && (Dzy_lo > 0))) {
      z_s_TL = D012 - z_mod - 1;
      z_yi = 1;
      direction_xy_flips = 1;
      Dz_gradient_hi = Dzy_hi;
      Dz_gradient_lo = Dzy_lo;
    }
    else if (Dzy_hi < 0) {
      z_s_TL = z_mod;
      z_yi = -1;
      z_yp = D012 - z_yp - 1;
      direction_xy_flips = 0;

      subs128(0, 0, Dzy_hi, Dzy_lo, &Dz_gradient_hi, &Dz_gradient_lo);
    }
    else /* (Dzy_hi == 0 && Dzy_lo == 0) */ {
      z_s_TL = z_mod;
      z_yi = 0;
      direction_xy_flips = 0;
      z_yp = 0;

      Dz_gradient_hi = Dz_gradient_lo = 0;
    }

    int64_t Dzx_hi, Dzx_lo;
    muls128(Dzx_hi_sp, Dzx_lo_sp, 0, 1 << RASTERIZER_SUBPIXEL_BITS, &Dzx_hi, &Dzx_lo);

    divmods128by64(Dzx_hi, Dzx_lo, D012, NULL, &z_xq, &z_xp);
    if ((Dzx_hi > 0) || ((Dzx_hi == 0) && (Dzx_lo > 0))) {
      z_xi = 1;
      direction_xy_flips ^= 1;

      if ((Dzx_hi > Dz_gradient_hi) || ((Dzx_hi == Dz_gradient_hi) && (Dzx_lo > Dz_gradient_lo))) {
        Dz_gradient_hi = Dzx_hi;
        Dz_gradient_lo = Dzx_lo;
      }
    }
    else if (Dzx_hi < 0) {
      z_xi = -1;
      z_xp = D012 - z_xp - 1;

      int64_t nDzx_hi, nDzx_lo;
      subs128(0, 0, Dzx_hi, Dzx_lo, &nDzx_hi, &nDzx_lo);
      if ((nDzx_hi > Dz_gradient_hi) || ((nDzx_hi == Dz_gradient_hi) && (nDzx_lo > Dz_gradient_lo))) {
        Dz_gradient_hi = nDzx_hi;
        Dz_gradient_lo = nDzx_lo;
      }
    }
    else /* (Dzx == 0) */ {
      z_xi = 0;
      z_xp = 0;
    }

    /* Dzx_sp and Dzy_sp needed 65 bits, we used another (max. 8) RASTERIZER_SUBPIXEL_BITS, giving us 72
     * bits used (we're ignoring that we turned this into an absolute value, arguably returning us a bit)
     * For 128 bits, this leaves us 128-72 = 56 bits to play with, let's adopt 8 bits fixed point at 32
     * bits signed total for the offset factor */
#if RASTERIZER_SUBPIXEL_BITS > 8
#error You'll want to verify this logic when upping the bits
#endif
    muls128(Dz_gradient_hi, Dz_gradient_lo, ((int64_t)offset_factor_fixed8) >> 63, (int64_t)offset_factor_fixed8, &Dz_gradient_hi, &Dz_gradient_lo);
    int64_t Dz_gradient_rem;
    divrems128by64(Dz_gradient_hi, Dz_gradient_lo, D012, &Dz_gradient_hi, &Dz_gradient_lo, &Dz_gradient_rem);
    /* We want a "ceil" function effect, negative numbers should round to their next greatest negative number,
     * positive numbers should round to their next greatest positive number. We check if there is a remainder,
     * and, depending on the sign of the gradient, adopt an increment or decrement. */
    if (Dz_gradient_rem) {
      /* Dz_gradient here should be well under 32 bits; we ignore Dz_gradient_hi so, while it is accessible
       * for debugging, for a release build, the compiler can optimize it out. */
      if (Dz_gradient_lo < 0) {
        subs128(Dz_gradient_hi, Dz_gradient_lo, 0, 1, &Dz_gradient_hi, &Dz_gradient_lo);
      }
      else {
        adds128(Dz_gradient_hi, Dz_gradient_lo, 0, 1, &Dz_gradient_hi, &Dz_gradient_lo);
      }
    }

    /* Find correction to 8 bit fixed point */
    int64_t Dz_offset = Dz_gradient_lo + offset_units_fixed8;
    z_offset = (int32_t)((Dz_offset + (1 << 8) - 1) >> 8);

    /* We have z_s_TL (the top-left fragment of our quadruple fragments), now take 1-pixel steps in X and Y
     * directions to find bottom-left (BL), top-right (TR) and bottom-right (BR) fragment numerators and
     * starting positions. */
    int64_t step_mask;
    z_BL = z_TL + z_yq;
    z_s_BL = z_s_TL - z_yp;
    step_mask = z_s_BL >> 63;
    z_s_BL += D012 & step_mask;
    z_BL = z_TL + (z_yi & step_mask);

    if (direction_xy_flips) {
      /* Flip numerator to column (x) direction so we can form TR and BR */
      z_s_TL = D012 - z_s_TL - 1;
      z_s_BL = D012 - z_s_BL - 1;
    }

    z_TR = z_TL + z_xq;
    z_s_TR = z_s_TL - z_xp;
    step_mask = z_s_TR >> 63;
    z_s_TR += D012 & step_mask;
    z_TR = z_TL + (z_xi & step_mask);

    z_BR = z_BL + z_xq;
    z_s_BR = z_s_BL - z_xp;
    step_mask = z_s_BR >> 63;
    z_s_BR += D012 & step_mask;
    z_BR = z_BL + (z_xi & step_mask);

    if (direction_xy_flips) {
      /* Flip numerator back to rows so we can iterate */
      z_s_TL = D012 - z_s_TL - 1;
      z_s_TR = D012 - z_s_TR - 1;
      z_s_BL = D012 - z_s_BL - 1;
      z_s_BR = D012 - z_s_BR - 1;
    }

    /* Convert z_yq z_yp and z_xq z_xp to take a double pixel step
     * (given that, in each dimension, we process two pixels at a time, we need
     * to double our step.) */
    z_yp += z_yp;
    z_xp += z_xp;
    if (z_yp >= D012) {
      z_yp -= D012;
      if (z_yq < 0) z_yq--;
      else if (z_yq > 0) z_yq++;
    }
    if (z_xp >= D012) {
      z_xp -= D012;
      if (z_xq < 0) z_xq--;
      else if (z_xq > 0) z_xq++;
    }


    int64_t Dp01_row, Dp12_row, Dp20_row;
    // Dp01 = determinant for triangle formed by edge 01 and point p:
    //        | px py 1 |
    // Dp01 = | x0 y0 1 |
    //        | x1 y1 1 |
    int64_t Dp01_dx_sp = (y0 - y1);
    int64_t Dp01_dy_sp = (x1 - x0);
    Dp01_row = Px * Dp01_dx_sp + Py * Dp01_dy_sp + ((int64_t)x0) * ((int64_t)y1) - ((int64_t)y0) * ((int64_t)x1);


    // Dp12 = determinant for triangle formed by edge 12 and point p:
    //        | px py 1 |
    // Dp12 = | x1 y1 1 |
    //        | x2 y2 1 |
    int64_t Dp12_dx_sp = (y1 - y2);
    int64_t Dp12_dy_sp = (x2 - x1);
    Dp12_row = Px * Dp12_dx_sp + Py * Dp12_dy_sp + ((int64_t)x1) * ((int64_t)y2) - ((int64_t)y1) * ((int64_t)x2);

    // Dp20 = determinant for triangle formed by edge 20 and point p:
    //        | px py 1 |
    // Dp20 = | x2 y2 1 |
    //        | x0 y0 1 |
    int64_t Dp20_dx_sp = (y2 - y0);
    int64_t Dp20_dy_sp = (x0 - x2);
    Dp20_row = Px * Dp20_dx_sp + Py * Dp20_dy_sp + ((int64_t)x2) * ((int64_t)y0) - ((int64_t)y2) * ((int64_t)x0);

    if (orientation == RASTERIZER_COUNTERCLOCKWISE) {
      Dp01_dx_sp = -Dp01_dx_sp;
      Dp01_dy_sp = -Dp01_dy_sp;

      Dp12_dx_sp = -Dp12_dx_sp;
      Dp12_dy_sp = -Dp12_dy_sp;

      Dp20_dx_sp = -Dp20_dx_sp;
      Dp20_dy_sp = -Dp20_dy_sp;

      Dp01_row = -Dp01_row;
      Dp12_row = -Dp12_row;
      Dp20_row = -Dp20_row;
    }

    // Compute the stepping variables for whole pixels (how much to step when
    // we're at one pixel, and would like to skip (1 << RASTERIZER_SUBPIXEL_BITS) sub-pixels
    // to get to the same subpixel one whole pixel on whatever stepping dim.)
    int64_t Dp01_dx = Dp01_dx_sp << RASTERIZER_SUBPIXEL_BITS;
    int64_t Dp01_dy = Dp01_dy_sp << RASTERIZER_SUBPIXEL_BITS;
    int64_t Dp12_dx = Dp12_dx_sp << RASTERIZER_SUBPIXEL_BITS;
    int64_t Dp12_dy = Dp12_dy_sp << RASTERIZER_SUBPIXEL_BITS;
    int64_t Dp20_dx = Dp20_dx_sp << RASTERIZER_SUBPIXEL_BITS;
    int64_t Dp20_dy = Dp20_dy_sp << RASTERIZER_SUBPIXEL_BITS;

    // Compute the stepping variables for stepping 2 pixels at a time. This is
    // trivial but something we explicitly want outside the loop.
    Dp01_dx_2 = Dp01_dx << 1;
    Dp01_dy_2 = Dp01_dy << 1;
    Dp12_dx_2 = Dp12_dx << 1;
    Dp12_dy_2 = Dp12_dy << 1;
    Dp20_dx_2 = Dp20_dx << 1;
    Dp20_dy_2 = Dp20_dy << 1;

    // Now compute row values for each of the four pixels.
    Dp01_row_TL = Dp01_row;
    Dp01_row_TR = Dp01_row + Dp01_dx;
    Dp01_row_BL = Dp01_row + Dp01_dy;
    Dp01_row_BR = Dp01_row + Dp01_dx + Dp01_dy;
    Dp12_row_TL = Dp12_row;
    Dp12_row_TR = Dp12_row + Dp12_dx;
    Dp12_row_BL = Dp12_row + Dp12_dy;
    Dp12_row_BR = Dp12_row + Dp12_dx + Dp12_dy;
    Dp20_row_TL = Dp20_row;
    Dp20_row_TR = Dp20_row + Dp20_dx;
    Dp20_row_BL = Dp20_row + Dp20_dy;
    Dp20_row_BR = Dp20_row + Dp20_dx + Dp20_dy;

    // Classify the edges so we know when to apply >0 and >=0 depending on whether
    // the edge is inclusive on an exact match, or not. (Thus avoiding overdraw.)
    // For this we apply the "top-left" rule: if an edge is horizontal, then a pixel
    // that is exactly on the edge is considered to be "inside" the triangle if the
    // edge is at the top of the triangle. Otherwise, if the edge is not horizontal,
    // it is considered to be "inside" the triangle if the edge is to the left of the
    // triangle. We check whether or not the edge is "at the top" or "on the left" on
    // the basis of the triangle vertices being clockwise.
    // 
    // Classify 01
    if (y0 == y1) {
      if (x0 >= x1) {
        /* Horizontal edge at the bottom of the triangle, exclude it */
        Dp01_row_TL--;
        Dp01_row_TR--;
        Dp01_row_BL--;
        Dp01_row_BR--;
      }
    }
    else if (y0 < y1) {
      /* Non-horizontal edge on the right side of the triangle, exclude it */
      Dp01_row_TL--;
      Dp01_row_TR--;
      Dp01_row_BL--;
      Dp01_row_BR--;
    }

    // Classify 12
    if (y1 == y2) {
      if (x1 >= x2) {
        /* Horizontal edge at the bottom of the triangle, exclude it */
        Dp12_row_TL--;
        Dp12_row_TR--;
        Dp12_row_BL--;
        Dp12_row_BR--;
      }
    }
    else if (y1 < y2) {
      /* Non-horizontal edge on the right side of the triangle, exclude it */
      Dp12_row_TL--;
      Dp12_row_TR--;
      Dp12_row_BL--;
      Dp12_row_BR--;
    }

    // Classify 20
    if (y2 == y0) {
      if (x2 >= x0) {
        /* Horizontal edge at the bottom of the triangle, exclude it */
        Dp20_row_TL--;
        Dp20_row_TR--;
        Dp20_row_BL--;
        Dp20_row_BR--;
      }
    }
    else if (y2 < y0) {
      /* Non-horizontal edge on the right side of the triangle, exclude it */
      Dp20_row_TL--;
      Dp20_row_TR--;
      Dp20_row_BL--;
      Dp20_row_BR--;
    }

    pixel_TL = rgba + top * stride + left * 4;
    pixel_TR = pixel_TL + 4;
    pixel_BL = pixel_TL + stride;
    pixel_BR = pixel_BL + 4;
    pixel_mod = 2 * stride - (right - left) * 4; // bltdmod ;-) Keep in mind we go forward not 1, but 2 rows.
    zbuf_TL = zbuf + top * zstride + left * zstep;
    zbuf_TR = zbuf_TL + zstep;
    zbuf_BL = zbuf_TL + zstride;
    zbuf_BR = zbuf_BL + zstep;
    zbuf_mod = 2 * zstride - (right - left) * zstep;
    stencil_TL = stencilbuf + top * stencil_stride + left * stencil_step;
    stencil_TR = stencil_TL + stencil_step;
    stencil_BL = stencil_TL + stencil_stride;
    stencil_BR = stencil_BL + stencil_step;
    stencil_mod = 2 * stencil_stride - (right - left) * stencil_step;

    switch (early_z_zbuf_func) {
      case REZF_NEVER: {
        /* nothing to do here.. we'll pass through here so we follow one and
         * the same code path in case there are already fragments */
        break;
      }
      case REZF_LESS: {
        switch (zstep) {
          case 2: {
#define RASTERIZER_RESUME_CONDITION 1
#define RASTERIZER_RESUME_LABEL ez_less16
#define RASTERIZER_EARLY_Z_CHECK(zbuf_ptr, zbuf_val) (uint8_t)((((int32_t)(zbuf_val)) - (int32_t)(*(uint16_t *)(zbuf_ptr))) >> 31)
#include "rasterizer_core_inc.h"
#undef RASTERIZER_EARLY_Z_CHECK
#undef RASTERIZER_RESUME_CONDITION
#undef RASTERIZER_RESUME_LABEL
            break;
          }
          case 3: {
#define RASTERIZER_RESUME_CONDITION 2
#define RASTERIZER_RESUME_LABEL ez_less24
#define RASTERIZER_EARLY_Z_CHECK(zbuf_ptr, zbuf_val) (uint8_t)((((int32_t)(zbuf_val)) - ((((int32_t)*(uint8_t *)(zbuf_ptr)) << 16) | (((int32_t)*(uint8_t *)(zbuf_ptr + 1)) << 8) | ((int32_t)*(uint8_t *)(zbuf_ptr + 2)))) >> 31)
#include "rasterizer_core_inc.h"
#undef RASTERIZER_EARLY_Z_CHECK
#undef RASTERIZER_RESUME_CONDITION
#undef RASTERIZER_RESUME_LABEL
            break;
          }
          case 4: {
#define RASTERIZER_RESUME_CONDITION 3
#define RASTERIZER_RESUME_LABEL ez_less32
#define RASTERIZER_EARLY_Z_CHECK(zbuf_ptr, zbuf_val) (uint8_t)((((int64_t)(zbuf_val)) - (int64_t)(*(uint32_t *)(zbuf_ptr))) >> 63)
#include "rasterizer_core_inc.h"
#undef RASTERIZER_EARLY_Z_CHECK
#undef RASTERIZER_RESUME_CONDITION
#undef RASTERIZER_RESUME_LABEL
            break;
          }
        }
        break;
      }
      case REZF_LEQUAL: {
        switch (zstep) {
          case 2: {
#define RASTERIZER_RESUME_CONDITION 4
#define RASTERIZER_RESUME_LABEL ez_lequal16
#define RASTERIZER_EARLY_Z_CHECK(zbuf_ptr, zbuf_val) (int64_t)((((int32_t)(zbuf_val)) - ((int32_t)(*(uint16_t *)(zbuf_ptr))) - 1) >> 31)
#include "rasterizer_core_inc.h"
#undef RASTERIZER_EARLY_Z_CHECK
#undef RASTERIZER_RESUME_CONDITION
#undef RASTERIZER_RESUME_LABEL
            break;
          }
          case 3: {
#define RASTERIZER_RESUME_CONDITION 5
#define RASTERIZER_RESUME_LABEL ez_lequal24
#define RASTERIZER_EARLY_Z_CHECK(zbuf_ptr, zbuf_val) (int64_t)((((int32_t)(zbuf_val)) - (((((int32_t)*(uint8_t *)(zbuf_ptr)) << 16) | (((int32_t)*(uint8_t *)(zbuf_ptr + 1)) << 8) | ((int32_t)*(uint8_t *)(zbuf_ptr + 2)))) - 1) >> 31)
#include "rasterizer_core_inc.h"
#undef RASTERIZER_EARLY_Z_CHECK
#undef RASTERIZER_RESUME_CONDITION
#undef RASTERIZER_RESUME_LABEL
            break;
          }
          case 4: {
#define RASTERIZER_RESUME_CONDITION 6
#define RASTERIZER_RESUME_LABEL ez_lequal32
#define RASTERIZER_EARLY_Z_CHECK(zbuf_ptr, zbuf_val) (int64_t)((((int64_t)(zbuf_val)) - ((int64_t)(*(uint32_t *)(zbuf_ptr))) - 1) >> 63)
#include "rasterizer_core_inc.h"
#undef RASTERIZER_EARLY_Z_CHECK
#undef RASTERIZER_RESUME_CONDITION
#undef RASTERIZER_RESUME_LABEL
            break;
          }
        }
        break;
      }
      case REZF_EQUAL:
      case REZF_GREATER:
      case REZF_NOTEQUAL:
      case REZF_GEQUAL:
      case REZF_ALWAYS: {
#define RASTERIZER_RESUME_CONDITION 7
#define RASTERIZER_RESUME_LABEL ez_always
#define RASTERIZER_EARLY_Z_CHECK(zbuf_ptr, zbuf_val) (~(int64_t)0)
#include "rasterizer_core_inc.h"
#undef RASTERIZER_EARLY_Z_CHECK
#undef RASTERIZER_RESUME_CONDITION
#undef RASTERIZER_RESUME_LABEL
        ;
      }
    }

    if (fragbf->num_rows_) {
      rasterizer->resume_at_ = __LINE__ + 2;
      return orientation;
      case __LINE__: ;
    }
    rasterizer->resume_at_ = 0;
    return 0;
    default:
      return -1;
  }
}
