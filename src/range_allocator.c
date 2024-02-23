#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef RANGE_ALLOCATOR_H_INCLUDED
#define RANGE_ALLOCATOR_H_INCLUDED
#include "range_allocator.h"
#endif

static void ral_range_sz_remove(struct ral_range_allocator *ral, struct ral_range *s);
static void ral_range_pos_remove(struct ral_range_allocator *ral, struct ral_range *s);

static struct ral_range *ral_range_alloc_range(struct ral_range_allocator *ral, uintptr_t from, uintptr_t to);
static void ral_range_free(struct ral_range_allocator *ral, struct ral_range *rr);

static int ral_range_sanity_check(struct ral_range_allocator *ral);

void ral_range_allocator_init(struct ral_range_allocator *ral) {
  ral->sz_root_ = ral->sz_seq_ = NULL;
  ral->pos_root_ = ral->pos_seq_ = NULL;
  ral->watermark_ = 0;
}

void ral_range_allocator_cleanup(struct ral_range_allocator *ral) {
  while (ral->pos_seq_) {
    struct ral_range *rr = ral->pos_seq_;
    ral_range_sz_remove(ral, rr);
    ral_range_pos_remove(ral, rr);
    ral_range_free(ral, rr);
  }
}

void ral_range_init(struct ral_range *rr) {
  rr->pos_parent_ = rr->pos_left_ = rr->pos_right_ = rr->pos_next_ = rr->pos_prev_ = NULL;
  rr->sz_parent_ = rr->sz_left_ = rr->sz_right_ = rr->sz_next_ = rr->sz_prev_ = NULL;
  rr->from_ = rr->to_ = 0;
}

void ral_range_cleanup(struct ral_range *rr) {
}

static struct ral_range *ral_range_sz_rotate_left(struct ral_range_allocator *ral, struct ral_range *A) {
  /*
   *   A            B
   *  / \    ==>   / \
   * 0   B        A   2
   *    / \      / \
   *   1   2    0   1
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct ral_range *B = A->sz_right_;
  A->sz_right_ = B->sz_left_;
  B->sz_left_ = A;

  struct ral_range *A_parent = A->sz_parent_;
  A->sz_parent_ = B;
  if (A->sz_right_) A->sz_right_->sz_parent_ = A;
  B->sz_parent_ = A_parent;
  if (A_parent) {
    if (A_parent->sz_left_ == A) A_parent->sz_left_ = B;
    else /* (A_parent->sz_right_ == A) */ A_parent->sz_right_ = B;
  }
  else {
    /* changing roots */
    ral->sz_root_ = B;
  }

  int root_color = A->sz_is_red_;
  A->sz_is_red_ = B->sz_is_red_;
  B->sz_is_red_ = root_color;

  return B;
}

static struct ral_range *ral_range_sz_rotate_right(struct ral_range_allocator *ral, struct ral_range *B) {
  /*
   *     B          A
   *    / \   =>   / \
   *   A   2      0   B
   *  / \            / \
   * 0   1          1   2
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct ral_range *A = B->sz_left_;
  B->sz_left_ = A->sz_right_;
  A->sz_right_ = B;

  struct ral_range *B_parent = B->sz_parent_;
  B->sz_parent_ = A;
  if (B->sz_left_) B->sz_left_->sz_parent_ = B;
  A->sz_parent_ = B_parent;
  if (B_parent) {
    if (B_parent->sz_left_ == B) B_parent->sz_left_ = A;
    else /* (B_parent->sz_right_ == B) */ B_parent->sz_right_ = A;
  }
  else {
    /* changing roots */
    ral->sz_root_ = A;
  }

  int root_color = B->sz_is_red_;
  B->sz_is_red_ = A->sz_is_red_;
  A->sz_is_red_ = root_color;

  return A;
}

static struct ral_range *ral_range_sz_fixup(struct ral_range_allocator *ral, struct ral_range *z) {
  struct ral_range *x;
  if (!(z->sz_left_ && z->sz_left_->sz_is_red_) && !(z->sz_right_ && z->sz_right_->sz_is_red_)) {
    /* nothing to see here, move along */
    return z->sz_parent_;
  }

  if (z->sz_parent_->sz_left_ == z) {
    if (z->sz_right_ && z->sz_right_->sz_is_red_) {
      z = ral_range_sz_rotate_left(ral, z);
    }
    x = ral_range_sz_rotate_right(ral, z->sz_parent_);
  }
  else /* (z->sz_parent->sz_right == z) */ {
    if (z->sz_left_ && z->sz_left_->sz_is_red_) {
      z = ral_range_sz_rotate_right(ral, z);
    }
    x = ral_range_sz_rotate_left(ral, z->sz_parent_);
  }
  /* Red children both turn black, deficiency resolved, return NULL. */
  x->sz_left_->sz_is_red_ = x->sz_right_->sz_is_red_ = 0;
  return NULL;
}

