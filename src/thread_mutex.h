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
#ifndef THREAD_MUTEX_H
#define THREAD_MUTEX_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct thread_mutex {
  /* Size here needs to be "sufficient" to get the native mutex primitive to fit.
   * Compilation will fail on the respective target platform if it's too small.
   * (Goal here is to avoid bringing in the kitchen sink just for a mutex. */
  uint64_t native_mutex_[5];
};

void thread_mutex_init(struct thread_mutex *m);
void thread_mutex_cleanup(struct thread_mutex *m);

/* Finalize initialization, including any allocation or construction that may fail.
 * Returns non-zero upon failure, zero upon success. */
int thread_mutex_finish_initialization(struct thread_mutex *m);

void thread_mutex_enter(struct thread_mutex *m);
void thread_mutex_leave(struct thread_mutex *m);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* THREAD_MUTEX_H */
