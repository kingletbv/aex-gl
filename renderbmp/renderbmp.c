/* Copyright 2023 Kinglet B.V.
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

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef STDARG_H_INCLUDED
#define STDARG_H_INCLUDED
#include <stdarg.h>
#endif

#ifndef ERRNO_H_INCLUDED
#define ERRNO_H_INCLUDED
#include <errno.h>
#endif

#ifndef GLSL_ES1_TOKENS_H_INCLUDED
#define GLSL_ES1_TOKENS_H_INCLUDED
#include "glsl_es1_tokens.h"
#endif

#ifndef GLSL_ES1_COMPILER_H_INCLUDED
#define GLSL_ES1_COMPILER_H_INCLUDED
#include "glsl_es1_compiler.h"
#endif

int test(void) {
  struct glsl_es1_compiler compiler, *cc;
  cc = &compiler;
  glsl_es1_compiler_init(cc);

  if (!glsl_es1_tokens_sanity_check()) {
    fprintf(stderr, "Failed sanity check\n");
    return -1;
  }

  enum glsl_es1_compiler_result ccr;
  const char *input_frag =
    "#define defined\n"
    "#if defined(defined)\n"
    "#error Hello\n"
    "#endif\n"
    "\n";
  ccr = glsl_es1_compiler_compile_mem(cc, "input.frag", input_frag, strlen(input_frag));
    
   glsl_es1_compiler_cleanup(cc);

  return 0;
}

size_t fpack(FILE *fp, const char *format, ...) {
  va_list args;
  uint8_t val_8;
  uint16_t val_16;
  uint32_t val_32;
  uint64_t val_64;
  uint8_t buffer[8];
  size_t len = 0;

  va_start(args, format);

  while (*format) {
    switch (*format++) {
      case 'b':
      case 'B':
        val_8 = (uint8_t)(va_arg(args, unsigned int));
        if (!fwrite(&val_8, 1, 1, fp)) {
          return len;
        }
        len += 1;
        break;
      case 'w':
        val_16 = (uint16_t)va_arg(args, unsigned int);
        buffer[0] = (char)(val_16 & 0xFF);
        buffer[1] = (char)((val_16 >> 8) & 0xFF);
        if (!fwrite(buffer, 2, 1, fp)) {
          return len;
        }
        len += 2;
        break;
      case 'W':
        val_16 = (uint16_t)va_arg(args, unsigned int);
        buffer[0] = (char)((val_16 >> 8) & 0xFF);
        buffer[1] = (char)(val_16 & 0xFF);
        if (!fwrite(buffer, 2, 1, fp)) {
          return len;
        }
        len += 2;
        break;
      case 'd':
        val_32 = va_arg(args, uint32_t);
        buffer[0] = (char)(val_32 & 0xFF);
        buffer[1] = (char)((val_32 >> 8) & 0xFF);
        buffer[2] = (char)((val_32 >> 16) & 0xFF);
        buffer[3] = (char)((val_32 >> 24) & 0xFF);
        if (!fwrite(buffer, 4, 1, fp)) {
          return len;
        }
        len += 4;
        break;
      case 'D':
        val_32 = va_arg(args, uint32_t);
        buffer[0] = (char)((val_32 >> 24) & 0xFF);
        buffer[1] = (char)((val_32 >> 16) & 0xFF);
        buffer[2] = (char)((val_32 >> 8) & 0xFF);
        buffer[3] = (char)(val_32 & 0xFF);
        if (!fwrite(buffer, 4, 1, fp)) {
          return len;
        }
        len += 4;
        break;
      case 'q':
        val_64 = va_arg(args, uint64_t);
        buffer[0] = (char)(val_64 & 0xFF);
        buffer[1] = (char)((val_64 >> 8) & 0xFF);
        buffer[2] = (char)((val_64 >> 16) & 0xFF);
        buffer[3] = (char)((val_64 >> 24) & 0xFF);
        buffer[4] = (char)((val_64 >> 32) & 0xFF);
        buffer[5] = (char)((val_64 >> 40) & 0xFF);
        buffer[6] = (char)((val_64 >> 48) & 0xFF);
        buffer[7] = (char)((val_64 >> 56) & 0xFF);
        if (!fwrite(buffer, 8, 1, fp)) {
          return len;
        }
        len += 8;
        break;
      case 'Q':
        val_64 = va_arg(args, uint64_t);
        buffer[0] = (char)((val_64 >> 56) & 0xFF);
        buffer[1] = (char)((val_64 >> 48) & 0xFF);
        buffer[2] = (char)((val_64 >> 40) & 0xFF);
        buffer[3] = (char)((val_64 >> 32) & 0xFF);
        buffer[4] = (char)((val_64 >> 24) & 0xFF);
        buffer[5] = (char)((val_64 >> 16) & 0xFF);
        buffer[6] = (char)((val_64 >> 8) & 0xFF);
        buffer[7] = (char)(val_64 & 0xFF);
        if (!fwrite(buffer, 8, 1, fp)) {
          return len;
        }
        len += 8;
        break;

      default:
        /* Unknown format specifier */
        errno = EINVAL;
        return len;
    }
  }
  return len;
}

