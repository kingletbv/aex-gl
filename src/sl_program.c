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
  sl_info_log_init(&prog->log_);
  primitive_assembly_init(&prog->pa_);
  clipping_stage_init(&prog->cs_);
  fragment_buffer_init(&prog->fragbuf_);

  prog->next_program_using_vertex_shader_ = NULL;
  prog->prev_program_using_vertex_shader_ = NULL;
  prog->vertex_shader_ = NULL;
  prog->next_program_using_fragment_shader_ = NULL;
  prog->prev_program_using_fragment_shader_ = NULL;
  prog->fragment_shader_ = NULL;
  abt_init(&prog->abt_);
  attrib_routing_init(&prog->ar_);
  sl_uniform_table_init(&prog->uniforms_);
  prog->gl_last_link_status_ = 0;
}

void sl_program_cleanup(struct sl_program *prog) {
  sl_info_log_cleanup(&prog->log_);
  primitive_assembly_cleanup(&prog->pa_);
  clipping_stage_cleanup(&prog->cs_);
  fragment_buffer_cleanup(&prog->fragbuf_);
  sl_program_detach_shader(prog, prog->fragment_shader_);
  sl_program_detach_shader(prog, prog->vertex_shader_);
  abt_cleanup(&prog->abt_);
  attrib_routing_cleanup(&prog->ar_);
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
    prog->vertex_shader_ = sh;
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
    prog->fragment_shader_ = sh;
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
  int r;

  prog->gl_last_link_status_ = 0;

  if (!prog) return SL_ERR_INVALID_ARG;
  int fail_invalid_arg = 0;
  if (!prog->vertex_shader_) {
    dx_error(&prog->log_.dx_, "Program has no vertex shader attached\n");
    fail_invalid_arg = 1;
  }
  if (!prog->vertex_shader_->gl_last_compile_status_) {
    dx_error(&prog->log_.dx_, "Program's vertex shader has not successfully been compiled\n");
    fail_invalid_arg = 1;
  }
  if (!prog->fragment_shader_) {
    dx_error(&prog->log_.dx_, "Program has no fragment shader attached\n");
    fail_invalid_arg = 1;
  }
  if (!prog->fragment_shader_->gl_last_compile_status_) {
    dx_error(&prog->log_.dx_, "Program's fragment shader has not successfully been compiled\n");
    fail_invalid_arg = 1;
  }
  if (fail_invalid_arg) {
    return SL_ERR_INVALID_ARG;
  }

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
            if (ref_range_mark_range_allocated(&rra, (uintptr_t)ab->bound_index_, (uintptr_t)ab->bound_index_ + num_attribs_needed)) {
              /* Should always succeed unless it could not get the memory */
              ref_range_allocator_cleanup(&rra);
              return SL_ERR_NO_MEM;
            }
            ab->active_index_ = ab->bound_index_;
            ab->var_ = v;
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
          if (ref_range_alloc(&rra, (uintptr_t)num_attribs_needed, &base_of_range)) {
            ref_range_allocator_cleanup(&rra);
            return SL_ERR_NO_MEM;
          }
          if (base_of_range > INT_MAX) {
            return SL_ERR_OVERFLOW;
          }
          ab->active_index_ = (int)base_of_range;
          ab->var_ = v;
        }
        else {
          /* already locked in the loop above.. */
        }
      }
    } while (v != prog->vertex_shader_->cu_.global_frame_.variables_);
  }

  /* Allocate and route varyings */
  struct sl_variable *fv = prog->fragment_shader_->cu_.global_frame_.variables_;
  if (fv) {
    do {
      fv = fv->chain_;

      int qualifiers = sl_type_qualifiers(fv->type_);
      if (qualifiers & SL_TYPE_QUALIFIER_VARYING) {
        /* Varying on the side of the fragment shader, if we can find a varying of the
         * same name (and type) on the side of the vertex shader, we can route. */
        struct sl_variable *vv = sl_compilation_unit_find_variable(&prog->vertex_shader_->cu_, fv->name_);
        if (vv) {
          int vv_qualifiers = sl_type_qualifiers(vv->type_);
          if (vv_qualifiers & SL_TYPE_QUALIFIER_VARYING) {
            /* Have matching varying on the vertex shader side, and it is also a varying, route. */
            if (!sl_are_variables_compatible(vv, fv)) {
              dx_error(&prog->log_.dx_, "Incompatible types for \"%s\" varyings", vv->name_);
              return SL_ERR_INVALID_ARG;
            }
            else {
              r = attrib_routing_add_variables(&prog->ar_, fv, vv);
              if (r) {
                dx_error(&prog->log_.dx_, "Failed adding \"%s\" varyings", vv->name_);
                return r;
              }
            }
          }
        }
      }
    } while (fv != prog->fragment_shader_->cu_.global_frame_.variables_);
  }

  /* We now allocate the various buffers that do not rely on the user-defined attributes. */
  if (primitive_assembly_alloc_buffers(&prog->pa_) ||
      clipping_stage_alloc_varyings(&prog->cs_, prog->ar_.num_attribs_routed_) ||
      fragment_buffer_alloc_buffers(&prog->fragbuf_)) {
    dx_no_memory(&prog->log_.dx_);
    return SL_ERR_NO_MEM;
  }

  /* All attribute variables have a corresponding attrib_binding in prog->abt_, furthermore,
   * they all have an appropriately configured active_index_.
   * Note that there still may be attrib_bindings with no corresponding sl_variable, these
   * should be skipped (they are attributes bound with no source equivalent.) */
  ab = prog->abt_.seq_;
  if (ab) {
    do {
      if (ab->var_) {
        struct sl_variable *v = ab->var_;
        switch (v->reg_alloc_.kind_) {
          case slrak_void:
          case slrak_array:
          case slrak_struct:
            /* Cannot handle these .. */
            dx_error(&prog->log_.dx_, "Cannot bind attribute variable \"%s\" due to incorrect type.", v->name_);
            return SL_ERR_INVALID_ARG;
          case slrak_float:
          case slrak_vec2:
          case slrak_vec3:
          case slrak_vec4:
          case slrak_mat2:
          case slrak_mat3:
          case slrak_mat4: {
            size_t num_attribs = 1;
            size_t num_components = 1;
            switch (v->reg_alloc_.kind_) {
              case slrak_float:
                num_attribs = 1;
                num_components = 1;
                break;
              case slrak_vec2:
                num_attribs = 1;
                num_components = 2;
                break;
              case slrak_vec3:
                num_attribs = 1;
                num_components = 3;
                break;
              case slrak_vec4:
                num_attribs = 1;
                num_components = 4;
                break;
              case slrak_mat2:
                num_attribs = 2;
                num_components = 2;
                break;
              case slrak_mat3:
                num_attribs = 3;
                num_components = 3;
                break;
              case slrak_mat4:
                num_attribs = 4;
                num_components = 4;
                break;
            }
            size_t attrib_index;
            for (attrib_index = 0; attrib_index < num_attribs; ++attrib_index) {
              size_t component_index;
              for (component_index = 0; component_index < num_components; ++component_index) {
                int column_index;
                /* We're adding a column for primitive assembly to reflect fetching this attribute. Note however, that this
                 * data will flow straight into the register (in sl_execution) and does not pass through the buffers inside
                 * the primitive_assembly, those buffers are fixed and were already allocated before at 
                 * primitive_assembly_alloc_buffers() above. */
                column_index = primitive_assembly_add_column(&prog->pa_, PACT_ATTRIBUTE, PADT_FLOAT, (int)(ab->active_index_ + attrib_index), (int)component_index, v->reg_alloc_.v_.regs_[component_index + num_components * attrib_index]);
                if (column_index < 0) {
                  dx_no_memory(&prog->log_.dx_);
                }
              }
            }
            break;
          }
          case slrak_int:
          case slrak_ivec2:
          case slrak_ivec3:
          case slrak_ivec4:
            dx_error(&prog->log_.dx_, "Integer types in primitive assembly for attribute binding not supported");
            break;
          case slrak_bool:
          case slrak_bvec2:
          case slrak_bvec3:
          case slrak_bvec4:
            dx_error(&prog->log_.dx_, "Boolean types in primitive assembly for attribute binding not supported");
            break;
        }
      }

      ab = ab->next_;
    } while (ab != prog->abt_.seq_);
  }

  /* Done with allocator for attribute indices, clean up */
  ref_range_allocator_cleanup(&rra);

  prog->gl_last_link_status_ = 1;

  return 0;
}

