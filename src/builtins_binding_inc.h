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

/* Prior to including this file, define the xx macro:
 *   #define xx(function_prototype, runtime_function, eval_function)
 * where
 *   function_prototype, is the GLSL function prototype for the builtin.
 *   runtime_function, is the function that executes this builtin during shader execution
 *   eval_function, is the function that executes this builtin during compile time for constant expression evaluation
 */

/* Angle and Trigonometry Functions */
xx("float radians(float degrees);", builtin_radians_f_runtime, builtin_radians_f_eval)
xx("vec2 radians(vec2 degrees);", builtin_radians_v2_runtime, builtin_radians_v2_eval)
xx("vec3 radians(vec3 degrees);", builtin_radians_v3_runtime, builtin_radians_v3_eval)
xx("vec4 radians(vec4 degrees);", builtin_radians_v4_runtime, builtin_radians_v4_eval)

xx("float degrees(float radians);", builtin_degrees_f_runtime, builtin_degrees_f_eval)
xx("vec2 degrees(vec2 radians);", builtin_degrees_v2_runtime, builtin_degrees_v2_eval)
xx("vec3 degrees(vec3 radians);", builtin_degrees_v3_runtime, builtin_degrees_v3_eval)
xx("vec4 degrees(vec4 radians);", builtin_degrees_v4_runtime, builtin_degrees_v4_eval)

xx("float sin(float angle);", builtin_sin_f_runtime, builtin_sin_f_eval)
xx("vec2 sin(vec2 angle);", builtin_sin_v2_runtime, builtin_sin_v2_eval)
xx("vec3 sin(vec3 angle);", builtin_sin_v3_runtime, builtin_sin_v3_eval)
xx("vec4 sin(vec4 angle);", builtin_sin_v4_runtime, builtin_sin_v4_eval)

xx("float cos(float angle);", builtin_cos_f_runtime, builtin_cos_f_eval)
xx("vec2 cos(vec2 angle);", builtin_cos_v2_runtime, builtin_cos_v2_eval)
xx("vec3 cos(vec3 angle);", builtin_cos_v3_runtime, builtin_cos_v3_eval)
xx("vec4 cos(vec4 angle);", builtin_cos_v4_runtime, builtin_cos_v4_eval)

xx("float tan(float angle);", builtin_tan_f_runtime, builtin_tan_f_eval)
xx("vec2 tan(vec2 angle);", builtin_tan_v2_runtime, builtin_tan_v2_eval)
xx("vec3 tan(vec3 angle);", builtin_tan_v3_runtime, builtin_tan_v3_eval)
xx("vec4 tan(vec4 angle);", builtin_tan_v4_runtime, builtin_tan_v4_eval)

xx("float asin(float x);", builtin_asin_f_runtime, builtin_asin_f_eval)
xx("vec2 asin(vec2 x);", builtin_asin_v2_runtime, builtin_asin_v2_eval)
xx("vec3 asin(vec3 x);", builtin_asin_v3_runtime, builtin_asin_v3_eval)
xx("vec4 asin(vec4 x);", builtin_asin_v4_runtime, builtin_asin_v4_eval)

xx("float acos(float x);", builtin_acos_f_runtime, builtin_acos_f_eval)
xx("vec2 acos(vec2 x);", builtin_acos_v2_runtime, builtin_acos_v2_eval)
xx("vec3 acos(vec3 x);", builtin_acos_v3_runtime, builtin_acos_v3_eval)
xx("vec4 acos(vec4 x);", builtin_acos_v4_runtime, builtin_acos_v4_eval)

xx("float atan(float y, float x);", builtin_atan_ff_runtime, builtin_atan_ff_eval)
xx("vec2 atan(vec2 y, vec2 x);", builtin_atan_v2v2_runtime, builtin_atan_v2v2_eval)
xx("vec3 atan(vec3 y, vec3 x);", builtin_atan_v3v3_runtime, builtin_atan_v3v3_eval)
xx("vec4 atan(vec4 y, vec4 x);", builtin_atan_v4v4_runtime, builtin_atan_v4v4_eval)

