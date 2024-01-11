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

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef SL_EXPR_H_INCLUDED
#define SL_EXPR_H_INCLUDED
#include "sl_expr.h"
#endif


void sl_expr_init(struct sl_expr *x) {
  x->op_ = exop_invalid;
  situs_init(&x->op_loc_);
  x->num_children_ = 0;
  x->children_ = NULL;
  sl_expr_temp_init_void(&x->literal_value_);
  x->num_components_ = 0;
  memset(x->component_selection_, 0, sizeof(x->component_selection_));
  x->field_selection_ = NULL;
  x->function_ = NULL;
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

int sl_expr_eval(struct sl_type_base *tb, struct sl_expr *x, struct sl_expr_temp *r) {
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
          res = -1; // XXX: IMPLEMENT
          break;
        case exop_logical_or:
          res = -1; // XXX: IMPLEMENT
          break;
        case exop_logical_xor:
          res = -1; // XXX: IMPLEMENT
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

    case exop_function_call:
      /* XXX: Fish out the builtin functions and call those, return -1 for everything else. */
      return -1;

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

      switch (tmp.kind_) {
        case sletk_void:
          sl_expr_temp_cleanup(&tmp);
          if (children) free(children);
          return 0; /* easy. */
        case sletk_array:
        case sletk_struct: {
          if (x->num_components_ != x->num_children_) {
            for (n = 0; n < x->num_children_; ++n) {
              sl_expr_temp_cleanup(children + n);
            }
            if (children) free(children);
            sl_expr_temp_cleanup(&tmp);
            return -1;
          }
          for (n = 0; n < x->num_components_; ++n) {
            if (sl_expr_temp_copy(tmp.v_.comp_.elements_ + n, children + n)) {
              for (n = 0; n < x->num_children_; ++n) {
                sl_expr_temp_cleanup(children + n);
              }
              if (children) free(children);
              sl_expr_temp_cleanup(&tmp);
              return -1;
            }
          }
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

      for (n = 0; n < x->num_components_; ++n) {
        struct sl_component_selection *cs = x->swizzle_ + n;
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
      if (failed) {
        for (n = 0; n < x->num_children_; ++x) {
          sl_expr_temp_cleanup(children + n);
        }
        if (children) free(children);
        sl_expr_temp_cleanup(&tmp);
        return -1;
      }
      break;
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
      if (t->kind_ != sltk_array) return NULL;
      return t->derived_type_;
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
  x->function_ = f;
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

struct sl_expr *sl_expr_alloc_field_selection(struct sl_expr *expr, struct sl_type_field *field, const struct situs *field_loc) {
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


