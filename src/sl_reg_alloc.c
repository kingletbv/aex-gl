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

#ifndef SL_REG_ALLOC_H_INCLUDED
#define SL_REG_ALLOC_H_INCLUDED
#include "sl_reg_alloc.h"
#endif

#ifndef SL_TYPES_H_INCLUDED
#define SL_TYPES_H_INCLUDED
#include "sl_types.h"
#endif

static int sl_reg_alloc_get_cardinality(sl_reg_alloc_kind_t kind) {
  switch (kind) {
    case slrak_void:
      return 0;
    case slrak_array:
      return 0;
    case slrak_struct:
      return 0;
    case slrak_float:
      return 1;
    case slrak_int:
      return 1;
    case slrak_bool:
      return 1;
    case slrak_vec2:
      return 2;
    case slrak_vec3:
      return 3;
    case slrak_vec4:
      return 4;
    case slrak_bvec2:
      return 2;
    case slrak_bvec3:
      return 3;
    case slrak_bvec4:
      return 4;
    case slrak_ivec2:
      return 2;
    case slrak_ivec3:
      return 3;
    case slrak_ivec4:
      return 4;
    case slrak_mat2:
      return 4;
    case slrak_mat3:
      return 9;
    case slrak_mat4:
      return 16;
    case slrak_sampler2D:
      return 1;
    case slrak_samplerCube:
      return 1;
  }
  return 0;
}

void sl_reg_alloc_init(struct sl_reg_alloc *ra) {
  ra->kind_ = slrak_void;
  ra->offset_ = NULL;
  ra->rvalue_ = NULL;
}

int sl_reg_alloc_set_type(struct sl_reg_alloc *ra, const struct sl_type *t) {
  /* Clear out the old */
  if ((ra->kind_ == slrak_struct) || (ra->kind_ == slrak_array)) {
    sl_reg_alloc_cleanup(ra);
    sl_reg_alloc_init(ra);
  }
  if (!t) {
    /* special case for dud initialization short-hand */
    ra->kind_ = slrak_void;
    return 0;
  }
  struct sl_type *tunq = sl_type_unqualified(t);
  sl_reg_alloc_kind_t ra_kind = slrak_void;

  switch (tunq->kind_) {
    case sltk_array:
      ra_kind = slrak_array;
      break;
    case sltk_struct:
      ra_kind = slrak_struct;
      break;
    case sltk_void:
      ra_kind = slrak_void;
      break;
    case sltk_float:
      ra_kind = slrak_float;
      break;
    case sltk_int:
      ra_kind = slrak_int;
      break;
    case sltk_bool:
      ra_kind = slrak_bool;
      break;
    case sltk_vec2:
      ra_kind = slrak_vec2;
      break;
    case sltk_vec3:
      ra_kind = slrak_vec3;
      break;
    case sltk_vec4:
      ra_kind = slrak_vec4;
      break;
    case sltk_bvec2:
      ra_kind = slrak_bvec2;
      break;
    case sltk_bvec3:
      ra_kind = slrak_bvec3;
      break;
    case sltk_bvec4:
      ra_kind = slrak_bvec4;
      break;
    case sltk_ivec2:
      ra_kind = slrak_ivec2;
      break;
    case sltk_ivec3:
      ra_kind = slrak_ivec3;
      break;
    case sltk_ivec4:
      ra_kind = slrak_ivec4;
      break;
    case sltk_mat2:
      ra_kind = slrak_mat2;
      break;
    case sltk_mat3:
      ra_kind = slrak_mat3;
      break;
    case sltk_mat4:
      ra_kind = slrak_mat4;
      break;
    case sltk_sampler2D:
      ra_kind = slrak_sampler2D;
      break;
    case sltk_samplerCube:
      ra_kind = slrak_samplerCube;
      break;
  }
  size_t n;
  if (ra_kind == slrak_array) {
    ra->v_.array_.num_elements_ = tunq->array_size_;
    ra->v_.array_.element_type_ = tunq->derived_type_;
    ra->v_.array_.head_ = (struct sl_reg_alloc *)malloc(sizeof(struct sl_reg_alloc));
    if (!ra->v_.array_.head_) {
      /* No mem */
      return -1;
    }
    sl_reg_alloc_init(ra->v_.array_.head_);
    int r;
    r = sl_reg_alloc_set_type(ra->v_.array_.head_, tunq->derived_type_);
    if (r) {
      /* No memory or an overflow */
      sl_reg_alloc_cleanup(ra->v_.array_.head_);
      ra->v_.array_.head_ = NULL;
      return -1;
    }
  }
  else if (ra_kind == slrak_struct) {
    size_t num_fields = 0;
    struct sl_type_field *field = tunq->fields_;
    if (field) {
      do {
        field = field->chain_;
        ++num_fields;
      } while (field != tunq->fields_);
    }

    ra->v_.comp_.num_fields_ = num_fields;
    if (num_fields > (SIZE_MAX / sizeof(struct sl_reg_alloc))) {
      /* unlikely overflow. */
      return -1;
    }

    ra->v_.comp_.fields_ = (struct sl_reg_alloc *)malloc(sizeof(struct sl_reg_alloc) * ra->v_.comp_.num_fields_);
    if (!ra->v_.comp_.fields_) {
      /* enomem */
      return -1;
    }

    size_t field_index = 0;
    field = tunq->fields_;
    if (field) {
      do {
        field = field->chain_;

        int r;
        sl_reg_alloc_init(ra->v_.comp_.fields_ + field_index);
        r = sl_reg_alloc_set_type(ra->v_.comp_.fields_ + field_index, field->type_);
        if (r) {
          /* no memory, or an overflow */
          size_t n;
          for (n = 0; n < field_index; ++n) {
            sl_reg_alloc_cleanup(ra->v_.comp_.fields_ + n);
          }
          free(ra->v_.comp_.fields_);
          ra->v_.comp_.fields_ = NULL;

          return -1;
        }
        ++field_index;
      } while (field != tunq->fields_);
    }
    ra->v_.comp_.struct_type_ = tunq;
  }
  else {
    memset(ra->v_.regs_, 0, sizeof(ra->v_.regs_));
    size_t num_regs = sl_reg_alloc_get_cardinality(ra_kind);
    for (n = 0; n < num_regs; ++n) {
      ra->v_.regs_[n] = SL_REG_NONE;
    }
  }
  ra->kind_ = ra_kind;
  return 0;
}

