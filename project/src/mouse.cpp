#include "mouse.hpp"
#include <GLFW/glfw3.h>

Mouse::Mouse(Camera *camera) : m_camera(camera)
{
}

void Mouse::onRelease(GLFWwindow *window, int button)
{
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
    {
        int nextMode;
        if (m_mouseLook)
        {
            nextMode = GLFW_CURSOR_NORMAL;
            m_mouseLook = false;
        }
        else
        {
            nextMode = GLFW_CURSOR_DISABLED;
            m_mouseLook = true;
        }

        glfwSetInputMode(window, GLFW_CURSOR, nextMode);
        break;
    }
    default:
    {
        std::cout << "unhandled mouse button: " << button << '\n';
    }
    }
}

void Mouse::onButton(GLFWwindow *window, int button, int action)
{
    if (action == GLFW_RELEASE)
        onRelease(window, button);
    else
        std::cout << "unhandled mouse action: " << action << '\n';
}

void Mouse::onMove(double x, double y)
{
    if (m_mouseLook)
    {
        double dx = x - m_mouseLastX;
        double dy = m_mouseInvertedY ? y - m_mouseLastY : m_mouseLastY - y;
        m_camera->yaw(dx * m_sens);
        m_camera->pitch(dy * m_sens);
    }
    m_mouseLastX = x;
    m_mouseLastY = y;
}