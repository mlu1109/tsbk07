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