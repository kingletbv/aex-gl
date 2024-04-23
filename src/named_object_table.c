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

#ifndef NAMED_OBJECT_TABLE_H_INCLUDED
#define NAMED_OBJECT_TABLE_H_INCLUDED
#include "named_object_table.h"
#endif

void named_object_init(struct named_object *no) {
  no->is_red_ = 0;
  no->left_ = no->right_ = no->parent_ = no->next_ = no->prev_ = NULL;
  no->name_ = 0;
}

void named_object_cleanup(struct named_object *no) {
}

void not_init(struct named_object_table *not) {
  not->root_ = NULL;
  not->seq_ = NULL;
}

void not_cleanup(struct named_object_table *not) {
  while (not->seq_) {
    struct named_object *no = not->seq_;
    not_remove(not, no);
    named_object_cleanup(no);
    free(no);
  }
}

static struct named_object *not_rotate_left(struct named_object_table *not, struct named_object *A) {
  /*
   *   A            B
   *  / \    ==>   / \
   * 0   B        A   2 
   *    / \      / \
   *   1   2    0   1
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct named_object *B = A->right_;
  A->right_ = B->left_;
  B->left_ = A;
 
  struct named_object *A_parent = A->parent_;
  A->parent_ = B;
  if (A->right_) A->right_->parent_ = A;
  B->parent_ = A_parent;
  if (A_parent) {
    if (A_parent->left_ == A) A_parent->left_ = B;
    else /* (A_parent->right_ == A) */ A_parent->right_ = B;
  }
  else {
    /* changing roots */
    not->root_ = B;
  }

  int root_color = A->is_red_;
  A->is_red_ = B->is_red_;
  B->is_red_ = root_color;

  return B;
}

// Rotate right operation
static struct named_object *not_rotate_right(struct named_object_table *not, struct named_object *B) {
  /*
   *     B          A
   *    / \   =>   / \
   *   A   2      0   B
   *  / \            / \
   * 0   1          1   2
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct named_object *A = B->left_;
  B->left_ = A->right_;
  A->right_ = B;

  struct named_object *B_parent = B->parent_;
  B->parent_ = A;
  if (B->left_) B->left_->parent_ = B;
  A->parent_ = B_parent;
  if (B_parent) {
    if (B_parent->left_ == B) B_parent->left_ = A;
    else /* (B_parent->right_ == B) */ B_parent->right_ = A;
  }
  else {
    /* changing roots */
    not->root_ = A;
  }

  int root_color = B->is_red_;
  B->is_red_ = A->is_red_;
  A->is_red_ = root_color;

  return A;
}

static struct named_object *not_fixup(struct named_object_table *not, struct named_object *z) {
  struct named_object *x;
  if (!(z->left_ && z->left_->is_red_) && !(z->right_ && z->right_->is_red_)) {
    /* nothing to see here, move along */
    return z->parent_;
  }

  if (z->parent_->left_ == z) {
    if (z->right_ && z->right_->is_red_) {
      z = not_rotate_left(not, z);
    }
    x = not_rotate_right(not, z->parent_);
  }
  else /* (z->parent->right == z) */ {
    if (z->left_ && z->left_->is_red_) {
      z = not_rotate_right(not, z);
    }
    x = not_rotate_left(not, z->parent_);
  }
  /* Red children both turn black, deficiency resolved, return NULL. */
  x->left_->is_red_ = x->right_->is_red_ = 0;
  return NULL;
}

