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

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef SL_EXPR_H_INCLUDED
#define SL_EXPR_H_INCLUDED
#include "sl_expr.h"
#endif

#ifndef SL_STMT_H_INCLUDED
#define SL_STMT_H_INCLUDED
#include "sl_stmt.h"
#endif

#ifndef SL_EXECUTION_H_INCLUDED
#define SL_EXECUTION_H_INCLUDED
#include "sl_execution.h"
#endif

void sl_exec_f_add(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) left + right
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_i_add(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, const int64_t *restrict left_column, const int64_t *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) left + right
#define BINOP_SNIPPET_TYPE int64_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_f_sub(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) left - right
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_i_sub(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, const int64_t *restrict left_column, const int64_t *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) left - right
#define BINOP_SNIPPET_TYPE int64_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_f_mul(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) left * right
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_i_mul(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, const int64_t *restrict left_column, const int64_t *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) left * right
#define BINOP_SNIPPET_TYPE int64_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_f_div(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) left / right
#define BINOP_SNIPPET_TYPE float
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_i_div(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, const int64_t *restrict left_column, const int64_t *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) left / right
#define BINOP_SNIPPET_TYPE int64_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_logical_or(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const uint8_t *restrict left_column, const uint8_t *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) (0 - ((left || right) && !(left && right)))
#define BINOP_SNIPPET_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_f_lt(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left < right)
#define BINOP_SNIPPET_TYPE float
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_f_le(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left <= right)
#define BINOP_SNIPPET_TYPE float
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_f_gt(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left > right)
#define BINOP_SNIPPET_TYPE float
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_f_ge(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left >= right)
#define BINOP_SNIPPET_TYPE float
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_f_eq(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left == right)
#define BINOP_SNIPPET_TYPE float
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_f_ne(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const float *restrict left_column, const float *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left != right)
#define BINOP_SNIPPET_TYPE float
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_i_lt(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const int64_t *restrict left_column, const int64_t *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left < right)
#define BINOP_SNIPPET_TYPE int64_t
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_i_le(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const int64_t *restrict left_column, const int64_t *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left <= right)
#define BINOP_SNIPPET_TYPE int64_t
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_i_gt(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const int64_t *restrict left_column, const int64_t *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left > right)
#define BINOP_SNIPPET_TYPE int64_t
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_i_ge(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const int64_t *restrict left_column, const int64_t *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left >= right)
#define BINOP_SNIPPET_TYPE int64_t
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_i_eq(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const int64_t *restrict left_column, const int64_t *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left == right)
#define BINOP_SNIPPET_TYPE int64_t
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_i_ne(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const int64_t *restrict left_column, const int64_t *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left != right)
#define BINOP_SNIPPET_TYPE int64_t
#define BINOP_SNIPPET_RESULT_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_b_eq(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const uint8_t *restrict left_column, const uint8_t *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left == right)
#define BINOP_SNIPPET_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_b_ne(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const uint8_t *restrict left_column, const uint8_t *restrict right_column) {
#define BINOP_SNIPPET_OPERATOR(left, right) 0 - (left != right)
#define BINOP_SNIPPET_TYPE uint8_t
#include "sl_binop_snippet_inc.h"
#undef BINOP_SNIPPET_OPERATOR
#undef BINOP_SNIPPET_TYPE
}

void sl_exec_f_move(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const float *restrict src_column) {
  if (result_column == src_column) return;
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict src = src_column + row;
        int n;
        for (n = 0; n < 8; n++) {
          result[n] = src[n];
        }

        uint8_t delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) goto done;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict src = src_column + row;
        int n;
        for (n = 0; n < 4; n++) {
          result[n] = src[n];
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        result_column[row] = src_column[row];
        uint8_t delta = chain_column[row];
        if (!delta) goto done;
        row += delta;
      } while (row & 3);
    }
  }
done:;
}

void sl_exec_i_move(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, const int64_t *restrict src_column) {
  if (result_column == src_column) return;
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        int64_t *restrict result = result_column + row;
        const int64_t *restrict src = src_column + row;
        int n;
        for (n = 0; n < 8; n++) {
          result[n] = src[n];
        }

        uint8_t delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) goto done;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        int64_t *restrict result = result_column + row;
        const int64_t *restrict src = src_column + row;
        int n;
        for (n = 0; n < 4; n++) {
          result[n] = src[n];
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        result_column[row] = src_column[row];
        uint8_t delta = chain_column[row];
        if (!delta) goto done;
        row += delta;
      } while (row & 3);
    }
  }
done:;
}

void sl_exec_b_move(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const uint8_t *restrict src_column) {
  if (result_column == src_column) return;
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        uint8_t *restrict result = result_column + row;
        const uint8_t *restrict src = src_column + row;
        int n;
        for (n = 0; n < 8; n++) {
          result[n] = src[n];
        }

        uint8_t delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) goto done;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        uint8_t *restrict result = result_column + row;
        const uint8_t *restrict src = src_column + row;
        int n;
        for (n = 0; n < 4; n++) {
          result[n] = src[n];
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        result_column[row] = src_column[row];
        uint8_t delta = chain_column[row];
        if (!delta) goto done;
        row += delta;
      } while (row & 3);
    }
  }
done:;
}

void sl_exec_f_init(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, float src) {
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result = result_column + row;
        int n;
        for (n = 0; n < 8; n++) {
          result[n] = src;
        }

        uint8_t delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) goto done;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result = result_column + row;
        int n;
        for (n = 0; n < 4; n++) {
          result[n] = src;
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        result_column[row] = src;
        uint8_t delta = chain_column[row];
        if (!delta) goto done;
        row += delta;
      } while (row & 3);
    }
  }
done:;
}

void sl_exec_i_init(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, int64_t src) {
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        int64_t *restrict result = result_column + row;
        int n;
        for (n = 0; n < 8; n++) {
          result[n] = src;
        }

        uint8_t delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) goto done;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        int64_t *restrict result = result_column + row;
        int n;
        for (n = 0; n < 4; n++) {
          result[n] = src;
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        result_column[row] = src;
        uint8_t delta = chain_column[row];
        if (!delta) goto done;
        row += delta;
      } while (row & 3);
    }
  }
done:;
}

void sl_exec_b_init(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, uint8_t src) {
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        uint8_t *restrict result = result_column + row;
        int n;
        for (n = 0; n < 8; n++) {
          result[n] = src;
        }

        uint8_t delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) goto done;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        uint8_t *restrict result = result_column + row;
        int n;
        for (n = 0; n < 4; n++) {
          result[n] = src;
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        result_column[row] = src;
        uint8_t delta = chain_column[row];
        if (!delta) goto done;
        row += delta;
      } while (row & 3);
    }
  }
done:;
}


void sl_exec_f_dot_product2(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const float *restrict left_0_column, const float *restrict left_1_column, const float *restrict right_0_column, const float *restrict right_1_column) {
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict left0 = left_0_column + row;
        const float *restrict left1 = left_1_column + row;
        const float *restrict right0 = right_0_column + row;
        const float *restrict right1 = right_1_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          result[n] = left0[n] * right0[n] + left1[n] * right1[n];
        }

        uint8_t delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) goto done;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict left0 = left_0_column + row;
        const float *restrict left1 = left_1_column + row;
        const float *restrict right0 = right_0_column + row;
        const float *restrict right1 = right_1_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          result[n] = left0[n] * right0[n] + left1[n] * right1[n];
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        result_column[row] = left_0_column[row] * right_0_column[row] + left_1_column[row] * right_1_column[row];
        uint8_t delta = chain_column[row];
        if (!delta) goto done;
        row += delta;
      } while (row & 3);
    }
  }
done:;
}

void sl_exec_f_dot_product3(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, 
                            const float *restrict left_0_column, 
                            const float *restrict left_1_column, 
                            const float *restrict left_2_column, 
                            const float *restrict right_0_column,
                            const float *restrict right_1_column,
                            const float *restrict right_2_column) {
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict left0 = left_0_column + row;
        const float *restrict left1 = left_1_column + row;
        const float *restrict left2 = left_2_column + row;
        const float *restrict right0 = right_0_column + row;
        const float *restrict right1 = right_1_column + row;
        const float *restrict right2 = right_2_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          result[n] = left0[n] * right0[n] + left1[n] * right1[n] + left2[n] * right2[n];
        }

        uint8_t delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) goto done;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict left0 = left_0_column + row;
        const float *restrict left1 = left_1_column + row;
        const float *restrict left2 = left_2_column + row;
        const float *restrict right0 = right_0_column + row;
        const float *restrict right1 = right_1_column + row;
        const float *restrict right2 = right_2_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          result[n] = left0[n] * right0[n] + left1[n] * right1[n] + left2[n] * right2[n];
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        result_column[row] = left_0_column[row] * right_0_column[row] 
                           + left_1_column[row] * right_1_column[row]
                           + left_2_column[row] * right_2_column[row];
        uint8_t delta = chain_column[row];
        if (!delta) goto done;
        row += delta;
      } while (row & 3);
    }
  }
