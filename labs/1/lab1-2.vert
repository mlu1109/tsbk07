#version 150

uniform mat4 myMatrix;
in vec3 inPosition;

void main(void)
{
	gl_Position = myMatrix * vec4(inPosition, 1.0);
}