static void ral_range_sz_remove(struct ral_range_allocator *ral, struct ral_range *s) {
  /* Remove from chain first (easy bit) */
  if (s->sz_next_ != s) {
    s->sz_next_->sz_prev_ = s->sz_prev_;
    s->sz_prev_->sz_next_ = s->sz_next_;
    if (ral->sz_seq_ == s) ral->sz_seq_ = s->sz_next_;
    s->sz_next_ = s->sz_prev_ = NULL;
  }
  else {
    assert(s == ral->sz_seq_);
    s->sz_next_ = s->sz_prev_ = NULL;
    ral->sz_seq_ = NULL;
  }

  if (s->sz_right_) {
    /* Substitute with minimum node in right tree, then delete the minimum node. */
    struct ral_range *rmin = s->sz_right_;
    while (rmin->sz_left_) rmin = rmin->sz_left_;

    /* Swap out s with rmin */
    struct ral_range *parent, *left, *right;
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
      assert(ral->sz_root_ == s);
      ral->sz_root_ = rmin;
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
      assert(ral->sz_root_ == s);
      ral->sz_root_ = s->sz_left_;
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
      assert(ral->sz_root_ == s);
      ral->sz_root_ = s->sz_right_;
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
      assert(s == ral->sz_root_);
      ral->sz_root_ = NULL;
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
    struct ral_range *x = s;
    for (;;) {
      struct ral_range *sib = NULL;

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
          assert(ral->sz_root_ == s);
          ral->sz_root_ = NULL;
        }
        return ;
      }
      sib = (struct ral_range *)(((uintptr_t)x->sz_parent_->sz_left_) ^ ((uintptr_t)x->sz_parent_->sz_right_) ^ ((uintptr_t)x));
      if (!sib->sz_is_red_) {
        /* Sibling is black, we can induce a deficiency by making it red */
        sib->sz_is_red_ = 1;
        x = ral_range_sz_fixup(ral, sib); /* deal with consecutive reds */

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
            assert(ral->sz_root_ == s);
            ral->sz_root_ = NULL;
          }
          return;
        }
      }
      else {
        /* Sibling is red. */
        if (x->sz_parent_->sz_left_ == x) {
          ral_range_sz_rotate_left(ral, x->sz_parent_);
          sib = x->sz_parent_->sz_right_;
        }
        else {
          ral_range_sz_rotate_right(ral, x->sz_parent_);
          sib = x->sz_parent_->sz_left_;
        }

        /* New sibling must be black; we thus revert to the case above, and induce
         * a deficiency in the sibling by making it red, thus balancing the siblings
         * and moving the deficiency to the parent */
        assert(!sib->sz_is_red_);
        sib->sz_is_red_ = 1;
        x = ral_range_sz_fixup(ral, sib);  /* deal with consecutive reds */

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
            assert(ral->sz_root_ == s);
            ral->sz_root_ = NULL;
          }
          return ;
        }
      }
    }
  }
}

static void ral_range_sz_process_insert(struct ral_range_allocator *ral, struct ral_range *x) {
  for (;;) {
    /* The idea is we start off by adding the new sym 'x' somewhere into
     * the tree as a child-less leaf node that is red (thus the number of
     * black nodes remains consistent but there may be consecutive reds).
     * We then take the red 'x' and either remove its "redness" or move its 
     * redness up the tree, and setting x to be the parent to repeat that
     * process until we're done.
     * This implies that x is always red. */
    assert(x->sz_is_red_);

    struct ral_range *parent = x->sz_parent_;
    if (!parent) {
      /* No parent, therefore x is root; make it black and we're done. */
      assert(x == ral->sz_root_);
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
      struct ral_range *pp = parent->sz_parent_;
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
          parent = ral_range_sz_rotate_left(ral, parent);
        }
        /* Now convert:
         *        PP             P
         *       /             /  \
         *     (P)      to   (X)  (PP)
         *     /
         *   (X)
         * Thus implicitly moving x one level up.
         */
        ral_range_sz_rotate_right(ral, pp);
      }
      else /* (parent == pp->right_) */ {
        /* This is the mirror version of the p == pp->left_ case (see above for ASCII art) */
        if (x == parent->sz_left_) {
          x = parent;
          parent = ral_range_sz_rotate_right(ral, parent);
        }
        ral_range_sz_rotate_left(ral, pp);
      }
    }
    else {
      /* No fixups needed, exit */
      return;
    }

  }
}

static int ral_range_sz_cmp(const struct ral_range *left, const struct ral_range *right) {
  uintptr_t left_range = left->to_ - left->from_;
  uintptr_t right_range = right->to_ - right->from_;
  if (left_range < right_range) return -1;
  else if (left_range > right_range) return 1;
  else /* (left_range == right_range) */ {
    /* Sizes are equal, compare the start of the range, this ensures lower ranges go earlier. */
    if (left->from_ < right->from_) return -1;
    else if (left->from_ > right->from_) return 1;
    else /* left->from_ == right->from_, and the ranges are the same, therefore: left == right */ {
      return 0;
    }
  }
}

static void ral_range_sz_insert(struct ral_range_allocator *ral, struct ral_range *rr) {
  struct ral_range *s;
  s = ral->sz_root_;
  struct ral_range *next = NULL, *prev = NULL;
  if (!s) {
    ral->sz_root_ = rr;
    rr->sz_is_red_ = 0;
    ral->sz_seq_ = rr;
    rr->sz_next_ = rr->sz_prev_ = rr;
    return;
  }
  for (;;) {
    int cmp = ral_range_sz_cmp(rr, s);
    if (cmp < 0) {
      next = s;
      if (s->sz_left_) {
        s = s->sz_left_;
      }
      else {
        if (!prev) {
          /* Inserting at head */
          prev = ral->sz_seq_->sz_prev_;
          ral->sz_seq_ = rr; /* new head */
        }

        s->sz_left_ = rr;
        rr->sz_parent_ = s;
        rr->sz_is_red_ = 1;
        rr->sz_next_ = next;
        rr->sz_prev_ = prev;
        rr->sz_next_->sz_prev_ = rr->sz_prev_->sz_next_ = rr;
        rr->sz_left_ = rr->sz_right_ = NULL;
        ral_range_sz_process_insert(ral, rr);
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
          next = ral->sz_seq_;
        }

        s->sz_right_ = rr;
        rr->sz_parent_ = s;
        rr->sz_is_red_ = 1;
        rr->sz_next_ = next;
        rr->sz_prev_ = prev;
        rr->sz_next_->sz_prev_ = rr->sz_prev_->sz_next_ = rr;
        rr->sz_left_ = rr->sz_right_ = NULL;
        ral_range_sz_process_insert(ral, rr);
        return;
      }
    }
  }
}

