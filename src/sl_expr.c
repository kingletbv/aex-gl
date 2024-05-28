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

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef SL_EXPR_H_INCLUDED
#define SL_EXPR_H_INCLUDED
#include "sl_expr.h"
#endif

#ifndef DIAGS_H_INCLUDED
#define DIAGS_H_INCLUDED
#include "pp/diags.h"
#endif

void sl_expr_init(struct sl_expr *x) {
  x->op_ = exop_invalid;
  situs_init(&x->op_loc_);
  x->num_children_ = 0;
  x->children_ = NULL;
  sl_expr_temp_init_void(&x->literal_value_);
  sl_reg_alloc_init(&x->base_regs_);
  sl_reg_alloc_init(&x->offset_reg_);
  x->offset_limit_ = 1;
  sl_reg_alloc_init(&x->rvalue_);
  x->num_components_ = 0;
  memset(x->component_selection_, 0, sizeof(x->component_selection_));
  x->field_selection_ = NULL;
  x->field_index_ = 0;
  x->function_ = NULL;
  x->next_caller_ = x->prev_caller_ = NULL;
  x->constructor_type_ = NULL;
  x->variable_ = NULL;
}

void sl_expr_cleanup(struct sl_expr *x) {
  if (x->children_) {
    /* We don't free the entire tree as that would assume the allocation,
     * use sl_expr_free() for this. */
    free(x->children_);
    x->children_ = NULL;
  }
  sl_expr_temp_cleanup(&x->literal_value_);
  sl_reg_alloc_cleanup(&x->base_regs_);
  sl_reg_alloc_cleanup(&x->offset_reg_);
  sl_reg_alloc_cleanup(&x->rvalue_);
  sl_expr_detach_caller(x);
}

void sl_expr_free(struct sl_expr *x) {
  size_t n;
  if (!x) return;
  for (n = 0; n < x->num_children_; ++n) {
    sl_expr_free(x->children_[n]);
  }
  sl_expr_cleanup(x);
  free(x);
}

int sl_expr_is_lvalue(struct sl_type_base *tb, const struct sl_expr *x) {
  switch (x->op_) {
    case exop_invalid:
      return 0;
    case exop_variable: {
      if (!x->variable_ || !x->variable_->type_) return 0;
      int is_constant = !!(sl_type_qualifiers(x->variable_->type_) & SL_TYPE_QUALIFIER_CONST);
      if (is_constant) return 0;
      return 1;
    }
    case exop_literal:
      return 0;
    case exop_array_subscript:
      return sl_expr_is_lvalue(tb, x->children_[0]);
    case exop_component_selection: {
      /* This is an lvalue if the vector is an lvalue and we don't duplicate columns in the selection */
      int is_child_lvalue = sl_expr_is_lvalue(tb, x->children_[0]);
      if (!is_child_lvalue) return 0;
      size_t n;
      if (!x->num_components_) return 0;
      for (n = 0; n < x->num_components_; ++n) {
        size_t dup_check;
        for (dup_check = 0; dup_check < x->num_components_; ++dup_check) {
          if (n == dup_check) continue;
          if (x->swizzle_[n].component_index_ == x->swizzle_[dup_check].component_index_) {
            /* Duplicate column - hence not an lvalue (GLSL 1 5.8 assignments p46) */
            return 0;
          }
        }
      }
      return 1;
    }
    case exop_field_selection:
      return sl_expr_is_lvalue(tb, x->children_[0]);
    case exop_post_inc:
    case exop_post_dec:
    case exop_pre_inc:
    case exop_pre_dec:
      return 0;
    case exop_negate:
      return 0;
    case exop_logical_not:
      return 0;
    case exop_multiply:
    case exop_divide:
    case exop_add:
    case exop_subtract:
    case exop_lt:
    case exop_le:
    case exop_ge:
    case exop_gt:
    case exop_eq:
    case exop_ne:
      return 0;
    case exop_function_call:
      return 0;
    case exop_constructor:
      return 0;
    case exop_logical_and:
    case exop_logical_or:
    case exop_logical_xor:
      return 0;
    case exop_assign:
    case exop_mul_assign:
    case exop_div_assign:
    case exop_add_assign:
    case exop_sub_assign:
      return 0;
    case exop_sequence:
      return 0;
    case exop_conditional:
      return 0;
    default:
      assert(!"Unhandled expression type");
  }
  return 0;
}

static int sl_expr_validate_mul(struct diags *dx, struct sl_type_base *tb, const struct sl_expr *x) {
  int r = sl_expr_validate(dx, tb, x->children_[0]);
  r |= sl_expr_validate(dx, tb, x->children_[1]);
  if (r & SLXV_INVALID) return r;
  struct sl_expr *x0 = x->children_[0];
  struct sl_expr *x1 = x->children_[1];
  struct sl_type *t0 = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
  struct sl_type *t1 = sl_type_unqualified(sl_expr_type(tb, x->children_[1]));
  if (!t0 || !t1) return r | SLXV_INVALID;
  
  if (t0->kind_ > t1->kind_) {
    struct sl_type *t = t0;
    t0 = t1;
    t1 = t;
    struct sl_expr *e = x0;
    x0 = x1;
    x1 = e;
  }
  switch (t0->kind_) {
    case sltk_float:
      switch (t1->kind_) {
        case sltk_float:
        case sltk_vec2:
        case sltk_vec3:
        case sltk_vec4:
        case sltk_mat2:
        case sltk_mat3:
        case sltk_mat4:
          return r;
        default:
          break;
      }
      break;

    case sltk_vec2:
      switch (t1->kind_) {
        case sltk_vec2:
        case sltk_mat2:
          return r;
        default:
          break;
      }
      break;

    case sltk_vec3:
      switch (t1->kind_) {
        case sltk_vec3:
        case sltk_mat3:
          return r;
        default:
          break;
      }
      break;

    case sltk_vec4:
      switch (t1->kind_) {
        case sltk_vec4:
        case sltk_mat4:
          return r;
        default:
          break;
      }
      break;

    case sltk_mat2:
      if (t1->kind_ == sltk_mat2) {
        return r;
      }
      break;

    case sltk_mat3:
      if (t1->kind_ == sltk_mat3) {
        return r;
      }
      break;

    case sltk_mat4:
      if (t1->kind_ == sltk_mat4) {
        return r;
      }
      break;

    case sltk_int:
      switch (t1->kind_) {
        case sltk_int:
        case sltk_ivec2:
        case sltk_ivec3:
        case sltk_ivec4:
          return r;
        default:
          break;
      }
      break;
    case sltk_ivec2:
      if (t1->kind_ == sltk_ivec2) {
          return r;
      }
      break;
    case sltk_ivec3:
      if (t1->kind_ == sltk_ivec3) {
        return r;
      }
      break;
    case sltk_ivec4:
      if (t1->kind_ == sltk_ivec4) {
        return r;
      }
      break;
    default:
      break;
  }

  dx_error_loc(dx, &x->op_loc_, "Invalid types for multiplication");
  return r | SLXV_INVALID;
}

static int sl_expr_validate_div_sub_add(struct diags *dx, struct sl_type_base *tb, const struct sl_expr *x) {
  int r = sl_expr_validate(dx, tb, x->children_[0]);
  r |= sl_expr_validate(dx, tb, x->children_[1]);
  if (r & SLXV_INVALID) return r;
  struct sl_expr *x0 = x->children_[0];
  struct sl_expr *x1 = x->children_[1];
  struct sl_type *t0 = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
  struct sl_type *t1 = sl_type_unqualified(sl_expr_type(tb, x->children_[1]));
  if (!t0 || !t1) return r | SLXV_INVALID;

  if (t0->kind_ > t1->kind_) {
    struct sl_type *t = t0;
    t0 = t1;
    t1 = t;
    struct sl_expr *e = x0;
    x0 = x1;
    x1 = e;
  }
  switch (t0->kind_) {
    case sltk_float:
      switch (t1->kind_) {
        case sltk_float:
        case sltk_vec2:
        case sltk_vec3:
        case sltk_vec4:
        case sltk_mat2:
        case sltk_mat3:
        case sltk_mat4:
          return r;
        default:
          break;
      }
      break;

    case sltk_vec2:
    case sltk_vec3:
    case sltk_vec4:
      if (t1->kind_ == t0->kind_) {
        return r;
      }
      break;

    case sltk_mat2:
      if (t1->kind_ == sltk_mat2) {
        return r;
      }
      break;

    case sltk_mat3:
      if (t1->kind_ == sltk_mat3) {
        return r;
      }
      break;

    case sltk_mat4:
      if (t1->kind_ == sltk_mat4) {
        return r;
      }
      break;

    case sltk_int:
      switch (t1->kind_) {
        case sltk_int:
        case sltk_ivec2:
        case sltk_ivec3:
        case sltk_ivec4:
          return r;
        default:
          break;
      }
      break;

    case sltk_ivec2:
    case sltk_ivec3:
    case sltk_ivec4:
      if (t1->kind_ == t0->kind_) {
        return r;
      }
      break;

    default:
      break;
  }

  const char *op = "operation";
  switch (x->op_) {
    case exop_divide:
    case exop_div_assign:
      op = "division";
      break;
    case exop_add:
    case exop_add_assign:
      op = "addition";
      break;
    case exop_subtract:
    case exop_sub_assign:
      op = "subtraction";
      break;
    default:
      assert(0 && "Invalid op");
      break;
  }

  dx_error_loc(dx, &x->op_loc_, "Invalid types for %s", op);
  return r | SLXV_INVALID;
}

static int sl_expr_type_contains_array(struct sl_type *t) {
  t = sl_type_unqualified(t);
  if (!t) return 0;
  if (t->kind_ == sltk_array) return 1;
  if (t->kind_ == sltk_struct) {
    struct sl_type_field *f = t->fields_;
    if (f) {
      do {
        if (sl_expr_type_contains_array(f->type_)) return 1;
        f = f->chain_;
      } while (f != t->fields_);
    }
  }
  return 0;
}

static int sl_expr_type_contains_sampler(struct sl_type *t) {
  t = sl_type_unqualified(t);
  if (!t) return 0;
  if (t->kind_ == sltk_sampler2D) return 1;
  if (t->kind_ == sltk_samplerCube) return 1;
  if (t->kind_ == sltk_struct) {
    struct sl_type_field *f = t->fields_;
    if (f) {
      do {
        if (sl_expr_type_contains_sampler(f->type_)) return 1;
        f = f->chain_;
      } while (f != t->fields_);
    }
  }
  return 0;
}

static int sl_expr_vec_mat_num_components(sl_type_kind_t kind) {
  switch (kind) {
    case sltk_float:
    case sltk_int:
    case sltk_bool:
      return 1;
    case sltk_vec2:
    case sltk_ivec2:
    case sltk_bvec2:
      return 2;
    case sltk_vec3:
    case sltk_ivec3:
    case sltk_bvec3:
      return 3;
    case sltk_vec4:
    case sltk_ivec4:
    case sltk_bvec4:
      return 4;
    case sltk_mat2:
      return 4;
    case sltk_mat3:
      return 9;
    case sltk_mat4:
      return 16;
    default:
      assert(0 && "Invalid type");
      return 0;
  }
}

