#pragma once
#ifndef GAME_VERTEX_H
#define GAME_VERTEX_H

#include <cstdio>
#include <cstdlib>

template<typename T>
struct Vertex{
    T x;
    T y;
    T z;
    T* getData();
    T& operator[](unsigned int index);
};

#include "Vertex.tpp"
#endif //GAME_VERTEX_H
