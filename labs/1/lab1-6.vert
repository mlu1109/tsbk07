#version 150

uniform mat4 rotZ;
uniform mat4 rotY;
uniform mat4 rotX;
in vec3 inPosition;
in vec3 inNormal;
out vec3 inFragNormal;

void main(void)
{
    mat4 rot = rotX * rotY * rotZ;
    inFragNormal = vec3(rot * vec4(inNormal, 1.0));
    gl_Position = rot * vec4(inPosition, 1.0);
}
