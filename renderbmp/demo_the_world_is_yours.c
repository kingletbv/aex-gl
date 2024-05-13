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
#include <math.h>
#include <inttypes.h>

#include "demos.h"

#include "../3rdparty/world/world.h"

#include "opengl_es2_headers.h"

static void generate_world_c(void) {
  FILE *fp = fopen("../3rdparty/world/world.bin", "rb");

  fseek(fp, 0, SEEK_END);
  size_t flen = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  float *buf = (float *)malloc(flen);
  fread(buf, 1, flen, fp); /* assumes little-endian */
  fclose(fp);
  size_t num_floats = flen / sizeof(float);
  FILE *fpout = fopen("../3rdparty/world/world.c", "wb");
  fprintf(fpout, "#include \"world.h\"\n\n");
  fprintf(fpout, "size_t g_num_world_coords_ = %d;\n", (int)(num_floats / 2));
  fprintf(fpout, "float g_world_coords_[] = {\n");
  size_t n;
  for (n = 0; n < num_floats; n += 2) {
    fprintf(fpout, "  %ff, %ff%s", buf[n], buf[n + 1], ((n + 2) < num_floats) ? ",\n" : "\n");
  }
  fprintf(fpout, "};\n");
  fclose(fpout);
}

void rotn(float *m, float nx, float ny, float nz, float ang) {
  float c = cosf(ang);
  float s = sinf(ang);
  m[0] = nx * nx + (ny * ny + nz * nz) * c;
  m[1] = nx * ny * (1 - c) + nz * s;
  m[2] = nx * nz * (1 - c) - ny * s;
  m[3] = 0.f;
  m[4] = nx * ny * (1 - c) - nz * s;
  m[5] = ny * ny + (nx * nx + nz * nz) * c;
  m[6] = ny * nz * (1 - c) + nx * s;
  m[7] = 0.f;
  m[8] = nx * nz * (1 - c) + ny * s;
  m[9] = ny * nz * (1 - c) - nx * s;
  m[10] = nz * nz + (nx * nx + ny * ny) * c;
  m[11] = 0.f;
  m[12] = 0.f;
  m[13] = 0.f;
  m[14] = 0.f;
  m[15] = 1.f;
}

void persp_fovy(float *m, float near, float far, float width, float height, float fovy) {
  float range = height / (2.f * tanf(fovy / 2.f));
  float q = far / (far - near);
  float w = range / (width / 2.f);
  float h = range / (height / 2.f);
  m[0] = w;
  m[1] = 0.f;
  m[2] = 0.f;
  m[3] = 0.f;
  m[4] = 0.f;
  m[5] = h;
  m[6] = 0.f;
  m[7] = 0.f;
  m[8] = 0.f;
  m[9] = 0.f;
  m[10] = q;
  m[11] = 1.f;
  m[12] = 0.f;
  m[13] = 0.f;
  m[14] = -q * near;
  m[15] = 0.f;
}

void trln(float *m, float x, float y, float z) {
  m[0] = 1.f;
  m[1] = 0.f;
  m[2] = 0.f;
  m[3] = 0.f;
  m[4] = 0.f;
  m[5] = 1.f;
  m[6] = 0.f;
  m[7] = 0.f;
  m[8] = 0.f;
  m[9] = 0.f;
  m[10] = 1.f;
  m[11] = 0.f;
  m[12] = x;
  m[13] = y;
  m[14] = z;
  m[15] = 1.f;
}

void unitm(float *m) {
  m[0] = 1.f;
  m[1] = 0.f;
  m[2] = 0.f;
  m[3] = 0.f;
  
  m[4] = 0.f;
  m[5] = 1.f;
  m[6] = 0.f;
  m[7] = 0.f;
  
  m[8] = 0.f;
  m[9] = 0.f;
  m[10] = 1.f;
  m[11] = 0.f;

  m[12] = 0.f;
  m[13] = 0.f;
  m[14] = 0.f;
  m[15] = 1.f;
}

void mul_pair(float *r, float *a, float *b) {
  size_t col, row;
  for (col = 0; col < 4; ++col) {
    for (row = 0; row < 4; ++row) {
      r[col * 4 + row] = a[0 + row] * b[col * 4 + 0] +
                         a[4 + row] * b[col * 4 + 1] +
                         a[8 + row] * b[col * 4 + 2] +
                         a[12+ row] * b[col * 4 + 3];
    }
  }
}

