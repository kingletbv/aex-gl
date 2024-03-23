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

/* file is intended to be included at sites where a unary operation snippet
 * is desired, not compiled on its own
 * Prior to including the snippet, you should define:
 * #define UNOP_SNIPPET_OPERATOR(opd) <<your operator here>>
 * #define UNOP_SNIPPET_TYPE <<your type here>>
 * 
 * For instance:
 * #define UNOP_SNIPPET_OPERATOR(opd) (-opd)
 * #define UNOP_SNIPPET_TYPE float
 *
 * It assumes the following variables/parameters are in scope:
 * chain_column: of type "const uint8_t * restrict"; points to the chain column
 * result_column: of type "UNOP_SNIPPET_TYPE * restrict"; points to the destination
 * opd_column: of type "const UNOP_SNIPPET_TYPE * restrict"; points to the operand
 */

#ifndef UNOP_SNIPPET_RESULT_TYPE
#define UNOP_SNIPPET_RESULT_TYPE UNOP_SNIPPET_TYPE
#define UNOP_SNIPPET_UNDEF_RESULT_TYPE
#endif

for (;;) {
  uint64_t chain;
  if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
    do {
      UNOP_SNIPPET_RESULT_TYPE *restrict result = result_column + row;
      const UNOP_SNIPPET_TYPE *restrict opd = opd_column + row;
      int n;
      /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
      for (n = 0; n < 8; n++) {
        result[n] = UNOP_SNIPPET_OPERATOR((opd[n]));
      }

      uint8_t delta = (chain & 0xFF00000000000000) >> 56;
      if (!delta) goto done;
      row += 7 + delta;
    } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
  }
  else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
    do {
      UNOP_SNIPPET_RESULT_TYPE *restrict result = result_column + row;
      const UNOP_SNIPPET_TYPE *restrict opd = opd_column + row;
      int n;
      /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
      for (n = 0; n < 4; n++) {
        result[n] = UNOP_SNIPPET_OPERATOR((opd[n]));
      }
      uint8_t delta = (chain & 0xFF000000) >> 24;
      if (!delta) goto done;
      row += 3 + delta;
    } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
  }
  else {
    do {
      /* Not trying to evoke auto-vectorization, just get it done. */
      result_column[row] = UNOP_SNIPPET_OPERATOR((opd_column[row]));
      uint8_t delta = chain_column[row];
      if (!delta) goto done;
      row += delta;
    } while (row & 3);
  }
}
done: ;
#ifdef UNOP_SNIPPET_UNDEF_RESULT_TYPE
#undef UNOP_SNIPPET_UNDEF_RESULT_TYPE
#undef UNOP_SNIPPET_RESULT_TYPE
#endif