void write_rgba_bmp(FILE *fp, uint8_t *rgba32, int width, int height, size_t stride) {
  /* Write the BMP file header */

  /* signature */
  fwrite("BM", 2, 1, fp);
  
  /* file size */
  fpack(fp, "d", 14 + 40 + (4 * width * height));
  fpack(fp, "ww", 0, 0);  /* reserved fields */
  fpack(fp, "d", 14 + 40); /* offset to pixel data */

  /* Write the bitmap info header */
  fpack(fp, "d", 40); /* size of bitmap info header (ergo its version) */
  fpack(fp, "dd", width, height);
  fpack(fp, "ww", 1, 32); /* 1 plane, 32 bits per pixel */
  fpack(fp, "d", 0); /* BI_RGB: no compression */
  fpack(fp, "d", 0); /* size of image data, can be zero if no compression */
  fpack(fp, "dddd", 
        0,  /* horizontal pixels per meter */ 
        0,  /* vertical pixels per meter*/
        0,  /* number of colors in palette, none for BI_RGB */
        0); /* number of "important" colors, all of em, not palette based. */

  /* Write the pixel data; start with bottom row and work our way up. */
  size_t row;
  row = ((size_t)height) - 1;
  do {
    size_t col;
    for (col = 0; col < width; col++) {
      uint8_t *rgba = rgba32 + row * stride + col * 4;
      fpack(fp, "bbbb", rgba[2], rgba[1], rgba[0], rgba[3]);
    }
  } while (row--);

}

void tri(uint8_t *rgba, size_t stride, 
         uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
         int32_t x0, int32_t y0,
         int32_t x1, int32_t y1,
         int32_t x2, int32_t y2) {

  int64_t px, py;
  for (py = scissor_top; py < scissor_bottom; ++py) {
    for (px = scissor_left; px < scissor_right; ++px) {
      int64_t Dp01, Dp12, Dp20;
      // Dp01 = determinant for triangle formed by edge 01 and point p:
      //        | px py 1 |
      // Dp01 = | x0 y0 1 |
      //        | x1 y1 1 |
      Dp01 = px * (y0 - y1) + py * (x1 - x0) + x0 * y1 - y0 * x1;

      // Dp12 = determinant for triangle formed by edge 12 and point p:
      //        | px py 1 |
      // Dp12 = | x1 y1 1 |
      //        | x2 y2 1 |
      Dp12 = px * (y1 - y2) + py * (x2 - x1) + x1 * y2 - y1 * x2;

      // Dp20 = determinant for triangle formed by edge 20 and point p:
      //        | px py 1 |
      // Dp20 = | x2 y2 1 |
      //        | x0 y0 1 |
      Dp20 = px * (y2 - y0) + py * (x0 - x2) + x2 * y0 - y2 * x0;

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
        if (x0 < x1) {
          /* Horizontal edge at the top of the triangle */
          Dp01++;
        }
      }
      else if (y0 > y1) {
        /* Non-horizontal edge on the left side of the triangle */
        Dp01++;
      }

      // Classify 12
      if (y1 == y2) {
        if (x1 < x2) {
          /* Horizontal edge at the top of the triangle */
          Dp12++;
        }
      }
      else if (y1 > y2) {
        /* Non-horizontal edge on the left side of the triangle */
        Dp12++;
      }
    
      // Classify 20
      if (y2 == y0) {
        if (x2 < x0) {
          /* Horizontal edge at the top of the triangle */
          Dp20++;
        }
      }
      else if (y2 > y0) {
        /* Non-horizontal edge on the left side of the triangle */
        Dp20++;
      }

      uint8_t *pixel = rgba + py * stride + px * 4;
      uint32_t pix = pixel[3] | (pixel[2] << 8) | (pixel[1] << 16) | (pixel[0] << 24);
      if (Dp01 > 0 && Dp12 > 0 && Dp20 > 0) {
        if (pix != 0xFF0000FF) {
          // Erroneous pixel, should be red. Turn black.
          pixel[0] = 0xFF;
          pixel[1] = 0xFF;
          pixel[2] = 0xFF;
          pixel[3] = 0xFF;
        }
        else {
          pixel[0] = 0xFF;
          pixel[1] = 0x00;
          pixel[2] = 0x00;
          pixel[3] = 0xFF;
        }
      }
      else {
        if (pix == 0xFF0000FF) {
          // Erroneous pixel, should _not_ be red. Turn white.
          pixel[0] = 0x00;
          pixel[1] = 0x00;
          pixel[2] = 0x00;
          pixel[3] = 0xFF;
        }
      }
    }
  }
}

