/* Copyright 2023 Kinglet B.V.
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

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef STDARG_H_INCLUDED
#define STDARG_H_INCLUDED
#include <stdarg.h>
#endif

#ifndef GLSL_ES1_COMPILER_H_INCLUDED
#define GLSL_ES1_COMPILER_H_INCLUDED
#include "glsl_es1_compiler.h"
#endif

#ifndef GLSL_ES1_TOKENS_H_INCLUDED
#define GLSL_ES1_TOKENS_H_INCLUDED
#include "glsl_es1_tokens.h"
#endif

#ifndef PP_INPUT_FILE_H_INCLUDED
#define PP_INPUT_FILE_H_INCLUDED
#include "pp_input_file.h"
#endif

static int glsl_es1_compiler_stderr_vprintf_handler(void *baton, const char *file, int line_num, const char *fmt, va_list args);


void glsl_es1_compiler_init(struct glsl_es1_compiler *cc) {
  pp_init(&cc->pp_);
  glsl_es1_stack_init(&cc->parser_);
  cc->all_done_ = cc->fatal_error_ = 0;
  cc->is_typename_permitted_ = 1;
  cc->glsl_input_file_ = "";
  cc->glsl_input_line_ = 0;
  cc->vprintf_handler = glsl_es1_compiler_stderr_vprintf_handler;
}

void glsl_es1_compiler_cleanup(struct glsl_es1_compiler *compiler) {
  pp_cleanup(&compiler->pp_);
  glsl_es1_stack_cleanup(&compiler->parser_);
}

int glsl_es1_compiler_printf(struct glsl_es1_compiler *cc, const char *fmt, ...) {
  int r;
  va_list args;
  va_start(args, fmt);
  r = cc->vprintf_handler(cc->vprintf_baton_, NULL, 0, fmt, args);
  va_end(args);
  return r;
}

int glsl_es1_compiler_error_loc(struct glsl_es1_compiler *cc, struct situs *sit, const char *fmt, ...) {
  int r;
  va_list args;
  // Duplicate fmt to append newline.
  size_t fmt_len = strlen(fmt);
  char *dup_fmt = malloc(fmt_len + 2);
  if (!dup_fmt) {
    glsl_es1_compiler_no_memory(cc);
    return -1;
  }
  memcpy(dup_fmt, fmt, fmt_len);
  dup_fmt[fmt_len] = '\n';
  dup_fmt[fmt_len + 1] = '\0';
  cc->have_error_ = 1;
  va_start(args, fmt);
  r = cc->vprintf_handler(cc->vprintf_baton_, situs_filename(sit), situs_line(sit), dup_fmt, args);
  va_end(args);
  free(dup_fmt);
  return r;
}

int glsl_es1_compiler_error(struct glsl_es1_compiler *cc, const char *fmt, ...) {
  int r;
  va_list args;
  cc->have_error_ = 1;
  va_start(args, fmt);
  r = cc->vprintf_handler(cc->vprintf_baton_, cc->glsl_input_file_, cc->glsl_input_line_, fmt, args);
  va_end(args);
  return r;
}

int glsl_es1_compiler_fatal_loc(struct glsl_es1_compiler *cc, struct situs *sit, const char *fmt, ...) {
  int r;
  va_list args;
  cc->have_error_ = cc->fatal_error_ = 1;
  va_start(args, fmt);
  r = cc->vprintf_handler(cc->vprintf_baton_, situs_filename(sit), situs_line(sit), fmt, args);
  va_end(args);
  return r;
}

int glsl_es1_compiler_fatal(struct glsl_es1_compiler *cc, const char *fmt, ...) {
  int r;
  va_list args;
  cc->have_error_ = cc->fatal_error_ = 1;
  va_start(args, fmt);
  r = cc->vprintf_handler(cc->vprintf_baton_, cc->glsl_input_file_, cc->glsl_input_line_, fmt, args);
  va_end(args);
  return r;
}

int glsl_es1_compiler_warn_loc(struct glsl_es1_compiler *cc, struct situs *sit, const char *fmt, ...) {
  int r;
  va_list args;
  // Duplicate fmt to append newline.
  size_t fmt_len = strlen(fmt);
  char *dup_fmt = malloc(fmt_len + 2);
  if (!dup_fmt) {
    glsl_es1_compiler_no_memory(cc);
    return -1;
  }
  memcpy(dup_fmt, fmt, fmt_len);
  dup_fmt[fmt_len] = '\n';
  dup_fmt[fmt_len + 1] = '\0';
  va_start(args, fmt);
  r = cc->vprintf_handler(cc->vprintf_baton_, situs_filename(sit), situs_line(sit), dup_fmt, args);
  va_end(args);
  free(dup_fmt);
  return r;
}

int glsl_es1_compiler_warn(struct glsl_es1_compiler *cc, const char *fmt, ...) {
  int r;
  va_list args;
  va_start(args, fmt);
  r = cc->vprintf_handler(cc->vprintf_baton_, cc->glsl_input_file_, cc->glsl_input_line_, fmt, args);
  va_end(args);
  return r;
}

void glsl_es1_compiler_no_memory(struct glsl_es1_compiler *cc) {
  glsl_es1_compiler_fatal(cc, "No memory\n");
}


static void glsl_es1_compiler_skip_template_cruft(struct glsl_es1_compiler *cc) {
  while ((cc->pp_.pp_output_) &&
          ((cc->pp_.pp_output_->tok_ == PPTK_TEMPLATE_DOUBLE_CURLY_OPEN) ||
          (cc->pp_.pp_output_->tok_ == PPTK_TEMPLATE_DOUBLE_CURLY_CLOSE) ||
          ((cc->pp_.pp_output_->tok_ == PPTK_TEMPLATE_LIT) && !cc->pp_.pp_output_->text_len_))) {
    pptk_free(pptk_pop_front(&cc->pp_.pp_output_));
  }
}

static void glsl_es1_compiler_next_token(struct glsl_es1_compiler *cc) {
  pptk_free(pptk_pop_front(&cc->pp_.pp_output_));
  glsl_es1_compiler_skip_template_cruft(cc);
}

enum glsl_es1_compiler_result glsl_es1_compiler_stage(struct glsl_es1_compiler *cc) {
  int r;
final_completion:
  if (cc->all_done_) {
    return GLSL_ES1_R_SUCCESS;
  }
  if (cc->fatal_error_) {
    return GLSL_ES1_R_FAILED;
  }

c_parser_input:
  if (cc->pp_.pp_output_ || cc->pp_.pp_output_final_) {
    cc->glsl_input_line_ = cc->pp_.ppme_input_line_;
    cc->glsl_input_file_ = cc->pp_.ppme_input_file_;

    do {
      int sym = 0;

      do {
        if (cc->pp_.pp_output_) {
          struct pptk *tk = cc->pp_.pp_output_;
          sym = g_pptk_to_glsl_es1_[tk->tok_];
          if (sym == GLSL_ES1_IDENTIFIER) {
            /* Possibly retarget to TYPEDEF_NAME at runtime, depending on:
              * 1) if a TYPEDEF_NAME is currently accepted by the grammar (cp_stack_accepts(stack, CP_TYPEDEF_NAME))
              *    AND
              * 2) TYPEDEF_NAME is permitted based on the context. If we successfully retarget as TYPEDEF_NAME, we must
              *    immediately *disable* this permission. It will be re-enabled by the reduction of declaration-specifiers.
              *    (A second occurrance of a typedef name is actually the declarator's identifier even if it clashes with
              *    an existing typedef name.)
              * See also the reduction of declaration-specifiers in particular. */
            if (cc->is_typename_permitted_ && glsl_es1_stack_accepts(&cc->parser_, GLSL_ES1_TYPE_NAME)) {
              // XXX: Cascade through the scopes to look for a typedef name, if any.
#if 0
              struct name_space *pns = cc->ctx_.block_ ? cc->ctx_.block_->ns_ : &cc->global_ns_;
              while (pns) {
                struct sym *s = st_find(&pns->ordinary_idents_, tk->text_);
                if (s) {
                  struct decl *d = (struct decl *)s;
                  if (d->sc_ == SC_TYPEDEF) {
                    /* Found a typedef where one is accepted, rewrite */
                    cc->is_typename_permitted_ = 0;
                    sym = GLSL_ES1_TYPE_NAME;
                    tk->v_type_ = PPVT_TYPE_NAME;
                    tk->v_.type_ = d->type_;
                    tk->tok_ = PPTK_TYPEDEF_NAME;
                    became_typedefname = 1;
                    break;
                  }
                }

                pns = pns->parent_;
              }
#endif
            }
          }
        }
        else {
          if (cc->pp_.pp_output_final_) {
            sym = GLSL_ES1_INPUT_END;
          }
          else {
            /* Need to get more input */
            goto preprocessor;
          }
        }
        if (sym == GLSL_ES1_WHITESPACE) {
          /* Skip whitespace as input -- step to next tk */
          glsl_es1_compiler_next_token(cc);
        }
      } while (sym == GLSL_ES1_WHITESPACE);\
      r = glsl_es1_parse(&cc->parser_, sym, cc, cc->pp_.pp_output_);

      if ((r == _GLSL_ES1_SYNTAX_ERROR) || (r == _GLSL_ES1_FEED_ME)) {
        if (r == _GLSL_ES1_SYNTAX_ERROR) {
          if (cc->pp_.pp_output_) {
            glsl_es1_compiler_error(cc, "syntax error at column %d: \"%s\"\n", situs_col(&cc->pp_.pp_output_->situs_), cc->pp_.pp_output_->text_);
          }
          else {
            glsl_es1_compiler_error(cc, "syntax error, unexpected end of file\n", cc->glsl_input_file_, cc->glsl_input_line_);
          }
          /* keep going */
          goto c_parser_input;
        }

        /* Step to next tk */
        glsl_es1_compiler_next_token(cc);
      }

      if (r == _GLSL_ES1_FINISH) {
        cc->all_done_ = 1;
        goto final_completion;
      }
      else if ((r != _GLSL_ES1_SYNTAX_ERROR) && (r != _GLSL_ES1_FEED_ME)) {
        if (cc->pp_.pp_output_) {
          cc->fatal_error_ = 1;
          cc->have_error_ = 1;
          glsl_es1_compiler_printf(cc, "%s(%d): internal error (%d) from cp_parse at column %d: \"%s\"\n", cc->glsl_input_file_, cc->glsl_input_line_, r, situs_col(&cc->pp_.pp_output_->situs_), cc->pp_.pp_output_->text_);
        }
        else {
          cc->fatal_error_ = 1;
          cc->have_error_ = 1;
          glsl_es1_compiler_printf(cc, "%s(%d): internal error (%d) from cp_parse at end of file\n", cc->glsl_input_file_, cc->glsl_input_line_, r);
        }
        return GLSL_ES1_R_FAILED;
      }
    } while (r == _GLSL_ES1_FEED_ME);
  }

