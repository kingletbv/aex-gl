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

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

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

#ifndef REF_RANGE_ALLOCATOR_H_INCLUDED
#define REF_RANGE_ALLOCATOR_H_INCLUDED
#include "ref_range_allocator.h"
#endif

static void ref_range_sz_remove(struct ref_range_allocator *rra, struct ref_range *s);
static void ref_range_pos_remove(struct ref_range_allocator *rra, struct ref_range *s);

static void ref_range_free(struct ref_range_allocator *rra, struct ref_range *rr);

static int ref_range_sanity_check(struct ref_range_allocator *rra);

void ref_range_allocator_init(struct ref_range_allocator *rra) {
  rra->sz_root_ = rra->sz_seq_ = NULL;
  rra->pos_root_ = rra->pos_seq_ = NULL;
  rra->watermark_ = 0;
}

void ref_range_allocator_cleanup(struct ref_range_allocator *rra) {
  while (rra->pos_seq_) {
    struct ref_range *rr = rra->pos_seq_;
    if (!rr->refcount_) ref_range_sz_remove(rra, rr);
    ref_range_pos_remove(rra, rr);
    ref_range_free(rra, rr);
  }
}

static void ref_range_init(struct ref_range *rr) {
  rr->pos_parent_ = rr->pos_left_ = rr->pos_right_ = rr->pos_next_ = rr->pos_prev_ = NULL;
  rr->sz_parent_ = rr->sz_left_ = rr->sz_right_ = rr->sz_next_ = rr->sz_prev_ = NULL;
  rr->at_ = 0;
}

static void ref_range_cleanup(struct ref_range *rr) {
}

static struct ref_range *ref_range_alloc_range(struct ref_range_allocator *rra) {
  rra;
  struct ref_range *rr = (struct ref_range *)malloc(sizeof(struct ref_range));
  if (!rr) return NULL;
  ref_range_init(rr);
  return rr;
}

static void ref_range_free(struct ref_range_allocator *rra, struct ref_range *rr) {
  rra;
  if (!rr) return;
  ref_range_cleanup(rr);
  free(rr);
}

static struct ref_range *ref_range_sz_rotate_left(struct ref_range_allocator *rra, struct ref_range *A) {
  /*
   *   A            B
   *  / \    ==>   / \
   * 0   B        A   2
   *    / \      / \
   *   1   2    0   1
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct ref_range *B = A->sz_right_;
  A->sz_right_ = B->sz_left_;
  B->sz_left_ = A;

  struct ref_range *A_parent = A->sz_parent_;
  A->sz_parent_ = B;
  if (A->sz_right_) A->sz_right_->sz_parent_ = A;
  B->sz_parent_ = A_parent;
  if (A_parent) {
    if (A_parent->sz_left_ == A) A_parent->sz_left_ = B;
    else /* (A_parent->sz_right_ == A) */ A_parent->sz_right_ = B;
  }
  else {
    /* changing roots */
    rra->sz_root_ = B;
  }

  int root_color = A->sz_is_red_;
  A->sz_is_red_ = B->sz_is_red_;
  B->sz_is_red_ = root_color;

  return B;
}

static struct ref_range *ref_range_sz_rotate_right(struct ref_range_allocator *rra, struct ref_range *B) {
  /*
   *     B          A
   *    / \   =>   / \
   *   A   2      0   B
   *  / \            / \
   * 0   1          1   2
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct ref_range *A = B->sz_left_;
  B->sz_left_ = A->sz_right_;
  A->sz_right_ = B;

  struct ref_range *B_parent = B->sz_parent_;
  B->sz_parent_ = A;
  if (B->sz_left_) B->sz_left_->sz_parent_ = B;
  A->sz_parent_ = B_parent;
  if (B_parent) {
    if (B_parent->sz_left_ == B) B_parent->sz_left_ = A;
    else /* (B_parent->sz_right_ == B) */ B_parent->sz_right_ = A;
  }
  else {
    /* changing roots */
    rra->sz_root_ = A;
  }

  int root_color = B->sz_is_red_;
  B->sz_is_red_ = A->sz_is_red_;
  A->sz_is_red_ = root_color;

  return A;
}

static struct ref_range *ref_range_sz_fixup(struct ref_range_allocator *rra, struct ref_range *z) {
  struct ref_range *x;
  if (!(z->sz_left_ && z->sz_left_->sz_is_red_) && !(z->sz_right_ && z->sz_right_->sz_is_red_)) {
    /* nothing to see here, move along */
    return z->sz_parent_;
  }

  if (z->sz_parent_->sz_left_ == z) {
    if (z->sz_right_ && z->sz_right_->sz_is_red_) {
      z = ref_range_sz_rotate_left(rra, z);
    }
    x = ref_range_sz_rotate_right(rra, z->sz_parent_);
  }
  else /* (z->sz_parent->sz_right == z) */ {
    if (z->sz_left_ && z->sz_left_->sz_is_red_) {
      z = ref_range_sz_rotate_right(rra, z);
    }
    x = ref_range_sz_rotate_left(rra, z->sz_parent_);
  }
  /* Red children both turn black, deficiency resolved, return NULL. */
  x->sz_left_->sz_is_red_ = x->sz_right_->sz_is_red_ = 0;
  return NULL;
}

