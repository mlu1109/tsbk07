#version 150

in vec3 inFragNormal;
out vec4 outColor;

void main(void)
{
	const vec3 light = vec3(0, 0.58, -0.58);
	float shade = clamp(dot(normalize(inFragNormal), light), 0, 1);
	outColor = vec4(shade, shade, shade, 1.0);
}
