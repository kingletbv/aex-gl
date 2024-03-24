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

#ifndef SL_COMPILATION_UNIT_H_INCLUDED
#define SL_COMPILATION_UNIT_H_INCLUDED
#include "sl_compilation_unit.h"
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

void sl_compilation_unit_init(struct sl_compilation_unit *cu) {
  st_init(&cu->global_scope_, NULL);
  sl_frame_init(&cu->global_frame_);
  sl_exec_call_graph_results_init(&cu->register_counts_);
}

void sl_compilation_unit_cleanup(struct sl_compilation_unit *cu) {
  sl_exec_call_graph_results_cleanup(&cu->register_counts_);
  sl_frame_cleanup(&cu->global_frame_);
  st_cleanup(&cu->global_scope_);
}

struct sl_function *sl_compilation_unit_find_function(struct sl_compilation_unit *cu, const char *name) {
  struct sym *s;
  s = st_find(&cu->global_scope_, name, strlen(name));
  if (s && (s->kind_ == SK_FUNCTION)) return s->v_.function_;
  return NULL;
}
