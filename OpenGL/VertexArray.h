#ifndef GAME_VERTEXARRAY_H
#define GAME_VERTEXARRAY_H

#include <GL/glew.h>
#include <vector>

class VertexArray
{
public:

    VertexArray(const float* vertPositions,unsigned int vertPositionsCount,GLenum type = GL_STATIC_DRAW);
    ~VertexArray();

    void addNormals(const float* normalPositions,unsigned int normalPosCount){addComponent(2,normalPositions,normalPosCount);};
    void addColor(const float* colorPositions,unsigned int colorPosCount){addComponent(1,colorPositions,colorPosCount);};

    void Draw(GLenum mode = GL_TRIANGLES);

    [[nodiscard]] unsigned int getVertexCount() const;
private:
    void addComponent(unsigned int attrib,const float* positions,unsigned int positionCount);
    // Handles to VBO and VAO.
    std::vector<GLuint> vbo{GL_NONE};
    GLuint mVAO = GL_NONE;

    unsigned int indecies;

    GLenum type;

};
#endif //GAME_VERTEXARRAY_H