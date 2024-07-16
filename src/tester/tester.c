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

/* xx(name, description) */
#define CODE_TESTS \
  xx(codetest1, "Clipping test") \
  xx(slirtest1, "SL IR test")

struct shader_test_code {
  const char *code_;
  const char *expected_dump_;
} shader_tests[] = {
  /* 0 */
  { "void main(void) {\n"
    "  dump(vec3(1,2,3));\n"
    "}\n",
    "vec3(1.000000, 2.000000, 3.000000)\n"
  },

  /* 1 */
  { "void main(void) {\n"
    "  dump(1+1);\n"
    "}",
    "2\n"
  },

  /* 2 */
  { "void dump_arg(int arg) {\n"
    "  dump(arg);\n"
    "}\n"
    "void main(void) {\n"
    "  dump_arg(1+1);\n"
    "}\n",
    "2\n"
  },

  /* 3 */
  { "void main(void) {\n"
    "  ivec4 v = ivec4(1,2,3,4);\n"
    "  dump(v.z);\n"
    "}\n",
    "3\n"
  },

  /* 4 */
  { "void main(void) {\n"
    "  ivec4 v = ivec4(1,2,3,4);\n"
    "  dump((v.zw).y);\n"
    "}\n",
    "4\n"
  },

  /* 5 */
  { "void main(void) {\n"
    "  ivec4 v = ivec4(1,2,3,4);\n"
    "  dump(v[2]);\n"
    "}\n",
    "3\n"
  },

  /* 6 */
  { "void main(void) {\n"
    "  ivec4 v = ivec4(0, 1, 2, 3);\n"
    "  dump(v.wzyx[0]);\n"
    "  dump(v.wzyx[1]);\n"
    "  dump(v.wzyx[2]);\n"
    "  dump(v.wzyx[3]);\n"
    "}\n",
    "3\n"
    "2\n"
    "1\n"
    "0\n"
  },

  /* 7 */
  { "void main(void) {\n"
    "  ivec4 v = ivec4(0, 1, 2, 3);\n"
    "  v.xyzw = v.wzyx;\n"
    "  dump(v.wzyx[0]);\n"
    "  dump(v.wzyx[1]);\n"
    "  dump(v.wzyx[2]);\n"
    "  dump(v.wzyx[3]);\n"
    "}\n",
    "0\n"
    "1\n"
    "2\n"
    "3\n"
  },

  /* 8 */
  { "void main(void) {\n"
    "  ivec4 v = ivec4(0, 1, 2, 3);\n"
    "  v.xyzw = v.wzyx;\n"
    "  v.wzyx[2] = 6; // note that we're putting it, after swizzling, in y. However, that's an lvalue, so it should change the 2nd (#1) component, not the 3rd (#2) component.\n"
    "  dump(v);\n"
    "}\n",
    "ivec4(3, 6, 1, 0)\n"
  },

  /* 9 */
  { "void main(void) {\n"
    "  /* 0 2   \n"
    "   * 1 3 */\n"
    "  mat2 m = mat2(0, 1, 2, 3);\n"
    "  dump(m[0]);\n"
    "  dump(m[1]);\n"
    "}\n",
    "vec2(0.000000, 1.000000)\n"
    "vec2(2.000000, 3.000000)\n"
  },

  /* 10 */
  { "void main(void) {\n"
    "  /* 0 3 6 \n"
    "   * 1 4 7 \n"
    "   * 2 5 8*/\n"
    "  mat3 m = mat3(0, 1, 2, 3, 4, 5, 6, 7, 8);\n"
    "  dump(m[0]);\n"
    "  dump(m[1]);\n"
    "}\n",
    "vec3(0.000000, 1.000000, 2.000000)\n"
    "vec3(3.000000, 4.000000, 5.000000)\n"
  }
};

#define xx(name, desc) int name(void);
CODE_TESTS
#undef xx

int run_codetests(void) {
  int num_tests_success = 0;
  int num_tests_total = 0;
  int rv;
#define xx(name, desc) num_tests_total++;
  CODE_TESTS
#undef xx
#define xx(name, desc) rv = name(); if (rv) fprintf(stderr, "Failed test " #name " with return value %d\n", rv); else num_tests_success++;
  CODE_TESTS
#undef xx
  fprintf(stdout, "%d/%d tests ran successfully\n", num_tests_success, num_tests_total);
  return (num_tests_success != num_tests_total);
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

  int rv;
  rv = run_codetests();
  if (rv) {
    return EXIT_FAILURE;
  }

  size_t first_selected_test = 0;
  size_t end_selected_test = sizeof(shader_tests) / sizeof(*shader_tests);

  //end_selected_test = 1 + (first_selected_test = 0);

  size_t n;
  for (n = first_selected_test; n < end_selected_test; ++n) {
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

  fprintf(stdout, "%d / %zu tests passed\n", num_successes, (end_selected_test - first_selected_test));
  
  return (num_successes == (int)(end_selected_test - first_selected_test)) ? EXIT_SUCCESS : EXIT_FAILURE;
}
