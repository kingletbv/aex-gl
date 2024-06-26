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
#ifndef PRIMITIVE_ASSEMBLY_H
#define PRIMITIVE_ASSEMBLY_H

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef BLEND_H_INCLUDED
#define BLEND_H_INCLUDED
#include "blend.h"
#endif

#ifndef RASTERIZER_H_INCLUDED
#define RASTERIZER_H_INCLUDED
#include "rasterizer.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct sl_shader;

/* Number of rows in the primitive assembly buffer. */
#define PRIMITIVE_ASSEMBLY_MAX_ROWS 256

typedef enum primitive_assembly_data_type {
  PADT_NONE,
  PADT_FLOAT,
  PADT_UINT8,
  PADT_UINT16,
  PADT_INT32
} primitive_assembly_data_type_t;

typedef enum primitive_assembly_mode {
  PAM_POINTS,
  PAM_LINES,
  PAM_LINE_STRIP,
  PAM_LINE_LOOP,
  PAM_TRIANGLES,
  PAM_TRIANGLE_STRIP,
  PAM_TRIANGLE_FAN
} primitive_assembly_mode_t;

typedef enum primitive_assembly_index_type {
  PAIT_UNSIGNED_BYTE,
  PAIT_UNSIGNED_SHORT,
  PAIT_UNSIGNED_INT
} primitive_assembly_index_type_t;

typedef enum primitive_assembly_stencil_func {
  PASF_EQUAL,
  PASF_NOTEQUAL,
  PASF_LESS,
  PASF_GREATER,
  PASF_LEQUAL,
  PASF_GEQUAL,
  PASF_ALWAYS,
  PASF_NEVER
} primitive_assembly_stencil_func_t;

typedef enum primitive_assembly_stencil_op {
  PASO_ZERO,
  PASO_REPLACE,
  PASO_INCR,
  PASO_DECR,
  PASO_INCR_WRAP,
  PASO_DECR_WRAP,
  PASO_KEEP,
  PASO_INVERT
} primitive_assembly_stencil_op_t;

typedef enum primitive_assembly_zbuf_func {
  PAZF_NEVER,
  PAZF_LESS,
  PAZF_EQUAL,
  PAZF_LEQUAL,
  PAZF_GREATER,
  PAZF_NOTEQUAL,
  PAZF_GEQUAL,
  PAZF_ALWAYS
} primitive_assembly_zbuf_func_t;

struct primitive_assembly_column_descriptor {
  /* Datatype of the primitive assembly column */
  primitive_assembly_data_type_t data_type_;

  /* Index of the attribute variable, -1 if unbound */
  int attrib_index_;

  /* Index of the element in the attribute variable */
  int attrib_element_index_;

  /* Index of the register corresponding to this attribute */
  int register_;
};

struct primitive_assembly {
  /* Current number of rows, maximum is PRIMITIVE_ASSEMBLY_MAX_ROWS */
  size_t num_rows_;

  /* Array of num_cols_ column descriptions */
  size_t num_cols_;
  size_t num_cols_allocated_;
  struct primitive_assembly_column_descriptor *column_descriptors_;

  /* Vertex indices buffer */
  size_t num_vertex_indices_;
  uint32_t vertex_indices_[191];

  /* While assembling primitives, this is used to mark internal progress */
  size_t index_at_;
  size_t end_index_;
  int continue_at_;
  int continue_from_fragments_;

  /* backups for continuation on fragments in primitive_assembly_process_primitives() */
  struct sl_variable *vgl_Position_;
  struct sl_function *vmain_;
  struct sl_variable *fgl_FragCoord_;
  rasterizer_early_zbuf_func_t early_z_zbuf_func_;
  float *v0_, *v1_, *v2_;
  int32_t offset_factor_f8_, offset_units_f8_;
  int32_t sx0_, sy0_, sz0_, sx1_, sy1_, sz1_, sx2_, sy2_, sz2_;
  uint32_t norm_scissor_left_, norm_scissor_top_, norm_scissor_right_, norm_scissor_bottom_;
  size_t pa_row_index_, clip_tri_index_, prior_num_rows_in_fragbuf_;

