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
#ifndef NAMED_OBJECT_TABLE_H
#define NAMED_OBJECT_TABLE_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum named_object_table_result {
  NOT_OK = 0,
  NOT_NOMEM = -1,
  NOT_DUPLICATE = -2,
  NOT_NOT_FOUND = -3
} named_object_table_result_t;

struct named_object {
  struct named_object *next_, *prev_, *left_, *right_, *parent_;
  int is_red_:1;

  uintptr_t name_;
};

struct named_object_table {
  /* Red-Black tree of all named objects with their name_ as key */
  struct named_object *root_;

  /* Cyclic chain of all named objects in order of creation */
  struct named_object *seq_;
};

/* Careful with this: in normal operation, not_find_or_insert() will allocate the object
 * of size named_object_size() and invoke named_object_init() on it, after which it'll
 * place the object in the tree. The caller is then expected to initialize the remainder
 * of the object. However, if the caller were to also call named_object_init(), it would
 * corrupt the datastructure by resetting parts of it! The use-case for calling it yourself
 * are when the object you're initializing will never go into a named_object_table and
 * you'd like to initialize it to clean values; that's probably not something you'll encounter
 * often.
 * In summary: don't call this for objects in a named_object_table. */
void named_object_init(struct named_object *no);
void named_object_cleanup(struct named_object *no);

void not_init(struct named_object_table *not);
void not_cleanup(struct named_object_table *not);

struct named_object *not_find(struct named_object_table *not, uintptr_t name);

/* named_object is intended to be the first field in the struct that
 * implements the corresponding named object (framebuffer, texture,
 * renderbuffer, etc.), hence having to pass named_object_size
 * into these functions, to ensure the allocation is large enough.
 * If a new named_object is returned (e.g. NOT_OK and not NOT_DUPLICATE)
 * then the caller needs to initialize all fields not directly associated
 * with struct named_object but with the struct containing the named_object.
 */
named_object_table_result_t not_find_or_insert(struct named_object_table *not, uintptr_t name, size_t named_object_size, struct named_object **new_ab);

void not_remove(struct named_object_table *not, struct named_object *no);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ATTRIB_BINDING_TABLE_H */