xx("float atan(float y_over_x);", builtin_atan_f_runtime, builtin_atan_f_eval)
xx("vec2 atan(vec2 y_over_x);", builtin_atan_v2_runtime, builtin_atan_v2_eval)
xx("vec3 atan(vec3 y_over_x);", builtin_atan_v3_runtime, builtin_atan_v3_eval)
xx("vec4 atan(vec4 y_over_x);", builtin_atan_v4_runtime, builtin_atan_v4_eval)

/* Exponential Functions */
xx("float pow(float x, float y);", builtin_pow_ff_runtime, builtin_pow_ff_eval)
xx("vec2 pow(vec2 x, vec2 y);", builtin_pow_v2v2_runtime, builtin_pow_v2v2_eval)
xx("vec3 pow(vec3 x, vec3 y);", builtin_pow_v3v3_runtime, builtin_pow_v3v3_eval)
xx("vec4 pow(vec4 x, vec4 y);", builtin_pow_v4v4_runtime, builtin_pow_v4v4_eval)

xx("float exp(float x);", builtin_exp_f_runtime, builtin_exp_f_eval)
xx("vec2 exp(vec2 x);", builtin_exp_v2_runtime, builtin_exp_v2_eval)
xx("vec3 exp(vec3 x);", builtin_exp_v3_runtime, builtin_exp_v3_eval)
xx("vec4 exp(vec4 x);", builtin_exp_v4_runtime, builtin_exp_v4_eval)

xx("float log(float x);", builtin_log_f_runtime, builtin_log_f_eval)
xx("vec2 log(vec2 x);", builtin_log_v2_runtime, builtin_log_v2_eval)
xx("vec3 log(vec3 x);", builtin_log_v3_runtime, builtin_log_v3_eval)
xx("vec4 log(vec4 x);", builtin_log_v4_runtime, builtin_log_v4_eval)

xx("float exp2(float x);", builtin_exp2_f_runtime, builtin_exp2_f_eval)
xx("vec2 exp2(vec2 x);", builtin_exp2_v2_runtime, builtin_exp2_v2_eval)
xx("vec3 exp2(vec3 x);", builtin_exp2_v3_runtime, builtin_exp2_v3_eval)
xx("vec4 exp2(vec4 x);", builtin_exp2_v4_runtime, builtin_exp2_v4_eval)

xx("float log2(float x);", builtin_log2_f_runtime, builtin_log2_f_eval)
xx("vec2 log2(vec2 x);", builtin_log2_v2_runtime, builtin_log2_v2_eval)
xx("vec3 log2(vec3 x);", builtin_log2_v3_runtime, builtin_log2_v3_eval)
xx("vec4 log2(vec4 x);", builtin_log2_v4_runtime, builtin_log2_v4_eval)

xx("float sqrt(float x);", builtin_sqrt_f_runtime, builtin_sqrt_f_eval)
xx("vec2 sqrt(vec2 x);", builtin_sqrt_v2_runtime, builtin_sqrt_v2_eval)
xx("vec3 sqrt(vec3 x);", builtin_sqrt_v3_runtime, builtin_sqrt_v3_eval)
xx("vec4 sqrt(vec4 x);", builtin_sqrt_v4_runtime, builtin_sqrt_v4_eval)

xx("float inversesqrt(float x);", builtin_inversesqrt_f_runtime, builtin_inversesqrt_f_eval)
xx("vec2 inversesqrt(vec2 x);", builtin_inversesqrt_v2_runtime, builtin_inversesqrt_v2_eval)
xx("vec3 inversesqrt(vec3 x);", builtin_inversesqrt_v3_runtime, builtin_inversesqrt_v3_eval)
xx("vec4 inversesqrt(vec4 x);", builtin_inversesqrt_v4_runtime, builtin_inversesqrt_v4_eval)

/* Common Functions */
xx("float abs(float x);", builtin_abs_f_runtime, builtin_abs_f_eval)
xx("vec2 abs(vec2 x);", builtin_abs_v2_runtime, builtin_abs_v2_eval)
xx("vec3 abs(vec3 x);", builtin_abs_v3_runtime, builtin_abs_v3_eval)
xx("vec4 abs(vec4 x);", builtin_abs_v4_runtime, builtin_abs_v4_eval)

