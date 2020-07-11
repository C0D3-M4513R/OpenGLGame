
#ifndef GAME_LICENSESCENE_H
#define GAME_LICENSESCENE_H


#include "StartScene.h"
#include <vector>
#include <glfreetype/TextRenderer.hpp>

class LicenseScene: public StartScene {
public:
    LicenseScene(GLFWwindow* win):StartScene(win){};

    void up() override;
    void down() override;
    void exit() override;
    void enter() override;
protected:
    void setup() override;
    void loop() override;

    static void scrollCallback(GLFWwindow* window, double x, double y);
private:
    inline static constexpr unsigned const int n = 4;
    inline static char const*const arr[n] = {"glew","glfw","glm","STLParser"};
    inline static unsigned int active = 0;
    inline static int selected = -1;
    inline static unsigned int scroll = 0;
    std::vector<glfreetype::font_data> fonts;
};


#endif //GAME_LICENSESCENE_H
