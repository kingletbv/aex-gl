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

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
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
  u->fragment_variable_ = u->vertex_variable_ = u->debug_variable_ = NULL;
  u->slab_[0] = 0;
}

static void sl_uniform_free(struct sl_uniform *u) {
}

void sl_uniform_table_init(struct sl_uniform_table *ut) {
  ut->uniforms_ = NULL;
}

void sl_uniform_table_cleanup(struct sl_uniform_table *ut) {
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
        *ra_size = sizeof(int32_t);
        *ra_alignment_req = sizeof(int32_t);
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


static int sl_uniform_get_ra_location_info(struct sl_reg_alloc *ra, size_t location, size_t *poffset, sl_reg_alloc_kind_t *plocation_type, size_t *pname_length, char *name_buf, size_t *pfinal_array_size, size_t *pentry_in_final_array) {
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
    sprintf(name_index, "[%zu]" , element_index);
    size_t len = strlen(name_index);
    if (name_buf) {
      strcpy(name_buf, name_index);
      name_buf += len;
    }

    size_t offset_in_element;
    size_t name_length_in_element;
    r = sl_uniform_get_ra_location_info(ra->v_.array_.head_, location_in_element, &offset_in_element, plocation_type, &name_length_in_element, name_buf, pfinal_array_size, pentry_in_final_array);
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

        r = sl_uniform_get_ra_location_info(ra->v_.comp_.fields_ + n, locations_remaining, &field_internal_offset, plocation_type, &field_internal_name_length, name_buf, pfinal_array_size, pentry_in_final_array);
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

int sl_uniform_get_location_info(struct sl_uniform_table *ut, size_t location, void **pp_slab_memory, sl_reg_alloc_kind_t *plocation_type, size_t *pname_length, char *name_buf, size_t *pfinal_array_size, size_t *pentry_in_final_array) {
  int r;
  size_t num_locations_remaining = location;
  struct sl_uniform *u = ut->uniforms_;
  if (u) {
    do {
      u = u->chain_;

      struct sl_variable *v = u->vertex_variable_ ? u->vertex_variable_ : (u->fragment_variable_ ? u->fragment_variable_ : u->debug_variable_);

      size_t num_locations_in_uniform;
      r = sl_uniform_get_reg_alloc_num_indices(&v->reg_alloc_, &num_locations_in_uniform);
      if (r) return r;

      if (num_locations_in_uniform > num_locations_remaining) {
        size_t u_offset_inside;
        size_t u_name_len;
        char *u_name_buf = NULL;
        sl_reg_alloc_kind_t location_type = slrak_void;
        size_t uniform_varname_length = strlen(v->name_);
        size_t final_array_size;
        size_t entry_in_final_array;
        if (name_buf) {
          strcpy(name_buf, v->name_);
          u_name_buf = name_buf + uniform_varname_length;
        }
        
        r = sl_uniform_get_ra_location_info(&v->reg_alloc_, num_locations_remaining, &u_offset_inside, &location_type, &u_name_len, u_name_buf, &final_array_size, &entry_in_final_array);
        if (r) return r;

        if (pname_length) *pname_length = uniform_varname_length + u_name_len;
        if (pp_slab_memory) *pp_slab_memory = ((char *)u->slab_) + u_offset_inside;
        if (plocation_type) *plocation_type = location_type;
        if (pfinal_array_size) *pfinal_array_size = final_array_size;
        if (pentry_in_final_array) *pentry_in_final_array = entry_in_final_array;
        return 0;
      }
      else {
        num_locations_remaining -= num_locations_in_uniform;
      }

    } while (u != ut->uniforms_);
  }
  return SL_ERR_INVALID_ARG;
}

static int sl_uniform_get_ra_named_location(struct sl_reg_alloc *ra, const char *name, size_t *plocation) {
  int r;
  const char *cp = name;
  if (ra->kind_ == slrak_array) {
    /* Next char up should be [, followed by non-zero digits, followed by ].
     * Alternatively, this is a final array element, and name should be '\0' -- however -- in
     * that case, the type of the array should be a simple type and we return an offset of 0
     * to represent the location of the first element in the array. */
    if (*cp == '\0') {
      if (sl_uniform_reg_alloc_is_simple_type(ra->v_.array_.head_)) {
        *plocation = 0;
        return 0;
      }
      /* Ending on an array that is not of a simple type. The name cannot point to a struct,
       * or an array of arrays, consequently, we fail this name. */
      return SL_ERR_INVALID_ARG;
    }
    else {
      /* Should be [ digits ] */
      if (*cp != '[') {
        return SL_ERR_INVALID_ARG;
      }
      cp++;
      if (((*cp) >= '0') && ((*cp) <= '9')) {
        /* Let's roll a manual atoi() so we know when it overflows and always fit the type size */
        size_t index = (size_t)((*cp) - '0');

        const char *start_of_digits = cp;
        cp++;
        while (((*cp) >= '0') && ((*cp) <= '9')) {
          if ((SIZE_MAX / 10) < index) {
            /* Alas, overflow; this represents an invalid name and as such we return SL_ERR_INVALID_ARG
             * instead of SL_ERR_OVERFLOW. */
            return SL_ERR_INVALID_ARG;
          }
          index = index * 10;
          size_t digit = (size_t)((*cp) - '0');
          index = index + digit;
          if (index < digit) {
            /* Overflow */
            return SL_ERR_INVALID_ARG;
          }
          cp++;
        }
        const char *end_of_digits = cp;
        if ((*cp) != ']') {
          return SL_ERR_INVALID_ARG;
        }
        cp++;
        if (index >= ra->v_.array_.num_elements_) {
          /* Array index is out of bounds for the array size in question */
          return SL_ERR_INVALID_ARG;
        }

        size_t locations_per_element;
        r = sl_uniform_get_reg_alloc_num_indices(ra->v_.array_.head_, &locations_per_element);
        if (r) return r;
        if ((SIZE_MAX / locations_per_element) < index) {
          /* Index is so large it'd cause an overflow of the number of locations (would expect
           * this to get caught elsewhere.) */
          return SL_ERR_INVALID_ARG;
        }

        size_t base_element_location_offset = locations_per_element * index;
        size_t location_offset_inside_element;
        r = sl_uniform_get_ra_named_location(ra->v_.array_.head_, cp, &location_offset_inside_element);
        if (r) return r;
        size_t location = base_element_location_offset + location_offset_inside_element;
        if (location < base_element_location_offset) return SL_ERR_INVALID_ARG;
        *plocation = location;
        return 0;
      }
      else {
        /* Not getting digits followed by ] */
        return SL_ERR_INVALID_ARG;
      }
    }
  }
  else if (ra->kind_ == slrak_struct) {
    if ((*cp) != '.') {
      /* Expecting a field selection, but not getting one.. */
      return SL_ERR_INVALID_ARG;
    }
    cp++;
    /* Next expecting an identifier representing the field */
    const char *end_ident;
    end_ident = cp;
    if ((((*end_ident) >= 'a') && ((*end_ident) <= 'z')) ||
        (((*end_ident) >= 'A') && ((*end_ident) <= 'Z')) ||
        ((*end_ident) == '_')) {
      end_ident++;
      while ((((*end_ident) >= 'a') && ((*end_ident) <= 'z')) ||
             (((*end_ident) >= 'A') && ((*end_ident) <= 'Z')) ||
             (((*end_ident) >= '0') && ((*end_ident) <= '9')) ||
             ((*end_ident) == '_')) {
        end_ident++;
      }
    }
    if (end_ident == cp) {
      /* No identifier for the field was found.. */
      return SL_ERR_INVALID_ARG;
    }
    size_t ident_len = end_ident - cp;
    /* Locate the index of the field, and while locating it, count the number
     * of locations we're skipping. */
    size_t field_index;
    size_t location_field_offset = 0;
    struct sl_type_field *tf;
    tf = ra->v_.comp_.struct_type_->fields_;
    field_index = 0;
    if (tf) {
      do {
        tf = tf->chain_;

        if ((strlen(tf->ident_) == ident_len) && !memcmp(cp, tf->ident_, ident_len)) {
          /* Found matching field */
          size_t location_offset_inside_field;
          r = sl_uniform_get_ra_named_location(ra->v_.comp_.fields_ + field_index, end_ident, &location_offset_inside_field);
          if (r) return r;
          size_t final_location = location_field_offset + location_offset_inside_field;
          if (final_location < location_field_offset) return SL_ERR_INVALID_ARG; /* overflow */
          *plocation = final_location;
          return 0;
        }

        size_t num_locations_for_field;
        r = sl_uniform_get_reg_alloc_num_indices(ra->v_.comp_.fields_ + field_index, &num_locations_for_field);
        if (r) return r;
        location_field_offset += num_locations_for_field;
        if (location_field_offset < num_locations_for_field) return SL_ERR_INVALID_ARG; /* overflow */

      } while (tf != ra->v_.comp_.struct_type_->fields_);
    }
    /* Couldn't match a field */
    return SL_ERR_INVALID_ARG;
  }
  else {
    if (ra->kind_ == slrak_void) {
      /* Cannot locate a void */
      return SL_ERR_INVALID_ARG;
    }
    /* ... otherwise we've reached a leaf in the datastructure: a "simple" type; make sure the name
     *     ends here, and, if so, return success at location offset 0; callers will bump this up
     *     to the appropriate location. */
    if (*name != '\0') {
      return SL_ERR_INVALID_ARG;
    }
    *plocation = 0;
    return 0;
  }
}

int sl_uniform_get_named_location(struct sl_uniform_table *ut, const char *name, size_t *plocation) {
  int r;
  const char *end_ident;
  end_ident = name;
  if ((((*end_ident) >= 'a') && ((*end_ident) <= 'z')) ||
      (((*end_ident) >= 'A') && ((*end_ident) <= 'Z')) ||
      ((*end_ident) == '_')) {
    end_ident++;
    while ((((*end_ident) >= 'a') && ((*end_ident) <= 'z')) ||
           (((*end_ident) >= 'A') && ((*end_ident) <= 'Z')) ||
           (((*end_ident) >= '0') && ((*end_ident) <= '9')) ||
           ((*end_ident) == '_')) {
      end_ident++;
    }
  }
  if (end_ident == name) {
    /* Should always start with the name of the uniform variable.. */
    return SL_ERR_INVALID_ARG;
  }

  /* Find the uniform */
  struct sl_uniform *u = ut->uniforms_;
  if (u) {
    size_t locations_skipped = 0;
    do {
      u = u->chain_;

      struct sl_variable *v = u->vertex_variable_ ? u->vertex_variable_ : (u->fragment_variable_ ? u->fragment_variable_ : u->debug_variable_);
      size_t u_name_len = strlen(v->name_);
      if ((u_name_len == (end_ident - name)) && !memcmp(v->name_, name, u_name_len)) {
        /* Lengths match and string matches; this is the uniform variable we're looking for. */
        size_t location_inside_uniform;
        r = sl_uniform_get_ra_named_location(&v->reg_alloc_, end_ident, &location_inside_uniform);
        if (r) return r;
        if (plocation) *plocation = locations_skipped + location_inside_uniform;
        return 0;
      }
      else {
        size_t num_locations_in_uniform;
        r = sl_uniform_get_reg_alloc_num_indices(&v->reg_alloc_, &num_locations_in_uniform);
        if (r) return r;
        locations_skipped += num_locations_in_uniform;
      }

    } while (u != ut->uniforms_);
  }

  /* We get here if we were unable to match the name of the uniform variable */
  return SL_ERR_INVALID_ARG;
}


int sl_uniform_table_add_uniform(struct sl_uniform_table *ut, struct sl_uniform **pp_uniform, struct sl_variable *vertex_side, struct sl_variable *fragment_side, struct sl_variable *debug_side) {
  if (!vertex_side && !fragment_side && !debug_side) return SL_ERR_INVALID_ARG;
  if ((vertex_side || fragment_side) && !sl_are_variables_compatible(vertex_side, fragment_side)) {
    return SL_ERR_INVALID_ARG;
  }

  /* either vertex_side or, if that's NULL, fragment_side (one of these is guaranteed to not be NULL). */
  struct sl_variable *var = vertex_side ? vertex_side : (fragment_side ? fragment_side : debug_side);

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
  u->debug_variable_ = debug_side;
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

static int sl_uniform_max_ra_name_length(struct sl_reg_alloc *ra, size_t *pmax_name_length) {
  int r;
  if (ra->kind_ == slrak_array) {
    char name_index[20 + 2 + 1];
    sprintf(name_index, "[%" PRIu64 "]", (uint64_t)ra->v_.array_.num_elements_);
    size_t len = strlen(name_index);
    size_t child_len;
    r = sl_uniform_max_ra_name_length(ra->v_.array_.head_, &child_len);
    if (r) return r;
    *pmax_name_length = len + child_len;
    return 0;
  }
  else if (ra->kind_ == slrak_array) {
    size_t n;
    size_t max_len = 0;
    struct sl_type_field *tf = ra->v_.comp_.struct_type_->fields_;
    for (n = 0; n < ra->v_.comp_.num_fields_; ++n) {
      size_t field_len;
      tf = tf->chain_;
      field_len = 1 /* "." */ + strlen(tf->ident_);
      size_t child_len;
      r = sl_uniform_max_ra_name_length(ra->v_.comp_.fields_ + n, &child_len);
      field_len += child_len;
      if (field_len > max_len) max_len = field_len;
    }
    *pmax_name_length = max_len;
    return 0;
  }
  else {
    /* no name part when we arrive here, as the final simple field has now been selected.. */
    *pmax_name_length = 0;
    return 0;
  }
}

int sl_uniform_table_max_name_length(struct sl_uniform_table *ut, size_t *pmax_name_length) {
  int r;
  struct sl_uniform *u = ut->uniforms_;
  size_t max_name_length = 0;
  if (u) {
    do {
      u = u->chain_;

      struct sl_variable *v = u->vertex_variable_ ? u->vertex_variable_ : (u->fragment_variable_ ? u->fragment_variable_ : u->debug_variable_);
      size_t uniform_name_length;
      uniform_name_length = strlen(v->name_);
      size_t uniform_ra_max_length;
      r = sl_uniform_max_ra_name_length(&v->reg_alloc_, &uniform_ra_max_length);
      if (r) return r;

      size_t uniform_max_length = uniform_name_length + uniform_ra_max_length;
      if (max_name_length < uniform_max_length) max_name_length = uniform_max_length;

    } while (u != ut->uniforms_);
  }
  *pmax_name_length = max_name_length + 1 /* null terminator goes here */;
  return 0;
}

int sl_uniform_table_num_locations(struct sl_uniform_table *ut, size_t *pnum_locations) {
  int r;
  struct sl_uniform *u = ut->uniforms_;
  size_t max_num_locations = 0;
  if (u) {
    do {
      u = u->chain_;

      struct sl_variable *v = u->vertex_variable_ ? u->vertex_variable_ : (u->fragment_variable_ ? u->fragment_variable_ : u->debug_variable_);

      size_t num_locations = 0;
      r = sl_uniform_get_reg_alloc_num_indices(&v->reg_alloc_, &num_locations);
      if (r) return r;
      
      max_num_locations += num_locations;

    } while (u != ut->uniforms_);
  }
  *pnum_locations = max_num_locations;
  return SL_ERR_OK;
}

static void sl_uniform_load_f(size_t num, float *dst, float val) {
  float *restrict p = (float * restrict)dst;
  while (num--) {
    *p++ = val;
  }
}

static void sl_uniform_load_i(size_t num, int64_t *dst, int64_t val) {
  int64_t *restrict p = (int64_t * restrict)dst;
  while (num--) {
    *p++ = val;
  }
}

static void sl_uniform_load_b(size_t num, uint8_t *dst, uint8_t val) {
  uint8_t * restrict p = (uint8_t * restrict)dst;
  while (num--) {
    *p++ = val;
  }
}

static void sl_uniform_load_voidp(size_t num, void **dst, void *val) {
  void **restrict p = (void * restrict * restrict)dst;
  while (num--) {
    *p++ = val;
  }
}

int sl_uniform_load_ra_for_execution(struct sl_execution *exec, void *base_mem, size_t offset, size_t *pnum_slab_bytes_consumed, struct sl_reg_alloc *ra,
                                     size_t loading_table_size,
                                     void **sampler_2D_uniform_loading_table,
                                     void **sampler_Cube_uniform_loading_table) {
  int r;
  if (ra->kind_ == slrak_array) {
    size_t n;
    size_t num_elements = ra->v_.array_.num_elements_;
    size_t original_offset = offset;
    for (n = 0; n < ra->v_.array_.num_elements_; ++n) {
      size_t num_bytes_consumed;
      r = sl_uniform_load_ra_for_execution(exec, base_mem, offset, &num_bytes_consumed, ra->v_.array_.head_, loading_table_size, sampler_2D_uniform_loading_table, sampler_Cube_uniform_loading_table);
      if (r) return r;
      offset += num_bytes_consumed;
    }
    *pnum_slab_bytes_consumed = offset - original_offset;
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

      size_t field_offset_from_base = field_offset + offset;
      if (field_offset_from_base < field_offset) {
        return SL_ERR_OVERFLOW;
      }
      size_t num_bytes_consumed_for_field;
      r = sl_uniform_load_ra_for_execution(exec, base_mem, field_offset_from_base, &num_bytes_consumed_for_field, ra->v_.comp_.fields_ + n, loading_table_size, sampler_2D_uniform_loading_table, sampler_Cube_uniform_loading_table);
      if (r) return r;

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
    *pnum_slab_bytes_consumed = aligned_struct_size;
    return 0;
  }
  else {
    size_t num_components = 0;
    size_t n;
    size_t original_offset = offset;
    switch (ra->kind_) {
      case slrak_void:
        return SL_ERR_INVALID_ARG;
      case slrak_float:
      case slrak_vec2:
      case slrak_vec3:
      case slrak_vec4:
      case slrak_mat2:
      case slrak_mat3:
      case slrak_mat4:
        /* Align offset to float */
        offset = (offset + sizeof(float) - 1) & ~(sizeof(float) - 1);
        switch (ra->kind_) {
          case slrak_float: num_components = 1; break;
          case slrak_vec2:  num_components = 2; break;
          case slrak_vec3:  num_components = 3; break;
          case slrak_vec4:  num_components = 4; break;
          case slrak_mat2:  num_components = 4; break;
          case slrak_mat3:  num_components = 9; break;
          case slrak_mat4:  num_components = 16; break;
        }
        for (n = 0; n < num_components; ++n) {
          sl_uniform_load_f(exec->max_num_rows_, exec->float_regs_[ ra->v_.regs_[n] ], ((float *)(((char *)base_mem) + offset))[n]);
        }
        *pnum_slab_bytes_consumed = offset - original_offset + num_components * sizeof(float);
        break;
      case slrak_bool:
      case slrak_bvec2:
      case slrak_bvec3:
      case slrak_bvec4:
        switch (ra->kind_) {
          case slrak_bool:  num_components = 1; break;
          case slrak_bvec2: num_components = 2; break;
          case slrak_bvec3: num_components = 3; break;
          case slrak_bvec4: num_components = 4; break;
        }
        for (n = 0; n < num_components; ++n) {
          sl_uniform_load_b(exec->max_num_rows_, exec->bool_regs_[ra->v_.regs_[n]], ((uint8_t *)(((char *)base_mem) + offset))[n]);
        }
        *pnum_slab_bytes_consumed = offset - original_offset + num_components * sizeof(uint8_t);
        break;
      case slrak_int:
      case slrak_ivec2:
      case slrak_ivec3:
      case slrak_ivec4:
        /* Align offset to int64_t */
        offset = (offset + sizeof(int64_t) - 1) & ~(sizeof(int64_t) - 1);
        switch (ra->kind_) {
          case slrak_int:   num_components = 1; break;
          case slrak_ivec2: num_components = 2; break;
          case slrak_ivec3: num_components = 3; break;
          case slrak_ivec4: num_components = 4; break;
        }
        for (n = 0; n < num_components; ++n) {
          sl_uniform_load_i(exec->max_num_rows_, exec->int_regs_[ra->v_.regs_[n]], ((int64_t *)(((char *)base_mem) + offset))[n]);
        }
        *pnum_slab_bytes_consumed = offset - original_offset + num_components * sizeof(int64_t);
        break;
      case slrak_sampler2D: {
        /* Align offset to void* */
        offset = (offset + sizeof(void*) - 1) & ~(sizeof(void*) - 1);
        uint32_t loading_offset = *(uint32_t *)(((char *)base_mem) + offset);
        void *sampler_2d_ptr = NULL;
        if (loading_offset < loading_table_size) {
          sampler_2d_ptr = sampler_2D_uniform_loading_table[loading_offset];
        }
        sl_uniform_load_voidp(exec->max_num_rows_, exec->sampler_2D_regs_[ra->v_.regs_[0]], sampler_2d_ptr);
        *pnum_slab_bytes_consumed = offset - original_offset + sizeof(void *);
        break;
      }
      case slrak_samplerCube: {
        /* Align offset to void* */
        offset = (offset + sizeof(void *) - 1) & ~(sizeof(void *) - 1);
        uint32_t loading_offset = *(uint32_t *)(((char *)base_mem) + offset);
        void *sampler_cube_ptr = NULL;
        if (loading_offset < loading_table_size) {
          sampler_cube_ptr = sampler_Cube_uniform_loading_table[loading_offset];
        }
        sl_uniform_load_voidp(exec->max_num_rows_, exec->sampler_cube_regs_[ra->v_.regs_[0]], sampler_cube_ptr);
        *pnum_slab_bytes_consumed = offset - original_offset + sizeof(void *);
        break;
      }
      default:
        return SL_ERR_INVALID_ARG;
    }
    return 0;
  }
}


