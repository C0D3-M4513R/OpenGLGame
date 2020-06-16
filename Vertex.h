#pragma once
#ifndef GAME_VERTEX_H
#define GAME_VERTEX_H

#include <string>

template<typename T>
struct Vertex{
    T x;//r
    T y;//g
    T z;//b
    const T operator[](unsigned int index) const;
    T& operator[](unsigned int index);

    [[maybe_unused]] [[nodiscard]] const char* toString() const;
};

#include "Vertex.tpp"
#endif //GAME_VERTEX_H
