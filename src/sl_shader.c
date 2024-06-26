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

#ifndef INTTYPES_H_INCLUDED
#define INTTYPES_H_INCLUDED
#include <inttypes.h>
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

#ifndef SHA1_H_INCLUDED
#define SHA1_H_INCLUDED
#include "sha1.h"
#endif

void sl_shader_init(struct sl_shader *sh) {
  sh->type_ = SLST_INVALID_SHADER;
  sh->source_length_ = 0;
  sh->source_ = NULL;
  sh->hash_ = 0; /* filled in at compilation time */
  sl_type_base_init(&sh->tb_);
  sl_compilation_unit_init(&sh->cu_);
  sl_exec_init(&sh->exec_);

  sh->programs_ = NULL;
  
  sh->gl_shader_object_name_ = 0; /* no name assigned yet */
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
  sl_type_base_cleanup(&sh->tb_);
}

void sl_shader_set_type(struct sl_shader *sh, enum sl_shader_type typ) {
  sl_shader_cleanup(sh);
  sl_shader_init(sh);
  sh->type_ = typ;
}

int sl_shader_set_source(struct sl_shader *sh, size_t num_strings, const char * const * string_ptrs, const int *lengths) {
  size_t total_length = 0;
  size_t n;
  for (n = 0; n < num_strings; ++n) {
    size_t new_length;
    if (lengths) {
      if (lengths[n] && !string_ptrs[n]) {
        /* invalid argument */
        return SL_ERR_INVALID_ARG;
      }
      if (lengths[n] < 0) return -2;
      new_length = total_length + (size_t)lengths[n];
    }
    else {
      if (!string_ptrs[n]) {
        /* invalid argument */
        return SL_ERR_INVALID_ARG;
      }
      new_length = total_length + strlen(string_ptrs[n]);
    }
    if (new_length < total_length) {
      /* overflow */
      return SL_ERR_OVERFLOW;
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
    size_t len = 0;
    if (lengths) {
      len = (size_t)lengths[n];
    }
    else {
      len = strlen(string_ptrs[n]);
    }
    memcpy(buf + at, string_ptrs[n], len);
    at += len;
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
  cc.pp_.dx_ = cc.dx_;

  /* Overwrite type-base to be the one from the shader */
  cc.tb_ = &sh->tb_;

  /* Have glsl_es1_compiler use sl_shader's compilation-unit instead of creating its own */
  cc.cu_ = &sh->cu_;

  enum glsl_es1_compiler_result cr;
  if (sh->type_ == SLST_INVALID_SHADER) {
    dx_error(cc.dx_, "Shader type has not been specified\n");
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
    /* Declare builtins for the vertex shader */
    cr = glsl_es1_compile_builtin_prologue(&cc,
      "highp   vec4  gl_Position;\n"
      "mediump float gl_PointSize;\n");

    if (cr != GLSL_ES1_R_SUCCESS) {
      r = SL_ERR_INTERNAL;
      goto cleanup;
    }
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
  else if (sh->type_ == SLST_DEBUG_SHADER) {
    cr = glsl_es1_compile_builtin_prologue(&cc, "void dump(float f); "); /* trailing space matters!! otherwise compiler won't see final semicolon */

    if (cr != GLSL_ES1_R_SUCCESS) {
      r = SL_ERR_INTERNAL;
      goto cleanup;
    }
    /* Last declaration emited is our output function */
    assert(cc.cu_->global_scope_.seq_);
    assert(cc.cu_->global_scope_.seq_->prev_->kind_ == SK_FUNCTION);
    struct sl_function *f = cc.cu_->global_scope_.seq_->prev_->v_.function_;
    f->is_dump_fn_ = 1;
    f->builtin_runtime_fn_ = sl_exec_debug_dump_builtin;
  }

  /* All variables introduced this far should not be subject to initialization from its literal. */
  struct sl_variable *v = cc.current_frame_->variables_;
  if (v) {
    do {
      v = v->chain_;

      /* This flag will preserve register contents and prevent sl_exec_run() from initializing 
       * the variable from its literal (see sl_exec_initialize_globals). */
      v->is_externally_initialized_ = 1;
    } while (v != cc.current_frame_->variables_);
  }

  /* Develop a hash for the shader source and store in shader->hash_ */
  union {
    uint8_t sha1_digest[160/8];
    uint64_t tokens[3];
  } sha1_token;

  struct sha1_inner_state sha1;
  sha1_init(&sha1);
  sha1_process(&sha1, sh->source_, sh->source_length_);
  sha1_finish(&sha1, sha1_token.sha1_digest);
  sh->hash_ = sha1_token.tokens[0];

  const char *debug_file_extension = ".xunknown";
  const char *debug_file_prefix = "x";
  const char *filename = "";
  switch (sh->type_) {
    case SLST_FRAGMENT_SHADER:
      filename = "fragment-shader";
      debug_file_extension = ".frag";
      debug_file_prefix = "f";
      break;
    case SLST_VERTEX_SHADER:
      filename = "vertex-shader";
      debug_file_extension = ".vert";
      debug_file_prefix = "v";
      break;
    case SLST_DEBUG_SHADER:
      filename = "debug-shader";
      debug_file_extension = ".debug";
      debug_file_prefix = "d";
      break;
  };
  cr = glsl_es1_compiler_compile_mem(&cc, filename, sh->source_, sh->source_length_);
  const char *compilation_suffix = "";
  if (cr != GLSL_ES1_R_SUCCESS) {
    sh->gl_last_compile_status_ = 0;
    switch (cr) {
      case GLSL_ES1_R_HAVE_ERRORS:
        r = SL_ERR_HAD_ERRORS;
        compilation_suffix = "-e";
        break;
      case GLSL_ES1_R_FAILED:
      default:
        compilation_suffix = "-x";
        r = SL_ERR_INTERNAL;
        break;
    }
  }
  else {
    sh->gl_last_compile_status_ = 1;
  }


  if (0) {
    char s[150];
    sprintf(s, "C:\\temp\\aex-debug-shader\\%s%016" PRIX64 "%s%s", debug_file_prefix, sha1_token.tokens[0], compilation_suffix, debug_file_extension);
    FILE *fp = fopen(s, "wb");
    if (fp) {
      fwrite(sh->source_, 1, sh->source_length_, fp);
      fclose(fp);
    }
  }

cleanup:
  /* Set to NULL so the compilation-unit of the shader is not cleaned up */
  cc.cu_ = NULL;
  glsl_es1_compiler_cleanup(&cc);

  return r;
}
