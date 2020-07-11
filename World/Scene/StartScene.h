#ifndef GAME_STARTSCENE_H
#define GAME_STARTSCENE_H
#include "Scene.h"

class StartScene:public Scene {
public:
    StartScene(GLFWwindow* win):Scene(win){};
protected:
    void setup() override;
    void loop() override;

    //Keyboard inputs!
    void up() override;
    void down() override;
    void left() override{};
    void right() override{};
    void exit() override;
    void enter() override;

private:
    inline static constexpr unsigned const int n = 5;
    inline static char const*const arr[n] = {"Start","Options","Credits","Legal","Quit"};
    inline static unsigned int active = 0;
    inline static int selected = -1;
    std::vector<glfreetype::font_data> fonts;
};


#endif //GAME_STARTSCENE_H
