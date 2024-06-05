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

#ifndef THREAD_CONTEXT_H_INCLUDED
#define THREAD_CONTEXT_H_INCLUDED
#include "thread_context.h"
#endif

#define EGL_IMPL_C /* this switches some declarations in egl_imp.h */

#ifndef SL_DEFS_H_INCLUDED
#define SL_DEFS_H_INCLUDED
#include "sl_defs.h"
#endif

#ifndef EGL_IMPL_H_INCLUDED
#define EGL_IMPL_H_INCLUDED
#include "egl_impl.h"
#endif

#ifndef GL_ES2_IMPL_H_INCLUDED
#define GL_ES2_IMPL_H_INCLUDED
#include "gl_es2_impl.h"
#endif

#ifndef DEBUG_DUMP_H_INCLUDED
#define DEBUG_DUMP_H_INCLUDED
#include "debug_dump.h"
#endif

#define AEX_WSTRINGIZE_IMPL(x) L#x
#define AEX_WSTRINGIZE(x) AEX_WSTRINGIZE_IMPL(x)

int g_aex_egl_error_ = 0;

static aex_egl_context_t egl_context_from_gl_es2_context(struct gl_es2_context *c) {
  uintptr_t offset = (uintptr_t)&(((aex_egl_context_t)0)->gl_es2_ctx_);
  uintptr_t c_uintptr = (uintptr_t)c;
  uintptr_t egl_uintptr = c_uintptr - offset;
  aex_egl_context_t ctx = (aex_egl_context_t)egl_uintptr;
  return ctx;
}

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
  context->was_current_before_ = 0;
  gl_es2_ctx_init(&context->gl_es2_ctx_);
  context->current_draw_hwnd_ = INVALID_HANDLE_VALUE;
}

static void aex_egl_context_cleanup(aex_egl_context_t context) {
  gl_es2_ctx_cleanup(&context->gl_es2_ctx_);
}

static aex_egl_context_t aex_egl_context_alloc(void) {
  aex_egl_context_t context = (aex_egl_context_t)malloc(sizeof(struct aex_egl_context));
  if (!context) return NULL;
  aex_egl_context_init(context);

  if (gl_es2_ctx_complete_initialization(&context->gl_es2_ctx_)) {
    aex_egl_context_cleanup(context);
    free(context);
    return NULL;
  }

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
  ctx->gl_es2_ctx_.is_egl_context_ = 1;

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
  if (!ctx) {
    tc_set_context(NULL);
    return AEX_EGL_TRUE;
  }

  tc_set_context(&ctx->gl_es2_ctx_);

  struct gl_es2_context *c = gl_es2_ctx();
  if (c != &ctx->gl_es2_ctx_) {
    /* Something went very wrong */
    gl_es2_ctx_release(c);
    return set_egl_err(AEX_EGL_CONTEXT_LOST);
  }

  /* We have set the config and it is locked */

  HWND hwnd = draw->hwnd_;
  if (hwnd == INVALID_HANDLE_VALUE) {
    gl_es2_ctx_release(c);
    return set_egl_err(AEX_EGL_BAD_NATIVE_WINDOW);
  }
  RECT client_rect;
  GetClientRect(hwnd, &client_rect);
  int client_width, client_height;
  client_width = (int)(client_rect.right - client_rect.left);
  client_height = (int)(client_rect.bottom - client_rect.top);

  int need_fb_storage_alloc = 0;

  if (!ctx->was_current_before_) {
    /* Set viewport and scissor rectangles to size of the native surface */
    c->vp_width_ = (int32_t)client_width;
    c->vp_height_ = (int32_t)client_height;
    c->vp_x_ = 0;
    c->vp_y_ = 0;
    c->scissor_width_ = (int32_t)client_width;
    c->scissor_height_ = (int32_t)client_height;
    c->scissor_left_ = 0;
    c->scissor_bottom_counted_from_bottom_ = 0;
    ctx->was_current_before_ = 1;
    need_fb_storage_alloc = 1;
  }
  else {
    int fb_width, fb_height;
    int r;
    struct gl_es2_framebuffer *fb = (struct gl_es2_framebuffer *)not_find(&c->framebuffer_not_, 0);
    if (!fb) {
      need_fb_storage_alloc = 1;
    }
    else {
      r = gl_es2_framebuffer_get_dims(fb, &fb_width, &fb_height);
      if (!r) {
        need_fb_storage_alloc = 1;
      }
      if ((fb_width != client_width) || (fb_height != client_height)) {
        need_fb_storage_alloc = 1;
      }
    }
  }

  egl_context_from_gl_es2_context(c)->current_draw_hwnd_ = hwnd;

  gl_es2_ctx_release(c);
  if (need_fb_storage_alloc) {
    int sl_err = GL_ES2_FUNCTION_ID(context_set_framebuffer_storage)(32, 16, 16, client_width, client_height);
    switch (sl_err) {
      case SL_ERR_OK:
        return AEX_EGL_TRUE;
      case SL_ERR_INVALID_ARG:
        return set_egl_err(AEX_EGL_BAD_PARAMETER);
      case SL_ERR_OVERFLOW:
      case SL_ERR_NO_MEM:
        return set_egl_err(AEX_EGL_BAD_ALLOC);
      default:
        return set_egl_err(AEX_EGL_CONTEXT_LOST);
    }
  }

  return AEX_EGL_TRUE;
}