done:;
}

void sl_exec_f_dot_product4(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, 
                            const float *restrict left_0_column, 
                            const float *restrict left_1_column, 
                            const float *restrict left_2_column, 
                            const float *restrict left_3_column, 
                            const float *restrict right_0_column,
                            const float *restrict right_1_column,
                            const float *restrict right_2_column,
                            const float *restrict right_3_column) {
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict left0 = left_0_column + row;
        const float *restrict left1 = left_1_column + row;
        const float *restrict left2 = left_2_column + row;
        const float *restrict left3 = left_3_column + row;
        const float *restrict right0 = right_0_column + row;
        const float *restrict right1 = right_1_column + row;
        const float *restrict right2 = right_2_column + row;
        const float *restrict right3 = right_3_column + row;
        int n;
        /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
        for (n = 0; n < 8; n++) {
          result[n] = left0[n] * right0[n] + left1[n] * right1[n] + left2[n] * right2[n] + left3[n] * right3[n];
        }

        uint8_t delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) goto done;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result = result_column + row;
        const float *restrict left0 = left_0_column + row;
        const float *restrict left1 = left_1_column + row;
        const float *restrict left2 = left_2_column + row;
        const float *restrict left3 = left_3_column + row;
        const float *restrict right0 = right_0_column + row;
        const float *restrict right1 = right_1_column + row;
        const float *restrict right2 = right_2_column + row;
        const float *restrict right3 = right_3_column + row;
        int n;
        /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
        for (n = 0; n < 4; n++) {
          result[n] = left0[n] * right0[n] + left1[n] * right1[n] + left2[n] * right2[n] + left3[n] * right3[n];
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && ((chain = (*(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        result_column[row] = left_0_column[row] * right_0_column[row] 
                           + left_1_column[row] * right_1_column[row]
                           + left_2_column[row] * right_2_column[row]
                           + left_3_column[row] * right_3_column[row];
        uint8_t delta = chain_column[row];
        if (!delta) goto done;
        row += delta;
      } while (row & 3);
    }
  }
done:;
}

static void sl_exec_split_chains_by_bool(struct sl_execution *exec, struct sl_reg_alloc *cond, uint32_t input_chain, uint32_t *ptrue_chain, uint32_t *pfalse_chain) {
  uint8_t row;
  if (input_chain == SL_EXEC_NO_CHAIN) return;
  row = (uint8_t)input_chain;
  const uint8_t * restrict cond_col = exec->bool_regs_[cond->v_.regs_[0]];
  uint8_t * restrict chain_col = exec->exec_chain_reg_;
  uint32_t true_chain = SL_EXEC_NO_CHAIN, false_chain = SL_EXEC_NO_CHAIN;
  uint8_t true_tail;
  uint8_t false_tail;
  uint8_t delta;
  do {
    delta = chain_col[row];

    if (cond_col[row]) {
      if (true_chain != SL_EXEC_NO_CHAIN) {
        chain_col[true_tail] = row - true_tail;
        true_tail = row;
      }
      else {
        true_chain = row;
        true_tail = row;
      }
    }
    else {
      if (false_chain != SL_EXEC_NO_CHAIN) {
        chain_col[false_tail] = row - false_tail;
        false_tail = row;
      }
      else {
        false_chain = row;
        false_tail = row;
      }
    }

    row += delta;
  } while (delta);

  if (true_chain != SL_EXEC_NO_CHAIN) {
    chain_col[true_tail] = 0;
  }
  if (false_chain != SL_EXEC_NO_CHAIN) {
    chain_col[false_tail] = 0;
  }

  *ptrue_chain = true_chain;
  *pfalse_chain = false_chain;
}

static void sl_exec_move(struct sl_execution *exec, uint8_t row, struct sl_reg_alloc *dst, struct sl_reg_alloc *src) {
  int num_components = 0;
  int n;

  switch (src->kind_) {
    case slrak_array:
    case slrak_struct: {
      size_t index;
      for (index = 0; index < src->v_.comp_.num_elements_; ++index) {
        sl_exec_move(exec, row, dst->v_.comp_.elements_ + index, src->v_.comp_.elements_ + index);
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
        sl_exec_f_move(row, exec->exec_chain_reg_, exec->float_regs_[dst->v_.regs_[n]], exec->float_regs_[src->v_.regs_[n]]);
      }
      break;
    }
    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      switch (src->kind_) {
        case slrak_int:   num_components = 1; break;
        case slrak_ivec2: num_components = 2; break;
        case slrak_ivec3: num_components = 3; break;
        case slrak_ivec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_i_move(row, exec->exec_chain_reg_, exec->int_regs_[dst->v_.regs_[n]], exec->int_regs_[src->v_.regs_[n]]);
      }
      break;
    }
    case slrak_bool:
    case slrak_bvec2:
    case slrak_bvec3:
    case slrak_bvec4: {
      switch (src->kind_) {
        case slrak_bool:  num_components = 1; break;
        case slrak_bvec2: num_components = 2; break;
        case slrak_bvec3: num_components = 3; break;
        case slrak_bvec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_b_move(row, exec->exec_chain_reg_, exec->bool_regs_[dst->v_.regs_[n]], exec->bool_regs_[src->v_.regs_[n]]);
      }
      break;
    }
  }
}

static void sl_exec_init_literal(struct sl_execution *exec, uint8_t row, struct sl_reg_alloc *dst, struct sl_expr_temp *src) {
  int num_components = 0;
  int n;

  switch (src->kind_) {
    case slrak_array:
    case slrak_struct: {
      size_t index;
      for (index = 0; index < src->v_.comp_.num_elements_; ++index) {
        sl_exec_init_literal(exec, row, dst->v_.comp_.elements_ + index, src->v_.comp_.elements_ + index);
      }
      break;
    }
    case slrak_float:
      sl_exec_f_init(row, exec->exec_chain_reg_, exec->float_regs_[dst->v_.regs_[0]], src->v_.f_);
      break;
    case slrak_vec2:
    case slrak_vec3:
    case slrak_vec4: {
      switch (src->kind_) {
        case slrak_vec2:  num_components = 2; break;
        case slrak_vec3:  num_components = 3; break;
        case slrak_vec4:  num_components = 4; break;
        case slrak_mat2:  num_components = 4; break;
        case slrak_mat3:  num_components = 9; break;
        case slrak_mat4:  num_components = 16; break;
          break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_f_init(row, exec->exec_chain_reg_, exec->float_regs_[dst->v_.regs_[n]], src->v_.v_[n]);
      }
      break;
    }
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
        sl_exec_f_init(row, exec->exec_chain_reg_, exec->float_regs_[dst->v_.regs_[n]], src->v_.m_[n]);
      }
      break;
    }
    case slrak_int:
      sl_exec_i_init(row, exec->exec_chain_reg_, exec->int_regs_[dst->v_.regs_[0]], src->v_.i_);
      break;
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      switch (src->kind_) {
        case slrak_ivec2: num_components = 2; break;
        case slrak_ivec3: num_components = 3; break;
        case slrak_ivec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_i_init(row, exec->exec_chain_reg_, exec->int_regs_[dst->v_.regs_[n]], src->v_.iv_[n]);
      }
      break;
    }
    case slrak_bool:
      sl_exec_b_init(row, exec->exec_chain_reg_, exec->bool_regs_[dst->v_.regs_[0]], src->v_.b_);
      break;
    case slrak_bvec2:
    case slrak_bvec3:
    case slrak_bvec4: {
      switch (src->kind_) {
        case slrak_bvec2: num_components = 2; break;
        case slrak_bvec3: num_components = 3; break;
        case slrak_bvec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_b_init(row, exec->exec_chain_reg_, exec->bool_regs_[dst->v_.regs_[n]], src->v_.bv_[n]);
      }
      break;
    }
  }
}