static void ref_range_sz_remove(struct ref_range_allocator *rra, struct ref_range *s) {
  /* Remove from chain first (easy bit) */
  if (s->sz_next_ != s) {
    s->sz_next_->sz_prev_ = s->sz_prev_;
    s->sz_prev_->sz_next_ = s->sz_next_;
    if (rra->sz_seq_ == s) rra->sz_seq_ = s->sz_next_;
    s->sz_next_ = s->sz_prev_ = NULL;
  }
  else {
    assert(s == rra->sz_seq_);
    s->sz_next_ = s->sz_prev_ = NULL;
    rra->sz_seq_ = NULL;
  }

  if (s->sz_right_) {
    /* Substitute with minimum node in right tree, then delete the minimum node. */
    struct ref_range *rmin = s->sz_right_;
    while (rmin->sz_left_) rmin = rmin->sz_left_;

    /* Swap out s with rmin */
    struct ref_range *parent, *left, *right;
    int is_red;
    parent = rmin->sz_parent_;
    left = rmin->sz_left_;
    right = rmin->sz_right_;
    is_red = rmin->sz_is_red_;
    rmin->sz_parent_ = s->sz_parent_;
    rmin->sz_left_ = s->sz_left_;
    rmin->sz_right_ = s->sz_right_;
    rmin->sz_is_red_ = s->sz_is_red_;
    s->sz_parent_ = (parent == s) ? rmin : parent;
    s->sz_left_ = left;
    s->sz_right_ = right;
    s->sz_is_red_ = is_red;

    /* Fix up parents */
    if (s->sz_parent_->sz_right_ == rmin) {
      s->sz_parent_->sz_right_ = s;
    }
    else if (s->sz_parent_->sz_left_ == rmin) {
      s->sz_parent_->sz_left_ = s;
    }
    if (rmin->sz_parent_) {
      if (rmin->sz_parent_->sz_right_ == s) {
        rmin->sz_parent_->sz_right_ = rmin;
      }
      else if (rmin->sz_parent_->sz_left_ == s) {
        rmin->sz_parent_->sz_left_ = rmin;
      }
    }
    else {
      assert(rra->sz_root_ == s);
      rra->sz_root_ = rmin;
    }

    /* Fix up children */
    if (rmin->sz_left_) rmin->sz_left_->sz_parent_ = rmin;
    if (rmin->sz_right_) rmin->sz_right_->sz_parent_ = rmin;
    /* s->left_ should be NULL as the leftmost child of the right branch */
    assert(!s->sz_left_);
    if (s->sz_right_) s->sz_right_->sz_parent_ = s;

    /* s is now in a new location, guaranteed to be a leaf or degree 1 node */
  }


  if (s->sz_left_) {
    /* left must be a red node as right must be NULL at this point (otherwise
     * we would have an inconsistent number of black nodes from left to right) */
    assert(s->sz_left_->sz_is_red_);
    assert(!s->sz_is_red_);
    assert(!s->sz_right_);
    /* Swap positions for s->sz_left and s, then s has been removed. */
    if (s->sz_parent_) {
      if (s->sz_parent_->sz_left_ == s) {
        s->sz_parent_->sz_left_ = s->sz_left_;
      }
      else if (s->sz_parent_->sz_right_ == s) {
        s->sz_parent_->sz_right_ = s->sz_left_;
      }
    }
    else {
      assert(rra->sz_root_ == s);
      rra->sz_root_ = s->sz_left_;
    }
    s->sz_left_->sz_parent_ = s->sz_parent_;
    s->sz_left_->sz_is_red_ = 0;
    /* Done, s has been evicted from the tree. */
    return ;
  }
  else if (s->sz_right_) {
    /* Same logic as above but now for right, the left branch must be NULL, and
     * the right branch must be red, meaning the s node must be black. */
    assert(s->sz_right_->sz_is_red_);
    assert(!s->sz_is_red_);
    assert(!s->sz_left_);
    /* Swap positions for s->right_ and s */
    if (s->sz_parent_) {
      if (s->sz_parent_->sz_left_ == s) {
        s->sz_parent_->sz_left_ = s->sz_right_;
      }
      else if (s->sz_parent_->sz_right_ == s) {
        s->sz_parent_->sz_right_ = s->sz_right_;
      }
    }
    else {
      assert(rra->sz_root_ == s);
      rra->sz_root_ = s->sz_right_;
    }
    s->sz_right_->sz_parent_ = s->sz_parent_;
    s->sz_right_->sz_is_red_ = 0;
    /* Done, s has been evicted */
    return ;
  }
  else if (s->sz_is_red_) {
    /* !s->sz_left_ && !s->sz_right_ */
    /* No branches (s is a true leaf node), and the color is red, easy removal,
     * patching up parent. */
    if (s->sz_parent_) {
      if (s->sz_parent_->sz_left_ == s) s->sz_parent_->sz_left_ = NULL;
      else if (s->sz_parent_->sz_right_ == s) s->sz_parent_->sz_right_ = NULL;
    }
    else {
      /* No parent, this must be the last node */
      assert(s == rra->sz_root_);
      rra->sz_root_ = NULL;
    }
    /* Done */
    return ;
  }
  else /* (!s->sz_left_ && !s->sz_right_ && !s->sz_is_red_) */ {
    /* s is a true leaf node, and black...
     * when we remove s from the tree, the tree will be imbalanced (for its
     * sibling will have one more black.)
     * one way to view this is to think of s as being "short one black" - however,
     * we cannot change s itself to red, because we're about to remove s. We can
     * make its sibling also "short one black", thereby propagating the deficiency
     * upwards (the siblings will be balanced but now the parent as a whole is
     * "short one black" compared to *its* siblings.) */
    struct ref_range *x = s;
    for (;;) {
      struct ref_range *sib = NULL;

      if (!x->sz_parent_) {
        /* Reached the root; done. */
        if (s->sz_parent_) {
          if (s->sz_parent_->sz_left_ == s) {
            s->sz_parent_->sz_left_ = NULL;
          }
          else if (s->sz_parent_->sz_right_ == s) {
            s->sz_parent_->sz_right_ = NULL;
          }
        }
        else {
          assert(rra->sz_root_ == s);
          rra->sz_root_ = NULL;
        }
        return ;
      }
      sib = (struct ref_range *)(((uintptr_t)x->sz_parent_->sz_left_) ^ ((uintptr_t)x->sz_parent_->sz_right_) ^ ((uintptr_t)x));
      if (!sib->sz_is_red_) {
        /* Sibling is black, we can induce a deficiency by making it red */
        sib->sz_is_red_ = 1;
        x = ref_range_sz_fixup(rra, sib); /* deal with consecutive reds */

        if (!x || x->sz_is_red_) {
          if (x && x->sz_is_red_) {
            /* We can make it black, all is balanced again */
            x->sz_is_red_ = 0;
          }
          /* Deficiency resolved in fixup, all is balanced. */
          if (s->sz_parent_) {
            if (s->sz_parent_->sz_left_ == s) {
              s->sz_parent_->sz_left_ = NULL;
            }
            else if (s->sz_parent_->sz_right_ == s) {
              s->sz_parent_->sz_right_ = NULL;
            }
          }
          else {
            assert(rra->sz_root_ == s);
            rra->sz_root_ = NULL;
          }
          return;
        }
      }
      else {
        /* Sibling is red. */
        if (x->sz_parent_->sz_left_ == x) {
          ref_range_sz_rotate_left(rra, x->sz_parent_);
          sib = x->sz_parent_->sz_right_;
        }
        else {
          ref_range_sz_rotate_right(rra, x->sz_parent_);
          sib = x->sz_parent_->sz_left_;
        }

        /* New sibling must be black; we thus revert to the case above, and induce
         * a deficiency in the sibling by making it red, thus balancing the siblings
         * and moving the deficiency to the parent */
        assert(!sib->sz_is_red_);
        sib->sz_is_red_ = 1;
        x = ref_range_sz_fixup(rra, sib);  /* deal with consecutive reds */

        if (!x || x->sz_is_red_) {
          if (x && x->sz_is_red_) {
            /* Make parent black, thus completing cycle */
            x->sz_is_red_ = 0;
          }
          if (s->sz_parent_) {
            if (s->sz_parent_->sz_left_ == s) {
              s->sz_parent_->sz_left_ = NULL;
            }
            else if (s->sz_parent_->sz_right_ == s) {
              s->sz_parent_->sz_right_ = NULL;
            }
          }
          else {
            assert(rra->sz_root_ == s);
            rra->sz_root_ = NULL;
          }
          return ;
        }
      }
    }
  }
}

static void ref_range_sz_process_insert(struct ref_range_allocator *rra, struct ref_range *x) {
  for (;;) {
    /* The idea is we start off by adding the new sym 'x' somewhere into
     * the tree as a child-less leaf node that is red (thus the number of
     * black nodes remains consistent but there may be consecutive reds).
     * We then take the red 'x' and either remove its "redness" or move its 
     * redness up the tree, and setting x to be the parent to repeat that
     * process until we're done.
     * This implies that x is always red. */
    assert(x->sz_is_red_);

    struct ref_range *parent = x->sz_parent_;
    if (!parent) {
      /* No parent, therefore x is root; make it black and we're done. */
      assert(x == rra->sz_root_);
      x->sz_is_red_ = 0;
      return;
    }
    if ((parent->sz_left_ && parent->sz_left_->sz_is_red_) && (parent->sz_right_ && parent->sz_right_->sz_is_red_)) {
      /*
       *     P         (P)
       *    / \   =>   / \
       *  (L) (R)     L   R
       * Children of parent (X and its sibling) are red, flip colors to parent.
       */

      /* Parent cannot be red as the sibling of X is also red, which means the
       * tree as we had it prior to insert had to consecutive reds */
      assert(!parent->sz_is_red_);
      parent->sz_is_red_ = 1;
      parent->sz_left_->sz_is_red_ = 0;
      parent->sz_right_->sz_is_red_ = 0;
      x = parent;
    }
    else if (parent->sz_is_red_) {
      /*    (P)        (P)
       *    /      or    \
       *  (X)            (X)
       * What we do depends on what is *above* the red (P).
       * Note that, because the root may never be red, P being red implies
       * it must have a parent. */
      assert(parent->sz_parent_);
      assert(!parent->sz_parent_->sz_is_red_); /* cannot have pre-existing consecutive reds */
      struct ref_range *pp = parent->sz_parent_;
      if (parent == pp->sz_left_) {
        /*        PP           PP
         *       /            /
         *     (P)      or  (P)
         *     /              \
         *   (X)              (X)
         * We want to detect the second and rotate it to the first case */
        if (x == parent->sz_right_) {
          /* Converting:
           *     PP           PP
           *    /            /
           *  (P)    to    (X)
           *    \          /
           *    (X)      (P)
           * note that this swaps X and P around, so we change X to P thus
           * giving it the same 'level' as the original location of X.
           */
          x = parent; /* X is now P */
          parent = ref_range_sz_rotate_left(rra, parent);
        }
        /* Now convert:
         *        PP             P
         *       /             /  \
         *     (P)      to   (X)  (PP)
         *     /
         *   (X)
         * Thus implicitly moving x one level up.
         */
        ref_range_sz_rotate_right(rra, pp);
      }
      else /* (parent == pp->right_) */ {
        /* This is the mirror version of the p == pp->left_ case (see above for ASCII art) */
        if (x == parent->sz_left_) {
          x = parent;
          parent = ref_range_sz_rotate_right(rra, parent);
        }
        ref_range_sz_rotate_left(rra, pp);
      }
    }
    else {
      /* No fixups needed, exit */
      return;
    }

  }
}

