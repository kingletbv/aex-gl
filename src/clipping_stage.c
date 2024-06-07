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

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef CLIPPING_STAGE_H_INCLUDED
#define CLIPPING_STAGE_H_INCLUDED
#include "clipping_stage.h"
#endif

void clipping_stage_init(struct clipping_stage *cs) {
  cs->num_varyings_ = 0;
  cs->input_varyings_ = NULL;
  cs->triangle_varyings_a_ = NULL;
  cs->num_triangles_in_b_ = 0;
  cs->triangle_varyings_b_ = NULL;
}


int clipping_stage_alloc_varyings(struct clipping_stage *cs, size_t num_extra_varyings) {
  /* num_varyings_ = XYZW + SX SY SZ + additionals */
  cs->num_varyings_ = 7 + num_extra_varyings;

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

static size_t clipping_stage_process_line_plane(struct clipping_stage *cs,
                                                float d0, float d1, 
                                                float *restrict input_v,
                                                float *restrict output_v) {
  size_t varying_index;
  float *v0, *v1;
  v0 = input_v;
  v1 = v0 + cs->num_varyings_;
  if (d0 >= 0.f) {
    /* v0 inside */
    if (d1 >= 0.f) {
      /* v1 inside, all inside, pass line */
      memcpy(output_v, input_v, sizeof(float) * cs->num_varyings_ * 2);
      return 1;
    }
    else {
      /* v0 inside, v1 outside */
      memcpy(output_v, input_v, sizeof(float) * cs->num_varyings_);
      float ood01 = 1.f / (d0 - d1);
      for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
        output_v[varying_index] = (-d1 * v0[varying_index] + d0 * v1[varying_index]) * ood01;
      }
      return 1;
    }
  }
  else {
    /* v0 outside */
    if (d1 >= 0.f) {
      /* v0 outside, v1 inside */
      float ood01 = 1.f / (d1 - d0);
      for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
        output_v[varying_index] = (d1 * v0[varying_index] + -d0 * v1[varying_index]) * ood01;
      }
      memcpy(output_v, input_v, sizeof(float) * cs->num_varyings_);
      return 1;
    }
    else {
      /* v0 outside, v1 outside - ignore line */
      return 0;
    }
  }
}