int sl_expr_validate_vec_mat_scalar_constructor(struct diags *dx, struct sl_type_base *tb, const struct sl_expr *x) {
  struct sl_type *t = x->constructor_type_;
  if (!t) return SLXV_INVALID;
  t = sl_type_unqualified(t);
  switch (t->kind_) {
    case sltk_float:
    case sltk_int:
    case sltk_bool: {
      /* Should have one child, which may be a vec, mat, or scalar */
      if (x->num_children_ != 1) {
        dx_error_loc(dx, &x->op_loc_, "Scalar constructor with incorrect number of parameters");
        return SLXV_INVALID;
      }
      int r = sl_expr_validate(dx, tb, x->children_[0]);
      if (r & SLXV_INVALID) return r;
      struct sl_type *t0 = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
      if (!t0) return r | SLXV_INVALID;
      switch (t0->kind_) {
        case sltk_float:
        case sltk_int:
        case sltk_bool:
          /* Initialize, possibly by type conversion */
          return r;
        case sltk_vec2:
        case sltk_vec3:
        case sltk_vec4:
        case sltk_ivec2:
        case sltk_ivec3:
        case sltk_ivec4:
        case sltk_bvec2:
        case sltk_bvec3:
        case sltk_bvec4:
        case sltk_mat2:
        case sltk_mat3:
        case sltk_mat4:
          /* Initialize with the first component of non-scalar type */
          return r;
        default:
          /* Cannot initialize with any other type */
          dx_error_loc(dx, &x->op_loc_, "Scalar constructor with incompatible parameter type");
          return r | SLXV_INVALID;
      }
    }
    case sltk_mat2:
    case sltk_mat3:
    case sltk_mat4: {
      /* A matrix can be constructed from another matrix. However, if a matrix argument is
       * given to a matrix constructor, it is an error to give any other arguments. 5.4.2 p. 42 */
      /* Is there a matrix in the arguments? If so, then valid only if it is the only argument
       * NOTE: If this matrix check passes, and the constructor is not a single matrix, then
       *       we fall through to vecs for additional processing. */
      size_t n;
      int r = 0;
      for (n = 0; n < x->num_children_; ++n) {
        struct sl_type *t = sl_type_unqualified(sl_expr_type(tb, x->children_[n]));
        if (!t) return r | SLXV_INVALID;
        if (t->kind_ == sltk_mat2 ||
            t->kind_ == sltk_mat3 ||
            t->kind_ == sltk_mat4) {
          /* Yes, there is a matrix in the arguments */
          if (x->num_children_ > 1) {
            dx_error_loc(dx, &x->op_loc_, "Matrix constructor with matrix argument must consist of only the matrix argument");
            return r | SLXV_INVALID;
          }
          /* Matrix argument in isolation, yay, return its validation as our result */
          return r | sl_expr_validate(dx, tb, x->children_[n]);
        }
      }
      /* At this point: no matrix argument, fall through and treat matrix constructor as a vector constructor 
       * (e.g. component-by-component construction) */
    }
    case sltk_vec2:
    case sltk_vec3:
    case sltk_vec4:
    case sltk_ivec2:
    case sltk_ivec3:
    case sltk_ivec4:
    case sltk_bvec2:
    case sltk_bvec3:
    case sltk_bvec4: {
      /* Count the number of components in the constructor type */
      int components_needed = sl_expr_vec_mat_num_components(t->kind_);
      int components_remaining = components_needed;
      /* Validate each child */
      size_t n;
      int r = 0;
      for (n = 0; n < x->num_children_; ++n) {
        r |= sl_expr_validate(dx, tb, x->children_[n]);
        if (r & SLXV_INVALID) return r;
      }
      /* Now count the components provided by each */
      for (n = 0; n < x->num_children_; ++n) {
        struct sl_type *t = sl_type_unqualified(sl_expr_type(tb, x->children_[n]));
        if (!t) return r | SLXV_INVALID;
        switch (t->kind_) {
          case sltk_float:
          case sltk_int:
          case sltk_bool:
          case sltk_vec2:
          case sltk_vec3:
          case sltk_vec4:
          case sltk_ivec2:
          case sltk_ivec3:
          case sltk_ivec4:
          case sltk_bvec2:
          case sltk_bvec3:
          case sltk_bvec4:
          case sltk_mat2:
          case sltk_mat3:
          case sltk_mat4: {
            if (components_remaining <= 0) {
              /* All components already filled, so this argument is futile & invalid. */
              dx_error_loc(dx, &x->op_loc_, "Constructor with too many parameters");
              return r | SLXV_INVALID;
            }
            components_remaining -= sl_expr_vec_mat_num_components(t->kind_);
            break;
          }
          default:
            dx_error_loc(dx, &x->op_loc_, "Constructor with incompatible parameter type");
            return r | SLXV_INVALID;
        }
      }
      if (components_remaining > 0) {
        /* Not all components filled */
        dx_error_loc(dx, &x->op_loc_, "Constructor with too few parameters");
        return r | SLXV_INVALID;
      }
      return r;
    }
    default:
      /* Should never get here; caller should not have invoked this function for this type. */
      assert(!!"Invalid type");
      return SLXV_INVALID;
  }
}