static int ref_range_sz_cmp(struct ref_range_allocator *rra, const struct ref_range *left, const struct ref_range *right) {
  uintptr_t left_range = (rra->pos_seq_ == left) ? left->at_ : (left->at_ - left->pos_prev_->at_);
  uintptr_t right_range = (rra->pos_seq_ == right) ? right->at_ : (right->at_ - right->pos_prev_->at_);

  if (left_range < right_range) return -1;
  else if (left_range > right_range) return 1;
  else /* (left_range == right_range) */ {
    /* Sizes are equal, compare the end of the range, this ensures lower ranges go earlier. */
    if (left->at_ < right->at_) return -1;
    else if (left->at_ > right->at_) return 1;
    else /* left->at_ == right->at_, and the ranges are the same, therefore: left == right */ {
      return 0;
    }
  }
}

static void ref_range_sz_insert(struct ref_range_allocator *rra, struct ref_range *rr) {
  struct ref_range *s;
  s = rra->sz_root_;
  struct ref_range *next = NULL, *prev = NULL;
  if (!s) {
    rra->sz_root_ = rr;
    rr->sz_is_red_ = 0;
    rra->sz_seq_ = rr;
    rr->sz_next_ = rr->sz_prev_ = rr;
    rr->sz_left_ = rr->sz_right_ = rr->sz_parent_ = NULL;
    return;
  }
  for (;;) {
    int cmp = ref_range_sz_cmp(rra, rr, s);
    if (cmp < 0) {
      next = s;
      if (s->sz_left_) {
        s = s->sz_left_;
      }
      else {
        if (!prev) {
          /* Inserting at head */
          prev = rra->sz_seq_->sz_prev_;
          rra->sz_seq_ = rr; /* new head */
        }

        s->sz_left_ = rr;
        rr->sz_parent_ = s;
        rr->sz_is_red_ = 1;
        rr->sz_next_ = next;
        rr->sz_prev_ = prev;
        rr->sz_next_->sz_prev_ = rr->sz_prev_->sz_next_ = rr;
        rr->sz_left_ = rr->sz_right_ = NULL;
        ref_range_sz_process_insert(rra, rr);
        return;
      }
    }
    else /* (cmp >= 0) */ {
      prev = s;
      if (s->sz_right_) {
        s = s->sz_right_;
      }
      else {
        if (!next) {
          /* Inserting at tail */
          next = rra->sz_seq_;
        }

        s->sz_right_ = rr;
        rr->sz_parent_ = s;
        rr->sz_is_red_ = 1;
        rr->sz_next_ = next;
        rr->sz_prev_ = prev;
        rr->sz_next_->sz_prev_ = rr->sz_prev_->sz_next_ = rr;
        rr->sz_left_ = rr->sz_right_ = NULL;
        ref_range_sz_process_insert(rra, rr);
        return;
      }
    }
  }
}

static struct ref_range *ref_range_sz_find_best_fit(struct ref_range_allocator *rra, uintptr_t min_size_needed) {
  struct ref_range *s;
  struct ref_range *best_fit = NULL;
  s = rra->sz_root_;
  while (s) {
    uintptr_t s_range = (s == rra->pos_seq_) ? s->at_ : (s->pos_prev_->at_ - s->at_);
    if (s_range < min_size_needed) {
      s = s->sz_right_;
    }
    else {
      best_fit = s;
      s = s->sz_left_;
    }
  }
  return best_fit;
}


static struct ref_range *ref_range_pos_rotate_left(struct ref_range_allocator *rra, struct ref_range *A) {
  /*
   *   A            B
   *  / \    ==>   / \
   * 0   B        A   2
   *    / \      / \
   *   1   2    0   1
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct ref_range *B = A->pos_right_;
  A->pos_right_ = B->pos_left_;
  B->pos_left_ = A;

  struct ref_range *A_parent = A->pos_parent_;
  A->pos_parent_ = B;
  if (A->pos_right_) A->pos_right_->pos_parent_ = A;
  B->pos_parent_ = A_parent;
  if (A_parent) {
    if (A_parent->pos_left_ == A) A_parent->pos_left_ = B;
    else /* (A_parent->pos_right_ == A) */ A_parent->pos_right_ = B;
  }
  else {
    /* changing roots */
    rra->pos_root_ = B;
  }

  int root_color = A->pos_is_red_;
  A->pos_is_red_ = B->pos_is_red_;
  B->pos_is_red_ = root_color;

  return B;
}

static struct ref_range *ref_range_pos_rotate_right(struct ref_range_allocator *rra, struct ref_range *B) {
  /*
   *     B          A
   *    / \   =>   / \
   *   A   2      0   B
   *  / \            / \
   * 0   1          1   2
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct ref_range *A = B->pos_left_;
  B->pos_left_ = A->pos_right_;
  A->pos_right_ = B;

  struct ref_range *B_parent = B->pos_parent_;
  B->pos_parent_ = A;
  if (B->pos_left_) B->pos_left_->pos_parent_ = B;
  A->pos_parent_ = B_parent;
  if (B_parent) {
    if (B_parent->pos_left_ == B) B_parent->pos_left_ = A;
    else /* (B_parent->pos_right_ == B) */ B_parent->pos_right_ = A;
  }
  else {
    /* changing roots */
    rra->pos_root_ = A;
  }

  int root_color = B->pos_is_red_;
  B->pos_is_red_ = A->pos_is_red_;
  A->pos_is_red_ = root_color;

  return A;
}

static struct ref_range *ref_range_pos_fixup(struct ref_range_allocator *rra, struct ref_range *z) {
  struct ref_range *x;
  if (!(z->pos_left_ && z->pos_left_->pos_is_red_) && !(z->pos_right_ && z->pos_right_->pos_is_red_)) {
    /* nothing to see here, move along */
    return z->pos_parent_;
  }

  if (z->pos_parent_->pos_left_ == z) {
    if (z->pos_right_ && z->pos_right_->pos_is_red_) {
      z = ref_range_pos_rotate_left(rra, z);
    }
    x = ref_range_pos_rotate_right(rra, z->pos_parent_);
  }
  else /* (z->pos_parent->pos_right == z) */ {
    if (z->pos_left_ && z->pos_left_->pos_is_red_) {
      z = ref_range_pos_rotate_right(rra, z);
    }
    x = ref_range_pos_rotate_left(rra, z->pos_parent_);
  }
  /* Red children both turn black, deficiency resolved, return NULL. */
  x->pos_left_->pos_is_red_ = x->pos_right_->pos_is_red_ = 0;
  return NULL;
}

