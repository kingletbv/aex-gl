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

#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct data_buffer {
  size_t size_;
  void *data_;
};

void data_buffer_init(struct data_buffer *db);
void data_buffer_cleanup(struct data_buffer *db);

/* size is the new size of the data_buffer, data_buffer::data_ will be
 * reallocated (or freed if 0) to this size. data points to a region of
 * memory where the data is with which to initialize the newly sized data_.
 * If data is NULL, the resize occurs, any existing data prior to the
 * resize will remain (but might be truncated if the size is smaller),
 * alternatively, if the size is larger, that space will be initialized with
 * 0 bytes.
 * Returns 0 upon success, non-zero upon memory allocation failure. */
int data_buffer_set_data(struct data_buffer *db, size_t size, const void *data);

/* copies the data into the data_buffer db without resizing it or changing it otherwise.
 * The data is copied at offset and with length size. These values are clamped to fit
 * the actual size of the data_buffer (so not all data might be copied, a best effort
 * is made, depending on the current size of the buffer.) Because of this, this function
 * does not fail. */
void data_buffer_copy_data(struct data_buffer *db, size_t offset, size_t size, const void *src);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DATA_BUFFER_H */
