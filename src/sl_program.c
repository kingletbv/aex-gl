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

#ifndef SL_PROGRAM_H_INCLUDED
#define SL_PROGRAM_H_INCLUDED
#include "sl_program.h"
#endif

#ifndef SL_SHADER_H_INCLUDED
#define SL_SHADER_H_INCLUDED
#include "sl_shader.h"
#endif

#ifndef SL_TYPES_H_INCLUDED
#define SL_TYPES_H_INCLUDED
#include "sl_types.h"
#endif

#ifndef REF_RANGE_ALLOCATOR_H_INCLUDED
#define REF_RANGE_ALLOCATOR_H_INCLUDED
#include "ref_range_allocator.h"
#endif

#ifndef SL_UNIFORMS_H_INCLUDED
#define SL_UNIFORMS_H_INCLUDED
#include "sl_uniforms.h"
#endif


void sl_program_init(struct sl_program *prog) {
  primitive_assembly_init(&prog->pa_);
  sl_info_log_init(&prog->log_);
  prog->next_program_using_vertex_shader_ = NULL;
  prog->prev_program_using_vertex_shader_ = NULL;
  prog->next_program_using_fragment_shader_ = NULL;
  prog->prev_program_using_fragment_shader_ = NULL;
  abt_init(&prog->abt_);
  sl_uniform_table_init(&prog->uniforms_);
}

