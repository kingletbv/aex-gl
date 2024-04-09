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

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef INTTYPES_H_INCLUDED
#define INTTYPES_H_INCLUDED
#include <inttypes.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef SL_DEFS_H_INCLUDED
#define SL_DEFS_H_INCLUDED
#include "sl_defs.h"
#endif

#ifndef SL_TYPES_H_INCLUDED
#define SL_TYPES_H_INCLUDED
#include "sl_types.h"
#endif

#ifndef SL_FRAME_H_INCLUDED
#define SL_FRAME_H_INCLUDED
#include "sl_frame.h"
#endif

#ifndef SL_UNIFORMS_H_INCLUDED
#define SL_UNIFORMS_H_INCLUDED
#include "sl_uniforms.h"
#endif

static void sl_uniform_init(struct sl_uniform *u) {
  u->chain_ = NULL;
  u->fragment_variable_ = u->vertex_variable_ = NULL;
  u->slab_[0] = 0;
}

static void sl_uniform_free(struct sl_uniform *u) {
}

void sl_uniform_table_init(struct sl_uniform_table *ut) {
  ut->uniforms_ = NULL;
}

void sl_uniform_table_cleanup(struct sl_uniform_table *ut) {
}

static int sl_uniform_are_reg_allocs_compatible(struct sl_reg_alloc *a, struct sl_reg_alloc *b) {
  if (!a || !b) return 0;

  if (a->kind_ != b->kind_) {
    return 0;
  }
  if (a->kind_ == slrak_array) {
    if (a->v_.array_.num_elements_ == b->v_.array_.num_elements_) {
      /* Same number of elements, check child head reg_allocs for compatibility. Note that
       * we don't try to solve this via comparing pointer identities of ->v_.array_.element_type_ 
       * which would work if both a and b are from the same type_base, but that is not the
       * case when comparing different shader uniforms for compatibility (and so the types
       * would be from different type_bases.) */
      return sl_uniform_are_reg_allocs_compatible(a->v_.array_.head_, b->v_.array_.head_);
    }
    return 0;
  }
  else if (a->kind_ == slrak_struct) {
    if (a->v_.comp_.num_fields_ != b->v_.comp_.num_fields_) return 0;
    /* Iterate over the fields, all fields must be equal. */
    size_t n;
    for (n = 0; n < a->v_.comp_.num_fields_; ++n) {
      if (!sl_uniform_are_reg_allocs_compatible(a->v_.comp_.fields_ + n, b->v_.comp_.fields_ + n)) {
        /* Mismatch on any field is a mismatch on all */
        return 0;
      }
    }
    /* Matched all fields, a is compatible with b */
    return 1;
  }
  else {
    /* The kind of reg_alloc is a simple type (vec, mat, scalar) and they're identical, therefore
     * the reg_allocs compatible. */
    return 1;
  }
}

int sl_uniform_are_variables_compatible(struct sl_variable *vertex_side, struct sl_variable *fragment_side) {
  if (!vertex_side || !fragment_side) {
    /* variables are not in conflict if one is missing */
    return 1;
  }

  return sl_uniform_are_reg_allocs_compatible(&vertex_side->reg_alloc_, &fragment_side->reg_alloc_);
}

static int sl_uniform_get_reg_alloc_num_indices(struct sl_reg_alloc *ra, size_t *pnum_locations) {
  int r;
  if (!ra) return SL_ERR_INVALID_ARG;
  if (ra->kind_ == slrak_array) {
    size_t num_per_element;
    r = sl_uniform_get_reg_alloc_num_indices(ra->v_.array_.head_, &num_per_element);
    if (r) return r;

    if ((SIZE_MAX / num_per_element) < ra->v_.array_.num_elements_) {
      /* Overflow of size */
      return SL_ERR_OVERFLOW;
    }
    *pnum_locations = num_per_element * ra->v_.array_.num_elements_;
    return 0;
  }
  else if (ra->kind_ == slrak_struct) {
    size_t num_counted;
    size_t n;
    num_counted = 0;
    for (n = 0; n < ra->v_.comp_.num_fields_; ++n) {
      size_t num_per_field;
      r = sl_uniform_get_reg_alloc_num_indices(ra->v_.array_.head_, &num_per_field);
      if (r) return r;

      num_counted += num_per_field;
      if (num_counted < num_per_field) return SL_ERR_OVERFLOW;
    }
    *pnum_locations = num_counted;
    return 0;
  }
  else {
    if (ra->kind_ == slrak_void) {
      return SL_ERR_INVALID_ARG;
    }
    *pnum_locations = 1;
    return 0;
  }
}

