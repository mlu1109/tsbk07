#include "callbacks.hpp"

Mouse *g_mouse;

/* Errors */

void errorCallback(int, const char *description)
{
    std::cout << "Error: " << description << '\n';
}

/* Window */

/*
void windowResizeCallback(GLFWwindow *window, int width, int height)
{

}
*/

/* Cursor/Mouse */

static void cursorPositionCallback(GLFWwindow *, double xpos, double ypos)
{
    g_mouse->onMove(xpos, ypos);
}

/*
void cursorEnterCallback(GLFWwindow *window, int entered)
{
    if (entered)
    {
        // The cursor entered the client area of the window
    }
    else
    {
        // The cursor left the client area of the window
    }
}
*/

void mouseButtonCallback(GLFWwindow *window, int button, int action, int /* mods */)
{
    g_mouse->onButton(window, button, action);
}

/* Init */

void initCallbacks(GLFWwindow *window, Mouse *mouse)
{
    g_mouse = mouse;
    /* Error */
    glfwSetErrorCallback(errorCallback);
    /* Window */
    //glfwSetWindowSizeCallback(window, windowResizeCallback);
    /* Cursor/Mouse */
    //glfwSetCursorEnterCallback(g_window, cursorEnterCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}
