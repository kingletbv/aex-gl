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

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

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

#ifndef INTTYPES_H_INCLUDED
#define INTTYPES_H_INCLUDED
#include <inttypes.h>
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

#ifndef SL_COMPILATION_UNIT_H_INCLUDED
#define SL_COMPILATION_UNIT_H_INCLUDED
#include "sl_compilation_unit.h"
#endif

#ifndef SL_REG_MOVE_H_INCLUDED
#define SL_REG_MOVE_H_INCLUDED
#include "sl_reg_move.h"
#endif

static int sl_exec_push_execution_frame(struct sl_execution *exec);

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

void sl_exec_i_to_f_move(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const int64_t *restrict opd_column) {
#define UNOP_SNIPPET_OPERATOR(opd) ((float)opd)
#define UNOP_SNIPPET_RESULT_TYPE float
#define UNOP_SNIPPET_TYPE int64_t
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_RESULT_TYPE
#undef UNOP_SNIPPET_TYPE
}

void sl_exec_b_to_f_move(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const uint8_t *restrict opd_column) {
#define UNOP_SNIPPET_OPERATOR(opd) ((float)!!opd)
#define UNOP_SNIPPET_RESULT_TYPE float
#define UNOP_SNIPPET_TYPE uint8_t
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_RESULT_TYPE
#undef UNOP_SNIPPET_TYPE
}

void sl_exec_f_to_i_move(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, const float *restrict opd_column) {
#define UNOP_SNIPPET_OPERATOR(opd) ((int64_t)opd)
#define UNOP_SNIPPET_RESULT_TYPE int64_t
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_RESULT_TYPE
#undef UNOP_SNIPPET_TYPE
}

void sl_exec_b_to_i_move(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, const uint8_t *restrict opd_column) {
#define UNOP_SNIPPET_OPERATOR(opd) ((int64_t)opd)
#define UNOP_SNIPPET_RESULT_TYPE int64_t
#define UNOP_SNIPPET_TYPE uint8_t
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_RESULT_TYPE
#undef UNOP_SNIPPET_TYPE
}

void sl_exec_f_to_b_move(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const float *restrict opd_column) {
#define UNOP_SNIPPET_OPERATOR(opd) ((uint8_t)(opd != 0.f))
#define UNOP_SNIPPET_RESULT_TYPE uint8_t
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_RESULT_TYPE
#undef UNOP_SNIPPET_TYPE
}

void sl_exec_i_to_b_move(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const int64_t *restrict opd_column) {
#define UNOP_SNIPPET_OPERATOR(opd) ((uint8_t)(!!opd))
#define UNOP_SNIPPET_RESULT_TYPE uint8_t
#define UNOP_SNIPPET_TYPE int64_t
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_RESULT_TYPE
#undef UNOP_SNIPPET_TYPE
}


void sl_exec_f_increment(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const float *restrict opd_column) {
#define UNOP_SNIPPET_OPERATOR(opd) (opd + 1.f)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void sl_exec_i_increment(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, const int64_t *restrict opd_column) {
#define UNOP_SNIPPET_OPERATOR(opd) (opd + 1)
#define UNOP_SNIPPET_TYPE int64_t
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void sl_exec_f_decrement(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const float *restrict opd_column) {
#define UNOP_SNIPPET_OPERATOR(opd) (opd - 1.f)
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void sl_exec_i_decrement(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, const int64_t *restrict opd_column) {
#define UNOP_SNIPPET_OPERATOR(opd) (opd - 1)
#define UNOP_SNIPPET_TYPE int64_t
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void sl_exec_b_logical_not(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const uint8_t *restrict opd_column) {
#define UNOP_SNIPPET_OPERATOR(opd) !opd
#define UNOP_SNIPPET_TYPE uint8_t
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void sl_exec_f_negate(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const float *restrict opd_column) {
#define UNOP_SNIPPET_OPERATOR(opd) -opd
#define UNOP_SNIPPET_TYPE float
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void sl_exec_i_negate(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, const int64_t *restrict opd_column) {
#define UNOP_SNIPPET_OPERATOR(opd) -opd
#define UNOP_SNIPPET_TYPE int64_t
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
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

void sl_exec_i_mul_constant(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, const int64_t *restrict opd_column, int64_t constant_val) {
#define UNOP_SNIPPET_OPERATOR(opd) (opd * constant_val)
#define UNOP_SNIPPET_TYPE int64_t
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
}

void sl_exec_i_mul_constant_and_add(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, const int64_t *restrict left_column, const int64_t *restrict right_column, int64_t constant_val) {
#define BINOP_SNIPPET_OPERATOR(left, right) left * constant_val + right
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
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
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
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
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
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
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

void sl_exec_p_move(uint8_t row, uint8_t *restrict chain_column, void **restrict result_column, void * const *restrict src_column) {
  if (result_column == src_column) return;
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        void **restrict result = result_column + row;
        void * const *restrict src = src_column + row;
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
        void **restrict result = result_column + row;
        void * const *restrict src = src_column + row;
        int n;
        for (n = 0; n < 4; n++) {
          result[n] = src[n];
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
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

void sl_exec_f_indir_load(uint8_t row, uint8_t *restrict chain_column, float *restrict result_column, const int64_t *restrict src_column, const float * restrict * restrict float_regs) {
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        float *restrict result = result_column + row;
        const int64_t *restrict src = src_column + row;
        int n;
        for (n = 0; n < 8; n++) {
          result[n] = ((float_regs[src[n]]) + row)[n];
        }

        uint8_t delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) goto done;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        float *restrict result = result_column + row;
        const int64_t *restrict src = src_column + row;
        int n;
        for (n = 0; n < 4; n++) {
          result[n] = ((float_regs[src[n]]) + row)[n];
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        result_column[row] = (float_regs[src_column[row]])[row];
        uint8_t delta = chain_column[row];
        if (!delta) goto done;
        row += delta;
      } while (row & 3);
    }
  }
done:;
}

void sl_exec_i_indir_load(uint8_t row, uint8_t *restrict chain_column, int64_t *restrict result_column, const int64_t *restrict src_column, const int64_t * restrict * restrict int_regs) {
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        int64_t *restrict result = result_column + row;
        const int64_t *restrict src = src_column + row;
        int n;
        for (n = 0; n < 8; n++) {
          result[n] = ((int_regs[src[n]]) + row)[n];
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
          result[n] = ((int_regs[src[n]]) + row)[n];
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        result_column[row] = (int_regs[src_column[row]])[row];
        uint8_t delta = chain_column[row];
        if (!delta) goto done;
        row += delta;
      } while (row & 3);
    }
  }
done:;
}

void sl_exec_b_indir_load(uint8_t row, uint8_t *restrict chain_column, uint8_t *restrict result_column, const int64_t *restrict src_column, const uint8_t * restrict * restrict bool_regs) {
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        uint8_t *restrict result = result_column + row;
        const int64_t *restrict src = src_column + row;
        int n;
        for (n = 0; n < 8; n++) {
          result[n] = ((bool_regs[src[n]]) + row)[n];
        }

        uint8_t delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) goto done;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        uint8_t *restrict result = result_column + row;
        const int64_t *restrict src = src_column + row;
        int n;
        for (n = 0; n < 4; n++) {
          result[n] = ((bool_regs[src[n]]) + row)[n];
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        result_column[row] = (bool_regs[src_column[row]])[row];
        uint8_t delta = chain_column[row];
        if (!delta) goto done;
        row += delta;
      } while (row & 3);
    }
  }
done:;
}

void sl_exec_p_indir_load(uint8_t row, uint8_t *restrict chain_column, void **restrict result_column, const int64_t *restrict src_column, void * restrict * const restrict *restrict voidp_regs) {
  for (;;) {
    uint64_t chain;
    if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
      do {
        void **restrict result = result_column + row;
        const int64_t *restrict src = src_column + row;
        int n;
        for (n = 0; n < 8; n++) {
          result[n] = ((voidp_regs[src[n]]) + row)[n];
        }

        uint8_t delta = (chain & 0xFF00000000000000) >> 56;
        if (!delta) goto done;
        row += 7 + delta;
      } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
    }
    else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
      do {
        void **restrict result = result_column + row;
        const int64_t *restrict src = src_column + row;
        int n;
        for (n = 0; n < 4; n++) {
          result[n] = ((voidp_regs[src[n]]) + row)[n];
        }
        uint8_t delta = (chain & 0xFF000000) >> 24;
        if (!delta) goto done;
        row += 3 + delta;
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
    }
    else {
      do {
        /* Not trying to evoke auto-vectorization, just get it done. */
        result_column[row] = (voidp_regs[src_column[row]])[row];
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
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
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
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
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
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
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
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
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
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
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
      } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
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

static void sl_exec_split_chains_by_bool(struct sl_execution *exec, struct sl_expr *cond, uint32_t input_chain, uint32_t *ptrue_chain, uint32_t *pfalse_chain) {
  uint8_t row;
  if (input_chain == SL_EXEC_NO_CHAIN) return;
  row = (uint8_t)input_chain;
  const uint8_t * restrict cond_col = BOOL_REG_PTR(cond, 0);
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

static void sl_exec_move_param(struct sl_execution *exec, uint8_t row, struct sl_execution_frame *dst_ef, struct sl_reg_alloc *dst, struct sl_execution_frame *src_ef, struct sl_reg_alloc *src) {
  sl_reg_move_crossframe(exec, row, (int)(src_ef - exec->execution_frames_), src, NULL, (int)(dst_ef - exec->execution_frames_), dst, NULL, 1, 1, 1);
}


static void sl_exec_init_literal(struct sl_execution *exec, uint8_t row, struct sl_reg_alloc *dst, struct sl_expr_temp *src, int offset) {
  int num_components = 0;
  int n;

  switch (src->kind_) {
    case slrak_array: {
      if ((INT_MAX / dst->v_.array_.num_elements_) < offset) {
        /* overflow */
        return ;
      }
      int array_start = offset * (int)dst->v_.array_.num_elements_;
      for (n = 0; n < dst->v_.array_.num_elements_; ++n) {
        sl_exec_init_literal(exec, row, dst, src->v_.comp_.elements_ + n, array_start + n);
      }
      break;
    }
    case slrak_struct: {
      size_t index;
      for (index = 0; index < src->v_.comp_.num_elements_; ++index) {
        sl_exec_init_literal(exec, row, dst->v_.comp_.fields_ + index, src->v_.comp_.elements_ + index, offset);
      }
      break;
    }
    case slrak_float:
      sl_exec_f_init(row, exec->exec_chain_reg_, exec->float_regs_[FLOAT_REG_INDEX_NRV(dst, 0) + offset], src->v_.f_);
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
        sl_exec_f_init(row, exec->exec_chain_reg_, exec->float_regs_[FLOAT_REG_INDEX_NRV(dst, n) + offset], src->v_.v_[n]);
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
        sl_exec_f_init(row, exec->exec_chain_reg_, exec->float_regs_[FLOAT_REG_INDEX_NRV(dst, n) + offset], src->v_.m_[n]);
      }
      break;
    }
    case slrak_int:
      sl_exec_i_init(row, exec->exec_chain_reg_, exec->int_regs_[INT_REG_INDEX_NRV(dst, 0) + offset], src->v_.i_);
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
        sl_exec_i_init(row, exec->exec_chain_reg_, exec->int_regs_[INT_REG_INDEX_NRV(dst, n) + offset], src->v_.iv_[n]);
      }
      break;
    }
    case slrak_bool:
      sl_exec_b_init(row, exec->exec_chain_reg_, exec->bool_regs_[BOOL_REG_INDEX_NRV(dst, 0) + offset], src->v_.b_);
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
        sl_exec_b_init(row, exec->exec_chain_reg_, exec->bool_regs_[BOOL_REG_INDEX_NRV(dst, n) + offset], src->v_.bv_[n]);
      }
      break;
    }
  }
}