static void sl_exec_mul(struct sl_execution *exec, uint8_t row, struct sl_reg_alloc *dst, struct sl_reg_alloc *left, struct sl_reg_alloc *right) {
  int r, c;
  if (left->kind_ == right->kind_) {
    switch (left->kind_) {
      case slrak_float:
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      exec->float_regs_[dst->v_.regs_[0]], 
                      exec->float_regs_[left->v_.regs_[0]],
                      exec->float_regs_[right->v_.regs_[0]]);
        break;
      case slrak_int:
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      exec->int_regs_[dst->v_.regs_[0]], 
                      exec->int_regs_[left->v_.regs_[0]],
                      exec->int_regs_[right->v_.regs_[0]]);
        break;
      case slrak_vec2:
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      exec->float_regs_[dst->v_.regs_[0]], 
                      exec->float_regs_[left->v_.regs_[0]],
                      exec->float_regs_[right->v_.regs_[0]]);
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      exec->float_regs_[dst->v_.regs_[1]], 
                      exec->float_regs_[left->v_.regs_[1]],
                      exec->float_regs_[right->v_.regs_[1]]);
        break;
      case slrak_vec3:
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      exec->float_regs_[dst->v_.regs_[0]], 
                      exec->float_regs_[left->v_.regs_[0]],
                      exec->float_regs_[right->v_.regs_[0]]);
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      exec->float_regs_[dst->v_.regs_[1]], 
                      exec->float_regs_[left->v_.regs_[1]],
                      exec->float_regs_[right->v_.regs_[1]]);
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      exec->float_regs_[dst->v_.regs_[2]], 
                      exec->float_regs_[left->v_.regs_[2]],
                      exec->float_regs_[right->v_.regs_[2]]);
        break;
      case slrak_vec4:
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      exec->float_regs_[dst->v_.regs_[0]], 
                      exec->float_regs_[left->v_.regs_[0]],
                      exec->float_regs_[right->v_.regs_[0]]);
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      exec->float_regs_[dst->v_.regs_[1]], 
                      exec->float_regs_[left->v_.regs_[1]],
                      exec->float_regs_[right->v_.regs_[1]]);
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      exec->float_regs_[dst->v_.regs_[2]], 
                      exec->float_regs_[left->v_.regs_[2]],
                      exec->float_regs_[right->v_.regs_[2]]);
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      exec->float_regs_[dst->v_.regs_[3]], 
                      exec->float_regs_[left->v_.regs_[3]],
                      exec->float_regs_[right->v_.regs_[3]]);
        break;
      case slrak_ivec2:
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      exec->int_regs_[dst->v_.regs_[0]],
                      exec->int_regs_[left->v_.regs_[0]],
                      exec->int_regs_[right->v_.regs_[0]]);
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      exec->int_regs_[dst->v_.regs_[1]],
                      exec->int_regs_[left->v_.regs_[1]],
                      exec->int_regs_[right->v_.regs_[1]]);
        break;
      case slrak_ivec3:
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      exec->int_regs_[dst->v_.regs_[0]],
                      exec->int_regs_[left->v_.regs_[0]],
                      exec->int_regs_[right->v_.regs_[0]]);
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      exec->int_regs_[dst->v_.regs_[1]],
                      exec->int_regs_[left->v_.regs_[1]],
                      exec->int_regs_[right->v_.regs_[1]]);
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      exec->int_regs_[dst->v_.regs_[2]],
                      exec->int_regs_[left->v_.regs_[2]],
                      exec->int_regs_[right->v_.regs_[2]]);
        break;
      case slrak_ivec4:
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      exec->int_regs_[dst->v_.regs_[0]],
                      exec->int_regs_[left->v_.regs_[0]],
                      exec->int_regs_[right->v_.regs_[0]]);
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      exec->int_regs_[dst->v_.regs_[1]],
                      exec->int_regs_[left->v_.regs_[1]],
                      exec->int_regs_[right->v_.regs_[1]]);
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      exec->int_regs_[dst->v_.regs_[2]],
                      exec->int_regs_[left->v_.regs_[2]],
                      exec->int_regs_[right->v_.regs_[2]]);
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      exec->int_regs_[dst->v_.regs_[3]],
                      exec->int_regs_[left->v_.regs_[3]],
                      exec->int_regs_[right->v_.regs_[3]]);
        break;
      case slrak_mat2:
        for (c = 0; c < 2; ++c) {
          for (r = 0; r < 2; ++r) {
            sl_exec_f_dot_product2(row, exec->exec_chain_reg_,
                                   exec->float_regs_[dst->v_.regs_[c * 2 + r]],
                                   exec->float_regs_[left->v_.regs_[r + 0]], 
                                   exec->float_regs_[left->v_.regs_[r + 2]],
                                   exec->float_regs_[right->v_.regs_[c * 2 + 0]], 
                                   exec->float_regs_[right->v_.regs_[c * 2 + 1]]);
          }
        }
        break;
      case slrak_mat3:
        for (c = 0; c < 3; ++c) {
          for (r = 0; r < 3; ++r) {
            sl_exec_f_dot_product3(row, exec->exec_chain_reg_,
                                   exec->float_regs_[dst->v_.regs_[c * 3 + r]],
                                   exec->float_regs_[left->v_.regs_[r + 0]], 
                                   exec->float_regs_[left->v_.regs_[r + 3]],
                                   exec->float_regs_[left->v_.regs_[r + 6]],
                                   exec->float_regs_[right->v_.regs_[c * 3 + 0]], 
                                   exec->float_regs_[right->v_.regs_[c * 3 + 1]],
                                   exec->float_regs_[right->v_.regs_[c * 3 + 2]]);
          }
        }
        break;
      case slrak_mat4:
        for (c = 0; c < 4; ++c) {
          for (r = 0; r < 4; ++r) {
            sl_exec_f_dot_product4(row, exec->exec_chain_reg_,
                                   exec->float_regs_[dst->v_.regs_[c * 4 + r]],
                                   exec->float_regs_[left->v_.regs_[r + 0]], 
                                   exec->float_regs_[left->v_.regs_[r + 4]],
                                   exec->float_regs_[left->v_.regs_[r + 8]],
                                   exec->float_regs_[left->v_.regs_[r + 12]],
                                   exec->float_regs_[right->v_.regs_[c * 4 + 0]], 
                                   exec->float_regs_[right->v_.regs_[c * 4 + 1]],
                                   exec->float_regs_[right->v_.regs_[c * 4 + 2]],
                                   exec->float_regs_[right->v_.regs_[c * 4 + 3]]);
          }
        }
        break;
    }
  }
  else if (left->kind_ == slrak_float) {
    int num_coords = 0;
    switch (right->kind_) {
      case slrak_vec2: num_coords = 2; break;
      case slrak_vec3: num_coords = 3; break;
      case slrak_vec4: num_coords = 4; break;
      case slrak_mat2: num_coords = 4; break;
      case slrak_mat3: num_coords = 9; break;
      case slrak_mat4: num_coords = 16; break;
    }
    for (r = 0; r < num_coords; ++r) {
      sl_exec_f_mul(row, exec->exec_chain_reg_,
                    exec->float_regs_[dst->v_.regs_[r]], 
                    exec->float_regs_[left->v_.regs_[0]],
                    exec->float_regs_[right->v_.regs_[r]]);
    }
  }
  else if (right->kind_ == slrak_float) {
    int num_coords = 0;
    switch (left->kind_) {
      case slrak_vec2: num_coords = 2; break;
      case slrak_vec3: num_coords = 3; break;
      case slrak_vec4: num_coords = 4; break;
      case slrak_mat2: num_coords = 4; break;
      case slrak_mat3: num_coords = 9; break;
      case slrak_mat4: num_coords = 16; break;
    }
    for (r = 0; r < num_coords; ++r) {
      sl_exec_f_mul(row, exec->exec_chain_reg_,
                    exec->float_regs_[dst->v_.regs_[r]], 
                    exec->float_regs_[left->v_.regs_[r]],
                    exec->float_regs_[right->v_.regs_[0]]);
    }
  }
  else if (left->kind_ == slrak_int) {
    int num_coords = 0;
    switch (right->kind_) {
      case slrak_ivec2: num_coords = 2; break;
      case slrak_ivec3: num_coords = 3; break;
      case slrak_ivec4: num_coords = 4; break;
    }
    for (r = 0; r < num_coords; ++r) {
      sl_exec_i_mul(row, exec->exec_chain_reg_,
                    exec->int_regs_[dst->v_.regs_[r]], 
                    exec->int_regs_[left->v_.regs_[0]],
                    exec->int_regs_[right->v_.regs_[r]]);
    }
  }
  else if (right->kind_ == slrak_int) {
    int num_coords = 0;
    switch (left->kind_) {
      case slrak_ivec2: num_coords = 2; break;
      case slrak_ivec3: num_coords = 3; break;
      case slrak_ivec4: num_coords = 4; break;
    }
    for (r = 0; r < num_coords; ++r) {
      sl_exec_i_mul(row, exec->exec_chain_reg_,
                    exec->int_regs_[dst->v_.regs_[r]],
                    exec->int_regs_[left->v_.regs_[r]],
                    exec->int_regs_[right->v_.regs_[0]]);
    }
  }
  else if ((left->kind_ == slrak_vec2) && (right->kind_ == slrak_mat2)) {
    for (r = 0; r < 2; ++r) {
      sl_exec_f_dot_product2(row, exec->exec_chain_reg_,
                             exec->float_regs_[dst->v_.regs_[r]],
                             exec->float_regs_[left->v_.regs_[0]],
                             exec->float_regs_[left->v_.regs_[1]],
                             exec->float_regs_[right->v_.regs_[0 + r * 2]],
                             exec->float_regs_[right->v_.regs_[1 + r * 2]]);
    }
  }
  else if ((left->kind_ == slrak_mat2) && (right->kind_ == slrak_vec2)) {
    for (r = 0; r < 2; ++r) {
      sl_exec_f_dot_product2(row, exec->exec_chain_reg_,
                             exec->float_regs_[dst->v_.regs_[r]],
                             exec->float_regs_[left->v_.regs_[0 + r]],
                             exec->float_regs_[left->v_.regs_[2 + r]],
                             exec->float_regs_[right->v_.regs_[0]],
                             exec->float_regs_[right->v_.regs_[1]]);
    }
  }
  else if ((left->kind_ == slrak_vec3) && (right->kind_ == slrak_mat3)) {
    for (r = 0; r < 3; ++r) {
      sl_exec_f_dot_product3(row, exec->exec_chain_reg_,
                             exec->float_regs_[dst->v_.regs_[r]],
                             exec->float_regs_[left->v_.regs_[0]],
                             exec->float_regs_[left->v_.regs_[1]],
                             exec->float_regs_[left->v_.regs_[2]],
                             exec->float_regs_[right->v_.regs_[0 + r * 3]],
                             exec->float_regs_[right->v_.regs_[1 + r * 3]],
                             exec->float_regs_[right->v_.regs_[2 + r * 3]]);
    }
  }
  else if ((left->kind_ == slrak_mat3) && (right->kind_ == slrak_vec3)) {
    for (r = 0; r < 3; ++r) {
      sl_exec_f_dot_product3(row, exec->exec_chain_reg_,
                             exec->float_regs_[dst->v_.regs_[r]],
                             exec->float_regs_[left->v_.regs_[0 + r]],
                             exec->float_regs_[left->v_.regs_[3 + r]],
                             exec->float_regs_[left->v_.regs_[6 + r]],
                             exec->float_regs_[right->v_.regs_[0]],
                             exec->float_regs_[right->v_.regs_[1]],
                             exec->float_regs_[right->v_.regs_[2]]);
    }
  }
  else if ((left->kind_ == slrak_vec4) && (right->kind_ == slrak_mat4)) {
    for (r = 0; r < 4; ++r) {
      sl_exec_f_dot_product4(row, exec->exec_chain_reg_,
                             exec->float_regs_[dst->v_.regs_[r]],
                             exec->float_regs_[left->v_.regs_[0]],
                             exec->float_regs_[left->v_.regs_[1]],
                             exec->float_regs_[left->v_.regs_[2]],
                             exec->float_regs_[left->v_.regs_[3]],
                             exec->float_regs_[right->v_.regs_[0 + r * 4]],
                             exec->float_regs_[right->v_.regs_[1 + r * 4]],
                             exec->float_regs_[right->v_.regs_[2 + r * 4]],
                             exec->float_regs_[right->v_.regs_[3 + r * 4]]);
    }
  }
  else if ((left->kind_ == slrak_mat4) && (right->kind_ == slrak_vec4)) {
    for (r = 0; r < 4; ++r) {
      sl_exec_f_dot_product4(row, exec->exec_chain_reg_,
                             exec->float_regs_[dst->v_.regs_[r]],
                             exec->float_regs_[left->v_.regs_[0 + r]],
                             exec->float_regs_[left->v_.regs_[4 + r]],
                             exec->float_regs_[left->v_.regs_[8 + r]],
                             exec->float_regs_[left->v_.regs_[12 + r]],
                             exec->float_regs_[right->v_.regs_[0]],
                             exec->float_regs_[right->v_.regs_[1]],
                             exec->float_regs_[right->v_.regs_[2]],
                             exec->float_regs_[right->v_.regs_[3]]);
    }
  }
}

