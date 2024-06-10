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

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED
#include <math.h>
#endif

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef PRIMITIVE_ASSEMBLY_H_INCLUDED
#define PRIMITIVE_ASSEMBLY_H_INCLUDED
#include "primitive_assembly.h"
#endif

#ifndef ATTRIB_SET_H_INCLUDED
#define ATTRIB_SET_H_INCLUDED
#include "attrib_set.h"
#endif

#ifndef DATA_BUFFER_H_INCLUDED
#define DATA_BUFFER_H_INCLUDED
#include "data_buffer.h"
#endif

#ifndef ATTRIB_ROUTING_H_INCLUDED
#define ATTRIB_ROUTING_H_INCLUDED
#include "attrib_routing.h"
#endif

#ifndef CLIPPING_STAGE_H_INCLUDED
#define CLIPPING_STAGE_H_INCLUDED
#include "clipping_stage.h"
#endif

#ifndef PERSPECTIVE_DIVISION_H_INCLUDED
#define PERSPECTIVE_DIVISION_H_INCLUDED
#include "perspective_division.h"
#endif

#ifndef VIEWPORT_TRANSFORMATION_H_INCLUDED
#define VIEWPORT_TRANSFORMATION_H_INCLUDED
#include "viewport_transformation.h"
#endif

#ifndef RASTERIZER_H_INCLUDED
#define RASTERIZER_H_INCLUDED
#include "rasterizer.h"
#endif

#ifndef FRAGMENT_BUFFER_H_INCLUDED
#define FRAGMENT_BUFFER_H_INCLUDED
#include "fragment_buffer.h"
#endif

#ifndef BLEND_H_INCLUDED
#define BLEND_H_INCLUDED
#include "blend.h"
#endif

#ifndef SL_SHADER_H_INCLUDED
#define SL_SHADER_H_INCLUDED
#include "sl_shader.h"
#endif

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
#endif

#ifndef SL_REG_ALLOC_H_INCLUDED
#define SL_REG_ALLOC_H_INCLUDED
#include "sl_reg_alloc.h"
#endif

#ifndef GL_ES2_CONTEXT_H_INCLUDED
#define GL_ES2_CONTEXT_H_INCLUDED
#include "gl_es2_context.h"
#endif

/* EMIT_TRIANGLE_BY_TRIANGLE - Set to non-zero to emit all fragments for a single triangle, 
 * and then execute the framebuffer. Set to zero to bundle up all fragments for multiple triangles
 * together for best efficiency. The former is useful for debugging. The latter is better for
 * performance. */
#define EMIT_TRIANGLE_BY_TRIANGLE 1

void primitive_assembly_init(struct primitive_assembly *pa) {
  pa->num_rows_ = 0;
  pa->num_cols_ = 0;
  pa->num_cols_allocated_ = 0;
  pa->column_descriptors_ = NULL;
  pa->num_vertex_indices_ = 0;
  memset(pa->vertex_indices_, 0, sizeof(pa->vertex_indices_));
  pa->index_at_ = 0;
  pa->continue_at_ = 0;
  pa->continue_from_fragments_ = 0;
}


void primitive_assembly_reset(struct primitive_assembly *pa) {
  pa->num_cols_ = 0;
  pa->num_rows_ = 0;
}

int primitive_assembly_add_column(struct primitive_assembly *pa,
                                  primitive_assembly_data_type_t data_type,
                                  int attrib_index,
                                  int attrib_element_index,
                                  int target_register) {
  if (pa->num_cols_ == pa->num_cols_allocated_) {
    size_t new_num_cols_allocated = pa->num_cols_allocated_ + pa->num_cols_allocated_ + 1;
    if (new_num_cols_allocated <= pa->num_cols_allocated_) {
      return -1;
    }
    struct primitive_assembly_column_descriptor *ncd = (struct primitive_assembly_column_descriptor *)realloc(pa->column_descriptors_, sizeof(struct primitive_assembly_column_descriptor) * new_num_cols_allocated);
    if (!ncd) {
      return -1;
    }
    pa->column_descriptors_ = ncd;
    pa->num_cols_allocated_ = new_num_cols_allocated;
  }
  int col = (int)pa->num_cols_++;
  pa->column_descriptors_[col].data_type_ = data_type;
  pa->column_descriptors_[col].attrib_index_ = attrib_index;
  pa->column_descriptors_[col].attrib_element_index_ = attrib_element_index;
  pa->column_descriptors_[col].register_ = target_register;
  return col;
}

void primitive_assembly_cleanup(struct primitive_assembly *pa) {
  if (pa->column_descriptors_) free(pa->column_descriptors_);
}

int primitive_assembly_elements_u8(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, const uint8_t *indices, size_t num_indices) {
  /* Note: primitive_assembly_elements_u8, primitive_assembly_elements_u16 and primitive_assembly_elements_u32 share pretty much the
   *       same code with a different index type. If you fix a bug here, chances are you'll need to fix it there as well. */
  switch (pa->continue_at_) {
    case 0:
      if (pam == PAM_POINTS) {
        for (;;) {
          while ((pa->index_at_ < num_indices) && 
                  (pa->num_vertex_indices_ < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if (pa->index_at_== num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_LINES) {
        for (;;) {
          while (((pa->index_at_ + 2) <= num_indices) &&
                 ((pa->num_vertex_indices_ + 2) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if ((pa->index_at_ + 2) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_LINE_STRIP) {
        // Need at least 2 inputs to start, then keep pushing out pair-wise.
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->num_vertex_indices_ + 2) <= (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_))) &&
                 ((pa->index_at_ + 2) <= num_indices)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_]; /* note we only advance once */
          }
          while (pa->num_vertex_indices_) {
            /* Yield result */
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if ((pa->index_at_ + 2) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_LINE_LOOP) {
        // Need at least 2 inputs to start, then keep pushing out pair-wise. Difference with PAM_LINE_STRIP is that
        // we will also pair up the last with the first line item.
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->num_vertex_indices_ + 2) <= (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_))) &&
                 ((pa->index_at_ + 2) <= num_indices)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_]; /* note we only advance once */
          }
          if (((pa->num_vertex_indices_ + 2) <= (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_))) &&
              ((pa->index_at_ + 1) <= num_indices) && (num_indices >= 2)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[0]; /* loop to start */
          }
          while (pa->num_vertex_indices_) {
            /* Yield result */
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if ((pa->index_at_ + 1) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_TRIANGLES) {
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->index_at_ + 3) <= num_indices) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_TRIANGLE_STRIP) {
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->index_at_ + 3) <= num_indices) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            if (pa->index_at_ & 0) {
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 1];
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 0];
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 2];
            }
            else {
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 0];
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 1];
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 2];
            }
            pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_TRIANGLE_FAN) {
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->index_at_ + 3) <= num_indices) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[0];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 1];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 2];
            pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else {
        /* Trouble at the mill, unknown value */
        assert(0 && "Unknown internal enum used");
        return 0;
      }
    default:
      assert(0 && "Unknown internal state");
      pa->continue_at_ = 0;
      return 0;
  }
}

int primitive_assembly_elements_u16(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, const uint16_t *indices, size_t num_indices) {
  /* Note: primitive_assembly_elements_u8, primitive_assembly_elements_u16 and primitive_assembly_elements_u32 share pretty much the
   *       same code with a different index type. If you fix a bug here, chances are you'll need to fix it there as well. */
  switch (pa->continue_at_) {
    case 0:
      if (pam == PAM_POINTS) {
        for (;;) {
          while ((pa->index_at_ < num_indices) && 
                  (pa->num_vertex_indices_ < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if (pa->index_at_== num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_LINES) {
        for (;;) {
          while (((pa->index_at_ + 2) <= num_indices) &&
                 ((pa->num_vertex_indices_ + 2) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if ((pa->index_at_ + 2) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_LINE_STRIP) {
        // Need at least 2 inputs to start, then keep pushing out pair-wise.
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->num_vertex_indices_ + 2) <= (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_))) &&
                 ((pa->index_at_ + 2) <= num_indices)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_]; /* note we only advance once */
          }
          while (pa->num_vertex_indices_) {
            /* Yield result */
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if ((pa->index_at_ + 2) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_LINE_LOOP) {
        // Need at least 2 inputs to start, then keep pushing out pair-wise. Difference with PAM_LINE_STRIP is that
        // we will also pair up the last with the first line item.
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->num_vertex_indices_ + 2) <= (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_))) &&
                 ((pa->index_at_ + 2) <= num_indices)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_]; /* note we only advance once */
          }
          if (((pa->num_vertex_indices_ + 2) <= (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_))) &&
              ((pa->index_at_ + 1) <= num_indices) && (num_indices >= 2)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[0]; /* loop to start */
          }
          while (pa->num_vertex_indices_) {
            /* Yield result */
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if ((pa->index_at_ + 1) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_TRIANGLES) {
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->index_at_ + 3) <= num_indices) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_TRIANGLE_STRIP) {
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->index_at_ + 3) <= num_indices) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            if (pa->index_at_ & 0) {
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 1];
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 0];
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 2];
            }
            else {
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 0];
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 1];
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 2];
            }
            pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_TRIANGLE_FAN) {
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->index_at_ + 3) <= num_indices) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[0];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 1];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 2];
            pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else {
        /* Trouble at the mill, unknown value */
        assert(0 && "Unknown internal enum used");
        return 0;
      }
    default:
      assert(0 && "Unknown internal state");
      pa->continue_at_ = 0;
      return 0;
  }
}

int primitive_assembly_elements_u32(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, const uint32_t *indices, size_t num_indices) {
  /* Note: primitive_assembly_elements_u8, primitive_assembly_elements_u16 and primitive_assembly_elements_u32 share pretty much the
   *       same code with a different index type. If you fix a bug here, chances are you'll need to fix it there as well. */
  switch (pa->continue_at_) {
    case 0:
      if (pam == PAM_POINTS) {
        for (;;) {
          while ((pa->index_at_ < num_indices) && 
                  (pa->num_vertex_indices_ < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if (pa->index_at_== num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_LINES) {
        for (;;) {
          while (((pa->index_at_ + 2) <= num_indices) &&
                 ((pa->num_vertex_indices_ + 2) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if ((pa->index_at_ + 2) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_LINE_STRIP) {
        // Need at least 2 inputs to start, then keep pushing out pair-wise.
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->num_vertex_indices_ + 2) <= (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_))) &&
                 ((pa->index_at_ + 2) <= num_indices)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_]; /* note we only advance once */
          }
          while (pa->num_vertex_indices_) {
            /* Yield result */
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if ((pa->index_at_ + 2) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_LINE_LOOP) {
        // Need at least 2 inputs to start, then keep pushing out pair-wise. Difference with PAM_LINE_STRIP is that
        // we will also pair up the last with the first line item.
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->num_vertex_indices_ + 2) <= (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_))) &&
                 ((pa->index_at_ + 2) <= num_indices)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_]; /* note we only advance once */
          }
          if (((pa->num_vertex_indices_ + 2) <= (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_))) &&
              ((pa->index_at_ + 1) <= num_indices) && (num_indices >= 2)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[0]; /* loop to start */
          }
          while (pa->num_vertex_indices_) {
            /* Yield result */
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if ((pa->index_at_ + 1) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_TRIANGLES) {
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->index_at_ + 3) <= num_indices) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_++];
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_TRIANGLE_STRIP) {
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->index_at_ + 3) <= num_indices) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            if (pa->index_at_ & 0) {
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 1];
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 0];
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 2];
            }
            else {
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 0];
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 1];
              pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 2];
            }
            pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_TRIANGLE_FAN) {
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->index_at_ + 3) <= num_indices) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[0];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 1];
            pa->vertex_indices_[pa->num_vertex_indices_++] = indices[pa->index_at_ + 2];
            pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > num_indices) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else {
        /* Trouble at the mill, unknown value */
        assert(0 && "Unknown internal enum used");
        return 0;
      }
    default:
      assert(0 && "Unknown internal state");
      pa->continue_at_ = 0;
      return 0;
  }
}

