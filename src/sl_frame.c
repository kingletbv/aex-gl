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

#ifndef DIAGS_H_INCLUDED
#define DIAGS_H_INCLUDED
#include "pp/diags.h"
#endif

#ifndef SL_TYPES_H_INCLUDED
#define SL_TYPES_H_INCLUDED
#include "sl_types.h"
#endif

#ifndef SL_FRAME_H_INCLUDED
#define SL_FRAME_H_INCLUDED
#include "sl_frame.h"
#endif

#ifndef SYM_TABLE_H_INCLUDED
#define SYM_TABLE_H_INCLUDED
#include "sym_table.h"
#endif

#ifndef SL_STMT_H_INCLUDED
#define SL_STMT_H_INCLUDED
#include "sl_stmt.h"
#endif

void sl_variable_init(struct sl_variable *v) {
  v->name_ = NULL;
  situs_init(&v->location_);
  v->chain_ = NULL;
  v->type_ = NULL;
  sl_expr_temp_init_void(&v->value_);
  v->symbol_ = NULL;
  v->is_parameter_ = 0;
  v->parameter_index_ = 0;
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
  f->overload_chain_ = f;
  f->chain_ = f;
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
  sl_frame_init(&f->frame_);
  f->body_ = NULL;
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
  sl_stmt_free_list(f->body_);
  sl_frame_cleanup(&f->frame_);
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
  p->variable_ = NULL;
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

int sl_function_post_process_parameters(struct sl_type_base *tb, struct sl_function *f) {
  /* Convert "int f(void)" to "int f()" - the C style "no arguments" prototype is
    * permitted but not necessary in GLSL (6.1 p. 54) */
  if ((f->num_parameters_ == 1) && (f->parameters_[0].type_ == &tb->void_)) {
    sl_parameter_cleanup(f->parameters_);
    f->num_parameters_--;
  }
  return 0;
}

int sl_function_match(struct sl_function *fa, struct sl_function *fb) {
  if (fa->num_parameters_ != fb->num_parameters_) {
    return 0;
  }
  size_t n;
  for (n = 0; n < fa->num_parameters_; ++n) {
    struct sl_parameter *pa = fa->parameters_ + n;
    struct sl_parameter *pb = fb->parameters_ + n;
    struct sl_type *ta = sl_type_unqualified(pa->type_);
    struct sl_type *tb = sl_type_unqualified(pb->type_);
    if (pa->type_ != pb->type_) {
      return 0;
    }
  }

  return 1;
}

int sl_function_match_parameter_types(struct sl_function *f, size_t num_parameters, struct sl_type **parameter_types) {
  if (f->num_parameters_ != num_parameters) {
    return 0;
  }
  size_t n;
  for (n = 0; n < f->num_parameters_; ++n) {
    struct sl_parameter *p = f->parameters_ + n;
    struct sl_type *t = sl_type_unqualified(p->type_);
    struct sl_type *param_type = parameter_types[n];
    param_type = sl_type_unqualified(param_type);
    if (t != param_type) {
      return 0;
    }
  }

  return 1;
}

int sl_function_match_validation(struct diags *dx, struct sl_function *fnew, struct sl_function *fexisting) {
  if (!sl_function_match(fnew, fexisting)) {
    /* Functions don't match */
    dx_error_loc(dx, &fnew->location_, "function \"%s\" does not match previous declaration (see line %d)", fnew->name_, situs_line(&fexisting->location_));

    return -1;
  }

  /* Functions match, they should not have different parameter (in/out/inout) qualifiers */
  size_t n;
  for (n = 0; n < fnew->num_parameters_; ++n) {
    struct sl_parameter *pnew = fnew->parameters_ + n;
    struct sl_parameter *pexisting = fexisting->parameters_ + n;
    int qualifiers_new = sl_type_qualifiers(pnew->type_);
    int qualifiers_existing = sl_type_qualifiers(pexisting->type_);
    if ((qualifiers_new ^ qualifiers_existing) & SL_PARAMETER_QUALIFIER_MASK) {
      dx_error_loc(dx, &pnew->location_, "parameter \"%s\" of function \"%s\" does not match previous declaration (see line %d)", pnew->name_, fnew->name_, situs_line(&pexisting->location_));
      return -1;
    }
  }

  return 0;
}

void sl_function_search(struct sym_table *current_scope, struct sl_function *f, 
                        struct sym_table **ppst_found_at, struct sym **ppsym_found_at, struct sl_function **ppfunc_found) {
  struct sym_table *st;
  /* bug ? also check/fix sl_function_call_search() below */

  if (ppst_found_at) *ppst_found_at = NULL;
  if (ppsym_found_at) *ppsym_found_at = NULL;
  if (ppfunc_found) *ppfunc_found = NULL;

  if (!f->name_) {
    /* Anonymous functions can never be found */
    return;
  }

  size_t name_len = strlen(f->name_);
  for (st = current_scope; st; st = st->parent_) {
    struct sym *s = st_find(st, f->name_, name_len);
    if (s) {
      if (s->kind_ == SK_FUNCTION) {
        struct sl_function *cf = s->v_.function_;
        if (cf) {
          do {
            cf = cf->overload_chain_;

            if (sl_function_match(f, cf)) {
              /* Found a match */
              if (ppst_found_at) *ppst_found_at = st;
              if (ppsym_found_at) *ppsym_found_at = s;
              if (ppfunc_found) *ppfunc_found = cf;
              return;
            }

          } while (cf != s->v_.function_);
        }
      }
      else {
        /* Not a function, but its presence occludes our vision of
         * any functions lower than this. */
        if (ppst_found_at) *ppst_found_at = st;
        if (ppsym_found_at) *ppsym_found_at = s;
        if (ppfunc_found) *ppfunc_found = NULL;
        return;
      }
    }
  }

  /* Not found */
}

void sl_function_call_search(struct sym_table *current_scope, const char *name, size_t num_params, struct sl_type **param_types,
                             struct sym_table **ppst_found_at, struct sym **ppsym_found_at, struct sl_function **ppfunc_found) {
  /* bug ? also check/fix sl_function_search() above */
  struct sym_table *st;
  /* very similar to sl_function_search, but from the perspective of a function call (so we have the types of the parameters,
   * and nothing else. */

  if (ppst_found_at) *ppst_found_at = NULL;
  if (ppsym_found_at) *ppsym_found_at = NULL;
  if (ppfunc_found) *ppfunc_found = NULL;

  if (!name) {
    /* Anonymous functions can never be found */
    return;
  }

  size_t name_len = strlen(name);
  for (st = current_scope; st; st = st->parent_) {
    struct sym *s = st_find(st, name, name_len);
    if (s) {
      if (s->kind_ == SK_FUNCTION) {
        struct sl_function *cf = s->v_.function_;
        if (cf) {
          do {
            cf = cf->overload_chain_;

            if (sl_function_match_parameter_types(cf, num_params, param_types)) {
              /* Found a match */
              if (ppst_found_at) *ppst_found_at = st;
              if (ppsym_found_at) *ppsym_found_at = s;
              if (ppfunc_found) *ppfunc_found = cf;
              return;
            }

          } while (cf != s->v_.function_);
        }
      }
      else {
        /* Not a function, but its presence occludes our vision of
         * any functions lower than this. */
        if (ppst_found_at) *ppst_found_at = st;
        if (ppsym_found_at) *ppsym_found_at = s;
        if (ppfunc_found) *ppfunc_found = NULL;
        return;
      }
    }
  }

  /* Not found */
}
