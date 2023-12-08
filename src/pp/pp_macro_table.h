#ifndef PP_MACRO_TABLE_H
#define PP_MACRO_TABLE_H

#ifndef STDDEF_H_INCLUDED
#define STDDEF_H_INCLUDED
#include <stddef.h> /* size_t */
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct pptk;

enum macro_table_result {
  MACRO_TABLE_OK = 0,
  MACRO_TABLE_NOMEM = -1,
  MACRO_TABLE_DUPLICATE = -2,
  MACRO_TABLE_NOT_FOUND = -3
};

struct macro_table {
  /* Root of the red-black tree of macros. */
  struct macro *root_;

  /* First macro in cyclic list of macros in order of parsing. */
  struct macro *seq_;
};

struct macro {
  /* Red-black tree fields. */
  int is_red_:1;
  struct macro *left_, *right_;

  /* Next and previous macros in cyclic list of macros in order of parsing. */
  struct macro *prev_, *next_;

  /* Name of the macro (ends with null terminator) - the name_len_ is the length
   * excluding the null terminator. */
  char *name_;
  size_t name_len_;

  /* Number of times this macro is referenced. For instance, if we parse a function macro
   * invocation, and, while parsing that macro function invocation, the macro is #undef'ed:
   *    x(
   * #undef x
   *    param-to-x)
   * Then the #undef will remove the macro from the symbol table, but the macro will continue
   * to be valid until the expansion completes.
   */
  int refcount_;

  /* A function style macro need not have arguments; hence the flag */
  int is_function_style_:1;

  /* Function style macros with "..." as their last argument are variadic  */
  int is_variadic_:1;

  /* Function or object style macro is already being invoked.. This guards against nested invocation
   * of the same macro, e.g. "#define x x" does not infinitely recurse. */
  int nested_invocation_:1;

  struct pptk *args_;
  struct pptk *replacement_list_;

};

void mt_init(struct macro_table *obj);
void mt_cleanup(struct macro_table *obj);
int mt_insert(struct macro_table *obj,
              const char *name, size_t name_len,
              struct macro **pp_new_macro);
int mt_remove(struct macro_table *obj, const char *name, size_t name_len);
int mt_remove_macro(struct macro_table *obj, struct macro *s);
struct macro *mt_find(struct macro_table *obj, const char *name, size_t name_len);
const struct macro *mt_find_c(const struct macro_table *obj, const char *name, size_t name_len);

void macro_free(struct macro *m);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PP_MACRO_TABLE_H */
