#ifndef SL_EXECUTION_H
#define SL_EXECUTION_H

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

#define SL_EXEC_NO_CHAIN UINT32_MAX

struct sl_stmt;
struct sl_expr;
struct sl_reg_allocator;

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
  } v_;
};

struct sl_execution {
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
  void **sampler_2D_regs_;

  size_t num_sampler_cube_regs_;
  void **sampler_cube_regs_;
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

int sl_exec_prep(struct sl_execution *exec, struct sl_compilation_unit *cu, struct sl_function *f);

int sl_exec_run(struct sl_execution *exec);

uint32_t sl_exec_join_chains(struct sl_execution *exec, uint32_t a, uint32_t b);

void sl_exec_call_graph_results_init(struct sl_exec_call_graph_results *cgr);
void sl_exec_call_graph_results_cleanup(struct sl_exec_call_graph_results *cgr);
int sl_exec_call_graph_analysis(struct sl_exec_call_graph_results *cgr, struct sl_function *f);
void sl_exec_cgr_max(struct sl_exec_call_graph_results *cgr, const struct sl_exec_call_graph_results *lcgr);
void sl_exec_cgr_swap(struct sl_exec_call_graph_results *a, struct sl_exec_call_graph_results *b);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_EXECUTION_H */
