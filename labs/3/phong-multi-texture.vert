#version 150
in vec3 inVertex;
in vec3 inNormal;
in vec2 inTexCoord;

out vec3 vertex;
out vec3 normal;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

void main(void)
{
    texCoord = inTexCoord;
	normal = mat3(model) * inNormal;
    vertex = vec3(model * vec4(inVertex, 1));
    gl_Position = project * view * model * vec4(inVertex, 1);
}