static size_t clipping_stage_process_triangle_plane(struct clipping_stage *cs,
                                                    float d0, float d1, float d2,
                                                    float *restrict input_v,
                                                    float *restrict output_v) {
  size_t varying_index;
  float *v0, *v1, *v2;
  v0 = input_v;
  v1 = v0 + cs->num_varyings_;
  v2 = v1 + cs->num_varyings_;

  if (d0 >= 0.f) {
    /* v0 inside */
    if (d1 >= 0.f) {
      /* v1 inside */
      if (d2 >= 0.f) {
        /* v2 inside, all inside, pass triangle */
        memcpy(output_v, input_v, sizeof(float) * cs->num_varyings_ * 3);
        output_v += cs->num_varyings_ * 3;
        return 1;
      }
      else /* (d2 < 0.f) */ {
        /* v0, v1 inside, v2 outside */
        /* form 0-1-c */
        float ood12 = 1.f / (d1 - d2);
        memcpy(output_v, input_v, sizeof(float) * 2 * cs->num_varyings_);
        output_v += 2 * cs->num_varyings_;
        float *c12 = output_v;
        for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
          output_v[varying_index] = (-d2 * v1[varying_index] + d1 * v2[varying_index]) * ood12;
        }
        output_v += cs->num_varyings_;
        // form c-d-0
        memcpy(output_v, c12, sizeof(float) * cs->num_varyings_);
        output_v += cs->num_varyings_;

        float *d20 = output_v;
        float ood20 = 1.f / (d0 - d2);
        for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
          output_v[varying_index] = (-d2 * v0[varying_index] + d0 * v2[varying_index]) * ood20;
        }
        output_v += cs->num_varyings_;
        memcpy(output_v, v0, sizeof(float) * cs->num_varyings_);
        output_v += cs->num_varyings_;
        return 2;
      }
    }
    else /* (d1 < 0.f) */ {
      if (d2 >= 0.f) {
        /* v0, v2 inside, v1 outside */
        /* form 0-c-2 */
        float ood01 = 1.f / (d0 - d1);
        memcpy(output_v, v0, sizeof(float) * cs->num_varyings_);
        output_v += cs->num_varyings_;
        float *c01 = output_v;
        for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
          output_v[varying_index] = (-d1 * v0[varying_index] + d0 * v1[varying_index]) * ood01;
        }
        output_v += cs->num_varyings_;
        memcpy(output_v, v2, sizeof(float) * cs->num_varyings_);
        output_v += cs->num_varyings_;
        // form 2-c-d
        memcpy(output_v, v2, sizeof(float) * cs->num_varyings_);
        output_v += cs->num_varyings_;
        memcpy(output_v, c01, sizeof(float) * cs->num_varyings_);
        output_v += cs->num_varyings_;
        float ood12 = 1.f / (d2 - d1);
        for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
          output_v[varying_index] = (-d1 * v2[varying_index] + d2 * v1[varying_index]) * ood12;
        }
        output_v += cs->num_varyings_;
        return 2;
      }
      else /* (d2 < 0.f) */ {
        /* v0 inside, v1, v2 outside */
        /* form 0-c-d */
        memcpy(output_v, v0, sizeof(float) * cs->num_varyings_);
        output_v += cs->num_varyings_;
        float ood01 = 1.f / (d0 - d1);
        float *c01 = output_v;
        for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
          output_v[varying_index] = (-d1 * v0[varying_index] + d0 * v1[varying_index]) * ood01;
        }
        output_v += cs->num_varyings_;
        float ood02 = 1.f / (d0 - d2);
        for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
          output_v[varying_index] = (-d2 * v0[varying_index] + d0 * v2[varying_index]) * ood02;
        }
        output_v += cs->num_varyings_;
        return 1;
      }
    }
  }
  else /* (d0 < 0.f) */ {
    /* v0 outside */
    if (d1 >= 0.f) {
      /* v1 inside */
      if (d2 >= 0.f) {
        /* v0 outside, v1, v2 inside */
        /* form c-1-2 */
        float ood01 = 1.f / (d1 - d0);
        float *c01 = output_v;
        for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
          output_v[varying_index] = (-d0 * v1[varying_index] + d1 * v0[varying_index]) * ood01;
        }
        output_v += cs->num_varyings_;
        memcpy(output_v, v1, sizeof(float) * cs->num_varyings_);
        output_v += cs->num_varyings_;
        memcpy(output_v, v2, sizeof(float) * cs->num_varyings_);
        output_v += cs->num_varyings_;
        /* form c-2-d */
        memcpy(output_v, c01, sizeof(float) * cs->num_varyings_);
        output_v += cs->num_varyings_;
        memcpy(output_v, v2, sizeof(float) * cs->num_varyings_);
        output_v += cs->num_varyings_;
        float ood02 = 1.f / (d2 - d0);
        for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
          output_v[varying_index] = (-d0 * v2[varying_index] + d2 * v0[varying_index]) * ood02;
        }
        output_v += cs->num_varyings_;
        return 2;
      }
      else {
        /* v0, v2 outside, v1 inside */
        /* form c-1-d */
        float ood01 = 1.f / (d1 - d0);
        for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
          output_v[varying_index] = (-d0 * v1[varying_index] + d1 * v0[varying_index]) * ood01;
        }
        output_v += cs->num_varyings_;
        memcpy(output_v, v1, sizeof(float) * cs->num_varyings_);
        output_v += cs->num_varyings_;
        float ood12 = 1.f / (d1 - d2);
        for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
          output_v[varying_index] = (-d2 * v1[varying_index] + d1 * v2[varying_index]) * ood12;
        }
        output_v += cs->num_varyings_;
        return 1;
      }
    }
    else {
      /* v1 outside */
      if (d2 >= 0.f) {
        /* v0, v1 outside, v2 inside */
        /* form c-2-d */
        float ood12 = 1.f / (d2 - d1);
        for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
          output_v[varying_index] = (-d1 * v2[varying_index] + d2 * v1[varying_index]) * ood12;
        }
        output_v += cs->num_varyings_;
        memcpy(output_v, v2, sizeof(float) * cs->num_varyings_);
        output_v += cs->num_varyings_;
        float ood02 = 1.f / (d2 - d0);
        for (varying_index = CLIPPING_STAGE_IDX_X; varying_index < cs->num_varyings_; ++varying_index) {
          output_v[varying_index] = (-d0 * v2[varying_index] + d2 * v0[varying_index]) * ood02;
        }
        output_v += cs->num_varyings_;
        return 1;
      }
      else {
       /* v0, v1, v2 outside - ignore triangle. */
        return 0;
      }
    }
  }
}