void tri2(uint8_t *rgba, size_t stride,
          uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
          int32_t x0, int32_t y0,
          int32_t x1, int32_t y1,
          int32_t x2, int32_t y2) {
  int64_t Dp01_row, Dp12_row, Dp20_row;
  // Dp01 = determinant for triangle formed by edge 01 and point p:
  //        | px py 1 |
  // Dp01 = | x0 y0 1 |
  //        | x1 y1 1 |
  Dp01_row = ((int64_t)(scissor_left)) * (y0 - y1) + ((int64_t)(scissor_top)) * (x1 - x0) + x0 * y1 - y0 * x1;

  int64_t Dp01_dx = (y0 - y1);
  int64_t Dp01_dy = (x1 - x0);

  // Dp12 = determinant for triangle formed by edge 12 and point p:
  //        | px py 1 |
  // Dp12 = | x1 y1 1 |
  //        | x2 y2 1 |
  Dp12_row = ((int64_t)(scissor_left)) * (y1 - y2) + ((int64_t)(scissor_top)) * (x2 - x1) + x1 * y2 - y1 * x2;

  int64_t Dp12_dx = (y1 - y2);
  int64_t Dp12_dy = (x2 - x1);

  // Dp20 = determinant for triangle formed by edge 20 and point p:
  //        | px py 1 |
  // Dp20 = | x2 y2 1 |
  //        | x0 y0 1 |
  Dp20_row = ((int64_t)(scissor_left)) * (y2 - y0) + ((int64_t)(scissor_top)) * (x0 - x2) + x2 * y0 - y2 * x0;

  int64_t Dp20_dx = (y2 - y0);
  int64_t Dp20_dy = (x0 - x2);

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
    if (x0 < x1) {
      /* Horizontal edge at the top of the triangle */
      Dp01_row++;
    }
  }
  else if (y0 > y1) {
    /* Non-horizontal edge on the left side of the triangle */
    Dp01_row++;
  }

  // Classify 12
  if (y1 == y2) {
    if (x1 < x2) {
      /* Horizontal edge at the top of the triangle */
      Dp12_row++;
    }
  }
  else if (y1 > y2) {
    /* Non-horizontal edge on the left side of the triangle */
    Dp12_row++;
  }

  // Classify 20
  if (y2 == y0) {
    if (x2 < x0) {
      /* Horizontal edge at the top of the triangle */
      Dp20_row++;
    }
  }
  else if (y2 > y0) {
    /* Non-horizontal edge on the left side of the triangle */
    Dp20_row++;
  }

  int64_t px, py;
  for (py = scissor_top; py < scissor_bottom; ++py) {
    int64_t Dp01, Dp12, Dp20;
    Dp01 = Dp01_row;
    Dp12 = Dp12_row;
    Dp20 = Dp20_row;
    Dp01_row += Dp01_dy;
    Dp12_row += Dp12_dy;
    Dp20_row += Dp20_dy;
    for (px = scissor_left; px < scissor_right; ++px) {
      uint8_t *pixel = rgba + py * stride + px * 4;
      if (Dp01 > 0 && Dp12 > 0 && Dp20 > 0) {
        pixel[0] = pixel[0] ^ 0xFF;
        pixel[1] = 0x00;
        pixel[2] = 0x00;
        pixel[3] = 0xFF;
      }

      Dp01 += Dp01_dx;
      Dp12 += Dp12_dx;
      Dp20 += Dp20_dx;

    }
  }
}

// Number of bits per pixel, e.g. (1 << SUBPIXEL_BITS) positions within a single pixel.
#define SUBPIXEL_BITS 4

