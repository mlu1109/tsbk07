#include "input.h"
#include "camera.h"
#include "MicroGlut.h"
#include "loadobj.h"
#include "VectorUtils3.h"

#define SPEED 1

struct Mouse
{
    bool button[3];
    bool capture;
    float sens;
    int lastX;
    int lastY;
} mouse;

void mouseWarp(int x, int y)
{
    glutWarpPointer(x, y);
    mouse.lastX = x;
    mouse.lastY = y;
}

void mouseMotionFunc(int x, int y)
{
    int dx = mouse.lastX - x;
    int dy = mouse.lastY - y;
    mouse.lastX = x;
    mouse.lastY = y;

    if (mouse.button[0] == true && mouse.button[1] == true)
    {
    }
    else if (mouse.button[1] == true)
    {
        cameraReset();
    }
    else if (mouse.button[2] == true)
    {
        cameraMoveForward(dy * mouse.sens);
    }
    else
    {
        cameraRotate(dx, dy, mouse.sens);
    }
}

void mouseButtonFunc(int button, int state, int x, int y)
{
    mouse.lastX = x;
    mouse.lastY = y;

    switch (button)
    {
    case 0:
        mouse.button[0] = state == 0;
        break;
    case 1:
        mouse.button[1] = state == 0;
        cameraReset();
        break;
    case 2:
        mouse.button[2] = state == 0;
        break;
    default:
        break;
    }
}

void keyboardHandler(Object *movable)
{
    // Move camera
    if (glutKeyIsDown('w') || glutKeyIsDown('W'))
    {
        cameraMoveForward(SPEED);
    }
    if (glutKeyIsDown('a') || glutKeyIsDown('A'))
    {
        cameraMoveLeft(SPEED);
    }
    if (glutKeyIsDown('s') || glutKeyIsDown('S'))
    {
        cameraMoveBackward(SPEED);
    }
    if (glutKeyIsDown('d') || glutKeyIsDown('D'))
    {
        cameraMoveRight(SPEED);
    }
    if (glutKeyIsDown(32)) // Space
    {
        cameraReset();
    }
    // Move object
    if (movable != NULL)
    {
        if (glutKeyIsDown('i') || glutKeyIsDown('I'))
        {
            objectMoveZ(movable, SPEED);
        }
        if (glutKeyIsDown('j') || glutKeyIsDown('J'))
        {
            objectMoveX(movable, -SPEED);
        }
        if (glutKeyIsDown('k') || glutKeyIsDown('K'))
        {
            objectMoveZ(movable, -SPEED);
        }
        if (glutKeyIsDown('l') || glutKeyIsDown('L'))
        {
            objectMoveX(movable, SPEED);
        }
    }
}

void inputInit()
{
    glutMouseFunc(&mouseButtonFunc);
    glutPassiveMotionFunc(&mouseMotionFunc);
    //glutHideCursor();
    mouse.button[0] = false;
    mouse.button[1] = false;
    mouse.button[2] = false;
    mouse.sens = 0.0024;
}