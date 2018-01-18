#version 150

uniform mat4 rotZ;
in vec3 inPosition;

void main(void)
{
	mat4 move = rotZ;
	gl_Position = move * vec4(inPosition, 1.0);
}