int primitive_assembly_elements_arrayed(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, size_t num_elements) {
  switch (pa->continue_at_) {
    case 0:
      pa->end_index_ = pa->index_at_ + num_elements;
      if (pam == PAM_POINTS) {
        for (;;) {
          while ((pa->index_at_ < pa->end_index_) &&
                 (pa->num_vertex_indices_ < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if (pa->index_at_== pa->end_index_) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_LINES) {
        for (;;) {
          while (((pa->index_at_ + 2) <= pa->end_index_) &&
                 ((pa->num_vertex_indices_ + 2) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_++;
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 2) > pa->end_index_) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_LINE_STRIP) {
        // Need at least 2 inputs to start, then keep pushing out pair-wise.
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->num_vertex_indices_ + 2) <= (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_))) &&
                 ((pa->index_at_ + 2) <= pa->end_index_)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_++;
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_; /* note we only advance once */
          }
          while (pa->num_vertex_indices_) {
            /* Yield result */
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if ((pa->index_at_ + 2) > pa->end_index_) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_LINE_LOOP) {
        // Need at least 2 inputs to start, then keep pushing out pair-wise. Difference with PAM_LINE_STRIP is that
        // we will also pair up the last with the first line item.
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->num_vertex_indices_ + 2) <= (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_))) &&
                 ((pa->index_at_ + 2) <= pa->end_index_)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_++;
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_; /* note we only advance once */
          }
          if (((pa->num_vertex_indices_ + 2) <= (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_))) &&
              ((pa->index_at_ + 1) <= pa->end_index_) && (num_elements >= 2)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_++;
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)0; /* loop to start */
          }
          while (pa->num_vertex_indices_) {
            /* Yield result */
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if ((pa->index_at_ + 1) > pa->end_index_) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_TRIANGLES) {
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->index_at_ + 3) <= pa->end_index_) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_++;
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_++;
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > pa->end_index_) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_TRIANGLE_STRIP) {
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->index_at_ + 3) <= pa->end_index_) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            if (pa->index_at_ & 0) {
              pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_ + 1;
              pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_ + 0;
              pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_ + 2;
            }
            else {
              pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_ + 0;
              pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_ + 1;
              pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_ + 2;
            }
            pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > pa->end_index_) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_TRIANGLE_FAN) {
        pa->index_at_ = 0;
        for (;;) {
          while (((pa->index_at_ + 3) <= pa->end_index_) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = 0;
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_ + 1;
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_ + 2;
            pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > pa->end_index_) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else {
        /* Trouble at the mill, unknown value */
        assert(0 && "Unknown internal enum used");
        return 0;
      }
    default:
      assert(0 && "Unknown internal state");
      pa->continue_at_ = 0;
      return 0;
  }
}