static int sl_uniform_get_reg_alloc_slab_size(struct sl_reg_alloc *ra, size_t *ra_size, size_t *ra_alignment_req) {
  int r;
  if (!ra) return SL_ERR_INVALID_ARG;
  if (ra->kind_ == slrak_array) {
    size_t element_size;
    r = sl_uniform_get_reg_alloc_slab_size(ra->v_.array_.head_, &element_size, ra_alignment_req);
    if (r) return r;

    if ((SIZE_MAX / element_size) < ra->v_.array_.num_elements_) {
      /* Overflow of size */
      return SL_ERR_OVERFLOW;
    }
    *ra_size = element_size * ra->v_.array_.num_elements_;
    /* ra_alignment_req already set by sl_uniform_get_reg_alloc_slab_size() recursion above */
    return 0;
  }
  else if (ra->kind_ == slrak_struct) {
    size_t field_offset;
    size_t n;
    size_t max_field_alignment = 1;
    field_offset = 0;
    for (n = 0; n < ra->v_.comp_.num_fields_; ++n) {
      size_t field_size, field_alignment;
      r = sl_uniform_get_reg_alloc_slab_size(ra->v_.comp_.fields_ + n, &field_size, &field_alignment);
      if (r) return r;

      /* Provide appropriate alignment for field */
      size_t aligned_field_offset = (field_offset + field_alignment - 1);
      if (aligned_field_offset < field_offset) return SL_ERR_OVERFLOW;
      aligned_field_offset = aligned_field_offset & ~(field_alignment - 1);

      if (max_field_alignment < field_alignment) max_field_alignment = field_alignment;

      aligned_field_offset += field_size;
      if (aligned_field_offset < field_size) return SL_ERR_OVERFLOW;

      field_offset = aligned_field_offset;
    }
    /* post-struct alignment padding, this occurs when we finish with a field with less size
     * necessary than the alignment of the struct as a whole */
    size_t aligned_struct_size = field_offset + max_field_alignment - 1;
    if (aligned_struct_size < field_offset) return SL_ERR_OVERFLOW;
    aligned_struct_size = aligned_struct_size & ~(max_field_alignment - 1);
    *ra_size = aligned_struct_size;
    *ra_alignment_req = max_field_alignment;
    return 0;
  }
  else {
    switch (ra->kind_) {
      case slrak_void:
        /* should not have void reg_allocs come down here */
        return SL_ERR_INVALID_ARG;
      case slrak_float:
        *ra_size = sizeof(float);
        *ra_alignment_req = sizeof(float);
        break;
      case slrak_int:
        *ra_size = sizeof(int64_t);
        *ra_alignment_req = sizeof(int64_t);
        break;
      case slrak_bool:
        *ra_size = sizeof(uint8_t);
        *ra_alignment_req = sizeof(uint8_t);
        break;
      case slrak_vec2:
        *ra_size = 2 * sizeof(float);
        *ra_alignment_req = sizeof(float);
        break;
      case slrak_vec3:
        *ra_size = 3 * sizeof(float);
        *ra_alignment_req = sizeof(float);
        break;
      case slrak_vec4:
        *ra_size = 4 * sizeof(float);
        *ra_alignment_req = sizeof(float);
        break;
      case slrak_bvec2:
        *ra_size = 2 * sizeof(uint8_t);
        *ra_alignment_req = sizeof(uint8_t);
        break;
      case slrak_bvec3:
        *ra_size = 3 * sizeof(uint8_t);
        *ra_alignment_req = sizeof(uint8_t);
        break;
      case slrak_bvec4:
        *ra_size = 4 * sizeof(uint8_t);
        *ra_alignment_req = sizeof(uint8_t);
        break;
      case slrak_ivec2:
        *ra_size = 2 * sizeof(int64_t);
        *ra_alignment_req = sizeof(int64_t);
        break;
      case slrak_ivec3:
        *ra_size = 3 * sizeof(int64_t);
        *ra_alignment_req = sizeof(int64_t);
        break;
      case slrak_ivec4:
        *ra_size = 4 * sizeof(int64_t);
        *ra_alignment_req = sizeof(int64_t);
        break;
      case slrak_mat2:
        *ra_size = 4 * sizeof(float);
        *ra_alignment_req = sizeof(float);
        break;
      case slrak_mat3:
        *ra_size = 9 * sizeof(float);
        *ra_alignment_req = sizeof(float);
        break;
      case slrak_mat4:
        *ra_size = 16 * sizeof(float);
        *ra_alignment_req = sizeof(float);
        break;
      case slrak_sampler2D:
      case slrak_samplerCube:
        *ra_size = sizeof(void *);
        *ra_alignment_req = sizeof(void *);
        break;
      default:
        return SL_ERR_INTERNAL;
    }
    return 0;
  }
}

