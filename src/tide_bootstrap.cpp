#include "tide_display.h"
#include "tide_process.h"
#include "tide_log.h"

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <time.h>

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
#define DEFAULT_WINDOW_TITLE "Tide"

tide::DISPLAY* CreateDisplay()
{
    if(glfwInit())
    {
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

        GLFWwindow* window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_TITLE, 0, 0);
        if(window)
        {
            glfwMakeContextCurrent(window);
            if(gladLoadGL())
            {
                tide::DISPLAY* display = (tide::DISPLAY*) malloc(sizeof(tide::DISPLAY));
                display->window = window;
                return display;
            }
            else printf("%s GLAD error", TIDE_ERROR_PREFIX);
        }
        else printf("%s Window error", TIDE_ERROR_PREFIX);
    }
    else printf("%s GLFW error", TIDE_ERROR_PREFIX);
    glfwTerminate();
    return NULL;
}

static tide::DISPLAY* display;

int main()
{
    display = CreateDisplay();
    if(display)
    {
        printlnf("%s Tide successfully loaded", TIDE_LOG_PREFIX);
        tide::ProcessInit();

        const double dt = 0.01;
        double accumulator = 0.0;
        
        double time = 0.0;

        clock_t startTime = clock();


        while(!glfwWindowShouldClose(display->window))
        {
            clock_t endTime = clock();
            auto frameTime = endTime - startTime;
            startTime = endTime;
            accumulator += frameTime;

            if(accumulator >= dt)
            {
                accumulator -= dt;
                time += dt;
                tide::ProcessUpdate(time, dt);
            }
            tide::ProcessDraw();
            glfwSwapBuffers(display->window);
            glfwPollEvents();
        }
        tide::ProcessClose();
        free(display);
    }
    printlnf("%s Tide successfully destroyed", TIDE_LOG_PREFIX);
    return 0;
}

tide::DISPLAY* GetDisplay()
{
    return display;
}