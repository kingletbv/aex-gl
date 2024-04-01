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
#ifndef SL_SHADER_H
#define SL_SHADER_H

#ifndef SL_COMPILATION_UNIT_H_INCLUDED
#define SL_COMPILATION_UNIT_H_INCLUDED
#include "sl_compilation_unit.h"
#endif

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
#endif

#ifndef SL_INFO_LOG_H_INCLUDED
#define SL_INFO_LOG_H_INCLUDED
#include "sl_info_log.h"
#endif

enum sl_shader_type {
  SLST_INVALID_SHADER,
  SLST_VERTEX_SHADER,
  SLST_FRAGMENT_SHADER
};

#ifdef __cplusplus
extern "C" {
#endif

struct sl_shader {
  /* Type of shader */
  enum sl_shader_type type_;

  /* Sourcecode for the shader */
  size_t source_length_;
  char *source_;

  /* The compilation unit for the shader; contains the AST and symbol tables */
  struct sl_compilation_unit cu_;

  /* The execution for the shader; holds the runtime execution state of the shader */
  struct sl_execution exec_;

  /* GL specific bits */
  uint32_t gl_shader_object_name_;
  int gl_delete_status_:1;
  int gl_last_compile_status_:1;

  /* Info log for shader compilation */
  struct sl_info_log gl_info_log_;

};

void sl_shader_init(struct sl_shader *sh);
void sl_shader_cleanup(struct sl_shader *sh);

void sl_shader_set_type(struct sl_shader *sh, enum sl_shader_type typ);
int sl_shader_set_source(struct sl_shader *sh, size_t num_strings, const char **string_ptrs, const int *lengths);

int sl_shader_compile(struct sl_shader *sh);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_SHADER_H */