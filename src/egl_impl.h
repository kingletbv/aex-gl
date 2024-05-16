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
#ifndef EGL_IMPL_H
#define EGL_IMPL_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Declaration specifier */
#define AEX_EGL_DECL_SPEC 

/* Declarator attribute */
#define AEX_EGL_DECLARATOR_ATTRIB

/* Function identifier decoration */
#define AEX_EGL_FUNCTION_ID(id) aex_egl_##id

#ifdef EGL_IMPL_C
#ifndef _WINDOWS_
#error egl_impl.c implementation file should define EGL_IMPL_C *and* include Windows.h
#endif

typedef struct aex_egl_config {
  int unused_;
} *aex_egl_config_t;
  
typedef struct aex_egl_display {
  HDC hdc;
  int initialized_:1;
  struct aex_egl_config display_config_;
} *aex_egl_display_t;

typedef struct aex_egl_context {
  int unused_;
} *aex_egl_context_t;

typedef struct aex_egl_surface {
  int unused_;
  HWND hwnd_;
} *aex_egl_surface_t;

typedef int32_t aex_egl_int;
typedef unsigned int aex_egl_boolean_t;
typedef unsigned int aex_egl_enum_t;

#else
typedef void *aex_egl_display_t;
typedef void *aex_egl_config_t;
typedef void *aex_egl_context_t;
typedef void *aex_egl_surface_t;
typedef int32_t aex_egl_int;
typedef unsigned int aex_egl_boolean_t;
typedef unsigned int aex_egl_enum_t;
#endif

typedef void (*aex_egl_generic_function_ptr_t)(void);

#define AEX_EGL_NO_CONTEXT (aex_egl_context_t)(0);
#define AEX_EGL_NO_SURFACE (aex_egl_surface_t)(0);

/* Has Windows.h been included? (this is different from "are we compiling on a windows platform" which would be #ifdef _WIN32) */
#ifdef _WINDOWS_ 
typedef HWND aex_egl_native_window_t;
typedef HBITMAP aex_egl_native_pixmap_t;
typedef HDC aex_egl_native_display_t;
#else
typedef void *aex_egl_native_window_t;
typedef void *aex_egl_native_pixmap_t;
typedef void *aex_egl_native_display_t;
#endif

#define AEX_EGL_FALSE 0
#define AEX_EGL_TRUE 1

#define AEX_EGL_NONE                0x3038
#define AEX_EGL_MATCH_NATIVE_PIXMAP 0x3041

#define AEX_EGL_CONTEXT_MAJOR_VERSION                      0x3098
#define AEX_EGL_CONTEXT_MINOR_VERSION                      0x30FB
#define AEX_EGL_CONTEXT_OPENGL_PROFILE_MASK                0x30FD
#define AEX_EGL_CONTEXT_OPENGL_DEBUG                       0x31B0
#define AEX_EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE          0x31B1
#define AEX_EGL_CONTEXT_OPENGL_ROBUST_ACCESS               0x31B2
#define AEX_EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY 0x31BD

#define AEX_EGL_ALPHA_SIZE              0x3021
#define AEX_EGL_ALPHA_MASK_SIZE         0x303E
#define AEX_EGL_BIND_TO_TEXTURE_RGB     0x3039
#define AEX_EGL_BIND_TO_TEXTURE_RGBA    0x303A
#define AEX_EGL_BLUE_SIZE               0x3022
#define AEX_EGL_BUFFER_SIZE             0x3020
#define AEX_EGL_COLOR_BUFFER_TYPE       0x303F
#define AEX_EGL_CONFIG_CAVEAT           0x3027
#define AEX_EGL_CONFIG_ID               0x3028
#define AEX_EGL_CONFORMANT              0x3042
#define AEX_EGL_DEPTH_SIZE              0x3025
#define AEX_EGL_GREEN_SIZE              0x3023
#define AEX_EGL_LEVEL                   0x3029
#define AEX_EGL_LUMINANCE_SIZE          0x303D
#define AEX_EGL_MAX_PBUFFER_WIDTH       0x302C
#define AEX_EGL_MAX_PBUFFER_HEIGHT      0x302A
#define AEX_EGL_MAX_PBUFFER_PIXELS      0x302B
#define AEX_EGL_MAX_SWAP_INTERVAL       0x303C
#define AEX_EGL_MIN_SWAP_INTERVAL       0x303B
#define AEX_EGL_NATIVE_RENDERABLE       0x302D
#define AEX_EGL_NATIVE_VISUAL_ID        0x302E
#define AEX_EGL_NATIVE_VISUAL_TYPE      0x302F
#define AEX_EGL_RED_SIZE                0x3024
#define AEX_EGL_RENDERABLE_TYPE         0x3040
#define AEX_EGL_SAMPLE_BUFFERS          0x3032
#define AEX_EGL_SAMPLES                 0x3031
#define AEX_EGL_STENCIL_SIZE            0x3026
#define AEX_EGL_SURFACE_TYPE            0x3033
#define AEX_EGL_TRANSPARENT_TYPE        0x3034
#define AEX_EGL_TRANSPARENT_RED_VALUE   0x3037
#define AEX_EGL_TRANSPARENT_GREEN_VALUE 0x3036
#define AEX_EGL_TRANSPARENT_BLUE_VALUE  0x3035

