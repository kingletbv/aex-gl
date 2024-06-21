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

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef LIMITS_H_INCLUDED
#define LIMITS_H_INCLUDED
#include <limits.h>
#endif

#ifndef ERRNO_H_INCLUDED
#define ERRNO_H_INCLUDED
#include <errno.h>
#endif

#ifndef STDARG_H_INCLUDED
#define STDARG_H_INCLUDED
#include <stdarg.h>
#endif

#ifndef SOURCE_GEN_H_INCLUDED
#define SOURCE_GEN_H_INCLUDED
#include "source_gen.h"
#endif

static void sg_check_line_completion(struct source_gen *sg, int is_final) {
  for (;;) {
    int start_of_line = 0;
    if (!sg->line_buffer_size_) return;
    while ((start_of_line < sg->line_buffer_size_) && (sg->line_buffer_[start_of_line] == ' ')) {
      /* Space at start consumed for auto-indentation */
      start_of_line++;
    }
    int pos = start_of_line;

    while ((pos < sg->line_buffer_size_) && (sg->line_buffer_[pos] != '\n')) {
      pos++;
    }

    if ((pos == sg->line_buffer_size_) && !is_final) {
      /* Do not have a completed line to emit.. */
      return;
    }
  
    /* Otherwise line ends with newline at pos, or is the last line. */
    int line_end_pos = pos;

  
    int curly_brace_delta = 0;
    int curly_brace_underrun = 0;

    int inside_multiline_comment = sg->inside_comment_;
    int inside_doubleslash_comment = 0;

    int last_char_was_slash = 0;
    int last_char_was_asterisk = 0;

    int inside_string = 0;
    int inside_char = 0;

    int last_char_was_backslash = 0;

    for (pos = start_of_line; pos < line_end_pos; ++pos) {
      int inside_comment = inside_multiline_comment || inside_doubleslash_comment;
      if (inside_string) {
        switch (sg->line_buffer_[pos]) {
          case '\\': 
            if (last_char_was_backslash) last_char_was_backslash = 0;
            else last_char_was_backslash = 1;
            break;
          case '"':
            if (last_char_was_backslash) /* do \", nothing */;
            else {
              /* string ends here */
              inside_string = 0;
            }
            break;
        }
        if (sg->line_buffer_[pos] != '\\') last_char_was_backslash = 0;
      }
      if (inside_char) {
        switch (sg->line_buffer_[pos]) {
          case '\\':
            if (last_char_was_backslash) last_char_was_backslash = 0;
            else last_char_was_backslash = 1;
            break;
          case '\'':
            if (last_char_was_backslash) /* do \", nothing */;
            else {
              /* char ends here */
              inside_char = 0;
            }
            break;
        }
        if (sg->line_buffer_[pos] != '\\') last_char_was_backslash = 0;
      }
  
      switch (sg->line_buffer_[pos]) {
        case '/':
          if (!inside_multiline_comment && !inside_doubleslash_comment) {
            if (last_char_was_slash) {
              /* Inside double-slash comment to the end of the line */
              inside_doubleslash_comment = 1;
            }
            else {
              last_char_was_slash = 1;
            }
          }
          else if (inside_multiline_comment && last_char_was_asterisk) {
            /* Close multi-line comment */
            inside_multiline_comment = 0;
            sg->inside_comment_ = 0;
          }
          break;
        case '{':
          if (!inside_comment) curly_brace_delta++;
          break;
        case '}':
          if (!inside_comment) curly_brace_delta--;
          break;
        case '*':
          if (!inside_comment) {
            if (last_char_was_slash) {
              /* Open multi-line comment */
              inside_multiline_comment = 1;
              sg->inside_comment_ = 1;
              /* NOTE: important, last character is *not* deemed a '*' as that would immediately close the comment with the sequence / * / which would be incorrect. */
            }
            else {
              last_char_was_asterisk = 1;
            }
          }
          else {
            last_char_was_asterisk = 1;
          }
          break;
        case '"':
          inside_string = 1;
          break;
        case '\'':
          inside_char = 1;
          break;
        case '(':
          /* XXX: Want to record position of opening parenthesis and retain how many we have open.. */
        case ')':
          break;
      }
      if (sg->line_buffer_[pos] != '/') last_char_was_slash = 0;
      if (sg->line_buffer_[pos] != '*') last_char_was_asterisk = 0;

      if (curly_brace_delta < curly_brace_underrun) curly_brace_underrun = curly_brace_delta;
    }

    int current_line_indent = sg->indent_ + 2 * curly_brace_underrun;
    if (current_line_indent < 0) current_line_indent = 0;

    fprintf(sg->fp_, "%*s", current_line_indent, "");
    fwrite(sg->line_buffer_ + start_of_line, 1, line_end_pos - start_of_line, sg->fp_);
    if (!is_final) fprintf(sg->fp_, "\n");
    if (line_end_pos < sg->line_buffer_size_) {
      memcpy(sg->line_buffer_, sg->line_buffer_ + line_end_pos + 1 /* newline */, sg->line_buffer_size_ - line_end_pos - 1);
      sg->line_buffer_size_ -= line_end_pos + 1;
    }
    else {
      memcpy(sg->line_buffer_, sg->line_buffer_ + line_end_pos + 0 /* no newline */, sg->line_buffer_size_ - line_end_pos);
      sg->line_buffer_size_ -= line_end_pos;
    }

    int next_line_indent = sg->indent_ + 2 * curly_brace_delta;
    if (next_line_indent < 0) next_line_indent = 0;
    sg->indent_ = next_line_indent;
    sg->inside_comment_ = inside_multiline_comment;
  }
}