  /* The XYZ set of vertices that make up the triangles for line segment triangulation */
  size_t line_seg_tri_index_;
  int primary_dimension_; /* primary dimension of line segment, zero for horizontal, non-zero for vertical */
  int last_orientation_;
  int32_t line_segment_triangulation_[3 * 3 * 4];

};

void primitive_assembly_init(struct primitive_assembly *pa);

void primitive_assembly_reset(struct primitive_assembly *pa);

int primitive_assembly_add_column(struct primitive_assembly *pa,
                                  primitive_assembly_data_type_t data_type,
                                  int attrib_index,
                                  int attrib_element_index,
                                  int target_register);

void primitive_assembly_cleanup(struct primitive_assembly *pa);

int primitive_assembly_elements_u8(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, const uint8_t *indices, size_t num_indices);
int primitive_assembly_elements_u16(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, const uint16_t *indices, size_t num_indices);
int primitive_assembly_elements_u32(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, const uint32_t *indices, size_t num_indices);
int primitive_assembly_elements_arrayed(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, size_t num_elements);

int primitive_assembly_gather_attribs(struct primitive_assembly *pa, struct attrib_set *as, struct sl_execution *exec);

void primitive_assembly_draw_elements(struct primitive_assembly *pa,
                                      struct attrib_set *as,
                                      struct sl_shader *vertex_shader,
                                      struct attrib_routing *ar,
                                      struct clipping_stage *cs,
                                      struct rasterizer *ras,
                                      struct fragment_buffer *fragbuf,
                                      struct sl_shader *fragment_shader,
                                      int32_t vp_x,
                                      int32_t vp_y,
                                      uint32_t vp_width,
                                      uint32_t vp_height,
                                      float depth_range_near,
                                      float depth_range_far,
                                      uint32_t screen_width,
                                      uint32_t screen_height,
                                      int32_t scissor_left, int32_t scissor_bottom_counted_from_bottom,
                                      int32_t scissor_width, int32_t scissor_height,
                                      uint32_t max_z,
                                      uint8_t *rgba, size_t rgba_stride,
                                      uint8_t *zbuf, size_t zbuf_stride, size_t zbuf_step,
                                      uint8_t *stencil_buf, size_t stencil_stride, size_t stencil_step,
                                      int permitted_orientations,
                                      int enable_stencil_test, 
                                      uint32_t stencil_cw_mask,
                                      primitive_assembly_stencil_func_t stencil_cw_func, uint32_t stencil_cw_func_ref, uint32_t stencil_cw_func_mask,
                                      primitive_assembly_stencil_op_t stencil_cw_sfail, 
                                      primitive_assembly_stencil_op_t stencil_cw_zfail, 
                                      primitive_assembly_stencil_op_t stencil_cw_zpass,
                                      uint32_t stencil_ccw_mask,
                                      primitive_assembly_stencil_func_t stencil_ccw_func, uint32_t stencil_ccw_func_ref, uint32_t stencil_ccw_func_mask,
                                      primitive_assembly_stencil_op_t stencil_ccw_sfail, 
                                      primitive_assembly_stencil_op_t stencil_ccw_zfail, 
                                      primitive_assembly_stencil_op_t stencil_ccw_zpass,
                                      int enable_zbuf_test,
                                      primitive_assembly_zbuf_func_t zbuf_func,
                                      int enable_zbuf_write,
                                      int enable_red, int enable_green, int enable_blue, int enable_alpha,
                                      blend_eq_t rgb_eq, blend_eq_t alpha_eq,
                                      blend_func_t src_rgb_fn, blend_func_t src_alpha_fn,
                                      blend_func_t dst_rgb_fn, blend_func_t dst_alpha_fn,
                                      uint8_t constant_red, uint8_t constant_grn, uint8_t constant_blu, uint8_t constant_alpha,
                                      float offset_factor, float offset_units,
                                      primitive_assembly_mode_t mode,
                                      size_t num_elements,
                                      primitive_assembly_index_type_t index_type,
                                      size_t arrayed_starting_index,
                                      const void *indices);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PRIMITIVE_ASSEMBLY_H */
