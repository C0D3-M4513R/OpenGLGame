#include <iostream>
#include <GLFW/glfw3.h>
#include "../Object/Player.h"
#include "../OpenGL/Shader.h"
#include "../Object/Camera.h"
#include "Renderer.h"
#include "Scene.h"
#include <glfreetype/TextRenderer.hpp>
#include "../Callback/Window.h"

void Scene::Activate(){
    if(activeScene!=nullptr){
        throw std::runtime_error("There is still a Scene active! Not Activating this one, as it may lead to funny results.");
    }
    activeScene=this;
    setup();
    loop();
    activeScene=nullptr;
    delete this;
}
Scene const& Scene::getScene(){
    return *activeScene;
}
/**
* This should prepare everything, for displaying stuff
*/
void Scene::setup() {
    // Load shader.
    shader = new Shader("resources/Simple.vert", "resources/Simple.frag");
    if (!shader->IsGood()) { return ; }
    std::cout<<"Good Shader\n";


    //Those arrays should be deleted by Face!
    const unsigned int n = 4;
    auto *vertices = new glm::vec3[n];
    vertices[0] = {-0.5f, -0.5f, 0.0f};
    vertices[1] = {0.5f, -0.5f, 0.0f};
    vertices[2] = {-0.5f, 0.5f, 0.0f};
    vertices[3] = {0.5f, 0.5f, 0.0f};
    auto *color = new glm::vec3[n];
    color[0] = {0.f, 1.f, 0.0f};
    color[1] = {0.f, 0.f, 1.0f};
    color[2] = {1.f, 0.f, 0.0f};
    color[3] = {1.f, 1.f, 1.0f};

    //Init objects
    meshes.resize(2);
    meshes[0]=new Face("resources/cube.stl", FILE_TYPE::STL, GL_STATIC_DRAW);
    meshes[1]=new Face(vertices, n, color);

    player = new Player("resources/cube.stl", FILE_TYPE::STL, GL_STATIC_DRAW);
    player->setActivePlayer();
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

            glfreetype::font_data our_font;
            our_font.init("/usr/share/fonts/truetype/freefont/FreeSans.ttf", 25 /* size */);

            glm::vec3 rotate = {0.1f, 0.f, 0.f};

            glfwSetFramebufferSizeCallback(win,Callback::framebufferSizeCallback);

            glfwSetWindowMaximizeCallback(win,Callback::maximiseCallback);

            // annimation loop
            while (!glfwWindowShouldClose(win)) {
                Clear();


                glfwPollEvents();
                meshes[0]->rotate(rotate);
                rotate += glm::vec3(0.f, 0.f, 0.01f);
                Render();

                glfreetype::print(our_font,0,Renderer::getResolutionY()-27,"This is a TEST! äöüß");

                Present();
            }
        }
        Scene::~Scene(){
            delete shader;
        }

    const Shader& Scene::getShader() const {
        return *shader;
    }