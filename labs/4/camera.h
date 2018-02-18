#ifndef CAMERA_H
#define CAMERA_H

#include "VectorUtils3.h"
#include "algebra.h"

struct Camera
{
    vec3 pos;
    vec3 look;
    vec3 up;
    vec3 reset_pos;
    vec3 reset_look;
    vec3 reset_up;
} camera;

void cameraInit(vec3 pos, vec3 look, vec3 up);
void cameraReset();
void cameraRotate(float a_x, float a_y, float sens);
void cameraMoveForward(float amount);
void cameraMoveLeft(float amount);
void cameraMoveBackward(float amount);
void cameraMoveRight(float amount);
mat4 cameraLookAt();

#endif