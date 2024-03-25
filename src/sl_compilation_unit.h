/* Copyright 2023-2024 Kinglet B.V.
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

#ifndef SL_COMPILATION_UNIT_H
#define SL_COMPILATION_UNIT_H

#ifndef SYM_TABLE_H_INCLUDED
#define SYM_TABLE_H_INCLUDED
#include "sym_table.h"
#endif

#ifndef SL_FRAME_H_INCLUDED
#define SL_FRAME_H_INCLUDED
#include "sl_frame.h"
#endif

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct sl_compilation_unit {

  /* The symbol table representing the global scope. */
  struct sym_table global_scope_;

  /* The global frame (containing all function definitions and global variables) */
  struct sl_frame global_frame_;

  /* The number of registers needed to execute any of the functions. */
  struct sl_exec_call_graph_results register_counts_;

};

void sl_compilation_unit_init(struct sl_compilation_unit *cu);
void sl_compilation_unit_cleanup(struct sl_compilation_unit *cu);

struct sl_function *sl_compilation_unit_find_function(struct sl_compilation_unit *cu, const char *name);
struct sl_variable *sl_compilation_unit_find_variable(struct sl_compilation_unit *cu, const char *name);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_COMPILATION_UNIT_H */