static void sl_exec_div(struct sl_execution *exec, uint8_t row, struct sl_reg_alloc *dst, struct sl_reg_alloc *left, struct sl_reg_alloc *right) {
  int r;

  if (left->kind_ == right->kind_) {
    switch (left->kind_) {
      case slrak_float:
      case slrak_vec2:
      case slrak_vec3:
      case slrak_vec4:
      case slrak_mat2:
      case slrak_mat3:
      case slrak_mat4: {
        int num_components = 0;
        switch (left->kind_) {
          case slrak_float: num_components = 1; break;
          case slrak_vec2: num_components = 2; break;
          case slrak_vec3: num_components = 3; break;
          case slrak_vec4: num_components = 4; break;
          case slrak_mat2: num_components = 2; break;
          case slrak_mat3: num_components = 3; break;
          case slrak_mat4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_exec_f_div(row, exec->exec_chain_reg_, 
                        exec->float_regs_[dst->v_.regs_[r]],
                        exec->float_regs_[left->v_.regs_[r]],
                        exec->float_regs_[right->v_.regs_[r]]);
        }
        break;
      }
      case slrak_int:
      case slrak_ivec2:
      case slrak_ivec3:
      case slrak_ivec4: {
        int num_components = 0;
        switch (left->kind_) {
          case slrak_int:   num_components = 1; break;
          case slrak_ivec2: num_components = 2; break;
          case slrak_ivec3: num_components = 3; break;
          case slrak_ivec4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_exec_i_div(row, exec->exec_chain_reg_, 
                        exec->int_regs_[dst->v_.regs_[r]],
                        exec->int_regs_[left->v_.regs_[r]],
                        exec->int_regs_[right->v_.regs_[r]]);
        }
        break;
      }
    }
  }
  else if (left->kind_ == slrak_float) {
    int num_components = 0;
    switch (right->kind_) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_f_div(row, exec->exec_chain_reg_, 
                    exec->float_regs_[dst->v_.regs_[r]],
                    exec->float_regs_[left->v_.regs_[0]],
                    exec->float_regs_[right->v_.regs_[r]]);
    }
  }
  else if (right->kind_ == slrak_float) {
    int num_components = 0;
    switch (left->kind_) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_f_div(row, exec->exec_chain_reg_, 
                    exec->float_regs_[dst->v_.regs_[r]],
                    exec->float_regs_[left->v_.regs_[r]],
                    exec->float_regs_[right->v_.regs_[0]]);
    }
  }
  else if (left->kind_ == slrak_int) {
    int num_components = 0;
    switch (right->kind_) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_i_div(row, exec->exec_chain_reg_,
                    exec->int_regs_[dst->v_.regs_[r]],
                    exec->int_regs_[left->v_.regs_[0]],
                    exec->int_regs_[right->v_.regs_[r]]);
    }
  }
  else if (right->kind_ == slrak_int) {
    int num_components = 0;
    switch (left->kind_) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_i_div(row, exec->exec_chain_reg_,
                    exec->int_regs_[dst->v_.regs_[r]],
                    exec->int_regs_[left->v_.regs_[r]],
                    exec->int_regs_[right->v_.regs_[0]]);
    }
  }
}

