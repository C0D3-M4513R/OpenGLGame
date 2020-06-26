#include <string>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL_log.h>
#include "Face.h"
#include "Renderer.h"
#include "FileSTL/stlio.hpp"

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
    if(!hasColor) SDL_LogError(SDL_LOG_CATEGORY_SYSTEM,"Color data was requested, but there is reportedly none. I'm gonna return all white, to prevent illegal memory-access. hasColor:%s",hasColor?"true":"false");
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
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM,"Normal data was requested, but there is reportedly none. I'm gonna just return a nullptr, to prevent illegal memory-access. hasNormal:%s",hasNormal?"true":"false");
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
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"Moving from %s by probably %s in direction %u, where 0=x,1=y,2=z",std::to_string(origin[direction]).c_str(),std::to_string(amount).c_str(),direction);
    if (amount>0) amount = fminf(amount,offset[direction*2]-origin[direction]);
    else amount = fmaxf(amount,offset[direction*2+1]-origin[direction]);
    origin[direction]+=amount;
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"Moved to %s by a total of %s in direction %u, where 0=x,1=y,2=z",std::to_string(origin[direction]).c_str(),std::to_string(amount).c_str(),direction);
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
    SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM,"Recalculating Offsets");
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
    SDL_LogVerbose(SDL_LOG_CATEGORY_SYSTEM,"Offsets are: x:%f,-x: %f y:%f,-y:%f z:%f,-z:%f",offset[0],offset[1],offset[2],offset[3],offset[4],offset[5]);
}

/**
 *
 * @param vertexData Vertex vertexData
 * @param vertexSize vertexSize of vertexData
 * @param origin Middle point of object. MUST LIE INSIDE THE OBJECT. Otherwise the behavior is unpredictable and unreliable;
 */


Face::Face(glm::vec3* vertexData, unsigned int size,glm::vec3 *colorData, GLenum modeParam,bool dynamic,glm::vec3 origin)
:vertexData(vertexData), size(size), hasColor(colorData!= nullptr), colorData(colorData), hasNormal(false), dynamic(dynamic), origin(origin)
{
    recalculateOffset();
    SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM,"Face: color enabled? %s",hasColor?"yes":"no");
    vertexArray=new VertexArray(getVertData(), size * 3,modeParam);

}

Face::Face(glm::vec3* vertexData, unsigned int vertexSize, GLenum modeParam,bool dynamic, glm::vec3 origin)
:Face(vertexData,vertexSize, nullptr,modeParam,dynamic,origin)
{}

Face::Face(const char *filePath, FILE_TYPE type,bool dynamic) {
    switch (type) {
        case STL:
            std::pair<tyti::stl::solid, bool> tmp = tyti::stl::read(filePath);
            if(!tmp.second) exit(-2);
            size = tmp.first.vertices.size();
            SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM,"vertex size %u",size);
            vertexData = new glm::vec3[size];
            for(unsigned int i =0;i<size;i++){
                tyti::stl::vec3& vecOrig = tmp.first.vertices[i];
                glm::vec3& vec = vertexData[i];
                vec[0]=vecOrig[0];
                vec[1]=vecOrig[1];
                vec[2]=vecOrig[2];
            }

            drawMode=GL_TRIANGLES;
            recalculateOffset();

            hasColor=false;
            colorData = nullptr;
            SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM,"Face: color enabled? %s",hasColor?"yes":"no");
            vertexArray=new VertexArray(getVertData(), size * 3, hasColor, dynamic);
            break;
    }
}

Face::~Face(){
    delete vertexData;
    delete colorData;
    delete vertexArray;
}


void Face::updateVA(int mode) {
    switch (mode) {
        case 1:
            //vertexArray->updateData(getColorData(), size * 3 * sizeof(float), size * 3 * sizeof(float));
            break;
        case 0:
            [[fallthrough]];
        default:
            vertexArray->updateData(getVertData(), size * 3 * sizeof(float), 0);
            break;
    }
}

void Face::Draw() {
    //i'm gonna update the vertices here, if the object is dynamic
    if(dynamic) updateVA();
    Renderer::getShader()->applyMVP(
            glm::translate(//move
                    glm::identity<glm::mat4>(),
                    origin
            )*rotation
    );
    vertexArray->Draw(drawMode);
}