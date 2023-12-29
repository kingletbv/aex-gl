/* Copyright 2020-2023 Kinglet B.V.
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

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef SITUS_H_INCLUDED
#define SITUS_H_INCLUDED
#include "pp/situs.h"
#endif

#ifndef SYM_TABLE_H_INCLUDED
#define SYM_TABLE_H_INCLUDED
#include "sym_table.h"
#endif

/* Contains red-black tree implementation using ideas from the paper:
 * "Revisiting 2-3 Red-Black Trees with a Pedagogically Sound yet Efficient
 *  Deletion Algorithm: The Parity-Seeking Delete Algorithm" June 2022.
 * (Kamaledin Ghiasi-Shirazia, Taraneh Ghandia, Ali Taghizadeha, Ali Rahimi-Baigia)
 * Great paper to read if you want to follow along. Could not bend my head
 * around the paper's specific use of a nilSentinel node with a left branch
 * pointing to the root node, so code here is more conventional (and cumbersome.)
 * Key difference with regular red-black tree implementations is the ability
 * to delete and insert "from the bottom" (from the node up) rather than having
 * to travel down the tree to ensure the right conditions exist (which can be
 * expensive.)
 * Call st_test() to run some tests.
 */

static int st_depth_check(int *failed_check, struct sym *s) {
  if (!s) return 0;

  int left = st_depth_check(failed_check, s->left_);
  int right = st_depth_check(failed_check, s->right_);

  if (left != right) {
    *failed_check = 1;
    fprintf(stderr, "st_test(): black node balance failed\n");
  }

  return (!s->is_red_) + left;
}

static int st_recurse_linkages(struct sym *parent, struct sym *s) {
  if (s) {
    if (s->parent_ != parent) {
      fprintf(stderr, "st_test(): parent mismatch\n");
      return -1;
    }
    if (s->parent_ && s->is_red_ && s->parent_->is_red_) {
      fprintf(stderr, "st_test(): consecutive reds\n");
      return -1;
    }
    int r;
    r = st_recurse_linkages(s, s->left_);
    r = r ? r : st_recurse_linkages(s, s->right_);
    if (r) return r;
  }
  return 0;
}

static int st_sanity(struct sym_table *st) {
  int failed_balance = 0;
  st_depth_check(&failed_balance, st->root_);
  return st_recurse_linkages(NULL, st->root_) || failed_balance;
}

static int st_recurse_parent_child_linkages(struct sym *parent, struct sym *s) {
  if (s) {
    if (s->parent_ != parent) {
      fprintf(stderr, "st_test(): parent mismatch\n");
      return -1;
    }
    int r;
    r = st_recurse_parent_child_linkages(s, s->left_);
    r = r ? r : st_recurse_parent_child_linkages(s, s->right_);
    if (r) return r;
  }
  return 0;
}

static int st_sanity_only_parents(struct sym_table *st) {
  return st_recurse_parent_child_linkages(NULL, st->root_);
}

void sym_init(struct sym *s) {
  s->is_red_ = 0;
  s->left_ = s->right_ = s->parent_ = s->next_ = s->prev_ = NULL;
  situs_init(&s->loc_);
  s->name_len_ = 0;
  s->name_ = NULL;
}

void sym_cleanup(struct sym *s) {
  situs_cleanup(&s->loc_);
}

static struct sym *st_rotate_left(struct sym_table *st, struct sym *A) {

  /*
   *   A            B
   *  / \    ==>   / \
   * 0   B        A   2 
   *    / \      / \
   *   1   2    0   1
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct sym *B = A->right_;
  A->right_ = B->left_;
  B->left_ = A;
 
  struct sym *A_parent = A->parent_;
  A->parent_ = B;
  if (A->right_) A->right_->parent_ = A;
  B->parent_ = A_parent;
  if (A_parent) {
    if (A_parent->left_ == A) A_parent->left_ = B;
    else /* (A_parent->right_ == A) */ A_parent->right_ = B;
  }
  else {
    /* changing roots */
    st->root_ = B;
  }

  int root_color = A->is_red_;
  A->is_red_ = B->is_red_;
  B->is_red_ = root_color;

  return B;
}

// Rotate right operation
static struct sym *st_rotate_right(struct sym_table *st, struct sym *B) {
  /*
   *     B          A
   *    / \   =>   / \
   *   A   2      0   B
   *  / \            / \
   * 0   1          1   2
   * A.is_red_ = B.is_red_
   * B.is_red_ = A.is_red_
   */
  struct sym *A = B->left_;
  B->left_ = A->right_;
  A->right_ = B;

