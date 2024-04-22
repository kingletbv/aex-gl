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

#ifndef SL_DEFS_H_INCLUDED
#define SL_DEFS_H_INCLUDED
#include "sl_defs.h"
#endif

const char *sl_err_str(int sl_err_code) {
  const char *err;
  switch (sl_err_code) {
    case SL_ERR_OK: err = "SL_ERR_OK"; break;
    case SL_ERR_INVALID_ARG: err = "SL_ERR_INVALID_ARG"; break;
    case SL_ERR_OVERFLOW: err = "SL_ERR_OVERFLOW"; break;
    case SL_ERR_NO_MEM: err = "SL_ERR_NO_MEM"; break;
    case SL_ERR_INTERNAL: err = "SL_ERR_INTERNAL"; break;
    case SL_ERR_HAD_ERRORS: err = "SL_ERR_HAD_ERRORS"; break;
    default: err = "SL_ERR_???"; break;
  }
  return err;
}