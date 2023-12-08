#ifndef GLSL_ES1_COMPILER_H
#define GLSL_ES1_COMPILER_H

#ifndef PP_H_INCLUDED
#define PP_H_INCLUDED
#include "pp.h"
#endif

#ifndef GLSL_ES_1_H_INCLUDED
#define GLSL_ES_1_H_INCLUDED
#include "glsl_es_1.h"
#endif

enum glsl_es1_compiler_result {
  GLSL_ES1_R_SUCCESS,
  GLSL_ES1_R_NEED_INPUT,
  GLSL_ES1_R_NEW_INCLUDE,
  GLSL_ES1_R_OLD_INCLUDE,
  GLSL_ES1_R_HAVE_ERRORS,
  GLSL_ES1_R_FAILED
};

struct glsl_es1_compiler {
  struct preprocessor pp_;
  struct glsl_es1_stack parser_;

  int all_done_:1;
  int fatal_error_:1;

  const char *glsl_input_file_;
  int         glsl_input_line_;
};

void glsl_es1_compiler_init(struct glsl_es1_compiler *compiler);
void glsl_es1_compiler_cleanup(struct glsl_es1_compiler *compiler);

#endif /* GLSL_ES1_COMPILER_H */