int sl_expr_validate(struct diags *dx, struct sl_type_base *tb, const struct sl_expr *x) {
  if (!x) {
    /* If NULL, then x is invalid, however, this would have been reported earlier (causing
     * it to be NULL) - therefore we don't report here. */
    return SLXV_INVALID;
  }

  switch (x->op_) {
    case exop_invalid:
      return SLXV_INVALID;
    case exop_variable: {
      if (!x->variable_) return SLXV_INVALID;
      if (!x->variable_->type_) return SLXV_INVALID;
      if (sl_type_qualifiers(x->variable_->type_) & SL_TYPE_QUALIFIER_CONST) {
        return 0; /* ok but constant */
      }
      return SLXV_NOT_CONSTANT; /* ok */
    }
    case exop_literal: {
      return 0;
    }
    case exop_array_subscript: {
      int r = sl_expr_validate(dx, tb, x->children_[0]);
      int index_validation = sl_expr_validate(dx, tb, x->children_[1]);
      r |= index_validation;
      if (r & SLXV_INVALID) return r;
      struct sl_type *array_type = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
      int typematch = 0;
      switch (array_type->kind_) {
        case sltk_array:
        case sltk_vec2:
        case sltk_vec3:
        case sltk_vec4:
        case sltk_mat2:
        case sltk_mat3:
        case sltk_mat4:
        case sltk_ivec2:
        case sltk_ivec3:
        case sltk_ivec4:
        case sltk_bvec2:
        case sltk_bvec3:
        case sltk_bvec4:
          typematch = 1;
          break;
      }
      if (!typematch) {
        dx_error_loc(dx, &x->op_loc_, "Array subscript on non-indexable type");
        return r | SLXV_INVALID;
      }
      struct sl_type *index_type = sl_type_unqualified(sl_expr_type(tb, x->children_[1]));
      if (index_type->kind_ != sltk_int) {
        dx_error_loc(dx, &x->op_loc_, "Array subscript with non-integral index type");
        return r | SLXV_INVALID;
      }
      if (!(index_validation & SLXV_NOT_CONSTANT)) {
        /* Index is constant and legal int, evaluate it and check it is >= 0 and < array size */
        struct sl_expr_temp index_value;
        sl_expr_temp_init(&index_value, NULL);
        if (sl_expr_eval(tb, x->children_[1], &index_value)) {
          sl_expr_temp_cleanup(&index_value);
          return r | SLXV_INVALID;
        }
        if (index_value.kind_ != sletk_int) {
          sl_expr_temp_cleanup(&index_value);
          return r | SLXV_INVALID;
        }
        if (index_value.v_.i_ < 0) {
          dx_error_loc(dx, &x->op_loc_, "Array subscript with negative index");
          sl_expr_temp_cleanup(&index_value);
          return r | SLXV_INVALID;
        }
        uint64_t num_components = 0;
        switch (array_type->kind_) {
          case sltk_array:
            if (((uint64_t)index_value.v_.i_) >= array_type->array_size_) {
              dx_error_loc(dx, &x->op_loc_, "Array subscript with index >= array size");
              sl_expr_temp_cleanup(&index_value);
              return r | SLXV_INVALID;
            }
            break;
          case sltk_vec2:
          case sltk_ivec2:
          case sltk_bvec2:
          case sltk_mat2:
            num_components = 2;
            break;
          case sltk_vec3:
          case sltk_ivec3:
          case sltk_bvec3:
          case sltk_mat3:
            num_components = 3;
            break;
          case sltk_vec4:
          case sltk_ivec4:
          case sltk_bvec4:
          case sltk_mat4:
            num_components = 4;
            break;
        }
        if (num_components) {
          if (((uint64_t)index_value.v_.i_) >= array_type->array_size_) {
            dx_error_loc(dx, &x->op_loc_, "Array subscript with index >= array size");
            sl_expr_temp_cleanup(&index_value);
            return r | SLXV_INVALID;
          }

        }
        sl_expr_temp_cleanup(&index_value);
      }
      return r;
    }
    case exop_component_selection: /* e.g. myvec3.xxz */ {
      int r = sl_expr_validate(dx, tb, x->children_[0]);
      if (r & SLXV_INVALID) return r;
      struct sl_type *vec_type = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
      int vec_size = 0;
      if (vec_type->kind_ != sltk_vec2 &&
          vec_type->kind_ != sltk_vec3 &&
          vec_type->kind_ != sltk_vec4 &&
          vec_type->kind_ != sltk_bvec2 &&
          vec_type->kind_ != sltk_bvec3 &&
          vec_type->kind_ != sltk_bvec4 &&
          vec_type->kind_ != sltk_ivec2 &&
          vec_type->kind_ != sltk_ivec3 &&
          vec_type->kind_ != sltk_ivec4) {
        dx_error_loc(dx, &x->op_loc_, "Component selection on non-vector type");
        return r | SLXV_INVALID;
      }
      switch (vec_type->kind_) {
        case sltk_vec2:
        case sltk_bvec2:
        case sltk_ivec2:
          vec_size = 2;
          break;
        case sltk_vec3:
        case sltk_bvec3:
        case sltk_ivec3:
          vec_size = 3;
          break;
        case sltk_vec4:
        case sltk_bvec4:
        case sltk_ivec4:
          vec_size = 4;
          break;
        default:
          assert(0 && "Invalid vector type");
          break;
      }
      if (x->num_components_ > 4) {
        dx_error_loc(dx, &x->op_loc_, "Component selection with more than 4 components");
        return r | SLXV_INVALID;
      }
      size_t n;
      for (n = 0; n < x->num_components_; ++n) {
        if (x->swizzle_[n].component_index_ >= vec_size) {
          dx_error_loc(dx, &x->op_loc_, "Component selection with component exceeding vector size");
          return r | SLXV_INVALID;
        }
      }
      return r;
    }
    case exop_field_selection:     /* e.g. mystruct.myfield */ {
      int r = sl_expr_validate(dx, tb, x->children_[0]);
      if (r & SLXV_INVALID) return r;
      struct sl_type_field *tf = x->field_selection_;
      struct sl_type *struct_type = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
      if (struct_type->kind_ != sltk_struct) {
        dx_error_loc(dx, &x->op_loc_, "Field selection on non-struct type");
        return r | SLXV_INVALID;
      }
      if (!tf) {
        dx_error_loc(dx, &x->op_loc_, "Field selection missing field");
        return r | SLXV_INVALID;
      }
      struct sl_type_field *f = struct_type->fields_;
      if (f) {
        do {
          f = f->chain_;

        } while (f != tf && f != struct_type->fields_);
      }
      if (f == tf) {
        /* Found */
        return r;
      }
      /* Field not found */
      dx_error_loc(dx, &x->op_loc_, "Field selection with unknown field");
      return r | SLXV_INVALID;
    }
    case exop_post_inc:
    case exop_post_dec:
    case exop_pre_inc:
    case exop_pre_dec: {
        int r = sl_expr_validate(dx, tb, x->children_[0]);
        if (r & SLXV_INVALID) return r;
        if (!sl_expr_is_lvalue(tb, x->children_[0])) {
          dx_error_loc(dx, &x->op_loc_, "Increment/decrement on non-lvalue");
          return r | SLXV_INVALID;
        }
        struct sl_type *t = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
        switch (t->kind_) {
          case sltk_float:
          case sltk_vec2:
          case sltk_vec3:
          case sltk_vec4:
            break;
          case sltk_int:
          case sltk_ivec2:
          case sltk_ivec3:
          case sltk_ivec4:
            break;
          default:
            dx_error_loc(dx, &x->op_loc_, "Increment/decrement on non-scalar type");
            return r | SLXV_INVALID;
        }
        return r;
    }

    case exop_negate: {
      int r = sl_expr_validate(dx, tb, x->children_[0]);
      if (r & SLXV_INVALID) return r;
      struct sl_type *t = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
      switch (t->kind_) {
        case sltk_float:
        case sltk_vec2:
        case sltk_vec3:
        case sltk_vec4:
          break;
        case sltk_int:
        case sltk_ivec2:
        case sltk_ivec3:
        case sltk_ivec4:
          break;
        default:
          dx_error_loc(dx, &x->op_loc_, "Negate on non-scalar type");
          return r | SLXV_INVALID;
      }
      return r;
    }
    case exop_logical_not: {
      int r = sl_expr_validate(dx, tb, x->children_[0]);
      if (r & SLXV_INVALID) return r;
      struct sl_type *t = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
      if (t->kind_ != sltk_bool) {
        dx_error_loc(dx, &x->op_loc_, "Logical not on non-boolean type");
        return r | SLXV_INVALID;
      }
      return r;
    }
    case exop_multiply:
      return sl_expr_validate_mul(dx, tb, x);

    case exop_divide:
    case exop_add:
    case exop_subtract:
      return sl_expr_validate_div_sub_add(dx, tb, x);

    case exop_lt:
    case exop_le:
    case exop_ge:
    case exop_gt: {
      int r = sl_expr_validate(dx, tb, x->children_[0]);
      r = r | sl_expr_validate(dx, tb, x->children_[1]);
      if (r & SLXV_INVALID) return r;
      struct sl_type *t0 = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
      struct sl_type *t1 = sl_type_unqualified(sl_expr_type(tb, x->children_[1]));
      if ((t0->kind_ != sltk_float) && (t0->kind_ != sltk_int)) {
        dx_error_loc(dx, &x->op_loc_, "Comparison on non-scalar type");
        return r | SLXV_INVALID;
      }
      if ((t1->kind_ != sltk_float) && (t1->kind_ != sltk_int)) {
          dx_error_loc(dx, &x->op_loc_, "Comparison on non-scalar type");
          return r | SLXV_INVALID;
        }
      if (t0->kind_ != t1->kind_) {
        dx_error_loc(dx, &x->op_loc_, "Comparison on incompatible types");
        return r | SLXV_INVALID;
      }
      return r;
    }

    case exop_eq:
    case exop_ne: {
      int r = sl_expr_validate(dx, tb, x->children_[0]);
      r = r | sl_expr_validate(dx, tb, x->children_[1]);
      if (r & SLXV_INVALID) return r;
      struct sl_type *t0 = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
      struct sl_type *t1 = sl_type_unqualified(sl_expr_type(tb, x->children_[1]));
      if (t0 != t1) {
        dx_error_loc(dx, &x->op_loc_, "Equality on incompatible types");
        return r | SLXV_INVALID;
      }
      if (sl_expr_type_contains_array(t0)) {
        dx_error_loc(dx, &x->op_loc_, "Equality on array type is not permitted");
        return r | SLXV_INVALID;
      }
      if (sl_expr_type_contains_sampler(t0)) {
        dx_error_loc(dx, &x->op_loc_, "Equality on sampler type is not permitted");
        return r | SLXV_INVALID;
      }
      return r;
    }

    case exop_function_call: {
      if (x->num_children_ != x->function_->num_parameters_) {
        dx_error_loc(dx, &x->op_loc_, "Function call with wrong number of parameters");
        return SLXV_INVALID;
      }
      size_t n;
      int r = 0;
      if (!x->function_->builtin_eval_fn_) {
        /* No compile-time evaluation implies the function is not const */
        r |= SLXV_NOT_CONSTANT;
      }
      for (n = 0; n < x->num_children_; ++n) {
        r = r | sl_expr_validate(dx, tb, x->children_[n]);
        if (r & SLXV_INVALID) return r;
      }
      /* No further validation if this is a very special case polymorphic debug dump function */
      if (x->function_->is_dump_fn_) return r;

      struct sl_function *f = x->function_;
      for (n = 0; n < x->num_children_; ++n) {
        struct sl_type *t = sl_expr_type(tb, x->children_[n]);
        if (!t) return r | SLXV_INVALID;
        int qualifiers = sl_type_qualifiers(t);
        t = sl_type_unqualified(t);
        struct sl_parameter *p = &f->parameters_[n];
        int param_qualifiers = sl_type_qualifiers(p->type_);
        struct sl_type *pt = sl_type_unqualified(p->type_);
        if (param_qualifiers & (SL_PARAMETER_QUALIFIER_INOUT | SL_PARAMETER_QUALIFIER_OUT)) {
          if (!sl_expr_is_lvalue(tb, x->children_[n])) {
            const char *param_qualifier_str = "";
            if (param_qualifiers & SL_PARAMETER_QUALIFIER_OUT) param_qualifier_str = "out";
            if (param_qualifiers & SL_PARAMETER_QUALIFIER_INOUT) param_qualifier_str = "inout";
            dx_error_loc(dx, &x->children_[n]->op_loc_, "%s parameter requires lvalue", param_qualifier_str);
            return r | SLXV_INVALID;
          }
        }

        if (!pt) return r | SLXV_INVALID;
        if (t != pt) {
          dx_error_loc(dx, &x->op_loc_, "Function call with incompatible parameter type");
          return r | SLXV_INVALID;
        }
      }

      return r;
    }
    case exop_constructor: {
      struct sl_type *t = sl_type_unqualified(x->constructor_type_);
      if (!t) return SLXV_INVALID;
      if (t->kind_ == sltk_struct) {
        size_t num_fields = 0;
        struct sl_type_field *tf = x->constructor_type_->fields_;
        if (tf) {
          do {
            tf = tf->chain_;
            ++num_fields;
          } while (tf != x->constructor_type_->fields_);
        }

        if (x->num_children_ != num_fields) {
          dx_error_loc(dx, &x->op_loc_, "Constructor with wrong number of parameters");
          return SLXV_INVALID;
        }
        size_t n;
        int r = 0;
        for (n = 0; n < x->num_children_; ++n) {
          r = r | sl_expr_validate(dx, tb, x->children_[n]);
          if (r & SLXV_INVALID) return r;
        }

        size_t param_index = 0;
        tf = x->constructor_type_->fields_;
        if (tf) {
          do {
            tf = tf->chain_;

            struct sl_type *t = sl_type_unqualified(sl_expr_type(tb, x->children_[param_index]));
            if (!t) return r | SLXV_INVALID;
            
            struct sl_type *tp = sl_type_unqualified(tf->type_);
            if (t != tp) {
              dx_error_loc(dx, &x->children_[param_index]->op_loc_, "Constructor with incompatible parameter type");
              return r | SLXV_INVALID;
            }

            ++param_index;
          } while (tf != x->constructor_type_->fields_);
        }

        return r;
      } else {
        switch (t->kind_) {
          case sltk_float:
          case sltk_int:
          case sltk_bool:
          case sltk_vec2:
          case sltk_vec3:
          case sltk_vec4:
          case sltk_ivec2:
          case sltk_ivec3:
          case sltk_ivec4:
          case sltk_bvec2:
          case sltk_bvec3:
          case sltk_bvec4:
          case sltk_mat2:
          case sltk_mat3:
          case sltk_mat4: {
            int r = 0;
            r = sl_expr_validate_vec_mat_scalar_constructor(dx, tb, x);
            return r;
          }
          default:
            dx_error_loc(dx, &x->op_loc_, "Constructor with invalid type");
            return SLXV_INVALID;
        }
      }
      break;
    }

    case exop_logical_and:
    case exop_logical_or:
    case exop_logical_xor: {
      int r = sl_expr_validate(dx, tb, x->children_[0]);
      r = r | sl_expr_validate(dx, tb, x->children_[1]);
      if (r & SLXV_INVALID) return r;
      struct sl_type *t = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
      if (t->kind_ != sltk_bool) {
        dx_error_loc(dx, &x->op_loc_, "Logical not on non-boolean type");
        return r | SLXV_INVALID;
      }
      t = sl_type_unqualified(sl_expr_type(tb, x->children_[1]));
      if (t->kind_ != sltk_bool) {
        dx_error_loc(dx, &x->op_loc_, "Logical not on non-boolean type");
        return r | SLXV_INVALID;
      }
      return r;
    }

    case exop_assign: {
      int r = sl_expr_validate(dx, tb, x->children_[0]);
      r |= sl_expr_validate(dx, tb, x->children_[1]);
      if (r & SLXV_INVALID) return r;
      if (!sl_expr_is_lvalue(tb, x->children_[0])) {
        dx_error_loc(dx, &x->op_loc_, "Assignment requires lvalue");
        return r | SLXV_INVALID;
      }
      struct sl_type *lvalue_type = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
      struct sl_type *expr_type = sl_type_unqualified(sl_expr_type(tb, x->children_[1]));
      if (lvalue_type != expr_type) {
        dx_error_loc(dx, &x->op_loc_, "Assignment on incompatible lvalue type");
        return r | SLXV_INVALID;
      }
      return r;
    }

    case exop_mul_assign: {
      int r = sl_expr_validate_mul(dx, tb, x);
      if (r & SLXV_INVALID) return r;
      if (!sl_expr_is_lvalue(tb, x->children_[0])) {
        dx_error_loc(dx, &x->op_loc_, "Multiply-assign on non-lvalue");
        return r | SLXV_INVALID;
      }
      struct sl_type *lvalue_type = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
      struct sl_type *expr_type = sl_type_unqualified(sl_expr_type(tb, x->children_[1]));

      int typematch = 0;
      sl_type_kind_t left_kind = lvalue_type->kind_;
      sl_type_kind_t right_kind = expr_type->kind_;
      if (left_kind == right_kind) {
        typematch = 1;
      }
      else {
        switch (left_kind) {
          case sltk_int:
          case sltk_float:
            /* Types mismatch and the lvalue is smaller than the rvalue in number of components.
             * Cannot op-assign it, e.g.  "f *= v2" makes no sense.  */
            break;
          case sltk_ivec2:
          case sltk_ivec3:
          case sltk_ivec4:
            if (right_kind == sltk_int) typematch = 1;
            break;
          case sltk_vec2:
            /* Note that "v2 *= m2" >does< make sense. */
            if (right_kind == sltk_float) typematch = 1;
            if (right_kind == sltk_mat2) typematch = 1;
            break;
          case sltk_vec3:
            if (right_kind == sltk_float) typematch = 1;
            if (right_kind == sltk_mat3) typematch = 1;
            break;
          case sltk_vec4:
            if (right_kind == sltk_float) typematch = 1;
            if (right_kind == sltk_mat4) typematch = 1;
            break;
          case sltk_mat2:
          case sltk_mat3:
          case sltk_mat4:
            /* Note that the right type can't be a vec because the resulting lvalue
             * would be a vec, and not a mat. */
            if (right_kind == sltk_float) typematch = 1;
            break;
        }
      }

      if (!typematch) {
        dx_error_loc(dx, &x->op_loc_, "Multiply-assign on incompatible lvalue type");
        return r | SLXV_INVALID;
      }
      return r;
    }

    case exop_div_assign:
    case exop_add_assign:
    case exop_sub_assign: {
      int r = sl_expr_validate_div_sub_add(dx, tb, x);
      if (r & SLXV_INVALID) return r;
      if (!sl_expr_is_lvalue(tb, x->children_[0])) {
        dx_error_loc(dx, &x->op_loc_, "Assignment on non-lvalue");
        return r | SLXV_INVALID;
      }
      struct sl_type *lvalue_type = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
      struct sl_type *expr_type = sl_type_unqualified(sl_expr_type(tb, x->children_[1]));
      int typematch = 0;
      sl_type_kind_t left_kind = lvalue_type->kind_;
      sl_type_kind_t right_kind = expr_type->kind_;
      if (left_kind == right_kind) {
        typematch = 1;
      }
      else {
        switch (left_kind) {
          case sltk_int:
          case sltk_float:
            /* Types mismatch and the lvalue is smaller than the rvalue in number of components.
             * Cannot op-assign it, e.g.  "f /= v2" makes no sense.  */
            break;
          case sltk_ivec2:
          case sltk_ivec3:
          case sltk_ivec4:
            if (right_kind == sltk_int) typematch = 1;
            break;
          case sltk_vec2:
          case sltk_vec3:
          case sltk_vec4:
            if (right_kind == sltk_float) typematch = 1;
            break;
          case sltk_mat2:
          case sltk_mat3:
          case sltk_mat4:
            /* Note that the right type can't be a vec because the resulting lvalue
             * would be a vec, and not a mat. */
            if (right_kind == sltk_float) typematch = 1;
            break;
        }
      }
      if (!typematch) {
        dx_error_loc(dx, &x->op_loc_, "Assignment on incompatible lvalue type");
        return r | SLXV_INVALID;
      }
      return r;
    }

    case exop_sequence:
      return sl_expr_validate(dx, tb, x->children_[0]) | sl_expr_validate(dx, tb, x->children_[1]);

    case exop_conditional: {
      int r = sl_expr_validate(dx, tb, x->children_[0]);
      r = r | sl_expr_validate(dx, tb, x->children_[1]);
      r = r | sl_expr_validate(dx, tb, x->children_[2]);
      struct sl_type *ct = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
      if (ct->kind_ != sltk_bool) {
        dx_error_loc(dx, &x->op_loc_, "Conditional expression with non-boolean condition");
        return r | SLXV_INVALID;
      }
      struct sl_type *t1 = sl_type_unqualified(sl_expr_type(tb, x->children_[1]));
      struct sl_type *t2 = sl_type_unqualified(sl_expr_type(tb, x->children_[2]));
      if (t1 != t2) {
        dx_error_loc(dx, &x->op_loc_, "Conditional expression with incompatible types");
        return r | SLXV_INVALID;
      }
      if (t1->kind_ == sltk_array) {
        dx_error_loc(dx, &x->op_loc_, "Conditional expression with array type is not permitted");
        return r | SLXV_INVALID;
      }
      return r;
    }
    default:
      return SLXV_INVALID;
  }
}