struct ral_range *ral_range_sz_find_best_fit(struct ral_range_allocator *ral, uintptr_t min_size_needed) {
  struct ral_range *s;
  struct ral_range *best_fit = NULL;
  s = ral->sz_root_;
  while (s) {
    uintptr_t s_range = s->to_ - s->from_;
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

static struct ral_range *ral_range_pos_rotate_left(struct ral_range_allocator *ral, struct ral_range *A) {
  /*
   *   A            B
   *  / \    ==>   / \
   * 0   B        A   2
   *    / \      / \
   *   1   2    0   1
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct ral_range *B = A->pos_right_;
  A->pos_right_ = B->pos_left_;
  B->pos_left_ = A;

  struct ral_range *A_parent = A->pos_parent_;
  A->pos_parent_ = B;
  if (A->pos_right_) A->pos_right_->pos_parent_ = A;
  B->pos_parent_ = A_parent;
  if (A_parent) {
    if (A_parent->pos_left_ == A) A_parent->pos_left_ = B;
    else /* (A_parent->pos_right_ == A) */ A_parent->pos_right_ = B;
  }
  else {
    /* changing roots */
    ral->pos_root_ = B;
  }

  int root_color = A->pos_is_red_;
  A->pos_is_red_ = B->pos_is_red_;
  B->pos_is_red_ = root_color;

  return B;
}

static struct ral_range *ral_range_pos_rotate_right(struct ral_range_allocator *ral, struct ral_range *B) {
  /*
   *     B          A
   *    / \   =>   / \
   *   A   2      0   B
   *  / \            / \
   * 0   1          1   2
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct ral_range *A = B->pos_left_;
  B->pos_left_ = A->pos_right_;
  A->pos_right_ = B;

  struct ral_range *B_parent = B->pos_parent_;
  B->pos_parent_ = A;
  if (B->pos_left_) B->pos_left_->pos_parent_ = B;
  A->pos_parent_ = B_parent;
  if (B_parent) {
    if (B_parent->pos_left_ == B) B_parent->pos_left_ = A;
    else /* (B_parent->pos_right_ == B) */ B_parent->pos_right_ = A;
  }
  else {
    /* changing roots */
    ral->pos_root_ = A;
  }

  int root_color = B->pos_is_red_;
  B->pos_is_red_ = A->pos_is_red_;
  A->pos_is_red_ = root_color;

  return A;
}

static struct ral_range *ral_range_pos_fixup(struct ral_range_allocator *ral, struct ral_range *z) {
  struct ral_range *x;
  if (!(z->pos_left_ && z->pos_left_->pos_is_red_) && !(z->pos_right_ && z->pos_right_->pos_is_red_)) {
    /* nothing to see here, move along */
    return z->pos_parent_;
  }

  if (z->pos_parent_->pos_left_ == z) {
    if (z->pos_right_ && z->pos_right_->pos_is_red_) {
      z = ral_range_pos_rotate_left(ral, z);
    }
    x = ral_range_pos_rotate_right(ral, z->pos_parent_);
  }
  else /* (z->pos_parent->pos_right == z) */ {
    if (z->pos_left_ && z->pos_left_->pos_is_red_) {
      z = ral_range_pos_rotate_right(ral, z);
    }
    x = ral_range_pos_rotate_left(ral, z->pos_parent_);
  }
  /* Red children both turn black, deficiency resolved, return NULL. */
  x->pos_left_->pos_is_red_ = x->pos_right_->pos_is_red_ = 0;
  return NULL;
}

static void ral_range_pos_remove(struct ral_range_allocator *ral, struct ral_range *s) {
  /* Remove from chain first (easy bit) */
  if (s->pos_next_ != s) {
    s->pos_next_->pos_prev_ = s->pos_prev_;
    s->pos_prev_->pos_next_ = s->pos_next_;
    if (ral->pos_seq_ == s) ral->pos_seq_ = s->pos_next_;
    s->pos_next_ = s->pos_prev_ = NULL;
  }
  else {
    assert(s == ral->pos_seq_);
    s->pos_next_ = s->pos_prev_ = NULL;
    ral->pos_seq_ = NULL;
  }

  if (s->pos_right_) {
    /* Substitute with minimum node in right tree, then delete the minimum node. */
    struct ral_range *rmin = s->pos_right_;
    while (rmin->pos_left_) rmin = rmin->pos_left_;

    /* Swap out s with rmin */
    struct ral_range *parent, *left, *right;
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
      assert(ral->pos_root_ == s);
      ral->pos_root_ = rmin;
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
      assert(ral->pos_root_ == s);
      ral->pos_root_ = s->pos_left_;
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
      assert(ral->pos_root_ == s);
      ral->pos_root_ = s->pos_right_;
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
      assert(s == ral->pos_root_);
      ral->pos_root_ = NULL;
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
    struct ral_range *x = s;
    for (;;) {
      struct ral_range *sib = NULL;

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
          assert(ral->pos_root_ == s);
          ral->pos_root_ = NULL;
        }
        return;
      }
      sib = (struct ral_range *)(((uintptr_t)x->pos_parent_->pos_left_) ^ ((uintptr_t)x->pos_parent_->pos_right_) ^ ((uintptr_t)x));
      if (!sib->pos_is_red_) {
        /* Sibling is black, we can induce a deficiency by making it red */
        sib->pos_is_red_ = 1;
        x = ral_range_pos_fixup(ral, sib); /* deal with consecutive reds */

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
            assert(ral->pos_root_ == s);
            ral->pos_root_ = NULL;
          }
          return;
        }
      }
      else {
        /* Sibling is red. */
        if (x->pos_parent_->pos_left_ == x) {
          ral_range_pos_rotate_left(ral, x->pos_parent_);
          sib = x->pos_parent_->pos_right_;
        }
        else {
          ral_range_pos_rotate_right(ral, x->pos_parent_);
          sib = x->pos_parent_->pos_left_;
        }

        /* New sibling must be black; we thus revert to the case above, and induce
         * a deficiency in the sibling by making it red, thus balancing the siblings
         * and moving the deficiency to the parent */
        assert(!sib->pos_is_red_);
        sib->pos_is_red_ = 1;
        x = ral_range_pos_fixup(ral, sib);  /* deal with consecutive reds */

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
            assert(ral->pos_root_ == s);
            ral->pos_root_ = NULL;
          }
          return;
        }
      }
    }
  }
}

