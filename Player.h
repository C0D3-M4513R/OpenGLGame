#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H


#include <iostream>
#include "Face.h"
#include "Camera.h"

class Player:public Face {
    public:
        Player(glm::vec3* vertexData, unsigned int vertexSize,glm::vec3 origin = {0, 0, 0},glm::vec3 camOffset={0,0,0}, GLenum modePar=defaultMode,GLenum type = GL_STATIC_DRAW);
        Player(glm::vec3* vertexData, unsigned int size,glm::vec3* colorData,glm::vec3 origin = {0, 0, 0},glm::vec3 camOffset={0,0,0}, GLenum modePar=defaultMode,GLenum type = GL_STATIC_DRAW);
        Player(const char* filePath,FILE_TYPE=STL,GLenum drawType = GL_STATIC_DRAW,glm::vec3 origin={0,0,0},glm::vec3 camOffset={0,0,0});
        /**
        * Gets the active Player Object.
        *
        * @param object A Face object, which will represent the Player
        * @return A nullable pointer to the Player Object
        */
        static Player* getPlayer(){
            return Player::instance;
        }

        void setActivePlayer();

        [[nodiscard]] const Camera& getCamera() const;
    private:
        void positionUpdateCallback();
        Player();
        inline static Player* instance=nullptr;
        Camera* cam = nullptr;
        glm::vec3 camOffset;
};


#endif //GAME_PLAYER_H
