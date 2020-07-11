#include <iostream>
#include <GLFW/glfw3.h>
#include "../../Object/Player.h"
#include "../../OpenGL/Shader.h"
#include "../../Object/Camera.h"
#include "../Renderer.h"
#include "Scene.h"
#include <glfreetype/TextRenderer.hpp>
#include "../../Callback/Window.h"
#include "../../Callback/Keyboard.h"

void Scene::Activate(){
    activeScene.push(this);//set this as the now active scene
    bool terminateOld = terminate; //save state for later!

    //Define callbacks:
    glfwSetKeyCallback(win,Callback::keyCallback);
    glfwSetFramebufferSizeCallback(win,Callback::framebufferSizeCallback);
    glfwSetWindowMaximizeCallback(win,Callback::maximiseCallback);

    if(!terminate) setup();
    glfwSetWindowShouldClose(win,false);
    // annimation loop
    while (!(glfwWindowShouldClose(win)||terminate)) {
        Clear();

        glfwPollEvents();
        loop();

        Present();
    }
    activeScene.pop();
    if(!terminateOld) delete this;
}
Scene& Scene::getScene(){
#ifndef NDEBUG
    if(activeScene.empty()) std::cerr<<"No more Scenes in queue! This will in a function call on NULLPTR. That may or may not be fatal!"<<std::endl;
#endif
    return *activeScene.front();
}
/**
* This should prepare everything, for displaying stuff
*/
void Scene::setup() {
    // Load shader.
    shader = new Shader("resources/Simple.vert", "resources/Simple.frag");
    if (!shader->IsGood()) { return ; }
    std::cout<<"Good Shader\n";

    //Init objects
    meshes.resize(2);
    meshes[0]=new Face("resources/cube.stl", FILE_TYPE::STL, GL_STATIC_DRAW);
    meshes[1]=new Face("resources/cube.stl", FILE_TYPE::STL, GL_STATIC_DRAW);
//    meshes[1]=new Face(vertices, n, color);
    meshes[1]->moveY(0.5);

    player = new Player("resources/cube.stl", FILE_TYPE::STL, GL_STATIC_DRAW);
    player->moveZ(-0.6f);

    //Init Font
    our_font.init("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 25 /* size */);
    //Set Time to right before loop start!
    glfwSetTime(0);
}

void Scene::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::Render() {
    Player::getPlayer()->Draw();
    Face::DrawAll();
}

void Scene::Present() {
    glfwSwapBuffers(win);
}


/**
* Animation loop
*/
void Scene::loop() {
    glm::vec3 rotate = {0.1f, 0.f, glfwGetTime()*0.01f};
    meshes[0]->rotate(rotate);
    Render();

    glfreetype::print(our_font,0,Renderer::getResolutionY()-27,"This is a TEST! äöüß");
}
Scene::~Scene(){
    delete shader;
}

const Shader& Scene::getShader() const {
return *shader;
}

//Keyboard Callbacks
void Scene::exit() {
    bool isFullscreen = glfwGetWindowMonitor(win) != nullptr;
#ifndef NDEBUG
    std::cout << "Escape pressed! Exiting " << (isFullscreen ? "Fullscreen" : "Scene") << ".\n";
#endif
    if (isFullscreen) {
        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetWindowMonitor(win));
        //Note: The height and width 1 are imaginary. They just can't be <1. Else the Window resize fails.
        //Note: The width and height will be applied if I called glfwRestoreWindow.
        glfwSetWindowMonitor(win, nullptr,0,0,1,1,mode->refreshRate);
    }
    else glfwSetWindowShouldClose(win, true);
}
void Scene::up(){
    if(player!=nullptr)player->moveY(0.1f);
}
void Scene::down(){
    if(player!=nullptr)player->moveY(-0.1f);
}
void Scene::left(){
    if(player!=nullptr)player->moveX(0.1f);
}
void Scene::right(){
    if(player!=nullptr)player->moveX(-0.1f);
}