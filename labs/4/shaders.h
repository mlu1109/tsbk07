#ifndef SHADERS_H
#define SHADERS_H

#include "VectorUtils3.h"

void shaderLight(GLuint program, GLfloat *lightPosition, GLfloat *lightColor, GLint *isDirectional, const GLfloat *k_a, const GLfloat *k_d, const GLfloat *k_s);
void shaderUpload(GLuint program, mat4 *model, mat4 *view, vec3 *cameraPosition, GLuint texUnit, GLfloat specularExponent);
void shaderUpload2Textures(GLuint program, mat4 *model, mat4 *view, vec3 *cameraPosition, GLuint texUnit, GLuint texUnit2, GLfloat specularExponent);

#endif