#define AEX_EGL_RGB_BUFFER 0x308E

#define AEX_EGL_PBUFFER_BIT                 0x0001
#define AEX_EGL_PIXMAP_BIT                  0x0002
#define AEX_EGL_WINDOW_BIT                  0x0004
#define AEX_EGL_VG_COLORSPACE_LINEAR_BIT    0x0020
#define AEX_EGL_VG_ALPHA_FORMAT_PRE_BIT     0x0040
#define AEX_EGL_MULTISAMPLE_RESOLVE_BOX_BIT 0x0200
#define AEX_EGL_SWAP_BEHAVIOR_PRESERVED_BIT 0x0400

#define AEX_EGL_OPENGL_ES_BIT  0x0001
#define AEX_EGL_OPENVG_BIT     0x0002
#define AEX_EGL_OPENGL_ES2_BIT 0x0004
#define AEX_EGL_OPENGL_BIT     0x0008

#define AEX_EGL_OPENGL_ES_API 0x30A0
#define AEX_EGL_OPENVG_API    0x30A1
#define AEX_EGL_OPENGL_API    0x30A2

#define AEX_EGL_SUCCESS                       0x3000
#define AEX_EGL_NOT_INITIALIZED               0x3001
#define AEX_EGL_BAD_ACCESS                    0x3002
#define AEX_EGL_BAD_ALLOC                     0x3003
#define AEX_EGL_BAD_ATTRIBUTE                 0x3004
#define AEX_EGL_BAD_CONFIG                    0x3005
#define AEX_EGL_BAD_CONTEXT                   0x3006
#define AEX_EGL_BAD_CURRENT_SURFACE           0x3007
#define AEX_EGL_BAD_DISPLAY                   0x3008
#define AEX_EGL_BAD_MATCH                     0x3009
#define AEX_EGL_BAD_NATIVE_PIXMAP             0x300A
#define AEX_EGL_BAD_NATIVE_WINDOW             0x300B
#define AEX_EGL_BAD_PARAMETER                 0x300C
#define AEX_EGL_BAD_SURFACE                   0x300D
#define AEX_EGL_CONTEXT_LOST                  0x300E

AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(BindAPI)(aex_egl_enum_t api);
AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(ChooseConfig)(aex_egl_display_t dpy, const aex_egl_int *attrib_list, aex_egl_config_t *configs, aex_egl_int config_size, aex_egl_int *num_config);
AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(GetConfigAttrib)(aex_egl_display_t dpy, aex_egl_config_t config, aex_egl_int attribute, aex_egl_int *value);
AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(GetConfigs)(aex_egl_display_t dpy, aex_egl_config_t *configs, aex_egl_int config_size, aex_egl_int *num_config);
AEX_EGL_DECL_SPEC aex_egl_context_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(CreateContext)(aex_egl_display_t dpy, aex_egl_config_t config, aex_egl_context_t share_context, const aex_egl_int *attrib_list);
AEX_EGL_DECL_SPEC aex_egl_surface_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(CreatePbufferSurface)(aex_egl_display_t dpy, aex_egl_config_t config, const aex_egl_int *attrib_list);
AEX_EGL_DECL_SPEC aex_egl_surface_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(CreateWindowSurface)(aex_egl_display_t dpy, aex_egl_config_t config, aex_egl_native_window_t win, const aex_egl_int *attrib_list);
AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(DestroyContext)(aex_egl_display_t dpy, aex_egl_context_t ctx);
AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(DestroySurface)(aex_egl_display_t dpy, aex_egl_surface_t surface);
AEX_EGL_DECL_SPEC aex_egl_display_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(GetDisplay)(aex_egl_native_display_t display_id);
AEX_EGL_DECL_SPEC aex_egl_int       AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(GetError)(void);
AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(Initialize)(aex_egl_display_t dpy, aex_egl_int *major, aex_egl_int *minor);
AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(MakeCurrent)(aex_egl_display_t dpy, aex_egl_surface_t draw, aex_egl_surface_t read, aex_egl_context_t ctx);
AEX_EGL_DECL_SPEC const char *AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(QueryString)(aex_egl_display_t dpy, aex_egl_int name);
AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(SwapBuffers)(aex_egl_display_t dpy, aex_egl_surface_t surface);
AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(SwapInterval)(aex_egl_display_t dpy, aex_egl_int interval);
AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(Terminate)(aex_egl_display_t dpy);
AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(WaitGL)(void);
AEX_EGL_DECL_SPEC aex_egl_boolean_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(WaitNative)(aex_egl_int engine);

// Note that for functions like eglGetProcAddress, which returns a function pointer, special handling may be needed:
AEX_EGL_DECL_SPEC aex_egl_generic_function_ptr_t AEX_EGL_DECLARATOR_ATTRIB AEX_EGL_FUNCTION_ID(GetProcAddress)(const char *procname);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* EGL_IMPL_H */
