
#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <GLFW/glfw3.h>
#include "../../OpenGL/Shader.h"
#include "../../Object/Camera.h"
#include "../../Object/Player.h"
#include <glfreetype/TextRenderer.hpp>
#include <queue>

class Scene{
    public:
        virtual ~Scene();
        Scene(GLFWwindow* window):win(window),player(nullptr){};
        void Activate();

        [[nodiscard]] const Shader& getShader()const;
        [[nodiscard]] static Scene& getScene();

        inline static bool terminate = false;

        //keyboard callbacks
        virtual void up();
        virtual void down();
        virtual void left();
        virtual void right();
        virtual void exit();
        inline virtual void enter() {};
    protected:
        static Scene* getActiveScene() {return activeScene.front();}
        bool isActiveScene() {return activeScene.front()==this;}

        GLFWwindow* win;

        Shader *shader;
        Player* player;

        std::vector<Face*> meshes;

        virtual void setup();
        virtual void loop();

        virtual void Render();
    private:
        inline static std::queue<Scene*> activeScene;

        void Clear();
        void Present();

        //Used with debug implementation
        glfreetype::font_data our_font;
};
#endif //GAME_SCENE_H
