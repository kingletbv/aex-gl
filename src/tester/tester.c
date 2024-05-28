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

struct shader_test_code {
  const char *code_;
  const char *expected_dump_;
} shader_tests[] = {
  { "void main(void) {\n"
    "  dump(vec3(1,2,3));\n"
    "}\n",
    "vec3(1.000000, 2.000000, 3.000000)\n3"
  }
};

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


int main(int argc, char **argv) {
  int num_successes = 0;
  int total_num = (int)(sizeof(shader_tests) / sizeof(*shader_tests));

  size_t n;
  for (n = 0; n < sizeof(shader_tests) / sizeof(*shader_tests); ++n) {
    struct shader_test_code *stc = shader_tests + n;
    int sh = glCreateShader(AEX_GL_DEBUG_SHADER);
    check_for_and_print_gl_err(stderr);
    glShaderSource(sh, 1, &stc->code_, NULL);
    check_for_and_print_gl_err(stderr);
    glCompileShader(sh);
    check_for_and_print_gl_err(stderr);

    GLint fcompile_status = 0;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &fcompile_status);

    if (fcompile_status == GL_FALSE) {
      fprintf(stderr, "Failed to compile shader. Diagnostics:\n");
      if (print_shader_log(stderr, sh)) {
        /* Failure printing log */
        return -1;
      }
    }

    int prog = glCreateProgram();
    check_for_and_print_gl_err(stderr);
    glAttachShader(prog, sh);
    check_for_and_print_gl_err(stderr);
    glLinkProgram(prog);
    check_for_and_print_gl_err(stderr);

    GLint plink_status = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &plink_status);

    if (plink_status == GL_FALSE) {
      fprintf(stderr, "Failed to link program. Diagnostics:\n");
      if (print_program_log(stderr, prog)) {
        /* Failure printing log */
        return -1;
      }
    }
    else {
      aex_gl_es2_run_debug_shader(prog);
      if (!check_for_and_print_gl_err(stderr)) {
        const char *dump_output = aex_gl_es2_get_shader_debug_string(sh);
        if (strcmp(dump_output, stc->expected_dump_)) {
          fprintf(stderr, "Failed test #%zu\nExpected output:\n%sActual output:\n%s\n", n, stc->expected_dump_, dump_output);
        }
        else {
          num_successes++;
        }
      }
      else {
        fprintf(stderr, "Failed test #%zu\n", n);
      }
    }

    glDeleteProgram(prog);
    check_for_and_print_gl_err(stderr);

    glDeleteShader(sh);
    check_for_and_print_gl_err(stderr);
  }

  fprintf(stdout, "%d / %d tests passed\n", num_successes, total_num);
  
  return (num_successes == total_num) ? EXIT_SUCCESS : EXIT_FAILURE;
}
