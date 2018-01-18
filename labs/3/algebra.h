#ifndef ALGEBRA_H
#define ALGEBRA_H

#include <math.h>
#include "VectorUtils3.h"

mat4 mult2(mat4 a, mat4 b);
mat4 mult3(mat4 a, mat4 b, mat4 c);
mat4 mult4(mat4 a, mat4 b, mat4 c, mat4 d);
vec3 v3sub(vec3 a, vec3 b);
vec3 v3add(vec3 a, vec3 b);
vec3 v3scale(vec3 v, float f);
#endif

