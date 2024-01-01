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

#ifndef GLSL_ES1_TOKENS_H
#define GLSL_ES1_TOKENS_H

#ifndef PP_TOKENS_H_INCLUDED
#define PP_TOKENS_H_INCLUDED
#include "pp_tokens.h"
#endif

#ifndef GLSL_ES_1_H_INCLUDED
#define GLSL_ES_1_H_INCLUDED
#include "glsl_es_1.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define ENUM_GLSL_ES1_TOKENS\
  /* 0 value for invalid is important so array initializers will default to invalid if unspecified.*/ \
  xx(PPTK_INVALID,            GLSL_ES1_OTHER) \
  xx(PPTK_BANG,               GLSL_ES1_BANG) \
  xx(PPTK_TILDE,              GLSL_ES1_TILDE) \
  xx(PPTK_PERCENT,            GLSL_ES1_PERCENT) \
  xx(PPTK_PERCENT_EQUALS,     GLSL_ES1_MOD_ASSIGN) \
  xx(PPTK_AMPERSAND,          GLSL_ES1_AMPERSAND) \
  xx(PPTK_AMPERSAND_AMPERSAND, GLSL_ES1_AND_OP) \
  xx(PPTK_AMPERSAND_EQUALS,   GLSL_ES1_AND_ASSIGN) \
  xx(PPTK_PAR_OPEN,           GLSL_ES1_LEFT_PAREN) \
  xx(PPTK_PAR_CLOSE,          GLSL_ES1_RIGHT_PAREN) \
  xx(PPTK_ASTERISK,           GLSL_ES1_STAR) \
  xx(PPTK_ASTERISK_EQUALS,    GLSL_ES1_MUL_ASSIGN) \
  xx(PPTK_PLUS,               GLSL_ES1_PLUS) \
  xx(PPTK_PLUS_PLUS,          GLSL_ES1_INC_OP) \
  xx(PPTK_PLUS_EQUALS,        GLSL_ES1_ADD_ASSIGN) \
  xx(PPTK_COMMA,              GLSL_ES1_COMMA) \
  xx(PPTK_MINUS,              GLSL_ES1_DASH) \
  xx(PPTK_MINUS_MINUS,        GLSL_ES1_DEC_OP) \
  xx(PPTK_MINUS_EQUALS,       GLSL_ES1_SUB_ASSIGN) \
  xx(PPTK_DOT,                GLSL_ES1_DOT) \
  xx(PPTK_DOT_DOT_DOT,        GLSL_ES1_OTHER) \
  xx(PPTK_SLASH,              GLSL_ES1_SLASH) \
  xx(PPTK_SLASH_EQUALS,       GLSL_ES1_DIV_ASSIGN) \
  xx(PPTK_COLON,              GLSL_ES1_COLON) \
  xx(PPTK_SEMICOLON,          GLSL_ES1_SEMICOLON) \
  xx(PPTK_LT,                 GLSL_ES1_LEFT_ANGLE) \
  xx(PPTK_LT_LT,              GLSL_ES1_LEFT_OP) \
  xx(PPTK_LT_LT_EQUALS,       GLSL_ES1_LEFT_ASSIGN) \
  xx(PPTK_LT_EQUALS,          GLSL_ES1_LE_OP) \
  xx(PPTK_EQUALS,             GLSL_ES1_EQUAL) \
  xx(PPTK_EQUALS_EQUALS,      GLSL_ES1_EQ_OP) \
  xx(PPTK_BANG_EQUALS,        GLSL_ES1_NE_OP) \
  xx(PPTK_GT,                 GLSL_ES1_RIGHT_ANGLE) \
  xx(PPTK_GT_EQUALS,          GLSL_ES1_GE_OP) \
  xx(PPTK_GT_GT,              GLSL_ES1_RIGHT_OP) \
  xx(PPTK_GT_GT_EQUALS,       GLSL_ES1_RIGHT_ASSIGN) \
  xx(PPTK_ARROW,              GLSL_ES1_OTHER) \
  xx(PPTK_SQBRACE_OPEN,       GLSL_ES1_LEFT_BRACKET) \
  xx(PPTK_SQBRACE_CLOSE,      GLSL_ES1_RIGHT_BRACKET) \
  xx(PPTK_CARET,              GLSL_ES1_CARET) \
  xx(PPTK_CARET_EQUALS,       GLSL_ES1_XOR_ASSIGN) \
  xx(PPTK_CUBRACE_OPEN,       GLSL_ES1_LEFT_BRACE) \
  xx(PPTK_CUBRACE_CLOSE,      GLSL_ES1_RIGHT_BRACE) \
  xx(PPTK_BAR,                GLSL_ES1_VERTICAL_BAR) \
  xx(PPTK_BAR_EQUALS,         GLSL_ES1_OR_ASSIGN) \
  xx(PPTK_BAR_BAR,            GLSL_ES1_OR_OP) \
  xx(PPTK_QUESTION_MARK,      GLSL_ES1_QUESTION) \
  xx(PPTK_HASH_MARK,          GLSL_ES1_OTHER) \
  xx(PPTK_HASH_HASH_MARK,     GLSL_ES1_OTHER) \
  xx(PPTK_AUTO,               GLSL_ES1_IDENTIFIER) \
  xx(PPTK_BREAK,              GLSL_ES1_BREAK) \
  xx(PPTK_CASE,               GLSL_ES1_IDENTIFIER) \
  xx(PPTK_CHAR,               GLSL_ES1_IDENTIFIER) \
  xx(PPTK_CONST,              GLSL_ES1_CONST) \
  xx(PPTK_CONTINUE,           GLSL_ES1_CONTINUE) \
  xx(PPTK_DEFAULT,            GLSL_ES1_OTHER) \
  xx(PPTK_DO,                 GLSL_ES1_DO) \
  xx(PPTK_DOUBLE,             GLSL_ES1_OTHER) \
  xx(PPTK_ELSE,               GLSL_ES1_ELSE) \
  xx(PPTK_ENUM,               GLSL_ES1_OTHER) \
  xx(PPTK_EXTERN,             GLSL_ES1_OTHER) \
  xx(PPTK_FLOAT,              GLSL_ES1_FLOAT) \
  xx(PPTK_FOR,                GLSL_ES1_FOR) \
  xx(PPTK_GOTO,               GLSL_ES1_OTHER) \
  xx(PPTK_IF,                 GLSL_ES1_IF) \
  xx(PPTK_INLINE,             GLSL_ES1_OTHER) \
  xx(PPTK_INT,                GLSL_ES1_INT) \
  xx(PPTK_LONG,               GLSL_ES1_OTHER) \
  xx(PPTK_REGISTER,           GLSL_ES1_IDENTIFIER) \
  xx(PPTK_RESTRICT,           GLSL_ES1_IDENTIFIER) \
  xx(PPTK_RETURN,             GLSL_ES1_RETURN) \
  xx(PPTK_SHORT,              GLSL_ES1_OTHER) \
  xx(PPTK_SIGNED,             GLSL_ES1_IDENTIFIER) \
  xx(PPTK_SIZEOF,             GLSL_ES1_OTHER) \
  xx(PPTK_STATIC,             GLSL_ES1_OTHER) \
  xx(PPTK_STRUCT,             GLSL_ES1_STRUCT) \
  xx(PPTK_SWITCH,             GLSL_ES1_OTHER) \
  xx(PPTK_TYPEDEF,            GLSL_ES1_OTHER) \
  xx(PPTK_UNION,              GLSL_ES1_OTHER) \
  xx(PPTK_UNSIGNED,           GLSL_ES1_OTHER) \
  xx(PPTK_VOID,               GLSL_ES1_VOID) \
  xx(PPTK_VOLATILE,           GLSL_ES1_OTHER) \
  xx(PPTK_WHILE,              GLSL_ES1_WHILE) \
  xx(PPTK_BOOL,               GLSL_ES1_IDENTIFIER) /* "_Bool", see PPTK_BOOL_ID for "bool" below. */ \
  xx(PPTK_COMPLEX,            GLSL_ES1_IDENTIFIER) \
  xx(PPTK_IMAGINARY,          GLSL_ES1_IDENTIFIER) \
  xx(PPTK_INTEGER_LIT,        GLSL_ES1_INTCONSTANT) \
  xx(PPTK_FLOAT_LIT,          GLSL_ES1_FLOATCONSTANT) \
  xx(PPTK_STRING_LIT,         GLSL_ES1_OTHER /* reconsider as: GLSL_ES1_STRING_LIT for debug logging purposes */ ) \
  xx(PPTK_CHAR_LIT,           GLSL_ES1_OTHER) \
  xx(PPTK_IDENT,              GLSL_ES1_IDENTIFIER) \
  xx(PPTK_WHITESPACE,         GLSL_ES1_WHITESPACE) \
  xx(PPTK_NEWLINE_WHITESPACE, GLSL_ES1_WHITESPACE) \
