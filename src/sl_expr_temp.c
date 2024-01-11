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

#ifndef LIMITS_H_INCLUDED
#define LIMITS_H_INCLUDED
#include <limits.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef SL_EXPR_TEMP_H_INCLUDED
#define SL_EXPR_TEMP_H_INCLUDED
#include "sl_expr_temp.h"
#endif

#ifndef SL_TYPES_H_INCLUDED
#define SL_TYPES_H_INCLUDED
#include "sl_types.h"
#endif

struct sl_type *sl_expr_temp_type(struct sl_type_base *tb, const struct sl_expr_temp *slet) {
  switch (slet->kind_) {
    case sletk_void:
      return &tb->void_;
    case sletk_array:
    case sletk_struct:
      return slet->v_.comp_.struct_or_array_type_;
    case sletk_float:
      return &tb->float_;
    case sletk_int:
      return &tb->int_;
    case sletk_bool:
      return &tb->bool_;
    case sletk_vec2:
      return &tb->vec2_;
    case sletk_vec3:
      return &tb->vec3_;
    case sletk_vec4:
      return &tb->vec4_;
    case sletk_bvec2:
      return &tb->bvec2_;
    case sletk_bvec3:
      return &tb->bvec3_;
    case sletk_bvec4:
      return &tb->bvec4_;
    case sletk_ivec2:
      return &tb->ivec2_;
    case sletk_ivec3:
      return &tb->ivec3_;
    case sletk_ivec4:
      return &tb->ivec4_;
    case sletk_mat2:
      return &tb->mat2_;
    case sletk_mat3:
      return &tb->mat3_;
    case sletk_mat4:
      return &tb->mat4_;
  }
  return NULL;
}

void sl_expr_temp_init_void(struct sl_expr_temp *slet) {
  slet->kind_ = sletk_void;
}

void sl_expr_temp_init_float(struct sl_expr_temp *slet, float f) {
  slet->kind_ = sletk_float;
  slet->v_.f_ = f;
}

void sl_expr_temp_init_int(struct sl_expr_temp *slet, int64_t i) {
  slet->kind_ = sletk_int;
  slet->v_.i_ = i;
}

void sl_expr_temp_init_bool(struct sl_expr_temp *slet, int b) {
  slet->kind_ = sletk_bool;
  slet->v_.b_ = b;
}

void sl_expr_temp_init_vec2(struct sl_expr_temp *slet, float x, float y) {
  slet->kind_ = sletk_vec2;
  slet->v_.v_[0] = x;
  slet->v_.v_[1] = y;
}

void sl_expr_temp_init_vec3(struct sl_expr_temp *slet, float x, float y, float z) {
  slet->kind_ = sletk_vec3;
  slet->v_.v_[0] = x;
  slet->v_.v_[1] = y;
  slet->v_.v_[2] = z;
}

void sl_expr_temp_init_vec4(struct sl_expr_temp *slet, float x, float y, float z, float w) {
  slet->kind_ = sletk_vec4;
  slet->v_.v_[0] = x;
  slet->v_.v_[1] = y;
  slet->v_.v_[2] = z;
  slet->v_.v_[3] = w;
}

void sl_expr_temp_init_bvec2(struct sl_expr_temp *slet, int x, int y) {
  slet->kind_ = sletk_bvec2;
  slet->v_.bv_[0] = x;
  slet->v_.bv_[1] = y;
}

void sl_expr_temp_init_bvec3(struct sl_expr_temp *slet, int x, int y, int z) {
  slet->kind_ = sletk_bvec3;
  slet->v_.bv_[0] = x;
  slet->v_.bv_[1] = y;
  slet->v_.bv_[2] = z;
}

void sl_expr_temp_init_bvec4(struct sl_expr_temp *slet, int x, int y, int z, int w) {
  slet->kind_ = sletk_bvec4;
  slet->v_.bv_[0] = x;
  slet->v_.bv_[1] = y;
  slet->v_.bv_[2] = z;
  slet->v_.bv_[3] = w;
}

void sl_expr_temp_init_ivec2(struct sl_expr_temp *slet, int64_t x, int64_t y) {
  slet->kind_ = sletk_ivec2;
  slet->v_.iv_[0] = x;
  slet->v_.iv_[1] = y;
}

void sl_expr_temp_init_ivec3(struct sl_expr_temp *slet, int64_t x, int64_t y, int64_t z) {
  slet->kind_ = sletk_ivec3;
  slet->v_.iv_[0] = x;
  slet->v_.iv_[1] = y;
  slet->v_.iv_[2] = z;
}

void sl_expr_temp_init_ivec4(struct sl_expr_temp *slet, int64_t x, int64_t y, int64_t z, int64_t w) {
  slet->kind_ = sletk_ivec4;
  slet->v_.iv_[0] = x;
  slet->v_.iv_[1] = y;
  slet->v_.iv_[2] = z;
  slet->v_.iv_[3] = w;
}

void sl_expr_temp_init_mat2(struct sl_expr_temp *slet, float a, float b, float c, float d) {
  slet->kind_ = sletk_mat2;
  slet->v_.m_[0] = a;
  slet->v_.m_[1] = b;
  slet->v_.m_[2] = c;
  slet->v_.m_[3] = d;
}

void sl_expr_temp_init_mat3(struct sl_expr_temp *slet, float a, float b, float c, float d, float e, float f, float g, float h, float i) {
  slet->kind_ = sletk_mat3;
  slet->v_.m_[0] = a;
  slet->v_.m_[1] = b;
  slet->v_.m_[2] = c;
  slet->v_.m_[3] = d;
  slet->v_.m_[4] = e;
  slet->v_.m_[5] = f;
  slet->v_.m_[6] = g;
  slet->v_.m_[7] = h;
  slet->v_.m_[8] = i;
}

void sl_expr_temp_init_mat4(struct sl_expr_temp *slet, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p) {
  slet->kind_ = sletk_mat4;
  slet->v_.m_[0] = a;
  slet->v_.m_[1] = b;
  slet->v_.m_[2] = c;
  slet->v_.m_[3] = d;
  slet->v_.m_[4] = e;
  slet->v_.m_[5] = f;
  slet->v_.m_[6] = g;
  slet->v_.m_[7] = h;
  slet->v_.m_[8] = i;
  slet->v_.m_[9] = j;
  slet->v_.m_[10] = k;
  slet->v_.m_[11] = l;
  slet->v_.m_[12] = m;
  slet->v_.m_[13] = n;
  slet->v_.m_[14] = o;
  slet->v_.m_[15] = p;
}

int sl_expr_temp_init_array(struct sl_expr_temp *slet, struct sl_type *array_type) {
  struct sl_type *unq_arr = sl_type_unqualified(array_type);
  if (unq_arr->kind_ != sltk_array) {
    /* not an array type */
    return -1;
  }
  slet->kind_ = sletk_array;
  slet->v_.comp_.struct_or_array_type_ = array_type;
  slet->v_.comp_.num_elements_ = (size_t)unq_arr->array_size_;
  if (((uint64_t)slet->v_.comp_.num_elements_) != unq_arr->array_size_) {
    /* conversion from uint64_t to size_t failed (overflowed) */
    return -1;
  }
  size_t mem_needed = sizeof(struct sl_expr_temp) * slet->v_.comp_.num_elements_;
  if ((SIZE_MAX / sizeof(struct sl_expr_temp)) < slet->v_.comp_.num_elements_) {
    /* overflow */
    return -1;
  }
  slet->v_.comp_.elements_ = (struct sl_expr_temp *)malloc(sizeof(struct sl_expr_temp) * slet->v_.comp_.num_elements_);
  if (!slet->v_.comp_.elements_) {
    /* no mem */
    return -1;
  }
  size_t n;
  for (n = 0; n < slet->v_.comp_.num_elements_; ++n) {
    int r = sl_expr_temp_init(&slet->v_.comp_.elements_[n], unq_arr->derived_type_);
    if (r) {
      /* failed to init; need to reverse steps */
      while (n) {
        --n;
        sl_expr_temp_cleanup(slet->v_.comp_.elements_ + n);
      }
      return -1;
    }
  }
  return 0;
}

int sl_expr_temp_init_struct(struct sl_expr_temp *slet, struct sl_type *struct_type) {
  struct sl_type *unq_struct = sl_type_unqualified(struct_type);
  if (unq_struct->kind_ != sltk_struct) {
    /* not a struct type */
    return -1;
  }
  slet->kind_ = sletk_struct;
  size_t num_fields = 0;
  struct sl_type_field *field = unq_struct->fields_;
  if (field) {
    do {
      field = field->chain_;
      ++num_fields;
    } while (field != unq_struct->fields_);
  }
  slet->v_.comp_.struct_or_array_type_ = struct_type;
  slet->v_.comp_.num_elements_ = num_fields;
  if ((SIZE_MAX / sizeof(struct sl_expr_temp)) < slet->v_.comp_.num_elements_) {
    /* overflow */
    return -1;
  }
  slet->v_.comp_.elements_ = (struct sl_expr_temp *)malloc(sizeof(struct sl_expr_temp) * num_fields);
  if (!slet->v_.comp_.elements_) {
    /* no mem */
    return -1;
  }
  field = unq_struct->fields_;
  size_t num_field = 0;
  if (field) {
    do {
      field = field->chain_;
      int r = sl_expr_temp_init(slet->v_.comp_.elements_ + num_field, field->type_);
      if (r) {
        /* failed to init; need to reverse steps */
        while (num_field) {
          --num_field;
          sl_expr_temp_cleanup(slet->v_.comp_.elements_ + num_field);
        }
        return -1;
      }
      ++num_field;
    } while (field != unq_struct->fields_);
  }
  return 0;
}