AEX_EGL_DECL_SPEC const char *AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(QueryString)(aex_egl_display_t dpy, aex_egl_int name) {
  return NULL;
}


AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(SwapBuffers)(aex_egl_display_t dpy, aex_egl_surface_t surface) {
  struct gl_es2_context *c = gl_es2_ctx();

  if (!c->is_egl_context_) {
    gl_es2_ctx_release(c);
    return set_egl_err(AEX_EGL_CONTEXT_LOST);
  }

  HWND hwnd = egl_context_from_gl_es2_context(c)->current_draw_hwnd_;

  struct gl_es2_framebuffer *fb = (struct gl_es2_framebuffer *)not_find(&c->framebuffer_not_, 0);
  if (!fb) {
    gl_es2_ctx_release(c);
    return set_egl_err(AEX_EGL_NOT_INITIALIZED);
  }

  int bmp_width, bmp_height;

  if (!gl_es2_framebuffer_get_dims(fb, &bmp_width, &bmp_height)) {
    gl_es2_ctx_release(c);
    return set_egl_err(AEX_EGL_NOT_INITIALIZED);
  }

  void *rgba_ptr = NULL;
  size_t rgba_stride;
  gl_es2_framebuffer_attachment_raw_ptr(&fb->color_attachment0_, &rgba_ptr, &rgba_stride);

  char s[50];
  static int snapshot = 0;
  c->is_detailed_debug_frame_ = 0;
  c->debug_frame_op_ = 0;
  if ((GetAsyncKeyState(VK_F11) & 1) && (GetAsyncKeyState(VK_CONTROL) & 1)) {
    snapshot++;
    sprintf(s, "C:\\temp\\aex-debug\\scrn%d.bmp", snapshot);
    FILE *fp = fopen(s, "wb");
    if (fp) {
      dd_write_rgba_bmp(fp, rgba_ptr, bmp_width, bmp_height, rgba_stride);
      fclose(fp);
    }
    struct gl_es2_texture *tex;
    tex = (struct gl_es2_texture *)c->texture_not_.seq_;
    if (tex) {
      do {

        switch (tex->kind_) {
          case gl_es2_texture_2d:
            sprintf(s, "C:\\temp\\aex-debug\\tex%d.bmp", (int)tex->no_.name_);
            dd_write_bmp(s, &tex->texture_2d_);
            break;
          case gl_es2_texture_cube_map:
            sprintf(s, "C:\\temp\\aex-debug\\tex%d-0.bmp", (int)tex->no_.name_);
            dd_write_bmp(s, tex->texture_cube_maps_ + 0);
            sprintf(s, "C:\\temp\\aex-debug\\tex%d-1.bmp", (int)tex->no_.name_);
            dd_write_bmp(s, tex->texture_cube_maps_ + 1);
            sprintf(s, "C:\\temp\\aex-debug\\tex%d-2.bmp", (int)tex->no_.name_);
            dd_write_bmp(s, tex->texture_cube_maps_ + 2);
            sprintf(s, "C:\\temp\\aex-debug\\tex%d-3.bmp", (int)tex->no_.name_);
            dd_write_bmp(s, tex->texture_cube_maps_ + 3);
            sprintf(s, "C:\\temp\\aex-debug\\tex%d-4.bmp", (int)tex->no_.name_);
            dd_write_bmp(s, tex->texture_cube_maps_ + 4);
            sprintf(s, "C:\\temp\\aex-debug\\tex%d-5.bmp", (int)tex->no_.name_);
            dd_write_bmp(s, tex->texture_cube_maps_ + 5);
            break;
        }

        tex = (struct gl_es2_texture *)tex->no_.next_;
      } while (tex != (struct gl_es2_texture *)c->texture_not_.seq_);
    }

    if ((GetAsyncKeyState(VK_SHIFT) & 1) && !c->is_detailed_debug_frame_) {
      c->is_detailed_debug_frame_ = 1;
      c->debug_frame_ordinal_ = snapshot;
      c->debug_frame_op_ = 0;
    }
  }

  /* Following code is pretty bad because we're allocating, and worse, we're allocating GDI objects. */
  HDC hdcwnd = GetDC(hwnd);
  HDC hdcmem = CreateCompatibleDC(hdcwnd);
  HBITMAP hbmp = CreateCompatibleBitmap(hdcwnd, bmp_width, bmp_height);

  // Select the bitmap into the off-screen DC.
  HBITMAP hprev_bmp = (HBITMAP)SelectObject(hdcmem, hbmp);

  // Set up the bitmap header
  BITMAPINFO bmi;
  ZeroMemory(&bmi, sizeof(bmi));
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = bmp_width;
  bmi.bmiHeader.biHeight = -bmp_height; // Top-down approach
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;
  bmi.bmiHeader.biCompression = BI_RGB;

  uint8_t *rowbuffer = (uint8_t *)malloc(bmp_width * 4);
  if (!rowbuffer) {
    gl_es2_ctx_release(c);
    return set_egl_err(AEX_EGL_BAD_ALLOC);
  }
  int row;
  for (row = 0; row < bmp_height; ++row) {
    size_t col;
    uint8_t *prgba = ((uint8_t *)rgba_ptr) + rgba_stride * row;
    uint8_t *pbgra = rowbuffer;
    for (col = 0; col < bmp_width; ++col) {
      pbgra[0] = prgba[2];
      pbgra[1] = prgba[1];
      pbgra[2] = prgba[0];
      pbgra[3] = prgba[3];
      pbgra += 4;
      prgba += 4;
    }
    SetDIBits(hdcmem, hbmp, bmp_height - 1 - row, 1, rowbuffer, &bmi, DIB_RGB_COLORS);
  }
  free(rowbuffer);

  // Perform the bit blit
  BitBlt(hdcwnd, 0, 0, bmp_width, bmp_height, hdcmem, 0, 0, SRCCOPY);

  // Clean up
  SelectObject(hdcmem, hprev_bmp);
  DeleteObject(hbmp);
  DeleteDC(hdcmem);
  ReleaseDC(hwnd, hdcwnd);

  /* Reconsider dimensions */
  RECT client_rect;
  GetClientRect(hwnd, &client_rect);
  int client_width, client_height;
  client_width = (int)(client_rect.right - client_rect.left);
  client_height = (int)(client_rect.bottom - client_rect.top);

  if ((bmp_width != client_width) || (bmp_height != client_height)) {
    int sl_err = GL_ES2_FUNCTION_ID(context_set_framebuffer_storage)(32, 16, 16, client_width, client_height);
    gl_es2_ctx_release(c);
    switch (sl_err) {
      case SL_ERR_OK:
        return AEX_EGL_TRUE;
      case SL_ERR_INVALID_ARG:
        return set_egl_err(AEX_EGL_BAD_PARAMETER);
      case SL_ERR_OVERFLOW:
      case SL_ERR_NO_MEM:
        return set_egl_err(AEX_EGL_BAD_ALLOC);
      default:
        return set_egl_err(AEX_EGL_CONTEXT_LOST);
    }
  }


  gl_es2_ctx_release(c);
  return AEX_EGL_TRUE;
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
    tc_set_api(TC_OPENGL_ES2_API);
    return AEX_EGL_TRUE;
  }
  return set_egl_err(AEX_EGL_BAD_PARAMETER);
}
