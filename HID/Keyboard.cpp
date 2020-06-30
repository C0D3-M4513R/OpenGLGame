#include "Keyboard.h"
#include "../Player.h"
#include <GLFW/glfw3.h>

void HID::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (action) {
        default:
            [[fallthrough]];
        case GLFW_PRESS:
            [[fallthrough]];
        case GLFW_REPEAT:
            switch (key) {
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, true);
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
                    Player::getPlayer()->moveX(0.1f);
#else
                    Player::getPlayer()->moveX(-0.1f);
#endif
                    break;
                case GLFW_KEY_D:
                    [[fallthrough]];
                case GLFW_KEY_RIGHT:
#if defined(__MINGW32__) || defined(__MINGW64__)
                    Player::getPlayer()->moveX(-0.1f);
#else
                    Player::getPlayer()->moveX(0.1f);
#endif
                    break;
            }
            break;
        case GLFW_RELEASE:
            break;
    }

}