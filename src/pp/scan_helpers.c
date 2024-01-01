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

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED
#include <math.h>
#endif

#ifndef INTTYPES_H_INCLUDED
#define INTTYPES_H_INCLUDED
#include <inttypes.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef PP_H_INCLUDED
#define PP_H_INCLUDED
#include "pp.h"
#endif

#ifndef SCAN_HELPERS_H_INCLUDED
#define SCAN_HELPERS_H_INCLUDED
#include "scan_helpers.h"
#endif

#ifndef PP_TOKENS_H_INCLUDED
#define PP_TOKENS_H_INCLUDED
#include "pp_tokens.h"
#endif

int sch_parse_decimal(struct preprocessor *pp, uint64_t *pval, const char *text, size_t len, struct situs *loc) {
  uint64_t val = 0;
  size_t n;
  for (n = 0; n < len; ++n) {
    uint64_t next_digit = text[n] - '0';
    if (val >(UINT64_MAX / 10)) {
      pp_error_loc(pp, loc, "Overflow on integer");
      val = 0;
      return -1;
    }
    val = val * 10;
    val += next_digit;
    if (val < next_digit) {
      pp_error_loc(pp, loc, "Overflow on integer");
      val = 0;
      return -1;
    }
  }
  *pval = val;
  return 0;
}


int sch_parse_octal(struct preprocessor *pp, uint64_t *pval, const char *text, size_t len, struct situs *loc) {
  uint64_t val = 0;
  size_t n;
  for (n = 1 /* leading 0 */; n < len; ++n) {
    uint64_t next_digit = text[n] - '0';
    if (val >(UINT64_MAX / 8)) {
      pp_error_loc(pp, loc, "Overflow on integer");
      val = 0;
      return -1;
    }
    val = val * 8;
    val += next_digit;
    if (val < next_digit) {
      pp_error_loc(pp, loc, "Overflow on integer");
      val = 0;
      return -1;
    }
  }
  *pval = val;
  return 0;
}

int sch_parse_hexadecimal(struct preprocessor *pp, uint64_t *pval, const char *text, size_t len, struct situs *loc) {
  uint64_t val = 0;
  size_t n;
  for (n = 2 /* "0x"|"0X" */; n < len; ++n) {
    uint64_t next_digit;
    if ((text[n] >= '0') && (text[n] <= '9')) {
      next_digit = text[n] - '0';
    }
    else if ((text[n] >= 'a') && (text[n] <= 'f')) {
      next_digit = text[n] - 'a' + 0xA;
    }
    else /* ((text[n] >= 'A') && (text[n] <= 'F')) */ {
      next_digit = text[n] - 'A' + 0xA;
    }

    if (val > (UINT64_MAX / 16)) {
      pp_error_loc(pp, loc, "Overflow on integer");
      val = 0;
      return -1;
    }
    val = val * 16;
    val += next_digit;
    if (val < next_digit) {
      pp_error_loc(pp, loc, "Overflow on integer");
      val = 0;
      return -1;
    }
  }
  *pval = val;
  return 0;
}

int sch_parse_binary(struct preprocessor *pp, uint64_t *pval, const char *text, size_t len, struct situs *loc) {
  uint64_t val = 0;
  size_t n;
  for (n = 2 /* "0b" */; n < len; ++n) {
    uint64_t next_digit;
    next_digit = text[n] - '0';

    if (val >(UINT64_MAX / 2)) {
      pp_error_loc(pp, loc, "Overflow on integer");
      val = 0;
      return -1;
    }
    val = val * 2;
    val += next_digit;
    if (val < next_digit) {
      pp_error_loc(pp, loc, "Overflow on integer");
      val = 0;
      return -1;
    }
  }
  *pval = val;
  return 0;
}

