#ifndef PP_INPUT_FILE_H
#define PP_INPUT_FILE_H

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

struct pptk;
struct preprocessor;

#ifndef PP_TOKENIZER_H_INCLUDED
#define PP_TOKENIZER_H_INCLUDED
#include "pp_tokenizer.h"
#endif

#ifndef PP_LINE_DIRECTIVES_H_INCLUDED
#define PP_LINE_DIRECTIVES_H_INCLUDED
#include "pp_line_directives.h"
#endif

#ifndef PP_TRIGRAPHS_H_INCLUDED
#define PP_TRIGRAPHS_H_INCLUDED
#include "pp_trigraphs.h"
#endif

#ifndef PP_LINE_CONTINUATIONS_H_INCLUDED
#define PP_LINE_CONTINUATIONS_H_INCLUDED
#include "pp_line_continuations.h"
#endif

#ifndef SITUS_H_INCLUDED
#define SITUS_H_INCLUDED
#include "situs.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum pp_input_file_kind {
  /* input file is a FILE* pointer */
  IFK_FILE,

  /* input file is a static one time memory buffer */
  IFK_MEM,

  /* input file consist of repeatedly supplied buffers */
  IFK_MANUAL
};

struct pp_input_file {
  /* If set, the input file is unpoppable (and EOF will not clear it from the stack) */
  int unpoppable_:1;

  /* Backpointer to the preprocessor where if_set_input() will set the buffer. */
  struct preprocessor *pp_;

  /* Pointer to the containing file (i.e. the file that included this file.) */
  struct pp_input_file *containing_file_;

  /* Name of the file as used in error messages */
  char *filename_;

  /* Caller defined baton */
  void *baton_;

  /* File handle or NULL if this is not a file based input file. */
  FILE *fp_;

  /* Pointer to the callback function that will invoke if_set_input() for the input_file.
   * If NULL, then CCR_NEED_INPUT is returned from cc_compile() instead. */
  int (*input_request_fn_)(void *baton, struct pp_input_file *ifile);

  /* Tokenizer for this input file, converts plain text into pptk tokens. */
  struct pptk_stack pptk_;

  /* Line directives parser */
  struct ppld_stack ppld_; /* line directives */
  int ppld_input_not_finished_ : 1;
  int ppld_end_processed_pop_now_ : 1;

  /* Current input line and filename. Filename is reserved using cc_preserve_filename() (and
   * can be used blindly for the remainder of the cc lifetime.) */
  int ppld_input_line_;
  const char *ppld_input_line_filename_;

  /* The *next* input line and filename. This is 1 ahead of ppld_input_line_ unless the #line
   * directive changes this to a different line and/or filename */
  int pptk_input_line_;
  const char *pptk_input_line_filename_;
  
  /* without concern for #line; used to measure number of input lines processed per actual
   * input line (e.g. taking into account multi-line comment tokens and the like.) */
  int pptk_true_input_line_number_at_start_;

  /* Last line in file is not terminate with a newline; this helps us distinguish two cases
   * when we encounter the end of the input with no tokens pending, either a) we already 
   * pushed an empty final source line, or b) we did not push the last line and the last
   * line happens to be empty. */
  int pptk_pushed_final_source_line_:1;

  /* Tokenizer for line continuations, filters line continuations out from the text */
  struct pplc_stack pplc_;

  /* Situs for the current post-line-continuation input */
  struct situs post_line_continuation_situs_;

  /* Buffer holding input following line continuation processing */
  char *post_line_continuation_;
  size_t post_line_continuation_size_;
  size_t post_line_continuation_size_allocated_;

  /* Tokenizer for the trigraphs, converts plain text into text with the trigraphs replaced */
  struct pptg_stack pptg_;

  /* Situs for the current post-trigraph input (feeds into line continuation processing.) */
  struct situs post_trigraph_situs_;

  /* Buffer holding input following tri-graph conversion (feeds into line-continuation 
   * processing.) */
  char *post_trigraph_;
  size_t post_trigraph_size_;
  size_t post_trigraph_size_allocated_;

  /* Array (attached to the end of struct pp_input_file) of auxiliary data; the input_file
   * allocation is expanded to accommodate if its needed (typically used as a read buffer
   * space.) */
  char aux_[1];
};

void if_init(struct pp_input_file *ifile, struct preprocessor *pp);
void if_cleanup(struct pp_input_file *ifile);

/* Returns non-zero in case of memory failure, zero upon success. */
int if_set_filename(struct preprocessor *pp, struct pp_input_file *ifile, const char *filename);

void if_set_input(struct pp_input_file *ifile, const void *input_buf, size_t input_buf_size, int is_final);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PP_INPUT_FILE_H */
