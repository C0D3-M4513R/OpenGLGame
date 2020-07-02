#include "Renderer.h"
#include "Camera.h"
#include "Shader.h"
#include "Face.h"
#include "Callback/Keyboard.h"
#include "Player.h"
#include "Callback/Window.h"
#include "Callback/Debug.h"


#include <GL/glew.h>
#include <glfreetype/TextRenderer.hpp>
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
            glfwSetErrorCallback(Callback::errorCallback);

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


            return true;
        }

        /**
         * This should prepare everything, for displaying stuff
         */
        bool setup() {
            // Load shader.
            cam = new Camera({0, 0, 1}, {0, 0, 0});
            std::cout<<"Good Cam\n";
            shader = new Shader("resources/Simple.vert", "resources/Simple.frag");
            if (!shader->IsGood()) { return false; }
            std::cout<<"Good Shader\n";


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

//            //Init objects
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

        /**
         * Animation loop
         */
        void loop() {

            glfreetype::font_data our_font;
            our_font.init("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 25 /* size */);

            glm::vec3 rotate = {0.1f, 0.f, 0.f};

            glfwSetFramebufferSizeCallback(win,Callback::framebufferSizeCallback);

            glfwSetWindowMaximizeCallback(win,Callback::maximiseCallback);

            // annimation loop
            while (!glfwWindowShouldClose(win)) {
                Clear();


                glfwPollEvents();
                meshes[0]->rotate(rotate);
                rotate += glm::vec3(0.f, 0.f, 0.01f);
                Render();

                glfreetype::print(our_font,0,resolution.second-27,"This is a TEST! äöüß");

                Present();
            }
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

    unsigned int getResolutionX() {return resolution.first;}
    unsigned int getResolutionY() {return resolution.second;}
    void setResolutionX(unsigned int x){resolution.first=x;};
    void setResolutionY(unsigned int y){resolution.second=y;};
}