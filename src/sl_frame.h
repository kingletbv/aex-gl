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

#ifdef __cplusplus
extern "C" {
#endif

struct sl_type;

struct sl_variable {
  /* Name of the variable, may be NULL if, for some reason, this is an 
   * anonymous variable. */
  char *name_;

  /* Location of the variable; typically the location of name_'s declaration.  */
  struct situs location_;

  /* Next in tail cyclic chain of all variables for a frame.
   * (sl_frame::variables_). */
  struct sl_variable *chain_;

  /* Type of the variable; should always be valid. */
  struct sl_type *type_;

  /* Value of the variable, should always be valid. Is always compatible with 
   * the type of the variable. That is to say: the type of the value should be 
   * the same as type_, except potentially for qualifiers. */
  struct sl_expr_temp value_;

  /* The symbol that represents this variable in a scope, can be NULL if the 
   * scope no longer exists or this is some sort of internal temp variable. */
  struct sym *symbol_;
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
};

struct sl_function {
  /* Name of the function */
  char *name_;

  /* Location of the function; typically the location of name_'s declaration.
   * If a function is first declared and then defined, this situs is updated
   * to the location of the definition. */
  struct situs location_;
       
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
};

struct sl_frame {
  /* Tail cyclic chain of all variables in this frame, in order of declaration. */
  struct sl_variable *variables_;
};

void sl_frame_init(struct sl_frame *frame);
void sl_frame_cleanup(struct sl_frame *frame);

/* Allocates a variable, and adds it to the frame. The name may be NULL or have the name of the
 * variable; similarly, loc can be NULL or be the location where the variable is declared.
 * vartype must be set to the variable's type. If a memory allocation failure occurs, NULL is returned. */
struct sl_variable *sl_frame_alloc_variable(struct sl_frame *frame, const char *name, const struct situs *loc, struct sl_type *vartype);

/* Allocates a function, but does not yet add it to the frame. */
struct sl_function *sl_frame_alloc_function(const char *name, const struct situs *loc, struct sl_type *return_type);

/* Frees a function */
void sl_frame_free_function(struct sl_function *f);

/* Appends a parameter to a function, returns the index to it; if an error occurs, SIZE_MAX
 * is returned instead. */
size_t sl_function_append_parameter(struct sl_function *f, const char *name, const struct situs *loc, struct sl_type *type);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_FRAME_H */
