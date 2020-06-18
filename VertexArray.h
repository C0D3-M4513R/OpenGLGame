#ifndef GAME_VERTEXARRAY_H
#define GAME_VERTEXARRAY_H

#include <GL/glew.h>

template <typename T>
class VertexArray
{
public:
    VertexArray(const T* vertPositions,unsigned int vertPositionsCount,GLenum type=GL_FLOAT, bool color = false,bool dynamic = false);
    VertexArray(const float* vertPositions,unsigned int vertPositionsCount, bool color = false,bool dynamic = false):VertexArray(vertPositions,vertPositionsCount,GL_FLOAT,color,dynamic){};
    ~VertexArray();

    void Draw(GLenum mode = GL_TRIANGLES);
    void updateData(const float* data, int size, int offset = 0);

    [[nodiscard]] unsigned int getVertexCount() const;
private:
    // Handles to VBO and VAO.
    GLuint mVBO = GL_NONE;
    GLuint mVAO = GL_NONE;

    unsigned int vertexCount = 0;


};
#include "VertexArray.tpp"

#endif //GAME_VERTEXARRAY_H
