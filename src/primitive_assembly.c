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

#ifndef CLIPPING_STAGE_H_INCLUDED
#define CLIPPING_STAGE_H_INCLUDED
#include "clipping_stage.h"
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

void primitive_assembly_init(struct primitive_assembly *pa) {
  pa->num_rows_ = 0;
  pa->num_cols_ = 0;
  pa->column_descriptors_ = NULL;
  pa->column_data_ = NULL;
  pa->slab_ = NULL;
  pa->num_vertex_indices_ = 0;
  memset(pa->vertex_indices_, 0, sizeof(pa->vertex_indices_));
  pa->index_at_ = 0;
  pa->continue_at_ = 0;
}


static size_t primitive_assembly_calc_rowsize(struct primitive_assembly *pa) {
  size_t n;
  size_t rowsize = 0;
  for (n = 0; n < pa->num_cols_; ++n) {
    struct primitive_assembly_column_descriptor *cd = pa->column_descriptors_ + n;
    switch (cd->data_type_) {
      case PADT_NONE:
        break;
      case PADT_FLOAT:
        rowsize += sizeof(float);
        break;
      case PADT_UINT8:
        rowsize += sizeof(uint8_t);
        break;
      case PADT_UINT16:
        rowsize += sizeof(uint16_t);
        break;
      case PADT_INT32:
        rowsize += sizeof(int32_t);
        break;
    }
  }
  return rowsize;
}

static void primitive_assembly_update_column_data_ptrs(struct primitive_assembly *pa) {
  size_t n;
  size_t rowsize = 0;
  uint8_t *p = (uint8_t *)pa->slab_;
  for (n = 0; n < pa->num_cols_; ++n) {
    pa->column_data_[n] = p;

    struct primitive_assembly_column_descriptor *cd = pa->column_descriptors_ + n;
    switch (cd->data_type_) {
      case PADT_NONE:
        break;
      case PADT_FLOAT:
        p += sizeof(float) * PRIMITIVE_ASSEMBLY_MAX_ROWS;
        break;
      case PADT_UINT8:
        p += sizeof(uint8_t) * PRIMITIVE_ASSEMBLY_MAX_ROWS;
        break;
      case PADT_UINT16:
        p += sizeof(uint16_t) * PRIMITIVE_ASSEMBLY_MAX_ROWS;
        break;
      case PADT_INT32:
        p += sizeof(int32_t) * PRIMITIVE_ASSEMBLY_MAX_ROWS;
        break;
    }
  }
}