void sl_reg_alloc_cleanup(struct sl_reg_alloc *ra) {
  if (ra->kind_ == slrak_struct) {
    size_t n;
    if (ra->v_.comp_.fields_) {
      for (n = 0; n < ra->v_.comp_.num_fields_; ++n) {
        sl_reg_alloc_cleanup(ra->v_.comp_.fields_ + n);
      }
      free(ra->v_.comp_.fields_);
      ra->v_.comp_.fields_ = NULL;
    }
  }
  else if (ra->kind_ == slrak_array) {
    if (ra->v_.array_.head_) {
      sl_reg_alloc_cleanup(ra->v_.array_.head_);
    }
    free(ra->v_.array_.head_);
  }
  if (ra->offset_) {
    sl_reg_alloc_cleanup(ra->offset_);
    free(ra->offset_);
  }
  if (ra->rvalue_) {
    sl_reg_alloc_cleanup(ra->rvalue_);
    free(ra->rvalue_);
  }
}

int sl_reg_alloc_is_allocated(const struct sl_reg_alloc *ra) {
  while ((ra->kind_ == slrak_array) || (ra->kind_ == slrak_struct)) {
    if (ra->kind_ == slrak_struct) {
      if (!ra->v_.comp_.fields_) return 1; /* no elements, ergo, nothing to allocate to begin with. */
      ra = ra->v_.comp_.fields_; /* tail recursion removed */
    }
    else if (ra->kind_ == slrak_array) {
      ra = ra->v_.array_.head_;
    }
  }

  return ra->v_.regs_[0] != SL_REG_NONE;
}

