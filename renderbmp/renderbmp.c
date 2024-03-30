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

#ifndef INTTYPES_H_INCLUDED
#define INTTYPES_H_INCLUDED
#include <inttypes.h>
#endif

#ifndef GLSL_ES1_TOKENS_H_INCLUDED
#define GLSL_ES1_TOKENS_H_INCLUDED
#include "glsl_es1_tokens.h"
#endif

#ifndef GLSL_ES1_COMPILER_H_INCLUDED
#define GLSL_ES1_COMPILER_H_INCLUDED
#include "glsl_es1_compiler.h"
#endif

#ifndef PRIMITIVE_ASSEMBLY_H_INCLUDED
#define PRIMITIVE_ASSEMBLY_H_INCLUDED
#include "primitive_assembly.h"
#endif

#ifndef ATTRIB_SET_H_INCLUDED
#define ATTRIB_SET_H_INCLUDED
#include "attrib_set.h"
#endif

#ifndef CLIPPING_STAGE_H_INCLUDED
#define CLIPPING_STAGE_H_INCLUDED
#include "clipping_stage.h"
#endif

#ifndef VIEWPORT_TRANSFORMATION_H_INCLUDED
#define VIEWPORT_TRANSFORMATION_H_INCLUDED
#include "viewport_transformation.h"
#endif

#ifndef RASTERIZER_H_INCLUDED
#define RASTERIZER_H_INCLUDED
#include "rasterizer.h"
#endif

#ifndef FRAGMENT_BUFFER_H_INCLUDED
#define FRAGMENT_BUFFER_H_INCLUDED
#include "fragment_buffer.h"
#endif

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
#endif

#ifndef SL_COMPILATION_UNIT_H_INCLUDED
#define SL_COMPILATION_UNIT_H_INCLUDED
#include "sl_compilation_unit.h"
#endif

#ifndef REF_RANGE_ALLOCATOR_H_INCLUDED
#define REF_RANGE_ALLOCATOR_H_INCLUDED
#include "ref_range_allocator.h"
#endif

