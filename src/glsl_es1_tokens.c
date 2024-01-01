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

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef GLSL_ES1_TOKENS_H_INCLUDED
#define GLSL_ES1_TOKENS_H_INCLUDED
#include "glsl_es1_tokens.h"
#endif

int g_pptk_to_glsl_es1_[] = {
#define xx(pptk_id, glsl_id) [pptk_id] = glsl_id,
  ENUM_GLSL_ES1_TOKENS
#undef xx
};

int glsl_es1_tokens_sanity_check(void) {
  size_t n;
  int success = 1;
  int glsl_token = GLSL_ES1_ATTRIBUTE; /* first token */
  for (glsl_token = GLSL_ES1_ATTRIBUTE; glsl_token < GLSL_ES1_ERROR /* 1+ last token */; glsl_token++) {
    for (n = 0; n < sizeof(g_pptk_to_glsl_es1_) / sizeof(g_pptk_to_glsl_es1_[0]); ++n) {
      if (g_pptk_to_glsl_es1_[n] == glsl_token) {
        break;
      }
    }
    if (n == (sizeof(g_pptk_to_glsl_es1_) / sizeof(g_pptk_to_glsl_es1_[0]))) {
      printf("glsl_es1 token %d is not mapped by any pptk token\n", glsl_token);
      success = 0;
    }
  }
  return success;
}
