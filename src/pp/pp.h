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

#ifndef PP_H
#define PP_H

#ifndef PP_MACRO_TABLE_H_INCLUDED
#define PP_MACRO_TABLE_H_INCLUDED
#include "pp_macro_table.h"
#endif

struct preprocessor;

#ifndef PP_MACRO_EXPANDER_H_INCLUDED
#define PP_MACRO_EXPANDER_H_INCLUDED
#include "pp_macro_expander.h"
#endif

#ifndef SITUS_H_INCLUDED
#define SITUS_H_INCLUDED
#include "situs.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct pp_input_file;

enum preprocessor_result {
  PPR_SUCCESS,
  PPR_NEED_INPUT,
  PPR_NEW_INCLUDE,
  PPR_OLD_INCLUDE,
  PPR_HAVE_ERRORS,
  PPR_FAILED
};

enum pp_if_section_state {
  PP_IFSS_SKIP, /* all groups in if-section should be skipped */
  PP_IFSS_SELECTED, /* current group should have tokens selected, subsequent #else or #elif groups should skip */
  PP_IFSS_NOT_YET_SELECTED /* current group should be skipped, subsequent #else should be selected, subsequent #elif should be evaluated */
};

/* Preprocessor #if section as it is being parsed. */
struct pp_if_section {
  enum pp_if_section_state state_;
  struct pp_if_section *parent_; /* #endif pops to parent, if there is a parent. */
};

struct pp_filename_buffer {
  struct pp_filename_buffer *next_;
  char filename_[1];
};

struct preprocessor {
  /* Error flags, indicates whether compilation should be aborted,
   * the fatal_error_ indicates if compilation should be aborted _now_. */
  int have_error_:1;
  int fatal_error_:1;

  /* The macro table; houses all object and function macros; 
   * builtin macros must also have an entry */
  struct macro_table macro_table_;

  /* Stack of #if sections. */
  struct pp_if_section *if_section_stack_;

  /* Macro expansions */
  int ppme_input_final_:1;
  struct pptk *ppme_input_;
  const char *ppme_input_file_;
  int ppme_input_line_;
  struct ppme_stack ppme_; /* macro-expander */

  /* Line directive processing (intermediate phase of translation) */
  int ppld_input_final_:1;
  int have_ppld_input_line_:1;
  struct pptk *ppld_input_;

  /* Input stack, this is the stack of files #include'ed by the preprocessor */
  struct pp_input_file *input_stack_;

  /* Output from the preprocessor, ready for compilation */
  struct pptk *pp_output_;
  int pp_output_final_:1;

  /* All filenames (in situs and elsewhere) are allocated from these buffers and
   * then passed around as pointers. They are not freed until the preprocessor itself
   * is cleaned up. */
  struct pp_filename_buffer *filename_buffers_;

  /* The preprocessor assigns an early predecessor of types to some literals, this
   * may depend on which type fits the size of the literal. Hence we have to inform
   * it of the bits per type. Always two's complement.
   */
  int bits_per_char_;       /* typically 8 */
  int bits_per_short_;      /* typically 16 */
  int bits_per_int_;        /* typically 32 */
  int bits_per_long_;       /* typically 32 */
  int bits_per_long_long_;  /* typically 64 */

  /* The filename argument for the most recent #include preprocessing directive. */
  int include_is_system_include_ : 1;  /* if non-zero, #include <header.h> was used, if zero #include "header.h" was used. */
  int include_is_template_emit_ : 1;   /* if non-zero, #emit "header.h" was used, if zero, #include was used. */
  char *include_file_arg_;

  /* Print handlers for error reporting */
  int (*vprintf_handler)(void *printf_baton, const char *file, int line_num, const char *fmt, va_list args);
  void *vprintf_baton_;
};

void pp_init(struct preprocessor *pp);
void pp_cleanup(struct preprocessor *pp);

enum preprocessor_result pp_preprocessor_stage(struct preprocessor *pp);

int pp_printf(struct preprocessor *pp, const char *fmt, ...);
int pp_error_loc(struct preprocessor *pp, struct situs *sit, const char *fmt, ...);
int pp_error(struct preprocessor *pp, const char *fmt, ...);
int pp_fatal_loc(struct preprocessor *pp, struct situs *sit, const char *fmt, ...);
int pp_fatal(struct preprocessor *pp, const char *fmt, ...);
int pp_warn_loc(struct preprocessor *pp, struct situs *sit, const char *fmt, ...);
int pp_warn(struct preprocessor *pp, const char *fmt, ...);
void pp_no_memory(struct preprocessor *pp);

struct pp_if_section *pp_if_push(struct preprocessor *pp);
struct pp_input_file *pp_push_input_file(struct preprocessor *pp, const char *filename, size_t aux_data);
void pp_if_pop(struct preprocessor *pp);


int pp_concat_sub_output(struct preprocessor *pp, struct situs *output_situs, char **output_buf, size_t *output_pos, size_t *output_buf_size, struct situs *span_situs, const char *span_text);
int pp_concat_sub_output_b(struct preprocessor *pp, struct situs *output_situs, char **output_buf, size_t *output_pos, size_t *output_buf_size, struct situs *span_situs, const char *span_text, size_t span_text_len);
int pp_concat_output(struct preprocessor *pp, struct situs *output_situs, char **output_buf, size_t *output_pos, size_t *output_buf_size, struct situs *span_situs, const char *span_text);
int pp_concat_output_b(struct preprocessor *pp, struct situs *output_situs, char **output_buf, size_t *output_pos, size_t *output_buf_size, struct situs *span_situs, const char *span_text, size_t span_text_len);

/* Creates a copy of filename in a preprocessor managed buffer and returns a pointer to it.
 * The pointer remains valid for the lifetime of the preprocessor. */
char *pp_preserve_filename(struct preprocessor *pp, const char *filename);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PP_H */