void not_remove(struct named_object_table *not, struct named_object *no) {
  /* Remove from chain first (easy bit) */
  if (no->next_ != no) {
    no->next_->prev_ = no->prev_;
    no->prev_->next_ = no->next_;
    if (not->seq_ == no) not->seq_ = no->next_;
    no->next_ = no->prev_ = NULL;
  }
  else {
    assert(no == not->seq_);
    no->next_ = no->prev_ = NULL;
    not->seq_ = NULL;
  }

  if (no->right_) {
    /* Substitute with minimum node in right tree, then delete the minimum node. */
    struct named_object *rmin = no->right_;
    while (rmin->left_) rmin = rmin->left_;

    /* Swap out s with rmin */
    struct named_object *parent, *left, *right;
    int is_red;
    parent = rmin->parent_;
    left = rmin->left_;
    right = rmin->right_;
    is_red = rmin->is_red_;
    rmin->parent_ = no->parent_;
    rmin->left_ = no->left_;
    rmin->right_ = no->right_;
    rmin->is_red_ = no->is_red_;
    no->parent_ = (parent == no) ? rmin : parent;
    no->left_ = left;
    no->right_ = right;
    no->is_red_ = is_red;

    /* Fix up parents */
    if (no->parent_->right_ == rmin) {
      no->parent_->right_ = no;
    }
    else if (no->parent_->left_ == rmin) {
      no->parent_->left_ = no;
    }
    if (rmin->parent_) {
      if (rmin->parent_->right_ == no) {
        rmin->parent_->right_ = rmin;
      }
      else if (rmin->parent_->left_ == no) {
        rmin->parent_->left_ = rmin;
      }
    }
    else {
      assert(not->root_ == no);
      not->root_ = rmin;
    }

    /* Fix up children */
    if (rmin->left_) rmin->left_->parent_ = rmin;
    if (rmin->right_) rmin->right_->parent_ = rmin;
    /* s->left_ should be NULL as the leftmost child of the right branch */
    assert(!no->left_);
    if (no->right_) no->right_->parent_ = no;

    /* s is now in a new location, guaranteed to be a leaf or degree 1 node */
  }


  if (no->left_) {
    /* left must be a red node as right must be NULL at this point (otherwise
     * we would have an inconsistent number of black nodes from left to right) */
    assert(no->left_->is_red_);
    assert(!no->is_red_);
    assert(!no->right_);
    /* Swap positions for s->left and s, then s has been removed. */
    if (no->parent_) {
      if (no->parent_->left_ == no) {
        no->parent_->left_ = no->left_;
      }
      else if (no->parent_->right_ == no) {
        no->parent_->right_ = no->left_;
      }
    }
    else {
      assert(not->root_ == no);
      not->root_ = no->left_;
    }
    no->left_->parent_ = no->parent_;
    no->left_->is_red_ = 0;
    /* Done, s has been evicted from the tree. */
    return ;
  }
  else if (no->right_) {
    /* Same logic as above but now for right, the left branch must be NULL, and
     * the right branch must be red, meaning the s node must be black. */
    assert(no->right_->is_red_);
    assert(!no->is_red_);
    assert(!no->left_);
    /* Swap positions for s->right_ and s */
    if (no->parent_) {
      if (no->parent_->left_ == no) {
        no->parent_->left_ = no->right_;
      }
      else if (no->parent_->right_ == no) {
        no->parent_->right_ = no->right_;
      }
    }
    else {
      assert(not->root_ == no);
      not->root_ = no->right_;
    }
    no->right_->parent_ = no->parent_;
    no->right_->is_red_ = 0;
    /* Done, s has been evicted */
    return ;
  }
  else if (no->is_red_) {
    /* !s->left_ && !s->right_ */
    /* No branches (s is a true leaf node), and the color is red, easy removal,
     * patching up parent. */
    if (no->parent_) {
      if (no->parent_->left_ == no) no->parent_->left_ = NULL;
      else if (no->parent_->right_ == no) no->parent_->right_ = NULL;
    }
    else {
      /* No parent, this must be the last node */
      assert(no == not->root_);
      not->root_ = NULL;
    }
    /* Done */
    return ;
  }
  else /* (!s->left_ && !s->right_ && !s->is_red_) */ {
    /* s is a true leaf node, and black...
     * when we remove s from the tree, the tree will be imbalanced (for its
     * sibling will have one more black.)
     * one way to view this is to think of s as being "short one black" - however,
     * we cannot change s itself to red, because we're about to remove s. We can
     * make its sibling also "short one black", thereby propagating the deficiency
     * upwards (the siblings will be balanced but now the parent as a whole is
     * "short one black" compared to *its* siblings.) */
    struct named_object *x = no;
    for (;;) {
      struct named_object *sib = NULL;

      if (!x->parent_) {
        /* Reached the root; done. */
        if (no->parent_) {
          if (no->parent_->left_ == no) {
            no->parent_->left_ = NULL;
          }
          else if (no->parent_->right_ == no) {
            no->parent_->right_ = NULL;
          }
        }
        else {
          assert(not->root_ == no);
          not->root_ = NULL;
        }
        return ;
      }
      sib = (struct named_object *)(((uintptr_t)x->parent_->left_) ^ ((uintptr_t)x->parent_->right_) ^ ((uintptr_t)x));
      if (!sib->is_red_) {
        /* Sibling is black, we can induce a deficiency by making it red */
        sib->is_red_ = 1;
        x = not_fixup(not, sib); /* deal with consecutive reds */

        if (!x || x->is_red_) {
          if (x && x->is_red_) {
            /* We can make it black, all is balanced again */
            x->is_red_ = 0;
          }
          /* Deficiency resolved in fixup, all is balanced. */
          if (no->parent_) {
            if (no->parent_->left_ == no) {
              no->parent_->left_ = NULL;
            }
            else if (no->parent_->right_ == no) {
              no->parent_->right_ = NULL;
            }
          }
          else {
            assert(not->root_ == no);
            not->root_ = NULL;
          }
          return;
        }
      }
      else {
        /* Sibling is red. */
        if (x->parent_->left_ == x) {
          not_rotate_left(not, x->parent_);
          sib = x->parent_->right_;
        }
        else {
          not_rotate_right(not, x->parent_);
          sib = x->parent_->left_;
        }

        /* New sibling must be black; we thus revert to the case above, and induce
         * a deficiency in the sibling by making it red, thus balancing the siblings
         * and moving the deficiency to the parent */
        assert(!sib->is_red_);
        sib->is_red_ = 1;
        x = not_fixup(not, sib);  /* deal with consecutive reds */

        if (!x || x->is_red_) {
          if (x && x->is_red_) {
            /* Make parent black, thus completing cycle */
            x->is_red_ = 0;
          }
          if (no->parent_) {
            if (no->parent_->left_ == no) {
              no->parent_->left_ = NULL;
            }
            else if (no->parent_->right_ == no) {
              no->parent_->right_ = NULL;
            }
          }
          else {
            assert(not->root_ == no);
            not->root_ = NULL;
          }
          return ;
        }
      }
    }
  }
}