static void sl_exec_f_offset_load(uint8_t * restrict chain_column,
                                  uint8_t row,
                                  float * restrict dst,
                                  const float * restrict * restrict float_regs,
                                  int base_reg,
                                  const int64_t * restrict indices) {
  for (;;) {
    float * restrict dst_row = dst + row;
    const float * restrict src_row = float_regs[base_reg + indices[row]];
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_i_offset_load(uint8_t * restrict chain_column,
                                  uint8_t row,
                                  int64_t * restrict dst,
                                  const int64_t * restrict * restrict int_regs,
                                  int base_reg,
                                  const int64_t * restrict indices) {
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const int64_t * restrict src_row = int_regs[base_reg + indices[row]];
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_b_offset_load(uint8_t * restrict chain_column,
                                  uint8_t row,
                                  uint8_t * restrict dst,
                                  const uint8_t * restrict * restrict bool_regs,
                                  int base_reg,
                                  const int64_t * restrict indices) {
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = bool_regs[base_reg + indices[row]];
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_p_offset_load(uint8_t * restrict chain_column,
                                  uint8_t row,
                                  void * restrict * restrict dst,
                                  void * const restrict * restrict * restrict voidp_regs,
                                  int base_reg,
                                  const int64_t * restrict indices) {
  for (;;) {
    void * restrict * restrict dst_row = dst + row;
    void * const restrict * restrict src_row = voidp_regs[base_reg + indices[row]];
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_f_offset_load_strided(uint8_t * restrict chain_column,
                                          uint8_t row,
                                          float * restrict dst,
                                          const float * restrict * restrict float_regs,
                                          int base_reg,
                                          const int64_t * restrict indices,
                                          int index_stride,
                                          int array_offset) {
  for (;;) {
    float * restrict dst_row = dst + row;
    const float * restrict src_row = float_regs[base_reg + index_stride * indices[row] + array_offset];
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_i_offset_load_strided(uint8_t * restrict chain_column,
                                          uint8_t row,
                                          int64_t * restrict dst,
                                          const int64_t * restrict * restrict int_regs,
                                          int base_reg,
                                          const int64_t * restrict indices,
                                          int index_stride,
                                          int array_offset) {
  for (;;) {
    int64_t * restrict dst_row = dst + row;
    const int64_t * restrict src_row = int_regs[base_reg + index_stride * indices[row] + array_offset];
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_b_offset_load_strided(uint8_t * restrict chain_column,
                                          uint8_t row,
                                          uint8_t * restrict dst,
                                          const uint8_t * restrict * restrict bool_regs,
                                          int base_reg,
                                          const int64_t * restrict indices,
                                          int index_stride,
                                          int array_offset) {
  for (;;) {
    uint8_t * restrict dst_row = dst + row;
    const uint8_t * restrict src_row = bool_regs[base_reg + index_stride * indices[row] + array_offset];
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_p_offset_load_strided(uint8_t * restrict chain_column,
                                          uint8_t row,
                                          void * restrict * restrict dst,
                                          void * const restrict * restrict * restrict voidp_regs,
                                          int base_reg,
                                          const int64_t * restrict indices,
                                          int index_stride,
                                          int array_offset) {
  for (;;) {
    void *restrict *restrict dst_row = dst + row;
    void *const restrict *restrict src_row = voidp_regs[base_reg + index_stride * indices[row] + array_offset];
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}


static void sl_exec_offset_load_strided(struct sl_execution *exec, uint8_t row, 
                                        struct sl_reg_alloc *dst,
                                        const struct sl_reg_alloc *arr,
                                        const struct sl_reg_alloc *index,
                                        int index_stride,
                                        int array_offset) {
  size_t num_components = 0;
  size_t n;
  switch (arr->kind_) {
    case slrak_struct: {
      for (n = 0; n < arr->v_.comp_.num_fields_; ++n) {
        sl_exec_offset_load_strided(exec, row, dst->v_.comp_.fields_ + n, arr->v_.comp_.fields_ + n, index, index_stride, array_offset);
      }
      break;
    }
    case slrak_array: {
      int new_index_stride;
      int new_array_offset;
      if (arr->v_.array_.num_elements_ > (INT_MAX/index_stride)) {
        /* overflow */
        return;
      }
      new_index_stride = (int)(index_stride * arr->v_.array_.num_elements_);
      new_array_offset = (int)(array_offset * arr->v_.array_.num_elements_);
      for (n = 0; n < arr->v_.array_.num_elements_; ++n) {
        sl_exec_offset_load_strided(exec, row, dst->v_.array_.head_, arr->v_.array_.head_, index, new_index_stride, (int)(new_array_offset + n));
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
      switch (arr->kind_) {
        case slrak_float: num_components = 1; break;
        case slrak_vec2: num_components = 2; break;
        case slrak_vec3: num_components = 3; break;
        case slrak_vec4: num_components = 4; break;
        case slrak_mat2: num_components = 4; break;
        case slrak_mat3: num_components = 9; break;
        case slrak_mat4: num_components = 16; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_f_offset_load_strided(exec->exec_chain_reg_, row,
                                      FLOAT_REG_PTR_NRV(dst, n),
                                      exec->float_regs_,
                                      FLOAT_REG_INDEX_NRV(arr, n),
                                      INT_REG_PTR_NRV(index, 0),
                                      index_stride,
                                      array_offset);
      }
      break;
    }

    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      switch (arr->kind_) {
        case slrak_int: num_components = 1; break;
        case slrak_ivec2: num_components = 2; break;
        case slrak_ivec3: num_components = 3; break;
        case slrak_ivec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_i_offset_load_strided(exec->exec_chain_reg_, row,
                                      INT_REG_PTR_NRV(dst, n),
                                      exec->int_regs_,
                                      INT_REG_INDEX_NRV(arr, n),
                                      INT_REG_PTR_NRV(index, 0),
                                      index_stride,
                                      array_offset);
      }
      break;
    }
    case slrak_bool:
    case slrak_bvec2:
    case slrak_bvec3:
    case slrak_bvec4: {
      switch (arr->kind_) {
        case slrak_bool: num_components = 1; break;
        case slrak_bvec2: num_components = 2; break;
        case slrak_bvec3: num_components = 3; break;
        case slrak_bvec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_b_offset_load_strided(exec->exec_chain_reg_, row,
                                      BOOL_REG_PTR_NRV(dst, n),
                                      exec->bool_regs_,
                                      BOOL_REG_INDEX_NRV(arr, n),
                                      INT_REG_PTR_NRV(index, 0),
                                      index_stride,
                                      array_offset);
      }
      break;
    }
    case slrak_sampler2D: {
      sl_exec_p_offset_load_strided(exec->exec_chain_reg_, row,
                                    SAMPLER_2D_REG_PTR_NRV(dst, 0),
                                    exec->sampler_2D_regs_,
                                    SAMPLER_2D_REG_INDEX_NRV(arr, 0),
                                    INT_REG_PTR_NRV(index, 0),
                                    index_stride,
                                    array_offset);
      break;
    }
    case slrak_samplerCube: {
      sl_exec_p_offset_load_strided(exec->exec_chain_reg_, row,
                                    SAMPLER_CUBE_REG_PTR_NRV(dst, 0),
                                    exec->sampler_cube_regs_,
                                    SAMPLER_CUBE_REG_INDEX_NRV(arr, 0),
                                    INT_REG_PTR_NRV(index, 0),
                                    index_stride,
                                    array_offset);
      break;
    }
  }
}

static void sl_exec_offset_load(struct sl_execution *exec, uint8_t row, 
                                struct sl_reg_alloc *dst,
                                const struct sl_reg_alloc *arr,
                                const struct sl_reg_alloc *index) {
  size_t num_components = 0;
  size_t n;
  switch (arr->kind_) {
    case slrak_struct: {
      for (n = 0; n < arr->v_.comp_.num_fields_; ++n) {
        sl_exec_offset_load(exec, row, dst->v_.comp_.fields_ + n, arr->v_.comp_.fields_ + n, index);
      }
      break;
    }
    case slrak_array: {
      if (arr->v_.array_.num_elements_ > INT_MAX) {
        return ; /* overflow */
      }
      int new_index_stride = (int)arr->v_.array_.num_elements_;
      for (n = 0; n < arr->v_.array_.num_elements_; ++n) {
        sl_exec_offset_load_strided(exec, row, dst->v_.array_.head_, arr->v_.array_.head_, index, new_index_stride, 0);
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
      switch (arr->kind_) {
        case slrak_float: num_components = 1; break;
        case slrak_vec2: num_components = 2; break;
        case slrak_vec3: num_components = 3; break;
        case slrak_vec4: num_components = 4; break;
        case slrak_mat2: num_components = 4; break;
        case slrak_mat3: num_components = 9; break;
        case slrak_mat4: num_components = 16; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_f_offset_load(exec->exec_chain_reg_, row,
                              FLOAT_REG_PTR_NRV(dst, n),
                              exec->float_regs_,
                              FLOAT_REG_INDEX_NRV(arr, n),
                              INT_REG_PTR_NRV(index, 0));
      }
      break;
    }

    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      switch (arr->kind_) {
        case slrak_int: num_components = 1; break;
        case slrak_ivec2: num_components = 2; break;
        case slrak_ivec3: num_components = 3; break;
        case slrak_ivec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_i_offset_load(exec->exec_chain_reg_, row,
                              INT_REG_PTR_NRV(dst, n),
                              exec->int_regs_,
                              INT_REG_INDEX_NRV(arr, n),
                              INT_REG_PTR_NRV(index, 0));
      }
      break;
    }
    case slrak_bool:
    case slrak_bvec2:
    case slrak_bvec3:
    case slrak_bvec4: {
      switch (arr->kind_) {
        case slrak_bool: num_components = 1; break;
        case slrak_bvec2: num_components = 2; break;
        case slrak_bvec3: num_components = 3; break;
        case slrak_bvec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_b_offset_load(exec->exec_chain_reg_, row,
                              BOOL_REG_PTR_NRV(dst, n),
                              exec->bool_regs_,
                              BOOL_REG_INDEX_NRV(arr, n),
                              INT_REG_PTR_NRV(index, 0));
      }
      break;
    }
    case slrak_sampler2D: {
      sl_exec_p_offset_load(exec->exec_chain_reg_, row,
                            SAMPLER_2D_REG_PTR_NRV(dst, 0),
                            exec->sampler_2D_regs_,
                            SAMPLER_2D_REG_INDEX_NRV(arr, 0),
                            INT_REG_PTR_NRV(index, 0));
      break;
    }
    case slrak_samplerCube: {
      sl_exec_p_offset_load(exec->exec_chain_reg_, row,
                            SAMPLER_CUBE_REG_PTR_NRV(dst, 0),
                            exec->sampler_cube_regs_,
                            SAMPLER_CUBE_REG_INDEX_NRV(arr, 0),
                            INT_REG_PTR_NRV(index, 0));
      break;
    }
  }
}

static void sl_exec_indirect_load(struct sl_execution *exec, uint8_t row, 
                                  struct sl_reg_alloc *dst,
                                  const struct sl_reg_alloc *indir,
                                  int array_quantity) {
  size_t num_components = 0;
  size_t n;
  switch (indir->kind_) {
    case slrak_struct: {
      for (n = 0; n < indir->v_.comp_.num_fields_; ++n) {
        sl_exec_indirect_load(exec, row, dst->v_.comp_.fields_ + n, indir->v_.comp_.fields_ + n, array_quantity);
      }
      break;
    }
    case slrak_array: {
      if (indir->v_.array_.num_elements_ > INT_MAX) {
        return ; /* overflow */
      }

      array_quantity *= (int)dst->v_.array_.num_elements_;

      sl_exec_indirect_load(exec, row, dst->v_.array_.head_, indir->v_.array_.head_, array_quantity);

      break;
    }
    case slrak_float:
    case slrak_vec2:
    case slrak_vec3:
    case slrak_vec4:
    case slrak_mat2:
    case slrak_mat3:
    case slrak_mat4: {
      switch (indir->kind_) {
        case slrak_float: num_components = 1; break;
        case slrak_vec2: num_components = 2; break;
        case slrak_vec3: num_components = 3; break;
        case slrak_vec4: num_components = 4; break;
        case slrak_mat2: num_components = 4; break;
        case slrak_mat3: num_components = 9; break;
        case slrak_mat4: num_components = 16; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_f_indir_load(row, exec->exec_chain_reg_,
                             FLOAT_REG_PTR_NRV(dst, n),
                             INT_REG_PTR_NRV(indir, n),
                             exec->float_regs_);
      }
      break;
    }

    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      switch (indir->kind_) {
        case slrak_int: num_components = 1; break;
        case slrak_ivec2: num_components = 2; break;
        case slrak_ivec3: num_components = 3; break;
        case slrak_ivec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_i_indir_load(row, exec->exec_chain_reg_,
                             INT_REG_PTR_NRV(dst, n),
                             INT_REG_PTR_NRV(indir, n),
                             exec->int_regs_);
      }
      break;
    }
    case slrak_bool:
    case slrak_bvec2:
    case slrak_bvec3:
    case slrak_bvec4: {
      switch (indir->kind_) {
        case slrak_bool: num_components = 1; break;
        case slrak_bvec2: num_components = 2; break;
        case slrak_bvec3: num_components = 3; break;
        case slrak_bvec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_b_indir_load(row, exec->exec_chain_reg_,
                             BOOL_REG_PTR_NRV(dst, n),
                             INT_REG_PTR_NRV(indir, n),
                             exec->bool_regs_);
      }
      break;
    }
    case slrak_sampler2D: {
      sl_exec_p_indir_load(row, exec->exec_chain_reg_,
                           SAMPLER_2D_REG_PTR_NRV(dst, 0),
                           INT_REG_PTR_NRV(indir, 0),
                           exec->sampler_2D_regs_);
      break;
    }
    case slrak_samplerCube: {
      sl_exec_p_indir_load(row, exec->exec_chain_reg_,
                           SAMPLER_CUBE_REG_PTR_NRV(dst, 0),
                           INT_REG_PTR_NRV(indir, 0),
                           exec->sampler_cube_regs_);
      break;
    }
  }
}

static void sl_exec_indirect_offset_load(struct sl_execution *exec, uint8_t row, 
                                         struct sl_reg_alloc *dst,
                                         const struct sl_reg_alloc *indir,
                                         const struct sl_reg_alloc *index) {
  // XXX: Implement
}

static void sl_exec_f_offset_store(uint8_t * restrict chain_column,
                                   uint8_t row,
                                   float * restrict * restrict float_regs,
                                   int base_reg,
                                   const int64_t * restrict indices,
                                   const float * restrict src) {
  for (;;) {
    float * restrict dst_row = float_regs[base_reg + indices[row]];
    const float * restrict src_row = src + row;
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_i_offset_store(uint8_t * restrict chain_column,
                                   uint8_t row,
                                   int64_t * restrict * restrict int_regs,
                                   int base_reg,
                                   const int64_t * restrict indices,
                                   const int64_t * restrict src) {
  for (;;) {
    int64_t * restrict dst_row = int_regs[base_reg + indices[row]];
    const int64_t * restrict src_row = src + row;
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_b_offset_store(uint8_t * restrict chain_column,
                                   uint8_t row,
                                   uint8_t * restrict * restrict bool_regs,
                                   int base_reg,
                                   const int64_t * restrict indices,
                                   const uint8_t * restrict src) {
  for (;;) {
    uint8_t * restrict dst_row = bool_regs[base_reg + indices[row]];
    const uint8_t * restrict src_row = src + row;
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_p_offset_store(uint8_t * restrict chain_column,
                                   uint8_t row,
                                   void * restrict * restrict * restrict voidp_regs,
                                   int base_reg,
                                   const int64_t * restrict indices,
                                   const void * const restrict * restrict src) {
  for (;;) {
    void * restrict * restrict dst_row = voidp_regs[base_reg + indices[row] ]; 
    void * const restrict * restrict src_row = src + row;
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}


static void sl_exec_f_offset_store_strided(uint8_t * restrict chain_column,
                                           uint8_t row,
                                           float * restrict * restrict float_regs,
                                           int base_reg,
                                           const int64_t * restrict indices,
                                           int index_stride,
                                           int array_offset,
                                           const float * restrict src) {
  for (;;) {
    float * restrict dst_row = float_regs[base_reg + index_stride * indices[row] + array_offset];
    const float * restrict src_row = src + row;
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_i_offset_store_strided(uint8_t * restrict chain_column,
                                           uint8_t row,
                                           int64_t * restrict * restrict int_regs,
                                           int base_reg,
                                           const int64_t * restrict indices,
                                           int index_stride,
                                           int array_offset,
                                           const int64_t * restrict src) {
  for (;;) {
    int64_t * restrict dst_row = int_regs[base_reg + index_stride * indices[row] + array_offset];
    const int64_t * restrict src_row = src + row;
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_b_offset_store_strided(uint8_t * restrict chain_column,
                                           uint8_t row,
                                           uint8_t * restrict * restrict bool_regs,
                                           int base_reg,
                                           const int64_t * restrict indices,
                                           int index_stride,
                                           int array_offset,
                                           const uint8_t * restrict src) {
  for (;;) {
    uint8_t * restrict dst_row = bool_regs[base_reg + index_stride * indices[row] + array_offset];
    const uint8_t * restrict src_row = src + row;
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_p_offset_store_strided(uint8_t * restrict chain_column,
                                           uint8_t row,
                                           void * restrict * restrict * restrict voidp_regs,
                                           int base_reg,
                                           const int64_t * restrict indices,
                                           int index_stride,
                                           int array_offset,
                                           const void * const restrict * restrict src) {
  for (;;) {
    void * restrict * restrict dst_row = voidp_regs[base_reg + index_stride * indices[row] + array_offset]; 
    void * const restrict * restrict src_row = src + row;
    
    *dst_row = *src_row;

    uint8_t delta = chain_column[row];
    if (!delta) return;
    row += delta;
  }
}

static void sl_exec_offset_store_strided(struct sl_execution *exec, uint8_t row, 
                                         struct sl_reg_alloc *dst_arr,
                                         struct sl_reg_alloc *dst_index,
                                         int index_stride,
                                         int array_offset,
                                         const struct sl_reg_alloc *src) {
  size_t num_components = 0;
  size_t n;
  switch (dst_arr->kind_) {
    case slrak_struct: {
      for (n = 0; n < dst_arr->v_.comp_.num_fields_; ++n) {
        sl_exec_offset_store_strided(exec, row, dst_arr->v_.comp_.fields_ + n, dst_index, index_stride, array_offset, src->v_.comp_.fields_ + n);
      }
      break;
    }
    case slrak_array: {
      int new_index_stride;
      int new_array_offset;
      if (dst_arr->v_.array_.num_elements_ > (INT_MAX/index_stride)) {
        /* overflow */
        return;
      }
      new_index_stride = (int)(index_stride * dst_arr->v_.array_.num_elements_);
      new_array_offset = (int)(array_offset * dst_arr->v_.array_.num_elements_);

      for (n = 0; n < dst_arr->v_.array_.num_elements_; ++n) {
        sl_exec_offset_store_strided(exec, row, dst_arr->v_.array_.head_, dst_index, new_index_stride, (int)(new_array_offset + n), src->v_.array_.head_);
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
      switch (dst_arr->kind_) {
        case slrak_float: num_components = 1; break;
        case slrak_vec2: num_components = 2; break;
        case slrak_vec3: num_components = 3; break;
        case slrak_vec4: num_components = 4; break;
        case slrak_mat2: num_components = 4; break;
        case slrak_mat3: num_components = 9; break;
        case slrak_mat4: num_components = 16; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_f_offset_store_strided(exec->exec_chain_reg_, row,
                                       exec->float_regs_,
                                       FLOAT_REG_INDEX_NRV(dst_arr, n),
                                       INT_REG_PTR_NRV(dst_index, 0),
                                       index_stride, array_offset,
                                       FLOAT_REG_PTR_NRV(src, n));
      }
      break;
    }

    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      switch (dst_arr->kind_) {
        case slrak_int: num_components = 1; break;
        case slrak_ivec2: num_components = 2; break;
        case slrak_ivec3: num_components = 3; break;
        case slrak_ivec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_i_offset_store_strided(exec->exec_chain_reg_, row,
                                       exec->int_regs_,
                                       INT_REG_INDEX_NRV(dst_arr, n),
                                       INT_REG_PTR_NRV(dst_index, 0),
                                       index_stride, array_offset,
                                       INT_REG_PTR_NRV(src, n));
      }
      break;
    }
    case slrak_bool:
    case slrak_bvec2:
    case slrak_bvec3:
    case slrak_bvec4: {
      switch (dst_arr->kind_) {
        case slrak_bool: num_components = 1; break;
        case slrak_bvec2: num_components = 2; break;
        case slrak_bvec3: num_components = 3; break;
        case slrak_bvec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_b_offset_store_strided(exec->exec_chain_reg_, row,
                                       exec->bool_regs_,
                                       BOOL_REG_INDEX_NRV(dst_arr, n),
                                       INT_REG_PTR_NRV(dst_index, 0),
                                       index_stride, array_offset,
                                       BOOL_REG_PTR_NRV(src, n));
      }
      break;
    }
    case slrak_sampler2D: {
      sl_exec_p_offset_store_strided(exec->exec_chain_reg_, row,
                                     exec->sampler_2D_regs_,
                                     SAMPLER_2D_REG_INDEX_NRV(dst_arr, 0),
                                     INT_REG_PTR_NRV(dst_index, 0),
                                     index_stride, array_offset,
                                     SAMPLER_2D_REG_PTR_NRV(src, 0));
      break;
    }
    case slrak_samplerCube: {
      sl_exec_p_offset_store_strided(exec->exec_chain_reg_, row,
                                     exec->sampler_cube_regs_,
                                     SAMPLER_CUBE_REG_INDEX_NRV(dst_arr, 0),
                                     INT_REG_PTR_NRV(dst_index, 0),
                                     index_stride, array_offset,
                                     SAMPLER_CUBE_REG_PTR_NRV(src, 0));
      break;
    }

  }
}


static void sl_exec_offset_store(struct sl_execution *exec, uint8_t row, 
                                 struct sl_reg_alloc *dst_arr,
                                 struct sl_reg_alloc *dst_index,
                                 const struct sl_reg_alloc *src) {
  size_t num_components = 0;
  size_t n;
  switch (dst_arr->kind_) {
    case slrak_struct: {
      for (n = 0; n < dst_arr->v_.comp_.num_fields_; ++n) {
        sl_exec_offset_store(exec, row, dst_arr->v_.comp_.fields_ + n, dst_index, src->v_.comp_.fields_ + n);
      }
      break;
    }
    case slrak_array: {
      if (dst_arr->v_.array_.num_elements_ > INT_MAX) {
        return; /* overflow */
      }
      int new_index_stride = (int)dst_arr->v_.array_.num_elements_;
      for (n = 0; n < dst_arr->v_.array_.num_elements_; ++n) {
        sl_exec_offset_store_strided(exec, row, dst_arr->v_.array_.head_, dst_index, new_index_stride, 0, src->v_.array_.head_);
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
      switch (dst_arr->kind_) {
        case slrak_float: num_components = 1; break;
        case slrak_vec2: num_components = 2; break;
        case slrak_vec3: num_components = 3; break;
        case slrak_vec4: num_components = 4; break;
        case slrak_mat2: num_components = 4; break;
        case slrak_mat3: num_components = 9; break;
        case slrak_mat4: num_components = 16; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_f_offset_store(exec->exec_chain_reg_, row,
                               exec->float_regs_,
                               FLOAT_REG_INDEX_NRV(dst_arr, n),
                               INT_REG_PTR_NRV(dst_index, 0),
                               FLOAT_REG_PTR_NRV(src, n));
      }
      break;
    }

    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      switch (dst_arr->kind_) {
        case slrak_int: num_components = 1; break;
        case slrak_ivec2: num_components = 2; break;
        case slrak_ivec3: num_components = 3; break;
        case slrak_ivec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_i_offset_store(exec->exec_chain_reg_, row,
                               exec->int_regs_,
                               INT_REG_INDEX_NRV(dst_arr, n),
                               INT_REG_PTR_NRV(dst_index, 0),
                               INT_REG_PTR_NRV(src, n));
      }
      break;
    }
    case slrak_bool:
    case slrak_bvec2:
    case slrak_bvec3:
    case slrak_bvec4: {
      switch (dst_arr->kind_) {
        case slrak_bool: num_components = 1; break;
        case slrak_bvec2: num_components = 2; break;
        case slrak_bvec3: num_components = 3; break;
        case slrak_bvec4: num_components = 4; break;
      }
      for (n = 0; n < num_components; ++n) {
        sl_exec_b_offset_store(exec->exec_chain_reg_, row,
                               exec->bool_regs_,
                               BOOL_REG_INDEX_NRV(dst_arr, n),
                               INT_REG_PTR_NRV(dst_index, 0),
                               BOOL_REG_PTR_NRV(src, n));
      }
      break;
    }
    case slrak_sampler2D: {
      sl_exec_p_offset_store(exec->exec_chain_reg_, row,
                             exec->sampler_2D_regs_,
                             SAMPLER_2D_REG_INDEX_NRV(dst_arr, 0),
                             INT_REG_PTR_NRV(dst_index, 0),
                             SAMPLER_2D_REG_PTR_NRV(src, 0));
      break;
    }
    case slrak_samplerCube: {
      sl_exec_p_offset_store(exec->exec_chain_reg_, row,
                             exec->sampler_cube_regs_,
                             SAMPLER_CUBE_REG_INDEX_NRV(dst_arr, 0),
                             INT_REG_PTR_NRV(dst_index, 0),
                             SAMPLER_CUBE_REG_PTR_NRV(src, 0));
      break;
    }
  }
}

static size_t sl_exec_str_dump(const char *s, char *output_str, size_t at) {
  size_t len = strlen(s);
  if (output_str) memcpy(output_str + at, s, len);
  return at + len;
}


size_t sl_exec_offset_dump_strided(struct sl_execution *exec, 
                                   char *output_str,
                                   uint8_t single_row, 
                                   const struct sl_reg_alloc *arr,
                                   const struct sl_reg_alloc *index,
                                   int index_stride,
                                   int array_offset) {
  char digits[100];
  int have_index = index && (index->kind_ != slrak_void);
  size_t num_components = 0;
  size_t str_size = 0;
  size_t n;
  switch (arr->kind_) {
    case slrak_struct: {
      str_size += sl_type_dump(arr->v_.comp_.struct_type_, output_str ? output_str + str_size : NULL);
      str_size = sl_exec_str_dump("(", output_str, str_size);

      for (n = 0; n < arr->v_.comp_.num_fields_; ++n) {
        if (n) {
          str_size = sl_exec_str_dump(", ", output_str, str_size);
        }
        str_size += sl_exec_offset_dump_strided(exec, output_str ? output_str + str_size : NULL, single_row, arr->v_.comp_.fields_ + n, index, index_stride, array_offset);
      }
      str_size = sl_exec_str_dump(")", output_str, str_size);
      break;
    }
    case slrak_array: {
      str_size += sl_type_dump_array_of_element(arr->v_.array_.element_type_, arr->v_.array_.num_elements_, output_str ? output_str + str_size : NULL);
      str_size = sl_exec_str_dump("(", output_str, str_size);
      int new_index_stride;
      int new_array_offset;
      if (arr->v_.array_.num_elements_ > (INT_MAX/index_stride)) {
        /* overflow -- in such a fatal case of internal corruption, we just abort; any attempt
         * to then allocate and print will result in a truncated output at the point of the 
         * problem (as it'll abort at the same place again). */
        return str_size;
      }
      new_index_stride = (int)(index_stride * arr->v_.array_.num_elements_);
      new_array_offset = (int)(array_offset * arr->v_.array_.num_elements_);
      for (n = 0; n < arr->v_.array_.num_elements_; ++n) {
        if (n) {
          str_size = sl_exec_str_dump(", ", output_str, str_size);
        }
        str_size += sl_exec_offset_dump_strided(exec, output_str ? output_str + str_size : NULL, single_row, arr->v_.array_.head_, index, new_index_stride, (int)(new_array_offset + n));
      }
      str_size = sl_exec_str_dump(")", output_str, str_size);
      break;
    }
    case slrak_float:
    case slrak_vec2:
    case slrak_vec3:
    case slrak_vec4:
    case slrak_mat2:
    case slrak_mat3:
    case slrak_mat4: {
      const char *sltype = "??";
      switch (arr->kind_) {
        case slrak_float: num_components = 1; sltype = ""; break;
        case slrak_vec2:  num_components = 2; sltype = "vec2"; break;
        case slrak_vec3:  num_components = 3; sltype = "vec3"; break;
        case slrak_vec4:  num_components = 4; sltype = "vec4"; break;
        case slrak_mat2:  num_components = 4; sltype = "mat2"; break;
        case slrak_mat3:  num_components = 9; sltype = "mat3"; break;
        case slrak_mat4:  num_components = 16; sltype = "mat4"; break;
      }
      str_size = sl_exec_str_dump(sltype, output_str, str_size);
      if (arr->kind_ != slrak_float) str_size = sl_exec_str_dump("(", output_str, str_size);
      for (n = 0; n < num_components; ++n) {
        int base_reg = FLOAT_REG_INDEX_NRV(arr, n);
        int64_t *index_rows = have_index ? INT_REG_PTR_NRV(index, 0) : NULL;
        int reg = base_reg + (index_rows ? ((int)index_rows[single_row]) * index_stride : 0) + array_offset;
        float component = exec->float_regs_[reg][single_row];
        if (n) {
          snprintf(digits, sizeof(digits), ", %f", component);
        }
        else {
          snprintf(digits, sizeof(digits), "%f", component);
        }
        str_size = sl_exec_str_dump(digits, output_str, str_size);
      }
      if (arr->kind_ != slrak_float) str_size = sl_exec_str_dump(")", output_str, str_size);
      break;
    }

    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      const char *sltype = "??";
      switch (arr->kind_) {
        case slrak_int: num_components = 1; sltype = ""; break;
        case slrak_ivec2:  num_components = 2; sltype = "ivec2"; break;
        case slrak_ivec3:  num_components = 3; sltype = "ivec3"; break;
        case slrak_ivec4:  num_components = 4; sltype = "ivec4"; break;
      }
      str_size = sl_exec_str_dump(sltype, output_str, str_size);
      if (arr->kind_ != slrak_int) str_size = sl_exec_str_dump("(", output_str, str_size);
      for (n = 0; n < num_components; ++n) {
        int base_reg = INT_REG_INDEX_NRV(arr, n);
        int64_t *index_rows = have_index ? INT_REG_PTR_NRV(index, 0) : NULL;
        int reg = base_reg + (index_rows ? ((int)index_rows[single_row]) * index_stride : 0) + array_offset;
        int64_t component = exec->int_regs_[reg][single_row];
        if (n) {
          snprintf(digits, sizeof(digits), ", %" PRId64, component);
        }
        else {
          snprintf(digits, sizeof(digits), "%" PRId64, component);
        }
        str_size = sl_exec_str_dump(digits, output_str, str_size);
      }
      if (arr->kind_ != slrak_int) str_size = sl_exec_str_dump(")", output_str, str_size);
      break;
    }
    case slrak_bool:
    case slrak_bvec2:
    case slrak_bvec3:
    case slrak_bvec4: {
      const char *sltype = "??";
      switch (arr->kind_) {
        case slrak_bool: num_components = 1; sltype = ""; break;
        case slrak_bvec2:  num_components = 2; sltype = "bvec2"; break;
        case slrak_bvec3:  num_components = 3; sltype = "bvec3"; break;
        case slrak_bvec4:  num_components = 4; sltype = "bvec4"; break;
      }
      str_size = sl_exec_str_dump(sltype, output_str, str_size);
      if (arr->kind_ != slrak_bool) str_size = sl_exec_str_dump("(", output_str, str_size);
      for (n = 0; n < num_components; ++n) {
        int base_reg = BOOL_REG_INDEX_NRV(arr, n);
        int64_t *index_rows = have_index ? INT_REG_PTR_NRV(index, 0) : NULL;
        int reg = base_reg + (index_rows ? ((int)index_rows[single_row]) * index_stride : 0) + array_offset;
        uint8_t component = exec->bool_regs_[reg][single_row];
        const char *value = NULL;
        if (n) {
          value = component ? ", true" : ", false";
        }
        else {
          value = component ? "true" : "false";
        }
        str_size = sl_exec_str_dump(value, output_str, str_size);
      }
      if (arr->kind_ != slrak_bool) str_size = sl_exec_str_dump(")", output_str, str_size);
      break;
    }
  }
  return str_size;
}

size_t sl_exec_dump(struct sl_execution *exec, char *output_str, 
                    uint8_t single_row, const struct sl_reg_alloc *ra) {
  return sl_exec_offset_dump_strided(exec, output_str, single_row, ra, NULL, 1, 0);
}

void sl_exec_offset_dumpf_strided(struct sl_execution *exec, 
                                  FILE *fp,
                                  uint8_t single_row, 
                                  const struct sl_reg_alloc *arr,
                                  const struct sl_reg_alloc *index,
                                  int index_stride,
                                  int array_offset) {
  size_t len_needed = sl_exec_offset_dump_strided(exec, NULL, single_row, arr, index, index_stride, array_offset);
  char *m = (char *)malloc(len_needed);
  if (!m) return;
  sl_exec_offset_dump_strided(exec, m, single_row, arr, index, index_stride, array_offset);
  fwrite(m, 1, len_needed, fp);
  free(m);
}

void sl_exec_dumpf(struct sl_execution *exec, FILE *fp,
                    uint8_t single_row, const struct sl_reg_alloc *ra) {
  sl_exec_offset_dumpf_strided(exec, fp, single_row, ra, NULL, 1, 0);
}

static void sl_exec_decrement(struct sl_execution *exec, uint8_t row, struct sl_reg_alloc *dst, struct sl_reg_alloc *opd) {
  sl_reg_alloc_kind_t kind = opd->kind_;
  switch (kind) {
    case slrak_float:
    case slrak_vec2:
    case slrak_vec3:
    case slrak_vec4:
    case slrak_mat2:
    case slrak_mat3:
    case slrak_mat4: {
      size_t num_components = 0;
      switch (kind) {
        case slrak_float: num_components = 1; break;
        case slrak_vec2: num_components = 2; break;
        case slrak_vec3: num_components = 3; break;
        case slrak_vec4: num_components = 4; break;
        case slrak_mat2: num_components = 4; break;
        case slrak_mat3: num_components = 9; break;
        case slrak_mat4: num_components = 16; break;
      }
      size_t n;
      for (n = 0; n < num_components; ++n) {
        sl_exec_f_decrement(row, exec->exec_chain_reg_, FLOAT_REG_PTR_NRV(dst, n), FLOAT_REG_PTR_NRV(opd, n));
      }
      break;
    }
    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      size_t num_components = 0;
      switch (kind) {
        case slrak_int:  num_components = 1; break;
        case slrak_ivec2:num_components = 2; break;
        case slrak_ivec3:num_components = 3; break;
        case slrak_ivec4:num_components = 4; break;
      }
      size_t n;
      for (n = 0; n < num_components; ++n) {
        sl_exec_i_decrement(row, exec->exec_chain_reg_, INT_REG_PTR_NRV(dst, n), INT_REG_PTR_NRV(opd, n));
      }
      break;
    }
  }
}

static void sl_exec_increment(struct sl_execution *exec, uint8_t row, struct sl_reg_alloc *dst, struct sl_reg_alloc *opd) {
  sl_reg_alloc_kind_t kind = opd->kind_;
  switch (kind) {
    case slrak_float:
    case slrak_vec2:
    case slrak_vec3:
    case slrak_vec4:
    case slrak_mat2:
    case slrak_mat3:
    case slrak_mat4: {
      size_t num_components = 0;
      switch (kind) {
        case slrak_float: num_components = 1; break;
        case slrak_vec2: num_components = 2; break;
        case slrak_vec3: num_components = 3; break;
        case slrak_vec4: num_components = 4; break;
        case slrak_mat2: num_components = 4; break;
        case slrak_mat3: num_components = 9; break;
        case slrak_mat4: num_components = 16; break;
      }
      size_t n;
      for (n = 0; n < num_components; ++n) {
        sl_exec_f_increment(row, exec->exec_chain_reg_, FLOAT_REG_PTR_NRV(dst, n), FLOAT_REG_PTR_NRV(opd, n));
      }
      break;
    }
    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      size_t num_components = 0;
      switch (kind) {
        case slrak_int:  num_components = 1; break;
        case slrak_ivec2:num_components = 2; break;
        case slrak_ivec3:num_components = 3; break;
        case slrak_ivec4:num_components = 4; break;
      }
      size_t n;
      for (n = 0; n < num_components; ++n) {
        sl_exec_i_increment(row, exec->exec_chain_reg_, INT_REG_PTR_NRV(dst, n), INT_REG_PTR_NRV(opd, n));
      }
      break;
    }
  }
}

static void sl_exec_negate(struct sl_execution *exec, uint8_t row, struct sl_expr *dst, struct sl_expr *opd) {
  sl_reg_alloc_kind_t kind = EXPR_RVALUE(opd)->kind_;
  switch (kind) {
    case slrak_float:
    case slrak_vec2:
    case slrak_vec3:
    case slrak_vec4:
    case slrak_mat2:
    case slrak_mat3:
    case slrak_mat4: {
      size_t num_components = 0;
      switch (kind) {
        case slrak_float: num_components = 1; break;
        case slrak_vec2: num_components = 2; break;
        case slrak_vec3: num_components = 3; break;
        case slrak_vec4: num_components = 4; break;
        case slrak_mat2: num_components = 4; break;
        case slrak_mat3: num_components = 9; break;
        case slrak_mat4: num_components = 16; break;
      }
      size_t n;
      for (n = 0; n < num_components; ++n) {
        sl_exec_f_negate(row, exec->exec_chain_reg_, FLOAT_REG_PTR_NRV(&dst->base_regs_, n), FLOAT_REG_PTR(opd, n));
      }
      break;
    }
    case slrak_int:
    case slrak_ivec2:
    case slrak_ivec3:
    case slrak_ivec4: {
      size_t num_components = 0;
      switch (kind) {
        case slrak_int:  num_components = 1; break;
        case slrak_ivec2:num_components = 2; break;
        case slrak_ivec3:num_components = 3; break;
        case slrak_ivec4:num_components = 4; break;
      }
      size_t n;
      for (n = 0; n < num_components; ++n) {
        sl_exec_i_negate(row, exec->exec_chain_reg_, INT_REG_PTR_NRV(&dst->base_regs_, n), INT_REG_PTR(opd, n));
      }
      break;
    }
  }
}

static void sl_exec_logical_not(struct sl_execution *exec, uint8_t row, struct sl_expr *dst, struct sl_expr *opd) {
  size_t num_components = 0;
  switch (EXPR_RVALUE(opd)->kind_) {
    case slrak_bool:  num_components = 1; break;
    case slrak_bvec2: num_components = 2; break;
    case slrak_bvec3: num_components = 3; break;
    case slrak_bvec4: num_components = 4; break;
  }
  size_t n;
  for (n = 0; n < num_components; ++n) {
    sl_exec_b_logical_not(row, exec->exec_chain_reg_, BOOL_REG_PTR_NRV(&dst->base_regs_, n), BOOL_REG_PTR(opd, n));
  }
}

static void sl_exec_mul(struct sl_execution *exec, uint8_t row, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  int r, c;
  sl_reg_alloc_kind_t left_kind = EXPR_RVALUE(left)->kind_;
  sl_reg_alloc_kind_t right_kind = EXPR_RVALUE(right)->kind_;
  if (left_kind == right_kind) {
    switch (left_kind) {
      case slrak_float:
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      FLOAT_REG_PTR_NRV(&dst->base_regs_, 0),
                      FLOAT_REG_PTR(left, 0),
                      FLOAT_REG_PTR(right, 0));
        break;
      case slrak_int:
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      INT_REG_PTR_NRV(&dst->base_regs_, 0),
                      INT_REG_PTR(left, 0),
                      INT_REG_PTR(right, 0));
        break;
      case slrak_vec2:
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      FLOAT_REG_PTR_NRV(&dst->base_regs_, 0),
                      FLOAT_REG_PTR(left, 0),
                      FLOAT_REG_PTR(right, 0));
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      FLOAT_REG_PTR_NRV(&dst->base_regs_, 1),
                      FLOAT_REG_PTR(left, 1),
                      FLOAT_REG_PTR(right, 1));
        break;
      case slrak_vec3:
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      FLOAT_REG_PTR_NRV(&dst->base_regs_, 0),
                      FLOAT_REG_PTR(left, 0),
                      FLOAT_REG_PTR(right, 0));
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      FLOAT_REG_PTR_NRV(&dst->base_regs_, 1),
                      FLOAT_REG_PTR(left, 1),
                      FLOAT_REG_PTR(right, 1));
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      FLOAT_REG_PTR_NRV(&dst->base_regs_, 2),
                      FLOAT_REG_PTR(left, 2),
                      FLOAT_REG_PTR(right, 2));
        break;
      case slrak_vec4:
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      FLOAT_REG_PTR_NRV(&dst->base_regs_, 0),
                      FLOAT_REG_PTR(left, 0),
                      FLOAT_REG_PTR(right, 0));
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      FLOAT_REG_PTR_NRV(&dst->base_regs_, 1),
                      FLOAT_REG_PTR(left, 1),
                      FLOAT_REG_PTR(right, 1));
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      FLOAT_REG_PTR_NRV(&dst->base_regs_, 2),
                      FLOAT_REG_PTR(left, 2),
                      FLOAT_REG_PTR(right, 2));
        sl_exec_f_mul(row, exec->exec_chain_reg_,
                      FLOAT_REG_PTR_NRV(&dst->base_regs_, 3),
                      FLOAT_REG_PTR(left, 3),
                      FLOAT_REG_PTR(right, 3));
        break;
      case slrak_ivec2:
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      INT_REG_PTR_NRV(&dst->base_regs_, 0),
                      INT_REG_PTR(left, 0),
                      INT_REG_PTR(right, 0));
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      INT_REG_PTR_NRV(&dst->base_regs_, 1),
                      INT_REG_PTR(left, 1),
                      INT_REG_PTR(right, 1));
        break;
      case slrak_ivec3:
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      INT_REG_PTR_NRV(&dst->base_regs_, 0),
                      INT_REG_PTR(left, 0),
                      INT_REG_PTR(right, 0));
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      INT_REG_PTR_NRV(&dst->base_regs_, 1),
                      INT_REG_PTR(left, 1),
                      INT_REG_PTR(right, 1));
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      INT_REG_PTR_NRV(&dst->base_regs_, 2),
                      INT_REG_PTR(left, 2),
                      INT_REG_PTR(right, 2));
        break;
      case slrak_ivec4:
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      INT_REG_PTR_NRV(&dst->base_regs_, 0),
                      INT_REG_PTR(left, 0),
                      INT_REG_PTR(right, 0));
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      INT_REG_PTR_NRV(&dst->base_regs_, 1),
                      INT_REG_PTR(left, 1),
                      INT_REG_PTR(right, 1));
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      INT_REG_PTR_NRV(&dst->base_regs_, 2),
                      INT_REG_PTR(left, 2),
                      INT_REG_PTR(right, 2));
        sl_exec_i_mul(row, exec->exec_chain_reg_,
                      INT_REG_PTR_NRV(&dst->base_regs_, 3),
                      INT_REG_PTR(left, 3),
                      INT_REG_PTR(right, 3));
        break;
      case slrak_mat2:
        for (c = 0; c < 2; ++c) {
          for (r = 0; r < 2; ++r) {
            sl_exec_f_dot_product2(row, exec->exec_chain_reg_,
                                   FLOAT_REG_PTR_NRV(&dst->base_regs_, c*2+r),
                                   FLOAT_REG_PTR(left, r+0),
                                   FLOAT_REG_PTR(left, r+2),
                                   FLOAT_REG_PTR(right, c*2 + 0),                                
                                   FLOAT_REG_PTR(right, c*2 + 1));
          }
        }
        break;
      case slrak_mat3:
        for (c = 0; c < 3; ++c) {
          for (r = 0; r < 3; ++r) {
            sl_exec_f_dot_product3(row, exec->exec_chain_reg_,
                                   FLOAT_REG_PTR_NRV(&dst->base_regs_, c*3+r),
                                   FLOAT_REG_PTR(left, r+0),
                                   FLOAT_REG_PTR(left, r+3),
                                   FLOAT_REG_PTR(left, r+6),
                                   FLOAT_REG_PTR(right, c*3 + 0),                                
                                   FLOAT_REG_PTR(right, c*3 + 1),
                                   FLOAT_REG_PTR(right, c*3 + 2));
          }
        }
        break;
      case slrak_mat4:
        for (c = 0; c < 4; ++c) {
          for (r = 0; r < 4; ++r) {
            sl_exec_f_dot_product4(row, exec->exec_chain_reg_,
                                   FLOAT_REG_PTR_NRV(&dst->base_regs_, c*4+r),
                                   FLOAT_REG_PTR(left, r+0),
                                   FLOAT_REG_PTR(left, r+4),
                                   FLOAT_REG_PTR(left, r+8),
                                   FLOAT_REG_PTR(left, r+12),
                                   FLOAT_REG_PTR(right, c*4 + 0),                                
                                   FLOAT_REG_PTR(right, c*4 + 1),
                                   FLOAT_REG_PTR(right, c*4 + 2),
                                   FLOAT_REG_PTR(right, c*4 + 3));
          }
        }
        break;
    }
  }
  else if (left_kind == slrak_float) {
    int num_coords = 0;
    switch (right_kind) {
      case slrak_vec2: num_coords = 2; break;
      case slrak_vec3: num_coords = 3; break;
      case slrak_vec4: num_coords = 4; break;
      case slrak_mat2: num_coords = 4; break;
      case slrak_mat3: num_coords = 9; break;
      case slrak_mat4: num_coords = 16; break;
    }
    for (r = 0; r < num_coords; ++r) {
      sl_exec_f_mul(row, exec->exec_chain_reg_,
                    FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                    FLOAT_REG_PTR(left, 0),
                    FLOAT_REG_PTR(right, r));
    }
  }
  else if (right_kind == slrak_float) {
    int num_coords = 0;
    switch (left_kind) {
      case slrak_vec2: num_coords = 2; break;
      case slrak_vec3: num_coords = 3; break;
      case slrak_vec4: num_coords = 4; break;
      case slrak_mat2: num_coords = 4; break;
      case slrak_mat3: num_coords = 9; break;
      case slrak_mat4: num_coords = 16; break;
    }
    for (r = 0; r < num_coords; ++r) {
      sl_exec_f_mul(row, exec->exec_chain_reg_,
                    FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                    FLOAT_REG_PTR(left, r),
                    FLOAT_REG_PTR(right, 0));
    }
  }
  else if (left_kind == slrak_int) {
    int num_coords = 0;
    switch (right_kind) {
      case slrak_ivec2: num_coords = 2; break;
      case slrak_ivec3: num_coords = 3; break;
      case slrak_ivec4: num_coords = 4; break;
    }
    for (r = 0; r < num_coords; ++r) {
      sl_exec_i_mul(row, exec->exec_chain_reg_,
                    INT_REG_PTR_NRV(&dst->base_regs_, r),
                    INT_REG_PTR(left, 0),
                    INT_REG_PTR(right, r));
    }
  }
  else if (right_kind == slrak_int) {
    int num_coords = 0;
    switch (left_kind) {
      case slrak_ivec2: num_coords = 2; break;
      case slrak_ivec3: num_coords = 3; break;
      case slrak_ivec4: num_coords = 4; break;
    }
    for (r = 0; r < num_coords; ++r) {
      sl_exec_i_mul(row, exec->exec_chain_reg_,
                    INT_REG_PTR_NRV(&dst->base_regs_, r),
                    INT_REG_PTR(left, r),
                    INT_REG_PTR(right, 0));
    }
  }
  else if ((left_kind == slrak_vec2) && (right_kind == slrak_mat2)) {
    for (r = 0; r < 2; ++r) {
      sl_exec_f_dot_product2(row, exec->exec_chain_reg_,
                             FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                             FLOAT_REG_PTR(left, 0),
                             FLOAT_REG_PTR(left, 1),
                             FLOAT_REG_PTR(right, 0 + r * 2),
                             FLOAT_REG_PTR(right, 1 + r * 2));
    }
  }
  else if ((left_kind == slrak_mat2) && (right_kind == slrak_vec2)) {
    for (r = 0; r < 2; ++r) {
      sl_exec_f_dot_product2(row, exec->exec_chain_reg_,
                             FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                             FLOAT_REG_PTR(left, 0 + r),
                             FLOAT_REG_PTR(left, 2 + r),
                             FLOAT_REG_PTR(right, 0),
                             FLOAT_REG_PTR(right, 1));
    }
  }
  else if ((left_kind == slrak_vec3) && (right_kind == slrak_mat3)) {
    for (r = 0; r < 3; ++r) {
      sl_exec_f_dot_product3(row, exec->exec_chain_reg_,
                             FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                             FLOAT_REG_PTR(left, 0),
                             FLOAT_REG_PTR(left, 1),
                             FLOAT_REG_PTR(left, 2),
                             FLOAT_REG_PTR(right, 0 + r * 3),
                             FLOAT_REG_PTR(right, 1 + r * 3),
                             FLOAT_REG_PTR(right, 2 + r * 3));
    }
  }
  else if ((left_kind == slrak_mat3) && (right_kind == slrak_vec3)) {
    for (r = 0; r < 3; ++r) {
      sl_exec_f_dot_product3(row, exec->exec_chain_reg_,
                             FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                             FLOAT_REG_PTR(left, 0 + r),
                             FLOAT_REG_PTR(left, 3 + r),
                             FLOAT_REG_PTR(left, 6 + r),
                             FLOAT_REG_PTR(right, 0),
                             FLOAT_REG_PTR(right, 1),
                             FLOAT_REG_PTR(right, 2));
    }
  }
  else if ((left_kind == slrak_vec4) && (right_kind == slrak_mat4)) {
    for (r = 0; r < 4; ++r) {
      sl_exec_f_dot_product4(row, exec->exec_chain_reg_,
                             FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                             FLOAT_REG_PTR(left, 0),
                             FLOAT_REG_PTR(left, 1),
                             FLOAT_REG_PTR(left, 2),
                             FLOAT_REG_PTR(left, 3),
                             FLOAT_REG_PTR(right, 0 + r * 4),
                             FLOAT_REG_PTR(right, 1 + r * 4),
                             FLOAT_REG_PTR(right, 2 + r * 4),
                             FLOAT_REG_PTR(right, 3 + r * 4));
    }
  }
  else if ((left_kind == slrak_mat4) && (right_kind == slrak_vec4)) {
    for (r = 0; r < 4; ++r) {
      sl_exec_f_dot_product4(row, exec->exec_chain_reg_,
                             FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                             FLOAT_REG_PTR(left, 0 + r),
                             FLOAT_REG_PTR(left, 4 + r),
                             FLOAT_REG_PTR(left, 8 + r),
                             FLOAT_REG_PTR(left, 12 + r),
                             FLOAT_REG_PTR(right, 0),
                             FLOAT_REG_PTR(right, 1),
                             FLOAT_REG_PTR(right, 2),
                             FLOAT_REG_PTR(right, 3));
    }
  }
}

static void sl_exec_div(struct sl_execution *exec, uint8_t row, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  int r;

  sl_reg_alloc_kind_t left_kind = EXPR_RVALUE(left)->kind_;
  sl_reg_alloc_kind_t right_kind = EXPR_RVALUE(right)->kind_;

  if (left_kind == right_kind) {
    switch (left_kind) {
      case slrak_float:
      case slrak_vec2:
      case slrak_vec3:
      case slrak_vec4:
      case slrak_mat2:
      case slrak_mat3:
      case slrak_mat4: {
        int num_components = 0;
        switch (left_kind) {
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
                        FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                        FLOAT_REG_PTR(left, r),
                        FLOAT_REG_PTR(right, r));
        }
        break;
      }
      case slrak_int:
      case slrak_ivec2:
      case slrak_ivec3:
      case slrak_ivec4: {
        int num_components = 0;
        switch (left_kind) {
          case slrak_int:   num_components = 1; break;
          case slrak_ivec2: num_components = 2; break;
          case slrak_ivec3: num_components = 3; break;
          case slrak_ivec4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_exec_i_div(row, exec->exec_chain_reg_, 
                        INT_REG_PTR_NRV(&dst->base_regs_, r),
                        INT_REG_PTR(left, r),
                        INT_REG_PTR(right, r));
        }
        break;
      }
    }
  }
  else if (left_kind == slrak_float) {
    int num_components = 0;
    switch (right_kind) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_f_div(row, exec->exec_chain_reg_, 
                    FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                    FLOAT_REG_PTR(left, 0),
                    FLOAT_REG_PTR(right, r));
    }
  }
  else if (right_kind == slrak_float) {
    int num_components = 0;
    switch (left_kind) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_f_div(row, exec->exec_chain_reg_, 
                    FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                    FLOAT_REG_PTR(left, r),
                    FLOAT_REG_PTR(right, 0));
    }
  }
  else if (left_kind == slrak_int) {
    int num_components = 0;
    switch (right_kind) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_i_div(row, exec->exec_chain_reg_,
                    INT_REG_PTR_NRV(&dst->base_regs_, r),
                    INT_REG_PTR(left, 0),
                    INT_REG_PTR(right, r));
    }
  }
  else if (right_kind == slrak_int) {
    int num_components = 0;
    switch (left_kind) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_i_div(row, exec->exec_chain_reg_,
                    INT_REG_PTR_NRV(&dst->base_regs_, r),
                    INT_REG_PTR(left, r),
                    INT_REG_PTR(right, 0));
    }
  }
}

