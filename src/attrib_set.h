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

#ifndef ATTRIB_SET_H
#define ATTRIB_SET_H

#ifdef __cplusplus
extern "C" {
#endif

struct data_buffer;

typedef enum attrib_data_type {
  ADT_BYTE,
  ADT_UNSIGNED_BYTE,
  ADT_SHORT,
  ADT_UNSIGNED_SHORT,
  ADT_INT,
  ADT_UNSIGNED_INT,
  ADT_FIXED,
  ADT_FLOAT
} attrib_data_type_t;

typedef enum attrib_type {
  AT_FLOAT,
  AT_VEC2,
  AT_VEC3,
  AT_VEC4,
  AT_MAT2,  /* consecutive columns spread out over 2 consecutive vec2 attributes */
  AT_MAT3,  /* consecutive columns spread out over 3 consecutive vec3 attributes */
  AT_MAT4   /* consecutive columns spread out over 4 consecutive vec4 attributes */
} attrib_type_t;

struct attrib {
  /* buffer, if any, NULL otherwise
   * If a buffer is present, then "ptr_" is an offset into it. If no
   * buffer is present (buf_ == NULL), then ptr_ specifies a pointer to
   * the data. */
  struct data_buffer *buf_;

  /* Number of elements per attrib value (e.g. dimensionality of vector)
   * max. 4; note that a shader can access more than size_ elements, in
   * that case, the remaining elements are from the generic values */
  int size_;

  /* Datatype of attribute, as declared by client. This is what ptr_ refers
   * to. */
  attrib_data_type_t data_type_;

  /* Type of attribute, as declared in the shader. Note that the shader's
   * type specified here and the actual data type by this indexed attrib
   * may differ, e.g. you could specify a vec4 at the shader level and
   * only specify a size_ 2 here (for x and y) ; the remaining (z and w)
   * would be retrieved from the generic_values_.
   * Additionally, if the shader type specified here is MAT2/3/4, then
   * multiple consecutive vec2/3/4's will contain the corresponding column
   * of the mat. Each of those attribs will have type MAT2/3/4. */
  attrib_type_t att_;

  /* If an integer type, whether the value should be normalized.
   * Signed integer types are normalized to -1..1 from their corresponding
   * MIN_INT, MAX_INT range. Unsigned integer types are normalized to
   * 0..1, from their corresponding 0, MAX_UINT range. */
  int normalize_:1;

  /* If enabled (true,) ptr_ is deemed valid, and points into an array or is an
   * offset into the buffer specified by buf_. If disabled (false,) ptr_ is
   * ignored, and all 4 elements are accessed from the generic_value_ */
  int enabled_:1;

  /* Pointer to the data if buf_ is NULL, alternatively, an offset (convert
   * to uintptr_t) into the buf_ if buf_ is not NULL. */
  void *ptr_;

  /* Number of bytes between consecutive elements in the buffer, if 0, then
   * the same element is repeated over and over. (This is different from the
   * OpenGL API, use sizeof(float)*size_ to pack tightly.) */
  size_t stride_;

  /* Generic values for the attribute, used when accessing elements outside
   * the size_ constraint, or when the attribute is not enabled */
  float generic_values_[4];

  /* String containing the name of the attribute */
  char *name_;
};

struct attrib_set {
  size_t num_attribs_;
  size_t num_attribs_allocated_;
  struct attrib *attribs_;
};

void attrib_set_init(struct attrib_set *as);
void attrib_set_cleanup(struct attrib_set *as);

int attrib_set_alloc_attrib(struct attrib_set *as);

#ifdef  __cplusplus
} /* extern "C" */
#endif

#endif /* BUFFERED_ATTRIB_SET_H */
