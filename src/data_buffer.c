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
#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef DATA_BUFFER_H_INCLUDED
#define DATA_BUFFER_H_INCLUDED
#include "data_buffer.h"
#endif

void data_buffer_init(struct data_buffer *db) {
  db->data_ = NULL;
  db->size_ = 0;
}

void data_buffer_cleanup(struct data_buffer *db) {
  if (db->data_) free(db->data_);
}

int data_buffer_set_data(struct data_buffer *db, size_t size, void *data) {
  if (db->size_ != size) {
    void *p = realloc(db->data_, size);
    if (!p) {
      return -1;
    }
    db->data_ = p;
    if (db->size_ < size) {
      memset(((char *)db->data_) + db->size_, 0, size - db->size_);
    }
    db->size_ = size;
  }
  if (data) {
    memcpy(db->data_, data, size);
  }
  return 0;
}

void data_buffer_copy_data(struct data_buffer *db, size_t offset, size_t size, void *src) {
  if (offset >= db->size_) return;
  
  size_t end_offset = offset + size;
  if (end_offset < offset) {
    /* overflow, but this also implies we clamp it */
    end_offset = db->size_;
  }
  if (end_offset > db->size_) {
    end_offset = db->size_;
  }
  memcpy(((char *)db->data_) + offset, src, end_offset - offset);
}
