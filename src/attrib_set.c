/* Copyright 2023 Kinglet B.V.
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

#ifndef LIMITS_H_INCLUDED
#define LIMITS_H_INCLUDED
#include <limits.h>
#endif

#ifndef STRING_H_INCLUDED_
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef ATTRIB_SET_H_INCLUDED
#define ATTRIB_SET_H_INCLUDED
#include "attrib_set.h"
#endif

void attrib_set_init(struct attrib_set *as) {
  as->num_attribs_ = as->num_attribs_allocated_ = 0;
  as->attribs_ = NULL;
}


void attrib_set_cleanup(struct attrib_set *as) {
  size_t n;
  for (n = 0; n < as->num_attribs_; ++n) {
    if (as->attribs_[n].name_) {
      free(as->attribs_[n].name_);
    }
  }
  if (as->attribs_) free(as->attribs_);
}

/* Only fails in case of memory allocation failure, this can only happen
 * if name is non-NULL. Returns 0 upon success. */
static int attrib_set_attrib_init(struct attrib *attr, char *name) {
  attr->buf_ = NULL;
  attr->size_ = 1;
  attr->att_ = AT_FLOAT;
  attr->normalize_ = 0;
  attr->enabled_ = 0;
  attr->ptr_ = NULL;
  attr->stride_ = sizeof(float);
  attr->generic_values_[0] = attr->generic_values_[1] = attr->generic_values_[2] = 0.f;
  attr->generic_values_[3] = 1.f;
  attr->name_ = NULL;
  if (name) {
    attr->name_ = strdup(name);
    if (!attr->name_) {
      return -1;
    }
  }
  return 0;
}


int attrib_set_alloc_attrib(struct attrib_set *as) {
  if (as->num_attribs_ == as->num_attribs_allocated_) {
    size_t new_num_attribs_allocated = as->num_attribs_allocated_ + as->num_attribs_allocated_ + 1;
    if (new_num_attribs_allocated <= as->num_attribs_allocated_) {
      /* overflow */
      return -1;
    }
    if (new_num_attribs_allocated > INT_MAX) {
      /* overflow */
      return -1;
    }

    struct attrib *new_attribs = (struct attrib *)realloc(as->attribs_, sizeof(struct attrib) * new_num_attribs_allocated);
    if (!new_attribs) {
      /* enomem */
      return -1;
    }

    as->attribs_ = new_attribs;
    as->num_attribs_allocated_ = new_num_attribs_allocated;
  }

  attrib_set_attrib_init(&as->attribs_[as->num_attribs_], NULL);
  return (int)as->num_attribs_++;
}