int primitive_assembly_gather_attribs(struct primitive_assembly *pa, struct attrib_set *as, struct sl_execution *exec) {
  size_t col;
  size_t row;
  size_t num_rows = PRIMITIVE_ASSEMBLY_MAX_ROWS - pa->num_rows_;
  uint32_t * restrict indices = (uint32_t * restrict)pa->vertex_indices_;
  if (num_rows > pa->num_vertex_indices_) {
    num_rows = pa->num_vertex_indices_;
  }
  if (!num_rows) return !!pa->num_rows_;
  for (col = 0; col < pa->num_cols_; ++col) {
    struct primitive_assembly_column_descriptor *pacd = pa->column_descriptors_ + col;
    if (pacd->register_ == SL_REG_NONE) {
      /* no register assigned, therefore nothing to load. */
      continue;
    }
    if (pacd->attrib_index_ == -1) {
      /* Cannot gather attribs that are unbound */
      continue;
    }
    float *restrict pf = ((float * restrict)exec->float_regs_[pacd->register_]) + pa->num_rows_;
   
    struct attrib *attr = as->attribs_ + pacd->attrib_index_;

    if ((attr->size_ <= pacd->attrib_element_index_) ||
        !attr->enabled_) {
      float sf = attr->generic_values_[pacd->attrib_element_index_];
      for (row = 0; row < num_rows; ++row) {
        *pf++ = sf;
      }
    }
    else {
      void *p;
      if (attr->buf_) {
        p = ((uint8_t *)attr->buf_->data_) + (uintptr_t)attr->ptr_;
      }
      else {
        p = attr->ptr_;
      }

      switch (attr->data_type_) {
        case ADT_BYTE: {
          p = ((int8_t *)p) + pacd->attrib_element_index_;
          int8_t *restrict pi8;
          if (!attr->normalize_) {
            if (attr->stride_ != sizeof(int8_t)) {
              size_t stride = attr->stride_;
              for (row = 0; row < num_rows; ++row) {
                pi8 = (int8_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                *pf++ = (float)*pi8;
              }
            }
            else {
              pi8 = (int8_t *restrict)p;
              for (row = 0; row < num_rows; ++row) {
                *pf++ = (float)pi8[indices[row]];
              }
            }
          }
          else /* attr->normalize to 0..1 */ {
            /* this is not ideal but prevents compilers converting it into divison (testing
              * on godbolt.org suggests the average compiler is not "sufficiently smart".) */
            float norm = 2.f/255;
            if (attr->stride_ != sizeof(int8_t)) {
              size_t stride = attr->stride_;
              for (row = 0; row < num_rows; ++row) {
                pi8 = (int8_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                *pf++ = (((float)*pi8) + 128.f) * norm - 1.f;
              }
            }
            else {
              pi8 = (uint8_t *restrict)p;
              for (row = 0; row < num_rows; ++row) {
                *pf++ = ((float)pi8[indices[row]] + 128.f) * norm - 1.f;
              }
            }
          }
          break;
        }
        case ADT_UNSIGNED_BYTE: {
          p = ((uint8_t *)p) + pacd->attrib_element_index_;
          uint8_t *restrict pu8;
          if (!attr->normalize_) {
            if (attr->stride_ != sizeof(uint8_t)) {
              size_t stride = attr->stride_;
              for (row = 0; row < num_rows; ++row) {
                pu8 = (uint8_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                *pf++ = (float)*pu8;
              }
            }
            else {
              pu8 = (uint8_t *restrict)p;
              for (row = 0; row < num_rows; ++row) {
                *pf++ = (float)pu8[indices[row]];
              }
            }
          }
          else /* attr->normalize to 0..1 */ {
            /* this is not ideal but prevents compilers converting it into divison (testing
              * on godbolt.org suggests the average compiler is not "sufficiently smart".) */
            float norm = 1.f/255;
            if (attr->stride_ != sizeof(uint8_t)) {
              size_t stride = attr->stride_;
              for (row = 0; row < num_rows; ++row) {
                pu8 = (uint8_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                *pf++ = ((float)*pu8) * norm;
              }
            }
            else {
              pu8 = (uint8_t *restrict)p;
              for (row = 0; row < num_rows; ++row) {
                *pf++ = ((float)pu8[indices[row]]) * norm;
              }
            }
          }
          break;
        }
        case ADT_SHORT: {
          p = ((int16_t *)p) + pacd->attrib_element_index_;
          int16_t *restrict pi16;
          if (!attr->normalize_) {
            if (attr->stride_ != sizeof(int16_t)) {
              size_t stride = attr->stride_;
              for (row = 0; row < num_rows; ++row) {
                pi16 = (int16_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                *pf++ = (float)*pi16;
              }
            }
            else {
              pi16 = (int16_t *restrict)p;
              for (row = 0; row < num_rows; ++row) {
                *pf++ = (float)pi16[indices[row]];
              }
            }
          }
          else /* attr->normalize to 0..1 */ {
            /* this is not ideal but prevents compilers converting it into divison (testing
              * on godbolt.org suggests the average compiler is not "sufficiently smart".) */
            float norm = 2.f/65535;
            if (attr->stride_ != sizeof(int16_t)) {
              size_t stride = attr->stride_;
              for (row = 0; row < num_rows; ++row) {
                pi16 = (int16_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                *pf++ = (((float)*pi16) + 32768.f) * norm - 1.f;
              }
            }
            else {
              pi16 = (uint16_t *restrict)p;
              for (row = 0; row < num_rows; ++row) {
                *pf++ = ((float)pi16[indices[row]] + 32768.f) * norm - 1.f;
              }
            }
          }
          break;
        }
        case ADT_UNSIGNED_SHORT: {
          p = ((uint16_t *)p) + pacd->attrib_element_index_;
          uint16_t *restrict pu16;
          if (!attr->normalize_) {
            if (attr->stride_ != sizeof(uint16_t)) {
              size_t stride = attr->stride_;
              for (row = 0; row < num_rows; ++row) {
                pu16 = (uint16_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                *pf++ = (float)*pu16;
              }
            }
            else {
              pu16 = (uint16_t *restrict)p;
              for (row = 0; row < num_rows; ++row) {
                *pf++ = (float)pu16[indices[row]];
              }
            }
          }
          else /* attr->normalize to 0..1 */ {
            /* this is not ideal but prevents compilers converting it into divison (testing
              * on godbolt.org suggests the average compiler is not "sufficiently smart".) */
            float norm = 1.f/65535;
            if (attr->stride_ != sizeof(uint16_t)) {
              size_t stride = attr->stride_;
              for (row = 0; row < num_rows; ++row) {
                pu16 = (uint16_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                *pf++ = ((float)*pu16) * norm;
              }
            }
            else {
              pu16 = (uint16_t *restrict)p;
              for (row = 0; row < num_rows; ++row) {
                *pf++ = ((float)pu16[indices[row]]) * norm;
              }
            }
          }
          break;
        }
        case ADT_INT: {
          p = ((int32_t *)p) + pacd->attrib_element_index_;
          int32_t *restrict pi32;
          if (!attr->normalize_) {
            if (attr->stride_ != sizeof(int32_t)) {
              size_t stride = attr->stride_;
              for (row = 0; row < num_rows; ++row) {
                pi32 = (int32_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                *pf++ = (float)*pi32;
              }
            }
            else {
              pi32 = (int32_t *restrict)p;
              for (row = 0; row < num_rows; ++row) {
                *pf++ = (float)pi32[indices[row]];
              }
            }
          }
          else /* attr->normalize to 0..1 */ {
            /* this is not ideal but prevents compilers converting it into divison (testing
              * on godbolt.org suggests the average compiler is not "sufficiently smart".) */
            float norm = 2.f/65535;
            if (attr->stride_ != sizeof(int32_t)) {
              size_t stride = attr->stride_;
              for (row = 0; row < num_rows; ++row) {
                pi32 = (int32_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                *pf++ = (((float)*pi32) + 2147483648.f) * norm - 1.f;
              }
            }
            else {
              pi32 = (uint32_t *restrict)p;
              for (row = 0; row < num_rows; ++row) {
                *pf++ = ((float)pi32[indices[row]] +  2147483648.f) * norm - 1.f;
              }
            }
          }
          break;
        }
        case ADT_UNSIGNED_INT: {
          p = ((uint32_t *)p) + pacd->attrib_element_index_;
          uint32_t *restrict pu32;
          if (!attr->normalize_) {
            if (attr->stride_ != sizeof(uint32_t)) {
              size_t stride = attr->stride_;
              for (row = 0; row < num_rows; ++row) {
                pu32 = (uint32_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                *pf++ = (float)*pu32;
              }
            }
            else {
              pu32 = (uint32_t *restrict)p;
              for (row = 0; row < num_rows; ++row) {
                *pf++ = (float)pu32[indices[row]];
              }
            }
          }
          else /* attr->normalize to 0..1 */ {
            /* this is not ideal but prevents compilers converting it into divison (testing
              * on godbolt.org suggests the average compiler is not "sufficiently smart".) */
            float norm = (float)(1./4294967295.);
            if (attr->stride_ != sizeof(uint32_t)) {
              size_t stride = attr->stride_;
              for (row = 0; row < num_rows; ++row) {
                pu32 = (uint32_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                *pf++ = ((float)*pu32) * norm;
              }
            }
            else {
              pu32 = (uint32_t *restrict)p;
              for (row = 0; row < num_rows; ++row) {
                *pf++ = ((float)pu32[indices[row]]) * norm;
              }
            }
          }
          break;
        }
        case ADT_FIXED: {
          p = ((int32_t *)p) + pacd->attrib_element_index_;
          int32_t *restrict pfixed;
          if (attr->stride_ != sizeof(int32_t)) {
            size_t stride = attr->stride_;
            for (row = 0; row < num_rows; ++row) {
              pfixed = (int32_t *restrict)(((uint8_t *)p) + stride * indices[row]);
              *pf++ = ((float)*pfixed) / 65536.f;
            }
          }
          else {
            pfixed = (int32_t *restrict)p;
            for (row = 0; row < num_rows; ++row) {
              *pf++ = ((float)(pfixed[indices[row]])) / 65536.f;
            }
          }

          break;
        }
        case ADT_FLOAT: {
          p = ((float *)p) + pacd->attrib_element_index_;
          float *restrict psf;
          if (attr->stride_ != sizeof(float)) {
            size_t stride = attr->stride_;
            for (row = 0; row < num_rows; ++row) {
              psf = (float *restrict)(((uint8_t *)p) + stride * indices[row]);
              *pf++ = *psf;
            }
          }
          else {
            psf = (float *restrict)p;
            for (row = 0; row < num_rows; ++row) {
              *pf++ = psf[ indices[row] ];
            }
          }
          break;
        }
        default:
          assert(0 && "Invalid internal data type");
      }
    }
  }
  pa->num_rows_ += num_rows;
  
  if (pa->num_vertex_indices_ > num_rows) {
    memcpy(pa->vertex_indices_, pa->vertex_indices_ + num_rows, num_rows - pa->num_vertex_indices_);
  }
  pa->num_vertex_indices_ -= num_rows;

  return !!pa->num_rows_;
}

int primitive_assembly_process_primitives(struct primitive_assembly *pa,
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
                                          const void *indices) {
  int r;
  struct sl_variable *vgl_Position = NULL;
  struct sl_function *vmain = NULL;
  struct sl_variable *fgl_FragCoord = NULL;

  float *v0 = NULL, *v1 = NULL, *v2 = NULL;
  int32_t offset_factor_f8 = 0;
  int32_t offset_units_f8 = 0;

  int32_t sx0 = 0, sy0 = 0, sz0 = 0, sx1 = 0, sy1 = 0, sz1 = 0, sx2 = 0, sy2 = 0, sz2 = 0;

  uint32_t norm_scissor_left = 0, norm_scissor_top = 0, norm_scissor_right = 0, norm_scissor_bottom = 0;
  size_t pa_row_index = 0;
  size_t clip_tri_idx = 0;
  size_t prior_num_rows_in_fragbuf = 0;
  size_t line_seg_tri_index = 0;
  int line_dimension = 0;
  int orientation = 0;

  if (pa->continue_from_fragments_) {
    vgl_Position = pa->vgl_Position_;
    vmain = pa->vmain_;
    fgl_FragCoord = pa->fgl_FragCoord_;
    v0 = pa->v0_;
    v1 = pa->v1_;
    v2 = pa->v2_;
    offset_factor_f8 = pa->offset_factor_f8_;
    offset_units_f8 = pa->offset_units_f8_;
    sx0 = pa->sx0_;
    sy0 = pa->sy0_;
    sz0 = pa->sz0_;
    sx1 = pa->sx1_;
    sy1 = pa->sy1_;
    sz1 = pa->sz1_;
    sx2 = pa->sx2_;
    sy2 = pa->sy2_;
    sz2 = pa->sz2_;
    norm_scissor_left = pa->norm_scissor_left_;
    norm_scissor_top = pa->norm_scissor_top_;
    norm_scissor_right = pa->norm_scissor_right_;
    norm_scissor_bottom = pa->norm_scissor_bottom_;
    pa_row_index = pa->pa_row_index_;
    clip_tri_idx = pa->clip_tri_index_;
    prior_num_rows_in_fragbuf = pa->prior_num_rows_in_fragbuf_;
    line_seg_tri_index = pa->line_seg_tri_index_;
    line_dimension = pa->primary_dimension_;
    orientation = pa->last_orientation_;
    switch (pa->continue_from_fragments_) {
      case 1:
        goto continue_from_fragments;
      case 2:
        goto continue_from_tail_end;
      case 3:
        goto continue_from_lineseg_fragments;
      case 4:
        goto continue_from_single_triangle;
    }
    
  }

  vgl_Position = sl_compilation_unit_find_variable(&vertex_shader->cu_, "gl_Position");
  vmain = sl_compilation_unit_find_function(&vertex_shader->cu_, "main");
  fgl_FragCoord = sl_compilation_unit_find_variable(&fragment_shader->cu_, "gl_FragCoord");

  /* Normalize scissor rect from "bottom-left positive-y is up" coordinate system to 
   * "top-left positive-y is down" coordinate system that rasterizer expects.
   * Clamp to the actual screen while doing so. */
  norm_scissor_left = (scissor_left >= 0) ? (uint32_t)scissor_left : 0;
  if (scissor_bottom_counted_from_bottom <= (int32_t)screen_height) {
    norm_scissor_bottom = (scissor_bottom_counted_from_bottom >= 0) ? (screen_height - scissor_bottom_counted_from_bottom) : screen_height;
  }
  else {
    norm_scissor_bottom = 0;
  }
  if ((scissor_bottom_counted_from_bottom + scissor_height) <= (int32_t)screen_height) {
    norm_scissor_top = ((scissor_bottom_counted_from_bottom + scissor_height) >= 0) ? (screen_height - (scissor_bottom_counted_from_bottom + scissor_height)) : screen_height;
  }
  else {
    norm_scissor_top = 0;
  }
  norm_scissor_right = ((scissor_left + scissor_width) >= 0) ? (uint32_t)(scissor_left + scissor_width) : 0;

  if ((norm_scissor_left >= norm_scissor_right) || (norm_scissor_top >= norm_scissor_bottom)) {
    /* scissor rect holding no space; no pixel can be emitted, draw call is no-op. */
    return 0;
  }

  pa->index_at_ = indices ? 0 : arrayed_starting_index;

  /* Convert polygon offset values to 8-bit fixed point */
  offset_factor_f8 = (int32_t)(offset_factor * 256.f);
  offset_units_f8 = (int32_t)(offset_units * 256.f);

  for (;;) {
    if (indices) {
      switch (index_type) {
        case PAIT_UNSIGNED_BYTE:
          r = primitive_assembly_elements_u8(pa, as, mode, indices, num_elements);
          break;
        case PAIT_UNSIGNED_SHORT:
          r = primitive_assembly_elements_u16(pa, as, mode, indices, num_elements);
          break;
        case PAIT_UNSIGNED_INT:
          r = primitive_assembly_elements_u32(pa, as, mode, indices, num_elements);
          break;
      }
    }
    else {
      r = primitive_assembly_elements_arrayed(pa, as, mode, num_elements);
    }

    if (!r) {
      /* No more primitives to process */
      break;
    }

    while (primitive_assembly_gather_attribs(pa, as, &vertex_shader->exec_)) {

      /* Set up execution chain */
      uint8_t * restrict exec_chain = vertex_shader->exec_.exec_chain_reg_;
      size_t exec_row;
      for (exec_row = 0; exec_row < (pa->num_rows_ - 1); ++exec_row) {
        exec_chain[exec_row] = 1;
      }
      exec_chain[pa->num_rows_ - 1] = 0;

      /* Run vertex shader */
      sl_exec_run(&vertex_shader->exec_, vmain, 0);

      for (pa_row_index = 0; pa_row_index < pa->num_rows_; /* advancement depends on mode */) {
        switch (mode) {
          case PAM_POINTS:
            // XXX: Handle points..
            pa_row_index++;
            break;
          case PAM_LINES:
          case PAM_LINE_STRIP:
          case PAM_LINE_LOOP: {
            float *iv0, *iv1;
            iv0 = cs->input_varyings_;
            iv1 = iv0 + cs->num_varyings_;
            iv0[CLIPPING_STAGE_IDX_X] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[0]])[pa_row_index];
            iv0[CLIPPING_STAGE_IDX_Y] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[1]])[pa_row_index];
            iv0[CLIPPING_STAGE_IDX_Z] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[2]])[pa_row_index];
            iv0[CLIPPING_STAGE_IDX_W] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[3]])[pa_row_index];

            iv1[CLIPPING_STAGE_IDX_X] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[0]])[pa_row_index + 1];
            iv1[CLIPPING_STAGE_IDX_Y] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[1]])[pa_row_index + 1];
            iv1[CLIPPING_STAGE_IDX_Z] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[2]])[pa_row_index + 1];
            iv1[CLIPPING_STAGE_IDX_W] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[3]])[pa_row_index + 1];

            size_t attrib_route_index;
            for (attrib_route_index = 0; attrib_route_index < ar->num_attribs_routed_; ++attrib_route_index) {
              iv0[CLIPPING_STAGE_IDX_GENERIC + attrib_route_index] =
                (vertex_shader->exec_.float_regs_[ar->attribs_routed_[attrib_route_index].from_source_reg_])[pa_row_index];
              iv1[CLIPPING_STAGE_IDX_GENERIC + attrib_route_index] =
                (vertex_shader->exec_.float_regs_[ar->attribs_routed_[attrib_route_index].from_source_reg_])[pa_row_index + 1];
            }

            pa_row_index += 2;

            if (clipping_stage_process_line(cs)) {
              perspective_division(2 * cs->num_triangles_in_b_, 
                                    cs->num_varyings_ - CLIPPING_STAGE_IDX_X, 
                                    cs->num_varyings_ * sizeof(float), 
                                    cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_X);

              viewport_transformation(vp_x, vp_y, vp_width, vp_height, depth_range_near, depth_range_far,
                                      screen_width, screen_height, max_z, 
                                      2 * cs->num_triangles_in_b_, 
                                      cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_X,
                                      cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_Y,
                                      cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_Z,
                                      cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_W,
                                      cs->num_varyings_ * sizeof(float),
                                      (int32_t *)cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_SX,
                                      (int32_t *)cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_SY,
                                      (int32_t *)cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_SZ,
                                      cs->num_varyings_ * sizeof(int32_t));

              v0 = cs->triangle_varyings_b_ + cs->num_varyings_ * clip_tri_idx;
              v1 = v0 + cs->num_varyings_;
              sx0 = *(int32_t *)(v0+CLIPPING_STAGE_IDX_SX);
              sy0 = *(int32_t *)(v0+CLIPPING_STAGE_IDX_SY);
              sz0 = *(int32_t *)(v0+CLIPPING_STAGE_IDX_SZ);
              sx1 = *(int32_t *)(v1+CLIPPING_STAGE_IDX_SX);
              sy1 = *(int32_t *)(v1+CLIPPING_STAGE_IDX_SY);
              sz1 = *(int32_t *)(v1+CLIPPING_STAGE_IDX_SZ);

              /* Rasterize line - each pixel has a diamond shape around it, if a line intersects that
               * diamond shape, the pixel is drawn. For edge cases, if a line "leaves" that diamond shape,
               * the pixel is drawn.
               * For implementation, to determine if a line intersects the pixel, we recognize that the diamond
               * is a fairly regular convex shape. So instead of intersecting a line with diamonds, we extrude
               * a diamond shape on our line and if that diamond shape intersects the pixel center, the pixel is
               * drawn. Benefit of doing it that way is we can use the triangle rasterizer to ensure exactness
               * of pixel coverage and keep things fully integer for determinism. Downside is that we'll be 
               * emitting 4 triangles per line segment and we kind of suck doing many small triangles fast.
               * Note that, to ensure we only cover a pixel when the line segment "leaves" the diamond shape,
               * our extrusion ends with an empty diamond, not a solid diamond, for a line from A to B, going
               * to the right:
               *    ________________________
               *   /                       /
               *  /                       /
               * /  A                    /  B
               * \                       \
               *  \                       \
               *   \_______________________\
               * Note how, at B, the point B is not covered by the segment, but instead by an "empty" diamond.
               * Also note that the direction of the line matters here, if the line were to start at B and
               * end to the left at A, we would have:
               * _____                  ______
               * \    \   instead of   /     /
               * /____/                \_____\
               */

              int32_t *v = pa->line_segment_triangulation_;

              /*        \    |    /
               *         \ F | G /
               *          \  |  /
               *       E   \ | /   H
               *            \|/
               *  -----------+----------->
               *            /|\   
               *       D   / | \   A
               *          /  |  \
               *         / C | B \
               *        /    |    \
               *             v
               *  +-> +x
               *  |
               *  v 
               *    +y
               */
              const int32_t diamond_diameter = 1 * (int32_t)(1 << (RASTERIZER_SUBPIXEL_BITS - 1));
              if (sx0 >= sx1) {
                int32_t dx = sx0 - sx1;

                if (sy0 >= sy1) {
                  int32_t dy = sy0 - sy1;

                  if (dx >= dy) {
                    /* D
                     * ______
                     * \1    \0
                     * /_____/
                     */
                    line_dimension = 0;
                    v[0] = sx1 + diamond_diameter;
                    v[1] = sy1;
                    v[2] = sz1;
                    v[3] = sx1;
                    v[4] = sy1 - diamond_diameter;
                    v[5] = sz1;
                    v[6] = sx0;
                    v[7] = sy0 - diamond_diameter;
                    v[8] = sz0;
                    v += 9;
                    v[0] = sx1 + diamond_diameter;
                    v[1] = sy1;
                    v[2] = sz1;
                    v[3] = sx0;
                    v[4] = sy0 - diamond_diameter;
                    v[5] = sz0;
                    v[6] = sx0 + diamond_diameter;
                    v[7] = sy0;
                    v[8] = sz0;
                    v += 9;
                    v[0] = sx1;
                    v[1] = sy1 + diamond_diameter;
                    v[2] = sz1;
                    v[3] = sx1 + diamond_diameter;
                    v[4] = sy1;
                    v[5] = sz1;
                    v[6] = sx0 + diamond_diameter;
                    v[7] = sy0;
                    v[8] = sz0;
                    v += 9;
                    v[0] = sx1;
                    v[1] = sy1 + diamond_diameter;
                    v[2] = sz1;
                    v[3] = sx0 + diamond_diameter;
                    v[4] = sy0;
                    v[5] = sz0;
                    v[6] = sx0;
                    v[7] = sy0 + diamond_diameter;
                    v[8] = sz0;
                  }
                  else {
                    /* F
                     * |\/|
                     * |1 |
                     * |  |
                     *  \/
                     *   0
                     */
                    line_dimension = 1;
                    v[0] = sx1 - diamond_diameter;
                    v[1] = sy1;
                    v[2] = sz1;
                    v[3] = sx1;
                    v[4] = sy1 + diamond_diameter;
                    v[5] = sz1;
                    v[6] = sx0 - diamond_diameter;
                    v[7] = sy0;
                    v[8] = sz0;
                    v += 9;
                    v[0] = sx0 - diamond_diameter;
                    v[1] = sy0;
                    v[2] = sz0;
                    v[3] = sx1;
                    v[4] = sy1 + diamond_diameter;
                    v[5] = sz1;
                    v[6] = sx0;
                    v[7] = sy0 + diamond_diameter;
                    v[8] = sz0;
                    v += 9;
                    v[0] = sx1;
                    v[1] = sy1 + diamond_diameter;
                    v[2] = sz1;
                    v[3] = sx1 + diamond_diameter;
                    v[4] = sy1;
                    v[5] = sz1;
                    v[6] = sx0 + diamond_diameter;
                    v[7] = sy0;
                    v[8] = sz0;
                    v += 9;
                    v[0] = sx0 + diamond_diameter;
                    v[1] = sy0;
                    v[2] = sz0;
                    v[3] = sx0;
                    v[4] = sy0 + diamond_diameter;
                    v[5] = sz0;
                    v[6] = sx1;
                    v[7] = sy1 + diamond_diameter;
                    v[8] = sz1;
                  }
                }
                else /* (sy0 < sy1) */ {
                  int32_t dy = sy1 - sy0;

                  if (dx >= dy) {
                    /* E 
                     * ______
                     * \1    \0
                     * /_____/
                     */
                    line_dimension = 0;
                    v[0] = sx1 + diamond_diameter;
                    v[1] = sy1;
                    v[2] = sz1;
                    v[3] = sx1;
                    v[4] = sy1 - diamond_diameter;
                    v[5] = sz1;
                    v[6] = sx0;
                    v[7] = sy0 - diamond_diameter;
                    v[8] = sz0;
                    v += 9;
                    v[0] = sx1 + diamond_diameter;
                    v[1] = sy1;
                    v[2] = sz1;
                    v[3] = sx0;
                    v[4] = sy0 - diamond_diameter;
                    v[5] = sz0;
                    v[6] = sx0 + diamond_diameter;
                    v[7] = sy0;
                    v[8] = sz0;
                    v += 9;
                    v[0] = sx1;
                    v[1] = sy1 + diamond_diameter;
                    v[2] = sz1;
                    v[3] = sx1 + diamond_diameter;
                    v[4] = sy1;
                    v[5] = sz1;
                    v[6] = sx0 + diamond_diameter;
                    v[7] = sy0;
                    v[8] = sz0;
                    v += 9;
                    v[0] = sx1;
                    v[1] = sy1 + diamond_diameter;
                    v[2] = sz1;
                    v[3] = sx0 + diamond_diameter;
                    v[4] = sy0;
                    v[5] = sz0;
                    v[6] = sx0;
                    v[7] = sy0 + diamond_diameter;
                    v[8] = sz0;
                  }
                  else {
                    /* C
                     *    0
                     *   /\
                     *  |  |
                     *  |  |
                     *  |/\|
                     *    1
                     */
                    line_dimension = 1;
                    v[0] = sx1 - diamond_diameter;
                    v[1] = sy1;
                    v[2] = sz1;
                    v[3] = sx0 - diamond_diameter;
                    v[4] = sy0;
                    v[5] = sz0;
                    v[6] = sx0;
                    v[7] = sy0 - diamond_diameter;
                    v[8] = sz0;
                    v += 9;
                    v[0] = sx1 - diamond_diameter;
                    v[1] = sy1;
                    v[2] = sz1;
                    v[3] = sx0;
                    v[4] = sy0 - diamond_diameter;
                    v[5] = sz0;
                    v[6] = sx1;
                    v[7] = sy1 - diamond_diameter;
                    v[8] = sz1;
                    v += 9;
                    v[0] = sx1;
                    v[1] = sy1 - diamond_diameter;
                    v[2] = sz1;
                    v[3] = sx0;
                    v[4] = sy0 - diamond_diameter;
                    v[5] = sz0;
                    v[6] = sx0 + diamond_diameter;
                    v[7] = sy0;
                    v[8] = sz0;
                    v += 9;
                    v[0] = sx1;
                    v[1] = sy1 - diamond_diameter;
                    v[2] = sz1;
                    v[3] = sx0 + diamond_diameter;
                    v[4] = sy0;
                    v[5] = sz0;
                    v[6] = sx1 + diamond_diameter;
                    v[7] = sy1;
                    v[8] = sz1;
                  }
                }
              }
              else if (sy0 >= sy1) {
                int32_t dx = sx1 - sx0;
                int32_t dy = sy0 - sy1;
                if (dx >= dy) {
                  /* H
                   *  _____
                   * /0   /1
                   * \____\
                   */
                  line_dimension = 0;
                  v[0] = sx0 - diamond_diameter;
                  v[1] = sy0;
                  v[2] = sz0;
                  v[3] = sx0;
                  v[4] = sy0 - diamond_diameter;
                  v[5] = sz0;
                  v[6] = sx1;
                  v[7] = sy1 - diamond_diameter;
                  v[8] = sz1;
                  v += 9;
                  v[0] = sx0 - diamond_diameter;
                  v[1] = sy0;
                  v[2] = sz0;
                  v[3] = sx1;
                  v[4] = sy1 - diamond_diameter;
                  v[5] = sz1;
                  v[6] = sx1 - diamond_diameter;
                  v[7] = sy1;
                  v[8] = sz1;
                  v += 9;
                  v[0] = sx0 - diamond_diameter;
                  v[1] = sy0;
                  v[2] = sz0;
                  v[3] = sx1 - diamond_diameter;
                  v[4] = sy1;
                  v[5] = sz1;
                  v[6] = sx0;
                  v[7] = sy0 + diamond_diameter;
                  v[8] = sz0;
                  v += 9;
                  v[0] = sx0;
                  v[1] = sy0 + diamond_diameter;
                  v[2] = sz0;
                  v[3] = sx1 - diamond_diameter;
                  v[4] = sy1;
                  v[5] = sz1;
                  v[6] = sx1;
                  v[7] = sy1 + diamond_diameter;
                  v[8] = sz1;
                }
                else {
                  /* G
                   *   1
                   * |\/|
                   * |  |
                   * | 0|
                   *  \/
                   */
                  line_dimension = 1;
                  v[0] = sx1 - diamond_diameter;
                  v[1] = sy1;
                  v[2] = sz1;
                  v[3] = sx1;
                  v[4] = sy1 + diamond_diameter;
                  v[5] = sz1;
                  v[6] = sx0 - diamond_diameter;
                  v[7] = sy0;
                  v[8] = sz0;
                  v += 9;
                  v[0] = sx0 - diamond_diameter;
                  v[1] = sy0;
                  v[2] = sz0;
                  v[3] = sx1;
                  v[4] = sy1 + diamond_diameter;
                  v[5] = sz1;
                  v[6] = sx0;
                  v[7] = sy0 + diamond_diameter;
                  v[8] = sz0;
                  v += 9;
                  v[0] = sx1;
                  v[1] = sy1 + diamond_diameter;
                  v[2] = sz1;
                  v[3] = sx1 + diamond_diameter;
                  v[4] = sy1;
                  v[5] = sz1;
                  v[6] = sx0 + diamond_diameter;
                  v[7] = sy0;
                  v[8] = sz0;
                  v += 9;
                  v[0] = sx0 + diamond_diameter;
                  v[1] = sy0;
                  v[2] = sz0;
                  v[3] = sx0;
                  v[4] = sy0 + diamond_diameter;
                  v[5] = sz0;
                  v[6] = sx1;
                  v[7] = sy1 + diamond_diameter;
                  v[8] = sz1;
                }
              }
              else {
                int32_t dx = sx1 - sx0;
                int32_t dy = sy1 - sy0;
                if (dx >= dy) {
                  /* A
                   *  _____
                   * /0   /1
                   * \____\
                   */
                  line_dimension = 0;
                  v[0] = sx0 - diamond_diameter;
                  v[1] = sy0;
                  v[2] = sz0;
                  v[3] = sx0;
                  v[4] = sy0 - diamond_diameter;
                  v[5] = sz0;
                  v[6] = sx1;
                  v[7] = sy1 - diamond_diameter;
                  v[8] = sz1;
                  v += 9;
                  v[0] = sx0 - diamond_diameter;
                  v[1] = sy0;
                  v[2] = sz0;
                  v[3] = sx1;
                  v[4] = sy1 - diamond_diameter;
                  v[5] = sz1;
                  v[6] = sx1 - diamond_diameter;
                  v[7] = sy1;
                  v[8] = sz1;
                  v += 9;
                  v[0] = sx0 - diamond_diameter;
                  v[1] = sy0;
                  v[2] = sz0;
                  v[3] = sx1 - diamond_diameter;
                  v[4] = sy1;
                  v[5] = sz1;
                  v[6] = sx0;
                  v[7] = sy0 + diamond_diameter;
                  v[8] = sz0;
                  v += 9;
                  v[0] = sx0;
                  v[1] = sy0 + diamond_diameter;
                  v[2] = sz0;
                  v[3] = sx1 - diamond_diameter;
                  v[4] = sy1;
                  v[5] = sz1;
                  v[6] = sx1;
                  v[7] = sy1 + diamond_diameter;
                  v[8] = sz1;
                }
                else {
                  /* B
                   *   /\
                   *  | 0|
                   *  |  |
                   *  |/\|
                   *    1
                   */
                  line_dimension = 1;
                  v[0] = sx1 - diamond_diameter;
                  v[1] = sy1;
                  v[2] = sz1;
                  v[3] = sx0 - diamond_diameter;
                  v[4] = sy0;
                  v[5] = sz0;
                  v[6] = sx0;
                  v[7] = sy0 - diamond_diameter;
                  v[8] = sz0;
                  v += 9;
                  v[0] = sx1 - diamond_diameter;
                  v[1] = sy1;
                  v[2] = sz1;
                  v[3] = sx0;
                  v[4] = sy0 - diamond_diameter;
                  v[5] = sz0;
                  v[6] = sx1;
                  v[7] = sy1 - diamond_diameter;
                  v[8] = sz1;
                  v += 9;
                  v[0] = sx1;
                  v[1] = sy1 - diamond_diameter;
                  v[2] = sz1;
                  v[3] = sx0;
                  v[4] = sy0 - diamond_diameter;
                  v[5] = sz0;
                  v[6] = sx0 + diamond_diameter;
                  v[7] = sy0;
                  v[8] = sz0;
                  v += 9;
                  v[0] = sx1;
                  v[1] = sy1 - diamond_diameter;
                  v[2] = sz1;
                  v[3] = sx0 + diamond_diameter;
                  v[4] = sy0;
                  v[5] = sz0;
                  v[6] = sx1 + diamond_diameter;
                  v[7] = sy1;
                  v[8] = sz1;
                }
              }

              float one_over_weight;
              if (line_dimension) {
                one_over_weight = 1.f / (float)(sy1 - sy0);
              }
              else {
                one_over_weight = 1.f / (float)(sx1 - sx0);
              }

              size_t attrib_index;
              for (attrib_index = CLIPPING_STAGE_IDX_X; attrib_index < cs->num_varyings_; ++attrib_index) {
                v0[attrib_index] *= one_over_weight;
                v1[attrib_index] *= one_over_weight;
              }

              for (line_seg_tri_index = 0; line_seg_tri_index < 4; ++line_seg_tri_index) {

                prior_num_rows_in_fragbuf = fragbuf->num_rows_;

                while (0 != (orientation = rasterizer_triangle(ras, fragbuf, 
                                                               rgba, rgba_stride,     // bitmap
                                                               REZF_ALWAYS,
                                                               zbuf, zbuf_stride, zbuf_step,  // z-buffer
                                                               stencil_buf, stencil_stride, stencil_step,  // stencil buffer
                                                               norm_scissor_left, norm_scissor_top, 
                                                               norm_scissor_right, norm_scissor_bottom,  // scissor-rect
                                                               pa->line_segment_triangulation_[line_seg_tri_index * 9 + 0], pa->line_segment_triangulation_[line_seg_tri_index * 9 + 1], pa->line_segment_triangulation_[line_seg_tri_index * 9 + 2],
                                                               pa->line_segment_triangulation_[line_seg_tri_index * 9 + 3], pa->line_segment_triangulation_[line_seg_tri_index * 9 + 4], pa->line_segment_triangulation_[line_seg_tri_index * 9 + 5],
                                                               pa->line_segment_triangulation_[line_seg_tri_index * 9 + 6], pa->line_segment_triangulation_[line_seg_tri_index * 9 + 7], pa->line_segment_triangulation_[line_seg_tri_index * 9 + 8],
                                                               RASTERIZER_BOTH /* lines are unaffected by culling; and our triangulation should always be clockwise */,
                                                               offset_factor_f8, offset_units_f8))) {
                  size_t frag_row;
                  int frag_coord_x_reg = fgl_FragCoord->reg_alloc_.v_.regs_[0];
                  int frag_coord_y_reg = fgl_FragCoord->reg_alloc_.v_.regs_[1];
                  int frag_coord_z_reg = fgl_FragCoord->reg_alloc_.v_.regs_[2];
                  int frag_coord_w_reg = fgl_FragCoord->reg_alloc_.v_.regs_[3];
                  
                  if (frag_coord_x_reg != SL_REG_NONE) {
                    float * restrict x = (float * restrict)fragment_shader->exec_.float_regs_[frag_coord_x_reg];
                    for (frag_row = prior_num_rows_in_fragbuf; frag_row < fragbuf->num_rows_; ++frag_row) {
                      int32_t frag_xcoord = ((int32_t *)fragbuf->column_data_[FB_IDX_X_COORD])[frag_row];
                      
                      x[frag_row] = 0.5f + (float)frag_xcoord;
                    }
                  }
                  if (frag_coord_y_reg != SL_REG_NONE) {
                    float * restrict y = fragment_shader->exec_.float_regs_[frag_coord_y_reg];
                    for (frag_row = prior_num_rows_in_fragbuf; frag_row < fragbuf->num_rows_; ++frag_row) {
                      int32_t frag_ycoord = ((int32_t *)fragbuf->column_data_[FB_IDX_Y_COORD])[frag_row];

                      y[frag_row] = 0.5f + (float)frag_ycoord;
                    }
                  }
                  int primary_coord;
                  int32_t sp0, sp1;
                  if (line_dimension) {
                    primary_coord = FB_IDX_Y_COORD;
                    sp0 = sy0; sp1 = sy1;
                  }
                  else {
                    primary_coord = FB_IDX_X_COORD;
                    sp0 = sx0; sp1 = sx1;
                  }
                  if (frag_coord_z_reg != SL_REG_NONE) {
                    float * restrict z = fragment_shader->exec_.float_regs_[frag_coord_z_reg];
                    for (frag_row = prior_num_rows_in_fragbuf; frag_row < fragbuf->num_rows_; ++frag_row) {
                      int32_t frag_primary_coord = ((((int32_t *)fragbuf->column_data_[primary_coord])[frag_row]) << RASTERIZER_SUBPIXEL_BITS) + RASTERIZER_SUBPIXEL_BITS/2;
                      int32_t w0 = sp1 - frag_primary_coord;
                      int32_t w1 = frag_primary_coord - sp0;

                      z[frag_row] = (w0 * v0[CLIPPING_STAGE_IDX_Z]) + (w1 * v1[CLIPPING_STAGE_IDX_Z]);
                    }
                  }
                  if (frag_coord_w_reg != SL_REG_NONE) {
                    /* w reg will contain "one-over-w" */
                    float * restrict oow = fragment_shader->exec_.float_regs_[frag_coord_w_reg];
                    for (frag_row = prior_num_rows_in_fragbuf; frag_row < fragbuf->num_rows_; ++frag_row) {
                      int32_t frag_primary_coord = ((((int32_t *)fragbuf->column_data_[primary_coord])[frag_row]) << RASTERIZER_SUBPIXEL_BITS) + RASTERIZER_SUBPIXEL_BITS/2;
                      int32_t w0 = sp1 - frag_primary_coord;
                      int32_t w1 = frag_primary_coord - sp0;

                      oow[frag_row] = (w0 * v0[CLIPPING_STAGE_IDX_W]) + (w1 * v1[CLIPPING_STAGE_IDX_W]);
                    }
                  }

                  if (ar->num_attribs_routed_) {
                    float * restrict actual_w = (float * restrict)fragbuf->column_data_[FB_IDX_W];
                    float *restrict oow = fragment_shader->exec_.float_regs_[frag_coord_w_reg];
                    for (frag_row = prior_num_rows_in_fragbuf; frag_row < fragbuf->num_rows_; ++frag_row) {
                      actual_w[frag_row] = 1.f / oow[frag_row];
                    }
                    size_t attrib_route_index;
                    for (attrib_route_index = 0; attrib_route_index < ar->num_attribs_routed_; ++attrib_route_index) {
                      struct attrib_route *attr = ar->attribs_routed_ + attrib_route_index;
                      float * restrict tgt = fragment_shader->exec_.float_regs_[attr->to_target_reg_];
                      float * restrict oow = fragment_shader->exec_.float_regs_[frag_coord_w_reg];
                      float fv0 = v0[CLIPPING_STAGE_IDX_GENERIC + attrib_route_index];
                      float fv1 = v1[CLIPPING_STAGE_IDX_GENERIC + attrib_route_index];
                      for (frag_row = prior_num_rows_in_fragbuf; frag_row < fragbuf->num_rows_; ++frag_row) {
                        int32_t frag_primary_coord = ((((int32_t *)fragbuf->column_data_[primary_coord])[frag_row]) << RASTERIZER_SUBPIXEL_BITS) + RASTERIZER_SUBPIXEL_BITS/2;
                        int32_t w0 = sp1 - frag_primary_coord;
                        int32_t w1 = frag_primary_coord - sp0;
                        tgt[frag_row] = w0 * fv0 * actual_w[frag_row]
                                      + w1 * fv1 * actual_w[frag_row];
                      }
                    }
                  }

                  while ((fragbuf->num_rows_ == FRAGMENT_BUFFER_MAX_ROWS) ||
                         (fragbuf->num_rows_ && (fragbuf->fragment_orientation_ != orientation))) {
                     /* Full, or stalled because of an triangle - fragbuf orientation mismatch.
                      * Return to caller to process & clear fragbuf.
                      * Note that, while line segment orientation is always clockwise, there might be something
                      * inside the fragment buffer prior to the line segment, with a different orientation. */
                    pa->continue_from_fragments_ = 3;
                    goto return_for_continuation;
                  continue_from_lineseg_fragments:
                    pa->continue_from_fragments_ = 0;
                  }

                  prior_num_rows_in_fragbuf = fragbuf->num_rows_;
                }
              }
            }
            break;
          }
          case PAM_TRIANGLES:
          case PAM_TRIANGLE_STRIP:
          case PAM_TRIANGLE_FAN: {
            float *iv0, *iv1, *iv2;
            iv0 = cs->input_varyings_;
            iv1 = iv0 + cs->num_varyings_;
            iv2 = iv1 + cs->num_varyings_;
            iv0[CLIPPING_STAGE_IDX_X] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[0]])[pa_row_index];
            iv0[CLIPPING_STAGE_IDX_Y] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[1]])[pa_row_index];
            iv0[CLIPPING_STAGE_IDX_Z] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[2]])[pa_row_index];
            iv0[CLIPPING_STAGE_IDX_W] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[3]])[pa_row_index];

            iv1[CLIPPING_STAGE_IDX_X] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[0]])[pa_row_index + 1];
            iv1[CLIPPING_STAGE_IDX_Y] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[1]])[pa_row_index + 1];
            iv1[CLIPPING_STAGE_IDX_Z] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[2]])[pa_row_index + 1];
            iv1[CLIPPING_STAGE_IDX_W] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[3]])[pa_row_index + 1];

            iv2[CLIPPING_STAGE_IDX_X] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[0]])[pa_row_index + 2];
            iv2[CLIPPING_STAGE_IDX_Y] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[1]])[pa_row_index + 2];
            iv2[CLIPPING_STAGE_IDX_Z] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[2]])[pa_row_index + 2];
            iv2[CLIPPING_STAGE_IDX_W] = (vertex_shader->exec_.float_regs_[vgl_Position->reg_alloc_.v_.regs_[3]])[pa_row_index + 2];

            size_t attrib_route_index;
            for (attrib_route_index = 0; attrib_route_index < ar->num_attribs_routed_; ++attrib_route_index) {
              iv0[CLIPPING_STAGE_IDX_GENERIC + attrib_route_index] =
                (vertex_shader->exec_.float_regs_[ar->attribs_routed_[attrib_route_index].from_source_reg_])[pa_row_index + 0];
              iv1[CLIPPING_STAGE_IDX_GENERIC + attrib_route_index] =
                (vertex_shader->exec_.float_regs_[ar->attribs_routed_[attrib_route_index].from_source_reg_])[pa_row_index + 1];
              iv2[CLIPPING_STAGE_IDX_GENERIC + attrib_route_index] =
                (vertex_shader->exec_.float_regs_[ar->attribs_routed_[attrib_route_index].from_source_reg_])[pa_row_index + 2];
            }

            pa_row_index += 3;

            if (clipping_stage_process_triangle(cs)) {
              /* XXX: Ugly reliance on size here, we stitch the SX,SY,SZ variables alongside 
                *      the clipping output triangles, however the one is in floats, the other is in
                *      int32_t's. We assume sizeof(float) == sizeof(int32_t). Find some better way */
              assert(sizeof(float) == sizeof(int32_t));

              perspective_division(3 * cs->num_triangles_in_b_, 
                                    cs->num_varyings_ - CLIPPING_STAGE_IDX_X, 
                                    cs->num_varyings_ * sizeof(float), 
                                    cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_X);

              viewport_transformation(vp_x, vp_y, vp_width, vp_height, depth_range_near, depth_range_far,
                                      screen_width, screen_height, max_z, 
                                      3 * cs->num_triangles_in_b_, 
                                      cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_X,
                                      cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_Y,
                                      cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_Z,
                                      cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_W,
                                      cs->num_varyings_ * sizeof(float),
                                      (int32_t *)cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_SX,
                                      (int32_t *)cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_SY,
                                      (int32_t *)cs->triangle_varyings_b_ + CLIPPING_STAGE_IDX_SZ,
                                      cs->num_varyings_ * sizeof(int32_t));

              for (clip_tri_idx = 0; (clip_tri_idx + 2) < (3 * cs->num_triangles_in_b_); clip_tri_idx += 3) {
                v0 = cs->triangle_varyings_b_ + cs->num_varyings_ * clip_tri_idx;
                v1 = v0 + cs->num_varyings_;
                v2 = v1 + cs->num_varyings_;
                sx0 = *(int32_t *)(v0+CLIPPING_STAGE_IDX_SX);
                sy0 = *(int32_t *)(v0+CLIPPING_STAGE_IDX_SY);
                sz0 = *(int32_t *)(v0+CLIPPING_STAGE_IDX_SZ);
                sx1 = *(int32_t *)(v1+CLIPPING_STAGE_IDX_SX);
                sy1 = *(int32_t *)(v1+CLIPPING_STAGE_IDX_SY);
                sz1 = *(int32_t *)(v1+CLIPPING_STAGE_IDX_SZ);
                sx2 = *(int32_t *)(v2+CLIPPING_STAGE_IDX_SX);
                sy2 = *(int32_t *)(v2+CLIPPING_STAGE_IDX_SY);
                sz2 = *(int32_t *)(v2+CLIPPING_STAGE_IDX_SZ);

                int64_t D012 = rasterizer_compute_D012(sx0, sy0, sz0,
                                                       sx1, sy1, sz1,
                                                       sx2, sy2, sz2);
                float ooD012 = (float)(1. / D012);

                size_t attrib_index;
                for (attrib_index = CLIPPING_STAGE_IDX_X; attrib_index < cs->num_varyings_; ++attrib_index) {
                  v0[attrib_index] *= ooD012;
                  v1[attrib_index] *= ooD012;
                  v2[attrib_index] *= ooD012;
                }

                prior_num_rows_in_fragbuf = fragbuf->num_rows_;


                rasterizer_early_zbuf_func_t early_z_zbuf_func = REZF_ALWAYS;
                if (enable_zbuf_test && !enable_stencil_test) {
                  switch (zbuf_func) {
                    case PAZF_LESS:
                      early_z_zbuf_func = REZF_LESS;
                      break;
                    case PAZF_LEQUAL:
                      early_z_zbuf_func = REZF_LEQUAL;
                      break;
                  }
                }

                while (0 != (orientation = rasterizer_triangle(ras, fragbuf, 
                                                               rgba, rgba_stride,     // bitmap
                                                               early_z_zbuf_func,
                                                               zbuf, zbuf_stride, zbuf_step,  // z-buffer
                                                               stencil_buf, stencil_stride, stencil_step,  // stencil buffer
                                                               norm_scissor_left, norm_scissor_top, 
                                                               norm_scissor_right, norm_scissor_bottom,  // scissor-rect
                                                               sx0, sy0, sz0,
                                                               sx1, sy1, sz1,
                                                               sx2, sy2, sz2,
                                                               permitted_orientations,
                                                               offset_factor_f8, offset_units_f8))) {
                  if (orientation == fragbuf->fragment_orientation_) {
                    /* Run over all attributes to be filled in (including gl_FragCoord come to think of it),
                     * and fill out their coordinates. gl_FragCoord is special here as it's the only "attribute"
                     * that is not perspectively correct, but passed in raw form, prior to any division. */
                    size_t frag_row;
                    int frag_coord_x_reg = fgl_FragCoord->reg_alloc_.v_.regs_[0];
                    int frag_coord_y_reg = fgl_FragCoord->reg_alloc_.v_.regs_[1];
                    int frag_coord_z_reg = fgl_FragCoord->reg_alloc_.v_.regs_[2];
                    int frag_coord_w_reg = fgl_FragCoord->reg_alloc_.v_.regs_[3];
                    int64_t * restrict dp12 = (int64_t * restrict)fragbuf->column_data_[FB_IDX_DP12];
                    int64_t * restrict dp20 = (int64_t * restrict)fragbuf->column_data_[FB_IDX_DP20];
                    int64_t * restrict dp01 = (int64_t * restrict)fragbuf->column_data_[FB_IDX_DP01];

                    if (frag_coord_x_reg != SL_REG_NONE) {
                      float * restrict x = (float * restrict)fragment_shader->exec_.float_regs_[frag_coord_x_reg];
                      for (frag_row = prior_num_rows_in_fragbuf; frag_row < fragbuf->num_rows_; ++frag_row) {
                        x[frag_row] = 0.5f + (float)((int32_t *)fragbuf->column_data_[FB_IDX_X_COORD])[frag_row];
                      }
                    }
                    if (frag_coord_y_reg != SL_REG_NONE) {
                      float * restrict y = fragment_shader->exec_.float_regs_[frag_coord_y_reg];
                      for (frag_row = prior_num_rows_in_fragbuf; frag_row < fragbuf->num_rows_; ++frag_row) {
                        y[frag_row] = 0.5f + (float)((int32_t *)fragbuf->column_data_[FB_IDX_Y_COORD])[frag_row];
                      }
                    }
                    if (frag_coord_z_reg != SL_REG_NONE) {
                      float * restrict z = fragment_shader->exec_.float_regs_[frag_coord_z_reg];
                      for (frag_row = prior_num_rows_in_fragbuf; frag_row < fragbuf->num_rows_; ++frag_row) {
                        z[frag_row] = dp12[frag_row] * v0[CLIPPING_STAGE_IDX_Z]
                                    + dp20[frag_row] * v1[CLIPPING_STAGE_IDX_Z]
                                    + dp01[frag_row] * v2[CLIPPING_STAGE_IDX_Z];
                      }
                    }
                    if (frag_coord_w_reg != SL_REG_NONE) {
                      /* w reg will contain "one-over-w" */
                      float * restrict oow = fragment_shader->exec_.float_regs_[frag_coord_w_reg];
                      for (frag_row = prior_num_rows_in_fragbuf; frag_row < fragbuf->num_rows_; ++frag_row) {
                        oow[frag_row] = dp12[frag_row] * v0[CLIPPING_STAGE_IDX_W]
                                      + dp20[frag_row] * v1[CLIPPING_STAGE_IDX_W]
                                      + dp01[frag_row] * v2[CLIPPING_STAGE_IDX_W];
                      }
                    }

                    if (ar->num_attribs_routed_) {
                      float * restrict actual_w = (float * restrict)fragbuf->column_data_[FB_IDX_W];
                      float *restrict oow = fragment_shader->exec_.float_regs_[frag_coord_w_reg];
                      for (frag_row = prior_num_rows_in_fragbuf; frag_row < fragbuf->num_rows_; ++frag_row) {
                        actual_w[frag_row] = 1.f / oow[frag_row];
                      }
                      size_t attrib_route_index;
                      for (attrib_route_index = 0; attrib_route_index < ar->num_attribs_routed_; ++attrib_route_index) {
                        struct attrib_route *attr = ar->attribs_routed_ + attrib_route_index;
                        float * restrict tgt = fragment_shader->exec_.float_regs_[attr->to_target_reg_];
                        float fv0 = v0[CLIPPING_STAGE_IDX_GENERIC + attrib_route_index];
                        float fv1 = v1[CLIPPING_STAGE_IDX_GENERIC + attrib_route_index];
                        float fv2 = v2[CLIPPING_STAGE_IDX_GENERIC + attrib_route_index];
                        for (frag_row = prior_num_rows_in_fragbuf; frag_row < fragbuf->num_rows_; ++frag_row) {
                          tgt[frag_row] = dp12[frag_row] * fv0 * actual_w[frag_row]
                                        + dp20[frag_row] * fv1 * actual_w[frag_row]
                                        + dp01[frag_row] * fv2 * actual_w[frag_row];
                        }
                      }
                    }
                  }
              
                  while ((fragbuf->num_rows_ == FRAGMENT_BUFFER_MAX_ROWS) ||
                         (fragbuf->num_rows_ && (fragbuf->fragment_orientation_ != orientation))) {
                    /* Full, or stalled because of an triangle - fragbuf orientation mismatch.
                     * return to caller to process & clear fragbuf */
                    pa->continue_from_fragments_ = 1;
                    goto return_for_continuation;
continue_from_fragments:
                    pa->continue_from_fragments_ = 0;
                  }
                  prior_num_rows_in_fragbuf = fragbuf->num_rows_;
                }
                /* We have a triangle */
                if (EMIT_TRIANGLE_BY_TRIANGLE && fragbuf->num_rows_) {
                  while (fragbuf->num_rows_) {
                    pa->continue_from_fragments_ = 4;
                    goto return_for_continuation;
continue_from_single_triangle:
                    pa->continue_from_fragments_ = 0;
                  }
                }
              }
            }

            break;
          }
        }
      }
      pa->num_rows_ = 0;
    }
  }

  while (fragbuf->num_rows_) {
    pa->continue_from_fragments_ = 2;
    return 1;
continue_from_tail_end:
    pa->continue_from_fragments_ = 0;
  }

  return 0;

