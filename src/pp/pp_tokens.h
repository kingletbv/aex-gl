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

#ifndef PPTOKENS_H
#define PPTOKENS_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef SITUS_H_INCLUDED
#define SITUS_H_INCLUDED
#include "situs.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Returned by the line directives parser when a file is to be included */
#define PPLD_INCLUDE_FILE -1

/* Returned by the pptokenizer to indicate a line of input tokens is ready
 * Careful what this value is, it should not collide with existing constants _PPTK_FINISH to _PPTK_INTERNAL_ERROR. Those constants are all positive, hence us going for negative. */
#define PPTK_TOKENIZER_LINE_READY -1

/* Returned by the pptokenizer to suggest the caller should check whether 
 * a headername is currently acceptable by the grammer. 
 * If so, it should set pptk_set_mode(&pptokenizer, M_PPTK_HEADER_NAME_POSSIBLE) 
 * to ensure the headername will be tokenized. */
#define PPTK_TOKENIZER_HEADERNAME_CHECK -2

/* xx(PPTK_CONSTANT, PPLD_CONSTANT, PPCE_CONSTANT, PPME_CONSTANT)
 * PPLD_CONSTANTs are from pp_line_directives.h (generated from pp_line_directives.cbrt) */

#define PPTK_ENUM_PPTOKENS \
  /* 0 value for invalid is important so array initializers will default to invalid if unspecified.*/ \
  xx(PPTK_INVALID,            0,                   PPCE_OTHER,               PPME_OTHER,      CP_OTHER) \
  xx(PPTK_BANG,               PPLD_OTHER_TOK,      PPCE_BANG,                PPME_OTHER,      CP_BANG) \
  xx(PPTK_TILDE,              PPLD_OTHER_TOK,      PPCE_TILDE,               PPME_OTHER,      CP_TILDE) \
  xx(PPTK_PERCENT,            PPLD_OTHER_TOK,      PPCE_PERCENT,             PPME_OTHER,      CP_PERCENT) \
  xx(PPTK_PERCENT_EQUALS,     PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_PERCENT_EQUALS) \
  xx(PPTK_AMPERSAND,          PPLD_OTHER_TOK,      PPCE_AMPERSAND,           PPME_OTHER,      CP_AMPERSAND) \
  xx(PPTK_AMPERSAND_AMPERSAND, PPLD_OTHER_TOK,     PPCE_AMPERSAND_AMPERSAND, PPME_OTHER,      CP_AMPERSAND_AMPERSAND) \
  xx(PPTK_AMPERSAND_EQUALS,   PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_AMPERSAND_EQUALS) \
  xx(PPTK_PAR_OPEN,           PPLD_PAR_OPEN_TOK,   PPCE_PAR_OPEN,            PPME_PAR_OPEN,   CP_PAR_OPEN) \
  xx(PPTK_PAR_CLOSE,          PPLD_PAR_CLOSE_TOK,  PPCE_PAR_CLOSE,           PPME_PAR_CLOSE,  CP_PAR_CLOSE) \
  xx(PPTK_ASTERISK,           PPLD_OTHER_TOK,      PPCE_ASTERISK,            PPME_OTHER,      CP_ASTERISK) \
  xx(PPTK_ASTERISK_EQUALS,    PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_ASTERISK_EQUALS) \
  xx(PPTK_PLUS,               PPLD_OTHER_TOK,      PPCE_PLUS,                PPME_OTHER,      CP_PLUS) \
  xx(PPTK_PLUS_PLUS,          PPLD_OTHER_TOK,      PPCE_PLUS_PLUS,           PPME_OTHER,      CP_PLUS_PLUS) \
  xx(PPTK_PLUS_EQUALS,        PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_PLUS_EQUALS) \
  xx(PPTK_COMMA,              PPLD_COMMA_TOK,      PPCE_OTHER,               PPME_COMMA,      CP_COMMA) \
  xx(PPTK_MINUS,              PPLD_OTHER_TOK,      PPCE_MINUS,               PPME_OTHER,      CP_MINUS) \
  xx(PPTK_MINUS_MINUS,        PPLD_OTHER_TOK,      PPCE_MINUS_MINUS,         PPME_OTHER,      CP_MINUS_MINUS) \
  xx(PPTK_MINUS_EQUALS,       PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_MINUS_EQUALS) \
  xx(PPTK_DOT,                PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_DOT) \
  xx(PPTK_DOT_DOT_DOT,        PPLD_ELLIPSIS_TOK,   PPCE_OTHER,               PPME_OTHER,      CP_DOT_DOT_DOT) \
  xx(PPTK_SLASH,              PPLD_OTHER_TOK,      PPCE_SLASH,               PPME_OTHER,      CP_SLASH) \
  xx(PPTK_SLASH_EQUALS,       PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_SLASH_EQUALS) \
  xx(PPTK_COLON,              PPLD_OTHER_TOK,      PPCE_COLON,               PPME_OTHER,      CP_COLON) \
  xx(PPTK_SEMICOLON,          PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_SEMICOLON) \
  xx(PPTK_LT,                 PPLD_OTHER_TOK,      PPCE_LT,                  PPME_OTHER,      CP_LT) \
  xx(PPTK_LT_LT,              PPLD_OTHER_TOK,      PPCE_LT_LT,               PPME_OTHER,      CP_LT_LT) \
  xx(PPTK_LT_LT_EQUALS,       PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_LT_LT_EQUALS) \
  xx(PPTK_LT_EQUALS,          PPLD_OTHER_TOK,      PPCE_LT_EQUALS,           PPME_OTHER,      CP_LT_EQUALS) \
  xx(PPTK_EQUALS,             PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_EQUALS) \
  xx(PPTK_EQUALS_EQUALS,      PPLD_OTHER_TOK,      PPCE_EQUALS_EQUALS,       PPME_OTHER,      CP_EQUALS_EQUALS) \
  xx(PPTK_BANG_EQUALS,        PPLD_OTHER_TOK,      PPCE_BANG_EQUALS,         PPME_OTHER,      CP_BANG_EQUALS) \
  xx(PPTK_GT,                 PPLD_OTHER_TOK,      PPCE_GT,                  PPME_OTHER,      CP_GT) \
  xx(PPTK_GT_EQUALS,          PPLD_OTHER_TOK,      PPCE_GT_EQUALS,           PPME_OTHER,      CP_GT_EQUALS) \
  xx(PPTK_GT_GT,              PPLD_OTHER_TOK,      PPCE_GT_GT,               PPME_OTHER,      CP_GT_GT) \
  xx(PPTK_GT_GT_EQUALS,       PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_GT_GT_EQUALS) \
  xx(PPTK_ARROW,              PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_ARROW) \
  xx(PPTK_SQBRACE_OPEN,       PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_SQBRACE_OPEN) \
  xx(PPTK_SQBRACE_CLOSE,      PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_SQBRACE_CLOSE) \
  xx(PPTK_CARET,              PPLD_OTHER_TOK,      PPCE_CARET,               PPME_OTHER,      CP_CARET) \
  xx(PPTK_CARET_EQUALS,       PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_CARET_EQUALS) \
  xx(PPTK_CUBRACE_OPEN,       PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_CUBRACE_OPEN) \
  xx(PPTK_CUBRACE_CLOSE,      PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_CUBRACE_CLOSE) \
  xx(PPTK_BAR,                PPLD_OTHER_TOK,      PPCE_BAR,                 PPME_OTHER,      CP_BAR) \
  xx(PPTK_BAR_EQUALS,         PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_BAR_EQUALS) \
  xx(PPTK_BAR_BAR,            PPLD_OTHER_TOK,      PPCE_BAR_BAR,             PPME_OTHER,      CP_BAR_BAR) \
  xx(PPTK_QUESTION_MARK,      PPLD_OTHER_TOK,      PPCE_QUESTION_MARK,       PPME_OTHER,      CP_QUESTION_MARK) \
  xx(PPTK_HASH_MARK,          PPLD_HASH_TOK,       PPCE_OTHER,               PPME_OTHER,      CP_OTHER) \
  xx(PPTK_HASH_HASH_MARK,     PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_OTHER) \
  xx(PPTK_AUTO,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_AUTO) \
  xx(PPTK_BREAK,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_BREAK) \
  xx(PPTK_CASE,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_CASE) \
  xx(PPTK_CHAR,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_CHAR) \
  xx(PPTK_CONST,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_CONST) \
  xx(PPTK_CONTINUE,           PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_CONTINUE) \
  xx(PPTK_DEFAULT,            PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_DEFAULT) \
  xx(PPTK_DO,                 PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_DO) \
  xx(PPTK_DOUBLE,             PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_DOUBLE) \
  xx(PPTK_ELSE,               PPLD_ELSE_TOK,       PPCE_IDENT,               PPME_IDENT,      CP_ELSE) \
  xx(PPTK_ENUM,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_ENUM) \
  xx(PPTK_EXTERN,             PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_EXTERN) \
  xx(PPTK_FLOAT,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_FLOAT) \
  xx(PPTK_FOR,                PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_FOR) \
  xx(PPTK_GOTO,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_GOTO) \
  xx(PPTK_IF,                 PPLD_IF_TOK,         PPCE_IDENT,               PPME_IDENT,      CP_IF) \
  xx(PPTK_INLINE,             PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_INLINE) \
  xx(PPTK_INT,                PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_INT) \
  xx(PPTK_LONG,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_LONG) \
  xx(PPTK_REGISTER,           PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_REGISTER) \
  xx(PPTK_RESTRICT,           PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_RESTRICT) \
  xx(PPTK_RETURN,             PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_RETURN) \
  xx(PPTK_SHORT,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_SHORT) \
  xx(PPTK_SIGNED,             PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_SIGNED) \
  xx(PPTK_SIZEOF,             PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_SIZEOF) \
  xx(PPTK_STATIC,             PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_STATIC) \
  xx(PPTK_STRUCT,             PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_STRUCT) \
  xx(PPTK_SWITCH,             PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_SWITCH) \
  xx(PPTK_TYPEDEF,            PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_TYPEDEF) \
  xx(PPTK_UNION,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_UNION) \
  xx(PPTK_UNSIGNED,           PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_UNSIGNED) \
  xx(PPTK_VOID,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_VOID) \
  xx(PPTK_VOLATILE,           PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_VOLATILE) \
  xx(PPTK_WHILE,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_WHILE) \
  xx(PPTK_BOOL,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_BOOL) /* "_Bool" */ \
  xx(PPTK_COMPLEX,            PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_COMPLEX) \
  xx(PPTK_IMAGINARY,          PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IMAGINARY) \
  xx(PPTK_INTEGER_LIT,        PPLD_OTHER_TOK,      PPCE_INTEGER_LIT,         PPME_OTHER,      CP_INTEGER_LIT) \
  xx(PPTK_FLOAT_LIT,          PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_FLOAT_LIT) \
  xx(PPTK_STRING_LIT,         PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_STRING_LIT) \
  xx(PPTK_CHAR_LIT,           PPLD_OTHER_TOK,      PPCE_CHAR_LIT,            PPME_OTHER,      CP_CHAR_LIT) \
  xx(PPTK_IDENT,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_WHITESPACE,         PPLD_WS_TOK,         PPCE_WS,                  PPME_WS_TOK,     CP_WHITESPACE) \
  xx(PPTK_NEWLINE_WHITESPACE, PPLD_NEWLINE_TOK,    PPCE_WS,                  PPME_WS_TOK,     CP_WHITESPACE) \