  struct sym *B_parent = B->parent_;
  B->parent_ = A;
  if (B->left_) B->left_->parent_ = B;
  A->parent_ = B_parent;
  if (B_parent) {
    if (B_parent->left_ == B) B_parent->left_ = A;
    else /* (B_parent->right_ == B) */ B_parent->right_ = A;
  }
  else {
    /* changing roots */
    st->root_ = A;
  }

  int root_color = B->is_red_;
  B->is_red_ = A->is_red_;
  A->is_red_ = root_color;

  return A;
}

static struct sym *st_fixup(struct sym_table *st, struct sym *z) {
  struct sym *x;
  if (!(z->left_ && z->left_->is_red_) && !(z->right_ && z->right_->is_red_)) {
    /* nothing to see here, move along */
    return z->parent_;
  }

  if (z->parent_->left_ == z) {
    if (z->right_ && z->right_->is_red_) {
      z = st_rotate_left(st, z);
    }
    x = st_rotate_right(st, z->parent_);
  }
  else /* (z->parent->right == z) */ {
    if (z->left_ && z->left_->is_red_) {
      z = st_rotate_right(st, z);
    }
    x = st_rotate_left(st, z->parent_);
  }
  /* Red children both turn black, deficiency resolved, return NULL. */
  x->left_->is_red_ = x->right_->is_red_ = 0;
  return NULL;
}

