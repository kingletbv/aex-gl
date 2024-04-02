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

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef SL_DEFS_H_INCLUDED
#define SL_DEFS_H_INCLUDED
#include "sl_defs.h"
#endif

#ifndef SL_SHADER_H_INCLUDED
#define SL_SHADER_H_INCLUDED
#include "sl_shader.h"
#endif

#ifndef SL_COMPILATION_UNIT_H_INCLUDED
#define SL_COMPILATION_UNIT_H_INCLUDED
#include "sl_compilation_unit.h"
#endif

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
#endif

#ifndef SL_PROGRAM_H_INCLUDED
#define SL_PROGRAM_H_INCLUDED
#include "sl_program.h"
#endif

#ifndef GLSL_ES1_COMPILER_H_INCLUDED
#define GLSL_ES1_COMPILER_H_INCLUDED
#include "glsl_es1_compiler.h"
#endif

void sl_shader_init(struct sl_shader *sh) {
  sh->type_ = SLST_INVALID_SHADER;
  sh->source_length_ = 0;
  sh->source_ = NULL;
  sl_compilation_unit_init(&sh->cu_);
  sl_exec_init(&sh->exec_);

  sh->programs_ = NULL;
  
  sh->gl_shader_object_name_ = 0; /* no name assigned yet */
  sh->gl_delete_status_ = 0;
  sh->gl_last_compile_status_ = 0;
  sl_info_log_init(&sh->gl_info_log_);
}

void sl_shader_cleanup(struct sl_shader *sh) {
  sl_exec_cleanup(&sh->exec_);
  sl_compilation_unit_cleanup(&sh->cu_);
  if (sh->source_) free(sh->source_);
  sl_info_log_cleanup(&sh->gl_info_log_);
  while (sh->programs_) {
    sl_program_detach_shader(sh->programs_, sh);
  }
}

void sl_shader_set_type(struct sl_shader *sh, enum sl_shader_type typ) {
  sl_shader_cleanup(sh);
  sl_shader_init(sh);
  sh->type_ = typ;
}

int sl_shader_set_source(struct sl_shader *sh, size_t num_strings, const char **string_ptrs, const int *lengths) {
  size_t total_length = 0;
  size_t n;
  for (n = 0; n < num_strings; ++n) {
    if (lengths[n] < 0) return -2;
    size_t new_length = total_length + (size_t)lengths[n];
    if (new_length < total_length) {
      /* overflow */
      return SL_ERR_OVERFLOW;
    }
    if (lengths[n] && !string_ptrs[n]) {
      /* invalid argument */
      return SL_ERR_INVALID_ARG;
    }
    total_length = new_length;
  }
  size_t bufsize = total_length + 1;
  if (bufsize < total_length) return SL_ERR_OVERFLOW;

  char *buf = (char *)malloc(bufsize);
  if (!buf) return SL_ERR_NO_MEM;
  size_t at = 0;
  for (n = 0; n < num_strings; ++n) {
    /* If we get this far we can assume the buffers are good */
    memcpy(buf + at, string_ptrs[n], (size_t)lengths[n]);
    at += (size_t)lengths[n];
  }
  buf[at++] = '\0';
  assert(at == bufsize);
  if (sh->source_) free(sh->source_);
  sh->source_ = buf;
  sh->source_length_ = total_length;
  return 0;
}


int sl_shader_compile(struct sl_shader *sh) {
  int r = SL_ERR_OK;
  struct glsl_es1_compiler cc;
  glsl_es1_compiler_init(&cc);

  /* Overwrite default dx in favor of info log dx */
  cc.dx_ = &sh->gl_info_log_.dx_;

  /* Have glsl_es1_compiler use sl_shader's compilation-unit instead of creating its own */
  cc.cu_ = &sh->cu_;

  enum glsl_es1_compiler_result cr;
  if (sh->type_ == SLST_INVALID_SHADER) {
    r = SL_ERR_INVALID_ARG;
    goto cleanup;
  }
  
  /* Declare built-in constants for GLSL v1 shaders */
  /* NOTE: Below are the minimums, see page 61 of GLSL ES Specification 1.00  -- we want to upgrade this to something more sensible. */
  cr = glsl_es1_compile_builtin_prologue(&cc,
    "const mediump int gl_MaxVertexAttribs = 8;\n"
    "const mediump int gl_MaxVertexUniformVectors = 128;\n"
    "const mediump int gl_MaxVaryingVectors = 8;\n"
    "const mediump int gl_MaxVertexTextureImageUnits = 0;\n"
    "const mediump int gl_MaxCombinedTextureImageUnits = 8;\n"
    "const mediump int gl_MaxTextureImageUnits = 8;\n"
    "const mediump int gl_MaxFragmentUniformVectors = 16;\n"
    "const mediump int gl_MaxDrawBuffers = 1;\n");
  if (cr != GLSL_ES1_R_SUCCESS) {
    r = SL_ERR_INTERNAL;
    goto cleanup;
  }

  /* Built-in uniform state */
  cr = glsl_es1_compile_builtin_prologue(&cc,
    "/* Depth range in window coordinates */\n"
    "struct gl_DepthRangeParameters {\n"
    "  highp float near;  // n\n"
    "  highp float far;   // f\n"
    "  highp float diff;  // f - n\n"
    "};"
    "uniform gl_DepthRangeParameters gl_DepthRange;\n");
  if (cr != GLSL_ES1_R_SUCCESS) {
    r = SL_ERR_INTERNAL;
    goto cleanup;
  }

  if (sh->type_ == SLST_VERTEX_SHADER) {
    /* No built-in variables for the vertex shader */
  }
  else if (sh->type_ == SLST_FRAGMENT_SHADER) {
    /* Declare builtins for the GLSL v1 fragment shader */
    cr = glsl_es1_compile_builtin_prologue(&cc, 
      "mediump vec4 gl_FragCoord;\n"
      "        bool gl_FrontFacing;\n"
      "mediump vec4 gl_FragColor;\n"
      "mediump vec4 gl_FragData[gl_MaxDrawBuffers];\n"
      "mediump vec2 gl_PointCoord;\n");

    if (cr != GLSL_ES1_R_SUCCESS) {
      r = SL_ERR_INTERNAL;
      goto cleanup;
    }
  }

  const char *filename = (sh->type_ == SLST_FRAGMENT_SHADER) ? "fragment-shader" : "vertex-shader";
  cr = glsl_es1_compiler_compile_mem(&cc, filename, sh->source_, sh->source_length_);
  if (cr != GLSL_ES1_R_SUCCESS) {
    sh->gl_last_compile_status_ = 0;
    switch (cr) {
      case GLSL_ES1_R_HAVE_ERRORS:
        r = SL_ERR_HAD_ERRORS;
        break;
      case GLSL_ES1_R_FAILED:
      default:
        r = SL_ERR_INTERNAL;
        break;
    }
    goto cleanup;
  }
  else {
    sh->gl_last_compile_status_ = 1;
  }

cleanup:
  /* Set to NULL so the compilation-unit of the shader is not cleaned up */
  cc.cu_ = NULL;
  glsl_es1_compiler_cleanup(&cc);

  return r;
}
