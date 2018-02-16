#include "algebra.h"
#include <string.h>

mat4 mult2(mat4 a, mat4 b)
{
    return Mult(a, b);
}

mat4 mult3(mat4 a, mat4 b, mat4 c)
{
    return mult2(a, mult2(b, c));
}

mat4 mult4(mat4 a, mat4 b, mat4 c, mat4 d)
{
    return mult2(a, mult3(b, c, d));
}

vec3 calcNormal(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
{
    vec3 a = {x1, y1, z1};
    vec3 b = {x2, y2, z2};
    return CrossProduct(a, b);
}
