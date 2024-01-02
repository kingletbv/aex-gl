/* Copyright 2023-2024 Kinglet B.V.
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

#ifndef SL_TYPES_H
#define SL_TYPES_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef SITUS_H_INCLUDED
#define SITUS_H_INCLUDED
#include "situs.h"
#endif 

#ifdef __cplusplus
extern "C" {
#endif

#define SL_TYPE_BASE_HASH_SIZE 127

#define SL_TYPE_QUALIFIER_CONST            0x01
#define SL_TYPE_QUALIFIER_ATTRIBUTE        0x02
#define SL_TYPE_QUALIFIER_VARYING          0x04
#define SL_TYPE_QUALIFIER_INVARIANT        0x08
#define SL_TYPE_QUALIFIER_UNIFORM          0x10
#define SL_TYPE_QUALIFIER_HIGH_PRECISION   0x20
#define SL_TYPE_QUALIFIER_MEDIUM_PRECISION 0x40
#define SL_TYPE_QUALIFIER_LOW_PRECISION    0x80

typedef enum sl_type_kind {
  sltk_invalid,
  sltk_qualifier,
  sltk_array,
  sltk_struct,

  sltk_void,
  sltk_float,
  sltk_int,
  sltk_bool,
  sltk_vec2,
  sltk_vec3,
  sltk_vec4,
  sltk_bvec2,
  sltk_bvec3,
  sltk_bvec4,
  sltk_ivec2,
  sltk_ivec3,
  sltk_ivec4,
  sltk_mat2,
  sltk_mat3,
  sltk_mat4,
  sltk_sampler2D,
  sltk_samplerCube
} sl_type_kind_t;

#define SL_TYPE_FIRST_STATIC_SIMPLE_TYPE sltk_void
#define SL_TYPE_LAST_STATIC_SIMPLE_TYPE sltk_samplerCube

struct sl_type_field {
  /* Tail cyclic chain of fields in a structure */
  struct sl_type_field *chain_;

  /* Identifier of the field */
  char *ident_;
  struct situs ident_loc_;

  /* Type of the field */
  struct sl_type *type_;
};

struct sl_type {
  /* Identifier for the type that is unique (assigned upon construction); this is
   * used for hashing, rather than using the pointer (which would also be a good
   * candidate given types are never freed). Reason for using an identifier is such
   * that consecutive runs are deterministic and identical in the hashes that are
   * created to reduce chances of Heisenbugs and the like. */
  uint32_t id_;

  /* Kind of type this is; might be a complex type that depends on other types. For
   * the simple types you could also check against the pointer of the corresponding
   * sl_type_base entry for the simple type (e.g. "float_") - however using the enum
   * has the benefit that the compiler can generate jump tables and whatnot. */
  sl_type_kind_t kind_;

  /* Tail cyclic chain of types in the hash table; see sl_type_base::hash_table_[] */
  struct sl_type *hash_chain_;

  /* Tail cyclic chain in sequence of all types (facilitates listing in order of creation)
   * the head of the chain is in sl_type_base::types_ */
  struct sl_type *seq_chain_;

  /* For simple types this is the name of the type, memory is a compile-time global */
  const char *name_;

  /* For a struct, this is the name of the struct */
  char *tag_;
  struct situs tag_loc_;

  /* If the type is sltk_qualifier, the derived type points to the type that the
   * qualifiers in qualifiers_ apply to.
   * If the type is sltk_array, the derived type points to the type of the elements
   * in teh array.
   */
  struct sl_type *derived_type_;

  /* If the type is sltk_qualifier, this contains an int with a combination of 
   * SL_TYPE_QUALIFIER_XXX bits set for all qualifiers that apply. The derived_type_
   * points to the type being qualified, this derived type is not itself an sltk_qualifier
   * type (qualifiers are merged into the same sltk_qualifier node.) */
  int qualifiers_;

