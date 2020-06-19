#ifndef GAME_FACE_H
#define GAME_FACE_H

#include <utility>
#include <glm/glm.hpp>
#include "VertexArray.h"

template<typename T>
class Face{
    public:
        //TODO: scaling, apply move/rotate/scale
        Face(const glm::vec3* vertexData, unsigned int vertexSize, bool dynamic = false, glm::vec3 origin = {0, 0, 0});
        Face(const glm::vec3* vertexData, unsigned int size,glm::vec3* colorData, bool dynamic = false, glm::vec3 origin = {0, 0, 0});
        Face(const glm::vec3* vertexData, unsigned int vertexSize,glm::vec3* colorData,unsigned int colorSize, bool dynamic = false, glm::vec3 origin = {0, 0, 0});
        ~Face();

        void moveX(T amount) {move(0,amount);};
        void moveY(T amount) {move(1,amount);};
        void moveZ(T amount) {move(2,amount);};

        void rotateX(T amount) {rotate(0,amount);};
        void rotateY(T amount) {rotate(1,amount);};
        void rotateZ(T amount) {rotate(2,amount);};

        void Draw(GLenum mode = GL_TRIANGLE_STRIP);
    private:
        //you shouldn't need those methods

        //all directions accessible from public methods
        void move(uint8_t direction,T amount);

        //amount is in radians. It is easier to work with
        void rotate(uint8_t direction,T amount);

        //why manually request a Offset recalculation
        void recalculateOffset();
        //why expose data? individual vertices shouldn't be needed to be manipulated
        T* getData();

        VertexArray<T>* triangle;
        void updateVA(int mode=0);
        bool dynamic;

        const unsigned int vertexSize;
        const glm::vec3*const vertexData;

        const unsigned int colorSize;
        glm::vec3*const colorData;

        //Max value in a direction
        //Direction is in the following order x,-x,y,-y,z,-z
        T offset[6];
        glm::vec3 rotateData={0,0,0};
        glm::vec3 origin;

};

#include "Face.tpp"

#endif //GAME_FACE_H