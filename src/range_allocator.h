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


/* self-test, returns non-zero upon failure, zero upon pass. */
int ral_range_test(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* RANGE_ALLOCATOR_H */