int sl_expr_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r) {
  size_t n;
  switch (x->op_) {
    case exop_invalid:
      return -1;
    case exop_variable:
      return sl_expr_temp_copy(r, &x->variable_->value_);
    case exop_literal:
      return sl_expr_temp_copy(r, &x->literal_value_);
    case exop_array_subscript: {
      struct sl_expr_temp array_value;
      struct sl_expr_temp index_value;
      sl_expr_temp_init(&array_value, NULL);
      sl_expr_temp_init(&index_value, NULL);
      if (sl_expr_eval(tb, x->children_[0], &array_value)) {
        sl_expr_temp_cleanup(&array_value);
        sl_expr_temp_cleanup(&index_value);
        return -1;
      }
      if (sl_expr_eval(tb, x->children_[1], &index_value)) {
        sl_expr_temp_cleanup(&array_value);
        sl_expr_temp_cleanup(&index_value);
        return -1;
      }
      if (sl_expr_temp_copy(r, array_value.v_.comp_.elements_ + index_value.v_.i_)) {
        sl_expr_temp_cleanup(&array_value);
        sl_expr_temp_cleanup(&index_value);
        return -1;
      }
      sl_expr_temp_cleanup(&array_value);
      sl_expr_temp_cleanup(&index_value);
      return 0;
    }
    case exop_component_selection: {
      struct sl_type *vec_type = sl_expr_type(tb, x->children_[0]);
      vec_type = sl_type_unqualified(vec_type);
      if (!vec_type) return -1;
      struct sl_expr_temp result_value;
      sl_expr_temp_init(&result_value, NULL);
      struct sl_expr_temp vec_value;
      sl_expr_temp_init(&vec_value, NULL);
      sl_expr_temp_kind_t scalar_kind = sletk_void;
      switch (vec_type->kind_) {
        case sltk_vec2:
        case sltk_vec3:
        case sltk_vec4:
          scalar_kind = sletk_float;
          switch (x->num_components_) {
            case 1:
              result_value.kind_ = sletk_float;
              break;
            case 2:
              result_value.kind_ = sletk_vec2;
              break;
            case 3:
              result_value.kind_ = sletk_vec3;
              break;
            case 4:
              result_value.kind_ = sletk_vec4;
              break;
            default:
              assert(0 && "Invalid number of components");
              break;
          }
          break;
        case sltk_bvec2:
        case sltk_bvec3:
        case sltk_bvec4:
          scalar_kind = sletk_bool;
          switch (x->num_components_) {
            case 1:
              result_value.kind_ = sletk_bool;
              break;
            case 2:
              result_value.kind_ = sletk_bvec2;
              break;
            case 3:
              result_value.kind_ = sletk_bvec3;
              break;
            case 4:
              result_value.kind_ = sletk_bvec4;
              break;
            default:
              assert(0 && "Invalid number of components");
              break;
          }
          break;
        case sltk_ivec2:
        case sltk_ivec3:
        case sltk_ivec4:
          scalar_kind = sletk_int;
          switch (x->num_components_) {
            case 1:
              result_value.kind_ = sletk_int;
              break;
            case 2:
              result_value.kind_ = sletk_ivec2;
              break;
            case 3:
              result_value.kind_ = sletk_ivec3;
              break;
            case 4:
              result_value.kind_ = sletk_ivec4;
              break;
            default:
              assert(0 && "Invalid number of components");
              break;
          }
          break;
        default:
          sl_expr_temp_cleanup(&vec_value);
          return -1;
      }
      
      switch (scalar_kind) {
        case sletk_float: {
          float *pf = NULL;
          if (x->num_components_ == 1) {
            pf = &result_value.v_.f_;
          }
          else {
            pf = result_value.v_.v_;
          }

          for (n = 0; n < x->num_components_; ++n) {
            pf[n] = vec_value.v_.v_[x->swizzle_[n].parameter_index_];
          }
          break;
        }

        case sletk_int: {
          int64_t *pi = NULL;
          if (x->num_components_ == 1) {
            pi = &result_value.v_.i_;
          }
          else {
            pi = result_value.v_.iv_;
          }

          for (n = 0; n < x->num_components_; ++n) {
            pi[n] = vec_value.v_.iv_[x->swizzle_[n].parameter_index_];
          }

          break;
        }

        case sletk_bool: {
          int *pb = NULL;
          if (x->num_components_ == 1) {
            pb = &result_value.v_.b_;
          }
          else {
            pb = result_value.v_.bv_;
          }

          for (n = 0; n < x->num_components_; ++n) {
            pb[n] = vec_value.v_.bv_[x->swizzle_[n].parameter_index_];
          }
          break;
        }
      }

      if (sl_expr_temp_copy(r, &result_value)) {
        sl_expr_temp_cleanup(&result_value);
        sl_expr_temp_cleanup(&vec_value);
        return -1;
      }
      sl_expr_temp_cleanup(&result_value);
      sl_expr_temp_cleanup(&vec_value);
      return 0;
    }
    case exop_field_selection: {
      struct sl_expr_temp struct_value;
      sl_expr_temp_init(&struct_value, NULL);
      if (sl_expr_eval(tb, x->children_[0], &struct_value)) {
        sl_expr_temp_cleanup(&struct_value);
        return -1;
      }

      struct sl_type *struct_type = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));

      if (struct_type->kind_ != sltk_struct) {
        sl_expr_temp_cleanup(&struct_value);
        return -1;
      }

      size_t field_index = 0;
      struct sl_type_field *f = struct_type->fields_;
      if (f) {
        for (;;) {
          f = f->chain_;

          if (f == x->field_selection_) {
            break;
          }

          field_index++;
          if (f == struct_type->fields_) {
            f = NULL;
            break;
          }
        }
      }
      if (!f) {
        /* Field not found, should not happen */
        sl_expr_temp_cleanup(&struct_value);
        return -1;
      }

      if (sl_expr_temp_copy(r, struct_value.v_.comp_.elements_ + field_index)) {
        sl_expr_temp_cleanup(&struct_value);
        return -1;
      }
      sl_expr_temp_cleanup(&struct_value);
      return 0;
    }
    case exop_post_inc:
    case exop_post_dec:
    case exop_pre_inc:
    case exop_pre_dec:
      return -1; /* not constant expression */

    case exop_negate: {
      struct sl_expr_temp opd;
      sl_expr_temp_init(&opd, NULL);
      if (sl_expr_eval(tb, x->children_[0], &opd)) {
        sl_expr_temp_cleanup(&opd);
        return -1;
      }
      if (sl_expr_temp_negate(r, &opd)) {
        sl_expr_temp_cleanup(&opd);
        return -1;
      }
      sl_expr_temp_cleanup(&opd);
      return 0;
    }
    case exop_logical_not: {
      struct sl_expr_temp opd;
      sl_expr_temp_init(&opd, NULL);
      if (sl_expr_eval(tb, x->children_[0], &opd)) {
        sl_expr_temp_cleanup(&opd);
        return -1;
      }
      if (sl_expr_temp_logical_not(r, &opd)) {
        sl_expr_temp_cleanup(&opd);
        return -1;
      }
      sl_expr_temp_cleanup(&opd);
      return 0;
    }

    case exop_multiply:
    case exop_divide:
    case exop_add:
    case exop_subtract:
    case exop_lt:
    case exop_le:
    case exop_ge:
    case exop_gt:
    case exop_eq:
    case exop_ne:
    case exop_logical_and:
    case exop_logical_or:
    case exop_logical_xor:
    case exop_sequence: {
      /* binary op */
      struct sl_expr_temp lopd, ropd;
      sl_expr_temp_init(&lopd, NULL);
      sl_expr_temp_init(&ropd, NULL);
      if (sl_expr_eval(tb, x->children_[0], &lopd)) {
        sl_expr_temp_cleanup(&lopd);
        sl_expr_temp_cleanup(&ropd);
        return -1;
      }
      if (sl_expr_eval(tb, x->children_[1], &ropd)) {
        sl_expr_temp_cleanup(&lopd);
        sl_expr_temp_cleanup(&ropd);
        return -1;
      }
      int res = 0;
      switch (x->op_) {
        case exop_multiply:
          res = sl_expr_temp_mul(r, &lopd, &ropd);
          break;
        case exop_divide:
          res = sl_expr_temp_div(r, &lopd, &ropd);
          break;
        case exop_add:
          res = sl_expr_temp_add(r, &lopd, &ropd);
          break;
        case exop_subtract:
          res = sl_expr_temp_sub(r, &lopd, &ropd);
          break;
        case exop_lt:
          res = sl_expr_temp_lt(r, &lopd, &ropd);
          break;
        case exop_le:
          res = sl_expr_temp_le(r, &lopd, &ropd);
          break;
        case exop_ge:
          res = sl_expr_temp_ge(r, &lopd, &ropd);
          break;
        case exop_gt:
          res = sl_expr_temp_gt(r, &lopd, &ropd);
          break;
        case exop_eq:
          res = sl_expr_temp_eq(r, &lopd, &ropd);
          break;
        case exop_ne:
          res = sl_expr_temp_ne(r, &lopd, &ropd);
          break;
        case exop_logical_and:
          res = sl_expr_temp_logical_and(r, &lopd, &ropd);
          break;
        case exop_logical_or:
          res = sl_expr_temp_logical_or(r, &lopd, &ropd);
          break;
        case exop_logical_xor:
          res = sl_expr_temp_logical_xor(r, &lopd, &ropd);
          break;
        case exop_sequence:
          res = 0;
          sl_expr_temp_copy(r, &ropd);
          break;
      }
      sl_expr_temp_cleanup(&lopd);
      sl_expr_temp_cleanup(&ropd);
      return res;
    }

    case exop_function_call: {
      /* Fish out the builtin functions and call those, return -1 for everything else. */
      if (x->function_ && x->function_->builtin_eval_fn_) {
        x->function_->builtin_eval_fn_(tb, x, r);
        return 0;
      }
      else {
        return -1;
      }
      break;
    }

    case exop_constructor: {
      struct sl_expr_temp tmp;
      if (sl_expr_temp_init(&tmp, x->constructor_type_)) {
        return -1;
      }


      struct sl_expr_temp *children = NULL;
      if (x->num_children_) {
        children = (struct sl_expr_temp *)malloc(sizeof(struct sl_expr_temp) * x->num_children_);
        if (!children) {
          sl_expr_temp_cleanup(&tmp);
          return -1;
        }
        for (n = 0; n < x->num_children_; ++n) {
          sl_expr_temp_init(children + n, NULL);
        }
      }

      for (n = 0; n < x->num_children_; ++n) {
        if (sl_expr_eval(tb, x->children_[n], children + n)) {
          sl_expr_temp_cleanup(&tmp);
          while (n--) {
            sl_expr_temp_cleanup(children + n);
          }
          if (children) free(children);
          return -1;
        }
      }

      float *f = NULL;
      int64_t *i = NULL;
      int *b = NULL;
      int done = 0;

      switch (tmp.kind_) {
        case sletk_void:
          sl_expr_temp_cleanup(&tmp);
          if (children) free(children);
          return 0; /* easy. */
        case sletk_array:
        case sletk_struct: {
          if (tmp.v_.comp_.num_elements_ != x->num_children_) {
            for (n = 0; n < x->num_children_; ++n) {
              sl_expr_temp_cleanup(children + n);
            }
            if (children) free(children);
            sl_expr_temp_cleanup(&tmp);
            return -1;
          }
          for (n = 0; n < x->num_children_; ++n) {
            if (sl_expr_temp_copy(tmp.v_.comp_.elements_ + n, children + n)) {
              for (n = 0; n < x->num_children_; ++n) {
                sl_expr_temp_cleanup(children + n);
              }
              if (children) free(children);
              sl_expr_temp_cleanup(&tmp);
              return -1;
            }
          }
          done = 1;
          break;
        }
        case sletk_float:
          f = &tmp.v_.f_;
          break;
        case sletk_int:
          i = &tmp.v_.i_;
          break;
        case sletk_bool:
          b = &tmp.v_.b_;
          break;
        case sletk_vec2:
        case sletk_vec3:
        case sletk_vec4:
          f = tmp.v_.v_;
          break;
        case sletk_bvec2:
        case sletk_bvec3:
        case sletk_bvec4:
          b = tmp.v_.bv_;
          break;
        case sletk_ivec2:
        case sletk_ivec3:
        case sletk_ivec4:
          i = tmp.v_.iv_;
          break;
        case sletk_mat2:
        case sletk_mat3:
        case sletk_mat4:
          f = tmp.v_.m_;
          break;
        default:
          assert(!"Unhandled constructor type");
          break;
      }

      int failed = 0;

      if (!done) {
        /* Did not complete yet (this was not a struct/array but is a scalar/vector/matrix) */
        for (n = 0; n < x->num_components_; ++n) {
          const struct sl_component_selection *cs = x->swizzle_ + n;
          switch (cs->conversion_) {
            case slcc_float_to_float:
              switch (children[cs->parameter_index_].kind_) {
                case sletk_float:
                  f[n] = children[cs->parameter_index_].v_.f_;
                  break;
                case sletk_vec2:
                case sletk_vec3:
                case sletk_vec4:
                  f[n] = children[cs->parameter_index_].v_.v_[cs->component_index_];
                  break;
                case sletk_mat2:
                case sletk_mat3:
                case sletk_mat4:
                  f[n] = children[cs->parameter_index_].v_.m_[cs->component_index_];
                  break;
                default:
                  /* conversion mismatches type found */
                  failed = 1;
                  break;
              }
              break;
              case slcc_float_to_int:
                switch (children[cs->parameter_index_].kind_) {
                  case sletk_float:
                    i[n] = (int64_t)children[cs->parameter_index_].v_.f_;
                    break;
                  case sletk_vec2:
                  case sletk_vec3:
                  case sletk_vec4:
                    i[n] = (int64_t)children[cs->parameter_index_].v_.v_[cs->component_index_];
                    break;
                  case sletk_mat2:
                  case sletk_mat3:
                  case sletk_mat4:
                    i[n] = (int64_t)children[cs->parameter_index_].v_.m_[cs->component_index_];
                    break;
                  default:
                    /* conversion mismatches type found */
                    failed = 1;
                    break;
                }
                break;
              case slcc_float_to_bool:
                switch (children[cs->parameter_index_].kind_) {
                  case sletk_float:
                    b[n] = !!children[cs->parameter_index_].v_.f_;
                    break;
                  case sletk_vec2:
                  case sletk_vec3:
                  case sletk_vec4:
                    b[n] = !!children[cs->parameter_index_].v_.v_[cs->component_index_];
                    break;
                  case sletk_mat2:
                  case sletk_mat3:
                  case sletk_mat4:
                    b[n] = !!children[cs->parameter_index_].v_.m_[cs->component_index_];
                    break;
                  default:
                    /* conversion mismatches type found */
                    failed = 1;
                    break;
                }
                break;
              case slcc_int_to_float:
                switch (children[cs->parameter_index_].kind_) {
                  case sletk_int:
                    f[n] = (float)children[cs->parameter_index_].v_.i_;
                    break;
                  case sletk_ivec2:
                  case sletk_ivec3:
                  case sletk_ivec4:
                    f[n] = (float)children[cs->parameter_index_].v_.iv_[cs->component_index_];
                    break;
                  default:
                    /* conversion mismatches type found */
                    failed = 1;
                    break;
                }
                break;
              case slcc_int_to_int:
                switch (children[cs->parameter_index_].kind_) {
                  case sletk_int:
                    i[n] = children[cs->parameter_index_].v_.i_;
                    break;
                  case sletk_ivec2:
                  case sletk_ivec3:
                  case sletk_ivec4:
                    i[n] = children[cs->parameter_index_].v_.iv_[cs->component_index_];
                    break;
                  default:
                    /* conversion mismatches type found */
                    failed = 1;
                    break;
                }
                break;
              case slcc_int_to_bool:
                switch (children[cs->parameter_index_].kind_) {
                  case sletk_int:
                    b[n] = !!children[cs->parameter_index_].v_.i_;
                    break;
                  case sletk_ivec2:
                  case sletk_ivec3:
                  case sletk_ivec4:
                    b[n] = !!children[cs->parameter_index_].v_.iv_[cs->component_index_];
                    break;
                  default:
                    /* conversion mismatches type found */
                    failed = 1;
                    break;
                }
                break;
              case slcc_bool_to_float:
                switch (children[cs->parameter_index_].kind_) {
                  case sletk_bool:
                    f[n] = (float)!!children[cs->parameter_index_].v_.b_;
                    break;
                  case sletk_bvec2:
                  case sletk_bvec3:
                  case sletk_bvec4:
                    f[n] = (float)!!children[cs->parameter_index_].v_.bv_[cs->component_index_];
                    break;
                  default:
                    /* conversion mismatches type found */
                    failed = 1;
                    break;
                }
                break;
              case slcc_bool_to_int:
                switch (children[cs->parameter_index_].kind_) {
                  case sletk_bool:
                    i[n] = (int64_t)!!children[cs->parameter_index_].v_.b_;
                    break;
                  case sletk_bvec2:
                  case sletk_bvec3:
                  case sletk_bvec4:
                    i[n] = (int64_t)!!children[cs->parameter_index_].v_.bv_[cs->component_index_];
                    break;
                  default:
                    /* conversion mismatches type found */
                    failed = 1;
                    break;
                }
                break;
              case slcc_bool_to_bool:
                switch (children[cs->parameter_index_].kind_) {
                  case sletk_bool:
                    i[n] = children[cs->parameter_index_].v_.b_;
                    break;
                  case sletk_bvec2:
                  case sletk_bvec3:
                  case sletk_bvec4:
                    i[n] = children[cs->parameter_index_].v_.bv_[cs->component_index_];
                    break;
                  default:
                    /* conversion mismatches type found */
                    failed = 1;
                    break;
                }
                break;
              default:
                failed = 1;
              break;
          }
          if (failed) break;
        }
      }
      if (!failed) {
        failed = sl_expr_temp_copy(r, &tmp);
      }
      for (n = 0; n < x->num_children_; ++n) {
        sl_expr_temp_cleanup(children + n);
      }
      if (children) free(children);
      sl_expr_temp_cleanup(&tmp);

      return failed ? -1 : 0;
    }

    case exop_assign:
    case exop_mul_assign:
    case exop_div_assign:
    case exop_add_assign:
    case exop_sub_assign:
      /* Not constant expression */
      return -1;

    case exop_conditional: {
      struct sl_expr_temp condition_opd, ropd;
      sl_expr_temp_init(&condition_opd, NULL);
      sl_expr_temp_init(&ropd, NULL);
      if (sl_expr_eval(tb, x->children_[0], &condition_opd)) {
        sl_expr_temp_cleanup(&condition_opd);
        return -1;
      }
      if (condition_opd.kind_ != sletk_bool) {
        sl_expr_temp_cleanup(&condition_opd);
        return -1;
      }
      struct sl_expr *branch_taken = NULL;
      if (condition_opd.v_.b_) {
        /* Execute true branch */
        branch_taken = x->children_[1];
      }
      else {
        branch_taken = x->children_[2];
      }
      if (sl_expr_eval(tb, branch_taken, r)) {
        sl_expr_temp_cleanup(&condition_opd);
        sl_expr_temp_cleanup(&ropd);
        return -1;
      }
      sl_expr_temp_cleanup(&condition_opd);
      sl_expr_temp_cleanup(&ropd);

      return 0;
    }

    default:
      assert(!"Unhandled expression type");
      return -1;
  }
}

