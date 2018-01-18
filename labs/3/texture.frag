#version 150
in vec2 inFragTexCoord;
out vec4 outColor;

uniform sampler2D texUnit;

void main(void)
{
	outColor = texture(texUnit, inFragTexCoord);
}