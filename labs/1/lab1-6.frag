#version 150

in vec3 inFragNormal;
out vec4 outColor;

void main(void)
{
	outColor = vec4(inFragNormal, 1.0);
}