size_t clipping_stage_process_triangle(struct clipping_stage *cs) {
  size_t tri_stride = cs->num_varyings_ * 3;
  float *input_v = cs->input_varyings_;
  float *output_v = cs->triangle_varyings_a_;
  float *v0, *v1, *v2;
  v0 = input_v;
  v1 = v0 + cs->num_varyings_;
  v2 = v1 + cs->num_varyings_;

  /* Note: OpenGL convention, DirectX would want z >= 0 (z == 0 after w div)
   * -z + w >= 0 
   */
  float d0 = -v0[CLIPPING_STAGE_IDX_Z] + v0[CLIPPING_STAGE_IDX_W];
  float d1 = -v1[CLIPPING_STAGE_IDX_Z] + v1[CLIPPING_STAGE_IDX_W];
  float d2 = -v2[CLIPPING_STAGE_IDX_Z] + v2[CLIPPING_STAGE_IDX_W];

  size_t num_tris = clipping_stage_process_triangle_plane(cs, d0, d1, d2, input_v, cs->triangle_varyings_a_);

  size_t n;
  size_t next_num_tris = 0;
  for (n = 0; n < num_tris; ++n) {
    v0 = cs->triangle_varyings_a_ + tri_stride * n;
    v1 = v0 + cs->num_varyings_;
    v2 = v1 + cs->num_varyings_;

    /* z + w >= 0 */
    d0 = v0[CLIPPING_STAGE_IDX_Z] + v0[CLIPPING_STAGE_IDX_W];
    d1 = v1[CLIPPING_STAGE_IDX_Z] + v1[CLIPPING_STAGE_IDX_W];
    d2 = v2[CLIPPING_STAGE_IDX_Z] + v2[CLIPPING_STAGE_IDX_W];

    size_t num_tris_passed = clipping_stage_process_triangle_plane(cs, d0, d1, d2, v0, cs->triangle_varyings_b_ + tri_stride * next_num_tris);
    next_num_tris += num_tris_passed;
  }

  num_tris = next_num_tris;
  next_num_tris = 0;

  for (n = 0; n < num_tris; ++n) {
    v0 = cs->triangle_varyings_b_ + tri_stride * n;
    v1 = v0 + cs->num_varyings_;
    v2 = v1 + cs->num_varyings_;

    /* -y + w >= 0 */
    d0 = -v0[CLIPPING_STAGE_IDX_Y] + v0[CLIPPING_STAGE_IDX_W];
    d1 = -v1[CLIPPING_STAGE_IDX_Y] + v1[CLIPPING_STAGE_IDX_W];
    d2 = -v2[CLIPPING_STAGE_IDX_Y] + v2[CLIPPING_STAGE_IDX_W];

    size_t num_tris_passed = clipping_stage_process_triangle_plane(cs, d0, d1, d2, v0, cs->triangle_varyings_a_ + tri_stride * next_num_tris);
    next_num_tris += num_tris_passed;
  }

  num_tris = next_num_tris;
  next_num_tris = 0;


  for (n = 0; n < num_tris; ++n) {
    v0 = cs->triangle_varyings_a_ + tri_stride * n;
    v1 = v0 + cs->num_varyings_;
    v2 = v1 + cs->num_varyings_;

    /* y + w >= 0 */
    d0 = v0[CLIPPING_STAGE_IDX_Y] + v0[CLIPPING_STAGE_IDX_W];
    d1 = v1[CLIPPING_STAGE_IDX_Y] + v1[CLIPPING_STAGE_IDX_W];
    d2 = v2[CLIPPING_STAGE_IDX_Y] + v2[CLIPPING_STAGE_IDX_W];

    size_t num_tris_passed = clipping_stage_process_triangle_plane(cs, d0, d1, d2, v0, cs->triangle_varyings_b_ + tri_stride * next_num_tris);
    next_num_tris += num_tris_passed;
  }

  num_tris = next_num_tris;
  next_num_tris = 0;


  for (n = 0; n < num_tris; ++n) {
    v0 = cs->triangle_varyings_b_ + tri_stride * n;
    v1 = v0 + cs->num_varyings_;
    v2 = v1 + cs->num_varyings_;

    /* -x + w >= 0 */
    d0 = -v0[CLIPPING_STAGE_IDX_X] + v0[CLIPPING_STAGE_IDX_W];
    d1 = -v1[CLIPPING_STAGE_IDX_X] + v1[CLIPPING_STAGE_IDX_W];
    d2 = -v2[CLIPPING_STAGE_IDX_X] + v2[CLIPPING_STAGE_IDX_W];

    size_t num_tris_passed = clipping_stage_process_triangle_plane(cs, d0, d1, d2, v0, cs->triangle_varyings_a_ + tri_stride * next_num_tris);
    next_num_tris += num_tris_passed;
  }

  num_tris = next_num_tris;
  next_num_tris = 0;


  for (n = 0; n < num_tris; ++n) {
    v0 = cs->triangle_varyings_a_ + tri_stride * n;
    v1 = v0 + cs->num_varyings_;
    v2 = v1 + cs->num_varyings_;

    /* x + w >= 0 */
    d0 = v0[CLIPPING_STAGE_IDX_X] + v0[CLIPPING_STAGE_IDX_W];
    d1 = v1[CLIPPING_STAGE_IDX_X] + v1[CLIPPING_STAGE_IDX_W];
    d2 = v2[CLIPPING_STAGE_IDX_X] + v2[CLIPPING_STAGE_IDX_W];

    size_t num_tris_passed = clipping_stage_process_triangle_plane(cs, d0, d1, d2, v0, cs->triangle_varyings_b_ + tri_stride * next_num_tris);
    next_num_tris += num_tris_passed;
  }

  num_tris = next_num_tris;
  cs->num_triangles_in_b_ = num_tris;

  return num_tris;
}

