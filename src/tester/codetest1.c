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

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef CLIPPING_STAGE_H_INCLUDED
#define CLIPPING_STAGE_H_INCLUDED
#include "../clipping_stage.h"
#endif

#define DEBUG_OUTPUT 1

int codetest1(void) {
  int r = -1;
  struct clipping_stage cs;
  clipping_stage_init(&cs);

  r = clipping_stage_alloc_varyings(&cs, 0);
  if (r) {
    fprintf(stderr, "Failed, no memory\n");
    goto test_exit;
  }

  float *v0, *v1, *v2;
  v0 = cs.input_varyings_;
  v1 = v0 + cs.num_varyings_;
  v2 = v1 + cs.num_varyings_;

  v0[CLIPPING_STAGE_IDX_X] = 8.79229736f; v0[CLIPPING_STAGE_IDX_Y] = -8.09803772f; v0[CLIPPING_STAGE_IDX_Z] = 15.0757446f; v0[CLIPPING_STAGE_IDX_W] = 16.8507690f;
  v1[CLIPPING_STAGE_IDX_X] = 8.79229736f; v1[CLIPPING_STAGE_IDX_Y] = -5.44734192f; v1[CLIPPING_STAGE_IDX_Z] = 14.8800659f; v1[CLIPPING_STAGE_IDX_W] = 16.6553955f;
  v2[CLIPPING_STAGE_IDX_X] = 12.6775513f; v2[CLIPPING_STAGE_IDX_Y] = -8.87055969f; v2[CLIPPING_STAGE_IDX_Z] = 2.27770996f; v2[CLIPPING_STAGE_IDX_W] = 4.07275391f;

  size_t num_triangles;
  num_triangles = clipping_stage_process_triangle(&cs);

  size_t n;
  float *ov = cs.triangle_varyings_b_;
  for (n = 0; n < num_triangles; ++n) {
    fprintf(stdout, "## Triangle #%d\n", (int)n);
    size_t k;
    for (k = 0; k < 3; ++k) {
      fprintf(stdout, "#%d  %f %f %f %f\n", (int)k,
              ov[CLIPPING_STAGE_IDX_X], ov[CLIPPING_STAGE_IDX_Y], ov[CLIPPING_STAGE_IDX_Z], ov[CLIPPING_STAGE_IDX_W]);
      float x, y, z, w;
      x = ov[CLIPPING_STAGE_IDX_X];
      y = ov[CLIPPING_STAGE_IDX_Y];
      z = ov[CLIPPING_STAGE_IDX_Z];
      w = ov[CLIPPING_STAGE_IDX_W];
      if ((x >= -w) && (x <= w) &&
          (y >= -w) && (y <= w) &&
          (z >= -w) && (z <= w) &&
          (w > 0.f)) {
        /* Pass */
      }
      else {
        fprintf(stderr, "#%d  %f %f %f %f\n", (int)k,
                ov[CLIPPING_STAGE_IDX_X], ov[CLIPPING_STAGE_IDX_Y], ov[CLIPPING_STAGE_IDX_Z], ov[CLIPPING_STAGE_IDX_W]);
        fprintf(stderr, "Error: vertex not within clipbox\n");
        r = -1;
        goto test_exit;
      }
      ov += cs.num_varyings_;
    }
  }

  r = 0;
test_exit:
  clipping_stage_cleanup(&cs);
  return r;
}