static void sl_exec_add(struct sl_execution *exec, uint8_t row, struct sl_reg_alloc *dst, struct sl_reg_alloc *left, struct sl_reg_alloc *right) {
  int r;

  if (left->kind_ == right->kind_) {
    switch (left->kind_) {
      case slrak_float:
      case slrak_vec2:
      case slrak_vec3:
      case slrak_vec4:
      case slrak_mat2:
      case slrak_mat3:
      case slrak_mat4: {
        int num_components = 0;
        switch (left->kind_) {
          case slrak_float: num_components = 1; break;
          case slrak_vec2: num_components = 2; break;
          case slrak_vec3: num_components = 3; break;
          case slrak_vec4: num_components = 4; break;
          case slrak_mat2: num_components = 2; break;
          case slrak_mat3: num_components = 3; break;
          case slrak_mat4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_exec_f_add(row, exec->exec_chain_reg_, 
                        exec->float_regs_[dst->v_.regs_[r]],
                        exec->float_regs_[left->v_.regs_[r]],
                        exec->float_regs_[right->v_.regs_[r]]);
        }
        break;
      }
      case slrak_int:
      case slrak_ivec2:
      case slrak_ivec3:
      case slrak_ivec4: {
        int num_components = 0;
        switch (left->kind_) {
          case slrak_int:   num_components = 1; break;
          case slrak_ivec2: num_components = 2; break;
          case slrak_ivec3: num_components = 3; break;
          case slrak_ivec4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_exec_i_add(row, exec->exec_chain_reg_, 
                        exec->int_regs_[dst->v_.regs_[r]],
                        exec->int_regs_[left->v_.regs_[r]],
                        exec->int_regs_[right->v_.regs_[r]]);
        }
        break;
      }
    }
  }
  else if (left->kind_ == slrak_float) {
    int num_components = 0;
    switch (right->kind_) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_f_add(row, exec->exec_chain_reg_, 
                    exec->float_regs_[dst->v_.regs_[r]],
                    exec->float_regs_[left->v_.regs_[0]],
                    exec->float_regs_[right->v_.regs_[r]]);
    }
  }
  else if (right->kind_ == slrak_float) {
    int num_components = 0;
    switch (left->kind_) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_f_add(row, exec->exec_chain_reg_, 
                    exec->float_regs_[dst->v_.regs_[r]],
                    exec->float_regs_[left->v_.regs_[r]],
                    exec->float_regs_[right->v_.regs_[0]]);
    }
  }
  else if (left->kind_ == slrak_int) {
    int num_components = 0;
    switch (right->kind_) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_i_add(row, exec->exec_chain_reg_,
                    exec->int_regs_[dst->v_.regs_[r]],
                    exec->int_regs_[left->v_.regs_[0]],
                    exec->int_regs_[right->v_.regs_[r]]);
    }
  }
  else if (right->kind_ == slrak_int) {
    int num_components = 0;
    switch (left->kind_) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_i_add(row, exec->exec_chain_reg_,
                    exec->int_regs_[dst->v_.regs_[r]],
                    exec->int_regs_[left->v_.regs_[r]],
                    exec->int_regs_[right->v_.regs_[0]]);
    }
  }
}

static void sl_exec_sub(struct sl_execution *exec, uint8_t row, struct sl_reg_alloc *dst, struct sl_reg_alloc *left, struct sl_reg_alloc *right) {
  int r;

  if (left->kind_ == right->kind_) {
    switch (left->kind_) {
      case slrak_float:
      case slrak_vec2:
      case slrak_vec3:
      case slrak_vec4:
      case slrak_mat2:
      case slrak_mat3:
      case slrak_mat4: {
        int num_components = 0;
        switch (left->kind_) {
          case slrak_float: num_components = 1; break;
          case slrak_vec2: num_components = 2; break;
          case slrak_vec3: num_components = 3; break;
          case slrak_vec4: num_components = 4; break;
          case slrak_mat2: num_components = 2; break;
          case slrak_mat3: num_components = 3; break;
          case slrak_mat4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_exec_f_sub(row, exec->exec_chain_reg_, 
                        exec->float_regs_[dst->v_.regs_[r]],
                        exec->float_regs_[left->v_.regs_[r]],
                        exec->float_regs_[right->v_.regs_[r]]);
        }
        break;
      }
      case slrak_int:
      case slrak_ivec2:
      case slrak_ivec3:
      case slrak_ivec4: {
        int num_components = 0;
        switch (left->kind_) {
          case slrak_int:   num_components = 1; break;
          case slrak_ivec2: num_components = 2; break;
          case slrak_ivec3: num_components = 3; break;
          case slrak_ivec4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_exec_i_sub(row, exec->exec_chain_reg_, 
                        exec->int_regs_[dst->v_.regs_[r]],
                        exec->int_regs_[left->v_.regs_[r]],
                        exec->int_regs_[right->v_.regs_[r]]);
        }
        break;
      }
    }
  }
  else if (left->kind_ == slrak_float) {
    int num_components = 0;
    switch (right->kind_) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_f_sub(row, exec->exec_chain_reg_, 
                    exec->float_regs_[dst->v_.regs_[r]],
                    exec->float_regs_[left->v_.regs_[0]],
                    exec->float_regs_[right->v_.regs_[r]]);
    }
  }
  else if (right->kind_ == slrak_float) {
    int num_components = 0;
    switch (left->kind_) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_f_sub(row, exec->exec_chain_reg_, 
                    exec->float_regs_[dst->v_.regs_[r]],
                    exec->float_regs_[left->v_.regs_[r]],
                    exec->float_regs_[right->v_.regs_[0]]);
    }
  }
  else if (left->kind_ == slrak_int) {
    int num_components = 0;
    switch (right->kind_) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_i_sub(row, exec->exec_chain_reg_,
                    exec->int_regs_[dst->v_.regs_[r]],
                    exec->int_regs_[left->v_.regs_[0]],
                    exec->int_regs_[right->v_.regs_[r]]);
    }
  }
  else if (right->kind_ == slrak_int) {
    int num_components = 0;
    switch (left->kind_) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_i_sub(row, exec->exec_chain_reg_,
                    exec->int_regs_[dst->v_.regs_[r]],
                    exec->int_regs_[left->v_.regs_[r]],
                    exec->int_regs_[right->v_.regs_[0]]);
    }
  }
}


static int sl_exec_reserve_n(struct sl_execution *exec, size_t n) {
  size_t new_num_needed = exec->num_execution_points_ + n;
  if (new_num_needed > exec->num_execution_points_allocated_) {
    size_t new_num_allocated = exec->num_execution_points_allocated_;
    /* Keep incrementing size until we have it, checking for overflow along the way */
    do {
      size_t new_new_num_allocated = new_num_allocated * 2 + 1;
      if (new_new_num_allocated <= new_num_allocated) {
        /* overflow */
        return -1;
      }
      new_num_allocated = new_new_num_allocated;
      if (new_num_allocated == 1) {
        new_num_allocated = 15;
      }
      if (new_num_allocated >= (SIZE_MAX / sizeof(struct sl_execution_point))) {
        /* overflow */
        return -1;
      }
    } while (new_num_allocated < new_num_needed);
    size_t new_size = new_num_allocated * sizeof(struct sl_execution_point);

    struct sl_execution_point *new_execution_points = (struct sl_execution_point *)realloc(exec->execution_points_, new_size);
    if (!new_execution_points) {
      /* no memory */
      return -1;
    }
    exec->execution_points_ = new_execution_points;
    exec->num_execution_points_allocated_ = new_num_allocated;
  }
  return 0;
}

static void sl_exec_clear_ep(struct sl_execution *exec, size_t ep_index) {
  struct sl_execution_point *ep = exec->execution_points_ + ep_index;
  ep->kind_ = SLEPK_NONE;
  memset(&ep->v_, 0, sizeof(ep->v_));
  ep->enter_chain_ = ep->revisit_chain_ = ep->post_chain_ = ep->alt_chain_ = SL_EXEC_NO_CHAIN;
  ep->continue_chain_ptr_ = 0;
}

void sl_exec_init(struct sl_execution *exec) {
  exec->num_execution_points_ = exec->num_execution_points_allocated_ = 0;
  exec->execution_points_ = NULL;
  exec->num_float_regs_ = 0;
  exec->float_regs_ = NULL;
  exec->num_int_regs_ = 0;
  exec->int_regs_ = NULL;
  exec->num_bool_regs_ = 0;
  exec->bool_regs_ = NULL;
}

void sl_exec_cleanup(struct sl_execution *exec) {
  if (exec->execution_points_) free(exec->execution_points_);
  if (exec->float_regs_) free(exec->float_regs_);
  if (exec->int_regs_) free(exec->int_regs_);
  if (exec->bool_regs_) free(exec->bool_regs_);
}

