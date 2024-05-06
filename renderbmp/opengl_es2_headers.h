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
#ifndef OPENGL_ES2_HEADERS_H
#define OPENGL_ES2_HEADERS_H

/* Check if we should rely on linking to grab standard opengl es2.0 names, or if
 * we link against AEX-GL with the aex_gl_es2_ prefixes
 * This is so we can run and compare demo output with non-AEX-GL platforms */

#ifdef USE_STANDARD_NON_AEX_GL_HEADERS
#ifndef GLES2_GL2_H_INCLUDED
#define GLES2_GL2_H_INCLUDED
#include <GLES2/gl2.h>
#endif
#else
#ifndef GLES2_GL2_H_INCLUDED
#define GLES2_GL2_H_INCLUDED
#define GL_GLES_PROTOTYPES 0  /* we will define the prototypes as static function pointers by including gl_es2_aex_func_map.c */
#include <GLES2/gl2.h>
#endif
#include "gl_es2_aex_func_map.c"
#endif

#endif /* OPENGL_ES2_HEADERS_H */
