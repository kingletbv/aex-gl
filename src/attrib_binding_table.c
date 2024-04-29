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

#ifndef ATTRIB_BINDING_TABLE_H_INCLUDED
#define ATTRIB_BINDING_TABLE_H_INCLUDED
#include "attrib_binding_table.h"
#endif

void attrib_binding_init(struct attrib_binding *ab) {
  ab->is_red_ = 0;
  ab->left_ = ab->right_ = ab->parent_ = ab->next_ = ab->prev_ = NULL;
  ab->bound_index_ = -1;
  ab->active_index_ = -1;
  ab->var_ = NULL;
  ab->name_len_ = 0;
  ab->name_[0] = '\0';
}

void attrib_binding_cleanup(struct attrib_binding *ab) {
}

void abt_init(struct attrib_binding_table *abt) {
  abt->root_ = NULL;
  abt->seq_ = NULL;
  abt->num_attrib_bindings_ = abt->num_attrib_bindings_allocated_ = 0;
  abt->attrib_bindings_ = NULL;
}

void abt_cleanup(struct attrib_binding_table *abt) {
  while (abt->seq_) {
    struct attrib_binding *ab = abt->seq_;
    abt_remove(abt, ab);
    attrib_binding_cleanup(ab);
    free(ab);
  }
  if (abt->attrib_bindings_) free(abt->attrib_bindings_);
}

static struct attrib_binding *abt_rotate_left(struct attrib_binding_table *abt, struct attrib_binding *A) {
  /*
   *   A            B
   *  / \    ==>   / \
   * 0   B        A   2 
   *    / \      / \
   *   1   2    0   1
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct attrib_binding *B = A->right_;
  A->right_ = B->left_;
  B->left_ = A;
 
  struct attrib_binding *A_parent = A->parent_;
  A->parent_ = B;
  if (A->right_) A->right_->parent_ = A;
  B->parent_ = A_parent;
  if (A_parent) {
    if (A_parent->left_ == A) A_parent->left_ = B;
    else /* (A_parent->right_ == A) */ A_parent->right_ = B;
  }
  else {
    /* changing roots */
    abt->root_ = B;
  }

  int root_color = A->is_red_;
  A->is_red_ = B->is_red_;
  B->is_red_ = root_color;

  return B;
}

// Rotate right operation
static struct attrib_binding *abt_rotate_right(struct attrib_binding_table *abt, struct attrib_binding *B) {
  /*
   *     B          A
   *    / \   =>   / \
   *   A   2      0   B
   *  / \            / \
   * 0   1          1   2
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct attrib_binding *A = B->left_;
  B->left_ = A->right_;
  A->right_ = B;

  struct attrib_binding *B_parent = B->parent_;
  B->parent_ = A;
  if (B->left_) B->left_->parent_ = B;
  A->parent_ = B_parent;
  if (B_parent) {
    if (B_parent->left_ == B) B_parent->left_ = A;
    else /* (B_parent->right_ == B) */ B_parent->right_ = A;
  }
  else {
    /* changing roots */
    abt->root_ = A;
  }

  int root_color = B->is_red_;
  B->is_red_ = A->is_red_;
  A->is_red_ = root_color;

  return A;
}

static struct attrib_binding *abt_fixup(struct attrib_binding_table *abt, struct attrib_binding *z) {
  struct attrib_binding *x;
  if (!(z->left_ && z->left_->is_red_) && !(z->right_ && z->right_->is_red_)) {
    /* nothing to see here, move along */
    return z->parent_;
  }

  if (z->parent_->left_ == z) {
    if (z->right_ && z->right_->is_red_) {
      z = abt_rotate_left(abt, z);
    }
    x = abt_rotate_right(abt, z->parent_);
  }
  else /* (z->parent->right == z) */ {
    if (z->left_ && z->left_->is_red_) {
      z = abt_rotate_right(abt, z);
    }
    x = abt_rotate_left(abt, z->parent_);
  }
  /* Red children both turn black, deficiency resolved, return NULL. */
  x->left_->is_red_ = x->right_->is_red_ = 0;
  return NULL;
}

