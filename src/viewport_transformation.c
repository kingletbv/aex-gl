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

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED
#include <math.h>
#endif

#ifndef RASTERIZER_H_INCLUDED
#define RASTERIZER_H_INCLUDED
#include "rasterizer.h"
#endif

#ifndef VIEWPORT_TRANSFORMATION_H_INCLUDED
#define VIEWPORT_TRANSFORMATION_H_INCLUDED
#include "viewport_transformation.h"
#endif

void viewport_transformation(int32_t vp_x, int32_t vp_y,
                             uint32_t vp_width, uint32_t vp_height,
                             float depth_range_near, float depth_range_far,
                             uint32_t screen_width, uint32_t screen_height, uint32_t max_z,
                             size_t num_vertices,
                             float *x, float *y, float *z, float *oow, size_t input_stride,
                             int32_t *sx, int32_t *sy, uint32_t *sz, size_t output_stride) {
  size_t n;
  float half_w = (float)(vp_width) / 2.f;
  float half_h = (float)(vp_height) / 2.f;
  float center_x = ((float)vp_x) + half_w;
  float center_y = ((float)vp_y) + half_h;
  float half_depth_range = (depth_range_far - depth_range_near) / 2.f;
  float depth_range_middle = (depth_range_near + depth_range_far) / 2.f;
  uint32_t subpixel_screen_height = screen_height << RASTERIZER_SUBPIXEL_BITS;
  float *px = x, *py = y, *pz = z, *poow = oow;
  int32_t *osx = sx, *osy = sy;
  uint32_t *osz = sz;
  for (n = 0; n < num_vertices; ++n) {

    *osx = (int32_t)((half_w * *px + center_x) * (1 << RASTERIZER_SUBPIXEL_BITS));
    *osy = subpixel_screen_height - (int32_t)((half_h * *py + center_y) * (1 << RASTERIZER_SUBPIXEL_BITS));
    float z_rescaled_0to1 = half_depth_range * *pz + depth_range_middle;

    /* in-place update the Z component to be in range depth_range_near..depth_range_far -
     * this is typically 0..1 and is what the fragment shaders are expecting. */
    *pz = z_rescaled_0to1;

    /* llroundf rounds to nearest int64, which we then cast down to uint32 for the z-buffer value.
     * This is necessary so we induce the desired round-to-nearest behavior, rather than whatever
     * arbitrary rounding mode the FPU/SSE/compiler might otherwise do (e.g. truncation.) */
    *osz = (uint32_t)llroundf(max_z * z_rescaled_0to1);

    px = (float *)(input_stride + (char *)px);
    py = (float *)(input_stride + (char *)py);
    pz = (float *)(input_stride + (char *)pz);
    poow = (float *)(input_stride + (char *)poow);
    osx = (int32_t *)(output_stride + (char *)osx);
    osy = (int32_t *)(output_stride + (char *)osy);
    osz = (uint32_t *)(output_stride + (char *)osz);
  }
}
