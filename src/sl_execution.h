#ifndef SL_EXECUTION_H
#define SL_EXECUTION_H

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef LIMITS_H_INCLUDED
#define LIMITS_H_INCLUDED
#include <limits.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SL_EXEC_CHAIN_MAX_NUM_ROWS 256

#define SL_EXEC_NO_CHAIN UINT32_MAX

/* pass in an expr and return the reg_alloc representing its r-value; whether the r-value is separate from the base_reg_
 * depends on whether it has an offset, or whether whether the base_reg_ is indirect, or whether context requires working
 * on a separate copy (e.g. assignment requires the rvalue to be distinct from the lvalue, in particular for multi-component
 * values being swizzled.)
 * Capture all these cases by checking whether the rvalue_ has a non-void kind. */
#define EXPR_RVALUE(expr) (((expr)->rvalue_.kind_ != slrak_void) ? (&(expr)->rvalue_) : (&(expr)->base_regs_))

/* pass in reg_alloc_ptr, a pointer to the sl_reg_alloc to load the register pointer for, and element_index, an index of the element/or component (e.g. the XYZ for a vec3, 0 for a scalar).
 * returns a pointer to the corresponding register for XXX_REG_PTR, and the index of the actual register (after frame corrections) for XXX_REG_INDEX */
#define FLOAT_REG_INDEX(expr, element_index) (EXPR_RVALUE(expr)->local_frame_ ? exec->execution_frames_[exec->num_execution_frames_-1].local_float_offset_ + EXPR_RVALUE(expr)->v_.regs_[element_index] : EXPR_RVALUE(expr)->v_.regs_[element_index])
#define INT_REG_INDEX(expr, element_index) (EXPR_RVALUE(expr)->local_frame_ ? exec->execution_frames_[exec->num_execution_frames_-1].local_int_offset_ + EXPR_RVALUE(expr)->v_.regs_[element_index] : EXPR_RVALUE(expr)->v_.regs_[element_index])
#define BOOL_REG_INDEX(expr, element_index) (EXPR_RVALUE(expr)->local_frame_ ? exec->execution_frames_[exec->num_execution_frames_-1].local_bool_offset_ + EXPR_RVALUE(expr)->v_.regs_[element_index] : EXPR_RVALUE(expr)->v_.regs_[element_index])
#define SAMPLER_2D_REG_INDEX(expr, element_index) (EXPR_RVALUE(expr)->local_frame_ ? exec->execution_frames_[exec->num_execution_frames_-1].local_sampler2D_offset_ + EXPR_RVALUE(expr)->v_.regs_[element_index] : EXPR_RVALUE(expr)->v_.regs_[element_index])
#define SAMPLER_CUBE_REG_INDEX(expr, element_index) (EXPR_RVALUE(expr)->local_frame_ ? exec->execution_frames_[exec->num_execution_frames_-1].local_samplerCube_offset_ + EXPR_RVALUE(expr)->v_.regs_[element_index] : EXPR_RVALUE(expr)->v_.regs_[element_index])

#define FLOAT_REG_PTR(expr, element_index) exec->float_regs_[FLOAT_REG_INDEX(expr, element_index)]
#define INT_REG_PTR(expr, element_index) exec->int_regs_[INT_REG_INDEX(expr, element_index)]
#define BOOL_REG_PTR(expr, element_index) exec->bool_regs_[BOOL_REG_INDEX(expr, element_index)]
#define SAMPLER_2D_REG_PTR(expr, element_index) exec->sampler_2D_regs_[SAMPLER_2D_REG_INDEX(expr, element_index)]
#define SAMPLER_CUBE_REG_PTR(expr, element_index) exec->sampler_cube_regs_[SAMPLER_CUBE_REG_INDEX(expr, element_index)]

