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

  /* The result of a sub-expression may be an lvalue, meaning we need to keep track
   * of the actual registers to modify when we with to assign, increment or decrement
   * it. This can be accomplished with a straightforward copying of a subset of register 
   * allocations from a sub-expression's child to itself, for all operations that return
   * an lvalue.... All operations except array subscription that is. For an array
   * subscription, the register to be returned is determined at runtime. The following
   * fields allow these registers-at-an-offset-of-a-base-register to be modeled at
   * runtime and at allocation time.
   * base_regs_: the base register holding the value of the sl_expr (which may be
   *             a compound, like a struct, hence plural, base_regS_). If no arrays
   *             are involved, base_regs_ holds the registers holding the value of
   *             the sub-expression after evaluation.
   * offset_reg_: if offset_reg_.kind_ != slrak_void, it holds a slrak_int register
   *              that, after evaluation of the sub-expression, contains the offset
   *              from the base register to the register holding the value of the
   *              sub-expression after evaluation. This is used to describe the 
   *              lvalue following an array subscript operation. For instance, for
   *              the array "float x[5];", the expression "x[3]" would have the
   *              base_regs_ pointing to the register for "x[0]" and have the
   *              offset_reg_ pointing to a register that, after evaluation of the
   *              sub-expression, holds the value "3" (for an offset of 3 from the
   *              base register at 0.) This gets more complicated with nested arrays,
   *              for instance, "float y[5][7]" (an array of 5 arrays containing 7
   *              elements), y[5] has base_regs_ pointing to y[0][0] and an offset
   *              of 5 - this reflects that it is the fifth element from the base.
   *              However, that fifth element will still be multiplied by the size
   *              of the element, e.g. y[5][0] has base_regs_ pointing to y[0][0]
   *              and an offset of 35 (the element is now a float, and no longer
   *              an array of 7 floats.)
   * offset_limit_: because the offset_reg_ holds the index as a runtime value, we
   *                don't know what that value might be at compile time. 
   *                Consequently, for register allocation, locking, and release, 
   *                purposes, we need a hint as to what the total range of registers
   *                starting from the base registers might be. offset_limit_ offers
   *                that range, measured in the element size of the type of the
   *                sub-expression. At runtime, when all register allocations have
   *                been performed and are set in stone, this field has no purpose.
   *                If no arrays are involved, this value is "1".
   * rvalue_: most, if not almost all, operations work on one or more input registers
   *          and output into an output register. Very few can work on a register 
   *          specified as a base register and an offset number of registers to count
   *          from that base register. Often, we will need to load from a base 
   *          register at an offset, into another, temporary, register, prior to 
   *          completing the operation. That temporary register is allocated in the
   *          rvalue_ field. (rvalue = right-value, lvalue = left-value, corresponding
   *          to the sides of an assignment.)
   *          If offset_reg_.kind_ == slrak_void (the default) then no rvalue_ is 
   *          needed as the base_regs_ can be used directly (you could say the 
   *          offset_reg_.kind_ of slrak_void corresponds to the value 0.)
   *          If, however, there is an offset_reg_, then, depending on how the sub-
   *          expression's child is used, an rvalue_ may need to be allocated.
   *          This is determined during allocation. After allocation has
   *          completed, it suffices to check (rvalue_.kind_ != slrak_void) to see
   *          if an rvalue was allocated, and to use base_regs_ otherwise.
   */
  struct sl_reg_alloc base_regs_;
  struct sl_reg_alloc offset_reg_;
  int64_t offset_limit_;
  struct sl_reg_alloc rvalue_;

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
  size_t field_index_;

  /* exop_function_call - the function called, and sibling exop_function_call
   * expressions calling the same function (cyclic). */
  struct sl_function *function_;
  struct sl_expr *next_caller_, *prev_caller_;

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
struct sl_expr *sl_expr_alloc_field_selection(struct sl_expr *x, struct sl_type_field *field, size_t field_index, const struct situs *field_loc);

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
 * existing allocations made for some of the nodes and/or variables. All sub-expressions are
 * allocated locally. The ract allocator is used as the allocator for the local frame.
 * If needs_rvalue is non-zero, then the expression x itself will have its rvalue_ member
 * initialized to an allocated value if, and only if, ract->base_regs_ corresponds to an
 * offseted array (that is to say, it's an lvalue with an offset_reg_.kind_ != slrak_void, 
 * which means it's not a known fixed register, but the register is determined at runtime) -
 * use this when the code using x needs to access or copy out of a fixed register.
 * Make sure any existing allocations are locked to prevent inappropriate double-use.
 * Returns 0 upon success, non-zero should be treated as a fatal error (typically memory, overflow
 * or an internal error.)
 */
int sl_expr_alloc_registers(struct sl_type_base *tb, struct sl_reg_allocator *ract, struct sl_expr *x, int needs_rvalue);

/* Attached the expression to the function, setting function_. Detaches from any old function. */
void sl_expr_attach_caller(struct sl_expr *x, struct sl_function *f);

/* Detaches the expression (presumably an exop_function_call) from the function it calls, and clears
 * sl_expr::function_; If this is not an exop_function_call, or if function_ is currently set to NULL,
 * this does nothing. */
void sl_expr_detach_caller(struct sl_expr *x);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_EXPR_H */
