/* Copyright 2024 Kinglet B.V.
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

#ifndef SL_FRAME_H
#define SL_FRAME_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef SL_EXPR_TEMP_H_INCLUDED
#define SL_EXPR_TEMP_H_INCLUDED
#include "sl_expr_temp.h"
#endif

#ifndef SITUS_H_INCLUDED
#define SITUS_H_INCLUDED
#include "pp/situs.h"
#endif

#ifndef SL_REG_ALLOC_H_INCLUDED
#define SL_REG_ALLOC_H_INCLUDED
#include "sl_reg_alloc.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct sl_type;
struct sl_stmt;
struct sl_frame;
struct sl_function;

struct sl_variable {
  /* Name of the variable, may be NULL if, for some reason, this is an 
   * anonymous variable. */
  char *name_;

  /* Location of the variable; typically the location of name_'s declaration.  */
  struct situs location_;

  /* Next in tail cyclic chain of all variables for a frame.
   * (sl_frame::variables_). */
  struct sl_variable *chain_;

  /* A back pointer to the frame that the variable has been declared in */
  struct sl_frame *frame_;

  /* Type of the variable; should always be valid. */
  struct sl_type *type_;

  /* Value of the variable, should always be valid. Is always compatible with 
   * the type of the variable. That is to say: the type of the value should be 
   * the same as type_, except potentially for qualifiers. */
  struct sl_expr_temp value_;

  /* The register the variable occupies (when not uniform or constant.) */
  struct sl_reg_alloc reg_alloc_;

  /* The symbol that represents this variable in a scope, can be NULL if the 
   * scope no longer exists or this is some sort of internal temp variable. */
  struct sym *symbol_;

  /* If non-zero, this is a parameter to a function, defined in the frame of the 
   * function. */
  int is_parameter_:1;
  size_t parameter_index_;
};

struct sl_parameter {
  /* Name of the parameter; this may be updated to be the name used in the
   * definition if this differs from a declared prototype. */
  char *name_;

  /* Location of the parameter; typically the location of name_'s declaration;
   * updated to reflect the site of definition once that is parsed. */
  struct situs location_;

  /* Type of the parameter; should always be valid. */
  struct sl_type *type_;

  /* The symbol that represents this parameter in a scope, can be NULL if the
   * scope no longer exists or this is some sort of internal temp variable. */
  struct sym *symbol_;

  /* The variable that represents this parameter in the function's frame. */
  struct sl_variable *variable_;
};

struct sl_frame {
  /* Tail cyclic chain of all variables in this frame, in order of declaration. */
  struct sl_variable *variables_;

  /* Pointer to the function this frame is declared for, or alternatively, NULL if
   * this is a global frame. */
  struct sl_function *function_;
};

struct sl_function {
  /* Name of the function */
  char *name_;

  /* Location of the function; typically the location of name_'s declaration.
   * If a function is first declared and then defined, this situs is updated
   * to the location of the definition. */
  struct situs location_;

  /* Next in tail cyclic chain for all functions per sym (there can be
   * multiple, overloaded, functions of the same sym.) */
  struct sl_function *overload_chain_;

  /* Next in tail cyclic chain of all functions for a frame.
   * (sl_frame::functions_). */
  struct sl_function *chain_;
        
  /* Return type of the function; should always be valid. */
  struct sl_type *return_type_;
        
  /* Number of parameters to the function. */
  size_t num_parameters_;
  size_t num_parameters_allocated_;
        
  /* Array of parameters to the function. */
  struct sl_parameter *parameters_;
        
  /* The symbol that represents this function in a scope, given that functions
   * only exist at global scope, this should not become NULL. Like location_,
   * symbol_->loc_ is updated to reflect the site of definition, rather than the
   * site of the prototype, if a prototype is first declared and later a definition
   * is found. */
  struct sym *symbol_;

  /* The frame of the function, this is the frame that contains the function's
   * parameters and local variables. */
  struct sl_frame frame_;

  /* The statements making up the body of this function, if the function has been
   * defined. If the function has only been declared, this will be NULL. If the
   * function has been defined but has an empty body, this will point to the 
   * slsk_compound statement for the function definition (whose scope includes
   * the declarations for the parameters); but the contents of that compound
   * statement will be empty. */
  struct sl_stmt *body_;
};

