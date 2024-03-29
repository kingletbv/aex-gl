/* Copyright 2023-2024 Kinglet B.V.
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

/* Angle and Trigonometry Functions */
xx("float radians(float degrees);")
xx("vec2 radians(vec2 degrees);")
xx("vec3 radians(vec3 degrees);")
xx("vec4 radians(vec4 degrees);")

xx("float degrees(float radians);")
xx("vec2 degrees(vec2 radians);")
xx("vec3 degrees(vec3 radians);")
xx("vec4 degrees(vec4 radians);")

xx("float sin(float angle);")
xx("vec2 sin(vec2 angle);")
xx("vec3 sin(vec3 angle);")
xx("vec4 sin(vec4 angle);")

xx("float cos(float angle);")
xx("vec2 cos(vec2 angle);")
xx("vec3 cos(vec3 angle);")
xx("vec4 cos(vec4 angle);")

xx("float tan(float angle);")
xx("vec2 tan(vec2 angle);")
xx("vec3 tan(vec3 angle);")
xx("vec4 tan(vec4 angle);")

xx("float asin(float x);")
xx("vec2 asin(vec2 x);")
xx("vec3 asin(vec3 x);")
xx("vec4 asin(vec4 x);")

xx("float acos(float x);")
xx("vec2 acos(vec2 x);")
xx("vec3 acos(vec3 x);")
xx("vec4 acos(vec4 x);")

xx("float atan(float y, float x);")
xx("vec2 atan(vec2 y, vec2 x);")
xx("vec3 atan(vec3 y, vec3 x);")
xx("vec4 atan(vec4 y, vec4 x);")

xx("float atan(float y_over_x);")
xx("vec2 atan(vec2 y_over_x);")
xx("vec3 atan(vec3 y_over_x);")
xx("vec4 atan(vec4 y_over_x);")

/* Exponential Functions */
xx("float pow(float x, float y);")
xx("vec2 pow(vec2 x, vec2 y);")
xx("vec3 pow(vec3 x, vec3 y);")
xx("vec4 pow(vec4 x, vec4 y);")

xx("float exp(float x);")
xx("vec2 exp(vec2 x);")
xx("vec3 exp(vec3 x);")
xx("vec4 exp(vec4 x);")

xx("float log(float x);")
xx("vec2 log(vec2 x);")
xx("vec3 log(vec3 x);")
xx("vec4 log(vec4 x);")

xx("float exp2(float x);")
xx("vec2 exp2(vec2 x);")
xx("vec3 exp2(vec3 x);")
xx("vec4 exp2(vec4 x);")

xx("float log2(float x);")
xx("vec2 log2(vec2 x);")
xx("vec3 log2(vec3 x);")
xx("vec4 log2(vec4 x);")

xx("float sqrt(float x);")
xx("vec2 sqrt(vec2 x);")
xx("vec3 sqrt(vec3 x);")
xx("vec4 sqrt(vec4 x);")

xx("float inversesqrt(float x);")
xx("vec2 inversesqrt(vec2 x);")
xx("vec3 inversesqrt(vec3 x);")
xx("vec4 inversesqrt(vec4 x);")

/* Common Functions */
xx("float abs(float x);")
xx("vec2 abs(vec2 x);")
xx("vec3 abs(vec3 x);")
xx("vec4 abs(vec4 x);")

xx("float sign(float x);")
xx("vec2 sign(vec2 x);")
xx("vec3 sign(vec3 x);")
xx("vec4 sign(vec4 x);")

xx("float floor(float x);")
xx("vec2 floor(vec2 x);")
xx("vec3 floor(vec3 x);")
xx("vec4 floor(vec4 x);")

xx("float ceil(float x);")
xx("vec2 ceil(vec2 x);")
xx("vec3 ceil(vec3 x);")
xx("vec4 ceil(vec4 x);")

xx("float fract(float x);")
xx("vec2 fract(vec2 x);")
xx("vec3 fract(vec3 x);")
xx("vec4 fract(vec4 x);")

xx("float mod(float x, float y);")
xx("vec2 mod(vec2 x, float y);")
xx("vec3 mod(vec3 x, float y);")
xx("vec4 mod(vec4 x, float y);")
xx("vec2 mod(vec2 x, vec2 y);")
xx("vec3 mod(vec3 x, vec3 y);")
xx("vec4 mod(vec4 x, vec4 y);")

