#version 150

uniform mat4 rotZ;
uniform mat4 rotY;
uniform mat4 rotX;
in vec3 inPosition;
in vec3 inColor;
out vec3 inFragColor;


void main(void)
{
	vec4 pos = rotX * rotY * rotZ * vec4(inPosition, 1.0);
	inFragColor = inColor;
	gl_Position = pos;
}
