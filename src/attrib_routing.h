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
#ifndef ATTRIB_ROUTING_H
#define ATTRIB_ROUTING_H

#ifndef STDDEF_H_INCLUDED
#define STDDEF_H_INCLUDED
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct attrib_route {
  /* Each route is from 1 specific float register in the source (vertex) shader,
   * to 1 specific float register in the target (fragment) shader.
   */
  int from_source_reg_;
  int to_target_reg_;
};

struct attrib_routing {
  /* Attributes routed in this routing. These are typically varyings and exclude
   * XYZW, which are routed manually between vertex and fragment shaders */
  size_t num_attribs_routed_;
  size_t num_attribs_routed_allocated_;
  struct attrib_route *attribs_routed_;
};

void attrib_routing_init(struct attrib_routing *ar);
void attrib_routing_cleanup(struct attrib_routing *ar);
int attrib_routing_add_variables(struct attrib_routing *ar, struct sl_variable *target, struct sl_variable *source);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ATTRIB_ROUTING_H */
