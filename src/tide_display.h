#ifndef TIDE_DISPLAY_H
#define TIDE_DISPLAY_H

struct GLFWwindow;

namespace tide
{
    struct DISPLAY
    {
        GLFWwindow* window;
        unsigned int fps;
    };
    
    DISPLAY* GetDisplay();
}

#endif // TIDE_DISPLAY_H