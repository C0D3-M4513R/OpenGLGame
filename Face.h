#pragma once
#ifndef GAME_FACE_H
#define GAME_FACE_H

#include <utility>
#include "Vertex.h"

template<typename T>
class Face{
    public:
        Face(Vertex<T>* data, unsigned int size, Vertex<T> origin = {0, 0, 0});

        T* getData();
        void moveX(T amount) {move(0,amount);};
        void moveY(T amount) {move(1,amount);};
        void moveZ(T amount) {move(2,amount);};
    private:
        void move(uint8_t direction,T amount);
        void recalculateOffset();

        const int size;
        Vertex<T>* data;

        //Max value in a direction
        //Direction is in the following order x,-x,y,-y,z,-z
        T offset[6] ;
        Vertex<T> origin;

};
#include "Face.tpp"

#endif //GAME_FACE_H