static void sl_exec_add(struct sl_execution *exec, uint8_t row, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  int r;

  sl_reg_alloc_kind_t left_kind = EXPR_RVALUE(left)->kind_;
  sl_reg_alloc_kind_t right_kind = EXPR_RVALUE(right)->kind_;
  if (left_kind == right_kind) {
    switch (left_kind) {
      case slrak_float:
      case slrak_vec2:
      case slrak_vec3:
      case slrak_vec4:
      case slrak_mat2:
      case slrak_mat3:
      case slrak_mat4: {
        int num_components = 0;
        switch (left_kind) {
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
                        FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                        FLOAT_REG_PTR(left, r),
                        FLOAT_REG_PTR(right, r));
        }
        break;
      }
      case slrak_int:
      case slrak_ivec2:
      case slrak_ivec3:
      case slrak_ivec4: {
        int num_components = 0;
        switch (left_kind) {
          case slrak_int:   num_components = 1; break;
          case slrak_ivec2: num_components = 2; break;
          case slrak_ivec3: num_components = 3; break;
          case slrak_ivec4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_exec_i_add(row, exec->exec_chain_reg_, 
                        INT_REG_PTR_NRV(&dst->base_regs_, r),
                        INT_REG_PTR(left, r),
                        INT_REG_PTR(right, r));
        }
        break;
      }
    }
  }
  else if (left_kind == slrak_float) {
    int num_components = 0;
    switch (right_kind) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_f_add(row, exec->exec_chain_reg_, 
                    FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                    FLOAT_REG_PTR(left, 0),
                    FLOAT_REG_PTR(right, r));
    }
  }
  else if (right_kind == slrak_float) {
    int num_components = 0;
    switch (left_kind) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_f_add(row, exec->exec_chain_reg_, 
                    FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                    FLOAT_REG_PTR(left, r),
                    FLOAT_REG_PTR(right, 0));
    }
  }
  else if (left_kind == slrak_int) {
    int num_components = 0;
    switch (right_kind) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_i_add(row, exec->exec_chain_reg_,
                    INT_REG_PTR_NRV(&dst->base_regs_, r),
                    INT_REG_PTR(left, 0),
                    INT_REG_PTR(right, r));
    }
  }
  else if (right_kind == slrak_int) {
    int num_components = 0;
    switch (left_kind) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_i_add(row, exec->exec_chain_reg_,
                    INT_REG_PTR_NRV(&dst->base_regs_, r),
                    INT_REG_PTR(left, r),
                    INT_REG_PTR(right, 0));
    }
  }
}

