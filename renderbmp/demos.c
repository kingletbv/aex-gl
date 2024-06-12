/* Copyright 2024 Kinglet B.V.
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

#ifdef _WIN32
/* high res timers */
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <inttypes.h>

#include "demos.h"
#include "smiley.h"
#include "opengl_es2_headers.h"

int print_shader_log(FILE *fp, GLuint shader) {
  GLint log_length = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
  char *buf = malloc(log_length + 1);
  if (!buf) {
    fprintf(fp, "Failed to allocate memory for log\n");
    return -1;
  }
  buf[0] = '\0';
  GLsizei log_size = (GLsizei)log_length + 1;
  GLsizei actual_log_length = 0;
  glGetShaderInfoLog(shader, log_size, &actual_log_length, (GLchar *)buf);
  buf[actual_log_length] = '\0';
  fprintf(fp, "%s\n", buf);
  free(buf);

  return 0;
}

int print_program_log(FILE *fp, GLuint program) {
  GLint log_length = 0;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);
  char *buf = malloc(log_length + 1);
  if (!buf) {
    fprintf(fp, "Failed to allocate memory for log\n");
    return -1;
  }
  buf[0] = '\0';
  GLsizei log_size = (GLsizei)log_length + 1;
  GLsizei actual_log_length = 0;
  glGetProgramInfoLog(program, log_size, &actual_log_length, (GLchar *)buf);
  buf[actual_log_length] = '\0';
  fprintf(fp, "%s\n", buf);
  free(buf);

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
  uint8_t *bmp = (uint8_t *)malloc(sizeof(uint8_t) * 4 * width);
  if (!bmp) return;
  row = ((size_t)height) - 1;
  do {
    size_t col;
    uint8_t *p = bmp;
    for (col = 0; col < width; col++) {
      uint8_t *rgba = rgba32 + row * stride + col * 4;
      *p++ = rgba[2]; *p++ = rgba[1]; *p++ = rgba[0]; *p++ = rgba[3];
      //fpack(fp, "bbbb", rgba[2], rgba[1], rgba[0], rgba[3]);
    }
    fwrite(bmp, sizeof(uint8_t), 4 * width, fp);
  } while (row--);
  free(bmp);
}

int check_for_and_print_gl_err(FILE *fp) {
  GLenum errcode = glGetError();
  const char *errmsg = "Unknown error!";
  switch (errcode) {
    case GL_NO_ERROR: return 0; /* had no error */
    case GL_INVALID_ENUM: errmsg = "Invalid enum"; break;
    case GL_INVALID_VALUE: errmsg = "Invalid value"; break;
    case GL_INVALID_OPERATION: errmsg = "Invalid operation"; break;
    case GL_OUT_OF_MEMORY: errmsg = "Out of memory"; break;
    case GL_INVALID_FRAMEBUFFER_OPERATION: errmsg = "Invalid framebuffer operation"; break;
  }
  fprintf(fp, "GL error: %s\n", errmsg);
  return 1; /* had an error */
}

int run_demo(int output_width, int output_height, demo_func_t df, const char *name, const char *output_bmp_file) {
  int rgba32_is_flipped_y = 0;
  int exit_ret = 0;

  glGetError(); /* clear any prior errors */

  uint8_t *rgba32 = (uint8_t *)malloc(output_width * output_height * 4);
  if (!rgba32) {
    fprintf(stderr, "No memory for bitmap\n");
    return -1;
  }

  GLuint fb;
  glGenFramebuffers(1, &fb);
  GLuint color_buffer, depth_buffer;
  glGenRenderbuffers(1, &color_buffer);
  glGenRenderbuffers(1, &depth_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, color_buffer);
#ifndef GL_RGBA8
#define GL_RGBA8    0x8058
#endif
  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, output_width, output_height);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, output_width, output_height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, fb);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_buffer);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);

  glViewport(0, 0, output_width, output_height);

  fprintf(stdout, "%s:\n", name);