int sl_reg_alloc_clone(struct sl_reg_alloc *dst, const struct sl_reg_alloc *src) {
  int r = 0;

  dst->kind_ = src->kind_;
  if (src->kind_ == slrak_struct) {
    size_t n;
    if (!src->v_.comp_.fields_) {
      /* No memory, or no elements, something wrong with the source. */
      return -1;
    }
    struct sl_reg_alloc *new_elements;
    new_elements = (struct sl_reg_alloc *)malloc(sizeof(struct sl_reg_alloc) * src->v_.comp_.num_fields_);
    if (!new_elements) {
      /* no memory */
      return -1;
    }
    for (n = 0; n < src->v_.comp_.num_fields_; ++n) {
      int r;
      sl_reg_alloc_init(new_elements + n);
      r = sl_reg_alloc_clone(new_elements + n, src->v_.comp_.fields_ + n);
      if (r) {
        /* no memory, or an overflow */
        size_t k;
        for (k = 0; k < n; ++k) {
          sl_reg_alloc_cleanup(new_elements + k);
        }
        free(new_elements);
        return -1;
      }
    }
    sl_reg_alloc_cleanup(dst);
    sl_reg_alloc_init(dst);
    dst->kind_ = src->kind_;
    dst->v_.comp_.fields_= new_elements;
    dst->v_.comp_.num_fields_ = src->v_.comp_.num_fields_;
    dst->v_.comp_.struct_type_ = src->v_.comp_.struct_type_;
  }
  else if (src->kind_ == slrak_array) {
    struct sl_reg_alloc *new_element_ra = (struct sl_reg_alloc *)malloc(sizeof(struct sl_reg_alloc));
    if (!new_element_ra) {
      /* No memory */
      return -1;
    }
    sl_reg_alloc_init(new_element_ra);
    r = sl_reg_alloc_clone(new_element_ra, src->v_.array_.head_);
    if (r) {
      /* No mem */
      sl_reg_alloc_cleanup(new_element_ra);
      free(new_element_ra);
      return -1;
    }
    sl_reg_alloc_cleanup(dst);
    sl_reg_alloc_init(dst);
    dst->kind_ = src->kind_;
    dst->v_.array_.num_elements_ = src->v_.array_.num_elements_;
    dst->v_.array_.element_type_ = src->v_.array_.element_type_;
    dst->v_.array_.head_ = src->v_.array_.head_;
  }
  else {
    sl_reg_alloc_cleanup(dst);
    sl_reg_alloc_init(dst);
    dst->kind_ = src->kind_;
    memcpy(dst->v_.regs_, src->v_.regs_, sizeof(dst->v_.regs_));
  }
  if (src->offset_) {
    /* Clone offset */
    if (!dst->offset_) {
      dst->offset_ = (struct sl_reg_alloc *)malloc(sizeof(struct sl_reg_alloc));
      if (!dst->offset_) return -1; /* no mem */
      sl_reg_alloc_init(dst->offset_);
    }
    r = sl_reg_alloc_clone(dst->offset_, src->offset_);
    if (r) return r;
  }
  if (src->rvalue_) {
    /* Clone rvalue */
    if (!dst->rvalue_) {
      dst->rvalue_ = (struct sl_reg_alloc *)malloc(sizeof(struct sl_reg_alloc));
      if (!dst->rvalue_) return -1; /* no mem */
      sl_reg_alloc_init(dst->rvalue_);
    }
    r = sl_reg_alloc_clone(dst->rvalue_, src->rvalue_);
    if (r) return r;
  }
  return 0;
}

static sl_reg_category_t sl_reg_alloc_get_category(sl_reg_alloc_kind_t kind) {
  switch (kind) {
    case slrak_void:
    case slrak_array:
    case slrak_struct:
      return slrc_invalid;
    case slrak_float:
      return slrc_float;
    case slrak_int:
      return slrc_int;
    case slrak_bool:
      return slrc_bool;
    case slrak_vec2:
      return slrc_float;
    case slrak_vec3:
      return slrc_float;
    case slrak_vec4:
      return slrc_float;
    case slrak_bvec2:
      return slrc_bool;
    case slrak_bvec3:
      return slrc_bool;
    case slrak_bvec4:
      return slrc_bool;
    case slrak_ivec2:
      return slrc_int;
    case slrak_ivec3:
      return slrc_int;
    case slrak_ivec4:
      return slrc_int;
    case slrak_mat2:
      return slrc_float;
    case slrak_mat3:
      return slrc_float;
    case slrak_mat4:
      return slrc_float;
    case slrak_sampler2D:
      return slrc_sampler2D;
    case slrak_samplerCube:
      return slrc_samplerCube;
  }
  return slrc_invalid;
}


