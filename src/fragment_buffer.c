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

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef FRAGMENT_BUFFER_H_INCLUDED
#define FRAGMENT_BUFFER_H_INCLUDED
#include "fragment_buffer.h"
#endif

void fragment_buffer_init(struct fragment_buffer *fb) {
  fb->num_rows_ = 0;
  fb->num_cols_ = 0;
  fb->column_descriptions_ = NULL;
  fb->column_data_ = NULL;
  fb->fixed_slab_ = NULL;
}

int fragment_buffer_alloc_buffers(struct fragment_buffer *fb) {
  fb->num_rows_ = 0;
  fb->num_cols_ = FB_IDX_NUM_FIXED_IDX;
  if (fb->column_descriptions_) free(fb->column_descriptions_);
  fb->column_descriptions_ = (struct fragment_buffer_column_descriptor *)malloc(sizeof(struct fragment_buffer_column_descriptor) * fb->num_cols_);
  if (!fb->column_descriptions_) {
    fb->num_cols_ = 0;
    return -1;
  }

  size_t rowsize = sizeof(uint8_t)  /* FBCT_EXECUTION_CHAIN */
                 + sizeof(uint8_t)  /* FBCT_MASK */
                 + sizeof(void*)    /* FBCT_PIXEL_PTR */
                 + sizeof(void*)    /* FBCT_ZBUF_PTR */
                 + sizeof(int32_t)  /* FBCT_X_COORD */
                 + sizeof(int32_t)  /* FBCT_Y_COORD */
                 + sizeof(uint32_t) /* FBCT_ZBUF_VALUE */;
  size_t size_needed = rowsize * FRAGMENT_BUFFER_MAX_ROWS;
  if (fb->fixed_slab_) free(fb->fixed_slab_);
  fb->fixed_slab_ = malloc(size_needed);
  if (!fb->fixed_slab_) {
    free(fb->column_descriptions_);
    fb->column_descriptions_ = NULL;
    return -1;
  }

  if (fb->column_data_) free(fb->column_data_);
  fb->column_data_ = (void **)malloc(sizeof(void *) * fb->num_cols_);
  if (!fb->column_data_) {
    free(fb->column_descriptions_);
    fb->column_descriptions_ = NULL;
    free(fb->fixed_slab_);
    fb->fixed_slab_ = NULL;
    return -1;
  }

  uint8_t *mem = (uint8_t *)fb->fixed_slab_;

  fb->column_descriptions_[FB_IDX_EXECUTION_CHAIN].col_type_ = FBCT_EXECUTION_CHAIN;
  fb->column_descriptions_[FB_IDX_EXECUTION_CHAIN].data_type_ = FBCT_UINT8;
  fb->column_data_[FB_IDX_EXECUTION_CHAIN] = mem;
  mem += sizeof(uint8_t) * FRAGMENT_BUFFER_MAX_ROWS;

  fb->column_descriptions_[FB_IDX_MASK].col_type_ = FBCT_MASK;
  fb->column_descriptions_[FB_IDX_MASK].data_type_ = FBCT_UINT8;
  fb->column_data_[FB_IDX_MASK] = mem;
  mem += sizeof(uint8_t) * FRAGMENT_BUFFER_MAX_ROWS;

  fb->column_descriptions_[FB_IDX_PIXEL_PTR].col_type_ = FBCT_PIXEL_PTR;
  fb->column_descriptions_[FB_IDX_PIXEL_PTR].data_type_ = FBCT_PTR;
  fb->column_data_[FB_IDX_PIXEL_PTR] = mem;
  mem += sizeof(void*) * FRAGMENT_BUFFER_MAX_ROWS;
        
  fb->column_descriptions_[FB_IDX_ZBUF_PTR].col_type_ = FBCT_ZBUF_PTR;
  fb->column_descriptions_[FB_IDX_ZBUF_PTR].data_type_ = FBCT_PTR;
  fb->column_data_[FB_IDX_ZBUF_PTR] = mem;
  mem += sizeof(void*) * FRAGMENT_BUFFER_MAX_ROWS;
        
  fb->column_descriptions_[FB_IDX_X_COORD].col_type_ = FBCT_X_COORD;
  fb->column_descriptions_[FB_IDX_X_COORD].data_type_ = FBCT_INT32;
  fb->column_data_[FB_IDX_X_COORD] = mem;
  mem += sizeof(int32_t) * FRAGMENT_BUFFER_MAX_ROWS;
        
  fb->column_descriptions_[FB_IDX_Y_COORD].col_type_ = FBCT_Y_COORD;
  fb->column_descriptions_[FB_IDX_Y_COORD].data_type_ = FBCT_INT32;
  fb->column_data_[FB_IDX_Y_COORD] = mem;
  mem += sizeof(int32_t) * FRAGMENT_BUFFER_MAX_ROWS;
        
  fb->column_descriptions_[FB_IDX_ZBUF_VALUE].col_type_ = FBCT_ZBUF_VALUE;
  fb->column_descriptions_[FB_IDX_ZBUF_VALUE].data_type_ = FBCT_UINT32;
  fb->column_data_[FB_IDX_ZBUF_VALUE] = mem;
  mem += sizeof(uint32_t) * FRAGMENT_BUFFER_MAX_ROWS;

  size_t size_used = mem - (uint8_t *)fb->fixed_slab_;
  assert(size_used == size_needed);

  return 0;
}

void fragment_buffer_cleanup(struct fragment_buffer *fb) {
  if (fb->column_descriptions_) {
    free(fb->column_descriptions_);
    fb->column_descriptions_ = NULL;
  }
  if (fb->column_data_) {
    free(fb->column_data_);
    fb->column_data_ = NULL;
  }
  if (fb->fixed_slab_) {
    free(fb->fixed_slab_);
    fb->fixed_slab_ = NULL;
  }
  fb->num_rows_ = 0;
  fb->num_cols_ = 0;
}
