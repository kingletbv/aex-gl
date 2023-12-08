#ifndef GL_ES2_IMPL_TYPES_H
#define GL_ES2_IMPL_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef uint8_t gl_es2_ubyte;
typedef unsigned int gl_es2_enum;
typedef unsigned int gl_es2_uint;
typedef char gl_es2_char;
typedef float gl_es2_float;
#ifdef _WIN64
typedef int64_t gl_es2_sizeiptr;
typedef int64_t gl_es2_intptr;
#else
typedef long gl_es2_sizeiptr;
typedef long gl_es2_intptr;
#endif
typedef unsigned int gl_es2_bitfield;
typedef int gl_es2_int;
typedef unsigned char gl_es2_boolean;
typedef int gl_es2_sizei;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GL_ES2_IMPL_TYPES_H */