static void ref_range_pos_remove(struct ref_range_allocator *rra, struct ref_range *s) {
  /* Remove from chain first (easy bit) */
  if (s->pos_next_ != s) {
    s->pos_next_->pos_prev_ = s->pos_prev_;
    s->pos_prev_->pos_next_ = s->pos_next_;
    if (rra->pos_seq_ == s) rra->pos_seq_ = s->pos_next_;
    s->pos_next_ = s->pos_prev_ = NULL;
  }
  else {
    assert(s == rra->pos_seq_);
    s->pos_next_ = s->pos_prev_ = NULL;
    rra->pos_seq_ = NULL;
  }

  if (s->pos_right_) {
    /* Substitute with minimum node in right tree, then delete the minimum node. */
    struct ref_range *rmin = s->pos_right_;
    while (rmin->pos_left_) rmin = rmin->pos_left_;

    /* Swap out s with rmin */
    struct ref_range *parent, *left, *right;
    int is_red;
    parent = rmin->pos_parent_;
    left = rmin->pos_left_;
    right = rmin->pos_right_;
    is_red = rmin->pos_is_red_;
    rmin->pos_parent_ = s->pos_parent_;
    rmin->pos_left_ = s->pos_left_;
    rmin->pos_right_ = s->pos_right_;
    rmin->pos_is_red_ = s->pos_is_red_;
    s->pos_parent_ = (parent == s) ? rmin : parent;
    s->pos_left_ = left;
    s->pos_right_ = right;
    s->pos_is_red_ = is_red;

    /* Fix up parents */
    if (s->pos_parent_->pos_right_ == rmin) {
      s->pos_parent_->pos_right_ = s;
    }
    else if (s->pos_parent_->pos_left_ == rmin) {
      s->pos_parent_->pos_left_ = s;
    }
    if (rmin->pos_parent_) {
      if (rmin->pos_parent_->pos_right_ == s) {
        rmin->pos_parent_->pos_right_ = rmin;
      }
      else if (rmin->pos_parent_->pos_left_ == s) {
        rmin->pos_parent_->pos_left_ = rmin;
      }
    }
    else {
      assert(rra->pos_root_ == s);
      rra->pos_root_ = rmin;
    }

    /* Fix up children */
    if (rmin->pos_left_) rmin->pos_left_->pos_parent_ = rmin;
    if (rmin->pos_right_) rmin->pos_right_->pos_parent_ = rmin;
    /* s->left_ should be NULL as the leftmost child of the right branch */
    assert(!s->pos_left_);
    if (s->pos_right_) s->pos_right_->pos_parent_ = s;

    /* s is now in a new location, guaranteed to be a leaf or degree 1 node */
  }


  if (s->pos_left_) {
    /* left must be a red node as right must be NULL at this point (otherwise
     * we would have an inconsistent number of black nodes from left to right) */
    assert(s->pos_left_->pos_is_red_);
    assert(!s->pos_is_red_);
    assert(!s->pos_right_);
    /* Swap positions for s->pos_left and s, then s has been removed. */
    if (s->pos_parent_) {
      if (s->pos_parent_->pos_left_ == s) {
        s->pos_parent_->pos_left_ = s->pos_left_;
      }
      else if (s->pos_parent_->pos_right_ == s) {
        s->pos_parent_->pos_right_ = s->pos_left_;
      }
    }
    else {
      assert(rra->pos_root_ == s);
      rra->pos_root_ = s->pos_left_;
    }
    s->pos_left_->pos_parent_ = s->pos_parent_;
    s->pos_left_->pos_is_red_ = 0;
    /* Done, s has been evicted from the tree. */
    return;
  }
  else if (s->pos_right_) {
    /* Same logic as above but now for right, the left branch must be NULL, and
     * the right branch must be red, meaning the s node must be black. */
    assert(s->pos_right_->pos_is_red_);
    assert(!s->pos_is_red_);
    assert(!s->pos_left_);
    /* Swap positions for s->right_ and s */
    if (s->pos_parent_) {
      if (s->pos_parent_->pos_left_ == s) {
        s->pos_parent_->pos_left_ = s->pos_right_;
      }
      else if (s->pos_parent_->pos_right_ == s) {
        s->pos_parent_->pos_right_ = s->pos_right_;
      }
    }
    else {
      assert(rra->pos_root_ == s);
      rra->pos_root_ = s->pos_right_;
    }
    s->pos_right_->pos_parent_ = s->pos_parent_;
    s->pos_right_->pos_is_red_ = 0;
    /* Done, s has been evicted */
    return;
  }
  else if (s->pos_is_red_) {
    /* !s->pos_left_ && !s->pos_right_ */
    /* No branches (s is a true leaf node), and the color is red, easy removal,
     * patching up parent. */
    if (s->pos_parent_) {
      if (s->pos_parent_->pos_left_ == s) s->pos_parent_->pos_left_ = NULL;
      else if (s->pos_parent_->pos_right_ == s) s->pos_parent_->pos_right_ = NULL;
    }
    else {
      /* No parent, this must be the last node */
      assert(s == rra->pos_root_);
      rra->pos_root_ = NULL;
    }
    /* Done */
    return;
  }
  else /* (!s->pos_left_ && !s->pos_right_ && !s->pos_is_red_) */ {
    /* s is a true leaf node, and black...
     * when we remove s from the tree, the tree will be imbalanced (for its
     * sibling will have one more black.)
     * one way to view this is to think of s as being "short one black" - however,
     * we cannot change s itself to red, because we're about to remove s. We can
     * make its sibling also "short one black", thereby propagating the deficiency
     * upwards (the siblings will be balanced but now the parent as a whole is
     * "short one black" compared to *its* siblings.) */
    struct ref_range *x = s;
    for (;;) {
      struct ref_range *sib = NULL;

      if (!x->pos_parent_) {
        /* Reached the root; done. */
        if (s->pos_parent_) {
          if (s->pos_parent_->pos_left_ == s) {
            s->pos_parent_->pos_left_ = NULL;
          }
          else if (s->pos_parent_->pos_right_ == s) {
            s->pos_parent_->pos_right_ = NULL;
          }
        }
        else {
          assert(rra->pos_root_ == s);
          rra->pos_root_ = NULL;
        }
        return;
      }
      sib = (struct ref_range *)(((uintptr_t)x->pos_parent_->pos_left_) ^ ((uintptr_t)x->pos_parent_->pos_right_) ^ ((uintptr_t)x));
      if (!sib->pos_is_red_) {
        /* Sibling is black, we can induce a deficiency by making it red */
        sib->pos_is_red_ = 1;
        x = ref_range_pos_fixup(rra, sib); /* deal with consecutive reds */

        if (!x || x->pos_is_red_) {
          if (x && x->pos_is_red_) {
            /* We can make it black, all is balanced again */
            x->pos_is_red_ = 0;
          }
          /* Deficiency resolved in fixup, all is balanced. */
          if (s->pos_parent_) {
            if (s->pos_parent_->pos_left_ == s) {
              s->pos_parent_->pos_left_ = NULL;
            }
            else if (s->pos_parent_->pos_right_ == s) {
              s->pos_parent_->pos_right_ = NULL;
            }
          }
          else {
            assert(rra->pos_root_ == s);
            rra->pos_root_ = NULL;
          }
          return;
        }
      }
      else {
        /* Sibling is red. */
        if (x->pos_parent_->pos_left_ == x) {
          ref_range_pos_rotate_left(rra, x->pos_parent_);
          sib = x->pos_parent_->pos_right_;
        }
        else {
          ref_range_pos_rotate_right(rra, x->pos_parent_);
          sib = x->pos_parent_->pos_left_;
        }

        /* New sibling must be black; we thus revert to the case above, and induce
         * a deficiency in the sibling by making it red, thus balancing the siblings
         * and moving the deficiency to the parent */
        assert(!sib->pos_is_red_);
        sib->pos_is_red_ = 1;
        x = ref_range_pos_fixup(rra, sib);  /* deal with consecutive reds */

        if (!x || x->pos_is_red_) {
          if (x && x->pos_is_red_) {
            /* Make parent black, thus completing cycle */
            x->pos_is_red_ = 0;
          }
          if (s->pos_parent_) {
            if (s->pos_parent_->pos_left_ == s) {
              s->pos_parent_->pos_left_ = NULL;
            }
            else if (s->pos_parent_->pos_right_ == s) {
              s->pos_parent_->pos_right_ = NULL;
            }
          }
          else {
            assert(rra->pos_root_ == s);
            rra->pos_root_ = NULL;
          }
          return;
        }
      }
    }
  }
}

static void ref_range_pos_process_insert(struct ref_range_allocator *rra, struct ref_range *x) {
  for (;;) {
    /* The idea is we start off by adding the new sym 'x' somewhere into
     * the tree as a child-less leaf node that is red (thus the number of
     * black nodes remains consistent but there may be consecutive reds).
     * We then take the red 'x' and either remove its "redness" or move its
     * redness up the tree, and setting x to be the parent to repeat that
     * process until we're done.
     * This implies that x is always red. */
    assert(x->pos_is_red_);

    struct ref_range *parent = x->pos_parent_;
    if (!parent) {
      /* No parent, therefore x is root; make it black and we're done. */
      assert(x == rra->pos_root_);
      x->pos_is_red_ = 0;
      return;
    }
    if ((parent->pos_left_ && parent->pos_left_->pos_is_red_) && (parent->pos_right_ && parent->pos_right_->pos_is_red_)) {
      /*
       *     P         (P)
       *    / \   =>   / \
       *  (L) (R)     L   R
       * Children of parent (X and its sibling) are red, flip colors to parent.
       */

      /* Parent cannot be red as the sibling of X is also red, which means the
       * tree as we had it prior to insert had to consecutive reds */
      assert(!parent->pos_is_red_);
      parent->pos_is_red_ = 1;
      parent->pos_left_->pos_is_red_ = 0;
      parent->pos_right_->pos_is_red_ = 0;
      x = parent;
    }
    else if (parent->pos_is_red_) {
      /*    (P)        (P)
       *    /      or    \
       *  (X)            (X)
       * What we do depends on what is *above* the red (P).
       * Note that, because the root may never be red, P being red implies
       * it must have a parent. */
      assert(parent->pos_parent_);
      assert(!parent->pos_parent_->pos_is_red_); /* cannot have pre-existing consecutive reds */
      struct ref_range *pp = parent->pos_parent_;
      if (parent == pp->pos_left_) {
        /*        PP           PP
         *       /            /
         *     (P)      or  (P)
         *     /              \
         *   (X)              (X)
         * We want to detect the second and rotate it to the first case */
        if (x == parent->pos_right_) {
          /* Converting:
           *     PP           PP
           *    /            /
           *  (P)    to    (X)
           *    \          /
           *    (X)      (P)
           * note that this swaps X and P around, so we change X to P thus
           * giving it the same 'level' as the original location of X.
           */
          x = parent; /* X is now P */
          parent = ref_range_pos_rotate_left(rra, parent);
        }
        /* Now convert:
         *        PP             P
         *       /             /  \
         *     (P)      to   (X)  (PP)
         *     /
         *   (X)
         * Thus implicitly moving x one level up.
         */
        ref_range_pos_rotate_right(rra, pp);
      }
      else /* (parent == pp->right_) */ {
        /* This is the mirror version of the p == pp->left_ case (see above for ASCII art) */
        if (x == parent->pos_left_) {
          x = parent;
          parent = ref_range_pos_rotate_right(rra, parent);
        }
        ref_range_pos_rotate_left(rra, pp);
      }
    }
    else {
      /* No fixups needed, exit */
      return;
    }

  }
}

