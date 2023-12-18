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
#ifndef PRIMITIVE_ASSEMBLY_H
#define PRIMITIVE_ASSEMBLY_H

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Number of rows in the primitive assembly buffer. */
#define PRIMITIVE_ASSEMBLY_MAX_ROWS 384

typedef enum primitive_assembly_data_type {
  PADT_NONE,
  PADT_FLOAT,
  PADT_UINT8,
  PADT_UINT16,
  PADT_INT32
} primitive_assembly_data_type_t;

typedef enum primitive_assembly_column_type {
  PACT_EXECUTION_CHAIN,
  PACT_POSITION_X,
  PACT_POSITION_Y,
  PACT_POSITION_Z,
  PACT_POSITION_W,
  PACT_POINT_SIZE,
  PACT_VARYING
} primitive_assembly_column_type_t;

typedef enum primitive_assembly_mode {
  PAM_POINTS,
  PAM_LINES,
  PAM_LINE_STRIP,
  PAM_LINE_LOOP,
  PAM_TRIANGLES,
  PAM_TRIANGLE_STRIP,
  PAM_TRIANGLE_FAN
} primitive_assembly_mode_t;

struct primitive_assembly_column_descriptor {
  /* Type of the primitive assembly column */
  primitive_assembly_column_type_t col_type_;

  /* Datatype of the primitive assembly column */
  primitive_assembly_data_type_t data_type_;

  /* Index of the attribute variable, -1 if unbound */
  int attrib_index_;

  /* Index of the element in the attribute variable */
  int attrib_element_index_;
};

struct primitive_assembly {
  /* Current number of rows, maximum is PRIMITIVE_ASSEMBLY_MAX_ROWS */
  size_t num_rows_;

  /* Memory allocated for the rows */
  void *slab_;

  /* Array of pointers into the slab_ to the start of the column data.
   * Each array has PRIMITIVE_ASSEMBLY_MAX_ROWS allocated, how many of
   * those are in use is determined by num_rows_. */
  void **column_data_;

  /* Array of num_cols_ column descriptions */
  size_t num_cols_;
  struct primitive_assembly_column_descriptor *column_descriptors_;

  /* Vertex indices buffer */
  size_t num_vertex_indices_;
  uint32_t vertex_indices_[191];

  /* While assembling primitives, this is used to mark internal progress */
  size_t index_at_;
  int continue_at_;
};

/* Initializes the primitive assembly but does not allocate the memory, to
 * complete the initialization, call primitive_assembly_alloc_buffers. */
void primitive_assembly_init(struct primitive_assembly *pa);

int primitive_assembly_alloc_buffers(struct primitive_assembly *pa);

void primitive_assembly_cleanup(struct primitive_assembly *pa);

int primitive_assembly_elements_u8(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, uint8_t *indices, size_t num_indices);
int primitive_assembly_elements_u16(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, uint16_t *indices, size_t num_indices);
int primitive_assembly_elements_u32(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, uint32_t *indices, size_t num_indices);



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PRIMITIVE_ASSEMBLY_H */
