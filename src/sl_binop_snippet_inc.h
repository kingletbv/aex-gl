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

/* file is intended to be included at sites where a binary operation snippet
 * is desired, not compiled on its own
 * Prior to including the snippet, you should define:
 * #define BINOP_SNIPPET_OPERATOR(op1, op2) <<your operator here>>
 * #define BINOP_SNIPPET_TYPE <<your type here>>
 * 
 * For instance:
 * #define BINOP_SNIPPET_OPERATOR(op1, op2) (op1 + op2)
 * #define BINOP_SNIPPET_TYPE float
 *
 * It assumes the following variables/parameters are in scope:
 * chain_column: of type "const uint8_t * restrict"; points to the chain column
 * result_column: of type "BINOP_SNIPPET_TYPE * restrict"; points to the destination
 * left_column: of type "const BINOP_SNIPPET_TYPE * restrict"; points to the left operand
 * right_column: of type "const BINOP_SNIPPET_TYPE * restrict"; points to the right operand
 */

#ifndef BINOP_SNIPPET_RESULT_TYPE
#define BINOP_SNIPPET_RESULT_TYPE BINOP_SNIPPET_TYPE
#define BINOP_SNIPPET_UNDEF_RESULT_TYPE
#endif

for (;;) {
  uint64_t chain;
  uint8_t delta;

  if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
    do {
      BINOP_SNIPPET_RESULT_TYPE *restrict result = result_column + row;
      const BINOP_SNIPPET_TYPE *restrict left = left_column + row;
      const BINOP_SNIPPET_TYPE *restrict right = right_column + row;
      int n;
      /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
      for (n = 0; n < 8; n++) {
        result[n] = BINOP_SNIPPET_OPERATOR((left[n]), (right[n]));
      }

      delta = (chain & 0xFF00000000000000) >> 56;
      if (!delta) break;
      row += 7 + delta;
    } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
  }
  else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
    do {
      BINOP_SNIPPET_RESULT_TYPE *restrict result = result_column + row;
      const BINOP_SNIPPET_TYPE *restrict left = left_column + row;
      const BINOP_SNIPPET_TYPE *restrict right = right_column + row;
      int n;
      /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
      for (n = 0; n < 4; n++) {
        result[n] = BINOP_SNIPPET_OPERATOR((left[n]), (right[n]));
      }
      delta = (chain & 0xFF000000) >> 24;
      if (!delta) break;
      row += 3 + delta;
    } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
  }
  else {
    do {
      /* Not trying to evoke auto-vectorization, just get it done. */
      result_column[row] = BINOP_SNIPPET_OPERATOR((left_column[row]), (right_column[row]));
      delta = chain_column[row];
      if (!delta) break;
      row += delta;
    } while (row & 3);
  }
  if (!delta) break;
}
#ifdef BINOP_SNIPPET_UNDEF_RESULT_TYPE
#undef BINOP_SNIPPET_UNDEF_RESULT_TYPE
#undef BINOP_SNIPPET_RESULT_TYPE
#endif
