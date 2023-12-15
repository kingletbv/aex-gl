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

#ifndef CLIPPING_STAGE_H
#define CLIPPING_STAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/* We clip one triangle at a time, against each of the 6 planes (near, far, left, top, right, bottom),
 * worst case, a triangle, when clipped against a plane, may result in 2 triangles, so we need to have
 * room for 2**6 triangles. I think a deeper analysis may provide a tighter guarantee, but this is the
 * safe number we'll run with for now. */
#define CLIPPING_STAGE_MAX_NUM_TRIANGLES 64

/* Indices of the "known" and always present varyings. */
#define CLIPPING_STAGE_IDX_X 0
#define CLIPPING_STAGE_IDX_Y 1
#define CLIPPING_STAGE_IDX_Z 2
#define CLIPPING_STAGE_IDX_W 3

struct clipping_stage {
  /* Number of varying attributes, the first four are:
   * x, y, z, w. 
   * but there are likely more defined by shaders. All varyings are float for now.
   */
  size_t num_varyings_;

  /* Input varyings, contains num_varyings_ floats */
  float *input_varyings_;

  /* Buffers for use during clipping. Each buffer has room for CLIPPING_STAGE_MAX_NUM_TRIANGLES.
   * Each triangle has 3 rows (one for each vertex), each row has num_varyings_ columns.
   * We maintain 2 buffers so we take a plane, clip all current triangles from one buffer to the other,
   * take the next plane, clip all triangles from the other buffer back into the one buffer, and so on,
   * until clipping has completed.
   */
  size_t num_triangles_in_a_;
  float *triangle_varyings_a_;
  size_t num_triangles_in_b_;
  float *triangle_varyings_b_;
};

/* First stage of initialization, this will always succeed and on return you can always call
 * clipping_stage_cleanup() safely. Prior to use, initialization must be completed by calling
 * clipping_stage_alloc_varyings() before processing triangles. */
void clipping_stage_init(struct clipping_stage *cs);

/* Allocates the internal buffers for num_extra_varyings *additional* varying attributes, on top
 * of (x, y, z, w). Must always be called after clipping_stage_init() as a part of initialization,
 * but can be called again later to change the number of varying attributes.
 * Returns 0 on success, -1 on failure. */
int clipping_stage_alloc_varyings(struct clipping_stage *cs, size_t num_extra_varyings);

void clipping_stage_cleanup(struct clipping_stage *cs);

/* Macro for direct access to the input varying values of the triangle.
 * varying_index is the index of the varying to be changed (0, 1, 2, 3 for X, Y, Z and W),
 * vertex_index is the index of the vertex of the triangle (0, 1 or 2). */
#define CLIPPING_STAGE_VARYING_INPUT(cs, varying_index, vertex_index) \
  ((cs)->input_varyings_[(vertex_index) * (cs)->num_varyings_ + (varying_index)])

/* Process the triangle currently set in the clipping_stage using the
 * CLIPPING_STAGE_VARYING_INPUT() macro.
 * Returns non-zero if clipping results in one or more triangles inside the view frustrum. */
int clipping_stage_process_triangle(struct clipping_stage *cs);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CLIPPING_STAGE_H */
