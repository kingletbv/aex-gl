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

#ifndef THREAD_CONTEXT_H
#define THREAD_CONTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

struct gl_es2_context;

#define TC_OPENGL_NO_API 0
#define TC_OPENGL_ES_API 1
#define TC_OPENGL_ES2_API 2
#define TC_OPENGL_VG_API 3

/* Get context from thread local storage, default NULL if uninintialized */
struct gl_es2_context *tc_get_context(void);

/* Set pctx as the context for the current thread, note that if you set this to
 * NULL, any subsequent OpenGL API call will attempt to use the default context,
 * creating it if necessary. */
void tc_set_context(struct gl_es2_context *pctx);

/* Get default context, create one if it doesn't yet exist.
 * Note: creation of the default context installs an atexit() handler to clean it up. */
struct gl_es2_context *tc_get_default_context(void);

/* Get current API from thread local storage, default TC_OPENGL_NO_API if uninitialized */
int tc_get_api(void);

/* Sets the API value for the current thread, one of the TC_OPENGL_XXX_API values. */
void tc_set_api(int api);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* THREAD_CONTEXT_H */
