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

const char *shader_code[] = {
  "void main(void) {\n"
  "  dump(3.f);\n"
  "}\n"
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

  size_t n;
  for (n = 0; n < sizeof(shader_code) / sizeof(*shader_code); ++n) {
    int sh = glCreateShader(AEX_GL_DEBUG_SHADER);
    check_for_and_print_gl_err(stderr);
    glShaderSource(sh, 1, &shader_code[n], NULL);
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

    glDeleteProgram(prog);

    glDeleteShader(sh);
    check_for_and_print_gl_err(stderr);
  }
  
  

  return EXIT_SUCCESS;
}
