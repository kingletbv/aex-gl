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
#ifndef GL_ES2_CONTEXT_H
#define GL_ES2_CONTEXT_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef REF_RANGE_ALLOCATOR_H_INCLUDED
#define REF_RANGE_ALLOCATOR_H_INCLUDED
#include "ref_range_allocator.h"
#endif

#ifndef NAMED_OBJECT_TABLE_H_INCLUDED
#define NAMED_OBJECT_TABLE_H_INCLUDED
#include "named_object_table.h"
#endif

#ifndef GL_ES2_IMPL_TYPES_H_INCLUDED
#define GL_ES2_IMPL_TYPES_H_INCLUDED
#include "gl_es2_impl_types.h"
#endif

#ifndef SL_PROGRAM_H_INCLUDED
#define SL_PROGRAM_H_INCLUDED
#include "sl_program.h"
#endif

#ifndef DATA_BUFFER_H_INCLUDED
#define DATA_BUFFER_H_INCLUDED
#include "data_buffer.h"
#endif

#ifndef SAMPLER_2D_H_INCLUDED
#define SAMPLER_2D_H_INCLUDED
#include "sampler_2d.h"
#endif

#ifndef SL_SHADER_H_INCLUDED
#define SL_SHADER_H_INCLUDED
#include "sl_shader.h"
#endif

#ifndef ATTRIB_SET_H_INCLUDED
#define ATTRIB_SET_H_INCLUDED
#include "attrib_set.h"
#endif

#ifndef RASTERIZER_H_INCLUDED
#define RASTERIZER_H_INCLUDED
#include "rasterizer.h"
#endif

/* glGet(GL_MAX_TEXTURE_IMAGE_UNITS)
 * glGet(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS)
 */
#define GL_ES2_IMPL_MAX_NUM_TEXTURE_UNITS 64

/* glGet(GL_MAX_VERTEX_ATTRIBS) */
#define GL_ES2_IMPL_MAX_NUM_VERTEX_ATTRIBS 1024

/* glGet(GL_MAX_VIEWPORT_DIMS)
 * as per design document "Bits of precision needed to determine area of 2D triangle.docx" */
#define GL_ES2_IMPL_MAX_VIEWPORT_DIMS 4194304

