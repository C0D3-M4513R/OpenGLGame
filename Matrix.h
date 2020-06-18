#ifndef GAME_MATRIX_H
#define GAME_MATRIX_H

#include "Vertex.h"
template<typename T>
class Vertex;

template<typename T>
class Matrix {
public:
    //Constructors
    Matrix(const unsigned int rows,const unsigned cols):Matrix(rows,cols, nullptr){};
    Matrix(const unsigned int rows,const unsigned cols,T** data):rows(rows),cols(cols),data(data){};
    Matrix(Vertex<T> vertex);
    ~Matrix();

    //Matrix generators
    static Matrix<float> rx(T amount);
    static Matrix<float> ry(T amount);
    static Matrix<float> rz(T amount);

    //Operator Overloads
    Matrix<T> operator*(const Matrix<T>& matrix) const;

    T* operator[](unsigned int index);
    const T* operator[](unsigned int index) const;

    //Getters/Setters
    [[nodiscard]] unsigned int getRows() const;
    [[nodiscard]] unsigned int getCols() const;
    void setData(T** matrixData){data=matrixData;};
    static T** arrayinit(unsigned int row, unsigned cols);
private:
    unsigned int rows, cols;
    T** data;
};

#include "Matrix.tpp"
#endif //GAME_MATRIX_H