static int ref_range_pos_cmp(const struct ref_range *left, const struct ref_range *right) {
  if (left->at_ < right->at_) return -1;
  else if (left->at_ > right->at_) return 1;
  else return 0;
}

static struct ref_range *ref_range_pos_find_at(struct ref_range_allocator *rra, uintptr_t pos) {
  struct ref_range *s;
  struct ref_range *best_fit = NULL;
  s = rra->pos_root_;
  while (s) {
    if (pos < s->at_) {
      best_fit = s;
      s = s->pos_left_;
    }
    else /* (pos >= s->at_) */ {
      s = s->pos_right_;
    }
  }
  return best_fit;
}

static struct ref_range *ref_range_pos_find_start(struct ref_range_allocator *rra, uintptr_t pos) {
  struct ref_range *s;
  struct ref_range *best_fit = NULL;
  s = rra->pos_root_;
  while (s) {
    uintptr_t range_end = s->at_;
    if (pos <= s->at_) {
      /* pos is either before, or adjacent to (pos == s->at_) the range, match it */
      best_fit = s;
      s = s->pos_left_;
    }
    else /* (pos > s->at_) */ {
      s = s->pos_right_;
    }
  }
  return best_fit;
}

static struct ref_range *ref_range_pos_find_end(struct ref_range_allocator *rra, uintptr_t pos) {
  struct ref_range *s;
  struct ref_range *best_fit = NULL;
  s = rra->pos_root_;
  while (s) {
    uintptr_t range_start = (s != rra->pos_seq_) ? s->pos_prev_->at_ : 0;
    if (pos < range_start) {
      /* pos is before the range start, not a match */
      s = s->pos_left_;
    }
    else /* (pos >= range_start ) */ {
      /* pos is on or after the range start, match */
      best_fit = s;
      s = s->pos_right_;
    }
  }
  return best_fit;
}


static int ref_range_apply_ref(struct ref_range_allocator *rra, uintptr_t from, uintptr_t to, int delta) {
  uintptr_t edges[] ={
    from,
    to
  };
  int deltas[] ={
    delta,
    -delta
  };
  int idx = 0;
  int current_delta = 0;

  /* rr: the current "write" position.
   *     if rr is not equal to rr_next, then it can safely be overwritten. */
  struct ref_range *rr;
  rr = ref_range_pos_find_start(rra, from);

  /* rr_next: the range after next_range_edge */
  struct ref_range *rr_next = NULL;
  if (rr && (rr->pos_next_ != rra->pos_seq_)) {
    rr_next = rr->pos_next_;
  }
  if (rr && !rr->refcount_) ref_range_sz_remove(rra, rr);

  /* this_range_refcount: the refcount of the current range */
  int this_range_refcount = rr ? rr->refcount_ : 0;

  /* the edge that marks the end of the current, pre-existing, range */
  uintptr_t next_range_edge = rr ? rr->at_ : 0;

  /* Check if the leading edge (from) is already behind us, if so, make sure its
   * refcount is already accurately processed */
  uintptr_t rr_start = (rr && (rr != rra->pos_seq_)) ? rr->pos_prev_->at_ : 0;
  while ((idx < 2) && (rr_start >= edges[idx])) {
    current_delta += deltas[idx];
    this_range_refcount += deltas[idx];
    idx++;
  }

  for (;;) {
    if (idx == (sizeof(edges)/sizeof(*edges))) {
      /* No more new edges; emit the edge we have stored, if any, and be done */
      if (next_range_edge) {
        /* We still have an original edge latched, emit it */
        if (rr != rr_next) {
          rr->at_ = next_range_edge;
          rr->refcount_ = this_range_refcount;
          if (!rr->refcount_) ref_range_sz_insert(rra, rr);
          rr = (rr->pos_next_ != rra->pos_seq_) ? rr->pos_next_ : NULL;
        }
        else {
          /* Build new ref_range */
          struct ref_range *new_rr = ref_range_alloc_range(rra);
          if (!new_rr) return -1;
          new_rr->at_ = next_range_edge;
          new_rr->refcount_ = this_range_refcount;

          struct ref_range *nrp = rr_next;
          if (nrp) {
            if (nrp->pos_left_) {
              nrp = nrp->pos_left_;
              while (nrp->pos_right_) {
                nrp = nrp->pos_right_;
              }
              nrp->pos_right_ = new_rr;
            }
            else {
              nrp->pos_left_ = new_rr;
            }
            new_rr->pos_parent_ = nrp;
            new_rr->pos_is_red_ = 1;
            new_rr->pos_next_ = rr_next;
            new_rr->pos_prev_ = rr_next->pos_prev_;
            new_rr->pos_prev_->pos_next_ = new_rr->pos_next_->pos_prev_ = new_rr;
            ref_range_pos_process_insert(rra, new_rr);
          }
          else {
            /* No further existing edges after next_range_edge; append new ref_range to the end */
            if (rra->pos_seq_) {
              new_rr->pos_parent_ = rra->pos_seq_->pos_prev_;
              rra->pos_seq_->pos_prev_->pos_right_ = new_rr;
              new_rr->pos_is_red_ = 1;
              new_rr->pos_next_ = rra->pos_seq_;
              new_rr->pos_prev_ = rra->pos_seq_->pos_prev_;
              new_rr->pos_prev_->pos_next_ = new_rr->pos_next_->pos_prev_ = new_rr;
              ref_range_pos_process_insert(rra, new_rr);
            }
            else {
              rra->pos_root_ = rra->pos_seq_ = new_rr;
              new_rr->pos_next_ = new_rr->pos_prev_ = new_rr;
              new_rr->pos_is_red_ = 0;
            }
          }
          if (!new_rr->refcount_) ref_range_sz_insert(rra, new_rr);
          rr = (new_rr->pos_next_ != rra->pos_seq_) ? new_rr->pos_next_ : NULL;
        }
      }

      /* Clear out any remaining interim edges */
      while (rr != rr_next) {
        struct ref_range *next = (rr->pos_next_ != rra->pos_seq_) ? rr->pos_next_ : NULL;
        ref_range_pos_remove(rra, rr);
        ref_range_free(rra, rr);
        rr = next;
      }

      return 0;
    }
    else if (!next_range_edge || 
             (edges[idx] < next_range_edge)) {
      /* Emit new edge for edges[idx] */
      current_delta += deltas[idx];
      int next_range_refcount = this_range_refcount + deltas[idx];
      if (this_range_refcount != next_range_refcount) {
        /* There is an edge .. */
        if (rr != rr_next) {
          /* Re-use existing ref_range */
          rr->at_ = edges[idx];
          rr->refcount_ = this_range_refcount;
          if (!rr->refcount_) ref_range_sz_insert(rra, rr);
          rr = (rr->pos_next_ != rra->pos_seq_) ? rr->pos_next_ : NULL;
        }
        else {
          /* Build new ref_range */
          struct ref_range *new_rr = ref_range_alloc_range(rra);
          if (!new_rr) return -1;
          new_rr->at_ = edges[idx];
          new_rr->refcount_ = this_range_refcount;
          struct ref_range *nrp = rr_next;
          if (nrp) {
            if (nrp->pos_left_) {
              nrp = nrp->pos_left_;
              while (nrp->pos_right_) {
                nrp = nrp->pos_right_;
              }
              nrp->pos_right_ = new_rr;
            }
            else {
              nrp->pos_left_ = new_rr;
            }
            new_rr->pos_parent_ = nrp;
            new_rr->pos_is_red_ = 1;
            new_rr->pos_next_ = rr_next;
            new_rr->pos_prev_ = rr_next->pos_prev_;
            new_rr->pos_prev_->pos_next_ = new_rr->pos_next_->pos_prev_ = new_rr;
            ref_range_pos_process_insert(rra, new_rr);
          }
          else {
            /* No further existing edges after next_range_edge; append new ref_range to the end */
            if (rra->pos_seq_) {
              new_rr->pos_parent_ = rra->pos_seq_->pos_prev_;
              rra->pos_seq_->pos_prev_->pos_right_ = new_rr;
              new_rr->pos_is_red_ = 1;
              new_rr->pos_next_ = rra->pos_seq_;
              new_rr->pos_prev_ = rra->pos_seq_->pos_prev_;
              new_rr->pos_prev_->pos_next_ = new_rr->pos_next_->pos_prev_ = new_rr;
              ref_range_pos_process_insert(rra, new_rr);
            }
            else {
              rra->pos_root_ = rra->pos_seq_ = new_rr;
              new_rr->pos_next_ = new_rr->pos_prev_ = new_rr;
              new_rr->pos_is_red_ = 0;
            }
          }
          if (!new_rr->refcount_) ref_range_sz_insert(rra, new_rr);
          rr = (new_rr->pos_next_ != rra->pos_seq_) ? new_rr->pos_next_ : NULL;
        }
        this_range_refcount = next_range_refcount;
        idx++;
      }
    }
    else if (edges[idx] > next_range_edge) {
      /* Emit existing edge */
      int next_range_refcount = current_delta + (rr_next ? rr_next->refcount_ : 0);
      if (this_range_refcount != next_range_refcount) {
        /* There is an edge */
        if (rr != rr_next) {
          /* Re-use existing ref_range */
          rr->at_ = next_range_edge;
          rr->refcount_ = this_range_refcount;
        }
        else {
          /* Build new ref_range */
          struct ref_range *new_rr = ref_range_alloc_range(rra);
          if (!new_rr) return -1;
          struct ref_range *nrp = rr_next;
          if (nrp) {
            if (nrp->pos_left_) {
              nrp = nrp->pos_left_;
              while (nrp->pos_right_) {
                nrp = nrp->pos_right_;
              }
              nrp->pos_right_ = new_rr;
            }
            else {
              nrp->pos_left_ = new_rr;
            }
            new_rr->pos_parent_ = nrp;
            new_rr->pos_next_ = rr_next;
            new_rr->pos_prev_ = rr_next->pos_prev_;
            new_rr->pos_prev_->pos_next_ = new_rr->pos_next_->pos_prev_ = new_rr;
          }
          else {
            /* No further existing edges after next_range_edge; append new ref_range to the end */
            new_rr->pos_parent_ = rra->pos_seq_->pos_prev_;
            rra->pos_seq_->pos_prev_->pos_right_ = new_rr;
            new_rr->pos_next_ = rra->pos_seq_ ? rra->pos_seq_ : new_rr;
            new_rr->pos_prev_ = new_rr->pos_next_->pos_prev_;
            new_rr->pos_prev_->pos_next_ = new_rr->pos_next_->pos_prev_ = new_rr;
          }
          new_rr->pos_is_red_ = 1;
          new_rr->at_ = next_range_edge;
          new_rr->refcount_ = this_range_refcount;
          ref_range_pos_process_insert(rra, new_rr);
          rr = new_rr;
        }
        if (!rr->refcount_) ref_range_sz_insert(rra, rr);
        rr = (rr->pos_next_ != rra->pos_seq_) ? rr->pos_next_ : NULL;
        this_range_refcount = next_range_refcount;
      }
      next_range_edge = rr_next ? rr_next->at_ : 0;
      if (rr_next && !rr_next->refcount_) ref_range_sz_remove(rra, rr_next);
      rr_next = (rr_next && (rr_next->pos_next_ != rra->pos_seq_)) ? rr_next->pos_next_ : NULL;
    }
    else /* edges[idx] == next_range_edge */ {
      /* Emit one edge representing both new and existing edges */
      current_delta += deltas[idx];
      int next_range_refcount = current_delta + (rr_next ? rr_next->refcount_ : 0);
      if (this_range_refcount != next_range_refcount) {
        if (rr != rr_next) {
          /* Re-use the existing ref_range */
          rr->at_ = next_range_edge;
          rr->refcount_ = this_range_refcount;
        }
        else {
          /* Build new ref_range */
          struct ref_range *new_rr = ref_range_alloc_range(rra);
          if (!new_rr) return -1;
          struct ref_range *nrp = rr_next;
          if (nrp) {
            if (nrp->pos_left_) {
              nrp = nrp->pos_left_;
              while (nrp->pos_right_) {
                nrp = nrp->pos_right_;
              }
              nrp->pos_right_ = new_rr;
            }
            else {
              nrp->pos_left_ = new_rr;
            }
            new_rr->pos_parent_ = nrp;
            new_rr->pos_next_ = rr_next;
            new_rr->pos_prev_ = rr_next->pos_prev_;
            new_rr->pos_prev_->pos_next_ = new_rr->pos_next_->pos_prev_ = new_rr;
          }
          else {
            /* No further existing edges after next_range_edge; append new ref_range to the end */
            new_rr->pos_parent_ = rra->pos_seq_->pos_prev_;
            rra->pos_seq_->pos_prev_->pos_right_ = new_rr;
            new_rr->pos_next_ = rra->pos_seq_ ? rra->pos_seq_ : new_rr;
            new_rr->pos_prev_ = new_rr->pos_next_->pos_prev_;
            new_rr->pos_prev_->pos_next_ = new_rr->pos_next_->pos_prev_ = new_rr;
          }
          new_rr->pos_is_red_ = 1;
          new_rr->at_ = next_range_edge;
          new_rr->refcount_ = this_range_refcount;
          ref_range_pos_process_insert(rra, new_rr);
          rr = new_rr;
        }
        if (!rr->refcount_) ref_range_sz_insert(rra, rr);
        rr = (rr->pos_next_ != rra->pos_seq_) ? rr->pos_next_ : NULL;
        this_range_refcount = next_range_refcount;
      }
      next_range_edge = rr_next ? rr_next->at_ : 0;
      if (rr_next && !rr_next->refcount_) ref_range_sz_remove(rra, rr_next);
      rr_next = (rr_next && (rr_next->pos_next_ != rra->pos_seq_)) ? rr_next->pos_next_ : NULL;
      idx++;
    }
  }
}

