#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <GLFW/glfw3.h>
#include <unordered_map>
#include "camera.hpp"

class Mouse
{
    Camera *m_camera;
    void onRelease(GLFWwindow *window, int button);
    double m_mouseLastX;
    double m_mouseLastY;
    double m_sens = 0.005;
    bool m_mouseLook = false;
    bool m_mouseInvertedY = false;
  public:
    Mouse(Camera *camera);
    void onMove(double x, double y);
    void onButton(GLFWwindow *window, int button, int action);
};

#endif