static int sl_uniform_reg_alloc_is_simple_type(struct sl_reg_alloc *ra) {
  switch (ra->kind_) {
    case slrak_void:
    case slrak_array:
    case slrak_struct:
      return 0;
    default:
      return 1;
  }
}


static int sl_uniform_get_location_info(struct sl_reg_alloc *ra, size_t location, size_t *poffset, sl_reg_alloc_kind_t *plocation_type, size_t *pname_length, char *name_buf, size_t *pfinal_array_size, size_t *pentry_in_final_array) {
  int r;
  if (!ra) return SL_ERR_INVALID_ARG;
  if (ra->kind_ == slrak_array) {
    size_t num_locations_per_element;
    r = sl_uniform_get_reg_alloc_num_indices(ra->v_.array_.head_, &num_locations_per_element);
    if (r) return r;

    size_t element_index = location / num_locations_per_element;
    size_t location_in_element = location % num_locations_per_element;

    // Max uint64 = 18446744073709551615, which is 20 digits, add [ ], and add null terminator.
    char name_index[20 + 2 + 1];
    sprintf(name_index, "[%" PRIu64 "]" , element_index);
    size_t len = strlen(name_index);
    if (name_buf) {
      strcpy(name_buf, name_index);
      name_buf += len;
    }

    size_t offset_in_element;
    size_t name_length_in_element;
    r = sl_uniform_get_location_info(ra->v_.array_.head_, location_in_element, &offset_in_element, plocation_type, &name_length_in_element, name_buf, pfinal_array_size, pentry_in_final_array);
    if (r) return r;

    if (sl_uniform_reg_alloc_is_simple_type(ra->v_.array_.head_)) {
      /* This is the final array, child is the final simple type; update pfinal_array_size and *pentry_in_final_array fields
       * now we know the final type is an array */
      *pfinal_array_size = ra->v_.array_.num_elements_;
      *pentry_in_final_array = element_index;
      return 0;
    }
    
    size_t element_slab_size;
    size_t element_align_req;
    r = sl_uniform_get_reg_alloc_slab_size(ra->v_.array_.head_, &element_slab_size, &element_align_req);
    if (r) return r;

    size_t offset_to_location;
    /* XXX: Do this, but now check for every kind of overflow */
    if ((SIZE_MAX / element_index) < element_slab_size) {
      return SL_ERR_OVERFLOW;
    }
    offset_to_location = element_index * element_slab_size;
    offset_to_location = offset_to_location + offset_in_element;
    if (offset_to_location < offset_in_element) return SL_ERR_OVERFLOW;

    *poffset = offset_to_location;
    /* plocation_type already filled by recursive call above */
    *pname_length = name_length_in_element + len;

    return 0;
  }
  else if (ra->kind_ == slrak_struct) {
    size_t field_offset;
    size_t n;
    field_offset = 0;
    struct sl_type_field *tf;
    tf = ra->v_.comp_.struct_type_->fields_;
    size_t locations_remaining = location;

    for (n = 0; n < ra->v_.comp_.num_fields_; ++n) {
      tf = tf->chain_;

      size_t field_size, field_alignment;
      r = sl_uniform_get_reg_alloc_slab_size(ra->v_.comp_.fields_ + n, &field_size, &field_alignment);
      if (r) return r;

      /* Provide appropriate alignment for field */
      size_t aligned_field_offset = (field_offset + field_alignment - 1);
      if (aligned_field_offset < field_offset) return SL_ERR_OVERFLOW;
      aligned_field_offset = aligned_field_offset & ~(field_alignment - 1);

      size_t num_locations_for_field;
      r = sl_uniform_get_reg_alloc_num_indices(ra->v_.comp_.fields_ + n, &num_locations_for_field);

      if (num_locations_for_field > locations_remaining) {
        /* Location is inside the current field */
        size_t field_internal_offset;
        size_t field_internal_name_length;
        size_t field_len = strlen(tf->ident_) + 1;
        if (name_buf) {
          *name_buf = '.';
          memcpy(name_buf + 1, tf->ident_, field_len + 1 /* include NULL terminator */ - 1 /* exclude dot */);
          name_buf += field_len;
        }

        r = sl_uniform_get_location_info(ra->v_.comp_.fields_ + n, locations_remaining, &field_internal_offset, plocation_type, &field_internal_name_length, name_buf, pfinal_array_size, pentry_in_final_array);
        if (r) return r;
        *poffset = aligned_field_offset + field_internal_offset;
        if (*poffset < aligned_field_offset) return SL_ERR_OVERFLOW;
        *pname_length = field_len + field_internal_name_length;
        if (*pname_length < field_len) return SL_ERR_OVERFLOW;
        return 0;
      }

      locations_remaining -= num_locations_for_field;

      aligned_field_offset += field_size;
      if (aligned_field_offset < field_size) return SL_ERR_OVERFLOW;

      field_offset = aligned_field_offset;
    }
    /* We get here then we skipped over the entire struct, meaning the location does not exist. */ 
    return SL_ERR_INVALID_ARG;
  }
  else {
    if (ra->kind_ == slrak_void) {
      /* should not have void reg_allocs come down here */
      return SL_ERR_INVALID_ARG;
    }
    *poffset = 0;
    *plocation_type = ra->kind_;
    *pname_length = 0;
    /* Presume we're not in an array (making this an array of size 1); if immediate caller is
     * an slrak_array then it'll update these to array specific values. */
    *pfinal_array_size = 1;
    *pentry_in_final_array = 0;
    return 0;
  }
}