void sl_program_cleanup(struct sl_program *prog) {
  sl_info_log_cleanup(&prog->log_);
  primitive_assembly_cleanup(&prog->pa_);
  sl_program_detach_shader(prog, prog->fragment_shader_);
  sl_program_detach_shader(prog, prog->vertex_shader_);
  abt_cleanup(&prog->abt_);
  sl_uniform_table_cleanup(&prog->uniforms_);
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

static int sl_program_num_attribs_for_var(struct sl_variable *v) {
  if (!v) return 0;
  struct sl_type *t = sl_type_unqualified(v->type_);
  int num_attribs_needed = 0;
  switch (t->kind_) {
    case sltk_float:
    case sltk_vec2:
    case sltk_vec3:
    case sltk_vec4:
      num_attribs_needed = 1;
      break;
    /* For matrices, each column is an attribute */
    case sltk_mat2:
      num_attribs_needed = 2;
      break;
    case sltk_mat3:
      num_attribs_needed = 3;
      break;
    case sltk_mat4:
      num_attribs_needed = 4;
      break;
    default:
      /* Cannot bind to attribute that's not float, vec2, vec3, vec4, mat2, mat3, mat4 (see GLSL v1 page 30 4.3.3. Attribute) */
      num_attribs_needed = 0;
      break;
  }
  return num_attribs_needed;
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

int sl_program_link(struct sl_program *prog) {
  if (!prog) return SL_ERR_INVALID_ARG;
  if (!prog->vertex_shader_) return SL_ERR_INVALID_ARG;
  if (!prog->vertex_shader_->gl_last_compile_status_) return SL_ERR_INVALID_ARG;

  struct ref_range_allocator rra;
  ref_range_allocator_init(&rra);

  /* First run through all manually bound attributes and lock their ranges */
  struct attrib_binding *ab;
  ab = prog->abt_.seq_;
  if (ab) {
    do {
      if (ab->bound_index_ != -1) {
        /* Have a bound index, lock that range and set it as the active index */
        struct sym *s = st_find(&prog->vertex_shader_->cu_.global_scope_, ab->name_, ab->name_len_);
        if (s && s->kind_ == SK_VARIABLE) {
          struct sl_variable *v;
          v = s->v_.variable_;
          assert(v);
          int qualifier = sl_type_qualifiers(v->type_);
          if (qualifier & SL_TYPE_QUALIFIER_ATTRIBUTE) {
            int num_attribs_needed = sl_program_num_attribs_for_var(v);
            if (!num_attribs_needed) {
              /* Cannot bind to this type, compiler should have issued an error and we 
               * shouldn't be getting this far.. */
              ref_range_allocator_cleanup(&rra);
              return SL_ERR_INTERNAL;
            }
            if (ab->bound_index_ < 0) {
              ref_range_allocator_cleanup(&rra);
              return SL_ERR_INVALID_ARG;
            }
            if (!ref_range_mark_range_allocated(&rra, (uintptr_t)ab->bound_index_, (uintptr_t)ab->bound_index_ + num_attribs_needed)) {
              /* Should always succeed unless it could not get the memory */
              ref_range_allocator_cleanup(&rra);
              return SL_ERR_NO_MEM;
            }
            ab->active_index_ = ab->bound_index_;
          }
        }
      }

      ab = ab->next_;
    } while (ab != prog->abt_.seq_);
  }

  /* Next, run through all attributes that have not yet been bound to any range, this is the
   * complement to the loop above, however, we cannot join loops as we don't want allocations
   * to overlap bound attributes */
  struct sl_variable *v = prog->vertex_shader_->cu_.global_frame_.variables_;
  if (v) {
    do {
      v = v->chain_;

      int qualifiers = sl_type_qualifiers(v->type_);
      if (qualifiers & SL_TYPE_QUALIFIER_ATTRIBUTE) {
        attrib_binding_table_result_t abtr;
        struct attrib_binding *ab = NULL;
        abtr = abt_find_or_insert(&prog->abt_, v->name_, strlen(v->name_), &ab);
        if (abtr == ABT_NOMEM) {
          ref_range_allocator_cleanup(&rra);
          return SL_ERR_NO_MEM;
        }
        assert(ab && "no other return code should lead to failure");
        if (ab->bound_index_ == -1) {
          /* No bound index, so we have to find one */
          int num_attribs_needed = sl_program_num_attribs_for_var(v);
          if (!num_attribs_needed) {
            /* invalid type for use as attribute, compiler should already have issued an error */
            return SL_ERR_INTERNAL;
          }
          uintptr_t base_of_range = 0;
          if (!ref_range_alloc(&rra, (uintptr_t)num_attribs_needed, &base_of_range)) {
            ref_range_allocator_cleanup(&rra);
            return SL_ERR_NO_MEM;
          }
          if (base_of_range > INT_MAX) {
            return SL_ERR_OVERFLOW;
          }
          ab->active_index_ = (int)base_of_range;
        }
        else {
          /* already locked in the loop above.. */
        }
      }
    } while (v != prog->vertex_shader_->cu_.global_frame_.variables_);
  }

  /* All attribute variables have a corresponding attrib_binding in prog->abt_, furthermore,
   * they all have an appropriately configured active_index_.
   * Note that there still may be attrib_bindings with no corresponding sl_variable, these
   * should be skipped (they are attributes bound with no source equivalent.) */

  /* Done with allocator for attribute indices, clean up */
  ref_range_allocator_cleanup(&rra);



  int r;
  r = primitive_assembly_init_fixed_columns(&prog->pa_);
  if (r) {
    return SL_ERR_NO_MEM;
  }
  
  return r;
}

int sl_program_load_uniform_for_execution(struct sl_program *prog, struct sl_uniform *u) {
  struct { struct sl_variable *v_;
           struct sl_shader *s_;
  } vs[] = {
    { u->vertex_variable_, prog->vertex_shader_ },
    { u->fragment_variable_, prog->fragment_shader_ }
  };
  size_t n;
  int r;
  for (n = 0; n < (sizeof(vs) / sizeof(*vs)); ++n) {
    size_t num_bytes_consumed;
    if (!vs[n].s_ || !vs[n].v_) continue;
    r = sl_uniform_load_ra_for_execution(&vs[n].s_->exec_, u->slab_, 0, &num_bytes_consumed, &vs[n].v_->reg_alloc_);
    if (r) return r;
  }
  return 0;
}

int sl_program_load_uniforms_for_execution(struct sl_program *prog) {
  int r;
  struct sl_uniform *u;
  u = prog->uniforms_.uniforms_;
  if (u) {
    do {
      u = u->chain_;

      r = sl_program_load_uniform_for_execution(prog, u);
      if (r) return r;

    } while (u != prog->uniforms_.uniforms_);
  }

  return 0;
}
