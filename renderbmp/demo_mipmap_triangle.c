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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <inttypes.h>

#include "demos.h"
#include "smiley.h"
#include "gl_es2_impl.h"

/* we will define the prototypes as static function pointers 
 * by including gl_es2_aex_func_map.c */
#define GL_GLES_PROTOTYPES 0
#include <GLES2/gl2.h>

#include "gl_es2_aex_func_map.c"

int demo_mipmap_triangle(int output_width, int output_height) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  /* Recreate the gradient background we had on our original bitmap, however, given
   * as we're using OpenGL ES2 this time around, we cannot directly set pixels to
   * our desired colors (there is no "glWritePixels"). Instead, use glClear with 
   * a scissor-rect. */
  {
    int row;
    /* Background is a purple-to-pink'ish gradient */
    for (row = 0; row < output_height; row++) {
      uint8_t rgba[4];
      rgba[0] = (row * 256) / (2 * output_height);
      rgba[1] = 0;
      rgba[2] = (row * 256) / (4 * output_height) + 64;
      rgba[3] = 255;

      glClearColor(rgba[0] / 255.f, 
                   rgba[1] / 255.f, 
                   rgba[2] / 255.f,
                   rgba[3] / 255.f);
      glScissor(0, output_height - row - 1, output_width, 1);
      glEnable(GL_SCISSOR_TEST);
      glClear(GL_COLOR_BUFFER_BIT);
    }
  }

  glScissor(0, 0, output_width, output_height);
  glDisable(GL_SCISSOR_TEST);

  /* Build vertex and fragment shaders */
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

  const char *vsrc =
    "attribute vec4 vgl_Position;\n"
    "attribute vec4 v_color;\n"
    "attribute vec2 v_st;\n"
    "varying vec2 vertex_st;\n"
    "varying vec4 vertex_color;\n"
    "const float half_pi = 3.14159265358979/2.;\n"
    "const float f_one = sin(half_pi);\n"
    "void main() {\n"
    "  vertex_color = v_color;\n"
    "  vertex_st = v_st;\n"
    "  gl_Position = vgl_Position;\n"
    "  gl_Position.x *= gl_Position.w;\n"
    "  gl_Position.y *= gl_Position.w;\n"
    "}\n";
  GLint vsrc_len = (int)strlen(vsrc);
  glShaderSource(vertex_shader, 1, &vsrc, &vsrc_len);

  glCompileShader(vertex_shader);

  GLint vcompile_status = 0;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vcompile_status);

  if (vcompile_status == GL_FALSE) {
    fprintf(stderr, "Failed to compile vertex shader. Diagnostics:\n");
    if (print_shader_log(stderr, vertex_shader)) {
      /* Failure printing log */
      return -1;
    }
  }

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  const char *fsrc = 
    "varying vec4 vertex_color;\n"
    "varying vec2 vertex_st;\n"
    "uniform sampler2D tex;\n"
    "void main() {\n"
    "  int x = int(gl_FragCoord.x) + int(gl_FragCoord.y);\n"
    "  int even_x = (x / 2) * 2;\n"
    //"  if ((x - even_x) == 1) discard;\n"
    "  int checker = int(10.f * vertex_st.x) + int(10.f * vertex_st.y);\n"
    "  int checker_even = (checker / 2) * 2;\n"
    "  float checker_flag = float(checker - checker_even);\n"
    "  vec4 tsmiley = texture2D(tex, vertex_st);\n"
    "  gl_FragColor = vec4(tsmiley.xyz, 1.);\n"
    "}\n";
  GLint fsrc_len = (int)strlen(fsrc);
  glShaderSource(fragment_shader, 1, &fsrc, &fsrc_len);

  glCompileShader(fragment_shader);

  GLint fcompile_status = 0;
  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fcompile_status);

  if (fcompile_status == GL_FALSE) {
    fprintf(stderr, "Failed to compile fragment shader. Diagnostics:\n");
    if (print_shader_log(stderr, fragment_shader)) {
      /* Failure printing log */
      return -1;
    }
  }

  GLuint program = glCreateProgram();

  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);

  glLinkProgram(program);

  GLint plink_status = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &plink_status);

  if (plink_status == GL_FALSE) {
    fprintf(stderr, "Failed to link program. Diagnostics:\n");
    if (print_program_log(stderr, program)) {
      /* Failure printing log */
      return -1;
    }
  }

  glUseProgram(program);

  GLint attrib_vgl_Position = glGetAttribLocation(program, "vgl_Position");
  GLint attrib_v_color = glGetAttribLocation(program, "v_color");
  GLint attrib_v_st = glGetAttribLocation(program, "v_st");

  /* Going for XYZ, will lean on W being implied 1.
   * Trying to get 1.f on the z-buf, but this is likely going to be difficult to fit
   * in a float. */
  uint32_t max_z = 0xFFFFFFFF;
  float verts[] = {
    -1.f + 2.f *   (8.f/256.f), 1.f - 2.f *   (8.f/256.f), (float)(-1.f + 1. * 2.f / (double)(max_z)), 9.f,
    -1.f + 2.f * (128.f/256.f), 1.f - 2.f *  (16.f/256.f), (float)(-1.f), 9.f,
    -1.f + 2.f *  (16.f/256.f), 1.f - 2.f * (248/256.f), (float)(-1.f), 1.f,
    -1.f + 2.f * (136.f/256.f), 1.f - 2.f * (256/256.f), (float)(-1.f), 1.f
  };
  glEnableVertexAttribArray(attrib_vgl_Position);
  glVertexAttribPointer(attrib_vgl_Position,  /* index */
                        4,                    /* size (number of components) */
                        GL_FLOAT,             /* type */
                        0,                    /* normalized */
                        0,                    /* stride (0 = adjacent in memory) */
                        verts);               /* pointer */

  float v_colors[] = {
    1.f, 0.f, 0.f, 1.f,
    0.f, 1.f, 0.f, 1.f,
    0.f, 0.f, 1.f, 1.f,
    1.f, 0.f, 1.f, 1.f,
  };
  glEnableVertexAttribArray(attrib_v_color);
  glVertexAttribPointer(attrib_v_color,       /* index */
                        4,                    /* size (number of components) */
                        GL_FLOAT,             /* type */
                        0,                    /* normalized */
                        0,                    /* stride (0 = adjacent in memory) */
                        v_colors);            /* pointer */

  float v_st[] = {
    0.f, 0.f,
    10.f, 0.f,
    0.f, 10.f,
    10.f, 10.f
  };
  glEnableVertexAttribArray(attrib_v_st);
  glVertexAttribPointer(attrib_v_st,          /* index */
                        2,                    /* size (number of components) */
                        GL_FLOAT,             /* type */
                        0,                    /* normalized */
                        0,                    /* stride (0 = adjacent in memory) */
                        v_st);                /* pointer */


  uint32_t indices[] = {
    0, 1, 2,
    2, 1, 3
  };

  static unsigned char smiley[256*256*3];

  size_t smiley_row, smiley_column;
  for (smiley_row = 0; smiley_row < 256; ++smiley_row) {
    for (smiley_column = 0; smiley_column < 256; ++smiley_column) {
      size_t source_index = smiley_row * 256 + smiley_column;
      size_t target_index = (256 - smiley_row - 1) * 256 * 3 + smiley_column * 3;
      smiley[target_index + 0] = g_smiley_256x256_luminance_data_[source_index];
      smiley[target_index + 1] = g_smiley_256x256_luminance_data_[source_index];
      smiley[target_index + 2] = g_smiley_256x256_luminance_data_[source_index];
    }
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, smiley);

  /* Go in, and modify each of the higher mipmap levels to color cycle, this will help us know
   * if mipmapping is working, and reason about what map we're on */
  uint8_t color_cycle[] = {
    0xFF, 0x00, 0x00, // Red
    0xFF, 0xFF, 0x00, // Yellow
    0x00, 0xFF, 0x00, // Green
    0x00, 0xFF, 0xFF, // Cyan
    0x00, 0x00, 0xFF, // Blue
    0xFF, 0x00, 0xFF  // Magenta
  };
  size_t num_colors_in_cycle = sizeof(color_cycle) / 3;

  /* mipmap_smiley will receive a color shading, the smiley[] bitmap
   * itself will not (otherwise we'll be filtering that color shading
   * which won't have the desired effect.) */
  static unsigned char mipmap_smiley[256*256*3];

  int level;
  for (level = 1; level < 9; ++level) {
    size_t smiley_dst_width, smiley_dst_height;
    size_t smiley_src_width, smiley_src_height;
    smiley_src_width = 256 >> (level - 1);
    smiley_src_height = 256 >> (level - 1);
    smiley_dst_width = smiley_src_width / 2;
    smiley_dst_height = smiley_src_height / 2;

    size_t smiley_src_stride = (smiley_src_width * 3 + 4 - 1) & ~(4 - 1);
    size_t smiley_dst_stride = (smiley_dst_width * 3 + 4 - 1) & ~(4 - 1);

    for (smiley_row = 0; smiley_row < smiley_dst_height; ++smiley_row) {
      for (smiley_column = 0; smiley_column < smiley_dst_width; ++smiley_column) {
        unsigned char *dst = smiley + smiley_row * smiley_dst_stride + 3 * smiley_column;
        unsigned char *dst_shaded = mipmap_smiley + smiley_row * smiley_dst_stride + 3 * smiley_column;
        unsigned char *src_tl = smiley + smiley_row * 2 * smiley_src_stride + 3 * 2 * smiley_column;
        unsigned char *src_tr = src_tl + 3;
        unsigned char *src_bl = src_tl + smiley_src_stride;
        unsigned char *src_br = src_bl + 3;
        size_t component;
        for (component = 0; component < 3; ++component) {
          unsigned char result = (unsigned char)((((uint32_t)src_tl[component])
                                                  + ((uint32_t)src_tr[component])
                                                  + ((uint32_t)src_bl[component])
                                                  + ((uint32_t)src_br[component])) / 4);
          unsigned char color_cycle_component = color_cycle[((level - 1) % num_colors_in_cycle) * 3 + component];
          dst[component] = result;
          dst_shaded[component] = result + ((((uint32_t)(0xFF - result)) * color_cycle_component) >> 8);
        }
      }
    }
    glTexImage2D(GL_TEXTURE_2D, level, GL_RGB, (GLsizei)smiley_dst_width, (GLsizei)smiley_dst_height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, mipmap_smiley);

  }
  //glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, indices);

  return 0;
}
