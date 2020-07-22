#include "Scene.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include "../../Object/Player.h"
#include "../../OpenGL/Shader.h"
#include "../Renderer.h"
#include "../../Callback/Window.h"
#include "../../Callback/Keyboard.h"
#include "MenuScene.h"
#include <glfreetype/TextRenderer.hpp>
#include <cstdlib>

void Scene::Activate(){
    activeScene.push(this);//set this as the now active scene
}
void Scene::run(GLFWwindow* win){
    Scene::win=win;
    shader=new Shader("resources/Simple.vert", "resources/Simple.frag");

    auto exitHandler = [](){
        while (!activeScene.empty()){
            std::cerr<<"This should only print once on exiting!\n";
            delete activeScene.top();
            activeScene.pop();
        }
        delete shader;
    };
    std::atexit(exitHandler);
    std::at_quick_exit(exitHandler);

    //Define callbacks:
    glfwSetKeyCallback(win,Callback::keyCallback);
    glfwSetFramebufferSizeCallback(win,Callback::framebufferSizeCallback);
    glfwSetWindowMaximizeCallback(win,Callback::maximiseCallback);

    unsigned int size = activeScene.size();
    if(size==0) throw std::runtime_error("No Scene to display!");
    activeScene.top()->setup();
    while (!terminate){
        if (activeScene.empty()){
            break;
        }else if(size<activeScene.size()){
            activeScene.top()->setup();
            size=activeScene.size();
        } else if (size>activeScene.size()){
            size=activeScene.size();
        }
        Scene* scene = activeScene.top();

        Clear();
        glfwPollEvents();
        scene->loop();
        Present();

        if(glfwWindowShouldClose(win)){
            if(scene->deleteOnExit) delete scene;
            activeScene.pop();
            glfwSetWindowShouldClose(win,false);
        }
    }
    //unnecessary, because the function will run at exit!
//    exitHandler();
}
Scene& Scene::getScene(){
#ifndef NDEBUG
    if(activeScene.empty()) std::cerr<<"No more Scenes in queue! This will in a function call on NULLPTR. That may or may not be fatal!"<<std::endl;
#endif
    return *activeScene.top();
}
/**
* This should prepare everything, for displaying stuff
*/
void Scene::setup() {
    //Init objects
    meshes.resize(2);
    meshes[0]=new Face("resources/cube.stl", FILE_TYPE::STL, GL_STATIC_DRAW);
    meshes[1]=new Face("resources/cube.stl", FILE_TYPE::STL, GL_STATIC_DRAW);
//    meshes[1]=new Face(vertices, n, color);
    meshes[1]->moveY(0.5);

    player = new Player("resources/cube.stl", FILE_TYPE::STL, GL_STATIC_DRAW);
    player->moveZ(-0.6f);

    //Init Font
    our_font.init("resources/FreeSans.ttf", 25 /* size */);
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
    delete player;
}

Shader& Scene::getShader() {
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