static void ral_range_pos_process_insert(struct ral_range_allocator *ral, struct ral_range *x) {
  for (;;) {
    /* The idea is we start off by adding the new sym 'x' somewhere into
     * the tree as a child-less leaf node that is red (thus the number of
     * black nodes remains consistent but there may be consecutive reds).
     * We then take the red 'x' and either remove its "redness" or move its
     * redness up the tree, and setting x to be the parent to repeat that
     * process until we're done.
     * This implies that x is always red. */
    assert(x->pos_is_red_);

    struct ral_range *parent = x->pos_parent_;
    if (!parent) {
      /* No parent, therefore x is root; make it black and we're done. */
      assert(x == ral->pos_root_);
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
      struct ral_range *pp = parent->pos_parent_;
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
          parent = ral_range_pos_rotate_left(ral, parent);
        }
        /* Now convert:
         *        PP             P
         *       /             /  \
         *     (P)      to   (X)  (PP)
         *     /
         *   (X)
         * Thus implicitly moving x one level up.
         */
        ral_range_pos_rotate_right(ral, pp);
      }
      else /* (parent == pp->right_) */ {
        /* This is the mirror version of the p == pp->left_ case (see above for ASCII art) */
        if (x == parent->pos_left_) {
          x = parent;
          parent = ral_range_pos_rotate_right(ral, parent);
        }
        ral_range_pos_rotate_left(ral, pp);
      }
    }
    else {
      /* No fixups needed, exit */
      return;
    }

  }
}

static int ral_range_pos_cmp(const struct ral_range *left, const struct ral_range *right) {
  if (left->from_ < right->from_) return -1;
  else if (left->from_ > right->from_) return 1;
  else /* (left->from_ == right->from_) */ {
    /* Same starting point, so they overlap. This is strange, but not something
     * that's a problem for the red-black tree per se, so we'll disambiguate using
     * the range's end here. */
    if (left->to_ < right->to_) return -1;
    else if (left->to_ > right->to_) return 1;
    return 0;
  }
}

static struct ral_range *ral_range_pos_find_at(struct ral_range_allocator *ral, uintptr_t pos) {
  struct ral_range *s;
  struct ral_range *best_fit = NULL;
  s = ral->pos_root_;
  while (s) {
    if (pos < s->from_) {
      s = s->pos_left_;
    }
    else if (pos >= s->to_) {
      s = s->pos_right_;
    }
    else /* (pos >= s->from_) && (pos < s->to_) */ {
      best_fit = s;
      s = s->pos_left_;
    }
  }
  return best_fit;
}


static struct ral_range *ral_range_pos_find_first_touching_or_after(struct ral_range_allocator *ral, uintptr_t left) {
  /* NOTE: We compare against ral_range::to_ - that works only if:
   * - None of the ral_range's overlap
   * - A ral_range will never be empty (e.g. from_ < to_ is always true)
   * Reason this works is because the sorted sequence of ranges by from_ will then be
   * equal to the sorted sequence of ranges by to_.
   */
  struct ral_range *s;
  struct ral_range *fit = NULL;
  s = ral->pos_root_;
  while (s) {
    if (s->to_ < left) {
      /* to is before left, and not touching. Not a fit */
      s = s->pos_right_;
    }
    else {
      /* to >= left, so either to the right of the left edge, or touching it. A fit */
      fit = s;
      s = s->pos_left_;
    }

  }
  return fit;
}

static struct ral_range *ral_range_pos_find_last_touching_or_before(struct ral_range_allocator *ral, uintptr_t right) {
  struct ral_range *s;
  struct ral_range *fit = NULL;
  s = ral->pos_root_;
  while (s) {
    if (s->from_ <= right) {
      /* from is before right, or touching. A fit */
      fit = s;
      s = s->pos_right_;
    }
    else {
      /* from > right, so to the right of the edge. Not a fit */
      s = s->pos_left_;
    }
  }
  return fit;
}

static struct ral_range *ral_range_pos_find_first_overlapping_or_after(struct ral_range_allocator *ral, uintptr_t left) {
  /* NOTE: We compare against ral_range::to_ - that works only if:
   * - None of the ral_range's overlap
   * - A ral_range will never be empty (e.g. from_ < to_ is always true)
   * Reason this works is because the sorted sequence of ranges by from_ will then be
   * equal to the sorted sequence of ranges by to_.
   */
  struct ral_range *s;
  struct ral_range *fit = NULL;
  s = ral->pos_root_;
  while (s) {
    if (s->to_ <= left) {
      /* to is before, or on, left. Not a fit */
      s = s->pos_right_;
    }
    else {
      /* to > left, so to the right of the left edge. A fit */
      fit = s;
      s = s->pos_left_;
    }

  }
  return fit;
}

