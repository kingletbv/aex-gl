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
#ifndef DEMOS_H
#define DEMOS_H

#ifndef GLES2_GL2_H_INCLUDED
#define GLES2_GL2_H_INCLUDED
#define GL_GLES_PROTOTYPES 0  /* we will define the prototypes as static function pointers by including gl_es2_aex_func_map.c */
#include <GLES2/gl2.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

int print_shader_log(FILE *fp, GLuint shader);
int print_program_log(FILE *fp, GLuint program);

size_t fpack(FILE *fp, const char *format, ...);
void write_rgba_bmp(FILE *fp, uint8_t *rgba32, int width, int height, size_t stride);

int check_for_and_print_gl_err(FILE *fp);

int run_demos(int output_width, int output_height);

int demo_mipmap_triangle(int output_width, int output_height);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DEMOS_H */