int sl_expr_temp_init(struct sl_expr_temp *slet, struct sl_type *t) {
  if (!t) {
    /* special case for dud initialization short-hand */
    slet->kind_ = sletk_void;
    return 0;
  }
  struct sl_type *tunq = sl_type_unqualified(t);
  switch (tunq->kind_) {
    case sltk_array:
      return sl_expr_temp_init_array(slet, t);
      break;
    case sltk_struct:
      return sl_expr_temp_init_struct(slet, t);
      break;
    case sltk_void:
      slet->kind_ = sletk_void;
      break;
    case sltk_float:
      sl_expr_temp_init_float(slet, 0.0f);
      break;
    case sltk_int:
      sl_expr_temp_init_int(slet, 0);
      break;
    case sltk_bool:
      sl_expr_temp_init_bool(slet, 0);
      break;
    case sltk_vec2:
      sl_expr_temp_init_vec2(slet, 0.0f, 0.0f);
      break;
    case sltk_vec3:
      sl_expr_temp_init_vec3(slet, 0.0f, 0.0f, 0.0f);
      break;
    case sltk_vec4:
      sl_expr_temp_init_vec4(slet, 0.0f, 0.0f, 0.0f, 0.0f);
      break;
    case sltk_bvec2:
      sl_expr_temp_init_bvec2(slet, 0, 0);
      break;
    case sltk_bvec3:
      sl_expr_temp_init_bvec3(slet, 0, 0, 0);
      break;
    case sltk_bvec4:
      sl_expr_temp_init_bvec4(slet, 0, 0, 0, 0);
      break;
    case sltk_ivec2:
      sl_expr_temp_init_ivec2(slet, 0, 0);
      break;
    case sltk_ivec3:
      sl_expr_temp_init_ivec3(slet, 0, 0, 0);
      break;
    case sltk_ivec4:
      sl_expr_temp_init_ivec4(slet, 0, 0, 0, 0);
      break;
    case sltk_mat2:
      sl_expr_temp_init_mat2(slet, 0.0f, 0.0f, 
                                   0.0f, 0.0f);
      break;
    case sltk_mat3:
      sl_expr_temp_init_mat3(slet, 0.f, 0.f, 0.f, 
                                   0.f, 0.f, 0.f, 
                                   0.f, 0.f, 0.f);
      break;
    case sltk_mat4:
      sl_expr_temp_init_mat4(slet, 0.f, 0.f, 0.f, 0.f,
                                   0.f, 0.f, 0.f, 0.f,
                                   0.f, 0.f, 0.f, 0.f,       
                                   0.f, 0.f, 0.f, 0.f);
      break;
  }
  return 0;
}

void sl_expr_temp_cleanup(struct sl_expr_temp *slet) {
  switch (slet->kind_) {
    case sletk_struct:
    case sletk_array: {
      size_t index;
      for (index = 0; index < slet->v_.comp_.num_elements_; ++index) {
        sl_expr_temp_cleanup(slet->v_.comp_.elements_ + index);
      }
      if (slet->v_.comp_.elements_) {
        free(slet->v_.comp_.elements_);
      }
      break;
    }
  }
}

int sl_expr_temp_copy(struct sl_expr_temp *dst, const struct sl_expr_temp *src) {
  int r;
  if ((src->kind_ == sletk_struct) || (src->kind_ == sletk_array)) {
    /* need to copy elements */
    size_t index;

    struct sl_expr_temp *elms;
    elms = (struct sl_expr_temp *)malloc(sizeof(struct sl_expr_temp) * src->v_.comp_.num_elements_);
    if (!elms) {
      /* no mem */
      return -1;
    }

    for (index = 0; index < src->v_.comp_.num_elements_; ++index) {
      r = sl_expr_temp_copy(elms + index, src->v_.comp_.elements_ + index);
      if (r) {
        /* failed to copy; need to reverse steps */
        while (index) {
          --index;
          sl_expr_temp_cleanup(elms + index);
        }
        free(elms);

        return -1;
      }
    }

    sl_expr_temp_cleanup(dst);
    dst->v_.comp_.num_elements_ = src->v_.comp_.num_elements_;
    dst->v_.comp_.struct_or_array_type_ = src->v_.comp_.struct_or_array_type_;
    dst->v_.comp_.elements_ = elms;

    return 0;
  }
  /* Simple copy will do */
  sl_expr_temp_cleanup(dst);
  memcpy(dst, src, sizeof(*dst));

  return 0;
}

int sl_expr_temp_negate(struct sl_expr_temp *dst, const struct sl_expr_temp *src) {
  struct sl_expr_temp tmp;
  switch (src->kind_) {
    case sletk_float:
      sl_expr_temp_init_float(&tmp, -src->v_.f_);
      break;
    case sletk_int:
      sl_expr_temp_init_int(&tmp, -src->v_.i_);
      break;
    case sletk_vec2:
      sl_expr_temp_init_vec2(&tmp, -src->v_.v_[0], -src->v_.v_[1]);
      break;
    case sletk_vec3:
      sl_expr_temp_init_vec3(&tmp, -src->v_.v_[0], -src->v_.v_[1], -src->v_.v_[2]);
      break;
    case sletk_vec4:
      sl_expr_temp_init_vec4(&tmp, -src->v_.v_[0], -src->v_.v_[1], -src->v_.v_[2], -src->v_.v_[3]);
      break;
    case sletk_ivec2:
      sl_expr_temp_init_ivec2(&tmp, -src->v_.iv_[0], -src->v_.iv_[1]);
      break;
    case sletk_ivec3:
      sl_expr_temp_init_ivec3(&tmp, -src->v_.iv_[0], -src->v_.iv_[1], -src->v_.iv_[2]);
      break;
    case sletk_ivec4:
      sl_expr_temp_init_ivec4(&tmp, -src->v_.iv_[0], -src->v_.iv_[1], -src->v_.iv_[2], -src->v_.iv_[3]);
      break;
    case sletk_mat2:
      sl_expr_temp_init_mat2(&tmp, -src->v_.m_[0], -src->v_.m_[1],
                                   -src->v_.m_[2], -src->v_.m_[3]);
      break;
    case sletk_mat3:
      sl_expr_temp_init_mat3(&tmp, -src->v_.m_[0], -src->v_.m_[1], -src->v_.m_[2],
                                   -src->v_.m_[3], -src->v_.m_[4], -src->v_.m_[5],
                                   -src->v_.m_[6], -src->v_.m_[7], -src->v_.m_[8]);
      break;
    case sletk_mat4:
      sl_expr_temp_init_mat4(&tmp, -src->v_.m_[0], -src->v_.m_[1], -src->v_.m_[2], -src->v_.m_[3],
                                   -src->v_.m_[4], -src->v_.m_[5], -src->v_.m_[6], -src->v_.m_[7],
                                   -src->v_.m_[8], -src->v_.m_[9], -src->v_.m_[10], -src->v_.m_[11],
                                   -src->v_.m_[12], -src->v_.m_[13], -src->v_.m_[14], -src->v_.m_[15]);
      break;
    default:
      return -1;
      break;
  }
  if (sl_expr_temp_copy(dst, &tmp)) {
    sl_expr_temp_cleanup(&tmp);
    return -1;
  }
  sl_expr_temp_cleanup(&tmp);
  return 0;
}

int sl_expr_temp_logical_not(struct sl_expr_temp *dst, const struct sl_expr_temp *src) {
  struct sl_expr_temp tmp;
  switch (src->kind_) {
    case sletk_bool:
      sl_expr_temp_init_bool(&tmp, !src->v_.b_);
      break;
    case sletk_bvec2:
      sl_expr_temp_init_bvec2(&tmp, !src->v_.bv_[0], !src->v_.bv_[1]);
      break;
    case sletk_bvec3:
      sl_expr_temp_init_bvec3(&tmp, !src->v_.bv_[0], !src->v_.bv_[1], !src->v_.bv_[2]);
      break;
    case sletk_bvec4:
      sl_expr_temp_init_bvec4(&tmp, !src->v_.bv_[0], !src->v_.bv_[1], !src->v_.bv_[2], !src->v_.bv_[3]);
      break;
    default:
      return -1;
      break;
  }
  if (sl_expr_temp_copy(dst, &tmp)) {
    sl_expr_temp_cleanup(&tmp);
    return -1;
  }
  sl_expr_temp_cleanup(&tmp);
  return 0;
}