xx("float sign(float x);", builtin_sign_f_runtime, builtin_sign_f_eval)
xx("vec2 sign(vec2 x);", builtin_sign_v2_runtime, builtin_sign_v2_eval)
xx("vec3 sign(vec3 x);", builtin_sign_v3_runtime, builtin_sign_v3_eval)
xx("vec4 sign(vec4 x);", builtin_sign_v4_runtime, builtin_sign_v4_eval)

xx("float floor(float x);", builtin_floor_f_runtime, builtin_floor_f_eval)
xx("vec2 floor(vec2 x);", builtin_floor_v2_runtime, builtin_floor_v2_eval)
xx("vec3 floor(vec3 x);", builtin_floor_v3_runtime, builtin_floor_v3_eval)
xx("vec4 floor(vec4 x);", builtin_floor_v4_runtime, builtin_floor_v4_eval)

xx("float ceil(float x);", builtin_ceil_f_runtime, builtin_ceil_f_eval)
xx("vec2 ceil(vec2 x);", builtin_ceil_v2_runtime, builtin_ceil_v2_eval)
xx("vec3 ceil(vec3 x);", builtin_ceil_v3_runtime, builtin_ceil_v3_eval)
xx("vec4 ceil(vec4 x);", builtin_ceil_v4_runtime, builtin_ceil_v4_eval)

xx("float fract(float x);", builtin_fract_f_runtime, builtin_fract_f_eval)
xx("vec2 fract(vec2 x);", builtin_fract_v2_runtime, builtin_fract_v2_eval)
xx("vec3 fract(vec3 x);", builtin_fract_v3_runtime, builtin_fract_v3_eval)
xx("vec4 fract(vec4 x);", builtin_fract_v4_runtime, builtin_fract_v4_eval)

xx("float mod(float x, float y);", builtin_mod_ff_runtime, builtin_mod_ff_eval)
xx("vec2 mod(vec2 x, vec2 y);", builtin_mod_v2v2_runtime, builtin_mod_v2v2_eval)
xx("vec3 mod(vec3 x, vec3 y);", builtin_mod_v3v3_runtime, builtin_mod_v3v3_eval)
xx("vec4 mod(vec4 x, vec4 y);", builtin_mod_v4v4_runtime, builtin_mod_v4v4_eval)
xx("vec2 mod(vec2 x, float y);", builtin_mod_v2f_runtime, builtin_mod_v2f_eval)
xx("vec3 mod(vec3 x, float y);", builtin_mod_v3f_runtime, builtin_mod_v3f_eval)
xx("vec4 mod(vec4 x, float y);", builtin_mod_v4f_runtime, builtin_mod_v4f_eval)

xx("float min(float x, float y);", builtin_min_ff_runtime, builtin_min_ff_eval)
xx("vec2 min(vec2 x, vec2 y);", builtin_min_v2v2_runtime, builtin_min_v2v2_eval)
xx("vec3 min(vec3 x, vec3 y);", builtin_min_v3v3_runtime, builtin_min_v3v3_eval)
xx("vec4 min(vec4 x, vec4 y);", builtin_min_v4v4_runtime, builtin_min_v4v4_eval)
xx("vec2 min(vec2 x, float y);", builtin_min_v2f_runtime, builtin_min_v2f_eval)
xx("vec3 min(vec3 x, float y);", builtin_min_v3f_runtime, builtin_min_v3f_eval)
xx("vec4 min(vec4 x, float y);", builtin_min_v4f_runtime, builtin_min_v4f_eval)

xx("float max(float x, float y);", builtin_max_ff_runtime, builtin_max_ff_eval)
xx("vec2 max(vec2 x, vec2 y);", builtin_max_v2v2_runtime, builtin_max_v2v2_eval)
xx("vec3 max(vec3 x, vec3 y);", builtin_max_v3v3_runtime, builtin_max_v3v3_eval)
xx("vec4 max(vec4 x, vec4 y);", builtin_max_v4v4_runtime, builtin_max_v4v4_eval)
xx("vec2 max(vec2 x, float y);", builtin_max_v2f_runtime, builtin_max_v2f_eval)
xx("vec3 max(vec3 x, float y);", builtin_max_v3f_runtime, builtin_max_v3f_eval)
xx("vec4 max(vec4 x, float y);", builtin_max_v4f_runtime, builtin_max_v4f_eval)