int ref_range_alloc(struct ref_range_allocator *rra, uintptr_t size, uintptr_t *result) {
  struct ref_range *rr = ref_range_sz_find_best_fit(rra, size);
  uintptr_t start_of_range;
  if (rr) {
    assert(!rr->refcount_);
    start_of_range = (rra->pos_seq_ == rr) ? 0 : rr->pos_prev_->at_;
  }
  else {
    /* No match, allocate range at the end */
    if (rra->pos_seq_) {
      if (!rra->pos_seq_->pos_prev_->refcount_) {
        /* Last range has 0 refcount, start at that range. */
        if (rra->pos_seq_->pos_prev_ != rra->pos_seq_) {
          start_of_range = rra->pos_seq_->pos_prev_->at_;
        }
        else {
          /* Last range is also the only range and has 0 refcount, so start at ofsset 0 */
          start_of_range = 0;
        }
      }
      else {
        /* Last range has non-zero refcount, start after that range */
        start_of_range = rra->pos_seq_->pos_prev_->at_;
      }
    }
    else {
      /* No ranges yet, so start at offset 0 */
      start_of_range = 0;
    }
  }
  
  int r;
  r = ref_range_apply_ref(rra, start_of_range, start_of_range + size, 1);
  if (r) return r;
  if (rra->watermark_ < (start_of_range + size)) rra->watermark_ = start_of_range + size;
  *result = start_of_range;
  return 0;
}

int ref_range_mark_range_allocated(struct ref_range_allocator *rra, uintptr_t from, uintptr_t to) {
  int r = ref_range_apply_ref(rra, from, to, 1);
  /* Discover last range end; we do this manually to handle the case where a negative refcount
   * (introduced through other means) was incremented to 0. */
  uintptr_t last_range_end = rra->pos_seq_ ? rra->pos_seq_->pos_prev_->at_ : 0;
  if (rra->watermark_ < last_range_end) rra->watermark_ = last_range_end;
  return r;
}

int ref_range_mark_range_free(struct ref_range_allocator *rra, uintptr_t from, uintptr_t to) {
  int r = ref_range_apply_ref(rra, from, to, -1);
  /* Discover last range end; we do this manually to handle the case where a zero refcount
   * was decremented to -1. */
  uintptr_t last_range_end = rra->pos_seq_ ? rra->pos_seq_->pos_prev_->at_ : 0;
  if (rra->watermark_ < last_range_end) rra->watermark_ = last_range_end;
  return r;
}

