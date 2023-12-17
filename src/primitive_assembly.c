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

#ifndef PRIMITIVE_ASSEMBLY_H_INCLUDED
#define PRIMITIVE_ASSEMBLY_H_INCLUDED
#include "primitive_assembly.h"
#endif

void primitive_assembly_init(struct primitive_assembly *pa) {
  pa->num_rows_ = 0;
  pa->num_cols_ = 0;
  pa->column_descriptors_ = NULL;
  pa->column_data_ = NULL;
  pa->slab_ = NULL;
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