static struct ral_range *ral_range_pos_find_last_overlapping_or_before(struct ral_range_allocator *ral, uintptr_t right) {
  struct ral_range *s;
  struct ral_range *fit = NULL;
  s = ral->pos_root_;
  while (s) {
    if (s->from_ < right) {
      /* from is before right. A fit */
      fit = s;
      s = s->pos_right_;
    }
    else {
      /* from >= right, so on, or to the right of the edge. Not a fit */
      s = s->pos_left_;
    }
  }
  return fit;
}

static struct ral_range *ral_range_alloc_range(struct ral_range_allocator *ral, uintptr_t from, uintptr_t to) {
  ral;
  struct ral_range *rr = (struct ral_range *)malloc(sizeof(struct ral_range));
  if (!rr) return NULL;
  ral_range_init(rr);
  rr->from_ = from;
  rr->to_ = to;

  return rr;
}

static void ral_range_free(struct ral_range_allocator *ral, struct ral_range *rr) {
  ral;
  if (!rr) return;
  ral_range_cleanup(rr);
  free(rr);
}

int ral_range_mark_range_free(struct ral_range_allocator *ral, uintptr_t from, uintptr_t to) {
  struct ral_range *rr_first = ral_range_pos_find_first_touching_or_after(ral, from);
  struct ral_range *rr_last = ral_range_pos_find_last_touching_or_before(ral, to);

  // Some thoughts: if no range is found (e.g. we're in a gap, which is a fairly likely case when
  //                we don't touch anything and don't mark things free that are already free,) then
  //                rr_first is NULL, rr_last is NULL, or rr_first->from_ > rr_last->from_
  //                ..
  //                rr_first is NULL and rr_last is non-NULL: append to the end (as last interval); no overlapping ranges
  //                rr_first is non_NULL and rr_last is NULL: insert at the start (as first interval); no overlapping ranges
  //                Both rr_first and rr_last are NULL: there are no intervals; insert as first element.
  if (!rr_first || !rr_last || (rr_first->from_ > rr_last->from_)) {
    /* No overlapping or adjacent ranges. */
    /* range from-to precedes any free range */
    if (to >= ral->watermark_) {
      /* Range to free is adjacent (or overlaps) the watermark beyond which everything is free,
       * so just adjust the watermark. */
      if (from < ral->watermark_) {
        ral->watermark_ = from;
      }
      return 0;
    }
    struct ral_range *rr;
    rr = ral_range_alloc_range(ral, from, to);
    if (!rr) return -1; /* no mem */
    if (!ral->pos_seq_) {
      /* First and only range */
      assert(!rr_first && !rr_last);
      ral->pos_seq_ = rr;
      rr->pos_next_ = rr->pos_prev_ = rr;
      ral->pos_root_ = rr;
      rr->pos_is_red_ = 0; /* root is always black */
      ral_range_sz_insert(ral, rr);
    }
    else if (rr_first && !rr_last) {
      /* Start of the range; add to the left of first node, 
       * note that the first node in a rb tree is always a leaf */
      ral->pos_seq_->pos_left_ = rr;
      rr->pos_parent_ = ral->pos_seq_;
      rr->pos_is_red_ = 1; /* new insertions are always red prior to processing */
      ral_range_pos_process_insert(ral, rr);
      rr->pos_next_ = ral->pos_seq_;
      rr->pos_prev_ = ral->pos_seq_->pos_prev_;
      rr->pos_next_->pos_prev_ = rr->pos_prev_->pos_next_ = rr;
      ral->pos_seq_ = rr;
      ral_range_sz_insert(ral, rr);
    }
    else if (!rr_first && rr_last) {
      /* End of the range; add to the right of the last ndoe,
       * note that the last node in a rb tree is always, a leaf */
      ral->pos_seq_->pos_prev_->pos_right_ = rr;
      rr->pos_parent_ = ral->pos_seq_->pos_prev_;
      rr->pos_is_red_ = 1; /* new insertions are always red prior to processing */
      ral_range_pos_process_insert(ral, rr);
      rr->pos_next_ = ral->pos_seq_;
      rr->pos_prev_ = ral->pos_seq_->pos_prev_;
      rr->pos_next_->pos_prev_ = rr->pos_prev_->pos_next_ = rr;
      ral_range_sz_insert(ral, rr);
    }
    else /* rr_first->from_ > rr_last->from_ */ {
      /* Could not find any overlapping range, so insert new one right in the middle
       * rr_first points to the first range after the new range, rr_last points to
       * the last range before the new range; hence rr_first is after rr_last in
       * the sequence; we'd also expect these two to be adjacent.
       * Find location to insert to the left of rr_first */
      if (rr_first->pos_left_) {
        struct ral_range *rr_parent = rr_first->pos_left_;
        while (rr_parent->pos_right_) rr_parent = rr_parent->pos_right_;
        rr_parent->pos_right_ = rr;
        rr->pos_parent_ = rr_parent;
      }
      else {
        rr_first->pos_left_ = rr;
        rr->pos_parent_ = rr_first;
      }
      rr->pos_is_red_ = 1;  /* new insertions are red prior to processing */
      ral_range_pos_process_insert(ral, rr);
      rr->pos_next_ = rr_first;
      rr->pos_prev_ = rr_last;
      rr->pos_next_->pos_prev_ = rr->pos_prev_->pos_next_ = rr;
      ral_range_sz_insert(ral, rr);
    }
    return 0;
  }

  assert(rr_first && rr_last && (rr_first->from_ < rr_last->to_));
  
  /* Overwriting or extending a range */
  uintptr_t new_from = (from < rr_first->from_) ? from : rr_first->from_;
  uintptr_t new_to = (to > rr_last->to_) ? to : rr_last->to_;

  /* Remove any ranges we no longer need, first range will be adapted to
   * the new range, any remaining overlapping ranges have to go */
  if (rr_first != rr_last) {
    struct ral_range *rr;
    do {
      rr = rr_first->pos_next_;
      ral_range_pos_remove(ral, rr);
      ral_range_sz_remove(ral, rr);
      ral_range_free(ral, rr);
    } while (rr != rr_last);
  }
  if (new_to < ral->watermark_) {
    ral_range_sz_remove(ral, rr_first);
    rr_first->from_ = new_from;
    rr_first->to_ = new_to;
    ral_range_sz_insert(ral, rr_first);
  }
  else {
    /* Area to free bridges the last range with the watermark area,
     * lower watermark to start of the last range and drop the range itself */
    ral->watermark_ = new_from;
    ral_range_sz_remove(ral, rr_first);
    ral_range_pos_remove(ral, rr_first);
    ral_range_free(ral, rr_first);
  }
  return 0;
}

