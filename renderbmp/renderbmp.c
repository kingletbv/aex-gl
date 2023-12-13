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
  tri4(rgba, stride, scissor_left, scissor_top, scissor_right, scissor_bottom,
       x0 << SUBPIXEL_BITS, y0 << SUBPIXEL_BITS,
       x1 << SUBPIXEL_BITS, y1 << SUBPIXEL_BITS,
       x2 << SUBPIXEL_BITS, y2 << SUBPIXEL_BITS);
}

uint32_t hsvtorgb(uint32_t hue /* range 0 .. 1536 */, uint8_t sat, uint8_t val) {
  uint32_t r, g, b;
  uint32_t region, remainder, p, q, t;

  if (sat == 0) {
    r = val;
    g = val;
    b = val;
    return (r << 16) | (g << 8) | b;
  }

  region = (hue >> 8) % 6;
  remainder = hue & 255;

  p = (val * (255 - sat)) >> 8;
  q = (val * (255 - ((sat * remainder) >> 8))) >> 8;
  t = (val * (255 - ((sat * (255 - remainder)) >> 8))) >> 8;

  switch (region) {
    case 0:
      r = val; g = t; b = p;
      break;
    case 1:
      r = q; g = val; b = p;
      break;
    case 2:
      r = p; g = val; b = t;
      break;
    case 3:
      r = p; g = q; b = val;
      break;
    case 4:
      r = t; g = p; b = val;
      break;
    default:
      r = val; g = p; b = q;
      break;
  }

  return (r << 16) | (g << 8) | b;
}

void tri5(uint8_t *rgba, size_t stride,
          uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
          int32_t x0, int32_t y0, uint32_t z0,
          int32_t x1, int32_t y1, uint32_t z1,
          int32_t x2, int32_t y2, uint32_t z2) {
  int64_t D012;
  // D012 = determinant of x and y coordinates (this is twice the area of the triangle (e.g. the area of the paralellogram))
  //        | x0 y0 1 |
  // D012 = | x1 y1 1 |
  //        | x2 y2 1 |
  D012 = ((int64_t)x1) * ((int64_t)y2) - ((int64_t)x2) * ((int64_t)y1) - ((int64_t)x0) * ((int64_t)y2) + ((int64_t)x2) * ((int64_t)y0) + ((int64_t)x0) * ((int64_t)y1) - ((int64_t)x1) * ((int64_t)y0);

  if (D012 <= 0) {
    // Counterclockwise backface, or colinear. Reject.
    return;
  }

  int64_t Dzx, Dzy, Dxyz;
  // Dzx = determinant of z and y coordinates (change in numerator for each successive column (x))
  //       | z0 y0 1 |
  // Dzx = | z1 y1 1 |
  //       | z2 y2 1 |
  Dzx = ((int64_t)z1) * ((int64_t)y2) - ((int64_t)z2) * ((int64_t)y1) - ((int64_t)z0) * ((int64_t)y2) + ((int64_t)z2) * ((int64_t)y0) + ((int64_t)z0) * ((int64_t)y1) - ((int64_t)z1) * ((int64_t)y0);

  // Dzy = determinant of x and z coordinates (change in numerator for each successive row (y))
  //       | x0 z0 1 |
  // Dzy = | x1 z1 1 |
  //       | x2 z2 1 |
  Dzy = ((int64_t)x1) * ((int64_t)z2) - ((int64_t)x2) * ((int64_t)z1) - ((int64_t)x0) * ((int64_t)z2) + ((int64_t)x2) * ((int64_t)z0) + ((int64_t)x0) * ((int64_t)z1) - ((int64_t)x1) * ((int64_t)z0);

  // Dxyz = determinant of x, y and z-buffer coordinates
  //        | x0 y0 z0 |
  // Dxyz = | x1 y1 z1 |
  //        | x2 y2 z2 |
  Dxyz = ((int64_t)x0) * ( ((int64_t)y1) * ((int64_t)z2) - ((int64_t)y2) * ((int64_t)z1) )
       - ((int64_t)x1) * ( ((int64_t)y0) * ((int64_t)z2) - ((int64_t)y2) * ((int64_t)z0) )
       + ((int64_t)x2) * ( ((int64_t)y0) * ((int64_t)z1) - ((int64_t)y1) * ((int64_t)z0) );

  int64_t z = Dzx * ((int64_t)(scissor_left)) + Dzy * ((int64_t)(scissor_top)) + Dxyz;
  int64_t z_r = z % D012;
  z = z / D012;

  int64_t z_s; // stepper variable; outer loop is rows so we start with initialization for Y.

  int64_t z_yi;
  int64_t z_yp;
  int64_t z_yq;
  int direction_xy_flips;
  z_yq = Dzy / D012;
  if (Dzy > 0) {
    z_s = D012 - z_r - 1;
    z_yi = 1;
    direction_xy_flips = 1;
  }
  else if (Dzy < 0) {
    z_s = z_r;
    z_yi = -1;
    Dzy = -Dzy;
    direction_xy_flips = 0;
  }
  else /* (Dzy == 0) */ {
    z_s = z_r;
    z_yi = 0;
    direction_xy_flips = 0;
    z_yp = 0;
  }
  z_yp = Dzy % D012;

  int64_t z_xi;
  int64_t z_xp;
  int64_t z_xq;
  z_xq = Dzx / D012;
  if (Dzx > 0) {
    z_xi = 1;
    direction_xy_flips ^= 1;
  }
  else if (Dzx < 0) {
    z_xi = -1;
    Dzx = -Dzx;
  }
  else /* (Dzx == 0) */ {
    z_xi = 0;
    z_xp = 0;
  }
  z_xp = Dzx % D012;

  int64_t Dp01_row, Dp12_row, Dp20_row;
  // Dp01 = determinant for triangle formed by edge 01 and point p:
  //        | px py 1 |
  // Dp01 = | x0 y0 1 |
  //        | x1 y1 1 |
  int64_t Dp01_dx = (y0 - y1);
  int64_t Dp01_dy = (x1 - x0);
  Dp01_row = ((int64_t)(scissor_left)) * Dp01_dx + ((int64_t)(scissor_top)) * Dp01_dy + x0 * y1 - y0 * x1;


  // Dp12 = determinant for triangle formed by edge 12 and point p:
  //        | px py 1 |
  // Dp12 = | x1 y1 1 |
  //        | x2 y2 1 |
  int64_t Dp12_dx = (y1 - y2);
  int64_t Dp12_dy = (x2 - x1);
  Dp12_row = ((int64_t)(scissor_left)) * Dp12_dx + ((int64_t)(scissor_top)) * Dp12_dy + x1 * y2 - y1 * x2;

  // Dp20 = determinant for triangle formed by edge 20 and point p:
  //        | px py 1 |
  // Dp20 = | x2 y2 1 |
  //        | x0 y0 1 |
  int64_t Dp20_dx = (y2 - y0);
  int64_t Dp20_dy = (x0 - x2);
  Dp20_row = ((int64_t)(scissor_left)) * Dp20_dx + ((int64_t)(scissor_top)) * Dp20_dy + x2 * y0 - y2 * x0;

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

    int64_t z_sx;
    if (direction_xy_flips) {
      /* Flip numerator to column (x) direction */
      z_sx = D012 - z_s - 2;
    }
    else {
      /* x and y are both ascending, or both descending, keep as-is */
      z_sx = z_s;
    }
    int64_t z_x = z;

    /* Step z_s to next row */
    z += z_yq;
    z_s -= z_yp;
    int64_t step_mask = z_s >> 63;
    z_s += D012 & step_mask;
    z += z_yi & step_mask;

    for (px = scissor_left; px < scissor_right; ++px) {
      uint8_t *pixel = rgba + py * stride + px * 4;
      if (Dp01 > 0 && Dp12 > 0 && Dp20 > 0) {
        uint32_t clr = hsvtorgb((uint32_t)z_x, 255, 255);
        pixel[0] = (uint8_t)(clr >> 16);
        pixel[1] = (uint8_t)(clr >> 8);
        pixel[2] = (uint8_t)(clr);
        pixel[3] = 0xFF;
      }

      Dp01 += Dp01_dx;
      Dp12 += Dp12_dx;
      Dp20 += Dp20_dx;

      /* Step z_x to next column */
      z_x += z_xq;
      z_sx -= z_xp;
      step_mask = z_sx >> 63;
      z_sx += D012 & step_mask;
      z_x += z_xi & step_mask;
    }
  }
}