static int ref_range_sanity_check_pos_parent_pointers(struct ref_range *parent, struct ref_range *child) {
  if (!child) return 1;
  if (child->pos_parent_ != parent) return 0;
  if (!ref_range_sanity_check_pos_parent_pointers(child, child->pos_left_)) return 0;
  if (!ref_range_sanity_check_pos_parent_pointers(child, child->pos_right_)) return 0;
  return 1;
}

static int ref_range_sanity_check_sz_parent_pointers(struct ref_range *parent, struct ref_range *child) {
  if (!child) return 1;

  if (child->sz_parent_ != parent) return 0;
  if (!ref_range_sanity_check_sz_parent_pointers(child, child->sz_left_)) return 0;
  if (!ref_range_sanity_check_sz_parent_pointers(child, child->sz_right_)) return 0;
  return 1;
}

static void ref_range_increment_sz_nodes(struct ref_range *rr) {
  if (!rr) return;
  rr->sanity_ |= 8;
  ref_range_increment_sz_nodes(rr->sz_left_);
  ref_range_increment_sz_nodes(rr->sz_right_);
}

static void ref_range_increment_pos_nodes(struct ref_range *rr) {
  if (!rr) return;
  rr->sanity_ |= 4;
  ref_range_increment_pos_nodes(rr->pos_left_);
  ref_range_increment_pos_nodes(rr->pos_right_);
}

static int ref_range_check_sz_nodes(struct ref_range *rr) {
  if (!rr) return 1;
  if (!rr->refcount_ && (rr->sanity_ != 15)) {
    fprintf(stderr, "ref_range_sanity_check_both_trees() failed\n");
    return 0;
  }
  if (rr->refcount_ && (rr->sanity_ != 5)) {
    fprintf(stderr, "ref_range_sanity_check_both_trees() failed\n");
    return 0;
  }
  if (!ref_range_check_sz_nodes(rr->sz_left_)) return 0;
  if (!ref_range_check_sz_nodes(rr->sz_right_)) return 0;
  return 1;
}

static int ref_range_check_pos_nodes(struct ref_range *rr) {
  if (!rr) return 1;
  if (!rr->refcount_ && (rr->sanity_ != 15)) {
    fprintf(stderr, "ref_range_sanity_check_both_trees() failed\n");
    return 0;
  }
  if (rr->refcount_ && (rr->sanity_ != 5)) {
    fprintf(stderr, "ref_range_sanity_check_both_trees() failed\n");
    return 0;
  }
  if (!ref_range_check_pos_nodes(rr->pos_left_)) return 0;
  if (!ref_range_check_pos_nodes(rr->pos_right_)) return 0;
  return 1;
}

static int ref_range_check_both_trees(struct ref_range_allocator *rra) {
  struct ref_range *rr = rra->pos_seq_;
  if (rr) {
    do {
      rr->sanity_ = 0;

      rr = rr->pos_next_;
    } while (rr != rra->pos_seq_);
  }

  rr = rra->sz_seq_;
  if (rr) {
    do {
      rr->sanity_ = 0;

      rr = rr->sz_next_;
    } while (rr != rra->sz_seq_);
  }

  rr = rra->pos_seq_;
  if (rr) {
    do {
      rr->sanity_ |= 1;

      rr = rr->pos_next_;
    } while (rr != rra->pos_seq_);
  }

  rr = rra->sz_seq_;
  if (rr) {
    do {
      rr->sanity_ |= 2;

      rr = rr->sz_next_;
    } while (rr != rra->sz_seq_);
  }

  ref_range_increment_pos_nodes(rra->pos_root_); /* marks sanity_ as 4 */
  ref_range_increment_sz_nodes(rra->sz_root_);   /* marks sanity_ as 8 */

  rr = rra->pos_seq_;
  if (rr) {
    do {
      if (!rr->refcount_ && (rr->sanity_ != 15)) {
        fprintf(stderr, "ref_range_sanity_check_both_trees() failed\n");
        return 0;
      }
      if (rr->refcount_ && (rr->sanity_ != 5)) {
        fprintf(stderr, "ref_range_sanity_check_both_trees() failed\n");
        return 0;
      }

      rr = rr->pos_next_;
    } while (rr != rra->pos_seq_);
  }

  rr = rra->pos_seq_;
  if (rr) {
    do {
      if (!rr->refcount_ && (rr->sanity_ != 15)) {
        fprintf(stderr, "ref_range_sanity_check_both_trees() failed\n");
        return 0;
      }
      if (rr->refcount_ && (rr->sanity_ != 5)) {
        fprintf(stderr, "ref_range_sanity_check_both_trees() failed\n");
        return 0;
      }

      rr = rr->pos_next_;
    } while (rr != rra->pos_seq_);
  }

  return ref_range_check_pos_nodes(rra->pos_root_) &&
         ref_range_check_sz_nodes(rra->sz_root_);
}

static int ref_range_sanity_check_pos_ascension(struct ref_range_allocator *rra) {
  struct ref_range *rr = rra->pos_seq_;
  if (rr) {
    do {
      if (rr->pos_next_ != rra->pos_seq_) {
        struct ref_range *rnext = rr->pos_next_;
        if (rr->at_ >= rnext->at_) {
          fprintf(stderr, "ref_range_sanity_check_pos_ascension() failed\n");
          return 0;
        }
      }

      rr = rr->pos_next_;
    } while (rr != rra->pos_seq_);
  }

  return 1;
}

static int ref_range_sanity_check_sz_ascension(struct ref_range_allocator *rra) {
  struct ref_range *rr = rra->sz_seq_;
  if (rr) {
    do {
      if (rr->sz_next_ != rra->sz_seq_) {
        struct ref_range *rnext = rr->sz_next_;
        uintptr_t rr_from = (rr == rra->pos_seq_) ? 0 : rr->pos_prev_->at_;
        uintptr_t rnext_from = (rnext == rra->pos_seq_) ? 0 : rnext->pos_prev_->at_;

        if ((rr->at_ - rr_from) > (rnext->at_ - rnext_from)) {
          fprintf(stderr, "ref_range_sanity_check_sz_ascension() failed\n");
          return 0;
        }
      }

      rr = rr->sz_next_;
    } while (rr != rra->sz_seq_);
  }

  return 1;
}

static int ref_range_check_balance_pos_black_nodes(struct ref_range *rr) {
  int left_count;
  int right_count;
  if (!rr) return 0;
  left_count = ref_range_check_balance_pos_black_nodes(rr->pos_left_);
  right_count = ref_range_check_balance_pos_black_nodes(rr->pos_right_);
  if (left_count == -1) {
    return -1;
  }
  if (left_count != right_count) {
    return -1;
  }
  if (rr->pos_is_red_) return left_count;
  return left_count + 1;
}

static int ref_range_sanity_check(struct ref_range_allocator *rra) {
  int r = ref_range_sanity_check_pos_parent_pointers(NULL, rra->pos_root_)
       && ref_range_sanity_check_sz_parent_pointers(NULL, rra->sz_root_)
       && ref_range_check_both_trees(rra)
       && ref_range_sanity_check_pos_ascension(rra)
       && ref_range_sanity_check_sz_ascension(rra);
  if (ref_range_check_balance_pos_black_nodes(rra->pos_root_) == -1) {
    r = 0;
  }

  if (!r) {
    fprintf(stderr, "ref_range_sanity_check() failed\n");
  }
  return r;
}