int sl_expr_temp_add(struct sl_expr_temp *dst, const struct sl_expr_temp *left, const struct sl_expr_temp *right) {
  /* note: we could simplify the code by first checking left and right's kind_ and making sure the "lower" kind_
   * goes before the higher (swapping) - but we don't, so the code that appears here is very similar to the code
   * that appears in other, non-commutative, binary operators. */
  struct sl_expr_temp tmp;
  if (left->kind_ == right->kind_) {
    switch (left->kind_) {
      case sletk_float:
        sl_expr_temp_init_float(&tmp, left->v_.f_ + right->v_.f_);
        break;
      case sletk_int:
        sl_expr_temp_init_int(&tmp, left->v_.i_ + right->v_.i_);
        break;
      case sletk_vec2:
        sl_expr_temp_init_vec2(&tmp, left->v_.v_[0] + right->v_.v_[0], 
                                     left->v_.v_[1] + right->v_.v_[1]);
        break;
      case sletk_vec3:
        sl_expr_temp_init_vec3(&tmp, left->v_.v_[0] + right->v_.v_[0], 
                                     left->v_.v_[1] + right->v_.v_[1], 
                                     left->v_.v_[2] + right->v_.v_[2]);
        break;
      case sletk_vec4:
        sl_expr_temp_init_vec4(&tmp, left->v_.v_[0] + right->v_.v_[0], 
                                     left->v_.v_[1] + right->v_.v_[1], 
                                     left->v_.v_[2] + right->v_.v_[2], 
                                     left->v_.v_[3] + right->v_.v_[3]);
        break;
      case sletk_ivec2:
        sl_expr_temp_init_ivec2(&tmp, left->v_.iv_[0] + right->v_.iv_[0], 
                                      left->v_.iv_[1] + right->v_.iv_[1]);
        break;
      case sletk_ivec3:
        sl_expr_temp_init_ivec3(&tmp, left->v_.iv_[0] + right->v_.iv_[0], 
                                      left->v_.iv_[1] + right->v_.iv_[1], 
                                      left->v_.iv_[2] + right->v_.iv_[2]);
        break;
      case sletk_ivec4:
        sl_expr_temp_init_ivec4(&tmp, left->v_.iv_[0] + right->v_.iv_[0], 
                                      left->v_.iv_[1] + right->v_.iv_[1], 
                                      left->v_.iv_[2] + right->v_.iv_[2], 
                                      left->v_.iv_[3] + right->v_.iv_[3]);
        break;
      case sletk_mat2:
        sl_expr_temp_init_mat2(&tmp, left->v_.m_[0] + right->v_.m_[0], left->v_.m_[1] + right->v_.m_[1], 
                                     left->v_.m_[2] + right->v_.m_[2], left->v_.m_[3] + right->v_.m_[3]);
        break;
      case sletk_mat3:
        sl_expr_temp_init_mat3(&tmp, left->v_.m_[0] + right->v_.m_[0], left->v_.m_[1] + right->v_.m_[1], left->v_.m_[2] + right->v_.m_[2], 
                                     left->v_.m_[3] + right->v_.m_[3], left->v_.m_[4] + right->v_.m_[4], left->v_.m_[5] + right->v_.m_[5], 
                                     left->v_.m_[6] + right->v_.m_[6], left->v_.m_[7] + right->v_.m_[7], left->v_.m_[8] + right->v_.m_[8]);
        break;
      case sletk_mat4:
        sl_expr_temp_init_mat4(&tmp, left->v_.m_[0] + right->v_.m_[0], left->v_.m_[1] + right->v_.m_[1], left->v_.m_[2] + right->v_.m_[2], left->v_.m_[3] + right->v_.m_[3], 
                                     left->v_.m_[4] + right->v_.m_[4], left->v_.m_[5] + right->v_.m_[5], left->v_.m_[6] + right->v_.m_[6], left->v_.m_[7] + right->v_.m_[7], 
                                     left->v_.m_[8] + right->v_.m_[8], left->v_.m_[9] + right->v_.m_[9], left->v_.m_[10] + right->v_.m_[10], left->v_.m_[11] + right->v_.m_[11], 
                                     left->v_.m_[12] + right->v_.m_[12], left->v_.m_[13] + right->v_.m_[13], left->v_.m_[14] + right->v_.m_[14], left->v_.m_[15] + right->v_.m_[15]);
        break;
      default:
        return -1;
    }
  }
  else if (left->kind_ == sletk_float) {
    switch (right->kind_) {
      case sletk_vec2:
        sl_expr_temp_init_vec2(&tmp, left->v_.f_ + right->v_.v_[0], 
                                     left->v_.f_ + right->v_.v_[1]);
        break;
      case sletk_vec3:
        sl_expr_temp_init_vec3(&tmp, left->v_.f_+ right->v_.v_[0], 
                                     left->v_.f_ + right->v_.v_[1], 
                                     left->v_.f_ + right->v_.v_[2]);
        break;
      case sletk_vec4:
        sl_expr_temp_init_vec4(&tmp, left->v_.f_ + right->v_.v_[0], 
                                     left->v_.f_ + right->v_.v_[1], 
                                     left->v_.f_ + right->v_.v_[2], 
                                     left->v_.f_ + right->v_.v_[3]);
        break;
      case sletk_mat2:
        sl_expr_temp_init_mat2(&tmp, left->v_.f_ + right->v_.m_[0], left->v_.f_ + right->v_.m_[1], 
                                     left->v_.f_ + right->v_.m_[2], left->v_.f_ + right->v_.m_[3]);
        break;
      case sletk_mat3:
        sl_expr_temp_init_mat3(&tmp, left->v_.f_ + right->v_.m_[0], left->v_.f_ + right->v_.m_[1], left->v_.f_ + right->v_.m_[2], 
                                     left->v_.f_ + right->v_.m_[3], left->v_.f_ + right->v_.m_[4], left->v_.f_ + right->v_.m_[5], 
                                     left->v_.f_ + right->v_.m_[6], left->v_.f_ + right->v_.m_[7], left->v_.f_ + right->v_.m_[8]);
        break;
      case sletk_mat4:
        sl_expr_temp_init_mat4(&tmp, left->v_.f_ + right->v_.m_[0], left->v_.f_ + right->v_.m_[1], left->v_.f_ + right->v_.m_[2], left->v_.f_ + right->v_.m_[3], 
                                     left->v_.f_ + right->v_.m_[4], left->v_.f_ + right->v_.m_[5], left->v_.f_ + right->v_.m_[6], left->v_.f_ + right->v_.m_[7], 
                                     left->v_.f_ + right->v_.m_[8], left->v_.f_ + right->v_.m_[9], left->v_.f_ + right->v_.m_[10], left->v_.f_ + right->v_.m_[11], 
                                     left->v_.f_ + right->v_.m_[12], left->v_.f_ + right->v_.m_[13], left->v_.f_ + right->v_.m_[14], left->v_.f_ + right->v_.m_[15]);
        break;
      default:
        return -1;
    }
  }
  else if (left->kind_ == sletk_int) {
    switch (right->kind_) {
      case sletk_ivec2:
        sl_expr_temp_init_ivec2(&tmp, left->v_.i_ + right->v_.iv_[0], 
                                      left->v_.i_ + right->v_.iv_[1]);
        break;
      case sletk_ivec3:
        sl_expr_temp_init_ivec3(&tmp, left->v_.i_ + right->v_.iv_[0], 
                                      left->v_.i_ + right->v_.iv_[1], 
                                      left->v_.i_ + right->v_.iv_[2]);
        break;
      case sletk_ivec4:
        sl_expr_temp_init_ivec4(&tmp, left->v_.i_ + right->v_.iv_[0], 
                                      left->v_.i_ + right->v_.iv_[1], 
                                      left->v_.i_ + right->v_.iv_[2], 
                                      left->v_.i_ + right->v_.iv_[3]);
        break;
      default:
        return -1;
    }
  }
  else if ((left->kind_ == sletk_vec2) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_vec2(&tmp, left->v_.v_[0] + right->v_.f_, 
                                 left->v_.v_[1] + right->v_.f_);
  }
  else if ((left->kind_ == sletk_vec3) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_vec3(&tmp, left->v_.v_[0] + right->v_.f_, 
                                 left->v_.v_[1] + right->v_.f_, 
                                 left->v_.v_[2] + right->v_.f_);
  }
  else if ((left->kind_ == sletk_vec4) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_vec4(&tmp, left->v_.v_[0] + right->v_.f_, 
                                 left->v_.v_[1] + right->v_.f_, 
                                 left->v_.v_[2] + right->v_.f_, 
                                 left->v_.v_[3] + right->v_.f_);
  }
  else if ((left->kind_ == sletk_ivec2) && (right->kind_ == sletk_int)) {
    sl_expr_temp_init_ivec2(&tmp, left->v_.iv_[0] + right->v_.i_,
                                  left->v_.iv_[1] + right->v_.i_);
  }
  else if ((left->kind_ == sletk_ivec3) && (right->kind_ == sletk_int)) {
    sl_expr_temp_init_ivec3(&tmp, left->v_.iv_[0] + right->v_.i_,
                                  left->v_.iv_[1] + right->v_.i_,
                                  left->v_.iv_[2] + right->v_.i_);
  }
  else if ((left->kind_ == sletk_ivec4) && (right->kind_ == sletk_int)) {
    sl_expr_temp_init_ivec4(&tmp, left->v_.iv_[0] + right->v_.i_,
                                  left->v_.iv_[1] + right->v_.i_,
                                  left->v_.iv_[2] + right->v_.i_,
                                  left->v_.iv_[3] + right->v_.i_);
  }
  else if ((left->kind_ == sletk_mat2) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_mat2(&tmp, left->v_.m_[0] + right->v_.f_, left->v_.m_[1] + right->v_.f_,
                                 left->v_.m_[2] + right->v_.f_, left->v_.m_[3] + right->v_.f_);
  }
  else if ((left->kind_ == sletk_mat3) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_mat3(&tmp, left->v_.m_[0] + right->v_.f_, left->v_.m_[1] + right->v_.f_, left->v_.m_[2] + right->v_.f_,
                                 left->v_.m_[3] + right->v_.f_, left->v_.m_[4] + right->v_.f_, left->v_.m_[5] + right->v_.f_,
                                 left->v_.m_[6] + right->v_.f_, left->v_.m_[7] + right->v_.f_, left->v_.m_[8] + right->v_.f_);
  }
  else if ((left->kind_ == sletk_mat4) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_mat4(&tmp, left->v_.m_[0] + right->v_.f_, left->v_.m_[1] + right->v_.f_, left->v_.m_[2] + right->v_.f_, left->v_.m_[3] + right->v_.f_,
                                 left->v_.m_[4] + right->v_.f_, left->v_.m_[5] + right->v_.f_, left->v_.m_[6] + right->v_.f_, left->v_.m_[7] + right->v_.f_,
                                 left->v_.m_[8] + right->v_.f_, left->v_.m_[9] + right->v_.f_, left->v_.m_[10] + right->v_.f_, left->v_.m_[11] + right->v_.f_,
                                 left->v_.m_[12] + right->v_.f_, left->v_.m_[13] + right->v_.f_, left->v_.m_[14] + right->v_.f_, left->v_.m_[15] + right->v_.f_);
  }
  else {
    return -1;
  }

  if (sl_expr_temp_copy(dst, &tmp)) {
    sl_expr_temp_cleanup(&tmp);
    return -1;
  }
  sl_expr_temp_cleanup(&tmp);
  return 0;
}

