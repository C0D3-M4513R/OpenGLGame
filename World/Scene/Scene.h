
#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <GLFW/glfw3.h>
#include "../../OpenGL/Shader.h"
#include "../../Object/Camera.h"
#include "../../Object/Player.h"
#include <glfreetype/TextRenderer.hpp>

class Scene{
    public:
        virtual ~Scene();
        Scene(GLFWwindow* window):win(window){};
        void Activate();

        [[nodiscard]] const Shader& getShader()const;
        [[nodiscard]] static Scene const& getScene();

        inline static bool terminate = false;
    protected:
        static Scene* getActiveScene() {return activeScene;}
        bool isActiveScene() {return this==activeScene;}

        GLFWwindow* win;

        Shader *shader;
        Player* player;

        std::vector<Face*> meshes;

        glfreetype::font_data our_font;

        virtual void setup();
        virtual void loop();

        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        virtual void Render();


    private:
        inline static Scene* activeScene = nullptr;

        void Clear();
        void Present();
};
#endif //GAME_SCENE_H