int primitive_assembly_alloc_buffers(struct primitive_assembly *pa) {
  pa->num_rows_ = 0;

  if (!pa->num_cols_) {
    /* First-time initialization of the column,
     * initialize with execution-chain + XYZW */
    size_t num_cols = PAC_IDX_NUM_FIXED_IDX;
    if (pa->column_descriptors_) free(pa->column_descriptors_);
    pa->column_descriptors_ = (struct primitive_assembly_column_descriptor *)malloc(sizeof(struct primitive_assembly_column_descriptor) * num_cols);
    if (!pa->column_descriptors_) {
      return -1;
    }
    pa->num_cols_ = num_cols;
    pa->column_descriptors_[PAC_IDX_EXECUTION_CHAIN].col_type_ = PACT_EXECUTION_CHAIN;
    pa->column_descriptors_[PAC_IDX_EXECUTION_CHAIN].data_type_ = PADT_UINT16;
    pa->column_descriptors_[PAC_IDX_EXECUTION_CHAIN].attrib_element_index_ = -1;
    pa->column_descriptors_[PAC_IDX_EXECUTION_CHAIN].attrib_index_ = -1;
    pa->column_descriptors_[PAC_IDX_POSITION_X].col_type_ = PACT_POSITION_X;
    pa->column_descriptors_[PAC_IDX_POSITION_X].data_type_ = PADT_FLOAT;
    pa->column_descriptors_[PAC_IDX_POSITION_X].attrib_element_index_ = -1;
    pa->column_descriptors_[PAC_IDX_POSITION_X].attrib_index_ = -1;
    pa->column_descriptors_[PAC_IDX_POSITION_Y].col_type_ = PACT_POSITION_Y;
    pa->column_descriptors_[PAC_IDX_POSITION_Y].data_type_ = PADT_FLOAT;
    pa->column_descriptors_[PAC_IDX_POSITION_Y].attrib_element_index_ = -1;
    pa->column_descriptors_[PAC_IDX_POSITION_Y].attrib_index_ = -1;
    pa->column_descriptors_[PAC_IDX_POSITION_Z].col_type_ = PACT_POSITION_Z;
    pa->column_descriptors_[PAC_IDX_POSITION_Z].data_type_ = PADT_FLOAT;
    pa->column_descriptors_[PAC_IDX_POSITION_Z].attrib_element_index_ = -1;
    pa->column_descriptors_[PAC_IDX_POSITION_Z].attrib_index_ = -1;
    pa->column_descriptors_[PAC_IDX_POSITION_W].col_type_ = PACT_POSITION_W;
    pa->column_descriptors_[PAC_IDX_POSITION_W].data_type_ = PADT_FLOAT;
    pa->column_descriptors_[PAC_IDX_POSITION_W].attrib_element_index_ = -1;
    pa->column_descriptors_[PAC_IDX_POSITION_W].attrib_index_ = -1;
    pa->column_descriptors_[PAC_IDX_POINT_SIZE].col_type_ = PACT_POINT_SIZE;
    pa->column_descriptors_[PAC_IDX_POINT_SIZE].data_type_ = PADT_FLOAT;
    pa->column_descriptors_[PAC_IDX_POINT_SIZE].attrib_element_index_ = -1;
    pa->column_descriptors_[PAC_IDX_POINT_SIZE].attrib_index_ = -1;
  }
  size_t rowsize = primitive_assembly_calc_rowsize(pa);
  size_t slab_size = rowsize * PRIMITIVE_ASSEMBLY_MAX_ROWS;
  if (pa->slab_) free(pa->slab_);
  pa->slab_ = malloc(slab_size);
  if (!pa->slab_) {
    free(pa->column_descriptors_);
    pa->column_descriptors_ = NULL;
    return -1;
  }

  if (pa->column_data_) free(pa->column_data_);
  pa->column_data_ = (void **)malloc(pa->num_cols_ * sizeof(void *));
  if (!pa->column_data_) {
    free(pa->column_descriptors_);
    pa->column_descriptors_ = NULL;
    if (pa->slab_) free(pa->slab_);
    pa->slab_ = NULL;
    return -1;
  }

  primitive_assembly_update_column_data_ptrs(pa);

  return 0;
}


void primitive_assembly_cleanup(struct primitive_assembly *pa) {
  if (pa->column_descriptors_) free(pa->column_descriptors_);
  if (pa->column_data_) free(pa->column_data_);
  if (pa->slab_) free(pa->slab_);
}

