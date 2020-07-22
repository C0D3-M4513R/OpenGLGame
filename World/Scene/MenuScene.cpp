#include "MenuScene.h"
#include "../Renderer.h"

MenuScene::~MenuScene() {
    delete [] arr;
    for(unsigned int i = 0; i < n; i++){
        delete display[i];
    }
    delete [] display;
    font->clean();
    delete font;
}

//Keyboard overrides
void MenuScene::exit([[maybe_unused]]int mods){
    if(selected!=-1) {
        selected=-1;
        return;
    }
    Scene::exit();
}
void MenuScene::up([[maybe_unused]]int mods){
    if(active>0&&selected==-1)active--;
#ifndef NDEBUG
    std::cout<<"UP active="<<active<<".\n";
#endif
}
void MenuScene::down([[maybe_unused]]int mods){
    if(active<n-1&&selected==-1)active++;
#ifndef NDEBUG
    std::cout<<"DOWN active="<<active<<".\n";
#endif
}
void MenuScene::enter([[maybe_unused]]int mods) {
#ifndef NDEBUG
    std::cout<<"Selected active="<<active<<".\n";
#endif
    if(selected==-1)selected=(int)active;
}

void MenuScene::setup(){
    for(unsigned int i = 0; i<n; i++){
        if(display[i]==nullptr)continue;
        ((SceneDeleteOnExit*)display[i])->deleteOnExit=false;
    }
}

void MenuScene::loop(){
    if(selected==-1){
        for(unsigned int i = 0; i < n;i++){
            if(i==active)glColor3ub(0xff,0xff,0);
            else glColor3ub(0xff,0xff,0xff);
            glfreetype::print(*font,(float)Renderer::getResolutionX()/3,Renderer::getResolutionY()-(float)Renderer::getResolutionY()/3-i*70,arr[i]);
            glColor3ub(0xff,0xff,0xff);
        }
    } else if(selected>-1&&(unsigned int)selected<n){
        Scene* scene = display[selected];
        selected=-1;
        if(scene==nullptr)exit();
        else scene->Activate();
    } else{
        throw std::runtime_error("Selected is out of bounds! Selected is "+std::to_string(selected)+". Bounds are -1 to "+std::to_string(n)+" exclusive.");
    }
};