struct sl_type *sl_expr_type(struct sl_type_base *tb, struct sl_expr *x) {
  switch (x->op_) {
    case exop_invalid:
      return NULL;
    case exop_variable:
      return x->variable_->type_;
    case exop_literal:
      return sl_expr_temp_type(tb, &x->literal_value_);
        
    case exop_array_subscript: {
      struct sl_type *t = sl_expr_type(tb, x->children_[0]);
      if (!t) return NULL;
      t = sl_type_unqualified(t);
      if (t->kind_ == sltk_array) return t->derived_type_;
      switch (t->kind_) {
        case sltk_array: return t->derived_type_;
        case sltk_vec2:
        case sltk_vec3:
        case sltk_vec4: return &tb->float_;
        case sltk_mat2: return &tb->vec2_;
        case sltk_mat3: return &tb->vec3_;
        case sltk_mat4: return &tb->vec4_;
        case sltk_ivec2:
        case sltk_ivec3:
        case sltk_ivec4: return &tb->int_;
        case sltk_bvec2:
        case sltk_bvec3:
        case sltk_bvec4: return &tb->bool_;
      }
      return NULL; /* should not have passed validation */
    }
    case exop_component_selection: /* e.g. myvec3.xxz */ {
      struct sl_type *t = sl_expr_type(tb, x->children_[0]);
      if (!t) return NULL;
      t = sl_type_unqualified(t);
      switch (t->kind_) {
        case sltk_vec2:
        case sltk_vec3:
        case sltk_vec4:
          switch (x->num_components_) {
            case 1: return &tb->float_;
            case 2: return &tb->vec2_;
            case 3: return &tb->vec3_;
            case 4: return &tb->vec4_;
          }
          return NULL;
        case sltk_bvec2:
        case sltk_bvec3:
        case sltk_bvec4:
          switch (x->num_components_) {
            case 1: return &tb->bool_;
            case 2: return &tb->bvec2_;
            case 3: return &tb->bvec3_;
            case 4: return &tb->bvec4_;
          }
          return NULL;

        case sltk_ivec2:
        case sltk_ivec3:
        case sltk_ivec4:
          switch (x->num_components_) {
            case 1: return &tb->int_;
            case 2: return &tb->ivec2_;
            case 3: return &tb->ivec3_;
            case 4: return &tb->ivec4_;
          }
          return NULL;
        default:
          return NULL;
      }

      break;
    }
    case exop_field_selection:     /* e.g. mystruct.myfield */
      return x->field_selection_->type_;

    case exop_post_inc:
    case exop_post_dec:
    case exop_pre_inc:
    case exop_pre_dec:
    case exop_negate:
      return sl_expr_type(tb, x->children_[0]);

    case exop_logical_not:
      return &tb->bool_;

    case exop_multiply: {
      struct sl_type *ltype = sl_expr_type(tb, x->children_[0]);
      struct sl_type *rtype = sl_expr_type(tb, x->children_[1]);
      ltype = sl_type_unqualified(ltype);
      rtype = sl_type_unqualified(rtype);
      sl_type_kind_t lkind = ltype->kind_;
      sl_type_kind_t rkind = rtype->kind_;
      sl_type_kind_t lesser_kind = lkind < rkind ? lkind : rkind;
      sl_type_kind_t greater_kind = lkind < rkind ? rkind : lkind;
      switch (lesser_kind) {
        case sltk_float:
          switch (greater_kind) {
            case sltk_float: return &tb->float_;
            case sltk_vec2: return &tb->vec2_;
            case sltk_vec3: return &tb->vec3_;
            case sltk_vec4: return &tb->vec4_;
            case sltk_mat2: return &tb->mat2_;
            case sltk_mat3: return &tb->mat3_;
            case sltk_mat4: return &tb->mat4_;
            default: return NULL;
          }
        case sltk_vec2:
          switch (greater_kind) {
            case sltk_vec2: return &tb->vec2_;
            case sltk_mat2: return &tb->vec2_;
            default: return NULL;
          }
        case sltk_vec3:
          switch (greater_kind) {
            case sltk_vec3: return &tb->vec3_;
            case sltk_mat3: return &tb->vec3_;
            default: return NULL;
          }
        case sltk_vec4:
          switch (greater_kind) {
            case sltk_vec4: return &tb->vec4_;
            case sltk_mat4: return &tb->vec4_;
            default: return NULL;
          }
        case sltk_mat2:
          if (greater_kind != sltk_mat2) return NULL;
          return &tb->mat2_;
        case sltk_mat3:
          if (greater_kind != sltk_mat3) return NULL;
          return &tb->mat3_;
        case sltk_mat4:
          if (greater_kind != sltk_mat4) return NULL;
          return &tb->mat4_;

        case sltk_int:
          switch (greater_kind) {
            case sltk_int: return &tb->int_;
            case sltk_ivec2: return &tb->ivec2_;
            case sltk_ivec3: return &tb->ivec3_;
            case sltk_ivec4: return &tb->ivec4_;
            default: return NULL;
          }
        case sltk_ivec2:
          if (greater_kind != sltk_ivec2) return NULL;
          return &tb->ivec2_;
        case sltk_ivec3:
          if (greater_kind != sltk_ivec3) return NULL;
          return &tb->ivec3_;
        case sltk_ivec4:
          if (greater_kind != sltk_ivec4) return NULL;
          return &tb->ivec4_;

        case sltk_bool:
          switch (greater_kind) {
            case sltk_bool: return &tb->bool_;
            case sltk_bvec2: return &tb->bvec2_;
            case sltk_bvec3: return &tb->bvec3_;
            case sltk_bvec4: return &tb->bvec4_;
            default: return NULL;
          }
        case sltk_bvec2:
          if (greater_kind != sltk_bvec2) return NULL;
          return &tb->bvec2_;
        case sltk_bvec3:
          if (greater_kind != sltk_bvec3) return NULL;
          return &tb->bvec3_;
        case sltk_bvec4:
          if (greater_kind != sltk_bvec4) return NULL;
          return &tb->bvec4_;

        default:
          return NULL;
      }
      break;
    }
    case exop_divide:
    case exop_add:
    case exop_subtract: {
      struct sl_type *ltype = sl_expr_type(tb, x->children_[0]);
      struct sl_type *rtype = sl_expr_type(tb, x->children_[1]);
      ltype = sl_type_unqualified(ltype);
      rtype = sl_type_unqualified(rtype);
      sl_type_kind_t lkind = ltype->kind_;
      sl_type_kind_t rkind = rtype->kind_;
      sl_type_kind_t lesser_kind = lkind < rkind ? lkind : rkind;
      sl_type_kind_t greater_kind = lkind < rkind ? rkind : lkind;

      switch (greater_kind) {
        case sltk_float:
          if (lesser_kind != sltk_float) return NULL;
          return &tb->float_;
        case sltk_int:
          if (lesser_kind != sltk_int) return NULL;
          return &tb->int_;
        case sltk_bool:
          if (lesser_kind != sltk_bool) return NULL;
          return &tb->bool_;
        case sltk_vec2:
          switch (lesser_kind) {
            case sltk_vec2: return &tb->vec2_;
            case sltk_float: return &tb->vec2_;
            default: return NULL;
          }
        case sltk_vec3:
          switch (lesser_kind) {
            case sltk_vec3: return &tb->vec3_;
            case sltk_float: return &tb->vec3_;
            default: return NULL;
          }
        case sltk_vec4:
          switch (lesser_kind) {
            case sltk_vec4: return &tb->vec4_;
            case sltk_float: return &tb->vec4_;
            default: return NULL;
          }
        case sltk_bvec2:
          switch (lesser_kind) {
            case sltk_bvec2: return &tb->bvec2_;
            case sltk_bool: return &tb->bvec2_;
            default: return NULL;
          }
        case sltk_bvec3:
          switch (lesser_kind) {
            case sltk_bvec3: return &tb->bvec3_;
            case sltk_bool: return &tb->bvec3_;
            default: return NULL;
          }
        case sltk_bvec4:
          switch (lesser_kind) {
            case sltk_bvec4: return &tb->bvec4_;
            case sltk_bool: return &tb->bvec4_;
            default: return NULL;
          }
        case sltk_ivec2:
          switch (lesser_kind) {
            case sltk_ivec2: return &tb->ivec2_;
            case sltk_int: return &tb->ivec2_;
            default: return NULL;
          }
        case sltk_ivec3:
          switch (lesser_kind) {
            case sltk_ivec3: return &tb->ivec3_;
            case sltk_int: return &tb->ivec3_;
            default: return NULL;
          }
        case sltk_ivec4:
          switch (lesser_kind) {
            case sltk_ivec4: return &tb->ivec4_;
            case sltk_int: return &tb->ivec4_;
            default: return NULL;
          }
        case sltk_mat2:
          switch (lesser_kind) {
            case sltk_mat2: return &tb->mat2_;
            case sltk_float: return &tb->mat2_;
            default: return NULL;
          }
        case sltk_mat3:
          switch (lesser_kind) {
            case sltk_mat3: return &tb->mat3_;
            case sltk_float: return &tb->mat3_;
            default: return NULL;
          }
        case sltk_mat4:
          switch (lesser_kind) {
            case sltk_mat4: return &tb->mat4_;
            case sltk_float: return &tb->mat4_;
            default: return NULL;
          }
        default:
              return NULL;
      }
      break;
    }
    case exop_lt:
    case exop_le:
    case exop_ge:
    case exop_gt:
    case exop_eq:
    case exop_ne:
      return &tb->bool_;

    case exop_function_call:
      return x->function_->return_type_;

    case exop_constructor:
      return x->constructor_type_;

    case exop_logical_and:
    case exop_logical_or:
    case exop_logical_xor:
      return &tb->bool_;

    case exop_assign:
    case exop_mul_assign:
    case exop_div_assign:
    case exop_add_assign:
    case exop_sub_assign:
      /* Type of assingment left=right is the type of the left value */
      return sl_expr_type(tb, x->children_[0]);

    case exop_sequence:    
      return sl_expr_type(tb, x->children_[1]);

    case exop_conditional:
      /* ternary ?: operator
        * second or third child operand's type is fine, they must be the same */
      return sl_expr_type(tb, x->children_[1]);
    default:
      assert(0 && "Unhandled expression operator");
      return NULL;
  }
}