void sl_frame_init(struct sl_frame *frame);
void sl_frame_cleanup(struct sl_frame *frame);

/* Allocates a variable, and adds it to the frame. The name may be NULL or have the name of the
 * variable; similarly, loc can be NULL or be the location where the variable is declared.
 * vartype must be set to the variable's type. If a memory allocation failure occurs, NULL is returned. */
struct sl_variable *sl_frame_alloc_variable(struct sl_frame *frame, const char *name, const struct situs *loc, struct sl_type *vartype);

/* Returns non-zero if the variable is global (i.e. is declared in a global frame) */
int sl_variable_is_global(const struct sl_variable *v);

/* Allocates a function, but does not yet add it to the frame. */
struct sl_function *sl_frame_alloc_function(const char *name, const struct situs *loc, struct sl_type *return_type);

/* Frees a function */
void sl_frame_free_function(struct sl_function *f);

/* Appends a parameter to a function, returns the index to it; if an error occurs, SIZE_MAX
 * is returned instead. */
size_t sl_function_append_parameter(struct sl_function *f, const char *name, const struct situs *loc, struct sl_type *type);

/* Post-process function parameters. Primarily this involves converting a C-style single 
 * void argument into a 0-argument function, e.g. "int x(void);" becomes "int x();".
 * Returns 0 upon success, non-zero upon error. */
int sl_function_post_process_parameters(struct sl_type_base *tb, struct sl_function *f);

/* Returns 1 if the two function prototypes match, 0 if not.
 * Note that a function match does not mean the match is valid, e.g.
 * int foo(in int a);
 * int foo(out int a);
 * These functions match, but the match is not valid (and so should result in an error), use
 * sl_function_match_validation() to check for this. */
int sl_function_match(struct sl_function *fa, struct sl_function *fb);

int sl_function_match_validation(struct diags *dx, struct sl_function *fnew, struct sl_function *fexisting);

/* Search the current_scope and its parents (in order) for the first function that matches f.
 * current_scope is the sym_table (scope) to begin the search at.
 * f is the function to be searched for.
 * ppst_found_at points to the pointer where the pointer to the sym_table where the function was found at will be stored,
 *               or NULL if no such pointer is to be stored.
 * ppsym_found_at points to the pointer where the pointer to the sym where the function was found at will be stored,
 *                or NULL if no such pointer is to be stored.
 * ppfunc_found points to the pointer where the pointer to the function that was found will be stored,
 *              or NULL if no such pointer is to be stored.
 * If a function is found, it will be stored in *ppfunc_found.
 * If no function is found, but some other symbol is found, then *ppsym_found_at will be set to the symbol and
 * *ppfunc_found to NULL.
 * If no function is found, and no other symbol is found, then *ppsym_found_at and *ppfunc_found will be set to NULL.
 */
void sl_function_search(struct sym_table *current_scope, struct sl_function *f,
                        struct sym_table **ppst_found_at, struct sym **ppsym_found_at, struct sl_function **ppfunc_found);

/* Search the current_scope and its parents (in order) for the first function that matches the name and parameters.
 * current_scope is the sym_table (scope) to begin the search at.
 * name is the name of the function to be searched for.
 * num_params is the number of parameters to the function.
 * param_types is an array of num_params types, which are the types of the parameters to the function.
 * ppst_found_at points to the pointer where the pointer to the sym_table where the function was found at will be stored,
 *               or NULL if no such pointer is to be stored.
 * ppsym_found_at points to the pointer where the pointer to the sym where the function was found at will be stored,
 *                or NULL if no such pointer is to be stored.
 * ppfunc_found points to the pointer where the pointer to the function that was found will be stored,
 *              or NULL if no such pointer is to be stored.
 * If a function is found, it will be stored in *ppfunc_found.
 * If no function is found, but some other symbol is found, then *ppsym_found_at will be set to the symbol and
 * *ppfunc_found to NULL.
 * If no function is found, and no other symbol is found, then *ppsym_found_at and *ppfunc_found will be set to NULL.
 */
void sl_function_call_search(struct sym_table *current_scope, const char *name, size_t num_params, struct sl_type **param_types,
                             struct sym_table **ppst_found_at, struct sym **ppsym_found_at, struct sl_function **ppfunc_found);


void sl_parameter_cleanup(struct sl_parameter *p);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_FRAME_H */
