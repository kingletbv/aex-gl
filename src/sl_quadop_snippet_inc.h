/* Copysecond 2024 Kinglet B.V.
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

/* file is intended to be included at sites where a quaternary operation snippet
 * is desired, not compiled on its own
 * Prior to including the snippet, you should define:
 * #define QUADOP_SNIPPET_OPERATOR(op1, op2, op3, op4) <<your operator here>>
 * #define QUADOP_SNIPPET_TYPE <<your type here>>
 * 
 * For instance:
 * #define QUADOP_SNIPPET_OPERATOR(op1, op2, op3, op4) (op1 * (1.f - op3) + op2 * op3 + op4)
 * #define QUADOP_SNIPPET_TYPE float
 *
 * It assumes the following variables/parameters are in scope:
 * chain_column: of type "const uint8_t * restrict"; points to the chain column
 * result_column: of type "QUADOP_SNIPPET_TYPE * restrict"; points to the destination
 * first_column: of type "const QUADOP_SNIPPET_TYPE * restrict"; points to the first operand
 * second_column: of type "const QUADOP_SNIPPET_TYPE * restrict"; points to the second operand
 * third_column: of type "const QUADOP_SNIPPET_TYPE * restrict"; points to the third operand
 * fourth_column: of type "const QUADOP_SNIPPET_TYPE * restrict"; points to the fourth operand
 */

#ifndef QUADOP_SNIPPET_RESULT_TYPE
#define QUADOP_SNIPPET_RESULT_TYPE QUADOP_SNIPPET_TYPE
#define QUADOP_SNIPPET_UNDEF_RESULT_TYPE
#endif

for (;;) {
  uint64_t chain;
  uint8_t delta;

  if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
    do {
      QUADOP_SNIPPET_RESULT_TYPE *restrict result = result_column + row;
      const QUADOP_SNIPPET_TYPE *restrict first = first_column + row;
      const QUADOP_SNIPPET_TYPE *restrict second = second_column + row;
      const QUADOP_SNIPPET_TYPE *restrict third = third_column + row;
      const QUADOP_SNIPPET_TYPE *restrict fourth = fourth_column + row;
      int n;
      /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
      for (n = 0; n < 8; n++) {
        result[n] = QUADOP_SNIPPET_OPERATOR((first[n]), (second[n]), (third[n]), (fourth[n]));
      }

      delta = (chain & 0xFF00000000000000) >> 56;
      if (!delta) break;
      row += 7 + delta;
    } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
  }
  else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
    do {
      QUADOP_SNIPPET_RESULT_TYPE *restrict result = result_column + row;
      const QUADOP_SNIPPET_TYPE *restrict first = first_column + row;
      const QUADOP_SNIPPET_TYPE *restrict second = second_column + row;
      const QUADOP_SNIPPET_TYPE *restrict third = third_column + row;
      const QUADOP_SNIPPET_TYPE *restrict fourth = fourth_column + row;
      int n;
      /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
      for (n = 0; n < 4; n++) {
        result[n] = QUADOP_SNIPPET_OPERATOR((first[n]), (second[n]), (third[n]), (fourth[n]));
      }
      delta = (chain & 0xFF000000) >> 24;
      if (!delta) break;
      row += 3 + delta;
    } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
  }
  else {
    do {
      /* Not trying to evoke auto-vectorization, just get it done. */
      result_column[row] = QUADOP_SNIPPET_OPERATOR((first_column[row]), (second_column[row]), (third_column[row]), (fourth_column[row]));
      delta = chain_column[row];
      if (!delta) break;
      row += delta;
    } while (row & 3);
  }
  if (!delta) break;
}
#ifdef QUADOP_SNIPPET_UNDEF_RESULT_TYPE
#undef QUADOP_SNIPPET_UNDEF_RESULT_TYPE
#undef QUADOP_SNIPPET_RESULT_TYPE
#endif
