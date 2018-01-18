#version 150

in vec3 inPosition;
in vec2 inTexCoord;
uniform mat4 transform;
uniform mat4 project;
out vec2 inFragTexCoord;

void main(void)
{
	inFragTexCoord = inTexCoord;
	gl_Position = project * transform * vec4(inPosition, 1.0);
}