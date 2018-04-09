#ifndef CALLBACKS_HPP
#define CALLBACKS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.hpp"
#include "mouse.hpp"

void initCallbacks(GLFWwindow *window, Mouse *mouse, Camera *camera);

#endif