struct sl_expr *sl_expr_alloc(expr_op_t op, const struct situs *loc) {
  struct sl_expr *x = (struct sl_expr *)malloc(sizeof(struct sl_expr));
  if (!x) return NULL;
  sl_expr_init(x);
  x->op_ = op;
  if (situs_clone(&x->op_loc_, loc)) {
    /* Failed memory alloc */
    sl_expr_free(x);
    return NULL;
  }
  return x;
}

struct sl_expr *sl_expr_alloc_float_lit(float f, const struct situs *loc) {
  struct sl_expr *x = sl_expr_alloc(exop_literal, loc);
  if (!x) return NULL;
  sl_expr_temp_init_float(&x->literal_value_, f);
  return x;
}

struct sl_expr *sl_expr_alloc_int_lit(int64_t i, const struct situs *loc) {
  struct sl_expr *x = sl_expr_alloc(exop_literal, loc);
  if (!x) return NULL;
  sl_expr_temp_init_int(&x->literal_value_, i);
  return x;
}

struct sl_expr *sl_expr_alloc_bool_lit(int b, const struct situs *loc) {
  struct sl_expr *x = sl_expr_alloc(exop_literal, loc);
  if (!x) return NULL;
  sl_expr_temp_init_bool(&x->literal_value_, b);
  return x;
}

struct sl_expr *sl_expr_alloc_function_call(struct sl_function *f, const struct situs *loc, struct sl_expr **pexpr, size_t pexpr_stride) {
  struct sl_expr *x = sl_expr_alloc(exop_function_call, loc);
  if (!x) return NULL;
  x->num_children_ = f->num_parameters_;
  x->children_ = (struct sl_expr **)malloc(sizeof(struct sl_expr *) * x->num_children_);
  if (!x->children_) {
    sl_expr_free(x);
    return NULL;
  }
  size_t n;
  for (n = 0; n < x->num_children_; ++n) {
    x->children_[n] = *pexpr; 
    pexpr = (struct sl_expr **)(((char *)pexpr) + pexpr_stride);
  }
  sl_expr_attach_caller(x, f); /* this sets function_ */
  return x;
}

struct sl_expr *sl_expr_alloc_constructor(struct sl_type *t, const struct situs *loc, size_t num_params, struct sl_expr **pexpr, size_t pexpr_stride) {
  struct sl_expr *x = sl_expr_alloc(exop_constructor, loc);
  if (!x) return NULL;
  x->constructor_type_ = t;
  x->num_children_ = num_params;
  x->children_ = (struct sl_expr **)malloc(sizeof(struct sl_expr *) * x->num_children_);
  if (!x->children_) {
    sl_expr_free(x);
    return NULL;
  }
  size_t n;
  for (n = 0; n < x->num_children_; ++n) {
    x->children_[n] = *pexpr;
    pexpr = (struct sl_expr **)(((char *)pexpr) + pexpr_stride);
  }
  return x;
}

struct sl_expr *sl_expr_alloc_variable(struct sl_variable *v, const struct situs *loc) {
  struct sl_expr *x = sl_expr_alloc(exop_variable, loc);
  if (!x) return NULL;
  x->variable_ = v;
  return x;
}

struct sl_expr *sl_expr_alloc_swizzle_selection(struct sl_expr *vec_expr, size_t num_tgt_components, uint8_t *component_selection, const struct situs *field_loc) {
  struct sl_expr *x = sl_expr_alloc(exop_component_selection, field_loc);
  if (!x) return NULL;
  x->num_children_ = 1;
  x->children_ = (struct sl_expr **)malloc(sizeof(struct sl_expr *));
  if (!x->children_) {
    sl_expr_free(x);
    return NULL;
  }
  x->children_[0] = vec_expr;
  x->num_components_ = (int)num_tgt_components;
  memcpy(x->component_selection_, component_selection, num_tgt_components);
  return x;
}

struct sl_expr *sl_expr_alloc_field_selection(struct sl_expr *expr, struct sl_type_field *field, size_t field_index, const struct situs *field_loc) {
  struct sl_expr *x = sl_expr_alloc(exop_field_selection, field_loc);
  if (!x) return NULL;
  x->num_children_ = 1;
  x->children_ = (struct sl_expr **)malloc(sizeof(struct sl_expr *));
  if (!x->children_) {
    sl_expr_free(x);
    return NULL;
  }
  x->children_[0] = expr;
  x->field_selection_ = field;
  x->field_index_ = field_index;
  return x;
}

struct sl_expr *sl_expr_alloc_unop(expr_op_t op,const struct situs *loc, struct sl_expr **opd) {
  struct sl_expr *x = sl_expr_alloc(op,loc);
  if(!x) return NULL;
  x->num_children_ = 1;
  x->children_ = (struct sl_expr **)malloc(sizeof(struct sl_expr *) * x->num_children_);
  if (!x->children_) {
    sl_expr_free(x);
    return NULL;
  }
  x->children_[0] = *opd; *opd = NULL;

  return x;
}

struct sl_expr *sl_expr_alloc_binop(expr_op_t op, const struct situs *loc, struct sl_expr **lhs, struct sl_expr **rhs) {
  struct sl_expr *x = sl_expr_alloc(op, loc);
  if (!x) return NULL;
  x->num_children_ = 2;
  x->children_ = (struct sl_expr **)malloc(sizeof(struct sl_expr *) * x->num_children_);
  if (!x->children_) {
    sl_expr_free(x);
    return NULL;
  }
  x->children_[0] = *lhs; *lhs = NULL;
  x->children_[1] = *rhs; *rhs = NULL;

  return x;
}

struct sl_expr *sl_expr_alloc_ternop(expr_op_t op,const struct situs *loc,struct sl_expr **opd0, struct sl_expr **opd1, struct sl_expr **opd2) {
  struct sl_expr *x = sl_expr_alloc(op, loc);
  if (!x) return NULL;
  x->num_children_ = 3;
  x->children_ = (struct sl_expr **)malloc(sizeof(struct sl_expr *) * x->num_children_);
  if (!x->children_) {
    sl_expr_free(x);
    return NULL;
  }
  x->children_[0] = *opd0; *opd0 = NULL;
  x->children_[1] = *opd1; *opd1 = NULL;
  x->children_[2] = *opd2; *opd2 = NULL;

  return x;
}

static int sl_expr_is_assignment(struct sl_expr *x) {
  static const int is_assign[] = {
    [exop_assign] = 1,
    [exop_mul_assign] = 1,
    [exop_div_assign] = 1,
    [exop_add_assign] = 1,
    [exop_sub_assign] = 1
  };
  if (x->op_ >= (sizeof(is_assign)/sizeof(*is_assign))) return 0;
  return is_assign[x->op_];
}

static int sl_expr_need_rvalue(struct sl_type_base *tb, struct sl_reg_allocator *ract, struct sl_expr *x) {
  if (x->offset_reg_.kind_ == slrak_void) {
    /* No need for separate rvalue as the registers for x can be used directly,
     * (are not at an offset and so don't need a separate load.) */
    sl_reg_alloc_void(&x->rvalue_);
    return 0;
  }
  struct sl_type *t = sl_expr_type(tb, x);
  if (!t) return -1;

  int r;
  r = sl_reg_alloc_set_type(&x->rvalue_, t, ract->local_frame_);
  if (r) return r;

  /* Allocate the rvalue and, importantly, leave it locked on exit */
  return sl_reg_allocator_lock_or_alloc(ract, &x->rvalue_);
}

static int sl_expr_clone_lvalue(struct sl_expr *dst, struct sl_expr *src) {
  int r;
  dst->offset_limit_ = src->offset_limit_;
  r = sl_reg_alloc_clone(&dst->offset_reg_, &src->offset_reg_);
  r = r ? r : sl_reg_alloc_clone(&dst->base_regs_, &src->base_regs_);
  return r;
}

