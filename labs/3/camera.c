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
	vec3 z = v3sub(camera.look, camera.pos);
	vec3 x = CrossProduct(z, camera.up);
	camera.look = MultVec3(mult2(ArbRotate(y, a_x * sens), ArbRotate(x, a_y * sens)), z);
}

void cameraMoveForward(float amount)
{
	vec3 mv = Normalize(v3sub(camera.look, camera.pos));
	mv = v3scale(mv, amount);
	camera.look = v3add(camera.look, mv);
	camera.pos = v3add(camera.pos, mv);
}

void cameraMoveLeft(float amount)
{
	vec3 fw = Normalize(v3sub(camera.look, camera.pos));
	vec3 mv = CrossProduct(fw, camera.up);
	mv = v3scale(mv, amount);
	camera.look = v3sub(camera.look, mv);
	camera.pos = v3sub(camera.pos, mv);
}

void cameraMoveBackward(float amount)
{
	vec3 mv = Normalize(v3sub(camera.look, camera.pos));
	mv = v3scale(mv, amount);
	camera.look = v3sub(camera.look, mv);
	camera.pos = v3sub(camera.pos, mv);
}

void cameraMoveRight(float amount)
{
	vec3 fw = Normalize(v3sub(camera.look, camera.pos));
	vec3 mv = CrossProduct(fw, camera.up);
	mv = v3scale(mv, amount);
	camera.look = v3add(camera.look, mv);
	camera.pos = v3add(camera.pos, mv);
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
