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
#ifndef PERSPECTIVE_DIVISION_H
#define PERSPECTIVE_DIVISION_H

#ifndef STDDEF_H_INCLUDED
#define STDDEF_H_INCLUDED
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Performs the perspective division for a set of vertices. Each vertex consists of a number
 * of attributes, starting with the XYZW homogeneous coordinates and followed by any attributes.
 * The perspective division is performed in-place, with exception of the W coordinate which
 * will be overwritten with the reciprocal of the W coordinate (1/W, one-over-w, oow).
 * num_attribs specifies the number of attributes and should include the XYZW coordinates,
 * so the minimum value is 4. The number of vertices is specified by num_vertices.
 * vertex_stride specifies the number of bytes between the start of one vertex and the start
 * of the next vertex.
 */
void perspective_division(size_t num_vertices, size_t num_attribs, size_t vertex_stride, float *restrict xyzwattribs);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PERSPECTIVE_DIVISION_H */
