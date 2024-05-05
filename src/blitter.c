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

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef BLITTER_H_INCLUDED
#define BLITTER_H_INCLUDED
#include "blitter.h"
#endif

static int bitcount(uint32_t x) {
  int l0 = ((x & 0xAAAAAAAA) >> 1) + (x & 0x55555555);
  int l1 = ((l0 & 0xCCCCCCCC) >> 2) + (l0 & 0x33333333);
  int l2 = ((l1 & 0xF0F0F0F0) >> 4) + (l1 & 0x0F0F0F0F);
  int l3 = ((l2 & 0xFF00FF00) >> 8) + (l2 & 0x00FF00FF);
  int l4 = ((l3 & 0xFFFF0000) >> 16) + (l3 & 0x0000FFFF);
  return l4;
}

static uint32_t check_alignment(uint32_t v) {
  /* Idea here: if a bit is set, then there is no alignment.
   * So, e.g. if bit 0 is set, then there is no 2-byte alignment,
   *          if bit 1 is set, then there is no 4-byte alignment,
   *          if bit 2 is set, then there is no 8-byte alignment,
   * and so on. Note that if, in the source, bit 0 is set, then that
   * implies that, not only is there no 2-byte alignment, but there is
   * also no 4, 8, 16, 32, and so on, alignment. */
  v = v | (v << 1);
  v = v | (v << 2);
  v = v | (v << 4);
  v = v | (v << 8);
  v = v | (v << 16);

  /* Let's count the number of 0 bits; note that we went all the way
   * up to 32 bits in our bit propagation above, so consequently,
   * if we invert all bits, then all the bits will be to the right (low end)
   * from the LSB up and be consecutive. If we count those bits, we
   * have the alignment of the input value:
   * 0 = 1 byte alignment (no alignment, no bits were set),
   * 1 = 2 byte alignment
   * 2 = 4 byte alignment
   * 3 = 8 byte alignment.
   * ... and so on. */

  return bitcount(~v);
}

