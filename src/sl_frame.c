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
