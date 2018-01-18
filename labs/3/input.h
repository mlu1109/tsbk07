#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include "camera.h"

void inputInit();
void mouseWarp();
void mouseButtonFunc(int button, int state, int x, int y);
void mouseMotionFunc(int x, int y);
void keyboardHandler();

#endif