void sg_init(struct source_gen *sg) {
  sg->indent_ = 0;
  sg->inside_comment_ = 0;
  sg->line_buffer_size_ = sg->line_buffer_size_allocated_ = 0;
  sg->line_buffer_ = NULL;
  sg->error_ = 0;
  sg->fp_ = NULL;
}

void sg_cleanup(struct source_gen *sg) {
  if (sg->line_buffer_) free(sg->line_buffer_);
}


void sg_printf(struct source_gen *sg, const char *fmt, ...) {

#if 0
  if (1) {
    // xxx.
  } else {
    // yyy.
  }

  /* note that this "if {}" is different from "} else {" even though their {} delta count is the same, the "} else {"
   * has an under-run that causes the line it is on to drop indentation */
  if (1) {}
  else
  {
    // xxx.
  }

#endif
  /* First accumulate into the line buffer .. */
 
  va_list args;
  va_start(args, fmt);

  va_list cargs;

  va_copy(cargs, args);
  size_t len;
#ifdef _MSC_VER
  len = _vscprintf(fmt, cargs);
  if (len == -1) {
    if (!sg->error_) sg->error_ = EINVAL;
    return;
  }
#else
  len = vsnprintf(NULL, 0, fmt, cargs) + 1;
#endif

  va_end(cargs);
  size_t new_line_buffer_len = sg->line_buffer_size_ + len;
  if (new_line_buffer_len >= sg->line_buffer_size_allocated_) {
    size_t new_buf_size = (sg->line_buffer_size_allocated_ + sg->line_buffer_size_allocated_) | 0xFF;
    if (new_buf_size <= new_line_buffer_len) {
      new_buf_size = new_line_buffer_len + 1;
    }
    void *newbuf = realloc(sg->line_buffer_, new_buf_size);
    if (!newbuf) {
      if (!sg->error_) sg->error_ = ENOMEM;
      return;
    }
    sg->line_buffer_ = newbuf;
    sg->line_buffer_size_allocated_ = new_line_buffer_len + 1;
  }

  vsnprintf(sg->line_buffer_ + sg->line_buffer_size_, len + 1, fmt, args);
  sg->line_buffer_size_ += len;

  va_end(args);

  /* .. and then see if we have a completed line, ready for indentation. */
  sg_check_line_completion(sg, 0);
}

void sg_finish(struct source_gen *sg) {
  sg_check_line_completion(sg, 1);
}

