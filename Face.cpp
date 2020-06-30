#include <string>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "Face.h"
#include "Renderer.h"
#include "STLParser/parse_stl.h"

float* Face::getVertData() {
    //vertexSize of the array is vertexSize*3, because each vertexSize object holds 3 Values(x,y,z)
    float* outData=new float[size*3];
    for (unsigned int i=0; i < size; i++) {
        glm::vec3 src = vertexData[i];
        outData[i * 3]=src[0];
        outData[i * 3 + 1]=src[1];
        outData[i * 3 + 2]=src[2];
    }
    return outData;
}
float* Face::getColorData() {
    if(!hasColor) std::cerr<<"Color data was requested, but there is reportedly none. I'm gonna return all white, to prevent illegal memory-access. hasColor:"<<(hasColor?"true":"false")<<newline;
    //vertexSize of the array is vertexSize*3, because each vertexSize object holds 3 Values(x,y,z)
    float* outData=new float[size*3];
    for (unsigned int i=0; i < size; i++) {
        if(hasColor) {
            glm::vec3 src = colorData[i];
            outData[i * 3] = src[0];
            outData[i * 3 + 1] = src[1];
            outData[i * 3 + 2] = src[2];
        } else{
            outData[i * 3] = 1.f;
            outData[i * 3 + 1] = 1.f;
            outData[i * 3 + 2] = 1.f;
        }
    }
    return outData;
}
float* Face::getNormalData() {
    if(!hasNormal) {
        std::cerr<<"Normal data was requested, but there is reportedly none. I'm gonna just return a nullptr, to prevent illegal memory-access. hasNormal:"<<(hasNormal?"true":"false")<<newline;
        return nullptr;
    }
    //vertexSize of the array is vertexSize*3, because each vertexSize object holds 3 Values(x,y,z)
    float* outData=new float[size*3];
    for (unsigned int i=0; i < size; i++) {
        glm::vec3 src = normalData[i];
        outData[i * 3]=src[0];
        outData[i * 3 + 1]=src[1];
        outData[i * 3 + 2]=src[2];
    }
    return outData;
}


void Face::move(const uint8_t direction,float amount) {
    std::cout<<"Moving from "<<origin[direction]<<" by probably "<<amount<<" in direction "<<direction<<", where 0=x,1=y,2=z"<<newline;
    if (amount>0) amount = fminf(amount,offset[direction*2]-origin[direction]);
    else amount = fmaxf(amount,offset[direction*2+1]-origin[direction]);
    origin[direction]+=amount;
    std::cout<<"Moved from "<<origin[direction]<<" by "<<amount<<" in direction "<<direction<<", where 0=x,1=y,2=z"<<newline;
}


void Face::rotate(glm::vec3 amount) {
    glm::vec3 current;
    glm::extractEulerAngleXYZ(rotation,current.x,current.y,current.z);
    rotation=glm::orientate4(amount);
}

void Face::scale(glm::vec3 amount) {
    scaleVec=amount;
}



void Face::recalculateOffset() {
    std::cout<<"Recalculating Offsets"<<newline;
    for(unsigned int i =0;i < 3;i++){
        float min=origin[i];
        float max=origin[i];
        for(unsigned int j = 0; j < size; j++){
            const float value = vertexData[j][i];
            min=value<min?value:min;
            max=value>max?value:max;
        }
        offset[i*2]=max;
        offset[i*2+1]=min;
    }
    std::cout<<"Offsets are: x:"<<offset[0]<<"-x:"<<offset[1]<<newline
            <<"y:"<<offset[2]<<"-y: "<<offset[3]<<newline
            <<"z:"<<offset[4]<<"-z: "<<offset[5]<<newline;
}

/**
 *
 * @param vertexData Vertex vertexData
 * @param vertexSize vertexSize of vertexData
 * @param origin Middle point of object. MUST LIE INSIDE THE OBJECT. Otherwise the behavior is unpredictable and unreliable;
 */


Face::Face(glm::vec3* vertexData, unsigned int size,glm::vec3 *colorData, GLenum modeParam,GLenum type,glm::vec3 origin)
:drawMode(modeParam),
size(size),vertexData(vertexData),
hasNormal(false), normalData(nullptr),
hasColor(colorData!= nullptr), colorData(colorData),
rotation(glm::identity<glm::mat4>()),origin(origin),scaleVec({1,1,1})
{
    recalculateOffset();
    std::cout<<"Face: color enabled? "<<(hasColor?"yes":"no")<<newline;
    vertexArray=new VertexArray(getVertData(), size * 3,type);
    if(hasColor) vertexArray->addColor(getColorData(),size*3);
}

Face::Face(glm::vec3* vertexData, unsigned int vertexSize, GLenum modeParam,GLenum type, glm::vec3 origin)
:Face(vertexData,vertexSize, nullptr,modeParam,type,origin)
{}

Face::Face(const char *filePath, FILE_TYPE fileType,GLenum drawType)
:rotation(glm::identity<glm::mat4>()),origin({0,0,0}),scaleVec({1,1,1}) {
    switch (fileType) {
        case STL:
            stl::stl_data data = stl::parse_stl(filePath);

            size = data.triangles.size()*3;//3 verts in a triangle
            std::cout<<"vertex size"<<size<<newline;
            //create arrays
            vertexData = new glm::vec3[size];
            hasNormal=true;
            normalData = new glm::vec3[size];
            hasColor=false;
            colorData= nullptr;
            //copy data
            for(unsigned int i =0;i<data.triangles.size();i++){
                stl::triangle& triangle = data.triangles[i];

                normalData[i*3] = {triangle.normal.x,triangle.normal.y,triangle.normal.z};
                vertexData[i*3] = {triangle.v1.x,triangle.v1.y,triangle.v1.z};

                normalData[i*3+1] = {triangle.normal.x,triangle.normal.y,triangle.normal.z};
                vertexData[i*3+1] = {triangle.v2.x,triangle.v2.y,triangle.v2.z};

                normalData[i*3+2] = {triangle.normal.x,triangle.normal.y,triangle.normal.z};
                vertexData[i*3+2] = {triangle.v3.x,triangle.v3.y,triangle.v3.z};
                }
            drawMode=GL_TRIANGLES;
            recalculateOffset();

            std::cout<<"Face: color enabled? "<<(hasColor?"yes":"no")<<newline;
            vertexArray=new VertexArray(getVertData(), size * 3, drawType);
            vertexArray->addColor(getVertData(),size*3);
            vertexArray->addNormals(getNormalData(),size*3);
            break;
    }
}

Face::~Face(){
    delete vertexData;
    delete colorData;
    delete normalData;
    delete vertexArray;
}


void Face::Draw() {
    Renderer::getShader().applyMVP(glm::translate(glm::identity<glm::mat4>() ,origin)*rotation);
    vertexArray->Draw(drawMode);
}