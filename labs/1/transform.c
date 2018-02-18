#include "transform.h"
#include <string.h>

void genRotX(GLfloat *mat, float a)
{
    memset(mat, 0, sizeof(GLfloat) * 16);
    mat[0] = 1.0f;
    mat[5] = cos(a);
    mat[6] = -sin(a);
    mat[9] = sin(a);
    mat[10] = cos(a);
    mat[15] = 1.0f;
}

void genRotY(GLfloat *mat, float a)
{
    memset(mat, 0, sizeof(GLfloat) * 16);
    mat[0] = cos(a);
    mat[2] = sin(a);
    mat[5] = 1.0f;
    mat[8] = -sin(a);
    mat[10] = cos(a);
    mat[15] = 1.0f;
}

void genRotZ(GLfloat *mat, float a)
{
    memset(mat, 0, sizeof(GLfloat) * 16);
    mat[0] = cos(a);
    mat[1] = -sin(a);
    mat[4] = sin(a);
    mat[5] = cos(a);
    mat[10] = 1.0f;
    mat[15] = 1.0f;
}

void genTranslate(GLfloat *mat, float x, float y, float z)
{
    memset(mat, 0, sizeof(GLfloat) * 16);
    mat[0] = 1.0f;
    mat[3] = x;
    mat[5] = 1.0f;
    mat[7] = y;
    mat[10] = 1.0f;
    mat[11] = z;
    mat[15] = 1.0f;
}