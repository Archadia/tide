#include "tide_display.h"

#include <GLFW/glfw3.h>

uint32_t GetDisplayWidth()
{
    int width;
    glfwGetWindowSize(GetDisplay()->window, &width, 0);
    return width;
}

uint32_t GetDisplayHeight()
{
    int height;
    glfwGetWindowSize(GetDisplay()->window, 0, &height);
    return height;
}