/* dec: decimal, looking at column "Decimal Constant" in 6.4.4.1 Integer Constants */
static void sch_process_dec_none(struct preprocessor *pp, uint64_t val, enum pp_value_type *pvt, struct situs *loc) {
  enum pp_value_type vt = PPVT_NO_VALUE;
  uint64_t one = 1;
  struct { int bits_; enum pp_value_type vt_signed; enum pp_value_type vt_unsigned; } bits_to_vt[] ={
    { pp->bits_per_int_, PPVT_INT, PPVT_UNSIGNED_INT },
    { pp->bits_per_long_, PPVT_LONG_INT, PPVT_UNSIGNED_LONG_INT },
    { pp->bits_per_long_long_, PPVT_LONG_LONG_INT, PPVT_UNSIGNED_LONG_LONG_INT }
  };

  int n;
  for (n = 0; n < sizeof(bits_to_vt) / sizeof(bits_to_vt[0]); ++n) {
    if (val <= ((one << (bits_to_vt[n].bits_ - 1)) - 1)) {
      vt = bits_to_vt[n].vt_signed;
      break;
    }
    else if (bits_to_vt[n].bits_ == 64) {
      /* Cannot shift left by 64 bits without overflow, so we have to special case this. */
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
    else if (val <= ((one << bits_to_vt[n].bits_) - 1)) {
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
  }

  if (vt == PPVT_NO_VALUE) {
    pp_error_loc(pp, loc, "Overflow on integer, value truncated");
    vt = bits_to_vt[sizeof(bits_to_vt) / sizeof(bits_to_vt[0]) - 1].vt_unsigned;
  }

  *pvt = vt;
}

void sch_process_pptk_dec_none(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc) {
  *pptk = pptk_alloc(pp, pp_chain, text, PPTK_INTEGER_LIT, loc);
  if (!*pptk) {
    pp_no_memory(pp);
    return;
  }
  sch_process_dec_none(pp, val, &(*pptk)->v_type_, loc);
  (*pptk)->v_.u64_ = val;
}

static void sch_process_dec_U(struct preprocessor *pp, uint64_t val, enum pp_value_type *pvt, struct situs *loc) {
  enum pp_value_type vt = PPVT_NO_VALUE;
  uint64_t one = 1;
  struct { int bits_; enum pp_value_type vt_unsigned; } bits_to_vt[] ={
    { pp->bits_per_int_, PPVT_UNSIGNED_INT },
    { pp->bits_per_long_, PPVT_UNSIGNED_LONG_INT },
    { pp->bits_per_long_long_, PPVT_UNSIGNED_LONG_LONG_INT }
  };

  int n;
  for (n = 0; n < sizeof(bits_to_vt) / sizeof(bits_to_vt[0]); ++n) {
    if (bits_to_vt[n].bits_ == 64) {
      /* Cannot shift left by 64 bits without overflow, so we have to special case this. */
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
    else if (val <= ((one << bits_to_vt[n].bits_) - 1)) {
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
  }

  if (vt == PPVT_NO_VALUE) {
    pp_error_loc(pp, loc, "Overflow on integer, value truncated");
    vt = bits_to_vt[sizeof(bits_to_vt) / sizeof(bits_to_vt[0]) - 1].vt_unsigned;
  }

  *pvt = vt;
}

void sch_process_pptk_dec_U(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc) {
  *pptk = pptk_alloc(pp, pp_chain, text, PPTK_INTEGER_LIT, loc);
  if (!*pptk) {
    pp_no_memory(pp);
    return;
  }
  sch_process_dec_U(pp, val, &(*pptk)->v_type_, loc);
  (*pptk)->v_.u64_ = val;
}

static void sch_process_dec_L(struct preprocessor *pp, uint64_t val, enum pp_value_type *pvt, struct situs *loc) {
  enum pp_value_type vt = PPVT_NO_VALUE;
  uint64_t one = 1;
  struct { int bits_; enum pp_value_type vt_signed; enum pp_value_type vt_unsigned; } bits_to_vt[] ={
    { pp->bits_per_long_, PPVT_LONG_INT, PPVT_UNSIGNED_LONG_INT },
    { pp->bits_per_long_long_, PPVT_LONG_LONG_INT, PPVT_UNSIGNED_LONG_LONG_INT }
  };

  int n;
  for (n = 0; n < sizeof(bits_to_vt) / sizeof(bits_to_vt[0]); ++n) {
    if (val <= ((one << (bits_to_vt[n].bits_ - 1)) - 1)) {
      vt = bits_to_vt[n].vt_signed;
      break;
    }
    else if (bits_to_vt[n].bits_ == 64) {
      /* Cannot shift left by 64 bits without overflow, so we have to special case this. */
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
    else if (val <= ((one << bits_to_vt[n].bits_) - 1)) {
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
  }

  if (vt == PPVT_NO_VALUE) {
    pp_error_loc(pp, loc, "Overflow on integer, value truncated");
    vt = bits_to_vt[sizeof(bits_to_vt) / sizeof(bits_to_vt[0]) - 1].vt_unsigned;
  }

  *pvt = vt;
}

void sch_process_pptk_dec_L(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc) {
  *pptk = pptk_alloc(pp, pp_chain, text, PPTK_INTEGER_LIT, loc);
  if (!*pptk) {
    pp_no_memory(pp);
    return;
  }
  sch_process_dec_L(pp, val, &(*pptk)->v_type_, loc);
  (*pptk)->v_.u64_ = val;
}

static void sch_process_dec_UL(struct preprocessor *pp, uint64_t val, enum pp_value_type *pvt, struct situs *loc) {
  enum pp_value_type vt = PPVT_NO_VALUE;
  uint64_t one = 1;
  struct { int bits_; enum pp_value_type vt_unsigned; } bits_to_vt[] ={
    { pp->bits_per_long_, PPVT_UNSIGNED_LONG_INT },
    { pp->bits_per_long_long_, PPVT_UNSIGNED_LONG_LONG_INT }
  };

  int n;
  for (n = 0; n < sizeof(bits_to_vt) / sizeof(bits_to_vt[0]); ++n) {
    if (bits_to_vt[n].bits_ == 64) {
      /* Cannot shift left by 64 bits without overflow, so we have to special case this. */
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
    else if (val <= ((one << bits_to_vt[n].bits_) - 1)) {
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
  }

  if (vt == PPVT_NO_VALUE) {
    pp_error_loc(pp, loc, "Overflow on integer, value truncated");
    vt = bits_to_vt[sizeof(bits_to_vt) / sizeof(bits_to_vt[0]) - 1].vt_unsigned;
  }

  *pvt = vt;
}

void sch_process_pptk_dec_UL(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc) {
  *pptk = pptk_alloc(pp, pp_chain, text, PPTK_INTEGER_LIT, loc);
  if (!*pptk) {
    pp_no_memory(pp);
    return;
  }
  sch_process_dec_UL(pp, val, &(*pptk)->v_type_, loc);
  (*pptk)->v_.u64_ = val;
}

static void sch_process_dec_LL(struct preprocessor *pp, uint64_t val, enum pp_value_type *pvt, struct situs *loc) {
  enum pp_value_type vt = PPVT_NO_VALUE;
  uint64_t one = 1;
  struct { int bits_; enum pp_value_type vt_signed; } bits_to_vt[] ={
    { pp->bits_per_long_long_, PPVT_LONG_LONG_INT }
  };

  int n;
  for (n = 0; n < sizeof(bits_to_vt) / sizeof(bits_to_vt[0]); ++n) {
    if (val <= ((one << (bits_to_vt[n].bits_ - 1)) - 1)) {
      vt = bits_to_vt[n].vt_signed;
      break;
    }
  }

  if (vt == PPVT_NO_VALUE) {
    pp_error_loc(pp, loc, "Overflow on integer, value truncated");
    vt = bits_to_vt[sizeof(bits_to_vt) / sizeof(bits_to_vt[0]) - 1].vt_signed;  
  }

  *pvt = vt;
}

void sch_process_pptk_dec_LL(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc) {
  *pptk = pptk_alloc(pp, pp_chain, text, PPTK_INTEGER_LIT, loc);
  if (!*pptk) {
    pp_no_memory(pp);
    return;
  }
  sch_process_dec_LL(pp, val, &(*pptk)->v_type_, loc);
  (*pptk)->v_.u64_ = val;
}

static void sch_process_dec_ULL(struct preprocessor *pp, uint64_t val, enum pp_value_type *pvt, struct situs *loc) {
  enum pp_value_type vt = PPVT_NO_VALUE;
  uint64_t one = 1;
  struct { int bits_; enum pp_value_type vt_unsigned; } bits_to_vt[] ={
    { pp->bits_per_long_long_, PPVT_UNSIGNED_LONG_LONG_INT }
  };

  int n;
  for (n = 0; n < sizeof(bits_to_vt) / sizeof(bits_to_vt[0]); ++n) {
    if (bits_to_vt[n].bits_ == 64) {
      /* Cannot shift left by 64 bits without overflow, so we have to special case this. */
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
    else if (val <= ((one << bits_to_vt[n].bits_) - 1)) {
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
  }

  if (vt == PPVT_NO_VALUE) {
    pp_error_loc(pp, loc, "Overflow on integer, value truncated");
    vt = bits_to_vt[sizeof(bits_to_vt) / sizeof(bits_to_vt[0]) - 1].vt_unsigned;
  }

  *pvt = vt;
}

void sch_process_pptk_dec_ULL(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc) {
  *pptk = pptk_alloc(pp, pp_chain, text, PPTK_INTEGER_LIT, loc);
  if (!*pptk) {
    pp_no_memory(pp);
    return;
  }
  sch_process_dec_ULL(pp, val, &(*pptk)->v_type_, loc);
  (*pptk)->v_.u64_ = val;
}


/* ohb: octal, hex, binary, looking at column "Octal or Hexadecimal Constant" in 6.4.4.1 Integer Constants */
static void sch_process_ohb_none(struct preprocessor *pp, uint64_t val, enum pp_value_type *pvt, struct situs *loc) {
  enum pp_value_type vt = PPVT_NO_VALUE;
  uint64_t one = 1;
  struct { int bits_; enum pp_value_type vt_signed; enum pp_value_type vt_unsigned; } bits_to_vt[] = {
    { pp->bits_per_int_, PPVT_INT, PPVT_UNSIGNED_INT },
    { pp->bits_per_long_, PPVT_LONG_INT, PPVT_UNSIGNED_LONG_INT },
    { pp->bits_per_long_long_, PPVT_LONG_LONG_INT, PPVT_UNSIGNED_LONG_LONG_INT }
  };

  int n;
  for (n = 0; n < sizeof(bits_to_vt) / sizeof(bits_to_vt[0]); ++n) {
    if (val <= ((one << (bits_to_vt[n].bits_ - 1)) - 1)) {
      vt = bits_to_vt[n].vt_signed;
      break;
    }
    else if (bits_to_vt[n].bits_ == 64) {
      /* Cannot shift left by 64 bits without overflow, so we have to special case this. */
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
    else if (val <= ((one << bits_to_vt[n].bits_) - 1)) {
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
  }

  if (vt == PPVT_NO_VALUE) {
    pp_error_loc(pp, loc, "Overflow on integer, value truncated");
    vt = bits_to_vt[sizeof(bits_to_vt) / sizeof(bits_to_vt[0]) - 1].vt_unsigned;
  }

  *pvt = vt;
}

void sch_process_pptk_ohb_none(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc) {
  *pptk = pptk_alloc(pp, pp_chain, text, PPTK_INTEGER_LIT, loc);
  if (!*pptk) {
    pp_no_memory(pp);
    return;
  }
  sch_process_ohb_none(pp, val, &(*pptk)->v_type_, loc);
  (*pptk)->v_.u64_ = val;
}

static void sch_process_ohb_U(struct preprocessor *pp, uint64_t val, enum pp_value_type *pvt, struct situs *loc) {
  enum pp_value_type vt = PPVT_NO_VALUE;
  uint64_t one = 1;
  struct { int bits_; enum pp_value_type vt_unsigned; } bits_to_vt[] = {
    { pp->bits_per_int_, PPVT_UNSIGNED_INT },
    { pp->bits_per_long_, PPVT_UNSIGNED_LONG_INT },
    { pp->bits_per_long_long_, PPVT_UNSIGNED_LONG_LONG_INT }
  };

  int n;
  for (n = 0; n < sizeof(bits_to_vt) / sizeof(bits_to_vt[0]); ++n) {
    if (bits_to_vt[n].bits_ == 64) {
      /* Cannot shift left by 64 bits without overflow, so we have to special case this. */
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
    else if (val <= ((one << bits_to_vt[n].bits_) - 1)) {
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
  }

  if (vt == PPVT_NO_VALUE) {
    pp_error_loc(pp, loc, "Overflow on integer, value truncated");
    vt = bits_to_vt[sizeof(bits_to_vt) / sizeof(bits_to_vt[0]) - 1].vt_unsigned;
  }

  *pvt = vt;
}

void sch_process_pptk_ohb_U(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc) {
  *pptk = pptk_alloc(pp, pp_chain, text, PPTK_INTEGER_LIT, loc);
  if (!*pptk) {
    pp_no_memory(pp);
    return;
  }
  sch_process_ohb_U(pp, val, &(*pptk)->v_type_, loc);
  (*pptk)->v_.u64_ = val;
}

static void sch_process_ohb_L(struct preprocessor *pp, uint64_t val, enum pp_value_type *pvt, struct situs *loc) {
  enum pp_value_type vt = PPVT_NO_VALUE;
  uint64_t one = 1;
  struct { int bits_; enum pp_value_type vt_signed; enum pp_value_type vt_unsigned; } bits_to_vt[] ={
    { pp->bits_per_long_, PPVT_LONG_INT, PPVT_UNSIGNED_LONG_INT },
    { pp->bits_per_long_long_, PPVT_LONG_LONG_INT, PPVT_UNSIGNED_LONG_LONG_INT }
  };

  int n;
  for (n = 0; n < sizeof(bits_to_vt) / sizeof(bits_to_vt[0]); ++n) {
    if (val <= ((one << (bits_to_vt[n].bits_ - 1)) - 1)) {
      vt = bits_to_vt[n].vt_signed;
      break;
    }
    else if (bits_to_vt[n].bits_ == 64) {
      /* Cannot shift left by 64 bits without overflow, so we have to special case this. */
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
    else if (val <= ((one << bits_to_vt[n].bits_) - 1)) {
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
  }

  if (vt == PPVT_NO_VALUE) {
    pp_error_loc(pp, loc, "Overflow on integer, value truncated");
    vt = bits_to_vt[sizeof(bits_to_vt) / sizeof(bits_to_vt[0]) - 1].vt_unsigned;
  }

  *pvt = vt;
}

void sch_process_pptk_ohb_L(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc) {
  *pptk = pptk_alloc(pp, pp_chain, text, PPTK_INTEGER_LIT, loc);
  if (!*pptk) {
    pp_no_memory(pp);
    return;
  }
  sch_process_ohb_L(pp, val, &(*pptk)->v_type_, loc);
  (*pptk)->v_.u64_ = val;
}

static void sch_process_ohb_UL(struct preprocessor *pp, uint64_t val, enum pp_value_type *pvt, struct situs *loc) {
  enum pp_value_type vt = PPVT_NO_VALUE;
  uint64_t one = 1;
  struct { int bits_; enum pp_value_type vt_unsigned; } bits_to_vt[] ={
    { pp->bits_per_long_, PPVT_UNSIGNED_LONG_INT },
    { pp->bits_per_long_long_, PPVT_UNSIGNED_LONG_LONG_INT }
  };

  int n;
  for (n = 0; n < sizeof(bits_to_vt) / sizeof(bits_to_vt[0]); ++n) {
    if (bits_to_vt[n].bits_ == 64) {
      /* Cannot shift left by 64 bits without overflow, so we have to special case this. */
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
    else if (val <= ((one << bits_to_vt[n].bits_) - 1)) {
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
  }

  if (vt == PPVT_NO_VALUE) {
    pp_error_loc(pp, loc, "Overflow on integer, value truncated");
    vt = bits_to_vt[sizeof(bits_to_vt) / sizeof(bits_to_vt[0]) - 1].vt_unsigned;
  }

  *pvt = vt;
}

void sch_process_pptk_ohb_UL(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc) {
  *pptk = pptk_alloc(pp, pp_chain, text, PPTK_INTEGER_LIT, loc);
  if (!*pptk) {
    pp_no_memory(pp);
    return;
  }
  sch_process_ohb_UL(pp, val, &(*pptk)->v_type_, loc);
  (*pptk)->v_.u64_ = val;
}

static void sch_process_ohb_LL(struct preprocessor *pp, uint64_t val, enum pp_value_type *pvt, struct situs *loc) {
  enum pp_value_type vt = PPVT_NO_VALUE;
  uint64_t one = 1;
  struct { int bits_; enum pp_value_type vt_signed; enum pp_value_type vt_unsigned; } bits_to_vt[] ={
    { pp->bits_per_long_long_, PPVT_LONG_LONG_INT, PPVT_UNSIGNED_LONG_LONG_INT }
  };

  int n;
  for (n = 0; n < sizeof(bits_to_vt) / sizeof(bits_to_vt[0]); ++n) {
    if (val <= ((one << (bits_to_vt[n].bits_ - 1)) - 1)) {
      vt = bits_to_vt[n].vt_signed;
      break;
    }
    else if (bits_to_vt[n].bits_ == 64) {
      /* Cannot shift left by 64 bits without overflow, so we have to special case this. */
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
    else if (val <= ((one << bits_to_vt[n].bits_) - 1)) {
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
  }

  if (vt == PPVT_NO_VALUE) {
    pp_error_loc(pp, loc, "Overflow on integer, value truncated");
    vt = bits_to_vt[sizeof(bits_to_vt) / sizeof(bits_to_vt[0]) - 1].vt_unsigned;
  }

  *pvt = vt;
}

void sch_process_pptk_ohb_LL(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc) {
  *pptk = pptk_alloc(pp, pp_chain, text, PPTK_INTEGER_LIT, loc);
  if (!*pptk) {
    pp_no_memory(pp);
    return;
  }
  sch_process_ohb_LL(pp, val, &(*pptk)->v_type_, loc);
  (*pptk)->v_.u64_ = val;
}

static void sch_process_ohb_ULL(struct preprocessor *pp, uint64_t val, enum pp_value_type *pvt, struct situs *loc) {
  enum pp_value_type vt = PPVT_NO_VALUE;
  uint64_t one = 1;
  struct { int bits_; enum pp_value_type vt_unsigned; } bits_to_vt[] ={
    { pp->bits_per_long_long_, PPVT_UNSIGNED_LONG_LONG_INT }
  };

  int n;
  for (n = 0; n < sizeof(bits_to_vt) / sizeof(bits_to_vt[0]); ++n) {
    if (bits_to_vt[n].bits_ == 64) {
      /* Cannot shift left by 64 bits without overflow, so we have to special case this. */
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
    else if (val <= ((one << bits_to_vt[n].bits_) - 1)) {
      vt = bits_to_vt[n].vt_unsigned;
      break;
    }
  }

  if (vt == PPVT_NO_VALUE) {
    pp_error_loc(pp, loc, "Overflow on integer, value truncated");
    vt = bits_to_vt[sizeof(bits_to_vt) / sizeof(bits_to_vt[0]) - 1].vt_unsigned;
  }

  *pvt = vt;
}

void sch_process_pptk_ohb_ULL(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc) {
  *pptk = pptk_alloc(pp, pp_chain, text, PPTK_INTEGER_LIT, loc);
  if (!*pptk) {
    pp_no_memory(pp);
    return;
  }
  sch_process_ohb_ULL(pp, val, &(*pptk)->v_type_, loc);
  (*pptk)->v_.u64_ = val;
}

int sch_read_oct_esc_value(const char *octal_num, int *val) {
  int n = 0;
  int v = 0;
  /* Octal escapes can be at most 3 digits long (eg. see C99 6.4.4.4 paragraph 14) */
  for (n = 0; n < 3; ++n) {
    if ((octal_num[n] >= '0') && (octal_num[n] <= '7')) {
      v = v << 3;
      v += octal_num[n] - '0';
    }
    else {
      break;
    }
  }
  *val = v;
  return n;
}

int sch_read_hex_esc_value(const char *hex_xx_num, int *val) {
  int n = 0;
  int v = 0;

  /* Hexadecimal escapes can be arbitrarily long (eg. see C99 6.4.4.4 paragraph 14) */
  for (;;) {
    if ((hex_xx_num[n] >= '0') && (hex_xx_num[n] <= '9')) {
      v = v << 4;
      v += hex_xx_num[n] - '0';
    }
    else if ((hex_xx_num[n] >= 'a') && (hex_xx_num[n] <= 'f')) {
      v = v << 4;
      v += hex_xx_num[n] - 'a' + 0xA;
    }
    else if ((hex_xx_num[n] >= 'A') && (hex_xx_num[n] <= 'F')) {
      v = v << 4;
      v += hex_xx_num[n] - 'A' + 0xA;
    }
    else {
      *val = v;
      return n;
    }

    ++n;
  }
}

int sch_read_char_value(struct preprocessor *pp, const char *lit_pos, int *val, int max_char, struct situs *loc, const char *kind) {
  const unsigned char *c = (const unsigned char *)lit_pos;
  if (*c != '\\') {
    if (*c == '\n') {
      pp_error_loc(pp, loc, "Newline in %s constant", kind);
    }
    *val = *c;
    return 1;
  }
  int num_digits, num;
  switch (c[1]) {
    case 'a': *val = '\a'; return 2;
    case 'b': *val = '\b'; return 2;
    case 'f': *val = '\f'; return 2;
    case 'n': *val = '\n'; return 2;
    case 'r': *val = '\r'; return 2;
    case 't': *val = '\t'; return 2;
    case 'v': *val = '\v'; return 2;
    case '\'': *val = '\''; return 2;
    case '\"': *val = '\"'; return 2;
    case '\\': *val = '\\'; return 2;
    case '?': *val = '\?'; return 2;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
      num_digits = sch_read_oct_esc_value(c + 1, &num);
      if ((!num_digits) || (num > max_char)) {
        pp_error_loc(pp, loc, "Invalid octal escape sequence");
        *val = *c;
        return 1;
      }
      else {
        *val = num;
        return 1 + num_digits;
      }
      break;
    case 'x':
      num_digits = sch_read_hex_esc_value(c + 1, &num);
      if ((!num_digits) || (num > max_char)) {
        pp_error_loc(pp, loc, "Invalid hex escape sequence");
        *val = *c;
        return 1;
      }
      else {
        *val = num;
        return 2 + num_digits;
      }
      break;
    default:
      pp_error_loc(pp, loc, "Invalid escape sequence \"\\%c\"\n", c[1]);
      *val = *c;
      return 1;

  }
}
