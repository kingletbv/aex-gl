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

#ifndef WINDOWS_H_INCLUDED
#define WINDOWS_H_INCLUDED
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0600
#include <Windows.h>
#endif

#ifndef THREAD_MUTEX_H_INCLUDED
#define THREAD_MUTEX_H_INCLUDED
#include "thread_mutex.h"
#endif

void thread_mutex_init(struct thread_mutex *m) {
  /* Increase the size of thread_mutex::native_mutex_ if this fails to compile, it needs to be
   * able to fit the native mutex size. */
  char is_native_mutex_large_enough_static_assertion[(sizeof(m->native_mutex_) >= sizeof(CRITICAL_SECTION)) ? 1 : -1];
  (void)is_native_mutex_large_enough_static_assertion;
  CRITICAL_SECTION *cs = (CRITICAL_SECTION *)m->native_mutex_;
  InitializeCriticalSection(cs);
}

int thread_mutex_finish_initialization(struct thread_mutex *m) {
  /* No further initialization needed */
  return 0;
}

void thread_mutex_cleanup(struct thread_mutex *m) {
  CRITICAL_SECTION *cs = (CRITICAL_SECTION *)m->native_mutex_;
  DeleteCriticalSection(cs);
}

void thread_mutex_enter(struct thread_mutex *m) {
  CRITICAL_SECTION *cs = (CRITICAL_SECTION *)m->native_mutex_;
  EnterCriticalSection(cs);
}

void thread_mutex_leave(struct thread_mutex *m) {
  CRITICAL_SECTION *cs = (CRITICAL_SECTION *)m->native_mutex_;
  LeaveCriticalSection(cs);
}