int ral_range_mark_range_allocated(struct ral_range_allocator *ral, uintptr_t from, uintptr_t to) {
  struct ral_range *rr_first = ral_range_pos_find_first_overlapping_or_after(ral, from);
  struct ral_range *rr_last = ral_range_pos_find_last_overlapping_or_before(ral, to);

  if (!rr_first || !rr_last || (rr_first->from_ > rr_last->from_)) {
    /* No overlapping ranges; this means that, aside from the watermark (which we'll
     * check next), the entire range from-to is already "not free", but allocated (again,
     * aside from the watermark.) */
    /* range from-to precedes any free range */
    if (to >= ral->watermark_) {
      /* Range to free is adjacent (or overlaps) the watermark beyond which everything is free,
       * so just adjust the watermark. */
      if (from > ral->watermark_) {
        /* There is a gap between the from-to range we are now marking allocated, and the
         * old watermark level. That gap should be considered free. Insert an interval to
         * reflect this */
        struct ral_range *rr;
        rr = ral_range_alloc_range(ral, ral->watermark_, from);
        if (!rr) return -1; /* no mem */
        if (!ral->pos_seq_) {
          /* First and only range */
          assert(!rr_first && !rr_last);
          ral->pos_seq_ = rr;
          rr->pos_next_ = rr->pos_prev_ = rr;
          ral->pos_root_ = rr;
          rr->pos_is_red_ = 0; /* root is always black */
          ral_range_sz_insert(ral, rr);
        }
        else {
          /* End of the range; add to the right of the last node */
          ral->pos_seq_->pos_prev_->pos_right_ = rr;
          rr->pos_parent_ = ral->pos_seq_->pos_prev_;
          rr->pos_is_red_ = 1; /* new insertions are always red prior to processing */
          ral_range_pos_process_insert(ral, rr);
          rr->pos_next_ = ral->pos_seq_;
          rr->pos_prev_ = ral->pos_seq_->pos_prev_;
          rr->pos_next_->pos_prev_ = rr->pos_prev_->pos_next_ = rr;
          ral_range_sz_insert(ral, rr);
        }
      }
      ral->watermark_ = to;
    }
    return 0;
  }

  if (to > ral->watermark_) {
    /* Note that there can be no vacant ranges in-between the old watermark level and
     * the "to" of our range to allocate; even if (to > ral->watermark) then we know that
     * (from < ral->watermark) because the range <from..to> overlaps at least 1 interval.
     * This simplifies our watermark logic here as we don't need to insert new ranges
     * to represent the exposed free area beyond the old watermark like we did above.. */
    ral->watermark_ = to;
  }

  if (rr_first == rr_last) {
    /* Overlap only one range, determine if we split this range in two */
    if ((rr_first->from_ < from) && (rr_first->to_ > to)) {
      /* Insert new range for last */
      struct ral_range *rr;
      rr = ral_range_alloc_range(ral, to, rr_first->to_);
      /* We insert this immediately after (next to) rr_first */
      if (rr_first->pos_right_) {
        struct ral_range *rr_parent = rr_first->pos_right_;
        while (rr_parent->pos_left_) rr_parent = rr_parent->pos_left_;
        rr_parent->pos_left_ = rr;
        rr->pos_parent_ = rr_parent;
      }
      else {
        rr_first->pos_right_ = rr;
        rr->pos_parent_ = rr_first;
      }
      rr->pos_is_red_ = 1;
      rr->pos_prev_ = rr_first;
      rr->pos_next_ = rr_first->pos_next_;
      rr->pos_next_->pos_prev_ = rr->pos_prev_->pos_next_ = rr;
      ral_range_pos_process_insert(ral, rr);
      ral_range_sz_insert(ral, rr);

      /* We rely on the ral_ranges not overlapping, consequently, rr_first maintains
       * its existing position (because its from doesn't change) - however, its size
       * changes so we re-insert it into the sz tree to reflect any new position. */
      ral_range_sz_remove(ral, rr_first);
      rr_first->to_ = from;
      ral_range_sz_insert(ral, rr_first);
      /* Range split, done */
      return 0;
    }
    else if ((rr_first->from_ >= from) && (rr_first->to_ <= to)) {
      /* Range fully overlapped by range to mark allocated, remove it */
      ral_range_sz_remove(ral, rr_first);
      ral_range_pos_remove(ral, rr_first);
      ral_range_free(ral, rr_first);
      return 0;
    }
    else if ((rr_first->from_ < from) /* && (rr_first->to_ <= to) */) {
      /* Clip tail end of free range */
      ral_range_sz_remove(ral, rr_first);
      rr_first->to_ = from;
      ral_range_sz_insert(ral, rr_first);
      return 0;
    }
    else /* (rr_first->from >= from) && (rr_first->to_ > to) */ {
      /* Clip head end of free range */
      ral_range_sz_remove(ral, rr_first);
      rr_first->from_ = to;
      ral_range_sz_insert(ral, rr_first);
      return 0;
    }
  }

  /* Remove any range in-between first and last */
  struct ral_range *rr;
  rr = rr_first->pos_next_;
  while (rr != rr_last) {
    struct ral_range *rr_next = rr->pos_next_;

    ral_range_sz_remove(ral, rr);
    ral_range_pos_remove(ral, rr);
    ral_range_free(ral, rr);

    rr = rr_next;
  }

  /* Check if we're clipping rr_first or have it fully covered */
  if (rr_first->from_ < from) {
    ral_range_sz_remove(ral, rr_first);
    rr_first->to_ = from;
    ral_range_sz_insert(ral, rr_first);
  }
  else {
    /* Fully covered */
    ral_range_sz_remove(ral, rr_first);
    ral_range_pos_remove(ral, rr_first);
    ral_range_free(ral, rr_first);
  }

  /* Same for rr_last */
  if (rr_last->to_ > to) {
    ral_range_sz_remove(ral, rr_last);
    rr_last->from_ = to;
    ral_range_sz_insert(ral, rr_last);
  }
  else {
    ral_range_sz_remove(ral, rr_last);
    ral_range_pos_remove(ral, rr_last);
    ral_range_free(ral, rr_last);
  }

  return 0;
}


