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
    activeScene.push(this);
    setup();
    glfwSetWindowShouldClose(win,false);
    // annimation loop
    while (!(glfwWindowShouldClose(win)||terminate)) {
        Clear();

        glfwPollEvents();
        loop();

        Present();
    }
    activeScene.pop();
    delete this;
}
Scene const& Scene::getScene(){
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
    player->setActivePlayer();

    //Define callbacks:
    glfwSetKeyCallback(win,keyCallback);
    glfwSetFramebufferSizeCallback(win,Callback::framebufferSizeCallback);
    glfwSetWindowMaximizeCallback(win,Callback::maximiseCallback);
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

void Scene::keyCallback([[maybe_unused]]GLFWwindow* window, [[maybe_unused]]int key, [[maybe_unused]] int scancode, [[maybe_unused]]int action, [[maybe_unused]] int mods) {
    switch (action) {
        default:
            [[fallthrough]];
        case GLFW_PRESS:
            [[fallthrough]];
        case GLFW_REPEAT:
            switch (key) {
                case GLFW_KEY_ESCAPE:
                    if(glfwGetWindowMonitor(window) != nullptr) {
                        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetWindowMonitor(window));
                        //Note: The height and width 1 are imaginary. They just can't be <1. Else the Window resize fails.
                        //Note: The width and height will be applied if I called glfwRestoreWindow.
                        glfwSetWindowMonitor(window, nullptr,0,0,1,1,mode->refreshRate);
                    }
                    else glfwSetWindowShouldClose(window, true);
                    break;
                case GLFW_KEY_W:
                    [[fallthrough]];
                case GLFW_KEY_UP:
                    Player::getPlayer()->moveY(0.1f);
                    break;
                case GLFW_KEY_S:
                    [[fallthrough]];
                case GLFW_KEY_DOWN:
                    Player::getPlayer()->moveY(-0.1f);
                    break;
                case GLFW_KEY_A:
                    [[fallthrough]];
                case GLFW_KEY_LEFT:
#if defined(__MINGW32__) || defined(__MINGW64__)
                    Player::getPlayer()->moveX(-0.1f);
#else
                    Player::getPlayer()->moveX(0.1f);
#endif
                    break;
                case GLFW_KEY_D:
                    [[fallthrough]];
                case GLFW_KEY_RIGHT:
#if defined(__MINGW32__) || defined(__MINGW64__)
                    Player::getPlayer()->moveX(0.1f);
#else
                    Player::getPlayer()->moveX(-0.1f);
#endif
                    break;
            }
            break;
        case GLFW_RELEASE:
            break;
    }
}