void abt_remove(struct attrib_binding_table *abt, struct attrib_binding *ab) {
  /* Remove from chain first (easy bit) */
  if (ab->next_ != ab) {
    ab->next_->prev_ = ab->prev_;
    ab->prev_->next_ = ab->next_;
    if (abt->seq_ == ab) abt->seq_ = ab->next_;
    ab->next_ = ab->prev_ = NULL;
  }
  else {
    assert(ab == abt->seq_);
    ab->next_ = ab->prev_ = NULL;
    abt->seq_ = NULL;
  }

  if (ab->right_) {
    /* Substitute with minimum node in right tree, then delete the minimum node. */
    struct attrib_binding *rmin = ab->right_;
    while (rmin->left_) rmin = rmin->left_;

    /* Swap out s with rmin */
    struct attrib_binding *parent, *left, *right;
    int is_red;
    parent = rmin->parent_;
    left = rmin->left_;
    right = rmin->right_;
    is_red = rmin->is_red_;
    rmin->parent_ = ab->parent_;
    rmin->left_ = ab->left_;
    rmin->right_ = ab->right_;
    rmin->is_red_ = ab->is_red_;
    ab->parent_ = (parent == ab) ? rmin : parent;
    ab->left_ = left;
    ab->right_ = right;
    ab->is_red_ = is_red;

    /* Fix up parents */
    if (ab->parent_->right_ == rmin) {
      ab->parent_->right_ = ab;
    }
    else if (ab->parent_->left_ == rmin) {
      ab->parent_->left_ = ab;
    }
    if (rmin->parent_) {
      if (rmin->parent_->right_ == ab) {
        rmin->parent_->right_ = rmin;
      }
      else if (rmin->parent_->left_ == ab) {
        rmin->parent_->left_ = rmin;
      }
    }
    else {
      assert(abt->root_ == ab);
      abt->root_ = rmin;
    }

    /* Fix up children */
    if (rmin->left_) rmin->left_->parent_ = rmin;
    if (rmin->right_) rmin->right_->parent_ = rmin;
    /* s->left_ should be NULL as the leftmost child of the right branch */
    assert(!ab->left_);
    if (ab->right_) ab->right_->parent_ = ab;

    /* s is now in a new location, guaranteed to be a leaf or degree 1 node */
  }


  if (ab->left_) {
    /* left must be a red node as right must be NULL at this point (otherwise
     * we would have an inconsistent number of black nodes from left to right) */
    assert(ab->left_->is_red_);
    assert(!ab->is_red_);
    assert(!ab->right_);
    /* Swap positions for s->left and s, then s has been removed. */
    if (ab->parent_) {
      if (ab->parent_->left_ == ab) {
        ab->parent_->left_ = ab->left_;
      }
      else if (ab->parent_->right_ == ab) {
        ab->parent_->right_ = ab->left_;
      }
    }
    else {
      assert(abt->root_ == ab);
      abt->root_ = ab->left_;
    }
    ab->left_->parent_ = ab->parent_;
    ab->left_->is_red_ = 0;
    /* Done, s has been evicted from the tree. */
    return ;
  }
  else if (ab->right_) {
    /* Same logic as above but now for right, the left branch must be NULL, and
     * the right branch must be red, meaning the s node must be black. */
    assert(ab->right_->is_red_);
    assert(!ab->is_red_);
    assert(!ab->left_);
    /* Swap positions for s->right_ and s */
    if (ab->parent_) {
      if (ab->parent_->left_ == ab) {
        ab->parent_->left_ = ab->right_;
      }
      else if (ab->parent_->right_ == ab) {
        ab->parent_->right_ = ab->right_;
      }
    }
    else {
      assert(abt->root_ == ab);
      abt->root_ = ab->right_;
    }
    ab->right_->parent_ = ab->parent_;
    ab->right_->is_red_ = 0;
    /* Done, s has been evicted */
    return ;
  }
  else if (ab->is_red_) {
    /* !s->left_ && !s->right_ */
    /* No branches (s is a true leaf node), and the color is red, easy removal,
     * patching up parent. */
    if (ab->parent_) {
      if (ab->parent_->left_ == ab) ab->parent_->left_ = NULL;
      else if (ab->parent_->right_ == ab) ab->parent_->right_ = NULL;
    }
    else {
      /* No parent, this must be the last node */
      assert(ab == abt->root_);
      abt->root_ = NULL;
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
    struct attrib_binding *x = ab;
    for (;;) {
      struct attrib_binding *sib = NULL;

      if (!x->parent_) {
        /* Reached the root; done. */
        if (ab->parent_) {
          if (ab->parent_->left_ == ab) {
            ab->parent_->left_ = NULL;
          }
          else if (ab->parent_->right_ == ab) {
            ab->parent_->right_ = NULL;
          }
        }
        else {
          assert(abt->root_ == ab);
          abt->root_ = NULL;
        }
        return ;
      }
      sib = (struct attrib_binding *)(((uintptr_t)x->parent_->left_) ^ ((uintptr_t)x->parent_->right_) ^ ((uintptr_t)x));
      if (!sib->is_red_) {
        /* Sibling is black, we can induce a deficiency by making it red */
        sib->is_red_ = 1;
        x = abt_fixup(abt, sib); /* deal with consecutive reds */

        if (!x || x->is_red_) {
          if (x && x->is_red_) {
            /* We can make it black, all is balanced again */
            x->is_red_ = 0;
          }
          /* Deficiency resolved in fixup, all is balanced. */
          if (ab->parent_) {
            if (ab->parent_->left_ == ab) {
              ab->parent_->left_ = NULL;
            }
            else if (ab->parent_->right_ == ab) {
              ab->parent_->right_ = NULL;
            }
          }
          else {
            assert(abt->root_ == ab);
            abt->root_ = NULL;
          }
          return;
        }
      }
      else {
        /* Sibling is red. */
        if (x->parent_->left_ == x) {
          abt_rotate_left(abt, x->parent_);
          sib = x->parent_->right_;
        }
        else {
          abt_rotate_right(abt, x->parent_);
          sib = x->parent_->left_;
        }

        /* New sibling must be black; we thus revert to the case above, and induce
         * a deficiency in the sibling by making it red, thus balancing the siblings
         * and moving the deficiency to the parent */
        assert(!sib->is_red_);
        sib->is_red_ = 1;
        x = abt_fixup(abt, sib);  /* deal with consecutive reds */

        if (!x || x->is_red_) {
          if (x && x->is_red_) {
            /* Make parent black, thus completing cycle */
            x->is_red_ = 0;
          }
          if (ab->parent_) {
            if (ab->parent_->left_ == ab) {
              ab->parent_->left_ = NULL;
            }
            else if (ab->parent_->right_ == ab) {
              ab->parent_->right_ = NULL;
            }
          }
          else {
            assert(abt->root_ == ab);
            abt->root_ = NULL;
          }
          return ;
        }
      }
    }
  }
}

static void abt_process_insert(struct attrib_binding_table *abt, struct attrib_binding *x) {
  for (;;) {
    /* The idea is we start off by adding the new sym 'x' somewhere into
     * the tree as a child-less leaf node that is red (thus the number of
     * black nodes remains consistent but there may be consecutive reds).
     * We then take the red 'x' and either remove its "redness" or move its 
     * redness up the tree, and setting x to be the parent to repeat that
     * process until we're done.
     * This implies that x is always red. */
    assert(x->is_red_);

    struct attrib_binding *parent = x->parent_;
    if (!parent) {
      /* No parent, therefore x is root; make it black and we're done. */
      assert(x == abt->root_);
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
      struct attrib_binding *pp = parent->parent_;
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
          parent = abt_rotate_left(abt, parent);
        }
        /* Now convert:
         *        PP             P
         *       /             /  \
         *     (P)      to   (X)  (PP)
         *     /
         *   (X)
         * Thus implicitly moving x one level up.
         */
        abt_rotate_right(abt, pp);
      }
      else /* (parent == pp->right_) */ {
        /* This is the mirror version of the p == pp->left_ case (see above for ASCII art) */
        if (x == parent->left_) {
          x = parent;
          parent = abt_rotate_right(abt, parent);
        }
        abt_rotate_left(abt, pp);
      }
    }
    else {
      /* No fixups needed, exit */
      return;
    }
  }
}