xx("float clamp(float x, float a, float b);", builtin_clamp_fff_runtime, builtin_clamp_fff_eval)
xx("vec2 clamp(vec2 x, vec2 a, vec2 b);", builtin_clamp_v2v2v2_runtime, builtin_clamp_v2v2v2_eval)
xx("vec3 clamp(vec3 x, vec3 a, vec3 b);", builtin_clamp_v3v3v3_runtime, builtin_clamp_v3v3v3_eval)
xx("vec4 clamp(vec4 x, vec4 a, vec4 b);", builtin_clamp_v4v4v4_runtime, builtin_clamp_v4v4v4_eval)
xx("vec2 clamp(vec2 x, float a, float b);", builtin_clamp_v2ff_runtime, builtin_clamp_v2ff_eval)
xx("vec3 clamp(vec3 x, float a, float b);", builtin_clamp_v3ff_runtime, builtin_clamp_v3ff_eval)
xx("vec4 clamp(vec4 x, float a, float b);", builtin_clamp_v4ff_runtime, builtin_clamp_v4ff_eval)

xx("float mix(float x, float a, float b);", builtin_mix_fff_runtime, builtin_mix_fff_eval)
xx("vec2 mix(vec2 x, vec2 a, vec2 b);", builtin_mix_v2v2v2_runtime, builtin_mix_v2v2v2_eval)
xx("vec3 mix(vec3 x, vec3 a, vec3 b);", builtin_mix_v3v3v3_runtime, builtin_mix_v3v3v3_eval)
xx("vec4 mix(vec4 x, vec4 a, vec4 b);", builtin_mix_v4v4v4_runtime, builtin_mix_v4v4v4_eval)
xx("vec2 mix(vec2 x, vec2 a, float b);", builtin_mix_v2v2f_runtime, builtin_mix_v2v2f_eval)
xx("vec3 mix(vec3 x, vec3 a, float b);", builtin_mix_v3v3f_runtime, builtin_mix_v3v3f_eval)
xx("vec4 mix(vec4 x, vec4 a, float b);", builtin_mix_v4v4f_runtime, builtin_mix_v4v4f_eval)

xx("float step(float x, float y);", builtin_step_ff_runtime, builtin_step_ff_eval)
xx("vec2 step(vec2 x, vec2 y);", builtin_step_v2v2_runtime, builtin_step_v2v2_eval)
xx("vec3 step(vec3 x, vec3 y);", builtin_step_v3v3_runtime, builtin_step_v3v3_eval)
xx("vec4 step(vec4 x, vec4 y);", builtin_step_v4v4_runtime, builtin_step_v4v4_eval)
xx("vec2 step(float x, vec2 y);", builtin_step_fv2_runtime, builtin_step_fv2_eval)
xx("vec3 step(float x, vec3 y);", builtin_step_fv3_runtime, builtin_step_fv3_eval)
xx("vec4 step(float x, vec4 y);", builtin_step_fv4_runtime, builtin_step_fv4_eval)

xx("float smoothstep(float x, float a, float b);", builtin_smoothstep_fff_runtime, builtin_smoothstep_fff_eval)
xx("vec2 smoothstep(vec2 x, vec2 a, vec2 b);", builtin_smoothstep_v2v2v2_runtime, builtin_smoothstep_v2v2v2_eval)
xx("vec3 smoothstep(vec3 x, vec3 a, vec3 b);", builtin_smoothstep_v3v3v3_runtime, builtin_smoothstep_v3v3v3_eval)
xx("vec4 smoothstep(vec4 x, vec4 a, vec4 b);", builtin_smoothstep_v4v4v4_runtime, builtin_smoothstep_v4v4v4_eval)
xx("vec2 smoothstep(float a, float b, vec2 x);", builtin_smoothstep_ffv2_runtime, builtin_smoothstep_ffv2_eval)
xx("vec3 smoothstep(float a, float b, vec3 x);", builtin_smoothstep_ffv3_runtime, builtin_smoothstep_ffv3_eval)
xx("vec4 smoothstep(float a, float b, vec4 x);", builtin_smoothstep_ffv4_runtime, builtin_smoothstep_ffv4_eval)

