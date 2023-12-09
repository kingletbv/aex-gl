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


int main(int argc, char **argv) {
  int r;
  r = test();
  if (r) {
    return EXIT_FAILURE;
  }
  FILE *fp = fopen("..\\jig\\test.bmp", "wb"); // relative to project file.
  if (!fp) {
    perror("fopen");
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
    tri2(rgba32, 256*4,
        0, 0, 256, 256, /* scissor rect */
        last_col, 0,    /* vertex 0 */
        col, 0,         /* vertex 1 */
        0, 255);        /* vertex 2 */
    last_col = col;
  }

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

  write_rgba_bmp(fp, rgba32, 256, 256, 256 * 4);
  fclose(fp);

  return EXIT_SUCCESS;
}
