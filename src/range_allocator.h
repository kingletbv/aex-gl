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

#ifndef RANGE_ALLOCATOR_H
#define RANGE_ALLOCATOR_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct ral_range {
  int pos_is_red_:1, sz_is_red_:1;
  struct ral_range *pos_parent_, *pos_left_, *pos_right_, *pos_next_, *pos_prev_;
  struct ral_range *sz_parent_, *sz_left_, *sz_right_, *sz_next_, *sz_prev_;

  uintptr_t from_, to_;

  /* Little helper value for sanity calculations */
  int sanity_;
};

// Basic idea is to have:
// - One tree representing the intervals ordered by range - this is the Positional tree.
// - Another tree representing the intervals ordered by size of the interval - this is the Size tree
// When you need to allocate a certain size, you look through the size tree to find the best fit.
// When you need to free or allocate (lock or unlock) a specific range, you look through the intervals by range to find the nodes
// to change.
// For the intervals ordered by range, it is important we can find adjacent ranges, to merge or split, as appropriate.
// For the intervals ordered by the size of the interval, we need to remove and reinsert ranges, as the range grows and shrinks.
// (E.g. if we release a range of 3, and this adds into a range of 5 to produce a range of 8, then that should be modeled as
// a growth from 5 to 8, which may or may not, require an edit of the tree.)
// 

struct ral_range_allocator {
  struct ral_range *sz_root_;
  struct ral_range *sz_seq_;
  struct ral_range *pos_root_;
  struct ral_range *pos_seq_;

  /* After the watermark_ everything is free; the last ral_range therefore does not
   * align with the watermark_ as then the ral_range would be unnecessary and the
   * watermark_ could be lower. */
  uintptr_t watermark_;
};

void ral_range_allocator_init(struct ral_range_allocator *ral);
void ral_range_allocator_cleanup(struct ral_range_allocator *ral);

/* marks the range from from (inclusive), to to (exclusive) as allocated. Returns zero upon success,
 * non-zero upon memory failure. from must be less than to (and may not be equal.) */
int ral_range_mark_range_allocated(struct ral_range_allocator *ral, uintptr_t from, uintptr_t to);

/* marks the range from from (inclusive), to to (exclusive) as free. Returns zero upon success,
 * non-zero upon memory failure. from must be less than to (and may not be equal.) */
int ral_range_mark_range_free(struct ral_range_allocator *ral, uintptr_t from, uintptr_t to);

/* allocates "size" consecutive units, returning them in *result. The range returned is the
 * best fit of all available ranges (where "best-fit" means the range will occupy the smallest
 * area that can still fit it), if multiple ranges match, then the lowest range is returned. */
int ral_range_alloc(struct ral_range_allocator *ral, uintptr_t size, uintptr_t *result);

/* self-test, returns non-zero upon failure, zero upon pass. */
int ral_range_test(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* RANGE_ALLOCATOR_H */