/* Geometric Functions */
xx("float length(float x);", builtin_length_f_runtime, builtin_length_f_eval) /* yah ... */
xx("float length(vec2 x);", builtin_length_v2_runtime, builtin_length_v2_eval)
xx("float length(vec3 x);", builtin_length_v3_runtime, builtin_length_v3_eval)
xx("float length(vec4 x);", builtin_length_v4_runtime, builtin_length_v4_eval)

xx("float distance(float p0, float p1);", builtin_distance_ff_runtime, builtin_distance_ff_eval)
xx("float distance(vec2 p0, vec2 p1);", builtin_distance_v2v2_runtime, builtin_distance_v2v2_eval)
xx("float distance(vec3 p0, vec3 p1);", builtin_distance_v3v3_runtime, builtin_distance_v3v3_eval)
xx("float distance(vec4 p0, vec4 p1);", builtin_distance_v4v4_runtime, builtin_distance_v4v4_eval)

xx("float dot(float x, float y);", builtin_dot_ff_runtime, builtin_dot_ff_eval)
xx("float dot(vec2 x, vec2 y);", builtin_dot_v2v2_runtime, builtin_dot_v2v2_eval)
xx("float dot(vec3 x, vec3 y);", builtin_dot_v3v3_runtime, builtin_dot_v3v3_eval)
xx("float dot(vec4 x, vec4 y);", builtin_dot_v4v4_runtime, builtin_dot_v4v4_eval)

xx("vec3 cross(vec3 x, vec3 y);", builtin_cross_v3v3_runtime, builtin_cross_v3v3_eval)

xx("float normalize(float x);", builtin_normalize_f_runtime, builtin_normalize_f_eval) /* yah .... */
xx("vec2 normalize(vec2 x);", builtin_normalize_v2_runtime, builtin_normalize_v2_eval)
xx("vec3 normalize(vec3 x);", builtin_normalize_v3_runtime, builtin_normalize_v3_eval)
xx("vec4 normalize(vec4 x);", builtin_normalize_v4_runtime, builtin_normalize_v4_eval)

xx("float faceforward(float N, float I, float Nref);", builtin_faceforward_fff_runtime, builtin_faceforward_fff_eval)
xx("vec2 faceforward(vec2 N, vec2 I, vec2 Nref);", builtin_faceforward_v2v2v2_runtime, builtin_faceforward_v2v2v2_eval)
xx("vec3 faceforward(vec3 N, vec3 I, vec3 Nref);", builtin_faceforward_v3v3v3_runtime, builtin_faceforward_v3v3v3_eval)
xx("vec4 faceforward(vec4 N, vec4 I, vec4 Nref);", builtin_faceforward_v4v4v4_runtime, builtin_faceforward_v4v4v4_eval)

