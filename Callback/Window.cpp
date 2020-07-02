#include <GL/glew.h>
#include <cassert>
#include <stdexcept>
#include <z3.h>
#include <iostream>
#include "Window.h"
#include "../Renderer.h"

namespace Callback {
    void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
        std::cout<<"Resized Window. Telling OpenGL\n";
        glViewport(0, 0, width, height);
        Renderer::setResolutionX(width);
        Renderer::setResolutionY(height);
    }
    void maximiseCallback(GLFWwindow* window, int maximized){
        GLFWmonitor* primary = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primary);
        if(maximized==GLFW_TRUE){
            glfwSetWindowMonitor(window,primary,0,0,mode->width,mode->height,mode->refreshRate);
        }
    }
}