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

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifdef USE_STANDARD_NON_AEX_GL_HEADERS
#ifndef GLES2_GL2_H_INCLUDED
#define GLES2_GL2_H_INCLUDED
#include <GLES2/gl2.h>
#endif
#else
#include "gl_es2_impl.h"

#ifndef GLES2_GL2_H_INCLUDED
#define GLES2_GL2_H_INCLUDED
#define GL_GLES_PROTOTYPES 0  /* we will define the prototypes as static function pointers by including gl_es2_aex_func_map.c */
#include <GLES2/gl2.h>
#endif
#include "gl_es2_aex_func_map.c"
#endif

#ifndef GL_ES2_CONTEXT_H_INCLUDED
#define GL_ES2_CONTEXT_H_INCLUDED
#include "gl_es2_context.h"
#endif

#ifndef SL_IR_H_INCLUDED
#define SL_IR_H_INCLUDED
#include "sl_ir.h"
#endif

#ifndef IR_H_INCLUDED
#define IR_H_INCLUDED
#include "ir.h"
#endif

#ifndef SOURCE_GEN_H_INCLUDED
#define SOURCE_GEN_H_INCLUDED
#include "source_gen.h"
#endif

#define DEBUG_OUTPUT 1


int print_shader_log(FILE *fp, GLuint shader);
int print_program_log(FILE *fp, GLuint program);

int slirtest1(void) {
  int r = -1;

  int output_width = 1920, output_height = 1080;

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
      goto test_exit;
    }
  }

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

  const char *fsrc =
    "precision highp float;\n"
    "varying vec4 vertex_color;\n"
    "void main() {\n"
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
      goto test_exit;
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
      goto test_exit;
    }
  }

  glUseProgram(program);

  struct gl_es2_context *c = gl_es2_ctx_dont_lock();

  uintptr_t prog_name = (uintptr_t)program;
  struct gl_es2_program *prog = NULL;
  prog = (struct gl_es2_program *)not_find(&c->program_not_, prog_name);
  if (!prog) {
    goto test_exit;
  }

  struct ireg_registry ireg;
  ireg_init(&ireg);
  sl_ir_register_instructions(&ireg);

  struct ir_body body;
  ir_body_init(&body);

  struct ir_block *start = ir_body_alloc_block(&body);
  struct ir_block *blk;
  struct ir_temp *tmp = ir_body_alloc_temp_unused_virtual(&body);
  struct sl_execution_frame frame = {0};
  struct sl_compilation_unit *cu = &prog->fragment_shader_.shader_->shader_.cu_;
  struct sl_function *f = sl_compilation_unit_find_function(cu, "main");
  
  frame.f_ = f;
  frame.local_float_offset_ = (int)cu->global_frame_.ract_.rra_floats_.watermark_;
  frame.local_int_offset_ = (int)cu->global_frame_.ract_.rra_ints_.watermark_;
  frame.local_bool_offset_ = (int)cu->global_frame_.ract_.rra_bools_.watermark_;
  frame.local_sampler2D_offset_ = (int)cu->global_frame_.ract_.rra_sampler2D_.watermark_;
  frame.local_samplerCube_offset_ = (int)cu->global_frame_.ract_.rra_samplerCube_.watermark_;

  blk = sl_ir_stmt(start, tmp, &frame, f->body_);
  
  struct source_gen sg;
  sg_init(&sg);
  sg.fp_ = stdout;

  ir_print_body(&sg, &ireg, &body);

  sg_finish(&sg);

  ir_body_cleanup(&body);

  r = 0;
test_exit:

  return r;
}
