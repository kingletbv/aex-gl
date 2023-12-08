/* Copyright 2020-2023 Kinglet B.V.
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

#ifndef STDARG_H_INCLUDED
#define STDARG_H_INCLUDED
#include <stdarg.h>
#endif

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef PP_H_INCLUDED
#define PP_H_INCLUDED
#include "pp.h"
#endif

#ifndef PP_MACRO_TABLE_H_INCLUDED
#define PP_MACRO_TABLE_H_INCLUDED
#include "pp_macro_table.h"
#endif

#ifndef PP_TOKENS_H_INCLUDED
#define PP_TOKENS_H_INCLUDED
#include "pp_tokens.h"
#endif

#ifndef PP_TOKENIZER_H_INCLUDED
#define PP_TOKENIZER_H_INCLUDED
#include "pp_tokenizer.h"
#endif

#ifndef PP_LINE_DIRECTIVES_H_INCLUDED
#define PP_LINE_DIRECTIVES_H_INCLUDED
#include "pp_line_directives.h"
#endif

#ifndef PP_MACRO_EXPANDER_H_INCLUDED
#define PP_MACRO_EXPANDER_H_INCLUDED
#include "pp_macro_expander.h"
#endif

#ifndef PP_CONST_EXPR_H_INCLUDED
#define PP_CONST_EXPR_H_INCLUDED
#include "pp_const_expr.h"
#endif

#ifndef PP_LINE_CONTINUATIONS_H_INCLUDED
#define PP_LINE_CONTINUATIONS_H_INCLUDED
#include "pp_line_continuations.h"
#endif

#ifndef PP_TRIGRAPHS_H_INCLUDED
#define PP_TRIGRAPHS_H_INCLUDED
#include "pp_trigraphs.h"
#endif

#ifndef PP_INPUT_FILE_H_INCLUDED
#define PP_INPUT_FILE_H_INCLUDED
#include "pp_input_file.h"
#endif

static int pp_stderr_vprintf_handler(void *baton, const char *file, int line_num, const char *fmt, va_list args);

static struct pp_input_file *pp_push_input_file(struct preprocessor *pp, const char *filename, size_t aux_data) {
  char *sf = pp_preserve_filename(pp, filename);
  if (!sf) return NULL;
  struct pp_input_file *ifile = (struct pp_input_file *)malloc(sizeof(struct pp_input_file) + aux_data);
  if (!ifile) {
    free(sf);
    return NULL;
  }
  if_init(ifile, pp);
  ifile->filename_ = sf;
  ifile->ppld_input_line_ = 0;
  ifile->ppld_input_line_filename_ = NULL;
  ifile->pptk_input_line_ = 1;
  ifile->pptk_input_line_filename_ = sf;
  return ifile;
}

static void pp_pop_input_file(struct preprocessor *pp) {
  struct pp_input_file *ifile = pp->input_stack_;
  if (ifile) {
    if_cleanup(ifile);
    free(ifile);
  }
}

void pp_init(struct preprocessor *pp) {
  pp->have_error_ = 0;
  pp->fatal_error_ = 0;

  mt_init(&pp->macro_table_);
  pp->if_section_stack_ = NULL;

  pp->ppme_input_final_ = 0;
  pp->ppme_input_ = NULL;
  pp->ppme_input_file_ = NULL;
  pp->ppme_input_line_ = 0;
  ppme_stack_init(&pp->ppme_);

  pp->ppld_input_final_ = 0;
  pp->have_ppld_input_line_ = 0;
  pp->ppld_input_ = NULL;

  pp->input_stack_ = NULL;

  pp->pp_output_ = NULL;
  pp->pp_output_final_ = 0;

  pp->filename_buffers_ = NULL;

  pp->bits_per_char_ = 8;
  pp->bits_per_short_ = 16;
  pp->bits_per_int_ = 32;
  pp->bits_per_long_ = 32;
  pp->bits_per_long_long_ = 64;

  pp->include_is_system_include_ = 0;
  pp->include_is_template_emit_ = 0;
  pp->include_file_arg_ = NULL;

  pp->vprintf_handler = pp_stderr_vprintf_handler;
  pp->vprintf_baton_ = NULL;
}

void pp_cleanup(struct preprocessor *pp) {
  mt_cleanup(&pp->macro_table_);

  while (pp->if_section_stack_) pp_if_pop(pp);

  ppme_stack_cleanup(&pp->ppme_);
  pptk_free(pp->ppme_input_);

  pptk_free(pp->ppld_input_);

  while (pp->input_stack_) pp_pop_input_file(pp);

  pptk_free(pp->pp_output_);

  while (pp->filename_buffers_) {
    struct pp_filename_buffer *fb = pp->filename_buffers_;
    pp->filename_buffers_ = fb->next_;
    free(fb);
  }

  if (pp->include_file_arg_) {
    free(pp->include_file_arg_);
  }
}

struct pp_if_section *pp_if_push(struct preprocessor *pp) {
  struct pp_if_section *ldifs = (struct pp_if_section *)malloc(sizeof(struct pp_if_section));
  if (!ldifs) return NULL;
  ldifs->parent_ = pp->if_section_stack_;
  pp->if_section_stack_ = ldifs;
  ldifs->state_ = PP_IFSS_SKIP;
  return ldifs;
}

void pp_if_pop(struct preprocessor *pp) {
  struct pp_if_section *ldifs = pp->if_section_stack_;
  pp->if_section_stack_ = ldifs->parent_;
  free(ldifs);
}

int pp_concat_sub_output(struct preprocessor *pp, struct situs *output_situs, char **output_buf, size_t *output_pos, size_t *output_buf_size, struct situs *span_situs, const char *span_text) {
  size_t span_text_len = strlen(span_text);
  return pp_concat_output_b(pp, output_situs, output_buf, output_pos, output_buf_size, span_situs, span_text, span_text_len);
}

int pp_concat_sub_output_b(struct preprocessor *pp, struct situs *output_situs, char **output_buf, size_t *output_pos, size_t *output_buf_size, struct situs *span_situs, const char *span_text, size_t span_text_len) {
  size_t buf_size_needed = (*output_pos) + span_text_len;
  if (buf_size_needed > *output_buf_size) {
    char *newbuf = (char *)realloc(*output_buf, buf_size_needed + 1);
    if (!newbuf) {
      pp_no_memory(pp);
      return -1;
    }
    *output_buf = newbuf;
    *output_buf_size = buf_size_needed;
  }
  memcpy((*output_buf) + *output_pos, span_text, span_text_len);
  (*output_buf)[(*output_pos) + span_text_len] = '\0';
  struct situs substitution;
  situs_init(&substitution);
  if (situs_clone(&substitution, span_situs)) {
    pp_no_memory(pp);
    return -1;
  }

  struct situs_span *sss = (span_situs->num_spans_ > 1) ? span_situs->u_.many_.spans_ : &span_situs->u_.one_;
  /* Mark all of the situs a substitution */
  size_t n;
  sss->is_substitution_ = 1;
  for (n = 1; n < substitution.num_spans_; ++n) {
    sss[n].is_aux_ = 1;
    sss[n].num_bytes_ = 0;
  }
  sss[0].num_bytes_ = span_text_len;
  if (situs_concat(output_situs, &substitution)) {
    situs_cleanup(&substitution);
    pp_no_memory(pp);
    return -1;
  }
  situs_cleanup(&substitution);
  (*output_pos) += span_text_len;
  return 0;
}

