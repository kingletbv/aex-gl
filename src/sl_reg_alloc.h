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

  /* Optional offset_ value, in number of registers. This contains an integer
   * register that holds the number of registers (offset) from the registers 
   * specified in v_ at which to load the actual value.
   * For instance, for expression:
   *   float f[10];
   *   f[3];
   * The offset_ will contain a register that holds the value '3', and v_
   * will hold the register corresponding to f[0].
   * This combines in non-trivial ways, e.g.:
   *   struct { float a_, b_[7]; } c[5];
   *   c[3].b_[2];
   *   c[3].a_;
   * For the expression "c[3].b_[2]" the offset_ is an integer register whose 
   * value evaluates to "3 * 7 + 2" (=23), and v_ will hold the register
   * corresponding to c[0].b_[0].
   * For the expression "c[3].a_" the offset_ is an integer whose value is "3",
   * and v_ will hold the register corresponding to c[0].a_.
   * In other words, the array layout into registers is by field, in a
   * "struct of arrays with registers" sort of way, and not an "array of structs with
   * registers" sort of way.
   */
  struct sl_reg_alloc *offset_;

  /* Optional rvalue_ value, matching the type and kind_ of this sl_reg_alloc.
   * The rvalue_ is set when both offset_ is non-NULL, and, the value is accessed
   * as an rvalue_ (it is read from as part of an expression.)
   * In this particular case, we need to have the value at reg[v_+offset_] into
   * a register of its own so the operator implementations do not have to deal 
   * with the complexity of loading registers at an offset.
   * For instance:
   *   float f[10];
   *   f[3] += 2.;
   * The expression f[3] is needed twice, once to load reg[f+3] into its own
   * register, so it can be added to the constant literal 2., and once to store
   * the result of that computation back into reg[f+3]. The load of reg[f+3]
   * has to go into a register of its own, so it can be fed into the "add"
   * operator. That register also needs to be allocated and that allocation 
   * resides in rvalue_.
   */
  struct sl_reg_alloc *rvalue_;

  union {
    int regs_[16];
    struct sl_reg_alloc_compound comp_;   /* struct */
    struct sl_reg_alloc_array array_;
  } v_;
};

struct sl_reg_allocator {
  struct ref_range_allocator rra_floats_;
  struct ref_range_allocator rra_ints_;
  struct ref_range_allocator rra_bools_;
  struct ref_range_allocator rra_sampler2D_;
  struct ref_range_allocator rra_samplerCube_;
};

void sl_reg_alloc_init(struct sl_reg_alloc *ra);
int sl_reg_alloc_set_type(struct sl_reg_alloc *ra, const struct sl_type *t);
void sl_reg_alloc_cleanup(struct sl_reg_alloc *ra);
int sl_reg_alloc_is_allocated(const struct sl_reg_alloc *ra);
int sl_reg_alloc_clone(struct sl_reg_alloc *dst, const struct sl_reg_alloc *src);

void sl_reg_allocator_init(struct sl_reg_allocator *ra);
void sl_reg_allocator_cleanup(struct sl_reg_allocator *ra);

int sl_reg_allocator_alloc(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra);
int sl_reg_allocator_lock(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra);
int sl_reg_allocator_unlock(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra);
int sl_reg_allocator_lock_or_alloc(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_REG_ALLOC_H */
