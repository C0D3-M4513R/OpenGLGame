#include "Keyboard.h"
#include "../Object/Player.h"
#include "../World/Renderer.h"
#include <GLFW/glfw3.h>

void Callback::keyCallback([[maybe_unused]]GLFWwindow* window, [[maybe_unused]]int key, [[maybe_unused]] int scancode, [[maybe_unused]]int action, [[maybe_unused]] int mods) {
    switch (action) {
        default:
            [[fallthrough]];
        case GLFW_PRESS:
            [[fallthrough]];
        case GLFW_REPEAT:
            switch (key) {
                case GLFW_KEY_ESCAPE:
                    if(glfwGetWindowMonitor(window) != nullptr) {
                        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetWindowMonitor(window));
                        //Note: The height and width 1 are imaginary. They just can't be <1. Else the Window resize fails.
                        //Note: The width and height will be applied if I called glfwRestoreWindow.
                        glfwSetWindowMonitor(window, nullptr,0,0,1,1,mode->refreshRate);
                    }
                    else glfwSetWindowShouldClose(window, true);
                    break;
                case GLFW_KEY_W:
                    [[fallthrough]];
                case GLFW_KEY_UP:
                    Player::getPlayer()->moveY(0.1f);
                    break;
                case GLFW_KEY_S:
                    [[fallthrough]];
                case GLFW_KEY_DOWN:
                    Player::getPlayer()->moveY(-0.1f);
                    break;
                case GLFW_KEY_A:
                    [[fallthrough]];
                case GLFW_KEY_LEFT:
#if defined(__MINGW32__) || defined(__MINGW64__)
                    Player::getPlayer()->moveX(-0.1f);
#else
                    Player::getPlayer()->moveX(0.1f);
#endif
                    break;
                case GLFW_KEY_D:
                    [[fallthrough]];
                case GLFW_KEY_RIGHT:
#if defined(__MINGW32__) || defined(__MINGW64__)
                    Player::getPlayer()->moveX(0.1f);
#else
                    Player::getPlayer()->moveX(-0.1f);
#endif
                    break;
            }
            break;
        case GLFW_RELEASE:
            break;
    }
}