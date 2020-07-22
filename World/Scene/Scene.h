
#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <GLFW/glfw3.h>
#include "SceneDeleteOnExit.h"
#include "../../OpenGL/Shader.h"
#include "../../Object/Player.h"
#include <glfreetype/TextRenderer.hpp>
#include <stack>

class Scene : public SceneDeleteOnExit{
    public:
        virtual ~Scene();
        Scene():player(nullptr){};
        void Activate();
        static void run(GLFWwindow* win);

        [[nodiscard]] static Shader& getShader();
        [[nodiscard]] static Scene& getScene();

        inline static bool terminate = false;

        //keyboard callbacks
        virtual void up(int mods=0);
        virtual void down(int mods=0);
        virtual void left(int mods=0);
        virtual void right(int mods=0);
        virtual void exit(int mods=0);
        inline virtual void enter(int mods=0) {};
    protected:
        static Scene* getActiveScene() {return activeScene.top();}
        bool isActiveScene() {return activeScene.top()==this;}


        inline static GLFWwindow* win = nullptr;//init to nullptr to prevent undefined reference. Initialised in Scene::run

        inline static Shader* shader= nullptr;//init to nullptr to prevent undefined reference. Initialised in Scene::run
        Player* player;//Initialization is Implementation defined!
                       //Maybe make this a std::optional?

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
