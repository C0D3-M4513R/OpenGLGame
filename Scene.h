
#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "Player.h"

class Scene{
    public:
        ~Scene();
        Scene(GLFWwindow* window):win(window){};
        void Activate();

        [[nodiscard]] const Shader& getShader()const;
        [[nodiscard]] static Scene const& getScene();
    private:
        inline static Scene* activeScene = nullptr;
        GLFWwindow* win;

        Shader *shader;
        Player* player;

        std::vector<Face*> meshes;

        void setup();
        void loop();

        void Clear();
        void Render();
        void Present();
};
#endif //GAME_SCENE_H