\
  xx(PPTK_HEADER_NAME,        GLSL_ES1_OTHER) \
\
  xx(PPTK_IFDEF,              GLSL_ES1_IDENTIFIER) \
  xx(PPTK_IFNDEF,             GLSL_ES1_IDENTIFIER) \
  xx(PPTK_ELIF,               GLSL_ES1_IDENTIFIER) \
  xx(PPTK_ENDIF,              GLSL_ES1_IDENTIFIER) \
  xx(PPTK_INCLUDE,            GLSL_ES1_IDENTIFIER) \
  xx(PPTK_EMIT,               GLSL_ES1_IDENTIFIER) \
  xx(PPTK_DEFINE,             GLSL_ES1_IDENTIFIER) \
  xx(PPTK_UNDEF,              GLSL_ES1_IDENTIFIER) \
  xx(PPTK_LINE,               GLSL_ES1_IDENTIFIER) \
  xx(PPTK_ERROR_KEYWORD,      GLSL_ES1_IDENTIFIER) \
  xx(PPTK_PRAGMA,             GLSL_ES1_IDENTIFIER) \
  xx(PPTK_DEFINED,            GLSL_ES1_IDENTIFIER) \
  xx(PPTK_PRAGMA_OP,          GLSL_ES1_IDENTIFIER) \