int pp_concat_output(struct preprocessor *pp, struct situs *output_situs, char **output_buf, size_t *output_pos, size_t *output_buf_size, struct situs *span_situs, const char *span_text) {
  size_t span_text_len = situs_len(span_situs);
  return pp_concat_output_b(pp, output_situs, output_buf, output_pos, output_buf_size, span_situs, span_text, span_text_len);
}

int pp_concat_output_b(struct preprocessor *pp, struct situs *output_situs, char **output_buf, size_t *output_pos, size_t *output_buf_size, struct situs *span_situs, const char *span_text, size_t span_text_len) {
  size_t buf_size_needed = (*output_pos) + span_text_len;
  if (buf_size_needed > *output_buf_size) {
    char *newbuf = (char *)realloc(*output_buf, buf_size_needed + 1);
    if (!newbuf) {
      pp_no_memory(pp);
      return -1;
    }
    *output_buf = newbuf;
    *output_buf_size = buf_size_needed;
  }
  memcpy((*output_buf) + *output_pos, span_text, span_text_len);
  (*output_buf)[(*output_pos) + span_text_len] = '\0';
  if (situs_concat(output_situs, span_situs)) {
    pp_no_memory(pp);
    return -1;
  }
  (*output_pos) += span_text_len;
  return 0;
}

