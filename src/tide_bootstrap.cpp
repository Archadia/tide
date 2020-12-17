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
            else TIDE_ERROR("GLAD error");
        }
        else TIDE_ERROR("Window error");
    }
    else TIDE_ERROR("GLFW error");
    glfwTerminate();
    return NULL;
}

static tide::DISPLAY* display;

int main()
{
    display = CreateDisplay();
    if(display)
    {
        glfwSwapInterval(0);
        
        TIDE_LOG("Tide successfully loaded");
        tide::ProcessInit();
        
        const double dt = 0.01;
        double acc1 = 0.0;
        double acc2 = 0.0;
        
        double time = 0.0;
        
        clock_t startTime = clock();
        
        double frameTime;
        
        int frameCount;
        
        while(!glfwWindowShouldClose(display->window))
        {
            clock_t endTime = clock();
            frameTime = 1.0*(endTime - startTime)/CLOCKS_PER_SEC;
            startTime = endTime;
            
            acc1 += frameTime;
            acc2 += frameTime;
            
            if(acc1 >= dt)
            {
                acc1 -= dt;
                time += dt;
                tide::ProcessUpdate(time, dt);
            }
            
            if(acc2 >= 1.0)
            {
                display->fps = frameCount;
                frameCount = 0;
                acc2 -= 1;
            }
            
            frameCount++;
            tide::ProcessDraw();
            glfwSwapBuffers(display->window);
            glfwPollEvents();
            
            
        }
        tide::ProcessClose();
        free(display);
    }
    TIDE_LOG("Tide successfully destroyed");
    return 0;
}

tide::DISPLAY* GetDisplay()
{
    return display;
}