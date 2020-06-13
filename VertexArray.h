#pragma once
#ifndef GAME_VERTEXARRAY_H
#define GAME_VERTEXARRAY_H

#include <GL/glew.h>

class VertexArray
{
public:
    VertexArray(const float* vertPositions, unsigned int vertPositionsCount, bool color = false,bool dynamic = false);
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

#endif //GAME_VERTEXARRAY_H
