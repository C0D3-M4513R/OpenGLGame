#include "Renderer.h"
#include "Camera.h"
#include "Shader.h"
#include "Face.h"
#include "HID/Keyboard.h"
#include "Player.h"


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

namespace Renderer {
    namespace {
        bool hasInit = false;

        auto resolution = std::pair<int, int>(1000, 1000);


        Shader *shader;
        Camera *cam;

        GLFWwindow *win;
        GLFWmonitor *monitor;

        std::vector<Face *> meshes;


        void GLAPIENTRY glDebugOutput(
                [[maybe_unused]] GLenum source,
                [[maybe_unused]] GLenum type,
                [[maybe_unused]] unsigned int id,
                [[maybe_unused]] GLenum severity,
                [[maybe_unused]] GLsizei length,
                [[maybe_unused]] const char *message,
                [[maybe_unused]] const void *userParam
        ) {
            // ignore non-significant error/warning codes
//            if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

            std::cout << "---------------" << newline;
            std::cout << "Debug message (" << id << "): " << message << newline;

            switch (source) {
                case GL_DEBUG_SOURCE_API:
                    std::cout << "Source: API" << newline;
                    break;
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                    std::cout << "Source: Window System" << newline;
                    break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER:
                    std::cout << "Source: Shader Compiler" << newline;
                    break;
                case GL_DEBUG_SOURCE_THIRD_PARTY:
                    std::cout << "Source: Third Party" << newline;
                    break;
                case GL_DEBUG_SOURCE_APPLICATION:
                    std::cout << "Source: Application" << newline;
                    break;
                case GL_DEBUG_SOURCE_OTHER:
                    std::cout << "Source: Other" << newline;
                    break;
            }
            switch (type) {
                case GL_DEBUG_TYPE_ERROR:
                    std::cout << "Type: Error" << newline;
                    break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                    std::cout << "Type: Deprecated Behaviour" << newline;
                    break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                    std::cout << "Type: Undefined Behaviour" << newline;
                    break;
                case GL_DEBUG_TYPE_PORTABILITY:
                    std::cout << "Type: Portability" << newline;
                    break;
                case GL_DEBUG_TYPE_PERFORMANCE:
                    std::cout << "Type: Performance" << newline;
                    break;
                case GL_DEBUG_TYPE_MARKER:
                    std::cout << "Type: Marker" << newline;
                    break;
                case GL_DEBUG_TYPE_PUSH_GROUP:
                    std::cout << "Type: Push Group" << newline;
                    break;
                case GL_DEBUG_TYPE_POP_GROUP:
                    std::cout << "Type: Pop Group" << newline;
                    break;
                case GL_DEBUG_TYPE_OTHER:
                    std::cout << "Type: Other" << newline;
                    break;
            }
            switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH:
                    std::cout << "Severity: high" << newline;
                    break;
                case GL_DEBUG_SEVERITY_MEDIUM:
                    std::cout << "Severity: medium" << newline;
                    break;
                case GL_DEBUG_SEVERITY_LOW:
                    std::cout << "Severity: low" << newline;
                    break;
                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    std::cout << "Severity: notification" << newline;
                    break;
            }
            std::cout.flush();
        }

        void errorCallback(int error_code, const char *description) {
            std::cerr << "GLFW ERROR: Error Code:" << error_code << newline;
            std::cerr << "            Description:" << description << std::endl;
        }

        /**
        * This should Initialise a window and make it usable with OpenGL
        * @return true, if Initialisation succeeded
         */
        [[nodiscard]]
        bool init() {
            // returns zero on success else non-zero
            if (glfwInit() != GLFW_TRUE) {
                const char *err;
                glfwGetError(&err);
                std::cerr << "error initializing glfw: " << err << std::endl;
                return false;
            }
            glfwSetErrorCallback(errorCallback);

            std::cout << "Hello World!" << newline;


            //Create a Window on a monitor
            monitor = glfwGetPrimaryMonitor();
//            glfwGetMonitorWorkarea(monitor, nullptr, nullptr,&resolution.first,&resolution.second);

            win = glfwCreateWindow(resolution.first, resolution.second, "OpenGLGame", nullptr, nullptr);

            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
            glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

            glfwMakeContextCurrent(win);

            //Define callbacks:
            glfwSetKeyCallback(win, HID::keyCallback);

            // Initialize GLEW.
            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK) return false;

