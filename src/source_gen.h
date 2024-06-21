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
#ifndef SOURCE_GEN_H
#define SOURCE_GEN_H

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct source_gen {
  /* Generated source file */
  
  /* The file handle to output to */
  FILE *fp_;

  /* Current indentation, in spaces */
  int indent_;

  /* Non-zero if we are inside a multi-line comment
   * { and } have no indentation effect while inside comments */
  int inside_comment_:1;

  /* The line buffer, accumulates output from sg_printf(), until a full line
   * is emitted, at which point it emits the line with proper indentation. */
  size_t line_buffer_size_;
  size_t line_buffer_size_allocated_;
  char *line_buffer_;

  /* If non-zero, an error occurred */
  int error_;
};

void sg_init(struct source_gen *sg);
void sg_cleanup(struct source_gen *sg);

void sg_printf(struct source_gen *sg, const char *fmt, ...);
void sg_finish(struct source_gen *sg);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SOURCE_GEN_H */