void st_remove(struct sym_table *st, struct sym *s) {
  /* Remove from chain first (easy bit) */
  if (s->next_ != s) {
    s->next_->prev_ = s->prev_;
    s->prev_->next_ = s->next_;
    if (st->seq_ == s) st->seq_ = s->next_;
    s->next_ = s->prev_ = NULL;
  }
  else {
    assert(s == st->seq_);
    s->next_ = s->prev_ = NULL;
    st->seq_ = NULL;
  }

  if (s->right_) {
    /* Substitute with minimum node in right tree, then delete the minimum node. */
    struct sym *rmin = s->right_;
    while (rmin->left_) rmin = rmin->left_;

    /* Swap out s with rmin */
    struct sym *parent, *left, *right;
    int is_red;
    parent = rmin->parent_;
    left = rmin->left_;
    right = rmin->right_;
    is_red = rmin->is_red_;
    rmin->parent_ = s->parent_;
    rmin->left_ = s->left_;
    rmin->right_ = s->right_;
    rmin->is_red_ = s->is_red_;
    s->parent_ = (parent == s) ? rmin : parent;
    s->left_ = left;
    s->right_ = right;
    s->is_red_ = is_red;

    /* Fix up parents */
    if (s->parent_->right_ == rmin) {
      s->parent_->right_ = s;
    }
    else if (s->parent_->left_ == rmin) {
      s->parent_->left_ = s;
    }
    if (rmin->parent_) {
      if (rmin->parent_->right_ == s) {
        rmin->parent_->right_ = rmin;
      }
      else if (rmin->parent_->left_ == s) {
        rmin->parent_->left_ = rmin;
      }
    }
    else {
      assert(st->root_ == s);
      st->root_ = rmin;
    }

    /* Fix up children */
    if (rmin->left_) rmin->left_->parent_ = rmin;
    if (rmin->right_) rmin->right_->parent_ = rmin;
    /* s->left_ should be NULL as the leftmost child of the right branch */
    assert(!s->left_);
    if (s->right_) s->right_->parent_ = s;

    /* s is now in a new location, guaranteed to be a leaf or degree 1 node */
  }


  if (s->left_) {
    /* left must be a red node as right must be NULL at this point (otherwise
     * we would have an inconsistent number of black nodes from left to right) */
    assert(s->left_->is_red_);
    assert(!s->is_red_);
    assert(!s->right_);
    /* Swap positions for s->left and s, then s has been removed. */
    if (s->parent_) {
      if (s->parent_->left_ == s) {
        s->parent_->left_ = s->left_;
      }
      else if (s->parent_->right_ == s) {
        s->parent_->right_ = s->left_;
      }
    }
    else {
      assert(st->root_ == s);
      st->root_ = s->left_;
    }
    s->left_->parent_ = s->parent_;
    s->left_->is_red_ = 0;
    /* Done, s has been evicted from the tree. */
    return ;
  }
  else if (s->right_) {
    /* Same logic as above but now for right, the left branch must be NULL, and
     * the right branch must be red, meaning the s node must be black. */
    assert(s->right_->is_red_);
    assert(!s->is_red_);
    assert(!s->left_);
    /* Swap positions for s->right_ and s */
    if (s->parent_) {
      if (s->parent_->left_ == s) {
        s->parent_->left_ = s->right_;
      }
      else if (s->parent_->right_ == s) {
        s->parent_->right_ = s->right_;
      }
    }
    else {
      assert(st->root_ == s);
      st->root_ = s->right_;
    }
    s->right_->parent_ = s->parent_;
    s->right_->is_red_ = 0;
    /* Done, s has been evicted */
    return ;
  }
  else if (s->is_red_) {
    /* !s->left_ && !s->right_ */
    /* No branches (s is a true leaf node), and the color is red, easy removal,
     * patching up parent. */
    if (s->parent_) {
      if (s->parent_->left_ == s) s->parent_->left_ = NULL;
      else if (s->parent_->right_ == s) s->parent_->right_ = NULL;
    }
    else {
      /* No parent, this must be the last node */
      assert(s == st->root_);
      st->root_ = NULL;
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
    struct sym *x = s;
    for (;;) {
      struct sym *sib = NULL;

      if (!x->parent_) {
        /* Reached the root; done. */
        if (s->parent_) {
          if (s->parent_->left_ == s) {
            s->parent_->left_ = NULL;
          }
          else if (s->parent_->right_ == s) {
            s->parent_->right_ = NULL;
          }
        }
        else {
          assert(st->root_ == s);
          st->root_ = NULL;
        }
        return ;
      }
      sib = (struct sym *)(((uintptr_t)x->parent_->left_) ^ ((uintptr_t)x->parent_->right_) ^ ((uintptr_t)x));
      if (!sib->is_red_) {
        /* Sibling is black, we can induce a deficiency by making it red */
        sib->is_red_ = 1;
        x = st_fixup(st, sib); /* deal with consecutive reds */

        if (!x || x->is_red_) {
          if (x && x->is_red_) {
            /* We can make it black, all is balanced again */
            x->is_red_ = 0;
          }
          /* Deficiency resolved in fixup, all is balanced. */
          if (s->parent_) {
            if (s->parent_->left_ == s) {
              s->parent_->left_ = NULL;
            }
            else if (s->parent_->right_ == s) {
              s->parent_->right_ = NULL;
            }
          }
          else {
            assert(st->root_ == s);
            st->root_ = NULL;
          }
          return;
        }
      }
      else {
        /* Sibling is red. */
        if (x->parent_->left_ == x) {
          st_rotate_left(st, x->parent_);
          sib = x->parent_->right_;
        }
        else {
          st_rotate_right(st, x->parent_);
          sib = x->parent_->left_;
        }

        /* New sibling must be black; we thus revert to the case above, and induce
         * a deficiency in the sibling by making it red, thus balancing the siblings
         * and moving the deficiency to the parent */
        assert(!sib->is_red_);
        sib->is_red_ = 1;
        x = st_fixup(st, sib);  /* deal with consecutive reds */

        if (!x || x->is_red_) {
          if (x && x->is_red_) {
            /* Make parent black, thus completing cycle */
            x->is_red_ = 0;
          }
          if (s->parent_) {
            if (s->parent_->left_ == s) {
              s->parent_->left_ = NULL;
            }
            else if (s->parent_->right_ == s) {
              s->parent_->right_ = NULL;
            }
          }
          else {
            assert(st->root_ == s);
            st->root_ = NULL;
          }
          return ;
        }
      }
    }
  }
}


static int cycle_counter = 0;

static void st_process_insert(struct sym_table *st, struct sym *x) {
  for (;;) {
    /* The idea is we start off by adding the new sym 'x' somewhere into
     * the tree as a child-less leaf node that is red (thus the number of
     * black nodes remains consistent but there may be consecutive reds).
     * We then take the red 'x' and either remove its "redness" or move its 
     * redness up the tree, and setting x to be the parent to repeat that
     * process until we're done.
     * This implies that x is always red. */
    assert(x->is_red_);
    cycle_counter++;

    struct sym *parent = x->parent_;
    if (!parent) {
      /* No parent, therefore x is root; make it black and we're done. */
      assert(x == st->root_);
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
      struct sym *pp = parent->parent_;
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
          parent = st_rotate_left(st, parent);
        }
        /* Now convert:
         *        PP             P
         *       /             /  \
         *     (P)      to   (X)  (PP)
         *     /
         *   (X)
         * Thus implicitly moving x one level up.
         */
        st_rotate_right(st, pp);
      }
      else /* (parent == pp->right_) */ {
        /* This is the mirror version of the p == pp->left_ case (see above for ASCII art) */
        if (x == parent->left_) {
          x = parent;
          parent = st_rotate_right(st, parent);
        }
        st_rotate_left(st, pp);
      }
    }
    else {
      /* No fixups needed, exit */
      return;
    }

  }
}