void tri3(uint8_t *rgba, size_t stride,
          uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
          int32_t x0, int32_t y0,
          int32_t x1, int32_t y1,
          int32_t x2, int32_t y2) {
  // Go 4 pixels at a time, the four fragments form a square, the square is valid if a single fragment
  // is inside the triangle and scissor (we will therefore intentionally be generating fragments that
  // are outside the triangle, or the scissor; we will discard those fragments later - reason for this
  // is to be able to do mip-mapping and dFdx/dFdy type stuff.)

  // Be inclusive of odd edges on the scissor rectangle.
  int64_t left = scissor_left & ~(int64_t)1;
  int64_t top = scissor_top & ~(int64_t)1;
  int64_t right = (scissor_right + 1) & ~(int64_t)1;
  int64_t bottom = (scissor_bottom + 1) & ~(int64_t)1;

  // The scissor rectangle is still in pixel coordinates, convert it into sub-pixel coordinates
  int64_t left_sp = left << SUBPIXEL_BITS;
  int64_t top_sp = top << SUBPIXEL_BITS;
  int64_t right_sp = right << SUBPIXEL_BITS;
  int64_t bottom_sp = bottom << SUBPIXEL_BITS;

  int64_t Dp01_row, Dp12_row, Dp20_row;
  // Dp01 = determinant for triangle formed by edge 01 and point p:
  //        | px py 1 |
  // Dp01 = | x0 y0 1 |
  //        | x1 y1 1 |
  Dp01_row = left_sp * (y0 - y1) + top_sp * (x1 - x0) + x0 * y1 - y0 * x1;

  // Compute stepping variables, note that these are at discrete sub-pixels
  int64_t Dp01_dx_sp = (y0 - y1);
  int64_t Dp01_dy_sp = (x1 - x0);

  // Dp12 = determinant for triangle formed by edge 12 and point p:
  //        | px py 1 |
  // Dp12 = | x1 y1 1 |
  //        | x2 y2 1 |
  Dp12_row = left_sp * (y1 - y2) + top_sp * (x2 - x1) + x1 * y2 - y1 * x2;

  // Compute stepping variables, note that these are at discrete sub-pixels
  int64_t Dp12_dx_sp = (y1 - y2);
  int64_t Dp12_dy_sp = (x2 - x1);

  // Dp20 = determinant for triangle formed by edge 20 and point p:
  //        | px py 1 |
  // Dp20 = | x2 y2 1 |
  //        | x0 y0 1 |
  Dp20_row = left_sp * (y2 - y0) + top_sp * (x0 - x2) + x2 * y0 - y2 * x0;

  // Compute stepping variables, note that these are at discrete sub-pixels
  int64_t Dp20_dx_sp = (y2 - y0);
  int64_t Dp20_dy_sp = (x0 - x2);

  // Compute the stepping variables for whole pixels (how much to step when
  // we're at one pixel, and would like to skip (1 << SUBPIXEL_BITS) sub-pixels
  // to get to the same subpixel one whole pixel on whatever stepping dim.)
  int64_t Dp01_dx = Dp01_dx_sp << SUBPIXEL_BITS;
  int64_t Dp01_dy = Dp01_dy_sp << SUBPIXEL_BITS;
  int64_t Dp12_dx = Dp12_dx_sp << SUBPIXEL_BITS;
  int64_t Dp12_dy = Dp12_dy_sp << SUBPIXEL_BITS;
  int64_t Dp20_dx = Dp20_dx_sp << SUBPIXEL_BITS;
  int64_t Dp20_dy = Dp20_dy_sp << SUBPIXEL_BITS;

  // Compute the stepping variables for stepping 2 pixels at a time. This is
  // trivial but something we explicitly want outside the loop.
  int64_t Dp01_dx_2 = Dp01_dx << 1;
  int64_t Dp01_dy_2 = Dp01_dy << 1;
  int64_t Dp12_dx_2 = Dp12_dx << 1;
  int64_t Dp12_dy_2 = Dp12_dy << 1;
  int64_t Dp20_dx_2 = Dp20_dx << 1;
  int64_t Dp20_dy_2 = Dp20_dy << 1;

  // Now compute row values for each of the four pixels.
  int64_t Dp01_row_TL = Dp01_row;
  int64_t Dp01_row_TR = Dp01_row + Dp01_dx;
  int64_t Dp01_row_BL = Dp01_row + Dp01_dy;
  int64_t Dp01_row_BR = Dp01_row + Dp01_dx + Dp01_dy;
  int64_t Dp12_row_TL = Dp12_row;
  int64_t Dp12_row_TR = Dp12_row + Dp12_dx;
  int64_t Dp12_row_BL = Dp12_row + Dp12_dy;
  int64_t Dp12_row_BR = Dp12_row + Dp12_dx + Dp12_dy;
  int64_t Dp20_row_TL = Dp20_row;
  int64_t Dp20_row_TR = Dp20_row + Dp20_dx;
  int64_t Dp20_row_BL = Dp20_row + Dp20_dy;
  int64_t Dp20_row_BR = Dp20_row + Dp20_dx + Dp20_dy;

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

  int64_t px, py;
  uint8_t *pixel_TL = rgba + top * stride + left * 4;
  uint8_t *pixel_TR = pixel_TL + 4;
  uint8_t *pixel_BL = pixel_TL + stride;
  uint8_t *pixel_BR = pixel_BL + 4;
  int64_t pixel_mod = 2 * stride - (right - left) * 4; // bltdmod ;-) Keep in mind we go forward not 1, but 2 rows.

  for (py = top; py < bottom; py += 2) {
    int64_t Dp01_TL, Dp12_TL, Dp20_TL;
    int64_t Dp01_TR, Dp12_TR, Dp20_TR;
    int64_t Dp01_BL, Dp12_BL, Dp20_BL;
    int64_t Dp01_BR, Dp12_BR, Dp20_BR;
    // Copy row values to be ready for column increments
    Dp01_TL = Dp01_row_TL;
    Dp12_TL = Dp12_row_TL;
    Dp20_TL = Dp20_row_TL;
    Dp01_TR = Dp01_row_TR;
    Dp12_TR = Dp12_row_TR;
    Dp20_TR = Dp20_row_TR;
    Dp01_BL = Dp01_row_BL;
    Dp12_BL = Dp12_row_BL;
    Dp20_BL = Dp20_row_BL;
    Dp01_BR = Dp01_row_BR;
    Dp12_BR = Dp12_row_BR;
    Dp20_BR = Dp20_row_BR;
    Dp01_row_TL += Dp01_dy_2;
    Dp12_row_TL += Dp12_dy_2;
    Dp20_row_TL += Dp20_dy_2;
    Dp01_row_TR += Dp01_dy_2;
    Dp12_row_TR += Dp12_dy_2;
    Dp20_row_TR += Dp20_dy_2;
    Dp01_row_BL += Dp01_dy_2;
    Dp12_row_BL += Dp12_dy_2;
    Dp20_row_BL += Dp20_dy_2;
    Dp01_row_BR += Dp01_dy_2;
    Dp12_row_BR += Dp12_dy_2;
    Dp20_row_BR += Dp20_dy_2;
    for (px = left; px < right; px += 2) {
      // Compute the masks for each determinant at each of the four pixels
      // The idea is that if the determinant is positive, then the pixel is
      // inside the triangle; consequently, if the MSB sign bit is zero, then
      // the mask should be all 1's.
      // Note that the test is now >= 0, different from tri2.
      int64_t Dp01_TL_Mask = ~(Dp01_TL >> 63);
      int64_t Dp12_TL_Mask = ~(Dp12_TL >> 63);
      int64_t Dp20_TL_Mask = ~(Dp20_TL >> 63);
      int64_t Dp01_TR_Mask = ~(Dp01_TR >> 63);
      int64_t Dp12_TR_Mask = ~(Dp12_TR >> 63);
      int64_t Dp20_TR_Mask = ~(Dp20_TR >> 63);
      int64_t Dp01_BL_Mask = ~(Dp01_BL >> 63);
      int64_t Dp12_BL_Mask = ~(Dp12_BL >> 63);
      int64_t Dp20_BL_Mask = ~(Dp20_BL >> 63);
      int64_t Dp01_BR_Mask = ~(Dp01_BR >> 63);
      int64_t Dp12_BR_Mask = ~(Dp12_BR >> 63);
      int64_t Dp20_BR_Mask = ~(Dp20_BR >> 63);

      int64_t TL_Mask = Dp01_TL_Mask & Dp12_TL_Mask & Dp20_TL_Mask;
      int64_t TR_Mask = Dp01_TR_Mask & Dp12_TR_Mask & Dp20_TR_Mask;
      int64_t BL_Mask = Dp01_BL_Mask & Dp12_BL_Mask & Dp20_BL_Mask;
      int64_t BR_Mask = Dp01_BR_Mask & Dp12_BR_Mask & Dp20_BR_Mask;

      int64_t Any_Fragment_Valid = TL_Mask | TR_Mask | BL_Mask | BR_Mask;

      if (Any_Fragment_Valid) {
        // Emit 4 fragments; don't write the pixel if the mask is invalid.
        if (TL_Mask) {
          pixel_TL[0] = pixel_TL[0] ^ 0xFF;
          pixel_TL[1] = 0x00;
          pixel_TL[2] = 0x00;
          pixel_TL[3] = 0xFF;
        }
        if (TR_Mask) {
          pixel_TR[0] = pixel_TR[0] ^ 0xFF;
          pixel_TR[1] = 0x00;
          pixel_TR[2] = 0x00;
          pixel_TR[3] = 0xFF;
        }
        if (BL_Mask) {
          pixel_BL[0] = pixel_BL[0] ^ 0xFF;
          pixel_BL[1] = 0x00;
          pixel_BL[2] = 0x00;
          pixel_BL[3] = 0xFF;
        }
        if (BR_Mask) {
          pixel_BR[0] = pixel_BR[0] ^ 0xFF;
          pixel_BR[1] = 0x00;
          pixel_BR[2] = 0x00;
          pixel_BR[3] = 0xFF;
        }
      }

      Dp01_TL += Dp01_dx_2;
      Dp12_TL += Dp12_dx_2;
      Dp20_TL += Dp20_dx_2;
      Dp01_TR += Dp01_dx_2;
      Dp12_TR += Dp12_dx_2;
      Dp20_TR += Dp20_dx_2;
      Dp01_BL += Dp01_dx_2;
      Dp12_BL += Dp12_dx_2;
      Dp20_BL += Dp20_dx_2;
      Dp01_BR += Dp01_dx_2;
      Dp12_BR += Dp12_dx_2;
      Dp20_BR += Dp20_dx_2;

      // Move over to the next 4 pixels
      pixel_TL += 4 * 2;
      pixel_TR += 4 * 2;
      pixel_BL += 4 * 2;
      pixel_BR += 4 * 2;
    }
    pixel_TL += pixel_mod;
    pixel_TR += pixel_mod;
    pixel_BL += pixel_mod;
    pixel_BR += pixel_mod;
  }
}

