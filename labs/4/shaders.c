#include "shaders.h"

void shaderUpload(GLuint program, mat4 *modelView)
{
    glUniformMatrix4fv(glGetUniformLocation(program, "modelView"), 1, GL_TRUE, modelView->m);
}