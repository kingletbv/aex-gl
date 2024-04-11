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
#ifndef SL_UNIFORMS_H
#define SL_UNIFORMS_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef SL_REG_ALLOC_H_INCLUDED
#define SL_REG_ALLOC_H_INCLUDED
#include "sl_reg_alloc.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct sl_variable;

struct sl_uniform {
  /* Tail cyclic chain inside sl_uniform_table::uniforms_. */
  struct sl_uniform *chain_;

  /* Vertex and/or fragment variables, either may be NULL, but not both. */
  struct sl_variable *vertex_variable_, *fragment_variable_;
  
  /* Memory used to represent the data for this uniform; depends on the variable; 
   * this is a variable length array, the sl_uniform as a whole is allocated 
   * oversized to accommodate it. */
  uint64_t slab_[1];
};

struct sl_uniform_table {
  struct sl_uniform *uniforms_;
};

void sl_uniform_table_init(struct sl_uniform_table *ut);
void sl_uniform_table_cleanup(struct sl_uniform_table *ut);

int sl_uniform_table_add_uniform(struct sl_uniform_table *ut, struct sl_uniform **pp_uniform, struct sl_variable *vertex_side, struct sl_variable *fragment_side);

/* Maximum possible length of a uniform location name, including a null terminator. */
int sl_uniform_table_max_name_length(struct sl_uniform_table *ut, size_t *pmax_name_length);

int sl_uniform_load_ra_for_execution(struct sl_execution *exec, void *base_mem, size_t offset, size_t *pnum_slab_bytes_consumed, struct sl_reg_alloc *ra);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_UNIFORMS_H */
