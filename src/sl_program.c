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

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef SL_PROGRAM_H_INCLUDED
#define SL_PROGRAM_H_INCLUDED
#include "sl_program.h"
#endif

#ifndef SL_SHADER_H_INCLUDED
#define SL_SHADER_H_INCLUDED
#include "sl_shader.h"
#endif

void sl_program_init(struct sl_program *prog) {
  primitive_assembly_init(&prog->pa_);
  sl_info_log_init(&prog->log_);
  prog->next_program_using_vertex_shader_ = NULL;
  prog->prev_program_using_vertex_shader_ = NULL;
  prog->next_program_using_fragment_shader_ = NULL;
  prog->prev_program_using_fragment_shader_ = NULL;
}

void sl_program_cleanup(struct sl_program *prog) {
  sl_info_log_cleanup(&prog->log_);
  primitive_assembly_cleanup(&prog->pa_);
  sl_program_detach_shader(prog, prog->fragment_shader_);
  sl_program_detach_shader(prog, prog->vertex_shader_);
}

void sl_program_attach_shader(struct sl_program *prog, struct sl_shader *sh) {
  if (!sh) return;
  if (sh->type_ == SLST_VERTEX_SHADER) {
    if (prog->vertex_shader_) sl_program_detach_shader(prog, prog->vertex_shader_);
    if (sh->programs_) {
      prog->next_program_using_vertex_shader_ = sh->programs_;
      prog->prev_program_using_vertex_shader_ = sh->programs_->prev_program_using_vertex_shader_;
    }
    else {
      sh->programs_ = prog;
      prog->next_program_using_vertex_shader_ = prog->prev_program_using_vertex_shader_ = prog;
    }
  }
  else if (sh->type_ == SLST_FRAGMENT_SHADER) {
    if (prog->fragment_shader_) sl_program_detach_shader(prog, prog->fragment_shader_);
    if (sh->programs_) {
      prog->next_program_using_fragment_shader_ = sh->programs_;
      prog->prev_program_using_fragment_shader_ = sh->programs_->prev_program_using_fragment_shader_;
    }
    else {
      sh->programs_ = prog;
      prog->next_program_using_fragment_shader_ = prog->prev_program_using_fragment_shader_ = prog;
    }
  }
  else {
    assert(0 && "Unknown shader type, cannot attach to program.");
  }
}

void sl_program_detach_shader(struct sl_program *prog, struct sl_shader *sh) {
  if (!sh) return;
  if (sh == prog->vertex_shader_) {
    if (prog->next_program_using_vertex_shader_ == prog) {
      sh->programs_ = NULL;
    }
    else {
      if (sh->programs_ == prog) sh->programs_ = prog->next_program_using_vertex_shader_;
      prog->next_program_using_vertex_shader_->prev_program_using_vertex_shader_ = prog->prev_program_using_vertex_shader_;
      prog->prev_program_using_vertex_shader_->next_program_using_vertex_shader_ = prog->next_program_using_vertex_shader_;
    }
    prog->next_program_using_vertex_shader_ = prog->prev_program_using_vertex_shader_ = NULL;
    prog->vertex_shader_ = NULL;
  }
  else /* (sh == prog->fragment_shader_) */ {
    assert(sh == prog->fragment_shader_);
    if (prog->next_program_using_fragment_shader_ == prog) {
      sh->programs_ = NULL;
    }
    else {
      if (sh->programs_ == prog) sh->programs_ = prog->next_program_using_fragment_shader_;
      prog->next_program_using_fragment_shader_->prev_program_using_fragment_shader_ = prog->prev_program_using_fragment_shader_;
      prog->prev_program_using_fragment_shader_->next_program_using_fragment_shader_ = prog->next_program_using_fragment_shader_;
    }
    prog->next_program_using_fragment_shader_ = prog->prev_program_using_fragment_shader_ = NULL;
    prog->fragment_shader_ = NULL;
  }
}