int sl_expr_temp_sub(struct sl_expr_temp *dst, const struct sl_expr_temp *left, const struct sl_expr_temp *right) {
  struct sl_expr_temp tmp;
  if (left->kind_ == right->kind_) {
    switch (left->kind_) {
      case sletk_float:
        sl_expr_temp_init_float(&tmp, left->v_.f_ - right->v_.f_);
        break;
      case sletk_int:
        sl_expr_temp_init_int(&tmp, left->v_.i_ - right->v_.i_);
        break;
      case sletk_vec2:
        sl_expr_temp_init_vec2(&tmp, left->v_.v_[0] - right->v_.v_[0], 
                                     left->v_.v_[1] - right->v_.v_[1]);
        break;
      case sletk_vec3:
        sl_expr_temp_init_vec3(&tmp, left->v_.v_[0] - right->v_.v_[0], 
                                     left->v_.v_[1] - right->v_.v_[1], 
                                     left->v_.v_[2] - right->v_.v_[2]);
        break;
      case sletk_vec4:
        sl_expr_temp_init_vec4(&tmp, left->v_.v_[0] - right->v_.v_[0], 
                                     left->v_.v_[1] - right->v_.v_[1], 
                                     left->v_.v_[2] - right->v_.v_[2], 
                                     left->v_.v_[3] - right->v_.v_[3]);
        break;
      case sletk_ivec2:
        sl_expr_temp_init_ivec2(&tmp, left->v_.iv_[0] - right->v_.iv_[0], 
                                      left->v_.iv_[1] - right->v_.iv_[1]);
        break;
      case sletk_ivec3:
        sl_expr_temp_init_ivec3(&tmp, left->v_.iv_[0] - right->v_.iv_[0], 
                                      left->v_.iv_[1] - right->v_.iv_[1], 
                                      left->v_.iv_[2] - right->v_.iv_[2]);
        break;
      case sletk_ivec4:
        sl_expr_temp_init_ivec4(&tmp, left->v_.iv_[0] - right->v_.iv_[0], 
                                      left->v_.iv_[1] - right->v_.iv_[1], 
                                      left->v_.iv_[2] - right->v_.iv_[2], 
                                      left->v_.iv_[3] - right->v_.iv_[3]);
        break;
      case sletk_mat2:
        sl_expr_temp_init_mat2(&tmp, left->v_.m_[0] - right->v_.m_[0], left->v_.m_[1] - right->v_.m_[1], 
                                     left->v_.m_[2] - right->v_.m_[2], left->v_.m_[3] - right->v_.m_[3]);
        break;
      case sletk_mat3:
        sl_expr_temp_init_mat3(&tmp, left->v_.m_[0] - right->v_.m_[0], left->v_.m_[1] - right->v_.m_[1], left->v_.m_[2] - right->v_.m_[2], 
                                     left->v_.m_[3] - right->v_.m_[3], left->v_.m_[4] - right->v_.m_[4], left->v_.m_[5] - right->v_.m_[5], 
                                     left->v_.m_[6] - right->v_.m_[6], left->v_.m_[7] - right->v_.m_[7], left->v_.m_[8] - right->v_.m_[8]);
        break;
      case sletk_mat4:
        sl_expr_temp_init_mat4(&tmp, left->v_.m_[0] - right->v_.m_[0], left->v_.m_[1] - right->v_.m_[1], left->v_.m_[2] - right->v_.m_[2], left->v_.m_[3] - right->v_.m_[3], 
                                     left->v_.m_[4] - right->v_.m_[4], left->v_.m_[5] - right->v_.m_[5], left->v_.m_[6] - right->v_.m_[6], left->v_.m_[7] - right->v_.m_[7], 
                                     left->v_.m_[8] - right->v_.m_[8], left->v_.m_[9] - right->v_.m_[9], left->v_.m_[10] - right->v_.m_[10], left->v_.m_[11] - right->v_.m_[11], 
                                     left->v_.m_[12] - right->v_.m_[12], left->v_.m_[13] - right->v_.m_[13], left->v_.m_[14] - right->v_.m_[14], left->v_.m_[15] - right->v_.m_[15]);
        break;
      default:
        return -1;
    }
  }
  else if (left->kind_ == sletk_float) {
    switch (right->kind_) {
      case sletk_vec2:
        sl_expr_temp_init_vec2(&tmp, left->v_.f_ - right->v_.v_[0], 
                                     left->v_.f_ - right->v_.v_[1]);
        break;
      case sletk_vec3:
        sl_expr_temp_init_vec3(&tmp, left->v_.f_- right->v_.v_[0], 
                                     left->v_.f_ - right->v_.v_[1], 
                                     left->v_.f_ - right->v_.v_[2]);
        break;
      case sletk_vec4:
        sl_expr_temp_init_vec4(&tmp, left->v_.f_ - right->v_.v_[0], 
                                     left->v_.f_ - right->v_.v_[1], 
                                     left->v_.f_ - right->v_.v_[2], 
                                     left->v_.f_ - right->v_.v_[3]);
        break;
      case sletk_mat2:
        sl_expr_temp_init_mat2(&tmp, left->v_.f_ - right->v_.m_[0], left->v_.f_ - right->v_.m_[1], 
                                     left->v_.f_ - right->v_.m_[2], left->v_.f_ - right->v_.m_[3]);
        break;
      case sletk_mat3:
        sl_expr_temp_init_mat3(&tmp, left->v_.f_ - right->v_.m_[0], left->v_.f_ - right->v_.m_[1], left->v_.f_ - right->v_.m_[2], 
                                     left->v_.f_ - right->v_.m_[3], left->v_.f_ - right->v_.m_[4], left->v_.f_ - right->v_.m_[5], 
                                     left->v_.f_ - right->v_.m_[6], left->v_.f_ - right->v_.m_[7], left->v_.f_ - right->v_.m_[8]);
        break;
      case sletk_mat4:
        sl_expr_temp_init_mat4(&tmp, left->v_.f_ - right->v_.m_[0], left->v_.f_ - right->v_.m_[1], left->v_.f_ - right->v_.m_[2], left->v_.f_ - right->v_.m_[3], 
                                     left->v_.f_ - right->v_.m_[4], left->v_.f_ - right->v_.m_[5], left->v_.f_ - right->v_.m_[6], left->v_.f_ - right->v_.m_[7], 
                                     left->v_.f_ - right->v_.m_[8], left->v_.f_ - right->v_.m_[9], left->v_.f_ - right->v_.m_[10], left->v_.f_ - right->v_.m_[11], 
                                     left->v_.f_ - right->v_.m_[12], left->v_.f_ - right->v_.m_[13], left->v_.f_ - right->v_.m_[14], left->v_.f_ - right->v_.m_[15]);
        break;
      default:
        return -1;
    }
  }
  else if (left->kind_ == sletk_int) {
    switch (right->kind_) {
      case sletk_ivec2:
        sl_expr_temp_init_ivec2(&tmp, left->v_.i_ - right->v_.iv_[0], 
                                      left->v_.i_ - right->v_.iv_[1]);
        break;
      case sletk_ivec3:
        sl_expr_temp_init_ivec3(&tmp, left->v_.i_ - right->v_.iv_[0], 
                                      left->v_.i_ - right->v_.iv_[1], 
                                      left->v_.i_ - right->v_.iv_[2]);
        break;
      case sletk_ivec4:
        sl_expr_temp_init_ivec4(&tmp, left->v_.i_ - right->v_.iv_[0], 
                                      left->v_.i_ - right->v_.iv_[1], 
                                      left->v_.i_ - right->v_.iv_[2], 
                                      left->v_.i_ - right->v_.iv_[3]);
        break;
      default:
        return -1;
    }
  }
  else if ((left->kind_ == sletk_vec2) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_vec2(&tmp, left->v_.v_[0] - right->v_.f_, 
                                 left->v_.v_[1] - right->v_.f_);
  }
  else if ((left->kind_ == sletk_vec3) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_vec3(&tmp, left->v_.v_[0] - right->v_.f_, 
                                 left->v_.v_[1] - right->v_.f_, 
                                 left->v_.v_[2] - right->v_.f_);
  }
  else if ((left->kind_ == sletk_vec4) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_vec4(&tmp, left->v_.v_[0] - right->v_.f_, 
                                 left->v_.v_[1] - right->v_.f_, 
                                 left->v_.v_[2] - right->v_.f_, 
                                 left->v_.v_[3] - right->v_.f_);
  }
  else if ((left->kind_ == sletk_ivec2) && (right->kind_ == sletk_int)) {
    sl_expr_temp_init_ivec2(&tmp, left->v_.iv_[0] - right->v_.i_,
                                  left->v_.iv_[1] - right->v_.i_);
  }
  else if ((left->kind_ == sletk_ivec3) && (right->kind_ == sletk_int)) {
    sl_expr_temp_init_ivec3(&tmp, left->v_.iv_[0] - right->v_.i_,
                                  left->v_.iv_[1] - right->v_.i_,
                                  left->v_.iv_[2] - right->v_.i_);
  }
  else if ((left->kind_ == sletk_ivec4) && (right->kind_ == sletk_int)) {
    sl_expr_temp_init_ivec4(&tmp, left->v_.iv_[0] - right->v_.i_,
                                  left->v_.iv_[1] - right->v_.i_,
                                  left->v_.iv_[2] - right->v_.i_,
                                  left->v_.iv_[3] - right->v_.i_);
  }
  else if ((left->kind_ == sletk_mat2) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_mat2(&tmp, left->v_.m_[0] - right->v_.f_, left->v_.m_[1] - right->v_.f_,
                                 left->v_.m_[2] - right->v_.f_, left->v_.m_[3] - right->v_.f_);
  }
  else if ((left->kind_ == sletk_mat3) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_mat3(&tmp, left->v_.m_[0] - right->v_.f_, left->v_.m_[1] - right->v_.f_, left->v_.m_[2] - right->v_.f_,
                                 left->v_.m_[3] - right->v_.f_, left->v_.m_[4] - right->v_.f_, left->v_.m_[5] - right->v_.f_,
                                 left->v_.m_[6] - right->v_.f_, left->v_.m_[7] - right->v_.f_, left->v_.m_[8] - right->v_.f_);
  }
  else if ((left->kind_ == sletk_mat4) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_mat4(&tmp, left->v_.m_[0] - right->v_.f_, left->v_.m_[1] - right->v_.f_, left->v_.m_[2] - right->v_.f_, left->v_.m_[3] - right->v_.f_,
                                 left->v_.m_[4] - right->v_.f_, left->v_.m_[5] - right->v_.f_, left->v_.m_[6] - right->v_.f_, left->v_.m_[7] - right->v_.f_,
                                 left->v_.m_[8] - right->v_.f_, left->v_.m_[9] - right->v_.f_, left->v_.m_[10] - right->v_.f_, left->v_.m_[11] - right->v_.f_,
                                 left->v_.m_[12] - right->v_.f_, left->v_.m_[13] - right->v_.f_, left->v_.m_[14] - right->v_.f_, left->v_.m_[15] - right->v_.f_);
  }
  else {
    return -1;
  }

  if (sl_expr_temp_copy(dst, &tmp)) {
    sl_expr_temp_cleanup(&tmp);
    return -1;
  }
  sl_expr_temp_cleanup(&tmp);
  return 0;
}