int ref_range_test(void) {
  int r;
  srand(123);

  struct ref_range_allocator rra;
  ref_range_allocator_init(&rra);

  r = ref_range_apply_ref(&rra, 0, 2, 1);
  if (r) {
    fprintf(stderr, "ref_range_test(): failed ref_range_apply_ref()\n");
    return r;
  }

  r = ref_range_apply_ref(&rra, 1, 2, 1);
  if (r) {
    fprintf(stderr, "ref_range_test(): failed ref_range_apply_ref()\n");
    return r;
  }

  ref_range_allocator_cleanup(&rra);

  ref_range_allocator_init(&rra);

  uintptr_t result1 = UINTPTR_MAX;
  r = ref_range_alloc(&rra, 1000, &result1);
  if (r) {
    fprintf(stderr, "ref_range_test(): failed ref_range_alloc()\n");
    return r;
  }
  uintptr_t result2 = UINTPTR_MAX;
  r = ref_range_alloc(&rra, 1000, &result2);
  if (r) {
    fprintf(stderr, "ref_range_test(): failed ref_range_alloc()\n");
    return r;
  }
  struct ref_range *rr = rra.pos_seq_;
  fprintf(stderr, "Post-alloc #1 & #2 (%d, %d):\n", (int)result1, (int)result2);
  if (rr) {
    uintptr_t last = 0;
    do {
      fprintf(stderr, "from: %d to: %3d refcount: %d\n", (int)last, (int)rr->at_, rr->refcount_);
      last = rr->at_;

      rr = rr->pos_next_;
    } while (rr != rra.pos_seq_);
  }
  r = ref_range_mark_range_free(&rra, result1, result1 + 1000);
  if (r) {
    fprintf(stderr, "ref_range_test(): failed ref_range_mark_range_free()\n");
    return r;
  }
  fprintf(stderr, "Post-free #1:\n");
  rr = rra.pos_seq_;
  if (rr) {
    uintptr_t last = 0;
    do {
      fprintf(stderr, "from: %d to: %3d refcount: %d\n", (int)last, (int)rr->at_, rr->refcount_);
      last = rr->at_;

      rr = rr->pos_next_;
    } while (rr != rra.pos_seq_);
  }
  uintptr_t result3 = UINTPTR_MAX;
  r = ref_range_alloc(&rra, 500, &result3);
  if (r) {
    fprintf(stderr, "ref_range_test(): failed ref_range_alloc()\n");
    return r;
  }
  fprintf(stderr, "Post-alloc2 (%d):\n", (int)result1);
  rr = rra.pos_seq_;
  if (rr) {
    uintptr_t last = 0;
    do {
      fprintf(stderr, "from: %d to: %3d refcount: %d\n", (int)last, (int)rr->at_, rr->refcount_);
      last = rr->at_;

      rr = rr->pos_next_;
    } while (rr != rra.pos_seq_);
  }
  // XXX: This is broken, expecting a refcount==0 range but not getting it.
  r = ref_range_mark_range_free(&rra, result2, result2 + 1000);
  if (r) {
    fprintf(stderr, "ref_range_test(): failed ref_range_mark_range_free()\n");
    return r;
  }
  fprintf(stderr, "Post-free #2:\n");
  rr = rra.pos_seq_;
  if (rr) {
    uintptr_t last = 0;
    do {
      fprintf(stderr, "from: %d to: %3d refcount: %d\n", (int)last, (int)rr->at_, rr->refcount_);
      last = rr->at_;

      rr = rr->pos_next_;
    } while (rr != rra.pos_seq_);
  }
  // XXX: This is broken and borked. Should yield empty range.
  r = ref_range_mark_range_free(&rra, result3, result3 + 500);
  if (r) {
    fprintf(stderr, "ref_range_test(): failed ref_range_mark_range_free()\n");
    return r;
  }
  fprintf(stderr, "Post-free #3:\n");
  rr = rra.pos_seq_;
  if (rr) {
    uintptr_t last = 0;
    do {
      fprintf(stderr, "from: %d to: %3d refcount: %d\n", (int)last, (int)rr->at_, rr->refcount_);
      last = rr->at_;

      rr = rr->pos_next_;
    } while (rr != rra.pos_seq_);
  }

  if (rra.pos_seq_) {
    fprintf(stderr, "ref_range_test(): all memory freed should result in no ranges.. failed.\n");
    return -1;
  }

  char ranges[256] ={ 0 };
  char range_chk[sizeof(ranges)/sizeof(*ranges)] ={ 0 };
  uintptr_t k;
  for (k = 0; k < (sizeof(ranges)/sizeof(*ranges)); ++k) ranges[k] = 0; /* all is free */
  size_t n;
  for (n = 0; n < 20000; ++n) {
    uintptr_t from, to;
    int alloc_or_free;
    if (0 && n == 30) {
      fprintf(stderr, "Entering area of interest\n");

      struct ref_range *rr = rra.pos_seq_;
      fprintf(stderr, "All ranges:\n");
      if (rr) {
        uintptr_t last = 0;
        do {
          fprintf(stderr, "from: %d to: %3d refcount: %d\n", (int)last, (int)rr->at_, rr->refcount_);
          last = rr->at_;

          rr = rr->pos_next_;
        } while (rr != rra.pos_seq_);
      }
    }
    do {
      from = rand() % (sizeof(ranges) / sizeof(*ranges));
      to = rand() % (sizeof(ranges) / sizeof(*ranges));
      if (from > to) {
        uintptr_t s = from;
        from = to;
        to = s;
      }
    } while (from == to);
    alloc_or_free = rand() & 1;

    if (alloc_or_free) {
      /* alloc range */
      for (k = from; k < to; ++k) {
        ranges[k] += 1;
      }
      int r;
      r = ref_range_apply_ref(&rra, from, to, 1);
      if (r) {
        fprintf(stderr, "ref_range_test(): failed ref_range_mark_range_allocated()\n");
        return r;
      }
      r = ref_range_sanity_check(&rra);
      if (!r) {
        fprintf(stderr, "ref_range_test(): sanity check failed.\n");
        return -1;
      }
    }
    else {
      /* free range */
      for (k = from; k < to; ++k) {
        ranges[k] += -1;
      }
      int r;
      r = ref_range_apply_ref(&rra, from, to, -1);
      if (r) {
        fprintf(stderr, "ref_range_test(): failed ref_range_mark_range_free()\n");
        return r;
      }
      r = ref_range_sanity_check(&rra);
      if (!r) {
        fprintf(stderr, "ref_range_test(): sanity check failed.\n");
        return -1;
      }
    }

    /* Perform range check */
    uintptr_t k;
    for (k = 0; k < (sizeof(ranges)/sizeof(*ranges)); ++k) range_chk[k] = 0;
    struct ref_range *rr = rra.pos_seq_;
    if (0) {
      fprintf(stderr, "All ranges:\n");
      if (rr) {
        uintptr_t last = 0;
        do {
          fprintf(stderr, "from: %d to: %3d refcount: %d\n", (int)last, (int)rr->at_, rr->refcount_);
          last = rr->at_;

          rr = rr->pos_next_;
        } while (rr != rra.pos_seq_);
      }
    }
    if (rr) {
      uintptr_t last = 0;
      do {
        if (last == rr->at_) {
          fprintf(stderr, "ref_range_test(): invalid zero-sized range from: %d to: %3d refcount: %d\n", (int)last, (int)rr->at_, rr->refcount_);
          return -1;
        }
        last = rr->at_;

        rr = rr->pos_next_;
      } while (rr != rra.pos_seq_);
    }
    char is_zero[sizeof(ranges)/sizeof(*ranges)] = {0};
    if (rr) {
      uintptr_t last = 0;
      do {
        for (k = last; k < rr->at_; ++k) {
          if (!rr->refcount_) {
            is_zero[k] = 1;
          }
          else {
            is_zero[k] = 0;
          }
        }
        last = rr->at_;

        rr = rr->pos_next_;
      } while (rr != rra.pos_seq_);
    }
    char marked_zero[sizeof(ranges)/sizeof(*ranges)] = {0};
    rr = rra.sz_seq_;
    if (rr) {
      do {
        uintptr_t from;
        if (rr != rra.pos_seq_) from = rr->pos_prev_->at_;
        else from = 0;

        if (rr->refcount_) {
          fprintf(stderr, "ref_range_test(): non-zero refcount found in available space range from: %d to: %3d refcount: %d\n", (int)from, (int)rr->at_, rr->refcount_);
          return -1;
        }

        for (k = from; k < rr->at_; ++k) {
          marked_zero[k] = 1;

          if (!is_zero[k]) {
            fprintf(stderr, "ref_range_test(): zero not found in range from %d to: %3d refcount: %d\n", (int)from, (int)rr->at_, rr->refcount_);
            return -1;
          }
        }

        rr = rr->sz_next_;
      } while (rr != rra.sz_seq_);
    }
    for (k = 0; k < (sizeof(ranges)/sizeof(*ranges)); ++k) {
      if (marked_zero[k] != is_zero[k]) {
        /* Marked zero means in the sz_seq, is_zero means in the pos_seq, there is a mismatch */
        fprintf(stderr, "ref_range_test(): zero not found in range from %d to: %3d refcount: %d\n", (int)from, (int)rr->at_, rr->refcount_);
        return -1;
      }
    }

    rr = rra.pos_seq_;
    for (k = 0; k < (sizeof(ranges)/sizeof(*ranges)); ++k) {
      if (rr && rr->at_ == k) {
        rr = (rr->pos_next_ == rra.pos_seq_) ? NULL : rr->pos_next_;
      }
      if (!rr) range_chk[k] = 0;
      else range_chk[k] = rr->refcount_;
    }
    
    for (k = 0; k < (sizeof(ranges)/sizeof(*ranges)); ++k) {
      if (range_chk[k] != ranges[k]) {
        fprintf(stderr, "ref_range_test(): sanity range check failed.\n");
        return -1;
      }
    }
  }

  ref_range_allocator_cleanup(&rra);

  return 0;
}
