#pragma once
#ifndef GAME_VERTEX_H
#define GAME_VERTEX_H

#include <string>

template<typename T>
class Matrix;

template<typename T>
struct Vertex{
    Vertex(Matrix<T> matrix):x(matrix[0][0]),y(matrix[1][0]),z(matrix[2][0]){};
    Vertex():x(0),y(0),z(0){};
    Vertex(std::initializer_list<T> c);

    T x;//r
    T y;//g
    T z;//b
    const T operator[](unsigned int index) const;
    T& operator[](unsigned int index);


    [[maybe_unused]] [[nodiscard]] const char* toString() const;
};

#include "Vertex.tpp"
#endif //GAME_VERTEX_H
