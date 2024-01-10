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

#ifndef SITUS_H_INCLUDED
#define SITUS_H_INCLUDED
#include "pp/situs.h"
#endif

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef DIAGS_H_INCLUDED
#define DIAGS_H_INCLUDED
#include "pp/diags.h"
#endif

#ifndef SYM_TABLE_H_INCLUDED
#define SYM_TABLE_H_INCLUDED
#include "sym_table.h"
#endif

#ifndef SL_EXPR_H_INCLUDED
#define SL_EXPR_H_INCLUDED
#include "sl_expr.h"
#endif

#ifndef SL_FRAME_H_INCLUDED
#define SL_FRAME_H_INCLUDED
#include "sl_frame.h"
#endif

#ifndef GLSL_ES1_ASSIST_H_INCLUDED
#define GLSL_ES1_ASSIST_H_INCLUDED
#include "glsl_es1_assist.h"
#endif

/* Index by uint8, returns a uint8 of which the high nibble is the
 * name set and the low nibble is the component index.
 * You can't mix name sets, for instance:
 * vec2 v2;
 * v2.ry; // r is from set 1, y is from set 2, this is illegal
 * hence we identify the set here.
 */
static uint8_t g_swizzle_map[256] ={
  ['r'] = 0x10,['g'] = 0x11,['b'] = 0x12,['a'] = 0x13,
  ['x'] = 0x20,['y'] = 0x21,['z'] = 0x22,['w'] = 0x23,
  ['s'] = 0x30,['t'] = 0x31,['p'] = 0x32,['q'] = 0x33
};

void glsl_es1_function_call_init(struct glsl_es1_function_call *fc) {
  fc->name_ = NULL;
  situs_init(&fc->loc_);
  fc->constructor_type_ = NULL;
  fc->num_parameters_ = 0;
  fc->num_parameters_allocated_ = 0;
  fc->parameters_ = NULL;
}

void glsl_es1_function_call_move(struct glsl_es1_function_call *dst, struct glsl_es1_function_call *src) {
  if (dst->name_) free(dst->name_);
  dst->name_ = src->name_;
  src->name_ = NULL;
  situs_swap(&dst->loc_, &src->loc_);
  dst->constructor_type_ = src->constructor_type_;
  src->constructor_type_ = NULL;
  if (dst->parameters_) {
    size_t n;
    for (n = 0; n < dst->num_parameters_; ++n) {
      sl_expr_free(dst->parameters_[n].expr_);
      situs_cleanup(&dst->parameters_[n].loc_);
    }
    free(dst->parameters_);
  }
  dst->num_parameters_ = src->num_parameters_;
  src->num_parameters_ = 0;
  dst->num_parameters_allocated_ = src->num_parameters_allocated_;
  src->num_parameters_allocated_ = 0;
  dst->parameters_ = src->parameters_;
  src->parameters_ = NULL;
}

void glsl_es1_function_call_cleanup(struct glsl_es1_function_call *fc) {
  if (fc->name_) free(fc->name_);
  situs_cleanup(&fc->loc_);
  if (fc->parameters_) {
    size_t n;
    for (n = 0; n < fc->num_parameters_; ++n) {
      sl_expr_free(fc->parameters_[n].expr_);
      situs_cleanup(&fc->parameters_[n].loc_);
    }
    free(fc->parameters_);
  }
}

int glsl_es1_function_call_append_parameter(struct glsl_es1_function_call *fc, struct sl_expr *param_expr, const struct situs *param_loc) {
  if (fc->num_parameters_ == fc->num_parameters_allocated_) {
    size_t new_num_parameters_allocated = fc->num_parameters_allocated_ + fc->num_parameters_allocated_ + 1;
    struct glsl_es1_function_call_parameter *new_parameters = realloc(fc->parameters_, new_num_parameters_allocated * sizeof(struct glsl_es1_function_call_parameter));
    if (!new_parameters) return -1;
    fc->parameters_ = new_parameters;
    fc->num_parameters_allocated_ = new_num_parameters_allocated;
  }
  fc->parameters_[fc->num_parameters_].expr_ = param_expr;
  situs_init(&fc->parameters_[fc->num_parameters_].loc_);
  situs_clone(&fc->parameters_[fc->num_parameters_].loc_, param_loc);
  fc->num_parameters_++;
  return 0;
}

