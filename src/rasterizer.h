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

#ifndef RASTERIZER_H
#define RASTERIZER_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define RASTERIZER_SUBPIXEL_BITS 8

#define RASTERIZER_CLOCKWISE 1
#define RASTERIZER_COUNTERCLOCKWISE 2
#define RASTERIZER_BOTH (RASTERIZER_CLOCKWISE | RASTERIZER_COUNTERCLOCKWISE)

struct fragment_buffer;

struct rasterizer {
  int resume_at_;

  /* context recovery values */
  int direction_xy_flips_;
  int64_t D012_;

  int orientation_; /* RASTERIZER_CLOCKWISE or RASTERIZER_COUNTERCLOCKWISE */

  int32_t z_offset_;

  int32_t x0_, y0_, z0_;
  int32_t x1_, y1_, z1_;
  int32_t x2_, y2_, z2_;

  uint8_t *pixel_TL_, *pixel_TR_, *pixel_BL_, *pixel_BR_;
  uint8_t *zbuf_TL_, *zbuf_TR_, *zbuf_BL_, *zbuf_BR_;
  uint8_t *stencil_TL_, *stencil_TR_, *stencil_BL_, *stencil_BR_;
  int64_t pixel_mod_, zbuf_mod_, stencil_mod_;
  int64_t px_, py_;

  int64_t Dp01_TL_, Dp12_TL_, Dp20_TL_;
  int64_t Dp01_TR_, Dp12_TR_, Dp20_TR_;
  int64_t Dp01_BL_, Dp12_BL_, Dp20_BL_;
  int64_t Dp01_BR_, Dp12_BR_, Dp20_BR_;

  int64_t Dp01_row_TL_, Dp12_row_TL_, Dp20_row_TL_;
  int64_t Dp01_row_TR_, Dp12_row_TR_, Dp20_row_TR_;
  int64_t Dp01_row_BL_, Dp12_row_BL_, Dp20_row_BL_;
  int64_t Dp01_row_BR_, Dp12_row_BR_, Dp20_row_BR_;

  int64_t z_TL_, z_TR_, z_BL_, z_BR_;
  int64_t z_s_TL_, z_s_TR_, z_s_BL_, z_s_BR_;
  int64_t z_sx_TL_, z_sx_TR_, z_sx_BL_, z_sx_BR_;
  int64_t z_x_TL_, z_x_TR_, z_x_BL_, z_x_BR_;
  
  int64_t scissor_top_row_mask_, scissor_bottom_row_mask_;
  int64_t scissor_left_column_mask_, scissor_right_column_mask_;
  int64_t left_, top_, right_, bottom_;

  int64_t TL_Mask_, TR_Mask_, BL_Mask_, BR_Mask_;
  int64_t Dp01_dx_2_, Dp12_dx_2_, Dp20_dx_2_;
  int64_t Dp01_dy_2_, Dp12_dy_2_, Dp20_dy_2_;
  int64_t z_xp_, z_xq_, z_xi_;
  int64_t z_yp_, z_yq_, z_yi_;

};

void rasterizer_init(struct rasterizer *rasterizer);
void rasterizer_cleanup(struct rasterizer *rasterizer);

/* Computes "D012" - the Determinant of vertices 0, 1 and 2 of a triangle:
 *        | x0 y0 1 |
 * D012 = | x1 y1 1 |
 *        | x2 y2 1 |
 * This corresponds to twice the surface area of the triangle and is an important 
 * value for the interpolation of attributes using barycentric coordinates. The
 * rasterizer, for each fragment, returns the determinents between the triangle
 * edges and the fragment's center point in FB_IDX_DP01, FB_IDX_DP12 and
 * FB_IDX_DP20; these represent double the area of those sub-triangles and are
 * therefore a blended weight of their respective opposite vertex corner only when
 * they are divided by the overal double triangle area D012.
 * Division is expensive and so we want to move this outside the inner loop. 
 * Because any attribute' must be's barycentric coordinates are multiplied against
 * their corresponding value at the vertices, we can pre-divide that value at the
 * vertices with D012 to get the desired effect. That, however, requires us to
 * get to D012 prior to rasterizer_triangle() computing this value for itself,
 * hence this function.
 */
int64_t rasterizer_compute_D012(int32_t px0, int32_t py0, uint32_t pz0,
                                int32_t px1, int32_t py1, uint32_t pz1,
                                int32_t px2, int32_t py2, uint32_t pz2);


/* Returns the orientation of the triangle, if the fragment buffer fragbf is full and needs to be processed, zero otherwise.
 * Relatively self-explanatory, permitted_orientations is one of RASTERIZER_CLOCKWISE, RASTERIZER_COUNTERCLOCKWISE,
 * or RASTERIZER_BOTH (i.e. it is a combination of RASTERIZER_CLOCKWISE and RASTERIZER_COUNTERCLOCKWISE).
 * Returns the orientation, if the fragment buffer is full and needs to be processed, and zero otherwise. If the detected 
 * orientation is not in the permitted_orientations, zero is returned and no fragments are generated.
 */
int rasterizer_triangle(struct rasterizer *rasterizer,
                        struct fragment_buffer *fragbf,
                        uint8_t *rgba, size_t stride,
                        uint8_t *zbuf, size_t zstride, size_t zstep,
                        uint8_t *stencilbuf, size_t stencil_stride, size_t stencil_step,
                        uint32_t scissor_left, uint32_t scissor_top, uint32_t scissor_right, uint32_t scissor_bottom,
                        int32_t x0, int32_t y0, uint32_t z0,
                        int32_t x1, int32_t y1, uint32_t z1,
                        int32_t x2, int32_t y2, uint32_t z2,
                        int permitted_orientations,
                        int32_t offset_factor_fixed8, int32_t offset_units_fixed8);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* RASTERIZER_H */
