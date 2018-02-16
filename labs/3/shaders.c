#include "shaders.h"

void shaderUpload(GLuint program, mat4 *model, mat4 *view, GLuint texUnit, GLfloat specularExponent)
{
    if (model != NULL)
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, model->m);
    if (view != NULL)
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, view->m);
    if (texUnit != -1)
        glUniform1i(glGetUniformLocation(program, "texUnit"), texUnit);
    if (specularExponent != -1)
        glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent);
}

void shaderUpload2Textures(GLuint program, mat4 *model, mat4 *view, GLuint texUnit, GLuint texUnit2, GLfloat specularExponent)
{
    glUniform1i(glGetUniformLocation(program, "texUnit2"), texUnit2);
    shaderUpload(program, model, view, texUnit, specularExponent);
}