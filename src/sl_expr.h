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

#ifndef SL_EXPR_H
#define SL_EXPR_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef SITUS_H_INCLUDED
#define SITUS_H_INCLUDED
#include "pp/situs.h"
#endif

#ifndef SL_EXPR_TEMP_H_INCLUDED
#define SL_EXPR_TEMP_H_INCLUDED
#include "sl_expr_temp.h"
#endif

#ifndef SL_FRAME_H_INCLUDED
#define SL_FRAME_H_INCLUDED
#include "sl_frame.h"
#endif

#ifndef SL_TYPES_H_INCLUDED
#define SL_TYPES_H_INCLUDED
#include "sl_types.h"
#endif

#ifndef SL_REG_ALLOC_H_INCLUDED
#define SL_REG_ALLOC_H_INCLUDED
#include "sl_reg_alloc.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Flags returned by sl_expr_validate() */

/* The expression is not valid and should not be evaluated */
#define SLXV_INVALID 1

/* The expression is not a compile time constant (and cannot be used in array declarations etc.) */
#define SLXV_NOT_CONSTANT 2

typedef enum expr_op {
  exop_invalid,

  exop_variable,
  exop_literal,
  exop_array_subscript,
  exop_component_selection, /* e.g. myvec3.xxz */
  exop_field_selection,     /* e.g. mystruct.myfield */
  exop_post_inc,
  exop_post_dec,
  exop_pre_inc,
  exop_pre_dec,

  exop_negate,
  exop_logical_not,

  exop_multiply,
  exop_divide,

  exop_add,
  exop_subtract,

  exop_lt,
  exop_le,
  exop_ge,
  exop_gt,

  exop_eq,
  exop_ne,

  exop_function_call,
  exop_constructor,

  exop_logical_and,
  exop_logical_or,
  exop_logical_xor,

  exop_assign,
  exop_mul_assign,
  exop_div_assign,
  exop_add_assign,
  exop_sub_assign,
        
  exop_sequence,    // comma operator

  exop_conditional  // ternary ?: operator
} expr_op_t;

typedef enum sl_component_conversion {
  slcc_invalid,
  slcc_float_to_float,
  slcc_float_to_int,
  slcc_float_to_bool,
  slcc_int_to_float,
  slcc_int_to_int,
  slcc_int_to_bool,
  slcc_bool_to_float,
  slcc_bool_to_int,
  slcc_bool_to_bool
} sl_component_conversion_t;

struct sl_component_selection {
  /* Determines the conversion to be applied to the component from
   * its source to its destination. */
  sl_component_conversion_t conversion_;

  /* Index of the parameter to select this component from
   * If -1, then this component is to be a constant value. */
  int8_t parameter_index_;

  /* Index of the component to select from the parameter.
   * If the parameter_index_ is -1, then the component_index_
   * intends to select a constant value.
   * 0: Select 0., 0, or false, for int, float and bool respectively. 
   * 1: Select 1., 1, or true, for int, float and bool respectively.
   */
  uint8_t component_index_;
};

struct sl_expr {
  expr_op_t op_;
  struct situs op_loc_;

  size_t num_children_;
  struct sl_expr **children_;

  /* exop_literal */
  struct sl_expr_temp literal_value_;

  /* register allocation, applicable for non-constant values. */
  struct sl_reg_alloc reg_alloc_;
  
  /* Runtime helper during register allocation */
  int was_prelocked_:1;

  /* exop_component_selection; each component_selection_[] contains
   * the ASCII letter of the component (e.g. x,y,z,w,r,g,b,a,s,t,p or q) */
  int num_components_;
  char component_selection_[4];

  /* expr_constructor
   * exop_component_selection
   * For each target component, specifies the origin as an index into
   * children_ (parameter_index_) and the component of the datatype (0
   * for scalars, vectors like you'd expect (xyzw) and column-major for
   * matrices.)
   * Note that this can be computed from the children_ and the 
   * constructor_type_*/
  struct sl_component_selection swizzle_[16];