int primitive_assembly_elements_u8(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, uint8_t *indices, size_t num_indices) {
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
              pa->vertex_indices_[pa->num_vertex_indices_ + 0] = indices[pa->index_at_ + 1];
              pa->vertex_indices_[pa->num_vertex_indices_ + 1] = indices[pa->index_at_ + 0];
              pa->vertex_indices_[pa->num_vertex_indices_ + 2] = indices[pa->index_at_ + 2];
            }
            else {
              pa->vertex_indices_[pa->num_vertex_indices_ + 0] = indices[pa->index_at_ + 0];
              pa->vertex_indices_[pa->num_vertex_indices_ + 1] = indices[pa->index_at_ + 1];
              pa->vertex_indices_[pa->num_vertex_indices_ + 2] = indices[pa->index_at_ + 2];
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
            pa->vertex_indices_[pa->num_vertex_indices_ + 0] = indices[0];
            pa->vertex_indices_[pa->num_vertex_indices_ + 1] = indices[pa->index_at_ + 1];
            pa->vertex_indices_[pa->num_vertex_indices_ + 2] = indices[pa->index_at_ + 2];
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

int primitive_assembly_elements_u16(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, uint16_t *indices, size_t num_indices) {
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
              pa->vertex_indices_[pa->num_vertex_indices_ + 0] = indices[pa->index_at_ + 1];
              pa->vertex_indices_[pa->num_vertex_indices_ + 1] = indices[pa->index_at_ + 0];
              pa->vertex_indices_[pa->num_vertex_indices_ + 2] = indices[pa->index_at_ + 2];
            }
            else {
              pa->vertex_indices_[pa->num_vertex_indices_ + 0] = indices[pa->index_at_ + 0];
              pa->vertex_indices_[pa->num_vertex_indices_ + 1] = indices[pa->index_at_ + 1];
              pa->vertex_indices_[pa->num_vertex_indices_ + 2] = indices[pa->index_at_ + 2];
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
            pa->vertex_indices_[pa->num_vertex_indices_ + 0] = indices[0];
            pa->vertex_indices_[pa->num_vertex_indices_ + 1] = indices[pa->index_at_ + 1];
            pa->vertex_indices_[pa->num_vertex_indices_ + 2] = indices[pa->index_at_ + 2];
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

int primitive_assembly_elements_u32(struct primitive_assembly *pa, struct attrib_set *as, primitive_assembly_mode_t pam, uint32_t *indices, size_t num_indices) {
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
              pa->vertex_indices_[pa->num_vertex_indices_ + 0] = indices[pa->index_at_ + 1];
              pa->vertex_indices_[pa->num_vertex_indices_ + 1] = indices[pa->index_at_ + 0];
              pa->vertex_indices_[pa->num_vertex_indices_ + 2] = indices[pa->index_at_ + 2];
            }
            else {
              pa->vertex_indices_[pa->num_vertex_indices_ + 0] = indices[pa->index_at_ + 0];
              pa->vertex_indices_[pa->num_vertex_indices_ + 1] = indices[pa->index_at_ + 1];
              pa->vertex_indices_[pa->num_vertex_indices_ + 2] = indices[pa->index_at_ + 2];
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
            pa->vertex_indices_[pa->num_vertex_indices_ + 0] = indices[0];
            pa->vertex_indices_[pa->num_vertex_indices_ + 1] = indices[pa->index_at_ + 1];
            pa->vertex_indices_[pa->num_vertex_indices_ + 2] = indices[pa->index_at_ + 2];
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
      if (pam == PAM_POINTS) {
        for (;;) {
          while ((pa->index_at_ < num_elements) &&
                 (pa->num_vertex_indices_ < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if (pa->index_at_== num_elements) {
            // Yield completion.
            pa->index_at_ = 0;
            pa->continue_at_ = 0;
            return 0;
          }
        }
      }
      else if (pam == PAM_LINES) {
        for (;;) {
          while (((pa->index_at_ + 2) <= num_elements) &&
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
          if ((pa->index_at_ + 2) > num_elements) {
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
                 ((pa->index_at_ + 2) <= num_elements)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_++;
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_; /* note we only advance once */
          }
          while (pa->num_vertex_indices_) {
            /* Yield result */
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if ((pa->index_at_ + 2) > num_elements) {
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
                 ((pa->index_at_ + 2) <= num_elements)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_++;
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_; /* note we only advance once */
          }
          if (((pa->num_vertex_indices_ + 2) <= (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_))) &&
              ((pa->index_at_ + 1) <= num_elements) && (num_elements >= 2)) {
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)pa->index_at_++;
            pa->vertex_indices_[pa->num_vertex_indices_++] = (uint32_t)0; /* loop to start */
          }
          while (pa->num_vertex_indices_) {
            /* Yield result */
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__: ;
          }
          if ((pa->index_at_ + 1) > num_elements) {
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
          while (((pa->index_at_ + 3) <= num_elements) &&
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
          if ((pa->index_at_ + 3) > num_elements) {
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
          while (((pa->index_at_ + 3) <= num_elements) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            if (pa->index_at_ & 0) {
              pa->vertex_indices_[pa->num_vertex_indices_ + 0] = (uint32_t)pa->index_at_ + 1;
              pa->vertex_indices_[pa->num_vertex_indices_ + 1] = (uint32_t)pa->index_at_ + 0;
              pa->vertex_indices_[pa->num_vertex_indices_ + 2] = (uint32_t)pa->index_at_ + 2;
            }
            else {
              pa->vertex_indices_[pa->num_vertex_indices_ + 0] = (uint32_t)pa->index_at_ + 0;
              pa->vertex_indices_[pa->num_vertex_indices_ + 1] = (uint32_t)pa->index_at_ + 1;
              pa->vertex_indices_[pa->num_vertex_indices_ + 2] = (uint32_t)pa->index_at_ + 2;
            }
            pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > num_elements) {
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
          while (((pa->index_at_ + 3) <= num_elements) &&
                 ((pa->num_vertex_indices_ + 3) < (sizeof(pa->vertex_indices_)/sizeof(*pa->vertex_indices_)))) {
            pa->vertex_indices_[pa->num_vertex_indices_ + 0] = 0;
            pa->vertex_indices_[pa->num_vertex_indices_ + 1] = (uint32_t)pa->index_at_ + 1;
            pa->vertex_indices_[pa->num_vertex_indices_ + 2] = (uint32_t)pa->index_at_ + 2;
            pa->index_at_++;
          }
          while (pa->num_vertex_indices_) {
            // Yield result
            pa->continue_at_ = __LINE__ + 2;
            return 1;
    case __LINE__:;
          }
          if ((pa->index_at_ + 3) > num_elements) {
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

int primitive_assembly_gather_attribs(struct primitive_assembly *pa, struct attrib_set *as) {
  size_t col;
  size_t row;
  size_t num_rows = PRIMITIVE_ASSEMBLY_MAX_ROWS - pa->num_rows_;
  uint32_t * restrict indices = (uint32_t * restrict)pa->vertex_indices_;
  if (num_rows > pa->num_vertex_indices_) {
    num_rows = pa->num_vertex_indices_;
  }
  if (!num_rows) return 0;
  for (col = 0; col < pa->num_cols_; ++col) {
    struct primitive_assembly_column_descriptor *pacd = pa->column_descriptors_ + col;
    void *data = pa->column_data_[col];
    switch (pacd->data_type_) {
      case PADT_NONE:
        break;
      case PADT_FLOAT: {
        float *restrict pf = ((float * restrict)data) + pa->num_rows_;
        switch (pacd->col_type_) {
          case PACT_EXECUTION_CHAIN:
            for (row = 0; row < num_rows; ++row) {
              *pf++ = 1.f;
            }
            break;
          case PACT_POSITION_X:
          case PACT_POSITION_Y:
          case PACT_POSITION_Z:
          case PACT_POSITION_W:
          case PACT_POINT_SIZE:
          case PACT_ATTRIBUTE:
          case PACT_VARYING: {
            if (pacd->attrib_index_ != -1) {
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
            else {
              /* Cannot gather attribs that are unbound */
            }
            break;
          }
        }
        break;
      }
      case PADT_UINT8: {
        uint8_t *restrict pu8 = (uint8_t *restrict)data;
        switch (pacd->col_type_) {
          case PACT_EXECUTION_CHAIN:
            for (row = 0; row < num_rows; ++row) {
              *pu8++ = 1;
            }
            break;
          case PACT_POSITION_X:
          case PACT_POSITION_Y:
          case PACT_POSITION_Z:
          case PACT_POSITION_W:
          case PACT_POINT_SIZE:
          case PACT_ATTRIBUTE:
          case PACT_VARYING:
            if (pacd->attrib_index_ != -1) {
              struct attrib *attr = as->attribs_ + pacd->attrib_index_;

              if ((attr->size_ <= pacd->attrib_element_index_) ||
                  !attr->enabled_) {
                uint8_t u8 = (uint8_t)attr->generic_values_[pacd->attrib_element_index_];
                for (row = 0; row < num_rows; ++row) {
                  *pu8++ = u8;
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
                    if (attr->stride_ != sizeof(int8_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pi8 = (int8_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu8++ = (uint8_t)*pi8;
                      }
                    }
                    else {
                      pi8 = (int8_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu8++ = (uint8_t)pi8[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_UNSIGNED_BYTE: {
                    p = ((uint8_t *)p) + pacd->attrib_element_index_;
                    uint8_t *restrict psu8;
                    if (attr->stride_ != sizeof(uint8_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        psu8 = (uint8_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu8++ = (uint8_t)*psu8;
                      }
                    }
                    else {
                      psu8 = (uint8_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu8++ = (uint8_t)psu8[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_SHORT: {
                    p = ((int16_t *)p) + pacd->attrib_element_index_;
                    int16_t *restrict pi16;
                    if (attr->stride_ != sizeof(int16_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pi16 = (int16_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu8++ = (uint8_t)*pi16;
                      }
                    }
                    else {
                      pi16 = (int16_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu8++ = (uint8_t)pi16[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_UNSIGNED_SHORT: {
                    p = ((uint16_t *)p) + pacd->attrib_element_index_;
                    uint16_t *restrict pu16;
                    if (attr->stride_ != sizeof(uint16_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pu16 = (uint16_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu8++ = (uint8_t)*pu16;
                      }
                    }
                    else {
                      pu16 = (uint16_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu8++ = (uint8_t)pu16[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_INT: {
                    p = ((int32_t *)p) + pacd->attrib_element_index_;
                    int32_t *restrict pi32;
                    if (attr->stride_ != sizeof(int32_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pi32 = (int32_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu8++ = (uint8_t)*pi32;
                      }
                    }
                    else {
                      pi32 = (int32_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu8++ = (uint8_t)pi32[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_UNSIGNED_INT: {
                    p = ((uint32_t *)p) + pacd->attrib_element_index_;
                    uint32_t *restrict pu32;
                    if (attr->stride_ != sizeof(uint32_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pu32 = (uint32_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu8++ = (uint8_t)*pu32;
                      }
                    }
                    else {
                      pu32 = (uint32_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu8++ = (uint8_t)pu32[indices[row]];
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
                        *pu8++ = (uint8_t)((*pfixed) >> 16);
                      }
                    }
                    else {
                      pfixed = (int32_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu8++ = (uint8_t)(pfixed[indices[row]] >> 16);
                      }
                    }
                    break;
                  }
                  case ADT_FLOAT: {
                    p = ((float *)p) + pacd->attrib_element_index_;
                    float *restrict pf;
                    if (attr->stride_ != sizeof(float)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pf = (float *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu8++ = (uint8_t)*pf;
                      }
                    }
                    else {
                      pf = (float *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu8++ = (uint8_t)pf[indices[row]];
                      }
                    }
                    break;
                  }
                }
              }
            }
            else {
              /* Cannot gather attribs that are unbound */
            }
            break;
        }
        break;
      }
      case PADT_UINT16: {
        uint16_t *restrict pu16 = ((uint16_t *restrict)data) + pa->num_rows_;
        switch (pacd->col_type_) {
          case PACT_EXECUTION_CHAIN:
            for (row = 0; row < num_rows; ++row) {
              *pu16++ = 1;
            }
            break;
          case PACT_POSITION_X:
          case PACT_POSITION_Y:
          case PACT_POSITION_Z:
          case PACT_POSITION_W:
          case PACT_POINT_SIZE:
          case PACT_ATTRIBUTE:
          case PACT_VARYING:
            if (pacd->attrib_index_ != -1) {
              struct attrib *attr = as->attribs_ + pacd->attrib_index_;

              if ((attr->size_ <= pacd->attrib_element_index_) ||
                !attr->enabled_) {
                uint16_t u16 = (uint16_t)attr->generic_values_[pacd->attrib_element_index_];
                for (row = 0; row < num_rows; ++row) {
                  *pu16++ = u16;
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
                    if (attr->stride_ != sizeof(int8_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pi8 = (int8_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu16++ = (uint16_t)*pi8;
                      }
                    }
                    else {
                      pi8 = (int8_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu16++ = (uint16_t)pi8[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_UNSIGNED_BYTE: {
                    p = ((uint8_t *)p) + pacd->attrib_element_index_;
                    uint8_t *restrict pu8;
                    if (attr->stride_ != sizeof(uint8_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pu8 = (uint8_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu16++ = (uint16_t)*pu8;
                      }
                    }
                    else {
                      pu8 = (uint8_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu16++ = (uint16_t)pu8[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_SHORT: {
                    p = ((int16_t *)p) + pacd->attrib_element_index_;
                    int16_t *restrict pi16;
                    if (attr->stride_ != sizeof(int16_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pi16 = (int16_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu16++ = (uint16_t)*pi16;
                      }
                    }
                    else {
                      pi16 = (int16_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu16++ = (uint16_t)pi16[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_UNSIGNED_SHORT: {
                    p = ((uint16_t *)p) + pacd->attrib_element_index_;
                    uint16_t *restrict psu16;
                    if (attr->stride_ != sizeof(uint16_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        psu16 = (uint16_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu16++ = *psu16;
                      }
                    }
                    else {
                      psu16 = (uint16_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu16++ = psu16[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_INT: {
                    p = ((int32_t *)p) + pacd->attrib_element_index_;
                    int32_t *restrict pi32;
                    if (attr->stride_ != sizeof(int32_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pi32 = (int32_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu16++ = (uint16_t)*pi32;
                      }
                    }
                    else {
                      pi32 = (int32_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu16++ = (uint16_t)pi32[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_UNSIGNED_INT: {
                    p = ((uint32_t *)p) + pacd->attrib_element_index_;
                    uint32_t *restrict pu32;
                    if (attr->stride_ != sizeof(uint32_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pu32 = (uint32_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu16++ = (uint16_t)*pu32;
                      }
                    }
                    else {
                      pu32 = (uint32_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu16++ = (uint16_t)pu16[indices[row]];
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
                        *pu16++ = (uint16_t)((*pfixed) >> 16);
                      }
                    }
                    else {
                      pfixed = (int32_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu16++ = (uint16_t)(pfixed[indices[row]] >> 16);
                      }
                    }
                    break;
                  }
                  case ADT_FLOAT: {
                    p = ((float *)p) + pacd->attrib_element_index_;
                    float *restrict pf;
                    if (attr->stride_ != sizeof(float)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pf = (float *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pu16++ = (uint16_t)*pf;
                      }
                    }
                    else {
                      pf = (float *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pu16++ = (uint16_t)pf[indices[row]];
                      }
                    }
                    break;
                  }
                }
              }
            }
            else {
              /* Cannot gather attribs that are unbound */
            }
            break;
        }
        break;
      }
      case PADT_INT32: {
        int32_t *restrict pi32 = (int32_t *restrict)data;
        switch (pacd->col_type_) {
          case PACT_EXECUTION_CHAIN:
            for (row = 0; row < num_rows; ++row) {
              *pi32++ = 1;
            }
            break;
          case PACT_POSITION_X:
          case PACT_POSITION_Y:
          case PACT_POSITION_Z:
          case PACT_POSITION_W:
          case PACT_POINT_SIZE:
          case PACT_ATTRIBUTE:
          case PACT_VARYING:
            if (pacd->attrib_index_ != -1) {
              struct attrib *attr = as->attribs_ + pacd->attrib_index_;

              if ((attr->size_ <= pacd->attrib_element_index_) ||
                !attr->enabled_) {
                uint16_t u16 = (uint16_t)attr->generic_values_[pacd->attrib_element_index_];
                for (row = 0; row < num_rows; ++row) {
                  *pi32++ = u16;
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
                    if (attr->stride_ != sizeof(int8_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pi8 = (int8_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pi32++ = (int32_t)*pi8;
                      }
                    }
                    else {
                      pi8 = (int8_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pi32++ = (int32_t)pi8[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_UNSIGNED_BYTE: {
                    p = ((uint8_t *)p) + pacd->attrib_element_index_;
                    uint8_t *restrict pu8;
                    if (attr->stride_ != sizeof(uint8_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pu8 = (uint8_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pi32++ = (int32_t)*pu8;
                      }
                    }
                    else {
                      pu8 = (uint8_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pi32++ = (int32_t)pu8[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_SHORT: {
                    p = ((int16_t *)p) + pacd->attrib_element_index_;
                    int16_t *restrict pi16;
                    if (attr->stride_ != sizeof(int16_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pi16 = (int16_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pi32++ = (int32_t)*pi16;
                      }
                    }
                    else {
                      pi16 = (int16_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pi32++ = (int32_t)pi16[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_UNSIGNED_SHORT: {
                    p = ((uint16_t *)p) + pacd->attrib_element_index_;
                    uint16_t *restrict pu16;
                    if (attr->stride_ != sizeof(uint16_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pu16 = (uint16_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pi32++ = (int32_t)*pu16;
                      }
                    }
                    else {
                      pu16 = (uint16_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pi32++ = (int32_t)pu16[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_INT: {
                    p = ((int32_t *)p) + pacd->attrib_element_index_;
                    int32_t *restrict psi32;
                    if (attr->stride_ != sizeof(int32_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        psi32 = (int32_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pi32++ = *psi32;
                      }
                    }
                    else {
                      psi32 = (int32_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pi32++ = psi32[indices[row]];
                      }
                    }
                    break;
                  }
                  case ADT_UNSIGNED_INT: {
                    p = ((uint32_t *)p) + pacd->attrib_element_index_;
                    uint32_t *restrict pu32;
                    if (attr->stride_ != sizeof(uint32_t)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pu32 = (uint32_t *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pi32++ = (int32_t)*pu32;
                      }
                    }
                    else {
                      pu32 = (uint32_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pi32++ = (int32_t)pu32[indices[row]];
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
                        *pi32++ = (int32_t)((*pfixed) >> 16);
                      }
                    }
                    else {
                      pfixed = (int32_t *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pi32++ = (int32_t)(pfixed[indices[row]] >> 16);
                      }
                    }
                    break;
                  }
                  case ADT_FLOAT: {
                    p = ((float *)p) + pacd->attrib_element_index_;
                    float *restrict pf;
                    if (attr->stride_ != sizeof(float)) {
                      size_t stride = attr->stride_;
                      for (row = 0; row < num_rows; ++row) {
                        pf = (float *restrict)(((uint8_t *)p) + stride * indices[row]);
                        *pi32++ = (int32_t)*pf;
                      }
                    }
                    else {
                      pf = (float *restrict)p;
                      for (row = 0; row < num_rows; ++row) {
                        *pi32++ = (int32_t)pf[indices[row]];
                      }
                    }
                    break;
                  }
                }
              }
            }
            else {
              /* Cannot gather attribs that are unbound */
            }
            break;
        }
        break;
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

void primitive_assembly_draw_elements(struct primitive_assembly *pa,
                                      struct attrib_set *as,
                                      struct clipping_stage *cs,
                                      struct rasterizer *ras,
                                      struct fragment_buffer *fragbuf,
                                      int32_t vp_x,
                                      int32_t vp_y,
                                      uint32_t vp_width,
                                      uint32_t vp_height,
                                      float depth_range_near,
                                      float depth_range_far,
                                      uint32_t screen_width,
                                      uint32_t screen_height,
                                      uint32_t max_z,
                                      uint8_t *rgba, size_t rgba_stride,
                                      uint8_t *zbuf, size_t zbuf_stride, size_t zbuf_step,
                                      int enable_red, int enable_green, int enable_blue, int enable_alpha,
                                      blend_eq_t rgb_eq, blend_eq_t alpha_eq,
                                      blend_func_t src_rgb_fn, blend_func_t src_alpha_fn,
                                      blend_func_t dst_rgb_fn, blend_func_t dst_alpha_fn,
                                      uint8_t constant_red, uint8_t constant_grn, uint8_t constant_blu, uint8_t constant_alpha,
                                      primitive_assembly_mode_t mode, 
                                      size_t num_elements,
                                      primitive_assembly_index_type_t index_type,
                                      void *indices) {
  int r;

  // XXX: Configure pipeline to handle points, lines or triangles, depending on what mode is.

  do {
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

    if (r) {
      while (primitive_assembly_gather_attribs(pa, as)) {

        // XXX: Run vertex shader on top of the primitive assembly here...

        switch (mode) {
          case PAM_POINTS:
            // XXX: Handle points..
            break;
          case PAM_LINES:
          case PAM_LINE_STRIP:
          case PAM_LINE_LOOP:
            // XXX: Handle lines
            break;
          case PAM_TRIANGLES:
          case PAM_TRIANGLE_STRIP:
          case PAM_TRIANGLE_FAN: {
            size_t tri_row_index;
            for (tri_row_index = 0; tri_row_index < pa->num_rows_; tri_row_index += 3) {
              float *iv0, *iv1, *iv2;
              iv0 = cs->input_varyings_;
              iv1 = iv0 + cs->num_varyings_;
              iv2 = iv1 + cs->num_varyings_;
              iv0[CLIPPING_STAGE_IDX_X] = ((float *)(pa->column_data_[PAC_IDX_POSITION_X]))[tri_row_index];
              iv0[CLIPPING_STAGE_IDX_Y] = ((float *)(pa->column_data_[PAC_IDX_POSITION_Y]))[tri_row_index];
              iv0[CLIPPING_STAGE_IDX_Z] = ((float *)(pa->column_data_[PAC_IDX_POSITION_Z]))[tri_row_index];
              iv0[CLIPPING_STAGE_IDX_W] = ((float *)(pa->column_data_[PAC_IDX_POSITION_W]))[tri_row_index];
              iv1[CLIPPING_STAGE_IDX_X] = ((float *)(pa->column_data_[PAC_IDX_POSITION_X]))[tri_row_index + 1];
              iv1[CLIPPING_STAGE_IDX_Y] = ((float *)(pa->column_data_[PAC_IDX_POSITION_Y]))[tri_row_index + 1];
              iv1[CLIPPING_STAGE_IDX_Z] = ((float *)(pa->column_data_[PAC_IDX_POSITION_Z]))[tri_row_index + 1];
              iv1[CLIPPING_STAGE_IDX_W] = ((float *)(pa->column_data_[PAC_IDX_POSITION_W]))[tri_row_index + 1];
              iv2[CLIPPING_STAGE_IDX_X] = ((float *)(pa->column_data_[PAC_IDX_POSITION_X]))[tri_row_index + 2];
              iv2[CLIPPING_STAGE_IDX_Y] = ((float *)(pa->column_data_[PAC_IDX_POSITION_Y]))[tri_row_index + 2];
              iv2[CLIPPING_STAGE_IDX_Z] = ((float *)(pa->column_data_[PAC_IDX_POSITION_Z]))[tri_row_index + 2];
              iv2[CLIPPING_STAGE_IDX_W] = ((float *)(pa->column_data_[PAC_IDX_POSITION_W]))[tri_row_index + 2];

              if (clipping_stage_process_triangle(cs)) {
                /* XXX: Ugly reliance on size here, we stitch the SX,SY,SZ variables alongside 
                 *      the clipping output triangles, however the one is in floats, the other is in
                 *      int32_t's. We assume sizeof(float) == sizeof(int32_t). Find some better way */
                assert(sizeof(float) == sizeof(int32_t));

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

                size_t clip_tri_idx;
                for (clip_tri_idx = 0; (clip_tri_idx + 2) < (3 * cs->num_triangles_in_b_); clip_tri_idx += 3) {
                  float *v0 = cs->triangle_varyings_b_ + cs->num_varyings_ * clip_tri_idx;
                  float *v1 = v0 + cs->num_varyings_;
                  float *v2 = v1 + cs->num_varyings_;
                  int32_t sx0 = *(int32_t *)(v0+CLIPPING_STAGE_IDX_SX);
                  int32_t sy0 = *(int32_t *)(v0+CLIPPING_STAGE_IDX_SY);
                  int32_t sz0 = *(int32_t *)(v0+CLIPPING_STAGE_IDX_SZ);
                  int32_t sx1 = *(int32_t *)(v1+CLIPPING_STAGE_IDX_SX);
                  int32_t sy1 = *(int32_t *)(v1+CLIPPING_STAGE_IDX_SY);
                  int32_t sz1 = *(int32_t *)(v1+CLIPPING_STAGE_IDX_SZ);
                  int32_t sx2 = *(int32_t *)(v2+CLIPPING_STAGE_IDX_SX);
                  int32_t sy2 = *(int32_t *)(v2+CLIPPING_STAGE_IDX_SY);
                  int32_t sz2 = *(int32_t *)(v2+CLIPPING_STAGE_IDX_SZ);

                  while (rasterizer_triangle(ras, fragbuf, 
                                             rgba, 256*4,     // bitmap
                                             zbuf, zbuf_stride, zbuf_step,  // z-buffer
                                             0, 0, 256, 256,  // scissor-rect
                                             sx0, sy0, sz0,
                                             sx1, sy1, sz1,
                                             sx2, sy2, sz2)) {


                    // XXX: Run fragment shader on top of the fragment buffer here...
                                        
                    // Run an experimental "fragment shader" to fill out the color to be identical to
                    // our prior experiments.
                    size_t frag_row;
                    for (frag_row = 0; frag_row < fragbuf->num_rows_; ++frag_row) {
                      uint32_t z = ((uint32_t *restrict)fragbuf->column_data_[FB_IDX_ZBUF_VALUE])[frag_row];
                      ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_FRAG_RED])[frag_row] = (uint8_t)((z & 1) ? 0xCF : 0x3F);
                      ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_FRAG_GREEN])[frag_row] = (uint8_t)((z & 1) ? 0xCF : 0x3F);
                      ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_FRAG_BLUE])[frag_row] = (uint8_t)((z & 1) ? 0xCF : 0x3F);
                      ((uint8_t *restrict)fragbuf->column_data_[FB_IDX_FRAG_ALPHA])[frag_row] = 0xFF;
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
                          fragbuf->column_data_[FB_IDX_TEMP_BYTE_7],

                          fragbuf->column_data_[FB_IDX_TEMP_BYTE_8],
                          fragbuf->column_data_[FB_IDX_TEMP_BYTE_9],
                          fragbuf->column_data_[FB_IDX_TEMP_BYTE_10],
                          fragbuf->column_data_[FB_IDX_TEMP_BYTE_11]);

                    fragbuf->num_rows_ = 0;
                  }
                }
              }
            }
            pa->num_rows_ = 0;
            
            break;
          }
        }
      }

    }
  } while (r == 1);
}
