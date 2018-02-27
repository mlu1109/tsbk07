#version 150
in vec3 inVertex;
in vec3 inNormal;
out vec3 normal;

uniform mat4 mvp;
uniform mat3 nmvp;

void main(void)
{
    normal = nmvp * inNormal;
    gl_Position = mvp * vec4(inVertex, 1.0);
}
