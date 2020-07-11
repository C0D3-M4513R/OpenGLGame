
#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <GLFW/glfw3.h>
#include "../../OpenGL/Shader.h"
#include "../../Object/Player.h"
#include <glfreetype/TextRenderer.hpp>
#include <stack>

class Scene{
    public:
        virtual ~Scene();
        Scene():player(nullptr){};
        void Activate();
        static void run(GLFWwindow* win);

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
        static Scene* getActiveScene() {return activeScene.top();}
        bool isActiveScene() {return activeScene.top()==this;}

        inline static GLFWwindow* win = nullptr;

        Shader* shader;
        Player* player;

        std::vector<Face*> meshes;

        virtual void setup();
        virtual void loop();

        virtual void Render();
    private:
        inline static std::stack<Scene*> activeScene;

        static void  Clear();
        static void Present();

        //Used with debug implementation
        glfreetype::font_data our_font;
};
#endif //GAME_SCENE_H
