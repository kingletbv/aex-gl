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

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef STDARG_H_INCLUDED
#define STDARG_H_INCLUDED
#include <stdarg.h>
#endif

#ifndef INTTYPES_H_INCLUDED
#define INTTYPES_H_INCLUDED
#include <inttypes.h>
#endif

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
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

  sl_type_field_free_chain(t->fields_);
  t->fields_ = NULL;
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

struct sl_type *sl_type_base_qualified_type(struct sl_type_base *tb, struct sl_type *derived_type, int extra_qualifiers) {
  int hash;
  if (!derived_type) return NULL; /* silent pass-through of failure */
  if (derived_type->kind_ == sltk_qualifier) {
    /* pop the sltk_qualifier and combine its qualifier mask with the extra_qualifiers */
    extra_qualifiers |= derived_type->qualifiers_;
    derived_type = derived_type->derived_type_;
  }

  if (!extra_qualifiers) {
    /* No qualifiers means no sltk_qualifier type but just the original type */
    return derived_type;
  }

  hash = sl_type_hash_qualifiers(extra_qualifiers, derived_type);
  struct sl_type *t;
  t = tb->hash_table_[hash];
  if (t) {
    struct sl_type *next = t->hash_chain_;
    do {
      t = t->hash_chain_;

      if ((t->kind_ == sltk_qualifier) &&
          (t->qualifiers_ == extra_qualifiers) &&
          (t->derived_type_ == derived_type)) {
        /* Found pre-existing matching type, return it */
        return t;
      }

    } while (t != tb->hash_table_[hash]);
  }
    
  /* Create new type */
  t = (struct sl_type *)malloc(sizeof(struct sl_type));
  if (!t) return NULL; /* no memory */
  sl_type_init(t, tb);
  t->kind_ = sltk_qualifier;
  t->derived_type_ = derived_type;
  t->qualifiers_ = extra_qualifiers;
  if (tb->hash_table_[hash]) {
    t->hash_chain_ = tb->hash_table_[hash]->hash_chain_;
    tb->hash_table_[hash]->hash_chain_ = t->hash_chain_;
  }
  else {
    t->hash_chain_ = t;
  }
  tb->hash_table_[hash] = t;

  return t;
}

struct sl_type *sl_type_base_array_type(struct sl_type_base *tb, struct sl_type *derived_type, uint64_t array_size) {
  int hash;
  if (!derived_type) return NULL; /* silent pass-through of failure */

  hash = sl_type_hash_array(array_size, derived_type);
  struct sl_type *t;
  t = tb->hash_table_[hash];
  if (t) {
    struct sl_type *next = t->hash_chain_;
    do {
      t = t->hash_chain_;

      if ((t->kind_ == sltk_array) &&
          (t->array_size_ == array_size) &&
          (t->derived_type_ == derived_type)) {
        /* Found pre-existing matching type, return it */
        return t;
      }

    } while (t != tb->hash_table_[hash]);
  }

  /* Create new type */
  t = (struct sl_type *)malloc(sizeof(struct sl_type));
  if (!t) return NULL; /* no memory */
  sl_type_init(t, tb);
  t->kind_ = sltk_array;
  t->derived_type_ = derived_type;
  t->array_size_ = array_size;
  if (tb->hash_table_[hash]) {
    t->hash_chain_ = tb->hash_table_[hash]->hash_chain_;
    tb->hash_table_[hash]->hash_chain_ = t->hash_chain_;
  }
  else {
    t->hash_chain_ = t;
  }
  tb->hash_table_[hash] = t;

  return t;
}

struct sl_type *sl_type_base_struct_type(struct sl_type_base *tb,const char *tag,struct situs *tag_loc,struct sl_type_field *fields) {
  struct sl_type *t;
  t = (struct sl_type *)malloc(sizeof(struct sl_type));
  if (!t) return NULL; /* no memory */
  sl_type_init(t, tb);
  t->kind_ = sltk_struct;
  if (tag) {
    size_t tag_len = strlen(tag);
    t->tag_ = (char *)malloc(tag_len + 1);
    if (!t->tag_) {
      free(t);
      return NULL; /* no memory */
    }
    memcpy(t->tag_, tag, tag_len);
    t->tag_[tag_len] = '\0';
  }
  if (tag_loc) {
    if (situs_clone(&t->tag_loc_,tag_loc)) {
      situs_cleanup(&t->tag_loc_);
      free(t->tag_);
      free(t);
      return NULL; /* no memory */
    }
  }
  return t;
}


struct sl_type_field *sl_type_field_join(struct sl_type_field *front, struct sl_type_field *back) {
  if (!back) return front;
  if (!front) return back;

  struct sl_type_field *front_head = front->chain_;
  struct sl_type_field *front_tail = front;

  struct sl_type_field *back_head = back->chain_;
  struct sl_type_field *back_tail = back;

  back_tail->chain_ = front_head;
  front_tail->chain_ = back_head;

  return back_tail;
}


struct sl_type_field *sl_type_field_alloc(struct sl_type_base *tb, const char *ident, struct situs *ident_loc, struct sl_type *field_type) {
  struct sl_type_field *tf = (struct sl_type_field *)malloc(sizeof(struct sl_type_field));
  if (!tf) return NULL;
  if (ident) {
    size_t ident_len = strlen(ident);
    tf->ident_ = (char *)malloc(ident_len + 1);
    if (!tf->ident_) {
      free(tf);
      return NULL;
    }
    memcpy(tf->ident_, ident, ident_len);
    tf->ident_[ident_len] = '\0';
  }
  else {
    tf->ident_ = NULL;
  }
  situs_init(&tf->ident_loc_);
  if (situs_clone(&tf->ident_loc_, ident_loc)) {
    free(tf->ident_);
    free(tf);
    return NULL;
  }
  tf->type_ = field_type;
   