            // Tell SDL we want to use OpenGL 3.3
            // These attributes must be set before creating the window.

            GLint major, minor;
            glGetIntegerv(GL_MAJOR_VERSION, &major);
            glGetIntegerv(GL_MINOR_VERSION, &minor);
            if (major < 3 || (major = 3 && minor < 3)) {
                std::cerr << "Minimum Requirements not met! OpenGL version should be at least 3.3, but is " << major
                          << "." << minor << newline;
                std::cerr
                        << "This might lead to the Program not starting, so don't bother posting an issue, unless this is resolved!"
                        << newline;
            }

            // Use double buffering.
            GLint doublebuffer=GL_FALSE;
            glGetIntegerv(GLFW_DOUBLEBUFFER, &doublebuffer);
            if (doublebuffer != GLFW_TRUE) {
                std::cerr << "DoubleBuffering is NOT supported." << newline;
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
            std::cout << "Maximum nr of vertex attributes supported:" << nrAttributes << newline;

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            glEnable(GL_CULL_FACE);

            int flags;
            glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
            if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(glDebugOutput, nullptr);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            }


            return true;
        }

        /**
         * This should prepare everything, for displaying stuff
         */
        bool setup() {
            // Load shader.
            cam = new Camera({0, 0, 0}, {0, 0, 1});
            shader = new Shader("resources/Simple.vert", "resources/Simple.frag");
            if (!shader->IsGood()) { return false; }


            //Those arrays should be deleted by Face!
            const unsigned int n = 4;
            auto *vertices = new glm::vec3[n];
            vertices[0] = {-0.5f, -0.5f, 0.0f};
            vertices[1] = {0.5f, -0.5f, 0.0f};
            vertices[2] = {-0.5f, 0.5f, 0.0f};
            vertices[3] = {0.5f, 0.5f, 0.0f};
            auto *color = new glm::vec3[n];
            color[0] = {0.f, 1.f, 0.0f};
            color[1] = {0.f, 0.f, 1.0f};
            color[2] = {1.f, 0.f, 0.0f};
            color[3] = {1.f, 1.f, 1.0f};

            //Init objects
            meshes.resize(2);
            meshes[0] = new Face("resources/cube.stl", FILE_TYPE::STL, GL_STATIC_DRAW);
            meshes[1] = new Face(vertices, n, color);

            Player::getPlayer(new Face("resources/cube.stl", FILE_TYPE::STL, GL_STATIC_DRAW));

            // Init succeeded!
            return true;
        }

        void Clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void Render() {
            Player::getPlayer()->Draw();
            for (Face *mesh:meshes) {
                mesh->Draw();
            }

        }

        void Present() {
            glfwSwapBuffers(win);
        }

        void nextFrame() {
            Clear();
            Render();
            Present();
        }

        /**
         * Animation loop
         */
        void loop() {

            glm::vec3 rotate = {0.1f, 0.f, 0.f};

            // annimation loop
            while (!glfwWindowShouldClose(win)) {
                glfwPollEvents();
                meshes[0]->rotate(rotate);
                rotate += glm::vec3(0.f, 0.f, 0.01f);

                nextFrame();
            }
        }

        void destruct() {
            //Follow with undoing everything inn init
            //destroy window
            glfwDestroyWindow(win);
            glfwTerminate();

            for (Face *mesh : meshes) {
                delete mesh;
            }
            delete cam;
            delete shader;
        }
    };

    bool run() {
        if (!hasInit) {
            hasInit = true;
            if (!init()) exit(-1);
            setup();
            loop();
            destruct();
            return true;
        }
        return false;
    }

    const Shader &getShader() {
        return *shader;
    }

    const Camera &getCamera() {
        return *cam;
    }

    const std::pair<unsigned int, unsigned int> &getResolution() {
        return resolution;
    }

    unsigned int getResolutionX() {
        return resolution.first;
    }

    unsigned int getResolutionY() {
        return resolution.second;
    }
}