int sl_uniform_table_add_uniform(struct sl_uniform_table *ut, struct sl_uniform **pp_uniform, struct sl_variable *vertex_side, struct sl_variable *fragment_side) {
  if (!vertex_side && !fragment_side) return SL_ERR_INVALID_ARG;
  if (!sl_uniform_are_variables_compatible(vertex_side, fragment_side)) {
    return SL_ERR_INVALID_ARG;
  }

  /* either vertex_side or, if that's NULL, fragment_side (one of these is guaranteed to not be NULL). */
  struct sl_variable *var = vertex_side ? vertex_side : fragment_side;

  size_t slab_size, slab_align;
  int r;
  r = sl_uniform_get_reg_alloc_slab_size(&var->reg_alloc_, &slab_size, &slab_align);
  if (r) return r;
  if (slab_size < sizeof(uint64_t)) slab_size = sizeof(uint64_t);
  struct sl_uniform *u = (struct sl_uniform *)malloc(sizeof(struct sl_uniform) + slab_size - sizeof(uint64_t));
  if (!u) return SL_ERR_NO_MEM;

  sl_uniform_init(u);
  u->vertex_variable_ = vertex_side;
  u->fragment_variable_ = fragment_side;
  memset(u->slab_, 0, slab_size);
  if (ut->uniforms_) {
    /* tail cyclic append to end */
    u->chain_ = ut->uniforms_->chain_;
    ut->uniforms_->chain_ = u;
    ut->uniforms_ = u;
  }
  else {
    u->chain_ = u;
    ut->uniforms_ = u;
  }

  *pp_uniform = u;
  return 0;
}


