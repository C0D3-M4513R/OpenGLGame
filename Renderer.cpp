#include "Renderer.h"
#include "Camera.h"
#include "Shader.h"
#include "Face.h"


#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <cstdlib>

namespace Renderer {
    namespace {
        bool hasInit = false;

        auto resolution=std::pair<unsigned int,unsigned int>(1000,1000);


        Shader* shader;
        Camera* cam;

        SDL_Window* win;
        SDL_GLContext GL_Context;

        Face* triangle_vertices;


        /**
        * This should Initialise a window and make it usable with OpenGL
        * @return true, if Initialisation succeeded
         */

        [[nodiscard]]
        bool init() {
            // returns zero on success else non-zero
            if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
                SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_CRITICAL);
                SDL_Log("error initializing SDL: %s\n", SDL_GetError());
                return false;
            }
            SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
            SDL_Log("Hello World!");

            //set default logging level to verbose
            SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);


            // Tell SDL we want to use OpenGL 3.3
            // These attributes must be set before creating the window.
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

            // Use double buffering.
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

            // Require hardware acceleration.
            SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

            // We request from OpenGL at least 8-bits per channel for the color buffer and depth buffer
            SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,8);

            win = SDL_CreateWindow("GAME", // creates a window
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   resolution.first, resolution.second, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
            if (win == nullptr) return false;

            // Create OpenGL context.
            GL_Context = SDL_GL_CreateContext(win);
            if (GL_Context == nullptr) return false;

            // Initialize GLEW.
            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK) return false;

            //confirm
            int nrAttributes;
            glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Maximum nr of vertex attributes supported: %i", nrAttributes);

            glEnable (GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);

            glEnable(GL_CULL_FACE);

            return true;
        }

        /**
         * This should prepare everything, for displaying stuff
         */
        bool setup() {
            // Load shader.
            cam=new Camera({0,0,0},{0,0,1});
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

            triangle_vertices = new Face("resources/cube.stl",FILE_TYPE::STL,GL_STREAM_DRAW);

            // Init succeeded!
            return true;
        }

        void Clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void Render() {
            shader->Activate();
            triangle_vertices->Draw();
        }

        void Present() {
            SDL_GL_SwapWindow(win);
        }

        void nextFrame(){
            Clear();
            Render();
            Present();
        }

        /**
         * Animation loop
         */
        void loop() {
            // controls annimation loop
            bool close = false;

            glm::vec3 rotate = {0.1f,0.f,0.f};

            // annimation loop
            while (!close) {
                SDL_Event event;

                // Events mangement
                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                        [[unlikely]]
                        case SDL_QUIT:
                            // handling of close button
                            close = true;
                            break;

                        case SDL_WINDOWEVENT:
                            switch (event.window.event) {
                                case SDL_WINDOWEVENT_RESIZED:
                                    //window was resized
                                    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
                                    SDL_Log("Window was resized. Size should be %ix%i", event.window.data1,
                                            event.window.data2);
                                    glViewport(0, 0, event.window.data1, event.window.data2);
                                    break;
                            }
                            break;
                    }
                }

                const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);


                if (keyboard_state_array[SDL_SCANCODE_W] || keyboard_state_array[SDL_SCANCODE_UP]) {
                    triangle_vertices->moveY(0.1f);
                }
                if (keyboard_state_array[SDL_SCANCODE_S] || keyboard_state_array[SDL_SCANCODE_DOWN]) {
                    triangle_vertices->moveY(-0.1f);
                }
                if (keyboard_state_array[SDL_SCANCODE_A] || keyboard_state_array[SDL_SCANCODE_LEFT]) {
                    triangle_vertices->moveX(0.1f);
                }
                if (keyboard_state_array[SDL_SCANCODE_D] || keyboard_state_array[SDL_SCANCODE_RIGHT]) {
                    triangle_vertices->moveX(-0.1f);
                }

                triangle_vertices->rotate(rotate);
                rotate+=glm::vec3(0.f,0.f,0.01f);

                nextFrame();
            }
        }

        void destruct() {
            //Follow with undoing everything inn init
            SDL_GL_DeleteContext(GL_Context);
            //destroy window
            SDL_DestroyWindow(win);
            SDL_QuitSubSystem(SDL_INIT_VIDEO);

            delete triangle_vertices;
            delete cam;
            delete shader;
        }
    };
    bool run(){
        if(!hasInit) {
            hasInit=true;
            if (!init()) exit(-1);
            setup();
            loop();
            destruct();
            return true;
        }
        return false;
    }
    const Shader*const getShader(){
        return shader;
    }
    const Camera*const getCamera(){
        return cam;
    }
    const std::pair<unsigned int,unsigned int>& getResolution(){
        return resolution;
    }
    const unsigned int getResolutionX(){
        return resolution.first;
    }
    const unsigned int getResolutionY(){
        return resolution.second;
    }
}