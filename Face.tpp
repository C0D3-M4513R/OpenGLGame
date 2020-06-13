#include <string>
#include "Face.h"



template<typename T>
T* Face<T>::getData() {
    //vertexSize of the array is vertexSize*3, because each vertexSize object holds 3 Values(x,y,z)
    T* outData=new T[getDataSize()+colorSize*3];
    for (unsigned int i=0; i < vertexSize; i++) {
        Vertex<T> src = vertexData[i];
        outData[i * 3]=src[0];
        outData[i * 3 + 1]=src[1];
        outData[i * 3 + 2]=src[2];
    }
    for (unsigned int i=vertexSize; i < vertexSize+colorSize; i++) {
        Vertex<T> src = colorData[i-vertexSize];
        outData[i * 3]=src[0];
        outData[i * 3 + 1]=src[1];
        outData[i * 3 + 2]=src[2];
    }
    return outData;
}

template<typename T>
unsigned int Face<T>::getDataSize() {
    return vertexSize*3;
}

template<typename T>
void Face<T>::move(const uint8_t direction, T amount) {
    const bool pos = amount>0;
    T& val = offset[direction*2 + (pos?0:1)];
    T& val2 = offset[direction*2 + (pos?1:0)];
    //I make the assumption here, that all values are between -1.f and 1.f
    if (val == 1.f||val==-1.f) amount=0;
    else if(amount+val>1.f) amount = 1.f-val;
    else if(amount+val<-1.f) amount = -(1.f+val);
    for (unsigned int i=0; i < vertexSize; i++) {
        T& localDir = vertexData[i][direction];
        localDir += amount;
    }
    origin[direction]+=amount;
    val+=amount;
    val2+=amount;

}

template<typename T>
void Face<T>::recalculateOffset() {
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"Recalculating Offsets");
    for(unsigned int i =0;i < 3;i++){
        T min=origin[i];
        T max=origin[i];
        for(unsigned int j = 0; j < vertexSize; j++){
            T value = vertexData[j][i];
            min=value<min?value:min;
            max=value>max?value:max;
        }
        offset[i*2]=max;
        offset[i*2+1]=min;
    }
    SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Offsets are: x:%f,-x: %f y:%f,-y:%f z:%f,-z:%f",offset[0],offset[1],offset[2],offset[3],offset[4],offset[5]);
}
/**
 *
 * @tparam T Type of vertexData
 * @param vertexData Vertex vertexData
 * @param vertexSize vertexSize of vertexData
 * @param origin Middle point of object. MUST LIE INSIDE THE OBJECT. Otherwise the behavior is unpredictable and unreliable;
 */

template<typename T>
Face<T>::Face(Vertex<T> *vertexData, unsigned int vertexSize, Vertex<T> *colorData, unsigned int colorSize,
              Vertex<T> origin): vertexData(vertexData),vertexSize(vertexSize),colorData(colorData),colorSize(colorSize),origin(origin) {
    recalculateOffset();
}
template<typename T>
Face<T>::Face(Vertex<T> *vertexData, unsigned int size, Vertex<T> *colorData,Vertex<T> origin)
    :vertexData(vertexData),vertexSize(size),colorData(colorData),colorSize(size),origin(origin) {
    recalculateOffset();
}
template<typename T>
Face<T>::Face(Vertex<T> *vertexData, unsigned int vertexSize,
              Vertex<T> origin): vertexData(vertexData),vertexSize(vertexSize),colorData(nullptr),colorSize(0),origin(origin) {
    recalculateOffset();
}
