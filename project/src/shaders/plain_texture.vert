#version 150
layout(location = 0) in vec3 inVertex;
layout(location = 1) in vec2 texCoord;
uniform mat4 mvp;

void main(void)
{
    gl_Position = mvp * vec4(inVertex, 1.0);
    texCoord = inTexCoord;
}