int sl_reg_allocator_lock_reg_range(struct sl_reg_allocator *ract, sl_reg_category_t cat, int head_reg, int array_quantity) {
  struct ref_range_allocator *rra = NULL;
  switch (cat) {
    case slrc_float:
      rra = &ract->rra_floats_;
      break;
    case slrc_int:
      rra = &ract->rra_ints_;
      break;
    case slrc_bool:
      rra = &ract->rra_bools_;
      break;
    case slrc_sampler2D:
      rra = &ract->rra_sampler2D_;
      break;
    case slrc_samplerCube:
      rra = &ract->rra_samplerCube_;
      break;
  }
  if (!rra) {
    /* Invalid category */
    assert(0);
    return -1;
  }
  int r;
  if (head_reg < 0) { return -1; /* invalid reg */ }
  if (array_quantity <= 0) { return -1; /* invalid reg range */ }
  uintptr_t from = (uintptr_t)head_reg;
  uintptr_t to = from + (uintptr_t)array_quantity;
  if (to < from) {
    /* overflow */
    return -1;
  }
  r = ref_range_mark_range_allocated(rra, from, to);
  return r;
}

int sl_reg_allocator_unlock_reg_range(struct sl_reg_allocator *ract, sl_reg_category_t cat, int head_reg, int array_quantity) {
  struct ref_range_allocator *rra = NULL;
  switch (cat) {
    case slrc_float:
      rra = &ract->rra_floats_;
      break;
    case slrc_int:
      rra = &ract->rra_ints_;
      break;
    case slrc_bool:
      rra = &ract->rra_bools_;
      break;
    case slrc_sampler2D:
      rra = &ract->rra_sampler2D_;
      break;
    case slrc_samplerCube:
      rra = &ract->rra_samplerCube_;
      break;
  }
  if (!rra) {
    /* Invalid category */
    assert(0);
    return -1;
  }
  int r;
  if (head_reg < 0) { return -1; /* invalid reg */ }
  if (array_quantity <= 0) { return -1; /* invalid reg range */ }
  uintptr_t from = (uintptr_t)head_reg;
  uintptr_t to = from + (uintptr_t)array_quantity;
  if (to < from) {
    /* overflow */
    return -1;
  }
  r = ref_range_mark_range_free(rra, from, to);
  return r;
}

int sl_reg_allocator_alloc_reg_range(struct sl_reg_allocator *ract, sl_reg_category_t cat, int array_quantity, int *result) {
  struct ref_range_allocator *rra = NULL;
  switch (cat) {
    case slrc_float:
      rra = &ract->rra_floats_;
      break;
    case slrc_int:
      rra = &ract->rra_ints_;
      break;
    case slrc_bool:
      rra = &ract->rra_bools_;
      break;
    case slrc_sampler2D:
      rra = &ract->rra_sampler2D_;
      break;
    case slrc_samplerCube:
      rra = &ract->rra_samplerCube_;
      break;
  }
  if (!rra) {
    /* Invalid category */
    assert(0);
    return -1;
  }
  int r;
  if (array_quantity <= 0) { return -1; /* invalid reg range */ }
  uintptr_t from = 0;
  r = ref_range_alloc(rra, (uintptr_t)array_quantity, &from);
  if (r) return r;
  *result = (int)from;
  return 0;
}


static int sl_reg_allocator_lock_descend(struct sl_reg_allocator *ract, int array_quantity, struct sl_reg_alloc *ra);
static int sl_reg_allocator_unlock_descend(struct sl_reg_allocator *ract, int array_quantity, struct sl_reg_alloc *ra);