void blitter_blit(void *dst, const void *src, size_t dst_stride, size_t src_stride, size_t width, size_t height, enum blitter_components format, enum blitter_data_type src_type) {
  uintptr_t dst_uint = (uintptr_t)dst;
  uintptr_t src_uint = (uintptr_t)src;
  /* Check alignment */
  uint32_t dst_alignment = check_alignment((uint32_t)dst_uint);
  uint32_t src_alignment = check_alignment((uint32_t)src_uint);
  uint32_t src_stride_alignment = check_alignment((uint32_t)src_stride);
  uint32_t dst_stride_alignment = check_alignment((uint32_t)dst_stride);

  if (dst_stride_alignment < dst_alignment) dst_alignment = dst_stride_alignment;
  if (src_stride_alignment < src_alignment) src_alignment = src_stride_alignment;

  uint32_t alignment = src_alignment;
  if (dst_alignment < alignment) alignment = dst_alignment;

  size_t bytes_per_src_pixel = 1;
  switch (src_type) {
    case blit_unsigned_byte:
      switch (format) {
        case blit_alpha:
          bytes_per_src_pixel = 1;
          break;
        case blit_luminance:
          bytes_per_src_pixel = 1;
          break;
        case blit_luminance_alpha:
          bytes_per_src_pixel = 2;
          break;
        case blit_rgb:
          bytes_per_src_pixel = 3;
          break;
        case blit_rgba:
          bytes_per_src_pixel = 4;
          break;
      }
      break;
    case blit_unsigned_short_565:
    case blit_unsigned_short_4444:
    case blit_unsigned_short_5551:
      bytes_per_src_pixel = 2;
      break;
  }

  size_t bytes_per_src_row = width * bytes_per_src_pixel;

  size_t bytes_per_dst_pixel = 1;
  switch (format) {
    case blit_alpha:
      bytes_per_dst_pixel = 1;
      break;
    case blit_luminance:
      bytes_per_dst_pixel = 1;
      break;
    case blit_luminance_alpha:
      bytes_per_dst_pixel = 2;
      break;
    case blit_rgb:
      bytes_per_dst_pixel = 3;
      break;
    case blit_rgba:
      bytes_per_dst_pixel = 4;
      break;
  }

  size_t bytes_per_dst_row = width * bytes_per_dst_pixel;

  uint32_t src_width_byte_alignment = check_alignment((uint32_t)bytes_per_src_row);
  uint32_t dst_width_byte_alignment = check_alignment((uint32_t)bytes_per_dst_row);

  if (src_width_byte_alignment < alignment) alignment = src_width_byte_alignment;
  if (dst_width_byte_alignment < alignment) alignment = dst_width_byte_alignment;

  uint32_t max_width_cluster = check_alignment((uint32_t)width);

  /* alignment now holds the byte alignment value when source, destination, and both their strides,
   * and the width to be processed in source and destination pixel formats, are taken into account. */


  if (alignment == 1) {
    uint8_t *restrict dp = (uint8_t * restrict)dst;
    uint8_t *restrict sp = (uint8_t * restrict)src;
    uint8_t *restrict dpc = dp;
    uint8_t *restrict spc = sp;

    /* per-byte access necessary; this takes care of any misaligned memory access CPU architectures but
     * is slow. */
    size_t row, col;
    switch (src_type) {
      case blit_unsigned_byte:
        switch (format) {
          case blit_alpha:
          case blit_luminance:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                *dpc++ = *spc++;
              }
            }
            break;
          case blit_luminance_alpha:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                *dpc++ = *spc++;
                *dpc++ = *spc++;
              }
            }
            break;
          case blit_rgb:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                *dpc++ = *spc++;
                *dpc++ = *spc++;
                *dpc++ = *spc++;
              }
            }
            break;
          case blit_rgba:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                *dpc++ = *spc++;
                *dpc++ = *spc++;
                *dpc++ = *spc++;
                *dpc++ = *spc++;
              }
            }
            break;
        }
        break;
      case blit_unsigned_short_565: {
        switch (format) {
          case blit_alpha:
            /* The 565 format has no alpha channel, therefore make it an implied full-alpha */
            for (row = 0; row < height; ++row) {
              dpc = dp;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                *dpc++ = 0xFF;
              }
            }
            break;
          case blit_luminance:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                /* We're going byte-by-byte but want to pick this up
                 * in the endianness of the architecture we're running on. */
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;
                /* Luminance is in the R channel */
                uint16_t lum_masked = src_pixel & 0xF800;
                uint8_t luminance = (uint8_t)((lum_masked >> 8) | (lum_masked >> 13));
                *dpc++ = luminance;
              }
            }
            break;
          case blit_luminance_alpha:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;
                /* Luminance is in the R channel */
                uint16_t lum_masked = src_pixel & 0xF800;
                uint8_t luminance = (uint8_t)((lum_masked >> 8) | (lum_masked >> 13));
                *dpc++ = luminance;
                *dpc++ = 0xFF; /* no alpha channel, ergo always full */
              }
            }
            break;
          case blit_rgb:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;

                uint16_t red_masked = src_pixel & 0xF800;
                uint8_t red = (uint8_t)((red_masked >> 8) | (red_masked >> 13));
                uint16_t green_masked = src_pixel & 0x07E0;
                uint8_t green = (uint8_t)((green_masked >> 3) | (green_masked >> 9));
                uint16_t blue_masked = src_pixel & 0x001F;
                uint8_t blue = (uint8_t)((blue_masked >> 3) | (blue_masked >> 2));
                *dpc++ = red;
                *dpc++ = green;
                *dpc++ = blue;
              }
            }
            break;
          case blit_rgba:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;

                uint16_t red_masked = src_pixel & 0xF800;
                uint8_t red = (uint8_t)((red_masked >> 8) | (red_masked >> 13));
                uint16_t green_masked = src_pixel & 0x07E0;
                uint8_t green = (uint8_t)((green_masked >> 3) | (green_masked >> 9));
                uint16_t blue_masked = src_pixel & 0x001F;
                uint8_t blue = (uint8_t)((blue_masked >> 3) | (blue_masked >> 2));
                *dpc++ = red;
                *dpc++ = green;
                *dpc++ = blue;
                *dpc++ = 0xFF;
              }
            }
            break;
        }
        break;
      }
      case blit_unsigned_short_4444: {
        switch (format) {
          case blit_alpha:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;
                uint16_t alpha_masked = src_pixel & 0x000F;
                uint8_t alpha = (uint8_t)((alpha_masked) | (alpha_masked << 4));
                *dpc++ = alpha;
              }
            }
            break;
          case blit_luminance:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;
                uint16_t luminance_masked = src_pixel & 0xF000;
                uint8_t luminance = (uint8_t)((luminance_masked >> 8) | (luminance_masked >> 12));
                *dpc++ = luminance;
              }
            }
            break;
          case blit_luminance_alpha:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;
                /* Luminance is in the R channel */
                uint16_t luminance_masked = src_pixel & 0xF000;
                uint8_t luminance = (uint8_t)((luminance_masked >> 8) | (luminance_masked >> 12));
                uint16_t alpha_masked = src_pixel & 0x000F;
                uint8_t alpha = (uint8_t)((alpha_masked) | (alpha_masked << 4));
                *dpc++ = luminance;
                *dpc++ = alpha;
              }
            }
            break;
          case blit_rgb:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;

                uint16_t red_masked = src_pixel & 0xF000;
                uint8_t red = (uint8_t)((red_masked >> 8) | (red_masked >> 12));
                uint16_t green_masked = src_pixel & 0x0F00;
                uint8_t green = (uint8_t)((green_masked >> 4) | (green_masked >> 8));
                uint16_t blue_masked = src_pixel & 0x00F0;
                uint8_t blue = (uint8_t)((blue_masked) | (blue_masked >> 4));
                *dpc++ = red;
                *dpc++ = green;
                *dpc++ = blue;
              }
            }
            break;
          case blit_rgba:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;

                uint16_t red_masked = src_pixel & 0xF000;
                uint8_t red = (uint8_t)((red_masked >> 8) | (red_masked >> 12));
                uint16_t green_masked = src_pixel & 0x0F00;
                uint8_t green = (uint8_t)((green_masked >> 4) | (green_masked >> 8));
                uint16_t blue_masked = src_pixel & 0x00F0;
                uint8_t blue = (uint8_t)((blue_masked) | (blue_masked >> 4));
                uint16_t alpha_masked = src_pixel & 0x000F;
                uint8_t alpha = (uint8_t)((alpha_masked << 4) | (alpha_masked));
                *dpc++ = red;
                *dpc++ = green;
                *dpc++ = blue;
                *dpc++ = alpha;
              }
            }
            break;
        }
        break;
      }
      case blit_unsigned_short_5551: {
        switch (format) {
          case blit_alpha:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;
                uint16_t alpha_masked = src_pixel & 0x0001;
                uint8_t alpha = (uint8_t)(0x100 - alpha_masked);
                *dpc++ = alpha;
              }
            }
            break;
          case blit_luminance:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;
                uint16_t luminance_masked = src_pixel & 0xF800;
                uint8_t luminance = (uint8_t)((luminance_masked >> 8) | (luminance_masked >> 13));
                *dpc++ = luminance;
              }
            }
            break;
          case blit_luminance_alpha:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;
                /* Luminance is in the R channel */
                uint16_t luminance_masked = src_pixel & 0xF800;
                uint8_t luminance = (uint8_t)((luminance_masked >> 8) | (luminance_masked >> 13));
                uint16_t alpha_masked = src_pixel & 0x003E;
                uint8_t alpha = (uint8_t)((alpha_masked << 2) | (alpha_masked >> 3));
                *dpc++ = luminance;
                *dpc++ = alpha;
              }
            }
            break;
          case blit_rgb:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;

                uint16_t red_masked = src_pixel & 0xF800;
                uint8_t red = (uint8_t)((red_masked >> 8) | (red_masked >> 13));
                uint16_t green_masked = src_pixel & 0x07C0;
                uint8_t green = (uint8_t)((green_masked >> 3) | (green_masked >> 8));
                uint16_t blue_masked = src_pixel & 0x003E;
                uint8_t blue = (uint8_t)((blue_masked << 2) | (blue_masked >> 3));
                *dpc++ = red;
                *dpc++ = green;
                *dpc++ = blue;
              }
            }
            break;
          case blit_rgba:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc = sp;
              sp += src_stride;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel;
                uint8_t *src_pixel_b = (uint8_t *)&src_pixel;
                src_pixel_b[0] = *spc++;
                src_pixel_b[1] = *spc++;

                uint16_t red_masked = src_pixel & 0xF800;
                uint8_t red = (uint8_t)((red_masked >> 8) | (red_masked >> 13));
                uint16_t green_masked = src_pixel & 0x07C0;
                uint8_t green = (uint8_t)((green_masked >> 3) | (green_masked >> 8));
                uint16_t blue_masked = src_pixel & 0x003E;
                uint8_t blue = (uint8_t)((blue_masked << 2) | (blue_masked >> 3));
                uint16_t alpha_masked = src_pixel & 0x0001;
                uint8_t alpha = (uint8_t)(0x100 - alpha_masked);
                *dpc++ = red;
                *dpc++ = green;
                *dpc++ = blue;
                *dpc++ = alpha;
              }
            }
            break;
        }
        break;
      }
    }
  }
  else if (alignment >= 2) {
    /* 2-byte aligned, can go 16-bits at a time */
    uint8_t *restrict dp = (uint8_t * restrict)dst;
    uint8_t *restrict sp = (uint8_t * restrict)src;
    uint8_t *restrict dpc = dp;
    uint8_t *restrict spc = sp;

    /* per-byte access necessary; this takes care of any misaligned memory access CPU architectures but
     * is slow. */
    size_t row, col;

    switch (src_type) {
      case blit_unsigned_byte: {
        uint16_t *restrict dp16 = (uint16_t * restrict)dst;
        uint16_t *restrict sp16 = (uint16_t * restrict)src;
        uint16_t *restrict dpc16;
        uint16_t *restrict spc16;
        size_t src_stride_16 = src_stride / sizeof(uint16_t);
        size_t dst_stride_16 = dst_stride / sizeof(uint16_t);
        int width16 = (int)(width/sizeof(uint16_t));
        switch (format) {
          case blit_alpha:
          case blit_luminance:
            for (row = 0; row < height; ++row) {
              dpc16 = dp16;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp16 += dst_stride_16;
              for (col = 0; col < width16; ++col) {
                *dpc16++ = *spc16++;
              }
            }
            break;
          case blit_luminance_alpha:
            for (row = 0; row < height; ++row) {
              dpc16 = dp16;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp16 += dst_stride_16;
              for (col = 0; col < width16; ++col) {
                *dpc16++ = *spc16++;
                *dpc16++ = *spc16++;
              }
            }
            break;
          case blit_rgb:
            for (row = 0; row < height; ++row) {
              dpc16 = dp16;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp16 += dst_stride_16;
              for (col = 0; col < width16; ++col) {
                *dpc16++ = *spc16++;
                *dpc16++ = *spc16++;
                *dpc16++ = *spc16++;
              }
            }
            break;
          case blit_rgba:
            for (row = 0; row < height; ++row) {
              dpc16 = dp16;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp16 += dst_stride_16;
              for (col = 0; col < width16; ++col) {
                *dpc16++ = *spc16++;
                *dpc16++ = *spc16++;
                *dpc16++ = *spc16++;
                *dpc16++ = *spc16++;
              }
            }
            break;
        }
        break;
      }
      case blit_unsigned_short_565: {
        uint16_t *restrict sp16 = (uint16_t * restrict)src;
        uint16_t *restrict spc16;
        size_t src_stride_16 = src_stride / sizeof(uint16_t);

        switch (format) {
          case blit_alpha:
            /* The 565 format has no alpha channel, therefore make it an implied full-alpha */
            for (row = 0; row < height; ++row) {
              dpc = dp;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                *dpc++ = 0xFF;
              }
            }
            break;
          case blit_luminance:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;
                /* Luminance is in the R channel */
                uint16_t lum_masked = src_pixel & 0xF800;
                uint8_t luminance = (uint8_t)((lum_masked >> 8) | (lum_masked >> 13));
                *dpc++ = luminance;
              }
            }
            break;
          case blit_luminance_alpha:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;
                /* Luminance is in the R channel */
                uint16_t lum_masked = src_pixel & 0xF800;
                uint8_t luminance = (uint8_t)((lum_masked >> 8) | (lum_masked >> 13));
                *dpc++ = luminance;
                *dpc++ = 0xFF; /* no alpha channel, ergo always full */
              }
            }
            break;
          case blit_rgb:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;

                uint16_t red_masked = src_pixel & 0xF800;
                uint8_t red = (uint8_t)((red_masked >> 8) | (red_masked >> 13));
                uint16_t green_masked = src_pixel & 0x07E0;
                uint8_t green = (uint8_t)((green_masked >> 3) | (green_masked >> 9));
                uint16_t blue_masked = src_pixel & 0x001F;
                uint8_t blue = (uint8_t)((blue_masked >> 3) | (blue_masked >> 2));
                *dpc++ = red;
                *dpc++ = green;
                *dpc++ = blue;
              }
            }
            break;
          case blit_rgba:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;

                uint16_t red_masked = src_pixel & 0xF800;
                uint8_t red = (uint8_t)((red_masked >> 8) | (red_masked >> 13));
                uint16_t green_masked = src_pixel & 0x07E0;
                uint8_t green = (uint8_t)((green_masked >> 3) | (green_masked >> 9));
                uint16_t blue_masked = src_pixel & 0x001F;
                uint8_t blue = (uint8_t)((blue_masked >> 3) | (blue_masked >> 2));
                *dpc++ = red;
                *dpc++ = green;
                *dpc++ = blue;
                *dpc++ = 0xFF;
              }
            }
            break;
        }
        break;
      }
      case blit_unsigned_short_4444: {
        uint16_t *restrict sp16 = (uint16_t * restrict)src;
        uint16_t *restrict spc16;
        size_t src_stride_16 = src_stride / sizeof(uint16_t);

        switch (format) {
          case blit_alpha:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;
                uint16_t alpha_masked = src_pixel & 0x000F;
                uint8_t alpha = (uint8_t)((alpha_masked) | (alpha_masked << 4));
                *dpc++ = alpha;
              }
            }
            break;
          case blit_luminance:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;
                uint16_t luminance_masked = src_pixel & 0xF000;
                uint8_t luminance = (uint8_t)((luminance_masked >> 8) | (luminance_masked >> 12));
                *dpc++ = luminance;
              }
            }
            break;
          case blit_luminance_alpha:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;
                /* Luminance is in the R channel */
                uint16_t luminance_masked = src_pixel & 0xF000;
                uint8_t luminance = (uint8_t)((luminance_masked >> 8) | (luminance_masked >> 12));
                uint16_t alpha_masked = src_pixel & 0x000F;
                uint8_t alpha = (uint8_t)((alpha_masked) | (alpha_masked << 4));
                *dpc++ = luminance;
                *dpc++ = alpha;
              }
            }
            break;
          case blit_rgb:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;

                uint16_t red_masked = src_pixel & 0xF000;
                uint8_t red = (uint8_t)((red_masked >> 8) | (red_masked >> 12));
                uint16_t green_masked = src_pixel & 0x0F00;
                uint8_t green = (uint8_t)((green_masked >> 4) | (green_masked >> 8));
                uint16_t blue_masked = src_pixel & 0x00F0;
                uint8_t blue = (uint8_t)((blue_masked) | (blue_masked >> 4));
                *dpc++ = red;
                *dpc++ = green;
                *dpc++ = blue;
              }
            }
            break;
          case blit_rgba:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;

                uint16_t red_masked = src_pixel & 0xF000;
                uint8_t red = (uint8_t)((red_masked >> 8) | (red_masked >> 12));
                uint16_t green_masked = src_pixel & 0x0F00;
                uint8_t green = (uint8_t)((green_masked >> 4) | (green_masked >> 8));
                uint16_t blue_masked = src_pixel & 0x00F0;
                uint8_t blue = (uint8_t)((blue_masked) | (blue_masked >> 4));
                uint16_t alpha_masked = src_pixel & 0x000F;
                uint8_t alpha = (uint8_t)((alpha_masked << 4) | (alpha_masked));
                *dpc++ = red;
                *dpc++ = green;
                *dpc++ = blue;
                *dpc++ = alpha;
              }
            }
            break;
        }
        break;
      }
      case blit_unsigned_short_5551: {
        uint16_t *restrict sp16 = (uint16_t * restrict)src;
        uint16_t *restrict spc16;
        size_t src_stride_16 = src_stride / sizeof(uint16_t);

        switch (format) {
          case blit_alpha:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;

                uint16_t alpha_masked = src_pixel & 0x0001;
                uint8_t alpha = (uint8_t)(0x100 - alpha_masked);
                *dpc++ = alpha;
              }
            }
            break;
          case blit_luminance:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;

                uint16_t luminance_masked = src_pixel & 0xF800;
                uint8_t luminance = (uint8_t)((luminance_masked >> 8) | (luminance_masked >> 13));
                *dpc++ = luminance;
              }
            }
            break;
          case blit_luminance_alpha:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;

                /* Luminance is in the R channel */
                uint16_t luminance_masked = src_pixel & 0xF800;
                uint8_t luminance = (uint8_t)((luminance_masked >> 8) | (luminance_masked >> 13));
                uint16_t alpha_masked = src_pixel & 0x003E;
                uint8_t alpha = (uint8_t)((alpha_masked << 2) | (alpha_masked >> 3));
                *dpc++ = luminance;
                *dpc++ = alpha;
              }
            }
            break;
          case blit_rgb:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;

                uint16_t red_masked = src_pixel & 0xF800;
                uint8_t red = (uint8_t)((red_masked >> 8) | (red_masked >> 13));
                uint16_t green_masked = src_pixel & 0x07C0;
                uint8_t green = (uint8_t)((green_masked >> 3) | (green_masked >> 8));
                uint16_t blue_masked = src_pixel & 0x003E;
                uint8_t blue = (uint8_t)((blue_masked << 2) | (blue_masked >> 3));
                *dpc++ = red;
                *dpc++ = green;
                *dpc++ = blue;
              }
            }
            break;
          case blit_rgba:
            for (row = 0; row < height; ++row) {
              dpc = dp;
              spc16 = sp16;
              sp16 += src_stride_16;
              dp += dst_stride;
              for (col = 0; col < width; ++col) {
                uint16_t src_pixel = *spc16++;

                uint16_t red_masked = src_pixel & 0xF800;
                uint8_t red = (uint8_t)((red_masked >> 8) | (red_masked >> 13));
                uint16_t green_masked = src_pixel & 0x07C0;
                uint8_t green = (uint8_t)((green_masked >> 3) | (green_masked >> 8));
                uint16_t blue_masked = src_pixel & 0x003E;
                uint8_t blue = (uint8_t)((blue_masked << 2) | (blue_masked >> 3));
                uint16_t alpha_masked = src_pixel & 0x0001;
                uint8_t alpha = (uint8_t)(0x100 - alpha_masked);
                *dpc++ = red;
                *dpc++ = green;
                *dpc++ = blue;
                *dpc++ = alpha;
              }
            }
            break;
        }
        break;
      }
    }
  }
}