static int glsl_es1_sl_type_num_components(struct sl_type *t) {
  switch (t->kind_) {
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
  }
  return 0;
}

static sl_type_kind_t glsl_es1_sl_type_scalar_kind(struct sl_type *t) {
  switch (t->kind_) {
    case sltk_float:
    case sltk_vec2:
    case sltk_vec3:
    case sltk_vec4:
    case sltk_mat2:
    case sltk_mat3:
    case sltk_mat4:
      return sltk_float;
    case sltk_int:
    case sltk_ivec2:
    case sltk_ivec3:
    case sltk_ivec4:
      return sltk_int;
    case sltk_bool:
    case sltk_bvec2:
    case sltk_bvec3:
    case sltk_bvec4:
      return sltk_bool;
  }
  return sltk_invalid;
}

static sl_component_conversion_t glsl_es1_sl_type_component_conversion(struct sl_type *from, struct sl_type *to) {
  sl_type_kind_t from_scalar = glsl_es1_sl_type_scalar_kind(from);
  sl_type_kind_t to_scalar = glsl_es1_sl_type_scalar_kind(to);
  switch (from_scalar) {
    case sltk_float:
      switch (to_scalar) {
        case sltk_float:
          return slcc_float_to_float;
        case sltk_int:
          return slcc_float_to_int;
        case sltk_bool:
          return slcc_float_to_bool;
      }
      break;
    case sltk_int:
      switch (to_scalar) {
        case sltk_float:
          return slcc_int_to_float;
        case sltk_int:
          return slcc_int_to_int;
        case sltk_bool:
          return slcc_int_to_bool;
      }
      break;
    case sltk_bool:
      switch (to_scalar) {
        case sltk_float:
          return slcc_bool_to_float;
        case sltk_int:
          return slcc_bool_to_int;
        case sltk_bool:
          return slcc_bool_to_bool;
      }
      break;
  }
  return slcc_invalid;
}

