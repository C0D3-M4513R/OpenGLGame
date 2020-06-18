#ifndef GAME_VERTEX_H
#define GAME_VERTEX_H

#include <string>

#include "Matrix.h"
template<typename T>
class Matrix;

template<typename T>
struct Vertex{
    //TODO: add toString function
    Vertex(Matrix<T> matrix):Vertex(matrix[0][0],matrix[1][0],matrix[2][0]){};
    Vertex():Vertex(0,0,0){};
    Vertex(T x, T y, T z):x(x),y(y),z(z){};
    Vertex(std::initializer_list<T> c);

    T x;//r
    T y;//g
    T z;//b

    const T operator[](unsigned int index) const;
    T& operator[](unsigned int index);

    Vertex<T> operator+(Vertex<T>& vertex) const;
    void operator+=(Vertex<T>& vertex);
};

#include "Vertex.tpp"
#endif //GAME_VERTEX_H
