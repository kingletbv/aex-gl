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

#ifndef REF_RANGE_ALLOCATOR_H
#define REF_RANGE_ALLOCATOR_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct ref_range {
  int pos_is_red_:1, sz_is_red_:1;
  struct ref_range *pos_parent_, *pos_left_, *pos_right_, *pos_next_, *pos_prev_;
  struct ref_range *sz_parent_, *sz_left_, *sz_right_, *sz_next_, *sz_prev_;

  uintptr_t at_;

  /* Reference count for the range pos_prev_->at_ to, but not including, at_; if this ref_range
   * is the first in ref_range_allocator->pos_seq_ then the range is from 0 to, but not including, at_. */
  int refcount_;
};

struct ref_range_allocator {
  struct ref_range *sz_root_;
  struct ref_range *sz_seq_;
  struct ref_range *pos_root_;
  struct ref_range *pos_seq_;
};



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* REF_RANGE_ALLOCATOR */
