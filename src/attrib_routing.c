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

#ifndef SL_DEFS_H_INCLUDED
#define SL_DEFS_H_INCLUDED
#include "sl_defs.h"
#endif

#ifndef SL_REG_ALLOC_H_INCLUDED
#define SL_REG_ALLOC_H_INCLUDED
#include "sl_reg_alloc.h"
#endif

#ifndef SL_FRAME_H_INCLUDED
#define SL_FRAME_H_INCLUDED
#include "sl_frame.h"
#endif

#ifndef ATTRIB_ROUTING_H_INCLUDED
#define ATTRIB_ROUTING_H_INCLUDED
#include "attrib_routing.h"
#endif

static int attrib_routing_add_pairing(struct attrib_routing *ar, int target_register, int source_register) {
  if (ar->num_attribs_routed_ == ar->num_attribs_routed_allocated_) {
    size_t new_size = ar->num_attribs_routed_allocated_ * 2 + 1;
    if (new_size <= ar->num_attribs_routed_allocated_) return SL_ERR_OVERFLOW;
    if (new_size < 16) new_size = 16;
    struct attrib_route *new_pairs = realloc(ar->attribs_routed_, new_size * sizeof(struct attrib_route));
    if (!new_pairs) return 0;
    ar->attribs_routed_ = new_pairs;
    ar->num_attribs_routed_allocated_ = new_size;
  }
  struct attrib_route *pair = ar->attribs_routed_ + ar->num_attribs_routed_++;
  pair->from_source_reg_ = source_register;
  pair->to_target_reg_ = target_register;
  return 0;
}

static int attrib_routing_pair_up(struct attrib_routing *ar, struct sl_reg_alloc *tgt, struct sl_reg_alloc *src, int array_quantity) {
  int num_components = 0;
  int n;

  switch (src->kind_) {
    case slrak_array: {
      if (tgt->v_.array_.num_elements_ > (INT_MAX / array_quantity)) {
        /* overflow */
        return SL_ERR_OVERFLOW;
      }
      array_quantity *= (int)tgt->v_.array_.num_elements_;
      return attrib_routing_pair_up(ar, tgt->v_.array_.head_, src->v_.array_.head_, array_quantity);
    }
    case slrak_struct: {
      size_t index;
      for (index = 0; index < src->v_.comp_.num_fields_; ++index) {
        int r = attrib_routing_pair_up(ar, tgt->v_.comp_.fields_ + index, src->v_.comp_.fields_ + index, array_quantity);
        if (r) return r;
      }
      break;
    }
    case slrak_float:
    case slrak_vec2:
    case slrak_vec3:
    case slrak_vec4:
    case slrak_mat2:
    case slrak_mat3:
    case slrak_mat4: {
      switch (src->kind_) {
        case slrak_float: num_components = 1; break;
        case slrak_vec2:  num_components = 2; break;
        case slrak_vec3:  num_components = 3; break;
        case slrak_vec4:  num_components = 4; break;
        case slrak_mat2:  num_components = 4; break;
        case slrak_mat3:  num_components = 9; break;
        case slrak_mat4:  num_components = 16; break;
          break;
      }
      for (n = 0; n < num_components; ++n) {
        if (tgt->local_frame_ || src->local_frame_) {
          return SL_ERR_INVALID_ARG; /* Cannot route local variables (locals cannot be varyings) */
        }
        int tgt_reg = tgt->v_.regs_[n];
        int src_reg = src->v_.regs_[n];

        int k;
        for (k = 0; k < array_quantity; ++k) {
          int r;
          r = attrib_routing_add_pairing(ar, tgt_reg + k, src_reg + k);
          if (r) return r;
        }
      }
      break;
    }
    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      /* Nothing we can do for integers .. Ignore */
      return SL_ERR_INTERNAL;
    }
    case slrak_bool:
    case slrak_bvec2:
    case slrak_bvec3:
    case slrak_bvec4: {
      /* Nothing we can do for booleans .. Ignore */
      return SL_ERR_INTERNAL;
    }
  }

  return SL_ERR_OK;
}

void attrib_routing_init(struct attrib_routing *ar) {
  ar->num_attribs_routed_ = ar->num_attribs_routed_allocated_ = 0;
  ar->attribs_routed_ = NULL;
}

void attrib_routing_cleanup(struct attrib_routing *ar) {
  if (ar->attribs_routed_) free(ar->attribs_routed_);
}

int attrib_routing_add_variables(struct attrib_routing *ar, struct sl_variable *target, struct sl_variable *source) {
  return attrib_routing_pair_up(ar, &target->reg_alloc_, &source->reg_alloc_, 1);
}
