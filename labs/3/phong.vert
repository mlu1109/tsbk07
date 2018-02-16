#version 150

in vec3 inVertex;
in vec3 inNormal;

out vec3 vertex;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;

void main(void)
{
	normal = mat3(model) * inNormal;
    vertex = vec3(model * vec4(inVertex, 1.0));
    gl_Position = project * view * model * vec4(inVertex, 1.0);
}
