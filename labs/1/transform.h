#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <math.h>
#include "GL_utilities.h"

void genRotX(GLfloat *mat, float a);
void genRotY(GLfloat *mat, float a);
void genRotZ(GLfloat *mat, float a);
void genProj(GLfloat *mat, float near, float far, float right, float left, float top, float bottom);

#endif
