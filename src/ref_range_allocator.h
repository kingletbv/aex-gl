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

  /* Debug aid */
  int sanity_;
};

struct ref_range_allocator {
  /* RB tree in ascending order of size and position */
  struct ref_range *sz_root_;
  struct ref_range *sz_seq_;

  /* RB tree in order of position */
  struct ref_range *pos_root_;
  struct ref_range *pos_seq_;
  
  /* End of highest non-zero refcount range */
  uintptr_t watermark_;
};

void ref_range_allocator_init(struct ref_range_allocator *rra);
void ref_range_allocator_cleanup(struct ref_range_allocator *rra);

/* marks the range from from (inclusive), to to (exclusive) as unavailable for allocation.
 * Returns zero upon success, non-zero upon memory failure. from must be less than to (and 
 * may not be equal.)
 * This function may be called multiple times over the same or overlapping ranges, as long as
 * each such ref_range_mark_range_allocated() call is paired by an equal and matching
 * ref_range_mark_free() call. The internal implementation uses a refcount to facilitate this.
 */
int ref_range_mark_range_allocated(struct ref_range_allocator *rra, uintptr_t from, uintptr_t to);

/* marks the range from from (inclusive), to to (exclusive) as free. Returns zero upon success,
 * non-zero upon memory failure. from must be less than to (and may not be equal.)
 * Note that the implementation is refcounted, so multiple ref_range_mark_allocated() calls,
 * or a combination of a ref_range_alloc() call with multiple ref_range_mark_allocated() calls,
 * may be matched with multiple ref_range_mark_range_free() calls. */
int ref_range_mark_range_free(struct ref_range_allocator *rra, uintptr_t from, uintptr_t to);

/* allocates "size" consecutive units, returning them in *result. The range returned is the
 * best fit of all available ranges (where "best-fit" means the range will occupy the smallest
 * area that can still fit it), if multiple ranges match, then the lowest range is returned.
 * The allocated range should be matched with a single ref_range_mark_range_free() call to
 * free the range again for future allocations. */
int ref_range_alloc(struct ref_range_allocator *rra, uintptr_t size, uintptr_t *result);

/* returns the refcount value of the range that contains position "at" */
int ref_range_get_ref_at(struct ref_range_allocator *rra, uintptr_t at);

/* self-test, returns non-zero upon failure, zero upon pass. */
int ref_range_test(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* REF_RANGE_ALLOCATOR */
