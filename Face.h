#ifndef GAME_FACE_H
#define GAME_FACE_H

#include <utility>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/detail/qualifier.hpp>
#include "VertexArray.h"
enum FILE_TYPE{
    STL
};

class Face{
    public:
        constexpr static GLenum defaultMode = GL_TRIANGLE_STRIP;
        //TODO: scaling, apply move/rotate/scale
        Face(glm::vec3* vertexData, unsigned int vertexSize, GLenum modePar=defaultMode,GLenum type = GL_STATIC_DRAW, glm::vec3 origin = {0, 0, 0});
        Face(glm::vec3* vertexData, unsigned int size,glm::vec3* colorData, GLenum modePar=defaultMode,GLenum type = GL_STATIC_DRAW, glm::vec3 origin = {0, 0, 0});
        Face(const char* filePath,FILE_TYPE=STL,GLenum drawType = GL_STATIC_DRAW, glm::vec3 origin = {0, 0, 0});
        ~Face();

        void moveX(float amount) {move(0,amount);};
        void moveY(float amount) {move(1,amount);};
        void moveZ(float amount) {move(2,amount);};

        //amount is in radians. It is easier to work with
        void rotate(glm::vec3 amount);
        void scale(glm::vec3 amount);

        void Draw() const;
        inline static void DrawAll() {for(Face* face: faces) if(face->draw) face->Draw();};
        void setDraw(bool drawPar=true){draw=drawPar;}
        GLenum drawMode;
    private:
        //all directions accessible from public methods
        void move(uint8_t direction,float amount);

        //why manually request a Offset recalculation
        void recalculateOffset();
        //why expose data? individual vertices shouldn't be needed to be manipulated
        float* getVertData();
        float* getColorData();
        float* getNormalData();

        VertexArray* vertexArray;

        unsigned int size;
        glm::vec3* vertexData;
        bool hasNormal;
        glm::vec3* normalData;
        bool hasColor;
        glm::vec3* colorData;
protected:
        [[maybe_unused]] virtual void positionUpdateCallback(){};

        inline static std::vector<Face*> faces = std::vector<Face*>();
        const unsigned long id=faces.size();
        bool draw;
        //Max value in a direction
        //Direction is in the following order x,-x,y,-y,z,-z
        float offset[6];
        glm::mat4x4 rotation;
        glm::vec3 origin;
        glm::vec3 scaleVec;
};
#endif //GAME_FACE_H