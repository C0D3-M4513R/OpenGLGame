#include "StartScene.h"
#include "../../Callback/Window.h"
#include "../Renderer.h"
#include "../../Utility.h"
#include "LicenseScene.h"
#include <glfreetype/TextRenderer.hpp>

void StartScene::setup(){
    // Load shader.
    shader = new Shader("resources/Simple.vert", "resources/Simple.frag");
    if (!shader->IsGood()) { return ; }
    std::cout<<"Good Shader\n";

    fonts.resize(2);
    fonts[0].init("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 50 /* size */);
    fonts[1].init("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 20 /* size */);
    //Define callbacks:
    glfwSetKeyCallback(win,StartScene::keyCallback);
    glfwSetFramebufferSizeCallback(win,Callback::framebufferSizeCallback);
    glfwSetWindowMaximizeCallback(win,Callback::maximiseCallback);
    //Actually exit in this window!
    glfwSetWindowCloseCallback(win,[]([[maybe_unused]] GLFWwindow* window){terminate=true;});

}
void StartScene::loop(){
    switch (selected){
        case -1:
            for(unsigned int i = 0; i < n;i++){
                if(i==active)glColor3ub(0xff,0xff,0);
                else glColor3ub(0xff,0xff,0xff);
                glfreetype::print(fonts[0],(float)Renderer::getResolutionX()/3,Renderer::getResolutionY()-(float)Renderer::getResolutionY()/3-i*70,arr[i]);
            }
            break;
        default:
            [[fallthrough]];
        case 0:
            //terminate this instance
            glfwSetWindowShouldClose(win, true);
            break;
        case 1:
            //TODO: Replace with actual options!
            glfreetype::print(fonts[0],10,Renderer::getResolutionY()-(float)Renderer::getResolutionY()/3,"No Options available yet!");
            break;
        case 2:
            {
                char* credits=Utility::readFile("resources/Credits.txt",credits);
                glfreetype::print(fonts[0],(float)Renderer::getResolutionX()/2-50,Renderer::getResolutionY()-60.f,"Credits:");
                glfreetype::print(fonts[1],0,Renderer::getResolutionY()-(float)Renderer::getResolutionY()/3,credits);
                delete [] credits;
                break;
            }
        case 3:
            {
                (new LicenseScene(win))->Activate();
                break;
            }
        case 4:
            {
                terminate=true;
            }
    }
    glColor3ub(0xff,0xff,0xff);
}

void StartScene::keyCallback([[maybe_unused]]GLFWwindow* window, [[maybe_unused]]int key, [[maybe_unused]] int scancode, [[maybe_unused]]int action, [[maybe_unused]] int mods) {
    switch (action) {
        default:
            [[fallthrough]];
        case GLFW_PRESS:
            [[fallthrough]];
        case GLFW_REPEAT:
            switch (key) {
                case GLFW_KEY_ESCAPE: {
                    bool isFullscreen = glfwGetWindowMonitor(window) != nullptr;
#ifndef NDEBUG
                    std::cout << "Escape pressed! Exiting " << (isFullscreen ? "Fullscreen" : "Scene") << ".\n";
#endif
                    if(selected!=-1) {
                        selected=-1;
                        break;
                    }

                    if (isFullscreen) {
                        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetWindowMonitor(window));
                        //Note: The height and width 1 are imaginary. They just can't be <1. Else the Window resize fails.
                        //Note: The width and height would be applied if I called glfwRestoreWindow.
                        glfwSetWindowMonitor(window, nullptr, 0, 0, 1, 1, mode->refreshRate);
                    } else glfwSetWindowShouldClose(window, terminate = true);
                    break;
                }
                case GLFW_KEY_W:
                    [[fallthrough]];
                case GLFW_KEY_UP:
#ifndef NDEBUG
                    std::cout<<"UP active="<<active<<".\n";
#endif
                    if(active>0)[[likely]]active--;
                    break;
                case GLFW_KEY_S:
                    [[fallthrough]];
                case GLFW_KEY_DOWN:
#ifndef NDEBUG
                    std::cout<<"DOWN active="<<active<<".\n";
#endif
                    if(active<n-1)[[likely]]active++;
                    break;
                case GLFW_KEY_KP_ENTER:
                    [[fallthrough]];
                case GLFW_KEY_ENTER:
#ifndef NDEBUG
                    std::cout<<"Selected active="<<active<<".\n";
#endif
                    selected=(int)active;
                    break;
            }
            break;
        case GLFW_RELEASE:
            break;
    }
}