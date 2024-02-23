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

#ifndef SL_REG_ALLOC_H
#define SL_REG_ALLOC_H

#ifndef RANGE_ALLOCATOR_H_INCLUDED
#define RANGE_ALLOCATOR_H_INCLUDED
#include "range_allocator.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Register value if not yet allocated (which is the default after initialization) */
#define SL_REG_NONE -1

typedef enum sl_reg_category {
  slrc_invalid,
  slrc_float,
  slrc_int,
  slrc_bool,
  slrc_sampler2D,
  slrc_samplerCube
} sl_reg_category_t;

typedef enum sl_reg_alloc_kind {
  slrak_void,
  slrak_array,
  slrak_struct,
  slrak_float,
  slrak_int,
  slrak_bool,
  slrak_vec2,
  slrak_vec3,
  slrak_vec4,
  slrak_bvec2,
  slrak_bvec3,
  slrak_bvec4,
  slrak_ivec2,
  slrak_ivec3,
  slrak_ivec4,
  slrak_mat2,
  slrak_mat3,
  slrak_mat4,
  slrak_sampler2D,
  slrak_samplerCube
} sl_reg_alloc_kind_t;

struct sl_reg_alloc_compound {
  /* The sltk_struct type that this compound describes. */
  struct sl_type *struct_type_;

  /* For sletk_struct, the sequential index of each field corresponds to
   * each element */
  size_t num_fields_;
  struct sl_reg_alloc *fields_;
};

struct sl_reg_alloc_array {
  /* The sltk_array type that this array describes */
  struct sl_type *element_type_;

  /* The number of elements in the array */
  size_t num_elements_;

  /* The first element of the array, all fields are laid out consecutively */
  struct sl_reg_alloc *head_;
};

struct sl_reg_alloc {
  sl_reg_alloc_kind_t kind_;

  union {
    int regs_[16];
    struct sl_reg_alloc_compound comp_;   /* struct */
    struct sl_reg_alloc_array array_;
  } v_;
};

struct sl_reg_allocator {
  struct ral_range_allocator ral_floats_;
  struct ral_range_allocator ral_ints_;
  struct ral_range_allocator ral_bools_;
  struct ral_range_allocator ral_sampler2D_;
  struct ral_range_allocator ral_samplerCube_;

  size_t num_free_float_regs_;
  size_t num_free_float_regs_allocated_;
  int *free_float_regs_;
  int current_max_float_reg_;

  size_t num_free_int_regs_;
  size_t num_free_int_regs_allocated_;
  int *free_int_regs_;
  int current_max_int_reg_;

  size_t num_free_bool_regs_;
  size_t num_free_bool_regs_allocated_;
  int *free_bool_regs_;
  int current_max_bool_reg_;

  size_t num_free_sampler2D_regs_;
  size_t num_free_sampler2D_regs_allocated_;
  int *free_sampler2D_regs_;
  int current_max_sampler2D_reg_;

  size_t num_free_samplerCube_regs_;
  size_t num_free_samplerCube_regs_allocated_;
  int *free_samplerCube_regs_;
  int current_max_samplerCube_reg_;
};

void sl_reg_alloc_init(struct sl_reg_alloc *ra);
int sl_reg_alloc_set_type(struct sl_reg_alloc *ra, const struct sl_type *t);
void sl_reg_alloc_cleanup(struct sl_reg_alloc *ra);
int sl_reg_alloc_is_allocated(const struct sl_reg_alloc *ra);
int sl_reg_alloc_clone(struct sl_reg_alloc *dst, const struct sl_reg_alloc *src);

void sl_reg_allocator_init(struct sl_reg_allocator *ra);
void sl_reg_allocator_cleanup(struct sl_reg_allocator *ra);

int sl_reg_allocator_alloc_int_reg(struct sl_reg_allocator *ra);
int sl_reg_allocator_release_int_reg(struct sl_reg_allocator *ra, int reg);
int sl_reg_allocator_alloc_float_reg(struct sl_reg_allocator *ra);
int sl_reg_allocator_release_float_reg(struct sl_reg_allocator *ra, int reg);
int sl_reg_allocator_alloc_bool_reg(struct sl_reg_allocator *ra);
int sl_reg_allocator_release_bool_reg(struct sl_reg_allocator *ra, int reg);
int sl_reg_allocator_alloc_sampler2D_reg(struct sl_reg_allocator *ra);
int sl_reg_allocator_release_sampler2D_reg(struct sl_reg_allocator *ra, int reg);
int sl_reg_allocator_alloc_samplerCube_reg(struct sl_reg_allocator *ra);
int sl_reg_allocator_release_samplerCube_reg(struct sl_reg_allocator *ra, int reg);

int sl_reg_allocator_alloc(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra);
int sl_reg_allocator_lock(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra);
int sl_reg_allocator_unlock(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_REG_ALLOC_H */