static void sl_exec_sub(struct sl_execution *exec, uint8_t row, struct sl_expr *dst, struct sl_expr *left, struct sl_expr *right) {
  int r;

  sl_reg_alloc_kind_t left_kind = EXPR_RVALUE(left)->kind_;
  sl_reg_alloc_kind_t right_kind = EXPR_RVALUE(right)->kind_;
  if (left_kind == right_kind) {
    switch (left_kind) {
      case slrak_float:
      case slrak_vec2:
      case slrak_vec3:
      case slrak_vec4:
      case slrak_mat2:
      case slrak_mat3:
      case slrak_mat4: {
        int num_components = 0;
        switch (left_kind) {
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
                        FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                        FLOAT_REG_PTR(left, r),
                        FLOAT_REG_PTR(right, r));
        }
        break;
      }
      case slrak_int:
      case slrak_ivec2:
      case slrak_ivec3:
      case slrak_ivec4: {
        int num_components = 0;
        switch (left_kind) {
          case slrak_int:   num_components = 1; break;
          case slrak_ivec2: num_components = 2; break;
          case slrak_ivec3: num_components = 3; break;
          case slrak_ivec4: num_components = 4; break;
        }
        for (r = 0; r < num_components; ++r) {
          sl_exec_i_sub(row, exec->exec_chain_reg_, 
                        INT_REG_PTR_NRV(&dst->base_regs_, r),
                        INT_REG_PTR(left, r),
                        INT_REG_PTR(right, r));
        }
        break;
      }
    }
  }
  else if (left_kind == slrak_float) {
    int num_components = 0;
    switch (right_kind) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_f_sub(row, exec->exec_chain_reg_, 
                    FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                    FLOAT_REG_PTR(left, 0),
                    FLOAT_REG_PTR(right, r));
    }
  }
  else if (right_kind == slrak_float) {
    int num_components = 0;
    switch (left_kind) {
      case slrak_vec2: num_components = 2; break;
      case slrak_vec3: num_components = 3; break;
      case slrak_vec4: num_components = 4; break;
      case slrak_mat2: num_components = 2; break;
      case slrak_mat3: num_components = 3; break;
      case slrak_mat4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_f_sub(row, exec->exec_chain_reg_, 
                    FLOAT_REG_PTR_NRV(&dst->base_regs_, r),
                    FLOAT_REG_PTR(left, r),
                    FLOAT_REG_PTR(right, 0));
    }
  }
  else if (left_kind == slrak_int) {
    int num_components = 0;
    switch (right_kind) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_i_sub(row, exec->exec_chain_reg_,
                        INT_REG_PTR_NRV(&dst->base_regs_, r),
                        INT_REG_PTR(left, 0),
                        INT_REG_PTR(right, r));
    }
  }
  else if (right_kind == slrak_int) {
    int num_components = 0;
    switch (left_kind) {
      case slrak_ivec2: num_components = 2; break;
      case slrak_ivec3: num_components = 3; break;
      case slrak_ivec4: num_components = 4; break;
    }
    for (r = 0; r < num_components; ++r) {
      sl_exec_i_sub(row, exec->exec_chain_reg_,
                        INT_REG_PTR_NRV(&dst->base_regs_, r),
                        INT_REG_PTR(left, r),
                        INT_REG_PTR(right, 0));
    }
  }
}


/* 
 * Goal: src_ra contains a multi-component scalar type (e.g. vec, mat, ivec, bvec); src_index_ra contains an integer.
 *       We use the integer from src_index_ra to select which component from src_ra we need. However, we don't load
 *       the component's value, rather, we store the component register holding the value in the integer pointed to
 *       by dst_ra.
 * Note: src_index_ra might be local ; if so we need to offset before we access...
 *       src_ra might be local ; if so, we need to offset and convert the register index value to a global index before we store.
 *       dst_ra might be local ; if so, we need to offset before we access...
 */

static void sl_exec_load_effective_reg_index(struct sl_execution *exec, uint8_t row, struct sl_reg_alloc *dst_ra, const struct sl_reg_alloc *src_ra, const struct sl_reg_alloc *src_index_ra) {
  uint8_t * restrict chain_column = exec->exec_chain_reg_;

  int column_size = 0; /* column size of 0 means "no column" (but instead selecting a scalar from a vec) */
  switch (src_ra->kind_) {
    case slrak_mat2:
      column_size = 2;
      break;
    case slrak_mat3:
      column_size = 3;
      break;
    case slrak_mat4:
      column_size = 4;
      break;
  }

  if (!src_ra->is_indirect_) {
    if (src_ra->local_frame_) {
      int64_t global_offset = 0;
      switch (dst_ra->kind_) {
        case slrak_float:
        case slrak_vec2:
        case slrak_vec3:
        case slrak_vec4:
        case slrak_mat2:
        case slrak_mat3:
        case slrak_mat4:
          global_offset = exec->execution_frames_[exec->num_execution_frames_ - 1].local_float_offset_;
          break;
        case slrak_int:
        case slrak_ivec2:
        case slrak_ivec3:
        case slrak_ivec4:
          global_offset = exec->execution_frames_[exec->num_execution_frames_ - 1].local_int_offset_;
          break;
        case slrak_bool:
        case slrak_bvec2:
        case slrak_bvec3:
        case slrak_bvec4:
          global_offset = exec->execution_frames_[exec->num_execution_frames_ - 1].local_bool_offset_;
          break;
        case slrak_sampler2D:
          global_offset = exec->execution_frames_[exec->num_execution_frames_ - 1].local_sampler2D_offset_;
          break;
        case slrak_samplerCube:
          global_offset = exec->execution_frames_[exec->num_execution_frames_ - 1].local_samplerCube_offset_;
          break;
      }
      if (!column_size) {
        int64_t *restrict result_column = INT_REG_PTR_NRV(dst_ra, 0);
        const int64_t *restrict opd_column = INT_REG_PTR_NRV(src_index_ra, 0);
#define UNOP_SNIPPET_TYPE int64_t
#define UNOP_SNIPPET_OPERATOR(opd) global_offset + src_ra->v_.regs_[ (opd) ];
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
      }
      else {
        /* Matrix */
        int k;
        for (k = 0; k < column_size; ++k) {
          int64_t *restrict result_column = INT_REG_PTR_NRV(dst_ra, k);
          const int64_t *restrict opd_column = INT_REG_PTR_NRV(src_index_ra, 0);

#define UNOP_SNIPPET_TYPE int64_t
#define UNOP_SNIPPET_OPERATOR(opd) global_offset + src_ra->v_.regs_[ (opd) * (int64_t)column_size + k ];
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
        }
      }
    }
    else {
      if (!column_size) {
        int64_t *restrict result_column = INT_REG_PTR_NRV(dst_ra, 0);
        const int64_t *restrict opd_column = INT_REG_PTR_NRV(src_index_ra, 0);
#define UNOP_SNIPPET_TYPE int64_t
#define UNOP_SNIPPET_OPERATOR(opd) src_ra->v_.regs_[ (opd) ];
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
      }
      else {
        /* Matrix */
        int k;
        for (k = 0; k < column_size; ++k) {
          int64_t *restrict result_column = INT_REG_PTR_NRV(dst_ra, k);
          const int64_t *restrict opd_column = INT_REG_PTR_NRV(src_index_ra, 0);

#define UNOP_SNIPPET_TYPE int64_t
#define UNOP_SNIPPET_OPERATOR(opd) src_ra->v_.regs_[ (opd) * (int64_t)column_size + k ];
#include "sl_unop_snippet_inc.h"
#undef UNOP_SNIPPET_OPERATOR
#undef UNOP_SNIPPET_TYPE
        }
      }
    }
  }
  else {
    /* Vec or matrix we're indexing from is already indirect */

    int64_t * restrict component_indirect_columns[16];
    size_t n;
    int64_t global_offset = 0;
    if (src_ra->local_frame_) {
      global_offset = exec->execution_frames_[exec->num_execution_frames_ - 1].local_int_offset_;
    }
    for (n = 0; n < 16; ++n) {
      component_indirect_columns[n] = exec->int_regs_[ global_offset + src_ra->v_.regs_[n] ];
    }
    if (!column_size) {
      int64_t *restrict result_column = INT_REG_PTR_NRV(dst_ra, 0);
      const int64_t *restrict opd_column = INT_REG_PTR_NRV(src_index_ra, 0);

      for (;;) {
        uint64_t chain;
        uint8_t delta;
        if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
          do {
            int64_t *restrict result = result_column + row;
            const int64_t *restrict opd = opd_column + row;
            int n;
            /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
            for (n = 0; n < 8; n++) {
              result[n] = (component_indirect_columns[(opd[n])])[n + row];
            }

            delta = (chain & 0xFF00000000000000) >> 56;
            if (!delta) break;
            row += 7 + delta;
          } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
        }
        else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
          do {
            int64_t *restrict result = result_column + row;
            const int64_t *restrict opd = opd_column + row;
            int n;
            /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
            for (n = 0; n < 4; n++) {
              result[n] = (component_indirect_columns[(opd[n])])[n + row];
            }
            delta = (chain & 0xFF000000) >> 24;
            if (!delta) break;
            row += 3 + delta;
          } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
        }
        else {
          do {
            /* Not trying to evoke auto-vectorization, just get it done. */
            result_column[row] = (component_indirect_columns[(opd_column[row])])[row];
            delta = chain_column[row];
            if (!delta) break;
            row += delta;
          } while (row & 3);
        }
        if (!delta) break;
      }
    }
    else {
      /* Matrix */
      int k;
      for (k = 0; k < column_size; ++k) {
        int64_t *restrict result_column = INT_REG_PTR_NRV(dst_ra, k);
        const int64_t *restrict opd_column = INT_REG_PTR_NRV(src_index_ra, 0);

        for (;;) {
          uint64_t chain;
          uint8_t delta;
          if (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101)) {
            do {
              int64_t *restrict result = result_column + row;
              const int64_t *restrict opd = opd_column + row;
              int n;
              /* Try to elicit 8-wise SIMD instructions from auto-vectorization, e.g. AVX's VMULPS ymm0, ymm1, ymm2 */
              for (n = 0; n < 8; n++) {
                result[n] = (component_indirect_columns[column_size * (opd[n]) + k])[n + row];
              }

              delta = (chain & 0xFF00000000000000) >> 56;
              if (!delta) break;
              row += 7 + delta;
            } while (!(row & 7) && (((chain = *(uint64_t *)(chain_column + row)) & 0xFFFFFFFFFFFFFFULL) == 0x01010101010101));
          }
          else if (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101)) {
            do {
              int64_t *restrict result = result_column + row;
              const int64_t *restrict opd = opd_column + row;
              int n;
              /* Try to elicit forth 4-wise SIMD instructions from auto-vectorization, e.g. SSE's MULPS xmm0, xmm1 */
              for (n = 0; n < 4; n++) {
                result[n] = (component_indirect_columns[column_size * (opd[n]) + k])[n + row];
              }
              delta = (chain & 0xFF000000) >> 24;
              if (!delta) break;
              row += 3 + delta;
            } while (!(row & 3) && (((chain = *(uint32_t *)(chain_column + row)) & 0xFFFFFF) == 0x010101));
          }
          else {
            do {
              /* Not trying to evoke auto-vectorization, just get it done. */
              result_column[row] = (component_indirect_columns[column_size * (opd_column[row]) + k])[row];
              delta = chain_column[row];
              if (!delta) break;
              row += delta;
            } while (row & 3);
          }
          if (!delta) break;
        }
      }
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
  exec->cu_ = NULL;
  exec->max_num_rows_ = 0;
  exec->dump_text_ = NULL;
  exec->dump_text_len_ = 0;
  exec->num_execution_points_ = exec->num_execution_points_allocated_ = 0;
  exec->execution_points_ = NULL;
  exec->num_execution_frames_ = exec->num_execution_frames_allocated_ = 0;
  exec->execution_frames_ = NULL;
  exec->exec_chain_reg_ = NULL;
  exec->num_float_regs_ = 0;
  exec->float_regs_ = NULL;
  exec->num_int_regs_ = 0;
  exec->int_regs_ = NULL;
  exec->num_bool_regs_ = 0;
  exec->bool_regs_ = NULL;
  exec->num_sampler_2D_regs_ = 0;
  exec->sampler_2D_regs_ = NULL;
  exec->num_sampler_cube_regs_ = 0;
  exec->sampler_cube_regs_ = NULL;
}