xx("float min(float x, float y);")
xx("vec2 min(vec2 x, float y);")
xx("vec3 min(vec3 x, float y);")
xx("vec4 min(vec4 x, float y);")
xx("vec2 min(vec2 x, vec2 y);")
xx("vec3 min(vec3 x, vec3 y);")
xx("vec4 min(vec4 x, vec4 y);")

xx("float max(float x, float y);")
xx("vec2 max(vec2 x, float y);")
xx("vec3 max(vec3 x, float y);")
xx("vec4 max(vec4 x, float y);")
xx("vec2 max(vec2 x, vec2 y);")
xx("vec3 max(vec3 x, vec3 y);")
xx("vec4 max(vec4 x, vec4 y);")

xx("float clamp(float x, float minVal, float maxVal);")
xx("vec2 clamp(vec2 x, float minVal, float maxVal);")
xx("vec3 clamp(vec3 x, float minVal, float maxVal);")
xx("vec4 clamp(vec4 x, float minVal, float maxVal);")
xx("vec2 clamp(vec2 x, vec2 minVal, vec2 maxVal);")
xx("vec3 clamp(vec3 x, vec3 minVal, vec3 maxVal);")
xx("vec4 clamp(vec4 x, vec4 minVal, vec4 maxVal);")

xx("float mix(float x, float y, float a);")
xx("vec2 mix(vec2 x, vec2 y, float a);")
xx("vec3 mix(vec3 x, vec3 y, float a);")
xx("vec4 mix(vec4 x, vec4 y, float a);")
xx("vec2 mix(vec2 x, vec2 y, vec2 a);")
xx("vec3 mix(vec3 x, vec3 y, vec3 a);")
xx("vec4 mix(vec4 x, vec4 y, vec4 a);")

xx("float step(float edge, float x);")
xx("vec2 step(float edge, vec2 x);")
xx("vec3 step(float edge, vec3 x);")
xx("vec4 step(float edge, vec4 x);")
xx("vec2 step(vec2 edge, vec2 x);")
xx("vec3 step(vec3 edge, vec3 x);")
xx("vec4 step(vec4 edge, vec4 x);")

xx("float smoothstep(float edge0, float edge1, float x);")
xx("vec2 smoothstep(vec2 edge0, vec2 edge1, vec2 x);")
xx("vec3 smoothstep(vec3 edge0, vec3 edge1, vec3 x);")
xx("vec4 smoothstep(vec4 edge0, vec4 edge1, vec4 x);")
xx("vec2 smoothstep(float edge0, float edge1, vec2 x);")
xx("vec3 smoothstep(float edge0, float edge1, vec3 x);")
xx("vec4 smoothstep(float edge0, float edge1, vec4 x);")

/* Geometric Functions */
xx("float length(float x);") /* yah ... */
xx("float length(vec2 x);")
xx("float length(vec3 x);")
xx("float length(vec4 x);")

xx("float distance(float p0, float p1);")
xx("float distance(vec2 p0, vec2 p1);")
xx("float distance(vec3 p0, vec3 p1);")
xx("float distance(vec4 p0, vec4 p1);")

xx("float dot(float x, float y);")
xx("float dot(vec2 x, vec2 y);")
xx("float dot(vec3 x, vec3 y);")
xx("float dot(vec4 x, vec4 y);")

xx("vec3 cross(vec3 x, vec3 y);")

xx("float normalize(float x);") /* yah .... */
xx("vec2 normalize(vec2 x);")
xx("vec3 normalize(vec3 x);")
xx("vec4 normalize(vec4 x);")

xx("float faceforward(float N, float I, float Nref);")
xx("vec2 faceforward(vec2 N, vec2 I, vec2 Nref);")
xx("vec3 faceforward(vec3 N, vec3 I, vec3 Nref);")
xx("vec4 faceforward(vec4 N, vec4 I, vec4 Nref);")

xx("float reflect(float I, float N);")
xx("vec2 reflect(vec2 I, vec2 N);")
xx("vec3 reflect(vec3 I, vec3 N);")
xx("vec4 reflect(vec4 I, vec4 N);")

xx("float refract(float I, float N, float eta);")
xx("vec2 refract(vec2 I, vec2 N, float eta);")
xx("vec3 refract(vec3 I, vec3 N, float eta);")
xx("vec4 refract(vec4 I, vec4 N, float eta);")

/* Matrix Functions */
xx("mat2 matrixCompMult(mat2 x, mat2 y);")
xx("mat3 matrixCompMult(mat3 x, mat3 y);")
xx("mat4 matrixCompMult(mat4 x, mat4 y);")

