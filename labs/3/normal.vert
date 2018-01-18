#version 150
in vec3 inVertex;
in vec3 inNormal;
out vec3 inFragNormal;

uniform mat4 mvp;

void main(void)
{
	gl_Position = mvp * vec4(inVertex, 1.0);
    inFragNormal = inNormal;
}