\
  xx(PPTK_BOOL_ID,            GLSL_ES1_BOOL) \
  xx(PPTK_BOOL_CONSTANT,      GLSL_ES1_BOOLCONSTANT) \
  xx(PPTK_CARET_CARET,        GLSL_ES1_XOR_OP) \
  xx(PPTK_ATTRIBUTE,          GLSL_ES1_ATTRIBUTE) \
  xx(PPTK_DISCARD,            GLSL_ES1_DISCARD) \
  xx(PPTK_BVEC2,              GLSL_ES1_BVEC2) \
  xx(PPTK_BVEC3,              GLSL_ES1_BVEC3) \
  xx(PPTK_BVEC4,              GLSL_ES1_BVEC4) \
  xx(PPTK_IVEC2,              GLSL_ES1_IVEC2) \
  xx(PPTK_IVEC3,              GLSL_ES1_IVEC3) \
  xx(PPTK_IVEC4,              GLSL_ES1_IVEC4) \
  xx(PPTK_VEC2,               GLSL_ES1_VEC2) \
  xx(PPTK_VEC3,               GLSL_ES1_VEC3) \
  xx(PPTK_VEC4,               GLSL_ES1_VEC4) \
  xx(PPTK_MAT2,               GLSL_ES1_MAT2) \
  xx(PPTK_MAT3,               GLSL_ES1_MAT3) \
  xx(PPTK_MAT4,               GLSL_ES1_MAT4) \
  xx(PPTK_IN,                 GLSL_ES1_IN) \
  xx(PPTK_OUT,                GLSL_ES1_OUT) \
  xx(PPTK_INOUT,              GLSL_ES1_INOUT) \
  xx(PPTK_UNIFORM,            GLSL_ES1_UNIFORM) \
  xx(PPTK_VARYING,            GLSL_ES1_VARYING) \
  xx(PPTK_SAMPLER2D,          GLSL_ES1_SAMPLER2D) \
  xx(PPTK_SAMPLERCUBE,        GLSL_ES1_SAMPLERCUBE) \
  xx(PPTK_INVARIANT,          GLSL_ES1_INVARIANT) \
  xx(PPTK_HIGH_PRECISION,     GLSL_ES1_HIGH_PRECISION) \
  xx(PPTK_MEDIUM_PRECISION,   GLSL_ES1_MEDIUM_PRECISION) \
  xx(PPTK_LOW_PRECISION,      GLSL_ES1_LOW_PRECISION) \
  xx(PPTK_PRECISION,          GLSL_ES1_PRECISION) \
