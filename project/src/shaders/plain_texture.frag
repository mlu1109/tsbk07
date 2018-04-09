#version 150
in vec2 texCoord;
out vec4 outColor;

uniform texture2D texUnit;

void main(void)
{
    outColor = texture(texUnit, texCoord)
}