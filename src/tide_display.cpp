#include "tide_display.h"

#include <GLFW/glfw3.h>

uint32_t tide::GetDisplayWidth()
{
    int width;
    glfwGetWindowSize(tide::GetDisplay()->window, &width, 0);
    return width;
}

uint32_t tide::GetDisplayHeight()
{
    int height;
    glfwGetWindowSize(tide::GetDisplay()->window, 0, &height);
    return height;
}