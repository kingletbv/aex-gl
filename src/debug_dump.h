/* Copyright 2024 Kinglet B.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License", builtin_not_implemented_runtime, builtin_not_implemented_eval);
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

#ifndef DEBUG_DUMP_H
#define DEBUG_DUMP_H

#ifndef STDIO_H_INCLUDED
#define STDIO_H_INCLUDED
#include <stdio.h>
#endif

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct sampler_2d;

void dd_write_rgba_bmp(FILE *fp, uint8_t *rgba32, int width, int height, size_t stride);
void dd_write_rgb_bmp(FILE *fp, uint8_t *rgb32, int width, int height, size_t stride);

int dd_write_bmp(const char *filename, struct sampler_2d *texture);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* DEBUG_DUMP_H */