/* Vector Relational Functions */
xx("bvec2 lessThan(vec2 x, vec2 y);")
xx("bvec3 lessThan(vec3 x, vec3 y);")
xx("bvec4 lessThan(vec4 x, vec4 y);")
xx("bvec2 lessThan(ivec2 x, ivec2 y);")
xx("bvec3 lessThan(ivec3 x, ivec3 y);")
xx("bvec4 lessThan(ivec4 x, ivec4 y);")

xx("bvec2 lessThanEqual(vec2 x, vec2 y);")
xx("bvec3 lessThanEqual(vec3 x, vec3 y);")
xx("bvec4 lessThanEqual(vec4 x, vec4 y);")
xx("bvec2 lessThanEqual(ivec2 x, ivec2 y);")
xx("bvec3 lessThanEqual(ivec3 x, ivec3 y);")
xx("bvec4 lessThanEqual(ivec4 x, ivec4 y);")

xx("bvec2 greaterThan(vec2 x, vec2 y);")
xx("bvec3 greaterThan(vec3 x, vec3 y);")
xx("bvec4 greaterThan(vec4 x, vec4 y);")
xx("bvec2 greaterThan(ivec2 x, ivec2 y);")
xx("bvec3 greaterThan(ivec3 x, ivec3 y);")
xx("bvec4 greaterThan(ivec4 x, ivec4 y);")

xx("bvec2 greaterThanEqual(vec2 x, vec2 y);")
xx("bvec3 greaterThanEqual(vec3 x, vec3 y);")
xx("bvec4 greaterThanEqual(vec4 x, vec4 y);")
xx("bvec2 greaterThanEqual(ivec2 x, ivec2 y);")
xx("bvec3 greaterThanEqual(ivec3 x, ivec3 y);")
xx("bvec4 greaterThanEqual(ivec4 x, ivec4 y);")

xx("bvec2 equal(vec2 x, vec2 y);")
xx("bvec3 equal(vec3 x, vec3 y);")
xx("bvec4 equal(vec4 x, vec4 y);")
xx("bvec2 equal(ivec2 x, ivec2 y);")
xx("bvec3 equal(ivec3 x, ivec3 y);")
xx("bvec4 equal(ivec4 x, ivec4 y);")
xx("bvec2 equal(bvec2 x, bvec2 y);")
xx("bvec3 equal(bvec3 x, bvec3 y);")
xx("bvec4 equal(bvec4 x, bvec4 y);")

xx("bvec2 notEqual(vec2 x, vec2 y);")
xx("bvec3 notEqual(vec3 x, vec3 y);")
xx("bvec4 notEqual(vec4 x, vec4 y);")
xx("bvec2 notEqual(ivec2 x, ivec2 y);")
xx("bvec3 notEqual(ivec3 x, ivec3 y);")
xx("bvec4 notEqual(ivec4 x, ivec4 y);")
xx("bvec2 notEqual(bvec2 x, bvec2 y);")
xx("bvec3 notEqual(bvec3 x, bvec3 y);")
xx("bvec4 notEqual(bvec4 x, bvec4 y);")

xx("bool any(bvec2 x);")
xx("bool any(bvec3 x);")
xx("bool any(bvec4 x);")

xx("bool all(bvec2 x);")
xx("bool all(bvec3 x);")
xx("bool all(bvec4 x);")

xx("bvec2 not(bvec2 x);")
xx("bvec3 not(bvec3 x);")
xx("bvec4 not(bvec4 x);")

/* Texture Lookup Functions */
xx("vec4 texture2D(sampler2D sampler, vec2 coord);")
xx("vec4 texture2D(sampler2D sampler, vec2 coord, float bias);")

xx("vec4 texture2DProj(sampler2D sampler, vec3 coord);")
xx("vec4 texture2DProj(sampler2D sampler, vec3 coord, float bias);")
xx("vec4 texture2DProj(sampler2D sampler, vec4 coord);")
xx("vec4 texture2DProj(sampler2D sampler, vec4 coord, float bias);")

xx("vec4 texture2DLod(sampler2D sampler, vec2 coord, float lod);")

xx("vec4 texture2DProjLod(sampler2D sampler, vec3 coord, float lod);")
xx("vec4 texture2DProjLod(sampler2D sampler, vec4 coord, float lod);")

xx("vec4 textureCube(samplerCube sampler, vec3 coord);")
xx("vec4 textureCube(samplerCube sampler, vec3 coord, float bias);")
xx("vec4 textureCubeLod(samplerCube sampler, vec3 coord, float lod);")