int sl_expr_temp_div(struct sl_expr_temp *dst, const struct sl_expr_temp *left, const struct sl_expr_temp *right) {
  struct sl_expr_temp tmp;
  if (left->kind_ == right->kind_) {
    switch (left->kind_) {
      case sletk_float:
        sl_expr_temp_init_float(&tmp, left->v_.f_ / right->v_.f_);
        break;
      case sletk_int:
        sl_expr_temp_init_int(&tmp, left->v_.i_ / right->v_.i_);
        break;
      case sletk_vec2:
        sl_expr_temp_init_vec2(&tmp, left->v_.v_[0] / right->v_.v_[0], 
                                     left->v_.v_[1] / right->v_.v_[1]);
        break;
      case sletk_vec3:
        sl_expr_temp_init_vec3(&tmp, left->v_.v_[0] / right->v_.v_[0], 
                                     left->v_.v_[1] / right->v_.v_[1], 
                                     left->v_.v_[2] / right->v_.v_[2]);
        break;
      case sletk_vec4:
        sl_expr_temp_init_vec4(&tmp, left->v_.v_[0] / right->v_.v_[0], 
                                     left->v_.v_[1] / right->v_.v_[1], 
                                     left->v_.v_[2] / right->v_.v_[2], 
                                     left->v_.v_[3] / right->v_.v_[3]);
        break;
      case sletk_ivec2:
        sl_expr_temp_init_ivec2(&tmp, left->v_.iv_[0] / right->v_.iv_[0], 
                                      left->v_.iv_[1] / right->v_.iv_[1]);
        break;
      case sletk_ivec3:
        sl_expr_temp_init_ivec3(&tmp, left->v_.iv_[0] / right->v_.iv_[0], 
                                      left->v_.iv_[1] / right->v_.iv_[1], 
                                      left->v_.iv_[2] / right->v_.iv_[2]);
        break;
      case sletk_ivec4:
        sl_expr_temp_init_ivec4(&tmp, left->v_.iv_[0] / right->v_.iv_[0], 
                                      left->v_.iv_[1] / right->v_.iv_[1], 
                                      left->v_.iv_[2] / right->v_.iv_[2], 
                                      left->v_.iv_[3] / right->v_.iv_[3]);
        break;
      case sletk_mat2:
        sl_expr_temp_init_mat2(&tmp, left->v_.m_[0] / right->v_.m_[0], left->v_.m_[1] / right->v_.m_[1], 
                                     left->v_.m_[2] / right->v_.m_[2], left->v_.m_[3] / right->v_.m_[3]);
        break;
      case sletk_mat3:
        sl_expr_temp_init_mat3(&tmp, left->v_.m_[0] / right->v_.m_[0], left->v_.m_[1] / right->v_.m_[1], left->v_.m_[2] / right->v_.m_[2], 
                                     left->v_.m_[3] / right->v_.m_[3], left->v_.m_[4] / right->v_.m_[4], left->v_.m_[5] / right->v_.m_[5], 
                                     left->v_.m_[6] / right->v_.m_[6], left->v_.m_[7] / right->v_.m_[7], left->v_.m_[8] / right->v_.m_[8]);
        break;
      case sletk_mat4:
        sl_expr_temp_init_mat4(&tmp, left->v_.m_[0] / right->v_.m_[0], left->v_.m_[1] / right->v_.m_[1], left->v_.m_[2] / right->v_.m_[2], left->v_.m_[3] / right->v_.m_[3], 
                                     left->v_.m_[4] / right->v_.m_[4], left->v_.m_[5] / right->v_.m_[5], left->v_.m_[6] / right->v_.m_[6], left->v_.m_[7] / right->v_.m_[7], 
                                     left->v_.m_[8] / right->v_.m_[8], left->v_.m_[9] / right->v_.m_[9], left->v_.m_[10] / right->v_.m_[10], left->v_.m_[11] / right->v_.m_[11], 
                                     left->v_.m_[12] / right->v_.m_[12], left->v_.m_[13] / right->v_.m_[13], left->v_.m_[14] / right->v_.m_[14], left->v_.m_[15] / right->v_.m_[15]);
        break;
      default:
        return -1;
    }
  }
  else if (left->kind_ == sletk_float) {
    switch (right->kind_) {
      case sletk_vec2:
        sl_expr_temp_init_vec2(&tmp, left->v_.f_ / right->v_.v_[0], 
                                     left->v_.f_ / right->v_.v_[1]);
        break;
      case sletk_vec3:
        sl_expr_temp_init_vec3(&tmp, left->v_.f_ / right->v_.v_[0], 
                                     left->v_.f_ / right->v_.v_[1], 
                                     left->v_.f_ / right->v_.v_[2]);
        break;
      case sletk_vec4:
        sl_expr_temp_init_vec4(&tmp, left->v_.f_ / right->v_.v_[0], 
                                     left->v_.f_ / right->v_.v_[1], 
                                     left->v_.f_ / right->v_.v_[2], 
                                     left->v_.f_ / right->v_.v_[3]);
        break;
      case sletk_mat2:
        sl_expr_temp_init_mat2(&tmp, left->v_.f_ / right->v_.m_[0], left->v_.f_ / right->v_.m_[1], 
                                     left->v_.f_ / right->v_.m_[2], left->v_.f_ / right->v_.m_[3]);
        break;
      case sletk_mat3:
        sl_expr_temp_init_mat3(&tmp, left->v_.f_ / right->v_.m_[0], left->v_.f_ / right->v_.m_[1], left->v_.f_ / right->v_.m_[2], 
                                     left->v_.f_ / right->v_.m_[3], left->v_.f_ / right->v_.m_[4], left->v_.f_ / right->v_.m_[5], 
                                     left->v_.f_ / right->v_.m_[6], left->v_.f_ / right->v_.m_[7], left->v_.f_ / right->v_.m_[8]);
        break;
      case sletk_mat4:
        sl_expr_temp_init_mat4(&tmp, left->v_.f_ / right->v_.m_[0], left->v_.f_ / right->v_.m_[1], left->v_.f_ / right->v_.m_[2], left->v_.f_ / right->v_.m_[3], 
                                     left->v_.f_ / right->v_.m_[4], left->v_.f_ / right->v_.m_[5], left->v_.f_ / right->v_.m_[6], left->v_.f_ / right->v_.m_[7], 
                                     left->v_.f_ / right->v_.m_[8], left->v_.f_ / right->v_.m_[9], left->v_.f_ / right->v_.m_[10], left->v_.f_ / right->v_.m_[11], 
                                     left->v_.f_ / right->v_.m_[12], left->v_.f_ / right->v_.m_[13], left->v_.f_ / right->v_.m_[14], left->v_.f_ / right->v_.m_[15]);
        break;
      default:
        return -1;
    }
  }
  else if (left->kind_ == sletk_int) {
    switch (right->kind_) {
      case sletk_ivec2:
        sl_expr_temp_init_ivec2(&tmp, left->v_.i_ / right->v_.iv_[0], 
                                      left->v_.i_ / right->v_.iv_[1]);
        break;
      case sletk_ivec3:
        sl_expr_temp_init_ivec3(&tmp, left->v_.i_ / right->v_.iv_[0], 
                                      left->v_.i_ / right->v_.iv_[1], 
                                      left->v_.i_ / right->v_.iv_[2]);
        break;
      case sletk_ivec4:
        sl_expr_temp_init_ivec4(&tmp, left->v_.i_ / right->v_.iv_[0], 
                                      left->v_.i_ / right->v_.iv_[1], 
                                      left->v_.i_ / right->v_.iv_[2], 
                                      left->v_.i_ / right->v_.iv_[3]);
        break;
      default:
        return -1;
    }
  }
  else if ((left->kind_ == sletk_vec2) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_vec2(&tmp, left->v_.v_[0] / right->v_.f_, 
                                 left->v_.v_[1] / right->v_.f_);
  }
  else if ((left->kind_ == sletk_vec3) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_vec3(&tmp, left->v_.v_[0] / right->v_.f_, 
                                 left->v_.v_[1] / right->v_.f_, 
                                 left->v_.v_[2] / right->v_.f_);
  }
  else if ((left->kind_ == sletk_vec4) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_vec4(&tmp, left->v_.v_[0] / right->v_.f_, 
                                 left->v_.v_[1] / right->v_.f_, 
                                 left->v_.v_[2] / right->v_.f_, 
                                 left->v_.v_[3] / right->v_.f_);
  }
  else if ((left->kind_ == sletk_ivec2) && (right->kind_ == sletk_int)) {
    sl_expr_temp_init_ivec2(&tmp, left->v_.iv_[0] / right->v_.i_,
                                  left->v_.iv_[1] / right->v_.i_);
  }
  else if ((left->kind_ == sletk_ivec3) && (right->kind_ == sletk_int)) {
    sl_expr_temp_init_ivec3(&tmp, left->v_.iv_[0] / right->v_.i_,
                                  left->v_.iv_[1] / right->v_.i_,
                                  left->v_.iv_[2] / right->v_.i_);
  }
  else if ((left->kind_ == sletk_ivec4) && (right->kind_ == sletk_int)) {
    sl_expr_temp_init_ivec4(&tmp, left->v_.iv_[0] / right->v_.i_,
                                  left->v_.iv_[1] / right->v_.i_,
                                  left->v_.iv_[2] / right->v_.i_,
                                  left->v_.iv_[3] / right->v_.i_);
  }
  else if ((left->kind_ == sletk_mat2) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_mat2(&tmp, left->v_.m_[0] / right->v_.f_, left->v_.m_[1] / right->v_.f_,
                                 left->v_.m_[2] / right->v_.f_, left->v_.m_[3] / right->v_.f_);
  }
  else if ((left->kind_ == sletk_mat3) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_mat3(&tmp, left->v_.m_[0] / right->v_.f_, left->v_.m_[1] / right->v_.f_, left->v_.m_[2] / right->v_.f_,
                                 left->v_.m_[3] / right->v_.f_, left->v_.m_[4] / right->v_.f_, left->v_.m_[5] / right->v_.f_,
                                 left->v_.m_[6] / right->v_.f_, left->v_.m_[7] / right->v_.f_, left->v_.m_[8] / right->v_.f_);
  }
  else if ((left->kind_ == sletk_mat4) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_mat4(&tmp, left->v_.m_[0] / right->v_.f_, left->v_.m_[1] / right->v_.f_, left->v_.m_[2] / right->v_.f_, left->v_.m_[3] / right->v_.f_,
                                 left->v_.m_[4] / right->v_.f_, left->v_.m_[5] / right->v_.f_, left->v_.m_[6] / right->v_.f_, left->v_.m_[7] / right->v_.f_,
                                 left->v_.m_[8] / right->v_.f_, left->v_.m_[9] / right->v_.f_, left->v_.m_[10] / right->v_.f_, left->v_.m_[11] / right->v_.f_,
                                 left->v_.m_[12] / right->v_.f_, left->v_.m_[13] / right->v_.f_, left->v_.m_[14] / right->v_.f_, left->v_.m_[15] / right->v_.f_);
  }
  else {
    return -1;
  }

  if (sl_expr_temp_copy(dst, &tmp)) {
    sl_expr_temp_cleanup(&tmp);
    return -1;
  }
  sl_expr_temp_cleanup(&tmp);
  return 0;
}