  /* exop_field_selection */
  struct sl_type_field *field_selection_;

  /* exop_function_call - the function called */
  struct sl_function *function_;

  /* exop_constructor, the type constructed */
  struct sl_type *constructor_type_;

  /* exop_variable, the variable */
  struct sl_variable *variable_;
};

void sl_expr_init(struct sl_expr *x);
void sl_expr_cleanup(struct sl_expr *x);
void sl_expr_free(struct sl_expr *x);

struct sl_type *sl_expr_type(struct sl_type_base *tb, struct sl_expr *x);

struct sl_expr *sl_expr_alloc_float_lit(float f, const struct situs *loc);
struct sl_expr *sl_expr_alloc_int_lit(int64_t i, const struct situs *loc);
struct sl_expr *sl_expr_alloc_bool_lit(int b, const struct situs *loc);
struct sl_expr *sl_expr_alloc_function_call(struct sl_function *f, const struct situs *loc, struct sl_expr **pexpr, size_t pexpr_stride);
struct sl_expr *sl_expr_alloc_constructor(struct sl_type *t, const struct situs *loc, size_t num_params, struct sl_expr **pexpr, size_t pexpr_stride);
struct sl_expr *sl_expr_alloc_variable(struct sl_variable *v, const struct situs *loc);
struct sl_expr *sl_expr_alloc_swizzle_selection(struct sl_expr *x, size_t num_tgt_components, uint8_t *component_selection, const struct situs *field_loc);
struct sl_expr *sl_expr_alloc_field_selection(struct sl_expr *x, struct sl_type_field *field, const struct situs *field_loc);

/* Allocate a unary operator expression, op and loc are operator and situs location of the
 * expression. The opd is a pointer to the pointer of the operand. If the function succeeds,
 * the new expression (referencing *opd) is returned, and *opd is set to NULL, thus helping
 * ensure only one reference to the operand exists for cleanup purposes. */
struct sl_expr *sl_expr_alloc_unop(expr_op_t op,const struct situs *loc, struct sl_expr **opd);

/* Allocate a binary operator expression, op and loc are the operator and location respectively, the
 * lhs and rhs are pointers to the pointers of the left and right hand side expressions. If the function
 * succeeds, and the left and right hand side of the binop are now referenced by the returned expression, then
 * *lhs and *rhs are set to NULL, thus making it easier to avoid double-cleanup. */
struct sl_expr *sl_expr_alloc_binop(expr_op_t op,const struct situs *loc,struct sl_expr **lhs, struct sl_expr **rhs);

/* like sl_expr_alloc_binop, but for operators with 3 operands. */
struct sl_expr *sl_expr_alloc_ternop(expr_op_t op,const struct situs *loc,struct sl_expr **opd0, struct sl_expr **opd1, struct sl_expr **opd2);

/* Returns non-zero if this is an lvalue expression, zero otherwise. */
int sl_expr_is_lvalue(struct sl_type_base *tb, const struct sl_expr *x);

/* Returns a combination of SLXV_xxx flags, determining if the expression is valid and
 * if the expression is a compile-time constant. Will issue diagnostics if issues are
 * found with the expression that were, ostensibly, not diagnosed before.. */
int sl_expr_validate(struct diags *dx, struct sl_type_base *tb, const struct sl_expr *x);

/* Evaluate an expression and store the result in the given temporary. */
int sl_expr_eval(struct sl_type_base *tb, const struct sl_expr *x, struct sl_expr_temp *r);

/* Allocate temp registers for all expression nodes in the expression x, while respecting 
 * existing allocations made for some of the nodes and/or variables.
 * Make sure any existing allocations are locked to prevent inappropriate double-use.
 * Returns 0 upon success, non-zero should be treated as a fatal error (typically memory, overflow
 * or an internal error.)
 */
int sl_expr_alloc_registers(struct sl_type_base *tb, struct sl_reg_allocator *ract, struct sl_expr *x);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_EXPR_H */
