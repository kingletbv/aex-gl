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

void ref_range_allocator_init(struct ref_range_allocator *rra) {
  rra->sz_root_ = rra->sz_seq_ = NULL;
  rra->pos_root_ = rra->pos_seq_ = NULL;
}

void ref_range_allocator_cleanup(struct ref_range_allocator *rra) {
  while (rra->pos_seq_) {
    struct ref_range *rr = rra->pos_seq_;
    ref_range_sz_remove(rra, rr);
    ref_range_pos_remove(rra, rr);
    ref_range_free(rra, rr);
  }
}

void ref_range_init(struct ref_range *rr) {
  rr->pos_parent_ = rr->pos_left_ = rr->pos_right_ = rr->pos_next_ = rr->pos_prev_ = NULL;
  rr->sz_parent_ = rr->sz_left_ = rr->sz_right_ = rr->sz_next_ = rr->sz_prev_ = NULL;
  rr->at_ = 0;
}

void ref_range_cleanup(struct ref_range *rr) {
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

struct ref_range *ref_range_sz_find_best_fit(struct ref_range_allocator *rra, uintptr_t min_size_needed) {
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

struct ref_range *ref_range_pos_find_at(struct ref_range_allocator *rra, uintptr_t pos) {
  struct ref_range *s;
  struct ref_range *best_fit = NULL;
  s = rra->pos_root_;
  while (s) {
    if (pos < s->at_) {
      best_fit = s;
      s = s->pos_left_;
    }
    else /* (pos >= s->at_) */ {
      s = s->pos_left_;
    }
  }
  return best_fit;
}

struct ref_range *ref_range_pos_find_start(struct ref_range_allocator *rra, uintptr_t pos) {
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
      s = s->pos_left_;
    }
  }
  return best_fit;
}

struct ref_range *ref_range_pos_find_end(struct ref_range_allocator *rra, uintptr_t pos) {
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
      s = s->pos_left_;
    }
  }
  return best_fit;
}

int ref_range_apply_ref(struct ref_range_allocator *rra, uintptr_t from, uintptr_t to, int delta) {
  struct ref_range *rr;
  rr = ref_range_pos_find_start(rra, from);
  if (!rr) {
    if (from) {
      rr = ref_range_alloc_range(rra);
      if (!rr) return -1;

      if (rra->pos_seq_) {
        rra->pos_seq_->pos_prev_->pos_right_ = rr;
        rr->pos_parent_ = rra->pos_seq_->pos_prev_;
        rr->pos_prev_ = rra->pos_seq_->pos_prev_;
        rr->pos_next_ = rra->pos_seq_;
        rr->pos_prev_->pos_next_ = rr->pos_next_->pos_prev_ = rr;
        rr->pos_is_red_ = 1;
        ref_range_pos_process_insert(rra, rr);
      }
      else {
        rr->pos_prev_ = rr->pos_next_ = rr;
        rr->pos_parent_ = NULL;
        rr->pos_left_ = rr->pos_right_ = NULL;
        rr->pos_is_red_ = 0;
        rra->pos_root_ = rr;
        rra->pos_seq_ = rr;
      }
      rr->at_ = from;
      rr->refcount_ = 0;
      ref_range_sz_insert(rra, rr);
    }
    rr = ref_range_alloc_range(rra);
    if (!rra) return -1;

    if (rra->pos_seq_) {
      rra->pos_seq_->pos_prev_->pos_right_ = rr;
      rr->pos_parent_ = rra->pos_seq_->pos_prev_;
      rr->pos_prev_ = rra->pos_seq_->pos_prev_;
      rr->pos_next_ = rra->pos_seq_;
      rr->pos_prev_->pos_next_ = rr->pos_next_->pos_prev_ = rr;
      rr->pos_is_red_ = 1;
      ref_range_pos_process_insert(rra, rr);
    }
    else {
      rr->pos_prev_ = rr->pos_next_ = rr;
      rr->pos_parent_ = NULL;
      rr->pos_left_ = rr->pos_right_ = NULL;
      rr->pos_is_red_ = 0;
      rra->pos_root_ = rr;
      rra->pos_seq_ = rr;
    }
    rr->at_ = to;
    rr->refcount_ = delta;
    ref_range_sz_insert(rra, rr);

    return 0;
  }

  /* rr is start of the range; check if we overlap it or if it is adjacent,
   * if it overlaps, we need to split it into the area that applies the delta
   * and the area outside the from..to region. */
  if (rr->at_ > from) {
    /* Split overlapping start of range */
    struct ref_range *nrr = ref_range_alloc_range(rra);
    if (!nrr) return NULL;
    struct ref_range *prr = nrr;
    if (rr->pos_right_) {
      prr = rr->pos_right_;
      while (prr->pos_left_) prr = prr->pos_left_;
      nrr->pos_parent_ = prr;
      prr->pos_left_ = nrr;
    }
    else {
      rr->pos_right_ = nrr;
      nrr->pos_parent_ = rr;
    }
    nrr->pos_prev_ = rr;
    nrr->pos_next_ = rr->pos_next_;
    nrr->pos_prev_->pos_next_ = nrr->pos_next_->pos_prev_ = nrr;
    nrr->at_ = rr->at_;
    ref_range_sz_remove(rra, rr);
    rr->at_ = from;
    ref_range_sz_insert(rra, rr);
    nrr->refcount_ = rr->refcount_ + delta;
    nrr->pos_is_red_ = 1;
    ref_range_pos_process_insert(rra, nrr);
    ref_range_sz_insert(rra, nrr);
    rr = (nrr->pos_next_ == rra->pos_seq_) ? NULL : nrr->pos_next_;
  }
  /* We know the left edge of rr overlaps the region, so check
   * if the right edge of rr is beyond the "to" end of the region. */
  /* We know the left edge of rr overlaps the region, while the right
   * edge of rr is on or before the "to" end of the region, keep adapting
   * the range with the delta and step to the next one. */
  while (rr && (rr->at_ < to)) {
    /* rr is fully overlapped by range and will be followed by at least
     * one other range (the end of the from..to range has not been reached.) */
    rr->refcount_ += delta;
    rr = (rr->pos_next_ == rra->pos_seq_) ? NULL : rr->pos_next_;
  }

  if (rr) {
    /* here (rr->at_ >= to) is true, this implies that our from..to range ends at or 
     * before the end of the current rr range. */
  }

}