return_for_continuation:
  pa->vgl_Position_ = vgl_Position;
  pa->vmain_ = vmain;
  pa->fgl_FragCoord_ = fgl_FragCoord;
  pa->v0_ = v0;
  pa->v1_ = v1;
  pa->v2_ = v2;
  pa->offset_factor_f8_ = offset_factor_f8;
  pa->offset_units_f8_ = offset_units_f8;
  pa->sx0_ = sx0;
  pa->sy0_ = sy0;
  pa->sz0_ = sz0;
  pa->sx1_ = sx1;
  pa->sy1_ = sy1;
  pa->sz1_ = sz1;
  pa->sx2_ = sx2;
  pa->sy2_ = sy2;
  pa->sz2_ = sz2;
  pa->norm_scissor_left_ = norm_scissor_left;
  pa->norm_scissor_top_ = norm_scissor_top;
  pa->norm_scissor_right_ = norm_scissor_right;
  pa->norm_scissor_bottom_ = norm_scissor_bottom;
  pa->pa_row_index_ = pa_row_index;
  pa->clip_tri_index_ = clip_tri_idx;
  pa->prior_num_rows_in_fragbuf_ = prior_num_rows_in_fragbuf;
  pa->line_seg_tri_index_ = line_seg_tri_index;
  pa->primary_dimension_ = line_dimension;
  pa->last_orientation_ = orientation;
  return 1;
}

