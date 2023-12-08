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

#ifndef GLSL_ES1_COMPILER_H_INCLUDED
#define GLSL_ES1_COMPILER_H_INCLUDED
#include "glsl_es1_compiler.h"
#endif

#ifndef GLSL_ES1_TOKENS_H_INCLUDED
#define GLSL_ES1_TOKENS_H_INCLUDED
#include "glsl_es1_tokens.h"
#endif

void glsl_es1_compiler_init(struct glsl_es1_compiler *cc) {
  pp_init(&cc->pp_);
  glsl_es1_stack_init(&cc->parser_);
  cc->all_done_ = cc->fatal_error_ = 0;
  cc->glsl_input_file_ = "";
  cc->glsl_input_line_ = 0;
}

void glsl_es1_compiler_cleanup(struct glsl_es1_compiler *compiler) {
  pp_cleanup(&compiler->pp_);
  glsl_es1_stack_cleanup(&compiler->parser_);
}

enum c_compiler_result glsl_es1_compiler_stage(struct glsl_es1_compiler *cc) {
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
            int became_typedefname = 0;
            if (cc->ctx_.is_typedefname_permitted_ && cp_stack_accepts(&cc->cp_, CP_TYPEDEF_NAME)) {
              struct name_space *pns = cc->ctx_.block_ ? cc->ctx_.block_->ns_ : &cc->global_ns_;
              while (pns) {
                struct sym *s = st_find(&pns->ordinary_idents_, tk->text_);
                if (s) {
                  struct decl *d = (struct decl *)s;
                  if (d->sc_ == SC_TYPEDEF) {
                    /* Found a typedef where one is accepted, rewrite */
                    cc->ctx_.is_typedefname_permitted_ = 0;
                    sym = CP_TYPEDEF_NAME;
                    tk->v_.type_ = d->type_;
                    tk->tok_ = PPTK_TYPEDEF_NAME;
                    became_typedefname = 1;
                    break;
                  }
                }

                pns = pns->parent_;
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
          cc_c_parser_next_token(cc);
        }
      } while (sym == GLSL_ES1_WHITESPACE);\
      r = glsl_es1_parse(&cc->parser_, sym, cc, cc->pp_.pp_output_);
      r = cp_parse(&cc->cp_, sym, cc, cc->pp_.pp_output_);
      if ((r == _CP_SYNTAX_ERROR) || (r == _CP_FEED_ME)) {
        if (r == _CP_SYNTAX_ERROR) {
          if (cc->cp_input_) {
            cc_error(cc, "syntax error at column %d: \"%s\"\n", situs_col(&cc->cp_input_->situs_), cc->cp_input_->text_);
          }
          else {
            cc_error(cc, "syntax error, unexpected end of file\n", cc->cp_input_file_, cc->cp_input_line_);
          }
          /* keep going */
          goto c_parser_input;
        }

        /* Step to next tk */
        cc_c_parser_next_token(cc);
      }

      if (r == _CP_FINISH) {
        cc->all_done_ = 1;
        goto final_completion;
      }
      else if ((r != _CP_SYNTAX_ERROR) && (r != _CP_FEED_ME)) {
        if (cc->cp_input_) {
          cc_printf(cc, "%s(%d): internal error (%d) from cp_parse at column %d: \"%s\"\n", cc->cp_input_file_, cc->cp_input_line_, r, situs_col(&cc->cp_input_->situs_), cc->cp_input_->text_);
        }
        else {
          cc_printf(cc, "%s(%d): internal error (%d) from cp_parse at end of file\n", cc->cp_input_file_, cc->cp_input_line_, r);
        }
        return GLSL_ES1_R_FAILED;
      }
    } while (r == _CP_FEED_ME);
  }

preprocessor:
  return GLSL_ES1_R_NEED_INPUT;
}

enum c_compiler_result cc_compile(struct c_compiler *cc) {
  enum c_compiler_result cr;

  do {
    cc_c_parser_skip_template_cruft(cc);

    cr = cc_compiler_stage(cc);
    if (cr != GLSL_ES1_R_NEED_INPUT) {
      return cr;
    }

    cr = cc_preprocessor_stage(cc);
  } while (cr == GLSL_ES1_R_SUCCESS);

  return cr;
}
