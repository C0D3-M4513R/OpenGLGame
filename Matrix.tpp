//
// Created by Timon Kayser on 16.06.2020.
//

#include "Matrix.h"
#include <stdexcept>
#include <cmath>

template<typename T>
Matrix<T>::~Matrix() {
    for (unsigned int i =0; i < rows; i++) {
        delete[] data[i];
    }
    delete[] data;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &matrix) const {
    unsigned int newRows = 0;
    unsigned int newCols = 0;
    unsigned int n = 0;
    bool thisFirst = false;
    if (matrix.cols == rows) {
        thisFirst=true;
        newCols = cols;
        newRows = matrix.rows;
        n=rows;
    }
    else if (matrix.rows == cols){
        newRows = rows;
        newCols = matrix.cols;
        n=cols;
    }
    else throw std::runtime_error("Matrices are not compatible to be multiplied!");

    auto output = Matrix<T>(newRows, newCols, arrayinit(newRows, newCols));
    for (unsigned int i = 0; i < newRows; i++) {
        for (unsigned int j = 0; j < newCols; j++) {
            output.data[i][j]=0;
            for (unsigned int k = 0; k < n; k++) {
                output.data[i][j] += thisFirst?matrix.data[i][k] * data[k][j]:matrix.data[k][j] * data[i][k];
            }
        }
    }

    return output;
}

template<typename T>
unsigned int Matrix<T>::getRows() const {
    return rows;
}

template<typename T>
unsigned int Matrix<T>::getCols() const {
    return cols;
}

template<typename T>
T** Matrix<T>::arrayinit(const unsigned int rows, const unsigned int cols) {
    T** out = new T *[rows];
    for (unsigned int i =0; i <rows ; i++) {
        out[i] = new T[cols];
        for(unsigned int j = 0; j< cols; j++) {
            out[i][j] = 0;
        }
    }
    return out;
}

template<typename T>
T *Matrix<T>::operator[](unsigned int index) {
    return data[index];
}

template<typename T>
const T *Matrix<T>::operator[](unsigned int index) const {
    return data[index];
}

template<typename T>
Matrix<T>::Matrix(Vertex<T> vertex): rows(3), cols(1){
    data = arrayinit(3,1);
    data[0][0]=vertex.x;
    data[1][0]=vertex.y;
    data[2][0]=vertex.z;
}


template<typename T>
Matrix<float> Matrix<T>::rx(T rotation) {
    float** data = arrayinit(3,3);
    data[0][0]=1;
    data[0][1]=0;
    data[0][2]=0;
    data[1][0]=0;
    data[2][0]=0;
    data[1][1]=cos(rotation);
    data[2][2]=data[1][1];
    data[2][1]=sin(rotation);
    data[1][2]=-data[2][1];
#ifndef NDEBUG
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"Matrix rx with rotation=%s",std::to_string(rotation).c_str());
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"%s %s %s",std::to_string(data[0][0]).c_str(),std::to_string(data[0][1]).c_str(),std::to_string(data[0][2]).c_str());
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"%s %s %s",std::to_string(data[1][0]).c_str(),std::to_string(data[1][1]).c_str(),std::to_string(data[1][2]).c_str());
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"%s %s %s",std::to_string(data[2][0]).c_str(),std::to_string(data[2][1]).c_str(),std::to_string(data[2][2]).c_str());
#endif
    return Matrix<float>(3, 3, data);
};

template<typename T>
Matrix<float> Matrix<T>::ry(T rotation) {
    float** data = arrayinit(3,3);
    data[1][1]=1;
    data[1][0]=0;
    data[1][2]=0;
    data[2][1]=0;
    data[0][0]=cos(rotation);
    data[2][2]=data[0][0];
    data[0][2]=sin(rotation);
    data[2][0]=-data[0][2];
#ifndef NDEBUG
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"Matrix rx with rotation=%s",std::to_string(rotation).c_str());
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"%s %s %s",std::to_string(data[0][0]).c_str(),std::to_string(data[0][1]).c_str(),std::to_string(data[0][2]).c_str());
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"%s %s %s",std::to_string(data[1][0]).c_str(),std::to_string(data[1][1]).c_str(),std::to_string(data[1][2]).c_str());
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"%s %s %s",std::to_string(data[2][0]).c_str(),std::to_string(data[2][1]).c_str(),std::to_string(data[2][2]).c_str());
#endif
    return Matrix<float>(3, 3, data);
};

template<typename T>
Matrix<float> Matrix<T>::rz(T rotation) {
    float** data = arrayinit(3,3);
    data[2][2]=1;
    data[2][1]=0;
    data[2][0]=0;
    data[1][2]=0;
    data[0][2]=0;
    data[0][0]=cos(rotation);
    data[1][1]=data[0][0];
    data[1][0]=sin(rotation);
    data[0][1]=-data[1][0];
#ifndef NDEBUG
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"Matrix rx with rotation=%s",std::to_string(rotation).c_str());
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"%s %s %s",std::to_string(data[0][0]).c_str(),std::to_string(data[0][1]).c_str(),std::to_string(data[0][2]).c_str());
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"%s %s %s",std::to_string(data[1][0]).c_str(),std::to_string(data[1][1]).c_str(),std::to_string(data[1][2]).c_str());
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"%s %s %s",std::to_string(data[2][0]).c_str(),std::to_string(data[2][1]).c_str(),std::to_string(data[2][2]).c_str());
#endif
    return Matrix<float>(3, 3, data);
};