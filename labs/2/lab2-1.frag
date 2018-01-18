#version 150

// in vec3 inFragNormal;
in vec2 inFragTexCoord;
uniform float inTimeStamp;
out vec4 outColor;

void main(void)
{
	float t = inTimeStamp / 100;
	float a = sin(inFragTexCoord.s*t)/2;
	float b = sin(inFragTexCoord.t*t)/2;
	outColor = vec4(a, b, 1.0, 1.0);
}