static int blitter_alignment(void *bitmap, size_t stride, int width, size_t bytes_per_pixel) {
  uintptr_t bmp_uint = (uintptr_t)bitmap;
  /* Check alignment */
  uint32_t bmp_alignment = check_alignment((uint32_t)bmp_uint);
  uint32_t stride_alignment = check_alignment((uint32_t)stride);

  if (stride_alignment < bmp_alignment) bmp_alignment = stride_alignment;

  uint32_t alignment = bmp_alignment;

  size_t bytes_per_row = width * bytes_per_pixel;

  uint32_t width_byte_alignment = check_alignment((uint32_t)bytes_per_row);

  if (width_byte_alignment < alignment) alignment = width_byte_alignment;

  uint32_t max_width_cluster = check_alignment((uint32_t)width);

}

static void blitter_blit_apply_mask_fast64(uint64_t * restrict bitmap, size_t stride64,
                                           size_t y, size_t x64,
                                           size_t width64, size_t height,
                                           uint64_t lmask, uint64_t imask, uint64_t rmask, 
                                           uint64_t value) {
  uint64_t * restrict prow = bitmap + stride64 * y + x64;
  size_t row, col;
  value = value & imask;
  if (width64 == 0) {
    uint64_t mask = lmask & imask & rmask;
    uint64_t value_ = value & mask;
    for (row = 0; row < height; ++row) {
      uint64_t *restrict p = prow;
      prow += stride64;

      *p = (*p & ~mask) | value_;
    }
  }
  else if (width64 == 1) {
    uint64_t left_mask = lmask & imask;
    uint64_t right_mask = imask & rmask;
    uint64_t left_value = value & left_mask;
    uint64_t right_value = value & right_mask;
    for (row = 0; row < height; ++row) {
      uint64_t *restrict p = prow;
      prow += stride64;

      p[0] = (p[0] & ~left_mask) | left_value;
      p[1] = (p[1] & ~right_mask) | right_value;
    }
  }
  else {
    uint64_t left_mask = lmask & imask;
    uint64_t right_mask = imask & rmask;
    uint64_t left_value = value & left_mask;
    uint64_t right_value = value & right_mask;
    for (row = 0; row < height; ++row) {
      uint64_t *restrict p = prow;
      prow += stride64;

      *p = (*p & ~left_mask) | left_value;
      p++;
      for (col = 0; col < (width64 - 1); ++col) {
        *p = (*p & ~imask) | value;
        p++;
      }
      *p = (*p & ~right_mask) | right_value;
    }
  }
}


