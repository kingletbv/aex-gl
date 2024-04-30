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
#ifndef SL_INFO_LOG_H
#define SL_INFO_LOG_H

#ifndef DIAGS_H_INCLUDED
#define DIAGS_H_INCLUDED
#include "diags.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct sl_info_log {
  struct diags dx_;

  size_t gl_info_log_size_;
  size_t gl_info_log_size_allocated_;
  char *gl_info_log_;
};

void sl_info_log_init(struct sl_info_log *log);
void sl_info_log_cleanup(struct sl_info_log *log);
void sl_info_log_get_log(struct sl_info_log *log, size_t bufsize, size_t *length_used, char *buf);
void sl_info_log_clear(struct sl_info_log *log);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_INFO_LOG_H */
