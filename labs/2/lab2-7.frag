#version 150

in vec3 inFragNormal;
out vec4 outColor;
const vec3 light = vec3(0.58, 0.58, 0.58);

void main(void)
{
    float shade = clamp(dot(normalize(inFragNormal), light), 0, 1);
    outColor = vec4(shade, shade, shade, 1.0);
}