static void blitter_blit_apply_mask_fast192(uint64_t * restrict bitmap, size_t stride64,
                                            size_t y, size_t x64,
                                            size_t width192, size_t height,
                                            uint64_t lmask0, uint64_t lmask1, uint64_t lmask2,
                                            uint64_t imask0, uint64_t imask1, uint64_t imask2,
                                            uint64_t rmask0, uint64_t rmask1, uint64_t rmask2,
                                            uint64_t value0, uint64_t value1, uint64_t value2) {
  uint64_t * restrict prow = bitmap + stride64 * y + x64;
  size_t row, col;
  value0 = value0 & imask0;
  value1 = value1 & imask1;
  value2 = value2 & imask2;
  if (width192 == 0) {
    uint64_t mask0 = lmask0 & imask0 & rmask0;
    uint64_t mask1 = lmask1 & imask1 & rmask1;
    uint64_t mask2 = lmask2 & imask2 & rmask2;
    uint64_t value0_ = value0 & mask0;
    uint64_t value1_ = value1 & mask1;
    uint64_t value2_ = value2 & mask2;
    for (row = 0; row < height; ++row) {
      uint64_t *restrict p = prow;
      prow += stride64;

      p[0] = (p[0] & ~mask0) | value0_;
      p[1] = (p[1] & ~mask1) | value1_;
      p[2] = (p[2] & ~mask2) | value2_;
    }
  }
  else {
    uint64_t left_mask0 = lmask0 & imask0;
    uint64_t left_mask1 = lmask1 & imask1;
    uint64_t left_mask2 = lmask2 & imask2;
    uint64_t right_mask0 = imask0 & rmask0;
    uint64_t right_mask1 = imask1 & rmask1;
    uint64_t right_mask2 = imask2 & rmask2;
    uint64_t left_value0 = value0 & left_mask0;
    uint64_t left_value1 = value1 & left_mask1;
    uint64_t left_value2 = value2 & left_mask2;
    uint64_t right_value0 = value0 & right_mask0;
    uint64_t right_value1 = value1 & right_mask1;
    uint64_t right_value2 = value2 & right_mask2;
    if (width192 == 1) {
      for (row = 0; row < height; ++row) {
        uint64_t *restrict p = prow;
        prow += stride64;

        p[0] = (p[0] & ~left_mask0) | left_value0;
        p[1] = (p[1] & ~left_mask1) | left_value1;
        p[2] = (p[2] & ~left_mask2) | left_value2;
        p[3] = (p[3] & ~right_mask0) | right_value0;
        p[4] = (p[4] & ~right_mask1) | right_value1;
        p[5] = (p[5] & ~right_mask2) | right_value2;
      }
    }
    else {
      for (row = 0; row < height; ++row) {
        uint64_t *restrict p = prow;
        prow += stride64;

        p[0] = (p[0] & ~left_mask0) | left_value0;
        p[1] = (p[1] & ~left_mask1) | left_value1;
        p[2] = (p[2] & ~left_mask2) | left_value2;
        p += 3;
        for (col = 0; col < (width192 - 1); ++col) {
          p[0] = (p[0] & ~imask0) | value0;
          p[1] = (p[1] & ~imask1) | value1;
          p[2] = (p[2] & ~imask2) | value2;
          p += 3;
        }
        p[0] = (p[0] & ~right_mask0) | right_value0;
        p[1] = (p[1] & ~right_mask1) | right_value1;
        p[2] = (p[2] & ~right_mask2) | right_value2;
      }
    }
  }
}



void blitter_blit_apply_mask8(void *bitmap, size_t stride, uint8_t mask, uint8_t value,
                              size_t x, size_t y, size_t width, size_t height) {
  if (!width) return;
  if ((((uintptr_t)bitmap) | (uintptr_t)stride) & 0x7) {
    /* Bitmap wasn't allocated with 64-bit alignment in mind, go slow-path */
    size_t row, col;
    uint8_t *prow, *p;
    prow = ((uint8_t *)bitmap) + stride * y + x;
    value &= mask;
    for (row = 0; row < height; ++row) {
      p = prow;
      prow += stride;
      for (col = 0; col < width; ++col) {
        *p = (*p & ~mask) | value;
        p++;
      }
    }
    return;
  }

  size_t right_edge_inclusive = (x + width - 1);
  uint64_t lmask, imask, rmask;
  imask = mask | (((uint64_t)imask) << 8);
  imask |= imask << 16;
  imask |= imask << 32;
  lmask = (imask >> (8 * (x & 7)));
  rmask = (imask << (8 * (7 - (right_edge_inclusive & 7))));

  uint64_t value64 = value | (((uint64_t)value) << 8);
  value64 |= value64 << 16;
  value64 |= value64 << 32;

  size_t x64 = x/8;
  size_t width64 = (right_edge_inclusive/8) - x64;
  size_t stride64 = stride / 8;
  blitter_blit_apply_mask_fast64(bitmap, stride64, y, x/8, width64, height, lmask, imask, rmask, value);
}

static void lsl192(uint64_t *r0, uint64_t *r1, uint64_t *r2, uint64_t l0, uint64_t l1, uint64_t l2, int shift_left) {
  uint64_t v[] = {
    0, 0, 0, 0, l0, l1, l2, 0, 0, 0, 0
  };

  int zero = 192+64;
  if (shift_left < 0) {
    zero -= 64;
    shift_left = 64 - shift_left;
  }
  uint64_t sl0 = v[(zero + shift_left) / 64] << (shift_left & 63);
  sl0 |= (shift_left & 63) ? v[1 + (zero + shift_left) / 64] >> (64 - (shift_left & 63)) : 0;
  zero += 64;
  uint64_t sl1 = v[(zero + shift_left) / 64] << (shift_left & 63);
  sl1 |= (shift_left & 63) ? v[1 + (zero + shift_left) / 64] >> (64 - (shift_left & 63)) : 0;
  zero += 64;
  uint64_t sl2 = v[(zero + shift_left) / 64] << (shift_left & 63);
  sl2 |= (shift_left & 63) ? v[1 + (zero + shift_left) / 64] >> (64 - (shift_left & 63)) : 0;
  *r0 = sl0;
  *r1 = sl1;
  *r2 = sl2;
}

#define lsr192(r0, r1, r2, l0, l1, l2, shift_right) lsl192(r0,r1,r2,l0,l1,l2,-(int)(shift_right))

void blitter_blit_apply_mask3x8(void *bitmap, size_t stride, 
                                uint8_t mask0, uint8_t mask1, uint8_t mask2, 
                                uint8_t v0, uint8_t v1, uint8_t v2,
                                size_t x, size_t y, size_t width, size_t height) {

  if (!width) return;
  if ((((uintptr_t)bitmap) | (uintptr_t)stride) & 0x7) {
    size_t row, col;
    uint8_t *prow, *p;
    prow = ((uint8_t *)bitmap) + stride * y + x * 3;
    v0 &= mask0;
    v1 &= mask1;
    v2 &= mask2;
    for (row = 0; row < height; ++row) {
      p = prow;
      prow += stride;
      for (col = 0; col < width; ++col) {
        p[0] = (p[0] & ~mask0) | v0;
        p[1] = (p[1] & ~mask1) | v1;
        p[2] = (p[2] & ~mask2) | v2;
        p += 3;
      }
    }
    return;
  }

  size_t right_edge_inclusive = (x + width - 1);

  union {
    uint16_t u16;
    uint8_t v[2];
  } endian_check;
  endian_check.u16 = 0x0100;
  int big_endian = !!endian_check.v[0];

  uint64_t imask0, imask1, imask2;
  uint64_t value0, value1, value2;
  if (big_endian) {
    value0 = (((uint64_t)v0) << 56)
           | (((uint64_t)v1) << 48)
           | (((uint64_t)v2) << 40)
           | (((uint64_t)v0) << 32)
           | (((uint64_t)v1) << 24)
           | (((uint64_t)v2) << 16)
           | (((uint64_t)v0) << 8)
           | ((uint64_t)v1);
    value1 = (((uint64_t)v2) << 56)
           | (((uint64_t)v0) << 48)
           | (((uint64_t)v1) << 40)
           | (((uint64_t)v2) << 32)
           | (((uint64_t)v0) << 24)
           | (((uint64_t)v1) << 16)
           | (((uint64_t)v2) << 8)
           | ((uint64_t)v0);
    value2 = (((uint64_t)v1) << 56)
           | (((uint64_t)v2) << 48)
           | (((uint64_t)v0) << 40)
           | (((uint64_t)v1) << 32)
           | (((uint64_t)v2) << 24)
           | (((uint64_t)v0) << 16)
           | (((uint64_t)v1) << 8)
           | ((uint64_t)v2);

    imask0 = (((uint64_t)mask0) << 56)
           | (((uint64_t)mask1) << 48)
           | (((uint64_t)mask2) << 40)
           | (((uint64_t)mask0) << 32)
           | (((uint64_t)mask1) << 24)
           | (((uint64_t)mask2) << 16)
           | (((uint64_t)mask0) << 8)
           | ((uint64_t)mask1);
    imask1 = (((uint64_t)mask2) << 56)
           | (((uint64_t)mask0) << 48)
           | (((uint64_t)mask1) << 40)
           | (((uint64_t)mask2) << 32)
           | (((uint64_t)mask0) << 24)
           | (((uint64_t)mask1) << 16)
           | (((uint64_t)mask2) << 8)
           | ((uint64_t)mask0);
    imask2 = (((uint64_t)mask1) << 56)
           | (((uint64_t)mask2) << 48)
           | (((uint64_t)mask0) << 40)
           | (((uint64_t)mask1) << 32)
           | (((uint64_t)mask2) << 24)
           | (((uint64_t)mask0) << 16)
           | (((uint64_t)mask1) << 8)
           | ((uint64_t)mask2);
  }
  else {
    value0 = ((uint64_t)v0)
           | (((uint64_t)v1) << 8)
           | (((uint64_t)v2) << 16)
           | (((uint64_t)v0) << 24)
           | (((uint64_t)v1) << 32)
           | (((uint64_t)v2) << 40)
           | (((uint64_t)v0) << 48)
           | (((uint64_t)v1) << 56);
    value1 = ((uint64_t)v2)
           | (((uint64_t)v0) << 8)
           | (((uint64_t)v1) << 16)
           | (((uint64_t)v2) << 24)
           | (((uint64_t)v0) << 32)
           | (((uint64_t)v1) << 40)
           | (((uint64_t)v2) << 48)
           | (((uint64_t)v0) << 56);
    value2 = ((uint64_t)v1)
           | (((uint64_t)v2) << 8)
           | (((uint64_t)v0) << 16)
           | (((uint64_t)v1) << 24)
           | (((uint64_t)v2) << 32)
           | (((uint64_t)v0) << 40)
           | (((uint64_t)v1) << 48)
           | (((uint64_t)v2) << 56);

    imask0 = ((uint64_t)mask0)
           | (((uint64_t)mask1) << 8)
           | (((uint64_t)mask2) << 16)
           | (((uint64_t)mask0) << 24)
           | (((uint64_t)mask1) << 32)
           | (((uint64_t)mask2) << 40)
           | (((uint64_t)mask0) << 48)
           | (((uint64_t)mask1) << 56);
    imask1 = ((uint64_t)mask2)
           | (((uint64_t)mask0) << 8)
           | (((uint64_t)mask1) << 16)
           | (((uint64_t)mask2) << 24)
           | (((uint64_t)mask0) << 32)
           | (((uint64_t)mask1) << 40)
           | (((uint64_t)mask2) << 48)
           | (((uint64_t)mask0) << 56);
    imask2 = ((uint64_t)mask1)
           | (((uint64_t)mask2) << 8)
           | (((uint64_t)mask0) << 16)
           | (((uint64_t)mask1) << 24)
           | (((uint64_t)mask2) << 32)
           | (((uint64_t)mask0) << 40)
           | (((uint64_t)mask1) << 48)
           | (((uint64_t)mask2) << 56);
  }
  uint64_t left_mask_mask0, left_mask_mask1, left_mask_mask2;
  uint64_t right_mask_mask0, right_mask_mask1, right_mask_mask2;
  
  lsr192(&left_mask_mask0, &left_mask_mask1, &left_mask_mask2,
         ~(uint64_t)0, ~(uint64_t)0, ~(uint64_t)0,
         8 * ((x * 3) % 24));
  
  lsl192(&right_mask_mask0, &right_mask_mask1, &right_mask_mask2,
         ~(uint64_t)0, ~(uint64_t)0, ~(uint64_t)0,
         8 * (23 - ((right_edge_inclusive * 3) % 24)));

  uint64_t lmask0, lmask1, lmask2, rmask0, rmask1, rmask2;
  lmask0 = imask0 & left_mask_mask0;
  lmask1 = imask1 & left_mask_mask1;
  lmask2 = imask2 & left_mask_mask2;
  rmask0 = imask0 & right_mask_mask0;
  rmask1 = imask1 & right_mask_mask1;
  rmask2 = imask2 & right_mask_mask2;

  size_t x192 = x/24;
  size_t width192 = ((right_edge_inclusive*3)/24) - x192;
  size_t x64 = x192*3;
  size_t stride64 = stride / 8;
  blitter_blit_apply_mask_fast192(bitmap, stride64, y, x64, width192, height,
                                  lmask0, lmask1, lmask2,
                                  imask0, imask1, imask2,
                                  rmask0, rmask1, rmask2,
                                  value0, value1, value2);

}