static int sl_expr_regs_lock(struct sl_reg_allocator *ract, struct sl_expr *x) {
  int r = 0;
  if (x->base_regs_.local_frame_) {
    if (x->offset_limit_ > INT_MAX) return -1; /* overflow */
    r = sl_reg_allocator_lock_descend(ract, (int)x->offset_limit_, &x->base_regs_);
  }
  if (x->offset_reg_.kind_ != slrak_void) {
    if (x->offset_reg_.local_frame_) {
      r = r ? r : sl_reg_allocator_lock(ract, &x->offset_reg_);
    }
  }
  if (x->rvalue_.kind_ != slrak_void) {
    if (x->rvalue_.local_frame_) {
      r = r ? r : sl_reg_allocator_lock(ract, &x->rvalue_);
    }
  }
  return r;
}

static int sl_expr_regs_is_allocated(struct sl_expr *x) {
  return sl_reg_alloc_is_allocated(&x->base_regs_);
}

static int sl_expr_regs_alloc(struct sl_reg_allocator *ract, struct sl_expr *x) {
  /* Note we allocate for the type of x, we don't allocate in consideration of 
   * the offset_limit_ as that is intended for the case where we receive/inherit the
   * allocation from another expr */
  int r = sl_reg_allocator_alloc(ract, &x->base_regs_);
  if (!r) {
    x->offset_limit_ = 1;
    sl_reg_alloc_void(&x->offset_reg_);
  }
  return r;
}

static int sl_expr_regs_lock_or_alloc(struct sl_reg_allocator *ract, struct sl_expr *x) {
  if (sl_expr_regs_is_allocated(x)) {
    return sl_expr_regs_lock(ract, x);
  }
  return sl_expr_regs_alloc(ract, x);
}

static int sl_expr_regs_unlock(struct sl_reg_allocator *ract, struct sl_expr *x) {
  int r = 0;
  if (x->offset_limit_ > INT_MAX) return -1; /* overflow */
  if (x->base_regs_.local_frame_) {
    r = sl_reg_allocator_unlock_descend(ract, (int)x->offset_limit_, &x->base_regs_);
  }
  if (x->offset_reg_.local_frame_) {
    if (x->offset_reg_.kind_ != slrak_void) {
      r = r ? r : sl_reg_allocator_unlock(ract, &x->offset_reg_);
    }
  }
  if (x->rvalue_.kind_ != slrak_void) {
    if (x->rvalue_.local_frame_) {
      r = r ? r : sl_reg_allocator_lock(ract, &x->rvalue_);
    }
  }
  return r;
}

static int sl_expr_alloc_register_pre_pass(struct sl_type_base *tb, struct sl_reg_allocator *ract, struct sl_expr *x) {
  int r;
  /* Set all base_regs_ to the type of the expression, but don't allocate anything.
   * This ensures we can check whether something is allocated or not because the placeholders for the registers
   * will exist. Not doing this step would keep everything slrak_void, which is considered allocated, making the
   * whole thing fail. */
  r = sl_reg_alloc_set_type(&x->base_regs_, sl_expr_type(tb, x), ract->local_frame_);
  
  /* Clear offset, clear rvalue, set offset limit to known good value. These may all be updated from these defaults
   * by the sl_expr_alloc_register_main_pass() */
  if (!r) sl_reg_alloc_void(&x->offset_reg_);
  if (!r) sl_reg_alloc_void(&x->rvalue_);
  x->offset_limit_ = 1;
  if (r) return r;

  /* .. and do the same for the children */
  size_t n;
  for (n = 0; n < x->num_children_; ++n) {
    r = sl_expr_alloc_register_pre_pass(tb, ract, x->children_[n]);
    if (r) return r;
  }

  return 0;
}

