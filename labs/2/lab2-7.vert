#version 150

in vec3 inPosition;
in vec3 inNormal;
uniform mat4 transform;
uniform mat4 project;
uniform mat4 view;
out vec3 inFragNormal;

void main(void)
{
    mat4 mvp = project * view * transform;
    inFragNormal = mat3(mvp) * inNormal;
    gl_Position = mvp * vec4(inPosition, 1.0);
}