void tri3_no_subpixels(uint8_t *rgba, size_t stride,
                       uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
                       int32_t x0, int32_t y0,
                       int32_t x1, int32_t y1,
                       int32_t x2, int32_t y2) {
  tri3(rgba, stride, scissor_left, scissor_top, scissor_right, scissor_bottom,
       x0 << SUBPIXEL_BITS, y0 << SUBPIXEL_BITS,
       x1 << SUBPIXEL_BITS, y1 << SUBPIXEL_BITS,
       x2 << SUBPIXEL_BITS, y2 << SUBPIXEL_BITS);
}

void tri4(uint8_t *rgba, size_t stride,
          uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
          int32_t x0, int32_t y0,
          int32_t x1, int32_t y1,
          int32_t x2, int32_t y2) {
  // Go 4 pixels at a time, the four fragments form a square, the square is valid if a single fragment
  // is inside the triangle and scissor (we will therefore intentionally be generating fragments that
  // are outside the triangle, or the scissor; we will discard those fragments later - reason for this
  // is to be able to do mip-mapping and dFdx/dFdy type stuff.)

  // Be inclusive of odd edges on the scissor rectangle.
  int64_t left = scissor_left & ~(int64_t)1;
  int64_t top = scissor_top & ~(int64_t)1;
  int64_t right = (scissor_right + 1) & ~(int64_t)1;
  int64_t bottom = (scissor_bottom + 1) & ~(int64_t)1;

  // The scissor rectangle is still in pixel coordinates, convert it into sub-pixel coordinates
  int64_t left_sp = left << SUBPIXEL_BITS;
  int64_t top_sp = top << SUBPIXEL_BITS;
  int64_t right_sp = right << SUBPIXEL_BITS;
  int64_t bottom_sp = bottom << SUBPIXEL_BITS;

  int64_t Dp01_row, Dp12_row, Dp20_row;
  // Dp01 = determinant for triangle formed by edge 01 and point p:
  //        | px py 1 |
  // Dp01 = | x0 y0 1 |
  //        | x1 y1 1 |
  Dp01_row = left_sp * (y0 - y1) + top_sp * (x1 - x0) + x0 * y1 - y0 * x1;

  // Compute stepping variables, note that these are at discrete sub-pixels
  int64_t Dp01_dx_sp = (y0 - y1);
  int64_t Dp01_dy_sp = (x1 - x0);

  // Dp12 = determinant for triangle formed by edge 12 and point p:
  //        | px py 1 |
  // Dp12 = | x1 y1 1 |
  //        | x2 y2 1 |
  Dp12_row = left_sp * (y1 - y2) + top_sp * (x2 - x1) + x1 * y2 - y1 * x2;

  // Compute stepping variables, note that these are at discrete sub-pixels
  int64_t Dp12_dx_sp = (y1 - y2);
  int64_t Dp12_dy_sp = (x2 - x1);

  // Dp20 = determinant for triangle formed by edge 20 and point p:
  //        | px py 1 |
  // Dp20 = | x2 y2 1 |
  //        | x0 y0 1 |
  Dp20_row = left_sp * (y2 - y0) + top_sp * (x0 - x2) + x2 * y0 - y2 * x0;

  // Compute stepping variables, note that these are at discrete sub-pixels
  int64_t Dp20_dx_sp = (y2 - y0);
  int64_t Dp20_dy_sp = (x0 - x2);

  // Compute the stepping variables for whole pixels (how much to step when
  // we're at one pixel, and would like to skip (1 << SUBPIXEL_BITS) sub-pixels
  // to get to the same subpixel one whole pixel on whatever stepping dim.)
  int64_t Dp01_dx = Dp01_dx_sp << SUBPIXEL_BITS;
  int64_t Dp01_dy = Dp01_dy_sp << SUBPIXEL_BITS;
  int64_t Dp12_dx = Dp12_dx_sp << SUBPIXEL_BITS;
  int64_t Dp12_dy = Dp12_dy_sp << SUBPIXEL_BITS;
  int64_t Dp20_dx = Dp20_dx_sp << SUBPIXEL_BITS;
  int64_t Dp20_dy = Dp20_dy_sp << SUBPIXEL_BITS;

  // Compute the stepping variables for stepping 2 pixels at a time. This is
  // trivial but something we explicitly want outside the loop.
  int64_t Dp01_dx_2 = Dp01_dx << 1;
  int64_t Dp01_dy_2 = Dp01_dy << 1;
  int64_t Dp12_dx_2 = Dp12_dx << 1;
  int64_t Dp12_dy_2 = Dp12_dy << 1;
  int64_t Dp20_dx_2 = Dp20_dx << 1;
  int64_t Dp20_dy_2 = Dp20_dy << 1;

  // Now compute row values for each of the four pixels.
  int64_t Dp01_row_TL = Dp01_row;
  int64_t Dp01_row_TR = Dp01_row + Dp01_dx;
  int64_t Dp01_row_BL = Dp01_row + Dp01_dy;
  int64_t Dp01_row_BR = Dp01_row + Dp01_dx + Dp01_dy;
  int64_t Dp12_row_TL = Dp12_row;
  int64_t Dp12_row_TR = Dp12_row + Dp12_dx;
  int64_t Dp12_row_BL = Dp12_row + Dp12_dy;
  int64_t Dp12_row_BR = Dp12_row + Dp12_dx + Dp12_dy;
  int64_t Dp20_row_TL = Dp20_row;
  int64_t Dp20_row_TR = Dp20_row + Dp20_dx;
  int64_t Dp20_row_BL = Dp20_row + Dp20_dy;
  int64_t Dp20_row_BR = Dp20_row + Dp20_dx + Dp20_dy;

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

  int64_t px, py;
  uint8_t *pixel_TL = rgba + top * stride + left * 4;
  uint8_t *pixel_TR = pixel_TL + 4;
  uint8_t *pixel_BL = pixel_TL + stride;
  uint8_t *pixel_BR = pixel_BL + 4;
  int64_t pixel_mod = 2 * stride - (right - left) * 4; // bltdmod ;-) Keep in mind we go forward not 1, but 2 rows.

  for (py = top; py < bottom; py += 2) {
    int64_t Dp01_TL, Dp12_TL, Dp20_TL;
    int64_t Dp01_TR, Dp12_TR, Dp20_TR;
    int64_t Dp01_BL, Dp12_BL, Dp20_BL;
    int64_t Dp01_BR, Dp12_BR, Dp20_BR;
    // Copy row values to be ready for column increments
    Dp01_TL = Dp01_row_TL;
    Dp12_TL = Dp12_row_TL;
    Dp20_TL = Dp20_row_TL;
    Dp01_TR = Dp01_row_TR;
    Dp12_TR = Dp12_row_TR;
    Dp20_TR = Dp20_row_TR;
    Dp01_BL = Dp01_row_BL;
    Dp12_BL = Dp12_row_BL;
    Dp20_BL = Dp20_row_BL;
    Dp01_BR = Dp01_row_BR;
    Dp12_BR = Dp12_row_BR;
    Dp20_BR = Dp20_row_BR;
    Dp01_row_TL += Dp01_dy_2;
    Dp12_row_TL += Dp12_dy_2;
    Dp20_row_TL += Dp20_dy_2;
    Dp01_row_TR += Dp01_dy_2;
    Dp12_row_TR += Dp12_dy_2;
    Dp20_row_TR += Dp20_dy_2;
    Dp01_row_BL += Dp01_dy_2;
    Dp12_row_BL += Dp12_dy_2;
    Dp20_row_BL += Dp20_dy_2;
    Dp01_row_BR += Dp01_dy_2;
    Dp12_row_BR += Dp12_dy_2;
    Dp20_row_BR += Dp20_dy_2;
    for (px = left; px < right; px += 2) {
      // Compute the masks for each determinant at each of the four pixels
      // The idea is that if the determinant is positive, then the pixel is
      // inside the triangle; consequently, if the MSB sign bit is zero, then
      // the mask should be all 1's.
      // Because the test is >= 0, we can OR together the sign bits and
      // create a unified mask (if any sign bit is set, then the mask
      // is all zeroes, otherwise it is all ones.)
      int64_t TL_Mask = ~((Dp01_TL | Dp12_TL | Dp20_TL) >> 63);
      int64_t TR_Mask = ~((Dp01_TR | Dp12_TR | Dp20_TR) >> 63);
      int64_t BL_Mask = ~((Dp01_BL | Dp12_BL | Dp20_BL) >> 63);
      int64_t BR_Mask = ~((Dp01_BR | Dp12_BR | Dp20_BR) >> 63);

      int64_t Any_Fragment_Valid = TL_Mask | TR_Mask | BL_Mask | BR_Mask;

      if (Any_Fragment_Valid) {
        // Emit 4 fragments; don't write the pixel if the mask is invalid.
        if (TL_Mask) {
          pixel_TL[0] = pixel_TL[0] ^ 0xFF;
          pixel_TL[1] = 0x00;
          pixel_TL[2] = 0x00;
          pixel_TL[3] = 0xFF;
        }
        if (TR_Mask) {
          pixel_TR[0] = pixel_TR[0] ^ 0xFF;
          pixel_TR[1] = 0x00;
          pixel_TR[2] = 0x00;
          pixel_TR[3] = 0xFF;
        }
        if (BL_Mask) {
          pixel_BL[0] = pixel_BL[0] ^ 0xFF;
          pixel_BL[1] = 0x00;
          pixel_BL[2] = 0x00;
          pixel_BL[3] = 0xFF;
        }
        if (BR_Mask) {
          pixel_BR[0] = pixel_BR[0] ^ 0xFF;
          pixel_BR[1] = 0x00;
          pixel_BR[2] = 0x00;
          pixel_BR[3] = 0xFF;
        }
      }

      Dp01_TL += Dp01_dx_2;
      Dp12_TL += Dp12_dx_2;
      Dp20_TL += Dp20_dx_2;
      Dp01_TR += Dp01_dx_2;
      Dp12_TR += Dp12_dx_2;
      Dp20_TR += Dp20_dx_2;
      Dp01_BL += Dp01_dx_2;
      Dp12_BL += Dp12_dx_2;
      Dp20_BL += Dp20_dx_2;
      Dp01_BR += Dp01_dx_2;
      Dp12_BR += Dp12_dx_2;
      Dp20_BR += Dp20_dx_2;

      // Move over to the next 4 pixels
      pixel_TL += 4 * 2;
      pixel_TR += 4 * 2;
      pixel_BL += 4 * 2;
      pixel_BR += 4 * 2;
    }
    pixel_TL += pixel_mod;
    pixel_TR += pixel_mod;
    pixel_BL += pixel_mod;
    pixel_BR += pixel_mod;
  }
}