int ral_range_alloc(struct ral_range_allocator *ral, uintptr_t size, uintptr_t *result) {
  struct ral_range *rr = ral_range_sz_find_best_fit(ral, size);
  if (!rr) {
    /* No match, increase size of watermark to accommodate */
    uintptr_t new_watermark = ral->watermark_ + size;
    if (new_watermark < ral->watermark_) {
      return -1; /* failure due to overflow (insanely large allocation, presumably.) */
    }

    *result = ral->watermark_;
    ral->watermark_ = new_watermark;
    return 0;
  }

  int r;
  assert((rr->to_ - rr->from_) >= size); /* getting result that won't accommodate size ???? should never happen. */
  uintptr_t start_of_range = rr->from_;
  r = ral_range_mark_range_allocated(ral, start_of_range, start_of_range + size);
  if (r) return r;
  *result = start_of_range;
  return 0;
}


static int ral_range_sanity_check_pos_parent_pointers(struct ral_range *parent, struct ral_range *child) {
  if (!child) return 1;
  if (child->pos_parent_ != parent) return 0;
  if (!ral_range_sanity_check_pos_parent_pointers(child, child->pos_left_)) return 0;
  if (!ral_range_sanity_check_pos_parent_pointers(child, child->pos_right_)) return 0;
  return 1;
}

static int ral_range_sanity_check_sz_parent_pointers(struct ral_range *parent, struct ral_range *child) {
  if (!child) return 1;

  if (child->sz_parent_ != parent) return 0;
  if (!ral_range_sanity_check_sz_parent_pointers(child, child->sz_left_)) return 0;
  if (!ral_range_sanity_check_sz_parent_pointers(child, child->sz_right_)) return 0;
  return 1;
}

static int ral_range_sanity_check_range_sizes(struct ral_range_allocator *ral) {
  struct ral_range *rr = ral->pos_seq_;
  if (rr) {
    do {
      if (rr->from_ >= rr->to_) {
        fprintf(stderr, "ral_range_sanity_check_range_sizes() failed\n");
        return 0;
      }

      rr = rr->pos_next_;
    } while (rr != ral->pos_seq_);
  }

  return 1;
}

static void ral_range_increment_sz_nodes(struct ral_range *rr) {
  if (!rr) return;
  rr->sanity_ |= 8;
  ral_range_increment_sz_nodes(rr->sz_left_);
  ral_range_increment_sz_nodes(rr->sz_right_);
}

static void ral_range_increment_pos_nodes(struct ral_range *rr) {
  if (!rr) return;
  rr->sanity_ |= 4;
  ral_range_increment_pos_nodes(rr->pos_left_);
  ral_range_increment_pos_nodes(rr->pos_right_);
}

static int ral_range_check_sz_nodes(struct ral_range *rr) {
  if (!rr) return 1;
  if (rr->sanity_ != 15) {
    fprintf(stderr, "ral_range_sanity_check_both_trees() failed\n");
    return 0;
  }
  if (!ral_range_check_sz_nodes(rr->sz_left_)) return 0;
  if (!ral_range_check_sz_nodes(rr->sz_right_)) return 0;
  return 1;
}

static int ral_range_check_pos_nodes(struct ral_range *rr) {
  if (!rr) return 1;
  if (rr->sanity_ != 15) {
    fprintf(stderr, "ral_range_sanity_check_both_trees() failed\n");
    return 0;
  }
  if (!ral_range_check_pos_nodes(rr->pos_left_)) return 0;
  if (!ral_range_check_pos_nodes(rr->pos_right_)) return 0;
  return 1;
}

static int ral_range_check_both_trees(struct ral_range_allocator *ral) {
  struct ral_range *rr = ral->pos_seq_;
  if (rr) {
    do {
      rr->sanity_ = 0;

      rr = rr->pos_next_;
    } while (rr != ral->pos_seq_);
  }

  rr = ral->sz_seq_;
  if (rr) {
    do {
      rr->sanity_ = 0;

      rr = rr->sz_next_;
    } while (rr != ral->sz_seq_);
  }

  rr = ral->pos_seq_;
  if (rr) {
    do {
      rr->sanity_ |= 1;

      rr = rr->pos_next_;
    } while (rr != ral->pos_seq_);
  }

  rr = ral->sz_seq_;
  if (rr) {
    do {
      rr->sanity_ |= 2;

      rr = rr->sz_next_;
    } while (rr != ral->sz_seq_);
  }

  ral_range_increment_pos_nodes(ral->pos_root_); /* marks sanity_ as 4 */
  ral_range_increment_sz_nodes(ral->sz_root_);   /* marks sanity_ as 8 */

  rr = ral->pos_seq_;
  if (rr) {
    do {
      if (rr->sanity_ != 15) {
        fprintf(stderr, "ral_range_sanity_check_both_trees() failed\n");
        return 0;
      }

      rr = rr->pos_next_;
    } while (rr != ral->pos_seq_);
  }

  rr = ral->pos_seq_;
  if (rr) {
    do {
      if (rr->sanity_ != 15) {
        fprintf(stderr, "ral_range_sanity_check_both_trees() failed\n");
        return 0;
      }

      rr = rr->pos_next_;
    } while (rr != ral->pos_seq_);
  }

  return ral_range_check_pos_nodes(ral->pos_root_) && 
         ral_range_check_sz_nodes(ral->sz_root_);
}

