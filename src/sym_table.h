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

#ifndef SYM_TABLE_H
#define SYM_TABLE_H

#ifndef STDDEF_H_INCLUDED
#define STDDEF_H_INCLUDED
#include <stddef.h> /* size_t */
#endif

#ifndef SITUS_H_INCLUDED
#define SITUS_H_INCLUDED
#include "pp/situs.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct sym;
struct sl_type;
struct sl_variable;

typedef enum sym_table_result {
  STR_OK = 0,
  STR_NOMEM = -1,
  STR_DUPLICATE = -2,
  STR_NOT_FOUND = -3
} sym_table_result_t;

typedef enum sym_kind {
  SK_STRUCT,
  SK_FUNCTION,
  SK_VARIABLE
} sym_kind_t;

struct sym_table {
  /* Root of red-black tree of symbols. */
  struct sym *root_;

  /* First symbol in cyclic list, in order of insertion. */
  struct sym *seq_;

  /* Parent symbol table; linear chain to outer scopes until NULL. */
  struct sym_table *parent_;

};

struct sym {
  /* Red-black tree fields */
  int is_red_:1;
  struct sym *left_, *right_, *parent_;

  /* Next and previous symbols in insertion order, head of this is at sym_table::seq_ */
  struct sym *next_, *prev_;

  /* Symbol name */
  struct situs loc_;    /* location of symbol name in input */
  size_t name_len_;     /* length of symbol name excl. null terminator */
  char *name_;          /* null-terminated symbol name (allocated as part of sym) */

  /* Kind of symbol */
  sym_kind_t kind_;
  union {
    struct sl_type *type_;          /* valid for SK_STRUCT */
    struct sl_variable *variable_;  /* valid for SK_VARIABLE */
  } v_;
};

void sym_init(struct sym *s);
void sym_cleanup(struct sym *s);

void st_init(struct sym_table *st, struct sym_table *parent);

/* important: st_cleanup() will call sym_cleanup() and free any remaining symbols in the table(!!!)
 *            this may very likely not be what you want! If your embracing symbol structure is larger
 *            and requires its own deconstruction, be sure to st_remove(sym_table::seq_) until seq_
 *            is NULL. */
void st_cleanup(struct sym_table *st);

struct sym *st_find(struct sym_table *st, const char *name, size_t name_len);
sym_table_result_t st_find_or_insert(struct sym_table *st, sym_kind_t kind, const char *name, size_t name_len, const struct situs *loc, size_t full_sym_size, struct sym **new_sym);
void st_remove(struct sym_table *st, struct sym *s);

/* Tests internals, returns 0 upon success.
 * Will write to stdout and stderr. */
int st_test(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SYM_TABLE_H */
