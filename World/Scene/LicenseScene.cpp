#include <cstring>
#include "LicenseScene.h"
#include "../../Callback/Window.h"
#include "../Renderer.h"
#include "../../Utility.h"

void LicenseScene::setup(){
    // Load shader.
    shader = new Shader("resources/Simple.vert", "resources/Simple.frag");
    if (!shader->IsGood()) { return ; }
    std::cout<<"Good Shader\n";

    fonts.resize(2);
    fonts[0].init("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 50 /* size */);
    fonts[1].init("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 10 /* size */);
    //Define callbacks:
    glfwSetKeyCallback(win,LicenseScene::keyCallback);
    glfwSetFramebufferSizeCallback(win,Callback::framebufferSizeCallback);
    glfwSetWindowMaximizeCallback(win,Callback::maximiseCallback);
    //Actually exit in this window!
    glfwSetWindowCloseCallback(win,[]([[maybe_unused]] GLFWwindow* window){terminate=true;});
    glfwSetScrollCallback(win,LicenseScene::scrollCallback);

}
void LicenseScene::loop(){
    if (selected==-1) {
        for (unsigned int i = 0; i < n; i++) {
            if (i == active)glColor3ub(0xff, 0xff, 0);
            else glColor3ub(0xff, 0xff, 0xff);
            glfreetype::print(fonts[0], (float) Renderer::getResolutionX() / 3,
                              Renderer::getResolutionY() - (float) Renderer::getResolutionY() / 3 - i * 70, arr[i]);
            glColor3ub(0xff,0xff,0xff);
        }
    } else if(selected>=0&&selected<(int)n) {
        //Get License text
        char* license;
        license=Utility::readFile(((std::string)"resources/Licences/").append(arr[selected]).append(".txt").c_str(),license);

        //Print the Library name (no extra variables this way)
        glfreetype::print(fonts[0],(float)Renderer::getResolutionX()/2-strlen(arr[selected])*50*.7f,Renderer::getResolutionY()-60.f+scroll,arr[selected]);
        //Print License after
        glfreetype::print(fonts[0],(float)Renderer::getResolutionX()/2,Renderer::getResolutionY()-60.f+scroll," License:");
        //Print License text in smaller font
        glfreetype::print(fonts[1],0,Renderer::getResolutionY()-(float)Renderer::getResolutionY()/3+scroll,license);

        //deallocate the text, we used.
        //Or else we won't have any space left!
        delete [] license;
    }
}

void LicenseScene::keyCallback([[maybe_unused]]GLFWwindow* window, [[maybe_unused]]int key, [[maybe_unused]] int scancode, [[maybe_unused]]int action, [[maybe_unused]] int mods) {
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
                        scroll=0;
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
                    if(active>0)[[likely]]active--;
#ifndef NDEBUG
                    std::cout<<"UP active="<<active<<".\n";
#endif
                    break;
                case GLFW_KEY_S:
                    [[fallthrough]];
                case GLFW_KEY_DOWN:
                    if(active<n-1)[[likely]]active++;
#ifndef NDEBUG
                    std::cout<<"DOWN active="<<active<<".\n";
#endif
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
void LicenseScene::scrollCallback([[maybe_unused]]GLFWwindow* window, [[maybe_unused]]double x, [[maybe_unused]]double y){
    if((y==-1||scroll>5)&&selected!=-1)scroll-=y*50;
    std::cout<<y<<"\n";
}