static void not_process_insert(struct named_object_table *not, struct named_object *x) {
  for (;;) {
    /* The idea is we start off by adding the new sym 'x' somewhere into
     * the tree as a child-less leaf node that is red (thus the number of
     * black nodes remains consistent but there may be consecutive reds).
     * We then take the red 'x' and either remove its "redness" or move its 
     * redness up the tree, and setting x to be the parent to repeat that
     * process until we're done.
     * This implies that x is always red. */
    assert(x->is_red_);

    struct named_object *parent = x->parent_;
    if (!parent) {
      /* No parent, therefore x is root; make it black and we're done. */
      assert(x == not->root_);
      x->is_red_ = 0;
      return;
    }
    if ((parent->left_ && parent->left_->is_red_) && (parent->right_ && parent->right_->is_red_)) {
      /*
       *     P         (P)
       *    / \   =>   / \
       *  (L) (R)     L   R
       * Children of parent (X and its sibling) are red, flip colors to parent.
       */

      /* Parent cannot be red as the sibling of X is also red, which means the
       * tree as we had it prior to insert had to consecutive reds */
      assert(!parent->is_red_);
      parent->is_red_ = 1;
      parent->left_->is_red_ = 0;
      parent->right_->is_red_ = 0;
      x = parent;
    }
    else if (parent->is_red_) {
      /*    (P)        (P)
       *    /      or    \
       *  (X)            (X)
       * What we do depends on what is *above* the red (P).
       * Note that, because the root may never be red, P being red implies
       * it must have a parent. */
      assert(parent->parent_);
      assert(!parent->parent_->is_red_); /* cannot have pre-existing consecutive reds */
      struct named_object *pp = parent->parent_;
      if (parent == pp->left_) {
        /*        PP           PP
         *       /            /
         *     (P)      or  (P)
         *     /              \
         *   (X)              (X)
         * We want to detect the second and rotate it to the first case */
        if (x == parent->right_) {
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
          parent = not_rotate_left(not, parent);
        }
        /* Now convert:
         *        PP             P
         *       /             /  \
         *     (P)      to   (X)  (PP)
         *     /
         *   (X)
         * Thus implicitly moving x one level up.
         */
        not_rotate_right(not, pp);
      }
      else /* (parent == pp->right_) */ {
        /* This is the mirror version of the p == pp->left_ case (see above for ASCII art) */
        if (x == parent->left_) {
          x = parent;
          parent = not_rotate_right(not, parent);
        }
        not_rotate_left(not, pp);
      }
    }
    else {
      /* No fixups needed, exit */
      return;
    }
  }
}

