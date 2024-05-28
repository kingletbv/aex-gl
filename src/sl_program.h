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

#ifndef ATTRIB_ROUTING_H_INCLUDED
#define ATTRIB_ROUTING_H_INCLUDED
#include "attrib_routing.h"
#endif

#ifndef SL_UNIFORMS_H_INCLUDED
#define SL_UNIFORMS_H_INCLUDED
#include "sl_uniforms.h"
#endif

#ifndef CLIPPING_STAGE_H_INCLUDED
#define CLIPPING_STAGE_H_INCLUDED
#include "clipping_stage.h"
#endif

#ifndef FRAGMENT_BUFFER_H_INCLUDED
#define FRAGMENT_BUFFER_H_INCLUDED
#include "fragment_buffer.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct sl_shader;

struct sl_program {
  struct primitive_assembly pa_;
  struct clipping_stage cs_;
  struct fragment_buffer fragbuf_;

  struct sl_shader *vertex_shader_;
  struct sl_program *next_program_using_vertex_shader_, *prev_program_using_vertex_shader_;

  struct sl_shader *fragment_shader_;
  struct sl_program *next_program_using_fragment_shader_, *prev_program_using_fragment_shader_;

  struct sl_shader *debug_shader_;
  struct sl_program *next_program_using_debug_shader_, *prev_program_using_debug_shader_;

  struct attrib_binding_table abt_;
  struct attrib_routing ar_;

  struct sl_uniform_table uniforms_;

  int gl_last_link_status_:1;

  struct sl_info_log log_;

};

void sl_program_init(struct sl_program *prog);
void sl_program_cleanup(struct sl_program *prog);

void sl_program_attach_shader(struct sl_program *prog, struct sl_shader *sh);
void sl_program_detach_shader(struct sl_program *prog, struct sl_shader *sh);

int sl_program_load_uniform_for_execution(struct sl_program *prog, struct sl_uniform *u,
                                          size_t loading_table_size,
                                          void **sampler_2D_uniform_loading_table,
                                          void **sampler_Cube_uniform_loading_table);

int sl_program_load_uniforms_for_execution(struct sl_program *prog,
                                           size_t loading_table_size,
                                           void **sampler_2D_uniform_loading_table,
                                           void **sampler_Cube_uniform_loading_table);

int sl_program_link(struct sl_program *prog);

int sl_program_set_attrib_binding_index(struct sl_program *prog, const char *name, int index);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_PROGRAM_H */