/* Processes line, this is analogous to the triangle and could be done inplace, however,
 * we'll go for consistency and go back and forth between two buffers just like the triangle case. */
size_t clipping_stage_process_line(struct clipping_stage *cs) {
  size_t tri_stride = cs->num_varyings_ * 3;
  float *input_v = cs->input_varyings_;
  float *output_v = cs->triangle_varyings_a_;
  float *v0, *v1;
  cs->num_triangles_in_b_ = 0;
  v0 = input_v;
  v1 = v0 + cs->num_varyings_;

  /* Note: OpenGL convention, DirectX would want z >= 0 (z == 0 after w div)
   * -z + w >= 0
   */
  float d0 = -v0[CLIPPING_STAGE_IDX_Z] + v0[CLIPPING_STAGE_IDX_W];
  float d1 = -v1[CLIPPING_STAGE_IDX_Z] + v1[CLIPPING_STAGE_IDX_W];

  size_t num_lines = clipping_stage_process_line_plane(cs, d0, d1, input_v, cs->triangle_varyings_a_);

  if (!num_lines) return 0;

  v0 = cs->triangle_varyings_a_;
  v1 = v0 + cs->num_varyings_;

  /* z + w >= 0 */
  d0 = v0[CLIPPING_STAGE_IDX_Z] + v0[CLIPPING_STAGE_IDX_W];
  d1 = v1[CLIPPING_STAGE_IDX_Z] + v1[CLIPPING_STAGE_IDX_W];

  num_lines = clipping_stage_process_line_plane(cs, d0, d1, cs->triangle_varyings_a_, cs->triangle_varyings_b_);

  if (!num_lines) return 0;

  v0 = cs->triangle_varyings_b_;
  v1 = v0 + cs->num_varyings_;

  /* -y + w >= 0 */
  d0 = -v0[CLIPPING_STAGE_IDX_Y] + v0[CLIPPING_STAGE_IDX_W];
  d1 = -v1[CLIPPING_STAGE_IDX_Y] + v1[CLIPPING_STAGE_IDX_W];

  num_lines = clipping_stage_process_line_plane(cs, d0, d1, cs->triangle_varyings_b_, cs->triangle_varyings_a_);

  if (!num_lines) return 0;

  v0 = cs->triangle_varyings_a_;
  v1 = v0 + cs->num_varyings_;

  /* y + w >= 0 */
  d0 = v0[CLIPPING_STAGE_IDX_Y] + v0[CLIPPING_STAGE_IDX_W];
  d1 = v1[CLIPPING_STAGE_IDX_Y] + v1[CLIPPING_STAGE_IDX_W];

  num_lines = clipping_stage_process_line_plane(cs, d0, d1, cs->triangle_varyings_a_, cs->triangle_varyings_b_);

  if (!num_lines) return 0;

  v0 = cs->triangle_varyings_b_;
  v1 = v0 + cs->num_varyings_;

  /* -x + w >= 0 */
  d0 = -v0[CLIPPING_STAGE_IDX_X] + v0[CLIPPING_STAGE_IDX_W];
  d1 = -v1[CLIPPING_STAGE_IDX_X] + v1[CLIPPING_STAGE_IDX_W];

  num_lines = clipping_stage_process_line_plane(cs, d0, d1, cs->triangle_varyings_b_, cs->triangle_varyings_a_);

  if (!num_lines) return 0;

  v0 = cs->triangle_varyings_a_;
  v1 = v0 + cs->num_varyings_;

  /* x + w >= 0 */
  d0 = v0[CLIPPING_STAGE_IDX_X] + v0[CLIPPING_STAGE_IDX_W];
  d1 = v1[CLIPPING_STAGE_IDX_X] + v1[CLIPPING_STAGE_IDX_W];

  num_lines = clipping_stage_process_line_plane(cs, d0, d1, cs->triangle_varyings_a_, cs->triangle_varyings_b_);
  cs->num_triangles_in_b_ = num_lines; /* technically not the number of triangles but the number of lines */
    
  return num_lines;
}