int sl_expr_temp_mul(struct sl_expr_temp *dst, const struct sl_expr_temp *left, const struct sl_expr_temp *right) {
  struct sl_expr_temp tmp;
  float m[16];
  int row, col;
  if (left->kind_ == right->kind_) {
    switch (left->kind_) {
      case sletk_float:
        sl_expr_temp_init_float(&tmp, left->v_.f_ * right->v_.f_);
        break;
      case sletk_int:
        sl_expr_temp_init_int(&tmp, left->v_.i_ * right->v_.i_);
        break;
      case sletk_vec2:
        sl_expr_temp_init_vec2(&tmp, left->v_.v_[0] * right->v_.v_[0], 
                                     left->v_.v_[1] * right->v_.v_[1]);
        break;
      case sletk_vec3:
        sl_expr_temp_init_vec3(&tmp, left->v_.v_[0] * right->v_.v_[0], 
                                     left->v_.v_[1] * right->v_.v_[1], 
                                     left->v_.v_[2] * right->v_.v_[2]);
        break;
      case sletk_vec4:
        sl_expr_temp_init_vec4(&tmp, left->v_.v_[0] * right->v_.v_[0], 
                                     left->v_.v_[1] * right->v_.v_[1], 
                                     left->v_.v_[2] * right->v_.v_[2], 
                                     left->v_.v_[3] * right->v_.v_[3]);
        break;
      case sletk_ivec2:
        sl_expr_temp_init_ivec2(&tmp, left->v_.iv_[0] * right->v_.iv_[0], 
                                      left->v_.iv_[1] * right->v_.iv_[1]);
        break;
      case sletk_ivec3:
        sl_expr_temp_init_ivec3(&tmp, left->v_.iv_[0] * right->v_.iv_[0], 
                                      left->v_.iv_[1] * right->v_.iv_[1], 
                                      left->v_.iv_[2] * right->v_.iv_[2]);
        break;
      case sletk_ivec4:
        sl_expr_temp_init_ivec4(&tmp, left->v_.iv_[0] * right->v_.iv_[0], 
                                      left->v_.iv_[1] * right->v_.iv_[1], 
                                      left->v_.iv_[2] * right->v_.iv_[2], 
                                      left->v_.iv_[3] * right->v_.iv_[3]);
        break;
      case sletk_mat2:
        for (col = 0; col < 2; ++col) {
          for (row = 0; row < 2; ++row) {
            m[col * 2 + row] = left->v_.m_[row + 0] * right->v_.m_[col * 2 + 0] +
                               left->v_.m_[row + 2] * right->v_.m_[col * 2 + 1];
          }
        }
        sl_expr_temp_init_mat2(&tmp, m[0], m[1], m[2], m[3]);
        break;
      case sletk_mat3:
        for (col = 0; col < 3; ++col) {
          for (row = 0; row < 3; ++row) {
            m[col * 3 + row] = left->v_.m_[row + 0] * right->v_.m_[col * 3 + 0] +
                               left->v_.m_[row + 3] * right->v_.m_[col * 3 + 1] +
                               left->v_.m_[row + 6] * right->v_.m_[col * 3 + 2];
          }
        }
        sl_expr_temp_init_mat3(&tmp, m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8]);
        break;
      case sletk_mat4:
        for (col = 0; col < 3; ++col) {
          for (row = 0; row < 3; ++row) {
            m[col * 4 + row] = left->v_.m_[row + 0] * right->v_.m_[col * 4 + 0] +
                               left->v_.m_[row + 4] * right->v_.m_[col * 4 + 1] +
                               left->v_.m_[row + 8] * right->v_.m_[col * 4 + 2] +
                               left->v_.m_[row + 12] * right->v_.m_[col * 4 + 3];
          }
        }
        sl_expr_temp_init_mat4(&tmp, m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15]);
        break;
      default:
        return -1;
    }
  }
  else if (left->kind_ == sletk_float) {
    switch (right->kind_) {
      case sletk_vec2:
        sl_expr_temp_init_vec2(&tmp, left->v_.f_ * right->v_.v_[0], 
                                     left->v_.f_ * right->v_.v_[1]);
        break;
      case sletk_vec3:
        sl_expr_temp_init_vec3(&tmp, left->v_.f_ * right->v_.v_[0], 
                                     left->v_.f_ * right->v_.v_[1], 
                                     left->v_.f_ * right->v_.v_[2]);
        break;
      case sletk_vec4:
        sl_expr_temp_init_vec4(&tmp, left->v_.f_ * right->v_.v_[0], 
                                     left->v_.f_ * right->v_.v_[1], 
                                     left->v_.f_ * right->v_.v_[2], 
                                     left->v_.f_ * right->v_.v_[3]);
        break;
      case sletk_mat2:
        sl_expr_temp_init_mat2(&tmp, left->v_.f_ * right->v_.m_[0], left->v_.f_ * right->v_.m_[1], 
                                     left->v_.f_ * right->v_.m_[2], left->v_.f_ * right->v_.m_[3]);
        break;
      case sletk_mat3:
        sl_expr_temp_init_mat3(&tmp, left->v_.f_ * right->v_.m_[0], left->v_.f_ * right->v_.m_[1], left->v_.f_ * right->v_.m_[2], 
                                     left->v_.f_ * right->v_.m_[3], left->v_.f_ * right->v_.m_[4], left->v_.f_ * right->v_.m_[5], 
                                     left->v_.f_ * right->v_.m_[6], left->v_.f_ * right->v_.m_[7], left->v_.f_ * right->v_.m_[8]);
        break;
      case sletk_mat4:
        sl_expr_temp_init_mat4(&tmp, left->v_.f_ * right->v_.m_[0], left->v_.f_ * right->v_.m_[1], left->v_.f_ * right->v_.m_[2], left->v_.f_ * right->v_.m_[3], 
                                     left->v_.f_ * right->v_.m_[4], left->v_.f_ * right->v_.m_[5], left->v_.f_ * right->v_.m_[6], left->v_.f_ * right->v_.m_[7], 
                                     left->v_.f_ * right->v_.m_[8], left->v_.f_ * right->v_.m_[9], left->v_.f_ * right->v_.m_[10], left->v_.f_ * right->v_.m_[11], 
                                     left->v_.f_ * right->v_.m_[12], left->v_.f_ * right->v_.m_[13], left->v_.f_ * right->v_.m_[14], left->v_.f_ * right->v_.m_[15]);
        break;
      default:
        return -1;
    }
  }
  else if (left->kind_ == sletk_int) {
    switch (right->kind_) {
      case sletk_ivec2:
        sl_expr_temp_init_ivec2(&tmp, left->v_.i_ * right->v_.iv_[0], 
                                      left->v_.i_ * right->v_.iv_[1]);
        break;
      case sletk_ivec3:
        sl_expr_temp_init_ivec3(&tmp, left->v_.i_ * right->v_.iv_[0], 
                                      left->v_.i_ * right->v_.iv_[1], 
                                      left->v_.i_ * right->v_.iv_[2]);
        break;
      case sletk_ivec4:
        sl_expr_temp_init_ivec4(&tmp, left->v_.i_ * right->v_.iv_[0], 
                                      left->v_.i_ * right->v_.iv_[1], 
                                      left->v_.i_ * right->v_.iv_[2], 
                                      left->v_.i_ * right->v_.iv_[3]);
        break;
      default:
        return -1;
    }
  }
  else if ((left->kind_ == sletk_vec2) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_vec2(&tmp, left->v_.v_[0] * right->v_.f_, 
                                 left->v_.v_[1] * right->v_.f_);
  }
  else if ((left->kind_ == sletk_vec2) && (right->kind_ == sletk_mat2)) {
    m[0] = left->v_.v_[0] * right->v_.m_[0] + left->v_.v_[1] * right->v_.m_[1];
    m[1] = left->v_.v_[0] * right->v_.m_[2] + left->v_.v_[1] * right->v_.m_[3];
    sl_expr_temp_init_vec2(&tmp, m[0], m[1]);
  }
  else if ((left->kind_ == sletk_vec3) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_vec3(&tmp, left->v_.v_[0] * right->v_.f_, 
                                 left->v_.v_[1] * right->v_.f_, 
                                 left->v_.v_[2] * right->v_.f_);
  }
  else if ((left->kind_ == sletk_vec3) && (right->kind_ == sletk_mat3)) {
    m[0] = left->v_.v_[0] * right->v_.m_[0] + left->v_.v_[1] * right->v_.m_[1] + left->v_.v_[2] * right->v_.m_[2];
    m[1] = left->v_.v_[0] * right->v_.m_[3] + left->v_.v_[1] * right->v_.m_[4] + left->v_.v_[2] * right->v_.m_[5];
    m[2] = left->v_.v_[0] * right->v_.m_[6] + left->v_.v_[1] * right->v_.m_[7] + left->v_.v_[2] * right->v_.m_[8];
    sl_expr_temp_init_vec3(&tmp, m[0], m[1], m[2]);
  }
  else if ((left->kind_ == sletk_vec4) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_vec4(&tmp, left->v_.v_[0] * right->v_.f_, 
                                 left->v_.v_[1] * right->v_.f_, 
                                 left->v_.v_[2] * right->v_.f_, 
                                 left->v_.v_[3] * right->v_.f_);
  }
  else if ((left->kind_ == sletk_vec4) && (right->kind_ == sletk_mat4)) {
    m[0] = left->v_.v_[0] * right->v_.m_[0]  + left->v_.v_[1] * right->v_.m_[1]  + left->v_.v_[2] * right->v_.m_[2]  + left->v_.v_[3] * right->v_.m_[3];
    m[1] = left->v_.v_[0] * right->v_.m_[4]  + left->v_.v_[1] * right->v_.m_[5]  + left->v_.v_[2] * right->v_.m_[6]  + left->v_.v_[3] * right->v_.m_[7];
    m[2] = left->v_.v_[0] * right->v_.m_[8]  + left->v_.v_[1] * right->v_.m_[9]  + left->v_.v_[2] * right->v_.m_[10] + left->v_.v_[3] * right->v_.m_[11];
    m[3] = left->v_.v_[0] * right->v_.m_[12] + left->v_.v_[1] * right->v_.m_[13] + left->v_.v_[2] * right->v_.m_[14] + left->v_.v_[3] * right->v_.m_[15];
    sl_expr_temp_init_vec4(&tmp, m[0], m[1], m[2], m[3]);
  }
  else if ((left->kind_ == sletk_ivec2) && (right->kind_ == sletk_int)) {
    sl_expr_temp_init_ivec2(&tmp, left->v_.iv_[0] * right->v_.i_,
                                  left->v_.iv_[1] * right->v_.i_);
  }
  else if ((left->kind_ == sletk_ivec3) && (right->kind_ == sletk_int)) {
    sl_expr_temp_init_ivec3(&tmp, left->v_.iv_[0] * right->v_.i_,
                                  left->v_.iv_[1] * right->v_.i_,
                                  left->v_.iv_[2] * right->v_.i_);
  }
  else if ((left->kind_ == sletk_ivec4) && (right->kind_ == sletk_int)) {
    sl_expr_temp_init_ivec4(&tmp, left->v_.iv_[0] * right->v_.i_,
                                  left->v_.iv_[1] * right->v_.i_,
                                  left->v_.iv_[2] * right->v_.i_,
                                  left->v_.iv_[3] * right->v_.i_);
  }
  else if ((left->kind_ == sletk_mat2) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_mat2(&tmp, left->v_.m_[0] * right->v_.f_, left->v_.m_[1] * right->v_.f_,
                                 left->v_.m_[2] * right->v_.f_, left->v_.m_[3] * right->v_.f_);
  }
  else if ((left->kind_ == sletk_mat2) && (right->kind_ == sletk_vec2)) {
    m[0] = left->v_.m_[0] * right->v_.v_[0] + left->v_.m_[2] * right->v_.v_[1];
    m[1] = left->v_.m_[1] * right->v_.v_[0] + left->v_.m_[3] * right->v_.v_[1];
    sl_expr_temp_init_vec2(&tmp, m[0], m[1]);
  }
  else if ((left->kind_ == sletk_mat3) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_mat3(&tmp, left->v_.m_[0] * right->v_.f_, left->v_.m_[1] * right->v_.f_, left->v_.m_[2] * right->v_.f_,
                                 left->v_.m_[3] * right->v_.f_, left->v_.m_[4] * right->v_.f_, left->v_.m_[5] * right->v_.f_,
                                 left->v_.m_[6] * right->v_.f_, left->v_.m_[7] * right->v_.f_, left->v_.m_[8] * right->v_.f_);
  }
  else if ((left->kind_ == sletk_mat3) && (right->kind_ == sletk_vec3)) {
    m[0] = left->v_.m_[0] * right->v_.v_[0] + left->v_.m_[3] * right->v_.v_[1] + left->v_.m_[6] * right->v_.v_[2];
    m[1] = left->v_.m_[1] * right->v_.v_[0] + left->v_.m_[4] * right->v_.v_[1] + left->v_.m_[7] * right->v_.v_[2];
    m[2] = left->v_.m_[2] * right->v_.v_[0] + left->v_.m_[5] * right->v_.v_[1] + left->v_.m_[8] * right->v_.v_[2];
    sl_expr_temp_init_vec3(&tmp, m[0], m[1], m[2]);
  }
  else if ((left->kind_ == sletk_mat4) && (right->kind_ == sletk_float)) {
    sl_expr_temp_init_mat4(&tmp, left->v_.m_[0] * right->v_.f_, left->v_.m_[1] * right->v_.f_, left->v_.m_[2] * right->v_.f_, left->v_.m_[3] * right->v_.f_,
                                 left->v_.m_[4] * right->v_.f_, left->v_.m_[5] * right->v_.f_, left->v_.m_[6] * right->v_.f_, left->v_.m_[7] * right->v_.f_,
                                 left->v_.m_[8] * right->v_.f_, left->v_.m_[9] * right->v_.f_, left->v_.m_[10] * right->v_.f_, left->v_.m_[11] * right->v_.f_,
                                 left->v_.m_[12] * right->v_.f_, left->v_.m_[13] * right->v_.f_, left->v_.m_[14] * right->v_.f_, left->v_.m_[15] * right->v_.f_);
  }
  else if ((left->kind_ == sletk_mat4) && (right->kind_ == sletk_vec4)) {
    m[0] = left->v_.m_[0] * right->v_.v_[0] + left->v_.m_[4] * right->v_.v_[1] + left->v_.m_[8] * right->v_.v_[2] + left->v_.m_[12] * right->v_.v_[3];
    m[1] = left->v_.m_[1] * right->v_.v_[0] + left->v_.m_[5] * right->v_.v_[1] + left->v_.m_[9] * right->v_.v_[2] + left->v_.m_[13] * right->v_.v_[3];
    m[2] = left->v_.m_[2] * right->v_.v_[0] + left->v_.m_[6] * right->v_.v_[1] + left->v_.m_[10] * right->v_.v_[2] + left->v_.m_[14] * right->v_.v_[3];
    m[3] = left->v_.m_[3] * right->v_.v_[0] + left->v_.m_[7] * right->v_.v_[1] + left->v_.m_[11] * right->v_.v_[2] + left->v_.m_[15] * right->v_.v_[3];
    sl_expr_temp_init_vec4(&tmp, m[0], m[1], m[2], m[3]);
  }
  else {
    return -1;
  }

  if (sl_expr_temp_copy(dst, &tmp)) {
    sl_expr_temp_cleanup(&tmp);
    return -1;
  }
  sl_expr_temp_cleanup(&tmp);
  return 0;
}

