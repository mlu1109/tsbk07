#version 150
in vec3 inVertex;
in vec2 inTexCoord;
out vec2 inFragTexCoord;

uniform mat4 mvp;

void main(void)
{
    inFragTexCoord = inTexCoord;
	gl_Position = mvp * vec4(inVertex, 1.0);
}
