#ifndef GAME_FACE_H
#define GAME_FACE_H

#include <utility>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/detail/qualifier.hpp>
#include "VertexArray.h"

class Face{
    public:
        //TODO: scaling, apply move/rotate/scale
        Face(const glm::vec3* vertexData, unsigned int vertexSize, bool dynamic = false, glm::vec3 origin = {0, 0, 0});
        Face(const glm::vec3* vertexData, unsigned int size,glm::vec3* colorData, bool dynamic = false, glm::vec3 origin = {0, 0, 0});
        Face(const glm::vec3* vertexData, unsigned int vertexSize,glm::vec3* colorData,unsigned int colorSize, bool dynamic = false, glm::vec3 origin = {0, 0, 0});
        ~Face();

        void moveX(float amount) {move(0,amount);};
        void moveY(float amount) {move(1,amount);};
        void moveZ(float amount) {move(2,amount);};

        //amount is in radians. It is easier to work with
        void rotate(glm::vec3 amount);

        void Draw(GLenum mode = GL_TRIANGLE_STRIP);
    private:
        //you shouldn't need those methods

        //all directions accessible from public methods
        void move(uint8_t direction,float amount);



        //why manually request a Offset recalculation
        void recalculateOffset();
        //why expose data? individual vertices shouldn't be needed to be manipulated
        float* getData();

        VertexArray* vertexArray;
        void updateVA(int mode=0);
        bool dynamic;

        const unsigned int vertexSize;
        const glm::vec3*const vertexData;

        const unsigned int colorSize;
        glm::vec3*const colorData;

        //Max value in a direction
        //Direction is in the following order x,-x,y,-y,z,-z
        float offset[6];
        glm::mat4x4 rotation;
        glm::vec3 origin;
};
#endif //GAME_FACE_H