void tri4_no_subpixels(uint8_t *rgba, size_t stride,
                       uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
                       int32_t x0, int32_t y0,
                       int32_t x1, int32_t y1,
                       int32_t x2, int32_t y2) {
  tri3(rgba, stride, scissor_left, scissor_top, scissor_right, scissor_bottom,
       x0 << SUBPIXEL_BITS, y0 << SUBPIXEL_BITS,
       x1 << SUBPIXEL_BITS, y1 << SUBPIXEL_BITS,
       x2 << SUBPIXEL_BITS, y2 << SUBPIXEL_BITS);
}


int main(int argc, char **argv) {
  int r;
  r = test();
  if (r) {
    return EXIT_FAILURE;
  }

  static uint8_t rgba32[256 * 256 * 4];
  int row, col;
  for (row = 0; row < 256; row++) {
    for (col = 0; col < 256; col++) {
      uint8_t *rgba = rgba32 + row * 256 * 4 + col * 4;
      rgba[0] = row / 2;
      rgba[1] = 0;
      rgba[2] = row / 4 + 64;
      rgba[3] = 255;
    }
  }

#if 0
  tri2(rgba32, 256 * 4, 
       0, 0, 256, 256, /* scissor rect */
       0, 0,    /* vertex 0 */
       255, 0,  /* vertex 1 */
       0, 255); /* vertex 2 */

  tri(rgba32, 256 * 4,
      0, 0, 256, 256, /* scissor rect */
      0, 0,    /* vertex 0 */
      253, 0,  /* vertex 1 */
      2, 255); /* vertex 2 */
#endif
  int last_col = 0;
  int step = 1;
  for (col = step; col < 256; col += step) {
#if 0
    tri2(rgba32, 256*4,
         0, 0, 256, 256, /* scissor rect */
         last_col, 0,    /* vertex 0 */
         col, 0,         /* vertex 1 */
         0, 255);        /* vertex 2 */
#elif 0
    tri3_no_subpixels(rgba32, 256*4,
                      0, 0, 256, 256, /* scissor rect */
                      last_col, 0,    /* vertex 0 */
                      col, 0,         /* vertex 1 */
                      0, 255);        /* vertex 2 */
#elif 1
    tri4_no_subpixels(rgba32, 256*4,
                      0, 0, 256, 256, /* scissor rect */
                      last_col, 0,    /* vertex 0 */
                      col, 0,         /* vertex 1 */
                      0, 255);        /* vertex 2 */
#else
#endif
    last_col = col;
  }
#if 0
  tri3_no_subpixels(rgba32, 256*4,
                    0, 0, 256, 256, /* scissor rect */
                    0, 0,    /* vertex 0 */
                    16, 0,         /* vertex 1 */
                    0, 16);        /* vertex 2 */
#endif

  /* Superimpose faint grid effect */
  for (row = 0; row < 256; row++) {
    for (col = 0; col < 256; col++) {
      uint8_t *rgba = rgba32 + row * 256 * 4 + col * 4;
      if (row % 16 == 0 || col % 16 == 0) {
#define BRT ((uint32_t)0x40)
#define COMP_MIN(a, b) ((a) < (b) ? (a) : (b))
          rgba[0] = (uint8_t)COMP_MIN(rgba[0] + BRT, 0xFF);
          rgba[1] = (uint8_t)COMP_MIN(rgba[1] + BRT, 0xFF);
          rgba[2] = (uint8_t)COMP_MIN(rgba[2] + BRT, 0xFF);
          rgba[3] = 0xFF;
      }
    }
  }

  FILE *fp = fopen("..\\jig\\test.bmp", "wb"); // relative to project file.
  if (!fp) {
    perror("fopen");
    return EXIT_FAILURE;
  }
  write_rgba_bmp(fp, rgba32, 256, 256, 256 * 4);
  fclose(fp);

  return EXIT_SUCCESS;
}