void blitter_blit_apply_mask4x8(void *bitmap, size_t stride, 
                                uint8_t mask0, uint8_t mask1, uint8_t mask2, uint8_t mask3,
                                uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3,
                                size_t x, size_t y, size_t width, size_t height) {

  if (!width) return;
  if ((((uintptr_t)bitmap) | (uintptr_t)stride) & 0x7) {
    size_t row, col;
    uint8_t * restrict prow, * restrict p;
    prow = ((uint8_t * restrict)bitmap) + stride * y + x * 3;
    v0 &= mask0;
    v1 &= mask1;
    v2 &= mask2;
    v3 &= mask3;
    for (row = 0; row < height; ++row) {
      p = prow;
      prow += stride;
      for (col = 0; col < width; ++col) {
        p[0] = (p[0] & ~mask0) | v0;
        p[1] = (p[1] & ~mask1) | v1;
        p[2] = (p[2] & ~mask2) | v2;
        p[3] = (p[3] & ~mask3) | v3;
        p += 4;
      }
    }
    return;
  }

  size_t right_edge_inclusive = (x + width - 1);

  union {
    uint16_t u16;
    uint8_t v[2];
  } endian_check;
  endian_check.u16 = 0x0100;
  int big_endian = !!endian_check.v[0];

  uint64_t imask;
  uint64_t value;
  if (big_endian) {
    value = (((uint64_t)v0) << 56)
          | (((uint64_t)v1) << 48)
          | (((uint64_t)v2) << 40)
          | (((uint64_t)v3) << 32)
          | (((uint64_t)v0) << 24)
          | (((uint64_t)v1) << 16)
          | (((uint64_t)v2) << 8)
          | ((uint64_t)v3);

    imask = (((uint64_t)mask0) << 56)
          | (((uint64_t)mask1) << 48)
          | (((uint64_t)mask2) << 40)
          | (((uint64_t)mask3) << 32)
          | (((uint64_t)mask0) << 24)
          | (((uint64_t)mask1) << 16)
          | (((uint64_t)mask2) << 8)
          | ((uint64_t)mask3);
  }
  else {
    value = ((uint64_t)v0)
          | (((uint64_t)v1) << 8)
          | (((uint64_t)v2) << 16)
          | (((uint64_t)v3) << 24)
          | (((uint64_t)v0) << 32)
          | (((uint64_t)v1) << 40)
          | (((uint64_t)v2) << 48)
          | (((uint64_t)v3) << 56);

    imask = ((uint64_t)mask0)
          | (((uint64_t)mask1) << 8)
          | (((uint64_t)mask2) << 16)
          | (((uint64_t)mask3) << 24)
          | (((uint64_t)mask0) << 32)
          | (((uint64_t)mask1) << 40)
          | (((uint64_t)mask2) << 48)
          | (((uint64_t)mask3) << 56);
  }
  uint64_t left_mask_mask;
  uint64_t right_mask_mask;
  
  left_mask_mask = (~(uint64_t)0) >> (32 * (x & 1));
  right_mask_mask = (~(uint64_t)0) << (32 * (1 - (right_edge_inclusive & 1)));

  uint64_t lmask, rmask;
  lmask = imask & left_mask_mask;
  rmask = imask & right_mask_mask;

  size_t x64 = x/2;
  size_t width64 = (right_edge_inclusive/2) - x64;
  size_t stride64 = stride / 8;
  blitter_blit_apply_mask_fast64(bitmap, stride64, y, x64, width64, height,
                                  lmask, imask, rmask, value);
}

void blitter_blit_apply_mask16(void *bitmap, size_t stride, uint16_t mask, uint16_t value,
                               size_t x, size_t y, size_t width, size_t height) {
  if (!width) return;
  if ((((uintptr_t)bitmap) | (uintptr_t)stride) & 0x7) {
    size_t row, col;
    uint8_t * restrict prow, * restrict p;
    value &= mask;
    uint8_t v0, v1;
    uint8_t m0, m1;
    union {
      uint16_t u16;
      uint8_t u8[2];
    } endian_xlat;
    prow = ((uint8_t * restrict )bitmap) + stride * y + x * 2;
    endian_xlat.u16 = value;
    v0 = endian_xlat.u8[0];
    v1 = endian_xlat.u8[1];
    endian_xlat.u16 = mask;
    m0 = endian_xlat.u8[0];
    m1 = endian_xlat.u8[1];
    for (row = 0; row < height; ++row) {
      p = prow;
      prow += stride;
      for (col = 0; col < width; ++col) {
        p[0] = (p[0] & ~m0) | v0;
        p[1] = (p[1] & ~m1) | v1;
        p += 2;
      }
    }
    return;
  }

  size_t right_edge_inclusive = (x + width - 1);

  uint64_t imask;
  uint64_t ivalue;
  ivalue = (((uint64_t)value) << 48)
         | (((uint64_t)value) << 32)
         | (((uint64_t)value) << 16)
         | ((uint64_t)value);
  imask = (((uint64_t)mask) << 48)
        | (((uint64_t)mask) << 32)
        | (((uint64_t)mask) << 16)
        | ((uint64_t)mask);

  uint64_t left_mask_mask;
  uint64_t right_mask_mask;
  
  left_mask_mask = (~(uint64_t)0) >> (16 * (x & 3));
  right_mask_mask = (~(uint64_t)0) << (16 * (3 - (right_edge_inclusive & 3)));

  uint64_t lmask, rmask;
  lmask = imask & left_mask_mask;
  rmask = imask & right_mask_mask;

  size_t x64 = x/4;
  size_t width64 = (right_edge_inclusive/4) - x64;
  size_t stride64 = stride / 8;
  blitter_blit_apply_mask_fast64(bitmap, stride64, y, x64, width64, height,
                                  lmask, imask, rmask, ivalue);
}

