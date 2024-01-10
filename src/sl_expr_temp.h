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

#ifndef SL_EXPR_TEMP_H
#define SL_EXPR_TEMP_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct sl_type;

typedef enum sl_expr_temp_kind {
  sletk_void,
  sletk_array,
  sletk_struct,
  sletk_float,
  sletk_int,
  sletk_bool,
  sletk_vec2,
  sletk_vec3,
  sletk_vec4,
  sletk_bvec2,
  sletk_bvec3,
  sletk_bvec4,
  sletk_ivec2,
  sletk_ivec3,
  sletk_ivec4,
  sletk_mat2,
  sletk_mat3,
  sletk_mat4
} sl_expr_temp_kind_t;

struct sl_expr_temp_compound {
  /* The sltk_struct or sltk_array type that this compound describes. */
  struct sl_type *struct_or_array_type_;

  /* For sletk_array, this is what you'd expect (each element corresponds
   * to the array's element.)
   * For sletk_struct, the sequential index of each field corresponds to
   * each element */
  size_t num_elements_;
  struct sl_expr_temp *elements_;
};

struct sl_expr_temp {
  sl_expr_temp_kind_t kind_;

  union {
    float f_;
    int64_t i_;
    int b_;
    float v_[4];    /* vec2, vec3, vec4 */
    int bv_[4];      /* bvec2, bvec3, bvec4 */
    int64_t iv_[4];  /* ivec2, ivec3, ivec4 */
    float m_[16];    /* mat2, mat3, mat4 */
    struct sl_expr_temp_compound comp_;   /* struct, array */
  } v_;
};

struct sl_type *sl_expr_temp_type(struct sl_type_base *tb, const struct sl_expr_temp *slet);

void sl_expr_temp_init_void(struct sl_expr_temp *slet);
void sl_expr_temp_init_float(struct sl_expr_temp *slet, float f);
void sl_expr_temp_init_int(struct sl_expr_temp *slet, int64_t i);
void sl_expr_temp_init_bool(struct sl_expr_temp *slet, int b);
void sl_expr_temp_init_vec2(struct sl_expr_temp *slet, float x, float y);
void sl_expr_temp_init_vec3(struct sl_expr_temp *slet, float x, float y, float z);
void sl_expr_temp_init_vec4(struct sl_expr_temp *slet, float x, float y, float z, float w);
void sl_expr_temp_init_bvec2(struct sl_expr_temp *slet, int x, int y);
void sl_expr_temp_init_bvec3(struct sl_expr_temp *slet, int x, int y, int z);
void sl_expr_temp_init_bvec4(struct sl_expr_temp *slet, int x, int y, int z, int w);
void sl_expr_temp_init_ivec2(struct sl_expr_temp *slet, int64_t x, int64_t y);
void sl_expr_temp_init_ivec3(struct sl_expr_temp *slet, int64_t x, int64_t y, int64_t z);
void sl_expr_temp_init_ivec4(struct sl_expr_temp *slet, int64_t x, int64_t y, int64_t z, int64_t w);
void sl_expr_temp_init_mat2(struct sl_expr_temp *slet, float a, float b, float c, float d);
void sl_expr_temp_init_mat3(struct sl_expr_temp *slet, float a, float b, float c, float d, float e, float f, float g, float h, float i);
void sl_expr_temp_init_mat4(struct sl_expr_temp *slet, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p);

/* Initializes array and/or struct types to 0 memory. Returns 0 upon success or
 * -1 upon memory allocation error or overflow. */
int sl_expr_temp_init_array(struct sl_expr_temp *slet, struct sl_type *array_type);
int sl_expr_temp_init_struct(struct sl_expr_temp *slet, struct sl_type *struct_type);

/* initializes a temporary with the given type; this can also be a struct or array type.
 * The value(s) is/are initialized to its 0 state (e.g. 0, false, 0.f, etc.)
 * Passing in an sltk_void type is permitted but means the temp has no value. You can
 * also pass in a NULL pointer for the type to imply void, this ensures you can later
 * safely call sl_expr_temp_cleanup() on the temp irrespective of subsequent 
 * sl_expr_temp_init_XXX calls to fully initialize the temp.
 * Returns 0 upon success or -1 upon memory error or integer overflow. */
int sl_expr_temp_init(struct sl_expr_temp *slet, struct sl_type *t);

void sl_expr_temp_cleanup(struct sl_expr_temp *slet);

/* Returns 0 upon success or -1 upon memory error or integer overflow. */
int sl_expr_temp_copy(struct sl_expr_temp *dst, const struct sl_expr_temp *src);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_EXPR_TEMP_H */
