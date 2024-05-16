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
#include <Windows.h>
#endif

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef WCHAR_H_INCLUDED
#define WCHAR_H_INCLUDED
#include <wchar.h>
#endif

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#define EGL_IMPL_C /* this switches some declarations in egl_imp.h */

#ifndef EGL_IMPL_H_INCLUDED
#define EGL_IMPL_H_INCLUDED
#include "egl_impl.h"
#endif

#define AEX_WSTRINGIZE_IMPL(x) L#x
#define AEX_WSTRINGIZE(x) AEX_WSTRINGIZE_IMPL(x)

int g_aex_egl_error_ = 0;

/* Returns AEX_EGL_TRUE if err was EAX_EGL_SUCCESS, or AEX_EGL_FALSE otherwise.
 * Note that this only changes the error code in g_aex_egl_error if the pre-existing
 * value is either 0 or AEX_EGL_SUCCESS, otherwise the pre-existing error is preserved. */
static aex_egl_boolean_t set_egl_err(int err) {
  if (!g_aex_egl_error_ || (g_aex_egl_error_ == AEX_EGL_SUCCESS)) {
    g_aex_egl_error_ = err;
  }
  return (err == AEX_EGL_SUCCESS) ? AEX_EGL_TRUE : AEX_EGL_FALSE;

}

static aex_egl_display_t g_default_display_ = NULL;

static void aex_egl_config_init(aex_egl_config_t config) {
  config->unused_ = 0;
}

static void aex_egl_config_cleanup(aex_egl_config_t config) {}

static aex_egl_config_t aex_egl_config_alloc(void) {
  aex_egl_config_t config = (aex_egl_config_t)malloc(sizeof(struct aex_egl_config));
  if (!config) return NULL;
  aex_egl_config_init(config);
  return config;
}

static void aex_egl_config_free(aex_egl_config_t config) {
  if (!config) return;
  aex_egl_config_cleanup(config);
  free(config);
}

static void aex_egl_display_init(struct aex_egl_display *dp) {
  dp->hdc = INVALID_HANDLE_VALUE;
  dp->initialized_ = 0; /* switched by eglInitialize() */
  aex_egl_config_init(&dp->display_config_);
}

static void aex_egl_display_cleanup(struct aex_egl_display *dp) {
  aex_egl_config_cleanup(&dp->display_config_);
}

static struct aex_egl_display *aex_egl_display_alloc(void) {
  struct aex_egl_display *dp = (struct aex_egl_display *)malloc(sizeof(struct aex_egl_display));
  if (!dp) return NULL;
  aex_egl_display_init(dp);
  return dp;
}

static void aex_egl_display_free(struct aex_egl_display *dp) {
  if (!dp) return;
  aex_egl_display_cleanup(dp);
  free(dp);
}

static void aex_egl_context_init(aex_egl_context_t context) {
  context->unused_ = 0;
}

static void aex_egl_context_cleanup(aex_egl_context_t context) {
}

static aex_egl_context_t aex_egl_context_alloc(void) {
  aex_egl_context_t context = (aex_egl_context_t)malloc(sizeof(struct aex_egl_context));
  if (!context) return NULL;
  aex_egl_context_init(context);
  return context;
}

static void aex_egl_context_free(aex_egl_context_t context) {
  if (!context) return;
  aex_egl_context_cleanup(context);
  free(context);
}

static void aex_egl_surface_init(aex_egl_surface_t surface) {
  surface->hwnd_ = INVALID_HANDLE_VALUE;
}

static void aex_egl_surface_cleanup(aex_egl_surface_t surface) {
}

static aex_egl_surface_t aex_egl_surface_alloc(void) {
  aex_egl_surface_t surface = (aex_egl_surface_t)malloc(sizeof(struct aex_egl_surface));
  if (!surface) return NULL;
  aex_egl_surface_init(surface);
  return surface;
}