  /* If the type is sltk_array, this contains the number of elements in the array,
   * which is always a compile time, positive, non-zero, constant. */
  uint64_t array_size_;

  /* If the type is sltk_struct, this is a tail cyclic chain of all the fields in the
   * structure. */
  struct sl_type_field *fields_;
};

struct sl_type_base {
  /* Hash table of all types, each entry is indexed by (hash % SL_TYPE_BASE_HASH_SIZE),
   * each entry is a tail cyclic linked list through sl_type::hash_chain_ */
  struct sl_type *hash_table_[SL_TYPE_BASE_HASH_SIZE];

  /* Chain of all types allocated in the life time of this sl_type_base, linked 
   * together through sl_type::seq_chain_ */
  struct sl_type *types_;

  /* Next unique identifier to assign for any new type (sl_type::id_) */
  uint32_t next_id_;

  /* Fixed primitive types; it is intended that the caller can directly pick and
   * return one of these rather than having to go through the hash table to find
   * the corresponding hashed sl_type_kind_t of a simple type. */
  struct sl_type void_;
  struct sl_type float_;
  struct sl_type int_;
  struct sl_type bool_;
  struct sl_type vec2_;
  struct sl_type vec3_;
  struct sl_type vec4_;
  struct sl_type bvec2_;
  struct sl_type bvec3_;
  struct sl_type bvec4_;
  struct sl_type ivec2_;
  struct sl_type ivec3_;
  struct sl_type ivec4_;
  struct sl_type mat2_;
  struct sl_type mat3_;
  struct sl_type mat4_;
  struct sl_type sampler2D_;
  struct sl_type samplerCube_;
};

void sl_type_base_init(struct sl_type_base *sltb);
void sl_type_base_cleanup(struct sl_type_base *sltb);

/* construct qualified type given the type to derive from, and the extra qualifiers on top.
 * Returns NULL if there is no memory, or if derived_type was NULL.
 * If the derived_type is an sltk_qualifier type, then the qualifiers are merged using OR, and a new
 * type is formed. If extra_qualifiers is 0, then derived_type is returned.
 * A search is performed if an existing sltk_qualifier type exists with the specified qualifiers, or,
 * if no such type is found, a new type is formed and returned. */
struct sl_type *sl_type_base_qualified_type(struct sl_type_base *tb, struct sl_type *derived_type, int extra_qualifiers);

/* construct an array type given the type to derive from and the size of the array.
 * Returns NULL if there is no memory, or if derived_type was NULL.
 * A search is performed if an existing sltk_array type exists with the specified array size and
 * element type, or, if no such type is found, a new type is formed and returned. */
struct sl_type *sl_type_base_array_type(struct sl_type_base *tb, struct sl_type *derived_type, uint64_t array_size);

/* construct a struct type and have that type take ownership of the passed in struct fields.
 * It is permitted for the tag and tag_loc to be NULL (in which case the struct is anonymous.)
 * Does not add the struct to any sym table. Also, important, does not perform any search
 * to see if an identical struct already exists: structs are always unique types. */
struct sl_type *sl_type_base_struct_type(struct sl_type_base *tb, const char *tag, struct situs *tag_loc, struct sl_type_field *fields);

/* The field returned will be chained to itself (e.g. forms a tail cyclic chain by itself, ready for use with sl_type_field_join()) */
struct sl_type_field *sl_type_field_alloc(struct sl_type_base *tb, const char *ident, struct situs *ident_loc, struct sl_type *field_type);

/* Joins two tail-cyclic chains of sl_type_fields together and returns the last element of the new chain */
struct sl_type_field *sl_type_field_join(struct sl_type_field *front, struct sl_type_field *back);

/* Frees all fields in the chain */
void sl_type_field_free_chain(struct sl_type_field *chain);

/* Returns the type with any qualifiers removed */
struct sl_type *sl_type_unqualified(struct sl_type *t);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_TYPES_H */
