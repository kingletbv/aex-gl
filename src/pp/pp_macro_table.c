/* Copyright 2020-2024 Kinglet B.V.
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

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef PP_MACRO_TABLE_H_INCLUDED
#define PP_MACRO_TABLE_H_INCLUDED
#include "pp_macro_table.h"
#endif

#ifndef PP_TOKENS_H_INCLUDED
#define PP_TOKENS_H_INCLUDED
#include "pp_tokens.h"
#endif

static struct macro *mt_rotate_left(struct macro *h);
static struct macro *mt_rotate_right(struct macro *h);
static void mt_flip_colors(struct macro *h);
static struct macro *mt_move_red_left(struct macro *h);
static struct macro *mt_move_red_right(struct macro *h);
static struct macro *mt_fixup(struct macro *h);
static int mt_bincmp(const char *a, size_t a_len, const char *b, size_t b_len);
static struct macro *mt_insert_at(struct macro *h, const char *ident, size_t ident_len,
                                  struct macro **result, int *result_is_new);
static struct macro *mt_delete_and_retrieve_min(struct macro *h, struct macro **min_burrito);
static struct macro *mt_delete_at(struct macro *h, const char *value_key, size_t value_key_len, struct macro **name_deleted);
static int mt_remove_impl(struct macro_table *obj, struct macro *s);

// Rotate left operation
static struct macro *mt_rotate_left(struct macro *h) {
  struct macro *x = h->right_;
  h->right_ = x->left_;
  x->left_ = h;
  x->is_red_ = h->is_red_;
  h->is_red_ = 1;
  return x;
}

// Rotate right operation
static struct macro *mt_rotate_right(struct macro *h) {
  struct macro *x;
  x = h->left_;
  h->left_ = x->right_;
  x->right_ = h;
  x->is_red_ = h->is_red_;
  h->is_red_ = 1;
  return x;
}

// Flip the colors of a node and its two children
static void mt_flip_colors(struct macro *h) {
  struct macro *left = h->left_;
  struct macro *right = h->right_;
  h->is_red_ = !h->is_red_;
  left->is_red_ = !left->is_red_;
  right->is_red_ = !right->is_red_;
}

// Move red node to the left child
static struct macro *mt_move_red_left(struct macro *h) {
  struct macro *right = h->right_;
  mt_flip_colors(h);
  if (right && right->left_ && right->left_->is_red_) {
    h->right_ = mt_rotate_right(h->right_);
    h = mt_rotate_left(h);
    mt_flip_colors(h);
  }
  return h;
}

// Move red node to the right child
static struct macro *mt_move_red_right(struct macro *h) {
  struct macro *left = h->left_;
  mt_flip_colors(h);
  if (left && left->left_ && left->left_->is_red_) {
    h = mt_rotate_right(h);
    mt_flip_colors(h);
  }
  return h;
}

// Fix up the Red-Black tree
static struct macro *mt_fixup(struct macro *h) {
  if (h->right_ && h->right_->is_red_ && (!h->left_ || !h->left_->is_red_)) {
    h = mt_rotate_left(h);
  }

  if (h->left_ && h->left_->is_red_) {
    if (h->left_->left_ && h->left_->left_->is_red_) {
      h = mt_rotate_right(h);
    }
  }

  if (h->left_ && h->left_->is_red_ && h->right_ && h->right_->is_red_) {
    mt_flip_colors(h);
  }

  return h;
}

static int mt_bincmp(const char *a, size_t a_len, const char *b, size_t b_len) {
  int cmp;
  size_t min_len = a_len < b_len ? a_len : b_len;
  cmp = memcmp(a, b, min_len);
  if (cmp) return cmp;
  if (a_len < b_len) return -1;
  if (a_len > b_len) return 1;
  return 0;
}

static struct macro *mt_insert_at(struct macro *h, const char *ident, size_t ident_len,
                                  struct macro **result, int *result_is_new) {
  if (!h) {
    // At leaf, did not find an existing node so allocate a new one
    size_t ident_len = strlen(ident);
    struct macro *n = (struct macro *)malloc(sizeof(struct macro) + ident_len + 1);
    *result = n;
    if (!n) {
      return NULL;
    }
    *result_is_new = 1;
    n->is_red_ = 1;
    n->left_ = n->right_ = n->prev_ = n->next_ = NULL;
    n->name_ = (char *)(n + 1);
    memcpy(n->name_, ident, ident_len + 1);
    n->name_len_ = ident_len;

    return n;
  }

  int cmp = mt_bincmp(ident, ident_len, h->name_, h->name_len_);
  if (cmp < 0) {
    struct macro *new_left_branch = mt_insert_at(h->left_, ident, ident_len, result, result_is_new);
    if (!new_left_branch) return NULL;
    h->left_ = new_left_branch;
  }
  else if (cmp > 0) {
    struct macro *new_right_branch = mt_insert_at(h->right_, ident, ident_len, result, result_is_new);
    if (!new_right_branch) return NULL;
    h->right_ = new_right_branch;
  }
  else /* value_key == h->name_ */ {
    *result = h;
    *result_is_new = 0;
  }

  // The next line assumes that you have a function called 'mt_fixup'
  // that will fix-up the Red-Black tree properties after the insertion.
  return mt_fixup(h);
}