static int sl_reg_allocator_lock_descend(struct sl_reg_allocator *ract, int array_quantity, struct sl_reg_alloc *ra) {
  int r;
  if (ra->kind_ == slrak_struct) {
    size_t n;
    for (n = 0; n < ra->v_.comp_.num_fields_; ++n) {
      r = sl_reg_allocator_lock_descend(ract, array_quantity, ra->v_.comp_.fields_ + n);
      if (r) {
        /* No memory or an overflow */
        size_t k;
        for (k = 0; k < n; ++k) {
          sl_reg_allocator_unlock_descend(ract, array_quantity, ra->v_.comp_.fields_ + k);
        }
        return -1;
      }
    }
  }
  else if (ra->kind_ == slrak_array) {
    if (ra->v_.array_.num_elements_ > (INT_MAX / array_quantity)) {
      /* overflow */
      return -1;
    }
    array_quantity *= (int)ra->v_.array_.num_elements_;
    r = sl_reg_allocator_lock_descend(ract, array_quantity, ra->v_.array_.head_);
    if (r) {
      return r;
    }
  }
  else {
    int card = sl_reg_alloc_get_cardinality(ra->kind_);
    sl_reg_category_t cat = sl_reg_alloc_get_category(ra->kind_);
    int n;
    for (n = 0; n < card; ++n) {
      r = sl_reg_allocator_lock_reg_range(ract, cat, ra->v_.regs_[n], array_quantity);
      if (r) {
        /* No memory or an overflow */
        int k;
        for (k = 0; k < n; ++k) {
          sl_reg_allocator_unlock_reg_range(ract, cat, ra->v_.regs_[k], array_quantity);
        }
        return -1;
      }
    }
  }
  return 0;
}

static int sl_reg_allocator_unlock_descend(struct sl_reg_allocator *ract, int array_quantity, struct sl_reg_alloc *ra) {
  int r;
  if (ra->kind_ == slrak_struct) {
    size_t n;
    for (n = 0; n < ra->v_.comp_.num_fields_; ++n) {
      r = sl_reg_allocator_unlock_descend(ract, array_quantity, ra->v_.comp_.fields_ + n);
      if (r) {
        /* don't try to undo the unlock lest we flip-flop between lock and unlock in perpetuity */
        return r;
      }
    }
  }
  else if (ra->kind_ == slrak_array) {
    if (ra->v_.array_.num_elements_ >(INT_MAX / array_quantity)) {
      /* overflow */
      return -1;
    }
    array_quantity *= (int)ra->v_.array_.num_elements_;
    r = sl_reg_allocator_unlock_descend(ract, array_quantity, ra->v_.array_.head_);
    if (r) {
      return r;
    }
  }
  else {
    int card = sl_reg_alloc_get_cardinality(ra->kind_);
    sl_reg_category_t cat = sl_reg_alloc_get_category(ra->kind_);
    int n;
    for (n = 0; n < card; ++n) {
      r = sl_reg_allocator_unlock_reg_range(ract, cat, ra->v_.regs_[n], array_quantity);
      if (r) {
        /* don't try to undo the unlock lest we flip-flop between lock and unlock in perpetuity */
        return r;
      }
    }
  }
  return 0;
}

static int sl_reg_allocator_alloc_descend(struct sl_reg_allocator *ract, int array_quantity, struct sl_reg_alloc *ra) {
  int r;
  if (ra->kind_ == slrak_struct) {
    size_t n;
    for (n = 0; n < ra->v_.comp_.num_fields_; ++n) {
      r = sl_reg_allocator_alloc_descend(ract, array_quantity, ra->v_.comp_.fields_ + n);
      if (r) {
        /* No memory or an overflow */
        size_t k;
        for (k = 0; k < n; ++k) {
          sl_reg_allocator_unlock_descend(ract, array_quantity, ra->v_.comp_.fields_ + k);
        }
        return -1;
      }
    }
  }
  else if (ra->kind_ == slrak_array) {
    if (ra->v_.array_.num_elements_ >(INT_MAX / array_quantity)) {
      /* overflow */
      return -1;
    }
    array_quantity *= (int)ra->v_.array_.num_elements_;
    r = sl_reg_allocator_alloc_descend(ract, array_quantity, ra->v_.array_.head_);
    if (r) {
      return r;
    }
  }
  else {
    int card = sl_reg_alloc_get_cardinality(ra->kind_);
    sl_reg_category_t cat = sl_reg_alloc_get_category(ra->kind_);
    int n;
    for (n = 0; n < card; ++n) {
      int base_reg = 0;
      r = sl_reg_allocator_alloc_reg_range(ract, cat, array_quantity, &base_reg);
      if (r) {
        /* No memory or an overflow */
        int k;
        for (k = 0; k < n; ++k) {
          sl_reg_allocator_unlock_reg_range(ract, cat, ra->v_.regs_[k], array_quantity);
        }
        return -1;
      }
    }
  }
  return 0;
}