void mulm(float *m, ...) {
  va_list ap;
  va_start(ap, m);
  float *accumulator = m;
  float *next;
  unitm(m);
  while ((next = va_arg(ap, float *))) {
    float new_accumulator[16];
    mul_pair(new_accumulator, accumulator, next);
    memcpy(accumulator, new_accumulator, sizeof(new_accumulator));
  }
  va_end(ap);
}


int demo_the_world_is_yours(int output_width, int output_height) {
  /* Build vertex and fragment shaders */
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

  const char *vsrc =
    "attribute vec3 a_position;\n"
    "uniform mat4 u_matrix;\n"
    "void main() {\n"
    "  /* Multiply the position by the matrix */\n"
    "  gl_Position = u_matrix * vec4(a_position.xyz, 1);\n"
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
    "precision mediump float;\n"
    "\n"
    "uniform vec4 u_color;\n"
    "\n"
    "void main() {\n"
    "  gl_FragColor = u_color;\n"
    "  float z = 1. / gl_FragCoord.w;\n"
    "  /* 200 is not the center of the sphere (this is at 120 + 180 = 300), but\n"
    "   * the tangent of the sphere's surface by visual inspection for the fovy\n"
    "   * of 90 */\n"
    "  if (z >= 200.) {\n"
    "    discard;\n"
    "  }\n"
    "  /* Actual sphere front side runs from 120 (transaltion 300 minus 180 radius)\n"
    "   * to 200 (full tangent as per above) */\n"
    "  gl_FragColor = mix(vec4(222./255., 124./255.,  8./255., .8),\n"
    "                     vec4( 95./255.,   2./255., 31./255., 1.),\n"
    "                     (z - 120.) / (200. - 120.));\n"
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

  GLint attrib_a_position  = glGetAttribLocation(program, "a_position");

  GLint uniform_u_color = glGetUniformLocation(program, "u_color");
  GLint uniform_u_matrix = glGetUniformLocation(program, "u_matrix");

  float *world = (float *)malloc(sizeof(float) * g_num_world_coords_ * 3);
  if (!world) {
    fprintf(stderr, "Failed memory allocation\n");
    return -1;
  }

  size_t coord;
  for (coord = 0; coord < g_num_world_coords_; ++coord) {
    float longitude, lattitude;
    longitude = g_world_coords_[coord * 2 + 0];
    lattitude = g_world_coords_[coord * 2 + 1];
    float *xyz = world + coord * 3;

#define PI 3.14159265358979323846f

    xyz[0] = 180.f * sinf(PI * longitude / 180.f) * cosf(PI * lattitude / 180.f);
    xyz[1] = 180.f * sinf(PI * lattitude / 180.f);
    xyz[2] = -180.f * cosf(PI * longitude / 180.f) * cosf(PI * lattitude / 180.f);
  }

  glViewport(0, 0, output_width, output_height);

  //glClearColor(21.f/255.f, 2.f/255.f, 31.f/255.f, 1.f);
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

  glEnableVertexAttribArray(attrib_a_position);
  GLint size = 3;
  GLenum type = GL_FLOAT;
  GLboolean normalize = GL_FALSE;
  GLsizei stride = 0;
  
  glVertexAttribPointer(attrib_a_position, size, type, normalize, stride, world);

  float now_in_secs = 0.f;
  float angle = (now_in_secs * PI * 2.f) / 60.f;

  float projection_matrix[16];
  persp_fovy(projection_matrix, .1f, 4000.f, (float)output_width, (float)output_height, .5f * PI);
  float translation_matrix[16];
  trln(translation_matrix, 0.f, 0.f, 300.f);
  float rotation_matrix[16];
  rotn(rotation_matrix, 0.f, 1.f, 0.f, angle);
  float pitch_matrix[16];
  rotn(pitch_matrix, 1.f, 0.f, 0.f, PI * -40.f / 180.f);
  float roll_matrix[16];
  rotn(roll_matrix, 0.f, 0.f, 1.f, PI * -10.f / 180.f);
  float transformation_matrix[16];
  mulm(transformation_matrix, projection_matrix, translation_matrix, roll_matrix, pitch_matrix, rotation_matrix, NULL);

  glUniformMatrix4fv(uniform_u_matrix, 1, GL_FALSE, transformation_matrix);

  float color[4] = { 1.f, 1.f, 1.f, 1.f };

  glUniform4fv(uniform_u_color, 1, color);

  glDrawArrays(GL_LINES, 0, (GLsizei)g_num_world_coords_);

  free(world);

  return 0;
}