char *pp_preserve_filename(struct preprocessor *pp, const char *filename) {
  size_t len = strlen(filename);
  size_t size_needed = len + sizeof(struct pp_filename_buffer /* includes 1 byte sized array, hence len does not need to include null-terminator */);
  if (size_needed <= len) {
    return NULL; /* overflow */
  }
  struct pp_filename_buffer *fb = (struct pp_filename_buffer *)malloc(size_needed);
  if (!fb) return NULL;
  fb->next_ = pp->filename_buffers_;
  pp->filename_buffers_ = fb;
  memcpy(fb->filename_, filename, len);
  fb->filename_[len] = '\0';
  return fb->filename_;
}


static int pp_stderr_vprintf_handler(void *baton, const char *file, int line_num, const char *fmt, va_list args) {
  if (file) {
    fprintf(stderr, "%s(%d): ", file, line_num);
  }

  int r;
  r = vfprintf(stderr, fmt, args);

  return r < 0;
}

int pp_printf(struct preprocessor *pp, const char *fmt, ...) {
  int r;
  va_list args;
  va_start(args, fmt);
  r = pp->vprintf_handler(pp->vprintf_baton_, NULL, 0, fmt, args);
  va_end(args);
  return r;
}

int pp_error_loc(struct preprocessor *pp, struct situs *sit, const char *fmt, ...) {
  int r;
  va_list args;
  // Duplicate fmt to append newline.
  size_t fmt_len = strlen(fmt);
  char *dup_fmt = malloc(fmt_len + 2);
  if (!dup_fmt) {
    pp_no_memory(pp);
    return -1;
  }
  memcpy(dup_fmt, fmt, fmt_len);
  dup_fmt[fmt_len] = '\n';
  dup_fmt[fmt_len + 1] = '\0';
  pp->have_error_ = 1;
  va_start(args, fmt);
  r = pp->vprintf_handler(pp->vprintf_baton_, situs_filename(sit), situs_line(sit), dup_fmt, args);
  va_end(args);
  free(dup_fmt);
  return r;
}

int pp_error(struct preprocessor *pp, const char *fmt, ...) {
  int r;
  va_list args;
  pp->have_error_ = 1;
  va_start(args, fmt);
  r = pp->vprintf_handler(pp->vprintf_baton_, pp->ppme_input_file_, pp->ppme_input_line_, fmt, args);
  va_end(args);
  return r;
}

int pp_fatal_loc(struct preprocessor *pp, struct situs *sit, const char *fmt, ...) {
  int r;
  va_list args;
  pp->have_error_ = pp->fatal_error_ = 1;
  va_start(args, fmt);
  r = pp->vprintf_handler(pp->vprintf_baton_, situs_filename(sit), situs_line(sit), fmt, args);
  va_end(args);
  return r;
}

int pp_fatal(struct preprocessor *pp, const char *fmt, ...) {
  int r;
  va_list args;
  pp->have_error_ = pp->fatal_error_ = 1;
  va_start(args, fmt);
  r = pp->vprintf_handler(pp->vprintf_baton_, pp->ppme_input_file_, pp->ppme_input_line_, fmt, args);
  va_end(args);
  return r;
}