void primitive_assembly_process_fragments(struct primitive_assembly *pa,
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
                                          const void *indices,
                                          int dbg_overrule_rgb, uint8_t dbg_red, uint8_t dbg_grn, uint8_t dbg_blu) {
  struct sl_function *fmain = sl_compilation_unit_find_function(&fragment_shader->cu_, "main");
  struct sl_variable *fgl_FragColor = sl_compilation_unit_find_variable(&fragment_shader->cu_, "gl_FragColor");
  size_t frag_row;
  int orientation = fragbuf->fragment_orientation_;

  /* Set up execution chain */
  uint8_t * restrict frag_exec_chain = fragment_shader->exec_.exec_chain_reg_;
  size_t frag_exec_row;
  for (frag_exec_row = 0; frag_exec_row < (fragbuf->num_rows_ - 1); ++frag_exec_row) {
    frag_exec_chain[frag_exec_row] = 1;
  }
  frag_exec_chain[fragbuf->num_rows_ - 1] = 0;

  /* Run fragment shader */
  sl_exec_run(&fragment_shader->exec_, fmain, 0);
  
  /* Discarded fragments are in fragment_shader->exec_.execution_points_[0].alt_chain_
   * successful fragments are in fragment_shader->exec_.execution_points_[0].post_chain_ */

  /* Run over all discarded fragments and clear their corresponding mask (if any), to
   * prevent them from generating output. */
  /* XXX: Speed this up to take big steps (8 at a time) as per other primitives */
  uint32_t row = fragment_shader->exec_.execution_points_[0].alt_chain_;
  if (row != SL_EXEC_NO_CHAIN) {
    uint8_t delta;
    do {
      delta = fragment_shader->exec_.exec_chain_reg_[row];

      ((uint8_t * restrict)fragbuf->column_data_[FB_IDX_MASK])[row] = 0x00;

      row += delta;
    } while (delta);
  }

  /* Copy over fragment colors as determined by shader */
  /* XXX: Speed this up to take big steps (8 at a time) as per other primitives
   * XXX: Super common path, inline everything. */
  row = fragment_shader->exec_.execution_points_[0].post_chain_;
  if (row != SL_EXEC_NO_CHAIN) {
    uint8_t delta;
    float * restrict red_channel = fragment_shader->exec_.float_regs_[fgl_FragColor->reg_alloc_.v_.regs_[0] ];
    float * restrict green_channel = fragment_shader->exec_.float_regs_[fgl_FragColor->reg_alloc_.v_.regs_[1] ];
    float * restrict blue_channel = fragment_shader->exec_.float_regs_[fgl_FragColor->reg_alloc_.v_.regs_[2] ];
    float * restrict alpha_channel = fragment_shader->exec_.float_regs_[fgl_FragColor->reg_alloc_.v_.regs_[3] ];

    do {
      delta = fragment_shader->exec_.exec_chain_reg_[row];
      ((uint8_t * restrict)fragbuf->column_data_[FB_IDX_FRAG_RED])[row] = (uint8_t)(255.f * red_channel[row]);
      ((uint8_t * restrict)fragbuf->column_data_[FB_IDX_FRAG_GREEN])[row] = (uint8_t)(255.f * green_channel[row]);
      ((uint8_t * restrict)fragbuf->column_data_[FB_IDX_FRAG_BLUE])[row] = (uint8_t)(255.f * blue_channel[row]);
      ((uint8_t * restrict)fragbuf->column_data_[FB_IDX_FRAG_ALPHA])[row] = (uint8_t)(255.f * alpha_channel[row]);

      if (dbg_overrule_rgb) {
        ((uint8_t * restrict)fragbuf->column_data_[FB_IDX_FRAG_RED])[row] = dbg_red;
        ((uint8_t * restrict)fragbuf->column_data_[FB_IDX_FRAG_GREEN])[row] = dbg_grn;
        ((uint8_t * restrict)fragbuf->column_data_[FB_IDX_FRAG_BLUE])[row] = dbg_blu;
        ((uint8_t * restrict)fragbuf->column_data_[FB_IDX_FRAG_ALPHA])[row] = (uint8_t)(255.f * alpha_channel[row]);
      }

      row += delta;
    } while (delta);
  }

  // Stencil test
  primitive_assembly_stencil_func_t stencil_func;
  uint32_t stencil_func_ref;
  uint32_t stencil_func_ref_masked;
  uint32_t stencil_func_mask;
  primitive_assembly_stencil_op_t sfail_op, zfail_op, zpass_op;
  uint32_t stencil_mask;

  if (orientation == RASTERIZER_CLOCKWISE) {
    stencil_mask = stencil_cw_mask;
    stencil_func = stencil_cw_func;
    stencil_func_ref = stencil_cw_func_ref;
    stencil_func_mask = stencil_cw_func_mask;
    sfail_op = stencil_cw_sfail;
    zfail_op = stencil_cw_zfail;
    zpass_op = stencil_cw_zpass;
  }
  else /* (orientation == RASTERIZER_COUNTER_CLOCKWISE) */ {
    stencil_mask = stencil_ccw_mask;
    stencil_func = stencil_ccw_func;
    stencil_func_ref = stencil_ccw_func_ref;
    stencil_func_mask = stencil_ccw_func_mask;
    sfail_op = stencil_ccw_sfail;
    zfail_op = stencil_ccw_zfail;
    zpass_op = stencil_ccw_zpass;
  }
  stencil_func_ref_masked = stencil_func_ref & stencil_func_mask;

  if (enable_stencil_test) {
    /* Perform stencil function and store result in FB_IDX_TEMP_BYTE_0, note that the stencil
     * function result is combined with the rasterizer-generated mask, consequently, we need not
     * worry about writing pixels outside the triangle if we adhere to just the stencil mask. */
    switch (stencil_func) {
      case PASF_EQUAL:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t stencil_value = *((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint8_t mask = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          uint8_t r = mask & (uint8_t)-(int8_t)((stencil_value & stencil_func_mask) == stencil_func_ref_masked);
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row] = r;
        }
        break;
      case PASF_NOTEQUAL:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t stencil_value = *((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint8_t mask = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          uint8_t r = mask & (uint8_t)-(int8_t)((stencil_value & stencil_func_mask) != stencil_func_ref_masked);
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row] = r;
        }
        break;
      case PASF_LESS:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t stencil_value = *((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint8_t mask = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          uint8_t r = mask & (uint8_t)-(int8_t)((stencil_value & stencil_func_mask) < stencil_func_ref_masked);
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row] = r;
        }
        break;
      case PASF_GREATER:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t stencil_value = *((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint8_t mask = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          uint8_t r = mask & (uint8_t)-(int8_t)((stencil_value & stencil_func_mask) > stencil_func_ref_masked);
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row] = r;
        }
        break;
      case PASF_LEQUAL:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t stencil_value = *((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint8_t mask = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          uint8_t r = mask & (uint8_t)-(int8_t)((stencil_value & stencil_func_mask) <= stencil_func_ref_masked);
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row] = r;
        }
        break;
      case PASF_GEQUAL:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t stencil_value = *((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint8_t mask = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          uint8_t r = mask & (uint8_t)-(int8_t)((stencil_value & stencil_func_mask) >= stencil_func_ref_masked);
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row] = r;
        }
        break;
      case PASF_ALWAYS:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint8_t mask = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row] = mask;
        }
        break;
      case PASF_NEVER:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row] = 0x00;
        }
        break;
    }
  }
  else {
    /* Ensure FB_IDX_TEMP_BYTE_0 is initialized with mask if stencil is disabled; z-buffer testing
     * uses it as input. */
    for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
      uint8_t mask = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
      ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row] = mask;
    }
  }

  /* Z-buffer test - result of the z-buffer test (pass=0xFF, fail=0x00) is stored in FB_IDX_TEMP_BYTE_1
   * If !enable_zbuf_test, then always pass, if enable_zbuf_test and enable_zbuf_write, then a write
   * to the zbuffer occurs in case both the stencil function (in FB_IDX_TEMP_BYTE_0) and the zbuffer
   * function is a pass. */
  if (enable_zbuf_test) {
    if (enable_zbuf_write) {
      switch (zbuf_step) {
        case 2:
          /* 2 byte: 16 bit z-buffer */
          switch (zbuf_func) {
            case PAZF_NEVER:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = 0x00;
              }
              break;
            case PAZF_LESS:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value < zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint16_t outmask = (uint16_t)(int16_t)(int8_t)r;
                uint16_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
            case PAZF_EQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value == zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint16_t outmask = (uint16_t)(int16_t)(int8_t)r;
                uint16_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
            case PAZF_LEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value <= zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint16_t outmask = (uint16_t)(int16_t)(int8_t)r;
                uint16_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
            case PAZF_GREATER:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value > zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint16_t outmask = (uint16_t)(int16_t)(int8_t)r;
                uint16_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
            case PAZF_NOTEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value != zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint16_t outmask = (uint16_t)(int16_t)(int8_t)r;
                uint16_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
            case PAZF_GEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value >= zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint16_t outmask = (uint16_t)(int16_t)(int8_t)r;
                uint16_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
            case PAZF_ALWAYS:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass;
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint16_t outmask = (uint16_t)(int16_t)(int8_t)r;
                uint16_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
          }
          break;
        case 3:
          /* 3 byte: 24 bit z-buffer
           * Encoded big-endian so it is easier to debug / view in memory dumps
           */
          switch (zbuf_func) {
            case PAZF_NEVER:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = 0x00;
              }
              break;
            case PAZF_LESS:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value < zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);

                zbuf_ptr[0] = (uint8_t)(new_zbuf_value >> 16);
                zbuf_ptr[1] = (uint8_t)(new_zbuf_value >> 8);
                zbuf_ptr[2] = (uint8_t)new_zbuf_value;
              }
              break;
            case PAZF_EQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value == zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);

                zbuf_ptr[0] = (uint8_t)(new_zbuf_value >> 16);
                zbuf_ptr[1] = (uint8_t)(new_zbuf_value >> 8);
                zbuf_ptr[2] = (uint8_t)new_zbuf_value;
              }
              break;
            case PAZF_LEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value <= zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);

                zbuf_ptr[0] = (uint8_t)(new_zbuf_value >> 16);
                zbuf_ptr[1] = (uint8_t)(new_zbuf_value >> 8);
                zbuf_ptr[2] = (uint8_t)new_zbuf_value;
              }
              break;
            case PAZF_GREATER:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value > zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);

                zbuf_ptr[0] = (uint8_t)(new_zbuf_value >> 16);
                zbuf_ptr[1] = (uint8_t)(new_zbuf_value >> 8);
                zbuf_ptr[2] = (uint8_t)new_zbuf_value;
              }
              break;
            case PAZF_NOTEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value != zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);

                zbuf_ptr[0] = (uint8_t)(new_zbuf_value >> 16);
                zbuf_ptr[1] = (uint8_t)(new_zbuf_value >> 8);
                zbuf_ptr[2] = (uint8_t)new_zbuf_value;
              }
              break;
            case PAZF_GEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value >= zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);

                zbuf_ptr[0] = (uint8_t)(new_zbuf_value >> 16);
                zbuf_ptr[1] = (uint8_t)(new_zbuf_value >> 8);
                zbuf_ptr[2] = (uint8_t)new_zbuf_value;
              }
              break;
            case PAZF_ALWAYS:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass;
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);

                zbuf_ptr[0] = (uint8_t)(new_zbuf_value >> 16);
                zbuf_ptr[1] = (uint8_t)(new_zbuf_value >> 8);
                zbuf_ptr[2] = (uint8_t)new_zbuf_value;
              }
              break;
          }
          break;
        case 4:
          /* 32 bit z-buffer */
          switch (zbuf_func) {
            case PAZF_NEVER:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = 0x00;
              }
              break;
            case PAZF_LESS:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value < zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
            case PAZF_EQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value == zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
            case PAZF_LEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value <= zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
            case PAZF_GREATER:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value > zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
            case PAZF_NOTEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value != zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
            case PAZF_GEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value >= zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
            case PAZF_ALWAYS:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass;
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
                uint32_t outmask = (uint32_t)(int32_t)(int8_t)r;
                uint32_t new_zbuf_value = (zbuf_value & ~outmask) | (z_value & outmask);
                *zbuf_ptr = new_zbuf_value;
              }
              break;
          }
          break;
      }
    }
    else /* (!enable_zbuf_write) */ {
      switch (zbuf_step) {
        case 2:
          /* 2 byte: 16 bit z-buffer */
          switch (zbuf_func) {
            case PAZF_NEVER:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = 0x00;
              }
              break;
            case PAZF_LESS:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value < zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_EQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value == zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_LEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value <= zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_GREATER:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value > zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_NOTEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value != zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_GEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value >= zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_ALWAYS:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint16_t *zbuf_ptr = (((uint16_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint16_t zbuf_value = *zbuf_ptr;
                uint16_t z_value = (uint16_t)((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass;
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
          }
          break;
        case 3:
          /* 3 byte: 24 bit z-buffer
           * Encoded big-endian so it is easier to debug / view in memory dumps
           */
          switch (zbuf_func) {
            case PAZF_NEVER:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = 0x00;
              }
              break;
            case PAZF_LESS:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value < zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_EQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value == zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_LEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value <= zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_GREATER:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value > zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_NOTEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value != zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_GEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value >= zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_ALWAYS:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint8_t *zbuf_ptr = (((uint8_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = (((uint32_t)zbuf_ptr[0]) << 16) |
                                      (((uint32_t)zbuf_ptr[1]) << 8) |
                                      (((uint32_t)zbuf_ptr[2]));
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass;
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
          }
          break;
        case 4:
          /* 32 bit z-buffer */
          switch (zbuf_func) {
            case PAZF_NEVER:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = 0x00;
              }
              break;
            case PAZF_LESS:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value < zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_EQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value == zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_LEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value <= zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_GREATER:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value > zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_NOTEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value != zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_GEQUAL:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass & (uint8_t)-(int8_t)(z_value >= zbuf_value);
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
            case PAZF_ALWAYS:
              for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                uint32_t *zbuf_ptr = (((uint32_t *restrict*restrict)fragbuf->column_data_[FB_IDX_ZBUF_PTR])[frag_row]);
                uint32_t zbuf_value = *zbuf_ptr;
                uint32_t z_value = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
                uint8_t r = stencil_pass;
                ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = r;
              }
              break;
          }
          break;
      }
    }
  }
  else /* (!enable_zbuf_test) */ {
    /* Always pass if the rasterizer considers it a valid pixel */
    for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
      uint8_t stencil_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
      ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row] = stencil_pass;
    }
  }

  /* FB_IDX_MASK holds the mask for writes from the rasterizer, 0x00 should be completely
   *             ignored, 0xFF should be considered for the stencil functions
   * FB_IDX_TEMPL_BYTE_0 holds the result of the stencil function, 0xFF for a pass, 0x00 for
   *                     either a failure (or corresponding FB_IDX_MASK was 0x00.)
   * FB_IDX_TEMPL_BYTE_1 holds the result of the zbuf function, 0xFF for a pass, 0x00 for
   *                     either a failure, or the corresponding FB_IDX_TEMPL_BYTE_0 was 0x00.
   * Consequently, if FB_IDX_TEMPL_BYTE_1 is 0xFF, we should apply zpass stencil, if
   * FB_IDX_TEMPL_BYTE_1 is 0x00 and FB_IDX_TEMPL_BYTE_0 is 0xFF, we should apply zfail, if
   * FB_IDX_TEMPL_BYTE_0 is 0x00 and FB_IDX_MASK is 0xFF, we should apply sfail.
   * Finally, prior to blending, we should copy over FB_IDX_TEMPL_BYTE_1 to FB_IDX_MASK so
   * the blend function will not render pixels that were stencilled out. */

  if (enable_stencil_test && stencil_mask) {
    /* FB_IDX_TEMPL_BYTE_0 now holds the result of the stencil function, 0xFF for a pass, 0x00
     * for a failure */
    /* Consuming FB_IDX_TEMP_BYTE_2 and FB_IDX_TEMP_BYTE_3 here to hold the result of the first
     * stencil operator pass -- note how FB_IDX_TEMP_BYTE_2 is a byte size column yet we use it
     * as a short word, hence FB_IDX_TEMP_BYTE_3 is also taken. */

    /* sfail: the stencil failed (FB_IDX_TEMP_BYTE_0 is 0) but the pixel is valid (FB_IDX_MASK is 0xFF) */
    switch (sfail_op) {
      case PASO_ZERO:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];

          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = 0x0000 | ((*stencil_ptr) & ~mask);
        }
        break;
      case PASO_REPLACE:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];

          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = (stencil_func_ref & mask) | ((*stencil_ptr) & ~mask);
        }
        break;
      case PASO_INCR:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint16_t old_stencil_value = *stencil_ptr;
          uint16_t new_stencil_value = (old_stencil_value == 0xFFFF) ? old_stencil_value : old_stencil_value + 1;
          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = (new_stencil_value & mask) | (old_stencil_value & ~mask);
        }
        break;
      case PASO_DECR:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint16_t old_stencil_value = *stencil_ptr;
          uint16_t new_stencil_value = (old_stencil_value == 0x0000) ? old_stencil_value : old_stencil_value - 1;
          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = (new_stencil_value & mask) | (old_stencil_value & ~mask);
        }
        break;
      case PASO_INCR_WRAP:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint16_t old_stencil_value = *stencil_ptr;
          uint16_t new_stencil_value = old_stencil_value + 1;
          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = (new_stencil_value & mask) | (old_stencil_value & ~mask);
        }
        break;
      case PASO_DECR_WRAP:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint16_t old_stencil_value = *stencil_ptr;
          uint16_t new_stencil_value = old_stencil_value - 1;
          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = (new_stencil_value & mask) | (old_stencil_value & ~mask);
        }
        break;
      case PASO_KEEP:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t stencil_value = *((uint16_t *restrict *restrict)fragbuf->column_data_[FBCT_STENCIL_PTR])[frag_row];
          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = stencil_value;
        }
        break;
      case PASO_INVERT:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint16_t old_stencil_value = *stencil_ptr;
          uint16_t new_stencil_value = ~old_stencil_value;
          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = (new_stencil_value & mask) | (old_stencil_value & ~mask);
        }
        break;
    }

    /* zfail: the stencil passed (FB_IDX_TEMP_BYTE_0 is 0xFF) but the zbuffer test failed (FB_IDX_TEMP_BYTE_1 is 0x00) */
    switch (zfail_op) {
      case PASO_ZERO:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];

          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = 0x0000 | (old_stencil_value & ~mask);
        }
        break;
      case PASO_REPLACE:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];

          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = (stencil_func_ref & mask) | (old_stencil_value & ~mask);
        }
        break;
      case PASO_INCR:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];
          uint16_t new_stencil_value = (old_stencil_value == 0xFFFF) ? old_stencil_value : old_stencil_value + 1;
          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = (new_stencil_value & mask) | (old_stencil_value & ~mask);
        }
        break;
      case PASO_DECR:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];
          uint16_t new_stencil_value = (old_stencil_value == 0x0000) ? old_stencil_value : old_stencil_value - 1;
          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = (new_stencil_value & mask) | (old_stencil_value & ~mask);
        }
        break;
      case PASO_INCR_WRAP:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];
          uint16_t new_stencil_value = old_stencil_value + 1;
          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = (new_stencil_value & mask) | (old_stencil_value & ~mask);
        }
        break;
      case PASO_DECR_WRAP:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];
          uint16_t new_stencil_value = old_stencil_value - 1;
          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = (new_stencil_value & mask) | (old_stencil_value & ~mask);
        }
        break;
      case PASO_KEEP:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];
          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = old_stencil_value;
        }
        break;
      case PASO_INVERT:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_0])[frag_row];
          uint16_t must_be_clr = (uint16_t)(int16_t)(int8_t)((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t mask = must_be_set & (~must_be_clr) & stencil_mask;
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];
          uint16_t new_stencil_value = ~old_stencil_value;
          ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row] = (new_stencil_value & mask) | (old_stencil_value & ~mask);
        }
        break;
    }


    /* zpass: the stencil passed and the zbuffer test passed (FB_IDX_TEMP_BYTE_1 is 0xFF); this is the same as just testing for FB_IDX_TEMPL_BYTE_1,
     * Note that we also take the opportunity here to update FB_IDX_MASK to be set for every pixel that passed both the stencil and the zbuffer
     * tests. Also note that, given zpass is the final pass of three (sfail, zfail, zpass), this is where we write back into the stencil buffer */
    switch (zpass_op) {
      case PASO_ZERO:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint8_t must_be_set8 = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)must_be_set8;
          uint16_t mask = must_be_set & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];

          *stencil_ptr = 0x0000 | (old_stencil_value & ~mask);
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row] = must_be_set8;
        }
        break;
      case PASO_REPLACE:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint8_t must_be_set8 = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)must_be_set8;
          uint16_t mask = must_be_set & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];
          *stencil_ptr = (stencil_func_ref & mask) | (old_stencil_value & ~mask);
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row] = must_be_set8;
        }
        break;
      case PASO_INCR:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint8_t must_be_set8 = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)must_be_set8;
          uint16_t mask = must_be_set & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];
          uint16_t new_stencil_value = (old_stencil_value == 0xFFFF) ? old_stencil_value : old_stencil_value + 1;
          *stencil_ptr = (new_stencil_value & mask) | (old_stencil_value & ~mask);
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row] = must_be_set8;
        }
        break;
      case PASO_DECR:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint8_t must_be_set8 = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)must_be_set8;
          uint16_t mask = must_be_set & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];
          uint16_t new_stencil_value = (old_stencil_value == 0x0000) ? old_stencil_value : old_stencil_value - 1;
          *stencil_ptr = (new_stencil_value & mask) | (old_stencil_value & ~mask);
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row] = must_be_set8;
        }
        break;
      case PASO_INCR_WRAP:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint8_t must_be_set8 = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)must_be_set8;
          uint16_t mask = must_be_set & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];
          uint16_t new_stencil_value = old_stencil_value + 1;
          *stencil_ptr = (new_stencil_value & mask) | (old_stencil_value & ~mask);
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row] = must_be_set8;
        }
        break;
      case PASO_DECR_WRAP:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint8_t must_be_set8 = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)must_be_set8;
          uint16_t mask = must_be_set & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];
          uint16_t new_stencil_value = old_stencil_value - 1;
          *stencil_ptr = (new_stencil_value & mask) | (old_stencil_value & ~mask);
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row] = must_be_set8;
        }
        break;
      case PASO_KEEP:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint8_t must_be_set8 = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)must_be_set8;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];
          *stencil_ptr = old_stencil_value;
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row] = must_be_set8;
        }
        break;
      case PASO_INVERT:
        for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
          uint8_t must_be_set8 = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
          uint16_t must_be_set = (uint16_t)(int16_t)(int8_t)must_be_set8;
          uint16_t mask = must_be_set & stencil_mask;
          uint16_t *restrict stencil_ptr = ((uint16_t *restrict *restrict)fragbuf->column_data_[FB_IDX_STENCIL_PTR])[frag_row];
          uint16_t old_stencil_value = ((uint16_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_2])[frag_row];
          uint16_t new_stencil_value = ~old_stencil_value;
          *stencil_ptr = (new_stencil_value & mask) | (old_stencil_value & ~mask);
          ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row] = must_be_set8;
        }
        break;
    }
  }
  else {
    /* Stencil test not enabled, nevertheless, in the case of z-buffer test results we need to move
     * them to FB_IDX_MASK so we don't write pixels that failed the z-buffer test. */
    for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
      uint8_t zbuf_pass = ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_TEMP_BYTE_1])[frag_row];
      ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_MASK])[frag_row] = zbuf_pass;
    }
  }

  // Write out / blend pixels

  blend(fragbuf->num_rows_, fragbuf->column_data_[FB_IDX_MASK],
        fragbuf->column_data_[FB_IDX_FRAG_RED],
        fragbuf->column_data_[FB_IDX_FRAG_GREEN],
        fragbuf->column_data_[FB_IDX_FRAG_BLUE],
        fragbuf->column_data_[FB_IDX_FRAG_ALPHA],
        fragbuf->column_data_[FB_IDX_PIXEL_PTR],
        enable_red, enable_green, enable_blue, enable_alpha,
        rgb_eq, alpha_eq, src_rgb_fn, src_alpha_fn, dst_rgb_fn, dst_alpha_fn,
        constant_red, constant_grn, constant_blu, constant_alpha,
        fragbuf->column_data_[FB_IDX_TEMP_BYTE_0],
        fragbuf->column_data_[FB_IDX_TEMP_BYTE_1],
        fragbuf->column_data_[FB_IDX_TEMP_BYTE_2],
        fragbuf->column_data_[FB_IDX_TEMP_BYTE_3],

        fragbuf->column_data_[FB_IDX_TEMP_BYTE_4],
        fragbuf->column_data_[FB_IDX_TEMP_BYTE_5],
        fragbuf->column_data_[FB_IDX_TEMP_BYTE_6],
        fragbuf->column_data_[FB_IDX_TEMP_BYTE_7]);
        
  fragbuf->num_rows_ = 0;
}

