#ifndef GAME_MENUSCENE_H
#define GAME_MENUSCENE_H
#include "Scene.h"
#include <glfreetype/TextRenderer.hpp>

class MenuScene : public Scene {
public:
    MenuScene(unsigned const int n,char const*const * arr,Scene** display,unsigned int fontSize=50,char const* fontFile="resources/FreeSans.ttf")
    :Scene(),n(n),arr(arr),active(0),selected(-1),display(display){font=new glfreetype::font_data();font->init(fontFile,fontSize);};
    ~MenuScene() override;

    //Keyboard inputs!
    void up() override;
    void down() override;
    void left() override{};
    void right() override{};
    void exit() override;
    void enter() override;

protected:
    void setup() override;
    void loop() override;

    unsigned const int n;
    char const*const * arr;
    unsigned int active;
    int selected;
    glfreetype::font_data* font;

    Scene** display;
};


#endif //GAME_MENUSCENE_H