struct sl_expr *glsl_es1_field_or_swizzle_selection(struct diags *dx, struct sl_type_base *tb, struct sl_expr *x, char *field_id, const struct situs *field_loc) {
  if (!x) return NULL; /* pass-through errors */
  struct sl_type *t = sl_expr_type(tb, x);
  if (!t) {
    /* Failed to determine type, assume this error is already reported. */
    return NULL;
  }
  t = sl_type_unqualified(t);
  if (t->kind_ == sltk_struct) {
    /* Look for a matching field */
    struct sl_type_field *tf;
    tf = t->fields_;
    if (tf) {
      do {
        tf = tf->chain_;

        if (!strcmp(tf->ident_, field_id)) {
          /* Found a match */
          struct sl_expr *field_expr = sl_expr_alloc_field_selection(x, tf, field_loc);
          if (!field_expr) {
            dx_no_memory(dx);
            return NULL;
          }
          return field_expr;
        }
      } while (tf != t->fields_);
    }

    /* No match, report as error */
    dx_error_loc(dx, field_loc, "No field named \"%s\" in struct \"%s\", see line %d", field_id, t->name_, situs_line(&t->tag_loc_));
    return NULL;
  }
  else if ((t->kind_ == sletk_vec2) || (t->kind_ == sletk_vec3) || (t->kind_ == sletk_vec4) ||
           (t->kind_ == sletk_ivec2) || (t->kind_ == sletk_ivec3) || (t->kind_ == sletk_ivec4) ||
           (t->kind_ == sletk_bvec2) || (t->kind_ == sletk_bvec3) || (t->kind_ == sletk_bvec4)) {
    /* Look for a matching swizzle */
    int num_src_components = glsl_es1_sl_type_num_components(t);
    size_t num_tgt_components = strlen(field_id);
    if (num_tgt_components > 4) {
      dx_error_loc(dx, field_loc, "Too many vector components for swizzle \"%s\"", field_id);
      return NULL;
    }

    size_t n;
    int name_set = 0;
    char component_selection[4] = {0};
    /* While vec4 is the largest type we support, we want to initialize the entire swizzle
     * field in sl_expr. */
    struct sl_component_selection swizzle_table[16] = { 0 };

    sl_type_kind_t scalar_kind = glsl_es1_sl_type_scalar_kind(t);
    sl_component_conversion_t component_noconv = slcc_invalid;
    switch (scalar_kind) {
      case sltk_float:
        component_noconv = slcc_float_to_float;
        break;
      case sltk_int:
        component_noconv = slcc_int_to_int;
        break;
      case sltk_bool:
        component_noconv = slcc_bool_to_bool;
        break;
    }

    for (n = 0; n < num_tgt_components; ++n) {
      /* Determine the digit location of the component; this is needed if we report any error on it. */
      struct situs cut_field;
      struct situs digit_loc;
      situs_init(&cut_field);
      situs_init(&digit_loc);
      if (situs_clone(&cut_field, field_loc) ||
          situs_skip(&cut_field, n, field_id) ||
          situs_move_range(&digit_loc, &cut_field, 1, field_id+n)) {
        situs_cleanup(&cut_field);
        situs_cleanup(&digit_loc);
        dx_no_memory(dx);
        return NULL;
      }

      uint8_t swizzle = g_swizzle_map[(uint8_t)field_id[n]];

      if (!swizzle) {
        dx_error_loc(dx, &digit_loc, "Invalid component \"%c\", see line %d", field_id[n]);

        situs_cleanup(&cut_field);
        situs_cleanup(&digit_loc);

        return NULL;
      }
      else if ((swizzle & 0xF) >= num_src_components) {
        dx_error_loc(dx, &digit_loc, "Type %s is not large enough for component \"%c\", see line %d", t->name_, field_id[n]);

        situs_cleanup(&cut_field);
        situs_cleanup(&digit_loc);

        return NULL;
      }

      if (!name_set) {
        name_set = swizzle & 0xF0;
      }
      else if (name_set != (swizzle & 0xF0)) {
        dx_error_loc(dx, &digit_loc, "Cannot mix component sets");

        situs_cleanup(&cut_field);
        situs_cleanup(&digit_loc);

        return NULL;
      }

      component_selection[n] = (uint8_t)field_id[n];
      swizzle_table[n].conversion_ = component_noconv;
      swizzle_table[n].parameter_index_ = 0;
      swizzle_table[n].component_index_ = swizzle & 0xF;
    }

    struct sl_expr *expr = sl_expr_alloc_swizzle_selection(x, num_tgt_components, component_selection, field_loc);
    if (!expr) {
      dx_no_memory(dx);
      return NULL;
    }

    /* Copy over the swizzle */
    assert(sizeof(swizzle_table) == sizeof(expr->swizzle_));
    for (n = 0; n < sizeof(swizzle_table) / sizeof(*swizzle_table); ++n) {
      expr->swizzle_[n] = swizzle_table[n];
    }

    return expr;
  }
  else {
    dx_error_loc(dx, field_loc, "Cannot select field or swizzle components from type \"%s\"", t->name_);
    return NULL;
  }
}

