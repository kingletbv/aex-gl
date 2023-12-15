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

#ifndef FRAGMENT_BUFFER_H
#define FRAGMENT_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Number of rows in the fragment buffer.
 * Best keep this a multiple of 16 as small FBCT_UINT8 datatypes then don't
 * cause misalignment issues (there is no sophistication in the layout of the
 * initial buffers.) */
#define FRAGMENT_BUFFER_MAX_ROWS 256

typedef enum fragment_buffer_data_type {
  FBCT_NONE,
  FBCT_PTR,
  FBCT_FLOAT,
  FBCT_UINT8,
  FBCT_UINT16,
  FBCT_UINT32,
  FBCT_INT32
  // .. the rest, presumably ..
} frag_buf_data_type_t;

typedef enum fragment_buffer_column_type {
  FBCT_EXECUTION_CHAIN, /* FNCT_UINT8: Execution chain, delta of index to the next entry in the same execution set, e.g. "1" is next row.  */
  FBCT_MASK,            /* FBCT_UINT8: Either 0xFF, or 0x00. If 0x00, then the fragment generates no output (but still executes for side-effects) */
  FBCT_PIXEL_PTR,       /* FBCT_PTR: Pointer to the pixel to be written; may be NULL only if FBCT_MASK is 0x00. */
  FBCT_ZBUF_PTR,        /* FBCT_PTR: Pointer to the z-buffer depth value to be compared and tested against (only if after fragment shader); may be NULL only if FBCT_MASK is 0x00. */
  FBCT_X_COORD,         /* FBCT_INT32: X screen coordinate of pixel */
  FBCT_Y_COORD,         /* FBCT_INT32: Y screen coordinate of pixel */
  FBCT_ZBUF_VALUE       /* FBCT_UINT32: z-buffer value range 0 to (2^z-buffer-depth - 1) */
} frag_buf_col_type_t;

/* Fixed indices into column_data_ and column_descriptions_ that lead to known, hard-coded, columns. */
#define FB_IDX_EXECUTION_CHAIN 0
#define FB_IDX_MASK            1
#define FB_IDX_PIXEL_PTR       2
#define FB_IDX_ZBUF_PTR        3
#define FB_IDX_X_COORD         4
#define FB_IDX_Y_COORD         5
#define FB_IDX_ZBUF_VALUE      6
#define FB_IDX_NUM_FIXED_IDX   7

struct fragment_buffer_column_descriptor {
  /* Type of fragment buffer column (e.g. the semantic value of it) */
  frag_buf_col_type_t col_type_;

  /* Datatype of fragment buffer column */
  frag_buf_data_type_t data_type_;
};

struct fragment_buffer {
  /* Number of rows, maximum is FRAGMENT_BUF_MAX_ROWS */
  size_t num_rows_;

  size_t num_cols_;
  
  /* Buffer allocated from which we allocate the fixed fragment arrays (e.g. the
   * ones with an FB_IDX_xxx constant. */
  void *fixed_slab_;

  /* Array of num_cols_ pointers to data. 
   * Each column_data_[x] is a pointer to an array of the corresponding type in column_descriptors_[x].data_type_
   * Each array has FRAGMENT_BUFFER_MAX_ROWS allocated, how many of those rows are in use is determined by num_rows_
   * (but the allocation size is fixed and non-dynamic.) */
  void **column_data_;

  /* Array of num_cols_ column descriptions */
  struct fragment_buffer_column_descriptor *column_descriptions_;
};

/* Initializes a fragment buffer, but does not allocate any memory for it so consequently it cannot be used yet.
 * The benefit of this function is it cannot fail, and after calling it you can safely invoke fragment_buffer_cleanup()
 * regardless of whether fragment_buffer_alloc_buffers() succeeds or fails. */
void fragment_buffer_init(struct fragment_buffer *fb);

/* Second phase of fragment buffer initialization, allocates memory for the fragment buffer.
 * Returns 0 on success, -1 on failure.
 * Only after success is the fragment buffer ready for use. */
int fragment_buffer_alloc_buffers(struct fragment_buffer *fb);

void fragment_buffer_cleanup(struct fragment_buffer *fb);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FRAGMENT_BUFFER_H */