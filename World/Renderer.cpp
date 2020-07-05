#include "Renderer.h"
#include "../Callback/Keyboard.h"
#include "../Callback/Debug.h"
#include "Scene.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace Renderer {
    namespace {
        bool hasInit = false;

        auto resolution = std::pair<int, int>(1000, 1000);

        GLFWwindow *win;
        GLFWmonitor *monitor;


        /**
        * This should Initialise a window and make it usable with OpenGL
        * @return true, if Initialisation succeeded
         */
        [[nodiscard]]
        bool init() {
            glfwSetErrorCallback(Callback::errorCallback);
            // returns zero on success else non-zero
            if (glfwInit() != GLFW_TRUE) {
                std::cerr << "some error initializing glfw" << std::endl;
                return false;
            }

            std::cout << "Hello World!\n";


            //Create a Window on a monitor
            monitor = glfwGetPrimaryMonitor();
//            glfwGetMonitorWorkarea(monitor, nullptr, nullptr,&resolution.first,&resolution.second);

            win = glfwCreateWindow(resolution.first, resolution.second, "OpenGLGame", nullptr, nullptr);

            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
            glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
            glfwSetWindowSizeLimits(win,10,10,GLFW_DONT_CARE,GLFW_DONT_CARE);

            glfwMakeContextCurrent(win);

            //Define callbacks:
            glfwSetKeyCallback(win, Callback::keyCallback);

            // Initialize GLEW.
            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK) return false;

            // Tell SDL we want to use OpenGL 3.3
            // These attributes must be set before creating the window.

            GLint major, minor;
            glGetIntegerv(GL_MAJOR_VERSION, &major);
            glGetIntegerv(GL_MINOR_VERSION, &minor);
            if (major < 3 || (major = 3 && minor < 3)) {
                std::cerr << "Minimum Requirements not met! OpenGL version should be at least 3.3, but is " << major<< "." << minor << "\n";
                std::cerr<<"This might lead to the Program not starting, so don't bother posting an issue, unless this is resolved!\n";
            }

            // Use double buffering.
            GLint doublebuffer=GL_FALSE;
            glGetIntegerv(GL_DOUBLEBUFFER, &doublebuffer);
            if (doublebuffer != GLFW_TRUE) {
                std::cerr << "DoubleBuffering is NOT supported.\n";
            }

            // We request from OpenGL at least 8-bits per channel for the color buffer and depth buffer
            GLint res;
            glGetIntegerv(GL_RED_BITS, &res);
            if (res < 8) {
                std::cerr << "Minimum Requirements not met! GL_RED_BITS should be at least 8, but is " << res
                          << std::endl;
                exit(-1);
            }
            glGetIntegerv(GL_GREEN_BITS, &res);
            if (res < 8) {
                std::cerr << "Minimum Requirements not met! GL_GREEN_BITS should be at least 8, but is " << res
                          << std::endl;
                exit(-1);
            }
            glGetIntegerv(GL_BLUE_BITS, &res);
            if (res < 8) {
                std::cerr << "Minimum Requirements not met! GL_BLUE_BITS should be at least 8, but is " << res
                          << std::endl;
                exit(-1);
            }
            glGetIntegerv(GL_DEPTH_BITS, &res);
            if (res < 8) {
                std::cerr << "Minimum Requirements not met! GL_DEPTH_BITS should be at least 8, but is " << res
                          << std::endl;
                exit(-1);
            }

            //confirm
            int nrAttributes;
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
            std::cout << "Maximum nr of vertex attributes supported:" << nrAttributes << "\n";

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            glEnable(GL_CULL_FACE);

            int flags;
            glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
            if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(Callback::glDebugOutput, nullptr);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            }

            (new Scene(win))->Activate();


            return true;
        }


        void destruct() {
            //Follow with undoing everything inn init
            //destroy window
            glfwDestroyWindow(win);
            glfwTerminate();
        }
    };

    bool run() {
        if (!hasInit) {
            hasInit = true;
            if (!init()) exit(-1);

            destruct();
            return true;
        }
        return false;
    }
    unsigned int getResolutionX() {return resolution.first;}
    unsigned int getResolutionY() {return resolution.second;}
    void setResolutionX(unsigned int x){resolution.first=x;};
    void setResolutionY(unsigned int y){resolution.second=y;};
}