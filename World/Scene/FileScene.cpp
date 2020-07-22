#include "FileScene.h"
#include "../../Utility.h"
#include "../Renderer.h"
#include <cstring>

void FileScene::up(int mods){
    if(mods!=0&&mods&GLFW_MOD_SHIFT) right();
    scroll[0]=std::max(scroll[0]-(int)scrollAmount,0);
}
void FileScene::down(int mods){
    if(mods!=0&&mods&GLFW_MOD_SHIFT) left();
    scroll[0]+=scrollAmount;
}
void FileScene::left(int mods){
    scroll[1]=std::min(scroll[1]+(int)scrollAmount,0);
}
void FileScene::right(int mods){
    scroll[1]-=scrollAmount;
}
void FileScene::setup() {
    glfwSetScrollCallback(win,[](GLFWwindow* win,double x,double  y){
        bool shift = glfwGetKey(win,GLFW_KEY_LEFT_SHIFT)|glfwGetKey(win,GLFW_KEY_RIGHT_SHIFT);
        if(y==-1&&!shift)Scene::getScene().up();
        else if(y==-1&&shift)Scene::getScene().left();
        else if(y==1&&!shift)Scene::getScene().down();
        else if(y==1&&shift)Scene::getScene().right();
        if(x==-1)Scene::getScene().left();
        else if(x==1)Scene::getScene().right();
    });
    if(fonts == nullptr){
        fonts=new glfreetype::font_data[2]{glfreetype::font_data(),glfreetype::font_data()};
        fonts[0].init("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 50 /* size */);
        fonts[1].init("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 10 /* size */);
    }
}

void FileScene::loop(){
    //Print the title name
    glfreetype::print(fonts[0],(float)Renderer::getResolutionX()/2-strlen(title)*50*.7f+scroll[1],Renderer::getResolutionY()-60.f+scroll[0],title);
    //Print titleAddition after
    glfreetype::print(fonts[0],(float)Renderer::getResolutionX()/2+scroll[1],Renderer::getResolutionY()-60.f+scroll[0],titleAddition);
    //Print License text in smaller font
    glfreetype::print(fonts[1],scroll[1],Renderer::getResolutionY()-(float)Renderer::getResolutionY()/3+scroll[0],fileContents);
}

FileScene::FileScene(char const*const file,char const*const titleAddition)
:Scene(),fonts(nullptr),fileContents(Utility::readFile(file)),titleAddition(titleAddition){

    std::string string = file;
    unsigned int slash = string.find_last_of('/')+1;
    unsigned int dot = string.find_last_of('.');
    string=string.substr(slash,dot-slash);//Get only file name
    //This Code above assumes. that the file is named something like */fileName.[^/.]
    title=new char[dot-slash+1];
    std::strncpy(title,file+slash,dot-slash);
    title[dot-slash]='\000';
}

FileScene::~FileScene() {
    if(fonts!= nullptr){
        fonts[0].clean();
        fonts[1].clean();
        delete [] fonts;
    }
    delete [] title;
}