static struct macro *mt_delete_and_retrieve_min(struct macro *h, struct macro **min_burrito) {
  if (h->left_ == NULL) {
    *min_burrito = h;
    return NULL;
  }

  if ((!h->left_ || !h->left_->is_red_) && (!h->left_ || !h->left_->left_ || !h->left_->left_->is_red_)) {
    h = mt_move_red_left(h);
  }

  h->left_ = mt_delete_and_retrieve_min(h->left_, min_burrito);

  return mt_fixup(h);
}

static struct macro *mt_delete_at(struct macro *h, const char *value_key, size_t value_key_len, struct macro **name_deleted) {
  if (mt_bincmp(value_key, value_key_len, h->name_, h->name_len_) < 0) {
    if (!h->left_) {
      /* key not found in tree */
      *name_deleted = NULL;
      return h;
    }
    if ((!h->left_->is_red_) && (!h->left_->left_ || !h->left_->left_->is_red_)) {
      h = mt_move_red_left(h);
    }
    h->left_ = mt_delete_at(h->left_, value_key, value_key_len, name_deleted);
  }
  else {
    if (h->left_ && h->left_->is_red_) {
      h = mt_rotate_right(h);
    }

    if (!mt_bincmp(value_key, value_key_len, h->name_, h->name_len_) && !h->right_) {
      /* Found match at leaf */
      *name_deleted = h;
      return NULL;
    }

    if ((!h->right_ || (!h->right_->is_red_)) && (!h->right_ || !h->right_->left_ || !h->right_->left_->is_red_)) {
      h = mt_move_red_right(h);
    }

    if (!mt_bincmp(value_key, value_key_len, h->name_, h->name_len_)) {
      /* Found match at inner node */
      *name_deleted = h;
      struct macro *sub;
      struct macro *subs_right_arm = mt_delete_and_retrieve_min(h->right_, &sub);
      sub->left_ = h->left_;
      /* Preserve red-ness of this node, even as we're replacing this node. */
      sub->is_red_ = h->is_red_;
      h = sub;
      h->right_ = subs_right_arm;
    }
    else {
      if (!h->right_) {
        /* key not found in tree */
        *name_deleted = NULL;
        return h;
      }
      h->right_ = mt_delete_at(h->right_, value_key, value_key_len, name_deleted);
    }
  }

  return mt_fixup(h);
}


void mt_init(struct macro_table *obj) {
  obj->root_ = NULL;
  obj->seq_ = NULL;
}