#ifdef __cplusplus
extern "C" {
#endif

enum gl_es2_framebuffer_attachment_object_type {
  gl_es2_faot_none,         /* GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE of GL_NONE */
  gl_es2_faot_renderbuffer, /* GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE of GL_RENDERBUFFER */
  gl_es2_faot_texture       /* GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE of GL_TEXTURE */
};

enum gl_es2_cube_map_face {
  gl_es2_cube_map_positive_x,
  gl_es2_cube_map_negative_x,
  gl_es2_cube_map_positive_y,
  gl_es2_cube_map_negative_y,
  gl_es2_cube_map_positive_z,
  gl_es2_cube_map_negative_z
};

enum gl_es2_texture_kind {
  gl_es2_texture_invalid,
  gl_es2_texture_2d,
  gl_es2_texture_cube_map
};

enum gl_es2_framebuffer_completeness {
  gl_es2_framebuffer_complete,
  gl_es2_framebuffer_incomplete_attachment,
  gl_es2_framebuffer_incomplete_dimensions,
  gl_es2_framebuffer_incomplete_missing_attachment,
  gl_es2_framebuffer_incomplete_unsupported
};

enum gl_es2_renderbuffer_format {
  gl_es2_renderbuffer_format_none,
  gl_es2_renderbuffer_format_rgba32,
  gl_es2_renderbuffer_format_depth16,
  gl_es2_renderbuffer_format_depth32,
  gl_es2_renderbuffer_format_stencil16
};

enum gl_es2_cull_face {
  gl_es2_cull_face_front,
  gl_es2_cull_face_back,
  gl_es2_cull_face_front_and_back /* both sides rejected, but lines and points still pass */
};

enum gl_es2_front_face {
  gl_es2_front_face_clockwise,
  gl_es2_front_face_counterclockwise
};

struct gl_es2_framebuffer_attachment {
  enum gl_es2_framebuffer_attachment_object_type kind_;

  /* Pointer to renderbuffer or texture that is attached to the framebuffer */
  union {
    struct gl_es2_renderbuffer *rb_;
    struct gl_es2_texture *tex_;
  } v_;
    
  /* The following fields are for kind_ == gl_es2_faot_texture only */
  gl_es2_int level_;                        /* GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL */
  enum gl_es2_cube_map_face cube_map_face_; /* GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE */

  /* Next and previous attachment for the framebuffer
   * attachments that share this same renderbuffer or 
   * texture. */
  struct gl_es2_framebuffer_attachment *next_, *prev_;
  
  /* Convenience backpointer to the framebuffer of which this gl_es2_framebuffer_attachment is a field */
  struct gl_es2_framebuffer *fb_;
};

struct gl_es2_program_shader_attachment {
  /* Pointer to the attached shader; NULL if no shader is attached */
  struct gl_es2_shader *shader_;

  /* Next and previous attachment for other programs sharing this same shader */
  struct gl_es2_program_shader_attachment *next_, *prev_;

  /* Convenience backpointer to the program of which this gl_es2_program_shader_attachment is a field */
  struct gl_es2_program *program_;
};

struct gl_es2_framebuffer {
  struct named_object no_;

  struct gl_es2_framebuffer_attachment color_attachment0_;
  struct gl_es2_framebuffer_attachment depth_attachment_;
  struct gl_es2_framebuffer_attachment stencil_attachment_;
};

struct gl_es2_renderbuffer {
  struct named_object no_;

  enum gl_es2_renderbuffer_format format_;

  struct gl_es2_framebuffer_attachment *first_framebuffer_attached_to_;

  int width_, height_;
  size_t num_bytes_per_bitmap_row_;
  void *bitmap_;
};

struct gl_es2_texture {
  struct named_object no_;

  enum gl_es2_texture_kind kind_;

  struct gl_es2_framebuffer_attachment *first_framebuffer_attached_to_;

  struct sampler_2d texture_2d_;
  struct sampler_2d texture_cube_map_positive_x_;
  struct sampler_2d texture_cube_map_negative_x_;
  struct sampler_2d texture_cube_map_positive_y_;
  struct sampler_2d texture_cube_map_negative_y_;
  struct sampler_2d texture_cube_map_positive_z_;
  struct sampler_2d texture_cube_map_negative_z_;
};

struct gl_es2_texture_unit {
  struct gl_es2_texture *texture_2d_;       /* glBindTexture() target == GL_TEXTURE_2D */
  struct gl_es2_texture *texture_cube_map_; /* glBindTexture() target == GL_TEXTURE_CUBE_MAP */
};

struct gl_es2_buffer {
  struct named_object no_;
  
  /* glGetBufferParameteriv(GL_BUFFER_USAGE)
   * GL_ES2_STREAM_DRAW, GL_ES2_STATIC_DRAW or GL_ES2_DYNAMIC_DRAW */
  gl_es2_enum usage_;

  struct data_buffer buf_;
};

struct gl_es2_program {
  struct named_object no_;

  /* glGetProgramiv(GL_DELETE_STATUS) */
  int flagged_for_deletion_:1;

  struct gl_es2_program_shader_attachment vertex_shader_;
  struct gl_es2_program_shader_attachment fragment_shader_;

  struct sl_program program_;
};

struct gl_es2_shader {
  struct named_object no_;

  /* GL_ES2_VERTEX_SHADER or GL_ES2_FRAGMENT_SHADER */
  gl_es2_enum type_;

  struct gl_es2_program_shader_attachment *first_program_attached_to_;

  /* glGetShaderiv(GL_DELETE_STATUS) */
  int flagged_for_deletion_:1;

  /* glGetShaderiv(GL_COMPILE_STATUS) */
  int compilation_status_:1;

  struct sl_shader shader_;
};

struct gl_es2_context {
  gl_es2_enum current_error_;

  /* If non-zero, then internals become accessible:
   * - glBindAttribLocation on attributes that start with "gl_".
   * - glGetUniformLocation on uniforms that start with "gl_".
   *
   * default is 0.
   */
  int allow_internals_:1;

  struct ref_range_allocator framebuffer_rra_;
  struct named_object_table framebuffer_not_;

  struct ref_range_allocator renderbuffer_rra_;
  struct named_object_table renderbuffer_not_;

  struct ref_range_allocator texture_rra_;
  struct named_object_table texture_not_;

  struct ref_range_allocator buffer_rra_;
  struct named_object_table buffer_not_;

  struct ref_range_allocator program_rra_;
  struct named_object_table program_not_;

  struct ref_range_allocator shader_rra_;
  struct named_object_table shader_not_;

  struct attrib_set attribs_;

  struct rasterizer ras_;

  /* currently bound targets */

  /* glBindFramebuffer(GL_FRAMEBUFFER) */
  struct gl_es2_framebuffer *framebuffer_;

  /* glBindRenderbuffer(GL_RENDERBUFFER) */
  struct gl_es2_renderbuffer *renderbuffer_;

  /* Number of active texture units; currently always GL_ES2_MAX_NUM_TEXTURE_UNITS */
  size_t num_active_texture_units_;

  /* Currently active texture unit, starts out as 0 */
  size_t current_active_texture_unit_;
  struct gl_es2_texture_unit active_texture_units_[GL_ES2_IMPL_MAX_NUM_TEXTURE_UNITS];

  /* glBindBuffer(GL_ARRAY_BUFFER); NULL means use client memory */
  struct gl_es2_buffer *array_buffer_;

  /* glBindBuffer(GL_ELEMENT_ARRAY_BUFFER); NULL means use client memory */
  struct gl_es2_buffer *element_array_buffer_;

  /* glUseProgram() - Currently used program */
  struct gl_es2_program *current_program_;

  /* is depth testing enabled ? - glEnable/glDisable(GL_DEPTH_TEST) */
  int is_depth_test_enabled_:1;

  /* glDepthFunc() - glGet(GL_DEPTH_FUNC) */
  primitive_assembly_zbuf_func_t zbuf_func_;

  /* near and far clipping planes, glDepthRangef(), glGet(GL_DEPTH_RANGE) */
  float near_plane_, far_plane_;

  /* viewport, glViewport(), glGet(GL_VIEWPORT) and related glGet(GL_MAX_VIEWPORT_DIMS) */
  int32_t vp_x_, vp_y_, vp_width_, vp_height_;

  /* is culling enabled ? - glEnable/glDisable(GL_CULL_FACE) */
  int is_culling_enabled_:1;

  /* glCullFace() - glGet(GL_CULL_FACE_MODE) */
  enum gl_es2_cull_face cull_face_mode_;

  /* glFrontFace() - glGet(GL_FRONT_FACE) */
  enum gl_es2_front_face front_face_;

  /* glStencilMask()/glStencilMaskSeparate() - glGet(GL_STENCIL_BACK_WRITEMASK), glGet(GL_STENCIL_WRITEMASK) */
  uint32_t stencil_back_writemask_, stencil_writemask_;

  /* glStencilFunc()/glStencilFuncSeparate() - glGet(GL_STENCIL_FUNC/GL_STENCIL_VALUE_MASK/GL_STENCIL_REF/ ..
   * .. GL_STENCIL_BACK_FUNC/GL_STENCIL_BACK_VALUE_MASK/GL_STENCIL_BACK_REF) */
  primitive_assembly_stencil_func_t stencil_func_;
  uint32_t stencil_func_ref_;
  uint32_t stencil_func_mask_;

  primitive_assembly_stencil_func_t stencil_back_func_;
  uint32_t stencil_back_func_ref_;
  uint32_t stencil_back_func_mask_;

  primitive_assembly_stencil_op_t stencil_sfail_;
  primitive_assembly_stencil_op_t stencil_zfail_;
  primitive_assembly_stencil_op_t stencil_zpass_;

  primitive_assembly_stencil_op_t stencil_back_sfail_;
  primitive_assembly_stencil_op_t stencil_back_zfail_;
  primitive_assembly_stencil_op_t stencil_back_zpass_;


  /* glEnable(GL_SCISSOR_TEST), glDisable(GL_SCISSOR_TEST), glIsEnabled(GL_SCISSOR_TEST) */
  int is_scissor_test_enabled_:1;

  /* glScissor() - glGet(GL_SCISSOR_BOX) */
  int32_t scissor_left_, scissor_bottom_counted_from_bottom_;
  int32_t scissor_width_, scissor_height_;

  /* glDepthMask(f) - glGet(GL_DEPTH_WRITEMASK) */
  int depth_mask_:1;

  /* glColorMask(r,g,b,a) - glGet(GL_COLOR_WRITEMASK) */
  int red_mask_:1, green_mask_:1, blue_mask_:1, alpha_mask_:1;

  /* glClearColor(r,g,b,a) - glGet(GL_COLOR_CLEAR_VALUE) */
  uint8_t clear_color_red_;
  uint8_t clear_color_grn_;
  uint8_t clear_color_blu_;
  uint8_t clear_color_alpha_;

  /* glClearDepth(depth) - glGet(GL_DEPTH_CLEAR_VALUE) */
  float clear_depth_;

  /* glClearStencil(stencil) - glGet(GL_STENCIL_CLEAR_VALUE) */
  uint16_t clear_stencil_;

  /* is blending enabled? glEnable/glDisable(GL_BLEND) */
  int is_blend_enabled_:1;

  /* glBlendEquation & glGet(GL_BLEND_EQUATION_RGB) & glGet(GL_BLEND_EQUATION_ALPHA) */
  blend_eq_t blend_rgb_eq_;
  blend_eq_t blend_alpha_eq_;

  /* glBlendFunc/glBlendFuncSeparate & glGet(GL_BLEND_SRC_RGB) & glGet(GL_BLEND_SRC_ALPHA) & 
   * glGet(GL_BLEND_DST_RGB) & glGet(GL_BLEND_DST_ALPHA) */
  blend_func_t blend_src_rgb_fn_;
  blend_func_t blend_src_alpha_fn_;
  blend_func_t blend_dst_rgb_fn_;
  blend_func_t blend_dst_alpha_fn_;

  /* glBlendColor() & glGet(GL_BLEND_COLOR) */
  uint8_t blend_color_red_;
  uint8_t blend_color_grn_;
  uint8_t blend_color_blu_;
  uint8_t blend_color_alpha_;

  /* Is stencil testing and writing enabled? glEnable/glDisable(GL_STENCIL_TEST) */
  int is_stencil_test_enabled_:1;

  /* Is polygon offset enabled? glPolygonOffset(), glEnable/glDisable(GL_POLYGON_OFFSET_FILL) */
  int is_polygon_offset_fill_enabled_:1;

  /* is dithering enabled? - glEnable/glDisable(GL_DITHER)
   * (not supported) */
  int is_dither_enabled_:1;

  /* Is sample alpha to coverage enabled? - glEnable/glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE)
   * (not supported) */
  int is_sample_alpha_to_coverage_enabled_:1;

  /* Is sample coverage enabled? - glEnable/glDisable(GL_SAMPLE_COVERAGE)
   * (not supported) */
  int is_sample_coverage_enabled_:1;
};

/* initializes the global context, returns SL_ERR_OK (sl_defs.h) for success or
 * SL_ERR_NO_MEM for failure.
 * Upon failure, no context will have been initialized. */
int gl_es2_initialize_context(void);

/* Returns the global context. If one has not been initialized, initialize it and
 * return it. If the initialization fails (no memory), set the error to the context
 * and return it anyway (so here, unlike gl_es2_initialize_context(), we will always
 * end up with a context returned no matter what, even if it is a failed context.) */
struct gl_es2_context *gl_es2_ctx(void);

void gl_es2_ctx_get_normalized_scissor_rect(struct gl_es2_context *c, uint32_t *left, uint32_t *top, uint32_t *right, uint32_t *bottom);

void gl_es2_framebuffer_attachment_init(struct gl_es2_framebuffer *fb, struct gl_es2_framebuffer_attachment *fa);
void gl_es2_framebuffer_attachment_cleanup(struct gl_es2_framebuffer_attachment *fa);
void gl_es2_framebuffer_attachment_detach(struct gl_es2_framebuffer_attachment *fa);
void gl_es2_framebuffer_attachment_attach_texture(struct gl_es2_framebuffer_attachment *fa, struct gl_es2_texture *tex);
void gl_es2_framebuffer_attachment_attach_renderbuffer(struct gl_es2_framebuffer_attachment *fa, struct gl_es2_renderbuffer *rb);
void gl_es2_framebuffer_attachment_raw_ptr(struct gl_es2_framebuffer_attachment *fa, void **prawptr, size_t *pstride);
struct sampler_2d *gl_es2_framebuffer_attachment_get_texture_sampler_2d(struct gl_es2_framebuffer_attachment *fa);

void gl_es2_program_shader_attachment_init(struct gl_es2_program *prog, struct gl_es2_program_shader_attachment *psa);
void gl_es2_program_shader_attachment_cleanup(struct gl_es2_program_shader_attachment *psa);
void gl_es2_program_shader_attachment_detach(struct gl_es2_program_shader_attachment *psa);
void gl_es2_program_shader_attachment_attach(struct gl_es2_program_shader_attachment *psa, struct gl_es2_shader *shad);

void gl_es2_framebuffer_init(struct gl_es2_framebuffer *fb);
void gl_es2_framebuffer_cleanup(struct gl_es2_framebuffer *fb);
int gl_es2_framebuffer_get_dims(struct gl_es2_framebuffer *fb, int *pwidth, int *pheight);
int gl_es2_framebuffer_get_bitmap_row_num(struct gl_es2_framebuffer *fa, int y);
enum gl_es2_framebuffer_completeness gl_es2_framebuffer_check_completeness(struct gl_es2_framebuffer *fb);

void gl_es2_renderbuffer_init(struct gl_es2_renderbuffer *rb);
void gl_es2_renderbuffer_cleanup(struct gl_es2_renderbuffer *rb);

void gl_es2_texture_init(struct gl_es2_texture *tex);
void gl_es2_texture_cleanup(struct gl_es2_texture *tex);
struct sampler_2d *gl_es2_texture_get_sampler_2d_for_cube_map_face(struct gl_es2_texture *tex, enum gl_es2_cube_map_face cube_map_face);

void gl_es2_buffer_init(struct gl_es2_buffer *buf);
void gl_es2_buffer_cleanup(struct gl_es2_buffer *buf);

void gl_es2_program_init(struct gl_es2_program *prog);
void gl_es2_program_cleanup(struct gl_es2_program *prog);

void gl_es2_shader_init(struct gl_es2_shader *shad);
void gl_es2_shader_cleanup(struct gl_es2_shader *shad);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GL_ES2_CONTEXT_H */
