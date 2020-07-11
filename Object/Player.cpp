#include "Player.h"
#include <glm/vec3.hpp>

Player::Player(glm::vec3* vertexData, unsigned int vertexSize,glm::vec3 origin,glm::vec3 camOffset,GLenum modePar,GLenum type )
:Player(vertexData,vertexSize, nullptr,origin,camOffset,modePar,type){

};

Player::Player(glm::vec3* vertexData, unsigned int size,glm::vec3* colorData,glm::vec3 origin,glm::vec3 camOffset, GLenum modePar,GLenum type)
:Face(vertexData,size,colorData,modePar,type,origin),camOffset(camOffset){
    instance = this;
    draw=false;
    cam = new Camera(origin+camOffset+glm::vec3(0, 0, 1), origin+camOffset);
    positionUpdateCallback();
};
Player::Player(const char* filePath,FILE_TYPE type,GLenum drawType,glm::vec3 origin,glm::vec3 camOffset)
:Face(filePath,type,drawType,origin),camOffset(camOffset){
    instance = this;
    draw=false;
    cam = new Camera(origin+camOffset+glm::vec3(0, 0, 1), origin+camOffset);
    positionUpdateCallback();
};
Player::Player():Face(nullptr,0){
    instance = this;
    draw=false;
    cam = new Camera({0, 0, 1}, origin);
}
void Player::setActivePlayer(){
#ifdef NDEBUG
    instance=this;
#else
    if(instance!=this)
        instance=this;
    else std::cerr<<"This Player is already the active one. Nothing done!\n";
#endif
}
void Player::positionUpdateCallback(){
    auto objectOffset = origin+camOffset;
    objectOffset.z+=offset[4];
    cam->setPosition(objectOffset);
    cam->setLookAt(objectOffset+glm::vec3(0, 0, 1));
}

const Camera& Player::getCamera() const {
    return *cam;
}