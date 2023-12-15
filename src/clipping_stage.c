/* Copyright 2023 Kinglet B.V.
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

#ifndef CLIPPING_STAGE_H_INCLUDED
#define CLIPPING_STAGE_H_INCLUDED
#include "clipping_stage.h"
#endif

void clipping_stage_init(struct clipping_stage *cs) {
  cs->num_varyings_ = 0;
  cs->input_varyings_ = NULL;
  cs->num_triangles_in_a_ = 0;
  cs->triangle_varyings_a_ = NULL;
  cs->num_triangles_in_b_ = 0;
  cs->triangle_varyings_b_ = NULL;
}


int clipping_stage_alloc_varyings(struct clipping_stage *cs, size_t num_extra_varyings) {
  /* num_varyings_ = XYZW + additionals */
  cs->num_varyings_ = 4 + num_extra_varyings;

  size_t vertex_size = cs->num_varyings_ * sizeof(float);
  size_t triangle_size = vertex_size * 3;

  if (cs->input_varyings_) free(cs->input_varyings_);
  cs->input_varyings_ = (float *)malloc(triangle_size);
  if (!cs->input_varyings_) return -1;

  size_t buf_size = triangle_size * CLIPPING_STAGE_MAX_NUM_TRIANGLES;
  if (cs->triangle_varyings_a_) free(cs->triangle_varyings_a_);
  cs->triangle_varyings_a_ = (float *)malloc(buf_size);
  if (!cs->triangle_varyings_a_) return -1;

  if (cs->triangle_varyings_b_) free(cs->triangle_varyings_b_);
  cs->triangle_varyings_b_ = (float *)malloc(buf_size);
  if (!cs->triangle_varyings_b_) return -1;

  return 0;
}

void clipping_stage_cleanup(struct clipping_stage *cs) {
  if (cs->input_varyings_) free(cs->input_varyings_);
  if (cs->triangle_varyings_a_) free(cs->triangle_varyings_a_);
  if (cs->triangle_varyings_b_) free(cs->triangle_varyings_b_);
}

int clipping_stage_process_triangle(struct clipping_stage *cs) {
  // ... clipping magic here
}