void blitter_blit_apply_mask32(void *bitmap, size_t stride, uint32_t mask, uint32_t value,
                               size_t x, size_t y, size_t width, size_t height) {
  if (!width) return;
  if ((((uintptr_t)bitmap) | (uintptr_t)stride) & 0x7) {
    size_t row, col;
    uint8_t * restrict prow, * restrict p;
    value &= mask;
    uint8_t v0, v1, v2, v3;
    uint8_t m0, m1, m2, m3;
    union {
      uint32_t u32;
      uint8_t u8[4];
    } endian_xlat;
    prow = ((uint8_t * restrict )bitmap) + stride * y + x * 4;
    endian_xlat.u32 = value;
    v0 = endian_xlat.u8[0];
    v1 = endian_xlat.u8[1];
    v2 = endian_xlat.u8[2];
    v3 = endian_xlat.u8[3];
    endian_xlat.u32 = mask;
    m0 = endian_xlat.u8[0];
    m1 = endian_xlat.u8[1];
    m2 = endian_xlat.u8[2];
    m3 = endian_xlat.u8[3];
    for (row = 0; row < height; ++row) {
      p = prow;
      prow += stride;
      for (col = 0; col < width; ++col) {
        p[0] = (p[0] & ~m0) | v0;
        p[1] = (p[1] & ~m1) | v1;
        p[2] = (p[2] & ~m2) | v2;
        p[3] = (p[3] & ~m3) | v3;
        p += 4;
      }
    }
    return;
  }

  size_t right_edge_inclusive = (x + width - 1);

  uint64_t imask;
  uint64_t ivalue;
  ivalue = (((uint64_t)value) << 32)
         | ((uint64_t)value);
  imask = (((uint64_t)mask) << 32)
        | ((uint64_t)mask);

  uint64_t left_mask_mask;
  uint64_t right_mask_mask;
  
  left_mask_mask = (~(uint64_t)0) >> (32 * (x & 1));
  right_mask_mask = (~(uint64_t)0) << (32 * (1 - (right_edge_inclusive & 1)));

  uint64_t lmask, rmask;
  lmask = imask & left_mask_mask;
  rmask = imask & right_mask_mask;

  size_t x64 = x/2;
  size_t width64 = (right_edge_inclusive/2) - x64;
  size_t stride64 = stride / 8;
  blitter_blit_apply_mask_fast64(bitmap, stride64, y, x64, width64, height,
                                  lmask, imask, rmask, value);
}

static void blitter_blit_alpha_to_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t alpha = psrc[0];
      pdst[0] = alpha;
      psrc += 1;
      pdst += 1;
    }
  }
}

static void blitter_blit_alpha_to_luminance(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = 0x00;
      pdst[0] = red;
      psrc += 1;
      pdst += 1;
    }
  }
}

static void blitter_blit_alpha_to_luminance_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = 0x00;
      uint8_t alpha = psrc[0];
      pdst[0] = red;
      pdst[1] = alpha;
      psrc += 1;
      pdst += 2;
    }
  }
}

static void blitter_blit_alpha_to_rgb(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 3 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = 0x00;
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      psrc += 1;
      pdst += 3;
    }
  }
}

static void blitter_blit_alpha_to_rgba(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 4 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = 0x00;
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      uint8_t alpha = psrc[0];
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      pdst[3] = alpha;
      psrc += 1;
      pdst += 4;
    }
  }
}

static void blitter_blit_alpha_to_565(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = 0x00;
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07E0)
                      | ((((uint16_t)blue) >> 3) & 0x001F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 1;
      pdst += 2;
    }
  }
}

static void blitter_blit_alpha_to_4444(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = 0x00;
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      uint8_t alpha = psrc[0];
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF000)
                      | ((((uint16_t)green) << 4) & 0x0F00)
                      |  (((uint16_t)blue) & 0x00F0)
                      | ((((uint16_t)alpha) >> 4) & 0x000F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 1;
      pdst += 2;
    }
  }
}

static void blitter_blit_alpha_to_5551(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = 0x00;
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      uint8_t alpha = psrc[0];
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07C0)
                      | ((((uint16_t)blue) >> 2) & 0x003E)
                      | ((((uint16_t)alpha) >> 7) & 0x0001);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 1;
      pdst += 2;
    }
  }
}

static void blitter_blit_luminance_to_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t alpha = 0xFF;
      pdst[0] = alpha;
      psrc += 1;
      pdst += 1;
    }
  }
}

static void blitter_blit_luminance_to_luminance(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      pdst[0] = red;
      psrc += 1;
      pdst += 1;
    }
  }
}

static void blitter_blit_luminance_to_luminance_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t alpha = 0xFF;
      pdst[0] = red;
      pdst[1] = alpha;
      psrc += 1;
      pdst += 2;
    }
  }
}

static void blitter_blit_luminance_to_rgb(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 3 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      psrc += 1;
      pdst += 3;
    }
  }
}

static void blitter_blit_luminance_to_rgba(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 4 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      uint8_t alpha = 0xFF;
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      pdst[3] = alpha;
      psrc += 1;
      pdst += 4;
    }
  }
}

static void blitter_blit_luminance_to_565(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07E0)
                      | ((((uint16_t)blue) >> 3) & 0x001F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 1;
      pdst += 2;
    }
  }
}

static void blitter_blit_luminance_to_4444(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      uint8_t alpha = 0xFF;
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF000)
                      | ((((uint16_t)green) << 4) & 0x0F00)
                      |  (((uint16_t)blue) & 0x00F0)
                      | ((((uint16_t)alpha) >> 4) & 0x000F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 1;
      pdst += 2;
    }
  }
}

static void blitter_blit_luminance_to_5551(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 1 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      uint8_t alpha = 0xFF;
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07C0)
                      | ((((uint16_t)blue) >> 2) & 0x003E)
                      | ((((uint16_t)alpha) >> 7) & 0x0001);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 1;
      pdst += 2;
    }
  }
}

static void blitter_blit_luminance_alpha_to_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t alpha = psrc[1];
      pdst[0] = alpha;
      psrc += 2;
      pdst += 1;
    }
  }
}

static void blitter_blit_luminance_alpha_to_luminance(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      pdst[0] = red;
      psrc += 2;
      pdst += 1;
    }
  }
}

static void blitter_blit_luminance_alpha_to_luminance_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t alpha = psrc[1];
      pdst[0] = red;
      pdst[1] = alpha;
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_luminance_alpha_to_rgb(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 3 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      psrc += 2;
      pdst += 3;
    }
  }
}

static void blitter_blit_luminance_alpha_to_rgba(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 4 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      uint8_t alpha = psrc[1];
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      pdst[3] = alpha;
      psrc += 2;
      pdst += 4;
    }
  }
}

static void blitter_blit_luminance_alpha_to_565(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07E0)
                      | ((((uint16_t)blue) >> 3) & 0x001F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_luminance_alpha_to_4444(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      uint8_t alpha = psrc[1];
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF000)
                      | ((((uint16_t)green) << 4) & 0x0F00)
                      |  (((uint16_t)blue) & 0x00F0)
                      | ((((uint16_t)alpha) >> 4) & 0x000F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_luminance_alpha_to_5551(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = 0x00;
      uint8_t blue = 0x00;
      uint8_t alpha = psrc[1];
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07C0)
                      | ((((uint16_t)blue) >> 2) & 0x003E)
                      | ((((uint16_t)alpha) >> 7) & 0x0001);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_rgb_to_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 3 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t alpha = 0xFF;
      pdst[0] = alpha;
      psrc += 3;
      pdst += 1;
    }
  }
}

static void blitter_blit_rgb_to_luminance(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 3 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      pdst[0] = red;
      psrc += 3;
      pdst += 1;
    }
  }
}

static void blitter_blit_rgb_to_luminance_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 3 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t alpha = 0xFF;
      pdst[0] = red;
      pdst[1] = alpha;
      psrc += 3;
      pdst += 2;
    }
  }
}

static void blitter_blit_rgb_to_rgb(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 3 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 3 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = psrc[1];
      uint8_t blue = psrc[2];
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      psrc += 3;
      pdst += 3;
    }
  }
}

static void blitter_blit_rgb_to_rgba(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 3 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 4 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = psrc[1];
      uint8_t blue = psrc[2];
      uint8_t alpha = 0xFF;
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      pdst[3] = alpha;
      psrc += 3;
      pdst += 4;
    }
  }
}

static void blitter_blit_rgb_to_565(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 3 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = psrc[1];
      uint8_t blue = psrc[2];
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07E0)
                      | ((((uint16_t)blue) >> 3) & 0x001F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 3;
      pdst += 2;
    }
  }
}

static void blitter_blit_rgb_to_4444(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 3 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = psrc[1];
      uint8_t blue = psrc[2];
      uint8_t alpha = 0xFF;
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF000)
                      | ((((uint16_t)green) << 4) & 0x0F00)
                      |  (((uint16_t)blue) & 0x00F0)
                      | ((((uint16_t)alpha) >> 4) & 0x000F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 3;
      pdst += 2;
    }
  }
}

