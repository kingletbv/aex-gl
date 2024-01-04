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

#ifndef SL_FRAME_H_INCLUDED
#define SL_FRAME_H_INCLUDED
#include "sl_frame.h"
#endif

#ifndef SYM_TABLE_H_INCLUDED
#define SYM_TABLE_H_INCLUDED
#include "sym_table.h"
#endif

void sl_variable_init(struct sl_variable *v) {
  v->name_ = NULL;
  situs_init(&v->location_);
  v->chain_ = NULL;
  v->type_ = NULL;
  sl_expr_temp_init_void(&v->value_);
  v->symbol_ = NULL;
}

void sl_variable_cleanup(struct sl_variable *v) {
  situs_cleanup(&v->location_);
  sl_expr_temp_cleanup(&v->value_);
  /* We don't free the symbol, as it is owned by the scope. */
}

void sl_frame_init(struct sl_frame *frame) {
  frame->variables_ = NULL;
}

void sl_frame_cleanup(struct sl_frame *frame) {
  struct sl_variable *v = frame->variables_;
  if (v) {
    struct sl_variable *next = v->chain_;
    do {
      v = next;
      next = v->chain_;

      sl_variable_cleanup(v);
      free(v);

    } while (v != frame->variables_);
  }
}

static void sl_frame_add_variable(struct sl_frame *frame, struct sl_variable *v) {
  if (frame->variables_) {
    v->chain_ = frame->variables_->chain_;
    frame->variables_->chain_ = v;
  } else {
    v->chain_ = v;
  }
  frame->variables_ = v;
}

struct sl_variable *sl_frame_alloc_variable(struct sl_frame *frame, const char *name, const struct situs *loc, struct sl_type *vartype) {
  struct sl_variable *v = (struct sl_variable *)malloc(sizeof(struct sl_variable));
  if (!v) return NULL;
  sl_variable_init(v);
  if (loc && situs_clone(&v->location_, loc)) {
    /* Failed memory alloc */
    sl_variable_cleanup(v);
    free(v);
    return NULL;
  }
  if (name) {
    v->name_ = strdup(name);
    if (!v->name_) {
      sl_variable_cleanup(v);
      free(v);
      return NULL;
    }
  }
  v->type_ = vartype;
  if (sl_expr_temp_init(&v->value_, vartype)) {
    /* Failed to init value due to memory allocation failure */
    sl_variable_cleanup(v);
    free(v);
    return NULL;
  }

  sl_frame_add_variable(frame, v);
  return v;
}

struct sl_function *sl_frame_alloc_function(const char *name, const struct situs *loc, struct sl_type *return_type) {
  struct sl_function *f = (struct sl_function *)malloc(sizeof(struct sl_function));
  if (!f) return NULL;
  f->name_ = NULL;
  situs_init(&f->location_);
  f->return_type_ = return_type;
  f->num_parameters_ = 0;
  f->num_parameters_allocated_ = 0;
  f->parameters_ = NULL;
  f->symbol_ = NULL;
  if (loc && situs_clone(&f->location_, loc)) {
    /* Failed memory alloc */
    situs_cleanup(&f->location_);
    free(f);
    return NULL;
  }
  if (name) {
    f->name_ = strdup(name);
    if (!f->name_) {
      situs_cleanup(&f->location_);
      free(f);
      return NULL;
    }
  }
  return f;
}

void sl_parameter_cleanup(struct sl_parameter *p) {
  if (p->name_) free(p->name_);
  situs_cleanup(&p->location_);
}

void sl_frame_free_function(struct sl_function *f) {
  size_t n;
  if (!f) return;
  for (n = 0; n < f->num_parameters_; ++n) {
    sl_parameter_cleanup(f->parameters_ + n);
  }
  if (f->parameters_) {
    free(f->parameters_);
  }
  situs_cleanup(&f->location_);
  if (f->name_) free(f->name_);
  free(f);
}

size_t sl_function_append_parameter(struct sl_function *f, const char *name, const struct situs *loc, struct sl_type *type) {
  if (f->num_parameters_ == f->num_parameters_allocated_) {
    size_t new_num_parameters_allocated = f->num_parameters_allocated_ + f->num_parameters_allocated_ + 1;
    struct sl_parameter *new_parameters = (struct sl_parameter *)realloc(f->parameters_, sizeof(struct sl_parameter) * new_num_parameters_allocated);
    if (!new_parameters) return SIZE_MAX;
    f->parameters_ = new_parameters;
    f->num_parameters_allocated_ = new_num_parameters_allocated;
  }
  struct sl_parameter *p = f->parameters_ + f->num_parameters_;
  p->name_ = NULL;
  situs_init(&p->location_);
  p->type_ = type;
  p->symbol_ = NULL;
  if (loc && situs_clone(&p->location_, loc)) {
  /* Failed memory alloc */
    situs_cleanup(&p->location_);
    return SIZE_MAX;
  }
  if (name) {
    p->name_ = strdup(name);
    if (!p->name_) {
      situs_cleanup(&p->location_);
      return SIZE_MAX;
    }
  }
  f->num_parameters_++;
  return f->num_parameters_ - 1;
}