static int mt_remove_impl(struct macro_table *obj, struct macro *s) {
  struct macro *s_deleted;
  obj->root_ = mt_delete_at(obj->root_, s->name_, s->name_len_, &s_deleted);
  if (obj->root_) obj->root_->is_red_ = 0;

  assert((s == s_deleted) || !s_deleted); /* if a match exists in the tree, it must be s and not some other name */

  if (s_deleted) {
    if (s_deleted->next_ == s) {
      obj->seq_ = NULL;
    }
    else {
      if (obj->seq_ == s_deleted) {
        obj->seq_ = s_deleted->next_;
      }
      s_deleted->next_->prev_ = s_deleted->prev_;
      s_deleted->prev_->next_ = s_deleted->next_;
    }
    s->next_ = s->prev_ = NULL;

    return MACRO_TABLE_OK;
  }
  return MACRO_TABLE_NOT_FOUND;
}

void mt_cleanup(struct macro_table *obj) {
  while (obj->root_) {
    struct macro *nm = obj->root_;
    mt_remove_impl(obj, nm);
    free(nm);
  }
}

int mt_insert(struct macro_table *obj,
              const char *name, size_t name_len,
              struct macro **pp_new_macro) {
  struct macro *new_node;
  int is_new;
  struct macro *new_root = mt_insert_at(obj->root_, name, name_len, &new_node, &is_new);
  if (!new_root) {
    return MACRO_TABLE_NOMEM;
  }
  obj->root_ = new_root;
  if (is_new) {
    if (obj->seq_) {
      // fix up cyclic next_ and prev_ pointers
      new_node->prev_ = obj->seq_->prev_;
      new_node->next_ = obj->seq_;
      obj->seq_->prev_->next_ = new_node;
      obj->seq_->prev_ = new_node;
    }
    else {
      new_node->prev_ = new_node->next_ = new_node;
      obj->seq_ = new_node;
    }

    *pp_new_macro = new_node;

    return MACRO_TABLE_OK;
  }
  return MACRO_TABLE_DUPLICATE;
}

int mt_remove(struct macro_table *obj, const char *name, size_t name_len) {
  struct macro *s_deleted;
  obj->root_ = mt_delete_at(obj->root_, name, name_len, &s_deleted);
  if (s_deleted) {
    if (s_deleted->next_ == s_deleted) {
      obj->seq_ = NULL;
    }
    else {
      if (obj->seq_ == s_deleted) {
        obj->seq_ = s_deleted->next_;
      }
      s_deleted->next_->prev_ = s_deleted->prev_;
      s_deleted->prev_->next_ = s_deleted->next_;
    }
    s_deleted->next_ = s_deleted->prev_ = NULL;

    return MACRO_TABLE_OK;
  }
  return MACRO_TABLE_NOT_FOUND;
}

int mt_remove_macro(struct macro_table *obj, struct macro *s) {
   return mt_remove_impl(obj, s);
}

struct macro *mt_find(struct macro_table *obj, const char *name, size_t name_len) {
  struct macro *nm = obj->root_;
  for (;;) {
    if (!nm) return NULL;
    int cmp = mt_bincmp(name, name_len, nm->name_, nm->name_len_);
    if (cmp == 0) {
      return nm;
    }
    if (cmp < 0) {
      nm = nm->left_;
    }
    else {
      nm = nm->right_;
    }
  }
}

const struct macro *mt_find_c(const struct macro_table *obj, const char *name, size_t name_len) {
  const struct macro *nm = obj->root_;
  for (;;) {
    if (!nm) return NULL;
    int cmp = mt_bincmp(name, name_len, nm->name_, nm->name_len_);
    if (cmp == 0) {
      return nm;
    }
    if (cmp < 0) {
      nm = nm->left_;
    }
    else {
      nm = nm->right_;
    }
  }
}

void macro_free(struct macro *m) {
  if (!m) return;
  if (--m->refcount_) return; /* still have references, keep alive.. */
  pptk_free(m->args_);
  pptk_free(m->replacement_list_);
  free(m);
}
