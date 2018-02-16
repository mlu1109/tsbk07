#include "camera.h"

void cameraReset()
{
	camera.pos = camera.reset_pos;
	camera.look = camera.reset_look;
	camera.up = camera.reset_up;
}

void cameraRotate(float a_x, float a_y, float sens)
{
	// a_x rotates around "y" (camera_up)
	vec3 y = camera.up;
	// a_y rotates around "x" (z x camera_up)
	vec3 z = VectorSub(camera.look, camera.pos);
	vec3 x = CrossProduct(z, camera.up);
	camera.look = MultVec3(mult2(ArbRotate(y, a_x * sens), ArbRotate(x, a_y * sens)), z);
}

void cameraMoveForward(float amount)
{
	vec3 mv = Normalize(VectorSub(camera.look, camera.pos));
	mv = ScalarMult(mv, amount);
	camera.look = VectorAdd(camera.look, mv);
	camera.pos = VectorAdd(camera.pos, mv);
}

void cameraMoveLeft(float amount)
{
	vec3 fw = Normalize(VectorSub(camera.look, camera.pos));
	vec3 mv = CrossProduct(fw, camera.up);
	mv = ScalarMult(mv, amount);
	camera.look = VectorSub(camera.look, mv);
	camera.pos = VectorSub(camera.pos, mv);
}

void cameraMoveBackward(float amount)
{
	vec3 mv = Normalize(VectorSub(camera.look, camera.pos));
	mv = ScalarMult(mv, amount);
	camera.look = VectorSub(camera.look, mv);
	camera.pos = VectorSub(camera.pos, mv);
}

void cameraMoveRight(float amount)
{
	vec3 fw = Normalize(VectorSub(camera.look, camera.pos));
	vec3 mv = CrossProduct(fw, camera.up);
	mv = ScalarMult(mv, amount);
	camera.look = VectorAdd(camera.look, mv);
	camera.pos = VectorAdd(camera.pos, mv);
}

mat4 cameraLookAt()
{
	return lookAtv(camera.pos, camera.look, camera.up);
}

void cameraInit(vec3 pos, vec3 look, vec3 up)
{
	camera.reset_pos = pos;
	camera.reset_look = look;
	camera.reset_up = up;
	cameraReset();
}
