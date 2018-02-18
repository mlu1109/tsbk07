#include "shaders.h"

void shaderLight(GLuint program, GLfloat *lightPosition, GLfloat *lightColor, GLint *isDirectional, const GLfloat *k_a, const GLfloat *k_d, const GLfloat *k_s)
{
    if (lightPosition != NULL)
        glUniform3fv(glGetUniformLocation(program, "lightPosition"), 4, lightPosition);
    if (lightColor != NULL)
        glUniform3fv(glGetUniformLocation(program, "lightColor"), 4, lightColor);
    if (isDirectional != NULL)
        glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);
    if (k_a != NULL)
        glUniform1f(glGetUniformLocation(program, "k_a"), *k_a);
    if (k_d != NULL)
        glUniform1f(glGetUniformLocation(program, "k_d"), *k_d);
    if (k_s != NULL)
        glUniform1f(glGetUniformLocation(program, "k_s"), *k_s);
}

void shaderUpload(GLuint program, mat4 *model, mat4 *view, vec3 *cameraPosition, GLuint texUnit, GLfloat specularExponent)
{
    if (model != NULL)
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, model->m);
    if (view != NULL)
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, view->m);
    if (cameraPosition != NULL)
        glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, &cameraPosition->x);
    if (texUnit != -1)
        glUniform1i(glGetUniformLocation(program, "texUnit"), texUnit);
    if (specularExponent != -1)
        glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent);
}

void shaderUpload2Textures(GLuint program, mat4 *model, mat4 *view, vec3 *cameraPosition, GLuint texUnit, GLuint texUnit2, GLfloat specularExponent)
{
    glUniform1i(glGetUniformLocation(program, "texUnit2"), texUnit2);
    shaderUpload(program, model, view, cameraPosition, texUnit, specularExponent);
}