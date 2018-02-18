#version 150

uniform mat4 rotZ;
in vec3 inPosition;
in vec3 inColor;
out vec3 inFragColor;


void main(void)
{
    vec4 pos = vec4(inPosition, 1.0);
    inFragColor = inColor;
    gl_Position = rotZ * pos;
}