struct sl_expr *glsl_es1_function_call_realize(struct diags *dx, struct sym_table *st, struct sl_type_base *tb, struct glsl_es1_function_call *fs) {
  if (fs->constructor_type_) {
    /* Constructor call. */
    sl_type_kind_t scalar_kind = sltk_invalid;
    struct sl_type *t = sl_type_unqualified(fs->constructor_type_);
    int row, col;
    int num_target_components = 0;
    struct sl_component_selection swizzle[16]; /* 4x4 matrix is the largest type we support */
    
    scalar_kind = glsl_es1_sl_type_scalar_kind(t);
    sl_component_conversion_t constant_conv = slcc_invalid;
    switch (scalar_kind) {
      case sltk_float:
        constant_conv = slcc_float_to_float;
        break;
      case sltk_int:
        constant_conv = slcc_int_to_int;
        break;
      case sltk_bool:
        constant_conv = slcc_bool_to_bool;
        break;
    }

    /* Set swizzle to known value (the constant (param=-1) 0 (component=0)) */
    for (row = 0; row < 4; ++row) {
      for (col = 0; col < 4; ++col) {
        swizzle[col * 4 + row].conversion_ = constant_conv;
        swizzle[col * 4 + row].parameter_index_ = -1;
        swizzle[col * 4 + row].component_index_ = 0;
      }
    }

    struct sl_type *t_first_param = NULL;
    if (!fs->num_parameters_) {
      dx_error_loc(dx, &fs->loc_, "Constructor \"%s\" expects one or more parameters, got 0", t->name_);
      return NULL;
    }
    t_first_param = sl_expr_type(tb, fs->parameters_[0].expr_);
    if (!t_first_param) {
      /* No type, assume this was an issue that's already reported. */
      return NULL;
    }

    if ((fs->num_parameters_ == 1) &&
        ((t->kind_ == sltk_mat2 || t->kind_ == sltk_mat3 || t->kind_ == sltk_mat4)) &&
        ((t_first_param->kind_ == sltk_mat2 || t_first_param->kind_ == sltk_mat3 || t_first_param->kind_ == sltk_mat4)) ) {
      /* Matrix constructor from 1 matrix; this is a special case.. */
      /* Any components not explicitly set are set to the identity matrix. */
      for (col = 0; col < 4; ++col) {
        for (row = 0; row < 4; ++row) {
          swizzle[col * 4 + row].conversion_ = slcc_float_to_float;
          swizzle[col * 4 + row].parameter_index_ = -1;
          swizzle[col * 4 + row].component_index_ = col == row ? 1 : 0;
        }
      }
      switch (t->kind_) {
        case sltk_mat2: {
          switch (t_first_param->kind_) {
            case sltk_mat2:
              for (col = 0; col < 2; ++col) {
                for (row = 0; row < 2; ++row) {
                  swizzle[col * 2 + row].parameter_index_ = 0;
                  swizzle[col * 2 + row].component_index_ = col * 2 + row;
                }
              }
              break;
            case sltk_mat3:
              for (col = 0; col < 2; ++col) {
                for (row = 0; row < 2; ++row) {
                  swizzle[col * 2 + row].parameter_index_ = 0;
                  swizzle[col * 2 + row].component_index_ = col * 3 + row;
                }
              }
              break;
            case sltk_mat4:
              for (col = 0; col < 2; ++col) {
                for (row = 0; row < 2; ++row) {
                  swizzle[col * 2 + row].parameter_index_ = 0;
                  swizzle[col * 2 + row].component_index_ = col * 4 + row;
                }
              }
              break;
          }
          break;
        }
        case sltk_mat3:
          switch (t_first_param->kind_) {
            case sltk_mat2:
              for (col = 0; col < 2; ++col) {
                for (row = 0; row < 2; ++row) {
                  swizzle[col * 3 + row].parameter_index_ = 0;
                  swizzle[col * 3 + row].component_index_ = col * 2 + row;
                }
              }
              break;
            case sltk_mat3:
              for (col = 0; col < 3; ++col) {
                for (row = 0; row < 3; ++row) {
                  swizzle[col * 3 + row].parameter_index_ = 0;
                  swizzle[col * 3 + row].component_index_ = col * 3 + row;
                }
              }
              break;
            case sltk_mat4:
              for (col = 0; col < 3; ++col) {
                for (row = 0; row < 3; ++row) {
                  swizzle[col * 3 + row].parameter_index_ = 0;
                  swizzle[col * 3 + row].component_index_ = col * 4 + row;
                }
              }
              break;
          }
          break;
        case sltk_mat4:
          switch (t_first_param->kind_) {
            case sltk_mat2:
              for (col = 0; col < 2; ++col) {
                for (row = 0; row < 2; ++row) {
                  swizzle[col * 4 + row].parameter_index_ = 0;
                  swizzle[col * 4 + row].component_index_ = col * 2 + row;
                }
              }
              break;
            case sltk_mat3:
              for (col = 0; col < 3; ++col) {
                for (row = 0; row < 3; ++row) {
                  swizzle[col * 4 + row].parameter_index_ = 0;
                  swizzle[col * 4 + row].component_index_ = col * 3 + row;
                }
              }
              break;
            case sltk_mat4:
              for (col = 0; col < 4; ++col) {
                for (row = 0; row < 4; ++row) {
                  swizzle[col * 4 + row].parameter_index_ = 0;
                  swizzle[col * 4 + row].component_index_ = col * 4 + row;
                }
              }
              break;
          }
          break;
      }
    }
    else {
      /* Not matrix-to-matrix constructor, but scalar-to-vector or component-wise constructor */

      num_target_components = glsl_es1_sl_type_num_components(t);
      int num_src_components = 0;
      int n;
      for (n = 0; n < fs->num_parameters_; ++n) {
        struct sl_type *pt = sl_expr_type(tb, fs->parameters_[n].expr_);
        if (!pt) {
          /* No type, assume this was an issue that's already reported. */
          return NULL;
        }
        int num_param_components = glsl_es1_sl_type_num_components(pt);
        if (!num_param_components) {
          dx_error_loc(dx, &fs->parameters_[n].loc_, "Invalid parameter for constructor, it has no components");
          return NULL;
        }
        num_src_components += num_param_components;
      }

      if (num_src_components == 1) {
        struct sl_type *pt = sl_expr_type(tb, fs->parameters_->expr_);
        sl_component_conversion_t conv = glsl_es1_sl_type_component_conversion(pt, t);
        /* This also implies only 1 parameter as parameters with 0
         * components are an error */
        struct glsl_es1_function_call_parameter *p = fs->parameters_;
        switch (t->kind_) {
          case sltk_float:
          case sltk_int:
          case sltk_bool:
            /* Initialize scalar from scalar. */
            swizzle[0].conversion_ = conv;
            swizzle[0].parameter_index_ = 0;
            swizzle[0].component_index_ = 0;
            break;
          case sltk_vec2:
          case sltk_vec3:
          case sltk_vec4:
          case sltk_ivec2:
          case sltk_ivec3:
          case sltk_ivec4:
          case sltk_bvec2:
          case sltk_bvec3:
          case sltk_bvec4: {
            /* Initialize each component with the scalar. */
            int n;
            for (n = 0; n < num_target_components; ++n) {
              swizzle[n].conversion_ = conv;
              swizzle[n].parameter_index_ = 0;
              swizzle[n].component_index_ = 0;
            }
            break;
          }
          /* Initialize matrix diagonal from scalar. */
          case sltk_mat2:
            swizzle[0].conversion_ = conv;
            swizzle[0].parameter_index_ = 0;
            swizzle[0].component_index_ = 0;
            swizzle[3].conversion_ = conv;
            swizzle[3].parameter_index_ = 0;
            swizzle[3].component_index_ = 0;
            break;
          case sltk_mat3:
            /* Initialize matrix diagonal from scalar. */
            swizzle[0].conversion_ = conv;
            swizzle[0].parameter_index_ = 0;
            swizzle[0].component_index_ = 0;
            swizzle[4].conversion_ = conv;
            swizzle[4].parameter_index_ = 0;
            swizzle[4].component_index_ = 0;
            swizzle[8].conversion_ = conv;
            swizzle[8].parameter_index_ = 0;
            swizzle[8].component_index_ = 0;
            break;
          case sltk_mat4:
            /* Initialize matrix diagonal from scalar. */
            swizzle[0].conversion_ = conv;
            swizzle[0].parameter_index_ = 0;
            swizzle[0].component_index_ = 0;
            swizzle[5].conversion_ = conv;
            swizzle[5].parameter_index_ = 0;
            swizzle[5].component_index_ = 0;
            swizzle[10].conversion_ = conv;
            swizzle[10].parameter_index_ = 0;
            swizzle[10].component_index_ = 0;
            swizzle[15].conversion_ = conv;
            swizzle[15].parameter_index_ = 0;
            swizzle[15].component_index_ = 0;
            break;
        }
        // DONE
      }
      else if (num_target_components == num_src_components) {
        // Spread parameters over components
        int current_component = 0;
        for (n = 0; n < fs->num_parameters_; ++n) {
          // Note that we already checked that the parameter has components
          struct sl_type *pt = sl_expr_type(tb, fs->parameters_[n].expr_);
          sl_component_conversion_t conv = glsl_es1_sl_type_component_conversion(pt, t);
          int num_param_components = glsl_es1_sl_type_num_components(pt);
          int param_comp;
          for (param_comp = 0; param_comp < num_param_components; ++param_comp) {
            swizzle[current_component].conversion_ = conv;
            swizzle[current_component].parameter_index_ = n;
            swizzle[current_component].component_index_ = param_comp;
            ++current_component;
          }
        }
        assert(current_component == num_target_components);

        // DONE
      }
      else {
        dx_error_loc(dx, &fs->loc_, "Constructor \"%s\" expects %d components, got %d", t->name_, num_target_components, num_src_components);
        return NULL;
      }
    }
    /* Create a new expression corresponding to the constructor we just formed. */
    struct sl_expr *constr_expr = sl_expr_alloc_constructor(t, &fs->loc_, fs->num_parameters_, &fs->parameters_->expr_, sizeof(*fs->parameters_));

    if (!constr_expr) {
      /* No memory. */
      dx_no_memory(dx);
      return NULL;
    }

    /* Copy over the swizzle */
    size_t n;
    assert(sizeof(swizzle) == sizeof(constr_expr->swizzle_));
    for (n = 0; n < sizeof(swizzle) / sizeof(*swizzle); ++n) {
      constr_expr->swizzle_[n] = swizzle[n];
    }

    for (n = 0; n < fs->num_parameters_; ++n) {
      fs->parameters_[n].expr_ = NULL; /* Ownership transferred to function_expr */
    }

    return constr_expr;
  }
  else {
    /* Regular function call. */
    if (!fs->name_) {
      /* No name, assume this was an issue that's already reported. */
      return NULL;
    }
    struct sl_type **param_types = (struct sl_type **)malloc(fs->num_parameters_ * sizeof(struct sl_type *));
    if (!param_types) {
      dx_no_memory(dx);
      return NULL;
    }
    size_t n;

    for (n = 0; n < fs->num_parameters_; ++n) {
      /* XXX: Find some way of validating the expression */
      param_types[n] = NULL;
    }

    for (n = 0; n < fs->num_parameters_; ++n) {
      if (!fs->parameters_[n].expr_) {
        /* No expression, assume this was an issue that's already reported. */
        free(param_types);
        return NULL;
      }
      param_types[n] = sl_expr_type(tb, fs->parameters_[n].expr_);

      if (!param_types[n]) {
        /* No type, assume this was an issue that's already reported. */
        free(param_types);
        return NULL;
      }
    }

    struct sym_table *st_found = NULL;
    struct sym *sym_found = NULL;
    struct sl_function *func_found = NULL;
    sl_function_call_search(st, fs->name_, fs->num_parameters_, param_types, &st_found, &sym_found, &func_found);

    if (!func_found) {
      /* Function not found, but symbol found. */
      if (sym_found && sym_found->kind_ == SK_FUNCTION) {
        /* Symbol is a function, but not a match. */
        dx_error_loc(dx, &fs->loc_, "Function \"%s\" is not a match for the parameter types, see function at line: %d", fs->name_, situs_line(&fs->loc_));
        free(param_types);
        return NULL;
      }
      else if (sym_found) {
        /* Symbol is not a function. */
        dx_error_loc(dx, &fs->loc_, "Symbol \"%s\" is not a function", fs->name_);
        free(param_types);
        return NULL;
      }
      else {
        /* Symbol not found. */
        dx_error_loc(dx, &fs->loc_, "Function \"%s\" not found", fs->name_);
        free(param_types);
        return NULL;
      }
    }

    struct sl_expr *function_expr = sl_expr_alloc_function_call(func_found, &fs->loc_, &fs->parameters_->expr_, sizeof(*fs->parameters_));

    if (!function_expr) {
      /* No memory. */
      dx_no_memory(dx);
      free(param_types);
      return NULL;
    }

    for (n = 0; n < func_found->num_parameters_; ++n) {
      fs->parameters_[n].expr_ = NULL; /* Ownership transferred to function_expr */
    }

    free(param_types);

    return function_expr;
  }
}
