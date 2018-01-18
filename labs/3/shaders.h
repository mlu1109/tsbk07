#ifndef SHADERS_H
#define SHADERS_H

#include "VectorUtils3.h"

void shaderUpload(GLuint program, mat4 *mvp, mat4 *transform, GLuint texUnit, GLfloat specularExponent);

#endif