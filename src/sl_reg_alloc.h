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

#ifndef REF_RANGE_ALLOCATOR_H_INCLUDED
#define REF_RANGE_ALLOCATOR_H_INCLUDED
#include "ref_range_allocator.h"
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

  /* If non-zero, this register allocation is for a register on a local
   * frame. If zero, this register allocation is for a register on the global
   * frame. */
  int local_frame_:1;

  /* If non-zero, register indices in regs_ refer to integer registers 
   * that hold the actual register. If kind_ is slrak_array, or slrak_struct,
   * this field has no meaning (and the nested sl_reg_alloc's determine
   * if a field is indirect.) */
  int is_indirect_:1;

  union {
    int regs_[16];
    struct sl_reg_alloc_compound comp_;   /* struct */
    struct sl_reg_alloc_array array_;
  } v_;
};

struct sl_reg_allocator {
  int local_frame_:1;
  struct ref_range_allocator rra_floats_;
  struct ref_range_allocator rra_ints_;
  struct ref_range_allocator rra_bools_;
  struct ref_range_allocator rra_sampler2D_;
  struct ref_range_allocator rra_samplerCube_;
};

void sl_reg_alloc_init(struct sl_reg_alloc *ra);
int sl_reg_alloc_set_type(struct sl_reg_alloc *ra, const struct sl_type *t, int local_frame);
void sl_reg_alloc_void(struct sl_reg_alloc *ra); /* set type to void */
void sl_reg_alloc_cleanup(struct sl_reg_alloc *ra);
int sl_reg_alloc_is_allocated(const struct sl_reg_alloc *ra);
int sl_reg_alloc_clone(struct sl_reg_alloc *dst, const struct sl_reg_alloc *src);
int sl_reg_alloc_are_equal(const struct sl_reg_alloc *a, const struct sl_reg_alloc *b);

void sl_reg_allocator_init(struct sl_reg_allocator *ra);
void sl_reg_allocator_cleanup(struct sl_reg_allocator *ra);

int sl_reg_allocator_lock_reg_range(struct sl_reg_allocator *ract, sl_reg_category_t cat, int head_reg, int array_quantity);
int sl_reg_allocator_unlock_reg_range(struct sl_reg_allocator *ract, sl_reg_category_t cat, int head_reg, int array_quantity);
int sl_reg_allocator_alloc_reg_range(struct sl_reg_allocator *ract, sl_reg_category_t cat, int array_quantity, int *result);

int sl_reg_allocator_lock_or_alloc_descend(struct sl_reg_allocator *ract, int array_quantity, struct sl_reg_alloc *ra);
int sl_reg_allocator_alloc_descend(struct sl_reg_allocator *ract, int array_quantity, struct sl_reg_alloc *ra);
int sl_reg_allocator_lock_descend(struct sl_reg_allocator *ract, int array_quantity, struct sl_reg_alloc *ra);
int sl_reg_allocator_unlock_descend(struct sl_reg_allocator *ract, int array_quantity, struct sl_reg_alloc *ra);

int sl_reg_allocator_alloc(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra);
int sl_reg_allocator_lock(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra);
int sl_reg_allocator_unlock(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra);
int sl_reg_allocator_lock_or_alloc(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra);

/* Suppose we take all individual registers from rvalue, and assign them to the registers in lvalue,
 * in chronological order, would there be a register in lvalue that appears at a later point in rvalue?
 * If so, the value in rvalue would be overwritten by the time we need to assign it in lvalue.
 * For instance: "v.xyzw = v.wzyx;" - here the value of .x is overwritten before it can be assigned
 * to the .w component.
 * If such an overwrite conflict would occur, 1 is returned, if the chronological assignment is safe, 0 is
 * returned. */
int sl_reg_check_overlapping_assignment(const struct sl_reg_alloc *lvalue, const struct sl_reg_alloc *rvalue);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_REG_ALLOC_H */
