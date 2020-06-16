#pragma once
#ifndef GAME_FACE_H
#define GAME_FACE_H

#include <utility>
#include "VertexArray.h"
#include "Vertex.h"

template<typename T>
class Face{
    public:
        Face(Vertex<T>* vertexData, unsigned int vertexSize, Vertex<T> origin = {0, 0, 0});
        Face(Vertex<T>* vertexData, unsigned int size, Vertex<T>* colorData,Vertex<T> origin = {0, 0, 0});
        Face(Vertex<T>* vertexData, unsigned int vertexSize, Vertex<T>* colorData,unsigned int colorSize,Vertex<T> origin = {0, 0, 0});

        void moveX(T amount) {move(0,amount);};
        void moveY(T amount) {move(1,amount);};
        void moveZ(T amount) {move(2,amount);};
        void Draw(GLenum mode = GL_TRIANGLE_STRIP);
    private:
        //you shouldn't need those methods

        //all directions accessible from public methods
        void move(uint8_t direction,T amount);
        //why manually request a Offset recalculation
        void recalculateOffset();
        //why expose data? individual vertices shouldn't be needed to be manipulated
        T* getData();

        VertexArray* triangle;
        void updateVA(int mode=0);

        const unsigned int vertexSize;
        const Vertex<T>* vertexData;

        const unsigned int colorSize;
        Vertex<T>* colorData;

        //Max value in a direction
        //Direction is in the following order x,-x,y,-y,z,-z
        T offset[6] ;
        Vertex<T> origin;

};
#include "Face.tpp"

#endif //GAME_FACE_H
