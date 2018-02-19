#version 150

// bin vec3 inFragNormal;
in vec2 inFragTexCoord;
uniform float inTimeStamp;
uniform sampler2D texUnit;
out vec4 outColor;

void main(void)
{
    outColor = texture(texUnit, inFragTexCoord*10);
}
