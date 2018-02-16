#version 150
in vec3 inVertex;
in vec2 inTexCoord;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

void main(void)
{
    texCoord = inTexCoord;
	gl_Position = project * view * model * vec4(inVertex, 1.0);
}
