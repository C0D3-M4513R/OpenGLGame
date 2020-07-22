#ifndef GAME_FILESCENE_H
#define GAME_FILESCENE_H
#include <string>
#include "Scene.h"

constexpr inline static unsigned int const scrollAmount=50;
class FileScene : public Scene {
    public:
        FileScene(char const*const file,char const*const titleAddition="");
        FileScene* setFontData(glfreetype::font_data* fontData){fonts=fontData; return this;}
        ~FileScene() override;

    protected:
        void setup() override;
        void loop() override;

        void up(int mods=0) override;
        void down(int mods=0) override;
        void left(int mods=0) override;
        void right(int mods=0) override;
    private:
        std::string fileContents=nullptr;
        char* title=nullptr;
        char const*const titleAddition;
        int scroll[2]={0,0};//Y=0,X=1

        glfreetype::font_data* fonts=nullptr;

};


#endif //GAME_FILESCENE_H
