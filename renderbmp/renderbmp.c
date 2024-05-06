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

#include <stdio.h>
#include <stdlib.h>

#include "sl_defs.h"
#include "gl_es2_impl.h"
#include "demos.h"

#define OUTPUT_WIDTH 1920
#define OUTPUT_HEIGHT 1080

int main(int argc, char **argv) {
  int exit_ret = EXIT_FAILURE;

  int r = 0;

  r = aex_gl_es2_context_init(32, 16, 32, OUTPUT_WIDTH, OUTPUT_HEIGHT);
  if (r) {
    fprintf(stderr, "Failed aex_gl_es2_context_init(): %s\n", sl_err_str(r));
    return EXIT_FAILURE;
  }

  if (run_demos(OUTPUT_HEIGHT)) {
    exit_ret = EXIT_FAILURE;
  }
  else {
    exit_ret = EXIT_SUCCESS;
  }

  return exit_ret;
}
