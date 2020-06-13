#include "Renderer.h"
#include "Shader.h"

#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>


/**
 * This should Initialise a window and make it usable with OpenGL
 * @return true, if Initialisation succeeded
 */
[[nodiscard]]
bool Renderer::init() {
    // returns zero on success else non-zero
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_CRITICAL);
        SDL_Log("error initializing SDL: %s\n", SDL_GetError());
        return false;
    }
    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_DEBUG);
    SDL_Log("Hello World!");

    // Tell SDL we want to use OpenGL 3.3
    // These attributes must be set before creating the window.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    // Use double buffering.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Require hardware acceleration.
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // We request from OpenGL at least 8-bits per channel for the color buffer.
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);


    win = SDL_CreateWindow("GAME", // creates a window
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       resolution.first, resolution.second, SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
    if(win==nullptr) return false;

    // Create OpenGL context.
    GL_Context = SDL_GL_CreateContext(win);
    if(GL_Context == nullptr) return false;

    // Initialize GLEW.
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) return false;

    return true;
}

/**
 * This should prepare everything, for displaying stuff
 */
bool Renderer::setup() {
    // Load shader.
    shader = new Shader("resources/Simple.vert", "resources/Simple.frag");
    if(!shader->IsGood()) { return false; }

    auto* vertices = new Vertex<float>[4];
    vertices[0]={-0.5f,  -0.5f,  0.0f};
    vertices[1]={0.5f, -0.5f,  0.0f};
    vertices[2]={-0.5f, 0.5f,  0.0f};
    vertices[3]={0.5f, 0.5f,  0.0f};

    triangle_vertices = new Mesh<float>(vertices,4);

    // Create vertex array from triangle vertices.
    triangle = new VertexArray(triangle_vertices->getData(), 12);

    // Init succeeded!
    return true;
}

/**
 * Animation loop
 */
void Renderer::loop(){
    // controls annimation loop
    bool close = false;

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
                    switch (event.window.event){
                        case SDL_WINDOWEVENT_RESIZED:
                            //window was resized
                            resolution.first = event.window.data1;
                            resolution.second = event.window.data2;
                            SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_DEBUG);
                            SDL_Log("Window was resized. Size should be %ix%i",event.window.data1,event.window.data2);
                            break;
                    }
                    break;
            }
        }

        const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);

        if(keyboard_state_array[SDL_SCANCODE_W]||keyboard_state_array[SDL_SCANCODE_UP]){
            triangle_vertices->moveY(0.0325f);
            triangle->updateData(triangle_vertices->getData(),12*sizeof(float),0);
        }
        if(keyboard_state_array[SDL_SCANCODE_S]||keyboard_state_array[SDL_SCANCODE_DOWN]){
            triangle_vertices->moveY(-0.0325f);
            triangle->updateData(triangle_vertices->getData(),12*sizeof(float),0);
        }
        if(keyboard_state_array[SDL_SCANCODE_A]||keyboard_state_array[SDL_SCANCODE_LEFT]){
            triangle_vertices->moveX(-0.0325f);
            triangle->updateData(triangle_vertices->getData(),12*sizeof(float),0);
        }
        if(keyboard_state_array[SDL_SCANCODE_D]||keyboard_state_array[SDL_SCANCODE_RIGHT]){
            triangle_vertices->moveX(0.0325f);
            triangle->updateData(triangle_vertices->getData(),12*sizeof(float),0);
        }

        nextFrame();

        // calculates to 60 fps
        SDL_Delay(1000 / 60);
    }
}

/**
 * undo everything in setup
 */
void Renderer::destruct() {
    //empty, because everything works with destructors
}


Renderer::Renderer() {
    if(!init()) exit(-1);
    setup();
    loop();
}
Renderer::~Renderer() {
    destruct();
    //Follow with undoing everything inn init
    SDL_GL_DeleteContext(GL_Context);
    //destroy window
    SDL_DestroyWindow(win);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render()
{
    shader->Activate();
    triangle->Draw(GL_TRIANGLE_STRIP);
}

void Renderer::Present()
{
    SDL_GL_SwapWindow(win);
}

const std::pair<Sint32, Sint32> &Renderer::getResolution() const {
    return resolution;
}



