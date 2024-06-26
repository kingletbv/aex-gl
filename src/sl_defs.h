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
#ifndef SL_DEFS_H
#define SL_DEFS_H

#define SL_ERR_OK           0
#define SL_ERR_INVALID_ARG -1
#define SL_ERR_OVERFLOW    -2
#define SL_ERR_NO_MEM      -3
#define SL_ERR_INTERNAL    -4
#define SL_ERR_HAD_ERRORS  -5 /* compilation errors */

const char *sl_err_str(int sl_err_code);

#endif /* SL_DEFS_H */