int sl_exec_prep(struct sl_execution *exec, struct sl_reg_allocator *rac) {
  void *new_float_regs = NULL;
  void *new_int_regs = NULL;
  void *new_bool_regs = NULL;
  void *new_sampler2D_regs = NULL;
  void *new_samplerCube_regs = NULL;
  if (rac->current_max_float_reg_) {
    new_float_regs = malloc(sizeof(float *) * rac->current_max_float_reg_);
    if (!new_float_regs) goto fail;
    memset(new_float_regs, 0, sizeof(float *) * rac->current_max_float_reg_);
  }
  if (rac->current_max_int_reg_) {
    new_int_regs = malloc(sizeof(int64_t *) * rac->current_max_int_reg_);
    if (!new_int_regs) goto fail;
    memset(new_int_regs, 0, sizeof(int64_t *) * rac->current_max_int_reg_);
  }
  if (rac->current_max_bool_reg_) {
    new_bool_regs = malloc(sizeof(unsigned char *) * rac->current_max_bool_reg_);
    if (!new_bool_regs) goto fail;
    memset(new_bool_regs, 0, sizeof(int *) * rac->current_max_bool_reg_);
  }
  if (rac->current_max_sampler2D_reg_) {
    new_sampler2D_regs = malloc(sizeof(void *) * rac->current_max_sampler2D_reg_);
    if (!new_sampler2D_regs) goto fail;
    memset(new_sampler2D_regs, 0, sizeof(void *) * rac->current_max_sampler2D_reg_);
  }
  if (rac->current_max_samplerCube_reg_) {
    new_samplerCube_regs = malloc(sizeof(void *) * rac->current_max_samplerCube_reg_);
    if (!new_samplerCube_regs) goto fail;
    memset(new_samplerCube_regs, 0, sizeof(void *) * rac->current_max_samplerCube_reg_);
  }

  if (exec->float_regs_) free(exec->float_regs_);
  if (exec->int_regs_) free(exec->int_regs_);
  if (exec->bool_regs_) free(exec->bool_regs_);
  if (exec->sampler_2D_regs_) free(exec->sampler_2D_regs_);
  if (exec->sampler_cube_regs_) free(exec->sampler_cube_regs_);

  exec->num_float_regs_ = (size_t)rac->current_max_float_reg_;
  exec->float_regs_ = (float **)new_float_regs;

  exec->num_int_regs_ = (size_t)rac->current_max_int_reg_;
  exec->int_regs_ = (int64_t **)new_int_regs;

  exec->num_bool_regs_ = (size_t)rac->current_max_bool_reg_;
  exec->bool_regs_ = (unsigned char **)new_bool_regs;

  exec->num_sampler_2D_regs_ = (size_t)rac->current_max_sampler2D_reg_;
  exec->sampler_2D_regs_ = (void **)new_sampler2D_regs;

  exec->num_sampler_cube_regs_ = (size_t)rac->current_max_samplerCube_reg_;
  exec->sampler_cube_regs_ = (void **)new_samplerCube_regs;

  return 0;
fail:
  if (new_float_regs) free(new_float_regs);
  if (new_int_regs) free(new_int_regs);
  if (new_bool_regs) free(new_bool_regs);
  if (new_sampler2D_regs) free(new_sampler2D_regs);
  if (new_samplerCube_regs) free(new_samplerCube_regs);

  return -1;
}

void sl_exec_set_expr(struct sl_execution *exec, size_t ep_index, struct sl_expr *expr, uint32_t chain, size_t continuation_ptr) {
  sl_exec_clear_ep(exec, ep_index);
  struct sl_execution_point *ep = exec->execution_points_ + ep_index;
  ep->kind_ = SLEPK_EXPR;
  ep->v_.expr_ = expr;
  ep->enter_chain_ = chain;
  ep->continue_chain_ptr_ = continuation_ptr;
}

int sl_exec_push_expr(struct sl_execution *exec, struct sl_expr *expr, uint32_t chain, size_t continuation_ptr) {
  int r;
  r = sl_exec_reserve_n(exec, 1);
  if (r) return r;
  
  sl_exec_set_expr(exec, exec->num_execution_points_++, expr, chain, continuation_ptr);

  return 0;
}

void sl_exec_pop_ep(struct sl_execution *exec) {
  assert(exec->num_execution_points_);
  exec->num_execution_points_--;
}

uint32_t sl_exec_join_chains(struct sl_execution *exec, uint32_t a, uint32_t b) {
  /* Join chain starting at a and chain starting at b, into a single chain, the start
   * of which is returned. This is like a merge sort in that we need to maintain
   * the relative order.
   * Sentinel value inside the execution chain is 0, however, this cannot be used
   * to indicate an empty list as row 0 is a valid row. Consequently, to indicate
   * "no list", a or b should be set to SL_EXEC_NO_CHAIN. This is also why their
   * type is uint32_t. */
  uint8_t *ec = exec->exec_chain_reg_;

  uint32_t r;
  if (a == SL_EXEC_NO_CHAIN) return b;
  if (b == SL_EXEC_NO_CHAIN) return a;

  if (a < b) {
    r = a;
    uint8_t step = ec[a];
    if (step) {
      a += step;
    }
    else {
      a = SL_EXEC_NO_CHAIN;
    }
  }
  else if (a > b) {
    r = b;
    uint8_t step = ec[b];
    if (step) {
      b += step;
    }
    else {
      b = SL_EXEC_NO_CHAIN;
    }
  }
  else /* (a == b) */ {
    /* No merging necessary when they're the same chain */
    return a;
  }

  uint32_t q;
  q = r;
  for (;;) {
    if (a < b) {
      ec[q] = (uint8_t)(a - q);
      q = a;
      uint8_t step = ec[a];
      if (step) a += step;
      else a = SL_EXEC_NO_CHAIN;
    }
    else if (a > b) {
      ec[q] = (uint8_t)(b - q);
      q = b;
      uint8_t step = ec[b];
      if (step) b += step;
      else b = SL_EXEC_NO_CHAIN;
    }
    else /* (a == b) */ {
      /* This happens when both a and b are SL_EXEC_NO_CHAIN, it could also happen
       * when a or b are part of the same list (in which case the earlier one
       * will catch up to the later one until they are identical) - that latter
       * case means there's no further point in merging (as they're already 
       * merged. */
      
      /* No need to terminate ec[q] as it is guaranteed to be the tail of one or
       * the other list (and so is already 0) */
      return q;
    }
  }
}