static int ral_range_sanity_watermark_check(struct ral_range_allocator *ral) {
  if (!ral->pos_seq_) return 1;
  if (!(ral->watermark_ > ral->pos_seq_->pos_prev_->to_)) {
    /* There is no gap between the watermark and the last interval, this makes
     * the last interval redundant - or - the last interval is inside the watermark
     * area. */
    fprintf(stderr, "ral_range_sanity_watermark_check() failed\n");
    return 0;
  }
  return 1;
}

static int ral_range_sanity_check_pos_ascension(struct ral_range_allocator *ral) {
  struct ral_range *rr = ral->pos_seq_;
  if (rr) {
    do {
      if (rr->pos_next_ != ral->pos_seq_) {
        struct ral_range *rnext = rr->pos_next_;
        if ((rr->from_ >= rnext->from_) ||
            (rr->to_ >= rnext->from_) ||
            (rr->from_ >= rnext->to_) ||
            (rr->to_ >= rnext->to_)) {
          fprintf(stderr, "ral_range_sanity_check_pos_ascension() failed\n");
          return 0;
        }
      }

      rr = rr->pos_next_;
    } while (rr != ral->pos_seq_);
  }

  return 1;
}

static int ral_range_sanity_check_sz_ascension(struct ral_range_allocator *ral) {
  struct ral_range *rr = ral->sz_seq_;
  if (rr) {
    do {
      if (rr->sz_next_ != ral->sz_seq_) {
        struct ral_range *rnext = rr->sz_next_;
        if ((rr->to_ - rr->from_) > (rnext->to_ - rnext->from_)) {
          fprintf(stderr, "ral_range_sanity_check_sz_ascension() failed\n");
          return 0;
        }
      }

      rr = rr->sz_next_;
    } while (rr != ral->sz_seq_);
  }

  return 1;
}

static int ral_range_sanity_check(struct ral_range_allocator *ral) {
  int r = ral_range_sanity_check_pos_parent_pointers(NULL, ral->pos_root_)
       && ral_range_sanity_check_sz_parent_pointers(NULL, ral->sz_root_)
       && ral_range_sanity_check_range_sizes(ral)
       && ral_range_check_both_trees(ral)
       && ral_range_sanity_watermark_check(ral)
       && ral_range_sanity_check_pos_ascension(ral)
       && ral_range_sanity_check_sz_ascension(ral);
  if (!r) {
    fprintf(stderr, "ral_range_sanity_check() failed\n");
  }
  return r;
}

int ral_range_test(void) {
  srand(123);

  struct ral_range_allocator ral;
  ral_range_allocator_init(&ral);

  char ranges[256] = {0};
  char range_chk[sizeof(ranges)/sizeof(*ranges)] = {0};
  uintptr_t k;
  for (k = 0; k < (sizeof(ranges)/sizeof(*ranges)); ++k) ranges[k] = 1; /* all is free */
  size_t n;
  for (n = 0; n < 20000; ++n) {
    uintptr_t from, to;
    int alloc_or_free;
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
        ranges[k] = 0;
      }
      int r;
      r = ral_range_mark_range_allocated(&ral, from, to);
      if (r) {
        fprintf(stderr, "ral_range_test(): failed ral_range_mark_range_allocated()\n");
        return r;
      }
      r = ral_range_sanity_check(&ral);
      if (!r) {
        fprintf(stderr, "ral_range_test(): sanity check failed.\n");
        return -1;
      }
    }
    else {
      /* free range */
      uintptr_t k;
      for (k = from; k < to; ++k) {
        ranges[k] = 1;
      }
      int r;
      r = ral_range_mark_range_free(&ral, from, to);
      if (r) {
        fprintf(stderr, "ral_range_test(): failed ral_range_mark_range_free()\n");
        return r;
      }
      r = ral_range_sanity_check(&ral);
      if (!r) {
        fprintf(stderr, "ral_range_test(): sanity check failed.\n");
        return -1;
      }
    }

    /* Perform range check */
    uintptr_t k;
    for (k = 0; k < (sizeof(ranges)/sizeof(*ranges)); ++k) range_chk[k] = 0;
    struct ral_range *rr = ral.pos_seq_;
    if (rr) {
      do {
        for (k = rr->from_; k < rr->to_; ++k) {
          range_chk[k]++;
        }
        rr = rr->pos_next_;
      } while (rr != ral.pos_seq_);
    }
    for (k = ral.watermark_; k < (sizeof(ranges)/sizeof(*ranges)); ++k) {
      range_chk[k]++;
    }
    for (k = 0; k < (sizeof(ranges)/sizeof(*ranges)); ++k) {
      if (range_chk[k] != ranges[k]) {
        fprintf(stderr, "ral_range_test(): sanity range check failed.\n");
        return -1;
      }
    }
  }

  ral_range_allocator_cleanup(&ral);

  return 0;
}
