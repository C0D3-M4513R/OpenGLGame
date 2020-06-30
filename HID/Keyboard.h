#ifndef GAME_KEYBOARD_H
#define GAME_KEYBOARD_H

#include <GLFW/glfw3.h>

namespace HID {
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
//    void keyCallback(GLFWwindow*,int,int,int,int);
};


#endif //GAME_KEYBOARD_H