int pp_warn_loc(struct preprocessor *pp, struct situs *sit, const char *fmt, ...) {
  int r;
  va_list args;
  // Duplicate fmt to append newline.
  size_t fmt_len = strlen(fmt);
  char *dup_fmt = malloc(fmt_len + 2);
  if (!dup_fmt) {
    pp_no_memory(pp);
    return -1;
  }
  memcpy(dup_fmt, fmt, fmt_len);
  dup_fmt[fmt_len] = '\n';
  dup_fmt[fmt_len + 1] = '\0';
  va_start(args, fmt);
  r = pp->vprintf_handler(pp->vprintf_baton_, situs_filename(sit), situs_line(sit), dup_fmt, args);
  va_end(args);
  free(dup_fmt);
  return r;
}

int pp_warn(struct preprocessor *pp, const char *fmt, ...) {
  int r;
  va_list args;
  va_start(args, fmt);
  r = pp->vprintf_handler(pp->vprintf_baton_, pp->ppme_input_file_, pp->ppme_input_line_, fmt, args);
  va_end(args);
  return r;
}

void pp_no_memory(struct preprocessor *pp) {
  pp_fatal(pp, "No memory\n");
}

enum preprocessor_result pp_preprocessor_stage(struct preprocessor *pp) {
macro_expander:
  if (pp->ppme_input_ || pp->ppme_input_final_) {
    int r;
    int next_sym;
    if (pp->ppme_input_) {
      next_sym = g_pptk_to_ppme_[pp->ppme_input_->tok_];
    }
    else {
      next_sym = PPME_INPUT_END;
    }
    if (next_sym == PPME_IDENT) {
      struct macro *m = mt_find(&pp->macro_table_, pp->ppme_input_->text_, pp->ppme_input_->text_len_);

      if (m && !m->nested_invocation_) {
        if (m->is_function_style_) {
          next_sym = PPME_FUNCTION;
        }
        else {
          next_sym = PPME_OBJECT;
        }
      }
    }
    r = ppme_parse(&pp->ppme_, next_sym, pp, &pp->ppme_input_, pp->ppme_input_final_, &pp->pp_output_);
    switch (r) {
      case _PPME_FINISH:
        pp->pp_output_final_ = 1;
        /* Preprocessor is done */
        return PPR_SUCCESS;

      case _PPME_FEED_ME:
        /* Need more input; however, if we have a bit of output ready for the C parser, process that first so
         * we minimize the number of tokens concurrently in-flight. */
        if (pp->pp_output_) {
          return PPR_SUCCESS;
        }
        break; /* get more input .. */

      case _PPME_SYNTAX_ERROR:
        if (pp->ppme_input_) {
          pp_error_loc(pp, &pp->ppme_input_->situs_, "syntax error: \"%s\"\n", pp->ppme_input_->text_);
        }
        else {
          pp_printf(pp, "%s(%d): syntax error at end of file\n", pp->ppme_input_file_, pp->ppme_input_line_);
          pp->have_error_ = 1;
        }
        /* keep going */
        goto macro_expander;

      case _PPME_MATCH:
      case _PPME_OVERFLOW:
      case _PPME_NO_MEMORY:
      case _PPME_END_OF_INPUT:
      case _PPME_LEXICAL_ERROR:
      case _PPME_INTERNAL_ERROR:
      default:
        if (pp->ppme_input_) {
          pp_error_loc(pp, &pp->ppme_input_->situs_, "internal error (%d) from ppme_parse: \"%s\"\n", r, pp->ppme_input_->text_);
        }
        else {
          pp_printf(pp, "%s(%d): internal error (%d) from ppme_parse at end of file\n", pp->ppme_input_file_, pp->ppme_input_line_, r);
          pp->have_error_ = 1;
        }
        return PPR_FAILED;
    }
  }

line_directives:
  if (!pp->input_stack_) {
    pp->ppme_input_final_ = 1;
    goto macro_expander;
  }

  if (pp->input_stack_->ppld_end_processed_pop_now_) {
    /* Current input file has finished; so there is no sense in requesting more input.
     * Pop to parent if possible. */
    if (!pp->input_stack_->unpoppable_) {
      pp->ppld_input_final_ = 0;
      pp_pop_input_file(pp);
      return PPR_OLD_INCLUDE;
    }
    else {
      pp->ppme_input_final_ = 1;
      goto macro_expander;
    }
  }

