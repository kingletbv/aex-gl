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
    size_t num_cols = 5;
    if (pa->column_descriptors_) free(pa->column_descriptors_);
    pa->column_descriptors_ = (struct primitive_assembly_column_descriptor *)malloc(sizeof(struct primitive_assembly_column_descriptor) * num_cols);
    if (!pa->column_descriptors_) {
      return -1;
    }
    pa->num_cols_ = num_cols;
    pa->column_descriptors_[0].col_type_ = PACT_EXECUTION_CHAIN;
    pa->column_descriptors_[0].data_type_ = PADT_UINT16;
    pa->column_descriptors_[1].col_type_ = PACT_POSITION_X;
    pa->column_descriptors_[1].data_type_ = PADT_FLOAT;
    pa->column_descriptors_[2].col_type_ = PACT_POSITION_Y;
    pa->column_descriptors_[2].data_type_ = PADT_FLOAT;
    pa->column_descriptors_[3].col_type_ = PACT_POSITION_Z;
    pa->column_descriptors_[3].data_type_ = PADT_FLOAT;
    pa->column_descriptors_[4].col_type_ = PACT_POSITION_W;
    pa->column_descriptors_[4].data_type_ = PADT_FLOAT;
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
                      float norm = 1.f/255;
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
                      float norm = 1.f/65535;
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
                  case ADT_FIXED: {
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
            break;
        }
        break;
      }
      case PADT_INT32: {
        int32_t *pi32 = (int32_t *)data;
        break;
      }
    }
  }
}
