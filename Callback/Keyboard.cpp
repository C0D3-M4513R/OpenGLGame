#include "Keyboard.h"
#include "../World/Scene/Scene.h"
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
                    Scene::getScene().exit();
                    break;
                case GLFW_KEY_W:
                    [[fallthrough]];
                case GLFW_KEY_UP:
                    Scene::getScene().up(mods);
                    break;
                case GLFW_KEY_S:
                    [[fallthrough]];
                case GLFW_KEY_DOWN:
                    Scene::getScene().down(mods);
                    break;
                case GLFW_KEY_A:
                    [[fallthrough]];
                case GLFW_KEY_LEFT:
                    Scene::getScene().left(mods);
                    break;
                case GLFW_KEY_D:
                    [[fallthrough]];
                case GLFW_KEY_RIGHT:
                    Scene::getScene().right(mods);
                    break;
                case GLFW_KEY_KP_ENTER:
                    [[fallthrough]];
                case GLFW_KEY_ENTER:
                    Scene::getScene().enter(mods);
                    break;
            }
            break;
        case GLFW_RELEASE:
            break;
    }
}