\
  /* tokens reserved by GLSL 1 for future use, these should give an error */ \
  xx(PPTK_ASM,                GLSL_ES1_OTHER) \
  xx(PPTK_CLASS,              GLSL_ES1_OTHER) \
  xx(PPTK_TEMPLATE,           GLSL_ES1_OTHER) \
  xx(PPTK_THIS,               GLSL_ES1_OTHER) \
  xx(PPTK_PACKED,             GLSL_ES1_OTHER) \
  xx(PPTK_NOINLINE,           GLSL_ES1_OTHER) \
  xx(PPTK_PUBLIC,             GLSL_ES1_OTHER) \
  xx(PPTK_EXTERNAL,           GLSL_ES1_OTHER) \
  xx(PPTK_INTERFACE,          GLSL_ES1_OTHER) \
  xx(PPTK_FLAT,               GLSL_ES1_OTHER) \
  xx(PPTK_HALF,               GLSL_ES1_OTHER) \
  xx(PPTK_FIXED,              GLSL_ES1_OTHER) \
  xx(PPTK_SUPERP,             GLSL_ES1_OTHER) \
  xx(PPTK_INPUT,              GLSL_ES1_OTHER) \
  xx(PPTK_OUTPUT,             GLSL_ES1_OTHER) \
  xx(PPTK_HVEC2,              GLSL_ES1_OTHER) \
  xx(PPTK_HVEC3,              GLSL_ES1_OTHER) \
  xx(PPTK_HVEC4,              GLSL_ES1_OTHER) \
  xx(PPTK_DVEC2,              GLSL_ES1_OTHER) \
  xx(PPTK_DVEC3,              GLSL_ES1_OTHER) \
  xx(PPTK_DVEC4,              GLSL_ES1_OTHER) \
  xx(PPTK_FVEC2,              GLSL_ES1_OTHER) \
  xx(PPTK_FVEC3,              GLSL_ES1_OTHER) \
  xx(PPTK_FVEC4,              GLSL_ES1_OTHER) \
  xx(PPTK_SAMPLER1D,          GLSL_ES1_OTHER) \
  xx(PPTK_SAMPLER3D,          GLSL_ES1_OTHER) \
  xx(PPTK_SAMPLER1DSHADOW,    GLSL_ES1_OTHER) \
  xx(PPTK_SAMPLER2DSHADOW,    GLSL_ES1_OTHER) \
  xx(PPTK_SAMPLER2DRECT,      GLSL_ES1_OTHER) \
  xx(PPTK_SAMPLER3DRECT,      GLSL_ES1_OTHER) \
  xx(PPTK_SAMPLER2DRECTSHADOW,GLSL_ES1_OTHER) \
  xx(PPTK_CAST,               GLSL_ES1_OTHER) \
  xx(PPTK_NAMESPACE,          GLSL_ES1_OTHER) \
  xx(PPTK_USING,              GLSL_ES1_OTHER) \
\
  /* placemarker used during macro expansion of empty arguments adjacent to ## tokens (See C99 6.10.3.3-2) */ \
  xx(PPTK_PLACEMARKER,        0) \
\
  /* typedef-name, just prior to entering the C parser a pptk GLSL_ES1_IDENT may be promoted to a GLSL_ES1_TYPEDEF_NAME
   * if the identifier looks up to a typedef. This resolves a famous C ambiguity.
   * Note: for GLSL, they've reintroduced the same problem, but the token is now called TYPE_NAME, we adhere to that
   * name; but conceptually the same issue as in C. */ \
  xx(PPTK_TYPEDEF_NAME,       GLSL_ES1_TYPE_NAME) \
\
  /* literal text used when in template mode (this is text outside of {{ code_sections(); }} )*/ \
  xx(PPTK_TEMPLATE_LIT,       GLSL_ES1_OTHER) \
  xx(PPTK_TEMPLATE_DOUBLE_CURLY_OPEN, GLSL_ES1_OTHER) \
  xx(PPTK_TEMPLATE_DOUBLE_CURLY_CLOSE, GLSL_ES1_OTHER) \

extern int g_pptk_to_glsl_es1_[];

int glsl_es1_tokens_sanity_check(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GLSL_ES1_TOKENS_H */