xx("float reflect(float I, float N);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("vec2 reflect(vec2 I, vec2 N);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("vec3 reflect(vec3 I, vec3 N);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("vec4 reflect(vec4 I, vec4 N);", builtin_not_implemented_runtime, builtin_not_implemented_eval)

xx("float refract(float I, float N, float eta);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("vec2 refract(vec2 I, vec2 N, float eta);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("vec3 refract(vec3 I, vec3 N, float eta);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("vec4 refract(vec4 I, vec4 N, float eta);", builtin_not_implemented_runtime, builtin_not_implemented_eval)

/* Matrix Functions */
xx("mat2 matrixCompMult(mat2 x, mat2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("mat3 matrixCompMult(mat3 x, mat3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("mat4 matrixCompMult(mat4 x, mat4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)

/* Vector Relational Functions */
xx("bvec2 lessThan(vec2 x, vec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 lessThan(vec3 x, vec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 lessThan(vec4 x, vec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec2 lessThan(ivec2 x, ivec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 lessThan(ivec3 x, ivec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 lessThan(ivec4 x, ivec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)

xx("bvec2 lessThanEqual(vec2 x, vec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 lessThanEqual(vec3 x, vec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 lessThanEqual(vec4 x, vec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec2 lessThanEqual(ivec2 x, ivec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 lessThanEqual(ivec3 x, ivec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 lessThanEqual(ivec4 x, ivec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)

xx("bvec2 greaterThan(vec2 x, vec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 greaterThan(vec3 x, vec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 greaterThan(vec4 x, vec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec2 greaterThan(ivec2 x, ivec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 greaterThan(ivec3 x, ivec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 greaterThan(ivec4 x, ivec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)

xx("bvec2 greaterThanEqual(vec2 x, vec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 greaterThanEqual(vec3 x, vec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 greaterThanEqual(vec4 x, vec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec2 greaterThanEqual(ivec2 x, ivec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 greaterThanEqual(ivec3 x, ivec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 greaterThanEqual(ivec4 x, ivec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)

xx("bvec2 equal(vec2 x, vec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 equal(vec3 x, vec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 equal(vec4 x, vec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec2 equal(ivec2 x, ivec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 equal(ivec3 x, ivec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 equal(ivec4 x, ivec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec2 equal(bvec2 x, bvec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 equal(bvec3 x, bvec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 equal(bvec4 x, bvec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)

xx("bvec2 notEqual(vec2 x, vec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 notEqual(vec3 x, vec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 notEqual(vec4 x, vec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec2 notEqual(ivec2 x, ivec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 notEqual(ivec3 x, ivec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 notEqual(ivec4 x, ivec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec2 notEqual(bvec2 x, bvec2 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 notEqual(bvec3 x, bvec3 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 notEqual(bvec4 x, bvec4 y);", builtin_not_implemented_runtime, builtin_not_implemented_eval)

xx("bool any(bvec2 x);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bool any(bvec3 x);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bool any(bvec4 x);", builtin_not_implemented_runtime, builtin_not_implemented_eval)

xx("bool all(bvec2 x);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bool all(bvec3 x);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bool all(bvec4 x);", builtin_not_implemented_runtime, builtin_not_implemented_eval)

xx("bvec2 not(bvec2 x);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec3 not(bvec3 x);", builtin_not_implemented_runtime, builtin_not_implemented_eval)
xx("bvec4 not(bvec4 x);", builtin_not_implemented_runtime, builtin_not_implemented_eval)

/* Texture Lookup Functions */
xx("vec4 texture2D(sampler2D sampler, vec2 coord);",                   builtin_texture2D_runtime, NULL)
xx("vec4 texture2D(sampler2D sampler, vec2 coord, float bias);",       builtin_texture2D_bias_runtime, NULL)

xx("vec4 texture2DProj(sampler2D sampler, vec3 coord);",               builtin_texture2DProj_v3_runtime, NULL)
xx("vec4 texture2DProj(sampler2D sampler, vec3 coord, float bias);",   builtin_texture2DProj_v3_bias_runtime, NULL)
xx("vec4 texture2DProj(sampler2D sampler, vec4 coord);",               builtin_texture2DProj_v4_runtime, NULL)
xx("vec4 texture2DProj(sampler2D sampler, vec4 coord, float bias);",   builtin_texture2DProj_v4_bias_runtime, NULL)

xx("vec4 texture2DLod(sampler2D sampler, vec2 coord, float lod);",     builtin_texture2DLod_v2_runtime, NULL)

xx("vec4 texture2DProjLod(sampler2D sampler, vec3 coord, float lod);", builtin_texture2DProjLod_v3_runtime, NULL)
xx("vec4 texture2DProjLod(sampler2D sampler, vec4 coord, float lod);", builtin_texture2DProjLod_v4_runtime, NULL)

xx("vec4 textureCube(samplerCube sampler, vec3 coord);", builtin_textureCube_v3_runtime, NULL)
xx("vec4 textureCube(samplerCube sampler, vec3 coord, float bias);", builtin_textureCube_v3_bias_runtime, NULL)
xx("vec4 textureCubeLod(samplerCube sampler, vec3 coord, float lod);", builtin_textureCubeLod_v3_runtime, NULL)