/* compares the two strings much like strcmp does, but with varying lengths */
static int st_cmp(const char *name_a, size_t name_a_len, const char *name_b, size_t name_b_len) {
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

struct sym *st_find(struct sym_table *st, const char *name, size_t name_len) {
  struct sym *s;
  s = st->root_;
  while (s) {
    int cmp = st_cmp(name, name_len, s->name_, s->name_len_);
    if (cmp < 0) {
      s = s->left_;
    }
    else if (cmp > 0) {
      s = s->right_;
    }
    else {
      /* match */
      return s;
    }
  }
  return NULL; /* not found */
}

struct sym *st_alloc_sym(const char *name, size_t name_len, const struct situs *loc, size_t full_sym_size) {
  struct sym *s = (struct sym *)malloc(full_sym_size + name_len + 1 /* null terminator */);
  if (!s) return NULL;
  sym_init(s);
  if (loc && situs_clone(&s->loc_, loc)) {
    /* no memory */
    free(s);
    return NULL;
  }
  s->name_ = ((char *)s) + full_sym_size;
  memcpy(s->name_, name, name_len);
  s->name_[name_len] = '\0';
  s->name_len_ = name_len;
  return s;
}

sym_table_result_t st_find_or_insert(struct sym_table *st, const char *name, size_t name_len, const struct situs *loc, size_t full_sym_size, struct sym **new_sym) {
  struct sym *s;
  s = st->root_;
  if (!s) {
    /* Have insertion point at the root */
    struct sym *ns = st_alloc_sym(name, name_len, loc, full_sym_size);
    if (!ns) return STR_NOMEM;
    ns->next_ = ns->prev_ = ns;
    st->seq_ = ns;
    ns->is_red_ = 0; /* root is always black */
    st->root_ = ns;
    *new_sym = ns;
    return STR_OK;
  }
  for (;;) {
    int cmp = st_cmp(name, name_len, s->name_, s->name_len_);
    if (cmp < 0) {
      if (!s->left_) {
        /* Have insertion point */
        struct sym *ns = st_alloc_sym(name, name_len, loc, full_sym_size);
        if (!ns) return STR_NOMEM;
        ns->parent_ = s;
        s->left_ = ns;
        ns->is_red_ = 1;
        ns->prev_ = s->prev_;
        ns->next_ = s;
        ns->prev_->next_ = ns->next_->prev_ = ns;

        st_process_insert(st, ns);

        *new_sym = ns;
        return STR_OK;
      }
      s = s->left_;
    }
    else if (cmp > 0) {
      if (!s->right_) {
        /* Have insertion point */
        struct sym *ns = st_alloc_sym(name, name_len, loc, full_sym_size);
        if (!ns) return STR_NOMEM;

        ns->parent_ = s;
        s->right_ = ns;
        ns->is_red_ = 1;
        ns->prev_ = s;
        ns->next_ = s->next_;
        ns->prev_->next_ = ns->next_->prev_ = ns;

        st_process_insert(st, ns);

        *new_sym = ns;
        return STR_OK;
      }
      s = s->right_;
    }
    else /* (cmp == 0) */ {
      /* match */
      *new_sym = s;
      return STR_DUPLICATE;
    }
  }
}

void st_init(struct sym_table *st, struct sym_table *parent) {
  st->parent_ = parent;
  st->root_ = NULL;
  st->seq_ = NULL;
}

void st_cleanup(struct sym_table *st) {
  while (st->seq_) {
    struct sym *s = st->seq_;
    st_remove(st, s);
    sym_cleanup(s);
    free(s);
  }
}

struct testname {
  const char *name_;
  struct sym *s_; /* NULL if not present */
};

static struct testname g_testnames_[] = {
  {"Tokyo"},
  {"Delhi"},
  {"Shanghai"},
  {"Sao Paulo"},
  {"Mexico City"},
  {"Cairo"},
  {"Mumbai"},
  {"Beijing"},
  {"Dhaka"},
  {"Osaka"},
  {"New York City"},
  {"Karachi"},
  {"Buenos Aires"},
  {"Chongqing"},
  {"Istanbul"},
  {"Kolkata"},
  {"Manila"},
  {"Lagos"},
  {"Rio de Janeiro"},
  {"Tianjin"},
  {"Kinshasa"},
  {"Guangzhou"},
  {"Los Angeles"},
  {"Moscow"},
  {"Shenzhen"},
  {"Lahore"},
  {"Bangalore"},
  {"Paris"},
  {"Bogota"},
  {"Jakarta"},
  {"Chennai"},
  {"Lima"},
  {"Bangkok"},
  {"Seoul"},
  {"Nagoya"},
  {"Hyderabad"},
  {"London"},
  {"Tehran"},
  {"Chicago"},
  {"Chengdu"},
  {"Nanjing"},
  {"Wuhan"},
  {"Ho Chi Minh City"},
  {"Luanda"},
  {"Ahmedabad"},
  {"Kuala Lumpur"},
  {"Xi'an"},
  {"Hong Kong"},
  {"Dongguan"},
  {"Hangzhou"},
  {"Foshan"},
  {"Shenyang"},
  {"Riyadh"},
  {"Baghdad"},
  {"Santiago"},
  {"Surat"},
  {"Madrid"},
  {"Suzhou"},
  {"Pune"},
  {"Harbin"},
  {"Houston"},
  {"Dallas"},
  {"Toronto"},
  {"Dar es Salaam"},
  {"Miami"},
  {"Belo Horizonte"},
  {"Singapore"},
  {"Philadelphia"},
  {"Atlanta"},
  {"Fukuoka"},
  {"Khartoum"},
  {"Barcelona"},
  {"Johannesburg"},
  {"Saint Petersburg"},
  {"Qingdao"},
  {"Dalian"},
  {"Washington"},
  {"Yangon"},
  {"Alexandria"},
  {"Jinan"},
  {"Guadalajara"}
};

int st_test(void) {
  int r = -1;
  int schk;
  struct sym_table st;
  st_init(&st, NULL);

  size_t num_names = sizeof(g_testnames_)/sizeof(*g_testnames_);
  size_t n;
  for (n = 0; n < num_names; ++n) {
    struct testname *tn = g_testnames_ + n;
    //printf("#%2d : %s\n", (int)n, tn->name_);
  }

  srand(1233);
  for (n = 0; n < 100000; ++n) {
    size_t tni = rand() % (int)num_names;
    struct testname *tn = g_testnames_ + tni;
    if (tn->s_) {
      schk = st_sanity(&st);
      if (schk) { 
        fprintf(stderr, "(at line %d)\n", __LINE__); r = schk; goto exit;
      }
      st_remove(&st, tn->s_);
      schk = st_sanity(&st);
      if (schk) { 
        fprintf(stderr, "(at line %d)\n", __LINE__); r = schk; goto exit;
      }
      sym_cleanup(tn->s_);
      free(tn->s_);
      schk = st_sanity(&st);
      if (schk) { 
        fprintf(stderr, "(at line %d)\n", __LINE__); r = schk; goto exit; 
      }
      tn->s_ = NULL;
    }
    else {
      sym_table_result_t str;
      schk = st_sanity(&st);
      if (schk) { 
        fprintf(stderr, "(at line %d)\n", __LINE__); r = schk; goto exit;
      }
      str = st_find_or_insert(&st, tn->name_, strlen(tn->name_), NULL, sizeof(struct sym), &tn->s_);
      if (str != STR_OK) {
        fprintf(stderr, "st_test(): Failed testcase\n");
        goto exit;
      }
      schk = st_sanity(&st);
      if (schk) { 
        fprintf(stderr, "(at line %d)\n", __LINE__); r = schk; goto exit;
      }
    }
  }

  for (n = 0; n < num_names; ++n) {
    struct testname *tn = g_testnames_ + n;
    if (tn->s_) {
      st_remove(&st, tn->s_);
      sym_cleanup(tn->s_);
      free(tn->s_);
      tn->s_ = NULL;
    }
  }

  fprintf(stderr, "st_test(): All is well\n");

  r = 0;
exit:
  st_cleanup(&st);
  return r;
}
