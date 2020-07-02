#include <GL/glew.h>
#include <cassert>
#include <stdexcept>
#include "Window.h"
#include "../Renderer.h"

namespace HID {
    void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
#ifdef NDEBUG
        if (width <= 1 || height <= 1) {
            throw std::runtime_error("Either Width or Height is 0. This is NOT valid. EXITING!");
        }
#else
        assert(width>1);
        assert(height>1);
#endif
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