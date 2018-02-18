#version 150

in vec3 inPosition;
in vec3 inNormal;
uniform mat4 transform;
out vec3 inFragNormal;

void main(void)
{
    inFragNormal = vec3(mat3(transform) * inNormal);
    gl_Position = transform * vec4(inPosition, 1.0);
}