int test(void) {
  int r = 0;

  r = ref_range_test();
  if (r) {
    fprintf(stderr, "ref_range_test failed (%d)\n", r);
    return r;
  }

  struct glsl_es1_compiler compiler, *cc;
  cc = &compiler;
  glsl_es1_compiler_init(cc);

  if (!glsl_es1_tokens_sanity_check()) {
    fprintf(stderr, "Failed sanity check\n");
    return -1;
  }

  enum glsl_es1_compiler_result ccr;
  const char *input_frag =
    "struct blah {\n"
    "  vec4 x;\n"
    "  vec4 y;\n"
    "};\n"
    "const vec2 a;\n"
    "vec4 b = vec4(1.0, 2.0, 3.0, 4.0);\n"
    "vec4 c = vec4(a, 1.0, 2.0);\n"
    "int d = 123;\n"
    "int x = 0;\n"
    "blah e = blah(\n"
    "  vec4(1., 3., 5., 7.),\n"
    "  vec4(2., 4., 8., 10.)\n"
    ");\n"
    "int ask_brubeck() {\n"
    "  return 5;\n"
    "}\n"
    "void main() {\n"
    //"  c.x = 1.f;\n"
    //"  c.xy++;\n"
    //"  c.yz++;\n"
    //"  c.zw++;\n"
    //"  c.w++;\n"
    "  d = 0;\n"
    "  int n;\n"
    "  n = 10;\n"
    "  x = 0;\n"
    "  do {\n"
    "    if (n <= 5) {\n"
    "      x += ask_brubeck();\n"
    "      continue;\n"
    "    }\n"
    "    d++;\n"
    "  } while (--n > 0);\n"
    "  e.x.x++;\n"
    "}\n"
    "\n";
  ccr = glsl_es1_compiler_compile_mem(cc, "input.frag", input_frag, strlen(input_frag));
    
  if (ccr != GLSL_ES1_R_SUCCESS) {
    fprintf(stderr, "Failed compilation (%d)\n", ccr);
    glsl_es1_compiler_cleanup(cc);
    return -1;
  }

  struct sl_execution exec;
  sl_exec_init(&exec);

  r = sl_exec_prep(&exec, cc->cu_);

  if (r) {
    fprintf(stderr, "failed to prepare execution (%d)\n", r);
    sl_exec_cleanup(&exec);
    glsl_es1_compiler_cleanup(cc);
    return -1;
  }

  /* Prepare one row of registers */
  float *float_bank = NULL;
  int64_t *int_bank = NULL;
  uint8_t *bool_bank = NULL;
  void **sampler2D_bank = NULL;
  void **samplerCube_bank = NULL;
  uint8_t *chain_bank = NULL;

  size_t num_rows = 1;
  if (exec.num_float_regs_) float_bank = (float *)malloc(sizeof(float) * num_rows * exec.num_float_regs_);
  if (exec.num_int_regs_) int_bank = (int64_t *)malloc(sizeof(int64_t) * num_rows * exec.num_int_regs_);
  if (exec.num_bool_regs_) bool_bank = (uint8_t *)malloc(sizeof(uint8_t) * num_rows * exec.num_bool_regs_);
  if (exec.num_sampler_2D_regs_) sampler2D_bank = (void **)malloc(sizeof(void *) * num_rows * exec.num_sampler_2D_regs_);
  if (exec.num_sampler_cube_regs_) samplerCube_bank = (void **)malloc(sizeof(void *) * num_rows * exec.num_sampler_cube_regs_);
  chain_bank = (uint8_t *)malloc(sizeof(uint8_t) * num_rows * 1);
  if ((exec.num_float_regs_ && !float_bank) || 
      (exec.num_int_regs_ && !int_bank) || 
      (exec.num_bool_regs_ && !bool_bank) || 
      !chain_bank) {
    fprintf(stderr, "failed to allocate register banks\n");
    if (exec.num_float_regs_ && float_bank) free(float_bank);
    if (exec.num_int_regs_ && int_bank) free(int_bank);
    if (exec.num_bool_regs_ && bool_bank) free(bool_bank);
    if (exec.num_sampler_2D_regs_ && sampler2D_bank) free(sampler2D_bank);
    if (exec.num_sampler_cube_regs_ && samplerCube_bank) free(samplerCube_bank);
    if (chain_bank) free(chain_bank);
    sl_exec_cleanup(&exec);
    glsl_es1_compiler_cleanup(cc);
    return -1;
  }

  if (exec.float_regs_) exec.float_regs_[0] = float_bank;
  if (exec.int_regs_) exec.int_regs_[0] = int_bank;
  if (exec.bool_regs_) exec.bool_regs_[0] = bool_bank;
  if (exec.sampler_2D_regs_) exec.sampler_2D_regs_[0] = sampler2D_bank;
  if (exec.sampler_cube_regs_) exec.sampler_cube_regs_[0] = samplerCube_bank;
  exec.exec_chain_reg_ = chain_bank;
  size_t n;

  for (n = 0; n < (num_rows - 1); ++n) {
    exec.exec_chain_reg_[n] = 1;
  }
  exec.exec_chain_reg_[num_rows - 1] = 0;

  for (n = 1; n < exec.num_float_regs_; n++) {
    exec.float_regs_[n] = exec.float_regs_[n - 1] + num_rows;
  }
  for (n = 1; n < exec.num_int_regs_; n++) {
    exec.int_regs_[n] = exec.int_regs_[n - 1] + num_rows;
  }
  for (n = 1; n < exec.num_bool_regs_; n++) {
    exec.bool_regs_[n] = exec.bool_regs_[n - 1] + num_rows;
  }
  for (n = 1; n < exec.num_sampler_2D_regs_; n++) {
    exec.sampler_2D_regs_[n] = exec.sampler_2D_regs_[n - 1] + num_rows;
  }
  for (n = 1; n < exec.num_sampler_cube_regs_; n++) {
    exec.sampler_cube_regs_[n] = exec.sampler_cube_regs_[n - 1] + num_rows;
  }

  struct sl_function *f = sl_compilation_unit_find_function(cc->cu_, "main");
  if (!f) {
    fprintf(stderr, "main() function not found\n");
    glsl_es1_compiler_cleanup(cc);
    return -1;
  }

  struct sl_variable *v_c = sl_compilation_unit_find_variable(cc->cu_, "c");
  if (!v_c) {
    fprintf(stderr, "variable \"c\" not found\n");
    glsl_es1_compiler_cleanup(cc);
    return -1;
  }

  struct sl_variable *v_d = sl_compilation_unit_find_variable(cc->cu_, "d");
  if (!v_d) {
    fprintf(stderr, "variable \"d\" not found\n");
    glsl_es1_compiler_cleanup(cc);
    return -1;
  }

  struct sl_variable *v_x = sl_compilation_unit_find_variable(cc->cu_, "x");
  if (!v_x) {
    fprintf(stderr, "variable \"d\" not found\n");
    glsl_es1_compiler_cleanup(cc);
    return -1;
  }

  r = sl_exec_run(&exec, f, 0);
  if (r) {
    fprintf(stderr, "failed to run execution (%d)\n", r);
  }
  else {
    if (v_c->reg_alloc_.kind_ == slrak_vec4) {
      fprintf(stdout, "c = ");
      sl_exec_dumpf(&exec, stdout, 0, &v_c->reg_alloc_);
      fprintf(stdout, "\n");
#if 0
      fprintf(stdout, "c = vec4(%f, %f, %f, %f)\n", 
              (exec.float_regs_[v_c->reg_alloc_.v_.regs_[0]])[0],
              (exec.float_regs_[v_c->reg_alloc_.v_.regs_[1]])[0],
              (exec.float_regs_[v_c->reg_alloc_.v_.regs_[2]])[0],
              (exec.float_regs_[v_c->reg_alloc_.v_.regs_[3]])[0]);
#endif
    }
    else {
      fprintf(stderr, "expected \"c\" to be slrak_vec4 (%d)\n", r);
    }
    if (v_d->reg_alloc_.kind_ == slrak_int) {
      fprintf(stdout, "d = ");
      sl_exec_dumpf(&exec, stdout, 0, &v_d->reg_alloc_);
      fprintf(stdout, "\n");
#if 0
      fprintf(stdout, "d = %" PRId64 "\n", (exec.int_regs_[v_d->reg_alloc_.v_.regs_[0]])[0]);
#endif
    }
    else {
      fprintf(stderr, "expected \"d\" to be slrak_int (%d)\n", r);
    }
    if (v_x->reg_alloc_.kind_ == slrak_int) {
      fprintf(stdout, "x = ");
      sl_exec_dumpf(&exec, stdout, 0, &v_x->reg_alloc_);
      fprintf(stdout, "\n");
#if 0
      fprintf(stdout, "x = %" PRId64 "\n", (exec.int_regs_[v_x->reg_alloc_.v_.regs_[0]])[0]);
#endif
    }
    else {
      fprintf(stderr, "expected \"x\" to be slrak_int (%d)\n", r);
    }
  }

  if (exec.num_float_regs_ && float_bank) free(float_bank);
  if (exec.num_int_regs_ && int_bank) free(int_bank);
  if (exec.num_bool_regs_ && bool_bank) free(bool_bank);
  if (exec.num_sampler_2D_regs_ && sampler2D_bank) free(sampler2D_bank);
  if (exec.num_sampler_cube_regs_ && samplerCube_bank) free(samplerCube_bank);

  sl_exec_cleanup(&exec);
  glsl_es1_compiler_cleanup(cc);

  return r;
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
      z_sx = D012 - z_s - 1;
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
      uint8_t *pixel = rgba + (py - scissor_top) * stride + (px - scissor_left) * 4;
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
          if (samp->z_buf_value_ != samp->z_buf_value_recorded_) {
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

struct tri8_sample {
  // at point (x,y)
  int32_t x_;
  int32_t y_;
  // the z-buf value should be this
  uint64_t z_buf_value_;

  // area to record the observed values.
  uint64_t z_buf_value_recorded_;
  uint64_t z_buf_num_recorded_;
};

void tri8(uint8_t *rgba, size_t stride,
          uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
          int32_t x0, int32_t y0, uint32_t z0,
          int32_t x1, int32_t y1, uint32_t z1,
          int32_t x2, int32_t y2, uint32_t z2,
          size_t num_samples, struct tri8_sample *samples) {
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

  /* Ensure the Z-Buffer value rounds to nearest, rather than truncating. To do this, we would like to add half
   * denominator to the numerator (which has the effect of rounding to nearest.) A problem with this is we don't
   * know if the denominator (D012 in this case) is an odd number, if it is, then halving it would create a
   * round-off error as we dispose of the least significant bit.
   * To solve this, we double everything, D012, all the numerators, and all the numerator increments. Then, we
   * add "half double D012" (i.e. just the original D012) to the numerator and we're done.
   * D012, before doubling, uses 2n+3 = 2*30+3 = 63 bits, so we have space for the additional bit. */
  z_s += z_s + D012; /* double, and add D012 */
  z_yp += z_yp; /* numerator step-size for Y, double it */
  z_xp += z_xp; /* numerator step-size for X, double it */
  D012 += D012; /* and finally, our denominator, double it. */

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
      z_sx = D012 - z_s - 1;
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
      uint8_t *pixel = rgba + (py - scissor_top) * stride + (px - scissor_left) * 4;
      if (Dp01 > 0 && Dp12 > 0 && Dp20 > 0) {
        pixel[0] = (uint8_t)((z_x & 1) ? 0xCF : 0x3F);
        pixel[1] = (uint8_t)((z_x & 1) ? 0xCF : 0x3F);
        pixel[2] = (uint8_t)((z_x & 1) ? 0xCF : 0x3F);
        pixel[3] = 0xFF;
      }

      size_t sample_index;
      for (sample_index = 0; sample_index < num_samples; ++sample_index) {
        struct tri8_sample *samp = samples + sample_index;
        if (samp->x_ == px && samp->y_ == py) {
          samp->z_buf_num_recorded_ = z_sx;
          samp->z_buf_value_recorded_ = z_x;
          if (samp->z_buf_value_ != samp->z_buf_value_recorded_) {
            printf("pix8 sample mismatch at (%d, %d)\n", (int)px, (int)py);
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

struct tri9_sample {
  // at point (x,y)
  int32_t x_;
  int32_t y_;
  // the z-buf value should be this
  uint64_t z_buf_value_;

  // area to record the observed values.
  uint64_t z_buf_value_recorded_;
  uint64_t z_buf_num_recorded_;
};

void tri9(uint8_t *rgba, size_t stride,
          uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
          int32_t x0, int32_t y0, uint32_t z0,
          int32_t x1, int32_t y1, uint32_t z1,
          int32_t x2, int32_t y2, uint32_t z2,
          size_t num_samples, struct tri9_sample *samples) {
  int64_t D012;

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

  // Location where we go "Pen down" - convenience short-hand.
  int64_t Px = (int64_t)left_sp;
  int64_t Py = (int64_t)top_sp;

  // D012 = determinant of x and y coordinates (this is twice the area of the triangle (e.g. the area of the paralellogram))
  //        | x0 y0 1 |
  // D012 = | x1 y1 1 |
  //        | x2 y2 1 |
  D012 = ((int64_t)x1) * ((int64_t)y2) - ((int64_t)x2) * ((int64_t)y1) - ((int64_t)x0) * ((int64_t)y2) + ((int64_t)x2) * ((int64_t)y0) + ((int64_t)x0) * ((int64_t)y1) - ((int64_t)x1) * ((int64_t)y0);

  if (D012 <= 0) {
    // Counterclockwise backface, or colinear. Reject.
    return;
  }

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

  // z_num = Dzx * Px + Dzy * Py + Dxyz
  // we have the first two terms as Dzx_Px_hi/lo and Dzy_Py_hi/lo, and have Dzxy_hi/lo, but need to add them all together.
  int64_t z_num_hi, z_num_lo;
  adds128(Dzx_Px_hi, Dzx_Px_lo, Dzy_Py_hi, Dzy_Py_lo, &z_num_hi, &z_num_lo);
  adds128(z_num_hi, z_num_lo, Dxyz_hi, Dxyz_lo, &z_num_hi, &z_num_lo);

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

  int64_t z_TL = z_lo;
  int64_t z_TR;
  int64_t z_BL;
  int64_t z_BR;

  // stepper variable; outer loop is rows so we start with initialization for Y.
  int64_t z_s_TL;
  int64_t z_s_TR;
  int64_t z_s_BL;
  int64_t z_s_BR;

  int64_t Dzy_hi, Dzy_lo;
  muls128(Dzy_hi_sp, Dzy_lo_sp, 0, 1 << SUBPIXEL_BITS, &Dzy_hi, &Dzy_lo);

  int64_t z_yi;
  int64_t z_yp;
  int64_t z_yq;
  int direction_xy_flips;
  divmods128by64(Dzy_hi, Dzy_lo, D012, NULL, &z_yq, &z_yp);
  if ((Dzy_hi > 0) || ((Dzy_hi == 0) && (Dzy_lo > 0))) {
    z_s_TL = D012 - z_mod - 1;
    z_yi = 1;
    direction_xy_flips = 1;
  }
  else if (Dzy_hi < 0) {
    z_s_TL = z_mod;
    z_yi = -1;
    z_yp = D012 - z_yp - 1;
    direction_xy_flips = 0;
  }
  else /* (Dzy_hi == 0 && Dzy_lo == 0) */ {
    z_s_TL = z_mod;
    z_yi = 0;
    direction_xy_flips = 0;
    z_yp = 0;
  }

  int64_t Dzx_hi, Dzx_lo;
  muls128(Dzx_hi_sp, Dzx_lo_sp, 0, 1 << SUBPIXEL_BITS, &Dzx_hi, &Dzx_lo);

  // XXX:
  // Dzy_hi/lo and Dzx_hi/lo step one whole pixel, consequently, z_yq, z_yp, z_xq, z_xp,
  // all step one pixel. We need to step 2 pixels at a time.
  // Use the one-pixel steppers to define TL/TR/BL/BR, and then double the one pixel steppers
  // (overflowing z_xp*2/D012 into z_xq and overflowing z_yp*2/D012 into z_yq) so we can step
  // inside the core loop.

  int64_t z_xi;
  int64_t z_xp;
  int64_t z_xq;
  divmods128by64(Dzx_hi, Dzx_lo, D012, NULL, &z_xq, &z_xp);
  if ((Dzx_hi > 0) || ((Dzx_hi == 0) && (Dzx_lo > 0))) {
    z_xi = 1;
    direction_xy_flips ^= 1;
  }
  else if (Dzx_hi < 0) {
    z_xi = -1;
    z_xp = D012 - z_xp - 1;
  }
  else /* (Dzx == 0) */ {
    z_xi = 0;
    z_xp = 0;
  }

  /* Ensure the Z-Buffer value rounds to nearest, rather than truncating. To do this, we would like to add half
   * denominator to the numerator (which has the effect of rounding to nearest.) A problem with this is we don't
   * know if the denominator (D012 in this case) is an odd number, if it is, then halving it would create a
   * round-off error as we dispose of the least significant bit.
   * To solve this, we double everything, D012, all the numerators, and all the numerator increments. Then, we
   * add "half double D012" (i.e. just the original D012) to the numerator and we're done.
   * D012, before doubling, uses 2n+3 = 2*30+3 = 63 bits, so we have space for the additional bit. */
  /* double, and add D012 */
  z_s_TL += z_s_TL + D012;
  z_yp += z_yp; /* numerator step-size for Y, double it */
  z_xp += z_xp; /* numerator step-size for X, double it */
  D012 += D012; /* and finally, our denominator, double it. */

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
  Dp01_row = left_sp * Dp01_dx_sp + top_sp * Dp01_dy_sp + ((int64_t)x0) * ((int64_t)y1) - ((int64_t)y0) * ((int64_t)x1);


  // Dp12 = determinant for triangle formed by edge 12 and point p:
  //        | px py 1 |
  // Dp12 = | x1 y1 1 |
  //        | x2 y2 1 |
  int64_t Dp12_dx_sp = (y1 - y2);
  int64_t Dp12_dy_sp = (x2 - x1);
  Dp12_row = left_sp * Dp12_dx_sp + top_sp * Dp12_dy_sp + ((int64_t)x1) * ((int64_t)y2) - ((int64_t)y1) * ((int64_t)x2);

  // Dp20 = determinant for triangle formed by edge 20 and point p:
  //        | px py 1 |
  // Dp20 = | x2 y2 1 |
  //        | x0 y0 1 |
  int64_t Dp20_dx_sp = (y2 - y0);
  int64_t Dp20_dy_sp = (x0 - x2);
  Dp20_row = ((int64_t)(scissor_left)) * Dp20_dx_sp + ((int64_t)(scissor_top)) * Dp20_dy_sp + ((int64_t)x2) * ((int64_t)y0) - ((int64_t)y2) * ((int64_t)x0);

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

  uint8_t *pixel_TL = rgba;
  uint8_t *pixel_TR = pixel_TL + 4;
  uint8_t *pixel_BL = pixel_TL + stride;
  uint8_t *pixel_BR = pixel_BL + 4;
  int64_t pixel_mod = 2 * stride - (right - left) * 4; // bltdmod ;-) Keep in mind we go forward not 1, but 2 rows.

  int64_t px, py;
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

    int64_t z_sx_TL;
    int64_t z_sx_TR;
    int64_t z_sx_BL;
    int64_t z_sx_BR;
    if (direction_xy_flips) {
      /* Flip numerator to column (x) direction */
      z_sx_TL = D012 - z_s_TL - 1;
      z_sx_TR = D012 - z_s_TR - 1;
      z_sx_BL = D012 - z_s_BL - 1;
      z_sx_BR = D012 - z_s_BR - 1;
    }
    else {
      /* x and y are both ascending, or both descending, keep as-is */
      z_sx_TL = z_s_TL;
      z_sx_TR = z_s_TR;
      z_sx_BL = z_s_BL;
      z_sx_BR = z_s_BR;
    }

    int64_t z_x_TL = z_TL;
    int64_t z_x_TR = z_TR;
    int64_t z_x_BL = z_BL;
    int64_t z_x_BR = z_BR;

    /* Step z_s to next row */
    z_TL += z_yq;
    z_s_TL -= z_yp;
    step_mask = z_s_TL >> 63;
    z_s_TL += D012 & step_mask;
    z_TL += z_yi & step_mask;

    z_TR += z_yq;
    z_s_TR -= z_yp;
    step_mask = z_s_TR >> 63;
    z_s_TR += D012 & step_mask;
    z_TR += z_yi & step_mask;

    z_BL += z_yq;
    z_s_BL -= z_yp;
    step_mask = z_s_BL >> 63;
    z_s_BL += D012 & step_mask;
    z_BL += z_yi & step_mask;

    z_BR += z_yq;
    z_s_BR -= z_yp;
    step_mask = z_s_BR >> 63;
    z_s_BR += D012 & step_mask;
    z_BR += z_yi & step_mask;


    for (px = left; px < right; px += 2) {
      // Compute the masks for each determinant at each of the four pixels
      // The idea is that if the determinant is positive, then the pixel is
      // inside the triangle; consequently, if the MSB sign bit is zero, then
      // the mask should be all 1's.
      // Because the test is >= 0, we can OR together the sign bits and create
      // a unified mask (if any sign bit is set, then the mask is all zeroes,
      // otherwise it is all ones.)
      int64_t TL_Mask = ~((Dp01_TL | Dp12_TL | Dp20_TL) >> 63);
      int64_t TR_Mask = ~((Dp01_TR | Dp12_TR | Dp20_TR) >> 63);
      int64_t BL_Mask = ~((Dp01_BL | Dp12_BL | Dp20_BL) >> 63);
      int64_t BR_Mask = ~((Dp01_BR | Dp12_BR | Dp20_BR) >> 63);

      int64_t Any_Fragment_Valid = TL_Mask | TR_Mask | BL_Mask | BR_Mask;

      if (Any_Fragment_Valid) {
        // Emit 4 fragments.
        if (TL_Mask) {
          pixel_TL[0] = (uint8_t)((z_x_TL & 1) ? 0xCF : 0x3F);
          pixel_TL[1] = (uint8_t)((z_x_TL & 1) ? 0xCF : 0x3F);
          pixel_TL[2] = (uint8_t)((z_x_TL & 1) ? 0xCF : 0x3F);
          pixel_TL[3] = 0xFF;
        }
        if (TR_Mask) {
          pixel_TR[0] = (uint8_t)((z_x_TR & 1) ? 0xCF : 0x3F);
          pixel_TR[1] = (uint8_t)((z_x_TR & 1) ? 0xCF : 0x3F);
          pixel_TR[2] = (uint8_t)((z_x_TR & 1) ? 0xCF : 0x3F);
          pixel_TR[3] = 0xFF;
        }
        if (BL_Mask) {
          pixel_BL[0] = (uint8_t)((z_x_BL & 1) ? 0xCF : 0x3F);
          pixel_BL[1] = (uint8_t)((z_x_BL & 1) ? 0xCF : 0x3F);
          pixel_BL[2] = (uint8_t)((z_x_BL & 1) ? 0xCF : 0x3F);
          pixel_BL[3] = 0xFF;
        }
        if (BR_Mask) {
          pixel_BR[0] = (uint8_t)((z_x_BR & 1) ? 0xCF : 0x3F);
          pixel_BR[1] = (uint8_t)((z_x_BR & 1) ? 0xCF : 0x3F);
          pixel_BR[2] = (uint8_t)((z_x_BR & 1) ? 0xCF : 0x3F);
          pixel_BR[3] = 0xFF;
        }
      }

      {
        struct {
          int64_t x_, y_;
          uint64_t z_buf_num_;
          uint64_t z_buf_value_;
        } quadruple_samples[4] = {
          { px,     py,     z_sx_TL, z_x_TL },
          { px + 1, py,     z_sx_TR, z_x_TR },
          { px,     py + 1, z_sx_BL, z_x_BL },
          { px + 1, py + 1, z_sx_BR, z_x_BR }
        };
        size_t sample_index;
        for (sample_index = 0; sample_index < num_samples; ++sample_index) {
          struct tri9_sample *samp = samples + sample_index;
          if (((samp->x_ & ~1) == (px & ~1)) &&
              ((samp->y_ & ~1) == (py & ~1))) {
            samp->z_buf_num_recorded_ = quadruple_samples[(samp->x_ & 1) + (samp->y_ & 1) * 2].z_buf_num_;
            samp->z_buf_value_recorded_ = quadruple_samples[(samp->x_ & 1) + (samp->y_ & 1) * 2].z_buf_value_;
            if (samp->z_buf_value_ != samp->z_buf_value_recorded_) {
              printf("pix9 sample mismatch at (%d, %d)\n", (int)px, (int)py);
            }
          }
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

      /* Step z_x to next column */
      z_x_TL += z_xq;
      z_sx_TL -= z_xp;
      step_mask = z_sx_TL >> 63;
      z_sx_TL += D012 & step_mask;
      z_x_TL += z_xi & step_mask;

      z_x_TR += z_xq;
      z_sx_TR -= z_xp;
      step_mask = z_sx_TR >> 63;
      z_sx_TR += D012 & step_mask;
      z_x_TR += z_xi & step_mask;

      z_x_BL += z_xq;
      z_sx_BL -= z_xp;
      step_mask = z_sx_BL >> 63;
      z_sx_BL += D012 & step_mask;
      z_x_BL += z_xi & step_mask;

      z_x_BR += z_xq;
      z_sx_BR -= z_xp;
      step_mask = z_sx_BR >> 63;
      z_sx_BR += D012 & step_mask;
      z_x_BR += z_xi & step_mask;

      /* Step pixels to the next (stepping over 2 pixels at a time) */
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

void tri9_no_subpixels(uint8_t *rgba, size_t stride,
                       uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
                       int32_t x0, int32_t y0, uint32_t z0,
                       int32_t x1, int32_t y1, uint32_t z1,
                       int32_t x2, int32_t y2, uint32_t z2,
                       size_t num_samples, struct tri9_sample *samples) {
  tri9(rgba, stride, scissor_left, scissor_top, scissor_right, scissor_bottom,
       x0 << SUBPIXEL_BITS, y0 << SUBPIXEL_BITS, z0,
       x1 << SUBPIXEL_BITS, y1 << SUBPIXEL_BITS, z1,
       x2 << SUBPIXEL_BITS, y2 << SUBPIXEL_BITS, z2,
       num_samples, samples);
}


struct tri10_sample {
  // at point (x,y)
  int32_t x_;
  int32_t y_;
  // the z-buf value should be this
  uint64_t z_buf_value_;

  // area to record the observed values.
  uint64_t z_buf_value_recorded_;
  uint64_t z_buf_num_recorded_;
};

void tri10(uint8_t *rgba, size_t stride,
           uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
           int32_t x0, int32_t y0, uint32_t z0,
           int32_t x1, int32_t y1, uint32_t z1,
           int32_t x2, int32_t y2, uint32_t z2,
           size_t num_samples, struct tri10_sample *samples) {
  int64_t D012;

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

  // Location where we go "Pen down" - convenience short-hand.
  // (1 << (SUBPIXEL_BITS-1)) gets us to put the pen at the center of the pixel,
  // rather than the top-left corner of it.
  int64_t Px = (1 << (SUBPIXEL_BITS-1)) + left_sp;
  int64_t Py = (1 << (SUBPIXEL_BITS-1)) + top_sp;

  // D012 = determinant of x and y coordinates (this is twice the area of the triangle (e.g. the area of the paralellogram))
  //        | x0 y0 1 |
  // D012 = | x1 y1 1 |
  //        | x2 y2 1 |
  D012 = ((int64_t)x1) * ((int64_t)y2) - ((int64_t)x2) * ((int64_t)y1) - ((int64_t)x0) * ((int64_t)y2) + ((int64_t)x2) * ((int64_t)y0) + ((int64_t)x0) * ((int64_t)y1) - ((int64_t)x1) * ((int64_t)y0);

  if (D012 <= 0) {
    // Counterclockwise backface, or colinear. Reject.
    return;
  }

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

  int64_t z_TL = z_lo;
  int64_t z_TR;
  int64_t z_BL;
  int64_t z_BR;

  // stepper variable; outer loop is rows so we start with initialization for Y.
  int64_t z_s_TL;
  int64_t z_s_TR;
  int64_t z_s_BL;
  int64_t z_s_BR;

  int64_t Dzy_hi, Dzy_lo;
  muls128(Dzy_hi_sp, Dzy_lo_sp, 0, 1 << SUBPIXEL_BITS, &Dzy_hi, &Dzy_lo);

  int64_t z_yi;
  int64_t z_yp;
  int64_t z_yq;
  int direction_xy_flips;
  divmods128by64(Dzy_hi, Dzy_lo, D012, NULL, &z_yq, &z_yp);
  if ((Dzy_hi > 0) || ((Dzy_hi == 0) && (Dzy_lo > 0))) {
    z_s_TL = D012 - z_mod - 1;
    z_yi = 1;
    direction_xy_flips = 1;
  }
  else if (Dzy_hi < 0) {
    z_s_TL = z_mod;
    z_yi = -1;
    z_yp = D012 - z_yp - 1;
    direction_xy_flips = 0;
  }
  else /* (Dzy_hi == 0 && Dzy_lo == 0) */ {
    z_s_TL = z_mod;
    z_yi = 0;
    direction_xy_flips = 0;
    z_yp = 0;
  }

  int64_t Dzx_hi, Dzx_lo;
  muls128(Dzx_hi_sp, Dzx_lo_sp, 0, 1 << SUBPIXEL_BITS, &Dzx_hi, &Dzx_lo);

  int64_t z_xi;
  int64_t z_xp;
  int64_t z_xq;
  divmods128by64(Dzx_hi, Dzx_lo, D012, NULL, &z_xq, &z_xp);
  if ((Dzx_hi > 0) || ((Dzx_hi == 0) && (Dzx_lo > 0))) {
    z_xi = 1;
    direction_xy_flips ^= 1;
  }
  else if (Dzx_hi < 0) {
    z_xi = -1;
    z_xp = D012 - z_xp - 1;
  }
  else /* (Dzx == 0) */ {
    z_xi = 0;
    z_xp = 0;
  }

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

  uint8_t *pixel_TL = rgba + top * stride + left * 4;
  uint8_t *pixel_TR = pixel_TL + 4;
  uint8_t *pixel_BL = pixel_TL + stride;
  uint8_t *pixel_BR = pixel_BL + 4;
  int64_t pixel_mod = 2 * stride - (right - left) * 4; // bltdmod ;-) Keep in mind we go forward not 1, but 2 rows.

  int64_t px, py;
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

    int64_t z_sx_TL;
    int64_t z_sx_TR;
    int64_t z_sx_BL;
    int64_t z_sx_BR;
    if (direction_xy_flips) {
      /* Flip numerator to column (x) direction */
      z_sx_TL = D012 - z_s_TL - 1;
      z_sx_TR = D012 - z_s_TR - 1;
      z_sx_BL = D012 - z_s_BL - 1;
      z_sx_BR = D012 - z_s_BR - 1;
    }
    else {
      /* x and y are both ascending, or both descending, keep as-is */
      z_sx_TL = z_s_TL;
      z_sx_TR = z_s_TR;
      z_sx_BL = z_s_BL;
      z_sx_BR = z_s_BR;
    }

    int64_t z_x_TL = z_TL;
    int64_t z_x_TR = z_TR;
    int64_t z_x_BL = z_BL;
    int64_t z_x_BR = z_BR;

    /* Step z_s to next row */
    z_TL += z_yq;
    z_s_TL -= z_yp;
    step_mask = z_s_TL >> 63;
    z_s_TL += D012 & step_mask;
    z_TL += z_yi & step_mask;

    z_TR += z_yq;
    z_s_TR -= z_yp;
    step_mask = z_s_TR >> 63;
    z_s_TR += D012 & step_mask;
    z_TR += z_yi & step_mask;

    z_BL += z_yq;
    z_s_BL -= z_yp;
    step_mask = z_s_BL >> 63;
    z_s_BL += D012 & step_mask;
    z_BL += z_yi & step_mask;

    z_BR += z_yq;
    z_s_BR -= z_yp;
    step_mask = z_s_BR >> 63;
    z_s_BR += D012 & step_mask;
    z_BR += z_yi & step_mask;

    // (py - scissor_top)
    // negative when top row is above top of scissor window.
    int64_t scissor_top_row_mask = ~((py - scissor_top) >> 63);

    // ((py + 1) - scissor_bottom)
    // positive when bottom row of fragment quadruples are below 
    // bottom of scissor window, negative otherwise
    int64_t scissor_bottom_row_mask = ((py + 1) - scissor_bottom) >> 63;

    // (left - scissor_left)
    // negative when leftmost column is left of scissor window
    int64_t scissor_left_column_mask = ~((left - scissor_left) >> 63);

    for (px = left; px < right; px += 2) {
      // Compute the masks for each determinant at each of the four pixels
      // The idea is that if the determinant is positive, then the pixel is
      // inside the triangle; consequently, if the MSB sign bit is zero, then
      // the mask should be all 1's.
      // Because the test is >= 0, we can OR together the sign bits and create
      // a unified mask (if any sign bit is set, then the mask is all zeroes,
      // otherwise it is all ones.)
      int64_t TL_Mask = ~((Dp01_TL | Dp12_TL | Dp20_TL) >> 63);
      int64_t TR_Mask = ~((Dp01_TR | Dp12_TR | Dp20_TR) >> 63);
      int64_t BL_Mask = ~((Dp01_BL | Dp12_BL | Dp20_BL) >> 63);
      int64_t BR_Mask = ~((Dp01_BR | Dp12_BR | Dp20_BR) >> 63);
            
      // ((px + 1) - scissor_right)
      // positive when the right side column of fragment quadruples are to 
      // the right of the scissor window, negative otherwise.
      int64_t scissor_right_column_mask = ((px + 1) - scissor_right) >> 63;

      TL_Mask = TL_Mask & scissor_top_row_mask    & scissor_left_column_mask;
      TR_Mask = TR_Mask & scissor_top_row_mask    & scissor_right_column_mask;
      BL_Mask = BL_Mask & scissor_bottom_row_mask & scissor_left_column_mask;
      BR_Mask = BR_Mask & scissor_bottom_row_mask & scissor_right_column_mask;

      scissor_left_column_mask = ~(uint64_t)0;  /* only relevant for first column */

      int64_t Any_Fragment_Valid = TL_Mask | TR_Mask | BL_Mask | BR_Mask;

      if (Any_Fragment_Valid) {
        // Emit 4 fragments.
        if (TL_Mask) {
          pixel_TL[0] = (uint8_t)((z_x_TL & 1) ? 0xCF : 0x3F);
          pixel_TL[1] = (uint8_t)((z_x_TL & 1) ? 0xCF : 0x3F);
          pixel_TL[2] = (uint8_t)((z_x_TL & 1) ? 0xCF : 0x3F);
          pixel_TL[3] = 0xFF;
        }
        if (TR_Mask) {
          pixel_TR[0] = (uint8_t)((z_x_TR & 1) ? 0xCF : 0x3F);
          pixel_TR[1] = (uint8_t)((z_x_TR & 1) ? 0xCF : 0x3F);
          pixel_TR[2] = (uint8_t)((z_x_TR & 1) ? 0xCF : 0x3F);
          pixel_TR[3] = 0xFF;
        }
        if (BL_Mask) {
          pixel_BL[0] = (uint8_t)((z_x_BL & 1) ? 0xCF : 0x3F);
          pixel_BL[1] = (uint8_t)((z_x_BL & 1) ? 0xCF : 0x3F);
          pixel_BL[2] = (uint8_t)((z_x_BL & 1) ? 0xCF : 0x3F);
          pixel_BL[3] = 0xFF;
        }
        if (BR_Mask) {
          pixel_BR[0] = (uint8_t)((z_x_BR & 1) ? 0xCF : 0x3F);
          pixel_BR[1] = (uint8_t)((z_x_BR & 1) ? 0xCF : 0x3F);
          pixel_BR[2] = (uint8_t)((z_x_BR & 1) ? 0xCF : 0x3F);
          pixel_BR[3] = 0xFF;
        }
      }

      {
        struct {
          int64_t x_, y_;
          uint64_t z_buf_num_;
          uint64_t z_buf_value_;
        } quadruple_samples[4] = {
          { px,     py,     z_sx_TL, z_x_TL },
          { px + 1, py,     z_sx_TR, z_x_TR },
          { px,     py + 1, z_sx_BL, z_x_BL },
          { px + 1, py + 1, z_sx_BR, z_x_BR }
        };
        size_t sample_index;
        for (sample_index = 0; sample_index < num_samples; ++sample_index) {
          struct tri10_sample *samp = samples + sample_index;
          if (((samp->x_ & ~1) == (px & ~1)) &&
              ((samp->y_ & ~1) == (py & ~1))) {
            samp->z_buf_num_recorded_ = quadruple_samples[(samp->x_ & 1) + (samp->y_ & 1) * 2].z_buf_num_;
            samp->z_buf_value_recorded_ = quadruple_samples[(samp->x_ & 1) + (samp->y_ & 1) * 2].z_buf_value_;
            if (samp->z_buf_value_ != samp->z_buf_value_recorded_) {
              printf("pix9 sample mismatch at (%d, %d)\n", (int)px, (int)py);
            }
          }
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

      /* Step z_x to next column */
      z_x_TL += z_xq;
      z_sx_TL -= z_xp;
      step_mask = z_sx_TL >> 63;
      z_sx_TL += D012 & step_mask;
      z_x_TL += z_xi & step_mask;

      z_x_TR += z_xq;
      z_sx_TR -= z_xp;
      step_mask = z_sx_TR >> 63;
      z_sx_TR += D012 & step_mask;
      z_x_TR += z_xi & step_mask;

      z_x_BL += z_xq;
      z_sx_BL -= z_xp;
      step_mask = z_sx_BL >> 63;
      z_sx_BL += D012 & step_mask;
      z_x_BL += z_xi & step_mask;

      z_x_BR += z_xq;
      z_sx_BR -= z_xp;
      step_mask = z_sx_BR >> 63;
      z_sx_BR += D012 & step_mask;
      z_x_BR += z_xi & step_mask;

      /* Step pixels to the next (stepping over 2 pixels at a time) */
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

void tri10_no_subpixels(uint8_t *rgba, size_t stride,
                        uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
                        int32_t x0, int32_t y0, uint32_t z0,
                        int32_t x1, int32_t y1, uint32_t z1,
                        int32_t x2, int32_t y2, uint32_t z2,
                        size_t num_samples, struct tri10_sample *samples) {
  tri10(rgba, stride, scissor_left, scissor_top, scissor_right, scissor_bottom,
        x0 << SUBPIXEL_BITS, y0 << SUBPIXEL_BITS, z0,
        x1 << SUBPIXEL_BITS, y1 << SUBPIXEL_BITS, z1,
        x2 << SUBPIXEL_BITS, y2 << SUBPIXEL_BITS, z2,
        num_samples, samples);
}


void tri10_no_subpixels_topleft_sampled(uint8_t *rgba, size_t stride,
                                        uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
                                        int32_t x0, int32_t y0, uint32_t z0,
                                        int32_t x1, int32_t y1, uint32_t z1,
                                        int32_t x2, int32_t y2, uint32_t z2,
                                        size_t num_samples, struct tri10_sample *samples) {
  tri10(rgba, stride, scissor_left, scissor_top, scissor_right, scissor_bottom,
        (1 << (SUBPIXEL_BITS-1)) + (x0 << SUBPIXEL_BITS), (1 << (SUBPIXEL_BITS-1)) + (y0 << SUBPIXEL_BITS), z0,
        (1 << (SUBPIXEL_BITS-1)) + (x1 << SUBPIXEL_BITS), (1 << (SUBPIXEL_BITS-1)) + (y1 << SUBPIXEL_BITS), z1,
        (1 << (SUBPIXEL_BITS-1)) + (x2 << SUBPIXEL_BITS), (1 << (SUBPIXEL_BITS-1)) + (y2 << SUBPIXEL_BITS), z2,
        num_samples, samples);
}


int main(int argc, char **argv) {
  int exit_ret = EXIT_FAILURE;

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
#elif 0
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
#elif 0
  struct tri7_sample tri7_samples[] ={
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
       32, 16, 256+32, 256+16, /* scissor rect */
       103, 3, 0x0, /* vertex 0 */
       (1 << 30) - 1, (1 << 30) - 1, 0xFFFF,    /* vertex 1 */
       100, (1 << 30) - 1, 0xFFFFFFFF,         /* vertex 2 */
       sizeof(tri7_samples)/sizeof(*tri7_samples), tri7_samples);
#elif 0
  struct tri7_sample tri7_samples[] ={
    {
      0, 0,
      0x1,  // z-buf value
    },
    {
      1, 0,
      0x0,  // z-buf value
    },
    {
      0, 1,
      0x1,  // z-buf value
    },
    {
      1, 1,
      0x0,  // z-buf value
    }
  };
  tri7(rgba32, 256*4,
       0, 0, 256, 256, /* scissor rect */
       0, 0, 0x1,    /* vertex 0 */
       100, 0, 0x0,    /* vertex 1 */
       0, 100, 0x1,         /* vertex 2 */
       sizeof(tri7_samples)/sizeof(*tri7_samples), tri7_samples);
#elif 0
  struct tri7_sample tri7_samples[] ={
    {
      0, 0,
      0x1,  // z-buf value
    },
    {
      1, 0,
      0x1,  // z-buf value
    },
    {
      0, 1,
      0x0,  // z-buf value
    },
    {
      1, 1,
      0x0,  // z-buf value
    }
  };
  tri7(rgba32, 256*4,
       0, 0, 256, 256, /* scissor rect */
       0, 0, 0x1,    /* vertex 0 */
       100, 0, 0x1,    /* vertex 1 */
       0, (1 << 30) - 1, 0x0,         /* vertex 2 */
       sizeof(tri7_samples)/sizeof(*tri7_samples), tri7_samples);
#elif 0
  struct tri8_sample tri8_samples[] ={
    {
      0, 0,
      0x1,  // z-buf value
    },
    { // should cross-over exactly in the middle horizontally.
      64, 0,
      0x1,  // z-buf value
    },
    {
      65, 0,
      0x0,  // z-buf value
    },
    { // should cross-over exactly in the middle vertically too.
      0, 64,
      0x1,  // z-buf value
    },
    {
      0, 65,
      0x0,  // z-buf value
    }
  };
  tri8(rgba32, 256*4,
       0, 0, 256, 256, /* scissor rect */
       0, 0, 0x1,    /* vertex 0 */
       128, 0, 0x0,    /* vertex 1 */
       0, 128, 0x0,         /* vertex 2 */
       sizeof(tri8_samples)/sizeof(*tri8_samples), tri8_samples);
#elif 0
  struct tri9_sample tri9_samples[] ={
    {
      0, 0,
      0x1,  // z-buf value
    },
    { // should cross-over exactly in the middle horizontally.
      64, 0,
      0x1,  // z-buf value
    },
    {
      65, 0,
      0x0,  // z-buf value
    },
    { // should cross-over exactly in the middle vertically too.
      0, 64,
      0x1,  // z-buf value
    },
    {
      0, 65,
      0x0,  // z-buf value
    }
  };
  tri9_no_subpixels(rgba32, 256*4,
                    0, 0, 256, 256, /* scissor rect */
                    0, 0, 0x1,    /* vertex 0 */
                    128, 0, 0x0,    /* vertex 1 */
                    0, 128, 0x0,         /* vertex 2 */
                    sizeof(tri9_samples)/sizeof(*tri9_samples), tri9_samples);
#elif 0
  struct tri10_sample tri10_samples[] ={
    {
      0, 0,
      0x1,  // z-buf value
    },
    { // should cross-over exactly in the middle horizontally.
      64, 0,
      0x1,  // z-buf value
    },
    {
      65, 0,
      0x0,  // z-buf value
    },
    { // should cross-over exactly in the middle vertically too.
      0, 64,
      0x1,  // z-buf value
    },
    {
      0, 65,
      0x0,  // z-buf value
    }
  };
  tri10_no_subpixels_topleft_sampled(rgba32, 256*4,
                                     0, 0, 256, 256, /* scissor rect */
                                     0, 0, 0x1,    /* vertex 0 */
                                     128, 0, 0x0,    /* vertex 1 */
                                     0, 128, 0x0,         /* vertex 2 */
                                     sizeof(tri10_samples)/sizeof(*tri10_samples), tri10_samples);
#elif 0
  tri10_no_subpixels(rgba32, 256*4,
                     0, 0, 256, 256, /* scissor rect */
                     8, 8, 0x1,    /* vertex 0 */
                     128, 16, 0x0,    /* vertex 1 */
                     16, 128, 0x0,         /* vertex 2 */
                     0, NULL);
#elif 0
  tri10_no_subpixels(rgba32, 256*4,
                     9, 9, 11, 12, /* scissor rect */
                     8, 8, 0x1,    /* vertex 0 */
                     128, 16, 0x0,    /* vertex 1 */
                     16, 128, 0x0,         /* vertex 2 */
                     0, NULL);
#elif 0
  struct fragment_buffer fragbuf;
  fragment_buffer_init(&fragbuf);
  if (fragment_buffer_alloc_buffers(&fragbuf)) {
    fprintf(stderr, "fragment_buffer_alloc_buffers failed\n");
    fragment_buffer_cleanup(&fragbuf);
    return EXIT_FAILURE;
  }
  struct rasterizer razer;
  rasterizer_init(&razer);
  while (rasterizer_triangle(&razer, &fragbuf,
                             rgba32, 256*4, NULL, 0, 0,
                             0, 0, 256, 256, /* scissor rect */
                             8   << RASTERIZER_SUBPIXEL_BITS, 8   << RASTERIZER_SUBPIXEL_BITS, 0x1,    /* vertex 0 */
                             128 << RASTERIZER_SUBPIXEL_BITS, 16  << RASTERIZER_SUBPIXEL_BITS, 0x0,    /* vertex 1 */
                             16  << RASTERIZER_SUBPIXEL_BITS, 128 << RASTERIZER_SUBPIXEL_BITS, 0x0)) {      /* vertex 2 */
    size_t frag_index;
    for (frag_index = 0; frag_index < fragbuf.num_rows_; ++frag_index) {
      #if 0
      /* Replicating this behavior: */
      if (TL_Mask) {
        pixel_TL[0] = (uint8_t)((z_x_TL & 1) ? 0xCF : 0x3F);
        pixel_TL[1] = (uint8_t)((z_x_TL & 1) ? 0xCF : 0x3F);
        pixel_TL[2] = (uint8_t)((z_x_TL & 1) ? 0xCF : 0x3F);
        pixel_TL[3] = 0xFF;
      }
      #endif
      uint8_t mask = ((uint8_t *)fragbuf.column_data_[FB_IDX_MASK])[frag_index];
      if (mask) {
        uint32_t z = ((uint32_t *)fragbuf.column_data_[FB_IDX_ZBUF_VALUE])[frag_index];
        uint8_t *pixel = ((uint8_t **)fragbuf.column_data_[FB_IDX_PIXEL_PTR])[frag_index];
        pixel[0] = (uint8_t)((z & 1) ? 0xCF : 0x3F);
        pixel[1] = (uint8_t)((z & 1) ? 0xCF : 0x3F);
        pixel[2] = (uint8_t)((z & 1) ? 0xCF : 0x3F);
        pixel[3] = 0xFF;
      }
    }
    fragbuf.num_rows_ = 0;
  }
#elif 1
  struct primitive_assembly pa;
  struct attrib_set as;
  struct clipping_stage cs;
  struct rasterizer ras;
  struct fragment_buffer fragbuf;

  primitive_assembly_init(&pa);
  attrib_set_init(&as);
  clipping_stage_init(&cs);
  rasterizer_init(&ras);
  fragment_buffer_init(&fragbuf);

  if (primitive_assembly_alloc_buffers(&pa) ||
      clipping_stage_alloc_varyings(&cs, 0) ||
      fragment_buffer_alloc_buffers(&fragbuf)) {
    fprintf(stderr, "Failed to initialize due to allocation failure\n");
    goto exit_cleanup;
  }

  int32_t vp_x = 0; /* left */
  int32_t vp_y = 0; /* bottom */
  uint32_t vp_width = 256;
  uint32_t vp_height = 256;
  float depth_range_near = 0.f;
  float depth_range_far = 1.f;
  uint32_t screen_width = 256;
  uint32_t screen_height = 256;
  uint32_t max_z = 0xFFFF;

  /* Going for XYZ, will lean on W being implied 1.
   * Trying to get 1.f on the z-buf, but this is likely going to be difficult to fit
   * in a float. */
  float verts[] = {
    -1.f + 2.f *   (8.f/256.f), 1.f - 2.f *   (8.f/256.f), (float)(-1.f + 1. * 2.f / (double)(max_z)),
    -1.f + 2.f * (128.f/256.f), 1.f - 2.f *  (16.f/256.f), (float)(-1.f),
    -1.f + 2.f *  (16.f/256.f), 1.f - 2.f * (128.f/256.f), (float)(-1.f)
  };
  int xyz_attr = attrib_set_alloc_attrib(&as);
  if (xyz_attr < 0) goto exit_cleanup;
  as.attribs_[xyz_attr].buf_ = NULL;
  as.attribs_[xyz_attr].size_ = 3; /* Rely on fourth coord W to be the fixed 1. preset */
  as.attribs_[xyz_attr].data_type_ = ADT_FLOAT;
  as.attribs_[xyz_attr].att_ = AT_VEC4;
  as.attribs_[xyz_attr].normalize_ = 0;
  as.attribs_[xyz_attr].enabled_ = 1;
  as.attribs_[xyz_attr].ptr_ = verts;
  as.attribs_[xyz_attr].stride_ = sizeof(float) * 3;
  // as.attribs_[xyz_attr].name_ = // canonical way of setting this still to be made

  pa.column_descriptors_[PACT_POSITION_X].attrib_index_ = xyz_attr;
  pa.column_descriptors_[PACT_POSITION_X].attrib_element_index_ = 0;
  pa.column_descriptors_[PACT_POSITION_Y].attrib_index_ = xyz_attr;
  pa.column_descriptors_[PACT_POSITION_Y].attrib_element_index_ = 1;
  pa.column_descriptors_[PACT_POSITION_Z].attrib_index_ = xyz_attr;
  pa.column_descriptors_[PACT_POSITION_Z].attrib_element_index_ = 2;

  /* Note that xyz_attr has size_ 3, not size_ 4, consequently, attrib_element_index_ 
   * should map into the attrib::generic_values_[3] value, which, by default, should
   * map to 1. */
  pa.column_descriptors_[PACT_POSITION_W].attrib_index_ = xyz_attr;
  pa.column_descriptors_[PACT_POSITION_W].attrib_element_index_ = 3;

  uint32_t indices[] = {
    0, 1, 2
  };

  primitive_assembly_draw_elements(&pa, &as, &cs, &ras, &fragbuf,
                                   vp_x, vp_y, vp_width, vp_height, depth_range_near, depth_range_far,
                                   screen_width, screen_height, max_z,
                                   rgba32, screen_width*4,
                                   NULL, 256*4, 4,
                                   NULL, 256*2, 2,
                                   0, /* no stencil test */
                                   /* Settings for stencil on clockwise triangles: */
                                   ~(uint32_t)0, /* clockwise stencil mask: all output bits enabled */
                                   PASF_NEVER, /* clockwise stencil test: never pass */
                                   0, 0, /* clockwise stencil function reference value and function mask value */
                                   PASO_KEEP, PASO_KEEP, PASO_KEEP, /* clockwise stencil-fail, stencil-success, stencil&depth-success: all keep */
                                   /* Same for counter-clockwise: */
                                   ~(uint32_t)0, 
                                   PASF_NEVER,
                                   0, 0,
                                   PASO_KEEP, PASO_KEEP, PASO_KEEP,
                                   0, PAZF_LESS, 0, /* enable_zbuf_test, zbuf_func, enable_zbuf_write */
                                   1, 1, 1, 1,
                                   BEQ_FUNC_ADD, BEQ_FUNC_ADD,
                                   BF_SRC_ALPHA, BF_SRC_ALPHA,
                                   BF_ONE_MINUS_SRC_ALPHA, BF_ONE_MINUS_SRC_ALPHA,
                                   0, 0, 0, 0,
                                   PAM_TRIANGLES, 3, PAIT_UNSIGNED_INT, indices);

  exit_ret = EXIT_SUCCESS;
  exit_cleanup:
  fragment_buffer_cleanup(&fragbuf);
  rasterizer_cleanup(&ras);
  clipping_stage_cleanup(&cs);
  attrib_set_cleanup(&as);
  primitive_assembly_cleanup(&pa);

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
    exit_ret = EXIT_FAILURE;
    return exit_ret;
  }
  write_rgba_bmp(fp, rgba32, 256, 256, 256 * 4);
  fclose(fp);

  return exit_ret;
}