preprocessor:
  return GLSL_ES1_R_NEED_INPUT;
}

enum glsl_es1_compiler_result glsl_es1_compile(struct glsl_es1_compiler *cc) {
  enum glsl_es1_compiler_result cr;

  do {
    glsl_es1_compiler_skip_template_cruft(cc);

    cr = glsl_es1_compiler_stage(cc);
    if (cr != GLSL_ES1_R_NEED_INPUT) {
      return cr;
    }

    cr = pp_preprocessor_stage(&cc->pp_);
  } while (cr == GLSL_ES1_R_SUCCESS);

  return cr;
}

static int glsl_es1_compiler_stderr_vprintf_handler(void *baton, const char *file, int line_num, const char *fmt, va_list args) {
  if (file) {
    fprintf(stderr, "%s(%d): ", file, line_num);
  }

  int r;
  r = vfprintf(stderr, fmt, args);

  return r < 0;
}

struct glsl_es1_compiler_mem_input_file {
  struct pp_input_file if_;
  const void *mem_;
  size_t memsize_;
};

static int glsl_es1_compiler_file_mem_input_request_callback(void *baton, struct pp_input_file *ifile) {
  struct glsl_es1_compiler_mem_input_file *mif = (struct glsl_es1_compiler_mem_input_file *)ifile;
  if_set_input(ifile, mif->mem_, mif->memsize_, 1);
  return 0;
}

static struct pp_input_file *glsl_es1_compiler_push_input_file_mem(struct glsl_es1_compiler *cc, const char *filename, const void *mem, size_t memsize) {
  struct glsl_es1_compiler_mem_input_file *ifile;
  ifile = (struct glsl_es1_compiler_mem_input_file *)pp_push_input_file(&cc->pp_, filename, sizeof(struct glsl_es1_compiler_mem_input_file) - sizeof(struct pp_input_file));
  ifile->if_.input_request_fn_ = glsl_es1_compiler_file_mem_input_request_callback;
  ifile->mem_ = mem;
  ifile->memsize_ = memsize;
  return &ifile->if_;
}


enum glsl_es1_compiler_result glsl_es1_compiler_compile_mem(struct glsl_es1_compiler *cc, const char *glsl_input_filename, const char *glsl_input_text, size_t glsl_input_text_len) {
  struct pp_input_file *ifile = glsl_es1_compiler_push_input_file_mem(cc, glsl_input_filename, glsl_input_text, glsl_input_text_len);
  if (!ifile) {
    glsl_es1_compiler_no_memory(cc);
    return GLSL_ES1_R_FAILED;
  }
  return glsl_es1_compile(cc);
}
