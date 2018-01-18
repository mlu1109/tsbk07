#version 150

in vec3 inFragColor;
out vec4 outColor;

void main(void)
{
	outColor = vec4(inFragColor, 1.0);
}