int sl_exec_run(struct sl_execution *exec) {
  int r;
  while (exec->num_execution_points_) {
    /* Build "eps[epi]" shorthand for exec->execution_points_[exec->num_execution_points_ - 1] as we'll be referencing it, a lot. */
    size_t epi = exec->num_execution_points_;
    struct sl_execution_point *eps = exec->execution_points_;
#define CHAIN_REF(x) (((uintptr_t)&(x)) - (uintptr_t)exec->execution_points_)
    if (eps[epi].kind_ == SLEPK_STMT) {
      switch (eps[epi].v_.stmt_->kind_) {
        case slsk_expression: {
          if (eps[epi].enter_chain_ != SL_EXEC_NO_CHAIN) {
            r = sl_exec_push_expr(exec, eps[epi].v_.stmt_->expr_, eps[epi].enter_chain_, CHAIN_REF(eps[epi].post_chain_));
            if (r) return r;
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            break;  
          }
          break;
        }
        case slsk_if:
          break;
        case slsk_while:
          break;
        case slsk_do:
          break;
        case slsk_for:
          break;
        case slsk_continue:
          break;
        case slsk_break:
          break;
        case slsk_return:
          break;
        case slsk_discard:
          break;
        case slsk_compound:
          break;
      }
    }
    else if (eps[epi].kind_ == SLEPK_EXPR) {
      if (eps[epi].enter_chain_ != SL_EXEC_NO_CHAIN) {
        switch (eps[epi].v_.expr_->op_) {
          case exop_invalid: {
            /* Can't do anything when this happens, treat as no-op */
            assert(0);
            break;
          }
          case exop_variable: {
            /* The value is already in the appropriate registers; do the move anyway, as, though
             * it will turn into a no-op, it reduces coupling. */
            sl_exec_move(exec, eps[epi].enter_chain_, &eps[epi].v_.expr_->reg_alloc_, &eps[epi].v_.expr_->variable_->reg_alloc_);
            break;
          }
          case exop_literal: {
            /* Load constant value into the register */
            sl_exec_init_literal(exec, eps[epi].enter_chain_, &eps[epi].v_.expr_->reg_alloc_, &eps[epi].v_.expr_->literal_value_);
            break;
          }
          case exop_array_subscript:
          case exop_component_selection:
          case exop_field_selection:
          case exop_post_inc:
          case exop_post_dec:
          case exop_pre_inc:
          case exop_pre_dec:

          case exop_negate:
          case exop_logical_not: {
            break;
          }

          case exop_multiply:
          case exop_divide:
          case exop_add:
          case exop_subtract:
          case exop_lt:
          case exop_le:
          case exop_ge:
          case exop_gt:
          case exop_eq:
          case exop_ne:
          case exop_logical_xor: {
            /* Evaluate both branches first, then come back to evaluate this expression node on the revisit_chain_.. */

            /* Push 2nd child first, it will be evaluated last (LIFO) */
            sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[1], SL_EXEC_NO_CHAIN, CHAIN_REF(eps[epi].revisit_chain_));
            /* Now push the first child, its continuation is the second child's evaluation */
            sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[0], eps[epi].enter_chain_, CHAIN_REF(exec->execution_points_[exec->num_execution_points_-1].enter_chain_));
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            break;
          }

          case exop_function_call:
          case exop_constructor:
            break;

          /* Logical AND/OR evaluation: these should evalue the left branch first and, depending on the result, optionally evalue the right - sequence points! */
          case exop_logical_and:
          case exop_logical_or: {
            /* Now push the first child, its continuation returns here for a revisit */
            sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[0], eps[epi].enter_chain_, CHAIN_REF(eps[epi].revisit_chain_));
            break;
          }

          case exop_assign:
          case exop_mul_assign:
          case exop_div_assign:
          case exop_add_assign:
          case exop_sub_assign:
            break;

          case exop_sequence: {
            /* The sequence is implied in the evaluation of its children */
            size_t our_continue_ptr = eps[epi].continue_chain_ptr_;
            struct sl_expr *seq_expr = eps[epi].v_.expr_;
            sl_exec_pop_ep(exec); /* pop exop_sequence, we're realizing it on the stack */

            /* Push the 2nd child on the stack first (LIFO); its continuation is our (exop_sequence) continuation */
            sl_exec_push_expr(exec, seq_expr->children_[1], SL_EXEC_NO_CHAIN, our_continue_ptr);
            /* Now push the 1st child on the stack, to be evaluated first; its continuation is our second child. */
            sl_exec_push_expr(exec, seq_expr->children_[0], eps[epi].enter_chain_, CHAIN_REF(exec->execution_points_[exec->num_execution_points_-1].enter_chain_));
            
            break;
          }

          case exop_conditional: {
            /* Conditional evaluation: evaluate the first child first and, depending 
             * on the result, evaluate either the second or the third */
            sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[0], eps[epi].enter_chain_, CHAIN_REF(eps[epi].revisit_chain_));
            break;
          }
        }
      }
      else if (eps[epi].revisit_chain_ != SL_EXEC_NO_CHAIN) {
        int dont_pop = 0;
        switch (eps[epi].v_.expr_->op_) {
          case exop_multiply: {
            sl_exec_mul(exec, eps[epi].revisit_chain_, &eps[epi].v_.expr_->reg_alloc_,
                        &eps[epi].v_.expr_->children_[0]->reg_alloc_,
                        &eps[epi].v_.expr_->children_[1]->reg_alloc_);
            break;
          }
          case exop_divide: {
            sl_exec_div(exec, eps[epi].revisit_chain_, &eps[epi].v_.expr_->reg_alloc_,
                        &eps[epi].v_.expr_->children_[0]->reg_alloc_,
                        &eps[epi].v_.expr_->children_[1]->reg_alloc_);
            break;
          }

          case exop_add: {
            sl_exec_add(exec, eps[epi].revisit_chain_, &eps[epi].v_.expr_->reg_alloc_,
                        &eps[epi].v_.expr_->children_[0]->reg_alloc_,
                        &eps[epi].v_.expr_->children_[1]->reg_alloc_);
            break;
          }

          case exop_subtract: {
            sl_exec_sub(exec, eps[epi].revisit_chain_, &eps[epi].v_.expr_->reg_alloc_,
                        &eps[epi].v_.expr_->children_[0]->reg_alloc_,
                        &eps[epi].v_.expr_->children_[1]->reg_alloc_);
            break;
          }

          case exop_lt: {
            if (eps[epi].v_.expr_->children_[0]->reg_alloc_.kind_ == slrak_float) {
              sl_exec_f_lt((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           exec->bool_regs_[eps[epi].v_.expr_->reg_alloc_.v_.regs_[0]],
                           exec->float_regs_[eps[epi].v_.expr_->children_[0]->reg_alloc_.v_.regs_[0]],
                           exec->float_regs_[eps[epi].v_.expr_->children_[1]->reg_alloc_.v_.regs_[0]]);
            }
            else if (eps[epi].v_.expr_->children_[0]->reg_alloc_.kind_ == slrak_int) {
              sl_exec_i_lt((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           exec->bool_regs_[eps[epi].v_.expr_->reg_alloc_.v_.regs_[0]],
                           exec->int_regs_[eps[epi].v_.expr_->children_[0]->reg_alloc_.v_.regs_[0]],
                           exec->int_regs_[eps[epi].v_.expr_->children_[1]->reg_alloc_.v_.regs_[0]]);
            }
            break;
          }
          case exop_le: {
            if (eps[epi].v_.expr_->children_[0]->reg_alloc_.kind_ == slrak_float) {
              sl_exec_f_le((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           exec->bool_regs_[eps[epi].v_.expr_->reg_alloc_.v_.regs_[0]],
                           exec->float_regs_[eps[epi].v_.expr_->children_[0]->reg_alloc_.v_.regs_[0]],
                           exec->float_regs_[eps[epi].v_.expr_->children_[1]->reg_alloc_.v_.regs_[0]]);
            }
            else if (eps[epi].v_.expr_->children_[0]->reg_alloc_.kind_ == slrak_int) {
              sl_exec_i_le((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           exec->bool_regs_[eps[epi].v_.expr_->reg_alloc_.v_.regs_[0]],
                           exec->int_regs_[eps[epi].v_.expr_->children_[0]->reg_alloc_.v_.regs_[0]],
                           exec->int_regs_[eps[epi].v_.expr_->children_[1]->reg_alloc_.v_.regs_[0]]);
            }
            break;
          }
          case exop_ge: {
            if (eps[epi].v_.expr_->children_[0]->reg_alloc_.kind_ == slrak_float) {
              sl_exec_f_ge((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           exec->bool_regs_[eps[epi].v_.expr_->reg_alloc_.v_.regs_[0]],
                           exec->float_regs_[eps[epi].v_.expr_->children_[0]->reg_alloc_.v_.regs_[0]],
                           exec->float_regs_[eps[epi].v_.expr_->children_[1]->reg_alloc_.v_.regs_[0]]);
            }
            else if (eps[epi].v_.expr_->children_[0]->reg_alloc_.kind_ == slrak_int) {
              sl_exec_i_ge((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           exec->bool_regs_[eps[epi].v_.expr_->reg_alloc_.v_.regs_[0]],
                           exec->int_regs_[eps[epi].v_.expr_->children_[0]->reg_alloc_.v_.regs_[0]],
                           exec->int_regs_[eps[epi].v_.expr_->children_[1]->reg_alloc_.v_.regs_[0]]);
            }
            break;
          }
          case exop_gt: {
            if (eps[epi].v_.expr_->children_[0]->reg_alloc_.kind_ == slrak_float) {
              sl_exec_f_gt((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           exec->bool_regs_[eps[epi].v_.expr_->reg_alloc_.v_.regs_[0]],
                           exec->float_regs_[eps[epi].v_.expr_->children_[0]->reg_alloc_.v_.regs_[0]],
                           exec->float_regs_[eps[epi].v_.expr_->children_[1]->reg_alloc_.v_.regs_[0]]);
            }
            else if (eps[epi].v_.expr_->children_[0]->reg_alloc_.kind_ == slrak_int) {
              sl_exec_i_gt((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           exec->bool_regs_[eps[epi].v_.expr_->reg_alloc_.v_.regs_[0]],
                           exec->int_regs_[eps[epi].v_.expr_->children_[0]->reg_alloc_.v_.regs_[0]],
                           exec->int_regs_[eps[epi].v_.expr_->children_[1]->reg_alloc_.v_.regs_[0]]);
            }
            break;
          }

          case exop_eq: {
            if (eps[epi].v_.expr_->children_[0]->reg_alloc_.kind_ == slrak_float) {
              sl_exec_f_eq((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           exec->bool_regs_[eps[epi].v_.expr_->reg_alloc_.v_.regs_[0]],
                           exec->float_regs_[eps[epi].v_.expr_->children_[0]->reg_alloc_.v_.regs_[0]],
                           exec->float_regs_[eps[epi].v_.expr_->children_[1]->reg_alloc_.v_.regs_[0]]);
            }
            else if (eps[epi].v_.expr_->children_[0]->reg_alloc_.kind_ == slrak_int) {
              sl_exec_i_eq((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           exec->bool_regs_[eps[epi].v_.expr_->reg_alloc_.v_.regs_[0]],
                           exec->int_regs_[eps[epi].v_.expr_->children_[0]->reg_alloc_.v_.regs_[0]],
                           exec->int_regs_[eps[epi].v_.expr_->children_[1]->reg_alloc_.v_.regs_[0]]);
            }
            break;
          }
          case exop_ne: {
            if (eps[epi].v_.expr_->children_[0]->reg_alloc_.kind_ == slrak_float) {
              sl_exec_f_ne((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           exec->bool_regs_[eps[epi].v_.expr_->reg_alloc_.v_.regs_[0]],
                           exec->float_regs_[eps[epi].v_.expr_->children_[0]->reg_alloc_.v_.regs_[0]],
                           exec->float_regs_[eps[epi].v_.expr_->children_[1]->reg_alloc_.v_.regs_[0]]);
            }
            else if (eps[epi].v_.expr_->children_[0]->reg_alloc_.kind_ == slrak_int) {
              sl_exec_i_ne((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           exec->bool_regs_[eps[epi].v_.expr_->reg_alloc_.v_.regs_[0]],
                           exec->int_regs_[eps[epi].v_.expr_->children_[0]->reg_alloc_.v_.regs_[0]],
                           exec->int_regs_[eps[epi].v_.expr_->children_[1]->reg_alloc_.v_.regs_[0]]);
            }
            break;
          }

          case exop_logical_xor: {
            sl_exec_logical_or((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                exec->bool_regs_[eps[epi].v_.expr_->reg_alloc_.v_.regs_[0]], 
                                exec->bool_regs_[eps[epi].v_.expr_->children_[0]->reg_alloc_.v_.regs_[0]],
                                exec->bool_regs_[eps[epi].v_.expr_->children_[1]->reg_alloc_.v_.regs_[0]]);
            break;
          }

          case exop_logical_and: {
            /* We get here when child 0 has been evaluated, make a selection and evaluate for child 1,
             * false child 0 means the overal result is false and we can return, true child 0 means the
             * result for child 1 determines the outcome. */
            uint32_t true_chain = SL_EXEC_NO_CHAIN;
            uint32_t false_chain = SL_EXEC_NO_CHAIN;
            sl_exec_split_chains_by_bool(exec, &eps[epi].v_.expr_->children_[0]->reg_alloc_, eps[epi].revisit_chain_, 
                                         &true_chain, &false_chain);
            eps[epi].revisit_chain_ = SL_EXEC_NO_CHAIN;
            /* Child 0 == false ? Move child 0 result to our result and pass on to continuation. */
            sl_exec_move(exec, false_chain, &eps[epi].v_.expr_->reg_alloc_, &eps[epi].v_.expr_->children_[0]->reg_alloc_);
            uint32_t *chain_ptr = (uint32_t *)(((uintptr_t)exec->execution_points_) + eps[epi].continue_chain_ptr_);
            *chain_ptr = sl_exec_join_chains(exec, *chain_ptr, false_chain);
            
            /* Child 0 == true ? Move to process child 1 as the result, we return here in post_chain_ to move the result
             * in to the appropriat register */
            dont_pop = 1;
              
            sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[1], true_chain, CHAIN_REF(eps[epi].post_chain_));

            break;
          }

          case exop_logical_or: {
            /* Like the logical-and, child 0 has been evaluated. If child 0 is true, the result is true and
             * the second child must not be evaluated. Conversely, if child 0 is false, the result is the
             * evaluation of the second branch */
            uint32_t true_chain = SL_EXEC_NO_CHAIN;
            uint32_t false_chain = SL_EXEC_NO_CHAIN;
            sl_exec_split_chains_by_bool(exec, &eps[epi].v_.expr_->children_[0]->reg_alloc_, eps[epi].revisit_chain_,
                                         &true_chain, &false_chain);
            eps[epi].revisit_chain_ = SL_EXEC_NO_CHAIN;
            /* Child 0 == true ? Move child 0 result to our result and pass on to continuation */
            sl_exec_move(exec, true_chain, &eps[epi].v_.expr_->reg_alloc_, &eps[epi].v_.expr_->children_[0]->reg_alloc_);
            uint32_t *chain_ptr = (uint32_t *)(((uintptr_t)exec->execution_points_) + eps[epi].continue_chain_ptr_);
            *chain_ptr = sl_exec_join_chains(exec, *chain_ptr, true_chain);

            /* Child 0 == False ? Move to process child 1 as the result and return here in post_chain to move it in
             * position. */
            dont_pop = 1;
            sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[1], false_chain, CHAIN_REF(eps[epi].post_chain_));

            break;
          }

          case exop_conditional: {
            /* Split execution into a true and false chain, then evaluate children selectively accordingly. */
            /* XXX: How do we return here? In particular, we'd like to move the registers from the true_chain,
             *      and then separately move the registers from the false_chain (because they're moving from 
             *      different registers respectively, potentially...) */
            uint32_t true_chain = SL_EXEC_NO_CHAIN;
            uint32_t false_chain = SL_EXEC_NO_CHAIN;
            sl_exec_split_chains_by_bool(exec, &eps[epi].v_.expr_->children_[0]->reg_alloc_, eps[epi].revisit_chain_,
                                         &true_chain, &false_chain);
            eps[epi].revisit_chain_ = SL_EXEC_NO_CHAIN;
            dont_pop = 1;
            sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[1], true_chain, CHAIN_REF(eps[epi].post_chain_));
            sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[2], false_chain, CHAIN_REF(eps[epi].alt_chain_));
            break;
          }
        }

        if (!dont_pop) {
          uint32_t *continuation_ep = (uint32_t *)(((char *)exec->execution_points_) + eps[epi].continue_chain_ptr_);
          *continuation_ep = sl_exec_join_chains(exec, *continuation_ep, eps[epi].revisit_chain_);
          eps[epi].revisit_chain_ = SL_EXEC_NO_CHAIN;

          sl_exec_pop_ep(exec);
        }
      }
      else if (eps[epi].post_chain_ != SL_EXEC_NO_CHAIN) {
        switch (eps[epi].v_.expr_->op_) {
          case exop_logical_or:
          case exop_logical_and: {
            /* Move result from second child into result of logical-and or logical-or expression node */
            sl_exec_move(exec, eps[epi].post_chain_, &eps[epi].v_.expr_->reg_alloc_, &eps[epi].v_.expr_->children_[1]->reg_alloc_);
            break;
          }
        }
        uint32_t *continuation_ep = (uint32_t *)(((char *)exec->execution_points_) + eps[epi].continue_chain_ptr_);
        *continuation_ep = sl_exec_join_chains(exec, *continuation_ep, eps[epi].post_chain_);
        eps[epi].post_chain_ = SL_EXEC_NO_CHAIN;

        sl_exec_pop_ep(exec);
      }
      else if (eps[epi].alt_chain_ != SL_EXEC_NO_CHAIN) {
        switch (eps[epi].v_.expr_->op_) {
          case exop_conditional: {
            /* Handle completion of the false branch's execution */
            sl_exec_move(exec, eps[epi].alt_chain_, &eps[epi].v_.expr_->reg_alloc_, &eps[epi].v_.expr_->children_[2]->reg_alloc_);
            break;
          }
        }
        uint32_t *continuation_ep = (uint32_t *)(((char *)exec->execution_points_) + eps[epi].continue_chain_ptr_);
        *continuation_ep = sl_exec_join_chains(exec, *continuation_ep, eps[epi].alt_chain_);
        eps[epi].alt_chain_ = SL_EXEC_NO_CHAIN;

        sl_exec_pop_ep(exec);
      }
      else {
        sl_exec_pop_ep(exec);
      }
    }
  }

  return 0;
}