static void blitter_blit_rgb_to_5551(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 3 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = psrc[1];
      uint8_t blue = psrc[2];
      uint8_t alpha = 0xFF;
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07C0)
                      | ((((uint16_t)blue) >> 2) & 0x003E)
                      | ((((uint16_t)alpha) >> 7) & 0x0001);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 3;
      pdst += 2;
    }
  }
}

static void blitter_blit_rgba_to_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 4 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t alpha = psrc[3];
      pdst[0] = alpha;
      psrc += 4;
      pdst += 1;
    }
  }
}

static void blitter_blit_rgba_to_luminance(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 4 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      pdst[0] = red;
      psrc += 4;
      pdst += 1;
    }
  }
}

static void blitter_blit_rgba_to_luminance_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 4 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t alpha = psrc[3];
      pdst[0] = red;
      pdst[1] = alpha;
      psrc += 4;
      pdst += 2;
    }
  }
}

static void blitter_blit_rgba_to_rgb(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 4 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 3 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = psrc[1];
      uint8_t blue = psrc[2];
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      psrc += 4;
      pdst += 3;
    }
  }
}

static void blitter_blit_rgba_to_rgba(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 4 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 4 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = psrc[1];
      uint8_t blue = psrc[2];
      uint8_t alpha = psrc[3];
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      pdst[3] = alpha;
      psrc += 4;
      pdst += 4;
    }
  }
}

static void blitter_blit_rgba_to_565(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 4 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = psrc[1];
      uint8_t blue = psrc[2];
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07E0)
                      | ((((uint16_t)blue) >> 3) & 0x001F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 4;
      pdst += 2;
    }
  }
}

static void blitter_blit_rgba_to_4444(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 4 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = psrc[1];
      uint8_t blue = psrc[2];
      uint8_t alpha = psrc[3];
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF000)
                      | ((((uint16_t)green) << 4) & 0x0F00)
                      |  (((uint16_t)blue) & 0x00F0)
                      | ((((uint16_t)alpha) >> 4) & 0x000F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 4;
      pdst += 2;
    }
  }
}

static void blitter_blit_rgba_to_5551(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 4 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      uint8_t red = psrc[0];
      uint8_t green = psrc[1];
      uint8_t blue = psrc[2];
      uint8_t alpha = psrc[3];
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07C0)
                      | ((((uint16_t)blue) >> 2) & 0x003E)
                      | ((((uint16_t)alpha) >> 7) & 0x0001);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 4;
      pdst += 2;
    }
  }
}

static void blitter_blit_565_to_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t alpha = 0xFF;
      pdst[0] = alpha;
      psrc += 2;
      pdst += 1;
    }
  }
}

static void blitter_blit_565_to_luminance(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      pdst[0] = red;
      psrc += 2;
      pdst += 1;
    }
  }
}

static void blitter_blit_565_to_luminance_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      uint8_t alpha = 0xFF;
      pdst[0] = red;
      pdst[1] = alpha;
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_565_to_rgb(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 3 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      uint8_t green = (uint8_t)(((source_word.u16 >> 3) & 0xFC) | ((source_word.u16 >> 9) & 0x03));
      uint8_t blue = (uint8_t)(((source_word.u16 << 3) & 0xF8) | ((source_word.u16 >> 2) & 0x07));
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      psrc += 2;
      pdst += 3;
    }
  }
}

static void blitter_blit_565_to_rgba(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 4 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      uint8_t green = (uint8_t)(((source_word.u16 >> 3) & 0xFC) | ((source_word.u16 >> 9) & 0x03));
      uint8_t blue = (uint8_t)(((source_word.u16 << 3) & 0xF8) | ((source_word.u16 >> 2) & 0x07));
      uint8_t alpha = 0xFF;
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      pdst[3] = alpha;
      psrc += 2;
      pdst += 4;
    }
  }
}

static void blitter_blit_565_to_565(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      uint8_t green = (uint8_t)(((source_word.u16 >> 3) & 0xFC) | ((source_word.u16 >> 9) & 0x03));
      uint8_t blue = (uint8_t)(((source_word.u16 << 3) & 0xF8) | ((source_word.u16 >> 2) & 0x07));
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07E0)
                      | ((((uint16_t)blue) >> 3) & 0x001F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_565_to_4444(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      uint8_t green = (uint8_t)(((source_word.u16 >> 3) & 0xFC) | ((source_word.u16 >> 9) & 0x03));
      uint8_t blue = (uint8_t)(((source_word.u16 << 3) & 0xF8) | ((source_word.u16 >> 2) & 0x07));
      uint8_t alpha = 0xFF;
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF000)
                      | ((((uint16_t)green) << 4) & 0x0F00)
                      |  (((uint16_t)blue) & 0x00F0)
                      | ((((uint16_t)alpha) >> 4) & 0x000F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_565_to_5551(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      uint8_t green = (uint8_t)(((source_word.u16 >> 3) & 0xFC) | ((source_word.u16 >> 9) & 0x03));
      uint8_t blue = (uint8_t)(((source_word.u16 << 3) & 0xF8) | ((source_word.u16 >> 2) & 0x07));
      uint8_t alpha = 0xFF;
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07C0)
                      | ((((uint16_t)blue) >> 2) & 0x003E)
                      | ((((uint16_t)alpha) >> 7) & 0x0001);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_4444_to_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t alpha = (uint8_t)(((source_word.u16 << 4) & 0xF0) | (source_word.u16 & 0x0F));
      pdst[0] = alpha;
      psrc += 2;
      pdst += 1;
    }
  }
}

static void blitter_blit_4444_to_luminance(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF0) | ((source_word.u16 >> 12) & 0x0F));
      pdst[0] = red;
      psrc += 2;
      pdst += 1;
    }
  }
}

static void blitter_blit_4444_to_luminance_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF0) | ((source_word.u16 >> 12) & 0x0F));
      uint8_t alpha = (uint8_t)(((source_word.u16 << 4) & 0xF0) | (source_word.u16 & 0x0F));
      pdst[0] = red;
      pdst[1] = alpha;
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_4444_to_rgb(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 3 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF0) | ((source_word.u16 >> 12) & 0x0F));
      uint8_t green = (uint8_t)(((source_word.u16 >> 4) & 0xF0) | ((source_word.u16 >> 8) & 0x0F));
      uint8_t blue = (uint8_t)((source_word.u16 & 0xF0) | ((source_word.u16 >> 4) & 0x0F));
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      psrc += 2;
      pdst += 3;
    }
  }
}

static void blitter_blit_4444_to_rgba(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 4 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF0) | ((source_word.u16 >> 12) & 0x0F));
      uint8_t green = (uint8_t)(((source_word.u16 >> 4) & 0xF0) | ((source_word.u16 >> 8) & 0x0F));
      uint8_t blue = (uint8_t)((source_word.u16 & 0xF0) | ((source_word.u16 >> 4) & 0x0F));
      uint8_t alpha = (uint8_t)(((source_word.u16 << 4) & 0xF0) | (source_word.u16 & 0x0F));
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      pdst[3] = alpha;
      psrc += 2;
      pdst += 4;
    }
  }
}

static void blitter_blit_4444_to_565(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF0) | ((source_word.u16 >> 12) & 0x0F));
      uint8_t green = (uint8_t)(((source_word.u16 >> 4) & 0xF0) | ((source_word.u16 >> 8) & 0x0F));
      uint8_t blue = (uint8_t)((source_word.u16 & 0xF0) | ((source_word.u16 >> 4) & 0x0F));
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07E0)
                      | ((((uint16_t)blue) >> 3) & 0x001F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_4444_to_4444(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF0) | ((source_word.u16 >> 12) & 0x0F));
      uint8_t green = (uint8_t)(((source_word.u16 >> 4) & 0xF0) | ((source_word.u16 >> 8) & 0x0F));
      uint8_t blue = (uint8_t)((source_word.u16 & 0xF0) | ((source_word.u16 >> 4) & 0x0F));
      uint8_t alpha = (uint8_t)(((source_word.u16 << 4) & 0xF0) | (source_word.u16 & 0x0F));
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF000)
                      | ((((uint16_t)green) << 4) & 0x0F00)
                      |  (((uint16_t)blue) & 0x00F0)
                      | ((((uint16_t)alpha) >> 4) & 0x000F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_4444_to_5551(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF0) | ((source_word.u16 >> 12) & 0x0F));
      uint8_t green = (uint8_t)(((source_word.u16 >> 4) & 0xF0) | ((source_word.u16 >> 8) & 0x0F));
      uint8_t blue = (uint8_t)((source_word.u16 & 0xF0) | ((source_word.u16 >> 4) & 0x0F));
      uint8_t alpha = (uint8_t)(((source_word.u16 << 4) & 0xF0) | (source_word.u16 & 0x0F));
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07C0)
                      | ((((uint16_t)blue) >> 2) & 0x003E)
                      | ((((uint16_t)alpha) >> 7) & 0x0001);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_5551_to_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t alpha = (uint8_t)((0x100 - (source_word.u16 & 0x0001)) & 0xFF);
      pdst[0] = alpha;
      psrc += 2;
      pdst += 1;
    }
  }
}

static void blitter_blit_5551_to_luminance(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 1 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      pdst[0] = red;
      psrc += 2;
      pdst += 1;
    }
  }
}

