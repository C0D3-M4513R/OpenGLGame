#include <string>
#include <cmath>
#include "Face.h"



template<typename T>
T* Face<T>::getData() {
    //vertexSize of the array is vertexSize*3, because each vertexSize object holds 3 Values(x,y,z)
    T* outData=new T[vertexSize*3+colorSize*3];
    for (unsigned int i=0; i < vertexSize; i++) {
        Vertex<T> src = vertexData[i];
        outData[i * 3]=src[0]+origin.x;
        outData[i * 3 + 1]=src[1]+origin.y;
        outData[i * 3 + 2]=src[2]+origin.z;
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
void Face<T>::move(const uint8_t direction, T amount) {
    if (amount>0) amount = fminf(amount,offset[direction*2]-origin[direction]);
    else amount = fmaxf(amount,offset[direction*2+1]-origin[direction]);
    origin[direction]+=amount;
    if (!dynamic) updateVA();
}

template<typename T>
void Face<T>::recalculateOffset() {
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"Recalculating Offsets");
    for(unsigned int i =0;i < 3;i++){
        T min=origin[i];
        T max=origin[i];
        for(unsigned int j = 0; j < vertexSize; j++){
            const T value = vertexData[j][i];
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
Face<T>::Face(Vertex<T> *vertexData, unsigned int vertexSize, Vertex<T> *colorData, unsigned int colorSize, bool dynamic,
              Vertex<T> origin): vertexData(vertexData),vertexSize(vertexSize),colorData(colorData),colorSize(colorSize),dynamic(dynamic),origin(origin) {
    recalculateOffset();
    triangle=new VertexArray(getData(),vertexSize*3, true,dynamic);
}
template<typename T>
Face<T>::Face(Vertex<T> *vertexData, unsigned int size, Vertex<T> *colorData, bool dynamic,Vertex<T> origin)
    :vertexData(vertexData),vertexSize(size),colorData(colorData),colorSize(size),dynamic(dynamic),origin(origin) {
    recalculateOffset();
    triangle=new VertexArray(getData(),vertexSize*3, true,dynamic);
}
template<typename T>
Face<T>::Face(Vertex<T> *vertexData, unsigned int vertexSize, bool dynamic,
              Vertex<T> origin): vertexData(vertexData),vertexSize(vertexSize),colorData(nullptr),colorSize(0),dynamic(dynamic),origin(origin) {
    recalculateOffset();
    triangle=new VertexArray(getData(),vertexSize*3,false,dynamic);
}

template<typename T>
void Face<T>::updateVA(int mode) {
    switch (mode) {
        case 1:
            triangle->updateData(getData(),vertexSize*3*sizeof(T),colorSize*3*sizeof(T));
        case 0:
            [[fallthrough]];
        default:
            triangle->updateData(getData(),vertexSize*3*sizeof(T),0);
            break;
    }
}

template<typename T>
void Face<T>::Draw(GLenum mode) {
    //i'm gonna update the vertices here, if the object is dynamic
    if(dynamic) updateVA();
    triangle->Draw(mode);
}