void sl_exec_cleanup(struct sl_execution *exec) {
  if (exec->dump_text_) free(exec->dump_text_);
  if (exec->execution_points_) free(exec->execution_points_);
  if (exec->execution_frames_) free(exec->execution_frames_);
  if (exec->exec_chain_reg_) free(exec->exec_chain_reg_);
  if (exec->float_regs_) free(exec->float_regs_);
  if (exec->int_regs_) free(exec->int_regs_);
  if (exec->bool_regs_) free(exec->bool_regs_);
  if (exec->sampler_2D_regs_) free(exec->sampler_2D_regs_);
  if (exec->sampler_cube_regs_) free(exec->sampler_cube_regs_);
}

int sl_exec_prep(struct sl_execution *exec, struct sl_compilation_unit *cu) {
  void *new_float_regs = NULL;
  void *new_int_regs = NULL;
  void *new_bool_regs = NULL;
  void *new_sampler2D_regs = NULL;
  void *new_samplerCube_regs = NULL;
  if (cu->register_counts_.num_float_regs_) {
    new_float_regs = malloc(sizeof(float *) * cu->register_counts_.num_float_regs_);
    if (!new_float_regs) goto fail;
    memset(new_float_regs, 0, sizeof(float *) * cu->register_counts_.num_float_regs_);
  }
  if (cu->register_counts_.num_int_regs_) {
    new_int_regs = malloc(sizeof(int64_t *) * cu->register_counts_.num_int_regs_);
    if (!new_int_regs) goto fail;
    memset(new_int_regs, 0, sizeof(int64_t *) * cu->register_counts_.num_int_regs_);
  }
  if (cu->register_counts_.num_bool_regs_) {
    new_bool_regs = malloc(sizeof(unsigned char *) * cu->register_counts_.num_bool_regs_);
    if (!new_bool_regs) goto fail;
    memset(new_bool_regs, 0, sizeof(int *) * cu->register_counts_.num_bool_regs_);
  }
  if (cu->register_counts_.num_sampler2D_regs_) {
    new_sampler2D_regs = malloc(sizeof(void *) * cu->register_counts_.num_sampler2D_regs_);
    if (!new_sampler2D_regs) goto fail;
    memset(new_sampler2D_regs, 0, sizeof(void *) * cu->register_counts_.num_sampler2D_regs_);
  }
  if (cu->register_counts_.num_samplerCube_regs_) {
    new_samplerCube_regs = malloc(sizeof(void *) * cu->register_counts_.num_samplerCube_regs_);
    if (!new_samplerCube_regs) goto fail;
    memset(new_samplerCube_regs, 0, sizeof(void *) * cu->register_counts_.num_samplerCube_regs_);
  }

  if (exec->float_regs_) free(exec->float_regs_);
  if (exec->int_regs_) free(exec->int_regs_);
  if (exec->bool_regs_) free(exec->bool_regs_);
  if (exec->sampler_2D_regs_) free(exec->sampler_2D_regs_);
  if (exec->sampler_cube_regs_) free(exec->sampler_cube_regs_);

  exec->cu_ = cu;

  exec->num_float_regs_ = (size_t)cu->register_counts_.num_float_regs_;
  exec->float_regs_ = (float **)new_float_regs;

  exec->num_int_regs_ = (size_t)cu->register_counts_.num_int_regs_;
  exec->int_regs_ = (int64_t **)new_int_regs;

  exec->num_bool_regs_ = (size_t)cu->register_counts_.num_bool_regs_;
  exec->bool_regs_ = (unsigned char **)new_bool_regs;

  exec->num_sampler_2D_regs_ = (size_t)cu->register_counts_.num_sampler2D_regs_;
  exec->sampler_2D_regs_ = (void ***)new_sampler2D_regs;

  exec->num_sampler_cube_regs_ = (size_t)cu->register_counts_.num_samplerCube_regs_;
  exec->sampler_cube_regs_ = (void ***)new_samplerCube_regs;

  /* Set exernally_initialized_ to 1 for all varyings, attributes or uniforms */
  struct sl_variable *v;
  v = exec->cu_->global_frame_.variables_;
  if (v) {
    do {
      v = v->chain_;

      int quali = sl_type_qualifiers(v->type_);
      if ((quali & SL_TYPE_QUALIFIER_ATTRIBUTE) ||
          (quali & SL_TYPE_QUALIFIER_VARYING) ||
          (quali & SL_TYPE_QUALIFIER_UNIFORM)) {
        /* Do not initialize - the compiler should already have prevented the presence
         * of an initializer in the code, but the internal intializer (in v->initializer_)
         * must also not be used. Not just because it is wasteful, but also because it
         * would overwrite any externally set values of the variable. */
        v->is_externally_initialized_ = 1;
      }

    } while (v != exec->cu_->global_frame_.variables_);
  }


  return 0;
fail:
  if (new_float_regs) free(new_float_regs);
  if (new_int_regs) free(new_int_regs);
  if (new_bool_regs) free(new_bool_regs);
  if (new_sampler2D_regs) free(new_sampler2D_regs);
  if (new_samplerCube_regs) free(new_samplerCube_regs);

  return -1;
}

int sl_exec_allocate_registers_by_slab(struct sl_execution *exec, size_t max_num_rows) {
  float *float_bank = NULL;
  int64_t *int_bank = NULL;
  uint8_t *bool_bank = NULL;
  void **sampler2D_bank = NULL;
  void **samplerCube_bank = NULL;
  uint8_t *chain_bank = NULL;

  size_t num_rows = max_num_rows;

  size_t slab_size = 0;
  slab_size += sizeof(float) * num_rows * exec->num_float_regs_;
  slab_size += sizeof(int64_t) * num_rows * exec->num_int_regs_;
  slab_size += sizeof(uint8_t) * num_rows * exec->num_bool_regs_;
  slab_size += sizeof(void *) * num_rows * exec->num_sampler_2D_regs_;
  slab_size += sizeof(void *) * num_rows * exec->num_sampler_cube_regs_;

  char *slab = (char *)malloc(slab_size);
  if (!slab) return -1;
  uint8_t *exec_regs = (uint8_t *)malloc(sizeof(uint8_t) * num_rows);
  if (!exec_regs) {
    free(slab);
    return -1;
  }

  char *pslab = slab;

  if (exec->num_float_regs_) { float_bank = (float *)pslab; pslab += sizeof(float) * num_rows * exec->num_float_regs_; }
  if (exec->num_int_regs_) { int_bank = (int64_t *)pslab; pslab += sizeof(int64_t) * num_rows * exec->num_int_regs_; }
  if (exec->num_bool_regs_) { bool_bank = (uint8_t *)pslab; pslab += sizeof(uint8_t) * num_rows * exec->num_bool_regs_; }
  if (exec->num_sampler_2D_regs_) { sampler2D_bank = (void **)pslab; pslab += sizeof(void *) * num_rows * exec->num_sampler_2D_regs_; }
  if (exec->num_sampler_cube_regs_) { samplerCube_bank = (void **)pslab; pslab += sizeof(void *) * num_rows * exec->num_sampler_cube_regs_; }
  chain_bank = (uint8_t *)pslab;

  if (exec->float_regs_) exec->float_regs_[0] = float_bank;
  if (exec->int_regs_) exec->int_regs_[0] = int_bank;
  if (exec->bool_regs_) exec->bool_regs_[0] = bool_bank;
  if (exec->sampler_2D_regs_) exec->sampler_2D_regs_[0] = sampler2D_bank;
  if (exec->sampler_cube_regs_) exec->sampler_cube_regs_[0] = samplerCube_bank;
  exec->exec_chain_reg_ = exec_regs;
  size_t n;

  memset(exec->exec_chain_reg_, 0, num_rows * sizeof(uint8_t));

  for (n = 1; n < exec->num_float_regs_; n++) {
    exec->float_regs_[n] = exec->float_regs_[n - 1] + num_rows;
  }
  for (n = 1; n < exec->num_int_regs_; n++) {
    exec->int_regs_[n] = exec->int_regs_[n - 1] + num_rows;
  }
  for (n = 1; n < exec->num_bool_regs_; n++) {
    exec->bool_regs_[n] = exec->bool_regs_[n - 1] + num_rows;
  }
  for (n = 1; n < exec->num_sampler_2D_regs_; n++) {
    exec->sampler_2D_regs_[n] = exec->sampler_2D_regs_[n - 1] + num_rows;
  }
  for (n = 1; n < exec->num_sampler_cube_regs_; n++) {
    exec->sampler_cube_regs_[n] = exec->sampler_cube_regs_[n - 1] + num_rows;
  }

  exec->max_num_rows_ = num_rows;
  exec->slab_ = slab;
  return 0;
}

void sl_exec_set_expr(struct sl_execution *exec, size_t ep_index, struct sl_expr *expr, uint32_t chain, size_t continuation_ptr) {
  sl_exec_clear_ep(exec, ep_index);
  struct sl_execution_point *ep = exec->execution_points_ + ep_index;
  ep->kind_ = SLEPK_EXPR;
  ep->v_.expr_ = expr;
  ep->enter_chain_ = chain;
  ep->continue_chain_ptr_ = continuation_ptr;
}

void sl_exec_set_stmt(struct sl_execution *exec, size_t ep_index, struct sl_stmt *stmt, uint32_t chain, size_t continuation_ptr) {
  sl_exec_clear_ep(exec, ep_index);
  struct sl_execution_point *ep = exec->execution_points_ + ep_index;
  ep->kind_ = SLEPK_STMT;
  ep->v_.stmt_ = stmt;
  ep->enter_chain_ = chain;
  ep->continue_chain_ptr_ = continuation_ptr;
}

