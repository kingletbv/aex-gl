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

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef SL_TYPES_H_INCLUDED
#define SL_TYPES_H_INCLUDED
#include "sl_types.h"
#endif

static int sl_type_hash_array(uint64_t array_size, struct sl_type *derived_type) {
  uint32_t v = 0;
  v = (uint32_t)sltk_array;
  v = (v << 7) | (v >> (32 - 7));
  v += (uint32_t)array_size;
  v = (v << 7) | (v >> (32 - 7));
  v += (uint32_t)(array_size >> 32);
  v = (v << 7) | (v >> (32 - 7));
  v += (uint32_t)derived_type->id_;
  return (int)(v % SL_TYPE_BASE_HASH_SIZE);
}

static int sl_type_hash_qualifiers(int qualifiers, struct sl_type *derived_type) {
  uint32_t v = 0;
  v = (uint32_t)sltk_qualifier;
  v = (v << 7) | (v >> (32 - 7));
  v += (uint32_t)qualifiers;
  v = (v << 7) | (v >> (32 - 7));
  v += (uint32_t)derived_type->id_;
  return (int)(v % SL_TYPE_BASE_HASH_SIZE);
}

static int sl_type_hash(struct sl_type *t) {
  uint32_t v = 0;
  switch (t->kind_) {
    case sltk_array:
      return sl_type_hash_array(t->array_size_, t->derived_type_);
    case sltk_qualifier:
      return sl_type_hash_qualifiers(t->qualifiers_, t->derived_type_);
    default:
      break;
  }
  v += t->id_;
  return (int)(v % SL_TYPE_BASE_HASH_SIZE);
}

static void sl_type_field_init(struct sl_type_field *field) {
  field->chain_ = NULL;
  field->ident_ = NULL;
  situs_init(&field->ident_loc_);
  field->type_ = NULL;
}

static void sl_type_field_cleanup(struct sl_type_field *field) {
  if (field->ident_) free(field->ident_);
  situs_cleanup(&field->ident_loc_);
}

static void sl_type_init(struct sl_type *t, struct sl_type_base *tb) {
  t->id_ = tb->next_id_++;
  t->kind_ = sltk_invalid;
  t->hash_chain_ = NULL;
  if (tb->types_) {
    t->seq_chain_ = tb->types_->seq_chain_;
    tb->types_->seq_chain_ = t;
    tb->types_ = t;
  }
  else {
    tb->types_ = t->seq_chain_ = t;
  }
  t->name_ = NULL;
  t->tag_ = NULL;
  situs_init(&t->tag_loc_);
  t->derived_type_ = NULL;
  t->qualifiers_ = 0;
  t->array_size_ = 0;
  t->fields_ = NULL;
}

static void sl_type_cleanup(struct sl_type *t) {
  if (t->tag_) free(t->tag_);
  situs_cleanup(&t->tag_loc_);

  struct sl_type_field *field;
  field = t->fields_;
  if (field) {
    struct sl_type_field *next = field->chain_;
    do {
      field = next;
      next = field->chain_;

      sl_type_field_cleanup(field);

      free(field);

    } while (field != t->fields_);
    t->fields_ = NULL;
  }
}

static void sl_type_base_insert_type_into_hashtable(struct sl_type_base *tb, struct sl_type *t) {
  int hash = sl_type_hash(t);
  if (tb->hash_table_[hash]) {
    t->hash_chain_ = tb->hash_table_[hash]->hash_chain_;
    tb->hash_table_[hash]->hash_chain_ = t;
  }
  else {
    t->hash_chain_ = t;
  }
  tb->hash_table_[hash] = t;
}

void sl_type_base_init(struct sl_type_base *tb) {
  size_t n;
  for (n = 0; n < sizeof(tb->hash_table_)/sizeof(*tb->hash_table_); ++n) {
    tb->hash_table_[n] = NULL;
  }

  tb->types_ = NULL;
  tb->next_id_ = 1;
  struct {
    struct sl_type *obj_;
    sl_type_kind_t kind_;
    const char *name_;
  } simple_types[] = {
    &tb->void_, sltk_void, "void",
    &tb->float_, sltk_float, "float",
    &tb->int_, sltk_int, "int",
    &tb->bool_, sltk_bool, "bool",
    &tb->vec2_, sltk_vec2, "vec2",
    &tb->vec3_, sltk_vec3, "vec3",
    &tb->vec4_, sltk_vec4, "vec4",
    &tb->bvec2_, sltk_bvec2, "bvec2",
    &tb->bvec3_, sltk_bvec3, "bvec3",
    &tb->bvec4_, sltk_bvec4, "bvec4",
    &tb->ivec2_, sltk_ivec2, "ivec2",
    &tb->ivec3_, sltk_ivec3, "ivec3",
    &tb->ivec4_, sltk_ivec4, "ivec4",
    &tb->mat2_, sltk_mat2, "mat2",
    &tb->mat3_, sltk_mat3, "mat3",
    &tb->mat4_, sltk_mat4, "mat4",
    &tb->sampler2D_, sltk_sampler2D, "sampler2D",
    &tb->samplerCube_, sltk_samplerCube, "samplerCube"
  };
  for (n = 0; n < sizeof(simple_types)/sizeof(*simple_types); ++n) {
    struct sl_type *t = simple_types[n].obj_;
    sl_type_init(t, tb);
    t->name_ = simple_types[n].name_;
    t->kind_ = simple_types[n].kind_;
    sl_type_base_insert_type_into_hashtable(tb, t);
  }
}

void sl_type_base_cleanup(struct sl_type_base *tb) {
  struct sl_type *t;
  t = tb->types_;
  if (t) {
    struct sl_type *next = t->seq_chain_;
    do {
      t = next;
      next = t->seq_chain_;

      if ((t->kind_ >= SL_TYPE_FIRST_STATIC_SIMPLE_TYPE) && (t->kind_ <= SL_TYPE_LAST_STATIC_SIMPLE_TYPE)) {
        sl_type_cleanup(t);
        /* Note: not freeing types that were allocated as an sl_type_base field. */
      }
      else {
        sl_type_cleanup(t);
        free(t);
      }

    } while (t != tb->types_);
    tb->types_ = NULL;
  }
}