  if (!pp->ppld_input_ && !pp->have_ppld_input_line_ && pp->ppld_input_final_) {
    pp->input_stack_->ppld_end_processed_pop_now_ = 1;
    goto macro_expander;
  }

  if (pp->ppld_input_ || pp->have_ppld_input_line_ || pp->input_stack_->ppld_input_not_finished_) {
    for (;;) {
      int ppld_r;
      /* Single lines are always final from the perspective of the ppld parser (hence we pass 1 for is_final),
       * whether the total file is final (ppld_input_final_) determines if it is the last line of the file,
       * this line might be empty. */
      if (!pp->input_stack_->ppld_input_not_finished_) {
        struct pptk *pp_current = pp->ppld_input_ ? pptk_pop_front(&pp->ppld_input_) : NULL;
        ppld_r = ppld_parse(&pp->input_stack_->ppld_, pp_current ? g_pptk_to_ppld_[pp_current->tok_] : /* have_ppld_input_line_ == true */ PPLD_INPUT_END, pp, &pp_current, &pp->ppld_input_, pp->have_ppld_input_line_, &pp->ppme_input_);
        if (pp_current) {
          /* Front (in current) was not processed, recombine */
          pp->ppld_input_ = pptk_join(pp_current, pp->ppld_input_);
        }
      }
      else {
        // Finish up last line before processing tokens for a new one.
        ppld_r = ppld_parse(&pp->input_stack_->ppld_, PPLD_INPUT_END, pp, NULL, NULL, 0, &pp->ppme_input_);
        pp->input_stack_->ppld_input_not_finished_ = 0;
      }
      switch (ppld_r) {
        case PPLD_INCLUDE_FILE:
          pp->input_stack_->ppld_input_not_finished_ = 1;
          pp->have_ppld_input_line_ = 0;
          return PPR_NEW_INCLUDE;
        case _PPLD_FINISH:
          pp->ppme_input_file_ = pp->input_stack_->ppld_input_line_filename_;
          pp->ppme_input_line_ = pp->input_stack_->ppld_input_line_;
          pp->have_ppld_input_line_ = 0;
          goto macro_expander;
        case _PPLD_SYNTAX_ERROR:
          if (pp->ppld_input_) {
            pp_error_loc(pp, &pp->ppld_input_->situs_, "syntax error: \"%s\"\n", pp->ppld_input_->text_);
          }
          else {
            pp_printf(pp, "%s(%d): syntax error at end of file\n", pp->input_stack_->ppld_input_line_filename_, pp->input_stack_->ppld_input_line_);
            pp->have_error_ = 1;
          }
          break;
        case _PPLD_MATCH:
        case _PPLD_END_OF_INPUT:
          /* Should not reach here - lexer internal codes */
          assert(0 && "Should never return lexer internal codes");
          return PPR_FAILED;
        case _PPLD_LEXICAL_ERROR:
          /* Not lexing, should not return lexer errors */
          assert(0 && "Should never return lexer codes");
          return PPR_FAILED;
        case _PPLD_FEED_ME:
          /* Needs more input */
          /* If we have input for the macro-expander, process it first before creating more tokens */
          if (pp->ppme_input_) {
            pp->ppme_input_file_ = pp->input_stack_->ppld_input_line_filename_;
            pp->ppme_input_line_ = pp->input_stack_->ppld_input_line_;
            goto macro_expander;
          }
          goto tokenizer;
        case _PPLD_OVERFLOW:
          pp_printf(pp, "Overflow while parsing line directives\n");
          pp->have_error_ = 1;
          return PPR_FAILED;
        case _PPLD_NO_MEMORY:
          pp_printf(pp, "No memory while parsing line directives\n");
          pp->have_error_ = 1;
          return PPR_FAILED;
        case _PPLD_INTERNAL_ERROR:
          pp_printf(pp, "Internal error while parsing line directives\n");
          pp->have_error_ = 1;
          return PPR_FAILED;
        default:
          pp_printf(pp, "Unexpected returncode while parsing line directives\n");
          return PPR_FAILED;
      }
    }
  }

tokenizer:;
  int pptk_r;
  do {
    int break_out = 0;
    if (ppld_stack_accepts(&pp->input_stack_->ppld_, PPLD_HEADER_NAME)) {
      switch (pptk_mode(&pp->input_stack_->pptk_)) {
        case M_PPTK_DEFAULT:
        case M_PPTK_HEADER_NAME_POSSIBLE:
          pptk_set_mode(&pp->input_stack_->pptk_, M_PPTK_HEADER_NAME_POSSIBLE);
          break;
        case M_PPTK_TEMPLATE_DEFAULT:
        case M_PPTK_TEMPLATE_HEADER_NAME_POSSIBLE:
        case M_PPTK_TEMPLATE_START:
          pptk_set_mode(&pp->input_stack_->pptk_, M_PPTK_TEMPLATE_HEADER_NAME_POSSIBLE);
          break;
      }

    }
    else {
      switch (pptk_mode(&pp->input_stack_->pptk_)) {
        case M_PPTK_DEFAULT:
        case M_PPTK_HEADER_NAME_POSSIBLE:
          pptk_set_mode(&pp->input_stack_->pptk_, M_PPTK_DEFAULT);
          break;
        case M_PPTK_TEMPLATE_DEFAULT:
        case M_PPTK_TEMPLATE_HEADER_NAME_POSSIBLE:
          pptk_set_mode(&pp->input_stack_->pptk_, M_PPTK_TEMPLATE_DEFAULT);
          break;
        case M_PPTK_TEMPLATE_START:
          /* Keep as is; the pptk tokenizer itself will change in due course */
          break;
      }
    }

    pptk_r = pptk_scan(&pp->input_stack_->pptk_, pp, &pp->ppld_input_, &pp->input_stack_->post_line_continuation_situs_);
    switch (pptk_r) {
      case PPTK_TOKENIZER_LINE_READY:
        pp->have_ppld_input_line_ = 1;
        pp->input_stack_->ppld_input_line_ = pp->input_stack_->pptk_input_line_;
        pp->input_stack_->ppld_input_line_filename_ = pp->input_stack_->pptk_input_line_filename_;
        int current_line = pptk_endline(&pp->input_stack_->pptk_);
        pp->input_stack_->pptk_input_line_ += current_line - pp->input_stack_->pptk_true_input_line_number_at_start_;
        pp->input_stack_->pptk_true_input_line_number_at_start_ = current_line;
        goto line_directives;
      case _PPTK_FINISH:
        break;
      case _PPTK_MATCH:
      case _PPTK_END_OF_INPUT:
        assert(0); /* lexer internal code */
        break;
      case _PPTK_SYNTAX_ERROR: {
        struct situs *situs = (struct situs *)pptk_token_common_data(&pp->input_stack_->pptk_);
        if (situs) {
          pp_error_loc(pp, situs, "syntax error: \"%s\"\n", pptk_text(&pp->input_stack_->pptk_));
        }
        else {
          pp_printf(pp, "%s(%d): syntax error at column %d: \"%s\"\n", pp->input_stack_->pptk_input_line_filename_, pptk_line(&pp->input_stack_->pptk_), pptk_column(&pp->input_stack_->pptk_), pptk_text(&pp->input_stack_->pptk_));
          pp->have_error_ = 1;
        }
        break;
      }
      case _PPTK_LEXICAL_ERROR:
        pp_printf(pp, "%s(%d): lexical error at column %d: \"%s\"\n", pp->input_stack_->pptk_input_line_filename_, pptk_line(&pp->input_stack_->pptk_), pptk_column(&pp->input_stack_->pptk_), pptk_text(&pp->input_stack_->pptk_));
        pp->have_error_ = 1;
        break;
      case _PPTK_INTERNAL_ERROR:
        pp_printf(pp, "%s(%d): internal error at column %d: \"%s\"\n", pp->input_stack_->pptk_input_line_filename_, pptk_line(&pp->input_stack_->pptk_), pptk_column(&pp->input_stack_->pptk_), pptk_text(&pp->input_stack_->pptk_));
        pp->have_error_ = 1;
        break;
      case PPTK_TOKENIZER_HEADERNAME_CHECK:
        /* Scanner thinks we should check if a headername is acceptable, this depends on whether the line directive parser can handle it. */
        /* Go to the line directives parser, parse to the extent input is available, and then come back here (via _PPLD_FEED_ME) to
         * check ppld_stack_accepts(PPLD_HEADER_NAME) and set the pptk mode appropriately. */
        goto line_directives;
      case _PPTK_FEED_ME:
        if (pp->ppld_input_) {
          /* Process input first; this is not a complete line but ensures we push as much as possible prior to asking
           * for more input. */
          goto line_directives;
        }
        break;
    }
  } while ((pptk_r != _PPTK_FEED_ME) && (pptk_r != _PPTK_FINISH));

