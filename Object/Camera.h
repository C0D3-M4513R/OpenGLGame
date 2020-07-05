#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include <glm/glm.hpp>


class Camera {
public:
    Camera():Camera(glm::vec3(),glm::vec3()){};
    Camera(glm::vec3 look) : Camera(look,glm::vec3()) {};
    Camera(glm::vec3 look,glm::vec3 pos,glm::vec3 upPar = {0,1,0}) : lookAt(look),position(pos),up(upPar) {};
    ~Camera(){};

    [[nodiscard]] glm::mat4 view() const;

//getters/setters
    [[nodiscard]] const glm::vec3 &getLookAt() const {return lookAt;}
    void setLookAt(const glm::vec3 &lookAt) {Camera::lookAt = lookAt;}
    [[nodiscard]] const glm::vec3 &getPosition() const {return position;}
    void setPosition(const glm::vec3 &position) {Camera::position = position;}
    [[nodiscard]] const glm::vec3 &getUp() const {return up;}
    void setUp(const glm::vec3 &up) {Camera::up = up;}
private:
    glm::vec3 lookAt;
    glm::vec3 position;
    glm::vec3 up;
};
#endif //GAME_CAMERA_H
