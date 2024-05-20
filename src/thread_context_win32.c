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

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef THREAD_CONTEXT_H_INCLUDED
#define THREAD_CONTEXT_H_INCLUDED
#include "thread_context.h"
#endif

INIT_ONCE g_tls_keys_initialization_ = INIT_ONCE_STATIC_INIT;

static DWORD g_context_tls_key_ = 0;
static DWORD g_api_tls_key_ = 0;

static BOOL CALLBACK tc_initialize_tls_keys(PINIT_ONCE init_once,
                                            PVOID parameter,
                                            PVOID *ppcontext) {
  DWORD tls_key_ctx = TlsAlloc();
  DWORD tls_key_api = TlsAlloc();
  if ((tls_key_ctx == TLS_OUT_OF_INDEXES) ||
      (tls_key_api == TLS_OUT_OF_INDEXES)) {
    if (tls_key_ctx != TLS_OUT_OF_INDEXES) TlsFree(tls_key_ctx);
    if (tls_key_api != TLS_OUT_OF_INDEXES) TlsFree(tls_key_api);
    return FALSE;
  }
  g_context_tls_key_ = tls_key_ctx;
  g_api_tls_key_ = tls_key_api;
  return TRUE;
}

struct gl_es2_context *tc_get_context(void) {
  BOOL status;
  PVOID ctxp = NULL;
  status = InitOnceExecuteOnce(&g_tls_keys_initialization_, tc_initialize_tls_keys, NULL, &ctxp);
  if (!status) return NULL;
  return TlsGetValue(g_context_tls_key_);
}

void tc_set_context(struct gl_es2_context *pctx) {
  BOOL status;
  PVOID ctxp = NULL;
  status = InitOnceExecuteOnce(&g_tls_keys_initialization_, tc_initialize_tls_keys, NULL, &ctxp);
  if (!status) return;
  TlsSetValue(g_context_tls_key_, (LPVOID)pctx);
}


int tc_get_api(void) {
  BOOL status;
  PVOID ctxp = NULL;
  status = InitOnceExecuteOnce(&g_tls_keys_initialization_, tc_initialize_tls_keys, NULL, &ctxp);
  if (!status) return 0;
  return (int)(uintptr_t)TlsGetValue(g_api_tls_key_);
}

void tc_set_api(int api) {
  BOOL status;
  PVOID ctxp = NULL;
  status = InitOnceExecuteOnce(&g_tls_keys_initialization_, tc_initialize_tls_keys, NULL, &ctxp);
  if (!status) return;
  TlsSetValue(g_api_tls_key_, (LPVOID)(uintptr_t)api);
}
