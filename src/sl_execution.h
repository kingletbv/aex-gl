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
  SLEPK_STMT,
  SLEPK_EXPR
} sl_execution_point_kind_t;

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
   * the condition and try again.) */
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

  /* Offset, in bytes, from sl_execution::sl_execution_points_, to the chain where the
   * executions are to join after this execution_point (and any siblings in the case
   * of statements) have completed execution.
   * This is a size_t offset because execution_points_ might dynamically reallocate. 
   * The continue_chain_ptr_ should (for obvious reasons) always point up the stack from
   * the current execution_point, never down the stack.
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
  int **int_regs_;

  size_t num_bool_regs_;
  unsigned char **bool_regs_;

  size_t num_sampler_2D_regs_;
  void **sampler_2D_regs_;

  size_t num_sampler_cube_regs_;
  void **sampler_cube_regs_;
};

void sl_exec_init(struct sl_execution *exec);
void sl_exec_cleanup(struct sl_execution *exec);

int sl_exec_prep(struct sl_execution *exec, struct sl_reg_allocator *rac);

int sl_exec_run(struct sl_execution *exec);

uint32_t sl_exec_join_chains(struct sl_execution *exec, uint32_t a, uint32_t b);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_EXECUTION_H */
