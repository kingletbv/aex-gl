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
#ifndef ATTRIB_BINDING_TABLE_H
#define ATTRIB_BINDING_TABLE_H

typedef enum attrib_binding_table_result {
  ABT_OK = 0,
  ABT_NOMEM = -1,
  ABT_DUPLICATE = -2,
  ABT_NOT_FOUND = -3
} attrib_binding_table_result_t;

struct attrib_binding {
  struct attrib_binding *next_, *prev_, *left_, *right_, *parent_;
  int is_red_:1;

  /* -1 if not bound, otherwise the bound index */
  int bound_index_;

  /* active index, this is the actual index confirmed by linking a program,
   * when bound_index_ is set, it only takes effect after a program is linked,
   * unless no program has been linked (in which case its effect is immediate.)
   * If -1, then not bound, which might be the case if the attribute was
   * bound manually (via bound_index_) but not yet linked. */
  int active_index_;

  /* Pointer to the vertex variable this is bound to, post program linking.
   * Can be NULL if the variable is not declared or used in the vertex program. */
  struct sl_variable *var_;

  /* name of the attribute as it would appear in vertex shader code. */
  size_t name_len_;
  char name_[1];
};

struct attrib_binding_table {
  /* Red-Black tree of all bound attributes with their name_ as key */
  struct attrib_binding *root_;

  /* Cyclic chain of all bound attributes in order of creation */
  struct attrib_binding *seq_;
};

void abt_init(struct attrib_binding_table *abt);
void abt_cleanup(struct attrib_binding_table *abt);

struct attrib_binding *abt_find(struct attrib_binding_table *abt, const char *name, size_t name_len);
attrib_binding_table_result_t abt_find_or_insert(struct attrib_binding_table *abt, const char *name, size_t name_len, struct attrib_binding **new_ab);
void abt_remove(struct attrib_binding_table *abt, struct attrib_binding *ab);

#endif /* ATTRIB_BINDING_TABLE_H */