/* Same as above, but ignoring offset and rvalue; use these for manually working with the offsets and rvalues */
#define FLOAT_REG_INDEX_NRV(reg_alloc_ptr, element_index) ((reg_alloc_ptr)->local_frame_ ? exec->execution_frames_[exec->num_execution_frames_-1].local_float_offset_ + (reg_alloc_ptr)->v_.regs_[element_index] : (reg_alloc_ptr)->v_.regs_[element_index])
#define INT_REG_INDEX_NRV(reg_alloc_ptr, element_index) ((reg_alloc_ptr)->local_frame_ ? exec->execution_frames_[exec->num_execution_frames_-1].local_int_offset_ + (reg_alloc_ptr)->v_.regs_[element_index] : (reg_alloc_ptr)->v_.regs_[element_index])
#define BOOL_REG_INDEX_NRV(reg_alloc_ptr, element_index) ((reg_alloc_ptr)->local_frame_ ? exec->execution_frames_[exec->num_execution_frames_-1].local_bool_offset_ + (reg_alloc_ptr)->v_.regs_[element_index] : (reg_alloc_ptr)->v_.regs_[element_index])
#define SAMPLER_2D_REG_INDEX_NRV(reg_alloc_ptr, element_index) ((reg_alloc_ptr)->local_frame_ ? exec->execution_frames_[exec->num_execution_frames_-1].local_sampler2D_offset_ + (reg_alloc_ptr)->v_.regs_[element_index] : (reg_alloc_ptr)->v_.regs_[element_index])
#define SAMPLER_CUBE_REG_INDEX_NRV(reg_alloc_ptr, element_index) ((reg_alloc_ptr)->local_frame_ ? exec->execution_frames_[exec->num_execution_frames_-1].local_samplerCube_offset_ + (reg_alloc_ptr)->v_.regs_[element_index] : (reg_alloc_ptr)->v_.regs_[element_index])

#define FLOAT_REG_PTR_NRV(reg_alloc_ptr, element_index) exec->float_regs_[FLOAT_REG_INDEX_NRV(reg_alloc_ptr, element_index)]
#define INT_REG_PTR_NRV(reg_alloc_ptr, element_index) exec->int_regs_[INT_REG_INDEX_NRV(reg_alloc_ptr, element_index)]
#define BOOL_REG_PTR_NRV(reg_alloc_ptr, element_index) exec->bool_regs_[BOOL_REG_INDEX_NRV(reg_alloc_ptr, element_index)]
#define SAMPLER_2D_REG_PTR_NRV(reg_alloc_ptr, element_index) exec->sampler_2D_regs_[SAMPLER_2D_REG_INDEX_NRV(reg_alloc_ptr, element_index)]
#define SAMPLER_CUBE_REG_PTR_NRV(reg_alloc_ptr, element_index) exec->sampler_cube_regs_[SAMPLER_CUBE_REG_INDEX_NRV(reg_alloc_ptr, element_index)]

struct sl_stmt;
struct sl_expr;
struct sl_reg_allocator;
struct ir_temp;

typedef enum sl_execution_point_kind {
  SLEPK_NONE,
  SLEPK_BOOTSTRAP,
  SLEPK_STMT,
  SLEPK_EXPR
} sl_execution_point_kind_t;

struct sl_execution_frame {
  /* Function for which this frame is built */
  struct sl_function *f_;
  /* Base offsets for the local frame of this execution point; each function invocation
   * gets its own register space sufficient for storing all locals and temps, at this offset */
  int local_float_offset_;
  int local_int_offset_;
  int local_bool_offset_;
  int local_sampler2D_offset_;
  int local_samplerCube_offset_;

  /* Chains to join to for various contexts during conversion to SL IR */
  struct ir_temp *return_chain_;
  struct ir_temp *continue_chain_;
  struct ir_temp *break_chain_;
};

struct sl_execution_point {
  sl_execution_point_kind_t kind_;

  /* index of the first row in the enter chain, or SL_EXEC_NO_CHAIN if there is no
   * chain.
   * The enter chain represents entry into a statement or expression; e.g. for
   * a "for" loop, this would be prior to the execution of the initializer statement
   * of the for statement.
   */
  uint32_t enter_chain_; 

