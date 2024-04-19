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
  else if (alignment == 2) {
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
      case blit_unsigned_short_4444:
      case blit_unsigned_short_5551:
        break;
    }
  }
}
