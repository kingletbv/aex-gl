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

#ifndef STDARG_H_INCLUDED
#define STDARG_H_INCLUDED
#include <stdarg.h>
#endif

#ifndef ERRNO_H_INCLUDED
#define ERRNO_H_INCLUDED
#include <errno.h>
#endif


#ifndef DEBUG_DUMP_H_INCLUDED
#define DEBUG_DUMP_H_INCLUDED
#include "debug_dump.h"
#endif

#ifndef SAMPLER_2D_H_INCLUDED
#define SAMPLER_2D_H_INCLUDED
#include "sampler_2d.h"
#endif

static size_t fpack(FILE *fp, const char *format, ...) {
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

void dd_write_rgba_bmp(FILE *fp, uint8_t *rgba32, int width, int height, size_t stride) {
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
  uint8_t *bmp = (uint8_t *)malloc(sizeof(uint8_t) * 4 * width);
  if (!bmp) return;
  row = ((size_t)height) - 1;
  do {
    size_t col;
    uint8_t *p = bmp;
    for (col = 0; col < width; col++) {
      uint8_t *rgba = rgba32 + row * stride + col * 4;
      *p++ = rgba[2]; *p++ = rgba[1]; *p++ = rgba[0]; *p++ = rgba[3];
    }
    fwrite(bmp, sizeof(uint8_t), 4 * width, fp);
  } while (row--);
  free(bmp);
}


void dd_write_rgb_bmp(FILE *fp, uint8_t *rgba32, int width, int height, size_t stride) {
  /* Write the BMP file header */

  /* signature */
  fwrite("BM", 2, 1, fp);

  /* file size */
  fpack(fp, "d", 14 + 40 + (3 * width * height));
  fpack(fp, "ww", 0, 0);  /* reserved fields */
  fpack(fp, "d", 14 + 40); /* offset to pixel data */

  /* Write the bitmap info header */
  fpack(fp, "d", 40); /* size of bitmap info header (ergo its version) */
  fpack(fp, "dd", width, height);
  fpack(fp, "ww", 1, 24); /* 1 plane, 24 bits per pixel */
  fpack(fp, "d", 0); /* BI_RGB: no compression */
  fpack(fp, "d", 0); /* size of image data, can be zero if no compression */
  fpack(fp, "dddd",
        0,  /* horizontal pixels per meter */
        0,  /* vertical pixels per meter*/
        0,  /* number of colors in palette, none for BI_RGB */
        0); /* number of "important" colors, all of em, not palette based. */

/* Write the pixel data; start with bottom row and work our way up. */
  size_t row;
  uint8_t *bmp = (uint8_t *)malloc(sizeof(uint8_t) * 3 * width);
  if (!bmp) return;
  row = ((size_t)height) - 1;
  do {
    size_t col;
    uint8_t *p = bmp;
    for (col = 0; col < width; col++) {
      uint8_t *rgba = rgba32 + row * stride + col * 3;
      *p++ = rgba[2]; *p++ = rgba[1]; *p++ = rgba[0];
    }
    fwrite(bmp, sizeof(uint8_t), 3 * width, fp);
  } while (row--);
  free(bmp);
}



int dd_write_bmp(const char *filename, struct sampler_2d *texture) {
  FILE *fp = fopen(filename, "wb");
  if (!fp) {
    return -1;
  }

  switch (texture->mipmaps_[0].components_) {
    case s2d_rgb:
      dd_write_rgb_bmp(fp, texture->mipmaps_[0].bitmap_, texture->mipmaps_[0].width_, texture->mipmaps_[0].height_, 
                       texture->mipmaps_[0].num_bytes_per_bitmap_row_);
      break;

    case s2d_rgba:
      dd_write_rgba_bmp(fp, texture->mipmaps_[0].bitmap_, texture->mipmaps_[0].width_, texture->mipmaps_[0].height_, 
                        texture->mipmaps_[0].num_bytes_per_bitmap_row_);
      break;
  }

  fclose(fp);

  return 0;
}

