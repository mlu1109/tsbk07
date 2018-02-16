#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "VectorUtils3.h"
#include "camera.h"
#include "object.h"

void inputInit();
void mouseWarp(int x, int y);
void mouseButtonFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);
void keyboardHandler(Object *movable);

#endif