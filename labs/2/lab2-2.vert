#version 150

// Model specific
in vec3 inPosition;
in vec2 inTexCoord;
// Transformation matrices
uniform mat4 rotX;
uniform mat4 rotY;
uniform mat4 rotZ;
// Time
uniform float inTimeStamp;
// Out
// out vec3 inFragNormal;
out vec2 inFragTexCoord;

void main(void)
{
	// inFragNormal = inNormal;
	inFragTexCoord = inTexCoord;
	gl_Position = rotY*vec4(inPosition, 1.0);
}