struct named_object *not_find(struct named_object_table *not, uintptr_t name) {
  struct named_object *no;
  no = not->root_;
  while (no) {
    if (name < no->name_) {
      no = no->left_;
    }
    else if (name > no->name_) {
      no = no->right_;
    }
    else {
      /* match */
      return no;
    }
  }
  return NULL; /* not found */
}

static struct named_object *not_alloc_named_object(uintptr_t name, size_t named_object_size) {
  struct named_object *no = (struct named_object *)malloc(named_object_size);
  if (!no) return NULL;
  named_object_init(no);
  no->name_ = name;
  return no;
}

named_object_table_result_t not_find_or_insert(struct named_object_table *not, uintptr_t name, size_t named_object_size, struct named_object **new_no) {
  struct named_object *s;
  s = not->root_;
  if (!s) {
    /* Have insertion point at the root */
    struct named_object *no = not_alloc_named_object(name, named_object_size);
    if (!no) return NOT_NOMEM;
    no->next_ = no->prev_ = no;
    not->seq_ = no;
    no->is_red_ = 0; /* root is always black */
    not->root_ = no;
    *new_no = no;
    return NOT_OK;
  }
  for (;;) {
    if (name < s->name_) {
      if (!s->left_) {
        /* Have insertion point */
        struct named_object *no = not_alloc_named_object(name, named_object_size);
        if (!no) return NOT_NOMEM;
        no->parent_ = s;
        s->left_ = no;
        no->is_red_ = 1;
        no->prev_ = not->seq_->prev_;
        no->next_ = not->seq_;
        no->prev_->next_ = no->next_->prev_ = no;

        not_process_insert(not, no);

        *new_no = no;
        return NOT_OK;
      }
      s = s->left_;
    }
    else if (name > s->name_) {
      if (!s->right_) {
        /* Have insertion point */
        struct named_object *no = not_alloc_named_object(name, named_object_size);
        if (!no) return NOT_NOMEM;

        no->parent_ = s;
        s->right_ = no;
        no->is_red_ = 1;
        no->prev_ = not->seq_->prev_;
        no->next_ = not->seq_;
        no->prev_->next_ = no->next_->prev_ = no;

        not_process_insert(not, no);

        *new_no = no;
        return NOT_OK;
      }
      s = s->right_;
    }
    else /* (name == s->name_) */ {
      /* match */
      *new_no = s;
      return NOT_DUPLICATE;
    }
  }
}

