#include "StartScene.h"
#include "../../Callback/Window.h"
#include "../Renderer.h"
#include "../../Utility.h"
#include "LicenseScene.h"
#include <glfreetype/TextRenderer.hpp>

void StartScene::setup(){
    fonts.resize(2);
    fonts[0].init("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 50 /* size */);
    fonts[1].init("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 20 /* size */);
}
void StartScene::loop(){
    switch (selected){
        case -1:
            for(unsigned int i = 0; i < n;i++){
                if(i==active)glColor3ub(0xff,0xff,0);
                else glColor3ub(0xff,0xff,0xff);
                glfreetype::print(fonts[0],(float)Renderer::getResolutionX()/3,Renderer::getResolutionY()-(float)Renderer::getResolutionY()/3-i*70,arr[i]);
            }
            break;
        default:
            [[fallthrough]];
        case 0:
            //Switch to the game
            (new Scene())->Activate();
            selected=-1;//Prevent an accidental loop from happening
            break;
        case 1:
            //TODO: Replace with actual options!
            glfreetype::print(fonts[0],10,Renderer::getResolutionY()-(float)Renderer::getResolutionY()/3,"No Options available yet!");
            break;
        case 2:
            {
                char* credits=Utility::readFile("resources/Credits.txt",credits);
                glfreetype::print(fonts[0],(float)Renderer::getResolutionX()/2-50,Renderer::getResolutionY()-60.f,"Credits:");
                glfreetype::print(fonts[1],0,Renderer::getResolutionY()-(float)Renderer::getResolutionY()/3,credits);
                delete [] credits;
                break;
            }
        case 3:
            {
                (new LicenseScene())->Activate();
                selected=-1;//Prevent an accidental loop from happening
                break;
            }
        case 4:
            {
                terminate=true;
            }
    }
    glColor3ub(0xff,0xff,0xff);
}

void StartScene::up(){
    if(active>0&&selected==-1)active--;
#ifndef NDEBUG
    std::cout<<"UP active="<<active<<".\n";
#endif
}
void StartScene::down(){
    if(active<n-1&&selected==-1)active++;
#ifndef NDEBUG
    std::cout<<"DOWN active="<<active<<".\n";
#endif
}
void StartScene::enter() {
#ifndef NDEBUG
    std::cout<<"Selected active="<<active<<".\n";
#endif
    if(selected==-1)selected=(int)active;
}