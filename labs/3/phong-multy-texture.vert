#version 150
in vec3 inVertex;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 inFragVertex;
out vec3 inFragNormal;
out vec2 inFragTexCoord;

uniform mat4 mvp;

void main(void)
{
    inFragTexCoord = inTexCoord;
	inFragNormal = inNormal;
    inFragVertex = inVertex;
    gl_Position = mvp * vec4(inVertex, 1.0);
}