static void aex_egl_surface_free(aex_egl_surface_t surface) {
  if (!surface) return;
  aex_egl_surface_cleanup(surface);
  free(surface);
}

static void aex_egl_at_exit(void) {
  if (g_default_display_) aex_egl_display_free(g_default_display_);
}

AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(ChooseConfig)(aex_egl_display_t dpy, const aex_egl_int *attrib_list, aex_egl_config_t *configs, aex_egl_int config_size, aex_egl_int *num_config) {
  if (dpy != g_default_display_) {
    return set_egl_err(AEX_EGL_BAD_DISPLAY);
  }

  OutputDebugString(AEX_WSTRINGIZE(AEX_EGL_FUNCTION_ID(ChooseConfig)) "\n");

  const aex_egl_int *pattrib = attrib_list;
  aex_egl_int no_list = AEX_EGL_NONE;
  if (!pattrib) pattrib = &no_list;
  while (*pattrib != AEX_EGL_NONE) {
    aex_egl_int a = *pattrib++;
    aex_egl_int value = *pattrib++;
    const wchar_t *attrib_name;
    wchar_t unknown_attrib[64];

    switch (a) {
      case AEX_EGL_ALPHA_MASK_SIZE: attrib_name  = L"EGL_ALPHA_MASK_SIZE"; break;
      case AEX_EGL_ALPHA_SIZE: attrib_name  = L"EGL_ALPHA_SIZE"; break;
      case AEX_EGL_BIND_TO_TEXTURE_RGB: attrib_name  = L"EGL_BIND_TO_TEXTURE_RGB"; break;
      case AEX_EGL_BIND_TO_TEXTURE_RGBA: attrib_name  = L"EGL_BIND_TO_TEXTURE_RGBA"; break;
      case AEX_EGL_BLUE_SIZE: attrib_name  = L"EGL_BLUE_SIZE"; break;
      case AEX_EGL_BUFFER_SIZE: attrib_name  = L"EGL_BUFFER_SIZE"; break;
      case AEX_EGL_COLOR_BUFFER_TYPE: attrib_name  = L"EGL_COLOR_BUFFER_TYPE"; break;
      case AEX_EGL_CONFIG_CAVEAT: attrib_name  = L"EGL_CONFIG_CAVEAT"; break;
      case AEX_EGL_CONFIG_ID: attrib_name  = L"EGL_CONFIG_ID"; break;
      case AEX_EGL_CONFORMANT: attrib_name  = L"EGL_CONFORMANT"; break;
      case AEX_EGL_DEPTH_SIZE: attrib_name  = L"EGL_DEPTH_SIZE"; break;
      case AEX_EGL_GREEN_SIZE: attrib_name  = L"EGL_GREEN_SIZE"; break;
      case AEX_EGL_LEVEL: attrib_name  = L"EGL_LEVEL"; break;
      case AEX_EGL_LUMINANCE_SIZE: attrib_name  = L"EGL_LUMINANCE_SIZE"; break;
      case AEX_EGL_MATCH_NATIVE_PIXMAP: attrib_name  = L"EGL_MATCH_NATIVE_PIXMAP"; break;
      case AEX_EGL_NATIVE_RENDERABLE: attrib_name  = L"EGL_NATIVE_RENDERABLE"; break;
      case AEX_EGL_MAX_SWAP_INTERVAL: attrib_name  = L"EGL_MAX_SWAP_INTERVAL"; break;
      case AEX_EGL_MIN_SWAP_INTERVAL: attrib_name  = L"EGL_MIN_SWAP_INTERVAL"; break;
      case AEX_EGL_RED_SIZE: attrib_name  = L"EGL_RED_SIZE"; break;
      case AEX_EGL_SAMPLE_BUFFERS: attrib_name  = L"EGL_SAMPLE_BUFFERS"; break;
      case AEX_EGL_SAMPLES: attrib_name  = L"EGL_SAMPLES"; break;
      case AEX_EGL_STENCIL_SIZE: attrib_name  = L"EGL_STENCIL_SIZE"; break;
      case AEX_EGL_RENDERABLE_TYPE: attrib_name  = L"EGL_RENDERABLE_TYPE"; break;
      case AEX_EGL_SURFACE_TYPE: attrib_name  = L"EGL_SURFACE_TYPE"; break;
      case AEX_EGL_TRANSPARENT_TYPE: attrib_name  = L"EGL_TRANSPARENT_TYPE"; break;
      case AEX_EGL_TRANSPARENT_RED_VALUE: attrib_name  = L"EGL_TRANSPARENT_RED_VALUE"; break;
      case AEX_EGL_TRANSPARENT_GREEN_VALUE: attrib_name  = L"EGL_TRANSPARENT_GREEN_VALUE"; break;
      case AEX_EGL_TRANSPARENT_BLUE_VALUE: attrib_name  = L"EGL_TRANSPARENT_BLUE_VALUE"; break;
      default:
        swprintf(unknown_attrib, sizeof(unknown_attrib)/sizeof(*unknown_attrib), L"Unknown attribute (0x%04X)", a);
        attrib_name = unknown_attrib;
        break;
    }

    wchar_t buffer[256];
    swprintf(buffer, sizeof(buffer)/sizeof(*buffer), L"%s: %d\n", attrib_name, value);
    OutputDebugStringW(buffer);
  }

  return AEX_EGL_TRUE;
}

AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(GetConfigAttrib)(aex_egl_display_t dpy, aex_egl_config_t config, aex_egl_int attribute, aex_egl_int *value) {

  switch (attribute) {
    case AEX_EGL_ALPHA_MASK_SIZE: 
      break;
    case AEX_EGL_BIND_TO_TEXTURE_RGB: 
      break;
    case AEX_EGL_BIND_TO_TEXTURE_RGBA: 
      break;
    case AEX_EGL_RED_SIZE:
      if (value) *value = 8;
      break;
    case AEX_EGL_GREEN_SIZE:
      if (value) *value = 8;
      break;
    case AEX_EGL_BLUE_SIZE:
      if (value) *value = 8;
      break;
    case AEX_EGL_ALPHA_SIZE:
      if (value) *value = 8;
      break;
    case AEX_EGL_BUFFER_SIZE:
      break;
    case AEX_EGL_COLOR_BUFFER_TYPE: 
      if (value) *value = AEX_EGL_RGB_BUFFER;
      break;
    case AEX_EGL_CONFIG_CAVEAT: 
      break;
    case AEX_EGL_CONFIG_ID: 
      break;
    case AEX_EGL_CONFORMANT: 
      if (value) *value = AEX_EGL_OPENGL_ES2_BIT;
      break;
    case AEX_EGL_DEPTH_SIZE: 
      if (value) *value = 16; /* should also support 32 bit (!!) */
      break;
    case AEX_EGL_LEVEL:
      break;
    case AEX_EGL_LUMINANCE_SIZE: 
      break;
    case AEX_EGL_MAX_PBUFFER_WIDTH: 
      break;
    case AEX_EGL_MAX_PBUFFER_HEIGHT: 
      break;
    case AEX_EGL_MAX_PBUFFER_PIXELS: 
      break;
    case AEX_EGL_MAX_SWAP_INTERVAL: 
      break;
    case AEX_EGL_MIN_SWAP_INTERVAL: 
      break;
    case AEX_EGL_NATIVE_RENDERABLE: 
      break;
    case AEX_EGL_NATIVE_VISUAL_ID: 
      break;
    case AEX_EGL_NATIVE_VISUAL_TYPE: 
      break;
    case AEX_EGL_RENDERABLE_TYPE: 
      if (value) *value = AEX_EGL_OPENGL_ES2_BIT;
      break;
    case AEX_EGL_SAMPLE_BUFFERS: 
      break;
    case AEX_EGL_SAMPLES: 
      if (value) *value = 0; /* Don't support MSAA */
      break;
    case AEX_EGL_STENCIL_SIZE: 
      if (value) *value = 16;
      break;
    case AEX_EGL_SURFACE_TYPE: 
      if (value) *value = AEX_EGL_WINDOW_BIT;
      break;
    case AEX_EGL_TRANSPARENT_TYPE: 
      break;
    case AEX_EGL_TRANSPARENT_RED_VALUE: 
      break;
    case AEX_EGL_TRANSPARENT_GREEN_VALUE: 
      break;
    case AEX_EGL_TRANSPARENT_BLUE_VALUE: 
      break;
  }

  return AEX_EGL_FALSE;
}


AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(GetConfigs)(aex_egl_display_t dpy, aex_egl_config_t *configs, aex_egl_int config_size, aex_egl_int *num_config) {
  if (dpy != g_default_display_) {
    return set_egl_err(AEX_EGL_BAD_DISPLAY);
  }

  if (!num_config) {
    return set_egl_err(AEX_EGL_BAD_PARAMETER);
  }

  if (!dpy->initialized_) {
    return set_egl_err(AEX_EGL_NOT_INITIALIZED);
  }

  *num_config = 1;
  if (configs && config_size > 0) {
    *configs = &dpy->display_config_;
  }

  return AEX_EGL_TRUE;
}

AEX_EGL_DECL_SPEC aex_egl_context_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(CreateContext)(aex_egl_display_t dpy, aex_egl_config_t config, aex_egl_context_t share_context, const aex_egl_int *attrib_list) {
  OutputDebugString(AEX_WSTRINGIZE(AEX_EGL_FUNCTION_ID(CreateContext)) "\n");

  const aex_egl_int *pattrib = attrib_list;
  aex_egl_int no_list = AEX_EGL_NONE;
  if (!pattrib) pattrib = &no_list;
  while (*pattrib != AEX_EGL_NONE) {
    aex_egl_int a = *pattrib++;
    aex_egl_int value = *pattrib++;
    const wchar_t *attrib_name;
    wchar_t unknown_attrib[64];

    switch (a) {
      case AEX_EGL_CONTEXT_MAJOR_VERSION: attrib_name = L"EGL_CONTEXT_MAJOR_VERSION"; break;
      case AEX_EGL_CONTEXT_MINOR_VERSION: attrib_name = L"EGL_CONTEXT_MINOR_VERSION"; break;
      case AEX_EGL_CONTEXT_OPENGL_PROFILE_MASK: attrib_name = L"EGL_CONTEXT_OPENGL_PROFILE_MASK"; break;
      case AEX_EGL_CONTEXT_OPENGL_DEBUG: attrib_name = L"EGL_CONTEXT_OPENGL_DEBUG"; break;
      case AEX_EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE: attrib_name = L"EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE"; break;
      case AEX_EGL_CONTEXT_OPENGL_ROBUST_ACCESS: attrib_name = L"EGL_CONTEXT_OPENGL_ROBUST_ACCESS"; break;
      case AEX_EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY: attrib_name = L"EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY"; break;
      default:
        swprintf(unknown_attrib, sizeof(unknown_attrib)/sizeof(*unknown_attrib), L"Unknown attribute (0x%04X)", a);
        attrib_name = unknown_attrib;
        break;
    }

    wchar_t buffer[256];
    swprintf(buffer, sizeof(buffer)/sizeof(*buffer), L"  %s: %d\n", attrib_name, value);
    OutputDebugStringW(buffer);
  }

  aex_egl_context_t ctx = aex_egl_context_alloc();
  if (!ctx) {
    set_egl_err(AEX_EGL_BAD_ALLOC);
    return AEX_EGL_NO_CONTEXT;
  }

  return ctx;
}


AEX_EGL_DECL_SPEC aex_egl_surface_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(CreatePbufferSurface)(aex_egl_display_t dpy, aex_egl_config_t config, const aex_egl_int *attrib_list) {
  return NULL;
}


