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
};

void sl_exec_init(struct sl_execution *exec);
void sl_exec_cleanup(struct sl_execution *exec);

int sl_exec_run(struct sl_execution *exec);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_EXECUTION_H */
