/* Copyright 2023-2024 Kinglet B.V.
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

#ifndef BLEND_H
#define BLEND_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

typedef enum blend_eq {
  BEQ_FUNC_ADD,
  BEQ_FUNC_SUBTRACT,
  BEQ_FUNC_REVERSE_SUBTRACT
} blend_eq_t;

typedef enum blend_func {
  BF_ZERO,
  BF_ONE,
  BF_SRC_COLOR,
  BF_ONE_MINUS_SRC_COLOR,
  BF_DST_COLOR,
  BF_ONE_MINUS_DST_COLOR,
  BF_SRC_ALPHA,
  BF_ONE_MINUS_SRC_ALPHA,
  BF_DST_ALPHA,
  BF_ONE_MINUS_DST_ALPHA,
  BF_CONSTANT_COLOR,
  BF_ONE_MINUS_CONSTANT_COLOR,
  BF_CONSTANT_ALPHA,
  BF_ONE_MINUS_CONSTANT_ALPHA,
  BF_SRC_ALPHA_SATURATE
} blend_func_t;

void blend(size_t num_fragments, uint8_t *maskp, 
           uint8_t *rgba_src_red, uint8_t *rgba_src_green, uint8_t *rgba_src_blue, uint8_t *rgba_src_alpha,
           uint8_t **rgba_dstpp,
           int enable_red, int enable_green, int enable_blue, int enable_alpha,
           blend_eq_t rgb_eq, blend_eq_t alpha_eq,
           blend_func_t src_rgb_fn, blend_func_t src_alpha_fn,
           blend_func_t dst_rgb_fn, blend_func_t dst_alpha_fn,
           uint8_t constant_red, uint8_t constant_grn, uint8_t constant_blu, uint8_t constant_alpha,
           uint8_t *scratch_src_wgh_redp, uint8_t *scratch_src_wgh_grnp, uint8_t *scratch_src_wgh_blup, uint8_t *scratch_src_wgh_alpp,
           uint8_t *scratch_dst_wgh_redp, uint8_t *scratch_dst_wgh_grnp, uint8_t *scratch_dst_wgh_blup, uint8_t *scratch_dst_wgh_alpp);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BLEND_H */