  if ((pptk_r == _PPTK_FINISH) && !pp->input_stack_->pptk_pushed_final_source_line_) {
    /* Complete last line directive, the last line never ends with a newline. */
    pp->have_ppld_input_line_ = 1;
    pp->input_stack_->ppld_input_line_ = pp->input_stack_->pptk_input_line_;
    pp->input_stack_->ppld_input_line_filename_ = pp->input_stack_->pptk_input_line_filename_;
    pp->input_stack_->pptk_pushed_final_source_line_ = 1;
    goto line_directives;
  }

  if (pp->ppld_input_) {
    /* Process input first; this is not a complete line but ensures we push as much as possible prior to asking
     * for more input. */
    goto line_directives;
  }

  if (pptk_r == _PPTK_FINISH) {
    /* we only get here if trigraphs has depleted its input (or pptk
     * would not return _PPTK_FINISH but _PPTK_FEED_ME) */
    /* Cannot pop current input source, we've reached the end; bubble
     * any remaining input up through the stages until we ultimately
     * set all_done_ at the top... */
    pp->ppld_input_final_ = 1;
    goto line_directives;
  }
line_continuations:;
  int pplc_r;
  do {
    pplc_r = pplc_scan(&pp->input_stack_->pplc_, pp, &pp->input_stack_->post_line_continuation_, &pp->input_stack_->post_line_continuation_size_, &pp->input_stack_->post_line_continuation_size_allocated_, &pp->input_stack_->post_line_continuation_situs_, &pp->input_stack_->post_trigraph_situs_);
    switch (pplc_r) {
      case _PPLC_FINISH:
        break;
      case _PPLC_MATCH:
      case _PPLC_END_OF_INPUT:
        assert(0); /* lexer internal code */
        break;
      case _PPLC_SYNTAX_ERROR: {
        struct situs *situs = (struct situs *)pplc_token_common_data(&pp->input_stack_->pplc_);
        if (situs) {
          pp_error_loc(pp, situs, "syntax error: \"%s\"\n", pplc_text(&pp->input_stack_->pplc_));
        }
        else {
          pp_printf(pp, "%s(%d): syntax error at column %d: \"%s\"\n", pp->input_stack_->ppld_input_line_filename_, pplc_line(&pp->input_stack_->pplc_), pplc_column(&pp->input_stack_->pplc_), pplc_text(&pp->input_stack_->pplc_));
          pp->have_error_ = 1;
        }
        break;
      }
      case _PPLC_LEXICAL_ERROR:
        pp_printf(pp, "(%d): lexical error at column %d: \"%s\"\n", pplc_line(&pp->input_stack_->pplc_), pplc_column(&pp->input_stack_->pplc_), pplc_text(&pp->input_stack_->pplc_));
        pp->have_error_ = 1;
        break;
      case _PPLC_INTERNAL_ERROR:
        pp_printf(pp, "(%d): internal error at column %d: \"%s\"\n", pplc_line(&pp->input_stack_->pplc_), pplc_column(&pp->input_stack_->pplc_), pplc_text(&pp->input_stack_->pplc_));
        pp->have_error_ = 1;
        break;
      case _PPLC_FEED_ME:
        break;
    }
  } while ((pplc_r != _PPLC_FEED_ME) && (pplc_r != _PPLC_FINISH));