/* compares the two strings much like strcmp does, but with varying lengths */
static int abt_cmp(const char *name_a, size_t name_a_len, const char *name_b, size_t name_b_len) {
  size_t common_len = name_a_len < name_b_len ? name_a_len : name_b_len;
  int r;
  r = memcmp(name_a, name_b, common_len);
  if (r) return r;
  /* common part is identical, all that remains is that the shorter string preceeds the longer one */
  if (name_a_len < name_b_len) return -1;
  if (name_a_len > name_b_len) return 1;
  /* identical */
  return 0;
}

struct attrib_binding *abt_find(struct attrib_binding_table *abt, const char *name, size_t name_len) {
  struct attrib_binding *ab;
  ab = abt->root_;
  while (ab) {
    int cmp = abt_cmp(name, name_len, ab->name_, ab->name_len_);
    if (cmp < 0) {
      ab = ab->left_;
    }
    else if (cmp > 0) {
      ab = ab->right_;
    }
    else {
      /* match */
      return ab;
    }
  }
  return NULL; /* not found */
}

struct attrib_binding *abt_alloc_attrib_binding(struct attrib_binding_table *abt, const char *name, size_t name_len) {
  struct attrib_binding *ab = (struct attrib_binding *)malloc(sizeof(struct attrib_binding) + name_len + 1 /* null terminator */);
  if (!ab) return NULL;
  if (abt->num_attrib_bindings_ == abt->num_attrib_bindings_allocated_) {
    size_t new_num_attrib_bindings_allocated_ = abt->num_attrib_bindings_allocated_ * 2 + 1;
    if (new_num_attrib_bindings_allocated_ <= abt->num_attrib_bindings_allocated_) {
      free(ab);
      return NULL;
    }
    struct attrib_binding **new_attrib_bindings = (struct attrib_binding **)realloc(abt->attrib_bindings_, new_num_attrib_bindings_allocated_ * sizeof(struct attrib_binding *));
    if (!new_attrib_bindings) {
      free(ab);
      return NULL;
    }
    abt->attrib_bindings_ = new_attrib_bindings;
    abt->num_attrib_bindings_allocated_ = new_num_attrib_bindings_allocated_;
  }
  attrib_binding_init(ab);
  memcpy(ab->name_, name, name_len);
  ab->name_[name_len] = '\0';
  ab->name_len_ = name_len;
  ab->attrib_variable_index_ = abt->num_attrib_bindings_;
  abt->attrib_bindings_[abt->num_attrib_bindings_++] = ab;