static int sl_expr_alloc_register_main_pass(struct sl_type_base *tb, struct sl_reg_allocator *ract, struct sl_expr *x) {
  size_t n;
  int r = 0;
  if (x->op_ == exop_conditional) {
    /* Conditionals are special as we'd like the true and false sub-expressions to get into the
     * same register as the conditional expression itself; if at all possible. This prevents
     * having to manually move the results. */

    /* First we evaluate the condition, its evaluation result then splits the execution chain into
     * a true and false branch (so we don't need to preserve its register while evaluating those branches.) */
    r = r ? r : sl_expr_alloc_register_main_pass(tb, ract, x->children_[0]);
    r = r ? r : sl_expr_regs_unlock(ract, x->children_[0]);

    /* Lock or alloc the exop_conditional result value, try and clone it into the children if possible. */
    r = r ? r : sl_expr_regs_lock_or_alloc(ract, x);
    
    if (!sl_expr_regs_is_allocated(x->children_[1])) {
      r = r ? r : sl_expr_clone_lvalue(x->children_[1], x);
    }
    if (!sl_expr_regs_is_allocated(x->children_[2])) {
      r = r ? r : sl_expr_clone_lvalue(x->children_[2], x);
    }

    /* At this point, x is locked, the children are not locked, but are allocated and quite possibly identical to x.
     * Enter each child after unlocking x. */
    r = r ? r : sl_expr_regs_unlock(ract, x);
    r = r ? r : sl_expr_alloc_register_main_pass(tb, ract, x->children_[1]);
    r = r ? r : sl_expr_alloc_register_main_pass(tb, ract, x->children_[2]);

    /* Both children 1 & 2 are now locked, unlock them, and re-lock x so we return it locked as expected by caller. */
    r = r ? r : sl_expr_regs_lock(ract, x);
    r = r ? r : sl_expr_regs_unlock(ract, x->children_[1]);
    r = r ? r : sl_expr_regs_unlock(ract, x->children_[2]);

    if (r) return r;

    return 0;
  }
  else if (x->op_ == exop_sequence) {
    /* The value of a sequence (the comma operator) is the last value of that sequence; ideally we'd
     * like that last value to be in the same registers as the sequence's value, so try if possible. */
    
    /* Do first child and drop registers on the floor as we don't care for results */
    r = r?  r : sl_expr_alloc_register_main_pass(tb, ract, x->children_[0]);
    r = r ? r : sl_expr_regs_unlock(ract, x->children_[0]);

    /* Do the 2nd child, try and use those registers for the exop_sequence as well, if possible. */
    if (!sl_expr_regs_is_allocated(x)) {
      /* Re-use the registers for exop_sequence and leave em locked */
      r = r ? r : sl_expr_alloc_register_main_pass(tb, ract, x->children_[1]);
      r = r ? r : sl_expr_clone_lvalue(x, x->children_[1]);
    }
    else {
      /* x already allocated, clone registers; if at all possible */
      if (!sl_expr_regs_is_allocated(x->children_[1])) {
        r = r ? r : sl_expr_clone_lvalue(x->children_[1], x);
        r = r ? r : sl_expr_regs_lock(ract, x);
      }
      else {
        /* x already allocated, x->children_[1] already allocated, cannot share anything; make sure
         * the child has an rvalue if needed */
        r = r ? r : sl_expr_alloc_register_main_pass(tb, ract, x->children_[1]);
        r = r ? r : sl_expr_need_rvalue(tb, ract, x->children_[1]);
      }
      /* Enter sub-expression allocation for last child; lock x, unlock sub-expression, those
       * might be the same. */
      r = r ? r : sl_expr_regs_lock(ract, x);
      r = r ? r : sl_expr_regs_unlock(ract, x->children_[1]);
    }

    if (r) return r;
    return 0;
  }
  else if (x->op_ == exop_variable) {
    /* Clone the variable's reg_alloc and lock it; variables should already be locked,
     * but locking it extra ensures the caller can unlock without consequence. */
    r = r ? r : sl_reg_alloc_clone(&x->base_regs_, &x->variable_->reg_alloc_);
    r = r ? r : sl_expr_regs_lock(ract, x);

    return r;
  }
  else if (x->op_ == exop_literal) {
    /* Allocate for the literal (no dependencies, the value to initialize with
     * is in x->literal_value_, as simple as it gets) */
    r = r ? r : sl_expr_regs_lock_or_alloc(ract, x);
    
    return r;
  }
  else if (x->op_ == exop_array_subscript) {
    /* Allocate both array and subscript, leaving both locked */
    for (n = 0; n < 2; ++n) {
      struct sl_expr *child = x->children_[n];
      r = r ? r : sl_expr_alloc_register_main_pass(tb, ract, x->children_[n]);
      if (r) return r;
    }
    /* Clone the array child into x, we'll be modifying the offset but keeping
     * its base. */
    assert((x->children_[0]->base_regs_.kind_ == slrak_array) && "array subscription should operate on array");
    r = r ? r : sl_reg_alloc_clone(&x->base_regs_, x->children_[0]->base_regs_.v_.array_.head_);
    if (r) return r;
    
    if (x->children_[0]->offset_reg_.kind_ != slrak_void) {
      /* There already is an offset value, allocate a new register so we can
       * perform the calculation for this exop_array_subscript's offset changes. */
      r = r ? r : sl_reg_alloc_set_type(&x->offset_reg_, &tb->int_, ract->local_frame_);
      r = r ? r : sl_reg_allocator_alloc(ract, &x->offset_reg_);
    }
    else {
      /* There is no offset value as of yet, take it from the array subscript index */
      if (x->children_[1]->offset_reg_.kind_ == slrak_void) {
        /* Can take the base reg of child 1 as a clone for the offset ..*/
        r = r ? r : sl_reg_alloc_clone(&x->offset_reg_, &x->children_[1]->base_regs_);
      }
      else {
        /* Cannot take the base reg of child 1 as a clone because it itself has an 
         * offset. Instead, therefore, allocate a new register. This happens when we
         * do things like a[b[2]] -- here the subscript for a, b[2], itself has an 
         * offset. */
        r = r ? r : sl_reg_alloc_set_type(&x->offset_reg_, &tb->int_, ract->local_frame_);
        r = r ? r : sl_reg_allocator_alloc(ract, &x->offset_reg_);
      }
      r = r ? r : sl_reg_allocator_lock(ract, &x->offset_reg_);
    }
    /* Offset is locked, now also lock the base register */
    x->offset_limit_ = x->children_[0]->offset_limit_ * x->children_[0]->base_regs_.v_.array_.num_elements_;
    if (x->offset_limit_ > INT_MAX) return -1;
    r = r ? r : sl_reg_allocator_lock_descend(ract, (int)x->offset_limit_, &x->base_regs_);

    /* The children are still locked, unlock those
     * (note that the x->base_regs_ lock above and the unlock of x->children_[0] here will fully overlap, that's
     *  fine, this way is hopefully more clear and worth it for the reader.) */
    r = r ? r : sl_expr_regs_unlock(ract, x->children_[0]);
    r = r ? r : sl_expr_regs_unlock(ract, x->children_[1]);

    if (r) return r;

    return 0;
  }
  else if ((x->op_ == exop_component_selection) || (x->op_ == exop_constructor)) {
    // XXX: Take care with x->offset_reg_ ; we can only clone registers if the result is entirely in accordance
    //      with a single x->offset_reg_. Language requires that we return this as an lvalue (e.g. vec3 v; v.xy = vec2(1,0) is valid, v.xy needs to be an lvalue.)

    int can_lvalue = 1;
    /* Check that the children's offset_reg_ and offset_limit_ are all identical; if not, then
     * we won't be able to produce an lvalue; this would be fatal for exop_component_selection. */
    for (n = 0; n < x->num_components_; ++n) {
      struct sl_component_selection *cs = x->swizzle_ + n;
      if (cs->parameter_index_ == -1) {
        /* Cannot create an lvalue when one of the components is an implicit constant literal */
        can_lvalue = 0;
        break;
      }
      switch (cs->conversion_) {
        case slcc_float_to_float:
        case slcc_int_to_int:
        case slcc_bool_to_bool:
          /* good, keep going */
          break;
        default:
          /* Conversion implies no lvalue */
          can_lvalue = 0;
          break;
      }
      if (!can_lvalue) break;
    }

    /* Alloc & lock registers for children */
    for (n = 0; n < x->num_children_; ++n) {
      r = r ? r : sl_expr_alloc_register_main_pass(tb, ract, x->children_[n]);
    }

    if (can_lvalue) {
      struct sl_component_selection *first_cs = x->swizzle_;
      struct sl_expr *first_expr = x->children_[first_cs->parameter_index_];
      for (n = 1; n < x->num_components_; ++n) {
        struct sl_component_selection *cs = x->swizzle_ + n;
        struct sl_expr *child = x->children_[cs->parameter_index_];
        if (child->offset_limit_ != first_expr->offset_limit_) {
          /* Sourced from different sized array subscriptions, consequently we cannot lock and unlock so 
           * cannot produce an lvalue. */
          can_lvalue = 0;
          break;
        }
        else if ((child->offset_reg_.kind_ == slrak_void) &&
                 (first_expr->offset_reg_.kind_ == slrak_void)) {
          /* Both the first we compare against and the current child do not have an offset, this is good
           * for producing an lvalue.
           * .. keep going .. */
        }
        else if ((child->offset_reg_.kind_ != slrak_int) ||
                 (first_expr->offset_reg_.kind_ != slrak_int)) {
          /* Mismatching kinds, problem, cannot produce an lvalue */
          can_lvalue = 0;
          break;
        }
        else if (child->offset_reg_.v_.regs_[0] == first_expr->offset_reg_.v_.regs_[0]) {
          /* Matching kinds *and* they're ints *and* they're the same offset register.
           * This is conducive for producing an lvalue.
           * .. keep going .. */
        }
        else {
          /* Mismatching offset registers, cannot produce an lvalue because the overal datatype
           * produced would not produce an lvalue */
          can_lvalue = 0;
          break;
        }
      }
    }

    /* Check that all components are either from the global frame, or from a local frame;
     * if not, then we cannot produce an lvalue (because we cannot determine a local_frame_
     * flag that would be consistent.) */
    int is_local_frame = 1;
    if (can_lvalue) {
      for (n = 0; n < x->num_components_; ++n) {
        struct sl_component_selection *cs = x->swizzle_ + n;
        struct sl_expr *child = x->children_[cs->parameter_index_];
        if (!n) {
          is_local_frame = child->base_regs_.local_frame_;
        }
        else if (child->base_regs_.local_frame_ != is_local_frame) {
          /* Inconsistent local vs. global mix */
          can_lvalue = 0;
          break;
        }
      }
    }

    if (!can_lvalue && (x->op_ == exop_component_selection)) {
      /* Cannot lvalue and this is a component selection e.g. "vec2 v; v.xy", this is fatal. */
      assert(0 && "Failed to produce lvalue for an exop_component_selection");
      return -1;
    }

    if (can_lvalue) {
      /* Clone according to the swizzles; following the checks above we know this should work */
      x->base_regs_.local_frame_ = is_local_frame;
      for (n = 0; n < x->num_components_; ++n) {
        struct sl_component_selection *cs = x->swizzle_ + n;
        
        if (x->base_regs_.v_.regs_[n] == SL_REG_NONE) {
          x->base_regs_.v_.regs_[n] = x->children_[cs->parameter_index_]->base_regs_.v_.regs_[cs->component_index_];
        }
        else {
          /* Component already allocated with a register, making it impossible to clone...
           * ... this might occur if we do, say:
           *   vec2 v;
           *   vec3 a;
           *   bool c;
           *   v = c ? v.xy : v.yz;
           * In the last line, "c ? v.xy : v.yz", the ?: exop_conditional will pre-assign the allocation
           * for the "v.xy" and "v.yz" children, consequently, those swizzles will have allocations pre-assigned
           * to them. Following least surprise, we'll accommodate this behavior and not overwrite. The result
           * will not be an lvalue, but that's fine because it's used in an exop_conditional which doesn't need
           * or produce an lvalue. */
        }
      }
      assert(x->num_components_ && "Should have at least one component for swizzle");
      r = r ? r : sl_reg_alloc_clone(&x->offset_reg_, &x->children_[x->swizzle_->parameter_index_]->offset_reg_);
      x->offset_limit_ = x->children_[x->swizzle_->parameter_index_]->offset_limit_;

      /* Now lock in addition so x is locked when we return, even if we unlock the overlapping children. */
      sl_expr_regs_lock(ract, x);
    }
    else {
      /* Cannot lvalue, build out new registers for all and leave them locked */
      sl_expr_regs_lock_or_alloc(ract, x);
    }

    /* With our components now mapped out and locked, unlock all our children; this will leave the components that
     * we cloned from our children locked, because they've been +ref locked on top of the children_'s lock in the loop
     * above. */
    for (n = 0; n < x->num_children_; ++n) {
      r = r ? r : sl_expr_regs_unlock(ract, x->children_[n]);
    }

    return r;
  }
  else if (x->op_ == exop_field_selection) {
    // Find the reg_alloc_ of the child's field we'd like to select; then evaluate the child,
    // copy the field's corresponding reg_alloc over as our own reg_alloc and unlock everything else.
    struct sl_type *et = sl_type_unqualified(sl_expr_type(tb, x->children_[0]));
    if (!et) return -1;
    
    r = r ? r : sl_expr_alloc_register_main_pass(tb, ract, x->children_[0]);

    if (r) return r;

    assert((x->children_[0]->base_regs_.kind_ == slrak_struct) && "reg_alloc of exop_field_selection child should be slrak_struct");
    assert((et == x->children_[0]->base_regs_.v_.comp_.struct_type_) && "type mismatch on child expression and reg_alloc's value");

    /* Find the sequential index of the type */
    size_t field_index = 0;
    int field_was_found = 0;
    struct sl_type_field *tf;
    tf = et->fields_;
    if (tf) {
      do {
        tf = tf->chain_;

        if (tf == x->field_selection_) {
          field_was_found = 1;
          break; /* found it */
        }

        field_index++;
      } while (tf != et->fields_);
    }
    assert(field_was_found && "exop_field_selection but no field was found");

    /* Clone the field we selected */
    r = r ? r : sl_reg_alloc_clone(&x->base_regs_, x->children_[0]->base_regs_.v_.comp_.fields_ + field_index);
    r = r ? r : sl_reg_alloc_clone(&x->offset_reg_, &x->children_[0]->offset_reg_);
    if (!r) x->offset_limit_ = x->children_[0]->offset_limit_;

    /* Lock ourselves, unlock the child */
    r = r ? r : sl_expr_regs_lock(ract, x);
    r = r ? r : sl_expr_regs_unlock(ract, x->children_[0]);

    return r;
  }
  else if (x->op_ == exop_assign) {
    /* Recursively process each child (both lvalue and rvalue,) registers will be held locked */
    r = r ? r : sl_expr_alloc_register_main_pass(tb, ract, x->children_[0]);
    r = r ? r : sl_expr_alloc_register_main_pass(tb, ract, x->children_[1]);

    /* For the second child, we'd like to have an rvalue. Not so much for the first 
     * as exop_assign overwrites the destination without concern for prior contents. */
    r = r ? r : sl_expr_need_rvalue(tb, ract, x->children_[1]);

    /* Assume the assignment is performed here during evaluation.
     * Unlock lvalue, clone the registers for the rvalue into our own expression 
     * (and don't unlock).
     */
    r = r ? r : sl_expr_regs_unlock(ract, x->children_[0]);
    r = r ? r : sl_expr_regs_unlock(ract, x->children_[1]);

    /* Clone the assigned right value to the result and leave it locked (so caller has us
     * locked as expected.) */
    if (x->children_[1]->rvalue_.kind_ != slrak_void) {
      r = r ? r : sl_reg_alloc_clone(&x->base_regs_, &x->children_[1]->rvalue_);
    }
    else {
      r = r ? r : sl_reg_alloc_clone(&x->base_regs_, &x->children_[1]->base_regs_);
    }

    r = r ? r : sl_expr_regs_lock(ract, x);

    return r;
  }
  else if ((x->op_ == exop_mul_assign) ||
           (x->op_ == exop_div_assign) ||
           (x->op_ == exop_add_assign) ||
           (x->op_ == exop_sub_assign)) {
    /* This is like exop_assign and the regular case, but unlike exop_assign, we do need
     * the first child to be an rvalue, and unlike the regular case, even when we convert
     * the first child to an rvalue, we still need to keep the lvalue equivalent locked
     * for the assignment. */

    /* Recursively process each child (both lvalue and rvalue,) registers will be held locked */
    r = r ? r : sl_expr_alloc_register_main_pass(tb, ract, x->children_[0]);
    r = r ? r : sl_expr_alloc_register_main_pass(tb, ract, x->children_[1]);

    /* For the first child, we'd like to have an rvalue *and* an lvalue. */
    r = r ? r : sl_expr_need_rvalue(tb, ract, x->children_[0]);

    /* For the second child, we'd like to have an rvalue and, unlike the first child,
     * can let go of the lvalue if we allocated for an rvalue. */
    r = r ? r : sl_expr_need_rvalue(tb, ract, x->children_[1]);

    /* Assume the assignment is performed here during evaluation.. can now unlock */

    /* The op-assign operators produce a result, even if the language does not support it,
     * their operation does require a result-register to be available.
     * (Reason is, in "a += b", both "a" and "b" may be arrays, but are needed as rvalues so
     * the operation can succeed. In addition, we'd like to piggy-back on the simpler + - * /
     * binary operator logic as these are non-trivial. Having the exop_XXX_assign allocate its
     * own register helps facilitate this. */
    r = sl_expr_regs_alloc(ract, x);

    r = r ? r : sl_expr_regs_unlock(ract, x->children_[0]);
    r = r ? r : sl_expr_regs_unlock(ract, x->children_[1]);

    return r;
  }

  /* Recursively process each child. On the return from sl_expr_alloc_register_main_pass() the
   * corresponding x->children_[n]->reg_alloc_ will be held locked. */
  for (n = 0; n < x->num_children_; ++n) {
    r = sl_expr_alloc_register_main_pass(tb, ract, x->children_[n]);
    if (r) return r;
  }

  /* Prepare an rvalue_ for each child, if needed. The child needs an rvalue if the
   * actual value to retrieve is at a base register + register offset_.
   * We assume all operands need to be in rvalues, this is not necessarily true for
   * some expression types but these are handled as special cases above.
   * The corresponding x->children_[n]->reg_alloc_.rvalue_ will be helded locked on exit,
   * if an rvalue_ was necessary. */
  for (n = 0; n < x->num_children_; ++n) {
    r = sl_expr_need_rvalue(tb, ract, x->children_[n]);

    /* IF we have a separate rvalue_ register, then we can let go of the original
     * registers (because we have a copy of the value we need.) */
    if (x->children_[n]->rvalue_.kind_ != slrak_void) {
      /* To let go of the original registers, we first lock the rvalue (an additional time)
       * and then unlock all for the child expression; this ensures we unlock the full
       * range in consideration of any offset_reg_ and offset_limit_ for the child, but
       * leave the rvalue locked. */
      r = r ? r : sl_reg_allocator_lock(ract, &x->children_[n]->rvalue_);
      r = r ? r : sl_expr_regs_unlock(ract, x->children_[n]);
    }

    if (r) return r;
  }

  /* Now allocate or lock our expression's registers */
  r = sl_expr_regs_alloc(ract, x);

  /* ... during evaluation, this is where the computation occurs ... */

  /* We no longer need the children's registers to be locked, unlock each.
   * .. this is refcount based, there could be some other reason for a lock
   *    to still be held. */
  for (n = 0; n < x->num_children_; ++n) {
    if (x->children_[n]->rvalue_.kind_ != slrak_void) {
      /* Child has an rvalue, this also implies it is the only thing held locked
       * by this function right now (see above.) */
      r = sl_reg_allocator_unlock(ract, &x->children_[n]->rvalue_);
    }
    else {
      /* Child has no rvalue, implying the main value is what's currently held locked */
      r = sl_expr_regs_unlock(ract, x->children_[n]);
    }
    if (r) return r;
  }

  /* All done; x->reg_alloc_ still held locked, but callers are expecting it
   * that way. */
  return 0;
}

int sl_expr_alloc_registers(struct sl_type_base *tb, struct sl_reg_allocator *ract, struct sl_expr *x, int needs_rvalue) {
  int r;
  r = sl_expr_alloc_register_pre_pass(tb, ract, x);
  if (r) return r;

  r = sl_expr_alloc_register_main_pass(tb, ract, x);
  if (r) return r;

  if (needs_rvalue) {
    r = sl_expr_need_rvalue(tb, ract, x);
  }

  sl_expr_regs_unlock(ract, x);
  
  return 0;
}

void sl_expr_attach_caller(struct sl_expr *x, struct sl_function *f) {
  if (x->function_) sl_expr_detach_caller(x);
  if (f->callers_) {
    x->next_caller_ = f->callers_;
    x->prev_caller_ = x->next_caller_->prev_caller_;
    x->next_caller_->prev_caller_ = x->prev_caller_->next_caller_ = x;
    x->function_ = f;
  }
  else {
    x->next_caller_ = x->prev_caller_ = x;
    f->callers_ = x;
    x->function_ = f;
  }
}

void sl_expr_detach_caller(struct sl_expr *x) {
  if (!x->function_) return;
  if (x->next_caller_ == x) {
    assert(x->function_->callers_ == x);
    x->function_->callers_ = NULL;
  }
  else {
    x->next_caller_->prev_caller_ = x->prev_caller_;
    x->prev_caller_->next_caller_ = x->next_caller_;
    if (x->function_->callers_ == x) {
      x->function_->callers_ = x->next_caller_;
    }
  }
  x->next_caller_ = x->prev_caller_ = NULL;
  x->function_ = NULL;
}
