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

#ifndef VIEWPORT_TRANSFORMATION_H
#define VIEWPORT_TRANSFORMATION_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Performs OpenGL style viewport transformation:
 * vp_x: the left edge in pixels of the viewport, as measured from the bottom-left corner
 *       (e.g. 0 is all the way to the left of the destination window or bitmap)
 * vp_y: the bottom edge in pixels of the viewport, as measured from the bottom-left corner
 *       (e.g. 0 is all the way to the bottom of the destination window or bitmap)
 * vp_width: the absolute width in pixels, of the viewport
 * vp_height: the absolute height, in pixels, of the viewport
 * depth_range_near: the near depth-buffer range (typically this is 0.)
 * depth_range_far: the far depth-buffer range (typically, this is 1.)
 *                  note: depth_range_near and depth_range_far correspond to the values
 *                  set by glDepthRangef().
 * screen_width: the width, in pixels, of the destination window or bitmap, on which the viewport
 *               forms a rectangle.
 * screen_height: the height, in pixels, of the destination window or bitmap, on which the viewport
 *                forms a rectangle. Note that the screen_height is how the vp_y can be converted to
 *                a top-left origin coordinate system from the bottom-left origin coordinate system.
 * max_z: the maximum integer value of the z-buffer. This is typically 0xFFFF for a 16 bit z-buffer,
 *        0xFFFFFF for a 24 bit z-buffer, 0xFFFFFFFF for a 32 bit z-buffer, etc.
 * num_vertices: the number of vertices to be transformed
 * x,y,z,w: input pointers to the vertex coordinates to be transformed
 * input_stride: the distance of one coordinate element in the input pointers, to the next coordinate element,
 *               measured in bytes.
 * sx,sy,sz: pointers to the transformed vertex coordinates, the output of this function.
 * output_stride: the distance of one coordinate element in the output pointers, to the next coordinate
 *                element, measured in bytes. */
void viewport_transformation(int32_t vp_x, int32_t vp_y,
                             uint32_t vp_width, uint32_t vp_height,
                             float depth_range_near, float depth_range_far,
                             uint32_t screen_width, uint32_t screen_height, uint32_t max_z,
                             size_t num_vertices,
                             float *x, float *y, float *z, float *w, size_t input_stride,
                             int32_t *sx, int32_t *sy, uint32_t *sz, size_t output_stride);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* VIEWPORT_TRANSFORMATION_H */