int sl_program_load_uniform_for_execution(struct sl_program *prog, struct sl_uniform *u,
                                          size_t loading_table_size,
                                          void **sampler_2D_uniform_loading_table,
                                          void **sampler_Cube_uniform_loading_table) {
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
    r = sl_uniform_load_ra_for_execution(&vs[n].s_->exec_, u->slab_, 0, &num_bytes_consumed, &vs[n].v_->reg_alloc_,
                                         loading_table_size, sampler_2D_uniform_loading_table, sampler_Cube_uniform_loading_table);
    if (r) return r;
  }
  return 0;
}

int sl_program_load_uniforms_for_execution(struct sl_program *prog,
                                           size_t loading_table_size,
                                           void **sampler_2D_uniform_loading_table,
                                           void **sampler_Cube_uniform_loading_table) {
  int r;
  struct sl_uniform *u;
  u = prog->uniforms_.uniforms_;
  if (u) {
    do {
      u = u->chain_;

      r = sl_program_load_uniform_for_execution(prog, u, loading_table_size, sampler_2D_uniform_loading_table, sampler_Cube_uniform_loading_table);
      if (r) return r;

    } while (u != prog->uniforms_.uniforms_);
  }

  return 0;
}

int sl_program_set_attrib_binding_index(struct sl_program *prog, const char *name, int index) {
  attrib_binding_table_result_t abtr;
  struct attrib_binding *ab;
  abtr = abt_find_or_insert(&prog->abt_, name, strlen(name), &ab);
  if (abtr == ABT_NOMEM) {
    dx_no_memory(&prog->log_.dx_);
    return SL_ERR_NO_MEM;
  }
  else if ((abtr != ABT_OK) && (abtr != ABT_DUPLICATE)) {
    assert(0 && "Not expecting any other return values");
    dx_error(&prog->log_.dx_, "unexpected internal error");
    return SL_ERR_INTERNAL;
  }
  ab->bound_index_ = index;

  return SL_ERR_OK;
}