  tf->chain_ = tf;

  return tf;
}

void sl_type_field_free_chain(struct sl_type_field *chain) {
  struct sl_type_field *field;
  field = chain;
  if (field) {
    struct sl_type_field *next = field->chain_;
    do {
      field = next;
      next = field->chain_;

      sl_type_field_cleanup(field);

      free(field);

    } while (field != chain);
  }
}

static size_t sl_type_str_dump(const char *s, char *output_str, size_t at) {
  size_t len = strlen(s);
  if (output_str) memcpy(output_str + at, s, len);
  return at + len;
}

size_t sl_type_dump(const struct sl_type *t, char *output_str) {
  size_t str_size = 0;
  switch (t->kind_) {
    case sltk_invalid:
      str_size = sl_type_str_dump("@INVALID", output_str, str_size);
      break;
    case sltk_qualifier: {
      int q = t->qualifiers_;

      /* invariant-qualifier */
      if (q & SL_TYPE_QUALIFIER_INVARIANT) {
        str_size = sl_type_str_dump("invariant ", output_str, str_size);
      }

      /* storage qualifiers */
      if (q & SL_TYPE_QUALIFIER_CONST) {
        str_size = sl_type_str_dump("const ", output_str, str_size);
      }
      if (q & SL_TYPE_QUALIFIER_ATTRIBUTE) {
        str_size = sl_type_str_dump("attribute ", output_str, str_size);
      }
      if (q & SL_TYPE_QUALIFIER_UNIFORM) {
        str_size = sl_type_str_dump("uniform ", output_str, str_size);
      }
      if (q & SL_TYPE_QUALIFIER_VARYING) {
        str_size = sl_type_str_dump("varying ", output_str, str_size);
      }

      /* parameter-qualifiers */
      if (q & SL_PARAMETER_QUALIFIER_IN) {
        str_size = sl_type_str_dump("in ", output_str, str_size);
      }
      if (q & SL_PARAMETER_QUALIFIER_OUT) {
        str_size = sl_type_str_dump("out ", output_str, str_size);
      }
      if (q & SL_PARAMETER_QUALIFIER_INOUT) {
        str_size = sl_type_str_dump("inout ", output_str, str_size);
      }

      /* precision-qualifiers */
      if (q & SL_TYPE_QUALIFIER_HIGH_PRECISION) {
        str_size = sl_type_str_dump("highp ", output_str, str_size);
      }
      if (q & SL_TYPE_QUALIFIER_MEDIUM_PRECISION) {
        str_size = sl_type_str_dump("mediump ", output_str, str_size);
      }
      if (q & SL_TYPE_QUALIFIER_LOW_PRECISION) {
        str_size = sl_type_str_dump("lowp ", output_str, str_size);
      }

      str_size += sl_type_dump(t->derived_type_, output_str ? output_str + str_size : NULL);
      break;
    }

    case sltk_array: {
      struct sl_type *non_array_child = t->derived_type_;
      while (non_array_child->kind_ == sltk_array) {
        non_array_child = non_array_child->derived_type_;
      }
      str_size += sl_type_dump(non_array_child, output_str ? output_str + str_size : NULL);
      while (t->kind_ == sltk_array) {
        str_size = sl_type_str_dump("[", output_str, str_size);
        char digits[21] = { 0 };
        sprintf(digits, "%" PRIu64, t->array_size_);
        str_size = sl_type_str_dump(digits, output_str, str_size);
        str_size = sl_type_str_dump("]", output_str, str_size);

        t = t->derived_type_;
      }
      break;
    }
    case sltk_struct: {
      str_size = sl_type_str_dump(t->tag_, output_str, str_size);
      break;
    }

    case sltk_void:
    case sltk_float:
    case sltk_int:
    case sltk_bool:
    case sltk_vec2:
    case sltk_vec3:
    case sltk_vec4:
    case sltk_bvec2:
    case sltk_bvec3:
    case sltk_bvec4:
    case sltk_ivec2:
    case sltk_ivec3:
    case sltk_ivec4:
    case sltk_mat2:
    case sltk_mat3:
    case sltk_mat4:
    case sltk_sampler2D:
    case sltk_samplerCube:
      str_size = sl_type_str_dump(t->name_, output_str, str_size);
      break;
  }
  return str_size;
}


char *sl_type_to_str(const struct sl_type *t) {
  size_t size_needed = sl_type_dump(t, NULL);
  char *buf = (char *)malloc(size_needed + 1 /* NULL terminator */);
  if (!buf) return NULL;
  sl_type_dump(t, buf);
  buf[size_needed] = '\0';

  return buf;
}

size_t sl_buf_printf(char **p, size_t *buf_size_remaining, size_t *total_needed, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int n = vsnprintf(*p, *buf_size_remaining, fmt, ap);
  va_end(ap);
  if (n < 0) {
    /* error */
    return *total_needed;
  }
  *total_needed += n;
  if (*buf_size_remaining > (size_t)n) {
    /* output truncated */
    *buf_size_remaining -= n;
    *p += n;
  }
  else {
    *p += *buf_size_remaining;
    *buf_size_remaining = 0;
  }
  return *total_needed;
}