\
  xx(PPTK_HEADER_NAME,        PPLD_HEADER_NAME,    PPCE_OTHER,               PPME_OTHER,      CP_OTHER) \
\
  xx(PPTK_IFDEF,              PPLD_IFDEF_TOK,      PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_IFNDEF,             PPLD_IFNDEF_TOK,     PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_ELIF,               PPLD_ELIF_TOK,       PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_ENDIF,              PPLD_ENDIF_TOK,      PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_INCLUDE,            PPLD_INCLUDE_TOK,    PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_EMIT,               PPLD_INCLUDE_TOK,    PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_DEFINE,             PPLD_DEFINE_TOK,     PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_UNDEF,              PPLD_UNDEF_TOK,      PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_LINE,               PPLD_LINE_TOK,       PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_ERROR_KEYWORD,      PPLD_ERROR_TOK,      PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_PRAGMA,             PPLD_PRAGMA_TOK,     PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_DEFINED,            PPLD_IDENTIFIER_TOK, PPCE_DEFINED,             PPME_DEFINED,    CP_IDENT) \
  xx(PPTK_PRAGMA_OP,          PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
\
  xx(PPTK_BOOL_ID,            PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_OTHER,      CP_IDENT) /* "bool" */ \
  xx(PPTK_BOOL_CONSTANT,      PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_OTHER,      CP_IDENT) \
  xx(PPTK_CARET_CARET,        PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_IDENT,      CP_OTHER) \
  xx(PPTK_ATTRIBUTE,          PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_DISCARD,            PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_BVEC2,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_BVEC3,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_BVEC4,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_IVEC2,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_IVEC3,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_IVEC4,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_VEC2,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_VEC3,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_VEC4,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_MAT2,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_MAT3,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_MAT4,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_IN,                 PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_OUT,                PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_INOUT,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_UNIFORM,            PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_VARYING,            PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_SAMPLER2D,          PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_SAMPLERCUBE,        PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_INVARIANT,          PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_HIGH_PRECISION,     PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_MEDIUM_PRECISION,   PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_LOW_PRECISION,      PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_PRECISION,          PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
\
  /* tokens reserved by GLSL 1 for future use */ \
  xx(PPTK_ASM,                PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_CLASS,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_TEMPLATE,           PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_THIS,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_PACKED,             PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_NOINLINE,           PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_PUBLIC,             PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_EXTERNAL,           PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_INTERFACE,          PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_FLAT,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_HALF,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_FIXED,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_SUPERP,             PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_INPUT,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_OUTPUT,             PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_HVEC2,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_HVEC3,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_HVEC4,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_DVEC2,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_DVEC3,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_DVEC4,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_FVEC2,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_FVEC3,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_FVEC4,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_SAMPLER1D,          PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_SAMPLER3D,          PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_SAMPLER1DSHADOW,    PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_SAMPLER2DSHADOW,    PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_SAMPLER2DRECT,      PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_SAMPLER3DRECT,      PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_SAMPLER2DRECTSHADOW,PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_CAST,               PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_NAMESPACE,          PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_USING,              PPLD_IDENTIFIER_TOK, PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
  xx(PPTK_VERSION,            PPLD_VERSION_TOK,    PPCE_IDENT,               PPME_IDENT,      CP_IDENT) \
\
  /* placemarker used during macro expansion of empty arguments adjacent to ## tokens (See C99 6.10.3.3-2) */ \
  xx(PPTK_PLACEMARKER,        0,                   0,                        0,               0) \
\
  /* typedef-name, just prior to entering the C parser a pptk CP_IDENT may be promoted to a CP_TYPEDEF_NAME
   * if the identifier looks up to a typedef. This resolves a famous C ambiguity. */ \
  xx(PPTK_TYPEDEF_NAME,       PPLD_IDENT,          PPCE_IDENT,               PPME_IDENT,      CP_TYPEDEF_NAME) \
\
  /* literal text used when in template mode (this is text outside of {{ code_sections(); }} )*/ \
  xx(PPTK_TEMPLATE_LIT,       PPLD_OTHER_TOK,      PPCE_OTHER,               PPME_OTHER,      CP_TEMPLATE_LIT) \
  xx(PPTK_TEMPLATE_DOUBLE_CURLY_OPEN, PPLD_OTHER_TOK, PPCE_OTHER,            PPME_OTHER,      CP_OTHER) \
  xx(PPTK_TEMPLATE_DOUBLE_CURLY_CLOSE, PPLD_OTHER_TOK, PPCE_OTHER,           PPME_OTHER,      CP_OTHER) \


enum pptoken {
  /* Start at 0 is important for PPTK_INVALID, see comment above */
#define xx(id, ld_id, ppce_id, ppme_id, cp_id) id,
  PPTK_ENUM_PPTOKENS
#undef xx
};

enum pp_value_type {
  PPVT_SHORT,
  PPVT_INT,
  PPVT_LONG_INT,
  PPVT_LONG_LONG_INT,
  PPVT_UNSIGNED_SHORT,
  PPVT_UNSIGNED_INT,
  PPVT_UNSIGNED_LONG_INT,
  PPVT_UNSIGNED_LONG_LONG_INT,
  PPVT_FLOAT,
  PPVT_DOUBLE,
  PPVT_LONG_DOUBLE,
  PPVT_STRING,
  PPVT_TYPE_NAME,
  PPVT_NO_VALUE
};

struct sl_type;

struct pptk {
  struct pptk *next_, *prev_;
  enum pptoken tok_;
  size_t text_len_;
  char *text_;
  struct situs situs_;
  enum pp_value_type v_type_;
  union {
    struct {            /* ... if the tok == PPTK_STRING_LIT, then it holds the string literal. */
      /* String literals are not represented as expr_ dsp_ values because they may still be subject to string
       * concatenation (in which case you don't want to have reserved the final object space data.)
       * Conversion into expr occurs in c_parser.cbrt */
      int wide_:1;      /* ... if non-zero, this is a wide string, otherwise a char string */
      void *data_;      /* pointer to string data, can be either 16 bit or 8 bit data string, blob */
      size_t length_;   /* length of string data in characters (16 or 8 bit); excluding the null terminator that ends it. */
    } string_;
    uint64_t u64_;    /* PPVT_UNSIGNED_SHORT, PPVT_UNSIGNED_INT, PPVT_UNSIGNED_LONG_INT, PPVT_UNSIGNED_LONG_LONG_INT */
    int64_t i64_;     /* PPVT_SHORT, PPVT_INT, PPVT_LONG_INT, PPVT_LONG_LONG_INT */
    double d_;        /* PPVT_DOUBLE, PPVT_LONG_DOUBLE */
    float f_;         /* PPVT_FLOAT */ 
    struct sl_type *type_;      /* PPVT_TYPE_NAME */
  } v_;
};

struct macro_arg_inst {
  struct macro_arg_inst *next_, *prev_;
  struct pptk *tokens_;
};

extern int g_pptk_to_ppld_[];
extern int g_pptk_to_ppce_[];
extern int g_pptk_to_ppme_[];
extern int g_pptk_to_cp_[];

struct pptk *pptk_alloc(struct preprocessor *pp, struct pptk **pp_chain, const char *text, int sym, struct situs *psit);
struct pptk *pptk_alloc_len(struct preprocessor *pp, struct pptk **pp_chain, const char *text, size_t text_len, int sym, struct situs *psit);
void pptk_free(struct pptk *tk);
struct pptk *pptk_join(struct pptk *front, struct pptk *back);

struct pptk *pptk_pop_front(struct pptk **pp_chain);
struct pptk *pptk_pop_back(struct pptk **pp_chain);
struct pptk *pptk_trim_front(struct pptk *chain);
struct pptk *pptk_trim_back(struct pptk *chain);
struct pptk *pptk_trim(struct pptk *chain);

struct pptk *pptk_clone_single(struct preprocessor *pp, struct pptk *one);
struct pptk *pptk_clone(struct preprocessor *pp, struct pptk *chain);
struct pptk *pptk_clone_trimmed(struct preprocessor *pp, struct pptk *chain);

struct macro_arg_inst *macro_arg_inst_alloc(struct pptk *tokens);
void macro_arg_inst_free(struct macro_arg_inst *mai);

struct macro_arg_inst *macro_arg_inst_join(struct macro_arg_inst *front, struct macro_arg_inst *back);

/* Expands a single macro.. */
int macro_expand(struct preprocessor *pp, struct pptk *macro_ident, struct macro *m, struct macro_arg_inst *args, struct pptk **pp_output_chain);

/* Perform in-place macro expansion for a single chain of pptk tokens; chain is modified in-place to reflect the expansions (if any).
 * keep_defined determines if, in the input "defined(X)", the tokens are retained and not expanded (e.g. suppose "defined" is a
 * macro, it would not be expanded, suppose X is a macro, it would also not be expanded.) This should only be relevant when evaluating
 * "#if" line directives, and should be set to 0 for other cases. */
int pptk_perform_macro_expansion(struct preprocessor *pp, struct pptk **pp_chain, int keep_defined);

/* Re-scans an input sequence by taking an input_chain of tokens and  */
int pptk_rescan(struct preprocessor *pp, struct pptk *input_chain, struct pptk **pp_output_chain, int allow_header_name);

/* Scans a text string into a chain of tokens, performing no further processing - dud source location situs, so should not
 * be used in core compiler work, more intended for command line macro arguments etc. */
int pptk_scan_str(struct preprocessor *pp, const char *text, struct pptk **pp_output_chain);

/* Converts a chain of tokens into a string, allocating the string on the heap. Caller should release the memory
 * with free(). */
char *pptk_to_str(struct preprocessor *pp, struct pptk *chain);

/* Evaluate the preprocessor const expression as formed by the tokens in the input chain and store the evaluated result in *result and
 * whether the result is signed or unsigned in *is_unsigned. The result is always either int64_t or uint64_t and so a truth test need
 * not depend on *is_unsigned. */
int ppce_eval_const_expr(struct preprocessor *pp, struct pptk *input_chain, int64_t *result, int *is_unsigned);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PPTOKENS_H */