  /* index of the first row in the revisit chain, or SL_EXEC_NO_CHAIN if there is no
   * chain.
   * The revisit chain represents a return to the statement from a child, for instance,
   * for a "while" loop statement, the revisit chain are the executions returning following 
   * the child statement's executions. (And in the case of the while loop, we would test
   * the condition and try again.)
   * For expressions, this is the chain after the children have executed, but before the 
   * operator of the expression node itself has executed. */
  uint32_t revisit_chain_;

  /* index of the first row in the post chain, or SL_EXEC_NO_CHAIN if there is no
   * chain.
   * The post chain typically represents the children that have completed execution and
   * are "done" with this statement (for instance, children having completed either true
   * or false branches re-joining on the post-chain) - once all executions have completed
   * onto the post chain (e.g. no enter chain and no revisit chains), the execution point
   * is popped and the next statement (if any) is pushed. If there is no next statement,
   * the post_chain is merged onto the continue_chain (as pointed to by continue_chain_ptr_). 
   */
  uint32_t post_chain_;

  /* Index of the first row in the alternate chain, or SL_EXEC_NO_CHAIN.
   * This is currently used for exop_conditional, where the revisit_chain is used for
   * handling execution after the conditional has been evaluated, post_chain is used for
   * handling execution after the true branch has evaluated, and we need one more for
   * handling the false branch's evaluation. */
  uint32_t alt_chain_;

  /* Offset, in bytes, from sl_execution::sl_execution_points_, to the chain where the
   * executions are to join after this execution_point (and any siblings in the case
   * of statements) have completed execution.
   * This is a size_t offset because execution_points_ might dynamically reallocate. 
   * The continue_chain_ptr_ should (for obvious reasons) always point up the stack from
   * the current execution_point, never down the stack.
   * Note that this points to a chain, therefore, a uint32_t, not an sl_execution_point,
   * as it has multiple of these.
   */
  size_t continue_chain_ptr_;

  union {
    struct sl_expr *expr_;
    struct sl_stmt *stmt_;
    struct sl_function *f_;
  } v_;
};

struct sl_execution {
  /* Compilation unit being executed */
  struct sl_compilation_unit *cu_;

  /* Maximum number of rows the execution has been allocated for */
  size_t max_num_rows_;

  /* Dump text, used by the debug shader dump() function to write output */
  size_t dump_text_len_;
  char *dump_text_;

  /* Slab allocation for registers */
  void *slab_;

  /* Stack of all active execution points, the current execution point to be
   * executed is at the top of the stack. */
  size_t num_execution_points_;
  size_t num_execution_points_allocated_;
  struct sl_execution_point *execution_points_;

  /* Stack of all execution frames, each frame corresponds to a single function
   * invocation, starting with the initial invocation at 0. */
  size_t num_execution_frames_;
  size_t num_execution_frames_allocated_;
  struct sl_execution_frame *execution_frames_;

  /* Pointer to the execution chain column of data; the execution chain value 
   * is the number of rows to the next row in the same execution chain, or 0
   * as the sentinel value if this is the last row. Typically 1 if the chain
   * includes the next row down. Note that values may never be negative, 
   * consequently, chains are always in sorted order of their row. */
  uint8_t *exec_chain_reg_;

  /* Pointers to each of the registers (columns) of data, by scalar type (more
   * complex types are split out over these, e.g. a vec2 is two float registers) */
  size_t num_float_regs_;
  float **float_regs_;

  size_t num_int_regs_;
  int64_t **int_regs_;

  size_t num_bool_regs_;
  unsigned char **bool_regs_;

  size_t num_sampler_2D_regs_;
  void ***sampler_2D_regs_;

  size_t num_sampler_cube_regs_;
  void ***sampler_cube_regs_;

  /* Temporary runtime register values used by texture mappers */
  float sampler_2d_projected_s_[SL_EXEC_CHAIN_MAX_NUM_ROWS];
  float sampler_2d_projected_t_[SL_EXEC_CHAIN_MAX_NUM_ROWS];
};

