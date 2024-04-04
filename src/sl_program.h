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
#ifndef SL_PROGRAM_H
#define SL_PROGRAM_H

#ifndef PRIMITIVE_ASSEMBLY_H_INCLUDED
#define PRIMITIVE_ASSEMBLY_H_INCLUDED
#include "primitive_assembly.h"
#endif

#ifndef SL_INFO_LOG_H_INCLUDED
#define SL_INFO_LOG_H_INCLUDED
#include "sl_info_log.h"
#endif

#ifndef ATTRIB_BINDING_TABLE_H_INCLUDED
#define ATTRIB_BINDING_TABLE_H_INCLUDED
#include "attrib_binding_table.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct sl_shader;

struct sl_program {
  struct primitive_assembly pa_;

  struct sl_shader *vertex_shader_;
  struct sl_program *next_program_using_vertex_shader_, *prev_program_using_vertex_shader_;

  struct sl_shader *fragment_shader_;
  struct sl_program *next_program_using_fragment_shader_, *prev_program_using_fragment_shader_;

  struct attrib_binding_table abt_;

  struct sl_info_log log_;

};

void sl_program_init(struct sl_program *prog);
void sl_program_cleanup(struct sl_program *prog);

void sl_program_attach_shader(struct sl_program *prog, struct sl_shader *sh);
void sl_program_detach_shader(struct sl_program *prog, struct sl_shader *sh);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_PROGRAM_H */