void sl_exec_set_bootstrap(struct sl_execution *exec, size_t ep_index, struct sl_function *f, uint32_t chain, size_t continuation_ptr) {
  sl_exec_clear_ep(exec, ep_index);
  struct sl_execution_point *ep = exec->execution_points_ + ep_index;
  ep->kind_ = SLEPK_BOOTSTRAP;
  
  ep->v_.f_ = f;
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

int sl_exec_push_stmt(struct sl_execution *exec, struct sl_stmt *stmt, uint32_t chain, size_t continuation_ptr) {
  int r;
  r = sl_exec_reserve_n(exec, 1);
  if (r) return r;

  sl_exec_set_stmt(exec, exec->num_execution_points_++, stmt, chain, continuation_ptr);

  return 0;
}

int sl_exec_push_bootstrap(struct sl_execution *exec, struct sl_function *f, uint32_t chain, size_t continuation_ptr) {
  int r;
  r = sl_exec_reserve_n(exec, 1);
  if (r) return r;

  sl_exec_set_bootstrap(exec, exec->num_execution_points_++, f, chain, continuation_ptr);

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

void sl_exec_call_graph_results_init(struct sl_exec_call_graph_results *cgr) {
  cgr->num_execution_frames_ = 0;
  cgr->num_float_regs_ = 0;
  cgr->num_int_regs_ = 0;
  cgr->num_bool_regs_ = 0;
  cgr->num_sampler2D_regs_ = 0;
  cgr->num_samplerCube_regs_ = 0;
}

void sl_exec_call_graph_results_cleanup(struct sl_exec_call_graph_results *cgr) {
  cgr;
}

static int sl_exec_cga_expr(struct sl_exec_call_graph_results *cgr, struct sl_expr *x);

static int sl_exec_cga_stmt_list_impl(struct sl_exec_call_graph_results *cgr, struct sl_stmt *stmt_list) {
  struct sl_stmt *s;
  s = stmt_list;
  if (!s) return 0;
  for (;;) {
    struct sl_stmt *child;

    /* Enter s */
    int r = 0;
    if (s->expr_) {
      r = sl_exec_cga_expr(cgr, s->expr_);
    }
    if (!r && s->condition_) {
      r = sl_exec_cga_expr(cgr, s->condition_);
    }
    if (!r && s->post_) {
      r = sl_exec_cga_expr(cgr, s->post_);
    }
    if (r) {
      return r;
    }

    child = sl_stmt_first_child(s);
    if (child) {
      s = child;
    }
    else {
      /* Could not enter a child, we are at a leaf; find a sibling */
      struct sl_stmt *next = NULL;

      do {
        next = sl_stmt_next_sibling(s);

        /* Leaving s
          * { ... } */

        if (!next) {
          if (s->parent_ == stmt_list->parent_) {
            /* About to pop outside the statement list, we've reached the end. */
            return 0;
          }
          s = s->parent_;
        }
      } while (!next);
      s = next;
    }
  }
}

void sl_exec_cgr_max(struct sl_exec_call_graph_results *cgr, const struct sl_exec_call_graph_results *lcgr) {
  if (lcgr->num_execution_frames_ > cgr->num_execution_frames_)
    cgr->num_execution_frames_ = lcgr->num_execution_frames_;
  if (lcgr->num_float_regs_ > cgr->num_float_regs_)
    cgr->num_float_regs_ = lcgr->num_float_regs_;
  if (lcgr->num_int_regs_ > cgr->num_int_regs_)
    cgr->num_int_regs_ = lcgr->num_int_regs_;
  if (lcgr->num_bool_regs_ > cgr->num_bool_regs_)
    cgr->num_bool_regs_ = lcgr->num_bool_regs_;
  if (lcgr->num_sampler2D_regs_ > cgr->num_sampler2D_regs_)
    cgr->num_sampler2D_regs_ = lcgr->num_sampler2D_regs_;
  if (lcgr->num_samplerCube_regs_ > cgr->num_samplerCube_regs_)
    cgr->num_samplerCube_regs_ = lcgr->num_samplerCube_regs_;
}

void sl_exec_cgr_swap(struct sl_exec_call_graph_results *a, struct sl_exec_call_graph_results *b) {
  struct sl_exec_call_graph_results t = *a;
  *a = *b;
  *b = t;
}

static int sl_exec_cga_function_impl(struct sl_exec_call_graph_results *cgr, struct sl_function *f) {
  int r;
  if (f->visited_) return -1; /* invalid recursion */
  f->visited_ = 1;
  struct sl_exec_call_graph_results invocation_cgr = { 0 };
  r = sl_exec_cga_stmt_list_impl(&invocation_cgr, f->body_);
  f->visited_ = 0;
  if (r) return r;

  /* Relative to the calling context, this function invocation is the point where
   * we incur both the frame of the function to be called and the corresponding
   * need for an execution frame. */
  invocation_cgr.num_execution_frames_ += 1;
  invocation_cgr.num_float_regs_ += f->frame_.ract_.rra_floats_.watermark_;
  invocation_cgr.num_int_regs_ += f->frame_.ract_.rra_ints_.watermark_;
  invocation_cgr.num_bool_regs_ += f->frame_.ract_.rra_bools_.watermark_;
  invocation_cgr.num_sampler2D_regs_ += f->frame_.ract_.rra_sampler2D_.watermark_;
  invocation_cgr.num_samplerCube_regs_ += f->frame_.ract_.rra_samplerCube_.watermark_;

  sl_exec_cgr_max(cgr, &invocation_cgr);

  return 0;
}

static int sl_exec_cga_expr(struct sl_exec_call_graph_results *cgr, struct sl_expr *x) {
  int r;
  size_t n;

  for (n = 0; n < x->num_children_; ++n) {
    struct sl_exec_call_graph_results lcgr = { 0 };
    r = sl_exec_cga_expr(&lcgr, x->children_[n]);
    if (r) return r;
    sl_exec_cgr_max(cgr, &lcgr);
  }

  if (x->op_ == exop_function_call) {
    struct sl_function *f = x->function_;
    if (!f) {
      /* No function */
      return -1;
    }
    struct sl_exec_call_graph_results lcgr = { 0 };
    r = sl_exec_cga_function_impl(&lcgr, f);
    if (r) return r;
    sl_exec_cgr_max(cgr, &lcgr);
  }

  return 0;
}

int sl_exec_call_graph_analysis(struct sl_exec_call_graph_results *cgr, struct sl_function *f) {
  return sl_exec_cga_function_impl(cgr, f);
}

static int sl_exec_push_execution_frame(struct sl_execution *exec) {
  if (exec->num_execution_frames_ == exec->num_execution_frames_allocated_) {
    size_t new_num_allocated = exec->num_execution_frames_allocated_ * 2 + 1;
    if (new_num_allocated <= exec->num_execution_frames_allocated_) {
      /* overflow */
      return -1;
    }
    if (new_num_allocated >= (SIZE_MAX / sizeof(struct sl_execution_frame))) {
      /* overflow */
      return -1;
    }
    size_t new_size = new_num_allocated * sizeof(struct sl_execution_frame);
    struct sl_execution_frame *new_execution_frames = (struct sl_execution_frame *)realloc(exec->execution_frames_, new_size);
    if (!new_execution_frames) {
      /* no memory */
      return -1;
    }
    exec->execution_frames_ = new_execution_frames;
    exec->num_execution_frames_allocated_ = new_num_allocated;
  }
  struct sl_execution_frame *ef = exec->execution_frames_ + exec->num_execution_frames_++;
  ef->f_ = NULL;
  if (exec->num_execution_frames_ == 1) {
    ef->local_float_offset_ = 0;
    ef->local_int_offset_ = 0;
    ef->local_bool_offset_ = 0;
    ef->local_sampler2D_offset_ = 0;
    ef->local_samplerCube_offset_ = 0;
  }
  else {
    /* initialize with previous frame's offsets; caller can then increment as necessary */
    struct sl_execution_frame *pef = ef - 1;
    ef->local_float_offset_ = pef->local_float_offset_;
    ef->local_int_offset_ = pef->local_int_offset_;
    ef->local_bool_offset_ = pef->local_bool_offset_;
    ef->local_sampler2D_offset_ = pef->local_sampler2D_offset_;
    ef->local_samplerCube_offset_ = pef->local_samplerCube_offset_;
  }
  return 0;
}

void sl_exec_pop_execution_frame(struct sl_execution *exec) {
  if (exec->num_execution_frames_) exec->num_execution_frames_--;
}

static void sl_exec_need_rvalue(struct sl_execution *exec, uint32_t chain, struct sl_expr *x) {
  sl_reg_move(exec, chain, &x->base_regs_, &x->offset_reg_, &x->rvalue_, NULL);
}

static void sl_exec_initialize_globals(struct sl_execution *exec) {
  struct sl_variable *v;
  v = exec->cu_->global_frame_.variables_;
  if (v) {
    do {
      v = v->chain_;

      if (v->is_externally_initialized_) {
        /* Don't initialize attributes as the caller will have initialized them */
      }
      else {
        sl_exec_init_literal(exec, 0, &v->reg_alloc_, &v->value_, 0);
      }

    } while (v != exec->cu_->global_frame_.variables_);
  }
}

static int sl_exec_prepare_run(struct sl_execution *exec, struct sl_function *f, int exec_chain) {
  int r = 0;
  exec->num_execution_frames_ = 0;
  r = sl_exec_push_execution_frame(exec);
  if (r) return r;

  struct sl_execution_frame *ef = exec->execution_frames_;
  ef->f_ = f;
  ef->local_float_offset_ = (int)exec->cu_->global_frame_.ract_.rra_floats_.watermark_;
  ef->local_int_offset_ = (int)exec->cu_->global_frame_.ract_.rra_ints_.watermark_;
  ef->local_bool_offset_ = (int)exec->cu_->global_frame_.ract_.rra_bools_.watermark_;
  ef->local_sampler2D_offset_ = (int)exec->cu_->global_frame_.ract_.rra_sampler2D_.watermark_;
  ef->local_samplerCube_offset_ = (int)exec->cu_->global_frame_.ract_.rra_samplerCube_.watermark_;

  r = sl_exec_push_bootstrap(exec, f, exec_chain, 0);
  if (r) return r;

  return 0;
}

int sl_exec_run(struct sl_execution *exec, struct sl_function *f, int exec_chain) {
  int r;
  sl_exec_initialize_globals(exec);
  r = sl_exec_prepare_run(exec, f, exec_chain);
  if (r) return r;
  while (exec->num_execution_points_) {
    /* Build "eps[epi]" shorthand for exec->execution_points_[exec->num_execution_points_ - 1] as we'll be referencing it, a lot. */
    size_t epi = exec->num_execution_points_ - 1;
    struct sl_execution_point *eps = exec->execution_points_;
#define CHAIN_REF(x) (((uintptr_t)&(x)) - (uintptr_t)exec->execution_points_)
    if (eps[epi].kind_ == SLEPK_STMT) {
      if (eps[epi].enter_chain_ != SL_EXEC_NO_CHAIN) {
        switch (eps[epi].v_.stmt_->kind_) {
          case slsk_expression: {
            r = sl_exec_push_expr(exec, eps[epi].v_.stmt_->expr_, eps[epi].enter_chain_, CHAIN_REF(eps[epi].post_chain_));
            if (r) return r;
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            break;  
          }
          case slsk_if:
            /* Evaluate the condition, then come back on the revisit chain to determine what to do */
            r = sl_exec_push_expr(exec, eps[epi].v_.stmt_->condition_, eps[epi].enter_chain_, CHAIN_REF(eps[epi].revisit_chain_));
            if (r) return r;
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            break;
          case slsk_while: {
            /* Evaluate the condition, then come back on the revisit chain to determine what to do
             * note that we have to potentially execute an initializer first, e.g.:
             *   while (bool b = true) { .. }
             * is valid. */
            size_t condition_eval_index = exec->num_execution_points_;
            r = sl_exec_push_expr(exec, eps[epi].v_.stmt_->condition_, SL_EXEC_NO_CHAIN, CHAIN_REF(eps[epi].revisit_chain_));
            if (r) return r;
            size_t condition_prep_stmt_index = exec->num_execution_points_;
            /* Check if we have a preparatory statement for the condition (an initializer) and execute it prior to the condition expression,
             * if so. Otherwise wire the enter_chain_ to directly go into the expression. */
            if (eps[epi].v_.stmt_->prep_cond_) {
              r = sl_exec_push_stmt(exec, eps[epi].v_.stmt_->prep_cond_, eps[epi].enter_chain_, CHAIN_REF(eps[condition_eval_index].enter_chain_));
              if (r) return r;
            }
            else {
              eps[condition_eval_index].enter_chain_ = eps[epi].enter_chain_;
            }
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;

            break;
          }
          case slsk_do: {
            /* Execute the body, then return onto the alt_chain_ to evaluate the condition */
            r = sl_exec_push_stmt(exec, eps[epi].v_.stmt_->true_branch_, eps[epi].enter_chain_, CHAIN_REF(eps[epi].alt_chain_));
            if (r) return r;
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            break;
          }
          case slsk_for:
            /* Execute preparatory statements, then execute the condition preparatory statements, and evaluate the condition */
            /* Execute preparatory statements, have that flow into the revisit_chain which will test the condition */
            if (eps[epi].v_.stmt_->prep_) {
              r = sl_exec_push_stmt(exec, eps[epi].v_.stmt_->prep_, eps[epi].enter_chain_, CHAIN_REF(eps[epi].revisit_chain_));
              if (r) return r;
            }
            else {
              eps[epi].revisit_chain_ = eps[epi].enter_chain_;
            }
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            break;
          case slsk_continue: {
            /* continue should break out to nearest loop, and continue execution after the loop body, but before
             * the loop is re-evaluated again. This is not a consistent chain location between while/do/for so 
             * some care is needed. */
            size_t pepi = epi - 1;
            while (pepi) {
              if (eps[pepi].kind_ == SLEPK_STMT) {
                if (eps[pepi].v_.stmt_->kind_ == slsk_while) {
                  /* slsk_while: stick it on the enter_chain (it'll schedule the condition) */
                  eps[pepi].enter_chain_ = sl_exec_join_chains(exec, eps[pepi].enter_chain_, eps[epi].enter_chain_);
                  break;
                }
                else if (eps[pepi].v_.stmt_->kind_ == slsk_do) {
                  /* slsk_do: stick it on the alt_chain (it'll schedule the condition) */
                  eps[pepi].alt_chain_ = sl_exec_join_chains(exec, eps[pepi].alt_chain_, eps[epi].enter_chain_);
                  break;
                }
                else if (eps[pepi].v_.stmt_->kind_ == slsk_for) {
                  /* slsk_for: stick it on the revisit_chain (it'll schedule the condition) */
                  eps[pepi].revisit_chain_ = sl_exec_join_chains(exec, eps[pepi].revisit_chain_, eps[epi].enter_chain_);
                  break;
                }
              }
              else if (eps[pepi].kind_ == SLEPK_EXPR) {
                if (eps[pepi].v_.expr_->op_ == exop_function_call) {
                  /* exop_function_call: stick it on the alt_chain; this is where the function is about to return to
                   *                     caller (and copies out any parameters and the like). */
                  eps[pepi].alt_chain_ = sl_exec_join_chains(exec, eps[pepi].alt_chain_, eps[epi].enter_chain_);
                  break;
                }
              }
              else if (eps[pepi].kind_ == SLEPK_BOOTSTRAP) {
                eps[pepi].post_chain_ = sl_exec_join_chains(exec, eps[pepi].post_chain_, eps[epi].enter_chain_);
                break;
              }
              pepi--;
            }
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;


            break;
          }
          case slsk_break: {
            /* Continue on the post chain of the loop, or bootstrap */
            size_t pepi = epi - 1;
            while (pepi) {
              if (eps[pepi].kind_ == SLEPK_STMT) {
                if (eps[pepi].v_.stmt_->kind_ == slsk_while || eps[pepi].v_.stmt_->kind_ == slsk_do || eps[pepi].v_.stmt_->kind_ == slsk_for) {
                    eps[pepi].post_chain_ = sl_exec_join_chains(exec, eps[pepi].post_chain_, eps[epi].enter_chain_);
                  break;
                }
              }
              else if (eps[pepi].kind_ == SLEPK_EXPR) {
                if (eps[pepi].v_.expr_->op_ == exop_function_call) {
                  /* exop_function_call: stick it on the alt_chain; this is where the function is about to return to
                   *                     caller (and copies out any parameters and the like). */
                  eps[pepi].alt_chain_ = sl_exec_join_chains(exec, eps[pepi].alt_chain_, eps[epi].enter_chain_);
                  break;
                }
              }
              else if (eps[pepi].kind_ == SLEPK_BOOTSTRAP) {
                eps[pepi].post_chain_ = sl_exec_join_chains(exec, eps[pepi].post_chain_, eps[epi].enter_chain_);
                break;
              }
              pepi--;
            }
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            break;
          }
          case slsk_return: {
            if (eps[epi].v_.stmt_->expr_) {
              r = sl_exec_push_expr(exec, eps[epi].v_.stmt_->expr_, eps[epi].enter_chain_, CHAIN_REF(eps[epi].revisit_chain_));
              if (r) return r;
            }
            else {
              eps[epi].revisit_chain_ = sl_exec_join_chains(exec, eps[epi].revisit_chain_, eps[epi].enter_chain_);
            }
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            break;
          }
          case slsk_discard: {
            /* Drop whatever we're doing and re-join on the alt-chain of the bootstrap */
            assert(eps[0].kind_ == SLEPK_BOOTSTRAP);
            eps[0].alt_chain_ = sl_exec_join_chains(exec, eps[0].alt_chain_, eps[epi].enter_chain_);
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            break;
          }
          case slsk_compound: {
            /* If there are inner statements (true_branch_) then execute and continue on the post_chain, otherwise,
             * just directly move onto the post_chain */
            if (eps[epi].v_.stmt_->true_branch_) {
              r = sl_exec_push_stmt(exec, eps[epi].v_.stmt_->true_branch_, eps[epi].enter_chain_, CHAIN_REF(eps[epi].post_chain_));
              if (r) return r;
            }
            else {
              eps[epi].post_chain_ = eps[epi].enter_chain_;
            }
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            break;
          }
        }
        if (r) return r;
      }
      else if (eps[epi].revisit_chain_ != SL_EXEC_NO_CHAIN) {
        switch (eps[epi].v_.stmt_->kind_) {
          case slsk_if: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.stmt_->condition_);
            /* Split execution into a true and false chain, then evaluate children selectively accordingly. */
            uint32_t true_chain = SL_EXEC_NO_CHAIN;
            uint32_t false_chain = SL_EXEC_NO_CHAIN;
            sl_exec_split_chains_by_bool(exec, eps[epi].v_.stmt_->condition_, eps[epi].revisit_chain_,
                                         &true_chain, &false_chain);
            eps[epi].revisit_chain_ = SL_EXEC_NO_CHAIN;
            if (true_chain != SL_EXEC_NO_CHAIN) {
              r = sl_exec_push_stmt(exec, eps[epi].v_.stmt_->true_branch_, true_chain, CHAIN_REF(eps[epi].post_chain_));
            }
            if (false_chain != SL_EXEC_NO_CHAIN) {
              if (eps[epi].v_.stmt_->false_branch_) {
                r = sl_exec_push_stmt(exec, eps[epi].v_.stmt_->false_branch_, false_chain, CHAIN_REF(eps[epi].post_chain_));
              }
              else {
                eps[epi].post_chain_ = sl_exec_join_chains(exec, false_chain, eps[epi].post_chain_);
              }
            }
            if (r) return r;
            break;
          }
          case slsk_while: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.stmt_->condition_);
            /* Split execution into the chain going into the body, surfacing back on the enter_chain_, and
             * the post_chain_. */
            uint32_t true_chain = SL_EXEC_NO_CHAIN;
            uint32_t false_chain = SL_EXEC_NO_CHAIN;
            sl_exec_split_chains_by_bool(exec, eps[epi].v_.stmt_->condition_, eps[epi].revisit_chain_,
                                         &true_chain, &false_chain);
            eps[epi].revisit_chain_ = SL_EXEC_NO_CHAIN;
            if (true_chain != SL_EXEC_NO_CHAIN) {
              /* Come back onto the enter_chain after executing the body (true_branch) so we can re-evaluate
               * the condition expression. */
              sl_exec_push_stmt(exec, eps[epi].v_.stmt_->true_branch_, true_chain, CHAIN_REF(eps[epi].enter_chain_));
            }
            /* Any false evaluations of the while condition go straight into the post_chain_ for moving to
             * the statement after the current (while) statement */
            eps[epi].post_chain_ = sl_exec_join_chains(exec, false_chain, eps[epi].post_chain_);
            break;
          }
          case slsk_do: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.stmt_->condition_);
            /* Split on the condition; true enters body again, false goes onto post chain. */
            uint32_t true_chain = SL_EXEC_NO_CHAIN;
            uint32_t false_chain = SL_EXEC_NO_CHAIN;
            sl_exec_split_chains_by_bool(exec, eps[epi].v_.stmt_->condition_, eps[epi].revisit_chain_,
                                         &true_chain, &false_chain);
            eps[epi].revisit_chain_ = SL_EXEC_NO_CHAIN;
            /* Put true_chain_ on the enter_chain_ for re-evaluation, note that this is slightly inefficient
             * as we could just set things up now, but prevents code duplication */
            assert(eps[epi].enter_chain_ == SL_EXEC_NO_CHAIN && "enter_chain should be empty when we evaluate revisit_chain");
            eps[epi].enter_chain_ = true_chain;
            /* Any false evaluations of the while condition go straight into the post_chain_ for moving to
             * the statement after the current (while) statement */
            eps[epi].post_chain_ = sl_exec_join_chains(exec, false_chain, eps[epi].post_chain_);
            break;
          }
          case slsk_for: {
            /* Evaluate the condition, then come back on the alt chain to determine what to do
             * note that we have to potentially execute an initializer first, e.g.:
             *   while (bool b = true) { .. }
             * is valid. */
            size_t condition_eval_index = exec->num_execution_points_;
            r = sl_exec_push_expr(exec, eps[epi].v_.stmt_->condition_, SL_EXEC_NO_CHAIN, CHAIN_REF(eps[epi].alt_chain_));
            if (r) return r;
            size_t condition_prep_stmt_index = exec->num_execution_points_;
            /* Check if we have a preparatory statement for the condition (an initializer) and execute it prior to the condition expression,
             * if so. Otherwise wire the enter_chain_ to directly go into the expression. */
            if (eps[epi].v_.stmt_->prep_cond_) {
              r = sl_exec_push_stmt(exec, eps[epi].v_.stmt_->prep_cond_, eps[epi].revisit_chain_, CHAIN_REF(eps[condition_eval_index].enter_chain_));
              if (r) return r;
            }
            else {
              eps[condition_eval_index].enter_chain_ = eps[epi].revisit_chain_;
            }
            eps[epi].revisit_chain_ = SL_EXEC_NO_CHAIN;

            break;
          }

          case slsk_return: {
            /* Copy the return value (if any) into the caller exop_function_call result. */
            size_t pepi = epi - 1;
            while (pepi) {
              if (eps[pepi].kind_ == SLEPK_EXPR) {
                if (eps[pepi].v_.expr_->op_ == exop_function_call) {
                  sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.stmt_->expr_);
                  sl_exec_move_param(exec, eps[epi].revisit_chain_, 
                                     exec->execution_frames_ + exec->num_execution_frames_ - 2, &eps[pepi].v_.expr_->base_regs_,
                                     exec->execution_frames_ + exec->num_execution_frames_ - 1, EXPR_RVALUE(eps[epi].v_.stmt_->expr_));
                  eps[pepi].alt_chain_ = sl_exec_join_chains(exec, eps[pepi].alt_chain_, eps[epi].revisit_chain_);
                  break;
                }
              }
              pepi--;
            }
            if (!pepi) {
              /* No function call found, re-join the post_chain of the bootstrap so the execution chain is not lost */
              assert(eps[0].kind_ == SLEPK_BOOTSTRAP);
              eps[0].post_chain_ = sl_exec_join_chains(exec, eps[0].post_chain_, eps[epi].revisit_chain_);
            }
            eps[epi].revisit_chain_ = SL_EXEC_NO_CHAIN;
            break;
          }
        }
      }
      else if (eps[epi].alt_chain_ != SL_EXEC_NO_CHAIN) {
        switch (eps[epi].v_.stmt_->kind_) {
          case slsk_for: {
            sl_exec_need_rvalue(exec, eps[epi].alt_chain_, eps[epi].v_.stmt_->condition_);
            /* Coming back from condition evaluation, split execution flows, true branch goes into the body,
             * false branch goes into post */
            uint32_t true_chain = SL_EXEC_NO_CHAIN;
            uint32_t false_chain = SL_EXEC_NO_CHAIN;
            sl_exec_split_chains_by_bool(exec, eps[epi].v_.stmt_->condition_, eps[epi].alt_chain_,
                                        &true_chain, &false_chain);
            eps[epi].alt_chain_ = SL_EXEC_NO_CHAIN;
            if (true_chain != SL_EXEC_NO_CHAIN) {
              /* Set up the true chain, this involves the following:
               * - First execute the body
               * - Then execute the post_ expression (if any)
               * - Then return to the revisit_chain_ on the slsk_for and have it set up the condition check and body execution */
              if (eps[epi].v_.stmt_->post_) {
                size_t post_expr = exec->num_execution_points_;
                r = sl_exec_push_expr(exec, eps[epi].v_.stmt_->post_, SL_EXEC_NO_CHAIN, CHAIN_REF(eps[epi].revisit_chain_));
                r = r ? r : sl_exec_push_stmt(exec, eps[epi].v_.stmt_->true_branch_, true_chain, CHAIN_REF(eps[post_expr].enter_chain_));
                if (r) return r;
              }
              else {
                /* No post expression, so re-wire the body to continue on our revisit chain directly. */
                r = sl_exec_push_stmt(exec, eps[epi].v_.stmt_->true_branch_, true_chain, CHAIN_REF(eps[epi].revisit_chain_));
                if (r) return r;
              }
            }

            /* False chain joins up on the post */
            eps[epi].post_chain_ = sl_exec_join_chains(exec, false_chain, eps[epi].post_chain_);
            break;
          }
          case slsk_do: {
            /* Schedule the condition evaluation; then proceed to the revisit_chain_ to decide what to do */
            r = sl_exec_push_expr(exec, eps[epi].v_.stmt_->condition_, eps[epi].alt_chain_, CHAIN_REF(eps[epi].revisit_chain_));
            if (r) return r;
            eps[epi].alt_chain_ = SL_EXEC_NO_CHAIN;
            break;
          }
        }
      }
      else if (eps[epi].post_chain_ != SL_EXEC_NO_CHAIN) {
        /* Step to next by transforming current SLEPK_STMT to point to next */
        struct sl_stmt *next = sl_stmt_next_execution_sibling(eps[epi].v_.stmt_);
        if (next) {
          eps[epi].v_.stmt_ = next;
          eps[epi].enter_chain_ = eps[epi].post_chain_;
          eps[epi].post_chain_ = SL_EXEC_NO_CHAIN;
        }
        else {
          /* No next, pop current */
          uint32_t *continuation_ep = (uint32_t *)(((char *)exec->execution_points_) + eps[epi].continue_chain_ptr_);
          *continuation_ep = sl_exec_join_chains(exec, *continuation_ep, eps[epi].post_chain_);
          eps[epi].post_chain_ = SL_EXEC_NO_CHAIN;

          sl_exec_pop_ep(exec);
        }
      }
      else {
        /* No chains whatsoever.. Suppose no execution chain returns back to this statement (e.g. they all
         * discard or return or whatnot), then the best thing we can do here is pop the statement. */
        sl_exec_pop_ep(exec);
      }
    }
    else if (eps[epi].kind_ == SLEPK_EXPR) {
      int dont_pop = 0;
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
            assert((eps[epi].v_.expr_->offset_reg_.kind_ == slrak_void) && "variable sub-expression's lvalue should not be offset based");
            sl_reg_move(exec, eps[epi].enter_chain_, &eps[epi].v_.expr_->variable_->reg_alloc_, NULL, &eps[epi].v_.expr_->base_regs_, NULL);

            uint32_t *continuation_ep = (uint32_t *)(((char *)exec->execution_points_) + eps[epi].continue_chain_ptr_);
            *continuation_ep = sl_exec_join_chains(exec, *continuation_ep, eps[epi].enter_chain_);
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            sl_exec_pop_ep(exec);
            break;
          }
          case exop_literal: {
            /* Load constant value into the register */
            assert((eps[epi].v_.expr_->offset_reg_.kind_ == slrak_void) && "literal sub-expression's lvalue should not be offset based");
            sl_exec_init_literal(exec, eps[epi].enter_chain_, &eps[epi].v_.expr_->base_regs_, &eps[epi].v_.expr_->literal_value_, 0);

            uint32_t *continuation_ep = (uint32_t *)(((char *)exec->execution_points_) + eps[epi].continue_chain_ptr_);
            *continuation_ep = sl_exec_join_chains(exec, *continuation_ep, eps[epi].enter_chain_);
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            sl_exec_pop_ep(exec);
            break;
          }
          case exop_array_subscript: {
            /* Push children first, post evaluate the subscript */
            r = sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[1], SL_EXEC_NO_CHAIN, CHAIN_REF(eps[epi].revisit_chain_));
            /* Now push the first child, its continuation is the second child's evaluation */
            r = r ? r : sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[0], eps[epi].enter_chain_, CHAIN_REF(exec->execution_points_[exec->num_execution_points_-1].enter_chain_));
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            if (r) return r;
            break;
          }

          case exop_post_inc:
          case exop_post_dec:
          case exop_pre_inc:
          case exop_pre_dec:
          case exop_negate:
          case exop_logical_not: {
            /* Evaluate child, then come back on revisit_chain_, */
            r = sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[0], eps[epi].enter_chain_, CHAIN_REF(eps[epi].revisit_chain_));
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
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
          case exop_logical_xor:
          case exop_assign:
          case exop_mul_assign:
          case exop_div_assign:
          case exop_add_assign:
          case exop_sub_assign: {
            /* Evaluate both branches first, then come back to evaluate this expression node on the revisit_chain_.. */

            /* Push 2nd child first, it will be evaluated last (LIFO) */
            r = sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[1], SL_EXEC_NO_CHAIN, CHAIN_REF(eps[epi].revisit_chain_));
            /* Now push the first child, its continuation is the second child's evaluation */
            r = r ? r : sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[0], eps[epi].enter_chain_, CHAIN_REF(exec->execution_points_[exec->num_execution_points_-1].enter_chain_));
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            if (r) return r;
            break;
          }

          case exop_component_selection:
          case exop_field_selection:
          case exop_function_call:
          case exop_constructor: {
            /* Evaluate all children first; then continue to revisit_chain_ to perform the invocation */
            if (eps[epi].v_.expr_->num_children_) {
              size_t n;
              n = eps[epi].v_.expr_->num_children_;
              while (n--) {
                uint32_t chain = SL_EXEC_NO_CHAIN;
                size_t continuation = CHAIN_REF(exec->execution_points_[exec->num_execution_points_-1].enter_chain_);
                if (n == (eps[epi].v_.expr_->num_children_ - 1)) {
                  continuation = CHAIN_REF(eps[epi].revisit_chain_);
                }
                if (n == 0) {
                  chain = eps[epi].enter_chain_;
                }

                r = r ? r : sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[n], chain, continuation);
              }
            }
            else {
              /* No children, go straight to revisit_chain_ */
              eps[epi].revisit_chain_ = eps[epi].enter_chain_;
            }
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
            break;
          }

          /* Logical AND/OR evaluation: these should evalue the left branch first and, depending on the result, optionally evalue the right - sequence points! */
          case exop_logical_and:
          case exop_logical_or: {
            /* Now push the first child, its continuation returns here for a revisit */
            sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[0], eps[epi].enter_chain_, CHAIN_REF(eps[epi].revisit_chain_));
            break;
          }

          case exop_sequence: {
            /* The sequence is implied in the evaluation of its children */
            size_t our_continue_ptr = eps[epi].continue_chain_ptr_;
            struct sl_expr *seq_expr = eps[epi].v_.expr_;
            uint32_t enter_chain = eps[epi].enter_chain_;
            eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;

            /* Push the 2nd child on the stack first (LIFO); its continuation is our (exop_sequence) continuation */
            sl_exec_push_expr(exec, seq_expr->children_[1], SL_EXEC_NO_CHAIN, CHAIN_REF(eps[epi].revisit_chain_));
            /* Now push the 1st child on the stack, to be evaluated first; its continuation is our second child. */
            sl_exec_push_expr(exec, seq_expr->children_[0], enter_chain, CHAIN_REF(exec->execution_points_[exec->num_execution_points_-1].enter_chain_));
            
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
        switch (eps[epi].v_.expr_->op_) {
          case exop_array_subscript: {
            if (eps[epi].v_.expr_->children_[0]->base_regs_.kind_ == slrak_array) {
              /* Take the fixed array size that this subscript indexes, if the array child has an offset_, multiply
               * the offset value to form the new offset, then be done. We don't need to do anything on the lvalue
               * itself as the register allocator has ensured it is the correct field subset. */
              if (eps[epi].v_.expr_->children_[0]->offset_reg_.kind_  != slrak_void) {
                if (&eps[epi].v_.expr_->children_[1]->offset_reg_.kind_ != slrak_void) {
                  sl_exec_offset_load(exec, eps[epi].revisit_chain_, 
                                      &eps[epi].v_.expr_->children_[1]->rvalue_,
                                      &eps[epi].v_.expr_->children_[1]->base_regs_,
                                      &eps[epi].v_.expr_->children_[1]->offset_reg_);
                }
              
                /* New offset = old-offset * array-size + array-subscript-index */
                sl_exec_i_mul_constant_and_add(eps[epi].revisit_chain_, exec->exec_chain_reg_,
                                               INT_REG_PTR_NRV(&eps[epi].v_.expr_->offset_reg_, 0),
                                               INT_REG_PTR_NRV(&eps[epi].v_.expr_->children_[0]->offset_reg_, 0),
                                               INT_REG_PTR(eps[epi].v_.expr_->children_[1], 0),
                                               (int64_t)eps[epi].v_.expr_->children_[0]->base_regs_.v_.array_.num_elements_);

              }
              else {
                sl_reg_move(exec, eps[epi].revisit_chain_, 
                            &eps[epi].v_.expr_->children_[1]->base_regs_, &eps[epi].v_.expr_->children_[1]->offset_reg_,
                            &eps[epi].v_.expr_->offset_reg_, NULL);
              }
            }
            else {
              /* Not an array, base must be indirect and this array subscript is to access components */
              sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);

              sl_exec_load_effective_reg_index(exec, eps[epi].revisit_chain_, &eps[epi].v_.expr_->base_regs_, &eps[epi].v_.expr_->children_[0]->base_regs_, EXPR_RVALUE(eps[epi].v_.expr_->children_[1]));
            }
            break;
          }

          case exop_post_inc:
          case exop_post_dec: {
            if (eps[epi].v_.expr_->children_[0]->offset_reg_.kind_ != slrak_void) {
              /* Child is offsetted; load at an offset into the result */
              sl_exec_offset_load(exec, eps[epi].revisit_chain_,
                                  &eps[epi].v_.expr_->base_regs_,
                                  &eps[epi].v_.expr_->children_[0]->base_regs_,
                                  &eps[epi].v_.expr_->children_[0]->offset_reg_);
              /* Inc/Decrement the value from the result into the R value of the child */
              if (eps[epi].v_.expr_->op_ == exop_post_inc) {
                sl_exec_increment(exec, eps[epi].revisit_chain_, &eps[epi].v_.expr_->children_[0]->rvalue_, EXPR_RVALUE(eps[epi].v_.expr_));
              }
              else /* (eps[epi].v_->expr_->op_ == exop_pre_dec) */ {
                sl_exec_decrement(exec, eps[epi].revisit_chain_, &eps[epi].v_.expr_->children_[0]->rvalue_, EXPR_RVALUE(eps[epi].v_.expr_));
              }
              /* The result will now hold the original value, the R-value of the child now holds the
               * incremented value. Store the R-Value of the child into the child's L-Value (at the offset). */
              sl_exec_offset_store(exec, eps[epi].revisit_chain_,
                                   &eps[epi].v_.expr_->children_[0]->base_regs_,
                                   &eps[epi].v_.expr_->children_[0]->offset_reg_,
                                   EXPR_RVALUE(eps[epi].v_.expr_->children_[0]));
            }
            else {
              /* Move the pre-existing value into the result */
              sl_reg_move(exec, eps[epi].revisit_chain_,
                          &eps[epi].v_.expr_->children_[0]->base_regs_, NULL,
                          &eps[epi].v_.expr_->base_regs_, NULL);

              /* And then Inc/Decrement the value from the result (the pre-existing one) into the child's l-value */
              if (eps[epi].v_.expr_->op_ == exop_post_inc) {
                sl_exec_increment(exec, eps[epi].revisit_chain_, &eps[epi].v_.expr_->children_[0]->base_regs_, &eps[epi].v_.expr_->base_regs_);
              }
              else /* (eps[epi].v_->expr_->op_ == exop_post_dec) */ {
                sl_exec_decrement(exec, eps[epi].revisit_chain_, &eps[epi].v_.expr_->children_[0]->base_regs_, &eps[epi].v_.expr_->base_regs_);
              }
            }
            break;
          }
          case exop_pre_inc:
          case exop_pre_dec: {
            if (eps[epi].v_.expr_->children_[0]->offset_reg_.kind_ != slrak_void) {
              /* Child is offsetted; load via rvalue */
              sl_reg_move(exec, eps[epi].revisit_chain_,
                          &eps[epi].v_.expr_->children_[0]->base_regs_,
                          &eps[epi].v_.expr_->children_[0]->offset_reg_,
                          &eps[epi].v_.expr_->children_[0]->rvalue_,
                          NULL);
              /* Inc/Decrement the rvalue into the result value */
              if (eps[epi].v_.expr_->op_ == exop_pre_inc) {
                sl_exec_increment(exec, eps[epi].revisit_chain_, &eps[epi].v_.expr_->base_regs_, EXPR_RVALUE(eps[epi].v_.expr_->children_[0]));
              }
              else /* (eps[epi].v_->expr_->op_ == exop_pre_dec) */ {
                sl_exec_decrement(exec, eps[epi].revisit_chain_, &eps[epi].v_.expr_->base_regs_, EXPR_RVALUE(eps[epi].v_.expr_->children_[0]));
              }
              
              /* Store the result value into the original child's l-value */
              sl_reg_move(exec, eps[epi].revisit_chain_,
                          EXPR_RVALUE(eps[epi].v_.expr_), NULL,
                          &eps[epi].v_.expr_->children_[0]->base_regs_,
                          &eps[epi].v_.expr_->children_[0]->offset_reg_);
            }
            else {
              /* Child can be used directly. */
              /* Inc/Decrement the child's value into the result value */
              if (eps[epi].v_.expr_->op_ == exop_pre_inc) {
                sl_exec_increment(exec, eps[epi].revisit_chain_, &eps[epi].v_.expr_->base_regs_, EXPR_RVALUE(eps[epi].v_.expr_->children_[0]));
              }
              else /* (eps[epi].v_->expr_->op_ == exop_pre_dec) */ {
                sl_exec_decrement(exec, eps[epi].revisit_chain_, &eps[epi].v_.expr_->base_regs_, EXPR_RVALUE(eps[epi].v_.expr_->children_[0]));
              }
              /* Store the result value back into the original child's register */
              sl_reg_move(exec, eps[epi].revisit_chain_,
                          &eps[epi].v_.expr_->base_regs_, NULL,
                          &eps[epi].v_.expr_->children_[0]->base_regs_, NULL);
            }
            break;
          }
          case exop_negate: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);

            sl_exec_negate(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_, eps[epi].v_.expr_->children_[0]);
            break;
          }
          case exop_logical_not: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);

            sl_exec_logical_not(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_, eps[epi].v_.expr_->children_[0]);
            break;
          }

          case exop_mul_assign:
          case exop_multiply: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);

            sl_exec_mul(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_,
                        eps[epi].v_.expr_->children_[0],
                        eps[epi].v_.expr_->children_[1]);

            if (eps[epi].v_.expr_->op_ == exop_mul_assign) {
              /* Store the result value into the original child's l-value */
              sl_reg_move(exec, eps[epi].revisit_chain_, EXPR_RVALUE(eps[epi].v_.expr_), NULL, &eps[epi].v_.expr_->children_[0]->base_regs_, &eps[epi].v_.expr_->children_[0]->offset_reg_);
            }
            break;
          }
          case exop_div_assign:
          case exop_divide: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);

            sl_exec_div(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_,
                        eps[epi].v_.expr_->children_[0],
                        eps[epi].v_.expr_->children_[1]);
            if (eps[epi].v_.expr_->op_ == exop_div_assign) {
              /* Store the result value into the original child's l-value */
              sl_reg_move(exec, eps[epi].revisit_chain_, EXPR_RVALUE(eps[epi].v_.expr_), NULL, &eps[epi].v_.expr_->children_[0]->base_regs_, &eps[epi].v_.expr_->children_[0]->offset_reg_);
            }
            break;
          }

          case exop_add_assign:
          case exop_add: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);

            sl_exec_add(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_,
                        eps[epi].v_.expr_->children_[0],
                        eps[epi].v_.expr_->children_[1]);

            if (eps[epi].v_.expr_->op_ == exop_add_assign) {
              /* Store the result value into the original child's l-value */
              sl_reg_move(exec, eps[epi].revisit_chain_, EXPR_RVALUE(eps[epi].v_.expr_), NULL, &eps[epi].v_.expr_->children_[0]->base_regs_, &eps[epi].v_.expr_->children_[0]->offset_reg_);
            }

            break;
          }

          case exop_sub_assign:
          case exop_subtract: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);

            sl_exec_sub(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_,
                        eps[epi].v_.expr_->children_[0],
                        eps[epi].v_.expr_->children_[1]);

            if (eps[epi].v_.expr_->op_ == exop_sub_assign) {
              /* Store the result value into the original child's l-value */
              sl_reg_move(exec, eps[epi].revisit_chain_, EXPR_RVALUE(eps[epi].v_.expr_), NULL, &eps[epi].v_.expr_->children_[0]->base_regs_, &eps[epi].v_.expr_->children_[0]->offset_reg_);
            }
            break;
          }

          case exop_lt: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);

            if (eps[epi].v_.expr_->children_[0]->base_regs_.kind_ == slrak_float) {
              sl_exec_f_lt((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           BOOL_REG_PTR(eps[epi].v_.expr_, 0),
                           FLOAT_REG_PTR(eps[epi].v_.expr_->children_[0], 0),
                           FLOAT_REG_PTR(eps[epi].v_.expr_->children_[1], 0));
            }
            else if (eps[epi].v_.expr_->children_[0]->base_regs_.kind_ == slrak_int) {
              sl_exec_i_lt((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           BOOL_REG_PTR(eps[epi].v_.expr_, 0),
                           INT_REG_PTR(eps[epi].v_.expr_->children_[0], 0),
                           INT_REG_PTR(eps[epi].v_.expr_->children_[1], 0));
            }
            break;
          }
          case exop_le: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);

            if (eps[epi].v_.expr_->children_[0]->base_regs_.kind_ == slrak_float) {
              sl_exec_f_le((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           BOOL_REG_PTR(eps[epi].v_.expr_, 0),
                           FLOAT_REG_PTR(eps[epi].v_.expr_->children_[0], 0),
                           FLOAT_REG_PTR(eps[epi].v_.expr_->children_[1], 0));
            }
            else if (eps[epi].v_.expr_->children_[0]->base_regs_.kind_ == slrak_int) {
              sl_exec_i_le((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           BOOL_REG_PTR(eps[epi].v_.expr_, 0),
                           INT_REG_PTR(eps[epi].v_.expr_->children_[0], 0),
                           INT_REG_PTR(eps[epi].v_.expr_->children_[1], 0));
            }
            break;
          }
          case exop_ge: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);

            if (eps[epi].v_.expr_->children_[0]->base_regs_.kind_ == slrak_float) {
              sl_exec_f_ge((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           BOOL_REG_PTR(eps[epi].v_.expr_, 0),
                           FLOAT_REG_PTR(eps[epi].v_.expr_->children_[0], 0),
                           FLOAT_REG_PTR(eps[epi].v_.expr_->children_[1], 0));
            }
            else if (eps[epi].v_.expr_->children_[0]->base_regs_.kind_ == slrak_int) {
              sl_exec_i_ge((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           BOOL_REG_PTR(eps[epi].v_.expr_, 0),
                           INT_REG_PTR(eps[epi].v_.expr_->children_[0], 0),
                           INT_REG_PTR(eps[epi].v_.expr_->children_[1], 0));
            }
            break;
          }
          case exop_gt: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);

            if (eps[epi].v_.expr_->children_[0]->base_regs_.kind_ == slrak_float) {
              sl_exec_f_gt((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           BOOL_REG_PTR(eps[epi].v_.expr_, 0),
                           FLOAT_REG_PTR(eps[epi].v_.expr_->children_[0], 0),
                           FLOAT_REG_PTR(eps[epi].v_.expr_->children_[1], 0));
            }
            else if (eps[epi].v_.expr_->children_[0]->base_regs_.kind_ == slrak_int) {
              sl_exec_i_gt((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           BOOL_REG_PTR(eps[epi].v_.expr_, 0),
                           INT_REG_PTR(eps[epi].v_.expr_->children_[0], 0),
                           INT_REG_PTR(eps[epi].v_.expr_->children_[1], 0));
            }
            break;
          }

          case exop_eq: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);

            if (eps[epi].v_.expr_->children_[0]->base_regs_.kind_ == slrak_float) {
              sl_exec_f_eq((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           BOOL_REG_PTR(eps[epi].v_.expr_, 0),
                           FLOAT_REG_PTR(eps[epi].v_.expr_->children_[0], 0),
                           FLOAT_REG_PTR(eps[epi].v_.expr_->children_[1], 0));
            }
            else if (eps[epi].v_.expr_->children_[0]->base_regs_.kind_ == slrak_int) {
              sl_exec_i_eq((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           BOOL_REG_PTR(eps[epi].v_.expr_, 0),
                           INT_REG_PTR(eps[epi].v_.expr_->children_[0], 0),
                           INT_REG_PTR(eps[epi].v_.expr_->children_[1], 0));
            }
            break;
          }
          case exop_ne: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);

            if (eps[epi].v_.expr_->children_[0]->base_regs_.kind_ == slrak_float) {
              sl_exec_f_ne((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           BOOL_REG_PTR(eps[epi].v_.expr_, 0),
                           FLOAT_REG_PTR(eps[epi].v_.expr_->children_[0], 0),
                           FLOAT_REG_PTR(eps[epi].v_.expr_->children_[1], 0));
            }
            else if (eps[epi].v_.expr_->children_[0]->base_regs_.kind_ == slrak_int) {
              sl_exec_i_ne((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                           BOOL_REG_PTR(eps[epi].v_.expr_, 0),
                           INT_REG_PTR(eps[epi].v_.expr_->children_[0], 0),
                           INT_REG_PTR(eps[epi].v_.expr_->children_[1], 0));
            }
            break;
          }

          case exop_logical_xor: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);

            sl_exec_logical_or((uint8_t)eps[epi].revisit_chain_, exec->exec_chain_reg_,
                           BOOL_REG_PTR(eps[epi].v_.expr_, 0),
                           BOOL_REG_PTR(eps[epi].v_.expr_->children_[0], 0),
                           BOOL_REG_PTR(eps[epi].v_.expr_->children_[1], 0));
            break;
          }

          case exop_assign: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);

            sl_reg_move(exec, eps[epi].revisit_chain_, EXPR_RVALUE(eps[epi].v_.expr_->children_[1]), NULL, &eps[epi].v_.expr_->children_[0]->base_regs_, &eps[epi].v_.expr_->children_[0]->offset_reg_);
            break;
          }

          case exop_logical_and: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);

            /* We get here when child 0 has been evaluated, make a selection and evaluate for child 1,
             * false child 0 means the overal result is false and we can return, true child 0 means the
             * result for child 1 determines the outcome. */
            uint32_t true_chain = SL_EXEC_NO_CHAIN;
            uint32_t false_chain = SL_EXEC_NO_CHAIN;
            sl_exec_split_chains_by_bool(exec, eps[epi].v_.expr_->children_[0], eps[epi].revisit_chain_, 
                                         &true_chain, &false_chain);
            eps[epi].revisit_chain_ = SL_EXEC_NO_CHAIN;
            /* Child 0 == false ? Move child 0 result to our result and pass on to continuation. */
            sl_reg_move(exec, false_chain, EXPR_RVALUE(eps[epi].v_.expr_->children_[0]), NULL, &eps[epi].v_.expr_->base_regs_, NULL);
            uint32_t *chain_ptr = (uint32_t *)(((uintptr_t)exec->execution_points_) + eps[epi].continue_chain_ptr_);
            *chain_ptr = sl_exec_join_chains(exec, *chain_ptr, false_chain);
            
            /* Child 0 == true ? Move to process child 1 as the result, we return here in post_chain_ to move the result
             * in to the appropriat register */
            dont_pop = 1;
              
            sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[1], true_chain, CHAIN_REF(eps[epi].post_chain_));

            break;
          }

          case exop_logical_or: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);

            /* Like the logical-and, child 0 has been evaluated. If child 0 is true, the result is true and
             * the second child must not be evaluated. Conversely, if child 0 is false, the result is the
             * evaluation of the second branch */
            uint32_t true_chain = SL_EXEC_NO_CHAIN;
            uint32_t false_chain = SL_EXEC_NO_CHAIN;
            sl_exec_split_chains_by_bool(exec, eps[epi].v_.expr_->children_[0], eps[epi].revisit_chain_,
                                         &true_chain, &false_chain);
            eps[epi].revisit_chain_ = SL_EXEC_NO_CHAIN;
            /* Child 0 == true ? Move child 0 result to our result and pass on to continuation */
            sl_reg_move(exec, true_chain, EXPR_RVALUE(eps[epi].v_.expr_->children_[0]), NULL, &eps[epi].v_.expr_->base_regs_, NULL);
            uint32_t *chain_ptr = (uint32_t *)(((uintptr_t)exec->execution_points_) + eps[epi].continue_chain_ptr_);
            *chain_ptr = sl_exec_join_chains(exec, *chain_ptr, true_chain);

            /* Child 0 == False ? Move to process child 1 as the result and return here in post_chain to move it in
             * position. */
            dont_pop = 1;
            sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[1], false_chain, CHAIN_REF(eps[epi].post_chain_));

            break;
          }

          case exop_sequence: {
            if (!sl_reg_alloc_are_equal(&eps[epi].v_.expr_->base_regs_, &eps[epi].v_.expr_->children_[1]->base_regs_)) {
              sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[1]);
              sl_reg_move(exec, eps[epi].revisit_chain_, EXPR_RVALUE(eps[epi].v_.expr_->children_[1]), NULL, &eps[epi].v_.expr_->base_regs_, NULL);
            }
            else {
              /* eps[epi].v_.expr_ shares registers with second child, we're already done even if both are lvalues */
            }
          }

          case exop_conditional: {
            sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_->children_[0]);

            /* Split execution into a true and false chain, then evaluate children selectively accordingly. */
            /* The true branch returns on post_chain_ (and we move the true subset into registers there.)
             * the false branch returns on alt_chain_ (and we move the false subset into registers there.) */
            uint32_t true_chain = SL_EXEC_NO_CHAIN;
            uint32_t false_chain = SL_EXEC_NO_CHAIN;
            sl_exec_split_chains_by_bool(exec, eps[epi].v_.expr_->children_[0], eps[epi].revisit_chain_,
                                         &true_chain, &false_chain);
            eps[epi].revisit_chain_ = SL_EXEC_NO_CHAIN;
            dont_pop = 1;
            sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[1], true_chain, CHAIN_REF(eps[epi].post_chain_));
            sl_exec_push_expr(exec, eps[epi].v_.expr_->children_[2], false_chain, CHAIN_REF(eps[epi].alt_chain_));
            break;
          }

          case exop_field_selection: {
            /* Field selection should be a no-op, however, if a parent of the field selection has determined
             * a destination register, then it still needs a move. That's an edge case, in the common case
             * this move will degrade into a no-op. */
            size_t field_index = 0;
            struct sl_expr *x = eps[epi].v_.expr_;
            struct sl_expr *sx = eps[epi].v_.expr_->children_[0];
            struct sl_reg_alloc *child_field = &sx->base_regs_.v_.comp_.fields_[x->field_index_];
            if (x->offset_reg_.kind_ != slrak_void) {
              /* Check that the offset and base registers are the same for the child as they are for the
               * exop_field_selection expression. This *should* be the case, as there is no way a parent
               * could impose an offsetted base register on a child, and exop_field_selection won't put
               * in an offset unless it is from its own child. */
              if (x->offset_limit_ != sx->offset_limit_) {
                assert(0);
                return -1;
              }
              if (!sl_reg_alloc_are_equal(&x->offset_reg_, &sx->offset_reg_)) {
                assert(0);
                return -1;
              }
              if (!sl_reg_alloc_are_equal(&x->base_regs_, &sx->base_regs_)) {
                assert(0);
                return -1;
              }
              /* ok done, no copy needed. */
            }
            else {
              /* Child not at an offset, just perform a move; this will likely be a no-op unless the
               * parent of the exop_field_selection interfered during register allocation. */
              sl_reg_move(exec, eps[epi].revisit_chain_, child_field, NULL, &x->base_regs_, NULL);
            }
            break;
          }

          case exop_component_selection:
          case exop_constructor: {
            size_t ci; 
            for (ci = 0; ci < eps[epi].v_.expr_->num_components_; ++ci) {
              struct sl_component_selection *cs = eps[epi].v_.expr_->swizzle_ + ci;
              if (cs->parameter_index_ != -1) {
                struct sl_expr *param = eps[epi].v_.expr_->children_[cs->parameter_index_];
                struct sl_reg_alloc *param_ra;
                if (param->offset_reg_.kind_ != slrak_void) {
                  sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, param);
                  param_ra = &param->rvalue_;
                }
                else {
                  param_ra = &param->base_regs_;
                }
                
                /* Note that, for lvalues (exop_component_selection), the source and the destination
                 * registers will be teh same, the sl_exec_XXX_move() function detects this and it
                 * degenerates into a no-op. */
                switch (cs->conversion_) {
                  case slcc_float_to_float:
                    sl_exec_f_move(eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                   FLOAT_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                   FLOAT_REG_PTR_NRV(param_ra, cs->component_index_));
                    break;
                  case slcc_int_to_float:
                    sl_exec_i_to_f_move(eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                        FLOAT_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                        INT_REG_PTR_NRV(param_ra, cs->component_index_));
                    break;
                  case slcc_bool_to_float:
                    sl_exec_b_to_f_move(eps[epi].revisit_chain_, exec->exec_chain_reg_,
                                        FLOAT_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                        BOOL_REG_PTR_NRV(param_ra, cs->component_index_));
                    break;
                  case slcc_float_to_int:
                    sl_exec_f_to_i_move(eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                        INT_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                        FLOAT_REG_PTR_NRV(param_ra, cs->component_index_));
                    break;
                  case slcc_int_to_int:
                    sl_exec_i_move(eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                   INT_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                   INT_REG_PTR_NRV(param_ra, cs->component_index_));
                    break;
                  case slcc_bool_to_int:
                    sl_exec_b_to_i_move(eps[epi].revisit_chain_, exec->exec_chain_reg_,
                                        INT_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                        BOOL_REG_PTR_NRV(param_ra, cs->component_index_));
                    break;
                  case slcc_float_to_bool:
                    sl_exec_f_to_b_move(eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                        BOOL_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                        FLOAT_REG_PTR_NRV(param_ra, cs->component_index_));
                    break;
                  case slcc_int_to_bool:
                    sl_exec_i_to_b_move(eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                        BOOL_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                        INT_REG_PTR_NRV(param_ra, cs->component_index_));
                    break;
                  case slcc_bool_to_bool:
                    sl_exec_b_move(eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                        BOOL_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                        BOOL_REG_PTR_NRV(param_ra, cs->component_index_));
                    break;
                }
              }
              else {
                /* Initialize with a constant */
                switch (cs->component_index_) {
                  case 0:
                    switch (cs->conversion_) {
                      case slcc_float_to_float:
                      case slcc_int_to_float:
                      case slcc_bool_to_float:
                        sl_exec_f_init(eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                       FLOAT_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                       0.0f);
                        break;
                      case slcc_float_to_int:
                      case slcc_int_to_int:
                      case slcc_bool_to_int:
                        sl_exec_i_init(eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                       INT_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                       0);
                        break;
                      case slcc_float_to_bool:
                      case slcc_int_to_bool:
                      case slcc_bool_to_bool:
                        sl_exec_b_init(eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                       BOOL_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                       0);
                        break;
                    }
                    break;
                  case 1:
                    switch (cs->conversion_) {
                      case slcc_float_to_float:
                      case slcc_int_to_float:
                      case slcc_bool_to_float:
                        sl_exec_f_init(eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                       FLOAT_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                       1.0f);
                        break;
                      case slcc_float_to_int:
                      case slcc_int_to_int:
                      case slcc_bool_to_int:
                        sl_exec_i_init(eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                       INT_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                       1);
                        break;
                      case slcc_float_to_bool:
                      case slcc_int_to_bool:
                      case slcc_bool_to_bool:
                        sl_exec_b_init(eps[epi].revisit_chain_, exec->exec_chain_reg_, 
                                       BOOL_REG_PTR_NRV(&eps[epi].v_.expr_->base_regs_, ci),
                                       1);
                        break;
                    }
                    break;
                }
              }
            }
            break;
          }

          case exop_function_call: {
            if (eps[epi].v_.expr_->function_->builtin_runtime_fn_) {
              eps[epi].v_.expr_->function_->builtin_runtime_fn_(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_);
            }
            else {
              r = sl_exec_push_execution_frame(exec);
              if (r) return r;
              struct sl_execution_frame *parent = exec->execution_frames_ + exec->num_execution_frames_ - 2;
              struct sl_execution_frame *ef = exec->execution_frames_ + exec->num_execution_frames_ - 1;
              ef->f_ = eps[epi].v_.expr_->function_;
            
              size_t n;
              /* Have to have equal number of children as we do parameters.. */
              assert(ef->f_->num_parameters_ == eps[epi].v_.expr_->num_children_);

              /* Establish new frame */
              ef->local_float_offset_ += (int)parent->f_->frame_.ract_.rra_floats_.watermark_;
              ef->local_int_offset_ += (int)parent->f_->frame_.ract_.rra_ints_.watermark_;
              ef->local_bool_offset_ += (int)parent->f_->frame_.ract_.rra_bools_.watermark_;
              ef->local_sampler2D_offset_ += (int)parent->f_->frame_.ract_.rra_sampler2D_.watermark_;
              ef->local_samplerCube_offset_ += (int)parent->f_->frame_.ract_.rra_samplerCube_.watermark_;

              for (n = 0; n < ef->f_->num_parameters_; ++n) {
                struct sl_reg_alloc *param_ra = &ef->f_->parameters_[n].variable_->reg_alloc_;
                sl_exec_need_rvalue(exec, eps[epi].revisit_chain_, eps[epi].v_.expr_);
                struct sl_reg_alloc *call_arg_ra = EXPR_RVALUE(eps[epi].v_.expr_->children_[n]);
              
                sl_exec_move_param(exec, eps[epi].revisit_chain_, ef, param_ra, parent, call_arg_ra);
              }

              r = sl_exec_push_stmt(exec, ef->f_->body_, eps[epi].revisit_chain_, CHAIN_REF(eps[epi].alt_chain_));
              if (r) return r;
            
              eps[epi].revisit_chain_ = SL_EXEC_NO_CHAIN;
              dont_pop = 1;
            }
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
          case exop_conditional: {
            /* Move true results into our own register; register allocation tries to make this the same register
             * but can't guarantee it (e.g. if the true branch is a variable, it'll have a different reg.) */
            sl_exec_need_rvalue(exec, eps[epi].post_chain_, eps[epi].v_.expr_->children_[1]);
            sl_reg_move(exec, eps[epi].post_chain_, EXPR_RVALUE(eps[epi].v_.expr_->children_[1]), NULL, &eps[epi].v_.expr_->base_regs_, NULL);
            break;
          }
          case exop_logical_or:
          case exop_logical_and: {
            /* Move result from second child into result of logical-and or logical-or expression node */
            sl_exec_need_rvalue(exec, eps[epi].post_chain_, eps[epi].v_.expr_->children_[1]);
            sl_reg_move(exec, eps[epi].post_chain_, EXPR_RVALUE(eps[epi].v_.expr_->children_[1]), NULL, &eps[epi].v_.expr_->base_regs_, NULL);
            break;
          }
        }
        uint32_t *continuation_ep = (uint32_t *)(((char *)exec->execution_points_) + eps[epi].continue_chain_ptr_);
        *continuation_ep = sl_exec_join_chains(exec, *continuation_ep, eps[epi].post_chain_);
        eps[epi].post_chain_ = SL_EXEC_NO_CHAIN;
      }
      else if (eps[epi].alt_chain_ != SL_EXEC_NO_CHAIN) {
        switch (eps[epi].v_.expr_->op_) {
          case exop_conditional: {
            /* Handle completion of the false branch's execution */
          sl_exec_need_rvalue(exec, eps[epi].alt_chain_, eps[epi].v_.expr_->children_[2]);
          sl_reg_move(exec, eps[epi].alt_chain_, EXPR_RVALUE(eps[epi].v_.expr_->children_[2]), NULL, &eps[epi].v_.expr_->base_regs_, NULL);
            break;
          }
          case exop_function_call: {
            /* Copy back the "out" parameters and pop the function's frame */
            struct sl_execution_frame *func_frame = exec->execution_frames_ + exec->num_execution_frames_ - 1;
            struct sl_execution_frame *parent_frame = exec->execution_frames_ + exec->num_execution_frames_ - 2;

            size_t n;
            for (n = 0; n < func_frame->f_->num_parameters_; ++n) {
              struct sl_parameter *param = func_frame->f_->parameters_ + n;
              struct sl_reg_alloc *param_ra = &param->variable_->reg_alloc_;
              struct sl_reg_alloc *call_arg_ra = EXPR_RVALUE(eps[epi].v_.expr_->children_[n]);

              struct sl_type *param_type = param->type_;
              int qualifiers = sl_type_qualifiers(param_type);
              if ((qualifiers & SL_PARAMETER_QUALIFIER_OUT) && (qualifiers & SL_PARAMETER_QUALIFIER_INOUT)) {
                sl_exec_move_param(exec, eps[epi].revisit_chain_, parent_frame, call_arg_ra, func_frame, param_ra);
                if (eps[epi].v_.expr_->offset_reg_.kind_ != slrak_void) {
                  /* Store rvalue into actual lvalue passed in to the arg */
                  sl_exec_offset_store(exec, eps[epi].alt_chain_, 
                                       &eps[epi].v_.expr_->base_regs_, &eps[epi].v_.expr_->offset_reg_, 
                                       EXPR_RVALUE(eps[epi].v_.expr_));
                }
              }
            }
            exec->num_execution_frames_--;

            break;
          }
        }
        uint32_t *continuation_ep = (uint32_t *)(((char *)exec->execution_points_) + eps[epi].continue_chain_ptr_);
        *continuation_ep = sl_exec_join_chains(exec, *continuation_ep, eps[epi].alt_chain_);
        eps[epi].alt_chain_ = SL_EXEC_NO_CHAIN;
      }
      else {
        sl_exec_pop_ep(exec);
      }
    }
    else if (eps[epi].kind_ == SLEPK_BOOTSTRAP) {
      /* Bootstrap is a special case, it's the first thing that happens when we start running the execution. */
      if (eps[epi].enter_chain_ != SL_EXEC_NO_CHAIN) {
        r = sl_exec_push_stmt(exec, eps[epi].v_.f_->body_, eps[epi].enter_chain_, CHAIN_REF(eps[epi].post_chain_));
        if (r) return r;
        eps[epi].enter_chain_ = SL_EXEC_NO_CHAIN;
      }
      else {
        sl_exec_pop_ep(exec);
      }
    }
    else {
      /* Invalid execution point kind */
      assert(0);
    }
  }

  return 0;
}

void sl_exec_debug_dump_builtin(struct sl_execution *exec, int exec_chain, struct sl_expr *x) {
  sl_exec_need_rvalue(exec, exec_chain, x->children_[0]);

  struct sl_reg_alloc *ra = EXPR_RVALUE(x->children_[0]);

  size_t len_needed = sl_exec_offset_dump_strided(exec, NULL, exec_chain, ra, NULL, 1, 0);
  char *m = (char *)malloc(exec->dump_text_len_ + len_needed + 2);
  if (!m) return;
  memcpy(m, exec->dump_text_, exec->dump_text_len_);
  sl_exec_offset_dump_strided(exec, m + exec->dump_text_len_, exec_chain, ra, NULL, 1, 0);
  m[exec->dump_text_len_ + len_needed] = '\n';
  m[exec->dump_text_len_ + len_needed + 1] = '\0';
  if (exec->dump_text_) free(exec->dump_text_);
  exec->dump_text_ = m;
  exec->dump_text_len_ += len_needed + 1;
}