  if (pp->input_stack_->post_line_continuation_size_ || (pplc_r == _PPLC_FINISH)) {
    /* Process output of line continuation translation phase first, to ensure we push as much as possible before asking
     * more input. */
    pptk_set_input(&pp->input_stack_->pptk_, pp->input_stack_->post_line_continuation_, pp->input_stack_->post_line_continuation_size_, pplc_r == _PPLC_FINISH);
    pp->input_stack_->post_line_continuation_size_ = 0;
    goto tokenizer;
  }

trigraphs:;
  int pptg_r;
  do {
    pptg_r = pptg_scan(&pp->input_stack_->pptg_, pp, pp->input_stack_->filename_, &pp->input_stack_->post_trigraph_, &pp->input_stack_->post_trigraph_size_, &pp->input_stack_->post_trigraph_size_allocated_, &pp->input_stack_->post_trigraph_situs_);
    switch (pptg_r) {
      case _PPTG_FINISH:
        break;
      case _PPTG_MATCH:
      case _PPTG_END_OF_INPUT:
        assert(0); /* lexer internal code */
        break;
      case _PPTG_SYNTAX_ERROR: {
        struct situs *situs = (struct situs *)pptg_token_common_data(&pp->input_stack_->pptg_);
        if (situs) {
          pp_error_loc(pp, situs, "syntax error: \"%s\"\n", pptg_text(&pp->input_stack_->pptg_));
        }
        else {
          pp_printf(pp, "%s(%d): syntax error at column %d: \"%s\"\n", pp->input_stack_->ppld_input_line_filename_, pptg_line(&pp->input_stack_->pptg_), pptg_column(&pp->input_stack_->pptg_), pptg_text(&pp->input_stack_->pptg_));
          pp->have_error_ = 1;
        }
        break;
      }
      case _PPTG_LEXICAL_ERROR:
        pp_printf(pp, "(%d): lexical error at column %d: \"%s\"\n", pptg_line(&pp->input_stack_->pptg_), pptg_column(&pp->input_stack_->pptg_), pptg_text(&pp->input_stack_->pptg_));
        pp->have_error_ = 1;
        break;
      case _PPTG_INTERNAL_ERROR:
        pp_printf(pp, "(%d): internal error at column %d: \"%s\"\n", pptg_line(&pp->input_stack_->pptg_), pptg_column(&pp->input_stack_->pptg_), pptg_text(&pp->input_stack_->pptg_));
        pp->have_error_ = 1;
        break;
      case _PPTG_FEED_ME:
        break;
    }
  } while ((pptg_r != _PPTG_FEED_ME) && (pptg_r != _PPTG_FINISH));

  if (pp->input_stack_->post_trigraph_size_ || (pptg_r == _PPTG_FINISH)) {
    /* Process output of trigraph translation phase first, to ensure we push as much as possible before asking
     * more input. */
    pplc_set_input(&pp->input_stack_->pplc_, pp->input_stack_->post_trigraph_, pp->input_stack_->post_trigraph_size_, pptg_r == _PPTG_FINISH);
    pp->input_stack_->post_trigraph_size_ = 0;
    goto line_continuations;
  }

  /* Get more input from the current file. */
  if (!pp->input_stack_->input_request_fn_) {
    return PPR_NEED_INPUT;
  }

  /* Call for more input */
  int ir = pp->input_stack_->input_request_fn_(pp->input_stack_->baton_, pp->input_stack_);
  if (ir) {
    return ir;
  }
  /* And jump to trigraphs to process what we got. If we got nothing, and the caller or input_request_fn_
   * did not mark it as the end of the file, then we might loop infinitely here, but that'd be a bug in
   * the calling code (for not setting is_final() when calling if_set_input(), or not calling if_set_input()
   * at all.) */
  goto trigraphs;

}