#ifdef _WIN32
  LARGE_INTEGER freq, start_count;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&start_count);
#endif
  exit_ret = df(output_width, output_height);
#ifdef _WIN32
  LARGE_INTEGER end_count;
  QueryPerformanceCounter(&end_count);
  uint64_t delta = end_count.QuadPart - start_count.QuadPart;
  delta = delta * 1000000 /* usecs */;
  delta = delta / freq.QuadPart;
  uint64_t fps = freq.QuadPart / (end_count.QuadPart - start_count.QuadPart);

  fprintf(stdout, "usecs: %" PRIu64 " (~ %d fps)\n", delta, (int)fps);
#endif

  glReadPixels(0, 0, output_width, output_height, GL_RGBA, GL_UNSIGNED_BYTE, rgba32);

  /* glReadPixels returns out pixels with the Y upside down (positive Y goes up, memory layout
   * starts at the bottom-most row and works its way up. */
  rgba32_is_flipped_y = 1;

  if (!check_for_and_print_gl_err(stderr)) fprintf(stdout, "No GL errors.\n");

  /* Superimpose faint grid effect */
  size_t grid_stride = output_width * 4;
  uint8_t *grid_rgba = rgba32;
  if (rgba32_is_flipped_y) {
    /* We want to apply the grip from the top down to the bottom,
     * if the top is the last row of the bitmap, we need to account
     * for that, flip our access to the bitmap. */
    grid_rgba = rgba32 + (output_height - 1) * grid_stride;
    grid_stride = (size_t)-(intptr_t)grid_stride;
  }

  size_t row, col;
  for (row = 0; row < output_height; row++) {
    for (col = 0; col < output_width; col++) {
      uint8_t *rgba = grid_rgba + row * grid_stride + col * 4;
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

#ifdef USE_STANDARD_NON_AEX_GL_HEADERS
  FILE *fp = fopen(output_bmp_file, "wb"); // relative to project file.
#else
  FILE *fp = fopen(output_bmp_file, "wb"); // relative to project file.
#endif
  if (!fp) {
    perror("fopen");
    exit_ret = -1;
    return exit_ret;
  }
  if (!rgba32_is_flipped_y) {
    write_rgba_bmp(fp, rgba32, output_width, output_height, output_width * 4);
  }
  else /* (rgba32_is_flipped_y) */ {
    size_t stride = output_width * 4;
    write_rgba_bmp(fp, rgba32 + stride * (output_height - 1), output_width, output_height, (size_t)-(intptr_t)stride);
  }
  fclose(fp);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &fb);
  glDeleteRenderbuffers(1, &color_buffer);
  glDeleteRenderbuffers(1, &depth_buffer);

  free(rgba32);
  return 0;
}

int run_demos(void) {
  int exit_ret = 0;
  int rgba32_is_flipped_y = 0;
  int output_width;
  int output_height;

  glGetError(); /* clear any prior errors */

  output_width = 1920;
  output_height = 1080;

#ifdef USE_STANDARD_NON_AEX_GL_HEADERS
  /* Path for non-AEX-gl environment */
#define JIG_PATH "jig/"
#else
#define JIG_PATH "../jig/"
#endif

  exit_ret = run_demo(output_width, output_height, demo_sixteen_lines, "Sixteen lines", JIG_PATH "sixteen_lines.bmp");
  exit_ret = run_demo(output_width, output_height, demo_mipmap_triangle, "Mipmapped triangle", JIG_PATH "mipmap_triangle.bmp");
  exit_ret = run_demo(output_width, output_height, demo_the_world_is_yours, "The-World-Is-Yours line drawing", JIG_PATH "world_is_yours.bmp");
  exit_ret = run_demo(output_width, output_height, demo_z_triangles, "Z triangles", JIG_PATH "z_triangles.bmp");

  if (exit_ret) return exit_ret;

  return exit_ret;
}