  return ab;
}

attrib_binding_table_result_t abt_find_or_insert(struct attrib_binding_table *abt, const char *name, size_t name_len, struct attrib_binding **new_ab) {
  struct attrib_binding *s;
  s = abt->root_;
  if (!s) {
    /* Have insertion point at the root */
    struct attrib_binding *nab = abt_alloc_attrib_binding(abt, name, name_len);
    if (!nab) return ABT_NOMEM;
    nab->next_ = nab->prev_ = nab;
    abt->seq_ = nab;
    nab->is_red_ = 0; /* root is always black */
    abt->root_ = nab;
    *new_ab = nab;
    return ABT_OK;
  }
  for (;;) {
    int cmp = abt_cmp(name, name_len, s->name_, s->name_len_);
    if (cmp < 0) {
      if (!s->left_) {
        /* Have insertion point */
        struct attrib_binding *nab = abt_alloc_attrib_binding(abt, name, name_len);
        if (!nab) return ABT_NOMEM;
        nab->parent_ = s;
        s->left_ = nab;
        nab->is_red_ = 1;
        nab->prev_ = abt->seq_->prev_;
        nab->next_ = abt->seq_;
        nab->prev_->next_ = nab->next_->prev_ = nab;

        abt_process_insert(abt, nab);

        *new_ab = nab;
        return ABT_OK;
      }
      s = s->left_;
    }
    else if (cmp > 0) {
      if (!s->right_) {
        /* Have insertion point */
        struct attrib_binding *nab = abt_alloc_attrib_binding(abt, name, name_len);
        if (!nab) return ABT_NOMEM;

        nab->parent_ = s;
        s->right_ = nab;
        nab->is_red_ = 1;
        nab->prev_ = abt->seq_->prev_;
        nab->next_ = abt->seq_;
        nab->prev_->next_ = nab->next_->prev_ = nab;

        abt_process_insert(abt, nab);

        *new_ab = nab;
        return ABT_OK;
      }
      s = s->right_;
    }
    else /* (cmp == 0) */ {
      /* match */
      *new_ab = s;
      return ABT_DUPLICATE;
    }
  }
}