int mulu64(uint64_t a, uint64_t b, uint64_t *rhi, uint64_t *rlo){ 
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
    z2 ++;
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

int muls64(int64_t a, int64_t b, int64_t *rhi, int64_t *rlo) {
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
    if (!result_lo /* following +1, is carry set? */ ) {
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

/* Performs an unsigned division of a 128 bit numerator by a 64 bit denominator, returning
 * a 128 bit number, split into a high and a low 64 bit number.
 * Return value == -1 for division by zero.
 *                  1 if the result requires more than 64 bits
 *                  0 if the result requires less than 64 bits.
 * *rhi is optional, you can check the return value for 1 to see if the result fits into *rlo.
 */
int divu128by64(uint64_t numhi, uint64_t numlo, uint64_t den, uint64_t *rhi, uint64_t *rlo) {
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

  /* Return 0 if result fits in 64 bits, 1 if more bits are needed. */
  return (r[3] || r[2]) ? 1 : 0;
}



/* Performs an unsigned division of a 128 bit numerator by a 64 bit denominator, returning
 * a 128 bit number, split into a high and a low 64 bit number, and the remainder as a
 * 64 bit number.
 * Return value == -1 for division by zero.
 *                  1 if the result requires more than 64 bits
 *                  0 if the result requires less than 64 bits.
 * *rhi is optional, you can check the return value for 1 to see if the result fits into *rlo.
 */
int divremu128by64(uint64_t numhi, uint64_t numlo, uint64_t den, uint64_t *rhi, uint64_t *rlo, uint64_t *rem) {
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

/* Performs a signed division of a 128 bit numerator by a 64 bit denominator, returning
 * a 128 bit number, split into a high and a low 64 bit number.
 * Return value == -1 for division by zero.
 *                  1 if the result requires more than 64 bits
 *                  0 if the result requires less than 64 bits.
 * *rhi is optional, you can check the return value for 1 to see if the result fits into *rlo.
 */
int divs128by64(int64_t numhi, int64_t numlo, int64_t den, int64_t *rhi, int64_t *rlo) {
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
  int r = divu128by64((uint64_t)numhi, (uint64_t)numlo, (uint64_t)den, (uint64_t *)&result_hi, (uint64_t *)&result_lo);
  if (is_neg) {
    // Two's complement negation.
    result_lo = ~result_lo;
    result_hi = ~result_hi;
    result_lo++;
    if (!result_lo /* following +1, is carry set? */) {
      result_hi++;
    }
  }
  if (rhi) {
    *rhi = result_hi;
  }
  if (rlo) {
    *rlo = result_lo;
  }
  return r;
}

int divrems128by64(int64_t numhi, int64_t numlo, int64_t den, int64_t *rhi, int64_t *rlo, int64_t *rem) {
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

int divmods128by64(int64_t numhi, int64_t numlo, int64_t den, int64_t *rhi, int64_t *rlo, int64_t *mod) {
  int r;
  int64_t remainder = 0;
  r = divrems128by64(numhi, numlo, den, rhi, rlo, &remainder);
  if (remainder < 0) {
    remainder += den;
  }
  if (mod) *mod = remainder;
  return r;
}


void addu128(uint64_t ahi, uint64_t alo, uint64_t bhi, uint64_t blo, uint64_t *rhi, uint64_t *rlo) {
  uint64_t result_lo = alo + blo;
  uint64_t result_hi = ahi + bhi;
  if (result_lo < alo) {
    /* carry set */
    result_hi++;
  }
  if (rhi) {
    *rhi = result_hi;
  }
  if (rlo) {
    *rlo = result_lo;
  }
}

void adds128(int64_t ahi, int64_t alo, int64_t bhi, int64_t blo, int64_t *rhi, int64_t *rlo) {
  /* Implement signed addition as unsigned addition (identical if reg sizes are same) */
  addu128((uint64_t)ahi, (uint64_t)alo, (uint64_t)bhi, (uint64_t)blo, (uint64_t*)rhi, (uint64_t*)rlo);
}

void adds128by64(int64_t ahi, int64_t alo, int64_t b, int64_t *rhi, int64_t *rlo) {
  /* the "s" in adds128by64 is relevant here as we sign-extend b to 128 bits; this would
   * not be done for an unsigned variant. */
  int64_t bhi = b >> 63;
  addu128((uint64_t)ahi, (uint64_t)alo, (uint64_t)bhi, (uint64_t)b, (uint64_t *)rhi, (uint64_t *)rlo);
}

void neg128(int64_t hi, int64_t lo, int64_t *rhi, int64_t *rlo) {
  /* Two's complement negation */
  lo = ~lo;
  hi = ~hi;
  lo++;
  if (!lo /* following +1, is carry set? */) {
    hi++;
  }
  *rhi = hi;
  *rlo = lo;
}

void subs128(int64_t ahi, int64_t alo, int64_t bhi, int64_t blo, int64_t *rhi, int64_t *rlo) {
  /* Two's complement negation */
  blo = ~blo;
  bhi = ~bhi;
  blo++;
  if (!blo /* following +1, is carry set? */) {
    bhi++;
  }

  /* adding the negated value is the same as subtracting */
  adds128(ahi, alo, bhi, blo, rhi, rlo);
}

void mulu128(uint64_t ahi, uint64_t alo, uint64_t bhi, uint64_t blo, uint64_t *rhi, uint64_t *rlo) {
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

void muls128(int64_t ahi, int64_t alo, int64_t bhi, int64_t blo, int64_t *rhi, int64_t *rlo) {
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

void tri6(uint8_t *rgba, size_t stride,
          uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
          int32_t x0, int32_t y0, uint32_t z0,
          int32_t x1, int32_t y1, uint32_t z1,
          int32_t x2, int32_t y2, uint32_t z2) {
  int64_t D012;

  // Location where we go "Pen down" - convenience short-hand.
  int64_t Px = (int64_t)scissor_left;
  int64_t Py = (int64_t)scissor_top;

  // D012 = determinant of x and y coordinates (this is twice the area of the triangle (e.g. the area of the paralellogram))
  //        | x0 y0 1 |
  // D012 = | x1 y1 1 |
  //        | x2 y2 1 |
  D012 = ((int64_t)x1) * ((int64_t)y2) - ((int64_t)x2) * ((int64_t)y1) - ((int64_t)x0) * ((int64_t)y2) + ((int64_t)x2) * ((int64_t)y0) + ((int64_t)x0) * ((int64_t)y1) - ((int64_t)x1) * ((int64_t)y0);

  if (D012 <= 0) {
    // Counterclockwise backface, or colinear. Reject.
    return;
  }

  int64_t Dzx, Dzy, Dxyz;
  // Dzx = determinant of z and y coordinates (change in numerator for each successive column (x))
  //       | z0 y0 1 |
  // Dzx = | z1 y1 1 |
  //       | z2 y2 1 |
  Dzx = ((int64_t)z1) * ((int64_t)y2) - ((int64_t)z2) * ((int64_t)y1) - ((int64_t)z0) * ((int64_t)y2) + ((int64_t)z2) * ((int64_t)y0) + ((int64_t)z0) * ((int64_t)y1) - ((int64_t)z1) * ((int64_t)y0);

  int64_t Dzx_1 =  ((int64_t)z1) * ((int64_t)y2) /* 30 + 32 = 62 bits */;
  int64_t Dzx_2 = -((int64_t)z2) * ((int64_t)y1) /* 30 + 32 = 62 bits */;
  int64_t Dzx_3 = -((int64_t)z0) * ((int64_t)y2) /* 30 + 32 = 62 bits */;
  int64_t Dzx_4 =  ((int64_t)z2) * ((int64_t)y0) /* 30 + 32 = 62 bits */;
  int64_t Dzx_5 =  ((int64_t)z0) * ((int64_t)y1) /* 30 + 32 = 62 bits */;
  int64_t Dzx_6 = -((int64_t)z1) * ((int64_t)y0) /* 30 + 32 = 62 bits */;

  int64_t Dzx_a = Dzx_1 + Dzx_2 + Dzx_3 /* 62 + 62 + 62 = 64 bits */;
  int64_t Dzx_b = Dzx_4 + Dzx_5 + Dzx_6 /* 62 + 62 + 62 = 64 bits */;
  int64_t Dzx_lo, Dzx_hi;
  adds128(Dzx_a>>63, Dzx_a, Dzx_b>>63, Dzx_b, &Dzx_hi, &Dzx_lo);

  int64_t Dzx_Px_a_hi, Dzx_Px_a_lo;
  int64_t Dzx_Px_b_hi, Dzx_Px_b_lo;
  muls64(Dzx_a, Px, &Dzx_Px_a_hi, &Dzx_Px_a_lo);
  muls64(Dzx_b, Px, &Dzx_Px_b_hi, &Dzx_Px_b_lo);
  int64_t Dzx_Px_hi, Dzx_Px_lo;
  adds128(Dzx_Px_a_hi, Dzx_Px_a_lo, Dzx_Px_b_hi, Dzx_Px_b_lo, &Dzx_Px_hi, &Dzx_Px_lo);

  // Dzy = determinant of x and z coordinates (change in numerator for each successive row (y))
  //       | x0 z0 1 |
  // Dzy = | x1 z1 1 |
  //       | x2 z2 1 |
  Dzy = ((int64_t)x1) * ((int64_t)z2) - ((int64_t)x2) * ((int64_t)z1) - ((int64_t)x0) * ((int64_t)z2) + ((int64_t)x2) * ((int64_t)z0) + ((int64_t)x0) * ((int64_t)z1) - ((int64_t)x1) * ((int64_t)z0);

  int64_t Dzy_1 =  ((int64_t)x1) * ((int64_t)z2) /* 30 + 32 = 62 bits */;
  int64_t Dzy_2 = -((int64_t)x2) * ((int64_t)z1) /* 30 + 32 = 62 bits */;
  int64_t Dzy_3 = -((int64_t)x0) * ((int64_t)z2) /* 30 + 32 = 62 bits */;
  int64_t Dzy_4 =  ((int64_t)x2) * ((int64_t)z0) /* 30 + 32 = 62 bits */;
  int64_t Dzy_5 =  ((int64_t)x0) * ((int64_t)z1) /* 30 + 32 = 62 bits */;
  int64_t Dzy_6 = -((int64_t)x1) * ((int64_t)z0) /* 30 + 32 = 62 bits */;

  int64_t Dzy_a = Dzy_1 + Dzy_2 + Dzy_3 /* 62 + 62 + 62 = 64 bits */;
  int64_t Dzy_b = Dzy_4 + Dzy_5 + Dzy_6 /* 62 + 62 + 62 = 64 bits */;
  int64_t Dzy_lo, Dzy_hi;               /* 64 + 64 = 65 bits */
  adds128(Dzy_a>>63, Dzy_a, Dzy_b>>63, Dzy_b, &Dzy_hi, &Dzy_lo);

  int64_t Dzy_Py_a_hi, Dzy_Py_a_lo;
  int64_t Dzy_Py_b_hi, Dzy_Py_b_lo;
  muls64(Dzy_a, Py, &Dzy_Py_a_hi, &Dzy_Py_a_lo);
  muls64(Dzy_b, Py, &Dzy_Py_b_hi, &Dzy_Py_b_lo);
  int64_t Dzy_Py_hi, Dzy_Py_lo;
  adds128(Dzy_Py_a_hi, Dzy_Py_a_lo, Dzy_Py_b_hi, Dzy_Py_b_lo, &Dzy_Py_hi, &Dzy_Py_lo);

  // Dxyz = determinant of x, y and z-buffer coordinates
  //        | x0 y0 z0 |
  // Dxyz = | x1 y1 z1 |
  //        | x2 y2 z2 |
  Dxyz = ((int64_t)x0) * (((int64_t)y1) * ((int64_t)z2) - ((int64_t)y2) * ((int64_t)z1))
       - ((int64_t)x1) * (((int64_t)y0) * ((int64_t)z2) - ((int64_t)y2) * ((int64_t)z0))
       + ((int64_t)x2) * (((int64_t)y0) * ((int64_t)z1) - ((int64_t)y1) * ((int64_t)z0));

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

  // z_num = Dzx * Px + Dzy * Py + Dxyz
  // we have the first two terms as Dzx_Px_hi/lo and Dzy_Py_hi/lo, and have Dzxy_hi/lo, but need to add them all together.
  int64_t z_num_hi, z_num_lo;
  adds128(Dzx_Px_hi, Dzx_Px_lo, Dzy_Py_hi, Dzy_Py_lo, &z_num_hi, &z_num_lo);
  adds128(z_num_hi, z_num_lo, Dxyz_hi, Dxyz_lo, &z_num_hi, &z_num_lo);

  int64_t z_num = Dzx * ((int64_t)(scissor_left)) + Dzy * ((int64_t)(scissor_top)) + Dxyz;
  int64_t z_r = z_num % D012;
  if (z_r < 0) z_r += D012; /* convert remainder to modulo */
  int64_t z;
  z = z_num / D012;

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
    // many incremental, overflowing, steps) will always be in the range of 0 to D012-1. The one
    // exception to this is when we start processing fragments in quadruples, in which case some of the
    // fragments will be outside the triangle (and we would record the z-buffer value and start processing
    // the fragment as-if it had passed for sake of the fragments that did, but would not use the z-buffer
    // value itself as part of the z-buffer test.)
    ;
  }
    
#if 1
  z_r = z_mod;
  z = z_lo;
#endif

  int64_t z_s; // stepper variable; outer loop is rows so we start with initialization for Y.

  int64_t z_yi;
  int64_t z_yp;
  int64_t z_yq;
  int direction_xy_flips;
  z_yq = Dzy / D012;
  if ((Dzy_hi > 0) || ((Dzy_hi == 0) && (Dzy_lo > 0))) {
  //if (Dzy > 0) {
    z_s = D012 - z_r - 1;
    z_yi = 1;
    direction_xy_flips = 1;
  }
  else if (Dzy_hi < 0) {
    z_s = z_r;
    z_yi = -1;
    neg128(Dzy_hi, Dzy_lo, &Dzy_hi, &Dzy_lo);
    Dzy = -Dzy;
    direction_xy_flips = 0;
  }
  else /* (Dzy_hi == 0 && Dzy_lo == 0) */ {
    z_s = z_r;
    z_yi = 0;
    direction_xy_flips = 0;
    z_yp = 0;
  }
  z_yp = Dzy % D012;
  divmods128by64(Dzy_hi, Dzy_lo, D012, NULL, NULL, &z_yp);

  int64_t z_xi;
  int64_t z_xp;
  int64_t z_xq;
  z_xq = Dzx / D012;
  if ((Dzx_hi > 0) || ((Dzx_hi == 0) && (Dzx_lo > 0))) {
  //if (Dzx > 0) {
    z_xi = 1;
    direction_xy_flips ^= 1;
  }
  else if (Dzx < 0) {
    z_xi = -1;
    neg128(Dzx_hi, Dzx_lo, &Dzx_hi, &Dzx_lo);
    Dzx = -Dzx;
  }
  else /* (Dzx == 0) */ {
    z_xi = 0;
    z_xp = 0;
  }
  z_xp = Dzx % D012;
  divmods128by64(Dzx_hi, Dzx_lo, D012, NULL, NULL, &z_xp);

  int64_t Dp01_row, Dp12_row, Dp20_row;
  // Dp01 = determinant for triangle formed by edge 01 and point p:
  //        | px py 1 |
  // Dp01 = | x0 y0 1 |
  //        | x1 y1 1 |
  int64_t Dp01_dx = (y0 - y1);
  int64_t Dp01_dy = (x1 - x0);
  Dp01_row = ((int64_t)(scissor_left)) * Dp01_dx + ((int64_t)(scissor_top)) * Dp01_dy + x0 * y1 - y0 * x1;


  // Dp12 = determinant for triangle formed by edge 12 and point p:
  //        | px py 1 |
  // Dp12 = | x1 y1 1 |
  //        | x2 y2 1 |
  int64_t Dp12_dx = (y1 - y2);
  int64_t Dp12_dy = (x2 - x1);
  Dp12_row = ((int64_t)(scissor_left)) * Dp12_dx + ((int64_t)(scissor_top)) * Dp12_dy + x1 * y2 - y1 * x2;

  // Dp20 = determinant for triangle formed by edge 20 and point p:
  //        | px py 1 |
  // Dp20 = | x2 y2 1 |
  //        | x0 y0 1 |
  int64_t Dp20_dx = (y2 - y0);
  int64_t Dp20_dy = (x0 - x2);
  Dp20_row = ((int64_t)(scissor_left)) * Dp20_dx + ((int64_t)(scissor_top)) * Dp20_dy + x2 * y0 - y2 * x0;

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

    int64_t z_sx;
    if (direction_xy_flips) {
      /* Flip numerator to column (x) direction */
      z_sx = D012 - z_s - 2;
    }
    else {
      /* x and y are both ascending, or both descending, keep as-is */
      z_sx = z_s;
    }
    int64_t z_x = z;

    /* Step z_s to next row */
    z += z_yq;
    z_s -= z_yp;
    int64_t step_mask = z_s >> 63;
    z_s += D012 & step_mask;
    z += z_yi & step_mask;

    for (px = scissor_left; px < scissor_right; ++px) {
      uint8_t *pixel = rgba + py * stride + px * 4;
      if (Dp01 > 0 && Dp12 > 0 && Dp20 > 0) {
        uint32_t clr = hsvtorgb((uint32_t)z_x, 255, 255);
        pixel[0] = (uint8_t)(clr >> 16);
        pixel[1] = (uint8_t)(clr >> 8);
        pixel[2] = (uint8_t)(clr);
        pixel[3] = 0xFF;
      }

      Dp01 += Dp01_dx;
      Dp12 += Dp12_dx;
      Dp20 += Dp20_dx;

      /* Step z_x to next column */
      z_x += z_xq;
      z_sx -= z_xp;
      step_mask = z_sx >> 63;
      z_sx += D012 & step_mask;
      z_x += z_xi & step_mask;
    }
  }
}


struct tri7_sample {
  // at point (x,y)
  int32_t x_;
  int32_t y_;
  // the z-buf value should be this
  uint64_t z_buf_value_;
  // and the z-buf numerator should be this
  uint64_t z_buf_num_;

  // area to record the observed values.
  uint64_t z_buf_value_recorded_;
  uint64_t z_buf_num_recorded_;
};

void tri7(uint8_t *rgba, size_t stride,
          uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
          int32_t x0, int32_t y0, uint32_t z0,
          int32_t x1, int32_t y1, uint32_t z1,
          int32_t x2, int32_t y2, uint32_t z2,
          size_t num_samples, struct tri7_sample *samples) {
  int64_t D012;

  // Location where we go "Pen down" - convenience short-hand.
  int64_t Px = (int64_t)scissor_left;
  int64_t Py = (int64_t)scissor_top;

  // D012 = determinant of x and y coordinates (this is twice the area of the triangle (e.g. the area of the paralellogram))
  //        | x0 y0 1 |
  // D012 = | x1 y1 1 |
  //        | x2 y2 1 |
  D012 = ((int64_t)x1) * ((int64_t)y2) - ((int64_t)x2) * ((int64_t)y1) - ((int64_t)x0) * ((int64_t)y2) + ((int64_t)x2) * ((int64_t)y0) + ((int64_t)x0) * ((int64_t)y1) - ((int64_t)x1) * ((int64_t)y0);

  if (D012 <= 0) {
    // Counterclockwise backface, or colinear. Reject.
    return;
  }

  int64_t Dzx, Dzy, Dxyz;
  // Dzx = determinant of z and y coordinates (change in numerator for each successive column (x))
  //       | z0 y0 1 |
  // Dzx = | z1 y1 1 |
  //       | z2 y2 1 |
  Dzx = ((int64_t)z1) * ((int64_t)y2) - ((int64_t)z2) * ((int64_t)y1) - ((int64_t)z0) * ((int64_t)y2) + ((int64_t)z2) * ((int64_t)y0) + ((int64_t)z0) * ((int64_t)y1) - ((int64_t)z1) * ((int64_t)y0);

  int64_t Dzx_1 =  ((int64_t)z1) * ((int64_t)y2) /* 30 + 32 = 62 bits */;
  int64_t Dzx_2 = -((int64_t)z2) * ((int64_t)y1) /* 30 + 32 = 62 bits */;
  int64_t Dzx_3 = -((int64_t)z0) * ((int64_t)y2) /* 30 + 32 = 62 bits */;
  int64_t Dzx_4 =  ((int64_t)z2) * ((int64_t)y0) /* 30 + 32 = 62 bits */;
  int64_t Dzx_5 =  ((int64_t)z0) * ((int64_t)y1) /* 30 + 32 = 62 bits */;
  int64_t Dzx_6 = -((int64_t)z1) * ((int64_t)y0) /* 30 + 32 = 62 bits */;

  int64_t Dzx_a = Dzx_1 + Dzx_2 + Dzx_3 /* 62 + 62 + 62 = 64 bits */;
  int64_t Dzx_b = Dzx_4 + Dzx_5 + Dzx_6 /* 62 + 62 + 62 = 64 bits */;
  int64_t Dzx_lo, Dzx_hi;
  adds128(Dzx_a>>63, Dzx_a, Dzx_b>>63, Dzx_b, &Dzx_hi, &Dzx_lo);

  int64_t Dzx_Px_a_hi, Dzx_Px_a_lo;
  int64_t Dzx_Px_b_hi, Dzx_Px_b_lo;
  muls64(Dzx_a, Px, &Dzx_Px_a_hi, &Dzx_Px_a_lo);
  muls64(Dzx_b, Px, &Dzx_Px_b_hi, &Dzx_Px_b_lo);
  int64_t Dzx_Px_hi, Dzx_Px_lo;
  adds128(Dzx_Px_a_hi, Dzx_Px_a_lo, Dzx_Px_b_hi, Dzx_Px_b_lo, &Dzx_Px_hi, &Dzx_Px_lo);

  // Dzy = determinant of x and z coordinates (change in numerator for each successive row (y))
  //       | x0 z0 1 |
  // Dzy = | x1 z1 1 |
  //       | x2 z2 1 |
  Dzy = ((int64_t)x1) * ((int64_t)z2) - ((int64_t)x2) * ((int64_t)z1) - ((int64_t)x0) * ((int64_t)z2) + ((int64_t)x2) * ((int64_t)z0) + ((int64_t)x0) * ((int64_t)z1) - ((int64_t)x1) * ((int64_t)z0);

  int64_t Dzy_1 =  ((int64_t)x1) * ((int64_t)z2) /* 30 + 32 = 62 bits */;
  int64_t Dzy_2 = -((int64_t)x2) * ((int64_t)z1) /* 30 + 32 = 62 bits */;
  int64_t Dzy_3 = -((int64_t)x0) * ((int64_t)z2) /* 30 + 32 = 62 bits */;
  int64_t Dzy_4 =  ((int64_t)x2) * ((int64_t)z0) /* 30 + 32 = 62 bits */;
  int64_t Dzy_5 =  ((int64_t)x0) * ((int64_t)z1) /* 30 + 32 = 62 bits */;
  int64_t Dzy_6 = -((int64_t)x1) * ((int64_t)z0) /* 30 + 32 = 62 bits */;

  int64_t Dzy_a = Dzy_1 + Dzy_2 + Dzy_3 /* 62 + 62 + 62 = 64 bits */;
  int64_t Dzy_b = Dzy_4 + Dzy_5 + Dzy_6 /* 62 + 62 + 62 = 64 bits */;
  int64_t Dzy_lo, Dzy_hi;               /* 64 + 64 = 65 bits */
  adds128(Dzy_a>>63, Dzy_a, Dzy_b>>63, Dzy_b, &Dzy_hi, &Dzy_lo);

  int64_t Dzy_Py_a_hi, Dzy_Py_a_lo;
  int64_t Dzy_Py_b_hi, Dzy_Py_b_lo;
  muls64(Dzy_a, Py, &Dzy_Py_a_hi, &Dzy_Py_a_lo);
  muls64(Dzy_b, Py, &Dzy_Py_b_hi, &Dzy_Py_b_lo);
  int64_t Dzy_Py_hi, Dzy_Py_lo;
  adds128(Dzy_Py_a_hi, Dzy_Py_a_lo, Dzy_Py_b_hi, Dzy_Py_b_lo, &Dzy_Py_hi, &Dzy_Py_lo);

  // Dxyz = determinant of x, y and z-buffer coordinates
  //        | x0 y0 z0 |
  // Dxyz = | x1 y1 z1 |
  //        | x2 y2 z2 |
  Dxyz = ((int64_t)x0) * (((int64_t)y1) * ((int64_t)z2) - ((int64_t)y2) * ((int64_t)z1))
    - ((int64_t)x1) * (((int64_t)y0) * ((int64_t)z2) - ((int64_t)y2) * ((int64_t)z0))
    + ((int64_t)x2) * (((int64_t)y0) * ((int64_t)z1) - ((int64_t)y1) * ((int64_t)z0));

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

  // z_num = Dzx * Px + Dzy * Py + Dxyz
  // we have the first two terms as Dzx_Px_hi/lo and Dzy_Py_hi/lo, and have Dzxy_hi/lo, but need to add them all together.
  int64_t z_num_hi, z_num_lo;
  adds128(Dzx_Px_hi, Dzx_Px_lo, Dzy_Py_hi, Dzy_Py_lo, &z_num_hi, &z_num_lo);
  adds128(z_num_hi, z_num_lo, Dxyz_hi, Dxyz_lo, &z_num_hi, &z_num_lo);

  int64_t z_num = Dzx * ((int64_t)(scissor_left)) + Dzy * ((int64_t)(scissor_top)) + Dxyz;
  int64_t z_r = z_num % D012;
  if (z_r < 0) z_r += D012; /* convert remainder to modulo */
  int64_t z;
  z = z_num / D012;

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
    // many incremental, overflowing, steps) will always be in the range of 0 to D012-1. The one
    // exception to this is when we start processing fragments in quadruples, in which case some of the
    // fragments will be outside the triangle (and we would record the z-buffer value and start processing
    // the fragment as-if it had passed for sake of the fragments that did, but would not use the z-buffer
    // value itself as part of the z-buffer test.)
    ;
  }

#if 1
  z_r = z_mod;
  z = z_lo;
#endif

  int64_t z_s; // stepper variable; outer loop is rows so we start with initialization for Y.

  int64_t z_yi;
  int64_t z_yp;
  int64_t z_yq;
  int direction_xy_flips;
  z_yq = Dzy / D012;
  if ((Dzy_hi > 0) || ((Dzy_hi == 0) && (Dzy_lo > 0))) {
  //if (Dzy > 0) {
    z_s = D012 - z_r - 1;
    z_yi = 1;
    direction_xy_flips = 1;
  }
  else if (Dzy_hi < 0) {
    z_s = z_r;
    z_yi = -1;
    neg128(Dzy_hi, Dzy_lo, &Dzy_hi, &Dzy_lo);
    Dzy = -Dzy;
    direction_xy_flips = 0;
  }
  else /* (Dzy_hi == 0 && Dzy_lo == 0) */ {
    z_s = z_r;
    z_yi = 0;
    direction_xy_flips = 0;
    z_yp = 0;
  }
  z_yp = Dzy % D012;
  divmods128by64(Dzy_hi, Dzy_lo, D012, NULL, NULL, &z_yp);

  int64_t z_xi;
  int64_t z_xp;
  int64_t z_xq;
  z_xq = Dzx / D012;
  if ((Dzx_hi > 0) || ((Dzx_hi == 0) && (Dzx_lo > 0))) {
  //if (Dzx > 0) {
    z_xi = 1;
    direction_xy_flips ^= 1;
  }
  else if (Dzx < 0) {
    z_xi = -1;
    neg128(Dzx_hi, Dzx_lo, &Dzx_hi, &Dzx_lo);
    Dzx = -Dzx;
  }
  else /* (Dzx == 0) */ {
    z_xi = 0;
    z_xp = 0;
  }
  z_xp = Dzx % D012;
  divmods128by64(Dzx_hi, Dzx_lo, D012, NULL, NULL, &z_xp);

  int64_t Dp01_row, Dp12_row, Dp20_row;
  // Dp01 = determinant for triangle formed by edge 01 and point p:
  //        | px py 1 |
  // Dp01 = | x0 y0 1 |
  //        | x1 y1 1 |
  int64_t Dp01_dx = (y0 - y1);
  int64_t Dp01_dy = (x1 - x0);
  Dp01_row = ((int64_t)(scissor_left)) * Dp01_dx + ((int64_t)(scissor_top)) * Dp01_dy + ((int64_t)x0) * ((int64_t)y1) - ((int64_t)y0) * ((int64_t)x1);


  // Dp12 = determinant for triangle formed by edge 12 and point p:
  //        | px py 1 |
  // Dp12 = | x1 y1 1 |
  //        | x2 y2 1 |
  int64_t Dp12_dx = (y1 - y2);
  int64_t Dp12_dy = (x2 - x1);
  Dp12_row = ((int64_t)(scissor_left)) * Dp12_dx + ((int64_t)(scissor_top)) * Dp12_dy + ((int64_t)x1) * ((int64_t)y2) - ((int64_t)y1) * ((int64_t)x2);

  // Dp20 = determinant for triangle formed by edge 20 and point p:
  //        | px py 1 |
  // Dp20 = | x2 y2 1 |
  //        | x0 y0 1 |
  int64_t Dp20_dx = (y2 - y0);
  int64_t Dp20_dy = (x0 - x2);
  Dp20_row = ((int64_t)(scissor_left)) * Dp20_dx + ((int64_t)(scissor_top)) * Dp20_dy + ((int64_t)x2) * ((int64_t)y0) - ((int64_t)y2) * ((int64_t)x0);

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

    int64_t z_sx;
    if (direction_xy_flips) {
      /* Flip numerator to column (x) direction */
      z_sx = D012 - z_s - 2;
    }
    else {
      /* x and y are both ascending, or both descending, keep as-is */
      z_sx = z_s;
    }
    int64_t z_x = z;

    /* Step z_s to next row */
    z += z_yq;
    z_s -= z_yp;
    int64_t step_mask = z_s >> 63;
    z_s += D012 & step_mask;
    z += z_yi & step_mask;

    for (px = scissor_left; px < scissor_right; ++px) {
      uint8_t *pixel = rgba + py * stride + px * 4;
      if (Dp01 > 0 && Dp12 > 0 && Dp20 > 0) {
        uint32_t clr = hsvtorgb((uint32_t)z_x, 255, 255);
        pixel[0] = (uint8_t)(clr >> 16);
        pixel[1] = (uint8_t)(clr >> 8);
        pixel[2] = (uint8_t)(clr);
        pixel[3] = 0xFF;
      }

      size_t sample_index;
      for (sample_index = 0; sample_index < num_samples; ++sample_index) {
        struct tri7_sample *samp = samples + sample_index;
        if (samp->x_ == px && samp->y_ == py) {
          samp->z_buf_num_recorded_ = z_sx;
          samp->z_buf_value_recorded_ = z_x;
          if ((samp->z_buf_num_ != samp->z_buf_num_recorded_) ||
              (samp->z_buf_value_ != samp->z_buf_value_recorded_)) {
            printf("pix7 sample mismatch at (%d, %d)\n", (int)px, (int)py);
          }
        }
      }

      Dp01 += Dp01_dx;
      Dp12 += Dp12_dx;
      Dp20 += Dp20_dx;

      /* Step z_x to next column */
      z_x += z_xq;
      z_sx -= z_xp;
      step_mask = z_sx >> 63;
      z_sx += D012 & step_mask;
      z_x += z_xi & step_mask;
    }
  }
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
#elif 0
    tri4_no_subpixels(rgba32, 256*4,
                      0, 0, 256, 256, /* scissor rect */
                      last_col, 0,    /* vertex 0 */
                      col, 0,         /* vertex 1 */
                      0, 255);        /* vertex 2 */
#elif 0
    tri5(rgba32, 256*4,
         0, 0, 256, 256, /* scissor rect */
         last_col, 0, 255, /* vertex 0 */
         col, 0, 0,      /* vertex 1 */
         0, 255, 0);     /* vertex 2 */
    
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
#elif 0
  tri5(rgba32, 256*4,
        0, 0, 256, 256, /* scissor rect */
        0, 0, 4 * 255, /* vertex 0 */
        255, 0, 2 * 255,      /* vertex 1 */
        0, 255, 2 * 255);     /* vertex 2 */
#elif 0
  tri5(rgba32, 256*4,
    0, 0, 256, 256, /* scissor rect */
    10, 5, 4 * 255, /* vertex 0 */
    255, 10, 2 * 255,      /* vertex 1 */
    5, 255, 2 * 255);     /* vertex 2 */
#elif 0
  tri5(rgba32, 256*4,
    0, 0, 256, 256, /* scissor rect */
    3, 3, 0 * 255, /* vertex 0 */
    55, 0, 1 * 255,      /* vertex 1 */
    0, 65, 2 * 255);     /* vertex 2 */
#elif 0
  tri6(rgba32, 256*4,
    0, 0, 256, 256, /* scissor rect */
    3, 3, 2 * 255, /* vertex 0 */
    55, 0, 1 * 255,      /* vertex 1 */
    0, 65, 0 * 255);     /* vertex 2 */
#elif 0
  struct tri7_sample tri7_samples[] = {
    {
      103, 3, 
      0xFFFFFFFF,  // z-buf value
      0         // z-buf numerator
    },
    {
      155, 0,
      0xFFFF,  // z-buf value
      0         // z-buf numerator
    },
    {
      100, 65,
      0 * 255,  // z-buf value
      0         // z-buf numerator
    },
  };
  tri7(rgba32, 256*4,
    0, 0, 256, 256, /* scissor rect */
    103, 3, 0xFFFFFFFF, /* vertex 0 */
    155, 0, 0xFFFF,    /* vertex 1 */
    100, 65, 0,         /* vertex 2 */
    sizeof(tri7_samples)/sizeof(*tri7_samples), tri7_samples);     
#elif 1
  struct tri7_sample tri7_samples[] = {
    {
      103, 3, 
      0xFFFFFFFF,  // z-buf value
      0         // z-buf numerator
    },
    {
      (1 << 27) - 1, 0,
      0xFFFF,  // z-buf value
      0         // z-buf numerator
    },
    {
      100, 65,
      0 * 255,  // z-buf value
      0         // z-buf numerator
    },
  };
  tri7(rgba32, 256*4,
    0, 0, 256, 256, /* scissor rect */
    103, 3, 0x0, /* vertex 0 */
    (1 << 30) - 1, (1 << 30) - 1, 0xFFFF,    /* vertex 1 */
    100, (1 << 30) - 1, 0xFFFFFFFF,         /* vertex 2 */
    sizeof(tri7_samples)/sizeof(*tri7_samples), tri7_samples);     

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