int sl_expr_temp_lt(struct sl_expr_temp *dst, const struct sl_expr_temp *left, const struct sl_expr_temp *right) {
  struct sl_expr_temp tmp;
  if (left->kind_ != right->kind_) return -1;
  if (left->kind_ == sletk_float) {
    sl_expr_temp_init_bool(&tmp, left->v_.f_ < right->v_.f_);
  }
  else if (left->kind_ == sletk_int) {
    sl_expr_temp_init_bool(&tmp, left->v_.i_ < right->v_.i_);
  }
  else {
    return -1;
  }

  if (sl_expr_temp_copy(dst, &tmp)) {
    sl_expr_temp_cleanup(&tmp);
    return -1;
  }
  sl_expr_temp_cleanup(&tmp);
  return 0;

}

int sl_expr_temp_le(struct sl_expr_temp *dst, const struct sl_expr_temp *left, const struct sl_expr_temp *right) {
  struct sl_expr_temp tmp;
  if (left->kind_ != right->kind_) return -1;
  if (left->kind_ == sletk_float) {
    sl_expr_temp_init_bool(&tmp, left->v_.f_ <= right->v_.f_);
  }
  else if (left->kind_ == sletk_int) {
    sl_expr_temp_init_bool(&tmp, left->v_.i_ <= right->v_.i_);
  }
  else {
    return -1;
  }

  if (sl_expr_temp_copy(dst, &tmp)) {
    sl_expr_temp_cleanup(&tmp);
    return -1;
  }
  sl_expr_temp_cleanup(&tmp);
  return 0;

}

