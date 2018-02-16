#ifndef SHADERS_H
#define SHADERS_H

#include "VectorUtils3.h"

void shaderUpload(GLuint program, mat4 *model, mat4 *view, vec3 *cameraPosition, GLuint texUnit, GLfloat specularExponent);
void shaderUpload2Textures(GLuint program, mat4 *model, mat4 *view, vec3 *cameraPosition, GLuint texUnit, GLuint texUnit2, GLfloat specularExponent);

#endif