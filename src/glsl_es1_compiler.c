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


void glsl_es1_compiler_init(struct glsl_es1_compiler *cc) {
  dx_diags_init(&cc->default_dx_);
  cc->dx_ = &cc->default_dx_;
  pp_init(&cc->pp_);
  cc->pp_.dx_ = cc->dx_; /* override pp diags to share cc one */
  glsl_es1_stack_init(&cc->parser_);
  sl_type_base_init(&cc->tb_);
  cc->all_done_ = 0;
  cc->is_typename_permitted_ = 1;
  cc->last_type_specifier_ = NULL;
  cc->current_function_prototype_ = NULL;
  st_init(&cc->global_scope_, NULL);
  cc->current_scope_ = &cc->global_scope_;
  sl_frame_init(&cc->global_frame_);
  cc->current_frame_ = &cc->global_frame_;
}

void glsl_es1_compiler_cleanup(struct glsl_es1_compiler *cc) {
  st_cleanup(&cc->global_scope_);
  sl_frame_cleanup(&cc->global_frame_);
  sl_type_base_cleanup(&cc->tb_);
  pp_cleanup(&cc->pp_);
  glsl_es1_stack_cleanup(&cc->parser_);
  dx_diags_cleanup(&cc->default_dx_);
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
  if (cc->dx_->fatal_error_) {
    return GLSL_ES1_R_FAILED;
  }

c_parser_input:
  if (cc->pp_.pp_output_ || cc->pp_.pp_output_final_) {
    cc->dx_->input_file_ = cc->pp_.ppme_input_file_;
    cc->dx_->input_line_ = cc->pp_.ppme_input_line_;

    do {
      int sym = 0;

      do {
        if (cc->pp_.pp_output_) {
          struct pptk *tk = cc->pp_.pp_output_;
          sym = g_pptk_to_glsl_es1_[tk->tok_];
          if (sym == GLSL_ES1_IDENTIFIER) {
            /* Possibly retarget to TYPEDEF_NAME at runtime, depending on:
              * 1) if a TYPE_NAME is currently accepted by the grammar
              *    AND
              * 2) TYPE_NAME is permitted based on the context. If we successfully retarget as TYPEDEF_NAME, we must
              *    immediately *disable* this permission. It will be re-enabled by the reduction of declaration-specifiers.
              *    (A second occurrance of a typedef name is actually the declarator's identifier even if it clashes with
              *    an existing typedef name.)
              * See also the reduction of declaration-specifiers in particular. */
            if (cc->is_typename_permitted_ && glsl_es1_stack_accepts(&cc->parser_, GLSL_ES1_TYPE_NAME)) {
              // Cascade through the scopes to look for a typedef name, if any.
              struct sym_table *st = cc->current_scope_;
              struct sl_type *named_type = NULL;
              while (st) {
                struct sym *s = st_find(st, tk->text_, tk->text_len_);
                if (s) {
                  if (s->kind_ == SK_STRUCT) {
                    named_type = s->v_.type_;
                  }
                  /* Even if we found something other than a struct, we still stop the
                   * search and let the parser deal with it as an identifier. */
                  break;
                }
                st = st->parent_;
              }
              if (named_type) {
                sym = GLSL_ES1_TYPE_NAME;
                cc->is_typename_permitted_ = 0;
                tk->v_type_ = PPVT_TYPE_NAME;
                tk->v_.type_ = named_type;
                tk->tok_ = PPTK_TYPEDEF_NAME;
              }
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
            dx_error(cc->dx_, "syntax error at column %d: \"%s\"\n", situs_col(&cc->pp_.pp_output_->situs_), cc->pp_.pp_output_->text_);
          }
          else {
            dx_error(cc->dx_, "syntax error, unexpected end of file\n", cc->dx_->input_file_, cc->dx_->input_line_);
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
          cc->dx_->fatal_error_ = 1;
          cc->dx_->have_error_ = 1;
          dx_printf(cc->dx_, "%s(%d): internal error (%d) from cp_parse at column %d: \"%s\"\n", cc->dx_->input_file_, cc->dx_->input_line_, r, situs_col(&cc->pp_.pp_output_->situs_), cc->pp_.pp_output_->text_);
        }
        else {
          cc->dx_->fatal_error_ = 1;
          cc->dx_->have_error_ = 1;
          dx_printf(cc->dx_, "%s(%d): internal error (%d) from cp_parse at end of file\n", cc->dx_->input_file_, cc->dx_->input_line_, r);
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
    dx_no_memory(cc->dx_);
    return GLSL_ES1_R_FAILED;
  }
  return glsl_es1_compile(cc);
}