static int sl_reg_allocator_lock_or_alloc_descend(struct sl_reg_allocator *ract, int array_quantity, struct sl_reg_alloc *ra) {
  int r;
  if (ra->kind_ == slrak_struct) {
    size_t n;
    for (n = 0; n < ra->v_.comp_.num_fields_; ++n) {
      r = sl_reg_allocator_lock_or_alloc_descend(ract, array_quantity, ra->v_.comp_.fields_ + n);
      if (r) {
        /* No memory or an overflow */
        size_t k;
        for (k = 0; k < n; ++k) {
          sl_reg_allocator_unlock_descend(ract, array_quantity, ra->v_.comp_.fields_ + k);
        }
        return -1;
      }
    }
  }
  else if (ra->kind_ == slrak_array) {
    if (ra->v_.array_.num_elements_ >(INT_MAX / array_quantity)) {
      /* overflow */
      return -1;
    }
    array_quantity *= (int)ra->v_.array_.num_elements_;
    r = sl_reg_allocator_lock_or_alloc_descend(ract, array_quantity, ra->v_.array_.head_);
    if (r) {
      return r;
    }
  }
  else {
    int card = sl_reg_alloc_get_cardinality(ra->kind_);
    sl_reg_category_t cat = sl_reg_alloc_get_category(ra->kind_);
    int n;
    int is_allocated = sl_reg_alloc_is_allocated(ra);
    for (n = 0; n < card; ++n) {
      int base_reg = 0;
      if (!is_allocated) {
        r = sl_reg_allocator_alloc_reg_range(ract, cat, array_quantity, &base_reg);
      }
      else {
        r = sl_reg_allocator_lock_reg_range(ract, cat, ra->v_.regs_[n], array_quantity);
      }
      if (r) {
        /* No memory or an overflow */
        int k;
        for (k = 0; k < n; ++k) {
          sl_reg_allocator_unlock_reg_range(ract, cat, ra->v_.regs_[k], array_quantity);
        }
        return -1;
      }
    }
  }
  return 0;
}

int sl_reg_allocator_lock(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra) {
  int r;
  r = sl_reg_allocator_lock_descend(ract, 1, ra);
  if (!r && ra->offset_) {
    r = sl_reg_allocator_lock_descend(ract, 1, ra->offset_);
  }
  return r;
}

int sl_reg_allocator_unlock(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra) {
  int r;
  r = sl_reg_allocator_unlock_descend(ract, 1, ra);
  if (!r && ra->offset_) {
    r = sl_reg_allocator_unlock_descend(ract, 1, ra->offset_);
  }
  return r;
}

int sl_reg_allocator_alloc(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra) {
  int r;
  r = sl_reg_allocator_alloc_descend(ract, 1, ra);
  if (!r && ra->offset_) {
    r = sl_reg_allocator_alloc_descend(ract, 1, ra->offset_);
  }
  return r;
}

int sl_reg_allocator_lock_or_alloc(struct sl_reg_allocator *ract, struct sl_reg_alloc *ra) {
  int r;
  r = sl_reg_allocator_lock_or_alloc_descend(ract, 1, ra);
  if (!r && ra->offset_) {
    r = sl_reg_allocator_lock_or_alloc_descend(ract, 1, ra->offset_);
  }
  return r;
}

void sl_reg_allocator_init(struct sl_reg_allocator *ra) {
  ref_range_allocator_init(&ra->rra_floats_);
  ref_range_allocator_init(&ra->rra_ints_);
  ref_range_allocator_init(&ra->rra_bools_);
  ref_range_allocator_init(&ra->rra_sampler2D_);
  ref_range_allocator_init(&ra->rra_samplerCube_);
}

void sl_reg_allocator_cleanup(struct sl_reg_allocator *ra) {
  ref_range_allocator_cleanup(&ra->rra_floats_);
  ref_range_allocator_cleanup(&ra->rra_ints_);
  ref_range_allocator_cleanup(&ra->rra_bools_);
  ref_range_allocator_cleanup(&ra->rra_sampler2D_);
  ref_range_allocator_cleanup(&ra->rra_samplerCube_);
}