struct sl_exec_call_graph_results {
  size_t num_execution_frames_;
  size_t num_float_regs_;
  size_t num_int_regs_;
  size_t num_bool_regs_;
  size_t num_sampler2D_regs_;
  size_t num_samplerCube_regs_;
};


void sl_exec_init(struct sl_execution *exec);
void sl_exec_cleanup(struct sl_execution *exec);

int sl_exec_prep(struct sl_execution *exec, struct sl_compilation_unit *cu);

int sl_exec_allocate_registers_by_slab(struct sl_execution *exec, size_t max_num_rows);

int sl_exec_run(struct sl_execution *exec, struct sl_function *f, int exec_chain);

uint32_t sl_exec_join_chains(struct sl_execution *exec, uint32_t a, uint32_t b);

void sl_exec_call_graph_results_init(struct sl_exec_call_graph_results *cgr);
void sl_exec_call_graph_results_cleanup(struct sl_exec_call_graph_results *cgr);
int sl_exec_call_graph_analysis(struct sl_exec_call_graph_results *cgr, struct sl_function *f);
void sl_exec_cgr_max(struct sl_exec_call_graph_results *cgr, const struct sl_exec_call_graph_results *lcgr);
void sl_exec_cgr_swap(struct sl_exec_call_graph_results *a, struct sl_exec_call_graph_results *b);

/* Dump the value in the registers determined by "ra" to the output string "output_str" and return the
 * length of that string, excluding NULL terminators. If output_str is NULL, the length is still computed
 * and returned but no output is written. single_row specifies the single row whose output for ra is dumped.
 * Internally uses sl_exec_offset_dump_strided. */
size_t sl_exec_dump(struct sl_execution *exec, char *output_str, 
                    uint8_t single_row, const struct sl_reg_alloc *ra);

/* Dumps the value in the registered determined by the arr, index, index_stride and array_offset combinations.
 * Serializes this value in human readable form to output_str and returns the size of the string in bytes, 
 * excluding any NULL terminator (the function does not write a NULL terminator, caller has to terminate if
 * necessary.)
 * arr contains the base registers and the datatype; the base registers are typically the first element of
 * an array (hence "arr") if the value being dumped is part of an array, or just the reg_alloc if not part of
 * an array.
 * index is the offset, measured in index_stride times registers, from the first registers in arr, to the
 * register to be dumped. Note that index is, itself, an int reg_alloc and is stored in a register. If index
 * is NULL, or if it is of type slrak_void, then it is deemed a constant 0 (e.g. no offset from the arr
 * registers.)
 * index_stride is the number of registers between two consecutive entries of the same type in arr; it
 * helps address elements deep within nested arrays, but when not doing so, should be set to 1 (and 
 * definitely not 0).
 * array_offset is the offset from the base registers in arr, it is the static equivalent of the runtime
 * index field. When not dumping elements inside arrays, it should be set to 0.
 * For ease of use, the sl_exec_dump() function performs the same function for the common case of no
 * deep array introspection. */
static size_t sl_exec_offset_dump_strided(struct sl_execution *exec, 
                                          char *output_str,
                                          uint8_t single_row, 
                                          const struct sl_reg_alloc *arr,
                                          const struct sl_reg_alloc *index,
                                          int index_stride,
                                          int array_offset);

/* Convenience wrapper around sl_exec_offset_dump_strided that writes to FILE pointer fp instead
 * of memory. */
void sl_exec_offset_dumpf_strided(struct sl_execution *exec, 
                                  FILE *fp,
                                  uint8_t single_row, 
                                  const struct sl_reg_alloc *arr,
                                  const struct sl_reg_alloc *index,
                                  int index_stride,
                                  int array_offset);

/* Convenience wrapper analogous to sl_exec_dump which sends its output to fp */
void sl_exec_dumpf(struct sl_execution *exec, FILE *fp,
                    uint8_t single_row, const struct sl_reg_alloc *ra);

void sl_exec_debug_dump_builtin(struct sl_execution *exec, int exec_chain, struct sl_expr *x);
  

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_EXECUTION_H */