int sl_expr_temp_gt(struct sl_expr_temp *dst, const struct sl_expr_temp *left, const struct sl_expr_temp *right) {
  struct sl_expr_temp tmp;
  if (left->kind_ != right->kind_) return -1;
  if (left->kind_ == sletk_float) {
    sl_expr_temp_init_bool(&tmp, left->v_.f_ > right->v_.f_);
  }
  else if (left->kind_ == sletk_int) {
    sl_expr_temp_init_bool(&tmp, left->v_.i_ > right->v_.i_);
  }
  else {
    return -1;
  }

  if (sl_expr_temp_copy(dst, &tmp)) {
    sl_expr_temp_cleanup(&tmp);
    return -1;
  }
  sl_expr_temp_cleanup(&tmp);
  return 0;

}

int sl_expr_temp_ge(struct sl_expr_temp *dst, const struct sl_expr_temp *left, const struct sl_expr_temp *right) {
  struct sl_expr_temp tmp;
  if (left->kind_ != right->kind_) return -1;
  if (left->kind_ == sletk_float) {
    sl_expr_temp_init_bool(&tmp, left->v_.f_ >= right->v_.f_);
  }
  else if (left->kind_ == sletk_int) {
    sl_expr_temp_init_bool(&tmp, left->v_.i_ >= right->v_.i_);
  }
  else {
    return -1;
  }

  if (sl_expr_temp_copy(dst, &tmp)) {
    sl_expr_temp_cleanup(&tmp);
    return -1;
  }
  sl_expr_temp_cleanup(&tmp);
  return 0;

}

int sl_expr_temp_eq(struct sl_expr_temp *dst, const struct sl_expr_temp *left, const struct sl_expr_temp *right) {
  struct sl_expr_temp tmp;
  size_t n;
  
  if (left->kind_ != right->kind_) return -1;
  switch (left->kind_) {
    case sletk_float:
      sl_expr_temp_init_bool(&tmp, left->v_.f_ == right->v_.f_);
      break;
    case sletk_int:
      sl_expr_temp_init_bool(&tmp, left->v_.i_ == right->v_.i_);
      break;
    case sletk_vec2:
    case sletk_vec3:
    case sletk_vec4: {
      size_t num_comps = 2 + ((size_t)left->kind_) - (size_t)sletk_vec2;
      for (n = 0; n < num_comps; ++n) {
        if (left->v_.v_[n] != right->v_.v_[n]) {
          sl_expr_temp_init_bool(&tmp, 0);
          break;
        }
      }
      sl_expr_temp_init_bool(&tmp, 1);
      break;
    }
    case sletk_ivec2:
    case sletk_ivec3:
    case sletk_ivec4: {
      size_t num_comps = 2 + ((size_t)left->kind_) - (size_t)sletk_ivec2;
      for (n = 0; n < num_comps; ++n) {
        if (left->v_.iv_[n] != right->v_.iv_[n]) {
          sl_expr_temp_init_bool(&tmp, 0);
          break;
        }
      }
      sl_expr_temp_init_bool(&tmp, 1);
      break;
    }
    case sletk_bvec2:
    case sletk_bvec3:
    case sletk_bvec4: {
      size_t num_comps = 2 + ((size_t)left->kind_) - (size_t)sletk_bvec2;
      for (n = 0; n < num_comps; ++n) {
        if (left->v_.bv_[n] != right->v_.bv_[n]) {
          sl_expr_temp_init_bool(&tmp, 0);
          break;
        }
      }
      sl_expr_temp_init_bool(&tmp, 1);
      break;
    }
    case sletk_mat2:
    case sletk_mat3:
    case sletk_mat4: {
      size_t num_comps = 2 + ((size_t)left->kind_) - (size_t)sletk_mat2;
      num_comps *= num_comps;
      for (n = 0; n < num_comps; ++n) {
        if (left->v_.m_[n] != right->v_.m_[n]) {
          sl_expr_temp_init_bool(&tmp, 0);
          break;
        }
      }
      sl_expr_temp_init_bool(&tmp, 1);
      break;
    }
    case sletk_array:
    case sletk_struct: {
      if (sl_type_unqualified(left->v_.comp_.struct_or_array_type_) != sl_type_unqualified(left->v_.comp_.struct_or_array_type_)) {
        sl_expr_temp_init_bool(&tmp, 0);
        break;
      }
      if (left->v_.comp_.num_elements_ != right->v_.comp_.num_elements_) {
        sl_expr_temp_init_bool(&tmp, 0);
        break;
      }
      sl_expr_temp_init_bool(&tmp, 1);
      for (n = 0; n < left->v_.comp_.num_elements_; ++n) {
        int r;
        r = sl_expr_temp_eq(&tmp, &left->v_.comp_.elements_[n], &right->v_.comp_.elements_[n]);
        if (r) return r;
        if (!tmp.v_.b_) break;
      }
      break;
    }
    default:
      return -1;
  }

  if (sl_expr_temp_copy(dst, &tmp)) {
    sl_expr_temp_cleanup(&tmp);
    return -1;
  }
  sl_expr_temp_cleanup(&tmp);
  return 0;

}

int sl_expr_temp_ne(struct sl_expr_temp *dst, const struct sl_expr_temp *left, const struct sl_expr_temp *right) {
  struct sl_expr_temp tmp;
  int r;
  r = sl_expr_temp_eq(&tmp, left, right);
  if (r) return r;

  tmp.v_.b_ = !tmp.v_.b_;

  if (sl_expr_temp_copy(dst, &tmp)) {
    sl_expr_temp_cleanup(&tmp);
    return -1;
  }
  sl_expr_temp_cleanup(&tmp);
  return 0;

}
