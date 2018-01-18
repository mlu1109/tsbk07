#include "shaders.h"

void shaderUpload(GLuint program, mat4 *mvp, mat4 *transform, GLuint texUnit, GLfloat specularExponent)
{
    if (mvp != NULL)
        glUniformMatrix4fv(glGetUniformLocation(program, "mvp"), 1, GL_TRUE, mvp->m);
    if (transform != NULL)
        glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_TRUE, transform->m);
    if (texUnit != -1)
        glUniform1i(glGetUniformLocation(program, "texUnit"), texUnit);
    if (specularExponent != -1)
        glUniform1f(glGetUniformLocation(program, "specularExponent"), specularExponent);
}