static void blitter_blit_5551_to_luminance_alpha(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      uint8_t alpha = (uint8_t)((0x100 - (source_word.u16 & 0x0001)) & 0xFF);
      pdst[0] = red;
      pdst[1] = alpha;
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_5551_to_rgb(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 3 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      uint8_t green = (uint8_t)(((source_word.u16 >> 3) & 0xF8) | ((source_word.u16 >> 8) & 0x07));
      uint8_t blue = (uint8_t)((source_word.u16 & 0xF8) | ((source_word.u16 >> 5) & 0x07));
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      psrc += 2;
      pdst += 3;
    }
  }
}

static void blitter_blit_5551_to_rgba(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 4 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      uint8_t green = (uint8_t)(((source_word.u16 >> 3) & 0xF8) | ((source_word.u16 >> 8) & 0x07));
      uint8_t blue = (uint8_t)((source_word.u16 & 0xF8) | ((source_word.u16 >> 5) & 0x07));
      uint8_t alpha = (uint8_t)((0x100 - (source_word.u16 & 0x0001)) & 0xFF);
      pdst[0] = red;
      pdst[1] = green;
      pdst[2] = blue;
      pdst[3] = alpha;
      psrc += 2;
      pdst += 4;
    }
  }
}

static void blitter_blit_5551_to_565(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      uint8_t green = (uint8_t)(((source_word.u16 >> 3) & 0xF8) | ((source_word.u16 >> 8) & 0x07));
      uint8_t blue = (uint8_t)((source_word.u16 & 0xF8) | ((source_word.u16 >> 5) & 0x07));
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07E0)
                      | ((((uint16_t)blue) >> 3) & 0x001F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_5551_to_4444(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      uint8_t green = (uint8_t)(((source_word.u16 >> 3) & 0xF8) | ((source_word.u16 >> 8) & 0x07));
      uint8_t blue = (uint8_t)((source_word.u16 & 0xF8) | ((source_word.u16 >> 5) & 0x07));
      uint8_t alpha = (uint8_t)((0x100 - (source_word.u16 & 0x0001)) & 0xFF);
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF000)
                      | ((((uint16_t)green) << 4) & 0x0F00)
                      |  (((uint16_t)blue) & 0x00F0)
                      | ((((uint16_t)alpha) >> 4) & 0x000F);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 2;
      pdst += 2;
    }
  }
}

static void blitter_blit_5551_to_5551(void *dst, const void *src, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {  size_t row, col;
  uint8_t *restrict psrc, *restrict psrc_row;
  uint8_t *restrict pdst, *restrict pdst_row;
  psrc_row = ((uint8_t * restrict)src) + src_x * 2 + src_y * src_stride;
  pdst_row = ((uint8_t * restrict)dst) + dst_x * 2 + dst_y * dst_stride;
  for (row = 0; row < height; ++row) {
    psrc = psrc_row;
    psrc_row += src_stride;
    pdst = pdst_row;
    pdst_row += dst_stride;
    for (col = 0; col < width; ++col) {
      union {
        uint16_t u16;
        uint8_t u8[2];
      } source_word;
      source_word.u8[0] = psrc[0];
      source_word.u8[1] = psrc[1];
      uint8_t red = (uint8_t)(((source_word.u16 >> 8) & 0xF8) | ((source_word.u16 >> 13) & 0x07));
      uint8_t green = (uint8_t)(((source_word.u16 >> 3) & 0xF8) | ((source_word.u16 >> 8) & 0x07));
      uint8_t blue = (uint8_t)((source_word.u16 & 0xF8) | ((source_word.u16 >> 5) & 0x07));
      uint8_t alpha = (uint8_t)((0x100 - (source_word.u16 & 0x0001)) & 0xFF);
      union {
        uint16_t u16;
        uint8_t u8[2];
      } target_word;
      target_word.u16 = ((((uint16_t)red) << 8) & 0xF800)
                      | ((((uint16_t)green) << 3) & 0x07C0)
                      | ((((uint16_t)blue) >> 2) & 0x003E)
                      | ((((uint16_t)alpha) >> 7) & 0x0001);
      pdst[0] = target_word.u8[0];
      pdst[1] = target_word.u8[1];
      psrc += 2;
      pdst += 2;
    }
  }
}

void blitter_blit_format(void *dst, enum blitter_format dst_format, const void *src, enum blitter_format src_format, size_t dst_stride, size_t dst_x, size_t dst_y, size_t src_stride, size_t src_x, size_t src_y, size_t width, size_t height) {
  /* Dispatch to the appropriate function */
  switch (dst_format) {
    case blit_format_alpha:
      switch (src_format) {
        case blit_format_alpha:
          blitter_blit_alpha_to_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance:
          blitter_blit_luminance_to_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance_alpha:
          blitter_blit_luminance_alpha_to_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgb:
          blitter_blit_rgb_to_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgba:
          blitter_blit_rgba_to_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_565:
          blitter_blit_565_to_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_4444:
          blitter_blit_4444_to_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_5551:
          blitter_blit_5551_to_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
      }
      break;
    case blit_format_luminance:
      switch (src_format) {
        case blit_format_alpha:
          blitter_blit_alpha_to_luminance(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance:
          blitter_blit_luminance_to_luminance(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance_alpha:
          blitter_blit_luminance_alpha_to_luminance(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgb:
          blitter_blit_rgb_to_luminance(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgba:
          blitter_blit_rgba_to_luminance(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_565:
          blitter_blit_565_to_luminance(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_4444:
          blitter_blit_4444_to_luminance(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_5551:
          blitter_blit_5551_to_luminance(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
      }
      break;
    case blit_format_luminance_alpha:
      switch (src_format) {
        case blit_format_alpha:
          blitter_blit_alpha_to_luminance_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance:
          blitter_blit_luminance_to_luminance_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance_alpha:
          blitter_blit_luminance_alpha_to_luminance_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgb:
          blitter_blit_rgb_to_luminance_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgba:
          blitter_blit_rgba_to_luminance_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_565:
          blitter_blit_565_to_luminance_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_4444:
          blitter_blit_4444_to_luminance_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_5551:
          blitter_blit_5551_to_luminance_alpha(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
      }
      break;
    case blit_format_rgb:
      switch (src_format) {
        case blit_format_alpha:
          blitter_blit_alpha_to_rgb(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance:
          blitter_blit_luminance_to_rgb(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance_alpha:
          blitter_blit_luminance_alpha_to_rgb(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgb:
          blitter_blit_rgb_to_rgb(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgba:
          blitter_blit_rgba_to_rgb(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_565:
          blitter_blit_565_to_rgb(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_4444:
          blitter_blit_4444_to_rgb(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_5551:
          blitter_blit_5551_to_rgb(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
      }
      break;
    case blit_format_rgba:
      switch (src_format) {
        case blit_format_alpha:
          blitter_blit_alpha_to_rgba(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance:
          blitter_blit_luminance_to_rgba(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance_alpha:
          blitter_blit_luminance_alpha_to_rgba(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgb:
          blitter_blit_rgb_to_rgba(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgba:
          blitter_blit_rgba_to_rgba(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_565:
          blitter_blit_565_to_rgba(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_4444:
          blitter_blit_4444_to_rgba(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_5551:
          blitter_blit_5551_to_rgba(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
      }
      break;
    case blit_format_565:
      switch (src_format) {
        case blit_format_alpha:
          blitter_blit_alpha_to_565(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance:
          blitter_blit_luminance_to_565(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance_alpha:
          blitter_blit_luminance_alpha_to_565(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgb:
          blitter_blit_rgb_to_565(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgba:
          blitter_blit_rgba_to_565(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_565:
          blitter_blit_565_to_565(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_4444:
          blitter_blit_4444_to_565(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_5551:
          blitter_blit_5551_to_565(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
      }
      break;
    case blit_format_4444:
      switch (src_format) {
        case blit_format_alpha:
          blitter_blit_alpha_to_4444(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance:
          blitter_blit_luminance_to_4444(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance_alpha:
          blitter_blit_luminance_alpha_to_4444(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgb:
          blitter_blit_rgb_to_4444(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgba:
          blitter_blit_rgba_to_4444(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_565:
          blitter_blit_565_to_4444(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_4444:
          blitter_blit_4444_to_4444(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_5551:
          blitter_blit_5551_to_4444(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
      }
      break;
    case blit_format_5551:
      switch (src_format) {
        case blit_format_alpha:
          blitter_blit_alpha_to_5551(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance:
          blitter_blit_luminance_to_5551(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_luminance_alpha:
          blitter_blit_luminance_alpha_to_5551(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgb:
          blitter_blit_rgb_to_5551(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_rgba:
          blitter_blit_rgba_to_5551(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_565:
          blitter_blit_565_to_5551(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_4444:
          blitter_blit_4444_to_5551(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
        case blit_format_5551:
          blitter_blit_5551_to_5551(dst, src, dst_stride, dst_x, dst_y, src_stride, src_x, src_y, width, height);
          break;
      }
      break;
  }
}