uint64_t shader_blacklist_[] = {
  /* Fragment shaders */
#if 0
  0x19EB6F9FDC3DB6A1,
  0x555A65AF15B66C67,
#endif
  // 0x6FD47EA9E6F41970,  // <-- this is the main fragment shader.
#if 0
  0x96DFD67ADF173F9A,
  0x99F100749B71D60E,
  0xA598C6ADAA8B0DA3,
  0xAEF419E6472F4E51,
  0xBD75BD3015B68578,
  0xD070BFCECD398EDB,
  0xE3A02E700F9359C1,
  0xFD31B4CFA2F0B0BB,
#endif

  /* Vertex shaders */
#if 0
  0x00997A9B7232E7FD,
  0x054C105C4DCFD7E3,
  0x0634229D3EAFE565,
  0x1225FD504BCA38DD,
  0x1426142EB72D898A,
  0x1643B72A38634E92,
  0x2F5843FE6CA04FFF,
  0x34FD42ECAC1627AE,
  0x5F2A0F9B934D44E6,
#endif

#if 0
  0x66544151149F8A8E,
#endif

#if 0
  0x6D58F1BBF3E956AA,
  0x6E8490A7112C7EE8,
  0x829313213F448939,
  0xB4E1FFAAD9670225,
  0xF08021F046D68DC3,
#endif
  ~0
};

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
                                      const void *indices) {
  /* Check if either vertex_shader or fragment_shader are blacklisted. */
  size_t idx;
  int vmarked = 0;
  int fmarked = 0;
  for (idx = 0; idx < sizeof(shader_blacklist_)/sizeof(*shader_blacklist_); ++idx) {
    uint64_t hash = shader_blacklist_[idx];
    if (hash == vertex_shader->hash_) {
      vmarked = 1;
    }
    if (hash == fragment_shader->hash_) {
      fmarked = 1;
    }
  }
  if (vmarked) return;

  while (primitive_assembly_process_primitives(pa, as, vertex_shader, ar, cs, ras, fragbuf, fragment_shader, 
                                               vp_x, vp_y, vp_width, vp_height, depth_range_near, depth_range_far,
                                               screen_width, screen_height,
                                               scissor_left, scissor_bottom_counted_from_bottom, scissor_width, scissor_height,
                                               max_z, rgba, rgba_stride, zbuf, zbuf_stride, zbuf_step,
                                               stencil_buf, stencil_stride, stencil_step, 
                                               permitted_orientations,
                                               enable_stencil_test,
                                               stencil_cw_mask, stencil_cw_func, stencil_cw_func_ref, stencil_cw_func_mask,
                                               stencil_cw_sfail, stencil_cw_zfail, stencil_cw_zpass,
                                               stencil_ccw_mask, stencil_ccw_func, stencil_ccw_func_ref, stencil_ccw_func_mask,
                                               stencil_ccw_sfail, stencil_ccw_zfail, stencil_ccw_zpass,
                                               enable_zbuf_test,
                                               zbuf_func,
                                               enable_zbuf_write, enable_red, enable_green, enable_blue, enable_alpha,
                                               rgb_eq, alpha_eq, src_rgb_fn, src_alpha_fn, dst_rgb_fn, dst_alpha_fn, 
                                               constant_red, constant_grn, constant_blu, constant_alpha,
                                               offset_factor, offset_units,
                                               mode, num_elements, index_type, arrayed_starting_index, indices)) {

    primitive_assembly_process_fragments(pa, as, vertex_shader, ar, cs, ras, fragbuf, fragment_shader, 
                                         vp_x, vp_y, vp_width, vp_height, depth_range_near, depth_range_far,
                                         screen_width, screen_height,
                                         scissor_left, scissor_bottom_counted_from_bottom, scissor_width, scissor_height,
                                         max_z, rgba, rgba_stride, zbuf, zbuf_stride, zbuf_step,
                                         stencil_buf, stencil_stride, stencil_step, enable_stencil_test,
                                         stencil_cw_mask, stencil_cw_func, stencil_cw_func_ref, stencil_cw_func_mask,
                                         stencil_cw_sfail, stencil_cw_zfail, stencil_cw_zpass,
                                         stencil_ccw_mask, stencil_ccw_func, stencil_ccw_func_ref, stencil_ccw_func_mask,
                                         stencil_ccw_sfail, stencil_ccw_zfail, stencil_ccw_zpass,
                                         enable_zbuf_test,
                                         zbuf_func,
                                         enable_zbuf_write, enable_red, enable_green, enable_blue, enable_alpha,
                                         rgb_eq, alpha_eq, src_rgb_fn, src_alpha_fn, dst_rgb_fn, dst_alpha_fn, 
                                         constant_red, constant_grn, constant_blu, constant_alpha,
                                         offset_factor, offset_units,
                                         mode, num_elements, index_type, arrayed_starting_index, indices,
                                         0, 255, 192, 192);
  }
}

