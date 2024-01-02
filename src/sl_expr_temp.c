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

#ifndef SL_EXPR_TEMP_H_INCLUDED
#define SL_EXPR_TEMP_H_INCLUDED
#include "sl_expr_temp.h"
#endif

#ifndef SL_TYPES_H_INCLUDED
#define SL_TYPES_H_INCLUDED
#include "sl_types.h"
#endif

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
