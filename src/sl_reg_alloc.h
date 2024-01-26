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

#ifdef __cplusplus
extern "C" {
#endif

/* Register value if not yet allocated (which is the default after initialization) */
#define SL_REG_NONE -1

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
  /* The slrak_struct or slrak_array type that this compound describes. */
  struct sl_type *struct_or_array_type_;

  /* For sletk_array, this is what you'd expect (each element corresponds
   * to the array's element.)
   * For sletk_struct, the sequential index of each field corresponds to
   * each element */
  size_t num_elements_;
  struct sl_reg_alloc *elements_;
};

struct sl_reg_alloc {
  sl_reg_alloc_kind_t kind_;

  union {
    int regs_[16];
    struct sl_reg_alloc_compound comp_;   /* struct, array */
  } v_;
};

void sl_reg_alloc_init(struct sl_reg_alloc *ra);
int sl_reg_alloc_set_type(struct sl_reg_alloc *ra, const struct sl_type *t);
void sl_reg_alloc_cleanup(struct sl_reg_alloc *ra);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_REG_ALLOC_H */