AEX_EGL_DECL_SPEC aex_egl_surface_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(CreateWindowSurface)(aex_egl_display_t dpy, aex_egl_config_t config, aex_egl_native_window_t win, const aex_egl_int *attrib_list) {
  aex_egl_surface_t surf = aex_egl_surface_alloc();
  if (!surf) {
    set_egl_err(AEX_EGL_BAD_ALLOC);
    return AEX_EGL_NO_SURFACE;
  }
  surf->hwnd_ = win;

  return surf;
}


AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(DestroyContext)(aex_egl_display_t dpy, aex_egl_context_t ctx) {
  return AEX_EGL_FALSE;
}


AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(DestroySurface)(aex_egl_display_t dpy, aex_egl_surface_t surface) {
  if (dpy != g_default_display_) {
    return set_egl_err(AEX_EGL_BAD_DISPLAY);
  }
  if (!dpy->initialized_) {
    return set_egl_err(AEX_EGL_NOT_INITIALIZED);
  }
  if (surface) {
    aex_egl_surface_free(surface);
    return AEX_EGL_TRUE;
  }
  
  return set_egl_err(AEX_EGL_BAD_SURFACE);
}

AEX_EGL_DECL_SPEC aex_egl_display_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(GetDisplay)(aex_egl_native_display_t display_id) {
  if (g_default_display_) return g_default_display_;
  aex_egl_display_t dp = aex_egl_display_alloc();
  if (!dp) return NULL;
  dp->hdc = display_id;
  g_default_display_ = dp;
  atexit(aex_egl_at_exit);

  return dp;
}


AEX_EGL_DECL_SPEC aex_egl_int       AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(GetError)(void) {
  int err = g_aex_egl_error_;
  if (!err) return AEX_EGL_SUCCESS; /* note: this constant is not 0 but 0x3000 */
  g_aex_egl_error_ = 0;
  return err;
}


AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(Initialize)(aex_egl_display_t dpy, aex_egl_int *major, aex_egl_int *minor) {
  if (dpy != g_default_display_) {
    set_egl_err(AEX_EGL_BAD_DISPLAY);
    return AEX_EGL_FALSE;
  }
  dpy->initialized_ = 1;

  /* EGL version numbers, we go low here */
  if (major) *major = 1;
  if (minor) *minor = 0;
  return AEX_EGL_TRUE;
}


AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(MakeCurrent)(aex_egl_display_t dpy, aex_egl_surface_t draw, aex_egl_surface_t read, aex_egl_context_t ctx) {
  return AEX_EGL_FALSE;
}


AEX_EGL_DECL_SPEC const char *AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(QueryString)(aex_egl_display_t dpy, aex_egl_int name) {
  return NULL;
}


AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(SwapBuffers)(aex_egl_display_t dpy, aex_egl_surface_t surface) {
  return AEX_EGL_FALSE;
}


AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(Terminate)(aex_egl_display_t dpy) {
  if (!dpy || (dpy != g_default_display_)) {
    set_egl_err(AEX_EGL_BAD_DISPLAY);
    return AEX_EGL_FALSE;
  }

  aex_egl_display_free(dpy);
  if (dpy == g_default_display_) g_default_display_ = NULL;
  return AEX_EGL_TRUE;

  return AEX_EGL_FALSE;
}


AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(WaitGL)(void) {
  return AEX_EGL_FALSE;
}


AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(WaitNative)(aex_egl_int engine) {
  return AEX_EGL_FALSE;
}


AEX_EGL_DECL_SPEC aex_egl_generic_function_ptr_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(GetProcAddress)(const char *procname) {
  return NULL;
}


AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(SwapInterval)(aex_egl_display_t dpy, aex_egl_int interval) {
  return AEX_EGL_FALSE;
}

AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(BindAPI)(aex_egl_enum_t api) {
  if (api == AEX_EGL_OPENGL_ES_API) {
    return AEX_EGL_TRUE;
  }
  return set_egl_err(AEX_EGL_BAD_PARAMETER);
}
