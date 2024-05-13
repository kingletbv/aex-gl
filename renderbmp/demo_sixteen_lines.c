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

#include "opengl_es2_headers.h"

int demo_sixteen_lines(int output_width, int output_height) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    "varying vec4 vertex_color;\n"
    "const float half_pi = 3.14159265358979/2.;\n"
    "const float f_one = sin(half_pi);\n"
    "void main() {\n"
    "  vertex_color = v_color;\n"
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
    "precision highp float;\n"
    "varying vec4 vertex_color;\n"
    "void main() {\n"
    //"  gl_FragColor = vec4(1., 1., 1., 1.);\n"
    "  gl_FragColor = vertex_color;\n"
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

  /* Going for XYZ, will lean on W being implied 1.
   * Trying to get 1.f on the z-buf, but this is likely going to be difficult to fit
   * in a float. */
  uint32_t max_z = 0xFFFFFFFF;
#define PIX_COORD(x,y) \
  -1.f + 2.f * ((.5f + (float)4*(x)) / (float)output_width), 1.f - 2.f * ((.5f + (float)4*(y)) / (float)output_height), -.5f, 1.f

  float verts[] = {
    PIX_COORD(128 + 16, 128 + 0),
    PIX_COORD(128 + 32, 128 + 0),

    PIX_COORD(128 + 16, 128 + 8),
    PIX_COORD(128 + 32, 128 + 16),

    PIX_COORD(128 + 16, 128 + 16),
    PIX_COORD(128 + 32, 128 + 32),

    PIX_COORD(128 +  8, 128 + 16),
    PIX_COORD(128 + 16, 128 + 32),

    PIX_COORD(128 +  0, 128 + 16),
    PIX_COORD(128 +  0, 128 + 32),

    PIX_COORD(128 -  8, 128 + 16),
    PIX_COORD(128 - 16, 128 + 32),

    PIX_COORD(128 - 16, 128 + 16),
    PIX_COORD(128 - 32, 128 + 32),

    PIX_COORD(128 - 16, 128 + 8),
    PIX_COORD(128 - 32, 128 + 16),


    PIX_COORD(128 - 16, 128 + 0),
    PIX_COORD(128 - 32, 128 + 0),

    PIX_COORD(128 - 16, 128 - 8),
    PIX_COORD(128 - 32, 128 - 16),

    PIX_COORD(128 - 16, 128 - 16),
    PIX_COORD(128 - 32, 128 - 32),

    PIX_COORD(128 -  8, 128 - 16),
    PIX_COORD(128 - 16, 128 - 32),

    PIX_COORD(128 -  0, 128 - 16),
    PIX_COORD(128 -  0, 128 - 32),

    PIX_COORD(128 +  8, 128 - 16),
    PIX_COORD(128 + 16, 128 - 32),

    PIX_COORD(128 + 16, 128 - 16),
    PIX_COORD(128 + 32, 128 - 32),

    PIX_COORD(128 + 16, 128 - 8),
    PIX_COORD(128 + 32, 128 - 16)
  };

  glEnableVertexAttribArray(attrib_vgl_Position);
  glVertexAttribPointer(attrib_vgl_Position,  /* index */
                        4,                    /* size (number of components) */
                        GL_FLOAT,             /* type */
                        0,                    /* normalized */
                        0,                    /* stride (0 = adjacent in memory) */
                        verts);               /* pointer */

  float v_colors[] = {
    0.f, 0.f, 0.f, 1.f,
    0.f, 0.f, 0.f, 1.f,
    .6f, 0.f, 0.f, 1.f,
    .6f, 0.f, 0.f, 1.f,
    0.f, .6f, 0.f, 1.f,
    0.f, .6f, 0.f, 1.f,
    .6f, .6f, 0.f, 1.f,
    .6f, .6f, 0.f, 1.f,
    0.f, 0.f, .6f, 1.f,
    0.f, 0.f, .6f, 1.f,
    .6f, 0.f, .6f, 1.f,
    .6f, 0.f, .6f, 1.f,
    0.f, .6f, .6f, 1.f,
    0.f, .6f, .6f, 1.f,
    .6f, .6f, .6f, 1.f,
    .6f, .6f, .6f, 1.f,

    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 0.f, 0.f, 1.f,
    1.f, 0.f, 0.f, 1.f,
    0.f, 1.f, 0.f, 1.f,
    0.f, 1.f, 0.f, 1.f,
    1.f, 1.f, 0.f, 1.f,
    1.f, 1.f, 0.f, 1.f,
    0.f, 0.f, 1.f, 1.f,
    0.f, 0.f, 1.f, 1.f,
    1.f, 0.f, 1.f, 1.f,
    1.f, 0.f, 1.f, 1.f,
    0.f, 1.f, 1.f, 1.f,
    0.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
  };
  glEnableVertexAttribArray(attrib_v_color);
  glVertexAttribPointer(attrib_v_color,       /* index */
                        4,                    /* size (number of components) */
                        GL_FLOAT,             /* type */
                        0,                    /* normalized */
                        0,                    /* stride (0 = adjacent in memory) */
                        v_colors);            /* pointer */

  glDrawArrays(GL_LINES, 0, 16 * 2);
  //glDrawArrays(GL_LINES, 0, 4);
  
  return 0;
}
