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
#ifndef BLITTER_H
#define BLITTER_H

#ifdef __cplusplus
extern "C" {
#endif

enum blitter_data_type {
  blit_unsigned_byte,
  blit_unsigned_short_565,
  blit_unsigned_short_4444,
  blit_unsigned_short_5551
};

enum blitter_components {
  blit_alpha,
  blit_luminance,
  blit_luminance_alpha,
  blit_rgb,
  blit_rgba
};

void blitter_blit(void *dst, const void *src, size_t dst_stride, size_t src_stride, 
                  size_t width, size_t height, enum blitter_components format, enum blitter_data_type src_type);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* BLITTER_H */
