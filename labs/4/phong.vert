#version 150

in vec3 inPosition;
in vec3 inNormal;

out vec3 normal;
out vec3 vertex;

uniform mat4 project;
uniform mat4 modelView;

void main(void)
{
	mat3 normalMatrix1 = mat3(modelView);
	normal = normalMatrix1 * inNormal;
	vertex = vec3(modelView * vec4(inPosition, 1);
	gl_Position = project * modelView * vec4(inPosition, 1);
}